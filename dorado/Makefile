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
EXTRA_OBJS := $(OBJDIR)/mctrace.o $(OBJDIR)/bbtrace.o $(OBJDIR)/bbdis.o $(OBJDIR)/pdi.o $(OBJDIR)/pdidump.o $(OBJDIR)/test_pdi.o $(OBJDIR)/machine.o $(OBJDIR)/dorado.o $(OBJDIR)/dorado_web.o $(OBJDIR)/test_mb.o $(OBJDIR)/test_disasm.o $(OBJDIR)/test_microcode.o $(OBJDIR)/test_cpu.o $(OBJDIR)/test_baseboard.o $(OBJDIR)/test_memory.o $(OBJDIR)/test_display.o $(OBJDIR)/test_disk.o $(OBJDIR)/test_ethernet.o $(OBJDIR)/test_fastio.o $(OBJDIR)/audit_alufm.o $(OBJDIR)/altodiff_dorado.o $(OBJDIR)/test_snapshot.o
PALO_SRC ?= ../AltoInfo/palo/src
PALO_FS_OBJS := $(addprefix $(PALO_SRC)/,fs/basic.o fs/check.o fs/dir.o fs/disk.o fs/file.o fs/fs.o fs/meta.o fs/scan.o fs/print.o common/utils.o)
# Header-dependency tracking. We -include every .d file actually present in
# the build dir (not just a hand-maintained object list) so that ANY object —
# including the tool binaries' objects like rundiag.o / mb2eb.o that are not in
# the lists above — gets rebuilt when a shared header (e.g. cpu.h, which
# defines the dorado_cpu struct) changes. Missing this caused a struct-ABI
# mismatch: cpu.o rebuilt with new fields while rundiag.o kept the old layout.

all: $(BINDIR)/dorado $(BINDIR)/mbdis $(BINDIR)/mb2eb $(BINDIR)/dsk2trident $(BINDIR)/rundiag $(BINDIR)/mctrace $(BINDIR)/bbtrace $(BINDIR)/bbdis $(BINDIR)/pdidump $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio $(BINDIR)/test_snapshot

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
               -sEXPORTED_FUNCTIONS=_main,_dorado_web_boot,_dorado_web_boot_cedar,_dorado_web_boot_mesa,_dorado_web_boot_disk \
               --preload-file worlds/aemu.eb@/worlds/aemu.eb \
               --preload-file ../chm/bootfiles@/chm/bootfiles \
               --preload-file '../chm/dorado/AltoMesaDorado.eb!2'@/worlds/AltoMesaDorado.eb \
               --preload-file '../chm/dorado/CedarDorado.eb!6'@/worlds/CedarDorado.eb \
               --preload-file '../chm/cedar/germ-alt/Dorado.germ-6.1.6'@/worlds/Dorado.germ-6.1.6 \
               --preload-file '../CedarDisk/CedarDorado-boot.pdi'@/worlds/CedarDorado-boot.pdi \
               --preload-file '../chm/diskpacks/games-trident.pack'@/worlds/games-trident.pack \
               --preload-file ../chm/dorado/doradobaserom.mb!13@/ucode/doradobaserom.mb!13 \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb@/ucode/Bootstrap.mb \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb@/ucode/Initial.mb \
               --preload-file ../chm/dorado/expanded/kernel.dm!38_/kernel.mb@/ucode/kernel.mb \
               --preload-file ../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb@/ucode/memMisc.mb \
               --preload-file ../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb@/ucode/IfuComplex.mb \
               --shell-file src/web_shell.html

# The Cedar web option preloads the Pilot/Cedar PDI disk image. The raw .pdi
# is gitignored (large); the .gz is tracked, so decompress it on demand for
# `make web` (and the GitHub Pages CI build, which only has the .gz).
CEDAR_PDI := ../CedarDisk/CedarDorado-boot.pdi
$(CEDAR_PDI): $(CEDAR_PDI).gz
	gunzip -c $< > $@

