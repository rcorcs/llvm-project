
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
#include <memory>
#include <string>
#include <cxxabi.h>

#define DEBUG_TYPE "loop-rolling"

//#define TEST_DEBUG


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
  MATCH, BINOP, GEPSEQ, INTSEQ, MISMATCH
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
};

class MatchingNode : public Node {
public:
  template<typename ValueT>
  MatchingNode(std::vector<ValueT *> &Vs, BasicBlock &BB, Node *Parent=nullptr) : Node(NodeType::MATCH,Vs,BB,Parent) {}

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
      labelStream << "func: ";
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

  std::string getString() {
    std::string str;
    raw_string_ostream labelStream(str);
    labelStream << BinOpRef->getOpcodeName() << " seq.";
    return labelStream.str();
  }
};

template<typename ValueT>
static Node *buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent) {

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

  return new GEPSequenceNode(VL, Ptr, Indices, BB, Parent);
}

template<typename ValueT>
static Node *buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock &BB, Node *Parent) {
  if (!isa<IntegerType>(VL[0]->getType())) return nullptr;
  Type *Ty = VL[0]->getType();

  std::vector<Value*> Operands;

  std::set<Value *> NonBinOp;
  for (auto *V : VL) {
    auto *BinOp = dyn_cast<BinaryOperator>(V);
    if (BinOp==nullptr) NonBinOp.insert(V);
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

  return new BinOpSequenceNode(VL, BinOpRef, FixedV, Operands, BB, Parent);
}

template<typename ValueT>
static Node *createNode(std::vector<ValueT*> Vs, BasicBlock &BB, Node *Parent=nullptr) {
  bool Matching = true;
  for (unsigned i = 1; i<Vs.size(); i++) {
    Matching = Matching && match(Vs[0],Vs[i]);
    if (auto *I = dyn_cast<Instruction>(Vs[0])) {
      if (I->getParent()!=(&BB)) Matching = false;
    }
  }
  if (Matching) return new MatchingNode(Vs,BB,Parent);
  if (Node *N = buildGEPSequence(Vs, BB, Parent)) return N;
  if (Node *N = buildBinOpSequenceNode(Vs, BB, Parent)) return N;
  if (allConstant(Vs)) {
    if (Value *Step = isConstantSequence(Vs)) {
      return new IntSequenceNode(Vs, Step, BB, Parent);
    }
  }
  return new MismatchingNode(Vs,BB,Parent);
}



template<typename ValueT>
static size_t EstimateSize(std::vector<ValueT*> Code, const DataLayout &DL, TargetTransformInfo *TTI) {
  size_t size = 0;
  for (ValueT *V : Code) {
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
        case Instruction::GetElementPtr:
          size += (isa<GlobalValue>(getUnderlyingObject(I)))?2:1;
          break;
        case Instruction::Load: {
          size += 1;
          auto *LI = dyn_cast<LoadInst>(I);
          if (isa<GlobalValue>(getUnderlyingObject(LI->getPointerOperand()))) {
          //  size += 1;
          }
          break;
        }
        case Instruction::Store: {
          size += 1;
          auto *SI = dyn_cast<StoreInst>(I);
          if (isa<GlobalValue>(getUnderlyingObject(SI->getPointerOperand()))) {
          //  size += 1;
          }
          break;
        }
        default:
          size += 1;
      }
    } else if (auto *GV = dyn_cast<GlobalVariable>(V)) {
      size += 1;
      if (GV->hasInitializer()) {
	if (auto *ArrTy = dyn_cast<ArrayType>(GV->getInitializer()->getType())) {
	   size -= 1;
	   size_t Factor = 4; //DL.getTypeSizeInBits(ArrTy->getElementType())/8;
	   size += Factor*ArrTy->getNumElements();
	}
      }
    }
    //Cost = size - Cost;
    //errs() << "Cost: " << Cost << "\n";
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

};


class Tree {
public:
  Node *Root;
  std::vector<Node*> Nodes;
  std::map<Value*, Node*> NodeMap;
  std::vector<ScheduleNode> SchedulingOrder;

