//===- IntraProc/ControlDependenceGraph.h -----------------------*- C++ -*-===//
//
//                      Static Program Analysis for LLVM
//
// This file is distributed under a Modified BSD License (see LICENSE.TXT).
//
//===----------------------------------------------------------------------===//
//
// This file defines the ControlDependenceGraph class, which allows fast and 
// efficient control dependence queries. It is based on Ferrante et al's "The 
// Program Dependence Graph and Its Use in Optimization."
//
//===----------------------------------------------------------------------===//

#ifndef ANALYSIS_CONTROLDEPENDENCEGRAPH_H
#define ANALYSIS_CONTROLDEPENDENCEGRAPH_H

#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/DOTGraphTraits.h"

#include <map>
#include <set>
#include <iterator>

namespace llvm {

class BasicBlock;
class ControlDependenceGraphBase;

class ControlDependenceNode {
public:
  enum EdgeType { TRUE, FALSE, OTHER };
  typedef std::vector<ControlDependenceNode *>::iterator       node_iterator;
  typedef std::vector<ControlDependenceNode *>::const_iterator const_node_iterator;

  struct edge_iterator {
    typedef node_iterator::value_type      value_type;
    typedef node_iterator::difference_type difference_type;
    typedef node_iterator::reference       reference;
    typedef node_iterator::pointer         pointer;
    typedef std::input_iterator_tag        iterator_category;

    edge_iterator(ControlDependenceNode *n) : 
      node(n), stage(TRUE), it(n->TrueChildren.begin()), end(n->TrueChildren.end()) {
      while ((stage != OTHER) && (it == end)) this->operator++();
    }
    edge_iterator(ControlDependenceNode *n, EdgeType t, node_iterator i, node_iterator e) :
      node(n), stage(t), it(i), end(e) {
      while ((stage != OTHER) && (it == end)) this->operator++();
    }
    EdgeType type() const { return stage; }
    bool operator==(edge_iterator const &other) const { 
      return (this->stage == other.stage) && (this->it == other.it);
    }
    bool operator!=(edge_iterator const &other) const { return !(*this == other); }
    reference operator*()  { return *this->it; }
    pointer   operator->() { return &*this->it; }
    edge_iterator& operator++() {
      if (it != end) ++it;
      while ((stage != OTHER) && (it == end)) {
	if (stage == TRUE) {
	  it = node->FalseChildren.begin();
	  end = node->FalseChildren.end();
	  stage = FALSE;
	} else {
	  it = node->OtherChildren.begin();
	  end = node->OtherChildren.end();
	  stage = OTHER;
	}
      }
      return *this;
    }
    edge_iterator operator++(int) {
      edge_iterator ret(*this);
      assert(ret.stage == OTHER || ret.it != ret.end);
      this->operator++();
      return ret;
    }
  private:
    ControlDependenceNode *node;
    EdgeType stage;
    node_iterator it, end;
  };

  edge_iterator begin() { return edge_iterator(this); }
  edge_iterator end()   { return edge_iterator(this, OTHER, OtherChildren.end(), OtherChildren.end()); }

  node_iterator true_begin()   { return TrueChildren.begin(); }
  node_iterator true_end()     { return TrueChildren.end(); }

  node_iterator false_begin()  { return FalseChildren.begin(); }
  node_iterator false_end()    { return FalseChildren.end(); }

  node_iterator other_begin()  { return OtherChildren.begin(); }
  node_iterator other_end()    { return OtherChildren.end(); }

  node_iterator parent_begin() { return Parents.begin(); }
  node_iterator parent_end()   { return Parents.end(); }
  const_node_iterator parent_begin() const { return Parents.begin(); }
  const_node_iterator parent_end()   const { return Parents.end(); }

  BasicBlock *getBlock() const { return BB; }
  size_t getNumParents() const { return Parents.size(); }
  size_t getNumChildren() const { 
    return TrueChildren.size() + FalseChildren.size() + OtherChildren.size();
  }
  bool isRegion() const { return BB == NULL; }
  const ControlDependenceNode *enclosingRegion() const;

private:
  BasicBlock *BB;
  std::vector<ControlDependenceNode *> Parents;
  std::vector<ControlDependenceNode *> TrueChildren;
  std::vector<ControlDependenceNode *> FalseChildren;
  std::vector<ControlDependenceNode *> OtherChildren;

