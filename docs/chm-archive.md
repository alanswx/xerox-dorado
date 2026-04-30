# Computer History Museum — Xerox PARC archives

CHM hosts **two** Xerox PARC IFS mirrors:

- **https://xeroxalto.computerhistory.org/** — Alto-era material;
  contains Dorado boot files (`Io/Murray/`), MicroD source
  (`Indigo/AltoSource/MICROD.DM!2_/`), and the SIL/Route board
  toolchain (`_cd6_/sil/`).
- **https://xeroxparcarchive.computerhistory.org/** — the fuller PARC
  IFS dump, including the canonical `_cd8_/dorado/` source tree, all
  Dorado microcode `.mb`/`.midas` files, the Bootstrap and Initial
  microcode dumps, the BaseBoard ROM source, the `LoadMB`/`ReadMB`
  Alto tools, and `io/doradologic/` schematic-source archives.
- **https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html**
  — CHM's source-code landing page for the PARC archive; use this when
  starting from the public collection rather than a known IFS path.

URL conventions for both servers are documented in `chm-urls.md`.

The local file `chm/cross-reference.html`
(45 MB, 568,316 lines) was generated from the xeroxparcarchive contents
and maps every file name to its archive URL. Grep it to locate things.

---

## 1. Dorado emulator and microcode binaries

Ether-bootable (`.eb`) and packed-binary (`.pb`) microcode worlds — these
are the binaries the Dorado's Initial microcode pulls over Ethernet. Use
them as **input to our emulator** once we can load MB-format data.

| URL | What it is |
|---|---|
| https://xeroxalto.computerhistory.org/Io/Murray/.AltoMesaDorado.eb!1.html | Mesa emulator microcode for Dorado (running Alto-Mesa) |
| https://xeroxalto.computerhistory.org/Io/Murray/.SmalltalkDorado.eb!1.html | Smalltalk-80 emulator microcode for Dorado |
| https://xeroxalto.computerhistory.org/Io/Murray/.TestDorado.eb!1.html | Dorado diagnostic/test microcode (memory, IFU tests) |
| https://xeroxalto.computerhistory.org/Io/Murray/.BasicCedarDorado.pb!1.html | Cedar Pilot world for Dorado (~525 KB, microcode + boot file) |
| https://xeroxalto.computerhistory.org/Io/Murray/.Initial.eb!1.html | Initial loader (EtherBoot) — invoked early in the boot chain |
| https://xeroxalto.computerhistory.org/Io/Murray/.BootMeAgain.eb!1.html | Secondary boot helper |

**Sibling machines** (Dolphin/D0 and Dandelion/Star/8010) — same MicroD
.MB format, useful as cross-references:

| URL | What it is |
|---|---|
| https://xeroxalto.computerhistory.org/Io/Murray/.AlphaMesaPilotD0.eb!1.html | Mesa/Pilot for D0 (Dolphin) |
| https://xeroxalto.computerhistory.org/Io/Murray/.AlphaMesaOthelloD0.pb!1.html | Mesa Othello (Pilot debugger) for D0 |
| https://xeroxalto.computerhistory.org/Io/Murray/.PilotD0.eb!1.html | Pilot OS for D0 |
| https://xeroxalto.computerhistory.org/Io/Murray/.AltoD0.eb!1.html | Alto-emulator microcode running on D0 |
| https://xeroxalto.computerhistory.org/Io/Murray/.OthelloD0.pb!1.html | Othello debugger world for D0 |
| https://xeroxalto.computerhistory.org/Io/Murray/.BasicCedarD0.pb!1.html | Cedar for D0 |
| https://xeroxalto.computerhistory.org/Io/Murray/.BasicCedarDLion.pb!1.html | Cedar for Dandelion (Star/8010) |
| https://xeroxalto.computerhistory.org/Io/Murray/.OscarDicentra.pb!1.html | Mesa world (Dicentra release) |

## 2. MicroD — the microassembler post-processor

MicroD is the placer/linker that takes the output of the "Micro"
microassembler (`.DIB` files) and produces a `.MB` (micro-binary) file.
**We will need to read MicroD's source to understand the .MB format
exactly.**

| URL | What it is |
|---|---|
| https://xeroxalto.computerhistory.org/Indigo/AltoSource/.MICROD.DM!2.html | Packed `.dm` archive of MicroD BCPL source |
| https://xeroxalto.computerhistory.org/Indigo/AltoSource/MICROD.DM!2_/.index.html | Same, expanded into individual files (`mdmain`, `mdassign`, `mdplace`, `mdlink`, `mdload`, `mdscan`, `mddump`, `mdfixup`, `mdcheck`, `mdmake`, `mdlist0..2`, `mdinit`, `mdmisc`, `mderr`, `mdout`, `printmb.bcpl`, `mdadefs/mddecl/mdfields.d`, `mdasm.asm`, `loadoverlay.bcpl`, `easyjunta.bcpl`, plus `compilemicrod.cm`/`loadmicrod.cm`/`sendmicrod.cm`) |
| https://xeroxalto.computerhistory.org/_cd8_/alto/.microd.run!2.html | Built MicroD executable (Alto binary) |
| https://xeroxalto.computerhistory.org/_cd8_/alto/.microd.syms!2.html | Symbol table for `microd.run` |
| https://xeroxalto.computerhistory.org/_cd8_/altodocs/.microd.tty!2.html | MicroD user manual (Aug 1978 / Sep 1979) — describes the `.DIB`→`.MB` flow and switches |

