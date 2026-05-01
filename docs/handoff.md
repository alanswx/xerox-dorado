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
  Config' now follows `EMemDefs.mc` (`ChipSize` in b12/b13, C bits
  3..2, M0..M3 at `0200/0100/0040/0020`) and reports one present
  64K-chip/4MW module. Initial now gets through PRESETMAP, FINDMODULE,
  BootMem,
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
  core API that tests use today. `dorado_display_keyboard_set_key()`
  now exposes the named Alto keyboard matrix, imported from
  `AltoInfo/palo/src/simulator/keyboard.c`.
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

## Known gaps — full punch list

Compiled 2026-04-30 from a sweep of `dorado/src/*.c`, `dorado/include/*.h`,
and the narrative sections of this doc and `boot-bringup-plan.md`.
Section letters here are stable references — cite them in commit messages
and follow-up notes.

**Phase-progress index** (updated as gaps land — see
`docs/research-plan.md` for the per-gap plan and `docs/research/<id>-*.md`
for the as-built notes):

- ✅ **Phase 0** — archive sweep + BB ROM disassembler. (`bbdis` tool;
  `chm/disassembly/bb_*.s`; `chm-archive.md` §8.)
- ✅ **Phase 1** — local foundation gaps:
  - **B6** A[12:15]←FF[4:7] override — wired in `cpu.c::ff_a_low_override`.
  - **B7** `ff_full_function_ok` — gating audited correct; comment cleaned up.
  - **C4** dirty-victim WP fault — recorded in FaultInfo; test pinned.
  - **C5** ReadMap-on-Vacant — non-issue (PreFetch silence is per HM).
  - **G1** fast-IO drop counters — added 5 counters to `dorado_fastio_router`.
  - **D1** BB drop counters — added 2 counters to `dorado_baseboard`.
  - **B11** breakpoint/EventCnt/parity state slots — round-trip works;
    trap/tick semantics deferred until `Kernel.Press!6` is transcribed.
- ✅ **Phase 2** (research-complete; A1 implementation deferred):
  - **D2** ✅ BB ROM disassembled with 76 labels from the embedded
    symbol table; `bbdis --hunks` decodes Boot0 hunks. Annotated dumps
    in `chm/disassembly/`.
  - **A2** ✅ re-diagnosed: Boot0 (= Bootstrap) does **no** slow-IO.
    The original "Pd←Input returns 0" theory was wrong (FF=0o262 in
    Boot0 is `Pd←ALUFMRW`, not TIOA).
  - **B2 / H2** ✅ closed by A2 re-diagnosis — no slow-IO devices
    needed for Boot0.
  - **B8** ✅ research-only: spec captured in
    `docs/research/B8-rwcpreg-polarity.md`. Code change deferred —
    must land with A1 fix.
  - **A1** ✅ **RESOLVED 2026-04-30**: streaming was never broken.
    The 95/896 "match" rate is because the BB ROM Boot1Data is a
    DIFFERENT BUILD of Initial than the
    `bootstrap.dm!20_/Initial.mb` archive copy. `bbdis --hunks $C016
    224 0o6100` produces a bit-exact match against what our streaming
    delivers. BOOTSTAGE2 substitution shim is an intentional probe-
    time feature, not a bug workaround. See
    `docs/research/A1-bootstrap-streaming.md` "RESOLUTION" section.
- ✅ **Phase 3** (research-complete; B1+C3 implementations deferred):
  - **C6** ✅ MCR decode bit positions cross-checked against
    `EMemDefs.mc`. Fixed `dorado_mcr_disbr`. Added
    `dorado_mcr_dishold` getter. The 0xFEE7 special-case stays
    until B1 lands.
  - **C2** ✅ Per-slot Pipe4 error bits modeled.
    `dorado_pipe4_at(mem, srn)` composes the high-true value from
    per-slot state and XORs with the `0o150361` baseline. New
    `dorado_pipe4_set_error` API. The dirty-victim WP fault from C4
    now records `MAP_TROUBLE` in the triggering slot's Pipe4. Two
    new tests pin the encoding.
  - **B1/C1** ⏳ research note in `docs/research/B1-C1-hold-semantics.md`
    captures the spec from `InitialSubrs.mc` + `InitialMain.mc`.
    Implementation deferred — substantial architectural change.
    Note: boot-stage microcode runs with `mcr.disHold` set and
    uses cycle-counted waits, so it doesn't depend on Hold; only
    post-boot emulators (probe_aemu) require it.
  - **C3** ⏳ research note in `docs/research/C3-ecc.md`. ECC
    polynomial documented (HM §5.12). Deferred — nothing in our
    current test inventory exercises ECC.
