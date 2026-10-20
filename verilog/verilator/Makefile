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
# Fabric samples per 30 ns clock; two clocks execute one microinstruction.
# Instructions/second = sys_clk / (2*SYSPER). Lowering SYSPER stresses cell
# fidelity -- one bug "survived at
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

# The hardware HPS medium must serialize the same header, label, data and
# Fire code as the qualified file-backed medium, and preserve neighboring
# disk bytes when flushing an unaligned track. The host image is read-only;
# the test writes an in-memory copy only.
.PHONY: hps-persistence-test
hps-persistence-test:
	@cd ../.. && $(V) --cc --exe --build --top-module tb_hps_persistence \
		$(V_WARN) --Mdir verilog/verilator/obj_hps_persistence -CFLAGS '-O2 -std=c++20' -o hps-persistence-test \
		verilog/verilator/tb_hps_persistence.sv verilog/verilator/hps_persistence_test.cpp \
		fpga/mister/dorado_pack_hps.sv verilog/verilator/dorado_pack.sv >/dev/null
	@cd ../.. && verilog/verilator/obj_hps_persistence/hps-persistence-test \
		chm/diskpacks/games-trident.pack build/rtl-executive/hps-persistence

.PHONY: pilot-pack-test
pilot-pack-test:
	@cd ../.. && python3 tools/pdi_to_trident.py CedarDisk/CedarDorado-work.pdi.gz \
		build/rtl-executive/pilot-pack-fixture.pack && \
		$(V) --cc --exe --build -j 4 --top-module tb_pilot_pack $(V_WARN) \
		--Mdir verilog/verilator/obj_pilot_pack -o pilot-pack-test -CFLAGS '-O2 -std=c++20' \
		verilog/verilator/tb_pilot_pack.sv verilog/verilator/pilot_pack_test.cpp \
		fpga/mister/dorado_pack_hps.sv >/dev/null && \
		./verilog/verilator/obj_pilot_pack/pilot-pack-test build/rtl-executive/pilot-pack-fixture.pack

.PHONY: hps-pack-test
hps-pack-test:
	@cd ../.. && $(V) --cc --exe --build --timing --top-module tb_hps_pack \
		$(V_WARN) --Mdir verilog/verilator/obj_hps_pack -CFLAGS '-O2 -std=c++20' -o hps-pack-test \
		verilog/verilator/tb_hps_pack.sv verilog/verilator/hps_pack_test.cpp \
		fpga/mister/dorado_pack_hps.sv verilog/verilator/dorado_pack.sv >/dev/null && \
		{ out=$$(verilog/verilator/obj_hps_pack/hps-pack-test chm/diskpacks/games-trident.pack \
			+pack=chm/diskpacks/games-trident.pack 2>&1); rc=$$?; \
		  echo "$$out" | grep HPS-PACK; exit $$rc; }

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

# ProcH's physical overflow detector and ContA's signed-jump condition.
# Old d13 polarity fails even the equal-operands/no-overflow case.
.PHONY: signed-compare-test
signed-compare-test:
	@cd ../.. && $(V) --binary --top-module tb_signed_compare $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_signed_compare -o tb_signed_compare \
		verilog/verilator/tb_signed_compare.sv verilog/generated/ProcH-Rev-Ce.v \
		>/dev/null && ./verilog/verilator/obj_signed_compare/tb_signed_compare

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
# into AEmu InitMap (real 1076B = 23e), not StartEmulator (25b), which
# skips the world's required initialization. Verilator takes the FIRST
# matching plusarg, so do not prepend a default when WORLDARGS supplies it.
WORLD_HANDOFF_ARGS = $(if $(filter +enterat=%,$(WORLDARGS)),,+enterat=c92) $(if $(filter +enterworld=%,$(WORLDARGS)),,+enterworld=23e)
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
		    +preload=verilog/verilator/obj_bworld/im.txt +worldms +bootchain +start=3904 $(WORLD_HANDOFF_ARGS) +frame +mb0 +nostuck +terminal $(WORLDARGS) 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# THE SAME, ON ELEVEN BOARDS WITH A PACK ON THE DISK CABLE: pass
# WORLDARGS="+pack=chm/diskpacks/games-trident.pack ..." and the Alto world boots
# from it (or tries to).
# Bounded Initial.mb regression: ResetTags used to lose both completion
# writes because MemC stored parity but compared it as VA[4]. No disk I/O
# is reached here; the full map walk and OS boot are deliberately separate.
.PHONY: clock-domains-test
clock-domains-test:
	@$(MAKE) -s boot-world-disk WORLDARGS='+cycles=100000 +hasram +ioreset +fastwait +bbword=0000 +bbfeed +check-clock-domains'

.PHONY: reset-tags-test
reset-tags-test:
	@$(MAKE) -s boot-world-disk WORLDARGS='+cycles=800000 +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +check-reset-tags'

# Read the storage configuration through AEmu's actual GetMemConfig code,
# after Initial has initialized the processor, without the long map walk.
.PHONY: memory-config-test
memory-config-test:
	@$(MAKE) -s boot-world-disk WORLDARGS='+cycles=300000 +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +enterat=c64 +enterworld=200 +check-memconfig'

# Memory muffler shift registers must retain write data at bus turnaround.
.PHONY: memory-dmux-test
memory-dmux-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_dmux $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_memory_dmux -o tb_memory_dmux \
		verilog/verilator/tb_memory_dmux.sv \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_memory_dmux/tb_memory_dmux

.PHONY: map-buffer-hold-test
map-buffer-hold-test:
	@cd ../.. && $(V) --binary --top-module tb_map_buffer_hold $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_map_buffer_hold -o tb_map_buffer_hold \
		verilog/verilator/tb_map_buffer_hold.sv verilog/generated/MemX-Rev-Ch.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_map_buffer_hold/tb_map_buffer_hold

# Captured Cedar module3-to0 handoff, with the original MSA serializers.
.PHONY: storage-module-switch-test
storage-module-switch-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_module_switch $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_storage_module_switch -o module-switch-test \
		verilog/verilator/tb_storage_module_switch.sv \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_storage_module_switch/module-switch-test

# Real AltoDiabloDisk.mc initialization, not the old per-word jam bench.
.PHONY: disk-init-format-test
disk-init-format-test:
	@$(MAKE) -s boot-world-disk WORLDARGS='+cycles=350000 +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +enterat=c64 +enterworld=601 +check-disk-format'

