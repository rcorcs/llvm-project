.SUFFIXES:

.SECONDARY:

BIN=main
LDFLAGS=-lm

#CCBIN=/home/rcor/dev/CSLPinSLP/build/bin/
CCBIN=/home/rcor/dev/llvm-build/build/bin/

#no-vec
#OPTS=-O3 -disable-slp-vectorization -disable-loop-vectorization -enable-unsafe-fp-math

#no-reordering
#OPTS=-O3 -slp-vectorizer -disable-loop-vectorization -disable-input-reordering -enable-unsafe-fp-math

#vanilla slp
OPTS=-O3 -slp-vectorizer -cslp -disable-loop-vectorization -enable-unsafe-fp-math

#cslp[node-size=1]
#OPTS=-O3 -slp-vectorizer -disable-loop-vectorization -cslp -cslp-fnsize-limit=2 -enable-unsafe-fp-math

SRC=$(wildcard *.c)
LBINS=$(SRC:.c=.bc)
OBJS=$(SRC:.c=.o)

%.bc: %.c
	$(CCBIN)clang -w -emit-llvm -c -O0 $< -o $@ $(CFLAGS)
	$(CCBIN)opt $(OPTS) $@ -o $@
ifeq ($(KEEPLLVM),True)
	llvm-dis $@
endif

$(BIN): $(LBINS)
	llvm-link $(LBINS) -o $(BIN).bc
	#llvm-link /home/rcor/bin/exp/execution_timer.bc $(BIN).bc -o $(BIN).bc
	llc $(BIN).bc -o $(BIN).o -filetype=obj -mattr=+avx2
	g++ $(BIN).o -o $(BIN) $(LDFLAGS)
	rm -f $(BIN).bc
	rm -f $(BIN).o

clean:
	rm -f $(LBINS)
	rm -f $(OBJS)

clear-all:
	rm -f $(LBINS)
	rm -f $(OBJS)
	rm -f $(BIN)

