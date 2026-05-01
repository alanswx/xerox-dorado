# CHM Dorado Hard Disk Pack Research

Date: 2026-05-01

Scope: CHM/PARC archive notes on what hard disk pack the Dorado boot path expects,
how the boot microcode is laid out on it, and what is still missing before we can
make or emulate one faithfully. This is research only; no emulator code was
changed.

## Local CHM Copies Pulled / Transcoded

Downloaded from CHM:

- `docs/research/chm-hard-disk-pack/downloads/ALTOTRIDENT.PRESS-2.pdf`
  from `https://xeroxparcarchive.computerhistory.org/_cd8_/altodocs/.ALTOTRIDENT.PRESS%212.pdf`
- `docs/research/chm-hard-disk-pack/downloads/DoradoManual-A.press-8.pdf`
  from `https://xeroxparcarchive.computerhistory.org/_cd8_/doradodocs/.DoradoManual-A.press%218.pdf`
- `docs/research/chm-hard-disk-pack/downloads/DoradoBooting.press-7.pdf`
  from `https://xeroxparcarchive.computerhistory.org/_cd8_/doradodocs/.DoradoBooting.press%217.pdf`
- `docs/research/chm-hard-disk-pack/downloads/DoradoBootingImpl.press-1.pdf`
  from `https://xeroxparcarchive.computerhistory.org/_cd8_/doradodocs/.DoradoBootingImpl.press%211.pdf`
- `docs/research/chm-hard-disk-pack/downloads/DiskFormat.txt-1.html`
  from `https://xeroxparcarchive.computerhistory.org/_cd8_/doradodocs/.DiskFormat.txt%211.html`
- `docs/research/chm-hard-disk-pack/downloads/chm-cross-reference.html`
  from `https://xeroxparcarchive.computerhistory.org/cross_reference.html`

Converted to searchable text:

- `docs/research/chm-hard-disk-pack/transcoded/ALTOTRIDENT.PRESS-2.txt`
- `docs/research/chm-hard-disk-pack/transcoded/DoradoManual-A.press-8.txt`
- `docs/research/chm-hard-disk-pack/transcoded/DoradoBootingImpl.press-1.txt`
- `docs/research/chm-hard-disk-pack/transcoded/Dorado_Booting_Operation_and_Mechanisms_Apr80.txt`
- `docs/research/chm-hard-disk-pack/transcoded/Dorado_Hardware_Manual_Sep1981.txt`
- `docs/research/chm-hard-disk-pack/transcoded/DskEth.txt`

`DoradoBooting.press-7.pdf` is a 1-page Press/PDF file whose `pdftotext`
output was blank; the existing local `Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf`
and the downloaded `DoradoBootingImpl.press-1.pdf` contain the usable text.

## What Kind Of Pack

The Dorado disk controller was built for Century Data Trident drives. The
hardware manual says it controls up to four Century Data Trident drives, either
T-80 or T-300. `ALTOTRIDENT.PRESS` says the T-80 and T-300 were connected in
the Alto Trident environment, and the Dorado docs say T-300 format requirements
match the T-80. The physical pack described for T-80 is IBM 3336-type media.

For the Dorado Initial disk boot path, the important point is not just the
physical drive family. The boot file is read by Dorado Pilot disk microcode, not
the 9-sector Alto Trident/IFS format used by the Spruce pack.

Pilot disk definitions say the Dorado Trident disk is formatted as:

- 815 cylinders
- 5 heads on the drive-0 layout
- 28 usable sectors
- 2-word header
- 10-word label
- 256-word data page (`sizeData = 0400B`)

The real drive-0 address is virtualized for Pilot compatibility:

- emulated cylinder range is `4075 = 5 * 815`
- emulated head is ignored
- real cylinder = emulated cylinder mod 815
- real head = emulated cylinder / 815
- real sector = emulated sector

The existing Spruce `dsk300` image is still useful for Trident mechanics, but it
is an Alto/IFS-style 9-sector format with 1024-word data records. It is not the
format Initial uses for the private Dorado hard-microcode boot file.

## Hard Microcode File Layout

`InitialDisk.mc` is the direct specification for the private hard-disk
microcode file:

- The file always starts at page 4, i.e. cylinder 0, head 0, sector 4.
- Its Pilot FileID processor is zero.
- Its serial is `064732B` in both serial words.
- Its type is zero.
- It is not temporary and not immutable.
- It is not in a Pilot logical volume.
- It looks like a standard Pilot boot file containing an `.eb` file, including
  the initial overhead page.

`DiskBootTransfer.mc` explains the run chaining:

- Microcode and germ files are standard Pilot boot files.
- At a run break, the `bootChainLink` in the last page label points to the next
  run.
- The file ends with `bootChainLink = [-1, -1]`.
- A nonzero `bootChainLink` after a label-check failure means "continue at this
  disk address"; `[-1, -1]` means EOF.

`PilotDiskDefs.mc` gives the label fields the microcode actually mutates:

- `Lab.filePageLo = 5`
- `Lab.filePageHi = 6`
- `Lab.fileFlags = 7`
- `Lab.bootChainLink = 10`
- label size is 10 words

During reads, the microcode checks only the first 8 label words and reads the
last 2 words, which is how the boot-chain link can be learned after a label
check mismatch.

## How To Build A Pack Image

Based on the CHM docs, a synthesized Initial-compatible hard pack should be:

1. A T-80/T-300 Trident virtual pack using the Dorado Pilot layout, not the
   Alto/IFS 9-sector layout.
2. Sector/page records containing a 2-word header, 10-word Pilot label, and
   256-word data page.
3. Page 4 contains page zero of the `.eb` file, including the `.eb` overhead
   page.
4. Subsequent pages contain the rest of the `.eb` item stream.
5. Labels identify the hard-microcode file with serial `064732B`, type zero,
   file page numbers incrementing from zero, and flags clear.
6. If the file is contiguous, each page can advance by normal disk-address
   increment and the final page should carry `bootChainLink = [-1, -1]`.
7. If non-contiguous, the last page of each run carries the next run's disk
   address in `bootChainLink`.

The most likely source payload is one of the local `.eb` files, for example
`chm/dorado/InitialDiskDorado.eb!3` or the corresponding current microcode
world. `DoradoBootingImpl.press` confirms `LoadMB /E` creates EB-format files
and that an EB file is an overhead page followed by LoadRam items.

## What Is Still Missing

- I did not find `FormatTridentImpl` in the local tree or CHM cross-reference by
  that exact name. `InitialDisk.mc` says the private convention is shared with
  `FormatTridentImpl` in Othello, so finding that source would remove ambiguity
  about exact label packing and formatter behavior.
- We still need a canonical Dorado Pilot pack image from CHM or elsewhere. The
  public files found so far include Alto Trident packs and source/microcode, but
  not a known-good Dorado Initial hard-microcode pack.
- We need to decide the preservation container for synthesized Dorado Pilot
  packs. The ContrAlto/Bitsavers T-300 image container stores Alto-style
  2+10+1024-word sectors; Dorado Pilot sectors are 2+10+256-word pages.
- `DiskFormat.txt!1` is valuable for hardware timing and confirms the T-300 has
  the same minimum format requirements as T-80, but it does not describe the
  private Initial boot file installation tool.

## Emulation Implication

For booting Initial from disk, emulating only the Alto Trident/Spruce 9-sector
pack is the wrong target. We need a Dorado Pilot-format Trident surface and a
synthetic or recovered private boot file at page 4. The hardware timing docs are
useful for controller behavior, but the immediate boot-content problem is the
Pilot page/label layout and `.eb` placement described above.
