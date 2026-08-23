# Verilator + Dear ImGui harness for the Dorado RTL.
#
# Adapted from the Apple-IIgs_MiSTer `vsim` harness (the newest of the local
# MiSTer sim frameworks, Jul-2026), so the same `sim/` support library --
# sim_video, sim_bus, sim_input, sim_clock, sim_console, sim_audio,
# sim_blkdevice, and vendored Dear ImGui -- carries over unchanged. Keeping
# that interface identical is the point: the top module is MiSTer's `emu`,
# so RTL that runs here runs on MiSTer without a second port.
#
# NOTE: sim.v (the MiSTer `emu` wrapper for the Dorado) and sim_main.cpp are
# NOT written yet -- `make` will not link until they are. `make lint` is the
# working gate today and proves every generated board still elaborates.
# The IIgs harness's own Makefile is kept as Makefile.iigs-reference.
#
#   make            build ./obj_dir/Vemu  (needs sim.v + sim_main.cpp)
#   make lint       lint the generated boards only (fast, no C++)
#   make boards     regenerate the RTL from chm/sil (needs python3)
#   make run        build and run
#
# The RTL under ../generated is GENERATED from PARC's Sil wire lists by
# tools/sil_to_verilog.py -- do not edit it; regenerate.

# EVERY TEST RULE ENDS IN `| grep`, and a pipeline's exit status is the LAST
# command's -- so a testbench that hit `$$fatal` still left the rule green.
# Found 2026-08-18, when a cell fix broke two tests that both reported PASS.
#
# `SHELL := /bin/bash` + `.SHELLFLAGS := -o pipefail -c` is the usual fix and
# it does NOTHING here: macOS ships GNU Make 3.81 and `.SHELLFLAGS` arrived in
# 3.82, so make quietly ignored it -- the same failure mode again. Each rule
# captures the status explicitly instead, which works in any make and any sh.
#
V        = verilator
TOP      = --top-module emu
RTL      = ../rtl
CELLS    = ../cells
VENDOR   = ../vendor/6502
VENDOR2  = ../vendor/riot
GEN      = ../generated
SIM      = ./sim

# Board RTL: the eleven boards of a working monochrome machine, which is what
# `dorado_backplane` instantiates. Override BOARDS to build a subset, and
# regenerate a matching top with
#   make -C .. backplane MACHINE="--boards=ProcH,ProcL --module=dorado_proc"
BOARDS  ?= ProcH-Rev-Ce ProcL-Rev-Ci ContA-Rev-Cd ContB-Rev-Cd IFU-Rev-Ch \
           MemC-Rev-Be MemD-Rev-Ca MemX-Rev-Ch DskEth-Rev-Cf DispY-Rev-Cl \
           BaseBd-Rev-Am
BOARD_V  = $(addprefix $(GEN)/,$(addsuffix .v,$(BOARDS)))

# The generated top module carries both `dorado_backplane` and the
# `dorado_machine` wrapper sim.v instantiates.
TOP_GEN  = $(GEN)/dorado_backplane.v

V_SRC    = sim.v $(TOP_GEN) $(BOARD_V)

# PINMISSING is expected and benign: a cell declares every pin the part uses
# on ANY of the sixteen boards, while an instance connects only the pins that
# board wires. Suppressing it here rather than in the generator keeps the
# generated RTL honest about what is actually connected.
# LATCH is the F10145A 16x4 ECL register file doing what the part does: its
# write is ASYNCHRONOUS (level-sensitive on CE'/WE'), so a latch is correct
# and 405 instances of it would drown a real warning.
V_WARN   = -Wno-PINMISSING -Wno-DECLFILENAME -Wno-UNDRIVEN -Wno-UNUSEDSIGNAL \
           -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNOPTFLAT -Wno-MULTIDRIVEN \
           -Wno-LATCH -Wno-fatal

V_DEFINE = +define+SIMULATION=1 \
           -CFLAGS "-I../$(SIM)/imgui -I../$(SIM) -g" \
           --timescale-override 1ps/1ps -Wno-TIMESCALEMOD \
           -I$(CELLS) -I$(GEN) -I$(VENDOR) -I$(VENDOR2) -I.

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	CXXFLAGS += `sdl2-config --cflags` -I../$(SIM)/imgui
	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif
