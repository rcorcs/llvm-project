#include "RegionMelder.h"
#include "CFMelderUtils.h"
#include "RegionReplicator.h"
#include "SeqAlignmentUtils.h"
#include "llvm/ADT/CachedHashString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <chrono>
#include <string>

using namespace llvm;
#define ENABLE_TIMING 1

#define DEBUG_TYPE "cfmelder"

static cl::opt<bool> DisableMelding(
    "disable-melding", cl::init(false), cl::Hidden,
    cl::desc("Disables melding step, runs region simplification if required"));

static cl::opt<bool> EnableFullPredication(
    "enable-full-predication", cl::init(false), cl::Hidden,
    cl::desc("Enable full predication for merged blocks"));

static cl::opt<bool> UseLatencyCostModel(
    "use-latency-for-alignment", cl::init(false), cl::Hidden,
    cl::desc("Use latency cost model for instruction alignment"));

static cl::opt<bool>
    DumpSeqAlignStats("dump-seq-align-stats", cl::init(false), cl::Hidden,
                      cl::desc("Dump information on sequence alignment"));

STATISTIC(NumMeldings, "Number of profitable meldings performed");
STATISTIC(BBToBBMeldings,
          "Number of profitable basic block to basic block meldings");
STATISTIC(BBToRegionMeldings,
          "Number of profitable basic block to region meldings");
STATISTIC(RegionToRegionMeldings,
          "Number of profitable region to region meldings");
STATISTIC(InstrAlignTime,
          "Time spent in instruction alignment in microseconds");

AlignedSeq<Value *>
RegionMelder::getAlignmentOfBlocks(BasicBlock *LeftBb, BasicBlock *RightBb,
                                   ScoringFunction<Value *> &ScoringFunc) {
  // do sequence aligment
  SmallVector<Value *, 32> LSeq;
  SmallVector<Value *, 32> RSeq;
  linearizeBb(LeftBb, LSeq);
  linearizeBb(RightBb, RSeq);

  auto SMSA =
      SmithWaterman<Value *, SmallVectorImpl<Value *>, nullptr>(ScoringFunc);

  auto Result = SMSA.compute(LSeq, RSeq);

  return Result;
}

void RegionMelder::computeRegionSeqAlignment(
    DenseMap<BasicBlock *, BasicBlock *> BbMap) {

  shared_ptr<ScoringFunction<Value *>> ScoringFuncSize =
      make_shared<CodeSizeCostModel>(MA.getCFGInfo().getTTI());
  shared_ptr<ScoringFunction<Value *>> ScoringFuncLat =
      make_shared<GPULatencyCostModel>();

  auto ScoringFunc = UseLatencyCostModel ? ScoringFuncLat : ScoringFuncSize;

  for (auto It = BbMap.begin(); It != BbMap.end(); It++) {
    BasicBlock *LBB = It->first;
    BasicBlock *RBB = It->second;

    RegionInstrAlignement.concat(getAlignmentOfBlocks(LBB, RBB, *ScoringFunc));
  }
  if (DumpSeqAlignStats) {

    int SavedCycles = 0;
    for (auto Entry : RegionInstrAlignement) {
      Value *Left = Entry.getLeft();
      Value *Right = Entry.getRight();
      if (Left && Right) {
        if (isa<BasicBlock>(Left))
          continue;
        SavedCycles += (*ScoringFunc)(Left, Right);
      } else {
        SavedCycles -= (*ScoringFunc).gap(0);
      }
    }
    INFO << "Number of cycles saved by alignment : " << SavedCycles << "\n";
  }
}

bool requireUnpredication(BasicBlock *Current, BasicBlock *Corresponding) {
  // if current contains instructions with side effetcs we need to unpredicate
  // these include stores, calls and divisions
  for (auto &I : *Current) {
    if (I.mayHaveSideEffects()) {
      return true;
    }
  }
  return Corresponding->size() > 1;
}