  friend class ControlDependenceGraphBase;

  void clearAllChildren() {
    TrueChildren.clear();
    FalseChildren.clear();
    OtherChildren.clear();
  }
  void clearAllParents() { Parents.clear(); }

  void addTrue(ControlDependenceNode *Child);
  void addFalse(ControlDependenceNode *Child);
  void addOther(ControlDependenceNode *Child);
  void addParent(ControlDependenceNode *Parent);
  void removeTrue(ControlDependenceNode *Child);
  void removeFalse(ControlDependenceNode *Child);
  void removeOther(ControlDependenceNode *Child);
  void removeParent(ControlDependenceNode *Child);

  ControlDependenceNode() : BB(NULL) {}
  ControlDependenceNode(BasicBlock *BB) : BB(BB) {}
};

template <> struct GraphTraits<ControlDependenceNode *> {
  using NodeRef = ControlDependenceNode *;
  using ChildIteratorType = ControlDependenceNode::edge_iterator;
  using nodes_iterator = df_iterator<ControlDependenceNode *>;

  static NodeRef getEntryNode(NodeRef N) { return N; }

  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->begin();
  }

  static inline ChildIteratorType child_end(NodeRef N) {
    return N->end();
  }

  static nodes_iterator nodes_begin(NodeRef N) {
    return df_begin(getEntryNode(N));
  }

  static nodes_iterator nodes_end(NodeRef N) {
    return df_end(getEntryNode(N));
  }
};

class ControlDependenceGraphBase {
public:
  ControlDependenceGraphBase() : root(NULL) {}
  ControlDependenceGraphBase(ControlDependenceGraphBase &&Arg)
    : root(Arg.root),
      nodes(std::move(Arg.nodes)),
      bbMap(std::move(Arg.bbMap)) {}

  ControlDependenceGraphBase &operator=(ControlDependenceGraphBase &&RHS) {
    root = RHS.root;
    nodes = std::move(RHS.nodes);
    bbMap = std::move(RHS.bbMap);
    return *this;
  }


  virtual ~ControlDependenceGraphBase() { releaseMemory(); }
  void releaseMemory() {
    errs() << "Deleting nodes: "<< nodes.size() << "\n";
    for (ControlDependenceNode *Node : nodes)
      delete Node;
    nodes.clear();
    bbMap.clear();
    root = NULL;
    errs() << "Done deletion\n";
  }

  void graphForFunction(Function &F, PostDominatorTree &pdt);

  ControlDependenceNode *getEntry()             { return root; }
  const ControlDependenceNode *getEntry() const { return root; }
  ControlDependenceNode *operator[](const BasicBlock *BB)             { return getNode(BB); }
  const ControlDependenceNode *operator[](const BasicBlock *BB) const { return getNode(BB); }
  ControlDependenceNode *getNode(const BasicBlock *BB) { 
    return bbMap[BB];
  }
  const ControlDependenceNode *getNode(const BasicBlock *BB) const {
    return (bbMap.find(BB) != bbMap.end()) ? bbMap.find(BB)->second : NULL;
  }
  bool controls(BasicBlock *A, BasicBlock *B) const;
  bool influences(BasicBlock *A, BasicBlock *B) const;
  const ControlDependenceNode *enclosingRegion(BasicBlock *BB) const;

private:
  ControlDependenceNode *root;
  std::vector<ControlDependenceNode *> nodes;
  std::map<const BasicBlock *,ControlDependenceNode *> bbMap;
  static ControlDependenceNode::EdgeType getEdgeType(const BasicBlock *, const BasicBlock *);
  void computeDependencies(Function &F, PostDominatorTree &pdt);
  void insertRegions(PostDominatorTree &pdt);
};

class ControlDependenceGraph : public ControlDependenceGraphBase {
public:
 ControlDependenceGraph() {}
 ControlDependenceGraph(ControlDependenceGraph &&Arg) : ControlDependenceGraphBase(std::move(static_cast<ControlDependenceGraphBase &>(Arg))) {
  }

