
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
#include <algorithm>    // std::find
#include <fstream>
#include <memory>
#include <string>
#include <cxxabi.h>

#define DEBUG_TYPE "loop-rolling"

#define TEST_DEBUG

using namespace llvm;

static cl::opt<bool>
AlwaysRoll("loop-rolling-always", cl::init(false), cl::Hidden,
                 cl::desc("Always roll loops, skipping the profitability analysis"));

static cl::opt<int>
SizeThreshold("loop-rolling-size-threshold", cl::init(2), cl::Hidden,
                 cl::desc("Size threshold for the loop rolling profitability analysis"));

static cl::opt<bool>
MatchAlignment("loop-rolling-match-alignment", cl::init(false), cl::Hidden,
                 cl::desc("Consider alignment while matching instructions"));

static std::string demangle(const char* name) {
  int status = -1;
  std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
  return (status == 0) ? res.get() : std::string(name);
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
static Value *isConstantSequence(const std::vector<ValueT *> VL) {
  auto *CInt = dyn_cast<ConstantInt>(VL[0]);
  if (CInt==nullptr) return nullptr;

  APInt Last = CInt->getValue();
  ConstantInt *Step = nullptr;
  for(unsigned i = 1; i<VL.size(); i++) {
    if (VL[0]->getType()!=VL[i]->getType()) return nullptr;
    auto *CInt = dyn_cast<ConstantInt>(VL[i]);
    APInt Val = CInt->getValue();
    APInt StepInt(Val);
    StepInt -= Last;
    if (Step) {
      if (Step->getValue()!=StepInt) return nullptr;
    } else Step = (ConstantInt*)ConstantInt::get(VL[0]->getType(), StepInt);
    Last = Val;
  }

  return Step;
}

template<typename GetElementPtrT>
static bool matchGEP(GetElementPtrT *GEP1, GetElementPtrT *GEP2) {
  Type *Ty1 = GEP1->getSourceElementType();
  SmallVector<Value*, 16> Idxs1(GEP1->idx_begin(), GEP1->idx_end());

  Type *Ty2 = GEP2->getSourceElementType();
  SmallVector<Value*, 16> Idxs2(GEP2->idx_begin(), GEP2->idx_end());

  if (Ty1!=Ty2) return false;
  if (Idxs1.size()!=Idxs2.size()) return false;

  if (Idxs1.empty())
    return true;

  for (unsigned i = 1; i<Idxs1.size(); i++) {
    Value *V1 = Idxs1[i];
    Value *V2 = Idxs2[i];

    //structs must have constant indices, therefore they must be constants and must be identical when merging
    if (isa<StructType>(Ty1)) {
      if (V1!=V2) return false;
    }
    Ty1 = GetElementPtrInst::getTypeAtIndex(Ty1, V1);
    Ty2 = GetElementPtrInst::getTypeAtIndex(Ty2, V2);
    if (Ty1!=Ty2) return false;
  }
  return true;
}

template<typename GetElementPtrT>
static Type *matchGEPUntilLastType(GetElementPtrT *GEP1, GetElementPtrT *GEP2) {
  Type *Ty1 = GEP1->getSourceElementType();
  SmallVector<Value*, 16> Idxs1(GEP1->idx_begin(), GEP1->idx_end());

  Type *Ty2 = GEP2->getSourceElementType();
  SmallVector<Value*, 16> Idxs2(GEP2->idx_begin(), GEP2->idx_end());

  if (Ty1!=Ty2) return nullptr;
  if (Idxs1.size()!=Idxs2.size()) return nullptr;

  if (Idxs1.empty())
    return Ty1;

  for (unsigned i = 1; i<Idxs1.size()-1; i++) {
    Value *V1 = Idxs1[i];
    Value *V2 = Idxs2[i];

    //structs must have constant indices, therefore they must be constants and must be identical when merging
    if (isa<StructType>(Ty1)) {
      if (V1!=V2) return nullptr;
    }
    Ty1 = GetElementPtrInst::getTypeAtIndex(Ty1, V1);
    Ty2 = GetElementPtrInst::getTypeAtIndex(Ty2, V2);
    if (Ty1!=Ty2) return nullptr;
  }
  return Ty1;
}



static bool match(Value *V1, Value *V2) {
  Instruction *I1 = dyn_cast<Instruction>(V1);
  Instruction *I2 = dyn_cast<Instruction>(V2);
  
  if (V1->getType()!=V2->getType()) return false;

  if(I1 && I2) {
    //return I1->isSameOperationAs(I2);
    if (I1->getOpcode()!=I2->getOpcode()) return false;
    if (I1->getNumOperands()!=I2->getNumOperands()) return false;

    for (unsigned i = 0; i<I1->getNumOperands(); i++)
      if (I1->getOperand(i)->getType()!=I2->getOperand(i)->getType()) return false;

    switch (I1->getOpcode()) {
    case Instruction::Alloca:
    case Instruction::Invoke:
    case Instruction::PHI:
      return false;
    case Instruction::Call: {
      CallInst *CI1 = dyn_cast<CallInst>(I1);
      CallInst *CI2 = dyn_cast<CallInst>(I2);
      if (CI1->getCalledFunction()==nullptr || CI2->getCalledFunction()==nullptr) return false;
      if (CI1->getCalledFunction()!=CI2->getCalledFunction()) return false;
      if (CI1->getCalledFunction()->isVarArg()) return false;
      return true;
    }
    case Instruction::Load: {
      auto *LI1 = dyn_cast<LoadInst>(I1);
      auto *LI2 = dyn_cast<LoadInst>(I2);
      return (MatchAlignment?(LI1->getAlign()==LI2->getAlign()):true);
    }
    case Instruction::Store: {
      auto *SI1 = dyn_cast<StoreInst>(I1);
      auto *SI2 = dyn_cast<StoreInst>(I2);
      return (MatchAlignment?(SI1->getAlign()==SI2->getAlign()):true);
    }
    case Instruction::GetElementPtr: {
      auto *GEP1 = dyn_cast<GetElementPtrInst>(I1);
      auto *GEP2 = dyn_cast<GetElementPtrInst>(I2);
      bool Result = matchGEP(GEP1,GEP2);
      return Result;
    }
    default:
      return true;
    }
  }

  return V1==V2;
}

enum NodeType {
  MATCH, IDENTICAL, BINOP, GEPSEQ, INTSEQ, ALTSEQ, CONSTEXPR, REDUCTION, RECURRENCE, MISMATCH, MULTI
};

class Node {
private:
  BasicBlock *BBPtr;
  Node *Parent;
  NodeType NType;
  std::vector<Value*> Values;
  std::vector<Node *> Children;
public:
  Node(NodeType NT, BasicBlock *BB, Node *Parent=nullptr) : BBPtr(BB), Parent(Parent), NType(NT) {}

  template<typename ValueT>
  Node(NodeType NT, std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : BBPtr(BB), Parent(Parent), NType(NT) {
    for (auto *V : Vs) Values.push_back(V);
  }

  Node *getParent() { return Parent; }

  size_t size() { return Values.size(); }

  void pushChild( Node * N ) { Children.push_back(N); }

  Value *getValue(unsigned i) { return Values[i]; }
  std::vector<Value*> &getValues() { return Values; }

  Type *getType() { return Values[0]->getType(); }    

  const std::vector< Node * > &getChildren() { return Children; }
  size_t getNumChildren() { return Children.size(); }

  Node *getChild(unsigned i) { return Children[i]; }

  void clearChildren() { Children.clear(); }

  NodeType getNodeType() { return NType; }

  BasicBlock *getBlock() { return BBPtr; }

  virtual std::string getString() = 0;
  virtual Instruction *getValidInstruction(unsigned i) = 0;

  bool mustKeepOrder() {
    if (getNodeType()==NodeType::MATCH || getNodeType()==NodeType::MULTI) {
      for (unsigned i = 0; i<size(); i++) {
        if (auto *I = getValidInstruction(i)) {
          if (I->mayReadOrWriteMemory() || I->mayHaveSideEffects()) return true;
        }
      }
    }
    return false;
  }
};

class MultiNode : public Node {
public:
  std::vector< std::vector<Instruction *> > Groups;

  MultiNode(BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::MULTI,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    return "multi flow";
  }

  void addGroup(std::vector<Instruction *> &Vs) { Groups.push_back(Vs); }
  size_t getNumGroups() { return Groups.size(); }
  std::vector<Instruction *> &getGroup(size_t i) { return Groups[i]; }

};


class MatchingNode : public Node {
public:
  template<typename ValueT>
  MatchingNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::MATCH,Vs,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return dyn_cast<Instruction>(getValue(i));
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    Value *V = getValue(0);

    if (Instruction *I = dyn_cast<Instruction>(V)) {
      labelStream << I->getOpcodeName();
      if (CallInst *CI = dyn_cast<CallInst>(I)) {
        Function *F = CI->getCalledFunction();
        if (F && F->hasName())
          labelStream << ": " << demangle(F->getName().data());
      }
    } else if (isa<Constant>(V) && !isa<Function>(V)) {
      if (isa<ConstantInt>(V) || isa<ConstantFP>(V) || isa<ConstantPointerNull>(V) ||
          isa<UndefValue>(V))
        V->printAsOperand(labelStream, false);
      else
        labelStream << "const";
    } else if (isa<Argument>(V)) {
      labelStream << "arg";
    } else if (isa<Function>(V)) {
      labelStream << "func";
      Function *F = dyn_cast<Function>(V);
      if (F && F->hasName())
        labelStream << ": " << demangle(F->getName().data());
    }

    return labelStream.str();
  }
};

class ConstantExprNode : public Node {
public:
  template<typename ValueT>
  ConstantExprNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::CONSTEXPR,Vs,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    Value *V = getValue(0);

    if (auto *CE = dyn_cast<ConstantExpr>(V)) {
      labelStream << "cosntexpr: " << Instruction::getOpcodeName(CE->getOpcode());
    } else labelStream << "expected constexpr";

    return labelStream.str();
  }
};


class MismatchingNode : public Node {
public:
  template<typename ValueT>
  MismatchingNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::MISMATCH,Vs,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    if (allConstant(getValues())) {
      if (isConstantSequence(getValues())) {
        std::string str;
        raw_string_ostream labelStream(str);
        Value *V = getValue(0);
        Value *Last = *getValues().rbegin();
        if (isa<ConstantInt>(V) || isa<ConstantFP>(V) || isa<ConstantPointerNull>(V) ||
          isa<UndefValue>(V)) {
          V->printAsOperand(labelStream, false);
	  labelStream << "..";
          Last->printAsOperand(labelStream, false);
	} else labelStream << "const";

        return labelStream.str();
      }
      return "constant mismatch";
    }
    return "mismatch";
  }
};

class IdenticalNode : public Node {
public:
  template<typename ValueT>
  IdenticalNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::IDENTICAL,Vs,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    Value *V = getValue(0);

    if (Instruction *I = dyn_cast<Instruction>(V)) {
      labelStream << I->getOpcodeName();
      if (CallInst *CI = dyn_cast<CallInst>(I)) {
        Function *F = CI->getCalledFunction();
        if (F && F->hasName())
          labelStream << ": " << demangle(F->getName().data());
      }
    } else if (isa<Constant>(V) && !isa<Function>(V)) {
      if (isa<ConstantInt>(V) || isa<ConstantFP>(V) || isa<ConstantPointerNull>(V) ||
          isa<UndefValue>(V))
        V->printAsOperand(labelStream, false);
      else
        labelStream << "const";
    } else if (isa<Argument>(V)) {
      labelStream << "arg";
    } else if (isa<Function>(V)) {
      labelStream << "func";
      Function *F = dyn_cast<Function>(V);
      if (F && F->hasName())
        labelStream << ": " << demangle(F->getName().data());
    }

    return labelStream.str();
  }
};

class IntSequenceNode : public Node {
private:
  Value *Step;
public:
  template<typename ValueT>
  IntSequenceNode(std::vector<ValueT *> &Vs, Value *Step, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::INTSEQ,Vs,BB,Parent), Step(Step) {}

