# Performance plan — closing the 2.2x gap to real hardware

Written 2026-08-04, after the clock audit.

## Where we actually are

Measured in the machine's own unit (`dorado` prints it at the end of every
run; see `dorado_machine_uinstructions`):

| path | microinstr/s | vs real 16.67 MIPS Dorado |
|---|---|---|
| Alto (Galaxian) | 7.6 M | **0.46x** |
| Cedar desktop | 6.4 M | **0.39x** |

The older "1.75x / 1.51x faster than the hardware" claim was `bb.cycles`
per second — the BaseBoard 6502 counter, 3.70 per Dorado microinstruction —
read as microinstructions. We are slower than the machine by a bit over 2x,
which is what a PARC veteran meant by "it feels slow".

**Target: 16.7 M microinstructions/s on the Alto path**, i.e. 2.2x from
here. That is real-time parity, not a stretch goal — the Dorado's
microcycle is 60 ns and matching it is the point of the exercise.

## Where the time goes

Grouped by what the work is *for*, from `sample` on a Galaxian run
(2026-08-04, M4 Max). Percentages are of ~11k visible samples, so treat
them as ±2 points, not exact.

| group | share | components |
|---|---|---|
| Interpreter proper | ~23% | `execute_uinstr` plus its semantics helpers |
| **Per-cycle I/O polling** | **~35-40%** | `run_until` self 15%, `machine_store_va` 13%, `ethernet_wakeup_mask` 3%, `display_dwt_wakeup` 1.3%, BaseBoard ~4% |
| Task scheduling | ~5% | `task_schedule`, recomputed every microinstruction |
| Memory translation | ~7% | `map_index`, `memory_ref_task`, `visible_word_at_va` |

The middle row is the opportunity. Nothing in it changes at 60 ns
granularity: display fields are 16.7 ms, keyboard and mouse are human-scale,
the BaseBoard 6502 legitimately runs ~62x slower than we step it, and
Ethernet packets are microseconds at best. We ask, 7.6 million times a
second, questions whose answers change a few hundred times a second.

## The phases, cheapest and safest first

### Phase 0 — compiler flags (no behavioural risk)

`-O3`, LTO, `-mcpu=apple-m4`, then PGO. Interpreter dispatch loops
typically gain 10-25% from these alone. Costs an afternoon, cannot change
emulator semantics, and may move the target before a line of emulator code
is written. **Do this first so later phases are measured against the right
baseline.**

### Phase 1 — cache the task scheduler (~5%)

`task_schedule` recomputes best-next-task every microinstruction, but the
wakeup set changes rarely. Cache the result, invalidate on
wakeup/block/unblock. Behaviour-neutral by construction: the cached value is
a pure function of state already tracked.

### Phase 2 — predecode the microinstruction (~10-15%)

The IM is decoded into fields already, but each execution re-derives FF
field meaning, LC routing, ALUFM indexing and branch-target computation
through branchy dispatch. Precompute a per-IM-word recipe — a flags word
saying which post-effects apply — keyed by IM address.

**Constraint:** keep this strictly an emulator-side cache. `CLAUDE.md`'s
Phase-2 rule is to avoid constructs that will not map to RTL (function
pointers per opcode, implementation-defined bit-field layout) in code
intended to become Verilog. A lookup table keyed by IM address does not
change the model's structure; a dispatch table of function pointers
replacing the field decode would. Stay on the first side of that line.

### Phase 3 — VA→physical translation cache (~5%)

Consecutive references overwhelmingly hit the same page. Even a one-entry
cache usually pays.

### Phase 4 — deadline scheduling instead of per-cycle polling (~1.5x, the big one)

Replace "do this every cycle" with "next event at cycle N": one compare per
cycle against the nearest deadline. Collapses input seeding, the ethernet
poll, the display wakeup check and the BaseBoard step onto a single branch.

The Lisp path already demonstrates the principle — it gates its seeding to
one per 16384 cycles — while the Alto path re-seeds ~10 cells *per
microinstruction*, each with a VA translate and a cache invalidation.

### Phase 5 — decouple the master clock from the 6502 (~4% plus fidelity)

`bb.cycles` is derived from 6502 instruction lengths, which is why every
cycle number in the tree is in 6502 units and why the 3.70 factor exists.
Making the time base microinstructions lets the BaseBoard run at its real
rate (one 6502 instruction per ~62 microinstructions rather than per 1).

Compounding phases 1-4: 1.5 x 1.25 x 1.05 ≈ **2.0x**, before Phase 0.

---

## Caveats — read before starting any phase

**1. The estimates are one profile, one path, one machine.** Galaxian on an
M4 Max. Cedar spends real time in the PDI and STP paths Galaxian never
touches, and its 0.39x may have a different composition entirely. Profile
Cedar separately before assuming this ranking transfers. Do not quote these
percentages as measurements of anything but the Alto path.

**2. Byte-identical framebuffers are the gate, not pixel counts.** `cmp`
two `.pgm` files. A pixel count cannot tell a correct screen from a
differently-wrong one, and this project has shipped at least three
regressions that a count waved through.

**3. Phase 4 is the dangerous one, and it is dangerous in a specific way.**
Changing *when* I/O is observed is exactly the class of change that has
desynced boots here before (memory: cycle-accurate timing; several worlds
are sensitive to I/O cadence, and the EFTP RxOn-clear is gated to the Cedar
path precisely because ungating it stalls the Alto boot). A deadline
scheduler must reproduce the same *observable* cadence, not merely a
similar one. Budget for the possibility that some polls cannot be moved.