# Alto disk-boot pack (Diablo-on-Trident). The raw .pack is gitignored (large);
# the .gz is tracked, so decompress it for `make web` and the Pages CI build.
WEB_ALTO_PACK := ../chm/diskpacks/games-trident.pack
$(WEB_ALTO_PACK): $(WEB_ALTO_PACK).gz
	gunzip -c $< > $@

.PHONY: web
web: worlds $(CEDAR_PDI) $(WEB_ALTO_PACK) | web-dir
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

# rundiag — run a Dorado hardware diagnostic (kernel/Ifu/memA/...) on the engine.
$(BINDIR)/rundiag: $(LIB_OBJS) $(OBJDIR)/rundiag.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# dsk2trident — convert a ContrAlto Diablo-31 .dsk into a Diablo-on-Trident pack.
$(BINDIR)/dsk2trident: $(LIB_OBJS) $(OBJDIR)/dsk2trident.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# altofs — optional wrapper around the local gitignored AltoInfo/palo FS
# library. It is deliberately not in `all`; use it through lisp-disk-image.
$(PALO_FS_OBJS):
	$(MAKE) -C $(PALO_SRC) par

$(OBJDIR)/altofs.o: src/altofs.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(PALO_SRC) $(DEPFLAGS) -c -o $@ $<

$(BINDIR)/altofs: $(OBJDIR)/altofs.o $(PALO_FS_OBJS) | $(BINDIR)
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

$(BINDIR)/test_snapshot: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/test_snapshot.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/audit_alufm: $(LIB_OBJS) $(OBJDIR)/audit_alufm.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: tests/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $@

test: $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio $(BINDIR)/test_snapshot
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
	@echo
	$(BINDIR)/test_snapshot

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
run-cedar-real: sdl
	./build/dorado-sdl --boot-reason disk --no-alto-boot --disk-real --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' --speed 4000000
run-cedar-screenshot: sdl
	SDL_VIDEODRIVER=dummy ./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' --speed 4000000 --screenshot 300 --shot-prefix /tmp/dorado-sdl-cedar

# Alto-on-Dorado disk boot (Diablo-on-Trident). A read-only golden pack lives
# under testdata/trident-readonly/ and every run copies it to build/run-disks/
# before booting, so disk writes never mutate the baseline image.
# DSK_SRC may be overridden for `make refresh-alto-golden`.
# The route uses the real DSK task/controller path. AEmu's Diablo-on-Trident
# status mapper needs the Alto NoDataTransferred compatibility bit for KBLK/KCB
# status words; see docs/alto-disk-boot-plan.md.
DSK_SRC ?= ../AltoInfo/ContrAlto2-beta/Disks/games.dsk
TRIDENT_GOLDEN_DIR := testdata/trident-readonly
TRIDENT_RUN_DIR := build/run-disks
ALTO_GOLDEN_PACK_GZ ?= ../chm/diskpacks/games-trident.pack.gz
ALTO_GOLDEN_PACK ?= $(TRIDENT_GOLDEN_DIR)/alto-games-trident.pack
ALTO_RUN_PACK ?= $(TRIDENT_RUN_DIR)/alto-games-trident.pack
LISP_DSK0 ?= $(TRIDENT_RUN_DIR)/lisp-drive0.dsk
LISP_DSK1 ?= $(TRIDENT_RUN_DIR)/lisp-drive1.dsk
LISP_AUX_DSK0 ?= $(TRIDENT_RUN_DIR)/lisp-aux-drive0.dsk
LISP_AUX_DSK1 ?= $(TRIDENT_RUN_DIR)/lisp-aux-drive1.dsk
LISP_RUN_PACK ?= $(TRIDENT_RUN_DIR)/lisp-diablo-trident.pack
LISP_FULL_PACK ?= $(TRIDENT_RUN_DIR)/lisp-full-diablo-trident.pack
LISP_MAIN_PARTITION ?= 5
LISP_AUX_PARTITION ?= 4
LISP_BOOT_FILE ?=
LISP_FULL_BOOT_FILE ?= Sys.Boot.
PALO_PAR ?= ../AltoInfo/palo/src/par
ALTO_OS_DISK ?= ../AltoInfo/ContrAlto2-beta/Disks/bcpl.dsk
LISP_OS_DIR ?= $(TRIDENT_RUN_DIR)/lisp-os-files
LISP_OS_SYS_BOOT ?= $(LISP_OS_DIR)/Sys.Boot.
LISP_OS_EXEC ?= $(LISP_OS_DIR)/Executive.Run.
LISP_OS_SWAT ?= $(LISP_OS_DIR)/Swat.
LISP_OS_SWATEE ?= $(LISP_OS_DIR)/Swatee.
LISP_OS_SYSFONT ?= $(LISP_OS_DIR)/SysFont.Al.
LISP_OS_SYS_ERRORS ?= $(LISP_OS_DIR)/SYS.ERRORS.
LISP_OS_COM_CM ?= $(LISP_OS_DIR)/Com.Cm.
LISP_OS_USER_CM ?= $(LISP_OS_DIR)/User.Cm.
LISP_OS_SCAVENGER ?= $(LISP_OS_DIR)/SCAVENGER.RUN.
LISP_OS_HOST_FILES = \
	$(LISP_OS_SYS_BOOT) \
	$(LISP_OS_EXEC) \
	$(LISP_OS_SWAT) \
	$(LISP_OS_SWATEE) \
	$(LISP_OS_SYSFONT) \
	$(LISP_OS_SYS_ERRORS) \
	$(LISP_OS_COM_CM) \
	$(LISP_OS_USER_CM) \
	$(LISP_OS_SCAVENGER)