ifeq ($(UNAME_S), Linux)
	LIBS += -lGL -ldl `sdl2-config --libs`
	CXXFLAGS += `sdl2-config --cflags` -I../$(SIM)/imgui
endif

IMGUI = $(SIM)/imgui/imgui.cpp $(SIM)/imgui/imgui_demo.cpp \
        $(SIM)/imgui/imgui_draw.cpp $(SIM)/imgui/imgui_tables.cpp \
        $(SIM)/imgui/imgui_widgets.cpp \
        $(SIM)/imgui/imgui_impl_sdl.cpp $(SIM)/imgui/imgui_impl_opengl2.cpp

# The MiSTer sim_* helpers stay in sim/ for when the design produces video
# and reads a bus; they are not linked yet, because the boards do not compute
# and an unused helper that fails to build is just a broken build.
SIM_SRC = sim_main.cpp $(IMGUI)

.PHONY: all lint boards run clean
all:
	$(V) $(TOP) $(V_WARN) $(V_DEFINE) --cc $(V_SRC) \
		--exe $(SIM_SRC) \
		-CFLAGS '$(CXXFLAGS)' -LDFLAGS '$(LIBS)'
	$(MAKE) -j -C obj_dir -f Vemu.mk Vemu

# Lint only the generated boards -- no C++, no imgui, seconds not minutes.
# This is the gate worth running after regenerating: it proves every board
# still elaborates.
# Do the generated PROM contents read back correctly through the RTL? This
# checks the WIRING -- A0 is the most significant address bit and Q0 the most
# significant output bit -- against the closed forms the Hardware Manual and
# the C emulator use, not against the .mem files, which would be circular.
# Run from the repository root: the INIT_FILE paths are repo-relative.
.PHONY: prom-test
prom-test:
	@cd ../.. && $(V) --binary --top-module tb_proms $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_proms -o tb_proms \
		verilog/verilator/tb_proms.sv >/dev/null && \
		{ out=$$(./verilog/verilator/obj_proms/tb_proms 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_proms; exit $$rc; }

# Does the ALU compute what its datasheet says? cell_MC10181 is the Dorado's
# ALU -- two slices on ProcH, two on ProcL -- and it is a transcription of the
# datasheet's function table, so it gets checked against INDEPENDENT
# expressions of the same functions, exhaustively over both operands.
.PHONY: alu-test
alu-test:
	@cd ../.. && $(V) --binary --top-module tb_alu $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_alu -o tb_alu \
		verilog/verilator/tb_alu.sv >/dev/null && \
		{ out=$$(./verilog/verilator/obj_alu/tb_alu 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_alu; exit $$rc; }

# THE CROSS-CHECK the whole approach is for: four MC10181 slices chained into
# the 16-bit ALU, against vectors the C emulator's own alu_op() produces. The
# cell is a transcription of Motorola's datasheet, alu_op() was written from
# the Hardware Manual -- neither derived from the other. Also the only test
# that exercises the carry ripple BETWEEN slices.
.PHONY: alu-diff
alu-diff:
	@$(MAKE) -s -C ../../dorado build/alu_vectors
	@cd ../.. && ./dorado/build/alu_vectors > verilog/verilator/obj_alu/alu.vec
	@cd ../.. && $(V) --binary --top-module tb_alu_vs_c $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_alu -o tb_alu_vs_c \
		verilog/verilator/tb_alu_vs_c.sv >/dev/null && \
		{ out=$$(./verilog/verilator/obj_alu/tb_alu_vs_c \
			+vectors=verilog/verilator/obj_alu/alu.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_alu_vs_c; exit $$rc; }

# Does the assembled machine actually run? The gate is the CLOCK: the
# BaseBoard generates it and fans it out to every slot, and if those nets stop
# moving nothing downstream can compute. Deliberately NOT a toggle count --
# see tools/rtl_machine_check.py for why that would punish correct cells.
.PHONY: machine-test
# The assembled machine under the EVENT SCHEDULER -- the control for
# machine-test, and five seconds against its several minutes. See
# tb_converge.sv for what the pair says.
.PHONY: converge-test
converge-test:
	@cd ../.. && $(V) --binary --top-module tb_converge $(V_WARN) \
		-Iverilog/cells -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_converge -o tb_converge \
		verilog/verilator/tb_converge.sv \
		verilog/generated/dorado_backplane.v $(BOARD_V:../%=verilog/%) \
		>/dev/null && { out=$$(./verilog/verilator/obj_converge/tb_converge 2>&1); \
		  rc=$$?; echo "$$out" | grep tb_converge; exit $$rc; }

# THE MACHINE COMPUTES: PARC's own ALU prologue on four boards -- 25 octal
# from CPReg into Q, held across an instruction, and stored into ALUFM[0].
# Note the exit code is captured EXPLICITLY: a %Fatal line contains the string
# "tb_compute", so piping straight into grep would report a failed assertion
# as a pass.
.PHONY: compute-test
compute-test:
	@cd ../.. && $(V) --binary --top-module tb_compute $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_compute -o tb_compute \
		verilog/verilator/tb_compute.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_compute/tb_compute 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_compute|Assertion"; exit $$rc; }

# PARC's IRTable entries against the IM parity generators. A DIAGNOSTIC, not a
# gate: all thirteen still FAIL, and tb_parity.sv now says exactly why and what
# the one-line fix costs elsewhere.
.PHONY: parity-probe
parity-probe:
	@cd ../.. && $(V) --binary --top-module tb_parity $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_parity -o tb_parity \
		verilog/verilator/tb_parity.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_parity/tb_parity 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_parity|Assertion"; exit $$rc; }

# THE MACHINE EXECUTES MICROCODE OUT OF IM.
.PHONY: exec-test
exec-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks
	@mkdir -p obj_exec
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_exec -o tb_exec \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec/tb_exec \
		    +vectors=verilog/verilator/obj_exec/exec.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# PARC's BLOCK LOADER: a 17-byte hunk walked into IM, read back word for word.
.PHONY: boot0-test
boot0-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks
	@mkdir -p obj_boot0
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 16 \
		> verilog/verilator/obj_boot0/boot0.vec
	@cd ../.. && $(V) --binary --top-module tb_boot0 $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_boot0 -o tb_boot0 \
		verilog/verilator/tb_boot0.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_boot0/tb_boot0 \
		    +vectors=verilog/verilator/obj_boot0/boot0.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_boot0; exit $$rc; }

# PARC's SendViaMIR: Boot0's inner loop, loading words into IM.
.PHONY: sendmir
sendmir:
	@cd ../.. && $(V) --binary --top-module tb_sendmir $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_sendmir -o tb_sendmir \
		verilog/verilator/tb_sendmir.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_sendmir/tb_sendmir 2>&1); \
		  rc=$$?; echo "$$out" | grep tb_sendmir; exit $$rc; }

