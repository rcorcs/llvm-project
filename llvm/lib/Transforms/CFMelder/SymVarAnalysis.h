#ifndef  LLVM_LIB_TRANSFORMS_CFMELDER_SYM_VAR_ANALYSIS_
#define  LLVM_LIB_TRANSFORMS_CFMELDER_SYM_VAR_ANALYSIS_
#include "llvm/Analysis/DivergenceAnalysis.h"

namespace llvm {
class SymVarAnalysis {
  DivergenceAnalysis DA;
  SyncDependenceAnalysis SDA;

public:
  /// Runs the divergence analysis on @F, a GPU kernel
  SymVarAnalysis(Function &F);

  /// Whether any divergence was detected.
  bool hasDivergence() const { return DA.hasDetectedDivergence(); }

  /// The GPU kernel this analysis result is for
  const Function &getFunction() const { return DA.getFunction(); }

  /// Whether \p V is divergent at its definition.
  bool isDivergent(const Value &V) const;

  /// Whether \p U is divergent. Uses of a uniform value can be divergent.
  bool isDivergentUse(const Use &U) const;

};

} // namespace llvm

#endif