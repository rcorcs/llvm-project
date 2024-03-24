#include "LoopRollingUtils.h"
#include "RegionRolling.h"

#include "llvm/Transforms/Scalar/LoopRolling.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Analysis/DominanceFrontierImpl.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/Verifier.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <cxxabi.h>


#define TEST_DEBUG true

using namespace llvm;

class RegionEntry {
public:
  RegionEntry(BasicBlock *Entry, BasicBlock *Exit) : Entry(Entry), Exit(Exit) {}
  BasicBlock *getEntry() { return Entry; }
  BasicBlock *getExit() { return Exit; }
  
private:
  BasicBlock *Entry;
  BasicBlock *Exit;
};

class AlignedBlock {
public:
  AlignedBlock(bool isEntry, bool isExit) : isEntryNode(isEntry), isExitNode(isExit) {}
  void pushBlock(BasicBlock *BB) { Blocks.push_back(BB); }

  size_t getNumSuccessors() { return Successors.size(); }
  AlignedBlock *getSuccessor(int i) { return Successors[i]; }
  void addSuccessor(AlignedBlock *AB) { Successors.push_back(AB); }

  std::vector<Node*> ScheduledNodes;

  std::vector<BasicBlock*> Blocks;


  bool isEntry() { return isEntryNode; }
  bool isExit() { return isExitNode; }

private:
  std::vector<AlignedBlock*> Successors;
  bool isEntryNode;
  bool isExitNode;
};

class AlignedRegion {
public:
  void releaseMemory() {
    for (AlignedBlock *AB : AlignedBlocks) delete AB;
    AlignedBlocks.clear();
    BlockMap.clear();
  }

  void growGraph(Node *N, ScalarEvolution *SE, std::set<Node*> &Visited);
  template<typename ValueT>
  Node *find(std::vector<ValueT *> &Vs);
  Node *find(Instruction *I);

  bool contains(Value *V) { return ValuesInNode.count(V); }

  AlignedBlock *getAlignedBlock(Instruction *I) {
    BasicBlock *BB = I->getParent();
    for (AlignedBlock *AB : AlignedBlocks) {
      if (std::find(AB->Blocks.begin(), AB->Blocks.end(), BB)!=AB->Blocks.end())
        return AB;
    }
    return nullptr;
  } 

  AlignedBlock *getAlignedBlock(BasicBlock *BB) {
    for (AlignedBlock *AB : AlignedBlocks) {
      if (std::find(AB->Blocks.begin(), AB->Blocks.end(), BB)!=AB->Blocks.end())
        return AB;
    }
    return nullptr;
  }

  AlignedBlock *getAlignedBlock(BasicBlock *BB, unsigned i) {
    for (AlignedBlock *AB : AlignedBlocks) {
      if (AB->Blocks[i]==BB) 
        return AB;
    }
    return nullptr;
  }


  template<typename ValueT>
  AlignedBlock *getAlignedBlock(std::vector<ValueT *> &Vs) {
    std::set<AlignedBlock*> FoundAlignedBlocks;
    bool AllBlocks = true;
    for (unsigned i = 0; i<Vs.size(); i++) {
      if (auto *BB = dyn_cast<BasicBlock>(Vs[i])) {
        if (AlignedBlock *ABX = getAlignedBlock(BB))
          FoundAlignedBlocks.insert(ABX);
      } else AllBlocks = false;
    }

    bool ValidBlock = false;
    if (AllBlocks && FoundAlignedBlocks.size()==1) {
      ValidBlock = true;
      AlignedBlock *AB = *FoundAlignedBlocks.begin();
      for (unsigned i = 0; i<Vs.size(); i++) {
        if (auto *BB = dyn_cast<BasicBlock>(Vs[i])) {
          ValidBlock = ValidBlock && (BB==AB->Blocks[i]);
        }
      }
      if (ValidBlock) return AB;
    }
    return nullptr;
  }


  Node *getLabelNode(AlignedBlock *AB) {
    if (ABToNode.find(AB)!=ABToNode.end()) return ABToNode[AB];
    return nullptr;
  }


  std::vector<BasicBlock*> EntryBlocks;
  std::vector<BasicBlock*> ExitBlocks;
  
  Node *ExitLabelNode;


  std::vector<AlignedBlock *> AlignedBlocks;
  std::map<BasicBlock*, AlignedBlock *> BlockMap;

  std::map<AlignedBlock*, Node*> ABToNode;

  std::vector<Node*> Nodes;
  std::unordered_map<Value*, std::unordered_set<Node*> > NodeMap;
  std::unordered_map<Value*, std::unordered_set<Node*> > NodeMap2;
  std::unordered_set<Value *> ValuesInNode;
  std::unordered_set<Value *> Inputs;

  unsigned getNumRegions() {
    return EntryBlocks.size();
  }

  unsigned getNumLabelNodes() {
    unsigned s = 0;
    for (Node *N : Nodes) {
      if (N->getNodeType()==NodeType::LABEL)
        s++;
    }
    return s;
  }

  unsigned getNumGoodNodes() {
    unsigned s = 0;
    for (Node *N : Nodes) {
      if (N->getNodeType()!=NodeType::LABEL && N->getNodeType()!=NodeType::MISMATCH)
        s++;
    }
    return s;
  }

  AlignedRegion RemoveFirst(unsigned Skip) {
    AlignedRegion NewAR;
    if (Skip==0) return *this;
    if (Skip >= EntryBlocks.size()) return NewAR;

    size_t n = EntryBlocks.size()-Skip;

    for (size_t i = Skip; i<EntryBlocks.size(); i++)
      NewAR.EntryBlocks.push_back(EntryBlocks[i]);
    for (size_t i = Skip; i<ExitBlocks.size(); i++)
      NewAR.ExitBlocks.push_back(ExitBlocks[i]);
   
    std::map<AlignedBlock*,AlignedBlock*> Old2New;
    for (auto *AB : AlignedBlocks) {
      auto *NewAB = new AlignedBlock(AB->isEntry(), AB->isExit());
      for (size_t i = Skip; i<AB->Blocks.size(); i++)
        NewAB->Blocks.push_back(AB->Blocks[i]);
      
      Old2New[AB] = NewAB;
      
      NewAR.AlignedBlocks.push_back(NewAB);
      for (BasicBlock *BB : NewAB->Blocks)
        NewAR.BlockMap[BB] = NewAB;
    }
    for (auto *AB : AlignedBlocks) {
      AlignedBlock *NewAB = Old2New[AB];
      for (unsigned i = 0; i<AB->getNumSuccessors(); i++) {
        NewAB->addSuccessor( Old2New[ AB->getSuccessor(i) ] );
      }
    }

    return NewAR;
  }


  AlignedRegion RemoveLast(unsigned Skip) {
    AlignedRegion NewAR;
    if (Skip==0) return *this;
    if (Skip >= EntryBlocks.size()) return NewAR;

    size_t n = EntryBlocks.size()-Skip;

    NewAR.EntryBlocks = EntryBlocks;
    NewAR.EntryBlocks.resize(n);
    NewAR.ExitBlocks = ExitBlocks;
    NewAR.ExitBlocks.resize(n);
   
    std::map<AlignedBlock*,AlignedBlock*> Old2New;
    for (auto *AB : AlignedBlocks) {
      auto *NewAB = new AlignedBlock(AB->isEntry(), AB->isExit());
      NewAB->Blocks = AB->Blocks;
      NewAB->Blocks.resize(n);
      
      Old2New[AB] = NewAB;
      
      NewAR.AlignedBlocks.push_back(NewAB);
      for (BasicBlock *BB : NewAB->Blocks)
        NewAR.BlockMap[BB] = NewAB;
    }
    for (auto *AB : AlignedBlocks) {
      AlignedBlock *NewAB = Old2New[AB];
      for (unsigned i = 0; i<AB->getNumSuccessors(); i++) {
        NewAB->addSuccessor( Old2New[ AB->getSuccessor(i) ] );
      }
    }

    return NewAR;
  }

  bool align(ScalarEvolution *SE);
  bool validateAlignment();

  template<typename ValueT>
  Node *buildRecurrenceNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *createNode(std::vector<ValueT*> Vs, Node *Parent, ScalarEvolution *SE);

  void addNode(Node *N) {
    if (N) {
      if (std::find(Nodes.begin(), Nodes.end(), N)==Nodes.end()) {
        Nodes.push_back(N);
	if (N->getNodeType()!=NodeType::MISMATCH && N->getNodeType()!=NodeType::MULTI && N->getNodeType()!=NodeType::IDENTICAL && N->getNodeType()!=NodeType::RECURRENCE) {
          //for (auto *V : N->getValues()) {
	  for (unsigned i = 0; i<N->size(); i++) {
            Value *V = N->getValidInstruction(i);
	    if (V) ValuesInNode.insert(V);
            if (V) NodeMap2[V].insert(N);
	  }
	}
	if (N->size()) NodeMap[N->getValue(0)].insert(N);
      }
    }
  }

};


bool IsIsomorphic(BasicBlock *BB1, BasicBlock *ExitBB1, BasicBlock *BB2, BasicBlock *ExitBB2, AlignedBlock *AB, std::set<BasicBlock*> &Visited1, std::set<BasicBlock*> &Visited2) {

  if (Visited1.count(BB1) && Visited2.count(BB2)) return true;
  if (Visited1.count(BB1) || Visited2.count(BB2)) return false;

  Visited1.insert(BB1);
  Visited2.insert(BB2);

  if (AB) AB->pushBlock(BB2);

  if (BB1==ExitBB1 && BB2==ExitBB2)
    return true;
  if (BB1==ExitBB1 || BB2==ExitBB2)
    return false;

  BranchInst *Br1 = dyn_cast<BranchInst>(BB1->getTerminator());
  BranchInst *Br2 = dyn_cast<BranchInst>(BB2->getTerminator());

  if (Br1 && Br2 && Br1->getNumSuccessors()==Br2->getNumSuccessors()) {
    bool Isomorphic = true;
    for (unsigned i = 0; i<Br1->getNumSuccessors(); i++) {
      Isomorphic = Isomorphic && IsIsomorphic(Br1->getSuccessor(i), ExitBB1, Br2->getSuccessor(i), ExitBB2, AB?AB->getSuccessor(i):nullptr, Visited1, Visited2);
    }
    return Isomorphic;
  }

  return false;
}

bool IsIsomorphic(BasicBlock *BB1, BasicBlock *ExitBB1, BasicBlock *BB2, BasicBlock *ExitBB2, AlignedBlock *AB) {
  std::set<BasicBlock*> Visited1, Visited2;
  return IsIsomorphic(BB1, ExitBB1, BB2, ExitBB2, AB, Visited1, Visited2);
}

AlignedBlock *initializeAlignedRegion(AlignedRegion *AR, BasicBlock *BB, BasicBlock *ExitBB, std::set<BasicBlock *> &Visited) {
  //if (BB==ExitBB) return nullptr;
  if (Visited.count(BB)) return AR->BlockMap[BB];

  AlignedBlock *AB = new AlignedBlock(Visited.empty(), BB==ExitBB);

  Visited.insert(BB);

  AB->pushBlock(BB);
  AR->AlignedBlocks.push_back(AB);
  AR->BlockMap[BB] = AB;

  BranchInst *Br = dyn_cast<BranchInst>(BB->getTerminator());
  if (Br && BB!=ExitBB) {
    for (unsigned i = 0; i<Br->getNumSuccessors(); i++) {
      AlignedBlock *SuccAB = initializeAlignedRegion(AR, Br->getSuccessor(i), ExitBB, Visited);
      AB->addSuccessor(SuccAB);
    }
  }

  return AB;
}

void initializeAlignedRegion(AlignedRegion *AR, BasicBlock *BB, BasicBlock *ExitBB) {
  std::set<BasicBlock*> Visited;
  initializeAlignedRegion(AR, BB, ExitBB, Visited);
}


class RegionCodeGenerator {
public:

  RegionCodeGenerator(Function &F, AlignedRegion &AR) : F(F), AR(AR) {}