# ONE MICROINSTRUCTION AT A TIME -- the primitive Boot0 is built out of.
.PHONY: step-test
step-test:
	@cd ../.. && $(V) --binary --top-module tb_step $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_step -o tb_step \
		verilog/verilator/tb_step.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_step/tb_step 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_step; exit $$rc; }

# THE HARNESS IS A DEPENDENCY, not an assumption. This used to run whatever
# ./obj_dir/Vemu happened to be lying there, and on 2026-08-18 that turned out
# to be a binary a week older than the cell library it was supposed to be
# testing. Rebuild first; `make all` is incremental.
machine-test: all
	@cd ../.. && { out=$$(./verilog/verilator/obj_dir/Vemu --headless --cycles 20000 2>&1); \
	  echo "$$out" | python3 tools/rtl_machine_check.py \
			verilog/generated/dorado_backplane.probes; }

# THE BOOT INTERFACE, RTL against the C emulator. A Dorado starts by the
# BaseBoard's 6502 driving nine data bits, a three-bit function code and a
# strobe across the backplane; ContA decodes it and latches CPReg. The C
# emulator models the same protocol from PARC's doradoio.mdefs, so the two
# check each other -- see tb_cpreg.sv.
.PHONY: cpreg-diff
cpreg-diff:
	@$(MAKE) -s -C ../../dorado build/cpreg_vectors
	@mkdir -p obj_cpreg
	@cd ../.. && ./dorado/build/cpreg_vectors > verilog/verilator/obj_cpreg/cpreg.vec
	@cd ../.. && $(V) --binary --top-module tb_cpreg $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_cpreg -o tb_cpreg \
		verilog/verilator/tb_cpreg.sv verilog/generated/ContA-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_cpreg/tb_cpreg \
			+vectors=verilog/verilator/obj_cpreg/cpreg.vec 2>&1); rc=$$?; echo "$$out" | grep tb_cpreg; exit $$rc; }

