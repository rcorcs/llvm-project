mkdir -p ../build
cd ../build/
mkdir -p release; cd release;
cmake -DLLVM_ENABLE_PROJECTS='clang;compiler-rt;lld' -DCMAKE_BUILD_TYPE="Release" -DLLVM_ENABLE_ASSERTIONS=On ../../llvm

make clang opt -j4
