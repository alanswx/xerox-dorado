# Handoff: continue building the Xerox Dorado emulator

This document is for the next person (or LLM) picking up this project.
Read it first. It tells you the current state, what's runnable, what's
broken, what to work on next, and the gotchas that cost me hours so
you don't repeat them.

## TL;DR

- **What:** A C emulator for the Xerox Dorado (1978–1985 PARC research
  workstation). Goal: run the original Mesa/Cedar/Lisp/Smalltalk/Alto
  emulator microcode against a Trident disk pack and show pixels on a
  framebuffer.
- **Status:** Microengine + memory subsystem + IFU + tasking + slow-IO
  routing + BaseBoard 6502 model are working. Display + Disk + Fast-IO
  transport have Phase-2 stubs that move data end-to-end. The full
  BaseBoard Boot0 path reaches the BB's Continuous loop.
  `probe_full_boot_with_bootstrap` still substitutes canonical
  Initial.MB at BOOTSTAGE2 because Bootstrap streaming does not yet
  match `chm/Initial.mb`. NOSTORAGE no longer needs a probe bypass:
  Config' now uses the bit layout Initial's compiled LSH/LDF code
  consumes (`ChipSize` low bits, module-present bits in bits 4..7).
  For bring-up it reports one present 4K-chip module, enough to map
  the first 64K without spending the probe budget on a full 4MW map
  walk. Initial now gets through PRESETMAP, FINDMODULE, BootMem,
  BootEmulator's first-64K clear loop, display/disk init, and the
  100 ms RTClock wait after MapBufBusy, Pipe5, Config, ALU one-bit
  shift, memory-ref FF branch, `Store←T` A/Mar, and Junk timer fixes.
  The full probe mounts `spruce-server.dsk300` when present. Because
  the DDC terminal back-channel is not modeled yet, the probe forces
  boot keys up and redirects the remaining false `GotBootKey` case to
  `DiskHardMicrocodeBoot`. Display scanline wakeups now run the
  AHT/DispM terminal task continuously (`display outs=35414`,
  frame 59 snapshot) and keep the boot keyboard words all-up. Disk
  bring-up now forces PilotDisk's one-time normal-mode branch past the
  disabled loop. A CPU Md-latch timing fix lets DSK read the IOCB seal
  correctly, so the full probe now reaches `KSameDrive`,
  `KContinueCmmd`, and `KCheckSeek`. It still does not reach
  `DoDiskBlock` or DiskData; it falls back through the seek/sector
  status path and then spends the probe budget in `Read1Muff`. The next
  blocker is modeling the disk sector/status/muffler path closely enough
  for `WaitForSector` and the sequence-PROM command timing to proceed.
  The full-boot probe currently uses a temporary first-256-page identity
  map shim at `DiskHardMicrocodeBoot` because Initial's real `PresetMap`
  writes still leave the first-64K map vacant at disk boot time.
  Initial's final `mcr.noWake` load appears as `MCR=0xFEE7`; the
  emulator currently special-cases that as normal references enabled
  with only fault wakeups suppressed until the MCR active-low decode is
  corrected from the source docs/schematics. The AEmu bypass
  probe currently halts at `PC=0o7777`.
- **Repo:** `/Users/alans/Documents/development/Dorado`
- **Most useful entry points to read:** `CLAUDE.md` (project mission),
  `dorado/CLAUDE.md` (code-side guide), `docs/INDEX.md` (doc map).

## Build and run

```sh
cd dorado
make           # builds everything in build/
make test      # runs all test binaries; should print "All <X> tests passed."
make clean     # nuke build/
```

C99, no external deps. The `vendor/6502/` dir contains a 6502
emulator (used for the BaseBoard model). `build/mbdis`, `build/mctrace`,
`build/bbtrace` are diagnostic CLIs; the rest are test binaries.
The Makefile uses compiler-generated `.d` sidecars (`-MMD -MP`), so
header edits under `include/` rebuild the affected objects. Last
focused verification: `make build/test_memory`, `./build/test_memory`,
`make build/test_disk`, `./build/test_disk`, `make build/test_cpu`, and
`./build/test_cpu` passed on 2026-04-30. Full-probe disk facts from
the latest run: `KSameDrive@0o6553`, `KContinueCmmd@0o6572`, and
`KCheckSeek@0o6561` are hit; DiskData/FIFO counts remain zero.

## Read these first (in order)

1. **Hardware Manual:** `DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`
   — the canonical reference. Cite section + page numbers in commits and code.
2. **Booting memo:** `DoradoDocs/manuals/Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf`
   — explains the boot sequence at a high level. Path A is BB → Bootstrap
   → Initial → Ethernet; Path B is LoadMB (used by `probe_aemu`).
3. **Project mission:** `CLAUDE.md` (top of repo). Describes the two-phase
   plan (C emulator → Verilog) and working norms.
4. **Code-side guide:** `dorado/CLAUDE.md`. What's built, conventions,
   format findings (the .MB bit shuffle, ALUFM extraction).
5. **The phased plan:** `docs/boot-bringup-plan.md`. Distilled state
   of everything; updated each session.
6. **I/O reference:** `docs/io-systems-architecture.md` (overview),
   `docs/disk-architecture.md` (HM §9 detailed), `docs/display-architecture.md`
   (HM §11 detailed), `docs/memory-architecture.md` (HM §5 detailed).
7. **JCN encoding:** `docs/jcn-encoding.md` — the 7-way JCN field split.

## Repo layout

