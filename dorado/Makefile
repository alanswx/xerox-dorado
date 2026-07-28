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
EXTRA_OBJS := $(OBJDIR)/mctrace.o $(OBJDIR)/bbtrace.o $(OBJDIR)/bbdis.o $(OBJDIR)/pdi.o $(OBJDIR)/pdidump.o $(OBJDIR)/test_pdi.o $(OBJDIR)/machine.o $(OBJDIR)/dorado.o $(OBJDIR)/dorado_web.o $(OBJDIR)/test_mb.o $(OBJDIR)/test_disasm.o $(OBJDIR)/test_microcode.o $(OBJDIR)/test_cpu.o $(OBJDIR)/test_baseboard.o $(OBJDIR)/test_memory.o $(OBJDIR)/test_display.o $(OBJDIR)/test_disk.o $(OBJDIR)/test_dsk2trident.o $(OBJDIR)/test_ethernet.o $(OBJDIR)/test_fastio.o $(OBJDIR)/audit_alufm.o $(OBJDIR)/altodiff_dorado.o $(OBJDIR)/test_snapshot.o
PALO_SRC ?= ../AltoInfo/palo/src
PALO_FS_OBJS := $(addprefix $(PALO_SRC)/,fs/basic.o fs/check.o fs/dir.o fs/disk.o fs/file.o fs/fs.o fs/meta.o fs/scan.o fs/print.o common/utils.o)
# Header-dependency tracking. We -include every .d file actually present in
# the build dir (not just a hand-maintained object list) so that ANY object —
# including the tool binaries' objects like rundiag.o / mb2eb.o that are not in
# the lists above — gets rebuilt when a shared header (e.g. cpu.h, which
# defines the dorado_cpu struct) changes. Missing this caused a struct-ABI
# mismatch: cpu.o rebuilt with new fields while rundiag.o kept the old layout.

all: $(BINDIR)/dorado $(BINDIR)/mbdis $(BINDIR)/mb2eb $(BINDIR)/dsk2trident $(BINDIR)/rundiag $(BINDIR)/mctrace $(BINDIR)/bbtrace $(BINDIR)/bbdis $(BINDIR)/pdidump $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_dsk2trident $(BINDIR)/test_ethernet $(BINDIR)/test_fastio $(BINDIR)/test_snapshot

$(BINDIR)/dorado: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/typetext.o $(OBJDIR)/dorado.o | $(BINDIR)
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
             src/fastio.c src/altoref.c src/pdi.c src/machine.c \
             src/typetext.c src/dorado_web.c
WEB_NODE_SRCS := $(filter-out src/dorado_web.c,$(WEB_SRCS)) src/dorado.c
WEB_NODE_JS := $(BINDIR)/dorado-node.js
WEB_LISP_ASSET_DIR := web-assets
WEB_STP_DIR := build/web-stp
WEB_LISP_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lyric-xcl.snap.gz
WEB_LISP_PACK_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lyric-xcl.pack.gz
WEB_LISP_RAW_DIR := $(BINDIR)/web-lisp
WEB_LISP_RAW_SNAPSHOT := $(WEB_LISP_RAW_DIR)/lisp-lyric-xcl.snap
WEB_LISP_RAW_PACK := $(WEB_LISP_RAW_DIR)/lisp-lyric-xcl.pack
WEB_LISP_RAW_SYSOUT := $(WEB_LISP_RAW_DIR)/lisp-lyric-usable.sysout
WEB_CEDAR_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/cedar-login.snap.gz
WEB_CEDAR_RAW_DIR := $(BINDIR)/web-cedar
WEB_CEDAR_RAW_SNAPSHOT := $(WEB_CEDAR_RAW_DIR)/cedar-login.snap
WEB_CEDAR_DESKTOP_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/cedar-desktop.snap.gz
WEB_CEDAR_CORPUS_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/cedar-corpus.snap.gz
WEB_CEDAR_CORPUS_PDI_GZ := $(WEB_LISP_ASSET_DIR)/cedar-corpus.pdi.gz
WEB_CEDAR_CORPUS_RAW_SNAPSHOT := $(WEB_CEDAR_RAW_DIR)/cedar-corpus.snap
WEB_CEDAR_CORPUS_RAW_PDI := $(WEB_CEDAR_RAW_DIR)/cedar-corpus.pdi
WEB_CEDAR_DESKTOP_PDI_GZ := $(WEB_LISP_ASSET_DIR)/cedar-desktop.pdi.gz
WEB_CEDAR_DESKTOP_RAW_SNAPSHOT := $(WEB_CEDAR_RAW_DIR)/cedar-desktop.snap
WEB_CEDAR_DESKTOP_RAW_PDI := $(WEB_CEDAR_RAW_DIR)/cedar-desktop.pdi
WEB_CEDAR_DEMO_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/cedar-demo.snap.gz
WEB_CEDAR_DEMO_PDI_GZ := $(WEB_LISP_ASSET_DIR)/cedar-demo.pdi.gz
WEB_CEDAR_DEMO_RAW_SNAPSHOT := $(WEB_CEDAR_RAW_DIR)/cedar-demo.snap
WEB_CEDAR_DEMO_RAW_PDI := $(WEB_CEDAR_RAW_DIR)/cedar-demo.pdi
# _GNU_SOURCE exposes timegm()/strcasecmp() in Emscripten's musl sysroot;
# macOS headers expose them by default, so the native build doesn't need it.
# No SDL: the web frontend presents via the canvas 2d context and takes
# input through exported C functions (see src/dorado_web.c header).
WEB_CFLAGS := -std=c99 -D_GNU_SOURCE -O2 -Iinclude -Ivendor/6502
# STACK_SIZE: Emscripten's default wasm stack is 64 KB and overflow corrupts
# static data silently (no guard page); give the emulator real headroom.
WEB_LDFLAGS := -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=67108864 \
               -sSTACK_SIZE=1048576 \
               -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS \
               -sEXPORTED_FUNCTIONS=_main,_dorado_web_boot,_dorado_web_boot_cedar,_dorado_web_boot_cedar_desktop,_dorado_web_boot_cedar_demo,_dorado_web_boot_cedar_corpus,_dorado_web_boot_mesa,_dorado_web_boot_disk,_dorado_web_boot_lisp,_dorado_web_key,_dorado_web_mouse,_dorado_web_debug,_dorado_web_paste \
               --preload-file worlds/aemu.eb@/worlds/aemu.eb \
               --preload-file ../chm/bootfiles@/chm/bootfiles \
               --preload-file '../chm/dorado/AltoMesaDorado.eb!2'@/worlds/AltoMesaDorado.eb \
               --preload-file '../chm/dorado/CedarDorado.eb!6'@/worlds/CedarDorado.eb \
               --preload-file '../chm/cedar/germ-alt/Dorado.germ-6.1.6'@/worlds/Dorado.germ-6.1.6 \
               --preload-file '../CedarDisk/CedarDorado-boot.pdi'@/worlds/CedarDorado-boot.pdi \
               --preload-file '$(WEB_CEDAR_SNAPSHOT_GZ)'@/worlds/cedar-login.snap.gz \
               --preload-file '$(WEB_CEDAR_DESKTOP_SNAPSHOT_GZ)'@/worlds/cedar-desktop.snap.gz \
               --preload-file '$(WEB_CEDAR_DESKTOP_PDI_GZ)'@/worlds/cedar-desktop.pdi.gz \
               --preload-file '$(WEB_CEDAR_DEMO_SNAPSHOT_GZ)'@/worlds/cedar-demo.snap.gz \
               --preload-file '$(WEB_CEDAR_DEMO_PDI_GZ)'@/worlds/cedar-demo.pdi.gz \
               --preload-file '$(WEB_CEDAR_CORPUS_SNAPSHOT_GZ)'@/worlds/cedar-corpus.snap.gz \
               --preload-file '$(WEB_CEDAR_CORPUS_PDI_GZ)'@/worlds/cedar-corpus.pdi.gz \
               --preload-file '../chm/diskpacks/games-trident.pack'@/worlds/games-trident.pack \
               --preload-file '$(WEB_LISP_SNAPSHOT_GZ)'@/worlds/lisp-lyric-xcl.snap.gz \
               --preload-file '$(WEB_LISP_PACK_GZ)'@/worlds/lisp-lyric-xcl.pack.gz \
               --preload-file '$(WEB_STP_DIR)'@/stp \
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
CEDAR_EB := ../chm/dorado/CedarDorado.eb!6
CEDAR_GERM := ../chm/cedar/germ-alt/Dorado.germ-6.1.6
CEDAR_SNAPSHOT := build/good-packs/cedar-login.snap
CEDAR_SNAPSHOT_SCREEN := build/good-packs/cedar-login.pgm
CEDAR_SNAPSHOT_CYCLES ?= 650000000
CEDAR_NATIVE_SNAPSHOT_GZ := snapshot-assets/cedar-login-native.snap.gz
CEDAR_BESTOF_PDI := ../CedarDisk/CedarDorado-bestof.pdi
CEDAR_BESTOF_SNAPSHOT := build/good-packs/cedar-bestof-login.snap
CEDAR_BESTOF_SNAPSHOT_SCREEN := build/good-packs/cedar-bestof-login.pgm
CEDAR_BESTOF_SNAPSHOT_CYCLES ?= 1200000000
CEDAR_BESTOF_NATIVE_SNAPSHOT_GZ := snapshot-assets/cedar-bestof-login-native.snap.gz
# The work volume (65K pages, matched germ + BasicCedar boot, free-page labels
# stamped with the volume ID) is the medium that carries the full LoaderDriver
# install: login -> Basic.Loadees -> BootEssentials -> fonts. Runs never write
# the image back unless DORADO_PDI_SAVE=1, so targets run it in place.
CEDAR_WORK_PDI := ../CedarDisk/CedarDorado-work.pdi
$(CEDAR_PDI): $(CEDAR_PDI).gz
	gunzip -c $< > $@
$(CEDAR_BESTOF_PDI): $(CEDAR_BESTOF_PDI).gz
	gunzip -c $< > $@
$(CEDAR_WORK_PDI): $(CEDAR_WORK_PDI).gz
	gunzip -c $< > $@

# Alto disk-boot pack (Diablo-on-Trident). The raw .pack is gitignored (large);
# the .gz is tracked, so decompress it for `make web` and the Pages CI build.
WEB_ALTO_PACK := ../chm/diskpacks/games-trident.pack
$(WEB_ALTO_PACK): $(WEB_ALTO_PACK).gz
	gunzip -c $< > $@

