---
gap: B11
status: landed (state slots only — trap/tick semantics deferred)
---

# B11 — Breakpoint, parity, and event-counter state slots

## Source citations

- **HM §4.10** Breakpoints: `BrkIns←B` loads `Opcode ← B[0:7]` and
  sets `BrkPending`. The next IFU dispatch traps to the breakpoint
  vector.
- **HM §4.11** Event counters: `EventCntA` and `EventCntB` are
  16-bit counters that tick on selected events programmed via
  `InsSetorEvent←B` (the B[0]=0 mode).
- **HM Table 11c**: `B←EventCntA'` (FA=1 FB=7 FC=1),
  `B←EventCntB'` (FA=1 FB=7 FC=4), `EventCntB←B` (FA=1 FB=3 FC=1),
  `BrkIns←B` (FA=1 FB=3 FC=7), `InsSetorEvent←B` (FA=1 FB=3 FC=0).
- **HM §4.10** also describes a parity-error flipflop set by
  IFUM/IM/RM parity faults. We don't model parity at the bit level
  yet, so the state slot is provisional.

Authoritative implementation lives in **`Kernel.Press!6`** (the
kernel microcode listings) and **`MidasInternal.press!8`** (the
debugger's view of the breakpoint protocol). Both are PRESS-format
documents on `_cd8_/doradodocs/` per `docs/chm-archive.md` §8.
We have not yet pulled Press-to-PDF; once we do, the trap-vector
encoding and EventCnt control bits should be transcribed here and
the implementation completed.

## Spec

State slots added to `dorado_cpu`:

```
brk_pending           — set by BrkIns←B; no trap yet
brk_opcode            — high 8 bits of B captured by BrkIns←B
event_cnt_a           — 16-bit counter (no tick events yet)
event_cnt_b           — 16-bit counter
event_cnt_ctrl_lo     — low 8 bits saved when InsSetorEvent←B(B[0]=0)
event_cnt_ctrl_hi     — high 4 bits (B[4:7]) saved similarly
parity_error          — for future use
```

FF wiring:

| FF        | Operation                             | Behavior                                |
|-----------|----------------------------------------|------------------------------------------|
| 0o131     | `EventCntB ← B`                        | `event_cnt_b = B` (was no-op).           |
| 0o137     | `BrkIns ← B`                           | `brk_pending=1`; `brk_opcode = B[0:7]`. |
| 0o130     | `InsSetorEvent ← B` (B[0]=0)           | `event_cnt_ctrl_{lo,hi} = B[4:15]`.     |
| FA=1 FB=7 FC=1 | `B ← EventCntA'`                  | returns `~event_cnt_a` (was 0).         |
| FA=1 FB=7 FC=4 | `B ← EventCntB'`                  | returns `~event_cnt_b` (was 0).         |

Follow-up correction while debugging boot/display:

- HM Table 20 says `IFUReset` halts/clears the IFU pipeline, clears
  errors/testing features and `BrkPending`/`BrkIns`, and explicitly does
  not clear `InsSet` or the reschedule condition.
- `LoadRam.mc` relies on this: each IFUM item is addressed with
  `InsSetOrEvent←B` + `BrkIns←B`, and the load path executes `IFUReset`
  before the loaded emulator starts so the IFUM-addressing `BrkIns` state
  is not treated as a pending breakpoint substitution.

What is **not** yet implemented:

- Breakpoint trap on the next IFU dispatch (the `brk_pending` flag
  is set by `BrkIns←B`, cleared by `IFUReset`, but not yet consumed by
  opcode substitution/trap handling).
- Event-counter ticking. The control bits are saved but no event
  source increments the counters.
- Parity-error injection / IFU parity trap.

These pieces require either:
1. Disassembly of the trap vector layout from `Kernel.Press!6`, OR
2. Observation of real microcode using these features (currently
   none of our probes exercise them).

## Implementation

- `dorado/include/cpu.h`: 7 new state fields in `dorado_cpu`.
- `dorado/src/cpu.c`:
  - `B←EventCntA'`, `B←EventCntB'` return `~event_cnt_*`.
  - `EventCntB ← B` writes `event_cnt_b`.
  - `BrkIns ← B` sets `brk_pending` and `brk_opcode`.
  - `InsSetorEvent ← B` saves the control bits when B[0]=0.
  - `IFUReset` clears `brk_pending`/`brk_opcode` and halts the IFU pipeline
    without changing `ifu_insset`.

## Verification

- `tests/test_cpu.c::test_b11_event_cnt_brk_state` runs two
  microinstructions and verifies `event_cnt_b` and the BrkPending
  state landed correctly.
- Existing tests still pass.
