# =============================================================================
# rtl_core/Makefile — Verilator Build & Lock-Step Cosimulation Harness
# =============================================================================

VERILATOR ?= verilator
CC        ?= cc
CXX       ?= c++

DORADO_DIR := ../dorado
OBJ_DIR    := obj_dir
C_BUILD    := $(OBJ_DIR)/c_ref

SV_SRCS := \
	verilator.vlt \
	dorado_rtl_pkg.sv \
	dorado_rtl_alu_shifter.sv \
	dorado_rtl_cpu.sv \
	dorado_rtl_io.sv \
	dorado_rtl_top.sv

C_REF_SRCS := \
	$(DORADO_DIR)/src/mb.c \
	$(DORADO_DIR)/src/disasm.c \
	$(DORADO_DIR)/src/microcode.c \
	$(DORADO_DIR)/src/cpu.c \
	$(DORADO_DIR)/src/memory.c \
	$(DORADO_DIR)/src/io.c \
	$(DORADO_DIR)/src/fastio.c \
	$(DORADO_DIR)/src/baseboard.c \
	$(DORADO_DIR)/src/display.c \
	$(DORADO_DIR)/src/dispm.c \
	$(DORADO_DIR)/src/disk.c \
	$(DORADO_DIR)/src/ethernet.c \
	$(DORADO_DIR)/src/altoref.c \
	$(DORADO_DIR)/src/pdi.c

C_REF_OBJS := $(patsubst $(DORADO_DIR)/src/%.c,$(C_BUILD)/%.o,$(C_REF_SRCS))

CFLAGS   := -std=c99 -D_GNU_SOURCE -O3 -g -I$(abspath $(DORADO_DIR)/include) -I$(abspath $(DORADO_DIR)/vendor/6502)
CXXFLAGS := -std=c++17 -O3 -g -I$(abspath $(DORADO_DIR)/include)

# Use 64K storage words in cosimulation to keep Verilator C++ compile & init fast
VERILATOR_FLAGS := \
	--cc --exe --build -j 4 \
	-O3 --x-assign fast --x-initial fast --noassert \
	-Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ \
	-GSTORAGE_WORDS=65536 \
	--top-module dorado_rtl_top \
	-CFLAGS "$(CXXFLAGS)"

.PHONY: all test clean lint

all: $(OBJ_DIR)/Vdorado_rtl_top

$(C_BUILD)/%.o: $(DORADO_DIR)/src/%.c
	@mkdir -p $(C_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Vdorado_rtl_top: $(SV_SRCS) sim_cosim.cpp $(C_REF_OBJS)
	$(VERILATOR) $(VERILATOR_FLAGS) $(SV_SRCS) sim_cosim.cpp $(abspath $(C_REF_OBJS)) -o Vdorado_rtl_top

lint:
	$(VERILATOR) --lint-only -Wall -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ $(SV_SRCS)

test: $(OBJ_DIR)/Vdorado_rtl_top
	./$(OBJ_DIR)/Vdorado_rtl_top

clean:
	rm -rf $(OBJ_DIR)