  Value *cloneGraph(Node *N, IRBuilder<> &Builder);
  void setNodeOperands(Node *N);
  Value *generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder);
  void generateExtract(Node *N, Instruction *NewI, IRBuilder<> &Builder);
  void generateNode(Node *N, IRBuilder<> &Builder);
  bool generate(AlignedRegion &AR);

  AlignedRegion &AR;
  Function &F;

  std::unordered_map<Node*, Value *> NodeToValue;
  std::unordered_map<GlobalVariable*, Instruction *> GlobalLoad;

  std::unordered_set<Instruction *> Garbage;
  std::vector<Value *> CreatedCode;
  std::unordered_map<Instruction *, Instruction *> Extracted;

  //std::unordered_map<Type *, Value *> CachedCastIndVar;
  
  //std::unordered_map<Type *, Value *> CachedRem2;
  //Value *AltSeqCmp{nullptr};

  BasicBlock *PreHeader;
  BasicBlock *Header;
  BasicBlock *Latch;
  BasicBlock *Exit;
  BasicBlock *LoopExit;
  PHINode *IndVar;
};

Value *RegionCodeGenerator::generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder) {
  Module *M = F.getParent();
  LLVMContext &Context = F.getContext();

  bool AllSame = true;
  for (unsigned i = 0; i<VL.size(); i++) {
    AllSame = AllSame && VL[i]==VL[0];
  }
  if (AllSame) return VL[0];

  if (allConstant(VL)) {
    errs() << "All constants\n";

    auto *ArrTy = ArrayType::get(VL[0]->getType(), VL.size());

    SmallVector<Constant*,8> Consts;
    for (auto *V : VL) Consts.push_back(dyn_cast<Constant>(V));

    Value *IndexedValue = nullptr;
    for (auto &Pair : GlobalLoad) {
      auto *GA = Pair.first;

      if (GA->hasInitializer()) {
        auto *C = GA->getInitializer();
        auto *GArrTy = dyn_cast<ArrayType>(C->getType());
        if (GArrTy==nullptr) continue;
	if (ArrTy!=GArrTy) continue;
        if (GArrTy->getNumElements()!=Consts.size()) continue;
        for (unsigned i = 0; i<Consts.size(); i++) {
          if (C->getAggregateElement(i)!=Consts[i]) continue;
        }
        //Found Array
        errs() << "Found Array: "; GA->dump();
        IndexedValue = Pair.second;
        break;
      }
    }
    if (IndexedValue==nullptr) {
      auto *ConstArray = ConstantArray::get(ArrTy, Consts);
      GlobalVariable *GArray = new GlobalVariable(*M, ArrTy,true,GlobalValue::LinkageTypes::PrivateLinkage,ConstArray);
      //CreatedCode.push_back(GArray);
      //GArray->setInitializer(ConstArray);
      SmallVector<Value*,8> Indices;
      Type *IndVarTy = IntegerType::get(Context, 8);
      Indices.push_back(ConstantInt::get(IndVarTy, 0));
      Indices.push_back(IndVar);

      errs() << "Created array: "; GArray->dump();

      auto *GEP = Builder.CreateGEP(VL[0]->getType(), GArray, Indices);
      //CreatedCode.push_back(GEP);

      auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
      //CreatedCode.push_back(Load);

      GlobalLoad[GArray] = Load;
      IndexedValue = Load;
    }
    return IndexedValue;
  } else {
    errs() << "Non constants\n";

    errs() << "Array Type: " << VL.size() << ":"; VL[0]->getType()->dump();

    //BasicBlock &Entry = F->getEntryBlock();
    //IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());
    IRBuilder<> ArrBuilder(PreHeader);

    Type *IndVarTy = IntegerType::get(Context, 8);
    Value *ArrPtr = ArrBuilder.CreateAlloca(VL[0]->getType(), ConstantInt::get(IndVarTy, VL.size()));
    //CreatedCode.push_back(ArrPtr);
    
    errs() << "Created array: "; ArrPtr->dump();

    //ArrBuilder.SetInsertPoint(PreHeaderPt);
    for (unsigned i = 0; i<VL.size(); i++) {
      auto *GEP = ArrBuilder.CreateGEP(VL[i]->getType(), ArrPtr, ConstantInt::get(IndVarTy, i));
      //CreatedCode.push_back(GEP);
      auto *Store = ArrBuilder.CreateStore(VL[i], GEP);
      //CreatedCode.push_back(Store);
    }

    auto *GEP = Builder.CreateGEP(VL[0]->getType(), ArrPtr, IndVar);
    //CreatedCode.push_back(GEP);

    auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
    //CreatedCode.push_back(Load);
    
    return Load;
  }
  return VL[0]; //TODO: return nullptr?
}


void RegionCodeGenerator::generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder) {
  LLVMContext &Context = F.getContext();

  std::set<unsigned> NeedExtract;

  for (unsigned i = 0; i<N->size(); i++) {
    auto *I = N->getValidInstruction(i);
    if (I==nullptr) continue;
    //if (I->getParent()!=(&BB)) continue; //TODO: what is the equivalent for RegionCodeGen? I->getParent() not corresponding exit block?
    for (auto *U : I->users()) {
      if (!AR.contains(U)) {
#ifdef TEST_DEBUG
	errs() << "Found use: " << i << ": "; U->dump();
#endif
        NeedExtract.insert(i);
	break;
      }
    }
  }

  if (NeedExtract.empty()) return;

#ifdef TEST_DEBUG
  //errs() << "Extracting: "; NewI->dump();
#endif

  if (NeedExtract.size()==1 && N->getNodeType()==NodeType::REDUCTION) {
    ReductionNode *RN = (ReductionNode*)N;
    if (RN->getBinaryOperator()==RN->getValidInstruction(*NeedExtract.begin())) {
      return;
    }
  }

  //BasicBlock &Entry = F.getEntryBlock();
  //IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());
  IRBuilder<> ArrBuilder(PreHeader);

  Type *IndVarTy = IntegerType::get(Context, 8);
  Value *ArrPtr = ArrBuilder.CreateAlloca(NewI->getType(), ConstantInt::get(IndVarTy, N->size()));
  //CreatedCode.push_back(ArrPtr);

  auto *GEP = Builder.CreateGEP(NewI->getType(), ArrPtr, IndVar);
  //CreatedCode.push_back(GEP);
  auto *Store = Builder.CreateStore(NewI, GEP);
  //CreatedCode.push_back(Store);

  IRBuilder<> ExitBuilder(LoopExit);
  for (unsigned i : NeedExtract) {
    Instruction *I = N->getValidInstruction(i);
    auto *GEP = ExitBuilder.CreateGEP(NewI->getType(), ArrPtr, ConstantInt::get(IndVarTy, i));
    //CreatedCode.push_back(GEP);
    auto *Load = ExitBuilder.CreateLoad(NewI->getType(), GEP);
    //CreatedCode.push_back(Load);
    Extracted[I] = Load;
  }

}

Value *RegionCodeGenerator::cloneGraph(Node *N, IRBuilder<> &Builder) {
  if(N->getNodeType()==NodeType::LABEL) {
	  errs() << "Getting Label operand\n";
	  printVs(N->getValues());
  }

  if (NodeToValue.find(N)!=NodeToValue.end()) {
	  errs() << "Found Label operand\n";
	  if (NodeToValue[N]==nullptr) errs() << "block is null\n";
	  //else  NodeToValue[N]->dump();

	  return NodeToValue[N];
  }

  errs() << "Here?? need to generate node value\n";
  switch(N->getNodeType()) {
    case NodeType::IDENTICAL: {
#ifdef TEST_DEBUG
      errs() << "Generating IDENTICAL\n";
#endif
      return N->getValue(0);
    }
    case NodeType::MULTI: {
#ifdef TEST_DEBUG
      errs() << "Generating MULTI\n";
#endif
      for (unsigned i = 0; i<N->getNumChildren(); i++) {
        cloneGraph(N->getChild(i), Builder);
      }
      return nullptr; //there is no single value to return
    }
    case NodeType::PHI: {
      errs() << "Generating PHI Node\n";
      IRBuilder<> PHIBuilder(&*Builder.GetInsertBlock()->getFirstInsertionPt());
      PHINode *NewPHI = PHIBuilder.CreatePHI(N->getType(), 0);
      NodeToValue[N] = NewPHI;

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
            //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
	    Garbage.insert(I);
	  }
	}

      generateExtract(N, NewPHI, PHIBuilder);

      return NewPHI;
    }
    case NodeType::MATCH: {
#ifdef TEST_DEBUG
      errs() << "Generating MATCH\n";
#endif

#ifdef TEST_DEBUG
      errs() << "Match: "; 
      
      if (isa<Function>(N->getValue(0))) {
        errs() << N->getValue(0)->getName() << "\n";
      } else {
        errs() << "\n";
        for (auto *V : N->getValues()) V->dump();
      }
#endif
      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I)  {

        for (unsigned x = 0; x<N->size(); x++) {
           if (N->getValidInstruction(x)) {
              errs() << x << ": ";
              N->getValidInstruction(x)->dump();
           }
        }
        errs() << "Generated Match for: "; I->dump();
        errs() << "in block: " << Builder.GetInsertBlock()->getName() << "\n";
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          errs() << "Operand: " << N->getChild(i)->getString() << "\n";
        }
        std::vector<Value*> Operands;
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          Operands.push_back(cloneGraph(N->getChild(i), Builder));
        }
        
        errs() << "Cloning matching instruction\n";
        errs() << "in block: " << Builder.GetInsertBlock()->getName() << "\n";
        Instruction *NewI = I->clone();
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,nullptr);
        }
        NodeToValue[N] = NewI;

#ifdef TEST_DEBUG
        errs() << "Operands done: " << Operands.size() << " (" << NewI->getNumOperands() << ")\n";
#endif

        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        NewI->getAllMetadata(MDs);
	errs() << "HERE........ 1\n";
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }

	errs() << "HERE........ 2\n";
	if (!MatchAlignment) {
          if (auto *LI = dyn_cast<LoadInst>(NewI)) {
            LI->setAlignment(Align());
	  }
	  else if (auto *SI = dyn_cast<StoreInst>(NewI)) {
            SI->setAlignment(Align());
	  }
	}
	

        Builder.Insert(NewI);
        //CreatedCode.push_back(NewI);

        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }
#ifdef TEST_DEBUG
        //errs() << "Generated: "; NewI->dump();
#endif

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
            //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
	    Garbage.insert(I);
	  }
	}
	
        generateExtract(N, NewI, Builder);
        
#ifdef TEST_DEBUG
	//errs() << "Gen: "; NewI->dump();
#endif
        return NewI;
      } else return N->getValue(0); //TODO: maybe an assert false
    }
    case NodeType::CONSTEXPR: {
#ifdef TEST_DEBUG
      errs() << "Generating CONSTEXPR\n";
#endif

#ifdef TEST_DEBUG
      errs() << "Matching ConstExpr: "; 
      if (isa<Function>(N->getValue(0))) {
        errs() << N->getValue(0)->getName() << "\n";
      } else {
        errs() << "\n";
        for (auto *V : N->getValues()) V->dump();
      }
#endif
      auto *I = dyn_cast<ConstantExpr>(N->getValue(0));
      if (I) {
        Instruction *NewI = I->getAsInstruction();
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,nullptr);
        }
        NodeToValue[N] = NewI;
        errs() << "Generated ConstExpr for: "; I->dump();
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          errs() << "Operand: " << N->getChild(i)->getString() << "\n";
        }

        std::vector<Value*> Operands;
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          Operands.push_back(cloneGraph(N->getChild(i), Builder));
        }

        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        NewI->getAllMetadata(MDs);
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }

        Builder.Insert(NewI);
        //CreatedCode.push_back(NewI);

        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
	    Garbage.insert(I);
	  }
	}
        
	
        //generateExtract(N, NewI, Builder);
        

#ifdef TEST_DEBUG
        //errs() << "Generated: "; NewI->dump();
#endif

        return NewI;
      } else return N->getValue(0); //TODO: maybe an assert false
    }
    case NodeType::GEPSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating GEPSEQ\n";
