
#include "llvm/Transforms/Scalar/LoopRerolling.h"
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

#define DEBUG_TYPE "loop-rerolling"

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

  class Node {
  private:
    std::vector<Value*> Values;
    std::vector<Node *> Children;
    Node *Parent;

  public:
    Node(Node *Parent=nullptr) : Match(false), Parent(Parent) {}

    template<typename ValueT>
    Node(std::vector<ValueT *> &Vs, Node *Parent=nullptr) : Match(false), Parent(Parent) {
      for(auto *V : Vs) pushValue(V);
    }

    Node *getParent() { return Parent; }

    size_t size() { return Values.size(); }

    void pushValue(Value *V) { Values.push_back(V); }
    void pushChild( Node * N ) { Children.push_back(N); }

    Value *getValue(unsigned i) { return Values[i]; }
    std::vector<Value*> &getValues() { return Values; }

    Type *getType() { return Values[0]->getType(); }    

    const std::vector< Node * > &getChildren() { return Children; }
    size_t getNumChildren() { return Children.size(); }

    Node *getChild(unsigned i) { return Children[i]; }

    void clearChildren() { Children.clear(); }

    bool isMatching() { return Match; }
    bool Match;
  };

  class Tree {
  public:
    Node *Root;
    std::vector<Node*> Nodes;

    Tree(Node *Root) : Root(Root) { addNode(Root); }
    void addNode(Node *N) { if (N) Nodes.push_back(N); if (Root==nullptr) Root = N; }

    void destroy() {
      for (Node *N : Nodes) delete N;
      Root = nullptr;
      Nodes.clear();
    }
  };


static void growTree(Node *N, Tree *T) {
  Instruction *I = dyn_cast<Instruction>(N->getValue(0));
  if (I==nullptr) return;

  for (unsigned i = 0; i<I->getNumOperands(); i++) {
    errs() << "Op: " << i << "\n";
    std::vector<Value*> Vs;
    for (unsigned j = 0; j<N->size(); j++) {
      Vs.push_back( dyn_cast<Instruction>(N->getValue(j))->getOperand(i) );
    }
    Node *Child = new Node(Vs);
    Child->Match = match(Vs); 
    T->addNode(Child);
    N->pushChild(Child);
    for (auto *V : Vs) V->dump();
    if (Child->isMatching()) {
      errs() << "MATCH!\n";
      growTree(Child, T);
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

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage);

static Value *generateMismatchingCode(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage) {
  Function *F = PreHeader->getParent();
  Module *M = F->getParent();
  LLVMContext &Context = F->getContext();

  if (allConstant(VL)) {
    if (isConstantSequence(VL)) {
      auto *BaseValue = dyn_cast<ConstantInt>(VL[0]);
      auto *CastIndVar = Builder.CreateIntCast(IndVar, BaseValue->getType(), false);
      if (BaseValue->isZero()) return CastIndVar;
      else return Builder.CreateAdd(CastIndVar, BaseValue);
    } else {
	    
      auto *ArrTy = ArrayType::get(VL[0]->getType(), VL.size());
      GlobalVariable* GArray = dyn_cast<GlobalVariable>(M->getOrInsertGlobal("vals", ArrTy));
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

      return Builder.CreateLoad(VL[0]->getType(), GEP);
    }
  } else {
    if (Value *V = generateGEPSequence(VL, IndVar, PreHeader, Builder, Garbage)) {
      return V;
    }
    BasicBlock &Entry = F->getEntryBlock();
    IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

    Type *IndVarTy = IntegerType::get(Context, 64);
    Value *ArrPtr = ArrBuilder.CreateAlloca(VL[0]->getType(), ConstantInt::get(IndVarTy, VL.size()));

    ArrBuilder.SetInsertPoint(PreHeader->getTerminator());
    for (unsigned i = 0; i<VL.size(); i++) 
      ArrBuilder.CreateStore(VL[i], ArrBuilder.CreateGEP(ArrPtr, ConstantInt::get(IndVarTy, i)) );

    return Builder.CreateLoad(VL[0]->getType(), Builder.CreateGEP(ArrPtr, IndVar));
  }
  return VL[0];
}

static Value *generateGEPSequence(std::vector<Value *> &VL, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage) {
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

    Value *IndVarIdx = generateMismatchingCode(Indices, IndVar, PreHeader, Builder, Garbage);
    return Builder.CreateGEP(Ptr, IndVarIdx);
  }
  return nullptr;
}

static Value *cloneTree(Node *N, Value *IndVar, BasicBlock *PreHeader, IRBuilder<> &Builder, std::vector<Instruction *> &Garbage) {
  if (N->isMatching()) {
    std::vector<Value*> Operands;
    for (unsigned i = 0; i<N->getNumChildren(); i++) {
      Operands.push_back(cloneTree(N->getChild(i), IndVar, PreHeader, Builder, Garbage));
    }

    Instruction *I = dyn_cast<Instruction>(N->getValue(0));
    if (I) {
      Instruction *NewI = I->clone();

      Builder.Insert(NewI);

      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,Operands[i]);
      }
      
      for (auto *V : N->getValues()) {
	errs() << "Deleting: "; V->dump();

        auto *I = dyn_cast<Instruction>(V);

	if (I && std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
      }
      return NewI;
    } else return N->getValue(0);
  } else {
    return generateMismatchingCode(N->getValues(), IndVar, PreHeader, Builder, Garbage);
  }
}


