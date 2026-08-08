# Local artifact index

Every file currently in the project tree, grouped by purpose. See the
parent `CLAUDE.md` for the project mission. See `chm-archive.md` for
external resources hosted at the Computer History Museum.

Sizes are approximate; "★" marks files you will reach for repeatedly.

---

## Primary references (read these first)

| File | Size | Notes |
|---|---|---|
| ★ `DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf` | 14 MB | E.R. Fiala, 14 Sep 1981. The Dorado architecture bible. 150 pp. + glossary + crib sheet. |
| ★ `DoradoDocs/manuals/Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf` | 836 KB | Ed Taft memo, 21 Apr 1980. How the BaseBoard, Bootstrap microcode, Initial microcode, and emulator microcode hand off. Read before touching the boot path. |
| ★ `DoradoDocs/manuals/Introduction_to_Cedar_7.0.md` (+ `04_Introduction_to_Cedar_Version_7.0_May1987.pdf`) | 1.1k lines | The PARC user-level guide to living in the Cedar world: herald boot buttons (BootTool), CommandTool, DF files/Bringover, checkpoint/rollback, user profiles, Iago, boot switches, MP codes. Written for Cedar 7.0 — one version past our 6.1 — but the concepts and commands match what our booted desktop presents. |
| `DoradoDocs/manuals/DoradoManual-A.press!8.pdf` | 142 KB | Same content as Hardware Manual front matter, "press" file format. Treat as duplicate. |
| `DoradoDocs/manuals/DoradoManual-B.press!8.pdf` | 190 KB | Companion press file. |
| `DoradoDocs/manuals/DoradoManual-Figs.press!8.pdf` | 93 KB | Figures only (programmer's view, card cage, control section, etc.). |

## Trident drive references

Located in `DoradoDocs/manuals/trident/`. These are Century Data manuals from
Bitsavers for the physical Trident drives behind the Dorado disk controller.
Use them with Hardware Manual section 9 and `docs/disk-architecture.md`.

| File | Size | Notes |
|---|---:|---|
| ★ `76205-902_Performance_Specification_Models_T25_T50_T80_T200_and_T300_198011.pdf` | 2.8 MB | T-80/T-300 physical specs and timing. |
| ★ `Trident_T25_T50_T80_OEM_Reference_Manual.pdf` | 2.0 MB | Raw T-25/T-50/T-80 interface, sector/index timing, read/write timing. |
| `76205-205_T25_T50_T80_Installation_and_Operation_197905.pdf` | 11 MB | Operation, cabling, configuration context. |
| `76205-303_T25_T50_T80_T85_Maintenance_198107.pdf` | 5.7 MB | Maintenance and diagnostic behavior. |
| `Trident_T25_T50_T80_schem_Mar81.pdf` | 7.1 MB | T-25/T-50/T-80 drive schematics. |
| `76200-402_T200_T300_Theory_of_Operation_198101.pdf` | 11 MB | T200/T300 theory of operation for larger packs. |
| `76206-304_Trident_Disk_Drives_Models_T202_T302_Maintenance_Manual_Apr82.pdf` | 12 MB | Later T202/T302 maintenance reference. |
| `Trident_1150_Formatter_Performance_Specification_1977.pdf` | 2.5 MB | Formatter performance reference for timing/interface questions. |

## Schematics — board PDFs (1980-vintage revisions)

Located in `DoradoDocs/schematics/`. These are the schematics that ship with
the Hardware Manual; the `DoradoDocs/doradodrawings/` directory contains
multiple revisions of each.

| File | Section | Use when |
|---|---|---|
| `Backplane.pdf` | Backplane wiring | Tracing inter-board signals (A/B/Mar/Pd/Md). |
| `Baseboard.pdf` | BaseBoard | Modeling boot logic, EPROM access, status LED. |
| `ProcH.pdf`, `ProcL.pdf` | Processor (high/low halves) | Implementing ALU, RM, STK, T, shifter. |
| `Control_A.pdf`, `Control_B.pdf` | Control section | JCN decoding, task scheduler, Link/TPC. |
| `IFU.pdf` | Instruction Fetch Unit | IFUM, prefetch, IFUJump entry vector. |
| `MEMC.pdf`, `MEMD.pdf`, `MEMX.pdf` | Memory: Cache, Data, eXtension | Cache, Map, Pipe, BR, ECC. |
| `DispY.pdf` | Display, **monochrome** | The DDC: Alto/7-wire mono video, cursor, OIS terminal + keyboard link. |
| `DskEth.pdf` | Disk + Ethernet (combined card) | Disk T-80 controller and 3 Mbit/10 Mbit Ethernet. |
| `PCMSA.pdf` | PC-MSA board | Memory storage array |
| `ECL_SSI_Symbols.pdf`, `Library_Symbols.pdf` | Schematic conventions | Reading any of the above. |
| `Board_Revisions_Aug1980.pdf` | Board rev list (Aug 1980) | Cross-checking which schematic matches a given machine. |
| `manuals/photos/DISPM-APCREV-DA_*.jpeg` | Board photographs | Sanity-checking package types and hand-wired patches. |

## Sil design data — `chm/sil/` (the design INPUT, not a rendering)

PARC's own design-automation output for all sixteen boards. Unlike the PDFs
above, which are renderings, these are the files the machine was **built
from**, and the plain-text ones use the machine's own signal names — the same
vocabulary as the Hardware Manual and the C emulator.

| File | What | Use when |
|---|---|---|
| `<Board>-Rev-Xx.wl` | **Wire list — the gate-level netlist.** Every net by name, every package/pin, with pin DIRECTION (`f17.15o` drives, `g17.12i` receives) and board coordinates. | Writing RTL; tracing what actually drives a signal. |
| `<Board>-Rev-Xx.lc` | Component list: part number → packages. | Building the cell library; sizing the design. |
| `<Board>-Rev-Xx-C.nl`, `-E.nl` | Backplane interface = the board's port list. | Cross-checking the emulator; declaring Verilog module ports. |
| `<Board>NN.sil` | Drawing sheets, Sil's binary format. | Human reading only — the `.wl` is derived from these and is plain text. |
| `Build.cm`, `Print<Board>.cm` | PARC's build scripts. | Reproducing their toolchain. |

`tools/sil_netlist_report.py` reads the `.nl` and `.lc` files
(`--chips`, `--grep`). Boards present: ProcH, ProcL, ContA, ContB, IFU,
MemC, MemD, MemX, DispY, DispM, DskEth, BaseBd, msa, PCMSA, IOTest, Music.

- **`docs/sil-netlist-crosscheck.md`** — the C emulator checked against all
  eleven boards of a working machine. Confirms every field width, register
  width and branch condition; names six gaps; found no contradictions.
- **`docs/verilog-from-sil.md`** — what these files mean for Phase 2:
  module boundaries, the ~50-part cell library, the five-tier test plan.

## Schematics — every revision (`DoradoDocs/doradodrawings/`)

This directory holds the full revision history of every board, in `.press`
PDF form. Naming pattern: `<board>-<reviewer>Rev-<rev>.press!N.pdf`.
Reviewers seen: `apcRev` (production), `mwRev` (multiwire prototype),
`pcRev` (printed-circuit). Use this directory when you need to verify a
specific component value or trace; use the top-level `DoradoDocs/schematics/`
copy for everyday reading.

Highlights:

- `RevStatus.press!1.pdf` — list of all board revisions and their status.
- `DoradoLibraries.press!1.pdf` — schematic symbol library.
- `doradoBPDwgs.press!1.pdf` — backplane drawings.
- `BaseBd-*.pdf` (3 revs), `baseboardpc.press!2.pdf` — BaseBoard.
- `ContA-*.pdf`, `ContB-*.pdf` — Control board halves.
- `DispM-*.pdf` (4 revs), `DispY-*.pdf` (8 revs) — display variants.
- `DskEth-*.pdf` (6 revs) — disk+ethernet board.
- `IFU-*.pdf` (3 revs).
- `MemC-*.pdf` (2 revs), `MemD-*.pdf` (3 revs), `MemX-*.pdf` (4 revs).
- `ProcH-*.pdf` (2 revs), `ProcL-*.pdf` (2 revs).
- `IOTest-*.pdf` (3 revs) — I/O test board (use for diagnostic microcode).
- `msa-Rev-Bg.press!1.pdf`, `PCMSA-rev-Pa.press!4.pdf` — MSA / memory storage array.
- `Music-Rev-Ac.press!1.pdf`, `Mike.press!1.pdf` — accessory boards.
- `terminaldrawings.press!1.pdf` — DispM keyboard/mouse.
- `*-mwRW.press!1.pdf` — single-page rework deltas between revs.

`DoradoDocs/doradologic/` adds `MWBoardLayout.press!1.pdf` (multiwire
layout) and `IFU-Cf-to-Cg-mwRW.press!1.pdf` (IFU rework).

## Firmware — BaseBoard EPROMs (`firmware/`)

Five 2 KB EPROM dumps, dated 24 Nov 1987. Sockets and address mapping
confirmed by `labels.JPG`:

| File | Socket | Address range | Content |
|---|---|---|---|
| `B-08.BIN` | row B slot 8  | F000–F7FF | BaseBoard 6502 code (low half) |
| `B-10.BIN` | row B slot 10 | F800–FFFF | BaseBoard 6502 code (high half; 6502 reset vector at FFFC lives in here) |
| `C-08.BIN` | row C slot 8  | C000–C7FF | Initial microcode + tables |
| `C-10.BIN` | row C slot 10 | C800–CFFF | Initial microcode + tables (continued) |
| `C-12.BIN` | row C slot 12 | D000–D7FF | extension EPROM (post-1981 add-on; mostly 0xFF in this dump) |

The 1981 Hardware Manual + Booting memo describe four EPROMs at F000,
F800, C000, C800. The 1987 set adds a fifth at D000–D7FF — likely the
Initial microcode grew when later emulators (Cedar, multi-instruction-set
IFU) were added. `B-08.BIN` and `B-10.BIN` together hold the full 4 KB
BaseBoard 6502 program; the C/D chips hold compressed Bootstrap+Initial
microcode plus loader tables.

`labels.JPG` is the photograph of the chips in their sockets — keep it.

`chm/dorado/doradobaserom.mb!13` (49 KB) is the **MB-format
source-level dump** of the same BaseBoard ROM (file revision 13). Use this
when you want symbols, not raw bytes. Source-level `.masm` files for the
BaseBoard ROM are in `chm/dorado/expanded/doradobaserom.dm!12_/`.

## AltoInfo/ — Alto emulator references

Four artifacts dropped by the user. **Two are source trees we'll lift
from**, two are pre-built binaries useful as cross-validation oracles.
Detailed survey lives in `altoinfo-survey.md`. Headlines:

- `Contralto2-2.0-Beta/` — ContrAlto v2 C# source. Has a working
  **Trident T-80 / T-300 SMD drive emulation** (`TridentController.cs`,
  `TridentDrive.cs`, `TridentTask.cs`, `DiskPack.cs`) — same drive
  family as the Dorado uses. Port this to C for our emulator.
- `salto/` — Juergen Buchmueller's C Alto emulator (GPL-2). Useful as a
  C-language **architectural template** for a microcoded engine: see
  `src/cpu.c`, `include/cpu.h`, the per-task split (`curt.c`, `dht.c`,
  `dvt.c`, `dwt.c`, `ksec.c`, `kwd.c`, `mrt.c`). Don't copy GPL code;
  use it as inspiration.
- `ContrAlto-mono/`, `ContrAlto2-beta/` — pre-built binaries. The
  binary drop's `Disks/` includes **`spruce-server.dsk300`** (276 MB,
  real Trident T-300 pack image) and a dozen Diablo `.dsk` images.

