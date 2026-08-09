# Koto Lisp colour-support archive inventory

Bounded CHM inventory and fetch; no emulator source was changed and no
bring-up was run.

## Canonical full-image target

The required image is:

`[phylum]<LISP>KOTO>BASICS>Released-Full.sysout!2`

CHM cross-reference identity: **4,348,928 bytes**, CRC-32
**dce38209**, dated **25-Jan-1988 17:00:07 PST**. The fetched file matches
all four fields and is at:

`chm/lisp/koto/basics/Released-Full.sysout!2`

The following Koto images were deliberately not fetched or substituted:

- `LISP.SYSOUT!3` — 3,251,200 bytes, CRC `3b59ba69`.
- `Full.sysout!11` — 5,312,512 bytes, CRC `c49d1631`.
- Lyric `Released-Full.sysout!2` and all repository `FULL.SYSOUT`/`LISP.SYSOUT`
  artifacts.

## Fetched Koto set

- **Boot/runtime:** `DORADOLISPMC.EB!1`, `INIT.NOGREET!1`, and every Koto
  `Lisp.run!1`–`!5` with its matching `Lisp.syms!1`–`!5`. `!3` is the
  closest pre-image runtime revision to the 25-Jan-1988 full image; CHM
  does not state an explicit image/runtime pairing, so all revisions are
  retained rather than guessing.
- **Colour libraries:** `DORADOCOLOR!1`, `.DCOM!1`, `.TEDIT!1`; `COLOR!1`
  and `.DCOM!1`; `COLORDEMO!1` and `.DCOM!1`; `COLORPOLYGONS!1` and
  `.DCOM!1`; `HLCOLOR`, `LLCOLOR`, `BITMAPFNS`, `READNUMBER`, and the
  Koto `Library>DOC>COLOR.TEDIT!1`.
- **Dorado/display support:** `doradokeyboards!1`; `ADISPLAY`,
  `DOVEDISPLAY`, `DOVEDISPLAYHACK`, `DSKDISPLAY`, `HLDISPLAY`, `LLDISPLAY`,
  and `BOOTSTRAP`, each with its CHM source and compiled form where present.
- **Microcode source:** packed `LISPDMC.DM!1` plus all 43 expanded members,
  including `LispDisplayMain.MC`, `displaydefs.mc`, `LISPDEFS.mc`,
  `MicroDLISP.CM`, and the assembly/release command files.
- **Additional Koto colour user code:** `COLORNNGS!1`, `.DCOM!1`, and
  `.TEDIT!1`.

## Validation

All **95 fetched payload files** match the local CHM cross-reference’s
archive path, version, byte size, and zlib/IEEE CRC-32. The three saved
directory indexes and the expanded-archive index are also retained for
provenance. Key SHA-256 values:

| artifact | SHA-256 |
|---|---|
| `basics/Released-Full.sysout!2` | `db3b89d85c6a6806eba7a0dddfaa106d8c4e0b428c95896fbe7342b915f273a3` |
| `basics/DORADOLISPMC.EB!1` | `8fb2ee5e216c7f419adc1558c32879188ce70a48beb2ebcee50898005f3259eb` |
| `library/DORADOCOLOR!1` | `3bb568fbe5a0296199d11b61b68e3e90af030ca4cb96e39828bc96b07cc25ecc` |
| `library/DORADOCOLOR.DCOM!1` | `662d02ab8d4345bb23af1a150a799845eaeeb3754ecd2129434f2c26ec27c119` |
| `sources/LISPDMC.DM!1` | `6b6877b08fdd03257b364bc4eb1d2dc0d8a2636832f742c3ecb087f9522fc2b0` |

## Findings and remaining ambiguity

`DORADOCOLOR!1` contains `\\DORADO\\STARTCOLOR`, the monitor/channel/colour
control-block setup, colour-map transfer, and 4- and 8-bit paths. It allocates
an 8-page colour-map image. `COLORDEMO` selects 8 bits per pixel on a Dorado;
the fetched driver contains no 24-bit path, which is consistent with Nick’s
note that Lisp used only 4/8-bit modes.

The specific “allocate two extra pages because of a Dorado colour-microcode
bug” comment was not located in this bounded Koto subset. It remains an
open search item, as does a CHM artifact explicitly pairing
`Released-Full.sysout!2` with one particular `Lisp.run` revision.