The MicroD user manual is the most important document on this list.
Read `printmb.bcpl` from the source archive for the canonical .MB
serializer.

## 3. SIL / Analyze / Build / Route / Gobble — design-automation

These are the Alto-era CAD tools that produced the Dorado schematics
(SIL is a schematic-capture format; the `.press` PDFs in this repo are
SIL output). Useful if we ever need to re-process the schematics.

| URL | What it is |
|---|---|
| https://xeroxalto.computerhistory.org/_cd6_/sil/.index.html | Top-level directory |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.doradodesignautomation.cm!1.html | Recipe for the Dorado-specific build/route templates |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.silmanual.press!1.pdf | SIL manual |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.silsources.dm!1.html | SIL source archive |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.sillibraries.dm!1.html | SIL symbol libraries |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.ecldatasheets.press!1.pdf | ECL parts datasheets used by the Dorado |
| https://xeroxalto.computerhistory.org/_cd6_/sil/.ttldatasheets.press!1.pdf | TTL parts datasheets |

Dorado board route binaries (output of the Route tool):

- `routemlb.br!1` — memory logic board
- `routestor.br!1` (+ holes / press) — storage board
- `routedc.br!1` — data cache / display
- `routed1bp.br!1` — D1 backplane
- `routesb.br!1`, `routepm.br!1`, `routeip.br!1`, `routemwdc.br!1`,
  `routeaub.br!2` — other Dorado boards
- `routemanual.press!1.pdf` — Route manual
- `routeemboards.dm!1`, `routetcboards.dm!1` — board-set archives

## 4. Cross-reference hints to additional Dorado directories

The local file `DoradoDocs/manuals/a` (a fragment of the
cross-reference HTML) shows pointers into directories that the
sub-agent's broad sweep didn't pick up. These exist on the archive
even if the agent could not enumerate them via `index.html` browsing:

- `[_CD8_]<dorado>` — appears to hold `doradobaserom.dm!12_` and
  `UnBug.bfs!1_`. Worth probing for further BaseBoard sources.
- `[Indigo]<Dorado>` — appears to hold `doradobaserom.dm!11_`.
- `[IO]<DoradoLogic>Debug>` — appears to hold `DoradoBaseDebug.dm!1_`.

URL probes to try next session:

- https://xeroxalto.computerhistory.org/_cd8_/dorado/.index.html
- https://xeroxalto.computerhistory.org/indigo/dorado/.index.html
- https://xeroxalto.computerhistory.org/io/doradologic/.index.html

## 5. Where the canonical Dorado source lives

The xeroxparcarchive server has the full Dorado source tree. Every
file referenced in the cross-reference HTML for the following paths
has been confirmed to resolve:

| Server path | Contents |
|---|---|
| `xeroxparcarchive…/_cd8_/dorado/` | **The canonical Dorado tree.** All current `.mb` microcode (Mesa, Cedar, AEmu, DSemu, TriEmu, TriMesa), all `Initial*.eb` boot stages, all `.dm` source archives (bootstrap, doradobaserom, Ifu, kernel, MEMA, memMisc, Tricond, eventCounters, edp, ftest, diex, DSemu, bootEmu, DoradoMidasRun), `LoadMB.run`/`ReadMB.run`, plus `test/` subdir. |
| `xeroxparcarchive…/_cd8_/doradomicrocode/` | **Partially mirrored locally in `chm/doradomicrocode/`.** Separate Dorado microcode source tree with subdirs `DoradoMicrocodeSources`, `KernelSources`, `MicroD`, `Micro`, `Top`, `Code`, `Initial`, `BlockOps`, `CedarDual3`, `EtherTester`, `Documentation`, etc. The local subset currently includes the display/disk/memory sources needed for bring-up (`ADefs`, `DisplayDefs`, `DisplayMain`, `DisplayAux`, `LoadRam`, `NewMemory`, `PilotDisk`, `PilotDiskDefs`, plus Mesa build recipes). |
| `xeroxparcarchive…/_cd8_/dorado/test/` | `Cedar.mb!4`, `Cedar.midas!3`, `TestDorado.eb!5`, `TestW.mb!1`, `TestW.midas!1`. |
| `xeroxparcarchive…/indigo/dorado/` | Older versions of every file in `_cd8_/dorado/`. Skip unless you specifically need a previous revision. |
| `xeroxparcarchive…/io/doradologic/` | Schematic-source `.dm` archives for every board revision (BaseBd, ContA/B, DispM, DispY, DskEth, IFU, MemC/D/X, ProcH/L, MSA, IOTest, etc.) plus `DoradoFiles.dm`, `DoradoBuildFiles.dm`, `DoradoProms.run`, `RevStatus.memo`. |
| `xeroxparcarchive…/io/doradologic/debug/` | `DoradoBaseDebug.dm!1` — BaseBoard debug ROM source. |
| `xeroxparcarchive…/io/doradologic/aux/`, `…/displayboard/` | Aux and display-board sub-archives (indexes captured locally). |