  template<typename ValueT>
  Tree(std::vector<ValueT*> &Vs, BasicBlock &BB) {
    Root = createNode(Vs,BB);
    addNode(Root);
#ifdef TEST_DEBUG
    errs() << "Grow Tree 0\n";
#endif
    growTree(Root,BB);
#ifdef TEST_DEBUG
    errs() << "Grow Tree 1\n";
#endif
    buildSchedulingOrder();
#ifdef TEST_DEBUG
    errs() << "Scheduling order\n";
#endif
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

  bool isSchedulable(BasicBlock &BB);

  std::string getDotString() {
    std::string dotStr;
    raw_string_ostream os(dotStr);
    os << "digraph VTree {\n";

    std::map<Node*, int> NodeId;

    int id = 0;
    //Nodes
    for (Node *N : Nodes) {
      id++;
      NodeId[N] = id;

      os << id << " [label=\"" << N->getString() << "\""
        << ", style=\"filled\" , fillcolor=" << ((N->isMatching()) ? "\"#8ae18a\"" : "\"#ff6671\"")
        << ", shape=" << ((N->isMatching()) ? "box" : "oval") << "];\n";
    }

    //Edges
    for (Node *N : Nodes) {
      //int ChildId = 0;
      for (Node *Child : N->getChildren()) {
        std::string EdgeLabel = "";
        //if (N->isCallInst()) EdgeLabel = std::string(" [label=\"") + std::to_string(ChildId) + std::string("\"]");
        os << NodeId[N] << " -> " << NodeId[Child] << EdgeLabel << "\n";
        //ChildId++;
      }
    }

    os << "}\n";
    return os.str();
  }

private:
  void growTree(Node *N, BasicBlock &BB);

  void buildSchedulingOrder(Node *N, unsigned i, std::set<Node*> &Visited);
  void buildSchedulingOrder() {
    for (unsigned i = 0; i<Root->size(); i++) {
      std::set<Node*> Visited;
      buildSchedulingOrder(Root, i, Visited);
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
  std::map<GlobalVariable*, Instruction *> GlobalLoad;

  Value *cloneTree(Node *N, IRBuilder<> &Builder);
  void generateExtract(std::vector<Value *> &VL, Instruction * NewI, IRBuilder<> &Builder);

  Value *generateMismatchingCode(std::vector<Value *> &VL, IRBuilder<> &Builder);
  Value *generateGEPSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  Value *generateBinOpSequence(std::vector<Value *> &VL, IRBuilder<> &Builder);
  
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


void Tree::growTree(Node *N, BasicBlock &BB) {
  switch(N->getNodeType()) {
    case NodeType::MISMATCH: break;
    case NodeType::INTSEQ: break;
    case NodeType::GEPSEQ: {
       auto &Vs = ((GEPSequenceNode*)N)->getIndices();
       Node *Child = find(Vs);
       if (Child==nullptr) Child = createNode(Vs, BB, N);

       this->addNode(Child);
       N->pushChild(Child);
       
       growTree(Child, BB);
       break;
    }
    case NodeType::BINOP: {
       auto &Vs = ((BinOpSequenceNode*)N)->getOperands();
       Node *Child = find(Vs);
       if (Child==nullptr) Child = createNode(Vs, BB, N);

       this->addNode(Child);
       N->pushChild(Child);
       
       growTree(Child, BB);
       break;
    }
    case NodeType::MATCH: {
      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I==nullptr) return;
      for (unsigned i = 0; i<I->getNumOperands(); i++) {
        std::vector<Value*> Vs;
        for (unsigned j = 0; j<N->size(); j++) {
          Vs.push_back( dyn_cast<Instruction>(N->getValue(j))->getOperand(i) );
        }
        Node *Child = find(Vs);
        if (Child==nullptr) Child = createNode(Vs, BB, N);

        this->addNode(Child);
        N->pushChild(Child);
        
        growTree(Child, BB);
      }
      break;
    }
  }
}

void Tree::buildSchedulingOrder(Node *N, unsigned i, std::set<Node*> &Visited) {
  if (Visited.count(N)) return;
  Visited.insert(N);

  switch(N->getNodeType()) {
    case NodeType::MISMATCH: break;
    case NodeType::INTSEQ: break;
    case NodeType::MATCH: {
      if (auto *I = dyn_cast<Instruction>(N->getValue(i))) {
        for (unsigned j = 0; j<N->getNumChildren(); j++) {
          buildSchedulingOrder(N->getChild(j), i, Visited);
        }
        //if (std::find(SchedulingOrder.begin(),SchedulingOrder.end(),I)==SchedulingOrder.end()) {
        bool Found = false;
        for (auto &SN : SchedulingOrder) {
          if (SN.contains(I)) { Found = true; break; }
        }
        if (!Found) {
          if (I->mayWriteToMemory() || SchedulingOrder.empty()) {
            //errs() << "Breaking Scheduling Node\n";
            ScheduleNode SN;
            SchedulingOrder.push_back(SN);
          }
          //I->dump();
          SchedulingOrder.back().add(I);
        }
      }
    } break;
    case NodeType::BINOP: {
      if (auto *I = dyn_cast<BinaryOperator>(N->getValue(i))) {
        for (unsigned j = 0; j<N->getNumChildren(); j++) {
          buildSchedulingOrder(N->getChild(j), i, Visited);
        }
        //if (std::find(SchedulingOrder.begin(),SchedulingOrder.end(),I)==SchedulingOrder.end()) {
        bool Found = false;
        for (auto &SN : SchedulingOrder) {
          if (SN.contains(I)) { Found = true; break; }
        }
        if (!Found) {
          if (I->mayWriteToMemory() || SchedulingOrder.empty()) {
            //errs() << "Breaking Scheduling Node\n";
            ScheduleNode SN;
            SchedulingOrder.push_back(SN);
          }
          //I->dump();
          SchedulingOrder.back().add(I);
        }
      }
    } break;
    case NodeType::GEPSEQ: {
      if (auto *I = dyn_cast<GetElementPtrInst>(N->getValue(i))) {
        for (unsigned j = 0; j<N->getNumChildren(); j++) {
          buildSchedulingOrder(N->getChild(j), i, Visited);
        }

        //if (std::find(SchedulingOrder.begin(),SchedulingOrder.end(),I)==SchedulingOrder.end()) {
        bool Found = false;
        for (auto &SN : SchedulingOrder) {
          if (SN.contains(I)) { Found = true; break; }
        }
        if (!Found) {
          if (I->mayWriteToMemory() || SchedulingOrder.empty()) {
            //errs() << "Breaking Scheduling Node\n";
            ScheduleNode SN;
            SchedulingOrder.push_back(SN);
          }
          //I->dump();
          SchedulingOrder.back().add(I);
        }
      }
    } break;
  }
}

bool Tree::isSchedulable(BasicBlock &BB) {
  //errs() << "Attempting scheduling...\n";

  //for ( auto *I : SchedulingOrder) I->dump();
  
  if (SchedulingOrder.empty()) return false;

  Instruction *I = nullptr;
  for (auto &IRef : BB) {
    if (SchedulingOrder[0].contains(&IRef)) {
      I = &IRef;
      break;
    }
  }
  if (I==nullptr) return false;

  auto *LastI = dyn_cast<Instruction>(Root->getValue(Root->size()-1))->getNextNode();
  auto It = SchedulingOrder.begin(), E = SchedulingOrder.end();
  size_t Count = SchedulingOrder[0].size();

  //errs() << "Start: "; I->dump();
  while (I!=LastI && It!=E) {
    if ( (*It).contains(I) ) {
      Count--;
      if (Count==0) {
        It++;
        if (It!=E) Count = (*It).size();
      }
    } else {
      //errs() << "Skipping: "; I->dump();
      if (I->mayReadOrWriteMemory()) return false;
    }
    I = I->getNextNode();
  }

  //errs() << "I: "; I->dump();
  //errs() << "Last: "; LastI->dump();
  bool Result = (I==LastI && It==E);
  //errs() << "Schedulable: " << Result << "\n";
  return Result;
}

void CodeGenerator::generateExtract(std::vector<Value *> &VL, Instruction * NewI, IRBuilder<> &Builder) {
  LLVMContext &Context = F.getContext();

  std::set<unsigned> NeedExtract;

  for (unsigned i = 0; i<VL.size(); i++) {
    Value *V = VL[i];
    if (!(isa<Operator>(V) || isa<Instruction>(V))) continue;
    //if (!isa<Instruction>(V)) continue;

    for (auto *U : V->users()) {
      if (T.find(U)==nullptr) {
        NeedExtract.insert(i);
	break;
      }
    }
  }

  if (NeedExtract.empty()) return;

#ifdef TEST_DEBUG
  errs() << "Extracting: "; NewI->dump();
#endif

  BasicBlock &Entry = F.getEntryBlock();
  IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

  Type *IndVarTy = IntegerType::get(Context, 8);
  Value *ArrPtr = ArrBuilder.CreateAlloca(NewI->getType(), ConstantInt::get(IndVarTy, VL.size()));
  CreatedCode.push_back(ArrPtr);

  auto *GEP = Builder.CreateGEP(ArrPtr, IndVar);
  CreatedCode.push_back(GEP);
  auto *Store = Builder.CreateStore(NewI, GEP);
  CreatedCode.push_back(Store);

  IRBuilder<> ExitBuilder(Exit);
  for (unsigned i : NeedExtract) {
    Instruction *I = dyn_cast<Instruction>(VL[i]);
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

Value *CodeGenerator::generateGEPSequence(std::vector<Value *> &VL, IRBuilder<> &Builder) {
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
  
  for (unsigned i = 1; i<VL.size(); i++) {
    if (auto *GEP = dyn_cast<GetElementPtrInst>(VL[i])) {
      if (std::find(Garbage.begin(), Garbage.end(), GEP)==Garbage.end())
        Garbage.push_back(GEP);
    }
  }

  //errs() << "Generating GEP\n";
  Value *IndVarIdx = generateMismatchingCode(Indices, Builder);
  auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(Ptr, IndVarIdx));
  CreatedCode.push_back(GEP);

  generateExtract(VL, GEP, Builder);

  return GEP;
}

Value *CodeGenerator::generateBinOpSequence(std::vector<Value *> &VL, IRBuilder<> &Builder) {
  if (!isa<IntegerType>(VL[0]->getType())) return nullptr;
  Type *Ty = VL[0]->getType();

  std::vector<Value*> Operands;
  Operands.push_back(nullptr);

  unsigned Opcode = 0;
  for (unsigned i = 1; i<VL.size(); i++) {
    auto *BinOp = dyn_cast<BinaryOperator>(VL[i]);
    if (BinOp==nullptr) return nullptr;
    
    if (BinOp->getParent()!=(&BB)) return nullptr;

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

  //errs() << "Generating BinOp\n";

  Instruction *BinOpRef = nullptr;
  for (unsigned i = 1; i<VL.size(); i++) {
    if (auto *BinOp = dyn_cast<BinaryOperator>(VL[i])) {
      BinOpRef = BinOp;
      if (std::find(Garbage.begin(), Garbage.end(), BinOp)==Garbage.end())
        Garbage.push_back(BinOp);
    }
  }

  Value *Op = generateMismatchingCode(Operands, Builder);
  Instruction *NewI = BinOpRef->clone();
  Builder.Insert(NewI);
  CreatedCode.push_back(NewI);

  NewI->setOperand(0,VL[0]);
  NewI->setOperand(1,Op);

  generateExtract(VL, NewI, Builder);

  return NewI;
}

Value *CodeGenerator::cloneTree(Node *N, IRBuilder<> &Builder) {
  if (NodeMap.find(N)!=NodeMap.end()) return NodeMap[N];

  switch(N->getNodeType()) {
    case NodeType::MATCH: {
      std::vector<Value*> Operands;
      for (unsigned i = 0; i<N->getNumChildren(); i++) {
        Operands.push_back(cloneTree(N->getChild(i), Builder));
      }

#ifdef TEST_DEBUG
      errs() << "Match: "; 
      if (isa<Function>(N->getValue(0)))
        errs() << N->getValue(0)->getName() << "\n";
      else {
        errs() << "\n";
        for (auto *V : N->getValues()) V->dump();
      }
#endif
      Instruction *I = dyn_cast<Instruction>(N->getValue(0));
      if (I) {
        Instruction *NewI = I->clone();
        Builder.Insert(NewI);
        CreatedCode.push_back(NewI);
        NodeMap[N] = NewI;


        SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
        NewI->getAllMetadata(MDs);
        for (std::pair<unsigned, MDNode *> MDPair : MDs) {
          NewI->setMetadata(MDPair.first, nullptr);
        }

        for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
          NewI->setOperand(i,Operands[i]);
        }
        
        for (auto *V : N->getValues()) {
          auto *I = dyn_cast<Instruction>(V);
          if (I && std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end()) Garbage.push_back(I);
        }

        generateExtract(N->getValues(), NewI, Builder);

        return NewI;
      } else return N->getValue(0);
    }
    case NodeType::GEPSEQ: {
#ifdef TEST_DEBUG
      errs() << "Generating GEP\n";
#endif
      auto *GN = (GEPSequenceNode*)N;

      std::vector<Value*> Tmp;
      for (unsigned i = 1; i<GN->size(); i++) {
        if (auto *GEP = dyn_cast<GetElementPtrInst>(GN->getValue(i))) {
          if (std::find(Garbage.begin(), Garbage.end(), GEP)==Garbage.end()) {
            Garbage.push_back(GEP);
	    Tmp.push_back(GEP);
	  }
        }
      }

      Value *IndVarIdx = cloneTree(N->getChild(0), Builder);
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getPointerOperand(), IndVarIdx));
      CreatedCode.push_back(GEP);

      generateExtract(Tmp, GEP, Builder);

      NodeMap[N] = GEP;
      return GEP;
    }
    case NodeType::BINOP: {
#ifdef TEST_DEBUG
      errs() << "Generating BINOP\n";
#endif
      auto *BON = (BinOpSequenceNode*)N;

      std::vector<Value*> Tmp;
      for (unsigned i = 1; i<BON->size(); i++) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(BON->getValue(i))) {
          if (std::find(Garbage.begin(), Garbage.end(), BinOp)==Garbage.end()) {
            Garbage.push_back(BinOp);
	    Tmp.push_back(BinOp);
	  }
        }
      }

      Value *Op = cloneTree(BON->getChild(0), Builder);
      Instruction *NewI = BON->getReference()->clone();
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);

      NewI->setOperand(0,BON->getFixedOperand());
      NewI->setOperand(1,Op);

      generateExtract(Tmp, NewI, Builder);

      NodeMap[N] = NewI;
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

      NodeMap[N] = NewV;
      return NewV;
    }
    case NodeType::MISMATCH: {
      Value *NewV = generateMismatchingCode(N->getValues(), Builder);
      NodeMap[N] = NewV;
      return NewV;
    }
  }
}

