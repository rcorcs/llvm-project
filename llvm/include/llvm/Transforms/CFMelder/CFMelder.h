#ifndef LLVM_TRANSFORMS_CFMEGER_CFMERGER_H
#define LLVM_TRANSFORMS_CFMEGER_CFMERGER_H

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"

namespace llvm {

class CFMelderPass
    : public PassInfoMixin<CFMelderPass> {

public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

bool runCFMelderPass(Function &F, DominatorTree &DT, PostDominatorTree &PDT,
                    LoopInfo &LI, TargetTransformInfo &TTI);

FunctionPass *createCFMelderPass();
}

#endif
