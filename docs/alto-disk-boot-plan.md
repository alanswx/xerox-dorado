# Booting Alto software off an emulated disk (Diablo-on-Trident) — plan

**Status: ALTO DISK PATH RENDERS FROM A REAL PACK BOOT (2026-06-21).
Milestones A+B+C are done. With a freshly regenerated full-size mirrored
Diablo-on-Trident pack, `--eb worlds/aemu.eb --disk 0=PACK --boot-reason disk
--no-alto-boot` reaches the pack-backed Alto world and installs a display list.
Keep `--no-alto-boot` in truth runs; an earlier "renders from pack" result was
actually Ethernet fallback. See updates 2026-06-21f through 2026-06-21h.**

## Implementation status (2026-06-20)

Done and verified:
- **Milestone A — pack geometry parameterized.** `dorado_disk_geometry` now
  carries `header_words/label_words/data_words` (0 => native-Trident defaults);
  `DORADO_DISK_DIABLO = {815,5,29, 2,8,256}`. `pack_load`/`pack_save` and the
  controller read/write stream use the per-media accessors. Native Trident and
  Cedar PDI paths are byte-for-byte unchanged (the accessors return the old
  constants). Regression green: 11/11 tests, Cedar login 28463 dark px (gate
  28465), Galaxian 121548 dark px (gate 121553).
- **Milestone B — `dsk2trident` converter** (`src/dsk2trident.c`,
  `make`/`build/dsk2trident`). Converts a ContrAlto Diablo-31 `.dsk` into a
  Diablo-on-Trident pack. Mapping verified byte-exact against the microcode:
  `cyl = 406 * diabloDrive + diabloCyl + offsetCylinderDiablo(3)`,
  `sector = nSectorsDiablo(16B=14) * effHead + diabloSector`, `effHead =
  diabloHead XOR (diabloCyl & 1)` (staggerSectors). By default the image is
  mirrored into emulated Diablo drives 0 and 1; the drive-1 copy sets the AEmu
  drive bit in the header word. `--all-heads` replicates onto every Trident head
  so the boot finds the image regardless of the partition the AEmu picks. Octal
  Micro literals pinned via the "626B = 406" comment in ASeek.
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

Historical blocker, fixed later (the boot used to fall to EBoot and the screen
stayed blank):
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

#### Pinned to the disk STATUS MUFFLERS (loader-entry memory diff)

A `DORADO_MEMDUMP_AT=0` diff (memory at the very first booted opcode, before the
loader runs) shows **exactly 10 differing words**, and per the `AltoDiabloDisk.mc`
header they are all disk control state:

```
KBLK (VM 521-524):  521 = KCB pointer        salto=1       ours=0
                    522 = status this sector  salto=007410  ours=017400
KCB  (VM 431-436):  432-436 = ending status / cmd / block ptrs (boot KCB; stale
                    after boot)
                    430 = RTC value (used only for the KWait timeout)
                    604-605 = (salto 000400/000001, ours 0/0)
```

`M[062]` and `M[002]` MATCH at opcode 0 -- they are downstream corruption.

Decoding `VM 522` (disk status; format in `AltoDiabloDisk.mc`: bits0-3 sector,
4-7 = 17B done, 8 seekFail, 9 seekProg, 10 notReady, 11 dataLate,
12 noDataXfer, 13 checksumErr, 14-15 completionCode):
- salto `007410` = sector 0, done, **bit 12 (no data transferred)** set.
- ours `017400`  = sector **1**, done, bit 12 clear.

The `AltoLoop` runs once per sector and stores this status word (built by
`AMapHdwStatus` reading the controller mufflers) into `VM 522` every sector, and
`ACmmdEnd2`/`AForgetCmmd` set `VM 521` (the KCB chain pointer). So our
**disk status mufflers** report different bits than a real Diablo/Trident
controller (different sector field, missing the idle "no data transferred"
bit), the AEmu microcode faithfully writes those into KBLK, and the running OS
reads divergent disk state -- which eventually corrupts `M[062]` and stalls the
render.

