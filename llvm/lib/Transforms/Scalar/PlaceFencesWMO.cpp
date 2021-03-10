//===- PlaceFencesWMO.cpp - Place fences for WMO --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass lowers atomic intrinsics to non-atomic form for use in a known
// non-preemptible environment.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Scalar/PlaceFencesWMO.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Transforms/Scalar.h"
using namespace llvm;

#define DEBUG_TYPE "placefenceswmo"


static cl::opt<int>
    PlacementMethod("placement-method", cl::init(0), cl::Hidden,
                     cl::desc("Placement method: 0 - naive; 1 - basic."));

static bool naivePlaceFences(Function &F) {
  for (Instruction &I : instructions(&F)) {
    switch (I.getOpcode()) {
    case Instruction::Load:
    case Instruction::Store: {
      IRBuilder<> Builder(&I);
      Builder.CreateFence(AtomicOrdering::SequentiallyConsistent);
    } break;
    default: break;
    }
  }
  return true;
}

static bool basicPlaceFences(Function &F) {
  for (Instruction &I : instructions(&F)) {
    Value *Ptr = nullptr;
    if (LoadInst *LI = dyn_cast<LoadInst>(&I)) {
      Ptr = getUnderlyingObject(LI->getPointerOperand());
    }
    if (StoreInst *SI = dyn_cast<StoreInst>(&I)) {
      Ptr = getUnderlyingObject(SI->getPointerOperand());
    }
    if (Ptr) Ptr->dump();
    if (Ptr && !isa<AllocaInst>(Ptr)) {
      IRBuilder<> Builder(&I);
      Builder.CreateFence(AtomicOrdering::SequentiallyConsistent);
    }
  }
  return true;
}



PreservedAnalyses PlaceFencesWMOPass::run(Function &F, FunctionAnalysisManager &) {
  bool Modified = false;
  switch(PlacementMethod) {
  case 0:
    Modified = naivePlaceFences(F);
    break;
  case 1:
    Modified = basicPlaceFences(F);
    break;
  default: break;
  }
  if (Modified)
    return PreservedAnalyses::none();
  return PreservedAnalyses::all();
}

namespace {
class PlaceFencesWMOLegacyPass : public FunctionPass {
public:
  static char ID;

  PlaceFencesWMOLegacyPass() : FunctionPass(ID) {
    initializePlaceFencesWMOLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override {
    // Don't skip optnone functions; atomics still need to be lowered.
    FunctionAnalysisManager DummyFAM;
    auto PA = Impl.run(F, DummyFAM);
    return !PA.areAllPreserved();
  }

private:
  PlaceFencesWMOPass Impl;
  };
}

char PlaceFencesWMOLegacyPass::ID = 0;
INITIALIZE_PASS(PlaceFencesWMOLegacyPass, "placefenceswmo",
                "Place fences for architectures with Weak Memory Ordering", false, false)

Pass *llvm::createPlaceFencesWMOPass() { return new PlaceFencesWMOLegacyPass(); }

