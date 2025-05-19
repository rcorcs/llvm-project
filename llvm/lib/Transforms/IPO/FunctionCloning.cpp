#include "llvm/Transforms/IPO/FunctionCloning.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Transforms/Utils/CodeMoverUtils.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"

#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/CommandLine.h"

#define DEBUG_TYPE "STATS_FUNCTION_CLONING"

#include <memory>
#include <string>
#include <cxxabi.h>

using namespace llvm;

static cl::opt<bool> EnableConstantsOnly("funcspec-constants-only",
                                          cl::init(false), cl::Hidden);

static cl::opt<bool> EnableFEDotFiles("funcexp-dot-files",
                                          cl::init(false), cl::Hidden);

static cl::opt<int> MinCallsites("funcexp-min-callsites",
                                          cl::init(1), cl::Hidden);

static cl::opt<int> MinMatches("funcexp-min-matches",
                                          cl::init(1), cl::Hidden);

static cl::opt<int> MaxMismatches("funcexp-max-mismatches",
                                          cl::init(1), cl::Hidden);

static cl::opt<int> MinTreeSize("funcexp-min-tree-size",
                                          cl::init(2), cl::Hidden);

static std::string demangle(const char* name) {
  int status = -1; 

  std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
  return (status == 0) ? res.get() : std::string(name);
}

void checkCall(FunctionType *FTy, Value *Func, ArrayRef<Value *> Args) {
  errs() << "Args.size(): " << Args.size() << "\n";
  errs() << "FTy->getNumParams(): " << FTy->getNumParams() << "\n";
  errs() << "FTy->isVarArg(): " << FTy->isVarArg() << "\n";

  for (unsigned i = 0; i != Args.size(); ++i) {
    errs() << "i: " << i << "\n";
    errs() << "Args[i]->getType(): "; Args[i]->getType()->dump();
    errs() << "FTy->getParamType(i): "; FTy->getParamType(i)->dump();
  }
}

void deleteInstructionsSafely(std::set<Instruction*>& toDeleteSet) {

    // Step 2: Reorder instructions so that users are deleted before the instructions they use
    std::vector<Instruction*> ordered;

    std::set<Instruction*> visited;

    std::function<void(Instruction*)> dfs = [&](Instruction* I) {
        if (visited.count(I)) return;
        visited.insert(I);

        for (auto& use : I->uses()) {
            if (Instruction* user = dyn_cast<Instruction>(use.getUser())) {
                if (toDeleteSet.count(user))
                    dfs(user);
            }
        }

        ordered.push_back(I);
    };

    for (Instruction* I : toDeleteSet) {
        dfs(I);
    }

    // Step 3: Delete in the safe order
    for (Instruction* I : ordered) {
        if (I->use_empty()) {
            I->eraseFromParent();
        } else {
            // Optional: Replace uses with UndefValue to allow deletion
            I->replaceAllUsesWith(UndefValue::get(I->getType()));
            I->eraseFromParent();
        }
    }
}

void deleteInstructionsSafely(std::vector<Instruction*>& toDelete) {
   // Step 1: Create a set for fast lookup
   std::set<Instruction*> toDeleteSet(toDelete.begin(), toDelete.end());
   deleteInstructionsSafely(toDeleteSet);
}


__attribute__((noinline))
static bool match(Value *V1, Value *V2) {
  //if (V1==V2) return true;
  if (V1->getType()!=V2->getType()) return false;
  if (isa<Instruction>(V1) && isa<Instruction>(V2)) {
    Instruction *I1 = dyn_cast<Instruction>(V1);
    Instruction *I2 = dyn_cast<Instruction>(V2);

    //default check
    return (I1->getOpcode()==I2->getOpcode() && I1->getNumOperands()==I2->getNumOperands());
  }
  return ((isa<Constant>(V1) || isa<GlobalValue>(V1)) && V1==V2);
  //return false;
}

class CallMatching {
//private:
public:
  enum NodeType {
    None,
    Matching,
    Mismatching,
    Reuse 
  };

  class Node {
  //private:
  public:
    Value *V;
    std::vector< std::pair<Value*, CallInst*> > MatchingValues;
    std::vector<Node *> Children;
    Node *Parent;
    NodeType NT;

  public:
    Node(Value *V, Node *Parent=nullptr) : V(V), Parent(Parent), NT(NodeType::None) {}

    std::vector< std::pair<Value*, CallInst*> > Values;

    void addValue(Value *V, CallInst *CI) {
      //errs() << "Adding value to node: " << getString() << "\n";
      //errs() << "value: "; V->dump();
      //errs() << "ci: "; CI->dump();
      Values.push_back( std::pair<Value*, CallInst*>(V,CI) );
    }

    void addMatch(Value *V, CallInst *CI) {
      MatchingValues.push_back( std::pair<Value*, CallInst*>(V,CI) );
    }

    size_t getNumMatches() {
      return MatchingValues.size();
    }

    void setNodeType( NodeType NTy ) { NT = NTy; }

    NodeType getNodeType() {
      return NT;
    }

    Node *getParent() { return Parent; }

    void pushChild( Node * N ) { Children.push_back(N); }

    Value *getValue() { return V; }
    const std::vector< std::pair<Value*, CallInst*> > &getMatchingValues() { return MatchingValues; }
    
    const std::vector< Node * > &getChildren() { return Children; }
    void clearChildren() { Children.clear(); }

    bool isFunction() { return isa<Function>(V); }
    bool isCallInst() { return isa<CallInst>(V); }

