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

| diagnostic     | result | where | diagnosis |
|----------------|--------|-------|-----------|
| kernel         | FAIL @1.34M steps (was @79) | shifter test `RLSH`, pc 0o2604 | **First bug FIXED** (runner RBase 0, not 017). Next: masked field-shift (RF←/WF← ShC) path |
| IfuComplex     | FAIL @1622 | `IFUEXCEPTIONERR` | (agent investigating) |
| memA           | TIMEOUT | pc 0o2433 | (agent investigating) |
| memMisc        | TIMEOUT | pc 0o4253 | (agent investigating) |
| eventCounters  | FAIL @674 | `GENIOERR2` | General-IO/event-counter stub: `event_cnt_a` (GenIn) never tracks GenOut (loopback); counters never increment (HM §12.3). Small fix + a counting feature |
| Tricond        | FAIL @105 | `STATE.ERRS` | (agent investigating) |

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

### eventCounters (diagnosed)
General-IO subtest `GENIO`: it writes a pattern to GenOut (EventCntB) and expects
GenIn (EventCntA) to track it (backpanel loopback). Our `event_cnt_a` is a
write-never stub (`B←EventCntA'` always returns ~0). Fix: mirror GenOut→GenIn in
general-IO mode (cpu.c ~927/1335). The later subtests need real per-cycle event
counting (HM §12.3 A/B event classes) — a separate, larger feature.

## Next

Debug the **kernel** failure first: it is core datapath, runs with the least
setup, and fails at step 79. Use `RUNDIAG_TRAIL=1` + the kernel sources
(`chm/doradomicrocode/kernelsources/{Preamble,Kernel,Kernel1..5,KernelALU,Postamble}.mc`)
to identify whether the failing check is a real engine bug or a missing setup
the runner should apply. Then triage the others.
