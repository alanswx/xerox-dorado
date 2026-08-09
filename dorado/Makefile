CC      ?= cc
# -O3 -flto: measured 11.9% over -O2 on the Alto path, byte-identical
# framebuffer (2026-08-04, docs/performance-plan.md Phase 0). -mcpu=apple-m4
# was tried and does NOT help (slightly worse than plain -O3), so it is
# deliberately absent -- it would also make the build host-specific.
# For the rest of Phase 0's gain, see `make pgo` below.
CFLAGS  ?= -std=c99 -D_GNU_SOURCE -Wall -Wextra -Wshadow -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -O3 -flto -g -Iinclude -Ivendor/6502
LDFLAGS ?=
DEPFLAGS := -MMD -MP

OBJDIR  := build
BINDIR  := build

# SDL2 frontend (optional target `make sdl` / build/dorado-sdl).
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS   := $(shell sdl2-config --libs 2>/dev/null)

MB_OBJS   := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/dispm.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/pdi.o $(OBJDIR)/mbdis.o
LIB_OBJS  := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/dispm.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/altoref.o $(OBJDIR)/pdi.o
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
             src/memory.c src/io.c src/display.c src/dispm.c src/disk.c src/ethernet.c \
             src/fastio.c src/altoref.c src/pdi.c src/machine.c \
             src/typetext.c src/dorado_web.c
WEB_NODE_SRCS := $(filter-out src/dorado_web.c,$(WEB_SRCS)) src/dorado.c
WEB_NODE_JS := $(BINDIR)/dorado-node.js
WEB_LISP_ASSET_DIR := web-assets
WEB_KEYBOARD_SVG := ../docs/images/alto-keyboard-map.svg
WEB_STP_DIR := build/web-stp
# The Lyric world shipped to the browser is the LIBRARY world (204 packages,
# the network init, HELPSYS + the IRM served over Leaf).  Its snapshot and
# pack are NOT preloaded into index.data -- they are copied beside it and
# fetched only when the Lyric world is chosen, like cedar-src.tar.gz, so a
# visitor who came for Galaxian never downloads them.
WEB_LISP_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lispusers.snap.gz
WEB_LISP_PACK_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lispusers.pack.gz
# The full-library Lyric world (Released-Full.sysout!2), a SECOND Lyric entry
# rather than a replacement -- see dorado_web.c WEB_LISP_FULL_PACK.
WEB_LISP_FULL_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lyric-full.snap.gz
WEB_LISP_FULL_PACK_GZ := $(WEB_LISP_ASSET_DIR)/lisp-lyric-full.pack.gz
WEB_SMALLTALK_SNAPSHOT_GZ := $(WEB_LISP_ASSET_DIR)/smalltalk76.snap.gz
WEB_SMALLTALK_PACK_GZ     := $(WEB_LISP_ASSET_DIR)/smalltalk76.pack.gz
WEB_SMALLTALK_RAW_DIR     := $(BINDIR)/web-smalltalk
WEB_SMALLTALK_RAW_SNAPSHOT := $(WEB_SMALLTALK_RAW_DIR)/smalltalk76.snap
WEB_SMALLTALK_RAW_PACK     := $(WEB_SMALLTALK_RAW_DIR)/smalltalk76.pack
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
# Browser build flags. Measured 2026-08-04 on the node harness (same wasm),
# Galaxian 1 B cycles, every step byte-identical:
#
#   -O2 (the old default)                   0.54x real hardware
#   -O3                                     0.54x   (no gain -- omitted)
#   -O3 -flto                               0.57x
#   -O3 -flto + the NATIVE PGO profile      0.68x   <- 27% over the old default
#
# The last line is the surprise: emscripten cannot COLLECT a profile (an
# instrumented wasm build runs but writes no .profraw -- the LLVM profile
# runtime is not there), but it can USE one. Profiles are keyed by function
# name, so the profile `make pgo` collects from the NATIVE binary applies to
# the wasm build of the same sources.
#
# Conditional on purpose: with no build/pgo/dorado.profdata present the flag
# is omitted rather than passed and failing, so `make web` still works in a
# clean tree and in CI. Run `make pgo` before `make web` for the fast build.
WEB_PGO := $(if $(wildcard build/pgo/dorado.profdata),\
	-fprofile-instr-use=build/pgo/dorado.profdata,)
WEB_CFLAGS := -std=c99 -D_GNU_SOURCE -O3 -flto $(WEB_PGO) -Iinclude -Ivendor/6502
# STACK_SIZE: Emscripten's default wasm stack is 64 KB and overflow corrupts
# static data silently (no guard page); give the emulator real headroom.
WEB_LDFLAGS := -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=67108864 \
               -sSTACK_SIZE=1048576 \
               -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS \
               -sEXPORTED_FUNCTIONS=_main,_dorado_web_boot,_dorado_web_boot_cedar,_dorado_web_boot_cedar_desktop,_dorado_web_boot_cedar_demo,_dorado_web_boot_cedar_corpus,_dorado_web_boot_mesa,_dorado_web_boot_disk,_dorado_web_boot_lisp,_dorado_web_boot_lisp_full,_dorado_web_boot_smalltalk,_dorado_web_set_dispm,_dorado_web_key,_dorado_web_mouse,_dorado_web_mouse_delta,_dorado_web_mouse_delta_active,_dorado_web_mouse_buttons,_dorado_web_debug,_dorado_web_paste,_dorado_web_save_state,_dorado_web_restore_state,_dorado_web_world_tag,_dorado_web_panel,_dorado_web_boot_button,_dorado_web_color_frame,_dorado_web_color_w,_dorado_web_color_h,_dorado_web_color_display_right \
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
               --preload-file '../chm/microcode/SmalltalkDorado.eb!1'@/worlds/SmalltalkDorado.eb \
               --preload-file '$(WEB_SMALLTALK_SNAPSHOT_GZ)'@/worlds/smalltalk76.snap.gz \
               --preload-file '$(WEB_SMALLTALK_PACK_GZ)'@/worlds/smalltalk76.pack.gz \
               --preload-file '$(WEB_STP_DIR)'@/stp \
               --preload-file ../chm/dorado/doradobaserom.mb!13@/ucode/doradobaserom.mb!13 \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb@/ucode/Bootstrap.mb \
               --preload-file ../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb@/ucode/Initial.mb \
               --preload-file ../chm/dorado/expanded/kernel.dm!38_/kernel.mb@/ucode/kernel.mb \
               --preload-file ../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb@/ucode/memMisc.mb \
               --preload-file ../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb@/ucode/IfuComplex.mb \
               --shell-file $(BINDIR)/web_shell.gen.html

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
# The preload bundle is cache-busted by build id (see web_shell.html's
# locateFile): stamp the shell each build so a redeploy that adds a world is
# actually seen by returning visitors instead of their cached index.data.
WEB_BUILD_ID := $(shell git rev-parse --short HEAD 2>/dev/null || date +%s)
.PHONY: web-shell
web-shell: src/web_shell.html | $(BINDIR)
	sed 's/@@BUILD_ID@@/$(WEB_BUILD_ID)/g' src/web_shell.html > $(BINDIR)/web_shell.gen.html

web: worlds $(CEDAR_PDI) $(WEB_ALTO_PACK) $(WEB_KEYBOARD_SVG) $(WEB_CEDAR_SNAPSHOT_GZ) $(WEB_CEDAR_DESKTOP_SNAPSHOT_GZ) $(WEB_CEDAR_DESKTOP_PDI_GZ) $(WEB_CEDAR_DEMO_SNAPSHOT_GZ) $(WEB_CEDAR_DEMO_PDI_GZ) $(WEB_CEDAR_CORPUS_SNAPSHOT_GZ) $(WEB_CEDAR_CORPUS_PDI_GZ) $(WEB_LISP_SNAPSHOT_GZ) $(WEB_LISP_PACK_GZ) $(WEB_LISP_FULL_SNAPSHOT_GZ) $(WEB_LISP_FULL_PACK_GZ) $(WEB_SMALLTALK_SNAPSHOT_GZ) $(WEB_SMALLTALK_PACK_GZ) web-stp web-cedar-src web-lisp-src web-shell | web-dir
	cp -f '$(WEB_KEYBOARD_SVG)' web/alto-keyboard-map.svg
	cp -f '$(WEB_LISP_SNAPSHOT_GZ)' web/lisp-lispusers.snap.gz
	cp -f '$(WEB_LISP_PACK_GZ)' web/lisp-lispusers.pack.gz
	cp -f '$(WEB_LISP_FULL_SNAPSHOT_GZ)' web/lisp-lyric-full.snap.gz
	cp -f '$(WEB_LISP_FULL_PACK_GZ)' web/lisp-lyric-full.pack.gz
	$(EMCC) $(WEB_CFLAGS) $(WEB_SRCS) $(WEB_LDFLAGS) -o web/index.html
	@# Cache-bust the emcc-generated loader too. Module.locateFile already
	@# stamps index.data/index.wasm with the build id, but emcc emits a plain
	@# `<script async src=index.js>`; a browser that had the page open kept
	@# serving the OLD index.js beside the NEW wasm/data -- which is how a
	@# rebuilt frontend silently did not take effect during testing.
	@# (sed -i is not portable: BSD needs an argument, GNU must not have one.)
	sed 's|src=index\.js|src="index.js?v=$(WEB_BUILD_ID)"|' web/index.html \
		> web/index.html.tmp && mv -f web/index.html.tmp web/index.html

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
	@# Preloaded means EVERY visitor downloads it, including one who came for
	@# Galaxian. These three are large and are not part of any browser demo
	@# yet: Gargoyle (4.7 MB) and Whiteboard were mirrored to answer "why
	@# doesn't Run Gargoyle work", and DoradoLogic holds the ProcH .sil
	@# schematics whose Sil viewer path is still unproven. Keep them for the
	@# native/SDL tree; move them into the lazily-fetched bundle (see
	@# web-cedar-src) if a browser demo ever needs them.
	rm -rf '$(WEB_STP_DIR)/CedarChest6.1/Gargoyle' \
	       '$(WEB_STP_DIR)/CedarChest6.1/Whiteboard'
	rm -f '$(WEB_STP_DIR)/CedarChest6.1/Top/Gargoyle.df' \
	      '$(WEB_STP_DIR)/CedarChest6.1/Top/Whiteboard.df'
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

# The rest of the release, as a SEPARATE bundle fetched only when a Cedar
# world is chosen -- so `List` and `OpenR` can reach the whole release in the
# browser without putting 10 MB in front of someone who came to play Galaxian.
# It rides BESIDE index.data in the Pages artifact rather than inside it, and
# web_shell.html unpacks it into MEMFS at /stp (see fetchCedarSources): a tar
# is the one archive a browser can walk with ~30 lines of JS and no library,
# and gzip comes free from DecompressionStream, which the shell already uses
# for the snapshots.
#
# Everything EXCEPT what is already preloaded above and the .bcd binaries
# (12.3 MB of compiled code the desktop checkpoint carries installed). A
# deny-list, not an allow-list of source extensions: the first cut listed
# .mesa/.tioga/.df/.cm/.config and silently dropped BOTH version maps
# (`CedarSource.VersionMap` -- the file OpenR resolves short names through),
# the Tioga .style files, and every capitalised variant (`.Mesa`, `.Tioga`,
# `.Profile`), because find's -name is case-sensitive. 963 files, 62 of the
# release's 70 directories, ~2.9 MiB gzipped. The 6 directories not covered
# hold a single .bcd and nothing else.
WEB_CEDAR_SRC := web/cedar-src.tar.gz
WEB_LISP_SRC  := web/lisp-src.tar.gz
.PHONY: web-cedar-src
# The Interlisp served tree, fetched lazily the way cedar-src is: only when a
# Lisp world is chosen, so it never lands on a visitor who came for Galaxian.
# This is what makes the browser build able to serve MORE than the pack holds
# -- the pack ceiling is 22,736 pages (AltoDiabloDisk.mc hard-codes 406
# cylinders and LISP.VIRTUALMEM. takes 15,003 of them), and Leaf lifts it.
#
# Everything in the root goes: the .LCOM library, the display fonts, the
# greeting file, and the documentation. A DENY-list, not an allow-list --
# the Cedar bundle shipped with an allow-list of source extensions and
# silently dropped both version maps and every capitalised variant, because
# `find -name` is case-sensitive.
.PHONY: web-lisp-src
web-lisp-src: | web-dir
	@cd '$(LISP_LISPUSERS_DIR)' && find . -type f ! -name '.*' \
	   | sed 's|^\./||' | sort \
	   | COPYFILE_DISABLE=1 tar -czf '$(CURDIR)/$(WEB_LISP_SRC)' -T -
	@printf 'web-lisp-src: %s (%s files, fetched on demand)\n' \
		"$$(du -h '$(WEB_LISP_SRC)' | cut -f1)" \
		"$$(tar -tzf '$(WEB_LISP_SRC)' | wc -l | tr -d ' ')"

