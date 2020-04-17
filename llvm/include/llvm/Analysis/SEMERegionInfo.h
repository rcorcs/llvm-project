
#ifndef LLVM_ANALYSIS_MULTIEXITREGIONINFO_H
#define LLVM_ANALYSIS_MULTIEXITREGIONINFO_H

#include "llvm/Analysis/ControlDependenceGraph.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include <stack>
#include <set>

namespace llvm {

class PostDominatorTree;
class SEMERegion;
template <class RegionTr> class SEMERegionBase;
class SEMERegionInfo;
template <class RegionTr> class SEMERegionInfoBase;
class SEMERegionNode;

// Class to be specialized for different users of RegionInfo
// (i.e. BasicBlocks or MachineBasicBlocks). This is only to avoid needing to
// pass around an unreasonable number of template parameters.
template <class FuncT_>
struct SEMERegionTraits {
  // FuncT
  // BlockT
  // RegionT
  // RegionNodeT
  // RegionInfoT
  using BrokenT = typename FuncT_::UnknownRegionTypeError;
};

template <>
struct SEMERegionTraits<Function> {
  using FuncT = Function;
  using CDGNodeT = ControlDependenceNode;
  using ControlDepGraphT = ControlDependenceGraph;
  using BlockT = BasicBlock;
  using RegionT = SEMERegion;
  using RegionInfoT = SEMERegionInfo;
  using DomTreeNodeT = DomTreeNode;
  using PostDomTreeT = PostDominatorTree;

  static unsigned getNumSuccessors(BasicBlock *BB) {
    return BB->getTerminator()->getNumSuccessors();
  }
};

template<class Tr>
class SEMERegionBase {
  using BlockT = typename Tr::BlockT;
  using RegionT = typename Tr::RegionT;

  std::vector<BlockT *> Blocks;
  std::vector<RegionT *> Children;
public:

  BlockT *getEntry() {
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

  /// Add a new subregion to this Region.
  ///
  /// @param SubRegion The new subregion that will be added.
  void addSubRegion(RegionT *SubRegion) {
    Children.push_back(SubRegion);
  }

  void addBasicBlock(BlockT *NewBB) {
    Blocks.push_back(NewBB);
  }

};

class SEMERegion : public SEMERegionBase<SEMERegionTraits<Function>> {
};

template <class Tr>
class SEMERegionInfoBase {
  using BlockT = typename Tr::BlockT;
  using RegionT = typename Tr::RegionT;

  using ControlDepGraphT = typename Tr::ControlDepGraphT;
  using CDGNodeT = typename Tr::CDGNodeT;
  using DomTreeNodeT = typename Tr::DomTreeNodeT;
  using PostDomTreeT = typename Tr::PostDomTreeT;

  using BBtoRegionMap = DenseMap<BlockT *, RegionT *>;


  /// The top level region.
  RegionT *TopLevelRegion = nullptr;

  /// Map every BB to the smallest region, that contains BB.
  BBtoRegionMap BBtoRegion;

  RegionT *buildRegionFromCDGNode(CDGNodeT *Node, Function &F, ControlDepGraphT &CDG);
  unsigned countCFGEntries(BlockT *Root, ControlDepGraphT &CDG, std::set<CDGNodeT *> &D);
  void collectDescendants(CDGNodeT *Root, CDGNodeT *Src, std::set<CDGNodeT *> &D);
  CDGNodeT * searchLeftmostBlock(CDGNodeT *Root);
  RegionT *buildRegionsTree(CDGNodeT *N, ControlDepGraphT &CDG, Function &F);

public:

  ~SEMERegionInfoBase() {
    for (RegionT *R : Regions) delete R;
  }
  std::vector<RegionT *> Regions;
  std::map<BlockT *, std::set<RegionT *> > EntryBBMap;
  
  void buildRHG(Function &F, ControlDepGraphT &CDG);

  /// Get the smallest region that contains a BasicBlock.
  ///
  /// @param BB The basic block.
  /// @return The smallest region, that contains BB or NULL, if there is no
  /// region containing BB.
  RegionT *getRegionFor(BlockT *BB) const;

  ///  Set the smallest region that surrounds a basic block.
  ///
  /// @param BB The basic block surrounded by a region.
  /// @param R The smallest region that surrounds BB.
  void setRegionFor(BlockT *BB, RegionT *R);

  /// A shortcut for getRegionFor().
  ///
  /// @param BB The basic block.
  /// @return The smallest region, that contains BB or NULL, if there is no
  /// region containing BB.
  RegionT *operator[](BlockT *BB) const;

