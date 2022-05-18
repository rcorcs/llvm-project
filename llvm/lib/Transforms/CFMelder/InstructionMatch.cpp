#include "InstructionMatch.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"

// code is taken from : https://github.com/rcorcs/llvm-project/tree/func-merge

static bool matchIntrinsicCalls(Intrinsic::ID ID, const CallInst *CI1,
                                const CallInst *CI2) {
  Intrinsic::ID ID1;
  Intrinsic::ID ID2;
  if (Function *F = CI1->getCalledFunction())
    ID1 = (Intrinsic::ID)F->getIntrinsicID();
  if (Function *F = CI2->getCalledFunction())
    ID2 = (Intrinsic::ID)F->getIntrinsicID();

  if (ID1 != ID)
    return false;
  if (ID1 != ID2)
    return false;

  switch (ID) {
  default:
    break;
  case Intrinsic::coro_id: {
    /*
    auto *InfoArg = CS.getArgOperand(3)->stripPointerCasts();
    if (isa<ConstantPointerNull>(InfoArg))
      break;
    auto *GV = dyn_cast<GlobalVariable>(InfoArg);
    Assert(GV && GV->isConstant() && GV->hasDefinitiveInitializer(),
      "info argument of llvm.coro.begin must refer to an initialized "
      "constant");
    Constant *Init = GV->getInitializer();
    Assert(isa<ConstantStruct>(Init) || isa<ConstantArray>(Init),
      "info argument of llvm.coro.begin must refer to either a struct or "
      "an array");
    */
    break;
  }
  case Intrinsic::ctlz: // llvm.ctlz
  case Intrinsic::cttz: // llvm.cttz
    // is_zero_undef argument of bit counting intrinsics must be a constant int
    return CI1->getArgOperand(1) == CI2->getArgOperand(1);
  case Intrinsic::experimental_constrained_fadd:
  case Intrinsic::experimental_constrained_fsub:
  case Intrinsic::experimental_constrained_fmul:
  case Intrinsic::experimental_constrained_fdiv:
  case Intrinsic::experimental_constrained_frem:
  case Intrinsic::experimental_constrained_fma:
  case Intrinsic::experimental_constrained_sqrt:
  case Intrinsic::experimental_constrained_pow:
  case Intrinsic::experimental_constrained_powi:
  case Intrinsic::experimental_constrained_sin:
  case Intrinsic::experimental_constrained_cos:
  case Intrinsic::experimental_constrained_exp:
  case Intrinsic::experimental_constrained_exp2:
  case Intrinsic::experimental_constrained_log:
  case Intrinsic::experimental_constrained_log10:
  case Intrinsic::experimental_constrained_log2:
  case Intrinsic::experimental_constrained_rint:
  case Intrinsic::experimental_constrained_nearbyint:
    // visitConstrainedFPIntrinsic(
    //    cast<ConstrainedFPIntrinsic>(*CS.getInstruction()));
    break;
  case Intrinsic::dbg_declare: // llvm.dbg.declare
    // Assert(isa<MetadataAsValue>(CS.getArgOperand(0)),
    //       "invalid llvm.dbg.declare intrinsic call 1", CS);
    // visitDbgIntrinsic("declare",
    // cast<DbgInfoIntrinsic>(*CS.getInstruction()));
    break;
  case Intrinsic::dbg_addr: // llvm.dbg.addr
    // visitDbgIntrinsic("addr", cast<DbgInfoIntrinsic>(*CS.getInstruction()));
    break;
  case Intrinsic::dbg_value: // llvm.dbg.value
    // visitDbgIntrinsic("value", cast<DbgInfoIntrinsic>(*CS.getInstruction()));
    break;
  case Intrinsic::dbg_label: // llvm.dbg.label
    // visitDbgLabelIntrinsic("label",
    // cast<DbgLabelInst>(*CS.getInstruction()));
    break;
  case Intrinsic::memcpy:
  case Intrinsic::memmove:
  case Intrinsic::memset: {
    // isvolatile argument of memory intrinsics must be a constant int
    return CI1->getArgOperand(3) == CI2->getArgOperand(3);
  }
  case Intrinsic::memcpy_element_unordered_atomic:
  case Intrinsic::memmove_element_unordered_atomic:
  case Intrinsic::memset_element_unordered_atomic: {
    const auto *AMI1 = cast<AtomicMemIntrinsic>(CI1);
    const auto *AMI2 = cast<AtomicMemIntrinsic>(CI2);

    ConstantInt *ElementSizeCI1 =
        dyn_cast<ConstantInt>(AMI1->getRawElementSizeInBytes());

    ConstantInt *ElementSizeCI2 =
        dyn_cast<ConstantInt>(AMI2->getRawElementSizeInBytes());

    return (ElementSizeCI1 != nullptr && ElementSizeCI1 == ElementSizeCI2);
  }
  case Intrinsic::gcroot:
  case Intrinsic::gcwrite:
  case Intrinsic::gcread:
    // llvm.gcroot parameter #2 must be a constant.
    return CI1->getArgOperand(1) == CI2->getArgOperand(1);
  case Intrinsic::init_trampoline:
    break;
  case Intrinsic::prefetch:
    // arguments #2 and #3 in llvm.prefetch must be constants
    return CI1->getArgOperand(1) == CI2->getArgOperand(1) &&
           CI1->getArgOperand(2) == CI2->getArgOperand(2);
  case Intrinsic::stackprotector:
    /*
    Assert(isa<AllocaInst>(CS.getArgOperand(1)->stripPointerCasts()),
           "llvm.stackprotector parameter #2 must resolve to an alloca.", CS);
    */
    break;
  case Intrinsic::lifetime_start:
  case Intrinsic::lifetime_end:
  case Intrinsic::invariant_start:
    // size argument of memory use markers must be a constant integer
    return CI1->getArgOperand(0) == CI2->getArgOperand(0);
  case Intrinsic::invariant_end:
    // llvm.invariant.end parameter #2 must be a constant integer
    return CI1->getArgOperand(1) == CI2->getArgOperand(1);
  case Intrinsic::localescape: {
    /*
    BasicBlock *BB = CS.getParent();
    Assert(BB == &BB->getParent()->front(),
           "llvm.localescape used outside of entry block", CS);
    Assert(!SawFrameEscape,
           "multiple calls to llvm.localescape in one function", CS);
    for (Value *Arg : CS.args()) {
      if (isa<ConstantPointerNull>(Arg))
        continue; // Null values are allowed as placeholders.
      auto *AI = dyn_cast<AllocaInst>(Arg->stripPointerCasts());
      Assert(AI && AI->isStaticAlloca(),
             "llvm.localescape only accepts static allocas", CS);
    }
    FrameEscapeInfo[BB->getParent()].first = CS.getNumArgOperands();
    SawFrameEscape = true;
    */
    break;
  }
  case Intrinsic::localrecover: {
    /*
    Value *FnArg = CS.getArgOperand(0)->stripPointerCasts();
    Function *Fn = dyn_cast<Function>(FnArg);
    Assert(Fn && !Fn->isDeclaration(),
           "llvm.localrecover first "
           "argument must be function defined in this module",
           CS);
    auto *IdxArg = dyn_cast<ConstantInt>(CS.getArgOperand(2));
    Assert(IdxArg, "idx argument of llvm.localrecover must be a constant int",
           CS);
    auto &Entry = FrameEscapeInfo[Fn];
    Entry.second = unsigned(
        std::max(uint64_t(Entry.second), IdxArg->getLimitedValue(~0U) + 1));
    */
    break;
  }
  };
  return false; // TODO: change to false by default
}