web-cedar-src: | web-dir
	@cd ../chm/cedar/stp-root && { \
		find Cedar6.1 -type f ! -iname '*.bcd' \
			! -path 'Cedar6.1/Top/*' ! -path 'Cedar6.1/Viewers/*'; \
		: 'Sil needs these at RUNTIME, so they are bcds and ride here' ; \
		: 'rather than in index.data, which every visitor downloads:' ; \
		: 'its own module bcds (the bound config asks for their type' ; \
		: 'symbols when it opens a viewer) and AMTypes, which does the' ; \
		: 'asking. 0.8 MiB gzipped. Without them the browser desktop' ; \
		: 'restores fine and `Sil ProcH01.sil` then dies on STP_MISSING.' ; \
		: '' ; \
		: 'CedarChest6.0/Top is 48 KB of DFs and is NOT optional: the' ; \
		: 'served creation date of every file comes from its volume Top' ; \
		: 'DF index (ethernet.c ftp_dates_load scans <root>/<vol>/Top),' ; \
		: 'and FSRemoteFileImpl.Retrieve Confirm answers `skip` (STP' ; \
		: 'markNo) for any candidate whose Creation-Date is not the one' ; \
		: 'the local DF pinned. Ship Sil without its DF and the plist' ; \
		: 'carries the synthetic 01-Jan-84 fallback, the guest declines' ; \
		: 'every module bcd, and `Sil ProcH01.sil` hangs with no' ; \
		: 'STP_MISSING to show for it. SHIP A VOLUME, SHIP ITS Top.' ; \
		find CedarChest6.0/Sil CedarChest6.0/Top Cedar6.1/AMTypes -type f; \
	   } | sort | tar -czf '$(CURDIR)/$(WEB_CEDAR_SRC)' -T -
	@printf 'web-cedar-src: %s (%s files, %s directories, fetched on demand)\n' \
		"$$(du -h '$(WEB_CEDAR_SRC)' | cut -f1)" \
		"$$(tar -tzf '$(WEB_CEDAR_SRC)' | wc -l | tr -d ' ')" \
		"$$(tar -tzf '$(WEB_CEDAR_SRC)' | cut -d/ -f2 | sort -u | wc -l | tr -d ' ')"

# Node-hosted wasm32 CLI used only to create ABI-compatible browser snapshots.
# NODERAWFS lets the ordinary dorado CLI read/write the repository paths.
$(WEB_NODE_JS): $(WEB_NODE_SRCS) | $(BINDIR)
	$(EMCC) $(WEB_CFLAGS) $(WEB_NODE_SRCS) \
		-sENVIRONMENT=node -sNODERAWFS=1 -sALLOW_MEMORY_GROWTH=1 \
		-sINITIAL_MEMORY=67108864 -sSTACK_SIZE=1048576 -o $@

# (lisp-lyric-web-snapshot, which baked the plain no-library desktop from
# --ftp-sysout, is retired: the browser Lyric world is the library world
# below.)

# The browser Lyric world: the LIBRARY pack (204 packages + the network
# init), booted from cold under the node-hosted wasm32 binary, because the
# native and wasm32 snapshot ABIs differ while their runs are cycle/data
# deterministic.  Boot flags mirror lisp-lispusers-snapshot exactly; the
# wasm checkpoint is then paired with the NATIVE run's already-flushed pack.
WEB_LISPUSERS_RAW_SNAPSHOT := $(WEB_LISP_RAW_DIR)/lisp-lispusers.snap
WEB_LISPUSERS_RAW_PACK := $(WEB_LISP_RAW_DIR)/lisp-lispusers.pack
# The FULL-library Lyric world for the browser.  Same two-bake shape as
# lisp-lispusers-web-snapshot below: the native and wasm32 snapshot ABIs
# differ, so the wasm checkpoint must be baked by the node build and is then
# paired with the NATIVE run's already-flushed pack.
LISP_FULL_SNAPSHOT ?= build/good-packs/lisp-lyric-full.snap
LISP_FULL_SNAPSHOT_PACK ?= build/good-packs/lisp-lyric-full.pack
LISP_FULL_SNAPSHOT_CYCLES ?= 14000000000
WEB_LISP_FULL_RAW_SNAPSHOT := $(WEB_LISP_RAW_DIR)/lisp-lyric-full.snap
WEB_LISP_FULL_RAW_PACK := $(WEB_LISP_RAW_DIR)/lisp-lyric-full.pack

# Native checkpoint: boot Released-Full.sysout!2 and snapshot at the Exec.
.PHONY: lisp-lyric-full-snapshot
lisp-lyric-full-snapshot: $(BINDIR)/dorado
	@test -r '$(LISP_FULL_GOOD_PACK)' || \
		(echo "Missing $(LISP_FULL_GOOD_PACK) -- run 'make lisp-lyric-full-pack' first." && false)
	mkdir -p '$(dir $(LISP_FULL_SNAPSHOT))'
	cp -f '$(LISP_FULL_GOOD_PACK)' '$(LISP_FULL_SNAPSHOT_PACK)'
	chmod 644 '$(LISP_FULL_SNAPSHOT_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_FULL_SNAPSHOT_PACK)' --boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_RELFULL_SYSOUT_FILE)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_FULL_SNAPSHOT_CYCLES) \
		--snapshot-out '$(LISP_FULL_SNAPSHOT)' \
		--out /tmp/dorado-lisp-lyric-full-snapshot.pgm
	@echo 'lisp-lyric-full-snapshot: expect ~209,000 display-list pixels above.'

# wasm32 checkpoint, paired with the native run's flushed pack.
.PHONY: lisp-lyric-full-web-snapshot
lisp-lyric-full-web-snapshot: $(WEB_NODE_JS)
	@test -r '$(LISP_FULL_SNAPSHOT_PACK)' || \
		(echo "Missing $(LISP_FULL_SNAPSHOT_PACK); run 'make lisp-lyric-full-snapshot' first." && false)
	mkdir -p '$(WEB_LISP_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(LISP_FULL_GOOD_PACK)' '$(WEB_LISP_FULL_RAW_PACK)'
	chmod 644 '$(WEB_LISP_FULL_RAW_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	DORADO_SNAPSHOT_NO_PACK_FLUSH=1 \
	node '$(WEB_NODE_JS)' \
		--eb worlds/aemu.eb \
		--disk 0='$(WEB_LISP_FULL_RAW_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_RELFULL_SYSOUT_FILE)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_FULL_SNAPSHOT_CYCLES) \
		--snapshot-out '$(WEB_LISP_FULL_RAW_SNAPSHOT)'
	gzip -c '$(WEB_LISP_FULL_RAW_SNAPSHOT)' > '$(WEB_LISP_FULL_SNAPSHOT_GZ)'
	gzip -c '$(LISP_FULL_SNAPSHOT_PACK)' > '$(WEB_LISP_FULL_PACK_GZ)'
	@ls -l '$(WEB_LISP_FULL_SNAPSHOT_GZ)' '$(WEB_LISP_FULL_PACK_GZ)'

