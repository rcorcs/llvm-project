//===- FunctionMerging.h - A function merging pass ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the general function merging optimization.
//
// It identifies similarities between functions, and If profitable, merges them
// into a single function, replacing the original ones. Functions do not need
// to be identical to be merged. In fact, there is very little restriction to
// merge two function, however, the produced merged function can be larger than
// the two original functions together. For that reason, it uses the
// TargetTransformInfo analysis to estimate the code-size costs of instructions
// in order to estimate the profitability of merging two functions.
//
// This function merging transformation has three major parts:
// 1. The input functions are linearized, representing their CFGs as sequences
//    of labels and instructions.
// 2. We apply a sequence alignment algorithm, namely, the Needleman-Wunsch
//    algorithm, to identify similar code between the two linearized functions.
// 3. We use the aligned sequences to perform code generate, producing the new
//    merged function, using an extra parameter to represent the function
//    identifier.
//
// This pass integrates the function merging transformation with an exploration
// framework. For every function, the other functions are ranked based their
// degree of similarity, which is computed from the functions' fingerprints.
// Only the top candidates are analyzed in a greedy manner and if one of them
// produces a profitable result, the merged function is taken.
//
//===----------------------------------------------------------------------===//
//
// This optimization was proposed in
//
// Function Merging by Sequence Alignment: An Interprocedural Code-Size
// Optimization
// Rodrigo C. O. Rocha, Pavlos Petoumenos, Zheng Wang, Murray Cole, Hugh Leather
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_IPO_FUNCTIONMERGING_H
#define LLVM_TRANSFORMS_IPO_FUNCTIONMERGING_H

//#include "llvm/ADT/KeyValueCache.h"

#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringSet.h"

#include "llvm/Transforms/Utils/Cloning.h"

#include "llvm/ADT/SequenceAlignment.h"

#include "llvm/Transforms/IPO/SearchStrategy.h"
#include "llvm/Transforms/IPO/tsl/robin_map.h"
#include "llvm/Support/CommandLine.h"

#include <map>
#include <set>
#include <vector>

extern llvm::cl::opt<bool> EnableHyFMNW;
extern llvm::cl::opt<bool> EnableHyFMPA;
extern llvm::cl::opt<bool> HyFMProfitability;

namespace llvm {

/// A set of parameters used to control the transforms by MergeFunctions.
struct FunctionMergingOptions {
  bool MaximizeParamScore;
  bool IdenticalTypesOnly;
  bool EnableUnifiedReturnType;

  FunctionMergingOptions(bool MaximizeParamScore = true,
                         bool IdenticalTypesOnly = true,
                         bool EnableUnifiedReturnType = true)
      : MaximizeParamScore(MaximizeParamScore),
        IdenticalTypesOnly(IdenticalTypesOnly),
        EnableUnifiedReturnType(EnableUnifiedReturnType) {}

  FunctionMergingOptions &maximizeParameterScore(bool MPS) {
    MaximizeParamScore = MPS;
    return *this;
  }

  FunctionMergingOptions &matchOnlyIdenticalTypes(bool IT) {
    IdenticalTypesOnly = IT;
    return *this;
  }

  FunctionMergingOptions &enableUnifiedReturnTypes(bool URT) {
    EnableUnifiedReturnType = URT;
    return *this;
  }
};

class FunctionMergeResult {
private:
  Function *F1;
  Function *F2;
  Function *MergedFunction;
  bool HasIdArg;
  bool NeedUnifiedReturn;
  std::map<unsigned, unsigned> ParamMap1;
  std::map<unsigned, unsigned> ParamMap2;

  FunctionMergeResult()
      : F1(nullptr), F2(nullptr), MergedFunction(nullptr), HasIdArg(false),
        NeedUnifiedReturn(false) {}

public:
  FunctionMergeResult(Function *F1, Function *F2, Function *MergedFunction,
                      bool NeedUnifiedReturn = false)
      : F1(F1), F2(F2), MergedFunction(MergedFunction), HasIdArg(true),
        NeedUnifiedReturn(NeedUnifiedReturn) {}