// bool FunctionMerger::matchLandingPad(LandingPadInst *LP1, LandingPadInst
// *LP2) {
static bool matchLandingPad(LandingPadInst *LP1, LandingPadInst *LP2) {
  if (LP1->getType() != LP2->getType())
    return false;
  if (LP1->isCleanup() != LP2->isCleanup())
    return false;
  if (LP1->getNumClauses() != LP2->getNumClauses())
    return false;
  for (unsigned i = 0; i < LP1->getNumClauses(); i++) {
    if (LP1->isCatch(i) != LP2->isCatch(i))
      return false;
    if (LP1->isFilter(i) != LP2->isFilter(i))
      return false;
    if (LP1->getClause(i) != LP2->getClause(i))
      return false;
  }
  return true;
}

bool matchLoadInsts(const LoadInst *LI1, const LoadInst *LI2) {
  return LI1->isVolatile() == LI2->isVolatile() &&
         LI1->getAlignment() == LI2->getAlignment() &&
         LI1->getOrdering() == LI2->getOrdering();
}

bool matchStoreInsts(const StoreInst *SI1, const StoreInst *SI2) {
  return SI1->isVolatile() == SI2->isVolatile() &&
         SI1->getAlignment() == SI2->getAlignment() &&
         SI1->getOrdering() == SI2->getOrdering();
}

