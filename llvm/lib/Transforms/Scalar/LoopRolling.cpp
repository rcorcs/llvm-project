//
#include "llvm/Transforms/Scalar/LoopRolling.h"
#include "LoopRollingUtils.h"
#include "RegionRolling.h"

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

#define DEBUG_TYPE "loop-rolling"

#define TEST_DEBUG

using namespace llvm;

cl::opt<bool>
AlwaysRoll("loop-rolling-always", cl::init(false), cl::Hidden,
                 cl::desc("Always roll loops, skipping the profitability analysis"));

static cl::opt<int>
SizeThreshold("loop-rolling-size-threshold", cl::init(2), cl::Hidden,
                 cl::desc("Size threshold for the loop rolling profitability analysis"));

cl::opt<bool>
MatchAlignment("loop-rolling-match-alignment", cl::init(false), cl::Hidden,
                 cl::desc("Consider alignment while matching instructions"));

static cl::opt<bool>
EnableExtensions("loop-rolling-extensions", cl::init(false), cl::Hidden,
                 cl::desc("Enable loop rolling extensions"));

bool match(Value *V1, Value *V2) {
  Instruction *I1 = dyn_cast<Instruction>(V1);
  Instruction *I2 = dyn_cast<Instruction>(V2);
  
  errs() << "match analysis\n";
  if (V1->getType()!=V2->getType()) return false;

  if(I1 && I2) {
    errs() << "comparing two instructions for matching\n";
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
      if (EnableExtensions) {
        errs() << "Call1\n";
        if (CI1->getCalledOperand()==nullptr || CI2->getCalledOperand()==nullptr) return false;
        errs() << "Call2\n";
        CI1->getCalledOperand()->dump();
        CI2->getCalledOperand()->dump();
        if (CI1->getCalledOperand()!=CI2->getCalledOperand()) return false;
        errs() << "Call3\n";
        //if (CI1->getCalledFunction()->isVarArg()) return false;
        //errs() << "Call4\n";
      return true;
      } else {
        errs() << "Call1\n";
        if (CI1->getCalledFunction()==nullptr || CI2->getCalledFunction()==nullptr) return false;
        errs() << "Call2\n";
        if (CI1->getCalledFunction()!=CI2->getCalledFunction()) return false;
        errs() << "Call3\n";
        //if (CI1->getCalledFunction()->isVarArg()) return false;
        //errs() << "Call4\n";
      }
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

  ConstantExpr *CExpr1 = dyn_cast<ConstantExpr>(V1);
  ConstantExpr *CExpr2 = dyn_cast<ConstantExpr>(V2);
  if (CExpr1 && CExpr2 && EnableExtensions) {
    errs() << "comparing two constant expressions for matching\n";
    CExpr1->dump();
    CExpr2->dump();
    //return I1->isSameOperationAs(I2);
    if (CExpr1->getOpcode()!=CExpr2->getOpcode()) return false;
    if (CExpr1->getNumOperands()!=CExpr2->getNumOperands()) return false;

    for (unsigned i = 0; i<CExpr1->getNumOperands(); i++)
      if (CExpr1->getOperand(i)->getType()!=CExpr2->getOperand(i)->getType()) return false;
    
    bool Matched = true;
    for (unsigned i = 0; i<CExpr1->getNumOperands(); i++)
      Matched = Matched && match(CExpr1->getOperand(i), CExpr2->getOperand(i));

    // switch (CExpr1->getOpcode()) { 
    // default:
    //   return true;
    // }
    return Matched;
  }

  return V1==V2;
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
    //Root = ReductionNode::get(BO,U,BB,nullptr);
    if (Root) {
      addNode(Root);
      std::set<Node*> Visited;
      growGraph(Root,BB, Visited);
    }
  }
  AlignedGraph(SelectInst *Sel, Instruction *U, BasicBlock *BB, unsigned Skip = 0, ScalarEvolution *SE=nullptr) : BB(BB), SE(SE) {
    Root = buildMinMaxReduction(Sel,U,BB,Skip, nullptr);
    if (Root) {
      addNode(Root);
      std::set<Node*> Visited;
      errs() << "Growing AlignedGraph: MinMaxReductionNode root\n";
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
            if (N->getNodeType()==NodeType::MINMAXREDUCTION) {
              if (SelectInst *Sel = dyn_cast<SelectInst>(V))
                ValuesInNode.insert(Sel->getCondition());
            }
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
    NodeMap.clear();
    NodeMap2.clear();
    SchedulingOrder.clear();
    ValuesInNode.clear();
    Inputs.clear();
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
  Node *buildMinMaxReduction(ValueT *V, Instruction *, BasicBlock *BB, unsigned Skip, Node *Parent);
  //template<typename ValueT>
  //Node *buildGEPSequence(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  //template<typename ValueT>
  //Node *buildGEPSequence2(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  //template<typename ValueT>
  //Node *buildAlternatingSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  //template<typename ValueT>
  //Node *buildBinOpSequenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  template<typename ValueT>
  Node *buildRecurrenceNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
  //template<typename ValueT>
  //Node *buildConstExprNode(std::vector<ValueT *> &VL, BasicBlock *BB, Node *Parent);
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
  std::unordered_map<SelectInst *, Instruction *> MinMaxReductions;

  void clear() {
    Stores.clear();
    Calls.clear();
    Reductions.clear();
    MinMaxReductions.clear();
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
    if (SelectInst *Sel = dyn_cast<SelectInst>(I)) {
      if (MinMaxReductions.find(Sel)!=MinMaxReductions.end()) MinMaxReductions[Sel] = nullptr;
    }
    for (auto &Pair : MinMaxReductions) {
      if (Pair.second==I) MinMaxReductions[Pair.first] = nullptr;
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

template<typename ValueT>
Node *AlignedGraph::buildReduction(ValueT *V, Instruction *U, BasicBlock *BB, Node *Parent) {
  Node *N = ReductionNode::get(V, U, BB, Parent);
  PHINode *PHI = dyn_cast<PHINode>(U);
  if (N && PHI)
    Inputs.insert(PHI);
  return N;
}

template<typename ValueT>
Node *AlignedGraph::buildMinMaxReduction(ValueT *V, Instruction *U, BasicBlock *BB, unsigned Skip, Node *Parent) {
  Node *N = MinMaxReductionNode::get(V, U, BB, Skip, Parent);
  PHINode *PHI = dyn_cast<PHINode>(U);
  if (N && PHI)
    Inputs.insert(PHI);
  return N;
}



template<typename ValueT>
Node *AlignedGraph::buildRecurrenceNode(std::vector<ValueT *> &Vs, BasicBlock *BB, Node *Parent) {
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

  //Inputs.insert(Vs[0]);

  auto *RN = new RecurrenceNode(Vs, Vs[0], BB, Parent);
  RN->pushChild(N);

  return RN;
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

/*
template<typename ValueT>
static bool buildMinMaxReduction(std::vector<ValueT*> Vs, BasicBlock *BB, Node *Parent) {
  errs() << "buildMinMaxReduction:\n";
  printVs(Vs);
  for (auto *V : Vs) {
    if (SelectInst *Sel = dyn_cast<SelectInst>(V)) {
       CmpInst *Cmp = dyn_cast<CmpInst>(Sel->getCondition());
       if (Cmp) {
         Sel->dump();
         errs() << "SELECT: CMP: operands: " << Cmp->getNumOperands() << "\n";
       }
    }
  }
}
*/

template<typename ValueT>
Node *AlignedGraph::createNode(std::vector<ValueT*> Vs, BasicBlock *BB, Node *Parent) {
  
  errs() << "Creating Node\n";

  bool SameBlock = true;
  if (BB) {
    for (unsigned i = 0; i<Vs.size(); i++)
    if (auto *I = dyn_cast<Instruction>(Vs[i])) {
      SameBlock = SameBlock && (I->getParent()==BB);
    }
  }

  if (Node *N = IdenticalNode::get(Vs, BB, Parent)) {
  //if (AllSame) {
    errs() << "All the Same\n";
    Inputs.insert(Vs[0]);
    //return new IdenticalNode(Vs,BB,Parent);
    return N;
  }
  if (SameBlock) {
    if (Node *N = MatchingNode::get(Vs, BB, Parent)) {
    //if (Matching) {
      errs() << "Matching\n";
      //return new MatchingNode(Vs,BB,Parent);
      return N;
    }
  }
  if (GEPSequenceNode *N = GEPSequenceNode::get(Vs, BB, Parent)) {
    if (N->getPointerOperand()) 
      Inputs.insert(N->getPointerOperand());
    else Inputs.insert(N->getReference()->getPointerOperand());
    errs() << "GEP Seq\n";
    return N;
  }

  //if (Node *N = buildBinOpSequenceNode(Vs, BB, Parent)) {
  if (Node *N = BinOpSequenceNode::get(Vs, BB, Parent, SE)) {
    errs() << "BinOp Seq\n";
    return N;
  }
  //if (Node *N = RecurrenceNode::get(Vs, BB, Parent)) {
  if (Node *N = buildRecurrenceNode(Vs, BB, Parent)) {
    errs() << "Recurrence\n";
    Inputs.insert(Vs[0]);
    return N;
  }

  if (Node *N = IntSequenceNode::get(Vs, BB, Parent)) {
    errs() << "Int Seq\n";
    return N;
  }

  if (AlternatingSequenceNode *N = AlternatingSequenceNode::get(Vs, BB, Parent)) {
  //if (Node *N = buildAlternatingSequenceNode(Vs, BB, Parent)) {
    errs() << "Alt Seq\n";
    Inputs.insert(N->getFirst()); //Vs[0]);
    Inputs.insert(N->getSecond()); //Vs[1]);
    return N;
  }
  if (Node *N = ConstantExprNode::get(Vs, BB, Parent)) {
  //if (Node *N = buildConstExprNode(Vs, BB, Parent)) {
    errs() << "Const Expr\n";
    return N;
  }

  /*
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
  */

  errs() << "Mismatching\n";
  printVs(Vs);
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
    case NodeType::MINMAXREDUCTION: {
       auto &Vs = ((MinMaxReductionNode*)N)->getOperands();
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
  if (NeedExtract.size()==1 && N->getNodeType()==NodeType::MINMAXREDUCTION) {
    MinMaxReductionNode *RN = (MinMaxReductionNode*)N;
    if (RN->getSelection()==RN->getValidInstruction(*NeedExtract.begin())) {
      return;
    }
  }

  BasicBlock &Entry = F.getEntryBlock();
  IRBuilder<> ArrBuilder(&*Entry.getFirstInsertionPt());

  Type *IndVarTy = IntegerType::get(Context, 8);
  Value *ArrPtr = ArrBuilder.CreateAlloca(NewI->getType(), ConstantInt::get(IndVarTy, N->size()));
  CreatedCode.push_back(ArrPtr);

  auto *GEP = Builder.CreateGEP(NewI->getType(), ArrPtr, IndVar);
  CreatedCode.push_back(GEP);
  auto *Store = Builder.CreateStore(NewI, GEP);
  CreatedCode.push_back(Store);

  IRBuilder<> ExitBuilder(Exit);
  for (unsigned i : NeedExtract) {
    Instruction *I = N->getValidInstruction(i);
    auto *GEP = ExitBuilder.CreateGEP(NewI->getType(), ArrPtr, ConstantInt::get(IndVarTy, i));
    CreatedCode.push_back(GEP);
    auto *Load = ExitBuilder.CreateLoad(NewI->getType(), GEP);
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

      auto *GEP = Builder.CreateGEP(VL[0]->getType(), GArray, Indices);
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
      auto *GEP = ArrBuilder.CreateGEP(VL[i]->getType(), ArrPtr, ConstantInt::get(IndVarTy, i));
      CreatedCode.push_back(GEP);
      auto *Store = ArrBuilder.CreateStore(VL[i], GEP);
      CreatedCode.push_back(Store);
    }

    auto *GEP = Builder.CreateGEP(VL[0]->getType(), ArrPtr, IndVar);
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
      auto *GEP = dyn_cast<Instruction>(Builder.CreateGEP(GN->getReference()->getType(), Ptr, IndVarIdx));
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
    case NodeType::MINMAXREDUCTION: {
#ifdef TEST_DEBUG
      errs() << "Generating MINMAXREDUCTION\n";
#endif
      auto *RN = (MinMaxReductionNode*)N;

      assert(RN->getNumChildren() && "Expected child with varying operands!");
      Value *Op = cloneGraph(RN->getChild(0), Builder);
#ifdef TEST_DEBUG
      errs() << "Closing MINMAXREDUCTION\n";
#endif

      
      /*
      Instruction *NewI = RN->getBinaryOperator()->clone();
      for(unsigned i = 0; i<NewI->getNumOperands(); i++) {
        NewI->setOperand(i,nullptr);
      }
      Builder.Insert(NewI);
      CreatedCode.push_back(NewI);
      NodeToValue[N] = NewI;
      */
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
          SelectInst *Sel = dyn_cast<SelectInst>(I);
          Instruction *Cond = dyn_cast<Instruction>(Sel->getCondition());
          if (Cond) {
            if (std::find(Garbage.begin(), Garbage.end(), Cond)==Garbage.end())
	      Garbage.insert(Cond);
          }
        }
      }
      for (unsigned i = 0; i<RN->size(); i++) {
        if (auto *I = RN->getValidInstruction(i)) {
          if (std::find(Garbage.begin(), Garbage.end(), I)==Garbage.end())
	    Garbage.insert(I);
        }
      }

      PHINode *PHI = nullptr;
      if (Header->getFirstNonPHI()) {
        IRBuilder<> PHIBuilder(&*Header->getFirstInsertionPt());
        PHI = PHIBuilder.CreatePHI(Op->getType(),2);
      } else {
        PHI = Builder.CreatePHI(Op->getType(),2);
      }
      //IRBuilder<> PHIBuilder(Header->getFirstNonPHI());
      //PHINode *PHI = PHIBuilder.CreatePHI(NewI->getType(),2);
      CreatedCode.push_back(PHI);

      Value *Cond = nullptr;
      if (RN->getOperation()==MinMaxReductionNode::OperationType::MIN) {
        if (Op->getType()->isIntOrPtrTy()) {
          Cond = Builder.CreateICmpSLT(Op, PHI);
        } else if (Op->getType()->isFloatingPointTy()) {
          Cond = Builder.CreateFCmpOLT(Op, PHI);
        }
      } else if (RN->getOperation()==MinMaxReductionNode::OperationType::MAX) {
        if (Op->getType()->isIntOrPtrTy()) {
          Cond = Builder.CreateICmpSGT(Op, PHI);
        } else if (Op->getType()->isFloatingPointTy()) {
          Cond = Builder.CreateFCmpOGT(Op, PHI);
        }
      }
      if (Cond==nullptr) {
        errs() << "ERROR: INVALID Cond==nullptr\n";
        assert("What should I do? This is unexpected!");
      }
      CreatedCode.push_back(Cond);

      Instruction *NewI = dyn_cast<Instruction>(Builder.CreateSelect(Cond, Op, PHI));
      CreatedCode.push_back(NewI);
      NodeToValue[N] = NewI;

      PHI->addIncoming(NewI,Header);
      PHI->addIncoming(RN->getStartValue(),PreHeader);

      Extracted[RN->getSelection()] = NewI;//PHI;
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
  
  BB.getParent()->dump();
 
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
  if (G.Root->getNodeType() == NodeType::MINMAXREDUCTION) errs() << "MinMaxReduction ";
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
    errs() << "MINMAXREDUCTION: " << NodeFreq[NodeType::MINMAXREDUCTION] << "\n";
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

static SelectInst *getPossibleMinMaxReduction(Value *V) {
  if (!EnableExtensions) return nullptr;

  errs() << "getPossibleMinMaxReduction\n";
  if (V==nullptr) return nullptr;
  SelectInst *Sel = dyn_cast<SelectInst>(V);
  if (Sel==nullptr) return nullptr;

  SelectInst *SelT = dyn_cast<SelectInst>(Sel->getTrueValue());
  SelectInst *SelF = dyn_cast<SelectInst>(Sel->getFalseValue());
  SelectInst *RecSel = nullptr;
  Value *Val = nullptr;
  if (SelT) {
    RecSel = SelT;
    Val = Sel->getFalseValue();
  }
  if (SelF) {
    RecSel = SelF;
    Val = Sel->getTrueValue();
  }
  unsigned HasTwoConditions = 0;
  CmpInst *Cmp = dyn_cast<CmpInst>(Sel->getCondition());
  //if (Cmp && Cmp->isRelational()) {
  if (Cmp && !Cmp->isEquality()) {
    for (unsigned i = 0; i<Cmp->getNumOperands(); i++) {
      Cmp->getOperand(i)->dump();
      if (Cmp->getOperand(i)==RecSel) {
        errs() << "i:" << i << " selection\n";
        HasTwoConditions++;
      }
      if (Cmp->getOperand(i)==Val) {
        errs() << "i:" << i << " matching value\n";
        HasTwoConditions++;
      }
    }
  }
  if (HasTwoConditions!=2) return nullptr;

  return Sel;
  
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
      for (unsigned i = 0; i<CI->arg_size(); i++) {
        if (BinaryOperator *BO = getPossibleReduction(CI->getArgOperand(i))) {
#ifdef TEST_DEBUG
          //errs() << "Possible reduction\n";
	  //BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        } else if (SelectInst *Sel = getPossibleMinMaxReduction(CI->getArgOperand(i))) {
          errs() << "TODO: SELECTION : MINMAX\n";
          I.dump();
          Sel->dump();
          if (I.getParent()==Sel->getParent()) {
            errs() << "Added to working list\n";
            Seeds.MinMaxReductions[Sel] = &I;
          }
        }
      }
    }
    else if (auto *CB = dyn_cast<CallBase>(&I)) {
      for (unsigned i = 0; i<CB->arg_size(); i++) {
        if (BinaryOperator *BO = getPossibleReduction(CB->getArgOperand(i))) {
#ifdef TEST_DEBUG
          //errs() << "Possible reduction\n";
	  //BO->dump();
#endif
          Seeds.Reductions[BO] = &I;
        } else if (SelectInst *Sel = getPossibleMinMaxReduction(CB->getArgOperand(i))) {
          errs() << "TODO: SELECTION : MINMAX\n";
          I.dump();
          Sel->dump();
          if (I.getParent()==Sel->getParent()) {
            errs() << "Added to working list\n";
            Seeds.MinMaxReductions[Sel] = &I;
          }
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
        } else if (SelectInst *Sel = getPossibleMinMaxReduction(Br->getCondition())) {
          errs() << "TODO: SELECTION : MINMAX\n";
          I.dump();
          Sel->dump();
          if (I.getParent()==Sel->getParent()) {
            errs() << "Added to working list\n";
            Seeds.MinMaxReductions[Sel] = &I;
          }
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
      } else if (SelectInst *Sel = getPossibleMinMaxReduction(Ret->getReturnValue())) {
        errs() << "TODO: SELECTION : MINMAX\n";
        I.dump();
        Sel->dump();
          if (I.getParent()==Sel->getParent()) {
            errs() << "Added to working list\n";
            Seeds.MinMaxReductions[Sel] = &I;
          }
      }
    }
    else if (auto *PHI = dyn_cast<PHINode>(&I)) {
      //if (PHI->getNumIncomingValues()!=2) continue;
      errs() << "collecting possible seeds from PHI Nodes\n";
      PHI->dump();
      for (Value *V : PHI->incoming_values()) {
      //if (PHI->getBasicBlockIndex(PHI->getParent())>=0) {
        //Value *V = PHI->getIncomingValueForBlock(PHI->getParent());
	V->dump();
        if (BinaryOperator *BO = getPossibleReduction(V)) {
#ifdef TEST_DEBUG
          errs() << "Possible reduction\n";
	  BO->dump();
#endif
          if (I.getParent()==BO->getParent())
            Seeds.Reductions[BO] = &I;
        } else if (SelectInst *Sel = getPossibleMinMaxReduction(V)) {
          errs() << "TODO: SELECTION : MINMAX\n";
          I.dump();
          Sel->dump();
          if (I.getParent()==Sel->getParent()) {
            errs() << "Added to working list\n";
            Seeds.MinMaxReductions[Sel] = &I;
          }
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
    
    for (auto &Pair : Seeds.MinMaxReductions) {
      if (Pair.second==nullptr) continue;
      if (!isa<PHINode>(Pair.second)) continue; //skip non-phi nodes
      errs() << "HERE1\n";
      errs() << "Attempting alignment of min-max reduction starting with:";
      Pair.first->dump();
      Pair.second->dump();
      bool TryAgain = true;
      for (unsigned Skip = 0; Skip<=2; Skip++) {
        errs() << "HERE SKIP: " << Skip << "\n";
        AlignedGraph G(Pair.first, Pair.second, &BB, Skip, SE);
        errs() << "Alignment done!!\n";
         errs() << G.getDotString() << "\n";
        if (G.isSchedulable(BB)) {
          errs() << "GENERATING CODE FOR MIN MAX REDUCTION\n";
          NumAttempts++;
          CodeGenerator CG(F, BB, G);
          bool HasRolled = CG.generate(Seeds);
          Changed = Changed || HasRolled;
          errs() << "Has rolled:"<< HasRolled << "\n";
          errs() << "HERE2\n";
          if (HasRolled) {
            NumRolledLoops++;
            TryAgain = false;
          }
        }
        errs() << "Done with MinMaxReduction seed. Next...\n";
        errs() << "HERE3\n";
        G.destroy();
        if (!TryAgain) break;
      }
    }
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
    for (auto &Pair : Seeds.MinMaxReductions) {
      if (Pair.second==nullptr) continue;
      if (isa<PHINode>(Pair.second)) continue; //skip phi-nodes
      if (!Pair.second->isTerminator()) continue; //skip non-terminators
      errs() << "Attempting alignment of min-max reduction starting with:";
      Pair.first->dump();
      AlignedGraph G(Pair.first, Pair.second, &BB, 0, SE);
      if (G.isSchedulable(BB)) {
        errs() << "GENERATING CODE FOR MIN MAX REDUCTION\n";
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
    for (auto &Pair : Seeds.MinMaxReductions) {
      if (Pair.second==nullptr) continue;
      if (isa<PHINode>(Pair.second)) continue; //skip phi-nodes
      if (Pair.second->isTerminator()) continue; //skip terminators
      errs() << "Attempting alignment of min-max reduction starting with:";
      Pair.first->dump();
      AlignedGraph G(Pair.first, Pair.second, &BB, 0, SE);
      errs() << "Alignment done!!\n";
      if (G.isSchedulable(BB)) {
        errs() << "GENERATING CODE FOR MIN MAX REDUCTION\n";
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
  bool Changed = false;

  if (EnableExtensions) {
    RegionRoller RR(F);
    Changed = Changed || RR.run();
  }

  LoopRoller RL(F, SE);
  Changed = Changed || RL.run();
  
  return Changed;
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

/*
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
*/