 ControlDependenceGraph &operator=(ControlDependenceGraph &&RHS) {
    ControlDependenceGraphBase::operator=(std::move(static_cast<ControlDependenceGraphBase &>(RHS)));
    return *this;
  }
 ~ControlDependenceGraph() {}
 ControlDependenceGraph(Function &F, PostDominatorTree &PDT) {
   graphForFunction(F,PDT);
 }
};

/// Analysis pass which computes a \c ControlDependenceGraph.
//class ControlDependenceGraphPass
class ControlDependenceAnalysis
    : public AnalysisInfoMixin<ControlDependenceAnalysis> {
  friend AnalysisInfoMixin<ControlDependenceAnalysis>;

  static AnalysisKey Key;

public:
  /// Provide the result type for this analysis pass.
  using Result = ControlDependenceGraph;

  /// Run the analysis pass over a function and produce a control dependence graph.
  Result run(Function &F, FunctionAnalysisManager &AM);
};


class ControlDependenceGraphPass : public FunctionPass {
public:
  static char ID;
  ControlDependenceGraphPass();
  virtual ~ControlDependenceGraphPass() {}
  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    //AU.addRequired<PostDominatorTree>();
    AU.addRequired<PostDominatorTreeWrapperPass>();
    AU.setPreservesAll();
  }
  virtual bool runOnFunction(Function &F) {
    //PostDominatorTree &pdt = getAnalysis<PostDominatorTree>();
    PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
    CDG.graphForFunction(F,PDT);
    return false;
  }

  ControlDependenceGraph &getCDG() { return CDG; }
  const ControlDependenceGraph &getCDG() const { return CDG; }
private:
  ControlDependenceGraph CDG;
};

template <> struct GraphTraits<ControlDependenceGraph *>
  : public GraphTraits<ControlDependenceNode *> {
  static NodeRef getEntryNode(ControlDependenceGraph *CD) {
    return CD->getEntry();
  }

  static nodes_iterator nodes_begin(ControlDependenceGraph *CD) {
    if (getEntryNode(CD))
      return df_begin(getEntryNode(CD));
    else
      return df_end(getEntryNode(CD));
  }

  static nodes_iterator nodes_end(ControlDependenceGraph *CD) {
    return df_end(getEntryNode(CD));
  }
};

template <> struct DOTGraphTraits<ControlDependenceGraph *>
  : public DefaultDOTGraphTraits {
  DOTGraphTraits(bool isSimple = false) : DefaultDOTGraphTraits(isSimple) {}

  static std::string getGraphName(ControlDependenceGraph *Graph) {
    return "Control dependence graph";
  }

  std::string getNodeLabel(ControlDependenceNode *Node, ControlDependenceGraph *Graph) {
    if (Node->isRegion()) {
      return "REGION";
    } else {
      return Node->getBlock()->hasName() ? Node->getBlock()->getName().str() : "ENTRY";
    }
  }

  static std::string getEdgeSourceLabel(ControlDependenceNode *Node, ControlDependenceNode::edge_iterator I) {
    switch (I.type()) {
    case ControlDependenceNode::TRUE:
      return "T";
    case ControlDependenceNode::FALSE:
      return "F";
    case ControlDependenceNode::OTHER:
      return "";
    }
  }
};

/*
class ControlDependenceGraphs : public ModulePass {
public:
  static char ID;

  ControlDependenceGraphs() : ModulePass(ID) {}
  virtual ~ControlDependenceGraphs() {
    graphs.clear();
  }

  virtual bool runOnModule(Module &M) {
    //for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
    for (Function &F : M) {
      if (F.isDeclaration())
	continue;
      ControlDependenceGraphBase &cdg = graphs[&F];
      //PostDominatorTree &pdt = getAnalysis<PostDominatorTree>(F);
      PostDominatorTree &pdt = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
      cdg.graphForFunction(F,pdt);
    }
    return false;
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    //AU.addRequired<PostDominatorTree>();
    AU.addRequired<PostDominatorTreeWrapperPass>();
    AU.setPreservesAll();
  }

  ControlDependenceGraphBase &operator[](const Function *F) { return graphs[F]; }
  ControlDependenceGraphBase &graphFor(const Function *F) { return graphs[F]; }
private:
  std::map<const Function *, ControlDependenceGraphBase> graphs;
};
*/

} // namespace llvm

#endif // ANALYSIS_CONTROLDEPENDENCEGRAPH_H
