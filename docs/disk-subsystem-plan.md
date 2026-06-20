# Disk subsystem — implementation plan

**Status: plan (2026-06-20). Research complete; this is the sequenced build-out.**

The disk subsystem is the linchpin for a *complete* Dorado emulator. Today the
only working disk path is a **PDI shim** in `machine.c` that fakes the Cedar
germ's disk I/O by serving pages straight from a PDI image; the real controller
data path (sequence PROM, framed read/write, ECC, authentic timing) is bypassed
(gaps **F1-F6**). Finishing it unblocks, in order of payoff:

- **Cedar past login** — install a real **system volume** with Othello (needs a
  working *write* path), instead of the read-only PDI boot file.
- **Interlisp-D** — `Lisp.run` (BCPL on the Alto exec) `CreateFile`s and
  demand-pages `LISP.VIRTUALMEM` on an Alto-format Trident pack. The
  Alto->Trident mapping is done in **Alto microcode** (`AltoDiabloDisk.mc`), so
  C only needs a correct Trident **read+write** path — no separate Diablo-format
  C code. (See `docs/new-os-pairs-tasks.md` Task A.)
- **Real Trident boot** as a second boot route alongside Ethernet.
- **Phase-2 (Verilog)** parity — the synthetic disk timing (gap F4/K1) must
  land in C before the RTL port has anything to mirror.

This plan is the *roadmap*; the *reference* is `docs/disk-architecture.md` (HM
§9 in detail: TIOA map, DiskControl, Format RAM, read/write sequence PROMs, Tag
register, FIFO, mufflers, wakeup flow, Fire Code ECC + recovery) and the
schematic numbers in `docs/hardware-specs.md` §HS7. Gap definitions: `docs/
handoff.md` §F (F1-F6).

---

## 1. Current state (what exists)

Phase-1 controller (`dorado/src/disk.c`, `include/disk.h`) is done and
test-covered (`tests/test_disk.c`):

- **Trident pack** read/write in the ContrAlto/Bitsavers layout: T-80
  (815×5×9) / T-300 (815×19×9), 2074 B/sector = header(2w)+label(10w)+
  data(1024w)+dummy(1w), CHS order. Load/save/create/sector-accessor all work.
- **Drive** model: per-drive CHS head position, online/ready/select, seek/index
  flags, subsector divider.
- **Controller slow-IO** on TIOA 10-14₈ (DSK task 14₈): DiskControl bit decode,
  DiskTag strobe decode, DiskRam (Format RAM, auto-increment), DiskMuff status
  select, DiskData FIFO push/pop, the 16-word FIFO, and the wakeup flip-flops
  (IndexTW/SectorTW/TagTW/RdFifoTW/WrFifoTW).
- **PDI** loader (`pdi.c`) for the Pilot/Cedar logical-volume image.
- **Fast-IO transport** (`fastio.c`): pack→FIFO→IOStore(DSK)→main memory works
  synchronously (one munch per call). **Note (HM §9.3/§9.8):** the *real* data
  path is the disk task doing word-at-a-time `Pd←Input`/`Output←B` on **DiskData
  (slow IO)**, paced by the FIFO threshold wakeups — there is no Fin/Fout
  fast-IO on the Dorado disk. The current munch is a shortcut to reconcile with
  the real FIFO loop in D2.

**What's faked / missing (the gaps):**

- **PDI shim (`machine.c`)** — the *only* reason Cedar boots. It watches the
  germ build a `DiskHeadDorado` SA4000 IOCB at fixed VAs, then completes it by
  copying PDI pages directly into VM. The sequence PROM, framed FIFO stream, and
  status path are never exercised. **This shim is the thing the plan retires.**
- **F1** — read sequence-PROM not executed; `refill_fifo` dumps
  header+label+data contiguously (no preamble/sync/data/ECC/postamble framing).
- **F2** — Fire Code ECC absent.
- **F3** — write side is a stub (sets `WrFifoTW`, commits nothing to the pack).
- **F4** — sector/index timing is synthetic (`advance_sector` poked from probe
  code, not driven from the cycle clock).
- **F5** — status readout partial (KSTATE subset; KSTAT/KRAM/KTAG/KFIFO and the
  error-summary/threshold bits return 0).
- **F6** — command start / read-FIFO path: DiskTag decode is in place but the
  `KCmmdInTime`/`DoDiskBlock` leading-edge command start + RdFifoTW threshold
  behavior isn't wired.

