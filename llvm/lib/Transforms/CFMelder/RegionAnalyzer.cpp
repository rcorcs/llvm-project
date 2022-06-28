#include "RegionAnalyzer.h"
#include "SmithWaterman.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/CodeGen/DIE.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/MCA/HardwareUnits/RetireControlUnit.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TableGen/Record.h"
#include <algorithm>
#include <memory>

using namespace llvm;

static cl::opt<double> SimilarityThreshold(
    "cf-merging-similarity-threshold", cl::init(0.2), cl::Hidden,
    cl::desc(
        "Minimum similarity required to merge two basicblocks or two regions"));

static cl::opt<bool> PerformGreedyRegionMatch(
    "use-greedy-region-match", cl::init(false), cl::Hidden,
    cl::desc("Enable greedy region match in control-flow melding"));

static cl::opt<bool> DisableRegionReplication(
    "disable-region-replication", cl::init(false), cl::Hidden,
    cl::desc("Disable replicating regions in control-flow melding"));

static cl::opt<bool>
    RunBranchFusionOnly("run-branch-fusion-only", cl::init(false), cl::Hidden,
                        cl::desc("Run branch fusion only, no region-melding"));


void ControlFlowGraphInfo::recompute() {
  DT.recalculate(getFunction());
  PDT.recalculate(getFunction());

  LI = std::make_shared<LoopInfo>(DT);

  /// calculate regions
  DominanceFrontier DF;
  DF.analyze(DT);
  RI = std::make_shared<RegionInfo>();
  RI->recalculate(getFunction(), &DT, &PDT, &DF);
}

ControlFlowGraphInfo::ControlFlowGraphInfo(Function &F, DominatorTree &DT, PostDominatorTree &PDT)
    : F(F), DT(DT), PDT(PDT) {
  LI = std::make_shared<LoopInfo>(DT);
  /// calculate regions
  DominanceFrontier DF;
  DF.analyze(DT);
  RI = std::make_shared<RegionInfo>();
  RI->recalculate(F, &DT, &PDT, &DF);
}


bool RegionComparator::compare() {
  DenseMap<BasicBlock *, int> LabelMapR1;
  DenseMap<BasicBlock *, int> LabelMapR2;
  SmallVector<BasicBlock *, 0> StackR1;
  SmallVector<BasicBlock *, 0> StackR2;

  StackR1.push_back(R1->getEntry());
  StackR2.push_back(R2->getEntry());
  // do a parallel DFS traversal
  while (!StackR1.empty()) {
    BasicBlock *CurrBlockR1 = StackR1.pop_back_val();
    BasicBlock *CurrBlockR2 = StackR2.pop_back_val();
    // num of successors must match
    if (CurrBlockR1->getTerminator()->getNumSuccessors() !=
        CurrBlockR2->getTerminator()->getNumSuccessors())
      return false;
    // add to map
    LabelMapR1.insert(
        std::pair<BasicBlock *, int>(CurrBlockR1, LabelMapR1.size()));
    LabelMapR2.insert(
        std::pair<BasicBlock *, int>(CurrBlockR2, LabelMapR2.size()));
    // update the mapping
    Mapping.insert(
        std::pair<BasicBlock *, BasicBlock *>(CurrBlockR1, CurrBlockR2));

    // iterate over all successors
    for (unsigned I = 0; I < CurrBlockR1->getTerminator()->getNumSuccessors();
         I++) {
      BasicBlock *SuccR1 = CurrBlockR1->getTerminator()->getSuccessor(I);
      BasicBlock *SuccR2 = CurrBlockR2->getTerminator()->getSuccessor(I);
      // check if we already visited this successor
      auto ItR1 = LabelMapR1.find(SuccR1);
      auto ItR2 = LabelMapR2.find(SuccR2);

      int SuccLabelR1 = -1, SuccLabelR2 = -1;

      if (ItR1 != LabelMapR1.end())
        SuccLabelR1 = ItR1->second;

      if (ItR2 != LabelMapR2.end())
        SuccLabelR2 = ItR2->second;

      if (SuccLabelR1 != SuccLabelR2)
        return false;

      // check if exit block is consisitent
      if ((SuccR1 == R1->getExit() && SuccR2 != R2->getExit()) ||
          (SuccR1 != R1->getExit() && SuccR2 == R2->getExit()))
        return false;

      // add succ to stack only if successor is not the exit block and
      // it is not visited before
      if (SuccR1 != R1->getExit() && SuccLabelR1 == -1 &&
          SuccR2 != R2->getExit() && SuccLabelR2 == -1) {
        StackR1.push_back(SuccR1);
        StackR2.push_back(SuccR2);
      }
    }
  }
  return true;
}

