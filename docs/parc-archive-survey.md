# Xerox PARC archive — Dorado survey

What's on `xeroxparcarchive.computerhistory.org` (and the older
`xeroxalto.computerhistory.org` mirror) that's relevant to the Dorado,
which items are usable as disk images, what file formats they're in,
and which tools can read or write them.

This doc complements `chm-archive.md` (which catalogs what we've
already pulled into `chm/`) by surveying the whole archive end-to-end
with disk-image questions specifically in mind.

---

## 1. The two CHM mirrors

| Server | What it is | When to use |
|---|---|---|
| `https://xeroxalto.computerhistory.org/` | Alto-era PARC IFS dump. Includes the `Io/Murray/` microcode worlds, the MicroD source, the SIL toolchain. | Pulling individual Alto/Dorado microcode `.eb`/`.pb` files; finding MicroD sources. |
| `https://xeroxparcarchive.computerhistory.org/` | The fuller, later PARC IFS dump. Includes the canonical `_cd8_/dorado/` source tree, `_cd8_/doradomicrocode/`, `_cd8_/doradosource/`, `_cd8_/basicdisks/`, `io/doradologic/`, `indigo/dorado/`, `indigo/doradosource/`, `indigo/basicdisks/`. | Pulling Dorado source archives, board schematic sources, and BFS disk packs. |
| Source-code landing page | `https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html` | Starting point for browsing collections. |

Both mirrors use the same URL convention (see `docs/chm-urls.md`). File
pages are `<dirname>/.<filename>.html`; binary downloads are
`<dirname>/<filename>` (no leading dot in the path component).

---

## 2. Dorado-specific directories worth knowing

These are the directories on `xeroxparcarchive.computerhistory.org`
that hold material specific to the Dorado (or directly used by it).

| Path | Contents | Pulled? |
|---|---|---|
| `_cd8_/dorado/` ★ | The canonical Dorado tree — microcode `.mb`, Initial-stage `.eb`, `.dm` source archives, `LoadMB.run`/`ReadMB.run`. | Yes — `chm/dorado/`. |
| `_cd8_/dorado/test/` | `Cedar.mb!4`, `TestDorado.eb!5`, `TestW.mb!1`. | Yes. |
| `_cd8_/doradomicrocode/` | **Newer microcode tree, not yet pulled.** 15 subdirs incl. `7.0/`, `BlockOps/`, `CedarDual3/`, `Code/`, `Cpa/`, `Documentation/`, `DoradoMicrocodeSources/`, `EtherTester/`, `Initial/`, `KernelSources/`, `LoadAltoDumpFile/`, `LoadMB/`, `Micro/`, `MicroD/`, `Top/`. | **No — high-value gap.** |
| `_cd8_/doradosource/` | 40 source-archive `.dm` files: `AEmuDibs.dm!32`, `AemuSources.dm!82`, `BootstrapSources.dm!12`, `D1ALTOMCSOURCE.DM!1`, `DMesaSources.dm!86` (547 KB), `DoradoMidasManual.dm!10`, `DoradoMicroAssembler.Dm!3`, `DoradoProms.dm!14`, `DSemuSources.dm!17`, `IfuSources.dm!53`, `kernelSources.dm!40`, `MEMASOURCE.DM!15`, `MicroMemo.Dm!2`, `MidasDoradoSources.dm!27` (496 KB), `MidasInternal.Dm!4`, `MidasSources.dm!24` (246 KB), `Trinity/`, plus Test/Temp/diagnosticListings subdirs and `D1MidasDevelopmentDisk.dm!1` (1.3 MB). | **No — substantial gap.** |
| `_cd8_/doradodocs/` | Press files for the Dorado manuals (we have PDFs). | Documents only — already covered by local PDFs. |
| `_cd8_/basicdisks/` ★ | 5 BFS disk packs (Mesa, NonProg, IFS, GSL, BcplProg). | **No — see §4.** |
| `_cd8_/altodocs/` | `ALTOTRIDENT.PRESS!2` (Trident on Alto memo). | No — single doc; pull on demand. |
| `indigo/dorado/` | Older versions of every file in `_cd8_/dorado/`. | Index only. |
| `indigo/doradosource/` | Older versions of `_cd8_/doradosource/`. | No. |
| `indigo/basicdisks/` ★ | 9 BFS disk packs — superset of `_cd8_/basicdisks/`, adds `BravoX.BFS!2`, `Mesa5.bfs!1`, `MesaProg.BFS!12`, `ProofReader.BFS!12`. | **No — see §4.** |
| `io/doradologic/` | Per-board SIL schematic-source `.dm` archives (BaseBd, ContA/B, DispM/Y, DskEth, IFU, MemC/D/X, ProcH/L), `DoradoBaseDebug.dm!1`, manifests. | Selectively yes — `chm/io-doradologic/`. |
| `io/doradologic/aux/`, `…/debug/`, `…/displayboard/` | Sub-archives. | Indexes only. |
| `io/doradobuilddocs/` | Build documentation. | No. |
| `io/doradodrawings/` | Schematic drawings. | We have local PDFs. |