  RegionT *getTopLevelRegion() const { return TopLevelRegion; }
};

class SEMERegionInfo : public SEMERegionInfoBase<SEMERegionTraits<Function>> {
public:
//SEMERegionInfo(const Function &F, const ControlDependenceGraph &CDG);

void dotGraph() {
  errs() << "digraph {\n";
  for (SEMERegion *R : Regions) {
    errs() << ((uintptr_t)R) << "[shape=\"box\", label=\"";

    errs() << R->getEntry()->getName() << " [entry]\\n";
    for (BasicBlock *BB : R->blocks()) {
      if (BB!=R->getEntry()) {
        errs() << BB->getName() << "\\n";
      }
    }
    errs() << "\"];\n";
  }
  
  for (SEMERegion *R : Regions) {
    for (SEMERegion *Child : *R)
    errs() << ((uintptr_t)R) << "->" << ((uintptr_t)Child) << ";\n";
  }
  
  errs() << "}\n";
}

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

//build region tree using a postorder traversal
template <class Tr>
typename Tr::RegionT *SEMERegionInfoBase<Tr>::buildRegionsTree(typename Tr::CDGNodeT *Root, typename Tr::ControlDepGraphT &CDG, Function &F) {

  std::set<CDGNodeT *> Visited;

  auto RecursivePostOrder = [&](auto&& self, CDGNodeT *Node) -> RegionT* {
    if (Visited.count(Node)) return nullptr;
    Visited.insert(Node);
    
    std::vector<RegionT *> Children;
  
    for (CDGNodeT *Child : *Node) {
      RegionT *CR = self(self, Child);
      if (CR) Children.push_back(CR);
    }
  
    RegionT *R = buildRegionFromCDGNode(Node, F, CDG);
    if (R) {
      for (RegionT *CR : Children) R->addSubRegion(CR);
    } else {
      if (Children.size()>1) errs() << "ERROR: Too Many Children!\n";
      else if (Children.size())
        R = Children[0];
    }
  
    return R;
  };

  return RecursivePostOrder(RecursivePostOrder,Root);
}

template <class Tr>
typename Tr::CDGNodeT * SEMERegionInfoBase<Tr>::searchLeftmostBlock(typename Tr::CDGNodeT *Root) {
  std::set<CDGNodeT *> Visited;
  auto RecursiveSearch = [&](auto&& self, CDGNodeT *Node) -> CDGNodeT* {
    if (Visited.count(Node)) return nullptr;
       Visited.insert(Node);
    
    if (Node->getBlock()) return Node;

    for (CDGNodeT *Child : *Node) {
      CDGNodeT *FoundN = self(self,Child);
      if (FoundN!=nullptr) return FoundN;
    }
    return nullptr;
  };
  return RecursiveSearch(RecursiveSearch,Root);
}

template <class Tr>
void SEMERegionInfoBase<Tr>::collectDescendants(CDGNodeT *Root, CDGNodeT *Src, std::set<CDGNodeT *> &D) {
  std::set<CDGNodeT *> Visited;
  auto CollectRecursively = [&](auto&& self, CDGNodeT *Node, bool Building) -> void {
    if (Visited.count(Node)) return;
       Visited.insert(Node);

    Building = Building || (Node==Src);
    if (Building && Node!=Src) D.insert(Node);

    for (ControlDependenceNode *Child : *Node) {
      self(self, Child,Building);
    }
  };
  CollectRecursively(CollectRecursively,Root,false);
}

template <class Tr>
unsigned SEMERegionInfoBase<Tr>::countCFGEntries(BlockT *Root, ControlDepGraphT &CDG, std::set<CDGNodeT *> &D) {
  std::set<BlockT *> Visited;
  unsigned NumEntries = 0;
  auto CountRecursively = [&](auto &&self, BlockT *BB) -> void {
    if (Visited.count(BB)) return;
    Visited.insert(BB);
  
    //for (ControlDependenceNode *Child : *Node) {
    for (auto ItBB = succ_begin(BB), E = succ_end(BB); ItBB!=E; ItBB++) {
      BlockT *SuccBB = *ItBB;
      NumEntries += (D.count(CDG.getNode(SuccBB)) && !D.count(CDG.getNode(BB)));
      self(self,SuccBB);
    }
  };
  CountRecursively(CountRecursively, Root);
  return NumEntries;
}

template <class Tr>
typename Tr::RegionT *SEMERegionInfoBase<Tr>::buildRegionFromCDGNode(CDGNodeT *Node, Function &F, ControlDepGraphT &CDG) {
  //skip leaves (non-internal nodes)
  if (Node->getNumChildren()==0) return nullptr;

  std::set<CDGNodeT *> D;
  collectDescendants(CDG.getRoot(),Node,D);
  D.insert(Node);
  unsigned NumEntries = countCFGEntries(&F.getEntryBlock(), CDG, D);
  //skip regions with multi-entries
  if (NumEntries>1) return nullptr;
  RegionT *R = new RegionT;

  CDGNodeT *EntryNode = searchLeftmostBlock(Node);

  R->addBasicBlock(EntryNode->getBlock());

  for (auto *ReachedNode : D) {
    if (ReachedNode->getBlock()) R->addBasicBlock(ReachedNode->getBlock());
  }
  bool Found = false;
  for (RegionT *Other : EntryBBMap[R->getEntry()]) {
    if (Other->getNumBlocks()==R->getNumBlocks()) {
      Found = true;
      for (auto It1 = R->block_begin(), It2 = Other->block_begin(), E = R->block_end(); It1!=E; It1++, It2++) {
        Found = Found && ( (*It1)==(*It2) );
      }
    }
  }
  if (Found) {
    delete R;
    return nullptr;
  }

  Regions.push_back(R);
  EntryBBMap[R->getEntry()].insert(R);

  return R;
}

template <class Tr>
void SEMERegionInfoBase<Tr>::buildRHG(Function &F, ControlDepGraphT &CDG) {
  TopLevelRegion = buildRegionsTree(CDG.getRoot(), CDG, F);
}

} // namespace llvm



#endif
