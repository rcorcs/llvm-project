
#include "llvm/Transforms/IPO/FunctionCloning.h"


#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"


#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Transforms/Utils/Cloning.h"

#define DEBUG_TYPE "STATS_FUNCTION_CLONING"


#include <memory>
#include <string>
#include <cxxabi.h>

using namespace llvm;


static std::string demangle(const char* name) {
        int status = -1; 

        std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
        return (status == 0) ? res.get() : std::string(name);
}


static bool match(Value *V1, Value *V2) {
  //if (V1==V2) return true;
  if (V1->getType()!=V2->getType()) return false;
  if (isa<Instruction>(V1) && isa<Instruction>(V2)) {
    Instruction *I1 = dyn_cast<Instruction>(V1);
    Instruction *I2 = dyn_cast<Instruction>(V2);

    //default check
    return (I1->getOpcode()==I2->getOpcode() && I1->getNumOperands()==I2->getNumOperands());
  }
  return (isa<Constant>(V1) && !isa<Function>(V1) && V1==V2);
  //return false;
}

class CallMatching {
private:

  class Node {
  private:
    Value *V;
    std::vector< std::pair<Value*, CallInst*> > MatchingValues;
    std::vector<Node *> Children;
    Node *Parent;

  public:
    Node(Value *V, Node *Parent=nullptr) : V(V), Parent(Parent) {}

    void addMatch(Value *V, CallInst *CI) {
      MatchingValues.push_back( std::pair<Value*, CallInst*>(V,CI) );
    }

    size_t getNumMatches() {
      return MatchingValues.size();
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
      } else if (isa<Constant>(V)) V->dump();
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

  void destroyNodesRec( Node *N );
  unsigned growTreeNode( Node *N , Tree &T);
  void buildTrees();

public:
  unsigned Cost;

  CallMatching(CallInst *CI, std::vector<CallInst *> &AllCIs) : CI(CI), AllCIs(AllCIs) { buildTrees(); }
  ~CallMatching();
  

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
};

CallMatching::~CallMatching() {
  for (Tree &T : Trees) destroyNodesRec(T.Root);
  delete Root;
}

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

void CallMatching::buildTrees() {
  Cost = 0;
  Root = new Node(CI);
  AllNodes.insert(Root);
  for (CallInst *OtherCI : AllCIs) {
    if (OtherCI==CI) continue;
    Root->addMatch(OtherCI, OtherCI);
  }

  for (unsigned i = 0; i<CI->getNumArgOperands(); i++) {
    Value *V = CI->getArgOperand(i);

    Node *N = new Node(V,Root);
    AllNodes.insert(N);
    Root->pushChild(N);
    Trees.push_back(Tree(N));

    if (isInternalizable(V)) {
      for (CallInst *OtherCI : AllCIs) {
        if (OtherCI==CI) continue;
        Value *OtherV = OtherCI->getArgOperand(i);
        if (match(V, OtherV)) N->addMatch(OtherV, OtherCI);
      }
      Cost += growTreeNode(N, Trees[Trees.size()-1]);
    }
  }

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
        CrossEdges[N->getValue()].push_back( CE );
        Cost += CE.getNumMatches();
      }
    }
  }

}

unsigned CallMatching::growTreeNode( Node *N , Tree &T) {
  unsigned Cost = N->getNumMatches();
  Value *V = N->getValue();
  if (N->getNumMatches()==0) return Cost;
  if (Instruction *I = dyn_cast<Instruction>(V)) {
    bool GrowOpcode = true;
    switch(I->getOpcode()) {
    case Instruction::PHI:
    case Instruction::Load:
    case Instruction::Invoke:
      GrowOpcode = false;
      break;
    }
    if (!GrowOpcode) return Cost;
 
    for (unsigned i = 0; i<I->getNumOperands(); i++) {
      Value *ChildV = I->getOperand(i);
      Node *ChildN = new Node(ChildV, N);
      AllNodes.insert(ChildN);
      T.addNode(ChildN);
      if (isInternalizable(ChildV)) {
        for (auto &Pair : N->getMatchingValues()) {
          CallInst *OtherCI = Pair.second;
          Value *OtherV = dyn_cast<Instruction>(Pair.first)->getOperand(i);
          if (match(ChildV, OtherV)) ChildN->addMatch(OtherV, OtherCI);
        }
        Cost += growTreeNode(ChildN, T);
      }
      N->pushChild(ChildN);
    }
  }
  return Cost;
}

