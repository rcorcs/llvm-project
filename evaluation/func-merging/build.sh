
LLPATH=../../build/release/bin/

${LLPATH}/clang $1 -O1 -emit-llvm -S -o $1.ll
${LLPATH}/opt $1.ll -passes=func-merging -S -o $1.opt.ll