  std::pair<Function *, Function *> getFunctions() {
    return std::pair<Function *, Function *>(F1, F2);
  }

  std::map<unsigned, unsigned> &getArgumentMapping(Function *F) {
    return (F1 == F) ? ParamMap1 : ParamMap2;
  }

  Value *getFunctionIdValue(Function *F) {
    if (F == F1)
      return ConstantInt::getTrue(IntegerType::get(F1->getContext(), 1));
    else if (F == F2)
      return ConstantInt::getFalse(IntegerType::get(F2->getContext(), 1));
    else
      return nullptr;
  }

  void setFunctionIdArgument(bool HasFuncIdArg) { HasIdArg = HasFuncIdArg; }

  bool hasFunctionIdArgument() { return HasIdArg; }

  void setUnifiedReturn(bool NeedUnifiedReturn) {
    this->NeedUnifiedReturn = NeedUnifiedReturn;
  }

  bool needUnifiedReturn() { return NeedUnifiedReturn; }

  // returns whether or not the merge operation was successful
  operator bool() const { return (MergedFunction != nullptr); }

  void setArgumentMapping(Function *F, std::map<unsigned, unsigned> &ParamMap) {
    if (F == F1)
      ParamMap1 = ParamMap;
    else if (F == F2)
      ParamMap2 = ParamMap;
  }

  void addArgumentMapping(Function *F, unsigned SrcArg, unsigned DstArg) {
    if (F == F1)
      ParamMap1[SrcArg] = DstArg;
    else if (F == F2)
      ParamMap2[SrcArg] = DstArg;
  }

  Function *getMergedFunction() { return MergedFunction; }

  //  static const FunctionMergeResult Error;
};

struct AlignmentStats {
  int Insts{0};
  int Matches{0};
  int CoreMatches{0};
  bool isProfitable() const {return (Matches == Insts) || (CoreMatches > 0);};
};

class FunctionMerger {
private:
  Module *M;

  // ProfileSummaryInfo *PSI;
  function_ref<BlockFrequencyInfo *(Function &)> LookupBFI;

  Type *IntPtrTy;

  const DataLayout *DL;
  LLVMContext *ContextPtr;

  // cache of linear functions
  // KeyValueCache<Function *, SmallVector<Value *, 8>> LFCache;

  // statistics for analyzing this optimization for future improvements
  // unsigned LastMaxParamScore = 0;
  // unsigned TotalParamScore = 0;
  // int CountOpReorder = 0;
  // int CountBinOps = 0;

  enum LinearizationKind { LK_Random, LK_Canonical };

  void orderBlocks(Function &F, SmallVectorImpl<BasicBlock *> &FVec,
                 LinearizationKind LK = LinearizationKind::LK_Canonical);

  static bool matchInstructions(Instruction *I1, Instruction *I2,
                                const FunctionMergingOptions &Options = {});
  static bool matchWholeBlocks(Value *V1, Value *V2);

  void replaceByCall(Function *F, FunctionMergeResult &MergedFunc,
                     const FunctionMergingOptions &Options = {});
  bool replaceCallsWith(Function *F, FunctionMergeResult &MergedFunc,
                        const FunctionMergingOptions &Options = {});

  void updateCallGraph(Function *F, FunctionMergeResult &MFR,
                       StringSet<> &AlwaysPreserved,
                       const FunctionMergingOptions &Options);

public:
  FunctionMerger(Module *M) : M(M), IntPtrTy(nullptr) {
    //, ProfileSummaryInfo *PSI=nullptr, function_ref<BlockFrequencyInfo
    //*(Function &)> LookupBFI=nullptr) : M(M), PSI(PSI), LookupBFI(LookupBFI),
    // IntPtrTy(nullptr) {
    if (M) {
      DL = &M->getDataLayout();
      ContextPtr = &M->getContext();
      IntPtrTy = DL->getIntPtrType(*ContextPtr);
    }
  }