## CHM downloads — `chm/` (~12 MB, 339 files)

Pulled April 2026 from **two** CHM mirrors:
`xeroxalto.computerhistory.org` and
`xeroxparcarchive.computerhistory.org`; the PARC source-code landing
page is
`https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html`.
URL conventions documented in
`chm-urls.md`. Full inventory in `chm-archive.md` §7. Highlights:

- **`chm/dorado/`** ★ — the canonical `_cd8_/dorado/` tree. Microcode
  (`Mesa.mb!3`, `Cedar.mb!6`, `AEmu.mb!2`, `DSemu.mb!1`,
  `TriEmu.mb!3`, `TriMesa.mb!3`, `doradobaserom.mb!13`), tools
  (`LoadMB.run!6`, `ReadMB.run!1`), all Initial-stage Ether boots,
  plus 15 expanded `.dm` archives (incl. **`Bootstrap.mb` +
  `Initial.mb`** in `expanded/bootstrap.dm!20_/`,
  **BaseBoard ROM `.masm` sources** in `expanded/doradobaserom.dm!12_/`,
  IFU/kernel/memA/memMisc/TriconD/eventCounters/edp/ftest/DSemu
  microcode and diagnostics, **DoradoLisp.MB** in
  `expanded/UnBug.bfs!1_/`, plus a complete worked Micro→MicroD→.MB
  example in `expanded/ftest.dm!1_/`).
