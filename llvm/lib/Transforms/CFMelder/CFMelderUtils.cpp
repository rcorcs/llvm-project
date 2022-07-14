#include "CFMelderUtils.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/CFGPrinter.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <sstream>
using namespace llvm;

bool Utils::requireRegionSimplification(Region *R) {
  BasicBlock *Exit = R->getExit();
  for (auto PredIt = pred_begin(Exit); PredIt != pred_end(Exit); ++PredIt) {
    BasicBlock *Pred = *PredIt;
    if (!R->contains(Pred))
      return true;
  }
  return false;
}

Region *Utils::getRegionWithEntryExit(RegionInfo &RI, BasicBlock *Entry,
                                      BasicBlock *Exit) {

  // errs() << "entry : " << Entry->getNameOrAsOperand() << "\n";
  // errs() << "exit : " << Exit->getNameOrAsOperand() << "\n";
  SmallVector<Region *, 32> WorkList;
  WorkList.push_back(RI.getTopLevelRegion());
  Region *RFound = nullptr;
  while (!WorkList.empty()) {
    Region *R = WorkList.pop_back_val();
    // R->print(errs());
    if (R->getEntry() == Entry && R->getExit() == Exit) {
      // errs() << "found\n";
      RFound = R;
      break;
    }

    for (std::unique_ptr<Region> &SubR : *R) {
      WorkList.push_back(SubR.get());
    }
  }
  return RFound;
}

bool Utils::isValidMergeLocation(BasicBlock &BB, DominatorTree &DT,
                                 PostDominatorTree &PDT) {

  const BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator());

  if (!BI)
    return false;

  if (BI->getNumSuccessors() != 2) {
    return false;
  }

  BasicBlock *BBLeft = BI->getSuccessor(0);
  BasicBlock *BBRight = BI->getSuccessor(1);

  // BB must dominate both successors
  if (!DT.dominates(&BB, BBLeft) || !DT.dominates(&BB, BBRight))
    return false;

  // there can not be edges between successors of BB
  if (Utils::hasPathBetween(BBLeft, BBRight, DT))
    return false;

  // successors of BB can not post-dominate each other
  if (PDT.dominates(BBLeft, BBRight) || PDT.dominates(BBRight, BBLeft))
    return false;

  if (!PDT.getNode(&BB)->getIDom()->getBlock()) {
    DEBUG << "No IPDOM for block " << BB.getName()
          << ", This case is not considered\n";
    return false;
  }

  // FIXME : filter regions that contain swithc instructions for now
  BasicBlock *IPDom = PDT.getNode(&BB)->getIDom()->getBlock();
  SmallVector<BasicBlock *, 32> WorkList;
  SmallSet<BasicBlock *, 32> Visited;
  WorkList.push_back(BBRight);
  WorkList.push_back(BBLeft);
  while (!WorkList.empty()) {
    BasicBlock *Curr = WorkList.pop_back_val();
    Visited.insert(Curr);
    if (containsUnhandledInstructions(Curr))
      return false;
    for (unsigned I = 0; I < Curr->getTerminator()->getNumSuccessors(); I++) {
      BasicBlock *Succ = Curr->getTerminator()->getSuccessor(I);
      if (Visited.find(Succ) == Visited.end() && Succ != &BB && Succ != IPDom) {
        WorkList.push_back(Succ);
      }
    }
  }

  return true;
}

bool Utils::containsUnhandledInstructions(const BasicBlock *BB) {
  for (const Instruction &I : *BB) {
    if (I.getOpcode() == Instruction::Switch)
      return true;
  }
  return false;
}

bool Utils::hasPathBetween(BasicBlock *L, BasicBlock *R, DominatorTree &DT) {

  // path exists if any of the predecessors of L is dominated by R
  for (auto It = pred_begin(L); It != pred_end(R); ++It) {
    if (DT.dominates(R, *It))
      return true;
  }
  // OR, any of the predecessors of R are dominated by L
  for (auto It = pred_begin(R); It != pred_end(R); ++It) {
    if (DT.dominates(L, *It))
      return true;
  }

  return false;
}

std::pair<unsigned, unsigned>
Utils::computeLatReductionAtBest(BasicBlock *BB1, BasicBlock *BB2) {
  DenseMap<unsigned, std::pair<unsigned, unsigned>> FreqMap;
  unsigned LatReducedAtBest = 0;
  unsigned TotalLatency = 0;
  for (auto It = BB1->begin(); It != BB1->end(); ++It) {
    if (FreqMap.find(It->getOpcode()) == FreqMap.end())
      FreqMap[It->getOpcode()] = std::make_pair(0, 0);

    FreqMap[It->getOpcode()].first += Utils::getInstructionCost(&(*It));
    TotalLatency += Utils::getInstructionCost(&(*It));
  }

  for (auto It = BB2->begin(); It != BB2->end(); ++It) {
    if (FreqMap.find(It->getOpcode()) == FreqMap.end())
      FreqMap[It->getOpcode()] = std::make_pair(0, 0);

    FreqMap[It->getOpcode()].second += Utils::getInstructionCost(&(*It));
    TotalLatency += Utils::getInstructionCost(&(*It));
  }

  for (auto It : FreqMap) {
    std::pair<unsigned, unsigned> &Counts = It.second;
    // PHI nodes are not melded
    if (It.first == Instruction::PHI) {
      continue;
    }

    LatReducedAtBest += std::min(Counts.first, Counts.second);
  }

  return std::pair<unsigned, unsigned>(LatReducedAtBest, TotalLatency);
}