The two **uncaptured high-value directories** for our work are
`_cd8_/doradomicrocode/` (newer microcode + tools) and
`_cd8_/doradosource/` (the actual MC sources, not just .MB binaries).
Worth a future pull pass.

---

## 3. Disk-image formats found on the archive

The PARC archives use **four interchangeable extensions** for raw
Alto disk-pack images, plus several non-disk formats that are easy
to mistake for disk images.

### 3.1 Alto disk-pack image (Diablo 30, 2.5 MB)

| Extension | Notes |
|---|---|
| `.bfs` | "Backup File System" — most common. Typical 1.4–4.9 MB depending on whether it's a 1- or 2-pack image. |
| `.altodisk` | Same content as `.bfs`, different name. |
| `.copydisk` | Same content as `.bfs`, different name. CopyDisk was the Alto utility that produced them. |
| `.disk` | Same content. Used in some sub-collections (e.g. `cyan/d0/d0cardtester.disk!1_/`). |

The CHM source-code landing page describes them explicitly as
*"an image of an entire Alto disk pack, analogous to an `.iso`
CD-ROM image"*.

**How CHM presents them.** Each disk-pack image appears in two
forms in parallel:
1. The raw binary blob, downloadable at `<dir>/<file>.bfs!N`.
2. An auto-generated browsable directory, indexed at
   `<dir>/<file>.bfs!N_/` (note the trailing `_`), where every file
   *inside* the pack is exposed as an individual HTML page.

For example, `Mesa6-14.bfs!4` appears at:
- raw image: `_cd8_/basicdisks/Mesa6-14.bfs!4`
- expanded:  `_cd8_/basicdisks/Mesa6-14.bfs!4_/.index.html`

This confirms what the user noted: **the archive holds both the
disk image AND the unpacked file tree**. That unpacking was done
once by `restore_alto_files` (see §5) when the website was
generated.

The same convention applies to `.altodisk!N_/`, `.copydisk!N_/`,
and `.disk!N_/` directory paths in the cross-reference index.

### 3.2 Alto Dump-format archive `.dm` (NOT a disk image)

A `.dm` ("Dump") file is a multi-file archive — closer to a `.tar`
than a disk image. It contains a list of named files with their
attributes, packed sequentially. The Alto's `Dump` and `Load`
utilities created/extracted these. The archive presents them with
the same `_/` expansion convention, but the contents are an
arbitrary file collection, not a filesystem snapshot.

Examples in our tree: `chm/dorado/expanded/bootstrap.dm!20_/`,
`chm/dorado/expanded/UnBug.bfs!1_/` (yes — a BFS expanded the same
way as a DM, since both formats expose a file tree).

### 3.3 Microcode worlds `.eb` / `.pb` (NOT disk images)

- `.eb` — Ether-Bootable. Microcode + small bootstrap, sized to
  fit an Ethernet boot transfer. Already covered in
  `dorado-software.md`.
- `.pb` — Pilot Backup. Memory image of a complete Mesa/Pilot
  world (microcode + OS image + state). No disk involved.

### 3.4 Trident disk packs (T-80, T-300) — NOT in the PARC archives

Searched both mirrors and `chm/cross-reference.html` (45 MB index):
no Trident pack images are hosted on `xeroxalto` or
`xeroxparcarchive`. The `eris/lispcore/dlioninitial/trident/`
directories exist, but they hold *boot-config* files for Lisp on a
DLion (Dandelion) running with a Trident drive — not the disk
images themselves. The names `klamath/` and `sierra/` in those
paths are pack labels, not images.

The only Trident pack image we know of is the Spruce print-server
T-300 image we already have locally:
`AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300`.

This was a real surprise — Mesa-on-Dorado and Cedar-on-Dorado
disk packs apparently never made it onto a public mirror. (See
`dorado-software.md` for our boot-recipe workarounds.)

---

## 4. BFS disk packs we can pull from `xeroxparcarchive`

