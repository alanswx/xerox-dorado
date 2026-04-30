---
gap: B7
status: landed
---

# B7 — `ff_full_function_ok` gating audit

The TODO at `cpu.c:450` flagged a worry that "Unknown FF values that
*would* override B fall through to the BSEL path." Investigation
confirmed the existing gating is in fact complete; only the comment
was misleading.

## Source citations

- **HM §3.6**: FF is interpreted as a function only when BSEL is not
  selecting a constant (BSEL[2:7] are all that's needed for
  primary-source decode; BSEL >= 4 selects one of the four constant
  forms `0,,FF`, `0o377,,FF`, `FF,,0`, `FF,,0o377`) and JCN does not
  encode a long branch.
- **HM Table 8a/8b**: For ASEL ∈ {0..3} (memory references), FF[0:1]
  selects the alternate A source (`Md`, `Id`, `Q`, `T`); FF[2:7] may
  encode a branch condition (HM Table 13). The whole 8-bit FF is
  *not* a Table-11 function.
- **HM Table 11**: All B-bus overrides live at FA=1, FB ∈ {6, 7}.
  Other (FA, FB) pairs in Table 11 are post-ALU side effects, not
  B sources.

## Spec audit

The current code path:

```
ff_decode_ok(u):
  if BSEL >= 4 → FF not a function       ✓ (HM §3.6)
  if JCN top4 == 0 and JCN top1 == 0 (long) → FF not a function  ✓

ff_full_function_ok(u):
  if !ff_decode_ok → false                ✓
  if ASEL <= 3 → false (memory-ref form)  ✓ (HM Table 8a/8b)
  else true

ff_override_b(u):
  if !ff_full_function_ok → no override   ✓
  if FA != 1 → no override                ✓ (HM Table 11)
  switch FB:
    case 6 (Pipe / Config / FaultInfo)    ✓ FC 0..7 all enumerated
    case 7 (CPReg / Link / IFUM / event)  ✓ FC 0..7 all enumerated
  default unreachable
```

All possible B-source override cases are caught. The TODO was
written defensively but did not correspond to a real gap.

## Implementation

- `dorado/src/cpu.c`: replace the TODO comment with a positive
  statement of the gating invariant (HM Table 11 only places
  B-source overrides at FA=1 FB∈{6,7}; FA != 1 short-circuit is
  complete; FB switches enumerate all 16 cases).

## Verification

- All existing tests pass (`./build/test_cpu` clean).
- No behavior change — comment only.