void RegionComparator::getMapping(
    DenseMap<BasicBlock *, BasicBlock *> &Result) {
  Result.clear();
  Result.insert(Mapping.begin(), Mapping.end());
}

MergeableRegionPair::MergeableRegionPair(Region &R1, Region &R2,
                                         RegionComparator &Comparator) {
  LEntry = R1.getEntry();
  LExit = R1.getExit();
  REntry = R2.getEntry();
  RExit = R2.getExit();
  Comparator.getMapping(this->Mapping);
  // CalculateSimilarityScore();
  SimilarityScore = Utils::computeRegionSimilarity(Mapping, LExit);
  // errs() << "Similarity score : " << similarityScore << "\n";
}

BasicBlock *MergeableRegionPair::getMatchingRightBb(BasicBlock *BB) {
  auto It = Mapping.find(BB);
  assert(It != Mapping.end() && "Matching Right BB is not found for BB");
  return It->second;
}

bool MergeableRegionPair::dominates(std::shared_ptr<MergeableRegionPair> &Other,
                                    DominatorTree &DT) {
  if (!DT.dominates(getLeftEntry(), Other->getLeftEntry()))
    return false;

  if (!DT.dominates(getRightEntry(), Other->getRightEntry()))
    return false;

  return true;
}

RegionAnalyzer::RegionAnalyzer(BasicBlock *BB, ControlFlowGraphInfo &CFGInfo)
    : DivergentBB(BB), CFGInfo(CFGInfo) {

  BranchInst *Bi = dyn_cast<BranchInst>(DivergentBB->getTerminator());
  assert(Bi && Bi->isConditional() &&
         "Top BB needs to have a conditional branch");
  DivergentCondition = Bi->getCondition();
}

void RegionAnalyzer::computeSARegionMatch() {
  RegionMeldingProfitabilityModel ScoringFunc;
  auto SMSA =
      SmithWaterman<Region *, SmallVectorImpl<Region *>, nullptr>(ScoringFunc);

  auto Result = SMSA.compute(LeftRegions, RightRegions);
  int AlignedReginPairs = 0;
  for (auto Entry : Result) {
    Region *L = Entry.getLeft();
    Region *R = Entry.getRight();
    if (Entry.match()) {
      RegionComparator RC(L, R);
      bool Check = RC.compare();
      assert(Check && "Aligned regions are not similar!");
      std::shared_ptr<MergeableRegionPair> RegionPair =
          std::make_shared<MergeableRegionPair>(*L, *R, RC);

      BestRegionMatch.push_back(RegionPair);
      AlignedReginPairs++;
    }
  }

  INFO << "Number of aligned region pairs : " << AlignedReginPairs << "\n";
}