  bool validMergeTypes(Function *F1, Function *F2,
                       const FunctionMergingOptions &Options = {});

  static bool areTypesEquivalent(Type *Ty1, Type *Ty2, const DataLayout *DL,
                                 const FunctionMergingOptions &Options = {});

  static bool isSAProfitable(AlignedSequence<Value *> &AlignedBlocks);
  static bool isPAProfitable(BasicBlock *BB1, BasicBlock *BB2);

  static void extendAlignedSeq(AlignedSequence<Value *> &AlignedSeq, AlignedSequence<Value *> &AlignedSubSeq, AlignmentStats &stats);
  static void extendAlignedSeq(AlignedSequence<Value *> &AlignedSeq, BasicBlock *BB1, BasicBlock *BB2, AlignmentStats &stats);

  static bool match(Value *V1, Value *V2);

  template<typename RegionT>
  static AlignedSequence<Value *> alignBlocks(RegionT &F1, RegionT &F2, AlignmentStats &TotalAlignmentStats, TargetTransformInfo *TTI=nullptr, const FunctionMergingOptions &Options = {});

  void updateCallGraph(FunctionMergeResult &Result,
                       StringSet<> &AlwaysPreserved,
                       const FunctionMergingOptions &Options = {});

  FunctionMergeResult merge(Function *F1, Function *F2, std::string Name = "",
                            const FunctionMergingOptions &Options = {});

  class CodeGenerator {
  private:
    LLVMContext *ContextPtr;
    Type *IntPtrTy;

    Value *IsFunc1;

    // BlockListType &Blocks1;
    // BlockListType &Blocks2;
    std::vector<BasicBlock *> Blocks1;
    std::vector<BasicBlock *> Blocks2;

    BasicBlock *EntryBB1;
    BasicBlock *EntryBB2;
    BasicBlock *PreBB;

    std::map<std::pair<BasicBlock *, BasicBlock*>, std::pair<BasicBlock *, BasicBlock*> > EdgeMap;

    Type *RetType1;
    Type *RetType2;
    Type *ReturnType;

    bool RequiresUnifiedReturn;

    Function *MergedFunc;

    SmallPtrSet<BasicBlock *, 8> CreatedBBs;
    SmallPtrSet<Instruction *, 8> CreatedInsts;

  protected:
    void removeRedundantInstructions(std::vector<Instruction *> &WorkInst,
                                     DominatorTree &DT);

  public:
    template <typename BlockListType>
    CodeGenerator(BlockListType &Blocks1, BlockListType &Blocks2) {
      for (BasicBlock &BB : Blocks1)
        this->Blocks1.push_back(&BB);
      for (BasicBlock &BB : Blocks2)
        this->Blocks2.push_back(&BB);
    }

    virtual ~CodeGenerator() {}

    CodeGenerator &setContext(LLVMContext *ContextPtr) {
      this->ContextPtr = ContextPtr;
      return *this;
    }

    void addEdgeMap(BasicBlock *BB1, BasicBlock *BB2, BasicBlock *NewBB1, BasicBlock *NewBB2) {
      EdgeMap[std::pair<BasicBlock *, BasicBlock*>(BB1,BB2)] = std::pair<BasicBlock *, BasicBlock*>(NewBB1,NewBB2);
    }

    std::pair<BasicBlock *, BasicBlock *> getNewEdge(BasicBlock *BB1, BasicBlock *BB2) {
      return EdgeMap[std::pair<BasicBlock *, BasicBlock*>(BB1,BB2)];
    }

    CodeGenerator &setIntPtrType(Type *IntPtrTy) {
      this->IntPtrTy = IntPtrTy;
      return *this;
    }

    CodeGenerator &setFunctionIdentifier(Value *IsFunc1) {
      this->IsFunc1 = IsFunc1;
      return *this;
    }

    CodeGenerator &setEntryPoints(BasicBlock *EntryBB1, BasicBlock *EntryBB2) {
      this->EntryBB1 = EntryBB1;
      this->EntryBB2 = EntryBB2;
      return *this;
    }