Most of the above has been pulled into `chm/` — see §7 for the local
inventory.

Things still **not** found on either CHM server:

- **`Midas`** itself (the Alto-side debugger client that drives the
  Dorado). The `DoradoMidasRun.dm` archive contains `midas.run`,
  `midas.midas` and friends — that is the Dorado-side runtime, not the
  Alto-side debugger UI. The Alto-side Midas would live somewhere
  under a Midas-specific directory; we haven't located it.
- **MBtoBase** (the `.mb` → BaseBoard-EPROM converter, referenced by
  `getNewDoradoUCode.cm` from the BaseBoard build chain). Not on CHM
  servers; would have to be reconstructed from `printmb.bcpl` and the
  BaseBoard ROM build scripts. The mb-to-base intermediate appears
  inside `bootEmu.dm` as `mbtobase.run` (we have it).
- **APNew** (EPROM programmer driver). Not located.

These remaining gaps don't block emulator work — we have the binary
EPROM dumps locally already (`firmware/`), and the live
toolchain isn't required to *run* microcode, only to *build* fresh
microcode from sources.

### Cross-references for the truly missing things

- **`[_CD8_]<DoradoDocs>DoradoB-WEverything.dm!1_>`** — contains the
  CSL-81-1 Dorado paper bundle (`DoradoB-W.Press`) plus separate
  processor, IFU, and memory-system papers/figures from January 1981.
  These are useful for CPU/IFU/memory timing (`Hold`, task switching,
  cache/map/storage pipeline, FastIn/FastOut behavior), but they are
  not disk-controller references. Disk-specific details remain in HM
  §9, DskEth drawings, and `DiskFormat.txt!1`.
- **Bitsavers** (`bitsavers.org/pdf/xerox/parc/`) — has separate
  scanned Dorado documents; use as a cross-check for PDF quality and
  alternate scans.
- **Al Kossow's archives** — sometimes have PARC tapes that didn't
  make any public mirror.
- **CHM directly** — internal catalogues hold more than the public
  side; ask if a specific tool turns out to be load-bearing.

## 6. How to use this archive day-to-day

- For **microcode binaries**: already pulled, see `chm/microcode/`. Strip
  the EB overhead per HM Booting memo §LoadRam, then load into IM.
- For **microcode behavior reference**: already pulled, see `chm/microd/`.
  Start with `mdfields.d` (field bit-positions) and `printmb.bcpl` (MB
  serializer); cross-check against `chm/microd-manual/microd.tty!2`.
- For **filename lookup**: grep the local
  `chm/cross-reference.html`. Don't try to read it whole.
- For **schematics**: prefer the local PDFs in `DoradoDocs/schematics/`
  and `DoradoDocs/doradodrawings/`. The CHM SIL route binaries are
  binary blobs — only useful if we want to re-render schematics.
- For **URL conventions and curl recipes**: see `chm-urls.md`.

## 7. What we have locally now

After the April 2026 sync, `chm/` contains 339 files / ~12 MB:

### `chm/microcode/` (from `xeroxalto…/Io/Murray/`, 17 files, ~2.7 MB)

Pre-packaged Ether-bootable microcode worlds:
- **Dorado**: `AltoMesaDorado.eb!1` (35 KB), `SmalltalkDorado.eb!1`,
  `TestDorado.eb!1`, `BasicCedarDorado.pb!1` (513 KB),
  `Initial.eb!1`, `BootMeAgain.eb!1`, `Fiala.eb!1`, `Murray.eb!1`.
- **D0 / Dolphin** (sibling): `AltoD0.eb!1`, `AlphaMesaPilotD0.eb!1`,
  `AlphaMesaOthelloD0.pb!1`, `BasicCedarD0.pb!1`, `PilotD0.eb!1`,
  `OthelloD0.pb!1`, `D0.eg!1`.
- **DLion / Dandelion**: `BasicCedarDLion.pb!1`.
- **Dicentra**: `OscarDicentra.pb!1`.

### `chm/microd/` (from `xeroxalto…/Indigo/AltoSource/MICROD.DM!2_/`, 31 files, ~125 KB)

