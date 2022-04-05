//===- BranchFusion.cpp - A branch fusion pass ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Scalar/BranchFusion.h"

#include "llvm/Transforms/IPO/FunctionMerging.h"

#include "llvm/ADT/PtrToRefUtils.h"

#include "llvm/ADT/PostOrderIterator.h"

#include "llvm/ADT/SequenceAlignment.h"
#include "llvm/ADT/iterator_range.h"

#include "llvm/InitializePasses.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/IRBuilder.h>

#include "llvm/Support/Error.h"

#include "llvm/Support/CommandLine.h"

#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"

#include "llvm/Transforms/Scalar.h"

//#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/BreadthFirstIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"

#include "llvm/Analysis/Utils/Local.h"
#include "llvm/Transforms/Utils/Local.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Utils/FunctionComparator.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"

#include <algorithm>
#include <list>

#include <limits.h>

#include <functional>
#include <queue>
#include <vector>

#include <algorithm>
#include <stdlib.h>
#include <time.h>

#define DEBUG_TYPE "brfusion"

using namespace llvm;

static cl::opt<bool>
    EnableSOA("brfusion-soa", cl::init(true), cl::Hidden,
              cl::desc("Enable the state-of-the-art brfusion technique"));

static cl::opt<bool>
    ForceAll("brfusion-force", cl::init(false), cl::Hidden,
              cl::desc("Force all valid branch fusion transformations found"));

static cl::opt<int>
    TraversalStrategy("brfusion-traversal", cl::init(0), cl::Hidden,
              cl::desc("Select which traversal strategy: 0:rpo 1:po 2:dominated-first"));

static std::string GetValueName(const Value *V) {
  if (V) {
    std::string name;
    raw_string_ostream namestream(name);
    V->printAsOperand(namestream, false);
    return namestream.str();
  } else
    return "[null]";
}

class BranchFusion {
public:
  bool runImpl(Function &F);
};

PreservedAnalyses BranchFusionPass::run(Function &F,
                                        FunctionAnalysisManager &AM) {
  // auto &FAM =
  // AM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();
  // std::function<TargetTransformInfo *(Function &)> GTTI =
  //     [&FAM](Function &F) -> TargetTransformInfo * {
  //   return &FAM.getResult<TargetIRAnalysis>(F);
  // };

  BranchFusion BF;
  if (!BF.runImpl(F)) //, GTTI))
    return PreservedAnalyses::all();
  return PreservedAnalyses::none();
}

class BranchFusionLegacyPass : public FunctionPass {
public:
  static char ID;
  BranchFusionLegacyPass() : FunctionPass(ID) {
    initializeBranchFusionLegacyPassPass(*PassRegistry::getPassRegistry());
  }
  bool runOnFunction(Function &F) override {
    BranchFusion BF;
    return BF.runImpl(F);
  }
  /*
  bool runOnFunction(Function &F) override {
    return false;
    //FunctionMerging FM;
    //return FM.runImpl(M, GTTI);
  }
  */
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    // AU.addRequired<TargetTransformInfoWrapperPass>();
    //  ModulePass::getAnalysisUsage(AU);
  }
};

class SEMERegion {
private:
  BasicBlock *Entry;
  std::vector<BasicBlock *> Blocks;
  std::vector<BasicBlock *> Exits;

  void collectDominatedRegion(BasicBlock *BB, DominatorTree &DT, std::set<Value *> &Visited);
public:
  using iterator = PtrToRefIterator<BasicBlock, std::vector<BasicBlock *>::iterator>;

  SEMERegion(BasicBlock *Entry, DominatorTree &DT) : Entry(Entry) {
    std::set<Value *> Visited;
    collectDominatedRegion(Entry, DT, Visited);
  }

  const BasicBlock &getEntryBlock() const { return *Entry; }
  BasicBlock &getEntryBlock() { return *Entry; }

  bool contains(BasicBlock *BB) { return std::find(Blocks.begin(), Blocks.end(), BB)!=Blocks.end(); }
  bool isExitBlock(BasicBlock *BB) { return std::find(Exits.begin(), Exits.end(), BB)!=Exits.end(); }

