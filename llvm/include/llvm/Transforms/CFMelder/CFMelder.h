#ifndef LLVM_TRANSFORMS_CFMEGER_CFMERGER_H
#define LLVM_TRANSFORMS_CFMEGER_CFMERGER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

// Function pass for control-flow melding
class CFMelderPass : public PassInfoMixin<CFMelderPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

FunctionPass *createCFMelderPass();

// Module pass to drive control-flow melding
class CFMelderCodeSizePass : public PassInfoMixin<CFMelderCodeSizePass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

ModulePass *createCFMelderCodeSizePass();

SmallVector<unsigned> runCFM(BasicBlock *BB, DominatorTree &DT,
                             PostDominatorTree &PDT, TargetTransformInfo &TTI,
                             SmallVector<unsigned> &OnIdxs);

} // namespace llvm

#endif