.PHONY: web
web: worlds $(CEDAR_PDI) $(WEB_ALTO_PACK) $(WEB_CEDAR_SNAPSHOT_GZ) $(WEB_CEDAR_DESKTOP_SNAPSHOT_GZ) $(WEB_CEDAR_DESKTOP_PDI_GZ) $(WEB_CEDAR_DEMO_SNAPSHOT_GZ) $(WEB_CEDAR_DEMO_PDI_GZ) $(WEB_CEDAR_CORPUS_SNAPSHOT_GZ) $(WEB_CEDAR_CORPUS_PDI_GZ) $(WEB_LISP_SNAPSHOT_GZ) $(WEB_LISP_PACK_GZ) web-stp | web-dir
	$(EMCC) $(WEB_CFLAGS) $(WEB_SRCS) $(WEB_LDFLAGS) -o web/index.html

# Pruned STP release tree preloaded into the wasm FS at /stp: CedarChest6.1
# (AISImages, AIS/AISViewer, PressReader, Sil, ...) plus the font
# directories, so the browser's in-process STP server can answer Bringovers
# and AISViewer demand-fetches. The full stp-root would push index.data past
# GitHub's 100 MB per-file limit; Cedar6.1 itself stays out because the
# desktop checkpoint already carries the installed release.
.PHONY: web-stp
web-stp:
	rm -rf '$(WEB_STP_DIR)'
	mkdir -p '$(WEB_STP_DIR)'
	cp -R ../chm/cedar/stp-root/CedarChest6.1 '$(WEB_STP_DIR)/'
	cp -R ../chm/cedar/stp-root/Fonts '$(WEB_STP_DIR)/'
	cp -R ../chm/cedar/stp-root/PressFonts '$(WEB_STP_DIR)/'
	cp -R ../chm/cedar/stp-root/CedarFonts '$(WEB_STP_DIR)/'
	# Cedar6.1 is 24 MB and would push index.data past GitHub's 100 MB
	# per-file limit, but the interpreter DEMAND-FETCHES from it: an Eval
	# through ViewerOps asks for Cedar6.1/Viewers/{ViewerOps,ButtonsImpl}.bcd
	# and fails with "UnknownError[sig: ...]" if they are absent (that was
	# the browser-only Moon.cm failure of 2026-07-19). Ship the Top DFs
	# (the date index scans <vol>/Top) and the Viewers directory.
	mkdir -p '$(WEB_STP_DIR)/Cedar6.1'
	cp -R ../chm/cedar/stp-root/Cedar6.1/Top '$(WEB_STP_DIR)/Cedar6.1/'
	cp -R ../chm/cedar/stp-root/Cedar6.1/Viewers '$(WEB_STP_DIR)/Cedar6.1/'

# Node-hosted wasm32 CLI used only to create ABI-compatible browser snapshots.
# NODERAWFS lets the ordinary dorado CLI read/write the repository paths.
$(WEB_NODE_JS): $(WEB_NODE_SRCS) | $(BINDIR)
	$(EMCC) $(WEB_CFLAGS) $(WEB_NODE_SRCS) \
		-sENVIRONMENT=node -sNODERAWFS=1 -sALLOW_MEMORY_GROWTH=1 \
		-sINITIAL_MEMORY=67108864 -sSTACK_SIZE=1048576 -o $@

