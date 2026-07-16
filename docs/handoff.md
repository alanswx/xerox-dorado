# Handoff: continue building the Xerox Dorado emulator

## ===> 2026-07-16: WHERE EVERYTHING STANDS (read this first)

**The emulator boots Cedar 6.1 to a live, usable Viewers desktop, and the
desktop installs and runs period software over the in-process network.**
This section is the self-contained handoff; the dated sections below it
are the historical log.

### What works, with the exact repro for each

| Capability | Repro | Gate |
|---|---|---|
| Alto games / NetExec (netboot) | `make run-galaxian` etc. | headless `--cycles 160000000` = 121,549 px |
| Cedar 6.1 cold boot -> login | `make run-cedar-work`, type `Guest` + 2 Returns | login at ~1B cycles |
| Cedar 6.1 full install -> Viewers desktop | same, keep running | desktop at ~21B cycles (34 loadees, fonts, icons) |
| Desktop checkpoint (instant) | `make run-cedar-desktop-sdl` / web dropdown | restores at 21.0B |
| Herald soft reboot | click `Boot` then `CedarWork` on the desktop | fresh Basic boot to desktop (germ re-entry) |
| App install into the live desktop | `Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack` then `Run ...` | 2026-07-16 scripted run |
| Interlisp-D Lyric desktop | web dropdown / lisp targets | saved Exec checkpoint |
| Diagnostics | `build/rundiag` | all six PASS |

Media/PDI invariants that keep the Cedar path alive: germ and microcode
versions must match (`CedarDorado.eb!6` + `Dorado.germ-6.1.6`); synthetic
PDIs need `DORADO_PDI_IGNORE_LABEL_FLAGS=1` (File.FP DA hints are 0),
free-page labels stamped with the volume ID
(`tools/pdi_stamp_free_labels.py`), and BOTH physical- and logical-root
bootingInfo records (`tools/pdi_install_lv_bootfiles.py`) — cold boot
reads the PV root, BootTool/Booting.Boot soft boots read the LV root.

### The three bugs that cost the most this week (don't rediscover them)

1. **WASM stack overflow** (fixed d22bcc3): a framebuffer-sized stack
   local overflowed Emscripten's 64 KB stack and silently corrupted
   static data (symptom: `getenv()` going NULL mid-run, PDI save-back
   ignored). `ddc_fb` is now static; both wasm links use
   `-sSTACK_SIZE=1048576`. If the web build acts haunted, suspect stack
   first.
2. **PDI germ-stream cursor** (fixed 02da524): the polled germ-boot
   bridge streams GERMDATA pages via a cursor that used to persist across
   boots and snapshot restore, so herald soft reboots continued the OLD
   stream (germ asked page 0o213, got 0o2257) and halt-spun. Any
   non-GERMDATA polled command now resets the stream. The CLI masked it
   (re-applies `--pilot-disk` after `--snapshot-in`); SDL/web didn't.
3. **Return-vs-Link** (fixed c25240b): an explicit `Link<-` in the same
   microinstruction overrides Return's `Link<-CIA+1` reload (HM 4.5,
   DMesaFloat.mc's co-routine contract). This was the "fonts crash".

### Live debugging leverage (browser included)

- `Module.ccall('dorado_web_debug')` in the browser console dumps the
  machine X-ray (per-task hot PCs, IFU dispatch history, disk/PDA/FTP
  state). Histograms need a build with `DORADO_MACHINE_PCHIST=1`.
- The same dump is `DORADO_FINAL_DEBUG=1` (or `--progress`) on the CLI.
- `DORADO_DISK_IOCB_TRACE=1` now prints requested-vs-streamed pages per
  polled IOCB — how the germ-stream bug was caught.
- A hidden/backgrounded browser tab suspends the rAF main loop — it looks
  exactly like a hang and isn't one.
- Scripted desktop interaction: focus-click the CommandTool first
  (`--click 700,467`); repeated characters need `--key-hold 1600000`.

### Open problems, in priority order

1. **`Run AISViewer` (a packaging CONFIG) hangs the CommandTool** —
   >2.5B cycles, no `Ran:` line, world otherwise alive. Plain modules
   (`Run AISImpl`, `Run AISViewerImpl`) load fine, including straight
   from the remote release dir. Root-cause the config-load path (Loader
   pulling inner modules? a second STP conversation wedging?). Repro is
   scripted — see running-the-emulator.md "Installing CedarChest
   applications".
2. **Installed-system-volume boot** (the kitchensink images): still
   renders 0 px at 2B cycles even with LV records + stamped labels
   (2026-07-16 test on a patched copy). These volumes take the
   installed-volume path (not our login volumes' simple path); finishing
   that disk bridge unlocks the three software-stuffed volumes in
   `CedarDisk/`. Start from the 2026-07-10 analysis in CONTINUE-HERE.md.
3. **Disk-resident microcode boot** ("disk bootable" in the historical
   sense): we still netboot `CedarDorado.eb!6` via the fake Ethernet.
   A real CSL Dorado read microcode + germ from the disk (Othello
   installs them; `MicrocodeDorado.df` lists the set; Initial's
   disk-boot path reads them). Needs: Initial's disk microcode-fetch
   path in the emulator + a PDI carrying the microcode partition.
   References: `docs/parc-veteran-notes.md`,
   `chm/cedar/cedar6.1-docs/ReleaseMessage.tioga!9` (the from-scratch
   install procedure), Booting memo.
4. **Multiple volumes**: `--pilot-disk SLOT=PATH` mounts 4 drives, and
   the IOCB bridge honors the drive field, but whether Cedar's Dorado
   head enumerates a second drive is unproven. Alternative: multiple
   LOGICAL volumes on one physical disk is the authentic layout
   (Alto | Basic | CedarWork | ... on one T-300) — rusty-backup writes
   single-LV images today; extending it to multi-LV + using Othello's
   partitioning is the cleaner road.
5. **Replace rusty-backup with a real Othello install** — the veteran's
   pointers (ReleaseMessage.tioga, Othello, `File.SetRoot`) describe the
   authentic procedure; it would produce media with correct DA hints,
   VAM, and multi-LV layout, retiring most compat switches at once.

### Fetching more software

`tools/fetch_cedarchest_app.py 'Name.df!N'` mirrors any CedarChest6.1
package into the served tree (versions from
`cyan/cedarchest6.1/top/.index.html`; some payloads survive only in
`cedarchest6.0/...` with identical dates). `tools/pbm2ais.py` turns any
`pdftoppm -mono` page (e.g. the Dorado schematics in
`DoradoDocs/schematics/`) into an AIS raster the desktop can display.

## 2026-07-14: cold-boot regression root-caused and fixed; the FS.Error wall
## was the File.FP DA hint; fonts now served at their real export paths.

**Context.** A host reboot wiped `/private/tmp`, taking the 2026-07-13
session's load-bearing artifacts with it: `CedarDorado-chs-contig10.pdi` (the
only image that cold-booted end to end) and the prefail snapshots. Neither
committed PDI could reproduce the baseline. Root-causing that exposed two
emulator-side problems that the lost image had been masking, both now fixed:

1. **Cold boots stalled in the germ (fixed).** Commit 2e8018b hard-coded CHS
   decoding of every IOCB DiskAddress, but the committed images store *flat*
   page numbers in their boot-chain links (`bootFile firstLink=119`), so the
   germ's first chain read decoded as page 119*28=3332 and retried forever.
   The fix restores the pre-2e8018b bridge semantics verbatim for *polled*
   IOCBs (CSB interrupt mask 0 -- only the germ polls): flat addressing, raw
   label copyback, GERMDATA sequential streaming, unconditional success.
   Pilot's interrupt-driven traffic keeps the per-action CHS-faithful path.
   `machine_germ_complete_disk_iocb` in machine.c; verified against a
   pre-regression reference build (worktree at 2e8018b^) -- both committed
   PDIs reach the SimpleTerminal login at ~646M cycles.

2. **`FS.Error: File.FP from directory/cache doesn't correspond to a local
   volume file` on every volume (fixed).** The Cedar-nucleus label fileID
   field is `File.FP` = FileID (words 0-1) + **DA, a 32-bit disk-address
   hint (words 2-3)** -- see `CedarDisk/PARC_PILOT_FORMAT.md` §2.1. Pilot
   builds its expected label from the FP it holds (directory/cache B-tree),
   so on real media the stored labels carry the hint and the 8-word hardware
   compare covers it. All our converted/synthetic PDIs store DA=0 in every
   label, so FS's FP-to-label validation failed -- LoaderDriver could not
   even open its command-file cache, printed the FS.Error above for both
   `Basic.Loadees` paths, and gave up before any STP traffic. Under the
   existing media-compat switch `DORADO_PDI_IGNORE_LABEL_FLAGS=1`, label
   words 2-3 are now treated as dontCare (the lost chs-contig10 image
   presumably carried correct DA hints -- that is what its name meant).
   With this, both bestof and the work volume resolve and serve
   `Basic.Loadees` over STP again; bestof then hits its documented
   "No more free pages" wall, so the 65K-page work volume is the medium.

   Also fixed on the way: a successful label *check* now reads the final
   two label words (the boot-chain link) into `IOCB.diskLabel` -- the
   controller's private label, per PilotDisk.mc -- instead of the client's
   labelPtr, where the completion copyback used to clobber the fresh link
   with the stale private one.

**Fonts are served properly now.** Three parts:

- `[Indigo]<Fonts>Top>FontMetrics.df!2` and `PressFonts.df!1` fetched into
  `chm/cedar/stp-root/CedarFonts/Top/`. `XC1-2-2-Fonts.df` does not survive
  anywhere in the archive (only `[Cyan]<CedarPrinter6.1>` printer variants);
  its GetFonts probe fails cleanly and is skipped by InstallerImpl.
- The font files moved to their full export paths -- `Fonts/TiogaFonts/*.ks`
  (127), `Fonts/FontMetrics/*.tfm` (234), `PressFonts/*.sd` (34) -- matching
  the STP resolver's Directory/Name-Body mapping (`Exports
  [Fonts]<Fonts>TiogaFonts>` -> `Fonts/TiogaFonts/...`). The old flat
  `Fonts/` layout could never have served a demand-fetch.
  `tools/fetch_cedar_fonts.py` now parses all three DFs, keeps every export
  path component, and uses the archive's own (case-sensitive) file names --
  34 `.sd` + 5 `.ks` files had failed on case alone.
- `eth_ftp_resolve_file` strips an IFS `!<version>` suffix (FS demand-fetches
  an attached file by the exact version its DF pinned; our tree stores bare
  names and the DF date index is keyed the same way).

**The 2026-07-13 "zero font requests" mystery is resolved -- it was correct
behavior.** `InstallerImpl.TryForFonts` runs `DFOperations.BringOver[action:
enter]` under autoConfirm (the normal non-long-dialogue boot answers every
question with its default instantly): `enter` with explicit dates skips the
remote GetFileInfo and does `FS.Copy[remoteCheck: FALSE, attach: TRUE]` -- a
pure local attach, zero network traffic. The demand-fetch happens later, at
ViewersPackage START: `VFontsImpl.CreateDefaultFont` calls
`ImagerFont.Find["Xerox/TiogaFonts/Tioga10"]`, which is
`FS.EnumerateForNames["///Fonts/Xerox/TiogaFonts/Tioga10.*!h"]` over the
*local* (attached) name table followed by `FS.Open` on the match -- and that
open issues the STP Retrieve. If Find fails, CreateDefaultFont retries
`EstablishFont["Tioga", 10]` **with no catch**, so the second
`Imager.Error[$fontNotFound]` (raised by ImagerTypefaceImpl, which lives in
ImagerPackage.bcd -- matching the 2026-07-13 VM-dump identification)
propagates uncaught and DebugNub blanks the screen. Sources fetched to
`chm/cedar/cedar6.1/{imager,dfpackage}/` and `viewers/VFontsImpl.mesa!1`.

**Verification state.** Unit tests: all pass except the documented
pre-existing `test_ethernet` NetDir reply mismatch. The full 30B-cycle cold
boot outcome (later the same day): after two more fixes -- free-page labels
stamped with the volume ID (a687874: Cedar verifies allocation candidates
against `FileInternal.FreeLabel`, whose fileID is the VolumeID; rusty wrote
0, so FS reported volumeFull with 64K pages free) and BSP connection slots
raised to 16 with closed-slot recycling (02a1203: evicting a live
connection stranded its client and hung the install at transfer #50) --
the ENTIRE install completes from committed artifacts: 96 STP transfers
including all three fonts DFs. The Imager crash then still fires at
ViewersPackage START (12,939,649,700 cycles pinned) with ZERO font-file
demand-fetches, so the frontier is in-guest between BringOver[enter]'s
attach and ImagerFont.Find's EnumerateForNames. Live state and staged
diagnostics: docs/CONTINUE-HERE.md top section.

**Beware:** `make run-galaxian` is an *interactive* SDL target, not the
pixel gate. `/private/tmp` does not survive reboots -- keep load-bearing
media derivations reproducible from committed files and tools.

## 2026-07-13: Cedar installs the boot essentials, then crashes in the Imager
## for want of fonts. FONTS ARE THE ONE REMAINING BLOCKER TO THE DESKTOP.

**TL;DR for whoever picks this up.** Four emulator bugs fixed (below). A cold
boot now logs in as Guest, transfers all 34 `Basic.Loadees` packages, STARTs
them all, runs `InstallerImpl`, and BringOvers the entire `BootEssentials.df`
closure. It then dies on an **uncaught signal raised by `ImagerPackage.bcd`,
because no font ever reaches the guest**; Cedar's crash path blanks the
screen on the way into the (nonexistent) debugger. Get fonts into the guest
and the Viewers desktop should follow. Nothing else is known to be broken.
Pin `DORADO_FAKE_TIME=1783285880` on every run or nothing reproduces.

**Where it stops now.** A clean cold boot logs in as Guest, transfers all 34
`Basic.Loadees` packages, **STARTs them all**, runs `InstallerImpl`, and
BringOvers the whole `BootEssentials.df` closure -- 93 STP transfers, ending
with every TIP table, `.icons`, JaM program, style and `User.Profile` the
Viewers desktop needs.

**Then Cedar takes an uncaught error and its crash path blanks the screen.**
The blank display is a *symptom*, not the bug -- do not go looking for a
missing Viewers display turn-on.

Evidence (`DORADO_STORE_TRACE_VA=0420,0421`, the Cedar CSB): across a 30B
cycle boot the DCB-chain head at absolute `0420` is written exactly four
times -- `0` early, `003120` (blank DCB = `TerminalFace.Connect`) at 675M,
`003126` (real DCB = `TurnOn`) at 681M as SimpleTerminal comes up, and then
**`0` at cycle 11,618,516,237, which is the last write in the whole run.**

That final write is a *bare* Disconnect with no preceding TurnOff. It cannot
be SimpleTerminal's shutdown or a terminal Select: both go through
`TerminalImpl.BWTurnOff` first, which writes the blank-DCB value `003120`.
The only code that nils the head in one write is
`TerminalHeadDorado.InitializeCleanup`'s DeviceCleanup handler
(`chm/cedar/os-src/TerminalHeadDorado.mesa!1.cyan61`):

```
turnOff => { state _ csbPtr^; csbPtr.dcbChainHead _ pDCBNull; ...spin... };
turnOn  => { csbPtr^ _ state };          -- restores the head
```

and the only caller of `DeviceCleanup.Perform[turnOff]` is DebugNub's
`MemorySwap.SwapIt` (`chm/cedar/tentacles/DebugNub.mesa!1.txt`) on its way
into the debugger. With no debugger to swap to it reaches
`SetMP[MPCodes.cantWorldSwap]` / `GermSwap.Teledebug` and never gets to the
matching `DeviceCleanup.Perform[turnOn]`, so the display stays dark forever.
The final `WDC=1` corroborates it: exactly one unbalanced
`DisableInterrupts`, from `CoreSwap`.

So the guest is "alive and scheduling" (`ready` non-empty, clock ticking)
only because the other processes keep running while the faulting one sits in
the debugger.

**The uncaught signal is raised by `ImagerPackage.bcd` -- it is the missing
fonts.** Traced end to end (see recipe below). With the clock pinned, the
IWDC (`CoreSwap`'s `DisableInterrupts`, uPC `0o516`) lands at cycle
11,503,742,140 and the display blanks 8,000 cycles later. There is **no page
fault** -- it is a Mesa SIGNAL, not a memory fault. The `br31` call chain
across the window reads:

```
br31=0x120808 (VA 0o4404564)  a loaded BCD    -- raises the signal
  -> br31=0x34100 / 0x33F00   MesaRuntime     -- SignalsImpl hunts for a catcher (2454 dispatches)
  -> br31=0x3470C             Tentacles       -- DebugNub.CoreSwap
  -> br31=0x32500             HeadsDorado     -- ProcessorHead SetMP[cantWorldSwap]
  -> br31=0x33208             HeadsDorado     -- TerminalHead DeviceCleanup turnOff, then the
                                                 GetClockPulses spin (op=364 alpha=011 = RClockM)
