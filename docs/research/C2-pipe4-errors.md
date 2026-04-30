---
gap: C2
status: landed (ECC-event injection deferred until C3 lands)
---

# C2 — Per-slot Pipe4 errors

## Source citations

- **`chm/doradomicrocode/doradomicrocodesources/EMemDefs.mc`** lines
  22–35 — canonical Mesa-side bit definitions (Title: *September 23,
  1981 4:15 PM, Taft*):
  ```
  * B_ NOT (FaultInfo')
  MC[fi.emuFault, b8];                * emulator made a fault
  MC[fi.numfaults, b9, b10, b11];     * nFaults-1 at time pipe read

  * B_ Pipe4 (_Errors) constants and Values
  MC[pipe4.ref, b0];
  MC[pipe4.notMapTrouble, b1];
  MC[pipe4.wProtect, b2];
  MC[pipe4.dirty, b3];
  MC[pipe4.vacant, b2, b3];           * vacant is dirty & wProtect
  MC[m1pipe4.wpdref, b0, b2, b3];     * all the ref bits

  MC[pipe4.notMemError, b4];
  MC[pipe4.notEcFault, b5];
  MC[pipe4.quadWordMask, 1400];
  MC[pipe4.syndromeMask, 377];
  ```
- **HM page 51** (cited in `cpu.c`'s pre-fix comment): `0o150361 XOR
  Pipe4'` yields the high-true semantic value. The no-error baseline
  is `0o150361 = 0xD0F1`.
- **Active-low `not*` field convention**: Mesa source tests for the
  *negated* condition by ANDing the high-true word against the bit
  mask. In the high-true representation, `MapTrouble = NOT
  notMapTrouble` reads as bit 14 set when there's a fault.

## Spec — high-true Pipe4 layout (LSB bit numbering)

| Bit (LSB) | Manual `bN` | High-true name | Meaning                               |
|-----------|-------------|----------------|----------------------------------------|
| 15        | b0          | `ref`          | slot has a real reference              |
| 14        | b1          | `MapTrouble`   | dirty-victim writeback failed          |
| 13        | b2          | `wProtect`     | snapshot of map WP at fill time        |
| 12        | b3          | `dirty`        | snapshot of cache dirty at fill time   |
| 11        | b4          | `MemError`     | single-bit ECC, corrected              |
| 10        | b5          | `EcFault`      | uncorrectable ECC                      |
| 9..8      | b6:7        | `quadWord`     | which 16-word pair of the 4×16 munch   |
| 7..0      | b8:15       | `syndrome`     | 8-bit ECC syndrome                     |

`vacant = wProtect & dirty` per `EMemDefs.mc`.

## Implementation

- `dorado/include/memory.h`:
  - 3 new fields on each pipe slot: `pipe4_errors`, `pipe4_syndrome`,
    `pipe4_quadword`.
  - 3 public `PIPE4_ERR_*` flags: `MAP_TROUBLE`, `MEM_ERROR`,
    `EC_FAULT`.
  - 2 new accessors: `dorado_pipe4_at(mem, srn)` returns the
    encoded `B<-Pipe4'` word; `dorado_pipe4_set_error(...)` sets
    error bits + syndrome on a slot.
- `dorado/src/memory.c`:
  - `dorado_pipe4_at` composes the high-true value from per-slot
    state, then XORs with `0o150361` to produce the raw `Pipe4'`
    the processor sees.
  - `pipe_push` clears `pipe4_errors / syndrome / quadword` on each
    new ref so old errors don't leak.
  - `record_writeback_fault` (added in gap C4) now also calls
    `dorado_pipe4_set_error(MAP_TROUBLE)` for the triggering SRN —
    so a dirty-victim WP fault now surfaces both as a `fault_count`
    bump (visible via `B<-FaultInfo'`) AND as a `MapTrouble` bit
    in `B<-Pipe4'` for the relevant slot.
- `dorado/src/cpu.c`:
  - `B<-Pipe4'` (FA=1 FB=6 FC=5) now calls `dorado_pipe4_at(...)`
    instead of returning the constant baseline.

## Verification

- `tests/test_memory.c::test_pipe4_error_encoding` — verifies the
  no-error baseline (only `ref` set), then injects MapTrouble + a
  syndromed MemError and checks the high-true XOR is bit-exact.
- `tests/test_memory.c::test_dirty_victim_pipe4_map_trouble` — the
  C4-style dirty-victim WP fault scenario, but checks that
  `B<-Pipe4'` now surfaces MapTrouble in addition to the FaultInfo
  count.
- All 9 test suites still pass.

## Deferred

- **wProtect / dirty fill-time snapshots** — Pipe4 also reports the
  cache-line WP/Dirty values *as observed during the fill*. We don't
  yet capture those at fill time — they stay 0 in our encoding. Add
  when microcode that reads them is exercised.
- **MemError / EcFault injection** — the bit positions and accessor
  exist, but no path in our model currently calls
  `dorado_pipe4_set_error(MEM_ERROR | EC_FAULT, ...)`. Wire when
  C3 (ECC) lands.
