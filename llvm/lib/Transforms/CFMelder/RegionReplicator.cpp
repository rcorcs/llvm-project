#include "RegionReplicator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Analysis/MemorySSAUpdater.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Region *RegionReplicator::replicate(BasicBlock *ExpandedBlock,
                                    BasicBlock *MatchedBlock,
                                    Region *RegionToReplicate) {

  BasicBlock *EntryToReplicate = RegionToReplicate->getEntry();
  BasicBlock *ExitToReplicate = RegionToReplicate->getExit();
  // errs() << "replicate CFG\n";
  // replicate the CFG of region and place the ExpandedBlock in right place
  auto RepEntryExitPair =
      replicateCFG(ExpandedBlock, MatchedBlock, RegionToReplicate);

  // errs() << "recompute CF analysis\n";
  // recompute CF analysis
  MA.getCFGInfo().recompute();
  auto RI = MA.getCFGInfo().getRegionInfo();

  Region *ReplicatedR = Utils::getRegionWithEntryExit(
      *RI, RepEntryExitPair.first, RepEntryExitPair.second);
  // errs() << "place PHI nodes in the replicated region for correct def-use\n";
  // place PHI nodes in the replicated region for correct def-use
  ValueToValueMapTy PHIMap;
  addPhiNodes(ExpandedBlock, ReplicatedR, PHIMap);

  // errs() << "concretize the branch conditions within the replicated
  // region\n"; concretize the branch conditions within the replicated region
  concretizeBranchConditions(ExpandedBlock, ReplicatedR, PHIMap);

  // errs() << "finalizing replication\n";
  if (EnableFullPredication) {
    // errs() << "Full predication enabled\n";
    Region *OrigRegion =
        Utils::getRegionWithEntryExit(*RI, EntryToReplicate, ExitToReplicate);
    assert(OrigRegion && "Can not find the replicated region!");
    fullPredicateStores(OrigRegion, MatchedBlock);
  }

  // MA.getCFGInfo().getFunction().getParent()->print(errs(), nullptr);
  return ReplicatedR;
}

void RegionReplicator::getBasicBlockMapping(
    DenseMap<BasicBlock *, BasicBlock *> &Map, bool IsExpandingLeft) {
  for (auto It : Mapping) {
    BasicBlock *Orig = It.first;
    BasicBlock *Replicated = It.second;
    if (IsExpandingLeft) {
      Map.insert(std::pair<BasicBlock *, BasicBlock *>(Replicated, Orig));
    } else {
      Map.insert(std::pair<BasicBlock *, BasicBlock *>(Orig, Replicated));
    }
  }
}

