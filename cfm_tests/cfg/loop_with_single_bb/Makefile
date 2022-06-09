include ../config.mk

clang_flags=-emit-llvm -S -Xclang -disable-O0-optnone 
opt_flags=-sroa -S

obj := $(patsubst %.c,%.merged.ll,$(wildcard *.c))
inputs := $(wildcard input*.txt)
all : ${obj}
	
%.merged.ll : %.c	
	${clang} ${clang_flags} $^
	${opt} ${opt_flags} < $(patsubst %.c,%.ll,$^) > $(patsubst %.c,%.optimized.ll,$^)
	${opt}  ${cfmerger_flags} < $(patsubst %.c,%.optimized.ll,$^) > $@



test : ${obj}
	@diff $^ $(patsubst %.merged.ll,%.optimized.ll,$^)
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \

clean :
	rm  -f *.ll output*.* .*.dot

