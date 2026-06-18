# Xerox PARC Filesystems & Disk Images — README

A working guide to the Xerox PARC filesystem + disk-image support built into
**Rusty Backup**, written for the Alto/Cedar/Dorado emulator project. It explains
the container format, what's implemented, how to try it, and what's been
validated against real hardware images vs. defined from sources.

> **The premise (updated).** Two real emulators now act as validation oracles:
> the **Salto** Alto II emulator for the Alto (Diablo/BFS) side, and **Dwarf**
> (Draco/Duchess) for the **Pilot/Cedar** side — Dwarf ships real ViewPoint 2.0
> and XDE 5.0 Pilot disks, which are the first period Pilot volumes anyone has
> had to test against. So Pilot/Cedar is **no longer "defined-only"** — it is
> validated against real disks (and the oracle corrected three things our
> source-only spec got wrong; see §6). The only family still without a surviving
> sample is **Alto TFS (Trident)** — implemented and self-consistent, but
> synthetic-validated.

---

## 1. The document set

| Document | What it is |
|---|---|
| **`PARC_README.md`** (this file) | Overview, status, and how to use the tools. Start here. |
| [`PARC_FS.md`](PARC_FS.md) | Full filesystem analysis (Alto BFS/TFS, IFS, Pilot/Cedar) + design rationale + implementation log. |
| [`PARC_DISK_IMAGE_SPEC.md`](PARC_DISK_IMAGE_SPEC.md) | Normative spec for the **PDI** container (the on-disk image format). |
| [`PARC_PILOT_FORMAT.md`](PARC_PILOT_FORMAT.md) | Normative Pilot/Cedar on-disk format (labels, volume roots, VAM, run tables, Othello). |
| [`PARC_HANDOFF.md`](PARC_HANDOFF.md) | Earlier next-steps doc. **Largely superseded** — its #1 item (a Pilot oracle) is resolved by Dwarf, and TFS is now implemented; this README is current. |

All structural facts cite the PARC source mirror at `~/PARC-Stuff` (CHM archive).

---

## 2. The one idea that matters: labels are stored per sector

Every Alto and Pilot disk records each sector as **three** independently
addressed fields, not one:

```
[ header (disk address) ] [ LABEL (file id + chain/run + free/used) ] [ DATA ]
```

The filesystem's entire structure lives in the **label**, not the data. So a
plain "raw image" of just the data areas **cannot represent these filesystems**.
Every container here preserves the per-sector label. That single constraint
drives the whole design — and it's why the Dwarf *Draco* `.zdisk` (label-bearing)
is usable but the Dwarf *Duchess* `.dsk` (flat, data-only) is not.

---

## 3. What's supported today

| Format / FS | Read | Write / Create | Browse (GUI) | Notes |
|---|:--:|:--:|:--:|---|
| **PDI** container (`.pdi`) | Yes | Yes | — | Our flat, self-describing, label-inclusive image. `fsFamily` selects Diablo / Trident / Pilot. |
| **CopyDisk** packs (`.bfs`/`.copydisk`/`.altodisk`) | Yes | (via PDI) | Yes | Period Alto network-stream packs; ~41 in the archive, all Diablo. |
| **Salto** `.dsk` (Alto II emulator) | Yes | Yes | Yes | Auto-detects byte order; `write` emits Salto-native little-endian. Boots in Salto (§5). |
| **ContrAlto2** `.dsk` (Diablo) | Yes | — | Yes | ContrAlto2/Bitsavers Diablo pack (`[dummy][header][label][data]`, sector-interleaved); same size as a Salto `.dsk`, distinguished by content. Read as Alto BFS. |
| **Dwarf** `.zdisk` / `.zdelta` (Draco 6085) | Yes | Yes | Yes | **Real Pilot disks** (ViewPoint/XDE). zlib stream, label-inclusive; the Pilot oracle (§5a). |
| **Alto BFS** (Diablo 31/44) | Yes | Yes | Yes | Browse, extract, add/delete, resize; the mature path. |
| **Alto TFS** (Trident T-80/T-300) | Yes | Yes | Yes | Same FS, parameterized; pack container + PDI. **Validated against a real Spruce T-300 pack** (from ContrAlto2), §7. |
| **Pilot / Cedar** | Yes | Yes (blank + add/delete) | Yes (read-only) | Validated against real ViewPoint/XDE disks; file browse + names; both ID generations (§6). |
| **Cedar boot disk** (germ + boot file install) | Yes | Yes | — | Install the real `Dorado.germ` + `BasicCedarDorado.boot` into a volume's `bootingInfo` + boot-chain (§7b). A **structural** target: byte-exact round-trips through our own reader; **not yet bootable on any emulator** (the xerox-dorado disk path is a stub — §7b). |