  Value *getStart() { return getValue(0); }
  Value *getEnd() { return getValue(size()-1); }
  Value *getStep() { return Step; }

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    getStart()->printAsOperand(labelStream, false);
    labelStream << "..";
    getEnd()->printAsOperand(labelStream, false);
    labelStream << ", ";
    getStep()->printAsOperand(labelStream, false);
    return labelStream.str();
  }
};

class AlternatingSequenceNode : public Node {
private:
  Value *First;
  Value *Second;
public:
  template<typename ValueT>
  AlternatingSequenceNode(std::vector<ValueT *> &Vs, Value *First, Value *Second, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::ALTSEQ,Vs,BB,Parent), First(First), Second(Second) {}

  Value *getFirst() { return First; }
  Value *getSecond() { return Second; }

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << "alt: ";
    getFirst()->printAsOperand(labelStream, false);
    labelStream << ", ";
    getSecond()->printAsOperand(labelStream, false);
    return labelStream.str();
  }
};

class GEPSequenceNode : public Node {
private:
  GetElementPtrInst *RefGEP;
  Value *Ptr;
  std::vector<Value*> Indices;
public:
  template<typename ValueT>
  GEPSequenceNode(std::vector<ValueT *> &Vs, GetElementPtrInst *RefGEP, Value *Ptr, std::vector<Value*> &Indices, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::GEPSEQ,Vs,BB,Parent), RefGEP(RefGEP), Ptr(Ptr), Indices(Indices) {}

  Value *getPointerOperand() { return Ptr; }
  std::vector<Value *> &getIndices() { return Indices; }

  Instruction *getValidInstruction(unsigned i) {
    auto *I = dyn_cast<GetElementPtrInst>(getValue(i));

    if (I==nullptr) return nullptr;
    if (Ptr && getUnderlyingObject(I)!=Ptr) return nullptr;
    //if (I==nullptr || getUnderlyingObject(I)!=Ptr) return nullptr;
    if (I->getParent()!=getBlock()) return nullptr;
    if (I->getOpcode()!=Instruction::GetElementPtr) return nullptr;
    if (I->getNumOperands()!=RefGEP->getNumOperands()) return nullptr;

    return I;
  }

  GetElementPtrInst *getReference() { return RefGEP; }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << "GEP seq.";
    return labelStream.str();
  }
};

class BinOpSequenceNode : public Node {
private:
  BinaryOperator *BinOpRef;
  std::vector<Value *> LeftOperands;
  std::vector<Value *> RightOperands;
public:
  template<typename ValueT>
  BinOpSequenceNode(std::vector<ValueT *> &Vs, BinaryOperator *BinOpRef, std::vector<Value *> &LeftOperands, std::vector<Value *> &RightOperands, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::BINOP,Vs,BB,Parent), BinOpRef(BinOpRef), LeftOperands(LeftOperands), RightOperands(RightOperands) {}

  BinaryOperator *getReference() { return BinOpRef; }
  std::vector<Value *> &getLeftOperands() { return LeftOperands; }
  std::vector<Value *> &getRightOperands() { return RightOperands; }

  Instruction *getValidInstruction(unsigned i) {
    auto *I = dyn_cast<BinaryOperator>(getValue(i));
    if (I && I->getOpcode()!=BinOpRef->getOpcode()) return nullptr;
    return I;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << BinOpRef->getOpcodeName() << " seq.";
    return labelStream.str();
  }
};


class RecurrenceNode : public Node {
private:
  Value *StartValue;
public:
  template<typename ValueT>
  RecurrenceNode(std::vector<ValueT *> &Vs, Value *StartValue, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::RECURRENCE,Vs,BB,Parent), StartValue(StartValue) {}
  Value *getStartValue() { return StartValue; }

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << "recurrence";
    return labelStream.str();
  }
};


class ReductionNode : public Node {
private:
  BinaryOperator *BinOpRef;
  Value *Start;
  std::vector<Value *> Vs;
public:

  ReductionNode(BinaryOperator *BinOp, PHINode *Start, std::vector<BinaryOperator*> &BOs, std::vector<Value*> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::REDUCTION,BOs,BB,Parent), BinOpRef(BinOp), Start(Start), Vs(Vs) {}

  BinaryOperator *getBinaryOperator() { return BinOpRef; }
  std::vector<Value *> &getOperands() { return Vs; }
  Value *getStartValue() { if (Start) return Start; else return getNeutralValue(); }

  Instruction *getValidInstruction(unsigned i) {
    auto *I = dyn_cast<BinaryOperator>(getValue(i));
    //if (I && I->getOpcode()!=BinOpRef->getOpcode()) return nullptr;
    return I;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << BinOpRef->getOpcodeName() << " red.";
    return labelStream.str();
  }

  Value *getNeutralValue() {
    Type *Ty = BinOpRef->getType();

    switch(BinOpRef->getOpcode()) {
    case Instruction::Add:
    case Instruction::Sub:
    case Instruction::Or:
    case Instruction::Xor:
      return ConstantInt::get(Ty, 0);
    case Instruction::FAdd:
      return ConstantFP::get(Ty, 0.0);
    case Instruction::Mul:
    case Instruction::UDiv:
    case Instruction::SDiv:
    case Instruction::And:
      return ConstantInt::get(Ty, 1);
    case Instruction::FMul:
      return ConstantFP::get(Ty, 1.0);
    default:
      return nullptr;
    }
  }

  static bool isValidOperation(BinaryOperator *BO) {
    switch(BO->getOpcode()) {
    case Instruction::Add:
    case Instruction::FAdd:
    case Instruction::Mul:
    case Instruction::FMul:
    case Instruction::Or:
    case Instruction::And:
    case Instruction::Xor:
      //return (BO->isAssociative() && BO->isCommutative());
      return true; 
    default:
      return false;
    }
  }

  static void collectValues(BinaryOperator *BO, PHINode *PHI, std::vector<BinaryOperator*> &BOs, std::vector<Value*> &Vs) {
    BOs.push_back(BO);
    Value *V0 = BO->getOperand(0);
    Value *V1 = BO->getOperand(1);
    BinaryOperator *BO0 = dyn_cast<BinaryOperator>(V0);
    BinaryOperator *BO1 = dyn_cast<BinaryOperator>(V1);
    if (BO0 && BO0->getParent()==BO->getParent() && BO0->getOpcode()==BO->getOpcode()) collectValues(BO0, PHI, BOs, Vs);
    else if (PHI!=V0) Vs.push_back(V0);
    if (BO1 && BO1->getParent()==BO->getParent() && BO1->getOpcode()==BO->getOpcode()) collectValues(BO1, PHI, BOs, Vs);
    else if (PHI!=V1) Vs.push_back(V1);
  }

};

template<typename ValueT>
static size_t EstimateSize(ValueT *V, const DataLayout &DL, TargetTransformInfo *TTI) {
  size_t size = 0;
    if (V==nullptr) return 0;
    //V->dump();
    //size_t Cost = size;
    if (auto *I = dyn_cast<Instruction>(V)) {
  
      switch(I->getOpcode()) {
        case Instruction::PHI:
        case Instruction::Alloca:
          size += 0;
          break;
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
          size += 1;
          break;
        case Instruction::Call:
          size += 1 + dyn_cast<CallBase>(I)->getNumArgOperands();
          break;
        case Instruction::GetElementPtr: {
	  Value *V = getUnderlyingObject(I);
	  size += 1;
	  if (V)
            size += (isa<GlobalValue>(V))?1:0;
          break;
	}
        case Instruction::Load: {
          size += 2;
          //auto *LI = dyn_cast<LoadInst>(I);
          //if (isa<GlobalValue>(getUnderlyingObject(LI->getPointerOperand()))) {
            //size += 1;
          //}
          break;
        }
        case Instruction::Store: {
          size += 2;
          //auto *SI = dyn_cast<StoreInst>(I);
          //if (isa<GlobalValue>(getUnderlyingObject(SI->getPointerOperand()))) {
            //size += 1;
          //}
          break;
        }
        default: {
          size += 1;
          BinaryOperator *BO = dyn_cast<BinaryOperator>(I);
	  if (BO) {
            if (isa<Constant>(BO->getOperand(0)) || isa<Constant>(BO->getOperand(1))) {
	      unsigned BitWidth = 0;
	      if (BO->getType()->isIntegerTy()) 
	        BitWidth = BO->getType()->getIntegerBitWidth();
	      if (BitWidth>=32) size += 1;
	      if (BitWidth>=64) size += 1;
	    }
	  }
	}
      }
    } else if (auto *GV = dyn_cast<GlobalVariable>(V)) {
      size += 1;
      //if (GV->hasInitializer()) {
	//if (auto *ArrTy = dyn_cast<ArrayType>(GV->getInitializer()->getType())) {
	   //size -= 1;
	   //size_t Factor = 1; //DL.getTypeSizeInBits(ArrTy->getElementType())/8;
	   //size += Factor*ArrTy->getNumElements();
	//}
      //}
    }
    //Cost = size - Cost;
    //errs() << "Cost: " << Cost << "\n";
    //
  return size;
}

template<typename ValueListT>
static size_t EstimateSize(ValueListT &Code, const DataLayout &DL, TargetTransformInfo *TTI) {
  size_t size = 0;
  for (auto *V : Code) {
    size += EstimateSize(V,DL,TTI);
  }
  return size;
}

