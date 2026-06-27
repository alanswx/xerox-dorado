# HANDOFF — Dorado hardware diagnostics + implementing Hold (gap B1)

**Date: 2026-06-23; status banner updated 2026-06-26. Branch:
`fidelity-timing`. Normal unit/boot gates green; diagnostic status is in
`docs/running-diagnostics.md`.**
**You are picking this up after a context reset. Read the "Read first" docs
below before writing any code.**

> **STATUS UPDATE (2026-06-26) — this doc's original diagnostic to-do is
> obsolete.**
> The single source of truth is
> [`docs/running-diagnostics.md`](running-diagnostics.md). Current verified
> passes: kernel, eventCounters, memMisc, IfuSimple, IfuComplex, TriconD
> no-pack, and the memA D/X/S slices. TriconD must be run as DSK task `14` with
> `RUNDIAG_DISK=1` and done label `TESTOK-WITHOUT-DISK`; memA's full S-board
> chaos/burn-in path is not a quick gate.
>
> **Build note:** `make` now builds `build/rundiag`; header deps track all
> objects (commit 43192fd) — no more stale-object crashes.

> **CORRECTION (2026-06-23, later): the premise of this doc is wrong.** Reading
> the original PARC `.mc` diagnostic sources (now mirrored at
> `chm/doradosource/diagnostics/`) shows the three diagnostics do **NOT** all
> gate on one "Hold (gap B1)" mechanism. They need three *different* things:
> **Tricond** is the Trident **disk-controller** muffler diagnostic (`KSTATE`),
> nothing to do with Hold; **memA** exercises the HM **§3.12 Hold&TaskSim debug
> simulator** (HOLDSIM shift register + TASKSIM wakeup counter, FF=0o154) feeding
> a memory-simulator subtest; **memMisc** (`aMapTest`) **explicitly disables the
> hold simulator** and instead needs the **fault-task (task 15) per-fault
> handshake + accurate Pipe4 status bits**. None needs the memory-miss engine
> stall. See `docs/running-diagnostics.md` → "Corrected per-diagnostic
> diagnosis" for the source citations. The rest of this doc is kept for the
> code-location pointers, but ignore its "all gate on Hold" framing.

## TL;DR

We got PARC's original **Dorado hardware diagnostics running on our microengine**
(`build/rundiag`) — the one real-Dorado-grounded validation oracle (ContrAlto is
an Alto, and cannot validate Dorado-specific behavior). The original "all fail
for Hold" framing is obsolete; use `running-diagnostics.md` as the live map.

## Read first (in this order)

1. [`docs/running-diagnostics.md`](running-diagnostics.md) — how to run the
   diagnostics, the per-diagnostic root-cause table, and the full diagnosis of
   all six. **This is your map.**
2. [`docs/timing-project-scope.md`](timing-project-scope.md) — the scoped
   cycle-accurate-timing project (Hold is the central gap; the go/no-go).
3. [`docs/memory-architecture.md`](memory-architecture.md) — the memory
   subsystem reference (cache/Map/Pipe/BR), and HM §4–§5 Hold semantics.
4. [`docs/cycle-accurate-timing-plan.md`](cycle-accurate-timing-plan.md) — the
   detailed working log (Hold = gap B1, the prior `DORADO_HOLD` experiment).
5. [`docs/contralto-oracle-validity.md`](contralto-oracle-validity.md) — why the
   diagnostics (not ContrAlto) are the right oracle for Dorado-specific timing.
6. The **Hardware Manual** `DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`
   §4 (Hold/tasking) and §5 (memory, pp. 41–42). Treat as canon.

Memory notes (auto-loaded): `dorado-diagnostics-runner`,
`cycle-accurate-timing-major-fix`, `contralto-oracle-only-architectural`.

## How to run the oracle (your inner loop)

```
cd dorado && make build/rundiag
build/rundiag <diag.mb> [entry=BEGIN] [done=DONE] [err=ERR] [maxsteps]
RUNDIAG_TRAIL=1 build/rundiag ... 2>&1   # dump last 48 PCs (T/flags/symbol) on non-PASS
```
PASS = reach `DONE`; FAIL = reach `ERR` (the `ERROR` macro = `BRANCH[ERR]`);
HALT = unimplemented op (names it); TIMEOUT = stuck (usually a wait loop).