---

## 4. The PDI container (in one minute)

A single self-describing file: a fixed 512-byte header (magic `PARCDISK`,
geometry, field sizes, flags, `fsFamily`) followed by one fixed-size record per
sector in disk-address order, each `[header?][label][data]`. The key invariant:

```
recordOffset(VDA) = 512 + VDA × recordBytes
```

`fsFamily` is `0` Alto/Diablo, `1` Alto/Trident, `2` Pilot/Cedar; for Pilot,
`flags` bit 2 selects the 80-bit vs 32-bit file-ID generation. Full byte-level
spec: [`PARC_DISK_IMAGE_SPEC.md`](PARC_DISK_IMAGE_SPEC.md). ECC/sync/gap bytes
are **not** stored — PDI holds logical sectors; a cycle-accurate consumer
regenerates ECC. (Both emulators confirm this model: Salto and Dwarf each compute
checksums/ECC from the logical sector, and neither stores them in its image.)

PDI is our recommended interchange format, but we read the emulators' native
images directly too, so you don't have to convert.

---

## 5. Alto side — Salto interop and boot semantics

The Salto Alto II emulator stores a Diablo-31 pack as a flat array of 534-byte
records `[pageno·1w][header·2w][label·8w][data·256w]`, VDA order, no file header —
**exactly PDI's per-sector content**. Handled automatically: byte order
auto-detected and normalized (Salto-native is little-endian; some images are
big-endian); the 2-word sector header reconstructed (word 1 = packed Diablo DA);
no checksums stored (Salto computes them from the cooked data).

**Verified:** a disk our code *writes* (read `games.dsk` → our reader → our
writer) comes back **byte-identical** and **boots to the Alto Executive** in
Salto. Distributed Salto disks are `.dsk.Z` (Unix `compress`); `gzip -dc foo.dsk.Z
> foo.dsk` first.

### 5.1 How the Alto disk boot works, and why imaging must preserve layout

Traced against the microcode/OS sources and verified in Salto. Boot has **three
stages**; only the last is layout-dependent:

1. **Microcode (control ROM)** reads the boot disk address from memory loc 12₈,
   reads *one* sector (label → 402₈, data → loc 1), transfers to loc 1.
   (`AltoIICode3.mu`, `NOVEM`/`DiskBoot`.)
2. **Page-1 loader** (`DiskBoot.asm`) follows the sector **label `next`-chain** to
   load the rest, then `JMP @0`. Chain-driven — no absolute addresses.
3. **The OS image (`Sys.Boot`)** — `JMP @0` resumes a **frozen memory snapshot**
   of the OS (`InOutLd.asm`) that **embeds absolute disk addresses** of the live
   filesystem (e.g. `LastLdCB`). Depends on the disk's exact physical layout.

**Consequence for imaging/emulator tooling:** a bootable Alto disk can be
relocated **only verbatim**. We confirmed it the hard way — a defragmented
rebuild reproduces the boot image byte-for-byte (identical SHA, loads, `JMP @0`
succeeds) yet hangs, because the resumed OS dereferences a stale address. So
backup→restore preserves boot; defrag / file-edit / resize-to-new-geometry break
it, unavoidably. Treat a bootable pack as opaque for round-tripping.

### 5a. Pilot side — Dwarf as the oracle

