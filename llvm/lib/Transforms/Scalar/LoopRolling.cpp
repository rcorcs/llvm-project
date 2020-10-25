
#include "llvm/Transforms/Scalar/LoopRolling.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/Verifier.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

#include <algorithm>    // std::find

#define DEBUG_TYPE "loop-rolling"

using namespace llvm;

static bool match(Value *V1, Value *V2) {
  Instruction *I1 = dyn_cast<Instruction>(V1);
  Instruction *I2 = dyn_cast<Instruction>(V2);

  if(I1 && I2 && I1->getOpcode()==I2->getOpcode()) {
    switch (I1->getOpcode()) {
    case Instruction::Call: {
      CallInst *CI1 = dyn_cast<CallInst>(I1);
      CallInst *CI2 = dyn_cast<CallInst>(I2);
      if (CI1->getCalledFunction()==nullptr || CI2->getCalledFunction()==nullptr) return false;
      if (CI1->getCalledFunction()!=CI2->getCalledFunction()) return false;
      if (CI1->getCalledFunction()->isVarArg()) return false;
      return true;
    }
    case Instruction::PHI:
      return false;
    default:
      return I1->getNumOperands()==I2->getNumOperands();
    }
  }

  return V1==V2;
}

template<typename ValueT>
static bool match(std::vector<ValueT*> Vs) {
  for (unsigned i = 1; i<Vs.size(); i++) {
    if (!match(Vs[0],Vs[i])) return false;
  }
  return true;
}

template<typename ValueT>
static size_t EstimateSize(std::vector<ValueT*> Code, TargetTransformInfo *TTI) {
  size_t size = 0;
  for (ValueT *V : Code) {
    if (auto *I = dyn_cast<Instruction>(V)) {
      switch(I->getOpcode()) {
      case Instruction::Alloca:
      case Instruction::PHI:
    case Instruction::ZExt:
    case Instruction::SExt:
    case Instruction::FPToUI:
    case Instruction::FPToSI:
    case Instruction::FPExt:
    case Instruction::PtrToInt:
    case Instruction::IntToPtr:
    case Instruction::SIToFP:
    case Instruction::UIToFP:
    case Instruction::Trunc:
    case Instruction::FPTrunc:
    case Instruction::BitCast:
        size += 0;
        break;
      default:
        size += TTI->getInstructionCost(
          I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      }
    } else if (auto *GV = dyn_cast<GlobalVariable>(V)) {
      size += 1;
      if (GV->hasInitializer()) {
	if (auto *ArrTy = dyn_cast<ArrayType>(GV->getInitializer()->getType())) {
	   size += ArrTy->getNumElements()-1;
	}
      }
    }
  }
  return size;
}

class Node {
private:
  Node *Parent;
  bool Match;
  std::vector<Value*> Values;
  std::vector<Node *> Children;
public:
  Node(Node *Parent=nullptr) : Parent(Parent), Match(true) {}

  template<typename ValueT>
  Node(std::vector<ValueT *> &Vs, Node *Parent=nullptr) : Parent(Parent), Match(true) {
    for(auto *V : Vs) pushValue(V);
  }

  Node *getParent() { return Parent; }

  size_t size() { return Values.size(); }

  void pushValue(Value *V) { Values.push_back(V); Match = Match && match(Values[0],V); }
  void pushChild( Node * N ) { Children.push_back(N); }

  Value *getValue(unsigned i) { return Values[i]; }
  std::vector<Value*> &getValues() { return Values; }

  Type *getType() { return Values[0]->getType(); }    

  const std::vector< Node * > &getChildren() { return Children; }
  size_t getNumChildren() { return Children.size(); }

  Node *getChild(unsigned i) { return Children[i]; }

  void clearChildren() { Children.clear(); }

  bool isMatching() { return Match; }
};

class Tree {
public:
  Node *Root;
  std::vector<Node*> Nodes;

  Tree(Node *Root) : Root(Root) {
    addNode(Root);
    growTree(Root);
  }
  
  template<typename ValueT>
  Tree(std::vector<ValueT*> &Vs) {
    Root = new Node(Vs);
    addNode(Root);
    growTree(Root);
  }

  size_t getWidth() {
    return Root->size();
  }

  void addNode(Node *N) {
    if (N) Nodes.push_back(N);
    if (Root==nullptr) Root = N;
  }

  Node *find(Value *V);

  void destroy() {
    for (Node *N : Nodes) delete N;
    Root = nullptr;
    Nodes.clear();
  }
private:
  void growTree(Node *N);
  Node *findRec(Value *V, Node *N);
};

class MultiTree {
public:
  std::vector<Tree *> Trees;
  size_t Width;

  MultiTree() : Width(0) {}

  bool addTree(Tree *T) {
    if (Width==0) Width = T->getWidth();
    else if (Width!=T->getWidth()) return false;
    Trees.push_back(T);
    return true;
  }

  std::vector<Tree*> &getTrees() { return Trees; }
};

Node *Tree::findRec(Value *V, Node *N) {
  if (std::find(N->getValues().begin(), N->getValues().end(), V)!=N->getValues().end())
    return N;
  for (Node *Child : N->getChildren()) {
    if (Node *Found = findRec(V, Child)) return Found;
  }
  return nullptr;
}

Node *Tree::find(Value *V) {
  return findRec(V, Root);
}

void Tree::growTree(Node *N) {
  Instruction *I = dyn_cast<Instruction>(N->getValue(0));
  if (I==nullptr) return;

  for (unsigned i = 0; i<I->getNumOperands(); i++) {
    std::vector<Value*> Vs;
    for (unsigned j = 0; j<N->size(); j++) {
      Vs.push_back( dyn_cast<Instruction>(N->getValue(j))->getOperand(i) );
    }
    Node *Child = new Node(Vs);
    this->addNode(Child);
    N->pushChild(Child);
    if (Child->isMatching()) {
      growTree(Child);
    }
  }
}

/// \returns True if all of the values in \p VL are constants (but not
/// globals/constant expressions).
template<typename ValueT>
static bool allConstant(const std::vector<ValueT *> VL) {
  // Constant expressions and globals can't be vectorized like normal integer/FP
  // constants.
  for (ValueT *i : VL)
    if (!isa<Constant>(i) || isa<ConstantExpr>(i) || isa<GlobalValue>(i))
      return false;
  return true;
}

template<typename ValueT>
static bool isConstantSequence(const std::vector<ValueT *> VL) {
  auto *CInt = dyn_cast<ConstantInt>(VL[0]);
  if (CInt==nullptr) return false;

  APInt Last = CInt->getValue();
  for(unsigned i = 1; i<VL.size(); i++) {
    auto *CInt = dyn_cast<ConstantInt>(VL[i]);
    APInt Val = CInt->getValue();
    Last++;
    if (Last!=Val) return false;
  }

  return true;
}

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, Instruction *PreHeaderPt, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode, ValueToValueMapTy &VMap);

