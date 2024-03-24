#ifndef LLVM_TRANSFORMS_SCALAR_LOOPROLLINGUTILS_H
#define LLVM_TRANSFORMS_SCALAR_LOOPROLLINGUTILS_H

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Analysis/DominanceFrontierImpl.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
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

using namespace llvm;

bool match(Value *V1, Value *V2);

extern cl::opt<bool> AlwaysRoll;
extern cl::opt<bool> MatchAlignment;

static std::string demangle(const char* name) {
  int status = -1;
  std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
  return (status == 0) ? res.get() : std::string(name);
}

template<typename ValueT>
void printVs(std::vector<ValueT*> Vs) {
  for (auto *V : Vs) {
    if (BasicBlock *BB = dyn_cast<BasicBlock>(V)) 
      errs() << "  BB: " << BB->getName().str() << "\n";
    else V->dump();
  }
}




/// \returns True if all of the values in \p VL are constants (but not
/// globals/constant expressions).
template<typename ValueT>
static bool allConstant(const std::vector<ValueT *> VL) {
  // Constant expressions and globals can't be vectorized like normal integer/FP
  // constants.
  for (ValueT *i : VL)
    if (!(isa<Constant>(i) || isa<ConstantExpr>(i) || isa<GlobalValue>(i)))
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
    if (CInt==nullptr) return nullptr;
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


enum NodeType {
  MATCH, IDENTICAL, BINOP, GEPSEQ, INTSEQ, ALTSEQ, CONSTEXPR, MINMAXREDUCTION, REDUCTION, RECURRENCE, MISMATCH, MULTI, LABEL, PHI
};

static std::string NodeTypeString(NodeType ty) {
  switch(ty) {
  case NodeType::MATCH: return "MATCH";
  case NodeType::IDENTICAL: return "IDENTICAL";
  case NodeType::BINOP: return "BINOP";
  case NodeType::GEPSEQ: return "GEPSEQ";
  case NodeType::INTSEQ: return "INTSEQ";
  case NodeType::ALTSEQ: return "ALTSEQ";
  case NodeType::CONSTEXPR: return "CONSTEXPR";
  case NodeType::REDUCTION: return "REDUCTION";
  case NodeType::RECURRENCE: return "RECURRENCE";
  case NodeType::MULTI: return "MULTI";
  case NodeType::LABEL: return "LABEL";
  case NodeType::PHI: return "PHI";
  case NodeType::MISMATCH: return "MISMATCH";
  }
}

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

  template<typename ValueT>
  static MatchingNode *get(std::vector<ValueT*> &Vs, BasicBlock *BB=nullptr, Node *Parent=nullptr) {
    bool Matching = true;
    //bool HasSideEffect = false;
    std::unordered_set<Value*> UniqueValues;
    UniqueValues.insert(Vs[0]);
    if (auto *I = dyn_cast<Instruction>(Vs[0])) {
      //if (I->getParent()!=BB) Matching = false;
      //HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
    }
    for (unsigned i = 1; i<Vs.size(); i++) {
      UniqueValues.insert(Vs[i]);
      Matching = Matching && match(Vs[i-1],Vs[i]);
      if (auto *I = dyn_cast<Instruction>(Vs[i])) {
        //if (I->getParent()!=BB) Matching = false;
       // HasSideEffect = HasSideEffect || I->mayHaveSideEffects();
      }
    }
    errs() << "Match: " << Matching << "\n";
    errs() << UniqueValues.size() << " x " << Vs.size() << "\n";

    //Matching = Matching && (UniqueValues.size()==Vs.size() || (!HasSideEffect));
    Matching = Matching && UniqueValues.size()==Vs.size();
    errs() << "Final Match: " << Matching << "\n";

    if (Matching) {
      //errs() << "Matching\n";
      return new MatchingNode(Vs,BB,Parent);
    }
    return nullptr;
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

  template<typename ValueT>
  static ConstantExprNode *get(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent);

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

class LabelNode : public Node {
public:
  template<typename ValueT>
  LabelNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::LABEL,Vs,BB,Parent) {}

  Instruction *getValidInstruction(unsigned i) {
    return nullptr;
  }

  std::string getString() {
    return "label";
  }

  template<typename ValueT>
  static LabelNode *get(std::vector<ValueT*> &Vs, BasicBlock *BB=nullptr, Node *Parent=nullptr) {
    bool AllBlocks = true;
    for (unsigned i = 0; i<Vs.size(); i++) {
      AllBlocks = AllBlocks && isa<BasicBlock>(Vs[i]);
    }
    if (AllBlocks) {
      return new LabelNode(Vs,BB,Parent);
    }
    return nullptr;
  }
};

class MatchingPHINode : public Node {
public:
  template<typename ValueT>
  MatchingPHINode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::PHI,Vs,BB,Parent) {}

  std::vector<Node *> Labels;
  
  void pushLabel(Node *LN) {
    Labels.push_back(LN);
  }

  Instruction *getValidInstruction(unsigned i) {
    return dyn_cast<PHINode>(getValue(i));
  }

  std::string getString() {
    return "phi";
  }

  template<typename ValueT>
  static MatchingPHINode *get(std::vector<ValueT*> &Vs, BasicBlock *BB=nullptr, Node *Parent=nullptr) {
    bool SameTypes = true;
    bool AllPHIs = true;
    for (auto *V : Vs) {
      AllPHIs = AllPHIs && isa<PHINode>(V);
      SameTypes = SameTypes && V->getType()==Vs[0]->getType();
    }
    if (SameTypes && AllPHIs) return new MatchingPHINode(Vs,BB,Parent);
    return nullptr;
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

  template<typename ValueT>
  static IdenticalNode *get(std::vector<ValueT*> &Vs, BasicBlock *BB=nullptr, Node *Parent=nullptr) {
    bool AllSame = true;
    for (unsigned i = 1; i<Vs.size(); i++) {
      AllSame = AllSame && Vs[i]==Vs[0];
    }
    if (AllSame) {
      return new IdenticalNode(Vs,BB,Parent);
    }
    return nullptr;
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

  template<typename ValueT>
  static IntSequenceNode *get(std::vector<ValueT*> &Vs, BasicBlock *BB=nullptr, Node *Parent=nullptr) {
    if (allConstant(Vs)) {
      if (Value *Step = isConstantSequence(Vs)) {
        //errs() << "Int Seq\n";
        return new IntSequenceNode(Vs, Step, BB, Parent);
      }
    }
    return nullptr;
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

  template<typename ValueT>
  static AlternatingSequenceNode *get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
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
    //if (I->getParent()!=getBlock()) return nullptr;
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

  template<typename ValueT>
  static GEPSequenceNode *get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);

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
  
  template<typename ValueT>
  static BinOpSequenceNode *get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent, ScalarEvolution *SE);

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

  //template<typename ValueT>
  //RecurrenceNode *get(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent);
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

  template<typename ValueT>
  static ReductionNode *get(ValueT *V, Instruction *U, BasicBlock *BB, Node *Parent);

};

class MinMaxReductionNode : public Node {
public:
  enum OperationType {
    MIN, MAX, NONE
  };
private:
  SelectInst *SelRef;
  Value *Start;
  std::vector<Value *> Vs;
  OperationType Operation;
public:

  MinMaxReductionNode(SelectInst *Sel, OperationType Op, Value *Start, std::vector<SelectInst *> Sels, std::vector<Value*> &Vs, BasicBlock *BB, Node *Parent=nullptr) : Node(NodeType::MINMAXREDUCTION,Sels,BB,Parent), SelRef(Sel), Operation(Op), Start(Start), Vs(Vs) {}

  SelectInst *getSelection() { return SelRef; }
  std::vector<Value *> &getOperands() { return Vs; }
  Value *getStartValue() { if (Start) return Start; else return getNeutralValue(); }
  
  OperationType getOperation() { return Operation; }

  Instruction *getValidInstruction(unsigned i) {
    auto *I = dyn_cast<SelectInst>(getValue(i));
    return I;
  }

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    if (getOperation()==OperationType::MIN)
      labelStream << " min";
    else if (getOperation()==OperationType::MAX)
      labelStream << " max";
    
    labelStream << " red.";
    return labelStream.str();
  }

  Value *getNeutralValue() {
    return nullptr; //ConstantInt::get(Ty, 0);
  }

  //static void collectValues(SelectInst *Sel, std::vector<SelectInst *> &Sels, std::vector<Value*> &Vs);


  template<typename ValueT>
  static MinMaxReductionNode *get(ValueT *V, Instruction *U, BasicBlock *BB, unsigned Skip, Node *Parent);

};


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
    if (!StartInt.isZero()) return false;

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
For a given base address, Addr, the sequence
A[0], A[1], A[2], ...
can be simplified as
*A, A[1], A[2], ...
making it less obvious that these operations match.
In LLVM, the indexing operation is represented using GetElementPtr (GEP).
This special node tries to identify this pattern of GEP sequence.
*/
template<typename ValueT>
GEPSequenceNode *buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {

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
      if (BB && GEP->getParent()!=BB) return nullptr;
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


  //Inputs.insert(Ptr);
  return new GEPSequenceNode(VL, RefGEP, Ptr, Indices, BB, Parent);
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


template<typename ValueT>
GEPSequenceNode *buildGEPSequence2(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  //errs() << "GEPSeq2\n";
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

  //Inputs.insert(RefGEP->getPointerOperand());
  return new GEPSequenceNode(VL, RefGEP, nullptr, Indices, BB, Parent);
}



template<typename ValueT>
GEPSequenceNode *GEPSequenceNode::get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  if (GEPSequenceNode *N = buildGEPSequence(VL, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }
  if (GEPSequenceNode *N = buildGEPSequence2(VL, BB, Parent)) {
    errs() << "GEP Seq\n";
    return N;
  }
  return nullptr;
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
ConstantExprNode *ConstantExprNode::get(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {
//Node *AlignedGraph::buildConstExprNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {

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


/*
 Identifies a sequence of alternating values of the same type, for example:
 V0, V1, V0, V1, V0, V1, ...
 In a loop with induction variable i starting from 0, can be represented by the expression:
 (i%2==0) ? V0 : V1
 V0 and V1 must be loop invariant, after loop rolling, i.e., they must be input values to the aligned graph.
*/
template<typename ValueT>
AlternatingSequenceNode *AlternatingSequenceNode::get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
//Node *AlignedGraph::buildAlternatingSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  if (VL.size()<2) return nullptr;

  Value *First = VL[0];
  Value *Second = VL[1];

  if (First->getType()!=Second->getType()) return nullptr;

  for (unsigned i = 2; i<VL.size(); i++) {
    if (VL[i] != VL[i%2]) return nullptr;
  }
  
  //Inputs.insert(First);
  //Inputs.insert(Second);

  return new AlternatingSequenceNode(VL, First, Second, BB, Parent);
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
BinOpSequenceNode *BinOpSequenceNode::get(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent, ScalarEvolution *SE) {
//Node *AlignedGraph::buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent) {
  //errs() << "BinOP?\n";
  //VL[0]->dump();

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
ReductionNode *ReductionNode::get(ValueT *V, Instruction *U, BasicBlock *BB, Node *Parent) {
  if (V==nullptr) return nullptr;
  BinaryOperator *BO = dyn_cast<BinaryOperator>(V);
  if (BO==nullptr) return nullptr;

  errs() << "Building reduction\n";
  U->dump();
  BO->dump();

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

  //if (PHI) Inputs.insert(PHI);

  return new ReductionNode(BO, PHI, BOs, Vs, BB, Parent);
}

template<typename ValueT>
MinMaxReductionNode *MinMaxReductionNode::get(ValueT *V, Instruction *U, BasicBlock *BB, unsigned Skip, Node *Parent) {
  if (V==nullptr) return nullptr;
  SelectInst *Sel = dyn_cast<SelectInst>(V);
  if (Sel==nullptr) return nullptr;

  errs() << "Building min-max reduction\n";
  U->dump();
  Sel->dump();

  if (Sel->getParent()!=BB) return nullptr;
  //if (!ReductionNode::isValidOperation(BO)) return nullptr;

  SelectInst *SelRef = Sel;
  if (Sel==nullptr) return nullptr;
  PHINode *PHI = dyn_cast<PHINode>(U);
  Value *Start = nullptr;
  std::vector<SelectInst*> Sels;
  std::vector<Value*> Vs;
  //ReductionNode::collectValues(BO,PHI,BOs,Vs);
  //collectMinMaxReduction(Sel);

  SelectInst *NextSel = Sel;
  Value *Val = nullptr;
  MinMaxReductionNode::OperationType OperationRef = MinMaxReductionNode::OperationType::NONE;
  
  int LastVal = -1;

  do {
    Sel = NextSel;
    NextSel = nullptr;

    CmpInst *Cmp = dyn_cast<CmpInst>(Sel->getCondition());
    if (Cmp==nullptr) return nullptr;
    if (Cmp->getNumUses()>1) return nullptr;

    errs() << "Processing selection...\n";
    Cmp->dump();
    Sel->dump();

    MinMaxReductionNode::OperationType Operation = MinMaxReductionNode::OperationType::NONE;
    switch (Cmp->getPredicate()) {
    case CmpInst::Predicate::ICMP_ULT:
    case CmpInst::Predicate::ICMP_ULE:
    case CmpInst::Predicate::ICMP_SLT:
    case CmpInst::Predicate::ICMP_SLE:
    case CmpInst::Predicate::FCMP_OLT:
    case CmpInst::Predicate::FCMP_OLE:
    case CmpInst::Predicate::FCMP_ULT:
    case CmpInst::Predicate::FCMP_ULE:
      if (Cmp->getOperand(0)==Sel->getTrueValue() && Cmp->getOperand(1)==Sel->getFalseValue()) {
        Operation = MinMaxReductionNode::OperationType::MIN;
        errs() << "Min operation found\n";
      } else if (Cmp->getOperand(0)==Sel->getFalseValue() && Cmp->getOperand(1)==Sel->getTrueValue()) {
        Operation = MinMaxReductionNode::OperationType::MAX;
        errs() << "Max operation found\n";
      } else {
        errs() << "Invalid Cmp-Select pair\n";
        return nullptr;
        //assert("Unexpected pattern");
      }
      break;
    case CmpInst::Predicate::ICMP_UGT:
    case CmpInst::Predicate::ICMP_UGE:
    case CmpInst::Predicate::ICMP_SGT:
    case CmpInst::Predicate::ICMP_SGE:
    case CmpInst::Predicate::FCMP_OGT:
    case CmpInst::Predicate::FCMP_OGE:
    case CmpInst::Predicate::FCMP_UGT:
    case CmpInst::Predicate::FCMP_UGE:
      if (Cmp->getOperand(0)==Sel->getTrueValue() && Cmp->getOperand(1)==Sel->getFalseValue()) {
        Operation = MinMaxReductionNode::OperationType::MAX;
        errs() << "Max operation found\n";
      } else if (Cmp->getOperand(0)==Sel->getFalseValue() && Cmp->getOperand(1)==Sel->getTrueValue()) {
        Operation = MinMaxReductionNode::OperationType::MIN;
        errs() << "Min operation found\n";
      } else {
        errs() << "Invalid Cmp-Select pair\n";
        return nullptr;
        //assert("Unexpected pattern");
      }
      break;
    default:
      assert("Unexpected predicate");
    }

    if (Operation!=MinMaxReductionNode::OperationType::NONE) {
      if (OperationRef==MinMaxReductionNode::OperationType::NONE)
        OperationRef = Operation;
      if (OperationRef!=Operation) {
        errs() << "TODO: Invalid: differet reduction operation!\n";
        break;
      }
      if (NextSel=dyn_cast<SelectInst>(Sel->getTrueValue())) {
        Val = Sel->getFalseValue();
        LastVal = 0;
        Sels.push_back(Sel);
        Vs.push_back(Val);
      }
      else if (NextSel=dyn_cast<SelectInst>(Sel->getFalseValue())) {
        Val = Sel->getTrueValue();
        LastVal = 1;
        Sels.push_back(Sel);
        Vs.push_back(Val);
      } else {
        errs() << "TODO: Something must be done here!\n";
        if (LastVal==1) {
          Val = Sel->getTrueValue();
          Start = Sel->getFalseValue();
        } else {
          Val = Sel->getFalseValue();
          Start = Sel->getTrueValue();
        }

        Sels.push_back(Sel);
        Vs.push_back(Val);
        break;
      }

    }
  } while (NextSel);
  errs() << "Done with collection of min/max\n";

  if (Skip >= Sels.size()) return nullptr;
  if (Skip) {
    size_t n = Sels.size()-Skip;
    Start = Sels[n];
    Sels.resize(n);
    Vs.resize(n);
  }

  if (Sels.size()<=1) return nullptr;
  if (Start==nullptr) return nullptr;

  errs() << "Created MinMaxReductionNode:\n";
  if (OperationRef==MinMaxReductionNode::OperationType::MIN)
  errs() << "MIN\n";
  else if (OperationRef==MinMaxReductionNode::OperationType::MAX)
  errs() << "MAX\n";
  else errs() << "NONE\n";
  errs() << "Start:";
  if (Start) Start->dump();
  else errs() << " none\n";
  errs() << "Vals:\n";
  printVs(Vs);

  return new MinMaxReductionNode(SelRef, OperationRef, Start, Sels, Vs, BB, Parent);
}



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
          size += 1 + dyn_cast<CallBase>(I)->arg_size();
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

#endif