# THE MICROINSTRUCTION LOAD PATH, all 36 bits. Six MC10172s on ContA and ContB
# demultiplex nine CPOut lines across four function codes into the field lines,
# and dorado_decode_mir() in disasm.c says where each byte's bits belong -- one
# from PARC's 1979 wire list, one from the Hardware Manual. See tb_mir.sv.
.PHONY: mir-diff
mir-diff:
	@$(MAKE) -s -C ../../dorado build/cpreg_vectors
	@mkdir -p obj_cpreg
	@cd ../.. && ./dorado/build/cpreg_vectors > verilog/verilator/obj_cpreg/cpreg.vec
	@cd ../.. && $(V) --binary --top-module tb_mir $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_mir -o tb_mir \
		verilog/verilator/tb_mir.sv verilog/generated/ContA-Rev-Cd.v \
		verilog/generated/ContB-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_mir/tb_mir \
			+vectors=verilog/verilator/obj_cpreg/cpreg.vec 2>&1); rc=$$?; echo "$$out" | grep tb_mir; exit $$rc; }

# A WHOLE MICROINSTRUCTION jammed into the register and read back, which is
# what the BaseBoard does 475 times during a cold boot. Needs BOTH Control
# boards wired by the backplane, because the MIR reset crosses it. See
# tb_mirreg.sv.
.PHONY: mirreg-diff
mirreg-diff:
	@$(MAKE) -s -C ../../dorado build/cpreg_vectors
	@mkdir -p obj_cpreg
	@cd ../.. && ./dorado/build/cpreg_vectors > verilog/verilator/obj_cpreg/cpreg.vec
	@cd ../.. && $(V) --binary --top-module tb_mirreg $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_mirreg -o tb_mirreg \
		verilog/verilator/tb_mirreg.sv verilog/generated/dorado_control.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_mirreg/tb_mirreg \
			+vectors=verilog/verilator/obj_cpreg/cpreg.vec 2>&1); rc=$$?; echo "$$out" | grep tb_mirreg; exit $$rc; }

# THE JAM'S OPERAND: the data written into IM comes from CPReg, once the MIR
# clock is held by PARC's own two manifold words. This is the half writeim-test
# says it does not prove. See tb_operand.sv.
.PHONY: operand-test
operand-test:
	@cd ../.. && $(V) --binary --top-module tb_operand $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_operand -o tb_operand \
		verilog/verilator/tb_operand.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_operand/tb_operand 2>&1); rc=$$?; echo "$$out" | grep tb_operand; exit $$rc; }

# A JAMMED WRITE-IM deposits into IM, and RSTK[3] selects the half -- checked
# both ways against a wiped array. See tb_writeim.sv for why the baseline is the
# whole difficulty.
.PHONY: writeim-test
writeim-test:
	@cd ../.. && $(V) --binary --top-module tb_writeim $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_writeim -o tb_writeim \
		verilog/verilator/tb_writeim.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_writeim/tb_writeim 2>&1); rc=$$?; echo "$$out" | grep tb_writeim; exit $$rc; }

# FOUR BOARDS: Control plus both processor boards, running, with a jammed
# microinstruction's fields on the datapath. See tb_datapath.sv.
.PHONY: datapath-test
datapath-test:
	@cd ../.. && $(V) --binary --top-module tb_datapath $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_datapath -o tb_datapath \
		verilog/verilator/tb_datapath.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_datapath/tb_datapath 2>&1); rc=$$?; echo "$$out" | grep tb_datapath; exit $$rc; }

