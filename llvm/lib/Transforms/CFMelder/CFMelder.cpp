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
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/CGSCCPassManager.h"
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
#include "llvm/Transforms/IPO/FunctionMerging.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
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

static cl::opt<unsigned>
    MaxIterations("cfmelding-max-iteration", cl::init(10), cl::Hidden,
                  cl::desc("Maximum number of iterations performed by CFMelder "
                           "on the whole function"));
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

class CFMelderCodeSizeLegacyPass : public ModulePass {
public:
  static char ID;

  CFMelderCodeSizeLegacyPass() : ModulePass(ID) {
    initializeCFMelderCodeSizeLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  bool runOnModule(Module &M) override;
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

  ControlFlowGraphInfo CFGInfo(F, DT, PDT, TTI);

  for (auto BBIt : post_order(&F.getEntryBlock())) {
    BasicBlock &BB = *BBIt;
    // check if this BB is the enrty to a diamond shaped control-flow
    Value *BranchI = dyn_cast<Value>(BB.getTerminator());
    if (Utils::isValidMergeLocation(BB, DT, PDT) &&
        (GPUDA->isDivergent(*BranchI) || RunCFMelderAnalysisOnly)) {

      // DebugLoc DebugLocation = BB.begin()->getDebugLoc();
      INFO << "------------------------------------------------------------"
              "-------------------\n";
      INFO << "Valid merge location found at BB ";
      BB.printAsOperand(errs(), false);
      errs() << "\n";
      RegionAnalyzer MA(&BB, CFGInfo);
      MA.computeRegionMatch();
      MA.printAnalysis(INFO);
      INFO << "This merge is : " << (MA.hasAnyProfitableMatch() ? "" : "NOT")
           << " PROFITABLE!\n";
    }
  }

  return false;
}

static bool simplifyFunction(Function &F, TargetTransformInfo &TTI,
                             SimplifyCFGOptions &Options) {
  bool Changed = false;
  bool LocalChange = false;
  do {
    LocalChange = false;
    for (auto &BB : make_range(F.begin(), F.end())) {
      if (simplifyCFG(&BB, TTI, nullptr, Options)) {
        LocalChange = true;
        break;
      }
    }

    Changed |= LocalChange;

  } while (LocalChange);
  return Changed;
}

/*
static int computeCodeSize(Function *F, TargetTransformInfo &TTI) {
  float CodeSize = 0;
  for (Instruction &I : instructions(*F)) {
    switch(I.getOpcode()) {
    case Instruction::PHI:
      CodeSize += 0.2;
      break;
    default:
      CodeSize += TTI.getInstructionCost(
                       &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize)
                    .getValue()
                    .getValue();
    }
  }
  return CodeSize;
}
*/

SmallVector<unsigned> llvm::runCFM(BasicBlock *BB, DominatorTree &DT,
                                   PostDominatorTree &PDT,
                                   TargetTransformInfo &TTI,
                                   SmallVector<unsigned> &OnIdxs) {
  bool LocalChange = false, AnyChange = false;
  SmallVector<unsigned> ProfitableIdxs;
  if (Utils::isValidMergeLocation(*BB, DT, PDT)) {
    Function *Func = BB->getParent();
    ControlFlowGraphInfo CFGInfo(*Func, DT, PDT, TTI);
    RegionAnalyzer RA(BB, CFGInfo);
    RA.computeRegionMatch();

    // if OnIdxs is non-empty apply melding to those indexes only
    SmallVector<unsigned> IdxsToProcess;
    if (!OnIdxs.empty()) {
      for (unsigned I : OnIdxs)
        IdxsToProcess.push_back(I);
    } else {
      for (unsigned I = 0; I < RA.regionMatchSize(); ++I)
        IdxsToProcess.push_back(I);
    }

    if (RA.hasAnyProfitableMatch()) {

      for (unsigned I : IdxsToProcess) {
        if (!RA.isRegionMatchProfitable(I))
          continue;

        int SizeBefore = EstimateFunctionSize(Func, TTI);
        RegionMelder RM(RA);
        RM.merge(I);
        int SizeAfter = EstimateFunctionSize(Func, TTI);

        if (SizeBefore > SizeAfter) {
          ProfitableIdxs.push_back(I);
        }
        LocalChange = true;
      }

      if (LocalChange) {
        DT.recalculate(*Func);
        PDT.recalculate(*Func);
        AnyChange = true;
        LocalChange = false;
      }
    }
  }

  // PP: What I really want is to return whether this call modified Func
  // Could pass-by-ref a bool for that but I am lazy so I add garbage to
  // ProfitableIdxs to make sure that it's always non-empty when Func was
  // modified
  if (AnyChange && ProfitableIdxs.size() == 0)
    ProfitableIdxs.push_back(UINT_MAX);
  return ProfitableIdxs;
}

static bool runImplCodeSize(Function &F, DominatorTree &DT,
                            PostDominatorTree &PDT, LoopInfo &LI,
                            TargetTransformInfo &TTI) {
  INFO << "Procesing function : " << F.getName() << "\n";
  Function *Func = &F;
  bool LocalChange = false, Changed = false;

  int OrigCodeSize = EstimateFunctionSize(&F, TTI);
  unsigned CountIter = 0;

  do {
    CountIter++;

    LocalChange = false;
    for (BasicBlock *BB : post_order(&Func->getEntryBlock())) {
      if (Utils::isValidMergeLocation(*BB, DT, PDT)) {
        INFO << "Valid merge location found at block "
             << BB->getNameOrAsOperand() << "\n";
        ControlFlowGraphInfo CFGInfo(*Func, DT, PDT, TTI);
        RegionAnalyzer RA(BB, CFGInfo);
        RA.computeRegionMatch();

        if (RA.hasAnyProfitableMatch()) {

          // Store the indexes of profitable merges
          SmallVector<int, 8> Profitable;

          // clone function
          ValueToValueMapTy VMap;
          Function *ClonedFunc = CloneFunction(Func, VMap);
          DominatorTree ClonedDT(*ClonedFunc);
          PostDominatorTree ClonedPDT(*ClonedFunc);
          ControlFlowGraphInfo ClonedCFGInfo(*ClonedFunc, ClonedDT, ClonedPDT,
                                             TTI);

          RegionAnalyzer ClonedRA(dyn_cast<BasicBlock>(VMap[BB]),
                                  ClonedCFGInfo);
          ClonedRA.computeRegionMatch();

          for (unsigned I = 0; I < ClonedRA.regionMatchSize(); ++I) {
            if (!ClonedRA.isRegionMatchProfitable(I))
              continue;

            int SizeBefore = EstimateFunctionSize(ClonedFunc, TTI);
            RegionMelder ClonedRM(ClonedRA);
            ClonedRM.merge(I);
            int SizeAfter = EstimateFunctionSize(ClonedFunc, TTI);
            DEBUG << "Size changed from " << SizeBefore << " to " << SizeAfter
                  << " : " << (SizeBefore - SizeAfter) << " : "
                  << ((SizeBefore > SizeAfter) ? "Profitable" : "Unprofitable")
                  << " Branch Fusion! [" << F.getName().str() << "] ";
            BB->getTerminator()->dump();
            if (SizeBefore > SizeAfter) {
              Profitable.push_back(I);
            }
          }

          // If there are profitble merges perform them on Func
          if (!Profitable.empty()) {
            for (int I : Profitable) {
              RegionMelder RM(RA);
              RM.merge(I);
            }
            LocalChange = true;
          }

          // delete the cloned functon
          ClonedFunc->eraseFromParent();

          if (LocalChange) {
            DT.recalculate(*Func);
            PDT.recalculate(*Func);
            break;
          }
        }
      }
    }

    Changed |= LocalChange;

    if (RunMeldingOnce)
      break;
  } while (LocalChange); // && CountIter < MaxIterations);

  if (Changed) {
    // simplifyFunction(
    //             *Func, TTI,
    //             SimplifyCFGOptionsObj.setSimplifyCondBranch(false));

    int FinalCodeSize = EstimateFunctionSize(&F, TTI);
    double PercentReduction =
        (OrigCodeSize - FinalCodeSize) * 100 / (double)OrigCodeSize;
    INFO << "Size reduction for function " << F.getName() << ": "
         << OrigCodeSize << " to  " << FinalCodeSize << " (" << PercentReduction
         << "%)"
         << "\n";
  }

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

  // simplifyCFG options
  SimplifyCFGOptions SimplifyCFGOptionsObj;

  bool Changed = false, LocalChange = false;
  unsigned CountIter = 0;

  do {
    LocalChange = false;
    CountIter++;

    for (auto &BBIt : post_order(&F.getEntryBlock())) {
      BasicBlock &BB = *BBIt;
      // errs() << "BB name : " << BB.getNameOrAsOperand() << "\n";
      // check if this BB is the enrty to a diamond shaped control-flow
      if (Utils::isValidMergeLocation(BB, DT, PDT)) {
        INFO << "Valid merge location found at BB ";
        BB.printAsOperand(errs(), false);
        errs() << "\n";

        ControlFlowGraphInfo CFGInfo(F, DT, PDT, TTI);
        RegionAnalyzer MA(&BB, CFGInfo);
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
            CFGInfo.recompute();
          }
          if (LocalChange) {
            if (!NoSimplifyCFGAfterMelding) {
              INFO << "Running CFG simplification\n";
              if (simplifyFunction(
                      F, TTI,
                      SimplifyCFGOptionsObj.setSimplifyCondBranch(false)
                          .sinkCommonInsts(false)
                          .hoistCommonInsts(false))) {
                DT.recalculate(F);
                PDT.recalculate(F);
              }
            }
            break;
          }
        }
      }
    }

