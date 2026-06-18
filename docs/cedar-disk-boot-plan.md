# Cedar Disk-Boot Plan (alternative to the stuck Ethernet path)

Status: 2026-06-17. Drafted while the Route B Ethernet bring-up is blocked on
the BootChannelDisk->BootChannelEther codeLink resolution bug
(`CONTINUE-HERE.md` session 23, follow-ups 1-9). Disk boot is a promising
PARALLEL track that **sidesteps that bug**.

## 1. Why disk boot, now

- **We finally have a real Cedar Dorado disk image.** PR #1
  (`danifunker:disk-things`) adds `CedarDisk/CedarDorado-boot.pdi.gz` plus a
  normative Pilot/Cedar on-disk-format spec (`CedarDisk/PARC_PILOT_FORMAT.md`).
  Validated locally: PARCDISK container, `fsFamily=2` (Pilot), 512-byte data +
  20-byte labels, **1400 pages**, 512-byte file header. Page 0 is a valid
  **physical-volume root**: seal `0o121212`, version 6, `physicalRoot` label,
  `labelLength=11`. `bootingInfo[germ]`=FileID 2, `bootingInfo[bootFile]`=FileID
  3. So the image is a genuine, bootable-shaped Cedar physical volume -- the
  first one the project has had.
- **It avoids the Ethernet codeLink bug.** The germ's default request is
  `pRequest=[bootPhysicalVolume, sa4000]` (PilotBoot.mc). `BootChannelDisk.Create`
  MATCHES `sa4000` in its `SELECT` and calls `SA4000Face` directly -- it does NOT
  fall through to the `RemainingChannels.Create` indirect codeLink that the
  Ethernet path dies on. And Initial's own disk read of the germ happens BEFORE
  the germ runs at all. So the disk path exercises a different code path that
  the Ethernet codeLink defect doesn't touch.
- **Much of the machinery already exists.** `dorado/src/disk.c` models the
  Dorado disk controller + a Trident drive; `machine.c` already INTERCEPTS the
  germ's disk-read at the `DiskBootSoft` spin (PC `0o7012`, the
  `DISK_CMD_DESCRIPTOR`/`DISK_CMD_LABEL`/`DISK_CMD_GERMDATA` passes) to plant the
  germ. We extend that, plus add the Pilot disk format.

## 2. The one real mismatch to bridge: Alto-Trident vs Pilot-Trident format

Our disk model (`disk.h`) is the **Alto** Trident: 1024-word (2048-byte)
sectors, 9 sectors/track, ContrAlto/Bitsavers byte layout. The Cedar/Pilot
Dorado disk is the **same physical drive formatted for Pilot**: **256-word
(512-byte) pages, 28 sectors/track, 815 cyl x 5 heads**, with **20-byte labels**
that hold the entire filesystem (`PARC_PILOT_FORMAT.md` §1, §10). These are
different sector geometries and label semantics. So we add a **Pilot disk mode**
alongside the Alto mode rather than reshaping the existing one.

## 3. Phased plan

### Phase 0 -- validate + land the assets (mostly DONE today)
- [x] Pull + decompress `CedarDorado-boot.pdi`; confirm PARCDISK/Pilot header.
- [x] Parse + validate the PV root (seal/version/physicalRoot/bootingInfo).
- [ ] Merge PR #1 so the image + spec live in-tree (`CedarDisk/`).
- [ ] Offline PDI inspector: a small CLI (`pdidump`, C in `dorado/src/`, or reuse
      the Python parser) that dumps PV root, LV root(s), subvolume table, VAM,
      and a label scan (file list with FileID + attributes). Deliverable: see
      exactly where the germ (FileID 2) and bootFile (FileID 3) pages live, and
      confirm the page checksums (`PARC_PILOT_FORMAT.md` §8).

### Phase 1 -- PDI loader + Pilot disk model in the emulator
- [ ] `pdi.c`/`pdi.h`: load a PARCDISK `fsFamily=2` image into memory as an
      array of (label[10w], data[256w]) pages indexed by page number (= VDA).
- [ ] Add a **Pilot geometry mode** to `disk.c` (815x5x28, 256-word pages,
      20-byte labels) selectable per-drive; keep the Alto mode intact.
- [ ] VDA<->(cyl,head,sector) mapping per Pilot's virtual->real convention
      (`PilotDiskDefs.mc`: 28 of 29 sectors addressed; emulated head ignored).
- [ ] Label-verify semantics: "8 words checked, last 2 read"
      (`PARC_PILOT_FORMAT.md` §2); page read with `filePage` auto-increment.
- [ ] Unit test (`test_pdi`/`test_disk`): mount the image, read page 0, verify
      the PV root seal + recomputed checksum; read the germ file by FileID and
      check its first page.