LISP_OS_INSERTS = \
	--insert '$(LISP_OS_SYS_BOOT)' Sys.Boot. \
	--insert '$(LISP_OS_EXEC)' Executive.Run. \
	--insert '$(LISP_OS_SWAT)' Swat. \
	--insert '$(LISP_OS_SWATEE)' Swatee. \
	--insert '$(LISP_OS_SYSFONT)' SysFont.Al. \
	--insert '$(LISP_OS_SYS_ERRORS)' SYS.ERRORS. \
	--insert '$(LISP_OS_COM_CM)' Com.Cm. \
	--insert '$(LISP_OS_USER_CM)' User.Cm. \
	--insert '$(LISP_OS_SCAVENGER)' SCAVENGER.RUN.
LISP_RUN_FILE ?= ../chm/lisp/Lisp.run!6
LISP_SYMS_FILE ?= ../chm/lisp/Lisp.syms!4
LISP_MC_FILE ?= ../chm/lisp/DORADOLISPMC.EB!1
LISP_ALTOD1_FILE ?= ../chm/lisp/altod1mc.eb!1
LISP_UPDATE_FILE ?= ../chm/lisp/cm/UpdateLisp.CM!8
LISP_SYSOUT_FILE ?= $(firstword $(wildcard ../chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1 ../chm/lisp/harmony/basics/LISP.SYSOUT!15 ../chm/lisp/fugue.6/basics/Lisp.sysout!1))
LISP_INIT_FILE ?= $(firstword $(wildcard ../chm/lisp/harmony/basics/INIT.NONET!3 ../chm/lisp/fugue.6/basics/DoradoInit.nonet!1))
LISP_CORE_INSERTS = \
	--insert '$(LISP_RUN_FILE)' LISP.RUN. \
	--insert '$(LISP_SYMS_FILE)' LISP.SYMS. \
	--insert '$(LISP_MC_FILE)' DORADOLISPMC.EB. \
	--insert '$(LISP_ALTOD1_FILE)' ALTOD1MC.EB.

$(TRIDENT_GOLDEN_DIR) $(TRIDENT_RUN_DIR) $(LISP_OS_DIR):
	mkdir -p $@