#endif
      //return nullptr;
      auto *GN = (GEPSequenceNode*)N;

      Value *Ptr = GN->getPointerOperand();
      if (Ptr==nullptr) {
        IRBuilder<> PreHeaderBuilder(PreHeader);
        auto *GEP = GN->getReference()->clone();
        auto *IdxTy = GN->getReference()->getOperand(GN->getReference()->getNumOperands()-1)->getType();
        auto *Zero = ConstantInt::get(IdxTy, 0);
        GEP->setOperand(GN->getReference()->getNumOperands()-1, Zero);
        PreHeaderBuilder.Insert(GEP);
        Ptr = GEP;
      }

      assert(GN->getNumChildren() && "Expected child with indices!");
      Value *IndVarIdx = cloneGraph(GN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing GEPSEQ\n";
#endif
      //auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getPointerOperand(), IndVarIdx));
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getReference()->getType(), Ptr, IndVarIdx));
      //CreatedCode.push_back(GEP);
      NodeToValue[N] = GEP;

      for (unsigned i = 0; i<GN->size(); i++) {
        if (auto *I = GN->getValidInstruction(i)) {
	  Garbage.insert(I);
          //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) {
          //  Garbage.push_back(I);
	  //}
        }
      }

      //TODO
      generateExtract(N, GEP, Builder);

#ifdef TEST_DEBUG
      errs() << "Gen: "; GEP->dump();
#endif
      return GEP;
    }
    case NodeType::BINOP: {
#ifdef TEST_DEBUG
      errs() << "Generating BINOP\n";
#endif
      auto *BON = (BinOpSequenceNode*)N;

      assert(BON->getNumChildren() && "Expected child with varying operands!");
      Value *Op0 = cloneGraph(BON->getChild(0), Builder);
      Value *Op1 = cloneGraph(BON->getChild(1), Builder);

#ifdef TEST_DEBUG
      errs() << "Closing BINOP\n";
#endif
      Instruction *NewI = BON->getReference()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      NodeToValue[N] = NewI;
      //CreatedCode.push_back(NewI);

      for (unsigned i = 0; i<BON->size(); i++) {
        if (auto *I = BON->getValidInstruction(i)) {
	  Garbage.insert(I);
          //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) {
          //  Garbage.push_back(I);
	  //}
        }
      }

      NewI->setOperand(0,Op0);
      NewI->setOperand(1,Op1);

      generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
      //errs() << "Gen: "; NewI->dump();
#endif
      return NewI;
    }
    /*
    case NodeType::RECURRENCE: {
#ifdef TEST_DEBUG
      errs() << "Generating RECURRENCE\n";
#endif
      auto *RN = (RecurrenceNode*)N;
          
      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(RN->getStartValue()->getType(),0);
      } else {
        PHI = Builder.CreatePHI(RN->getStartValue()->getType(),0);
      }
      CreatedCode.push_back(PHI);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NodeToValue[N] = PHI;

#ifdef TEST_DEBUG
      errs() << "Gen: "; PHI->dump();
#endif
      
      return PHI;
    }
    case NodeType::REDUCTION: {
#ifdef TEST_DEBUG
      errs() << "Generating REDUCTION\n";
#endif
      auto *RN = (ReductionNode*)N;

      assert(RN->getNumChildren() && "Expected child with varying operands!");
      Value *Op = cloneGraph(RN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing REDUCTION\n";
#endif
      Instruction *NewI = RN->getBinaryOperator()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);
      NodeToValue[N] = NewI;

      for (unsigned i = 0; i<RN->size(); i++) {
        if (auto *I = RN->getValidInstruction(i)) {
	  Garbage.insert(I);
        }
      }

      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      } else {
        PHI = Builder.CreatePHI(NewI->getType(),2);
      }
      //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
      //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      CreatedCode.push_back(PHI);
      PHI->addIncoming(NewI,Header);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NewI->setOperand(0,PHI);
      NewI->setOperand(1,Op);

      Extracted[RN->getBinaryOperator()] = NewI;//PHI;
      generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
      errs() << "Gen: "; NewI->dump();
#endif
      return NewI;
    }
    */
    case NodeType::INTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating INTSEQ\n";
#endif

      Value *NewV = nullptr;

      auto *ISN = (IntSequenceNode*)N;
      auto *StartValue = dyn_cast<ConstantInt>(ISN->getStart());
      auto *StepValue = dyn_cast<ConstantInt>(ISN->getStep());
      Value *CastIndVar = IndVar;
      //if (CachedCastIndVar.find(StartValue->getType())!=CachedCastIndVar.end()) {
      //  CastIndVar = CachedCastIndVar[StartValue->getType()];
      //} else {
	auto *CIndVarI = Builder.CreateIntCast(IndVar, StartValue->getType(), false);
	if (CIndVarI!=IndVar) {
          //CreatedCode.push_back(CastIndVar);
          //CachedCastIndVar[StartValue->getType()] = CIndVarI;
          CastIndVar = CIndVarI;
	}
      //}
      if (StartValue->isZero() && StepValue->isOne()){
        NewV = CastIndVar;
      } else if (StepValue->isZero()){
        NewV = StartValue;
      } else {
	Value *Factor = CastIndVar;
	if (!StepValue->isOne()) {
          auto *Mul = Builder.CreateMul(CastIndVar, StepValue);
          //CreatedCode.push_back(Mul);
	  Factor = Mul;
	}
	auto *Add = Builder.CreateAdd(Factor, StartValue);
        //CreatedCode.push_back(Add);
	NewV = Add;
      }

#ifdef TEST_DEBUG
      errs() << "Gen: "; NewV->dump();
#endif
      NodeToValue[N] = NewV;
      return NewV;
    }
    case NodeType::ALTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating ALTSEQ\n";
#endif
      errs() << "in block: " << Builder.GetInsertBlock()->getName() << "\n";

      auto *ASN = (AlternatingSequenceNode*)N;

      auto *SeqTy = ASN->getFirst()->getType();

      errs() << "Values:\n";
      for (unsigned i = 0; i<N->size(); i++) N->getValue(i)->dump();

      Value *NewV = nullptr;

      auto IsNegatedInt = [](const APInt &V1, const APInt &V2) -> bool {
	APInt NegV1(V1);
        NegV1.negate(); //in place
	return V1.eq(V2);
      };

      auto *CInt1 = dyn_cast<ConstantInt>(ASN->getFirst());
      auto *CInt2 = dyn_cast<ConstantInt>(ASN->getSecond());
      if (CInt1 && CInt2 && CInt1->isZero() && CInt2->isOne()) {
        errs() << "Generated Version 1:\n";
	
        /*
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          NewV = CachedRem2[SeqTy];
	} else {
        */
        Value *CastIndVar = IndVar;
        /*
        if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
          CastIndVar = CachedCastIndVar[SeqTy];
        } else {*/
          auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
          if (CIndVarI!=IndVar) {
            CreatedCode.push_back(CastIndVar);
            //CachedCastIndVar[SeqTy] = CIndVarI;
            CastIndVar = CIndVarI;
          }
        //}

        auto *Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
        if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
          CreatedCode.push_back(Rem2I);
        Rem2->dump();

        //CachedRem2[SeqTy] = Rem2;

        NewV = Rem2;
	//}
      } else if (CInt1 && CInt2 && IsNegatedInt(CInt1->getValue(), CInt2->getValue()) ) {
        errs() << "Generated Version 2:\n";
        PHINode *PHI = nullptr;
        if (Header->getFirstNonPHI()) {
          IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
          PHI = PHIBuilder.CreatePHI(SeqTy,2);
        } else {
          PHI = Builder.CreatePHI(SeqTy,2);
        }
        //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
        //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
        CreatedCode.push_back(PHI);
        PHI->addIncoming(CInt1,PreHeader);

	auto Neg = Builder.CreateNeg(PHI);
        PHI->addIncoming(Neg,Header);

        if (auto *NegI = dyn_cast<Instruction>(Neg))
          CreatedCode.push_back(NegI);

	PHI->dump();
	Neg->dump();

	NewV = PHI;
      } else if (CInt1 && CInt2) {
        errs() << "Generated Version 3:\n";
        
        Value *Rem2 = nullptr;
        /*
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          Rem2 = CachedRem2[SeqTy];
	} else {
	
          Value *CastIndVar = IndVar;
          if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
            CastIndVar = CachedCastIndVar[SeqTy];
          } else {
            auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
            if (CIndVarI!=IndVar) {
              CreatedCode.push_back(CastIndVar);
              CachedCastIndVar[SeqTy] = CIndVarI;
              CastIndVar = CIndVarI;
            }
          }

	  Rem2 = CastIndVar;
	  //if (G.Root->size()>2){
	  if (N->size()>2){
            Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
            if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
              CreatedCode.push_back(Rem2I);
            Rem2->dump();
	  }

          CachedRem2[SeqTy] = Rem2;

	}
        */
          Value *CastIndVar = IndVar;
          auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
	  Rem2 = CastIndVar;
	  if (N->size()>2){
            Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
            if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
              CreatedCode.push_back(Rem2I);
            Rem2->dump();
	  }
	APInt Diff(CInt2->getValue());
	Diff -= CInt1->getValue();

	auto *Mul = Builder.CreateMul(Rem2, ConstantInt::get(SeqTy, Diff));
        if (auto *MulI = dyn_cast<Instruction>(Mul))
          CreatedCode.push_back(MulI);
        Mul->dump();

	auto *Add = Builder.CreateAdd(Mul, ASN->getFirst());
        if (auto *AddI = dyn_cast<Instruction>(Add))
          CreatedCode.push_back(AddI);

	Add->dump();

	NewV = Add;
        
      } else {
        errs() << "Generated Version 4:\n";
        /*
        IRBuilder<> HeaderBuilder(Header);
        if (Header->getTerminator()!=nullptr) {
          HeaderBuilder.SetInsertPoint(&*Header->getFirstInsertionPt());
        }
        */
        errs() << "Here0\n";
        IRBuilder<> *TmpBuilder = &Builder;
        if (ASN->getFirst() && ASN->getSecond()) {
        //Instruction *I1 = dyn_cast<Instruction>(ASN->getFirst());
        //Instruction *I2 = dyn_cast<Instruction>(ASN->getSecond());
        //if ( !isa<Instruction>(ASN->getFirst()) && !isa<Instruction>(ASN->getSecond()) ) {
        //  TmpBuilder = &HeaderBuilder;
        //}
        /* else if ( (I1==nullptr || AR.getAlignedBlock(I1)==nullptr) && (I2==nullptr || AR.getAlignedBlock(I2)==nullptr) ) {
          TmpBuilder = &HeaderBuilder;
        }*/
        }
        errs() << "Here1\n";
        
	//if (AltSeqCmp==nullptr) {

	Value *Rem2 = IndVar;
        /*
	if (CachedRem2.find(IndVar->getType())!=CachedRem2.end()) {
          Rem2 = CachedRem2[IndVar->getType()];
	} else {
	
	  if (N->size()>2){
        */
        errs() << "Here2\n";
            Rem2 = TmpBuilder->CreateURem(IndVar, ConstantInt::get(IndVar->getType(), 2));
            if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
              CreatedCode.push_back(Rem2I);
            Rem2->dump();
        /*
            CachedRem2[IndVar->getType()] = Rem2;
	  }

	}
        */
        errs() << "Here3\n";
        Value *Cond = TmpBuilder->CreateICmpEQ(Rem2, ConstantInt::get(IndVar->getType(), 0));
        if (auto *CondI = dyn_cast<Instruction>(Cond))
          CreatedCode.push_back(CondI);
        Cond->dump();
        
	  //AltSeqCmp = Cond;
	//}
        errs() << "Here4\n";
        //auto *Sel = TmpBuilder->CreateSelect(AltSeqCmp, ASN->getFirst(), ASN->getSecond());
        auto *Sel = TmpBuilder->CreateSelect(Cond, ASN->getFirst(), ASN->getSecond());
        if (auto *SelI = dyn_cast<Instruction>(Sel))
          CreatedCode.push_back(SelI);

        Sel->dump();
        
	NewV = Sel;
      }
      if (NewV) NodeToValue[N] = NewV;
      return NewV;
    }
    case NodeType::MISMATCH: {
      Value *NewV = generateMismatchingCode(N->getValues(), Builder);
#ifdef TEST_DEBUG
      errs() << "Gen: "; NewV->dump();
#endif
      NodeToValue[N] = NewV;
      return NewV;
    }
    default:
      assert(true && "Unknown node type!");
  }
}