.PHONY: lisp-lyric-web-snapshot
lisp-lyric-web-snapshot: $(WEB_NODE_JS)
	@test -r '$(LISP_GOOD_PACK)' || \
		(echo "Missing $(LISP_GOOD_PACK)." && false)
	@test -r '$(LISP_DESKTOP_SNAPSHOT_PACK)' || \
		(echo "Missing deterministic native checkpoint pack $(LISP_DESKTOP_SNAPSHOT_PACK). Run 'make lisp-lyric-desktop-snapshot' first." && false)
	mkdir -p '$(WEB_LISP_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(LISP_GOOD_PACK)' '$(WEB_LISP_RAW_PACK)'
	chmod 644 '$(WEB_LISP_RAW_PACK)'
	../tools/interlisp-sysout/discard_stale_process.py \
		$(foreach record,$(LISP_GOOD_STALE_PROCESSES),--record '$(record)') \
		'$(LISP_GOOD_SYSOUT_FILE)' '$(WEB_LISP_RAW_SYSOUT)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	DORADO_SNAPSHOT_NO_PACK_FLUSH=1 \
	node '$(WEB_NODE_JS)' \
		--eb worlds/aemu.eb \
		--disk 0='$(WEB_LISP_RAW_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-sysout '$(WEB_LISP_RAW_SYSOUT)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles '$(LISP_DESKTOP_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(WEB_LISP_RAW_SNAPSHOT)'
	# Native and wasm32 runs are cycle/data deterministic, but their raw C
	# snapshot ABIs differ. Pair the wasm32 checkpoint with the native run's
	# already-flushed media image; restore validation checks this pair.
	cp -f '$(LISP_DESKTOP_SNAPSHOT_PACK)' '$(WEB_LISP_RAW_PACK)'
	gzip -c '$(WEB_LISP_RAW_SNAPSHOT)' > '$(WEB_LISP_SNAPSHOT_GZ)'
	gzip -c '$(WEB_LISP_RAW_PACK)' > '$(WEB_LISP_PACK_GZ)'

# The compressed browser assets are generated together and intentionally
# checked into/deployed with the web build. Give make a useful diagnostic in a
# checkout where they have not been generated yet.
$(WEB_LISP_SNAPSHOT_GZ) $(WEB_LISP_PACK_GZ):
	@echo "Missing WebAssembly Lyric checkpoint assets."
	@echo "Run 'make lisp-lyric-web-snapshot' with emsdk on PATH."
	@false

.PHONY: cedar-login-web-snapshot
cedar-login-web-snapshot: $(WEB_NODE_JS) $(CEDAR_PDI)
	mkdir -p '$(WEB_CEDAR_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	node '$(WEB_NODE_JS)' \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_PDI)' \
		--cycles '$(CEDAR_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(WEB_CEDAR_RAW_SNAPSHOT)'
	gzip -c '$(WEB_CEDAR_RAW_SNAPSHOT)' > '$(WEB_CEDAR_SNAPSHOT_GZ)'

$(WEB_CEDAR_SNAPSHOT_GZ):
	@echo "Missing WebAssembly Cedar login checkpoint asset."
	@echo "Run 'make cedar-login-web-snapshot' with emsdk on PATH."
	@false

# Cedar Viewers DESKTOP checkpoint for the browser (wasm32 ABI — raw
# snapshots serialize C structs, so the native .snap cannot be reused).
# Re-runs the full Guest install under node with the wasm build; the
# snapshot pairs with the PDI image saved at the same cycle
# (DORADO_PDI_SAVE=1 on a copy). NODERAWFS lets the node build read the
# repository stp-root and write the pair to build/web-cedar/.
.PHONY: cedar-corpus-web-snapshot
cedar-corpus-web-snapshot: $(WEB_NODE_JS) $(CEDAR_CORPUS_PDI)
	mkdir -p '$(WEB_CEDAR_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(CEDAR_CORPUS_PDI)' '$(WEB_CEDAR_CORPUS_RAW_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' node '$(WEB_NODE_JS)' \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(WEB_CEDAR_CORPUS_RAW_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--cycles '$(CEDAR_CORPUS_CYCLES)' \
		--snapshot-out '$(WEB_CEDAR_CORPUS_RAW_SNAPSHOT)'
	gzip -c '$(WEB_CEDAR_CORPUS_RAW_SNAPSHOT)' > '$(WEB_CEDAR_CORPUS_SNAPSHOT_GZ)'
	gzip -c '$(WEB_CEDAR_CORPUS_RAW_PDI)' > '$(WEB_CEDAR_CORPUS_PDI_GZ)'

$(WEB_CEDAR_CORPUS_SNAPSHOT_GZ) $(WEB_CEDAR_CORPUS_PDI_GZ):
	@echo "Missing WebAssembly Cedar corpus checkpoint assets."
	@echo "Run 'make cedar-corpus-web-snapshot' with emsdk on PATH."
	@false

.PHONY: cedar-desktop-web-snapshot
cedar-desktop-web-snapshot: $(WEB_NODE_JS) $(CEDAR_WORK_PDI)
	mkdir -p '$(WEB_CEDAR_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(CEDAR_WORK_PDI)' '$(WEB_CEDAR_DESKTOP_RAW_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' node '$(WEB_NODE_JS)' \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(WEB_CEDAR_DESKTOP_RAW_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--cycles '$(CEDAR_DESKTOP_CYCLES)' \
		--snapshot-out '$(WEB_CEDAR_DESKTOP_RAW_SNAPSHOT)'
	gzip -c '$(WEB_CEDAR_DESKTOP_RAW_SNAPSHOT)' > '$(WEB_CEDAR_DESKTOP_SNAPSHOT_GZ)'
	gzip -c '$(WEB_CEDAR_DESKTOP_RAW_PDI)' > '$(WEB_CEDAR_DESKTOP_PDI_GZ)'

$(WEB_CEDAR_DESKTOP_SNAPSHOT_GZ) $(WEB_CEDAR_DESKTOP_PDI_GZ):
	@echo "Missing WebAssembly Cedar desktop checkpoint assets."
	@echo "Run 'make cedar-desktop-web-snapshot' with emsdk on PATH."
	@false

# Cedar offline apps-demo checkpoint for the browser (wasm32 ABI). Cold-boots
# the work volume (the install attaches Chess40 into the font catalog), logs
# in as Guest, then at the desktop BringOvers ChessHack + Clock into memory,
# starts them (they park as icons), and pre-caches the Chess40 font -- so the
# restored world opens ChessHack (click the icon) with NO network. Same recipe
# as the native `cedar-demo-snapshot`, run under node for the wasm ABI.
.PHONY: cedar-demo-web-snapshot
cedar-demo-web-snapshot: $(WEB_NODE_JS) $(CEDAR_WORK_PDI)
	mkdir -p '$(WEB_CEDAR_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(CEDAR_WORK_PDI)' '$(WEB_CEDAR_DEMO_RAW_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' node '$(WEB_NODE_JS)' \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(WEB_CEDAR_DEMO_RAW_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--type-at 31100000000 --click 700,733 \
		--paste-at 31300000000 --paste 'Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack\nBringover -p [Cedar]<CedarChest6.1>Top>Clock\nRun ChessHackImpl\nRun Clock\nClock\nChessHack\nEval ImagerFont.Find["Xerox/TiogaFonts/Chess40"]\n' \
		--cycles '$(CEDAR_DEMO_CYCLES)' \
		--snapshot-out '$(WEB_CEDAR_DEMO_RAW_SNAPSHOT)'
	gzip -c '$(WEB_CEDAR_DEMO_RAW_SNAPSHOT)' > '$(WEB_CEDAR_DEMO_SNAPSHOT_GZ)'
	gzip -c '$(WEB_CEDAR_DEMO_RAW_PDI)' > '$(WEB_CEDAR_DEMO_PDI_GZ)'

$(WEB_CEDAR_DEMO_SNAPSHOT_GZ) $(WEB_CEDAR_DEMO_PDI_GZ):
	@echo "Missing WebAssembly Cedar apps-demo checkpoint assets."
	@echo "Run 'make cedar-demo-web-snapshot' with emsdk on PATH."
	@false

web-dir:
	@mkdir -p web

$(OBJDIR)/dorado_sdl.o: src/dorado_sdl.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BINDIR)/dorado-sdl: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/typetext.o $(OBJDIR)/dorado_sdl.o | $(BINDIR)
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

$(BINDIR)/test_dsk2trident: $(LIB_OBJS) $(OBJDIR)/test_dsk2trident.o | $(BINDIR)
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

test: $(BINDIR)/test_pdi $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_dsk2trident $(BINDIR)/test_ethernet $(BINDIR)/test_fastio $(BINDIR)/test_snapshot
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
	$(BINDIR)/test_dsk2trident
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
# Boot + the lock-step EFTP transfer take ~80-160M cycles before any game
# code runs (~1M cycles per 512-byte packet), so give a run time to load.
#
# WORK: Galaxian, PinBall, Boggs (Boggs paints its portrait by ~200M).
# WORK after a keystroke: AstroRoids, Invaders (blank "type a key" start
# screen; type any key once loaded).  Reversi is a text-prompt UI.
# MissileCommand paints its attract screen, then dies on its first custom
# RAM-microcode opcode (70000B) -- screen persists, game is dead.
#
# CAN NEVER RUN under AEmu-on-Dorado: Pool (JMPRAM), StarWars, Trek
# (70000B-family opcodes).  They load custom Alto RAM microcode and jump
# into it; the real Dorado AEmu has no Alto control RAM (AEmuSources
# ATraps.mc: WRTRAM no-ops, RDRAM returns 0, JMPRAM + 70000B..77777B all
# trap), so they trap into Swat and idle in TeleSwat's receive loop
# (socket 60B) exactly as they would on real hardware.  The machine-debug
# TRAPPC field (M[527B]) shows the trapped PC.
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
.PHONY: run-cedar run-cedar-screenshot cedar-login-snapshot run-cedar-snapshot-sdl \
	run-cedar-bestof cedar-bestof-snapshot run-cedar-bestof-snapshot-sdl \
	run-cedar-work
run-cedar: sdl
	./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' --pilot-disk '$(CEDAR_PDI)' --speed 4000000
# The full install path: log in as Guest at the prompt; LoaderDriver then
# transfers Basic.Loadees + BootEssentials + fonts from --ftp-root.
run-cedar-work: sdl $(CEDAR_WORK_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' --pilot-disk '$(CEDAR_WORK_PDI)' --ftp-root ../chm/cedar/stp-root --speed 4000000

# ---- The CORPUS pack: recovered PARC files on a bootable Cedar volume ----
# Built by tools/make_corpus_volume.sh (see it for why each step is needed).
# The volume is a repack of the recovered corpus into Cedar's own layout with
# free space and flat boot links; it boots, installs, and reaches the desktop,
# where List shows real 1985 files. Checkpoint so it opens instantly.
CEDAR_CORPUS_PDI := ../CedarDisk/CedarDorado-corpus.pdi
CEDAR_CORPUS_PDI_GZ := ../CedarDisk/CedarDorado-corpus.pdi.gz
CEDAR_CORPUS_SNAPSHOT := build/good-packs/cedar-corpus.snap
CEDAR_CORPUS_RUN_PDI := build/good-packs/cedar-corpus.pdi
CEDAR_CORPUS_SNAPSHOT_GZ := snapshot-assets/cedar-corpus-native.snap.gz
CEDAR_CORPUS_RUN_PDI_GZ := snapshot-assets/cedar-corpus-native.pdi.gz
# The corpus volume finds Basic.Loadees on its OWN disk and loads packages
# locally -- the authentic CSL configuration, and slower than our network
# path. 36 G reaches the desktop.
#
# INTERMITTENT: this local-load path sometimes HANGS at
# "Loading [Cedar]<Cedar6.1>AMModel>AMModelPackage.bcd" and never
# proceeds (~40 K display-list pixels instead of ~167 K, screen frozen on
# the loading list while the herald clock keeps ticking). Same volume,
# same pinned clock, so it is a real emulator-side race worth chasing --
# see docs/handoff.md. If a bake lands there, just run it again and check
# the pixel count before shipping the checkpoint.
CEDAR_CORPUS_CYCLES ?= 36000000000

$(CEDAR_CORPUS_PDI): $(CEDAR_CORPUS_PDI_GZ)
	gunzip -c '$<' > '$@'

.PHONY: cedar-corpus-snapshot
cedar-corpus-snapshot: $(BINDIR)/dorado $(CEDAR_CORPUS_PDI)
	mkdir -p '$(dir $(CEDAR_CORPUS_SNAPSHOT))' '$(dir $(CEDAR_CORPUS_SNAPSHOT_GZ))'
	cp -f '$(CEDAR_CORPUS_PDI)' '$(CEDAR_CORPUS_RUN_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_CORPUS_RUN_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--cycles '$(CEDAR_CORPUS_CYCLES)' \
		--snapshot-out '$(CEDAR_CORPUS_SNAPSHOT)' \
		--out build/good-packs/cedar-corpus.pgm
	gzip -c '$(CEDAR_CORPUS_SNAPSHOT)' > '$(CEDAR_CORPUS_SNAPSHOT_GZ)'
	gzip -c '$(CEDAR_CORPUS_RUN_PDI)' > '$(CEDAR_CORPUS_RUN_PDI_GZ)'

$(CEDAR_CORPUS_SNAPSHOT): $(CEDAR_CORPUS_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_CORPUS_RUN_PDI): $(CEDAR_CORPUS_RUN_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

# Cold boot the corpus volume (the whole install, ~33 G cycles).
.PHONY: run-cedar-corpus run-cedar-corpus-sdl
run-cedar-corpus: sdl $(CEDAR_CORPUS_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_CORPUS_PDI)' \
		--ftp-root ../chm/cedar/stp-root --speed 4000000

# Open the corpus desktop instantly from its checkpoint.
run-cedar-corpus-sdl: sdl $(CEDAR_CORPUS_SNAPSHOT) $(CEDAR_CORPUS_RUN_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_CORPUS_RUN_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_CORPUS_SNAPSHOT)' --speed 4000000

# ---- Cedar desktop verification gate --------------------------------------
# Catches the two bug classes that shipped on 2026-07-18/19, both of which
# passed every test we had at the time:
#   1. a checkpoint baked MID-INSTALL (screen shows the installer, not the
#      desktop) -- the pixel count gives it away;
#   2. a file the BROWSER's pruned tree lacks. Cedar's interpreter
#      demand-fetches while evaluating, so a missing Cedar6.1 bcd fails
#      ONLY on the web ("UnknownError[sig: ...]"), never in SDL.
# So this gate deliberately serves the PRUNED web tree, and drives the
# desktop the way a visitor does: click the prompt, type Moon.cm, watch
# for the picture. Discriminating counts (2026-07-19): plain desktop
# 167K px, failed Eval 169K (just error text), moon painted 246K.
CEDAR_GATE_MIN_PX ?= 200000
CEDAR_GATE_CYCLES ?= 40000000000

.PHONY: verify-cedar-gate-selftest
verify-cedar-gate-selftest:
	@echo "gate self-test: a log with missing files must FAIL"
	@printf 'STP_MISSING Cedar6.1/Viewers/ViewerOps.bcd\ndorado: 168952 display-list pixels\n' > build/gate-bad.log
	@if ../tools/check_cedar_gate.sh build/gate-bad.log 200000 >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the gate passed a bad log"; exit 1; fi
	@echo "gate self-test: a clean painted log must PASS"
	@printf 'dorado: 245822 display-list pixels\n' > build/gate-good.log
	@../tools/check_cedar_gate.sh build/gate-good.log 200000 >/dev/null \
		|| (echo "SELFTEST FAIL: the gate failed a good log"; exit 1)
	@rm -f build/gate-bad.log build/gate-good.log
	@echo "gate self-test: PASS (fails bad logs, passes good ones)"

.PHONY: verify-cedar-desktop
verify-cedar-desktop: $(BINDIR)/dorado $(CEDAR_DESKTOP_SNAPSHOT) $(CEDAR_DESKTOP_PDI) web-stp
	@echo "verify-cedar-desktop: restoring the desktop and running Moon.cm"
	@echo "  (serving the PRUNED web tree, so browser-only gaps fail here)"
	@cp -f '$(CEDAR_DESKTOP_PDI)' build/verify-cedar.pdi
	@DORADO_FTP_TRACE=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/verify-cedar.pdi \
		--ftp-root '$(WEB_STP_DIR)' \
		--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' --key-hold 1600000 \
		--type-at 31100000000 --click 700,733 \
		--paste-at 31300000000 --paste 'Moon.cm\n' \
		--cycles '$(CEDAR_GATE_CYCLES)' \
		--out build/verify-cedar.pgm > build/verify-cedar.log 2>&1
	@../tools/check_cedar_gate.sh build/verify-cedar.log '$(CEDAR_GATE_MIN_PX)'

# ---- Cedar Viewers DESKTOP checkpoint (2026-07-15 milestone) --------------
# The guest mutates the PDI during the install, so the snapshot is only valid
# with the PDI image saved at the same cycle: the producer runs on a COPY with
# DORADO_PDI_SAVE=1 and preserves the pair as gzipped assets that survive
# `make clean`. Restore must supply the SAVED pdi and --ftp-root (reapplied
# after --snapshot-in), and no --type (its trigger cycles are already past).
CEDAR_DESKTOP_SNAPSHOT := build/good-packs/cedar-desktop.snap
CEDAR_DESKTOP_PDI := build/good-packs/cedar-desktop.pdi
CEDAR_DESKTOP_SCREEN := build/good-packs/cedar-desktop.pgm
# 31B: the complete-tree profile boot (AIS packages + the DoradoWelcome
# command files, buttons, and welcome banner) settles ~29B with BSP sender
# retransmission; the wedge-era 47B is history.
CEDAR_DESKTOP_CYCLES ?= 31000000000
# PIN THE CLOCK. time(NULL) in eth_fill_alto_time is the emulator's only
# nondeterministic input, and it moves how far a boot gets: two bakes of
# this same recipe once landed ~6B cycles apart, one at the desktop and
# one still installing fonts (recognizable by ~87K display-list pixels
# instead of ~167K). Pinned, a rebake reproduces the shipped artifact.
CEDAR_DESKTOP_FAKE_TIME ?= 1783285880
CEDAR_DESKTOP_SNAPSHOT_GZ := snapshot-assets/cedar-desktop-native.snap.gz
CEDAR_DESKTOP_PDI_GZ := snapshot-assets/cedar-desktop-native.pdi.gz

.PHONY: cedar-desktop-snapshot run-cedar-desktop-sdl
cedar-desktop-snapshot: $(BINDIR)/dorado $(CEDAR_WORK_PDI)
	mkdir -p '$(dir $(CEDAR_DESKTOP_SNAPSHOT))' '$(dir $(CEDAR_DESKTOP_SNAPSHOT_GZ))'
	cp -f '$(CEDAR_WORK_PDI)' '$(CEDAR_DESKTOP_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_DESKTOP_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--cycles '$(CEDAR_DESKTOP_CYCLES)' \
		--snapshot-out '$(CEDAR_DESKTOP_SNAPSHOT)' \
		--out '$(CEDAR_DESKTOP_SCREEN)'
	gzip -c '$(CEDAR_DESKTOP_SNAPSHOT)' > '$(CEDAR_DESKTOP_SNAPSHOT_GZ)'
	gzip -c '$(CEDAR_DESKTOP_PDI)' > '$(CEDAR_DESKTOP_PDI_GZ)'

$(CEDAR_DESKTOP_SNAPSHOT): $(CEDAR_DESKTOP_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_DESKTOP_PDI): $(CEDAR_DESKTOP_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

run-cedar-desktop-sdl: sdl $(CEDAR_DESKTOP_SNAPSHOT) $(CEDAR_DESKTOP_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_DESKTOP_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' --speed 4000000

# ---- IAGO: Cedar's own disk utility, and the volumes it builds ----------
# Iago formats disks and creates physical/logical volumes.  It is a
# BOOT-TIME program, offered only when Booting.switches[l] is set, which
# --boot-switches supplies (GermSwap.Switch is {zero..nine, a..z}, so l is
# ordinal 21; the emulator plants it at PrincOps.SD[sBootSwitches]).
#
# You MUST log in first: IagoMainImpl.DoIt calls IagoCommands.Login itself,
# BEFORE the switch test, so the Cedar login prompt IS Iago running.  A run
# that stops at login never reaches the question.
#
# Command entry: IagoOps.GetCommand auto-completes on a SPACE, so typing a
# command's FULL name leaves the tail as input for the NEXT prompt.  Type
# only through the token that makes the name unique ('Create P', not
# 'Create Physical Volume').
CEDAR_IAGO_SNAPSHOT := build/good-packs/cedar-iago.snap
CEDAR_IAGO_PDI := build/good-packs/cedar-iago.pdi
CEDAR_IAGO_SCREEN := build/good-packs/cedar-iago.pgm
CEDAR_IAGO_CYCLES ?= 3200000000
# Shipped, because this pair is a real DEPENDENCY and not just a
# convenience: cedar-diskvolume takes the Iago checkpoint as its INPUT, so
# without it none of the volume targets can run at all.  1.0 MB gzipped
# against a 3.2 B-cycle cold boot.  `make cedar-iago-snapshot` rebakes it.
CEDAR_IAGO_SNAPSHOT_GZ := snapshot-assets/cedar-iago-native.snap.gz
CEDAR_IAGO_PDI_GZ := snapshot-assets/cedar-iago-native.pdi.gz

$(CEDAR_IAGO_SNAPSHOT): $(CEDAR_IAGO_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_IAGO_PDI): $(CEDAR_IAGO_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

.PHONY: cedar-iago-snapshot run-cedar-iago-sdl cedar-iago-volume
# Cold boot to Iago's "> " prompt (31,348 px): log in as Guest, answer y.
cedar-iago-snapshot: $(BINDIR)/dorado $(CEDAR_WORK_PDI)
	mkdir -p '$(dir $(CEDAR_IAGO_SNAPSHOT))'
	cp -f '$(CEDAR_WORK_PDI)' '$(CEDAR_IAGO_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_IAGO_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--boot-switches l \
		--type-at 760000000  --type 'Guest\n\n' \
		--type-at 2400000000 --type 'y' \
		--cycles '$(CEDAR_IAGO_CYCLES)' \
		--snapshot-out '$(CEDAR_IAGO_SNAPSHOT)' \
		--out '$(CEDAR_IAGO_SCREEN)'

run-cedar-iago-sdl: sdl $(CEDAR_IAGO_SNAPSHOT)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_IAGO_PDI)' \
		--ftp-root ../chm/cedar/stp-root --boot-switches l \
		--snapshot-in '$(CEDAR_IAGO_SNAPSHOT)' --speed 4000000

# Build a Cedar volume from nothing on a second drive: Create Physical
# Volume, Create Logical Volume, Erase Logical Volume (a label on every one
# of the 114,100 sectors plus the VAM -- the heaviest write the emulator is
# asked for; it takes most of the cycle budget below).
CEDAR_IAGO_TARGET := build/good-packs/cedar-iago-target.pdi
CEDAR_IAGO_VOL_SNAPSHOT := build/good-packs/cedar-iago-erased.snap
CEDAR_IAGO_VOL_PDI := build/good-packs/cedar-iago-erased.pdi
CEDAR_IAGO_VOL_CYCLES ?= 12000000000

cedar-iago-volume: $(BINDIR)/dorado $(CEDAR_IAGO_SNAPSHOT)
	cp -f '$(CEDAR_IAGO_PDI)' '$(CEDAR_IAGO_VOL_PDI)'
	python3 ../tools/pdi_create_blank.py '$(CEDAR_IAGO_TARGET)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_IAGO_VOL_PDI)' \
		--pilot-disk 1='$(CEDAR_IAGO_TARGET)' \
		--ftp-root ../chm/cedar/stp-root --boot-switches l \
		--snapshot-in '$(CEDAR_IAGO_SNAPSHOT)' \
		--type-at 3250000000 --type 'Create P\n' \
		--type-at 3400000000 --type '1\n' \
		--type-at 3550000000 --type 'CedarClean\n' \
		--type-at 3700000000 --type 'y' \
		--type-at 3850000000 --type 'n' \
		--type-at 4050000000 --type 'Create L\n' \
		--type-at 4200000000 --type 'CedarCleanLV\n' \
		--type-at 4350000000 --type 'CedarClean\n' \
		--type-at 4500000000 --type '\n' \
		--type-at 4650000000 --type 'y' \
		--type-at 4900000000 --type 'n' \
		--type-at 5250000000 --type 'Erase L\n' \
		--type-at 5400000000 --type 'CedarCleanLV\n' \
		--type-at 5550000000 --type 'y' \
		--cycles '$(CEDAR_IAGO_VOL_CYCLES)' \
		--snapshot-out '$(CEDAR_IAGO_VOL_SNAPSHOT)' \
		--out build/good-packs/cedar-iago-erased.pgm

# ---- A Cedar volume built from a blank disk, the way Iago builds one -----
#
# What the period procedure actually says (ReleaseMessage.tioga!9, "Installing
# Cedar 6.1 from scratch"): you NET boot Cedar ("get to the CedarNetExec ...
# type Cedar ... log in ... When asked if you want to use Iago, type Y"), so
# the local disk RD0 is free, and Iago's Create User World then formats and
# fills it.  That is why Create User World only ever targets RD0.  Our
# emulator boots Cedar from RD0, so we build on RD1 instead and run the steps
# individually -- which the same document calls out as the hand path:
# "clients who wish to have non-standard volumes will have to do the various
# steps by hand."
#
# The step order below is Create User World's own (IagoCommands1Impl):
#   CreatePhysicalVolume -> CreateLogicalVolume -> EraseVolume ->
#   CreateVMFile -> Install microcode/germ/bootFile
# with ONE deliberate omission.  Install Initial Microcode writes
# InitialDiskDorado.eb (5192 bytes = exactly 11 pages) into the disk's
# hard-microcode region at low physical pages, and IagoOps.ReservePages
# reserves that region only when the drive ordinal is 0:
#     IF Disk.DriveAttributes[...].ordinal # 0 OR p = reserved THEN RETURN
# On RD1 those pages belong to the logical volume, so the microcode lands on
# the VAM's header (logical page 1 = physical page 4) and the volume boots to
# "FS.Error: Local volume's permanent data structures are inconsistent".
# The emulator supplies microcode with --eb, so skipping it costs nothing.
#
# Iago's prompts are answered positionally by cycle, so these --type-at
# offsets are part of the recipe.  GetCommand auto-completes on a SPACE, so
# each command name is typed only through the token that makes it unique.
CEDAR_VOL_TARGET := build/good-packs/cedar-vol-target.pdi
CEDAR_VOL_WORK := build/good-packs/cedar-vol-work.pdi
CEDAR_VOL_CYCLES ?= 24000000000

.PHONY: cedar-diskvolume cedar-diskvolume-desktop
cedar-diskvolume: $(BINDIR)/dorado $(CEDAR_IAGO_SNAPSHOT)
	cp -f '$(CEDAR_IAGO_PDI)' '$(CEDAR_VOL_WORK)'
	python3 ../tools/pdi_create_blank.py '$(CEDAR_VOL_TARGET)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_VOL_WORK)' \
		--pilot-disk 1='$(CEDAR_VOL_TARGET)' \
		--ftp-root ../chm/cedar/stp-root --boot-switches l \
		--snapshot-in '$(CEDAR_IAGO_SNAPSHOT)' \
		--type-at 3250000000 --type 'Create P\n' \
		--type-at 3400000000 --type '1\n' \
		--type-at 3550000000 --type 'CedarClean\n' \
		--type-at 3700000000 --type 'y' \
		--type-at 3850000000 --type 'n' \
		--type-at 4050000000 --type 'Create L\n' \
		--type-at 4200000000 --type 'CedarCleanLV\n' \
		--type-at 4350000000 --type 'CedarClean\n' \
		--type-at 4500000000 --type '\n' \
		--type-at 4650000000 --type 'y' \
		--type-at 4900000000 --type 'n' \
		--type-at 5250000000 --type 'Erase L\n' \
		--type-at 5400000000 --type 'CedarCleanLV\n' \
		--type-at 5550000000 --type 'y' \
		--type-at 12050000000 --type 'Install B\n' \
		--type-at 12200000000 --type 'CedarCleanLV\n' \
		--type-at 12350000000 --type '\n' \
		--type-at 12550000000 --type '\n' \
		--type-at 14500000000 --type 'y' \
		--type-at 14800000000 --type 'Install G\n' \
		--type-at 14950000000 --type 'CedarCleanLV\n' \
		--type-at 15100000000 --type '\n' \
		--type-at 15300000000 --type '\n' \
		--type-at 15900000000 --type 'Install Cedar\n' \
		--type-at 16050000000 --type 'CedarCleanLV\n' \
		--type-at 16200000000 --type '\n' \
		--type-at 16400000000 --type '\n' \
		--type-at 17000000000 --type 'Create V\n' \
		--type-at 17150000000 --type 'CedarCleanLV\n' \
		--type-at 17300000000 --type '\n' \
		--cycles '$(CEDAR_VOL_CYCLES)' \
		--out build/good-packs/cedar-vol-build.pgm
	@echo "cedar-diskvolume: rewriting the root page checksums"
	@# Cedar zeroes a root page's checksum while the volume is mounted and
	@# rewrites it on a clean dismount.  The emulator never dismounts, so do
	@# what the dismount would have done.
	python3 ../tools/pdi_fix_root_checksums.py '$(CEDAR_VOL_TARGET)'
	@echo "built $(CEDAR_VOL_TARGET) -- boot it with:"
	@echo "  ./build/dorado --boot-reason disk --no-alto-boot \\"
	@echo "    --eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \\"
	@echo "    --pilot-disk $(CEDAR_VOL_TARGET) --ftp-root ../chm/cedar/stp-root"

# Boot the fresh volume and let Cedar install itself onto it, ending at the
# Viewers desktop.  ReleaseMessage: "the installation will take roughly half
# an hour for a Dorado" -- here it is ~34 B cycles.
CEDAR_VOL_DESKTOP_PDI := build/good-packs/cedar-vol-desktop.pdi
CEDAR_VOL_DESKTOP_SNAPSHOT := build/good-packs/cedar-vol-desktop.snap
CEDAR_VOL_DESKTOP_CYCLES ?= 34000000000

cedar-diskvolume-desktop: $(BINDIR)/dorado
	cp -f '$(CEDAR_VOL_TARGET)' '$(CEDAR_VOL_DESKTOP_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_VOL_DESKTOP_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--type-at 760000000 --type 'Guest\n\n' \
		--cycles '$(CEDAR_VOL_DESKTOP_CYCLES)' \
		--snapshot-out '$(CEDAR_VOL_DESKTOP_SNAPSHOT)' \
		--out build/good-packs/cedar-vol-desktop.pgm

# Load a substantial CedarChest collection onto the volume.
#
# CedarChestDoc.tioga: "The directory and DF-file structure for <CedarChestr>
# is the same as that for <Cedarr>.  Each package should be entirely
# described by a DF-file on <CedarChestr>Top>."  So one Bringover per package
# DF is the documented way in, and it is what the 2026-07-16 session used for
# ChessHack.  Two rules that make it work, both learned the hard way:
#   - plain Bringover, NOT `Bringover -p`.  -p fetches public files only, so
#     a package's data files silently do not arrive and you get "1 files
#     acted upon" (the .df and nothing else).
#   - the served .df files are CR-terminated; an LF-terminated one parses as
#     a single line.
# CommandToolCommands.tioga records what Bringover does underneath: "Case 4:
# 'to' is an LName, 'attach' is TRUE ... Case 4 is used by BringOver" -- it
# attaches the local name to the remote file, and the bytes are cached
# locally as they are used, which is why the volume's file count grows.
CEDAR_VOL_SOFTWARE_PDI := build/good-packs/cedar-vol-software.pdi
CEDAR_VOL_SOFTWARE_SNAPSHOT := build/good-packs/cedar-vol-software.snap
# Shipped so the finished volume is usable without a rebuild: the three
# targets together are ~2.5 hours of wall clock (24 B + 34 B + 90 B cycles).
# 3.8 MB gzipped.  run-cedar-diskvolume-sdl opens it directly.
CEDAR_VOL_SOFTWARE_PDI_GZ := snapshot-assets/cedar-vol-software-native.pdi.gz
CEDAR_VOL_SOFTWARE_SNAPSHOT_GZ := snapshot-assets/cedar-vol-software-native.snap.gz

$(CEDAR_VOL_SOFTWARE_PDI): $(CEDAR_VOL_SOFTWARE_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_VOL_SOFTWARE_SNAPSHOT): $(CEDAR_VOL_SOFTWARE_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

# Open the finished volume: a Cedar 6.1 Viewers desktop on a disk that was
# built from a blank file by Iago, carrying ten CedarChest packages.
.PHONY: run-cedar-diskvolume-sdl
run-cedar-diskvolume-sdl: sdl $(CEDAR_VOL_SOFTWARE_SNAPSHOT) $(CEDAR_VOL_SOFTWARE_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_VOL_SOFTWARE_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_VOL_SOFTWARE_SNAPSHOT)' --speed 4000000
# MEASURED, twice, because guessing wasted two long bakes: a CedarChest
# package takes roughly 15 B cycles to Bringover -- the cost is per-file STP
# round trips, not the byte count.  1.2 B/package got through one; 3 B/package
# (86 B total) reached package 3 of 16, still mid-transfer.  A command typed
# while a Bringover is still running interleaves with its output and is lost,
# so the budget has to cover the LAST package, not the average.
#
# 16 packages would therefore need ~270 B cycles (~2.5 hours).  The default
# list below is a curated eight at 15 B each; override to take the rest:
#   make cedar-diskvolume-software CEDAR_CHEST_PACKAGES="Sil Interpress ..." \
#        CEDAR_VOL_SOFTWARE_SPACING=15000000000 CEDAR_VOL_SOFTWARE_CYCLES=280000000000
CEDAR_VOL_SOFTWARE_CYCLES ?= 90000000000
CEDAR_VOL_SOFTWARE_SPACING ?= 6000000000
# The DEFAULT set is the SMALL-CLOSURE packages, and the distinction matters
# far more than the file count.  A package's Bringover cost is dominated by
# its IMPORT CLOSURE, not its own payload: Interpress is 47 files but its
# Bringover goes on to pull MesaRuntime.df, Rope.df, Real.df,
# BasicPackages.df, IO.df, FS.df ... i.e. most of the Cedar runtime, and it
# had not finished after 15 B cycles.  Sil (59 files) is the same shape.
#
# These eight are self-contained (AIS/AISViewer/ChessHack/Clock are the ones
# earlier sessions already installed and ran), so the bake completes:
CEDAR_CHEST_PACKAGES ?= AIS AISViewer AISImages ChessHack Clock CardTable \
	Abutters DoradoArchive
# The heavy ones want their own, much longer run.  They work -- the transfers
# are correct, they are just slow:
#   make cedar-diskvolume-software \
#     CEDAR_CHEST_PACKAGES="Sil Interpress ViewRec BiScrollers PopUpButtons \
#                           ImagerPress PressReader SirPress" \
#     CEDAR_VOL_SOFTWARE_SPACING=40000000000 \
#     CEDAR_VOL_SOFTWARE_CYCLES=360000000000

.PHONY: cedar-diskvolume-software
cedar-diskvolume-software: $(BINDIR)/dorado
	cp -f '$(CEDAR_VOL_DESKTOP_PDI)' '$(CEDAR_VOL_SOFTWARE_PDI)'
	@# Build the --paste series: click the CommandTool for type-in focus,
	@# then one Bringover per package, 1.2 B cycles apart.
	set -e; args=""; at=34200000000; \
	for p in $(CEDAR_CHEST_PACKAGES); do \
	  args="$$args --paste-at $$at --paste 'Bringover [Cedar]<CedarChest6.1>Top>'$$p'\n'"; \
	  at=$$((at + $(CEDAR_VOL_SOFTWARE_SPACING))); \
	done; \
	eval DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_VOL_SOFTWARE_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_VOL_DESKTOP_SNAPSHOT)' \
		--key-hold 1600000 --click 700,733 \
		$$args \
		--cycles '$(CEDAR_VOL_SOFTWARE_CYCLES)' \
		--snapshot-out '$(CEDAR_VOL_SOFTWARE_SNAPSHOT)' \
		--out build/good-packs/cedar-vol-software.pgm
	python3 ../tools/pdi_fix_root_checksums.py '$(CEDAR_VOL_SOFTWARE_PDI)'

# Offline "apps demo" checkpoint: restore the desktop (which now carries
# Chess40 in its font catalog), BringOver ChessHack + Clock into memory,
# start them (they park as icons), and pre-cache the Chess40 font -- then
# snapshot. The result restores to a clean desktop with ChessHack + Clock
# icons and runs the apps with NO network (click an icon to open it; the
# chess board paints from the cached font). Pins the clock; input is the
# desktop checkpoint. Board screenshot: docs/images/cedar-chesshack-board-*.
CEDAR_DEMO_SNAPSHOT := build/good-packs/cedar-demo.snap
CEDAR_DEMO_PDI := build/good-packs/cedar-demo.pdi
CEDAR_DEMO_SCREEN := build/good-packs/cedar-demo.pgm
CEDAR_DEMO_SNAPSHOT_GZ := snapshot-assets/cedar-demo-native.snap.gz
CEDAR_DEMO_PDI_GZ := snapshot-assets/cedar-demo-native.pdi.gz
CEDAR_DEMO_CYCLES ?= 52000000000

.PHONY: cedar-demo-snapshot run-cedar-demo-sdl
cedar-demo-snapshot: $(BINDIR)/dorado $(CEDAR_DESKTOP_SNAPSHOT) $(CEDAR_DESKTOP_PDI)
	mkdir -p '$(dir $(CEDAR_DEMO_SNAPSHOT))' '$(dir $(CEDAR_DEMO_SNAPSHOT_GZ))'
	cp -f '$(CEDAR_DESKTOP_PDI)' '$(CEDAR_DEMO_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_DEMO_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' --key-hold 1600000 \
		--type-at 31100000000 --click 700,733 \
		--paste-at 31300000000 --paste 'Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack\nBringover -p [Cedar]<CedarChest6.1>Top>Clock\nRun ChessHackImpl\nRun Clock\nClock\nChessHack\nEval ImagerFont.Find["Xerox/TiogaFonts/Chess40"]\n' \
		--cycles '$(CEDAR_DEMO_CYCLES)' \
		--snapshot-out '$(CEDAR_DEMO_SNAPSHOT)' \
		--out '$(CEDAR_DEMO_SCREEN)'
	gzip -c '$(CEDAR_DEMO_SNAPSHOT)' > '$(CEDAR_DEMO_SNAPSHOT_GZ)'
	gzip -c '$(CEDAR_DEMO_PDI)' > '$(CEDAR_DEMO_PDI_GZ)'

$(CEDAR_DEMO_SNAPSHOT): $(CEDAR_DEMO_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_DEMO_PDI): $(CEDAR_DEMO_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

run-cedar-demo-sdl: sdl $(CEDAR_DEMO_SNAPSHOT) $(CEDAR_DEMO_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_DEMO_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_DEMO_SNAPSHOT)' --speed 4000000

# The "archive" checkpoint: the desktop plus every 2026-07-16 CedarChest
# package attached (AIS/AISViewer/AISImages, PressReader+ShowPress,
# Sil+BiScrollers/ViewRec/Abutters/SirPress/ImagerPress/Interpress) and the
# ORIGINAL Dorado artifacts on the volume (ProcH/IFU press schematics, the
# SIL symbol libraries, Taft's Booting memo as a Tioga file). Built by
# scripted BringOver from the desktop checkpoint; assets survive make clean.
CEDAR_ARCHIVE_SNAPSHOT := build/good-packs/cedar-archive.snap
CEDAR_ARCHIVE_PDI := build/good-packs/cedar-archive.pdi
CEDAR_ARCHIVE_SNAPSHOT_GZ := snapshot-assets/cedar-archive-native.snap.gz
CEDAR_ARCHIVE_PDI_GZ := snapshot-assets/cedar-archive-native.pdi.gz

$(CEDAR_ARCHIVE_SNAPSHOT): $(CEDAR_ARCHIVE_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_ARCHIVE_PDI): $(CEDAR_ARCHIVE_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

.PHONY: run-cedar-archive-sdl
run-cedar-archive-sdl: sdl $(CEDAR_ARCHIVE_SNAPSHOT) $(CEDAR_ARCHIVE_PDI)
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_ARCHIVE_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_ARCHIVE_SNAPSHOT)' --speed 4000000

run-cedar-real: sdl
	./build/dorado-sdl --boot-reason disk --no-alto-boot --disk-real --eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' --pilot-disk '$(CEDAR_PDI)' --speed 4000000
run-cedar-screenshot: sdl
	SDL_VIDEODRIVER=dummy ./build/dorado-sdl --boot-reason disk --no-alto-boot --eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' --pilot-disk '$(CEDAR_PDI)' --speed 4000000 --screenshot 300 --shot-prefix /tmp/dorado-sdl-cedar

cedar-login-snapshot: $(BINDIR)/dorado $(CEDAR_PDI)
	mkdir -p '$(dir $(CEDAR_SNAPSHOT))' '$(dir $(CEDAR_NATIVE_SNAPSHOT_GZ))'
	./build/dorado --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_PDI)' \
		--cycles '$(CEDAR_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(CEDAR_SNAPSHOT)' \
		--out '$(CEDAR_SNAPSHOT_SCREEN)'
	gzip -c '$(CEDAR_SNAPSHOT)' > '$(CEDAR_NATIVE_SNAPSHOT_GZ)'

# `make clean` removes build/, so preserve a compressed native-ABI checkpoint
# outside it and rehydrate the immediate SDL launch automatically.
$(CEDAR_SNAPSHOT): $(CEDAR_NATIVE_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

run-cedar-snapshot-sdl: sdl $(CEDAR_SNAPSHOT)
	./build/dorado-sdl --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_PDI)' \
		--snapshot-in '$(CEDAR_SNAPSHOT)' --scale 1 --speed 400000

# Corrected Cedar filesystem populated with a representative application set.
run-cedar-bestof: sdl $(CEDAR_BESTOF_PDI)
	./build/dorado-sdl --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_BESTOF_PDI)' --speed 4000000

cedar-bestof-snapshot: $(BINDIR)/dorado $(CEDAR_BESTOF_PDI)
	mkdir -p '$(dir $(CEDAR_BESTOF_SNAPSHOT))' \
		'$(dir $(CEDAR_BESTOF_NATIVE_SNAPSHOT_GZ))'
	./build/dorado --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_BESTOF_PDI)' \
		--cycles '$(CEDAR_BESTOF_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(CEDAR_BESTOF_SNAPSHOT)' \
		--out '$(CEDAR_BESTOF_SNAPSHOT_SCREEN)'
	gzip -c '$(CEDAR_BESTOF_SNAPSHOT)' > '$(CEDAR_BESTOF_NATIVE_SNAPSHOT_GZ)'

$(CEDAR_BESTOF_SNAPSHOT): $(CEDAR_BESTOF_NATIVE_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

run-cedar-bestof-snapshot-sdl: sdl $(CEDAR_BESTOF_SNAPSHOT) $(CEDAR_BESTOF_PDI)
	./build/dorado-sdl --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_BESTOF_PDI)' \
		--snapshot-in '$(CEDAR_BESTOF_SNAPSHOT)' --scale 1 --speed 400000

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
LISP_BCPLPROG_DSK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-loader.dsk
LISP_BCPLPROG_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-loader-trident.pack
LISP_MAIN_PARTITION ?= 5
LISP_AUX_PARTITION ?= 4
LISP_VMEM_PAGES ?= 15002
LISP_ALTOFS_FLAGS ?=
LISP_BOOT_FILE ?=
LISP_FULL_BOOT_FILE ?= Sys.Boot.
PALO_PAR ?= ../AltoInfo/palo/src/par
ALTO_OS_DISK ?= ../AltoInfo/ContrAlto2-beta/Disks/bcpl.dsk
BCPLPROG_BFS ?= ../chm/basicdisks/BcplProg.BFS!13
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
LISP_CREATEFILE_FILE ?= ../chm/alto/CreateFile.run!1
LISP_SYSOUT_FILE ?= $(firstword $(wildcard ../chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1 ../chm/lisp/harmony/basics/LISP.SYSOUT!15 ../chm/lisp/fugue.6/basics/Lisp.sysout!1))
LISP_REMOTE_SYSOUT_FILE ?= ../chm/lisp/fugue.6/basics/SMALL.SYSOUT!1
LISP_REMOTE_SYSOUT_NAME ?= LISP.SYSOUT
LISP_REMOTE_HOST ?= DORADO
LISP_REMOTE_VMEM_PAGE_SPEC ?= 1500D
LISP_REMOTE_FTP_TRACE ?= 1
LISP_LYRIC_VMEM_PAGE_SPEC ?= 15002D
# CreateFile.run writes ~1 page of labels per ~250K cycles after a ~1B boot.
# 5.8B cycles covers 15002D; a 19000D VMEM needs ~9B or the pack is saved
# with a silently TRUNCATED page chain (2026-07-06: "19000-page" pack really
# ended at page 17564).  Verify with dsk2trident --extract + altofs --extract
# LISP.VIRTUALMEM. and check the byte size, not just the CreateFile screen.
LISP_LYRIC_CREATEFILE_CYCLES ?= 5800000000
LISP_LYRIC_CYCLES ?= 7000000000
LISP_LYRIC_SHOT_EVERY ?= 500000000
LISP_LYRIC_FTP_TRACE ?=
LISP_LYRIC_SNAPSHOT_OUT ?=
LISP_LYRIC_SNAPSHOT_IN ?= /tmp/dorado-lisp-lyric-m-7b.snap
LISP_LYRIC_RESUME_PACK ?= /tmp/dorado-lisp-lyric-m-pchist.pack
LISP_LYRIC_RESUME_CYCLES ?= 10000000000
LISP_LYRIC_RESUME_SHOT_EVERY ?= 500000000
LISP_INIT_FILE ?= $(firstword $(wildcard ../chm/lisp/harmony/basics/INIT.NONET!3 ../chm/lisp/fugue.6/basics/DoradoInit.nonet!1))
LISP_BCPLPROG_CREATEFILE_DSK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-createfile.dsk
LISP_BCPLPROG_CREATEFILE_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-createfile-trident.pack
LISP_BCPLPROG_VMEM_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-vmem-trident.pack
LISP_BCPLPROG_EXPANDED_DSK0 ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-expanded-drive0.dsk
LISP_BCPLPROG_EXPANDED_DSK1 ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-expanded-drive1.dsk
LISP_BCPLPROG_EXPANDED_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-expanded-trident.pack
LISP_BCPLPROG_EXPANDED_VMEM_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-expanded-vmem-trident.pack
LISP_BCPLPROG_PHYSICAL_DSK0 ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-physical-drive0.dsk
LISP_BCPLPROG_PHYSICAL_DSK1 ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-physical-drive1.dsk
LISP_BCPLPROG_PHYSICAL_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-physical-trident.pack
LISP_BCPLPROG_SCAVENGED_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-scavenged-trident.pack
LISP_BCPLPROG_SCAVENGED_VMEM_PACK ?= $(TRIDENT_RUN_DIR)/bcplprog-lisp-scavenged-vmem-trident.pack
LISP_GOOD_PACK ?= build/good-packs/lisp-lyric-desktop.pack
LISP_GOOD_RUN_PACK ?= $(TRIDENT_RUN_DIR)/lisp-lyric-desktop-run.pack
LISP_GOOD_SYSOUT_FILE ?= ../chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1
LISP_GOOD_USABLE_SYSOUT ?= $(TRIDENT_RUN_DIR)/lisp-lyric-usable.sysout
# Deleted saved PROCESS records whose stack pages are absent from the sysout.
# The sanitizer makes their contexts safe for Xerox PROCESSWORLD to discard.
LISP_GOOD_STALE_PROCESSES ?= 74:101500 74:101600 74:101700
LISP_DESKTOP_SNAPSHOT ?= build/good-packs/lisp-lyric-xcl.snap
LISP_DESKTOP_SNAPSHOT_PACK ?= build/good-packs/lisp-lyric-xcl.pack
LISP_DESKTOP_SNAPSHOT_CYCLES ?= 8800000000
LISP_DESKTOP_SNAPSHOT_SCREEN ?= build/good-packs/lisp-lyric-xcl.pgm
LISP_CURRENT_VMEM_PAGES ?= 19000
LISP_CURRENT_GOOD_PACK ?= build/good-packs/lisp-current-19000.pack
LISP_CURRENT_DSK0 ?= build/good-packs/lisp-current-drive0.dsk
LISP_CURRENT_DSK1 ?= build/good-packs/lisp-current-drive1.dsk
LISP_CURRENT_AUX_DSK0 ?= build/good-packs/lisp-current-aux-drive0.dsk
LISP_CURRENT_AUX_DSK1 ?= build/good-packs/lisp-current-aux-drive1.dsk
LISP_CURRENT_RUN_PACK ?= $(TRIDENT_RUN_DIR)/lisp-current-19000-run.pack
LISP_CURRENT_FAKE_TIME ?= 1783285880
LISP_CURRENT_WORK_PREFIX ?= build/good-packs/lisp-current-work
LISP_CORE_INSERTS = \
	--insert '$(LISP_RUN_FILE)' LISP.RUN. \
	--insert '$(LISP_SYMS_FILE)' LISP.SYMS. \
	--insert '$(LISP_MC_FILE)' DORADOLISPMC.EB. \
	--insert '$(LISP_ALTOD1_FILE)' ALTOD1MC.EB.

$(TRIDENT_GOLDEN_DIR) $(TRIDENT_RUN_DIR) $(LISP_OS_DIR):
	mkdir -p $@

# `make clean` removes build/good-packs, but the compressed browser checkpoint
# media is preserved outside build/.  It is a valid writable Lyric pack and is
# sufficient to perform a fresh deterministic sysout boot for a native ABI
# snapshot.  Rehydrate it automatically instead of requiring a manually saved
# local pack.
$(LISP_GOOD_PACK): $(WEB_LISP_PACK_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
	chmod 644 '$@'

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
.PHONY: lisp-bcplprog-loader-image lisp-bcplprog-createfile-image
.PHONY: lisp-bcplprog-vmem-image lisp-bcplprog-expanded-image
.PHONY: lisp-bcplprog-expanded-vmem-image lisp-bcplprog-physical-image
.PHONY: lisp-bcplprog-scavenged-image lisp-bcplprog-scavenged-vmem-image
.PHONY: run-lisp-bcplprog-loader-smoke
.PHONY: run-lisp-remote-sysout-smoke run-lisp-lyric-remote-long
.PHONY: run-lisp-lyric-resume-debug
.PHONY: lisp-current-good-pack run-lisp-good-sdl run-lisp-current-sdl
.PHONY: lisp-lyric-desktop-snapshot run-lisp-snapshot-sdl
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

# Preserve the installed Alto OS layout from the CHM BcplProg BFS stream.
# Swat/Swatee check original real disk addresses and file serials; extracting
# the OS files and reinserting them into a fresh image changes those labels.
# This target therefore converts the raw BFS stream by loading it with palo
# `-ibfs` on the first insert, then performs all later inserts against the
# resulting AAR-format writable copy.
lisp-bcplprog-loader-image: $(BINDIR)/dsk2trident $(PALO_PAR) $(BCPLPROG_BFS) | $(TRIDENT_RUN_DIR)
	cp -f '$(BCPLPROG_BFS)' '$(LISP_BCPLPROG_DSK)'
	$(PALO_PAR) -ibfs -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_RUN_FILE)' LISP.RUN.
	$(PALO_PAR) -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_SYMS_FILE)' LISP.SYMS.
	$(PALO_PAR) -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_MC_FILE)' DORADOLISPMC.EB.
	$(PALO_PAR) -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_ALTOD1_FILE)' ALTOD1MC.EB.
	$(PALO_PAR) -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_UPDATE_FILE)' UPDATELISP.CM.
	$(if $(LISP_INIT_FILE),$(PALO_PAR) -1 '$(LISP_BCPLPROG_DSK)' -rw -i '$(LISP_INIT_FILE)' INIT.LCOM.,)
	$(BINDIR)/dsk2trident --all-heads '$(LISP_BCPLPROG_DSK)' '$(LISP_BCPLPROG_PACK)'

lisp-bcplprog-createfile-image: $(BINDIR)/dsk2trident $(PALO_PAR) lisp-bcplprog-loader-image | $(TRIDENT_RUN_DIR)
	cp -f '$(LISP_BCPLPROG_DSK)' '$(LISP_BCPLPROG_CREATEFILE_DSK)'
	$(PALO_PAR) -1 '$(LISP_BCPLPROG_CREATEFILE_DSK)' -rw -i '$(LISP_CREATEFILE_FILE)' CREATEFILE.RUN.
	$(BINDIR)/dsk2trident --all-heads '$(LISP_BCPLPROG_CREATEFILE_DSK)' '$(LISP_BCPLPROG_CREATEFILE_PACK)'

lisp-bcplprog-vmem-image: $(BINDIR)/dorado lisp-bcplprog-createfile-image
	cp -f '$(LISP_BCPLPROG_CREATEFILE_PACK)' '$(LISP_BCPLPROG_VMEM_PACK)'
	./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_VMEM_PACK)' --boot-reason disk --no-alto-boot \
		--type 'createfile.run LISP.VIRTUALMEM $(LISP_REMOTE_VMEM_PAGE_SPEC)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles 1400000000 --out /tmp/dorado-lisp-createfile-vmem.pgm

lisp-bcplprog-expanded-image: $(BINDIR)/altofs $(BINDIR)/dsk2trident lisp-bcplprog-createfile-image
	cp -f '$(LISP_BCPLPROG_CREATEFILE_DSK)' '$(LISP_BCPLPROG_EXPANDED_DSK0)'
	$(BINDIR)/altofs --disk0 '$(LISP_BCPLPROG_EXPANDED_DSK0)' \
		--disk1 '$(LISP_BCPLPROG_EXPANDED_DSK1)' \
		--existing --force-existing --source-cylinders 203 --source-sectors 12 \
		--source-single-drive --source-preserve-vda --init-blank-free --no-vmem
	$(BINDIR)/dsk2trident --all-heads --diablo-cylinders 406 --diablo-sectors 14 \
		--drive1 '$(LISP_BCPLPROG_EXPANDED_DSK1)' \
		'$(LISP_BCPLPROG_EXPANDED_DSK0)' '$(LISP_BCPLPROG_EXPANDED_PACK)'

lisp-bcplprog-expanded-vmem-image: $(BINDIR)/dorado lisp-bcplprog-expanded-image
	cp -f '$(LISP_BCPLPROG_EXPANDED_PACK)' '$(LISP_BCPLPROG_EXPANDED_VMEM_PACK)'
	./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_EXPANDED_VMEM_PACK)' --boot-reason disk --no-alto-boot \
		--type 'createfile.run LISP.VIRTUALMEM $(LISP_LYRIC_VMEM_PAGE_SPEC)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_LYRIC_CREATEFILE_CYCLES) --shot-prefix /tmp/dorado-lisp-expanded-createfile \
		--shot-every 400000000 --out /tmp/dorado-lisp-expanded-createfile.pgm

# Preferred Lisp pack construction path.  Preserve BcplProg's physical sector
# placement, then let the original Alto Scavenger repair directory metadata
# before native CreateFile allocates the large VMEM file.  The older
# lisp-bcplprog-expanded-* targets preserve VDA instead; that is useful for
# host-tool experiments but fails Lisp's Swat/Swatee disk-label checks.
lisp-bcplprog-physical-image: $(BINDIR)/altofs $(BINDIR)/dsk2trident lisp-bcplprog-createfile-image
	cp -f '$(LISP_BCPLPROG_CREATEFILE_DSK)' '$(LISP_BCPLPROG_PHYSICAL_DSK0)'
	$(BINDIR)/altofs --disk0 '$(LISP_BCPLPROG_PHYSICAL_DSK0)' \
		--disk1 '$(LISP_BCPLPROG_PHYSICAL_DSK1)' \
		--existing --force-existing --source-cylinders 203 --source-sectors 12 \
		--source-single-drive --init-blank-free --no-vmem
	$(BINDIR)/dsk2trident --all-heads --diablo-cylinders 406 --diablo-sectors 14 \
		--drive1 '$(LISP_BCPLPROG_PHYSICAL_DSK1)' \
		'$(LISP_BCPLPROG_PHYSICAL_DSK0)' '$(LISP_BCPLPROG_PHYSICAL_PACK)'

lisp-bcplprog-scavenged-image: $(BINDIR)/dorado lisp-bcplprog-physical-image
	cp -f '$(LISP_BCPLPROG_PHYSICAL_PACK)' '$(LISP_BCPLPROG_SCAVENGED_PACK)'
	./build/dorado --eb worlds/aemu.eb \
		--eftp '../chm/bootfiles/Scavenger.boot!1' \
		--disk 0='$(LISP_BCPLPROG_SCAVENGED_PACK)' \
		--cycles 14000000000 --key-hold 1000000 \
		--type-at 1100000000 --type 'Y' \
		--type-at 1600000000 --type 'Y' \
		--type-at 5200000000 --type 'bravo.scratchbin2.\n' \
		--shot-prefix /tmp/dorado-lisp-scavenger --shot-every 1000000000 \
		--out /tmp/dorado-lisp-scavenger.pgm

lisp-bcplprog-scavenged-vmem-image: $(BINDIR)/dorado lisp-bcplprog-scavenged-image
	cp -f '$(LISP_BCPLPROG_SCAVENGED_PACK)' '$(LISP_BCPLPROG_SCAVENGED_VMEM_PACK)'
	./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_SCAVENGED_VMEM_PACK)' --boot-reason disk --no-alto-boot \
		--type 'createfile.run LISP.VIRTUALMEM $(LISP_LYRIC_VMEM_PAGE_SPEC)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_LYRIC_CREATEFILE_CYCLES) --shot-prefix /tmp/dorado-lisp-scavenged-createfile \
		--shot-every 400000000 --out /tmp/dorado-lisp-scavenged-createfile.pgm