# PostClock0 must keep a new task's TIOA out of the old Output window.
.PHONY: disk-tioa-phase-test
disk-tioa-phase-test:
	@$(MAKE) -s boot-world-disk WORLDARGS='+cycles=350000 +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +enterat=c64 +enterworld=601 +check-disk-format +check-tioa-phase'

.PHONY: parity170-test
parity170-test:
	@cd ../.. && $(V) --binary --top-module tb_parity170 $(V_WARN) \
		--Mdir verilog/verilator/obj_parity170 -o tb_parity170 \
		verilog/verilator/tb_parity170.sv verilog/cells/cell_MC10170.v >/dev/null
	@./obj_parity170/tb_parity170

.PHONY: cache-config-test
cache-config-test:
	@cd ../.. && $(V) --binary --top-module tb_cache_config $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_cache_config -o tb_cache_config \
		verilog/verilator/tb_cache_config.sv verilog/generated/MemC-Rev-Be.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_cache_config/tb_cache_config

.PHONY: cache-word-address-capture-test
cache-word-address-capture-test:
	@cd ../.. && $(V) --binary --top-module tb_cache_word_address_capture $(V_WARN) \
		-Iverilog/cells -Iverilog/generated \
		--Mdir verilog/verilator/obj_cache_word_address -o tb_cache_word_address_capture \
		verilog/verilator/tb_cache_word_address_capture.sv verilog/generated/MemD-Rev-Ca.v \
		>/dev/null && ./verilog/verilator/obj_cache_word_address/tb_cache_word_address_capture

.PHONY: memory-write-clock-test
memory-write-clock-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_write_clock $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_memory_write_clock -o tb_memory_write_clock \
		verilog/verilator/tb_memory_write_clock.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_memory_write_clock/tb_memory_write_clock

.PHONY: memory-address-bus-test
memory-address-bus-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_address_bus $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_memory_address_bus -o tb_memory_address_bus \
		verilog/verilator/tb_memory_address_bus.sv verilog/generated/MemC-Rev-Be.v \
		>/dev/null && ./verilog/verilator/obj_memory_address_bus/tb_memory_address_bus

.PHONY: dram-write-test
dram-write-test:
	@cd ../.. && $(V) --binary --top-module tb_dram_write $(V_WARN) \
		--Mdir verilog/verilator/obj_dram_write -o tb_dram_write \
		verilog/verilator/tb_dram_write.sv verilog/cells/cell_MK4096P_6.v \
		verilog/cells/cell_MK4096P_addr.v verilog/cells/cell_MosRam.v >/dev/null && \
		./verilog/verilator/obj_dram_write/tb_dram_write

.PHONY: cache-input-capture-test
cache-input-capture-test:
	@cd ../.. && $(V) --binary --top-module tb_cache_input_capture $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_cache_input_capture -o tb_cache_input_capture \
		verilog/verilator/tb_cache_input_capture.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_cache_input_capture/tb_cache_input_capture

.PHONY: md-capture-test
md-capture-test:
	@cd ../.. && $(V) --binary --top-module tb_md_capture $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_md_capture -o tb_md_capture \
		verilog/verilator/tb_md_capture.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_md_capture/tb_md_capture

.PHONY: memory-ecc-classify-test
memory-ecc-classify-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_ecc_classify $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_memory_ecc_classify -o tb_memory_ecc_classify \
		verilog/verilator/tb_memory_ecc_classify.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_memory_ecc_classify/tb_memory_ecc_classify

.PHONY: memory-byte-parity-test
memory-byte-parity-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_byte_parity $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_memory_byte_parity -o tb_memory_byte_parity \
		verilog/verilator/tb_memory_byte_parity.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_memory_byte_parity/tb_memory_byte_parity

.PHONY: ifu-f-clock-test
ifu-f-clock-test:
	@cd ../.. && $(V) --binary --top-module tb_ifu_f_clock $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ifu_f_clock -o tb_ifu_f_clock \
		verilog/verilator/tb_ifu_f_clock.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ifu_f_clock/tb_ifu_f_clock

.PHONY: ifu-f-capture-test
ifu-f-capture-test:
	@cd ../.. && $(V) --binary --top-module tb_ifu_f_capture $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ifu_f_capture -o tb_ifu_f_capture \
		verilog/verilator/tb_ifu_f_capture.sv verilog/generated/MemD-Rev-Ca.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ifu_f_capture/tb_ifu_f_capture

.PHONY: ifu-pcf-capture-test
ifu-pcf-capture-test:
	@$(MAKE) exec-world9 EXECARGS='+start=0 +cycles=5000 +nostuck +check-pcf-setup'

.PHONY: disk-input-hold-test
disk-input-hold-test:
	@cd ../.. && $(V) --binary --top-module tb_disk_input_hold $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_disk_input_hold -o tb_disk_input_hold \
		verilog/verilator/tb_disk_input_hold.sv verilog/generated/DskEth-Rev-Cf.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_disk_input_hold/tb_disk_input_hold

.PHONY: ethernet-input-hold-test ethernet-receive-sequence-test
ethernet-input-hold-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_input_hold $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_input_hold -o input-hold \
		verilog/verilator/tb_ethernet_input_hold.sv verilog/generated/DskEth-Rev-Cf.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ethernet_input_hold/input-hold

ethernet-receive-sequence-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_receive_sequence $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_receive_sequence -o receive-sequence \
		verilog/verilator/tb_ethernet_receive_sequence.sv verilog/generated/DskEth-Rev-Cf.v fpga/mister/dorado_ethernet_wire.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ethernet_receive_sequence/receive-sequence

.PHONY: ethernet-fifo-parity-test
ethernet-fifo-parity-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_fifo_parity $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_fifo_parity -o tb_ethernet_fifo_parity \
		verilog/verilator/tb_ethernet_fifo_parity.sv verilog/generated/DskEth-Rev-Cf.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ethernet_fifo_parity/tb_ethernet_fifo_parity

.PHONY: disk-fifo-parity-test
disk-fifo-parity-test:
	@cd ../.. && $(V) --binary --top-module tb_disk_fifo_parity $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_disk_fifo_parity -o tb_disk_fifo_parity \
		verilog/verilator/tb_disk_fifo_parity.sv verilog/generated/DskEth-Rev-Cf.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_disk_fifo_parity/tb_disk_fifo_parity

