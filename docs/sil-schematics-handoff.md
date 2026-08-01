# Handoff — the Dorado's own schematics, in Sil vector format

Written 2026-07-31. Read `docs/cedarchest-packages.md` §1 and §1.1 first for
the inventory and the shorter version; this is the working detail.

**Where it stands in one line:** every piece works except the last `Run` —
`VersionMismatch[BiScrollers]` — and nothing from this line of work has been
shipped to either platform.

---

## 1. Shipping status — READ THIS FIRST

Asked directly: *is the new image snapshotted and shipped for both
platforms?* **No, on every count.** Deliberately, because Sil does not load.

| thing | state |
|---|---|
| Native shipped checkpoint `build/good-packs/cedar-desktop.{snap,pdi}` | **unchanged** — the pre-existing one. It was overwritten by a failed bake and restored from a backup; verified afterwards at 167,537 px. |
| wasm32 checkpoints `dorado/web-assets/*.gz` | **unchanged**, untouched by this work. |
| The Sil/schematics image | baked, **not installed**. Kept as `build/good-packs/cedar-sil-wip.{snap,pdi,pgm}` (gitignored, local only). 167,318 px, a healthy desktop. |
| `User.Profile` change that produced it | **reverted** in the tree. The exact block is in §4 below — that is the only copy. |
| Commits `316b1fc`, `968fe3b` | committed, **NOT pushed**. |
| GitHub Pages | serving `d0204ba`. **Does not include the `.DF` date fix.** |

So two separate "not shipped" facts: the Sil image (intentional), and the
`.DF` bug fix (just unpushed — `git push origin main` deploys it).

The WIP image is worth keeping: it is ~25 minutes of compute, and it is a
volume with all 32 schematic sheets and every Sil dependency already
installed. Whoever resolves §3 can start from it instead of rebaking.

---

## 2. What was established

### 2.1 The drawings exist, and are directly fetchable

**506 distinct `.sil` files across 72 `.dm` archives** under
`io/doradologic/` in the CHM PARC archive — the complete Dorado logic set,
by board and revision: ProcH 33 sheets, ProcL 34, ContA 34, DskEth 49,
MemC 37, MemD 27, MemX 26, IFU 65, BaseBd 17, plus cables and Midas.

Two things that cost time:

- **The archive serves the `.dm` archives PRE-EXPANDED.** No unpacking:
  `io/doradologic/ProcH-apcRev-Da.dm!1_/ProcH01.sil` is a direct download.
- **Paths are case-sensitive, and `chm/cross-reference.html` lower-cases
  them.** `proch-apcrev-da.dm!1_/` 404s; `ProcH-apcRev-Da.dm!1_/` works.
  The cross-reference is still the right index — just fix the case from the
  parent listing (`io/doradologic/.index.html`).

`ProcH01.sil` is 6,452 bytes of vectors, `08-Oct-1981 18:15:10 PDT`,
Clark.pa. Its strings are the signal names our own C uses: `BMux`,
`FF[4:7]`, `MemBase`, `Mapping Register`, `AMux`, `Md`, `Q`, `MemIn`.

### 2.2 They reach the emulated volume

Staged in `chm/cedar/stp-root/CedarChest6.1/DoradoLogic/` (all 32 ProcH
sheets, committed) with `Top/DoradoLogic.df` modelled on `AISImages.df` —
CR-terminated, carrying the archive's own dates and byte-exact sizes. A cold
boot attaches them:

```
ProcH32.sil <-- [Cedar]<CedarChest6.1>DoradoLogic>ProcH32.sil!1 {08-Oct-81 21:16:07 EDT}
End: [Cedar]<CedarChest6.1>Top>DoradoLogic.df!1
33 files acted upon
```

### 2.3 Sil is the right viewer and the right command

`Sil.df!5` is a genuine Cedar port — its own first line is
`-- Sil.df  Sil for Cedar6.1`, Ken Pier, November 1986 — shipping `Sil.bcd`,
`Sil.tip`, `Sil.icons`, the `sil.lb5`–`lb8` **symbol libraries** the drawings
are built from, and `SilBiScrollers` for scroll/zoom in a viewer.

The command is `Sil`, from `SilKernelImpl.mesa`:

