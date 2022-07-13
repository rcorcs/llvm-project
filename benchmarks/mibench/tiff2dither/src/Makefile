.SUFFIXES:

.SECONDARY:

BIN=main

-include Makefile.config
-include Instrumentation.config
-include Benchmark.config

SRC=$(wildcard *.c)
LBINS=$(SRC:.c=.bc)
OBJS=$(SRC:.c=.o)

%.bc: %.c
	clang -w $(USEPROFDATA) -emit-llvm -c -O0 $< -o $@ $(CFLAGS)
ifdef INSTR
	opt -load $(INSTR_PATH) $(INSTR) $@ -o $@
ifdef USEPROFDATA
	opt -load /home/rodrigo/dev/prof/work-instr-32/build/src/libStripProfMD.so -strip-prof-md $@ -o $@
endif
	#python clear-llvm.py $@ > $@.tmp
	#mv $@.tmp $@
endif
	opt $(OPTS) $@ -o $@
ifeq ($(KEEPLLVM),True)
	llvm-dis $@
endif

#%.o: %.bc
#	llc $< -o $@ -filetype=obj	


$(BIN): $(LBINS)
	#gcc $(OBJS) -o $(BIN)
	llvm-link $(LBINS) -o $(BIN).bc
ifdef INSTR_LINK
	llvm-link $(BIN).bc $(INSTR_LINK) -o $(BIN).bc
endif
	llvm-link /home/rodrigo/mybench/execution_timer.bc $(BIN).bc -o $(BIN).bc
ifeq ($(USEPAPI),-lpapi)
	llvm-link /home/rodrigo/dev/prof/papi/ipc-prof.bc $(BIN).bc -o $(BIN).bc
endif
	llc $(BIN).bc -o $(BIN).o -filetype=obj
	g++ $(BIN).o -o $(BIN) $(LDFLAGS) $(USEPAPI)
	#clang -x ir -O0 $(BIN).ll -o $(BIN) $(LDFLAGS) -march=native
	rm -f $(BIN).bc
	rm -f $(BIN).o

build: $(BIN)

clean:
	rm -f $(LBINS)
	rm -f $(OBJS)

clear-all:
	rm -f $(LBINS)
	rm -f $(OBJS)
	rm -f $(BIN)

prof:
	clang -O0 $(SRC) -fprofile-instr-generate -o $(BIN) $(CFLAGS) $(LDFLAGS)
	./$(BIN) $(INPUT)
	#mv default.profraw profraw
	llvm-profdata merge -output=profdata default.profraw
