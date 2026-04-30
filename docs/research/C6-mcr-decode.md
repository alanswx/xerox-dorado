---
gap: C6
status: landed (partial — disHold getter added, disBR fixed; 0xFEE7 special-case still in place)
---

# C6 — MCR active-low decode

## Source citations

- **`chm/doradomicrocode/doradomicrocodesources/EMemDefs.mc`**
  (Title: *September 23, 1981 4:15 PM, Taft*) — the canonical MCR
  bit definitions. Manual MSB-first numbering; `b0` is the high
  bit (LSB-bit 15), `b15` the low bit (LSB-bit 0).
  ```
  Set[mcr.mcrVshift, 13];           * left shift "column" for position in mcr
  MC[mcr.noWake, b15];              * no faulttask wakeups
  MC[mcr.noReportSE, b14];          * don't report single errors
  MC[mcr.noRefHold, b9, b10];       * disable hold and memory references
  MC[mcr.noRef, b10];               * no storage references
  MC[mcr.disHold, b9];              * disable hold
  MC[mcr.disCF, b8];                * disable base registers   ← swapped comment
  MC[mcr.disBR, b8];                * disable cache flags      ← swapped comment
  MC[mcr.useMCRv, b2];              * use mcr for victim/next-victim
  MC[mcr.fdMiss, b1];               * force dirty victim
  ```
  **Important**: `mcr.disCF` and `mcr.disBR` are **the same bit**
  (b8 = LSB bit 7). The two field names are aliases for one
  hardware bit; setting it disables both BR-relative virtual
  addressing and the cache-flag machinery. The descriptive
  comments in `EMemDefs.mc` are swapped in the source — typo.

- **`chm/dorado/expanded/BootstrapSources.dm/InitialSubrs.mc`**
  lines 51–143 — the `SetMCR` subroutine and its callers.
  Important callers:
  ```
  Set[wantMCR, Or[mcr.useMCRV!, mcr.disCF!, mcr.disHold!,
                  mcr.noWake!, mcr.noRef!, mcr.fdMiss!]];
  T_ DPF[ColX, 2, mcr.MCRVShift];
  T_ T OR (HighByte[wantMCR]);
  T_ T OR (LowByte[wantMCR]), Call[SetMCR];
  ```
  `SetMCR` calls `LoadMCR[T, T]` — both A and B are the same value.

## Spec — bit positions in MCR (LSB-bit numbering)

| Field         | EMemDefs (b#) | LSB bit | Effect                                          |
|---------------|---------------|---------|-------------------------------------------------|
| `noWake`      | b15           | 0       | suppress fault-task wakeups                     |
| `noReportSE`  | b14           | 1       | suppress single-bit ECC error reporting         |
| `noRef`       | b10           | 5       | suppress storage references                    |
| `disHold`     | b9            | 6       | disable Hold (HM §4)                            |
| `disCF/disBR` | b8            | 7       | disable BR-relative addr & cache-flag machinery |
| `useMCRv`     | b2            | 13      | use MCR-supplied victim instead of LRU          |
| `fdMiss`      | b1            | 14      | force dirty miss                                |
| `mcrV` field  | shift=13      | 13–14   | column-victim selector (overlaps useMCRv etc.)  |

## Implementation (this commit)

- `dorado/include/memory.h`: declared `dorado_mcr_dishold`.
- `dorado/src/memory.c`:
  - `dorado_mcr_disbr` corrected to read LSB bit 7 (was bit 8 — wrong
    by one position, treating disBR and disCF as different bits when
    they're aliases for the same hardware bit).
  - new `dorado_mcr_dishold` returns LSB bit 6.
- `dorado/tests/test_memory.c::test_mcr_disbr_blocks_br_writes`:
  test fixture corrected from `0x0100` (LSB bit 8) to `0x0080`
  (LSB bit 7). The test now verifies disBR at the spec-correct bit.

## Still deferred (intentional)

- The `mcr_is_initial_nowake(0xFEE7)` special case is **kept** for now.
  Removing it makes Initial's `SetMCR(noRef|disHold|disCF|disBR|noWake|noReportSE)`
  correctly suppress storage references — but our boot probe relies on
  refs continuing to work because we don't yet model Hold (gap B1) or
  the `noRef → re-enable refs` transition that Initial does later. The
  special case is a probe-time band-aid; remove it after B1 lands.
- **Hold semantics** (`disHold` actually gating Hold) is gap B1. The
  `dorado_mcr_dishold` getter exists so the call site is wired; B1
  just needs to consult it before issuing a Hold.

## Verification

- All 9 test suites green (`make test`).
- `test_mcr_disbr_blocks_br_writes` updated to use the correct bit
  position; pins the new spec.
- `test_mcr_load_and_controls`, `test_mcr_disbr_blocks_br_writes`,
  `test_mcr_noref_suppresses_storage_access`,
  `test_mcr_dvavic_reads_cache_address_without_storage`,
  `test_mcr_noref_store_writes_cache_address` all still pass — none
  exercise the new `disHold` getter, but none use the old wrong
  `disBR` bit either.
