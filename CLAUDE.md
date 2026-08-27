# Xerox Dorado Emulator Project

## Mission

Build a software emulator of the **Xerox Dorado** (PARC research workstation,
1978–1985), in two phases:

1. **Phase 1 — C emulator.** A correct, observable, single-binary emulator
   that can load original Dorado microcode (`.mb` files) and run an emulator
   microprogram (Mesa, Cedar, Lisp, Smalltalk, or Alto). Goal: boot the Alto
   OS via the Mesa microcode, then bring up a richer image.
2. **Phase 2 — Verilog implementation.** A synthesizable RTL design that
   matches the cycle-accurate behavior of the C emulator. Target FPGA.

To our knowledge no public Dorado emulator exists. The Dorado is the
follow-on to the Alto (the Star/8010 ships later as the productized cousin
of this lineage); designed primarily to host the Mesa virtual machine but
also runs Cedar, Interlisp-D, Smalltalk-76/80, and the Alto/BCPL emulator.

## Top-level architecture (one paragraph)

Dorado is a 16-bit, microprogrammed ECL machine with a 60 ns
microinstruction cycle. The CPU is split across five sections that live on
separate boards: **Processor** (ALU + register file), **Control**
(microinstruction sequencing, 16-way hardware tasking), **IFU**
(Instruction Fetch Unit — decodes opcodes from up to 4 instruction sets),
**Memory** (4K-word 4-way cache, virtual-to-physical Map, up to 4M words of
storage), and the **I/O controllers** (Disk, Ethernet, Display). A
**BaseBoard** with a 6502 microcontroller and 5 EPROMs handles power
sequencing, the boot button, the green status LED, and the initial
microcode bootstrap. Microcode lives in IM (4096 × 34-bit RAM), opcode
decoding in IFUM (1024 × 24-bit RAM, 256 entries × 4 instruction sets), and
register state in RM (256 × 16) plus an emulator-only stack STK (256 × 16).
Everything cool happens at the microcode level — emulators are themselves
microprograms.

## Current status and active focus (June 2026)

The microengine, memory subsystem, IFU, 16-way tasking, slow/fast-I/O
routing, and the BaseBoard 6502 model all work and are test-covered. The
full BaseBoard -> Bootstrap -> Initial boot chain runs. The microengine has
been thoroughly cross-checked against the board schematics
(`docs/schematic-audit.md`); the bring-up fixed six real microengine bugs
(WF/RF field opcodes, TisId/RisId + IFetch, Q<-B for Pipe sources, the
Overflow branch condition, shifter Pd-mux masking, and Return clobbering a
same-instruction explicit Link<- load -- the Cedar-desktop blocker; an
explicit Link<- overrides Return's Link<-CIA+1 reload, per DMesaFloat.mc).
**Speed, corrected and then fixed (2026-08-04/05).** The figures here used
to say 29.1 M microinstructions/s on Alto and 25.2 M on Cedar, "1.75x and
1.51x the real 16.67 MIPS Dorado". Those were **`m->bb.cycles` per
second** -- the BaseBoard 6502 counter, which advances 3.70 per Dorado
microinstruction -- reported as if they were microinstructions. The true
figure was 0.46x / 0.39x: we were running at *under half* the speed of the
hardware, which is exactly what a PARC veteran meant by "it feels slow".

Both paths now beat the real machine natively:

| path | native (`make pgo`) | wasm / browser |
|---|---|---|
| Alto (Galaxian) | **1.33x** | 0.74x |
| Cedar desktop | **1.43x** | 0.84x |

> **Re-measured 2026-08-15:** Alto's 1.33x reproduces exactly; **Cedar
> measured 1.37x, not 1.43x** -- and 1.37x on a PRE-session build too, so the
> gap predates the change under test (machine state or an unpinned workload,
> not a regression). Re-derive with a pinned workload before quoting. And
> **never A/B two separately-PGO'd builds**: profile variation alone is ~3%,
> the size of a real regression. See `docs/performance-methodology.md`.

`dorado` prints the honest number at the end of every run -- emulated
Dorado seconds per CPU second, from `cpu->cycles` (microinstructions),
never from bb.cycles -- so this cannot be misread again. **Quote that
line, never a cycles/s number.** The full account, with every measurement
and the traps that produced two wrong answers on the way, is
`docs/performance-plan.md`; the methodology worth reusing is
`docs/performance-methodology.md`.

The big items were: PGO + LTO (1.95x, and `make pgo` is opt-in because it
is a two-stage build), suppressing the idle BaseBoard 6502 which ran ~62x
faster than the real 1 MHz part (+19.7%), giving the germ I/O bridge a
poll cadence instead of running it every microinstruction (Cedar
0.93x -> 1.26x), and the trace-flag cliff, which every Cedar and Lisp
recipe was paying for its whole run (Cedar 0.48x -> 0.73x).

**The clocks themselves are correct**, which is the good news: the guest's
own `\RCLK` advanced 162.4 ms over an interval in which 2,704,685
microinstructions x 60 ns = 162.3 ms elapsed, and the junk task fires
every 533 microinstructions = the 32 us `Junk.mc` specifies. Nothing is
mis-clocked; the emulator is simply not fast enough yet.

The 2026-07-18/31 optimizations below were real (they are byte-identical
speedups); only their headline ratio was wrong:

- **The display-active predicate was asked once per microinstruction.**
  `machine_alto_display_active()` re-translates four VAs and walks the DCB
  chain to answer "has the loaded world installed a display list?" -- 24%
  of total runtime, more than the interpreter. It answers a question about
  the world's display list, which changes when a world installs one, and
  the scanline wakeup it gates already ran on a ~1000-cycle cadence. It is
  evaluated on that same cadence now and cached
  (`machine_display_active_cached`, a file-scope static because a new
  `dorado_machine` member would break the snapshot ABI).
- **`dorado_trace_flag` was 8%** even as a memoized hash: 256 call sites
  asking, per microinstruction, "is tracing off?" in 114 different ways.
  All 114 keys are `DORADO_`-prefixed, so when the environment holds no
  `DORADO_*` variable the answer is no for every one of them; one pass over
  `environ` at startup collapses the whole thing to an inlined load and
  branch (`include/trace.h`, which is also now the ONE declaration --
  memory.c, display.c and disk.c each carried their own `extern`).
  Note the cliff: setting ANY `DORADO_*` variable disables that fast path,
  so gate runs (which set `DORADO_PDI_IGNORE_LABEL_FLAGS` and
  `DORADO_FAKE_TIME`) still pay it -- the Cedar figure above is measured
  WITH it paid, so a plain Cedar run is faster still.

All three of those "remaining hot spots" -- the idle BaseBoard, the
per-cycle ethernet connection scan, and the guest reads behind
`dorado_visible_word_at_va` -- were taken on 2026-08-04/05, along with the
germ I/O bridge. **Nothing but the interpreter is left**: `execute_uinstr`,
`next_pc`, `task_schedule`, `b_bus`, `apply_lc`, `lc_write_address`, ~44%
together with no single item above 8%. Measure with `sample <pid>` against
BOTH a shipped and a plain `-O2` build (LTO inlining makes attribution
lie), and gate on BYTE-IDENTICAL framebuffers across Galaxian, the Cedar
desktop and Lyric. It also builds to
**WebAssembly**
(`make web`) and auto-deploys to GitHub Pages
(`.github/workflows/deploy-pages.yml`): a dropdown picks the Alto games,
NetExec, Mesa NetExec, Cedar 6.1 (saved desktop or login checkpoints), or
Interlisp-D Lyric.

There are **two software paths**; how to load every combination is in
`docs/running-the-emulator.md`.

**Path A - Alto-on-Dorado: WORKING.** Initial netboots `worlds/aemu.eb`
(the Alto emulator on Dorado), then the running Alto world software-boots an
Alto boot file over the in-process fake Pup/EFTP server
(`dorado/src/ethernet.c`; Mayday `244B` + EFTP `30B`/`31B`). Alto games and
the NetExec menu come up and render. This is the validated path and the
regression gate (`make run-galaxian` = 121553 px, `make run-netexec`, etc.).
(Note: the EFTP RxOn-clear must stay gated to the Cedar path -- clearing the
rx queue on every Alto RxOn toggle drops the held lock-step packet and
stalls the Alto boot mid-stream; see `src/ethernet.c eth_write`.)

**Path B - Cedar/Pilot: BOOTS TO THE VIEWERS DESKTOP (2026-07-15).**
Initial netboots `CedarDorado.eb!6` (the Cedar/Mesa microcode), the matched
Pilot germ is planted into VM (`--germ`), and Cedar boots its Pilot
**physical volume** from a PDI disk image (`--pilot-disk`, `--boot-reason
disk`). The full chain now runs from committed artifacts all the way up:
SimpleTerminal login (type `Guest` + two Returns), Grapevine-down fallback,
LoaderDriver transferring all 34 `Basic.Loadees` over the in-process STP
server, the Installer's `BootEssentials.df` closure, font DFs attached and
demand-fetched, every loadee STARTed, and **the Cedar 6.1.0 Viewers desktop
comes up live** — Ready. menu, CommandTool at a `%` prompt, WatchTool/
EditTool icons painted with real Tioga fonts (screenshot:
`docs/images/cedar-desktop-first-boot-2026-07-15.png`). Run it with
`make run-cedar-work` (the 65K-page work volume,
`CedarDisk/CedarDorado-work.pdi.gz`); `make run-cedar` still boots the
small login-only volume. **Germ and microcode versions must match** --
`CedarDorado.eb!6` (17-May-1984) shipped
with Cedar 5.3/6.0/6.1, so the matched germ is `Dorado.germ-6.1.6` (Cedar
6.1), NOT the older `Dorado.germ!4` (Dec 1983, version-mismatch fault).
Cedar's keyboard is delivered to KeyBits at absolute `LONG[177033B]` with a
display vertical-field naked-notify driving SimpleTerminalImpl's watcher
(`machine_cedar_io`; grounded in `TerminalDefs.mesa KeyName`,
`TerminalHeadDorado.mesa`, HM Table 24). The framebuffer is 1024x808 and each
world presents at its native raster (Alto 808x606, Cedar lf 1024x808). Live
detail + next steps: `docs/CONTINUE-HERE.md`.

**Beyond the desktop (2026-07-16):** the herald's `Boot`/`CedarWork`
buttons soft-reboot the volume live (germ re-entry works), and CedarChest
applications install into the running desktop the way PARC did it --
`Bringover [Cedar]<CedarChest6.1>Top>ChessHack` + `Run` against the
in-process STP server. `tools/fetch_cedarchest_app.py` mirrors packages
from the CHM archive; `tools/pbm2ais.py` converts the Dorado's own
schematic PDFs into AIS rasters the desktop's AISViewer can display.
First-hand background from two PARC veterans (PSAdd name lookup, build
genealogy, board/DA history; Cedar/CedarChest architecture and the
XC1-2-2 fonts): `docs/parc-veteran-notes.md`.

**The machine displays itself, and a friendly front door (2026-07-18).**
Three things landed together:

- **The Dorado draws its own schematic.** `ProcH-BitSlice07.ais` (the
  1979 processor-board sheet) paints in an open AIS Viewer on the Cedar
  desktop -- screenshot
  `docs/images/cedar-ais-proch-bitslice07-2026-07-18.png`. The blocker
  was an ethernet bug, not graphics: our STP server had no retransmit
  ring, so a Pup dropped while the guest receiver re-armed froze the
  client's ack forever and every transfer over ~100 KB wedged. BSP makes
  retransmission the SENDER's job; on a duplicate ack the server now
  rewinds the transmit cursor (during a retrieve the file itself is the
  ring). Same bug had wedged cold-boot BringOvers of the complete tree.
- **A friendly desktop.** The saved checkpoint boots with `.cm` command
  files fetched, CommandTool menu buttons created, and a printed menu:
  a visitor types `Schematic.cm` / `Moon.cm` / `Memo.cm` / `Chess.cm`
  (or clicks the matching button) instead of a 100-character `Eval`.
  These are Cedar's own mechanisms, copied from Xerox's sample profiles
  (`chm/cedar/cedar6.1-docs/StandardUser*.Profile`); ours live in
  `chm/cedar/stp-root/CedarChest6.1/DoradoWelcome/`.
- **Clipboard paste** (`Cmd/Ctrl+V`) in SDL and the browser, plus
  `--paste-at CYCLES --paste TEXT` headless, all through one paced
  typing queue (`src/typetext.c`).

**Three traps when authoring files for the guest** (all fail silently as
"1 files acted upon", i.e. the `.df` and nothing else): Cedar text files
are **CR-terminated** (LF makes the whole file one line to the parser);
**`Bringover -p` fetches public files only**, so data files (`.cm`,
images, documents) need a plain `Bringover`; and a **semicolon is a
command separator**, so it cannot appear in `Echo` text.

The Stage-2 EFTP/Mayday boot server also serves Cedar boot files
(`CedarNetExec.boot`, `NEWOS.BOOT`, `OthelloDorado.boot`) byte-exact -- the
next stage once the germ reaches `DoInLoad` over the net rather than the disk.

**Playable Chess offline + the Othello track mapped (2026-07-21).** Three
things landed:

- **ChessHack plays chess in Cedar, offline.** The board paints with all
  pieces in the Chess40 font and runs with zero network. This required
  cracking Cedar's font resolution: there is NO font catalog --
  `ImagerFont.Find` searches the FS name tree for
  `///Fonts/Xerox/TiogaFonts/<name>.*` and the Imager ERRORs unless the
  stored name carries the `Xerox` component, which fonts get ONLY from the
  installer's cold-boot attach. Fix = one CR line in the served
  `TiogaFonts.df` + `make cedar-desktop-snapshot`.
- **An offline apps demo (Path B).** rusty-backup injecting files onto the
  volume CRASHES Cedar's live FS, so the demo instead pre-loads ChessHack +
  Clock into the checkpoint's MEMORY and pre-caches the font; it restores to
  a clean desktop with app icons and opens them with no network. Native
  `make cedar-demo-snapshot`/`run-cedar-demo-sdl`; web dropdown "Cedar 6.1 --
  apps demo (Chess, Clock, offline)". The corpus browser checkpoint (1109
  CRC-recovered names) was also finally baked + deployed.
- **The Othello/Iago track is mapped.** Standalone `OthelloDorado.boot!8` is
  version/format-dead (`062400` vs the germ's `063000`; a one-byte patch
  clears `germBadBootFile` but the payload misparses). The authentic path is
  **Iago** inside the working Cedar 6.1 -- a boot-time installer gated by
  `Booting.switches[l]`, writing via normal Pilot IOCBs our write path
  models. The interpreter and herald-button routes to set that switch are
  ruled out; the remaining task is boot-time injection of
  `Booting.switches[l]=TRUE`. See memory `othello-dead-end-iago-is-the-path`,
  `cedar-font-install-attach`.

**Iago runs, and Cedar builds its own volumes (2026-07-25).** The
Othello/Iago track is open. Two things landed:

- **The disk bridge no longer assumes a link convention.** The germ's
  polled IOCB path decoded `PhysicalRoot.bootingInfo` links as flat PDI
  page numbers; Pilot -- and anything Othello or Iago installs -- writes a
  real CHS `DiskAddress`. It now decodes each link both ways at mount and
  keeps the reading whose target page label matches that entry's fileID and
  firstPage, so the medium decides. Every shipped volume still reads flat
  and boots bit-identically; `tools/pdi_boot_links_to_chs.py` produces the
  CHS form for testing (germ link `(3,20)`, the value the period notes
  record).
