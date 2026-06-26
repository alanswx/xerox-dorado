# HANDOFF — Dorado hardware diagnostics + implementing Hold (gap B1)

**Date: 2026-06-23. Branch: `fidelity-timing`. All gates green.**
**You are picking this up after a context reset. Read the "Read first" docs
below before writing any code.**

> **STATUS UPDATE (2026-06-25) — much of this doc's "to-do" is now DONE.**
> The single source of truth for per-diagnostic status is
> [`docs/running-diagnostics.md`](running-diagnostics.md) (kept current); this
> banner is the short version. **2 of the suite now fully PASS:**
> - **kernel — PASS** (DONE @3,765,457 steps). Fixed: RBase, the bogus
>   `(T>>8)&7` shifter case (HM §3.11), `Wakeup[n]` 2-cycle latency (HM p27),
>   and the §3.12 **TASKSIM** counter (`Hold&TaskSim←B`, FF=0o154) — with the
>   **enable-bit gate** (`taskSim[0]`=0o10): only count while enabled, which is
>   also the memA fix below.
> - **memA — PASS** (DONE @220,787,595 steps). `CATUPADDRERR` was NOT a
>   cache-address fidelity gap (the diagnostic constants are octal; no 2-bit
>   offset). It was the TASKSIM enable-bit bug: a stale re-arm value kept waking
>   the §3.12 sim task (0o12), whose `noRef`+`useMcrV` STORE clobbered the
>   cache-addressing test. Source mirrored at `chm/.../memASource/`.
>
> **eventCounters** — genIO loopback fixed; the `KFAULT3` setup fault fixed
> (`IFUTest←B(0)` must NOT enable the periodic junk timer, HM §8.3); HM §4.11
> **event counters** implemented (per-cycle EventCntA/B + EmuOrFT/tasksAll
> gating) so **eventTrue PASSES**; `uinstr_reads_md` corrected (ASEL=2/3 read Md
> only when FF[0:1]=0). Now fails at `eventHold` — the loop reads no Md and its
> Hold comes from the **HM §5.4 reference/port-busy Hold coupled to the sim-task
> cadence**, i.e. the holistic cycle-accurate-timing project (below). Sources at
> `chm/.../eventCountersSource/`.
>
> **Ifu (Complex/Simple)** — scoped, sources mirrored at `chm/.../IfuSource/`
> (README). `IFUEXCEPTIONERR` needs the **cycle-accurate IFU test-mode pipeline**
> (`IFUTest←`/`ifuTick` single-stepping F/G/J/H/M) — its own large effort, not a
> contained exception latch. **memMisc** (fault-task + Pipe4) and **Tricond**
> (disk state mufflers) still pending.
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
an Alto, can't validate Dorado-specific behavior). We ran all six, fixed the
first kernel bug (79 → 1.34M steps), and root-caused the rest. The standout
result: **three diagnostics (memA, memMisc, Tricond) all fail for one reason —
the Hold mechanism (gap B1/C1) is unimplemented.** So they are now the
**self-checking regression tests** for the Hold work, which is the core of the
cycle-accurate-timing project.

**Your job: implement Hold (gap B1), using memA / memMisc / Tricond as the gate.**

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

The three Hold regression tests:
```
build/rundiag '../chm/dorado/expanded/MEMA.DM!18_/memA.mb'        BEGIN DONE ERR   # TIMEOUT now
build/rundiag '../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb'  BEGIN DONE ERR   # TIMEOUT now
build/rundiag '../chm/dorado/expanded/Tricond.dm!5_/Tricond.mb'   BEGIN DONE ERR   # FAIL @105 now
```
For the memory diagnostics, the default 16 MW memory makes an early page-walk
astronomically long — use `DORADO_STORAGE_MODULES=1` (4 MW) to reach the real
Hold deadlock fast.

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
- Galaxian headless = **121602** non-255 px:
  `./build/dorado --eb worlds/aemu.eb --eftp ../chm/bootfiles/Galaxian.boot!1 --cycles 250000000 --out /tmp/g.pgm`
- Cedar still boots to login (`make run-cedar`), NetExec in band.

## The other diagnostics — CURRENT (2026-06-25; see running-diagnostics.md)

- **kernel — PASS** (DONE @3,765,457). The §3.12 TASKSIM is now implemented
  with the `taskSim[0]`=0o10 **enable-bit gate** (count only while enabled).
- **eventCounters** — genIO + KFAULT3 setup fault + **eventTrue** fixed; fails
  at `eventHold` (HM §5.4 reference-Hold + sim-cadence — the timing project).
  The old "@674 GENIOERR2 / no event counting" note is obsolete: event counting
  (HM §4.11) is implemented; the GenIO loopback is the IFU-board GenOut→GenIn
  plug (`GenIn'`=EventCntA' reads back `GenOut`=EventCntB), NOT a counter mirror.
- **IfuComplex/IfuSimple** (FAIL @~1631 `IFUEXCEPTIONERR`): needs the
  cycle-accurate IFU **test-mode pipeline** (`IFUTest←`/`ifuTick` single-step of
  F/G/J/H/M), not just an exception latch. Sources mirrored at
  `chm/doradosource/diagnostics/IfuSource/` (+ README + roadmap).
- **memMisc** (TIMEOUT) and **Tricond** (`STATE.ERRS`) — still pending
  (fault-task+Pipe4; Trident disk-state mufflers).

## Key files

- `dorado/src/rundiag.c` — the diagnostic runner (RBase 0; `RUNDIAG_RBASE`,
  `RUNDIAG_TRAIL`).
- `dorado/src/cpu.c` — engine; Hold block ~3562, fault-task wake ~502,
  `Hold&TaskSim` no-op ~1522, `shifter_output` ~1992 (LDF hack ~2003).
- `dorado/src/memory.c` — Hold no-op ~488, `dorado_pipe4_at` ~282,
  `last_ref_latency` ~1030.
- Diagnostics: `chm/dorado/expanded/<name>.dm!NN_/<name>.mb`; kernel sources at
  `chm/doradomicrocode/kernelsources/`.