pair<BasicBlock *, BasicBlock *>
RegionReplicator::replicateCFG(BasicBlock *ExpandedBlock,
                               BasicBlock *MatchedBlock,
                               Region *RegionToReplicate) {
  // remember the predessors of expanded block
  SmallVector<BasicBlock *> PredsOfExpandedBlock;
  for (auto It = pred_begin(ExpandedBlock); It != pred_end(ExpandedBlock);
       ++It) {
    PredsOfExpandedBlock.push_back(*It);
  }
  // DenseMap<BasicBlock *, BasicBlock *> Mapping;

  // traverse the region to replicate and make new basic blocks
  SmallSet<BasicBlock *, 32> WorkList;
  SmallVector<BasicBlock *, 32> Visited;
  WorkList.insert(RegionToReplicate->getEntry());

  BasicBlock *ExpandedBlockExit = ExpandedBlock->getUniqueSuccessor();

  // if expanded block does not have a unique successor, create one
  if (!ExpandedBlockExit) {
    BasicBlock *NewUniqueSucc =
        BasicBlock::Create(ExpandedBlock->getParent()->getContext(),
                           "expanded.block.exit", ExpandedBlock->getParent());

    BranchInst *ExpandedBlockBr =
        dyn_cast<BranchInst>(ExpandedBlock->getTerminator());
    assert(ExpandedBlockBr != nullptr &&
           "Expanded block does not have branch instruction at the end! This "
           "case is not handled!");

    BranchInst::Create(ExpandedBlockBr->getSuccessor(0),
                       ExpandedBlockBr->getSuccessor(1),
                       ExpandedBlockBr->getCondition(), NewUniqueSucc);

    // upade incoming blocks of phis in ExpandedBlock's successors
    for (PHINode &PHI : ExpandedBlockBr->getSuccessor(0)->phis()) {
      PHI.replaceIncomingBlockWith(ExpandedBlock, NewUniqueSucc);
    }

    for (PHINode &PHI : ExpandedBlockBr->getSuccessor(1)->phis()) {
      PHI.replaceIncomingBlockWith(ExpandedBlock, NewUniqueSucc);
    }

    ExpandedBlockBr->eraseFromParent();
    BranchInst::Create(NewUniqueSucc, ExpandedBlock);

    ExpandedBlockExit = NewUniqueSucc;
  }
  // assert(ExpandedBlockExit != nullptr &&
  //        "Expanded block does not have a unique successor");

  while (!WorkList.empty()) {
    auto Curr = *WorkList.begin();
    WorkList.erase(Curr);
    BasicBlock *ReplicatedBB = nullptr;
    Visited.push_back(Curr);
    // if the visited block is the block we want to match eventually
    if (Curr == MatchedBlock) {
      ReplicatedBB = ExpandedBlock;
      // if there is a conditional branch at the end of matched block we need
      // add a conditional branch to exapnded block as well
      BranchInst *BI = dyn_cast<BranchInst>(Curr->getTerminator());
      assert(BI && "no branch instruction at end of replicated block!");
      if (BI->isConditional()) {
        ExpandedBlock->getTerminator()->eraseFromParent();
        BranchInst::Create(ExpandedBlock, ExpandedBlock,
                           ConstantInt::getTrue(Type::getInt1Ty(
                               MA.getParentFunction()->getContext())),
                           ExpandedBlock);
      }
    }
    // if not replicate the basic block
    else {
      ReplicatedBB =
          BasicBlock::Create(MA.getParentFunction()->getContext(),
                             "replicated.bb", MA.getParentFunction());
      // add a  branch
      if (dyn_cast<BranchInst>(Curr->getTerminator())->isUnconditional()) {
        BranchInst::Create(ExpandedBlock, ReplicatedBB);
      } else {
        // set branching condition to true, and concretize later
        BranchInst::Create(ExpandedBlock, ExpandedBlock,
                           ConstantInt::getTrue(Type::getInt1Ty(
                               MA.getParentFunction()->getContext())),
                           ReplicatedBB);
      }
    }

    // errs() << "replicating ";
    // Curr->printAsOperand(errs(), false);
    // errs() << " into ";
    // ReplicatedBB->printAsOperand(errs(), false);
    // errs() << "\n";

    // add replaicted basic block to mapping
    Mapping.insert(std::pair<BasicBlock *, BasicBlock *>(Curr, ReplicatedBB));

    // add all successors to visited including exit
    for (auto It = succ_begin(Curr); It != succ_end(Curr); ++It) {
      BasicBlock *Succ = *It;
      if (std::find(Visited.begin(), Visited.end(), Succ) == Visited.end() &&
          (Succ == RegionToReplicate->getExit() ||
           RegionToReplicate->contains(Succ))) {
        WorkList.insert(Succ);
      }
    }
  }

  // set edges in the new region
  for (auto It : Mapping) {
    BasicBlock *Orig = It.first;
    BasicBlock *Replicated = It.second;

    // errs() << "Orig : ";
    // Orig->printAsOperand(errs(), false);
    // errs() << "\n";
    // errs() << "Replicated : ";
    // Replicated->printAsOperand(errs(), false);
    // errs() << "\n";

    for (unsigned I = 0; I < Orig->getTerminator()->getNumSuccessors(); ++I) {
      BasicBlock *OrigSucc = Orig->getTerminator()->getSuccessor(I);
      auto It1 = Mapping.find(OrigSucc);
      // edges inside the region
      if (It1 != Mapping.end()) {
        Replicated->getTerminator()->setSuccessor(I, It1->second);
      }
      // edges to exit
      else {
        Replicated->getTerminator()->setSuccessor(I, ExpandedBlockExit);
      }
    }
  }

  // connect the new entry with rest of the control flow
  BasicBlock *OrigEntry = RegionToReplicate->getEntry();
  BasicBlock *OrigExit = RegionToReplicate->getExit();
  BasicBlock *ReplicatedEntry = Mapping[OrigEntry];
  BasicBlock *ReplicatedExit = Mapping[OrigExit];

  // traverse all predesessors of orig entry and connect to new entry
  for (BasicBlock *Pred : PredsOfExpandedBlock) {
    Pred->getTerminator()->replaceSuccessorWith(ExpandedBlock, ReplicatedEntry);
  }

  return pair<BasicBlock *, BasicBlock *>(ReplicatedEntry, ReplicatedExit);
}