static Value *generateMismatchingCode(std::vector<Value *> &VL, Value *IndVar, Instruction *PreHeaderPt, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode, ValueToValueMapTy &VMap) {
  Function *F = PreHeaderPt->getParent()->getParent();
  Module *M = F->getParent();
  LLVMContext &Context = F->getContext();

  if (allConstant(VL)) {
    if (isConstantSequence(VL)) {
      auto *BaseValue = dyn_cast<ConstantInt>(VL[0]);
      auto *CastIndVar = Builder.CreateIntCast(IndVar, BaseValue->getType(), false);
      CreatedCode.push_back(CastIndVar);
      if (BaseValue->isZero()) return CastIndVar;
      else {
	auto *Add = Builder.CreateAdd(CastIndVar, BaseValue);
        CreatedCode.push_back(Add);
	return Add;
      }
    } else {
	    
      auto *ArrTy = ArrayType::get(VL[0]->getType(), VL.size());
      GlobalVariable* GArray = dyn_cast<GlobalVariable>(M->getOrInsertGlobal("vals", ArrTy));
      CreatedCode.push_back(GArray);
      GArray->setLinkage(GlobalValue::LinkageTypes::PrivateLinkage);
      //GArray->setAlignment(4);

      SmallVector<Constant*,8> Consts;
      for (auto *V : VL) Consts.push_back(dyn_cast<Constant>(V));
      auto *ConstArray = ConstantArray::get(ArrTy, Consts);
      GArray->setInitializer(ConstArray);
 
      SmallVector<Value*,8> Indices;
      Type *IndVarTy = IntegerType::get(Context, 64);
      Indices.push_back(ConstantInt::get(IndVarTy, 0));
      Indices.push_back(IndVar);
      auto *GEP = Builder.CreateGEP(GArray, Indices);
      CreatedCode.push_back(GEP);

      auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
      CreatedCode.push_back(Load);
      return Load;
    }
  } else {
    if (Value *V = generateGEPSequence(VL, IndVar, PreHeaderPt, Builder, Garbage, CreatedCode, VMap)) {
      return V;
    }
    BasicBlock &Entry = F->getEntryBlock();
    IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

    Type *IndVarTy = IntegerType::get(Context, 64);
    Value *ArrPtr = ArrBuilder.CreateAlloca(VL[0]->getType(), ConstantInt::get(IndVarTy, VL.size()));
    CreatedCode.push_back(ArrPtr);

    ArrBuilder.SetInsertPoint(PreHeaderPt);
    for (unsigned i = 0; i<VL.size(); i++) {
      auto *GEP = ArrBuilder.CreateGEP(ArrPtr, ConstantInt::get(IndVarTy, i));
      CreatedCode.push_back(GEP);
      auto *Store = ArrBuilder.CreateStore(VL[i], GEP);
      CreatedCode.push_back(Store);
    }

    auto *GEP = Builder.CreateGEP(ArrPtr, IndVar);
    CreatedCode.push_back(GEP);
    auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
    CreatedCode.push_back(Load);
    return Load;
  }
  return VL[0];
}

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, Instruction *PreHeaderPt, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode, ValueToValueMapTy &VMap) {
  if (!isa<PointerType>(VL[0]->getType())) return nullptr;
  auto *Ptr = getUnderlyingObject(VL[0]);
  for (unsigned i = 1; i<VL.size(); i++)
    if (Ptr != getUnderlyingObject(VL[i])) return nullptr;

  if (Ptr==VL[0]) {
    std::vector<Value*> Indices;
    Indices.push_back(nullptr);
    
    Type *Ty = nullptr;
    for (unsigned i = 1; i<VL.size(); i++) {
      if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
        if (GEP->getPointerOperand()!=Ptr) return nullptr;
        if (GEP->getNumIndices()!=1) return nullptr;
	Value *Idx = GEP->getOperand(1);
	Indices.push_back(Idx);
	if (Ty && Ty!=Idx->getType()) return nullptr;
	if (Ty==nullptr) Ty = Idx->getType();
      } else return nullptr;
    }
    
    if (Ty==nullptr || !isa<IntegerType>(Ty)) return nullptr;

    Indices[0] = ConstantInt::get(Ty, 0);

    for (unsigned i = 1; i<VL.size(); i++) {
      if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
	if (std::find(Garbage.begin(), Garbage.end(), GEP)==Garbage.end())
	  Garbage.push_back(GEP);
      }
    }

    Value *IndVarIdx = generateMismatchingCode(Indices, IndVar, PreHeaderPt, Builder, Garbage, CreatedCode, VMap);
    auto *GEP = Builder.CreateGEP(Ptr, IndVarIdx);
    CreatedCode.push_back(GEP);
    return GEP;
  }
  return nullptr;
}

