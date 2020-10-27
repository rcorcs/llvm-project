//===- LoopRolling.h - Loop rolling pass ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPROLLING_H
#define LLVM_TRANSFORMS_SCALAR_LOOPROLLING_H

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

#include <vector>
#include <map>

namespace llvm {

class Function;

class SeedGroups {
public:
  std::map<Value *, std::vector<Instruction *> > Stores;
  std::map<Value *, std::vector<Instruction *> > Calls;

  void clear() {
    Stores.clear();
    Calls.clear();
  }
};


class LoopRolling : public PassInfoMixin<LoopRolling> {
public:
  /// Run the pass over the function.
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  bool runImpl(Function &F);

private:
  void collectSeedInstructions(BasicBlock &BB);
  //void codeGeneration(Tree &T, BasicBlock &BB);
  

  SeedGroups Seeds;
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPROLLING_H