So the remaining fix is **disk status-muffler accuracy** (the D4/F5 muffler
bits `AMapHdwStatus` consumes: the idle/no-data-transferred bit and the
hardware sector field), NOT the CPU and NOT the boot-AC path. This muffler code
is shared with the working Cedar disk path, so any change must be regression-
tested against `make run-cedar` (28465 px) as well as the Alto disk boot. The
decisive bring-up check is to re-run the `DORADO_MEMDUMP_AT=0` diff and confirm
the 10 KBLK/KCB words match salto.

#### Confirmed: a memory patch can't fix it -- the muffler must (`202c5e8`)

Experiment: at the loader entry, `dorado_poke_va` the 10 divergent words to
salto's values (one-shot). The render stayed blank. Reason: the `AltoLoop` runs
once per sector and re-stores the disk status into `VM 522` (and command
completion rewrites `VM 521`) from our mufflers, so a one-shot patch is
overwritten within a sector. This proves the fix has to be in the muffler /
status generation that `AMapHdwStatus` reads, not in memory. (`dorado_poke_va`
-- the coherent storage+cache poke, counterpart to `dorado_visible_word_at_va`
-- is kept as a cross-check primitive; the one-shot experiment block was
removed.)

Concretely, the idle `VM 522` status differs as: salto `007410` = sector 0,
done(17B), bit-12 "no data transferred"; ours `017400` = sector 1, done, no
bit-12. So the muffler work is: (a) the idle/no-transfer status bit, and (b) the
hardware sector field reported when idle. Both feed `AMapHdwStatus`. Verify any
change with the `DORADO_MEMDUMP_AT=0` diff (10 words -> 0) AND `make run-cedar`.

#### Update: the muffler is NOT a heuristic; the I/O page was a real gap (`e383c84`)

Investigating "is the status muffler a heuristic to fix per hardware?": no. The
mufflers (`disk_muffler_bit`) are modeled from `AltoDiabloDisk.mc`/the HM; the
only shim (case 021 head_overflow) is gated to the Cedar PDI path and does not
touch the Alto disk boot. So the muffler was the wrong target.

A full 64K `DORADO_MEMDUMP_AT=0` diff found the actual hardware gap: our AEmu
maps the Alto address space 1:1 onto Dorado VM and left the **Alto I/O page
(177000-177777)** as zeroed RAM. A real Alto reads it as memory-mapped I/O:
undecoded addresses return `177777` (IOB floats high), so the active-low input
words UTILIN (177030-3) and keyboard (177034-7) read "nothing pressed", and the
per-task bank registers (177740-757) read `bank|177760` = `177760` for bank 0.
Ours read 0 = "everything pressed" / bank 15. Fixed by initializing the I/O
page at DiskBoot (gated to the Alto disk path). This cut the loader-entry diff
from 512 words to 14 and is hardware-grounded -- but it did NOT move the actual
boot divergence.

So the I/O page was a genuine but separate fidelity fix. The **boot still
diverges at the same point**: booted opcode 11452, `IR=025412` = `LDA 1, AC3+0o12`
-> `M[062]`; salto reads `020324`, ours reads `0`. `M[062]` is disk-loaded data:
our async DSK-task DMA writes `020324` to it at cyc 80700899, ~193K cycles AFTER
the program reads it at cyc 80507368. The remaining 14 differing words are the
disk-control state (KBLK/KCB) + EIA/MEAR registers the Dorado AEmu has no
hardware for. So the true root is the **disk read completion / DMA timing**
(the D2 frontier): the booted OS reads disk-loaded memory before our
asynchronous DSK DMA has delivered it, whereas salto's synchronous Diablo read
has the data in place. That is the next fix -- the read-completion handshake
must guarantee data-before-status, and it is shared with the Cedar path so it
must be regression-tested against `make run-cedar`.

#### Update: the hang is a disk-completion spin loop on M[051]; root is M[062] timing

