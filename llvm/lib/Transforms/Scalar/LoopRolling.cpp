
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

static bool matchGEP(GetElementPtrInst *GEP1, GetElementPtrInst *GEP2) {
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

static bool match(Value *V1, Value *V2) {
  Instruction *I1 = dyn_cast<Instruction>(V1);
  Instruction *I2 = dyn_cast<Instruction>(V2);

  if (V1->getType()!=V2->getType()) return false;

  if(I1 && I2 && I1->getOpcode()==I2->getOpcode()) {
    if (I1->getNumOperands()!=I2->getNumOperands()) return false;

    for (unsigned i = 0; i<I1->getNumOperands(); i++)
      if (I1->getOperand(i)->getType()!=I2->getOperand(i)->getType()) return false;

    switch (I1->getOpcode()) {
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
      return LI1->getAlign()==LI2->getAlign();
    }
    case Instruction::Store: {
      auto *SI1 = dyn_cast<StoreInst>(I1);
      auto *SI2 = dyn_cast<StoreInst>(I2);
      return SI1->getAlign()==SI2->getAlign();
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
  MATCH, BINOP, GEPSEQ, INTSEQ, REDUCTION, RECURRENCE, MISMATCH
};

class Node {
private:
  Node *Parent;
  NodeType NType;
  std::vector<Value*> Values;
  std::vector<Node *> Children;
public:
  template<typename ValueT>
  Node(NodeType NT, std::vector<ValueT *> &Vs, BasicBlock &BB, Node *Parent=nullptr) : Parent(Parent), NType(NT) {
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

  bool isMatching() { return NType!=NodeType::MISMATCH; }
  NodeType getNodeType() { return NType; }

  virtual std::string getString() = 0;
  virtual Instruction *getValidInstruction(unsigned i) = 0;
};

class MatchingNode : public Node {
public:
  template<typename ValueT>
  MatchingNode(std::vector<ValueT *> &Vs, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::MATCH,Vs,BB,Parent) {}

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

class MismatchingNode : public Node {
public:
  template<typename ValueT>
  MismatchingNode(std::vector<ValueT *> &Vs, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::MISMATCH,Vs,BB,Parent) {}

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

class IntSequenceNode : public Node {
private:
  Value *Step;
public:
  template<typename ValueT>
  IntSequenceNode(std::vector<ValueT *> &Vs, Value *Step, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::INTSEQ,Vs,BB,Parent), Step(Step) {}

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

class GEPSequenceNode : public Node {
private:
  Value *Ptr;
  std::vector<Value*> Indices;
public:
  template<typename ValueT>
  GEPSequenceNode(std::vector<ValueT *> &Vs, Value *Ptr, std::vector<Value*> &Indices, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::GEPSEQ,Vs,BB,Parent), Ptr(Ptr), Indices(Indices) {}

  Value *getPointerOperand() { return Ptr; }
  std::vector<Value *> &getIndices() { return Indices; }

  Instruction *getValidInstruction(unsigned i) {
    auto *I = dyn_cast<GetElementPtrInst>(getValue(i));
    if (I && getUnderlyingObject(I)!=Ptr) return nullptr;
    return I;
  }

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
  Value *FixedOperand;
  std::vector<Value *> Operands;
public:
  template<typename ValueT>
  BinOpSequenceNode(std::vector<ValueT *> &Vs, BinaryOperator *BinOpRef, Value *FixedOperand, std::vector<Value *> &Operands, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::BINOP,Vs,BB,Parent), BinOpRef(BinOpRef), FixedOperand(FixedOperand), Operands(Operands) {}

  BinaryOperator *getReference() { return BinOpRef; }
  Value *getFixedOperand() { return FixedOperand; }
  std::vector<Value *> &getOperands() { return Operands; }

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
  RecurrenceNode(std::vector<ValueT *> &Vs, Value *StartValue, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::RECURRENCE,Vs,BB,Parent), StartValue(StartValue) {}
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

  ReductionNode(BinaryOperator *BinOp, PHINode *Start, std::vector<BinaryOperator*> &BOs, std::vector<Value*> &Vs, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::REDUCTION,BOs,BB,Parent), BinOpRef(BinOp), Start(Start), Vs(Vs) {}

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
    return Neutral;
  }

  static bool isValidOperation(BinaryOperator *BO) {
    switch(BO->getOpcode()) {
    case Instruction::Add:
    case Instruction::Or:
    case Instruction::Xor:
    case Instruction::Mul:
    case Instruction::And:
      return (BO->isAssociative() && BO->isCommutative());
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

class ScheduleNode {
  std::set<Instruction *> Instructions;

public:
  void add(Instruction *I) { Instructions.insert(I); }

  bool contains(Instruction *I) {
    return Instructions.count(I);
  }

  size_t size() { return Instructions.size(); }
  
  bool empty() { return Instructions.empty(); }

  std::set<Instruction *> &getInstructions() {
    return Instructions;
  }

};


class Tree {
public:
  BasicBlock *BB;
  Node *Root;
  std::vector<Node*> Nodes;
  std::unordered_map<Value*, std::unordered_set<Node*> > NodeMap;
  std::vector<ScheduleNode> SchedulingOrder;
  std::unordered_set<Value *> ValuesInNode;
  std::unordered_set<Value *> Inputs;

  Tree(BinaryOperator *BO, Instruction *U, BasicBlock &BB) : BB(&BB) {
    Root = buildReduction(BO,U,BB,nullptr);
    if (Root) {
      addNode(Root);
#ifdef TEST_DEBUG
      errs() << "Grow Tree 0\n";
#endif
      std::set<Node*> Visited;
      growTree(Root,BB, Visited);
#ifdef TEST_DEBUG
      errs() << "Grow Tree 1\n";
#endif
      buildSchedulingOrder();
#ifdef TEST_DEBUG
      errs() << "Scheduling order\n";
#endif
    }
  }

  template<typename ValueT>
  Tree(std::vector<ValueT*> &Vs, BasicBlock &BB) : BB(&BB) {
    Root = createNode(Vs,BB);
    addNode(Root);
#ifdef TEST_DEBUG
    errs() << "Grow Tree 0\n";
#endif
    std::set<Node*> Visited;
    growTree(Root,BB, Visited);
#ifdef TEST_DEBUG
    errs() << "Grow Tree 1\n";
#endif
    buildSchedulingOrder();
#ifdef TEST_DEBUG
    errs() << "Scheduling order\n";
#endif
  }

  BasicBlock *getBlock() { return BB; }

  size_t getWidth() {
    return Root->size();
  }

  void addNode(Node *N) {
    if (N && N->size()) {
      if (std::find(Nodes.begin(), Nodes.end(), N)==Nodes.end()) {
        Nodes.push_back(N);
	if (N->getNodeType()!=NodeType::MISMATCH && N->getNodeType()!=NodeType::RECURRENCE) {
          for (auto *V : N->getValues()) {
            //NodeMap[V].insert(N);
            ValuesInNode.insert(V);
	  }
	}
        NodeMap[N->getValue(0)].insert(N);
      }
      if (Root==nullptr) Root = N;
    }
  }

  bool contains(Value *V) { return ValuesInNode.count(V); }
  
  template<typename ValueT>
  Node *find(std::vector<ValueT *> &Vs);

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
    for (unsigned i = 0; i<Root->size(); i++) {
      auto *I = Root->getValidInstruction(i);
      Depends = Depends || dependsOn(I,V,I->getParent(),Visited);
    }
    return Depends;
  }

  bool invalidDependence(Value *V, std::unordered_set<Value*> &Visited);

  Instruction *getStartingInstruction(BasicBlock &BB);
  Instruction *getEndingInstruction(BasicBlock &BB);
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
        << ", style=\"filled\" , fillcolor=" << ((N->isMatching()) ? "\"#8ae18a\"" : "\"#ff6671\"")
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
  void growTree(Node *N, BasicBlock &BB, std::set<Node*> &Visited);

  template<typename ValueT>
  Node *buildReduction(ValueT *V, Instruction *, BasicBlock &BB, Node *Parent);
  template<typename ValueT>
  Node *buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent);
  template<typename ValueT>
  Node *buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent);
  template<typename ValueT>
  Node *buildRecurrenceNode(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent);
  template<typename ValueT>
  Node *createNode(std::vector<ValueT*> Vs, BasicBlock &BB, Node *Parent=nullptr);

  void buildSchedulingOrder(Node *N, unsigned i, std::set<Node*> &Visited);
  void buildSchedulingOrder() {
    ScheduleNode SN;
    SchedulingOrder.push_back(SN);

    Node *N = Root;
    if (N->getNodeType()==NodeType::REDUCTION) {
      N = N->getChild(0);
    }

    for (unsigned i = 0; i<N->size(); i++) {
      std::set<Node*> Visited;
      buildSchedulingOrder(N, i, Visited);
    }

    if (SchedulingOrder.back().empty()) {
      SchedulingOrder.pop_back();
    }

    for (Node *N : Nodes) {
      for (unsigned i = 0; i<N->size(); i++) {
        Value *V = N->getValidInstruction(i);
        if (V==nullptr) continue;
        for (auto *U : V->users()) {
          //if (this->find(U)==nullptr) {
          if (!this->contains(U)) {
	    Users.insert(U);
          }
        }
      }
    }

  }
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

  std::unordered_map<Node*, Value *> NodeToValue;
  //std::vector<Instruction *> Garbage;
  std::unordered_set<Instruction *> Garbage;
  std::vector<Value *> CreatedCode;
  std::unordered_map<Instruction *, Instruction *> Extracted;
  std::unordered_map<GlobalVariable*, Instruction *> GlobalLoad;

  Value *cloneTree(Node *N, IRBuilder<> &Builder);
  void generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder);
  //void generateExtract(std::vector<Value *> &VL, Instruction * NewI, IRBuilder<> &Builder);

  Value *generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder);
  //Value *generateGEPSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  //Value *generateBinOpSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  
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


template<typename ValueT>
Node *Tree::find(std::vector<ValueT *> &Vs) {
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

static void ReorderOperands(std::vector<Value*> &Operands, BasicBlock &BB) {
  std::unordered_map<const Value*,APInt> Ids;
  
  unsigned BitWidth = 64;
  if (Operands[0]->getType()->isIntegerTy()) {
    BitWidth = Operands[0]->getType()->getIntegerBitWidth();
  }
  unsigned i = 0;
  for (auto &I : BB) {
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

template<typename ValueT>
Node *Tree::buildReduction(ValueT *V, Instruction *U, BasicBlock &BB, Node *Parent) {
  if (V==nullptr) return nullptr;
  BinaryOperator *BO = dyn_cast<BinaryOperator>(V);
  if (BO==nullptr) return nullptr;
  if (BO->getParent()!=&BB) return nullptr;
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


template<typename ValueT>
Node *Tree::buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent) {

  if (!isa<PointerType>(VL[0]->getType())) return nullptr;
  auto *Ptr = getUnderlyingObject(VL[0]);
  for (unsigned i = 1; i<VL.size(); i++)
    if (Ptr != getUnderlyingObject(VL[i])) return nullptr;

  std::vector<Value*> Indices;

  Type *Ty = nullptr;
  for (unsigned i = 1; i<VL.size(); i++) {
    if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (GEP->getParent()!=(&BB)) return nullptr;
      if (GEP->getPointerOperand()!=Ptr) return nullptr;
      if (GEP->getNumIndices()!=1) return nullptr;
      Value *Idx = GEP->getOperand(1);
      Ty = Idx->getType();
      break;
    }
  }
  if (Ty==nullptr || !isa<IntegerType>(Ty)) return nullptr;

  for (unsigned i = 1; i<VL.size(); i++) {
    if (Ptr==VL[i]) {
      Indices.push_back(ConstantInt::get(Ty, 0));
    } else if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (GEP->getPointerOperand()!=Ptr) return nullptr;
      if (GEP->getNumIndices()!=1) return nullptr;
      Value *Idx = GEP->getOperand(1);
      Indices.push_back(Idx);
      if (Ty!=Idx->getType()) return nullptr;
    } else return nullptr;
  }

  Inputs.insert(Ptr);
  
  return new GEPSequenceNode(VL, Ptr, Indices, BB, Parent);
}

template<typename ValueT>
Node *Tree::buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent) {
  if (!isa<IntegerType>(VL[0]->getType())) return nullptr;

  std::vector<Value*> Operands;

  std::set<Value *> NonBinOp;
  for (auto *V : VL) {
    if (!isa<BinaryOperator>(V)) NonBinOp.insert(V);
  }
  if (NonBinOp.size()!=1) return nullptr;

  Value *FixedV = *NonBinOp.begin();

  BinaryOperator *BinOpRef = nullptr;
  for (unsigned i = 0; i<VL.size(); i++) {
    auto *BinOp = dyn_cast<BinaryOperator>(VL[i]);
    if (BinOp==nullptr) {
      Operands.push_back(nullptr);
      continue;
    }
    
    if (BinOp->getParent()!=(&BB)) return nullptr;

    if (BinOpRef) {
      //matching binop?
      if (BinOpRef->getOpcode()!=BinOp->getOpcode()) return nullptr;
    } else BinOpRef = BinOp;

    Value *Op = nullptr;
    if (BinOp->isCommutative() && BinOp->getOperand(1)==FixedV)
      Op = BinOp->getOperand(0);
    else if (BinOp->getOperand(0)==FixedV)
      Op = BinOp->getOperand(1);

    if (Op==nullptr) return nullptr;

    Operands.push_back(Op);
  }

  if (BinOpRef==nullptr) return nullptr;

  Type *Ty = FixedV->getType();

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

  for (unsigned i = 0; i<Operands.size(); i++) {
    if (Operands[i]==nullptr) Operands[i] = Neutral;
  }

  if (FixedV) Inputs.insert(FixedV);

  return new BinOpSequenceNode(VL, BinOpRef, FixedV, Operands, BB, Parent);
}

template<typename ValueT>
Node *Tree::buildRecurrenceNode(std::vector<ValueT *> &Vs, BasicBlock &BB, Node *Parent) {
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

template<typename ValueT>
Node *Tree::createNode(std::vector<ValueT*> Vs, BasicBlock &BB, Node *Parent) {
#ifdef TEST_DEBUG
  errs() << "Creating node\n";
#endif
  bool Matching = true;
  for (unsigned i = 1; i<Vs.size(); i++) {
    Matching = Matching && match(Vs[i-1],Vs[i]);
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      if (I->getParent()!=(&BB)) Matching = false;
    }
    //TODO: hack
    /*
    auto *GEP1 = dyn_cast<GetElementPtrInst>(Vs[0]);
    auto *GEP2 = dyn_cast<GetElementPtrInst>(Vs[i]);
    auto *LI1 = dyn_cast<LoadInst>(Vs[0]);
    auto *LI2 = dyn_cast<LoadInst>(Vs[i]);
    if (LI1 && LI2) {
      GEP1 = dyn_cast<GetElementPtrInst>(LI1->getPointerOperand());
      GEP2 = dyn_cast<GetElementPtrInst>(LI2->getPointerOperand());
    }
    if (GEP1 && GEP2) {
      if (GEP1->getPointerOperand()!=GEP2->getPointerOperand()) Matching = false;
    }
    */
  }
#ifdef TEST_DEBUG
  errs() << "trying MATCH\n";
#endif
  if (Matching) return new MatchingNode(Vs,BB,Parent);
#ifdef TEST_DEBUG
  errs() << "trying GEPSEQ\n";
#endif
  if (Node *N = buildGEPSequence(Vs, BB, Parent)) return N;
#ifdef TEST_DEBUG
  errs() << "trying BINOP\n";
#endif
  if (Node *N = buildBinOpSequenceNode(Vs, BB, Parent)) return N;
#ifdef TEST_DEBUG
  errs() << "trying RECURRENCE\n";
#endif

  if (Node *N = buildRecurrenceNode(Vs, BB, Parent)) return N;

#ifdef TEST_DEBUG
  errs() << "trying INTSEQ\n";
#endif

  if (allConstant(Vs)) {
    if (Value *Step = isConstantSequence(Vs)) {
      return new IntSequenceNode(Vs, Step, BB, Parent);
    }
  }
#ifdef TEST_DEBUG
  errs() << "building MISMATCH\n";
#endif
  for (auto *V : Vs) Inputs.insert(V);

  return new MismatchingNode(Vs,BB,Parent);
}

void Tree::growTree(Node *N, BasicBlock &BB, std::set<Node*> &Visited) {
  if (Visited.find(N)!=Visited.end()) return;
  Visited.insert(N);

  switch(N->getNodeType()) {
    case NodeType::MISMATCH: break;
    case NodeType::INTSEQ: break;
    case NodeType::RECURRENCE: break;
    case NodeType::REDUCTION: {
#ifdef TEST_DEBUG
       errs() << "Growing REDUCTION\n";
#endif
       auto &Vs = ((ReductionNode*)N)->getOperands();

       Node *Child = find(Vs);
       if (Child==nullptr) {
         Child = createNode(Vs, BB, N);
         this->addNode(Child);
         N->pushChild(Child);
         growTree(Child, BB, Visited);
       } else N->pushChild(Child);
#ifdef TEST_DEBUG
       errs() << "OK\n";
#endif
       break;
    }
    case NodeType::GEPSEQ: {
#ifdef TEST_DEBUG
       errs() << "Growing GEPSEQ\n";
#endif
       auto &Vs = ((GEPSequenceNode*)N)->getIndices();

       Node *Child = find(Vs);
       if (Child==nullptr) {
         Child = createNode(Vs, BB, N);
         this->addNode(Child);
         N->pushChild(Child);
         growTree(Child, BB, Visited);
       } else N->pushChild(Child);
#ifdef TEST_DEBUG
       errs() << "OK\n";
#endif
       break;
    }
    case NodeType::BINOP: {
#ifdef TEST_DEBUG
       errs() << "Growing BINOP\n";
#endif
       auto &Vs = ((BinOpSequenceNode*)N)->getOperands();
       Node *Child = find(Vs);
       if (Child==nullptr) {
         Child = createNode(Vs, BB, N);
         this->addNode(Child);
         N->pushChild(Child);
         growTree(Child, BB, Visited);
       } else N->pushChild(Child);
#ifdef TEST_DEBUG
       errs() << "OK\n";
#endif
       break;
    }
    case NodeType::MATCH: {
#ifdef TEST_DEBUG
       errs() << "Growing MATCH\n";
#endif
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
        Node *Child = find(Vs);
        if (Child==nullptr) {
	  Child = createNode(Vs, BB, N);
          this->addNode(Child);
          N->pushChild(Child);
          growTree(Child, BB, Visited);
	} else N->pushChild(Child);
#ifdef TEST_DEBUG
        errs() << "OK\n";
#endif
      }
      break;
    }
  }
}

void Tree::buildSchedulingOrder(Node *N, unsigned i, std::set<Node*> &Visited) {
  if (Visited.count(N)) return;
  Visited.insert(N);
  if (i>=N->size()) return;

  if (auto *I = N->getValidInstruction(i)) {
    for (auto *CN : N->getChildren()) {
      buildSchedulingOrder(CN, i, Visited);
    }
    //if (std::find(SchedulingOrder.begin(),SchedulingOrder.end(),I)==SchedulingOrder.end()) {
    if (I->mayReadOrWriteMemory() || I->mayHaveSideEffects()) {
    bool Found = false;
    for (auto &SN : SchedulingOrder) {
      if (SN.contains(I)) { Found = true; break; }
    }
#ifdef TEST_DEBUG
    if (Found) {
      errs() << "Already scheduled?! ";
      I->dump();
    }
#endif
    if (!Found) {
      //N->getValue(i)->dump();

      if (I->mayWriteToMemory() || I->mayHaveSideEffects()) {
        //errs() << "Breaking Scheduling Node\n";
	if (!SchedulingOrder.back().empty()) {
          ScheduleNode SN;
          SchedulingOrder.push_back(SN);
	}
        SchedulingOrder.back().add(I);
        ScheduleNode SN;
        SchedulingOrder.push_back(SN);
      } else {
        //I->dump();
        SchedulingOrder.back().add(I);
      }
    }
    }
  }
}

Instruction *Tree::getStartingInstruction(BasicBlock &BB) {
  Instruction *I = nullptr;
  for (auto &IRef : BB) {
    if (SchedulingOrder[0].contains(&IRef)) {
      I = &IRef;
      break;
    }
  }
  return I;
}

Instruction *Tree::getEndingInstruction(BasicBlock &BB) {
  return dyn_cast<Instruction>(Root->getValue(Root->size()-1));
}

bool Tree::invalidDependence(Value *V, std::unordered_set<Value*> &Visited) {
  if (isa<Instruction>(V) && contains(V)) {
	  errs() << "Invalid: "; V->dump();
	  return true;
  }

  if (Visited.find(V)!=Visited.end()) return false;
  if (isa<PHINode>(V)) return false;

  Visited.insert(V);

  Instruction *I = dyn_cast<Instruction>(V);
  if (I && I->getParent()==getBlock()) {
    for (unsigned i = 0 ; i<I->getNumOperands(); i++) {
      if (invalidDependence(I->getOperand(i), Visited)) return true;
    }
  }

  return false;
}


bool Tree::isSchedulable(BasicBlock &BB) {

  if (Root==nullptr) return false;

#ifdef TEST_DEBUG
  errs() << "Checking input dependencies\n";
#endif
  std::unordered_set<Value*> Visited;
  for (auto *V : Inputs) {
    if (invalidDependence(V,Visited)) {
#ifdef TEST_DEBUG
      errs() << "Invalid dependence found!\n";
#endif
      return false;
    }
  }

#ifdef TEST_DEBUG
  errs() << "Checking scheduling order\n";
#endif
  //for ( auto &SN : SchedulingOrder) {
  //  errs() << "Scheduling Node:\n";
  //  for (auto *I : SN.getInstructions()) I->dump();
  //}
  
  if (SchedulingOrder.empty()) return false;

  Instruction *I = getStartingInstruction(BB);
  if (I==nullptr) return false;

  //auto *LastI = dyn_cast<Instruction>(Root->getValue(Root->size()-1))->getNextNode();
  auto *LastI = BB.getTerminator();
  auto It = SchedulingOrder.begin(), E = SchedulingOrder.end();
  int Count = SchedulingOrder[0].size();

  //errs() << "Count: " << Count << "\n";
  //errs() << "Start: "; I->dump();
  while (I!=LastI && It!=E) {
    if (I->mayReadOrWriteMemory() || I->mayHaveSideEffects()) {
    //errs() << "Processing: "; I->dump();
    //errs() << "Count: " << Count << "\n";
    if ( (*It).contains(I) ) {
      //errs() << "Found: "; I->dump();
      Count--;
      if (Count==0) {
        It++;
        if (It!=E) Count = (*It).size();
      }
    } else {
      //errs() << "Not found: "; I->dump();
      //if (I->mayReadOrWriteMemory()) return false;
      //if (I->mayWriteToMemory()) {
      if (I->mayReadOrWriteMemory() || I->mayHaveSideEffects()) {
	      errs() << "Read/Write memory\n";
	      //return false;
	      break;
      }
    }
    } else {
      //errs() << "Non-memory: "; I->dump();
    }
    I = I->getNextNode();
  }

  //errs() << "I: "; I->dump();
  //errs() << "Last: "; LastI->dump();
  //bool Result = (I==LastI && It==E);
  bool Result = It==E;
#ifdef TEST_DEBUG
  errs() << "Schedulable: " << Result << "\n";
#endif
  return Result;
}

void CodeGenerator::generateExtract(Node *N, Instruction * NewI, IRBuilder<> &Builder) {
  LLVMContext &Context = F.getContext();

  std::set<unsigned> NeedExtract;

  for (unsigned i = 0; i<N->size(); i++) {
    Value *V = N->getValidInstruction(i);
    if (V==nullptr) continue;
#ifdef TEST_DEBUG
    errs() << "Looking for:"; V->dump();
#endif
    for (auto *U : V->users()) {
      if (!T.contains(U)) {
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
  errs() << "Mismatch:\n";
  for (Value *V : VL) V->dump();
#endif
  
  bool AllSame = true;
  for (unsigned i = 0; i<VL.size(); i++) {
    AllSame = AllSame && VL[i]==VL[0];
  }
  if (AllSame) return VL[0];

  if (allConstant(VL)) {
    auto *ArrTy = ArrayType::get(VL[0]->getType(), VL.size());

    SmallVector<Constant*,8> Consts;
    for (auto *V : VL) Consts.push_back(dyn_cast<Constant>(V));

    Value *IndexedValue = nullptr;
    for (auto &Pair : GlobalLoad) {
      auto *GA = Pair.first;

      if (GA->hasInitializer()) {
        auto *C = GA->getInitializer();
        auto *ArrTy = dyn_cast<ArrayType>(C->getType());
        if (ArrTy==nullptr) continue;
        if (ArrTy->getNumElements()!=Consts.size()) continue;
        for (unsigned i = 0; i<Consts.size(); i++) {
          if (C->getAggregateElement(i)!=Consts[i]) continue;
        }
        //Found Array
        //errs() << "Found Array: "; GA->dump();
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
      auto *GEP = Builder.CreateGEP(GArray, Indices);
      CreatedCode.push_back(GEP);

      auto *Load = Builder.CreateLoad(VL[0]->getType(), GEP);
      CreatedCode.push_back(Load);

      GlobalLoad[GArray] = Load;
      IndexedValue = Load;
    }
    return IndexedValue;
  } else {
    //BasicBlock &Entry = F->getEntryBlock();
    //IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());
    IRBuilder<> ArrBuilder(PreHeader);

    Type *IndVarTy = IntegerType::get(Context, 8);
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

Value *CodeGenerator::cloneTree(Node *N, IRBuilder<> &Builder) {
  if (NodeToValue.find(N)!=NodeToValue.end()) return NodeToValue[N];

  switch(N->getNodeType()) {
    case NodeType::MATCH: {
#ifdef TEST_DEBUG
      errs() << "Generating Match\n";
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
          Operands.push_back(cloneTree(N->getChild(i), Builder));
        }

#ifdef TEST_DEBUG
        errs() << "Operands done!\n";
#endif

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
      } else return N->getValue(0);
    }
    case NodeType::GEPSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating GEP\n";
#endif
      auto *GN = (GEPSequenceNode*)N;

      assert(GN->getNumChildren() && "Expected child with indices!");
      Value *IndVarIdx = cloneTree(GN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing GEP\n";
#endif
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getPointerOperand(), IndVarIdx));
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
      Value *Op = cloneTree(BON->getChild(0), Builder);
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

      NewI->setOperand(0,BON->getFixedOperand());
      NewI->setOperand(1,Op);

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
      Value *Op = cloneTree(RN->getChild(0), Builder);
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
      auto *CastIndVar = Builder.CreateIntCast(IndVar, StartValue->getType(), false);
      CreatedCode.push_back(CastIndVar);
      if (StartValue->isZero() && StepValue->isOne()){
        NewV = CastIndVar;
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

  //for (auto *N : T.Nodes) {
  //  if (N->getNodeType()==NodeType::MISMATCH && !allConstant(N->getValues()))
  //    return false;
  //}

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
  cloneTree(T.Root, Builder);
#ifdef TEST_DEBUG
  errs() << "Tree code generated!\n";
#endif
 
  //Late generation of recurrences
  for (Node *N : T.Nodes) {
    if (N->getNodeType()==NodeType::RECURRENCE) {
      //update PHI
      PHINode *PHI = dyn_cast<PHINode>(NodeToValue[N]);
      errs() << "PHI: recurrence " << Header->getName() << ",";NodeToValue[N->getChild(0)]->dump();
      PHI->addIncoming(NodeToValue[N->getChild(0)],Header);
    }
  }

#ifdef TEST_DEBUG
  errs() << "Root:\n";
  for (auto *V : T.Root->getValues()) V->dump();
  errs() << "Root size: " << T.Root->size() << "\n";
#endif

  auto *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  CreatedCode.push_back(Add);

  auto *Cond = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, T.getWidth()));
  CreatedCode.push_back(Cond);

  auto &DL = BB.getParent()->getParent()->getDataLayout();
  TargetTransformInfo TTI(DL);

#ifdef TEST_DEBUG
  errs() << "Estimating size: original\n";
#endif
  size_t SizeOriginal = EstimateSize(Garbage,DL,&TTI);
#ifdef TEST_DEBUG
  errs() << "Estimating size: modified\n";
#endif
  size_t SizeModified = 0;
  SizeModified += 2*EstimateSize(PreHeader,DL,&TTI); 
  SizeModified += EstimateSize(Header,DL,&TTI); 
  SizeModified += 0.25*EstimateSize(Exit,DL,&TTI); 

  bool Profitable = SizeOriginal > SizeModified;
  //BB.dump();
  //
  errs() << T.getDotString() << "\n";

  //PreHeader->dump();
  //Header->dump();
  //Exit->dump();

  errs() << "Gains: " << SizeOriginal << " - " << SizeModified << " = " << ( ((int)SizeOriginal) - ((int)SizeModified) ) << "; ";
  errs() << "Width: " << T.Root->size() << "; ";
  if (T.Root->getNodeType() == NodeType::REDUCTION) errs() << "Reduction ";
  if (Profitable) {
    errs() << "Profitable;\n";
    //errs() << T.getDotString() << "\n";
  } else errs() << "Unprofitable;\n";

  //if (false) {
  if (Profitable) {
	//std::string FileName = std::string("/tmp/roll.") + F.getParent()->getSourceFileName() + std::string(".") + F.getName().str();
	//FileName += "." + BB.getName().str() + ".dot";
	//T.writeDotFile(FileName);

    IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),PreHeader);
    IndVar->addIncoming(Add,Header);

    auto *Br = Builder.CreateCondBr(Cond,Header,Exit);
    CreatedCode.push_back(Br);

    Builder.SetInsertPoint(PreHeader);
    Builder.CreateBr(Header);

    Instruction *InstSplitPt = T.getStartingInstruction(BB);
    if (InstSplitPt==nullptr) {
      return false;
    }
    auto *EndPt = BB.getTerminator();

    //copy instructions to the Exit block
    Builder.SetInsertPoint(Exit);

    while (InstSplitPt!=EndPt) {
      auto *I = InstSplitPt;
      InstSplitPt = InstSplitPt->getNextNode();
      if (!T.dependsOn(I)) {
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
	 Seeds.remove(I);
	 I->eraseFromParent();
      }
    }
    for (auto It = BB.rbegin(), E = BB.rend(); It!=E; ) {
      Instruction *I = &*It;
      It++;
      if (Garbage.count(I)) {
	 Seeds.remove(I);
	 I->eraseFromParent();
      }
    }
    
    for (BasicBlock *Succ : SuccBBs) {
      for (Instruction &I : *Succ) { //TODO: run only over PHIs
        if (auto *PHI = dyn_cast<PHINode>(&I)) {
          PHI->replaceIncomingBlockWith(&BB,Exit);
        }
      }
    }

//#ifdef TEST_DEBUG
    errs() << "Done!\n";

    BB.dump();
    PreHeader->dump();
    Header->dump();
    Exit->dump();
//#endif
    if ( verifyFunction(*BB.getParent()) ) {
      errs() << "Broken Function!!\n";
      BB.getParent()->dump();
    }
    return true;
  } else {
    //errs() << "Unprofitable\n";

#ifdef TEST_DEBUG
    errs() << "Destroying Exit\n";
#endif
    DeleteDeadBlock(Exit);
#ifdef TEST_DEBUG
    errs() << "Destroying Header\n";
#endif
    DeleteDeadBlock(Header);
#ifdef TEST_DEBUG
    errs() << "Destroying Pre Header\n";
#endif
    DeleteDeadBlock(PreHeader);

#ifdef TEST_DEBUG
    errs() << "Done!\n";
#endif
    return false;
  }
}

static BinaryOperator *getPossibleReduction(Value *V) {
#ifdef TEST_DEBUG
  errs() << "looking for reduction\n";
#endif
  if (V==nullptr) return nullptr;
  BinaryOperator *BO = dyn_cast<BinaryOperator>(V);
  if (BO==nullptr) return nullptr;
  if (!ReductionNode::isValidOperation(BO)) return nullptr;
  BinaryOperator *BO1 = dyn_cast<BinaryOperator>(BO->getOperand(0));
  BinaryOperator *BO2 = dyn_cast<BinaryOperator>(BO->getOperand(1));
  unsigned PossibleReduction = 0;
  if (BO1 && BO1->getOpcode()==BO->getOpcode()) PossibleReduction += 1;
  if (BO2 && BO2->getOpcode()==BO->getOpcode()) PossibleReduction += 1;
  if (PossibleReduction==0) return nullptr;
  return BO;
}

void LoopRoller::collectSeedInstructions(BasicBlock &BB) {
  // Initialize the collections. We will make a single pass over the block.
  Seeds.clear();

#ifdef TEST_DEBUG
    errs() << "collecting seeds\n";
#endif
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
        errs() << "Possible reduction\n";
	BO->dump();
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
          errs() << "Possible reduction\n";
	  BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        }
      }
    }
    else if (auto *Br = dyn_cast<BranchInst>(&I)) {
      if (Br->isConditional()) {
        if (BinaryOperator *BO = getPossibleReduction(Br->getCondition())) {
#ifdef TEST_DEBUG
          errs() << "Possible reduction\n";
#endif
	  BO->dump();
          Seeds.Reductions[BO] = &I;
        }
      }
    }
    else if (auto *Ret = dyn_cast<ReturnInst>(&I)) {
      if (BinaryOperator *BO = getPossibleReduction(Ret->getReturnValue())) {
#ifdef TEST_DEBUG
        errs() << "Possible reduction\n";
	BO->dump();
#endif
        Seeds.Reductions[BO] = &I;
      }
    }
    else if (auto *PHI = dyn_cast<PHINode>(&I)) {
      //if (PHI->getNumIncomingValues()!=2) continue;
      if (PHI->getBasicBlockIndex(PHI->getParent())>=0) {
        Value *V = PHI->getIncomingValueForBlock(PHI->getParent());
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

bool LoopRoller::run() {
  std::vector<BasicBlock *> Blocks;
  for (BasicBlock &BB : F) Blocks.push_back(&BB);

  bool Changed = false;
  for (BasicBlock *BB : Blocks) {
    collectSeedInstructions(*BB);
#ifdef TEST_DEBUG
    errs() << "stores\n";
#endif
    //unsigned Count = 0;

    BB->dump();
    for (auto &Pair : Seeds.Stores) {
      if (Pair.second.size()>1) {
        Tree T(Pair.second, *BB);
	//errs() << T.getDotString() << "\n";
	//std::string FileName = std::string("/tmp/roll.") + F.getParent()->getSourceFileName() + std::string(".") + F.getName().str();
	//FileName += "." + std::to_string(Count++) + ".dot";
	//T.writeDotFile(FileName);
	if (T.isSchedulable(*BB)) {
	  CodeGenerator CG(F, *BB, T);
#ifdef TEST_DEBUG
	  errs() << "code gen 0\n";
#endif
	  Changed = Changed || CG.generate(Seeds);
#ifdef TEST_DEBUG
	  errs() << "code gen 1\n";
#endif
	}
#ifdef TEST_DEBUG
	errs() << "destroying tree\n";
#endif
        T.destroy();
      }
    }
#ifdef TEST_DEBUG
    errs() << "calls\n";
#endif
    for (auto &Pair : Seeds.Calls) {
      if (Pair.second.size()>1) {
        Tree T(Pair.second, *BB);
	//errs() << T.getDotString() << "\n";
	//std::string FileName = std::string("/tmp/roll.") + F.getParent()->getSourceFileName() + std::string(".") + F.getName().str();
	//FileName += "." + std::to_string(Count++) + ".dot";
	//T.writeDotFile(FileName);
	if (T.isSchedulable(*BB)) {
	  CodeGenerator CG(F, *BB, T);
#ifdef TEST_DEBUG
	  errs() << "code gen 0\n";
#endif
	  Changed = Changed || CG.generate(Seeds);
#ifdef TEST_DEBUG
	  errs() << "code gen 1\n";
#endif
	}
#ifdef TEST_DEBUG
	errs() << "destroying tree\n";
#endif
        T.destroy();
      }
    }
#ifdef TEST_DEBUG
    errs() << "reductions\n";
#endif
    for (auto &Pair : Seeds.Reductions) {
      if (Pair.second==nullptr) continue;
      //errs() << "REDUCTION TREE!!\n";
      Tree T(Pair.first, Pair.second, *BB);
      //errs() << T.getDotString() << "\n";
      if (T.isSchedulable(*BB)) {
        CodeGenerator CG(F, *BB, T);
#ifdef TEST_DEBUG
        errs() << "code gen 0\n";
#endif
        Changed = Changed || CG.generate(Seeds);
#ifdef TEST_DEBUG
        errs() << "code gen 1\n";
#endif
      }
#ifdef TEST_DEBUG
      errs() << "destroying tree\n";
#endif
      T.destroy();
    }
  }
#ifdef TEST_DEBUG
  errs() << "Done Loop Roller!\n";
#endif
  return Changed;
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
    if (F.isDeclaration()) return false;

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