void RegionCodeGenerator::setNodeOperands(Node *N) {
  //if (NodeToValue.find(N)!=NodeToValue.end()) return NodeToValue[N];

  switch(N->getNodeType()) {
    case NodeType::IDENTICAL: {
#ifdef TEST_DEBUG
      errs() << "Setting operands IDENTICAL\n";
#endif
      break;
    }
    case NodeType::MATCH: {
#ifdef TEST_DEBUG
      errs() << "Setting operands MATCH\n";
#endif

      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I) {
  
        Instruction *NewI = dyn_cast<Instruction>(NodeToValue[N]);
        IRBuilder<> Builder(NewI);

        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          NewI->setOperand(i,cloneGraph(N->getChild(i), Builder));
        }

      }
      break;
    }
    case NodeType::CONSTEXPR: {
#ifdef TEST_DEBUG
      errs() << "Setting operands CONSTEXPR\n";
#endif

      auto *I = dyn_cast<ConstantExpr>(N->getValue(0));
      if (I) {
        Instruction *NewI = dyn_cast<Instruction>(NodeToValue[N]);
        IRBuilder<> Builder(NewI);

        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          NewI->setOperand(i,cloneGraph(N->getChild(i), Builder));
        }

      }
      break;
    }
    case NodeType::GEPSEQ: {
      errs() << "setting operands GEPSEQ\n";
      auto *GN = (GEPSequenceNode*)N;
        
      Instruction *NewI = dyn_cast<Instruction>(NodeToValue[N]);
      IRBuilder<> Builder(NewI);

      Value *IndVarIdx = cloneGraph(GN->getChild(0), Builder);
      NewI->setOperand(NewI->getNumOperands()-1, IndVarIdx);

      break;
    }
    case NodeType::BINOP: {
#ifdef TEST_DEBUG
      errs() << "Generating BINOP\n";
#endif
      auto *BON = (BinOpSequenceNode*)N;
      Instruction *NewI = dyn_cast<Instruction>(NodeToValue[BON]);
      IRBuilder<> Builder(NewI);

      assert(BON->getNumChildren() && "Expected child with varying operands!");
      Value *Op0 = cloneGraph(BON->getChild(0), Builder);
      Value *Op1 = cloneGraph(BON->getChild(1), Builder);

#ifdef TEST_DEBUG
      errs() << "Closing BINOP\n";
#endif
      NewI->setOperand(0,Op0);
      NewI->setOperand(1,Op1);

#ifdef TEST_DEBUG
      //errs() << "Gen: "; NewI->dump();
#endif
      break;
    }
    /*
    case NodeType::RECURRENCE: {
#ifdef TEST_DEBUG
      errs() << "Setting operands RECURRENCE\n";
#endif
      auto *RN = (RecurrenceNode*)N;
          
      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(RN->getStartValue()->getType(),0);
      } else {
        PHI = Builder.CreatePHI(RN->getStartValue()->getType(),0);
      }
      CreatedCode.push_back(PHI);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NodeToValue[N] = PHI;

#ifdef TEST_DEBUG
      errs() << "Gen: "; PHI->dump();
#endif
      
      break;
    }
    case NodeType::REDUCTION: {
#ifdef TEST_DEBUG
      errs() << "Setting operands REDUCTION\n";
#endif
      auto *RN = (ReductionNode*)N;

      assert(RN->getNumChildren() && "Expected child with varying operands!");
      Value *Op = cloneGraph(RN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing REDUCTION\n";
#endif
      Instruction *NewI = RN->getBinaryOperator()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);
      NodeToValue[N] = NewI;

      for (unsigned i = 0; i<RN->size(); i++) {
        if (auto *I = RN->getValidInstruction(i)) {
	  Garbage.insert(I);
        }
      }

      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      } else {
        PHI = Builder.CreatePHI(NewI->getType(),2);
      }
      //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
      //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      CreatedCode.push_back(PHI);
      PHI->addIncoming(NewI,Header);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NewI->setOperand(0,PHI);
      NewI->setOperand(1,Op);

      Extracted[RN->getBinaryOperator()] = NewI;//PHI;
      generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
      errs() << "Gen: "; NewI->dump();
#endif
      break;
    }
    */
    case NodeType::INTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Setting operands INTSEQ\n";
#endif
      break;
    }
    /*
    case NodeType::ALTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Setting operands ALTSEQ\n";
#endif

      auto *ASN = (AlternatingSequenceNode*)N;

      auto *SeqTy = ASN->getFirst()->getType();

      errs() << "Values:\n";
      for (unsigned i = 0; i<N->size(); i++) N->getValue(i)->dump();

      Value *NewV = nullptr;

      auto IsNegatedInt = [](const APInt &V1, const APInt &V2) -> bool {
	APInt NegV1(V1);
        NegV1.negate(); //in place
	return V1.eq(V2);
      };

      auto *CInt1 = dyn_cast<ConstantInt>(ASN->getFirst());
      auto *CInt2 = dyn_cast<ConstantInt>(ASN->getSecond());
      if (CInt1 && CInt2 && CInt1->isZero() && CInt2->isOne()) {
        errs() << "Generated Version 1:\n";
	
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          NewV = CachedRem2[SeqTy];
	} else {

        Value *CastIndVar = IndVar;
        if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
          CastIndVar = CachedCastIndVar[SeqTy];
        } else {
          auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
          if (CIndVarI!=IndVar) {
            CreatedCode.push_back(CastIndVar);
            CachedCastIndVar[SeqTy] = CIndVarI;
            CastIndVar = CIndVarI;
          }
        }

        auto *Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
        if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
          CreatedCode.push_back(Rem2I);
        Rem2->dump();

        CachedRem2[SeqTy] = Rem2;

        NewV = Rem2;
	}
      } else if (CInt1 && CInt2 && IsNegatedInt(CInt1->getValue(), CInt2->getValue()) ) {
        errs() << "Generated Version 2:\n";
        PHINode *PHI = nullptr;
        if (Header->getFirstNonPHI()) {
          IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
          PHI = PHIBuilder.CreatePHI(SeqTy,2);
        } else {
          PHI = Builder.CreatePHI(SeqTy,2);
        }
        //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
        //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
        CreatedCode.push_back(PHI);
        PHI->addIncoming(CInt1,PreHeader);

	auto Neg = Builder.CreateNeg(PHI);
        PHI->addIncoming(Neg,Header);

        if (auto *NegI = dyn_cast<Instruction>(Neg))
          CreatedCode.push_back(NegI);

	PHI->dump();
	Neg->dump();

	NewV = PHI;
      } else if (CInt1 && CInt2) {
        errs() << "Generated Version 3:\n";

        Value *Rem2 = nullptr;
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          Rem2 = CachedRem2[SeqTy];
	} else {
	
          Value *CastIndVar = IndVar;
          if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
            CastIndVar = CachedCastIndVar[SeqTy];
          } else {
            auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
            if (CIndVarI!=IndVar) {
              CreatedCode.push_back(CastIndVar);
              CachedCastIndVar[SeqTy] = CIndVarI;
              CastIndVar = CIndVarI;
            }
          }

	  Rem2 = CastIndVar;
	  if (G.Root->size()>2){
            Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
            if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
              CreatedCode.push_back(Rem2I);
            Rem2->dump();
	  }


          CachedRem2[SeqTy] = Rem2;

	}
	APInt Diff(CInt2->getValue());
	Diff -= CInt1->getValue();

	auto *Mul = Builder.CreateMul(Rem2, ConstantInt::get(SeqTy, Diff));
        if (auto *MulI = dyn_cast<Instruction>(Mul))
          CreatedCode.push_back(MulI);
        Mul->dump();

	auto *Add = Builder.CreateAdd(Mul, ASN->getFirst());
        if (auto *AddI = dyn_cast<Instruction>(Add))
          CreatedCode.push_back(AddI);

	Add->dump();

	NewV = Add;
        
      } else {
        errs() << "Generated Version 4:\n";

	if (AltSeqCmp==nullptr) {

	Value *Rem2 = IndVar;

	if (CachedRem2.find(IndVar->getType())!=CachedRem2.end()) {
          Rem2 = CachedRem2[IndVar->getType()];
	} else {
	
	if (G.Root->size()>2){
          Rem2 = Builder.CreateURem(IndVar, ConstantInt::get(IndVar->getType(), 2));
          if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
            CreatedCode.push_back(Rem2I);
          Rem2->dump();

          CachedRem2[IndVar->getType()] = Rem2;
	}

	}

        Value *Cond = Builder.CreateICmpEQ(Rem2, ConstantInt::get(IndVar->getType(), 0));
        if (auto *CondI = dyn_cast<Instruction>(Cond))
          CreatedCode.push_back(CondI);
        Cond->dump();
        
	  AltSeqCmp = Cond;
	}
        auto *Sel = Builder.CreateSelect(AltSeqCmp, ASN->getFirst(), ASN->getSecond());
        if (auto *SelI = dyn_cast<Instruction>(Sel))
          CreatedCode.push_back(SelI);

        Sel->dump();

	NewV = Sel;
      }
      if (NewV) NodeToValue[N] = NewV;
      break;
    }
    */
    case NodeType::MISMATCH: {
      errs() << "Setting operands Mismatch\n";
      break;
    }
    default:
      assert(true && "Unknown node type!");
  }
}

void RegionCodeGenerator::generateNode(Node *N, IRBuilder<> &Builder) {
  switch(N->getNodeType()) {
    case NodeType::IDENTICAL: {
#ifdef TEST_DEBUG
      errs() << "Generating IDENTICAL\n";
#endif
      break;
    }
    case NodeType::MATCH: {
#ifdef TEST_DEBUG
      errs() << "Generating MATCH\n";
#endif

      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I) {
        Instruction *NewI = I->clone();
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,nullptr);
        }
        NodeToValue[N] = NewI;

#ifdef TEST_DEBUG
        errs() << "Operands done!\n";
#endif


        errs() << "generateNode 1\n";
        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        errs() << "generateNode 2\n";
        NewI->getAllMetadata(MDs);
        errs() << "generateNode 3\n";
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }
        errs() << "generateNode 4\n";

	/*
	if (!MatchAlignment) {
          if (auto *LI = dyn_cast<LoadInst>(NewI)) {
            LI->setAlignment(Align());
	  }
	  else if (auto *SI = dyn_cast<StoreInst>(NewI)) {
            SI->setAlignment(Align());
	  }
	}
        */

        Builder.Insert(NewI);
        //CreatedCode.push_back(NewI);
        errs() << "generateNode 5\n";

#ifdef TEST_DEBUG
        //errs() << "Generated: "; NewI->dump();
#endif

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
            //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
	    Garbage.insert(I);
	  }
	}
        
        errs() << "generateNode 6\n";
        generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
	//errs() << "Gen: "; NewI->dump();
#endif
      }
      break;
    }
    case NodeType::CONSTEXPR: {
#ifdef TEST_DEBUG
      errs() << "Generating CONSTEXPR\n";
#endif

      auto *I = dyn_cast<ConstantExpr>(N->getValue(0));
      if (I) {
        Instruction *NewI = I->getAsInstruction();
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,nullptr);
        }
        NodeToValue[N] = NewI;

	/*
        std::vector<Value*> Operands;
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          Operands.push_back(cloneGraph(N->getChild(i), Builder));
        }
	*/

        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        NewI->getAllMetadata(MDs);
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }

        Builder.Insert(NewI);
        //CreatedCode.push_back(NewI);

	/*
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }
	*/

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
	    Garbage.insert(I);
	  }
	}
        
        //generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
        //errs() << "Generated: "; NewI->dump();
#endif

      }
      break;
    }
    case NodeType::GEPSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating GEPSEQ\n";
#endif
      auto *GN = (GEPSequenceNode*)N;

      Value *Ptr = GN->getPointerOperand();
      if (Ptr==nullptr) {
        IRBuilder<> PreHeaderBuilder(PreHeader);
        auto *GEP = GN->getReference()->clone();
        auto *IdxTy = GN->getReference()->getOperand(GN->getReference()->getNumOperands()-1)->getType();
        auto *Zero = ConstantInt::get(IdxTy, 0);
        GEP->setOperand(GN->getReference()->getNumOperands()-1, Zero);
        PreHeaderBuilder.Insert(GEP);
        Ptr = GEP;
      }

      assert(GN->getNumChildren() && "Expected child with indices!");
      //Value *IndVarIdx = nullptr; //cloneGraph(GN->getChild(0), Builder);
      Value *PlaceHolder = ConstantInt::get(GN->getChild(0)->getType(), 0);