Diagnostic smoke commands are maintained in
[`docs/running-diagnostics.md`](running-diagnostics.md). Do not copy the older
commands from this historical handoff; several diagnostics need specific task,
disk, done-label, or memory-sizing knobs.

## What "implement Hold" means (the concrete gaps)

A **partial Hold model already exists, gated off** — start from it:
`dorado/src/cpu.c` ~line 3562 (the `DORADO_HOLD` block in `execute_uinstr`):
it converts an Md-consuming instruction to a jump-to-self when the fetch latency
hasn't elapsed (`task_md_ready[]`, `uinstr_reads_md()`), letting a higher task
run. It is gated by `getenv("DORADO_HOLD")` and is incomplete. The gaps the three
diagnostics need (cited by the diagnosis agents):

1. **`dorado/src/memory.c` ~488–495** — "Hold itself is gap B1 — currently a
   no-op." The engine never actually stalls a reference. **memA** busy-polls for
   the HOLD / reference-completion / `Pipe4'` status edges this should produce.
2. **`dorado/src/cpu.c` ~502** — the fault-task wake is approximate ("we model
   the observable fault-task wake by raising task 15 when…") and fires **once**,
   not per fault. **memMisc** needs **HOLD-on-fault + a per-fault fault-task
   (task 15) handshake** for I/O-task (subtask) references: hold the faulting
   subtask, wake task 15 on every uncleared fault, update `fault_count` /
   `fault_first_srn`, release on `dorado_fault_clear`, honor `dorado_mcr_dishold`.
3. **`dorado/src/cpu.c:1522** — `Hold&TaskSim ← B` (FF=0o154) is a `return pd`
   no-op, and there is **no Hold/TaskSim state** in `cpu.h`. **Tricond** writes
   this register, runs a hold-duration loop, then verifies the saved state. Add
   the state field(s), store on write, add the read-back B-source in
   `ff_override_b()`, and model the hold stall + task-sim that the loop counts.

Also relevant: `dorado_pipe4_at()` (`memory.c` ~282, gap C2) returns a fixed
baseline — memA reads `B←Pipe4'` (FF=0o165) expecting real per-reference status.

**Validate continuously** against the three diagnostics (lockstep deeper /
TIMEOUT→FAIL→PASS) AND the regression gates below at every step. The prior
`DORADO_WLAT` cadence experiment desynced the boot — expect Hold to interact with
boot timing; develop with `DORADO_HOLD`-style gating until it passes both the
diagnostics and the gates, then make it default.

## Regression gates (must stay green at every commit)

- `make test` → 12 suites pass.
- Galaxian headless = **121553** display-list pixels:
  `./build/dorado --eb worlds/aemu.eb --eftp ../chm/bootfiles/Galaxian.boot!1 --cycles 250000000 --out /tmp/g.pgm`
- Cedar still boots to login (`make run-cedar`), NetExec in band.

## The diagnostics — CURRENT (2026-06-26)

See [`docs/running-diagnostics.md`](running-diagnostics.md). Verified passing
commands are listed there with observed step counts for kernel, eventCounters,
memMisc, IfuSimple, IfuComplex, TriconD, and memA slices.

## Key files

- `dorado/src/rundiag.c` — the diagnostic runner (RBase 0; `RUNDIAG_RBASE`,
  `RUNDIAG_TRAIL`).
- `dorado/src/cpu.c` — engine; Hold block, fault-task wake,
  `Hold&TaskSim←B` decode/TASKSIM/HOLDSIM state, shifter path.
- `dorado/src/memory.c` — Hold no-op ~488, `dorado_pipe4_at` ~282,
  `last_ref_latency` ~1030.
- Diagnostics: `chm/dorado/expanded/<name>.dm!NN_/<name>.mb`; kernel sources at
  `chm/doradomicrocode/kernelsources/`.
