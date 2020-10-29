
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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

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
      case Instruction::Call:
	size += 1 + dyn_cast<CallBase>(I)->getNumArgOperands();
	break;
      case Instruction::GetElementPtr:
	size += 1;
	break;
      case Instruction::Load:
      case Instruction::Store:
	size += 1;
	break;
      default:
        size += 1;
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

/*
template<typename ValueT>
static size_t EstimateSize(std::vector<ValueT*> Code, TargetTransformInfo *TTI) {
  size_t size = 0;
  for (ValueT *V : Code) {
    if (V==nullptr) continue;
    V->dump();
    if (auto *I = dyn_cast<Instruction>(V)) {
       unsigned Cost = TTI->getInstructionCost(
          I, TargetTransformInfo::TargetCostKind::TCK_CodeSize);
      errs() << "Cost: " << Cost << "\n";
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
*/

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
  std::map<Value*, Node*> NodeMap;

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
    if (N) {
      if (std::find(Nodes.begin(), Nodes.end(), N)==Nodes.end()) {
        Nodes.push_back(N);
        for (auto *V : N->getValues())
          NodeMap[V] = N;
      }
    }
    if (Root==nullptr) Root = N;
  }

  Node *find(Value *V);
  Node *find(std::vector<Value *> Vs);

  void destroy() {
    for (Node *N : Nodes) delete N;
    Root = nullptr;
    Nodes.clear();
  }
private:
  void growTree(Node *N);
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


class SeedGroups {
public:
  std::map<Value *, std::vector<Instruction *> > Stores;
  std::map<Value *, std::vector<Instruction *> > Calls;

  void clear() {
    Stores.clear();
    Calls.clear();
  }

  void remove(Instruction *I) {
    for (auto &Pair : Stores) {
      Pair.second.erase(std::remove(Pair.second.begin(), Pair.second.end(), I), Pair.second.end());
    }
    for (auto &Pair : Calls) {
      Pair.second.erase(std::remove(Pair.second.begin(), Pair.second.end(), I), Pair.second.end());
    }
  }
};

class CodeGenerator {
public:
  CodeGenerator(Function &F, BasicBlock &BB, Tree &T) : F(F), BB(BB), T(T) {}

  bool generate(SeedGroups &Seeds);

private:
  Function &F;
  BasicBlock &BB;
  Tree &T;
  PHINode *IndVar;
  BasicBlock *PreHeader;
  BasicBlock *Header;
  BasicBlock *Exit;

  std::map<Node*, Value *> NodeMap;
  std::vector<Instruction *> Garbage;
  std::vector<Value *> CreatedCode;
  std::map<Instruction *, Instruction *> Extracted;

  Value *cloneTree(Node *N, IRBuilder<> &Builder);
  void generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder);
};

class LoopRoller {
public:
  LoopRoller(Function &F) : F(F) {}

  bool run();
private:
  Function &F;

  void collectSeedInstructions(BasicBlock &BB);
  //void codeGeneration(Tree &T, BasicBlock &BB);

  SeedGroups Seeds;
};


Node *Tree::find(Value *V) {
  if (NodeMap.find(V)!=NodeMap.end()) return NodeMap[V];
  return nullptr;
}

Node *Tree::find(std::vector<Value *> Vs) {
  if (Vs.empty()) return nullptr;

  Node *N = find(Vs[0]);
  for (unsigned i = 1; i<Vs.size(); i++) {
    if (find(Vs[i])!=N) return nullptr;
  }
  return N;
}


void Tree::growTree(Node *N) {
  Instruction *I = dyn_cast<Instruction>(N->getValue(0));
  if (I==nullptr) return;

  for (unsigned i = 0; i<I->getNumOperands(); i++) {
    std::vector<Value*> Vs;
    for (unsigned j = 0; j<N->size(); j++) {
      Vs.push_back( dyn_cast<Instruction>(N->getValue(j))->getOperand(i) );
    }
    Node *Child = find(Vs);
    if (Child==nullptr) Child = new Node(Vs);

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

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode);

static Value *generateBinOpSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode);

