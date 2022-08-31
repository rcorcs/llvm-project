LLPATH := $(shell pwd)/../../build/release/bin
OPT := -Oz -fno-unroll-loops -fno-vectorize -fno-slp-vectorize
BIN := build/main

SOA=false
BRFUSION=false
HYFMPA=false
HYFMNW=false
CFMELDER=false
ATLEASTONE=false

ifeq ($(USE_HYFMPA),true)
HYFMPA=true
HYFMNW=false
endif
ifeq ($(USE_HYFMNW),true)
HYFMNW=true
HYFMPA=false
endif

ifeq ($(ENABLE_BRFUSION),true)
BRFUSION=true
ATLEASTONE=true
endif

ifeq ($(ENABLE_SOA),true)
SOA=true
HYFMPA=false
HYFMNW=true
ATLEASTONE=true
endif

ifeq ($(ENABLE_CFMELDER),true)
CFMELDER=true
ATLEASTONE=true
endif

EXTRA_OPT := -mllvm -enable-brfusion=$(ATLEASTONE) -mllvm -run-brfusion-only=$(BRFUSION) -mllvm -run-cfm-only=$(CFMELDER) -mllvm -brfusion-dot=false -mllvm -brfusion-soa=$(SOA)  -mllvm -func-merging-hyfm-nw=$(HYFMNW)  -mllvm -func-merging-hyfm-pa=$(HYFMPA) -mllvm -func-merging-debug=true -mllvm -func-merging-verbose=false

Bench := linux
#=================== RULES ==================
.PHONY: clean default build $(BIN) 

default: $(BIN)

build: $(BIN)

$(BIN):
	$(MAKE) -C linux LLVM=$(LLPATH)/ KCFLAGS="$(OPT) $(EXTRA_OPT)" V=0 vmlinux
	cp -a linux/vmlinux $(BIN)
	$(LLPATH)/llvm-size -B -d $(BIN)

clean:
	$(MAKE) -C linux clean

