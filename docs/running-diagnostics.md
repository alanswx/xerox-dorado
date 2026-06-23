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

## Baseline (2026-06-23, first run)

| diagnostic     | result  | where |
|----------------|---------|-------|
| kernel         | FAIL    | step 79, first ALU/branch-condition sub-test (`ALUEQ0RT`), branches to ERR at 0o213 |
| IfuComplex     | FAIL    | step 1622, `IFUEXCEPTIONERR` |
| memA           | TIMEOUT | pc 0o2433 (no DONE/ERR in 2M steps) |
| memMisc        | TIMEOUT | pc 0o4253 |
| eventCounters  | FAIL    | step 674, `GENIOERR2` (general-I/O setup) |
| Tricond        | FAIL    | step 105, `STATE.ERRS` |

These are a mix of (a) **real engine discrepancies** (the kernel ALU/branch test
is the most fundamental — minimal setup, fails fastest — and is the first thing
to debug), (b) **runner setup gaps** (memA/memMisc time out — the memory
diagnostics likely need cache/map/config setup our minimal recipe doesn't do
yet; eventCounters/Tricond need device setup), and (c) features beyond the
current setup. Each is a concrete, Dorado-grounded validation target — and
unlike ContrAlto, these catch Dorado-specific datapath/timing bugs.

## Next

Debug the **kernel** failure first: it is core datapath, runs with the least
setup, and fails at step 79. Use `RUNDIAG_TRAIL=1` + the kernel sources
(`chm/doradomicrocode/kernelsources/{Preamble,Kernel,Kernel1..5,KernelALU,Postamble}.mc`)
to identify whether the failing check is a real engine bug or a missing setup
the runner should apply. Then triage the others.
