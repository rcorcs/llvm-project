//===- Transforms/FunctionMerging.h - function merging passes  ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
/// \file

//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_FUNCTION_CLONING_OPT_H
#define LLVM_TRANSFORMS_FUNCTION_CLONING_OPT_H

#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DemandedBits.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/InitializePasses.h"

#include "llvm/InitializePasses.h"
#include <llvm/Analysis/DependenceAnalysis.h>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constant.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Analysis/Passes.h"
#include "llvm/IR/PassManager.h"

namespace llvm{

class FunctionCloning {
public:
  bool runOnModule(Module &M);
};

class FunctionCloningLegacyPass : public ModulePass {
public:
  static char ID;
  FunctionCloningLegacyPass() : ModulePass(ID) {
     initializeFunctionCloningLegacyPassPass(*PassRegistry::getPassRegistry());
  }
  bool runOnModule(Module &M) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override;
};

class FunctionCloningPass : public PassInfoMixin<FunctionCloningPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};



} // namespace
#endif

