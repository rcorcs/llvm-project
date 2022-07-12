BIN=tiff2median
clang -O0 *.c -fprofile-instr-generate -o $BIN -lm
./$BIN -f lsb2msb -c none /home/rodrigo/ckr/datasets/image-tiff-bw/dataset/ff12044d79b78333/data.tiff /tmp/${BIN}.tiff
mv default.profraw $BIN-prof-1.profraw
./$BIN -f lsb2msb -c none /home/rodrigo/ckr/datasets/image-tiff-bw/dataset/ef44452eb2c322b0/data.tiff /tmp/${BIN}.tiff
mv default.profraw $BIN-prof-2.profraw
llvm-profdata merge -output=profdata $BIN-prof-1.profraw $BIN-prof-2.profraw 

