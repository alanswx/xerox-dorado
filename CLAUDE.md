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
(67,960 lines, plus 4,599 of cell models); 44 cell models cover 82.9% of logic packages; the 6502 and
6532 are real cores; **all 26 PROMs are generated from PARC's own BCPL**
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
503 internal nets of which 83 are `wor` ECL open-emitter buses, 405 ports out
to cables, lint clean, and `--boards` takes any subset so the machine can be
brought up a board at a time. One finding fell out of it: **DispM plugs into
DispY rather than replacing it** -- 42 nets are shared by the two display
boards and no other -- so a colour machine has both.

**Pick it up from `docs/verilog-handoff.md`** -- written to be read cold, now
with the port-list fix as a self-contained first task and then the work to
wire the machine together and test it against the C emulator.

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