void RegionAnalyzer::computeGreedyRegionMatch() {
  SmallVector<std::shared_ptr<MergeableRegionPair>, 0> MergeableRegionPairs;
  // case 3 : do a N*N region simlarty check
  DEBUG << "Performing a N*N region similarity check\n";
  for (Region *LRegion : LeftRegions) {
    for (Region *RRegion : RightRegions) {
      RegionComparator RC(LRegion, RRegion);
      if (RC.compare()) {
        std::shared_ptr<MergeableRegionPair> RegionPair =
            std::make_shared<MergeableRegionPair>(*LRegion, *RRegion, RC);

        // DEBUG << "Regions are similar\n";
        // DEBUG << *regionPair << "\n";
        // DEBUG << "Similarity score is " << regionPair->GetSimilarityScore()
        //       << "\n";

        MergeableRegionPairs.push_back(RegionPair);
      } else {
        // DEBUG << "Regions are not similar\n";
      }
    }
  }

  DominatorTree &DT = getCFGInfo().getDomTree();
  // Sort the mergeable region pairs based on their simlarity
  auto Comparator = [&](std::shared_ptr<MergeableRegionPair> &RP1,
                        std::shared_ptr<MergeableRegionPair> &RP2) {
    if (*RP1 == *RP2) {
      return RP1->dominates(RP2, DT);
    }
    return *RP1 > *RP2;
  };
  std::sort(MergeableRegionPairs.begin(), MergeableRegionPairs.end(),
            Comparator);

  // add the most profitable match first
  if (MergeableRegionPairs.size() > 0) {
    BestRegionMatch.push_back(MergeableRegionPairs[0]);
  }

  // then add next best mergeable pair given that prev best dominates
  // next best in L and R paths
  for (unsigned I = 1; I < MergeableRegionPairs.size(); I++) {
    auto &MRPair = MergeableRegionPairs[I];

    auto &CurrBest = BestRegionMatch[BestRegionMatch.size() - 1];

    if (CurrBest->dominates(MRPair, DT))
      BestRegionMatch.push_back(MRPair);
  }
}

void RegionAnalyzer::computeRegionMatch() {

  BasicBlock *LeftEntry = DivergentBB->getTerminator()->getSuccessor(0);
  BasicBlock *RightEntry = DivergentBB->getTerminator()->getSuccessor(1);

  // running branch fusion only
  if (RunBranchFusionOnly) {
    BasicBlock *LeftUniqueSucc = LeftEntry->getUniqueSuccessor();
    BasicBlock *RightUniqueSucc = RightEntry->getUniqueSuccessor();
    if (LeftUniqueSucc && RightUniqueSucc &&
        LeftUniqueSucc == RightUniqueSucc) {
      BestBbMatchSimilarityScore =
          Utils::computeBBSimilarty(LeftEntry, RightEntry);
      DEBUG << "Branch fusion can be applied to basic blocks ";
      LeftEntry->printAsOperand(errs(), false);
      errs() << ", ";
      RightEntry->printAsOperand(errs(), false);
      errs() << ", similarity score" << BestBbMatchSimilarityScore << "\n";
      BestBbMatch.first = LeftEntry;
      BestBbMatch.second = RightEntry;
      HasBbMatch = true;
    }
    return;
  }

  // find the regions on left and right paths
  findMergeableRegions(*DivergentBB);

  // Case 1 : No regions found. L and R paths have single BB
  if (LeftRegions.empty() && RightRegions.empty()) {
    BestBbMatchSimilarityScore =
        Utils::computeBBSimilarty(LeftEntry, RightEntry);
    DEBUG << "Basic blocks ";
    LeftEntry->printAsOperand(errs(), false);
    errs() << ", ";
    RightEntry->printAsOperand(errs(), false);
    errs() << " can be melded, similarity score " << BestBbMatchSimilarityScore
           << "\n";

    BestBbMatch.first = LeftEntry;
    BestBbMatch.second = RightEntry;
    HasBbMatch = true;
    return;
  }

  // Case 2 : L or R path has single BB
  if (LeftRegions.empty() || RightRegions.empty()) {

    if (LeftRegions.empty()) {
      BestBbMatch.first = LeftEntry;
    }
    if (RightRegions.empty()) {
      BestBbMatch.second = RightEntry;
    }

    PostDominatorTree &PDT = getCFGInfo().getPostDomTree();
    SmallVector<BasicBlock *, 8> MergeableBlocks;
    BasicBlock *IPDom = PDT.getNode(DivergentBB)->getIDom()->getBlock();

    assert(IPDom && "No IPDOM for divergent branch! This case is not handled");

    if (!BestBbMatch.first) {
      assert(LeftRegions.size() > 0 && BestBbMatch.second);
      findMergeableBBsInPath(LeftEntry, IPDom, MergeableBlocks);
      if (MergeableBlocks.size()) {
        BestBbMatch.first =
            findMostSimilarBb(BestBbMatch.second, MergeableBlocks);
      }
    }

    if (!BestBbMatch.second) {
      assert(RightRegions.size() > 0 && BestBbMatch.first);
      MergeableBlocks.clear();
      findMergeableBBsInPath(RightEntry, IPDom, MergeableBlocks);
      if (MergeableBlocks.size()) {
        BestBbMatch.second =
            findMostSimilarBb(BestBbMatch.first, MergeableBlocks);
      }
    }

    // if profitable match is found
    if (BestBbMatch.first && BestBbMatch.second) {
      BestBbMatchSimilarityScore =
          Utils::computeBBSimilarty(BestBbMatch.first, BestBbMatch.second);
      DEBUG << "BB to region merge detected for basic blocks ";
      BestBbMatch.first->printAsOperand(errs(), false);
      errs() << " ,";
      BestBbMatch.second->printAsOperand(errs(), false);
      errs();
      errs() << ", similarity score " << BestBbMatchSimilarityScore << "\n";
      HasBbMatch = true;
    }

    return;
  }
  if (PerformGreedyRegionMatch)
    computeGreedyRegionMatch();
  else
    computeSARegionMatch();

  DEBUG << "Region pairs in profitabilty order  \n";
  for (unsigned I = 0; I < BestRegionMatch.size(); ++I) {
    auto BestPair = BestRegionMatch[I];
    DEBUG << *BestPair
          << ", similarity score = " << BestPair->getSimilarityScore() << "\n";
  }
}