.PHONY: disk-stream-build disk-stream-test disk-writeback-test sector-counter-test
disk-stream-build:
	@cd ../.. && $(V) --binary --top-module tb_disk_stream $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_disk_stream -o tb_disk_stream \
		verilog/verilator/tb_disk_stream.sv verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v >/dev/null
disk-stream-test: disk-stream-build
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack +streamsector=1
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack +streamsector=28
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack +pollselect +fastio

disk-writeback-test: disk-stream-build
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack +writeback
	@cd ../.. && ./verilog/verilator/obj_disk_stream/tb_disk_stream +pack=chm/diskpacks/games-trident.pack +checkedwrite

sector-counter-test:
	@cd ../.. && $(V) --binary --top-module tb_sector_counter $(V_WARN) \
		--Mdir verilog/verilator/obj_sector_counter -o tb_sector_counter \
		verilog/verilator/tb_sector_counter.sv verilog/cells/cell_SN74123.v \
		verilog/cells/cell_SN74LS169.v >/dev/null
	@./obj_sector_counter/tb_sector_counter

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
		>/dev/null && ./verilog/verilator/obj_bwdisk/tb_bwdisk \
		    +preload=verilog/verilator/obj_bwdisk/im.txt +worldms +bootchain +start=3904 $(WORLD_HANDOFF_ARGS) +frame +mb0 +nostuck +terminal $(WORLDARGS)

# A standalone SDL front end to the full RTL machine. Its keyboard follows
# the same PS/2 -> terminal wire path as MiSTer; its screen reads actual
# DRAM/cache through the guest's Alto display list. No C CPU is linked.
EXECUTIVE_SYSPER ?= 4
EXECUTIVE_ARGS ?=
EXECUTIVE_SNAPSHOT ?= build/rtl-executive/alto-executive.rtl
EXECUTIVE_BOOT_SAMPLES ?= 390000000
EXECUTIVE_FPGA_DIR ?= build/rtl-executive/fpga_checkpoint_hps
EXECUTIVE_SDL_CFLAGS = $(shell sdl2-config --cflags 2>/dev/null)
EXECUTIVE_SDL_LIBS = $(shell sdl2-config --libs 2>/dev/null)
.PHONY: executive-sim run-executive executive-checkpoint run-executive-ready
executive-sim:
	@test -n "$(EXECUTIVE_SDL_LIBS)" || { echo "SDL2 is required (sdl2-config missing)"; exit 1; }
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_exec obj_bwdisk obj_executive
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks chm/dorado/AEmu.mb\!2 -1 4 > verilog/verilator/obj_exec/exec.vec
	@cd ../.. && { ./dorado/build/im_image chm/dorado/AEmu.mb\!2 2>/dev/null; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' 2>/dev/null | grep -v '^\#'; \
		./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 2>/dev/null | grep -v '^\#'; } > verilog/verilator/obj_bwdisk/im.txt
	@cd ../.. && $(V) --cc --timing --exe --top-module tb_exec \
		$(filter-out +define+SYSPER_OVERRIDE=%,$(V_WARN)) +define+SYSPER_OVERRIDE=$(EXECUTIVE_SYSPER) \
		+define+DORADO_STORAGE_INPUT_DELAY=2 +define+WORLD +define+SCREEN +define+FULL \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_executive -o dorado-executive \
		-CFLAGS '-O3 -std=c++20 -DVL_TIME_CONTEXT -I$(abspath obj_executive) $(EXECUTIVE_SDL_CFLAGS)' \
		-LDFLAGS '$(EXECUTIVE_SDL_LIBS)' \
		verilog/verilator/tb_exec.sv verilog/verilator/dorado_exec.cpp verilog/generated/dorado_full.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v
	@cd ../.. && python3 tools/sil_rtl_memory_view.py --obj-dir verilog/verilator/obj_executive -o verilog/verilator/obj_executive/rtl_memory_view.h
	@cd ../.. && python3 tools/sil_verilator_state.py --obj-dir verilog/verilator/obj_executive -o verilog/verilator/obj_executive/rtl_state.h
	@$(MAKE) -s -C obj_executive -f Vtb_exec.mk -j4

run-executive: executive-sim
	@cd ../.. && verilog/verilator/obj_executive/dorado-executive $(EXECUTIVE_ARGS)

# A real RTL cold boot is slow in Verilator. Preserve it for subsequent
# interactive runs; this is an exact-model snapshot, including disk writes.
executive-checkpoint: executive-sim
	@$(MAKE) -s -C ../../dorado ../chm/diskpacks/games-trident.pack
	@cd ../.. && mkdir -p "$$(dirname '$(EXECUTIVE_SNAPSHOT)')"
	@cd ../.. && verilog/verilator/obj_executive/dorado-executive --headless \
		--stop-at $(EXECUTIVE_BOOT_SAMPLES) --save-state '$(EXECUTIVE_SNAPSHOT)' $(EXECUTIVE_ARGS)

run-executive-ready:
	@test -x obj_executive/dorado-executive || $(MAKE) executive-sim
	@cd ../.. && test -f '$(EXECUTIVE_SNAPSHOT)' || { \
		echo 'Create the RTL checkpoint with make -C verilog/verilator executive-checkpoint'; exit 1; }
	@cd ../.. && verilog/verilator/obj_executive/dorado-executive \
		--restore '$(EXECUTIVE_SNAPSHOT)' $(EXECUTIVE_ARGS)

# Export the EXISTING matching model/checkpoint. Deliberately do not rebuild
# executive-sim here: regenerating its C++ changes the checkpoint fingerprint.
.PHONY: executive-mister-export
executive-mister-export:
	@cd ../.. && python3 tools/sil_fpga_checkpoint.py \
		--checkpoint '$(EXECUTIVE_SNAPSHOT)' --obj-dir verilog/verilator/obj_executive \
		--storage-words 65536 --hps-pack --base-pack chm/diskpacks/games-trident.pack \
		--output '$(EXECUTIVE_FPGA_DIR)'

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
		    +preload=verilog/verilator/obj_exec/im.txt +imparityon +keepmirdebug +cycles=400000 2>&1); rc=$$?; \
		  echo "$$out" | grep tb_exec; exit $$rc; }

