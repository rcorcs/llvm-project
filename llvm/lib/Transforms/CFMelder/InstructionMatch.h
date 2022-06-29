#ifndef LLVM_LIB_TRANSFORMS_INSTRUCTION_MATCH_H
#define LLVM_LIB_TRANSFORMS_INSTRUCTION_MATCH_H

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/IPO/FunctionMerging.h"

// code is taken from : https://github.com/rcorcs/llvm-project/tree/func-merge

using namespace llvm;

namespace llvm {

class InstructionMatch {
public:
  static bool match(Value *V1, Value *V2) {
    if (isa<Instruction>(V1) && isa<Instruction>(V2)) {
      Instruction *I1 = dyn_cast<Instruction>(V1);
      Instruction *I2 = dyn_cast<Instruction>(V2);

      if (I1->getOpcode() == I2->getOpcode() &&
          I1->getOpcode() == Instruction::Br)
        return true;
    }
    return FunctionMerger::match(V1, V2);
  };
  static int getInstructionCost(Instruction *I);
};

} // namespace llvm

#endif