#ifndef LLVM_LIB_TRANSFORMS_CFMELDER_UTILS_H
#define LLVM_LIB_TRANSFORMS_CFMELDER_UTILS_H

// #include "llvm/ADT/SequenceAlignment.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#define CFMELDER_DEBUG

#ifdef CFMELDER_DEBUG

#define INFO errs() << "INFO   : "
#define DEBUG errs() << "DEBUG  : "

#endif

#ifdef CFMELDER_INFO

#define INFO errs() << "INFO   : "
#define DEBUG

#endif

namespace llvm {

// utility functions
class Utils {
public:
  static bool containsUnhandledInstructions(const BasicBlock *BB);
  static bool isValidMergeLocation(BasicBlock &BB, DominatorTree &DT,
                                   PostDominatorTree &PDT);
  static BasicBlock *makeSingleExit(BasicBlock *BB);
  void getMultiSuccessorExitBlocks(Region *R,
                                   SmallVectorImpl<BasicBlock *> &BBs);
  static bool hasEdgeBetween(BasicBlock *BB1, BasicBlock *BB2);

  static bool match(Value *V1, Value *V2);

  static std::pair<unsigned, unsigned>
  computeLatReductionAtBest(BasicBlock *BB1, BasicBlock *BB2);
  static double computeBBSimilarty(BasicBlock *BB1, BasicBlock *BB2);
  static double
  computeRegionSimilarity(const DenseMap<BasicBlock *, BasicBlock *> &Mapping,
                          BasicBlock *LExit);

  static Region *getRegionWithEntryExit(RegionInfo &RI, BasicBlock *Entry,
                                        BasicBlock *Exit);
};

} // namespace llvm

#endif