.PHONY: lisp-lispusers-web-snapshot
lisp-lispusers-web-snapshot: $(WEB_NODE_JS)
	@test -r '$(LISP_LISPUSERS_PACK)' || \
		(echo "Missing $(LISP_LISPUSERS_PACK); run 'make lisp-lispusers-pack LISP_INIT_OVERRIDE=$(LISP_LISPUSERS_DIR)/INIT.DORADO' first." && false)
	@test -r '$(LISP_LISPUSERS_SNAPSHOT_PACK)' || \
		(echo "Missing native flushed pack $(LISP_LISPUSERS_SNAPSHOT_PACK); run 'make lisp-lispusers-snapshot LISP_INIT_OVERRIDE=$(LISP_LISPUSERS_DIR)/INIT.DORADO' first." && false)
	mkdir -p '$(WEB_LISP_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(LISP_LISPUSERS_PACK)' '$(WEB_LISPUSERS_RAW_PACK)'
	chmod 644 '$(WEB_LISPUSERS_RAW_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	DORADO_SNAPSHOT_NO_PACK_FLUSH=1 \
	node '$(WEB_NODE_JS)' \
		--eb worlds/aemu.eb \
		--disk 0='$(WEB_LISPUSERS_RAW_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles '$(LISP_DESKTOP_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(WEB_LISPUSERS_RAW_SNAPSHOT)'
	gzip -c '$(WEB_LISPUSERS_RAW_SNAPSHOT)' > '$(WEB_LISP_SNAPSHOT_GZ)'
	gzip -c '$(LISP_LISPUSERS_SNAPSHOT_PACK)' > '$(WEB_LISP_PACK_GZ)'
	@ls -l '$(WEB_LISP_SNAPSHOT_GZ)' '$(WEB_LISP_PACK_GZ)'

# The compressed browser assets are generated together and intentionally
# checked into/deployed with the web build. Give make a useful diagnostic in a
# checkout where they have not been generated yet.
$(WEB_LISP_SNAPSHOT_GZ) $(WEB_LISP_PACK_GZ):
	@echo "Missing WebAssembly Lyric checkpoint assets."
	@echo "Run 'make lisp-lispusers-web-snapshot' with emsdk on PATH."
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
		--key-hold 1600000 \
		--type-at 2500000000 --type 'Guest\n\n' \
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
		--type-at 36100000000 --click 700,733 \
		--paste-at 36300000000 --paste 'Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack\nBringover -p [Cedar]<CedarChest6.1>Top>Clock\nRun ChessHackImpl\nRun Clock\nClock\nChessHack\nEval ImagerFont.Find["Xerox/TiogaFonts/Chess40"]\n' \
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
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -Ivendor/microui $(DEPFLAGS) -c -o $@ $<

# The front panel and its widget toolkit. microui is vendored (public domain,
# ~1,100 lines of C) rather than depended on, like vendor/6502/fake6502.h --
# the project's rule is C99 with no external libraries, and SDL2 is already
# the frontend's one exception. -Wno-... : microui is not our code and is not
# warning-clean under -Wpedantic.
$(OBJDIR)/ui_panel.o: src/ui_panel.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -Ivendor/microui $(DEPFLAGS) -c -o $@ $<

$(OBJDIR)/microui.o: vendor/microui/microui.c | $(OBJDIR)
	$(CC) $(CFLAGS) -Wno-pedantic -Wno-unused-parameter -Ivendor/microui \
		$(DEPFLAGS) -c -o $@ $<

$(BINDIR)/dorado-sdl: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/typetext.o $(OBJDIR)/ui_panel.o $(OBJDIR)/microui.o $(OBJDIR)/dorado_sdl.o | $(BINDIR)
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

# ---- PGO: the largest single speedup available, and it is free ------------
# Profile-guided optimization on top of -O3 -flto took the Alto path from
# 0.52x to 1.02x real hardware -- 1.95x, byte-identical framebuffer
# (2026-08-04). An interpreter dispatch loop is close to the ideal PGO case:
# the branch pattern is stable and utterly opaque to static heuristics.
#
# It is a separate target, not the default, for three reasons: it needs a
# two-stage build, it leaves a .profdata artifact the tree would have to
# track or regenerate, and CI does not need it. Run it when you want the
# fast local binary; plain `make` stays a one-stage build.
#
# CAVEAT: the profile is trained on ONE world (Galaxian, the Alto path).
# Cross-world measurement showed the Cedar path still gains, so it is not
# pathologically overfit -- but if you are optimizing for Cedar or Lisp,
# retrain on that world by editing PGO_TRAIN below and re-running.
# llvm-profdata: plain on Linux, behind xcrun on macOS. Detected rather
# than hardcoded -- `xcrun` is macOS-only and broke this in CI.
# Ask the COMPILER where its own llvm-profdata is before trusting PATH:
# Ubuntu ships versioned binaries (llvm-profdata-18) and no bare name, which
# is what made this fail in CI. Order matters -- the tool that WROTE the
# .profraw should merge it, or the format version can disagree.
PROFDATA := $(shell \
	p=$$($(CC) -print-prog-name=llvm-profdata 2>/dev/null); \
	if [ -x "$$p" ]; then echo "$$p"; \
	elif command -v llvm-profdata >/dev/null 2>&1; then echo llvm-profdata; \
	elif ls /usr/lib/llvm-*/bin/llvm-profdata >/dev/null 2>&1; then \
		ls /usr/lib/llvm-*/bin/llvm-profdata | tail -1; \
	elif command -v xcrun >/dev/null 2>&1; then echo "xcrun llvm-profdata"; \
	fi)

PGO_PROFRAW := build/pgo/dorado-%p.profraw
PGO_PROFDATA := build/pgo/dorado.profdata
# Train on BOTH shipped paths and merge. Galaxian alone leaves the Cedar
# path -- the PDI disk bridge, the STP server, the Cedar/Mesa microcode --
# entirely unprofiled, and those are exactly where Cedar's remaining
# deficit lives. llvm-profdata merges the two runs' raw profiles.
PGO_TRAIN := --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' \
	--cycles 300000000 --out build/pgo/train.pgm
PGO_TRAIN_CEDAR := --boot-reason disk --no-alto-boot \
	--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
	--pilot-disk '$(CEDAR_DESKTOP_PDI)' --ftp-root ../chm/cedar/stp-root \
	--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' \
	--cycles 36500000000 --out build/pgo/train-cedar.pgm

.PHONY: pgo
pgo:
	@mkdir -p build/pgo
	@# -fprofile-instr-generate is a CLANG spelling; gcc calls it
	@# -fprofile-generate and produces a format emsdk's clang cannot read
	@# anyway. Fail with a sentence rather than a compiler error (CI's
	@# default cc is gcc, which is how this was found).
	@$(CC) -fprofile-instr-generate -E -x c /dev/null -o /dev/null 2>/dev/null || \
		(echo "pgo: $(CC) does not support -fprofile-instr-generate."; \
		 echo "     Use a clang, e.g.  make pgo CC=clang"; false)
	@echo "pgo: stage 1 -- instrumented build"
	@$(MAKE) -s clean-objs
	@# Derive from CFLAGS rather than restating it: the first version
	@# hardcoded a subset, dropped -D_GNU_SOURCE, and broke on glibc where
	@# timegm/strcasecmp need it (clang makes those an error, not a warning).
	@$(MAKE) -s $(BINDIR)/dorado \
		CFLAGS="$(CFLAGS) -fprofile-instr-generate"
	@echo "pgo: stage 2 -- training runs (Alto, then Cedar if its checkpoint is present)"
	@rm -f build/pgo/*.profraw
	@LLVM_PROFILE_FILE='$(PGO_PROFRAW)' $(BINDIR)/dorado $(PGO_TRAIN) >/dev/null 2>&1
	@if [ -r '$(CEDAR_DESKTOP_SNAPSHOT)' ] && [ -r '$(CEDAR_DESKTOP_PDI)' ]; then \
		LLVM_PROFILE_FILE='$(PGO_PROFRAW)' DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
		DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' \
		$(BINDIR)/dorado $(PGO_TRAIN_CEDAR) >/dev/null 2>&1 || true; \
	else \
		echo "pgo: no Cedar checkpoint -- training on the Alto path only"; \
	fi
	@test -n '$(PROFDATA)' || \
		(echo "pgo: no llvm-profdata found (install llvm, or use xcrun on macOS)"; false)
	@$(PROFDATA) merge -output='$(PGO_PROFDATA)' build/pgo/*.profraw
	@echo "pgo: stage 3 -- optimized rebuild (headless AND the SDL frontend)"
	@$(MAKE) -s clean-objs
	@$(MAKE) -s $(BINDIR)/dorado \
		CFLAGS="$(CFLAGS) -fprofile-instr-use=$(PGO_PROFDATA)"
	@# The SDL binary is the one a person actually watches. Building only
	@# the headless one meant `make pgo` doubled a number nobody could see.
	@$(MAKE) -s $(BINDIR)/dorado-sdl \
		CFLAGS="$(CFLAGS) -fprofile-instr-use=$(PGO_PROFDATA)" || \
		echo "pgo: SDL frontend not built (SDL2 missing?) -- headless is done"
	@echo "pgo: done. Verify byte-identical output before trusting it:"
	@echo "  cmp two .pgm files from the same run pre/post."

# Object files only -- never touches build/good-packs.
.PHONY: clean-objs
clean-objs:
	@rm -f $(OBJDIR)/*.o $(BINDIR)/dorado $(BINDIR)/dorado-sdl

clean:
	@# NEVER remove build/good-packs. dorado/CLAUDE.md tells you to copy
	@# every expensive artifact (checkpoints, packs, baked images) there
	@# BECAUSE it is gitignored but persists -- and `rm -rf build` deleted
	@# the lot on 2026-08-04 during a compiler-flag sweep, taking the only
	@# native Lyric checkpoint with it (the tracked web-assets copies are
	@# wasm32 and cannot substitute). The instruction and the tool
	@# contradicted each other; the tool was wrong.
	@if [ -d '$(OBJDIR)' ]; then \
		find '$(OBJDIR)' -mindepth 1 -maxdepth 1 ! -name good-packs \
			-exec rm -rf {} + ; \
	fi

# Deliberate: wipes good-packs too. Only for reclaiming disk, never as a
# step in a build or benchmark loop.
.PHONY: clean-everything
clean-everything:
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

# Mesa world (AltoMesaDorado.eb!2): Mesa-format outloads (0o162400). These
# will NOT run on worlds/aemu.eb -- booting a Mesa outload on the Alto
# emulator is the same category error as hosting Smalltalk there.
#
# MesaNetExec reaches its `>` prompt near 300M cycles; type at it directly.
# MESA_BOOT overrides the boot file, e.g.
#   make run-mesa MESA_BOOT='../chm/bootfiles/PPong.boot!1'
MESA_WORLD ?= ../chm/dorado/AltoMesaDorado.eb!2
MESA_BOOT  ?= ../chm/bootfiles/MesaNetExec.boot!1

.PHONY: run-mesa run-mesa-netexec run-ppong run-mazewar run-pupwatch run-triex
run-mesa: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' --eftp '$(MESA_BOOT)'
run-mesa-netexec: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/MesaNetExec.boot!1'
run-ppong: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/PPong.boot!1'
run-mazewar: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/MazeWar.boot!1'
run-pupwatch: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/Pupwatch.boot!1'
run-triex: sdl
	./build/dorado-sdl --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/TriEx.boot!1'

# ---------------------------------------------------------------------------
# Input-fidelity gates.
#
# Every other gate types with --key-hold between 1,000,000 and 4,000,000
# cycles and never moves the mouse. A display field is 277,778 cycles, so
# those gates hold a key 4-14x LONGER than the world's sampling interval,
# while a browser keystroke is SHORTER than one -- and a human moves the
# pointer constantly. That gap is not academic: on 2026-07-30 three separate
# input bugs shipped green (Mesa mouse-cell corruption, dropped browser
# keystrokes, an SDL pacing stall), because no gate drove input the way a
# person does.
#
# So: move the pointer, and type with a hold SHORTER than one field.
KEY_HOLD_FAST ?= 120000

.PHONY: verify-input
verify-input: $(BINDIR)/dorado
	@px=`./build/dorado --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/MesaNetExec.boot!1' \
		--type-at 120000000 --mouse 400,300 \
		--type-at 200000000 --type "abc" --key-hold '$(KEY_HOLD_FAST)' \
		--cycles 320000000 --out build/verify-input-mesa.pgm 2>/dev/null \
		| sed -n 's/.*: \([0-9][0-9]*\) display-list pixels.*/\1/p' | tail -1`; \
	echo "verify-input mesa (mouse + $(KEY_HOLD_FAST)-cycle keys): $$px px"; \
	echo "  (1250 = the 2026-07-30 mouse-corruption bug; 1573 = healthy)"; \
	test "$$px" -ge 1400

# ---------------------------------------------------------------------------
# Interlisp-D Lyric: restore the saved XCL desktop and confirm it is live.
# Lyric was broken and unnoticed for two weeks (2026-07-11..07-30, the
# oversized BSP data Pup) precisely because nothing gated it.
LISP_GATE_MIN_PX ?= 150000

.PHONY: verify-lisp
verify-lisp: $(BINDIR)/dorado
	@test -r '$(LISP_DESKTOP_SNAPSHOT)' || \
		(echo "Missing $(LISP_DESKTOP_SNAPSHOT); run make lisp-lyric-desktop-snapshot first." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_DESKTOP_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-gate.pack'
	@px=`DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_DESKTOP_SNAPSHOT)' \
		--cycles 9000000000 --out build/verify-lisp.pgm 2>/dev/null \
		| sed -n 's/.*: \([0-9][0-9]*\) display-list pixels.*/\1/p' | tail -1`; \
	echo "verify-lisp: $$px px (need >= $(LISP_GATE_MIN_PX); desktop = ~208985)"; \
	test "$$px" -ge '$(LISP_GATE_MIN_PX)'

# The same desktop, clicked the way a person clicks it. verify-lisp above only
# proves Lyric boots; it never touches the machine, and the 2026-07-30 IOPage
# corruption lived entirely on the interaction path.
#
# Hold duration is the whole point. --click's default --key-hold is 600,000
# cycles = 36 ms, shorter than any human click, and the bug does not fire:
# 2,000,000 (120 ms) leaves the desktop clean, 3,000,000 (180 ms) puts Lisp in
# RAID. That is exactly why this was reproducible in the browser and not in a
# single gate for two weeks. Click at (250,50) = the Prompt Window.
#
# Validated against its own bug: 191,601 px at 5673515 (RAID "Compiler/
# microcode error: unknown UFN"), 208,966 px with the IOPage write retired.
LISP_CLICK_GATE_MIN_PX ?= 200000
LISP_CLICK_HOLD ?= 3000000

.PHONY: verify-lisp-click
verify-lisp-click: $(BINDIR)/dorado
	@test -r '$(LISP_DESKTOP_SNAPSHOT)' || \
		(echo "Missing $(LISP_DESKTOP_SNAPSHOT); run make lisp-lyric-desktop-snapshot first." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_DESKTOP_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-click-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-click-gate.pack'
	@px=`DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-click-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_DESKTOP_SNAPSHOT)' \
		--type-at 8850000000 --click 250,50 \
		--key-hold '$(LISP_CLICK_HOLD)' \
		--cycles 9300000000 --out build/verify-lisp-click.pgm 2>/dev/null \
		| sed -n 's/.*: \([0-9][0-9]*\) display-list pixels.*/\1/p' | tail -1`; \
	echo "verify-lisp-click: $$px px (need >= $(LISP_CLICK_GATE_MIN_PX); healthy = 208966, RAID = 191601)"; \
	test "$$px" -ge '$(LISP_CLICK_GATE_MIN_PX)'

# ---------------------------------------------------------------------------
# Serving a hand-written file to Interlisp.
#
# Interlisp names everything we serve it at the ROOT of the tree --
# {DORADO}<>NAME -- and spells the STP command plist in upper case:
#   ((USER-NAME Guest)(USER-PASSWORD Guest)(DIRECTORY )(NAME-BODY PACMANFIX)...)
# eth_ftp_resolve_file used to read that with a case-sensitive strstr and to
# reject an empty Directory, so a retrieve our own STP LOOKUP had just
# resolved came back fileNotFound. Nothing gated it because every file we had
# ever served Lisp came from the pack or over Leaf.
#
# The probe is PACMANFIX (see docs/parc-feedback-todo.md section I), which is
# the only extensionless file in the served root. Read the wire, not pixels:
# a FILESLOAD that fails prints nothing distinctive.
#
# Validated against its own bug: before the fix the log holds
#   STP_LOOKUP PACMANFIX -> !1 2927 bytes
#   STP_MISSING ((USER-NAME Guest)...(DIRECTORY )(NAME-BODY PACMANFIX)...)
# and after it, STP_SERVE / STP_TRANSFER.
.PHONY: verify-lisp-serve
verify-lisp-serve: $(BINDIR)/dorado
	@test -r '$(LISP_FULL_SNAPSHOT)' || \
		(echo "Missing $(LISP_FULL_SNAPSHOT). Run 'make lisp-lyric-full-snapshot' once." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_FULL_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-serve-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-serve-gate.pack'
	@DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		DORADO_FTP_TRACE=1 \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-serve-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_FULL_SNAPSHOT)' \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--type-at 14050000000 --click 250,50 --key-hold 2000000 \
		--type-at 14150000000 --type "(IL:FILESLOAD PACMANFIX)\n" \
		--type-at 16200000000 --type "Guest\n" \
		--type-at 17400000000 --type "Guest\n" \
		--cycles 19500000000 --out build/verify-lisp-serve.pgm \
		> build/verify-lisp-serve.log 2>&1
	@grep -q '^STP_SERVE PACMANFIX' build/verify-lisp-serve.log && \
	 grep -q '^STP_TRANSFER PACMANFIX' build/verify-lisp-serve.log && \
	 echo "verify-lisp-serve: the root-level extensionless file was served over STP" || \
	 (echo "verify-lisp-serve: FAIL --"; \
	  grep -E '^STP_(LOOKUP|MISSING|SERVE)' build/verify-lisp-serve.log | head -5; false)

# ---------------------------------------------------------------------------
# An Interlisp pop-up menu you can actually navigate.
#
# The reported symptom (A3) was "you always get the first item and we can't
# even see it": the pointer merely entering the menu took an item and closed
# it. Root cause was two writers of the UTILIN cell -- see the comment in
# dorado_machine_set_mouse -- so the guest sampled all-buttons-up a few times
# a second while the button was held.
#
# The gate presses the RIGHT button on the background, dwells until the menu
# is up, travels onto TEdit (an item with no submenu) and measures whether
# that ROW IS INVERTED, which is Interlisp's "this item is selected, waiting
# for your release". A full-screen pixel count cannot see this: the menu-up
# and menu-gone screens are within 0.2% of each other.
#
# Validated against its own bug: 297/1066 black before the fix (menu already
# gone, TEdit's default action fired), 893/1066 after (row inverted, menu
# still up). Both readings are stable across trials.
LISP_MENU_MIN_INK ?= 700

.PHONY: verify-lisp-menu
verify-lisp-menu: $(BINDIR)/dorado
	@test -r '$(LISP_FULL_SNAPSHOT)' || \
		(echo "Missing $(LISP_FULL_SNAPSHOT). Run 'make lisp-lyric-full-snapshot' once." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_FULL_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-menu-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-menu-gate.pack'
	@DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-menu-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_FULL_SNAPSHOT)' \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--menu-button right --type-at 14400000000 \
		--drag 740,495,770,477 --drag-dwell 30000000 --drag-hold 60000000 \
		--cycles 14495000000 --out build/verify-lisp-menu.pgm >/dev/null 2>&1
	@ink=`python3 ../tools/pgm_ink.py build/verify-lisp-menu.pgm 742 471 824 484`; \
	echo "verify-lisp-menu: TEdit row ink $$ink/1066 (need >= $(LISP_MENU_MIN_INK);"; \
	echo "  893 = item selected with the menu still up, 297 = menu already collapsed)"; \
	test "$$ink" -ge '$(LISP_MENU_MIN_INK)'

# ---------------------------------------------------------------------------
# Control as a MODIFIER, driven the way a frontend drives it.
#
# "The Control key does not work" was reported by a PARC veteran (A1 in
# docs/parc-feedback-todo.md) and nothing gated it. --type's control-code path
# is not the interesting one: it is self-contained. What matters is
# dorado_machine_set_key(CTRL, 1) held as a LEVEL across another key, which is
# exactly what dorado_sdl.c and dorado_web.c do, and what --key-chord does.
#
# The probe is Interlisp's line editor: at the Exec, Ctrl-W deletes the
# previous word. Type "abcdef ghijkl", then Ctrl-W, then measure the ink in
# the Exec's first line. Ink, not a full-screen pixel count -- the screen is
# 209,000 px and one word is 145 of them.
#
# Validated against its own bug, both directions:
#   405 ink  no chord at all         ("abcdef ghijkl")
#   439 ink  --key-chord w           (a 'w' appended -- key delivered, no
#                                     modifier: the exact failure this gate
#                                     exists to catch)
#   260 ink  --key-chord ctrl,w      (word deleted -- Control held as a level)
# so the threshold sits below all three of the failure readings.
LISP_CTRL_MAX_INK ?= 330

.PHONY: verify-ctrl
verify-ctrl: $(BINDIR)/dorado
	@test -r '$(LISP_FULL_SNAPSHOT)' || \
		(echo "Missing $(LISP_FULL_SNAPSHOT). Run 'make lisp-lyric-full-snapshot' once." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_FULL_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-ctrl-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-ctrl-gate.pack'
	@DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-ctrl-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_FULL_SNAPSHOT)' \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--type-at 14050000000 --click 250,50 --key-hold 3000000 \
		--type-at 14200000000 --type "abcdef ghijkl" --key-hold 4000000 \
		--type-at 14450000000 --key-chord ctrl,w --key-hold 4000000 \
		--cycles 14560000000 --out build/verify-ctrl.pgm >/dev/null 2>&1
	@ink=`python3 ../tools/pgm_ink.py build/verify-ctrl.pgm 0 256 320 276`; \
	echo "verify-ctrl: Exec line ink $$ink (need <= $(LISP_CTRL_MAX_INK);"; \
	echo "  260 = Ctrl-W deleted the word, 405 = chord ignored, 439 = 'w' with no Control)"; \
	test "$$ink" -le '$(LISP_CTRL_MAX_INK)'

# Headless gate for the Mesa world: herald + prompt, and typing echoes.
.PHONY: verify-mesa
verify-mesa: $(BINDIR)/dorado
	@idle=$$(./build/dorado --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/MesaNetExec.boot!1' \
		--cycles 300000000 --out build/verify-mesa.pgm 2>/dev/null \
		| sed -n 's/.*: \([0-9]*\) display-list pixels.*/\1/p' | tail -1); \
	typed=$$(./build/dorado --eb '$(MESA_WORLD)' \
		--eftp '../chm/bootfiles/MesaNetExec.boot!1' \
		--type "abc" --type-at 160000000 --key-hold 4000000 \
		--cycles 300000000 --out build/verify-mesa-typed.pgm 2>/dev/null \
		| sed -n 's/.*: \([0-9]*\) display-list pixels.*/\1/p' | tail -1); \
	echo "verify-mesa: idle $$idle px, after typing abc $$typed px"; \
	test "$$idle" -ge 1200 && test "$$typed" -gt "$$idle"

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

# ---- Colour: the Dorado's SECOND screen (DispM) --------------------------
#
# The Dorado's black-and-white display is DispY -- 1024x808, ONE BIT PER PIXEL.
# Colour is a physically separate board, DispM, driving its own 640x480 or
# 1024x768 monitor. So `make run-cedar-color` opens TWO windows: the mono
# desktop, and a second window for the colour screen. That is what the guest
# believes too -- Cedar's `ColorDisplay left | right` command asks which side
# of the b/w display the colour one sits on.
#
# THE BOARD MUST BE PRESENT AT BOOT. ColorDisplayHeadDorado's presence test
# lives in its module initialization body, so it runs once when the
# configuration is STARTed and never again. A checkpoint baked without the
# board has already latched displayType _ none, and no amount of typing
# ColorDisplay on will change its mind -- which is why this has its own
# checkpoint rather than reusing cedar-desktop.
CEDAR_COLOR_SNAPSHOT := build/good-packs/cedar-color.snap
CEDAR_COLOR_PDI      := build/good-packs/cedar-color.pdi

.PHONY: run-cedar-color
run-cedar-color: sdl $(CEDAR_COLOR_SNAPSHOT) $(CEDAR_COLOR_PDI)
	@echo "Cedar 6.1 with the colour board (DispM). Two windows: the mono"
	@echo "desktop, and a second one for the colour screen."
	@echo "NOTE: this is a restored checkpoint; it may contain pre-merge 6.0"
	@echo "      BiScrollers already loaded in Cedar. To validate the merged"
	@echo "      CedarChest 6.1 package, use: make run-cedar-color-cold"
	@echo
	@echo "The colour screen starts BLACK and that is correct -- the display is"
	@echo "on, its palette is loaded (255 of 1024 ATable entries), but nothing"
	@echo "has been placed on it, so every pixel indexes ATable[0] = black."
	@echo "To get a picture, something has to be MOVED THERE. In the CommandTool:"
	@echo "  ColorDisplay ?        report the mode (expect: on, 8 bpp, left, 640x480)"
	@echo "  ColorDisplay left     put the colour screen left of the b/w one"
	@echo "  ColorDisplay 8        8 bits/pixel; try 4, 2, 1, 24 as well"
	@echo "  ColorDisplay dither   dithered rather than flat 8bpp"
	@echo "There is also a Color button in the message window (top right):"
	@echo "left-click toggles, right-click gives the options menu."
	@cp -f '$(CEDAR_COLOR_PDI)' build/run-cedar-color.pdi
	DORADO_DISPM_COLOR=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	./build/dorado-sdl --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/run-cedar-color.pdi \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_COLOR_SNAPSHOT)' --speed 4000000

.PHONY: run-cedar-color-cold
run-cedar-color-cold: sdl $(CEDAR_WORK_PDI)
	@echo "Cold boot with the colour board. COLD because User.Profile's"
	@echo "BootCommands only run at boot -- a restored checkpoint still has"
	@echo "the modules it was baked with, which is the whole BiScrollers issue."
	@echo "Starting from the clean CedarDorado-work volume (CedarChest 6.1 only)."
	@echo
	@echo "  1. log in:      type  Guest  then Return twice"
	@echo "  2. CHECK SIL:   click the  Sil  button in the CommandTool menu"
	@echo "                  line (or type: Sil ProcH01.sil). It must still"
	@echo "                  open the ProcH drawing -- the profile now loads"
	@echo "                  BiScrollers and Sil from CedarChest6.1, and Sil"
	@echo "                  is what that change puts at risk."
	@echo "  3. CHECK GARGOYLE: type  Color.cm  then, when it finishes,"
	@echo "                  Gargoyle. Watch for VersionMismatch[BiScrollers]"
	@echo "                  -- its absence is the fix working."
	@cp -f '$(CEDAR_WORK_PDI)' build/run-cedar-color-cold.pdi
	DORADO_DISPM_COLOR=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	./build/dorado-sdl --boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/run-cedar-color-cold.pdi \
		--ftp-root ../chm/cedar/stp-root --speed 4000000

# ---- Saved interactive colour-session checkpoints ------------------------
# These are the three states captured from the SDL session on 2026-08-09:
# before ColorDisplay, after the ColorDisplay controls were loaded, and after
# `ColorDisplay on`.  They share the disk state at the time of the session.
# The raw files live in build/good-packs when expanded; the compressed pairs
# in snapshot-assets are the durable, ABI-specific artifacts.
CEDAR_COLOR_SESSION_PDI := build/good-packs/cedar-6.1-color-session.pdi
CEDAR_COLOR_SESSION_PDI_GZ := snapshot-assets/cedar-6.1-color-session-native.pdi.gz
CEDAR_COLOR_BEFORE_SNAPSHOT := build/good-packs/cedar-6.1-color-before.snap
CEDAR_COLOR_BEFORE_SNAPSHOT_GZ := snapshot-assets/cedar-6.1-color-before-native.snap.gz
CEDAR_COLOR_CONTROLS_SNAPSHOT := build/good-packs/cedar-6.1-color-controls.snap
CEDAR_COLOR_CONTROLS_SNAPSHOT_GZ := snapshot-assets/cedar-6.1-color-controls-native.snap.gz
CEDAR_COLOR_ON_SNAPSHOT := build/good-packs/cedar-6.1-color-on.snap
CEDAR_COLOR_ON_SNAPSHOT_GZ := snapshot-assets/cedar-6.1-color-on-native.snap.gz

$(CEDAR_COLOR_SESSION_PDI): $(CEDAR_COLOR_SESSION_PDI_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_COLOR_BEFORE_SNAPSHOT): $(CEDAR_COLOR_BEFORE_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_COLOR_CONTROLS_SNAPSHOT): $(CEDAR_COLOR_CONTROLS_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'
$(CEDAR_COLOR_ON_SNAPSHOT): $(CEDAR_COLOR_ON_SNAPSHOT_GZ)
	mkdir -p '$(dir $@)'
	gunzip -c '$<' > '$@'

define RUN_CEDAR_SAVED_COLOR
	DORADO_DISPM_COLOR=1 DORADO_COLOR_SIDE=right \
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 ./build/dorado-sdl \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_COLOR_SESSION_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$1' --speed 4000000
endef

.PHONY: run-cedar-color-before run-cedar-color-controls run-cedar-color-on
run-cedar-color-before: sdl $(CEDAR_COLOR_BEFORE_SNAPSHOT) $(CEDAR_COLOR_SESSION_PDI)
	@echo "Cedar 6.1 colour session: before ColorDisplay was loaded"
	$(call RUN_CEDAR_SAVED_COLOR,$(CEDAR_COLOR_BEFORE_SNAPSHOT))

run-cedar-color-controls: sdl $(CEDAR_COLOR_CONTROLS_SNAPSHOT) $(CEDAR_COLOR_SESSION_PDI)
	@echo "Cedar 6.1 colour session: ColorDisplay controls loaded, before on"
	$(call RUN_CEDAR_SAVED_COLOR,$(CEDAR_COLOR_CONTROLS_SNAPSHOT))

run-cedar-color-on: sdl $(CEDAR_COLOR_ON_SNAPSHOT) $(CEDAR_COLOR_SESSION_PDI)
	@echo "Cedar 6.1 colour session: ColorDisplay on"
	$(call RUN_CEDAR_SAVED_COLOR,$(CEDAR_COLOR_ON_SNAPSHOT))

# Cold-boot a fresh colour checkpoint: board installed from the first
# instruction, log in, install the ColorDisplay package, turn it on. ~25 min.
# Writes to good-packs directly, so back up first if you care about the old one.
.PHONY: cedar-color-snapshot
cedar-color-snapshot: $(BINDIR)/dorado $(CEDAR_WORK_PDI)
	mkdir -p build/good-packs
	cp -f '$(CEDAR_WORK_PDI)' '$(CEDAR_COLOR_PDI)'
	DORADO_DISPM_COLOR=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_COLOR_PDI)' \
		--ftp-root ../chm/cedar/stp-root --key-hold 1600000 \
		--type-at 2500000000 --type 'Guest\n\n' \
		--type-at 38000000000 --click 700,733 \
		--paste-at 38500000000 --paste 'Bringover -p [Cedar]<CedarChest6.1>Top>ColorDisplay.df\n' \
		--paste-at 52000000000 --paste 'Run ImagerGrayImpl\n' \
		--paste-at 55000000000 --paste 'Run ImagerForkContextImpl\n' \
		--paste-at 58000000000 --paste 'Run ImagerMaskContextImpl\n' \
		--paste-at 61000000000 --paste 'Run ColorDisplayImpl\n' \
		--paste-at 65000000000 --paste 'ColorDisplay on\n' \
		--cycles 80000000000 \
		--snapshot-out '$(CEDAR_COLOR_SNAPSHOT)' \
		--out build/cedar-color.pgm
	@echo "baked $(CEDAR_COLOR_SNAPSHOT); check build/cedar-color.pgm is a desktop"

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
CEDAR_GATE_CYCLES ?= 45000000000

.PHONY: verify-cedar-gate-selftest
verify-cedar-gate-selftest:
	@echo "gate self-test: a log with missing files must FAIL"
	@printf 'STP_MISSING Cedar6.1/Viewers/ViewerOps.bcd\ndorado: 168952 display-list pixels\n' > build/gate-bad.log
	@if ../tools/check_cedar_gate.sh build/gate-bad.log 200000 >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the gate passed a bad log"; exit 1; fi
	@echo "gate self-test: a SOLID BLACK screen must FAIL (too much ink)"
	@# The failure mode a minimum-only gate cannot see. Inverted AIS images
	@# rendered a black square and passed for weeks, because solid black is
	@# MORE pixels, not fewer. See tools/check_cedar_gate.sh.
	@printf 'dorado: 500000 display-list pixels\n' > build/gate-black.log
	@if ../tools/check_cedar_gate.sh build/gate-black.log 200000 >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the gate passed an all-black screen"; exit 1; fi
	@echo "gate self-test: a clean painted log must PASS"
	@printf 'dorado: 245822 display-list pixels\n' > build/gate-good.log
	@../tools/check_cedar_gate.sh build/gate-good.log 200000 >/dev/null \
		|| (echo "SELFTEST FAIL: the gate failed a good log"; exit 1)
	@rm -f build/gate-bad.log build/gate-good.log build/gate-black.log
	@echo "gate self-test: PASS (fails bad logs, passes good ones)"

# ---- Did a pasted command reach the guest INTACT? ------------------------
#
# Built after three ~45-minute Gargoyle runs died on the same typo and it was
# only visible at the end, by converting a framebuffer to PNG and reading it.
# This answers the same question in a few minutes.
#
# It reads the WIRE, not the screen. A correct Bringover makes the guest ask
# our STP server for DoradoWelcome.df, which logs STP_SERVE; a corrupted one
# is rejected by the CommandTool and produces no network traffic at all. That
# needs no font table -- pgm_text.py knows the Interlisp font, not Cedar's
# Tioga -- and it is the same reasoning as verify-cedar-ls.
CEDAR_PASTE_SETTLE ?= 36100000000

.PHONY: verify-paste
verify-paste: $(BINDIR)/dorado $(CEDAR_COLOR_SNAPSHOT) $(CEDAR_COLOR_PDI)
	@echo "verify-paste: does a pasted command survive the trip to the guest?"
	@cp -f '$(CEDAR_COLOR_PDI)' build/verify-paste.pdi
	@DORADO_FTP_TRACE=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/verify-paste.pdi \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_COLOR_SNAPSHOT)' --key-hold 1600000 \
		--type-at $(CEDAR_PASTE_SETTLE) --click 700,733 \
		--paste-at $$(($(CEDAR_PASTE_SETTLE)+200000000)) \
		--paste 'Bringover [Cedar]<CedarChest6.1>Top>DoradoWelcome\n' \
		--cycles $$(($(CEDAR_PASTE_SETTLE)+10000000000)) \
		--out build/verify-paste.pgm > build/verify-paste.log 2>&1
	@../tools/check_paste_gate.sh build/verify-paste.log

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
		--type-at 36100000000 --click 700,733 \
		--paste-at 36300000000 --paste 'Moon.cm\n' \
		--cycles '$(CEDAR_GATE_CYCLES)' \
		--out build/verify-cedar.pgm > build/verify-cedar.log 2>&1
	@../tools/check_cedar_gate.sh build/verify-cedar.log '$(CEDAR_GATE_MIN_PX)'

# ---- Cedar remote-listing gate --------------------------------------------
# "ls" on a release directory. Before 2026-07-30 our STP server had no
# Enumerate at all: mark 0o14 (New-Directory) was aliased onto Retrieve and
# mark 0o12 (the old Directory) was not decoded, so a listing printed nothing
# -- silently, because FSRemoteFileImpl.InnerEnumerate swallows noSuchFile.
#
# The gate reads both ends of the wire out of one trace: STP_ENUM (we produced
# entries) and STP_LOOKUP (the guest asked us about those names afterwards,
# which it can only do having received them). See tools/check_cedar_ls_gate.sh
# for why a pixel count would NOT catch this.
CEDAR_LS_CYCLES ?= 45000000000

.PHONY: verify-cedar-ls
verify-cedar-ls: $(BINDIR)/dorado $(CEDAR_DESKTOP_SNAPSHOT) $(CEDAR_DESKTOP_PDI)
	@echo "verify-cedar-ls: listing [Cedar]<Cedar6.1>VersionMap> from the desktop"
	@cp -f '$(CEDAR_DESKTOP_PDI)' build/verify-cedar-ls.pdi
	@DORADO_FTP_TRACE=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/verify-cedar-ls.pdi \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' --key-hold 1600000 \
		--type-at 36100000000 --click 700,733 \
		--paste-at 36300000000 --paste 'List [Cedar]<Cedar6.1>VersionMap>*\n' \
		--cycles '$(CEDAR_LS_CYCLES)' \
		--out build/verify-cedar-ls.pgm > build/verify-cedar-ls.log 2>&1
	@../tools/check_cedar_ls_gate.sh build/verify-cedar-ls.log

# The listing gate judged against its own bug: a log with the pre-2026-07-30
# server's answer (markNo, no entries, no lookups) must FAIL.
.PHONY: verify-cedar-ls-selftest
verify-cedar-ls-selftest:
	@echo "ls gate self-test: the old server's log must FAIL"
	@printf 'FTP_CMD mark=0o14 len=148\nSTP_MISSING ((User-Name Guest.pa)(Directory Cedar6.1)(Name-Body VersionMap>*)\ndorado: 167129 display-list pixels\n' > build/ls-gate-bad.log
	@if ../tools/check_cedar_ls_gate.sh build/ls-gate-bad.log >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the gate passed the broken server's log"; exit 1; fi
	@echo "ls gate self-test: a listing that never reached the guest must FAIL"
	@{ printf 'STP_ENUM <Cedar6.1>VersionMap>* -> 11 files\n'; \
	   for n in CedarSource.VersionMap CedarSymbols.VersionMap \
	            VersionMap.mesa MakeVersionMap.cm; do \
	     printf 'STP_ENUM_ENTRY ((Name-Body %s)(Size 1))\n' "$$n"; done; \
	   printf 'dorado: 167129 display-list pixels\n'; } > build/ls-gate-noclient.log
	@if ../tools/check_cedar_ls_gate.sh build/ls-gate-noclient.log >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the gate passed a listing with no client half"; exit 1; fi
	@echo "ls gate self-test: a good log must PASS"
	@{ printf 'STP_ENUM <Cedar6.1>VersionMap>* -> 11 files\n'; \
	   for n in CedarSource.VersionMap CedarSymbols.VersionMap \
	            VersionMap.mesa MakeVersionMap.cm; do \
	     printf 'STP_ENUM_ENTRY ((Name-Body %s)(Size 1))\n' "$$n"; done; \
	   for i in 1 2 3 4 5 6 7 8; do \
	     printf 'STP_LOOKUP Cedar6.1/VersionMap/File%s -> !1 1 bytes x\n' "$$i"; done; \
	   printf 'dorado: 167129 display-list pixels\n'; } > build/ls-gate-good.log
	@../tools/check_cedar_ls_gate.sh build/ls-gate-good.log >/dev/null \
		|| (echo "SELFTEST FAIL: the gate failed a good log"; exit 1)
	@rm -f build/ls-gate-bad.log build/ls-gate-noclient.log build/ls-gate-good.log
	@echo "ls gate self-test: PASS (fails both half-broken logs, passes a good one)"

# ---- Sil: the Dorado draws its own board ----------------------------------
# Clicking the CommandTool's Sil button opens ProcH01.sil -- Ed Clark's 1981
# processor-board drawing, the VECTOR original rather than the AIS scan
# Schematic.cm paints -- in Sil, the CedarChest 6.0 drawing editor. This
# covers a chain nothing else touches: the CedarChest 6.0 Bringover/Run
# block in the served User.Profile, the 32 ProcH sheets on the volume, and
# the CreateButton that puts Sil in the menu line. It went unshipped in the
# browser for three days precisely because no gate looked (see
# docs/sil-schematics-handoff.md section 1).
#
# The click coordinate is MEASURED, not guessed: the Sil button spans
# x 728-740, y 29-36 on the restored desktop (an ink-column scan of the
# menu row). Guessing cost most of a session.
#
# SLOW -- about 25 minutes, since Sil parses and paints 6.5 KB of vectors.
# Not part of `make test`.
CEDAR_SIL_CYCLES ?= 40000000000

.PHONY: verify-cedar-sil
verify-cedar-sil: $(BINDIR)/dorado $(CEDAR_DESKTOP_SNAPSHOT) $(CEDAR_DESKTOP_PDI)
	@echo "verify-cedar-sil: clicking the Sil button to open ProcH01.sil (~25 min)"
	@cp -f '$(CEDAR_DESKTOP_PDI)' build/verify-cedar-sil.pdi
	@DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk build/verify-cedar-sil.pdi \
		--ftp-root ../chm/cedar/stp-root \
		--snapshot-in '$(CEDAR_DESKTOP_SNAPSHOT)' --key-hold 1600000 \
		--type-at 36100000000 --click 734,32 \
		--cycles '$(CEDAR_SIL_CYCLES)' \
		--out build/verify-cedar-sil.pgm > build/verify-cedar-sil.log 2>&1
	@../tools/check_cedar_sil_gate.sh build/verify-cedar-sil.log

.PHONY: verify-cedar-sil-selftest
verify-cedar-sil-selftest:
	@mkdir -p build
	@printf 'dorado: 167653 display-list pixels\n' > build/sil-gate-noclick.log
	@if ../tools/check_cedar_sil_gate.sh build/sil-gate-noclick.log >/dev/null 2>&1; \
	 then echo "SELFTEST FAIL: passed an untouched desktop"; exit 1; fi
	@printf 'dorado: 28703 display-list pixels\n' > build/sil-gate-cold.log
	@if ../tools/check_cedar_sil_gate.sh build/sil-gate-cold.log >/dev/null 2>&1; \
	 then echo "SELFTEST FAIL: passed a cold-booted login screen"; exit 1; fi
	@printf 'dorado: 89614 display-list pixels\n' > build/sil-gate-good.log
	@../tools/check_cedar_sil_gate.sh build/sil-gate-good.log >/dev/null \
		|| (echo "SELFTEST FAIL: the gate failed a good log"; exit 1)
	@rm -f build/sil-gate-noclick.log build/sil-gate-cold.log build/sil-gate-good.log
	@echo "sil gate self-test: PASS (rejects both real failure modes)"

# ---- Snapshot ABI: are the baked checkpoints still loadable? --------------
# dorado_machine_restore refuses any checkpoint whose header struct sizes
# disagree with the running build, so a struct that grows kills every
# checkpoint baked before it. On 2026-08-04 the Leaf ethernet growth
# (e5ecba1, +66,496 bytes) was found to have left BOTH saved-login
# checkpoints dead for a week -- the browser answered "snapshot restore
# rejected the saved image" and run-cedar-snapshot-sdl was broken -- because
# every Cedar gate restores the DESKTOP and none touches a login checkpoint.
#
# This reads the sizes out of each header, so it costs a second and boots
# nothing. Run it after ANY change to a struct that a snapshot serializes.
# The sizes are per-ABI: web-assets are wasm32 and are checked against the
# node build, skipped when emsdk is not on PATH.
.PHONY: verify-snapshot-abi
verify-snapshot-abi: $(BINDIR)/dorado
	@echo "verify-snapshot-abi: native checkpoints vs this build"
	@./build/dorado --print-abi > build/abi-native.txt
	@python3 ../tools/check_snapshot_abi.py --abi build/abi-native.txt \
		snapshot-assets
	@if [ -r '$(WEB_NODE_JS)' ] && \
	    node '$(WEB_NODE_JS)' --print-abi > build/abi-wasm.txt 2>/dev/null && \
	    grep -q sz_machine build/abi-wasm.txt; then \
		echo "verify-snapshot-abi: wasm32 checkpoints vs the node build"; \
		python3 ../tools/check_snapshot_abi.py --abi build/abi-wasm.txt \
			web-assets; \
	else \
		echo "  (skipping web-assets: $(WEB_NODE_JS) is missing or predates"; \
		echo "   --print-abi. Rebuild it, or the browser checkpoints go"; \
		echo "   unchecked: source ~/emsdk/emsdk_env.sh && make $(WEB_NODE_JS))"; \
	fi

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
CEDAR_DESKTOP_CYCLES ?= 36000000000
# PIN THE CLOCK. time(NULL) in eth_fill_alto_time is the emulator's only
# nondeterministic input, and it moves how far a boot gets: two bakes of
# this same recipe once landed ~6B cycles apart, one at the desktop and
# one still installing fonts (recognizable by ~87K display-list pixels
# instead of ~167K). Pinned, a rebake reproduces the shipped artifact.
CEDAR_DESKTOP_FAKE_TIME ?= 1783285880
CEDAR_DESKTOP_SNAPSHOT_GZ := snapshot-assets/cedar-desktop-native.snap.gz
CEDAR_DESKTOP_PDI_GZ := snapshot-assets/cedar-desktop-native.pdi.gz

.PHONY: cedar-desktop-snapshot run-cedar-desktop-sdl
# TYPE-AT IS TIMING-CRITICAL AND WAS WRONG. The login is typed at a fixed
# cycle, and the prompt has to be up by then or the keys are simply lost --
# the bake then runs its whole budget and snapshots the LOGIN SCREEN
# (28,570 px instead of ~167,000), overwriting the good checkpoint with a
# useless one. 760M worked once; it does not now, because serving the
# LookupFile protocol (2026-07-31) removed four retries and a 30-second
# negative cache from the cold boot and moved the prompt more than a billion
# cycles later. Measured: 760M and 1.2B lose the keys, 2B logs in.
#
# No gate covers this, because every Cedar gate RESTORES a checkpoint rather
# than cold-booting. Back up build/good-packs/cedar-desktop.{snap,pdi}
# before rebaking, and check the result is a desktop before trusting it.
cedar-desktop-snapshot: $(BINDIR)/dorado $(CEDAR_WORK_PDI)
	mkdir -p '$(dir $(CEDAR_DESKTOP_SNAPSHOT))' '$(dir $(CEDAR_DESKTOP_SNAPSHOT_GZ))'
	cp -f '$(CEDAR_WORK_PDI)' '$(CEDAR_DESKTOP_PDI)'
	DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 \
	DORADO_FAKE_TIME='$(CEDAR_DESKTOP_FAKE_TIME)' ./build/dorado \
		--boot-reason disk --no-alto-boot \
		--eb '$(CEDAR_EB)' --germ '$(CEDAR_GERM)' \
		--pilot-disk '$(CEDAR_DESKTOP_PDI)' \
		--ftp-root ../chm/cedar/stp-root \
		--key-hold 1600000 \
		--type-at 2500000000 --type 'Guest\n\n' \
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
		--type-at 36100000000 --click 700,733 \
		--paste-at 36300000000 --paste 'Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack\nBringover -p [Cedar]<CedarChest6.1>Top>Clock\nRun ChessHackImpl\nRun Clock\nClock\nChessHack\nEval ImagerFont.Find["Xerox/TiogaFonts/Chess40"]\n' \
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

# ---------------------------------------------------------------------------
# Lyric with the FULL sysout, built to PARC's own recipe.
#
# chm/lisp/cm/NewUserBigDisk.cm!6 ("FOR Lyric ONLY -- BEWARE") is Xerox's
# procedure for exactly this: a plain 20,000-page LISP.VIRTUALMEM, no
# partition extension.  Full.sysout!6 is 7,752,192 bytes = 15,141 pages, so
# 20,000 leaves 4,859 pages (2.5 MB) of working room.  Do NOT size this to
# the sysout (~15,200): that is the minimum that FITS, not a size that runs
# -- a full sysout has the whole library preloaded and needs room to cons.
#
# PARC noted 20,000 left THEM about 1,000 free pages, so their pack was
# ~21,000; ours is 22,736, i.e. 1,736 pages larger.  The cost is the 204
# on-pack packages, which Leaf serves over the wire now anyway.
#
# If this ever needs to exceed one partition, the mechanism is already in
# Lisp.run!6: `n/X` links a Lisp.xvirtualmem in partition n (1..7, at most
# two of them).  Our pack uses partitions 5 and 4, so 1-3 are free.
# See docs/lisp-leaf-handoff.md section 6.1.
# The sysout NEVER lands on the pack -- `lisp.run/M {DORADO}LISP.SYSOUT`
# streams it over FTP straight into VMEM (see run-lisp-lyric-full below).
# That is what makes 20,000 work at all: only the VMEM is charged to the
# 22,736-page volume.  Storing the sysout as a file too would need
# 20,000 + 15,141 = 35,141 pages and cannot fit by a factor of 1.5.
#
# So LISP_SYSOUT_FILE belongs on the BOOT target, not here: the pack build
# (lisp-bcplprog-scavenged-vmem-image) never reads it, and passing it here
# is silently inert.
# MEASURED 2026-08-06, by re-running only the CreateFile step against the
# cached scavenged pack and reading the SCREEN (CreateFile prints "There
# isn't enough space on your disk" and the emulator still exits 0):
#
#   15,002 (shipped)  fits   -139 pages vs Full.sysout!6 -- TOO SMALL
#   15,400            fits    259
#   16,000            fits    859
#   17,000            fits  1,859
#   18,000            fits  2,859
#   19,000            fits  3,859   <- default here
#   20,000 (PARC)     FAILS
#
# So our build order costs ~1,000 pages against NewUserBigDisk.cm, not the
# 5,000 a first look suggested: PARC place the VMEM on an erased disk and
# FTP the Lisp files in after, we insert the files first and CreateFile
# into the fragmented remainder. Recovering that last 1,000 means creating
# the VMEM before the inserts (altofs has --vmem-after-inserts, and NOT
# passing it is PARC's order) -- not needed, since 19,000 already gives
# Full.sysout!6 3,859 pages of working room against PARC's own 4,859.
# WHICH full sysout: use Released-Full.sysout!2. It BOOTS (2026-08-06);
# Full.sysout!6 does not, and the difference is the Lisp virtual-address
# space each one occupies:
#
#   LISP.SYSOUT!1          9,124 VPs   max VP 16,383   0 above 2^14   boots
#   Released-Full.sysout!2 11,457 VPs  max VP 16,383   0 above 2^14   BOOTS
#   Full.sysout!6         14,843 VPs   max VP 65,533 894 above 2^14   Raid
#
# 2^14 pages of 256 words = a 22-bit address space, which is exactly what
# the period BCPL assumes: VMem.decl declares LastVirtualPage = #37777 and
# reserves >= #40000 as the EMPTY/UNAVAIL sentinels of the buffer page
# table ("assumes 22-bit addresses!"). Full.sysout!6 is dated 17-Nov-1988,
# nineteen months AFTER the Lyric release, and was evidently saved on a
# machine with Briggs's ExtendedVmem installed -- see
# chm/lisp/lyric-lispusers/ExtendedVmem.tedit!1 and
# docs/lisp-leaf-handoff.md 6.13-6.18.
#
# Released-Full.sysout!2 is the RELEASED full sysout (Jan 1988) and is a
# genuine full-library image: 11,457 VPs against LISP.SYSOUT!1's 9,124.
LISP_RELFULL_SYSOUT_FILE ?= ../chm/lisp/lyric-basics/Released-Full.sysout!2
LISP_FULL_SYSOUT_FILE ?= ../chm/lisp/lyric-basics/Full.sysout!6
LISP_FULL_VMEM_PAGES ?= 19000
LISP_FULL_GOOD_PACK ?= build/good-packs/lisp-lyric-full-$(LISP_FULL_VMEM_PAGES).pack
LISP_FULL_WORK_PREFIX ?= build/good-packs/lisp-lyric-full-work
LISP_FULL_CYCLES ?= 12000000000

# INIT.DORADO, not the default INIT.NONET.  NONET's DIRECTORIES is
# ({DSK} {FLOPPY}) -- no network device -- so FILESLOAD has no {DORADO} to
# search, falls through to probing local partitions, and dies with
# "Device error: {DSK1}DISKDESCRIPTOR.;1" on the very first documented
# command.  INIT.DORADO is the same file with {FLOPPY} -> {DORADO} (a
# length-preserving substitution); see the LISP_INIT_OVERRIDE note below.
LISP_FULL_INIT_FILE ?= ../chm/lisp/ftp-root/INIT.DORADO

lisp-lyric-full-pack:
	mkdir -p '$(dir $(LISP_FULL_GOOD_PACK))'
	$(MAKE) lisp-bcplprog-scavenged-vmem-image \
		LISP_VMEM_PAGES=$(LISP_FULL_VMEM_PAGES) \
		LISP_LYRIC_VMEM_PAGE_SPEC=$(LISP_FULL_VMEM_PAGES)D \
		LISP_LYRIC_CREATEFILE_CYCLES=9500000000 \
		LISP_INIT_FILE='$(LISP_FULL_INIT_FILE)' \
		LISP_BCPLPROG_DSK='$(LISP_FULL_WORK_PREFIX)-loader.dsk' \
		LISP_BCPLPROG_PACK='$(LISP_FULL_WORK_PREFIX)-loader.pack' \
		LISP_BCPLPROG_CREATEFILE_DSK='$(LISP_FULL_WORK_PREFIX)-createfile.dsk' \
		LISP_BCPLPROG_CREATEFILE_PACK='$(LISP_FULL_WORK_PREFIX)-createfile.pack' \
		LISP_BCPLPROG_PHYSICAL_DSK0='$(LISP_FULL_WORK_PREFIX)-drive0.dsk' \
		LISP_BCPLPROG_PHYSICAL_DSK1='$(LISP_FULL_WORK_PREFIX)-drive1.dsk' \
		LISP_BCPLPROG_PHYSICAL_PACK='$(LISP_FULL_WORK_PREFIX)-physical.pack' \
		LISP_BCPLPROG_SCAVENGED_PACK='$(LISP_FULL_WORK_PREFIX)-scavenged.pack' \
		LISP_BCPLPROG_SCAVENGED_VMEM_PACK='$(LISP_FULL_GOOD_PACK)'

# Boot the 20,000-page pack with Full.sysout!6 served over FTP.  Headless;
# writes /tmp/dorado-lisp-lyric.pgm and periodic shots.  Needs the pack
# from lisp-lyric-full-pack to exist (it is NOT a prerequisite on purpose --
# that bake is ~20 minutes and should not be triggered by a boot attempt).
run-lisp-lyric-full: $(BINDIR)/dorado
	@test -r '$(LISP_FULL_GOOD_PACK)' || \
		(echo "Missing $(LISP_FULL_GOOD_PACK) -- run 'make lisp-lyric-full-pack' first." && false)
	@test -r '$(LISP_RELFULL_SYSOUT_FILE)' || \
		(echo "Missing $(LISP_RELFULL_SYSOUT_FILE) -- fetch Released-Full.sysout!2 from <Lisp>Lyric>Basics>." && false)
	$(if $(LISP_LYRIC_FTP_TRACE),DORADO_FTP_TRACE=1 )DORADO_DISPM_PRESENT=1 ./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(LISP_FULL_GOOD_PACK)' --boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_RELFULL_SYSOUT_FILE)' \
		--type 'lisp.run/M {$(LISP_REMOTE_HOST)}$(LISP_REMOTE_SYSOUT_NAME)\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles $(LISP_FULL_CYCLES) \
		--shot-prefix /tmp/dorado-lisp-lyric-full --shot-every 1000000000 \
		--out /tmp/dorado-lisp-lyric-full.pgm

# Interactive: the full-library Lyric world in a window.  Boots
# Released-Full.sysout!2 (see the note above LISP_RELFULL_SYSOUT_FILE for why
# that one and not Full.sysout!6).  Runs on a WRITABLE copy of the pack, so
# the VMEM the guest writes does not mutate the checked-in artifact.
LISP_FULL_RUN_PACK ?= $(TRIDENT_RUN_DIR)/lisp-lyric-full-run.pack

# FAST START: restore the baked checkpoint instead of booting for ~4 minutes.
# This is the one to use day to day; run-lisp-lyric-full-sdl below is the cold
# boot, for when you want to watch the sysout load or have no checkpoint.
.PHONY: run-lisp-lyric-full-snapshot-sdl
run-lisp-lyric-full-snapshot-sdl: sdl
	@test -r '$(LISP_FULL_SNAPSHOT)' || \
		(echo "Missing $(LISP_FULL_SNAPSHOT). Run 'make lisp-lyric-full-snapshot' once." && false)
	@test -r '$(LISP_FULL_SNAPSHOT_PACK)' || \
		(echo "Missing matching pack $(LISP_FULL_SNAPSHOT_PACK)." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	cp -f '$(LISP_FULL_SNAPSHOT_PACK)' '$(LISP_FULL_RUN_PACK)'
	chmod 644 '$(LISP_FULL_RUN_PACK)'
	@echo "Full-library Lyric (Released-Full.sysout!2, 25-Jan-88)."
	@echo "Click the Exec (XCL) window, then type a form, e.g.  (PLUS 1 2)"
	@echo "For the reference manual:  (IL:FILESLOAD HELPSYS)  then"
	@echo "  (IL:LOAD $(QUOTE){DORADO}<IRM>IRMDEMO)  -- answer Login with Guest/Guest"
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_FULL_RUN_PACK)' --boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_FULL_SNAPSHOT)' \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--scale 1 --speed 4000000

.PHONY: run-lisp-lyric-full-sdl
run-lisp-lyric-full-sdl: sdl
	@test -r '$(LISP_FULL_GOOD_PACK)' || \
		(echo "Missing $(LISP_FULL_GOOD_PACK) -- run 'make lisp-lyric-full-pack' first." && false)
	@test -r '$(LISP_RELFULL_SYSOUT_FILE)' || \
		(echo "Missing $(LISP_RELFULL_SYSOUT_FILE) -- fetch Released-Full.sysout!2 from <Lisp>Lyric>Basics>." && false)
	mkdir -p '$(dir $(LISP_FULL_RUN_PACK))'
	cp -f '$(LISP_FULL_GOOD_PACK)' '$(LISP_FULL_RUN_PACK)'
	chmod 644 '$(LISP_FULL_RUN_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_FULL_RUN_PACK)' --boot-reason disk --no-alto-boot \
		--ftp-sysout '$(LISP_RELFULL_SYSOUT_FILE)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--scale 1 --speed 4000000
	@# NOTE: deliberately NO --ftp-root here. dorado_ethernet_set_ftp_sysout
	@# and _set_ftp_root write the SAME field (ethernet.c ftp_sysout_path,
	@# a directory meaning "rooted tree" and a file meaning "serve this"),
	@# and dorado_machine_set_ftp_source applies root SECOND -- so passing
	@# both would silently serve ../chm/lisp/ftp-root/LISP.SYSOUT (the plain
	@# 4.8 MB sysout) instead of Released-Full.sysout!2, booting the wrong
	@# world with no error. The snapshot target above can use --ftp-root
	@# precisely because it has no --ftp-sysout to lose.

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

QUOTE := '

# The same desktop with the Interlisp library on its disk: 194 Lyric .LCOM
# packages, including the SKETCH illustrator.  Nothing is loaded yet -- click
# the prompt window and type, e.g.
#
#     (LOAD '{DSK}SKETCH.LCOM)
#
# The click matters: input goes to the window under the cursor, and without
# it the keystrokes land nowhere.  Note the Exec is XCL, so LOAD works bare
# but Interlisp functions need the IL: prefix.
.PHONY: run-lisp-lispusers-sdl
run-lisp-lispusers-sdl: sdl
	@test -r '$(LISP_LISPUSERS_SNAPSHOT)' || \
		(echo "Missing $(LISP_LISPUSERS_SNAPSHOT). Run 'make lisp-lispusers-snapshot' once." && false)
	@test -r '$(LISP_LISPUSERS_SNAPSHOT_PACK)' || \
		(echo "Missing matching pack $(LISP_LISPUSERS_SNAPSHOT_PACK)." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	cp -f '$(LISP_LISPUSERS_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-lispusers-run.pack'
	chmod 644 '$(TRIDENT_RUN_DIR)/lisp-lispusers-run.pack'
	@echo "Click the Exec (XCL) window, then type:  (LOAD $(QUOTE){DSK}SKETCH.LCOM)"
	@echo "For the reference manual:  (IL:FILESLOAD HELPSYS)  then"
	@echo "  (IL:LOAD $(QUOTE){DORADO}<IRM>IRMDEMO)  -- answer Login with Guest/Guest"
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-lispusers-run.pack' \
		--boot-reason disk --no-alto-boot \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--snapshot-in '$(LISP_LISPUSERS_SNAPSHOT)' \
		--scale 1 --speed 400000

# Same, but the LOAD is typed for you a moment after the restore, so the
# illustrator is already in by the time you touch the keyboard.  The cycle
# numbers are absolute and the checkpoint restores at 8.8 B, which is why
# they look large; SKETCH pulls five more files off {DSK} and takes a while.
.PHONY: run-lisp-sketch-sdl
run-lisp-sketch-sdl: sdl
	@test -r '$(LISP_LISPUSERS_SNAPSHOT)' || \
		(echo "Missing $(LISP_LISPUSERS_SNAPSHOT). Run 'make lisp-lispusers-snapshot' once." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	cp -f '$(LISP_LISPUSERS_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-sketch-run.pack'
	chmod 644 '$(TRIDENT_RUN_DIR)/lisp-sketch-run.pack'
	@echo 'Auto-loading SKETCH. The first "Loading" line appears after ~10 s;'
	@echo 'the full chain (SKETCHELEMENTS, SKETCHEDIT, SKETCHOBJ, SKETCHBMELT,'
	@echo 'SCALEBITMAP) is ~4 B cycles and takes ~3 min of wall clock.'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-sketch-run.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_LISPUSERS_SNAPSHOT)' \
		--type-at 8850000000 --click 250,50 --key-hold 2000000 \
		--type-at 8950000000 --type "(LOAD $(QUOTE){DSK}SKETCH.LCOM)\n" \
		--scale 1 --speed 400000

# The File Browser, loaded for you.  It needs TABLEBROWSER, which it pulls
# off {DSK} itself once both are on the pack -- and both must come from the
# SAME tree: FILEBROWSER.LCOM is !1 (Mar-1987) in phylum/lisp/lyric/library
# and !2 (Jun-1988) in phylum/lisplibrary/lyric, and only the release pair is
# self-consistent.
.PHONY: run-lisp-filebrowser-sdl
run-lisp-filebrowser-sdl: sdl
	@test -r '$(LISP_LISPUSERS_SNAPSHOT)' || \
		(echo "Missing $(LISP_LISPUSERS_SNAPSHOT). Run 'make lisp-lispusers-snapshot' once." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	cp -f '$(LISP_LISPUSERS_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-fb-run.pack'
	chmod 644 '$(TRIDENT_RUN_DIR)/lisp-fb-run.pack'
	@echo 'Loading FILEBROWSER (+TABLEBROWSER); ~1 min. Then sweep out a window:'
	@echo "type  (IL:FB $(QUOTE){DSK})  and drag the mouse with the LEFT button held."
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado-sdl \
		--eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-fb-run.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_LISPUSERS_SNAPSHOT)' \
		--type-at 8850000000 --click 250,50 --key-hold 2000000 \
		--type-at 8950000000 --type "(LOAD $(QUOTE){DSK}FILEBROWSER.LCOM)\n" \
		--scale 1 --speed 400000

# Interlisp loading a package that is NOT on its disk, over Leaf.  The probe
# must be a file the pack does not carry, or the test proves nothing --
# AISBLT is deliberately absent from lisp-lispusers-packages.txt.
LISP_LEAF_PROBE ?= AISBLT
# Where to seek to, and how many bytes to read back. 1000 is arbitrary except
# that it is not 0 and not a block boundary, so a server that ignored the
# offset would return the wrong bytes rather than accidentally the right ones.
LISP_LEAF_OFFSET ?= 1000
LISP_LEAF_COUNT  ?= 4

# Leaf is the IFS RANDOM-ACCESS protocol, so the gate drives a SEEK.
#
# It used to drive (FILESLOAD AISBLT) and assert the file streamed. That only
# reached Leaf because the STP retrieve failed first -- an empty Directory in
# the command plist, fixed 2026-08-07 -- and once the retrieve worked,
# FILESLOAD legitimately went over STP and this gate saw no Leaf traffic at
# all. A gate whose subject is reached only by way of a bug stops testing the
# moment the bug is fixed.
#
# The guest now OPENSTREAMs the file, seeks to LISP_LEAF_OFFSET, reads the
# byte there, and SEEKS AGAIN TO THAT BYTE TIMES 100. That last step is the
# whole trick: the second seek's address is a function of the DATA, so the
# wire alone proves the guest received the right bytes -- no screen reading,
# no OCR. (pgm_text.py cannot read this Exec's font; every glyph comes back
# '?'. Deriving the proof from behaviour is better than teaching it the font,
# because it cannot be fooled by a plausible-looking screen.)
#
# Both ends are still checked, they are just both on the wire:
#   - a LEAF_READ at a non-zero address           -> it seeked at all
#   - a LEAF_READ at the address the DATA implies -> it seeked to the right
#                                                    place, so the bytes it
#                                                    got match the host file
LISP_LEAF_OFFSET ?= 1000
LISP_LEAF_SCALE  ?= 100

.PHONY: verify-lisp-leaf
verify-lisp-leaf: $(BINDIR)/dorado
	@test -r '$(LISP_LISPUSERS_SNAPSHOT)' || \
		(echo "Missing $(LISP_LISPUSERS_SNAPSHOT); run 'make lisp-lispusers-snapshot LISP_INIT_OVERRIDE=$(LISP_LISPUSERS_DIR)/INIT.DORADO' first." && false)
	@grep -qx '$(LISP_LEAF_PROBE).LCOM' '$(LISP_LISPUSERS_PACKAGE_LIST)' && \
		(echo "$(LISP_LEAF_PROBE) is ON the pack; the gate would pass without the network." && false) || true
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_LISPUSERS_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-leaf-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-leaf-gate.pack'
	@DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		DORADO_FTP_TRACE=1 \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-leaf-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--snapshot-in '$(LISP_LISPUSERS_SNAPSHOT)' \
		--type-at 8850000000 --click 250,50 --key-hold 2000000 \
		--type-at 8950000000 \
		--type "(IL:SETQ IL:S (IL:OPENSTREAM '{DORADO}<>$(LISP_LEAF_PROBE).LCOM 'IL:INPUT))\n" \
		--type-at 11000000000 --type "Guest\n" \
		--type-at 12200000000 --type "Guest\n" \
		--type-at 14000000000 --type "(IL:SETFILEPTR IL:S $(LISP_LEAF_OFFSET))\n" \
		--type-at 15000000000 \
		--type "(IL:SETFILEPTR IL:S (IL:ITIMES (IL:BIN IL:S) $(LISP_LEAF_SCALE)))\n" \
		--type-at 16000000000 --type "(IL:BIN IL:S)\n" \
		--cycles 17200000000 --out build/verify-lisp-leaf.pgm \
		> build/verify-lisp-leaf.log 2>&1
	@sh ../tools/check_lisp_leaf_gate.sh build/verify-lisp-leaf.log '$(LISP_LEAF_PROBE)'
	@want=`python3 -c "d=open('$(LISP_LISPUSERS_DIR)/$(LISP_LEAF_PROBE).LCOM','rb').read();print(d[$(LISP_LEAF_OFFSET)]*$(LISP_LEAF_SCALE)//512*512)"`; \
	grep -q "^LEAF_READ handle=[0-9]* addr=$$want " build/verify-lisp-leaf.log && \
		echo "verify-lisp-leaf: and it seeked to block $$want, which only the right byte at $(LISP_LEAF_OFFSET) produces" || \
		(echo "verify-lisp-leaf: FAIL -- no read at block $$want, so the byte the guest read was not the one on disk"; \
		 grep "^LEAF_READ handle" build/verify-lisp-leaf.log | sed 's/^/      /'; false)

# The gate must fail when Leaf does not seek. Cheap to check without booting:
# feed the checker a log with only sequential reads.
.PHONY: verify-lisp-leaf-selftest
verify-lisp-leaf-selftest:
	@printf 'LEAF Reset x\nLEAF_REPLY Reset x\nLEAF Params x\nLEAF_REPLY Params x\nLEAF_OPEN "a"\nLEAF_READ_LEADER handle=1\nLEAF_READ handle=000001 addr=0 want=512 of 15365\n' > build/leaf-noseek.log
	@if sh ../tools/check_lisp_leaf_gate.sh build/leaf-noseek.log >/dev/null 2>&1; then \
		echo "SELFTEST FAIL: the checker passed a log with no seek"; exit 1; fi
	@printf 'LEAF Reset x\nLEAF_REPLY Reset x\nLEAF Params x\nLEAF_REPLY Params x\nLEAF_OPEN "a"\nLEAF_READ_LEADER handle=1\nLEAF_READ handle=000001 addr=7168 want=512 of 15365\n' > build/leaf-seek.log
	@sh ../tools/check_lisp_leaf_gate.sh build/leaf-seek.log >/dev/null 2>&1 || \
		(echo "SELFTEST FAIL: the checker failed a log with a seek"; exit 1)
	@echo "verify-lisp-leaf-selftest: the checker distinguishes a seek from a sequential read"

# ---------------------------------------------------------------------------
# Interlisp-D library packages on the Lyric pack.
#
# Lyric reaches remote files over Leaf, which we do not serve -- only Pup FTP
# and STP, which is enough for the Alto-side loader to fetch LISP.SYSOUT but
# not for Lisp itself.  So library packages have to arrive on the LOCAL disk,
# where Lisp names them {DSK}FOO.DCOM, and putting them there means editing the
# Alto file system inside the Trident pack.
#
# Two things about that edit are easy to get wrong:
#
#   --base is not optional.  dsk2trident --extract only covers the mapped
#   Diablo region (cyl+3, the partition's head); rebuilding without --base
#   zeroes everything outside it.  With --base the round trip is byte-exact --
#   verified by extracting and re-converting the shipped pack unchanged.
#
#   --no-vmem is not optional either.  Without it altofs recreates
#   LISP.VIRTUALMEM., which IS the saved Lisp world -- 15002 pages, and every
#   bit of the desktop state.
#
# The pack is reproducible but not byte-reproducible: Alto file leaders carry
# created/written/read times, and altofs stamps them from the host clock, so
# two runs of this target differ in ~100 bytes across the leader pages of the
# inserted files and SysDir.  Compare the directory listing, not the bytes.
#
# The pack alone is not enough to reach the files from a RESTORED checkpoint:
# the snapshot's virtual memory holds the directory Lisp read at ITS boot, so a
# file inserted afterwards is genuinely absent as far as that world is
# concerned ("File not found: {DSK}BANNER.DCOM" while a file already on the
# pack loads fine).  Hence the second target, which boots the edited pack from
# cold so Lisp reads the directory that has them, and saves that world.
# Full filenames, because the extension IS the release: Lyric compiles to
# .LCOM, Harmony and Koto to .DCOM, and the Common Lisp compiler to .DFASL.
# The default set is Lyric-vintage .LCOM -- what the shipped sysout can
# actually load -- and lives in a file rather than here because it is 194
# names chosen to fill the pack's free pages. Override with
# LISP_LISPUSERS_PACKAGES='A.LCOM B.LCOM' for a quick experiment.
LISP_LISPUSERS_PACKAGE_LIST ?= lisp-lispusers-packages.txt
# The hash MUST be escaped: an unescaped one starts a Make comment and would
# truncate this line in the middle of $(shell ...), which fails with the
# distinctly unhelpful "unterminated call to function `shell'".
LISP_LISPUSERS_PACKAGES ?= $(shell sed 's/\#.*//' '$(LISP_LISPUSERS_PACKAGE_LIST)')
LISP_LISPUSERS_DIR ?= ../chm/lisp/ftp-root
LISP_LISPUSERS_PACK ?= build/good-packs/lisp-lyric-lispusers.pack
LISP_LISPUSERS_SNAPSHOT ?= build/good-packs/lisp-lyric-lispusers.snap
LISP_LISPUSERS_SNAPSHOT_PACK ?= build/good-packs/lisp-lyric-lispusers-xcl.pack
# Alto utilities a Lisp pack has no use for.  The pack cannot be made bigger:
# AltoDiabloDisk.mc hard-codes 406 cylinders ("626B = 406 = number of
# cylinders", and anything past it is ABadCylinder), the Diablo disk address
# has ONE drive bit and ONE head bit, and 406*2+3 = 815 is exactly the T-80's
# cylinder count -- so 2 drives x 2 heads x 406 cyl x 14 sectors = 22,736
# pages is the architectural ceiling, and LISP.VIRTUALMEM. takes 15,003 of
# them.  Freeing these 33 files is the only way to buy room, and it buys
# 1,590 pages (0.79 MB) -- Bravo alone is 458.
#
# Fonts.Widths and Gacha10.al are deliberately NOT dropped: INIT.NONET, the
# greeting file this pack actually uses, names {DSK}FONTS.WIDTHS in
# PRESSFONTWIDTHSFILES and searches {DSK} itself for display fonts
# (DISPLAYFONTDIRECTORIES '({DSK} {FLOPPY})).  Deleting them produced
# file-not-found errors.
LISP_PACK_DROP ?= BRAVO.RUN. Ftp.Run. BCPL.RUN. NEPTUNE.RUN. EMPRESS.RUN. \
	CHAT.RUN. OEDIT.RUN. ALTOD1MC.EB. bravo.scratchbin. BLDR.RUN. \
	BRAVO.ERROR. BRAVO.MESSAGES. bravo.scratchbin2. \
	Bravo.State. bravo.scratchbravo. bravo.ts. SYS.BK. SYS.SYMS. \
	ALTODEFS.D. ALTOFILESYS.D. BCPLFILES.D. DISKS.D. STREAMS.D. SYSDEFS.D. \
	Dumper.boot. Rem.Cm. Ftp.log. Scavenger.Log$$. \
	UPDATELISP.CM. CREATEFILE.RUN.

# Swap the greeting file.  The stock pack carries INIT.NONET (renamed
# INIT.LCOM), whose DIRECTORIES is ({DSK} {FLOPPY}) -- no network device at
# all.  INIT.DORADO is that same file with {FLOPPY} -> {DORADO}, which is a
# byte-for-byte length-preserving substitution, so Lisp searches our served
# root for anything it cannot find locally.  Note the pack's INIT.LCOM is
# SOURCE despite the name (2,438 bytes, identical to INIT.NONET!1); Interlisp
# LOADs it interpreted, so no compiler is needed to change it.
LISP_INIT_OVERRIDE ?=

LISP_LISPUSERS_GEOM ?= --diablo-cylinders 406 --diablo-sectors 14
LISP_LISPUSERS_SCRATCH ?= build/lispusers

.PHONY: lisp-lispusers-fetch
lisp-lispusers-fetch:
	python3 ../tools/fetch_lispusers.py $(LISP_LISPUSERS_PACKAGES)

.PHONY: lisp-lispusers-pack
lisp-lispusers-pack: $(BINDIR)/dsk2trident $(BINDIR)/altofs $(LISP_GOOD_PACK)
	@mkdir -p '$(LISP_LISPUSERS_SCRATCH)' '$(dir $(LISP_LISPUSERS_PACK))'
	./build/dsk2trident --extract $(LISP_LISPUSERS_GEOM) '$(LISP_GOOD_PACK)' \
		'$(LISP_LISPUSERS_SCRATCH)/d0.dsk' --drive1 '$(LISP_LISPUSERS_SCRATCH)/d1.dsk'
	./build/altofs --disk0 '$(LISP_LISPUSERS_SCRATCH)/d0.dsk' \
		--disk1 '$(LISP_LISPUSERS_SCRATCH)/d1.dsk' --existing --no-vmem \
		$(foreach f,$(LISP_PACK_DROP),--delete '$(f)') \
		$(if $(LISP_INIT_OVERRIDE),--delete 'INIT.LCOM.' \
			--insert '$(LISP_INIT_OVERRIDE)' 'INIT.LCOM.',) \
		$(foreach p,$(LISP_LISPUSERS_PACKAGES),--insert '$(LISP_LISPUSERS_DIR)/$(p)' '$(p).')
	./build/dsk2trident --partition $(LISP_MAIN_PARTITION) $(LISP_LISPUSERS_GEOM) \
		--base '$(LISP_GOOD_PACK)' --drive1 '$(LISP_LISPUSERS_SCRATCH)/d1.dsk' \
		'$(LISP_LISPUSERS_SCRATCH)/d0.dsk' '$(LISP_LISPUSERS_PACK)'
	@echo 'lisp-lispusers-pack: wrote $(LISP_LISPUSERS_PACK)'
	./build/altofs --disk0 '$(LISP_LISPUSERS_SCRATCH)/d0.dsk' \
		--disk1 '$(LISP_LISPUSERS_SCRATCH)/d1.dsk' \
		--existing --preserve-existing-metadata --no-vmem --list 'SysDir.' | grep -E 'LCOM|DCOM|DFASL'

# Boot the edited pack from cold and save the world that can see the new
# files.  --ftp-root (not --ftp-sysout) serves LISP.SYSOUT by name out of the
# same directory the packages came from: the Alto-side loader asks for
# `{DORADO}LISP.SYSOUT`, and the STP server strips the {device} bracket.
.PHONY: lisp-lispusers-snapshot
lisp-lispusers-snapshot: $(BINDIR)/dorado lisp-lispusers-pack
	cp -f '$(LISP_LISPUSERS_PACK)' '$(LISP_LISPUSERS_SNAPSHOT_PACK)'
	chmod 644 '$(LISP_LISPUSERS_SNAPSHOT_PACK)'
	DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
	./build/dorado \
		--eb worlds/aemu.eb \
		--disk 0='$(LISP_LISPUSERS_SNAPSHOT_PACK)' \
		--boot-reason disk --no-alto-boot \
		--ftp-root '$(LISP_LISPUSERS_DIR)' \
		--type 'lisp.run/M {DORADO}LISP.SYSOUT\n' \
		--type-at 330000000 --key-hold 4000000 \
		--cycles '$(LISP_DESKTOP_SNAPSHOT_CYCLES)' \
		--snapshot-out '$(LISP_LISPUSERS_SNAPSHOT)' \
		--out build/lisp-lispusers-desktop.pgm

# Prove a package actually loads, by reading Lisp's own answer off the screen.
# A pixel count cannot do this: "Loading {DSK}BANNER.DCOM" and "File not
# found: {DSK}BANNER.DCOM" are both just text appearing in the prompt window
# (211,318 px vs 210,163 -- a 0.5% difference that says nothing about which
# one happened).  tools/pgm_text.py segments the glyphs and matches them
# against the guest font, so the gate reads the sentence.
LISP_LISPUSERS_PROBE ?= GREP.LCOM

.PHONY: verify-lisp-lispusers
verify-lisp-lispusers: $(BINDIR)/dorado
	@test -r '$(LISP_LISPUSERS_SNAPSHOT)' || \
		(echo "Missing $(LISP_LISPUSERS_SNAPSHOT); run 'make lisp-lispusers-snapshot' first." && false)
	@mkdir -p '$(TRIDENT_RUN_DIR)'
	@cp -f '$(LISP_LISPUSERS_SNAPSHOT_PACK)' '$(TRIDENT_RUN_DIR)/lisp-lispusers-gate.pack'
	@chmod 644 '$(TRIDENT_RUN_DIR)/lisp-lispusers-gate.pack'
	@DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME='$(LISP_CURRENT_FAKE_TIME)' \
		./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/lisp-lispusers-gate.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(LISP_LISPUSERS_SNAPSHOT)' \
		--type-at 8850000000 --click 250,50 --key-hold 2000000 \
		--type-at 8950000000 --type "(LOAD '{DSK}$(LISP_LISPUSERS_PROBE))\n" \
		--cycles 10200000000 --out build/verify-lisp-lispusers.pgm >/dev/null 2>&1
	@text=`python3 ../tools/pgm_text.py build/verify-lisp-lispusers.pgm \
		--diff build/lisp-lispusers-desktop.pgm --space-gap 6 --cols 18,320 2>/dev/null`; \
	echo "$$text"; \
	case "$$text" in \
	  *"not found"*) \
	    echo "verify-lisp-lispusers: FAIL -- Lisp cannot see {DSK}$(LISP_LISPUSERS_PROBE)."; \
	    echo "  Either the pack edit did not take, or -- far more likely -- this"; \
	    echo "  world was saved BEFORE the file was inserted, so its in-memory"; \
	    echo "  directory does not have it. Re-run 'make lisp-lispusers-snapshot'."; \
	    false;; \
	  *"compiled function"*) \
	    echo "verify-lisp-lispusers: FAIL -- wrong vintage, NOT a plumbing fault."; \
	    echo "  Lisp found the file, read it, and printed its true creation date,"; \
	    echo "  then rejected the compiled code itself. Every .DCOM in"; \
	    echo "  eris/lispusers is a 1982-84 (Harmony-era) build and the sysout we"; \
	    echo "  boot is Lyric (1986-87); the compiled-code format moved between"; \
	    echo "  them. Loading one of these needs a Harmony sysout, not a fix here."; \
	    false;; \
	  *"compiled on"*) echo "verify-lisp-lispusers: PASS -- $(LISP_LISPUSERS_PROBE) loaded";; \
	  *) echo "verify-lisp-lispusers: FAIL -- no recognisable reply from Lisp"; false;; \
	esac

