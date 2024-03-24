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
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/IRBuilder.h>
#include "llvm/Analysis/TargetTransformInfo.h"

#include "llvm/Support/Error.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"

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

static cl::opt<bool> Debug("brfusion-debug", cl::init(false), cl::Hidden,
                           cl::desc("Enable debug information"));

static cl::opt<bool>
    EnableSOA("brfusion-soa", cl::init(true), cl::Hidden,
              cl::desc("Enable the state-of-the-art brfusion technique"));

static cl::opt<bool>
    ForceAll("brfusion-force", cl::init(false), cl::Hidden,
             cl::desc("Force all valid branch fusion transformations found"));

static cl::opt<int> TraversalStrategy(
    "brfusion-traversal", cl::init(0), cl::Hidden,
    cl::desc("Select which traversal strategy: 0:rpo 1:po 2:dominated-first"));

static cl::opt<int> ReductionThreshold(
    "brfusion-threshold", cl::init(2), cl::Hidden,
    cl::desc("Select which traversal strategy: 0:rpo 1:po 2:dominated-first"));

static cl::opt<bool>
    WriteDotFile("brfusion-dot", cl::init(false), cl::Hidden,
             cl::desc("Write .dot files of the successful branch fusion"));

static cl::opt<bool> UseCostInFingerprint(
    "brfusion-fingerprint-cost", cl::init(true), cl::Hidden,
    cl::desc("Use instruction cost as part of the fingerprint"));


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
  unsigned CountChanges;

  bool merge(Function &F, BranchInst *BI, DominatorTree &DT,
           TargetTransformInfo &TTI, std::list<BranchInst *> &ListBIs,
           std::unordered_set<std::string> &PreviousBlocks);
public:
  BranchFusion() : CountChanges(0) {}
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
  if (BF.runImpl(F)) //, GTTI))
    return PreservedAnalyses::none();
  return PreservedAnalyses::all();
}

/*
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
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    // AU.addRequired<TargetTransformInfoWrapperPass>();
    //  ModulePass::getAnalysisUsage(AU);
  }
};
*/

class SEMERegion {
private:
  BasicBlock *Entry;
  std::vector<BasicBlock *> Blocks;
  std::vector<BasicBlock *> Exits;

  void collectDominatedRegion(BasicBlock *BB, DominatorTree &DT,
                              std::set<Value *> &Visited);

public:
  using iterator =
      PtrToRefIterator<BasicBlock, std::vector<BasicBlock *>::iterator>;

  SEMERegion(BasicBlock *Entry, DominatorTree &DT) : Entry(Entry) {
    std::set<Value *> Visited;
    collectDominatedRegion(Entry, DT, Visited);
  }

  const BasicBlock &getEntryBlock() const { return *Entry; }
  BasicBlock &getEntryBlock() { return *Entry; }

  bool contains(BasicBlock *BB) {
    return std::find(Blocks.begin(), Blocks.end(), BB) != Blocks.end();
  }
  bool isExitBlock(BasicBlock *BB) {
    return std::find(Exits.begin(), Exits.end(), BB) != Exits.end();
  }

  iterator begin() { return iterator(Blocks.begin()); }
  iterator end() { return iterator(Blocks.end()); }

  iterator exit_begin() { return iterator(Exits.begin()); }
  iterator exit_end() { return iterator(Exits.end()); }

  iterator_range<iterator> exits() {
    return make_range<iterator>(exit_begin(), exit_end());
  }

  size_t size() { return Blocks.size(); }

  size_t getNumExitBlocks() { return Exits.size(); }

  BasicBlock *getUniqueExitBlock() {
    if (Exits.size() == 1)
      return *Exits.begin();
    else
      return nullptr;
  }
};

void SEMERegion::collectDominatedRegion(BasicBlock *BB, DominatorTree &DT,
                                        std::set<Value *> &Visited) {
  if (DT.dominates(Entry, BB)) {
    // errs() << Entry->getName() << " dominate " << BB->getName() << "\n";
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
    // errs() << Entry->getName() << " NOT dominate " << BB->getName() << "\n";
  }
}


struct MatchingBlocks {
  BasicBlock *Blocks[2];
  std::map<Instruction *, Instruction *> MatchingInsts;

  MatchingBlocks() { Blocks[0] = Blocks[1] = nullptr; }

  MatchingBlocks(BasicBlock *BB0, BasicBlock *BB1) {
    Blocks[0] = BB0;
    Blocks[1] = BB1;
  }

  BasicBlock *operator[](size_t i) { return Blocks[i]; }

  void setMatchingPair(Instruction *I1, Instruction *I2) {
    MatchingInsts[I1] = I2;
    MatchingInsts[I2] = I1;
  }

  bool isMatchingPair(Instruction *I1, Instruction *I2) {
    if (MatchingInsts.find(I1) == MatchingInsts.end())
      return false;
    return (MatchingInsts[I1] == I2);
  }

  Instruction *getMatchingInstruction(Instruction *I) {
    if (MatchingInsts.find(I) == MatchingInsts.end())
      return nullptr;
    return MatchingInsts[I];
  }
};