static void generateExtract(Node *N, Instruction * NewI, Tree &T, Value *IndVar, IRBuilder<> &Builder,  std::map<Instruction*,Instruction*> &Extracted, BasicBlock *Exit, std::vector<Value *> &CreatedCode) {
  Function *F = Exit->getParent();
  LLVMContext &Context = F->getContext();

  std::set<unsigned> NeedExtract;

  for (unsigned i = 0; i<N->size(); i++) {
    Value *V = N->getValue(i);
    if (!isa<Instruction>(V)) continue;

    for (auto *U : V->users()) {
      if (T.find(U)==nullptr) {
        NeedExtract.insert(i);
	break;
      }
    }
  }

  if (NeedExtract.empty()) return;
  errs() << "Extracting: "; NewI->dump();

  BasicBlock &Entry = F->getEntryBlock();
  IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

  Type *IndVarTy = IntegerType::get(Context, 64);
  Value *ArrPtr = ArrBuilder.CreateAlloca(NewI->getType(), ConstantInt::get(IndVarTy, N->size()));
  CreatedCode.push_back(ArrPtr);

  auto *GEP = Builder.CreateGEP(ArrPtr, IndVar);
  CreatedCode.push_back(GEP);
  auto *Store = Builder.CreateStore(NewI, GEP);
  CreatedCode.push_back(Store);

  IRBuilder<> ExitBuilder(Exit);
  for (unsigned i : NeedExtract) {
    Instruction *I = dyn_cast<Instruction>(N->getValue(i));
    auto *GEP = ExitBuilder.CreateGEP(ArrPtr, ConstantInt::get(IndVarTy, i));
    CreatedCode.push_back(GEP);
    auto *Load = ExitBuilder.CreateLoad(GEP);
    CreatedCode.push_back(Load);
    Extracted[I] = Load;
  }

}