- ⏳ **Phase 4** disk (research-complete; F1-F5 implementation
  blocked on disk content):
  - Pulled canonical disk sources: `DiskDefs.mc`, `DiskSubrs.mc`,
    `DiskBootSoft.mc`, `DiskBootTransfer.mc`. Findings in
    `docs/research/F-disk-phase3-sweep.md`.
  - Two real issues identified:
    1. **DiskMuff wire format discrepancy** — DiskDefs.mc spec has
       muffAddr in low byte and clear bits at LSB 8-11; our model
       and tests use high byte for addr and LSB 0-3 for clears.
       Need a per-cycle disk trace to reconcile.
    2. **Boot-stage disk failure is content, not model**: the
       hard-microcode file InitialDisk reads (cyl 0, head 0,
       sector 4) doesn't exist on `spruce-server.dsk300` (which
       is an Alto Spruce pack). Same diagnosis as A1: the model
       is correct; we lack canonical content. Workaround: Path B
       (Ethernet) — see H1.
- 🔢 **B3 sub-items landed** in Phase 7: PCX', DBuf. Remaining:
  XorCarry, ModStkPBeforeW, MidasStrobe, RestoreStkP,
  Hold&TaskSim, WF/RF, EventCntA-write,
  RMaddr replace force-write (FA=0/2 FB=4-5 / FB=2-3),
  MemBX, UseDMD, FreezeBC, LoadTestSyndrome, Multiply.

### A. Showstoppers blocking real boot

A1. **Bootstrap → Initial CPReg streaming corrupts data**
(`probe_full_boot_with_bootstrap`). ~768 of 896 IM half-writes do not
match canonical `Initial.MB`; LH writes default to `0x0044`, iw2
RSTK[0]/JCN[7] bits flip (e.g. `IM[0o6347]` lands as `local→0o6347`
self-loop instead of `local→0o6346`). Currently bypassed by substituting
canonical `Initial.MB` at BOOTSTAGE2. Suspected causes: shifter LSH/LDF
mis-execution in Bootstrap's T-composition; a different Boot1Data layout
in the BB EPROM vs. `Initial.mb`; CPReg-byte ordering between BB
ABMux0/ABMux1 and Bootstrap's T-decode. See §2a below.

A2. **`probe_full_boot` (BB Boot0) halts at 0o7744.** BB ROM contains a
*newer* compiled Boot0 than `Bootstrap.MB`. Reads slow-IO inputs
(`Pd←Input`/`Pd←InputNoPE`) we don't model, so ALUFM[N] from the cobweb-
clear NOPs comes back zero (`boot-bringup-plan.md` Phase A.7).

A3. **Hard-disk boot doesn't transfer.** Initial reaches `KSameDrive`,
`KContinueCmmd`, `KCheckSeek`, `KWAITSECTOR`, `WAITFORSECTOR`, then times
out at `Read1Muff` (`0o6500`). `DiskData` reads/writes stay zero. The
mounted `spruce-server.dsk300` is a Spruce Alto pack and probably doesn't
contain Initial's private "page 4" Dorado microcode file. No Trident pack
with the right contents is available.

A4. **Probe-only workarounds still live in `tests/test_cpu.c`:**
first-256-page identity-map shim at `DiskHardMicrocodeBoot`; canonical
`Initial.MB` swap at BOOTSTAGE2; `ETemp0..3` forced all-up + `GotBootKey`
redirect to `DiskHardMicrocodeBoot`; special-case decode of
`mcr.noWake = 0xFEE7`.

### B. Microengine (`src/cpu.c`, `include/cpu.h`)

B1. **No Hold semantics.** Memory `Md` is delivered immediately; real
hardware stalls the engine on miss (~28 cycles), on Pipe full, on
FreezeBC, on `StkError`, on IFU map fault concurrent with IFUJump.
AEmu's `LRTYPETABLE`→`LRTYPEIM`→`LRLOOPTOFF`→`TOFFRET` loop spins forever
because of this. (`include/cpu.h` HM §4 Hold; `include/memory.h` line 13.)

