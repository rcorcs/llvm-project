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
#include "llvm/Transforms/IPO/FunctionMerging.h"

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

class ClonedFunctionInfo {
public:
  Function* Original{nullptr};
  Function* Cloned{nullptr};
  DominatorTree DT;
  PostDominatorTree PDT;
  ValueToValueMapTy VMap;

  ClonedFunctionInfo(Function* F) : Original(F) {
    Reinitialize();
  }

  ~ClonedFunctionInfo() {
    Invalidate();
  }

  void Reinitialize() {
    if (Cloned == nullptr) {
      VMap.clear();
      Cloned = llvm::CloneFunction(Original, VMap);
      DT = DominatorTree(*Cloned);
      PDT = PostDominatorTree(*Cloned);
    }
  }

  void Invalidate() {
    if (Cloned != nullptr) {
      Cloned->eraseFromParent();
      Cloned = nullptr;
    }
  }

  BasicBlock* getClonedBB(BasicBlock* BB) {
    return llvm::dyn_cast<BasicBlock>(VMap[BB]);
  }

  BranchInst* getClonedBI(BranchInst* BI) {
    return llvm::dyn_cast<BranchInst>(VMap[BI]);
  }
};

static bool runImpl(Function *F, DominatorTree &DT, PostDominatorTree &PDT,
                    LoopInfo &LI, TargetTransformInfo &TTI) {
  errs() << "Procesing function : " << F->getName() << "\n";
  int CFMCount = 0, BFCount = 0;
  bool LocalChange = false, Changed = false;

  int OrigCodeSize = EstimateFunctionSize(F, TTI);

  std::set<BasicBlock*> VisitedBBs;

  do {
    LocalChange = false;
    ClonedFunctionInfo CFMFunc(F);
    ClonedFunctionInfo BFFunc(F);

    for (BasicBlock *BB : post_order(&F->getEntryBlock())) {
      if (VisitedBBs.count(BB)) continue;
      VisitedBBs.insert(BB);

      BranchInst *BI = dyn_cast<BranchInst>(BB->getTerminator());
      if (BI && BI->isConditional()) {
        int BeforeSize = EstimateFunctionSize(F, TTI);

        int CFMProfit = 0;
        SmallVector<unsigned> ProfitableIdxs;
        if (RunCFMOnly || !RunBFOnly) {
          // clone and run cfmelder
          CFMFunc.Reinitialize();
          SmallVector<unsigned> EmptyIdxs; // run on all region matches
          ProfitableIdxs = runCFM(CFMFunc.getClonedBB(BB), CFMFunc.DT,
              CFMFunc.PDT, TTI, EmptyIdxs);
          // compute CFM code size reduction
          CFMProfit = BeforeSize - EstimateFunctionSize(CFMFunc.Cloned, TTI);
          if (ProfitableIdxs.size() > 0)
            CFMFunc.Invalidate();
        }
        errs() << "CFM code reduction : " << CFMProfit << "\n";

        int BFProfit = 0;
        if (!RunCFMOnly || RunBFOnly) {
          // clone and run brfusion
          BFFunc.Reinitialize();
          bool BFSuccess = MergeBranchRegions(*(BFFunc.Cloned),
              BFFunc.getClonedBI(BI), BFFunc.DT, TTI, false);
          // compute BF code size reduction
          BFProfit =
              BFSuccess ? BeforeSize - EstimateFunctionSize(BFFunc.Cloned, TTI) : 0;
          errs() << "Branch fusion code reduction : " << BFProfit << "\n";
          if (BFSuccess)
            BFFunc.Invalidate();
        }

        // pick best one and run on original function if profitable
        if (BFProfit > 0 || CFMProfit > 0) {
          if (BFProfit > CFMProfit) {
            errs() << "Profitable Branch Fusion: SEME-brfusion " << BB->getName().str() << ": "; BI->dump();
            MergeBranchRegions(*F, BI, DT, TTI, true);
            BFCount++;
          } else if (CFMProfit > 0) {
            // run on profitable idxs only
            errs() << "Profitable Branch Fusion: CFMelder " << BB->getName().str() << ": "; BI->dump();
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

    int FinalCodeSize = EstimateFunctionSize(F, TTI);
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

bool HybridBranchFusionLegacyPass::runOnModule(Module &M) { 
  errs() << "HybridBranchFusionLegacyPass not implemented\n";
  return false;
}

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