run-tricond-nopack: $(BINDIR)/rundiag
	RUNDIAG_DISK=1 RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN TESTOK-WITHOUT-DISK ERR 50000000
run-tricond-pack: $(BINDIR)/rundiag alto-disk-image
	RUNDIAG_DISK=1 RUNDIAG_DISK_MEDIA=diablo RUNDIAG_DISK_PATH='$(ALTO_RUN_PACK)' RUNDIAG_TASK=14 ./build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN DONE ERR 120000000

# ---- Alto Executive disk boot gate -----------------------------------------
# The AEmu world booting a real Alto OS off the Diablo-on-Trident games pack.
# 2,092 px = the BCPL Executive painted; 0 px = the display list is never
# built (the mouse pointer then smears, since the rasteriser only clears when
# it has a valid chain). This value lived in a commit message and rotted to 0
# unnoticed for a month -- see docs/CONTINUE-HERE.md 2026-07-28 (later).
# The 2026-06/07 failure was a STALE PACK, not the emulator: 3fe8ae1 changed
# dsk2trident's layout and the checked-in golden pack was never regenerated.
# If this ever fails again, regenerate first: make refresh-alto-golden.
ALTO_DISK_GATE_CYCLES ?= 700000000
ALTO_DISK_GATE_MIN_PX ?= 2000