ifeq ($(LISP_SYSOUT_FILE),)
lisp-disk-image-full:
	@echo "No Lisp sysout found. Set LISP_SYSOUT_FILE=/path/to/LISP.SYSOUT or fetch the CHM/archive.org Lisp sysout files."
	@false
else
lisp-disk-image-full: $(BINDIR)/altofs $(BINDIR)/dsk2trident $(PALO_PAR) $(LISP_OS_HOST_FILES) | $(TRIDENT_RUN_DIR)
	$(BINDIR)/altofs --disk0 '$(LISP_DSK0)' --disk1 '$(LISP_DSK1)' \
		--vmem-pages $(LISP_VMEM_PAGES) --vmem-after-inserts $(LISP_OS_INSERTS) $(LISP_CORE_INSERTS) \
		--insert '$(LISP_UPDATE_FILE)' UPDATELISP.CM. \
		$(if $(LISP_INIT_FILE),--insert '$(LISP_INIT_FILE)' INIT.LCOM.,) \
		$(LISP_ALTOFS_FLAGS) \
		$(if $(LISP_FULL_BOOT_FILE),--boot-file '$(LISP_FULL_BOOT_FILE)',)
	$(BINDIR)/dsk2trident --partition $(LISP_MAIN_PARTITION) --diablo-cylinders 406 --diablo-sectors 14 --drive1 '$(LISP_DSK1)' '$(LISP_DSK0)' '$(LISP_FULL_PACK)'
	$(BINDIR)/altofs --disk0 '$(LISP_AUX_DSK0)' --disk1 '$(LISP_AUX_DSK1)' \
		--no-vmem --insert '$(LISP_SYSOUT_FILE)' LISP.SYSOUT. \
		$(LISP_ALTOFS_FLAGS)
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
run-lisp-bcplprog-loader-smoke: $(BINDIR)/dorado lisp-bcplprog-loader-image
	DORADO_DISPM_PRESENT=1 ./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_PACK)' --boot-reason disk --no-alto-boot \
		--type 'lisp.run/M\n' --type-at 330000000 --key-hold 4000000 \
		--cycles 700000000 --out /tmp/dorado-lisp-bcplprog-loader-smoke.pgm

