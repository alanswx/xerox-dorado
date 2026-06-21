# Booting Alto software off an emulated disk (Diablo-on-Trident) — plan

**Status: THE OS BOOTS AND LOADS FROM DISK (2026-06-20). Milestones A+B+C
done and verified; the DSK-task completion store and the data-block order are
fixed, so `DiskBoot` reaches `Start` and the loaded image runs and loads ~893
blocks across its segments. Remaining: the running image does not yet render
the Exec -- an in-vivo Alto-opcode divergence localized with the salto
cross-check (see "Update 2026-06-20 (cont.)" below).**

## Implementation status (2026-06-20)

Done and verified:
- **Milestone A — pack geometry parameterized.** `dorado_disk_geometry` now
  carries `header_words/label_words/data_words` (0 => native-Trident defaults);
  `DORADO_DISK_DIABLO = {206,5,29, 2,8,256}`. `pack_load`/`pack_save` and the
  controller read/write stream use the per-media accessors. Native Trident and
  Cedar PDI paths are byte-for-byte unchanged (the accessors return the old
  constants). Regression green: 11/11 tests, Cedar login 28463 dark px (gate
  28465), Galaxian 121548 dark px (gate 121553).
- **Milestone B — `dsk2trident` converter** (`src/dsk2trident.c`,
  `make`/`build/dsk2trident`). Converts a ContrAlto Diablo-31 `.dsk` into a
  Diablo-on-Trident pack. Mapping verified byte-exact against the microcode:
  `cyl = diabloCyl + offsetCylinderDiablo(3)`, `sector = nSectorsDiablo(16B=14)
  * effHead + diabloSector`, `effHead = diabloHead XOR (diabloCyl & 1)`
  (staggerSectors). The Trident sector's header block carries the Alto/Diablo
  disk address verbatim (the `.dsk` header word1 = DA; the microcode header
  block reads it). `--all-heads` replicates onto every head so the boot finds
  the image regardless of the partition the AEmu picks. Octal Micro literals
  pinned via the "626B = 406" comment in ASeek.
- **Milestone C — mount + boot trigger + real controller read.** `--disk
  SLOT=PATH` auto-detects the Diablo geometry. `--eb worlds/aemu.eb --disk
  0=PACK --boot-reason disk` netboots AEmu then drives `ABoot -> DiskBoot`
  (BS up, `[NONE]` chord) — confirmed: the DSK task runs AltoDiabloDisk
  (`InitRamDiablo`, `DoACmmd`), seeks to the mapped cylinder, and streams
  header/label/data through the real FIFO. This is the first real consumer of
  the DSK-task framed-FIFO drain (Cedar's `--disk-real` uses the `read_page`
  bridge, not the microcode drain), which surfaced and fixed three controller
  bugs:
  1. **Per-block FIFO framing (D2).** Each block is followed by 2 garbage +
     2 ECC words in the FIFO (`AltoDiabloDisk DoAltoCmmd`/`ReadECC`:
     "a read block ends with 2 garbage words and 2 ECC words"). The stream is
     now framed (`read_block_framing`); the `read_page` bridge stays
     contiguous.
  2. **`head_overflow` latch.** The disk-task init probes T-80 vs AMS-315 by
     selecting illegal head 5 (overflow), then clears it with
     `tagDiskReset` (control-tag bit 3, `0o10`). The C control-tag handler did
     not clear `head_overflow` on DiskReset/ReZero, so every later read
     reported a hardware error. Fixed.
  3. **Read-stream one-shot.** `advance_sector` restarted the read stream on
     every sector pulse (because `enable_run` + a stale transfer op stayed
     set), resetting `read_stream_index` mid-block and aborting the read with
     `AReadBadTW`. The start is now one-shot per DiskControl command
     (`xfer_pending`); reads complete cleanly (`ACmmdEnd2`, zero aborts).