#ifdef TEST_DEBUG
      errs() << "Closing GEPSEQ\n";
#endif
      //auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getPointerOperand(), IndVarIdx));
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getReference()->getType(), Ptr, PlaceHolder));
      /*
      auto *GEP = GN->getReference()->clone();
      GEP->setOperand(0, GN->getPointerOperand());
      GEP->setOperand(GN->getReference()->getNumOperands()-1, IndVarIdx);
      Builder.Insert(GEP);
      */

      //CreatedCode.push_back(GEP);
      NodeToValue[N] = GEP;

      for (unsigned i = 0; i<GN->size(); i++) {
        if (auto *I = GN->getValidInstruction(i)) {
	  Garbage.insert(I);
          //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) {
          //  Garbage.push_back(I);
	  //}
        }
      }

      generateExtract(N, GEP, Builder);

#ifdef TEST_DEBUG
      errs() << "Gen: "; GEP->dump();
#endif
      break;
    }
    case NodeType::BINOP: {
#ifdef TEST_DEBUG
      errs() << "Generating BINOP\n";
#endif
      auto *BON = (BinOpSequenceNode*)N;

      assert(BON->getNumChildren() && "Expected child with varying operands!");
      //Value *Op0 = cloneGraph(BON->getChild(0), Builder);
      //Value *Op1 = cloneGraph(BON->getChild(1), Builder);

#ifdef TEST_DEBUG
      errs() << "Closing BINOP\n";
#endif
      Instruction *NewI = BON->getReference()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      NodeToValue[N] = NewI;
      //CreatedCode.push_back(NewI);

      for (unsigned i = 0; i<BON->size(); i++) {
        if (auto *I = BON->getValidInstruction(i)) {
	  Garbage.insert(I);
          //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) {
          //  Garbage.push_back(I);
	  //}
        }
      }

      /*
      NewI->setOperand(0,Op0);
      NewI->setOperand(1,Op1);
      */
      generateExtract(N, NewI, Builder);
#ifdef TEST_DEBUG
      //errs() << "Gen: "; NewI->dump();
#endif
      break;
    }
    /*
    case NodeType::RECURRENCE: {
#ifdef TEST_DEBUG
      errs() << "Generating RECURRENCE\n";
#endif
      auto *RN = (RecurrenceNode*)N;
          
      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(RN->getStartValue()->getType(),0);
      } else {
        PHI = Builder.CreatePHI(RN->getStartValue()->getType(),0);
      }
      CreatedCode.push_back(PHI);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NodeToValue[N] = PHI;

#ifdef TEST_DEBUG
      errs() << "Gen: "; PHI->dump();
#endif
      
      break;
    }
    case NodeType::REDUCTION: {
#ifdef TEST_DEBUG
      errs() << "Generating REDUCTION\n";
#endif
      auto *RN = (ReductionNode*)N;

      assert(RN->getNumChildren() && "Expected child with varying operands!");
      Value *Op = cloneGraph(RN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing REDUCTION\n";
#endif
      Instruction *NewI = RN->getBinaryOperator()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);
      NodeToValue[N] = NewI;

      for (unsigned i = 0; i<RN->size(); i++) {
        if (auto *I = RN->getValidInstruction(i)) {
	  Garbage.insert(I);
        }
      }

      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      } else {
        PHI = Builder.CreatePHI(NewI->getType(),2);
      }
      //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
      //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      CreatedCode.push_back(PHI);
      PHI->addIncoming(NewI,Header);
      PHI->addIncoming(RN->getStartValue(),PreHeader);
      NewI->setOperand(0,PHI);
      NewI->setOperand(1,Op);

      Extracted[RN->getBinaryOperator()] = NewI;//PHI;
      generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
      errs() << "Gen: "; NewI->dump();
#endif
      break;
    }
    */
    case NodeType::INTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating INTSEQ\n";
#endif

      Value *NewV = nullptr;

      auto *ISN = (IntSequenceNode*)N;
      auto *StartValue = dyn_cast<ConstantInt>(ISN->getStart());
      auto *StepValue = dyn_cast<ConstantInt>(ISN->getStep());
      Value *CastIndVar = IndVar;
      //if (CachedCastIndVar.find(StartValue->getType())!=CachedCastIndVar.end()) {
      //  CastIndVar = CachedCastIndVar[StartValue->getType()];
      //} else {
	auto *CIndVarI = Builder.CreateIntCast(IndVar, StartValue->getType(), false);
	if (CIndVarI!=IndVar) {
          //CreatedCode.push_back(CastIndVar);
          //CachedCastIndVar[StartValue->getType()] = CIndVarI;
          CastIndVar = CIndVarI;
	}
      //}
      if (StartValue->isZero() && StepValue->isOne()){
        NewV = CastIndVar;
      } else if (StepValue->isZero()){
        NewV = StartValue;
      } else {
	Value *Factor = CastIndVar;
	if (!StepValue->isOne()) {
          auto *Mul = Builder.CreateMul(CastIndVar, StepValue);
          //CreatedCode.push_back(Mul);
	  Factor = Mul;
	}
	auto *Add = Builder.CreateAdd(Factor, StartValue);
        //CreatedCode.push_back(Add);
	NewV = Add;
      }

#ifdef TEST_DEBUG
      errs() << "Gen: "; NewV->dump();
#endif
      NodeToValue[N] = NewV;
      break;
    }
    /*
    case NodeType::ALTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating ALTSEQ\n";
#endif

      auto *ASN = (AlternatingSequenceNode*)N;

      auto *SeqTy = ASN->getFirst()->getType();

      errs() << "Values:\n";
      for (unsigned i = 0; i<N->size(); i++) N->getValue(i)->dump();

      Value *NewV = nullptr;

      auto IsNegatedInt = [](const APInt &V1, const APInt &V2) -> bool {
	APInt NegV1(V1);
        NegV1.negate(); //in place
	return V1.eq(V2);
      };

      auto *CInt1 = dyn_cast<ConstantInt>(ASN->getFirst());
      auto *CInt2 = dyn_cast<ConstantInt>(ASN->getSecond());
      if (CInt1 && CInt2 && CInt1->isZero() && CInt2->isOne()) {
        errs() << "Generated Version 1:\n";
	
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          NewV = CachedRem2[SeqTy];
	} else {

        Value *CastIndVar = IndVar;
        if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
          CastIndVar = CachedCastIndVar[SeqTy];
        } else {
          auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
          if (CIndVarI!=IndVar) {
            CreatedCode.push_back(CastIndVar);
            CachedCastIndVar[SeqTy] = CIndVarI;
            CastIndVar = CIndVarI;
          }
        }

        auto *Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
        if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
          CreatedCode.push_back(Rem2I);
        Rem2->dump();

        CachedRem2[SeqTy] = Rem2;

        NewV = Rem2;
	}
      } else if (CInt1 && CInt2 && IsNegatedInt(CInt1->getValue(), CInt2->getValue()) ) {
        errs() << "Generated Version 2:\n";
        PHINode *PHI = nullptr;
        if (Header->getFirstNonPHI()) {
          IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
          PHI = PHIBuilder.CreatePHI(SeqTy,2);
        } else {
          PHI = Builder.CreatePHI(SeqTy,2);
        }
        //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
        //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
        CreatedCode.push_back(PHI);
        PHI->addIncoming(CInt1,PreHeader);

	auto Neg = Builder.CreateNeg(PHI);
        PHI->addIncoming(Neg,Header);

        if (auto *NegI = dyn_cast<Instruction>(Neg))
          CreatedCode.push_back(NegI);

	PHI->dump();
	Neg->dump();

	NewV = PHI;
      } else if (CInt1 && CInt2) {
        errs() << "Generated Version 3:\n";

        Value *Rem2 = nullptr;
	if (CachedRem2.find(SeqTy)!=CachedRem2.end()) {
          Rem2 = CachedRem2[SeqTy];
	} else {
	
          Value *CastIndVar = IndVar;
          if (CachedCastIndVar.find(SeqTy)!=CachedCastIndVar.end()) {
            CastIndVar = CachedCastIndVar[SeqTy];
          } else {
            auto *CIndVarI = Builder.CreateIntCast(IndVar, SeqTy, false);
            if (CIndVarI!=IndVar) {
              CreatedCode.push_back(CastIndVar);
              CachedCastIndVar[SeqTy] = CIndVarI;
              CastIndVar = CIndVarI;
            }
          }

	  Rem2 = CastIndVar;
	  if (G.Root->size()>2){
            Rem2 = Builder.CreateURem(CastIndVar, ConstantInt::get(SeqTy, 2));
            if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
              CreatedCode.push_back(Rem2I);
            Rem2->dump();
	  }


          CachedRem2[SeqTy] = Rem2;

	}
	APInt Diff(CInt2->getValue());
	Diff -= CInt1->getValue();

	auto *Mul = Builder.CreateMul(Rem2, ConstantInt::get(SeqTy, Diff));
        if (auto *MulI = dyn_cast<Instruction>(Mul))
          CreatedCode.push_back(MulI);
        Mul->dump();

	auto *Add = Builder.CreateAdd(Mul, ASN->getFirst());
        if (auto *AddI = dyn_cast<Instruction>(Add))
          CreatedCode.push_back(AddI);

	Add->dump();

	NewV = Add;
        
      } else {
        errs() << "Generated Version 4:\n";

	if (AltSeqCmp==nullptr) {

	Value *Rem2 = IndVar;

	if (CachedRem2.find(IndVar->getType())!=CachedRem2.end()) {
          Rem2 = CachedRem2[IndVar->getType()];
	} else {
	
	if (G.Root->size()>2){
          Rem2 = Builder.CreateURem(IndVar, ConstantInt::get(IndVar->getType(), 2));
          if (auto *Rem2I = dyn_cast<Instruction>(Rem2))
            CreatedCode.push_back(Rem2I);
          Rem2->dump();

          CachedRem2[IndVar->getType()] = Rem2;
	}

	}

        Value *Cond = Builder.CreateICmpEQ(Rem2, ConstantInt::get(IndVar->getType(), 0));
        if (auto *CondI = dyn_cast<Instruction>(Cond))
          CreatedCode.push_back(CondI);
        Cond->dump();
        
	  AltSeqCmp = Cond;
	}
        auto *Sel = Builder.CreateSelect(AltSeqCmp, ASN->getFirst(), ASN->getSecond());
        if (auto *SelI = dyn_cast<Instruction>(Sel))
          CreatedCode.push_back(SelI);

        Sel->dump();

	NewV = Sel;
      }
      if (NewV) NodeToValue[N] = NewV;
      break;
    }
    */
    case NodeType::MISMATCH: {
#ifdef TEST_DEBUG
      errs() << "Generating Mismatch\n";
#endif
      Value *NewV = generateMismatchingCode(N->getValues(), Builder);
#ifdef TEST_DEBUG
      errs() << "Gen: "; NewV->dump();
#endif
      NodeToValue[N] = NewV;
      break;
    }
    default:
      assert(true && "Unknown node type!");
  }
}

