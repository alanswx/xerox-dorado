# Xerox floppy and disk media: catalog and extraction

2026-08-11. Every disk image we hold, what is on it, and how to read it
without an emulator. Companion tool: `tools/pilot_floppy.py`.

## 1. The headline

**13,921 files / 321 MB decoded out of 566 disks**, across two container
formats and one filesystem, with no emulator in the loop.

**And the Medley 1.0 library is in that haul.** Earlier in this project I
said "CHM has no Medley library, and we have no Medley `.LCOM`". That was
true of the CHM IFS archive and false of our own tree:
`chm/bitsavers/bits/Xerox/1108/Medley_1.0/` has held the Medley 1.0 1108
distribution floppies since 28-Jul-2026. Decoded, they give **191 `.LCOM`
+ 54 `.DFASL`** across Medley Library #1-3 (132 files) and Medley
LispUsers #1-6 (307 files). `docs/lisp-release-gap-report.md` and the
session notes that say otherwise are wrong.

The other find: **Medley 1.1**, 14 disks / 210 files
(`1108_medley_1.1`) — the release whose `library.tar.Z` in
`Interlisp/history` does not decompress. We have it as floppies.

## 2. How to read them

```
python3 tools/pilot_floppy.py list    IMAGE[.IMD|.DMK] ...
python3 tools/pilot_floppy.py extract OUTDIR IMAGE[.IMD|.DMK] ...
```

Three layers, all documented in the tool's header:

- **ImageDisk `.IMD`** — Dunfield's container. 8-inch disks here are 77
  cyl x 2 heads; cyl 0 head 0 is FM 26x128 (the IBM 3740 label track,
  EBCDIC `HDR1`), cyl 0 head 1 MFM 26x256, the rest MFM 15x512.
- **David Keil `.DMK`** — raw track images. 16-byte header, then per
  track a 64-entry IDAM pointer table (bit 15 = double density) and the
  raw track. In a mixed-density image single-density bytes are stored
  **doubled**, so FM tracks are read with a stride of 2.
- **Xerox Pilot floppy volume** — record layouts from Xerox's own
  Interlisp FLOPPY package, `chm/lisp-archive/koto/lisp/sources/FLOPPY!1`
  (Kelly Roach, 1985): `PSECTOR9` (volume descriptor, seal 49932),
  `PFILELIST`/`PFLE`, `PMPAGE` (13003), `PLPAGE` (43690). Files are laid
  out `[PMPAGE][PLPAGE leader][data pages]...` contiguously, so walking
  the leader seals recovers everything without modeling Pilot's sector
  numbering.

Nothing is copied into the tree: the archives are in `chm/` and the tool
is in `tools/`, so any extraction regenerates in minutes.

## 3. Catalog

| disks | files | collection |
|---:|---:|---|
| 21 | 2,703 | `InterlispReleaseFloppies/LYRIC` (Lisp Sysout 1-5, Library 1-3, Display Fonts 1-7, Font Metrics 1-2, System Files, Installation) |
| 21 | 2,416 | `InterlispReleaseFloppies/KOTO` (Koto 1.0 + 2.0: Library 1-4, HelpSys, Display Fonts 1-6, Font Metrics) |
| 10 | 268 | `InterlispReleaseFloppies/HARMONY` (Library 1-2, **LispUsers 1-2**, Sysout, System Files) |
| 13 | 84 | `InterlispReleaseFloppies/LOOPS` (demo sysouts, source + DCOM, KB LOOPS) |
| 5 | 123 | `InterlispReleaseFloppies/SOURCE` (IMTS 2.3 + RAPIDS 1.3, Oct-1989) |
| 16 | 2,627 | `1108_intermezzoRelease_1985` |
| 11 | 233 | `1108_intermezzoSources_1985` |
| 10 | 10 | `1108_intermezzoSysouts_1985` |
| 13 | 399 | `1108_fugue_1983` (Lisp Library 1-2, Lisp Users 1-3, Sysout, Packages) |
| 8 | 57 | `1108_carol_1983` |
| 14 | 210 | **`1108_medley_1.1`** |
| 8 | 57 | `1108_medley_1.0` |
| 7 | 12 | `1186_Medley1.0` |
| 32 | 905 | `medley_copies` |
| 19 | 301 | `bitsavers Medley_1.0_1108_Copy1` (**Medley Library #1-3**) |
| 6 | 307 | `bitsavers Medley_1.0_1108_Lispusers` (**Medley LispUsers #1-6**) |
| 3 | 59 | `bitsavers Medley_1.0_1108_LOOPS` (Lyric LOOPS Library/System/Users) |
| 2 | 31 | `bitsavers Medley_1.0_1108_Rooms` |
| 295 | 1,382 | `1186_floppies` (Daybreak, 5.25in: 40 tracks) |
| 65 | 384 | `1186_Lyric_Sources` |
| 17 | 76 | `1186_Smalltalk-80_DV6_Dec87` |
| 13 | 604 | `8010_xde5.0` / `XDE_5.0_imd` |
| 6 | 59 | `LOOPS_110987` |
| 3 | 4 | `Lyric_Diagnostic` |

## 4. What does not decode, and why

80 disks yield no files. None of them is a decoder bug:

- **40 have a Pilot volume label but no Pilot files** — diagnostics and
  Othello media (`6085OfflineDiagnostics`, `8010OfflineDiagnostics`,
  `FixedDiskDiagnostic`, `AIS-EIDiagnostics`, `othello 12.0`,
  `OthelloV11`). These are boot floppies: the image is written raw, not
  as Pilot files. Compare the "SYSOUT" and "HUGEPILOT" floppy modes in
  IRM section 24.10 (`chm/lisp/ftp-root/IRM/CHAPFILES.TEDIT`), where a
  sysout spans several disks as one unnamed blob — which is also why
  every "Lisp Sysout #N" disk reports exactly 1 file.
- **6 Dolphin disks** (`st80src`, `st80image`, `lafite`,
  `dolphinFiles`) are **Alto-format**, not Pilot — the Dolphin used the
  Alto file system for `{DSK}`. Read those with the Alto tooling
  (`AltoInfo/palo`, `dsk2trident`).
- **33 of the 1186 5.25-inch disks** have no 8-inch Pilot volume
  descriptor, as expected for that media.

## 5. The hard disks

Darkstar's `.img` files and the bitsavers `8010_hd_images.zip` share one
format, and it is **not** a linear sector image: per `Darkstar/D/IO/SA1000.cs`,
byte 0 is a drive-type code and the rest is raw track data, 5,325 words
per track, 3 bytes per word. The arithmetic confirms it —
65,433,600 / 3 / 5,325 = **exactly 4,096 tracks**, and type `0x02` =
`Q2040` = 512 cylinders x 8 heads.

So extracting files means decoding the SA1000 track format *and* Pilot's
hard-disk volume structures (a scan finds only 2 `PLPAGE` seals, so the
floppy structures do not apply). That is the one place where booting the
image in Darkstar is genuinely the cheaper route — see
`docs/darkstar-extraction-plan.md`.

Available images, all 65,433,601 bytes:

| image | source | notes |
|---|---|---|
| `Harmony.img` | Darkstar + bitsavers (`Interlisp-D-Harmony.img`) | |
| `ViewPoint-*.img` | Darkstar + bitsavers | |
| `XDE.img` | Darkstar + bitsavers (`XDE-5.0.img`) | |
| `Koto.img` | **Darkstar only** | |
| `Lyric.img` | **Darkstar only** | |
| `Medley.img` | **Darkstar only** | the interesting one |