```
Dorado/
├── CLAUDE.md                       project mission + norms
├── DoradoDocs/manuals/             ★ Hardware Manual + booting memo
├── chm/dorado/                     ★ Dorado microcode + .MB sources from CHM
│   ├── Mesa.mb!3 / Cedar.mb!6      emulator microcodes
│   ├── AEmu.mb!2                   Alto emulator on Dorado
│   ├── doradobaserom.mb!13         BaseBoard EPROM .MB
│   └── expanded/
│       ├── BootstrapSources.dm/    Bootstrap.mc + BootDefs.mc source (BCPL)
│       ├── bootstrap.dm!20_/       Bootstrap.mb + Initial.mb
│       ├── doradobaserom.dm!12_/   BaseBoard 6502 source (.masm)
│       ├── kernel.dm!38_/          shared microcode helpers
│       ├── memMisc.dm!11_/         memory primitives
│       ├── Ifu.dm!51_/             IfuComplex.mb
│       └── ftest.dm!1_/            full Micro→MicroD→.MB worked example
├── AltoInfo/                       Alto/ContrAlto2 references (gitignored)
│   └── Contralto2-2.0-Beta/        ★ TridentDrive.cs etc — port reference
├── docs/                           project documentation (read INDEX.md)
└── dorado/                         ★ the C emulator
    ├── Makefile
    ├── include/                    public headers
    ├── src/                        implementation
    ├── tests/                      test_*.c
    └── vendor/6502/                fake6502 (6502 emulator for BB)
```

## What's built (verified by passing tests)

These all compile clean and pass:

### Microengine (`include/cpu.h`, `src/cpu.c`)
- Full microinstruction decoder (RSTK / ALUF / BSEL / LC / ASEL /
  BLOCK / FF / JCN). Verified against ~25K µinstrs.