bool RegionCodeGenerator::generate(AlignedRegion &AR) {
  LLVMContext &Context = F.getContext();

  PreHeader = BasicBlock::Create(Context, "rolled.reg.pre", &F);

  Header = BasicBlock::Create(Context, "rolled.reg.loop", &F);

  IRBuilder<> Builder(Header);

  Type *IndVarTy = IntegerType::get(Context, 8);

  IndVar = Builder.CreatePHI(IndVarTy, 0);

  std::map<AlignedBlock*, BasicBlock*> ABToBlock;
  for (AlignedBlock *AB : AR.AlignedBlocks) {
    //if (AB->isExit()) continue;
    BasicBlock *BB = BasicBlock::Create(Context, "rolled.reg.bb", &F);
    Node *LN = AR.getLabelNode(AB);
    if (LN==nullptr) {
      errs() << "ERROR: could not find LabelNode\n";
    }
    NodeToValue[ LN ] = BB; 
    ABToBlock[AB] = BB;
    if (AB->isExit()) Exit = BB;
  }

  Latch = BasicBlock::Create(Context, "rolled.reg.latch", &F);
  LoopExit = BasicBlock::Create(Context, "rolled.reg.exit", &F);
  //NodeToValue[AR.ExitLabelNode] = Latch;

  //Exit = BasicBlock::Create(Context, "rolled.reg.exit", &F);


  for (AlignedBlock *AB : AR.AlignedBlocks) {
    errs() << "Generating code for AB\n";
    //if (AB->isExit()) {
	   // errs() << "skipping exit node\n";
	    //continue;
    //}

    //CachedCastIndVar.clear();
    //CachedRem2.clear();
    //AltSeqCmp = nullptr;

    BasicBlock *BB = ABToBlock[AB];
    errs() << "Generating code in: " << BB->getName().str() <<  "\n";
    for (Node *N : AB->ScheduledNodes) {
      IRBuilder<> Builder(BB);
      errs() << "generateNode: " << N->getString() <<"\n";
      generateNode(N, Builder);
      errs() << "generateNode: DONE\n";
    }
  }

  //F.dump();

  for (AlignedBlock *AB : AR.AlignedBlocks) {
    //if (AB->isExit()) continue;
    for (Node *N : AB->ScheduledNodes) {
      setNodeOperands(N);
    }
  }

  bool Invalid = false;
  errs() << "Updating PHI Node incoming values:\n";
  for (AlignedBlock *AB : AR.AlignedBlocks) {
    if (Invalid) break;

    for (Node *N : AB->ScheduledNodes) {
      if (N->getNodeType()==NodeType::PHI) {
        MatchingPHINode *PN = (MatchingPHINode*)N;
        if (NodeToValue[N]==nullptr) {
          errs() << "ERROR: PHI node not generated?\n";
          Invalid = true;
          break;
        }
        PHINode *NewPHI = dyn_cast<PHINode>(NodeToValue[N]);
        errs() << "Updating: "; NewPHI->dump();
        errs() << "Children: " << PN->getNumChildren() << "\n";
        for (unsigned i = 0; i<PN->getNumChildren(); i++) {
          errs() << "i: " << i << "\n";
          Node *Child = PN->getChild(i);
          Node *Label = PN->Labels[i];
          BasicBlock *InBB = dyn_cast<BasicBlock>( NodeToValue[Label] );
          IRBuilder<> Builder(InBB);
          if (InBB->getTerminator()) {
            Builder.SetInsertPoint(InBB->getTerminator());
          }
          Value *InV = cloneGraph(Child, Builder);
          NewPHI->addIncoming(InV, InBB);
        }
        errs() << "Updated PHI: ";
        NewPHI->dump();
      }

    }
  }

  //F.dump();

  Builder.SetInsertPoint(Latch);

  auto *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  //CreatedCode.push_back(Add);

  Value *Cond = nullptr;
  //if (AltSeqCmp && G.Root->size()==2) {
  //  Cond = AltSeqCmp;
  //} else {
    auto *CondI = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, AR.AlignedBlocks.size()+1));
    //CreatedCode.push_back(CondI);

    Cond = CondI;
  //}
  auto &DL = F.getParent()->getDataLayout();
  TargetTransformInfo TTI(DL);

  errs() << "Computing size of original code\n";
  unsigned CostOld = EstimateSize(Garbage, DL, &TTI);

  errs() << "Computing size of rolled code\n";
  std::set<BasicBlock *> CreatedBlocks;

  //CreatedBlocks.insert(PreHeader);
  CreatedBlocks.insert(Header);
  CreatedBlocks.insert(Latch);
  CreatedBlocks.insert(Exit);
  //CreatedBlocks.insert(LoopExit);

  for (auto &Pair : ABToBlock) {
    CreatedBlocks.insert(Pair.second);
  }
  
  unsigned CostNew = 0;
  for (BasicBlock *BB : CreatedBlocks) {
    errs() << "Size of BB: " << BB->getName().str() << "\n";
    BB->dump();
    size_t size = EstimateSize(BB, DL, &TTI);
    errs() << "size: " << size << "\n";
    CostNew += size;
  }

  CostNew += 4*EstimateSize(PreHeader,DL,&TTI); 
  CostNew += 3*EstimateSize(LoopExit,DL,&TTI); 

  bool Profitable = CostNew + 1 < CostOld;

  errs() << "Cost Original: " << CostOld << ", ";
  errs() << "Cost Rolled: " << CostNew << ", Region ";
  errs() << (Profitable?"Profitable":"Unprofitable") << "; " << F.getName() << " | ";
  errs() << "NumRegions: " << AR.getNumRegions() << " NumLabels: " << AR.getNumLabelNodes() << " GoodNodes: " << AR.getNumGoodNodes() << "\n";

  if (!Invalid && (AlwaysRoll || Profitable) ) {

    BasicBlock *FirstEntry = AR.EntryBlocks[0];
    BasicBlock *LastExit = AR.ExitBlocks[AR.ExitBlocks.size()-1];

    IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),PreHeader);
    IndVar->addIncoming(Add,Latch);

    auto *Br = Builder.CreateCondBr(Cond,Header,LoopExit);
    //CreatedCode.push_back(Br);

    Builder.SetInsertPoint(PreHeader);
    Builder.CreateBr(Header);

    Builder.SetInsertPoint(LoopExit);
    Builder.CreateBr(LastExit);


    Builder.SetInsertPoint(Header);

    Node *N = AR.find(AR.EntryBlocks);
    if (N==nullptr) {
      errs() << "ERROR: could not map to entry block\n";
    }
    BasicBlock *EntryBB = dyn_cast<BasicBlock>(NodeToValue[N]);
    Builder.CreateBr(EntryBB);

    errs() << "Before Updating Extracted Values\n";
   // F.dump();

    for (auto &Pair : Extracted) {
      Pair.first->replaceAllUsesWith(Pair.second);
    }


    errs() << "Erasing old instructions\n";
    /*
    for (Instruction *I : Garbage) {
      for (Use &OpU : I->operands()) {
        Value *OpV = OpU.get();
        OpU.set(nullptr);
      }
    }
    for (Instruction *I : Garbage) {
      I->replaceAllUsesWith(nullptr);
    }
    */
    for (Instruction *I : Garbage) {
      if (!I->getType()->isVoidTy())
         I->replaceAllUsesWith(UndefValue::get(I->getType()));
    }
    for (Instruction *I : Garbage) {
      I->eraseFromParent();
    }

    std::set<BasicBlock *> DeleteBlocks;
    for (AlignedBlock *AB : AR.AlignedBlocks) {
      //if (AB->isExit()) continue;
      for (BasicBlock *BB : AB->Blocks) {
	      DeleteBlocks.insert(BB);
      }
    }

    DeleteBlocks.erase(FirstEntry);
    DeleteBlocks.erase(LastExit);

    for (BasicBlock *BB : DeleteBlocks) BB->eraseFromParent();

    Builder.SetInsertPoint(FirstEntry);
    Builder.CreateBr(PreHeader);

    Builder.SetInsertPoint(Exit);
    //Builder.CreateBr(LastExit);
    Builder.CreateBr(Latch);

    //F.dump();
    
    return true;
  } else {
    std::set<BasicBlock *> DeleteBlocks;
 

    DeleteBlocks.insert(PreHeader);
    DeleteBlocks.insert(Header);
    DeleteBlocks.insert(Latch);
    DeleteBlocks.insert(Exit);
    DeleteBlocks.insert(LoopExit);

    for (auto &Pair : ABToBlock) {
      DeleteBlocks.insert(Pair.second);
    }

    std::set<Instruction*> DeleteInsts;
    for (BasicBlock *BB : DeleteBlocks) {
      for (Instruction &I : *BB) {
	DeleteInsts.insert(&I);
        for (Use &OpU : I.operands()) {
          Value *OpV = OpU.get();
          OpU.set(nullptr);
        }
      }
    }

    for (Instruction *I : DeleteInsts) {
      I->eraseFromParent();
    }
    for (BasicBlock *BB : DeleteBlocks) {
      BB->eraseFromParent();
    }

  }
  return false;
}

bool RegionRoller::run() {

  errs() << "RegionRoller at function: " << F.getName().str() << "\n";

  DominatorTree DT(F);
  PostDominatorTree PDT(F);
  //DT.recalculate(F);
  //PDT.recalculate(F);

  /// calculate regions
  DominanceFrontier DF;
  DF.analyze(DT);
  RegionInfo RI;
  RI.recalculate(F, &DT, &PDT, &DF);

  std::map<BasicBlock *, std::vector<BasicBlock *> > RegionsByEntry;
  std::vector<RegionEntry> OrderedRegions;

  for (BasicBlock &BB : F) {
    auto *R = RI.getRegionFor(&BB);
    if (R && R->getExit() && R->getExit()!=(&BB)) {
      auto &Regions = RegionsByEntry[R->getEntry()];
      if (std::find(Regions.begin(),Regions.end(),R->getExit())==Regions.end()) {
        RegionsByEntry[R->getEntry()].push_back(R->getExit());
        RegionEntry RE(R->getEntry(),R->getExit());
        OrderedRegions.push_back(RE);
      }
    }
  }

  for (RegionEntry &RE : OrderedRegions) {
    
    BasicBlock *EntryRef = RE.getEntry();
    BasicBlock *ExitRef = RE.getExit();
    

    errs() << "Reference Region: " << EntryRef->getName().str() << " -> " << ExitRef->getName().str() << "\n";

    unsigned CountRegions = 1;

    if (EntryRef->isEHPad()) {
      errs() << "Skipping entry block with a landing pad\n";
      continue;
    }
    if (isa<InvokeInst>(ExitRef->getTerminator())) {
      errs() << "Skipping exit block with an invoke instruction\n";
      continue;
    }

    AlignedRegion AR;
    initializeAlignedRegion(&AR, EntryRef, ExitRef);
    AR.EntryBlocks.push_back(EntryRef);
    AR.ExitBlocks.push_back(ExitRef);

    BasicBlock *LinkBlock = ExitRef;
    errs() << "Link: " << LinkBlock->getName().str() << "\n";
    while (LinkBlock) {
      BasicBlock *NextLB = nullptr;
      for (BasicBlock *ExitBB : RegionsByEntry[LinkBlock]) {
	//errs() << "Evaluating Region: " << LinkBlock->getName().str() << " => " << ExitBB->getName().str() << "\n";
	//auto *Region = RI.getCommonRegion(LinkBlock, ExitBB);
	//Region->dump();
	if (IsIsomorphic(EntryRef, ExitRef, LinkBlock, ExitBB, nullptr)) {
          errs() << "Found Isomorphic:" << LinkBlock->getName().str() << " => " << ExitBB->getName().str() << "\n";
          IsIsomorphic(EntryRef, ExitRef, LinkBlock, ExitBB, AR.AlignedBlocks[0]);
          AR.EntryBlocks.push_back(LinkBlock);
          AR.ExitBlocks.push_back(ExitBB);
	  NextLB = ExitBB;
	  CountRegions++;
	}
      }
      LinkBlock = NextLB;
    }

    errs() << "Final Isomorphic Graph: " << CountRegions << ", " << AR.AlignedBlocks.size() << "\n";
    for (auto *AB : AR.AlignedBlocks) {
      if (AB->isEntry()) errs() << "ENTRY ";
      if (AB->isExit()) errs() << "EXIT ";
      errs() << "Blocks:\n";

      for (BasicBlock *BB : AB->Blocks) {
        errs() << "   " << BB->getName().str() << "\n";
      }
    }
    errs() << "-----\n";

    bool Modified = false;
    if (CountRegions>1 && AR.AlignedBlocks.size() > 1) {
      errs() << "Let's do it\n";  
      if (AR.align(nullptr)) {
        errs() << " ISOMORPHIC REGIONS: " << CountRegions << " regions, " << AR.AlignedBlocks.size() << " blocks each!\n";

        RegionCodeGenerator RCG(F, AR);
        Modified = RCG.generate(AR);
        if (Modified) {
          F.dump();
          if (verifyFunction(F)) {
            errs() << "ERROR: BROKEN FUNCTION\n";
          }
        }
      }

      //TODO: temporary removed this section to improve capabilities... bring it back later
      
      if (!Modified) {
        for (int Skip = 1; Skip<(((int)AR.AlignedBlocks.size())-1); Skip++) {
          errs() << "Trying again for region remove last: " << Skip << "\n";
          AlignedRegion NewAR = AR.RemoveLast(Skip);
          if (NewAR.align(nullptr)) {
            errs() << " ISOMORPHIC REGIONS: " << CountRegions << " regions, " << NewAR.AlignedBlocks.size() << " blocks each!\n";
            RegionCodeGenerator RCG(F, NewAR);
            Modified = RCG.generate(NewAR);
          }

          if (!Modified) {
            for (int SkipF = 1; SkipF < (((int)NewAR.AlignedBlocks.size())-1); SkipF++) {
              errs() << "Trying again for region remove front: " << SkipF << "\n";
              AlignedRegion NewAR2 = NewAR.RemoveFirst(SkipF);
              if (NewAR2.align(nullptr)) {
                errs() << " ISOMORPHIC REGIONS: " << CountRegions << " regions, " << NewAR2.AlignedBlocks.size() << " blocks each!\n";
                RegionCodeGenerator RCG(F, NewAR2);
                Modified = RCG.generate(NewAR2);
              }
              NewAR2.releaseMemory();
              if (Modified) {
                errs() << "FOUND PROFITABLE WITH SKIP First" << SkipF << "\n";
                break;
              }
            }
          }

          NewAR.releaseMemory();
          if (Modified) {
            errs() << "FOUND PROFITABLE WITH SKIP " << Skip << "\n";
            break;
          }
        }
      }
      

    }

    AR.releaseMemory();
    if (Modified) return true;    

    //errs() << "Skipping smaller regions\n";
    //break; //TODO remove this later
  }

  return false;
}