bool matchAllocaInsts(const AllocaInst *AI1, const AllocaInst *AI2) {
  if (AI1->getArraySize() != AI2->getArraySize() ||
      AI1->getAlignment() != AI2->getAlignment())
    return false;

  /*
  // If size is known, I2 can be seen as equivalent to I1 if it allocates
  // the same or less memory.
  if (DL->getTypeAllocSize(AI->getAllocatedType())
        < DL->getTypeAllocSize(cast<AllocaInst>(I2)->getAllocatedType()))
    return false;

  */

  return true;
}

bool matchGetElementPtrInsts(const GetElementPtrInst *GEP1,
                             const GetElementPtrInst *GEP2) {
  SmallVector<Value *, 8> Indices1(GEP1->idx_begin(), GEP1->idx_end());
  SmallVector<Value *, 8> Indices2(GEP2->idx_begin(), GEP2->idx_end());
  if (Indices1.size() != Indices2.size())
    return false;

  if (GEP1->isInBounds() != GEP2->isInBounds())
    return false;

  /*
  //TODO: some indices must be constant depending on the type being indexed.
  //For simplicity, whenever a given index is constant, keep it constant.
  //This simplification may degrade the merging quality.
  for (unsigned i = 0; i < Indices1.size(); i++) {
    if (isa<ConstantInt>(Indices1[i]) && isa<ConstantInt>(Indices2[i]) &&
  Indices1[i] != Indices2[i]) return false; // if different constant values
  }
  */

  Type *AggTy1 = GEP1->getSourceElementType();
  Type *AggTy2 = GEP2->getSourceElementType();

  // Assert(all_of(
  //  Idxs, [](Value* V) { return V->getType()->isIntOrIntVectorTy(); }),
  //  "GEP indexes must be integers", &GEP);
  SmallVector<Value *, 16> Idxs1(GEP1->idx_begin(), GEP1->idx_end());
  SmallVector<Value *, 16> Idxs2(GEP2->idx_begin(), GEP2->idx_end());
  if (Idxs1.size() != Idxs2.size())
    return false;
  // for (unsigned i = 0; i<Idxs1.size(); i++) {
  //  if (Idxs1[i]!=Idxs2[i]) return false;
  //}

  return true;
}

bool matchSwitchInsts(const SwitchInst *SI1, const SwitchInst *SI2) {
  if (SI1->getNumCases() == SI2->getNumCases()) {
    auto CaseIt1 = SI1->case_begin(), CaseEnd1 = SI1->case_end();
    auto CaseIt2 = SI2->case_begin(), CaseEnd2 = SI2->case_end();
    do {
      auto *Case1 = &*CaseIt1;
      auto *Case2 = &*CaseIt2;
      if (Case1 != Case2)
        return false; // TODO: could allow permutation!
      ++CaseIt1;
      ++CaseIt2;
    } while (CaseIt1 != CaseEnd1 && CaseIt2 != CaseEnd2);
    return true;
  }
  return false;
}