Full MicroD BCPL source. Key files:
- `mdfields.d` — bit-field positions in the .MB binary ★
- `mddecl.d`, `mdadefs.d` — supporting declarations
- `printmb.bcpl` — canonical .MB serializer ★
- `mdmain.bcpl`, `mdscan.bcpl`, `mdout.bcpl`, `mdassign.bcpl`,
  `mdplace.bcpl`, `mdload.bcpl`/`mdload0.bcpl`/`mdload1.bcpl`,
  `mdlink.bcpl`, `mdfixup.bcpl`, `mdcheck.bcpl`, `mddump.bcpl`,
  `mderr.bcpl`, `mdinit.bcpl`, `mdmake.bcpl`, `mdmisc.bcpl`,
  `mdaring.bcpl`, `mdalist.bcpl`, `mdlist0/1/2.bcpl`,
  `easyjunta.bcpl`, `loadoverlay.bcpl`, `mdasm.asm`,
  `OccupiedDefs.mc`, `compilemicrod.cm`, `loadmicrod.cm`,
  `sendmicrod.cm`.

### `chm/microd-manual/` (3 files, ~80 KB)

`microd.tty!2` (4.6 KB user manual), `microd.run!2` (Alto exec),
`microd.syms!2`.

### `chm/dorado/` ★ (from `xeroxparcarchive…/_cd8_/dorado/`, 41 top + 15 raw archives + 5 in test/, ~7.7 MB)

The canonical Dorado source tree.

**Microcode binaries (.mb)**: ★
- `Mesa.mb!3` (71 KB) — the Mesa emulator.
- `Cedar.mb!6` (66 KB) — Cedar emulator.
- `AEmu.mb!2` (40 KB) — Alto emulator (smallest, easiest first target).
- `DSemu.mb!1` (64 KB) — Smalltalk emulator.
- `TriEmu.mb!3` (44 KB) — Trident-aware emulator.
- `TriMesa.mb!3` (65 KB) — Trident + Mesa emulator combined.
- `doradobaserom.mb!13` (49 KB) — BaseBoard ROM, MicroD output form.

**Midas scripts (.midas)** — load instructions for each `.mb`:
- `Mesa.midas!4`, `Cedar.midas!7`, `AEmu.midas!2`,
  `Mesa.midas!4` plus `watchdog.midas!1`.

**Ether-bootable wrappings (.eb)**:
- `AltoMesaDorado.eb!2`, `AltoD1MC.eb!3`, `CedarDorado.eb!6`,
  `InitialAltoMesaDorado.eb!3`, `InitialDiskDorado.eb!3`,
  `InitialEtherAltoMesaDorado.eb!3`, `InitialEtherCedarDorado.eb!3`,
  `InitialEtherTestDorado.eb!3`.

**Tools (Alto `.run` + `.syms`)**:
- `LoadMB.run!6` ★ — loads a `.mb` into Dorado IM via the BaseBoard.
- `ReadMB.run!1` ★ — reads / dumps a `.mb` (could be a useful
  reference disassembler in working code).
- `pcprint.run!5`, `MakeCardImages.run!1`, `Resist.run!15`,
  `HighResist.run!1`.

### `_cd8_/doradomicrocode/` (remote; focused subset in `chm/doradomicrocode/`)

Top-level index:
`https://xeroxparcarchive.computerhistory.org/_cd8_/doradomicrocode/.index.html`

This is a separate archive tree from `chm/dorado/`. A quick index
survey on 30-Apr-2026 found these subdirectories: `7.0`, `BlockOps`,
`CedarDual3`, `Code`, `Cpa`, `Documentation`,
`DoradoMicrocodeSources`, `EtherTester`, `Initial`, `KernelSources`,
`LoadAltoDumpFile`, `LoadMB`, `Micro`, `MicroD`, and `Top`.

The critical pieces are in `DoradoMicrocodeSources` and `KernelSources`.
They contain source-level modules for the loaded Pilot/Cedar/Mesa
microcode path, including display, disk, memory startup, and `LoadRam`.
A focused local subset was downloaded on 30-Apr-2026:

- `ADefs.mc!3`
- `DisplayDefs.mc!1`
- `DisplayMain.mc!1`
- `DisplayAux.mc!1`
- `LoadRam.mc!1`
- `NewMemory.mc!1`
- `PilotDisk.mc!1`
- `PilotDiskDefs.mc!1`
- `AltoMesa.cm!1`
- `CedarMesa10MB.cm!1`

This is not a full mirror. When debugging post-LoadRam Mesa behavior,
prefer these sources over trying to infer everything from `.mb`
disassembly, and fetch additional files from the same archive as needed.

### `chm/doradosource/` (from `xeroxparcarchive…/_cd8_/doradosource/`, partial)

`AemuSources.dm!82_` is the 1984 Alto-emulator source archive matching
the `AEmu.mb!2` / `AltoMesaDorado.eb` era more closely than the later
`doradomicrocode` tree. A focused subset is mirrored locally:

- `ADefs.mc`
- `AEm0.mc`
- `EMemDefs.mc`
- `InitMem.mc`
- `RegisterDefs.mc`
- `S-Group.mc`
- `Start.mc`

Use these when decoding post-LoadRam `Initial`/AEmu paths such as
`ResumeEmulator`, `SetupBRs`, `Start`, `AEmuNext`, and Alto keyboard
boot selection. Fetch additional members from the same expanded archive
as needed.