void RegionMelder::cloneInstructions() {
  INFO << "Cloning instructions\n";
  // generate the control flow for merged region
  IRBuilder<> Builder(&MA.getParentFunction()->getEntryBlock());
  for (auto &Entry : RegionInstrAlignement) {

    Value *LEntry = Entry.get(0);
    Value *REntry = Entry.get(1);
    if (Entry.match()) {
      if (isa<BasicBlock>(LEntry)) {
        assert(isa<BasicBlock>(REntry) &&
               "Both matching entries must be basic blocks");
        BasicBlock *NewBb =
            BasicBlock::Create(MA.getParentFunction()->getContext(),
                               "merged.bb", MA.getParentFunction());
        // update value map
        MergedValuesToLeftValues[NewBb] = LEntry;
        MergedValuesToRightValues[NewBb] = REntry;

        BasicBlock *LeftBb = dyn_cast<BasicBlock>(LEntry);
        BasicBlock *RightBb = dyn_cast<BasicBlock>(REntry);
        // update label map
        LeftBbToMergedBb[LeftBb] = NewBb;
        RightBbToMergedBb[RightBb] = NewBb;

        for (auto &I : *dyn_cast<BasicBlock>(LEntry)) {
          if (isa<PHINode>(&I)) {
            Instruction *NewI = cloneInstruction(&I, Builder);
            MergedValuesToLeftValues[NewI] = &I;
            OrigToMergedValues[&I] = NewI;
            MergedInstructions.push_back(NewI);
          }
        }

        for (auto &I : *dyn_cast<BasicBlock>(REntry)) {
          if (isa<PHINode>(&I)) {
            Instruction *NewI = cloneInstruction(&I, Builder);
            MergedValuesToRightValues[NewI] = &I;
            OrigToMergedValues[&I] = NewI;
            MergedInstructions.push_back(NewI);
          }
        }
        // add to merged blocks
        MergedBBs.push_back(NewBb);

      } else {
        assert(isa<Instruction>(LEntry) && isa<Instruction>(REntry) &&
               "Both entries must be instructions");
        // skip phi nodes
        if (!isa<PHINode>(LEntry)) {
          Instruction *LeftI = dyn_cast<Instruction>(LEntry);
          Instruction *RightI = dyn_cast<Instruction>(REntry);

          Instruction *NewI = cloneInstruction(LeftI, Builder);

          // update the maps
          MergedValuesToLeftValues[NewI] = LeftI;
          MergedValuesToRightValues[NewI] = RightI;
          OrigToMergedValues[LeftI] = NewI;
          OrigToMergedValues[RightI] = NewI;

          MergedInstructions.push_back(NewI);
        }
      }
    } else {
      if (LEntry != nullptr && !isa<PHINode>(LEntry)) {
        Instruction *LeftI = dyn_cast<Instruction>(LEntry);
        Instruction *NewI = cloneInstruction(LeftI, Builder);
        // update map
        MergedValuesToLeftValues[NewI] = LeftI;
        OrigToMergedValues[LeftI] = NewI;
        MergedInstructions.push_back(NewI);

        // update splitRanges
        // unpredication is not done if one the blocks is just a branch, occurs
        // in region replication
        auto ClonedRightParentIt =
            MergedValuesToRightValues.find(NewI->getParent());
        assert(ClonedRightParentIt != MergedValuesToRightValues.end() &&
               "Cloned left BB not found for right value!");
        // if (cast<BasicBlock>(ClonedRightParentIt->second)->size() > 1)
        if (requireUnpredication(LeftI->getParent(),
                                 cast<BasicBlock>(ClonedRightParentIt->second)))
          updateSplitRangeMap(true, NewI);
      }

      if (REntry != nullptr && !isa<PHINode>(REntry)) {
        Instruction *RightI = dyn_cast<Instruction>(REntry);
        Instruction *NewI = cloneInstruction(RightI, Builder);
        // update map
        MergedValuesToRightValues[NewI] = RightI;
        OrigToMergedValues[RightI] = NewI;
        MergedInstructions.push_back(NewI);

        // update splitRanges
        // unpredication is not done if one the blocks is just a branch, occurs
        // in region replication
        auto ClonedLeftParentIt =
            MergedValuesToLeftValues.find(NewI->getParent());
        assert(ClonedLeftParentIt != MergedValuesToLeftValues.end() &&
               "Cloned left BB not found for right value!");
        // if (cast<BasicBlock>(ClonedLeftParentIt->second)->size() > 1)
        if (requireUnpredication(RightI->getParent(),
                                 cast<BasicBlock>(ClonedLeftParentIt->second)))
          updateSplitRangeMap(false, NewI);
      }
    }
  }
}

void RegionMelder::fixPhiNode(PHINode *Orig) {
  // orig->print(errs()); errs() << "\n";
  // get the merged phi node
  assert(OrigToMergedValues.find(Orig) != OrigToMergedValues.end() &&
         "phi node is not found in merged control flow!");

  PHINode *MergedPhi = dyn_cast<PHINode>(OrigToMergedValues[Orig]);

  for (unsigned I = 0; I < Orig->getNumIncomingValues(); I++) {
    BasicBlock *OrigIncomingBb = Orig->getIncomingBlock(I);
    Value *OrigIncomingV = Orig->getIncomingValue(I);
    // set the matching incoming block and value in merged PHI node
    BasicBlock *MergedIncomingBb = OrigIncomingBb;
    Value *MergedIncomingV = OrigIncomingV;
    if (LeftBbToMergedBb.find(OrigIncomingBb) != LeftBbToMergedBb.end())
      MergedIncomingBb = LeftBbToMergedBb[OrigIncomingBb];
    else if (RightBbToMergedBb.find(OrigIncomingBb) != RightBbToMergedBb.end())
      MergedIncomingBb = RightBbToMergedBb[OrigIncomingBb];

    assert(MergedIncomingBb != nullptr &&
           "matching incoming block not found for phi node!");

    MergedPhi->setIncomingBlock(I, MergedIncomingBb);

    // check if origIncoming value is merged
    if (OrigToMergedValues.find(OrigIncomingV) != OrigToMergedValues.end())
      MergedIncomingV = OrigToMergedValues[OrigIncomingV];

    // set the incoming value
    MergedPhi->setIncomingValue(I, MergedIncomingV);
  }
}

void RegionMelder::fixOperends() {
  INFO << "Fixing operends\n";
  // set the correct operends in merged instructions
  for (auto &Entry : RegionInstrAlignement) {
    Value *L = Entry.get(0);
    Value *R = Entry.get(1);

    Instruction *MergedI;
    if (Entry.match()) {
      // L->print(errs()); errs() << "\n";
      // R->print(errs()); errs() << "\n";
      // ignore basic blocks, branch instructions and phi nodes
      if (isa<BasicBlock>(L))
        continue;

      // handle phi nodes seperately
      if (isa<PHINode>(L)) {
        fixPhiNode(dyn_cast<PHINode>(L));
        fixPhiNode(dyn_cast<PHINode>(R));
        continue;
      }

      Instruction *LeftI = dyn_cast<Instruction>(L);
      Instruction *RightI = dyn_cast<Instruction>(R);
      assert(OrigToMergedValues[L] == OrigToMergedValues[R] &&
             "matching instructions must have common merged instruction");
      // set the operends of merged instruction
      MergedI = dyn_cast<Instruction>(OrigToMergedValues[L]);

      if (isa<BranchInst>(L))
        setOperendsForBr(dyn_cast<BranchInst>(L), dyn_cast<BranchInst>(R),
                         dyn_cast<BranchInst>(MergedI));
      else
        setOperends(LeftI, RightI, MergedI);

    } else {
      // ignore branch instructions
      // TODO : returns
      if (L != nullptr) {
        assert(!isa<BranchInst>(L) && "unmatched branch found!");
        if (isa<PHINode>(L)) {
          fixPhiNode(dyn_cast<PHINode>(L));
        } else if (isa<StoreInst>(L) && EnableFullPredication) {
          StoreInst *MergedSi = dyn_cast<StoreInst>(OrigToMergedValues[L]);
          setOprendsForNonMatchingStore(MergedSi, true);
        } else {
          assert(!isa<BasicBlock>(L) &&
                 "non matching value can not be a basicblock");
          Instruction *LeftI = dyn_cast<Instruction>(L);
          MergedI = dyn_cast<Instruction>(OrigToMergedValues[L]);

          setOperends(LeftI, nullptr, MergedI);
        }
      }

      if (R != nullptr) {
        assert(!isa<BranchInst>(R) && "unmatched branch found!");
        if (isa<PHINode>(R)) {
          fixPhiNode(dyn_cast<PHINode>(R));
        } else if (isa<StoreInst>(R) && EnableFullPredication) {
          StoreInst *MergedSi = dyn_cast<StoreInst>(OrigToMergedValues[R]);
          setOprendsForNonMatchingStore(MergedSi, false);
        } else {
          assert(!isa<BasicBlock>(R) &&
                 "non matching value can not be a basicblock");
          Instruction *RightI = dyn_cast<Instruction>(R);
          MergedI = dyn_cast<Instruction>(OrigToMergedValues[R]);

          setOperends(nullptr, RightI, MergedI);
        }
      }
    }
  }
}