bool matchCallInsts(const CallInst *CI1, const CallInst *CI2) {
  if (CI1->isInlineAsm() || CI2->isInlineAsm())
    return false;
  if (CI1->getCalledFunction() != CI2->getCalledFunction())
    return false;
  if (Function *F = CI1->getCalledFunction()) {
    if (Intrinsic::ID ID = (Intrinsic::ID)F->getIntrinsicID()) {

      if (!matchIntrinsicCalls(ID, CI1, CI2))
        return false;
    }
  }

  return CI1->getCallingConv() ==
         CI2->getCallingConv(); // &&
                                // CI->getAttributes() ==
                                // cast<CallInst>(I2)->getAttributes();
}

bool matchInvokeInsts(const InvokeInst *II1, const InvokeInst *II2) {
  return II1->getCallingConv() == II2->getCallingConv() &&
         matchLandingPad(II1->getLandingPadInst(), II2->getLandingPadInst());
}

bool matchInsertValueInsts(const InsertValueInst *IV1,
                           const InsertValueInst *IV2) {
  return IV1->getIndices() == IV2->getIndices();
}

bool matchExtractValueInsts(const ExtractValueInst *EV1,
                            const ExtractValueInst *EV2) {
  return EV1->getIndices() == EV2->getIndices();
}

bool matchFenceInsts(const FenceInst *FI1, const FenceInst *FI2) {
  return FI1->getOrdering() == FI2->getOrdering() &&
         FI1->getSyncScopeID() == FI2->getSyncScopeID();
}

bool InstructionMatch::IdenticalTypesOnly = true;

bool InstructionMatch::matchInstructions(Instruction *I1, Instruction *I2) {
  if (I1->getOpcode() != I2->getOpcode())
    return false;

  // Returns are special cases that can differ in the number of operands
  if (I1->getOpcode() == Instruction::Ret)
    return true;

  if (I1->getNumOperands() != I2->getNumOperands())
    return false;

  const DataLayout *DL =
      &I1->getParent()->getParent()->getParent()->getDataLayout();

  bool sameType = false;
  if (IdenticalTypesOnly) {
    sameType = (I1->getType() == I2->getType());
    for (unsigned i = 0; i < I1->getNumOperands(); i++) {
      sameType = sameType &&
                 (I1->getOperand(i)->getType() == I2->getOperand(i)->getType());
    }
  } else {
    assert(false &&
           "Matching instructions with non-identical types is not handled!");
    // sameType = areTypesEquivalent(I1->getType(), I2->getType(), DL, Options);
    // for (unsigned i = 0; i < I1->getNumOperands(); i++) {
    //   sameType = sameType &&
    //              areTypesEquivalent(I1->getOperand(i)->getType(),
    //                                 I2->getOperand(i)->getType(), DL,
    //                                 Options);
    // }
  }
  if (!sameType)
    return false;

  // if (I1->hasNoUnsignedWrap()!=I2->hasNoUnsignedWrap()) return false;
  // if (I1->hasNoSignedWrap()!=I2->hasNoSignedWrap()) return false;

  switch (I1->getOpcode()) {
    // case Instruction::Br: return false; //{ return (I1->getNumOperands()==1);
    // }

    //#define MatchCaseInst(Kind, I1, I2) case Instruction::#Kind

  case Instruction::Load:
    return matchLoadInsts(dyn_cast<LoadInst>(I1), dyn_cast<LoadInst>(I2));
  case Instruction::Store:
    return matchStoreInsts(dyn_cast<StoreInst>(I1), dyn_cast<StoreInst>(I2));
  case Instruction::Alloca:
    return matchAllocaInsts(dyn_cast<AllocaInst>(I1), dyn_cast<AllocaInst>(I2));
  case Instruction::GetElementPtr:
    return matchGetElementPtrInsts(dyn_cast<GetElementPtrInst>(I1),
                                   dyn_cast<GetElementPtrInst>(I2));
  case Instruction::Switch:
    return matchSwitchInsts(dyn_cast<SwitchInst>(I1), dyn_cast<SwitchInst>(I2));
  case Instruction::Call:
    return matchCallInsts(dyn_cast<CallInst>(I1), dyn_cast<CallInst>(I2));
  case Instruction::Invoke:
    return matchInvokeInsts(dyn_cast<InvokeInst>(I1), dyn_cast<InvokeInst>(I2));
  case Instruction::InsertValue:
    return matchInsertValueInsts(dyn_cast<InsertValueInst>(I1),
                                 dyn_cast<InsertValueInst>(I2));
  case Instruction::ExtractValue:
    return matchExtractValueInsts(dyn_cast<ExtractValueInst>(I1),
                                  dyn_cast<ExtractValueInst>(I2));
  case Instruction::Fence:
    return matchFenceInsts(dyn_cast<FenceInst>(I1), dyn_cast<FenceInst>(I2));
  case Instruction::AtomicCmpXchg: {
    const AtomicCmpXchgInst *CXI = dyn_cast<AtomicCmpXchgInst>(I1);
    const AtomicCmpXchgInst *CXI2 = cast<AtomicCmpXchgInst>(I2);
    return CXI->isVolatile() == CXI2->isVolatile() &&
           CXI->isWeak() == CXI2->isWeak() &&
           CXI->getSuccessOrdering() == CXI2->getSuccessOrdering() &&
           CXI->getFailureOrdering() == CXI2->getFailureOrdering() &&
           CXI->getSyncScopeID() == CXI2->getSyncScopeID();
  }
  case Instruction::AtomicRMW: {
    const AtomicRMWInst *RMWI = dyn_cast<AtomicRMWInst>(I1);
    return RMWI->getOperation() == cast<AtomicRMWInst>(I2)->getOperation() &&
           RMWI->isVolatile() == cast<AtomicRMWInst>(I2)->isVolatile() &&
           RMWI->getOrdering() == cast<AtomicRMWInst>(I2)->getOrdering() &&
           RMWI->getSyncScopeID() == cast<AtomicRMWInst>(I2)->getSyncScopeID();
  }
  default:
    if (const CmpInst *CI = dyn_cast<CmpInst>(I1))
      return CI->getPredicate() == cast<CmpInst>(I2)->getPredicate();
  }

  return true;
}

