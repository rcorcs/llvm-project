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


test : is_same 

is_same : ${obj}
	@cmp -s  $(patsubst %.merged.ll,%.optimized.ll,$^) $^ ; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
					echo "ERROR : transformation did NOT change source IR"; \
	else \
					echo "SUCCESS : transformation did change source IR"; \
	fi






clean :
	rm  -f *.ll output*.* .*.dot