void RegionReplicator::fullPredicateStores(Region *RToReplicate,
                                           BasicBlock *MatchedBlock) {

  for (BasicBlock *BB : RToReplicate->blocks()) {
    if (BB == MatchedBlock) {
      continue;
    }
    SmallVector<StoreInst *> Stores;
    for (Instruction &I : make_range(BB->begin(), BB->end())) {
      if (isa<StoreInst>(&I)) {
        Stores.push_back(cast<StoreInst>(&I));
      }
    }

    IRBuilder<> Builder(BB);
    // process the stores
    for (StoreInst *SI : Stores) {
      Value *NewData = SI->getValueOperand();
      Value *Addr = SI->getPointerOperand();
      // add a load instruction
      Builder.SetInsertPoint(SI);
      LoadInst *OldData = Builder.CreateLoad(Addr, "rr.redun.load");
      // add a select to pick the right value to store
      Value *SelectI = nullptr;
      if (IsExpandingLeft) {
        SelectI = SelectInst::Create(MA.getDivergentCondition(), OldData,
                                     NewData, "rr.store.sel", SI);
      } else {
        SelectI = SelectInst::Create(MA.getDivergentCondition(), NewData,
                                     OldData, "rr.store.sel", SI);
      }
      // give right value to store
      SI->setOperand(0, SelectI);
      SI->setOperand(1, Addr);
    }
  }
}

