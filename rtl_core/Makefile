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
	dorado_rtl_storage.sv \
	dorado_rtl_storage_ddr.sv \
	dorado_rtl_cpu.sv \
	dorado_rtl_io.sv \
	dorado_rtl_bootsrv.sv \
	dorado_rtl_assist.sv \
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

.PHONY: all test clean lint fpga-sim fpga-sim-ddr init

# The FPGA configuration in simulation: power-up images from init/, the
# in-FPGA boot server and loader assists, no C model (sim_fpga.cpp).
FPGA_OBJ := obj_fpga
init/start.txt: $(OBJ_DIR)/Vdorado_rtl_top
	mkdir -p init
	RTL_DUMP_INIT=init RTL_BOOT_ONLY=1 ./$(OBJ_DIR)/Vdorado_rtl_top > /dev/null || true

init: init/start.txt

$(FPGA_OBJ)/Vdorado_rtl_top: $(SV_SRCS) sim_fpga.cpp roms/bootfile.hex
	$(VERILATOR) --cc --exe --build -j 4 -O3 --x-assign fast --x-initial fast --noassert \
	  -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ \
	  -GSTORAGE_WORDS=65536 -GRESET_PC=$$(sed -n 's/^real_PC=0\([0-7]*\).*/16\x27o\1/p' init/start.txt) \
	  -DDORADO_INIT -DDORADO_NO_DISK --top-module dorado_rtl_top -Mdir $(FPGA_OBJ) \
	  -CFLAGS "$(CXXFLAGS)" $(SV_SRCS) sim_fpga.cpp -o Vdorado_rtl_top

# The same with storage in DDR3 (EXT_STORAGE, dorado_rtl_storage_ddr) against
# a model of the MiSTer DDRAM port.
FPGA_DDR_OBJ := obj_fpga_ddr
$(FPGA_DDR_OBJ)/Vdorado_rtl_top: $(SV_SRCS) sim_fpga.cpp roms/bootfile.hex
	$(VERILATOR) --cc --exe --build -j 4 -O3 --x-assign fast --x-initial fast --noassert \
	  -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ \
	  -GSTORAGE_WORDS=65536 -GEXT_STORAGE=1 -GRESET_PC=$$(sed -n 's/^real_PC=0\([0-7]*\).*/16\x27o\1/p' init/start.txt) \
	  -DDORADO_INIT -DDORADO_NO_DISK --top-module dorado_rtl_top -Mdir $(FPGA_DDR_OBJ) \
	  -CFLAGS "$(CXXFLAGS) -DSIM_DDR" $(SV_SRCS) sim_fpga.cpp -o Vdorado_rtl_top

fpga-sim-ddr: init/start.txt $(FPGA_DDR_OBJ)/Vdorado_rtl_top
	./$(FPGA_DDR_OBJ)/Vdorado_rtl_top 80000000 fpga-sim-ddr.pbm

fpga-sim: init/start.txt $(FPGA_OBJ)/Vdorado_rtl_top
	./$(FPGA_OBJ)/Vdorado_rtl_top 80000000 fpga-sim.pbm

all: $(OBJ_DIR)/Vdorado_rtl_top

$(C_BUILD)/%.o: $(DORADO_DIR)/src/%.c
	@mkdir -p $(C_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

roms/bootfile.hex roms/bol.hex: tools/make_boot_roms.py
	python3 tools/make_boot_roms.py roms

$(OBJ_DIR)/Vdorado_rtl_top: $(SV_SRCS) sim_cosim.cpp $(C_REF_OBJS) roms/bootfile.hex
	$(VERILATOR) $(VERILATOR_FLAGS) $(SV_SRCS) sim_cosim.cpp $(abspath $(C_REF_OBJS)) -o Vdorado_rtl_top

lint:
	$(VERILATOR) --lint-only -Wall -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ $(SV_SRCS)

test: $(OBJ_DIR)/Vdorado_rtl_top
	./$(OBJ_DIR)/Vdorado_rtl_top

clean:
	rm -rf $(OBJ_DIR) $(FPGA_OBJ) $(FPGA_DDR_OBJ)