    std::string getString() {
      std::string str;
      raw_string_ostream labelStream(str);
      if (Instruction *I = dyn_cast<Instruction>(V)) {
        labelStream << I->getOpcodeName();
        if (CallInst *CI = dyn_cast<CallInst>(I)) {
          Function *F = CI->getCalledFunction();
          //if (F && F->hasName())
          if (F)
            labelStream << ": " << F->getName().str();
            //labelStream << ": " << demangle(F->getName().data());
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
        labelStream << F->getName().str();
      }

      return labelStream.str();
    }

    void dump() {
      if (getNumMatches()==0) return;
      if (Instruction *I = dyn_cast<Instruction>(V)) {
        errs() << Instruction::getOpcodeName(I->getOpcode());
        if (CallInst *CI = dyn_cast<CallInst>(I)) {
          Function *F = CI->getCalledFunction();
          if (F && F->hasName())
            errs() << " " << demangle(F->getName().data());
        }
        errs() << "\n";
      } else if (isa<Constant>(V)) {
        if (Function *F = dyn_cast<Function>(V)) {
          errs() << "func: " << F->getName().str() << "\n";
        } else V->dump();
      }
    }
  };

  class Tree {
  public:
    Node *Root;
    std::vector<Node*> Nodes;

    Tree(Node *Root) : Root(Root) { Nodes.push_back(Root); }
    void addNode(Node *N) { Nodes.push_back(N); if (Root==nullptr) Root = N; }

    unsigned countMatchingNodes() {
      unsigned Count = 0;
      for (Node *N : Nodes) {
        Count += N->getNumMatches()?1:0;
      }
      return Count;
    }

    void dump() {
      for (Node *N : Nodes) {
        N->dump();
      }
    }

    std::string getDotString() {
      std::string dotStr;
      raw_string_ostream os(dotStr);
      os << "digraph VTree {\n";

      std::map<Node*, int> NodeId;

      int id = 0;
      //Nodes
      for (Node *N : Nodes) {
        if (N->isFunction()) continue;
        id++;
        NodeId[N] = id;

        bool Internalizable = N->getNumMatches()>0;

        os << id << " [label=\"" << N->getString() << "\""
          << "style=\"filled\" , fillcolor=" << ((Internalizable) ? "\"#8ae18a\"" : "\"#ff6671\"")
          << ", shape=" << ((Internalizable) ? "box" : "oval") << "];\n";
      }

      //Edges
      for (Node *N : Nodes) {
        int ChildId = 0;
        for (Node *Child : N->getChildren()) {
          if (Child->isFunction()) continue;
          std::string EdgeLabel = "";
          if (N->isCallInst()) EdgeLabel = std::string(" [label=\"") + std::to_string(ChildId) + std::string("\"]");
          os << NodeId[N] << " -> " << NodeId[Child] << EdgeLabel << "\n";

          ChildId++;
        }
      }
  
      os << "}\n";
      return os.str();
    }

  };


  class CrossEdgeMatching {
  public:
    std::pair<Value *, CallInst *> VPair;
    std::pair<Value *, CallInst *> SrcVPair;
    CrossEdgeMatching( Value *V, CallInst *CI, Value *SrcV, CallInst *SrcCI )
      : VPair(V,CI), SrcVPair(SrcV, SrcCI) {}
    
  };

  class CrossEdge {
  private:
    Node *N;
    Node *SrcN;

    std::vector< CrossEdgeMatching > MatchingEdges;
  public:
    CrossEdge(Node *N, Node *SrcN) : N(N), SrcN(SrcN) {}
 
    Node *getNode() { return N; }
    Node *getSourceNode() { return SrcN; }
 
    void addCrossEdgeMatching( Value *V, CallInst *CI, Value *SrcV, CallInst *SrcCI ) {
      MatchingEdges.push_back( CrossEdgeMatching(V,CI,SrcV,SrcCI) );
    }

    unsigned getNumMatches() {
      return MatchingEdges.size();
    }
 };


  CallInst *CI;
  std::vector<CallInst *> &AllCIs;
  std::vector<Tree> Trees;
  Node *Root;
  std::set<Node*> AllNodes;
  
  std::map<Value*, std::list<CrossEdge>> CrossEdges;

  void destroyNodesRec( Node *N, std::set<Node *> *Deleted=nullptr);
  unsigned growTreeNode( Node *N , Tree &T);
  void buildTrees(bool);

public:
  unsigned Cost;
  //unsigned NumMatches;
  bool HasMultiNode;
  unsigned PrunedMatches;
  unsigned PrunedNodes;

  CallMatching(CallInst *CI, std::vector<CallInst *> &AllCIs, bool ConstantsOnly=false) : HasMultiNode(false), CI(CI), AllCIs(AllCIs) { buildTrees(ConstantsOnly); }
  ~CallMatching();
  
  int getNumCalls() { return AllCIs.size(); }
  int getWidth() { return Root->Values.size(); }

  void validityPruning();

  bool validate() {
    int expectedWidth = getWidth();
    bool Valid = true;
    errs() << "Expected width: " << expectedWidth << "\n";
    errs() << "Printing number of values in nodes\n";
    for (Node *N : AllNodes) {
      errs() << "Node: " << N->getString() << " => " << N->Values.size() << "\n";
      //if (N->getNodeType()
      //Valid = Valid && N->Values.size()==expectedWidth;
      for (auto &Pair : N->Values) {
        Valid = Valid && Pair.first->getType()==N->Values[0].first->getType();
      }
    }
    errs() << "done!\n";
    errs() << "Valid? " << Valid << "\n";
    return Valid;
  } 

  void dump() {
    unsigned ArgId = 0;
    for (Tree &T : Trees) {
      errs() << "\n";
      errs() << "Arg: " << ArgId << "; ";
      errs() << "Root Matches: " << T.Root->getNumMatches() << "; ";
      errs() << "Num Nodes: " << T.countMatchingNodes() << "\n";
      T.dump();
      ArgId++;
    }
  }

