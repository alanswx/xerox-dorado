# Cedar, Smalltalk, Mesa/Pilot and Alto in the CHM archives — what exists, per release

Surveyed 2026-08-01 against `xeroxparcarchive.computerhistory.org` **and**
`xeroxalto.computerhistory.org`, to answer, for every non-Lisp system this
project cares about: what does each release actually contain, can it boot on
a Dorado, and what is worth asking CHM for.

Companion documents. `docs/interlisp-archive-survey.md` does the same job for
Interlisp-D. `docs/cedar-archive-gaps.md` is a **file-level** audit of Cedar
6.1 / CedarChest 6.1 against their own DF manifests; this document is the
**release-level** view across systems and does not redo that audit — but §1
corrects three of its conclusions, because they turned out to be search
errors of exactly the kind that document itself warns about.

**Headline: Cedar 7.0 was built for the Dorado.** The project has believed
otherwise. Evidence in §2.4. This is the single most consequential finding
here.

---

## 1. Corrections to previous findings

Four claims in this repo are wrong. All four are the same shape as the
Interlisp mistakes: a confident "it is not in the archive" produced by a
search that did not go deep enough, or looked on the wrong server.

### 1.1 "Cedar 7.0 is absent — not incomplete, absent: zero files"
(`docs/cedar-archive-gaps.md` §1, §3)

`cyan/cedar7.0/` does list zero files and one subdirectory, `cross`. But
`cross` **also** lists zero files and three subdirectories:
`documentation`, `makeboot`, `top`. Those hold **23 files, 0.3 MB,
1984-1986** — the MakeBoot cross-compiler that builds Pilot boot files
(`MakeBoot.df!3`, `MBLoaderCore.mesa!1`, `MBOutput.mesa!1`, …, all
`Wyatt.pa`, latest 12-Dec-1986).

This is the "empty root, populated grandchild" trap, one level deeper than
the `phylum/lispusers` case. It does **not** change the ask — there is still
no Cedar 7.0 `Top`, no `Basic.Loadees`, no boot file, no germ — but "zero
files" was not true and the tool that builds 7.0 boot files is in the
archive.

### 1.2 "`cedar6.1/dorado/` is absent (11 files)"
(`docs/cedar-archive-gaps.md` §4.1)

All 11 files are present, at **`cyan/cedar6.1/microcode/dorado/`**, with the
exact names and versions the DF pins, plus two expanded `.dm` archives
(`AEmuSources.dm!1_`, `DMesaSources.dm!1_`). Verified live:

```
AEmuSources.dm!1  322297   Cedar.mb!1  67284   Cedar.midas!1     832
DMesaSources.dm!1 511170   Mesa.mb!1   72608   Mesa.midas!1      791
InitialSelect.mb!1 10894   D1Alu.mc!1   3428   D1Lang.mc!1     44393
LoadMB.run!1        8302   MicrocodeDoradoWorking.df!1  1159
```

The cause is a DF-header parse error. `MicrocodeDorado.df!3` says

```
Directory [Cedar]<Cedar6.1>Microcode>Dorado>
```

— a **three**-component path. A parser that keeps the first and last
components resolves it to `cedar6.1/dorado`, which does not exist, and
reports the whole package missing. The gaps document warns about
two-component headers in its own §8 and still lost to the three-component
form.

Consequence: ask item 3 in that document ("`<Cedar6.1>Dorado>` — 11 files")
should be withdrawn, and so should the inference drawn from it, that
"`cedar6.1/dorado/` was dropped when the volume was imaged … if that
directory was skipped, others may have been too." No directory was skipped.

### 1.3 "`Germ.df!16` — 2 files missing"
(`docs/cedar-archive-gaps.md` §4.2)

`Germ.df!16` pins `germ/BootChannel.mesa!2` and
`germ/BootChannelDisk.mesa!4`; `cyan/cedar6.1/germ/` holds `!1` of each.
Both pinned versions exist one directory over, in **`cyan/cedar6.0/germ/`**,
at the same sizes and dates (3009 bytes / 13-Feb-1985; 11632 bytes /
27-Feb-1985). Version numbers are per-directory in IFS, so a release that
re-copies a file restarts its numbering; the *content* is not lost. Nothing
to ask for.

### 1.4 "Medley does not appear anywhere in this archive"
(`docs/interlisp-archive-survey.md` §5, item 2)

Out of scope for this document but found while enumerating volumes, so
recorded here: **`phylum/medley/` exists** — 22 files, 10.7 MB, 1988-1989,
including `phylum/medley/1.0/LISP.SYSOUT!1` (5,000,704 bytes, 09-Nov-1988)
and `Released-Full.sysout!1` (6,134,784), plus a `patches` directory of 8
`.LCOM` / 1 `.DFASL` AR fixes. Medley 1.0 is in the archive with two
bootable sysouts. That survey's ask item 2 should be withdrawn.

### 1.5 How the three stated traps were avoided here

- **Empty root, populated subtree.** Every count in this document comes from
  the recursive filename index, not a directory root. `cyan/cedar7.0` (§1.1),
  `_cd8_/doradomicrocode/7.0` (also zero files, one subdirectory `top`
  holding `Micro.df!2` and `MicroD.df!2`) and `cyan/cedar6.1/microcode`
  (2 files, one subdirectory holding 11) would each have been reported empty
  by a root-only scan.
- **Extensions change with the release.** No release here was characterised
  by a single extension. The per-release "dominant kinds" columns are the
  top extensions by count, computed over everything — which is how the
  Cedar-era shift from `.mesa`/`.bcd`/`.press` to `.tioga`/`.load`/`.install`,
  and Portable Cedar's `.mob`/`.o`/`.c`/`.z`, show up at all.