What's reusable from ContrAlto (already ported ✅, or to guide the port ⚠️): pack
I/O ✅, sector struct ✅, drive basics ✅, FIFO ✅; seek-time model
(`6ms + 0.602×cylDistance`) ⚠️; read/write state machines (`TridentController.cs`
`ReadWordCallback`/`ProcessDiskWrite`) ⚠️ guide the sequence-PROM port. Do **not**
port `AltoDiabloDisk.mc` / `DiabloDrive.cs` — those are Alto microcode/algorithms,
not the Dorado controller.

---

## 2. Design principles

1. **Model the controller, not the OS.** Implement the sequence-PROM /
   Format-RAM / FIFO / ECC hardware faithfully; let real microcode drive it.
   This is the same philosophy that fixed the microengine (CLAUDE.md).
2. **Keep the PDI shim until the real path passes the same boot.** Gate the
   real controller behind a flag, prove it boots Cedar identically, *then*
   retire the shim. No flag-day.
3. **Read before write.** The read path is the boot-relevant minimum and the
   easier half; land it first, validated against the PDI/Trident data we can
   already check.
4. **Timing model shared with the rest of the engine.** Drive sector/index
   pulses from `bb->cycles`, not a poke — this is also a Phase-2 prerequisite.
5. **Test against real microcode + round-trips,** not synthetic dumps.

---

## 3. Phased plan

Each phase is independently committable and has an explicit validation **gate**.
Ordering favors the boot-relevant minimum first, then write, then ECC/recovery.

### Phase D0 — Test scaffolding + format-RAM/tag decode completion (F6 prep)
**Goal:** make the controller observable and finish the command front-end so a
sequence can be *started* correctly.
- Add a `disk_seq` trace (env-gated, via `dorado_trace_flag`) dumping
  PC/step/tag/FIFO/word-count each controller tick.
- Complete **Tag** decode + timing for all four tag types (Drive-Select w/
  subsector divider, Head-Select, Cylinder-Seek w/ NotReady-until-index,
  Control w/ Wr/Rd enable, AltoLeader, strobe early/late). Set `TagTW` with the
  documented 200ns/1.2µs cadence.
- Wire **DiskControl** leading-edge command start (`KCmmdInTime`): latch the
  4×2-bit block op codes and the Active flip-flop on the next sector pulse (F6).
- Files: `disk.c`, `disk.h`, `tests/test_disk.c`.
- **Gate:** unit tests assert tag fields decode to the right CHS/op and that a
  control write arms the right block-op sequence; `make test` green.

### Phase D1 — Real sector/index timing model (F4)
**Goal:** the DSK task wakes on its own from a clock-driven pulse model.
- Add a per-drive timing model: 3600 RPM (16.67 ms/rev), 117 subsector pulses,
  subsector divider → sector pulses; index pulse once/rev; widths per HS7
  (sector ≈1.2µs, index ≈4.5µs). Advance from `bb->cycles` in the machine step,
  replacing `dorado_disk_controller_advance_sector` pokes.
- Seek model: `6ms + 0.602×cylDistance` (ContrAlto), NotReady during seek,
  TagTW for a seek deferred to the next index.
- Generate IndexTW/SectorTW wakeups → DSK task; honor BlockTillIndex.
- **Wakeup-clearance subtlety (HM §7.3):** disk wakeups use TIOA-based
  clearance — the wakeup drops at T0 of the *2nd* instruction after TIOA matches
  a disk address; the task must not Block until the 3rd instruction or it gets a
  spurious second wakeup. Model this so status-poll loops don't double-wake.
- Files: `disk.c`, `disk.h`, `machine.c` (call the advance from the run loop).
- **Gate:** with no probe pokes, a spinning drive produces the right
  sector/index cadence; `WaitForSector` microcode advances; existing tests pass
  (convert the synthetic test to drive the clock instead of poking).

### Phase D2 — Read sequence PROM + framed FIFO stream (F1, F5-partial, F6)
**Goal:** a real Trident read streams correctly framed data into memory.
- Implement the **read sequence PROM** as a small stepper (the 31-step table in
  `disk-architecture.md` §"Read Sequence PROM"): head-select → settle delay →
  preamble skip → issue-read → [stall until sync] → data words (per Format-RAM
  count) → ECC words → ECC remainder → next block → zero-tag. Use the Format RAM
  word counts (RAM[0..15]) exactly.