B2. **`Pd←Input` / `Pd←InputNoPE` slow-IO read FF ops** stubbed for many
devices (Hold/TaskSim register, MCR, Reset register). Boot0 needs them.

B3. **FF table gaps tagged "stub: silently honor" in `cpu.c`:**
- FA=0 FB=2 FC=4 `XorCarry`, FC=5 `XorSavedCarry`, FC=6 `Carry20`,
  FC=7 `ModStkPBeforeW` (line 627).
- FA=0 FB=4-5 RMaddr replace + force-RM-write (line 715).
- FA=2 FB=2-3 RMaddr replace + force-RM-write (line 915).
- FA=2 FB=5 FC=4..7 `MemBX←FF[6:7]` (line 932).
- FA=1 FB=4 FC=0 `UseDMD`, FC=1 `MidasStrobe`, FC=5 `RestoreStkP`.
- FA=1 FB=5 FC=4 `Hold&TaskSim`, FC=5 `WF←A`, FC=6 `RF←A`.
- FA=0 FB=7 FC=2 `Multiply`, FC=5 `TgetsMd`, FC=6 `FreezeBC`.
- FA=1 FB=2 FC=5 `LoadTestSyndrome`.
- FA=1 FB=3 FC=1 `EventCntB←B`; FA=1 FB=7 FC=1/4 `EventCntA'/B'` return 0.
- FA=1 FB=7 FC=0 `B←PCX'` returns 0; FC=5 `B←DBuf` returns 0.

B4. **Divide / CDivide** placeholders return Pd unchanged
(`cpu.c` lines 1055-1057).

B5. **Read IM** stubbed — advances PC but doesn't deliver inverted IM
bytes on B (`cpu.c` line 2046).

B6. **A[12:15] ← FF[4:7] override at A-bus time** not wired; FA=0 FB=0/1
silently no-ops (line 614).

B7. **B-source override gating** can be missed — `ff_full_function_ok`
doesn't always catch every BSEL constant case (TODO at line 450).

B8. **`B←RWCPReg` polarity asymmetry.** Inverts only on the legacy-stub /
single-step path; during BB free-run returns the raw value because
Bootstrap otherwise composes `Loc` with bit 15 set. Needs hardware-correct
fix once BB ROM Boot1Data layout is understood (line 542).

B9. **IFU pipeline simplifications** (`cpu.h` lines 92-96): no F/G→J→H→M
staging, single-cycle IFUJump, no Hold + IFU-map-fault interaction; only
InsSets 0/1's byte ordering modeled (cpu.c line 1579).

B10. **Reschedule / RescheduleNow** branch condition uses pending-wakeup
as a proxy for non-emulator tasks; device-driven IOAttention not wired
(line 1530).

B11. **Breakpoints / parity / performance counters** not modeled
(`BrkPending` ignored after `BrkIns←B`, line 858).

### C. Memory subsystem (`src/memory.c`, `include/memory.h`)

C1. **Atomic refs only.** No deferred references, no Hold, no cycle
counter (header line 13).

C2. **Per-slot Pipe4 error fields** not tracked beyond the constant
`0150361` baseline (cpu.c line 506).

C3. **ECC absent** — no syndrome generation, no double-bit error reporting.

C4. **WP fault on store** swallowed silently instead of asserting a real
WP fault (`memory.c` line 475).

C5. **`ReadMap` walks the map silently on Vacant** instead of generating
a map fault (`memory.c` line 614).

C6. **MCR active-low decode incomplete.** `mcr.noWake = 0xFEE7`
special-cased; full DisBR/DisCF/NoRef/FDMiss/UseMcrV semantics need to be
re-derived from schematics.

### D. BaseBoard (`src/baseboard.c`)

D1. BB 6502 unmapped offsets and EPROM writes silently dropped
(`baseboard.c` lines 223, 363) — should at least log; might mask a real
ROM fault.

D2. **No disassembly of the BB EPROM Boot0/Boot1Data** vs. `Bootstrap.mb`
— needed before A1 and A2 can be debugged with ground truth.

### E. Display (`src/display.c`, `include/display.h`)

E1. **`display_output_b` lacks per-(task,tioa) dispatch.** TODO at line
183: NLCB load, HRam load, Mixer load, PixelClk, Statics are collapsed
into one catch-all that only tracks NLCB/CLCB write counts plus the
WCB-flag protocol.