  iterator begin() { return iterator(Blocks.begin()); }
  iterator end() { return iterator(Blocks.end()); }

  iterator exit_begin() { return iterator(Exits.begin()); }
  iterator exit_end() { return iterator(Exits.end()); }

  iterator_range<iterator> exits() { return make_range<iterator>(exit_begin(), exit_end()); }

  size_t size() { return Blocks.size(); }

  size_t getNumExitBlocks() { return Exits.size(); }

  BasicBlock *getUniqueExitBlock() { if (Exits.size()==1) return *Exits.begin(); else return nullptr; }
};

void SEMERegion::collectDominatedRegion(BasicBlock *BB, DominatorTree &DT, std::set<Value *> &Visited) {
  if (DT.dominates(Entry, BB)) {
    //errs() << Entry->getName() << " dominate " << BB->getName() << "\n";
    if (Visited.find(BB) != Visited.end())
      return;
    Visited.insert(BB);

    Blocks.push_back(BB);

    for (auto BBIt = succ_begin(BB), EndIt = succ_end(BB); BBIt != EndIt;
         BBIt++) {
      collectDominatedRegion(*BBIt, DT, Visited);
    }
  } else {
    Exits.push_back(BB);
    //errs() << Entry->getName() << " NOT dominate " << BB->getName() << "\n";
  }
}

