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
| `DoradoDocs/manuals/DoradoManual-A.press!8.pdf` | 142 KB | Same content as Hardware Manual front matter, "press" file format. Treat as duplicate. |
| `DoradoDocs/manuals/DoradoManual-B.press!8.pdf` | 190 KB | Companion press file. |
| `DoradoDocs/manuals/DoradoManual-Figs.press!8.pdf` | 93 KB | Figures only (programmer's view, card cage, control section, etc.). |

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
| `DispY.pdf` | Y-channel display | Color/extended display output. |
| `DskEth.pdf` | Disk + Ethernet (combined card) | Disk T-80 controller and 3 Mbit/10 Mbit Ethernet. |
| `PCMSA.pdf` | PC-MSA board | Memory storage array |
| `ECL_SSI_Symbols.pdf`, `Library_Symbols.pdf` | Schematic conventions | Reading any of the above. |
| `Board_Revisions_Aug1980.pdf` | Board rev list (Aug 1980) | Cross-checking which schematic matches a given machine. |
| `manuals/photos/DISPM-APCREV-DA_*.jpeg` | Board photographs | Sanity-checking package types and hand-wired patches. |

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
`xeroxparcarchive.computerhistory.org`. URL conventions documented in
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

## Project plans (kept in sync with code)

| File | Notes |
|---|---|
| `boot-bringup-plan.md` | Phased plan from "Boot0 free-runs from IM" (current state) to "Mesa boots Pilot from disk." Phases A–F, with HM section refs and exit criteria. Update when phases land. |
| `memory-architecture.md` | Reference for the Map/cache/Pipe/BR design. Read before implementing Phase B. |
| `io-systems-architecture.md` | Reference for Slow I/O, Fast I/O, the task scheduler/Wakeup-Block-Next protocol, plus the Disk, Display, and Ethernet controllers. Read before implementing Phase 7. |
| `jcn-encoding.md` | The JCN field's 7-way encoding split (local / global / long / conditional / return / IFU jump). Distilled from HM §4.3 + Figure 6. |

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