```

Dumping the raiser's code (`DORADO_VMDUMP=04404400,04404700,<cycle>`) and
matching the words against the BCDs we serve identifies it exactly:
**`ImagerPackage.bcd`** (offset `0x6600`). The Imager cannot paint without a
font, and no font ever reaches the guest -- so it signals, nothing catches
it, and the crash path takes the display down. Everything else in this
section (BringOver requesting no font files, the missing sibling fonts DFs)
is the same bug seen from the other end.

**So: fix the fonts and the desktop should follow.** Concretely:

1. `InstallerImpl.GetFonts` calls `DFOperations.BringOver[... action: enter]`
   -- **`enter`, not `fetch`**. In `BringOverImpl.Localize`, `enter` skips
   `DFInternal.GetFileInfo` for explicit dates and takes the `AttachNeeded`
   path: it *attaches* files (local FS links to the remote copy) rather than
   copying bytes, so zero font transfers is arguably correct behavior and the
   fonts should be demand-fetched when the Imager opens one. Confirm whether
   the attach actually happens, and why no later font Retrieve appears.
2. Three of the four fonts DFs are still `STP_MISSING`
   (`FontMetrics`, `PressFonts`, `XC1-2-2-Fonts`); only `TiogaFonts.df` is
   served, and it is `[Indigo]<Fonts>Top>TiogaFonts.df!5` (exports to
   `[Fonts]<Fonts>`), not a genuine `[CedarFonts]<Top>` DF. FontMetrics in
   particular is what the Imager wants.
3. A blunter fallback if the DF path stays stubborn: pre-place the fonts on
   the Pilot disk / in the local FS so the Imager finds them without any
   BringOver at all.

**IMPORTANT -- pin the clock or nothing reproduces.** The guest reads host
wall-clock time (the Cedar banner shows today's real date), so Cedar's
timeouts make cycle numbers drift between otherwise identical runs -- the
same crash landed at 11.618B, 11.536B and 11.504B on three runs. Add
`DORADO_FAKE_TIME=1783285880` (the value the Lisp targets use) to every run
and cycle-exact gating works again. All cycle numbers quoted above assume it.

**Recipe used** (both runs need the same `DORADO_FAKE_TIME`):

```sh
# 1. find the crash cycle: the last write of 0 to the CSB head, and the IWDC
DORADO_FAKE_TIME=1783285880 DORADO_RM_WATCH=6 DORADO_RM_WATCH_VALUE=1 \
DORADO_RM_WATCH_LIMIT=0 DORADO_STORE_TRACE_VA=0420,0420 ./build/dorado ...
# 2. capture the call chain into it
DORADO_FAKE_TIME=1783285880 DORADO_IFUDISP_TRACE=1 DORADO_FAULT_TRACE=all \
DORADO_TRACE_GATE=11503600000,11503752000 ./build/dorado ...
# 3. name the module: dump its code and match it against the served BCDs
DORADO_FAKE_TIME=1783285880 DORADO_VMDUMP=04404400,04404700,11503000000 ./build/dorado ...
```

**State of the fonts.** `TryForFonts` asks for
`[CedarFonts]<Top>{TiogaFonts,FontMetrics,PressFonts,XC1-2-2-Fonts}.df`.
`tools/fetch_cedar_fonts.py` populates `chm/cedar/stp-root/CedarFonts/Top/`
and `chm/cedar/stp-root/Fonts/` (122 of the 187 fonts `TiogaFonts.df` names
resolve in the CHM index; the other 65 are simply absent from the archive).
As of the last run `TiogaFonts.df` is requested, served and transferred --
but **zero font files are then requested**, and the other three fonts DFs are
still `STP_MISSING`. The archive has no `[CedarFonts]<Top>` equivalents; the
one we serve is `[Indigo]<Fonts>Top>TiogaFonts.df!5`, which exports to
`[Fonts]<Fonts>` (so a font request would arrive as `Fonts/<name>` and
resolve under `chm/cedar/stp-root/Fonts/`, which is populated and ready).

Two questions that earlier drafts of this file raised are now **answered, do
not re-ask them**: a font *is* required (the Imager signals without one), and
our renderer *is* watching the right control block (absolute `0420` -- the
guest writes it correctly right up until the crash path nils it).

**Four emulator bugs fixed to get here** (all in the last session):

1. **Host stores bypassed the Map** (`6e54812`). `machine_store_va()` wrote
   *both* `storage[va]` -- treating a virtual address as physical -- and the
   mapped physical slot. Once Pilot owns the Map that raw write lands on
   whatever physical page shares the VA's number. FS-cache disk deliveries
   around VA `0o3757200` were overwriting the germ's credentials strings
   (`nameInGerm`/`passwordInGerm`), so `UserCredentialsImpl` dereferenced
   ASCII text as a `LONG STRING`, a monitor-enter faulted on a garbage lock,
   and the uncaught `VM.AddressFault` entered DebugNub. Each of the 219
   debugger entries leaked +1 WDC until interrupts were dead (`WDC=0333B`)
   and `BusyWait` MTrapped. That was the whole "unmatched IWDC" mystery. A
   vacant map entry has no translation (its `rp` field is Pilot's software
   word), so the legacy raw write survives only for that case, which is what
   pre-map germ planting needs.

2. **STP served only one BSP connection** (`892797d`, `45e99ba`). Cedar keeps
   several connections to socket 3 open at once -- LoaderDriver's, plus the
   ones FS/DFOperations open for the Installer -- and interleaves them. Each
   has its own byte-ID space seeded from its RFC connection ID, so a single
   cursor let the abandoned-but-still-retransmitting connection advance
   `ftp_rx_next` past the live one; the live `Retrieve`/`Yes` then looked like
   duplicates, were dropped, and BringOver deadlocked with the guest idle.
   Filtering by client socket instead just starved the other connection (87
   unanswered AMarks). The `ftp_*` fields are now a working set that is
   context-switched by client socket across `ftp_ctx[]` slots.

3. **Creation dates were synthetic** (`45e99ba`). A DF pins every file to an
   exact creation date and BringOver asks for that version; `FSRemoteFileImpl`
   compares it against our `HereIsPList` and on a mismatch reports
   `FS.Error: Could not find "<file>" created on <date>` and **skips the
   file** -- so the Installer silently installed nothing. The dates live in
   the DFs themselves, so the server now indexes them (961 dates) and serves
   each file's real date. Note the DFs are **CR-delimited**: `fgets()` reads a
   whole DF as one line and indexes zero dates.

4. `dorado_poke_va()` had the same Map bug as (1).

**Content added.** `tools/fetch_cedar_essentials.py` resolves the
BootEssentials DF closure (16 sub-DFs -> 42 files) from the CHM archive;
`tools/fetch_cedar_fonts.py` does the fonts. Both write into
`chm/cedar/stp-root/`.

**Snapshots are stale.** `dorado_ethernet` grew, so every committed snapshot
asset (Cedar login, Lisp desktop, the wasm32 web assets) must be regenerated
before those `make` targets and the web build work again.

**Repro (cold boot; ~25 min to 30B cycles):**

```sh
cd dorado
DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_FTP_TRACE=1 ./build/dorado \
  --boot-reason disk --no-alto-boot \
  --eb "../chm/dorado/CedarDorado.eb!6" \
  --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
  --pilot-disk /private/tmp/CedarDorado-chs-contig10.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --type-at 760000000 --type 'Guest\n\n' \
  --cycles 30000000000 --out /private/tmp/cedar.pgm 2>/private/tmp/cedar.log
```

Useful greps: `STP_DATES` (date index size), `STP_TRANSFER` / `STP_MISSING`
(what installed and what did not), `FTP_UNSERVED` (a Pup the shim never
answered -- Cedar blocks forever on those), and `--final-debug`'s
`[pilot-pda]` line (is the guest scheduling or idle?).

Alto Galaxian still renders (121,513 px) and all seven test binaries pass.

## 2026-07-12 (late): Cedar loadee death root-caused — the WDC climb is the
## debugger-entry storm; the real frontier is a wild monitor-lock pointer in
## the post-load START phase (SUPERSEDED — that wild pointer was bug 1 above)

**Current goal:** bring Cedar 6.1 from the SimpleTerminal login through
`Basic.Loadees` to the Viewer desktop.  Do not call this path working until a
Viewer/desktop framebuffer is captured.

**All 34 files transfer.** Guest login completes the full remote-loader
sequence from `chm/cedar/stp-root`, ending with `Viewers>End.bcd` at cycle
~4,820,750,000. The failure comes **after** the load, while LoaderDriver
STARTs the loaded packages — not at `InterpreterTool.bcd` as previously
written. A stale `FS.Error` line in SimpleTerminal scrollback is not
evidence of network state.

**Proven death chain** (RM[6] write-watch, gated IFUDISP captures, µtrace,
and map census, all deterministic from the 700M snapshot):

1. At cycle `4828691614`, task-0 guest code executes a monitor-enter-family
   opcode (`0o147`, alpha `0o20`, µdispatch vec `0o714`) whose long lock
   pointer is garbage: the fetch goes through `MemBase=MLBR (0o34)` with
   `BR=0o1431263151` — the low bytes are ASCII `"efi"`, a pointer field
   read from string text. The executing module's code lies inside the
   mapped `FilePackage` BCD region (`br31=0x42708` = VA `0o1023410`, inside
   `file FilePackage[0,7]` per `BasicCedarDorado.loadmap!69`, fetched to
   `chm/cedar/basiccedar/`); its caller is freshly-loaded loadee code at
   VA ~`0o1305440` (`br31=0x58B20`).
2. The page fault on that wild VA becomes an uncaught `VM.AddressFault`:
   Signaller -> `DebugNub.Catcher` -> `CoreSwap` -> `DisableInterrupts`
   (WDC 0->1) -> `ToDebugger` port -> `MemorySwap.SwapIt` -> no debugger ->
   `SetMP[cantWorldSwap]` -> `GermSwap.Teledebug`. The rebalancing
   `WriteWDC[savewdc]`/`EnableInterrupts` path never runs for these entries,
   so **every debugger entry permanently increments WDC**. After the first
   one, `MesaResched1` always takes `MesaIntDisabled` (WDC#0): no interrupt
   is ever processed again and `CurrentTime` freezes.
3. Frame faults follow whose state-vector `fsi`
   (`PDA[PDA[process].context.state].fsi`, `FrameFaultProcess`) yields
   `frSize ~= 0xFFFF`, so `framePiecePages = 257` — one page more than the
   entire 256-page `$mds` partition. `AllocateVirtualMemoryInternal[count,
   $mds]` fails **without a single FindHole map probe** (count > partition;
   verified: zero `aGETMAPFLAGS` MISC-alpha-`0o151` reads in the failing
   window), and VMFaultsImpl calls `WorryCallDebugger["No VM for frame
   heap"L]` — 217 more times, each leaking +1 WDC through the same debugger
   entry. The MDS is NOT full: the map census shows ~140 free pages in ONE
   contiguous hole at failure time.
4. By cycle `4963123921` WDC reaches `0333B` (219 unmatched debugger-entry
   `IWDC`s). With every faulted process frozen on `PDA.fault[qFreeze]` (the
   earlier "detached but runnable PSB 0100B" was a frozen PSB) and WDC#0,
   `BusyWait` MTraps: dead state `NWW=100000B`, `CurrentPSB=0`,
   `CurrentTime=17613B`, as previously recorded.

**Superseded:** the "unmatched IWDC / scheduler / Requeue bug" framing. The
WDC climb is Cedar's *intended* stop-the-world debugger reaction repeated
219 times; `IWDC`/`DWDC`/`MesaInterrupt`/`Requeue` execute correctly (5,419
increments vs 5,420 decrements balanced outside the debugger entries). The
host-side WDC/ready recovery probes remain invalid fixes, as before.

**Exonerated (do not re-investigate):**

- STP/BSP protocol and the per-sector IOCB cache path: a pre-failure
  snapshot's guest storage matches **all 34 BCD files byte-exact**
  (`tools/cedar_bcd_verify.py`; zero partial-mismatch pages; "absent" pages
  are merely swapped out).
- Map-op emulation on this path: the world uses the *long* map ops (`zMISC`
  alphas `0o150 SETMAP` / `0o151 GETMAPFLAGS`; `useLong=TRUE` per
  `VMInternal`), implemented in microcode (dispatch observed), and the
  successful `$normalVM` allocations read the map correctly through our
  `RMap<-`/Pipe path.
- Background only: the old 1-word map ops (`aGETF`/`aASSOC`, see
  `chm/doradomicrocode/doradomicrocodesources/CedarMesa10MBMiscOps.mc!1`,
  fetched today) carry a pre-Trinity "crock" that VACATES any map entry
  whose real page exceeds `0o7777` when read. Not implicated here, but it
  matters if a world ever runs those ops with `useLong=FALSE` on our >1MW
  configuration.

**Next diagnostic:** chase the wild monitor-lock pointer of step 1. Rerun
with `DORADO_TRACE_GATE=4828683000,4828693000 DORADO_IFUDISP_TRACE=1
DORADO_FAULT_TRACE=all` and walk back from the `FAULT_CPU` line (µPC
`0o606`, `mesa_pc=0x029A`, opcode `0o147 alpha=0o20` at `pcf=0o1232`,
`br31=0x42708`) to where the caller (`br31=0x58B20`, `pcf`
`0o2676..0o3263`) obtained the pointer argument. Suspect class per project
history: µengine mis-execution around XFER/state-vector/operand delivery in
freshly-STARTed loadee code (cf. the Md-bypass and IFU-operand offset
sagas), not data corruption.

**Fast repro (validated cycle-exact).** A snapshot taken 2.7M cycles before
the fault turns each diagnostic iteration from ~4.5 minutes into ~0.3
seconds. Because the guest mutates the PDI in memory during the run, a late
snapshot must be paired with a PDI image saved at the same cycle; and the
`--type` events must be dropped on restore (their trigger cycles are already
past, so they would re-fire immediately and inject stray keystrokes).
Generate the pair once (from `dorado/`):

```sh
cp /private/tmp/CedarDorado-chs-contig10.pdi /private/tmp/cedar-prefail.pdi
DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_PDI_SAVE=1 ./build/dorado \
  --snapshot-in /private/tmp/cedar-chs-contig10-current-700m.snap \
  --pilot-disk /private/tmp/cedar-prefail.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --type-at 760000000 --type 'Guest\n\n' \
  --cycles 4826000000 --snapshot-out /private/tmp/cedar-prefail2.snap \
  --out /private/tmp/cedar-prefail2.pgm
```

Then iterate (no `--type`; without `DORADO_PDI_SAVE` the PDI file is never
written, so the pair is reusable as-is):

```sh
DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
DORADO_FAULT_TRACE=all DORADO_IFUDISP_TRACE=1 \
DORADO_TRACE_GATE=4828683000,4828693000 ./build/dorado \
  --snapshot-in /private/tmp/cedar-prefail2.snap \
  --pilot-disk /private/tmp/cedar-prefail.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --cycles 4828695000 --out /private/tmp/cedar-fastcheck.pgm
```

Validated: the wild fault fires at exactly `4828691614` (identical
registers/VA/µinstruction) and the first WDC leak at exactly `4828758358`
from this pair. Caveat: snapshots serialize raw C structs, so the pair is
ABI-tied to the emulator build — regenerate it after any struct change
(same recipe), and expect the cycle numbers to hold only while the emulator
stays behaviorally identical up to 4.826B.

**Source evidence:** `chm/cedar/refs/PilotMesaProcess.mc!1` defines `IWDC`,
`DWDC`, `MesaInterrupt`, `IdleReschedule`, and `BusyWait` (which rejects an
idle nonzero WDC). `chm/cedar/tentacles/` (fetched today) holds
DebugNub/GermSwapImpl/DebuggerSwap: `CoreSwap`'s `DI; ToDebugger; EI` loop,
`MemorySwap.SwapIt`'s `savewdc`/`WriteWDC` bracket, and the freeze machinery
(`FreezingPoint`, `qFreeze=3`). `chm/cedar/cedar6.1/vm/` (completed today)
holds VMFaultsImpl/VMAllocImpl/VMInternal/VMInitImpl: `FrameFaultProcess`,
`AllocateForLocalFrames`, `AllocateVirtualMemoryInternal`,
`FindHole`/`IsFree`, and the `VMMapEntry`/`useLong` definitions cited above.

**New diagnostics (all trace-only):**

- `DORADO_MAPCOUNT="lo,hi,interval"` — periodic census of map entries over a
  VA range, classified the way Pilot's allocator reads them (mapped /
  vacant-free / vacant-allocated / vacant-with-Ref); with
  `DORADO_MAPCOUNT_LAYOUT=1` also prints a per-page I/F/A/R string so hole
  sizes can be computed (machine.c).
- `FAULTREG src=Pipe3'` — `B<-Pipe3'`/`Map'` reads now log under
  `DORADO_FAULTREG_TRACE=1` like the existing Pipe2'/Pipe4' lines (cpu.c).
- `tools/cedar_bcd_verify.py` — verifies loadee BCD content inside a
  `--snapshot-out` image against the `--ftp-root` originals.
- Offline disk inspection: copy the PDI, run with `DORADO_PDI_SAVE=1`, then
  `build/pdidump`. Note Cedar's laundry keeps the STP cache almost entirely
  in VM, so the disk image alone does not contain the transferred BCDs.

**Recent emulator fixes:** the CPU cycle counter is now 64-bit (long Cedar
runs no longer wrap), and the Pilot timer channel mask is `0100000B`, as in
the original `PilotMesaProcess.mc`, rather than the Alto timer bit. The PDI
bridge remains enabled for post-germ IOCBs. The current synthetic-media run
uses `DORADO_PDI_IGNORE_LABEL_FLAGS=1`; strict label correctness is separate
unfinished work.

**Source-backed STP behavior:**

- Xerox `chm/cedar/stp/server-6.0/STPServerImpl.mesa!9`, `RetrieveFile`,
  specifies: `HereIsPList`, wait for client `Yes`, `HereIsFile`, copy bytes,
  `Yes "Transfer complete"`, EOC.  `Finish` uses `PupStream.SendMark`, so
  the response EOC is an ordinary BSP `Mark`, not an `AMark` that demands an
  immediate client acknowledgement.
- Its `SendPropList` forces `Server-Filename`, `Directory`, `Name-Body`,
  `Version`, and `Byte-Size` into a response.  The emulator now sends those
  fields, with `Byte-Size 8`, along with the requested creation date and
  size.  It also sends BCD payloads as ordinary BSP Data under Cedar's
  advertised 31-Pup / ~45-KB window.
- The matching original client source,
  `chm/cedar/stp/client-6.1/STPImpl.mesa!5`, has `GetFile` consume
  `HereIsFile`, copy the byte stream, then require `Yes` and EOC.  Use this
  exact state machine when investigating any subsequent stall; do not invent
  a different STP handshake.
- More specifically, `STPImpl.DoFiles` and `StartRemote` send
  `PutCommand[yes, 0C, "Yes, please"]` only after their confirmation callback
  returns a local writable stream.  `FSRemoteFileImpl.Retrieve` obtains that
  stream from the local cache.  The absence of the command is therefore a
  local-cache/disk completion problem, not an STP filename or a missing BCD.
- The earlier no-`Yes` state was the cache allocation failure at the first
  32-page boundary.  Per-sector IOCB progression now makes the client emit
  its live `Yes`, allowing every list entry to transfer.  The remaining
  failure is after local disk writes, when the Loader reads the cached BCD.

**Logging and media:** `DORADO_FTP_TRACE=1` emits one `STP_SERVE` line per
metadata request and one `STP_TRANSFER` line per actual BCD payload, plus
`STP_MISSING` and `FTP_ABORT` failures. `DORADO_PDI_BCD_TRACE=1` is the
focused local-file trace; `DORADO_PDI_SECTOR_CYCLES=10000` is an opt-in
real-sector-cadence experiment. `DORADO_UCODE_TRACE=1` plus
`DORADO_TRACE_GATE=lo,hi` limits microcode trace to a fault/scheduler window;
`DORADO_STORE_TRACE_VA=lo,hi` records stores. `DORADO_RM_WATCH=6` with
`DORADO_RM_WATCH_VALUE=333` pinpoints the first bad WDC increment. The
temporary `DORADO_PILOT_WDC_RECOVER` and `DORADO_PILOT_READY_RECOVER` probes
are diagnostics only, not fixes. All CHM source/media downloads
are stored in `chm/`, never `/tmp`; temporary logs, snapshots, and framebuffer
captures belong in `/private/tmp`. The complete release tree is
`chm/cedar/stp-root/Cedar6.1/`.

**Expected load order:** `chm/cedar/stp-root/Cedar6.1/Top/Basic.Loadees`
contains 33 BCDs, starting `RPCRuntime`, `UserProfileImpl`, `IdleImpl`,
`DFPackage`, `BasicPackages`, `InstallerImpl`, then the Viewers and tool
packages.  All real list entries are present under the tree; the final
`Viewers>End.bcd/e` is a LoaderDriver `/e` switch, not a missing file.

**Repro:**

```sh
cd dorado
DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_FTP_TRACE=1 ./build/dorado \
  --snapshot-in /private/tmp/cedar-chs-contig10-current-700m.snap \
  --pilot-disk /private/tmp/CedarDorado-chs-contig10.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --type-at 760000000 --type 'Guest\n\n' \
  --cycles 5400000000 --out /private/tmp/cedar-request.pgm \
  2>/private/tmp/cedar-request.log
```

(The full timeline needs ~5B cycles: End.bcd transfers at ~4.82B, the wild
fault is at 4,828,691,614, the final WDC write at 4,963,123,921. A 2.4B run
stops mid-transfer at TiogaPackage.bcd.)