The `_cd8_/basicdisks/` and `indigo/basicdisks/` directories hold
**9 unique Alto disk packs** that runnable Alto microcode (incl.
`AltoMesaDorado.eb` running on our Dorado) could mount.

| File | Size | Date | Likely contents |
|---|---|---|---|
| `BcplProg.BFS!13` | 1.36 MB | 1985-10-29 | BCPL programming environment |
| `BravoX.BFS!2` | 1.90 MB | 1982-08-25 | BravoX (the late-Bravo editor) |
| `GSL.BFS!1` | 1.85 MB | 1985-05-01 | GSL — graphics or service language? |
| `IFS.BFS!3` | 1.81 MB | 1984-12-03 | IFS (file-server) software image |
| `Mesa5.bfs!1` | 3.78 MB | 1980-11-29 | Mesa 5 development |
| `Mesa6-14.bfs!4` | 4.88 MB | 1983-05-21 | Mesa 6.14 development (★ largest, newest Mesa) |
| `MesaProg.BFS!12` | 2.18 MB | 1985-10-30 | Mesa programming environment |
| `NonProg.BFS!16` | 1.82 MB | 1986-02-25 | Non-programmer Alto environment |
| `ProofReader.BFS!12` | 1.87 MB | 1983-11-07 | Proofreader (text/spelling tool) |

Total: ~22 MB across 9 images.

`Mesa6-14.bfs!4` is the headline image — it's a Mesa development
environment on an Alto disk, the closest public artifact to "Mesa
running on real hardware."

Reachable URLs follow the standard pattern, e.g.:
- `https://xeroxparcarchive.computerhistory.org/_cd8_/basicdisks/Mesa6-14.bfs!4`
- `https://xeroxparcarchive.computerhistory.org/_cd8_/basicdisks/.mesa6-14.bfs!4.html`
- `https://xeroxparcarchive.computerhistory.org/_cd8_/basicdisks/Mesa6-14.bfs!4_/.index.html`

There are also four `.disk!N_/` directories on the archive (all
D0/Dolphin diagnostic packs, not Dorado): `D0cardtester.disk!1`,
`EDmicroDiagnosticsRev1x1.disk!1`, `IOcardtests.disk!1`,
`testerdrawings.disk!1` — useful as cross-validation if we ever
try to bring up a D0 emulator, otherwise out of scope.

### Disk packs we already have locally

- `chm/dorado/UnBug.bfs!1` (2.3 MB) — **this is itself a BFS disk
  pack**, the Dorado developers' Alto debug environment. Currently
  documented as "117 files" in `chm-archive.md` because we've only
  ever consumed the unpacked form (`expanded/UnBug.bfs!1_/`). Worth
  re-flagging that the original is a mountable Alto pack.
- `AltoInfo/ContrAlto2-beta/Disks/*.dsk` — Diablo 30 packs in the
  format ContrAlto/salto consume. Same byte layout as `.bfs`,
  different filename.
- `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` — the only
  Trident T-300 pack we have or know to exist publicly.

---

## 5. Tools for reading/writing these disk images

### `restore_alto_files` — Paul McJones (canonical, but Mac-only)

The CHM site itself was generated by this tool. It reads:

- BFS / `.altodisk` / `.copydisk` / `.disk` (Alto disk packs).
- `.dm` (Dump archives).
- Archivist tape images (the originals from the PARC tape vault).
- Bravo, Tioga, Press, and other Alto file formats — converts to
  HTML/PDF for the website.

Source: `https://xeroxparcarchive.computerhistory.org/src/restore_alto_files.tar.gz`
(also mirrored at `xeroxalto.computerhistory.org/src/`). 201 KB
gzipped, MIT-licensed C, written for macOS but portable.

This is the **read-side ground truth**. If our emulator's BFS
parser disagrees with `restore_alto_files`, we are wrong, not it.

### `palo` — Hugo Snaves (https://github.com/hsnaves/palo)

Alto microcode assembler + simulator + archiver tool, modern, in
C with SDL2. The "archiver" component handles BFS pack
manipulation — read individual files, list contents, possibly
write. Inspired by ContrAlto. Does not currently support Dorado.

Companion: `https://github.com/hsnaves/alto_files` — curated Alto
BFS images organized for palo's tooling.

### `ContrAlto` / `ContrAlto2` — Living Computer Museum

C# source already in `AltoInfo/Contralto2-2.0-Beta/`. Two relevant
pieces:

- `IFS/AltoFS.cs` (and adjacent files) — full AltoFS implementation
  in C#, reads and writes BFS packs at the file level.
