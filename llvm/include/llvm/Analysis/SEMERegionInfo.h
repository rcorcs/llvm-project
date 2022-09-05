
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
template <class NodeT> class DescendantTree;

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

  using FuncT = typename Tr::FuncT;
  using ControlDepGraphT = typename Tr::ControlDepGraphT;
  using CDGNodeT = typename Tr::CDGNodeT;
  using DomTreeNodeT = typename Tr::DomTreeNodeT;
  using PostDomTreeT = typename Tr::PostDomTreeT;

  using BBtoRegionMap = DenseMap<BlockT *, RegionT *>;


  /// The top level region.
  RegionT *TopLevelRegion = nullptr;

  /// Map every BB to the smallest region, that contains BB.
  BBtoRegionMap BBtoRegion;

  RegionT *buildRegionFromCDGNode(CDGNodeT *Node, FuncT &F, ControlDepGraphT &CDG, DescendantTree<CDGNodeT *> &DTree);
  unsigned countCFGEntries(ControlDepGraphT &CDG, std::set<CDGNodeT *> &Reachable);
  CDGNodeT * searchLeftmostBlock(CDGNodeT *Root);
  std::set<CDGNodeT *> computeDescendants(CDGNodeT *Node, DescendantTree<CDGNodeT *> &DTree);

  void addRegion(RegionT *R) {
    Regions.push_back(R);
    EntryBBMap[R->getEntry()].insert(R);
    //for (BlockT *BB : R->blocks()) {}
  }

  /// Wipe this region tree's state without releasing any resources.
  ///
  /// This is essentially a post-move helper only. It leaves the object in an
  /// assignable and destroyable state, but otherwise invalid.
  void wipe() {
    TopLevelRegion = nullptr;
    Regions.clear();
    EntryBBMap.clear();
    BBtoRegion.clear();
  }

  void releaseMemory() {
    for (RegionT *R : Regions)
      delete R;
    wipe();
  }
public:

  SEMERegionInfoBase() {}

  SEMERegionInfoBase(SEMERegionInfoBase &&Arg)
    : TopLevelRegion(std::move(Arg.TopLevelRegion)),
      BBtoRegion(std::move(Arg.BBtoRegion)),
      Regions(std::move(Arg.Regions)),
      EntryBBMap(std::move(Arg.EntryBBMap)) {
     //errs() << "Reference copy\n";
    Arg.wipe();
  }

  SEMERegionInfoBase &operator=(SEMERegionInfoBase &&RHS) {
    //errs() << "assignment copy\n";
    TopLevelRegion = std::move(RHS.TopLevelRegion);
    BBtoRegion = std::move(RHS.BBtoRegion);
    Regions = std::move(RHS.Regions);
    EntryBBMap = std::move(RHS.EntryBBMap);
    RHS.wipe();
    return *this;
  }

  SEMERegionInfoBase(const SEMERegionInfoBase &) = delete;
  SEMERegionInfoBase &operator=(const SEMERegionInfoBase &) = delete;

  ~SEMERegionInfoBase() {
    releaseMemory();
  }

  std::vector<RegionT *> Regions;
  std::map<BlockT *, std::set<RegionT *> > EntryBBMap;
  
  void buildRegionsTree(FuncT &F, ControlDepGraphT &CDG);

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
 using Base = SEMERegionInfoBase<SEMERegionTraits<Function>>;
public:

  SEMERegionInfo() {}

  SEMERegionInfo(SEMERegionInfo &&Arg) : Base(std::move(static_cast<Base &>(Arg))) {}

  SEMERegionInfo &operator=(SEMERegionInfo &&RHS) {
    Base::operator=(std::move(static_cast<Base &>(RHS)));
    return *this;
  }

//SEMERegionInfo(const Function &F, const ControlDependenceGraph &CDG);
};

template <> struct GraphTraits<SEMERegion *> {
  using NodeRef = SEMERegion *;
  using ChildIteratorType = SEMERegion::iterator;
  using nodes_iterator = df_iterator<SEMERegion *>;

  static NodeRef getEntryNode(SEMERegion *N) { return N; }

  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->begin();
  }

  static inline ChildIteratorType child_end(NodeRef N) {
    return N->end();
  }

  static nodes_iterator nodes_begin(SEMERegion *N) {
    return df_begin(getEntryNode(N));
  }

  static nodes_iterator nodes_end(SEMERegion *N) {
    return df_end(getEntryNode(N));
  }
};