`make build/dorado build/test_cpu && ./build/test_cpu` and `./build/test_pdi`
pass after the 64-bit/timer changes. The full `make test` still has the
pre-existing `test_ethernet` NetDir reply mismatch. Next diagnostic: the
wild-monitor-lock-pointer capture described at the top of this file (the
earlier "trace the Requeue path around cycle 2015316632" suggestion is
superseded — that store was `MesaFault` legitimately parking a faulted
process on a fault queue).

This document is for the next person (or LLM) picking up this project.
Read it first. It tells you the current state, what's runnable, what's
broken, what to work on next, and the gotchas that cost me hours so
you don't repeat them.

## TL;DR

- **What:** A C emulator for the Xerox Dorado (1978–1985 PARC research
  workstation). Goal: run the original Mesa/Cedar/Lisp/Smalltalk/Alto
  emulator microcode against a Trident disk pack and show pixels on a
  framebuffer.
- **Status:** Microengine + memory subsystem + IFU + tasking + slow-IO
  routing + BaseBoard 6502 model are working. Display + Disk + Fast-IO
  transport have Phase-2 stubs that move data end-to-end. The full
  BaseBoard Boot0 path reaches the BB's Continuous loop.
  `probe_full_boot_with_bootstrap` still substitutes canonical
  Initial.MB at BOOTSTAGE2 because Bootstrap streaming does not yet
  match `chm/Initial.mb`. NOSTORAGE no longer needs a probe bypass:
  Config' now follows `EMemDefs.mc` (`ChipSize` in b12/b13, C bits
  3..2, M0..M3 at `0200/0100/0040/0020`) and reports one present
  64K-chip/4MW module. Initial now gets through PRESETMAP, FINDMODULE,
  BootMem,
  BootEmulator's first-64K clear loop, display/disk init, and the
  100 ms RTClock wait after MapBufBusy, Pipe5, Config, ALU one-bit
  shift, memory-ref FF branch, `Store←T` A/Mar, Junk timer, and
  IFUReset/IFUTest fixes.
  The full probe mounts `spruce-server.dsk300` when present. Because
  the DDC terminal back-channel is not modeled yet, the probe forces
  boot keys up and redirects the remaining false `GotBootKey` case to
  `DiskHardMicrocodeBoot`. Display scanline wakeups now run the
  AHT/DispM terminal task continuously (`display outs=35414`,
  frame 59 snapshot) and keep the boot keyboard words all-up. Disk
  bring-up now forces PilotDisk's one-time normal-mode branch past the
  disabled loop. A CPU Md-latch timing fix lets DSK read the IOCB seal
  correctly, so the full probe now reaches `KSameDrive`,
  `KContinueCmmd`, and `KCheckSeek`. It still does not reach
  `DoDiskBlock` or DiskData; it falls back through the seek/sector
  status path and then spends the probe budget in `Read1Muff`. The next
  blocker is modeling the disk sector/status/muffler path closely enough
  for `WaitForSector` and the sequence-PROM command timing to proceed.
  The full-boot probe currently uses a temporary first-256-page identity
  map shim at `DiskHardMicrocodeBoot` because Initial's real `PresetMap`
  writes still leave the first-64K map vacant at disk boot time.
  Initial's final `mcr.noWake` load appears as `MCR=0xFEE7`; the
  emulator currently special-cases that as normal references enabled
  with only fault wakeups suppressed until the MCR active-low decode is
  corrected from the source docs/schematics. The AEmu bypass now saves
  RBase per task; this fixed an EB direct-load corruption where task 1's
  `RBase=4` made task 0's `Start.mc` read `EORegs.VirtualBanks`
  (`RM[0x48]`) instead of `AEmRegs.EmuBRHiReg` (`RM[0x18]`) and turned
  `BR37=0x20006` into `0x400006`. The current framebuffer snapshot is
  still all white (`display iofetch=0`, low-core `DAStart=0`), so
  display is waiting on software/disk progress rather than failing to
  render FIFO data. The latest disk trace showed Pilot DriveTag words
  like `0x08f0`; bit `0x0800` is KSelect bookkeeping, so DriveTag
  subsector-count decode now masks to the real tag-bus field. A 100M
  full-boot disk trace also proved `Read20Muffs` exits correctly:
  `KTemp0` shifts through `0x8000`, `ALU<0` latches, and the
  conditional branch exits at `0o6605`. The remaining disk question is
  why the boot path keeps re-entering hard/status handling against the
  Spruce pack.
  2026-05-02 update: the natural Initial Ethernet path now receives the
  full `AltoMesaDorado.eb!1`, verifies the zero checksum, runs
  `LoadRam`, and jumps through the EB End item to `0o1076`. This only
  shows up with a larger budget (`DORADO_BOOT_BUDGET=140000000`); the
  older 60M budget stopped in Initial's checksum loop before LoadRam.
  Two late fixes were needed: ALU branch-condition latches are
  task-local, and the EB/LoadRam handoff restores Initial's standard
  ALUFM table because `LoadRam.mc` carries IM/IFUM/RM/End records but
  no ALUFM records. Without the latter, `InitMem.mc:NextMapEntry` saw
  ALUF[4] as `A+B+1` instead of subtract and overran map enumeration.
  The loaded world now fingerprints as Mesa, enters
  `DiskHardMicrocodeBoot`/`DoDiskBlock`, repeatedly finds sectors and
  streams disk records, and low-core through IOBR contains non-blank
  Alto display/disk data. It still does not boot Alto software or
  display useful pixels: IFU arms remain zero and display `IOFetch`
  remains zero. The latest memory change raises the modeled machine to
  four 64K-chip/4MW storage modules. Two modules removed the first
  `RealPages=4000` fault at real page `4057`, but a 140M fault trace
  showed the loaded Alto/Mesa map also using real pages `EA40`/`EAC0`;
  those are valid only on a full `0x10000`-page, 16MW Dorado. Re-run
  the 140M natural Ethernet boot and check whether the high-real-page
  storage faults are gone; if they are, return to the disk/data-transfer
  path and display-list progress.
  Follow-up: with four modules, the 140M probe reports
  `Memory: faults=0`, `RealPages=0x0000` (the 16-bit representation of
  64K real pages), `BR31=0xD240180`, and non-blank IOBR/MDS low core.
  The remaining blocker is again disk/display handoff: DiskMuff/Tag I/O
  is busy, `rd_fifo_tw=1`, but `DiskData` reads are still zero and
  display `IOFetch` is still zero.
- **Repo:** `/Users/alans/Documents/development/Dorado`
- **Most useful entry points to read:** `CLAUDE.md` (project mission),
  `dorado/CLAUDE.md` (code-side guide), `docs/INDEX.md` (doc map).

## Current Working State

- The current Lisp bring-up shortcut is `make run-lisp-current-sdl`.
  It copies `build/good-packs/lisp-lyric-desktop.pack` to
  `build/run-disks/lisp-lyric-desktop-run.pack` and boots that pack with
  `./build/dorado-sdl`. It now also derives
  `build/run-disks/lisp-lyric-usable.sysout` with
  `tools/interlisp-sysout/discard_stale_process.py`; the archived sysout is
  never modified. The SDL command must keep `DORADO_DISPM_PRESENT=1`; its
  initial omission made the interactive target remain in Swat even though
  the validated headless recipe reached the desktop. The target also pins
  `DORADO_FAKE_TIME=1783285880` for deterministic startup.
- Fast startup is now `make run-lisp-snapshot-sdl`; the one-time producer is
  `make lisp-lyric-desktop-snapshot`. The saved 8.8B-cycle desktop restores in
  about 0.3 seconds and requires its matching `.pack`. SDL now accepts
  `--snapshot-in/--snapshot-out` directly. After `make clean`, the producer's
  real `$(LISP_GOOD_PACK)` prerequisite rehydrates automatically from the
  preserved compressed web checkpoint pack.
- `make clean` is `rm -rf build`, so it also removes research packs under
  `build/good-packs`, not just compiler output. The old 19K-current, Harmony,
  and Medley experiments were deleted by the July 10 clean; useful descendants
  still exist under `/private/tmp` (`lisp-cur19k-goodtime.pack`,
  `lisp-harmony-19k.pack`, and `lisp-medley-boot.pack`). None is required by a
  supported run target.
- WebAssembly has a separate wasm32-native checkpoint because raw snapshots
  contain ABI-sized C structs. `web-assets/` holds the 5.6 MB compressed
  snapshot/pack pair; the web boot menu restores it through the Interlisp-D
  Lyric option (`?boot=lisp` is the direct-link/smoke-test form).
- Cedar login is now a validated native and wasm32 checkpoint. The native
  launch is `make run-cedar-snapshot-sdl`; its compressed checkpoint survives
  `make clean`. The browser's Cedar option expands a 447 KB wasm32-native asset
  and restores the keyboard-responsive `Name:` prompt. Mesa NetExec remains the
  next web checkpoint candidate after its capture frame is revalidated.
- The SDL/web keyboard path is working through Interlisp's decoder. A held
  `a` appears in Dorado low core as active-low `175777`; the display-field
  reschedule enters LLKEY's keyboard context; and a baseline/key snapshot
  diff finds the decoded ASCII `0141` newly queued in Lisp VM. Mouse movement
  and left-button transitions also reach LLKEY. The saved desktop still does
  not consume the queued character or alter the framebuffer, even after a
  click in the Exec window, and its Prompt/Exec windows never show an initial
  prompt. A live atom probe shows non-NIL `\\TTY.PROCESS`,
  `\\RUNNING.PROCESS`, and `\\PROCESSES`, so the remaining bug is guest
  process/TTY readiness after startup rather than SDL focus or key mapping.
  `DORADO_LISP_FIND_ATOM` is available as a read-only live-sysout diagnostic.
- The saved-process blocker is now specific. `\RUNNING.PROCESS` is
  `ERIS#LEAF`, not the TTY's Exec process. Leaf spins forever over stale
  `PROCOWNEDLOCKS={75,1730}` after `CREATECELL` reuses that unrooted monitor
  cell. Letting Leaf continue then RAID-errors while decrementing the saved
  `\PROCESSES` root `{57,73730}`, whose reference count is already zero.
  Direct queue skips, cold process-root clearing, and forced rescheduling all
  failed validation and were removed. A correct fix needs a sysout saved with
  processes off or guest-aware process/refcount reconstruction.
- The added `CedarDorado-kitchensink*.pdi` files now live under `CedarDisk/` and
  pass PDI/Pilot structural, checksum, subvolume, and label scans. Each is a
  complete 65,450-page logical volume, not one fragment of a three-disk span.
  All already contain the same germ and byte-identical boot FileID 3 as the
  working small PDI. They enter the installed-system-volume path but remain
  blank (disk 1 through 1.2B cycles), so the next gate is the post-germ volume
  disk bridge rather than adding boot files or mounting a second disk.
- The old Dandelion IOPage shortcut remains available for the pre-IFU AEmu
  path, with `dorado_memory_host_io_write` protecting its RP0 alias. It is no
  longer refreshed after Dorado Lisp enters the IFU: LLKEY explicitly selects
  the Dorado absolute low-core addresses, and clearing a whole device cache
  line's dirty state on every host refresh could discard unrelated guest I/O
  state. The memory regression still proves the IOPage alias does not corrupt
  RP0.
- The SDL frontend now accepts scripted typing options again:
  `--type`, `--type-at`, and `--key-hold`. The shortcut types
  `lisp.run/M {DORADO}LISP.SYSOUT` after boot.
- Verified behavior: the SDL dummy-driver path reaches the Alto/Mesa
  world load and the scripted typing point; the stored good pack was
  already verified to reach the Lisp desktop rather than Swat.
- GitHub `origin/main` and local `main` now match at `a172061` as of
  2026-07-09 (`docs: Interlisp bring-up — Medley 1.0 boots; scar root =
  control transfer into dead stack page`).

## High-level outstanding work (July 2026)

The foundational emulator and both software boot paths work. Alto software
boots over Ethernet, **Cedar 6.1 boots to its Viewers desktop (2026-07-15:
Guest login -> LoaderDriver STP install -> demand-fetched fonts -> live
Clock/CommandTool; `make run-cedar-work`, checkpoint restore via
`make run-cedar-desktop-sdl`)**, and the Lyric and Medley Interlisp-D worlds
reach their desktops. The remaining work is no longer basic boot bring-up;
it falls into five tracks, in this order.

1. **Interlisp desktop usability: Lyric fixed; finish the Medley gate.** The
   saved-record semantics are now identified from `PROC!37`: Lyric's record at
   `{74,101600}` is a 64-word `PROCESS`, and `{74,101700}` is its
   `PROCFX0/PROCFX` sleeping-stack pointer. It is the deleted `RTP` process,
   with both `PROCDELETED` and a surviving `PROCRESETVARSLST`. Hard-reset
   `PROCESSWORLD` preserves it for unwind cleanup, but `\RELEASE.PROCESS`
   dereferences the unavailable rebased `{1,101700}` stack before clearing
   `PROCFX`. `tools/interlisp-sysout/discard_stale_process.py` now performs
   exactly that final `PROCFX=0` state transition on a derived sysout after
   validating the record, deleted state, reset state, and absent stack page.
   A fresh 8.5B-cycle Lyric boot reached a RAID-free Prompt Window + Exec
   desktop (206,668 display-list pixels), and `make run-lisp-current-sdl`
   uses the sanitized derivative by default. The analogous Medley record is
   `{74,075500}`, `PROCFX={74,075600}`; structural sanitation passes, but run
   one full Medley boot before declaring both releases gated. A Lyric
   follow-up delivered `(PLUS 1 2)` after desktop startup but did not visibly
   echo because the headless run never clicked a window; make the eventual
   keyboard gate focus-aware. The older FULL.SYSOUT and Harmony images remain
   incomplete as archived and are not substitutes.

2. **Deepen the Cedar desktop (the login-prompt wall fell 2026-07-15).**
   The cold boot + Guest install now ends at the live Viewers desktop from
   committed artifacts (see `docs/CONTINUE-HERE.md` for the full account —
   the final blocker was `Return` clobbering a same-instruction explicit
   `Link←` load, c25240b). Remaining depth work: (a) the herald's boot
   buttons and RollBack cannot work yet — the work volume's six
   `bootingInfo` slots are all empty (we plant the germ into VM; nothing is
   installed on-disk) — so run Cedar's `Checkpoint` command and/or install
   a germ+boot file into `bootingInfo` to make soft boot/rollback real;
   (b) serve more optional content (EditorComforts and friends) via the
   STP tree; (c) the era-matched `Dorado.germ!4` + `OthelloDorado.boot!8`
   net-boot path still stops after EFTP sequence 1 — compare the old
   germ's receive/CSB layout and re-arm behavior with the 6.1 germ path if
   Othello-built volumes are pursued.

3. **Replace the remaining bring-up shims with hardware-faithful paths.** The
   largest items are the Trident sequence-PROM/FIFO/status/ECC/write path
   (replacing the PDI IOCB-completion bridge), cache-miss/deferred-reference
   Hold semantics, and Cedar's authentic display-task/vertical-field interrupt
   path instead of direct C rasterization and notification. Ethernet boots are
   functional, but wire timing still needs fidelity work such as transmit
   deferral while receiving and collision/completion timing.

4. **Turn diagnostics and edge cases into stable regression gates.** The unit
   suite and primary Alto/Cedar gates are green, but some original PARC
   diagnostics have stale or harness-sensitive expected results. Record exact
   reproducible invocations, resolve the eventCounters/IfuComplex discrepancy,
   and extend coverage to the long memA burn-in and pack-backed TriconD paths.
   Lower-priority C-model gaps include ECC/parity injection, detailed IFU
   pipeline timing, breakpoint/performance-counter behavior, and disassembler
   polish. Alto programs that require writable Alto RAM microcode are expected
   to trap under the real Dorado AEmu and are not emulator bugs.

5. **Begin the synthesizable RTL implementation only after the C timing model
   is a stable contract.** Phase 2 has not started. Hold, I/O sequencing, and
   interrupt/timing behavior should be settled in C first so the Verilog model
   has an authoritative cycle-level reference.

The detailed April/May gap lists below remain useful as implementation history
and as an inventory of low-level fidelity work, but their statements about
Ethernet being absent, Alto not booting, Cedar not loading, or the framebuffer
remaining blank are superseded by this section and `docs/CONTINUE-HERE.md`.

## Build and run

```sh
cd dorado
make           # builds everything in build/
make test      # runs all test binaries; should print "All <X> tests passed."
make clean     # nuke build/
```

C99, no external deps. The `vendor/6502/` dir contains a 6502
emulator (used for the BaseBoard model). `build/mbdis`, `build/mctrace`,
`build/bbtrace` are diagnostic CLIs; the rest are test binaries.
The Makefile uses compiler-generated `.d` sidecars (`-MMD -MP`), so
header edits under `include/` rebuild the affected objects. Last
focused verification: `make build/test_memory`, `./build/test_memory`,
`make build/test_disk`, `./build/test_disk`, `make build/test_cpu`, and
`./build/test_cpu` passed on 2026-04-30. Full-probe disk facts from
the latest run: `KSameDrive@0o6553`, `KContinueCmmd@0o6572`, and
`KCheckSeek@0o6561` are hit; DiskData/FIFO counts remain zero.

## Read these first (in order)

1. **Hardware Manual:** `DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`
   — the canonical reference. Cite section + page numbers in commits and code.
2. **Booting memo:** `DoradoDocs/manuals/Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf`
   — explains the boot sequence at a high level. Path A is BB → Bootstrap
   → Initial → Ethernet; Path B is LoadMB (used by `probe_aemu`).
3. **Project mission:** `CLAUDE.md` (top of repo). Describes the two-phase
   plan (C emulator → Verilog) and working norms.
4. **Code-side guide:** `dorado/CLAUDE.md`. What's built, conventions,
   format findings (the .MB bit shuffle, ALUFM extraction).
5. **The phased plan:** `docs/boot-bringup-plan.md`. Distilled state
   of everything; updated each session.
6. **I/O reference:** `docs/io-systems-architecture.md` (overview),
   `docs/disk-architecture.md` (HM §9 detailed), `docs/display-architecture.md`
   (HM §11 detailed), `docs/memory-architecture.md` (HM §5 detailed).
7. **JCN encoding:** `docs/jcn-encoding.md` — the 7-way JCN field split.

## Repo layout

```
Dorado/
├── CLAUDE.md                       project mission + norms
├── DoradoDocs/manuals/             ★ Hardware Manual + booting memo
├── chm/dorado/                     ★ Dorado microcode + .MB sources from CHM
│   ├── Mesa.mb!3 / Cedar.mb!6      emulator microcodes
│   ├── AEmu.mb!2                   Alto emulator on Dorado
│   ├── doradobaserom.mb!13         BaseBoard EPROM .MB
│   └── expanded/
│       ├── BootstrapSources.dm/    Bootstrap.mc + BootDefs.mc source (BCPL)
│       ├── bootstrap.dm!20_/       Bootstrap.mb + Initial.mb
│       ├── doradobaserom.dm!12_/   BaseBoard 6502 source (.masm)
│       ├── kernel.dm!38_/          shared microcode helpers
│       ├── memMisc.dm!11_/         memory primitives
│       ├── Ifu.dm!51_/             IfuComplex.mb
│       └── ftest.dm!1_/            full Micro→MicroD→.MB worked example
├── AltoInfo/                       Alto/ContrAlto2 references (gitignored)
│   └── Contralto2-2.0-Beta/        ★ TridentDrive.cs etc — port reference
├── docs/                           project documentation (read INDEX.md)
└── dorado/                         ★ the C emulator
    ├── Makefile
    ├── include/                    public headers
    ├── src/                        implementation
    ├── tests/                      test_*.c
    └── vendor/6502/                fake6502 (6502 emulator for BB)
```