- **Case.** Every match is lower-cased on both sides. It matters in this
  material: `cyan/cedarchest7.0/top` holds `SetsAndStuff.DF!12` next to
  `Spy.df!11`; `_cd8_/dorado` holds `MEMA.DM!18` next to `memMisc.dm!11`;
  `indigo/basicdisks` holds `Mesa5.bfs!1` next to `NonProg.BFS!16`;
  `indigo/doradosource` holds `DoradoMicroAssembler.Dm!3`. A case-sensitive
  `.df` filter drops 8 of CedarChest 7.0's 17 DFs.

A fourth trap, specific to this material, cost real time and is worth
naming: **the two CHM servers are not mirrors, and `chm/cross-reference.html`
indexes only one of them.** `SmalltalkDorado.eb!1` and
`Indigo/BasicDisks/Smalltalk14.bfs!1` — the entire Smalltalk boot chain —
exist **only** on `xeroxalto`, and therefore appear nowhere in the local
cross-reference. See §3.

---

## 2. Cedar

### 2.1 Release totals

Unique `name!version` per directory, over the whole subtree.

| release | era | files | size | dominant kinds |
|---|---|---:|---:|---|
| Cedar 3.0 - 5.0 (accreted) | 1979-1984 | 9,991 | 120.0 MB | 3957 bcd, 3641 mesa, 598 df, 475 cm, 266 mc |
| CedarHacks 5.2 | 1982-1985 | 409 | 3.6 MB | 140 mesa, 134 bcd, 26 load, 25 tioga |
| PostCedar 5.2 | 1981-1985 | 1,533 | 15.3 MB | 739 mesa, 173 bcd, 133 load, 115 df, 50 ais |
| Cedar 5.3 | 1979-1985 | 3,965 | 52.1 MB | 1621 bcd, 1560 mesa, 215 df, 109 load |
| Cedar 6.0 | 1979-1986 | 3,671 | 43.5 MB | 1470 bcd, 1394 mesa, 194 cm, 119 mc, 107 df |
| **Cedar 6.1** | 1979-1986 | **3,135** | **36.3 MB** | 1298 bcd, 1238 mesa, 180 cm, 87 df, 59 mc |
| Cedar 7.0 | 1984-1986 | **23** | 0.3 MB | 18 mesa — the MakeBoot tool only |
| CedarChest 6.0 | 1980-1986 | 6,797 | 74.7 MB | 2507 bcd, 2386 mesa, 443 load, 311 tioga |
| **CedarChest 6.1** | 1980-1987 | **6,748** | **72.3 MB** | 2286 bcd, 2216 mesa, 558 load, 371 tioga |
| CedarChest 7.0 | 1984-1988 | 488 | 9.7 MB | 264 mesa, 154 bcd, 17 tioga, 17 df |
| CedarCommon 2.0 | 1982-1993 | 1,245 | 17.6 MB | 846 mesa, 115 tioga, 69 df, 59 ip |
| Portable Cedar (to 10.1) | 1980-1993 | 14,371 | 323.9 MB | 3442 mesa, 3290 mob, 2046 o, 1573 z |

Precursors, all smaller and none Dorado-bootable except ISLCedar:

| tree | era | files | size |
|---|---|---:|---:|
| `indigo/precedar` | 1982-1983 | 89 | 1.4 MB |
| `indigo/cedarviewers` | 1981-1983 | 262 | 1.8 MB |
| `ivy/cedarviewers` | 1982-1983 | 9 | 0.0 MB |
| `cyan/preisl` / `cyan/preislcedar` | 1980-1984 | 334 / 1,843 | 6.1 / 16.3 MB |
| `cyan/isl` / `cyan/islcedar` | 1980-1983 | 501 / 893 | 3.3 / 10.8 MB |
| `indigo/cedarpeach` | 1985-1986 | 89 | 0.6 MB |

Satellites of the 6.x releases:

| tree | era | files | size | what |
|---|---|---:|---:|---|
| `cyan/cedarfonts` | 1979-1988 | 1,297 | 21.4 MB | **TeX** fonts (663 tfm, 85 gf, Computer Modern) — not Tioga screen fonts |
| `_cd6_/tioga` | 1979-1985 | 457 | 0.6 MB | the **Tioga** fonts: 259 tfm, 122 ks, 69 strike |
| `indigo/fonts` | 1980-1986 | 134 | 4.6 MB | press fonts + `top/TiogaFonts.df!5` (28-May-1985) |
| `cyan/cedarlang` | 1979-1983 | 898 | 8.3 MB | Cedar language / compiler sources |
| `cyan/cedarprinter6.1` | 1983-1987 | 239 | 4.4 MB | Raven/Interpress printer world (DLion only) |
| `cyan/cedarps` | 1984-1987 | 449 | 5.4 MB | PostScript printing (DLion only) |
| `_cd8_/cedardocs` | 1978-1985 | 239 | 8.2 MB | 65 press, 37 bravo — early Cedar documents |
| `_cd8_/cedarlib` | 1979-1986 | 70 | 0.8 MB | includes `cedarnetexec/CedarNetExec.boot!4` |

### 2.2 Where each release lives

| release | path(s) |
|---|---|
| Cedar 3.0 - 5.0 | `indigo/cedar` (one accreted tree; see below) |
| Cedar 5.2 | no tree of its own — `indigo/postcedar5.2`, `indigo/cedarhacks5.2` |
| Cedar 5.3 | `indigo/cedar5.3` |
| Cedar 6.0 / 6.1 / 7.0 | `cyan/cedar6.0`, `cyan/cedar6.1`, `cyan/cedar7.0` |
| CedarChest 6.0 / 6.1 / 7.0 | `cyan/cedarchest6.0`, `…6.1`, `…7.0` |
| Cedar 7.0-era networking | `pixel/ncp` (891 files, 11.0 MB, 1985-1989) |
| Dorado microcode, all releases | `_cd8_/doradomicrocode` (434 files, 1980-**1990**) |
| CedarCommon 2.0 | `_cdmiwok_01_/cedarcommon2.0` |
| Portable Cedar 10.x | `_cdcsl_93-16_/1/cedar` |