    Changed |= LocalChange;
    // if one melding is requested, exit (debugging)
    if (RunMeldingOnce) {
      break;
    }
  } while (LocalChange); // && CountIter < MaxIterations);

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

  return PreservedAnalyses::none();
}

bool CFMelderCodeSizeLegacyPass::runOnModule(Module &M) {
  errs() << "Hello\n";
  return false;
}

void CFMelderCodeSizeLegacyPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<TargetTransformInfoWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
}

PreservedAnalyses CFMelderCodeSizePass::run(Module &M,
                                            ModuleAnalysisManager &MAM) {
  auto &FAM = MAM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();
  bool Changed = false;
  SmallVector<Function *, 64> Funcs;

  for (auto &F : M) {
    if (F.isDeclaration())
      continue;
    Funcs.push_back(&F);
  }

  for (Function *F : Funcs) {
    auto &DT = FAM.getResult<DominatorTreeAnalysis>(*F);
    auto &PDT = FAM.getResult<PostDominatorTreeAnalysis>(*F);
    auto &TTI = FAM.getResult<TargetIRAnalysis>(*F);
    auto &LI = FAM.getResult<LoopAnalysis>(*F);
    Changed |= runImplCodeSize(*F, DT, PDT, LI, TTI);
  }
  if (!Changed)
    return PreservedAnalyses::all();
  PreservedAnalyses PA;
  return PA;
}

char CFMelderLegacyPass::ID = 0;

INITIALIZE_PASS_BEGIN(CFMelderLegacyPass, "cfmelder",
                      "Meld similar control-flow", false, false)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(CFMelderLegacyPass, "cfmelder", "Meld similar control-flow",
                    false, false)

char CFMelderCodeSizeLegacyPass::ID = 0;

INITIALIZE_PASS_BEGIN(CFMelderCodeSizeLegacyPass, "cfmelder-codesize",
                      "Meld similar control-flow for code size reduction",
                      false, false)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(CFMelderCodeSizeLegacyPass, "cfmelder-codesize",
                    "Meld similar control-flow for code size reduction", false,
                    false)

// Initialization Routines
void llvm::initializeCFMelder(PassRegistry &Registry) {
  initializeCFMelderLegacyPassPass(Registry);
}

void llvm::initializeCFMelderCodeSize(PassRegistry &Registry) {
  initializeCFMelderCodeSizeLegacyPassPass(Registry);
}

FunctionPass *llvm::createCFMelderPass() { return new CFMelderLegacyPass(); }

ModulePass *llvm::createCFMelderCodeSizePass() {
  return new CFMelderCodeSizeLegacyPass();
}