.PHONY: verify-alto-disk
verify-alto-disk: $(BINDIR)/dorado worlds
	mkdir -p '$(TRIDENT_RUN_DIR)'
	cp -f '$(WEB_ALTO_PACK)' '$(TRIDENT_RUN_DIR)/alto-disk-gate.pack'
	chmod 644 '$(TRIDENT_RUN_DIR)/alto-disk-gate.pack'
	./build/dorado --eb worlds/aemu.eb \
		--disk 0='$(TRIDENT_RUN_DIR)/alto-disk-gate.pack' \
		--boot-reason disk --cycles '$(ALTO_DISK_GATE_CYCLES)' \
		--out build/verify-alto-disk.pgm > build/verify-alto-disk.log 2>&1
	@px=$$(sed -n 's/^dorado: \([0-9]*\) display-list pixels.*/\1/p' \
		build/verify-alto-disk.log | tail -1); \
	echo "verify-alto-disk: $$px px (need >= $(ALTO_DISK_GATE_MIN_PX); good = 2092)"; \
	test "$$px" -ge '$(ALTO_DISK_GATE_MIN_PX)'

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

# ---- Smalltalk-76 checkpoints --------------------------------------------
# The boot is ~1.95 B cycles (about 90 s native, far too slow in a browser),
# so both the SDL "play now" target and the web build restore a checkpoint.
# The guest writes to its pack during the boot, so a snapshot is only valid
# with the pack as flushed at the same cycle -- keep the pair together.
SMALLTALK_SNAPSHOT        := build/good-packs/smalltalk76-desktop.snap
SMALLTALK_SNAPSHOT_PACK   := build/good-packs/smalltalk76-desktop.pack
SMALLTALK_SNAPSHOT_SCREEN := build/good-packs/smalltalk76-desktop.pgm
SMALLTALK_NATIVE_SNAPSHOT_GZ := snapshot-assets/smalltalk76-native.snap.gz
SMALLTALK_NATIVE_PACK_GZ     := snapshot-assets/smalltalk76-native.pack.gz

