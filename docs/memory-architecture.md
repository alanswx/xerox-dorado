# Dorado memory architecture — reference

What we'll need to build before Mesa or Cedar can boot. **Not yet
implemented in the C emulator** — captured here so the design isn't
forgotten between sessions.

Sources: HM §1, §5, §6 (IFU), Figure 8 (Overall Structure of the
Memory System), Figure 9 (Cache, Map, Storage Addressing).

## At a glance

```
   Microcode (in IM)
        │  Mar (16 bits)        MemBase (5 bits, per task)
        │            ↘           ↙
        ▼             ┌────────────────┐
                      │ BR  32 × 28-bit │  base register memory
                      └────────┬───────┘
                               │ BR[MemBase] + Mar = 28-bit Virtual Address
                               ▼
                ┌─────────────────────────────────────┐
                │  Cache  4 KW                         │   Pipe (16 × 6 words)
                │  64 rows × 4 cols × 16 words/line    │   trace ring
                │  hit on VA[4:19]                     │
                └─────────┬───────────────┬───────────┘
                          │ hit           │ miss / Map ref
                          │               ▼
                          │     ┌──────────────────────┐
                          │     │  Map  16 K (or 64 K) │  virtual→physical
                          │     │   × 19-bit entries    │  + flags (W, D, R, ECC)
                          │     │   addr = VA[10:23]    │
                          │     └─────────┬────────────┘
                          │               │ 12-bit real page + flags
                          │               ▼
                          │     ┌──────────────────────┐
                          │     │  Main storage        │  256 K × 16 words
                          │     │  + EC bits per word  │  (1–4 modules)
                          │     │  Hamming SEC + DED   │
                          │     └──────────┬───────────┘
                          │                │
                          ▼                ▼
                ┌─────────────────────────────────────┐
                │  Md (memory data) → A / B / T / RM  │
                │  IOA / IOB → slow I/O               │
                │  Fin / Fout → fast I/O munches       │
                └─────────────────────────────────────┘
```

## Address spaces

| Space             | Width   | Notes                                                 |
|-------------------|---------|-------------------------------------------------------|
| Virtual address (VA) | 22 to 28 bits | configurable; 22 bits = 4 M words = 8 MB     |
| Real page address  | 12 bits | 256 K real-page table for the 16 K Map; bigger Maps possible |
| Real word address  | 22 bits (typ.) | real page (12) + page-relative word (10)        |
| Physical word storage | 16+8 bits | 16 data + 8 EC bits per word in DRAM              |
| Cache line         | 16 words | one cache "munch" = 16×16 = 256 bits              |

VA bits 0:3 are reserved for future expansion; bits 4:31 form the
28-bit "real" virtual address used today. Of those, bits 4:9 are the
6-bit page offset within a 64-word storage row, bits 10:23 form the
14-bit Map index (or 16-bit for the 64 K Map), bits 24:31 are the
high-order page bits.

## Addressing pipeline

A processor memory reference unfolds over many cycles:

| Phase           | Source / Action                                            |
|-----------------|------------------------------------------------------------|
| t₀              | Microcode delivers `Mar` (from A bus) and selects `MemBase` |
| t₁              | `BR[MemBase] + Mar` → 28-bit VA; cache CAM lookup begins    |
| t₃ (hit)        | Cache delivers Md to processor (T, RM, A, B path)          |
| t₃ (miss)       | Map lookup uses VA[10:23]; check write-protect / dirty     |
| t₁₄             | Map result; storage row select on miss                     |
| t₂₈ (miss)      | Storage data ready, cache line filled, Md eventually valid |

Latency: cache hit ≈ 3 cycles, cache miss ≈ 28+ cycles (which the
memory system can pipeline at 8-cycle issue cadence). The processor
**Holds** when it tries to use Md before it's ready.

## Reference types (HM §5.2 — drives ASEL + FF[0:1])

| Microcode form     | Meaning                                                       |
|--------------------|---------------------------------------------------------------|
| `Fetch←T;`         | start a fetch (16-bit word) at VA = BR[MemBase] + T          |
| `T←Md;`            | read the result of a previous fetch into T                    |
| `Store←Rtemp, DBuf←T;` | write T to VA = BR[MemBase] + Rtemp                       |
| `IFetch←Stack;`    | fetch with VA = BR[MemBase] + Stack, IFU operand byte replacing low 8 of address |
| `LongFetch←Rtemp, B←T;` | fetch with VA[4:31] = BR[4:15] + (T[4:15] ‖ Rtemp[0:15]) — full 28-bit addressing |
| `PreFetch←R;`      | warm the cache for VA, no Md change, no map fault             |
| `DummyRef←R;`      | log VA into the Pipe without doing memory                     |
| `Flush←R;`         | invalidate cache munch containing VA, flush dirty back        |
| `Map←R, MapBuf←T;` | write a Map entry — emulator/fault task only                  |
| `IOFetch←R;` / `IOStore←R;` | initiate fast I/O munch transfer (I/O tasks only)     |

The reference type determines whether T, RM/STK, Md, Id, or Q is
the source for VA on this cycle (HM Table 8a).

## The cache (HM §5.11)

- 4096 words organized as 64 rows × 4 columns × 16 words/line.
- Tag CAM: 4 entries per row, compared in parallel with VA[4:19].
- Replacement: LRU per row (the manual hints; tagging in MEMC).
- Writes update both the cache (if hit) and the storage (if dirty bit
  set on prior write — but the actual write-back happens later).