- **FIFO** gets the 20-bit cell (16 data + 2 byte-parity + 2 type) and the
  **ReadTW** threshold (≥3 words normal, ≥1 read-and-check); Over/Underflow FFs.
  Model the TW-drop cadence (HM §9.3): RdFifoTW drops at T0 of the 4th cycle
  after an `Input` that takes the count below 3 (read) / 2nd cycle (check);
  WrFifoTW drops 4th cycle after an `Output` that takes free-space below 4.
- Switch the data path from the fast-IO munch to the real **DiskData slow-IO
  FIFO loop**: the disk task `Input`s/`Output`s words paced by the TWs above.
- `refill_fifo` emits **framed** data (sync discarded, N data, checksum/ECC) per
  block with the Done terminator — not the contiguous dump.
- Files: `disk.c`, `disk.h`.
- **Gate:** read a known Trident sector via the real path and verify the bytes
  match `dorado_disk_pack_sector`; FIFO thresholds/wakeups behave; the
  read-and-check op compares correctly.

### Phase D3 — Fire Code ECC generate + check (F2)
**Goal:** ECC words are produced/verified so status reflects data integrity.
- Implement the 32-stage LFSR: write `P(X)=X^32+X^23+X^21+X^11+X^2+1`; read
  split `X^11+X^2+1`→ECC[0:10] and `X^21+1`→ECC[11:31] (taps in HS7 /
  `disk-architecture.md` §"Fire Code ECC"). Generate on read/write, check on
  read → set `ReadDataErr`. Sync word `0o201` standard / `0o001` Alto-Trident.
- **Recovery** (Chinese-Remainder single-burst correction) only if microcode
  actually faults on an injected error; generate-and-check is the minimum.
- Files: `disk.c`, `disk.h` (new `disk_ecc.c` optional), `tests/test_disk.c`.
- **Gate:** ECC of a known sector is stable across read/write round-trip; a
  deliberately corrupted sector sets `ReadDataErr`; (optional) recovery fixes a
  single burst.

### Phase D4 — Full status readout (F5) + boot Cedar through the real path
**Goal:** status-polling microcode reads real values; Cedar boots with the shim
**off**.
- Decode the full muffler map: KSTATE 000-017, **KSTAT 020-037** (Seek
  Incomplete/HeadOvfl/DevCheck/NotSelected/NotOnLine/NotReady/SectorOvfl/Fifo
  Under-Overflow/ReadDataErr/ReadOnly/CylOffset/IOBParityErr/FifoParityErr/
  Write-ReadErr summary), KRAM 040-057, KTAG 060-077, KFIFO 100-117.
- Provide a **Trident pack built from the PDI** (or mount the PDI behind the
  Trident geometry) so the same Cedar boot can run through the real controller.
- Add a `--disk-real` gate (default keeps the shim) to A/B the two paths.
- Files: `disk.c`, `machine.c`.
- **Gate (milestone):** Cedar 6.1 boots to the login prompt via the **real
  controller** (shim disabled), pixel-identical to the shim path. Then make the
  real path the default and remove the shim.

### Phase D5 — Write path (F3)
**Goal:** sectors can be written back; an OS can format/write a volume.
- Implement the **write sequence PROM** (the write step table in
  `disk-architecture.md` §"Write Sequence PROM"): preamble/sync write, data from
  the FIFO, ECC words appended, header-compare before data write, write-gate.
  Commit the FIFO to `dorado_disk_pack` (honor `read_only`), mark sectors
  modified, `dorado_disk_pack_save` on flush.
- WriteTW threshold (≥4 free words); OutputLate handling.
- Files: `disk.c`, `disk.h`.
- **Gate:** write-then-read round-trip of a sector matches; a scratch Trident
  pack survives save/reload.

### Phase D6 — Volume install + Lisp pack (integration)
**Goal:** turn the write path into the two payoffs.
- **Othello / Cedar system volume:** run the Pilot volume utility against a
  writable Trident pack; reach Cedar past "No System Volume."
- **Interlisp-D pack:** boot the Alto exec, run `Lisp.run` to `CreateFile
  LISP.VIRTUALMEM` and copy the world onto an Alto-format Trident pack (the
  Alto/Diablo→Trident mapping is microcode; the C controller just serves Trident
  read/write). Then demand-page Lisp. (See `docs/new-os-pairs-tasks.md` Task A.)
- Files: `machine.c`, `Makefile` (`make run-lisp`, a writable-pack flow), docs.
- **Gate:** Othello writes a volume that re-mounts; `Lisp.run` populates
  `LISP.VIRTUALMEM` and Interlisp-D starts demand-paging.

