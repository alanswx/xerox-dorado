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
	dorado_rtl_mailbox.sv \
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

.PHONY: hps-sim all test clean lint fpga-sim fpga-sim-ddr init cedar

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

# The Cedar checkpoint lock-step harness (sim_cedar.cpp): the whole C machine
# (host services included) beside the RTL, 4M words of storage.
CEDAR_OBJ    := obj_cedar
CEDAR_C_SRCS := $(C_REF_SRCS) $(DORADO_DIR)/src/machine.c $(DORADO_DIR)/src/typetext.c
CEDAR_C_OBJS := $(patsubst $(DORADO_DIR)/src/%.c,$(C_BUILD)/%.o,$(CEDAR_C_SRCS))
CEDAR_STEPS  ?= 2000000

$(CEDAR_OBJ)/Vdorado_rtl_top: $(SV_SRCS) sim_cedar.cpp cosim_sync.h $(CEDAR_C_OBJS) roms/bootfile.hex
	$(VERILATOR) --cc --exe --build -j 4 -O3 --x-assign fast --x-initial fast --noassert \
	  -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ \
	  -GSTORAGE_WORDS=4194304 --top-module dorado_rtl_top -Mdir $(CEDAR_OBJ) \
	  -CFLAGS "$(CXXFLAGS) -DCOSIM_STORAGE_WORDS=4194304u" \
	  $(SV_SRCS) sim_cedar.cpp $(abspath $(CEDAR_C_OBJS)) -o Vdorado_rtl_top

cedar: $(CEDAR_OBJ)/Vdorado_rtl_top
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./$(CEDAR_OBJ)/Vdorado_rtl_top $(CEDAR_STEPS)

# The MiSTer Cedar configuration (DDR storage, HPS mailbox, the checkpoint in
# init_cedar/ from `CEDAR_SPLIT=1 CEDAR_DUMP_INIT=../rtl_core/init_cedar
# obj_cedar/Vdorado_rtl_top 0`), with the HPS program in-process (sim_hps.cpp).
HPS_OBJ   := obj_hps
HPS_STEPS ?= 2000000

$(HPS_OBJ)/Vdorado_rtl_top: $(SV_SRCS) sim_hps.cpp sim_ddr_model.h hps/hps_host.cpp hps/hps_host.h $(CEDAR_C_OBJS) init_cedar/checkpoint_cpu.svh
	$(VERILATOR) --cc --exe --build -j 4 -O3 --x-assign fast --x-initial fast --noassert \
	  -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ \
	  -GSTORAGE_WORDS=4194304 -GEXT_STORAGE=1 -GHPS_MAILBOX=1 \
	  -DDORADO_INIT -DDORADO_CHECKPOINT -DDORADO_NO_DISK -I$(abspath init_cedar) \
	  --top-module dorado_rtl_top -Mdir $(HPS_OBJ) \
	  -CFLAGS "$(CXXFLAGS) -I$(abspath .)" \
	  $(SV_SRCS) sim_hps.cpp $(abspath hps/hps_host.cpp) $(abspath $(CEDAR_C_OBJS)) -o Vdorado_rtl_top
	ln -sfn ../init_cedar $(HPS_OBJ)/init

hps-sim: $(HPS_OBJ)/Vdorado_rtl_top
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./$(HPS_OBJ)/Vdorado_rtl_top $(HPS_STEPS)

all: $(OBJ_DIR)/Vdorado_rtl_top

$(C_BUILD)/%.o: $(DORADO_DIR)/src/%.c
	@mkdir -p $(C_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

roms/bootfile.hex roms/bol.hex: tools/make_boot_roms.py
	python3 tools/make_boot_roms.py roms

$(OBJ_DIR)/Vdorado_rtl_top: $(SV_SRCS) sim_cosim.cpp cosim_sync.h $(C_REF_OBJS) roms/bootfile.hex
	$(VERILATOR) $(VERILATOR_FLAGS) $(SV_SRCS) sim_cosim.cpp $(abspath $(C_REF_OBJS)) -o Vdorado_rtl_top

lint:
	$(VERILATOR) --lint-only -Wall -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ $(SV_SRCS)

test: $(OBJ_DIR)/Vdorado_rtl_top
	./$(OBJ_DIR)/Vdorado_rtl_top

clean:
	rm -rf $(OBJ_DIR) $(FPGA_OBJ) $(FPGA_DDR_OBJ) $(CEDAR_OBJ)
