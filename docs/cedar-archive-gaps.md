# What the CHM archive is missing from Cedar 6.0, 6.1 and 7.0

Written 2026-08-01, to answer a specific question: **is there more at the
Computer History Museum that never went online?** This is the list to take
to them.

Everything here was measured against
`https://xeroxparcarchive.computerhistory.org/cyan/` on 2026-08-01, not
recalled. §7 says how to reproduce it.

---

## 1. The one-paragraph answer

**Cedar 6.1's base release is essentially complete** — 84 of 86 packages
have every file their own DF pins, 13 missing files out of 2,584 (0.5%).
**CedarChest 6.1 is 91% complete** — 281 of 316 packages whole, 596 missing
of 6,422 (9.3%), and the losses are concentrated: 18 packages are missing
their entire payload directory. **Cedar 7.0 is absent** — not incomplete,
absent: zero files. That is the single biggest thing worth asking about.

---

## 2. Headline comparison

| tree | payload dirs | DFs in `top` | pinned files | missing | complete? |
|---|---:|---:|---:|---:|---|
| `cedar6.0` | 98 | 105 | *(not file-audited)* | | 21 DFs have no payload dir |
| **`cedar6.1`** | 96 | 86 | **2,584** | **13 (0.5%)** | 84/86 packages whole |
| `cedar7.0` | 1 | 0 | 0 | — | **empty** |
| `cedarchest6.0` | 265 | 261 | *(not file-audited)* | | 17 DFs have no payload dir |
| **`cedarchest6.1`** | 297 | 316 | **6,422** | **596 (9.3%)** | 281/316 packages whole |
| `cedarchest7.0` | 17 | 14 | *(not file-audited)* | | a 17-package fragment |

"Pinned files" = every `name!version` a package's own `.df` claims to
export. "Missing" = that exact version is not in the directory the DF names.
This is the right measure: a DF is the release's own manifest, so a missing
pinned file is the archive contradicting itself.

---

## 3. Cedar 7.0 — the big ask

`cyan/cedar7.0/` contains **no files at all** and one subdirectory, `cross`,
which itself lists nothing. For scale, `cross-reference.html` mentions
`cedar6.1` 6,272 times and `cedar7.0` **48**.

There is no 7.0 base release: no `Top` DFs, no `Basic.Loadees`, no boot
file, no germ, nothing installable. `cedarchest7.0` is real but small — 17
packages: `checksummer documentation endiandebug finger fsrope gargoyle
keynote lightning pupbootserver pupdebug pupetherboot pupnetwatcher
puproutertool rpcechotool setsandstuff spy top`.

**Ask CHM:** was a Cedar 7.0 release tree ever captured? A 7.0 IFS volume,
a `Top` directory, or a 7.0 boot file would be the single most valuable
addition — it is a whole release generation that is otherwise lost. (Note
we do hold the 7.0 *manual*, `Introduction_to_Cedar_7.0`, so the release
certainly existed.)

---

## 4. Cedar 6.1 base — two gaps, one of which matters to us

Only two of 86 packages are incomplete.

### 4.1 `MicrocodeDorado.df!3` — the whole `cedar6.1/dorado/` directory is absent (11 files)

```
dorado/MicrocodeDoradoWorking.df!1     dorado/Cedar.mb!1      dorado/Mesa.mb!1
dorado/AEmuSources.dm!1                dorado/Cedar.midas!1   dorado/Mesa.midas!1
dorado/DMesaSources.dm!1               dorado/InitialSelect.mb!1
dorado/D1Alu.mc!1                      dorado/D1Lang.mc!1     dorado/LoadMB.run!1
```

`cedar6.1/top/` and `cedar6.1/microcode/` are both fine; it is only the
`dorado/` subdirectory that is gone.

**This is a pointer problem, not a loss.** Every one of those names exists
elsewhere in the archive, and we already hold later versions locally in
`chm/dorado/`: `Cedar.mb!6`, `Mesa.mb!3`, `Cedar.midas!7`, `Mesa.midas!4`,
`LoadMB.run!6`. `AEmuSources` (750 index references) and `DMesaSources`
(2,029) are likewise present elsewhere. So this gap does not block anything
— but it does mean **`cedar6.1/dorado/` was dropped when the volume was
imaged**, which is a hint worth passing on: if that directory was skipped,
others may have been too.

### 4.2 `Germ.df!16` — 2 files

```
germ/BootChannel.mesa!2      germ/BootChannelDisk.mesa!4
```

Sources, not binaries; the germ itself is intact. Minor, but named here
because it is the *only* other gap in the base release, and because the
germ is load-bearing for this project.

---

## 5. CedarChest 6.1 — 35 packages with gaps

Sorted by damage. **`in 6.0`** = a payload directory of that name exists in
`cedarchest6.0`, so the gap may be fillable from the older release (with the
vintage caveat in §6). **`in 7.0`** = same for `cedarchest7.0`.