- `Disk/Trident*.cs` — working Trident T-80 / T-300 controller +
  drive emulation. Reads the `.dsk300` format we have.

Already noted in `altoinfo-survey.md` as the porting target for our
Trident driver and as the validation oracle for AltoFS reads.

### `rosetta.tar.gz` — magnetic-tape readers

Source: `https://xeroxparcarchive.computerhistory.org/src/rosetta.tar.gz`.
C sources for reading the MAXC and Archivist 9-track 1/2"
magnetic tapes that the archive was originally extracted from.
Not relevant to file-level work (we'd never re-process tapes), but
worth knowing exists.

### Native Alto-side tools (in our `chm/` already)

These are the period tools — they ran on the Alto and produced the
formats we now read.

| Tool | What it does | Local path |
|---|---|---|
| `LoadMB.run!6` | Loads a `.mb` microcode binary into Dorado IM via the BaseBoard. | `chm/dorado/LoadMB.run!6` |
| `ReadMB.run!1` | Dumps an existing `.mb` symbolically. | `chm/dorado/ReadMB.run!1` |
| `LoadAltoDumpFile.bcd!1` | Loads an Alto Dump (`.dm`) file into a running system. | Not yet pulled — `_cd8_/doradomicrocode/LoadAltoDumpFile/`. |
| `MakeCardImages.run!1` | Creates board-card images (PROM programmer input). | `chm/dorado/MakeCardImages.run!1` |
| `BSYS` (MAXC) / `Archivist` (Dorado) | Period archive-tape utilities. | Source in `rosetta.tar.gz`. |
| `DoradoProms.run!3` | PROM programmer for Dorado boards. | `chm/io-doradologic/DoradoProms.run!3` |

These are Alto `.run` binaries — useful as reference behaviour, but
to actually invoke them you'd need a working Alto emulator (we have
ContrAlto for that) or our own Dorado running an Alto microcode
world.

### What none of these tools do

No tool we've found writes a fresh BFS pack from scratch with
arbitrary contents. The expectation in the period was that you
booted into Alto, ran `OS/Executive`, copied files in, then used
`CopyDisk` to dump the pack. So if we want to inject a custom file
into a BFS image, we either:
1. Add it via a running Alto-emulator session (cleanest), or
2. Build a minimal BFS writer in our own code (palo + ContrAlto2's
   AltoFS.cs are the references for what to implement).

---

## 6. Implications for our emulator work

1. **The "no disk packs exist" claim in `dorado-software.md` §"What
   CHM does NOT have as raw disk packs" is wrong** for Alto packs
   — there are 9 BFS images in `BasicDisks/`, plus our local
   `UnBug.bfs!1`. It is correct only for *Trident* (T-80/T-300)
   packs. Worth fixing.
2. **`Mesa6-14.bfs!4` is interesting.** Mesa-on-Alto running under
   our Alto-emulator microcode is a fallback path if Mesa-on-Dorado
   bring-up stalls.
3. **`UnBug.bfs!1` is a debug environment.** It's the most
   Dorado-specific BFS we have — Dorado developers' working disk,
   complete Bravo/Executive/Swat/Sys.Boot. Once we can mount BFS
   packs, this is the natural first thing to mount.
4. **Disk-pack writing is unsolved.** Reading BFS is well-supported
   (palo, ContrAlto, restore_alto_files). Writing arbitrary
   packs is not. Either run-and-snapshot (using our emulator once
   it boots Alto) or hand-roll a writer.
5. **Two new pull targets.** `_cd8_/doradomicrocode/` (15 subdirs
   incl. `Initial/`, `MicroD/`, `LoadMB/`, `LoadAltoDumpFile/`)
   and `_cd8_/doradosource/` (40 source `.dm` archives) are both
   worth pulling — newer microcode revisions and the actual MC
   sources we'd need if we ever want to re-build the microcode.

---

## 7. Suggested next steps

1. Pull `_cd8_/basicdisks/Mesa6-14.bfs!4` and `_cd8_/doradomicrocode/`
   into `chm/`. Add to `chm-archive.md` §7.
2. Update `dorado-software.md` to reflect the BFS/altodisk/copydisk
   findings — remove or correct the "no raw disk packs exist"
   claim.
3. Implement BFS read in our emulator (file enumeration first;
   actual filesystem mount second). ContrAlto2 `AltoFS.cs` is the
   reference; palo's archiver is a second opinion.
4. Defer Trident-pack writing until we have an Alto-on-Dorado
   session that can produce one via `CopyDisk`.
