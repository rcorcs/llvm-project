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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Scalar.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>
#include <string>

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

bool HybridBranchFusionLegacyPass::runOnModule(Module &M) {
  return false;
}

void HybridBranchFusionLegacyPass::getAnalysisUsage(
    AnalysisUsage &AU) const {
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<TargetTransformInfoWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
}

PreservedAnalyses
HybridBranchFusionModulePass::run(Module &M, ModuleAnalysisManager &MAM) {
  errs() << "Hello Pass\n";

  return PreservedAnalyses::all();
}

char HybridBranchFusionLegacyPass::ID = 0;

INITIALIZE_PASS_BEGIN(HybridBranchFusionLegacyPass, "hybrid-brfusion",
                      "Hybrid branch fusion for code size", false, false)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(HybridBranchFusionLegacyPass, "hybrid-brfusion", "Hybrid branch fusion for code size",
                    false, false)


// Initialization Routines
void llvm::initializeHybridBranchFusion(PassRegistry &Registry) {
  initializeHybridBranchFusionLegacyPassPass(Registry);
}

ModulePass *llvm::createHybridBranchFusionModulePass() { return new HybridBranchFusionLegacyPass(); }