- **`chm/microcode/`** — 17 Ether-bootable / packed-binary worlds from
  `Io/Murray/` on the older xeroxalto mirror. Dorado (8) + D0 (7) +
  DLion (1) + Dicentra (1). Useful as alternative test fixtures and
  for cross-validating MB-vs-EB wrapping.
- **`chm/microd/`** — full MicroD BCPL source (31 files). Start with
  `mdfields.d` + `printmb.bcpl` to learn the .MB layout.
- **`chm/microd-manual/`** — `microd.tty!2` user manual + `microd.run!2`
  Alto executable + `microd.syms!2` symbols.
- **`chm/io-doradologic/`** — high-value selections from
  `io/doradologic/`: `DoradoBaseDebug.dm!1`, `DoradoFiles.dm!1`,
  `DoradoBuildFiles.dm!1`, `DoradoProms.run!3`,
  `wirelengthanalysis.mesa!1`, `RevStatus.memo!1`, plus saved indexes.
  The per-board SIL `.dm` archives (BaseBd, ContA/B, DispM/Y, DskEth,
  IFU, MemC/D/X, ProcH/L) were skipped — we have the equivalent PDFs.
- **`chm/indigo-dorado/`** — just the directory index. Mostly older
  versions of files we have.

## CHM cross-reference index