static size_t EstimateSize(BasicBlock *BB, const DataLayout &DL, TargetTransformInfo *TTI) {
  size_t size = 0;
  for (auto &I : *BB) {
    size += EstimateSize(&I,DL,TTI);
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

class AlignedGraph {
public:
  BasicBlock *BB;
  ScalarEvolution *SE;
  Node *Root;
  std::vector<Node*> Nodes;
  std::unordered_map<Value*, std::unordered_set<Node*> > NodeMap;
  std::unordered_map<Value*, std::unordered_set<Node*> > NodeMap2;
  std::vector<Node *> SchedulingOrder;
  std::unordered_set<Value *> ValuesInNode;
  std::unordered_set<Value *> Inputs;

  AlignedGraph(Node *N, BasicBlock *BB, ScalarEvolution *SE=nullptr) : BB(BB), SE(SE) {
    Root = N;
    if (Root) {
      addNode(Root);
      std::set<Node*> Visited;
      growGraph(Root, BB, Visited);
    }
  }

  AlignedGraph(BinaryOperator *BO, Instruction *U, BasicBlock *BB, ScalarEvolution *SE=nullptr) : BB(BB), SE(SE) {
    Root = buildReduction(BO,U,BB,nullptr);
    if (Root) {
      addNode(Root);
      std::set<Node*> Visited;
      growGraph(Root,BB, Visited);
    }
  }

  template<typename ValueT>
  AlignedGraph(std::vector<ValueT*> &Vs, BasicBlock *BB, ScalarEvolution *SE=nullptr) : BB(BB), SE(SE) {
    Root = createNode(Vs,BB);
    addNode(Root);
    std::set<Node*> Visited;
    growGraph(Root,BB, Visited);
  }

  BasicBlock *getBlock() { return BB; }

  size_t getWidth() {
    switch (Root->getNodeType()) {
      case NodeType::MULTI:{
        return Root->getChild(0)->size();
      }
      default:
        return Root->size();
    }
  }

  void addNode(Node *N) {
    if (N) {
      if (std::find(Nodes.begin(), Nodes.end(), N)==Nodes.end()) {
        Nodes.push_back(N);
	if (N->getNodeType()!=NodeType::MISMATCH && N->getNodeType()!=NodeType::MULTI && N->getNodeType()!=NodeType::IDENTICAL && N->getNodeType()!=NodeType::RECURRENCE) {
          //for (auto *V : N->getValues()) {
	  for (unsigned i = 0; i<N->size(); i++) {
            Value *V = N->getValidInstruction(i);
	    if (V) ValuesInNode.insert(V);
            NodeMap2[V].insert(N);
	  }
	}
	if (N->size()) NodeMap[N->getValue(0)].insert(N);
      }
      if (Root==nullptr) Root = N;
    }
  }

  bool contains(Value *V) { return ValuesInNode.count(V); }
  
  template<typename ValueT>
  Node *find(std::vector<ValueT *> &Vs);

  Node *find(Instruction *I);

  void destroy() {
    for (Node *N : Nodes) delete N;
    Root = nullptr;
    Nodes.clear();
  }

  bool dependsOn(Instruction *I, Value *V, BasicBlock *BB, std::unordered_set<Value*> &Visited) {
    if (I->getParent()!=BB) return false;
    if (isa<PHINode>(I)) return false;
    if (Visited.find(I)!=Visited.end()) return false;
    Visited.insert(I);

    for (unsigned i = 0; i<I->getNumOperands(); i++) {
      if (I->getOperand(i)==V) return true;
    }
    bool Depends = false;
    for (unsigned i = 0; i<I->getNumOperands(); i++) {
      if (Instruction *IV = dyn_cast<Instruction>(I->getOperand(i))) {
        Depends = Depends || dependsOn(IV,V,BB,Visited);
	if (Depends) break;
      }
    }
    return Depends;
  }

  bool dependsOn(Value *V) {
    bool Depends = false;
    std::unordered_set<Value*> Visited;
    if (Root->getNodeType()==NodeType::MULTI) {
      for (unsigned i = 0; i<Root->getNumChildren(); i++) {
        Node *Child = Root->getChild(i);
        for (unsigned j = 0; j<Child->size(); j++) {
          auto *I = Child->getValidInstruction(j);
          Depends = Depends || dependsOn(I,V,I->getParent(),Visited);
        }
      }
    } else {
      for (unsigned i = 0; i<Root->size(); i++) {
        auto *I = Root->getValidInstruction(i);
        Depends = Depends || dependsOn(I,V,I->getParent(),Visited);
      }
    }
    return Depends;
  }

  bool invalidDependence(Value *V, std::unordered_set<Value*> &Visited);

  Instruction *getStartingInstruction(BasicBlock &BB);
  //Instruction *getEndingInstruction(BasicBlock &BB);
  bool isSchedulable(BasicBlock &BB);

  std::string getDotString() {
    std::string dotStr;
    raw_string_ostream os(dotStr);
    os << "digraph VTree {\n";

    std::unordered_map<Node*, int> NodeId;

    int id = 0;
    //Nodes
    for (Node *N : Nodes) {
      NodeId[N] = id;

      os << id << " [label=\"" << N->getString() << "\""
        << ", style=\"filled\" , fillcolor=" << ((N->getNodeType()!=NodeType::MISMATCH) ? "\"#8ae18a\"" : "\"#ff6671\"")
        << ", shape=box" << "];\n";
        //<< ", shape=" << ((N->isMatching()) ? "box" : "oval") << "];\n";

      id++;

    }

    //Edges
    for (Node *N : Nodes) {
      //int ChildId = 0;
      for (Node *Child : N->getChildren()) {
        std::string EdgeLabel = "";
        //if (N->isCallInst()) EdgeLabel = std::string(" [label=\"") + std::to_string(ChildId) + std::string("\"]");
        os << NodeId[Child] << "->" << NodeId[N] << ' ' << EdgeLabel << "\n";
        //ChildId++;
      }
    }

    std::unordered_map<Value *, int> ExternalNodes;
    for (Node *N : Nodes) {
      for (unsigned i = 0; i<N->size(); i++) {
        Value *V = N->getValidInstruction(i);
        if (V==nullptr) continue;
        for (auto *U : V->users()) {
          //if (this->find(U)==nullptr) {
          if (!this->contains(U)) {
	    if (ExternalNodes.find(U)==ExternalNodes.end()) {
	      std::string Name = "user";
	      if (Instruction *UI = dyn_cast<Instruction>(U)) Name = UI->getOpcodeName();
              os << id << " [label=\"" << Name << "\""
                 << ", style=\"filled\" , fillcolor=\"#f2eb5c\""
                 << ", shape=box" << "];\n";

	      ExternalNodes[U] = id;
	      id++;
	    }

	    os << NodeId[N] << "->" << ExternalNodes[U] << "\n";
            
            break;
          }
        }
      }

    }
    os << "}\n";
    return os.str();
  }

  void writeDotFile(std::string FileName) {
    std::ofstream out(FileName.c_str(), std::ofstream::out);
    out << getDotString();
    out.close();
  }

  std::set<User*> Users;
private:
  void growGraph(Node *N, BasicBlock *BB, std::set<Node*> &Visited);

  template<typename ValueT>
  Node *buildReduction(ValueT *V, Instruction *, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildGEPSequence2(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildAlternatingSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildRecurrenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildConstExprNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *createNode(std::vector<ValueT*> Vs, BasicBlock *BB, Node *Parent=nullptr);
};

/*
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
*/

class SeedGroups {
public:
  std::unordered_map<Value *, std::vector<Instruction *> > Stores;
  std::unordered_map<Value *, std::vector<Instruction *> > Calls;
  std::unordered_map<BinaryOperator *, Instruction *> Reductions;

  void clear() {
    Stores.clear();
    Calls.clear();
    Reductions.clear();
  }

  void remove(Instruction *I) {
    for (auto &Pair : Stores) {
      Pair.second.erase(std::remove(Pair.second.begin(), Pair.second.end(), I), Pair.second.end());
    }
    for (auto &Pair : Calls) {
      Pair.second.erase(std::remove(Pair.second.begin(), Pair.second.end(), I), Pair.second.end());
    }
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I)) {
      if (Reductions.find(BO)!=Reductions.end()) Reductions[BO] = nullptr;
      //Reductions.erase(BO);
      //Reductions.erase(std::remove(Reductions.begin(), Reductions.end(), BO), Reductions.end());
    }
    for (auto &Pair : Reductions) {
      if (Pair.second==I) Reductions[Pair.first] = nullptr;
    }
  }

  std::vector<Instruction *> *getGroupWith(Instruction *I) {
    for (auto &Pair : Stores) {
      if (std::find(Pair.second.begin(), Pair.second.end(), I)!=Pair.second.end()) return &(Pair.second);
    }
    for (auto &Pair : Calls) {
      if (std::find(Pair.second.begin(), Pair.second.end(), I)!=Pair.second.end()) return &(Pair.second);
    }
    return nullptr;
  }
};

class CodeGenerator {
public:
  CodeGenerator(Function &F, BasicBlock &BB, AlignedGraph &G) : F(F), BB(BB), G(G) {}

  bool generate(SeedGroups &Seeds);

private:
  Function &F;
  BasicBlock &BB;
  AlignedGraph &G;
  PHINode *IndVar;
  BasicBlock *PreHeader;
  BasicBlock *Header;
  BasicBlock *Exit;

  std::unordered_map<Node*, Value *> NodeToValue;
  //std::vector<Instruction *> Garbage;
  std::unordered_set<Instruction *> Garbage;
  std::vector<Value *> CreatedCode;
  std::unordered_map<Instruction *, Instruction *> Extracted;
  std::unordered_map<GlobalVariable*, Instruction *> GlobalLoad;

  std::unordered_map<Type *, Value *> CachedCastIndVar;
  
  
  std::unordered_map<Type *, Value *> CachedRem2;
  Value *AltSeqCmp{nullptr};

  Value *cloneGraph(Node *N, IRBuilder<> &Builder);
  void generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder);
  //void generateExtract(std::vector<Value *> &VL, Instruction * NewI, IRBuilder<> &Builder);

  Value *generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder);
  //Value *generateGEPSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  //Value *generateBinOpSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  
};

class LoopRoller {
public:
  LoopRoller(Function &F, ScalarEvolution *SE) : F(F), SE(SE), NumAttempts(0), NumRolledLoops(0) {}

  bool run();
private:
  Function &F;
  ScalarEvolution *SE;
  unsigned NumAttempts;
  unsigned NumRolledLoops;

  void collectSeedInstructions(BasicBlock &BB);
  bool attemptRollingSeeds(BasicBlock &BB);
  //void codeGeneration(Tree &T, BasicBlock &BB);

  SeedGroups Seeds;
};

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
  AlignedBlock() {}
  void pushBlock(BasicBlock *BB) { Blocks.push_back(BB); }
  AlignedBlock *getSuccessor(int i) { return Successors[i]; }
  void addSuccessor(AlignedBlock *AB) { Successors.push_back(AB); }



private:
  std::vector<BasicBlock*> Blocks;
  std::vector<AlignedBlock*> Successors;
};

class AlignedRegion {
public:
  std::vector<AlignedBlock *> AlignedBlocks;
  std::map<BasicBlock*, AlignedBlock *> BlockMap;

  void releaseMemory() {
    for (AlignedBlock *AB : AlignedBlocks) delete AB;
    AlignedBlocks.clear();
    BlockMap.clear();
  }
};

class RegionRoller {
public:
  RegionRoller(Function &F) : F(F) {}

  bool run();
private:
  Function &F;
};

bool IsIsomorphic(BasicBlock *BB1, BasicBlock *ExitBB1, BasicBlock *BB2, BasicBlock *ExitBB2, AlignedBlock *AB, std::set<BasicBlock*> &Visited) {
  if (BB1==ExitBB1 && BB2==ExitBB2) return true;
  if (BB1==ExitBB1 || BB2==ExitBB2) return false;
  if (Visited.count(BB1) && Visited.count(BB2)) return true;
  if (Visited.count(BB1) || Visited.count(BB2)) return false;
  Visited.insert(BB1);
  Visited.insert(BB2);

  BranchInst *Br1 = dyn_cast<BranchInst>(BB1->getTerminator());
  BranchInst *Br2 = dyn_cast<BranchInst>(BB2->getTerminator());

  if (Br1 && Br2 && Br1->getNumSuccessors()==Br2->getNumSuccessors()) {
    bool Isomorphic = true;
    if (AB) AB->pushBlock(BB2);
    for (unsigned i = 0; i<Br1->getNumSuccessors(); i++) {
      Isomorphic = Isomorphic && IsIsomorphic(Br1->getSuccessor(i), ExitBB1, Br2->getSuccessor(i), ExitBB2, AB?AB->getSuccessor(i):nullptr, Visited);
    }
    return Isomorphic;
  }

  return false;
}

bool IsIsomorphic(BasicBlock *BB1, BasicBlock *ExitBB1, BasicBlock *BB2, BasicBlock *ExitBB2, AlignedBlock *AB) {
  std::set<BasicBlock*> Visited;
  return IsIsomorphic(BB1, ExitBB1, BB2, ExitBB2, AB, Visited);
}

