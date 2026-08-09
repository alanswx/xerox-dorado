# Koto Lisp colour-display archive inventory

Fetched from `https://xeroxparcarchive.computerhistory.org/` on 2026-08-08,
using the checked-in `chm/cross-reference.html` to resolve names and versions.
These are plain IFS files; `tools/fetch_chm_archive.py` is for expanded `.dm`
archives and was not applicable here.

## Exact Nick lead

The requested path is present and fetched:

`[phylum]<LISP>KOTO>Library>DORADOCOLOR!1`

The source is `17,132` bytes, CRC32 `9c2e369c`. `\DORADO\STARTCOLOR` is a
function in this file, not a separate archive member. It sets up the Dorado
monitor, channel, and colour control blocks, including the 4- and 8-bit scan
constants.

## Koto boot target

The version-matched full Koto set is under
`eris/lisp/koto/basics/`:

- `Full.sysout!16` — `4,205,568` bytes, CRC32 `fc20f0a9`
- `DORADOLISPMC.EB!1` — `36,696` bytes, CRC32 `6210ad82`
- `AltoD1MC.eb!1`, `Lisp.run!1`, and `Lisp.syms!1`
- `INIT.NOGREET!1` and `LISP11SAX000INITIAL.DB!1`

This is the version-matched Koto bring-up candidate. The Koto
`LISP.SYSOUT!15` was deliberately not fetched or selected: it is the smaller
image. The known-bad `chm/lisp/lyric-basics/Full.sysout!6` is also explicitly
rejected. A concurrent broader inventory in `chm/lisp/koto/` contains the
repository-preferred `basics/Released-Full.sysout!2`; it is a full-library
candidate and was left untouched. Before bring-up, compare its release
metadata against the Koto microcode; do not silently mix it with the
version-matched `Full.sysout!16`. For the existing Lyric path, the standard
full-library target remains `chm/lisp/lyric-basics/Released-Full.sysout!2`
with its Lyric boot set.

## Colour material fetched

The archive mirror is preserved below this directory, including source,
compiled, and documentation forms where available:

- Dorado path: `DORADOCOLOR`, `LLCOLOR`, `COLOR`, `COLORDEMO`, `BITMAPFNS`,
  `HLCOLOR`, and `COLORPOLYGONS`.
- Related card paths: `BUSMASTER`, `BUSCOLOR`, and `COLORNNGS`.
- Additional colour material: `SKETCHCOLOR` and `PRINTCOLORSAMPLE`.
- Koto release notes: `doc/releasenotes/` (`ADDENDA`, `BUGFIXES`, `FEATURES`,
  `FIG1`, `FIG2`, `LIBRARYPACKAGES`, `OVERVIEW`, and `TOC`).

The source dependency chain is visible without running Lisp:
`COLOR` loads `LLCOLOR` and `READNUMBER`; `COLORDEMO` loads `COLOR`; the
BUSMASTER/NNGS variants load `BUSMASTER` and `COLOR`. `LLCOLOR` declares
`ExtraColorDisplayPages 2`, defaults to 4 bits per pixel, and contains the
generic 4/8-bit operations. No 24-bit Lisp implementation was found in the
Koto colour sources.

The Dorado microcode sources Nick’s path needs were already present in the
repository and were not duplicated here:
`chm/doradomicrocode/doradomicrocodesources/ColorDisplay.mc!1`,
`RastDefs.mc!5`, `RastMain.mc!6`, `DMesaRastMiscDisp.mc!1`, and
`DMesaRastMiscOps.mc!2`.

## Validation and gaps

The fetch contains 55 non-empty archive files totalling 5,545,490 bytes (plus
this README). Every fetched archive file matches the CHM index’s recorded
size and CRC32; SHA-256 values can be regenerated with `shasum -a 256`. No
emulator source was changed and no Koto bring-up was run.

The broader `chm/lisp/koto/` inventory overlaps 25 of these files byte-for-byte
and was not overwritten or deleted. This directory retains the exact
`eris`/`phylum` archive provenance and the supplemental files that inventory
does not contain.

No separate `STARTCOLOR` file exists. `COLORPOLYGONS` refers to the older
`POLYGONS` library, which is not present in the Koto tree (only an older
Carol/base-library copy is indexed); it was not mixed into this inventory.
The Koto full image and boot recipe are therefore identified, but still need
an actual guest validation later.
