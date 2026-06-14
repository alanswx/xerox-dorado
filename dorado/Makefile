CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Wshadow -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -O2 -g -Iinclude -Ivendor/6502
LDFLAGS ?=
DEPFLAGS := -MMD -MP

OBJDIR  := build
BINDIR  := build

# SDL2 frontend (optional target `make sdl` / build/dorado-sdl).
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS   := $(shell sdl2-config --libs 2>/dev/null)

MB_OBJS   := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/mbdis.o
LIB_OBJS  := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/altoref.o
EXTRA_OBJS := $(OBJDIR)/mctrace.o $(OBJDIR)/bbtrace.o $(OBJDIR)/bbdis.o $(OBJDIR)/machine.o $(OBJDIR)/dorado.o $(OBJDIR)/test_mb.o $(OBJDIR)/test_disasm.o $(OBJDIR)/test_microcode.o $(OBJDIR)/test_cpu.o $(OBJDIR)/test_baseboard.o $(OBJDIR)/test_memory.o $(OBJDIR)/test_display.o $(OBJDIR)/test_disk.o $(OBJDIR)/test_ethernet.o $(OBJDIR)/test_fastio.o $(OBJDIR)/audit_alufm.o $(OBJDIR)/altodiff_dorado.o
DEPS := $(sort $(MB_OBJS) $(LIB_OBJS) $(EXTRA_OBJS))
DEPS := $(DEPS:.o=.d)

all: $(BINDIR)/dorado $(BINDIR)/mbdis $(BINDIR)/mb2eb $(BINDIR)/mctrace $(BINDIR)/bbtrace $(BINDIR)/bbdis $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio

$(BINDIR)/dorado: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/dorado.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Bootable microcode worlds, generated from the checked-in .mb sources via
# mb2eb into worlds/ (kept in the tree so a run needs nothing in /tmp).
# The Cedar/Mesa worlds are already ready .eb files in ../chm and need no
# generation. Regenerate with `make worlds`.
.PHONY: worlds
worlds: $(BINDIR)/mb2eb
	@mkdir -p worlds
	$(BINDIR)/mb2eb '../chm/dorado/AEmu.mb!2' worlds/aemu.eb 01076

# The SDL frontend links SDL2; build with `make sdl`.
.PHONY: sdl
sdl: $(BINDIR)/dorado-sdl

$(OBJDIR)/dorado_sdl.o: src/dorado_sdl.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BINDIR)/dorado-sdl: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/dorado_sdl.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDL_LIBS)

