//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//
#include "llvm/Transforms/CFMelder/CFMelder.h"
#include "CFMelderUtils.h"
#include "RegionMelder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/DivergenceAnalysis.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Analysis/DominanceFrontierImpl.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Analysis/RegionInfoImpl.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Frontend/OpenMP/OMP.h.inc"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "cfmelder"

static cl::opt<bool>
    ForceMerging("force-cf-merging", cl::init(false), cl::Hidden,
                 cl::desc("Always perform merging regardless of target"));

static cl::opt<bool>
    RunCFMelderAnalysisOnly("cfmelder-analysis-only", cl::init(false),
                            cl::Hidden,
                            cl::desc("Run control-melding analysis only"));
// static cl::opt<bool>
//     DisableRecursiveMelding("disable-recursive-melding", cl::init(false),
//     cl::Hidden,
//                  cl::desc("Disable recurisve melding"));

static cl::opt<std::string> RunCFMeldingOnlyOnFunction(
    "run-cfmelding-on-function", cl::init(""), cl::Hidden,
    cl::desc("Limit CFMelding for this function only"));

static cl::opt<bool> NoSimplifyCFGAfterMelding(
    "no-simplifycfg-after-melding", cl::init(false), cl::Hidden,
    cl::desc("Do not simplify the CFG after control-flow melding"));

static cl::opt<bool> RunMeldingOnce("run-cfmelding-once", cl::init(false),
                                    cl::Hidden,
                                    cl::desc("Perform one melding and exit"));

namespace {

class CFMelderLegacyPass : public FunctionPass {

public:
  static char ID;

  CFMelderLegacyPass() : FunctionPass(ID) {
    initializeCFMelderLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  bool runOnFunction(Function &F) override;
};
} // namespace

static bool runAnalysisOnly(Function &F, DominatorTree &DT,
                            PostDominatorTree &PDT, LoopInfo &LI,
                            TargetTransformInfo &TTI) {

  auto GPUDA = std::make_unique<DivergenceInfo>(F, DT, PDT, LI, TTI,
                                                /* KnownReducible = */ false);

  INFO << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
          "+++++++++++++++\n";
  INFO << "Function name : " << F.getName() << "\n";
  INFO << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
          "+++++++++++++++\n";

  // loop over BBs
  for (auto &BB : F) {
    // check if this BB is the enrty to a diamond shaped control-flow
    Value *BranchI = dyn_cast<Value>(BB.getTerminator());
    if (Utils::isValidMergeLocation(BB, PDT) &&
        (GPUDA->isDivergent(*BranchI) || RunCFMelderAnalysisOnly)) {

      // DebugLoc DebugLocation = BB.begin()->getDebugLoc();
      INFO << "------------------------------------------------------------"
              "-------------------\n";
      INFO << "Valid merge location found at BB ";
      BB.printAsOperand(errs(), false);
      errs() << "\n";
      RegionAnalyzer MA(&BB, DT, PDT);
      MA.computeRegionMatch();
      MA.printAnalysis(INFO);
      INFO << "This merge is : " << (MA.hasAnyProfitableMatch() ? "" : "NOT")
           << " PROFITABLE!\n";
    }
  }

  return false;
}

static bool simplifyFunction(Function &F, TargetTransformInfo &TTI) {
  bool Changed = false;
  bool LocalChange = false;
  do {
    LocalChange = false;
    for (auto &BB : make_range(F.begin(), F.end())) {
      if (simplifyCFG(&BB, TTI, nullptr,
                      SimplifyCFGOptions()
                          .setSimplifyCondBranch(false)
                          .sinkCommonInsts(false)
                          .hoistCommonInsts(false))) {
        LocalChange = true;
        break;
      }
    }

    Changed |= LocalChange;

  } while (LocalChange);
  return Changed;
}

static bool runImpl(Function &F, DominatorTree &DT, PostDominatorTree &PDT,
                    LoopInfo &LI, TargetTransformInfo &TTI) {

  // if the target does not has branch divergence, just exit
  if (!TTI.hasBranchDivergence() && !ForceMerging)
    return false;

  // analysis only
  if (RunCFMelderAnalysisOnly) {
    return runAnalysisOnly(F, DT, PDT, LI, TTI);
  }

  INFO << "Function name : " << F.getName() << "\n";

  if (RunCFMeldingOnlyOnFunction.size() > 0) {
    std::stringstream RunOnlyFuncs(RunCFMeldingOnlyOnFunction);
    std::set<string> FuncNames;

    while (RunOnlyFuncs.good()) {
      std::string Fs;
      std::getline(RunOnlyFuncs, Fs, ';');
      FuncNames.insert(Fs);
    }

    bool ContainsName = false;
    for (auto FN : FuncNames) {
      if (F.getName().contains(FN)) {
        ContainsName = true;
        break;
      }
    }
    if (!ContainsName)
      return false;
  }

  // if (RunCFMeldingOnlyOnFunction.size() > 0 &&
  // !F.getName().contains(RunCFMeldingOnlyOnFunction))
  //   return false;

  bool Changed = false, LocalChange = false;

  do {
    LocalChange = false;

    for (auto &BB : F) {
      // check if this BB is the enrty to a diamond shaped control-flow
      if (Utils::isValidMergeLocation(BB, PDT)) {
        INFO << "Valid merge location found at BB ";
        BB.printAsOperand(errs(), false);
        errs() << "\n";

        RegionAnalyzer MA(&BB, DT, PDT);
        MA.computeRegionMatch();
        if (MA.hasAnyProfitableMatch()) {
          INFO << "Melding is profitable\n";

          for (unsigned I = 0; I < MA.regionMatchSize(); I++) {
            // skip unprofitable melding
            if (!MA.isRegionMatchProfitable(I))
              continue;
            RegionMelder RM(MA);
            RM.merge(I);
            LocalChange = true;
            MA.recomputeControlFlowAnalyses();
          }
          if (LocalChange) {
            if (!NoSimplifyCFGAfterMelding) {
              INFO << "Running CFG simplification\n";
              if (simplifyFunction(F, TTI)) {
                DT.recalculate(F);
                PDT.recalculate(F);
              }
            }
            break;
          }
        }
      }
    }
    // if one melding is requested, exit (debugging)
    if (RunMeldingOnce) {
      break;
    }

    Changed |= LocalChange;

  } while (LocalChange);

  return Changed;
}

bool CFMelderLegacyPass::runOnFunction(Function &F) {
  auto &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  auto &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
  auto &TTI = getAnalysis<TargetTransformInfoWrapperPass>().getTTI(F);
  auto &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  return runImpl(F, DT, PDT, LI, TTI);
}

void CFMelderLegacyPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<TargetTransformInfoWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
}