bool merge(Function &F, BranchInst *BI, DominatorTree &DT,
           TargetTransformInfo &TTI, std::list<BranchInst *> &ListBIs) {
  BI->dump();

  BasicBlock *BBT = BI->getSuccessor(0);
  BasicBlock *BBF = BI->getSuccessor(1);
  Value *BrCond = BI->getCondition();

  SEMERegion LeftR(BBT, DT);
  SEMERegion RightR(BBF, DT);

  int SizeLeft = 0;
  int SizeRight = 0;

  std::set<BasicBlock *> KnownBBs;
  for (BasicBlock &BB : LeftR) {
    KnownBBs.insert(&BB);
    for (Instruction &I : BB) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeLeft += cost.getValue().getValue();
    }
  }
  for (BasicBlock &BB : RightR) {
    KnownBBs.insert(&BB);
    for (Instruction &I : BB) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeRight += cost.getValue().getValue();
    }
  }


  bool IsSingleExit = LeftR.getNumExitBlocks() == 1 && RightR.getNumExitBlocks() == 1 &&
                      LeftR.getUniqueExitBlock() == RightR.getUniqueExitBlock();

  if (EnableSOA) {
    bool IsSOA = LeftR.size() == 1 && RightR.size() == 1 && IsSingleExit;
    if (!IsSOA) {
      errs() << "Skipping NOT SOA\n";
      return false;
    }
    errs() << "Processing SOA-valid Branch\n";
  }

  //if (!IsSingleExit) return false;


  for (BasicBlock *BB : KnownBBs) {
    for (auto It = pred_begin(BB), E = pred_end(BB); It!=E; It++) {
      BasicBlock *PredBB = *It;
      if (!KnownBBs.count(PredBB) && PredBB!=BI->getParent()) {
        return false;
      }
    }
  }

  // only accept instructions as incoming values from the basic blocks
  // being merged.
  for (BasicBlock &BB : LeftR.exits()) {
    for (PHINode &PHI : BB.phis()) {
      for (unsigned i = 0; i < PHI.getNumIncomingValues(); i++) {
        if (KnownBBs.count(PHI.getIncomingBlock(i)))
          if (!isa<Instruction>(PHI.getIncomingValue(i)))
            return false;
      }
    }
  }
  for (BasicBlock &BB : RightR.exits()) {
    for (PHINode &PHI : BB.phis()) {
      for (unsigned i = 0; i < PHI.getNumIncomingValues(); i++) {
        if (KnownBBs.count(PHI.getIncomingBlock(i)))
          if (!isa<Instruction>(PHI.getIncomingValue(i)))
            return false;
      }
    }
  }


  AlignmentStats TotalAlignmentStats;
  AlignedSequence<Value *> AlignedInsts = FunctionMerger::alignBlocks(LeftR, RightR, TotalAlignmentStats);

  int CountMatchUsefullInsts = 0;
  for (auto &Entry : AlignedInsts) {

    errs() << "-----------------------------------------------------\n";
    if (Entry.get(0)) {
      if (isa<BasicBlock>(Entry.get(0)))
        errs() << Entry.get(0)->getName() << "\n";
      else
        Entry.get(0)->dump();
    } else
      errs() << "\t-\n";
    if (Entry.get(1)) {
      if (isa<BasicBlock>(Entry.get(1)))
        errs() << Entry.get(1)->getName() << "\n";
      else
        Entry.get(1)->dump();
    } else
      errs() << "\t-\n";

    if (Entry.match()) {
      if (isa<BinaryOperator>(Entry.get(0)))
        CountMatchUsefullInsts++;
      if (isa<CallInst>(Entry.get(0)))
        CountMatchUsefullInsts++;
      if (isa<InvokeInst>(Entry.get(0)))
        CountMatchUsefullInsts++;
      if (isa<CmpInst>(Entry.get(0)))
        CountMatchUsefullInsts++;
      if (isa<CastInst>(Entry.get(0)))
        CountMatchUsefullInsts++;
      // if (isa<StoreInst>(Pair.first))
      //   CountMatchUsefullInsts++;
    }
  }

  LLVMContext &Context = F.getContext();
  const DataLayout *DL = &F.getParent()->getDataLayout();
  Type *IntPtrTy = DL->getIntPtrType(Context);


  ValueToValueMapTy VMap;
  // initialize VMap
  for (Argument &Arg : F.args()) {
    VMap[&Arg] = &Arg;
  }

  for (BasicBlock &BB : F) {
    if (KnownBBs.count(&BB))
      continue;
    VMap[&BB] = &BB;
    for (Instruction &I : BB) {
      VMap[&I] = &I;
    }
  }

  FunctionMergingOptions Options = FunctionMergingOptions()
                                       .enableUnifiedReturnTypes(false)
                                       .matchOnlyIdenticalTypes(true);

  BasicBlock *EntryBB = BasicBlock::Create(Context, "", &F);

  FunctionMerger::SALSSACodeGen CG(LeftR, RightR);
  CG.setFunctionIdentifier(BrCond)
      .setEntryPoints(BBT, BBF)
      .setReturnTypes(F.getReturnType(), F.getReturnType())
      .setMergedFunction(&F)
      .setMergedEntryPoint(EntryBB)
      .setMergedReturnType(F.getReturnType(), false)
      .setContext(&Context)
      .setIntPtrType(IntPtrTy);
  if (!CG.generate(AlignedInsts, VMap, Options)) {
    errs() << "ERROR: Failed to generate the fused branches!\n";
    errs() << "Destroying generated code\n";

    F.dump();
    CG.destroyGeneratedCode();
    errs() << "Generated code destroyed\n";
    EntryBB->eraseFromParent();
    errs() << "Branch fusion reversed\n";
    F.dump();

    return false;
  }

  int MergedSize = 0;
  errs() << "Computing size...\n";
  for (Instruction *I : CG) {
    auto cost = TTI.getInstructionCost(
        I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
    MergedSize += cost.getValue().getValue();
  }

  errs() << "SizeLeft: " << SizeLeft << "\n";
  errs() << "SizeRight: " << SizeRight << "\n";
  errs() << "Original Size: " << (SizeLeft + SizeRight) << "\n";
  errs() << "New Size: " << MergedSize << "\n";

  errs() << "SizeDiff: " << (SizeLeft + SizeRight) << " X " << MergedSize
  << " : " << ( (int)(SizeLeft + SizeRight) - ((int)MergedSize) ) << " : ";

  //bool Profitable = MergedSize < SizeLeft + SizeRight + 2;
  bool Profitable = MergedSize < SizeLeft + SizeRight;

  if (!Profitable && !ForceAll) {
    errs() << "Unprofitable Branch Fusion!\n";
    errs() << "Destroying generated code\n";

    //F.dump();
    CG.destroyGeneratedCode();
    errs() << "Generated code destroyed\n";
    EntryBB->eraseFromParent();
    errs() << "Branch fusion reversed\n";
    F.dump();

    return false;
  } else {
    errs() << "Profitable Branch Fusion!\n";
    float Profit = ((float)(SizeLeft + SizeRight) - MergedSize) /
                   ((float)SizeLeft + SizeRight);
    errs() << "Destroying original code: " << (SizeLeft + SizeRight) << " X "
           << MergedSize << ": " << ((int)(Profit * 100.0)) << "% Reduction ["
           << CountMatchUsefullInsts << "] : " << GetValueName(&F) << "\n";

    //F.dump();
    IRBuilder<> Builder(BI);
    Instruction *NewBI = Builder.CreateBr(EntryBB);
    BI->eraseFromParent();

    std::set<BasicBlock *> VisitedBB;
    auto ProcessPHIs = [&](auto ExitSet) {
      for (BasicBlock &BB : ExitSet) {
        if (VisitedBB.count(&BB))
          continue;
        VisitedBB.insert(&BB);

        for (Instruction &I : BB) {
          if (PHINode *PHI = dyn_cast<PHINode>(&I)) {
            std::map<BasicBlock *, std::set<Value *>> NewEntries;
            std::set<BasicBlock *> OldEntries;
            for (unsigned i = 0; i < PHI->getNumIncomingValues(); i++) {
              BasicBlock *InBB = PHI->getIncomingBlock(i);
              if (KnownBBs.count(InBB)) {
                if (Instruction *OpI =
                        dyn_cast<Instruction>(PHI->getIncomingValue(i))) {
                  Value *NewV = VMap[OpI];

                  // PHI->setIncomingValue(i,NewV);

                  if (NewV == nullptr)
                    errs() << "ERROR: Null mapped value!\n";

		  auto Pair = CG.getNewEdge(InBB, &BB);
                  BasicBlock *NewBB = Pair.first;
		  /*
                  BasicBlock *NewBB =
                      dyn_cast<Instruction>(
                          VMap[InBB->getTerminator()])
                          ->getParent();
                  */

                  // PHI->setIncomingBlock(i,NewBB);
                  /*
                  if (NewEntries.find(NewBB)!=NewEntries.end() &&
                  NewEntries[NewBB] != NewV) { errs() << "ERROR: Conflicting
                  mapped values!\n"; NewEntries[NewBB]->dump(); NewV->dump();
                  }
                  */

                  NewEntries[NewBB].insert(NewV);
                  OldEntries.insert(InBB);
                } else {
                  errs() << "ERROR: Cannot handle non-instruction values!\n";
                }
              }
            }
            for (BasicBlock *BB : OldEntries) {
              PHI->removeIncomingValue(BB, false);
            }
            //PHI->dump();
            for (auto &Pair : NewEntries) {
              if (Pair.second.size() == 1) {
                Value *V = *Pair.second.begin();
                //errs() << Pair.first->getName() << " -> ";
                //V->dump();
                PHI->addIncoming(V, Pair.first);
              } else {

                IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());
                AllocaInst *Addr = Builder.CreateAlloca(PHI->getType());
                CG.insert(Addr);

                for (Value *V : Pair.second) {
                  if (Instruction *OpI = dyn_cast<Instruction>(V)) {
                    CG.StoreInstIntoAddr(OpI, Addr);
                  } else {
                    errs() << "ERROR: must also handle non-instruction values "
                              "via a select\n";
                  }
                }

                Builder.SetInsertPoint(Pair.first->getTerminator());
                Value *LI = Builder.CreateLoad(PHI->getType(), Addr);

                PHI->addIncoming(LI, Pair.first);
              }
            }
          }
        }
      }
    };
    ProcessPHIs(LeftR.exits());
    ProcessPHIs(RightR.exits());

    std::vector<Instruction *> DeadInsts;
    for (BasicBlock *BB : KnownBBs) {
      for (Instruction &I : *BB) {
        I.replaceAllUsesWith(VMap[&I]);

        I.dropAllReferences();
        DeadInsts.push_back(&I);
      }
    }
    for (Instruction *I : DeadInsts) {
      if (BranchInst *BI = dyn_cast<BranchInst>(I)) {
        ListBIs.remove(BI);
      }
      I->eraseFromParent();
    }
    //F.dump();
    for (BasicBlock *BB : KnownBBs) {
      BB->eraseFromParent();
    }

    //F.dump();
    if (!CG.commitChanges()) {
      F.dump();
      errs() << "ERROR: committing final changes to the fused branches\n";
    }
    return true;
  }
}