    CodeGenerator &setReturnTypes(Type *RetType1, Type *RetType2) {
      this->RetType1 = RetType1;
      this->RetType2 = RetType2;
      return *this;
    }

    CodeGenerator &setMergedEntryPoint(BasicBlock *PreBB) {
      this->PreBB = PreBB;
      return *this;
    }

    CodeGenerator &setMergedReturnType(Type *ReturnType,
                                       bool RequiresUnifiedReturn = false) {
      this->ReturnType = ReturnType;
      this->RequiresUnifiedReturn = RequiresUnifiedReturn;
      return *this;
    }

    CodeGenerator &setMergedFunction(Function *MergedFunc) {
      this->MergedFunc = MergedFunc;
      return *this;
    }

    Function *getMergedFunction() { return MergedFunc; }
    Type *getMergedReturnType() { return ReturnType; }
    bool getRequiresUnifiedReturn() { return RequiresUnifiedReturn; }

    Value *getFunctionIdentifier() { return IsFunc1; }

    LLVMContext &getContext() { return *ContextPtr; }

    std::vector<BasicBlock *> &getBlocks1() { return Blocks1; }
    std::vector<BasicBlock *> &getBlocks2() { return Blocks2; }

    BasicBlock *getEntryBlock1() { return EntryBB1; }
    BasicBlock *getEntryBlock2() { return EntryBB2; }
    BasicBlock *getPreBlock() { return PreBB; }

    Type *getReturnType1() { return RetType1; }
    Type *getReturnType2() { return RetType2; }

    Type *getIntPtrType() { return IntPtrTy; }

    void insert(Value *V) {
      if (V==nullptr) return;
      if (BasicBlock *BB = dyn_cast<BasicBlock>(V))
        CreatedBBs.insert(BB);
      else if (Instruction *I = dyn_cast<Instruction>(V))
        CreatedInsts.insert(I);
    }
    //void insert(BasicBlock *BB) { CreatedBBs.insert(BB); }
    //void insert(Instruction *I) { CreatedInsts.insert(I); }

    void erase(BasicBlock *BB) { CreatedBBs.erase(BB); }
    void erase(Instruction *I) { CreatedInsts.erase(I); }

    virtual bool generate(AlignedSequence<Value *> &AlignedSeq,
                          ValueToValueMapTy &VMap,
                          const FunctionMergingOptions &Options = {}) = 0;

    virtual bool commitChanges(bool) = 0;

    void destroyGeneratedCode();

    SmallPtrSet<Instruction *, 8>::const_iterator begin() const {
      return CreatedInsts.begin();
    }
    SmallPtrSet<Instruction *, 8>::const_iterator end() const {
      return CreatedInsts.end();
    }
  };

  class SALSSACodeGen : public FunctionMerger::CodeGenerator {

    std::map<Instruction *, std::map<Instruction *, unsigned>>
      CoalescingCandidates;

  public:
    template <typename BlockListType>
    SALSSACodeGen(BlockListType &Blocks1, BlockListType &Blocks2)
        : CodeGenerator(Blocks1, Blocks2) {}
    virtual ~SALSSACodeGen() {}
    virtual bool generate(AlignedSequence<Value *> &AlignedSeq,
                          ValueToValueMapTy &VMap,
                          const FunctionMergingOptions &Options = {}) override;
    virtual bool commitChanges(bool) override;

    void StoreInstIntoAddr(Instruction *IV, Value *Addr);
    AllocaInst *MemfyInst(std::set<Instruction *> &InstSet);
  };
};

size_t EstimateFunctionSize(Function *F, TargetTransformInfo &TTI);

FunctionMergeResult MergeFunctions(Function *F1, Function *F2,
                                   const FunctionMergingOptions &Options = {});

class FunctionMergingPass : public PassInfoMixin<FunctionMergingPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};



static inst_range getInstructions(Function *F) { return instructions(F); }

static iterator_range<BasicBlock::iterator> getInstructions(BasicBlock *BB) {
  return make_range(BB->begin(), BB->end());
}

