//===- IntraProc/ControlDependenceGraph.cpp ---------------------*- C++ -*-===//
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

#include "llvm/Analysis/ControlDependenceGraph.h"

#include "llvm/Analysis/DOTGraphTraitsPass.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h"
#include "llvm/InitializePasses.h"


#include <deque>
#include <set>
#include <algorithm>

using namespace llvm;

namespace llvm {

ControlDependenceGraphPass::ControlDependenceGraphPass() : FunctionPass(ID) {
  initializeControlDependenceGraphPassPass(*PassRegistry::getPassRegistry());
}

void ControlDependenceNode::addTrue(ControlDependenceNode *Child) {
  node_iterator CN = std::find(TrueChildren.begin(), TrueChildren.end(), Child); //TrueChildren.find(Child);
  if (CN == TrueChildren.end())
    TrueChildren.push_back(Child);
}

void ControlDependenceNode::addFalse(ControlDependenceNode *Child) {
  node_iterator CN = std::find(FalseChildren.begin(), FalseChildren.end(), Child); //FalseChildren.find(Child);
  if (CN == FalseChildren.end())
    FalseChildren.push_back(Child);
}

void ControlDependenceNode::addOther(ControlDependenceNode *Child) {
  node_iterator CN = std::find(OtherChildren.begin(), OtherChildren.end(), Child);//OtherChildren.find(Child);
  if (CN == OtherChildren.end())
    OtherChildren.push_back(Child);
}

void ControlDependenceNode::addParent(ControlDependenceNode *Parent) {
  assert(std::find(Parent->begin(), Parent->end(), this) != Parent->end()
  	 && "Must be a child before adding the parent!");
  Parents.push_back(Parent);
}

void ControlDependenceNode::removeTrue(ControlDependenceNode *Child) {
  node_iterator CN = std::find(TrueChildren.begin(), TrueChildren.end(), Child); //TrueChildren.find(Child);
  if (CN != TrueChildren.end())
    TrueChildren.erase(CN);
}

void ControlDependenceNode::removeFalse(ControlDependenceNode *Child) {
  node_iterator CN = std::find(FalseChildren.begin(), FalseChildren.end(), Child); //FalseChildren.find(Child);
  if (CN != FalseChildren.end())
    FalseChildren.erase(CN);
}

void ControlDependenceNode::removeOther(ControlDependenceNode *Child) {
  node_iterator CN = std::find(OtherChildren.begin(), OtherChildren.end(), Child);//OtherChildren.find(Child);
  if (CN != OtherChildren.end())
    OtherChildren.erase(CN);
}

void ControlDependenceNode::removeParent(ControlDependenceNode *Parent) {
  node_iterator PN =  std::find(Parents.begin(), Parents.end(), Parent); //Parents.find(Parent);
  if (PN != Parents.end())
    Parents.erase(PN);
}

const ControlDependenceNode *ControlDependenceNode::enclosingRegion() const {
  if (this->isRegion()) {
    return this;
  } else {
    assert(this->Parents.size() == 1);
    const ControlDependenceNode *region = *this->Parents.begin();
    assert(region->isRegion());
    return region;
  }
}

ControlDependenceNode::EdgeType
ControlDependenceGraphBase::getEdgeType(const BasicBlock *A, const BasicBlock *B) {
  if (const BranchInst *b = dyn_cast<BranchInst>(A->getTerminator())) {
    if (b->isConditional()) {
      if (b->getSuccessor(0) == B) {
	return ControlDependenceNode::TRUE;
      } else if (b->getSuccessor(1) == B) {
	return ControlDependenceNode::FALSE;
      } else {
	assert(false && "Asking for edge type between unconnected basic blocks!");
      }
    }
  }
  return ControlDependenceNode::OTHER;
}

void ControlDependenceGraphBase::computeDependencies(Function &F, PostDominatorTree &pdt) {
  root = new ControlDependenceNode();
  nodes.push_back(root);

  //for (Function::iterator BB = F.begin(), E = F.end(); BB != E; ++BB) {
  for (BasicBlock &BB : F) {
    ControlDependenceNode *bn = new ControlDependenceNode(&BB);
    nodes.push_back(bn);
    bbMap[&BB] = bn;
  }

  //for (Function::iterator BB = F.begin(), E = F.end(); BB != E; ++BB) {
  for (BasicBlock &BB : F) {
    BasicBlock *A = &BB;
    ControlDependenceNode *AN = bbMap[A];

    for (succ_iterator succ = succ_begin(A), end = succ_end(A); succ != end; ++succ) {
      BasicBlock *B = *succ;
      assert(A && B);
      if (A == B || !pdt.dominates(B,A)) {
	BasicBlock *L = pdt.findNearestCommonDominator(A,B);
	ControlDependenceNode::EdgeType type = ControlDependenceGraphBase::getEdgeType(A,B);
	if (A == L) {
	  switch (type) {
	  case ControlDependenceNode::TRUE:
	    AN->addTrue(AN); break;
	  case ControlDependenceNode::FALSE:
	    AN->addFalse(AN); break;
	  case ControlDependenceNode::OTHER:
	    AN->addOther(AN); break;
	  }
	  AN->addParent(AN);
	}
	for (DomTreeNode *cur = pdt[B]; cur && cur != pdt[L]; cur = cur->getIDom()) {
	  ControlDependenceNode *CN = bbMap[cur->getBlock()];
	  switch (type) {
	  case ControlDependenceNode::TRUE:
	    AN->addTrue(CN); break;
	  case ControlDependenceNode::FALSE:
	    AN->addFalse(CN); break;
	  case ControlDependenceNode::OTHER:
	    AN->addOther(CN); break;
	  }
	  assert(CN);
	  CN->addParent(AN);
	}
      }
    }
  }

  // ENTRY -> START
  for (DomTreeNode *cur = pdt[&F.getEntryBlock()]; cur; cur = cur->getIDom()) {
    if (cur->getBlock()) {
      ControlDependenceNode *CN = bbMap[cur->getBlock()];
      assert(CN);
      root->addOther(CN); CN->addParent(root);
    }
  }
}

void ControlDependenceGraphBase::insertRegions(PostDominatorTree &pdt) {
  typedef po_iterator<PostDominatorTree*> po_pdt_iterator;  
  typedef std::pair<ControlDependenceNode::EdgeType, ControlDependenceNode *> cd_type;
  typedef std::set<cd_type> cd_set_type;
  typedef std::map<cd_set_type, ControlDependenceNode *> cd_map_type;

  cd_map_type cdMap;
  cd_set_type initCDs;
  initCDs.insert(std::make_pair(ControlDependenceNode::OTHER, root));
  cdMap.insert(std::make_pair(initCDs,root));

  for (po_pdt_iterator DTN = po_pdt_iterator::begin(&pdt), END = po_pdt_iterator::end(&pdt);
       DTN != END; ++DTN) {
    if (!DTN->getBlock())
      continue;

    ControlDependenceNode *node = bbMap[DTN->getBlock()];
    assert(node);

    cd_set_type cds;
    //for (ControlDependenceNode::node_iterator P = node->Parents.begin(), E = node->Parents.end(); P != E; ++P) {
    //  ControlDependenceNode *parent = *P;
    for (ControlDependenceNode *parent : node->Parents) {
      //if (parent->TrueChildren.find(node) != parent->TrueChildren.end())
      if (std::find(parent->TrueChildren.begin(),parent->TrueChildren.end(),node) != parent->TrueChildren.end())
	cds.insert(std::make_pair(ControlDependenceNode::TRUE, parent));
      //if (parent->FalseChildren.find(node) != parent->FalseChildren.end())
      if (std::find(parent->FalseChildren.begin(),parent->FalseChildren.end(),node) != parent->FalseChildren.end())
	cds.insert(std::make_pair(ControlDependenceNode::FALSE, parent));
      //if (parent->OtherChildren.find(node) != parent->OtherChildren.end())
      if (std::find(parent->OtherChildren.begin(),parent->OtherChildren.end(),node) != parent->OtherChildren.end())
	cds.insert(std::make_pair(ControlDependenceNode::OTHER, parent));
    }

    cd_map_type::iterator CDEntry = cdMap.find(cds);
    ControlDependenceNode *region;
    if (CDEntry == cdMap.end()) {
      region = new ControlDependenceNode();
      nodes.push_back(region);
      cdMap.insert(std::make_pair(cds,region));
      for (cd_set_type::iterator CD = cds.begin(), CDEnd = cds.end(); CD != CDEnd; ++CD) {
	switch (CD->first) {
	case ControlDependenceNode::TRUE:
	  CD->second->addTrue(region);
	  break;
	case ControlDependenceNode::FALSE:
	  CD->second->addFalse(region);
	  break;
	case ControlDependenceNode::OTHER:
	  CD->second->addOther(region); 
	  break;
	}
	region->addParent(CD->second);
      }
    } else {
      region = CDEntry->second;
    }
    for (cd_set_type::iterator CD = cds.begin(), CDEnd = cds.end(); CD != CDEnd; ++CD) {
      switch (CD->first) {
      case ControlDependenceNode::TRUE:
	CD->second->removeTrue(node);
	break;
      case ControlDependenceNode::FALSE:
	CD->second->removeFalse(node);
	break;
      case ControlDependenceNode::OTHER:
	CD->second->removeOther(node);
	break;
      }
      region->addOther(node);
      node->addParent(region);
      node->removeParent(CD->second);
    }
  }

  std::vector<ControlDependenceNode *> LazyPush;

  // Make sure that each node has at most one true or false edge
  for (auto N = nodes.begin(), E = nodes.end(); N != E; ++N) {
    ControlDependenceNode *node = *N;
  //for (ControlDependenceNode *node : nodes) {
    assert(node);
    if (node->isRegion())
      continue;

    // Fix too many true nodes
    if (node->TrueChildren.size() > 1) {
      ControlDependenceNode *region = new ControlDependenceNode();
      //nodes.push_back(region);
      LazyPush.push_back(region);
      for (ControlDependenceNode::node_iterator C = node->true_begin(), CE = node->true_end();
	   C != CE; ++C) {
	ControlDependenceNode *child = *C;
	assert(node);
	assert(child);
	assert(region);
	region->addOther(child);
	child->addParent(region);
	child->removeParent(node);
	node->removeTrue(child);
      }
      node->addTrue(region);
      region->addParent(node);
    }

    // Fix too many false nodes
    if (node->FalseChildren.size() > 1) {
      ControlDependenceNode *region = new ControlDependenceNode();
      //nodes.push_back(region);
      LazyPush.push_back(region);
      for (ControlDependenceNode::node_iterator C = node->false_begin(), CE = node->false_end();
	   C != CE; ++C) {
	ControlDependenceNode *child = *C;
	region->addOther(child);
	child->addParent(region);
	child->removeParent(node);
	node->removeFalse(child);
      }
      node->addFalse(region);
      region->addParent(node);
    }
  }

  for (ControlDependenceNode *node : LazyPush) nodes.push_back(node);
}

void ControlDependenceGraphBase::graphForFunction(Function &F, PostDominatorTree &pdt) {
  computeDependencies(F,pdt);
  insertRegions(pdt);
}

bool ControlDependenceGraphBase::controls(BasicBlock *A, BasicBlock *B) const {
  const ControlDependenceNode *n = getNode(B);
  assert(n && "Basic block not in control dependence graph!");
  while (n->getNumParents() == 1) {
    n = *n->parent_begin();
    if (n->getBlock() == A)
      return true;
  }
  return false;
}

bool ControlDependenceGraphBase::influences(BasicBlock *A, BasicBlock *B) const {
  const ControlDependenceNode *n = getNode(B);
  assert(n && "Basic block not in control dependence graph!");

  std::deque<ControlDependenceNode *> worklist;
  worklist.insert(worklist.end(), n->parent_begin(), n->parent_end());

  while (!worklist.empty()) {
    n = worklist.front();
    worklist.pop_front();
    if (n->getBlock() == A) return true;
    worklist.insert(worklist.end(), n->parent_begin(), n->parent_end());
  }

  return false;
}

const ControlDependenceNode *ControlDependenceGraphBase::enclosingRegion(BasicBlock *BB) const {
  if (const ControlDependenceNode *node = this->getNode(BB)) {
    return node->enclosingRegion();
  } else {
    return NULL;
  }
}

} // namespace llvm

