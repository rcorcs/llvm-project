
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

namespace llvm {

SEMERegionInfoPass::SEMERegionInfoPass() : FunctionPass(ID) {
  initializeSEMERegionInfoPassPass(*PassRegistry::getPassRegistry());
}

bool SEMERegionInfoPass::runOnFunction(Function &F) {
  ControlDependenceGraph &CDG = getAnalysis<ControlDependenceGraphPass>().getCDG();
  SRI.buildRegionsTree(F, CDG);
  return false;
}

}

namespace {

struct SEMERegionInfoPassGraphTraits {
  static SEMERegionInfo *getGraph(SEMERegionInfoPass *SRIP) {
    return &SRIP->getSEMERegionInfo();
  }
};

struct SEMERegionInfoPrinter
  : public DOTGraphTraitsPrinter<SEMERegionInfoPass, false, SEMERegionInfo*, SEMERegionInfoPassGraphTraits> {
  static char ID;
  SEMERegionInfoPrinter() :
    DOTGraphTraitsPrinter<SEMERegionInfoPass, false, SEMERegionInfo*, SEMERegionInfoPassGraphTraits>("rhg", ID) {
    initializeSEMERegionInfoPrinterPass(*PassRegistry::getPassRegistry());
  }
};

}

char SEMERegionInfoPrinter::ID = 0;
INITIALIZE_PASS(SEMERegionInfoPrinter, "dot-rhg",
	        "Print the SEME region hierarchy graph as a 'dot' file",
                true, true)

char SEMERegionInfoPass::ID = 0;
INITIALIZE_PASS(SEMERegionInfoPass, "rhg",
	        "Build SEME Region Info",
                true, true)