void RegionMelder::setOperendsForBr(BranchInst *LeftBr, BranchInst *RightBr,
                                    BranchInst *MergedBr) {

  // for branches inside the merged regions : pick the correct condition using a
  // select branch labels are set in RAUW phase
  if (ExitBlockL && LeftBr->getParent() != ExitBlockL) {
    assert(LeftBr->getNumSuccessors() == RightBr->getNumSuccessors() &&
           "branches inside the merged region must have same number of "
           "successors!");
    if (LeftBr->isConditional()) {
      Value *LeftCond = LeftBr->getCondition();
      Value *RightCond = RightBr->getCondition();
      Value *MergedCond = nullptr;
      if (OrigToMergedValues.find(LeftCond) != OrigToMergedValues.end())
        LeftCond = OrigToMergedValues[LeftCond];
      if (OrigToMergedValues.find(RightCond) != OrigToMergedValues.end())
        RightCond = OrigToMergedValues[RightCond];

      MergedCond = LeftCond;
      // create a select if left and right conditions are not same
      if (LeftCond != RightCond) {
        IRBuilder<> Builder(MergedBr);
        MergedCond = Builder.CreateSelect(MA.getDivergentCondition(), LeftCond,
                                          RightCond);
      }
      MergedBr->setCondition(MergedCond);
    }

    return;
  }

  // for branches in exit blocks : create two new basic blocks and copy left and
  // right branches trasfer control to new blocks based on merge path

  // create two new basic blocks and copy the branches from left and right sides
  BasicBlock *NewBbLeftBr =
      BasicBlock::Create(MA.getParentFunction()->getContext(),
                         "merged.branch.split", MA.getParentFunction());
  BasicBlock *NewBbRightBr =
      BasicBlock::Create(MA.getParentFunction()->getContext(),
                         "merged.branch.split", MA.getParentFunction());

  MergedBBs.push_back(NewBbLeftBr);
  MergedBBs.push_back(NewBbRightBr);

  // clone the original branches and add them to new BBs
  IRBuilder<> Builder(NewBbLeftBr);
  Instruction *NewLeftBr = LeftBr->clone();
  Instruction *NewRightBr = RightBr->clone();
  Builder.Insert(NewLeftBr);
  Builder.SetInsertPoint(NewBbRightBr);
  Builder.Insert(NewRightBr);

  // create a new branch in the merged block to set the targets based on
  // mergepath remove exiting mergeBr
  Builder.SetInsertPoint(MergedBr->getParent());
  BranchInst *NewBi = Builder.CreateCondBr(MA.getDivergentCondition(),
                                           NewBbLeftBr, NewBbRightBr);
  MergedBr->eraseFromParent();

  // update the value maps
  MergedValuesToLeftValues[NewBi] = LeftBr;
  MergedValuesToLeftValues[NewBi] = RightBr;
  OrigToMergedValues[LeftBr] = NewBi;
  OrigToMergedValues[RightBr] = NewBi;

  // fix the phi uses in all successors
  for (BasicBlock *SuccBb : dyn_cast<BranchInst>(NewLeftBr)->successors()) {
    SuccBb->replacePhiUsesWith(LeftBr->getParent(), NewBbLeftBr);
  }

  for (BasicBlock *SuccBb : dyn_cast<BranchInst>(NewRightBr)->successors()) {
    SuccBb->replacePhiUsesWith(RightBr->getParent(), NewBbRightBr);
  }
}

void RegionMelder::setOprendsForNonMatchingStore(StoreInst *SI, bool IsLeft) {
  // non matching store instructions causes invalid memory write in
  // L or R path. To avoid this we have to add a redundant load that reads the
  // curent value of the address. and depending on the path we pick the correct
  // to value to write. i.e. current value in the non-matching path or intended
  // value in the matching path
  Value *Addr = SI->getPointerOperand();
  Value *Val = SI->getValueOperand();

  // find the merged operends
  if (OrigToMergedValues.find(Addr) != OrigToMergedValues.end())
    Addr = OrigToMergedValues[Addr];
  if (OrigToMergedValues.find(Val) != OrigToMergedValues.end())
    Val = OrigToMergedValues[Val];

  // create a load for the addr (gets current value)
  IRBuilder<> Builder(SI);
  Builder.SetInsertPoint(SI);
  LoadInst *RedunLoad = Builder.CreateLoad(Addr, "redun.load");
  // create a switch to pick the right value
  Value *ValueL = nullptr, *ValueR = nullptr;
  if (IsLeft) {
    ValueL = Val;
    ValueR = RedunLoad;
  } else {
    ValueL = RedunLoad;
    ValueR = Val;
  }
  Value *ValToStore =
      Builder.CreateSelect(MA.getDivergentCondition(), ValueL, ValueR);

  // set the value
  SI->setOperand(0, ValToStore);
  // set the addr
  SI->setOperand(1, Addr);
}

