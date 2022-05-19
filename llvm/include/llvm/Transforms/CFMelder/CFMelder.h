#ifndef LLVM_TRANSFORMS_CFMEGER_CFMERGER_H
#define LLVM_TRANSFORMS_CFMEGER_CFMERGER_H

#include "llvm/IR/PassManager.h"

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/DivergenceAnalysis.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Analysis/DominanceFrontierImpl.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Analysis/RegionInfoImpl.h"
#include "llvm/Analysis/TargetTransformInfo.h"


namespace llvm {

class CFMelderPass
    : public PassInfoMixin<CFMelderPass> {

public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};


FunctionPass *createCFMelderPass();
bool runCFMelderPass(Function &F, DominatorTree &DT,
                           PostDominatorTree &PDT, LoopInfo &LI,
                           TargetTransformInfo &TTI);
}

#endif