void CallMatching::destroyNodesRec( Node *N ) {
  AllNodes.erase(N);
  for (Node *Child : N->getChildren()) {
    destroyNodesRec(Child);
  }
  delete N;
}

void CallMatching::writeDotFile() {
  std::string PrefixName = std::to_string(Cost) + std::string(".") + demangle(CI->getParent()->getParent()->getName().data());

  std::string CIName = "";
  if (CI->getCalledFunction()->hasName())
    CIName = demangle(CI->getCalledFunction()->getName().data());

  PrefixName += std::string(".") + CIName + std::string(".") + std::string(CI->getName());

  std::string FileName = PrefixName + std::string(".dot");

  std::error_code ec;
  raw_fd_ostream os (FileName, ec, sys::fs::F_Text);

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
  for (Function &F : M) {
    if (F.isDeclaration() ||  F.isVarArg()) continue;
    std::vector<CallInst*> Calls;
    for (User *U : F.users()) {
      if (isa<CallInst>(U)) {
        Calls.push_back(dyn_cast<CallInst>(U));
      }
    }
    errs() << "*** Function: " << demangle(F.getName().data()) << "\n";
    for (CallInst *CI : Calls) {
      errs() << "Call: ";
      CI->dump();
      CallMatching CM(CI,Calls);
      CM.dump();
      errs() << "\n";
      if (CM.Cost>0) {
        errs() << "Cost: " << CM.Cost << "\n";
        CM.writeDotFile();
      }
    }
    errs() << "\n";
    errs() << "\n";
  }

  return false;
}