## What's built (verified by passing tests)

These all compile clean and pass:

### Microengine (`include/cpu.h`, `src/cpu.c`)
- Full microinstruction decoder (RSTK / ALUF / BSEL / LC / ASEL /
  BLOCK / FF / JCN). Verified against ~25K µinstrs.
- ALU with all 16 ALUFM ops + arithmetic-vs-logical distinction
  (Carry'/Overflow only updated on arithmetic ops, per HM page 30).
- Full JCN: Local Jump/Call, Global Call, Long Jump/Call, Conditional
  (cond 0..6 + Overflow), Subroutine Return, IFU Jump. Regression:
  Initial's `0o7557` long branch (`FF=0o304`, `JCN=0o002`) now targets
  `0o6102`; the previous reversed packing sent it to `0o1310`.
- Shifter (ShC- and FF-controlled, all four mask ops). Bootstrap's
  compact `LDF[T,3,10]` form is handled as a focused special case; it
  extracts the BaseBoard dispatch bits and returns the spread
  BigBDispatch offset.
- Alt-source memory refs (`Store←{Md,Id,Q,T}` /
  `Fetch←{Md,Id,Q,T}`) and LC `Md` destinations (`T←Md`,
  `RM/STK←Md`) are wired. A focused regression covers
  `Fetch←T + RM/STK←Md`.
- FF dispatcher: large subset (TaskingOff/On, Wakeup[task], B←Pipe0..5,
  Pd←ALUFMRW, IFUMLH/RH, PCF←B, IFUReset, BrkIns, etc.).
- Tasking: 16 priority-scheduled tasks, T/TPC/MemBase/Link replicated,
  BLOCK clears wakeup, FreezeBC, Reschedule trap, SubTask OR into
  RBase[2:3]/MemBase[2:3].
- IFU: IFUM 1024×24-bit, prefetch+pipeline, IFUJump dispatch, 4 entry-
  vector slots, NotReady trap, conditional IFUJump.

### Memory (`include/memory.h`, `src/memory.c`)
- 4MW main storage, cache 4×64×16, Map (16K entries × 256-word pages),
  16-entry Pipe, 32-entry BR (28-bit each).
- Refs: Fetch / Store / IFetch / PreFetch / LongFetch / IOFetch /
  IOStore / Map / Flush / DummyRef.
- Faults: page (vacant), write-protect, map-trouble. FaultInfo register.
- ProcSRN / ASRN pipe slot allocation.
- Memory Control Register state for `LoadMcr[A,B]`: DisBR blocks BR
  writes and BR-based virtual addressing, NoRef suppresses storage
  references, FDMiss forces fill/miss behavior, and NoWake suppresses
  fault wakeups.
- MapBufBusy/Pipe5 timing for `Map<-`: per HM §5 and MEMX/MEMC
  schematics, `Map<-` now marks the addressed pipe slot busy for 9
  cycles and `B<-Pipe5` exposes that busy state in the sign bit for
  Initial's `WAITFORMAPBUF` loop.
- `ReadMap` and `Map<-` now share the same 256-word-page map index
  helper, avoiding stale reads from the old `va >> 6` path.
- **No Hold semantics** — refs are atomic. Md is delivered immediately.
  This is a known gap; AEmu's IFU dispatch loop appears to need real
  Hold to stall properly.

### BaseBoard (`include/baseboard.h`, `src/baseboard.c`)
- 6502 + 5 RIOT chips at 0x400/0x480/0x500/0x580/0x600.
- Loads `chm/dorado/doradobaserom.mb!13` as the 64K ROM image.
- Cold boot through CoolBoot → RebootDorado → LoadDoradoCode →
  Continuous (steady-state).
- MCPBus strobes (Control / Clock / ABMux0 / ABMux1 / MIR0..3).
- CPReg ABMux strobes match the BB stream: ABMux1 latches CPRegL,
  ABMux0 latches CPRegH. The old ABMux1 SetSS-as-AMSync shortcut was
  removed; Bootstrap's ReadBB synchronizes on CPRegH after ABMux0.
- MIR injection + SetSS single-step, Run/Halt gate.
- Analog comparators (in-spec voltage/current) so RebootDorado advances
  through SuppliesAllUp.

### Slow I/O routing (`include/io.h`, `src/io.c`)
- Per-(task, TIOA) device callback table. Pd←Input / Pd←InputNoPE /
  Output←B in cpu.c dispatch through it.
- Floating-bus default: 0xFFFF + bad-parity flag (matches HM §7
  page 86 "IOB has bad parity if a nonexistent register is selected").

### Display (`include/display.h`, `src/display.c`)
- 808×606 mono framebuffer, MSB-leftmost packing.
- DDC slow-IO catch-all on tasks DHT/AHT/AWT/DWT.
- State buckets: per-channel NLCB/CLCB (16×12-bit, A and B), HRam
  (1024×3-bit), Mixer (1024×24-bit), PixelClk, Statics.
- Per-channel FIFO (256 words) for IOFetch← munch delivery.
- `dorado_display_render_fifo()` drains FIFO into framebuffer
  (1-bit-per-pixel, MSB=leftmost).
- PGM snapshot helper (`dorado_display_snapshot_pgm`).
- Headless keyboard state is held as five complemented Alto-style
  words (`1 = key up`, `0 = key down`) so SDL can later feed the same
  core API that tests use today. `dorado_display_keyboard_set_key()`
  now exposes the named Alto keyboard matrix, imported from
  `AltoInfo/palo/src/simulator/keyboard.c`.
- Frame count is a core display value advanced at vblank, not by GUI
  repaint. The framebuffer remains available for mid-frame GUI viewing
  and for headless snapshots such as "dump frame 100".

### Disk (`include/disk.h`, `src/disk.c`)
- Trident T-80 / T-300 pack format (ContrAlto2/Bitsavers byte layout).
  Create / load / save / sector access.
- Drive struct: per-drive online/select state, current cyl/head/sec,
  seek-in-progress, index-pulse latches.
- Controller registered on task 14₈, TIOA 10₈-14₈:
  - DiskControl bit decode (HM page 97).
  - Format RAM 16×12 with auto-increment + EnableRun on last word.
  - DiskData 16-word FIFO push/pop.
  - DiskTag dispatch (HM pages 99-101): Drive Select / Head /
    Cylinder / Control with Read/Write/ReZero/HeadAdvance.
  - DiskMuff status readout (KSTATE / KSTAT subset).
- Synthetic `dorado_disk_controller_advance_sector()` helper for
  sector-pulse simulation.

### Fast I/O (`include/fastio.h`, `src/fastio.c`)
- `fast_io_cb` on `dorado_memory` fires on IOFetch / IOStore.
- `dorado_fastio_dispatch` routes by task: DWT/AWT → display FIFO,
  DSK → disk controller FIFO. Synchronous transport (one munch per
  ref, no cycle-accurate timing).
- End-to-end test (`test_fastio.c`):
  - disk pack → FIFO → IOStore(DSK) → main memory ✅
  - main memory → IOFetch(DWT/AWT) → display FIFO → framebuffer ✅

## What works as a **probe** (informational, not pass/fail)

Probes live in `tests/test_cpu.c::main()`. They run real microcode
and report what happens. None of them currently boot a complete
system, but they show how far the model gets:

- **`probe_bootstrap_pure`** — load Bootstrap.MB into IM, no BB, run
  from BOOTSTRAP. Walks 16 init instructions, hits READBB, enters
  spin loop waiting for CPReg. With `cpu.cpreg=0x8000` (pre-set
  AMSync), exits the spin once and proceeds through 0o7746 → 0o7715
  → 0o7702 → re-enters READBB. Demonstrates the bootstrap loader is
  executing real microcode.
- **`probe_initial`** — load Initial.MB + Bootstrap.MB layered, run
  from INITIAL. Initial's first instruction at 0o7500 globally calls
  0o7700 (= READBB in Bootstrap region). Both Initial and Bootstrap
  share IM by design; both depend on the BB CPReg protocol.
- **`probe_full_boot`** — full BB cold-boot + Boot0 jam + Dorado
  free-run from IM. This now reaches `LoadDoradoCode` and the BB's
  `Continuous` loop. The run logged 475 injected Boot0 instructions,
  about 13.3M IM-fetched Dorado cycles, and 64 IM entries written
  starting at 0o7700. The important fixes were correct Write IM
  right-half layout, one-cycle dispatch OR, explicit Link-write
  precedence, and a narrow shifter-source fix for Bootstrap's
  `LDF[T,3,10]` form.
- **`probe_full_boot_with_bootstrap`** — BB drives Bootstrap.MB
  (substituted into IM at first IM-fetch) with its real Boot1 byte
  stream. Bootstrap now runs the complete CPReg stream: 7144 CPReg
  changes after swap, 1792 Write IM half-writes, and 896 unique Initial
  targets beginning at 0o6100. The long-branch fix lets Initial get past
  the old `0o1310` no-code stop and run into its configuration path.
  After the memory-ref FF branch, `Store←T` A/Mar, and Junk timer
  fixes, the probe gets through display/disk initialization and the
  100 ms RTClock wait. It mounts `spruce-server.dsk300` if present,
  attempts `DiskHardMicrocodeBoot`, and falls through to Ethernet.
  Current state: `display outs=3`, `disk outs=32`, disk sector
  wakeups generated, no disk FIFO reads/writes yet, EMU waiting near
  `AWAITETHERBOOTREPLY` (`0o6057`) while DSK can be sampled in its
  idle loop (`0o6775`).
- **`probe_aemu`** — layered load Initial + kernel + memMisc +
  IfuComplex + AEmu, run from STARTEMULATOR. After the long-branch
  packing fix, this bypass now halts after 203 cycles at `PC=0o7777`
  via `JNKINITPC`. It likely needs Bootstrap/Initial support code or
  real main-memory state that the bypass does not currently plant.

## Known gaps — full punch list

Compiled 2026-04-30 from a sweep of `dorado/src/*.c`, `dorado/include/*.h`,
and the narrative sections of this doc and `boot-bringup-plan.md`.
Section letters here are stable references — cite them in commit messages
and follow-up notes.

**Phase-progress index** (updated as gaps land — see
`docs/research-plan.md` for the per-gap plan and `docs/research/<id>-*.md`
for the as-built notes):

- ✅ **Phase 0** — archive sweep + BB ROM disassembler. (`bbdis` tool;
  `chm/disassembly/bb_*.s`; `chm-archive.md` §8.)
- ✅ **Phase 1** — local foundation gaps:
  - **B6** A[12:15]←FF[4:7] override — wired in `cpu.c::ff_a_low_override`.
  - **B7** `ff_full_function_ok` — gating audited correct; comment cleaned up.
  - **C4** dirty-victim WP fault — recorded in FaultInfo; test pinned.
  - **C5** ReadMap-on-Vacant — non-issue (PreFetch silence is per HM).
  - **G1** fast-IO drop counters — added 5 counters to `dorado_fastio_router`.
  - **D1** BB drop counters — added 2 counters to `dorado_baseboard`.
  - **B11** breakpoint/EventCnt/parity state slots — round-trip works;
    trap/tick semantics deferred until `Kernel.Press!6` is transcribed.
- ✅ **Phase 2** (research-complete; A1 implementation deferred):
  - **D2** ✅ BB ROM disassembled with 76 labels from the embedded
    symbol table; `bbdis --hunks` decodes Boot0 hunks. Annotated dumps
    in `chm/disassembly/`.
  - **A2** ✅ re-diagnosed: Boot0 (= Bootstrap) does **no** slow-IO.
    The original "Pd←Input returns 0" theory was wrong (FF=0o262 in
    Boot0 is `Pd←ALUFMRW`, not TIOA).
  - **B2 / H2** ✅ closed by A2 re-diagnosis — no slow-IO devices
    needed for Boot0.
  - **B8** ✅ research-only: spec captured in
    `docs/research/B8-rwcpreg-polarity.md`. Code change deferred —
    must land with A1 fix.
  - **A1** ✅ **RESOLVED 2026-04-30**: streaming was never broken.
    The 95/896 "match" rate is because the BB ROM Boot1Data is a
    DIFFERENT BUILD of Initial than the
    `bootstrap.dm!20_/Initial.mb` archive copy. `bbdis --hunks $C016
    224 0o6100` produces a bit-exact match against what our streaming
    delivers. BOOTSTAGE2 substitution shim is an intentional probe-
    time feature, not a bug workaround. See
    `docs/research/A1-bootstrap-streaming.md` "RESOLUTION" section.
- ✅ **Phase 3** (research-complete; B1+C3 implementations deferred):
  - **C6** ✅ MCR decode bit positions cross-checked against
    `EMemDefs.mc`. Fixed `dorado_mcr_disbr`. Added
    `dorado_mcr_dishold` getter. The 0xFEE7 special-case stays
    until B1 lands.
  - **C2** ✅ Per-slot Pipe4 error bits modeled.
    `dorado_pipe4_at(mem, srn)` composes the high-true value from
    per-slot state and XORs with the `0o150361` baseline. New
    `dorado_pipe4_set_error` API. The dirty-victim WP fault from C4
    now records `MAP_TROUBLE` in the triggering slot's Pipe4. Two
    new tests pin the encoding.
  - **B1/C1** ⏳ research note in `docs/research/B1-C1-hold-semantics.md`
    captures the spec from `InitialSubrs.mc` + `InitialMain.mc`.
    Implementation deferred — substantial architectural change.
    Note: boot-stage microcode runs with `mcr.disHold` set and
    uses cycle-counted waits, so it doesn't depend on Hold; only
    post-boot emulators (probe_aemu) require it.
  - **C3** ⏳ research note in `docs/research/C3-ecc.md`. ECC
    polynomial documented (HM §5.12). Deferred — nothing in our
    current test inventory exercises ECC.
- ⏳ **Phase 4** disk (research-complete; F1-F5 implementation
  blocked on disk content):
  - Pulled canonical disk sources: `DiskDefs.mc`, `DiskSubrs.mc`,
    `DiskBootSoft.mc`, `DiskBootTransfer.mc`. Findings in
    `docs/research/F-disk-phase3-sweep.md`.
  - Two real issues identified:
    1. **DiskMuff wire format discrepancy** — DiskDefs.mc spec has
       muffAddr in low byte and clear bits at LSB 8-11; our model
       and tests use high byte for addr and LSB 0-3 for clears.
       Need a per-cycle disk trace to reconcile.
    2. **Boot-stage disk failure is content, not model**: the
       hard-microcode file InitialDisk reads (cyl 0, head 0,
       sector 4) doesn't exist on `spruce-server.dsk300` (which
       is an Alto Spruce pack). Same diagnosis as A1: the model
       is correct; we lack canonical content. Workaround: Path B
       (Ethernet) — see H1.
- 🔢 **B3 sub-items landed** in Phase 7: PCX', DBuf. Remaining:
  XorCarry, ModStkPBeforeW, MidasStrobe, RestoreStkP,
  Hold&TaskSim, WF/RF, EventCntA-write,
  RMaddr replace force-write (FA=0/2 FB=4-5 / FB=2-3),
  MemBX, UseDMD, FreezeBC, LoadTestSyndrome, Multiply.

### A. Showstoppers blocking real boot

A1. **Bootstrap → Initial CPReg streaming corrupts data**
(`probe_full_boot_with_bootstrap`). ~768 of 896 IM half-writes do not
match canonical `Initial.MB`; LH writes default to `0x0044`, iw2
RSTK[0]/JCN[7] bits flip (e.g. `IM[0o6347]` lands as `local→0o6347`
self-loop instead of `local→0o6346`). Currently bypassed by substituting
canonical `Initial.MB` at BOOTSTAGE2. Suspected causes: shifter LSH/LDF
mis-execution in Bootstrap's T-composition; a different Boot1Data layout
in the BB EPROM vs. `Initial.mb`; CPReg-byte ordering between BB
ABMux0/ABMux1 and Bootstrap's T-decode. See §2a below.

A2. **`probe_full_boot` (BB Boot0) halts at 0o7744.** BB ROM contains a
*newer* compiled Boot0 than `Bootstrap.MB`. Reads slow-IO inputs
(`Pd←Input`/`Pd←InputNoPE`) we don't model, so ALUFM[N] from the cobweb-
clear NOPs comes back zero (`boot-bringup-plan.md` Phase A.7).

A3. **Hard-disk boot doesn't transfer.** Initial reaches `KSameDrive`,
`KContinueCmmd`, `KCheckSeek`, `KWAITSECTOR`, `WAITFORSECTOR`, then times
out at `Read1Muff` (`0o6500`). `DiskData` reads/writes stay zero. The
mounted `spruce-server.dsk300` is a Spruce Alto pack and probably doesn't
contain Initial's private "page 4" Dorado microcode file. No Trident pack
with the right contents is available.

A4. **Probe-only workarounds still live in `tests/test_cpu.c`:**
first-256-page identity-map shim at `DiskHardMicrocodeBoot`; canonical
`Initial.MB` swap at BOOTSTAGE2; `ETemp0..3` forced all-up + `GotBootKey`
redirect to `DiskHardMicrocodeBoot`; special-case decode of
`mcr.noWake = 0xFEE7`.

### B. Microengine (`src/cpu.c`, `include/cpu.h`)

B1. **No Hold semantics.** Memory `Md` is delivered immediately; real
hardware stalls the engine on miss (~28 cycles), on Pipe full, on
FreezeBC, on `StkError`, on IFU map fault concurrent with IFUJump.
AEmu's `LRTYPETABLE`→`LRTYPEIM`→`LRLOOPTOFF`→`TOFFRET` loop spins forever
because of this. (`include/cpu.h` HM §4 Hold; `include/memory.h` line 13.)

B2. **`Pd←Input` / `Pd←InputNoPE` slow-IO read FF ops** stubbed for many
devices (Hold/TaskSim register, MCR, Reset register). Boot0 needs them.

B3. **FF table gaps tagged "stub: silently honor" in `cpu.c`:**
- FA=0 FB=2 FC=4 `XorCarry`, FC=5 `XorSavedCarry`, FC=6 `Carry20`,
  FC=7 `ModStkPBeforeW` (line 627).
- FA=0 FB=4-5 RMaddr replace + force-RM-write (line 715).
- FA=2 FB=2-3 RMaddr replace + force-RM-write (line 915).
- FA=2 FB=5 FC=4..7 `MemBX←FF[6:7]` (line 932).
- FA=1 FB=4 FC=0 `UseDMD`, FC=1 `MidasStrobe`, FC=5 `RestoreStkP`.
- FA=1 FB=5 FC=4 `Hold&TaskSim`, FC=5 `WF←A`, FC=6 `RF←A`.
- FA=0 FB=7 FC=2 `Multiply`, FC=5 `TgetsMd`, FC=6 `FreezeBC`.
- FA=1 FB=2 FC=5 `LoadTestSyndrome`.
- FA=1 FB=3 FC=1 `EventCntB←B`; FA=1 FB=7 FC=1/4 `EventCntA'/B'` return 0.
- FA=1 FB=7 FC=0 `B←PCX'` returns 0; FC=5 `B←DBuf` returns 0.

B4. **Divide / CDivide** placeholders return Pd unchanged
(`cpu.c` lines 1055-1057).