run-lisp-remote-sysout-smoke: $(BINDIR)/dorado lisp-bcplprog-vmem-image
	$(if $(LISP_REMOTE_FTP_TRACE),DORADO_FTP_TRACE=1 )DORADO_DISPM_PRESENT=1 ./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_VMEM_PACK)' --boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_REMOTE_SYSOUT_FILE)' \
		--type 'lisp.run/M {$(LISP_REMOTE_HOST)}$(LISP_REMOTE_SYSOUT_NAME)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles 1800000000 --out /tmp/dorado-lisp-remote-sysout-smoke.pgm

run-lisp-lyric-remote-long: $(BINDIR)/dorado lisp-bcplprog-scavenged-vmem-image
	$(if $(LISP_LYRIC_FTP_TRACE),DORADO_FTP_TRACE=1 )DORADO_DISPM_PRESENT=1 ./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_BCPLPROG_SCAVENGED_VMEM_PACK)' --boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_SYSOUT_FILE)' \
		--type 'lisp.run/M {$(LISP_REMOTE_HOST)}$(LISP_REMOTE_SYSOUT_NAME)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_LYRIC_CYCLES) \
		--shot-prefix /tmp/dorado-lisp-lyric --shot-every $(LISP_LYRIC_SHOT_EVERY) \
		$(if $(LISP_LYRIC_SNAPSHOT_OUT),--snapshot-out '$(LISP_LYRIC_SNAPSHOT_OUT)',) \
		--out /tmp/dorado-lisp-lyric.pgm