static Value *generateMismatchingCode(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode) {
  Function *F = PreHeader->getParent();
  Module *M = F->getParent();
  LLVMContext &Context = F->getContext();

  //errs() << "Mismatch:\n";
  //for (Value *V : VL) V->dump();

  bool AllSame = true;
  for (unsigned i = 0; i<VL.size(); i++) {
    AllSame = AllSame && VL[i]==VL[0];
  }
  if (AllSame) return VL[0];

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

      SmallVector<Constant*,8> Consts;
      for (auto *V : VL) Consts.push_back(dyn_cast<Constant>(V));
      auto *ConstArray = ConstantArray::get(ArrTy, Consts);

      GlobalVariable *GArray = new GlobalVariable(*M, ArrTy,true,GlobalValue::LinkageTypes::PrivateLinkage,ConstArray);
      CreatedCode.push_back(GArray);
      //GArray->setInitializer(ConstArray);
 
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
    if (Value *V = generateGEPSequence(VL, IndVar, PreHeader, Builder, Garbage, CreatedCode)) {
      return V;
    }
    if (Value *V = generateBinOpSequence(VL, IndVar, PreHeader, Builder, Garbage, CreatedCode)) {
      return V;
    }
    //BasicBlock &Entry = F->getEntryBlock();
    //IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());
    IRBuilder<> ArrBuilder(PreHeader);

    Type *IndVarTy = IntegerType::get(Context, 64);
    Value *ArrPtr = ArrBuilder.CreateAlloca(VL[0]->getType(), ConstantInt::get(IndVarTy, VL.size()));
    CreatedCode.push_back(ArrPtr);

    //ArrBuilder.SetInsertPoint(PreHeaderPt);
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

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode) {
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

    Value *IndVarIdx = generateMismatchingCode(Indices, IndVar, PreHeader, Builder, Garbage, CreatedCode);
    auto *GEP = Builder.CreateGEP(Ptr, IndVarIdx);
    CreatedCode.push_back(GEP);
    return GEP;
  } //TODO: handle reversed loops
  return nullptr;
}


static Value *generateBinOpSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage, std::vector<Value *> &CreatedCode) {

  if (!isa<IntegerType>(VL[0]->getType())) return nullptr;
  Type *Ty = VL[0]->getType();

  std::vector<Value*> Operands;
  Operands.push_back(nullptr);

  unsigned Opcode = 0;
  for (unsigned i = 1; i<VL.size(); i++) {
    auto *BinOp = dyn_cast<BinaryOperator>(VL[i]);
    if (BinOp==nullptr) return nullptr;

    if (Opcode) {
      if (Opcode!=BinOp->getOpcode()) return nullptr;
    } else Opcode = BinOp->getOpcode();

    Value *Op = nullptr;
    if (BinOp->isCommutative() && BinOp->getOperand(1)==VL[0])
      Op = BinOp->getOperand(0);
    else if (BinOp->getOperand(0)==VL[0])
      Op = BinOp->getOperand(1);

    if (Op==nullptr) return nullptr;

    Operands.push_back(Op);
  }

  switch(Opcode) {
  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Or:
  case Instruction::Xor:
    Operands[0] = ConstantInt::get(Ty, 0);
    break;
  case Instruction::Mul:
  case Instruction::UDiv:
  case Instruction::SDiv:
  case Instruction::And:
    Operands[0] = ConstantInt::get(Ty, 1);
    break;
  default:
    return nullptr;
  }

  Instruction *BinOpRef = nullptr;
  for (unsigned i = 1; i<VL.size(); i++) {
    if (auto *BinOp = dyn_cast<BinaryOperator>(VL[i])) {
      BinOpRef = BinOp;
      if (std::find(Garbage.begin(), Garbage.end(), BinOp)==Garbage.end())
        Garbage.push_back(BinOp);
    }
  }

  Value *Op = generateMismatchingCode(Operands, IndVar, PreHeader, Builder, Garbage, CreatedCode);
  Instruction *NewI = BinOpRef->clone();
  Builder.Insert(NewI);
  CreatedCode.push_back(NewI);

  NewI->setOperand(0,VL[0]);
  NewI->setOperand(1,Op);
  return NewI;
}


void CodeGenerator::generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder) {
  LLVMContext &Context = F.getContext();

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
  //errs() << "Extracting: "; NewI->dump();

  BasicBlock &Entry = F.getEntryBlock();
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

Value *CodeGenerator::cloneTree(Node *N, IRBuilder<> &Builder) {
  if (NodeMap.find(N)!=NodeMap.end()) return NodeMap[N];

  if (N->isMatching()) {
    std::vector<Value*> Operands;
    for (unsigned i = 0; i<N->getNumChildren(); i++) {
      Operands.push_back(cloneTree(N->getChild(i), Builder));
    }

    //errs() << "Match: "; N->getValue(0)->dump();

    Instruction *I = dyn_cast<Instruction>(N->getValue(0));
    if (I) {
      Instruction *NewI = I->clone();
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);
      NodeMap[N] = NewI;

      //errs() << "Cloned: "; NewI->dump();

      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,Operands[i]);
      }
      
      for (auto *V : N->getValues()) {
        auto *I = dyn_cast<Instruction>(V);
	if (I && std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
      }

      generateExtract(N, NewI, Builder);

      return NewI;
    } else return N->getValue(0);
  } else {
    Value *NewV = generateMismatchingCode(N->getValues(), IndVar, PreHeader, Builder, Garbage, CreatedCode);
    NodeMap[N] = NewV;
    return NewV;
  }
}

static Instruction *SchedulingPoint(Tree &T, BasicBlock &BB) {
  Instruction *LastInst = dyn_cast<Instruction>(T.Root->getValues()[T.getWidth()-1]);
  
  auto SplitPt = BB.begin();
  while ( &*SplitPt!=LastInst ) SplitPt++;
  SplitPt++;

  return &*SplitPt;
}