void RegionReplicator::addPhiNodes(BasicBlock *ExpandedBlock,
                                   Region *ReplicatedRegion,
                                   ValueToValueMapTy &PHIMap) {

  // users of defs in expanded block are only replaced if the user is
  // outside the replicated region
  // auto ShouldReplaceUse = [&] (Use &U) -> bool {
  //   Instruction* User = dyn_cast<Instruction>(U.getUser());
  //   if (User->getParent() == ReplicatedRegion->getExit()) return false;
  //   for (auto * BB : ReplicatedRegion->blocks()){
  //     if (User->getParent() == BB) return false;
  //   }
  //   return true;
  // };
  // errs() << "computing DF\n";
  // compute DF
  DominatorTree &DT = MA.getCFGInfo().getDomTree();
  DominanceFrontier DF;
  DF.analyze(DT);

  // errs() << "get uses outside the expanded block\n";
  // get uses outside the expanded block
  SmallSet<Instruction *, 32> InstrsWithOutsideUses;

  for (auto &I : *ExpandedBlock) {
    for (Use &Use : I.uses()) {
      Instruction *User = cast<Instruction>(Use.getUser());
      if (User->getParent() != ExpandedBlock) {
        // errs() << "instruction has users outside expanded block\n";
        // I.print(errs());
        // errs() << "\n";
        InstrsWithOutsideUses.insert(&I);
        break;
      }
    }
  }

  SmallSet<Instruction *, 32> WorkList(InstrsWithOutsideUses);

  // errs() << "processing the work list\n";
  DenseSet<Instruction *> Visited;
  while (!WorkList.empty()) {
    Instruction *I = *WorkList.begin();
    WorkList.erase(I);

    // errs() << "processing instruction : \n";
    // I->print(errs());
    // errs() << "\n";

    // if (Visited.count(I)) {
    //   errs() << "repeting:"; I->dump();
    // }
    Visited.insert(I);

    // add phi nodes in DF for this instruction
    auto It = DF.find(I->getParent());

    assert(It != DF.end() &&
           "Exapanded block does not have a DF within the replicated region!");

    for (auto *BB : It->second) {
      // the expanded block is outside any loops, if the dominance frontier
      // includes loops header ignore it
      if (BB == I->getParent())
        continue;
      // errs() << "processing frontier at " << BB->getNameOrAsOperand() << "\n";
      // add a phi node only if DF is within the replicated region
      if (ReplicatedRegion->contains(BB) || ReplicatedRegion->getExit() == BB) {
        PHINode *NewPHI =
            PHINode::Create(I->getType(), 0, "rr.phi", &*BB->begin());
        for (auto PredIt = pred_begin(BB); PredIt != pred_end(BB); ++PredIt) {
          BasicBlock *Pred = *PredIt;
          if (DT.dominates(I->getParent(), Pred)) {
            NewPHI->addIncoming(I, Pred);
          } else {
            NewPHI->addIncoming(llvm::UndefValue::get(I->getType()), Pred);
          }
        }
        // I->replaceUsesWithIf(NewPHI, ShouldReplaceUse);
        // figure out what instruction in expanded block NewPHI points to
        // this is neede for RAUW later
        Value *DefInExpandedBlock = I;
        while (PHIMap.count(DefInExpandedBlock) > 0) {
          DefInExpandedBlock = PHIMap[DefInExpandedBlock];
        }

        // check the found def is really in expanded block
        assert(dyn_cast<Instruction>(DefInExpandedBlock)->getParent() ==
                   ExpandedBlock &&
               "found definition is not in expanded block!");

        // update the PHI map
        PHIMap.insert(std::pair<Value *, Value *>(NewPHI, DefInExpandedBlock));
        // this phi must be furthur processed using its DF
        // errs() << "new PHI node : \n";
        // NewPHI->print(errs());
        // errs() << "\n";
        WorkList.insert(NewPHI);
      }
    }
  }

  // errs() << "finally change the phi nodes in the successors of exit\n";
  // finally change the phi nodes in the successors of exit
  BasicBlock *Exit = ReplicatedRegion->getExit();
  for (auto SuccIt = succ_begin(Exit); SuccIt != succ_end(Exit); ++SuccIt) {
    BasicBlock *Succ = *SuccIt;
    for (auto &PN : Succ->phis()) {
      PN.replaceIncomingBlockWith(ExpandedBlock, Exit);
    }
  }
}

static void visitDFS(BasicBlock *From, BasicBlock *To,
                     SmallVector<BasicBlock *> CurrPath,
                     DenseSet<BasicBlock *> Visited,
                     SmallVector<SmallVector<BasicBlock *>> &Paths) {
  Visited.insert(From);
  CurrPath.push_back(From);

  if (From == To) {
    Paths.push_back(CurrPath);
  } else {
    for (auto SuccIt = succ_begin(From); SuccIt != succ_end(From); SuccIt++) {
      BasicBlock *Succ = *SuccIt;
      if (!Visited.count(Succ)) {
        visitDFS(Succ, To, CurrPath, Visited, Paths);
      }
    }
  }
  Visited.erase(From);
}