static void buildTree(std::vector<Instruction*> Seeds) {
  for (auto *V : Seeds) V->dump();
  Tree T(new Node(Seeds));
  T.Root->Match = true;
  growTree(T.Root, &T);


  BasicBlock &BB = *Seeds[0]->getParent();
  LLVMContext &Context = BB.getParent()->getContext();

  BB.getParent()->dump();

  auto SplitPt = BB.begin();
  while ( &*SplitPt!=Seeds[Seeds.size()-1] ) SplitPt++;
  SplitPt++;

  BasicBlock *Header = BasicBlock::Create(Context, "reroll.header", BB.getParent());
  BasicBlock *Exit = BasicBlock::Create(Context, "reroll.exit", BB.getParent());

  IRBuilder<> Builder(Exit);
  while (SplitPt!=BB.end()) {
    Instruction *I = &*SplitPt;
    SplitPt++;

    I->removeFromParent();
    Builder.Insert(I);
  }
  
  Builder.SetInsertPoint(&BB);
  Builder.CreateBr(Header);

  Builder.SetInsertPoint(Header);

  Type *IndVarTy = IntegerType::get(Context, 64);

  PHINode *IndVar = Builder.CreatePHI(IndVarTy, 0);
  IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),&BB);

  std::vector<Instruction *> Garbage;
  cloneTree(T.Root, IndVar, &BB, Builder, Garbage);

  for (auto It = Garbage.rbegin(), E = Garbage.rend(); It!=E; It++) {
    (*It)->eraseFromParent();
  }

  Value *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  IndVar->addIncoming(Add,Header);

  Value *Cond = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, Seeds.size()));
  Builder.CreateCondBr(Cond,Header,Exit);

  T.destroy();
}

void LoopRerolling::collectSeedInstructions(BasicBlock &BB) {
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

bool LoopRerolling::runImpl(Function &F) {
  for (BasicBlock &BB : F) {
    collectSeedInstructions(BB);
    for (auto &Pair : Seeds) {
      if (Pair.second.size()>1)
        buildTree(Pair.second); 
    }
  }
  return true;
}

PreservedAnalyses LoopRerolling::run(Function &F, FunctionAnalysisManager &AM) {
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

class LoopRerollingLegacyPass : public FunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid

  explicit LoopRerollingLegacyPass() : FunctionPass(ID) {
    initializeLoopRerollingLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override {
    if (skipFunction(F))
      return false;
    return Impl.runImpl(F);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
  }

private:
  LoopRerolling Impl;

};

char LoopRerollingLegacyPass::ID = 0;

INITIALIZE_PASS(LoopRerollingLegacyPass, "loop-rerolling", "Loop rerolling over straight-line code", false, false)

// The public interface to this file...
FunctionPass *llvm::createLoopRerollingPass() {
  return new LoopRerollingLegacyPass();
}