- ALU with all 16 ALUFM ops + arithmetic-vs-logical distinction
  (Carry'/Overflow only updated on arithmetic ops, per HM page 30).
- Full JCN: Local Jump/Call, Global Call, Long Jump/Call, Conditional
  (cond 0..6 + Overflow), Subroutine Return, IFU Jump. Regression:
  Initial's `0o7557` long branch (`FF=0o304`, `JCN=0o002`) now targets
  `0o6102`; the previous reversed packing sent it to `0o1310`.
- Shifter (ShC- and FF-controlled, all four mask ops). Bootstrap's
  compact `LDF[T,3,10]` form is handled as a focused special case; it
  extracts the BaseBoard dispatch bits and returns the spread
  BigBDispatch offset.
- Alt-source memory refs (`Store←{Md,Id,Q,T}` /
  `Fetch←{Md,Id,Q,T}`) and LC `Md` destinations (`T←Md`,
  `RM/STK←Md`) are wired. A focused regression covers
  `Fetch←T + RM/STK←Md`.
- FF dispatcher: large subset (TaskingOff/On, Wakeup[task], B←Pipe0..5,
  Pd←ALUFMRW, IFUMLH/RH, PCF←B, IFUReset, BrkIns, etc.).
- Tasking: 16 priority-scheduled tasks, T/TPC/MemBase/Link replicated,
  BLOCK clears wakeup, FreezeBC, Reschedule trap, SubTask OR into
  RBase[2:3]/MemBase[2:3].
- IFU: IFUM 1024×24-bit, prefetch+pipeline, IFUJump dispatch, 4 entry-
  vector slots, NotReady trap, conditional IFUJump.

### Memory (`include/memory.h`, `src/memory.c`)
- 4MW main storage, cache 4×64×16, Map (16K entries × 256-word pages),
  16-entry Pipe, 32-entry BR (28-bit each).
- Refs: Fetch / Store / IFetch / PreFetch / LongFetch / IOFetch /
  IOStore / Map / Flush / DummyRef.
- Faults: page (vacant), write-protect, map-trouble. FaultInfo register.
- ProcSRN / ASRN pipe slot allocation.
- Memory Control Register state for `LoadMcr[A,B]`: DisBR blocks BR
  writes and BR-based virtual addressing, NoRef suppresses storage
  references, FDMiss forces fill/miss behavior, and NoWake suppresses
  fault wakeups.
- MapBufBusy/Pipe5 timing for `Map<-`: per HM §5 and MEMX/MEMC
  schematics, `Map<-` now marks the addressed pipe slot busy for 9
  cycles and `B<-Pipe5` exposes that busy state in the sign bit for
  Initial's `WAITFORMAPBUF` loop.
- `ReadMap` and `Map<-` now share the same 256-word-page map index
  helper, avoiding stale reads from the old `va >> 6` path.
- **No Hold semantics** — refs are atomic. Md is delivered immediately.
  This is a known gap; AEmu's IFU dispatch loop appears to need real
  Hold to stall properly.

### BaseBoard (`include/baseboard.h`, `src/baseboard.c`)
- 6502 + 5 RIOT chips at 0x400/0x480/0x500/0x580/0x600.
- Loads `chm/dorado/doradobaserom.mb!13` as the 64K ROM image.
- Cold boot through CoolBoot → RebootDorado → LoadDoradoCode →
  Continuous (steady-state).
- MCPBus strobes (Control / Clock / ABMux0 / ABMux1 / MIR0..3).
- CPReg ABMux strobes match the BB stream: ABMux1 latches CPRegL,
  ABMux0 latches CPRegH. The old ABMux1 SetSS-as-AMSync shortcut was
  removed; Bootstrap's ReadBB synchronizes on CPRegH after ABMux0.
- MIR injection + SetSS single-step, Run/Halt gate.
- Analog comparators (in-spec voltage/current) so RebootDorado advances
  through SuppliesAllUp.

### Slow I/O routing (`include/io.h`, `src/io.c`)
- Per-(task, TIOA) device callback table. Pd←Input / Pd←InputNoPE /
  Output←B in cpu.c dispatch through it.
- Floating-bus default: 0xFFFF + bad-parity flag (matches HM §7
  page 86 "IOB has bad parity if a nonexistent register is selected").

### Display (`include/display.h`, `src/display.c`)
- 808×606 mono framebuffer, MSB-leftmost packing.
- DDC slow-IO catch-all on tasks DHT/AHT/AWT/DWT.
- State buckets: per-channel NLCB/CLCB (16×12-bit, A and B), HRam
  (1024×3-bit), Mixer (1024×24-bit), PixelClk, Statics.
- Per-channel FIFO (256 words) for IOFetch← munch delivery.
- `dorado_display_render_fifo()` drains FIFO into framebuffer
  (1-bit-per-pixel, MSB=leftmost).
- PGM snapshot helper (`dorado_display_snapshot_pgm`).
- Headless keyboard state is held as five complemented Alto-style
  words (`1 = key up`, `0 = key down`) so SDL can later feed the same
  core API that tests use today.
- Frame count is a core display value advanced at vblank, not by GUI
  repaint. The framebuffer remains available for mid-frame GUI viewing
  and for headless snapshots such as "dump frame 100".

### Disk (`include/disk.h`, `src/disk.c`)
- Trident T-80 / T-300 pack format (ContrAlto2/Bitsavers byte layout).
  Create / load / save / sector access.
- Drive struct: per-drive online/select state, current cyl/head/sec,
  seek-in-progress, index-pulse latches.
- Controller registered on task 14₈, TIOA 10₈-14₈:
  - DiskControl bit decode (HM page 97).
  - Format RAM 16×12 with auto-increment + EnableRun on last word.
  - DiskData 16-word FIFO push/pop.
  - DiskTag dispatch (HM pages 99-101): Drive Select / Head /
    Cylinder / Control with Read/Write/ReZero/HeadAdvance.
  - DiskMuff status readout (KSTATE / KSTAT subset).
- Synthetic `dorado_disk_controller_advance_sector()` helper for
  sector-pulse simulation.

### Fast I/O (`include/fastio.h`, `src/fastio.c`)
- `fast_io_cb` on `dorado_memory` fires on IOFetch / IOStore.
- `dorado_fastio_dispatch` routes by task: DWT → display FIFO,
  DSK → disk controller FIFO. Synchronous transport (one munch per
  ref, no cycle-accurate timing).
- End-to-end test (`test_fastio.c`):
  - disk pack → FIFO → IOStore(DSK) → main memory ✅
  - main memory → IOFetch(DWT) → display FIFO → framebuffer ✅

## What works as a **probe** (informational, not pass/fail)

Probes live in `tests/test_cpu.c::main()`. They run real microcode
and report what happens. None of them currently boot a complete
system, but they show how far the model gets:

- **`probe_bootstrap_pure`** — load Bootstrap.MB into IM, no BB, run
  from BOOTSTRAP. Walks 16 init instructions, hits READBB, enters
  spin loop waiting for CPReg. With `cpu.cpreg=0x8000` (pre-set
  AMSync), exits the spin once and proceeds through 0o7746 → 0o7715
  → 0o7702 → re-enters READBB. Demonstrates the bootstrap loader is
  executing real microcode.
- **`probe_initial`** — load Initial.MB + Bootstrap.MB layered, run
  from INITIAL. Initial's first instruction at 0o7500 globally calls
  0o7700 (= READBB in Bootstrap region). Both Initial and Bootstrap
  share IM by design; both depend on the BB CPReg protocol.
- **`probe_full_boot`** — full BB cold-boot + Boot0 jam + Dorado
  free-run from IM. This now reaches `LoadDoradoCode` and the BB's
  `Continuous` loop. The run logged 475 injected Boot0 instructions,
  about 13.3M IM-fetched Dorado cycles, and 64 IM entries written
  starting at 0o7700. The important fixes were correct Write IM
  right-half layout, one-cycle dispatch OR, explicit Link-write
  precedence, and a narrow shifter-source fix for Bootstrap's
  `LDF[T,3,10]` form.
- **`probe_full_boot_with_bootstrap`** — BB drives Bootstrap.MB
  (substituted into IM at first IM-fetch) with its real Boot1 byte
  stream. Bootstrap now runs the complete CPReg stream: 7144 CPReg
  changes after swap, 1792 Write IM half-writes, and 896 unique Initial
  targets beginning at 0o6100. The long-branch fix lets Initial get past
  the old `0o1310` no-code stop and run into its configuration path.
  After the memory-ref FF branch, `Store←T` A/Mar, and Junk timer
  fixes, the probe gets through display/disk initialization and the
  100 ms RTClock wait. It mounts `spruce-server.dsk300` if present,
  attempts `DiskHardMicrocodeBoot`, and falls through to Ethernet.
  Current state: `display outs=3`, `disk outs=32`, disk sector
  wakeups generated, no disk FIFO reads/writes yet, EMU waiting near
  `AWAITETHERBOOTREPLY` (`0o6057`) while DSK can be sampled in its
  idle loop (`0o6775`).
- **`probe_aemu`** — layered load Initial + kernel + memMisc +
  IfuComplex + AEmu, run from STARTEMULATOR. After the long-branch
  packing fix, this bypass now halts after 203 cycles at `PC=0o7777`
  via `JNKINITPC`. It likely needs Bootstrap/Initial support code or
  real main-memory state that the bypass does not currently plant.

## What's NOT working (the actual bring-up gaps)

Listed in priority order. These are the next concrete tasks.

### 1. AEmu bypass needs refreshed setup

`probe_aemu` used to cycle in LRTYPETABLE/LRTYPEIM, but after the
long-branch packing fix it halts earlier at `PC=0o7777` via
`JNKINITPC`. The bypass probe is no longer the best immediate path:
it has blank main-memory tables and does not currently layer or emulate
all support code that real Initial/Bootstrap would provide. Two paths
to make it useful again:

- **Plant valid Mesa state.** The structure of the frame, MDS
  (Memory Descriptor System), context info, etc. would need to be
  extracted from Mesa documentation. Then the probe pre-loads memory
  with that state. This requires reading Pilot/Mesa documentation
  in `chm/dorado/expanded/` to figure out the layout.
- **Implement Hold semantics.** In our model, Md returns immediately
  with stored data (0). On real hardware, when microcode references
  Md before the read completes, the engine *holds*. AEmu's loop
  uses Hold to wait; without it, the engine plows ahead with stale
  Md and loops forever. This means our memory subsystem needs a
  cycle counter and a `pending_md` queue. See HM §5 / Figure 9.

### 2. Full BB→Bootstrap→Initial path now runs the boot loop (with workarounds)

**Latest status (2026-04-29 late):** With canonical-Initial
substitution at BOOTSTAGE2 plus kernel.mb/memMisc.mb/IfuComplex.mb
layered under Initial.mb to provide fault-task and helper microcode,
Initial runs through a full setup sequence:

1. INITIAL (0o7500) → READBB for checksum (0o7700) → INITIAL1 (0o7501)
2. WRITEALUF table init (writes 16 ALUFM entries via WRITEALUFTABLE)
3. RMINITL (0o6102) — RM/STK init iteration (multiple passes)
4. IFUMINITL (0o6145) — IFUM init
5. PRESETMAP / RESETMAPL / WRITEMAP / WAITFORMAPBUF — Map init
6. CLRCACHEFCOLL — cache flush
7. SETBRFORPAGE — BR setup
8. NOSTORAGE test passes via real `B←Config'` (one 4MW storage module)
9. FINDMODULE (0o6357) advances after FA=2/FB=7 ALU shift support
10. Initial reaches and exits the BootMem wait loop at `0o6116`
11. BootEmulator clears the first 64K at `0o6226`
12. Display init runs (`DisplayInitConfig`, `THTInitPC`, `THTInit1`)
    and starts DDC slow-I/O outputs
13. Disk/Junk task setup runs; the Junk timer advances RTClock and
    exits the old post-init wait around `0o6205`
14. Initial attempts `DiskHardMicrocodeBoot`, then falls through to
    Ethernet boot wait because the hard-disk boot path has not yet
    produced a successful transfer

State at end:
- 80M run with `spruce-server.dsk300`: EMU is waiting near
  `AWAITETHERBOOTREPLY` (`0o6057`); with DSK wakeups active the
  sampled final PC is often DSK idle (`0o6775`)
- display outs=3, disk outs=32, disk sector wakeups generated,
  disk FIFO reads/writes=0
- `tasking_on=1`; `wakeup_pending` may include DSK when sampled
- Memory: faults=15, `first_srn=0`, `Mar=0xFE21`
- Initial variables: `R400=0x0100`, `RNUM=4`, `REALPAGES=4`,
  `DISPLAYCONFIG=0xFFFF`

**Underlying issues, in priority order to fix:**

#### 2a. Bootstrap streaming corrupts data (still unfixed; bypassed via substitution)

Original investigation showed:

**Of the 896 entries Bootstrap writes, only ~94 match the canonical
Initial.MB. The other ~768 are CORRUPTED.** Specifically the LH writes
(iw0) frequently land as `0x0044` (a near-default decode of
`RSTK=00 ALUF=00 BSEL=RM/STK LC=NoLoad ASEL=A←RM/STK`). The RH writes
(iw1) sometimes land correctly (1 of the 5 dumped addresses matched).

For `IM[0o6347]` specifically:
- LOADED: `iw0=0o000104 iw1=0o017723 iw2=0o040000` → `JCN=0o247(local)` → self-loop to `0o6347`
- CANON:  `iw0=0o051164 iw1=0o017723 iw2=0o100000` → `JCN=0o246(local)` → jump to `0o6346` (`READTERMINALRET`)

iw1 happens to match by coincidence. iw0 differs (default-ish vs real
`RSTK=12 ALUF=11 LC=RM/STK←Pd`), and iw2's `RSTK[0]/JN1bit7` bits are
flipped. The local-jump target offset becomes `0o247` instead of `0o246`,
so the instruction self-loops via the corrupt low bit.

Diagnostic capture (in `test_cpu.c`'s probe with
`cpu.dbg_writeim_log = 1` enabled at swap):

```
#00 pc=0o7720 addr=0o6100 half=LH sec=0 b=0x0044 T=0x0044
#01 pc=0o7724 addr=0o6100 half=RH sec=0 b=0xE682 T=0xE682
#02 pc=0o7722 addr=0o6101 half=LH sec=1 b=0x0044 T=0x0044   ← T=0x0044 again
#06 pc=0o7720 addr=0o6103 half=LH sec=0 b=0x1286 T=0x1286   ← varied data
#14 pc=0o7720 addr=0o6107 half=LH sec=0 b=0x5274 T=0x5274   ← canon iw0 of 0o6347!
```

So canon iw0 of REAL `0o6347` (= `0x5274`) appears in the byte stream
at WRITE position 7 (= addr `0o6107`), not at position 167 (= addr
`0o6347`). This rules out "BB streams in real-address order".

Hypotheses for the root cause:

1. **Bootstrap's `LSH[T,10]` + `LDF[T,10,0]` + `T XOR Byte1` T-composition
   misexecutes** in our cpu.c shifter. The Dorado does this via the
   barrel shifter and ALUF=`A XOR B`. The right answer would be the
   16-bit data byte for that real address; we get something off.
   Investigate by stepping through one full BootByteL iteration and
   comparing T bit-by-bit against expected.
2. **The BB ROM Boot1Data is not in real-address-sequential order.**
   `chm/dorado/expanded/bootstrap.dm!20_/Initial.mb` may not be the
   same layout the BB ROM has — the BB ROM is a separate build from
   1987. Disassemble Boot1Data from the BB ROM (C000-D7FF) and decode
   manually to verify the byte format.
3. **CPReg-byte ordering between the BB and Bootstrap doesn't match
   the Type-0/Type-1 packet format described in `BootstrapMain.mc`.**
   Our BB pushes `ABMux1` (low byte) first, then `ABMux0` (high byte
   with AMSync). Bootstrap reads `T←~CPReg` once and decodes via
   `LSH[T,10]/LDF[T,10,0]`. If the CPReg layout has the byte data in
   a different position than Bootstrap expects, every T composition
   is shifted/garbled.

The trace shows that the SECOND ReadBB (which provides the dispatch
+ right-half-byte) drives the 3-bit dispatch (`BTemp`) into a
LH-or-RH selector that does seem to alternate correctly (we get pairs
of LH+RH writes per address, with `secondary` varying — not stuck at
sec=0). So the dispatch decode is at least partially right. But
the data-byte composition into T is wrong in the LH writes.

Easiest debugging approach: capture a focused per-cycle trace of the
microengine through ONE full BootByteL iteration (from Cnt-test back
to Cnt-test). Log T before each instruction, the result of LSH[T,10],
LDF[T,3,10], LDF[T,10,0]. Compare to what Bootstrap.MB intends.

To enable the post-swap Write IM log used to find this:
```
cpu.dbg_writeim_log = 0;          // off pre-swap
// at swap:
cpu.dbg_writeim_log = 1;
cpu.dbg_writeim_n = 0;             // reset buffer (256-deep)
```
The struct fields are in `include/cpu.h` near the bottom of
`dorado_cpu`. The Write IM trace in `src/cpu.c` is in the `fn == 7`
arm of next_pc.

Also note: `probe_initial` (which directly loads Initial.MB without
streaming) gets stuck differently — INITIAL at 0o7500 does
`Call[ReadBBLoc=0o7700]`, which spins because no BB is providing
CPReg data. So even with correct microcode placement, INITIAL itself
won't run without a working CPReg byte stream from BB.

#### 2b. NOSTORAGE fixed by Config' storage-present response

Initial computes a value via shifter ops at 0o6041..0o6277, stores in
`RM/STK[RBase*16+8]`, then at 0o6210 tests
`RM/STK[RBase*16+8] AND 0xF000`. If zero, branches to NOSTORAGE
(0o6247).

Fixed: `B←Config'` now comes from `dorado_memory_config_word()` instead
of hard-coded `0xFFFF`. The field layout matches what Initial's
compiled shifter code consumes: `ChipSize` in low bits and M0..M3 in
bits 4..7, so `ModMask_ LSH[ModMask,10]` left-justifies the present
module mask. For bring-up we report `ChipSize=0` (4K chips) and one
present module; that maps the first 64K words quickly while leaving
the larger backing store available for later emulator work. With this,
the full boot probe no longer hits the NOSTORAGE bypass; Initial
reaches `FINDMODULE` naturally.

The old probe-side `0o6247 → 0o6357` bypass remains in
`test_cpu.c`, but it no longer fires in the normal run.

#### 2c. PRESETMAP / WAITFORMAPBUF loop (fixed enough for boot path)

After Config' was implemented with 64K-chip reporting, Initial entered
`FINDMODULE` but spent the budget in map initialization rather than the
old display `LONGWAIT` path. That is why current bring-up reports the
small 4K-chip module. The earlier 60M-cycle hot loop was:
`WRITEMAP(0o6340) → 0o6365 → WAITFORMAPBUF(0o6360) → 0o6245 → 0o6244
→ 0o6366 → WAITFORMAPBUF → 0o6245 → 0o6244 → 0o6367 → DORETURN →
RETN → PRESETMAPE/PRESETMAPL → SETBRFORPAGE → ...`.

After adding MapBufBusy, the first cache-address flag model, switching
to HM Table 16's 16K-entry x 256-word page map geometry, correcting
Pipe5 cache flags to manual bits 8..11, and reporting the smaller
module geometry, the normal 60M probe gets past map setup and into
BootEmulator/display/disk initialization.

`LoadMcr[A,B]` is now real enough to cover the bits Initial appears to
use first (dVA<-Victim, DisBR, DisCF, NoRef, FDMiss, UseMcrV,
NoWake), and `CFlags<-A'` plus the cache-address-section portion of
`B<-Pipe5` are modeled at a basic level, including Victim/NextVictim.
This is no longer the top blocker. The remaining bug was not in
MapBufBusy: Initial's FINDMODULE path uses HM Table 11d ALU one-bit
shifts (`FA=2, FB=7`, especially `Pd←ALU lsh 1` at 0o6357). The old
stub returned the unshifted ALU output, so the module/page scan never
advanced correctly. `cpu.c` now implements rsh/rcy/brsh/arsh/lsh/lcy,
and `test_alu_shift_ff_functions` pins them down.

#### 2d. BootMem / BootEmulator memory-reference loops (fixed enough for display start)

The old park at `LWRETN`/`LONGWAIT` was caused by two processor
decode/modeling gaps:

- `FF=0o363` on a memory reference means Store plus the Table 13
  low-six-bit branch condition `Cnt=0&-1`; it is not the full Table 11
  `Wakeup[3]` side effect.
- `ASEL=2/3` alternate memory references must drive the selected
  source onto A and Mar. `Store←T` was previously using A=0, so
  BootEmulator's first-64K clear loop recomputed `T=1` forever.

With those fixed, the 64K clear loop at `0o6226` runs for 65,537 hits
and exits to display initialization. The later `0o6205` RTClock wait
is also fixed by modeling Junk task timer wakeups. Current top blocker
is now the boot-media path: hard-disk microcode boot is attempted but
does not transfer sector FIFO data, then Initial waits for Ethernet
boot replies.

### 3. Disk Phase 3: real timing + Fire Code ECC + sequence PROMs

For booting an actual Alto OS, Mesa needs the disk to sequence
through real read operations (preamble + sync + data + ECC + post-
amble) per the read PROM (HM page 99). Currently our Read tag
short-circuits and dumps header+label into the FIFO contiguously.

For a basic boot this might be enough — Mesa probably trusts the
Format-RAM-determined timing and our short-circuit might satisfy
its expectations. Try it before implementing the full PROM
sequencer.

The Fire Code ECC (`P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1`)
might also be skippable for boot (Mesa might accept successful-read
status without verifying ECC). Implement only if you observe Mesa
faulting on missing ECC.

### 4. Display Phase 3: pixel clock + waveforms + 7-wire interface

Lower priority unless you're trying to get keyboard/mouse working
(the back channel is on the 7-wire interface). For pure framebuffer
output via DWT, Phase 2 is sufficient.

### 5. Hold semantics in memory (touches #1)

If you take path #1 (plant Mesa state) you may not need Hold. If
you take path B, Hold is essential. See HM §5 for timing.

## Most important pitfalls I hit

In rough order of "hours wasted":

### The ALUFM bit-mapping trap (FIXED, but worth knowing)

The `Pd←ALUFMRW` operation writes a 6-bit ALUFM entry from B per
HM Table 11d: `ALUFMEM ← B.8, B[11:15]`. The manual's bit
positions are **MSB-first**; our C uses LSB-first. The mapping
is **NOT** the obvious one. The correct C code:

```c
uint8_t alufm =
    (uint8_t)((((b >> 7) & 1) << 5) |   /* B[8] manual = B_C[7] → entry top (carry) */
              (((b >> 4) & 1) << 4) |   /* B[11] manual = B_C[4] */
              (((b >> 3) & 1) << 3) |   /* B[12] manual = B_C[3] */
              (((b >> 2) & 1) << 2) |   /* B[13] manual = B_C[2] */
              (((b >> 1) & 1) << 1) |   /* B[14] manual = B_C[1] */
              (((b >> 0) & 1) << 0));   /* B[15] manual = B_C[0] (op LSB) */
```

The wrong (mirrored) version is what I had originally. It made
Bootstrap.MB's runtime ALUFM init silently corrupt the table,
which made a downstream conditional jump land in a trap slot,
which made Boot0 appear to be broken when really it was running
correctly against corrupted state. Test: `test_alufmrw_bit_mapping`
in `tests/test_cpu.c`.

### CPU_QUADRANT_SIZE (FIXED)

`include/cpu.h` had `CPU_QUADRANT_SIZE = 0o4000` (= 2K). Per HM §4.3,
a quadrant is 4K-word = the whole IM today. Was 0o10000 (= 4K).
Wrong size → Global Calls / Long jumps to addresses outside 12-bit
IM range. This fixed one class of fictional-address jumps; later the
long-branch packing fix exposed a separate `probe_aemu` bypass issue at
`PC=0o7777`.

### Long branch packing (FIXED)

HM Figure 6 says Long Jump/Call forms TNIA as
`CIA[2:3] || FF[0:7] || JCN[4:7]`. In our 12-bit IM address that means
`(FF << 4) | (JCN & 0xF)`, not `(JCN << 8) | FF`. The regression is
Initial.MB `0o7557`: `FF=0o304`, `JCN=0o002` must branch to `0o6102`
(`RMINITL`), not `0o1310`.

### B←RWCPReg normally returns ~CPReg, with a current boot-path caveat

Per HM page 31: `B←RWCPReg = Link←B, B←CPReg'`. The prime denotes
inversion. So microcode reading CPReg always sees the complement.
The current cpu.c still does this for the legacy-stub path and while
the BaseBoard is single-stepping/IRTable setup. During Dorado
free-run, it currently exposes the raw BB CPReg stream; that is a
pragmatic boot bring-up compromise that lets Bootstrap receive the
BB stream correctly, but it should be rechecked against the hardware
manual once the `0o6347` Initial loop is understood.

**Several Bootstrap mysteries trace back to this** — when reading the
source, watch for whether the surrounding code accounts for the
inversion.

### Stale incremental builds

Make does NOT always pick up changes when `cpu.h` fields are
added/removed. Symptom: tests fail with bizarre values like
`cycles=1806855336`. **Always `make clean && make` after touching
struct definitions.**

### The BB-loaded Boot0 is NOT Bootstrap.MB

`chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb` is the source
form. The BaseBoard EPROM contains a *newer* compiled-and-packed
version that takes a different code path through the trap
reservations. They share the entry point (0o7740 = BOOTSTRAP) but
the bytes differ. Don't conflate the two.

### Octal in C source

C accepts `0` prefix for octal, NOT `0o`. (I keep typing `0o`
out of habit from Python/Rust.) Use `025` not `0o25`.

### `probe_aemu`'s memory mapping

The probe maps 16 pages identity-RW. AEmu's memory references
then succeed (no faults) but read 0 (unwritten storage). To make
AEmu progress, plant data at the addresses it reads from, OR
implement Hold so the engine stalls instead of looping with stale Md.

## Coding norms

From `CLAUDE.md`:
- C99, no external libs except the vendored 6502 in `vendor/6502/`.
- Octal where the manual uses octal. Microinstruction addresses,
  IM contents, RM values — all octal.
- Symbol names mirror the manual: RSTK, ALUF, BSEL, LC, ASEL, FF,
  JCN, BLOCK, IM, IFUM, ALUFM, RM, STK, BR, MemBase, TPC, Pd, Md, Mar.
- Cite HM section + page numbers in non-obvious code: `// HM §3.7
  Table 9`. Cite MicroD source when touching .MB format.
- No emojis in code or docs.
- **Treat the Hardware Manual as canon.** When sources disagree,
  prefer the September 1981 Hardware Manual unless deliberately
  tracking a later revision.
- **Don't invent behavior.** If a microcode source uses a feature
  you haven't implemented, find the manual passage describing it
  before writing code.

## Test patterns

When fixing a bug you found via a probe, write a *focused* test
that:
1. Sets up minimal microcode that triggers the bug.
2. Verifies the corrected behavior.
3. Goes in the matching `tests/test_*.c` file.
4. Has a clear PASS/FAIL output.

Examples already in the tree:
- `test_alufmrw_bit_mapping` — uses 1 microinstruction to exercise
  the Pd←ALUFMRW mapping.
- `test_carry_preserved_on_logical` — 3 microinstructions verifying
  HM page 30 ("Carry' is the result of the last *arithmetic* op").
- `test_alufm_canonical_decoding` — verifies the standard ALUFM
  convention is recovered from real .MB files.

This way the bug stays fixed even if someone refactors later.

## CHM (Computer History Museum) archives

The richest archive is at `xeroxparcarchive.computerhistory.org`,
specifically `_cd8_/dorado/` and `_cd8_/doradosource/`. URL
convention is in `docs/chm-urls.md`. To grab a source file:

```sh
curl -sO "https://xeroxparcarchive.computerhistory.org/_cd8_/doradosource/BootstrapSources.dm!12_/BootstrapMain.mc"
```

Drop the file in `chm/dorado/expanded/<DirName>/` and refer to it
in code/comments.

The cross-reference is at `chm/cross-reference.html` (568K lines —
grep it; never paginate it). Maps every PARC IFS file to its
archive location.

## Active task list

When you start, skim the tasks via `TaskList`. The incomplete ones
labeled "pending" or "in_progress" are the open work.

Currently active when I left off:
- **#58 in_progress:** BB→Bootstrap→Initial now loads 896 Initial IM
  entries and runs Initial. With canonical Initial substitution, the
  probe now reaches display/disk init, exits the RTClock wait, attempts
  hard-disk boot, and falls through to Ethernet.
- **#45 in_progress:** `probe_full_boot` reaches `LoadDoradoCode` and
  the BB `Continuous` loop. `probe_full_boot_with_bootstrap` is now the
  canonical deeper path for Initial bring-up.
- **#46 pending:** Phase 7 slow-IO subsystem. Mostly DONE in
  practice (slow-IO routing layer + display + disk all wired).
  Could be marked completed.

## Suggested first action for the next session

The probe currently bypasses one issue (Bootstrap streaming
corruption) to let Initial run. NOSTORAGE, PRESETMAP, FINDMODULE,
BootMem, the first-64K clear loop, and the `0o6205` RTClock wait are
no longer the current blockers. The CURRENT BLOCKER is boot media:
Initial reaches `DiskHardMicrocodeBoot`, but the mounted Trident pack
does not yet produce a successful boot transfer, so execution falls to
the unimplemented Ethernet boot path.

### Highest-value: finish the boot-media path

Latest disk bring-up checkpoint:

- `dorado_disk_controller_advance_sector()` now models index wrap:
  index pulses assert both `IndexTW` and `SectorTW`, clear
  `BlockTillIndex`, and non-index sector pulses are masked while
  `BlockTillIndex` is set. `test_block_till_index` covers this.
- The full-boot probe advances the synthetic spindle on every service
  call while `BlockTillIndex` or a seek is pending. This keeps the
  controller behavior close to HM page 97 while avoiding probe-only
  boot-transfer timeouts caused by an arbitrary fake spindle rate.
- With disk tracing enabled, Initial again reaches `KSameDrive`,
  `KContinueCmmd`, `KCheckSeek`, `KWAITSECTOR`, and `WAITFORSECTOR`
  before the first disk boot timeout. It still does not reach
  `DoDiskBlock` or read `DiskData`; `InitialDisk.mc` says the hard disk
  microcode file is a private Dorado convention starting at page 4
  (cylinder 0/head 0/sector 4), while the mounted `spruce-server.dsk300`
  is an Alto Spruce T-300 pack and is not known to contain that file.
  Treat Spruce as a controller validation pack, not as a required
  successful Initial hard-disk boot source.
- Current disk model details to preserve: KSTATE block-mode muffler
  signals `RdOnlyBlock'`, `WriteBlock'`, and `CheckBlock'` are
  active-low; Cylinder Tag/ReZero now hold `NotReady` and delay `TagTW`
  until the synthetic sector/index cadence reaches index; subsector
  count uses the HM examples' floor division (`117 / (count+1)`, so
  count 3 -> 29 sector pulses/rev). A one-hot Tag[0:3] attempt was rejected because
  Initial's observed `0xFFEF` tag value behaves as preload/idle, not all
  commands at once; the C model still decodes command values from the
  high nibble `0..3` and ignores other high nibbles.