# The native core uses SYSPER=4. Verify original Bootstrap words loaded
# through CP, then execute with IM parity enabled from the first instruction.
.PHONY: bootstrap-parity-test
bootstrap-parity-test:
	@$(MAKE) -s -C ../../dorado build/boot0_hunks build/im_image
	@mkdir -p obj_bootstrap_parity
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/boot0_hunks 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' 0xfc0 16 > verilog/verilator/obj_bootstrap_parity/boot.vec
	@cd ../.. && ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb' > verilog/verilator/obj_bootstrap_parity/im.txt 2>/dev/null
	@cd ../.. && $(V) --binary --top-module tb_exec \
		$(filter-out +define+SYSPER_OVERRIDE=%,$(V_WARN)) +define+SYSPER_OVERRIDE=4 \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_bootstrap_parity -o bootstrap-parity \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_proc.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v >/dev/null
	@cd ../.. && python3 verilog/verilator/test_bootstrap_parity.py verilog/verilator/obj_bootstrap_parity/bootstrap-parity verilog/verilator/obj_bootstrap_parity

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
# TASKINITLOOP has set the task PCs -- the cc30e6c5 failure was the AEmu
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
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_step -o tb_step \
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
.PHONY: baseboard-manifold-test
baseboard-manifold-test:
	@cd ../.. && $(V) --binary --top-module tb_baseboard_manifold $(V_WARN) \
		-Iverilog/cells -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_baseboard_manifold -o baseboard-manifold-test \
		verilog/verilator/tb_baseboard_manifold.sv verilog/generated/BaseBd-Rev-Am.v \
		>/dev/null && ./verilog/verilator/obj_baseboard_manifold/baseboard-manifold-test

# Original ROM Initial component gate. Entry F43 is its checksum-cleared
# debug entry, not a native BaseBoard boot. No world handoff or guest patch.
# CPU manifold ownership is essential: retaining the loading fixture masks
# Initial's enable-all-errors and the real native BootEmulator failures.
.PHONY: riot-clock-test
riot-clock-test:
	@cd ../.. && $(V) --binary --top-module tb_riot_clock $(V_WARN) \
		--Mdir verilog/verilator/obj_riot_clock -o riot-clock-test \
		verilog/verilator/tb_riot_clock.sv verilog/cells/cell_MCS6532.v \
		verilog/vendor/riot/M6532.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_riot_clock/riot-clock-test

.PHONY: initial-rom-test
INITIAL_ROM_CYCLES ?= 16000000
INITIAL_ROM_ARGS ?=
initial-rom-test:
	@$(MAKE) -s -C ../../dorado build/baseboard_rom_dump build/im_image build/boot0_hunks
	@mkdir -p obj_initial_rom obj_exec
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/baseboard_rom_dump 'chm/dorado/doradobaserom.mb!13' > verilog/verilator/obj_initial_rom/rom.txt
	@cd ../.. && ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' > verilog/verilator/obj_initial_rom/alu.txt 2>/dev/null
	@cd ../.. && python3 tools/baseboard_rom_im.py verilog/verilator/obj_initial_rom/rom.txt --alu-fixture verilog/verilator/obj_initial_rom/alu.txt > verilog/verilator/obj_initial_rom/im.txt
	@cd ../.. && ./dorado/build/boot0_hunks 'chm/dorado/AEmu.mb!2' -1 4 > verilog/verilator/obj_exec/exec.vec
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_initial_rom -o tb_initial \
		+define+WORLD +define+SCREEN +define+FULL \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_full.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_initial_rom/tb_initial \
		+preload=verilog/verilator/obj_initial_rom/im.txt +start=3907 \
		+cycles=$$(($(INITIAL_ROM_CYCLES) * $(SYSPER) / 4)) +mb0 +nostuck +terminal \
		+pendulum +hasram +ioreset +fastwait +imparityon +keepmirdebug \
		+cpu-manifold +paritysources +initial-rom-check $(INITIAL_ROM_ARGS)

.PHONY: initial-network-test
# Match the FPGA's four storage slots and the BaseBoard clock waveform.
# Keep this fixture separate from obj_initial_rom and the Executive model.
.PHONY: initial-rom-fourmodule-test
INITIAL_FOUR_CYCLES ?= 16000000
INITIAL_FOUR_ARGS ?=
initial-rom-fourmodule-test:
	@$(MAKE) -s -C ../../dorado build/baseboard_rom_dump build/im_image build/boot0_hunks
	@mkdir -p obj_initial_four obj_exec
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && python3 tools/sil_backplane.py --emit verilog/verilator/obj_initial_four/dorado_full.v --module=dorado_full \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,msa,msaOdd,msa1,msa1Odd,msa2,msa2Odd,msa3,msa3Odd,IFU,DispY,DskEth >/dev/null
	@cd ../.. && ./dorado/build/baseboard_rom_dump 'chm/dorado/doradobaserom.mb!13' > verilog/verilator/obj_initial_four/rom.txt
	@cd ../.. && ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' > verilog/verilator/obj_initial_four/alu.txt 2>/dev/null
	@cd ../.. && python3 tools/baseboard_rom_im.py verilog/verilator/obj_initial_four/rom.txt --alu-fixture verilog/verilator/obj_initial_four/alu.txt > verilog/verilator/obj_initial_four/im.txt
	@cd ../.. && ./dorado/build/boot0_hunks 'chm/dorado/AEmu.mb!2' -1 4 > verilog/verilator/obj_exec/exec.vec
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_initial_four -o tb_initial \
		+define+WORLD +define+SCREEN +define+FULL +define+PILOT_PACK \
		+define+BASEBOARD_MANIFOLD +define+FOUR_STORAGE_MODULES +define+DORADO_STORAGE_INPUT_DELAY=2 \
		verilog/verilator/tb_exec.sv verilog/verilator/obj_initial_four/dorado_full.v \
		verilog/generated/BaseBd-Rev-Am.v verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_initial_four/tb_initial \
		+preload=verilog/verilator/obj_initial_four/im.txt +start=3907 \
		+cycles=$$(($(INITIAL_FOUR_CYCLES) * $(SYSPER) / 4)) +mb0 +nostuck +terminal \
		+pendulum +hasram +ioreset +fastwait +imparityon +keepmirdebug \
		+cpu-manifold +paritysources +initial-rom-check +native-clock $(INITIAL_FOUR_ARGS)

