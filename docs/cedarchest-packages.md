# CedarChest 6.1 — what exists, what we have, what is worth having

CedarChest is the applications library that shipped alongside the Cedar 6.1
release: PARC's own tools, games, editors and network utilities, each as a
`.df` in `[Cedar]<CedarChest6.1>Top>`. The archive index lists **316
packages**; the served tree in `chm/cedar/stp-root/CedarChest6.1/` currently
carries **20** of them.

Written 2026-07-31, from
`https://xeroxparcarchive.computerhistory.org/cyan/cedarchest6.1/top/.index.html`
and `chm/cross-reference.html`.

**A caveat on the descriptions below.** Almost no CedarChest `.df` carries a
header comment — most begin with nothing but their own filename. Of ~55
sampled, only three described themselves (`Finger.df: a database storing
information on people and machines`; `Nut.df: the "glue" allowing different
database applications to communicate`; `Summoner.df: the principal .df file
for the Compute Server`). Everything else here is inferred from the package
name and the Cedar literature, and is flagged where the inference is weak.
The categories are mine, not Xerox's.

---

## 1. The headline: the Dorado's schematics survive in editable vector form

The machine already draws its own schematic — but as a **raster**, converted
from the scanned PDFs by `tools/pbm2ais.py` and displayed in AISViewer
(`docs/images/cedar-ais-proch-bitslice07-2026-07-18.png`). That is a picture
of a drawing.

The **original drawings still exist**, in the vector format they were drawn
in: **506 distinct `.sil` files across 72 `.dm` archives** under
`io/doradologic/` in the PARC archive — the complete Dorado logic set, board
by board and revision by revision:

| archive (one revision each) | sheets |
|---|---|
| `dsketh-{apcRev-Da,apcRev-Db,mwRev-Cd,mwRev-Ce,mwRev-Cf,Rev-Ce,Rev-Cf}.dm` | 48–49 |
| `conta-{apcRev-Da,mwRev-Cd,Rev-Cd}.dm` | 34 |
| `procl-{mwRev-Ci,Rev-Ci,…}.dm` | 34 |
| `proch-{apcRev-Da,mwRev-Ce,Rev-Ce}.dm` | 33 |
| plus MemC (37), MemD (27), MemX (26), IFU (65), BaseBd (17), cables, Midas | |

A single sheet, from the cross-reference:

```
[IO]<DoradoLogic>ProcH-apcRev-Da.dm!1_  ProcH01.sil  6452 bytes
                                        08-Oct-1981 18:15:10 PDT  Clark.pa
```

6,452 bytes — a vector drawing, not a scan. Ed Clark's file, timestamped
during the machine's life.

**And we can already display them.** `Sil.df!5` is mirrored, and it is not
the Alto original: its own first line reads `-- Sil.df  Sil for Cedar6.1`,
last edited by Ken Pier in November 1986. The package ships `Sil.bcd`,
`Sil.load`, `Sil.tip`, `Sil.icons`, the `sil.lb5`–`sil.lb8` **symbol
libraries** (the logic-gate shapes the schematics are drawn from), and
`SilBiScrollers` for scrolling/zooming inside a Cedar viewer.

So the chain is complete in principle:

1. extract the `.sil` sheets from an `io/doradologic/*.dm` archive (the repo
   already does this elsewhere — see `chm/dorado/expanded/bootstrap.dm!20_/`);
2. serve them from `chm/cedar/stp-root/`;
3. `Run Sil` on the desktop and open `ProcH01.sil`.

That would be a strictly better version of the existing demo: the emulated
Dorado displaying its own schematics **in the original editable vector
format, with the symbol library they were drawn with** — zoomable, and
arguably the most on-point thing this project could show.

### 1.1 How far this got (2026-07-31), and the one thing in the way

Tried it. Three of the four unknowns are gone; the fourth stopped it.

**Resolved:**

- **No `.dm` unpacking needed.** The archive serves those archives
  pre-expanded: `io/doradologic/ProcH-apcRev-Da.dm!1_/ProcH01.sil` is a
  direct download. (Note the paths are **case-sensitive** and
  `cross-reference.html` lower-cases them — `proch-apcrev-da...` 404s.)
