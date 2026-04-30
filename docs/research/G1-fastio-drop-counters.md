---
gap: G1
status: landed
---

# G1 — Fast-IO unrouted-discard logging

## Source citations

- **HM §8** Fast I/O: a Fin/Fout transfer Holds the processor when
  the destination FIFO is full or the source FIFO is empty.
- `fastio.c` (pre-fix): silently dropped the remaining munch words
  in three places (display FIFO full, disk FIFO full, disk FIFO
  empty), and silently ignored every IOFetch/IOStore from a
  non-DWT/DSK task.

## Spec

We do not yet model Hold (gap B1), so silent drops continue. To make
those drops *detectable* — so probes and tests can flag missing
routes — we add five counters to `dorado_fastio_router`:

```
drops_display_fifo_full
drops_disk_fifo_full
drops_disk_fifo_empty
drops_unrouted_iofetch
drops_unrouted_iostore
```

The counters are pure diagnostics: no behavior change.

## Implementation

- `dorado/include/fastio.h`: 5 new `uint32_t` counters in
  `dorado_fastio_router`.
- `dorado/src/fastio.c::dorado_fastio_dispatch`: bump the relevant
  counter whenever a transfer can't complete.

## Verification

- All existing fast-IO tests still pass.
- Future probe sessions can read these counters to confirm whether
  unmodelled tasks (Ether, terminal) are silently issuing IOFetch /
  IOStore. This will be useful for H1 once we start wiring an
  Ethernet controller.