/*
static bool AllUsesIn(Value *V, Value *TargetUse) {
    for (User *U : V->users()) {
       if ( U!=TargetUse ) return false;
    }
    return true;
}

class FunctionSpecialization {
public:
  Function *F;
  std::set<CallInst *> Calls;
  bool UnusedReturnValue;
  std::map<unsigned, Constant *> ConstantParams;
 
  FunctionSpecialization(Function *F, std::set<CallInst *> Calls) : F(F), Calls(Calls) {
    UnusedReturnValue = false;
  }

  void setUnusedReturnValue(bool UnusedReturnValue) {
    this->UnusedReturnValue = UnusedReturnValue;
  }

  bool hasUnusedReturnValue() {
    return UnusedReturnValue;
  }

  void setConstantParameter(unsigned ParamId, Constant *ParamVal) {
    ConstantParams[ParamId] = ParamVal;
  }

  Constant *getConstantParameter(unsigned ParamId) {
    return ConstantParams[ParamId];
  }
};

bool FunctionCloning::runOnModule(Module &M) {
  std::map<Function *, unsigned> countCalls;
  std::map<Function *, unsigned> countUnusedReturns;
  std::map<Function *, std::map<unsigned, std::map<unsigned, unsigned> > > countOpcodeArgs;
  std::map<Function *, std::map<unsigned, std::map<Constant *, unsigned> > > countConstantArgs;
  std::map<Function *, std::map<unsigned, std::map<Function *, unsigned> > > countCallAsArgs;

  std::vector<Function *> WorkList;

  for(auto &F : M){
    if (F.isDeclaration()) continue;
    WorkList.push_back(&F);
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (I.getOpcode()==Instruction::Call) {
          CallInst *CI = dyn_cast<CallInst>(&I);
          if (CI==nullptr || CI->getCalledFunction()==nullptr) continue;
          if (CI->getCalledFunction()->isDeclaration()) continue;
          if (CI->getCalledFunction()->isVarArg()) continue;

          Function *CalledF = CI->getCalledFunction();

          countCalls[CalledF]++;
          
          if (CalledF->getReturnType()!=nullptr && !CalledF->getReturnType()->isVoidTy()) {
            if (I.getNumUses()==0) {
              countUnusedReturns[CalledF]++;
            }
          }

          for (unsigned i = 0; i<CI->getNumArgOperands(); i++) {
             Value *Arg = CI->getArgOperand(i);
             if (Arg==nullptr) continue;
             if (Constant *ConstArg = dyn_cast<Constant>(Arg)) {
                countConstantArgs[CalledF][i][ConstArg]++;
             }

             if (Instruction *IArg = dyn_cast<Instruction>(Arg)) {
                countOpcodeArgs[CalledF][i][IArg->getOpcode()]++;
             }

             //if(Arg->getNumUses()!=1) continue;
             if (CallInst *CIArg = dyn_cast<CallInst>(Arg)) {
                if (CIArg->getCalledFunction()==nullptr) continue;
                if (CIArg->getCalledFunction()->isDeclaration()) continue;
                if (CIArg->getCalledFunction()->isVarArg()) continue;
                if ( !AllUsesIn(CIArg,CI) ) continue;
                if (CIArg->getNumUses()!=1) continue;

                countCallAsArgs[CalledF][i][CIArg->getCalledFunction()]++;
             }
          }
        }
      }
    }
  }


  for(auto &kv : countOpcodeArgs){
      errs() << "\tCalls to function: " << demangle(kv.first->getName().data()) << "\n";
      for(auto &ArgsCount : kv.second){
         bool firstEntry = true;
         for(auto &OpcodeCount : ArgsCount.second){
            float ratio = ((float)OpcodeCount.second)/((float)countCalls[kv.first]);
               if(firstEntry){
                  errs() << "[count-opcode-args]\t" << demangle(kv.first->getName().data()) << ":" << ArgsCount.first << ": ";
                  firstEntry = false;
               }
               errs() << Instruction::getOpcodeName(OpcodeCount.first);
               errs() << " [" << OpcodeCount.second << " (" << (int)(100*ratio) << ")]; ";
         }
         errs() << "\n";
      }
   }

  std::set<Function *> MaybeDeadFunction;

  for (auto *F : WorkList) {
    if (F->isDeclaration() || F->isVarArg()) continue;

    unsigned TotalNumCalls = countCalls[F];
    if (TotalNumCalls==0) continue;

    LLVMContext &Context = F->getContext();

    bool RemoveReturnValue = (countUnusedReturns[F]==TotalNumCalls);
    bool RemoveRedundantParams = false;

    FunctionType *FTy = F->getFunctionType();
    SmallVector<Constant *, 8> ConstParams(FTy->getNumParams());
    SmallVector<Function *, 8> FCallParams(FTy->getNumParams());

    unsigned NumConstParams = 0;
    unsigned NumFCallParams = 0;


    for (unsigned i = 0; i<FTy->getNumParams(); i++) {
      ConstParams[i] = nullptr;
      FCallParams[i] = nullptr;
      for (auto &Pair : countConstantArgs[F][i]) {
        if (Pair.second==TotalNumCalls) {
          ConstParams[i] = Pair.first;
          NumConstParams++;
        }
      }
      for (auto &Pair : countCallAsArgs[F][i]) {
        if (Pair.second==TotalNumCalls) {
          FCallParams[i] = Pair.first;
          NumFCallParams++;
        }
      }
    }

    std::set<CallInst *> Calls;

    for (auto *U : F->users()) {
      if (CallInst *CI = dyn_cast<CallInst>(U)) {
        if (CI->getCalledFunction()==F)
          Calls.insert(CI);
      }
    }

    std::map<unsigned,unsigned> IdenticalParams;
    for (CallInst *CI1 : Calls) {
      //CI1->dump();
      for (unsigned i = 0; i<CI1->getNumArgOperands(); i++) {
        if (ConstParams[i]) continue;
        for (unsigned j = 0; j<i; j++) {
          if (i==j) continue;
          if (ConstParams[j]) continue;
          if (CI1->getArgOperand(i)!=CI1->getArgOperand(j)) continue;
          bool MatchAll = true;
          for (CallInst *CI2 : Calls) {
            if (CI1!=CI2) {
              if (CI2->getArgOperand(i)!=CI2->getArgOperand(j)) {
                MatchAll = false;
                break;
              }
            }
          }
          if (MatchAll) {
            IdenticalParams[i] = j;
            RemoveRedundantParams = true;
            break;
          }
        }
      }
    }

    //TODO: ignore identical parameters...
    std::vector<Type *> Params;
    std::map<unsigned, unsigned> ParamMap;
    for (unsigned i = 0; i<FTy->getNumParams(); i++) {
      if (ConstParams[i]==nullptr && FCallParams[i]==nullptr && IdenticalParams.find(i)==IdenticalParams.end()) {
        ParamMap[i] = Params.size();
        Params.push_back( FTy->getParamType(i) );
      }
    }

    if (!RemoveReturnValue && !RemoveRedundantParams && NumConstParams==0 && NumFCallParams==0) continue;

    errs() << "Cloning: " << F->getName() << "\n";
    std::map<unsigned, unsigned> FusionMap;
    for (unsigned i = 0; i<FTy->getNumParams(); i++) {
      if (FCallParams[i]) {
        FusionMap[i] = Params.size();
        Function *ParamFunc = FCallParams[i];
        for (unsigned j = 0; j<ParamFunc->getFunctionType()->getNumParams(); j++) {
          Params.push_back( ParamFunc->getFunctionType()->getParamType(j) );
        }
      }
    }

    Type *RetTy = Type::getVoidTy(Context);
    if (!RemoveReturnValue) {
      RetTy = FTy->getReturnType();
    }
    
    FunctionType *NewFTy = FunctionType::get(RetTy, ArrayRef<Type *>(Params), false);

    std::string Name = std::string("cloned.") + std::string(F->getName().str());

    Function *ClonedF = Function::Create(NewFTy, GlobalValue::LinkageTypes::InternalLinkage,
                                         Twine(Name), M);

    //ClonedF->setAttributes(F->getAttributes());
    ClonedF->setAlignment(F->getAlignment());
    ClonedF->setCallingConv(F->getCallingConv());
    ClonedF->setDSOLocal(F->isDSOLocal());
    ClonedF->setUnnamedAddr(F->getUnnamedAddr());
    ClonedF->setVisibility(F->getVisibility());
    if (F->hasPersonalityFn()) {
      ClonedF->setPersonalityFn(F->getPersonalityFn());
    }
    if (F->hasComdat()) {
      ClonedF->setComdat(F->getComdat());
    }
    if (F->hasSection()) {
      ClonedF->setSection(F->getSection());
    }
    
    std::vector<Value *> ParamList;
    for (auto PIt = ClonedF->arg_begin(), E = ClonedF->arg_end(); PIt != E; PIt++) {
      ParamList.push_back(&*PIt);
    }

    BasicBlock *BB = BasicBlock::Create(Context, "", ClonedF);

    IRBuilder<> Builder(BB);

    std::vector<CallInst *> InlineWorkList;


    std::map<unsigned,Value *> FusionValMap;
    for (unsigned i = 0; i<FTy->getNumParams(); i++) {
      if (IdenticalParams.find(i)!=IdenticalParams.end()) continue; //do not inline *exactly* the same function call twice
      if (FCallParams[i]) {
        Function *ParamFunc = FCallParams[i];
        MaybeDeadFunction.insert(ParamFunc);
        std::vector<Value *> Args;
        unsigned offset = FusionMap[i];
        for (unsigned j = 0; j<ParamFunc->getFunctionType()->getNumParams(); j++) {
          Args.push_back( ParamList[offset + j] );
        }
        CallInst *ParamCI = Builder.CreateCall(ParamFunc,ArrayRef<Value *>(Args));
        FusionValMap[i] = ParamCI;
        InlineWorkList.push_back(ParamCI);
      }
    }

    std::vector<Value *> Args;
    for (unsigned i = 0; i<FTy->getNumParams(); i++) {
      Value *ConstArg = ConstParams[i];
      Value *FCallArg = FCallParams[i];
      Value *Arg = nullptr;
      if (ConstArg) Arg = ConstArg;
      else if (IdenticalParams.find(i)!=IdenticalParams.end()) Arg = Args[IdenticalParams[i]];
      else if (FCallArg) Arg = FusionValMap[i];
      else Arg = ParamList[ParamMap[i]];
      Args.push_back(Arg);
    }

    CallInst *CI = Builder.CreateCall(F,ArrayRef<Value *>(Args));
    if (RetTy->isVoidTy()) {
      Builder.CreateRetVoid();
    } else {
      Builder.CreateRet(CI);
    }
    InlineWorkList.push_back(CI);

    ClonedF->dump();
    //verifyFunction(*ClonedF, &errs());

    for (CallInst *CI : InlineWorkList) {
      InlineFunctionInfo IFI;
      InlineFunction(CI, IFI);
    }

    //ClonedF->dump();
    verifyFunction(*ClonedF, &errs());

    for (CallInst *CI : Calls) {
        std::vector<Value *> Args;
        std::vector<CallInst *> FCallArgs;
        for (unsigned i = 0; i<FTy->getNumParams(); i++) {
          if (IdenticalParams.find(i)==IdenticalParams.end()) {
            if (FCallParams[i]!=nullptr) {
              CallInst *ParamCI = dyn_cast<CallInst>(CI->getArgOperand(i));
              FCallArgs.push_back(ParamCI);
            } else if (ConstParams[i]==nullptr) {
              Args.push_back( CI->getArgOperand(i) );
            }
          }
        }
        
        for (CallInst *ParamCI : FCallArgs) {
          for (unsigned i = 0; i<ParamCI->getNumArgOperands(); i++) {
            Args.push_back( ParamCI->getArgOperand(i) );
          }
        }

        errs() << "Args & Params: " << Args.size() << " & " << NewFTy->getNumParams() << "\n";

        IRBuilder<> Builder(CI);
        auto *NewCI = Builder.CreateCall( ClonedF, ArrayRef<Value *>(Args) );
        if (!RetTy->isVoidTy()) CI->replaceAllUsesWith(NewCI);
        if (CI->getNumUses()==0) CI->eraseFromParent();
        else {
            errs() << "ERROR: Call should have no other use!\n";
            CI->dump();
            errs() << "Users:\n";
            for (auto *U: CI->users()) U->dump();
        }
        for (CallInst *ParamCI : FCallArgs) {
          if (ParamCI->getNumUses()==0) ParamCI->eraseFromParent();
          else {
            errs() << "ERROR: Param call should have no other use!\n";
            ParamCI->dump();
            errs() << "Users:\n";
            for (auto *U: ParamCI->users()) U->dump();
          }
        }
    }

    countCallAsArgs.erase(F);
    for (auto &Pair1 : countCallAsArgs) {
      for (auto &Pair2 : Pair1.second) {
        if (Pair2.second.find(F)!=Pair2.second.end()) {
          Pair2.second[ClonedF] = Pair2.second[F];
          Pair2.second[F] = 0;
          Pair2.second.erase(F);
        }
      }
    }
    MaybeDeadFunction.insert(F);
  }

  //TODO; keep track of the ParamCI functions and delete the ones that are not in use anymore
  for (Function *F: MaybeDeadFunction) {
    if (F->getNumUses()==0) F->eraseFromParent();
  }

  return false;
}
*/

void FunctionCloning::getAnalysisUsage(AnalysisUsage &AU) const {}

char FunctionCloning::ID = 0;
INITIALIZE_PASS(FunctionCloning, "func-cloning", "Function Cloning", false, false)