run-lisp-lyric-resume-debug: $(BINDIR)/dorado
	@test -r '$(LISP_LYRIC_SNAPSHOT_IN)' || \
		(echo "Missing LISP_LYRIC_SNAPSHOT_IN=$(LISP_LYRIC_SNAPSHOT_IN). Run run-lisp-lyric-remote-long with LISP_LYRIC_SNAPSHOT_OUT=... first." && false)
	@test -r '$(LISP_LYRIC_RESUME_PACK)' || \
		(echo "Missing LISP_LYRIC_RESUME_PACK=$(LISP_LYRIC_RESUME_PACK). Use the mutable pack that matches the snapshot." && false)
	DORADO_DISPM_PRESENT=1 ./build/dorado \
		--snapshot-in '$(LISP_LYRIC_SNAPSHOT_IN)' \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_LYRIC_RESUME_PACK)' --boot-reason disk --no-alto-boot \
		--cycles $(LISP_LYRIC_RESUME_CYCLES) \
		--shot-prefix /tmp/dorado-lisp-lyric-resume \
		--shot-every $(LISP_LYRIC_RESUME_SHOT_EVERY) \
		--out /tmp/dorado-lisp-lyric-resume.pgm

lisp-current-good-pack:
	mkdir -p '$(dir $(LISP_CURRENT_GOOD_PACK))'
		$(MAKE) lisp-bcplprog-scavenged-vmem-image \
		LISP_VMEM_PAGES=$(LISP_CURRENT_VMEM_PAGES) \
		LISP_LYRIC_VMEM_PAGE_SPEC=$(LISP_CURRENT_VMEM_PAGES)D \
		LISP_LYRIC_CREATEFILE_CYCLES=9000000000 \
		LISP_RUN_FILE=../chm/lisp/current/LISP.RUN\!3 \
		LISP_SYMS_FILE=../chm/lisp/current/LISP.SYMS\!2 \
		LISP_MC_FILE=../chm/lisp/current/DORADOLISPMC.EB\!1 \
		LISP_INIT_FILE=../chm/lisp/current/DoradoInit.nonet\!1 \
		LISP_SYSOUT_FILE=../chm/lisp/current/FULL.SYSOUT\!2 \
		LISP_BCPLPROG_DSK='$(LISP_CURRENT_WORK_PREFIX)-loader.dsk' \
		LISP_BCPLPROG_PACK='$(LISP_CURRENT_WORK_PREFIX)-loader.pack' \
		LISP_BCPLPROG_CREATEFILE_DSK='$(LISP_CURRENT_WORK_PREFIX)-createfile.dsk' \
		LISP_BCPLPROG_CREATEFILE_PACK='$(LISP_CURRENT_WORK_PREFIX)-createfile.pack' \
		LISP_BCPLPROG_PHYSICAL_DSK0='$(LISP_CURRENT_DSK0)' \
		LISP_BCPLPROG_PHYSICAL_DSK1='$(LISP_CURRENT_DSK1)' \
		LISP_BCPLPROG_PHYSICAL_PACK='$(LISP_CURRENT_WORK_PREFIX)-physical.pack' \
		LISP_BCPLPROG_SCAVENGED_PACK='$(LISP_CURRENT_WORK_PREFIX)-scavenged.pack' \
		LISP_BCPLPROG_SCAVENGED_VMEM_PACK='$(LISP_CURRENT_GOOD_PACK)'

