#ifndef LLVM_LIB_TRANSFORMS_REGION_REPLICATOR_H
#define LLVM_LIB_TRANSFORMS_REGION_REPLICATOR_H

#include "RegionAnalyzer.h"

namespace llvm {

class RegionReplicator {
private:
  RegionAnalyzer &MA;
  bool IsExpandingLeft;
  // mapping from orig to replicated basic blocks
  DenseMap<BasicBlock *, BasicBlock *> Mapping;

  BasicBlock *replicateCFG(BasicBlock *ExpandedBlock, BasicBlock *MatchedBlock,
                           Region *RegionToReplicate);
  void addPhiNodes(BasicBlock *ExpandedBlock, Region *ReplicatedRegion);
  void concretizeBranchConditions(BasicBlock *ExpandedBlock,
                                  Region *ReplicatedRegion);
  void fullPredicateStores(Region *RToReplicate,
                                              BasicBlock *MatchedBlock);

public:
  RegionReplicator(RegionAnalyzer &MA, bool IsExpandingLeft)
      : MA(MA), IsExpandingLeft(IsExpandingLeft) {}

  // expands SingleBB to have the same control flow as R
  Region *replicate(BasicBlock *ExpandedBlock, BasicBlock *MatchedBlock,
                    Region *RegionToReplicate);
  void getBasicBlockMapping(DenseMap<BasicBlock *, BasicBlock *> &Map,
                            bool IsExpandingLeft);
};

} // namespace llvm

#endif