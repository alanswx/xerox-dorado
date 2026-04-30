---
gap: B6
status: landed
---

# B6 — A[12:15] ← FF[4:7] override

## Source citations

- **HM Table 11a** (FA=0 FB=0/1): "A[12:15] ← FF[4:7]". The low 4
  bits of the A bus are replaced by FF[4:7] before the value reaches
  the ALU and Mar.
- **HM §3.6** (FF gating): FF is interpreted as a function only when
  BSEL is not selecting a constant (BSEL <= 3) and JCN is not a long
  goto/call. Already encoded in `ff_decode_ok` /
  `ff_full_function_ok`.
- **HM Table 8a/8b**: for ASEL ∈ {0..3}, FF[0:1] is a memory-ref
  alt-A-source selector (Md/Id/Q/T) — FF is not a function in that
  case. The override therefore only applies when ASEL > 3. Existing
  guard `if (u->asel <= 3) return 0;` in `ff_full_function_ok`
  covers this.

## Spec

When the FF field is interpreted as a Table-11a function and the
function code matches FA=0 with FB ∈ {0, 1}, replace the low 4 bits
of the A bus with FF[4:7] (= `ff & 0xF` in C-LSB).

The override fires before the ALU sees A. ASEL still selects the
upper 12 bits' source (RM/STK, T, Id, shifter); only A[12:15] is
overwritten.

`FB == 0` and `FB == 1` both encode the override; the manual lists
them as paired variants. We treat them identically until a
microcode-level distinction surfaces.

## Cross-check / deltas

None observed. The fix is small enough that no microcode source
required cross-checking.

## Implementation

- `dorado/src/cpu.c`:
  - new helper `ff_a_low_override(u)` — gates on
    `ff_full_function_ok` then checks `fa==0 && fb<=1`.
  - in the main step path immediately after `a_bus()`, apply
    `a = (a & 0xFFF0) | (ff & 0xF)` when override fires.
  - the legacy comment in `ff_apply_post` ("already handled at
    A-bus time, but harmless here") replaced with one that points
    at `ff_a_low_override`.

## Verification

- **synthetic test** `tests/test_cpu.c::test_a_low_ff_override`:
  - FA=0 FB=0 with BSEL=T (function gated): T ← 0xAAA5 (override fires).
  - FA=0 FB=0 with BSEL=constant: T ← 0xAAAA (override suppressed).
  - FA=0 FB=2 with BSEL=T: T ← 0xAAAA (different FB; no override).
  - FA=0 FB=1 with BSEL=T: T ← 0xAAAD (FB[2]=1 lifts the high bit
    of FF[4:7] to 0xD).
- All four cases pass. `make test` (`./build/test_cpu`) clean.