.PHONY: smalltalk-desktop-snapshot
smalltalk-desktop-snapshot: $(BINDIR)/dorado smalltalk-pack
	mkdir -p '$(dir $(SMALLTALK_SNAPSHOT))' snapshot-assets
	cp -f '$(SMALLTALK_PACK)' '$(SMALLTALK_SNAPSHOT_PACK)'
	chmod 644 '$(SMALLTALK_SNAPSHOT_PACK)'
	./build/dorado --cycles '$(SMALLTALK_CYCLES)' --eb '$(SMALLTALK_EB)' \
		--disk 0='$(SMALLTALK_SNAPSHOT_PACK)' \
		--boot-reason disk --no-alto-boot \
		--key-hold 3000000 --type-at 350000000 \
		--type 'Bootfrom xmsmall.boot\n' \
		--snapshot-out '$(SMALLTALK_SNAPSHOT)' \
		--out '$(SMALLTALK_SNAPSHOT_SCREEN)'
	gzip -c '$(SMALLTALK_SNAPSHOT)' > '$(SMALLTALK_NATIVE_SNAPSHOT_GZ)'
	gzip -c '$(SMALLTALK_SNAPSHOT_PACK)' > '$(SMALLTALK_NATIVE_PACK_GZ)'

# Restore the saved desktop instantly. Each run gets its own writable copy of
# the pack so the checkpoint pair stays immutable.
.PHONY: run-smalltalk-snapshot-sdl
run-smalltalk-snapshot-sdl: sdl
	@test -r '$(SMALLTALK_NATIVE_SNAPSHOT_GZ)' || \
		(echo "Missing $(SMALLTALK_NATIVE_SNAPSHOT_GZ). Run 'make smalltalk-desktop-snapshot' once." && false)
	mkdir -p '$(TRIDENT_RUN_DIR)' '$(dir $(SMALLTALK_SNAPSHOT))'
	gunzip -c '$(SMALLTALK_NATIVE_SNAPSHOT_GZ)' > '$(SMALLTALK_SNAPSHOT)'
	gunzip -c '$(SMALLTALK_NATIVE_PACK_GZ)' > '$(TRIDENT_RUN_DIR)/smalltalk76-run.pack'
	chmod 644 '$(TRIDENT_RUN_DIR)/smalltalk76-run.pack'
	./build/dorado-sdl --eb '$(SMALLTALK_EB)' \
		--disk 0='$(TRIDENT_RUN_DIR)/smalltalk76-run.pack' \
		--boot-reason disk --no-alto-boot \
		--snapshot-in '$(SMALLTALK_SNAPSHOT)'