static Value *cloneTree(Node *N, Tree &T, Value *IndVar, Instruction *PreHeaderPt, BasicBlock *Exit, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode, std::map<Instruction*,Instruction*> &Extracted, ValueToValueMapTy &VMap) {
  if (N->isMatching()) {
    std::vector<Value*> Operands;
    for (unsigned i = 0; i<N->getNumChildren(); i++) {
      Operands.push_back(cloneTree(N->getChild(i), T, IndVar, PreHeaderPt, Exit, Builder, Garbage, CreatedCode, Extracted, VMap));
    }

    Instruction *I = dyn_cast<Instruction>(N->getValue(0));
    if (I) {
      Instruction *NewI = I->clone();
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);

      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,Operands[i]);
      }
      
      for (auto *V : N->getValues()) {
	//errs() << "Deleting: "; V->dump();
        auto *I = dyn_cast<Instruction>(V);
	if (I && std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
      }

      generateExtract(N, NewI, T, IndVar, Builder, Extracted, Exit, CreatedCode);

      return NewI;
    } else return N->getValue(0);
  } else {
    return generateMismatchingCode(N->getValues(), IndVar, PreHeaderPt, Builder, Garbage, CreatedCode, VMap);
  }
}

static Instruction *SchedulingPoint(Tree &T, BasicBlock &BB) {
  Instruction *LastInst = dyn_cast<Instruction>(T.Root->getValues()[T.getWidth()-1]);
  
  auto SplitPt = BB.begin();
  while ( &*SplitPt!=LastInst ) SplitPt++;
  SplitPt++;

  return &*SplitPt;
}

static void CodeGen(Tree &T, BasicBlock &BB) {
  LLVMContext &Context = BB.getParent()->getContext();

  Instruction *InstSplitPt = SchedulingPoint(T, BB);

  BasicBlock *Header = BasicBlock::Create(Context, "reroll.header", BB.getParent());

  IRBuilder<> Builder(Header);

  Type *IndVarTy = IntegerType::get(Context, 64);

  PHINode *IndVar = Builder.CreatePHI(IndVarTy, 0);
  IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),&BB);

  ValueToValueMapTy VMap;
  std::vector<Value *> CreatedCode;
  std::vector<Instruction *> Garbage;

  std::map<Instruction*,Instruction*> Extracted;
  BasicBlock *Exit = BasicBlock::Create(Context, "reroll.exit", BB.getParent());

  cloneTree(T.Root, T, IndVar, InstSplitPt, Exit, Builder, Garbage, CreatedCode, Extracted, VMap);

  TargetTransformInfo TTI(BB.getParent()->getParent()->getDataLayout());
  size_t SizeOriginal = EstimateSize(Garbage,&TTI);
  size_t SizeModified = EstimateSize(CreatedCode,&TTI) + 3;

  bool Profitable = SizeOriginal >= SizeModified;

  errs() << "Gains: " << SizeOriginal << " - " << SizeModified << " = " << ( ((int)SizeOriginal) - ((int)SizeModified) ) << "\n";
  if (Profitable) {
    errs() << "Profitable: finishing code generation\n";

    Value *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
    IndVar->addIncoming(Add,Header);

    Value *Cond = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, T.getWidth()));
    Builder.CreateCondBr(Cond,Header,Exit);

    auto SplitPt = BB.begin();
    while ( &*SplitPt!=InstSplitPt ) SplitPt++;

    Builder.SetInsertPoint(Exit);
    while (SplitPt!=BB.end()) {
      Instruction *I = &*SplitPt;
      SplitPt++;

      I->removeFromParent();
      Builder.Insert(I);
    }
    
    Builder.SetInsertPoint(&BB);
    Builder.CreateBr(Header);

    for (auto &Pair : Extracted) {
      Pair.first->replaceAllUsesWith(Pair.second);
    }

    for (auto It = Garbage.rbegin(), E = Garbage.rend(); It!=E; It++) {
      (*It)->eraseFromParent();
    }

  } else {
    errs() << "Unprofitable: deleting generated code\n";
    for (auto It = CreatedCode.rbegin(), E = CreatedCode.rend(); It!=E; It++) {
      Value *V = *It;
      if (auto *I = dyn_cast<Instruction>(V)) {
        I->dropAllReferences();
        I->eraseFromParent();
      } else if (auto *GV = dyn_cast<GlobalVariable>(V)) {
        GV->dropAllReferences();
        GV->eraseFromParent();
      }
    }
    Header->eraseFromParent();
    Exit->eraseFromParent();
  }
}

