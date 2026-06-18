# Pilot / Cedar Nucleus Filesystem — On-Disk Format (Normative)

- **Status:** Normative definition. Supersedes the "PROVISIONAL" Pilot label
  layout in `PARC_DISK_IMAGE_SPEC.md` §5.3.
- **Byte order:** big-endian within each 16-bit word; 32-bit (Mesa `LONG`/`INT`)
  quantities are stored **low-word-first** (low 16 bits at the lower word
  offset) — confirmed against the disk microcode (`PilotDiskDefs.mc`).
- **Container:** PARC Disk Image (PDI), `fsFamily = 2`, `dataBytes = 512`,
  `labelBytes = 20`. The PDI container already carries Pilot disks with no
  format change; this document defines how to interpret their labels and pages.

This is the authoritative on-disk definition for the Pilot operating system
filesystem underlying Cedar/Mesa (Dolphin / Dorado / Dandelion). It is grounded
in the original PARC Mesa sources (paths in §11). **Both file-ID generations are
defined** — the original-Pilot 80-bit `UniversalID` and the Cedar-nucleus 32-bit
`FileID` — selected by PDI `flags` bit 2.

## 0. Why this document is normative (no emulator yet)

There is no Pilot/Cedar emulator and no sample Pilot volume in the archive. Rather
than wait for an emulator to act as the validation oracle, **this format is the
oracle**: it is derived directly from the period Mesa sources and made
self-consistent (every structure cross-checks by field offset, and root pages
carry a recomputable checksum). A future emulator conforms to this definition.
The two things that genuinely live outside the Mesa sources — per-sector hardware
**ECC** and the **disk-formatting/placement** done by an offline installer — are
explicitly out of band here (§7, §9).

---

## 1. Sector model

Every Pilot sector is three independently addressed fields, exactly as for the
Alto families:

```
+---------------+-------------------+---------------------+
| HEADER (2 w)  |   LABEL (10 w)    |   DATA (256 w)      |
| disk address  | file id + chain   |   file contents     |
+---------------+-------------------+---------------------+
```

- **Data:** 256 words = **512 bytes** (`DiskFace.wordsPerPage = 256`).
- **Label:** 10 words = **20 bytes** (`PilotDiskDefs.mc`: `sizeLabel = 12B`).
- **Header:** 2-word `DiskAddress` (`cylinder`, then `head:sector` packed one
  byte each). In PDI the header is redundant with record position and is omitted
  (`headerBytes = 0`); the sector's address is its VDA.

Pages are numbered sequentially across the pack from 0 (adjacent physical sectors
have consecutive page numbers — `DiskFace.mesa`).

The label holds the entire file structure. A data-only image cannot represent the
filesystem; PDI preserves the label per sector.

---

## 2. The label (10 words) — both ID generations

`DiskFace.Label` (confirmed against `PilotDiskDefs.mc`):

| Word | Field | Meaning |
|----:|-------|---------|
| 0–4 | `fileID` | File identity (5 words). Variant per generation — see below. |
| 5 | `filePage` low | Low 16 bits of the file-relative page number. |
| 6 | `filePage` high | High bits (`[0..127]`; `maxPagesPerFile = 2^23 − 1`). |
| 7 | `attributes` | Page role (§2.2). |
| 8–9 | `dontCare` | Software-defined; the boot-chain link (a `DiskAddress`) when booting. **Not** matched on label verify. |

The disk hardware/microcode verifies words 0–7 on every access and reads words
8–9 without verifying them ("8 words checked, last 2 read"). When the controller
advances through a run of pages it auto-increments `filePage` (word 5/6).

### 2.1 `fileID` — the generation switch (`DiskFace.FileID`, `SELECT OVERLAID`)

The 5-word `fileID` slot is an overlaid union with **no stored tag**; the
generation is fixed per volume and recorded in PDI `flags` bit 2:

- **Cedar nucleus** (`flags` bit 2 = 0) — variant `rel`: a `File.FP` followed by
  one zero fill word.
  - words 0–1: `FileID` — 32-bit per-volume unique file id (`File.FileID: TYPE[2]`).
  - words 2–3: `DA` — 32-bit disk-address hint (`File.DA: TYPE[2]`).
  - word 4: fill, `= 0`.
