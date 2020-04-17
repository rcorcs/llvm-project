
#include "llvm/Analysis/SEMERegionInfo.h"

#include "llvm/Analysis/DOTGraphTraitsPass.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h"
#include "llvm/InitializePasses.h"

#include "llvm/Support/raw_ostream.h"

#include <deque>
#include <set>

using namespace llvm;

static ControlDependenceNode * leftmost_block(ControlDependenceNode *Node, std::set<ControlDependenceNode *> &Visited) {
  if (Visited.count(Node)) return nullptr;
  Visited.insert(Node);

  if (Node->getBlock()) return Node;

  for (ControlDependenceNode *Child : *Node) {
    ControlDependenceNode *FoundN = leftmost_block(Child,Visited);
    if (FoundN!=nullptr) return FoundN;
  }

  return nullptr;
}


static void descendants(ControlDependenceNode *Node, ControlDependenceNode *Src, bool Building, std::set<ControlDependenceNode *> &Visited, std::set<ControlDependenceNode *> &D) {
  if (Visited.count(Node)) return;
  Visited.insert(Node);

  Building = Building || (Node==Src);
  if (Building && Node!=Src) D.insert(Node);

  for (ControlDependenceNode *Child : *Node) {
    descendants(Child,Src,Building,Visited,D);
  }
}

static void countCFGEntries(BasicBlock *BB, std::set<BasicBlock *> &Visited, ControlDependenceGraph &CDG, std::set<ControlDependenceNode *> &D, unsigned &NumEntries) {
  if (Visited.count(BB)) return;
  Visited.insert(BB);

  //for (ControlDependenceNode *Child : *Node) {
  for (auto ItBB = succ_begin(BB), E = succ_end(BB); ItBB!=E; ItBB++) {
    BasicBlock *SuccBB = *ItBB;
    NumEntries += (D.count(CDG.getNode(SuccBB)) && !D.count(CDG.getNode(BB)));
    countCFGEntries(SuccBB,Visited,CDG,D,NumEntries);
  }
}

static SEMERegion *buildRegionFromCDGNode(ControlDependenceNode *Node, Function &F, ControlDependenceGraph &CDG, SEMERegionInfo &SRI) {
    //skip leaves (non-internal nodes)
    if (Node->getNumChildren()==0) return nullptr;

    std::set<ControlDependenceNode *> D;  {
      std::set<ControlDependenceNode *> Visited;
      descendants(CDG.getRoot(),Node,false,Visited,D);
    }
    D.insert(Node);
    unsigned NumEntries = 0; {
      std::set<BasicBlock *> Visited;
      countCFGEntries(&F.getEntryBlock(), Visited, CDG, D, NumEntries);
    }
    //skip regions with multi-entries
    if (NumEntries>1) return nullptr;
    SEMERegion *R = new SEMERegion;

    ControlDependenceNode *EntryNode; {
      std::set<ControlDependenceNode *> Visited;
      EntryNode = leftmost_block(Node,Visited);
    }
    R->addBasicBlock(EntryNode->getBlock());

    /* //code from the paper that does *not* make sense
    if (EntryNode!=Node) {
      std::set<ControlDependenceNode *> Visited;
      D.clear();
      descendants(CDG.getRoot(),EntryNode,false,Visited,D);
      D.insert(EntryNode);
    }*/

    for (auto *ReachedNode : D) {
      if (ReachedNode->getBlock()) R->addBasicBlock(ReachedNode->getBlock());
    }
    bool Found = false;
    for (SEMERegion *Other : SRI.EntryMap[R->getEntryBlock()]) {
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

    SRI.Regions.push_back(R);
    SRI.EntryMap[R->getEntryBlock()].insert(R);

    return R;
}

static SEMERegion *buildRHGRecursively(ControlDependenceNode *Node, std::set<ControlDependenceNode *> &Visited,Function &F, ControlDependenceGraph &CDG,SEMERegionInfo &SRI) {
  if (Visited.count(Node)) return nullptr;
  Visited.insert(Node);
  
  std::vector<SEMERegion*> Children;

  for (ControlDependenceNode *Child : *Node) {
    SEMERegion *R = buildRHGRecursively(Child,Visited,F,CDG,SRI);
    if (R) Children.push_back(R);
  }

  SEMERegion *R = buildRegionFromCDGNode(Node, F, CDG, SRI);
  if (R) {
    for (SEMERegion *Child : Children) R->addChildRegion(Child);
  } else {
    if (Children.size()>1) errs() << "ERROR: Too Many Children!\n";
    else if (Children.size())
      R = Children[0];
  }
  return R;
}

void printIdent(unsigned ident) {
  for (unsigned i = 0; i<ident; i++) errs() << "  ";
}

void printRHG(SEMERegion *R, unsigned ident=0) {
  printIdent(ident);
  errs() << "Region: " << R->getEntryBlock()->getName() << " {";
  for (BasicBlock *BB : R->blocks()) {
    if (BB!=R->getEntryBlock()) {
      errs() << '\n';
      printIdent(ident);
      errs() << "  " << BB->getName();
    }
  }
  if (R->getNumBlocks()>1) {
    errs() << "\n";
    printIdent(ident);
  }
  errs() << "},\n";

  if (!R->empty()) {
    printIdent(ident);
    errs() << "{\n";
    for (SEMERegion *Child : *R) printRHG(Child,ident+1);
    printIdent(ident);
    errs() << "}\n";
  }
}

static void dotGraph(SEMERegionInfo &SRI) {
  errs() << "digraph {\n";
  for (SEMERegion *R : SRI.Regions) {
    errs() << ((uintptr_t)R) << "[shape=\"box\", label=\"";

    errs() << R->getEntryBlock()->getName() << " [entry]\\n";
    for (BasicBlock *BB : R->blocks()) {
      if (BB!=R->getEntryBlock()) {
        errs() << BB->getName() << "\\n";
      }
    }
    errs() << "\"];\n";
  }
  
  for (SEMERegion *R : SRI.Regions) {
    for (SEMERegion *Child : *R)
    errs() << ((uintptr_t)R) << "->" << ((uintptr_t)Child) << ";\n";
  }
  
  errs() << "}\n";
}

void buildRHG(Function &F, ControlDependenceGraph &CDG) {
  SEMERegionInfo SRI;
  std::set<ControlDependenceNode *> Visited;
  SRI.Root = buildRHGRecursively(CDG.getRoot(),Visited,F,CDG,SRI);

  //printRHG(SRI.Root);
  dotGraph(SRI);
  for (SEMERegion *R : SRI.Regions) delete R;
}

SEMERegionInfoPass::SEMERegionInfoPass() : FunctionPass(ID) {
  initializeSEMERegionInfoPassPass(*PassRegistry::getPassRegistry());
}

bool SEMERegionInfoPass::runOnFunction(Function &F) {
  PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
  ControlDependenceGraph CDG(F,PDT);
  buildRHG(F, CDG);
  return false;
}


char SEMERegionInfoPass::ID = 0;
INITIALIZE_PASS(SEMERegionInfoPass, "rhg",
	        "Build SEME Region Info",
                true, true)