template <> struct GraphTraits<SEMERegionInfo *> {
  using NodeRef = std::add_pointer_t<SEMERegion>;
  using ChildIteratorType = SEMERegion::iterator;
  using nodes_iterator = df_iterator<SEMERegion *>;

  static NodeRef getEntryNode(SEMERegionInfo *G) { return G->getTopLevelRegion(); }

  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->begin();
  }

  static inline ChildIteratorType child_end(NodeRef N) {
    return N->end();
  }

  static nodes_iterator nodes_begin(SEMERegionInfo *G) {
    return df_begin(getEntryNode(G));
  }

  static nodes_iterator nodes_end(SEMERegionInfo *G) {
    return df_end(getEntryNode(G));
  }
};

template <> struct DOTGraphTraits<SEMERegionInfo *>
  : public DefaultDOTGraphTraits {
  DOTGraphTraits(bool isSimple = false) : DefaultDOTGraphTraits(isSimple) {}

  static std::string getGraphName(SEMERegionInfo *Graph) {
    return "SEME Region Hierarchy Graph";
  }

  std::string getNodeLabel(SEMERegion *Node, SEMERegionInfo *Graph) {
    std::string Label = "";
    Label = Node->getEntry()->getName().str() + std::string(" [entry]\n");
    for (BasicBlock *BB : Node->blocks()) {
      if (BB!=Node->getEntry()) {
        Label +=  BB->getName().str() + std::string("\n");
      }
    }
    return Label;
  }

  static std::string getEdgeSourceLabel(SEMERegion *Node, SEMERegion::iterator I) {
      return "";
  }
};


class SEMERegionLegacyPass : public FunctionPass {
public:
  static char ID;
  SEMERegionLegacyPass();
  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<ControlDependenceGraphPass>();
    AU.setPreservesAll();
  }
  virtual bool runOnFunction(Function &F);

  SEMERegionInfo &getSEMERegionInfo() { return SRI; }
  const SEMERegionInfo &getSEMERegionInfo() const { return SRI; }
private:
  SEMERegionInfo SRI;
};

/// Analysis pass which computes \c BlockFrequencyInfo.
class SEMERegionAnalysis
    : public AnalysisInfoMixin<SEMERegionAnalysis> {
  friend AnalysisInfoMixin<SEMERegionAnalysis>;

  static AnalysisKey Key;

public:
  /// Provide the result type for this analysis pass.
  using Result = SEMERegionInfo;

  /// Run the analysis pass over a function and produce BFI.
  Result run(Function &F, FunctionAnalysisManager &AM);
};


template <class NodeT>
class DescendantNode {
  NodeT Item;
  std::set<DescendantNode<NodeT> *> Children;

public:
  DescendantNode(NodeT N) { Item = N; }
  void addChild(DescendantNode<NodeT> *CN) { Children.insert(CN); }
  NodeT item() { return Item; }
  NodeT operator*() { return Item; }

  using iterator = typename std::set<DescendantNode<NodeT> *>::iterator;
  //typedef std::set<DescendantNode<NodeT> *>::const_iterator const_node_iterator;

  iterator begin()   { return Children.begin(); }
  iterator end()     { return Children.end(); }
};

template <> struct GraphTraits< DescendantNode<ControlDependenceNode*>* > {
  using NodeRef = DescendantNode<ControlDependenceNode*>*;
  using ChildIteratorType = DescendantNode<ControlDependenceNode *>::iterator;
  using nodes_iterator = df_iterator< DescendantNode<ControlDependenceNode*>* >;

  static NodeRef getEntryNode(NodeRef N) { return N; }

  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->begin();
  }

  static inline ChildIteratorType child_end(NodeRef N) {
    return N->end();
  }

  static nodes_iterator nodes_begin(NodeRef N) {
    return ++df_begin(getEntryNode(N));
  }

  static nodes_iterator nodes_end(NodeRef N) {
    return df_end(getEntryNode(N));
  }
};

template <class NodeT>
class DescendantTree {
public:
  
  DescendantNode<NodeT> *TreeRoot;
  std::vector< DescendantNode<NodeT> *> Nodes;
  std::map<NodeT, DescendantNode<NodeT> *> NodesMap;

  DescendantTree(NodeT RootNode) {
    std::map<NodeT, char> VisitedDone;
    
    auto RecursiveBuild = [&](auto &&self, NodeT N) -> DescendantNode<NodeT> * {
      if (VisitedDone[N]!=0)
        return nullptr;
      VisitedDone[N] = 1;
      
      DescendantNode<NodeT> *DN = new DescendantNode<NodeT>(N);
      Nodes.push_back(DN);
      NodesMap[N] = DN;

      for (auto It = GraphTraits<NodeT>::child_begin(N), E = GraphTraits<NodeT>::child_end(N); It!=E; It++) {
        if (VisitedDone[*It]==1) continue; //back-edge
	if (DescendantNode<NodeT> *CN = self(self, *It)) { DN->addChild(CN); };
      }
       
      VisitedDone[N] = 2;

      return DN;
    };
    TreeRoot = RecursiveBuild(RecursiveBuild,RootNode);
  }