BasicBlock *
RegionAnalyzer::findMostSimilarBb(BasicBlock *BB,
                                  SmallVectorImpl<BasicBlock *> &Candidates) {
  assert(Candidates.size() > 0 && "empty basicblock candidate list!");
  BasicBlock *MostSimilar = Candidates[0];
  double BestScore = Utils::computeBBSimilarty(BB, MostSimilar);
  // MergeableRegionPair::ComputeBBSimilarity(BB, candidates[0]);
  for (BasicBlock *Candidate : Candidates) {
    double Score = Utils::computeBBSimilarty(BB, Candidate);
    if (Score > BestScore) {
      MostSimilar = Candidate;
      BestScore = Score;
    }
  }

  return MostSimilar;
}

// check if region list is in dominance order
bool verifyRegionList(SmallVectorImpl<Region *> &Regions, DominatorTree &DT) {
  for (auto It = Regions.begin(); It != Regions.end(); It++) {
    // check if the list is in dominance order
    if ((It + 1) != Regions.end()) {
      Region *Curr = *It;
      Region *Next = *(It + 1);
      if (!DT.dominates(Curr->getEntry(), Next->getEntry()))
        return false;
    }
  }
  return true;
}

void printRegionList(SmallVectorImpl<Region *> &Regions, DominatorTree &DT) {
  for (auto It = Regions.begin(); It != Regions.end(); It++) {

    errs() << "[ ";
    (*It)->getEntry()->printAsOperand(errs(), false);
    errs() << " : ";
    (*It)->getExit()->printAsOperand(errs(), false);
    errs() << " ]";
  }
}

