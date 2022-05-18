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


test : test1 test2 test3 test4 

test1  : input1.txt
	@${lli} $(obj) < $^ > output.merged.txt
	@${lli} $(patsubst %.merged.ll, %.optimized.ll, $(obj)) < $^ > output.txt
	@diff output.merged.txt output.txt
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \

test2 : input2.txt
	@${lli} $(obj) < $^ > output.merged.txt
	@${lli} $(patsubst %.merged.ll, %.optimized.ll, $(obj)) < $^ > output.txt
	@diff output.merged.txt output.txt
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \

test3 : input3.txt
	@${lli} $(obj) < $^ > output.merged.txt
	@${lli} $(patsubst %.merged.ll, %.optimized.ll, $(obj)) < $^ > output.txt
	@diff output.merged.txt output.txt
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \

test4 : input4.txt
	@${lli} $(obj) < $^ > output.merged.txt
	@${lli} $(patsubst %.merged.ll, %.optimized.ll, $(obj)) < $^ > output.txt
	@diff output.merged.txt output.txt
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \


test5 : input5.txt
	@${lli} $(obj) < $^ > output.merged.txt
	@${lli} $(patsubst %.merged.ll, %.optimized.ll, $(obj)) < $^ > output.txt
	@diff output.merged.txt output.txt
	@if [ $$? -eq 0 ]; then \
		echo "PASS" ; \
	else \
		@echo "FAIL" ; \
	fi \




clean :
	rm  -f *.ll output*.*

