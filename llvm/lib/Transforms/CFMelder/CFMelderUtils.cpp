#include "CFMelderUtils.h"
#include "InstructionMatch.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
using namespace llvm;

// check for diamond shaped control-flow
bool Utils::isValidMergeLocation(BasicBlock &BB, PostDominatorTree &PDT) {

  const BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator());

  if (!BI || BI->getNumSuccessors() < 2)
    return false;

  if (BI->getNumSuccessors() > 2) {
    DEBUG << "BI has more than 2 sucecessors, this case is unhandled\n";
    return false;
  }

  BasicBlock *BBLeft = BI->getSuccessor(0);
  BasicBlock *BBRight = BI->getSuccessor(1);

  // there can not be edges between successors of BB
  if (Utils::hasEdgeBetween(BBLeft, BBRight))
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
  BasicBlock * IPDom = PDT.getNode(&BB)->getIDom()->getBlock();
  SmallVector<BasicBlock *, 32> WorkList;
  SmallSet<BasicBlock *, 32> Visited;
  WorkList.push_back(BBRight);
  WorkList.push_back(BBLeft);
  while(!WorkList.empty()){
    BasicBlock* Curr = WorkList.pop_back_val();
    Visited.insert(Curr);
    if (containsUnhandledInstructions(Curr)) return false;
    for (unsigned I = 0; I < Curr->getTerminator()->getNumSuccessors(); I++){
      BasicBlock* Succ = Curr->getTerminator()->getSuccessor(I);
      if (Visited.find(Succ) == Visited.end() && Succ != &BB && Succ != IPDom){
        WorkList.push_back(Succ);
      }
    }
  }


  return true;
}

bool Utils::containsUnhandledInstructions(const BasicBlock* BB) {
  for(const Instruction& I : *BB){
    if (I.getOpcode() == Instruction::Switch) return true;
  }
  return false;
}


bool Utils::hasEdgeBetween(BasicBlock *BB1, BasicBlock *BB2) {

  for (auto It = succ_begin(BB1); It != succ_end(BB1); ++It) {
    if (*It == BB2)
      return true;
  }

  for (auto It = succ_begin(BB2); It != succ_end(BB2); ++It) {
    if (*It == BB1)
      return true;
  }

  return false;
}
// takes in a BB with multiple succsessors and make it a BB with single
// successor by moving the branch to a new BB. returns the new BB. if input BB
// is already has a single successor return null
BasicBlock *Utils::makeSingleExit(BasicBlock *BB) {
  BasicBlock *NewBb = nullptr;

  if (BB->getTerminator()->getNumSuccessors() > 1) {
    BranchInst *BI = dyn_cast<BranchInst>(BB->getTerminator());

    NewBb = BasicBlock::Create(BB->getParent()->getContext(), "region.exit.bb",
                               BB->getParent(), BB);
    BranchInst *NewBi = BranchInst::Create(
        BI->getSuccessor(0), BI->getSuccessor(1), BI->getCondition(), NewBb);
    BI->replaceAllUsesWith(NewBi);
    BI->eraseFromParent();
    BranchInst::Create(NewBb, BB);
  }

  return NewBb;
}

void Utils::getMultiSuccessorExitBlocks(Region *R,
                                        SmallVectorImpl<BasicBlock *> &BBs) {
  if (R->getExit() && R->getExit()->getTerminator()->getNumSuccessors() > 1) {
    BBs.push_back(R->getExit());
  }

  for (auto It = R->begin(); It != R->end(); It++) {
    Region *SubR = &(**It);
    getMultiSuccessorExitBlocks(SubR, BBs);
  }
}

std::pair<unsigned, unsigned> Utils::computeLatReductionAtBest(BasicBlock *BB1,
                                            BasicBlock *BB2) {
  DenseMap<unsigned, std::pair<unsigned, unsigned>> FreqMap;
  unsigned LatReducedAtBest = 0;
  unsigned TotalLatency = 0;
  for (auto It = BB1->begin(); It != BB1->end(); ++It) {
    if (FreqMap.find(It->getOpcode()) == FreqMap.end())
      FreqMap[It->getOpcode()] = std::make_pair(0, 0);

    FreqMap[It->getOpcode()].first += InstructionMatch::getInstructionCost(&(*It));
    TotalLatency += InstructionMatch::getInstructionCost(&(*It));
  }

  for (auto It = BB2->begin(); It != BB2->end(); ++It) {
    if (FreqMap.find(It->getOpcode()) == FreqMap.end())
      FreqMap[It->getOpcode()] = std::make_pair(0, 0);

    FreqMap[It->getOpcode()].second += InstructionMatch::getInstructionCost(&(*It));
    TotalLatency += InstructionMatch::getInstructionCost(&(*It));
  }

  for (auto It : FreqMap) {
    std::pair<unsigned, unsigned> &Counts = It.second;
    // PHI nodes are not melded
    if(It.first == Instruction::PHI){
      continue;
    }
    
    LatReducedAtBest += std::min(Counts.first, Counts.second);
  }

  return std::pair<unsigned, unsigned>(LatReducedAtBest, TotalLatency);
}

double Utils::computeBBSimilarty(BasicBlock *BB1, BasicBlock *BB2) {
  // double NMergeableAtBest = (double)computeMaxNumMergeableInsts(BB1, BB2);
  // double TotalInsts = (double)(BB1->size() + BB2->size());
  // return NMergeableAtBest / TotalInsts;
  auto LatInfo = computeLatReductionAtBest(BB1,BB2);
  double LatReductionAtBest = (double)LatInfo.first;
  double TotalLat = (double)LatInfo.second;
  // errs() << "latency reduction at best in bb : " << LatReductionAtBest << "\n";
  // errs() << "total latency in bb: " << TotalLat << "\n";
  return LatReductionAtBest/TotalLat;
}

double Utils::computeRegionSimilarity(
    const DenseMap<BasicBlock *, BasicBlock *> &Mapping, BasicBlock* LExit) {
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
    // errs() << "BB left : " << It.first->getName() << ", BB right : " << It.second->getName() << "\n";
    // errs() << "Lat reduction in bb  : " << LatInfo.first << "\n";
    // errs() << "Total lat in bb: " << LatInfo.second << "\n";
  }
  // errs() << "latency reduction at best in region: " << LatReductionAtBest << "\n";
  // errs() << "total latency in region: " << TotalLat << "\n";
  return (double)LatReductionAtBest / (double)TotalLat;
}