void RegionAnalyzer::findMergeableBBsInPath(
    BasicBlock *From, BasicBlock *To,
    SmallVectorImpl<BasicBlock *> &MeregeableBBs) {
  
  auto LI = getCFGInfo().getLoopInfo();
  DominatorTree &DT = getCFGInfo().getDomTree();
  PostDominatorTree &PDT = getCFGInfo().getPostDomTree();

  SmallVector<BasicBlock *, 16> WorkList;
  SmallVector<BasicBlock *, 16> Visited;
  WorkList.push_back(From);

  while (!WorkList.empty()) {
    BasicBlock *Cand = WorkList.pop_back_val();
    Visited.push_back(Cand);

    // FIXME : avoid merging with basic blocks inside loops
    bool InsideLoop = false;
    Loop *LoopOfCand = LI->getLoopFor(Cand);
    if (LoopOfCand) {
      BasicBlock *LoopHeder = LoopOfCand->getHeader();
      if (DT.dominates(From, LoopHeder)) {
        // DEBUG << "Ignoring basic blocks inside loops for region replication "
        //          "candidates : block "
        //       << Cand->getNameOrAsOperand() << "\n";
        InsideLoop = true;
      }
    }

    // if region replication is not allowed, basic blocks that post dominates
    // From can be a meld candidate
    if (!InsideLoop) {
      if (DisableRegionReplication) {
        if (PDT.dominates(Cand, From))
          MeregeableBBs.push_back(Cand);
      } else {
        MeregeableBBs.push_back(Cand);
      }
    }

    for (auto It = succ_begin(Cand); It != succ_end(Cand); ++It) {
      BasicBlock *Succ = *It;
      // add all univisted successors of current BB that are not equal to "To"
      // AND each successor must be dominated by "From"
      if (Succ != To && DT.dominates(From, Succ) &&
          std::find(Visited.begin(), Visited.end(), Succ) == Visited.end())
        WorkList.push_back(Succ);
    }
  }
}

void RegionAnalyzer::findMergeableRegions(BasicBlock &BB) {
  auto RI = getCFGInfo().getRegionInfo();
  DominatorTree &DT = getCFGInfo().getDomTree();
  PostDominatorTree &PDT = getCFGInfo().getPostDomTree();

  Region *R = RI->getRegionFor(&BB);

  assert(BB.getTerminator()->getNumSuccessors() == 2 &&
         "CFMelder : Entry block must have 2 successors!");

  BasicBlock *LeftEntry = BB.getTerminator()->getSuccessor(0);
  BasicBlock *RightEntry = BB.getTerminator()->getSuccessor(1);

  SmallVector<Region *, 0> VisitedRegions;

  // get all direct children of R and split them L and R paths
  // regions are added to L  and R lists based on dominance order
  for (auto It = R->begin(); It != R->end(); It++) {
    Region &SubR = **It;

    // if sub region is not direct child of R skip
    if (SubR.getParent() != R)
      continue;

    BasicBlock *EntryBb = SubR.getEntry();

    // check if this region beglongs to left or right paths
    if (DT.dominates(LeftEntry, EntryBb) && PDT.dominates(EntryBb, LeftEntry)) {

      auto ItLR = LeftRegions.begin();
      for (; ItLR != LeftRegions.end(); ItLR++) {
        if (DT.dominates(EntryBb, (*ItLR)->getEntry()))
          break;
      }

      LeftRegions.insert(ItLR, &SubR);
    }

    if (DT.dominates(RightEntry, EntryBb) && PDT.dominates(EntryBb, RightEntry)) {
      // add regions based on dominance order
      auto ItRR = RightRegions.begin();
      for (; ItRR != RightRegions.end(); ItRR++) {
        if (DT.dominates(EntryBb, (*ItRR)->getEntry()))
          break;
      }
      RightRegions.insert(ItRR, &SubR);
    }
  }

#ifdef CFMELDER_DEBUG
  DEBUG << "Left regions : ";
  printRegionList(LeftRegions, DT);
  errs() << "\n";
  DEBUG << "Right regions : ";
  printRegionList(RightRegions, DT);
  errs() << "\n";
#endif

  // verify dominance order
  assert(verifyRegionList(LeftRegions, DT) &&
         "Left regions are not in dominance order!");
  assert(verifyRegionList(RightRegions, DT) &&
         "Right regions are not in dominance order!");
}

unsigned RegionAnalyzer::regionMatchSize() const {
  if (HasBbMatch)
    return 1;
  return BestRegionMatch.size();
}