E2. **DDC input returns idle key word always** — no 7-wire terminal
back-channel, no keyboard message decoder, no boot-key selection
(`display.c` line 187). No way to select an emulator
(Mesa/Cedar/Lisp/Smalltalk/Alto) at boot.

E3. **No pixel clock / waveform / mixer** — `dorado_display_render_fifo`
assumes 1-bpp Alto mode and dumps the FIFO straight into the framebuffer
(header line 289).

E4. **DDC catch-all is registered on every TIOA** for tasks 3/4/011/013
(`display.c` line 220) — broad; refine once specific (task,TIOA) pairs
are observed.

E5. **Scanline timing is a synthetic shim** — real pixel-clock and HBlank
waveforms aren't driven from the actual control outputs (`display.h`
line 273).

### F. Disk (`src/disk.c`, `include/disk.h`)

F1. **Phase 3 sequence-PROM execution missing.** Read tag short-circuits
and dumps header+label into the FIFO contiguously instead of stepping
through preamble/sync/data/ECC/postamble per the read PROM (HM page 99).

F2. **Fire Code ECC absent** (`P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1`).

F3. **Write side stub:** sets `WrFifoTW=1` and marks active; nothing
commits the FIFO back to the pack (`disk.c` line 506).

F4. **Real sector-pulse timing → wakeup generation** is synthetic —
`dorado_disk_controller_advance_sector()` is poked manually from probe
code, not driven from a clock model (`disk.h` line 47).

F5. **Status readout partial** — KSTATE/KSTAT subset only;
`RdFifoTW` thresholds, block-mode status, ECC words, end-of-block
`ReadErr`/`WriteErr` summary bits not modeled.

F6. **Tag decode** carries both high-nibble (compatibility) and low-
nibble (`0x100A` ReZero) decoders side by side — chosen empirically; not
yet aligned to a single canonical decoding.

### G. Fast I/O (`src/fastio.c`)

G1. IOFetch/IOStore data destined for unrouted tasks is silently
discarded (line 38).

### H. I/O routing / devices

