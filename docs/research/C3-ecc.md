---
gap: C3
status: research (deferred — only needed for fault-injection tests)
---

# C3 — ECC

## Source citations

- **HM §5.12** describes the Fire Code polynomial:
  ```
  P(X) = X^32 + X^23 + X^21 + X^11 + X^2 + 1
  ```
  Generates a 32-bit syndrome over each 256-word page, with
  per-quadword (16-word) sub-syndromes recoverable from the full
  syndrome.
- **`EMemDefs.mc`** (already pulled):
  - `pipe4.notMemError = b4` — single-bit error, corrected.
  - `pipe4.notEcFault = b5` — uncorrectable error.
  - `pipe4.quadWordMask = 0o1400` — 2 bits identifying which
    quadword within the munch the error landed on.
  - `pipe4.syndromeMask = 0o377` — 8 bits of the syndrome are
    exposed via Pipe4.
- **C2 wiring** is already in place: `dorado_pipe4_set_error(srn,
  MEM_ERROR | EC_FAULT, syndrome, quadword)` deposits syndrome bits
  into the pipe slot, where microcode reads them via `B<-Pipe4'`.

## Spec

Read flow:
1. Cache miss → fill munch from storage (16 words).
2. ECC unit computes syndrome over the fetched data + stored ECC
   bits.
3. Syndrome = 0 → no error, deliver Md cleanly.
4. Syndrome ≠ 0 and is a single-bit pattern → correct in place,
   set `MemError + syndrome` in Pipe4, deliver corrected data.
5. Multi-bit / out-of-pattern → set `EcFault`, deliver corrupted
   data, microcode handles via fault task.

Write flow:
1. Store new word to cache line.
2. On eventual writeback: regenerate syndrome bits for the
   modified line, write data + new syndrome to storage.

## Implementation plan

### What to model first

Most boot and emulator microcode never **exercises** ECC because the
storage is in-spec — no errors. So a useful first-pass implementation
is:
1. **Storage**: keep `uint16_t storage[]` as today; no per-word ECC
   bits. Implicitly treat all storage as ECC-clean.
2. **Read path**: produce syndrome 0 always. No `MemError` /
   `EcFault` ever fires.
3. **Test-injection API**: `dorado_memory_inject_ecc_error(va, kind,
   syndrome)` flag a specific physical address such that the next
   read of that munch sets the appropriate Pipe4 error bits via
   `dorado_pipe4_set_error`.

This gives us a model that:
- Is invisible to any microcode that doesn't poke the injection API.
- Is exercisable by future fault-microcode tests.
- Defers actual polynomial implementation until something needs it.

### When to land the polynomial

- When microcode that **regenerates** ECC bits on write is exercised
  (Mesa's WriteMap path, certain emulator memory tests).
- When we run a Midas memory test that probes ECC behavior.

Until then the polynomial is a curio — the manual specifies it but
nothing in our test inventory exercises it.

## Verification approach (when implemented)

1. Implement Fire Code generation as a standalone helper:
   `uint32_t dorado_ecc_syndrome(const uint16_t munch[16],
                                 uint32_t stored_ecc);`
2. Test with known-good values from the HM examples (if any) or
   from the schematic-level ECC PROM dump.
3. Inject a single-bit error, verify syndrome identifies the bit
   position, verify correction yields the original data.
4. Inject double-bit, verify `EcFault` raised.

## Pre-conditions

- C2 (Pipe4 errors) ✅ landed — `dorado_pipe4_set_error` exists.
- B1 (Hold) — when ECC errors arrive, the microcode wakes the
  fault task; without Hold modeling, single-task tests can still
  observe the error bits but multi-task ECC handling needs B1.

## Why deferred

Nothing in our current test inventory or boot probe needs ECC. The
polynomial is well-documented; landing it is straightforward when
demand surfaces. Premature implementation would be unverifiable
(no microcode tests it today).