B5. **Read IM** stubbed — advances PC but doesn't deliver inverted IM
bytes on B (`cpu.c` line 2046).

B6. **A[12:15] ← FF[4:7] override at A-bus time** not wired; FA=0 FB=0/1
silently no-ops (line 614).

B7. **B-source override gating** can be missed — `ff_full_function_ok`
doesn't always catch every BSEL constant case (TODO at line 450).

B8. **`B←RWCPReg` polarity asymmetry.** Inverts only on the legacy-stub /
single-step path; during BB free-run returns the raw value because
Bootstrap otherwise composes `Loc` with bit 15 set. Needs hardware-correct
fix once BB ROM Boot1Data layout is understood (line 542).

B9. **IFU pipeline simplifications** (`cpu.h` lines 92-96): no F/G→J→H→M
staging, single-cycle IFUJump, no Hold + IFU-map-fault interaction; only
InsSets 0/1's byte ordering modeled (cpu.c line 1579).

B10. **Reschedule / RescheduleNow** branch condition uses pending-wakeup
as a proxy for non-emulator tasks; device-driven IOAttention not wired
(line 1530).

B11. **Breakpoints / parity / performance counters** partially modeled:
`BrkIns←B` sets `BrkPending`, and `IFUReset` now clears the pending
breakpoint/BrkIns state while preserving `InsSet` per HM Table 20.
The actual breakpoint substitution/trap path is still not modeled.

### C. Memory subsystem (`src/memory.c`, `include/memory.h`)

C1. **Atomic refs only.** No deferred references, no Hold, no cycle
counter (header line 13).

C2. **Per-slot Pipe4 error fields** not tracked beyond the constant
`0150361` baseline (cpu.c line 506).

C3. **ECC absent** — no syndrome generation, no double-bit error reporting.

C4. **WP fault on store** swallowed silently instead of asserting a real
WP fault (`memory.c` line 475).

C5. **`ReadMap` walks the map silently on Vacant** instead of generating
a map fault (`memory.c` line 614).

C6. **MCR active-low decode incomplete.** `mcr.noWake = 0xFEE7`
special-cased; full DisBR/DisCF/NoRef/FDMiss/UseMcrV semantics need to be
re-derived from schematics.

### D. BaseBoard (`src/baseboard.c`)

D1. BB 6502 unmapped offsets and EPROM writes silently dropped
(`baseboard.c` lines 223, 363) — should at least log; might mask a real
ROM fault.

D2. **No disassembly of the BB EPROM Boot0/Boot1Data** vs. `Bootstrap.mb`
— needed before A1 and A2 can be debugged with ground truth.

### E. Display (`src/display.c`, `include/display.h`)

E1. **`display_output_b` lacks per-(task,tioa) dispatch.** TODO at line
183: NLCB load, HRam load, Mixer load, PixelClk, Statics are collapsed
into one catch-all that only tracks NLCB/CLCB write counts plus the
WCB-flag protocol.

E2. **DDC input returns idle key word always** — no 7-wire terminal
back-channel, no keyboard message decoder, no boot-key selection
(`display.c` line 187). No way to select an emulator
(Mesa/Cedar/Lisp/Smalltalk/Alto) at boot.

E3. **No pixel clock / waveform / mixer** — `dorado_display_render_fifo`
assumes 1-bpp Alto mode and dumps the FIFO straight into the framebuffer
(header line 289).

E4. **DDC catch-all is registered on every TIOA** for tasks 3/4/011/013
(`display.c` line 220) — broad; refine once specific (task,TIOA) pairs
are observed.

E5. **Scanline timing is a synthetic shim** — real pixel-clock and HBlank
waveforms aren't driven from the actual control outputs (`display.h`
line 273).

### F. Disk (`src/disk.c`, `include/disk.h`)

F1. **Phase 3 sequence-PROM execution missing.** Read tag short-circuits
and dumps header+label into the FIFO contiguously instead of stepping
through preamble/sync/data/ECC/postamble per the read PROM (HM page 99).

F2. **Fire Code ECC absent** (`P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1`).

F3. **Write side stub:** sets `WrFifoTW=1` and marks active; nothing
commits the FIFO back to the pack (`disk.c` line 506).

F4. **Real sector-pulse timing → wakeup generation** is synthetic —
`dorado_disk_controller_advance_sector()` is poked manually from probe
code, not driven from a clock model (`disk.h` line 47).

F5. **Status readout partial** — KSTATE/KSTAT subset only;
`RdFifoTW` thresholds, block-mode status, ECC words, end-of-block
`ReadErr`/`WriteErr` summary bits not modeled.

F6. **Disk command start/read FIFO path** — native DiskTag strobe decode
is now in place, but the focused boot trace still reaches
`Read20Muffs` status handling before any `DiskData` FIFO read. The next
target is the `KCmmdInTime`/`DoDiskBlock` path and the DiskControl
leading-edge command start / `RdFifoTW` threshold behavior.

### G. Fast I/O (`src/fastio.c`)

G1. IOFetch/IOStore data destined for unrouted tasks is silently
discarded (line 38).

### H. I/O routing / devices

H1. **No Ethernet controller.** Now the next visible blocker after
hard-disk boot fails (Path A's Initial-fetches-emulator step).
`EtherMicrocodeBoot` falls through. A gated probe-only injector exists
for `.eb` images but isn't a real Ethernet model.

H2. **MCR / Hold/TaskSim / Reset register** not wired as slow-IO devices.
Returning floating-bus 0xFFFF + bad-parity for these is what blocks
Boot0 (A2 above).

### I. Tests / probes

I1. `probe_aemu` halts at `PC=0o7777` via `JNKINITPC`. Needs either
planted Mesa state (frame/MDS/context layout from Pilot docs) or Hold
semantics (B1).

I2. Probes carry environment-driven side paths (`DORADO_ETHER_BOOT_IMAGE`,
`DORADO_BOOT_SNAPSHOT`) — bring-up scaffolding; should be replaced or
migrated to real fixtures.

### J. .MB loader / disasm coverage

J1. Only **DMachine == 2 (model-1)** storage shuffle is exercised. Model
0 (3-word, no shuffle) and D0 (DMachine == 0) paths exist nominally but
are untested (`dorado/CLAUDE.md` "Model-1 storage bit-shuffle" section).

J2. Disassembler polish deferred: sharper FF/JCN sub-decoding, ALUFM
cross-reference, `.DLS`-format `--listing` mode.

### K. Phase-2 (Verilog) prerequisites not yet satisfied

K1. Cycle accuracy still skipped in many places (atomic refs, single-
cycle IFUJump, no Hold, synthetic disk timing). Each will need to land in
C before the RTL port has anything to mirror.

### Highest-leverage gaps (suggested ordering)

1. Disk sector/status path through `WaitForSector`/`Read1Muff`
   (F1–F5).
2. A real or controlled Ethernet boot path so `LoadRam` can pull in
   emulator microcode (H1).
3. Bootstrap streaming corruption — verify BB ROM Boot1Data layout vs.
   `Initial.mb` (A1, D2).
4. Hold semantics in the memory subsystem (B1, C1) — needed for AEmu
   and likely for any post-boot Mesa work.
5. `Pd←Input`/`Pd←InputNoPE` plus the slow-IO devices the BB ROM Boot0
   polls (B2, H2).

## What's NOT working (the actual bring-up gaps)

Listed in priority order. These are the next concrete tasks.

### 1. AEmu bypass needs refreshed setup

`probe_aemu` used to cycle in LRTYPETABLE/LRTYPEIM, but after the
long-branch packing fix it halts earlier at `PC=0o7777` via
`JNKINITPC`. The bypass probe is no longer the best immediate path:
it has blank main-memory tables and does not currently layer or emulate
all support code that real Initial/Bootstrap would provide. Two paths
to make it useful again:

- **Plant valid Mesa state.** The structure of the frame, MDS
  (Memory Descriptor System), context info, etc. would need to be
  extracted from Mesa documentation. Then the probe pre-loads memory
  with that state. This requires reading Pilot/Mesa documentation
  in `chm/dorado/expanded/` to figure out the layout.
- **Implement Hold semantics.** In our model, Md returns immediately
  with stored data (0). On real hardware, when microcode references
  Md before the read completes, the engine *holds*. AEmu's loop
  uses Hold to wait; without it, the engine plows ahead with stale
  Md and loops forever. This means our memory subsystem needs a
  cycle counter and a `pending_md` queue. See HM §5 / Figure 9.

### 2. Full BB→Bootstrap→Initial path now runs the boot loop (with workarounds)

**Latest status (2026-04-29 late):** With canonical-Initial
substitution at BOOTSTAGE2 plus kernel.mb/memMisc.mb/IfuComplex.mb
layered under Initial.mb to provide fault-task and helper microcode,
Initial runs through a full setup sequence:

1. INITIAL (0o7500) → READBB for checksum (0o7700) → INITIAL1 (0o7501)
2. WRITEALUF table init (writes 16 ALUFM entries via WRITEALUFTABLE)
3. RMINITL (0o6102) — RM/STK init iteration (multiple passes)
4. IFUMINITL (0o6145) — IFUM init
5. PRESETMAP / RESETMAPL / WRITEMAP / WAITFORMAPBUF — Map init
6. CLRCACHEFCOLL — cache flush
7. SETBRFORPAGE — BR setup
8. NOSTORAGE test passes via real `B←Config'` (one 4MW storage module)
9. FINDMODULE (0o6357) advances after FA=2/FB=7 ALU shift support
10. Initial reaches and exits the BootMem wait loop at `0o6116`
11. BootEmulator clears the first 64K at `0o6226`
12. Display init runs (`DisplayInitConfig`, `THTInitPC`, `THTInit1`)
    and starts DDC slow-I/O outputs
13. Disk/Junk task setup runs; the Junk timer advances RTClock and
    exits the old post-init wait around `0o6205`
14. Initial attempts `DiskHardMicrocodeBoot`, then falls through to
    Ethernet boot wait because the hard-disk boot path has not yet
    produced a successful transfer

State at end:
- 80M run with `spruce-server.dsk300`: EMU is waiting near
  `AWAITETHERBOOTREPLY` (`0o6057`); with DSK wakeups active the
  sampled final PC is often DSK idle (`0o6775`)
- display outs=3, disk outs=32, disk sector wakeups generated,
  disk FIFO reads/writes=0
- `tasking_on=1`; `wakeup_pending` may include DSK when sampled
- Memory: faults=15, `first_srn=0`, `Mar=0xFE21`
- Initial variables: `R400=0x0100`, `RNUM=4`, `REALPAGES=4`,
  `DISPLAYCONFIG=0xFFFF`

**Underlying issues, in priority order to fix:**

#### 2a. Bootstrap streaming corrupts data (still unfixed; bypassed via substitution)

Original investigation showed:

**Of the 896 entries Bootstrap writes, only ~94 match the canonical
Initial.MB. The other ~768 are CORRUPTED.** Specifically the LH writes
(iw0) frequently land as `0x0044` (a near-default decode of
`RSTK=00 ALUF=00 BSEL=RM/STK LC=NoLoad ASEL=A←RM/STK`). The RH writes
(iw1) sometimes land correctly (1 of the 5 dumped addresses matched).

For `IM[0o6347]` specifically:
- LOADED: `iw0=0o000104 iw1=0o017723 iw2=0o040000` → `JCN=0o247(local)` → self-loop to `0o6347`
- CANON:  `iw0=0o051164 iw1=0o017723 iw2=0o100000` → `JCN=0o246(local)` → jump to `0o6346` (`READTERMINALRET`)

iw1 happens to match by coincidence. iw0 differs (default-ish vs real
`RSTK=12 ALUF=11 LC=RM/STK←Pd`), and iw2's `RSTK[0]/JN1bit7` bits are
flipped. The local-jump target offset becomes `0o247` instead of `0o246`,
so the instruction self-loops via the corrupt low bit.

Diagnostic capture (in `test_cpu.c`'s probe with
`cpu.dbg_writeim_log = 1` enabled at swap):

```
#00 pc=0o7720 addr=0o6100 half=LH sec=0 b=0x0044 T=0x0044
#01 pc=0o7724 addr=0o6100 half=RH sec=0 b=0xE682 T=0xE682
#02 pc=0o7722 addr=0o6101 half=LH sec=1 b=0x0044 T=0x0044   ← T=0x0044 again
#06 pc=0o7720 addr=0o6103 half=LH sec=0 b=0x1286 T=0x1286   ← varied data
#14 pc=0o7720 addr=0o6107 half=LH sec=0 b=0x5274 T=0x5274   ← canon iw0 of 0o6347!
```

So canon iw0 of REAL `0o6347` (= `0x5274`) appears in the byte stream
at WRITE position 7 (= addr `0o6107`), not at position 167 (= addr
`0o6347`). This rules out "BB streams in real-address order".

Hypotheses for the root cause:

1. **Bootstrap's `LSH[T,10]` + `LDF[T,10,0]` + `T XOR Byte1` T-composition
   misexecutes** in our cpu.c shifter. The Dorado does this via the
   barrel shifter and ALUF=`A XOR B`. The right answer would be the
   16-bit data byte for that real address; we get something off.
   Investigate by stepping through one full BootByteL iteration and
   comparing T bit-by-bit against expected.
2. **The BB ROM Boot1Data is not in real-address-sequential order.**
   `chm/dorado/expanded/bootstrap.dm!20_/Initial.mb` may not be the
   same layout the BB ROM has — the BB ROM is a separate build from
   1987. Disassemble Boot1Data from the BB ROM (C000-D7FF) and decode
   manually to verify the byte format.
3. **CPReg-byte ordering between the BB and Bootstrap doesn't match
   the Type-0/Type-1 packet format described in `BootstrapMain.mc`.**
   Our BB pushes `ABMux1` (low byte) first, then `ABMux0` (high byte).
   During the Boot1 stream the first ABMux0 write clears CPRegH and the
   second writes `(MicroHalf << 1)|extra_bit`, creating the Dorado-visible
   data-ready transition. Bootstrap reads `T←~CPReg` once and decodes
   via `LSH[T,10]/LDF[T,10,0]`. If the CPReg layout has the byte data
   in a different position than Bootstrap expects, every T composition
   is shifted/garbled.

The trace shows that the SECOND ReadBB (which provides the dispatch
+ right-half-byte) drives the 3-bit dispatch (`BTemp`) into a
LH-or-RH selector that does seem to alternate correctly (we get pairs
of LH+RH writes per address, with `secondary` varying — not stuck at
sec=0). So the dispatch decode is at least partially right. But
the data-byte composition into T is wrong in the LH writes.

Easiest debugging approach: capture a focused per-cycle trace of the
microengine through ONE full BootByteL iteration (from Cnt-test back
to Cnt-test). Log T before each instruction, the result of LSH[T,10],
LDF[T,3,10], LDF[T,10,0]. Compare to what Bootstrap.MB intends.

To enable the post-swap Write IM log used to find this:
```
cpu.dbg_writeim_log = 0;          // off pre-swap
// at swap:
cpu.dbg_writeim_log = 1;
cpu.dbg_writeim_n = 0;             // reset buffer (256-deep)
```
The struct fields are in `include/cpu.h` near the bottom of
`dorado_cpu`. The Write IM trace in `src/cpu.c` is in the `fn == 7`
arm of next_pc.

Also note: `probe_initial` (which directly loads Initial.MB without
streaming) gets stuck differently — INITIAL at 0o7500 does
`Call[ReadBBLoc=0o7700]`, which spins because no BB is providing
CPReg data. So even with correct microcode placement, INITIAL itself
won't run without a working CPReg byte stream from BB.

#### 2b. NOSTORAGE fixed by Config' storage-present response

Initial computes a value via shifter ops at 0o6041..0o6277, stores in
`RM/STK[RBase*16+8]`, then at 0o6210 tests
  `RM/STK[RBase*16+8] AND 0xF000`. If zero, branches to NOSTORAGE
(0o6247).

Fixed: `B←Config'` now comes from `dorado_memory_config_word()` instead
of hard-coded `0xFFFF`. `EMemDefs.mc` defines `ChipSize` in b12/b13
(C bits 3..2) and present-module bits as M0=`0200`, M1=`0100`,
M2=`0040`, M3=`0020`; for bring-up the C model reports `ChipSize=3`
and one present 64K-chip/4MW module. With this, the full boot probe no
longer hits the NOSTORAGE bypass; Initial reaches `FINDMODULE`
naturally.

The old probe-side `0o6247 → 0o6357` bypass remains in
`test_cpu.c`, but it no longer fires in the normal run.

#### 2c. PRESETMAP / WAITFORMAPBUF loop (fixed enough for boot path)

After Config' was implemented with storage-present reporting, Initial
entered `FINDMODULE` but spent the budget in map initialization rather
than the old display `LONGWAIT` path. That was temporarily worked
around with a small-module report; current bring-up reports the real
64K-chip/4MW module layout from `EMemDefs.mc`. The earlier 60M-cycle
hot loop was:
`WRITEMAP(0o6340) → 0o6365 → WAITFORMAPBUF(0o6360) → 0o6245 → 0o6244
→ 0o6366 → WAITFORMAPBUF → 0o6245 → 0o6244 → 0o6367 → DORETURN →
RETN → PRESETMAPE/PRESETMAPL → SETBRFORPAGE → ...`.

After adding MapBufBusy, the first cache-address flag model, switching
to HM Table 16's 16K-entry x 256-word page map geometry, correcting
Pipe5 cache flags to manual bits 8..11, and reporting the smaller
module geometry, the normal 60M probe gets past map setup and into
BootEmulator/display/disk initialization.

`LoadMcr[A,B]` is now real enough to cover the bits Initial appears to
use first (dVA<-Victim, DisBR, DisCF, NoRef, FDMiss, UseMcrV,
NoWake), and `CFlags<-A'` plus the cache-address-section portion of
`B<-Pipe5` are modeled at a basic level, including Victim/NextVictim.
This is no longer the top blocker. The remaining bug was not in
MapBufBusy: Initial's FINDMODULE path uses HM Table 11d ALU one-bit
shifts (`FA=2, FB=7`, especially `Pd←ALU lsh 1` at 0o6357). The old
stub returned the unshifted ALU output, so the module/page scan never
advanced correctly. `cpu.c` now implements rsh/rcy/brsh/arsh/lsh/lcy,
and `test_alu_shift_ff_functions` pins them down.

#### 2d. BootMem / BootEmulator memory-reference loops (fixed enough for display start)

The old park at `LWRETN`/`LONGWAIT` was caused by two processor
decode/modeling gaps:

- `FF=0o363` on a memory reference means Store plus the Table 13
  low-six-bit branch condition `Cnt=0&-1`; it is not the full Table 11
  `Wakeup[3]` side effect.
- `ASEL=2/3` alternate memory references must drive the selected
  source onto A and Mar. `Store←T` was previously using A=0, so
  BootEmulator's first-64K clear loop recomputed `T=1` forever.

With those fixed, the 64K clear loop at `0o6226` runs for 65,537 hits
and exits to display initialization. The later `0o6205` RTClock wait
is also fixed by modeling Junk task timer wakeups. Current top blocker
is now the boot-media path: hard-disk microcode boot is attempted but
does not transfer sector FIFO data, then Initial waits for Ethernet
boot replies.

### 3. Disk Phase 3: real timing + Fire Code ECC + sequence PROMs

For booting an actual Alto OS, Mesa needs the disk to sequence
through real read operations (preamble + sync + data + ECC + post-
amble) per the read PROM (HM page 99). Currently our Read tag
short-circuits and dumps header+label into the FIFO contiguously.

For a basic boot this might be enough — Mesa probably trusts the
Format-RAM-determined timing and our short-circuit might satisfy
its expectations. Try it before implementing the full PROM
sequencer.

The Fire Code ECC (`P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1`)
might also be skippable for boot (Mesa might accept successful-read
status without verifying ECC). Implement only if you observe Mesa
faulting on missing ECC.

### 4. Display Phase 3: pixel clock + waveforms + 7-wire interface

Lower priority unless you're trying to get keyboard/mouse working
(the back channel is on the 7-wire interface). For pure framebuffer
output via DWT, Phase 2 is sufficient.

### 5. Hold semantics in memory (touches #1)

If you take path #1 (plant Mesa state) you may not need Hold. If
you take path B, Hold is essential. See HM §5 for timing.

## Most important pitfalls I hit

In rough order of "hours wasted":

### The ALUFM bit-mapping trap (FIXED, but worth knowing)

The `Pd←ALUFMRW` operation writes a 6-bit ALUFM entry from B per
HM Table 11d: `ALUFMEM ← B.8, B[11:15]`. The manual's bit
positions are **MSB-first**; our C uses LSB-first. The mapping
is **NOT** the obvious one. The correct C code:

```c
uint8_t alufm =
    (uint8_t)((((b >> 7) & 1) << 5) |   /* B[8] manual = B_C[7] → entry top (carry) */
              (((b >> 4) & 1) << 4) |   /* B[11] manual = B_C[4] */
              (((b >> 3) & 1) << 3) |   /* B[12] manual = B_C[3] */
              (((b >> 2) & 1) << 2) |   /* B[13] manual = B_C[2] */
              (((b >> 1) & 1) << 1) |   /* B[14] manual = B_C[1] */
              (((b >> 0) & 1) << 0));   /* B[15] manual = B_C[0] (op LSB) */
```

The wrong (mirrored) version is what I had originally. It made
Bootstrap.MB's runtime ALUFM init silently corrupt the table,
which made a downstream conditional jump land in a trap slot,
which made Boot0 appear to be broken when really it was running
correctly against corrupted state. Test: `test_alufmrw_bit_mapping`
in `tests/test_cpu.c`.

### CPU_QUADRANT_SIZE (FIXED)

`include/cpu.h` had `CPU_QUADRANT_SIZE = 0o4000` (= 2K). Per HM §4.3,
a quadrant is 4K-word = the whole IM today. Was 0o10000 (= 4K).
Wrong size → Global Calls / Long jumps to addresses outside 12-bit
IM range. This fixed one class of fictional-address jumps; later the
long-branch packing fix exposed a separate `probe_aemu` bypass issue at
`PC=0o7777`.

### Long branch packing (FIXED)

HM Figure 6 says Long Jump/Call forms TNIA as
`CIA[2:3] || FF[0:7] || JCN[4:7]`. In our 12-bit IM address that means
`(FF << 4) | (JCN & 0xF)`, not `(JCN << 8) | FF`. The regression is
Initial.MB `0o7557`: `FF=0o304`, `JCN=0o002` must branch to `0o6102`
(`RMINITL`), not `0o1310`.

### B←RWCPReg normally returns ~CPReg, with a current boot-path caveat

Per HM page 31: `B←RWCPReg = Link←B, B←CPReg'`. The prime denotes
inversion. So microcode reading CPReg always sees the complement.
The current cpu.c still does this for the legacy-stub path and while
the BaseBoard is single-stepping/IRTable setup. During Dorado
free-run, it currently exposes the raw BB CPReg stream; that is a
pragmatic boot bring-up compromise that lets Bootstrap receive the
BB stream correctly, but it should be rechecked against the hardware
manual once the `0o6347` Initial loop is understood.

**Several Bootstrap mysteries trace back to this** — when reading the
source, watch for whether the surrounding code accounts for the
inversion.

### Stale incremental builds

Make does NOT always pick up changes when `cpu.h` fields are
added/removed. Symptom: tests fail with bizarre values like
`cycles=1806855336`. **Always `make clean && make` after touching
struct definitions.**

### The BB-loaded Boot0 is NOT Bootstrap.MB

`chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb` is the source
form. The BaseBoard EPROM contains a *newer* compiled-and-packed
version that takes a different code path through the trap
reservations. They share the entry point (0o7740 = BOOTSTRAP) but
the bytes differ. Don't conflate the two.

### Octal in C source

C accepts `0` prefix for octal, NOT `0o`. (I keep typing `0o`
out of habit from Python/Rust.) Use `025` not `0o25`.

### `probe_aemu`'s memory mapping

The probe maps 16 pages identity-RW. AEmu's memory references
then succeed (no faults) but read 0 (unwritten storage). To make
AEmu progress, plant data at the addresses it reads from, OR
implement Hold so the engine stalls instead of looping with stale Md.

## Coding norms

From `CLAUDE.md`:
- C99, no external libs except the vendored 6502 in `vendor/6502/`.
- Octal where the manual uses octal. Microinstruction addresses,
  IM contents, RM values — all octal.
- Symbol names mirror the manual: RSTK, ALUF, BSEL, LC, ASEL, FF,
  JCN, BLOCK, IM, IFUM, ALUFM, RM, STK, BR, MemBase, TPC, Pd, Md, Mar.
- Cite HM section + page numbers in non-obvious code: `// HM §3.7
  Table 9`. Cite MicroD source when touching .MB format.
- No emojis in code or docs.
- **Treat the Hardware Manual as canon.** When sources disagree,
  prefer the September 1981 Hardware Manual unless deliberately
  tracking a later revision.
- **Don't invent behavior.** If a microcode source uses a feature
  you haven't implemented, find the manual passage describing it
  before writing code.

## Test patterns

When fixing a bug you found via a probe, write a *focused* test
that:
1. Sets up minimal microcode that triggers the bug.
2. Verifies the corrected behavior.
3. Goes in the matching `tests/test_*.c` file.
4. Has a clear PASS/FAIL output.

Examples already in the tree:
- `test_alufmrw_bit_mapping` — uses 1 microinstruction to exercise
  the Pd←ALUFMRW mapping.
- `test_carry_preserved_on_logical` — 3 microinstructions verifying
  HM page 30 ("Carry' is the result of the last *arithmetic* op").
- `test_alufm_canonical_decoding` — verifies the standard ALUFM
  convention is recovered from real .MB files.

This way the bug stays fixed even if someone refactors later.

## CHM (Computer History Museum) archives

The richest archive is at `xeroxparcarchive.computerhistory.org`,
specifically `_cd8_/dorado/` and `_cd8_/doradosource/`. URL
convention is in `docs/chm-urls.md`. To grab a source file:

```sh
curl -sO "https://xeroxparcarchive.computerhistory.org/_cd8_/doradosource/BootstrapSources.dm!12_/BootstrapMain.mc"
```

Drop the file in `chm/dorado/expanded/<DirName>/` and refer to it
in code/comments.

The cross-reference is at `chm/cross-reference.html` (568K lines —
grep it; never paginate it). Maps every PARC IFS file to its
archive location.

## Active task list

When you start, skim the tasks via `TaskList`. The incomplete ones
labeled "pending" or "in_progress" are the open work.

Currently active when I left off:
- **#58 in_progress:** BB→Bootstrap→Initial now loads 896 Initial IM
  entries and runs Initial. With canonical Initial substitution, the
  probe now reaches display/disk init, exits the RTClock wait, attempts
  hard-disk boot, and falls through to Ethernet.
- **#45 in_progress:** `probe_full_boot` reaches `LoadDoradoCode` and
  the BB `Continuous` loop. `probe_full_boot_with_bootstrap` is now the
  canonical deeper path for Initial bring-up.
- **#46 pending:** Phase 7 slow-IO subsystem. Mostly DONE in
  practice (slow-IO routing layer + display + disk all wired).
  Could be marked completed.

## Suggested first action for the next session

The probe currently bypasses one issue (Bootstrap streaming
corruption) to let Initial run. NOSTORAGE, PRESETMAP, FINDMODULE,
BootMem, the first-64K clear loop, and the `0o6205` RTClock wait are
no longer the current blockers. The CURRENT BLOCKER is boot media:
Initial reaches `DiskHardMicrocodeBoot`, but the mounted Trident pack
does not yet produce a successful boot transfer, so execution falls to
the unimplemented Ethernet boot path.

### Highest-value: finish the boot-media path

Latest disk bring-up checkpoint:

- `dorado_disk_controller_advance_sector()` now models index wrap:
  index pulses assert both `IndexTW` and `SectorTW`, clear
  `BlockTillIndex`, and non-index sector pulses are masked while
  `BlockTillIndex` is set. `test_block_till_index` covers this.
- The full-boot probe advances the synthetic spindle on every service
  call while `BlockTillIndex` or a seek is pending. This keeps the
  controller behavior close to HM page 97 while avoiding probe-only
  boot-transfer timeouts caused by an arbitrary fake spindle rate.
- With disk tracing enabled, Initial again reaches `KSameDrive`,
  `KContinueCmmd`, `KCheckSeek`, `KWAITSECTOR`, and `WAITFORSECTOR`
  before the first disk boot timeout. It still does not reach
  `DoDiskBlock` or read `DiskData`; `InitialDisk.mc` says the hard disk
  microcode file is a private Dorado convention starting at page 4
  (cylinder 0/head 0/sector 4), while the mounted `spruce-server.dsk300`
  is an Alto Spruce T-300 pack and is not known to contain that file.
  Treat Spruce as a controller validation pack, not as a required
  successful Initial hard-disk boot source.
- Current disk model details to preserve: KSTATE block-mode muffler
  signals `RdOnlyBlock'`, `WriteBlock'`, and `CheckBlock'` are
  active-low; Cylinder Tag/ReZero now hold `NotReady` and delay `TagTW`
  until the synthetic sector/index cadence reaches index; subsector
  count follows TriconD's sector-counter oracle (`count 3 -> 30`
  wakeups/rev, with the final partial group rounded up). DiskTag decode now uses native
  strobe bits from `DiskDefs.mc` (`0x8000` Drive, `0x4000` Cylinder,
  `0x2000` Head, `0x1000` Control) and ignores unstrobed preload/idle
  bus words. The probe spindle period is now
  `DORADO_DISK_SECTOR_PERIOD` with default 512 cycles; this lets DSK
  reach `KSameDrive`/`KCheckSeek` before Initial's first
  `BootTransferTimeout`. The latest focused probe no longer corrupts
  CHS to head 10. After the non-emulator `BLOCK` fix, the focused
  disk trace gets through `SendTagWait` correctly (`KTemp0=0003`,
  DiskMuff `003`, `Read1Muff=0001`) and reaches the `Read20Muffs`
  status scan, but still has no FIFO reads/writes.