H1. **No Ethernet controller.** Now the next visible blocker after
hard-disk boot fails (Path A's Initial-fetches-emulator step).
`EtherMicrocodeBoot` falls through. A gated probe-only injector exists
for `.eb` images but isn't a real Ethernet model.

H2. **MCR / Hold/TaskSim / Reset register** not wired as slow-IO devices.
Returning floating-bus 0xFFFF + bad-parity for these is what blocks
Boot0 (A2 above).

### I. Tests / probes

I1. `probe_aemu` halts at `PC=0o7777` via `JNKINITPC`. Needs either
planted Mesa state (frame/MDS/context layout from Pilot docs) or Hold
semantics (B1).

I2. Probes carry environment-driven side paths (`DORADO_ETHER_BOOT_IMAGE`,
`DORADO_BOOT_SNAPSHOT`) — bring-up scaffolding; should be replaced or
migrated to real fixtures.

### J. .MB loader / disasm coverage

J1. Only **DMachine == 2 (model-1)** storage shuffle is exercised. Model
0 (3-word, no shuffle) and D0 (DMachine == 0) paths exist nominally but
are untested (`dorado/CLAUDE.md` "Model-1 storage bit-shuffle" section).

J2. Disassembler polish deferred: sharper FF/JCN sub-decoding, ALUFM
cross-reference, `.DLS`-format `--listing` mode.

### K. Phase-2 (Verilog) prerequisites not yet satisfied

K1. Cycle accuracy still skipped in many places (atomic refs, single-
cycle IFUJump, no Hold, synthetic disk timing). Each will need to land in
C before the RTL port has anything to mirror.

### Highest-leverage gaps (suggested ordering)

1. Disk sector/status path through `WaitForSector`/`Read1Muff`
   (F1–F5).
2. A real or controlled Ethernet boot path so `LoadRam` can pull in
   emulator microcode (H1).
3. Bootstrap streaming corruption — verify BB ROM Boot1Data layout vs.
   `Initial.mb` (A1, D2).
4. Hold semantics in the memory subsystem (B1, C1) — needed for AEmu
   and likely for any post-boot Mesa work.
5. `Pd←Input`/`Pd←InputNoPE` plus the slow-IO devices the BB ROM Boot0
   polls (B2, H2).

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
   Our BB pushes `ABMux1` (low byte) first, then `ABMux0` (high byte).
   During the Boot1 stream the first ABMux0 write clears CPRegH and the
   second writes `(MicroHalf << 1)|extra_bit`, creating the Dorado-visible
   data-ready transition. Bootstrap reads `T←~CPReg` once and decodes
   via `LSH[T,10]/LDF[T,10,0]`. If the CPReg layout has the byte data
   in a different position than Bootstrap expects, every T composition
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
of hard-coded `0xFFFF`. `EMemDefs.mc` defines `ChipSize` in b12/b13
(C bits 3..2) and present-module bits as M0=`0200`, M1=`0100`,
M2=`0040`, M3=`0020`; for bring-up the C model reports `ChipSize=3`
and one present 64K-chip/4MW module. With this, the full boot probe no
longer hits the NOSTORAGE bypass; Initial reaches `FINDMODULE`
naturally.

The old probe-side `0o6247 → 0o6357` bypass remains in
`test_cpu.c`, but it no longer fires in the normal run.

#### 2c. PRESETMAP / WAITFORMAPBUF loop (fixed enough for boot path)

After Config' was implemented with storage-present reporting, Initial
entered `FINDMODULE` but spent the budget in map initialization rather
than the old display `LONGWAIT` path. That was temporarily worked
around with a small-module report; current bring-up reports the real
64K-chip/4MW module layout from `EMemDefs.mc`. The earlier 60M-cycle
hot loop was:
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
  commands at once; the C model still carries high-nibble compatibility
  decoding but also recognizes the observed native low-nibble restore
  tag `0x100A` as Control Tag + ReZero. The probe spindle period is now
  `DORADO_DISK_SECTOR_PERIOD` with default 512 cycles; this lets DSK
  reach `KSameDrive`/`KCheckSeek` before Initial's first
  `BootTransferTimeout`. The latest focused probe no longer corrupts
  CHS to head 10 and still ends with DSK at `Read1Muff` (`0o6500`),
  no FIFO reads/writes yet.
- New memory-system fidelity: `NoRef+UseMcrV` stores now update the
  selected cache-address entry without touching map/storage. This is
  needed by `InitialSubrs.mc` `ClearCacheFlags`.
- New disk fidelity: DiskMuff input now returns asserted signals as
  native `0x8000` on `IOB[15]`, per HM §9 pages 101-102. Returning
  `0x0001` made sign-branch tests (`R<0`/`ALU<0`) see true KSTATE/KSTAT
  signals as false.

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
   free-running hardware. A focused subset of `_cd8_/doradomicrocode/`
   was downloaded into `chm/doradomicrocode/doradomicrocodesources/`;
   `ADefs.mc!3` confirms the expected task numbers and
   `DisplayMain.mc!1` confirms the DHT/THT -> WCB -> DWT/TWT handoff.
   The emulator now models the WCB flag protocol, but the latest
   120M-cycle run still has `display iofetch=0` and DWT wakeups `0`.
   The useful new clue is that all display writes are task 4 to TIOA
   `0366` (`TNLCB`) plus two writes to `0367` (`TStatics`); there are
   no writes to `0364` (`AHTFlag`), and low-core display words
   `0420..0427` (`DAStart` region) are all zero. That means task 4 is
   alive and blanking/terminal-scanning, but no display control block
   chain has been installed for the word task to fetch. Current raw
   loaded IM samples:
   `0o6000=00104/71501/00000`, `0o6001=00104/131705/140000`,
   `0o6002=00104/14701/00000`, `0o6012=13116/14105/00000`,
   `0o6100=00204/60005/00000`, `0o5021=05406/77714/40000`.
   After the `NoRef` cache-address, cache `Vacant` lookup, DiskMuff
   `IOB[15]`, and display WCB protocol fixes, a 120M-cycle focused EB
   run moves farther: final `PC=0o6307`, display snapshot `frame=180`,
   task 4 hot at `0o6300/0o6301/0o6311/0o6721/0o6744`, but still
   `display iofetch=0`, DWT wakeups `0`, and disk FIFO reads/writes
   `0`. The cache `Vacant` fix is important because Initial's
   `ClearCacheFlags` writes all CacheA entries and then marks them
   vacant; those addresses remain readable through `dVA<-Victim` but
   must not be treated as cache-data hits.
4. Keep improving DiskTag/format-RAM/sequence-PROM behavior in parallel
   so real emulator disk I/O has a solid controller after microcode
   load. The next likely disk gap is the controller read/check FIFO
   status sequence: `RdFifoTW` thresholds, block-mode status, ECC words,
   and end-of-block `ReadErr`/`WriteErr` summary bits.
5. Re-run `build/test_cpu`; success means `CheckChecksumAndLoad` and
   `LoadRam` are reached after disk or Ethernet microcode load.

### 2026-04-30 EB direct-load probe update

`DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
./build/test_cpu` now parses the EB End item (`start=0o1076`) and has
a probe-side direct LoadRam path that installs EB IM/IFUM/RM items.
This is a diagnostic shortcut, not a real Ethernet controller.

Result: the run reaches Alto/Mesa initialization/display/disk PCs
(`0o1077`, `0o4656`, `0o5624`, `0o6744`, `0o7000`, etc.) and the
loaded IM samples match the EB payload, but it halts in task `0o14`
at `PC=0o6` on `halt: IFU not ready`. IFU diagnostics show
`active=0`, `PCF=0`, and `Post-LoadRam IFU transitions: arms=0`, so
no `PCF<-B` has happened before a page-zero IFUJump exit is reached.
Next best action: debug task startup/TPC/CoReturn/LdTPC after EB
initialization, especially why task `0o14` resumes through `0o3500`
with `Link=0o6`, instead of continuing to emulator `StartIFU`.

### 2026-04-30 EB direct-load stale I/O cleanup

The immediate disk-task `PC=0o6` halt was a probe artifact: the direct
LoadRam shortcut carried Initial's partially active disk controller state
into the AltoMesa image. The probe now resets the disk controller at EB
takeover, preserving the attached pack, and the display scanline model no
longer wakes DHT before display microcode selects a terminal task.

### 2026-04-30 memory-map follow-up

The memory-map docs/sources were rechecked after the stale-I/O cleanup.
`LoadRam.mc` confirms the direct EB loader was already loading RM from
item word 2. `NewMemory.mc`/`InitMem.mc` show that `B←Map'`/`Pipe3'`
returns the old RP, while old WP/Dirty are recovered through
`B←Errors'`/`Pipe4'`; the emulator now snapshots both. `EMemDefs.mc`
also corrected the Config layout: `ChipSize` is b12/b13 (C bits 3..2)
and module bits are M0=`0200`, M1=`0100`, M2=`0040`, M3=`0020`.

Current EB status: the direct run reaches the cycle budget rather than
an early halt, writes display-control outputs from task `0o4`, and
produces `/tmp/dorado_boot_display.pgm`. The Config fix moves the EB
state forward: `R400=0x0100`, BR31/BR36/BR37 now become `0x20000`
instead of `0x10000`, and the old Mesa `XFER/XFERMD` loop at `0x10000`
is gone.

Follow-up: `B←FaultInfo'` now clears the latched FaultInfo state, as
`InitMem.mc` expects, and the direct EB probe maps the skipped warm-start
bank `0x20000..0x2FFFF`. The same run now reports
`Memory: faults=0`; `Mar=0x2FE1F` is just the last reference, with
`Map[0x2FE]=rp02FE wp0 d0 r1`. The next blocker is task/display
bring-up, not memory translation: IFU still never arms, `DAStart`
remains zero in the IOBR bank, and the probe eventually switches to
task `014` with TPC `0177037` after running task 0 hot at
`0o5013/0o5014/0o5022`. Next best action is to trace the loaded Mesa
world's task initialization and wakeups, especially why DSK/display
wakeups become ready before their TPCs are valid and before `PCF←B`.

2026-05-01 update: the direct EB probe now also resets the modeled
display controller at the LoadRam/IOReset takeover, matching the disk
controller reset already done there. This removes Initial's stale
terminal-task state from the loaded world. With
`DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1
./build/test_cpu`, the run no longer switches into an invalid DSK TPC:
it stays in task 0, reports `Memory: faults=0`, `MCR=0x0004` with
`nowake=0`, and no post-EB display/disk wakeups. The new blocker is
inside the loaded Mesa/AltoMesa IFU startup path: it halts at
`CPU_HALT_IFU_NOT_READY`, final `PC=0o4`, after hot task-0 PCs around
`0o4654/0o4656/0o4657` (`Mesa.mb!3:SETDLP`) and `0o5724/0o5736`.
Suggested first action: trace that Mesa IFU path against `Mesa.mb!3`
symbols and IFU pause/PCF semantics; the I/O wakeup ordering bug is no
longer the first direct-EB blocker.

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