void RegionReplicator::concretizeBranchConditions(BasicBlock *ExpandedBlock,
                                                  Region *ReplicatedRegion,
                                                  ValueToValueMapTy &PHIMap) {

  BasicBlock *Entry = ReplicatedRegion->getEntry();
  BasicBlock *Exit = ReplicatedRegion->getExit();

  SmallVector<SmallVector<BasicBlock *>> Paths;
  DenseSet<BasicBlock *> Visited;
  SmallVector<BasicBlock *> CurrPath;

  // use DFS to enumerate all paths from entry to exit
  visitDFS(Entry, Exit, CurrPath, Visited, Paths);

  int BestPathIndex = -1;
  for (unsigned PathIdx = 0; PathIdx < Paths.size(); PathIdx++) {
    for (auto *BB : Paths[PathIdx]) {
      if (BB == ExpandedBlock) {
        if (BestPathIndex == -1)
          BestPathIndex = PathIdx;
        else {
          BestPathIndex = Paths[PathIdx].size() < Paths[BestPathIndex].size()
                              ? PathIdx
                              : BestPathIndex;
        }
      }
    }
  }

  // at least one path must contain ExpandedBlock
  assert(BestPathIndex != -1 && "None of the paths in replicated region goes "
                                "through the expanded block!");

  DEBUG << "Path found through the replicated block\n";
  auto BestPath = Paths[BestPathIndex];
  // for (unsigned BBIdx = 0; BBIdx < BestPath.size(); BBIdx++) {
  //   DEBUG << BestPath[BBIdx]->getNameOrAsOperand() << ((BBIdx !=
  //   BestPath.size() -1 )? " -> " : "\n");
  // }

  // concretize the branch conditions along the found path
  Value *TrueV = ConstantInt::getTrue(
      Type::getInt1Ty(MA.getParentFunction()->getContext()));
  Value *FalseV = ConstantInt::getFalse(
      Type::getInt1Ty(MA.getParentFunction()->getContext()));
  for (unsigned BBIdx = 0; BBIdx < BestPath.size() - 1; BBIdx++) {
    BasicBlock *BB = BestPath[BBIdx];
    BasicBlock *BBNext = BestPath[BBIdx + 1];

    BranchInst *BI = dyn_cast<BranchInst>(BB->getTerminator());
    if (BI->isConditional()) {
      Value *Cond = (BI->getSuccessor(0) == BBNext) ? TrueV : FalseV;
      BI->setCondition(Cond);
    }
  }

  // now we need to fix the broken def-use chains using the PHIMap
  // find broken def-use chains for all defs in expanded block
  BasicBlock *BestPathSecondExit = BestPath[BestPath.size() - 2];
  SmallVector<std::pair<Instruction *, Instruction *>> BrokenDefUsers;
  DominatorTree &DT = MA.getCFGInfo().getDomTree();
  PostDominatorTree &PDT = MA.getCFGInfo().getPostDomTree();

  // checks if a given instruction is inside replicated region
  auto IsInsideReplicatedRegion = [&](Instruction *I) -> bool {
    BasicBlock *Parent = I->getParent();
    return DT.dominates(Entry, Parent) && PDT.dominates(Exit, Parent);
  };

  // find broken def-user. Defs are inside the expanded block.
  // users are outside the replicated region
  for (Instruction &Def : *ExpandedBlock) {
    for (Use &U : Def.uses()) {
      Instruction *User = dyn_cast<Instruction>(U.getUser());
      if (!DT.dominates(&Def, User) && !IsInsideReplicatedRegion(User)) {
        DEBUG << "Def-Use chain is broken for definition "
              << Def.getNameOrAsOperand() << " and user "
              << User->getNameOrAsOperand() << "\n";

        BrokenDefUsers.push_back(
            std::pair<Instruction *, Instruction *>(&Def, User));
      }
    }
  }

  // if broken def-uses are there we must have PHI nodes in the replicated exit
  // added by addPhiNodes step
  if (BrokenDefUsers.size()) {
    assert(isa<PHINode>(*Exit->begin()) &&
           "there are broken def-use to fix but replicated exit does not "
           "contain PHI nodes!");
  }

  // RAUW phase : for each broken user we find the corresponding defining PHI node 
  // in the region exit. this PHI node must have non-undef value for the incoming path
  // selected in concretizeBranchCinditions
  for (auto &DefUser : BrokenDefUsers) {
    Instruction *Def = DefUser.first;
    Instruction *User = DefUser.second;
    for (auto &PHI : Exit->phis()) {

      Value *PHIVal = dyn_cast<Value>(&PHI);

      if (PHIMap[PHIVal] == dyn_cast<Value>(Def) &&
          PHI.getIncomingValueForBlock(BestPathSecondExit) !=
              llvm::UndefValue::get(PHI.getType())) {
        User->replaceUsesOfWith(Def, PHIVal);
      }
    }
  }
}