- **The sheets reach the volume.** All 32 ProcH sheets are staged in
  `CedarChest6.1/DoradoLogic/` with a `DoradoLogic.df` modelled on
  `AISImages.df`, and a cold boot attaches them: `33 files acted upon`,
  each with its true timestamp (`ProcH32.sil ... {08-Oct-81 21:16:07 EDT}`).
- **`Run Sil` IS the right command.** `SilKernelImpl.mesa` has
  `Commander.Register[key: "Sil", proc: StartSilCommand, doc: "Create a
  Window Sil Instance"]`.

**The blocker:**

```
% Run Sil
VersionMismatch[BiScrollers]
```

`Sil.bcd!4` is dated **26-Aug-86**; the BiScrollers implementation
`BiScrollers.df!12` pins is `BiScrollersImpl.BCD!11`, **14-Nov-86**. Sil was
compiled against an older BiScrollers interface than the release ships
beside it — the mixed-vintage problem §4's third trap describes. Everything
below Sil in the chain loads cleanly (Cursory, PopUpSelection2,
PopUpButtons, Geom2D, MJSContainers, TypeProps, Abutters, ViewRec,
BiScrollers ×3, SirPress, ImagerPressFontSubst, ImagerPress,
InterpressPackage); only the last `Run` fails.

Next moves, cheapest first: find whether an August BiScrollersImpl exists in
the archive (the DF pins November, but earlier versions are kept); or a
later `Sil.bcd` than `!4`; or load Sil against the `BiScrollers.BCD!3`
interface (26-Aug-86, the matching vintage) without running the November
impl. This is package archaeology, not emulator work.