namespace {

/*
struct ControlDependenceViewer
  : public DOTGraphTraitsViewer<ControlDependenceGraph, false> {
  static char ID;
  ControlDependenceViewer() : 
    DOTGraphTraitsViewer<ControlDependenceGraph, false>("control-deps", ID) {}
};
*/

struct ControlDependenceGraphPassGraphTraits {
  static ControlDependenceGraph *getGraph(ControlDependenceGraphPass *CDGP) {
    return &CDGP->getCDG();
  }
};


struct ControlDependencePrinter
  : public DOTGraphTraitsPrinter<ControlDependenceGraphPass, false, ControlDependenceGraph*, ControlDependenceGraphPassGraphTraits> {
  static char ID;
  ControlDependencePrinter() :
    DOTGraphTraitsPrinter<ControlDependenceGraphPass, false, ControlDependenceGraph*, ControlDependenceGraphPassGraphTraits>("cdg", ID) {

    initializeControlDependencePrinterPass(*PassRegistry::getPassRegistry());
  }
};

} // end anonymous namespace

char ControlDependenceGraphPass::ID = 0;
INITIALIZE_PASS_BEGIN(ControlDependenceGraphPass, "function-control-deps",
	        "Print the control dependency graph as a 'dot' file",
                true, true)
INITIALIZE_PASS_DEPENDENCY(PostDominatorTreeWrapperPass)
INITIALIZE_PASS_END(ControlDependenceGraphPass, "function-control-deps",
	        "Print the control dependency graph as a 'dot' file",
                true, true)
/*
char ControlDependenceGraphs::ID = 0;
static RegisterPass<ControlDependenceGraphs> Graphs("module-control-deps",
						    "Compute control dependency graphs for an entire module",
						    true, true);
*/

/*
char ControlDependenceViewer::ID = 0;
static RegisterPass<ControlDependenceViewer> Viewer("view-control-deps",
						    "View the control dependency graph",
						    true, true);
*/

char ControlDependencePrinter::ID = 0;
INITIALIZE_PASS(ControlDependencePrinter, "dot-cdg",
	        "Print the control dependency graph as a 'dot' file",
                true, true)


AnalysisKey ControlDependenceAnalysis::Key;
ControlDependenceGraph ControlDependenceAnalysis::run(Function &F,
                                                 FunctionAnalysisManager &AM) {
  errs() << "Running PDT\n";
  auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
  errs() << "Running CDG\n";
  ControlDependenceGraph CDG(F,PDT);
  errs() << "Done\n";
  return CDG;
}