`indigo/cedar` is not one release: it is the release directory as it stood
at the end, and its `documentation/` subdirectory holds the release messages
for **3.0, 3.1, 3.2, 3.3, 3.4, 3.4.1, 3.5, 3.5.1, 3.5.2, 4.0, 4.2, 4.4 and
5.0** (`CedarRelease3.0.msg!1` 10-May-1982 through `Cedar5.0.tioga!1`
21-Dec-1983). Its 507 `top` DFs carry 198 distinct package names against 178
payload directories, i.e. multiple versions of each package are retained.
Treat it as "Cedar 5.0, with its own history in place".

**Naming trap.** `_cd8_/cedardocs/lang/Cedar7T7.press`, `Cedar7T10.press`,
`cyan/cedarlang/documentation/cedar7t11.press` (1981-82) are **not** Cedar
7.0. They are numbered drafts of the Cedar *language* reference, five years
earlier than the 7.0 release.

### 2.3 Can it boot on a Dorado?

The Dorado Cedar chain needs five things: the microcode (`CedarDorado.eb`),
the germ (`Dorado.germ`), the boot file (`BasicCedarDorado.boot` or `.pb`),
the loadee list (`Basic.Loadees`), and the Dorado device heads
(`HeadsDorado`).

| release | ucode `.eb` | germ | boot file | `.pb` | Loadees | HeadsDorado |
|---|---|---|---|---|---|---|
| ISLCedar (Sep 83) | — | — | `BasicCedarDorado.boot!1`, `CedarDorado.boot!1` | — | — | — |
| Cedar 5.0 | `CedarDorado.eb!1`, `DoradoCedar.eb!6` | `Dorado.germ!3`, `!4` | `!13`, `!14`; `CedarDorado.boot!58`, `!59` | `!1` | — | `.bcd!1` |
| Cedar 5.3 | `CedarDorado.eb!1` | `Dorado.germ!1` | `BasicCedarDorado.boot!36` | `!14` | `Basic.Loadees!14` | `.bcd!1` |
| Cedar 6.0 | `CedarDorado.eb!1` | `Dorado.germ!6` | `!69` | `!32` | `Basic.Loadees!13` | `.bcd!14` |
| **Cedar 6.1** | `CedarDorado.eb!1` | `Dorado.germ!6`, `Dorado10.germ!5` | `!22` | `!22` | `Basic.Loadees!3` | `.bcd!1`, `.bcd!6` |
| Cedar 7.0 | — (see §2.4) | — | — | — | — | — |

Every release from 5.0 on is complete for Dorado except 7.0. Three details
worth keeping:

- **`CedarDorado.eb!6` and `CedarDorado.eb!1` are the same file.** Both are
  31,888 bytes dated 17-May-1984; the version suffix is per-directory
  (`_cd8_/dorado` vs `cyan/cedar6.1/top`). Our `--eb CedarDorado.eb!6` is
  the 6.1 release microcode.
- **Cedar 6.1 ships a second Dorado germ**, `Dorado10.germ!5` (18,432 bytes,
  05-Jun-1986), built from `GermDorado10.bcd!5` / `GermDorado10.config!1`,
  with its own `Dorado10.loadmap!5`. It pairs with the `CedarDorado10MB.eb`
  / `Cedar10MB.mb` microcode in `_cd8_/doradomicrocode`. Untested here; the
  archive does not say what "10" means, and I did not determine it.
- **6.1 also ships an ether-loadable germ**, `CedarDorado.eg!6`
  (`cyan/cedar6.1/germ`), and `io/murray` has `Cedar5Dorado.eg!1` and
  `CedarDorado.eg!1`. The `.eg` route is the germ-over-Ethernet path that
  `docs/ethernet-local-boot-plan.md` calls Stage 2.

Only **six Dorado germs exist in either archive**, total:
`indigo/cedar/top/Dorado.germ!3` (24-May-1983) and `!4` (20-Dec-1983),
`indigo/cedar5.3/top/Dorado.germ!1` (29-May-1984),
`cyan/cedar6.0/top/Dorado.germ!6` (25-Jun-1985),
`cyan/cedar6.1/top/Dorado.germ!6` (05-Jun-1986),
`cyan/cedar6.1/germ/Dorado10.germ!5`. Everything else with a `.germ`
extension is D0, DLion, Daybreak or Dicentra. The project's
`Dorado.germ-6.1.6` is the last one Xerox built for the machine — with the
possible exception of a 7.0 germ that is not in the archive.

### 2.4 Cedar 7.0 **was** built for the Dorado

`CLAUDE.md` states "Cedar 7 was never built for Dorado". The archive
contradicts that. Four independent witnesses:

1. **A Cedar 7.0 Dorado device-head package is named by a DF that is
   online.** `_cd8_/doradomicrocode/top/EtherTester.df!3` (26-Jul-1990)
   contains, verbatim:

   ```
   Imports [Cedar7.0]<Top>HeadsCommon.df Of ~=
   Imports [Cedar7.0]<Top>HeadsDLion.df Of ~=
   Imports [Cedar7.0]<Top>HeadsDorado.df Of ~=
   ```

   `HeadsDorado` is the package that exists only to drive Dorado hardware.
   A Cedar 7.0 `Top` that exports it is a Cedar 7.0 built for the Dorado.

2. **A Cedar 7.0 boot-package import from Dorado microcode.**
   `_cd8_/doradomicrocode/top/BlockOps.df!5` imports
   `[Cedar7.0]<Top>BootPackages.df` and `[Cedar7.0]<Top>VM.df`; its product
   is `blockops/BlockCedar.eb!1` (32,848 bytes, 18-Jan-1988) — Dorado
   microcode, built against 7.0.

