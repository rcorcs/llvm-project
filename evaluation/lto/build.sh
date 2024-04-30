
LLPATH=../../build/release/bin/


${LLPATH}/clang func-merg-1.c -O2 -o func-merg-1.o -flto -fuse-ld=lld -c -fno-inline-functions
${LLPATH}/clang func-merg-2.c -O2 -o func-merg-2.o -flto -fuse-ld=lld -c -fno-inline-functions
${LLPATH}/clang main.c -O2 -o main.o -flto -fuse-ld=lld -c -fno-inline-functions

${LLPATH}/clang func-merg-1.o func-merg-2.o main.o -O2 -flto -fuse-ld=lld -mllvm -enable-func-merging=true -mllvm -func-merging-hyfm-nw=true -o main -fno-inline-functions