Diagnosing OUR execution directly (not just diffing salto) with the
opcode-stream histogram: after the divergence the booted OS is stuck in a tight
4-opcode spin loop, ~48000x in 200k opcodes:
```
035000  LDA 3,AC2        AC3 <- M[AC2]      (AC2=024 -> AC3=M[024]=050)
021401  LDA 0,AC3+1      AC0 <- M[AC3+1]    (-> AC0 = M[051])
101015  MOV# AC0,AC0 SNR skip if AC0 != 0
000775  JMP back
```
i.e. it spins until `M[051]` (a disk completion flag) goes nonzero. At the
divergence point (opcode 11451) `M[051]=0 in BOTH` salto and ours, and the KBLK
pointers match (`M[024]=M[521]=050`); only `M[522]` (per-sector status, Trident
vs Diablo sector field) and the program words `M[002]/M[062-066]` differ. So the
spin is downstream -- the PRIMARY divergence is `M[062]` (disk-loaded data),
which ours delivers ~193K cycles late.

Note from `AltoDiabloDisk.mc` (line ~2227): the status sector field it inserts
is "the raw hardware sector number, not the emulated Diablo sector number. I
doubt anyone cares" -- so `M[522]`'s sector field is inherently Trident-based in
the Diablo-on-Trident emulation (and differs from a real Diablo / salto). It is
not yet proven whether nonprog is sensitive to that or purely to the `M[062]`
data-delivery timing.

Bottom line for the next session: the boot hangs in a disk-completion wait; the
fix is the **DSK read completion vs DMA-delivery ordering** so the booted OS
never reads disk data before the async DSK task has written it (D2). The salto
cross-check has reached its useful limit here -- salto is a synchronous,
real-Alto+Diablo emulator, so its remaining divergences (Diablo-vs-Trident
sector timing, EIA/MEAR hardware, Alto-boot-ROM vs AEmu DiskBoot) are legitimate
machine differences, not necessarily bugs in ours. Diagnose OUR disk
completion/DMA timing directly from here.

#### Update 2026-06-21: physical timing ruled out; failure is the disk STATUS the OS reads

Two findings:

1. **Physical Trident timing is correct** (Century Data spec, bitsavers -- see
   docs/disk-architecture.md): T-80 = 3600 RPM / 1209 KB/s, matched exactly. So
   the boot hang is NOT the drive timing.

2. **The disk-read mechanics work; the divergence is upstream.** Decoding the
   stuck state: the OS spins at Nova PC 273-276 on `M[051]` = KCB+1 (ending
   status) for the KCB at VM 050. That KCB's command `044120` = check-header,
   check-label, read-data, with KCB+4 (label ptr) = `062`. The first divergence
   `M[062]` is the LABEL buffer. Our DSK *does* deliver the right label
   (`020324`) to M[062] -- via `ACmmdCheck`'s "store disk word where memory is
   zero" (the deferred-branch path, which our microengine models correctly:
   eval_branch_condition reads cpu->alu_zero BEFORE it is updated for the
   current instruction -> previous-instruction ALU = deferred = correct). But
   salto's program had ALREADY written the *expected* label `020324` to M[062]
   (CHECK = verify), whereas ours wrote `0` (CHECK = read). That is a program
   **control-flow** divergence: the OS chose verify-vs-read differently because
   the **disk-control state it read earlier diverged** (M[522] disk status:
   sector field is the raw Trident sector per AltoDiabloDisk's "I doubt anyone
   cares" note, plus the idle "no data transferred" bit-12 that AMapHdwStatus
   never sets; and M[002], a status copy, follows).

So the remaining root is the **disk STATUS the booted OS consumes** (VM 522 +
derived), which the AltoDiabloDisk-on-Trident emulation reports differently from
a real Diablo (sector field, no-data-transferred bit). The DSK read/transfer
path itself is correct. This may be partly fundamental (nonprog is sensitive to
Diablo status details the Diablo-on-Trident microcode deliberately approximates)
-- to confirm, the next step is to instrument ContrAlto2 (which runs nonprog on
a real Diablo) and diff the exact VM-522 status reads the OS makes, vs ours.

#### Update 2026-06-21b: systematic across all disks; the disk CHECK is erroring

Tried multiple Diablo images through the disk-boot path (`dsk2trident --all-heads`
then `--disk 0=PACK --boot-reason disk`): **nonprog, games, bcpl all render
blank** -- so the failure is the Diablo-on-Trident disk-boot PATH, not a nonprog
quirk.