AlignedBlock *initializeAlignedRegion(AlignedRegion *AR, BasicBlock *BB, BasicBlock *ExitBB, std::set<BasicBlock *> &Visited) {
  if (BB==ExitBB) return nullptr;
  if (Visited.count(BB)) return AR->BlockMap[BB];

  Visited.insert(BB);

  AlignedBlock *AB = new AlignedBlock;
  AB->pushBlock(BB);
  AR->AlignedBlocks.push_back(AB);
  AR->BlockMap[BB] = AB;

  BranchInst *Br = dyn_cast<BranchInst>(BB->getTerminator());
  if (Br) {
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

bool RegionRoller::run() {

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
  auto *TopRegion = RI.getTopLevelRegion();
  errs() << "Top Region:\n";
  TopRegion->dump();
  if (TopRegion->getExit())
    RegionsByEntry[TopRegion->getEntry()].push_back(TopRegion->getExit());

  //for (auto Region : *TopRegion) {
  for (auto It = TopRegion->begin(); It!=TopRegion->end(); It++) {
    errs() << "Sub Region:\n";
    (*It)->dump();
    if ((*It)->getExit())
      RegionsByEntry[(*It)->getEntry()].push_back( (*It)->getExit() );
  }

  for (auto It = TopRegion->begin(); It!=TopRegion->end(); It++) {
    errs() << "Reference Region:\n";
    (*It)->dump();

    AlignedRegion AR;
    initializeAlignedRegion(&AR, (*It)->getEntry(), (*It)->getExit());
    BasicBlock *LinkBlock = (*It)->getExit();
    errs() << "Link: " << LinkBlock->getName().str() << "\n";
    while (LinkBlock) {
      BasicBlock *NextLB = nullptr;
      for (BasicBlock *ExitBB : RegionsByEntry[LinkBlock]) {
	//errs() << "Evaluating Region: " << LinkBlock->getName().str() << " => " << ExitBB->getName().str() << "\n";
	//auto *Region = RI.getCommonRegion(LinkBlock, ExitBB);
	//Region->dump();
	if (IsIsomorphic((*It)->getEntry(), (*It)->getExit(), LinkBlock, ExitBB, nullptr)) {
          errs() << "Found Isomorphic:" << LinkBlock->getName().str() << " => " << ExitBB->getName().str() << "\n";
          IsIsomorphic((*It)->getEntry(), (*It)->getExit(), LinkBlock, ExitBB, AR.AlignedBlocks[0]);
	  NextLB = ExitBB;
	}
      }
      LinkBlock = NextLB;
    }

    AR.releaseMemory();
  }

  return false;
}

/*
template<typename ValueT>
Node *AlignedBlock::createNode(std::vector<ValueT*> Vs, Node *Parent) {
  
  errs() << "Creating Node\n";
  //for (auto *V : Vs) {
  //  if (isa<Function>(V)) errs() << "Function: " << V->getName() << "\n";
  //  else V->dump();
  //}
  bool AllSame = true;
  bool Matching = true;
  bool HasSideEffect = false;
  std::unordered_set<Value*> UniqueValues;
  UniqueValues.insert(Vs[0]);
  if (auto *I = dyn_cast<Instruction>(Vs[0])) {
    if (I->getParent()!=Blocks[0]) Matching = false;
    HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
  }
  for (unsigned i = 1; i<Vs.size(); i++) {
    UniqueValues.insert(Vs[i]);
    AllSame = AllSame && Vs[i]==Vs[0];
    Matching = Matching && match(Vs[i-1],Vs[i]);
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      if (I->getParent()!=Blocks[i]) Matching = false;
      HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
    }
  }
  errs() << "Match: " << Matching << "\n";
  errs() << UniqueValues.size() << " x " << Vs.size() << "\n";

  Matching = Matching && (UniqueValues.size()==Vs.size() || (!HasSideEffect));
  errs() << "Final Match: " << Matching << "\n";

  if (AllSame) {
    errs() << "All the Same\n";
    Inputs.insert(Vs[0]);
    return new IdenticalNode(Vs,BB,Parent);
  }
  if (Matching) {
    errs() << "Matching\n";
    return new MatchingNode(Vs,BB,Parent);
  }
  if (Node *N = buildGEPSequence(Vs, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }
  if (Node *N = buildGEPSequence2(Vs, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }

  if (Node *N = buildBinOpSequenceNode(Vs, BB, Parent)) {
    errs() << "BinOp Seq\n";
    return N;
  }
  if (Node *N = buildRecurrenceNode(Vs, BB, Parent)) {
    errs() << "Recurrence\n";
    return N;
  }

  if (allConstant(Vs)) {
    if (Value *Step = isConstantSequence(Vs)) {
      errs() << "Int Seq\n";
      return new IntSequenceNode(Vs, Step, BB, Parent);
    }
  }

  if (Node *N = buildAlternatingSequenceNode(Vs, BB, Parent)) {
    errs() << "Alt Seq\n";
    return N;
  }
  if (Node *N = buildConstExprNode(Vs, BB, Parent)) {
    errs() << "Const Expr\n";
    return N;
  }

  std::vector<ValueT *> Seq1;
  for (unsigned i = 0; i<Vs.size(); i+=2) {
    Seq1.push_back(Vs[i]);
  }

  std::vector<ValueT *> Seq2;
  for (unsigned i = 1; i<Vs.size(); i+=2) {
    Seq2.push_back(Vs[i]);
  }
  if (tempMatching(Seq1, BB, Parent) && tempMatching(Seq2, BB, Parent)) {
    errs() << "New Alternating Pattern:\n";
    for (auto *V : Seq1) {
      errs() << "1:"; V->dump();
    }
    for (auto *V : Seq2) {
      errs() << "2:"; V->dump();
    }
    //BB.dump();
  } 

  errs() << "Mismatching\n";
  for (auto *V : Vs) Inputs.insert(V);
  return new MismatchingNode(Vs,BB,Parent);
}
*/

template<typename ValueT>
Node *AlignedGraph::find(std::vector<ValueT *> &Vs) {
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

Node *AlignedGraph::find(Instruction *I) {
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

static void ReorderOperands(std::vector<Value*> &Operands, BasicBlock *BB) {
  std::unordered_map<const Value*,APInt> Ids;
  
  unsigned BitWidth = 64;
  if (Operands[0]->getType()->isIntegerTy()) {
    BitWidth = Operands[0]->getType()->getIntegerBitWidth();
  }
  unsigned i = 0;
  for (auto &I : *BB) {
    if (std::find(Operands.begin(),Operands.end(), &I)!=Operands.end())
      Ids[&I] = APInt(BitWidth,i);
    i++;
  }
  for (Value *V : Operands) {
    if (Ids.find(V)==Ids.end()) {
      if (auto *C = dyn_cast<ConstantInt>(V)) {
        Ids[V] = C->getValue();
      } else {
	Ids[V] = APInt::getMaxValue(BitWidth);
      }
    }
  }
  std::sort(Operands.begin(),Operands.end(), [&](const Value *A, const Value *B) -> bool {
    return Ids[A].slt(Ids[B]);
  });
}

/*
For a given binary operator, collect all neighboring instructions of the same opcode, composing a single reduction node.
This binary operator must be valid for reduction, i.e., must be associative, such as addition, multiplication, etc.
Every operand that is not part of the reduction node itself is an input value.

          /- phi
      /- + - input 
   - + 
  /   \- input
-+ 
  \
   - input

*/
template<typename ValueT>
Node *AlignedGraph::buildReduction(ValueT *V, Instruction *U, BasicBlock *BB, Node *Parent) {
  if (V==nullptr) return nullptr;
  BinaryOperator *BO = dyn_cast<BinaryOperator>(V);
  errs() << "Building reduction\n";
  U->dump();
  BO->dump();
  if (BO==nullptr) return nullptr;
  if (BO->getParent()!=BB) return nullptr;
  if (!ReductionNode::isValidOperation(BO)) return nullptr;

  PHINode *PHI = dyn_cast<PHINode>(U);

  std::vector<BinaryOperator*> BOs;
  std::vector<Value*> Vs;
  ReductionNode::collectValues(BO,PHI,BOs,Vs);

  if (BOs.size()<=1) return nullptr;

#ifdef TEST_DEBUG
  errs() << "BOs:\n";
  for (auto * V : BOs) V->dump();
  errs() << "Operands:\n";
  for (auto * V : Vs) V->dump();
#endif

  ReorderOperands(Vs, BB);

#ifdef TEST_DEBUG
  errs() << "Operands:\n";
  for (auto * V : Vs) V->dump();
  errs() << "ReductionNode\n"; 
#endif

  if (PHI) Inputs.insert(PHI);

  return new ReductionNode(BO, PHI, BOs, Vs, BB, Parent);
}


/*
For a given base address, Addr, the sequence
A[0], A[1], A[2], ...
can be simplified as
*A, A[1], A[2], ...
making it less obvious that these operations match.
In LLVM, the indexing operation is represented using GetElementPtr (GEP).
This special node tries to identify this pattern of GEP sequence.
*/
template<typename ValueT>
Node *AlignedGraph::buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {

  if (!isa<PointerType>(VL[0]->getType())) return nullptr;
  //auto *Ptr = getUnderlyingObject(VL[0]);
  //for (unsigned i = 1; i<VL.size(); i++)
  //  if (Ptr != getUnderlyingObject(VL[i])) return nullptr;

  //for (auto *V : VL) V->dump();

  std::vector<Value*> Indices;

  Value *Ptr = nullptr;
  GetElementPtrInst *RefGEP = nullptr;
  Type *Ty = nullptr;
  for (unsigned i = 0; i<VL.size(); i++) {
    if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (GEP->getParent()!=BB) return nullptr;
      //if (GEP->getPointerOperand()!=Ptr) return nullptr;
      if (GEP->getNumIndices()!=1) return nullptr; //strong restriction
      /*if (!GEP->hasIndices()) return nullptr;
      for (unsigned i = 1; i<GEP->getNumOperands()-1; i++) {
        Value *Idx = GEP->getOperand(i);
        auto *CIdx = dyn_cast<ConstantInt>(Idx);
	if (CIdx==nullptr) return nullptr;
	if (!CIdx->isZero()) return nullptr;
      }*/
      Value *Idx = GEP->getOperand(GEP->getNumOperands()-1);
      Ty = Idx->getType();
      RefGEP = GEP;
      Ptr = GEP->getPointerOperand();
      break;
    }
  }
  if (Ty==nullptr || !isa<IntegerType>(Ty)) return nullptr;

  errs() << "Ptr: "; Ptr->dump();

  bool AllGEPs = true;
  for (unsigned i = 0; i<VL.size(); i++) {
    if (Ptr==VL[i]) {
      AllGEPs = false;
      Indices.push_back(ConstantInt::get(Ty, 0));
    } else if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (GEP->getPointerOperand()!=Ptr) return nullptr;
      if (GEP->getNumIndices()!=1) return nullptr; //strong restriction
      if (GEP->getNumOperands()!=RefGEP->getNumOperands()) return nullptr;
      /*if (!GEP->hasIndices()) return nullptr;
      for (unsigned i = 1; i<GEP->getNumOperands()-1; i++) {
        Value *Idx = GEP->getOperand(i);
	if (RefGEP->getOperand(i)->getType()!=Idx->getType()) return nullptr;
        auto *CIdx = dyn_cast<ConstantInt>(Idx);
	if (CIdx==nullptr) return nullptr;
	if (!CIdx->isZero()) return nullptr;
      }*/
      Value *Idx = GEP->getOperand(GEP->getNumOperands()-1);
      Indices.push_back(Idx);
      if (Ty!=Idx->getType()) return nullptr;
    } else return nullptr;
  }

  errs() << "AllGEPs: " << AllGEPs << "\n";
  if (AllGEPs) return nullptr; //Should be a valid match then


  Inputs.insert(Ptr);
  return new GEPSequenceNode(VL, RefGEP, Ptr, Indices, BB, Parent);
}


template<typename ValueT>
Node *AlignedGraph::buildGEPSequence2(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  errs() << "GEPSeq2\n";
  if (!isa<PointerType>(VL[0]->getType())) return nullptr;
  //auto *Ptr = getUnderlyingObject(VL[0]);
  Type *Ty = nullptr;
  for (unsigned i = 1; i<VL.size(); i++) {
    auto *GEP1 = dyn_cast<GetElementPtrInst>(VL[i-1]);
    auto *GEP2 = dyn_cast<GetElementPtrInst>(VL[i]);
    if (GEP1==nullptr || GEP2==nullptr) return nullptr;
    Ty = matchGEPUntilLastType(GEP1,GEP2);
    if (Ty==nullptr) return nullptr;
  }
  Ty->dump();

  GetElementPtrInst *RefGEP = dyn_cast<GetElementPtrInst>(VL[0]);
  std::vector<Value*> Indices;

  for (unsigned i = 0; i<VL.size(); i++) {
    if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (GEP->getPointerOperand()!=RefGEP->getPointerOperand()) return nullptr;
      if (GEP->getNumOperands()!=RefGEP->getNumOperands()) return nullptr;
      if (!GEP->hasIndices()) return nullptr;
      for (unsigned i = 1; i<GEP->getNumOperands()-1; i++) {
        Value *Idx = GEP->getOperand(i);
	if (RefGEP->getOperand(i)->getType()!=Idx->getType()) return nullptr;
	if (RefGEP->getOperand(i)!=Idx) return nullptr;
      }
      Value *Idx = GEP->getOperand(GEP->getNumOperands()-1);
      Indices.push_back(Idx);
    }
  }

  if (Indices.empty()) return nullptr;

  if (!isa<ArrayType>(Ty)) {
    auto *STy = dyn_cast<StructType>(Ty);
    if (STy==nullptr) return nullptr;

    bool AllSameType = true;
    for (unsigned i = 1; i<STy->getNumElements(); i++) {
      if (STy->getElementType(0)!=STy->getElementType(i)) AllSameType = false;
    }

    if (!AllSameType) {
      Value *Step = isConstantSequence(Indices);
      if (Step==nullptr) return nullptr;
      auto *CStart = dyn_cast<ConstantInt>(Indices[0]);
      auto *CStep = dyn_cast<ConstantInt>(Step);
      if (CStart==nullptr) return nullptr;
      if (CStep==nullptr) return nullptr;
      if (CStep->isNegative()) return nullptr;
      APInt Min(CStart->getValue());
      APInt Max(CStart->getValue());
      for (auto *V : Indices) {
        auto *C = dyn_cast<ConstantInt>(V);
	APInt CInt(C->getValue());
	if (CInt.slt(Min)) Min = CInt;
	if (CInt.sgt(Max)) Max = CInt;
      }

      if (!CStep->isOne() || !CStart->isZero()) return nullptr;
      if (STy->getNumElements()<Indices.size()) return nullptr;
      for (unsigned i = 1; i<Indices.size(); i++) {
        if (STy->getElementType(0)!=STy->getElementType(i)) return nullptr;
      }
    }
  }

  errs() << "Indices:\n";
  for (auto *V : Indices) V->dump();

  //return nullptr;
  //Inputs.insert(Ptr);
  Inputs.insert(RefGEP->getPointerOperand());
  return new GEPSequenceNode(VL, RefGEP, nullptr, Indices, BB, Parent);
}

/*
 Identifies a sequence of alternating values of the same type, for example:
 V0, V1, V0, V1, V0, V1, ...
 In a loop with induction variable i starting from 0, can be represented by the expression:
 (i%2==0) ? V0 : V1
 V0 and V1 must be loop invariant, after loop rolling, i.e., they must be input values to the aligned graph.
*/
template<typename ValueT>
Node *AlignedGraph::buildAlternatingSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  if (VL.size()<2) return nullptr;

  Value *First = VL[0];
  Value *Second = VL[1];

  if (First->getType()!=Second->getType()) return nullptr;

  for (unsigned i = 2; i<VL.size(); i++) {
    if (VL[i] != VL[i%2]) return nullptr;
  }
  
  Inputs.insert(First);
  Inputs.insert(Second);

  return new AlternatingSequenceNode(VL, First, Second, BB, Parent);
}