run-lisp-good-sdl: sdl
	@test -r '$(LISP_GOOD_PACK)' || \
		(echo "Missing $(LISP_GOOD_PACK)." && false)
	mkdir -p '$(dir $(LISP_GOOD_RUN_PACK))'
	cp -f '$(LISP_GOOD_PACK)' '$(LISP_GOOD_RUN_PACK)'
	chmod 644 '$(LISP_GOOD_RUN_PACK)'
	../tools/interlisp-sysout/discard_stale_process.py \
		$(foreach record,$(LISP_GOOD_STALE_PROCESSES),--record '$(record)') \
		'$(LISP_GOOD_SYSOUT_FILE)' '$(LISP_GOOD_USABLE_SYSOUT)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_GOOD_RUN_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_GOOD_USABLE_SYSOUT)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--scale 1 --speed 4000000

run-lisp-current-sdl: run-lisp-good-sdl

# Create a matched machine snapshot + writable pack after Lyric reaches its
# Prompt Window / Exec (XCL) desktop.  Machine snapshots intentionally do not
# embed pack media, so these two files are an inseparable pair.
lisp-lyric-desktop-snapshot: $(BINDIR)/dorado $(LISP_GOOD_PACK)
	mkdir -p '$(dir $(LISP_DESKTOP_SNAPSHOT))'
	cp -f '$(LISP_GOOD_PACK)' '$(LISP_DESKTOP_SNAPSHOT_PACK)'
	chmod 644 '$(LISP_DESKTOP_SNAPSHOT_PACK)'
	../tools/interlisp-sysout/discard_stale_process.py \
		$(foreach record,$(LISP_GOOD_STALE_PROCESSES),--record '$(record)') \
		'$(LISP_GOOD_SYSOUT_FILE)' '$(LISP_GOOD_USABLE_SYSOUT)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_DESKTOP_SNAPSHOT_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_GOOD_USABLE_SYSOUT)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles '$(LISP_DESKTOP_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(LISP_DESKTOP_SNAPSHOT)' \
		--out '$(LISP_DESKTOP_SNAPSHOT_SCREEN)'

# Resume the saved XCL desktop immediately.  Copy the matched media so the
# saved pair remains immutable and each interactive run gets a writable pack.
run-lisp-snapshot-sdl: sdl
	@test -r '$(LISP_DESKTOP_SNAPSHOT)' || \
		(echo "Missing $(LISP_DESKTOP_SNAPSHOT). Run 'make lisp-lyric-desktop-snapshot' once." && false)
	@test -r '$(LISP_DESKTOP_SNAPSHOT_PACK)' || \
		(echo "Missing matching pack $(LISP_DESKTOP_SNAPSHOT_PACK)." && false)
	mkdir -p '$(dir $(LISP_GOOD_RUN_PACK))'
	cp -f '$(LISP_DESKTOP_SNAPSHOT_PACK)' '$(LISP_GOOD_RUN_PACK)'
	chmod 644 '$(LISP_GOOD_RUN_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_GOOD_RUN_PACK)' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_DESKTOP_SNAPSHOT)' \
		--scale 1 --speed 400000