3. **Dorado microcode kept being released after 6.1.**
   `_cd8_/doradomicrocode/top` holds `CedarDorado.eb!4` and
   `CedarDorado10MB.eb!4` (both 17-Feb-1987), `Cedar.mb!5` / `Cedar10MB.mb!4`
   (same date), `Kernel.mb!1` (29-Sep-1987), and `RastDorado.eb!6` /
   `Rast.mb!6` (**23-May-1989**). The tree's DFs are dated to 26-Jul-1990.
   The machine was still being microcoded three years after Cedar 6.1.

4. **A Dorado Cedar world binary from a year after 6.1 is in the archive.**
   `pixel/ncp/top/BasicCedarDorado.boot!1` — **771,584 bytes,
   10-Dec-1987, demers.pa**, against 6.1's `BasicCedarDorado.boot!22` at
   542,720 bytes / 03-Dec-1986. Every DF in `pixel/ncp/top` imports
   `[Cedar]<Cedar7.0>Top>` and `[Cedar]<CedarChest7.0>Top>`.

   Its boot-file version word is the one our germ accepts. Reading the first
   words of each Dorado boot file (`Range: bytes=0-31`) gives:

   | file | date | version word |
   |---|---|---|
   | `indigo/cedar/top/OthelloDorado.boot!8` | 09-Jun-1983 | `062400` |
   | `indigo/cedar/top/BasicCedarDorado.boot!14` | 21-Dec-1983 | `062400` |
   | `indigo/cedar5.3/top/BasicCedarDorado.boot!36` | 04-Mar-1985 | `063000` |
   | `cyan/cedar6.0/top/BasicCedarDorado.boot!69` | 02-May-1986 | `063000` |
   | `cyan/cedar6.1/top/BasicCedarDorado.boot!22` | 03-Dec-1986 | `063000` |
   | **`pixel/ncp/top/BasicCedarDorado.boot!1`** | **10-Dec-1987** | **`063000`** |

   `063000` is the value `docs/CONTINUE-HERE.md` records for the boot file
   we load today; `062400` is the value that makes `OthelloDorado.boot!8`
   version-dead. So the December-1987 world is in the same boot-file
   generation as everything from 5.3 onward, and is a live candidate for
   `--boot-file`.

What is **not** established: whether a Cedar 7.0 *germ* for the Dorado
exists anywhere (none is in either archive), and whether the December-1987
boot file is a 7.0 world or a late 6.1 world carrying NCP. The archive does
not label it, and I did not download or disassemble the 771 KB payload. The
honest statement is: **Cedar 7.0 had a Dorado head, Dorado microcode was
built against 7.0 through 1990, and a 1987 Dorado Cedar world exists — but
the 7.0 release tree itself is missing, so 7.0-on-Dorado cannot currently be
assembled from the archive.**

### 2.5 CedarChest, per release

| release | DF names in `top` | payload dirs | files | size |
|---|---:|---:|---:|---:|
| CedarChest 6.0 | 262 | 265 | 6,797 | 74.7 MB |
| CedarChest 6.1 | 316 | 297 | 6,748 | 72.3 MB |
| CedarChest 7.0 | 14 | 17 | 488 | 9.7 MB |

The 316 figure matches `docs/cedarchest-packages.md`. CedarChest 7.0's 17
packages are `checksummer documentation endiandebug finger fsrope gargoyle
keynote lightning pupbootserver pupdebug pupetherboot pupnetwatcher
puproutertool rpcechotool setsandstuff spy top`, dominated by
`setsandstuff` (280 files) and `gargoyle` (97 `.mesa`, no binaries). Its DFs
run to 23-Nov-1988 — the latest CedarChest material in the archive.

`cyan/cedarchest6.1/biscrollers/` is confirmed absent (the payload directory
exists in `cedarchest6.0`, `indigo/cedarhacks5.2`,
`_cdmiwok_01_/cedarcommon2.0` and Portable Cedar, but not in 6.1). The Sil
ask in `docs/cedar-archive-gaps.md` §6 stands unchanged.

### 2.6 Cedar after 7.0

Cedar did not stop; it left the Dorado. Two later trees:

- **`_cdmiwok_01_/cedarcommon2.0`** — 1,245 files, 17.6 MB, 1982-1993.
- **`_cdcsl_93-16_/1/cedar`** — 14,371 files, **323.9 MB**, 1980-1993, of
  which `release/` is 14,061 files across 369 packages. Extensions tell the
  story: 3,442 `.mesa` but also 3,290 `.mob`, 2,046 `.o`, 1,573 `.z`, plus
  `.c` and `.h` — this is Cedar compiled through `c2c` to C and run on Sun
  workstations under `x11`. `release/cedarrelease/` holds
  `Cedar10.0PreReleaseMessage.tioga` and `Cedar10.1ReleaseMessage.tioga`
  (1992-93), and `release/top` has 414 DFs.

Neither is Dorado-relevant, but both matter to the question "was 6.1 the
end of Cedar?" — no; 6.1 was the end of Cedar *on this hardware*.

---

## 3. Smalltalk

The single most important structural fact: **Smalltalk lives almost entirely
on `xeroxalto`, and the Dorado microcode for it lives almost entirely on
`xeroxparcarchive`.** Neither server alone holds a working set, and
`chm/cross-reference.html` — generated from `xeroxparcarchive` — contains no
Smalltalk anything except two 251-byte `Smalltalk.midas` files. Searching
only the local cross-reference produces "Smalltalk is not in the archive",
which is false.

### 3.1 Totals

| tree | server | era | files | size | what |
|---|---|---|---:|---:|---|
| `Filene/Smalltalk-76` | xeroxalto | 1977-1981 | 34 | 3.2 MB | the Smalltalk-76 release |
| `Indigo/BasicDisks/Smalltalk14.bfs!1` | xeroxalto | 1976-1980 | 44 members | 3.3 MB | an installed Alto Smalltalk disk (4,057,490 bytes packed) |
| `Io/Murray/SmalltalkDorado.eb!1` | xeroxalto | 12-Jul-1983 | 1 | 32,208 B | **the Dorado DSemu microcode we boot** |
| `_cd8_/dorado` + `indigo/dorado` | parcarchive | 1980-1984 | 4 | 0.26 MB | `DSemu.dm!17`, `DSemu.mb!1` |
| `_cd8_/doradosource/dsemusources.dm!17_` | parcarchive | 1981-1983 | 22 | — | DSemu build sources (`DSemuRelease.cm`, `DSemuFiles.cm`) |
| `indigo/dlion/bj/ST80MesaDaybreak8K.db!1` | parcarchive | 05-Mar-1988 | 1 | 64,984 B | Smalltalk-80 on Daybreak — not Dorado |