1. Use the `probe_full_boot_with_bootstrap` boot-landmark and per-TIOA
   disk counters to find exactly where `BootTransfer` fails.
2. If the failure is real pack contents, stop spending time on
   `spruce-server.dsk300` as an Initial hard-microcode source; it is
   an Alto Spruce pack and likely lacks the private Dorado hard
   microcode file at page 4.
3. Implement enough 3 Mb Ethernet boot support, or a controlled
   Initial Ethernet packet injector, to deliver `chm/microcode/*.eb`
   files and let `EtherMicrocodeBoot` load emulator microcode. A gated
   probe injector now exists: run
   `DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
   ./build/test_cpu`. It skips the first 512-byte EB overhead page,
   copies the zero-sum payload into Initial's `BootDataPtr` area, and
   lets `CheckChecksumAndLoad`/`LoadRam` run. Latest result reaches the
   loaded-image runtime loop around `PC=0o6000/0o6002/0o6012`.
   The probe compares sampled IM addresses against known `.mb` files:
   `Mesa.mb!3`, `TriMesa.mb!3`, `Cedar.mb!6`, `DSemu.mb!1`, and the
   UnBug Mesa image all report `0/6` (AEmu has no sampled addresses
   present), so `AltoMesaDorado.eb!1` should be treated as a distinct
   AltoMesa LoadRam image rather than a byte-for-byte copy of a checked-
   in `.mb`. Current post-load state: the loaded image now schedules
   multiple tasks when the probe clocks are allowed to run like
   free-running hardware (`[0]=8882891 [2]=28608 [4]=422755
   [14]=201554`, `switches=106789`, `ready_or=0x1015`,
   `wakeup_or=0x1014`), but DWT task 13 never runs and
   `display iofetch=0`. AHT/DSK are alive, so the next concrete target
   is DDC word-task wakeup fidelity: decode enough horizontal-task
   output state to set `NextWCBFlag`/`CurrentWCBFlag` and assert DWT
   wakeups when the channel wants display words. Current raw loaded IM
   samples:
   `0o6000=00104/71501/00000`, `0o6001=00104/131705/140000`,
   `0o6002=00104/14701/00000`, `0o6012=13116/14105/00000`,
   `0o6100=00204/60005/00000`, `0o5021=05406/77714/40000`.