bool CodeGenerator::generate(SeedGroups &Seeds) {
  LLVMContext &Context = BB.getParent()->getContext();

  Instruction *InstSplitPt = SchedulingPoint(T, BB);

  if (InstSplitPt==nullptr) {
    return false;
  }
 
  std::vector<BasicBlock*> SuccBBs;
  for (auto It = succ_begin(&BB), E = succ_end(&BB); It!=E; It++) SuccBBs.push_back(*It);

  PreHeader = BasicBlock::Create(Context, "rolled.pre", BB.getParent());

  Header = BasicBlock::Create(Context, "rolled.loop", BB.getParent());

  IRBuilder<> Builder(Header);

  Type *IndVarTy = IntegerType::get(Context, 64);

  IndVar = Builder.CreatePHI(IndVarTy, 0);
  CreatedCode.push_back(IndVar);

  std::map<Instruction*,Instruction*> Extracted;
  Exit = BasicBlock::Create(Context, "rolled.exit", BB.getParent());

  cloneTree(T.Root, Builder);

  auto *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  CreatedCode.push_back(Add);

  auto *Cond = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, T.getWidth()));
  CreatedCode.push_back(Cond);

  TargetTransformInfo TTI(BB.getParent()->getParent()->getDataLayout());

  size_t SizeOriginal = EstimateSize(Garbage,&TTI);
  size_t SizeModified = EstimateSize(CreatedCode,&TTI) + 3;

  bool Profitable = SizeOriginal >= SizeModified;

  errs() << "Gains: " << SizeOriginal << " - " << SizeModified << " = " << ( ((int)SizeOriginal) - ((int)SizeModified) ) << "\n";
  if (Profitable) {
    errs() << "Profitable: finishing code generation\n";

    IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),PreHeader);
    IndVar->addIncoming(Add,Header);

    auto *Br = Builder.CreateCondBr(Cond,Header,Exit);
    CreatedCode.push_back(Br);

    Builder.SetInsertPoint(PreHeader);
    Builder.CreateBr(Header);

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
    Builder.CreateBr(PreHeader);

    for (auto &Pair : Extracted) {
      Pair.first->replaceAllUsesWith(Pair.second);
    }

    for (auto It = Garbage.rbegin(), E = Garbage.rend(); It!=E; It++) {
      Seeds.remove(*It);
      (*It)->eraseFromParent();
    }
    
    for (BasicBlock *Succ : SuccBBs) {
      for (Instruction &I : *Succ) {
        if (auto *PHI = dyn_cast<PHINode>(&I)) {
          PHI->replaceIncomingBlockWith(&BB,Exit);
        }
      }
    }

    return true;
  } else {
    errs() << "Unprofitable: deleting generated code\n";

    /*
    for (auto It = CreatedCode.rbegin(), E = CreatedCode.rend(); It!=E; It++) {
      Value *V = *It;
      if (auto *I = dyn_cast<Instruction>(V)) {
        I->dropAllReferences();
        I->eraseFromParent();
      } else if (auto *GV = dyn_cast<GlobalVariable>(V)) {
        GV->dropAllReferences();
        GV->eraseFromParent();
      } else {
         errs() << "Forgotten:"; V->dump();
      }
    }
    
    Exit->eraseFromParent();
    Header->eraseFromParent();
    PreHeader->eraseFromParent();
    */
    DeleteDeadBlock(Exit);
    DeleteDeadBlock(Header);
    DeleteDeadBlock(PreHeader);

    return false;
  }
}

void LoopRoller::collectSeedInstructions(BasicBlock &BB) {
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
      auto &Stores = Seeds.Stores[getUnderlyingObject(SI->getPointerOperand())];
      
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
        Seeds.Calls[Callee].push_back(CI);
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

bool LoopRoller::run() {
  std::vector<BasicBlock *> Blocks;
  for (BasicBlock &BB : F) Blocks.push_back(&BB);

  for (BasicBlock *BB : Blocks) {
    collectSeedInstructions(*BB);
    for (auto &Pair : Seeds.Stores) {
      if (Pair.second.size()>1) {
        Tree T(Pair.second);
	CodeGenerator CG(F, *BB, T);
	CG.generate(Seeds);
        T.destroy();
      }
    }
    for (auto &Pair : Seeds.Calls) {
      if (Pair.second.size()>1) {
        Tree T(Pair.second);
	CodeGenerator CG(F, *BB, T);
	CG.generate(Seeds);
        T.destroy();
      }
    }
  }

  return true;
}

bool LoopRolling::runImpl(Function &F) {
  LoopRoller RL(F);
  return RL.run();
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

INITIALIZE_PASS(LoopRollingLegacyPass, "loop-rolling", "Loop rolling over straight-line code", false, false)

// The public interface to this file...
FunctionPass *llvm::createLoopRollingPass() {
  return new LoopRollingLegacyPass();
}