`Filene` is a volume that exists **only** on `xeroxalto`; there is no
`filene` under `xeroxparcarchive`.

### 3.2 What is in each, by version

**Smalltalk-76** (`Filene/Smalltalk-76`, the 5.5j / 5.5kXM era):

| kind | files |
|---|---|
| worlds | `release.boot!3` (1,002,496 B, 21-Nov-1980), `QuickSilver.boot!1` (1,592,832 B, 22-Jan-1981) |
| Alto-side loaders | `smalltalk.run!2`, `Smalltalk.Run.5.5j!1`, `xmsmalltalk.run!6`, `XmSmalltalk.Run.5.5j!1`, `smalltalk.syms!2`, `XmSmalltalk.Syms!6` |
| microcode | `BYTERP.MB!1` + `BYTERP.MU!1` (the non-XM byte interpreter), `XmByterp.mb!2` + `XmByterp.mu!1` + `XmByterp.mb.5.5j!1` (**the XM build**), `MIcrofloatmc.mb!1` + `.mu` + `.br` |
| change sets | `5.5j.changes.st!5`, `microchanges-5.5kXM.st!2`, `microchanges-5.5kXM-undo.st!1` |
| docs / art | `document.press!1` (136 KB), `syntax.press!1`, `caveats.press!1`, `Aurora.press!1`, `microSmall.memo!1`, 5 `.pic` bitmaps, `fonts.widths!1` |

**Smalltalk-80**, December 1980 V1, inside `Smalltalk14.bfs!1`:

```
st80dec16.im        498,604   20-Dec-1980   the ST-80 image
st80sources.v00     821,336   16-Dec-1980   sources
st80changes.v00      51,317   20-Dec-1980
st80dec16.changes    51,317   20-Dec-1980
d80load.run          54,884   19-Dec-1980   the Alto-side ST-80 loader
d80load.syms          3,916
small.boot          978,432   20-Dec-1980   an ST-76 world on the same disk
smalltalk.syms        7,430
```

plus a normal Alto disk around it (`Executive.Run`, `BRAVO.RUN`, `Ftp.Run`,
`Swat`, `Sys.Boot`, 14 `.AL` fonts). This is a complete, installed Alto
Smalltalk pack — the closest thing in either archive to the
`AltoInfo/ContrAlto2-beta/Disks/xmsmall.dsk` we currently boot from.

### 3.3 Can it run on a Dorado?

| version | Dorado microcode | world | verdict |
|---|---|---|---|
| Smalltalk-76 (5.5kXM) | `SmalltalkDorado.eb!1` (xeroxalto `Io/Murray`) or built from `DSemu.mb!1` / `DSemuSources.dm!17` | `release.boot!3` or `small.boot`, both Alto boot files | **yes** — this is what `make run-smalltalk` does, via the Executive's `Bootfrom` |
| Smalltalk-80 (Dec 1980) | same DSemu microcode; DSemu is the Alto emulator plus the ST bytecode set | `st80dec16.im` + `d80load.run` | **untested.** All the pieces are in `Smalltalk14.bfs`; nobody here has tried it |
| Smalltalk-78 | — | — | **not present** in either archive under any spelling |

Notes that matter for actually doing this:

- Our current pack comes from **ContrAlto, not CHM**. `xmsmall.dsk` is a
  ContrAlto beta distribution disk. The archive's equivalent is
  `Smalltalk14.bfs!1`, which is a `.bfs` (IFS boot-file-set dump), not a
  raw pack — it would need unpacking and rebuilding into a Trident pack the
  way `dsk2trident` does today. The archive holds no file named
  `xmsmall.boot`; the XM release pieces are `xmsmalltalk.run!6` +
  `XmByterp.mb!2` + `release.boot!3`.
- `SmalltalkDorado.eb!1` is dated **12-Jul-1983**, one day before
  `DSemu.dm!17` (13-Jul-1983) — the `.eb` is the shipped form of that source
  drop, which is why our `mb2eb` build of `DSemu.mb!1` (16-Apr-1984) is a
  slightly later microcode than the `.eb`.

---

## 4. Mesa and Pilot

### 4.1 Totals

| tree | era | files | size | dominant kinds | what |
|---|---|---:|---:|---|---|
| `indigo/mesa` | 1978-1983 | 1,506 | 18.8 MB | 624 bcd, 516 mesa, 89 bravo | Mesa 5/6 system, XDebug, Pup, FTP, doc |
| `indigo/xmesa` | 1978-1979 | 213 | 3.8 MB | 40 bcd, 37 mesa, 24 cm, 14 mu | XMesa (the Alto XM Mesa) |
| `indigo/nucleus` | 1981-1983 | 841 | 7.9 MB | 368 mesa, 365 bcd | the Pilot nucleus sources |
| `_cd8_/portola` | 1978-1993 | 177 | 11.9 MB | 136 txt, 12 boot, 4 eb, 3 germ | Pilot release engineering |
| `cyan/d0` | 1975-1984 | 983 | 23.9 MB | 155 cm, 89 tst, 67 mb, 41 eb | D0 microcode + Pilot releases |
| `indigo/dlion` | 1980-1989 | 766 | 13.0 MB | 296 mc, 73 db, 48 df | Dandelion microcode and germs |
| `_cd8_/doradomicrocode` | 1980-1990 | 434 | 3.8 MB | 85 cm, 70 mc, 13 mb, 13 df | **Dorado microcode as a release** |

