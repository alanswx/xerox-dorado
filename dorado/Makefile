CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Wshadow -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -O2 -g -Iinclude -Ivendor/6502
LDFLAGS ?=
DEPFLAGS := -MMD -MP

OBJDIR  := build
BINDIR  := build

# SDL2 frontend (optional target `make sdl` / build/dorado-sdl).
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS   := $(shell sdl2-config --libs 2>/dev/null)

MB_OBJS   := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/pdi.o $(OBJDIR)/mbdis.o
LIB_OBJS  := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/altoref.o $(OBJDIR)/pdi.o
EXTRA_OBJS := $(OBJDIR)/mctrace.o $(OBJDIR)/bbtrace.o $(OBJDIR)/bbdis.o $(OBJDIR)/pdi.o $(OBJDIR)/pdidump.o $(OBJDIR)/test_pdi.o $(OBJDIR)/machine.o $(OBJDIR)/dorado.o $(OBJDIR)/dorado_web.o $(OBJDIR)/test_mb.o $(OBJDIR)/test_disasm.o $(OBJDIR)/test_microcode.o $(OBJDIR)/test_cpu.o $(OBJDIR)/test_baseboard.o $(OBJDIR)/test_memory.o $(OBJDIR)/test_display.o $(OBJDIR)/test_disk.o $(OBJDIR)/test_ethernet.o $(OBJDIR)/test_fastio.o $(OBJDIR)/audit_alufm.o $(OBJDIR)/altodiff_dorado.o
DEPS := $(sort $(MB_OBJS) $(LIB_OBJS) $(EXTRA_OBJS))
DEPS := $(DEPS:.o=.d)

all: $(BINDIR)/dorado $(BINDIR)/mbdis $(BINDIR)/mb2eb $(BINDIR)/mctrace $(BINDIR)/bbtrace $(BINDIR)/bbdis $(BINDIR)/pdidump $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio

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

# WebAssembly frontend (Emscripten). `make web` builds web/index.html +
# .js/.wasm/.data, with the SDL2 port and the boot files preloaded into the
# virtual FS at the same paths the machine fopen()s. Requires emsdk on PATH
# (`source ~/emsdk/emsdk_env.sh`). Serve web/ over http (file:// won't load
# the .wasm). The boot files are preloaded read-only into MEMFS.
EMCC      ?= emcc
WEB_SRCS  := src/mb.c src/disasm.c src/microcode.c src/cpu.c src/baseboard.c \
             src/memory.c src/io.c src/display.c src/disk.c src/ethernet.c \
             src/fastio.c src/altoref.c src/pdi.c src/machine.c src/dorado_web.c
# _GNU_SOURCE exposes timegm()/strcasecmp() in Emscripten's musl sysroot;
# macOS headers expose them by default, so the native build doesn't need it.
WEB_CFLAGS := -std=c99 -D_GNU_SOURCE -O2 -Iinclude -Ivendor/6502 -sUSE_SDL=2
WEB_LDFLAGS := -sUSE_SDL=2 -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=67108864 \
               -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
               -sEXPORTED_FUNCTIONS=_main,_dorado_web_boot,_dorado_web_boot_cedar \
               --preload-file worlds/aemu.eb@/worlds/aemu.eb \
               --preload-file ../chm/bootfiles@/chm/bootfiles \
               --preload-file '../chm/dorado/CedarDorado.eb!6'@/worlds/CedarDorado.eb \
               --preload-file '../chm/cedar/germ-alt/Dorado.germ-6.1.6'@/worlds/Dorado.germ-6.1.6 \
               --preload-file '../CedarDisk/CedarDorado-boot.pdi'@/worlds/CedarDorado-boot.pdi \
               --preload-file ../chm/dorado/doradobaserom.mb!13@/ucode/doradobaserom.mb!13 \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb@/ucode/Bootstrap.mb \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb@/ucode/Initial.mb \
               --preload-file ../chm/dorado/expanded/kernel.dm!38_/kernel.mb@/ucode/kernel.mb \
               --preload-file ../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb@/ucode/memMisc.mb \
               --preload-file ../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb@/ucode/IfuComplex.mb \
               --shell-file src/web_shell.html

.PHONY: web
web: worlds | web-dir
	$(EMCC) $(WEB_CFLAGS) $(WEB_SRCS) $(WEB_LDFLAGS) -o web/index.html

web-dir:
	@mkdir -p web

$(OBJDIR)/dorado_sdl.o: src/dorado_sdl.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BINDIR)/dorado-sdl: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/dorado_sdl.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDL_LIBS)

$(BINDIR)/mbdis: $(MB_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(MB_OBJS) $(LDFLAGS)

# pdidump — offline Pilot/Cedar PARC Disk Image (PDI) inspector.
$(BINDIR)/pdidump: $(OBJDIR)/pdi.o $(OBJDIR)/pdidump.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_pdi: $(OBJDIR)/pdi.o $(OBJDIR)/test_pdi.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

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

test: $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio
	$(BINDIR)/test_pdi
	@echo
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

# Cedar/Pilot Route B. This direct disk/germ path reaches the Cedar
# SimpleTerminal display. The screen stays blank until roughly 650M emulated
# cycles, so run the SDL frontend with a larger cycles-per-frame budget.
.PHONY: run-cedar run-cedar-screenshot
run-cedar: sdl
	./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' --speed 4000000
run-cedar-screenshot: sdl
	SDL_VIDEODRIVER=dummy ./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' --speed 4000000 --screenshot 300 --shot-prefix /tmp/dorado-sdl-cedar

.PHONY: all clean test worlds sdl web web-dir

-include $(DEPS)
