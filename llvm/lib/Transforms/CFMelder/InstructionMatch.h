#ifndef LLVM_LIB_TRANSFORMS_INSTRUCTION_MATCH_H
#define LLVM_LIB_TRANSFORMS_INSTRUCTION_MATCH_H

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

// code is taken from : https://github.com/rcorcs/llvm-project/tree/func-merge

using namespace llvm;

namespace llvm { 

class InstructionMatch {
private:
  static bool IdenticalTypesOnly;
  static bool matchInstructions(Instruction *I1, Instruction *I2);

public:
  static bool match(Value *V1, Value *V2);
  static int getInstructionCost(Instruction* I);
};

}

#endif