static Instruction *SchedulingPoint(Tree &T, BasicBlock &BB) {
  Instruction *LastInst = dyn_cast<Instruction>(T.Root->getValues()[T.getWidth()-1]);
  return LastInst->getNextNode();
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

  Type *IndVarTy = IntegerType::get(Context, 8);

  IndVar = Builder.CreatePHI(IndVarTy, 0);
  CreatedCode.push_back(IndVar);

  Exit = BasicBlock::Create(Context, "rolled.exit", BB.getParent());

  cloneTree(T.Root, Builder);

  auto *Add = Builder.CreateAdd(IndVar, ConstantInt::get(IndVarTy, 1));
  CreatedCode.push_back(Add);

  auto *Cond = Builder.CreateICmpNE(Add, ConstantInt::get(IndVarTy, T.getWidth()));
  CreatedCode.push_back(Cond);

  auto &DL = BB.getParent()->getParent()->getDataLayout();
  TargetTransformInfo TTI(DL);

  size_t SizeOriginal = EstimateSize(Garbage,DL,&TTI);
  size_t SizeModified = EstimateSize(CreatedCode,DL,&TTI); // + 3;

  bool Profitable = SizeOriginal > SizeModified;

  errs() << "Gains: " << SizeOriginal << " - " << SizeModified << " = " << ( ((int)SizeOriginal) - ((int)SizeModified) ) << "\n";
  if (Profitable) {
    errs() << "Profitable: finishing code generation\n";

    //BB.dump();

    IndVar->addIncoming(ConstantInt::get(IndVarTy, 0),PreHeader);
    IndVar->addIncoming(Add,Header);

    auto *Br = Builder.CreateCondBr(Cond,Header,Exit);
    CreatedCode.push_back(Br);

    Builder.SetInsertPoint(PreHeader);
    Builder.CreateBr(Header);

    //copy instructions to the Exit block
    Builder.SetInsertPoint(Exit);
    while (InstSplitPt!=BB.getTerminator()) {
      auto *I = InstSplitPt;
      InstSplitPt = InstSplitPt->getNextNode();
      I->removeFromParent();
      Builder.Insert(I);
    }
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

    for (auto It = Garbage.rbegin(), E = Garbage.rend(); It!=E; It++) {
      //errs() << "Deleting: "; (*It)->dump();
      Seeds.remove(*It);
      (*It)->eraseFromParent();
    }
    
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
#endif
    return true;
  } else {
    errs() << "Unprofitable: deleting generated code\n";

    DeleteDeadBlock(Exit);
    DeleteDeadBlock(Header);
    DeleteDeadBlock(PreHeader);

#ifdef TEST_DEBUG
    errs() << "Done!\n";
#endif
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

  bool Changed = false;
  for (BasicBlock *BB : Blocks) {
    collectSeedInstructions(*BB);
#ifdef TEST_DEBUG
    errs() << "stores\n";
#endif
    for (auto &Pair : Seeds.Stores) {
      if (Pair.second.size()>1) {
        Tree T(Pair.second, *BB);
	errs() << T.getDotString() << "\n";
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
	errs() << T.getDotString() << "\n";
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