**BCPL compiled (.bcd)**:
- `MicroSample.bcd!2`, `MicroSampleDefs.bcd!1`, `SetClock.bcd!1`,
  `AlarmTest.bcd!1`, `CorrectMwWl.bcd!1`.

**Command files (.cm)**:
- `getdiagnostics.cm!1`, `doradomesadisk.cm!4`,
  `DoradoUnbugDisk.cm!7`, `UpdateEmulators.cm!12`.

**Dump-format archives — raw .dm files** (in `chm/dorado/`) **and
expanded** (in `chm/dorado/expanded/`):
- `bootstrap.dm!20` → `Bootstrap.mb` (2 KB), `Initial.mb` (17 KB) — ★
  the actual Bootstrap and Initial microcode; ~50 and ~700 instructions
  per the booting memo.
- `doradobaserom.dm!12` → BaseBoard ROM sources: `doradoboot.masm`,
  `doradocontinuous.masm`, `doradocpint.masm`, `doradoio.mdefs`,
  `doradomidashooks.masm`, `doradomidasint.masm`, `doradomufman.masm`,
  `doradoreset.masm`, `getnewdoradoucode.cm`, `mbtobase.run`. ★
- `Ifu.dm!51` → `IfuComplex.mb` (60 KB), `IfuSimple.mb` (60 KB),
  `IfuComplex.midas`, `IfuSimple.midas`, `showIfu.midas`,
  `showIfuChaos.midas` — IFU microcode test/diagnostic worlds.
- `kernel.dm!38` → `kernel.mb`, `kernel.midas` — Mesa kernel.
- `MEMA.DM!18` → `memA.mb` + 8 midas show-scripts (showap, showC,
  showChaos, showD, showS, showScheckout, showX). Memory diagnostics.
- `memMisc.dm!11` → `memMisc.mb`, `showFIO.midas`,
  `showPipeFaults.midas` — more memory diagnostics.
- `Tricond.dm!5` → `TriconD.mb`, `TriconD.midas` — Trident
  controller microcode.
- `eventCounters.dm!5` → `eventCounters.mb`, `showeventCounters.midas`.
- `edp.dm!3` → `edp.mb`, `edp.midas`.
- `ftest.dm!1` → `ftest.MB` + `ftest.MC` (Micro source!), `ftest.DIB`,
  `ftest.DLS` (MicroD listing!), `ftest.ER`, `ftest.cm`, `ftest.midas`.
  **The DIB/DLS/MC trio is a complete worked example of the Micro →
  MicroD → load pipeline. Read this first when implementing the
  decoder.** ★
- `DSemu.dm!17` → `DSemu.mb`, `Smalltalk.midas`.
- `diex.dm!2` → diagnostic worlds.
- `bootEmu.dm!3` (129 KB) → `mbtobase.run`, several Boot-related
  midas scripts.
- `DoradoMidasRun.dm!56` (240 KB) → `midas.run`, `midas.syms`,
  `midas.midas`, `loader.mb`, plus debug/normal/poke/special/svcrash/
  simdump/tests midas scripts. The Dorado-side debugger runtime.
