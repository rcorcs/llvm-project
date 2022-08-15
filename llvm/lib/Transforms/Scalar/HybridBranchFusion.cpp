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
#include "llvm/Transforms/Scalar/HybridBranchFusion.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/InitializePasses.h"
#include "llvm/Transforms/CFMelder/CFMelder.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/BranchFusion.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

using namespace llvm;

#define DEBUG_TYPE "hybrid-brfusion"

static cl::opt<bool> RunCFMOnly("run-cfm-only", cl::init(false), cl::Hidden,
                                cl::desc("Only run control-flow melding"));

static cl::opt<bool> RunBFOnly("run-brfusion-only", cl::init(false), cl::Hidden,
                               cl::desc("Only run branch fusion"));

namespace {

class HybridBranchFusionLegacyPass : public ModulePass {

public:
  static char ID;

  HybridBranchFusionLegacyPass() : ModulePass(ID) {
    initializeHybridBranchFusionLegacyPassPass(
        *PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  bool runOnModule(Module &M) override;
};

} // namespace

static int computeCodeSize(Function *F, TargetTransformInfo &TTI) {
  int CodeSize = 0;
  for (Instruction &I : instructions(*F)) {
    CodeSize += TTI.getInstructionCost(
                       &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize)
                    .getValue()
                    .getValue();
  }
  return CodeSize;
}

static bool runImpl(Function *F, DominatorTree &DT, PostDominatorTree &PDT,
                    LoopInfo &LI, TargetTransformInfo &TTI) {
  errs() << "Procesing function : " << F->getName() << "\n";
  int CFMCount = 0, BFCount = 0;
  bool LocalChange = false, Changed = false;

  int OrigCodeSize = computeCodeSize(F, TTI);

  do {
    LocalChange = false;
    for (BasicBlock *BB : post_order(&F->getEntryBlock())) {
      BranchInst *BI = dyn_cast<BranchInst>(BB->getTerminator());
      if (BI && BI->isConditional()) {
        int BeforeSize = computeCodeSize(F, TTI);
        // clone and run cfmelder
        ValueToValueMapTy CFMVMap;
        Function *CFMFunc = CloneFunction(F, CFMVMap);
        DominatorTree CFMDT(*CFMFunc);
        PostDominatorTree CFMPDT(*CFMFunc);
        SmallVector<unsigned> EmptyIdxs; // run on all region matches
        auto ProfitableIdxs = runCFM(dyn_cast<BasicBlock>(CFMVMap[BB]), CFMDT,
                                     CFMPDT, TTI, EmptyIdxs);
        // compute CFM code size reduction
        int CFMProfit = BeforeSize - computeCodeSize(CFMFunc, TTI);
        errs() << "CFM code reduction : " << CFMProfit << "\n";

        // clone and run brfusion
        ValueToValueMapTy BFVMap;
        Function *BFFunc = CloneFunction(F, BFVMap);
        DominatorTree BFDT(*BFFunc);
        PostDominatorTree BFPDT(*BFFunc);
        bool BFSuccess = MergeBranchRegions(
            *BFFunc, dyn_cast<BranchInst>(BFVMap[BI]), BFDT, TTI);
        // compute BF code size reduction
        int BFProfit =
            BFSuccess ? BeforeSize - computeCodeSize(BFFunc, TTI) : 0;
        errs() << "Branch fusion code reduction : " << BFProfit << "\n";

            // pick best one and run on original function if profitable
        if (BFProfit > 0 || CFMProfit > 0) {
          if (BFProfit > CFMProfit) {
            errs() << "Apply branch fusion to orig function\n";
            MergeBranchRegions(*F, BI, DT, TTI);
            BFCount++;
          } else {
            // run on profitable idxs only
            errs() << "Apply CFM to orig function\n";
            runCFM(BB, DT, PDT, TTI, ProfitableIdxs);
            CFMCount++;
          }
          LocalChange = true;
        }

        if (LocalChange) {
          DT.recalculate(*F);
          PDT.recalculate(*F);
          break;
        }
      }
    }
    Changed |= LocalChange;
  } while (LocalChange);

  if (Changed) {

    int FinalCodeSize = computeCodeSize(F, TTI);
    double PercentReduction =
        (OrigCodeSize - FinalCodeSize) * 100 / (double)OrigCodeSize;
    errs() << "Size reduction for function " << F->getName() << ": "
           << OrigCodeSize << " to  " << FinalCodeSize << " ("
           << PercentReduction << "%)"
           << "\n";
    errs() << "Brach fusion applied " << BFCount << " times and CFM applied "
           << CFMCount << " times\n";
  }

  return Changed;
}

bool HybridBranchFusionLegacyPass::runOnModule(Module &M) { return false; }

void HybridBranchFusionLegacyPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<TargetTransformInfoWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
}

PreservedAnalyses
HybridBranchFusionModulePass::run(Module &M, ModuleAnalysisManager &MAM) {
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
    Changed |= runImpl(F, DT, PDT, LI, TTI);
  }
  if (!Changed)
    return PreservedAnalyses::all();
  PreservedAnalyses PA;
  return PA;
}

char HybridBranchFusionLegacyPass::ID = 0;

INITIALIZE_PASS_BEGIN(HybridBranchFusionLegacyPass, "hybrid-brfusion",
                      "Hybrid branch fusion for code size", false, false)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(HybridBranchFusionLegacyPass, "hybrid-brfusion",
                    "Hybrid branch fusion for code size", false, false)

// Initialization Routines
void llvm::initializeHybridBranchFusion(PassRegistry &Registry) {
  initializeHybridBranchFusionLegacyPassPass(Registry);
}

ModulePass *llvm::createHybridBranchFusionModulePass() {
  return new HybridBranchFusionLegacyPass();
}