void RegionMelder::setOperends(Instruction *LeftI, Instruction *RightI,
                               Instruction *MergedI) {
  for (unsigned I = 0; I < MergedI->getNumOperands(); I++) {
    Value *LeftOp = nullptr, *RightOp = nullptr;

    if (LeftI)
      LeftOp = LeftI->getOperand(I);
    if (RightI)
      RightOp = RightI->getOperand(I);

    if (LeftOp && OrigToMergedValues.find(LeftOp) != OrigToMergedValues.end())
      LeftOp = OrigToMergedValues[LeftOp];
    if (RightOp && OrigToMergedValues.find(RightOp) != OrigToMergedValues.end())
      RightOp = OrigToMergedValues[RightOp];

    // if the operends are different add a select to pick the correct one
    Value *NewOp = LeftOp ? LeftOp : RightOp;
    if (LeftOp && RightOp && LeftOp != RightOp) {
      SelectInst *Select =
          SelectInst::Create(MA.getDivergentCondition(), LeftOp, RightOp,
                             "merged.select", MergedI);
      NewOp = dyn_cast<Value>(Select);
    }

    // set the new operenf
    MergedI->setOperand(I, NewOp);
  }
}

void RegionMelder::runPostMergeCleanup() {

  // replace all uses with merged vals
  // ignore basicblocks
  for (auto &Entry : RegionInstrAlignement) {

    Value *L = Entry.get(0);
    Value *R = Entry.get(1);

    Value *MergedValLeft = nullptr;
    Value *MergedValRight = nullptr;
    if (Entry.match()) {
      if (isa<BasicBlock>(L)) {
        MergedValLeft = LeftBbToMergedBb[dyn_cast<BasicBlock>(L)];
        MergedValRight = MergedValLeft;

      } else if (isa<PHINode>(L)) {
        MergedValLeft = OrigToMergedValues[L];
        MergedValRight = OrigToMergedValues[R];
      } else {
        MergedValLeft = OrigToMergedValues[L];
        MergedValRight = MergedValLeft;
      }

      L->replaceAllUsesWith(MergedValLeft);
      R->replaceAllUsesWith(MergedValRight);

    } else {
      if (L != nullptr) {
        if (isa<BasicBlock>(L))
          MergedValLeft = LeftBbToMergedBb[dyn_cast<BasicBlock>(L)];
        else
          MergedValLeft = OrigToMergedValues[L];

        L->replaceAllUsesWith(MergedValLeft);
      }
      if (R != nullptr) {
        if (isa<BasicBlock>(R))
          MergedValRight = LeftBbToMergedBb[dyn_cast<BasicBlock>(R)];
        else
          MergedValRight = OrigToMergedValues[R];

        R->replaceAllUsesWith(MergedValRight);
      }
    }
  }
  // fix outside phi nodes that are invalid after merge
  // FixOutsidePHINodes(outsidePhisBeforeRAUW);

  // erase orig instructions in merged regions
  for (auto &Entry : RegionInstrAlignement) {

    Value *L = Entry.get(0);
    Value *R = Entry.get(1);

    if (Entry.match()) {
      if (isa<BasicBlock>(L)) {
        if (L != R) {
          dyn_cast<BasicBlock>(L)->eraseFromParent();
          dyn_cast<BasicBlock>(R)->eraseFromParent();
        } else {
          dyn_cast<BasicBlock>(L)->eraseFromParent();
        }
      }
    } else {
      if (L != nullptr) {
        if (isa<BasicBlock>(L)) {
          dyn_cast<BasicBlock>(L)->eraseFromParent();
        }
      }

      if (R != nullptr) {
        if (isa<BasicBlock>(R)) {
          dyn_cast<BasicBlock>(R)->eraseFromParent();
        }
      }
    }
  }

  // merging can result in additional predessors for merged entry blocks
  // scan all phi nodes and add missing incoming blocks, value will be undef
  // because these transitions will not happen during execution
  for (auto &BB : *MA.getParentFunction()) {
    for (PHINode &PN : BB.phis()) {
      for (auto It = pred_begin(&BB); It != pred_end(&BB); ++It) {
        if (PN.getBasicBlockIndex(*It) < 0) {
          // PN.print(errs());  errs() << "\n";
          // (*it)->print(errs());
          PN.addIncoming(UndefValue::get(PN.getType()), *It);
        }
      }
    }
  }
}

Region *RegionMelder::getRegionToReplicate(BasicBlock *MatchedBlock,
                                           BasicBlock *PathEntry) {
  auto RI = MA.getCFGInfo().getRegionInfo();
  PostDominatorTree &PDT = MA.getCFGInfo().getPostDomTree();
  BasicBlock *Curr = MatchedBlock;
  Region *R = nullptr;
  do {
    Region *Candidate = RI->getRegionFor(Curr);
    BasicBlock *Entry = Candidate->getEntry();
    if (PDT.dominates(Entry, PathEntry)) {
      R = Candidate;
    } else {
      // BasicBlock *OldCurr = Curr;
      for (auto *Pred : make_range(pred_begin(Entry), pred_end(Entry))) {
        if (!Candidate->contains(Pred)) {
          Curr = Pred;
          break;
        }
      }
      // if (OldCurr == Curr) {
      //   errs() << "DID NOT CHANGE CURRENT! INFINITE LOOP!\n";
      // }
    }

  } while (!R);
  assert(R != nullptr && "can not find region to replicate!");
  return R;
}