$(BINDIR)/mbdis: $(MB_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(MB_OBJS) $(LDFLAGS)

$(BINDIR)/mb2eb: $(LIB_OBJS) $(OBJDIR)/mb2eb.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/altodiff-dorado: $(LIB_OBJS) $(OBJDIR)/altodiff_dorado.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/mctrace: $(LIB_OBJS) $(OBJDIR)/mctrace.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/bbtrace: $(LIB_OBJS) $(OBJDIR)/bbtrace.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/bbdis: $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/bbdis.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_mb: $(LIB_OBJS) $(OBJDIR)/test_mb.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_disasm: $(LIB_OBJS) $(OBJDIR)/test_disasm.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_microcode: $(LIB_OBJS) $(OBJDIR)/test_microcode.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_cpu: $(LIB_OBJS) $(OBJDIR)/test_cpu.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_baseboard: $(LIB_OBJS) $(OBJDIR)/test_baseboard.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_memory: $(LIB_OBJS) $(OBJDIR)/test_memory.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_display: $(LIB_OBJS) $(OBJDIR)/test_display.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_disk: $(LIB_OBJS) $(OBJDIR)/test_disk.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_ethernet: $(LIB_OBJS) $(OBJDIR)/test_ethernet.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_fastio: $(LIB_OBJS) $(OBJDIR)/test_fastio.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/audit_alufm: $(LIB_OBJS) $(OBJDIR)/audit_alufm.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: tests/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $@

test: $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio
	$(BINDIR)/test_mb
	@echo
	$(BINDIR)/test_disasm
	@echo
	$(BINDIR)/test_microcode
	@echo
	$(BINDIR)/test_memory
	@echo
	$(BINDIR)/test_display
	@echo
	$(BINDIR)/test_disk
	@echo
	$(BINDIR)/test_ethernet
	@echo
	$(BINDIR)/test_fastio
	@echo
	$(BINDIR)/test_cpu
	@echo
	$(BINDIR)/test_baseboard

clean:
	rm -rf $(OBJDIR)

# Interactive boot recipes (open an SDL window). Each builds the SDL
# frontend then launches the right emulator-microcode world (--eb) plus a
# Stage-2 boot file (--eftp). A boot file's format dictates its world:
#   od -An -to2 -N2 FILE -> 0o002401 = Alto B-format -> worlds/aemu.eb
#                           0o162400 = Mesa outload   -> CedarDorado.eb!6
#
# WORKING - Alto emulator world (worlds/aemu.eb): these come up interactive.
.PHONY: run-netexec run-crttest run-dmt
run-netexec: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/NETEXEC.BOOT!8'
run-crttest: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/CRTTEST.BOOT!1'
run-dmt: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/DMT.BOOT!22'

# Alto games (from CHM Io/Murray collection; all Alto B-format 0o002401).
# Highly-painting: Galaxian, MissileCommand, PinBall.  Boots with minimal
# display (cursor/attract): AstroRoids, Invaders, Reversi.  Boots but no
# display at 100 M cycles: Pool, StarWars, Trek, Boggs (may need keyboard
# input or more cycles to reach attract mode).
.PHONY: run-galaxian run-missilecommand run-pinball run-astroids run-invaders
.PHONY: run-reversi run-pool run-starwars run-trek run-boggs
run-galaxian: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1'
run-missilecommand: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/MissileCommand.boot!1'
run-pinball: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/PinBall.boot!1'
run-astroids: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/AstroRoids.boot!1'
run-invaders: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Invaders.boot!1'
run-reversi: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Reversi.boot!1'
run-pool: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Pool.boot!1'
run-starwars: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/StarWars.boot!1'
run-trek: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Trek.boot!1'
run-boggs: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Boggs.boot!1'

# Alto utilities / diagnostics (from CHM Io/Murray collection).
.PHONY: run-etherwatch run-ftp run-scavenger run-madtest
.PHONY: run-etherload run-keytest run-calculator run-edp
.PHONY: run-puptest run-bfstest run-messenger run-diex
.PHONY: run-copydisk run-gatecontrol run-showais run-clock
run-etherwatch: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EtherWatch.boot!1'
run-ftp: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/FTP.boot!1'
run-scavenger: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Scavenger.boot!1'
run-madtest: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/MadTest.boot!1'
run-etherload: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EtherLoad.boot!1'
run-keytest: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/KeyTest.boot!1'
run-calculator: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Calculator.boot!1'
run-edp: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EDP.boot!1'
run-puptest: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/PupTest.boot!1'
run-bfstest: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/BFSTest.boot!1'
run-messenger: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Messenger.boot!1'
run-diex: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/DiEx.boot!1'
run-copydisk: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/CopyDisk.boot!1'
run-gatecontrol: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/GateControl.boot!1'
run-showais: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/ShowAIS.boot!1'
run-clock: sdl
	./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Clock.boot!1'

# EXPERIMENTAL - Cedar/Mesa emulator world (../chm/dorado/CedarDorado.eb!6):
# these are Mesa/Pilot outload files. The boot path is INCOMPLETE - the
# microcode loads but does not yet run (needs InitialEtherCedarDorado.eb,
# the Pilot germ, and a Mesa-path Stage-2). They will not reach a UI yet.
.PHONY: run-cedarnetexec run-mazewar run-mesanetexec run-newos
run-cedarnetexec: sdl
	./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/CedarNetExec.boot!4'
run-mazewar: sdl
	./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/MazeWar.boot!1'
run-mesanetexec: sdl
	./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/MesaNetExec.boot!1'
run-newos: sdl
	./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/NEWOS.BOOT!21'

.PHONY: all clean test worlds sdl

-include $(DEPS)
