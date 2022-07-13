BIN=tiff2dither
clang -O0 *.c -fprofile-instr-generate -o $BIN -lm
./$BIN -c none /home/rodrigo/ckr/datasets/image-tiff/dataset/a8b493fa0aa6238e/data.tiff /tmp/${BIN}.tiff
mv default.profraw $BIN-prof-1.profraw
./$BIN -c packbits /home/rodrigo/ckr/datasets/image-tiff/dataset/fcf8cd1279578639/data.tiff /tmp/${BIN}.tiff
mv default.profraw $BIN-prof-2.profraw
llvm-profdata merge -output=profdata $BIN-prof-1.profraw $BIN-prof-2.profraw 