### Phase 2 -- wire the Pilot disk into the boot read path
- [ ] Study the read path: `PilotDiskDefs.mc`, `DiskBootSoft.mc`,
      `DiskBootTransfer.mc` (the IOCB/command structure, label match, transfer).
      The handoff already pulled `DiskDefs.mc`/`DiskSubrs.mc`; cross-read with
      the Pilot variants (`PARC_PILOT_FORMAT.md` §13 source map).
- [ ] Make the DSK controller (task 14, TIOA 10-14) serve Pilot-format
      sectors from the mounted PDI for real disk commands -- closing the F1-F5
      gaps (sequence-PROM read, label match, status) for the Pilot case. Start
      pragmatic (like the Ethernet fake server / the existing PC-`0o7012`
      interception) and tighten toward the real controller path.

### Phase 3 -- native Cedar disk boot
- [ ] **Initial reads the germ from disk.** Point Initial's
      `DiskHardMicrocodeBoot` (or an `InitialDisk` Cedar variant) at the mounted
      PDI; it reads the PV root, finds `bootingInfo[germ]` (FileID 2), reads the
      germ pages, plants + runs it. (Replaces today's `--germ` plant with a real
      disk read -- the same content arriving the hardware way.)
- [ ] **The germ reads the OS from disk.** With the DEFAULT
      `pRequest=[bootPhysicalVolume, sa4000]` (drop `--germ-netboot-bfn`), the
      germ's `BootChannelDisk` reads the PV root + `bootingInfo[bootFile]`
      (FileID 3) and `DoInLoad`s the boot file. This is the path that bypasses
      the Ethernet codeLink bug.
- [ ] Goal: the germ reaches `DoInLoad` over disk and streams the boot file ->
      MP code progresses past `germStarting` to `germInLoad`/`germFinished`.

### Phase 4 -- tighten to cycle-accurate (later)
- [ ] Replace any high-level interception with the real Pilot disk-controller
      microcode path (sequence PROMs, Fire Code ECC if the boot verifies it).

## 4. Smartest first concrete step

Two cheap, decisive moves before any big build:
1. **Offline `pdidump`** (Phase 0) -- fully map the image (germ/bootFile page
   runs, checksums, LV root, VAM). No emulator risk; confirms the image is
   complete and bootable and gives ground truth for Phase 1 tests.
2. **Germ-source-from-PDI smoke test** -- reuse the EXISTING `machine.c`
   germ-planting interception but source the germ bytes from the PDI's germ file
   (FileID 2) instead of `--germ`. If the germ from the image plants + runs like
   `Dorado.germ-6.1.6`, that validates (a) the image's germ is good and
   compatible with `CedarDorado.eb!6`, and (b) our page-read-by-FileID logic --
   for almost no new code. (Check the image germ's version matches the
   microcode first.)

## 5. Risks / open questions

- **The germ's disk path may hit a codeLink issue too.** `BootChannelDisk ->
  SA4000Face` is also an inter-module call. It may use direct (`zEFC`,
  known-working) links rather than the broken indirect codeLink -- likely fine,
  but verify early (it could even clarify the Ethernet bug by exercising the
  resolution in a cleaner spot).
- **`bootingInfo[*].firstPage` reads as 0** in the image -- boot files are
  probably located by **FileID label scan**, not `firstPage`. Confirm the
  lookup mechanism in `DiskBootSoft.mc` (FileID match vs firstPage/run table).
- **Germ/microcode version match.** The image's germ (FileID 2) must match
  `CedarDorado.eb!6` (Cedar 5.3/6.0/6.1). Verify before trusting a disk boot.
- **High-level fake vs real controller.** Phase 2 can start as a high-level
  interception (fast to a boot, like Ethernet) and be tightened later; decide
  per how faithful we need the first boot.
- **No period oracle for the format**, but the spec is self-consistent and the
  image round-trips through Rusty Backup's reader; treat any disagreement as a
  spec bug (`PARC_PILOT_FORMAT.md` §0, §12).

## 6. References
- PR #1 `danifunker:disk-things`: `CedarDisk/CedarDorado-boot.pdi.gz`,
  `CedarDisk/PARC_PILOT_FORMAT.md`, `CedarDisk/PARC_README.md`.
- Emulator: `dorado/src/disk.c`, `dorado/include/disk.h`, the `machine.c`
  germ-planting interception (PC `0o7012`).
- Microcode: `PilotDiskDefs.mc`, `DiskBootSoft.mc`, `DiskBootTransfer.mc`
  (`chm/.../DMesaSources.dm`); germ `BootChannelDisk.mesa`.
- Existing disk gaps: `docs/handoff.md` §F (F1-F5), `docs/disk-architecture.md`.