`chm/cross-reference.html` — a 45 MB,
568,316-line HTML file mapping every file name on the Xerox PARC archive
to one or more `xeroxalto.computerhistory.org/...` URLs. **Grep this
file** when you need to find where a specific Mesa, Cedar, or Dorado
source file lives.

Example queries:
- `grep -i 'doradobaserom' "chm/cross-reference.html"`
- `grep -i 'mesa.mb\|mesa.eb\|mesa.dm' "chm/cross-reference.html"`
- `grep -i 'microd' "chm/cross-reference.html"`

## Performance

- `docs/performance-plan.md` — the whole 2026-08-04/05 effort: where the
  time went, what each change was worth, and what is left. Both native
  paths went from under half real-Dorado speed to faster than it.
- `docs/performance-methodology.md` — **read before optimizing anything.**
  How to measure this emulator without fooling yourself: the unit is
  microinstructions (NOT `--cycles`, which counts 6502 cycles at 3.70 per
  microinstruction), the gate is byte-identical framebuffers on three
  worlds, and profile attribution lies under LTO. Every trap in it produced
  a confidently stated number that was false.
- `docs/color-graphics-todo.md` — **everything about the COLOUR display in
  one place**: reference (DispM sheet map, TIOA device numbers, the
  bit-level slow-IO word formats, mixer modes and RAM sizes, how the guest
  finds a colour monitor, what the Interlisp driver does) plus the ordered
  task list. The DDC drives mono and colour from the same board, the
  microcode is already inside `Mesa.mb`/`Cedar.mb`, the control writes
  already reach our display device, and `DORADOCOLOR.LCOM` + `COLORDEMO`
  are on the Lyric pack to test with. Start at task 0: trace whether any
  world actually programs the colour RAMs.