static bool isAddition(Instruction *I, ScalarEvolution *SE) {
  if (I->getOpcode()==Instruction::Add) return true;
  if (I->getOpcode()==Instruction::Or) {
    //errs() << "Checking if represents addition:"; I->dump();

    Value *Op1 = I->getOperand(0);
    ConstantInt *Op2 = dyn_cast<ConstantInt>(I->getOperand(1));
    if (Op2==nullptr) return false;
    if (!isa<PHINode>(Op1)) return false;

    auto *AddRec = dyn_cast<SCEVAddRecExpr>(SE->getSCEV(Op1));
    if (AddRec==nullptr) return false;
    //errs() << "SCEV:"; AddRec->dump();

    auto *Start = dyn_cast<SCEVConstant>(AddRec->getStart());
    if (Start==nullptr) return false;
    //errs() << "Start:"; Start->dump();
    auto *Step = dyn_cast<SCEVConstant>(AddRec->getStepRecurrence(*SE));
    if (Step==nullptr) return false;
    //errs() << "Step:"; Step->dump();


    auto StartInt = Start->getAPInt();
    auto StepInt = Step->getAPInt();

    if (! StepInt.abs().isPowerOf2() ) return false;
    if (StepInt.abs().slt(Op2->getValue())) return false;
    StartInt &= Op2->getValue();
    //errs() << "AND: " << StartInt << "\n";
    if (!StartInt.isNullValue()) return false;

    errs() << "Is Addition\n";
    return true;
  }
  return false;
}

static bool isEquivalent(Instruction *I, unsigned opcode, ScalarEvolution *SE) {
  if (SE && opcode==Instruction::Add) return isAddition(I, SE);
  return (I->getOpcode()==opcode);
}

/*
For a given binary operator with a neutral element, we can have a sequence such as:
a0 + 0, a1 + 1, a2 + 2, ...
which can often be simplified as:
a0, a1 + 1, a2 + 2, ...
making it less obvious that these operations match.
This special node tries to identify this pattern of sequence of binary operation,
reconstructing the operation over the neutral element.
However, this is a general implementation that works for any sequence of left- and right-hand side operands:
a0 + b0, a1 + b1, ..., ai, ..., aj, ..., an + bn.
The sequence can have multiple mismatching terms that are then rewritten as an operation with the neutral operand, e.g., ai + 0 and aj + 0.
Note that any of the ak or bk terms can be anything.
The mismatching terms can also be anything, even another binary operation, as long as they have different opcodes to the main binary operation.
If the input sequence contains multiple binary operations, the most frequent one is selected.

It also allows binop sequences to contain mixed opcodes that are equivalent.
For example, it may contain 'or' and 'add' -- an 'or' operation can be used if no carry is needed in the binary addition.
*/
template<typename ValueT>
Node *AlignedGraph::buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  errs() << "BinOP?\n";
  VL[0]->dump();

  if (!isa<IntegerType>(VL[0]->getType())) return nullptr;
  std::map<unsigned, unsigned> OpcodeFreq;
  for (auto *V : VL) {
    auto *BO = dyn_cast<BinaryOperator>(V);
    if (BO) {
      OpcodeFreq[BO->getOpcode()]++;
    }
  }

  unsigned MaxOpcode = 0;
  unsigned MaxFreq = 0;
  for (auto &Pair : OpcodeFreq) {
    if (Pair.second > MaxFreq) {
      MaxOpcode = Pair.first;
      MaxFreq = Pair.second;
    }
  }
  if (!MaxOpcode)  return nullptr;
  
  std::vector<Value*> LeftOperands;
  std::vector<Value*> RightOperands;

  BinaryOperator *BinOpRef = nullptr;
  for (unsigned i = 0; i<VL.size(); i++) {
    auto *BinOp = dyn_cast<BinaryOperator>(VL[i]);

    //if (BinOp==nullptr || BinOp->getParent()!=(&BB))
    
    //if (BinOp==nullptr || BinOp->getOpcode()!=MaxOpcode) {
    if (BinOp==nullptr || (!isEquivalent(BinOp, MaxOpcode, SE)) ) {
      LeftOperands.push_back(VL[i]);
      RightOperands.push_back(nullptr);
      continue;
    }
    if (BinOpRef==nullptr && BinOp->getOpcode()==MaxOpcode) BinOpRef = BinOp;

    if (BinOp->isCommutative() && isa<Constant>(BinOp->getOperand(0))) {
      LeftOperands.push_back(BinOp->getOperand(1));
      RightOperands.push_back(BinOp->getOperand(0));
    } else {
      LeftOperands.push_back(BinOp->getOperand(0));
      RightOperands.push_back(BinOp->getOperand(1));
    }
  }

  if (BinOpRef==nullptr) return nullptr;

  Type *Ty = BinOpRef->getType();

  Value *Neutral = nullptr;
  switch(BinOpRef->getOpcode()) {
  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Or:
  case Instruction::Xor:
    Neutral = ConstantInt::get(Ty, 0);
    break;
  case Instruction::Mul:
  case Instruction::UDiv:
  case Instruction::SDiv:
  case Instruction::And:
    Neutral = ConstantInt::get(Ty, 1);
    break;
  default:
    return nullptr;
  }

  for (unsigned i = 0; i<RightOperands.size(); i++) {
    if (RightOperands[i]==nullptr) RightOperands[i] = Neutral;
  }

  return new BinOpSequenceNode(VL, BinOpRef, LeftOperands, RightOperands, BB, Parent);
}

template<typename ValueT>
Node *AlignedGraph::buildRecurrenceNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {
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
      if (N->getValidInstruction(i-1)!=Vs[i]) Result.erase(N);
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

  Inputs.insert(Vs[0]);

  auto *RN = new RecurrenceNode(Vs, Vs[0], BB, Parent);
  RN->pushChild(N);

  return RN;
}


/*
Create a node of matching constant expressions.
This is similar to the node of matching instructions.
While constant expressions can be solved at compile time,
we generate conventional instructions in the rolled loop.
Note that they are not computing identical values.
Therefore, the alternative would be to keep them as individual constant expressions
but generate code to import their values from the loop, which has
a greater cost in terms of code size.
Constant expressions are often used for computing indexes of global arrays, using GEPOperators.
*/
template<typename ValueT>
Node *AlignedGraph::buildConstExprNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {

  ConstantExpr *CExpr = dyn_cast<ConstantExpr>(Vs[0]);
  if (CExpr==nullptr) return nullptr;

  std::unordered_set<Value*> UniqueValues;
  for (unsigned i = 0; i<Vs.size(); i++) {
    UniqueValues.insert(Vs[i]);
    auto *CV = dyn_cast<ConstantExpr>(Vs[i]);
    if (CV==nullptr) return nullptr;
    if (CV->getOpcode()!=CExpr->getOpcode()) return nullptr;
    if (CV->getType()!=CExpr->getType()) return nullptr;
    if (CV->getNumOperands()!=CExpr->getNumOperands()) return nullptr;
    for (unsigned i = 0; i<CExpr->getNumOperands(); i++) {
      if (CV->getOperand(i)->getType()!=CExpr->getOperand(i)->getType()) return nullptr;
    }

    auto *GEP1 = dyn_cast<GEPOperator>(CExpr);
    auto *GEP2 = dyn_cast<GEPOperator>(CV);
    if (GEP1 && GEP2) {
      errs() << "Checking Constant GEP:"; GEP2->dump();
      if (!matchGEP(GEP1,GEP2)) return nullptr;
    }
  }
  if (UniqueValues.size()!=Vs.size()) return nullptr;

  errs() << "Matching Constant Exprs:\n";
  for (auto *V : Vs) V->dump();

  return new ConstantExprNode(Vs,BB,Parent);
}


template<typename ValueT>
static bool tempMatching(std::vector<ValueT*> Vs, BasicBlock *BB, Node *Parent) {
  bool AllSame = true;
  bool Matching = true;
  std::unordered_set<Value*> UniqueValues;
  UniqueValues.insert(Vs[0]);
  if (auto *I = dyn_cast<Instruction>(Vs[0])) {
    if (I->getParent()!=(BB)) Matching = false;
  }
  for (unsigned i = 1; i<Vs.size(); i++) {
    UniqueValues.insert(Vs[i]);
    AllSame = AllSame && Vs[i]==Vs[0];
    Matching = Matching && match(Vs[i-1],Vs[i]);
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      if (I->getParent()!=(BB)) Matching = false;
    }
  }
  Matching = Matching && (UniqueValues.size()==Vs.size());

  return Matching;
}

