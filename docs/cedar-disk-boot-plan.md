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

## 1a. CRITICAL UPDATE (2026-06-17): disk path does NOT bypass the codeLink bug

Tested the strategic premise directly: ran the germ with its DEFAULT request
(`pRequest=[bootPhysicalVolume(2), sa4000(3)]`, no `--germ-netboot-bfn`) and
traced where it goes. Result: it reaches `BootChannelDisk` (br31=3E1164, NOT
BootChannelEther -- correct routing) but **germERRORs at the EXACT SAME
codeLink** as the ethernet path: identical dispatches at BootChannelDisk pcf
`0o42`-`0o54` (op 070/057/`zLLKB`344/100/255/055/`zRET`343), same stack values
(`0o177774`, `0o3424`, `0o6200`), resolving `0o3424 -> 0o6200 -> TrapsImpl`
(unbound) -> germERROR. No disk I/O is ever issued.

So the `zLLKB #9` tail-call that fails is in **BootChannelDisk.Create itself**,
hit REGARDLESS of device (sa4000 vs ethernet) -- NOT the ethernet-specific
`RemainingChannels.Create` pass-through as earlier believed (note: codeLink #9
= `0o3424` is a FRAME link, not the BootChannelEther proc link `0o1221` at
index #2, so it was never the ethernet route). The codeLink resolution bug
(CONTINUE-HERE follow-ups 5-9) is therefore the **shared blocker for BOTH disk
and ethernet boot** -- the two tracks CONVERGE on it in `BootChannelDisk.Create`.

CONSEQUENCES:
- The disk track does NOT give a free parallel path around the codeLink bug.
  The PDI foundation (loader/inspector/extractor/tests) is durable, correct
  infrastructure and the disk image is the cleaner post-fix boot target, but
  reaching a disk boot is GATED on the same codeLink-resolution fix.
- Open sub-question this raises: does our emulator even take BootChannelDisk's
  `SELECT deviceType FROM sa1000,sa4000 => ...` branch for sa4000(3), or does
  it fall through? Both runs hit the same codeLink at the same PC, which hints
  the failing call is in BootChannelDisk.Create's common prologue (before/at
  the SELECT), reached identically either way.
- TWO ways forward (not mutually exclusive): (A) fix the codeLink resolution
  (unblocks BOTH disk and ethernet -- the real critical path); (B) a HIGH-LEVEL
  disk interception that serves the PV root + germ + bootFile from the PDI
  ABOVE the BootChannel layer (like the existing PC-`0o7012` germ-read fake),
  bypassing the buggy `BootChannelDisk.Create` entirely to reach a Cedar boot
  before the codeLink fix lands. (B) is more emulator-specific scaffolding but
  could get pixels sooner.

## 2. The one real mismatch to bridge: Alto-Trident vs Pilot-Trident format

Our disk model (`disk.h`) is the **Alto** Trident: 1024-word (2048-byte)
sectors, 9 sectors/track, ContrAlto/Bitsavers byte layout. The Cedar/Pilot
Dorado disk is the **same physical drive formatted for Pilot**: **256-word
(512-byte) pages, 28 sectors/track, 815 cyl x 5 heads**, with **20-byte labels**
that hold the entire filesystem (`PARC_PILOT_FORMAT.md` §1, §10). These are
different sector geometries and label semantics. So we add a **Pilot disk mode**
alongside the Alto mode rather than reshaping the existing one.

## 3. Phased plan

### Phase 0 -- validate + land the assets (DONE 2026-06-17)
- [x] Pull + decompress `CedarDorado-boot.pdi`; confirm PARCDISK/Pilot header
      (version 1, fsFamily 2, 1400 pages, 512-byte header, label(20)+data(512)
      sectors).
- [x] Parse + validate the PV root: seal `0o121212`, version 6, label
      "CedarDorado", **checksum 0x765A matches our `dorado_pilot_checksum`**,
      `bootingInfo[germ]`=FileID 2 (firstLink page 87), `[bootFile]`=FileID 3
      (firstLink page 119). 1 subvolume, LV at physical page 84.
- [x] Merge PR #1 (MERGED); assets in `CedarDisk/`.
- [x] PDI loader + inspector + extractor + unit test built and committed:
      `dorado/src/pdi.c`, `include/pdi.h`, `src/pdidump.c`, `tests/test_pdi.c`
      (`build/pdidump <pdi> --files --scan --extract FILEID OUT`). Full suite
      green.
- [x] Label scan: germ (FileID 2) = 32 `data` pages (filePage 0..31) + 1
      `logicalRoot` page; bootFile (FileID 3) = 1060 pages [119..1178].

**FINDING (germ identity) -- the disk germ is NOT byte-identical to
`Dorado.germ-6.1.6`.** Extracting FileID 2 in file-page order and comparing:
page 0 matches except its LAST word; only **6/32** pages match `germ-6.1.6`,
and the matching ones line up at the SAME page index (filePages 2,3,6,8,9,11).
So it is a RELATED 6.x-family germ (shared constant/table pages), most likely
the relocated/installed disk-resident form, not our standalone raw germ image.
IMPLICATION: a disk boot must use the disk's OWN germ (FileID 2) + matched
bootFile (FileID 3) as a set; do not substitute `--germ Dorado.germ-6.1.6`.
OPEN: confirm the disk germ is Cedar-6.1 and matches `CedarDorado.eb!6`
(the microcode we netboot in stage 1) before trusting a full disk boot.

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