  void writeDotFile();
  Function *generateExpandedFunction(Module &M, std::list<Function *> &WorklistFns, std::list<CallInst*> &WorklistCalls);
  Value *generate(IRBuilder<> &Builder, Node *N, std::map<Node *, unsigned> &NodeToArgNo, std::vector<Argument *> &Args, std::vector<Instruction*> &Covered);

};

CallMatching::~CallMatching() {
  for (Tree &T : Trees) destroyNodesRec(T.Root);
  delete Root;
}

__attribute__((noinline))
static bool isInternalizable(Value *V) {
  Instruction *I = dyn_cast<Instruction>(V);
  if (I) {
    switch(I->getOpcode()) {
    case Instruction::PHI:
    case Instruction::Invoke:
      return false;
    }
    unsigned NumUsers = 0;
    for (auto U : I->users()) NumUsers++;
    if (NumUsers>1) return false;
  }
  return true;
}

void CallMatching::buildTrees(bool ConstantsOnly) {
  Cost = 0;
  //NumMatches = 0;
  Root = new Node(CI);
  AllNodes.insert(Root);
  Root->addValue(CI, CI);
  for (CallInst *OtherCI : AllCIs) {
    if (OtherCI==CI) continue;
    Root->addMatch(OtherCI, OtherCI);
    Root->addValue(OtherCI, OtherCI);
  }
  Root->setNodeType( NodeType::Matching );

  //errs() << "Root numMatches: " << Root->getNumMatches() << "\n";

  for (unsigned i = 0; i<CI->getNumArgOperands(); i++) {
    Value *V = CI->getArgOperand(i);

    Node *N = new Node(V,Root);
    //errs() << "Creating node: " << N->getString() << ": value "; V->dump();
    AllNodes.insert(N);
    Root->pushChild(N);
    Trees.push_back(Tree(N));

    N->addValue(V, CI);
    for (CallInst *OtherCI : AllCIs) {
      if (OtherCI==CI) continue;
      Value *OtherV = OtherCI->getArgOperand(i);
      N->addValue(OtherV, OtherCI);
    }
    N->setNodeType(NodeType::Mismatching);
    
    if (ConstantsOnly) {
      if (isa<Constant>(V)) {
        //errs() << "matching arg " << i << ": value "; V->dump();a
        for (CallInst *OtherCI : AllCIs) {
          if (OtherCI==CI) continue;
          Value *OtherV = OtherCI->getArgOperand(i);
          Instruction *OtherI = dyn_cast<Instruction>(OtherV);
          if (OtherI && OtherI->getParent()==OtherCI->getParent()) {
            if (match(V, OtherV)) N->addMatch(OtherV, OtherCI);
          }
        }
        
        errs() << "numMatches: " << N->getNumMatches() << "\n";
        //if (N->getNumMatches()==Root->getNumMatches()) {
        //  NumMatches += 1;
        //}
        //if (N->getNumMatches()>0) NumMatches += 1;
        //if (N->getNumMatches()>0) { //TODO: check condition
        if ((N->Values.size()>1 && N->getNumMatches()==Root->getNumMatches())
           || (N->Values.size()==1 && isa<Constant>(N->getValue()))) { //TODO: check condition
          N->setNodeType(NodeType::Matching);
          Cost += growTreeNode(N, Trees[Trees.size()-1]);
        } else N->MatchingValues.clear();
      }
    } else {
      if (isInternalizable(V)) {
        for (CallInst *OtherCI : AllCIs) {
          if (OtherCI==CI) continue;
          Value *OtherV = OtherCI->getArgOperand(i);
          //if (match(V, OtherV)) N->addMatch(OtherV, OtherCI);
          Instruction *OtherI = dyn_cast<Instruction>(OtherV);
          if (OtherI && OtherI->getParent()==OtherCI->getParent()) {
            if (match(V, OtherV)) N->addMatch(OtherV, OtherCI);
          }
        }
        //if (N->getNumMatches()>0) { //TODO: check condition
        //if (N->getNumMatches()==Root->getNumMatches()) { //TODO: check condition
        if ((N->Values.size()>1 && N->getNumMatches()==Root->getNumMatches())
           || (N->Values.size()==1 && isa<Constant>(N->getValue()))) { //TODO: check condition
          N->setNodeType(NodeType::Matching);
          Cost += growTreeNode(N, Trees[Trees.size()-1]);
        } else N->MatchingValues.clear();
      }
    }
  }

  //ConstantsOnly does not include arg reuse optimization
  if (ConstantsOnly) return; 

  
  validityPruning();

  errs() << "Done pruning\n";

  std::map<Value*, Node*> FirstInstanceNode;
  for (unsigned i = 0; i<CI->getNumArgOperands(); i++) {
    for (Node *N : Trees[i].Nodes) {
      if (N->isFunction()) continue;
       
      if (FirstInstanceNode.find(N->getValue())==FirstInstanceNode.end()) {
        FirstInstanceNode[N->getValue()] = N; 
      } else {
        bool Internalizable = N->getNumMatches()>0;
        if (Internalizable) continue;

        Node *SrcNode = FirstInstanceNode[N->getValue()];
        CrossEdge CE(N, SrcNode);

        Node *SrcParent = SrcNode->getParent();
        Node *Parent = N->getParent();
        
        if (Parent!=nullptr && SrcParent!=nullptr) {
          int SrcOpId = 0;
          for (Node *Child : SrcParent->getChildren()) {
            if (Child==SrcNode) break;
            SrcOpId++;
          }
  
          int OpId = 0;
          for (Node *Child : Parent->getChildren()) {
            if (Child==N) break;
            OpId++;
          }

          for (auto &Pair : Parent->getMatchingValues()) {
            for (auto &SrcPair : SrcParent->getMatchingValues()) {
              if (Pair.second==SrcPair.second) {
                Instruction *Inst = dyn_cast<Instruction>(Pair.first);
                Instruction *SrcInst = dyn_cast<Instruction>(SrcPair.first);
                if (Inst==nullptr || SrcInst==nullptr) {
                  errs() << "WEIRD!\n"; continue;
                }
                if (Inst->getOperand(OpId)==SrcInst->getOperand(SrcOpId)) {
                  CE.addCrossEdgeMatching( Inst->getOperand(OpId), Pair.second, SrcInst->getOperand(SrcOpId), SrcPair.second );
                  break;
                }
              }
            }
          }
        }
        N->setNodeType(NodeType::Reuse);
        CrossEdges[N->getValue()].push_back( CE );
        Cost += CE.getNumMatches();
      }
    }
  }

}

unsigned CallMatching::growTreeNode( Node *N , Tree &T) {
  unsigned Cost = N->Values.size(); //N->getNumMatches();
  Value *V = N->getValue();
  if (N->getNumMatches()==0) return Cost;
  if (Instruction *I = dyn_cast<Instruction>(V)) {
    bool GrowOpcode = true;
    switch(I->getOpcode()) {
    case Instruction::PHI:
    //case Instruction::Load:
    case Instruction::Invoke:
      GrowOpcode = false;
      break;
    }
    if (!GrowOpcode) return Cost;
 
    for (unsigned i = 0; i<I->getNumOperands(); i++) {
      Value *ChildV = I->getOperand(i);
      Node *ChildN = new Node(ChildV, N);
      ChildN->setNodeType(NodeType::Mismatching);
      //errs() << "Creating node: " << ChildN->getString() << ": value "; ChildV->dump();
      AllNodes.insert(ChildN);
      T.addNode(ChildN);
      ChildN->addValue(ChildV, CI);

      auto *BO = dyn_cast<BinaryOperator>(V);
      auto *ChildBO = dyn_cast<BinaryOperator>(ChildV);
      if (BO && ChildBO && BO->getOpcode()==ChildBO->getOpcode() && BO->isCommutative()) {
        errs() << "Multi-node found\n";
        HasMultiNode = true;
      }

      for (auto &Pair : N->getMatchingValues()) {
        CallInst *OtherCI = Pair.second;
        Value *OtherV = dyn_cast<Instruction>(Pair.first)->getOperand(i);
        ChildN->addValue(OtherV, OtherCI);
      }
      if (isInternalizable(ChildV)) {
        for (auto &Pair : N->getMatchingValues()) {
          CallInst *OtherCI = Pair.second;
          Value *OtherV = dyn_cast<Instruction>(Pair.first)->getOperand(i);
          //ChildN->addValue(OtherV, OtherCI);
          //if (match(ChildV, OtherV)) ChildN->addMatch(OtherV, OtherCI);
          Instruction *OtherI = dyn_cast<Instruction>(OtherV);
          if (OtherI && OtherI->getParent()==OtherCI->getParent()) {
            if (match(ChildV, OtherV)) ChildN->addMatch(OtherV, OtherCI);
          }
        }
        //if (ChildN->getNumMatches()>0) { //TODO: check condition
        //if (ChildN->getNumMatches()==Root->getNumMatches()) { //TODO: check condition
        if ((ChildN->Values.size()>1 && ChildN->getNumMatches()==Root->getNumMatches())
           || (ChildN->Values.size()==1 && isa<Constant>(ChildN->getValue()))) { //TODO: check condition
          ChildN->setNodeType(NodeType::Matching);
          Cost += growTreeNode(ChildN, T);
        } else ChildN->MatchingValues.clear();
      }
      N->pushChild(ChildN);
    }
  }
  return Cost;
}

void CallMatching::destroyNodesRec( Node *N, std::set<Node *> *Deleted ) {
  for (Node *Child : N->getChildren()) {
    destroyNodesRec(Child, Deleted);
  }
  AllNodes.erase(N);
  for (Tree &T : Trees) {
    //T.Nodes.erase(N);
    T.Nodes.erase(std::remove(T.Nodes.begin(), T.Nodes.end(), N), T.Nodes.end());
  }
  if (Deleted) Deleted->insert(N);
  delete N;
}


Value *CallMatching::generate(IRBuilder<> &Builder, Node *N, std::map<Node *, unsigned> &NodeToArgNo, std::vector<Argument *> &Args, std::vector<Instruction*> &Covered) {

  if (NodeToArgNo.find(N)!=NodeToArgNo.end()) {
    unsigned ArgNo = NodeToArgNo[N];
    errs() << "Node " << N->getString() << " mapped to ArgNo " << ArgNo << "\n";
    Value *V = Args[ArgNo];
    //errs() << "Mismatching node mapped to argument "; V->dump();
    return V;
  }
  if (CrossEdges.find(N->getValue())!=CrossEdges.end()) {
    for (auto &CE : CrossEdges[N->getValue()]) {
      if (CE.getNode()==N) {
        Node *SrcN = CE.getSourceNode();
        if (NodeToArgNo.find(SrcN)!=NodeToArgNo.end()) {
          //errs() << "SrcN value ";
          //SrcN->getValue()->dump();
          unsigned ArgNo = NodeToArgNo[SrcN];
          //errs() << "Node " << SrcN->getString() << " mapped to ArgNo " << ArgNo << "\n";
          Value *V = Args[ArgNo];
          //errs() << "Mismatching node mapped to argument "; V->dump();
          return V;
        }
        errs() << "ERROR: Should reused a previously produced value?\n";
      }
    }
  }
  
  if (isa<Constant>(N->getValue())) {
    //errs() << "Constant "; N->getValue()->dump();
    return N->getValue();
  }

  if (Instruction *I = dyn_cast<Instruction>(N->getValue())) {
    //errs() << "Cloning: "; I->dump();
    Instruction *NewI = I->clone();

    //Covered.push_back(I);
    for (auto &Pair : N->Values) {
      if (Instruction *CoveredI = dyn_cast<Instruction>(Pair.first)) {
        Covered.push_back(CoveredI);
      }
    }

    auto &Children = N->getChildren();
    for (unsigned i = 0; i<Children.size(); i++) {
      Node *CN = Children[i];
      if (CN==nullptr) {
        errs() << "ERROR: null CN?\n";
        break;
      }
      //errs() << "CN: " << CN->getString() << "\n";
      Value *NewV = generate(Builder, CN, NodeToArgNo, Args, Covered);
      NewI->setOperand(i, NewV);
    }

    Builder.Insert(NewI);
    //errs() << "NewI: "; NewI->dump();
    return NewI;
  }

  return nullptr;
}

Function *CallMatching::generateExpandedFunction(Module &M, std::list<Function *> &WorklistFns, std::list<CallInst*> &WorklistCalls) {

  std::vector<Type*> ArgTypes;
  std::vector<Node*> ArgNodes;
  std::map<Node *, unsigned> NodeToArgNo;
  
  errs() << "generateExpandedFunction...\n";
  //for (Tree &T : Trees) errs() << T.getDotString() << "\n";

  //errs() << "Generating expanded functions\n";
  for (Node *N : AllNodes) {
    if (N->getNodeType()==NodeType::Mismatching) {
      //errs() << "Arg:"; N->getValue()->dump();
      //errs() << "Num Values: " << N->Values.size() << "\n";
      
      NodeToArgNo[N] = ArgTypes.size();
      ArgTypes.push_back(N->getValue()->getType());
      ArgNodes.push_back(N);
    }
  }
  /*
  for (Tree &T : Trees) {
    //Nodes
    for (Node *N : T.Nodes) {
      //if (N->isFunction()) continue;
      errs() << "Node: " << N->getString() << "\n";
      for (auto &Pair : N->Values) Pair.first->dump();
      bool Internalizable = N->getNumMatches()>0;
      bool ReusedInput = false;
      if (!Internalizable) {
        if (CrossEdges.find(N->getValue())!=CrossEdges.end()) {
          for (auto &CE : CrossEdges[N->getValue()]) {
            if (CE.getNode()==N) {
              //NodeToArgNo[N] = 
              ReusedInput = true;
              break;
            }
          }
        }
      }
      errs() << "Internalizable:" << Internalizable << "\n";
      errs() << "ReusedInput:" << ReusedInput << "\n";
      //if (!Internalizable && !ReusedInput) {
      if (N->getNodeType()==NodeType::Mismatching) {
        errs() << "Arg:"; N->getValue()->dump();
        errs() << "Num Values: " << N->Values.size() << "\n";
        
        NodeToArgNo[N] = ArgTypes.size();
        ArgTypes.push_back(N->getValue()->getType());
        ArgNodes.push_back(N);
      }
    }
  }
  */
  errs() << "New function type:\n";
  for (Type *T : ArgTypes) {
    T->dump();
  }

  LLVMContext &Context = M.getContext();

  Type *RetTy = CI->getType(); //Type::getVoidTy(Context);
  //if (!RemoveReturnValue) {
  //  RetTy = FTy->getReturnType();
  //}
  
  FunctionType *NewFTy = FunctionType::get(RetTy, ArrayRef<Type *>(ArgTypes), false);

  std::string Name = std::string("c"); // + std::string(F->getName().str());

  //Function *ClonedF = Function::Create(NewFTy, GlobalValue::LinkageTypes::InternalLinkage,
  Function *ClonedF = Function::Create(NewFTy, CI->getCalledFunction()->getLinkage(),
                                       Twine(Name), M);
  //ClonedF->setCallingConv(CI->getCalledFunction()->getCallingConv());
  //ClonedF->addFnAttr(Attribute::NoInline);
  std::vector<Argument *> Args;
  for (unsigned i = 0; i<ClonedF->arg_size(); i++) {
    Args.push_back(ClonedF->getArg(i));
  }

  if (Args.size()!=ArgTypes.size()) errs() << "ERROR: Wrong number of arguments\n";

  //errs() << "Generating function code\n";
  //errs() << "Root: " << Root->getString() << "\n";
  //for (Node *CN : Root->getChildren()) {
  //  errs() << "CN: " << CN->getString() << "\n";
  //}

  BasicBlock *BB = BasicBlock::Create(Context, "", ClonedF);
  IRBuilder<> Builder(BB);
  std::vector<Instruction*> Covered;
  //Covered.push_back(CI);
  Value *RootV = generate(Builder, Root, NodeToArgNo, Args, Covered);
  
  if (RetTy->isVoidTy()) {
    Builder.CreateRetVoid();
  } else if (RootV) {
    Builder.CreateRet(RootV);
  } else {
    errs() << "ERROR: return value should not be null\n";
    ClonedF->eraseFromParent();
    return nullptr;
  }
  //BB->dump();
  //ClonedF->dump();
  if (verifyFunction(*ClonedF,&errs())) {
    errs() << "ERROR: generated broken function\n";
    ClonedF->eraseFromParent();
    return nullptr;
  }
  

  //std::vector<CallInst *> &AllCIs;
  //std::vector<Tree> Trees;
  //errs() << "Num CIs: " << AllCIs.size() << "\n";
  //errs() << "Num Trees: " << Trees.size() << "\n";

  std::map< CallInst *, std::vector<Value *> > ArgValues;

  for (unsigned i = 0; i<ArgNodes.size(); i++) {
    Node *N = ArgNodes[i];
    //errs() << "ArgNo " << i << "\n";
    //N->getValue()->dump();
    for (auto &Pair : N->Values) {
      //errs() << "for CI: "; Pair.second->dump();
      //errs() << "use arg: "; Pair.first->dump();
      ArgValues[Pair.second].push_back(Pair.first);
    }
  }

  int NumCIs = AllCIs.size();
  for (CallInst *CI : AllCIs) {
    //errs() << "OldCI: "; CI->dump();
    //for (unsigned i = 0; i<ArgValues[CI].size(); i++) {
    //  errs() << "arg " << i << ":";
    //  ArgValues[CI][i]->dump();
    //}
    //errs() << "Here\n";
    IRBuilder<> Builder(CI);
    //NewFTy->dump();
    //ClonedF->dump();
    //errs() << "args size: " << ArgValues[CI].size() << "\n";
    //for (Value *V : ArgValues[CI]) V->dump();
    //errs() << "Creating call\n";
    checkCall(NewFTy, ClonedF, ArgValues[CI]);
    Value *NewCI = Builder.CreateCall(NewFTy, ClonedF, ArgValues[CI]);
    //errs() << "Call created\n";
    //errs() << "NewCI: "; NewCI->dump();
    CI->replaceAllUsesWith(NewCI);
  }

  CI->getParent()->getParent()->dump();
  //errs() << "TO DELETE\n";
  //std::set<Instruction *> Deleted;

  //delete instructions that have been internalized
  for (Instruction *I : Covered) {
    //if (Deleted.count(I)) continue;
    //I->dump();
    if (CallInst *CI = dyn_cast<CallInst>(I)) WorklistCalls.remove(CI);
     //Deleted.insert(I);
    //I->eraseFromParent();
  }
  deleteInstructionsSafely(Covered);
  
  std::vector<CallInst*> Calls;
  for (Instruction &I : *BB) {
    if (CallInst *CI = dyn_cast<CallInst>(&I)) {
      Calls.push_back(CI);
    }
  }
  int NumMatches = 0;
  int NumMismatches = 0;
  int NumReuse = 0;
  for (CallMatching::Node *N : AllNodes) {
    //errs() << "Node: " << N->getString() << " : ";
    if (N->getNodeType()==CallMatching::NodeType::Matching) {
      NumMatches++;
      //errs() << " match\n";
    } else if (N->getNodeType()==CallMatching::NodeType::Mismatching) {
      NumMismatches++;
      //errs() << " mismatch\n";
    } else if (N->getNodeType()==CallMatching::NodeType::Reuse) {
      NumReuse++;
      //errs() << " reuse\n";
    }
  }

  errs() << "Specialized: #nodes " << AllNodes.size() <<
            "; #matches " << NumMatches <<
            "; #mismatches " << NumMismatches <<
            "; #reuse " << NumReuse <<
            "; #p_nodes " << PrunedNodes <<
            "; #p_matches " << PrunedMatches <<
            "; #cs " << NumCIs <<
            "; #args " << ArgNodes.size() <<
            "; new_fsize " << ClonedF->getInstructionCount() << "; new_arg_size " << ClonedF->arg_size() <<
            "; old_fsize " << CI->getCalledFunction()->getInstructionCount() << "; old_arg_size " << CI->getCalledFunction()->arg_size() << "\n";
  ClonedF->dump();

  std::set<Function*> Fns;
  //ClonedF->eraseFromParent();
  for (CallInst *CI : Calls) {
    if (CI==RootV) {
      Function *Callee = CI->getCalledFunction();
      if (Callee) Fns.insert(Callee);
      InlineFunctionInfo IFI;
      InlineFunction(*CI, IFI);
    } else if (CI->getNumUses()==1) {
      Function *Callee = CI->getCalledFunction();
      if (Callee) Fns.insert(Callee);
      InlineFunctionInfo IFI;
      InlineFunction(*CI, IFI);
    } //TODO: inline if function is small enough
  }
  //ClonedF->dump();

  for (auto *Callee : Fns) {
    if (Callee->getNumUses()==0) {
      WorklistFns.remove(Callee);
      Callee->eraseFromParent();
    }
  }
  

  return ClonedF;
}

void CallMatching::validityPruning() {
  //errs() << "validityPruning:\n";
  //errs() << "Computing scheduling order...\n";
  // Preserve the original order inside the list so dependencies among the
  // candidates themselves are kept.
  if (AllNodes.size() < 2) return;

  //SmallVector<Node *, 8> Ordered(AllNodes.begin(), AllNodes.end());
  SmallVector<Node *, 8> Ordered;
  for (Node *N : AllNodes) {
    if (N->getNodeType()==CallMatching::NodeType::Matching) Ordered.push_back(N);
  }

  llvm::sort(Ordered, [](Node *A, Node *B) {
    if (A==nullptr) return !true;
    if (B==nullptr) return !false;
    Instruction *AI = dyn_cast<Instruction>(A->getValue());
    Instruction *BI = dyn_cast<Instruction>(B->getValue());
    if (AI==nullptr) return !true;
    if (BI==nullptr) return !false;
    return !AI->comesBefore(BI);
  });

  /*
  for (Node *N : Ordered) {
    errs() << "Node: " << N->getString() << "\n";
    for (auto Pair : N->Values) {
      errs() << "- value:";
      if (Function *VF = dyn_cast<Function>(Pair.first)) {
        errs() << " " << VF->getName().str() << "\n";
      } else {
        Pair.first->dump();
      }
    }
  }
  */
  //errs() << "first node is root? " << (Ordered[0]==Root) << "\n";
  errs() << "computing scheduling validity...\n";

  std::map<CallInst*, Instruction*> Position;

  std::map<CallInst*, Function*> Fns;
  std::map<Function*, DominatorTree> DTs;
  std::map<Function*, PostDominatorTree> PDTs;

  std::map<Function*, LoopInfo> LIs;

  std::map<Function*, DependenceInfo*> DIs;
  std::map<Function*, ScalarEvolution*> SEs;
  std::map<Function*, AAResults*> AARs;
  std::map<Function*, BasicAAResult*> BAARs;
  std::map<Function*, AssumptionCache*> ACs;

  TargetLibraryInfoImpl TLII;
  TargetLibraryInfo TLI(TLII);

  for (auto Pair : Root->Values) {
    Position[Pair.second] = dyn_cast<Instruction>(Pair.first);
    Function *F = Pair.second->getParent()->getParent();
    Fns[Pair.second] = F;
    DTs[F] = DominatorTree(*F);
    PDTs[F] = PostDominatorTree(*F);
    LIs[F] = LoopInfo(DTs[F]);

    ACs[F] = new AssumptionCache(*F);

    SEs[F] = new ScalarEvolution(*F, TLI, *ACs[F], DTs[F], LIs[F]);

    AARs[F] = new AAResults(TLI);

    auto &DL = F->getParent()->getDataLayout();

    BAARs[F] = new BasicAAResult(DL, *F, TLI, *ACs[F], &DTs[F]);
    AARs[F]->addAAResult(*BAARs[F]);

    DIs[F] = new DependenceInfo(F, AARs[F], SEs[F], &LIs[F]);
  }
  
  std::set<Node *> IgnoreNodes;
  unsigned NewMismatches = 0;
  for (int idx = 1; idx<Ordered.size(); idx++) {
    Node *N = Ordered[idx];
    if (IgnoreNodes.count(N)) continue;

    if (N->getNodeType()==CallMatching::NodeType::Matching) {
      bool AllSchedulable = true;
      for (auto Pair : N->Values) {
        if (Instruction *I = dyn_cast<Instruction>(Pair.first)) {
          if (!isSafeToMoveBefore(*I, *Position[Pair.second], DTs[Fns[Pair.second]], &PDTs[Fns[Pair.second]], DIs[Fns[Pair.second]])) {
            errs() << "Not movable: "; I->dump();
            AllSchedulable = false;
          }
        }
      }
      if (AllSchedulable) {
        for (auto Pair : N->Values) {
          if (Instruction *I = dyn_cast<Instruction>(Pair.first)) {
            I->moveBefore(Position[Pair.second]);
            Position[Pair.second] = I;
          }
        }
      } else {
        NewMismatches++;
        //errs() << "Take node away: " << N->getString() << "\n";
        N->setNodeType(NodeType::Mismatching);
        //errs() << "AllNodes before: " << AllNodes.size() << "\n";
        //for (Tree &T : Trees) errs() << "T size: " << T.Nodes.size() << "\n";
        for (Node *Child : N->getChildren()) {
          destroyNodesRec( Child, &IgnoreNodes );
        }
        N->Children.clear();
        //errs() << "AllNodes after: " << AllNodes.size() << "\n";
        //for (Tree &T : Trees) errs() << "T size: " << T.Nodes.size() << "\n";
      }
    }
  }
  errs() << "Mismatch transitions: " << NewMismatches << "\n";
  errs() << "Number of Pruned Nodes: " << IgnoreNodes.size() <<  "\n";
  PrunedMatches = NewMismatches;
  PrunedNodes = IgnoreNodes.size();

  //errs() << "Scheduled\n";
  //for (auto Pair : Fns) Pair.second->dump();
  for (auto Pair : ACs) { delete Pair.second; }
  for (auto Pair : SEs) { delete Pair.second; }
  for (auto Pair : AARs) { delete Pair.second; }
  for (auto Pair : BAARs) { delete Pair.second; }
  for (auto Pair : DIs) { delete Pair.second; }
}


void CallMatching::writeDotFile() {
  std::string PrefixName = std::to_string(Cost) + std::string(".") + demangle(CI->getParent()->getParent()->getName().data());

  std::string CIName = "";
  if (CI->getCalledFunction()->hasName())
    CIName = demangle(CI->getCalledFunction()->getName().data());

  PrefixName += std::string(".") + CIName + std::string(".") + std::string(CI->getName());

  std::string FileName = PrefixName + std::string(".dot");

  std::error_code ec;
  raw_fd_ostream os (FileName, ec); //, sys::fs::F_Text);

  os << "digraph VTree {\nrankdir=BT\n";
  std::map<Node*, int> NodeId;
  std::map<Value*, int> ReusedInputId;
  int Id = 0;
  int CallId = Id++;

  /*
  os << CallId << " [margin=0 style=\"filled\" fillcolor=\"white\" shape=\"plaintext\" label=< <table border=\"0\" cellborder=\"1\" cellspacing=\"0\"><tr>";
  for (int ArgId = 0 ; ArgId < CI->getNumArgOperands(); ArgId++) {
    os << "<td port=\"" << ArgId << "\">" << ArgId << "</td>";
  }
  os << "</tr><tr><td cellspan=\"" << CI->getNumArgOperands() << "\">call: " << CIName << "</td></tr></table>>];\n";
  */
  os << CallId << " [label=\"call: " << CIName << "\""
    << ", style=\"filled\" , fillcolor=white, " << std::string("xlabel=<<font color=\"#3870c8\">") << std::to_string(AllCIs.size()-1) << std::string("</font>>")  << ", shape=box];\n";
  
  int TreeId = 0;
  for (Tree &T : Trees) {

    //Nodes
    for (Node *N : T.Nodes) {
      if (N->isFunction()) continue;
      NodeId[N] = Id;

      bool Internalizable = N->getNumMatches()>0;
      bool ReusedInput = false;

      std::string Colour = ((Internalizable) ? "\"#8ae18a\"" : "\"#ff6671\"");

      std::string CostLabel = "";
      if (Internalizable) {
        CostLabel = std::string("xlabel=<<font color=\"#3870c8\">") + std::to_string(N->getNumMatches()) + std::string("</font>>");
      } else {

        if (CrossEdges.find(N->getValue())!=CrossEdges.end()) {
          for (auto &CE : CrossEdges[N->getValue()]) {
            if (CE.getNode()==N) {
              ReusedInput = true;
              Colour = "\"#ede61c\"";
              break;
            }
          }
        }

      }

      os << Id << " [label=\"" << N->getString() << "\" " << CostLabel
        << ", style=\"filled\" , fillcolor=" << Colour
        << ", shape=" << ((Internalizable || ReusedInput) ? "box" : "oval") << "];\n";

      Id++;
    }

    os << CallId << " -> " << NodeId[T.Root] << "\n";// "[taillabel=<<font color=\"black\"> "<< TreeId <<" </font>>]\n";

    //Edges
    for (Node *N : T.Nodes) {
      int ChildId = 0;
      for (Node *Child : N->getChildren()) {
        if (Child->isFunction()) continue;
        //std::string EdgeLabel = "";
        //if (N->isCallInst()) EdgeLabel = std::string(" [taillabel=<<font color=\"black\"> ") + std::to_string(ChildId) + std::string(" </font>>]");
        os << NodeId[N] << " -> " << NodeId[Child] << "\n"; //EdgeLabel << "\n";
        ChildId++;
      }

      if (CrossEdges.find(N->getValue())!=CrossEdges.end()) {
        for (auto &CE : CrossEdges[N->getValue()]) {
          if (CE.getNode()==N) {
            std::string CostLabel = std::string("label=<<font color=\"#3870c8\">") + std::to_string(CE.getNumMatches()) + std::string("</font>>");
            os << NodeId[CE.getSourceNode()] << " -> " <<  NodeId[N] << " [style=dashed color=\"#8ae18a\" " << CostLabel << "]\n";
            break;
          }
        }
      }

      /*
      bool Internalizable = N->getNumMatches()>0;
      if (!Internalizable && ReusedInputId[N->getValue()]!=NodeId[N]) {
        os << ReusedInputId[N->getValue()] << " -> " << NodeId[N] << " [style=dashed]\n";
      }*/
    }

    TreeId++;
  }
  
  os << "}\n";

}

bool FunctionCloning::runOnModule(Module &M) {
  std::list<Function *> Fns;
  for (Function &F : M) {
    if (F.isDeclaration() || F.isVarArg()) continue;
    Fns.push_back(&F);
  }
  while (!Fns.empty()) {
    Function *F = Fns.front();
    Fns.pop_front();
    std::list<CallInst*> Calls;
    bool SkipFn = false;
    for (User *U : F->users()) {
      CallInst *CI = dyn_cast<CallInst>(U);
      if (CI && CI->getCalledFunction()==F && !F->isVarArg()) {
        Calls.push_back(CI);
      } else {
        SkipFn = true;
        break;
      }
    }
    errs() << "*** Function: " << demangle(F->getName().data()) << "\n";
    if (SkipFn) {
      errs() << "Skipping\n";
      continue;
    }
    while (!Calls.empty()) {
      std::vector<CallInst*> CallsVec;
      for (CallInst *CI : Calls) CallsVec.push_back(CI);

      CallInst *CI = Calls.front();

      Calls.pop_front();

      errs() << "Call: ";
      CI->dump();
      CallMatching CM(CI,CallsVec,EnableConstantsOnly);
      CM.dump();
      errs() << "\n";
      //errs() << "NumMatches: " << CM.NumMatches << "\n";
      int NumMatches = 0;
      int NumMismatches = 0;
      int NumReuse = 0;
      for (CallMatching::Node *N : CM.AllNodes) {
        errs() << "Node: " << N->getString() << " : ";
        if (N->getNodeType()==CallMatching::NodeType::Matching) {
          NumMatches++;
          errs() << " match\n";
        } else if (N->getNodeType()==CallMatching::NodeType::Mismatching) {
          NumMismatches++;
          errs() << " mismatch\n";
        } else if (N->getNodeType()==CallMatching::NodeType::Reuse) {
          NumReuse++;
          errs() << " reuse\n";
        }
      }
      errs() << "NumMatches: " << NumMatches << "\n";
      errs() << "NumMismatches: " << NumMismatches << "\n";
      errs() << "NumReuse: " << NumReuse << "\n";
      errs() << "HasMultiNode: " << CM.HasMultiNode << "\n";
      errs() << "Conditions:\n";
      errs() << "NumMatches: " << NumMatches << " >= " << MinMatches << "? " << (NumMatches >= MinMatches) << "\n";
      errs() << "TreeSize: " << (NumMatches + NumReuse) << " >= " << MinTreeSize << "? " << (NumMatches + NumReuse >= MinTreeSize) << "\n";
      errs() << "NumMismatches: " << ((int)NumMismatches - ((int)CI->getCalledFunction()->arg_size())) << " <= " << MaxMismatches << "? " << (((int)NumMismatches - ((int)CI->getCalledFunction()->arg_size())) <= (int)MaxMismatches) << "\n";
      errs() << "Callsites: " << CM.getNumCalls() << " >= " << MinCallsites << "? " << (CM.getNumCalls() >= MinCallsites) << "\n";
      //if (CM.Cost>0 || (EnableConstantsOnly && CM.NumMatches>0)) {
      //if ( ((!EnableConstantsOnly) && (NumMatches + NumReuse > 1)) || (EnableConstantsOnly && CM.NumMatches>0)) {
      //if ( NumMatches + NumReuse > 1 || NumMismatches<CI->getCalledFunction()->arg_size()) {
      if (
      NumMatches + NumReuse >= MinTreeSize &&
      NumMatches >= MinMatches &&
      ((int)NumMismatches - ((int)CI->getCalledFunction()->arg_size())) <= (int)MaxMismatches &&
      Calls.size() >= MinCallsites) {
        errs() << "Cost: " << CM.Cost << "\n";
        if (EnableFEDotFiles) CM.writeDotFile();
        errs() << "Number of uses: " << F->getNumUses() << "\n";
        errs() << "Number of calls: " << Calls.size() << "\n";
        if (F->getNumUses()!=CM.getWidth()) {
          errs() << "Invalid width: " << CM.getWidth() << "; expected: " << F->getNumUses() << "\n";
        } else if (CM.validate()) {
          errs() << "generating new function\n";
          Function *NewF = CM.generateExpandedFunction(M,Fns,Calls);
          //TODO: remove Calls instructions that have been deleted
        }
      }
    }
    errs() << "\n";
    errs() << "\n";
  }

  return false;
}

void FunctionCloning::getAnalysisUsage(AnalysisUsage &AU) const {}

char FunctionCloning::ID = 0;
INITIALIZE_PASS(FunctionCloning, "func-cloning", "Function Cloning", false, false)