| package | pinned | missing | in 6.0 | in 7.0 |
|---|---:|---:|:-:|:-:|
| AlgebraStructures | 143 | 142 | yes | |
| AlpineShared | 82 | 81 | yes | |
| AlpineUser | 57 | 55 | yes | |
| Benchmarks | 48 | 42 | | |
| **BiScrollers** | 35 | **34** | yes | |
| MF | 205 | 31 | yes | |
| CaminoReal | 28 | 27 | | |
| SpellingTool | 58 | 25 | yes | |
| Cartoon | 25 | 24 | | |
| AlgebraicSurfaces | 40 | 23 | | |
| CedarExamples | 14 | 13 | yes | |
| Asserting | 9 | 8 | yes | |
| CardTable | 9 | 8 | yes | |
| ArchivistBTree | 8 | 7 | yes | |
| AtomButtons | 8 | 7 | | |
| Cypress | 120 | 7 | yes | |
| XNSPrintingUI | 18 | 7 | | |
| ArpaWatch | 7 | 6 | | |
| BravoToTioga | 7 | 6 | yes | |
| AlpineBackup | 6 | 5 | yes | |
| Args | 6 | 5 | yes | |
| Graphs0 | 18 | 5 | | |
| TeX | 136 | 5 | yes | |
| BigCardinals | 5 | 4 | yes | |
| Catalog | 5 | 4 | yes | |
| AutoBackUp | 4 | 3 | yes | |
| CedarChestRelease | 8 | 3 | | |
| AlpineServer | 3 | 2 | yes | |
| ColorTrix | 60 | 1 | yes | |
| CopyFromVersionMap | 7 | 1 | yes | |
| Griffin | 106 | 1 | | |
| Kipper | 39 | 1 | yes | |
| RegularExpression | 14 | 1 | yes | |
| Sweep | 49 | 1 | yes | |
| ThreeDWorld | 69 | 1 | yes | |

**25 of the 35 have a CedarChest 6.0 payload directory.** Ten do not, and
those are the ones most likely to be genuinely lost: `Benchmarks`,
`CaminoReal`, `Cartoon`, `AlgebraicSurfaces`, `AtomButtons`,
`XNSPrintingUI`, `ArpaWatch`, `Graphs0`, `CedarChestRelease`, `Griffin`.

Also worth noting: **`cedarchest6.1` lists 316 DFs but has only 297 payload
directories**, and `cedarchest6.0` lists 261 DFs against 265 directories.
Some directories exist with no DF (`abbreviations`, `cedargriffin`, `ls`,
`meddle`, `positionrange`, `siroccodeps`, `styles`, `walnutcontrol` in
6.1) — a sign the two were captured by different passes.

---

## 6. The one gap that has actually cost us

**`cedarchest6.1/biscrollers/` — the entire payload directory, 34 of 35
pinned files.** The DF (`BiScrollers.df!12`) is there; the code is not.

This blocks running **Sil**, the PARC logic-drawing editor, and with it the
best demo this project could have: the emulated Dorado displaying its own
schematics in the original vector format. 506 `.sil` sheets of Dorado logic
survive under `io/doradologic/` — ProcH, ProcL, ContA, DskEth, MemC, MemD,
MemX, IFU, BaseBd — and 32 of them are already installed on our emulated
volume. `Sil.bcd!4` loads and stops at `VersionMismatch[BiScrollers]`
because the only BiScrollers binaries anywhere in the archive are
CedarChest **6.0**'s `!4` and `indigo/cedarhacks5.2`'s, and Sil 6.1 was
compiled against `BiScrollersImpl.BCD!11` (14-Nov-86), which exists nowhere.

**The specific request:** `[Cedar]<CedarChest6.1>BiScrollers>` — in
particular `BiScrollersImpl.BCD!11`, `BiScrollers.BCD!3`,
`BiScrollersButtonless.BCD!3`, `BiScrollersButtonned.BCD!4`,
`Geom2DImpl.BCD!1`, `BiScroller.TIP!1`, `Knob.TIP!1`. Any November-1986
CedarChest 6.1 IFS volume should have them.

Detail: `docs/sil-schematics-handoff.md`.

---

## 7. What to ask CHM, in priority order

1. **A Cedar 7.0 release volume** — any `Top`, `Basic.Loadees`, boot file or
   germ. An entire generation is missing (§3).
2. **`<CedarChest6.1>BiScrollers>`** — one directory, 34 files, unblocks Sil
   and the schematics demo (§6).
3. **`<Cedar6.1>Dorado>`** — 11 files. Recoverable from elsewhere, but its
   absence suggests directories were dropped during imaging, so it is
   evidence as much as a request (§4.1).
4. **The other 9 whole-directory losses in CedarChest 6.1** with no 6.0
   fallback: Benchmarks, CaminoReal, Cartoon, AlgebraicSurfaces,
   AtomButtons, XNSPrintingUI, ArpaWatch, Graphs0, Griffin (§5).
5. **A file-level audit of 6.0** has not been run — only 6.1. If CHM is
   looking anyway, `cedar6.0` (21 DFs with no payload) and `cedarchest6.0`
   (17) are worth the same treatment.

A general framing that may help them: **the DFs are the release's own
manifest.** Every gap above is the archive contradicting a Xerox manifest
that is itself online. That makes the ask concrete — not "do you have more
Cedar?" but "your `BiScrollers.df!12` promises 35 files and 34 are absent;
is there a tape with that directory?"

---

## 8. Reproducing

No repo tooling; this was a throwaway audit. The method, if it needs
re-running or extending to 6.0:

1. Directory level — fetch `cyan/<tree>/.index.html` for the subdirectory
   list and `cyan/<tree>/top/.index.html` for the DF list; compare
   lower-cased names.
2. File level — for each DF in `top`, fetch it, parse its `Exports` /
   `Directory` sections (NOT `Imports`), and check each `name!version`
   against the index of the directory the section names.

Two traps that cost time here:

- **The section regex must handle multi-level paths.** A header is
  `Exports [Cedar]<CedarChest6.1>Sil>`; a naive `<([^>]+)>` captures
  `CedarChest6.1` and audits the wrong directory, which reports 100%
  missing for everything. Use the form in
  `tools/fetch_cedarchest_app.py`.
- **Archive paths are case-sensitive, and `chm/cross-reference.html`
  lower-cases them.** `io/doradologic/proch-apcrev-da.dm!1_/` 404s;
  `ProcH-apcRev-Da.dm!1_/` works. Take the case from the parent
  `.index.html`. (Subdirectory names under `cyan/` genuinely are lower
  case; it is the `.dm` archive names that are mixed.)