```
Commander.Register[key: "Sil", proc: StartSilCommand,
                   doc: "Create a Window Sil Instance"]
```

`Help.LSil` (itself a Sil drawing) holds the full Cedar Sil command summary:
`^N` inputs a file relative to the mark, `^O` relative to the origin, mouse
is Select/Draw/Mark, `^Z` makes hardcopy.

---

## 3. The blocker — and why it is probably terminal for Sil

**Short version: CedarChest 6.1 never captured the payload directories for
`biscrollers`, `viewrec`, `popupbuttons` and `cursory` — only their DFs in
`Top`. Everything below follows from that.**

`tools/fetch_cedarchest_app.py` falls back to CedarChest **6.0** when a
payload directory is missing (documented behaviour, and right for
CardTable). So our "6.1" install of those four is 6.0 code behind 6.1 DFs —
a hybrid. Proof: our `BiScrollersImpl.BCD` is byte-identical to
`cyan/cedarchest6.0/biscrollers/BiScrollersImpl.BCD!4`, while
`BiScrollers.df!12` pins `!11` (14-Nov-86). `!11` exists **nowhere** in the
archive; the only BiScrollers payloads anywhere are `cedarchest6.0` and
`indigo/cedarhacks5.2`.

### Four iterations, each moving the failure one link along

| # | what was tried | result |
|---|---|---|
| 1 | 6.1 `Sil.bcd!4` against the (6.0) BiScrollers we have | `VersionMismatch[BiScrollers]` |
| 2 | 6.0 `Sil.bcd!18` instead, same deps | `VersionMismatch[ViewRec]` — past BiScrollers |
| 3 | add 6.0 ViewRec/Abutters/MJSContainers, on the `cedar-sil-wip` base | `Exported Type Clash for interface Abutters` — that base already has the 6.1 chain RUNNING, and two implementations of one interface cannot coexist |
| 4 | same, on a clean `cedar-sil-base` (files fetched, nothing run), with the 6.1 bcds `Delete`d first | `AMTypesAImpl.Error[reason: noSymbols, msg: "ViewRecImpl"]` |

Iteration 4 loads `CursoryImpl`, `PopUpSelection2Impl`, `PopUpButtonsImpl`,
`Geom2DImpl`, `MJSContainersImpl6`, `TypePropsImpl6` and `AbuttersImpl6`
cleanly, then dies in `DoGetTypeSymbols`.

### Why `noSymbols` is the end of this road

ViewRec is a record VIEWER built on Cedar's runtime type system, so loading
it needs **symbol files**, not just code. Symbols are resolved through the
profile's `VersionMap.SymbolsMaps`, which points at
`/Cedar/CedarChest6.1/VersionMap/CedarChestSymbols.VersionMap`. Both maps
are present and served — but they resolve names into
`CedarChest6.1/ViewRec/`, which is exactly one of the four directories the
archive never captured. So the symbols cannot be fetched, from either
vintage, and `Delete`ing the shadowing bcds (iteration 4) does not help
because the problem is absence, not shadowing.

### What is left to try

1. **Point `VersionMap.SymbolsMaps` at a CedarChest 6.0 symbols map**, if
   `cyan/cedarchest6.0/versionmap/` exists — then 6.0 symbols would resolve
   for 6.0 code. This is the one clean shot left and it is cheap.
2. **`indigo/cedarhacks5.2/biscrollers`** is the only other BiScrollers
   payload in the archive; a 5.2-era Sil chain might be internally
   consistent, though it is two releases from our world.
3. **Give up on Sil and use Griffin** (`df!6`, not yet mirrored) or the
   CedarChest **7.0** Gargoyle. Neither is known to read `.sil`.
4. **Render the `.sil` files outside Cedar.** The format is simple and
   documented by `Help.LSil`; a host-side converter to AIS would put the
   drawings on screen through the AISViewer path that already works, losing
   the "edit it in its own editor" part but keeping the vectors.

Note the schematics themselves are NOT blocked by any of this — they are on
the volume already (§2.2). Only the editor is.

## 3.1 Original blocker note

```
% Run Sil
VersionMismatch[BiScrollers]
```