void RegionMelder::merge(unsigned Index) {

  static int Count = 0;
  Utils::writeCFGToDotFile(*MA.getParentFunction(), std::to_string(Count++) + ".cfmelder.");

  bool RegionAlreadySimplified = false;

  if (MA.requireRegionReplication()) {
    INFO << "Replicating regions in BB-region match\n";
    auto Mapping = MA.getRegionMatch(Index);
    assert(Mapping.size() == 1 &&
           "more than one pair of basic blocks to match in BB-region match");

    // determine on which side region needs to be replicated
    BasicBlock *LeftPathEntry =
        MA.getDivergentBlock()->getTerminator()->getSuccessor(0);
    BasicBlock *RightPathEntry =
        MA.getDivergentBlock()->getTerminator()->getSuccessor(1);

    BasicBlock *ExpandedBlock, *MatchedBlock = nullptr;
    Region *RToReplicate = nullptr;
    BasicBlock *Left = Mapping.begin()->first, *Right = Mapping.begin()->second;
    bool ExpandingLeft = false;
    if (MA.getCFGInfo().getPostDomTree().dominates(Left, LeftPathEntry)) {
      DEBUG << "Replicating right region\n";
      ExpandedBlock = Left;
      MatchedBlock = Right;
      RToReplicate = getRegionToReplicate(MatchedBlock, RightPathEntry);
      ExpandingLeft = true;
    } else {
      DEBUG << "Replicating left region\n";
      ExpandedBlock = Right;
      MatchedBlock = Left;
      RToReplicate = getRegionToReplicate(MatchedBlock, LeftPathEntry);
    }

    // simplify the replicated region
    BasicBlock *ExitToReplicate = RToReplicate->getExit();
    BasicBlock *EntryToReplicate = RToReplicate->getEntry();
    if (MA.requireRegionSimplification(RToReplicate)) {
      INFO << "Replicated region is not a simple region, running region "
              "simplification\n";
      // BasicBlock *Entry = RToReplicate->getEntry();
      ExitToReplicate =
          simplifyRegion(RToReplicate->getExit(), RToReplicate->getEntry());

      // recompute control-flow analyses , FIXME : this might be too expensive
      MA.getCFGInfo().recompute();
      RToReplicate = Utils::getRegionWithEntryExit(
          *MA.getCFGInfo().getRegionInfo(), EntryToReplicate, ExitToReplicate);
      assert(RToReplicate && "Can not find region with given entry and exit");

      INFO << "region after region simplification : ";
      errs() << "[";
      RToReplicate->getEntry()->printAsOperand(errs(), false);
      errs() << " : ";
      RToReplicate->getExit()->printAsOperand(errs(), false);
      errs() << "]\n";

      RegionAlreadySimplified = true;
    }

    // errs() << "replicate the region\n";
    // replicate the region
    RegionReplicator RR(MA, ExpandingLeft, EnableFullPredication);
    Region *ReplicatedR =
        RR.replicate(ExpandedBlock, MatchedBlock, RToReplicate);

    // errs() << "prepare for melding\n";
    // prepare for melding
    if (ExpandingLeft) {
      EntryBlockL = ReplicatedR->getEntry();
      ExitBlockL = ReplicatedR->getExit();
      EntryBlockR = EntryToReplicate;
      ExitBlockR = ExitToReplicate;
    } else {
      EntryBlockR = ReplicatedR->getEntry();
      ExitBlockR = ReplicatedR->getExit();
      EntryBlockL = EntryToReplicate;
      ExitBlockL = ExitToReplicate;
    }

    // errs() << "here\n";
    RR.getBasicBlockMapping(CurrMapping, ExpandingLeft);

    BBToRegionMeldings++;

  } else {

    // set entry and exits
    EntryBlockL = MA.getRegionMatchEntryBlocks(Index).first;
    EntryBlockR = MA.getRegionMatchEntryBlocks(Index).second;

    // exit blocks are set for only region-region melding, otherwise null
    auto ExitBlocks = MA.getRegionMatchExitBlocks(Index);
    if (ExitBlocks.first && ExitBlocks.second) {
      ExitBlockL = ExitBlocks.first;
      ExitBlockR = ExitBlocks.second;
      RegionToRegionMeldings++;
    } else {
      BasicBlock *LeftUniqueSucc = EntryBlockL->getUniqueSuccessor();
      BasicBlock *RightUniqueSucc = EntryBlockR->getUniqueSuccessor();
      // if diamonf control-flow
      if (LeftUniqueSucc && RightUniqueSucc &&
          LeftUniqueSucc == RightUniqueSucc) {
        BBToBBMeldings++;
      } else {
        BBToRegionMeldings++;
      }
    }

    CurrMapping = MA.getRegionMatch(Index);
  }

  INFO << "Melding entry blocks ";
  EntryBlockL->printAsOperand(errs(), false);
  errs() << " , ";
  EntryBlockR->printAsOperand(errs(), false);
  errs() << "\n";

  if (ExitBlockL) {
    INFO << "Melding exit blocks ";
    ExitBlockL->printAsOperand(errs(), false);
    errs() << " , ";
    ExitBlockR->printAsOperand(errs(), false);
    errs() << "\n";
  }
  // run pre merge passes
  runPreMergePasses(RegionAlreadySimplified);
  Utils::writeCFGToDotFile(*MA.getParentFunction(), std::to_string(Count++) + ".cfmelder.");

  if (!DisableMelding) {
    // parentFunc->print(errs());
    // compute alignment
#if ENABLE_TIMING == 1
    auto T1 = std::chrono::high_resolution_clock::now();
#endif
    computeRegionSeqAlignment(CurrMapping);
#if ENABLE_TIMING == 1
    auto T2 = std::chrono::high_resolution_clock::now();
    auto micros =
        std::chrono::duration_cast<std::chrono::microseconds>(T2 - T1).count();
    InstrAlignTime += (unsigned int)(micros);
#endif
    // for(auto& Entry : RegionInstrAlignement) {
    //   if(Entry.getLeft())
    //     Entry.getLeft()->print(errs());
    //   else
    //     errs() << "_";
    //   errs() << ":";
    //   if(Entry.getRight())
    //     Entry.getRight()->print(errs());
    //   else
    //     errs() << "_";
    //   errs() << "\n";
    // }
    // while(true);

    // Merge the regions
    cloneInstructions();
    fixOperends();
    runPostMergeCleanup();
    runPostOptimizations();

    if (!EnableFullPredication)
      runUnpredicationPass();
  }
  NumMeldings++;
  Utils::writeCFGToDotFile(*MA.getParentFunction(), std::to_string(Count++) + ".cfmelder.");
  // verify the function
  assert(!verifyFunction(*MA.getParentFunction(), &errs()) &&
         "function verification failed!");
}

void RegionMelder::linearizeBb(BasicBlock *BB,
                               SmallVectorImpl<Value *> &LinearizedVals) {
  LinearizedVals.push_back(BB);
  for (Instruction &I : *BB) {
    LinearizedVals.push_back(&I);
  }
}

Instruction *RegionMelder::cloneInstruction(Instruction *OrigI,
                                            IRBuilder<> &Builder) {
  Instruction *NewI = OrigI->clone();

  BasicBlock *InsertAt = nullptr;
  // decide whta place to insert at
  if (LeftBbToMergedBb.find(OrigI->getParent()) != LeftBbToMergedBb.end())
    InsertAt = LeftBbToMergedBb[OrigI->getParent()];
  if (InsertAt == nullptr)
    InsertAt = RightBbToMergedBb[OrigI->getParent()];

  // insertion
  Builder.SetInsertPoint(InsertAt);
  Builder.Insert(NewI);

  return NewI;
}