[**Dwarf**](https://github.com/devhawala/dwarf) (Hans-Walter Latz) emulates the
Xerox Mesa machines. Its **Draco** half is a 6085/Daybreak running **Pilot**-based
ViewPoint 2.0 / XDE 5.0, and it ships three real disks — these are the first
period Pilot volumes available to test against.

- **`.zdisk` format** (Draco rigid disk, `iop6085/HDisk.java`): a raw **zlib**
  stream of `[6-word header: DAAD/heads/cyls/sectors/5CC5][per sector: dbl-word
  index, 10-word label, 256-word data]`, big-endian words, no ECC. A sibling
  `.zdelta` overlays changed sectors. We read both and import to a Pilot `Disk`.
  Geometry of the shipped disks: 8 heads × 960 cyl × 16 sec = 122 880 pages
  (~63 MB), Micropolis 1325.
- **Byte order gotcha (now handled):** on the 6085 the IOP moves *labels* over a
  byte-swapped DMA path, so **label words are stored byte-swapped relative to data
  words** (data is plain big-endian — the PV-root seal `0xA28A` reads directly).
  We normalize labels to big-endian on import and swap back on write.
- **Duchess `.dsk`** (the Guam/GVWin half) is flat, **data-only** (no labels), so
  it can't represent the filesystem and we don't browse it.

**Verified:** our Pilot reader parses all three Dwarf disks — correct volume names
(`XDE`, `ViewPoint`, `Tajo`), sizes, free-page counts, subvolumes — and lists and
extracts their files (xde5.0: 105 files; vp2.0.5: 659 across two logical volumes).

Your **xerox-dorado** emulator is also accounted for: its `dorado/src/disk.c` is a
clean Trident-pack reader we used as the TFS reference, and its bundled
`UnBug.bfs` (an Alto BFS pack) reads cleanly through our BFS path (119 files). Its
`firmware/*.BIN` are 6502 BaseBoard EPROMs (not disk format).

---

## 6. Pilot / Cedar — now validated against real disks

Defined normatively in [`PARC_PILOT_FORMAT.md`](PARC_PILOT_FORMAT.md); the
essentials, **as confirmed (and corrected) against the Dwarf disks**:

- **Sector label (10 words):** `fileID`(0–4) / `filePage`(5 lo, 6 hi) /
  `attributes`(7) / `dontCare`(8–9). 32-bit fields **low-word-first**. Page data
  = 256 words (512 B).
- **Volume structure (confirmed):** physical-volume root (page 0, seal `121212`₈,
  v6) → subvolume table → logical-volume root (logical page 0, seal `131313`₈) →
  VAM + files. A physical volume can hold **several logical volumes**; we
  enumerate all of them.
- **Two label schemes, both handled.** The shipped disks are **classic Pilot
  12.3**, where label word 7 is a `File.Type` (free page = **6**, VAM = 7, VFM =
  8, anonymous = 15, …) — *not* the Cedar-nucleus `attributes` enum (free = 9728,
  header = 9729, data = 9730) that our own writer emits. The reader recognizes
  both.
- **Both file-ID generations** (PDI `flags` bit 2): Cedar nucleus (32-bit FileID +
  DA hint) and original Pilot (80-bit `UniversalID`).

**Three corrections the oracle forced** (our source-only spec was wrong; now fixed
in code *and* `PARC_PILOT_FORMAT.md`):

1. **Root pages carry no computed checksum.** The `checksum` word (255) is
   declared `_0` and left zero by Pilot/Cedar — validity is the **seal + version**
   alone. (Earlier docs claimed a recomputable checksum; that was wrong.)
2. **6085 labels are byte-swapped** relative to data (the IOP DMA path, §5a).
3. **Classic-Pilot free/type encoding** (word 7 = `File.Type`, free = 6) coexists
   with the Cedar-nucleus encoding.

**Implemented in Rusty Backup:**

- **Read / inspect / browse / extract.** Parse the PV + LV roots and subvolume
  table; **enumerate files by a page-label scan across all subvolumes** (handles
  both label schemes, multiple logical volumes, and fragmented files — the
  scavenger's own method); read file contents.
- **Human file names** — the Pilot *nucleus* has no name directory, but files
  created through XDE's FileStream/FileTool carry their name in a **leader page**
  (logical page 0). We decode the XDE leaders (ASCII `0x1061` and 16-bit XCCS
  `0x1062`, "(dir)name(date)" with the date stripped) and the Cedar `fsLP`/`fullLP`
  leaders. **Coverage:** XDE volumes name ~90 % of files (e.g.
  `(XDEBoot7)TajoDove.boot`, `Mouse.TIP`, `ArpaConfig.bcd`); ViewPoint names its
  boot/system files. Files without a leader name surface by file ID.
  - **ViewPoint client files have no on-disk name** — no leader name, and the
    logical-volume root's `clientRootFile` is 0 (no Pilot central directory); a
    full-disk scan finds no name table. ViewPoint keeps user-visible names in the
    desktop / **NS-Filing** layer, which is not present on the local disk. So those
    surface by ID — an established limit, not a missing feature.
- **Create / edit.** Blank-volume creation (both generations, Othello-exact
  layout); add / delete files (header + data pages, multi-run allocation); the VAM
  is a real root file, rebuilt from the page labels on every change and
  cross-checked by the reader.

Files are page-granular (the nucleus `File.GetSize` returns a page count; exact
byte length lives in the PFS/client layer).

---

## 7. Alto TFS (Trident) — implemented, synthetic-validated

TFS is "the same Alto file system, parameterized for Trident hardware." Rusty
Backup implements it on a single shared codec:

- **`LabelCodec` (Diablo | Trident)** encodes/decodes the per-sector label —
  chain links, `numChars`, `pageNumber`, file id — for both shapes (8-word vs
  10-word; 1-word DA vs 2-word DH). The SysDir, leader pages, file pointers, and
  free-page bit table above it are byte-identical between the two. The BFS reader
  and the `build_disk` write engine both run on this codec + the page size, so the
  same code **reads and creates** Diablo *and* Trident volumes.
- **Geometry:** T-80 (815×5×9, ~75 MB), T-300 (815×19×9, ~285 MB); 2048-byte
  pages, 10-word labels.
- **Pack container** (`trident.rs`): the ContrAlto2 / dorado layout
  (`[dummy·1w][header·2w][label·10w][data·1024w]` per sector, little-endian),
  normalized to big-endian like Salto. A Trident volume stored as a **PDI**
  (`fsFamily = 1`) also browses directly.
- **CopyDisk diskType** for Trident is `2` (Diablo is `10`), for importing a
  Trident CopyDisk stream.

**Validated against a real Trident pack.** The **ContrAlto2** emulator ships a
real **Spruce print-server T-300** image (`Contralto/Disks/spruce-server-t300.zip`,
289 MB unzipped). Our reader walks it: SysDir at VDA 1, the DiskDescriptor
free-page count cross-checks, and the real files come out —
`Spruce.Spool` (30 MB), `spruce.fonts` (38 MB), `Spruce.Bands` (30 MB) — with
their multi-page chains followed correctly across the whole disk. (This corrected
two things: real Trident packs use a **physical sector interleave**, so we place
each sector at the VDA in its own 2-word header rather than by file position; and
the label `fileId` is **serialNumber-then-version**, the opposite order from
Diablo.) Synthetic round-trip (create → add → list → read → delete; pack-container
round-trip) covers the write path.

## 7a. Othello — formatting & installing a Pilot disk

Othello is the offline Pilot **volume manager / installer**. Reproducibility:

| Othello stage | Reproducible? |
|---|---|
| Low-level format (all sector headers/labels/data) | **Yes** — "all pages free, labelled" (ECC is the consumer's job) |
| Create physical volume + subvolumes + logical volumes | **Yes** — we write the roots/markers/VAM |
| Install germ + physical-volume boot file | **Yes, with the real images** — we have the period `Dorado.germ` + `BasicCedarDorado.boot` (not synthesized), and rusty-backup now installs them into `bootingInfo` + the boot chain (§7b). |
| Install the Initial **microcode** region; OS install from nothing | **Clone-only / out of scope** — the hardware-addressed Initial region and a from-nothing OS image still need a real source. |

So **blank**, **Othello-layout**, *and* a **germ + boot-file install** are now
feasible — the earlier "clone-only, none in the archive" caveat is superseded:
we have the real Dorado germ and Cedar 6.1 boot file, and (unlike the Alto
`Sys.Boot` snapshot, §5.1) the Cedar boot image is **position-independent**, so
an installer may lay it at any free run and just record where it went. See §7b.

## 7b. Installing a bootable Cedar disk for the Dorado (structural target)

**What this is.** A `.pdi` whose physical-volume root `bootingInfo` points at an
installed **germ** and **physical-volume boot file**, each written as a
label-chained Pilot boot file — i.e. structurally the disk a Dorado disk-germ
boot would read (`DoradoBooting.tioga` §1.3: Cedar microcode boot-loads the
installed germ, which loads the installed boot file). We have both real images
locally: `Dorado.germ!4` (32 pages) and `BasicCedarDorado.boot!22` (Cedar 6.1,
1060 pages).

**How it's laid out** (sources: `VolumeFormat.mesa`, `BootFile.mesa`, `File.mesa`,
`DiskBootTransfer.mc`, `BootChannelDisk.mesa`):

- **`bootingInfo`** is `ARRAY File.VolumeFile[checkpoint..bootFile] OF
  BootFile.DiskFileID` at **word 8** of the PV root; entries are 9 words
  (`fID` 5w · `firstPage` 2w · `firstLink` 2w). The disk-boot microcode reads
  **only the PV root's** copy. We populate `germ` (ordinal 2, word 26) and
  `bootFile` (ordinal 3, word 35).
- **`firstLink`** is the first page's disk address; in the PDI logical-sector
  model that's the **VDA** (a geometry-accurate consumer maps VDA ↔ cyl/head/
  sector for its pack, exactly as it regenerates ECC).
- **The boot chain** lives in each page's label `dontCare` (words 8-9): within a
  contiguous run the controller auto-increments; at a **run break** the last
  page of the run points at the next run's first page; the **last page of the
  file** holds `[-1, -1]`. The boot path uses this chain (not the run table),
  because it can't read the file system yet.

**Position-independence (the key enabling fact).** Confirmed against
`BootChannelDisk.mesa` / `DiskBootSoft.mc`: the germ reads the boot file's
location from `bootingInfo` at run time; the image embeds **no** disk address
(the loadmap is all VM addresses + file-relative pages). So the *content* is a
fixed Mesa memory image we can't synthesize — but we **have** it — and its
*placement* is free. That's what makes a from-scratch installer possible.

**Build one** (see §8 for the commands):

```sh
pilot_probe new 1400 cedar CedarDorado CedarDorado-boot.pdi
pilot_probe install-boot CedarDorado-boot.pdi germ     <path>/Dorado.germ!4
pilot_probe install-boot CedarDorado-boot.pdi bootfile <path>/BasicCedarDorado.boot!22
pilot_probe boot-info    CedarDorado-boot.pdi          # verify both chains
```

A prebuilt one is at `~/alto-demo/CedarDorado-boot.pdi` (745 KB). `boot-info`
walks each `bootingInfo` chain exactly as the boot microcode would and confirms
it; `extract-boot` pulls a slot back out byte-for-byte (sha256 matches the
source germ / boot file).

**Architecture of the prebuilt `CedarDorado-boot.pdi`.** A 1400-page (512 B/page)
Cedar volume; VDA = page index = PDI record index (`recordOffset = 512 + VDA *
532`, record = 20 B label + 512 B data). Verified by dumping the labels:

| VDA | Pages | Contents | `attributes` | label `fileID` |
|---:|---:|---|---|---|
| 0 | 1 | Physical-volume root — seal `121212`₈, **`bootingInfo`** (germ @ word 26, bootFile @ word 35) | `physicalRoot` (1) | `0001` |
| 1 | 1 | Bad-page list | `badPageList` (2) | `0001` |
| 2 | 1 | Credentials (blank) | `data` (9730) | `0001` |
| 3–83 | 81 | Reserved PV overhead — Othello's Initial / boot-microcode region (zero-filled; **no Initial microcode installed**, see scope) | (zero) | — |
| 84 | 1 | Logical-volume root — seal `131313`₈, logical page 0 | `logicalRoot` (5) | `0002` |
| 85 | 1 | VAM header (run table) | `header` (9729) | `0001` |
| 86 | 1 | VAM bitmap | `data` (9730) | `0001` |
| **87–118** | **32** | **germ** (`Dorado.germ`) — boot chain, `dontCare = [-1,-1]` at VDA 118 | `data` (9730) | `0002` |
| **119–1178** | **1060** | **boot file** (`BasicCedarDorado.boot`, Cedar 6.1) — boot chain, `[-1,-1]` at VDA 1178 | `data` (9730) | `0003` |
| 1179–1398 | 220 | Free pages | `freePage` (9728) | `0000` |
| 1399 | 1 | Subvolume end marker — seal `141414`₈ | `subVolumeMarker` (4) | `0001` |

The two `bootingInfo` entries that drive the boot, in the PV root (page 0):

```
bootingInfo[germ]     (word 26):  fID=0002  firstPage=0  firstLink=VDA 87
bootingInfo[bootFile] (word 35):  fID=0003  firstPage=0  firstLink=VDA 119
```

FileIDs are allocated from the LV-root counter: VAM = 1, germ = 2, boot file = 3.
(The LV-root *page* label also reads `fileID = 0002` because that is the logical
volume's id; our simple allocator's first file id coincidentally lands at 2 too —
harmless, since boot files are reached by the chain + `bootingInfo`, never by
scanning for a bare `fileID`.) Both boot files here are laid as a single
contiguous run, so the only non-trivial chain link is the `[-1,-1]` terminator;
a fragmented install would additionally carry forward links at each run break.

**Honest scope — it does not boot yet, and that is expected.**

- **The xerox-dorado emulator can't boot from disk at all today.** Its disk read
  data path is a stub: the runtime never mounts a pack (`dorado_disk_pack_load`
  is test-only), the read stream returns nothing without one, and the
  sequence-PROM/ECC/timing are unimplemented. The only path that boots
  end-to-end there is the **Alto NetExec over Ethernet**; the Cedar routes
  (CedarNetExec over Ethernet, or the `--germ` disk intercept) don't reach a UI
  yet — the Mesa VM isn't fully running. So this disk is a **structural test
  fixture** for bringing up that disk-germ path, not a runnable boot today.
- **Not installed:** the hardware-addressed **Initial microcode** region (Stage A
  of a disk boot) and a `bootingInfo[microcode]` soft-microcode file. The
  `install-boot ... microcode` slot exists, but the disk soft-microcode wants
  **MB** (micro-binary) format, whereas the Cedar microcode we have locally is
  **EB** (`CedarDorado.eb`, ether-bootable) — a different framing — so we don't
  install it by default. Identify the MB image and it drops into the same slot.
- **No run-table file objects** are emitted for the boot files (the boot path
  needs only the chain + `bootingInfo`); a running Cedar re-derives them via the
  scavenger. Adding catalog objects is a later refinement.

---

## 8. Tools — how to try it

```sh
cargo build --example parc_probe          # Alto BFS / Salto / Trident packs
cargo build --example zdisk_probe         # Dwarf .zdisk -> Pilot (validation)
cargo build --example pilot_probe         # Pilot / Cedar create / inspect
cargo build --example pilot_files_probe   # Pilot file population + list/extract
cargo build --release --bin rusty-backup  # the GUI (use release)
```

**Alto / Salto / Trident** — walk any pack:

```sh
cargo run --example parc_probe -- path/to/disk.dsk        # .dsk / .pdi / .bfs
```

**Dwarf Pilot disks** — read and inspect a real ViewPoint/XDE volume:

```sh
cargo run --example zdisk_probe -- ~/repos/dwarf/disks-6085/xde5.0.zdisk
cargo run --example pilot_files_probe -- ~/repos/dwarf/disks-6085/vp2.0.5.zdisk
```

**Pilot / Cedar** — create, inspect, add files:

```sh
cargo run --example pilot_probe -- new 4096 cedar MyVol out.pdi
cargo run --example pilot_probe -- add out.pdi somefile.bin
cargo run --example pilot_probe -- probe out.pdi          # use `pilot` for 80-bit gen
```

**Cedar boot disk** — install / verify / extract a germ + boot file (§7b):

```sh
cargo run --example pilot_probe -- install-boot out.pdi germ     Dorado.germ
cargo run --example pilot_probe -- install-boot out.pdi bootfile BasicCedarDorado.boot
cargo run --example pilot_probe -- boot-info    out.pdi               # walk both chains
cargo run --example pilot_probe -- extract-boot out.pdi germ germ.out # round-trip a slot out
```

**GUI** — Inspect → Browse opens a `.pdi` (Alto / Trident / Pilot), `.bfs`,
Salto `.dsk`, or Dwarf `.zdisk`. Pilot/Dwarf volumes show as a read-only
`Pilot/Cedar` volume; Alto/Trident packs are fully browsable/editable. Run the GUI
in **release** (a debug build trips egui's deadlock watchdog when backgrounded).

---

## 9. Validation status

| Path | How validated |
|---|---|
| Alto BFS read + PDI round-trip | All ~41 archive CopyDisk packs + the Salto/Dorado `.dsk` disks import + round-trip losslessly. |
| Alto BFS write/resize | 40/41 packs rebuild byte-identical; hermetic tests. |
| **Alto disk boots in Salto** | A disk our writer produces (byte-identical round-trip of `games.dsk`) **boots to the Alto Executive**. Backup→restore preserves boot; defrag rebuild does not (§5.1). |
| **Pilot/Cedar vs real disks** | Our reader parses all three **Dwarf** ViewPoint/XDE `.zdisk` volumes — names, sizes, free counts, subvolumes, file list + extraction. The oracle corrected the checksum / byte-order / type-encoding points (§6). |
| Pilot/Cedar create + edit | Round-trip self-consistency: subvolume/free counts, VAM cross-check, file read-back, PDI reopen. |
| **Alto TFS (Trident) vs real disk** | Reads ContrAlto2's real **Spruce T-300** pack — SysDir, DiskDescriptor cross-check, and the `Spruce.Spool`/`spruce.fonts`/`Spruce.Bands` files (chains + de-skew via per-sector headers). Write path covered by synthetic round-trip. |

---

## 10. Open items / where we'd value your input

The big one — a Pilot oracle — is **resolved** (Dwarf). Remaining:

1. **Disk-image format for your emulator** — load PDI as specified, or target a
   native layout? We already read Salto `.dsk`, Dwarf `.zdisk`, and the dorado
   Trident pack directly, so we can meet you wherever is convenient.
2. **Pilot target generation / release** — which Cedar/Pilot release (32-bit
   Cedar-nucleus vs 80-bit original-Pilot file IDs)? Both supported; default is
   Cedar-nucleus 32-bit. The Dwarf disks are classic Pilot 12.3 — if that's your
   target we can default to it.
3. **A real Trident pack** — *resolved*: ContrAlto2's Spruce T-300 image is a real
   one, and our reader handles it (§7). More samples would still help exercise the
   write path against real data.
4. **ViewPoint NS-Filing names** — recoverable only with the desktop/filing layer,
   not from the local disk. If that matters, it's a separate, server-side effort.
5. **Bootable disks.** *Alto:* preserved only by verbatim copy (§5.1) — a
   rebuild breaks the `Sys.Boot` snapshot. *Cedar/Dorado:* now **installable**
   from the real `Dorado.germ` + `BasicCedarDorado.boot` we have locally
   (position-independent; `bootingInfo` + boot chain — §7b), and a prebuilt
   structural disk is at `~/alto-demo/CedarDorado-boot.pdi`. It is **not yet
   runnable**, because the xerox-dorado disk read path is a stub (no path boots
   from disk there today; only the Alto NetExec-over-Ethernet path runs). The
   open work is on the **emulator** (the disk-germ data path + the Mesa VM), not
   on producing the disk — that's where we'd value your input next.

---

## 11. Source map

- PARC source mirror: `~/PARC-Stuff` (CHM archive). Per-topic paths at the end of
  [`PARC_FS.md`](PARC_FS.md) and [`PARC_PILOT_FORMAT.md`](PARC_PILOT_FORMAT.md).
- **Salto** (Alto oracle): disk format in `include/drive.h`, `src/drive.c`.
- **Dwarf** (Pilot oracle): `src/dev/hawala/dmachine/engine/iop6085/HDisk.java`
  (Draco `.zdisk` + the 10-word Pilot label), `.../agents/DiskAgent.java`
  (Duchess data-only `.dsk`). Disks in `disks-6085/*.zdisk`.
- **xerox-dorado** (Trident reference): `dorado/include/disk.h`, `dorado/src/disk.c`
  (pack format); `chm/.../PilotDiskDefs.mc`, `AltoDiabloDisk.mc`, `BFS.TTY` (format
  specs); `chm/dorado/UnBug.bfs` (sample Alto BFS pack).
- **ContrAlto2** (Alto/Trident emulator): `ContraltoLib/IO/DiskPack.cs` (the
  authoritative `.dsk`/`.dsk300` sector layout — little-endian, 2-byte dummy +
  header + label + data); `Contralto/Disks/*.dsk` (Diablo BFS disks) and
  `Contralto/Disks/spruce-server-t300.zip` (**a real Trident T-300 pack** — the
  TFS oracle).
