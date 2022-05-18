LLVM_HOME=/home/min/a/cgusthin/git/llvm-project-rocm/build_codesize
clang=$(LLVM_HOME)/bin/clang
opt=$(LLVM_HOME)/bin/opt
lli=$(LLVM_HOME)/bin/lli
#pass_location=$(LLVM_HOME)/lib/LLVMCFMelderPlugin.so
cfmerger_flags=-cfmelder  -S -force-cf-merging 