### Phase D7 — Frontend + format integration (cleanup)
- Settle the disk image formats the emulator accepts: **Trident pack** (primary,
  R/W), **PDI** (Cedar boot volume), and the writable Trident pack used by
  Othello/Lisp. No raw Alto/Diablo C format needed (microcode path).
- CLI: `--disk SLOT=PATH`, writable flag, `make run-*` targets; WASM stays
  Ethernet/PDI-boot only (no large packs preloaded).
- Update `docs/running-the-emulator.md`, `disk-architecture.md` "what we have",
  retire the shim notes in `handoff.md`.

---

## 3a. Progress log

- **D0 (done, committed):** `DORADO_DISK_SEQ` structured trace + cached
  trace-flag; command front-end confirmed complete.
- **D1 (done, committed):** clock-driven sector/index timing
  (`dorado_disk_controller_tick`, 3600 RPM) + DSK wakeup, self-guarded to a real
  pack / `--disk-real` PDI. Removes reliance on synthetic `advance_sector`.
- **D3 (done, committed, ahead of D2):** Fire Code ECC generate/check
  (`dorado_disk_ecc_*`). Self-contained; consumed later by read framing + write.
- **D4 (in progress):** `--disk-real` flag (off by default) disables the
  IOCB-level PDI shim, clock-drives the PDI, and lets the DSK task run
  PilotDisk.mc against the real controller. **Confirmed:** PilotDisk *does* drive
  the controller (drive-select/head/cyl/control tags, STARTF + block-till-index
  control loads, sector/index wakeups). **Blocker:** it loops in
  `SetDriveAndSubSector`/`KIdleLoop` ("Don't know current cylinder ... block til
  index and try again"), polling only IndexTW/SectorTW, and never issues a read.
  It configures the cadence to **spr=29 (subsector count 3)**, so the pulse
  cadence matches; the stall is in establishing drive/cylinder state for the
  **SA4000** PDI volume on the real path. The remaining real-path work:
  (a) the SA4000 drive-ready / current-cylinder establishment PilotDisk's idle
  loop waits on; (b) the CHS->Pilot-page mapping for served data (the shim used
  linear disk addresses and sidestepped this); (c) the framed read (D2) so
  DoDiskBlock reads data+ECC correctly; (d) the status path (D4 proper). This is
  the project's long-standing disk blocker and needs PilotDisk.mc-level
  iteration. The shim remains the default working Cedar boot throughout.

### D4 deep dive (2026-06-20): the real-path stall is an IOCB-completion deadlock, NOT the codeLink bug

Added a DSK-task microcode PC trace (`DORADO_DSK_PC_TRACE`). Under `--disk-real`
the DSK task **gets past** the BootChannelDisk codeLink bug (§1a was the older
netboot frontier; the 2026-06-18 IFUJump-StkP / Carry20 fixes moved past it) and
issues real disk I/O. The precise stall:

- The DSK task spins ~45M iterations in a 2-instruction loop **pc=0o7003 <-> 0o7012**
  (entered via 7023->7043->7012), with **T=0o14, Q=0o7065, md=0o125377,
  lva=0o2000006, MemBase=0o34**. `md=0o125377` is the disk **IOCB seal value**
  (`IOCBSealValue`): the loop polls the IOCB seal at VA 0o2000006 and waits for
  it to clear (the shim used to zero it on completion).
- An earlier/minor loop (~625 iters, **MemBase=0o31**) reads **CSB.next at
  0o177520 = 0** -- i.e. PilotDisk's IOCB processor sees *no linked IOCB* in the
  CSB chain.

So the boot posts an IOCB at ~0o2000000 (MemBase 0o34) and waits on its seal,
but it is not linked into the CSB.next chain (0o177520) that the disk-task
processor polls -> completion deadlock. The disk controller itself is fine
(PilotDisk drives it; the stall is above the controller, in the IOCB/CSB
hand-off). Next step to pursue: map the real PCs (7003/7012/7013/6650/7401..)
to PilotDisk.mc / DiskBootSoft.mc routines via the Cedar world's symbol table,
and determine why the posted IOCB isn't on the CSB chain the processor reads
(germ/boot IOCB-linking, or a MemBase-relative CSB the emulator resolves
differently). This is microcode-level archaeology, well-isolated but open-ended.

### D4 root cause (2026-06-20): CSB/IOCB memory-visibility, not the controller

Added `mbdis -r ADDR[,ADDR...]` (real-IM-address -> nearest source label via the
loader's image_to_real). Validated: the loaded `CedarDorado.eb!6` placement
matches `Cedar.mb!6` (exact-zero offsets on disk-boot labels). It maps the
DSK-task (task 14₈) spin precisely:

- The dominant ~45M-iteration loop at real **0o7002/0o7003 = `BootTransferTimeout`**
  and **0o7011-0o7013 = `BootLabelError`** -- both in `DiskBootTransfer.mc`.
- `BootTransferLp` posts the boot IOCB with `csb.next <- iocb; iocb.seal <-
  IOCBSealValue`, then waits up to ~2s (RTC430) for the seal to clear; on
  timeout it falls to `BootTransferTimeout` ("disk probably not on-line") and
  retries forever.

Root cause (trace lva/MemBase): the poster/seal-wait runs at **MemBase 0o34**
(reads iocb.seal at `lva=0o2000006`, IOCB up at ~0o2000000), while the disk
IOCB processor's idle loop reads **`CSB.next` at `lva=0o177520` via MemBase 0o31
and gets 0**. The IOCB posting is not visible to the processor, so the command
never executes -> timeout -> retry. This is a **CSB/IOCB memory-visibility
problem between the two MemBase paths** (MemBase/long-pointer resolution or
cache coherence), *above* the disk controller -- the controller itself drives
fine (D1) and the boot is past the BootChannelDisk codeLink bug. Next: trace the
physical addresses the `csb.next` write (MemBase 0o34) and read (MemBase 0o31)
resolve to and reconcile them (and the IOCB-at-0o2000000 vs shim-path-0o431
location). This is a memory-subsystem investigation, not controller work.

## 4. Dependencies & ordering notes

- **D1 (timing) before D2 (read)** — the read PROM is clocked by the pulse model.
- **D2/D3 before D4** — Cedar-through-real-path needs framed reads + status.
- **D4 before D5 is recommended** (prove read end-to-end before write), but D5
  is independent code and can start in parallel once D2's FIFO/PROM stepper
  exists.
- **D6 depends on D5** (write) for both Othello and Lisp.
- **Cross-subsystem:** authentic timing (D1) intersects Hold semantics (gaps
  B1/C1) and is a Phase-2 prerequisite (K1). The Ethernet boot route (HS8) stays
  the primary boot path; disk is the second route + the write-capable one.

## 5. Risks & open questions

- **Sequence-PROM step tables:** `disk-architecture.md` has the read/write
  steps from HM §9 p.99; the exact Format-RAM-count semantics per step are the
  main thing to get right. ContrAlto's callbacks are a cross-check, but its
  controller is Alto-shaped — trust HM §9 first.
- **Sync-lock stall:** the read PROM stalls WordClocks until the sync word; the
  emulator must model "wait for sync" deterministically against the pulse model.
- **ECC recovery** may never be needed (real packs read clean); keep it behind
  "only if microcode faults."
- **PDI vs Trident for Cedar:** D4 needs Cedar's volume presented behind Trident
  geometry — confirm the PDI→Trident sector mapping (or build a Trident pack
  from the PDI) so the real controller can serve the same boot.
- **Seek timing realism** vs. test speed: keep seek/rotation latency modeled but
  fast-forwardable for headless tests.

## 6. Validation strategy

- **Unit (`tests/test_disk.c`):** tag decode, Format-RAM load, sequence-PROM
  stepping, FIFO thresholds, ECC generate/check, read/write round-trip.
- **Integration:** Cedar boot through the real controller (D4 gate); Othello
  volume write+remount (D6); Lisp VMEM populate (D6).
- **Regression gate (don't break):** `make test` (11 suites), Galaxian 121553px,
  AEmu NETEXEC band, AltoMesaDorado herald, Cedar login — all must stay green
  through every phase. The PDI shim stays the default until D4's gate passes.

## 7. References

- `docs/disk-architecture.md` — HM §9 in detail (the reference).
- `docs/hardware-specs.md` §HS7 — schematic numbers (PROMs, ECC taps, FIFO,
  timing, mufflers).
- `docs/handoff.md` §F — gap definitions F1-F6.
- `dorado/src/disk.c`, `dorado/include/disk.h` — current controller.
- `dorado/src/machine.c` — the PDI shim to retire.
- ContrAlto `TridentController.cs` / `TridentDrive.cs` / `DiskPack.cs`
  (`AltoInfo/Contralto2-2.0-Beta/`) — porting cross-check (Trident only; not the
  Diablo/Alto code).
- `docs/new-os-pairs-tasks.md` — what the write path unblocks (Lisp, ST volume).
