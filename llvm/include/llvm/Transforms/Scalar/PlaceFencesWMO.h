//===- PlaceFencesWMO.cpp - Place fences for WMO ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
// This pass places fences when retargetting from a TSO architecture to one
// with a weark memory ordering memory model.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_PLACEFENCESWMO_H
#define LLVM_TRANSFORMS_SCALAR_PLACEFENCESWMO_H

#include "llvm/IR/PassManager.h"

namespace llvm {

/// A pass that lowers atomic intrinsic into non-atomic intrinsics.
class PlaceFencesWMOPass : public PassInfoMixin<PlaceFencesWMOPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &);
  //static bool isRequired() { return true; }
};
}

#endif // LLVM_TRANSFORMS_SCALAR_PLACEFENCESWMO_H
~                                                       