run-tricond-nopack: $(BINDIR)/rundiag
	RUNDIAG_DISK=1 RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN TESTOK-WITHOUT-DISK ERR 50000000
run-tricond-pack: $(BINDIR)/rundiag alto-disk-image
	RUNDIAG_DISK=1 RUNDIAG_DISK_MEDIA=diablo RUNDIAG_DISK_PATH='$(ALTO_RUN_PACK)' RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN DONE ERR 120000000

# ---- Smalltalk-76 (2026-07-28 milestone) ---------------------------------
# Booted the period way, per Smalltalk.midas!17 and DSemuRelease.cm: load
# DSemu (the Alto emulator PLUS the Smalltalk microcode, ether-boot BFN 111B)
# and let the Alto Executive's `Bootfrom` start the system off the "XM
# Smalltalk" pack.  --no-alto-boot is REQUIRED: without it the run silently
# falls back to Ethernet and the result is not what it looks like.
# --key-hold 3000000 is also required -- at the 600K default the doubled `o`
# in "Bootfrom" collapses into one press (the Alto path applies keys with no
# pacing) and the Executive sees "Botfrom".
SMALLTALK_EB     ?= ../chm/microcode/SmalltalkDorado.eb!1
SMALLTALK_DSK    ?= ../AltoInfo/ContrAlto2-beta/Disks/xmsmall.dsk
SMALLTALK_PACK   ?= $(TRIDENT_RUN_DIR)/xmsmall-trident.pack
SMALLTALK_CYCLES ?= 1950000000
SMALLTALK_MIN_PX ?= 100000

# Always rebuilt: the guest writes to its pack, so a run must not inherit a
# mutated one.
.PHONY: smalltalk-pack
smalltalk-pack: $(BINDIR)/dsk2trident
	@test -r '$(SMALLTALK_DSK)' || \
		(echo "Missing $(SMALLTALK_DSK) (AltoInfo/ is gitignored)." && false)
	mkdir -p '$(TRIDENT_RUN_DIR)'
	$(BINDIR)/dsk2trident --all-heads '$(SMALLTALK_DSK)' '$(SMALLTALK_PACK)'

.PHONY: run-smalltalk verify-smalltalk
run-smalltalk: sdl smalltalk-pack
	./build/dorado-sdl --eb '$(SMALLTALK_EB)' \
		--disk 0='$(SMALLTALK_PACK)' --boot-reason disk --no-alto-boot \
		--key-hold 3000000 --type-at 350000000 \
		--type 'Bootfrom xmsmall.boot\n'

# Headless gate: Smalltalk-76 must paint its Top View / Classes browser /
# UserView workspace (about 125K px; 0 px means it never reached the
# interpreter, ~1.4K means it stopped at "The keyset is stuck").
verify-smalltalk: $(BINDIR)/dorado smalltalk-pack
	./build/dorado --cycles '$(SMALLTALK_CYCLES)' --eb '$(SMALLTALK_EB)' \
		--disk 0='$(SMALLTALK_PACK)' --boot-reason disk --no-alto-boot \
		--key-hold 3000000 --type-at 350000000 \
		--type 'Bootfrom xmsmall.boot\n' \
		--out build/verify-smalltalk.pgm > build/verify-smalltalk.log 2>&1
	@px=$$(sed -n 's/^dorado: \([0-9]*\) display-list pixels.*/\1/p' \
		build/verify-smalltalk.log | tail -1); \
	echo "verify-smalltalk: $$px px (need >= $(SMALLTALK_MIN_PX))"; \
	test "$$px" -ge '$(SMALLTALK_MIN_PX)'

.PHONY: all clean test worlds sdl web web-dir

-include $(wildcard $(OBJDIR)/*.d)