initial-network-test:
	@$(MAKE) initial-rom-test INITIAL_ROM_CYCLES=31000000 \
		INITIAL_ROM_ARGS='+terminal-key=21 +etherhost=34 +largeformat +xmtrec=verilog/verilator/obj_initial_rom/request.txt'
	@cd ../.. && python3 tools/check_initial_request.py \
		verilog/verilator/obj_initial_rom/request.txt --sysper $(SYSPER)

.PHONY: initial-wire-test
initial-wire-test:
	@$(MAKE) initial-network-test SYSPER=4
	@cd ../.. && $(V) --binary --top-module tb_initial_wire $(V_WARN) \
		--Mdir verilog/verilator/obj_initial_wire -o initial-wire-test \
		verilog/verilator/tb_initial_wire.sv fpga/mister/dorado_ethernet_wire.sv >/dev/null && \
		./verilog/verilator/obj_initial_wire/initial-wire-test \
		+recording=verilog/verilator/obj_initial_rom/request.txt

# The native machine includes BaseBd, which also observes the TX cable.
# Exercise the complete generated wrapper rather than the smaller full top.
.PHONY: native-cable-test
native-cable-test:
	@cd ../.. && python3 tools/sil_fpga_native.py --pilot-disk \
		-o build/rtl-executive/native-cable-test >/dev/null && \
		$(V) --binary --top-module tb_native_cable $(V_WARN) \
		-Iverilog/cells -Iverilog/generated -Iverilog/vendor/6502 -Iverilog/vendor/riot \
		--Mdir verilog/verilator/obj_native_cable -o native-cable-test \
		-f build/rtl-executive/native-cable-test/sources.f \
		verilog/verilator/tb_native_cable.sv >/dev/null && \
		./verilog/verilator/obj_native_cable/native-cable-test

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

.PHONY: ethernet-host-test
ethernet-host-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_host $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_host -o ethernet-host-test \
		verilog/verilator/tb_ethernet_host.sv verilog/generated/DskEth-Rev-Cf.v \
		>/dev/null && ./verilog/verilator/obj_ethernet_host/ethernet-host-test

.PHONY: storage-ddram-test
storage-ddram-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_ddram $(V_WARN) \
		--Mdir verilog/verilator/obj_storage_ddram -o storage-ddram-test \
		fpga/mister/dorado_storage_ddram.sv verilog/verilator/tb_storage_ddram.sv \
		>/dev/null && ./verilog/verilator/obj_storage_ddram/storage-ddram-test

.PHONY: pack-wallclock-test
pack-wallclock-test:
	@cd ../.. && $(V) --binary --top-module tb_pack_wallclock $(V_WARN) \
		--Mdir verilog/verilator/obj_pack_wallclock -o wallclock-test \
		fpga/mister/dorado_pack_hps.sv verilog/verilator/tb_pack_wallclock.sv \
		>/dev/null && ./verilog/verilator/obj_pack_wallclock/wallclock-test

.PHONY: storage-linecache-test
storage-linecache-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_linecache $(V_WARN) \
		--Mdir verilog/verilator/obj_storage_linecache -o storage-linecache-test \
		fpga/mister/dorado_storage_ddram.sv verilog/verilator/tb_storage_linecache.sv \
		>/dev/null && ./verilog/verilator/obj_storage_linecache/storage-linecache-test

.PHONY: storage-group-test
storage-group-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_group -GDEFER_READS=$(STORAGE_DEFER_READS) -GADDR_BITS=$(STORAGE_GROUP_ADDR_BITS) $(V_WARN) \
		--Mdir verilog/verilator/obj_storage_group -o storage-group-test \
		fpga/mister/dorado_storage_group.sv verilog/cells/cell_MK4096P_6.v \
		verilog/verilator/tb_storage_group.sv \
		>/dev/null && ./verilog/verilator/obj_storage_group/storage-group-test

.PHONY: storage-arb-test storage-arb-throughput-test storage-arb-native-test
storage-arb-test: storage-arb-throughput-test storage-arb-native-test
	@for groups in 1 3 7 128; do for prefetch in 0 1; do for interleave in 0 1; do \
		cd ../.. && $(V) --binary --top-module tb_storage_arb $(V_WARN) -Wno-TIMESCALEMOD \
		-GGROUPS=$$groups -GPREFETCH_NEXT=$$prefetch -GINTERLEAVE_GROUPS=$$interleave \
		--Mdir verilog/verilator/obj_storage_arb_$${groups}_$${prefetch}_$$interleave -o arb-test \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_arb.sv >/dev/null && \
		./verilog/verilator/obj_storage_arb_$${groups}_$${prefetch}_$$interleave/arb-test || exit $$?; \
		cd verilog/verilator; \
	done; done; done

storage-arb-native-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_arb $(V_WARN) -Wno-TIMESCALEMOD \
		-GGROUPS=128 -GGROUP_ADDR_BITS=16 -GRAW_ADDR_BITS=23 -GPREFETCH_NEXT=1 -GINTERLEAVE_GROUPS=1 \
		--Mdir verilog/verilator/obj_storage_arb_native -o arb-test \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_arb.sv >/dev/null && \
		./verilog/verilator/obj_storage_arb_native/arb-test

storage-arb-throughput-test:
	@for groups in 3 128; do for interleave in 0 1; do \
		cd ../.. && $(V) --binary --top-module tb_storage_arb_throughput $(V_WARN) -Wno-TIMESCALEMOD \
		-GGROUPS=$$groups -GINTERLEAVE_GROUPS=$$interleave --Mdir verilog/verilator/obj_storage_arb_throughput_$${groups}_$$interleave -o throughput-test \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_arb_throughput.sv >/dev/null && \
		./verilog/verilator/obj_storage_arb_throughput_$${groups}_$$interleave/throughput-test || exit $$?; \
		cd verilog/verilator; \
	done; done

STORAGE_PREFETCH_NEXT ?= 0
STORAGE_INTERLEAVE_GROUPS ?= 0
STORAGE_DEFER_READS ?= 0
STORAGE_GROUP_ADDR_BITS ?= 14
STORAGE_DEFER_ARG = $(if $(filter 1,$(STORAGE_DEFER_READS)),--defer-reads,)
.PHONY: storage-array-test
storage-array-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_array -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		--Mdir verilog/verilator/obj_storage_array -o storage-array-test \
		fpga/mister/dorado_storage_ddram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/cells/cell_MK4096P_6.v \
		verilog/verilator/tb_storage_array.sv \
		>/dev/null && ./verilog/verilator/obj_storage_array/storage-array-test