4. Keep improving DiskMuff/sequence-PROM behavior in parallel so real
   emulator disk I/O has a solid controller after microcode load.
5. Re-run `build/test_cpu`; success means `CheckChecksumAndLoad` and
   `LoadRam` are reached after disk or Ethernet microcode load.

### Highest-leverage but hardest: fix Bootstrap streaming

Currently bypassed by substituting canonical Initial.MB at BOOTSTAGE2.
Real fix would mean we no longer need that workaround.

1. **Verify the BB ROM Boot1Data layout.** Disassemble bytes from
   `chm/dorado/doradobaserom.mb!13` C000-D7FF range and compare to
   Initial.mb in real-address order. If they're different builds,
   all debugging needs the BB ROM as ground truth.
2. Trace one full BootByteL iteration cycle-by-cycle through Bootstrap's
   T-composition (ReadBB1 → LSH[T,10] → ReadBB2 → LDF[T,10,0] → XOR Byte1).
3. Verify our shifter's LSH/LDF outputs match Bootstrap's intent
   (note: `10` in Mesa source is OCTAL, so `LSH[T,10]` = shift left 8).

Good luck. The infrastructure is solid; current state is "Initial
runs through canonical setup code with two probe-side workarounds,
stuck in I/O wait" — concrete next step is identifying the I/O
device at TIOA=0xC0.