# THE MACHINE RUNS: the Control section executes microinstruction cycles after
# PARC's boot sequence, with clk2' at exactly twice clk0'. See tb_run.sv.
.PHONY: run-test
run-test:
	@cd ../.. && $(V) --binary --top-module tb_run $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_run -o tb_run \
		verilog/verilator/tb_run.sv verilog/generated/dorado_control.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_run/tb_run 2>&1); rc=$$?; echo "$$out" | grep tb_run; exit $$rc; }

# PARC's own DoDoradoMicroInst, replayed. A DIAGNOSTIC, not a gate: it asserts
# nothing and shows where the Control section stops. See tb_startseq.sv.
.PHONY: startseq
startseq:
	@cd ../.. && $(V) --binary --top-module tb_startseq $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_startseq -o tb_startseq \
		verilog/verilator/tb_startseq.sv verilog/generated/dorado_control.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_startseq/tb_startseq 2>&1); rc=$$?; echo "$$out" | grep startseq; exit $$rc; }

# Does the BaseBoard's 6502 come out of reset and fetch from its own EPROMs?
# The whole board is real here -- Andrew Holme's netlist-derived 6502, MiSTer's
# 6532s, the '138 that decodes Rom0'..Rom7', the eight 2716 sockets, and the
# crystal/counter/flip-flop chain that clocks and releases it.
.PHONY: baseboard-test
# LET THE REAL FIRMWARE DRIVE THE MACHINE -- BaseBd + ContA + ContB + ProcH +
# ProcL, with the 6502 running its own EPROMs. A DIAGNOSTIC, not a gate.
.PHONY: firmware-probe
firmware-probe:
	@cd ../.. && $(V) --binary --top-module tb_firmware $(V_WARN) \
		-Iverilog/cells -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_fw -o tb_firmware \
		verilog/verilator/tb_firmware.sv \
		verilog/generated/dorado_boot.v \
		verilog/generated/BaseBd-Rev-Am.v verilog/generated/ContA-Rev-Cd.v \
		verilog/generated/ContB-Rev-Cd.v verilog/generated/ProcH-Rev-Ce.v \
		verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_fw/tb_firmware 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_firmware|Assertion"; exit $$rc; }

# THE MEMORY SECTION IS IN THE MACHINE and its clock runs -- the first rung
# for MemC/MemD/MemX, the same one the processor started on.
.PHONY: mem-test
mem-test:
	@cd ../.. && $(V) --binary --top-module tb_mem $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_mem -o tb_mem \
		verilog/verilator/tb_mem.sv verilog/generated/dorado_mem.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_mem/tb_mem 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_mem|Assertion"; exit $$rc; }

ifufetch-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks
	@mkdir -p obj_ifufetch
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_ifufetch/ifufetch.vec
	@cd ../.. && $(V) --binary --top-module tb_ifufetch $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_ifufetch -o tb_ifufetch \
		verilog/verilator/tb_ifufetch.sv verilog/generated/dorado_ifu.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_ifufetch/tb_ifufetch \
		    +vectors=verilog/verilator/obj_ifufetch/ifufetch.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep -E "tb_ifufetch|Assertion"; exit $$rc; }

ifu-test:
	@cd ../.. && $(V) --binary --top-module tb_ifu $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ifu -o tb_ifu \
		verilog/verilator/tb_ifu.sv verilog/generated/dorado_ifu.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_ifu/tb_ifu 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_ifu|Assertion"; exit $$rc; }

msa-test:
	@cd ../.. && $(V) --binary --top-module tb_msa $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_msa -o tb_msa \
		verilog/verilator/tb_msa.sv verilog/generated/msa.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_msa/tb_msa 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_msa|Assertion"; exit $$rc; }

storage-test:
	@cd ../.. && $(V) --binary --top-module tb_storage $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_storage -o tb_storage \
		verilog/verilator/tb_storage.sv verilog/generated/dorado_storage.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_storage/tb_storage 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_storage|Assertion"; exit $$rc; }


readback-test:
	@cd ../.. && $(V) --binary --top-module tb_readback $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_readback -o tb_readback \
		verilog/verilator/tb_readback.sv verilog/generated/dorado_storage.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_readback/tb_readback 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_readback|Assertion"; exit $$rc; }