Remaining blocker (the boot still falls to EBoot and the screen stays blank):
- **DSK-task command completion store.** The DSK reaches `ACmmdEnd2`
  (~21x) but the completion stores diverge: it never writes the boot KCB
  status at VM 432, and only occasionally advances KBLK (VM 521). `DiskBoot`
  spins in `KWait` and times out (`KBootTimeout -> EBoot`). Trace evidence:
  998 stores to VM 522 (the AltoLoop per-sector status) but 0 to VM 432 and
  only 3 to VM 521, while `ACmmdEnd2` (real 0o2367) is entered 21x — so the
  flow diverts between `ACmmdEnd2` entry and its stores (around
  `Call[DoMuffOutput]` / the conditional `ReSchedule`). This is the genuine D2
  frontier: the real DSK-task completion/reschedule path. Note `DiskBoot`'s
  `KWait` relies on **deferred branch conditions** (a `Branch[..,ALU=cc]`
  testing the *previous* microinstruction's ALU) -- verify the microengine
  models that on this path before chasing the store.

Repro: `./build/dorado-sdl --eb worlds/aemu.eb --disk 0=<diablo.pack>
--boot-reason disk --speed 4000000`. Traces: `DORADO_ALTOBOOT_TRACE=1`
(boot-decision + completion PCs + KCB dump), `DORADO_DISK_SEQ=1`,
`DORADO_DSK_PC_TRACE=1`, `DORADO_DISK_MUFF_TRACE=1`,
`DORADO_STORE_TRACE_VA="430,525"`.

## Update 2026-06-20 (cont.) — the boot now reaches `Start`; the load works

Two fixes cleared the "Remaining blocker" above and the boot now runs the
loaded image:

1. **Store routing (committed `e3ec7b2`).** The DSK-task completion stores
   (`Store_ T/KPtr` to VM 521/432 in `ACmmdEnd2`) were being misrouted to the
   slow-I/O disk muffler by a heuristic that treated any `lc==0` memory store
   as I/O. Removed the `(kind == DM_REF_STORE && lc==0)` clause; slow I/O is
   the `Output<-B` FF function (FF&077==0o36), modeled correctly. With the
   stores landing in memory, the boot status posts, `KWait` completes, and
   `DiskBoot` runs `T_ 1C, Branch[Start]` (PC=1) -- the loaded image executes.
2. **Data-block order (committed `4d4b54d`).** `AltoDiabloDisk.mc` drains each
   FIFO block into *descending* memory (`DskMAddr` counts down), so each block
   must be stored on-Trident word-reversed for word 0 to land at the low
   (entry) address. Without it the boot loader's entry word ended up at the
   high address and the loader looped on a bad BLT. `dsk2trident` now reverses
   each header/label/data block. After this the OS boots and loads ~893 blocks
   across its segments (healthy: ~3.5 M IFU dispatches in 100 M cycles).

### The in-vivo cross-check (salto) and the remaining divergence

Built and working: drive the C Alto reference emulator **salto**
(`AltoInfo/salto/`, gitignored) headless on the same `nonprog.dsk`, dump a
per-opcode trace (`SALTO_TRACE` in `src/emu.c`: `IR`, `AC0..3`, `CRY` at
`f2_load_ir`), and diff it against our `DORADO_IFUDISP_TRACE` opcode stream
(reconstruct `IR = (op<<8)|alpha`). Trim both to the boot-loader entry
(`IR=000345`, the `JMP 0345`) and compare.

Findings:
- **IR (control-flow) sequences match for 11457 opcodes**, then diverge. The
  diverging opcode is `125014` = `COM AC1,AC1, no-load, SZR` (skip if
  `~AC1 == 0`, i.e. skip if `AC1 == 177777`): salto skips (next `000403`),
  ours does not (next `000764`). So **AC1 has diverged** by op 11457 -- a real
  Alto-opcode-state divergence, not a disk problem.
- It is **not** the boot-entry AC state. salto enters the loader with
  `AC = 0,0,0,0`; a one-shot clear of our Alto ACs (STK[1..4]) at `DiskBoot`
  made the entry match but did **not** move the op-11457 divergence -- so the
  garbage entry ACs are overwritten before they matter. (That experiment was
  reverted; it is not the fix.)
- It is **not** a single-opcode bug. `altodiff-dorado` (the per-opcode oracle)
  runs `ADC`/`COM` etc. correctly in isolation (e.g. `ADC AC0,AC0` with AC0=0
  -> 177777), and the 20768-vector sweep is clean. So the divergence is a
  **context / multi-cycle-dependent** AC computation -- exactly the long-
  predicted "ctx1 corruption" (a multi-cycle op such as `BLT`/`BLKS`/`BitBlt`,
  or stale `Q`/`Cnt`/`ShC`/carry/`StkP` carried between opcodes, that altodiff's
  isolated single-step does not reproduce).

Caveat for the next push: our `IFUDISP` AC sample is offset by ~one opcode
versus salto's `f2_load_ir` sample (the previous opcode's STK writeback is not
yet visible at our dispatch trace point), and the offset is fragile around
multi-cycle ops. **The reliable next step is to cycle-align the AC sample** (or
sample STK[1..4] right after the writeback) so the first true AC divergence can
be pinned to a specific opcode, then reproduce that opcode's in-vivo entry
state in `altodiff` to expose the stale-state dependency.