- **Original Pilot** (`flags` bit 2 = 1) — variant `abs`: an 80-bit globally
  unique id.
  - words 0–4: `UniversalID` (`System.UniversalID: TYPE[5]`), permanently and
    universally unique.

`nullFileID` / `nullID` is all-zero in both generations.

### 2.2 `attributes` (word 7) — concrete values (Cedar nucleus)

From `VolumeFormat.Attributes` (Cedar file types occupy the Pilot type space
`[9728..9984)`):

| Value (dec / hex) | Name | Role |
|---:|------|------|
| 1 | `physicalRoot` | physical-volume root page (page 0 of the pack) |
| 2 | `badPageList` | physical-volume bad-page list |
| 3 | `badPage` | a page marked bad |
| 4 | `subVolumeMarker` | physical/logical subvolume marker pages |
| 5 | `logicalRoot` | logical-volume root page |
| 9728 / 0x2600 | `freePage` | a truly free page in a logical volume |
| 9729 / 0x2601 | `header` | a file run-table / properties (leader) page |
| 9730 / 0x2602 | `data` | a file data page |
| 9984 / 0x2700 | `lastCedar` | end of the Cedar attribute range |

A page's `attributes` value is the **authoritative** free/used and role marker
(the VAM bitmap, §6, is only a hint).

---

## 3. Physical volume root page (page 0)

`VolumeFormat.PhysicalRoot`. Seal `PRSeal = 121212`₈ (= 41610 = `0xA28A`) in word
0; `PRCurrentVersion = 6` in word 1. 256-word page; `checksum` is the last word.

| Word | Field | Type (words) | Notes |
|----:|-------|----|-------|
| 0 | `seal` | 1 | MUST be `121212`₈ |
| 1 | `version` | 1 | `6` |
| 2 | `labelLength` | 1 | chars used in `label` (`[0..40)`) |
| 3 | `pvID` | `VolumeID` (5) | physical-volume id |
| 8 | `bootingInfo` | `ARRAY[checkpoint..bootFile] OF DiskFileID` (4×9 = 36) | microcode finds boot files **at this fixed offset** |
| 44 | `label` | packed `[0..40)` chars (20) | volume label, ASCII |
| 64 | `subVolumeCount` | 1 | `[0..6)` |
| 65 | `subVolumeMarkerID` | `AbsID` (5) | id stamped on subvolume marker pages |
| 70 | `badPageCount` | `PageCount` INT (2) | |
| 72 | `maxBadPages` | `PageCount` INT (2) | `= allocatedBadPages` (one page of page-numbers) |
| 74 | `onLineCount` | 1 | temporarily unused |
| 75 | `subVolumes` | `ARRAY[0..6) OF SubVolumeDesc` (6×13 = 78) | the subvolume table (§4) |
| 153 | `fill1` | (99) | zero |
| 252 | `timeParametersValid` | BOOL (1) | |
| 253 | `timeParameters` | `TimeParameters` (2) | opaque |
| 255 | `checksum` | 1 | §8; MUST be last word |

`credentialsPageNumber = 2`. The bad-page list (attribute `badPageList`) is on
the page **following** the root, i.e. page 1: an `ARRAY[0..256) OF PageNumber`
(`allocatedBadPages = wordsPerPage / SIZE[PageNumber] = 256/2 = 128` entries... —
note `BadPageList` is `wordsPerPage / SIZE[PageNumber]` = 128 `PageNumber`s, one
page). An unused entry holds `nullBadPage = page 0`.

---

## 4. Subvolume table and markers

A physical volume hosts up to **6 subvolumes** (`maxSubVols = 6`); each is a
fragment of some logical volume. A logical volume is the concatenation of its
subvolumes (possibly spanning multiple packs).

`VolumeFormat.SubVolumeDesc` (13 words):

| Word | Field | Type (words) |
|----:|-------|----|
| 0 | `lvID` | `VolumeID` (5) — the logical volume this fragment belongs to |
| 5 | `lvSize` | `LogicalPageCount` INT (2) — total size of that logical volume |
| 7 | `lvPage` | `LogicalPage` INT (2) — logical page where this fragment starts |
| 9 | `pvPage` | `PageNumber` INT (2) — physical page where this fragment starts |
| 11 | `nPages` | `LogicalPageCount` INT (2) — pages in this fragment |

