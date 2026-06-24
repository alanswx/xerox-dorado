# Running the Dorado hardware diagnostics

**Date: 2026-06-23.** How to run PARC's original Dorado diagnostics on our
microengine — the one real-hardware-grounded oracle we can actually obtain (see
[`contralto-oracle-validity.md`](contralto-oracle-validity.md): ContrAlto is an
Alto, not a Dorado, so it can't validate Dorado-specific behavior).

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

## Baseline + debugging status (2026-06-23)

| diagnostic     | result | root cause | fix size |
|----------------|--------|-----------|----------|
| kernel         | FAIL @1.34M steps (was @79) | 1st bug FIXED (runner RBase 0). Next: Bootstrap-LDF special case over-matches kernel shifts (cpu.c ~2003) | medium |
| eventCounters  | FAIL @674 (`GENIOERR2`) | GenIn/GenOut general-IO stub (loopback polarity TBD) + no real event counting (HM §12.3) | small + medium |
| memA           | TIMEOUT | **HOLD (gap B1)** + `Pipe4'` status unmodeled | large (Hold project) |
| memMisc        | TIMEOUT | **HOLD-on-fault** + per-fault fault-task handshake | large (Hold project) |
| Tricond        | FAIL @105 (`STATE.ERRS`) | **Hold&TaskSim** register + Hold stall unmodeled (cpu.c:1522 no-op) | large (Hold project) |
| IfuComplex     | FAIL @1622 (`IFUEXCEPTIONERR`) | IFU exception latch (JMPEXC) + diagnostic mufflers unmodeled | large (IFU feature) |

### kernel (in progress)
- **Fixed:** the runner forced `RBase=017`, but the diagnostics' pre-loaded
  registers (`R1=1, RM1=-1, R10=125252B, R01=52525B, RHIGH1=100000B`) live in
  **RBase 0** (the .mb RM data: `RB0RM0`/`R1`/`RM1` at RM[0..]). With RBase=0 the
  kernel runs 79 → 1,343,804 steps (clears aluEQ0/aluLT0/rEven/rGE0/bypass/ALU
  ops). The bit-walking ALU=0 test passed before the fix only because it uses FF
  constants, not registers.
- **Next:** the shifter test (`Rlsh`/`RLSH*` = masked left shift of a walking bit
  via RF←/WF← field descriptors → ShC) fails at real PC 0o2604. Suspect
  `field_desc_to_shc` / the masked-shift (ShiftLMask) path in cpu.c.

### kernel — shifter bug ROOT-CAUSED (fix deferred)
The masked-shift test (`Rlsh`) fails at real PC 0o2604. Cause: the **Bootstrap
`LDF[T,3,10]` special case** in `shifter_output` (cpu.c ~2003, condition
`bsel==4 && aluf==4 && ff==1`) **over-matches**. The kernel's legitimate masked
shifts share `bsel=4/aluf=4/ff=1/lc=6/asel=7` and differ only in `rstk` (the
destination), so they wrongly receive the BaseBoard-dispatch hack
(`((T>>8)&7)<<1`) instead of an actual shift. `lc` does not distinguish them
(both lc=6 = `RM/STK←Pd`). The proper fix is to model the HM §3.11 SHA/SHB source
rule for the FF-controlled shift form (does it shift T or R?) so the normal path
produces both the Bootstrap and the kernel results and the hack can be removed —
guard with `test_bootstrap_ldf_dispatch` + the kernel diagnostic.

### eventCounters (diagnosed; loopback fix attempted, reverted)
General-IO subtest `GENIO`: writes a pattern to GenOut (EventCntB) and expects
GenIn (EventCntA) to track it (backpanel loopback). Our `event_cnt_a` is a
write-never stub. A `event_cnt_a = event_cnt_b` mirror on `EventCntB←B` was tried
but had **no effect** (the run is byte-identical, so `EventCntB←B` isn't reached
before the step-674 failure) — the exact GenIn/GenOut polarity/sequence needs
re-derivation (the diagnosing agent flagged this uncertainty: no wiring doc, and
the `eventCounters.cm` source is in the CHM archive but not local). The later
subtests additionally need real per-cycle event counting (HM §12.3).

### memA, memMisc, Tricond — all gate on HOLD (gap B1/C1) [the headline]
Three diagnostics fail/hang for the **same root cause: the Hold mechanism is
unimplemented**.
- **memA** (timeout): its multi-task memory *simulator* busy-polls for
  HOLD / reference-completion / `Pipe4'` status edges that never occur
  (`memory.c` HOLD no-op ~488; `dorado_pipe4_at` fixed baseline). Livelocks.
- **memMisc** (timeout): the ASRN/subtask fault test needs **HOLD-on-fault + a
  per-fault fault-task (task 15) handshake** for I/O-task references; ours holds
  nothing and wakes task 15 only once (`memory.c:495`, `cpu.c:~502`), so task 0
  spins on a fault count that never arrives. (Also: default 16 MW memory makes an
  earlier page-walk astronomically long; `DORADO_STORAGE_MODULES=1` skips to the
  real deadlock.)
- **Tricond** (FAIL @105, `STATE.ERRS`): it is actually the **Hold + task-sim**
  diagnostic; `Hold&TaskSim ← B` (FF=0o154) is a no-op (`cpu.c:1522`) with no
  Hold/TaskSim state, so the saved-state check reads garbage.

**Implication:** implementing Hold (the cycle-accurate-timing gap B1/C1) now has
**three self-checking real-hardware regression tests** — a far better gate than
"do games render."

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

Debug the **kernel** failure first: it is core datapath, runs with the least
setup, and fails at step 79. Use `RUNDIAG_TRAIL=1` + the kernel sources
(`chm/doradomicrocode/kernelsources/{Preamble,Kernel,Kernel1..5,KernelALU,Postamble}.mc`)
to identify whether the failing check is a real engine bug or a missing setup
the runner should apply. Then triage the others.