class DotPrinter {
public:
  std::string Str;
  raw_string_ostream OS;
  BranchInst *BI;
  SEMERegion &LeftR;
  SEMERegion &RightR;
	ModuleSlotTracker MST;
  DotPrinter(Function &F, BranchInst *BI, SEMERegion &LeftR, SEMERegion &RightR, AlignedSequence<Value *> &AlignedInsts) : OS(Str), BI(BI), LeftR(LeftR), RightR(RightR), MST(F.getParent()) {

    std::map<BasicBlock *, std::string> BB2Id;
    std::map<BasicBlock *, std::string> PortPrefix;

    write("digraph {\n");
    std::set<BasicBlock *> Visited;
    unsigned NId = 0;

    MatchingBlocks MB;
    int ClosedMB = 0;

    for (auto &Entry : AlignedInsts) {
      if (Entry.get(0)) {
         if (isa<BasicBlock>(Entry.get(0)))
          MB.Blocks[0] = dyn_cast<BasicBlock>(Entry.get(0));
        else if (Instruction *I = dyn_cast<Instruction>(Entry.get(0))) {
          if (I->isTerminator()) ClosedMB = ClosedMB | 1;
        }
      }
			
			if (Entry.get(1)) {
        if (isa<BasicBlock>(Entry.get(1)))
          MB.Blocks[1] = dyn_cast<BasicBlock>(Entry.get(1));
				else if (Instruction *I = dyn_cast<Instruction>(Entry.get(1))) {
					if (I->isTerminator()) ClosedMB = ClosedMB | 2;
				}
			}

      if (Entry.match()) {
        Instruction *I1 = dyn_cast<Instruction>(Entry.get(0));
        Instruction *I2 = dyn_cast<Instruction>(Entry.get(1));
        if (I1 && I2) MB.setMatchingPair(I1,I2);
      }

      if (ClosedMB==3) {
        if (MB.MatchingInsts.size()) {
          BasicBlock *BB1 = MB[0];
          BasicBlock *BB2 = MB[1];
          std::string Name = std::string("m_bb")+std::to_string(NId);
          writeNodePair(MB, Name);
          NId++;
          Visited.insert(BB1);
          Visited.insert(BB2);

          BB2Id[BB1] = Name;
          BB2Id[BB2] = Name;
          PortPrefix[BB1] = "f1";
          PortPrefix[BB2] = "f2";
				}
        ClosedMB = 0;
				MB.MatchingInsts.clear();
      }
    }

    NId = 0;
    for (BasicBlock &BB : F) {
      if (Visited.count(&BB)) continue;
      std::string Name = std::string("f_bb")+std::to_string(NId);
      writeNode(BB, Name,"f");
      NId++;
      Visited.insert(&BB);
      BB2Id[&BB] = Name;
      PortPrefix[&BB] = "f";
    }

    Visited.clear();
    for (BasicBlock &BB : F) {
      if (Visited.count(&BB)) continue;
      auto *TI = BB.getTerminator();
      BranchInst *Br = dyn_cast<BranchInst>(TI);
      for (unsigned i = 0; i<TI->getNumSuccessors(); i++) {
        write(BB2Id[&BB]+std::string(":")+PortPrefix[&BB]);
				if (Br && Br->isConditional()) { if (i==0) write("T"); else write("F"); }
				write(" -> ");
				std::string NodeAddr = BB2Id[TI->getSuccessor(i)]+std::string(":")+PortPrefix[TI->getSuccessor(i)];
				write(NodeAddr);
				write("\n");
      }
      Visited.insert(&BB);
    }

    write("}\n");
  }


  void write(std::string s) {
    Str += s;
  }

  void write(const char *s) {
    Str += s;
  }

  void writeBlockEntry(BasicBlock &BB) {
    write(BB.getName().str());
    write(":");
  }

  void write(Instruction &I) {
    I.print(OS, MST, false);
  }

  void writeNode(BasicBlock &BB, std::string Name, std::string Port) {
    bool HasCondBr = false;
    if (BranchInst *Br = dyn_cast<BranchInst>(BB.getTerminator())) {
      HasCondBr = Br->isConditional();
    }
    write(Name);
    write(" [shape = none, ");
    if (LeftR.contains(&BB))
      write(" xlabel=\"L\", ");
    else if (RightR.contains(&BB))
      write(" xlabel=\"R\", ");
    write("label  = <<table ");
    if (LeftR.contains(&BB))
      write(" bgcolor=\"LightYellow\" ");
    else if (RightR.contains(&BB))
      write(" bgcolor=\"LightCyan\" ");
    write(" border=\"0\" cellspacing=\"0\">\n");
    if (HasCondBr)
      write(std::string("<tr><td port=\"")+Port+"\" border=\"1\" colspan=\"2\">");
    else
      write(std::string("<tr><td port=\"")+Port+"\" border=\"1\">");
    writeBlockInnerTable(BB);
    write("</td></tr>\n");
    if (HasCondBr)
      write(std::string("<tr><td border=\"1\" port=\"")+Port+std::string("T\">T</td><td border=\"1\" port=\"")+Port+"F\">F</td></tr>\n");
    write("</table>> ]\n");
  }