  ~DescendantTree() {
    for(DescendantNode<NodeT> *N : Nodes) delete N;
  }

  DescendantNode<NodeT> &getDescendantNodeFor(NodeT N) { return *NodesMap[N]; }
  DescendantNode<NodeT> &operator[](NodeT N) { return *NodesMap[N]; }
};

class SEMERegionPrinterPass : public PassInfoMixin<SEMERegionPrinterPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};


//////////////// Implementation ///////////////////////

//build region tree using a postorder traversal
template <class Tr>
void SEMERegionInfoBase<Tr>::buildRegionsTree(typename Tr::FuncT &F, typename Tr::ControlDepGraphT &CDG) {
  releaseMemory();

  using CDGPtrT = std::add_pointer_t<ControlDepGraphT>;
  CDGNodeT *EntryNode = GraphTraits<CDGPtrT>::getEntryNode(&CDG);
  DescendantTree<CDGNodeT *> DTree(EntryNode);

  std::set<CDGNodeT *> Visited;
  auto RecursivePostOrder = [&](auto&& self, CDGNodeT *Node, std::vector<RegionT *> &Siblings) -> RegionT* {
    if (Visited.count(Node)) return nullptr;
    Visited.insert(Node);
    
    std::vector<RegionT *> Children;
  
    for (CDGNodeT *Child : *Node) {
      RegionT *CR = self(self, Child, Children);
      if (CR) Children.push_back(CR);
    }
  
    RegionT *R = buildRegionFromCDGNode(Node, F, CDG, DTree);
    if (R) {
      for (RegionT *CR : Children) R->addSubRegion(CR);
    } else {
      for (RegionT *CR : Children) Siblings.push_back(CR);
    }
  
    return R;
  };


  std::vector<RegionT *> ChildrenNodes;
  TopLevelRegion = RecursivePostOrder(RecursivePostOrder,EntryNode,ChildrenNodes);
  if (TopLevelRegion==nullptr) {
    if (ChildrenNodes.size()==1) {
    TopLevelRegion = ChildrenNodes[0];
    }// else errs() << "Wrong number of roots\n"; 
  }
}

template <class Tr>
typename Tr::CDGNodeT * SEMERegionInfoBase<Tr>::searchLeftmostBlock(CDGNodeT *Node) {
  for (auto It = GraphTraits<CDGNodeT *>::nodes_begin(Node), E = GraphTraits<CDGNodeT *>::nodes_end(Node); It!=E; It++) {
    if ( (*It)->getBlock() ) return (*It);
  }
  return nullptr;
}

template <class Tr>
unsigned SEMERegionInfoBase<Tr>::countCFGEntries(ControlDepGraphT &CDG, std::set<CDGNodeT *> &Reachable) {
  unsigned NumEntries = 0;

  for (CDGNodeT *Node : Reachable) {
    if (Node->getBlock()) {
      for (auto It = pred_begin(Node->getBlock()), E = pred_end(Node->getBlock()); It!=E; It++) {
        NumEntries += !Reachable.count(CDG.getNode(*It));
      }
    }
  }

  return NumEntries;
}

template <class Tr>
std::set<typename Tr::CDGNodeT *> SEMERegionInfoBase<Tr>::computeDescendants(CDGNodeT *Node, DescendantTree<CDGNodeT *> &DTree) {
  std::set<CDGNodeT *> Descendants;
  auto &DN = DTree[Node];
  for (auto It = GraphTraits< DescendantNode<CDGNodeT*>* >::nodes_begin(&DN), E = GraphTraits< DescendantNode<CDGNodeT*>* >::nodes_end(&DN); It!=E; It++) {
    Descendants.insert( (*It)->item() );
  }
  return Descendants;
}


template <class Tr>
typename Tr::RegionT *SEMERegionInfoBase<Tr>::buildRegionFromCDGNode(CDGNodeT *Node, FuncT &F, ControlDepGraphT &CDG, DescendantTree<CDGNodeT *> &DTree) {
  //skip leaves (non-internal nodes)
  if (Node->getNumChildren()==0) return nullptr;

  std::set<CDGNodeT *> Reachable = computeDescendants(Node, DTree);
  Reachable.insert(Node);

  unsigned NumEntries = countCFGEntries(CDG, Reachable);
  //skip regions with multi-entries
  if (NumEntries>1) return nullptr;

  RegionT *R = new RegionT;

  CDGNodeT *EntryNode = searchLeftmostBlock(Node);

  R->addBasicBlock(EntryNode->getBlock());

  for (auto *ReachedNode : Reachable) {
    if (ReachedNode->getBlock() && ReachedNode!=EntryNode) R->addBasicBlock(ReachedNode->getBlock());
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
  } else {
    addRegion(R);
    return R;
  }
}

} // namespace llvm



#endif