Regression after all of the above (tree clean, only `e3ec7b2`/`4d4b54d` etc.
committed): `make test` 11/11, Galaxian 121641 dark px (gate 121553), Cedar
login 28460 dark px (gate 28465).

### Update: cycle-aligned AC trace (`e237877`) -- two divergences, not one

The "not the boot-entry AC state" conclusion above was an artifact of the
one-opcode trace offset. With the offset removed it is wrong; there are TWO
separate divergences:

- **`DORADO_ALTOAC_TRACE`** (committed `e237877`) prints each Alto opcode's IR +
  instruction set + STK[1..4] *after* `apply_lc()` (the writeback that finishes
  the opcode), exactly matching salto's `f2_load_ir` sample. With it the diff is
  reliable opcode-for-opcode.
- **Divergence 1 (FIXED, `e237877`): entry ACs.** salto disk-boots cold (AC=0);
  our AEmu inherited the dirty emulator stack. Clearing Stack[1..4] once at
  `DiskBoot` makes the first **11453** booted opcodes match salto exactly
  (AC and IR), vs diverging at opcode 5 before. This is the cold-Alto fix.
- **Divergence 2 (OPEN): a memory-content divergence.** With divergence 1 fixed,
  the first mismatch is at booted opcode 11452, `IR=025412` = `LDA 1, AC3+0o12`
  (AC3=0o50 -> Alto `M[0o62]`). salto loads **020324**, ours loads **0**. The
  raw `.dsk` boot-sector word for `M[0o62]` is **010324**, so BOTH emulators
  overwrite `M[0o62]` after load and the writes diverge -- it is not a simple
  disk-read drop. Our store trace (`DORADO_STORE_TRACE_VA="62,62"`) shows a
  pre-boot EMU write of 0 (pc=0o6226) and many task-7 writes (pc=0o6655);
  the salto side has no memory-write trace yet.

Next step: instrument salto's memory writes (it has a flat 64K `mem[]`), dump
both emulators' full Alto memory at the instant each reaches booted opcode
11452, and diff -- the divergent words + the boot's load/clear pattern will name
the writer. (Since stores derive from ACs, which match through 11453, suspect a
non-AC writer: a block DMA, the `BLT` self-copy, or an indexed/indirect STA
whose address computation diverged.)

#### Update: traced the M[062] writers in both emulators

salto's writes to Alto `M[062]` were captured with a one-line `SALTO_MEMW` hook
in `write_mem()` (memory.c, gated on `getenv("SALTO_MEMW")`, prints
`va/data/altopc=cpu.r[6]/cyc`). Ours via `DORADO_STORE_TRACE_VA="62,62"`, and
`DORADO_ALTOAC_TRACE` now carries the cycle. Findings:

- The diverging read (booted opcode 11452, `LDA 1` from `M[062]`) is at our
  **cyc 80507368**.
- The last write to `M[062]` before that read is an **EMU program store of 0**
  (`pc=0o53` = STA microcode, Alto PC 024, cyc 80120094) -> our `M[062]=0`.
- A DSK-task DMA writes the correct **020324** to `M[062]` at **cyc 80700899**,
  ~193K cycles AFTER the read -- too late.
- salto produces `020324` via a **program store at Alto PC 220** (cyc 1434329),
  and its `M[062]` writers are at Alto PCs 002/351/202/136/220 -- NONE at PC 024.
  Ours writes `M[062]` from PCs 06/034/024 plus the DSK DMA.

So the write *patterns* diverge: salto fills `M[062]` from program stores; ours
fills it from a (late) DSK DMA while the program stores 0. Two non-exclusive
hypotheses:
1. **Memory diverged before opcode 11452.** The "11453 opcodes match" only
   covers AC0-3 + IR, not memory; a divergent *store* earlier (different value
   or address, ACs unchanged) would surface only here when re-loaded. ==> the
   decisive test is a full 64K Alto-memory diff at a common opcode index (both
   harnesses can dump at a chosen opcode count), then trace the first divergent
   word's writer.
2. **Disk-DMA timing race.** salto does disk reads synchronously (microcode
   stores, attributed to program PCs); our DSK task DMAs asynchronously and the
   data lands after the program reads it. If our read-completion handshake posts
   before the DMA finishes, the program reads stale memory. This is the D2
   completion-timing frontier (KWait's deferred branch conditions).

(The `SALTO_MEMW` hook lives in the gitignored `AltoInfo/salto/`; re-add it to
`write_mem()` if the salto tree is rebuilt.)

#### Root cause (full-memory diff, `33ce69d`): the disk subsystem, not the CPU

`DORADO_MEMDUMP_AT=<n>` (ours) and the matching salto hook dump `M[0..0o2000]`
at booted-opcode index `n` (counted from `IR=000345`) for a direct diff. At
BOTH opcode 1000 and 11000 only **~10 words differ**, and they are all in the
**disk control region**: `M[432..436]` (the DiskBoot KCB status block),
`M[522..523]` (KBLK), plus `M[002]`, `M[604..605]`. `M[062]` itself MATCHES (=0)
at opcode 1000 and 11000 -- it only diverges by 11452 as a downstream effect.
So the AC/IR match through 11453 is real and the CPU/microengine is not at
fault; the divergence is entirely in the disk subsystem.

Tracing the writers of the divergent words (`SALTO_MEMW=<octal>` vs
`DORADO_STORE_TRACE_VA`) shows the mechanism:
- In **salto**, low-memory disk data and control words are written by **program
  stores** (disk reads run synchronously as Alto microcode, attributed to
  program PCs) -- there is no separate disk task.
- In **ours**, the same words are written by the **async DSK task DMA**
  (task 14, `pc=0o3310/0o3323`) at cyc 79-80 M, interleaved with the program.
  For `M[062]` the DMA writes the correct `020324` but at cyc 80700899, ~193 K
  cycles AFTER the program reads it (cyc 80507368) -- the program reads stale
  memory.

So the booted Alto program's disk-driven control flow diverges because our
asynchronous DSK-task DMA + KCB status do not present the same
data-before-status ordering / KCB convention that salto's synchronous Diablo
read does. This is the **D2 completion frontier**: the fix is in the disk read
completion handshake (guarantee the block DMA lands in memory before the
command is reported complete) and/or the KCB status words the AltoDiabloDisk
microcode writes, so they match a real Diablo. It is disk-controller work, not
CPU work.

---

**Original plan (2026-06-20). Researched against AEmu / AltoDiabloDisk.mc +
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