`Sil.bcd!4` is dated **26-Aug-86**. The BiScrollers implementation its own
`BiScrollers.df!12` pins is `BiScrollersImpl.BCD!11`, **14-Nov-86**. Sil was
compiled against an older BiScrollers interface than the release ships
beside it. This is the mixed-vintage trap
`docs/running-the-emulator.md` already describes for AISViewer.

Everything below Sil loads cleanly — 14 of 15 `Run`s succeed: CursoryImpl,
PopUpSelection2Impl, PopUpButtonsImpl, Geom2DImpl, MJSContainersImpl,
TypePropsImpl, AbuttersImpl, ViewRecImpl, BiScrollersImpl,
BiScrollersButtonless, BiScrollersButtonned, SirPressImpl,
ImagerPressFontSubstImpl, ImagerPressImpl, InterpressPackage.

Next moves, cheapest first — all archaeology, no emulator work:

1. Look for an **August 1986 `BiScrollersImpl`** in the archive. The DF pins
   `!11` (November) but earlier versions are kept; `!` versions are browsable
   from the CedarChest6.1 BiScrollers directory index.
2. Look for a **`Sil.bcd` later than `!4`** — CedarChest6.0's Sil, or a
   version under `[Cedar]<CedarChest6.1>Sil>` newer than the DF's pin.
3. Load Sil against the **matching-vintage interface** `BiScrollers.BCD!3`
   (26-Aug-86) *without* running the November impl, following the AISViewer
   recipe's `Delete` insight: attached-but-not-loaded interface bcds shadow
   the loadstate, and after `Delete` symbols resolve from RUNNING modules.
4. If Sil proves unfixable, **Griffin** (`df!6`) is the other PARC
   illustrator and is not yet mirrored.

---

## 4. The profile block that installs it all

This is the only copy — the tree was reverted. Add to
`CommandTool.BootCommands` in `chm/cedar/stp-root/Cedar6.1/Top/User.Profile`,
immediately before the `Bringover ...DoradoWelcome` line. **The file is
CR-terminated** (its LF is inside the Tioga binary trailer; the text body has
none) — write it with `\r`, not `\n`.

```
Bringover -o CursoryImpl.bcd [Cedar]<CedarChest6.1>Top>Cursory
Bringover -o PopUpButtonsImpl.BCD PopUpSelection2Impl.bcd PopUpButton.TIP PopUpSelection2.tip [Cedar]<CedarChest6.1>Top>PopUpButtons
Bringover -o MJSContainersImpl.BCD TypePropsImpl.BCD ViewRecImpl.BCD ViewRec.icons [Cedar]<CedarChest6.1>Top>ViewRec
Bringover -o AbuttersImpl.BCD [Cedar]<CedarChest6.1>Top>Abutters
Bringover -o Geom2DImpl.BCD BiScrollersImpl.BCD BiScrollersButtonless.BCD BiScrollersButtonned.BCD BiScroller.TIP Knob.TIP [Cedar]<CedarChest6.1>Top>BiScrollers
Bringover -o SirPressImpl.bcd [Cedar]<CedarChest6.1>Top>SirPress
Bringover -o ImagerPressImpl.bcd ImagerPressFontSubstImpl.bcd [Cedar]<CedarChest6.1>Top>ImagerPress
Bringover -o InterpressPackage.bcd [Cedar]<CedarChest6.1>Top>Interpress
Bringover -o Sil.bcd Sil.tip Sil.icons sil.lb5 sil.lb6 sil.lb7 sil.lb8 [Cedar]<CedarChest6.1>Top>Sil
Run CursoryImpl
Run PopUpSelection2Impl
Run PopUpButtonsImpl
Run Geom2DImpl
Run MJSContainersImpl
Run TypePropsImpl
Run AbuttersImpl
Run ViewRecImpl
Run BiScrollersImpl
Run BiScrollersButtonless
Run BiScrollersButtonned
Run SirPressImpl
Run ImagerPressFontSubstImpl
Run ImagerPressImpl
Run InterpressPackage
Run Sil
Bringover [Cedar]<CedarChest6.1>Top>DoradoLogic
```

### Why it is shaped like that