### 4.2 Pilot releases represented

Pilot releases are named, not numbered. Those with boot material:

| codename | where | dated | machines |
|---|---|---|---|
| Trinity | `cyan/d0/trinity` | 1983-84 | D0 (`PilotD0.eb!4`, `!5`, `PilotTor.eb`) |
| Rubicon | `cyan/d0/archive` | 1982-83 | D0 (`RubiconInitialAltoD0.Eb!1-3`) |
| Sierra | `_cd8_/portola/sierra` | Jun 1983 | D0 (`TajoD0.boot!1`, `CoPilotD0.boot!1`, `D0.germ!1`) |
| Klamath | `_cd8_/portola/klamath`, `cyan/d0/klamath` | 1983-84 | D0 + DLion (`TajoD0.boot`, `CoPilotD0.boot`, `D0.germ!1`, `DLion.germ!1`) |
| Sequoia | `_cd8_/portola/sequoia` | Nov 1985 | DLion (`TajoDLion.boot!1`) |
| Calaveras | `_cd8_/portola/calaveras` | 1984-85 | D0 + DLion (`TajoD0.boot!1`, `PupGatewayDLion.boot!9`) |
| Mesa 10.0 | `indigo/mesa/10.0/basics` | 1982-83 | DLion (`TajoDLion.boot!1`, `CoPilotDLion.boot!1`, `DLion.germ!1`) |

**Every Tajo and CoPilot boot file in either archive is D0 or DLion.** There
is no `TajoDorado.boot`, no `CoPilotDorado.boot`, no `PilotDorado.eb`. I
checked `_cd8_/portola` (all seven subdirectories), `cyan/d0`,
`indigo/mesa`, `indigo/dlion`, `io/murray` on both servers, and a
whole-index filename scan for `tajo|copilot|pilot` with a `.boot`/`.db`/`.pb`
extension. Whether a Dorado Tajo was ever built is not something the archive
answers — the Mesa Development Environment had moved to the Dandelion by the
time these releases were cut — but if one existed it is not online.

What the Dorado *does* have on the Mesa side:

| artifact | where | dated |
|---|---|---|
| `Mesa.mb!3` (Dorado Mesa microcode) | `_cd8_/dorado`, `indigo/dorado` | 28-Jan-1984 |
| `Mesa.mb!1` (same file, release copy) | `cyan/cedar6.1/microcode/dorado` | 28-Jan-1984 |
| `AltoMesaDorado.eb!1/!2` | `cyan/cedar6.0/top`, `cyan/cedar6.1/top`, `indigo/dorado`, `io/murray` | 28-Jan-1984 |
| `DoradoMesa.eb!4`, `DoradoInitialMesa.eb!4` | `indigo/cedar/top` | 23-May-1983 |
| `InitialAltoMesaDorado.eb!1/!2/!3` | four trees | 1983-84 |
| `MesaNetExec.boot!1`, `AlphaMesaMesaNetExec.boot!1` | xeroxalto `Io/Murray` | 1983-84 |
| germs | see §2.3 — the Dorado germ ships **only** inside a Cedar release | |

That last point is the structural answer to "which Mesa/Pilot releases are
represented for the Dorado": none, as a separate product. The Dorado's Pilot
germ and boot files were shipped as part of Cedar, which is why the project's
`Dorado.germ-6.1.6` comes out of `cyan/cedar6.1/top` and not out of a Pilot
release directory. `docs/chm-urls.md` should probably say so.

---

## 5. Alto OS and the BCPL world (light treatment)

| tree | server | era | files | size | what |
|---|---|---|---:|---:|---|
| `Io/Murray` | xeroxalto | 1979-1985 | 90 | 8.9 MB | 50 `.boot` — **the games and tools we serve over EFTP** |
| `io/murray` | parcarchive | 1979-1985 | 106 | 10.8 MB | same tree, 17 Cedar/D0 files more, `SmalltalkDorado.eb` fewer |
| `_cd8_/bootfiles` | both | 1980-1986 | 22 | 0.6 MB | `NETEXEC.BOOT!8`, `COPYDISK`, `FTP`, `DMT`, `SCAVENGER`, + `.syms` |
| `_cd8_/alto` | both | 1975-1986 | 534 | 8.0 MB | 90 `.run`, 44 `.syms`, 40 `.dm` package dumps |
| `_cd8_/altodocs` | both | 1976-1983 | 107 | 5.4 MB | 53 press, 44 tty |
| `indigo/altosource` | both | to 1986 | 1,559 | 15.7 MB | 634 `.bcpl` — the OS and utility sources |
| `indigo/basicdisks` | parcarchive | 1975-1986* | 809 | 38.7 MB | 9 expanded `.bfs` basic disks |
| `Indigo/BasicDisks` | xeroxalto | 1975-1986* | 854 | 45.8 MB | the same 9 **plus `Smalltalk14.bfs!1`** |
| `indigo/bravox` | both | 1979-1981 | 385 | 8.0 MB | BravoX sources |
| `indigo/cslcopydisk` | both | 1978-1985 | 322 | 11.7 MB | 3 `.altodisk` images incl. `Clark-Games.altodisk!1` |
| `indigo/altofonts` | both | 1976-1987 | 610 | 1.4 MB | 221 strike, 195 al, 194 ks |

\* one row inside the expanded `.bfs` archives carries a corrupt `2035` date;
the real range ends in 1986.

The 50 boot files in `Io/Murray` are the source of everything the emulator
serves on the Alto path: `Galaxian.boot!1` (57,238, 17-Sep-1981),
`Invaders.boot!1`, `MazeWar.boot!1`, `PinBall.boot!1`, `PPong.boot!1`,
`Pupwatch.boot!1`, `TriEx.boot!1`, `MissileCommand.boot!1`, `AstroRoids`,
`StarWars`, `Trek`, `Reversi`, `Fly`, `Pool`, `Kal`, `Neptune`,
`NetExec.boot!1`, `MesaNetExec.boot!1`, `NewOs.boot!1`, plus diagnostics
(`BFSTest`, `CRTTest`, `KeyTest`, `MadTest`, `PupTest`, `DMT`, `EDP`,
`Scavenger`, `EtherWatch`, `EtherLoad`).

