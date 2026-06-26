# Running the Dorado hardware diagnostics

**Date: 2026-06-23 (updated 2026-06-25).** How to run PARC's original Dorado
diagnostics on our microengine — the one real-hardware-grounded oracle we can
actually obtain (see
[`contralto-oracle-validity.md`](contralto-oracle-validity.md): ContrAlto is an
Alto, not a Dorado, so it can't validate Dorado-specific behavior).

**Status: 2 of the suite now fully PASS — `kernel` and `memA`.** eventCounters
passes its genIO subtest and is blocked on real event counting; memMisc,
Ifu{Complex,Simple} and Tricond have larger feature gaps (see the table below).

## What the diagnostics are

`getdiagnostics.cm` names the suite: **kernel, Ifu, memA, memMisc,
eventCounters** (plus Tricond). They are self-checking microprograms PARC wrote
to verify *real* Dorado hardware. We have them extracted under
`chm/dorado/expanded/<name>.dm!NN_/<name>.mb`, with sources at
`chm/doradomicrocode/kernelsources/` (kernel) and the `.midas` Midas run-recipes
alongside each `.mb`.

## How a diagnostic runs (from kernel.midas + the kernel sources)

The Midas recipe (`kernel.midas`) is: Reset → load the `.mb` → set `MCR=1`
(disable memory stack over/underflow wakeups), default task 0 / rbase 17, set
`FLAGS` (task/hold/task-circ simulation; inactive on the first pass regardless) →
`Go BEGIN` with a timeout. The shared Preamble/Postamble framework defines the
pass/fail convention used by every diagnostic in the suite:

- **entry** = label `BEGIN`.
- **PASS** = reach label `DONE` — the success point: it breakpoints, bumps the
  32-bit `ITERATIONS` counter, runs the hold/task simulators, then loops to
  `BEGIN`. (On real hardware under Midas you watch ITERATIONS climb.)
- **FAIL** = reach label `ERR` — the `ERROR` macro is literally `BRANCH[ERR]`.

## The runner

`build/rundiag` loads a diagnostic, applies the recipe (MCR=1, task 0, rbase 17),
jumps to the entry, free-runs, and reports PASS (reached `DONE`) / FAIL (reached
`ERR`) / HALT (engine hit an unimplemented op — names which) / TIMEOUT.

```
make build/rundiag
build/rundiag <diag.mb> [entry=BEGIN] [done=done] [err=ERR] [maxsteps]
# the suite uses uppercase DONE:
build/rundiag '../chm/dorado/expanded/kernel.dm!38_/kernel.mb' BEGIN DONE ERR
RUNDIAG_TRAIL=1 build/rundiag ... 2>&1   # dump the last 48 PCs on a non-PASS
```

## Baseline + debugging status

> **CORRECTION (2026-06-23, grounded in the original `.mc` sources now mirrored
> at `chm/doradosource/diagnostics/`).** An earlier pass claimed memA, memMisc,
> and Tricond "all gate on the memory Hold mechanism (gap B1)." Reading the
> actual PARC sources shows this is **wrong** — they need **three different**
> mechanisms, and none of them is the memory-miss engine stall the
> cycle-accurate-timing doc calls gap B1. The table below is the corrected,
> source-checked diagnosis. See "Corrected per-diagnostic diagnosis" below.

| diagnostic     | result | root cause (source-checked) | fix size |
|----------------|--------|-----------|----------|
| kernel         | **PASS** (reaches DONE @4.06M steps; was @79 → 1.34M → 3.93M → 4.06M) | 4 bugs FIXED: (1) runner RBase 0; (2) bogus `(T>>8)&7)<<1` shifter special case removed — FF-controlled SHA/SHB source is BSEL[1:2] (HM §3.11); (3) microcode `Wakeup[n]` 2-cycle latency (HM p27) — fixes the `TASKTESTERR` preemption test; (4) §3.12 **TASKSIM** counter (`Hold&TaskSim←B`, FF=0o154) waking the jumpered sim task 0o12 — fixes `TESTTWERROR`. **First PARC hardware diagnostic to fully pass.** | — |
| eventCounters  | genIO + setup-fault + **eventTrue** FIXED; now fails at `EVENTAHOLDERRLO` @4.1M (eventHold) | **Three fixes landed:** (1) genIO loopback (19d5226). (2) The `KFAULT3` setup fault was a junk-task bug — `IFUTest←B(0)` (IFU setup) spuriously *enabled* the periodic junk timer, waking the still-parked junk task (empty IM 0o7777) → halt; per HM §8.3 `IFUTest.15` only *disables* the junk request when set, never enables (ca44d3e). (3) HM §4.11 **event counters** — per-cycle EventCntA/B counting with the event select decoded from the MOS load and **tasksAll** gating (default counts only EmuOrFT = task 0 + 0o17; without it the interleaved sim/junk cycles inflate the count) — **eventTrue passes** (9bb3cc0). **Remaining:** `eventHold` counts Hold events but our counter sees 0. Traced precisely (and *correcting* an earlier guess): the eventHold inner loop in **this .mb (UnBug.bfs) is `Fetch`(real 0o26, asel=3=Fetch←T) ↔ an RM op (real 0o23, asel=4=A←RM/STK, bsel=1=B←RM/STK, lc=6=RM/STK←Pd)** — it reads **no Md at all** (verified against HM Tables 8/10), so our Md-read Hold never fires. The `eventCounters2.mc` source I mirrored (from eventCountersSources.dm!5_) shows `rscr_(Md) or rscr`, so the **UnBug.bfs .mb is a different build** whose loop reads no Md. **DIAGNOSIS CORRECTED (2026-06-25, this session): the eventHold hold source is the HM §3.12 HOLDSIM debug register, NOT a §5.4 reference/port-busy Hold.** Triple-confirmed by a per-cycle trace of the loop: (1) the 8-cycle loop is `tk=0 0o26`(Fetch←T) + `tk=0 0o23`(lc=6 RM/STK←Pd) — neither reads Md, so md_not_ready is never true; (2) the §3.12 **sim task (task 12)** runs `0o5202`=`Hold&TaskSim←B` (FF=0o154) every iteration, arming HOLDSIM=0o362 (~1.03M times) — the only possible hold source; (3) a single Fetch every 8 cycles can't cause a port-busy Hold either. So `eventHold` needs the **§3.12 HOLDSIM recirculating-shift-register hold injection** (postamble.mc: "8-bit recirculating shift register in which a 1 in bit 7 causes HOLD two instructions later"). **Implemented this session (non-regressing, all standing gates green):** the HOLDSIM recirculation (verified 0o362→0o171→0o274→0o136→0o362, popcount-preserving) + a 2-instruction-delay trigger feeding `EVC_EV_HOLD` (cpu.c task_schedule + the hold block; cpu.h `holdsim`/`holdsim_delay`/`holdsim_hold`). **Remaining (the actual blocker):** the injected HOLD currently lands on **sim-task (tk=12) cycles**, not the task-0 reference cycle, so the EmuOrFT-gated counter still reads 0. Closing it needs the **HOLD freeze** (a held cycle must freeze the engine and stay on the current task, not jump-to-self/switch) coupled with a **cycle-exact sim-task (TASKSIM) cadence** so exactly one hold lands on each task-0 reference (the test checks `count == rscr2+1` exactly, no slop). This is the cycle-accurate sim-cadence core — bounded, with a no-slop oracle, and zero game/Cedar regression risk (HOLDSIM is inert unless a diagnostic arms it). | medium (HOLD freeze + cycle-exact §3.12 sim cadence) |
| memA           | **PASS** (reaches DONE @220787595 steps; was FAIL @87553 `CATUPADDRERR`). **Second PARC hardware diagnostic to fully pass.** | Root cause was NOT a cache-address-register fidelity gap (the earlier guess) — our `tag=va>>10` / `cache_address_va=(tag<<10)\|(row<<4)` round-trip is correct (verified against `getPipeCacheABits`/`setBrCacheABits` in `memSubrsC.mc`; the constants are **octal** — `CABitsInPipe1Shift=0o12=10`, `mcrVshift=0o13=11`, `nBitsInPage=0o10=8`/256-word page — so there was no "2-bit offset"). The real cause: the **Cache Address Test** (`cacheAddrTest` in the now-mirrored `memASource/memRWc.mc`) zeroes the whole cache then walks every (row,col) expecting 0, and calls `disableConditionalTask` at `beginCtest` to stop the §3.12 memory-simulator task (jumpered **task 0o12**). Our TASKSIM tick fired on any non-zero counter, so a stale re-arm value (0o104, **enable bit clear**) kept waking task 12, which made a `noRef`+`useMcrV` STORE (inheriting task 0's sweep MCR) that wrote the cache-address memory and clobbered the zeroed cache. **Fix:** gate the TASKSIM tick on the `0o10` ENABLE bit (postamble.mc: "taskSim[0] enables … taskSim[1:3] form a counter … counts to 17[octal]"). cpu.c. | DONE (fixed) |
| memMisc        | TIMEOUT | `aMapTest` (memMapA.mc) **explicitly disables the HOLD simulator** (`call[disableConditionalTask] * don't run HOLD simulator`). It tests the **Map ref bit** + **fault-task (task 15) wakeups** (write-protect fault, page fault) + **Pipe4 status bits** (wProtect/Mfault/memErr), looping over **every** map page (slow). Needs the per-fault fault-task handshake + accurate Pipe4, NOT engine-stall Hold. | large (fault/Pipe4) |
| Tricond        | FAIL @105 (`State.Errs`) | **Trident DISK-controller diagnostic.** Reads the disk-controller **state muffler `KSTATE`** via the DMux (`read20Muffs`), XORs 0o70, masks 0o76377, expects 0. Our engine returns no real disk muffler bits. **Nothing to do with Hold** — the "control for hold, task simulator" in TriconD.midas is boilerplate copied from kernel.midas. | large (disk mufflers) |
| IfuComplex/IfuSimple | FAIL @~1631 (`IFUEXCEPTIONERR`, task 2) | Source now mirrored (`IfuSource/`, see its README). Both fail in `iSingleStepTest`/`itTestCase1` — the first test using the **ifuTest register**. The IFU exception model (HM §6 Tables 14/20): an IFUJump can take an exception (kfault/FGparity/RAMparity/resched × 4 FGP bytes) and dispatch to a trap vector that sets `exceptionsMask`; `checkException` errors if the expected bit (here kfault3=b0, T=0o100000) isn't present. We model a simplified IFUJump with no exception conditions/trap vectors, so `exceptionsMask` stays 0. Needs: `ifuTest←B` register, IFUJump exception-dispatch, and the JMPEXC muffler feeding `exceptionsMask`. The `resched` exception (we partly model `reschedule_pending`) is the cheapest first end-to-end wire. | large (IFU §6 exception dispatch) |

### kernel (in progress)
- **Fixed:** the runner forced `RBase=017`, but the diagnostics' pre-loaded
  registers (`R1=1, RM1=-1, R10=125252B, R01=52525B, RHIGH1=100000B`) live in
  **RBase 0** (the .mb RM data: `RB0RM0`/`R1`/`RM1` at RM[0..]). With RBase=0 the
  kernel runs 79 → 1,343,804 steps (clears aluEQ0/aluLT0/rEven/rGE0/bypass/ALU
  ops). The bit-walking ALU=0 test passed before the fix only because it uses FF
  constants, not registers.
- **Next:** see TASKTESTERR below — the next failure is the §3.12 task
  simulator (TASKSIM), not the datapath.

### kernel — shifter bug FIXED (2026-06-23)
The masked-shift test (`Rlsh`/`Tlsh`/`Rrsh`/`Trsh` + the cycle tests) used to
fail at real PC 0o2604. Cause: a bogus special case in `shifter_output`
(`if (bsel==4 && aluf==4 && ff==1) return ((T>>8)&7)<<1`) that pattern-matched
one Bootstrap shift and returned a fixed value instead of shifting. It
**over-matched** the kernel's legitimate `lsh[rscr,1]` (same fields, different
`rstk`), corrupting it.

Root cause and fix, grounded in primary sources (HM §3.11 + the real microcode,
now mirrored at `chm/doradosource/`):
- The FF-controlled shifter (BSEL=4..7, ASEL=7) takes its SHA/SHB source from
  **BSEL[1:2]** exactly as the standard form takes it from `ShC[2:3]`: bit set
  ⇒ T, clear ⇒ RM/STK. Confirmed in kernel.mb (`Rlsh`=shift RM ⇒ BSEL=4;
  `Tlsh`=shift T ⇒ BSEL=7) and in `BootstrapMain.mc` (`BTemp_ LDF[T,3,10]` is
  one instruction, BSEL=7 shift T → `(T>>8)&7`; `BTemp_ LSH[BTemp,1]` is the
  *next* instruction, BSEL=4 shift the RM reg BTemp → `<<1`).
- The hack conflated those two real instructions into one and only happened to
  give Bootstrap the right answer because `BTemp` had just been loaded from T.
- Fix: deleted the special case; the genuine §3.11 path (already present)
  computes all of them. Result: kernel 1.34M → **3.93M** steps (clears the whole
  shifter suite), Galaxian still 121602 px, Cedar boot timing unchanged, all 12
  test suites green. The synthetic `test_bootstrap_ldf_dispatch` had encoded the
  hack's wrong semantics and was rewritten to model the two real instructions.

### kernel — TASKTESTERR FIXED: the wakeup→preempt latency (HM p27)
With the shifter fixed, the kernel failed next at `TASKTESTERR` (0o5546). This
is NOT the task simulator (an earlier guess) — it is `Kernel5.mc`'s **task
preemption** test. Root cause, from the real microcode + a task-tagged trace:
`notifyTask` (Postamble) holds its return address in the shared RM scratch
`rscr` across `notify[N]; branch[nxit]; nxit: link←rscr; return`. The simple
TaskSwitch test and the first preempt pass work, but ours preempted the
notifying task **one cycle too early** — immediately after `notify[N]`, before
it ran `nxit` (`link←rscr`). The higher task, sharing RBase=0, then ran its own
`rscr←link` and clobbered the first task's saved return, so on resume it
returned into the wrong code.

HM p27: "a minimum of two cycles elapses after the instruction containing
Wakeup before the task executes its first instruction." Modeling that 2-cycle
latency for microcode `Wakeup[n]` (a 2-stage `wakeup_pipe` in `cpu.c`
`task_schedule`) lets the notifying task run the instruction after the Wakeup
(moving its return from shared `rscr` into per-task `Link`) before being
preempted. Default on; `DORADO_WAKE_IMMEDIATE` reverts. Device wakeups
(`dorado_cpu_wakeup`) are intentionally left immediate (the boot is co-tuned to
them; that is the separate cycle-accurate-timing project). Validated:
kernel 3.93M → **4.06M**; Galaxian 121602, NetExec, Boggs/EDP/Calculator/
PinBall all unchanged, Cedar germ-load cycle identical (67279169), 12 suites
green. `test_wakeup_ff_function` was asserting the old immediate behavior and
was corrected to the HM 2-cycle timing.

### kernel — TESTTWERROR FIXED: §3.12 TASKSIM (kernel now PASSES)
`TESTTWERROR` (0o5626) was `Kernel5.mc`'s **TestTW** test — and (correcting an
earlier guess that it was the JunkTW/Pendulum signal) its own comment says
"the Counter is loaded with a FF=154 (Hold&TaskSim_B) function." It is the HM
**§3.12 TASKSIM** task-simulator: `hold&tasksim←0o400` loads the smallest count,
the test waits, and the jumpered simulator task must wake and zero Q.

