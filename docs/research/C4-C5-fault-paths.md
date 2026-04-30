---
gap: C4, C5
status: landed (C4); diagnosed (C5 — non-issue)
---

# C4 — Dirty-victim WP fault not recorded

## Source citations

- **HM §5** memory section: when a dirty cache line is the eviction
  victim and its map entry has been written-protected since the
  fill, the writeback fails. Real hardware drops the data, sets the
  Pipe4 syndrome bits, and updates FaultInfo for the *triggering*
  reference's SRN. (HM Table 16 / §5.11.)
- `memory.c:475` (pre-fix): silently dropped the data with no
  FaultInfo update.

## Spec

`cache_writeback_line` translates the line's VA with `is_write=1`. On
WP / Vacant / out-of-range failure, no storage write occurs. The
triggering reference still completes, but FaultInfo's
`fault_count` is incremented and `fault_first_srn` captures the
triggering SRN if it's the first uncleared fault.

Pipe4 syndrome bits are *not* set by this fix — that's a separate
gap (C2). Microcode reading `B←Pipe2'` / `B←FaultInfo'` will see
the count, which is the minimum visible behavior.

## Implementation

- `dorado/src/memory.c`:
  - `cache_writeback_line` returns `dorado_fault_kind`.
  - new helper `record_writeback_fault(mem, kind, srn)` updates
    `fault_count` / `fault_first_srn` per the existing main-fault
    block.
  - all four call sites (Fetch-class miss, Store-class miss, Flush
    dirty-hit) wrap the call to credit faults to the triggering
    reference's SRN.

## Verification

- `tests/test_memory.c::test_dirty_victim_wp_fault` (gap C4):
  builds a 4-way row, dirties one VA, marks its map entry WP=1,
  then evicts it via 4 fetches. Verifies the evicting reference
  succeeds, `fault_count` advances, and storage was not written.

# C5 — `ReadMap` / PreFetch on Vacant: not a gap

The handoff doc cited `memory.c:614` as "ReadMap walks the Map
silently on Vacant." Investigation shows:

- Line 614 is **PreFetch**, not ReadMap. Per HM page 39 PreFetch
  "does not clobber Md and never causes a map fault" — the silent
  walk is correct behavior.
- ReadMap (HM page 41 / Table 11a FA=0 FB=3 FC=1) is implemented in
  `cpu.c:658`. It packs the map entry's `WP/Dirty/Ref/RP` into a
  16-bit word for Pd. It is not supposed to fault — it's a
  diagnostic read that lets microcode inspect the map (including
  Vacant entries, which appear as WP=1 Dirty=1).

C5 is closed as a misdiagnosis. No code change required.