`indigo/cslcopydisk/Clark-Games.altodisk!1` (1,703,634 bytes, 15-Apr-1985) is
an installed Alto games pack — `PacMan7.RUN`, `PPong.run`, `Maze.run`,
`Pool.run`, `Polygons.run`, `Kinetic4.run`, `Equinox.run`, `Dali.run`,
`AClock.run` — on both servers. It is a plausible independent check on the
`games-trident.pack` reconstruction, and `PacMan7` is not among the boot
files we already serve.

---

## 6. Everything else Dorado-relevant

| tree | era | files | size | notes |
|---|---|---:|---:|---|
| `io/doradologic` | 1978-1988 | 3,449 | 94.0 MB | **2,124 `.sil`** across 83 board-revision `.dm` archives — every revision of ProcH, ProcL, ContA, DskEth, MemC/D/X, IFU, DispM, BaseBd |
| `io/doradodrawings` | 1978-1985 | 107 | 14.0 MB | 59 press, 46 ps — the plotted sheets |
| `io/doradobuilddocs` | 1980-1981 | 21 | 0.1 MB | 21 `.sil` |
| `_cd8_/doradosource` | 1976-1986 | 1,047 | 14.2 MB | 342 cm, 213 mc, 179 bcpl |
| `indigo/doradosource` | 1976-1985 | 1,527 | 20.8 MB | superset of the above: 561 cm, 395 mc, 223 bcpl |
| `_cd8_/dorado` | 1976-1986 | 252 | 6.9 MB | the canonical tree: `AEmu.mb!2`, `Cedar.mb!6`, `Mesa.mb!3`, `DSemu.mb!1`, `TriEmu.mb!3`, `TriMesa.mb!3`, `Ifu.dm!51`, `bootstrap.dm!20`, `doradobaserom.dm!12`, `UnBug.bfs!1` |
| `indigo/dorado` | 1976-1984 | 286 | 8.1 MB | same, with 21 `.eb` instead of 9 |
| `_cd8_/doradodocs` / `indigo/doradodocs` | 1979-1986 | 71 / 70 | 7.5 / 6.7 MB | 36 press each |
| `cyan/ais` | 1978-1985 | 117 | 59.2 MB | 114 `.ais` rasters — the AISViewer corpus |
| `indigo/interpresssamples` | 1984-1988 | 111 | 59.7 MB | 91 `.ip`, 19 `.interpress` |
| `indigo/interpressdoc` | 1986-1987 | 350 | 18.6 MB | 310 gargoyle, 31 ip |
| `indigo/imaging` / `cyan/imaging` | 1979-1989 | 1,753 / 1,282 | 64.9 / 27.4 MB | Imager, AIS tools, Gargoyle |
| `_cd6_/tioga` | 1979-1985 | 457 | 0.6 MB | **the Tioga screen fonts** (259 tfm, 122 ks, 69 strike) + `top/TiogaFonts.df!3` |

Diagnostics: the six the project runs come out of `_cd8_/dorado` as expanded
`.dm` archives — `eventCounters.dm!5`, `MEMA.DM!18`, `memMisc.dm!11`,
`Ifu.dm!51`, `kernel.dm!38` (15-Oct-1986, the newest file in that tree),
`Tricond.dm!5`, plus `ftest.dm!1`, `diex.dm!2`, `edp.dm!3`, `bootEmu.dm!3`
and `doradobaserom.dm!12`. `_cd8_/dorado/test/TestDorado.eb!5` (11-May-1984)
is the packaged diagnostic world.

**Font caution.** `cyan/cedarfonts` (1,297 files, 21.4 MB) is *TeX* — 663
`.tfm`, 85 `.gf`, Computer Modern at five resolutions. Cedar's Tioga screen
fonts are in `_cd6_/tioga` and `indigo/fonts/top/TiogaFonts.df!5`. Given how
much time `docs/parc-veteran-notes.md` and the Chess40 investigation spent
on font resolution, that distinction is worth having written down.

---

## 7. What to ask CHM for

Conservative, in priority order. Each item was checked in more than one
place, and the places are named. Items already covered by
`docs/cedar-archive-gaps.md` §7 are cited, not repeated.

**1. The Cedar 7.0 release tree — `[Cedar]<Cedar7.0>Top>` and its payload
directories.** Unchanged in priority from the existing ask, but now with a
much stronger case: §2.4 shows 7.0 had a `HeadsDorado`, that Dorado
microcode was built against 7.0 as late as 1990, and that a Dorado Cedar
world dated 10-Dec-1987 survives in `pixel/ncp`. The specific request should
name `[Cedar7.0]<Top>` DFs that other online DFs already prove existed:
`HeadsDorado.df`, `HeadsDLion.df`, `HeadsCommon.df`, `BootPackages.df`,
`BasicPackages.df`, `MesaRuntime.df`, `FS.df`, `IO.df`, `Imager.df`,
`Rope.df`, `SafeStorage.df`, `Tioga.df`, `VM.df`, `File.df`,
`RPCRuntime.df`, `CommDriver.df`, `Communication.df`, `BasicTime.df`,
`ThisMachine.df`, `MBQueue.df`, `BasicFinger.df` — plus a `Basic.Loadees`,
a `Dorado.germ` and a `BasicCedarDorado.boot`.
*Checked:* `cyan/cedar7.0` (recursively, live), `cyan/cedar7.0/cross/{top,
makeboot,documentation}`, a whole-index scan of all 202,273 parcarchive
rows for any path containing `cedar7`, `indigo/`, `pixel/`, `_cd4_`,
`_cdmiwok_01_`, `_cdcsl_93-16_`.

