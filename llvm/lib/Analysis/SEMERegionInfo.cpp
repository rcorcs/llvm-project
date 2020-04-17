
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

SEMERegionInfoPass::SEMERegionInfoPass() : FunctionPass(ID) {
  initializeSEMERegionInfoPassPass(*PassRegistry::getPassRegistry());
}

bool SEMERegionInfoPass::runOnFunction(Function &F) {
  PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
  ControlDependenceGraph CDG(F,PDT);
  SEMERegionInfo SRI;
  SRI.buildRHG(F, CDG);
  SRI.dotGraph();
  return false;
}


char SEMERegionInfoPass::ID = 0;
INITIALIZE_PASS(SEMERegionInfoPass, "rhg",
	        "Build SEME Region Info",
                true, true)