.PHONY: storage-board-test
storage-linecache-array-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_array -GLINE_CACHE_BITS=7 -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		--Mdir verilog/verilator/obj_storage_linecache_array -o array-test \
		fpga/mister/dorado_storage_ddram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/cells/cell_MK4096P_6.v \
		verilog/verilator/tb_storage_array.sv >/dev/null && \
		./verilog/verilator/obj_storage_linecache_array/array-test

.PHONY: storage-linecache-array-test storage-linecache-board-test
storage-linecache-board-test:
	@for bits in 14 16; do \
		cd ../.. && python3 tools/sil_storage_ddram.py $(STORAGE_DEFER_ARG) --chip-address-bits $$bits \
		-o verilog/verilator/obj_storage_linecache_board$$bits/msa_ddram.v \
		--reference-output verilog/verilator/obj_storage_linecache_board$$bits/msa.v && \
		$(V) --binary --top-module tb_storage_board -GADDR_BITS=$$bits -GLINE_CACHE_BITS=7 -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_storage_linecache_board$$bits -o board-test \
		verilog/verilator/obj_storage_linecache_board$$bits/msa_ddram.v \
		verilog/verilator/obj_storage_linecache_board$$bits/msa.v \
		fpga/mister/dorado_storage_ddram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_board.sv >/dev/null && \
		./verilog/verilator/obj_storage_linecache_board$$bits/board-test || exit $$?; \
		cd verilog/verilator; \
	done

storage-board-test:
	@cd ../.. && python3 tools/sil_storage_ddram.py $(STORAGE_DEFER_ARG) \
		-o verilog/verilator/obj_storage_board/msa_ddram.v && \
		$(V) --binary --top-module tb_storage_board -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_storage_board -o storage-board-test \
		verilog/verilator/obj_storage_board/msa_ddram.v verilog/generated/msa.v \
		fpga/mister/dorado_storage_ddram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_board.sv \
		>/dev/null && ./verilog/verilator/obj_storage_board/storage-board-test

.PHONY: memory-density64-test map-density64-test storage-board64-test
memory-density64-test:
	@cd ../.. && $(V) --binary --top-module tb_memory_density64 $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_memory_density64 -o density-test \
		verilog/verilator/tb_memory_density64.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_memory_density64/density-test

map-density64-test:
	@cd ../.. && python3 tools/sil_to_verilog.py \
		'chm/sil/MemX-Rev-Ch.dm!1_/MemX-Rev-Ch.wl' \
		--dict 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze' --map-kwords 64 \
		-o verilog/verilator/obj_map_density64/MemX.v
	@cd ../.. && $(V) --binary --top-module tb_map_density64 $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_map_density64 -o map-test \
		verilog/verilator/obj_map_density64/MemX.v verilog/verilator/tb_map_density64.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_map_density64/map-test

storage-board64-test:
	@cd ../.. && python3 tools/sil_storage_ddram.py $(STORAGE_DEFER_ARG) --chip-address-bits 16 \
		-o verilog/verilator/obj_storage_board64/msa_ddram.v \
		--reference-output verilog/verilator/obj_storage_board64/msa.v
	@cd ../.. && $(V) --binary --top-module tb_storage_board -GADDR_BITS=16 -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_storage_board64 -o storage-board-test \
		verilog/verilator/obj_storage_board64/msa_ddram.v verilog/verilator/obj_storage_board64/msa.v \
		fpga/mister/dorado_storage_ddram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_board.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_storage_board64/storage-board-test

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
		verilog/verilator/tb_pack.sv verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v verilog/cells/cell_MC1650.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_pack/tb_pack +pack=chm/diskpacks/games-trident.pack 2>&1); \
		  rc=$$?; echo "$$out" | grep -E "tb_pack|Assertion|%Error"; exit $$rc; }

# Same file/cable/Fire-code check with Pilot's ten-word labels.
# Use the CHS-linked Cedar pack prepared for the native boot experiment.
.PHONY: pilot-file-pack-test
PILOT_PACK ?= build/rtl-executive/cedar-work-native-chs.pack
pilot-file-pack-test:
	@cd ../.. && $(V) --binary --top-module tb_pack -GLABEL_WORDS=10 $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_pack_pilot_file -o tb_pack_pilot \
		verilog/verilator/tb_pack.sv verilog/verilator/dorado_pack.sv \
		verilog/cells/dorado_trident.v verilog/cells/cell_MC1650.v >/dev/null && \
		./verilog/verilator/obj_pack_pilot_file/tb_pack_pilot +pack=$(PILOT_PACK)

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
	rm -rf obj_*

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

.PHONY: mouse-test mouse-terminal-test
mouse-test:
	@cd ../.. && $(V) --binary --top-module tb_mouse $(V_WARN) \
		--Mdir verilog/verilator/obj_mouse -o tb_mouse \
		verilog/verilator/tb_mouse.sv fpga/mister/dorado_mouse.sv >/dev/null
	@./obj_mouse/tb_mouse
mouse-terminal-test:
	@cd ../.. && $(V) --binary --top-module tb_mouse_terminal $(V_WARN) \
		--Mdir verilog/verilator/obj_mouse_terminal -o tb_mouse_terminal \
		verilog/verilator/tb_mouse_terminal.sv fpga/mister/dorado_mouse.sv \
		verilog/cells/dorado_terminal.v >/dev/null
	@./obj_mouse_terminal/tb_mouse_terminal