static void collectFusableBranches(Function &F, std::list<BranchInst*> &ListBIs) {
  PostDominatorTree PDT(F);

  std::vector<BranchInst *> BIs;

  ReversePostOrderTraversal<Function*> RPOT(&F); 
  for (auto BBIt = RPOT.begin(); BBIt != RPOT.end(); ++BBIt) {
    BasicBlock *BB = (*BBIt);

    if (BB->getTerminator() == nullptr)
      continue;

    BranchInst *BI = dyn_cast<BranchInst>(BB->getTerminator());

    if (BI != nullptr && BI->isConditional()) {

      BasicBlock *BBT = BI->getSuccessor(0);
      BasicBlock *BBF = BI->getSuccessor(1);

      // check if this branch has a triangle shape
      //      bb1
      //      |  \
      //      |  bb2
      //      |  /
      //      bb3
      if (PDT.dominates(BBT, BBF) || PDT.dominates(BBF, BBT))
        continue;

      // otherwise, we can collect the sub-CFGs for each branch and merge them
      //       bb1
      //      /   \
      //     bb2  bb3
      //     ...  ...
      //

      // keep track of BIs
      BIs.push_back(BI);
    }
  }

  if (TraversalStrategy==1) {
    std::reverse(BIs.begin(), BIs.end());
  } else if(TraversalStrategy==2) {
    DominatorTree DT(F);
    DominatorTree *DTPtr = &DT;
    auto SortRuleLambda = [DTPtr](const Instruction *I1,
                                  const Instruction *I2) -> bool {
      if (DTPtr->dominates(I1, I2) == DTPtr->dominates(I2, I1))
        return (I1 < I2);
      else
        return !(DTPtr->dominates(I1, I2));
    };
    std::sort(BIs.begin(), BIs.end(), SortRuleLambda);
  }

  for (BranchInst *BI : BIs) {
    ListBIs.push_back(BI);
  }
}

