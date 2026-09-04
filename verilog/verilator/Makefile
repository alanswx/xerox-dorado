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
# `msa` is the STORAGE MODULE and carries no -Rev- in its file name, which is
# why a glob over the revisioned boards misses it. Without it in the machine
# there is no memory at all: MemAd/RAS/CAS/WE leave as ports and Sin reads
# whatever the outside drives.
BOARDS  ?= ProcH-Rev-Ce ProcL-Rev-Ci ContA-Rev-Cd ContB-Rev-Cd IFU-Rev-Ch \
           MemC-Rev-Be MemD-Rev-Ca MemX-Rev-Ch DskEth-Rev-Cf DispY-Rev-Cl \
           BaseBd-Rev-Am msa
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
# THE OVERSAMPLING RATIO. Real time on the FPGA needs sys_clk = SYSPER x 16.67
# MHz and measured Fmax is 48.99 MHz, so SYSPER=2 is the target and the default
# 16 runs at 0.18x. Lowering it stresses cell fidelity -- one bug "survived at
# 16x only because the pulse happened to sit inside one stable address" -- so
# these gates are exactly the tool for finding what breaks:
#     make -C verilog/verilator taskrun-test SYSPER=8
# Only the six benches with a WT() wait-scaler honour it; the rest hardcode 16.
SYSPER  ?= 16
V_WARN   = +define+SYSPER_OVERRIDE=$(SYSPER) -Wno-PINMISSING -Wno-DECLFILENAME -Wno-UNDRIVEN -Wno-UNUSEDSIGNAL \
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
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec -o tb_exec \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec/tb_exec \
		    +vectors=verilog/verilator/obj_exec/exec.vec 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# A WHOLE WORLD in IM, preloaded through the derived field map, then run.