PROGRESS_MONITOR_RTL ?= fpga/mister/dorado_instruction_monitor.sv
PROGRESS_TEST_OBJ ?= verilog/verilator/obj_instruction_progress_$(SYSPER)
.PHONY: instruction-progress-build instruction-progress-test storage-sdram-board-test storage-sdram-test pause-monitor-test speed-monitor-test t-controls-test control-phases-test run-control-test clock-phases-test clock-phase-audit key-queue-test keyboard-terminal-test
instruction-progress-build:
	@cd ../.. && mkdir -p "$(PROGRESS_TEST_OBJ)" && python3 tools/sil_backplane.py \
		--emit "$(PROGRESS_TEST_OBJ)/dorado_world.v" --module dorado_world \
		--boards ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,msa,msaOdd,IFU \
		--processor-monitor --instruction-monitor >/dev/null
	@cd ../.. && python3 tools/sil_to_verilog.py \
		chm/sil/ContA-Rev-Cd.dm\!1_/ContA-Rev-Cd.wl \
		--lc chm/sil/ContA-Rev-Cd.dm\!1_/ContA-Rev-Cd.lc \
		--dict chm/sil/msa-Rev-Bg.dm\!1_/ecldict.analyze \
		--processor-monitor --instruction-monitor -o "$(PROGRESS_TEST_OBJ)/ContA_monitor.v" >/dev/null
	@cd ../.. && $(V) --binary --top-module tb_instruction_progress $(V_WARN) -Wno-TIMESCALEMOD \
		-DWORLD -Iverilog/cells -Iverilog/generated \
		--Mdir $(PROGRESS_TEST_OBJ) -o progress-test \
		verilog/verilator/tb_instruction_progress.sv verilog/verilator/tb_exec.sv \
		$(PROGRESS_MONITOR_RTL) "$(PROGRESS_TEST_OBJ)/dorado_world.v" \
		"$(PROGRESS_TEST_OBJ)/ContA_monitor.v" verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v verilog/generated/IFU-Rev-Ch.v >/dev/null
instruction-progress-test: instruction-progress-build
	@cd ../.. && set -e; for case_name in ordinary held read-tpc write-tpc read-im; do \
		args=''; case $$case_name in held) args='+arm_pc=258 +expect_holds=1';; \
			read-*|write-*) args='+extended=1';; esac; \
		"$(PROGRESS_TEST_OBJ)/progress-test" \
			+preload=verilog/verilator/fixtures/instruction-progress/$$case_name.txt \
			+start=256 +cycles=100000 +nostuck $$args; \
	done

storage-sdram-board-test:
	@cd ../.. && python3 tools/sil_storage_ddram.py $(STORAGE_DEFER_ARG) --chip-address-bits 16 \
		-o verilog/verilator/obj_storage_sdram_board/msa_ddram.v \
		--reference-output verilog/verilator/obj_storage_sdram_board/msa.v
	@cd ../.. && $(V) --binary --top-module tb_storage_board -GADDR_BITS=16 -GPREFETCH_NEXT=$(STORAGE_PREFETCH_NEXT) -GINTERLEAVE_GROUPS=$(STORAGE_INTERLEAVE_GROUPS) -GDEFER_READS=$(STORAGE_DEFER_READS) $(V_WARN) \
		-DSTORAGE_USE_SDRAM -Wno-TIMESCALEMOD -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-REALCVT \
		-Iverilog/cells --Mdir verilog/verilator/obj_storage_sdram_board -o board-test \
		verilog/verilator/obj_storage_sdram_board/msa_ddram.v \
		verilog/verilator/obj_storage_sdram_board/msa.v \
		fpga/mister/dorado_storage_sdram.sv fpga/mister/dorado_storage_group.sv \
		fpga/mister/dorado_storage_arb.sv verilog/verilator/tb_storage_board.sv \
		verilog/verilator/fixtures/xsds_storage.sv \
		verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv >/dev/null
	@./obj_storage_sdram_board/board-test

SDRAM_TEST_RTL ?= fpga/mister/dorado_storage_sdram.sv
SDRAM_TEST_OBJ ?= verilog/verilator/obj_storage_sdram
SDRAM_TEST_HZ ?= 50000000
SDRAM_TEST_CLOCK_DELAY ?= 0
storage-sdram-test:
	@cd ../.. && $(V) --binary --top-module tb_storage_sdram $(V_WARN) -Wno-TIMESCALEMOD \
		-Wno-DEFPARAM -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-REALCVT \
		-GCLOCK_HZ=$(SDRAM_TEST_HZ) -GCLOCK_DELAY_NS=$(SDRAM_TEST_CLOCK_DELAY) \
		--Mdir $(SDRAM_TEST_OBJ) -o tb_storage_sdram \
		$(SDRAM_TEST_RTL) \
		verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv \
		verilog/verilator/tb_storage_sdram.sv >/dev/null
	@cd ../.. && "$(SDRAM_TEST_OBJ)/tb_storage_sdram"

pause-monitor-test:
	@cd ../.. && $(V) --binary --top-module tb_pause_monitor $(V_WARN) -Wno-TIMESCALEMOD \
		--Mdir verilog/verilator/obj_pause_monitor -o tb_pause_monitor \
		fpga/mister/dorado_speed_monitor.sv verilog/verilator/tb_pause_monitor.sv >/dev/null
	@./obj_pause_monitor/tb_pause_monitor

speed-monitor-test:
	@cd ../.. && $(V) --binary --top-module tb_speed_monitor $(V_WARN) -Wno-TIMESCALEMOD \
		--Mdir verilog/verilator/obj_speed_monitor -o tb_speed_monitor \
		fpga/mister/dorado_speed_monitor.sv verilog/verilator/tb_speed_monitor.sv >/dev/null
	@./obj_speed_monitor/tb_speed_monitor

RUN_CONTROL_OUT ?= build/run-control
run-control-test:
	@cd ../.. && python3 tools/test_run_control.py --output "$(RUN_CONTROL_OUT)" --mutations

CONTROL_PHASE_OUT ?= build/control-phase-next
control-phases-test:
	@cd ../.. && python3 tools/test_control_phases.py --output "$(CONTROL_PHASE_OUT)" --mutations

t-controls-test:
	@cd ../.. && python3 tools/check_t_control_wiring.py
	@cd ../.. && $(V) --binary --top-module tb_t_controls $(V_WARN) -Wno-TIMESCALEMOD \
		-Iverilog/cells --Mdir verilog/verilator/obj_t_controls -o tb_t_controls \
		verilog/rtl/dorado_t_controls.sv verilog/verilator/tb_t_controls.sv \
		verilog/cells/cell_MC10101.v verilog/cells/cell_MC10100.v \
		verilog/cells/cell_MC10102.v >/dev/null
	@./obj_t_controls/tb_t_controls