$(PALO_PAR):
	$(MAKE) -C ../AltoInfo/palo/src par

$(LISP_OS_SYS_BOOT): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e Sys.Boot. '$@'

$(LISP_OS_EXEC): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e Executive.Run. '$@'

$(LISP_OS_SWAT): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e Swat. '$@'

$(LISP_OS_SWATEE): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e Swatee. '$@'

$(LISP_OS_SYSFONT): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e SysFont.Al. '$@'

$(LISP_OS_SYS_ERRORS): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e SYS.ERRORS. '$@'

$(LISP_OS_COM_CM): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e Com.Cm. '$@'

$(LISP_OS_USER_CM): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e User.Cm. '$@'

$(LISP_OS_SCAVENGER): $(PALO_PAR) $(ALTO_OS_DISK) | $(LISP_OS_DIR)
	$(PALO_PAR) -1 '$(ALTO_OS_DISK)' -e SCAVENGER.RUN. '$@'

$(ALTO_GOLDEN_PACK): $(ALTO_GOLDEN_PACK_GZ) | $(TRIDENT_GOLDEN_DIR)
	chmod u+w '$@' 2>/dev/null || true
	gunzip -c '$<' > '$@'
	chmod 444 '$@'

.PHONY: trident-readonly-images alto-disk-image refresh-alto-golden
.PHONY: lisp-disk-image
.PHONY: lisp-disk-image-full
.PHONY: run-alto-disk run-alto-disk-screenshot
.PHONY: run-bfstest-smoke run-scavenger-smoke
.PHONY: run-tricond-nopack run-tricond-pack
trident-readonly-images: $(ALTO_GOLDEN_PACK)

refresh-alto-golden: $(BINDIR)/dsk2trident | $(TRIDENT_GOLDEN_DIR)
	chmod u+w '$(ALTO_GOLDEN_PACK)' 2>/dev/null || true
	$(BINDIR)/dsk2trident --all-heads '$(DSK_SRC)' '$(ALTO_GOLDEN_PACK)'
	chmod 444 '$(ALTO_GOLDEN_PACK)'

alto-disk-image: trident-readonly-images | $(TRIDENT_RUN_DIR)
	cp -f '$(ALTO_GOLDEN_PACK)' '$(ALTO_RUN_PACK)'
	chmod 644 '$(ALTO_RUN_PACK)'

lisp-disk-image: $(BINDIR)/altofs $(BINDIR)/dsk2trident | $(TRIDENT_RUN_DIR)
	$(BINDIR)/altofs --disk0 '$(LISP_DSK0)' --disk1 '$(LISP_DSK1)' \
		$(LISP_CORE_INSERTS) $(if $(LISP_BOOT_FILE),--boot-file '$(LISP_BOOT_FILE)',)
	$(BINDIR)/dsk2trident --all-heads --diablo-cylinders 406 --diablo-sectors 14 --drive1 '$(LISP_DSK1)' '$(LISP_DSK0)' '$(LISP_RUN_PACK)'

ifeq ($(LISP_SYSOUT_FILE),)
lisp-disk-image-full:
	@echo "No Lisp sysout found. Set LISP_SYSOUT_FILE=/path/to/LISP.SYSOUT or fetch the CHM/archive.org Lisp sysout files."
	@false