template<typename ValueT>
Node *AlignedGraph::createNode(std::vector<ValueT*> Vs, BasicBlock *BB, Node *Parent) {
  
  errs() << "Creating Node\n";
  //for (auto *V : Vs) {
  //  if (isa<Function>(V)) errs() << "Function: " << V->getName() << "\n";
  //  else V->dump();
  //}
  bool AllSame = true;
  bool Matching = true;
  bool HasSideEffect = false;
  std::unordered_set<Value*> UniqueValues;
  UniqueValues.insert(Vs[0]);
  if (auto *I = dyn_cast<Instruction>(Vs[0])) {
    if (I->getParent()!=BB) Matching = false;
    HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
  }
  for (unsigned i = 1; i<Vs.size(); i++) {
    UniqueValues.insert(Vs[i]);
    AllSame = AllSame && Vs[i]==Vs[0];
    Matching = Matching && match(Vs[i-1],Vs[i]);
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      if (I->getParent()!=BB) Matching = false;
      HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
    }
  }
  errs() << "Match: " << Matching << "\n";
  errs() << UniqueValues.size() << " x " << Vs.size() << "\n";

  Matching = Matching && (UniqueValues.size()==Vs.size() || (!HasSideEffect));
  errs() << "Final Match: " << Matching << "\n";

  if (AllSame) {
    errs() << "All the Same\n";
    Inputs.insert(Vs[0]);
    return new IdenticalNode(Vs,BB,Parent);
  }
  if (Matching) {
    errs() << "Matching\n";
    return new MatchingNode(Vs,BB,Parent);
  }
  if (Node *N = buildGEPSequence(Vs, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }
  if (Node *N = buildGEPSequence2(Vs, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }

  if (Node *N = buildBinOpSequenceNode(Vs, BB, Parent)) {
    errs() << "BinOp Seq\n";
    return N;
  }
  if (Node *N = buildRecurrenceNode(Vs, BB, Parent)) {
    errs() << "Recurrence\n";
    return N;
  }

  if (allConstant(Vs)) {
    if (Value *Step = isConstantSequence(Vs)) {
      errs() << "Int Seq\n";
      return new IntSequenceNode(Vs, Step, BB, Parent);
    }
  }

  if (Node *N = buildAlternatingSequenceNode(Vs, BB, Parent)) {
    errs() << "Alt Seq\n";
    return N;
  }
  if (Node *N = buildConstExprNode(Vs, BB, Parent)) {
    errs() << "Const Expr\n";
    return N;
  }

  std::vector<ValueT *> Seq1;
  for (unsigned i = 0; i<Vs.size(); i+=2) {
    Seq1.push_back(Vs[i]);
  }

  std::vector<ValueT *> Seq2;
  for (unsigned i = 1; i<Vs.size(); i+=2) {
    Seq2.push_back(Vs[i]);
  }
  if (tempMatching(Seq1, BB, Parent) && tempMatching(Seq2, BB, Parent)) {
    errs() << "New Alternating Pattern:\n";
    for (auto *V : Seq1) {
      errs() << "1:"; V->dump();
    }
    for (auto *V : Seq2) {
      errs() << "2:"; V->dump();
    }
    //BB.dump();
  } 

  errs() << "Mismatching\n";
  for (auto *V : Vs) Inputs.insert(V);
  return new MismatchingNode(Vs,BB,Parent);
}

void AlignedGraph::growGraph(Node *N, BasicBlock *BB, std::set<Node*> &Visited) {
  if (Visited.find(N)!=Visited.end()) return;
  Visited.insert(N);

  auto growGraphNode = [&](auto &Vs, Node *N, BasicBlock *BB, std::set<Node*> &Visited) {
       Node *Child = find(Vs);
       if (Child==nullptr) {
         Child = createNode(Vs, BB, N);
         this->addNode(Child);
         N->pushChild(Child);
         growGraph(Child, BB, Visited);
       } else N->pushChild(Child);
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
         growGraphNode(Vs, N, BB, Visited);
       }
    } break;
    case NodeType::REDUCTION: {
       auto &Vs = ((ReductionNode*)N)->getOperands();
       growGraphNode(Vs, N, BB, Visited);
    } break;
    case NodeType::GEPSEQ: {
       auto &Vs = ((GEPSequenceNode*)N)->getIndices();
       growGraphNode(Vs, N, BB, Visited);
    } break;
    case NodeType::BINOP: {
       auto &V0 = ((BinOpSequenceNode*)N)->getLeftOperands();
       growGraphNode(V0, N, BB, Visited);

       auto &V1 = ((BinOpSequenceNode*)N)->getRightOperands();
       growGraphNode(V1, N, BB, Visited);
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
        growGraphNode(Vs, N, BB, Visited);
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
        growGraphNode(Vs, N, BB, Visited);
      }
    } break;
    default: break;
  }
}

Instruction *AlignedGraph::getStartingInstruction(BasicBlock &BB) {
  for (Instruction &I : BB) {
    if (ValuesInNode.count(&I)) {
      return &I;
    }
  }
  return nullptr;
}

/*
Instruction *AlignedGraph::getEndingInstruction(BasicBlock &BB) {
  return dyn_cast<Instruction>(Root->getValue(Root->size()-1));
}
*/

bool AlignedGraph::invalidDependence(Value *V, std::unordered_set<Value*> &Visited) {
  if (Visited.find(V)!=Visited.end()) return false;
  if (isa<PHINode>(V)) return false;

  if (isa<Instruction>(V) && contains(V)) {
	  errs() << "Invalid: "; V->dump();
	  return true;
  }
  Visited.insert(V);

  Instruction *I = dyn_cast<Instruction>(V);
  if (I && I->getParent()==getBlock()) {
    for (unsigned i = 0 ; i<I->getNumOperands(); i++) {
      if (invalidDependence(I->getOperand(i), Visited)) return true;
    }
  }

  return false;
}


bool AlignedGraph::isSchedulable(BasicBlock &BB) {

  if (Root==nullptr) return false;
  std::unordered_set<Value*> Visited;
  for (auto *V : Inputs) {
    if (invalidDependence(V,Visited)) {
#ifdef TEST_DEBUG
      errs() << "Invalid dependence found!\n";
#endif
      return false;
    }
  }

  //set of all instructions that must be covered in the next steps
  std::set<Instruction *> AllInsts;
  for (Value *V : ValuesInNode) {
    if (Instruction *I = dyn_cast<Instruction>(V)) AllInsts.insert(I);
  }
  
  errs() << "Computing order of nodes for each lane\n";
  //1. Reconstruct the order the nodes appear in each lane
  //2. Identify instructions that appear in-between this code that
  //could prevent us from scheduling the rolled loop.
  //Any instruction, from outside the aligned graph, is an impediment if it may access a memory location
  //or may have side effects.
  std::map<unsigned, std::vector<Node*> > NodeSchedulingOrder;
  for (Instruction *I = getStartingInstruction(BB); (I!=nullptr && !I->isTerminator()) ; I = I->getNextNode()) {
    if (AllInsts.empty()) break; //finished analysis

    Node *N = find(I);
    if (N==nullptr || N->getNodeType()==NodeType::MISMATCH) {
      if (I->mayReadOrWriteMemory() || I->mayHaveSideEffects()) {
        errs() << "Read/Write memory found in between\n";
	I->dump();
	return false;
      }
    } else {
      AllInsts.erase(I);
      for (unsigned i = 0; i<N->size(); i++) {
        if (N->getValidInstruction(i)==I) {
          //only add unique nodes to the correct lane
          auto &LaneRef = NodeSchedulingOrder[i];
          if (std::find(LaneRef.begin(), LaneRef.end(), N)==LaneRef.end()) {
            LaneRef.push_back(N);
          }
        }
      }
    }
  }

  //Make sure all lanes have the same scheduling order.
  //An aligned graph is not schedulable if nodes with
  //critical instructions appear in different order in
  //different lanes. For example:
  // t00 = dummy0();
  // t01 = dummy1();
  // add0 = add t00, t01
  // store add0, ...
  // t10 = dummy1(); //different order
  // t11 = dummy0(); //different order
  // add1 = add t10, t11
  // store add1, ...
  // The code bellow verifies if all lanes have critical nodes
  // appearing in the same order as in the first lane.
  unsigned Indices[NodeSchedulingOrder.size()];
  for (unsigned i = 0; i<NodeSchedulingOrder.size(); i++) {
    Indices[i] = 0;
  }
  for (unsigned n = 0; n<NodeSchedulingOrder[0].size(); n++) {
    Indices[0] = n;
    if (NodeSchedulingOrder[0][n]->mustKeepOrder()) {
      for (unsigned i = 1; i<NodeSchedulingOrder.size(); i++) {
	while (Indices[i]<NodeSchedulingOrder[i].size()) {
	  unsigned nidx = Indices[i];
          Indices[i]++;

	  if (nidx >= NodeSchedulingOrder[i].size()) {
            errs() << "Found different order of nodes: index out of bound\n";
	    return false;
	  }
          if (NodeSchedulingOrder[i][nidx]->mustKeepOrder()) {
            errs() << "Hit: " << NodeSchedulingOrder[i][nidx]->getString() << "\n";
            NodeSchedulingOrder[i][nidx]->getValue(i)->dump();
	    if (NodeSchedulingOrder[i][nidx]!=NodeSchedulingOrder[0][n]) {
              errs() << "Found different order of nodes: nodes mismatching\n";
	      return false;
	    }
	    break;
	  }
	}
      }
    }
  }
  //all nodes that must be kept in order, should have been exausted
  for (unsigned i = 1; i<NodeSchedulingOrder.size(); i++) {
    unsigned nidx = Indices[i];
    Indices[i]++;
    if (nidx >= NodeSchedulingOrder[i].size()) continue;
    if (NodeSchedulingOrder[i][nidx]->mustKeepOrder()) {
      return false;
    }
  }

  //Write down the final scheduling order.
  //Missing nodes will be generated recursively.
  for (Node *N : NodeSchedulingOrder[0]) {
    SchedulingOrder.push_back(N);
  }

  //TODO: make sure the scheduling order is valid according to the graph.

  errs() << "Schedulable: " << true << "\n";
  return true;
}

void CodeGenerator::generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder) {
  LLVMContext &Context = F.getContext();

  std::set<unsigned> NeedExtract;

  for (unsigned i = 0; i<N->size(); i++) {
    auto *I = N->getValidInstruction(i);
    if (I==nullptr) continue;
    if (I->getParent()!=(&BB)) continue;
    for (auto *U : I->users()) {
      if (!G.contains(U)) {
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
  errs() << "Extracting: "; NewI->dump();
#endif

  if (NeedExtract.size()==1 && N->getNodeType()==NodeType::REDUCTION) {
    ReductionNode *RN = (ReductionNode*)N;
    if (RN->getBinaryOperator()==RN->getValidInstruction(*NeedExtract.begin())) {
      return;
    }
  }

  BasicBlock &Entry = F.getEntryBlock();
  IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

  Type *IndVarTy = IntegerType::get(Context, 8);
  Value *ArrPtr = ArrBuilder.CreateAlloca(NewI->getType(), ConstantInt::get(IndVarTy, N->size()));
  CreatedCode.push_back(ArrPtr);

  auto *GEP = Builder.CreateGEP(ArrPtr, IndVar);
  CreatedCode.push_back(GEP);
  auto *Store = Builder.CreateStore(NewI, GEP);
  CreatedCode.push_back(Store);

  IRBuilder<> ExitBuilder(Exit);
  for (unsigned i : NeedExtract) {
    Instruction *I = N->getValidInstruction(i);
    auto *GEP = ExitBuilder.CreateGEP(ArrPtr, ConstantInt::get(IndVarTy, i));
    CreatedCode.push_back(GEP);
    auto *Load = ExitBuilder.CreateLoad(GEP);
    CreatedCode.push_back(Load);
    Extracted[I] = Load;
  }

}


Value *CodeGenerator::generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder) {
  Module *M = F.getParent();
  LLVMContext &Context = F.getContext();

#ifdef TEST_DEBUG
  errs() << "Mismatched Values:\n";
  for (Value *V : VL) {
    if (isa<Instruction>(V)) errs() << "inst: ";
    else if (isa<ConstantInt>(V)) errs() << "int: ";
    else if (isa<ConstantExpr>(V)) errs() << "constexpr: ";
    else if (isa<Constant>(V)) errs() << "const: ";
    else if (isa<Argument>(V)) errs() << "arg: ";
    else errs() << "val: ";

    V->dump();
  }
#endif
  
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
      CreatedCode.push_back(GArray);
      //GArray->setInitializer(ConstArray);
      SmallVector<Value*,8> Indices;
      Type *IndVarTy = IntegerType::get(Context, 8);
      Indices.push_back(ConstantInt::get(IndVarTy, 0));
      Indices.push_back(IndVar);

      errs() << "Created array: "; GArray->dump();

      auto *GEP = Builder.CreateGEP(GArray, Indices);
      CreatedCode.push_back(GEP);

      auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
      CreatedCode.push_back(Load);

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
    CreatedCode.push_back(ArrPtr);
    
    errs() << "Created array: "; ArrPtr->dump();

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
  return VL[0]; //TODO: return nullptr?
}

Value *CodeGenerator::cloneGraph(Node *N, IRBuilder<> &Builder) {
  if (NodeToValue.find(N)!=NodeToValue.end()) return NodeToValue[N];

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
      if (I) {
        Instruction *NewI = I->clone();
        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,nullptr);
        }
        NodeToValue[N] = NewI;

        std::vector<Value*> Operands;
        for (unsigned i = 0; i<N->getNumChildren(); i++) {
          Operands.push_back(cloneGraph(N->getChild(i), Builder));
        }

#ifdef TEST_DEBUG
        errs() << "Operands done!\n";
#endif

        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        NewI->getAllMetadata(MDs);
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }

	if (!MatchAlignment) {
          if (auto *LI = dyn_cast<LoadInst>(NewI)) {
            LI->setAlignment(Align());
	  }
	  else if (auto *SI = dyn_cast<StoreInst>(NewI)) {
            SI->setAlignment(Align());
	  }
	}

        Builder.Insert(NewI);
        CreatedCode.push_back(NewI);

        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }
#ifdef TEST_DEBUG
        errs() << "Generated: "; NewI->dump();
#endif

        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
            //if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
	    Garbage.insert(I);
	  }
	}
        
        generateExtract(N, NewI, Builder);

#ifdef TEST_DEBUG
	errs() << "Gen: "; NewI->dump();
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
        CreatedCode.push_back(NewI);

        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }

	/*
        for (unsigned i = 0; i<N->size(); i++) {
          if (auto *I = N->getValidInstruction(i)) {
	    Garbage.insert(I);
	  }
	}
        
        generateExtract(N, NewI, Builder);
        */

#ifdef TEST_DEBUG
        errs() << "Generated: "; NewI->dump();
#endif

        return NewI;
      } else return N->getValue(0); //TODO: maybe an assert false
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
      Value *IndVarIdx = cloneGraph(GN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing GEPSEQ\n";
#endif
      //auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getPointerOperand(), IndVarIdx));
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(Ptr, IndVarIdx));
      /*
      auto *GEP = GN->getReference()->clone();
      GEP->setOperand(0, GN->getPointerOperand());
      GEP->setOperand(GN->getReference()->getNumOperands()-1, IndVarIdx);
      Builder.Insert(GEP);
      */
      CreatedCode.push_back(GEP);
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
      CreatedCode.push_back(NewI);

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
      errs() << "Gen: "; NewI->dump();
#endif
      return NewI;
    }
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

      /*
      for (int i = RN->size(); i>0; i--) {
        if (auto *I = RN->getValidInstruction((int)i - 1)) {
          if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) {
            Garbage.push_back(I);
	  }
        }
      }
      */
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
    case NodeType::INTSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating INTSEQ\n";
#endif

      Value *NewV = nullptr;

      auto *ISN = (IntSequenceNode*)N;
      auto *StartValue = dyn_cast<ConstantInt>(ISN->getStart());
      auto *StepValue = dyn_cast<ConstantInt>(ISN->getStep());
      Value *CastIndVar = IndVar;
      if (CachedCastIndVar.find(StartValue->getType())!=CachedCastIndVar.end()) {
        CastIndVar = CachedCastIndVar[StartValue->getType()];
      } else {
	auto *CIndVarI = Builder.CreateIntCast(IndVar, StartValue->getType(), false);
	if (CIndVarI!=IndVar) {
          CreatedCode.push_back(CastIndVar);
          CachedCastIndVar[StartValue->getType()] = CIndVarI;
          CastIndVar = CIndVarI;
	}
      }
      if (StartValue->isZero() && StepValue->isOne()){
        NewV = CastIndVar;
      } else if (StepValue->isZero()){
        NewV = StartValue;
      } else {
	Value *Factor = CastIndVar;
	if (!StepValue->isOne()) {
          auto *Mul = Builder.CreateMul(CastIndVar, StepValue);
          CreatedCode.push_back(Mul);
	  Factor = Mul;
	}
	auto *Add = Builder.CreateAdd(Factor, StartValue);
        CreatedCode.push_back(Add);
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
      return NewV;
    }
    case NodeType::MISMATCH: {
#ifdef TEST_DEBUG
      errs() << "Generating Mismatch\n";
#endif
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

bool CodeGenerator::generate(SeedGroups &Seeds) {
  LLVMContext &Context = BB.getParent()->getContext();

    //BB.dump();
  if (G.Root==nullptr) return false;
  if (G.Root->getNodeType()==NodeType::MISMATCH) return false;
  if (G.Root->getNodeType()==NodeType::MULTI) {
    for (int i = 0; i<G.Root->getNumChildren(); i++) {
      if (G.Root->getChild(i)->getNodeType()==NodeType::MISMATCH) {
        errs() << "MultiNode cannot have a mismatching child\n";
        return false;
      }
    }
  }

  std::vector<BasicBlock*> SuccBBs;
  for (auto It = succ_begin(&BB), E = succ_end(&BB); It!=E; It++) SuccBBs.push_back(*It);

  PreHeader = BasicBlock::Create(Context, "rolled.pre", BB.getParent());

  Header = BasicBlock::Create(Context, "rolled.loop", BB.getParent());

  IRBuilder<> Builder(Header);

  Type *IndVarTy = IntegerType::get(Context, 8);

  IndVar = Builder.CreatePHI(IndVarTy, 0);
  CreatedCode.push_back(IndVar);

  Exit = BasicBlock::Create(Context, "rolled.exit", BB.getParent());

  errs() << "Loop Rolling: " << BB.getParent()->getName() << "\n";
#ifdef TEST_DEBUG
  errs() << "Generating tree\n";
#endif
  for (Node *N : G.SchedulingOrder) {
    cloneGraph(N, Builder);
  }
  cloneGraph(G.Root, Builder);
#ifdef TEST_DEBUG
  errs() << "Graph code generated!\n";
#endif
 
  bool HasRecurrence = false;
  int HasMismatch = 0;
  //Late generation of recurrences
  for (Node *N : G.Nodes) {
    if (N->getNodeType()==NodeType::RECURRENCE) {
      HasRecurrence = true;
      //update PHI
      PHINode *PHI = dyn_cast<PHINode>(NodeToValue[N]);
      errs() << "PHI: recurrence " << Header->getName() << ",";NodeToValue[N->getChild(0)]->dump();
      PHI->addIncoming(NodeToValue[N->getChild(0)],Header);
    } else if (N->getNodeType()==NodeType::MISMATCH) {
      HasMismatch++;
    }
  }

#ifdef TEST_DEBUG
  errs() << "Root:\n";
  for (auto *V : G.Root->getValues()) V->dump();
  errs() << "Root size: " << G.Root->size() << "\n";
#endif

  auto *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  CreatedCode.push_back(Add);


  Value *Cond = nullptr;
  if (AltSeqCmp && G.Root->size()==2) {
    Cond = AltSeqCmp;
  } else {
    auto *CondI = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, G.getWidth()));
    CreatedCode.push_back(CondI);

    Cond = CondI;
  }

  auto &DL = BB.getParent()->getParent()->getDataLayout();
  TargetTransformInfo TTI(DL);

  size_t SizeOriginal = EstimateSize(Garbage,DL,&TTI);
  size_t SizeModified = 0;
  SizeModified += 4*EstimateSize(PreHeader,DL,&TTI); 
  SizeModified += EstimateSize(Header,DL,&TTI); 
  SizeModified += 2*EstimateSize(Exit,DL,&TTI); 

  bool Profitable = (SizeOriginal > SizeModified + SizeThreshold) && (HasMismatch<4);
  //BB.dump();
  //
  errs() << G.getDotString() << "\n";

  //PreHeader->dump();
  //Header->dump();
  //Exit->dump();

  errs() << "Gains: " << SizeOriginal << " - " << SizeModified << " = " << ( ((int)SizeOriginal) - ((int)SizeModified) ) << "; ";
  errs() << "Width: " << G.Root->size() << "; ";
  if (G.Root->getNodeType() == NodeType::REDUCTION) errs() << "Reduction ";
  if (HasRecurrence) errs() << "Recurrence ";

  if (Profitable) {
    errs() << "Profitable; ";
    //errs() << G.getDotString() << "\n";
  } else errs() << "Unprofitable; ";
  errs() << BB.getParent()->getName() << "\n";

  //if (false) {
  if (AlwaysRoll || Profitable) {
	//std::string FileName = std::string("/tmp/roll.") + F.getParent()->getSourceFileName() + std::string(".") + F.getName().str();
	//FileName += "." + BB.getName().str() + ".dot";
	//G.writeDotFile(FileName);
#ifdef TEST_DEBUG
    //BB.dump();
#endif

    IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),PreHeader);
    IndVar->addIncoming(Add,Header);

    auto *Br = Builder.CreateCondBr(Cond,Header,Exit);
    CreatedCode.push_back(Br);

    Builder.SetInsertPoint(PreHeader);
    Builder.CreateBr(Header);

    Instruction *InstSplitPt = G.getStartingInstruction(BB);
    if (InstSplitPt==nullptr) {
      return false;
    }
    auto *EndPt = BB.getTerminator();

    //copy instructions to the Exit block
    Builder.SetInsertPoint(Exit);

    while (InstSplitPt!=EndPt) {
      auto *I = InstSplitPt;
      InstSplitPt = InstSplitPt->getNextNode();
      if (!isa<PHINode>(I) && !G.dependsOn(I)) {
        I->removeFromParent();
        Builder.Insert(I);
      }
    }
    //move terminator to the exit block
    InstSplitPt->removeFromParent();
    Builder.Insert(InstSplitPt);


    Builder.SetInsertPoint(&BB);
    Builder.CreateBr(PreHeader);

    for (auto &Pair : Extracted) {
      Pair.first->replaceAllUsesWith(Pair.second);
    }

    //BB.dump();
    //PreHeader->dump();
    //Header->dump();
    //Exit->dump();
 
    for (auto It = Exit->rbegin(), E = Exit->rend(); It!=E; ) {
      Instruction *I = &*It;
      It++;
      if (Garbage.count(I)) {
	 //Garbage.erase(I);
	 //errs() << "Removed from " << I->getParent()->getName() << ": "; I->dump();
	 Seeds.remove(I);
	 I->eraseFromParent();
      }
    }
    for (auto It = BB.rbegin(), E = BB.rend(); It!=E; ) {
      Instruction *I = &*It;
      It++;
      if (Garbage.count(I)) {
	 //Garbage.erase(I);
	 //errs() << "Removed from " << I->getParent()->getName() << ": "; I->dump();
	 Seeds.remove(I);
	 I->eraseFromParent();
      }
    }
    //for (auto *V : Garbage) {
    //  errs() << "Garbage: "; V->dump();
    //}
    
    for (BasicBlock *Succ : SuccBBs) {
      for (Instruction &I : *Succ) { //TODO: run only over PHIs
        if (auto *PHI = dyn_cast<PHINode>(&I)) {
          PHI->replaceIncomingBlockWith(&BB,Exit);
        }
      }
    }

#ifdef TEST_DEBUG
    errs() << "Done!\n";

    //BB.dump();
    //PreHeader->dump();
    //Header->dump();
    //Exit->dump();
    //
    
    errs() << "NodeTypeFreq;\n";
    std::map<NodeType, unsigned> NodeFreq;
    for (Node *N : G.Nodes) {
      NodeFreq[N->getNodeType()]++;
    }
   // MATCH, IDENTICAL, BINOP, GEPSEQ, INTSEQ, ALTSEQ, CONSTEXPR, REDUCTION, RECURRENCE, MISMATCH, MULTI
    errs() << "MISMATCH: " << NodeFreq[NodeType::MISMATCH] << "\n";
    errs() << "MATCH: " << NodeFreq[NodeType::MATCH] << "\n";
    errs() << "IDENTICAL: " << NodeFreq[NodeType::IDENTICAL] << "\n";
    errs() << "CONSTEXPR: " << NodeFreq[NodeType::CONSTEXPR] << "\n";
    errs() << "BINOP: " << NodeFreq[NodeType::BINOP] << "\n";
    errs() << "INTSEQ: " << NodeFreq[NodeType::INTSEQ] << "\n";
    errs() << "ALTSEQ: " << NodeFreq[NodeType::ALTSEQ] << "\n";
    errs() << "GEPSEQ: " << NodeFreq[NodeType::GEPSEQ] << "\n";
    errs() << "REDUCTION: " << NodeFreq[NodeType::REDUCTION] << "\n";
    errs() << "RECURRENCE: " << NodeFreq[NodeType::RECURRENCE] << "\n";
    errs() << "MULTI: " << NodeFreq[NodeType::MULTI] << "\n";
#endif
    if ( verifyFunction(*BB.getParent()) ) {
      errs() << "Broken Function!!\n";
      BB.getParent()->dump();
    }
    return true;
  } else {
    //errs() << "Unprofitable\n";

    DeleteDeadBlock(Exit);
    DeleteDeadBlock(Header);
    DeleteDeadBlock(PreHeader);
    return false;
  }
}

