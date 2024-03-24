//===- BranchFusion.h - A function merging pass ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the branch fusion optimization.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_BRANCHFUSION_H
#define LLVM_TRANSFORMS_SCALAR_BRANCHFUSION_H

#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Dominators.h"


#include <map>
#include <vector>

namespace llvm {

class BranchFusionPass : public PassInfoMixin<BranchFusionPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};


// Module pass to drive control-flow melding
class BranchFusionModulePass : public PassInfoMixin<BranchFusionModulePass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

//ModulePass *createBranchFusionModulePass();

bool MergeBranchRegions(Function &F, BranchInst *BI, DominatorTree &DT,
           TargetTransformInfo &TTI, bool RunCleanup);

} // namespace llvm
#endif
