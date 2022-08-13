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

#ifndef LLVM_TRANSFORMS_SCALAR_HYBRIDBRANCHFUSION_H
#define LLVM_TRANSFORMS_SCALAR_HYBRIDBRANCHFUSION_H

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

// Module pass to drive branch fusion and CFM
class HybridBranchFusionModulePass
    : public PassInfoMixin<HybridBranchFusionModulePass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

// ModulePass *createHybridBranchFusionModulePass();

} // namespace llvm
#endif