- New memory-system fidelity: `NoRef+UseMcrV` stores now update the
  selected cache-address entry without touching map/storage. This is
  needed by `InitialSubrs.mc` `ClearCacheFlags`.
- New disk fidelity: DiskMuff output now uses native low-byte address
  selection plus high-byte clear bits from `DiskDefs.mc`, and DiskTag
  decode now requires native strobe bits (`0x8000` Drive, `0x4000`
  Cylinder, `0x2000` Head, `0x1000` Control). DiskMuff input returns
  asserted signals as right-justified `0x0001` in this emulator's C bit
  layout; `DiskSubrs.mc::Read1Muff` explicitly returns 0/1 and tests
  with `R odd`.
- New CPU fidelity: `BLOCK` selects STK only for the emulator task. For
  non-emulator tasks it only blocks/yields the task, so disk code such
  as `KTemp0_ muffSeekTagTW, Block, Call[Read1Muff]` writes RM rather
  than STK.

1. Use the `probe_full_boot_with_bootstrap` boot-landmark and per-TIOA
   disk counters to find why PilotDisk enters `Read20Muffs`/status
   handling before any `DiskData` FIFO read.
2. If the failure is real pack contents, stop spending time on
   `spruce-server.dsk300` as an Initial hard-microcode source; it is
   an Alto Spruce pack and likely lacks the private Dorado hard
   microcode file at page 4.
3. Implement enough 3 Mb Ethernet boot support, or a controlled
   Initial Ethernet packet injector, to deliver `chm/microcode/*.eb`
   files and let `EtherMicrocodeBoot` load emulator microcode. A gated
   probe injector now exists: run
   `DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
   ./build/test_cpu`. It skips the first 512-byte EB overhead page,
   copies the zero-sum payload into Initial's `BootDataPtr` area, and
   lets `CheckChecksumAndLoad`/`LoadRam` run. Latest result reaches the
   loaded-image runtime loop around `PC=0o6000/0o6002/0o6012`.
   The probe compares sampled IM addresses against known `.mb` files:
   `Mesa.mb!3`, `TriMesa.mb!3`, `Cedar.mb!6`, `DSemu.mb!1`, and the
   UnBug Mesa image all report `0/6` (AEmu has no sampled addresses
   present), so `AltoMesaDorado.eb!1` should be treated as a distinct
   AltoMesa LoadRam image rather than a byte-for-byte copy of a checked-
   in `.mb`. Current post-load state: the loaded image now schedules
   multiple tasks when the probe clocks are allowed to run like
   free-running hardware. A focused subset of `_cd8_/doradomicrocode/`
   was downloaded into `chm/doradomicrocode/doradomicrocodesources/`;
   `ADefs.mc!3` confirms the expected task numbers and
   `DisplayMain.mc!1` confirms the DHT/THT -> WCB -> DWT/TWT handoff.
   The emulator now models the WCB flag protocol, but the latest
   120M-cycle run still has `display iofetch=0` and DWT wakeups `0`.
   The useful new clue is that all display writes are task 4 to TIOA
   `0366` (`TNLCB`) plus two writes to `0367` (`TStatics`); there are
   no writes to `0364` (`AHTFlag`), and low-core display words
   `0420..0427` (`DAStart` region) are all zero. That means task 4 is
   alive and blanking/terminal-scanning, but no display control block
   chain has been installed for the word task to fetch. Current raw
   loaded IM samples:
   `0o6000=00104/71501/00000`, `0o6001=00104/131705/140000`,
   `0o6002=00104/14701/00000`, `0o6012=13116/14105/00000`,
   `0o6100=00204/60005/00000`, `0o5021=05406/77714/40000`.
   After the `NoRef` cache-address, cache `Vacant` lookup, DiskMuff
   `IOB[15]`, and display WCB protocol fixes, a 120M-cycle focused EB
   run moves farther: final `PC=0o6307`, display snapshot `frame=180`,
   task 4 hot at `0o6300/0o6301/0o6311/0o6721/0o6744`, but still
   `display iofetch=0`, DWT wakeups `0`, and disk FIFO reads/writes
   `0`. The cache `Vacant` fix is important because Initial's
   `ClearCacheFlags` writes all CacheA entries and then marks them
   vacant; those addresses remain readable through `dVA<-Victim` but
   must not be treated as cache-data hits.
4. Keep improving DiskTag/format-RAM/sequence-PROM behavior in parallel
   so real emulator disk I/O has a solid controller after microcode
   load. The next likely disk gap is the controller read/check FIFO
   status sequence: `RdFifoTW` thresholds, block-mode status, ECC words,
   and end-of-block `ReadErr`/`WriteErr` summary bits.
5. Re-run `build/test_cpu`; success means `CheckChecksumAndLoad` and
   `LoadRam` are reached after disk or Ethernet microcode load.

### 2026-04-30 EB direct-load probe update

`DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
./build/test_cpu` now parses the EB End item (`start=0o1076`) and has
a probe-side direct LoadRam path that installs EB IM/IFUM/RM items.
This is a diagnostic shortcut, not a real Ethernet controller.

Result: the run reaches Alto/Mesa initialization/display/disk PCs
(`0o1077`, `0o4656`, `0o5624`, `0o6744`, `0o7000`, etc.) and the
loaded IM samples match the EB payload, but it halts in task `0o14`
at `PC=0o6` on `halt: IFU not ready`. IFU diagnostics show
`active=0`, `PCF=0`, and `Post-LoadRam IFU transitions: arms=0`, so
no `PCF<-B` has happened before a page-zero IFUJump exit is reached.
Next best action: debug task startup/TPC/CoReturn/LdTPC after EB
initialization, especially why task `0o14` resumes through `0o3500`
with `Link=0o6`, instead of continuing to emulator `StartIFU`.

### 2026-04-30 EB direct-load stale I/O cleanup

The immediate disk-task `PC=0o6` halt was a probe artifact: the direct
LoadRam shortcut carried Initial's partially active disk controller state
into the AltoMesa image. The probe now resets the disk controller at EB
takeover, preserving the attached pack, and the display scanline model no
longer wakes DHT before display microcode selects a terminal task.

### 2026-04-30 memory-map follow-up

The memory-map docs/sources were rechecked after the stale-I/O cleanup.
`LoadRam.mc` confirms the direct EB loader was already loading RM from
item word 2. `NewMemory.mc`/`InitMem.mc` show that `B←Map'`/`Pipe3'`
returns the old RP, while old WP/Dirty are recovered through
`B←Errors'`/`Pipe4'`; the emulator now snapshots both. `EMemDefs.mc`
also corrected the Config layout: `ChipSize` is b12/b13 (C bits 3..2)
and module bits are M0=`0200`, M1=`0100`, M2=`0040`, M3=`0020`.

Current EB status: the direct run reaches the cycle budget rather than
an early halt, writes display-control outputs from task `0o4`, and
produces `/tmp/dorado_boot_display.pgm`. The Config fix moves the EB
state forward: `R400=0x0100`, BR31/BR36/BR37 now become `0x20000`
instead of `0x10000`, and the old Mesa `XFER/XFERMD` loop at `0x10000`
is gone.

Follow-up: `B←FaultInfo'` now clears the latched FaultInfo state, as
`InitMem.mc` expects, and the direct EB probe maps the skipped warm-start
bank `0x20000..0x2FFFF`. The same run now reports
`Memory: faults=0`; `Mar=0x2FE1F` is just the last reference, with
`Map[0x2FE]=rp02FE wp0 d0 r1`. The next blocker is task/display
bring-up, not memory translation: IFU still never arms, `DAStart`
remains zero in the IOBR bank, and the probe eventually switches to
task `014` with TPC `0177037` after running task 0 hot at
`0o5013/0o5014/0o5022`. Next best action is to trace the loaded Mesa
world's task initialization and wakeups, especially why DSK/display
wakeups become ready before their TPCs are valid and before `PCF←B`.

2026-05-01 update: the direct EB probe now also resets the modeled
display controller at the LoadRam/IOReset takeover, matching the disk
controller reset already done there. This removes Initial's stale
terminal-task state from the loaded world. With
`DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
./build/test_cpu`, the run no longer switches into an invalid DSK TPC:
it stays in task 0, reports `Memory: faults=0`, `MCR=0x0004` with
`nowake=0`, and no post-EB display/disk wakeups. The new blocker is
inside the loaded Mesa/AltoMesa IFU startup path: it halts at
`CPU_HALT_IFU_NOT_READY`, final `PC=0o4`, after hot task-0 PCs around
`0o4654/0o4656/0o4657` (`Mesa.mb!3:SETDLP`) and `0o5724/0o5736`.
Suggested first action: trace that Mesa IFU path against `Mesa.mb!3`
symbols and IFU pause/PCF semantics; the I/O wakeup ordering bug is no
longer the first direct-EB blocker.