- "Munch" = one 16-word line. Cache miss fills a whole munch.
- Bit 0 of each entry is "dirty"; bit 1 is "valid"; tag is VA[4:19].

For the C emulator we will eventually want a faithful 4-way LRU model.
For the first pass a fully-associative or even direct-mapped cache
is acceptable — we just have to guarantee that *behavior* (Md timing,
Hold) matches what microcode expects.

## The Map (HM §5.5)

- 16 K (default) or 64 K (extended) entries × 19 bits + parity.
- Indexed by VA[10:23] (14 or 16 bits).
- Each entry holds: 12 bits real page, write-protect (W), dirty (D),
  referenced (R), ECC single-error, double-error.
- Loaded by `Map←B (ASEL)` with data from MapBuf register.
- Read implicitly on cache miss; explicitly via `Pipe3'` (the
  Map field of the Pipe).

The Map is **not virtual to physical address**, it's
**virtual page → real page**. Combined with the page offset bits
(VA[24:31]) you get the 22-bit real-word address.

A Map fault sets a flag in the Pipe entry for the offending VA and
wakes the fault task (task 15).

## The Pipe (HM §5.8)

A 16-entry × 6-word ring buffer. Every memory reference automatically
allocates a Pipe entry and writes:

- Word 0 (VaHi): bits 0:15 of VA
- Word 1 (VaLo): bits 16:31 of VA
- Word 2 (Map'): map flags (W, D, R, single/double error)
- Word 3 (Errors'): single-bit-corrected, syndrome
- Word 4 (Config' / task / subtask info)
- Word 5 (Pipe5' — additional state)

Microcode reads Pipe entries via `Pipe0..Pipe5'` FF decodes.
Used for fault recovery (figuring out what address faulted) and for
maintenance (cache line ECC scrubbing).

## Base Registers (HM §5)

- 32 registers × 28 bits (BR memory).
- Indexed by MemBase (5 bits, per task).
- Loaded with `BrLo←A` (writes BR[MemBase][16:31] = A) and
  `BrHi←A` (writes BR[MemBase][4:15] = A[4:15]).
- Read indirectly via Pipe0 (which reflects the VA after BR addition).
- BR[31] is conventionally the **code base** for the IFU.

The IFU implicitly uses BR[31] to fetch opcodes; emulator microcode
explicitly uses other BRs for local frames, global frames, code-segment
secondary base, etc.

`MemBase` is **per task** but can be redirected with `MemBase←B`
(takes value from FF[3:7] for emulator) or `Pointers←B`
(loads MemBase + RBase together). The IFU also reloads MemBase at
opcode dispatch time from IFUM.MemB.

The 2-bit `MemBX` register acts like a stack pointer relative to
MemBase — see HM §6.4.

## ECC and storage layout

- Storage: 256 K × 16 words per module, 1–4 modules.
- 8 EC bits per 16-bit word (Hamming SEC + DED extension).
- Single bits corrected automatically; doubles fault.
- `Sin` / `Sout` are the storage buses, separate from Md.
- Refresh is task-driven (the Memory Refresh Task — task 8 by default).

## Mesa map primitives (HM §5.7)

The emulator microprogram implements MesaPrim opcodes that map/unmap
pages. Implementation is via `Map←B / RMap←R / MapBuf←T`. Specific to
the Mesa virtual-machine interface, not a hardware feature per se.

## Implementation strategy for the C emulator

In rough dependency order (matching what microcode actually exercises):

1. **Mar + base-register addition** — needed by every memory reference.
   `BR[MemBase] + Mar`, store in a "current VA" scratch.
2. **Flat 16-bit "fake" memory** — first pass: ignore the cache /
   Map / Pipe entirely. Treat memory as a flat `uint16_t mem[1<<22]`
   indexed by the low 22 bits of VA. This is enough for Bootstrap
   (which doesn't touch main storage) and Initial (which only zeros
   memory and reads keys).
3. **Md timing model** — minimum: Md becomes valid the cycle after
   `Fetch←` issues; Hold if read sooner. Cycle counts can come later.
4. **Cache** — once Mesa's microcode starts Long-Fetching across
   page boundaries, we'll need cache-line semantics for correctness
   (mostly: the LRU replacement affects when Pipe entries get
   recycled, which microcode peeks at).
5. **Map + Pipe** — needed by Mesa for page-fault handling and by the
   fault task. Implement as two arrays + a 16-entry ring.
6. **ECC** — only needed if microcode reads Pipe error fields; can be
   stubbed (always "no error") for the C emulator. Verilog will need it.

## Cycle-accuracy

The HM is explicit about cycle-by-cycle timing for memory operations
(Figure 9 has a t₀…t₅₁ ladder). For the **C emulator** we want
*observable-cycle-accuracy*: microcode that times itself by counting
cycles must see the same Md-ready cycle the hardware would deliver.
The internal cache-miss-storage-pipeline can be opaque so long as the
external timing matches.

For the **Verilog** target, we want full structural fidelity (separate
modules for cache, Map, Pipe, storage, with the actual 4-input mux
logic). The C emulator's cycle-accurate timing is the spec.

## What we don't yet fully understand

- The cache LRU mechanism's exact bit layout.
- How `LongFetch←` actually combines T[4:15] with Rtemp — looks like
  a concatenation but the numbering is non-obvious.
- The interplay between Map fault, Pipe recycling, and the fault
  task's wakeup latency.
- The IFU's interaction with the cache (uses BR[31], runs in parallel
  with the processor — Figure 8 implies a separate cache port).

These are TBD-after-microcode-driven-need.