template <class T> class Fingerprint {
public:
  uint64_t magnitude{0};
  static const size_t MaxOpcode = 68;
  std::array<uint32_t, MaxOpcode> OpcodeFreq;

  Fingerprint() = default;

  Fingerprint(T owner, TargetTransformInfo *TTI=nullptr) {
    // memset(OpcodeFreq, 0, sizeof(int) * MaxOpcode);
    for (size_t i = 0; i < MaxOpcode; i++)
      OpcodeFreq[i] = 0;

    for (Instruction &I : getInstructions(owner)) {
      auto cost = (TTI!=nullptr)?TTI->getInstructionCost(&I, TargetTransformInfo::TargetCostKind::TCK_CodeSize).getValue().value():1;
      OpcodeFreq[I.getOpcode()] += cost;
      //if (I.isTerminator())
      //  OpcodeFreq[0] += I.getNumSuccessors();
    }
    for (size_t i = 0; i < MaxOpcode; i++) {
      uint64_t val = OpcodeFreq[i];
      magnitude += val * val;
    }
  }

  uint32_t footprint() const { return sizeof(int) * MaxOpcode; }

  float distance(const Fingerprint &FP2) const {
    int Distance = 0;
    for (size_t i = 0; i < MaxOpcode; i++) {
      int Freq1 = OpcodeFreq[i];
      int Freq2 = FP2.OpcodeFreq[i];
      Distance += std::abs(Freq1 - Freq2);
    }
    return static_cast<float>(Distance);
  }
};

class BlockFingerprint : public Fingerprint<BasicBlock *> {
public:
  BasicBlock *BB{nullptr};
  size_t Size{0};

  BlockFingerprint(BasicBlock *BB, TargetTransformInfo *TTI=nullptr) : Fingerprint(BB,TTI), BB(BB) {
    for (Instruction &I : *BB) {
      if (!isa<LandingPadInst>(&I) && !isa<PHINode>(&I)) {
        Size++;
      }
    }
  }
};