static void buildTree(std::vector<Instruction*> Seeds) {
  for (auto *V : Seeds) V->dump();
  Tree T(Seeds);

  BasicBlock *BB = Seeds[0]->getParent();
  CodeGen(T, *BB);

  T.destroy();
}

void LoopRolling::collectSeedInstructions(BasicBlock &BB) {
  // Initialize the collections. We will make a single pass over the block.
  Seeds.clear();

  // Visit the store and getelementptr instructions in BB and organize them in
  // Stores and GEPs according to the underlying objects of their pointer
  // operands.
  for (Instruction &I : BB) {
    // Ignore store instructions that are volatile or have a pointer operand
    // that doesn't point to a scalar type.
    if (auto *SI = dyn_cast<StoreInst>(&I)) {
      //if (!SI->isSimple())
      //  continue;
      //if (!isValidElementType(SI->getValueOperand()->getType()))
      //  continue;
      auto &Stores = Seeds[getUnderlyingObject(SI->getPointerOperand())];
      
      bool Valid = true;
      if (Stores.size()) {
	Valid = false;
        auto *RefSI = dyn_cast<StoreInst>(Stores[0]);
	if (RefSI && RefSI->getValueOperand()->getType()==SI->getValueOperand()->getType())
	  Valid = true;
      }
      if (Valid) Stores.push_back(SI);
    }
    else if (auto *CI = dyn_cast<CallInst>(&I)) {
      //if (CI->getNumUses()>0) continue;

      Function *Callee = CI->getCalledFunction();
      if (Callee && !Callee->isVarArg()) {
        Seeds[Callee].push_back(CI);
      }
    }
    // Ignore getelementptr instructions that have more than one index, a
    // constant index, or a pointer operand that doesn't point to a scalar
    // type.
    /*
    else if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) {
      auto Idx = GEP->idx_begin()->get();
      if (GEP->getNumIndices() > 1 || isa<Constant>(Idx))
        continue;
      if (!isValidElementType(Idx->getType()))
        continue;
      if (GEP->getType()->isVectorTy())
        continue;
      GEPs[GEP->getPointerOperand()].push_back(GEP);
    }
    */
  }
}

bool LoopRolling::runImpl(Function &F) {
  for (BasicBlock &BB : F) {
    collectSeedInstructions(BB);
    for (auto &Pair : Seeds) {
      if (Pair.second.size()>1)
        buildTree(Pair.second); 
    }
  }
  return true;
}

PreservedAnalyses LoopRolling::run(Function &F, FunctionAnalysisManager &AM) {
  bool Changed = runImpl(F);
  if (!Changed)
    return PreservedAnalyses::all();
  PreservedAnalyses PA;
  //PA.preserve<DominatorTreeAnalysis>();
  //PA.preserve<GlobalsAA>();
  //PA.preserve<TargetLibraryAnalysis>();
  //if (MSSA)
  //  PA.preserve<MemorySSAAnalysis>();
  //if (LI)
  //  PA.preserve<LoopAnalysis>();
  return PA;
}

class LoopRollingLegacyPass : public FunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid

  explicit LoopRollingLegacyPass() : FunctionPass(ID) {
    initializeLoopRollingLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override {
    if (skipFunction(F))
      return false;
    return Impl.runImpl(F);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
  }

private:
  LoopRolling Impl;

};

char LoopRollingLegacyPass::ID = 0;

INITIALIZE_PASS(LoopRollingLegacyPass, "loop-rolling", "Loop rerolling over straight-line code", false, false)

// The public interface to this file...
FunctionPass *llvm::createLoopRollingPass() {
  return new LoopRollingLegacyPass();
}



