# Booting Alto software off an emulated disk (Diablo-on-Trident) — plan

**Status: plan (2026-06-20). Researched against AEmu / AltoDiabloDisk.mc +
ContrAlto Diablo format + the current C disk model.**

## Feasibility: yes — the microcode is already there

The Alto-on-Dorado world `AEmu.mb` contains the full Alto disk-boot path; it has
only ever been ether-booted so far.

- `AEm0.mc ABoot:` reads the emulated Alto KeyBits (VM 177034). BS down ->
  `EBoot` (Ethernet); otherwise falls into `DiskBoot:`.
- `DiskBoot`/`DiskBootRetry` build a KCB by hand at VM 431 (read command, data
  ptr = 1, disk address = 0), store the KCB pointer into the KBLK (VM 521), and
  spin on status (VM 432). On success: `PC=1, Branch[Start]` -> run the loaded
  image. On error/timeout: fall back to `EBoot`.
- The **DSK task** runs `AltoDiabloDisk.mc`, which services the KCB: maps the
  Alto/Diablo disk address to a physical Trident CHS, programs the Format RAM
  (`InitRamDiablo`), and drives the **same controller TIOA registers**
  (DiskControl 10 / DiskMuff 11 / DiskData 12 / DiskRam 13 / DiskTag 14) as the
  Pilot/Trident path -- block by block (header/label/data) through the FIFO.

Symbols confirmed present in `AEmu.mb`: `DISKBOOT`, `DISKBOOTRETRY`,
`INITRAMDIABLO`, `DOACMMD`, `AWAITSECTOR`, `SECTORWAITLOOP`, `UPDATESECTOR`,
`SETDRIVEANDSUBSECTOR`, `DISKSIO`, `CLEARDISK`.

**This is the authentic controller path** -- the DSK task drives the real FIFO,
so it exercises D1 (timing) + D2 (framed read) + D3 (ECC) + D5 (write) against
real microcode. (Cedar uses the IOCB bridge instead; Alto disk-boot does not.)

## The format / representation gap (the load-bearing change)

The Alto Diablo is emulated on **one head of the Trident**, low-level formatted
as **815 cyl x 5 heads x 29 short sectors**, each sector **2 header + 8 label +
256 data** words (+ 2 ECC). `InitRamDiablo` (AltoDiabloDisk.mc) programs the
Format RAM: `RAM[0]=1` (hdr 2w), `RAM[1]=7` (lbl 8w), `RAM[2]=0o377` (data
256w), `RAM[15]=1` (2 ECC words), `MaxSectors=0o36` (~29/30), subsector
count-1 = 3 (4 subsectors/sector), and sets `KSelect[4]` ("subsectors don't
evenly divide the revolution").

Our C pack is hardwired to the **native Trident** sector: `DORADO_DISK_DATA_WORDS
= 1024`, `LABEL_WORDS = 10`, 9 sectors/track (`disk.h`). It can't represent a
256-word Diablo sector or 29 sectors/track. The controller's read/write
streaming also uses the compile-time constants, not the Format RAM.

## Software change-set (ordered)

1. **Format-RAM-driven sector framing in the controller (the key piece, == D2).**
   Serve each block's word count from `format_ram[0..2]` (header/label/data
   counts the microcode loaded) plus the ECC count (`RAM[15]`), instead of the
   `DORADO_DISK_DATA_WORDS`/`LABEL_WORDS` constants. The same `refill_fifo` /
   read+write stream then serves both 256-word Diablo and 1024-word Trident
   sectors. The DSK task's `ACmmdRead`/`ACmmdWrite`/`ReadECC` loops drain/fill
   the FIFO per block, so the framing (data words then ECC/garbage words) must
   match what `AltoDiabloDisk.mc` expects. This is what makes Alto disk-boot
   work and finally gives the framed-FIFO path a real consumer.