template<typename RegionT>
AlignedSequence<Value *> FunctionMerger::alignBlocks(RegionT &F1, RegionT &F2, AlignmentStats &TotalAlignmentStats, TargetTransformInfo *TTI, const FunctionMergingOptions &Options) {

  AlignedSequence<Value *> AlignedSeq;
  if (EnableHyFMNW) { // HyFM [NW]
    errs() << "Running: HyFM-NW\n";
    int B1Max = 0;
    int B2Max = 0;
    size_t MaxMem = 0;

    int NumBB1 = 0;
    int NumBB2 = 0;
    size_t MemSize = 0;

    std::vector<BlockFingerprint> Blocks;
    for (BasicBlock &BB1 : F1) {
      BlockFingerprint BD1(&BB1,TTI);
      MemSize += BD1.footprint();
      NumBB1++;
      Blocks.push_back(std::move(BD1));
    }

    for (BasicBlock &BIt : F2) {
      NumBB2++;
      BasicBlock *BB2 = &BIt;
      BlockFingerprint BD2(BB2,TTI);

      auto BestIt = Blocks.end();
      float BestDist = std::numeric_limits<float>::max();
      for (auto BDIt = Blocks.begin(), E = Blocks.end(); BDIt != E; BDIt++) {
        auto D = BD2.distance(*BDIt);
        if (D < BestDist) {
          BestDist = D;
          BestIt = BDIt;
        }
      }

      bool MergedBlock = false;
      if (BestIt != Blocks.end()) {
        auto &BD1 = *BestIt;
        BasicBlock *BB1 = BD1.BB;

        SmallVector<Value *, 8> BB1Vec;
        SmallVector<Value *, 8> BB2Vec;

        BB1Vec.push_back(BB1);
        for (auto &I : *BB1)
          if (!isa<PHINode>(&I) && !isa<LandingPadInst>(&I))
            BB1Vec.push_back(&I);

        BB2Vec.push_back(BB2);
        for (auto &I : *BB2)
          if (!isa<PHINode>(&I) && !isa<LandingPadInst>(&I))
            BB2Vec.push_back(&I);

        NeedlemanWunschSA<SmallVectorImpl<Value *>> SA(ScoringSystem(-1, 2), FunctionMerger::match);

        auto MemReq = SA.getMemoryRequirement(BB1Vec, BB2Vec);

        if (MemReq > MaxMem) {
          MaxMem = MemReq;
          B1Max = BB1Vec.size();
          B2Max = BB2Vec.size();
        }

        AlignedSequence<Value *> AlignedBlocks = SA.getAlignment(BB1Vec, BB2Vec);

        if (!HyFMProfitability || isSAProfitable(AlignedBlocks)) {
          extendAlignedSeq(AlignedSeq, AlignedBlocks, TotalAlignmentStats);
          Blocks.erase(BestIt);
          MergedBlock = true;
        }
      }

      if (!MergedBlock)
        extendAlignedSeq(AlignedSeq, nullptr, BB2, TotalAlignmentStats);
    }

    for (auto &BD1 : Blocks)
      extendAlignedSeq(AlignedSeq, BD1.BB, nullptr, TotalAlignmentStats);

  } else if (EnableHyFMPA) { // HyFM [PA]
    errs() << "Running: HyFM-PA\n";

    int NumBB1 = 0;
    int NumBB2 = 0;
    size_t MemSize = 0;

    std::map<size_t, std::vector<BlockFingerprint>> BlocksF1;
    for (BasicBlock &BB1 : F1) {
      BlockFingerprint BD1(&BB1,TTI);
      NumBB1++;
      MemSize += BD1.footprint();
      BlocksF1[BD1.Size].push_back(std::move(BD1));
    }

    for (BasicBlock &BIt : F2) {
      NumBB2++;
      BasicBlock *BB2 = &BIt;
      BlockFingerprint BD2(BB2,TTI);

      auto &SetRef = BlocksF1[BD2.Size];

      auto BestIt = SetRef.end();
      float BestDist = std::numeric_limits<float>::max();
      for (auto BDIt = SetRef.begin(), E = SetRef.end(); BDIt != E; BDIt++) {
        auto D = BD2.distance(*BDIt);
        if (D < BestDist) {
          BestDist = D;
          BestIt = BDIt;
        }
      }

      bool MergedBlock = false;
      if (BestIt != SetRef.end()) {
        BasicBlock *BB1 = BestIt->BB;

        if (!HyFMProfitability || isPAProfitable(BB1, BB2)) {
          extendAlignedSeq(AlignedSeq, BB1, BB2, TotalAlignmentStats);
          SetRef.erase(BestIt);
          MergedBlock = true;
        }
      }

      if (!MergedBlock)
        extendAlignedSeq(AlignedSeq, nullptr, BB2, TotalAlignmentStats);
    }

    for (auto &Pair : BlocksF1)
      for (auto &BD1 : Pair.second)
        extendAlignedSeq(AlignedSeq, BD1.BB, nullptr, TotalAlignmentStats);

  } else { //default SALSSA
    errs() << "Running: SALSSA\n";
    SmallVector<Value *, 8> F1Vec;
    SmallVector<Value *, 8> F2Vec;

    for (BasicBlock &BB : F1) {
      F1Vec.push_back(&BB);
      for (Instruction &I : BB) {
        if (!isa<LandingPadInst>(&I) && !isa<PHINode>(&I)) {
          F1Vec.push_back(&I);
        }
      }
    }
    for (BasicBlock &BB : F2) {
      F2Vec.push_back(&BB);
      for (Instruction &I : BB) {
        if (!isa<LandingPadInst>(&I) && !isa<PHINode>(&I)) {
          F2Vec.push_back(&I);
        }
      }
    }

    NeedlemanWunschSA<SmallVectorImpl<Value *>> SA(ScoringSystem(-1, 2), FunctionMerger::match);

    AlignedSeq = SA.getAlignment(F1Vec, F2Vec);

  }


  return AlignedSeq;
}


} // namespace llvm
#endif