std::pair<BasicBlock *, BasicBlock *>
RegionAnalyzer::getRegionMatchEntryBlocks(unsigned I) {

  if (HasBbMatch)
    return BestBbMatch;
  std::shared_ptr<MergeableRegionPair> RP = BestRegionMatch[I];

  return std::pair<BasicBlock *, BasicBlock *>(RP->getLeftEntry(),
                                               RP->getRightEntry());
}

std::pair<BasicBlock *, BasicBlock *>
RegionAnalyzer::getRegionMatchExitBlocks(unsigned I) {
  // for single BB match exit block is null
  if (HasBbMatch)
    return std::pair<BasicBlock *, BasicBlock *>(nullptr, nullptr);
  std::shared_ptr<MergeableRegionPair> RP = BestRegionMatch[I];

  return std::pair<BasicBlock *, BasicBlock *>(RP->getLeftExit(),
                                               RP->getRightExit());
}

DenseMap<BasicBlock *, BasicBlock *>
RegionAnalyzer::getRegionMatch(unsigned I) {
  DenseMap<BasicBlock *, BasicBlock *> BbMap;

  if (HasBbMatch) {
    assert(I == 0 && "only one basicblock match is expected");
    BbMap.insert(BestBbMatch);
    return BbMap;
  }

  assert(I < BestRegionMatch.size() &&
         "requested list index is greater than its size!");

  std::shared_ptr<MergeableRegionPair> RP = BestRegionMatch[I];

  SmallVector<BasicBlock *, 0> WorkList;
  SmallVector<BasicBlock *, 0> VisitedBBs;
  WorkList.push_back(RP->getLeftEntry());

  while (!WorkList.empty()) {
    BasicBlock *CurrLeftBb = WorkList.pop_back_val();
    BasicBlock *MatchingRightBb = RP->getMatchingRightBb(CurrLeftBb);

    // add to map
    BbMap.insert(
        std::pair<BasicBlock *, BasicBlock *>(CurrLeftBb, MatchingRightBb));

    // this block is visited
    VisitedBBs.push_back(CurrLeftBb);
    // add all in-region successors to worklist
    for (unsigned I = 0; I < CurrLeftBb->getTerminator()->getNumSuccessors();
         I++) {
      BasicBlock *Succ = CurrLeftBb->getTerminator()->getSuccessor(I);
      if (Succ != RP->getLeftExit() &&
          std::find(VisitedBBs.begin(), VisitedBBs.end(), Succ) ==
              VisitedBBs.end())
        WorkList.push_back(Succ);
    }
  }

  // add matching exit blocks
  BbMap.insert(std::pair<BasicBlock *, BasicBlock *>(RP->getLeftExit(),
                                                     RP->getRightExit()));

  return BbMap;
}

// if the region's exit has predecessors from outside the region
// region needs a simplification
bool RegionAnalyzer::requireRegionSimplification(Region *R) {
  BasicBlock *Exit = R->getExit();
  for (auto PredIt = pred_begin(Exit); PredIt != pred_end(Exit); ++PredIt) {
    BasicBlock *Pred = *PredIt;
    if (!R->contains(Pred))
      return true;
  }
  return false;
}

// this function checks if the melding requires a region replication
// this occurs when we have to do a BB-Region meld.
// The matching basic block is inside the region but it does not post-dominate
// the entry.
bool RegionAnalyzer::requireRegionReplication() {
  if (!HasBbMatch)
    return false;
  BasicBlock *L = BestBbMatch.first;
  BasicBlock *R = BestBbMatch.second;
  BasicBlock *LeftEntry = DivergentBB->getTerminator()->getSuccessor(0);
  BasicBlock *RightEntry = DivergentBB->getTerminator()->getSuccessor(1);
  PostDominatorTree &PDT = getCFGInfo().getPostDomTree();

  return !PDT.dominates(L, LeftEntry) ||
         !PDT.dominates(R, RightEntry);
}