**4. "Byte-identical on Galaxian" is not sufficient for Phase 4.** The
worlds exercise different I/O. Minimum gate set before believing a polling
change: Galaxian (Alto/ethernet), `verify-cedar-desktop` (Cedar/STP/disk),
`verify-lisp` (Interlisp/Leaf), `verify-smalltalk`, `verify-alto-disk`.
Several are slow; that is the price of touching this.

**5. Do not tune against a run with any `DORADO_*` variable set.** Setting
any of them disables the trace-flag fast path, which is itself a
double-digit effect. Gate runs set `DORADO_PDI_IGNORE_LABEL_FLAGS` and
`DORADO_FAKE_TIME`, so a "before" measured under a gate and an "after"
measured bare will lie to you.

**6. Snapshot ABI.** Any new `dorado_machine`/`dorado_cpu` member breaks
every baked checkpoint. Prefer file-scope statics for caches, as the
existing code does for the keyboard queue and display-active memo, and run
`make verify-snapshot-abi` after anything structural.

**7. `cpu->cycles` is snapshotted.** A restored checkpoint carries its
bake's microinstruction count. Measure the delta from run start or a
restored Cedar desktop reports "14.7x real hardware", as it did before this
was fixed.

**8. Speedups can be real and still not felt.** Interactive "feel" is
frame pacing as much as throughput. If the SDL/browser frontends pace to
wall time, raising throughput past real-time changes nothing a user
notices. Check the frontends' pacing before concluding a phase failed —
and, symmetrically, before claiming one succeeded.

**9. Stop when the gate says stop.** If a phase cannot be made
byte-identical across the gate set, it does not ship, however good the
number is. Record what it was worth and move on.

---

## Progress

| phase | status | measured |
|---|---|---|
| 0 — compiler flags | **DONE** | **1.95x — 0.52x → 1.02x real hardware** |
| 1 — task-schedule cache | not started | — |
| 2 — predecode | not started | — |
| 3 — translation cache | not started | — |
| 4 — deadline scheduling | not started | — |
| 5 — master-clock decoupling | not started | — |

### Phase 0 result (2026-08-04)

Phase 0 alone hit the target. The Alto path crossed real-time parity with
no emulator code changed, every step byte-identical:

| build | Galaxian 1 B cycles | vs real Dorado |
|---|---|---|
| `-O2 -g` (old default) | 30.9 s | 0.52x |
| `-O3 -g` | 30.1 s | 0.54x |
| `-O3 -g -mcpu=apple-m4` | 30.4 s | 0.53x |
| `-O3 -flto` (**new default**) | 27.1 s | 0.60x |
| `-O3 -flto` + PGO (`make pgo`) | **15.8 s** | **1.02x** |

Findings worth keeping:

- **PGO is the whole story** — 1.7x on top of LTO. An interpreter dispatch
  loop is close to the ideal case: the branch pattern is stable and
  completely opaque to static heuristics.
- **`-mcpu=apple-m4` does not help** (marginally worse than plain `-O3`)
  and would make the build host-specific. Deliberately not used.
- **LTO is worth 11.9%** on its own and costs nothing, so it is the
  unconditional default. PGO is `make pgo` rather than the default: it
  needs a two-stage build and leaves a `.profdata` artifact, and CI does
  not need it.
- The profile is trained on Galaxian. The Cedar path still gained
  (0.39x → 0.50x), so it is not pathologically overfit — but retrain via
  `PGO_TRAIN` if you are tuning for Cedar or Lisp.

Gates: Galaxian byte-identical against the `-O2` baseline at every step,
`make test` 193 assertions, `verify-snapshot-abi` 15/15,
`verify-lisp` 209,145 px.

**Phases 1-5 are consequently not urgent.** The remaining work buys
headroom (Cedar is still at 0.50x, and the browser build gets none of
this), not parity on the Alto path. Re-derive the profile before starting
Phase 1 — the ranking above was measured on an `-O2` build and PGO will
have reshuffled it.

### A cost incurred getting here

`make clean` is `rm -rf build`, and `build/good-packs/` lives inside it —
the directory `dorado/CLAUDE.md` tells you to put expensive artifacts in
*because it persists*. A flag sweep that cleaned between builds destroyed
every local checkpoint. Recovered: everything tracked in
`snapshot-assets/` and `web-assets/` (all shipped state was safe), plus
the Lyric pack, which is disk media and therefore ABI-independent. Lost
and rebaked: the native Lyric snapshot. Not recovered: the several
`cedar-sil-*`, `cedar-clean-*`, `cedar-schematic-*` and `cedar-iago-*`
working images, which were experiment scratch.

`clean` now preserves `good-packs`; `clean-everything` is the explicit
opt-in, and `clean-objs` is what a build loop should use. The instruction
and the tool contradicted each other and the tool was wrong.

Baseline for all comparisons (2026-08-04, M4 Max, `-O2 -g`, clean
environment):

```
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' \
    --cycles 1000000000 --out /tmp/perf.pgm
270,396,790 microinstructions = 16.22 s of Dorado time in ~35.4 s CPU = 0.46x
```