muffler-test:
	@cd ../.. && $(V) --binary --top-module tb_muffler $(V_WARN) \
		-Iverilog/cells -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_muf -o tb_muffler \
		verilog/verilator/tb_muffler.sv \
		verilog/generated/dorado_baseboard.v \
		verilog/generated/BaseBd-Rev-Am.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_muf/tb_muffler 2>&1); rc=$$?; echo "$$out" | grep tb_muffler; exit $$rc; }

strap-test:
	@cd ../.. && $(V) --binary --top-module tb_strap $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_strap -o tb_strap \
		verilog/verilator/tb_strap.sv \
		verilog/generated/DispM-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DskEth-Rev-Cf.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_strap/tb_strap 2>&1); rc=$$?; echo "$$out" | grep tb_strap; exit $$rc; }

osc-test:
	@cd ../.. && $(V) --binary --top-module tb_osc $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_osc -o tb_osc \
		verilog/verilator/tb_osc.sv \
		verilog/generated/DispM-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_osc/tb_osc 2>&1); rc=$$?; echo "$$out" | grep tb_osc; exit $$rc; }

task-test:
	@cd ../.. && $(V) --binary --top-module tb_task $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_task -o tb_task \
		verilog/verilator/tb_task.sv verilog/generated/dorado_control.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_task/tb_task 2>&1); rc=$$?; echo "$$out" | grep tb_task; exit $$rc; }

taskrun-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks
	@mkdir -p obj_taskrun
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_taskrun/taskrun.vec
	@cd ../.. && $(V) --binary --top-module tb_taskrun $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_taskrun -o tb_taskrun \
		verilog/verilator/tb_taskrun.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_taskrun/tb_taskrun \
		    +vectors=verilog/verilator/obj_taskrun/taskrun.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_taskrun; exit $$rc; }

refdecode-test:
	@cd ../.. && $(V) --binary --top-module tb_refdecode $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_refdec -o tb_refdecode \
		verilog/verilator/tb_refdecode.sv verilog/generated/MemC-Rev-Be.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_refdec/tb_refdecode 2>&1); rc=$$?; echo "$$out" | grep tb_refdecode; exit $$rc; }

memrun-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks
	@mkdir -p obj_memrun
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_memrun/memrun.vec
	@cd ../.. && $(V) --binary --top-module tb_memrun $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_memrun -o tb_memrun \
		verilog/verilator/tb_memrun.sv verilog/generated/dorado_mem.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_memrun/tb_memrun \
		    +vectors=verilog/verilator/obj_memrun/memrun.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_memrun; exit $$rc; }

baseboard-test:
	@cd ../.. && $(V) --binary --top-module tb_baseboard $(V_WARN) \
		-Iverilog/cells -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_bb -o tb_baseboard \
		verilog/verilator/tb_baseboard.sv \
		verilog/generated/dorado_baseboard.v \
		verilog/generated/BaseBd-Rev-Am.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_bb/tb_baseboard 2>&1); rc=$$?; echo "$$out" | grep tb_baseboard; exit $$rc; }

# Each board lints alone; the backplane top module has to be linted WITH the
# boards it instantiates, because Verilator resolves a module by looking for a
# file of the same name and `ProcH_m_Rev_m_Ce` lives in `ProcH-Rev-Ce.v`.
TOP_V    = $(GEN)/dorado_backplane.v
# The one-board machine tb_baseboard runs, and the two-board Control machine
# tb_mirreg runs. Top modules, not boards, so they are linted with their boards
# rather than alone.
BB_V     = $(GEN)/dorado_baseboard.v
CT_V     = $(GEN)/dorado_control.v
PR_V     = $(GEN)/dorado_proc.v
# The five-board machine tb_firmware runs: BaseBd + ContA + ContB + ProcH + ProcL.
BT_V     = $(GEN)/dorado_boot.v
# The seven-board memory machine tb_mem runs.
IF_V     = $(GEN)/dorado_ifu.v
SM_V     = $(GEN)/dorado_storage.v
MM_V     = $(GEN)/dorado_mem.v

