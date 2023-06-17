
#ifndef LLVM_TRANSFORMS_SCALAR_REGIONROLLING_H
#define LLVM_TRANSFORMS_SCALAR_REGIONROLLING_H

class RegionRoller {
public:
  RegionRoller(Function &F) : F(F) {}
  bool run();
private:
  Function &F;
};

#endif