clock-phases-test:
	@cd ../.. && $(V) --binary --top-module tb_clock_phases $(V_WARN) \
		--Mdir verilog/verilator/obj_clock_phases -o tb_clock_phases \
		verilog/verilator/tb_clock_phases.sv verilog/rtl/dorado_clock_phases.sv \
		verilog/cells/cell_MC10176.v >/dev/null
	@./obj_clock_phases/tb_clock_phases

clock-phase-audit:
	@cd ../.. && $(V) --binary --top-module tb_clock_phase_audit $(V_WARN) \
		--Mdir verilog/verilator/obj_clock_phase_audit -o tb_clock_phase_audit \
		verilog/verilator/tb_clock_phase_audit.sv verilog/cells/cell_CLOCKGEN.v >/dev/null
	@./obj_clock_phase_audit/tb_clock_phase_audit

key-queue-test:
	@cd ../.. && $(V) --binary --top-module tb_key_queue $(V_WARN) \
		--Mdir verilog/verilator/obj_key_queue -o tb_key_queue \
		verilog/verilator/tb_key_queue.sv fpga/mister/dorado_key_queue.sv \
		verilog/cells/dorado_keyboard.v >/dev/null
	@./obj_key_queue/tb_key_queue

keyboard-terminal-test:
	@cd ../.. && $(V) --binary --top-module tb_keyboard_terminal $(V_WARN) \
		--Mdir verilog/verilator/obj_keyboard_terminal -o tb_keyboard_terminal \
		verilog/verilator/tb_keyboard_terminal.sv verilog/cells/dorado_keyboard.v \
		verilog/cells/dorado_terminal.v >/dev/null
	@./obj_keyboard_terminal/tb_keyboard_terminal

# A TRIDENT AT THE END OF THE CABLE: presence (Ready+OnLine+TERM together) and
# rotation at 3600 RPM. The gaps are counted as well as the pulses -- a line
# stuck low gives a healthy pulse count and only the duty cycle tells them
# apart, which is the lesson disk-input-test paid for.
.PHONY: ethernet-large-test
ethernet-large-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_wire $(V_WARN) +define+LARGE_FRAMES \
		--Mdir verilog/verilator/obj_ethernet_wire_large -o wire-large-test \
		fpga/mister/dorado_ethernet_wire.sv verilog/verilator/tb_ethernet_wire.sv \
		>/dev/null && ./verilog/verilator/obj_ethernet_wire_large/wire-large-test
	@cd ../.. && $(V) --binary --top-module tb_ethernet_uio $(V_WARN) +define+LARGE_FRAMES \
		--Mdir verilog/verilator/obj_ethernet_uio_large -o uio-large-test \
		fpga/mister/dorado_ethernet_uio.sv verilog/verilator/tb_ethernet_uio.sv \
		>/dev/null && ./verilog/verilator/obj_ethernet_uio_large/uio-large-test

.PHONY: pack-mount-test ethernet-clock-test ethernet-uio-test ethernet-wire-test ethernet-receive-test
ethernet-receive-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_receive $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_receive -o ethernet-receive-test \
		verilog/verilator/tb_ethernet_receive.sv verilog/generated/DskEth-Rev-Cf.v fpga/mister/dorado_ethernet_wire.sv >/dev/null
	@cd ../.. && for phase in 0 1 2 3 4 5 6 7; do \
		verilog/verilator/obj_ethernet_receive/ethernet-receive-test +phase=$$phase || exit $$?; done

pack-mount-test:
	@cd ../.. && $(V) --binary --top-module tb_pack_mount $(V_WARN) \
		--Mdir verilog/verilator/obj_pack_mount -o pack-mount-test \
		verilog/verilator/tb_pack_mount.sv fpga/mister/dorado_pack_mount.sv fpga/mister/dorado_pack_hps.sv >/dev/null
	@./obj_pack_mount/pack-mount-test

ethernet-clock-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_clock $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_ethernet_clock -o ethernet-clock-test \
		verilog/verilator/tb_ethernet_clock.sv verilog/generated/DskEth-Rev-Cf.v >/dev/null
	@cd ../.. && verilog/verilator/obj_ethernet_clock/ethernet-clock-test

ethernet-ddr-uio-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_ddr_uio $(V_WARN) \
		--Mdir verilog/verilator/obj_ethernet_ddr_uio -o tb_ethernet_ddr_uio \
		verilog/verilator/tb_ethernet_ddr_uio.sv fpga/mister/dorado_ethernet_uio.sv >/dev/null
	@cd ../.. && ./verilog/verilator/obj_ethernet_ddr_uio/tb_ethernet_ddr_uio

.PHONY: ethernet-ddr-uio-test
ethernet-uio-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_uio $(V_WARN) \
		--Mdir verilog/verilator/obj_ethernet_uio -o ethernet-uio-test \
		verilog/verilator/tb_ethernet_uio.sv fpga/mister/dorado_ethernet_uio.sv >/dev/null
	@./obj_ethernet_uio/ethernet-uio-test

ethernet-wire-test:
	@cd ../.. && $(V) --binary --top-module tb_ethernet_wire $(V_WARN) \
		--Mdir verilog/verilator/obj_ethernet_wire -o ethernet-wire-test \
		verilog/verilator/tb_ethernet_wire.sv fpga/mister/dorado_ethernet_wire.sv >/dev/null
	@./obj_ethernet_wire/ethernet-wire-test

.PHONY: trident-test
trident-test:
	@cd ../.. && $(V) --binary --top-module tb_trident $(V_WARN) \
		--Mdir verilog/verilator/obj_trident -o tb_trident \
		verilog/verilator/tb_trident.sv verilog/cells/dorado_trident.v verilog/cells/cell_MC1650.v \
		>/dev/null && { out=$$(./verilog/verilator/obj_trident/tb_trident 2>&1); \
		  rc=$$?; echo "$$out" | grep tb_trident; exit $$rc; }

.PHONY: pack-fault-test
pack-fault-test: pilot-pack-test
	@cd ../.. && $(V) --binary --top-module tb_pack_timeout_debug $(V_WARN) \
		--Mdir verilog/verilator/obj_pack_timeout_debug -o pack-timeout-test \
		verilog/verilator/tb_pack_timeout_debug.sv fpga/mister/dorado_pack_hps.sv \
		>/dev/null && ./verilog/verilator/obj_pack_timeout_debug/pack-timeout-test
