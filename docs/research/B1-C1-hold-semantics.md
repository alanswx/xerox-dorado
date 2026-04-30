---
gap: B1, C1
status: research (deferred — large architectural change)
---

# B1 / C1 — Hold semantics + deferred memory references

This is the largest gap in the punch list. It's deferred from this
session because partially-implemented Hold can break more than it
fixes. This note captures the spec, the implementation plan, and the
microcode evidence so a future focused session can land it cleanly.

## Source citations

### HM canon

- **HM §4 "Hold"** describes the Hold latch and the conditions that
  raise it.
- **HM §5 "Memory Section"** + **Figure 9** describes the memory
  pipeline stages and the latency that drives Hold-on-Md-read.
- **HM Table 17** (Memory Control Register) defines `disHold`
  (per `EMemDefs.mc`: `mcr.disHold = b9 = LSB bit 6`).

### Microcode evidence

- **`chm/dorado/expanded/BootstrapSources.dm/InitialSubrs.mc`**
  reveals the boot-stage protocol:
  - `SetMCR` (line 51): comment `LoadMCR >8 cycles after last mem
    op`. Sets up a 6-cycle delay (`Cnt_ 6S, Branch[., Cnt#0&-1]`) so
    8+ total cycles pass before `LoadMCR[T,T]` settles.
  - `LongWait` (line 60): generic busy-loop `T_ T-1, Branch[., ALU#0]`.
  - `ClearCacheFlags` (line 68): explicitly sets `mcr.disHold` so
    references that would normally Hold instead complete immediately
    while microcode handles cache-row scanning. The cycle counts in
    `Call[LongWait]` invocations are doing manually what Hold would
    do automatically.
  - `PresetMap` (line 113): same pattern — `mcr.disHold` set, then
    explicit `LongWait` between fetches.
- **`InitialMain.mc`** line 67: `Initial1` loads MCR with `noWake +
  disHold` immediately after the IFU reset. **Boot-stage code
  intentionally runs without Hold** — the cycle-counted waits are
  written into the microcode.
- **`InitialMain.mc`** line 234: `T_ mcr.noWake, Call[SetMCR];
  * Now allow holds, etc., but … no wakeups`. Once Initial finishes
  hardware init, it re-enables Hold for the emulator that's about to
  run.

This explains why **probe_full_boot_with_bootstrap progresses without
a Hold model** but **probe_aemu loops forever** — AEmu (and Mesa,
Cedar, etc.) run with Hold enabled and rely on the stall behavior.

### Schematics + IBIP doc

- **`indigo/daffodil/ibip/doc/ibip-bus-ifu-clockhold.dm!1_/`** (CHM
  cross-reference hit; not yet pulled) — IBIP-era doc on bus / IFU /
  clock / Hold timing. Promising lead for cycle-accurate timing.
- **MEMC / MEMD / MEMX schematics** in `DoradoDocs/schematics/` —
  trace the Hold signal generators (cache-miss-FF, Pipe-full-FF,
  StkError-FF, IFUMapFault-FF).
- **ProcH / ProcL schematics** — show how the Hold input gates the
  microengine clock.

## Spec

### Hold raises when (per HM §4)

1. The current microinstruction reads `Md` (BSEL=0, ALUF reads B,
   etc.) before the most-recent Fetch's data has propagated through
   the cache → Pd → Md latch.
2. The processor's pending Pipe is full (more refs in flight than
   the 16-entry Pipe can track).
3. `FreezeBC` was issued and the deferred consumer hasn't fired yet.
4. `StkError` (over/underflow) is set.
5. IFU map fault is concurrent with an IFUJump.

While Hold is asserted:
- The microinstruction is **re-issued** rather than completing.
- Tasking is suspended (no task switch out of the held task).
- Counters (Cnt, ShC) do not advance.

### `mcr.disHold` (b9 / LSB bit 6, per `EMemDefs.mc`)

When set, all Hold sources are gated off — the engine never stalls.
Microcode that needs to wait for memory data must use cycle counters
(`Cnt_ N, Branch[., Cnt#0&-1]` or `Call[LongWait]`).

### Md latency

HM §5: cache hit → Md available next cycle. Cache miss → ~28 cycles
to fill from storage. With `mcr.fdMiss` set, every reference acts
as a miss (test mode).

## Implementation plan

### Phase B1.A — minimal Md-latency stall

1. Add to `dorado_cpu`:
   - `uint64_t md_ready_cycle` — cycle at which Md is valid for the
     current task.
   - per-task `md_ready_cycle[16]` (Hold is per-task per HM §4).
2. After every `dorado_memory_ref(DM_REF_FETCH/IFETCH/LONGFETCH)`,
   set `md_ready_cycle = cpu->cycles + N` where N is 1 for cache
   hit, larger for miss. `dorado_memory_ref` would have to expose
   the hit/miss outcome via a new field on `dorado_memory`.
3. In `dorado_cpu_step`, before issuing a microinstruction that
   reads Md (BSEL=0), check `cpu->cycles < md_ready_cycle[ctask]`.
   If so:
   - If `mcr.disHold`, ignore — microcode is responsible for
     waiting (the fetch may return stale Md, but boot-stage
     microcode tolerates this).
   - Else, **don't advance PC**, don't consume tasks, just
     increment `cycles` and return.

### Phase B1.B — Pipe-full stall

Track in-flight references in a small queue. When > 16 outstanding,
Hold the engine until a slot frees.

### Phase B1.C — StkError + FreezeBC + IFU map fault

Each source sets a per-task Hold flipflop. Hold goes high when ANY
source is asserted (gated by `mcr.disHold`).

### Test plan

- **Phase B1.A test**: synthetic two-instruction microprogram —
  first instruction does Fetch, second instruction reads Md.
  Verify the second instruction stalls for the expected Md latency
  when `disHold=0`, completes immediately when `disHold=1`.
- **Phase B1.A real microcode**: re-run `probe_aemu` — the
  `LRTYPETABLE → LRTYPEIM → LRLOOPTOFF → TOFFRET` loop should
  progress past the Md-stale wait once Hold is in.
- **No regressions on boot probes**: `probe_full_boot_with_bootstrap`
  runs with `disHold=1` for most of init, then enables Hold near
  the emulator handoff (line 234 of `InitialMain.mc`). Should
  behave identically to today during boot, then differently after
  emulator handoff.

## Why deferred

This is a substantial architectural change touching:
- `dorado_cpu` state structure (per-task fields).
- `dorado_memory_ref`'s return contract (now exposes hit/miss).
- `dorado_cpu_step`'s control flow (must support stall + retry).
- New tests (synthetic + behavioral).

Estimate: 1 focused session, with `EMemDefs.mc` /
`InitialSubrs.mc` / `InitialMain.mc` already on hand as the
canonical specs and `ibip-bus-ifu-clockhold.dm` to fetch as a
cross-check.

## Pre-conditions for landing

- `dorado_mcr_dishold` getter exists ✓ (gap C6, landed).
- `dorado_pipe4_set_error(MAP_TROUBLE)` exists ✓ (gap C2, landed) —
  Pipe4 syndrome bits are exposed for any cache-miss fault that
  Hold catches.
- A `disHold`-aware path through cpu.c that consults
  `dorado_mcr_dishold(mem)` before stalling.