2026-05-01 follow-up: the optional `DORADO_POST_EB_TRACE=1` ring now
captures the loaded-world task-0 path. That corrected the diagnosis:
the focused direct EB run starts at `InitMap` and never reaches the
normal `InitTasks`/`StartEmulator` handoff before halting at the page-0
IFUJump. The hot `0o4654/0o4656/0o4657` loop is the `SetDMuxAddress`
helper used during memory configuration, and the later `0o5720/0o5747`
path is still memory/map setup context, not a live Alto opcode loop.
Next best action: compare `InitMem.mc` against the memory side effects
visible in this trace, especially `Config'`, `SetDMuxAddress`/`UseDMD`,
`Map<-`, `Pipe5`, and `LoadMCR`, until `InitMap` reaches
`StartEmulator`. Only then will task/display wakeups and `PCF<-B` be
expected.

2026-05-01 later follow-up: original docs/source closed several of the
direct-EB memory setup gaps. `LoadRam.mc` records do not carry ALUFM, so
the probe-side direct LoadRam path restores the standard ALUFM table
that Initial would have left behind. HM Table 11d says
`Pd<-ALUFMEM` is read-only; Mesa `SETDLP` was using it and the emulator
was accidentally rewriting ALUFM. HM §8.3/§12.1 plus `Kernel5.mc`
clarify the junk timer polarity: Dorado bit 15 is the low-order C bit,
`AckJunkTW.15=1` enables periodic junk wakeups, and `IFUTest.15=1`
disables them. `InitMem.mc` also shows `NextMapEntry` depends on
`DummyRef_ T, T_ MD`, so that compiled DummyRef shape now uses old `T`
as Mar. Finally, HM page 29's "FF branch condition ORs into TNIA" rule
now applies to return-class JCNs; this fixes `Return[ALU=0]` in
`NextMapEntry`. The focused direct EB run with
`DORADO_BOOT_BUDGET=26300000 DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1 DORADO_POST_EB_TRACE=1`
now passes, leaves the old unbounded `VAHi` map enumeration, reaches
`STARTEMULATOR(AEmu)=0o1133`/`DOBRS`, and later runs in disk/status
service code (`SEEKWAIT`, `KREADBADTW`, `WAITFORMAPBUF`) with
`Memory: faults=0`, `BR36=0x19100`, display FIFO activity still zero,
and disk FIFO reads/writes still zero. Next best action: continue from
the disk/status/muffler path using `DiskSubrs.mc` (`Read1Muff`,
`Read20Muffs`, `SeekWait`, `WaitForSector`) and HM §9/DskEth
schematics; verify DiskMuff signal selection, TW clearing, seek-ready,
sector/index, and FIFO-ready semantics.

2026-05-01 display/map correction: the latest fault diagnostics move
the active blocker back to display-memory ordering. The display model now
honors the `Statics` `DHTShutUp/DWTShutUp` bits from `DisplayDefs.mc`:
reset starts with both wakeups shut up, TIOA `0377`/`0367` updates the
Statics latch, and synthetic scanline/DWT wakeups stay suppressed until
microcode clears the bits. With
`DORADO_BOOT_BUDGET=120000000 DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1`
the run still reaches `NOTEMUFAULT`, but the last fault is now precise:
`task 3`, `store`, `TIOA=0375` (HRam), `VA=0x2010C`, with
`BR36=0x20000`, `EmuBRHiReg=0x0002`, and `Map[0x200]`/`Map[0x201]`
still vacant. `DORADO_FAULT_TRACE=1` is available for non-emulator fault
lines (`DORADO_FAULT_TRACE=all` includes task 0), and the normal probe
output now prints task MemBase/TIOA plus the AEmu BR registers. Next best
action: trace `JAMHRAM`/`InitHRam` in
`DisplayAux.mc` and the immediately preceding map writes; determine why
the display horizontal task can run through MDS before bank 2 has been
remapped.

2026-05-01 HRam protocol follow-up: the Sep 1981 Hardware Manual
display RAM-loading section and `DisplayAux.mc` `LoadHRamLoop` were
rechecked. `src/display.c` now models the HRam slow-IO protocol for
TIOA `0375`: `Keep'` low takes Dorado ownership, `LoadAddr` latches the
address while owned, active-low `Write'` stores the 3-bit HRam word, and
writes auto-increment. `test_hram_load_protocol` covers that path. The
CPU fault trace now includes decoded `asel/lc/ff/jcn` and raw IM words
for the faulting instruction, which is the next diagnostic for the
remaining task-3 store fault.

2026-05-01 `Output_ T` follow-up: the task-3 HRam fault is fixed. The
display/disk microcode uses source-level `Output_ T`; in the loaded EB
image the relevant instruction decodes as a store-shaped alternate
source with no LC destination. `src/cpu.c` now treats that narrow shape
as slow-I/O output when the current task/TIOA has a registered writer,
and `test_output_t_store_shape_routes_slow_io` pins it. New EB
checkpoint: `display outs=+1090`, `scanline wakeups=+970`, no task-3
HRam fault. The remaining fault is task 0 fetching from MDS
`VA=0x20004` at `pc=0o573` while `Map[0x200]` is vacant, so the next
debug target is the warm-start/LoadRam map restoration for bank 2.

### Highest-leverage but hardest: fix Bootstrap streaming

Currently bypassed by substituting canonical Initial.MB at BOOTSTAGE2.
Real fix would mean we no longer need that workaround.

1. **Verify the BB ROM Boot1Data layout.** Disassemble bytes from
   `chm/dorado/doradobaserom.mb!13` C000-D7FF range and compare to
   Initial.mb in real-address order. If they're different builds,
   all debugging needs the BB ROM as ground truth.
2. Trace one full BootByteL iteration cycle-by-cycle through Bootstrap's
   T-composition (ReadBB1 → LSH[T,10] → ReadBB2 → LDF[T,10,0] → XOR Byte1).
3. Verify our shifter's LSH/LDF outputs match Bootstrap's intent
   (note: `10` in Mesa source is OCTAL, so `LSH[T,10]` = shift left 8).

Good luck. The infrastructure is solid; current state is "Initial
runs through canonical setup code with two probe-side workarounds,
stuck in I/O wait" — concrete next step is identifying the I/O
device at TIOA=0xC0.

## Recent commit history (reverse chronological, latest first)

```
862ba67 Advance BB Bootstrap into Initial
64a586c Bring BaseBoard Boot0 to Continuous
7560889 docs: io-systems-architecture reflects Fast I/O DONE
50d2d80 Fast I/O transport: Fin/Fout busses for DSK and DWT (HM §8)
6345f28 docs: refresh display+disk status to Phase 2
63797e9 Display + Disk Phase 2: Tag decoder + sector reads + FIFO renderer
292fd24 boot-bringup-plan: document probe_initial + BootstrapSources finding
2005727 probe_initial: load Initial.MB+Bootstrap.MB layered, run from INITIAL
bc18f7f probe_full_boot+bootstrap diagnostics: Write IM target tracking
2606929 boot-bringup-plan: document Phase-1 display+disk status
840d12e probe_full_boot_with_bootstrap: BB drives Bootstrap.MB via real CPReg stream
fcf07ef Detailed display + disk architecture references
a62339f Doc updates: display + disk Phase 1 status
078b0bd Trident T-80 disk port (HM §9) — Phase 1
79aa902 Display framebuffer + DDC stub (HM §11) — Phase 1
5b87de0 Microengine correctness fixes: ALUFM, JCN, RWCPReg
18bcd3c Slow-I/O routing layer (HM §7)
```

`git log --oneline -30` for more.

---

When you finish your session, **update this doc** with:
- Anything new that's working.
- Any pitfalls you discovered.
- The new active-task IDs.
- Updated "Suggested first action."

Keep it short, keep it honest. Don't write aspirational status.

2026-05-01 snapshot/map update: current EB boot snapshot is still blank
white. `/tmp/dorado_boot_display.png` has `unique=1` and `nonwhite=0`;
the renderer test PNGs (`/tmp/test_dorado_display.png` and
`/tmp/test_dorado_render.png`) do contain pixels. The next blocker was
confirmed as task-0 IFetch at `pc=0o573`, `VA=0x20004`, with bank 2
vacant. `InitMem.mc` says `InitMapWarm` maps the emulator virtual bank
onto the first 64K of **real** Alto memory, so the direct EB LoadRam
probe now maps virtual bank 2 (`0x20000`) to real pages 0..255 and
reapplies it after `loadram_image_direct`. The EB image still later
vacates `Map[0x200]`; the probe records that cycle/task/PC and remaps
bank 2 again so bring-up can continue. The next stop was task 2
IFetch at `VA=0x624aa`, so the EB probe now maps the installed
real-storage page range before overlaying bank 2 to real page 0. This
is a probe correction, not a substitute for implementing the real
LoadRam/warm-map handoff.

2026-05-01 EB probe progress after installed-storage shim:
`display outs=+48333`, `dwt wakeups=+1`, disk slow I/O is active
(`outs=+3020`, `ins=+2984`), and display-list words through IOBR are
non-`FFFF`. The framebuffer snapshot is still blank white because
display fast-I/O fetches remain `0`. The new blocker is an emulator
IFetch fault after the Code BR goes bad (`BR37=0xfff0006` in the
summary). Next step: trace the BR load/StartIFU path around that
corruption before implementing more display behavior.

2026-05-01 loaded-world disk trace update: the focused EB run no longer
hits the old BR37 failure in the sampled 80M/120M runs; it reaches the
display loop (`display outs` in the millions) but still has
`display iofetch=0` and zero DAStart words, so software has not built a
display control block yet. Disk remains the active blocker. The
`DORADO_DISK_TRACE=1` buffer now resets at direct EB injection and
captures task-0 plus task-14 disk-code ranges. The loaded image gets
task 14 to `KCmmdRead` with `KCmmd=0005`, then immediately takes
`KReadBadTW`/`Read20Muffs` because `RdFifoTW=0`. The controller has
`EnableRun=1`, `Active=0`, and `DiskControl=0100`; the only
`DiskControl` writes in the focused window are `0400`/`0100`, with no
`O10:0005` command load. Suggested next action: inspect the compiled
instructions on the `KCmmdInTime` path in the EB-loaded image and
verify whether `Output_ KCmmd` is being skipped, revoked by the
same-sector check, or compiled into another slow-I/O output shape that
`cpu.c` does not yet route. Current bring-up fallback in `disk.c` starts
a read stream when task 14 explicitly polls `muffRdFifoTW` with
`EnableRun=1`, `Active=0`, and a mounted pack; replace this with the
real DiskControl/Format-RAM sequence-PROM start timing.

2026-05-01 follow-up after the fallback: full boot now gets out of the
disk wait and into LoadRam. The disk-loaded IM matches canonical
`Initial.mb` for 925/926 overlapping entries (`0o7512` is the only
difference), and the hot loop is now LoadRam
`0o7605/7606/7611/7612/7614/7620/7673/7675/7676`. New opt-in
`DORADO_LOADRAM_TRACE=1` records the item-stream state; it shows LoadRam
using `BR31=0x00200` and reading from `0x200`, but the first item words
there are zero, so LoadRam consumes endless zero IM items instead of an
End item. Next suggested action: verify whether `spruce-server.dsk300`
actually contains the Dorado hard-microcode boot file; if it does, trace
the disk boot memory stores into the `01000B` boot buffer. If it does
not, use the EB/direct LoadRam path for display bring-up and treat this
disk path as a media-content limitation until we have a known-good Dorado
boot pack.

2026-05-01 EB snapshot fix: the direct EB shortcut reinitialized
`display` after `loadram_image_direct` but did not reattach it to the
I/O table, leaving `display.attached=0` and skipping the end-of-run
snapshot. The probe now calls `dorado_display_attach_to_io` after that
reset. `DORADO_BOOT_BUDGET=120000000 DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1`
writes `/tmp/dorado_direct_eb.pgm` at frame 158 and finishes with no
memory faults, but the image is still all white (`unique=1`,
`nonwhite=0`) because display fast-I/O fetches remain `0`.

2026-05-01 IFU/Junk + disk-loop follow-up: CHM source copies were added
under `chm/doradomicrocode/doradomicrocodesources/Junk.mc!1` and
`chm/dorado/expanded/BootstrapSources.dm/InitialJunk.mc`. `InitialJunk`
confirms the early JNK task just acknowledges the junk TW and increments
RTC counters. HM §8.3 says `IFUReset` loads `IFUTest` with 1; in the
emulator that means setting Dorado bit 15 (`0x0001` in C-LSB layout),
which disables the optional junk timer. Implementing that side effect
stopped the 30M probe from being stolen by task 2/JNK at AEmu
`NotReady`.

The later 100M full-boot disk trace reached task 14 `Read20Muffs`
many times, but it is not an infinite flag-shift bug. In the traced
loop, `KTemp0` advances `0001, 0002, ... 4000, 8000`, the next
instruction latches `ALU<0`, and the branch exits at `0o6605`.
Current status is therefore: controller/status scanning works well
enough to return to PilotDisk error handling; the missing piece is
still either correct command/FIFO timing for the real sequence-PROM
path or a known-good Dorado boot pack rather than the Spruce Alto T-300
validation pack.

Follow-up after `4c7ba86`: the CPU slow-I/O shim now routes no-LC
store-shaped outputs for any source, including non-emulator `IOStore`
decode, with regressions for `Output_ T`, `Output_ RM/STK`, and the
IOStore-shaped case. This is the hardware-correct direction for
source-level `Output_ <source>` forms, but it did **not** make the
loaded boot trace emit `O10:0005`; the only DiskControl outputs still
seen in a 70M disk trace are `0400` and `0100`. The next disk debug
step is to identify the compiled instruction(s) corresponding to
PilotDisk's `Output_ KCmmd, Call[UpdateSector]` and confirm whether
they are skipped, revoked as "not in time", or using a TIOA/source
shape we still decode incorrectly.

2026-05-01 Ethernet bootstrap pass: added `dorado/include/ethernet.h`,
`dorado/src/ethernet.c`, and `tests/test_ethernet.c`, plus device-level
`IOAtten` callbacks in the slow-I/O table. `DORADO_BOOT_ETHERNET=1` is
now the default full-bootstrap probe path. Important CPU fix:
non-emulator `IOAtten` now comes from the addressed device only; using
`wakeup_pending` as a proxy made EOT branch to `EOAbrt` on every normal
transmit wakeup. With a temporary `DORADO_ETH_FORCE_ELOAD_ZERO=1`
probe guard, EOT reaches `EData` and EIT consumes queued
`AltoMesaDorado.eb!1` reply packets. Current blocker: EOT fetches zeros
from VM `177400B` instead of Initial's request packet, and EIT still
does not make Initial advance to `CheckChecksumAndLoad`, probably
because non-emulator task memory fetch/store through `MemBase=IOBR`
does not round-trip the packet buffers correctly. Next action: trace
EOT `Fetch_ EOPtr` and EIT `Store_ EIPtr`/`Fetch_ EIPtr` memory refs.

2026-05-01 real Ethernet LoadRam update: Initial now uses the sealed
boot-parameter path to request Mesa (`last_bfn=0110`) and EIT receives
all `AltoMesaDorado.eb!1` reply words. The EB End item in memory matches
the file (`[000002 000000 177730 001076]`, checksum zero), and LoadRam
jumps into the loaded world. Two CPU/IFU fixes landed here:

- `IFUJump` with an inactive IFU now vectors to the IFU NotReady trap
  instead of halting the emulator. This matches the hardware manual's
  NotReady retry model and the `AEmuNotReady` handler in `Start.mc`.
- IFUM function half ordering was corrected for real `LoadRam`: EB
  word0 (`PackedAlpha,,IFaddr'`) is written by `IFUMLH` and stored in
  `ifum_lo`; EB word1 (Sign/Length/TPause/TJump/N fields) is written by
  `IFUMRH` and stored in `ifum_hi`. The previous ordering corrupted the
  Mesa IFUM after Ethernet LoadRam.

The full probe now runs to the budget without a CPU halt. The current
default 80M Ethernet run writes `/tmp/dorado_boot_display.pgm` at frame
114 and touches the display controller heavily (`display outs=671639`),
but still has `display iofetch=0`, `DAStart[0420..0427]=0`, and an
all-white snapshot. It does not hit the old forced Alto `KWait` first-
sector shim (`alto-boot shims=0`) and finishes around `PC=0o7321` in the
display/disk task mix. The older shim path was corrected from
`AEm0.mc`: `KWait` masks `DoneStatus` (`07400`) first and then treats
the low byte as error bits, so a successful shimmed read must store
`07400`, not `07401`. Next debugging target: continue the real
Alto/Dorado disk command completion path that should eventually install
the display control block chain.

2026-05-01 AWT fast-I/O follow-up: the display word-task router now
accepts both DWT (`013`) and AWT (`011`) for `IOFetch<-` delivery to the
display FIFO, matching the `DisplayMain.mc` DHT/DWT vs THT/TWT split.
`test_fastio` covers both routes. This did not unblock the screen: an
80M full-bootstrap run reports frame 114, `display outs=671639`,
`display iofetch=0`, `dwt wakeups=2`, `unrouted_iofetch=0`, and an
all-white `/tmp/dorado_boot_display.pgm` (`unique=1`, `nonwhite=0`).
Task-cycle counts show AWT only ran 31 cycles and DWT 1 cycle; low-core
`DAStart[0420..0427]` remains zero. So the FIFO path is ready for both
word tasks, but software has not yet installed a display control block.
Continue with the AEmu disk/status `KWait` visibility problem rather
than adding more framebuffer rendering behavior.

2026-05-02 disk trace ring / AEmu handoff update: the disk trace in
`tests/test_cpu.c` now retains the last 8192 samples, so late
loaded-world disk behavior is visible instead of only the initial Disk
RAM setup. The late 80M trace is task 14 looping through the Alto disk
status/sector path (`AltoLoop`, `Read1Muff`, `Read20Muffs` after EB
relocation). It does not reach a `DiskData` transfer; `fifo reads=0`,
`fifo writes=0`, and the display remains blank because `DAStart` is
zero. The first-sector shim now checks absolute low memory, Alto MDS
(`BR36`), `BR31`, and `BR30`, but it still reports
`alto-boot shims=0`; final MDS `[0431..0440]` and `[0521..0523]` are
zero. The likely next debugging target is the command-handoff edge:
watch stores to the loaded-world low-core windows and the DSK task fetch
path that should notice a nonzero command pointer and branch to
`DoACmmd`.

2026-05-02 cache-visible / CSB follow-up: storage-only probes were
misleading for dirty cache lines. `dorado_visible_word_at_va()` now
checks cache first, and the boot diagnostics use it for low-core,
display, and disk handoff words. The important changed fact is that MDS
`0523` is visible as `177776`; MDS `0521` and the legacy `0431` command
block remain zero. The PilotDisk CSB theory was also checked: the real
CSB is `0177520` relative to `IOBR` (`PilotDiskDefs.mc`), and the new
diagnostic shows `IOBR+0177520 = [0000 0000 0000 0000]` while only the
irrelevant absolute page reads `FFFF`. `DORADO_CSB_TRACE=1` confirms
Initial/DSK clear `CSB.next` and no IOCB is posted. A 120M focused disk
trace still never selects `DiskData`; it cycles through the Alto disk
status/sector loop, and the display task outputs blank
`COLORBITMAPNIL` scan lines with `DAStart=0`. Next target: the Alto
`ACmmdCheck`/`Read20Muffs` status path after EB relocation, not
PilotDisk CSB.

