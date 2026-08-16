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
		./verilog/verilator/obj_proms/tb_proms | grep tb_proms

# Does the ALU compute what its datasheet says? cell_MC10181 is the Dorado's
# ALU -- two slices on ProcH, two on ProcL -- and it is a transcription of the
# datasheet's function table, so it gets checked against INDEPENDENT
# expressions of the same functions, exhaustively over both operands.
.PHONY: alu-test
alu-test:
	@cd ../.. && $(V) --binary --top-module tb_alu $(V_WARN) \
		-Iverilog/cells --Mdir verilog/verilator/obj_alu -o tb_alu \
		verilog/verilator/tb_alu.sv >/dev/null && \
		./verilog/verilator/obj_alu/tb_alu | grep tb_alu

# Does the assembled machine actually run? The gate is the CLOCK: the
# BaseBoard generates it and fans it out to every slot, and if those nets stop
# moving nothing downstream can compute. Deliberately NOT a toggle count --
# see tools/rtl_machine_check.py for why that would punish correct cells.
.PHONY: machine-test
machine-test:
	@cd ../.. && ./verilog/verilator/obj_dir/Vemu --headless --cycles 20000 \
		| python3 tools/rtl_machine_check.py \
			verilog/generated/dorado_backplane.probes

# Each board lints alone; the backplane top module has to be linted WITH the
# boards it instantiates, because Verilator resolves a module by looking for a
# file of the same name and `ProcH_m_Rev_m_Ce` lives in `ProcH-Rev-Ce.v`.
TOP_V    = $(GEN)/dorado_backplane.v

lint:
	@ok=0; bad=0; \
	for f in $(GEN)/*.v; do \
		if [ "$$f" = "$(TOP_V)" ]; then continue; fi; \
		if $(V) --lint-only $(V_WARN) -I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $$f >/dev/null 2>&1; then \
			ok=$$((ok+1)); \
		else \
			bad=$$((bad+1)); echo "  FAIL $$(basename $$f)"; \
		fi; \
	done; \
	echo "lint: $$ok board(s) clean, $$bad failed"; \
	if [ -f $(TOP_V) ]; then \
		if $(V) --lint-only --top-module dorado_backplane $(V_WARN) \
			-I$(CELLS) -I$(VENDOR) -I$(VENDOR2) $(GEN)/*.v >/dev/null 2>&1; then \
			echo "lint: backplane top module elaborates with its boards"; \
		else \
			bad=$$((bad+1)); echo "  FAIL dorado_backplane.v"; \
		fi; \
	fi; \
	test $$bad -eq 0

boards:
	$(MAKE) -C ../.. verilog-boards

run: all
	./obj_dir/Vemu

clean:
	rm -rf obj_dir