void RegionMelder::runUnpredicationPass() {

  for (auto &Range : SplitRanges) {
    BasicBlock *BB = Range.getStart()->getParent();
    BasicBlock *SplitBb = SplitBlock(BB, Range.getStart());
    SplitBb->setName("predication.split"); // FIX : for ambigous overloading
    BasicBlock *TailBlock = SplitBlock(SplitBb, Range.getEnd()->getNextNode());
    TailBlock->setName("predication.tail"); //  FIX : for ambigous overloading

    MergedBBs.push_back(SplitBb);

    // now only execute the splitBlock conditionally
    Instruction *OldBr = BB->getTerminator();
    BasicBlock *TrueTarget = nullptr;
    BasicBlock *FalseTarget = nullptr;
    if (Range.splitToTrue()) {
      TrueTarget = SplitBb;
      FalseTarget = TailBlock;
    } else {
      TrueTarget = TailBlock;
      FalseTarget = SplitBb;
    }
    BranchInst *NewBr = BranchInst::Create(TrueTarget, FalseTarget,
                                           MA.getDivergentCondition(), BB);
    OldBr->replaceAllUsesWith(NewBr);
    OldBr->eraseFromParent();

    // add phi node where necessary

    for (auto &I : *SplitBb) {
      SmallVector<Instruction *, 32> Users;
      PHINode *NewPhi = nullptr;
      for (auto It = I.user_begin(); It != I.user_end(); ++It) {
        Instruction *User = dyn_cast<Instruction>(*It);
        if (User->getParent() != SplitBb) {
          Users.push_back(User);
        }
      }

      if (!Users.empty()) {
        NewPhi =
            PHINode::Create(I.getType(), 2, "", TailBlock->getFirstNonPHI());
        NewPhi->addIncoming(&I, SplitBb);
        NewPhi->addIncoming(UndefValue::get(NewPhi->getType()), BB);
        for (auto User : Users) {
          User->replaceUsesOfWith(&I, NewPhi);
        }
      }
    }
  }
}

void RegionMelder::updateSplitRangeMap(bool Direction, Instruction *I) {
  InstrRange Range(I, I, Direction);
  if (!SplitRanges.empty() && SplitRanges.back().canExtendUsing(Range)) {
    InstrRange Prev = SplitRanges.pop_back_val();
    SplitRanges.push_back(Prev.extendUsing(Range));
  } else {
    SplitRanges.push_back(Range);
  }
}

BasicBlock *RegionMelder::simplifyRegion(BasicBlock *Exit, BasicBlock *Entry) {
  // only applies for region merges (not needed for bb merges)
  // if the exit block of the region (to be merged) has preds from outside that
  // region create a new exit block and add an edge from new to old exit

  // create a new exit block
  Function *ParentFunc = MA.getParentFunction();
  BasicBlock *NewExit = BasicBlock::Create(ParentFunc->getContext(), "new.exit",
                                           ParentFunc, Exit);
  // add a jump from new exit to old exit
  BranchInst::Create(Exit, NewExit);
  Region *MergedR = Utils::getRegionWithEntryExit(
      *MA.getCFGInfo().getRegionInfo(), Entry, Exit);

  // this can not be nullptr because must exit
  assert(MergedR && "Can not find region with given entry and exit");

  // move relavant phi nodes from old exit to new exit
  SmallVector<BasicBlock *, 4> IncomingBlocksToDelete;
  for (auto &Phi : Exit->phis()) {

    IncomingBlocksToDelete.clear();
    // create a new phi in new exit block
    PHINode *NewPhi =
        PHINode::Create(Phi.getType(), 1, "moved.phi", &*NewExit->begin());
    for (unsigned I = 0; I < Phi.getNumIncomingValues(); ++I) {
      Value *IncomingV = Phi.getIncomingValue(I);
      BasicBlock *IncomingB = Phi.getIncomingBlock(I);
      // incomingB->print(errs());

      if (MergedR->contains(IncomingB)) {
        NewPhi->addIncoming(IncomingV, IncomingB);
        IncomingBlocksToDelete.push_back(IncomingB);
      }
    }
    Phi.addIncoming(NewPhi, NewExit);
    // remove incoming values from within region for the old exit
    for (auto BB : IncomingBlocksToDelete) {
      Phi.removeIncomingValue(BB);
    }
  }

  // unlink the old exit from the region and link new exit block to region
  SmallVector<BasicBlock *, 4> PredsWithinRegion;
  for (auto It = pred_begin(Exit); It != pred_end(Exit); ++It) {
    BasicBlock *Pred = *It;
    // TODO : self loops?
    if (MergedR->contains(Pred))
      PredsWithinRegion.push_back(Pred);
  }

  for (BasicBlock *Pred : PredsWithinRegion) {
    Pred->getTerminator()->replaceSuccessorWith(Exit, NewExit);
  }

  return NewExit;
}

bool RegionMelder::isInsideMeldedRegion(BasicBlock *BB, BasicBlock *Entry,
                                        BasicBlock *Exit) {
  // melded region is a single BB
  if (!Exit) {
    return BB == Entry;
  }
  // melded region has mutiple BBs
  return (MA.getCFGInfo().getDomTree().dominates(Entry, BB) &&
          MA.getCFGInfo().getPostDomTree().dominates(Exit, BB));
}