bool BranchFusion::runImpl(Function &F) {
  if (F.isDeclaration())
    return false;

  TargetTransformInfo TTI(F.getParent()->getDataLayout());


  errs() << "Processing: " << GetValueName(&F) << "\n";

  int SizeBefore = 0;
  for (Instruction &I : instructions(&F)) {
    auto cost = TTI.getInstructionCost(
        &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
    SizeBefore += cost.getValue().getValue();
  }


  std::list<BranchInst *> ListBIs;
  collectFusableBranches(F,ListBIs);

  bool Changed = false;
  while (!ListBIs.empty()) {
    BranchInst *BI = ListBIs.front();
    ListBIs.pop_front();
    DominatorTree DT(F);
    Changed = Changed || merge(F, BI, DT, TTI, ListBIs);
  }

  if (Changed) {
    int SizeAfter = 0;
    for (Instruction &I : instructions(&F)) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeAfter += cost.getValue().getValue();
    }
    errs() << "FuncSize " << GetValueName(&F) << ": " << SizeBefore << " - "
           << SizeAfter << " = " << (SizeBefore - SizeAfter) << "\n";
  }

  return Changed;
}

char BranchFusionLegacyPass::ID = 0;
INITIALIZE_PASS(BranchFusionLegacyPass, "brfusion",
                "Fuse branches to reduce code size", false, false)

FunctionPass *llvm::createBranchFusionPass() {
  return new BranchFusionLegacyPass();
}