bool AlignedRegion::validateAlignment() {
  std::set<BasicBlock *> Visited;
  Visited.insert(EntryBlocks[0]);
  Visited.insert(ExitBlocks[ExitBlocks.size()-1]);

  for (auto *AB : AlignedBlocks) {
    if (AB->isExit()) {
      //HERE22
      BasicBlock *BB = AB->Blocks[AB->Blocks.size()-1];
      for (PHINode &PHI : BB->phis()) {
        if (find(&PHI)==nullptr) {
          errs() << "\nERROR: Instruction not aligned in block: " << BB->getName().str() <<"\n";
	  PHI.dump();
          return false;
	      }

      }
    }else {
    for (BasicBlock *BB : AB->Blocks) {

      if (Visited.count(BB)) continue;
      Visited.insert(BB);

      for (Instruction &I : *BB) {
        if (find(&I)==nullptr) {
          errs() << "\nERROR: Instruction not aligned in block: " << BB->getName().str() <<"\n";
	  I.dump();
          return false;
	      }
      }
    }
    }
  }

  return true;
}

bool AlignedRegion::align(ScalarEvolution *SE) {

  unsigned CountMismatchings = 0;
  unsigned CountTotal = 0;

  if (EntryBlocks.size()<=1) return false;

  for (auto *AB : AlignedBlocks) {
    errs() << "Creating AlignedBlock Node\n";
    Node *N = createNode(AB->Blocks,nullptr,SE);
    addNode(N);
    ABToNode[AB] = N;
    if (AB->isExit()) {
     ExitLabelNode = N;
    }
  }

  //ExitLabelNode = createNode(ExitBlocks,nullptr,SE);
  //addNode(ExitLabelNode);

  for (auto *AB : AlignedBlocks) {
    errs() << "Aligning:\n";
    if (AB->isEntry()) {
	    errs() << "Entry Node\n";
    }
    if (AB->isExit()) {
	    errs() << "Exit Node\n";
	    //continue;
    }
    /*
    if (!AB->isEntry()) {
      for (BasicBlock *BB : AB->Blocks) {
        for (const PHINode &PHI : BB->phis()) {
          //TODO: unsupported PHINode in graph
          return false;
        }
      }
    }
    */
    std::vector<BasicBlock::reverse_iterator> Its;
    for (BasicBlock *BB : AB->Blocks) {
      //errs() << BB->getName().str() << "\n";
      BB->dump();
      Its.push_back(BB->rbegin());
    }

    if (AB->isExit()) { 
      bool ValidExit = true;
      
        errs() << "Processing Exit Blocks\n";

        for (unsigned i = 0; i<AB->Blocks.size(); i++) {
          Its[i]++;
        }

        for (unsigned i = 0; i<AB->Blocks.size()-1; i++) {
          while (Its[i]!=AB->Blocks[i]->rend() && find(&*Its[i])) {
            Its[i]++;
          }
          if (Its[i]==AB->Blocks[i]->rend()) {
            ValidExit = false;
            break;
          }
        }
        
        if (!ValidExit) continue;

        errs() << "Searching for instruction in last exit block\n";
        const int Last = AB->Blocks.size()-1;
        while (Its[Last]!=AB->Blocks[Last]->rend()) {
          std::vector<Instruction *> Vs;

          for (unsigned i = 0; i<AB->Blocks.size(); i++) {
            if (Its[i]!=AB->Blocks[i]->rend()) {
              Vs.push_back(&*Its[i]);
            }
          }

          //TODO skip instructions  in last exit block untill we find the matching point,
          //otherwise loop rolling is invalid

          Node *N = createNode(Vs,nullptr,SE);
          if (N) {
  	        errs() << "NodeTracker:Created: " << N->getString() << "\n";
            auto NodeTy = N->getNodeType();
            //delete N;

            if (NodeTy!=NodeType::MISMATCH) {
              errs() << "Found good match\n";
              break;
            }
          }

          errs() << "next instruction\n";
          Its[Last]++;

        }

        ValidExit = ValidExit && (Its[Last]!=AB->Blocks[Last]->rend());
        errs() << "Done searching: " << (Its[Last]!=AB->Blocks[Last]->rend()) << "\n";
        if (!ValidExit) continue;

        errs() << "Exit block alignment starts with:\n";
        for (unsigned i = 0; i<AB->Blocks.size(); i++) {
           (&*Its[i])->dump();
        }
    }

    while (true) {
      std::vector<Instruction *> Vs;

      for (unsigned i = 0; i<AB->Blocks.size(); i++) {
        if (Its[i]!=AB->Blocks[i]->rend()) {
          Vs.push_back(&*Its[i]);
        }
      }

      if (Vs.size()==0)
        break;

      Node *N = nullptr;
      if (Vs.size()==AB->Blocks.size())
        N = find(Vs);

      if (N) {
        errs() << "Node found!\n";
        printVs(Vs);
	      errs() << "NodeTracker:Found: " << N->getString() << "\n";

        for (unsigned i = 0; i<AB->Blocks.size(); i++) {
          if (Its[i]!=AB->Blocks[i]->rend()) {
            Its[i]++;
          }
        }
        AB->ScheduledNodes.push_back(N);

        continue;
      } else {
        errs() << "Analyzing\n";
        printVs(Vs);

        bool ValidGEPNode = true;
        GEPSequenceNode *RefGEPN = nullptr;
        for (unsigned i = 0 ;i<AB->Blocks.size(); i++) {
          if (Its[i]==AB->Blocks[i]->rend())
            continue;

          if (Node *N = find(&*Its[i])) {
            if (N->getNodeType()==NodeType::GEPSEQ) {
              GEPSequenceNode *GEPN = ((GEPSequenceNode*)N);
              if (RefGEPN==nullptr) RefGEPN = GEPN;
              else if (GEPN!=RefGEPN) {
                ValidGEPNode = false;
              }
            }
          }
        }
        if (RefGEPN!=nullptr) {
           errs() << "Found GEP node\n";
        }
        if (ValidGEPNode && RefGEPN!=nullptr) {
          for (unsigned i = 0 ;i<AB->Blocks.size(); i++) {
            if (Its[i]==AB->Blocks[i]->rend())
              continue;
            if (Node *N = find(&*Its[i])) {
              if (N->getNodeType()==NodeType::GEPSEQ) {
                Its[i]++;
              }
            }
          }
          AB->ScheduledNodes.push_back(RefGEPN);
          errs() << "NodeTracker:Found:GEP: " << RefGEPN->getString() << "\n";
          continue;
        }
      }

      if (Vs.size()<AB->Blocks.size()) {
        errs() << "Ended with incomplete group of instruction\n";
        printVs(Vs);
        break;
      }

      errs() << "Creating new root node\n";
      N = createNode(Vs,nullptr,SE);
      if (N) {
  	    errs() << "NodeTracker:Created: " << N->getString() << "\n";
        if (N->getNodeType()==NodeType::MISMATCH) {
           errs() << "Here X1\n";
           //delete N;
           errs() << "Here X2\n";
           break;
        }
        AB->ScheduledNodes.push_back(N);
        addNode(N);
        std::set<Node*> Visited;
        growGraph(N, SE, Visited);

        for (unsigned i = 0; i<AB->Blocks.size(); i++) {
          if (Its[i]!=AB->Blocks[i]->rend()) {
            Its[i]++;
          }
        }

      } else {
        errs() << "Could not create node\n";
        break;
      }
    }

    std::reverse(AB->ScheduledNodes.begin(), AB->ScheduledNodes.end());

    errs() << "Scheduled nodes:\n";
    for (Node *N : AB->ScheduledNodes) {
      errs() << N->getString() << "\n";
      CountTotal++;
      if (N->getNodeType()==NodeType::MISMATCH)
        CountMismatchings++;
    }
    
    for (Node *N : AB->ScheduledNodes) {
      errs() << NodeTypeString(N->getNodeType()) << ":CHECK: " << N->getString() << " ";
      bool InvalidType = false;
      switch(N->getNodeType()) {
      case NodeType::RECURRENCE:
      case NodeType::REDUCTION:
      case NodeType::ALTSEQ:
        InvalidType = true;
      }
      if (InvalidType) {
        errs() << "InvalidType\n";
        return false;
      }
        errs() << "children ";
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          errs() << "op[" << i << "]=" << NodeTypeString(N->getChild(i)->getNodeType()) << " ";
        }
        errs() << "\n";
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          bool InvalidType = false;
          switch(N->getChild(i)->getNodeType()) {
          case NodeType::RECURRENCE:
          case NodeType::REDUCTION:
          //case NodeType::PHI: // TODO: PHI not hanlded yet
            InvalidType = true;
          }
          if (InvalidType) {
            errs() << "InvalidType: operand "<< i << "\n";
            return false;
          }
        }
    }
    
    
  }
  errs() << "Done\n";

  errs() << "ALIGNMENT: " << CountMismatchings << "/" << CountTotal << ((CountMismatchings==0)?"(PROFITABLE)":"") <<  " "; 

  for (Node *N : Nodes) {
    if (N->getNodeType()==NodeType::MISMATCH) {
      for (unsigned i = 0; i<N->size(); i++) {
        if (Instruction *I = dyn_cast<Instruction>(N->getValue(i))) {
          if (contains(I)) {
            errs() << "ERROR: Mismatching input instruction is also a matching instruction\n";
            I->dump();
            return false;
          }
        }
      }
    }
  }
  return validateAlignment();
}

template<typename ValueT>
Node *AlignedRegion::find(std::vector<ValueT *> &Vs) {
  if (Vs.empty()) return nullptr;

  //errs() << "Searching for:\n";
  //for (auto *V : Vs) V->dump();

  if (NodeMap.find(Vs[0])==NodeMap.end()) return nullptr;
  
  std::unordered_set<Node*> Result(NodeMap[Vs[0]]);
  //errs() << "Set: " << Result.size() << "\n";
  for (unsigned i = 1; i<Vs.size(); i++) {
    //if (NodeMap.find(Vs[i])==NodeMap.end()) return nullptr;
    //for (Node *N : NodeMap[Vs[i]]) Result.erase(N);
    for (auto It = Result.begin(), E = Result.end(); It!=E; ) {
      Node *N = *It;
      It++;

      if (N->getValue(i)!=Vs[i]) Result.erase(N);
    }
    //errs() << "Set: " << Result.size() << "\n";
  }
  //errs() << "Set: " << Result.size() << "\n";
  
  if (Result.empty()) return nullptr;
  return *Result.begin();
}