double Utils::computeBlockSimilarity(BasicBlock *BB1, BasicBlock *BB2) {
  // double NMergeableAtBest = (double)computeMaxNumMergeableInsts(BB1, BB2);
  // double TotalInsts = (double)(BB1->size() + BB2->size());
  // return NMergeableAtBest / TotalInsts;
  auto LatInfo = computeLatReductionAtBest(BB1, BB2);
  double LatReductionAtBest = (double)LatInfo.first;
  double TotalLat = (double)LatInfo.second;
  // errs() << "latency reduction at best in bb : " << LatReductionAtBest <<
  // "\n"; errs() << "total latency in bb: " << TotalLat << "\n";
  return LatReductionAtBest / TotalLat;
}

double Utils::computeBlockSimilarity(BasicBlock *BB1, BasicBlock *BB2,
                                     Region *Replicated,
                                     function_ref<int()> GetBrCost) {
  int LatReductionAtBest = 0, TotalLatency = 0;
  // iterate over all blocks in replicated region
  for (auto *BB : Replicated->blocks()) {
    // for the matched blocks compute the instructions that can merged in best case
    if (BB == BB1 || BB == BB2) {
      auto LatInfo = computeLatReductionAtBest(BB1, BB2);
      LatReductionAtBest += LatInfo.first;
      TotalLatency += LatInfo.second;
    } 
    // else you have to pay one more branch to execute the block conditionally
    else {
      for (Instruction &I : *BB) {
        TotalLatency += Utils::getInstructionCost(&I);
      }
      LatReductionAtBest -= GetBrCost();
    }
  }

  return (double)LatReductionAtBest / (double)TotalLatency;
}

double Utils::computeRegionSimilarity(
    const DenseMap<BasicBlock *, BasicBlock *> &Mapping, BasicBlock *LExit) {
  unsigned LatReductionAtBest = 0, TotalLat = 0;
  for (auto It : Mapping) {
    // exit is not melded, ignore it's profitablity
    if (It.first == LExit)
      continue;
    auto LatInfo = computeLatReductionAtBest(It.first, It.second);

    // NMergeableAtBest += computeMaxNumMergeableInsts(It.first, It.second);
    // TotalInsts += (unsigned)(It.first->size() + It.second->size());
    LatReductionAtBest += LatInfo.first;
    TotalLat += LatInfo.second;
    // errs() << "BB left : " << It.first->getName() << ", BB right : " <<
    // It.second->getName() << "\n"; errs() << "Lat reduction in bb  : " <<
    // LatInfo.first << "\n"; errs() << "Total lat in bb: " << LatInfo.second <<
    // "\n";
  }
  // errs() << "latency reduction at best in region: " << LatReductionAtBest <<
  // "\n"; errs() << "total latency in region: " << TotalLat << "\n";
  return (double)LatReductionAtBest / (double)TotalLat;
}

int Utils::getInstructionCost(Instruction *I) {
  int SavedCycles = 0;
  switch (I->getOpcode()) {
  case Instruction::Add:
  case Instruction::FAdd:
    SavedCycles = 5;
    break;
  case Instruction::Mul:
  case Instruction::FMul:
    SavedCycles = 10;
    break;
  case Instruction::SDiv:
  case Instruction::UDiv:
  case Instruction::FDiv:
    SavedCycles = 20;
    break;
  case Instruction::Store:
  case Instruction::Load:
    SavedCycles = 100;
    break;
  default:
    SavedCycles = 3;
    break;
  }
  return SavedCycles;
}

std::string Utils::getNameStr(Value *V) {
  std::string Out;
  llvm::raw_string_ostream SS(Out);
  V->printAsOperand(SS, false);
  return SS.str();
}

void Utils::writeCFGToDotFile(Function &F, std::string Prefix = "") {
  std::string Filename = (Prefix + F.getName() + ".dot").str();
  errs() << "Writing '" << Filename << "'...";

  std::error_code EC;
  raw_fd_ostream File(Filename, EC, sys::fs::OF_Text);

  DOTFuncInfo CFGInfo(&F);

  if (!EC)
    WriteGraph(File, &CFGInfo);
  else
    errs() << "  error opening file for writing!";
  errs() << "\n";
}