unsigned RegionAnalyzer::getMostProfitableRegionMatchIndex() {
  if (HasBbMatch) return 0;
  
  double MaxProfit = 0.0;
  unsigned MaxIndex = 0;
  for (unsigned I = 0; I < BestRegionMatch.size(); I++) {
    if (BestRegionMatch[I]->getSimilarityScore() > MaxProfit) {
      MaxIndex = I;
      MaxProfit = BestRegionMatch[I]->getSimilarityScore();
    }
  }
  return MaxIndex;
}

bool RegionAnalyzer::hasAnyProfitableMatch() {
  if (HasBbMatch) {
    // FIXME : if this requires region replication we need to be careful about
    // store instructions. avoid doing RR is stroe instructions are present in
    // other blocks for now
    // if (requireRegionReplication()) {
    //   Region *ReplicateR = nullptr;
    //   BasicBlock* BBInsideRegion = nullptr;
    //   BasicBlock *L = BestBbMatch.first;
    //   BasicBlock *R = BestBbMatch.second;
    //   BasicBlock *LeftEntry = DivergentBB->getTerminator()->getSuccessor(0);
    //   BasicBlock *RightEntry = DivergentBB->getTerminator()->getSuccessor(1);

    //   if (getPDT()->dominates(L, LeftEntry)) {
    //     ReplicateR = getRI()->getRegionFor(R);
    //     BBInsideRegion = R;
    //   }
    //   else {
    //     ReplicateR = getRI()->getRegionFor(L);
    //     BBInsideRegion = L;

    //   }

    //   // check if replicated region contains store instructions outside the
    //   melded blocks
    //   // if it does don't meld for now
    //   for(auto *BB : ReplicateR->blocks()){
    //     if (BB == BBInsideRegion) continue;
    //     for (auto &I : *BB) {
    //       if(isa<StoreInst>(&I)) {
    //         DEBUG << "Replicated region contains store instructions outside
    //         melded block, no melding performed!\n"; return false;
    //       }
    //     }
    //   }

    // }

    // if merging 2 basic blocks, avoid merging single branch blocks (size > 1)
    return BestBbMatchSimilarityScore >= SimilarityThreshold &&
           (BestBbMatch.first->size() > 1 && BestBbMatch.second->size() > 1);
  }

  if (BestRegionMatch.size() > 0) {
    for(auto &RegionPair : BestRegionMatch) {
      if (RegionPair->getSimilarityScore() >= SimilarityThreshold)
        return true;
    }
  }

  return false;
}

bool RegionAnalyzer::isRegionMatchProfitable(unsigned Index) {
  assert(Index < regionMatchSize() && "Region match index out of bounds!");
  if (HasBbMatch) {
    return hasAnyProfitableMatch();
  }
  return BestRegionMatch[Index]->getSimilarityScore() >= SimilarityThreshold;
}

void RegionAnalyzer::printAnalysis(llvm::raw_ostream &Os) {
  if (HasBbMatch) {
    Os << "Similarity Score : " << BestBbMatchSimilarityScore << "\n";
    Os << "Merge at BB level : \n"
       << "   merging BB ";
    BestBbMatch.first->printAsOperand(Os, false);
    Os << " with  ";
    BestBbMatch.second->printAsOperand(errs(), false);
    Os << "   requires " << (requireRegionReplication() ? "" : " no ")
       << "region replication\n";

  } else if (BestRegionMatch.size() > 0) {
    Os << "Merge at REGION level : \n";
    for (unsigned I = 0; I < BestRegionMatch.size(); I++) {
      Os << "Index : " << I << "\n";
      Os << "Similarity Score : " << BestRegionMatch[I]->getSimilarityScore()
         << "\n"
         << "   merging region entry ";
      BestRegionMatch[I]->getLeftEntry()->printAsOperand(errs(), false);
      Os << " with "
         << " region entry ";
      BestRegionMatch[I]->getRightEntry()->printAsOperand(errs(), false);
      Os << "\n"
         << "   merging region exit ";
      BestRegionMatch[I]->getLeftExit()->printAsOperand(errs(), false);
      Os << " with "
         << " region exit ";
      BestRegionMatch[I]->getRightExit()->printAsOperand(errs(), false);
      Os << "\n";
    }
  }
}