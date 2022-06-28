#ifndef LLVM_LIB_TRANSFORMS_REGION_ANALYZER_H
#define LLVM_LIB_TRANSFORMS_REGION_ANALYZER_H

#include "CFMelderUtils.h"
#include "SeqAlignmentUtils.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

namespace llvm {

/// convienience object to pass control-flow info around
class ControlFlowGraphInfo {
private:
  Function &F;
  DominatorTree &DT;
  PostDominatorTree &PDT;
  std::shared_ptr<RegionInfo> RI;
  std::shared_ptr<LoopInfo> LI;

public:
  ControlFlowGraphInfo(Function &F, DominatorTree &DT, PostDominatorTree &PDT);
  /// recompute analyses
  void recompute();
  /// 
  Function &getFunction() { return F; }
  /// get dom tree
  DominatorTree &getDomTree() { return DT; }
  /// get post-dom tree
  PostDominatorTree &getPostDomTree() { return PDT; }
  /// get region info
  shared_ptr<RegionInfo> getRegionInfo() { return RI; }
  /// get loop info
  shared_ptr<LoopInfo> getLoopInfo() { return LI; }

};

class RegionComparator {
private:
  const Region *R1;
  const Region *R2;
  DenseMap<BasicBlock *, BasicBlock *> Mapping;

public:
  RegionComparator(const Region *R1, const Region *R2) : R1(R1), R2(R2) {}
  // compare two regions, return true if they have equal control flow
  bool compare();
  // get the region mapping if exists
  void getMapping(DenseMap<BasicBlock *, BasicBlock *> &Result);
};

class MergeableRegionPair {
private:
  // Region *R1, *R2; // R1 == left, R2 == right
  BasicBlock *LEntry = nullptr, *LExit = nullptr;
  BasicBlock *REntry = nullptr, *RExit = nullptr;
  double SimilarityScore{0.0};
  DenseMap<BasicBlock *, BasicBlock *> Mapping;

  // void CalculateSimilarityScore();

public:
  MergeableRegionPair(Region &R1, Region &R2, RegionComparator &Comparator);

  double getSimilarityScore() const { return SimilarityScore; }

  bool operator<(const MergeableRegionPair &RegionPair) {
    return this->SimilarityScore < RegionPair.getSimilarityScore();
  }

  bool operator>(const MergeableRegionPair &RegionPair) {
    return this->SimilarityScore > RegionPair.getSimilarityScore();
  }

  bool operator==(const MergeableRegionPair &RegionPair) {
    return this->SimilarityScore == RegionPair.getSimilarityScore();
  }

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &OS,
                                       MergeableRegionPair &MRP) {
    OS << "[ Left entry : " << MRP.getLeftEntry()->getName()
       << ", Left exit : " << MRP.getLeftExit()->getName()
       << ", Right entry : " << MRP.getRightEntry()->getName()
       << ", Right exit : " << MRP.getRightExit()->getName() << " ]";
    return OS;
  }

  // returns true if R1 dominates other.R1 and R2 dominates other.R2
  // i.e. this and other can be independantly mergeable
  bool dominates(std::shared_ptr<MergeableRegionPair> &Other,
                 DominatorTree &DT);

  BasicBlock *getLeftEntry() { return LEntry; }
  BasicBlock *getLeftExit() { return LExit; }

  BasicBlock *getRightEntry() { return REntry; }
  BasicBlock *getRightExit() { return RExit; }

  DenseMap<BasicBlock *, BasicBlock *>::iterator begin() {
    return Mapping.begin();
  }
  DenseMap<BasicBlock *, BasicBlock *>::iterator end() { return Mapping.end(); }

  BasicBlock *getMatchingRightBb(BasicBlock *BB);
};

struct RegionMeldingProfitabilityModel : public ScoringFunction<Region *> {
public:
  int operator()(Region *R1, Region *R2) override {
    RegionComparator RC(R1, R2);
    if (!RC.compare())
      return 0;
    DenseMap<BasicBlock *, BasicBlock *> Mapping;
    RC.getMapping(Mapping);
    double Score = Utils::computeRegionSimilarity(Mapping, R1->getExit());

    return (int)(Score * 100);
  }
  int gap(int K) override { return 0; }
};

// class to analyze merebale control-flow
class RegionAnalyzer {
private:
  BasicBlock *DivergentBB;
  Value *DivergentCondition;
  ControlFlowGraphInfo &CFGInfo;

  // left path and right path regions
  SmallVector<Region *, 0> LeftRegions;
  SmallVector<Region *, 0> RightRegions;

  // best matching of regions
  SmallVector<std::shared_ptr<MergeableRegionPair>, 0> BestRegionMatch;
  // best mathcing basic blocks, if regions do not exist
  std::pair<BasicBlock *, BasicBlock *> BestBbMatch{nullptr, nullptr};
  bool HasBbMatch = false;
  double BestBbMatchSimilarityScore = 0.0;

  void findMergeableRegions(BasicBlock &BB);
  BasicBlock *findMostSimilarBb(BasicBlock *BB,
                                SmallVectorImpl<BasicBlock *> &Candidates);
  /// Find mergeable blocks in 'Regions' that are not contained inside local loops
  void findMergeableBBsInRegions(BasicBlock *From, SmallVectorImpl<Region*> &Regions,
                              SmallVectorImpl<BasicBlock *> &MergeableBBs);

  void computeGreedyRegionMatch();
  void computeSARegionMatch();

public:
  RegionAnalyzer(BasicBlock *BB, ControlFlowGraphInfo &CFGInfo);

  void computeRegionMatch();

  // region analyzer's result
  DenseMap<BasicBlock *, BasicBlock *> getRegionMatch(unsigned Index);
  std::pair<BasicBlock *, BasicBlock *>
  getRegionMatchEntryBlocks(unsigned Index);
  std::pair<BasicBlock *, BasicBlock *>
  getRegionMatchExitBlocks(unsigned Index);
  // bool isInsideRegionMatch(BasicBlock *BB, unsigned Index);
  bool hasAnyProfitableMatch();
  bool isRegionMatchProfitable(unsigned Index);
  unsigned regionMatchSize() const;
  unsigned getMostProfitableRegionMatchIndex();

  // region replication, only applicable for BB-region melding
  bool requireRegionReplication();

  bool requireRegionSimplification(Region *R);

  /// infomation about divergent region
  Value *getDivergentCondition() const { return DivergentCondition; }

  BasicBlock *getDivergentBlock() const { return DivergentBB; }

  Function *getParentFunction() const {
    return getDivergentBlock()->getParent();
  }

  /// control-flow analysis
  ControlFlowGraphInfo &getCFGInfo() { return CFGInfo; }

  void printAnalysis(llvm::raw_ostream &OS);
};

} // namespace llvm

#endif