  void writeNodePair(MatchingBlocks &MB, std::string Name) {
    BasicBlock *BB1 = MB[0];
    BasicBlock *BB2 = MB[1];

    bool HasCondBr1 = false;
    if (BranchInst *Br = dyn_cast<BranchInst>(BB1->getTerminator())) {
      HasCondBr1 = Br->isConditional();
    }
    bool HasCondBr2 = false;
    if (BranchInst *Br = dyn_cast<BranchInst>(BB2->getTerminator())) {
      HasCondBr2 = Br->isConditional();
    }

    write(Name);
    write(" [shape = none, label  = <<table border=\"0\" cellspacing=\"0\">\n");

    auto It1 = BB1->begin();
    auto It2 = BB2->begin();
    write("<tr><td port=\"f1\" border=\"1\" colspan=\"2\">");
    write("<table bgcolor=\"LightYellow\" border=\"0\">\n");
    write("<tr><td align=\"left\">");
    writeBlockEntry(*BB1);
    write("</td></tr>\n");
    while (It1!=BB1->end() && It2!=BB2->end()) {
      if (It1==BB1->end()) {
        write("<tr><td bgcolor=\"#e8765c70\" align=\"left\"> </td></tr>\n");
        It2++;
      } else {
				Instruction *MI = MB.getMatchingInstruction(&*It1);
				if (MI) {
					while (MI!=(&*It2)) {
            write("<tr><td bgcolor=\"#e8765c70\" align=\"left\"> </td></tr>\n");
						It2++;
					}
          write("<tr><td bgcolor=\"lightgreen\" align=\"left\">");
          write(*It1);
          write("</td></tr>\n");
					It2++;
				} else {
          write("<tr><td bgcolor=\"#e8765c70\" align=\"left\">");
          write(*It1);
          write("</td></tr>\n");
				}
				It1++;
      }
    }
    write("</table>\n");
    write("</td>\n");

    It1 = BB1->begin();
    It2 = BB2->begin();
    write("<td port=\"f2\" border=\"1\" colspan=\"2\">");
    write("<table bgcolor=\"LightCyan\" border=\"0\">\n");
    write("<tr><td align=\"left\">");
    writeBlockEntry(*BB2);
    write("</td></tr>\n");
    while (It1!=BB1->end() && It2!=BB2->end()) {
      if (It2==BB2->end()) {
        write("<tr><td bgcolor=\"#e8765c70\" align=\"left\"> </td></tr>\n");
        It1++;
      } else {
        Instruction *MI = MB.getMatchingInstruction(&*It2);
				if (MB.isMatchingPair(&*It1,&*It2)) {
          write("<tr><td bgcolor=\"lightgreen\" align=\"left\">");
          write(*It2);
          write("</td></tr>\n");
					It1++;
				} else {
					while (It1!=BB1->end() && MB.getMatchingInstruction(&*It1)==nullptr) {
						write("<tr><td bgcolor=\"#e8765c70\" align=\"left\"> </td></tr>\n");
						It1++;
					}
          write("<tr><td bgcolor=\"#e8765c70\" align=\"left\">");
          write(*It2);
          write("</td></tr>\n");
				}
				It2++;
      }
    }
    write("</table>\n");
    write("</td></tr>\n");

    if (HasCondBr1 || HasCondBr2) {
      write("<tr>");
      if (HasCondBr1)
        write("<td border=\"1\" port=\"f1T\">T</td><td border=\"1\" port=\"f1F\">F</td>\n");
      else
        write("<td border=\"0\" colspan=\"2\"> </td>\n");
      if (HasCondBr2)
        write("<td border=\"1\" port=\"f2T\">T</td><td border=\"1\" port=\"f2F\">F</td>\n");
      else
        write("<td border=\"0\" colspan=\"2\"> </td>\n");
      write("</tr>\n");
    }

    write("</table>> ]\n");

  }

  void writeBlockInnerTable(BasicBlock &BB) {
    write("<table border=\"0\">\n");
    write("<tr><td align=\"left\">");
    writeBlockEntry(BB);
    write("</td></tr>\n");
    for (Instruction &I: BB) {
      if (&I == this->BI) {
        write("<tr><td bgcolor=\"yellow\" align=\"left\">");
      }else 
        write("<tr><td align=\"left\">");
      write(I);
      write("</td></tr>\n");
    }
    write("</table>\n");
  }
};