- **`--boot-switches` reaches Iago.** `GermSwap.Switch` is the enumeration
  `{zero..nine, a..z}`, so `l` is ordinal 21 (word 1, mask `0400B`), and
  `GermSwap.InLoad` plants switches at `PrincOps.SD[sBootSwitches]` = germ
  MDS + `1242B`. The plant must be repeated: GERMREMAP relocates the germ
  over that MDS a few million cycles later, so the emulator re-plants until
  the value sticks. Two things that hid this: `IagoMainImpl.DoIt` calls
  `IagoCommands.Login` ITSELF before the switch test, so the Cedar login
  prompt we have always seen IS Iago -- you must log in first; and
  `IagoOps.GetCommand` auto-completes on a space, so scripted input must
  type only through the token that makes a command unique.

Iago then drives our disk path for real: `Describe Machine` (Dorado, 16339
real pages), `Describe Drives` (RD0..RD3, 114100 pages each), `Describe
Physical Volumes` (reads CedarWork correctly), and **`Create Physical
Volume` + `Create Logical Volume` on a blank disk from
`tools/pdi_create_blank.py`** -- a real `PhysicalRoot` at page 0 and
`LogicalRoot` at page 3, 114096 pages, written entirely through the
emulated disk. Checkpoint `dorado/build/good-packs/cedar-iago.{snap,pdi}`
restores straight to Iago's `>` prompt.

**A Cedar volume built from a blank disk, with software on it
(2026-07-25).** The whole chain is three reproducible make targets:

```
make cedar-diskvolume           # 60 MB of zeros -> bootable Cedar volume
make cedar-diskvolume-desktop   # boot it; Cedar installs itself (167,732 px)
make cedar-diskvolume-software  # one Bringover per CedarChest package
```

Result: 158 files / 19.94 MB on the volume, and `List ///*.df` shows 75
DFs / 862 KB -- ten CedarChest packages carrying real 1985-86 dates
(Abutters, AIS, AISImages, AISViewer, CardTable, ChessHack, Clock,
DoradoArchive, PopUpButtons, ViewRec) beside the Cedar runtime DFs their
import closures pulled. Screenshots:
`docs/images/cedar-cleanlv-desktop-2026-07-25.png`,
`cedar-diskvolume-software-2026-07-25.png`.

Written against the period procedure. `ReleaseMessage.tioga!9` ("Installing
Cedar 6.1 from scratch") NET boots Cedar first so the local RD0 is free for
`Create User World` -- which is why that macro only ever targets RD0, and
why our RD1 build is the same document's "clients who wish to have
non-standard volumes will have to do the various steps by hand". The step
order is `Create User World`'s own, with ONE deliberate omission: **Install
Initial Microcode** writes 11 pages into the hard-microcode region that
`IagoOps.ReservePages` reserves only on drive 0, so on RD1 it lands on the
VAM header and the volume boots to "Local volume's permanent data
structures are inconsistent". The emulator supplies microcode with `--eb`,
so skipping it costs nothing.

Two things worth not rediscovering:

- **Bringover cost is the IMPORT CLOSURE, not the payload.** Interpress is
  47 files but pulls MesaRuntime, Rope, Real, BasicPackages, IO, FS -- most
  of the Cedar runtime -- and had not finished after 15 B cycles. The
  default package set is the self-contained ones at 6 B cycles each; the
  heavy set is a documented override in the recipe.
- **Cedar 6.1 still streams its base system.** `Basic.Loadees` names all 34
  loadees by UNVERSIONED remote path, which forces a server round trip, so
  a volume boots from disk and carries its own software but the base system
  comes off the file server every boot. That is 6.1's design: the fuller
  `CedarDorado.boot` exists only as 1983 builds (the same version wall that
  killed `OthelloDorado.boot`), and FS attachments cannot redirect a remote
  name (`FS.mesa`: `attachedTo` is a property of a LOCAL file).

**Superseded (2026-07-25; the block below is kept for its diagnosis, but
its headline claim is now false).** Smalltalk-76 boots (2026-07-28), and
as of 2026-08-11 our own `mb2eb` build of `DSemu.mb!1` boots it too --
`worlds/dorado-smalltalk.eb` reaches the same desktop at **124,945 px**,
the shipped-`.eb` gate figure. The "our mb2eb build renders 0 px" line
below predates the 2026-07-26/28 fixes and was never re-tested against
them. See `docs/smalltalk80-bootstrap.md`.

**Smalltalk: narrowed to one microcode branch (2026-07-25, open).** It is
not the image, the disk or the pack -- the DSemu-class world itself does
not come up: `worlds/aemu.eb` boots Galaxian over Ethernet at 121,549 px
while both `SmalltalkDorado.eb!1` and our `mb2eb` build of `DSemu.mb!1`
render 0 px on the same path. Per-PC counts put the failure between two
labels in `InitMem.mc`: all three map loops complete and `EndOfStorage` is
reached once, but `InitMemDone` is never reached. The gap is the **XM-only**
page-377 write-protect path -- `GetEmulatorMapParams` returns "1 to
write-protect page 377, for XM Alto emulation; 0 to emulate a non-XM
Alto", so AEmu branches straight past it and DSemu, the XM build, goes in
and never comes out. Confirmed sound on the way: `VirtualBanks` (0o400 via
our DMux model), the `DummyRef`+Pipe0/Pipe1 VA readback, `B←Pipe5'` and the
9-cycle MapBufBusy model, `B←Config'`, and `Carry20`. Detail and the next
probes: `docs/handoff.md`.

**Smalltalk boots and runs its interpreter (2026-07-26).**
`SmalltalkDorado.eb!1` now completes all of `InitMem` and enters the
Smalltalk bytecode interpreter (`0o421` = `LOADX`, 3.2 M executions;
`JUNKTASKLOOP` idling in task 2). Two defects were in the way. **Fixed:**
IFU opcode fetches were landing in `pipe[ProcSRN]`, so an autonomous
prefetch could overwrite the slot between a `DummyRef` and the microcode's
readback of `VALo`/`VAHi` -- `InitMem.mc`'s map walk restarted from VA=1
forever. `DM_REF_IFETCH` now selects ASRN, like IOFetch/IOStore (HM §5:
"the IFU is using the AS for an opcode-fetch reference"). **Also fixed:** the
display-wake predicate tested controller activity against zero rather than
against a LoadRam baseline. Initial drives the terminal task while it runs,
so `output_task_count` is already 386 at the handoff -- nonzero for every
world the instant it loads, which woke tasks 3/4 at TPCs the world had not
initialized yet, starving the emulator task. Measured over time, a real
Alto world grows that counter past 1,000,000 while DSemu never touches the
controller (386 -> 387, flat), so the predicate now asks "has the LOADED
world driven the controller?" against a baseline captured at
`ether_loaded_world_cycle`. With that, Smalltalk completes InitMem on
default settings. (The baseline is a file-scope static, not a
`dorado_machine` member -- a new member changes the snapshot ABI and every
baked checkpoint fails to restore; after a restore it stays zero and
degrades to the old test, keeping checkpoints bit-identical.) Gates: Alto
framebuffer byte-identical, `verify-cedar-desktop` unchanged, 11/11 tests.
Also found while
gating (open, undiagnosed): the **Cedar cold-boot login path is
non-deterministic** run to run (28,490 vs 28,494 px from the identical
binary), so it is not a byte-exact gate; the Alto path is deterministic.
Detail: `docs/CONTINUE-HERE.md`.

**SMALLTALK-76 BOOTS TO ITS DESKTOP (2026-07-28).** `make run-smalltalk`
brings up Top View, the Classes browser with its four panes, and a UserView
workspace -- the same screen ContrAlto reaches; `make verify-smalltalk` is
the headless gate (124,945 px). Screenshot:
`docs/images/smalltalk76-desktop-2026-07-28.png`. Booted the period way
(`Smalltalk.midas!17`, `DSemuRelease.cm`): `SmalltalkDorado.eb!1` (DSemu =
the Alto emulator PLUS the Smalltalk microcode, ether-boot BFN `111B`) on the
"XM Smalltalk" pack from `dsk2trident --all-heads`, started by the Executive's
`Bootfrom xmsmall.boot`. **Three emulator bugs were in the way, each grounded
in the manual or the period microcode.** (1) **`TgetsMd` was unimplemented**
-- HM Table 10: "The only missing combination is T<-Md, RM/STK<-Md ...
accomplished by combining an LC value of 5 with the TgetsMd FF decode"
(Table 11a FA=0 FB=7 FC=5). With it stubbed, LC=5 always took T from Pd, and
the stand-in was a same-instruction Md bypass that corrupted every OTHER
LC=5+BSEL=RM/STK instruction -- including Dorado Smalltalk's recursive freer
(`DSmallsubrs.mc Recuf`, `T _ Arg1, Arg1 _ Md`), which then walked to
Father=-1 and object-faulted out on the 22nd bytecode. Implementing TgetsMd
and retiring the bypass took instruction-set-1 dispatches from 22 to hundreds
of thousands. (2) **The display word task's XM bank register**: Smalltalk
writes `0177751 <- 4` and its 640x800 bitmap lives in bank 1 while the DCBs
stay in bank 0 (`XMFaultTask.mc DispXM`); our rasteriser read bank 0, which
is what earlier sessions recorded as "the screen is noise". (3) **UTILIN
(`0o177030-3`) was only seeded when a mouse was attached** -- it is active
low, so an unseeded cell reads as every keyset key held and Smalltalk stops
with "The keyset is stuck". Gates: Alto Galaxian byte-identical,
`verify-cedar-desktop` 246,086 px, 11/11 tests. **Interaction WORKS**
(corrected 2026-08-15; the "no click/keystroke driven into the desktop yet"
note here was true when written and went stale): a user drives it
interactively, and `make verify-smalltalk-input` clicks `'Kernel Classes'`
in the browser -- it selects, and the second pane fills -- 128,547 px against
a 124,945 px unclicked desktop. Likely unblocked by the 2026-08-07 terminal
word-4 mouse fix, since Smalltalk reads buttons from the same UTILIN cell as
the Interlisp menus. Still open: the Alto OS's XM sizing probe that ContrAlto
runs and we do not, and re-running the Interlisp gate (its pack is a missing
build artifact here; that world is argued statically).

**Two long-standing regressions closed, both artifacts of over-broad
assumptions (2026-07-28).**

- **The Alto Executive boots from disk again (2,092 px).** It was never an
  emulator bug: `3fe8ae1` changed `dsk2trident`'s default `--sector-offset`
  from 1 to 0 and the checked-in `chm/diskpacks/games-trident.pack.gz` was
  never regenerated, so every sector sat one slot late (old slot n+1 == new
  slot n, visible by diffing the two packs). Regenerating the pack fixed it
  with zero source changes. Gate: `make verify-alto-disk`.
- **Five Mesa-world programs come back from 0 px.** Bisected to `40e6491`,
  which extended the one-shot cold-Alto init to real PC `0o2006` whenever
  `alto_ether_boot` was set. `0o2006` is EBoot in `AEmu.mb` but an unrelated
  instruction in `AltoMesaDorado.eb`, and `alto_ether_boot` only means
  "Initial netbooted the world" -- equally true of the Mesa world -- so the
  init wiped a running Mesa NetExec's stack and I/O page. The gate now keys
  on the served image's format word, which `ethernet.c` already decodes
  (`0o405` = Alto B-format, `0o345` = Mesa outload). MesaNetExec 1,513 px
  (herald + prompt), PPong 323,184, TriEx 20,787, Pupwatch 6,669, MazeWar
  2,590.

- **Interlisp-D Lyric boots to its Exec (XCL) desktop again (208,966 px).**
  Its 4.8 MB sysout retrieve had wedged since 2026-07-11:
  `eth_ftp_queue_file_chunk` sends up to 1,478-byte data Pups (what Cedar
  advertises as a directly connected host) and only clamps once the client's
  allocation is known -- which arrives in its first BSP ack. The Alto-side
  client never gets there; the first data Pup is already oversized, it drops
  it, never acks, and the window fills. Default is 512 now (the quantum this
  server advertises itself), bigger only for a client claiming more than a
  standard Pup carries. Honoring the client's own advertisement is NOT
  enough -- it says 532 and still drops 532 (2.5 B-cycle trace lines:
  1478 -> 68 wedged, 532 -> 141 crawling, 512 -> 22,809 streaming).

All three had the same shape: a fact about HOW something was loaded or
connected was used as if it said WHAT was on the other end.

**`ls` works on the emulated file server (2026-07-30).** A Cedar author
tried the emulator, said it was "really close", and asked for two things:
host Cedar 6.1 so you can `ls` a release directory, and make `OpenR` work.
`OpenR` turned out not to be broken -- it works natively, including the
author's own `OpenR TiogaDoc` -- but `ls` returned nothing, silently:
`FSRemoteFileImpl.InnerEnumerate` swallows `noSuchFile`, so a missing
directory and an empty one look identical. Our STP server had no Enumerate
at all (`markNewDirectory` 0o14 was aliased onto Retrieve; `markDirectory`
0o12 was not decoded, so an old-style client got no reply whatsoever).
`List [Cedar]<Cedar6.1>VersionMap>*` now prints all eleven files with their
true versions, sizes and dates -- `CedarSource.VersionMap!34  66296
04-Dec-86 13:05:32 EST`, and a summary line that totals correctly
(screenshot `docs/images/cedar-ls-versionmap-2026-07-30.png`; gate
`make verify-cedar-ls`). Three things were needed, all in
`src/ethernet.c`:

- **Enumerate**, framed exactly as `STPServerImpl.mesa!9 DoFiles` does it:
  HereIsPList once for New-Directory and before every entry for the old
  Directory, one bare plist per match, one EOC; `fileNotFound` only when
  nothing matched (any other reply code makes the client fall back to the
  old form). Cedar's `*` crosses `>` -- CommandTool's List has an `X`
  switch, "causes * to not match >", precisely because that is the default.
- **Real versions**, kept from the DFs the date index already parsed and
  threw away. Both the listing and the retrieve plist report them.
- **The LookupFile packet exchange** (`PupType.fileLookup`, 0o200), which
  this was not looking for. The first end-to-end listing printed all eleven
  NAMES and then junk -- `MakeVersionMap.cm!257  32 ??`. Our plists carried
  the truth; `List` was not reading it from there.
  `FSRemoteFileImpl.Info` tries a single-Pup exchange FIRST and only falls
  back to an STP enumerate if nothing answers, and unanswered,
  `FSFileLookupImpl` caches `noResponse` against the whole server for 30
  seconds. Reply body is version (1 word), createTime (2 words, seconds
  since the 1901 Pup epoch), length (2 words). It arrives on **socket 3**,
  not `PupWKS.fileLookup` (0o61), because our NetDir handler answers every
  name lookup with the STP address -- so it is answered by Pup type
  wherever it lands.

Two things this cost that are worth not re-learning. **A pixel count cannot
gate a listing**: the CommandTool typescript is already full, so a listing
scrolls rather than adding ink -- 167,129 px with the listing on screen
against 167,192 px for a listing of a directory that does not exist. The
gate reads both ends of the wire out of the trace instead: `STP_ENUM` for
what we sent, `STP_LOOKUP` for the guest naming those files back to us,
which it can only do having received them. And **enumeration state had to be
file-scope static** -- a new `dorado_ethernet` member changes the snapshot
ABI and every baked checkpoint fails to restore -- with the per-connection
cursor kept in `ftp_file_pos`, which the context switch already saves.
Gates: 12/12 tests (`test_ethernet` now plays the client half of
`STPImpl.TryNewDirectory` over real BSP and fails on `49332c3`),
`verify-cedar-desktop` 245,711 px.

**And the release reaches the browser.** The deployed page served 2 of
Cedar6.1's 70 directories, which is why `OpenR` silently opened the wrong
file there. The stated blocker -- "the full tree would push `index.data`
past GitHub's 100 MB per-file limit" -- was not real: `dorado/web/` is
gitignored, `make web` runs in CI and uploads through
`actions/upload-pages-artifact`, so `index.data` never enters git and git's
limit cannot bind it. (It measures 102,565,493 bytes = 97.81 MiB.) What
actually constrains it is that Emscripten preloads it in full before the
page boots, so anything added lands on every visitor. So the release ships
as a SEPARATE `web/cedar-src.tar.gz` fetched in the background only when a
Cedar world is chosen and unpacked into MEMFS at `/stp` -- a tar because it
is the one archive a browser can walk with ~30 lines of JS and no library,
gzip because `DecompressionStream` was already in the shell for the
snapshots. Contents are a DENY-list (everything but `.bcd` and the two
already-preloaded directories), not an allow-list of source extensions: the
first cut listed `.mesa/.tioga/.df/.cm/.config` and silently dropped both
version maps -- `CedarSource.VersionMap` is what `OpenR` resolves short
names through -- the Tioga `.style` files, and every capitalised variant
(`.Mesa`, `.Tioga`, `.Profile`), because `find -name` is case-sensitive.
963 files, 62 directories, 2.93 MiB. Verified in Chrome: `/stp/Cedar6.1`
goes from 2 directories to 64, `List [Cedar]<Cedar6.1>VersionMap>*` prints,
and `OpenR TiogaDoc` opens a viewer TITLED
`[Cedar]<Cedar6.1>Documentation>TiogaDoc.tioga!1` -- the title, not the
`Opened:` line, because `TryExtensions` will happily open a different file
and still say `Opened:`. Screenshot:
`docs/images/cedar-browser-ls-openr-2026-07-30.png`. Detail:
`docs/cedar-file-server-plan.md` §7.

(Toolchain note: emsdk is already installed at `~/emsdk` with emcc 6.0.0,
the version `.github/workflows/deploy-pages.yml` pins; it is not on PATH
until `source ~/emsdk/emsdk_env.sh`, and no shell profile sources it.)

**A FULL-LIBRARY Interlisp-D world boots, and it was the wrong sysout all
along (2026-08-07).** `Released-Full.sysout!2` (Jan-88) comes up to a live
Exec at 209,188 px -- `Xerox Lisp 25-Jan-88`, INIT.LCOM, INITCOMS, "Good
afternoon." -- carrying ~2,300 pages more preloaded library than the shipped
world. `make run-lisp-lyric-full-snapshot-sdl` restores it in seconds; the
browser dropdown has "Lyric -- full-library sysout", deployed. The months of
effort against **`Full.sysout!6`** were spent on an image that cannot work
on a stock machine: it is dated **17-Nov-1988**, nineteen months after the
Lyric release, and uses 894 virtual pages above **2^14** where both
`LISP.SYSOUT!1` and `Released-Full.sysout!2` stop at exactly 16,383. 2^14
pages x 256 words is a 22-bit address space, exactly what the period BCPL
assumes (`VMem.decl`: `LastVirtualPage = #37777`, `EMPTY = #40000`,
"assumes 22-bit addresses!"). `Full.sysout!6` needs a machine running
Nick Briggs's **ExtendedVmem** rebuilt, not an emulator fix -- his module
and manual are recovered into `chm/lisp/lyric-lispusers/`. Ten hypotheses
were eliminated by measurement first (VMEM size at four values including
PARC's own 20,000, the clock, transfer truncation, stale PROCESS records,
microcode and loader versions, the pack itself, RealPages wrap); the image
is delivered **byte-perfect**, 15,140 of 15,141 pages identical on disk.
Full account: `docs/lisp-leaf-handoff.md` §6.1-6.18. **The lesson worth
keeping: I asked "why does this artifact fail?" for a long time before
asking "is this the right artifact?" -- the answer was in the release
message and a directory listing.**

**Feedback from the PARC veterans, turned into work
(`docs/parc-feedback-todo.md`, 2026-08-07).** Two of their remarks
corrected us. **`DispM` is the COLOUR board and `DispY` the MONOCHROME
one** -- confirmed from the Hardware Manual (doc p.110): "on a Dorado with
only a 7-wire terminal and no color monitor, only the DispY board is
present"; `docs/color-graphics-todo.md` had asserted the opposite and
scoped its whole estimate from it. And the **keyboard mapping is now
specified**: Alto HW Manual doc p.27 (PDF p.34) gives four words at
`KBDAD = 177034B` with **depressed = 0**, and the Dorado manual's **Table
24** (doc p.117 = PDF p.124) shows the terminal microcomputer -- the
"processor in the keyboard" -- serialising keyboard words to
`177034B`-`177037B`, mouse/keyset to `177033B`, and X/Y as excess-200B
deltas. Also mapped: the **Sil design-automation tools** survive complete
in `_cd6_/sil` (`ANALYZE.RUN` + BCPL source, `ECLDICT.ANALYZE`, `BUILD`,
`GOBBLE`, **`DORADODESIGNAUTOMATION.CM`**), which Tim recommends as the
route to the Verilog since the Sil files were the design input and tracked
bug fixes -- blocked only by our holding no `.sil` files, just rendered
PDFs, while 293 Dorado-named ones sit in the archive.

**The Interlisp menu bug is FIXED, and it was two writers of one cell
(2026-08-07).** The Alto-terminal microcode stores the mouse/keyset word
into `0177030`/`0177033` every ~160,000 cycles (HM Table 24, message
`05B`), and we never drove the modelled back-channel's word 4 from the
mouse -- so it reported ALL BUTTONS UP while `machine_seed_utilin` poked
the real state into the same cell. A few times a second the guest sampled
the microcode's value; in Interlisp one all-up restarts
`\domousechording`, which by its own comment "ignores the down bit for
now" and holds the VIRTUAL utilin up for a full
`\mousechordmilliseconds`, so `MENU.HANDLER`'s `until (MOUSESTATE UP)`
fires on whatever item the pointer is over. `dorado_machine_set_mouse`
now sets terminal word 4 too. Menus navigate, items highlight, and
**submenus open** -- `EXEC>` gives `Xerox Common Lisp / Common Lisp /
Interlisp` (`docs/images/lisp-exec-submenu-2026-08-07.png`). Gate:
`make verify-lisp-menu`.

**What made this take five wrong theories: `MOUSESTATE` reads none of the
cells we were tracing.** Buttons reach it three hops downstream via
`\lastkeystate`, and position comes from `\em.cursorx/cursory` =
**`0426/0427`**, not the `0424/0425` we write. The virtual utilin is at
**`0o1400074`** (`\InterfacePage` = space 6 base 0 = word 393216,
`FAKEMOUSEBITS` = word 60 of `IFPAGELAYOUT`), self-checked in the guest
against `MachineType`=5=`\DORADO` and `FAKEKBDAD4/5`=`0177777`.
`DORADO_LISP_MOUSE_CHAIN=1` prints the whole chain.

**Keyboard audited, Control fixed (2026-08-07).** The 61-key matrix in
`display.c` is correct in **every** position, checked against three
independent sources: Alto HW Manual Figure 6, ContrAlto, and Cedar's own
`TerminalDefs.mesa KeyName` -- which also settles Figure 6's one
ambiguous row (word 2 bit 10 = Period, bit 11 = SemiColon), names the
three unmarked keys from the Alto keytops (Look / Next / Swat), and
confirms `KeyBits` starts **at** `177033`. The bugs were elsewhere: the
browser dropped **every** Ctrl+key event before it reached C (that is the
whole of "the Control key does not work"; native was never broken --
Ctrl-W deletes a word in the Lyric Exec, gate `make verify-ctrl`); Caps
Lock reached no host key in either frontend and is now mirrored as a
LATCH; `←`, LF and the three blank keys had no host key at all; and
`--type`/paste could not send TAB, ESC, DEL or BS. Cedar middle-click is
NOT a button-number error -- `Red(13) Blue(14) Yellow(15)` match our bits
exactly and Cedar has no second writer of `177033`. Detail and the
retracted theories: `docs/parc-feedback-todo.md` section A.

**Interlisp-D reads its own manual over the network (2026-08-01..04).**
The Lyric pack grew from 119 to 204 library packages (chosen by
dependency closure, `tools/lisp_pack_closure.py`), and **Leaf** — the IFS
random-access file protocol, Pup 0o260 — is served in-process
(`src/ethernet.c`), which lifts the pack's hard 22,736-page ceiling:
`make verify-lisp-leaf` watches a package that is NOT on the disk stream
in over the wire. On top of that, **the 1987 Interlisp-D Reference
Manual opens on screen**: `(IL:FILESLOAD HELPSYS)` then
`(IL:LOAD '{DORADO}<IRM>IRMDEMO)` (login Guest/Guest) shows the CAR page
— hash-index lookup in the 331 KB `IRM.HASHFILE` by random access, the
chapter streamed, TEdit rules drawn
(`docs/images/lisp-irm-car-dinfo-2026-08-03.png`). Three root causes on
the way, all in `docs/lisp-leaf-handoff.md` §5: Interlisp `(* ...)`
comments are PARSED, and one `HELPSYS:` token read as a package prefix
silently killed the init file at greet; DInfo drops an empty `<>`
directory when rebuilding node names, so the IRM lives in a served
`<IRM>` subdirectory; and the server was not answering **IFS leader-page
reads** (dates/name/author at 27-bit addresses >= 2^27-2048, made at
EVERY open) — the client parsed uninitialized packet buffer,
deterministically, while the wire looked clean. The browser build ships
the same world: the wasm checkpoint, pack and served tree are all
fetched lazily when the Lyric entry is chosen (index.data SHRANK ~6 MB),
and the deploy is live. Beware: `dorado_machine_restore` clobbers the
ethernet state with the bake-time ftp root — every web boot must re-apply
`dorado_machine_set_ftp_source` after restore.

**Superseded note (2026-07-28, earlier in the same session):** Booted the period way -- `SmalltalkDorado.eb!1` plus
the `xmsmall.dsk` pack converted with `dsk2trident --all-heads`, then the
Executive's `Bootfrom xmsmall.boot` -- DSemu loads the whole image (2674
sector reads, last page `next=0 nbytes=0`), enters the Smalltalk instruction
set at cycle 627.07 M and runs real bytecodes: the microcode trace shows
`SNDMSG`, `HASH`, `REFCKINC/DEC`, `ALLOC/DEALOC`, `MAPCODE`, `RTRN`. After 22
bytecodes and four `NovaCall`/`NovaRet` round trips a ROT probe comes up
empty -- `HASH` -> `.Emp` -> `.EmpN` -> `NovaCall(34C)`, the normal
object-fault call-out to the Alto-side swapper at Alto PC `0016666` -- and
that call never returns; the machine ends in the OS disk-wait spin. **Two
long-standing conclusions were wrong and are retracted**: `worlds/aemu.eb`
cannot host Smalltalk at all (ATraps.mc traps `0o72400-0o72777` =
`ReadWriteR`, and its `VERS` reports build 0), so the entire disk
"check error" investigation was chasing an unsupported configuration; and
"the label block delivers 8 words but only 5 land" is correct Alto behaviour
(`ANoCheckWord`: a zero word in a check block is a wildcard the controller
fills from disk). New `DORADO_ALTO_OPHIST=1` gives per-instruction-set,
per-opcode dispatch counts. Detail and next steps: `docs/CONTINUE-HERE.md`.