2. **A Diablo-on-Trident pack representation.** Cleanest: a media type that
   carries its own `header_words/label_words/data_words/sectors_per_track`
   (like PDI carries `label_words/data_words`), so the native Trident pack
   (Cedar) is untouched. Geometry `DORADO_DIABLO = {815, 5, 29}` with
   `{hdr 2, lbl 8, data 256}`. Either parameterize `dorado_disk_geometry` +
   `dorado_disk_sector` (data/label sized from geometry, load/save computes
   sector bytes from geometry), or add a parallel `dorado_diablo_pack`.

3. **Diablo sector timing.** The drive-select/subsector path already exists;
   set the Diablo cadence (`MaxSectors`~30, 4 subsectors/sector) and honor the
   `KSelect[4]` "doesn't evenly divide" flag in the index/sector-pulse model so
   `UpdateSector`/`AWaitSector` find sectors.

4. **Mount + boot trigger.** Teach `--disk SLOT=PATH` to recognize a Diablo
   pack (by size/geometry) and a `--boot-reason alto-disk` (or just don't hold
   BS and don't serve ether) so `ABoot` falls into `DiskBoot`. Add
   `make run-alto-disk`.

## Building a test image

The Alto sees its disk only through the Diablo-on-Trident mapping, so the test
image must be a **Trident pack (815x5x29, 256-word sectors) populated from a
known-bootable Alto Diablo pack** via the mapping. ContrAlto `.dsk` files are
exactly that source: Diablo Model 31 (203 cyl x 2 heads x 12 sectors), per
sector `dummy(1w)+header(2w)+label(8w)+data(256w)` = 534 B, total 2,601,648 B,
little-endian. The repo has many already-bootable ones in
`AltoInfo/.../Disks/` (`bravox54.dsk` Bravo, `games.dsk`, `nonprog.dsk` exec, …).

**Converter (one-shot tool):** for each Diablo `(cyl, head, sector)` record in
the `.dsk`, write its header/label/data to the mapped Trident position in a
blank `815x5x29` Diablo pack:
```
DoradoCyl    = diabloCyl + offsetCylinderDiablo(3)      [drive 0; +406 for drive 1]
DoradoHead   = bootPartition - 1                         [code is partition-1, NOT the
                                                          stale header-comment +1]
DoradoSector = nSectorsDiablo(=12 for a model-31 .dsk) * diabloHead + diabloSector
```
Caveats to replicate exactly (else the boot read misses):
- **staggerSectors = 1** (default): on **odd** Dorado cylinders the Diablo head
  bit is flipped. The converter must apply the same flip.
- **interleaveSectors = 0** in this build -> no 3:1 sector remap (good, simpler).
- Confirm `DefaultPartition` (the head `DiskBoot` reads) and place the pack on
  that head.

Alternatives:
- **Install from inside the running Alto-on-Dorado** (now that D5 write works):
  ether-boot AEmu, then use the Alto's own `Install`/disk tools to write the OS
  onto the emulated Diablo-on-Trident drive. Avoids the converter but needs the
  full write path exercised by real microcode.
- **Keep netbooting** (already works: `make run-galaxian` etc.) -- disk-boot is
  strictly additive.

## Gotchas (from the source)
- Head mapping is **partition - 1** (AltoDiabloDisk.mc code), not +1 (stale
  header comment).
- ECC: the read expects `data words, 2 garbage, 2 ECC` per block (`ReadECC`),
  write writes a sync word `0o201` then data then 2 ECC -- the framed FIFO must
  produce/consume these.
- All `.mc` sources use CR line endings; normalize with `tr '\r' '\n'`.

## References
- `chm/doradosource/AEmuSources-cedar6.0.dm!1_/AltoDiabloDisk.mc` (mapping,
  `InitRamDiablo`, `DoACmmd`/`DoAltoCmmd`), `.../AEm0.mc` (`ABoot`/`DiskBoot`),
  `.../DiskSubrs.mc`, `.../DiskDefs.mc` (shared controller, TIOA 10-14).
- ContrAlto `AltoInfo/Contralto2-2.0-Beta/ContraltoLib/IO/DiskPack.cs`,
  `DiabloDrive.cs` (Diablo image format); `AltoInfo/.../Disks/*.dsk`.
- `dorado/include/disk.h`, `dorado/src/disk.c` (current Trident pack + the
  Format-RAM machinery to make framing-driven).