bool BranchFusion::merge(Function &F, BranchInst *BI, DominatorTree &DT,
           TargetTransformInfo &TTI, std::list<BranchInst *> &ListBIs,
           std::unordered_set<std::string> &PreviousBlocks) {

  // Check the instruction is not deleted
  // This should not happen (we remove instructions we delete from ListBIs)
  // but apparently some other piece of code might remove branches
  if (BI->getParent() == nullptr)
    return false;

  if (Debug) {
    errs() << "Original version\n";
    F.dump();
    errs() << "Select branch for merging\n";
    BI->dump();
  }

  BasicBlock *BBT = BI->getSuccessor(0);
  BasicBlock *BBF = BI->getSuccessor(1);
  Value *BrCond = BI->getCondition();

  SEMERegion LeftR(BBT, DT);
  SEMERegion RightR(BBF, DT);

  int SizeLeft = 0;
  int SizeRight = 0;

  if (Debug) {
    errs() << "Select branch for merging\n";
    BI->dump();
    errs() << "LEFT REGION:\n";
    for (BasicBlock &BB : LeftR) BB.dump();
    errs() << "RIGHT REGION:\n";
    for (BasicBlock &BB : RightR) BB.dump();
  }

  std::set<BasicBlock *> KnownBBs;
  for (BasicBlock &BB : LeftR) {
    KnownBBs.insert(&BB);

    //predecessors must be within the same region or be the entry block
    for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
      BasicBlock *PredBB = *It;
      if (!LeftR.contains(PredBB) && PredBB != BI->getParent()) {
        return false;
      }
    }

    for (Instruction &I : BB) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeLeft += cost.getValue().value();
    }
  }
  for (BasicBlock &BB : RightR) {
    KnownBBs.insert(&BB);

    //predecessors must be within the same region or be the entry block
    for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
      BasicBlock *PredBB = *It;
      if (!RightR.contains(PredBB) && PredBB != BI->getParent()) {
        return false;
      }
    }

    for (Instruction &I : BB) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeRight += cost.getValue().value();
    }
  }

  //TODO: fix issue with PHI-node assignment from Function Merging
  if (BI->getSuccessor(0)->getUniquePredecessor()!=BI->getParent() ||
      BI->getSuccessor(1)->getUniquePredecessor()!=BI->getParent()) {
    return false;
  }

  //TODO: handle landingpad on exit blocks. This might break the link between the invoke and the landingpad.
  for (BasicBlock &BB : LeftR.exits()) {
    //errs() << "Left Exit: " << BB.getName().str() << "\n";
    if (BB.isLandingPad()) return false;
  }
  for (BasicBlock &BB : RightR.exits()) {
    //errs() << "Right Exit: " << BB.getName().str() << "\n";
    if (BB.isLandingPad()) return false;
  }


  bool IsSingleExit = LeftR.getNumExitBlocks() == 1 &&
                      RightR.getNumExitBlocks() == 1 &&
                      LeftR.getUniqueExitBlock() == RightR.getUniqueExitBlock();

  if (EnableSOA) {
    bool IsSOA = LeftR.size() == 1 && RightR.size() == 1 && IsSingleExit;
    if (!IsSOA) {
      errs() << "Skipping NOT SOA\n";
      return false;
    }
    errs() << "Processing SOA-valid Branch\n";
  }

  // if (!IsSingleExit) return false;

  /* fixed: 
  for (BasicBlock *BB : KnownBBs) {
    for (auto It = pred_begin(BB), E = pred_end(BB); It != E; It++) {
      BasicBlock *PredBB = *It;
      if (!KnownBBs.count(PredBB) && PredBB != BI->getParent()) {
        return false;
      }
    }
  }*/

  AlignmentStats TotalAlignmentStats;
  AlignedSequence<Value *> AlignedInsts =
      FunctionMerger::alignBlocks(LeftR, RightR, TotalAlignmentStats, (UseCostInFingerprint?(&TTI):nullptr) );

  int CountMatchUsefullInsts = 0;
  for (auto &Entry : AlignedInsts) {

    if (Debug) {
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
      } else {
        errs() << "\t-\n";
      }
    }

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
  
  std::string DotStr;
  if (WriteDotFile && (EnableHyFMNW || EnableHyFMPA)) {
    DotPrinter DP(F, BI, LeftR, RightR, AlignedInsts);
    DotStr = DP.Str;
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
    if (Debug) {
      errs() << "Destroying generated code\n";
      F.dump();
    }
    CG.destroyGeneratedCode();
    if (Debug) {
      errs() << "Generated code destroyed\n";
    }
    EntryBB->eraseFromParent();
    if (Debug) {
      errs() << "Branch fusion reversed\n";
      F.dump();
    }

    for (BasicBlock &BB : F) {
      if (!PreviousBlocks.count(BB.getName().str())) {
        errs() << "ERROR: Basic Block kept hanging by rollback: " << BB.getName().str() << "\n";
      }
    }


    return false;
  }

  /*
   Update PHI nodes in the exit blocks from both left and right SEME regions.
  */
  std::map<PHINode *, PHINode *> ReplacedPHIs;

  auto ProcessPHIs = [&](auto ExitSet,
                         std::set<BasicBlock *> &VisitedBB) -> bool {
    for (BasicBlock &BB : ExitSet) {
      if (VisitedBB.count(&BB))
        continue;
      VisitedBB.insert(&BB);

      auto PHIs = BB.phis();

      for (auto It = PHIs.begin(), E = PHIs.end(); It != E;) {
        PHINode *PHI = &*It;
        It++;

        if (Debug) {
          errs() << "Solving PHI node:";
          PHI->dump();
        }

        IRBuilder<> Builder(PHI);
        PHINode *NewPHI = Builder.CreatePHI(PHI->getType(), 0);
        CG.insert(NewPHI);
        VMap[PHI] = NewPHI;
        ReplacedPHIs[PHI] = NewPHI;


        // Same block can be a predecessor multiple times and can have multiple incoming edges into BB
        // To keep BB's predecessor information consistent with the phi incoming values,
        // we need to keep track of the number of incoming edges from each predecessor block
        //std::map<BasicBlock *, std::map<BasicBlock *, Value *>> NewEntries;
        std::map<BasicBlock *, std::map<BasicBlock *, std::pair<Value *, int>>> NewEntries;
        std::set<BasicBlock *> OldEntries;
        for (unsigned i = 0; i < PHI->getNumIncomingValues(); i++) {
          BasicBlock *InBB = PHI->getIncomingBlock(i);
          if (KnownBBs.count(InBB)) {
            Value *NewV = PHI->getIncomingValue(i);
            auto Pair = CG.getNewEdge(InBB, &BB);
            BasicBlock *NewBB = Pair.first;
            if (Instruction *OpI =
                    dyn_cast<Instruction>(PHI->getIncomingValue(i))) {
              NewV = VMap[OpI];

              if (NewV == nullptr) {
                errs() << "ERROR: Null mapped value!\n";
                return false;
              }
            }
            auto result_pair = NewEntries[NewBB].insert({InBB, {NewV, 1}});
            if (!result_pair.second)
              result_pair.first->second.second++;
            //NewEntries[NewBB][InBB] = NewV;
            OldEntries.insert(InBB);
          } else {
            //simply copy incoming values from outside the two regions being merged
            NewPHI->addIncoming(PHI->getIncomingValue(i),PHI->getIncomingBlock(i));
          }
        }

        if (Debug) {
          errs() << "Num entries: " << NewEntries.size() << "\n";
          for (auto &Pair : NewEntries) {
            errs() << "Incoming Block: " << Pair.first->getName().str() << "\n";
            for (auto &Pair2 : Pair.second) {
              errs() << "Block: " << Pair2.first->getName().str() << " -> "; Pair2.second.first->dump();
            }
          }
        }

        if (Debug) {
          errs() << "Creating New PHI\n";
          PHI->dump();
        }
        for (auto &Pair : NewEntries) {
          if (Debug) {
            errs() << "Incoming Block: " << Pair.first->getName().str() << "\n";
          }
          if (Pair.second.size() == 1) {
            auto &InnerPair = *(Pair.second.begin());
            Value *V = InnerPair.second.first;
            int repeats = InnerPair.second.second;
            for (int i = 0; i < repeats; ++i)
              NewPHI->addIncoming(V, Pair.first);
          } else if (Pair.second.size() == 2) {
            /*
            Values that were originally coming from different basic blocks that
            have been merged must be properly handled. In this case, we add a
            selection in the merged incomming block to produce the correct value
            for the phi node.
            */
            if (Debug) {
              errs() << "Found  PHI incoming from two different blocks\n";
            }
            Value *LeftV = nullptr;
            Value *RightV = nullptr;
            int repeats = 0;
            for (auto &InnerPair : Pair.second) {
              if (LeftR.contains(InnerPair.first)) {
                if (Debug) {
                  errs() << "Value coming from the Left block: "
                       << GetValueName(InnerPair.first) << " : ";
                  InnerPair.second.first->dump();
                }
                LeftV = InnerPair.second.first;
              }
              if (RightR.contains(InnerPair.first)) {
                if (Debug) {
                  errs() << "Value coming from the Right block: "
                       << GetValueName(InnerPair.first) << " : ";
                  InnerPair.second.first->dump();
                }
                RightV = InnerPair.second.first;
              }
              repeats = repeats > InnerPair.second.second ? repeats : InnerPair.second.second;
            }

            if (LeftV && RightV) {
              Value *MergedV = LeftV;
              if (LeftV != RightV) {
                IRBuilder<> Builder(Pair.first->getTerminator());
                // TODO: handle if one of the values is the terminator itself!
                MergedV = Builder.CreateSelect(BrCond, LeftV, RightV);
                if (SelectInst *SelI = dyn_cast<SelectInst>(MergedV))
                  CG.insert(SelI);
              }
              for (int i = 0; i < repeats; ++i)
                NewPHI->addIncoming(MergedV, Pair.first);
            } else {
              errs() << "ERROR: THIS IS WEIRD! MAYBE IT SHOULD NOT BE HERE!\n";
              return false;
            }
          } else {
            errs() << "ERROR: THIS IS WEIRD! MAYBE IT SHOULD NOT BE HERE!\n";
            return false;
            /*
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
      */
          }
        }

	/*
	unsigned CountPreds = 0;
        for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
          BasicBlock *PredBB = *It;

	  if (!LeftR.contains(PredBB) && !RightR.contains(PredBB)) {
		  CountPreds++;
		  errs() << "+PredBB: " << PredBB->getName().str() << "\n";
	  } else {
		  errs() << "-PredBB: " << PredBB->getName().str() << "\n";
	  }
        }
	if (CountPreds!=NewPHI->getNumIncomingValues()) {
		errs() << "ERROR: unexpected number of predecessor\n";
	}
	*/

        if (Debug) {
          errs() << "Resulting PHI node:";
          NewPHI->dump();
        }
      }
    }
    return true;
  };

  bool Error = false;

  std::set<BasicBlock *> VisitedBB;
  Error = Error || !ProcessPHIs(LeftR.exits(), VisitedBB);
  Error = Error || !ProcessPHIs(RightR.exits(), VisitedBB);

  /*
  VisitedBB.clear();
  auto ProcessLPs = [&](auto ExitSet,
                         std::set<BasicBlock *> &VisitedBB) -> bool {
    
    for (BasicBlock &BB : ExitSet) {
      if (VisitedBB.count(&BB))
        continue;
      VisitedBB.insert(&BB);
      if (BB.isLandingPad()) {
	errs() << "Candidate for fix up\n";
	BB.dump();
        for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
          BasicBlock *PredBB = *It;
	  errs() << "predecessor: " << PredBB->getName().str() << "\n";
	  PredBB->dump();
	  if (!isa<InvokeInst>(PredBB->getTerminator()) && PredBB->isLandingPad()) {
	    //BB.getLandingPadInst()->replaceAllUsesWith(PredBB->getLandingPadInst());
	    //BB.getLandingPadInst()->eraseFromParent();
	    errs() << "add phi node edge\n";
	  }
	}
      }
    }
    return false;
  };

  Error = Error || !ProcessLPs(LeftR.exits(), VisitedBB);
  Error = Error || !ProcessLPs(RightR.exits(), VisitedBB);
  */

  if (Debug) {
    errs() << "Modified function\n";
    F.dump();
  }

  int MergedSize = 0;
  if (Debug) {
    errs() << "Computing size...\n";
  }
  for (Instruction *I : CG) {
    auto cost = TTI.getInstructionCost(
        I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
    MergedSize += cost.getValue().value();
  }


  if (Debug) {
    errs() << "SizeLeft: " << SizeLeft << "\n";
    errs() << "SizeRight: " << SizeRight << "\n";
    errs() << "Original Size: " << (SizeLeft + SizeRight) << "\n";
    errs() << "New Size: " << MergedSize << "\n";
  }

  errs() << "SizeDiff: " << (SizeLeft + SizeRight) << " X " << MergedSize
         << " : " << ((int)(SizeLeft + SizeRight) - ((int)MergedSize)) << " : ";

  // bool Profitable = MergedSize < SizeLeft + SizeRight + 2;
  bool Profitable = MergedSize < SizeLeft + SizeRight;

  if (Error || (!Profitable && !ForceAll)) {
    errs() << "Unprofitable Branch Fusion! " << F.getName().str() << " ";
    BI->dump();
    if (Debug) {
      errs() << "Destroying generated code\n";
    }

    // F.dump();
    CG.destroyGeneratedCode();
    if (Debug) {
      errs() << "Generated code destroyed\n";
    }
    EntryBB->eraseFromParent();
    if (Debug) {
      errs() << "Branch fusion reversed\n";
      F.dump();
    }

    for (BasicBlock &BB : F) {
      if (!PreviousBlocks.count(BB.getName().str())) {
        errs() << "ERROR: Basic Block kept hanging by rollback: " << BB.getName().str() << "\n";
      }
    }


    return false;
  } else {
    errs() << "Profitable Branch Fusion! " << F.getName().str() << " ";
    BI->dump();
    float Profit = ((float)(SizeLeft + SizeRight) - MergedSize) /
                   ((float)SizeLeft + SizeRight);
    errs() << "Destroying original code: " << (SizeLeft + SizeRight) << " X "
           << MergedSize << ": " << ((int)(Profit * 100.0)) << "% Reduction ["
           << CountMatchUsefullInsts << "] : " << GetValueName(&F) << "\n";

    // errs() << "Before binding the code\n";
    // F.dump();

    IRBuilder<> Builder(BI);
    Instruction *NewBI = Builder.CreateBr(EntryBB);
    BI->eraseFromParent();

    std::vector<Instruction *> DeadInsts;

    for (auto &Pair : ReplacedPHIs) {
      Pair.first->replaceAllUsesWith(Pair.second);
      Pair.first->dropAllReferences();
      DeadInsts.push_back(Pair.first);
    }

    // errs() << "Before deleting the old code\n";
    // F.dump();
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
    for (BasicBlock *BB : KnownBBs) {
      BB->eraseFromParent();
    }

    if (Debug) {
      errs() << "After deleting the old code\n";
      //F.dump();
    }
    if (!CG.commitChanges(true)) {
      //F.dump();
      errs() << "ERROR: committing final changes to the fused branches !!!!!!!\n";
    }
    if (Debug) {
      errs() << "Final version\n";
      F.dump();
    }
    
    if (WriteDotFile && DotStr.size()) {
      std::string Filename = ".brfusion.";
      Filename += std::to_string(CountChanges) + std::string(".")+std::string(F.getName().str())+".dot";
      std::error_code EC;
      raw_fd_ostream File(Filename, EC, sys::fs::OF_Text);
      File << DotStr << "\n";
      File.close();

      CountChanges++;
    }
    return true;
  }
}