void RegionMelder::mergeOutsideDefsAtEntry() {

  SmallVector<BasicBlock *, 16> LeftEntryPreds, RightEntryPreds;

  auto CreateUnifyingBB = [&]() {
    BasicBlock *UnifyBB =
        BasicBlock::Create(MA.getParentFunction()->getContext(), "unify.bb",
                           MA.getParentFunction(), EntryBlockL);
    BranchInst::Create(UnifyBB, UnifyBB, MA.getDivergentCondition(), UnifyBB);

    for (auto &PHI : EntryBlockL->phis()) {
      // add a new phi in unifying block
      PHINode *MovedPHI =
          PHINode::Create(PHI.getType(), 1, "moved.phi", &*UnifyBB->begin());
      for (unsigned int I = 0; I < PHI.getNumIncomingValues(); ++I) {
        if (!isInsideMeldedRegion(PHI.getIncomingBlock(I), EntryBlockL,
                                  ExitBlockL)) {
          MovedPHI->addIncoming(PHI.getIncomingValue(I),
                                PHI.getIncomingBlock(I));
          PHI.setIncomingBlock(I, UnifyBB);
          PHI.setIncomingValue(I, MovedPHI);
        }
      }
    }

    for (auto &PHI : EntryBlockR->phis()) {
      // add a new phi in unifying block
      PHINode *MovedPHI =
          PHINode::Create(PHI.getType(), 1, "moved.phi", &*UnifyBB->begin());
      for (unsigned int I = 0; I < PHI.getNumIncomingValues(); ++I) {
        if (!isInsideMeldedRegion(PHI.getIncomingBlock(I), EntryBlockR,
                                  ExitBlockR)) {
          MovedPHI->addIncoming(PHI.getIncomingValue(I),
                                PHI.getIncomingBlock(I));
          PHI.setIncomingBlock(I, UnifyBB);
          PHI.setIncomingValue(I, MovedPHI);
        }
      }
    }

    // find the predecessors of left and right entries
    for (auto *LeftPred :
         make_range(pred_begin(EntryBlockL), pred_end(EntryBlockL))) {
      if (!isInsideMeldedRegion(LeftPred, EntryBlockL, ExitBlockL)) {
        LeftEntryPreds.push_back(LeftPred);
      }
    }

    for (auto *RightPred :
         make_range(pred_begin(EntryBlockR), pred_end(EntryBlockR))) {
      if (!isInsideMeldedRegion(RightPred, EntryBlockR, ExitBlockR)) {
        RightEntryPreds.push_back(RightPred);
      }
    }

    // add missing preds in phi nodes of unifybb
    for (auto &PHI : UnifyBB->phis()) {
      for (auto &Pred : LeftEntryPreds) {
        if (PHI.getBasicBlockIndex(Pred) == -1) {
          PHI.addIncoming(llvm::UndefValue::get(PHI.getType()), Pred);
        }
      }
      for (auto &Pred : RightEntryPreds) {
        if (PHI.getBasicBlockIndex(Pred) == -1) {
          PHI.addIncoming(llvm::UndefValue::get(PHI.getType()), Pred);
        }
      }
    }

    // set the branches correctly
    for (auto &LeftPred : LeftEntryPreds) {
      LeftPred->getTerminator()->replaceSuccessorWith(EntryBlockL, UnifyBB);
    }
    for (auto &RightPred : RightEntryPreds) {
      RightPred->getTerminator()->replaceSuccessorWith(EntryBlockR, UnifyBB);
    }

    UnifyBB->getTerminator()->setSuccessor(0, EntryBlockL);
    UnifyBB->getTerminator()->setSuccessor(1, EntryBlockR);

    return UnifyBB;
  };

  // create a unifiying basic block
  BasicBlock *UnifyingBB = CreateUnifyingBB();

  // add to mergedBBS
  MergedBBs.push_back(UnifyingBB);

  // recompute control-flow analyses
  MA.getCFGInfo().recompute();
  DominatorTree &DT = MA.getCFGInfo().getDomTree();
  PostDominatorTree &PDT = MA.getCFGInfo().getPostDomTree();

  // check if there are any def-use chains that are broken
  for (auto &BB : *MA.getParentFunction()) {
    // only need to check basic blocks detween top entry and unify BB
    if (DT.dominates(MA.getDivergentBlock(), &BB) &&
        PDT.dominates(UnifyingBB, &BB)) {
      // iterate over all users and check for broken def-uses
      for (auto &Def : make_range(BB.begin(), BB.end())) {
        SmallVector<Instruction *, 32> BrokenUsers;
        for (auto &Use : make_range(Def.use_begin(), Def.use_end())) {
          Instruction *User = dyn_cast<Instruction>(Use.getUser());
          // User->print(errs()); errs() << "\n";
          if (!DT.dominates(&Def, Use)) {
            // errs() << "broken user found\n";
            // errs() << "def : ";
            // Def.print(errs());
            // errs() << "\n";
            // errs() << "user : ";
            // User->print(errs());
            // errs() << "\n";

            BrokenUsers.push_back(User);
          }
        }
        PHINode *NewUnifyingPHI = nullptr;
        for (Instruction *BrokenUser : BrokenUsers) {
          // add a new phi node in the unifying block
          if (!NewUnifyingPHI) {
            NewUnifyingPHI = PHINode::Create(Def.getType(), 0, "unify.phi",
                                             &*UnifyingBB->begin());

            for (auto &LeftPred : LeftEntryPreds) {
              if (DT.dominates(Def.getParent(), LeftPred))
                NewUnifyingPHI->addIncoming(&Def, LeftPred);
              else
                NewUnifyingPHI->addIncoming(
                    llvm::UndefValue::get(NewUnifyingPHI->getType()), LeftPred);
            }
            for (auto &RightPred : RightEntryPreds) {
              if (DT.dominates(Def.getParent(), RightPred))
                NewUnifyingPHI->addIncoming(&Def, RightPred);
              else
                NewUnifyingPHI->addIncoming(
                    llvm::UndefValue::get(NewUnifyingPHI->getType()),
                    RightPred);
            }
          }
          BrokenUser->replaceUsesOfWith(&Def, NewUnifyingPHI);
        }
      }
    }
  }
}

void RegionMelder::updateMapping(BasicBlock *NewBb, BasicBlock *OldBb,
                                 bool IsLeft) {
  auto It = CurrMapping.begin();
  bool Found = false;
  for (; It != CurrMapping.end(); ++It) {
    if (IsLeft) {
      if (It->first == OldBb) {

        Found = true;
        break;
      }
    } else {
      if (It->second == OldBb) {
        Found = true;
        break;
      }
    }
  }
  assert(Found && "Old exit not found in the mapping!");
  CurrMapping.erase(It);
  if (IsLeft) {
    CurrMapping.insert(
        std::pair<BasicBlock *, BasicBlock *>(NewBb, ExitBlockR));
  } else {
    CurrMapping.insert(
        std::pair<BasicBlock *, BasicBlock *>(ExitBlockL, NewBb));
  }
}

