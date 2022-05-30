//===- FunctionMerging.h - A function merging pass ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This pass merges similar functions replacing the original ones with a call.
// This file also provides basic utility functions for merging any pair
// of functions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_IPO_FUNCTIONMERGING_H
#define LLVM_TRANSFORMS_IPO_FUNCTIONMERGING_H

#include "llvm/InitializePasses.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

#include <map>
#include <unordered_map>
#include <vector>

namespace llvm {

/// A set of parameters used to control the transforms by MergeFunctions.
struct FunctionMergingOptions {
  bool MaximizeParamScore;
  bool EnableUnifiedReturnType;
  bool EnableOperandReordering;
  unsigned MaxNumSelection;

  FunctionMergingOptions(bool MaximizeParamScore = true,
                         bool EnableUnifiedReturnType = true,
                         unsigned MaxNumSelection = 500)
      : MaximizeParamScore(MaximizeParamScore),
        EnableUnifiedReturnType(EnableUnifiedReturnType),
        MaxNumSelection(MaxNumSelection) {}

  FunctionMergingOptions &maximizeParameterScore(bool MPS) {
    MaximizeParamScore = MPS;
    return *this;
  }

  FunctionMergingOptions &enableUnifiedReturnTypes(bool URT) {
    EnableUnifiedReturnType = URT;
    return *this;
  }

  FunctionMergingOptions &enableOperandReordering(bool EOR) {
    EnableOperandReordering = EOR;
    return *this;
  }

  FunctionMergingOptions &maximumNumberSelections(unsigned MNS) {
    MaxNumSelection = MNS;
    return *this;
  }
};

class FunctionMergeResult {
private:
  Function *F1;
  Function *F2;
  Function *MergedFunction;
  bool HasIdArg;
  bool NeedUnifiedReturn;
  std::map<unsigned, unsigned> ParamMap1;
  std::map<unsigned, unsigned> ParamMap2;

  FunctionMergeResult()
      : F1(nullptr), F2(nullptr), MergedFunction(nullptr), HasIdArg(false),
        NeedUnifiedReturn(false) {}

public:
  FunctionMergeResult(Function *F1, Function *F2, Function *MergedFunction,
                      bool NeedUnifiedReturn = false)
      : F1(F1), F2(F2), MergedFunction(MergedFunction), HasIdArg(true),
        NeedUnifiedReturn(NeedUnifiedReturn) {}

  std::pair<Function *, Function *> getFunctions() {
    return std::pair<Function *, Function *>(F1, F2);
  }

  std::map<unsigned, unsigned> &getArgumentMapping(Function *F) {
    return (F1 == F) ? ParamMap1 : ParamMap2;
  }

  Value *getFunctionIdValue(Function *F) {
    if (F == F1)
      return ConstantInt::getTrue(IntegerType::get(F1->getContext(), 1));
    else if (F == F2)
      return ConstantInt::getFalse(IntegerType::get(F2->getContext(), 1));
    else
      return nullptr;
  }

  void setFunctionIdArgument(bool HasFuncIdArg) { HasIdArg = HasFuncIdArg; }

  bool hasFunctionIdArgument() { return HasIdArg; }

  void setUnifiedReturn(bool NeedUnifiedReturn) {
    this->NeedUnifiedReturn = NeedUnifiedReturn;
  }

  bool needUnifiedReturn() { return NeedUnifiedReturn; }

  // returns whether or not the merge operation was successful
  operator bool() const { return (MergedFunction != nullptr); }

  void setArgumentMapping(Function *F, std::map<unsigned, unsigned> &ParamMap) {
    if (F == F1)
      ParamMap1 = ParamMap;
    else if (F == F2)
      ParamMap2 = ParamMap;
  }

  void addArgumentMapping(Function *F, unsigned SrcArg, unsigned DstArg) {
    if (F == F1)
      ParamMap1[SrcArg] = DstArg;
    else if (F == F2)
      ParamMap2[SrcArg] = DstArg;
  }

  Function *getMergedFunction() { return MergedFunction; }
};

FunctionMergeResult MergeFunctions(Function *F1, Function *F2, const char *Name,
                                   const FunctionMergingOptions &Options);
void ReplaceFunctionByCall(Function *F, FunctionMergeResult &MFR);
void ReplaceFunctionsByCall(FunctionMergeResult &MFR);

class FunctionMergingPass : public PassInfoMixin<FunctionMergingPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

} // namespace llvm

#endif