**How to load a CedarChest package at cold boot** — worth recording, because
it is the reliable mechanism and it now works for everything up to Sil. Add
to `CommandTool.BootCommands` in
`chm/cedar/stp-root/Cedar6.1/Top/User.Profile` (CR-terminated!), using the
**selective** `Bringover -o <files> <DF>` form that the AIS lines there
already use. `-o` names the files and skips the import closure — a plain
`Bringover [Cedar]<CedarChest6.1>Top>Sil` walks the whole graph and stalls,
which is what §4's Bringover-cost note means in practice. Then `Run` the
implementation modules in dependency order, taking the order from each
package's own `.load` file. Remember the data files: `Run
PopUpSelection2Impl` fails with `Could not find PopUpSelection2.tip` unless
the `.tip` is in the `-o` list.

Remaining caveat: these are 1979–1981 revisions, so the sheet set will not
exactly match the 1979 PDFs in `DoradoDocs/schematics/`.

---

## 2. What we have (20)

Seventeen mirrored from upstream CedarChest 6.1:

`Abutters` · `AIS` · `AISViewer` · `BiScrollers` · `CardTable` ·
`ChessHack` · `Clock` · `Cursory` · **`Gargoyle`** · `ImagerPress` ·
`Interpress` · `PopUpButtons` · `PressReader` · **`Sil`** · `SirPress` ·
`ViewRec` · **`Whiteboard`**

Plus three authored here, which exist in no upstream index: `DoradoWelcome`
(the friendly `.cm` menu and its buttons), `AISImages`, `DoradoArchive`.

Gargoyle (4.7 MB) and Whiteboard (456 KB) were mirrored on 2026-07-31 in
response to "why doesn't `Run Gargoyle` work" — the answer being that it was
simply absent. Neither has been launched yet.

Verified working on the desktop so far: **ChessHack** (plays chess, board
paints in the Chess40 font, offline), **Clock**, **AIS/AISViewer** (displays
the schematic rasters), and the `DoradoWelcome` command files.

---

## 3. What is worth fetching, and why

Ranked by what would actually add something to this project.

### First tier — go get these

| package | why |
|---|---|
| **Griffin** `df!6` | The other PARC illustrator. Between Sil, Gargoyle and Griffin you have the full drawing lineage; useful if Sil chokes on the 1981 files. |
| **DoradoInfo** `df!5` | A CedarChest package *about the Dorado*. Contents unknown and worth 30 seconds to find out — it is the only package in 316 named for this machine. |
| **Documentation** `df!27` | The Cedar documentation set as Tioga documents. `OpenR` already works, so this turns the desktop into a browsable manual. |
| **PupWatch** `df!11` · **EtherWatch** `df!6` | Network monitors that would watch *our* emulated Pup/BSP wire from inside the guest — a genuine cross-check of `src/ethernet.c` against a period tool rather than against our own reading of the spec. |
| **STPServer** `df!3` | Cedar's own STP **server** — the thing `eth_ftp_*` reimplements in C. Running the real one inside the guest is the ultimate conformance test of §2.5's wire shape. |
| **MazeWar** `DF!11` | We already run the *Alto* MazeWar. The Cedar version on the same emulated machine is a nice side-by-side, and the DF says it comes from CedarChest6.0. |

### Second tier — makes the desktop feel like a workstation

`TopButtons` · `DeskTops` · `QuickViewer` · `Magnifier` · `NewCalc` ·
`NewClock` · `SpellingTool` · `StyleTool` · `IconEditor` · `FontEdit` ·
`EditorComforts` · `Remember` · `Lister` · `Grep` · `Compare` · `DiskTool`

Small, self-contained, and each one visibly changes the desktop. `IconEditor`
and `FontEdit` are the most demoable — editing the machine's own icons and
fonts, live.

### Third tier — fun, and cheap to try

`Football` · `PigsInSpace` · `ThreeCasabaFour` · `OneCasaba` ·
`MickeyMouse` · `Celtics` · `RandomDemo` · `VBounce` · `PolyHack` · `Tube` ·
`ColorTrix` · `Cookie` · `GoodTimes`

Caveat: several are probably **colour-display** programs (`ColorTrix`,
`ColorDisplay`, `ColorTool`, `ColorFns`). The Dorado's colour display is not
modelled, so those may not be reachable at all.

### Historically interesting, likely hard

| package | why | why hard |
|---|---|---|
| **JaM** `df!4` | The stack language PARC built before PostScript — Warnock and Geschke's work, the direct ancestor. | Interpreter plus its own imager bindings (`JaMImager`, `JaMIDunn`, `ImagerToJaM`). |
| **Walnut** `df!24` (+7 satellites) | The Cedar mail system, on Grapevine. | Needs a Grapevine server; ours deliberately answers "AllDown". |
| **Peanut** `df!9` | The lighter mail reader. | Same Grapevine dependency. |
| **Chat** `df!2` | Terminal emulator over Pup. | Needs a host to talk to; could be pointed at our own shim. |
| **Alpine\*** (4 packages) | PARC's transactional file server. | Server-side; wants a second machine. |
| **Summoner** `df!30` | "the principal .df file for the Compute Server" — distributed compute. | Wants a network of machines. |
| **TeX** `df!5` + `MF` · `Weave` · `Tangle` · `PLtoTF` · `TFtoPL` · `GFtoPress` | Knuth's TeX and METAFONT, running on a Dorado, printing through Press. | A large toolchain, and printing has nowhere to go. |

### Probably not worth it

The `DF/release/build` and `XNS*` families (65 packages between them) are
infrastructure for maintaining and releasing Cedar across a site of
machines. Interesting as history, invisible as a demo.

---

## 4. How to fetch and run any of them

```sh
python3 tools/fetch_cedarchest_app.py 'Griffin.df!6'      # name!version
```

The version comes from the index linked at the top. The tool parses the DF's
Exports/Directory sections, downloads every listed file, and lays them out
where the in-process STP server expects them. If a payload directory is
missing on `cyan`, try the CedarChest6.0 directory — CardTable's files
survive only there, with identical creation dates.

Then, in the running desktop (click the CommandTool's `%` prompt first):

```
Bringover [Cedar]<CedarChest6.1>Top>Griffin
Run GriffinImpl
Griffin
```

**Three traps, all of which have already cost time here:**

- **A package's run name is not its DF name.** ChessHack ships `ChessDefs`,
  `ChessHackImpl` and a `ChessHack.load` manifest — there is no
  `ChessHack.bcd`, so `Run ChessHack` fails no matter how it was fetched.
  Read the `.df` or `.load` and `Run` an *implementation* module. Same for
  AIS (`Run AISImpl` / `Run AISViewerImpl`).
- **`Bringover -p` fetches public files only.** Data files — `.cm`, images,
  documents — need a plain `Bringover`. The failure is silent: "1 files
  acted upon", meaning the `.df` and nothing else.
- **Fonts only attach at cold boot.** A package needing a font Cedar has not
  installed will raise `ImagerImpl.Error[$fontNotFound, ...]`, and neither a
  runtime `Bringover` nor file injection fixes it — the font must go into
  the served `TiogaFonts.df` and the desktop checkpoint be rebaked. See
  `docs/running-the-emulator.md` and memory `cedar-font-install-attach`.

**Cost note for the browser:** `CedarChest6.1` is preloaded into
`index.data`, which every visitor downloads. Gargoyle alone added 4.7 MB.
Anything large should follow the Cedar-sources precedent and ride in the
lazily-fetched `cedar-src.tar.gz` instead (`docs/cedar-file-server-plan.md`
§7.5).

---

## 5. The full index — 316 packages

`mirrored` marks what is in `chm/cedar/stp-root/CedarChest6.1/` today.
Versions are the latest in the 6.1 index.

### Graphics / illustration (36)

| package | latest | mirrored |
|---|---|:-:|
| Cartoon | `df!2` |  |
| ColorDisplay | `df!18` |  |
| ColorFns | `df!2` |  |
| ColorTool | `df!1` |  |
| ColorTrix | `df!131` |  |
| ConvolutionFilter | `df!1` |  |
| Draw2d | `df!19` |  |
| Dunn | `df!34` |  |
| Eikonix | `df!5` |  |
| Fig | `df!4` |  |
| FontEdit | `df!3` |  |
| FontTune | `df!3` |  |
| FrameBufferReader | `df!1` |  |
| Gargoyle | `df!27` | **YES** |
| Griffin | `df!6` |  |
| IconEditor | `df!4` |  |
| ImageFFT | `df!1` |  |
| ImagerExamples | `df!2` |  |
| ImagerForkContext | `df!10` |  |
| ImagerGoodies | `df!4` |  |
| ImagerGray | `df!13` |  |
| ImagerMaskContext | `df!3` |  |
| ImagerMemory | `df!1` |  |
| ImagerPD | `df!8` |  |
| ImagerSmooth | `df!1` |  |
| ImagerToJaM | `df!1` |  |
| ImagerViewer | `df!2` |  |
| JaMImager | `df!7` |  |
| LFBoundingBox | `df!1` |  |
| Magnifier | `DF!1` |  |
| Pixels | `df!13` |  |
| PolyHack | `DF!3` |  |
| Sil | `df!5` | **YES** |
| Texture2D | `df!1` |  |
| TiogaImager | `df!3` |  |
| TSetter | `df!6` |  |

### Press / Interpress / print (20)

| package | latest | mirrored |
|---|---|:-:|
| GFtoAC | `df!1` |  |
| GFtoPress | `df!1` |  |
| GFType | `df!1` |  |
| GriffinToIP | `df!1` |  |
| ImagerPress | `df!1` | **YES** |
| Interpress | `df!13` | **YES** |
| InterpressTools | `df!29` |  |
| MF | `df!1` |  |
| PeachPrint | `df!11` |  |
| PLtoTF | `df!2` |  |
| PressFileUtilities | `df!1` |  |
| PressReader | `df!1` | **YES** |
| PreView | `df!23` |  |
| Print | `df!2` |  |
| SirPress | `DF!2` | **YES** |
| Tangle | `df!2` |  |
| TeX | `df!5` |  |
| TFtoPL | `df!2` |  |
| ViewersToIP | `df!2` |  |
| Weave | `DF!2` |  |

### 3D / math / plotting (18)

| package | latest | mirrored |
|---|---|:-:|
| AlgebraicSurfaces | `df!14` |  |
| AlgebraStructures | `df!32` |  |
| BigCardinals | `df!2` |  |
| Contours | `df!47` |  |
| CubicSplinePackage | `df!2` |  |
| Geometry3d | `df!39` |  |
| Geometry3dApplied | `df!126` |  |
| Graphs | `DF!9` |  |
| Graphs0 | `df!1` |  |
| Histograms | `DF!16` |  |
| Histograph | `df!3` |  |
| HistoVAM | `df!1` |  |
| Intervals | `df!1` |  |
| Math | `df!4` |  |
| MathLib | `df!2` |  |
| SynChart | `df!1` |  |
| ThreeDWorld | `df!54` |  |
| Tube | `df!144` |  |

### Games / demos (16)

| package | latest | mirrored |
|---|---|:-:|
| CardTable | `df!1` | **YES** |
| Celtics | `df!7` |  |
| ChessHack | `df!3` | **YES** |
| Cookie | `DF!1` |  |
| Football | `df!3` |  |
| GoodTimes | `df!1` |  |
| Lightning | `df!3` |  |
| MazeWar | `DF!11` |  |
| MickeyMouse | `df!7` |  |
| OneCasaba | `df!7` |  |
| PigsInSpace | `df!2` |  |
| RandomDemo | `df!8` |  |
| ThreeCasabaFour | `df!2` |  |
| VBounce | `df!2` |  |
| Waterlily | `df!1` |  |
| Yodel | `df!3` |  |

### Desktop / viewers / UI (29)

| package | latest | mirrored |
|---|---|:-:|
| Abutters | `DF!5` | **YES** |
| AtomButtons | `df!2` |  |
| BiScrollers | `df!12` | **YES** |
| Clock | `df!1` | **YES** |
| Controls | `df!85` |  |
| Cursory | `df!3` | **YES** |
| DeskTops | `df!3` |  |
| EditorComforts | `df!7` |  |
| FastMouse | `df!10` |  |
| Forms | `df!27` |  |
| HierarchicalDisplays | `DF!1` |  |
| IconRegistry | `df!2` |  |
| IdleHacks | `DF!19` |  |
| Knobs | `df!6` |  |
| NewCalc | `df!1` |  |
| NewClock | `df!6` |  |
| PopUpButtons | `DF!22` | **YES** |
| PopUpCommand | `df!6` |  |
| PopUpMenu | `df!11` |  |
| PopUpMenus | `df!3` |  |
| Promptery | `DF!1` |  |
| QuickViewer | `DF!12` |  |
| Remember | `df!12` |  |
| SimpleViewer | `df!5` |  |
| TopButtons | `df!18` |  |
| ViewerAbort | `df!4` |  |
| ViewerConstraints | `df!1` |  |
| ViewRec | `DF!7` | **YES** |
| VisibleMouseClicks | `df!1` |  |

### Editor / text / Tioga (21)

| package | latest | mirrored |
|---|---|:-:|
| ACFind | `df!2` |  |
| BravoToTioga | `df!2` |  |
| CedarSyntax | `df!1` |  |
| ColumnLs | `df!3` |  |
| FindKeywords | `df!3` |  |
| Grep | `df!1` |  |
| KeyboardTioga | `DF!2` |  |
| OperatorPrecedenceParse | `DF!1` |  |
| PrettyPrint | `DF!2` |  |
| QFind | `df!1` |  |
| RegularExpression | `df!4` |  |
| SpellingTool | `df!8` |  |
| StructuredStreams | `DF!2` |  |
| StyleTool | `df!1` |  |
| TextReplace | `DF!1` |  |
| TiogaButtons | `df!2` |  |
| TiogaDWIM | `DF!25` |  |
| TiogaExecCommands | `df!7` |  |
| TiogaStreams | `df!2` |  |
| WordCount | `df!1` |  |
| XRef | `df!2` |  |

### Mail / messaging (15)

| package | latest | mirrored |
|---|---|:-:|
| Chat | `df!2` |  |
| ChatCommands | `df!6` |  |
| GVMail | `df!1` |  |
| InterVoice | `df!4` |  |
| Peanut | `df!9` |  |
| PeekMail | `df!2` |  |
| SimpleMailer | `df!2` |  |
| VoiceUtils | `df!7` |  |
| Walnut | `df!24` |  |
| WalnutKernel | `df!24` |  |
| WalnutQuery | `df!3` |  |
| WalnutRegistry | `df!2` |  |
| WalnutRescue | `df!4` |  |
| WalnutSend | `df!10` |  |
| WalnutSort | `df!7` |  |

### Network / Pup / XNS (34)

| package | latest | mirrored |
|---|---|:-:|
| ArpaWatch | `df!6` |  |
| CourierBinding | `df!3` |  |
| CrRPC | `df!7` |  |
| DicentraRS232CAsync | `df!2` |  |
| EtherLoad | `df!4` |  |
| EtherWatch | `df!6` |  |
| Finger | `df!14` |  |
| FTP | `df!2` |  |
| GPIB | `df!10` |  |
| HostButton | `df!4` |  |
| HostName | `df!2` |  |
| Lupine | `df!2` |  |
| LupineStubs | `df!1` |  |
| PupBootServer | `df!4` |  |
| PupBSPTool | `df!15` |  |
| PupDebug | `df!2` |  |
| PupDocs | `df!2` |  |
| PupEchoTool | `df!6` |  |
| PupEtherBoot | `df!8` |  |
| PupNetWatcher | `df!6` |  |
| PupRouterTool | `df!3` |  |
| PupWatch | `df!11` |  |
| RPCEchoTool | `df!1` |  |
| STPServer | `df!3` |  |
| TerminalEmulator | `DF!8` |  |
| TrickleChargeServer | `df!2` |  |
| XNSAuthentication | `df!18` |  |
| XNSBasicTypes | `df!2` |  |
| XNSClearinghouse | `df!13` |  |
| XNSPrinting | `df!2` |  |
| XNSPrintingUI | `df!17` |  |
| XNSServerLocation | `df!8` |  |
| XNSTools | `df!2` |  |
| XNSTransport | `df!18` |  |

### Database / storage (24)

| package | latest | mirrored |
|---|---|:-:|
| AlpineBackup | `df!9` |  |
| AlpineServer | `df!3` |  |
| AlpineShared | `df!3` |  |
| AlpineUser | `df!5` |  |
| ArchivistBTree | `df!5` |  |
| Catalog | `df!6` |  |
| Cypress | `DF!21` |  |
| DBIcons | `df!1` |  |
| DBTools | `df!4` |  |
| GList | `df!5` |  |
| HashTable | `df!1` |  |
| Heap | `df!1` |  |
| IntChainedHashTable | `df!2` |  |
| IntHashTable | `df!2` |  |
| LoganBerry | `df!7` |  |
| LoganBerryTools | `df!11` |  |
| LRUCache | `df!2` |  |
| Nut | `df!2` |  |
| Properties | `df!2` |  |
| PropertyLists | `df!3` |  |
| RedBlackTreeExtras | `DF!1` |  |
| Trans | `df!2` |  |
| Values | `df!1` |  |
| Whiteboard | `df!5` | **YES** |

### System / debugging / perf (22)

| package | latest | mirrored |
|---|---|:-:|
| Checksummer | `df!5` |  |
| CircularGarbage | `df!2` |  |
| CommanderPriority | `df!2` |  |
| ComputeWorkingSet | `df!1` |  |
| DebuggingWorld | `df!2` |  |
| DiskTool | `df!2` |  |
| EndianDebug | `df!3` |  |
| ExamineStorage | `df!2` |  |
| Ferret | `df!1` |  |
| FixBadPage | `df!6` |  |
| GmtDebug | `df!1` |  |
| MemoryMonitor | `DF!4` |  |
| MonitorTool | `DF!3` |  |
| ProcessWatcher | `DF!1` |  |
| RecoverFromCache | `df!1` |  |
| Scanner | `df!4` |  |
| ShowVAM | `df!1` |  |
| SlackProcess | `df!6` |  |
| SleepPeek | `DF!2` |  |
| Spy | `df!11` |  |
| Summoner | `df!30` |  |
| Watcher | `df!5` |  |

### DF / release / build tools (31)

| package | latest | mirrored |
|---|---|:-:|
| AutoBackUp | `df!2` |  |
| Benchmarks | `df!19` |  |
| CedarChest | `df!56` |  |
| CedarChestRelease | `df!117` |  |
| CedarChestVersionMap | `df!70` |  |
| CedarExamples | `DF!4` |  |
| CmdTest | `DF!2` |  |
| Compare | `df!5` |  |
| CopyFromVersionMap | `df!2` |  |
| Dependencies | `df!1` |  |
| DFCommandsExtras | `df!7` |  |
| DFDependencies | `DF!6` |  |
| DFIncludes | `df!1` |  |
| DFPackageExtras | `df!10` |  |
| DFTool | `df!1` |  |
| Documentation | `df!27` |  |
| DoradoInfo | `df!5` |  |
| Environment | `df!24` |  |
| GetDWIM | `DF!1` |  |
| GetFromRelease | `df!3` |  |
| GetMe | `DF!1` |  |
| Install | `DF!7` |  |
| ListArchives | `df!2` |  |
| Lister | `df!7` |  |
| Maintain | `df!3` |  |
| MakeDo | `DF!30` |  |
| RedirectDF | `df!2` |  |
| ReleaseTools | `df!7` |  |
| UsingDependencies | `df!1` |  |
| VersionMapBuilder | `df!1` |  |
| VersionOf | `DF!5` |  |

### Language / interpreter (28)

| package | latest | mirrored |
|---|---|:-:|
| Args | `df!6` |  |
| Asserting | `df!3` |  |
| Combiner | `df!1` |  |
| CommandToolProcedures | `DF!1` |  |
| Cucumber | `DF!1` |  |
| DirectoryList | `df!1` |  |
| ExecHacks | `df!15` |  |
| ExpungeOpens | `df!1` |  |
| FileCmds | `DF!3` |  |
| FileStreamTest | `df!1` |  |
| FileUtil | `df!2` |  |
| FSRope | `df!1` |  |
| FSUtil | `df!3` |  |
| JaM | `df!4` |  |
| Kipper | `df!3` |  |
| MetaCedar | `df!1` |  |
| Misp | `DF!1` |  |
| PascalRuntime | `df!1` |  |
| PasMesa | `df!1` |  |
| RecursivelyNIL | `df!1` |  |
| RepeatCommand | `df!1` |  |
| RollbackAndAnything | `df!3` |  |
| Sirocco | `df!1` |  |
| SiroccoDWIM | `df!7` |  |
| StatementInterpreter | `DF!2` |  |
| Sweep | `df!6` |  |
| Trc | `df!24` |  |
| WorkingDirectory | `df!1` |  |

### Other / uncategorised (22)

| package | latest | mirrored |
|---|---|:-:|
| AIS | `df!1` | **YES** |
| AISCopy | `df!1` |  |
| AISTools | `df!2` |  |
| AISViewer | `df!1` | **YES** |
| CaminoReal | `DF!5` |  |
| CKViewer | `df!2` |  |
| EncryptTool | `df!2` |  |
| Finch | `df!12` |  |
| GVTools | `df!8` |  |
| HeadsTTYDLion | `df!2` |  |
| JaMIDunn | `df!3` |  |
| LogViewer | `df!2` |  |
| PhoneList | `df!2` |  |
| PowerOff | `df!1` |  |
| RefTabPrint | `df!1` |  |
| SortLabels | `df!2` |  |
| Tajo | `df!2` |  |
| TapeTool | `df!1` |  |
| Tempus | `df!2` |  |
| UserProfileOps | `df!1` |  |
| VTables | `df!2` |  |
| XBus | `df!46` |  |
