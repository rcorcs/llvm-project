
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

SEMERegionLegacyPass::SEMERegionLegacyPass() : FunctionPass(ID) {
  initializeSEMERegionLegacyPassPass(*PassRegistry::getPassRegistry());
}

bool SEMERegionLegacyPass::runOnFunction(Function &F) {
  //errs() << "Legacy RHG: "<< F.getName() << "\n";
  ControlDependenceGraph &CDG = getAnalysis<ControlDependenceGraphPass>().getCDG();
  SRI.buildRegionsTree(F, CDG);
  CDG.releaseMemory();
  return false;
}

AnalysisKey SEMERegionAnalysis::Key;
SEMERegionInfo SEMERegionAnalysis::run(Function &F,
                                               FunctionAnalysisManager &AM) {
  //errs() << "RHG Analysis: "<< F.getName() << "\n";
  SEMERegionInfo SRI;
  ControlDependenceGraph &CDG = AM.getResult<ControlDependenceAnalysis>(F);
  SRI.buildRegionsTree(F, CDG);
  CDG.releaseMemory();
  return SRI;
}


}

namespace {

struct SEMERegionLegacyPassGraphTraits {
  static SEMERegionInfo *getGraph(SEMERegionLegacyPass *SRIP) {
    return &SRIP->getSEMERegionInfo();
  }
};

struct SEMERegionInfoPrinter
  : public DOTGraphTraitsPrinter<SEMERegionLegacyPass, false, SEMERegionInfo*, SEMERegionLegacyPassGraphTraits> {
  static char ID;
  SEMERegionInfoPrinter() :
    DOTGraphTraitsPrinter<SEMERegionLegacyPass, false, SEMERegionInfo*, SEMERegionLegacyPassGraphTraits>("rhg", ID) {
    initializeSEMERegionInfoPrinterPass(*PassRegistry::getPassRegistry());
  }
};

}

char SEMERegionInfoPrinter::ID = 0;
INITIALIZE_PASS(SEMERegionInfoPrinter, "dot-rhg",
	        "Print the SEME region hierarchy graph as a 'dot' file",
                true, true)

char SEMERegionLegacyPass::ID = 0;
//INITIALIZE_PASS(SEMERegionLegacyPass, "rhg",
//	        "Build SEME Region Info",
//                true, true)

INITIALIZE_PASS_BEGIN(SEMERegionLegacyPass, "rhg",
                      "Build SEME Region Info", true, true)
INITIALIZE_PASS_DEPENDENCY(ControlDependenceGraphPass)
INITIALIZE_PASS_END(SEMERegionLegacyPass, "rhg",
                      "Build SEME Region Info", true, true)


static void writeSEMERegionToDotFile(Function &F, SEMERegionInfo &SRI) {
  std::string Filename = "rhg.";
  Filename += (F.getName() + ".dot").str();
  errs() << "Writing '" << Filename << "'...";

  std::error_code EC;
  raw_fd_ostream File(Filename, EC, sys::fs::OF_Text);

  if (!EC)
    WriteGraph(File, &SRI, true);
  else
    errs() << "  error opening file for writing!";
  errs() << "\n";
}

PreservedAnalyses SEMERegionPrinterPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {

  SEMERegionInfo SRI;

  errs() << "Running function analysis: CDG\n";
  ControlDependenceGraph &CDG = AM.getResult<ControlDependenceAnalysis>(F);
  errs() << "Building SRI\n";
  SRI.buildRegionsTree(F, CDG);
  errs() << "writing DOT file\n";
  writeSEMERegionToDotFile(F, SRI);
  errs() << "Done! writing DOT file\n";
  CDG.releaseMemory();
  return PreservedAnalyses::all();
}