static BinaryOperator *getPossibleReduction(Value *V) {
#ifdef TEST_DEBUG
  //errs() << "looking for reduction\n";
#endif
  if (V==nullptr) return nullptr;
  BinaryOperator *BO = dyn_cast<BinaryOperator>(V);
  if (BO==nullptr) return nullptr;
  //BO->dump();
  if (!ReductionNode::isValidOperation(BO)) return nullptr;
  BinaryOperator *BO1 = dyn_cast<BinaryOperator>(BO->getOperand(0));
  BinaryOperator *BO2 = dyn_cast<BinaryOperator>(BO->getOperand(1));
  unsigned PossibleReduction = 0;
  if (BO1 && BO1->getOpcode()==BO->getOpcode()) PossibleReduction += 1;
  if (BO2 && BO2->getOpcode()==BO->getOpcode()) PossibleReduction += 1;
  if (PossibleReduction==0) return nullptr;
  //errs() << "Found\n";
  return BO;
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

      if (BinaryOperator *BO = getPossibleReduction(SI->getValueOperand())) {
#ifdef TEST_DEBUG
        //errs() << "Possible reduction\n";
	//BO->dump();
#endif
        Seeds.Reductions[BO] = &I;
      }
    }
    else if (auto *CI = dyn_cast<CallInst>(&I)) {
      //if (CI->getNumUses()>0) continue;
      Function *Callee = CI->getCalledFunction();
      if (Callee) {
        bool Valid = !Callee->isVarArg();
        if (Intrinsic::ID ID = (Intrinsic::ID)Callee->getIntrinsicID()) {
          switch(ID) {
          case Intrinsic::lifetime_start:
          case Intrinsic::lifetime_end:
            Valid = false;
            break;
          }
        }
        if (Valid) {
          Seeds.Calls[Callee].push_back(CI);
        }
      }
      for (unsigned i = 0; i<CI->getNumArgOperands(); i++) {
        if (BinaryOperator *BO = getPossibleReduction(CI->getArgOperand(i))) {
#ifdef TEST_DEBUG
          //errs() << "Possible reduction\n";
	  //BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        }
      }
    }
    else if (auto *CB = dyn_cast<CallBase>(&I)) {
      for (unsigned i = 0; i<CB->getNumArgOperands(); i++) {
        if (BinaryOperator *BO = getPossibleReduction(CB->getArgOperand(i))) {
#ifdef TEST_DEBUG
          //errs() << "Possible reduction\n";
	  //BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        }
      }
    }
    else if (auto *Br = dyn_cast<BranchInst>(&I)) {
      if (Br->isConditional()) {
        if (BinaryOperator *BO = getPossibleReduction(Br->getCondition())) {
#ifdef TEST_DEBUG
          //errs() << "Possible reduction\n";
	  //BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        }
      }
    }
    else if (auto *Ret = dyn_cast<ReturnInst>(&I)) {
      if (BinaryOperator *BO = getPossibleReduction(Ret->getReturnValue())) {
#ifdef TEST_DEBUG
        //errs() << "Possible reduction\n";
	//BO->dump();
#endif
        Seeds.Reductions[BO] = &I;
      }
    }
    else if (auto *PHI = dyn_cast<PHINode>(&I)) {
      //if (PHI->getNumIncomingValues()!=2) continue;
      PHI->dump();
      if (PHI->getBasicBlockIndex(PHI->getParent())>=0) {
        Value *V = PHI->getIncomingValueForBlock(PHI->getParent());
	V->dump();
        if (BinaryOperator *BO = getPossibleReduction(V)) {
#ifdef TEST_DEBUG
          errs() << "Possible reduction\n";
	  BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        }
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


bool LoopRoller::attemptRollingSeeds(BasicBlock &BB) {
    bool Changed = false;
    
    for (auto &Pair : Seeds.Stores) {
      bool Valid = true;
      errs() << "Attempting Group:\n";
      for (Instruction *I : Pair.second) {
	//I->dump();
        if (I->getNumUses()) {
	    Valid = false;
        }
      }
      if (Valid && Pair.second.size()>1) {
	//errs() << "Looking for groups\n";

	MultiNode *MN = new MultiNode(&BB);
	MN->addGroup(Pair.second);
	Instruction *I = Pair.second[0];
	I = I->getNextNode();
	for (; I!=Pair.second[1] && !I->isTerminator(); I=I->getNextNode()) {
	  auto *Group = Seeds.getGroupWith(I);
	  bool Valid = false;
	  if (Group && Pair.second.size()==Group->size()) {
	    if ( (*Group)[0]!=I ) continue; //accept only groups in order
	    Valid = true;
	    for (Instruction *OtherI : (*Group)) {
              if (OtherI->getNumUses()) {
		Valid = false;
	      }
	    }
	  }
	  if (Valid) {
	    MN->addGroup(*Group);
            //errs() << "Group:\n";
	    //for (Instruction *OtherI : (*Group)) {
	    //  OtherI->dump();
	    //}
	  }
	}

	if (MN->getNumGroups()>1) {
	  AlignedGraph G(MN,&BB,SE);
          if (G.isSchedulable(BB)) {
  	    NumAttempts++;
            CodeGenerator CG(F, BB, G);
	    bool HasRolled = CG.generate(Seeds);
            Changed = Changed || HasRolled;
	    if (HasRolled) NumRolledLoops++;
          }
          G.destroy();
	} else delete MN;
      }
    }

    for (auto &Pair : Seeds.Calls) {
      bool Valid = true;
      errs() << "Attempting Group:\n";
      for (Instruction *I : Pair.second) {
	//I->dump();
        if (I->getNumUses()) {
	    Valid = false;
        }
      }
      if (Valid && Pair.second.size()>1) {
	//errs() << "Looking for groups\n";
	MultiNode *MN = new MultiNode(&BB);
	MN->addGroup(Pair.second);
	Instruction *I = Pair.second[0];
	I = I->getNextNode();
	for (; I!=Pair.second[1] && !I->isTerminator(); I=I->getNextNode()) {
	  auto *Group = Seeds.getGroupWith(I);
	  bool Valid = false;
	  if (Group && Pair.second.size()==Group->size()) {
	    if ( (*Group)[0]!=I ) continue; //accept only groups in order
	    Valid = true;
	    for (Instruction *OtherI : (*Group)) {
              if (OtherI->getNumUses()) {
		Valid = false;
	      }
	    }
	  }
	  if (Valid) {
	    MN->addGroup(*Group);
            //errs() << "Group:\n";
	    //for (Instruction *OtherI : (*Group)) {
	    //  OtherI->dump();
	    //}
	  }
	}

	if (MN->getNumGroups()>1) {
	  AlignedGraph G(MN,&BB,SE);
          if (G.isSchedulable(BB)) {
  	    NumAttempts++;
            CodeGenerator CG(F, BB, G);
	    bool HasRolled = CG.generate(Seeds);
            Changed = Changed || HasRolled;
	    if (HasRolled) NumRolledLoops++;
          }
          G.destroy();
	} else delete MN;
      }
    }

#ifdef TEST_DEBUG
    //errs() << "stores\n";
#endif
    for (auto &Pair : Seeds.Stores) {
      if (Pair.second.size()>1) {
        std::vector<Instruction *> SavedInsts;
        std::vector<Instruction *> StoreInsts = Pair.second;
	bool Attempt = true;
	bool FirstAttempt = true;
	while (Attempt) {
          Attempt = false;
	  bool HasRolled = false;
          if (StoreInsts.size()>1) {
            AlignedGraph G(StoreInsts, &BB, SE);
	    if (G.isSchedulable(BB)) {
	      NumAttempts++;
	      CodeGenerator CG(F, BB, G);
	      HasRolled = CG.generate(Seeds);
	      if (HasRolled) NumRolledLoops++;
	      Changed = Changed || HasRolled;
	    } else {
	      errs() << G.getDotString() << "\n";
	      //BB.dump();
	    }
            G.destroy();
	  }
	  if (!HasRolled && FirstAttempt) {
            SavedInsts = StoreInsts;
	  }
	  FirstAttempt = false;
	  if (!SavedInsts.empty()) {
	    StoreInst *SI0 = dyn_cast<StoreInst>(SavedInsts[0]);
            if (SI0) {
              Attempt = true;
	      errs() << "Trying AGAIN\n";
	      StoreInsts.clear();
	      StoreInsts.push_back(SI0);
	      size_t step = 1;
	      for (; step<SavedInsts.size(); step++) {
	        StoreInst *SI = dyn_cast<StoreInst>(SavedInsts[step]);
                if (SI && match(SI0->getPointerOperand(), SI->getPointerOperand())) {
                  StoreInsts.push_back(SI);
                } else break;
	      }
              std::vector<Instruction *> Tmp;
	      for (; step<SavedInsts.size(); step++) {
                Tmp.push_back(SavedInsts[step]);
	      }
	      SavedInsts = Tmp;
	    }
	  }
        }
      }
    }
#ifdef TEST_DEBUG
    //errs() << "reductions (terminators)\n";
#endif
    for (auto &Pair : Seeds.Reductions) {
      if (Pair.second==nullptr) continue;
      if (!Pair.second->isTerminator()) continue; //skip non-terminators
      AlignedGraph G(Pair.first, Pair.second, &BB, SE);
      if (G.isSchedulable(BB)) {
	NumAttempts++;
        CodeGenerator CG(F, BB, G);
	bool HasRolled = CG.generate(Seeds);
        Changed = Changed || HasRolled;
	if (HasRolled) NumRolledLoops++;
      }
      G.destroy();
    }
#ifdef TEST_DEBUG
    //errs() << "calls\n";
#endif
    for (auto &Pair : Seeds.Calls) {
      if (Pair.second.size()>1) {
        AlignedGraph G(Pair.second, &BB, SE);
	if (G.isSchedulable(BB)) {
	  NumAttempts++;
          CodeGenerator CG(F, BB, G);
	  bool HasRolled = CG.generate(Seeds);
          Changed = Changed || HasRolled;
	  if (HasRolled) NumRolledLoops++;
	} else {
	  errs() << G.getDotString() << "\n";
	  //BB.dump();
	}
        G.destroy();
      }
    }
#ifdef TEST_DEBUG
    //errs() << "reductions (non-terminators)\n";
#endif
    for (auto &Pair : Seeds.Reductions) {
      if (Pair.second==nullptr) continue;
      if (Pair.second->isTerminator()) continue; //skip terminators
      AlignedGraph G(Pair.first, Pair.second, &BB, SE);
      if (G.isSchedulable(BB)) {
	NumAttempts++;
        CodeGenerator CG(F, BB, G);
	bool HasRolled = CG.generate(Seeds);
        Changed = Changed || HasRolled;
	if (HasRolled) NumRolledLoops++;
      }
      G.destroy();
    }

    return Changed;
}


bool LoopRoller::run() {
  std::vector<BasicBlock *> Blocks;
  for (BasicBlock &BB : F) Blocks.push_back(&BB);

  errs() << "Optimizing: " << F.getName() << "\n";
  //F.dump();

  bool Changed = false;

  for (BasicBlock *BB : Blocks) {
    errs() << "BlockSize: " << BB->size() << "\n";
    collectSeedInstructions(*BB);
    Changed = Changed || attemptRollingSeeds(*BB);
  }
#ifdef TEST_DEBUG
  errs() << "Done Loop Roller: " << NumRolledLoops << "/" << NumAttempts << "\n";
#endif
  if (NumAttempts==0) errs() << "Nothing found in: " << F.getName() << "\n";

  return Changed;
}

bool LoopRolling::runImpl(Function &F, ScalarEvolution *SE) {
  RegionRoller RR(F);
  RR.run();

  LoopRoller RL(F, SE);
  return RL.run();
}

PreservedAnalyses LoopRolling::run(Function &F, FunctionAnalysisManager &AM) {
  auto *SE = &AM.getResult<ScalarEvolutionAnalysis>(F);
  bool Changed = runImpl(F, SE);
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
    if (F.isDeclaration()) return false;

    //F.dump();
    auto *SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
    return Impl.runImpl(F, SE);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<ScalarEvolutionWrapperPass>();
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