static void collectFusableBranches(Function &F,
                                   std::list<BranchInst *> &ListBIs) {
  PostDominatorTree PDT(F);

  std::vector<BranchInst *> BIs;

  ReversePostOrderTraversal<Function *> RPOT(&F);
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

  if (TraversalStrategy == 1) {
    std::reverse(BIs.begin(), BIs.end());
  } else if (TraversalStrategy == 2) {
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
    SizeBefore += cost.getValue().value();
  }

  std::list<BranchInst *> ListBIs;
  collectFusableBranches(F, ListBIs);

  bool Changed = false;
  bool Last_changed = true;
  DominatorTree DT;
  std::unordered_set<std::string> PreviousBlocks;
  while (!ListBIs.empty()) {
    BranchInst *BI = ListBIs.front();
    ListBIs.pop_front();
    if (Last_changed) {
      DT = DominatorTree(F);
      PreviousBlocks.clear();
      std::transform(F.begin(), F.end(), std::inserter(PreviousBlocks, PreviousBlocks.begin()), [](BasicBlock& BB){return BB.getName().str();});
    }
    Last_changed = merge(F, BI, DT, TTI, ListBIs, PreviousBlocks);
    Changed |= Last_changed;
  }

  if (Changed) {
    int SizeAfter = 0;
    for (Instruction &I : instructions(&F)) {
      auto cost = TTI.getInstructionCost(
          &I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      SizeAfter += cost.getValue().value();
    }
    errs() << "FuncSize " << GetValueName(&F) << ": " << SizeBefore << " - "
           << SizeAfter << " = " << (SizeBefore - SizeAfter) << "\n";
  }

  return Changed;
}

/*
char BranchFusionLegacyPass::ID = 0;
INITIALIZE_PASS(BranchFusionLegacyPass, "brfusion",
                "Fuse branches to reduce code size", false, false)

FunctionPass *llvm::createBranchFusionPass() {
  return new BranchFusionLegacyPass();
}
*/

// MODULE VERSION

bool llvm::MergeBranchRegions(Function &F, BranchInst *BI, DominatorTree &DT,
           TargetTransformInfo &TTI, bool RunCleanup) {
  BasicBlock *BBT = BI->getSuccessor(0);
  BasicBlock *BBF = BI->getSuccessor(1);
  Value *BrCond = BI->getCondition();

  SEMERegion LeftR(BBT, DT);
  SEMERegion RightR(BBF, DT);

  if (Debug) {
    errs() << "Select branch for merging\n";
    BI->dump();
    errs() << "LEFT REGION:\n";
    for (BasicBlock &BB : LeftR) BB.dump();
    errs() << "RIGHT REGION:\n";
    for (BasicBlock &BB : RightR) BB.dump();
  }

  std::set<BasicBlock *> KnownBBs;
  for (BasicBlock &BB : LeftR) {
    KnownBBs.insert(&BB);

    //predecessors must be within the same region or be the entry block
    for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
      BasicBlock *PredBB = *It;
      if (!LeftR.contains(PredBB) && PredBB != BI->getParent()) {
        return false;
      }
    }

  }
  for (BasicBlock &BB : RightR) {
    KnownBBs.insert(&BB);

    //predecessors must be within the same region or be the entry block
    for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
      BasicBlock *PredBB = *It;
      if (!RightR.contains(PredBB) && PredBB != BI->getParent()) {
        return false;
      }
    }

  }

  //TODO: fix issue with PHI-node assignment from Function Merging
  if (BI->getSuccessor(0)->getUniquePredecessor()!=BI->getParent() ||
      BI->getSuccessor(1)->getUniquePredecessor()!=BI->getParent()) {
    return false;
  }

  //TODO: handle landingpad on exit blocks. This might break the link between the invoke and the landingpad.
  for (BasicBlock &BB : LeftR.exits()) {
    //errs() << "Left Exit: " << BB.getName().str() << "\n";
    if (BB.isLandingPad()) return false;
  }
  for (BasicBlock &BB : RightR.exits()) {
    //errs() << "Right Exit: " << BB.getName().str() << "\n";
    if (BB.isLandingPad()) return false;
  }


  bool IsSingleExit = LeftR.getNumExitBlocks() == 1 &&
                      RightR.getNumExitBlocks() == 1 &&
                      LeftR.getUniqueExitBlock() == RightR.getUniqueExitBlock();

  if (EnableSOA) {
    bool IsSOA = LeftR.size() == 1 && RightR.size() == 1 && IsSingleExit;
    if (!IsSOA) {
      errs() << "Skipping NOT SOA\n";
      return false;
    }
    errs() << "Processing SOA-valid Branch\n";
  }



  AlignmentStats TotalAlignmentStats;
  AlignedSequence<Value *> AlignedInsts =
      FunctionMerger::alignBlocks(LeftR, RightR, TotalAlignmentStats, (UseCostInFingerprint?(&TTI):nullptr) );

  int CountMatchUsefullInsts = 0;
  for (auto &Entry : AlignedInsts) {

    if (Debug) {
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
      } else {
        errs() << "\t-\n";
      }
    }

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
  
  std::string DotStr;
  if (WriteDotFile && (EnableHyFMNW || EnableHyFMPA)) {
    DotPrinter DP(F, BI, LeftR, RightR, AlignedInsts);
    DotStr = DP.Str;
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
    if (Debug) {
      errs() << "Destroying generated code\n";
      F.dump();
    }
    CG.destroyGeneratedCode();
    if (Debug) {
      errs() << "Generated code destroyed\n";
    }
    EntryBB->eraseFromParent();
    if (Debug) {
      errs() << "Branch fusion reversed\n";
      F.dump();
    }

    return false;
  }

  /*
   Update PHI nodes in the exit blocks from both left and right SEME regions.
  */
  std::map<PHINode *, PHINode *> ReplacedPHIs;

  auto ProcessPHIs = [&](auto ExitSet,
                         std::set<BasicBlock *> &VisitedBB) -> bool {
    for (BasicBlock &BB : ExitSet) {
      if (VisitedBB.count(&BB))
        continue;
      VisitedBB.insert(&BB);

      auto PHIs = BB.phis();

      for (auto It = PHIs.begin(), E = PHIs.end(); It != E;) {
        PHINode *PHI = &*It;
        It++;

        if (Debug) {
          errs() << "Solving PHI node:";
          PHI->dump();
        }

        IRBuilder<> Builder(PHI);
        PHINode *NewPHI = Builder.CreatePHI(PHI->getType(), 0);
        CG.insert(NewPHI);
        VMap[PHI] = NewPHI;
        ReplacedPHIs[PHI] = NewPHI;


        // Same block can be a predecessor multiple times and can have multiple incoming edges into BB
        // To keep BB's predecessor information consistent with the phi incoming values,
        // we need to keep track of the number of incoming edges from each predecessor block
        //std::map<BasicBlock *, std::map<BasicBlock *, Value *>> NewEntries;
        std::map<BasicBlock *, std::map<BasicBlock *, std::pair<Value *, int>>> NewEntries;
        std::set<BasicBlock *> OldEntries;
        for (unsigned i = 0; i < PHI->getNumIncomingValues(); i++) {
          BasicBlock *InBB = PHI->getIncomingBlock(i);
          if (KnownBBs.count(InBB)) {
            Value *NewV = PHI->getIncomingValue(i);
            auto Pair = CG.getNewEdge(InBB, &BB);
            BasicBlock *NewBB = Pair.first;
            if (Instruction *OpI =
                    dyn_cast<Instruction>(PHI->getIncomingValue(i))) {
              NewV = VMap[OpI];

              if (NewV == nullptr) {
                errs() << "ERROR: Null mapped value!\n";
                return false;
              }
            }
            auto result_pair = NewEntries[NewBB].insert({InBB, {NewV, 1}});
            if (!result_pair.second)
              result_pair.first->second.second++;
            //NewEntries[NewBB][InBB] = NewV;
            OldEntries.insert(InBB);
          } else {
            //simply copy incoming values from outside the two regions being merged
            NewPHI->addIncoming(PHI->getIncomingValue(i),PHI->getIncomingBlock(i));
          }
        }

        if (Debug) {
          errs() << "Num entries: " << NewEntries.size() << "\n";
          for (auto &Pair : NewEntries) {
            errs() << "Incoming Block: " << Pair.first->getName().str() << "\n";
            for (auto &Pair2 : Pair.second) {
              errs() << "Block: " << Pair2.first->getName().str() << " -> "; Pair2.second.first->dump();
            }
          }
        }

        if (Debug) {
          errs() << "Creating New PHI\n";
          PHI->dump();
        }
        for (auto &Pair : NewEntries) {
          if (Debug) {
            errs() << "Incoming Block: " << Pair.first->getName().str() << "\n";
          }
          if (Pair.second.size() == 1) {
            auto &InnerPair = *(Pair.second.begin());
            Value *V = InnerPair.second.first;
            int repeats = InnerPair.second.second;
            for (int i = 0; i < repeats; ++i)
              NewPHI->addIncoming(V, Pair.first);
          } else if (Pair.second.size() == 2) {
            /*
            Values that were originally coming from different basic blocks that
            have been merged must be properly handled. In this case, we add a
            selection in the merged incomming block to produce the correct value
            for the phi node.
            */
            if (Debug) {
              errs() << "Found  PHI incoming from two different blocks\n";
            }
            Value *LeftV = nullptr;
            Value *RightV = nullptr;
            int repeats = 0;
            for (auto &InnerPair : Pair.second) {
              if (LeftR.contains(InnerPair.first)) {
                if (Debug) {
                  errs() << "Value coming from the Left block: "
                       << GetValueName(InnerPair.first) << " : ";
                  InnerPair.second.first->dump();
                }
                LeftV = InnerPair.second.first;
              }
              if (RightR.contains(InnerPair.first)) {
                if (Debug) {
                  errs() << "Value coming from the Right block: "
                       << GetValueName(InnerPair.first) << " : ";
                  InnerPair.second.first->dump();
                }
                RightV = InnerPair.second.first;
              }
              repeats = repeats > InnerPair.second.second ? repeats : InnerPair.second.second;
            }

            if (LeftV && RightV) {
              Value *MergedV = LeftV;
              if (LeftV != RightV) {
                IRBuilder<> Builder(Pair.first->getTerminator());
                // TODO: handle if one of the values is the terminator itself!
                MergedV = Builder.CreateSelect(BrCond, LeftV, RightV);
                if (SelectInst *SelI = dyn_cast<SelectInst>(MergedV))
                  CG.insert(SelI);
              }
              for (int i = 0; i < repeats; ++i)
                NewPHI->addIncoming(MergedV, Pair.first);
            } else {
              errs() << "ERROR: THIS IS WEIRD! MAYBE IT SHOULD NOT BE HERE!\n";
              return false;
            }
          } else {
            errs() << "ERROR: THIS IS WEIRD! MAYBE IT SHOULD NOT BE HERE!\n";
            return false;
            /*
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
      */
          }
        }

	/*
	unsigned CountPreds = 0;
        for (auto It = pred_begin(&BB), E = pred_end(&BB); It != E; It++) {
          BasicBlock *PredBB = *It;

	  if (!LeftR.contains(PredBB) && !RightR.contains(PredBB)) {
		  CountPreds++;
		  errs() << "+PredBB: " << PredBB->getName().str() << "\n";
	  } else {
		  errs() << "-PredBB: " << PredBB->getName().str() << "\n";
	  }
        }
	if (CountPreds!=NewPHI->getNumIncomingValues()) {
		errs() << "ERROR: unexpected number of predecessor\n";
	}
	*/

        if (Debug) {
          errs() << "Resulting PHI node:";
          NewPHI->dump();
        }
      }
    }
    return true;
  };

  bool Error = false;

  std::set<BasicBlock *> VisitedBB;
  Error = Error || !ProcessPHIs(LeftR.exits(), VisitedBB);
  Error = Error || !ProcessPHIs(RightR.exits(), VisitedBB);


  if (Error) {
    if (Debug) {
      errs() << "Destroying generated code\n";
    }

    // F.dump();
    CG.destroyGeneratedCode();
    if (Debug) {
      errs() << "Generated code destroyed\n";
    }
    EntryBB->eraseFromParent();
    if (Debug) {
      errs() << "Branch fusion reversed\n";
      F.dump();
    }

    return false;
  } else {
    //float Profit = ((float)(SizeLeft + SizeRight) - MergedSize) /
    //               ((float)SizeLeft + SizeRight);
    //errs() << "Destroying original code: " << (SizeLeft + SizeRight) << " X "
    //       << MergedSize << ": " << ((int)(Profit * 100.0)) << "% Reduction ["
    //       << CountMatchUsefullInsts << "] : " << GetValueName(&F) << "\n";

    // errs() << "Before binding the code\n";
    // F.dump();

    IRBuilder<> Builder(BI);
    Instruction *NewBI = Builder.CreateBr(EntryBB);
    BI->eraseFromParent();

    std::vector<Instruction *> DeadInsts;

    for (auto &Pair : ReplacedPHIs) {
      Pair.first->replaceAllUsesWith(Pair.second);
      Pair.first->dropAllReferences();
      DeadInsts.push_back(Pair.first);
    }

    // errs() << "Before deleting the old code\n";
    // F.dump();
    for (BasicBlock *BB : KnownBBs) {
      for (Instruction &I : *BB) {
        I.replaceAllUsesWith(VMap[&I]);

        I.dropAllReferences();
        DeadInsts.push_back(&I);
      }
    }
    for (Instruction *I : DeadInsts) {
      //if (BranchInst *BI = dyn_cast<BranchInst>(I)) {
      //  ListBIs.remove(BI);
      //}
      I->eraseFromParent();
    }
    for (BasicBlock *BB : KnownBBs) {
      BB->eraseFromParent();
    }

    if (Debug) {
      errs() << "After deleting the old code\n";
      //F.dump();
    }
    if (!CG.commitChanges(RunCleanup)) {
      //F.dump();
      errs() << "ERROR: committing final changes to the fused branches !!!!!!!\n";
    }
    if (Debug) {
      errs() << "Final version\n";
      F.dump();
    }
    
    return true;
  }

  //utility function here
}