**Subvolume end marker** (`PhysicalMarker`, attribute `subVolumeMarker`): seal
`PSMSeal = 141414`₈ (= 49932 = `0xC30C`), version 0. Marks the end of each
subvolume on the physical side and carries non-reconstructable copies of physical
root + preceding subvolume descriptor + a checksum. Fields: `seal`(0),
`version`(1), `pvID`(2, 5w), `label`(7, 20w), `bootingInfo`(33B=27, 4×9=36w),
`maxBadPages`(77B=63), `labelLength`+`svNumber` packed (101B=65), `descriptor`
(102B=66, a `SubVolumeDesc`).

**Logical subvolume marker** (`LogicalSubvolumeMarker`): seal `LSMSeal =
151515`₈ (= 54093 = `0xD34D`), version 0. The logical-side end marker;
`seal`(0), packed `labelLength`/`type`/`coCedar`(2), `label`(3, 20w),
`bootingInfo`(27B=23, 6×9=54w).

---

## 5. Logical volume root page

`VolumeFormat.LogicalRoot`. Seal `LRSeal = 131313`₈ (= 45771 = `0xB2CB`) word 0;
`LRCurrentVersion = 5` word 1. Lives at the logical-volume root (a `logicalRoot`
page within a subvolume's physical range). 256-word page; checksum last word.

| Word | Field | Type (words) | Notes |
|----:|-------|----|-------|
| 0 | `seal` | 1 | MUST be `131313`₈ |
| 1 | `version` | 1 | `5` |
| 2 | `vID` | `VolumeID` (5) | logical-volume id |
| 7 | `labelLength` | 1 | |
| 8 | `label` | packed `[0..40)` chars (20) | volume name |
| 28 | `type` | `VolumeType` (1) | `pilot=0, pilotDebugger=1, pilotDebuggerDebugger=2, cedar=3` |
| 29 | `volumeSize` | `LogicalPageCount` INT (2) | size in pages |
| 31 | `bootingInfo` | `ARRAY[checkpoint..debuggee] OF DiskFileID` (6×9 = 54) | boot files |
| 85 | `rootFile` | `ARRAY VolumeFile OF RootFile` (16×6 = 96) | well-known root files (§5.1) |
| 181 | `fill` | (71) | zero |
| 252 | `coCedar` | BOOL (1) | |
| 253 | `lastFileID` | `File.FileID` (2) | last 32-bit file id allocated (nucleus) |
| 255 | `checksum` | 1 | §8 |

### 5.1 Root files — `File.VolumeFile` enumeration

`rootFile[i]` is a `RootFile = { fp: File.FP (4w), page: File.PageNumber (2w) }`
naming a well-known file. Index `i` is `File.VolumeFile`:

| # | Name | # | Name |
|--:|------|--:|------|
| 0 | `checkpoint` | 6 | `VM` (virtual-memory backing) |
| 1 | `microcode` | **7** | **`VAM`** (volume allocation map, §6) |
| 2 | `germ` | **8** | **`client`** (directory-system root → the FS/PFS name layer) |
| 3 | `bootFile` | 9 | `alpine` (Alpine file servers) |
| 4 | `debugger` | 10–15 | spare |
| 5 | `debuggee` | | |

There is **no `VFM` (volume file map) slot in the Cedar nucleus** — see §7.

`BootFile.DiskFileID` (9 words), used in every `bootingInfo` entry:
`fID`(0, `DiskFace.FileID` 5w) · `firstPage`(5, INT 2w) · `firstLink`(7,
`DontCare` 2w — initial boot-chain link). `nullDiskFileID` = `nullFP` + zeros.

---

## 6. Free space — the VAM

`rootFile[VAM]` points to the **Volume Allocation Map**, an ordinary file holding
a bitmap indexed by logical page number.

`VolumeFormat.VAMObject`:

| Word | Field | Type (words) |
|----:|-------|----|
| 0 | `rover` | `LogicalPage` INT (2) — allocator search hint |
| 2 | `size` | `LogicalPageCount` INT (2) — pages covered |
| 4… | `used` | bitmap (`PACKED ARRAY OF BOOL`) |

**Polarity and bit order (confirmed in `VolAllocMapImpl`):**
- **bit set (1) = page in use; bit clear (0) = free.**
- Within each 16-bit word, **LSB-first**: page *p* is bit `1 << (p mod 16)` of
  word `4 + p/16` of the VAM data.

The VAM is a **hint and is generous**: every free page is marked free, but a page
marked free *may* be in use. The authoritative free/used state is the page's
`attributes` label (`freePage` vs `header`/`data`). A reader treats the VAM as an
accelerator and a scavenger reconciles it against labels.

> **Implementation note.** Rusty Backup writes the VAM as a real root file
> (`rootFile[VAM]`, FileID 1) and **rebuilds its bitmap from the page labels**
> after every create / add / delete — the scavenger-correct direction, so the
> VAM can never drift from the truth. The reader cross-checks the VAM free count
> against the label scan and reports any disagreement.

---

## 7. Files — run tables, properties, enumeration

A file is a **header (leader) page** (or pages) plus its **data pages**:

- **Run table** — `VolumeFormat.LogicalRunObject`, stored in the header page
  (attribute `header`). Files are **extent-based**, not page chains:

  | Word | Field | Type (words) |
  |----:|-------|----|
  | 0 | `headerPages` | 1 |
  | 1 | `maxRuns` | 1 |
  | 2 | `intention` | `RunTableIntention` (3): `unstable` bit + `size` INT |
  | 5… | `runs` | `ARRAY OF LogicalRun` |

  `LogicalRun = { first: LogicalPage (2w), size: RunPageCount CARDINAL (1w) }`
  (3 words). The list terminates with a run whose `first = lastLogicalRun =
  LAST[INT]`. The run table is a **hint**: the real pages are an initial subset
  of those it describes; labels are the truth.

- **Properties** — each file has `propertyWords = 256` words (one page) of
  uninterpreted property storage in its leader (`File.PropertyStorage`).

- **Enumeration** — `File.NextFile[volume, prevFP] → FP` walks files in **physical
  disk-layout order** (a label scan). There is **no separate on-disk file-map
  index in the Cedar nucleus**: the per-file run table plus the page labels are
  sufficient, and `client` (`rootFile[8]`) holds the higher-level *name*
  directory (FS/PFS B-tree), not a nucleus file map.

  > Resolves the handoff's "VFM unknown": the **Cedar nucleus eliminated the
  > VFM**. The **original Pilot** generation *does* have both a `Vam` and a `Vfm`
  > (volume file map) plus a `Free` pseudo-file (`VolAllocMapImpl` references
  > `LogicalVolume.{Vam, Vfm}`). For original-Pilot *fast* enumeration the VFM
  > node format would be needed; a label scan works for both generations without
  > it. The VFM node layout remains the one open original-Pilot item (§10).

- **File size is a page count, not a byte count.** The nucleus `File.GetSize`
  returns a `PageCount`; there is no byte-exact length at the nucleus level. The
  byte length of the last page (and the file's human name) is a **PFS/client
  layer** concept stored above the nucleus. So a nucleus-level reader is
  page-granular by design — exact byte length arrives with the PFS naming layer.

---

## 8. Page checksum (root / marker pages)

Root and marker pages carry a software checksum in their last word
(`Checksum.ComputeChecksumSoftware`): a 16-bit ones-complement sum with a left
rotate after each word, normalizing `0xFFFF → 0`.

```rust
/// Pilot page checksum over `words` (the page words preceding the checksum word).
fn pilot_checksum(words: &[u16]) -> u16 {
    let mut cs: u16 = 0;
    for &w in words {
        let (t, carry) = cs.overflowing_add(w);   // 16-bit ones-complement add
        cs = if carry { t.wrapping_add(1) } else { t };
        cs = if cs & 0x8000 != 0 {                 // left rotate by 1
            (cs << 1) | 1
        } else {
            cs << 1
        };
    }
    if cs == 0xFFFF { 0 } else { cs }
}
```

Computed over the page's words `[0..255)` and stored at word 255. (This is the
page-integrity checksum, distinct from per-sector hardware ECC, which is **not**
stored in PDI — the consumer regenerates it; §9.)

---

## 9. Out of band (handled by the emulator, not this format)

- **Hardware ECC / sync / gap** — a controller concern. PDI stores logical
  sectors only; a cycle-accurate consumer regenerates ECC. (PDI spec §1, §7.)
- **Bare-metal formatting & placement** — choosing which physical pages hold the
  PV root, bad-page list, subvolume markers, LV root, and VAM on a freshly
  formatted pack is done by an offline installer (e.g. Othello), not the runtime
  nucleus, which only reads/mounts/manages existing volumes. Our blank-volume
  creator follows Othello's layout: it reserves
  `FormatTrident.pagesReservedInPartition1 = 3 * 28 = 84` pages of
  physical-volume overhead (PV root at page 0, bad-page list at 1, credentials
  at 2, then the "Initial" boot region), and starts the first subvolume — with
  the LV root at its logical page 0 — at page 84. An emulator that mounts via the
  subvolume table + `rootFile` pointers doesn't depend on the exact page numbers.

---

## 10. Geometry and a reference Pilot-on-Trident shape

The Dorado Trident formatted **for Pilot** (from `PilotDiskDefs.mc`):

- **815 cylinders × 5 heads × 28 sectors**, 256-word (512 B) pages. (28 of the 29
  usable sectors are addressed, so a partition is a contiguous VDA interval under
  Pilot's virtual→real mapping; the emulated head number is ignored.)
- `DiskAddress`: word 0 = `cylinder`; word 1 = `head` (high byte) · `sector`
  (low byte).

This is a sensible default geometry for a created Pilot PDI; the format itself is
geometry-independent (the PDI header is authoritative).

### Open items

1. **Word order** of 32-bit fields is defined as low-word-first (microcode-
   confirmed for `filePage`); re-confirm `INT`/`LONG` fields inside `VAMObject`,
   `SubVolumeDesc`, and run tables byte-for-byte when the first volume round-trips.
2. **Blank-volume placement** — *done*: matches Othello's 84-page
   (`pagesReservedInPartition1`) physical-volume reserve, LV root at the first
   subvolume's logical page 0, VAM as `rootFile[VAM]`. Self-consistency is the
   bar (no period oracle); exact byte-for-byte parity with a real Othello volume
   is unverifiable until one exists.
3. **Original-Pilot VFM node format** — needed only for fast original-Pilot
   enumeration; label scan suffices meanwhile. (Write side now synthesizes
   80-bit `UniversalID`s; read handles both generations.)
4. **PFS/FS name layer** — human-readable names live in the `client` root file's
   B-tree; decoding them is a separate, later layer.
5. **Checksum range** — confirm `[0..255)` (whole page minus checksum word) on the
   first real round-trip.

---

## 11. Othello — the Pilot volume formatter / installer

Othello is the offline Pilot/Cedar **physical-volume manager and installer**:
the tool that does everything the runtime nucleus deliberately does *not* (the
nucleus only mounts and manages already-formatted volumes). It is itself a small
bootable Mesa world — one boot image per machine: `OthelloD0.boot` (Dolphin/D0),
`OthelloDLion.boot` (Dandelion/DLion), `OthelloDorado.boot` (Dorado) — that you
boot *instead of* the OS to administer the disk.

It is in the archive at **`indigo/cedar/othello/`** (and per-release copies under
`indigo/cedar5.3/installer`, `cyan/cedar6.{0,1}/installer`, `indigo/nucleus/
installer`). Ready-built Othello boot images are under `io/murray/*Othello*.boot`
and `cyan/d0/klamath/OthelloD0.boot`.

### 11.1 What Othello does (mapped to this format)

| Othello stage | Source | Produces | Reproducible by us? |
|---|---|---|---|
| **Low-level physical format** — write every sector's header/label/data and verify; mark bad pages | `FormatTrident.mesa` (`Format`/`Scan`), `FormatSA1000andSA4000.mesa` (Dandelion/Dorado rigid disks), `FormatSA800.mesa` (floppy) | a blank *physical volume*: all pages `freePage`, bad-page list populated | **Yes** — in PDI this is just "all sectors present, labelled `freePage`, data zeroed." We skip hardware ECC (the consumer regenerates it). |
| **Create physical volume + subvolumes** — write the PV root (seal `121212`₈), subvolume table, markers | Othello `Create` command → `PhysicalVolume`/`VolumeInitImpl` | PV root page 0 + bad-page page 1 + subvolume markers (§3, §4) | **Yes** — we write these structures with a valid checksum (§8). |
| **Create logical volumes** — partition the physical volume | Othello `<LVname> <pages> <type>` (type via `GetLvTypeFromUser`: `normal`/`debugger`/…) | LV root pages (seal `131313`₈), VAM, root-file table (§5, §6) | **Yes** — structural; we write them. |
| **Install germ + physical-volume boot file** — into `bootingInfo` + the boot chain | Othello `Initial`; `FormatTrident.InstallBootMicrocode`, `MicrocodeFile.mesa` | a physical volume whose `bootingInfo[germ]`/`[bootFile]` are populated and chained | **Yes, with the real images** — the *content* is an unclonable Mesa memory image, but we **have** the period `Dorado.germ` + `BasicCedarDorado.boot`, and the image is **position-independent** (the germ reads its boot-file location from `bootingInfo` at run time — `BootChannelDisk.mesa`/`DiskBootSoft.mc`), so an installer lays it at any free run and records where. Implemented: `pilot::install_boot_file` (`pilot_probe install-boot`). The Initial **microcode** region (hardware-addressed) is still clone/MB-format-only. |
| **Install a system image ("Diagnostic")** — write a SYSOUT/`.db` OS image to a logical volume | Othello `Diagnostic <LV> <file.SYSOUT>`, over FTP from a server | an installed, runnable Cedar/Pilot volume | **Only by cloning** a real SYSOUT — same constraint as above. |

### 11.2 Othello command language (from real `.othello` scripts)

Othello reads command scripts (`*.othello`). A representative partition +
install run (`PARTITION29LISP.OTHELLO` + `INSTALLFULL.OTHELLO`):

```
Create RD0 LispUser 3          -- format drive RD0, physical volume "LispUser", 3 logical volumes
Othello 800   debugger         -- LV "Othello", 800 pages, type debugger
Lisp   16200  normal           -- LV "Lisp",  16200 pages, type normal
Dsk           normal           -- LV "Dsk",   remaining pages, type normal
Online RD0                     -- bring the drive online
Open   ERIS                    -- open FTP file source (a server)
Initial    RD0 <Lisp>Current>...Initial.db   -- install microcode/germ/boot ("Initial" region)
Diagnostic Lisp <Lisp>Current>Full.SYSOUT    -- install the OS image onto LV "Lisp"
Close
Quit
```

So Othello = **format + partition + install-bootloader + install-OS** in one
tool. The first three lines are pure structure (we reproduce them); the last two
move real boot/OS content from a server (we cannot synthesize that content).

### 11.3 The practical split for this project

- **"Blank Pilot/Cedar disk"** = Othello's format + create-PV + create-LV stages,
  with no boot files. Fully reproducible from this spec; validated by round-trip.
- **"Othello-formatted disk"** = the same, laid out to match Othello's *placement
  conventions* (subvolume sizes, marker/Initial-region positions read out of the
  Othello/`VolumeInitImpl` source) so an emulator or a real Othello sees a
  well-formed, partitionable volume. Reproducible — it differs from "blank" only
  in matching Othello's exact page geography, not in needing boot content.
- **"Germ + boot-file install"** = a blank/Othello volume whose `bootingInfo`
  points at an installed germ + physical-volume boot file, label-chained.
  **Implemented** (`pilot::install_boot_file`) using the real `Dorado.germ` +
  `BasicCedarDorado.boot`; byte-exact round-trip through our reader. A
  *structural* target — not yet bootable on any emulator (the xerox-dorado disk
  path is a stub). See `PARC_README.md` §7b.
- **"Fully installed/runnable disk"** = additionally the hardware Initial
  microcode region + a SYSOUT OS image. Still **gated on a real source** (MB-format
  microcode; a cloneable SYSOUT), exactly like Alto bootability.

---

## 12. Trying it out — a validation workflow (no emulator required)

The point of defining the format ourselves is that we can exercise it *now*,
against our own conformance instead of an emulator. A practical progression:

1. **Round-trip a blank volume (self-consistency).** `create_blank_pilot(geometry,
   generation)` → write PDI → re-read → assert: PV root seal/version/checksum
   verify; subvolume table resolves; LV root seal/checksum verify; VAM size
   matches `volumeSize`; every non-structural page labelled `freePage`; free count
   = volume size − reserved. This is the primary bar with no oracle.
2. **Add files, re-read.** Create a file (header page with a run table + data
   pages, labels `header`/`data`, VAM bits set), enumerate via label scan
   (`NextFile` order), read its bytes back byte-identical, then delete and confirm
   the VAM/labels return to `freePage`. Mirrors the Alto `parc_probe` harness.
3. **Cross-check the two ID generations.** Build the same volume as Cedar-nucleus
   (32-bit, `flags` bit 2 = 0) and original-Pilot (80-bit, bit 2 = 1); confirm
   both round-trip and that a reader keys off `flags` bit 2 alone.
4. **Match Othello geometry.** Use the reference Pilot-on-Trident shape (815 × 5 ×
   28, §10) and confirm sizes/offsets line up with the Othello `FormatTrident`
   constants; diff our blank against the placement `VolumeInitImpl` would choose.
5. **(When available) Emulator / real-volume confirmation.** Hand the friend a
   blank PDI (`fsFamily = 2`) and have the emulator mount it via the subvolume
   table + `rootFile` pointers; or, if a real Othello-made volume ever surfaces,
   image it to PDI and diff field-by-field against this spec — the first true
   external oracle. Anything that disagrees is a spec bug we fix here.

CLI surface to drive 1–4 (proposed, mirrors Alto `parc_probe`): `pilot_probe
new <geometry> <gen> out.pdi`, `pilot_probe probe <pdi>` (dump roots + VAM +
file list + checksum verify), `pilot_probe add/del/extract <pdi> …`.

---

## 13. Source map (within the PARC archive `~/PARC-Stuff`)

| Topic | Path |
|---|---|
| Per-sector header/label/data model; `Label`, `FileID` overlay | `indigo/cedar/heads/DiskFace.mesa!1` |
| Consolidated on-disk layout (roots, markers, VAM, run table, attributes) | `indigo/cedar/file/VolumeFormat.mesa!1` |
| Cedar-nucleus `FP`/`FileID`/`VolumeID`/`VolumeFile`/`PageNumber` | `indigo/cedar/file/File.mesa!1` |
| `LogicalPage`/`LogicalPageCount`/`TimeParameters`; subvolume model | `indigo/cedar/file/PhysicalVolume.mesa!1` |
| Boot-file `DiskFileID`/`Header` | `indigo/cedar/tentacles/BootFile.mesa!1` |
| Original-Pilot 80-bit `UniversalID`/`FileID`/`VolumeID` | `indigo/cedar/pilot/System.mesa!2` |
| Original-Pilot VAM/VFM allocator (polarity, bit order, `Vfm`/`Free`) | `indigo/cedar/pilot/VolAllocMapImpl.mesa!1` |
| Page checksum algorithm | `indigo/cedar/pilot/Checksum.mesa!1` |
| Microcode label/disk-address layout; Pilot-Trident geometry; boot chain | `indigo/cedar/microcode/dorado/DMesaSources.dm!7_/PilotDiskDefs.mc` |
| Boot chain follow | same dir: `DiskBootSoft.mc`, `DiskBootTransfer.mc` |
| Runtime mount/manage (invert for create placement) | `indigo/cedar/file/{PhysicalVolumeImpl,LogicalVolumeImpl,FileImpl,FileInit}.mesa!1` |
| Othello formatter / installer (command set, formatters) | `indigo/cedar/othello/` (`OthelloDefs.mesa`, `FormatTrident.mesa`, `FormatSA1000andSA4000.mesa`, `FormatSA800.mesa`, `MicrocodeFile.mesa`) |
| Othello boot images (per machine) | `io/murray/*Othello*.boot`, `cyan/d0/klamath/OthelloD0.boot` |
| Real Othello command scripts (grammar examples) | `erinyes/lisp/fugue.6/cm/*.OTHELLO`, `indigo/loops/*.othello` |

*All structural facts are taken from the cited sources and cross-checked by field
offset (every `bootingInfo`/`subVolumes`/`rootFile` array span lands exactly on
the next declared field). Re-confirm byte order on the first volume round-trip.*