else
lisp-disk-image-full: $(BINDIR)/altofs $(BINDIR)/dsk2trident $(PALO_PAR) $(LISP_OS_HOST_FILES) | $(TRIDENT_RUN_DIR)
	$(BINDIR)/altofs --disk0 '$(LISP_DSK0)' --disk1 '$(LISP_DSK1)' \
		--vmem-after-inserts $(LISP_OS_INSERTS) $(if $(LISP_FULL_BOOT_FILE),--boot-file '$(LISP_FULL_BOOT_FILE)',)
	$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_RUN_FILE)' LISP.RUN. '$(LISP_DSK0)'
	$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_SYMS_FILE)' LISP.SYMS. '$(LISP_DSK0)'
	$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_MC_FILE)' DORADOLISPMC.EB. '$(LISP_DSK0)'
	$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_ALTOD1_FILE)' ALTOD1MC.EB. '$(LISP_DSK0)'
	$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_UPDATE_FILE)' UPDATELISP.CM. '$(LISP_DSK0)'
	$(if $(LISP_INIT_FILE),$(PALO_PAR) -rw --disks 2 --cylinders 406 --heads 2 --sectors 14 -2 '$(LISP_DSK1)' -i '$(LISP_INIT_FILE)' INIT.LCOM. '$(LISP_DSK0)',)
	$(BINDIR)/dsk2trident --partition $(LISP_MAIN_PARTITION) --diablo-cylinders 406 --diablo-sectors 14 --drive1 '$(LISP_DSK1)' '$(LISP_DSK0)' '$(LISP_FULL_PACK)'
	$(BINDIR)/altofs --disk0 '$(LISP_AUX_DSK0)' --disk1 '$(LISP_AUX_DSK1)' \
		--no-vmem --insert '$(LISP_SYSOUT_FILE)' LISP.SYSOUT.
	$(BINDIR)/dsk2trident --base '$(LISP_FULL_PACK)' --partition $(LISP_AUX_PARTITION) --diablo-cylinders 406 --diablo-sectors 14 --drive1 '$(LISP_AUX_DSK1)' '$(LISP_AUX_DSK0)' '$(LISP_FULL_PACK)'
endif

run-alto-disk: sdl alto-disk-image
	./build/dorado-sdl --eb worlds/aemu.eb --disk 0='$(ALTO_RUN_PACK)' --boot-reason disk --speed 4000000
run-alto-disk-screenshot: sdl alto-disk-image
	SDL_VIDEODRIVER=dummy ./build/dorado-sdl --eb worlds/aemu.eb --disk 0='$(ALTO_RUN_PACK)' --boot-reason disk --speed 4000000 --screenshot 300 --shot-prefix /tmp/dorado-alto-disk
run-bfstest-smoke: $(BINDIR)/dorado alto-disk-image
	DORADO_DISK_WRITE_PROGRESS=5000 ./build/dorado --eb worlds/aemu.eb \
		--eftp '../chm/bootfiles/BFSTest.boot!1' \
		--disk 0='$(ALTO_RUN_PACK)' --cycles 4700000000 --key-hold 1000000 \
		--type-at 180000000 --type 'erase\n' \
		--type-at 260000000 --type 'Y' \
		--type-at 300000000 --type 'Y' \
		--type-at 340000000 --type 'Y' \
		--type-at 380000000 --type 'Y' \
		--type-at 900000000 --type 'Y' \
		--type-at 3400000000 --type 'createfile\nDORADOTEST\n10\n' \
		--out /tmp/dorado-bfstest-smoke.pgm
run-scavenger-smoke: $(BINDIR)/dorado run-bfstest-smoke
	./build/dorado --eb worlds/aemu.eb \
		--eftp '../chm/bootfiles/Scavenger.boot!1' \
		--disk 0='$(ALTO_RUN_PACK)' --cycles 10000000000 --key-hold 1000000 \
		--type-at 1100000000 --type 'Y' \
		--type-at 5200000000 --type 'Y' \
		--out /tmp/dorado-scavenger-smoke.pgm

run-tricond-nopack: $(BINDIR)/rundiag
	RUNDIAG_DISK=1 RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN TESTOK-WITHOUT-DISK ERR 50000000
run-tricond-pack: $(BINDIR)/rundiag alto-disk-image
	RUNDIAG_DISK=1 RUNDIAG_DISK_MEDIA=diablo RUNDIAG_DISK_PATH='$(ALTO_RUN_PACK)' RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN DONE ERR 120000000

.PHONY: all clean test worlds sdl web web-dir

-include $(wildcard $(OBJDIR)/*.d)