**2. `<CedarChest6.1>BiScrollers>`.** Confirmed still missing; see
`docs/cedar-archive-gaps.md` §6 for the file list and why it blocks Sil.
*Checked:* `cyan/cedarchest6.1` (no `biscrollers` payload dir), and the four
places the directory *does* exist — `cyan/cedarchest6.0/biscrollers` (29
files), `indigo/cedarhacks5.2/biscrollers` (37),
`_cdmiwok_01_/cedarcommon2.0/biscrollers` (26),
`_cdcsl_93-16_/1/cedar/release/biscrollers` (50) — none of which is the
Nov-1986 6.1 build Sil 6.1 needs.

**3. A Dorado Smalltalk disk, and Smalltalk-78.** Two related asks.
(a) Every Smalltalk world in the archive is an *Alto* world; the Dorado runs
them through DSemu, so this may be the complete truth, but an installed
Dorado Smalltalk pack (or the `.midas`/`.cm` recipe that built one beyond
the 251-byte `Smalltalk.midas`) would settle it.
(b) **Smalltalk-78 appears nowhere.** *Checked:* `Filene/Smalltalk-76` and
`Indigo/BasicDisks` on xeroxalto; `_cd8_/dorado`, `indigo/dorado`,
`_cd8_/doradosource`, `indigo/doradosource`, `indigo/dlion` on
parcarchive; whole-index filename scans of both servers for
`smalltalk|st76|st80|byterp|dsemu`. The only hits outside those trees are
`indigo/dlion/bj/ST80MesaDaybreak8K.db!1` (1988, Daybreak) and three
`SmallTalk10` Alto fonts.

**4. Cedar 6.0's own file-level audit.** Not an ask for material — an ask
for attention if CHM is looking anyway. `cyan/cedar6.0` has 105 `top` DFs
against 98 payload directories and `cyan/cedarchest6.0` 262 against 265;
neither has been audited against its manifests the way 6.1 was.

**Explicitly NOT worth asking for**, having checked:

- `<Cedar6.1>Dorado>` — present as `cyan/cedar6.1/microcode/dorado` (§1.2).
- `Germ.df!16`'s two `.mesa` files — present in `cyan/cedar6.0/germ` (§1.3).
- Dorado microcode for any Cedar release — every release from 5.0 has it,
  and `_cd8_/doradomicrocode` carries it forward to 1990 independently.
- A Dorado germ for 5.0 / 5.3 / 6.0 / 6.1 — all four exist, plus
  `Dorado10.germ!5` and two `.eg` ether-loadable germs.
- Smalltalk-76 or the ST-80 December-1980 image — both complete on
  xeroxalto, including sources and change sets.
- Alto games and boot files — `Io/Murray` has 50, plus a whole installed
  games pack in `indigo/cslcopydisk`.
- Dorado schematics — `io/doradologic` has 2,124 `.sil` across every board
  revision, 94 MB.
- Medley (for the Interlisp survey) — present at `phylum/medley` (§1.4).

---

## 8. Reproducing this survey

Two data sources, in this order.

**Primary: the local filename index.** `chm/cross-reference.html` is a
568,316-line cross-reference of the *whole* `xeroxparcarchive` collection.
Parsing each `<li><a href="PATH/.index.html">…</a>NAME!VER SIZE CRC DATE
TIME TZ TYPE AUTHOR` row yields **202,273 unique `(path, name!version)`
rows**, which is enough for every count in this document and costs zero
HTTP requests. Spot-checked against live `.index.html` listings for
`cyan/cedar6.1/top` (106 live / 106 unique), `_cd8_/dorado` (56 / 56) and
`io/murray` (106 / 106); the raw row count runs a few percent high because
a file listed under two names appears twice, so **de-duplicate on
`(path, lower(name))`**.

**Secondary: the live servers,** for anything the index cannot answer —
directory structure below an empty node, DF contents, and file headers.
About 40 requests total for this survey: `.index.html` for ~20 directories,
7 DFs fetched whole, and 8 `Range: bytes=0-31` probes for boot-file version
words.

Five practical notes, all of which cost time here:

- **`chm/cross-reference.html` indexes `xeroxparcarchive` only.**
  `xeroxalto` has its own, at
  `https://xeroxalto.computerhistory.org/cross_reference.html` (3.2 MB,
  14,680 rows, same row format). Its volumes are `_cd6_ _cd8_ Filene Ibis
  Indigo Io Ivy Pixel` — note **`Filene`**, which does not exist on the
  other server, and note that `Io/Murray` differs between the two in both
  directions. Anything Smalltalk requires this second index.
- **Paths in `cross-reference.html` are lower-cased**; live archive paths
  are not. Under `cyan/` the real directories are lower case anyway, but
  `.dm` archive directory names are mixed (`Io/DoradoLogic/ProcH-apcRev-Da.dm!1_/`).
  Take case from the parent `.index.html`.
- **Version numbers are per-directory.** `CedarDorado.eb!6` in
  `_cd8_/dorado` and `CedarDorado.eb!1` in `cyan/cedar6.1/top` are the same
  31,888 bytes dated 17-May-1984. Never treat a higher `!n` as a newer file
  across directories.
- **DF section headers can have three components.** `Directory
  [Cedar]<Cedar6.1>Microcode>Dorado>` must resolve to
  `cedar6.1/microcode/dorado`. Joining first-and-last, or matching
  `<([^>]+)>`, silently audits the wrong directory and reports 100% missing.
  This is the bug behind §1.2.
- **Boot-file version words are cheap to read.** `curl -H "Range:
  bytes=0-31"` returns 32 bytes; the first byte pair is the Pilot boot-file
  version, `0066` = `063000` (5.3 and later, what our germ accepts) and
  `0065` = `062400` (1983 and earlier, what makes `OthelloDorado.boot!8`
  version-dead). This is the fastest way to triage an unknown `.boot`
  without downloading a megabyte.