`Hold&TaskSim←B` (FF=0o154) was a no-op. Implemented per HM §3.12: the
Hold&TaskSim word is TASKSIM in the **left byte** and HOLDSIM in the **right
byte** (confirmed by the diagnostic framework's `sim.taskShift=0o10`,
`sim.taskMask=0o177400` vs `sim.holdShift=0`, `sim.holdMask=0o377` in
`preamble.mc` — the "hold value in left byte" comment in `postamble.mc` is
misleading). So TASKSIM = (B>>8)&0x7F, HOLDSIM = B&0xFE. **TASKSIM is a 4-bit
register** (postamble.mc: "taskSim[0] enables the task simulator and
taskSim[1:3] form a counter … when hardware counts to 17[octal] it awakens").
The `0o10` bit is the **ENABLE bit**: the counter runs (and overflows to a
wakeup) only while it is set — gating on it is what lets `disableConditionalTask`
stop the sim task (and is the **memA `CATUPADDRERR` fix**, see the memA row).
The wakeup target is task **0o12** (=10 dec) — Dorado task numbers are octal, so
the kernel's "task number 12" / `T_(12s)` is 0o12, NOT decimal 12 (the first
attempt woke decimal 12 = 0o14, which ran TaskError). HOLDSIM is stored but its
HOLD injection is unmodeled (the diagnostics' first pass runs with HOLDSIM=0).
Guarded by `test_tasksim_wakeup`. (Note: the `UnBug.bfs` `kernel.mb` we run does
not itself load TASKSIM — it PASSES at 3765457 steps unchanged by this gate.)

**With this, the kernel diagnostic PASSES end-to-end (reaches DONE)** — the
first of PARC's six Dorado hardware diagnostics to fully pass on our engine.
Galaxian 121602, Cedar boot cycle unchanged, 12 suites green; the TASKSIM tick
is inert (a no-op) unless microcode loads a non-zero `Hold&TaskSim`, which only
the diagnostics do.

### eventCounters (diagnosed; loopback fix attempted, reverted)
General-IO subtest `GENIO`: writes a pattern to GenOut (EventCntB) and expects
GenIn (EventCntA) to track it (backpanel loopback). Our `event_cnt_a` is a
write-never stub. A `event_cnt_a = event_cnt_b` mirror on `EventCntB←B` was tried
but had **no effect** (the run is byte-identical, so `EventCntB←B` isn't reached
before the step-674 failure) — the exact GenIn/GenOut polarity/sequence needs
re-derivation (the diagnosing agent flagged this uncertainty: no wiring doc, and
the `eventCounters.cm` source is in the CHM archive but not local). The later
subtests additionally need real per-cycle event counting (HM §12.3).

### Corrected per-diagnostic diagnosis (memA, memMisc, Tricond)

Grounded in the original `.mc` sources, now mirrored locally at
`chm/doradosource/diagnostics/` (TriconD-Diagnostics, memAll-Source =
`memMiscSource.dm`, diagnosticSubrs). The earlier "all three gate on the memory
Hold mechanism" claim does not survive reading the sources — they need three
different things, and none is the gap-B1 memory-miss engine stall.

- **Tricond → Trident DISK-controller diagnostic (`tricond.mc`).** "TriconD" =
  Trident Controller, Dorado. It fails at `State.Errs`: it reads the disk
  controller's **state muffler word `KSTATE`** through the DMux muffler-read
  subroutine (`read20Muffs`), normalises (`T XOR 0o70`), masks the determinate
  bits (`AND 0o76377`), and expects `0`. Our engine has no disk-controller
  muffler model, so the read is wrong. This has **nothing to do with Hold**; the
  `Hold&TaskSim` register in TriconD.midas is leftover boilerplate (the midas
  header says "KERNEL.MIDAS display relevant rm, im values"). Fixing it means
  modeling the Trident disk controller's muffler/state readback — a disk feature,
  on the disk track, not the timing track.

- **memA → the §3.12 Hold&TaskSim DEBUG simulator + memory simulator subtest.**
  memA reaches `SETHOLD` (0o5520) and writes `Hold&TaskSim ← B` (FF=0o154) — the
  HM **§3.12** hardware-checkout register: `HOLDSIM[0:7]` (a recirculating
  8-bit shift register; a 1 in bit 7 forces HOLD two instructions later) and
  `TASKSIM[0:6]` (a 7-bit counter that requests a task wakeup when it overflows
  0o177→0o200). It then runs `MEMSIMINIT` — a memory *simulator* subtest that
  expects those simulated holds/wakeups. `Hold&TaskSim ← B` is a no-op
  (`cpu.c:1522`) with no HOLDSIM/TASKSIM state, so the polled status never moves
  and memA livelocks. This is the §3.12 *debug simulator*, **not** the memory
  cache-miss stall. Self-contained and well-specified; doesn't touch the boot or
  memory-miss path.

- **memMisc → Map ref-bit + fault-task wakeups + Pipe4 status (`memMapA.mc`).**
  memMisc shares the memAll source; it hangs in `aMapTest`, whose first
  microinstruction is `call[disableConditionalTask] * don't run HOLD simulator`
  — i.e. it **explicitly turns the hold simulator off**. `aMapTest` (per its own
  header) "checks the map reference bit and that various faults cause wakeups":
  per map page it clears the Ref bit via `setMap`, `PreFetch`es (Ref must set),
  sets write-protect and `Store`s (must take a **write-protect fault** → fault
  task task 15 deposits FaultInfo in Q, `Pipe4.wProtect`/`Pipe4.Mfault` set),
  then marks the page vacant and `Fetch`es (must take a **page fault**,
  `Pipe4.memErr` low-true distinguishing it from a data error). It loops over
  **every** map page, so it is also just slow. What it needs is the **per-fault
  fault-task (task 15) handshake** (FaultInfo: nFaults incremented per fault,
  firstSrn latched) and **accurate Pipe4 per-reference status bits** — **not**
  engine-stall Hold.

**Implication (revised):** there is no single "implement Hold" that turns all
three green. They are three separate features: a disk-controller muffler model
(Tricond), the §3.12 Hold&TaskSim debug-simulator register (memA), and the
fault-task + Pipe4 machinery (memMisc). The §3.12 simulator (memA) is the only
one that is a genuine "Hold" feature, and it is the most self-contained.

### IfuComplex (diagnosed)
FAIL @1622, `IFUEXCEPTIONERR`, running in task 2 (the IFU-test/junk timer). It
self-checks the **IFU exception latch (`JMPEXC`) + IFU diagnostic muffler
signals** (JMPEXC/PCJ/FFK/HJ/MX), which the engine doesn't model:
no IFU-exception state in cpu.c; `dorado_memory_dmux_read` returns 0 for the IFU
mufflers; the IFU-test junk wakeup is a fixed 533-cycle cadence not coupled to
IFU events. Fix = model the IFU exception mechanism + wire the mufflers (HM §6);
recover the `IfuComplex` source from the CHM archive first to get the exact
expected bit pattern.

## Next

**kernel now PASSES** (4 bugs fixed; see above). Remaining diagnostics, by
tractability:
- **memA** — needs the §3.12 **HOLDSIM** half (the recirculating hold shift
  register that injects engine HOLD; TASKSIM is now done) plus `Pipe4'`
  per-reference status. The kernel work already built the TASKSIM half.
- **memMisc** — fault-task (task 15) per-fault handshake + Pipe4 status bits
  (the Map ref-bit / fault test; HOLD simulator is disabled there).
- **eventCounters** — GenIn/GenOut loopback + per-cycle event counting.
- **IfuComplex** — IFU exception latch (JMPEXC) + diagnostic mufflers.
- **Tricond** — Trident disk-controller state mufflers (disk track).

Use `RUNDIAG_TRAIL=1` (now task-tagged, `tk=`) + the real `.mc` sources
(`chm/doradomicrocode/kernelsources/`, `chm/doradosource/diagnostics/`) to
diagnose against the actual microcode, not summaries.