PreservedAnalyses BranchFusionModulePass::run(Module &M,
                                            ModuleAnalysisManager &MAM) {
  bool Changed = false;
  SmallVector<Function *, 64> Funcs;

  for (auto &F : M) {
    if (F.isDeclaration())
      continue;
    Funcs.push_back(&F);
  }

  for (Function *F : Funcs) {
    BranchFusion BF;
    Changed |= BF.runImpl(*F);
  }
  if (!Changed)
    return PreservedAnalyses::all();
  PreservedAnalyses PA;
  return PA;
}

/*
class BranchFusionLegacyModulePass : public ModulePass {
public:
  static char ID;
  BranchFusionLegacyModulePass() : ModulePass(ID) {
    initializeBranchFusionLegacyModulePassPass(*PassRegistry::getPassRegistry());
  }
  bool runOnModule(Module &M) override {
    bool Changed = false;
    SmallVector<Function *, 64> Funcs;

    for (auto &F : M) {
      if (F.isDeclaration())
        continue;
      Funcs.push_back(&F);
    }

    for (Function *F : Funcs) {
      BranchFusion BF;
      Changed |= BF.runImpl(*F);
    }
    return Changed;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    // AU.addRequired<TargetTransformInfoWrapperPass>();
    //  ModulePass::getAnalysisUsage(AU);
  }
};

char BranchFusionLegacyModulePass::ID = 0;
INITIALIZE_PASS(BranchFusionLegacyModulePass, "brfusion-module",
                "Fuse branches to reduce code size", false, false)

ModulePass *llvm::createBranchFusionModulePass() {
  return new BranchFusionLegacyModulePass();
}
*/