2026-05-02 forced-shim correction: the supposed loaded-world `KWait`
at `0o5550` was a bad symbol match. A focused `DORADO_POST_EB_TRACE=1`
run shows `0o5550` is a store/clear loop with `MemBase=30`, not the
`AEm0.mc` disk wait. The harness no longer forces the first-sector shim
there; it only keeps the older `0o1017` force point and the command
block scan path. Current 80M runs are honest again:
`alto-boot shims=0`, `fifo reads=0`, `read streams=0`, display remains
`COLORBITMAPNIL`, and no command pointer has been observed. Also note
that `spruce-server.dsk300` head 18/sector 0 is blank/free; the harness
now has `DORADO_ALTO_BOOT_CYL`, `DORADO_ALTO_BOOT_HEAD`, and
`DORADO_ALTO_BOOT_SECTOR` knobs, defaulting to local candidate
`0,0,2`, but this only matters once a real command handoff is found.

2026-05-02 AltoDiablo source check / status-loop correction: fetched and
checked the original CHM `AltoDiabloDisk.mc` and `TriDisk.mc` sources.
`AMapHdwStatus` reads KSTAT with `Read20Muffs`, maps
`NotSelected|NotOnLine|NotReady` to Alto unit-not-ready, maps FIFO
over/underflow to data-late, and then ORs in `07400` done bits. The
late focused trace is not stuck in a failing transfer: `VM 521` is zero,
task 14 is idling in `AltoLoop`, and the changing `xF00`/`8F00` word is
the current mapped idle status written to `VM 522` with sector bits.
There are still no `DiskData` reads because no active Alto command is
posted. Disk remains incomplete, but the current boot/display blocker is
now more likely the loaded-world software/display handoff than a pending
read-FIFO bug.

2026-05-02 DisplayInitConfig correction: `DisplayAux.mc` runs the
display hardware probe in the EMU task, not DHT/AHT. The display device
now routes only EMU task TIOAs `TStatus` (`0360`), `DDCStatus` (`0370`),
and `Statics` (`0377`) so the probe sees an idle single-bit status bus
instead of the floating bus / complemented keyboard word. An 80M run now
ends with `DISPLAYCONFIG=0x0000`, DHT/DispY outputs dominating
(`TIOA 376/374/372/375/377`), and AHT essentially idle. The display
configuration is therefore more hardware-like, but `DAStart` is still
zero and `display iofetch=0`; the DWT wakeups are initialization or
spurious wakeups before any DCB exists. Continue by tracing the loaded
software path that should write `DAStart` or post the Alto command at
`VM 521`.

2026-05-02 terminal boot pulse / invalid-TPC guard: the display model
now has a headless `dorado_display_boot_button()` helper. It follows
`DisplayAux.mc!1` by returning terminal serial bit `1` from `TStatus`
for a bounded number of scanline reads, enough for the boot message type
`17B`, then returns idle zero. The long boot harness arms this only
after the terminal display task has started. Initial/Mesa also sets the
AHT saved TPC to `0177777` while reinitializing task PCs; the synthetic
scanline clock now suppresses those stale display wakeups instead of
selecting the sentinel address. A 160M run no longer halts there and
finishes still running at frame 246 with `display outs=276615`,
`display iofetch=0`, DWT wakeups `2`, scanline wakeups `135241`, and
two invalid display wakeups suppressed.

2026-05-02 low-core store trace: the memory layer records the last
reference, and `DORADO_LOWCORE_TRACE=1` reports stores to the active
display/disk low-core windows through absolute memory, `DiskBR` (`030`),
`IOBR` (`031`), and Alto `MDS` (`036`). The 100M trace saw only clears
of `0420..0450` and `0521..0523`, keyboard all-up writes at
`0177034..0177041`, and a disk status write `DiskBR+0523 = 177776`.
There was no nonzero `DAStart` and no Alto command pointer at `VM 521`.
Treat the current blank screen as a loaded-software handoff problem:
find why the IFU/Mesa path does not install a display DCB chain or post
an Alto disk command, rather than adding more FIFO/rendering behavior.

2026-05-02 IFU/Alto sector force test: `DORADO_IFU_TRACE=1` now records
post-LoadRam IFU arm/stop transitions and PCX hot spots. Without a
software boot image the loaded AEmu path repeatedly starts IFU at
PCF/PCX zero (`STARTIFU 0o2201`, pause at `0o2202`, opcode `000`), with
the boot-parameter AC seed still in `STK[1..3]`; no `VM 521=0431`
command pointer is posted, only idle/status writes to DiskBR
`0522/0523`. The harness now has a probe-only force point at `0o2220`
that loads one Trident sector into Alto `MDS` (`BR36`) before the IFU
fetch. With the default Spruce candidate `CHS=0,0,2`, `alto-boot
shims=1` and the IFU spin stops (`arms=2`, `stops=1`), and IOBR
`DAStart` becomes nonzero-looking, but the sector is not a valid OS boot
image: `MDS` is corrupted to `0x3500000`, one fetch fault occurs, and
`display iofetch=0`. Treat this as evidence that the emulator can leave
the empty-core loop when code exists, but we still need correct
Alto/Dorado boot media or a real net/disk software boot path.

2026-05-02 Spruce sector sweep: tried forced sectors `0,0,{2,3,4,8}`
through the same `0o2220` IFU force point. Sector 2 faults at
`MDS=0x3500000`; sectors 3 and 8 fall back into the PC-zero IFU spin.
Sector 4 is the least bad: it runs to `PC=0o6612` with no memory fault
and produces many display slow-I/O outputs (`TIOA 376/372/374/375/377`),
but `DisplayMain.mc!1` fetches only low-core `DAStart` word `0420` as
the DCB chain pointer, and that word remains zero. The generated
`/tmp/dorado_eb_sector_4_80m.png` is all white (`unique=1`), and
`display iofetch=0`. The nonzero words at `0423..` are string/data
debris, not a valid DCB chain. This reinforces that Spruce is useful as
a storage-format fixture, but not as the AEmu OS boot payload.

2026-05-02 second-stage Ethernet boot check: re-read the Dorado booting
memo, `InitialEther.mc`, `InitialMain.mc`, `AEm0.mc`, and the local
`EtherBoot.mesa!1` transcription. The important distinction is now
documented in `docs/ethernet-architecture.md`: Initial's Pup
MicrocodeBoot (`264B`/`265B`) only loads emulator microcode. The
post-LoadRam Alto/Mesa software boot uses Alto conventions; BS selects
Ethernet boot, BS+Quote requests NetExec, and the Alto loader sends a
Mayday Pup (`244B`) followed by EFTP Data/End (`30B`/`31B`). Added a
gated probe, `DORADO_ALTO_BOOT_ETHERNET=1`, with Quote down by default
via `DORADO_ALTO_BOOT_QUOTE=1`, and it seeds both the headless display
keyboard state and the visible boot-key memory words. A focused 32M run
with `DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1`
ends with keyboard words `FFFE FFF7 FFFF FFFF`, but still shows zero
post-LoadRam Dorado EOT/EIT traffic and still reaches the disk path
(`alto-boot shims=1`, no Ethernet requests). Next: trace the exact AEmu
keyboard read / `EBoot` branch point; after that, implement the Alto
Ethernet/SIO surface and a Mayday/EFTP fake server rather than adding
more Initial `MicrocodeBootReply` behavior.

2026-05-02 direct EB map/wakeup follow-up: the focused
`AltoMesaDorado.eb!1` run exposed a probe artifact in the direct
LoadRam shortcut. The loaded `InitMap` path legitimately vacates the map
while rebuilding it, but the harness was still able to synthesize display
scanline wakeups into task 3 during that window. The probe now maps both
observed Alto MDS aliases (`0x20000` and `0x3500000`) onto the first
64K real words, clears non-EMU task TPCs at direct LoadRam handoff, and
suppresses synthetic display task wakeups until those aliases are mapped
again. This removes the previous DHT fault at
`task=3 PC=0o4776 VA=0x3500116`; `DORADO_FAULT_TRACE=1/2` now produces
no fault trace for the 32M focused run. The remaining state is an AEmu
software boot problem: IFU arms at `0o2201`, pauses at `0o2202`, then
the run remains around Alto PC zero with no display IOFetch and no
post-LoadRam Ethernet traffic.

2026-05-02 MDS keyboard fix / no-shim disk probe: the keyboard seed
helper now writes absolute memory, `IOBR` (`BR31`), and Alto `MDS`
(`BR36`), and keeps the all-up words refreshed after the EB-loaded
world starts unless `DORADO_ALTO_BOOT_ETHERNET=1` is explicitly forcing
BS/Quote. This corrected the visible boot-key diagnostic to
`FFFF FFFF FFFF FFFF`. Rerunning the focused 36M disk path with
`DORADO_ALTO_BOOT_SHIM=0` still reports `alto-boot shims=0`,
`fifo reads=0`, `read streams=3506`, `DiskControl=0100`,
`rd_fifo_tw=1`, and no `VM 0521=0431` command pointer. The duplicate
local EB file `chm/dorado/AltoMesaDorado.eb!2` gives the same loaded IM
fingerprint and counters as `chm/microcode/AltoMesaDorado.eb!1`.
Current blocker is therefore no longer keyboard visibility; it is the
loaded software/IFU handoff or missing second-stage boot payload that
should create a display DCB or post an Alto disk command.

2026-05-02 Initial-prefixed EB probe: `InitialAltoMesaDorado.eb!3`
loads cleanly through the same injector but fingerprints as Initial
microcode (`925/926` canonical Initial matches, `start=0o406`), not as
the missing Alto/Mesa software payload. It produces no `VM 0521`
command and no display DCB. Do not use the Initial-prefixed EB files as
the next disk/display target except when explicitly testing Initial
boot-stage behavior.

2026-05-02 DiskControl abort correction: after the full 16MW storage
fix, a tight 107.2M disk trace showed Initial loading the format RAM
while the emulated disk controller was still `Active` from an earlier
read-stream shortcut. HM page 97 says a `DiskControl` output while
`Active` aborts the current sector transfer; only the following output
loads the control register. `dorado/src/disk.c` now models that first
abort edge by clearing `Active`, the read stream, FIFO contents, and
FIFO wakeup latches without loading the new control word or zeroing the
Format RAM address. The next run target is to confirm whether this lets
the subsequent `0400`/`0100` reset and Format RAM load leave the
controller idle until the real `KCmmd` command is issued.

2026-05-02 transfer-start correction: the follow-up 107.2M trace showed
the abort edge worked (`0400` cleared `Active`/FIFO, `0100` then loaded
normally), but the older bring-up shortcut still started a read stream
at the first unblocked sector using only `EnableRun` plus Format RAM
word 4 (`0104`). That is not the HM page 97 transfer-start rule. The
sector helper and `RdFifoTW` muffler fallback now require non-zero
`DiskControl` op fields before starting a synthetic stream, so the next
trace should expose whether the loaded disk microcode ever emits the
real read/check `DiskControl` command.

2026-05-02 spindle-wakeup harness correction: `service_boot_disk` was
also using `(DiskControl & 0xFF) != 0` as "transfer armed", so
`DiskControl=0100` (`BlockTillIndex`, no block ops) prevented the
probe-only idle-sector-wakeup suppression. It now tests the four
documented op fields instead. This should reduce false DSK wakeups while
leaving real read/check/write commands wakeable.

2026-05-02 CPU memory-form `Output<-B` correction: the next clean trace
after removing the synthetic Format-RAM stream stopped at `pc=0o5751`,
which disassembles to `DiskSubrs.mc:Read20MLoop`:
`KTemp1_ (KTemp1)+1, Output_ KTemp1`. The compiled instruction has
`ASEL=1`, `LC=RM/STK<-Pd`, and `FF=0036`; our earlier slow-I/O shim
handled no-LC store/IOStore shapes but missed this narrow six-bit
Table-11a `Output<-B` form under memory-reference ASELs. `cpu.c` now
routes `FF&077 == 0036` to `dorado_io_write` and suppresses the false
memory reference when the current task/TIOA has a registered writer.
`test_output_b_memory_form_with_lc_routes_slow_io` covers the exact
`Read20MLoop` shape. The focused CPU/full-boot probe now reports
`Memory: faults=0`, reaches display slow-I/O activity, and writes
`/tmp/dorado_boot_display.pgm`. Current late state is no longer the
`Read20Muffs` CPU fault: task 0 is hot in the loaded Alto/Mesa path
around `0o5073/0o5127/0o5175`, task 14 is still active in disk code, and
display fast-I/O fetches remain zero.

2026-05-02 DWT FIFO-available wakeup checkpoint: HM display wakeup
logic is `WantsDWT = (NextWCBFlag && !CurrentWCBFlag) ||
(CurrentWCBFlag && FIFOAvailable)`. The model now continues to wake the
DWT/TWT word task while a current WCB is active and at least one
16-word munch fits in the per-channel FIFO. `test_display_wcb_flag_protocol`
covers this. A focused post-EB trace now produces many DWT wakeups, but
`display iofetch` is still zero. The next concrete blocker is the
compiled `DisplayMain.mc` `DWTStart` instruction at `0o6702`, source
`AAddress_ (IOFetch_ AAddress)+(Output_ T), Block, Branch[DWTStart]`:
decode says it should be an I/O-task `IOFetch`, so trace why that
instruction is not reaching the fast-I/O display FIFO router.

2026-05-03 combined `Output_ T` + `IOFetch` fix: the DWT trace showed
`0o6702` executing with `ref=none`, even though the isolated decode
said `FF=0136`/`ASEL=0` should be `IOFetch`. Root cause was the
`Read20MLoop` fix being too broad: it suppressed all memory-form
`FF&077 == 0036` slow-I/O outputs when a TIOA writer was registered.
`DisplayMain.mc` uses that exact low-six-bit `Output<-B` function in
combination with a real `IOFetch_ AAddress`, so the CPU now suppresses
only the non-IOFetch false memory references. The new regression
`test_dwtstart_memory_form_routes_iofetch` verifies both sides of the
combined instruction: display slow-I/O sees `Output_ T`, and fast-I/O
receives the 16-word munch. The focused boot probe now reports
`display iofetch=480` with `Memory: faults=0`. The generated
`/tmp/dorado_boot_display.pgm` is still all white (`unique=1`,
`nonwhite=0`), so the next blocker moved to display data/render timing:
confirm whether the fetched munches are blank/white data, remain in the
FIFO, or are being drained at the wrong synthetic scanline/frame phase.

2026-05-03 display subtask/WCB correction: FIFO tracing showed an
apparent B-channel munch containing low-MDS words (`B17E 5253 ...`).
`DisplayMain.mc!1` explicitly says Alto terminal emulation never expects
a B-channel command or B subtask, so this was a model artifact: slow-I/O
callbacks did not receive the processor subtask, and the display model
was inferring the current WCB channel from the sign bit of `Output_
AAddress` (`-1` selected B). Slow I/O now has subtask-aware read/write
entry points, CPU slow I/O passes `task_subtask[ctask]`, and DWT/AWT
current-WCB updates use the real subtask. The terminal horizontal task
only honors the A/terminal NextWCB flag. The boot probe now has no
B-channel FIFO data; it reports `display iofetch=32`, FIFO A has 32
zero words, and the framebuffer remains all white. That makes the next
blocker a real boot/display-state problem: the word task is fetching
blank data before a valid Alto DCB/`DAStart` chain is installed.

2026-05-03 terminal-task latch follow-up: `DisplayAux.mc` chooses the
terminal controller after probing DispM; a no-DispM boot should keep
terminal work on DispY/DHT/DWT. The model now latches the first DHT/AHT
`Statics`/`TStatics` writer as `terminal_task` instead of allowing later
stale writes to retarget WCB wakeups. Focused probe result after the
latch: frame 213, `display iofetch=32`, FIFO A contains 32 zero words,
FIFO B is empty, no Fast-I/O drops, `Memory: faults=0`, and
`/tmp/dorado_boot_display.pgm` is all white. Next debugging should stay
on the loaded Alto/Mesa disk/software path that should populate
`DAStart` and the Alto DCB chain.

2026-05-03 DiskBoot/DSemu status: task-0 `DiskBoot` now successfully
posts the old Alto KCB pointer. The missed write was `Store_ ETemp1,
DBuf_ T` at real PC `0o5140`; stale display `TIOA=0370` made the CPU's
store-shaped slow-I/O shortcut skip the real memory write. CPU execution
now allows task-0 stores to proceed even if stale TIOA names a writer,
with regression coverage in
`test_task0_store_with_stale_tioa_updates_memory`. The 180M boot probe
reports MDS `[0521..0524]=0119 0000 FFFF 0000`, i.e. `0521=0431` in
octal.

The remaining disk/display blocker moved later. The corrected disk trace
shows task 14 polling the relocated DSemu KBLK at `MDS+0x8051` and
storing status at `MDS+0x8052` (`Mar=D248051/D248052`), not consuming
the old `0521` command pointer. `alto-boot shims=1` confirms the
temporary first-sector path fired once, but after that no relocated
command pointer appears and no `DiskData` FIFO transfer starts. Next:
trace the loaded software path that should initialize/post the relocated
KBLK around `0x8051`/`0x8054`; the old `0521` post is no longer the
immediate failure.

2026-05-03 repeated old-Alto disk shim follow-up: the harness now
services each new `MDS+0521=0431` post instead of stopping after the
first one. This matches `AEm0.mc`'s `DiskBootRetry` behavior and moves
the 180M probe past task 0's old `KWAIT` loop. Current summary:
`alto-boot shims=2`, `IFU active=1`, task 0 no longer dominates, final
state is around task `014` disk status code (`PC=0o5764`), old MDS disk
windows are clear, and the real disk FIFO path still has
`read streams=0`.

The display is still blank, but the failure is now sharper. New
diagnostics show display IOFetch first at `D24FFFF` (`BR21/TChannel - 1`)
and later at `0080008` (`BR20/AChannel`), with all-zero munch words.
Compiled Mesa symbols put `TWTINITPC=0o6100`, `DWTINITPC=0o6107`, and
`DWTSTART=0o6115`; the final task table shows task `013` as
`MemBase/TIOA=020/373`, so the word task is fetching through the generic
A-channel base, not the terminal channel base that should carry the Alto
DCB bitmap stream. Next debug target: why task `013` transitions from
terminal `TChannelBR=021` to `AChannelBR=020`, and whether the synthetic
DWT wakeup rule is over-waking generic DWT.

2026-05-03 color/raster source and TIOA-map correction: pulled
`ColorDisplay.mc!1`, `RastDefs.mc!5`, `RastMain.mc!6`,
`DMesaRastMiscDisp.mc!1`, and `DMesaRastMiscOps.mc!2` from the CHM
microcode archive. `RastDefs` puts Monterey commands at `0320..0323`
and `DMesaRastMiscOps` implements MISC `245` as "start LT"; the current
180M trace does not use those TIOAs yet. More importantly, Mesa's
`TOUCH*` helper code retargets display-numbered tasks, so the emulator
must not treat every Output from task 3/4/11/13 as display hardware.
`display_attach_to_io()` now registers only DDC/terminal `0360..0377`
and raster `0320..0323`. Result: the false task-13 TIOA `006` display
flood is gone (`[13]` display outputs drop from ~1.45M to 5), and the
same 180M probe reaches final PC `0o6266` instead of `0o5764`. The
screen is still all white and disk still has `read streams=0`; continue
from the real disk/software boot path, with cleaner display counters.