void RegionMelder::runPreMergePasses(bool RegionAlreadySimplified) {

  // check if exit blocks need to be isolated
  if (ExitBlockL) {
    if (!RegionAlreadySimplified) {
      DEBUG << "Simplifying left region\n";
      BasicBlock *OldExitLeft = ExitBlockL;
      ExitBlockL = simplifyRegion(ExitBlockL, EntryBlockL);
      // update the mapping
      updateMapping(ExitBlockL, OldExitLeft, true);
      MA.getCFGInfo().recompute();
    }
  }

  if (ExitBlockR) {
    if (!RegionAlreadySimplified) {
      DEBUG << "Simplifying right region\n";
      BasicBlock *OldExitRight = ExitBlockR;
      ExitBlockR = simplifyRegion(ExitBlockR, EntryBlockR);
      // update the mapping
      updateMapping(ExitBlockR, OldExitRight, false);
      MA.getCFGInfo().recompute();
    }
  }

  mergeOutsideDefsAtEntry();
}

static void simplifyConditionalBranches(Function *F) {
  // check for conditional branches with same target and fold them
  for (auto &BB : *F) {
    if (BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator())) {
      if (BI->getNumSuccessors() == 2 &&
          BI->getSuccessor(0) == BI->getSuccessor(1)) {
        DEBUG << "Converting conditional branch to unconditional\n";
        IRBuilder<> Builder(BI);
        Builder.CreateBr(BI->getSuccessor(0));
        BI->eraseFromParent();
      }
    }
  }
}

void RegionMelder::runPostOptimizations() {
  Function *F = MA.getParentFunction();
  static int Count = 0;
  // Utils::writeCFGToDotFile(*MA.getParentFunction(), std::to_string(Count++) + ".cfmelder.");
  INFO << "Running post-merge optimizations\n";

  // first, simplify conditional branches
  simplifyConditionalBranches(F);

  // remove empty basic blocks with single incoming edge and single outgoing
  // edge
  for (BasicBlock *BB : MergedBBs) {

    if (pred_size(BB) == 1 && succ_size(BB) == 1 && BB->size() == 1) {
      // errs() << "processing block " << BB->getNameOrAsOperand() << "\n";
      BasicBlock *SinglePred = *pred_begin(BB);
      BasicBlock *SingleSucc = *succ_begin(BB);

      // check if SinglePred is also a predecessor of SingleSucc
      // SinglePred
      //  /   \
      // BB    |
      //  \   /
      // SingleSucc
      if (isa<BranchInst>(SinglePred->getTerminator())) {
        BranchInst *BI = dyn_cast<BranchInst>(SinglePred->getTerminator());
        if (BI->isConditional() && (BI->getSuccessor(0) == SingleSucc ||
                                    BI->getSuccessor(1) == SingleSucc)) {
          Value *Cond = BI->getCondition();
          // move the incoming values from phi nodes nodes to SinglePred
          // as select instructions
          for (auto &PHI : SingleSucc->phis()) {
            Value *TrueIncoming =
                BI->getSuccessor(0) == BB
                    ? PHI.getIncomingValueForBlock(BB)
                    : PHI.getIncomingValueForBlock(SinglePred);
            Value *FalseIncoming =
                BI->getSuccessor(1) == BB
                    ? PHI.getIncomingValueForBlock(BB)
                    : PHI.getIncomingValueForBlock(SinglePred);

            // create a select
            SelectInst *Sel = SelectInst::Create(
                Cond, TrueIncoming, FalseIncoming, "moved.sel", BI);
            // update the incoming value
            PHI.setIncomingValueForBlock(BB, Sel);
            PHI.setIncomingValueForBlock(SinglePred, Sel);
          }
        }
      }

      // connect SinglePred to SingleSucc
      SinglePred->getTerminator()->replaceSuccessorWith(BB, SingleSucc);
      // replace all uses in PHI nodes
      BB->replaceAllUsesWith(SinglePred);
      // unlink
      BB->eraseFromParent();
    }
  }

  // previous step can create conditional branches with same successors
  simplifyConditionalBranches(F);

  // check for phi nodes with identical incoming value, block pairs
  // and fold them

  for (auto &BB : *F) {
    for (PHINode &Phi : BB.phis()) {
      if (Phi.getNumIncomingValues() > pred_size(&BB)) {
        bool Changed = false;
        do {
          Changed = false;
          for (unsigned I = 0; I < Phi.getNumIncomingValues(); I++) {
            BasicBlock *IncomingBlk = Phi.getIncomingBlock(I);
            // only process incoming values from merged blocks
            if (std::find(MergedBBs.begin(), MergedBBs.end(), IncomingBlk) ==
                MergedBBs.end())
              continue;
            for (unsigned J = I + 1; J < Phi.getNumIncomingValues(); J++) {
              if (Phi.getIncomingBlock(I) == Phi.getIncomingBlock(J) &&
                  Phi.getIncomingValue(I) == Phi.getIncomingValue(J)) {
                Phi.removeIncomingValue(J);
                Changed = true;
                break;
              }
            }
            if (Changed) {
              break;
            }
          }
        } while (Changed);
      }
    }
  }

  // remove phi nodes with one incoming value
  SmallVector<PHINode *, 8> PNToDelete;
  for (auto &BB : *MA.getParentFunction()) {
    for (auto &Phi : BB.phis()) {
      if (Phi.getNumIncomingValues() == 1)
        PNToDelete.push_back(&Phi);
    }
  }

  for (PHINode *PN : PNToDelete) {
    DEBUG << "Erasing phi node\n";
    // PN->print(DEBUG);
    // DEBUG << "\n";
    PN->replaceAllUsesWith(PN->getIncomingValue(0));
    PN->eraseFromParent();
  }

  // Utils::writeCFGToDotFile(*F, std::to_string(Count++) + ".cfmelder.");
  // verifyFunction(*F);
}