- **`Bringover -o <files> <DF>` is the whole trick.** A plain
  `Bringover [Cedar]<CedarChest6.1>Top>Sil` walks the import closure and
  stalls — measured: 21 DFs fetched, then no progress for 70 B cycles
  (~47 emulated minutes), and the typescript frozen mid-ViewRec. The `-o`
  form names the files and skips the closure. The AIS lines already in the
  profile use it; that is what "the profile's selective `-o` Bringovers"
  in `docs/running-the-emulator.md` means.
- **Run order comes from each package's own `.load` file**, not guesswork:
  `Sil.load` = `Install BiScrollers ViewRec Abutters` + `Run SirPressImpl
  ImagerPressFontSubstImpl ImagerPressImpl InterpressPackage Sil.bcd`, and
  each `Install X` expands to that package's `.load`.
- **Data files are easy to forget and fail loudly.** `Run
  PopUpSelection2Impl` dies with `Could not find
  "[]<>Users>Guest.pa>PopUpSelection2.tip"` unless the `.tip` is in the
  `-o` list. Same class for `Knob.TIP` and `ViewRec.icons`.
- **The `.sil` Bringover is plain, not `-p`** — data files need it, per the
  standing trap.

---

## 5. Two bugs found on the way

### 5.1 `.DF` was matched case-sensitively (fixed, `316b1fc`)

`ftp_dates_scan_dir` tested `strcmp(name + n - 3, ".df")`, so the four DFs
the release spells `.DF` — **Abutters, PopUpButtons, SirPress, ViewRec** —
were never scanned. Nothing they export had a creation date, so all of it
was advertised with the synthetic `01-Jan-84` fallback, and BringOver
refuses a file whose advertised date is not the one its DF pinned:

```
[]<>Users>Guest.pa>PopUpButtons.df NOT updated: userSaidNo.
(Probably date mixup)
```

That silently broke installing **any** package whose import closure touches
those four, and PopUpButtons and ViewRec are common dependencies. Fixed;
the trace now reads `14-Nov-86 20:02:52 PST` and the closure completes.

**Not deployed** — see §1.

### 5.2 The desktop bake recipe types the login too early (fixed, `968fe3b`)

`make cedar-desktop-snapshot` typed `Guest` at cycle 760 M. The prompt is no
longer up by then, the keys are lost, and the bake runs its full budget and
snapshots the **login screen** (28,570 px instead of ~167,000) over the good
checkpoint. I did exactly that and restored from a backup.

The cause is ours: serving the LookupFile protocol earlier the same day
removed four retries and a 30-second negative cache from the cold boot,
moving the prompt more than a billion cycles later. Measured with both the
current and the pre-optimisation binary (identical 28,595 px, so the perf
work is *not* implicated): 760 M and 1.2 B lose the keys, 2 B logs in.
Recipe now types at 2.5 B with `--key-hold 1600000`.

**The gap that let it through: every Cedar gate RESTORES a checkpoint; none
cold-boots.** So the one path that builds everything else is ungated. A
cold-boot gate would be slow (~25 min) but covers the thing that, when it
breaks, breaks silently and destructively.

**Always back up `build/good-packs/cedar-desktop.{snap,pdi}` before
rebaking, and check the result is a desktop before trusting it.**

---

## 6. Reproducing

```sh
# stage more boards (32 ProcH sheets are already committed)
#   index:  https://xeroxparcarchive.computerhistory.org/io/doradologic/.index.html
#   note the CASE, and that the .dm!1_/ directories are pre-expanded

# bake, having applied the §4 profile block:
cd dorado
cp build/good-packs/cedar-desktop.snap build/good-packs/cedar-desktop.snap.bak
cp build/good-packs/cedar-desktop.pdi  build/good-packs/cedar-desktop.pdi.bak
make cedar-desktop-snapshot            # ~25 min; MUST end ~167,000 px

# or drive the WIP image that already has everything installed:
./build/dorado-sdl --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk build/good-packs/cedar-sil-wip.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --snapshot-in build/good-packs/cedar-sil-wip.snap --speed 4000000
```

In the desktop: click the CommandTool's `%` prompt for type-in focus, then
`Run Sil` to see the mismatch, or `List [Cedar]<CedarChest6.1>DoradoLogic>*`
to see the 32 sheets the volume already has.