// bool InstructionMatch::matchLandingPad(LandingPadInst *LP1,
//                                        LandingPadInst *LP2) {}

bool InstructionMatch::match(Value *V1, Value *V2) {
  
  // always align branch instructions because we do a basic block wise merge
  if (isa<Instruction>(V1) && isa<Instruction>(V2)) {
    Instruction *I1 = dyn_cast<Instruction>(V1);
    Instruction *I2 = dyn_cast<Instruction>(V2);
    
    if (I1->getOpcode() == I2->getOpcode() &&
        I1->getOpcode() == Instruction::Br)
      return true;
  }

  if (isa<Instruction>(V1) && isa<Instruction>(V2)) {
    return matchInstructions(dyn_cast<Instruction>(V1),
                             dyn_cast<Instruction>(V2));
  } else if (isa<BasicBlock>(V1) && isa<BasicBlock>(V2)) {
    BasicBlock *BB1 = dyn_cast<BasicBlock>(V1);
    BasicBlock *BB2 = dyn_cast<BasicBlock>(V2);
    if (BB1->isLandingPad() || BB2->isLandingPad()) {
      LandingPadInst *LP1 = BB1->getLandingPadInst();
      LandingPadInst *LP2 = BB2->getLandingPadInst();
      if (LP1 == nullptr || LP2 == nullptr)
        return false;
      return matchLandingPad(LP1, LP2);
    } else
      return true;
  }
  return false;
}

int InstructionMatch::getInstructionCost(Instruction *I) {
  int SavedCycles = 0;
  switch (I->getOpcode()) {
  case Instruction::Add:
  case Instruction::FAdd:
    SavedCycles = 5;
    break;
  case Instruction::Mul:
  case Instruction::FMul:
    SavedCycles = 10;
    break;
  case Instruction::SDiv:
  case Instruction::UDiv:
  case Instruction::FDiv:
    SavedCycles = 20;
    break;
  case Instruction::Store:
  case Instruction::Load:
    SavedCycles = 100;
    break;
  default:
    SavedCycles = 3;
    break;
  }
  return SavedCycles;
}