- `docs/stp-scan-design.md` — design for the one big non-interpreter item
  left, Cedar's 14.3% STP connection scan. Two routes, with the risks of
  each; not yet implemented.

## First-hand background

- `docs/parc-veteran-notes.md` — pointers and hardware history from two
  former PARC engineers (July 2026): PSAdd prefix servers, the Cedar
  build-system `.df` genealogy, CedarSnapshotDorado.boot, board wiring
  technologies (stitch-weld / Multi-Wire / PCB), SIL, Midas + the
  manifold, ECL PROM generators, and the original microcode source
  archives — cross-referenced against our local mirrors. Also the
  Cedar/CedarChest split, the CedarPS printing lineage, and where the
  XC1-2-2 fonts live.
- `chm/cedar/cedar6.1-docs/` — Cedar 6.1's own documentation, fetched on
  that advice: the release message, CommandTool command reference,
  CedarChest conventions, the glossary/introduction/overview, the style
  and syntax guides, `EtherBoot.tioga`, and **Xerox's own sample user
  profiles** (`StandardUser.Profile`, `StandardUser-LoginWork.cm`,
  `NoviceUserProfile.Tioga`). The profiles are the reference for how
  software was actually loaded and launched — `Install`, `.cm` command
  files, `Alias`, `CreateButton`, `.load` manifests, profile hooks — and
  are what our friendly desktop is modeled on.

## Project plans (kept in sync with code)