Node *AlignedRegion::find(Instruction *I) {
  Node *Result = nullptr;
  auto It = NodeMap2.find(I);
  if (It==NodeMap2.end()) return nullptr;
  
  for (Node *N : It->second) {
    if (N!=nullptr && N->getNodeType()!=NodeType::MISMATCH) {
      if (Result!=nullptr) return nullptr;
      Result = N;
    }
  }

  return Result;
}

template<typename ValueT>
Node *AlignedRegion::buildRecurrenceNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {
//RecurrenceNode *RecurrenceNode::get(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {
  if (Vs.size()<=1) return nullptr;

  //if (!this->Root) return nullptr;
  //if (Vs.size()!=this->Root->size()) return nullptr;
  //if (this->Root->getNodeType()!=NodeType::MATCH) return nullptr;
  //if (!isa<CallBase>(this->Root->getValue(0))) return nullptr;

  if (NodeMap.find(Vs[1])==NodeMap.end()) return nullptr;
  if (!isa<Instruction>(Vs[1])) return nullptr;
  
  std::unordered_set<Node*> Result(NodeMap[Vs[1]]);
  for (unsigned i = 2; i<Vs.size(); i++) {
    for (auto It = Result.begin(), E = Result.end(); It!=E; ) {
      Node *N = *It;
      It++;
      if ((Value*)N->getValidInstruction(i-1)!=(Value*)Vs[i]) Result.erase(N);
    }
  }
  if (Result.size()!=1) return nullptr;

  Node *N = *Result.begin();

  if (Vs.size()!=N->size()) return nullptr;
  if (N->getNodeType()!=NodeType::MATCH) return nullptr;

  for (unsigned i = 1; i<Vs.size(); i++) {
    if (Vs[i]->getType()!=Vs[0]->getType()) return nullptr;
    if (Vs[i]!=N->getValue(i-1)) return nullptr;
  }

  errs() << "Found possible recurrence! Init: "; Vs[0]->dump();
  for (auto *V : N->getValues()) V->dump();

  //Inputs.insert(Vs[0]);

  auto *RN = new RecurrenceNode(Vs, Vs[0], BB, Parent);
  RN->pushChild(N);

  return RN;
}

template<typename ValueT>
Node *AlignedRegion::createNode(std::vector<ValueT*> Vs, Node *Parent, ScalarEvolution *SE) {
  
  errs() << "Creating Node\n";

  std::set<AlignedBlock*> FoundAlignedBlocks;

  printVs(Vs);
  
  bool IsEntryOrExit = true;

  //bool ValidBlock = true;
  bool HasInstruction = false;
  for (unsigned i = 0; i<Vs.size(); i++) {
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      //ValidBlock = ValidBlock && (I->getParent()==AB->Blocks[i]);
      HasInstruction = true;
      if (AlignedBlock *ABX = getAlignedBlock(I->getParent())) {
        FoundAlignedBlocks.insert(ABX);
        IsEntryOrExit = IsEntryOrExit && (ABX->isEntry() || ABX->isExit());
      }
    }
  }

  errs() << "Has instruction: " << HasInstruction << "\n";
  errs() << "Blocks Found: " << FoundAlignedBlocks.size() << "\n";

  bool ValidBlock = true; //!HasInstruction || FoundAlignedBlocks.size()==1 || IsEntryOrExit;

  if (ValidBlock && FoundAlignedBlocks.size()==1) {
    AlignedBlock *AB = *FoundAlignedBlocks.begin();
    for (unsigned i = 0; i<Vs.size(); i++) {
      if (auto *I = dyn_cast<Instruction>(Vs[i])) {
        ValidBlock = ValidBlock && (I->getParent()==AB->Blocks[i]);
      }
    }
  }

  errs() << "ValidBlock: " << ValidBlock << "\n";
  
  if (Node *N = IdenticalNode::get(Vs, nullptr, Parent)) {
    errs() << "All the Same\n";
    Inputs.insert(Vs[0]);
    return N;
  }

  if (Node *N = LabelNode::get(Vs, nullptr, Parent)) {
    errs() << "Creating LABEL Node\n";
    printVs(Vs);
    return N;
  }

  if (ValidBlock) {
    errs() << "Valid blocks: testing match\n";
    if (Node *N = MatchingNode::get(Vs, nullptr, Parent)) {
      errs() << "Matching\n";
      printVs(Vs);
      return N;
    }
  }
  if (GEPSequenceNode *N = GEPSequenceNode::get(Vs, nullptr, Parent)) {
    if (N->getPointerOperand()) 
      Inputs.insert(N->getPointerOperand());
    else Inputs.insert(N->getReference()->getPointerOperand());


    errs() << "GEP Seq\n";
    printVs(Vs);
    bool Valid = true;
    for (unsigned i = 0; i<Vs.size(); i++) {
      if (!Valid) break;

      Value *V = Vs[i];
      Instruction *I = dyn_cast<GetElementPtrInst>(Vs[i]);
      if (I!=nullptr && Inputs.count(I)==0) {
        if (getAlignedBlock(I->getParent(),i) == nullptr) {
          errs() << "Block not in alignment\n";
          Valid = false;
          break;
        }
      }
    }
    if (Valid)
      return N;
    else delete N;

    //return N;
  }

  if (Node *N = BinOpSequenceNode::get(Vs, nullptr, Parent, SE)) {
    errs() << "BinOp Seq\n";
    printVs(Vs);
    return N;
  }

  if (Node *N = buildRecurrenceNode(Vs, nullptr, Parent)) {
    errs() << "Recurrence\n";
    Inputs.insert(Vs[0]);
    return N;
  }

  if (Node *N = IntSequenceNode::get(Vs, nullptr, Parent)) {
    errs() << "Int Seq\n";
    printVs(Vs);
    return N;
  }

  if (AlternatingSequenceNode *N = AlternatingSequenceNode::get(Vs, nullptr, Parent)) {
    errs() << "Alt Seq\n";
    Inputs.insert(N->getFirst()); //Vs[0]);
    Inputs.insert(N->getSecond()); //Vs[1]);
    return N;
  }
  if (Node *N = ConstantExprNode::get(Vs, nullptr, Parent)) {
    errs() << "Const Expr\n";
    return N;
  }
  if (Node *N = MatchingPHINode::get(Vs, nullptr, Parent)) {
    errs() << "All PHI nodes\n";
    bool ValidPHI = true;
    for (unsigned i = 0; i<Vs.size(); i++) {
      if (!ValidPHI) break;

      Value *V = Vs[i];
      PHINode *PHI = dyn_cast<PHINode>(V);
      for (unsigned j = 0; j<PHI->getNumIncomingValues(); j++) {
        errs() << "Looking for block: " << PHI->getIncomingBlock(j)->getName() << "\n";
        if (getAlignedBlock(PHI->getIncomingBlock(j),i) == nullptr) {
          errs() << "Block not in alignment\n";
          ValidPHI = false;
          break;
        }
      }
    }
    if (ValidPHI)
      return N;
    else delete N;
  }

  errs() << "Mismatching\n";
  printVs(Vs);

  for (auto *V : Vs) Inputs.insert(V);
  return new MismatchingNode(Vs,nullptr,Parent);
  //return nullptr;
}


void AlignedRegion::growGraph(Node *N, ScalarEvolution *SE, std::set<Node*> &Visited) {
  if (Visited.find(N)!=Visited.end()) return;
  Visited.insert(N);

  auto growGraphNode = [&](auto &Vs, Node *N, std::set<Node*> &Visited) {
       Node *Child = find(Vs);
       if (Child==nullptr || Child->getNodeType()==NodeType::ALTSEQ) {
         Child = createNode(Vs, N, SE);
         this->addNode(Child);
         N->pushChild(Child);
         growGraph(Child, SE, Visited);
       } else {
         N->pushChild(Child);
       }
  };

  switch(N->getNodeType()) {
    case NodeType::MISMATCH: break;
    case NodeType::IDENTICAL: break;
    case NodeType::ALTSEQ: break;
    case NodeType::INTSEQ: break;
    case NodeType::RECURRENCE: break;
    case NodeType::MULTI: {
       MultiNode *MN = ((MultiNode*)N);
       for (size_t i = 0; i<MN->getNumGroups(); i++) {
         auto &Vs = MN->getGroup(i);
         growGraphNode(Vs, N, Visited);
       }
    } break;
    case NodeType::REDUCTION: {
       auto &Vs = ((ReductionNode*)N)->getOperands();
       growGraphNode(Vs, N, Visited);
    } break;
    case NodeType::GEPSEQ: {
       auto &Vs = ((GEPSequenceNode*)N)->getIndices();
       growGraphNode(Vs, N, Visited);
    } break;
    case NodeType::BINOP: {
       auto &V0 = ((BinOpSequenceNode*)N)->getLeftOperands();
       growGraphNode(V0, N, Visited);

       auto &V1 = ((BinOpSequenceNode*)N)->getRightOperands();
       growGraphNode(V1, N, Visited);
    } break;
    case NodeType::CONSTEXPR: {
      auto *CE = dyn_cast<ConstantExpr>(N->getValue(0));
      if (CE==nullptr) return;
      for (unsigned i = 0; i<CE->getNumOperands(); i++) {
        std::vector<Value*> Vs;
        for (unsigned j = 0; j<N->size(); j++) {
	  auto *IV = dyn_cast<ConstantExpr>(N->getValue(j));
	  assert(IV && "ConstantExprNode expecting valid constant expression!");
	  assert((i < IV->getNumOperands()) && "Invalid number of operands!");
          Vs.push_back( IV->getOperand(i) );
        }
        growGraphNode(Vs, N, Visited);
      }
    } break;
    case NodeType::MATCH: {
      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I==nullptr) return;
      for (unsigned i = 0; i<I->getNumOperands(); i++) {
        std::vector<Value*> Vs;
        for (unsigned j = 0; j<N->size(); j++) {
	  Instruction *IV = N->getValidInstruction(j);
	  assert(IV && "Matching node expecting valid instructions!");
	  assert((i < IV->getNumOperands()) && "Invalid number of operands!");
          Vs.push_back( IV->getOperand(i) );
        }
        growGraphNode(Vs, N, Visited);
      }
    } break;
    case NodeType::PHI: {
      PHINode *PHI = dyn_cast<PHINode>(N->getValue(0));
      if (PHI==nullptr) return;
      errs() << "Growing PHI Node:";
      PHI->dump();
      for (unsigned i = 0; i<PHI->getNumIncomingValues(); i++) {
        std::vector<Value*> Vs;

        errs() << "Block: " << i << " : " << PHI->getIncomingBlock(i)->getName() << "\n";
        AlignedBlock *InAB = getAlignedBlock(PHI->getIncomingBlock(i), 0);
        if (InAB==nullptr) {
          errs() << "ERROR: Null InAB\n";
        }
        errs() << "In-blocks:\n";
        for (BasicBlock *BB : InAB->Blocks) errs() << BB->getName() << "\n";

        for (unsigned j = 0; j<N->size(); j++) {
          errs() << "j: " << j << "\n";
          PHINode *PHIV = dyn_cast<PHINode>(N->getValue(j));
	  assert(PHIV && "Matching phi node expecting valid instructions!");
          errs() << "PHIV: "; PHIV->dump();
	  assert((i < PHIV->getNumIncomingValues()) && "Invalid number of operands!");
	  assert((PHIV->getBasicBlockIndex(InAB->Blocks[j])>=0) && "Invalid number of operands!");
          Vs.push_back( PHIV->getIncomingValueForBlock(InAB->Blocks[j]) );
        }
        errs() << "Growing towards:\n";
        for (Value *V : Vs) V->dump();
        Node *Child = find(Vs);
        if (Child==nullptr) {
          Child = createNode(Vs, N, SE);
          this->addNode(Child);
          N->pushChild(Child);
          growGraph(Child, SE, Visited);
        } else N->pushChild(Child);
        
        if (Node *LN = getLabelNode(InAB)) {
          ((MatchingPHINode*)N)->pushLabel(LN);
        } else errs() << "ERROR: No Label found\n";
      }
    } break;
    default: break;
  }
}
