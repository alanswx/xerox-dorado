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
(`docs/schematic-audit.md`); the bring-up fixed five real microengine bugs
(WF/RF field opcodes, TisId/RisId + IFetch, Q<-B for Pipe sources, the
Overflow branch condition, and shifter Pd-mux masking). The emulator runs
~24 M microinstructions/s (≈1.4x the real 16.67 MIPS Dorado) after caching
the per-step trace `getenv()` calls (`dorado_trace_flag`, ~2.7x speedup). It
also builds to **WebAssembly** (`make web`) and auto-deploys to GitHub Pages
(`.github/workflows/deploy-pages.yml`): a dropdown picks the Alto games,
NetExec, or Cedar 6.1.

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

**Path B - Cedar/Pilot: BOOTS TO THE LOGIN PROMPT.** Initial netboots
`CedarDorado.eb!6` (the Cedar/Mesa microcode), the matched Pilot germ is
planted into VM (`--germ`), and Cedar boots its Pilot **physical volume** from
a PDI disk image (`--pilot-disk ../CedarDisk/CedarDorado-boot.pdi`,
`--boot-reason disk`). The full chain reaches **Cedar 6.1.0's SimpleTerminal
login prompt** ("Please login… / Name:"), and the **keyboard works**: typing a
name + Return advances the login. Run it with `make run-cedar`. **Germ and
microcode versions must match** -- `CedarDorado.eb!6` (17-May-1984) shipped
with Cedar 5.3/6.0/6.1, so the matched germ is `Dorado.germ-6.1.6` (Cedar
6.1), NOT the older `Dorado.germ!4` (Dec 1983, version-mismatch fault).
Cedar's keyboard is delivered to KeyBits at absolute `LONG[177033B]` with a
display vertical-field naked-notify driving SimpleTerminalImpl's watcher
(`machine_cedar_io`; grounded in `TerminalDefs.mesa KeyName`,
`TerminalHeadDorado.mesa`, HM Table 24). The framebuffer is 1024x808 and each
world presents at its native raster (Alto 808x606, Cedar lf 1024x808). Live
detail + next steps: `docs/CONTINUE-HERE.md`.

The Stage-2 EFTP/Mayday boot server also serves Cedar boot files
(`CedarNetExec.boot`, `NEWOS.BOOT`, `OthelloDorado.boot`) byte-exact -- the
next stage once the germ reaches `DoInLoad` over the net rather than the disk.

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

## Phase 2 (Verilog) — non-goal for now

Hold off on RTL until the C emulator boots something real. The C
emulator's structure (per-section modules, cycle-tick driver, Pipe and
ALUFM as explicit memories) should map fairly directly to Verilog modules
later. Avoid C tricks that won't translate (function pointers per opcode,
bit-fields with implementation-defined layout) in code paths intended to
become RTL.

## External resources

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