Tracing the completion store (`DORADO_STORE_TRACE_VA="51,51"`) corrected the
earlier "stuck spin loop" read: the **DSK does complete commands** -- task 14 at
`pc=0o2376` (`ACmmdEnd2`) writes the KCB+1 ending status, and task 0 (`pc=0o53`)
clears it to re-issue. So the boot PROGRESSES (completions out to cyc 162M+),
it is not hung on completion. But the ending-status values are
`027400`(normal), `027402`/`047402`/`127402`(...): the low 2 bits = **completion
code 2 = CHECK ERROR**. So the OS's read commands (which `check-header,
check-label, read-data`, e.g. cmd 044120 for disk addr 010324 = cyl26/head1/sec1)
intermittently **fail the header/label check** and the OS retries/diverges --
which is why M[062] (the label buffer) and downstream state diverge.

So the concrete, systematic bug is the **disk header/label CHECK failing**: the
data our controller presents for the check does not match the disk-address /
expected words the AltoDiabloDisk compares. The data READ path works (893 blocks
load), so this is specific to the CHECK comparison -- candidates: the header word
the converter stores vs the Alto disk-address format the check expects, the
header/label block alignment in the framed read stream for a check (vs read), or
the sector mapping delivering a sector whose header does not match the requested
address. Next: trace one ACmmdCheck (header check, disk addr 010324), compare the
disk header words our FIFO delivers against the expected M[060-061], and the
sector actually read vs requested.

#### Update 2026-06-21c: disk read VERIFIED correct; not timing; root is the disk STATUS

`DORADO_DISK_HDR_TRACE` confirms the disk READ is correct: for disk addr 010324
(cyl26/head1/sec1 -> Trident sector 16) the read delivers `header[0]=010324`
(the converter's reverse of .dsk `header[1]`, correct for the descending read ->
M[061]=010324), and the read sectors map with matching headers (16->sec1,
22->sec7, 17->sec2, ...). So the controller delivers the right sectors and the
right header/label/data; the CHECK errors are NOT bad disk data -- they come from
the program's EXPECTED values diverging.

A synchronous/fast-disk test (`DORADO_DISK_FAST_REV=290`) did NOT fix the boot,
so it is **not a disk-read timing race** and **not the M[522] sector-field
timing-phase** (both ruled out). The divergence is a real data/computation
divergence in the booted OS, driven by the disk **STATUS** it reads -- and the
one consistent (non-timing) status difference is the idle/seek-only
**"no data transferred" status bit (VM 522 bit 12, 0o10)**: salto (a real
Diablo) sets it; the AltoDiabloDisk's `AMapHdwStatus` never sets it (it maps the
Trident mufflers to NotReady/DataLate/SeekFail/done only). Since we run that
microcode faithfully, our status lacks bit 12 just as a real Dorado-AEmu would.

Conclusion: the Alto Diablo-disk boot fails SYSTEMATICALLY (nonprog, games, bcpl)
on the Diablo-on-Trident path, and the cause is the disk STATUS the booted Alto
OS consumes -- most likely a genuine incompleteness in the AltoDiabloDisk Diablo
emulation (the "no data transferred" bit, and the documented raw-Trident-sector
approximation), which we faithfully reproduce. This means Alto Diablo disks may
never have fully booted on a real Dorado-AEmu via AltoDiabloDisk. The disk-boot
INFRASTRUCTURE we built (geometry, converter, controller, framed read, ECC,
cold-AC/I-O-page/bank fixes) is correct. To confirm fundamentally, instrument
ContrAlto2 (boots nonprog on a real Diablo) and check whether the OS reads VM 522
bit 12. The validated working path remains the Alto ETHER boot (games, NetExec)
and Cedar login.

#### Update 2026-06-21d: controller sequencing bug fixed; render still blank

After checking the Trident manuals, the DiskControl/Format RAM interpretation was
tightened rather than treating the old conclusions as final. The controller now
sequences real-pack reads by DiskControl block operation and Format RAM block
counts; `RdFifoTW` uses the documented threshold (3 words for read, 1 for
read+check); `CompareErr`/`ReadDataErr`/FIFO under-overflow are explicit latches;
and polling `muffRdFifoTW` no longer starts a pending transfer as a side effect.

This fixed the immediate late check-abort failure. The failing trace used to load
`DiskControl` for one sector, then advance the emulated spindle before the DSK
task drained the transfer, so `ACmmdCheck` compared against the next sector's
header and reached `ACmmdAbort`. Rotation is now held while a transfer is
`Active`; this is a timing-compression compromise, not cycle-accurate hardware,
but it preserves the hardware invariant that a sector transfer completes inside
its sector window.

Verification:
- `make -C dorado test` passes.
- `DORADO_ALTOBOOT_TRACE=1 ./build/dorado --cycles 120000000 --eb worlds/aemu.eb
  --disk 0=../AltoDisks/nonprog-trident.pack --boot-reason disk` reaches repeated
  `DoACmmd`/`ACmmdEnd2` completions with no `ACmmdAbort`.
- The run still ends with `0 display-list pixels`, so this is not yet a validated
  Alto Diablo disk-render path. The next real bug is later than the controller
  block sequencing fixed here.

#### Update 2026-06-21e: DSK wakeup + overbroad NoDataTransferred hypothesis

Skeptical reread of the `ACmmdAbort` evidence changed the conclusion. With
`DORADO_ALTOCHECK_TRACE=1`, the apparent aborts were preceded by
`ACmmdSeekOnly` (0o3244), and the following store posted normal seek-only KCB
status. So the `ACmmdAbort` label is a shared tail, not proof that the check path
failed.

The real controller/scheduler bug was that `machine_run_until` only woke the DSK
task when `dorado_disk_controller_tick()` advanced to a new sector. FIFO
threshold wakeups are level conditions and must be serviced between sector
pulses. Waking DSK whenever `dorado_disk_controller_wakeup_pending()` is true
restored the expected disk-task drain rate.

After that, the remaining blank screen appeared to match the earlier status
hypothesis: the booted Alto OS consumes the Diablo status word's
`NoDataTransferred` bit (Alto bit 12, value 0o10), but `AltoDiabloDisk.mc` maps
Trident mufflers to the Alto KBLK/KCB status format without synthesizing that
Diablo bit. A pack-backed AEmu compatibility hook was added to OR 0o10 into
DSK-task stores to the per-sector status word (VM 0522) and normal KCB
completion statuses.

That was too broad. VM 0522 is the per-sector status word; KCB+1 is command
ending status. `AEm0.mc`'s `DiskBootRetry` treats low bits in KCB+1 as an
error, so adding 0o10 to the boot KCB completion status turned a successful
initial read (`017400`) into an apparent failure (`017410`), causing retries
and then `EBoot`.

Verification:
- `make -C dorado test` passes.
- `./build/test_disk` and `./build/test_fastio` pass.
- Headless Ethernet regression still passes:
  `./build/dorado --cycles 160000000 --eb worlds/aemu.eb --eftp
  ../chm/bootfiles/Galaxian.boot\!1` => `121553 display-list pixels`.
- Headless pack boot appeared to render:
  `./build/dorado --cycles 300000000 --eb worlds/aemu.eb --disk
  0=../AltoDisks/nonprog-trident.pack --boot-reason disk` =>
  `1474 display-list pixels`, but this was not a valid disk-only result because
  the run was allowed to fall back to Ethernet.

#### Update 2026-06-21f: fallback bug fixed; real disk-only path still blank

Truth-run with `--no-alto-boot` showed the pack was not rendering. The previous
300M-cycle render came from the default EFTP fallback, not the Trident pack.

Confirmed failure before the fix:
- `--disk 0=../AltoDisks/nonprog-trident.pack --boot-reason disk` entered
  `DiskBoot`, posted a boot KCB completion, retried, reached `EBoot`, and then
  rendered via EFTP.
- The same run with `--no-alto-boot` reached `DiskBootRetry` but produced
  `0 display-list pixels`, proving the pack path itself was still blank.
- Trace showed the DSK task stored boot KCB+1 as `017400`, then the
  compatibility hook changed it to `017410`. `DiskBootRetry` rejects that low
  `0o10` bit.

Fix:
- Remove the compatibility hook. It was post-mutating DSK-task stores after
  the real microcode wrote them; that invented behavior was not supported by
  `AltoDiabloDisk.mc` and made KCB command completions fail.

Result:
- With `--no-alto-boot`, AEmu now accepts the initial disk sector and starts
  executing the loaded Alto code (`IFUDISP` first booted IR `000345`). There is
  no `EBoot` fallback in the 90M-cycle trace.
- `nonprog-trident.pack` and `games-trident.pack` still produce
  `0 display-list pixels` at 300M cycles; `nonprog` also stays blank at 600M.
- The remaining failure is later: the loaded Alto code reaches a tight wait
  loop at `BR31=0FD22` (`035000`, `021401`, `101015`, `000775`), with display
  list head `M[0420]=0`. Continue from the post-boot Alto state divergence /
  disk-completion path, not from AEmu `DiskBoot` or EFTP.

#### Update 2026-06-21g: full 815-cylinder Diablo-on-Trident geometry

The next real failure was the first post-boot read/check KCB that addressed the
second emulated Diablo drive:

- `KBLK=143515`, `cmd=044120`, `dsk=010002`, `hdr=143525`, `lbl=143554`,
  `data=144211`.
- The DSK trace reached `ACmmdCheck -> ACheckBadTW -> AChecksumError`.
- The structured controller trace showed the command sought physical
  `chs=409/1/...`, loaded `DiskControl=0254`, but no read stream started.

Root cause: our `DORADO_DISK_DIABLO` geometry was truncated to 206 cylinders.
That is enough for AEmu's drive-0 mapping (`diabloCyl + 3`), but
`AltoDiabloDisk.mc` also maps Diablo drive 1 to `0406 + diabloCyl + 3`. The
pack therefore had no sector at cylinder 409, so `disk_begin_read_stream()`
failed and `ReadFifoTW` never asserted. AEmu correctly reported a checksum/check
failure from the missing transfer.

Fix: `DORADO_DISK_DIABLO` is now the full physical low-level format,
`815 x 5 x 29` with `2/8/256` blocks. Existing 206-cylinder generated packs are
invalid after this change and must be regenerated with `build/dsk2trident`.

#### Update 2026-06-21h: mirrored pack, mixed check/write, restore header check

After moving to full geometry, the converter still populated only emulated
Diablo drive 0. AEmu legitimately issued drive-1 commands, so `dsk2trident`
now mirrors the source image into both emulated drives by default:
`cyl = 406 * drive + diabloCyl + 3`. Use `--single-drive` only for diagnostic
packs.

Two controller details then mattered:

- Mixed read-check/write DiskControl chains must advance from a completed
  check block into the following write block. Without this, AEmu's compare-write
  path overfilled the read FIFO and took the `muffReadError` path.
- `CompareErr` is a pending compare latch, not an immediate permanent
  `ReadDataErr` at block boundary. The muffler reports pending `CompareErr` as
  read error until microcode clears it, but the controller should not pre-latch
  `read_data_err` before the DSK compare tail runs.

The final disk-only failure was a header read-check after ReZero. The KCB disk
address word was `052525`, while the on-disk Diablo header was `052524`; the
low bit is the AEmu restore-request command bit, not media identity. The
controller now tolerates that bit only for word 0 of a Diablo header read-check
on the transfer immediately following a ReZero.

Verification:
- Regenerate a clean pack:
  `./build/dsk2trident --all-heads ../AltoInfo/ContrAlto2-beta/Disks/nonprog.dsk /tmp/nonprog-full-trident.pack`.
- Truth-run with Ethernet fallback disabled:
  `./build/dorado --cycles 300000000 --eb worlds/aemu.eb --disk 0=/tmp/nonprog-full-trident.pack --boot-reason disk --no-alto-boot --out /tmp/dorado-nonprog-fresh-300m.pgm`
  => `2590 display-list pixels`.
- The emulator saves writable packs. Regenerate or copy a fresh pack between
  destructive write experiments; stale `/tmp/*.pack` files can preserve bad
  sectors from prior debugging runs.

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