| File | Notes |
|---|---|
| `running-the-emulator.md` | ★ **Runbook** — every software combination (microcode worlds, Pilot germs, OS/app boot files), which ones work, and the exact command to load each. Current status snapshot of both boot paths. |
| `CONTINUE-HERE.md` | ★ **Live bring-up log** — newest-first, session by session. The current germ-boot (Route B) state and the next concrete step live at the top. |
| `cedar-media-provenance.md` | How the reconstructed Cedar Pilot `.pdi` volumes were built with Rusty Backup, their exact boot-fixture recipe, and safe modification constraints. |
| `demo-plan.md` | ★ **What to make work, and in what order, for a demo people can try** — 244 of CedarChest 6.1's 316 packages are usable today; the blockage is concentrated in seven packages, all of which CedarChest 6.0 could fill. Ranks the work by (works) × (visible) ÷ (effort): Gargoyle is 4 missing files away, Sil has an untried retry, and the two other worlds have their own openings — Smalltalk needs one mouse click driven in, Interlisp needs `.DCOM` loading over the wire to reach 295 untouched library packages. Ends with the 10-minute visitor script. |
| `cedar-archive-gaps.md` | ★ **What the CHM archive is missing from Cedar 6.0 / 6.1 / 7.0** — a file-level audit measured against the archive, written to take to the museum. Cedar 6.1's base release is 99.5% complete (13 missing of 2,584); CedarChest 6.1 is 91% (596 of 6,422, concentrated in 18 whole-directory losses); **Cedar 7.0 is absent, not incomplete — zero files**. Names every gapped package, whether CedarChest 6.0 could fill it, and the two that matter here: `<CedarChest6.1>BiScrollers>` (blocks Sil and the schematics demo) and `<Cedar6.1>Dorado>`. The ask is framed against Xerox's own DF manifests. |
| `sil-schematics-handoff.md` | ★ **The Dorado's own schematics in Sil vector format** — 506 `.sil` files under `io/doradologic/`, how they reach the emulated volume (32 ProcH sheets committed), and the one thing in the way: `Run Sil` -> `VersionMismatch[BiScrollers]`. Carries the ONLY copy of the `User.Profile` block that installs Sil's whole chain at cold boot (selective `Bringover -o`, Run order from each `.load`), the shipping status of everything from this line of work, and the two bugs it turned up (case-sensitive `.DF` matching; the desktop bake typing the login 1.7 B cycles too early). |
| [lisp-leaf-handoff.md](lisp-leaf-handoff.md) | Interlisp-D over Leaf: the full server (incl. the IFS leader page), the init-file saga, the pack ceiling, and the working Reference Manual demo, native + browser (2026-08-01..04) |
| `cedarchest-packages.md` | ★ **CedarChest 6.1 inventory** — all 316 packages PARC shipped with Cedar 6.1, categorised, with a column for the 20 we mirror, and an analysis of which are worth fetching and why. Leads with the finding that the **Dorado's own schematics survive as 506 `.sil` vector files** under `io/doradologic/`, and that `Sil.df!5` (Sil for Cedar 6.1, symbol libraries included) is already mirrored — so the machine could display its own logic drawings in the original editable form rather than as a raster. Also documents the three traps in installing a package (run name != DF name, `Bringover -p` skips data files, fonts attach only at cold boot). |
| `cedar-file-server-plan.md` | ★ **Cedar as a file server** — `ls` on `[Cedar]<Cedar6.1>` (DONE 2026-07-30, §7: STP Enumerate + the LookupFile packet exchange) and why `OpenR` opens the wrong file in the browser, with measured evidence. Reference section on Cedar's remote-file stack (FS → FSRemoteFile → STP over Pup socket 3), the exact STP mark bytes and enumeration wire shape from the original Xerox server source, pseudo-servers (`PSAdd`), and the VersionMap that drives `OpenR`/`FindR`. Phased plan. Written after feedback from a Cedar author. |
| `schematic-audit.md` | Schematic-vs-emulator audit findings (microengine cross-checked against the board drawings) — discrepancies, confidence, what's verified consistent. |
| `hardware-specs.md` | Implementation specs for hardware the emulator does not yet faithfully model (Hold/Miss state machine, memory pipeline, ECC/Pipe4, IFU pipeline timing, disk read, DDC back-channel). |
| `handoff.md` | ★ **Read first** if you're a new session picking this up. Current state, build/run, what works, what doesn't, pitfalls, suggested first action. Includes the guest-file authoring traps (CR line endings, `Bringover -p`) and the performance profile. |
| `boot-bringup-plan.md` | Phased plan from "Bootstrap microengine runs" (current state, post ALUFM/quadrant fixes) to "Mesa boots Pilot from disk." Includes the normal boot sequence reference (Path A power-on/3-push, Path B LoadMB shortcut). |
| `memory-architecture.md` | Reference for the Map/cache/Pipe/BR design (HM §5). Read before memory work. |
| `io-systems-architecture.md` | High-level reference for Slow I/O, Fast I/O, the task scheduler/Wakeup-Block-Next protocol, plus an overview of the Disk, Display, and Ethernet controllers. Read first; then drill down into the per-subsystem docs below. |
| `display-architecture.md` | Detailed Display Controller reference (HM §11). DDC, NLCB/CLCB fields, HRam/Mixer/MiniMixer, 7-wire interface, back-channel keyboard/mouse messages, DHT/DWT microcode + wakeup logic. |
| `disk-architecture.md` | Detailed Disk Controller reference (HM §9). Trident T-80/T-300 specs, TIOA registers, DiskControl bits, Format RAM table, Sequence PROM tables (read+write), tag register layouts, KSTATE/KSTAT muffler signals, Fire Code ECC polynomial + recovery procedure. |
| `ethernet-architecture.md` | Detailed Ethernet Controller and netboot reference (HM Ethernet chapter + `InitialEther.mc` + CHM Pup gateway sources). Documents TIOA `015B`/`016B`, EIT/EOT semantics, `MicrocodeBootRequest`/`MicrocodeBootReply`, EB chunking, and the packet-level fake needed to netboot emulator microcode. |
| `ethernet-local-boot-plan.md` | Implementation plan for the hybrid first-boot path: Initial loads `AltoMesaDorado.eb` through fake Ethernet, then the loaded Alto emulator boots the OS from the local `spruce-server.dsk300` Trident pack. |
| `interlisp-d-boot.md` | Interlisp-D-on-Dorado boot model from a first-hand Xerox user (Nick): net-boot Alto exec -> `Lisp.run` (BCPL) loads `DoradoLispMC.EB` + maps a disk `LISP.VIRTUALMEM` swap. Multi-partition Alto/Diablo disk format (reconstructable). Records the CHM `phylum/lisp/lyric/` files fetched to `chm/lisp/` and the emulator validation (all three `.eb` load via our Stage-1 path; `DoradoLispMC.EB` is a brand-new microcode world). |
| `research/chm-hard-disk-pack.md` | CHM research note on the Dorado Initial hard-microcode disk pack. Distinguishes the Dorado Pilot 28-sector, 256-word-page layout from Alto/IFS 9-sector Trident packs, records the private page-4 boot-file convention, and lists what is still missing. |
| `jcn-encoding.md` | The JCN field's 7-way encoding split (local / global / long / conditional / return / IFU jump). Distilled from HM §4.3 + Figure 6. |
| `running-diagnostics.md` | How to run PARC's real Dorado hardware diagnostics via `build/rundiag` — a regression gate. |
| `dorado-software.md` | Survey of all Dorado-targeted software found in the CHM archive. |
| `cedar-boot-plan.md`, `cedar-disk-boot-plan.md` | Cedar bring-up plan docs (largely historical now that Cedar boots to the desktop; kept for the reasoning and references). |
| `disk-subsystem-plan.md` | The real-disk-controller plan (`--disk-real`) that will retire the PDI IOCB shim. |
| `cycle-accurate-timing-plan.md`, `timing-project-scope.md`, `fidelity-audit.md` | Cycle-accuracy program: the timing plan, its scope, and the fidelity audit that motivates it. |
| `alto-game-compatibility.md` | Per-game compatibility notes for the Alto boot files (what runs, what needs RAM microcode, pixel gates). |
| `contralto-oracle-validity.md` | Where ContrAlto is a valid cross-validation oracle (architectural state) and where it is not (I/O timing). |
| `architecture.md`, `glossary.md` | Distilled architecture notes (HM §1–4) and the Dorado jargon glossary. |
| `parc-archive-survey.md` | End-to-end survey of `xeroxparcarchive.computerhistory.org` for Dorado material. Covers the four Alto disk-pack image formats (`.bfs`/`.altodisk`/`.copydisk`/`.disk`), the 9 BFS packs in `BasicDisks/`, why no Trident packs are public, and the tools (`restore_alto_files`, `palo`, ContrAlto's AltoFS) for reading/writing them. |

## Suggested reading order for a new contributor

1. CLAUDE.md (this directory's parent) — mission and constraints.
2. `Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf` — the whole thing
   (40 minutes). Tells you what software/firmware exists and how it
   chains together.
3. `Dorado_Hardware_Manual_Sep1981.pdf` §1–§4 — Introduction, Overview,
   Processor, Control. Pay attention to the field tables.
4. Hardware Manual §5 (Memory) — the Map/cache/Pipe model is unusual.
5. Hardware Manual §6 (IFU) — the way prefetch interacts with branching
   is essential before implementing emulator microcode.
6. Hardware Manual §15 (Glossary) — bookmark.
7. Pick one I/O controller (§9, §10, or §11) and skim it.