**THE COLOUR BOARD WORKS (2026-08-08).** `src/dispm.c` models DispM -- the
Dorado's second, colour display board -- transcribed from Xerox's own driver,
which was in the tree we already serve the guest: `Cedar6.1/HeadsDorado/
ColorDisplayDorado.mesa` (registers, control blocks) and
`ColorDisplayHeadDorado.mesa` (the presence test), cross-checked against the
board's netlist. Cedar detects it on a cold boot (`presence reads: 360B=3
361B=1`, head latches `standard 640x480`), `ColorDisplay on` arms the ColorCSB
chain at 177414B, and viewers moved onto the colour screen render. Two
surfaces: an SDL second window and a browser second canvas -- **a Dorado's
colour monitor is a SEPARATE SCREEN at its own raster (640x480 or 1024x768),
not a colourisation of the 1024x808 mono one, which is genuinely 1 bit per
pixel.** `make run-cedar-color`. The bug that kept it black is worth knowing:
**the pixel value is not the table index** -- ATable has 1024 entries because
the index is TEN bits (`aaaaaaaabb`), so at 8 bpp it is `a<<2`.

Two threads are one step from done, both with a precise next action in
`docs/CONTINUE-HERE.md`: **Gargoyle** (Tim made his 1980s railroad book's
artwork in it -- import closure complete at 52 DFs, `Install` fixed, remaining
failures all caused by `Bringover -p` fetching public files only, now
corrected, with the missing 6.1 BiScrollers interface recovered from Paul’s
export but the guest run still needing validation) and **the colour cursor**
(software-composited, each screen 0-based, crossing by EDGE-PUSH, registered
via the ColorDisplayTool's SIDE toggle rather than by turning colour on; the
native frontends now use absolute motion on monochrome and terminal deltas
only on colour, with no environment flag). First-hand background from Tim on
how the book was
actually made -- colour monitor as an antialiasing soft-proofing device,
Interpress to a Dover then to film, photographs as HOLES in the page masters
-- is in `docs/parc-veteran-notes.md`.

**CEDAR CHECKPOINT / ROLLBACK WORKS (2026-08-14).** The crash a PARC veteran
reported is fixed end to end: `Checkpoint` at the CommandTool writes the
checkpoint, reboots, rolls back and **returns to the live desktop**, printing
Cedar's own `Creating checkpoint at ...` / `Rollback at ...` pair
(`docs/images/cedar-checkpoint-rollback-restored-2026-08-14.png`; gate
`make cedar-checkpoint-repro`, 162,855 px). Two bugs, both in the **polled**
germ IOCB arm, which until now only boot chains had exercised -- and boot
chains only read. It ignored the command's Action fields, so the outload's
writes (`cmd=0o100244` = `[check,check,WRITE]`) were serviced as reads; and a
file created at RUNTIME carries Pilot's CHS DiskAddresses even on a volume
whose stored links are flat, which flat-decoding collapsed onto 111
overlapping pages instead of 2,365 consecutive ones. New instrument:
**`DORADO_MP_TRACE`** decodes Pilot's maintenance-panel codes out of the
cursor bitmap at `LONG[431B]` through Xerox's own `digitFont` -- turn it on
FIRST for anything boot- or outload-shaped. **Budget >= 48 B cycles** (the gate uses 54 B): the
inload finishes at ~40.4 B but the resumed world does not repaint until
~47.5 B, and a shorter run looks exactly like a failure. (Both moved out on
2026-08-15 when `CEDAR_FIELD_INTERVAL_CYCLES` was corrected from an effective
222 Hz to a true 60 Hz -- the germ's disk work is unmoved, but Cedar's own
field-paced restore now takes the authentic 3.7x longer.)
Detail: `docs/cedar-checkpoint.md`.

Plans/state: `docs/running-the-emulator.md` (how to run everything),
`docs/CONTINUE-HERE.md` (live bring-up state), `docs/handoff.md` +
`dorado/CLAUDE.md` (gaps), `docs/hardware-specs.md` (specs for unbuilt
hardware), `docs/ethernet-local-boot-plan.md` (Stage-2 plan). The newest
Dorado Cedar world that exists is 6.1 (`chm/cedar/cedar6.1/`); Cedar 7 was
never built for Dorado.

## Why this is hard (read before scoping)

- **No CPU instruction set in the conventional sense.** Dorado executes
  34-bit horizontal microinstructions. The "Mesa instruction set" or "Alto
  instruction set" is not implemented in hardware — it is implemented by
  loading a different microprogram into IM. The emulator must therefore
  implement the *microengine*, not just an opcode interpreter.
- **Sixteen concurrent microcode tasks.** The hardware switches tasks
  every microinstruction based on I/O wakeup requests. Per-task state
  (T, TPC, MemBase, Link) is replicated. The C emulator must model this
  scheduler accurately or I/O microcode will deadlock.
- **The pipe matters.** Memory references leave trace info in a 16-entry
  Pipe memory that microcode reads back. Several microprograms depend on
  this; you cannot ignore it.
- **The IFU runs in parallel with the processor**, prefetching and
  pre-decoding the next opcode against base register 31. Branches in
  emulator microcode use `IFUJump` which consumes the prefetch.
- **ECC, faults, breakpoints, performance counters** are all visible to
  microcode. A useful emulator models faults; a complete one models
  timing.

## Repository layout (current, before any code is written)

```
Dorado/
├── CLAUDE.md                       ← this file
├── docs/
│   ├── INDEX.md                    ← human-readable map of every artifact
│   ├── architecture.md             ← distilled architecture notes
│   ├── memory-architecture.md      ← memory subsystem reference (cache/Map/Pipe/BR)
│   ├── jcn-encoding.md             ← JCN field encoding (HM Figure 6)
│   ├── chm-archive.md              ← URLs into the CHM mirrors
│   ├── chm-urls.md                 ← URL convention + curl recipes
│   ├── altoinfo-survey.md          ← what's reusable from AltoInfo/
│   └── glossary.md                 ← Dorado-specific jargon
├── DoradoDocs/                     ← all reference documents
│   ├── manuals/                    Hardware Manual (★), Booting memo, DoradoManual A/B/Figs, Board_Revisions
│   ├── schematics/                 16 canonical board PDFs (Backplane, Proc{H,L}, Mem{C,D,X}, IFU, …) + symbol libraries
│   ├── photos/                     board photos + EPROM chip-label photo
│   ├── doradodrawings/             every revision of every board (~50 PDFs, history)
│   └── doradologic/                additional logic docs
├── chm/                            ← downloads from CHM (xeroxalto + xeroxparcarchive)
│   ├── cross-reference.html        full PARC IFS filename → archive index (grep this)
│   ├── microcode/                  17 .eb/.pb microcode worlds (Dorado + D0/DLion siblings)
│   ├── microd/                     full MicroD BCPL source (.MB serializer)
│   ├── microd-manual/              MicroD user manual + Alto executable
│   ├── dorado/                  ★  canonical Dorado tree from xeroxparcarchive _cd8_/dorado/
│   │                               • Mesa.mb, Cedar.mb, AEmu.mb, DSemu.mb, TriEmu.mb, TriMesa.mb
│   │                               • All Initial*.eb stages, LoadMB.run, ReadMB.run
│   │                               • Bootstrap.mb + Initial.mb (in expanded/bootstrap.dm!20_)
│   │                               • doradobaserom.mb!13 + .masm sources (in expanded/doradobaserom.dm!12_)
│   │                               • Ifu.mb (Complex + Simple), kernel.mb, memA.mb, memMisc.mb, TriconD.mb
│   │                               • UnBug.bfs!1_ — 117 files incl. DoradoLisp.MB (only Lisp microcode found)
│   │                               • ftest.dm!1_ — full Micro→MicroD→.MB worked example (read first!)
│   ├── io-doradologic/             High-value selections from io/doradologic (DoradoBaseDebug, manifests)
│   └── indigo-dorado/              Just the directory index (older file versions)
│   See docs/chm-archive.md §7 for the full inventory and docs/chm-urls.md for the URL convention.
├── firmware/                       ← BaseBoard EPROM dumps (Nov 24 1987)
│   ├── B-08.BIN, B-10.BIN          BaseBoard 6502 code (F000–FFFF)
│   ├── C-08.BIN, C-10.BIN, C-12.BIN  Initial microcode + tables (C000–D7FF)
│   └── labels.JPG                  photo of chips in their sockets
├── dorado/                         ← the C emulator (in progress)
│   ├── CLAUDE.md                   the in-directory guide (read this when working here)
│   ├── Makefile                    `make` / `make test`
│   ├── compile_commands.json
│   ├── include/{mb,disasm}.h       loader + decoder APIs
│   ├── src/mb.c                    .MB loader (handles all 40 microcode files we have)
│   ├── src/disasm.c                model-1 unshuffle + microinstruction field decode
│   ├── src/mbdis.c                 dumper / symbolic disassembler CLI (mbdis [-d] file.MB)
│   └── tests/                      integration tests vs ftest.MB and all microcode in chm/
├── AltoInfo/                       ← Alto emulator references (NOT Dorado, but useful) — gitignored
│   ├── Contralto2-2.0-Beta/        ★ ContrAlto v2 source (C#) — Trident drive code lifts from here
│   ├── salto/                      ★ "salto" source (C) — microengine layout reference
│   ├── ContrAlto-mono/             pre-built binaries (validation oracle)
    └── ContrAlto2-beta/            pre-built binaries + spruce-server.dsk300 T-300 image
        (see docs/altoinfo-survey.md for what's reusable)
```

The 1981 Hardware Manual mentions 4 EPROMs at F000, F800, C000, C800. The
1987 firmware set has a 5th chip at D000–D7FF — i.e. the Initial microcode
or its tables grew past 4KB at some point and got an extension socket.

## Key reference points

| Topic                       | Where to look                                                   |
|-----------------------------|-----------------------------------------------------------------|
| Microinstruction format     | Hardware Manual §2.4 "Instruction Fields", Table 5              |
| Processor data path         | Hardware Manual §3, Table 3, Figure 1                           |
| ALU/ALUF/ALUFM              | Hardware Manual §3.7, Table 9                                   |
| RM/STK addressing           | Hardware Manual §3.1, Table 6                                   |
| Branching / JCN             | Hardware Manual §4.3–4.4, Table 13                              |
| Tasking                     | Hardware Manual §4.1–4.2, Table 22                              |
| Cache + Map + Pipe          | Hardware Manual §5.5–5.8, §5.11, Tables 16–17                   |
| IFU operation               | Hardware Manual §6, Table 18 "IFUM Fields"                      |
| Slow IO (TIOA/IOB)          | Hardware Manual §7, Table 21                                    |
| Fast IO (Fin/Fout)          | Hardware Manual §8                                              |
| Disk / Ethernet / Display   | Hardware Manual §9, §10, §11                                    |
| Boot mechanism              | Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf               |
| Crib sheet                  | Hardware Manual Figure 17 (last page)                           |

The big six microinstruction fields you will be implementing are
**RSTK, BSEL, ASEL, ALUF, LC, FF** plus the **JCN** (jump condition / next
address) and **BLOCK** bit. ALU operations are not encoded directly —
ALUF is a 4-bit pointer into ALUFM (16 × 6 bits) which the microcoder
loads at startup with the 16 ALU operations actually used by that
emulator.

## Build plan (dependency order)

These are coarse milestones — flesh out into tasks as you go.

1. **Decoder + disassembler** for 34-bit microinstructions. Parse a `.mb`
   file (MicroD output) into IM[0..4095] of decoded fields. Produce a
   listing with symbolic field names.
2. **Microengine core** — single-task, no IFU, no memory: ALU + RM + T +
   Q + Cnt + Shifter + JCN branching. Run a hand-written test microprogram
   that adds two numbers and halts.
3. **Memory subsystem** — Map, cache, Pipe, BR, MemBase. Honor
   `Fetch←/Store←/IFetch←` cycle counts. No ECC yet.
4. **IFU** — IFUM decoding, prefetch, IFUJump, four instruction sets.
   Run the Alto emulator microcode and execute a simple Alto program.
5. **Tasking** — 16 priority levels, wakeup latches, task-switch save/
   restore of T/TPC/MemBase/Link, hold logic.
6. **I/O controllers** — start with Display (DispM monochrome, easiest);
   then Disk (T-80 SMD, needed to boot anything real); then Ethernet
   (3 Mb/s Alto-style first).
7. **BaseBoard model** — minimal: respond to boot button, accept the
   Bootstrap microprogram via CPReg, hand off to Initial microcode. The
   real BaseBoard runs 6502 code from the EPROMs we have; we can either
   emulate the 6502 too, or stub the BaseBoard at the protocol level. Stub
   first, 6502-emulate later if needed for correctness.
8. **Bring up Mesa.** Load Mesa.mb, point it at a Trident disk image,
   and try to get an Alto OS hello-world out of the display.

For each milestone, prefer testing against original Xerox microcode rather
than synthetic test inputs — there are 30+ years of well-debugged
microprograms; we want bug parity with hardware, not with our own
intuitions.

## Phase 2 (Verilog) — still later, but the input is now in hand

Hold off on RTL until the C emulator boots something real. The C
emulator's structure (per-section modules, cycle-tick driver, Pipe and
ALUFM as explicit memories) should map fairly directly to Verilog modules
later. Avoid C tricks that won't translate (function pointers per opcode,
bit-fields with implementation-defined layout) in code paths intended to
become RTL.

**What changed (2026-08-08): we no longer have to infer the logic.**
`chm/sil/` holds PARC's own design-automation output for all sixteen
boards, and the `.wl` wire lists are the **gate-level netlist** in plain
text -- every net by name (`ALUCarry`, `ALUF.0`), every package and pin it
touches, and the DIRECTION of each pin (`f17.15o` drives, `g17.12i`
receives), plus the backplane pins where it leaves the board. With `.lc`
mapping packages to part numbers, that is a complete structural design.

Sizing: 5,563 packages across the machine, 127 part types; strip
terminators and spares and it is **3,026 logic packages in 118 types, of
which 50 types cover 90%**, plus 7 memory/PROM types (745 packages) that
become inferred RAMs. The parts are MECL 10K with published truth tables.
So the structural RTL is: ~50 cell models + a netlist-to-Verilog
generator, with each board's `.nl` file supplying the module's port list.

And the testing position is unusually strong -- the C emulator boots five
operating systems and already passes six of PARC's own hardware
diagnostics, which were written to test the boards.

**Started 2026-08-15, and further than the plan expected.** All sixteen
boards now GENERATE from PARC's wire lists and elaborate under Verilator
(67,960 lines, plus the cell models); the cell library covers 97.7% of the
eleven-board machine's logic packages as of 2026-08-18; **the BaseBoard's eight 2716 sockets now hold the real base ROM** --
the socket map was derived from the LS138 decode and the c07 strap block and
then validated three ways (the ROM's 6502 vectors all land inside ROM, and its
bytes occupy exactly the four 2K blocks the 1981 manual names), with the image
taken through the emulator's own loader so both models fetch the same bytes -- and a PARC veteran reading one of them found a class of bug worth
knowing about: EclDict names a gate's COMMON input once, on the first gate,
so four cells had wired it into that gate only. `make -C verilog cell-check`
now compares every cell against the dictionary's own per-gate input lists --
and, for the clocked parts, against its `[FF ...]` clock and set/reset lists --
finding NINE bugs across 430 packages, the largest being MC10231's common
clock (152 packages), which took the running machine from 27 moving signals
to 34; **the RTL ALU and the C emulator agree on 10,752 vectors**
(`make -C verilog alu-diff`) -- four MC10181 slices chained as ProcH and ProcL
build them, against `cpu.c`'s own `alu_op()`, neither derived from the other,
and the 6-bit ALUFM entry turns out to be exactly the part's controls
`{Cn, S3..S0, M}`; the 6502 and
6532 are real cores; **all 26 PROMs are generated from PARC's own BCPL, and
the 29 packages that hold them are wired into the RTL and read back correctly
(`make -C verilog prom-test`)**
(`<DoradoSource>DoradoProms.dm!14_`, mirrored into `chm/doradoproms/`), each
with a property check; and a Verilator + Dear ImGui harness builds and runs
with a `--headless` CI mode. The **backplane needs no schematic, and no
inference either**: every board directory states its backplane interface
TWICE -- `<Board>.bp` with bare pins, `-C.nl`/`-E.nl` slot-qualified -- and
across all sixteen boards the two agree exactly (2,054 pins, zero
discrepancy). It is not straight-through: the BaseBoard drives each board's
clock from a different pin while every receiver takes it on C9, and 182 pin
positions carry different nets on different boards, so **the name is the
connection**. 115 shared nets are driven by several boards at once -- that is
ECL wired-OR, the B bus among them. `tools/sil_backplane.py` measures all of
this. **The port lists are fixed (2026-08-16):** every board now emits exactly
the ports PARC states (1,920 of 1,922, 0 spurious) instead of an inference
that missed 703 backplane nets and invented 833 -- it keyed on `Term100`
packages, which are 100-ohm TERMINATORS, not connectors. 512 ports are
`inout`, the nets a board both drives and senses. **The top module is
generated too** (`make -C verilog backplane`): eleven boards wired by name,
501 internal nets of which 83 are `wor` ECL open-emitter buses, 407 ports out
to cables, lint clean, and `--boards` takes any subset so the machine can be
brought up a board at a time. One finding fell out of it: **DispM plugs into
DispY rather than replacing it** -- 42 nets are shared by the two display
boards and no other -- so a colour machine has both.

**The machine is assembled, clocked, and shaped for an FPGA (2026-08-16).**
`sim.v` instantiates it and the BaseBoard's clock distribution runs end to
end: all ten `CLK.<board>'` nets toggle. Two transformations keep it
synthesisable, both in the generator rather than hand-applied. **Wired-OR
buses became OR trees**: each board exports its contribution as `<net>__drv`
and reads the resolved bus back, so there is no `inout` and no
multiply-driven net anywhere (checked with MULTIDRIVEN un-waived: zero).
**Distributed clocks became clock enables**: the Dorado clocks 1,201 packages
from an ECL clock net, which would be 1,201 gated clocks on an FPGA, so every
clocked cell runs on a fabric `sys_clk` and uses that net as an enable; the
two DRAM cells went synchronous and now infer block RAM. Gate:
`make -C verilog machine-test`, a floor of 30 moving signals that should rise
as the cell library fills in. **The machine makes its own clock**: it is
GENERATED on the BaseBoard, not fed in -- an analog VCO (MPQ3303 transistor
quad), shaped by two MC1660s into anti-phase clocks, divided by four MC1690s
into `StartClockPulse'`/`EndClockPulse`, then fanned out to every slot. Only
the VCO is substituted, for a fabric-clock divider, because an analog
oscillator has no digital model and an FPGA has no VCO either; everything
after it is the board's own logic. `MB7071H` is modelled too -- the 256x4 RAM
that IS the register file: ProcH h06 is RM (`RbAdr`/`SelectRm'`), i06 is STK
(`StkAdr`/`SelectStk'`), four packages per board for 16 bits. **The OR/NOR
polarity question is settled from Motorola's own data sheets**: EclDict's role
`OUT` is the INVERTING (NOR) output and `o` the non-inverting (OR) one --
MC10101 labels its four `OUT` pins A-bar-OUT..D-bar-OUT, MC10212 labels pins
3,4,12,13 with bars and 2,14 without. Eight gates, two parts, unanimous;
single-sense parts (MC10110 OR vs MC10111 NOR, identical entries) take
polarity from the part name instead. That unblocked the whole OR/NOR family
and fixed two real bugs -- `cell_MC10105` had its senses reversed (31
packages in the machine) and `cell_MC10103` gave one gate's two outputs the
same expression. It also showed why `machine-test` must NOT gate on how many
signals toggle: correct logic holds nets steady, so the count fell 31 -> 27
as cells got MORE right. It gates on the clock reaching every slot instead.

**A FOUR-BOARD DORADO RUNS MICROINSTRUCTION CYCLES, AND COMPUTES
(2026-08-18/19).** The write path into the machine is proven end to end against
the C emulator, and the datapath now carries a value through Q and into ALUFM;
one polarity is left. Rung by rung, each line a gate you can run:

| rung | gate |
|---|---|
| the BaseBoard's 6502 boots from its own EPROMs | `baseboard-test` |
| it drives the control-processor bus, matching the C emulator | `cpreg-diff` |
| all 36 microinstruction bits decode, matching the C emulator | `mir-diff` |
| a microinstruction lands in the MIR, matching the C emulator | `mirreg-diff` |
| the Control section executes cycles | `run-test` |
| four boards, the microinstruction on the datapath | `datapath-test` |
| a jammed Write-IM deposits into IM, half-select and all | `writeim-test` |
| ...with the DATA from CPReg | `operand-test` |
| ...with the ADDRESS from CPReg | `operand-test` (Link -> TNIA -> IM) |
| the machine SINGLE-STEPS microinstructions | `step-test` |
| PARC's SendViaMIR loads words into IM | `sendmir` -- Boot0's inner loop |
| PARC's BLOCK LOADER walks REAL MICROCODE into IM | `boot0-test` -- and IM MATCHES THE C EMULATOR |
| **THE MACHINE EXECUTES MICROCODE OUT OF IM** | `exec-test` -- free-running and sequencing |
| **THE MACHINE COMPUTES** -- 25 octal from CPReg into Q, held, stored into ALUFM[0] | `compute-test` -- PARC's own ALU prologue |
| ...and T loads through the ALU, EXACTLY | `compute-test` -- 1234 gives 1234, a55a gives a55a |
| **...and it COMPUTES ON TWO OPERANDS** -- A from T, B from CPReg | `compute-test` -- all 24 entries of HM Table 9 match the C emulator |
| RM, the per-task register file, writes and reads back | `compute-test` -- and each value lands where the address pins say |
| **the REAL firmware clocks the MANIFOLD CHAIN on the backplane** | `firmware-probe` -- 24 Clock strobes, DMuxClk reaching every board |
| the MEMORY section is in a machine and clocked | `mem-test` -- seven boards; each memory board's local clock follows its MemClkEnable' |

| **the TASK PRIORITY ENCODER agrees with the C emulator** | `task-test` -- 23 request patterns, highest-numbered task wins |
| ...and the BNT REGISTER loads in a RUNNING machine | `taskrun-test` -- all 15 tasks, fallback to the emulator, fault task wins |
| **THE MACHINE SWITCHES TASKS** -- CTask becomes the task that asked | `taskrun-test` -- all 15, and with TaskingOff it does not |
| ...and each task KEEPS ITS OWN PC and LINK | `taskrun-test` -- TPC[15] survives task 7; the startup Link lands in slot 0 alone |
| **the MEMORY SECTION's front door** -- ASEL 0-3 is a storage reference | `refdecode-test` -- 16 cases against the C emulator's rule |
| **the memory boards RUN, and the microcode ASKS THEM for storage** | `memrun-test` -- seven boards, MemC clocked in step, ASEL=0 with `WantProcRef'` asserted |
| **THE MEMORY SECTION RUNS DRAM CYCLES** (re-earned 2026-08-23) | `memrun-test` -- the earlier green was an artifact of a mis-shifted Map-Mem PROM; `preStartMem'` now PULSES |
| **A WORD COMES OUT OF PARC'S STORAGE ARRAY** | `readback-test` -- eight boards, real microcode, the seeded word back through the SN74166s onto `Sin` |
| **...AND THE RETURN PATH CARRIES A WORD TO THE PROCESSOR** | `readback-test` -- cache -> `dMD` -> `Md`, the register microcode reads and `cpu.c` models |
| **THE PROCESSOR WRITES A WORD TO THE DISPLAY BOARD** | `display-test +slowio` -- nine boards, running microcode: `TIOA<-` puts 370B on the bus, `Output<-` strobes `IOBout` 960 times with `alub`=5a5a, and IOB carries it at the board on exactly 960 samples |
| **...AND ADDRESSES A DISK REGISTER** | `disk-test` -- nine boards with DskEth; f07 decodes `DISKCONTROL` alone (Cont 1921, Muff/Data/Ram/Tag 0) |
| **...WITH THE ADDRESS AND THE DATA TOGETHER** | `disk-test`/`display-test` -- the per-task TIOA holds across the `Output<-` that uses it |
| **THE DISK CONTROLLER OBEYS A COMMAND** | `disk-test` -- `ControlRegCl` clocks, and DebugMode/BlockTillIndex/EnableRun take the bits `include/disk.h` assigns them |
| **...AND THE DECODE DISCRIMINATES** | `disk-tag-test` -- the same loop aimed at DISKTAG (014B): Tag 127 / Cont 0, `TagClock` fires, `ControlRegCl` does not |
| **...AND THE BOARD DRIVES IOB BACK** | `disk-input-test` -- a `Pd<-Input` (FF `0o032`) in the loop: `bIOin'` 960, the MC10174s' enable asserts on 32, and on ZERO without it |
| **...AND THE READ MUX MATCHES BOTH C MODELS** | `disk-read-check` -- muffler on `IOB.15` alone (`disk.c`'s `0x0001`, HM pp.101-102) and `Host.0-7` on the HIGH byte (`ethernet.c`'s `local_host << 8`) |
| **...AND THE DRIVE INTERFACE IS ENUMERATED** | `disk-interface-check` -- 46 signals: 4 drives x (Clock pair IN, Data pair BIDIRECTIONAL, SecIndx', Selected'), 9 status in, 13 tag out |
| **...AND NO DISK IS FABRICATED WHEN NONE IS ATTACHED** | `disk-input-test` -- all 17 active-low cable lines idle DEASSERTED (six of them once read asserted, inventing a drive) |
| **A TRIDENT PRESENTS ITSELF WITH FOUR CABLE LINES** | `disk-input-test` -- `TtlReady'`+`TtlOnLine'`+`TtlTerm'`+`Selected0'` give `DrSelected`=1 and every drive-side fault clear |
| **...AND ITS SECTOR PULSE REACHES THE CONTROLLER** | `disk-input-test` -- `Sector` high on 8 of 8 pulses and 0 of 8 GAPS (the gap half is what separates tracking from stuck) |
| **...AND THE DISKMUFF CLEAR BITS SIT WHERE `disk.c` PUTS THEM** | `disk-read-check` -- d19 takes `bIOB.04-07` as ClearIndexTW/ClearSectorTW/ClearTWs/ClearErrors = 0x0800/0400/0200/0100 |
| **...AND A DISKMUFF WRITE CLEARS A TASK WAKEUP** | `disk-muff-test` -- `ClearIndexTW` on the 32 coincidence samples, and `IndexTW` DROPS from 140559/140559 |
| **...AND THE DRIVE'S SERIAL DATA REACHES THE CONTROLLER** | `disk-input-test` -- `cell_MC1650` modelled from its data sheet; 8 alternating bits give `PreReadData` 4 high / 4 low, and 0 of 8 when DESELECTED |
| **...AND ITS BIT CLOCK REACHES THE SHIFT REGISTER** | `disk-input-test` -- 32 cable bit periods = 32 `BitClock'B` edges, chain in SHIFT mode; the DATA is gated twice by the b20 sequencer (open) |
| **...AND THE FOUR PER-BLOCK OPS ARE SPLIT BY BIT** | `disk-read-check` -- f14 takes `bIOB.08/10/12/14`, f15 `.09/11/13/15`, both `PE'=Active` so they load while idle and shift one op per block |
| **A REAL DISK COMMAND LOADS, AND KEEPS LOADING** | `disk-cmd-test` -- `0x02C0` = SetDebugMode + Read op; the address rides an FF LITERAL (BSEL 6 = `FF,,0`) so T is written from IM every pass: 28 control-register writes, `Active` on 3,765 samples |
| **PARC'S OWN FORMAT PROGRAM IS IN PARC'S OWN FORMAT RAM** | `disk-format-test` -- all sixteen words of HM p.98's Alto Diablo emulation format, jammed one per run and read back exactly |
| **THE FORMAT RAM WALKS ALL SIXTEEN WORDS** | `disk-ram-test` -- `LastRamAddr'` asserts (0 -> 214) once the address comes from Q; its carry resets the DisableRun flip-flop, i.e. sets EnableRun, `disk.c` verbatim. T is PER-TASK and a jam does not write the file, so T reverts after one pass |
| **the FORMAT RAM's contents are SPECIFIED** (HM p.98) -- word counts at 00-03, CONTROL TAG COMMANDS at 04-07, drive timing at 08-14; `disk.c` reads the first four and the RTL's `Ram.04-07` the next four, so both models were right | mapped |
| **...AND A WORD COMES OFF THE DISK TO THE PROCESSOR** | `disk-format-test` -- 4 words into the FIFO, 6 popped, `FifoEmpty`=1, `DskData`=0aa0. `DskData` loads AUTOMATICALLY (b11's `OutRegWrite'` from `OutRegFull`+`FifoEmpty`), so the microcode must DRAIN |
| **THE DISK READ RUNS** | `disk-format-test` -- format program loaded, Read command Active, tag write, and 256 bit periods on the cable: the PROM program counter walks 0-5 through "read data for first block", `ShiftIn`/`ComputeECC` open, and the shift register fills with the fed pattern. Step durations match HM p.99 exactly (1 + `RAM[13]`+1 = 3 WordClocks) |
| **A DISKTAG WRITE RELEASES THE BIT COUNTER, AND WORDCLOCKS RUN** | `disk-format-test` -- `sCountBits` IS the tag strobe (b17's sibling output is `Tag_IOB`): b10's `PE'` 0->1, `CO'` 0->2, `WordClock'` 1->3 edges |
| **the READ SEQUENCER sits at step 0 doing what HM p.99 says** -- a21 enabled, a20 disabled, and the PROM selects format `RAM[6]`, "issue tag command in RAM[6] (head select)" | mapped |
| **...and the read path is STARTED BY THE FORMAT RAM** | mapped, not gated -- `sCountBits = (ShiftReg.08 \| Tag.000) & ShiftReg.15`, and `Tag.000` comes from `Ram.04-07` while running. A DISKMUFF write can also drive `PreReadData`/`PrePreBitClock` directly (PARC's diagnostic path) |
| ...and TWO REFERENCE KINDS match the C emulator's table | `memrun-test` -- `LFetch<-` and `IFetch<-`, each in its own cell of sixteen |
| **A WHOLE WORLD PRELOADS INTO IM** | `exec-world` -- 2,148 microinstructions of AEmu.mb, every one read back |
| ...through a field map DERIVED from the wire lists | `boot0-test` -- 65 addresses, all 8 fields, against a CP-bus load |
| **ALUFM and IFUM too** | `exec-world9` -- 16 + 256 entries; they confirm HM Table 11d and cpu.c's Table 20 |
| **NINE BOARDS DISPATCH AN ALTO OPCODE** | `exec-world9` -- START, the IFU traps, RESTARTIFU, then AND1 and SKPC |
| **IM PARITY, the long-open question, ANSWERED** | `im-parity-check` -- ODD over the 17-bit half, and the array stores its COMPLEMENT |
| **THE MACHINE RUNS WITH IM PARITY ENABLED** | `exec-parity` -- Error propagated on 0 of 400,000 samples, Stop never set |
| **THE FAULT TASK SERVICES A STACK UNDERFLOW** | `exec-tasking` -- RepeatCur 0, task 15 runs, 25 addresses, longest run 3 |

Forty-eight gates in all; `make -C verilog` has the list. **The datapath is
done**; parity is the one open item in the boot chain. Cell coverage is
**97.7%** of the eleven-board machine, and of the 64 packages left 42 are
analog. Four machine configurations are generated (`dorado_backplane` at eleven
boards, plus BaseBd alone, ContA+ContB, and ContA/ContB/ProcH/ProcL).

**Findings worth not rediscovering**, each written up in the handoff:

- **The BaseBoard's EPROMs are stored BIT-REVERSED** -- the board wires the
  2716's pin 9 (O0) to the 6502's DB7. `firmware/B-08.BIN` read bit-reversed is
  byte-for-byte `doradobaserom.mb!13`'s 0xF000 block, which retires the old
  note that those 1987 dumps were "a different set".
- **The wire list's per-pin `{x,y}` states the wire-wrap jumpers and the
  resistor platforms geometrically** -- a jumper position is a column of pins at
  one x; `PLAT1816` pairs pin N with pin 17-N, 122 pairs with no exception.
- **The machine would not settle**, and it was six cells modelled as transparent
  latches that are not, plus nothing else -- `F10145A`, `F10415A`, `F10470`,
  `i2125`, `MC10173`, `SN74LS259`, now on `sys_clk` with the part's own level as
  an enable. `loop-check` is the gate.
- **`cell-check` was blind to 18 of 112 parts** because a part may state its
  gates across several `[G]` lines; two of those had Tim's common-pin bug, in 82
  packages. It is blind to a THIRD form of it: a cell can read exactly the right
  pins and still GROUP them wrongly. `cell_MC10119` ANDed its shared pin 10 as a
  separate term instead of putting it in two OR groups -- the part is a 4-3-3-3,
  thirteen input slots across twelve pins -- which forced the output low for
  every FA=0 microinstruction, PARC's own Nop included, and reloaded Q from a
  dead bus every cycle. `cell_MC10141` meanwhile loaded its parallel entry
  ROTATED one place, turning 25 octal into 008a, in 60 packages.
- **PARC's dictionary and Motorola disagree on the MC10141's pin NAMES, and
  neither is wrong** -- EclDict swaps DL/DR and calls S1/S2 SL'/SR', two swaps
  consistent with each other, because it is only which end of a 16-bit register
  you call "left". The board reads correctly under PARC's names; a CELL must
  implement the datasheet's function per PIN NUMBER.
- **The nops in PARC's boot sequences are not padding.** Control signals come
  out of registers clocked by `Clock1'` while the datapath registers clock off
  the EARLIER `PreClock1'`, so the controls in force at any load edge are the
  ones the PREVIOUS instruction latched. Q is not loaded by `QFromCPReg#`; it is
  loaded by the Nop after it -- and a probe sampling right after an instruction
  reads one cycle early.
- **SIX BACKPLANE LINES ARE SPELLED TWO WAYS, and one is the memory hold.**
  PARC capitalised inconsistently and this backplane is wired by NAME, so a
  spelling difference leaves a line unconnected. `PrHold` (MemC) and `PRhold`
  (ProcH/ProcL) are ONE WIRE -- #07-E.42, #s05-E.42, #s04-E.42 -- so the memory
  section could not hold the processor at all. CASE-INSENSITIVE MATCHING WOULD
  BE WRONG: three case-variant backplane groups sit on DIFFERENT pins, and
  outside the backplane 63 names differ only by case, mostly per-board LOCAL
  clock fan-out. The rule is narrow -- merge only where every board agrees on
  the pin -- and lives as a six-entry table, `BACKPLANE_CASE_ALIASES`.
- **A SIP'S LEGS CAN BE CUT, and that is where a board's IDENTITY lives.**
  The wire list still lists the pin; it just has no resistor behind it, so the
  cut is invisible in the netlist and sits on the board's configuration sheet
  instead. What the cuts set is not decoration -- it is each board's slow-I/O
  ADDRESS and TASK NUMBER. They are modelled and gated now
  (`make -C verilog strap-test`), and two sheets state the ANSWER, so the
  straps check themselves:

  | board | strap | reads | schematic says |
  |---|---|---|---|
  | DispM | g41 leg 6 cut | `DDMTIOA` = 36B -> **0360-0367** | "making DDMTIOA = 360B" |
  | DispM | b52 legs 3,4 cut | `AltoWTask` = 1001 = **9** | "for Task 9D = 11B" |
  | DskEth | e41 legs 4-7 cut | `TIOA-Ad` = 1 -> **010-017** | "* Standard addresses are 10-17" |
  | DispY | g42 no legs cut | `DDCTIOA` = 37B -> 0370-0377 | (no stated result) |

  **And the C emulator agrees, independently**: `include/dispm.h` has
  `DISPM_TIOA_BOARD 0360`, `include/disk.h` has `DISK_TIOA_DISKCONTROL 010`
  ("Disk uses task 14 octal exclusively, on TIOA 10-14 octal"). Neither model
  was derived from the other. The encoding falls out of the DskEth table,
  which lists all 32 ranges: a cut leg is 0, an intact one is 1, MSB first,
  and the 5-bit field is the top of an 8-bit address whose low three bits
  select the register.

- **A WHOLE MICROCODE WORLD LOADS INTO THE RTL, and the three memories were
  each derived from the netlist rather than assumed.** IM is ContB's 144
  F10415A as 4096x36 with `bank = {addr[11], addr[0]}` -- and knowing WHICH
  BITS pick a bank is not knowing which VALUE picks which bank, so the bank
  ordering is EVALUATED from the two MC10101s' gate polarities. ALUFM is ProcL
  e13/e14, recognisable because their data pins come off the B bus, and it
  confirms HM Table 11d: `ALUFdec.0..5` = `B.08, B.11..B.15`, the top bit being
  the ALU's CARRY IN and NOT part of a contiguous field. IFUM is the IFU's 27
  F10415A, addressed `{InstrSet, opcode}`, whose two write enables `DecHi'` and
  `DecLo'` ARE the .MB's two words per entry -- and whose bit positions match
  `cpu.c`'s HM Table 20 layout exactly, with the names crossing over (`DecLo'`
  holds what the C emulator calls `ifum_hi`). Gate: `boot0-test` checks the map
  against a control-processor-bus load of the same microcode, 65 addresses and
  all eight fields, and three mutations of the generator are caught.
  **The nine-board machine then dispatches a real Alto opcode** -- `AND1`, and
  `SKPC` once IFUM is loaded.
- **THE IFU STOPPED FETCHING BECAUSE OF PARITY, TWICE -- and clearing the first
  is the FIRST PARITY AGREEMENT between the two models.** The `.MB` does NOT
  carry IFUM parity: the real machine computes the three IPar bits in its LOAD
  microcode (`ifuRamSubrs.mc:ifuAddParity`), and 248 of AEmu's 256 entries fail
  `cpu.c`'s own `ifum_parity_ok` as stored -- which is why the C emulator only
  checks it behind `DORADO_IFUM_PARITY_TRAP`. Copying them verbatim left the
  IFU in a permanent RAM parity error, `RamPe` high on 200,000 of 200,000
  samples; computing them takes it to ZERO. The RTL's generators come from the
  wire lists and `cpu.c`'s grouping from the microcode and the manual, neither
  derived from the other, so **IM parity -- where the machine still runs only
  with the enables cleared -- is the same question with a tractable second
  instance now solved.** The second error was FG parity over the instruction
The second error was FG parity over the instruction
  BYTES, and it is only PARTLY settled. Odd parity per byte -- D.17 over
  D.08-15, D.16 over D.00-07 -- minimises the error identically across six
  constant patterns, and with it the machine reaches `CVEND`, whose ASEL is
  `Fetch<-RM/STK`. But it is NOT established as the hardware convention:
  giving each cache line its own word makes the winning assignment depend on
  the DATA FUNCTION, which no real convention can do. **The counter is not
  measuring what it looks like** -- `IfuMemRef` makes two to six transitions
  over a whole run, so there are essentially no fetches, and a count of
  samples where a combinational signal sits high is an IDLE LEVEL rather than
  a per-fetch check. Contrast the IFUM parity above, where `RamPe` went to
  EXACTLY ZERO and entries are read continuously. **A level is not an event:
  before believing a sample count, check that the thing it counts actually
  happened.**
- **IM PARITY IS ANSWERED: ODD over the 17-bit half, stored COMPLEMENTED.**
  The machine used to run only with the parity enables cleared, and the
  question was whether PARC's IRTable entries satisfy our generator or our
  MC10170s compute something different. It is the second, by exactly one
  inversion. Three steps, none assumed. **Which bits each half covers was
  FITTED**, not guessed: `im_image` emits both the decoded fields and the two
  17-bit half-words, and fitting one against the other across 2,148 AEmu
  addresses gives a UNIQUE match for all 34 bits -- left half = `RSTK.1-3,
  ALUF, BSEL, LC, ASEL` + `RSTK.0` as secondary, right half = `FF, JCN` +
  `BLOCK`. **The sense the array wants was MEASURED** on the running machine:
  preloading even parity takes `IMLHPE` from 200,000 samples to 125, odd gives
  200,000, and dropping the secondary gives 199,680. **The sense PARC uses was
  READ OFF PARC's own hand-coding**: the IRTable's five-byte format carries
  explicit `P015`/`P1631` bits, and all eight entries carry ODD parity over
  those same 17 bits -- 8 of 8, 16 of 16 bits. The two are complements, so the
  array stores the complement of the parity bit, **exactly as `dBlock'` does**
  (tb_boot0 measured all 64 right-half secondary bits inverted). Two
  independent instances of one storage convention. Gate: `im-parity-check`.
  Consequence still open: `tb_compute.sv`'s `mi()` sets both parity bits to 1
  unconditionally, but PARC's own entries vary them, so computing them is what
  would let a JAM pass parity and the machine run with the enables on.
- **AND THE MACHINE NOW RUNS WITH IM PARITY ENABLED** -- the rung this project
  could never reach; `exec-test` has always had to clear the enables. Gate:
  `exec-parity`. What was in the way was not the parity VALUES at all. Bucketing
  the errors by TIME instead of totalling them shows they run from cycle 0 to
  **148 and then clear by themselves, never returning across 400,000 cycles**:
  the preloaded array's parity is correct and the machine's own power-up
  transient is what fails. With the enables on from the start, `Stop` latches at
  cycle **125** -- inside that window -- and **`Stop` gates the clock that would
  clear it**, so it is permanent. Switching the enables on at cycle 400 instead
  gives `Error` propagated on **0** samples, `Stop` never set, 24,991 clk0'
  edges. A TOTAL SAID "parity is broken"; A DISTRIBUTION SAID "the enables are
  on 148 cycles too early".
- **THE WORLD'S LOOP IS A HOLD, AND THE STACK POINTER IS AT ZERO.** Measured
  end to end, each link a counter over 400,000 samples: `StkP` = 0x00 on
  399,435 -> ProcL asserts `PrHoldReq` on **exactly 399,435** -> MemC latches
  `MiscHold` on 399,419, sixteen samples (one microinstruction) later -> `Hold`
  goes onto the backplane and is fanned out as PRhold/CBHold/IfuHold/IOHold/
  MXHold, all the same count -> ContA f20 (`CAHold' = ~Hold`, `RepeatCur =
  ~(CAHold' | SwitchUp)`) asserts `RepeatCur` -> the MIR holds and TNIA freezes
  at 0x040 for 12,481 microinstructions. **`StkP = 0` is stack UNDERFLOW**:
  ProcL j20 drives `PrHoldReq` from `StkP.6/7` and the RSTK field, which is the
  Dorado's stack overflow/underflow hold. **It sustains itself** -- MemC c24 ORs
  `MXHold` (which comes from `Hold`) into its own request, and the reset
  `DisHold` is a MODE BIT in a control register, not a per-cycle clear. `MDhold`
  and `RefHold` are 0, so the memory is not waiting for data or refresh; the
  processor is holding itself. **And StkP=0 is CORRECT** -- `cpu.c`'s own
  transcription of HM Table 6 says `StkP[2:7] = 0 denotes empty stack`, that
  `RSTK[0]=1` enables the underflow check ("underflow if StkP originally 0 OR
  finally 0"), and that underflow "would HOLD + wake fault task 15 on real
  hardware -- **we just track the flags**". The instruction at the hold carries
  RSTK = 8, i.e. `RSTK[0] = 1`. So the RTL is doing what the manual says and
  **the C emulator is the one that does not model it**. The hold is permanent
  only because this bench starts with `Return#` = `TaskingOff,Return`, so fault
  task 15 -- the thing that would service the underflow -- can never run; CTask
  is task 0 for all 400,000 samples. **And with tasking ON it does.** `Return#`'s
  FF is `0o142` = FA 1, FB 4, FC 2 = TaskingOff; FC 3 is TaskingOn, and
  re-encoding it as FF `0o143` gives `70 13 E1 4A 43`. Gate: `exec-tasking`.
  `RepeatCur` 399,419 -> **0**; CTask t0=400,000 -> **t15=399,819**; distinct
  addresses 15 -> **25**; longest run on one address 12,481 -> **3**. The fault
  task runs, the hold clears, and the machine sequences through real Alto opcode
  handlers -- `LDAIZ`, `JMPI2`, `DOCRYS`, `SKP1C`, `TRAP17`, `JSRIIX`, the last
  with ASEL = `Fetch<-T`. **The manual's mechanism working end to end in
  generated RTL**: a stack operation on an empty stack holds the processor,
  wakes the fault task, and the fault task clears it.
- **BUT THE FAULT TASK IS REQUESTED FROM RESET.** `TWReq.15` reads high on
  400,000 of 400,000 samples, so with tasking on the machine switches to task 15
  at cycle 181 and never returns: **task 0 executes TWO instructions** and task
  15 runs 23 -- and those 23 are ALTO EMULATOR handlers, not a fault handler,
  so `TPC[15]` was never initialised and the fault task is running whatever it
  landed on. Two candidates tested and BOTH ELIMINATED: seeding the Map with
  `tb_readback`'s own 21-array MemX seeding changes nothing (`+nomapseed` is the
  control), and sweeping AEmu's five named entry points -- `START` 0, `STARTMB`
  53, `AEMUNEXT` 51, `BOOT` 652, `STARTEMULATOR` 656 octal -- shows **0 is the
  best of them**, every other landing back in the 0x0c4 spin. So the blocker is
  narrow: something asserts the fault-task wakeup out of reset and nothing
  clears it.
- **SAMPLING A LEVEL AT TEN POINTS IS NOT MEASURING IT.** The hold chain was
  previously read as "all eight signals 0, whole run" from ten instantaneous
  `$display`s -- true of the machine BEFORE IFUM was loaded and the parity
  fixed, and stale by the time it was being quoted. It is 99.85% HIGH. Count
  levels, do not sample them, and re-take any level-based conclusion whenever
  the machine changes underneath it.
- **BYTE 0 OF A JAM IS FOUR BITS, and they ride as the NINTH CP-BUS BIT** of
  the four data strobes -- `b0[7]` RSTK.0 on fn 4, `b0[6]` P015 on fn 5,
  `b0[5]` JCN.7 on fn 6, `b0[4]` P1631 on fn 7 -- which is why byte 0's low four
  bits are always zero. Sweeping the ninth bit against each function code
  (`+jamsweep`) confirms it: fn 5 sets `IMLH`, fn 7 sets `IMRH`, under every
  variant tried. **This retracts a claim of mine** that "the jam never delivers
  a parity bit": capturing `IMLH` inside `parc_micro` reads 0 even for `Nop#`
  (P015=1), but the routing is sound, so the question is where to SAMPLE, not a
  missing connection. Both checkers reduce to
  `IM?HPE' = ~(XOR(17 field bits) ^ IM?H)`, so a zero parity bit passes only
  when the field-XOR is zero -- `Nop#` alone of PARC's eight, and not `Return#`,
  which is what the startup jams.
- **A GREEN CHECK IS WHAT PEOPLE QUOTE, so make it say what it cannot test.**
  Mutating `im-parity-check` four ways, two do NOT fail, both for principled
  reasons: every one of PARC's eight entries has `RSTK.0 = 0` and `BLOCK = 0`,
  so the dataset cannot separate 16-bit from 17-bit parity (the machine
  measurement does); and reversing the bit order within a field is a
  PERMUTATION, which parity is invariant under -- no parity check can ever
  detect it. The tool prints both limits beside its PASS.
- **Five ways a bench can lie to you, all found in one sitting.** `mbdis`
  prints IM addresses in OCTAL, so a hex visited-set must be converted before
  it is looked up. A bank SEARCH matches a wiped bank whenever the wanted value
  is `0000`, which `IM[0x805]` in AEmu.mb is. A vector set confined to low
  addresses leaves `addr[11]` always 0 and exercises half an interleave.
  `$sscanf` returns -1 on a PARTIAL match while still filling the fields it did
  convert, so gating on the count silently skips every short line. And `$fgets`
  returns 0 at EOF leaving its buffer alone, so `while (!$feof(fd))` processes
  the last line twice.
- **TAKE THE SHEET THE WIRE LIST NAMES -- the PDFs are other revisions.** Each
  `.wl` header lists every constituent `.sil` file with its own Rev and Date.
  `DoradoDocs/schematics/DispY.pdf` is `DispY31.sil Rev Ci 11/02/79`; the wire
  list says `Rev Cl 3/25/82`. Two and a half years apart, and **g41's cut list
  changed between them** (3,4,5 -> 4,5), so a first pass took the wrong table
  and dropped a pull that is really there. The per-revision scans in
  `DoradoDocs/doradodrawings/` carry the built sheet --
  `DispY-Rev-Cl.press!1.pdf` matched the wire list exactly. That sheet also
  revises the oscillator from 20 MHz to **50 MHz** and says to LEAVE a02 BLANK
  (no MC10318).

- **A resistor leg has no direction, and Sil's letter on one is not evidence.**
  `sip_drives` required the wire list to call a pack pin `out`, and pin 4 of an
  8-pin SIP is routinely marked `in` -- so 18 nets across five boards had NO
  driver at all: six strap bits (including `DDMTIOA.02`, without which the
  address reads 320B and the schematic's own arithmetic fails), the IFU's
  reference net `TTLHigh` sitting at 0, and six ACTIVE-LOW DskEth drive-status
  lines (`Selected0'`, `TtlReadOnly'`, `TtlEndOfCyl'`, ...) reading ASSERTED,
  which fabricates a disk that is not attached. A leg is a source of last
  resort now: where something else drives the net it stays off, because these
  resolve through an OR tree and a pull-up would nail the net high forever.

- **A pull-up and a pull-down on the same net are not always a divider.** That
  reading is right for DskEth's `RcvData` (a real bias network at the Ethernet
  receiver's input) and wrong for a CONFIGURATION STRAP, where the pull-down
  pack gives every line its default 0 and the pull-up pack has legs only where
  a 1 is wanted. The BaseBoard's `Midas.00-04` are the second kind, and they
  set **the BaseBoard's own muffler number, 9** -- k20 compares the muffler
  address `DMD.04..01` against the strap and drives `BaseMuf'`, the enable on
  the LS151 that sources `CPDMuxData`. Left open, `BaseMuf'` never asserts and
  the BaseBoard cannot answer a muffler read at all. Gate:
  `make -C verilog muffler-test`, which sweeps all sixteen addresses and
  requires exactly one to select the board.

- **A MEMORY REFERENCE ENTERS THROUGH ASEL, and the gates say so.** MemC takes
  `ASEL.0-2` straight off the backplane, plus `FF.0mem'`/`FF.1mem`, and b24 (an
  MC10103 quad OR) makes `WantProcRef' = IgnoreProc | ASEL.0`. PARC numbers a
  field MSB first, so ASEL.0 is 0 exactly when ASEL <= 3 -- which is the C
  emulator's rule verbatim (`include/memory.h`: "when ASEL is a memory
  reference (ASEL = 0..3 with FF[0:1] decoding the kind)"), from an
  independent derivation. The KIND decoder is a24, an MC10162 one-of-eight
  addressed by `{ASEL.1, ASEL.2, FF.1mem}`. Gate: `refdecode-test`. Pinning
  the full (ASEL, FF[0:1]) -> kind table against `cpu.c`'s dispatch needs the
  board's other inputs driven (`Dbusy`, `CacheRefInA'`, `WantCR`) -- and those
  are INTERNAL to MemC, so they cannot be forced from ports. **They come from a
  running machine**, which `memrun-test` now provides: `dorado_mem`, seven
  boards, tb_exec's startup, MemC's clock running 187 edges against the
  processor's 181. With microcode executing, all four settle to defined levels
  and THE MICROCODE ASKS FOR STORAGE -- the four AEmu hunks present ASEL=0,
  `WantProcRef'` asserts, and MemC responds with `Dbusy` and `WantCR` set. So
  the front-door rule now holds against an ASEL the machine chose for itself.
  **And two of the reference KINDS match `cpu.c` exactly**: sweeping ASEL 0-3
  against the two FF bits with the qualifiers live gives `ASEL=0 ff01=2 ->
  LFetch<-` and `ASEL=1 ff01=2 -> IFetch<-`, each asserting there and nowhere
  else in the sixteen, against `cpu.c`'s `DM_REF_LONGFETCH` and
  `DM_REF_IFETCH`. (`ff01` is `FF.0*2 + FF.1`, MSB first; the `_` suffix is
  Sil's assignment arrow.) The rest of the table is NOT gated -- `Store<-`
  comes off an MC10105 OR rather than the decoder, and the IO kinds are
  qualified by whether the current task is an I/O task, which `cpu.c` also
  conditions on (`io_task ? DM_REF_IOFETCH : DM_REF_MAP`). Next: MAR and an
  actual access.

- **`dorado_mem` has THREE MORE CLOCK PORTS than `dorado_proc`** -- `CLK_mc'`,
  `CLK_md'`, `CLK_mx'`, because the BaseBoard fans the clock to every slot.
  Leaving them undriven gives MemC zero local clock edges with BOTH its enables
  already asserted, which looks exactly like a gating bug and is not one.

- **`_Map` cross-checks between the two models.** ContA b17 decodes it as
  FA=0, FB=3, FC=1 = 0o31, and `cpu.c`'s comment on `DM_REF_RMAP` says the read
  form "carries the ReadMap function (FA forced 0, FB=3, FC=1 -- the FF[2:7]
  subfield decodes to 0o31)". No shared code, same number.

- **THE TASK WAKEUPS ARE ROUTED BY A BACKPLANE JUMPER, and none of them was
  connected.** Every I/O board puts its wakeup on the SAME two pins, C120 and
  C121, under its own local name -- DispY `WakeDWT`/`WakeDHT`, DispM
  `WakeAWT`/`WakeAHT`, DskEth `WakeEthRx`/`WakeEthTx` -- while ContA receives
  `TWReq.01`..`TWReq.15` on pins of its own. Nothing matches by name OR by pin,
  and our backplane wires by name, so all fifteen request lines went nowhere.
  `Backplane.pdf` (BPRight04.sil 7/24/80) shows pins 120/121 carrying
  `TWReq.xx*` across the generic I/O slots, annotated **"for desired Task wake
  up"** -- the routing is a JUMPER, so a board's task number is a property of
  its SLOT. Which is exactly why each board carries a task-number STRAP: the
  strap tells the board what the jumper tells the backplane, and the two must
  agree, so **the strap is the authority for which line to wire**. DispM
  `WakeAWT` -> `TWReq.09` (AltoWTask = 9, "Task 9D = 11B"), DispY `WakeDWT` ->
  `TWReq.11` (DWTTask = 1011), MemX `TWReq15` -> `TWReq.15` (the fault task is
  15, HM section 4.1 and `include/memory.h`). `BACKPLANE_WAKEUP_JUMPERS`. The
  head tasks and DskEth's two ethernet lines are NOT wired: nothing yet says
  which line they take.

- **A PIN NUMBER IS NOT A WIRE ON THIS BACKPLANE.** Chasing the above, C132 is
  `TWReq.11` on ContA, `TIOA.3` on ProcH, `IfuData.3` on the IFU and `TWReq15`
  on MemX -- four different signals at one connector position. The rule already
  recorded ("the name is the connection", and merge case-variants only where
  every board agrees on the pin) exists for precisely this; a first pass here
  read MemX's pin as proving it was ContA's `TWReq.11` and was wrong.

- **`BNT` is a REGISTER, so a static bench cannot gate it.** ContA h08/h09 are
  MC10141 shift registers whose parallel load needs `BNextRegsEn'` low on both
  select pins and `clk0'` running -- i.e. the machine EXECUTING. What is
  combinational is `PEnc`/`bPEnc`, the encode of the fifteen request lines, and
  that is what `task-test` gates: 23 patterns, all agreeing with `cpu.c`'s
  `task_bnt()` (highest-numbered requester wins, task 0 always available).
  **BNT is gated now too** (`taskrun-test`): tb_exec's startup is reused --
  four hunks of `AEmu.mb` walked into IM, MIR clock released, a free-running
  `Return#` -- and with the machine fetching from IM the register is shown to
  follow the encoder for all fifteen tasks, to fall back to the emulator when
  the requests are withdrawn, and to give 15 to the fault task against a
  competing 1. It follows WITHOUT tasking being on, because ContA h10 is an
  MC10166 comparing BNT against PEnc: `BNextRegsEn'` asserts exactly when they
  DIFFER, so the register is a load-on-change. `Switcha`/`BNTGtCT'` and CTask
  actually switching still need `FF=TaskingOn`, since `Return#` is
  "TaskingOff,Return" -- that is the next step.

- **A PIN CAN BE BROKEN OFF A LOGIC CHIP TOO.** MemX's "Stuffing and
  Configuration Instructions" (`Memx23.sil`, 10/29/79) says "Break h20.10
  before stuffing" -- h20 is an MC10105 and pin 10 carries `MapPerr`, so the
  memory-parity summary is `STPerr | HitPerr`, not `STPerr | MapPerr |
  HitPerr`. An open MECL input sits at VEE and reads 0 (hence the symbol
  sheet's "ALL UNUSED INPUTS MUST BE TIED TO VEE"). `BROKEN_PACKAGE_PINS`.
  The same sheet's instruction 1 is deliberately NOT applied: "if 256 chips
  are NOT installed in the MSA, break g10.6" is conditional on how much memory
  the machine was built with, and `ChipsAre256/16K` / `ChipsAre64K` are
  backplane INPUTS to MemX, so that configuration arrives from outside the
  board. Same for its blue wire (k4.6/k4.5/k4.4 to k4.16) and the b14/c12
  jumper sockets -- a 16K/64K/256K RAM choice nobody has made yet.

- **EVERY BOARD HAS SUCH A SHEET, and not all are titled the same.** Only two
  are headed "Configuration Information"; DskEth's IOA table is on an ordinary
  reference sheet and the BaseBoard's is headed "Stuffing Information", so
  search by CONTENT (`cut`, `legs`, `jumper`, `SIP`, `oscillator`) across
  `DoradoDocs/schematics/` and `doradodrawings/`. Still unread: MemC's "cut the
  4 107 legs marked X" (an OPTIONAL conversion of parity into VA.4 -- the built
  board is the un-converted one, so do NOT apply it), MemX's configuration
  PLATs, and ProcL's parallel discrete resistors.

- **The F100181 is NOT the 74181/MC10181 function set.** It has its own
  sixteen-entry table -- S3 selects arithmetic from logic, S2 selects BCD from
  binary -- so `cell_MC10181`'s decode must not be reused for it. Eight
  packages, all on MemC, the memory board's address arithmetic.
  Datasheet: `DoradoDocs/datasheets/F100181.pdf`.
- **K1115A is a CRYSTAL OSCILLATOR**, not a logic part, and the four positions
  run at THREE different frequencies -- DispY a05 50 MHz, DispM c05 10 MHz
  (the VCO), DispM d13 20 MHz, DskEth j20 unstated. `CELL_PARAMS` passes each
  one its own value; before that the generator had no per-position parameters
  and all four ran alike. The cell is a PHASE ACCUMULATOR because an integer
  divisor of the 266.667 MHz sys_clk gives 1 : 1.86 : 4.33 where the parts are
  1 : 2 : 5. Gate: `make -C verilog osc-test`. (The 20 MHz on the bitsavers
  DispY scan is the superseded Rev Ci sheet; the built Rev Cl says 50.)
- **`machine-test` IS GREEN: it was `cell_F10016`'s terminal count.** MemD wires
  an F10016's TC through an inverting MC10195 back to that counter's own CE, and
  the cell gated TC with CE -- `TC = ~TC` at terminal count, an oscillator that
  stopped the whole machine converging. The Fairchild data sheet
  (`DoradoDocs/datasheets/F10016.pdf`) is explicit: CE is "Count Enable (LOW to
  Count)", TC is "Terminal Count (10016 LOW at HHHH)" -- a function of the Q
  state ALONE. The eight TC->CE cascades in the machine work correctly that
  way. One line, 226 packages, and the memory boards now clock.
- **How it was found:** `MemClkEnable'a` is a wired-OR of `dMemRun`
  (from `SetRunRfsh`, a line the BASEBOARD drives) and `dStop`, so the memory
  clocks come on when the machine RUNS -- and `mem-test +define+MEM_RUN` then
  fails to converge. `loop-check` passes, so it is a machine-level oscillation,
  not a cell-level loop. A much smaller repro to debug.
- **The memory boards' clocks are GATED by `MemClkEnable'` from ContA**, so a
  dead local clock out of reset is CORRECT. Assert the consistency (clock runs
  iff enabled), not that it runs.
- **A 6532 PORT PIN READS ITS OWN PIN.** `read_excluding` drops a package's own
  contribution -- right for a gate, wrong for a port pin, where the chip reads
  the PIN and an output pin is what the chip itself drives (the core says so:
  "port output must be fed back to input"). Without it every read-modify-write
  on a port read ZERO for its own output bits, so `INC $0582`/`DEC $0582` in
  SetMufflerAddress produced 0x01/0xFF instead of 0x11/0x10 and the CP-bus
  function code never read `Clock`. `READBACK_OWN_PIN` in the generator fixes
  it, and the BaseBoard now clocks the muffler chain onto the backplane.
- **MEASURE THE PC WITH SYNC**, not the address bus: the bus includes DATA
  READS, which made ROM table lookups look like a PC parked in filler.
- **A 6532 port pin's pull-up is a NET property, not a cell one.** The core's
  `PA_out = out_a | ~dir_a` is the pull-up of a high-Z input pin -- a WIRE-AND
  convention -- and these nets resolve as wired-OR, so it won instead of losing
  and pinned 33 nets HIGH, `RCPReg.00-15` (the CP register read back) among
  them. Masking it inside the cell HOLDS THE 6502 IN RESET, because
  `WatchdogOut`'s only driver is the RIOT and its pull-up is real. The fix is
  `WEAK_PORT_DRIVERS` in the generator, symmetric to `OVERRIDE_DRIVERS`: the
  pull-up loses where something else drives, stands where nothing does. With
  it, the real firmware reaches the control-processor bus.
- **The BaseBoard boots itself past power-up.** Over a 260 M-cycle run all 397
  resets fall in the FIRST watchdog window; after the first Q21 edge there are
  none across 176 M sys_clk, including a window that is still ARMED. The
  firmware pacifies (240 `PacifyWatchdog` visits) and the watchdog stays
  satisfied. Two earlier notes claimed the opposite, both from reading a TOTAL
  instead of a DISTRIBUTION -- bucket by time before concluding a run has not
  settled. Still open, and narrower again: `SetMufflerAddress` RUNS -- the strobes come
  from F9FD/FA00/FA08/FA0B inside its F9F6 shift loop -- but the CP-bus
  function code reads 0 and 7 alternating where `MCPBusL = $10` should give a
  constant 1 (`Clock`), so the BaseBoard's k22/k17 never decode a DMux pulse.
  A field reading all-zero/all-one is the signature of pins not carrying the
  output register; check whether the DDR write reaches the RIOT model.
  MEASURE THE PC WITH SYNC: the address bus includes DATA READS, which made
  FF00/FF80 table lookups look like a PC parked in filler ROM.
- **The real firmware runs, and the WATCHDOG is what stops it.** `dorado_boot`
  (BaseBd+ContA+ContB+ProcH+ProcL) lets the 6502 run its own EPROMs; it is
  reset every 211,440 sys_clk, exactly periodic, and never reaches the Dorado.
  The chain is g21 (an MC14521B 24-stage divider -- the timer, and it was an
  unmodelled SKELETON until now) -> g22 (an SN74LS74 wired as a toggle FF) ->
  g23 (an SN7486 XORing `WatchdogIn` against `WatchdogOut`) -> j17 -> `BootMC'`
  -> j08 -> `MCReset'`. Measured: the TIMER is not the cause -- Q21 divides by
  2^21 and is correctly silent -- the resets track `WatchdogOut` one-for-one.
  What arms the watchdog is g22's Q' powering up at 1. And the 6532's
  `PA_out = out_a | ~dir_a` is NOT a bug -- it is the PULL-UP of a high-Z
  input pin, right for `WatchdogOut` (sole driver) and wrong for `WatchdogIn`
  (g22 drives it too, and a totem-pole output beats a pull-up). So the fix is
  PER-NET, not per-cell, and belongs in the generator, which already knows
  each net's drivers.
- **A jam is held by the SINGLE-STEP CHAIN, not by a parity error** --
  `doradocpint.masm` settles it. One routine, two entry points, differing only
  in a flag: `DoDoradoMicroInst` passes 1, `RunDoradoInstructionStream` passes
  0, and `LSRA` shifts that bit into the carry that IS the SetSS flag on the
  following `DoControl(SetRun)`. So a single-stepped jam is held by SetSS, and
  the `Return#` that STARTS a program is deliberately free-run with SetSS
  clear so it executes and the MIR then reloads from IM. A jam never needs an
  indefinite MIR hold. (`SetMidasStopMIRClk` is what `doradomufman.masm` calls
  it -- "turn on MIR debug feature" -- and is separate.) **This retracts a
  claim made earlier the same day**, that PARC's IRTable entries carry failing
  parity on purpose because the freeze IS the jam mechanism; the inference was
  seductive and does not follow.

- **`SetRun` and `SetSS'` are BACKPLANE wires, and the testbenches were
  keeping a second copy of them.** BaseBd f02 (MC10124) drives both; what it
  translates is latched by g07, an SN74LS175 clocked by `TControlStrb'`, from
  `TCPBus.07` (the Control byte's low bit, `SetRun = 1`) and `TCPBus.08` (the
  ninth CP-bus bit, where `DoControl`'s carry lands = SetSS). So a Control
  strobe already carries both. The `setrun`/`setss_n` ports agreed by luck in
  `jam_step` and did NOT in `step_again`, which sends three Control strobes and
  updates neither -- holding SetRun asserted across a strobe that clears it.
  g07 is modelled in the strobe task now, in eight testbenches; 25/25 pass.
  **Mutation-tested, with one blind spot that matters**: latching SetRun from
  the wrong bit is caught, INVERTING SetSS is not, though `SetSS'` reaches
  ContA i03.5 (the MC10176 run/step latch, whose pin 10 takes SetRun). PARC
  makes SetSS the whole difference between single-step and free-run, so
  something else is currently deciding it -- that is the next thread.

- **With that reconciled, the disputed `cell_MC10170` fix fails DIFFERENTLY**,
  and more usefully: the jam single-steps correctly (Stop=1 after exactly two
  `clk0'` edges) and the OPERAND is wrong -- Link reads 002 where CPReg held
  002A, the same shape as `tb_operand`'s `BMux=0000`. So the single-step chain
  is sound and it is the CPReg-to-B path the parity error has been propping up.
- **The real parity bug is that IM-WRITTEN microcode fails the check**, which
  is why `exec-test` must turn IM parity off. Measured: with the enables left
  on as `InitManifolds` leaves them, the machine runs 2 `clk0'` edges and
  stops with `StopMIRClk`=0 and `Stop`=1 -- the error stopping the machine,
  not the MIR being held. The write path is traced and its generators verified:
  ProcH/ProcL `c07` pin 2 make `BMux.16/17` as `TrueA ^ alub`, ContB `e01`
  xors that pair with `bRSTK.1`/`MidasOrRSTK.2`, `d05` NORs it into `RBMuxP`.
  `TrueA` is a real constant 1 (a spare MC10102 gate with both inputs open),
  and the two `TrueA` terms CANCEL, leaving
  `RBMuxP = bRSTK.1 ^ MidasOrRSTK.2 ^ XOR(16 B bits)` -- an EVEN-parity bit,
  the opposite convention from a jam, which is consistent. Next measurement:
  write a known half-word through the real path and compare the stored
  `IMLH`/`IMRH` against `XOR(17 data bits)`. Full account in the header of
  `tb_parity.sv`.
- **Manifold register 0 is not "the parity enables".** `12'h030` is
  `DisableDoradoErrors`; writing `12'h000` to it re-ENABLES every error class
  rather than turning parity off. Doing that to four jam-based testbenches
  broke them in a way that looked exactly like fallout from an unrelated cell
  change -- they still failed with that change reverted.
- **A general microinstruction encoder exists now, and it is CHECKED.**
  `tb_compute.sv`'s `mi()` builds the five bytes from the field values per the
  layout `doradoboot.masm` states, and reproduces all THIRTEEN IRTable entries
  byte for byte (parity aside). Build new microinstructions with it rather than
  hand-assembling them -- and it yields `P015`/`P1631` for all thirteen, which
  is the dataset the open parity question needs.
- **RM's low four address bits are ~RSTK**, from an MC1662 NOR at ProcH k08.
  Harmless (a permutation within each RBase bank) but it must be applied if RM
  is ever diffed against the C emulator, whose RM[n] is index n. A
  write-then-read test cannot see this; the physical landing address must be
  checked, exactly as with the IM address reversal.
- **The ALUFM entry is NOT a contiguous field of B.** HM Table 11d says
  "ALUFMEM <- B.8, B[11:15]" -- the entry's MSB, which is the ALU's CARRY IN,
  comes from B.08 and the other five from B[11:15]. Every LOGICAL entry is
  <= 037 octal and lands in those five bits, so a sweep can pass all sixteen
  logical functions and still have the carry going nowhere.
- **B and T carry OPPOSITE senses of CPReg, and PARC's code says so.** BMux is
  the complement of CPReg; `alub` is taken off it through an MC1662 NOR which
  inverts it back, so T ends up EQUAL to CPReg while IM write data ends up
  equal to its COMPLEMENT. That is exactly why `SendViaMIR` sends IM data with
  `SetCPReg~` and `PrepareProcessor` loads T with the plain `SetCPReg`. A probe
  that uses the wrong one makes a working T look inverted -- which cost a
  detour before `compute-test` drove it PARC's way.
- **A jam must be SINGLE-STEPPED** (SetRun+SetSS, no ClrStop): free-running
  reloads the MIR from IM one clock later. `run-test`'s "ClrStop and SetRun must
  share a byte" is right for free-running and wrong for a jam.
- **A jam executes only because the MIR CLOCK IS HELD, and PARC's boot ROM
  arranges that at power-up.** The blocker was never the FF decode -- that was
  right all along, and `B<-Link'` comes from ContA a13, not from b16. Within a
  microinstruction the MIR loads from IM (on `h*clk0'`) BEFORE the register
  that acts on the FF field clocks (on `clk1'`), so a jam is overwritten
  before it can do anything. What holds it is `StopMIRClk`, and that needs two
  MANIFOLD words `doradomufman.masm` writes once, just after the supplies come
  up: `DisableDoradoErrors` ("all except IM parity errors disabled") and
  `SetMidasStopMIRClk` ("turn on MIR debug feature"). A microinstruction the
  BaseBoard put in the MIR did not come from IM and fails its parity, so the
  "freeze the MIR on a parity error" debug feature is ALSO the jam mechanism.
- **Three memory cells had their address bits BACKWARDS** -- `F10415A` (IM,
  144 packages), `F10145A` (405, the biggest cell in the machine) and `F10470`
  (the DRAM) assembled the address LSB-first where PARC wires it MSB-first.
  For IM it is proved without appeal to convention: pin 2 takes `RA.01a`,
  which comes from `TNIA.05`, the second most significant bit. A Write-IM
  addressing 195 deposited at 780 -- 195 with its ten bits reversed. Nothing
  caught it because a consistently reversed address is a PERMUTATION, which
  only bites when IM is compared with something external, i.e. Boot0.
- **Boot0's inner loop WORKS, and the `run-test` loose end was strobe SPACING.**
  `DoDoradoMicroInst` puts ClrStop in its first Control byte and SetRun in a
  later one, which our model seemed to need combined; the real BaseBoard is a
  1 MHz 6502 running `JSR DoControl` between strobes, and `SetRun` must survive
  three `RunClk'` edges to reach `dRun` through ContA i03. Space the strobes
  and PARC's sequence runs as written. `make -C verilog boot0-test` walks a
  17-byte HUNK into IM -- four microinstructions, both halves, four consecutive
  addresses -- and reads all eight half-words back out of the array. A hunk is
  17 bytes because eight half-microinstructions of seventeen bits is 136 bits.
  **And it runs on REAL MICROCODE**: 16 hunks of Xerox's own `AEmu.mb!2`,
  packed through the C emulator's `.MB` loader, walked in through the
  control-processor bus, and all 128 half-words read back out of the modelled
  ECL array. The two sides share no code -- `mb.c`/`microcode.c` against 4,096
  words of RTL generated from PARC's wire lists. That is the first
  whole-subsystem cross-check between the two models.
- **AND THE MACHINE EXECUTES IT.** `make -C verilog exec-test` releases the MIR
  clock, puts the start address in Link and jams a `Return#` -- PARC's own
  `LoadDoradoCode` startup -- and the machine free-runs out of IM: 1,242
  `clk0'` cycles in 20,000, `Stop` clear, eight distinct `TNIA` values and nine
  distinct decoded `FF` fields as it sequences. The one thing between that and
  PARC's real boot is PARITY: the test must CLEAR the IM parity enables, which
  `InitManifolds` leaves ON, or the machine executes one instruction and stops.
  Either PARC's IRTable entries carry parity the generator accepts or our
  MC10170s on ContB j20/j21 differ -- that is the next question. And note **once
  `Stop` sets it gates the clock that would clear it** (`bCLKEnable' = Stop |
  Run'` gates `clk2'`, the stop latch's own clock), so `dStop`=0, `Run'`=0,
  `Stop`=1 is a real state and only ClrStop escapes it.
- **Three memory cells had their address bits BACKWARDS** -- `F10415A` (IM,
  144 packages), `F10145A` (405, the biggest cell in the machine) and `F10470`
  (the DRAM) assembled the address LSB-first where PARC wires it MSB-first.
  For IM it is proved without appeal to convention: pin 2 takes `RA.01a`,
  which comes from `TNIA.05`, the second most significant bit. A Write-IM
  addressing 195 deposited at 780 -- 195 with its ten bits reversed. Nothing
  caught it because a consistently reversed address is a PERMUTATION, which
  only bites when IM is compared with something external, i.e. Boot0.
- **Boot0 is the open rung, and a probe already narrowed it.** `CPRegToIM#`
  carries `FF=176`, which means BOTH halves of `Link<-CPReg` -- it puts CPReg
  on B *and* reloads Link from it -- so by the time the write fires the address
  register holds `~data`, which is the hazard `cpu.c` models as
  `link_at_issue`. And the write is several steps late: `preWE'` waits on
  `CRamClock`, whose D is `Phase2'`, and the five-stage Phase ring advances at
  most one stage per step. Measured over eight steps the write fires at 3, 5
  and 6, at the RELOADED Link. PARC's Nop-after-jam is demonstrable:
  without it the pending write deposits during the NEXT jam's byte-strobing.
- **A single step is at least TWO Control strobes**, and the first step out of
  a stop is only HALF a microinstruction. `rStop` is a LEVEL that lasts until
  the next Control strobe, so ClrStop+SetRun issued once and left free-runs;
  a following strobe without ClrStop is what lets the machine stop again. And
  the phase generator comes out of reset with `StartCycle` cleared, so the
  first window is the clk0 half only -- which is exactly why PARC's
  `DoIRTableInstAndNop` never jams an IRTable entry without a Nop after it
  ("the Nop holds CPReg constant through T3 of the PREVIOUS instruction").
- **`machine-test` was running a week-old prebuilt binary**, so it had been
  reporting on RTL from before several cell fixes. It rebuilds now -- and
  rebuilt, the assembled eleven-board machine DOES NOT CONVERGE. That is
  pre-existing (stashing this session's four cell changes reproduces it) and
  it is an oscillation, not slow settling (`--converge-limit 2000` does not
  help). The same RTL settles fine under Verilator's event scheduler, which
  the new five-second `make -C verilog converge-test` asserts, so the
  difference is the evaluation model rather than the boards.
- **A disabled `MC10159` must drive its outputs LOW, and the cell drove them
  HIGH** -- 67 packages. The data sheet's truth table has it in the last row,
  and it is the whole point of an enable on a part whose open-emitter outputs
  are wired-ORed: a disabled driver contributes NOTHING. Two disabled
  multiplexers were holding eight of TNIA's twelve bits high while all twelve
  MC10121 selectors carried Link exactly right. **When a wired-OR reads wrong,
  probe each driver's own stub** -- the generator emits one per driver
  (`TNIA_04__g24_3` beside `TNIA_04__g22_14`), and one run named the package
  and exonerated eleven others.
- **`cell_MC1662` modelled the OR part and MC1662 is the NOR** -- 33 packages,
  and six of them are ContB's IM ADDRESS multiplexer, which is a 2:1 select
  only as NORs. As ORs it degenerated and every jammed Write-IM landed at
  IM[0]. MC1662/MC1664 are a complementary pair with one pinout and one `[G]`
  summary, separated by the role letter alone; `cell_MC1664`'s own comment
  already said so. With it fixed, `dRA` tracks `TNIA` and `CPRegToLink#` puts
  CPReg into Link.
- **The gates were not gating.** Every Verilog test rule ended in `| grep`, and
  a pipeline's status is the last command's, so `$fatal` left the rule green --
  two tests reported PASS in a sweep and FAIL by hand in the same minute. The
  standard `.SHELLFLAGS := -o pipefail` fix does NOTHING on macOS, which ships
  GNU Make 3.81 (`.SHELLFLAGS` arrived in 3.82). The rules capture `$?`
  explicitly now.
- **"EclDict role `OUT` is the inverting output" does not generalise**, and
  PARC's net naming cannot settle it either -- nine MC10121 packages prime the
  same pin unanimously and are wrong. What settles it is the MECL data book's
  logic diagram read BY COORDINATES: the overbars are in a font the PDF does
  not embed, but the inverting bubble rasterises and `pdftotext -bbox` gives
  each pin label's y.
- **`doradoio.mdefs` and `doradoboot.masm` are a complete specification of the
  control-processor interface** -- every Control mask, the `Clock` function's
  bits, the CPIn readout selects, and an IRTable of hand-coded microinstructions
  whose byte-layout comment is the mir-diff table from the other side.

**Pick it up from `docs/verilog-handoff.md`** -- written to be read cold.

Full plan, including what is missing and the suggested order:
`docs/verilog-from-sil.md`. The board-by-board cross-check of the C
emulator against those same netlists (all eleven boards of a working
machine, six gaps found, no contradictions):
`docs/sil-netlist-crosscheck.md`.

## External resources

- **"The Structure of Cedar"** — Swinehart, Zellweger, Hagmann, ACM SIGPLAN
  1985, pp. 230-244:
  https://worrydream.com/refs/Swinehart_1985_-_The_Structure_of_Cedar.pdf
  The architects' account of the system we boot, one version before 6.1.
  Scanned images: text extraction returns binary noise, so read the pages
  visually. Figure 1 is the entire system on one page. Four things it
  settles, each of which changed a plan here: colour is an option on a
  second frame buffer reached through Imager and **Griffin** is the colour
  application (`docs/color-graphics-todo.md`); **FS will not open a
  remotely-named file for writing**, so everything our STP server serves is
  a read-only cache and edits must be written locally first; a **DF file is
  the period-correct container for "the sections and figures of a paper"**;
  and **Rollback "has become the conventional way to restart Cedar"**, which
  is why the checkpoint crash matters more than its symptom suggests.
  Details in `docs/parc-feedback-todo.md` section M.

- **Computer History Museum Xerox PARC archive** — https://xeroxparcarchive.computerhistory.org/ 
  The full IFS file-server contents from PARC. Microcode sources, MB
  binaries, schematics, tools. See `docs/chm-archive.md` for a curated list
  of Dorado-relevant directories.
- The local file `chm/cross-reference.html`
  is a 568k-line filename → archive-location index. Grep it.
- **`AltoInfo/`** — Alto-emulator source trees (ContrAlto2 in C#, salto in
  C) plus Alto/Trident pack images. Provides: a Trident T-80/T-300 drive
  emulation we can port, an architectural template for a microcoded
  engine in C, and a byte-level reference oracle for cross-validation
  once we run the Alto-emulator microcode on the Dorado. Survey:
  `docs/altoinfo-survey.md`.

## Working norms for this project

- **Treat the Hardware Manual as canon.** When two sources disagree
  (manual vs. schematic vs. memo), prefer the September 1981 Hardware
  Manual unless we are deliberately tracking a later revision.
- **Use the original terminology** in code and comments: IM, IFUM, RM,
  STK, ALUFM, T, Q, Cnt, ShC, Pd, Md, Mar, BR, MemBase, MemBX, Link,
  TPC, JCN, RSTK, BSEL, ASEL, ALUF, LC, FF, BLOCK. Octal literals are
  fine where the manual uses them. This makes cross-referencing painless.
- **Cite section numbers** in commit messages and code comments when
  implementing a tricky piece (e.g. `// HM §6.2 IFUJump entry vector`).
- **Do not invent behavior.** If a microcode source uses a feature we
  haven't implemented, find the manual passage that describes it before
  writing code.
- **No emojis in code or docs** unless I explicitly ask.

## Cross-validation strategy

Once the microengine + memory + IFU + tasking are working, load the
**Alto-emulator microcode for Dorado** — `chm/dorado/AEmu.mb!2`
(40 KB, the canonical source) or the Ether-bootable
`chm/microcode/AltoMesaDorado.eb!1` (35 KB). That makes the Dorado
present the Alto opcode set. We can then run the same Alto disk image
(e.g. `games.dsk` from `AltoInfo/ContrAlto2-beta/Disks/`) on **both**
ContrAlto2 and our Dorado emulator, and diff architectural state
opcode-by-opcode.

This is a much shorter feedback loop than trying to bring up Mesa first.
Mesa requires the full Map/Pipe/BR machinery; the Alto emulator on
Dorado uses a much smaller subset.

Once Alto-on-Dorado works, switch to `chm/dorado/Mesa.mb!3` and bring
up Pilot. From there, `chm/dorado/Cedar.mb!6` brings up Cedar; UnBug
contains `DoradoLisp.MB` for the Lisp emulator if we want it.

## Open questions (resolved)

These were the original pre-coding questions; here is where each landed.

1. **Which microcode first — Mesa or Alto?** Alto, as predicted. The
   Ethernet path serves `AltoMesaDorado.eb` (the Alto emulator on Dorado);
   it loads and starts via the real Initial -> LoadRam path.
2. **Do we have the binaries locally?** Yes - `chm/dorado/` holds Mesa,
   Cedar, AEmu, and the `.eb` netboot worlds; see `docs/chm-archive.md`.
3. **Disk format — T-80 or T-300?** Both are modeled (ContrAlto/Bitsavers
   byte layout, 2074-byte sectors). Moot for first boot: no installed
   Dorado pack exists, so we boot over Ethernet instead.
4. **Display vs disk first?** Display first (framebuffer is implemented).
   It renders but stays blank until booted software installs a display
   list, which is gated on Stage-2 software boot.

The live open question is now Stage-2 software boot - see
`docs/ethernet-local-boot-plan.md` Phases 6A/6B and the punch list in
`docs/handoff.md`.
