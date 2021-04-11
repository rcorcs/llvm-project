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
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

#include <vector>
#include <map>

namespace llvm {

class Function;

class LoopRolling : public PassInfoMixin<LoopRolling> {
public:
  /// Run the pass over the function.
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  bool runImpl(Function &F, ScalarEvolution *SE);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPROLLING_H
