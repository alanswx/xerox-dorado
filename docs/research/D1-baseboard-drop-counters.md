---
gap: D1
status: landed
---

# D1 — BaseBoard EPROM-write / RIOT-drop counters

## Source citations

- `chm/dorado/expanded/doradobaserom.dm!12_/*.masm` — the BB
  source. The 6502 program never writes its own EPROM region
  (0xC000–0xFFFF); doing so under the live build would be a real
  bug. Counting these makes such a bug visible during probes.
- 6532 RIOT register layout (per `doradoio.mdefs`): only the lower
  range of each chip is hooked to PA/PB/DDR/Timer; writes to other
  offsets are silently dropped by the chip itself. Counting them
  helps identify ROM accesses to addresses we haven't decoded.

## Spec

Two counters added to `dorado_baseboard`:

```
writes_to_eprom        — writes to 0xC000..0xFFFF (a real BB bug if > 0)
riot_writes_dropped    — RIOT writes to unhandled offsets
```

The previous implementation silently dropped both. Behavior is
unchanged; we now make the silence visible.

## Implementation

- `dorado/include/baseboard.h`: 2 new `uint32_t` counters in
  `dorado_baseboard`.
- `dorado/src/baseboard.c`:
  - `riot_register_write`'s default-branch increments
    `riot_writes_dropped` via the active baseboard pointer.
  - `bb_write` collapses the two EPROM-region checks (the duplicate
    0xD000..0xD7FF range was a subset of 0xC000..0xFFFF anyway) and
    increments `writes_to_eprom` before returning.

## Verification

- All existing tests pass; behavior unchanged for valid accesses.
- Future BB-related probes can read these counters before/after a
  run to flag any silent ROM writes or unhandled RIOT register
  pokes — both signals that BB code is poking at registers our
  model doesn't decode (and would be expected to under real
  hardware).