- `UnBug.bfs!1` (2.3 MB) → 117 files. **Includes `DoradoLisp.MB`
  (Lisp microcode — only place we've found Lisp), `Cedar.mb`,
  `Mesa.mb`, `dsemu.mb`, `Ifu.mb`, `IfuComplex.mb`, `IfuSimple.mb`,
  `kernel.mb`, `memA.mb`, `memMisc.mb`, `TriconD.mb`, `Keytest.mb`,
  `testvco.mb`, `eventCounters.mb`, `edp.mb`, `ftest.mb`, plus
  `doradobaserom.mb` (90 KB version, larger than top-level mb!13)**,
  many show-scripts, plus a complete Bravo + Executive + Swat + Sys.Boot
  Alto disk environment. Substantial pile of midas Scope* test scripts
  for ALU/Carry/Bypass diagnostics.

### `chm/dorado/test/` (from `…/_cd8_/dorado/test/`, 5 files)

`Cedar.mb!4`, `Cedar.midas!3`, `TestDorado.eb!5`, `TestW.mb!1`,
`TestW.midas!1`.

### `chm/io-doradologic/` (from `xeroxparcarchive…/io/doradologic/`, 14 files + indexes)

Schematic-source archives for the full Dorado mainframe, captured as
high-value items only — the per-board SIL `.dm` archives (BaseBd,
ContA/B, DispM/Y, DskEth, IFU, MemC/D/X, ProcH/L, MSA) were **not**
pulled because we already have the equivalent PDFs. Pulled:
- `DoradoBaseDebug.dm!1` (45 KB) — BaseBoard debug ROM.
- `DoradoFiles.dm!1` (340 KB) — manifest archive.
- `DoradoBuildFiles.dm!1` (411 KB) — build manifest archive.
- `DoradoProms.run!3` (13 KB) — PROM programmer.
- `D1LogicArchive.dir!1` (28 KB) — directory listing of D1 logic.
- `RevStatus.memo!1`, `RevStatus.bravo!1` — board revision status.
- `wirelengthanalysis.mesa!1` (Mesa source), `wirelengthanalysis.bcd!1`.
- `doradoasifiles.df!1`, `doradodisk.cm!1`, `DoradoFiles.cm!1`,
  `BuildBackupTemplate.cm!1`, `PCBoardSpec.bravo!1`.
- Saved indexes: top-level + `aux`, `debug`, `displayboard` subdirs.

### `chm/indigo-dorado/` (one file)

Just the directory index, saved for reference. Contents are mostly
older versions of files we already have; not pulled in bulk.

## 8. Phase-0 archive survey (2026-04-30)

Full re-walk of the `xeroxparcarchive` `_cd8_/` tree to feed
`docs/research-plan.md`. Earlier sections of this doc undercounted
several Dorado branches; this section is the authoritative inventory
of Dorado-relevant content on the live server, annotated with the
gap IDs (per `handoff.md`) each artifact serves.

### Top-level branches in `_cd8_/`

```
alto/  altodocs/  basicdisks/  bootfiles/  cedardocs/  cedarlib/
dicentra/  dls/  dorado/  doradodocs/  doradomicrocode/  doradosource/
grapevine/  ifs/  indigogv/  indigoifs/  juno/  laurel/  mockingbird/
portola/  press/  printingdocs/  pup/  pupgateway/  solidviews/
+ cd_copy / cd_link tools (not relevant)
```

Dorado-relevant branches: `dorado/` (canon), `doradodocs/` (papers),
`doradomicrocode/` (alt-organised source tree), `doradosource/` (the
broader source archive). `pup/`, `pupgateway/`, `bootfiles/` may also
matter for **H1 Ethernet** boot bring-up.

### `_cd8_/doradodocs/` ★ — **previously unsurveyed**

42 entries. PRESS-format documents (Xerox internal printable form);
plan to convert each to PDF locally.

| File | Gap | Notes |
|---|---|---|
| `DoradoHardwareManual.press!1` (667 KB) | all | Same source as our HM PDF — keep as cross-check. |
| `DoradoManual-A.press!8`, `-B.press!8`, `-Figs.press!8` | all | The 1981 manual we already use, in original form. |
| `DoradoBooting.press!7` (33 KB) | A1, A2, H1 | Newer (1984) booting memo — supersedes the 1980 memo we have. |
| `DoradoBootingImpl.press!1` (26 KB) | A1, A2, H1 | Boot **implementation** memo (1983) — likely the BB Boot0/Boot1 protocol detail. |
| `DoradoDebugging.press!2` (73 KB) | B5, B11 | Debugging facilities — Read IM, breakpoints, parity, perf counters. |
| `DoradoMidas.press!15` (280 KB) | B5, B11 | Midas debugger. |
| `MidasInternal.press!8` (168 KB) | B5, B11 | Midas internals. |
| `GarageMidasManual.Press!2` (159 KB), `GarageMidasListings.Press!2` (177 KB) | B5, B11 | Newer Midas variant. |
| `Kernel.Press!6` (278 KB) | B11, C1, C3 | Kernel microcode listings — fault handling ground truth. |
| `MakeKernelListings.cm!5` | B11 | Build script for Kernel.Press. |
| `DoradoMicroAssembler.Press!8` (216 KB) | J2 | Micro-assembler manual. |
| `MicroSample.memo!1` (7 KB) | J2 | Tiny example for `.mc` source format. |
| `OpcodeTimings.press!1` (20 KB), `opcodeTimings.bravo!2` | B1, K1 | Opcode timings — ground truth for Hold + cycle accuracy. |
| `ExtendedOpcodes.press!3` (19 KB) | B9 | Extended opcode set. |
| `DiskFormat.txt!1` (12 KB) | F1, F6 | **Disk format specification** — sequence-PROM context. |
| `Mc10139LABLES.SIL!3`, `Mc10149PROMLABLES.SIL!4` | F1 | **PROM label data** — likely the read/sequence PROM source. |
| `DougsTimingDiagrams.dm!1`, `.press!1` | B1, B9, K1 | **Timing diagrams** — load-bearing for Hold and IFU pipeline. |
| `DispMwakeupwiring.bravo!2` (241 B) | E5 | Display-monitor wakeup wiring note. |
| `D1DocsMail.dm!1` (382 KB) | (D1) | Dolphin docs — only relevant if model-0 .MB ever matters (J1). |
| `DoradoB-W.Press!1` (398 KB), `DoradoB-WCover.Press!1`, `DoradoB-WEverything.dm!1` (1.6 MB) | all | CSL-81-1 Dorado paper bundle (already noted in §5). |
| `BoardTester.press!7`, `BuildStandards.press!6`, `pcBoardSpec.press!2` | (none for emu) | Manufacturing docs. |
| `Archive.Directory!2`, `RevStatus.press!4`, `SustainingEngineers.press!2`, `Microcode.mail!1`, `Dorado.mail!1`, `bootDorado.press!1`, `DoradoUserOps.press!2`, `baseboardloadingchart.sil!1` | misc | Ancillary; not load-bearing. |

### `_cd8_/doradomicrocode/` — partially surveyed

Subdirectories (no top-level files):

```
7.0/  BlockOps/  CedarDual3/  Code/  Cpa/  Documentation/
DoradoMicrocodeSources/  EtherTester/  Initial/  KernelSources/
LoadAltoDumpFile/  LoadMB/  Micro/  MicroD/  Top/
```

The local mirror has a focused subset of `DoradoMicrocodeSources/`
already. For our gaps:

- **`EtherTester/`** ★ — **H1**. Not pulled. Spider next.
- **`Initial/`** ★ — A1, A2. Not pulled. Should contain the source
  for `Initial.mb` separate from `BootstrapSources.dm`.
- **`KernelSources/`** ★ — B11, C1, C3, C4, C5. Need full pull.
- **`LoadMB/`**, **`LoadAltoDumpFile/`** — B5 (Read IM context).
- **`Documentation/`** — likely contains Mesa/Cedar driver docs.
- **`BlockOps/`**, **`CedarDual3/`**, **`Cpa/`**, **`Top/`**, **`Code/`** — examine on demand.

### `_cd8_/doradosource/` — partially surveyed

40 sub-archives, 62 top-level files. Highest-value for our gaps:

| File / dir | Gap | Notes |
|---|---|---|
| **`DoradoProms.dm!14`** (54 KB) | F1, F6 | ★ **Read/sequence-PROM dumps** — the canonical artifact for disk Phase 3. Pull and decode early. |
| **`MEMASOURCE.DM!15`** (105 KB) | B1, C1, C2, C3, C6 | ★ Main-memory microcode source. Hold/Pipe/MCR ground truth. |
| **`memMiscSource.dm!9`** (76 KB) | C1, C3, C4, C5 | Memory helper microcode. We have a partial. |
| **`kernelSources.dm!40`** (170 KB) | B11, C4, C5 | Kernel source — fault handlers. We have a partial. |
| **`BootstrapSources.dm!12`** (64 KB) | A1 | We have it locally. |
| **`IfuSources.dm!53`** (277 KB) | B9 | We have it locally. |
| **`MidasSources.dm!24`** (245 KB), **`MidasDoradoSources.dm!27`** (496 KB), **`MidasInternal.Dm!4`** (134 KB) | B5, B11 | Midas/DoradoDebug source. Pull for Read IM + breakpoints. |
| **`ReadMB.dm!1`** (11 KB) | B5 | ReadMB source — direct Read IM example. |
| **`LoadMB.dm!6`** (22 KB) | B5 | LoadMB source — IM loader microcode. |
| **`eventCountersSources.dm!5`** (174 KB) | B11 | Event-counter (perf-counter) microcode. |
| **`diagnosticsubrs.dm!11`** (177 KB), `diagnosticListings/` | many | Diagnostics — exercises every sub-system. |
| **`emuEvents.dm!1`** (14 KB) | B11 | Emulator events. |
| `DoradoProms.dm!14`, `DoradoMicroAssembler.Dm!3`, `DoradoMidasManual.dm!10`, `MicroMemo.Dm!2` | J2, others | Tooling sources. |
| `D1Lang.Mc!14`, `D1Alu.Mc!3`, `D1AssemblerTest.Dm!2`, `D1ALTOMCSOURCE.DM!1`, `D1THEORY.DM!1` | J1 | Dolphin (D1/model-0) — only matters if we revive the model-0 .MB path. |
| `TriEmu.mb`-related: `Tricond.dm`, `TriDiskSources.dm!8`, `TriconD-Diagnostics.dm!8` | (Tricon, separate) | Trident-disk-specific microcode for the Tricon controller (different from regular Trident). |
| `Resist.dm!15`, `HighResist.dm!1`, `Ftestsource.dm!2`, `setClock.mesa!1`, `pcprint.dm!4` | misc | Tooling / tests. |

### `_cd8_/dorado/` — corrections to old §5

Several items the old `chm-urls.md` flagged as "not on the live
server" **are** on the live server:

- `bootstrap.dm!20` (20 KB) — present.
- `doradobaserom.dm!12` (84 KB) — present.
- `Ifu.dm!51` (128 KB) — present.
- `kernel.dm!38` (61 KB) — present.
- `MEMA.DM!18` (67 KB) — present.
- `memMisc.dm!11` (55 KB) — present.
- `Mesa.mb!3`, `Cedar.mb!6`, `AEmu.mb!2`, `DSemu.mb!1`, `TriEmu.mb!3`, `TriMesa.mb!3` — all present and pulled into `chm/dorado/`.

The original "not fetchable" claim was mistaken; `chm-urls.md` should
be amended.

New finds in `_cd8_/dorado/` not in old §7:

- `bootEmu.dm!3` (129 KB) — boot-emulator microcode. Possibly
  relevant for Path A boot.
- `diex.dm!2` (51 KB), `edp.dm!3` (40 KB) — diagnostic/debug
  archives.
- `DoradoMidasRun.dm!56` (239 KB) — Dorado-side Midas runtime.
- `eventCounters.dm!5` (50 KB) — built event-counter microcode.

### What's still missing

After this re-walk:

- **Alto-side Midas** UI (we have the Dorado-side runtime). Not
  load-bearing for emu correctness.
- **MBtoBase**, **APNew** — toolchain only.
- **Trident pack** containing Initial's "page 4" private microcode
  (A3). Not located here; check `bootfiles/`, `dicentra/` next pass.

### Prioritized fetch list (start with these)

The gaps drive the order. Each line: target dir + file → the gap it
unblocks.

1. `doradodocs/Mc10139LABLES.SIL!3`, `Mc10149PROMLABLES.SIL!4` → F1
2. `doradodocs/DiskFormat.txt!1` → F1, F6
3. `doradosource/DoradoProms.dm!14` → F1, F6
4. `doradosource/MEMASOURCE.DM!15` → B1, C1–C6
5. `doradodocs/DoradoBootingImpl.press!1` → A1, A2
6. `doradodocs/DoradoDebugging.press!2`, `DoradoMidas.press!15` → B5, B11
7. `doradosource/ReadMB.dm!1`, `MidasSources.dm!24` → B5
8. `doradodocs/Kernel.Press!6` → B11, C4, C5
9. `doradodocs/DougsTimingDiagrams.press!1`, `OpcodeTimings.press!1` → B1, K1
10. `doradomicrocode/EtherTester/` (full pull) → H1
11. `doradomicrocode/Initial/` (full pull) → A1, A2
12. `doradomicrocode/KernelSources/` (full pull) → B11, C-series
13. `doradosource/eventCountersSources.dm!5` → B11

PRESS-format files are not directly readable; we'll need either a
local Press-to-PDF converter (likely a Java/Smalltalk thing) or to
locate equivalents on Bitsavers. The HM is already on hand as PDF
because someone already converted; chase that route for the others
before pulling raw PRESS bytes.

### Cross-reference grep (Phase 0b)

Hit counts in `chm/cross-reference.html` (45 MB index, 568K lines):

```
   527  ECC      (mostly false-positive: Eccentric, Eros, …)
    65  Hold     (some Dorado timing memos)
   374  DDC      (Cedar dictionary-mostly false-positive)
   201  Trident
  1954  Midas
   128  EtherBoot
    11  DoradoDebug
     0  MCPBus / Read.PROM / Sequence.PROM / Boot1 / Boot0 / FireCode
```

The keyword search exposed a few previously unseen archives:

- **`indigo/doradosource/doradodebugging.dm!1_/`** — Press source of
  the DoradoDebugging document (Press-format archive).
- **`indigo/daffodil/ibip/doc/ibip-bus-ifu-clockhold.dm!1_/`** —
  IBIP-era doc on bus / IFU / clock / Hold timing. **High value for B1.**
- **`io/doradologic/debug/doradobasedebug.dm!1_/`** — BaseBoard debug
  ROM source (separate from `doradobaserom.dm`).
- **`_cd8_/alto/etherboot.dm!1_/`** — Alto-side EtherBoot server source.
  Pup boot protocol implementation. **High value for H1.**
- **`_cd8_/pup/`** — Pup protocol stack. Use for the EtherBoot
  packet format that the Dorado Initial speaks.

### `_cd8_/doradomicrocode/` subdirectories — sub-survey

| Subdir | Files | Notes |
|---|---|---|
| `KernelSources/` | 20 | ★ `Kernel1.mc` … `Kernel5.mc`, `KernelALU.mc`, `Preamble.mc`, `Postamble.mc`. Ground truth for B11 breakpoints, fault handling (C4/C5), and ALUFM init. |
| `Initial/` | 1 | Just `InitialSelect.mb!1` (11 KB) — possibly the current Initial the BB ROM streams. |
| `Documentation/` | 5 | `CedarBRAssignments.tioga!1`, `CedarRMAssignments.tioga!1`, `NewCedar*.tioga!2`, `EtherTesterDoc.tioga!1`. ★ For I1 (Mesa state planting), the BR/RM assignments are the layout we need. |
| `EtherTester/` | 34 | All Mesa-side test programs (`EtherTesterMainImpl.mesa!1`, `NewEthernetHeadDorado.mesa!1`, `NSCedar.eb!1`, etc). Mesa drivers, not microcode — useful as a reference for what Mesa expects of the Ethernet controller, but not the µcode itself. |

The other `doradomicrocode/` subdirs (`7.0/`, `BlockOps/`, `CedarDual3/`,
`Code/`, `Cpa/`, `LoadAltoDumpFile/`, `LoadMB/`, `Micro/`, `MicroD/`,
`Top/`) remain unsurveyed — pull on demand.