# The browser checkpoint. Machine snapshots are raw C structs, so the wasm32
# ABI differs from the native one and the asset must come from a wasm32 run;
# the MEDIA is identical either way, so pair it with the native run's already
# flushed pack (same rule as lisp-lyric-web-snapshot).
.PHONY: smalltalk-web-snapshot
smalltalk-web-snapshot: $(WEB_NODE_JS) smalltalk-pack
	@test -r '$(SMALLTALK_SNAPSHOT_PACK)' || \
		(echo "Missing $(SMALLTALK_SNAPSHOT_PACK). Run 'make smalltalk-desktop-snapshot' first." && false)
	mkdir -p '$(WEB_SMALLTALK_RAW_DIR)' '$(WEB_LISP_ASSET_DIR)'
	cp -f '$(SMALLTALK_PACK)' '$(WEB_SMALLTALK_RAW_PACK)'
	chmod 644 '$(WEB_SMALLTALK_RAW_PACK)'
	DORADO_SNAPSHOT_NO_PACK_FLUSH=1 node '$(WEB_NODE_JS)' \
		--cycles '$(SMALLTALK_CYCLES)' --eb '$(SMALLTALK_EB)' \
		--disk 0='$(WEB_SMALLTALK_RAW_PACK)' \
		--boot-reason disk --no-alto-boot \
		--key-hold 3000000 --type-at 350000000 \
		--type 'Bootfrom xmsmall.boot\n' \
		--snapshot-out '$(WEB_SMALLTALK_RAW_SNAPSHOT)' \
		--out '$(WEB_SMALLTALK_RAW_DIR)/smalltalk76.pgm'
	cp -f '$(SMALLTALK_SNAPSHOT_PACK)' '$(WEB_SMALLTALK_RAW_PACK)'
	gzip -c '$(WEB_SMALLTALK_RAW_SNAPSHOT)' > '$(WEB_SMALLTALK_SNAPSHOT_GZ)'
	gzip -c '$(WEB_SMALLTALK_RAW_PACK)' > '$(WEB_SMALLTALK_PACK_GZ)'

$(WEB_SMALLTALK_SNAPSHOT_GZ) $(WEB_SMALLTALK_PACK_GZ):
	@echo "Missing WebAssembly Smalltalk-76 checkpoint assets."
	@echo "Run 'make smalltalk-desktop-snapshot smalltalk-web-snapshot'."
	@false

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