PreservedAnalyses CFMelderPass::run(Function &F, FunctionAnalysisManager &AM) {

  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  auto &LI = AM.getResult<LoopAnalysis>(F);

  if (!runImpl(F, DT, PDT, LI, TTI)) {
    // No changes, all analyses are preserved.
    return PreservedAnalyses::all();
  }

  PreservedAnalyses PA;
  return PA;
}

char CFMelderLegacyPass::ID = 0;
// static RegisterPass<CFMelderLegacyPass>
//     Y("cfmelder_", "Merge similar control flow for divergence reduction");

INITIALIZE_PASS_BEGIN(CFMelderLegacyPass, "cfmelder",
                      "Meld similar control-flow", false, false)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(CFMelderLegacyPass, "cfmelder", "Meld similar control-flow",
                    false, false)

// Initialization Routines
void llvm::initializeCFMelder(PassRegistry &Registry) {
  initializeCFMelderLegacyPassPass(Registry);
}

// void LLVMInitializeAggressiveInstCombiner(LLVMPassRegistryRef R) {
//   initializeAggressiveInstCombinerLegacyPassPass(*unwrap(R));
// }

FunctionPass *llvm::createCFMelderPass() { return new CFMelderLegacyPass(); }

// void LLVMAddAggressiveInstCombinerPass(LLVMPassManagerRef PM) {
//   unwrap(PM)->add(createAggressiveInstCombinerPass());
// }

bool llvm::runCFMelderPass(Function &F, DominatorTree &DT,
                           PostDominatorTree &PDT, LoopInfo &LI,
                           TargetTransformInfo &TTI) {
  return runImpl(F, DT, PDT, LI, TTI);
}