# `make exec-world EXECARGS='+cycles=200000 +start=100'`.
.PHONY: exec-world
exec-world:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec -o tb_exec \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec/tb_exec \
		    +preload=verilog/verilator/obj_exec/im.txt $(EXECARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE NINE-BOARD MACHINE running a preloaded world: the memory section, the
# storage array and the IFU as well. Everything but the I/O boards and the
# BaseBoard, which is absent because this bench drives the CP bus itself.
.PHONY: exec-world9
exec-world9:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_exec9
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec9 -o tb_exec9 +define+WORLD \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_world.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec9/tb_exec9 \
		    +preload=verilog/verilator/obj_exec/im.txt $(EXECARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE ALTO WORLD ON TEN BOARDS: AEmu preloaded into IM (no transport), started
# at its own START, with the display board present. The world loads its own
# HRam (INITHRAM/LOADHRAMLOOP are its symbols, not Initial's).
world-display:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_wdisp
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_wdisp -o tb_wdisp +define+WORLD +define+SCREEN \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_screen.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_wdisp/tb_wdisp \
		    +preload=verilog/verilator/obj_exec/im.txt +worldms +frame +mb0 +nostuck $(WORLDARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# INITIAL BOOTS, AND THE BENCH HANDS IT A WORLD instead of a transport: one
# preload carries AEmu, Initial and Bootstrap (disjoint address ranges), Initial
# runs its whole initialisation, and at BootEmulator the machine is redirected
# into AEmu StartEmulator. See +enterworld in tb_exec.sv.
boot-world:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_bworld
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && { ./dorado/build/im_image chm/dorado/AEmu.mb\!2 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null | grep -v '^\#'; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null | grep -v '^\#'; } \
		> verilog/verilator/obj_bworld/im.txt
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_bworld -o tb_bworld +define+WORLD +define+SCREEN \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_screen.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_bworld/tb_bworld \
		    +preload=verilog/verilator/obj_bworld/im.txt +worldms +bootchain +start=3904 +enterat=c92 +enterworld=25b +frame +mb0 +nostuck $(WORLDARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE SAME, ON ELEVEN BOARDS WITH A PACK ON THE DISK CABLE: pass
# WORLDARGS="+pack=chm/diskpacks/games-trident.pack ..." and the Alto world boots
# from it (or tries to).
boot-world-disk:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_bwdisk
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && { ./dorado/build/im_image chm/dorado/AEmu.mb\!2 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null | grep -v '^\#'; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null | grep -v '^\#'; } \
		> verilog/verilator/obj_bwdisk/im.txt
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_bwdisk -o tb_bwdisk +define+WORLD +define+SCREEN +define+FULL \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_full.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_bwdisk/tb_bwdisk \
		    +preload=verilog/verilator/obj_bwdisk/im.txt +worldms +bootchain +start=3904 +enterat=c92 +enterworld=25b +frame +mb0 +nostuck $(WORLDARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE MACHINE RUNS WITH IM PARITY ENABLED. The preloaded array's parity is
# correct; the enables just must not be on during the power-up transient,
# because Stop gates the clock that would clear it.
.PHONY: exec-parity
exec-parity:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_exec9
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec9 -o tb_exec9 +define+WORLD \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_world.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec9/tb_exec9 \
		    +preload=verilog/verilator/obj_exec/im.txt +imparitylate +cycles=400000 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# WITH TASKING ON, the fault task services the stack underflow and the machine
# stops being held. See docs/verilog-handoff.md, "the world's loop".
.PHONY: exec-tasking
exec-tasking:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_exec9
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec9 -o tb_exec9 +define+WORLD \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_world.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec9/tb_exec9 \
		    +preload=verilog/verilator/obj_exec/im.txt +taskingon +cycles=400000 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# DOES ANY VIDEO SIGNAL MOVE? The same machine as `exec-world` plus DispY --
# the monochrome display board -- so the ONLY difference is the display. The
# six signals a monitor is made of are counted as TRANSITIONS, not sampled: a
# level sitting high is not a display running.
.PHONY: exec-screen
exec-screen:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_screen
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_screen \
		-o tb_screen +define+WORLD +define+SCREEN \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_screen.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_screen/tb_screen \
		    $(SCREENARGS) +preload=verilog/verilator/obj_exec/im.txt +taskingon \
		    +cycles=$(or $(CYCLES),400000) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE BOOT SCREEN: the ten-board machine (dorado_world + DispY) running
# PARC's own boot chain -- Initial + Bootstrap through the ReadBB handshake,
# exactly as exec-boot -- with the display board along for the ride and the
# frame capture on. What Initial's INITHRAM loads into the HRam is what the
# monitor gets; dorado_frame.pgm is what it painted. Tasking starts OFF (the
# jam is TaskingOff,Return) so DispY's wakeup cannot preempt Initial before
# TASKINITLOOP has set the task PCs -- the f12b9ae2 failure was the AEmu
# world with +taskingon, not this path.
.PHONY: exec-bootscreen
exec-bootscreen:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_screen
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && { ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null \
		| grep -v '^\#'; } > verilog/verilator/obj_exec/boot.txt
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_screen \
		-o tb_screen +define+WORLD +define+SCREEN \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_screen.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_screen/tb_screen \
		    $(SCREENARGS) +fastwait +bbword=0000 +bbfeed +frame +mb0 \
		    +preload=verilog/verilator/obj_exec/boot.txt +bootchain +start=3904 \
		    +cycles=$(or $(CYCLES),120000000) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# ELEVEN BOARDS: the screen machine plus DskEth, so Initial's Ethernet boot
# runs against the real controller (its cable idle).
.PHONY: exec-bootfull
exec-bootfull:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_full
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && { ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null \
		| grep -v '^\#'; } > verilog/verilator/obj_exec/boot.txt
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_full \
		-o tb_full +define+WORLD +define+SCREEN +define+FULL \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_full.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_full/tb_full \
		    $(SCREENARGS) +fastwait +bbword=0000 +bbfeed +frame +mb0 \
		    +preload=verilog/verilator/obj_exec/boot.txt +bootchain +start=3904 \
		    +cycles=$(or $(CYCLES),120000000) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE FAULT TASK RUNS ITS OWN HANDLER: arm MemX's fault counter and preset
# TPC[15] to AEmu's FAULTTASK, and task 15 executes the fault-handler region
# instead of emulator code. See docs/verilog-handoff.md.
.PHONY: exec-init
exec-init:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_exec9
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec9 -o tb_exec9 +define+WORLD \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_world.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec9/tb_exec9 \
		    +preload=verilog/verilator/obj_exec/im.txt +taskingon +faultinit +tpcinit +cycles=400000 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# PARC'S OWN BOOT CHAIN: Initial's entry global-calls Bootstrap's READBB, which
# waits for the control processor to feed it. See docs/verilog-handoff.md.
.PHONY: exec-boot
exec-boot:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_exec9
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 \
		> verilog/verilator/obj_exec/exec.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_exec/im.txt 2>/dev/null
	@# PARC's boot chain: Initial (0xc00-0xfbf) and Bootstrap (0xfc0-0xfff),
	@# adjacent and disjoint, so they coexist exactly as the machine expects.
	@cd ../.. && { ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null \
		| grep -v '^\#'; } > verilog/verilator/obj_exec/boot.txt
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_exec9 -o tb_exec9 +define+WORLD \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_world.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_exec9/tb_exec9 \
		    $(EXECARGS) +preload=verilog/verilator/obj_exec/boot.txt +bootchain +start=3904 +cycles=400000 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# PARC's BLOCK LOADER: a 17-byte hunk walked into IM, read back word for word.
.PHONY: boot0-test
boot0-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_boot0
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 16 \
		> verilog/verilator/obj_boot0/boot0.vec
	@# ...and four hunks from ABOVE 2048. The bank is {addr[11], addr[0]}, so a
	@# vector set confined to low addresses leaves addr[11] always 0 and exercises
	@# only half the interleave -- a swapped bank ordering would go unseen there.
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 0x800 4 \
		>> verilog/verilator/obj_boot0/boot0.vec
	@cd ../.. && ./dorado/build/im_image chm/dorado/AEmu.mb\!2 \
		> verilog/verilator/obj_boot0/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_boot0 $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_boot0 -o tb_boot0 \
		verilog/verilator/tb_boot0.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_boot0/tb_boot0 $(BOOT0ARGS) \
		    +vectors=verilog/verilator/obj_boot0/boot0.vec \
		    +imimage=verilog/verilator/obj_boot0/im.txt 2>&1); rc=$$?; \
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


display-test:
	@cd ../.. && $(V) --binary --top-module tb_display $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_display -o tb_display \
		verilog/verilator/tb_display.sv verilog/generated/dorado_display.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DispY-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_display/tb_display +slowio 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_display|Assertion"; exit $$rc; }


disk-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_disk -o tb_disk \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_disk/tb_disk +slowio 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# THE SAME LOOP, ONE ADDRESS HIGHER: DISKTAG (014B) instead of DISKCONTROL.
# One decoder output asserting proves little next to the other four going
# quiet when the address changes, in a machine that is running.
disk-tag-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_disktag -o tb_disktag \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_disktag/tb_disktag +slowio +tag 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# The same TIOA/Output loop aimed at EControl (016B) with TurnOnTx: does TxOn
# latch, and does the EOT wakeup reach TWReq.06?
eth-ctl-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethctl -o tb_ethctl \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_ethctl/tb_ethctl +slowio +eth 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# cell_F9401 against a published CRC-16 check value (0xFEE8 for "123456789").
crc-test:
	@cd ../.. && $(V) --binary --top-module tb_crc $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_crc -o tb_crc \
		verilog/verilator/tb_crc.sv verilog/cells/cell_F9401.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_crc/tb_crc 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_crc|Assertion"; exit $$rc; }

# The pack at the end of the cable: dorado_trident turning, dorado_pack serving
# one short sector of games-trident.pack, decoded and compared with the file.
pack-test:
	@cd ../.. && $(V) --binary --top-module tb_pack $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_pack -o tb_pack \
		verilog/verilator/tb_pack.sv verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_pack/tb_pack +pack=chm/diskpacks/games-trident.pack 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_pack|Assertion|%Error"; exit $$rc; }

# The RTL's boot server: build the MicrocodeBootReply stream for a .eb and
# round-trip every packet through the Manchester decoder, checking that the
# CRC-16 residue of data+check word is zero (the 9401's own criterion).
.PHONY: ebreplies-check
ebreplies-check:
	@cd ../.. && python3 tools/decode_xmt.py --selftest && \
	 python3 tools/eb_replies.py dorado/worlds/aemu.eb $${TMPDIR:-/tmp}/eb_replies.txt --selftest

# THE FIRST WAKEUP THIS BENCH CAN MOVE: a DISKMUFF write carrying IOB.04
# clears IndexTW, which is otherwise asserted for every sample of every run.
disk-muff-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_diskmuff -o tb_diskmuff \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_diskmuff/tb_diskmuff +slowio +muff 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# A REAL DISK COMMAND: DISKCONTROL 0x02C0 = SetDebugMode + a Read op, which
# loads ReadBlock and makes the controller Active.
disk-cmd-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_diskcmd -o tb_diskcmd \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_diskcmd/tb_diskcmd +slowio +read +tlit 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# THE FORMAT RAM: a DISKRAM write steps b21, the format-RAM address counter.
disk-ram-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_diskram -o tb_diskram \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_diskram/tb_diskram +slowio +ram +tdata 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# PARC'S OWN FORMAT PROGRAM in PARC's own format RAM: all sixteen words of
# the Alto Diablo emulation format (HM p.98), jammed one per run and read back.
disk-format-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_diskfmt -o tb_diskfmt \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_diskfmt/tb_diskfmt +slowio +ram16 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

# THE OTHER DIRECTION: a Pd<-Input in the loop, so the BOARD drives IOB back.
disk-input-test:
	@cd ../.. && $(V) --binary --top-module tb_disk $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_diskinput -o tb_diskinput \
		verilog/verilator/tb_disk.sv verilog/generated/dorado_disk.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/DskEth-Rev-*.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_diskinput/tb_diskinput +slowio +input 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_disk|Assertion"; exit $$rc; }

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
# The multi-board machine tops the exec/screen/disk/display benches run. Like
# the others they are tops, not boards, so they lint WITH their boards, not
# alone -- these four were missing from the skip list, which reported them as
# spurious standalone-lint failures ("4 failed") though they elaborate clean
# with boards.
WD_V     = $(GEN)/dorado_world.v
SC_V     = $(GEN)/dorado_screen.v
DK_V     = $(GEN)/dorado_disk.v
DP_V     = $(GEN)/dorado_display.v
FL_V     = $(GEN)/dorado_full.v

lint:
	@ok=0; bad=0; \
	for f in $(GEN)/*.v; do \
		if [ "$$f" = "$(TOP_V)" ] || [ "$$f" = "$(BB_V)" ] \
		   || [ "$$f" = "$(CT_V)" ] || [ "$$f" = "$(PR_V)" ] \
		   || [ "$$f" = "$(BT_V)" ] || [ "$$f" = "$(MM_V)" ] \
		   || [ "$$f" = "$(SM_V)" ] || [ "$$f" = "$(IF_V)" ] \
		   || [ "$$f" = "$(WD_V)" ] || [ "$$f" = "$(SC_V)" ] \
		   || [ "$$f" = "$(DK_V)" ] || [ "$$f" = "$(DP_V)" ] \
		   || [ "$$f" = "$(FL_V)" ]; then continue; fi; \
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
	for wt in dorado_world:$(WD_V) dorado_screen:$(SC_V) \
	          dorado_disk:$(DK_V) dorado_display:$(DP_V); do \
		top=$${wt%%:*}; wf=$${wt##*:}; \
		if [ -f $$wf ]; then \
			if $(V) --lint-only --top-module $$top $(V_WARN) \
				-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(GEN)/*.v >/dev/null 2>&1; then \
				echo "lint: $$top elaborates with its boards"; \
			else \
				bad=$$((bad+1)); echo "  FAIL $$top.v"; \
			fi; \
		fi; \
	done; \
	test $$bad -eq 0

boards:
	$(MAKE) -C ../.. verilog-boards

run: all
	./obj_dir/Vemu

clean:
	rm -rf obj_dir

# THE TERMINAL MICROCOMPUTER: HM Table 24 framing, the manual's own priority
# (a keyboard transition outranks a mouse move) and the excess-200B mouse
# encoding. The module is the RTL half of what dorado/src/display.c models.
.PHONY: term-test
term-test:
	@cd ../.. && $(V) --binary --top-module tb_term $(V_WARN) \
		--Mdir verilog/verilator/obj_term -o tb_term \
		verilog/verilator/tb_term.sv verilog/cells/dorado_terminal.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_term/tb_term 2>&1); \
		  rc=$$?; echo "$$out" | grep tb_term; exit $$rc; }

# A TRIDENT AT THE END OF THE CABLE: presence (Ready+OnLine+TERM together) and
# rotation at 3600 RPM. The gaps are counted as well as the pulses -- a line
# stuck low gives a healthy pulse count and only the duty cycle tells them
# apart, which is the lesson disk-input-test paid for.
.PHONY: trident-test
trident-test:
	@cd ../.. && $(V) --binary --top-module tb_trident $(V_WARN) \
		--Mdir verilog/verilator/obj_trident -o tb_trident \
		verilog/verilator/tb_trident.sv verilog/cells/dorado_trident.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_trident/tb_trident 2>&1); \
		  rc=$$?; echo "$$out" | grep tb_trident; exit $$rc; }