## Recent commit history (reverse chronological, latest first)

```
862ba67 Advance BB Bootstrap into Initial
64a586c Bring BaseBoard Boot0 to Continuous
7560889 docs: io-systems-architecture reflects Fast I/O DONE
50d2d80 Fast I/O transport: Fin/Fout busses for DSK and DWT (HM §8)
6345f28 docs: refresh display+disk status to Phase 2
63797e9 Display + Disk Phase 2: Tag decoder + sector reads + FIFO renderer
292fd24 boot-bringup-plan: document probe_initial + BootstrapSources finding
2005727 probe_initial: load Initial.MB+Bootstrap.MB layered, run from INITIAL
bc18f7f probe_full_boot+bootstrap diagnostics: Write IM target tracking
2606929 boot-bringup-plan: document Phase-1 display+disk status
840d12e probe_full_boot_with_bootstrap: BB drives Bootstrap.MB via real CPReg stream
fcf07ef Detailed display + disk architecture references
a62339f Doc updates: display + disk Phase 1 status
078b0bd Trident T-80 disk port (HM §9) — Phase 1
79aa902 Display framebuffer + DDC stub (HM §11) — Phase 1
5b87de0 Microengine correctness fixes: ALUFM, JCN, RWCPReg
18bcd3c Slow-I/O routing layer (HM §7)
```

`git log --oneline -30` for more.

---

When you finish your session, **update this doc** with:
- Anything new that's working.
- Any pitfalls you discovered.
- The new active-task IDs.
- Updated "Suggested first action."

Keep it short, keep it honest. Don't write aspirational status.
