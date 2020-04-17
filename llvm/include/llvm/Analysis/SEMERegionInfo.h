
#ifndef LLVM_ANALYSIS_MULTIEXITREGIONINFO_H
#define LLVM_ANALYSIS_MULTIEXITREGIONINFO_H

#include "llvm/Analysis/ControlDependenceGraph.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include <set>

namespace llvm {

template<class BlockT, class RegionT>
class SEMERegionBase {
  std::vector<BlockT *> Blocks;
  std::vector<RegionT *> Children;
public:

  BlockT *getEntryBlock() {
    return getBlocks().front();
  }

  unsigned getNumBlocks() const {
    return Blocks.size();
  }
 
  /// Get a list of the basic blocks which make up this loop.
  ArrayRef<BlockT *> getBlocks() const {
    return Blocks;
  }
  typedef typename ArrayRef<BlockT *>::const_iterator block_iterator;
  block_iterator block_begin() const { return getBlocks().begin(); }
  block_iterator block_end() const { return getBlocks().end(); }
  inline iterator_range<block_iterator> blocks() const {
    return make_range(block_begin(), block_end());
  }

  /// Return the regions contained entirely within this loop.
  const std::vector<RegionT *> &getSubRegions() const {
    return Children;
  }

  std::vector<RegionT *> &getSubRegionsVector() {
    return Children;
  }

  typedef typename std::vector<RegionT *>::const_iterator iterator;
  typedef typename std::vector<RegionT *>::const_reverse_iterator reverse_iterator;
  iterator begin() const { return getSubRegions().begin(); }
  iterator end() const { return getSubRegions().end(); }
  reverse_iterator rbegin() const { return getSubRegions().rbegin(); }
  reverse_iterator rend() const { return getSubRegions().rend(); }
  bool empty() const { return getSubRegions().empty(); }

  void addChildRegion(RegionT *R) {
    Children.push_back(R);
  }

  void addBasicBlock(BlockT *NewBB) {
    Blocks.push_back(NewBB);
  }

};

class SEMERegion : public SEMERegionBase<BasicBlock, SEMERegion> {
};

template <class BlockT, class RegionT>
class SEMERegionInfoBase {
public:
  RegionT Root;
  std::vector<RegionT> Regions;
  std::map<BlockT, std::set<RegionT> > EntryMap;
};


class SEMERegionInfo : public SEMERegionInfoBase<BasicBlock *, SEMERegion *> {
public:
//SEMERegionInfo(const Function &F, const ControlDependenceGraph &CDG);
};

class SEMERegionInfoPass : public FunctionPass {
public:
  static char ID;
  SEMERegionInfoPass();
  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<PostDominatorTreeWrapperPass>();
    AU.setPreservesAll();
  }
  virtual bool runOnFunction(Function &F);
};

} // namespace llvm

#endif