lint:
	@ok=0; bad=0; \
	for f in $(GEN)/*.v; do \
		if [ "$$f" = "$(TOP_V)" ] || [ "$$f" = "$(BB_V)" ] \
		   || [ "$$f" = "$(CT_V)" ] || [ "$$f" = "$(PR_V)" ] \
		   || [ "$$f" = "$(BT_V)" ] || [ "$$f" = "$(MM_V)" ] \
		   || [ "$$f" = "$(SM_V)" ] || [ "$$f" = "$(IF_V)" ]; then continue; fi; \
		if $(V) --lint-only $(V_WARN) -I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $$f >/dev/null 2>&1; then \
			ok=$$((ok+1)); \
		else \
			bad=$$((bad+1)); echo "  FAIL $$(basename $$f)"; \
		fi; \
	done; \
	echo "lint: $$ok board(s) clean, $$bad failed"; \
	if [ -f $(IF_V) ]; then \
		if $(V) --lint-only --top-module dorado_ifu $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(GEN)/*.v >/dev/null 2>&1; then \
			echo "lint: dorado_ifu elaborates -- the IFU is in a machine"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_ifu.v"; \
		fi; \
	fi; \
	if [ -f $(SM_V) ]; then \
		if $(V) --lint-only --top-module dorado_storage $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(GEN)/*.v >/dev/null 2>&1; then \
			echo "lint: dorado_storage elaborates -- the MSA is in a machine"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_storage.v"; \
		fi; \
	fi; \
	if [ -f $(TOP_V) ]; then \
		if $(V) --lint-only --top-module dorado_backplane $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(GEN)/*.v >/dev/null 2>&1; then \
			echo "lint: backplane top module elaborates with its boards"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_backplane.v"; \
		fi; \
	fi; \
	if [ -f $(BB_V) ]; then \
		if $(V) --lint-only --top-module dorado_baseboard_machine $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(BB_V) \
			$(GEN)/BaseBd-Rev-Am.v >/dev/null 2>&1; then \
			echo "lint: one-board BaseBoard machine elaborates"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_baseboard.v"; \
		fi; \
	fi; \
	if [ -f $(CT_V) ]; then \
		if $(V) --lint-only --top-module dorado_control_machine $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(CT_V) \
			$(GEN)/ContA-Rev-Cd.v $(GEN)/ContB-Rev-Cd.v >/dev/null 2>&1; then \
			echo "lint: two-board Control machine elaborates"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_control.v"; \
		fi; \
	fi; \
	if [ -f $(MM_V) ]; then \
		if $(V) --lint-only --top-module dorado_mem_machine $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(MM_V) \
			$(GEN)/ContA-Rev-Cd.v $(GEN)/ContB-Rev-Cd.v \
			$(GEN)/ProcH-Rev-Ce.v $(GEN)/ProcL-Rev-Ci.v \
			$(GEN)/MemC-Rev-Be.v $(GEN)/MemD-Rev-Ca.v \
			$(GEN)/MemX-Rev-Ch.v >/dev/null 2>&1; then \
			echo "lint: seven-board memory machine elaborates"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_mem.v"; \
		fi; \
	fi; \
	if [ -f $(BT_V) ]; then \
		if $(V) --lint-only --top-module dorado_boot_machine $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(BT_V) \
			$(GEN)/BaseBd-Rev-Am.v $(GEN)/ContA-Rev-Cd.v \
			$(GEN)/ContB-Rev-Cd.v $(GEN)/ProcH-Rev-Ce.v \
			$(GEN)/ProcL-Rev-Ci.v >/dev/null 2>&1; then \
			echo "lint: five-board boot machine elaborates"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_boot.v"; \
		fi; \
	fi; \
	if [ -f $(PR_V) ]; then \
		if $(V) --lint-only --top-module dorado_proc_machine $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(PR_V) \
			$(GEN)/ContA-Rev-Cd.v $(GEN)/ContB-Rev-Cd.v \
			$(GEN)/ProcH-Rev-Ce.v $(GEN)/ProcL-Rev-Ci.v >/dev/null 2>&1; then \
			echo "lint: four-board processor machine elaborates"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_proc.v"; \
		fi; \
	fi; \
	test $$bad -eq 0

boards:
	$(MAKE) -C ../.. verilog-boards

run: all
	./obj_dir/Vemu

clean:
	rm -rf obj_dir
