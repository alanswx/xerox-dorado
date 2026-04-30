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

## The cache (HM §5.11, pages 38–40, 47)

4096-word cache organized as **64 rows × 4 columns (ways) × 16-word
munch**. The cache holds **virtual** addresses, so the Map is
consulted only on miss. A "munch" = one 16-word cache line.

### Address split

For the C emulator's view of the 28-bit VA:

| VA bits (LSB) | Field | Width |
|---|---|---|
| `va & 0xF` | word offset within munch | 4 |
| `(va >> 4) & 0x3F` | row index (0..63) | 6 |
| `va >> 10` | tag (per-way comparator) | 18 |

(HM says "compares VA[4:19]" = 16 bits; in our 1024-page config the
useful tag is 18 bits. The two extra high bits are zero in any 24-bit
VM access and harmless if compared.)

### Replacement: 4-way LRU per row

`dorado_cache_row.lru[]` is a permutation: index 0 = MRU, index 3 =
next victim. On hit, the touched way moves to index 0; on miss, the
LRU way is selected (preferring an invalid way if any).

### Reference behavior — what happens on hit / miss

| Reference | Hit | Miss |
|---|---|---|
| **Fetch / IFetch / LongFetch** | Md ← line[offset]; no Map flag update | Translate via Map (fault check), pick victim (writeback if dirty), fill line, set **Map.Ref** for the new page. |
| **Store** | line[offset] ← B; mark line `dirty`; **Map.Dirty NOT set yet** (HM page 45 — set lazily on evict) | Translate (WP-check), write-allocate fill (sets Map.Ref), then write into the line. |
| **PreFetch** | no-op | Walk Map silently; on success, fill (sets Map.Ref). **Never faults** (HM page 39). |
| **Flush** | clean: invalidate the line. dirty: writeback line to storage, set **Map.Ref AND Map.Dirty**, then invalidate. | no-op (clean miss is silent). |
| **IOFetch** | (real HW: send dirty version from cache) sets Map.Ref | bypasses cache; sets Map.Ref |
| **IOStore** | unconditionally invalidate the cache line *without* writeback; sets **Map.Ref AND Map.Dirty** | bypasses cache; sets Map.Ref AND Map.Dirty |
| **DummyRef / Map** | n/a | n/a (no cache lookup; pipe-only or map-only) |

### Cache ↔ Map invariants (HM page 47)

- **It is illegal to map two distinct VAs to the same RP unless both
  are write-protected.** The cache holds VAs, so otherwise two
  in-flight cache copies of the same RP could become inconsistent.
- The Map is **only** consulted on cache miss (and on the
  dirty-victim writeback). A reference that hits doesn't touch
  Map.Ref or Map.Dirty.
- **Store-then-evict, not Store-then-set-dirty.** A `Store←` that
  hits the cache marks the *line* dirty but leaves Map.Dirty=0 for
  the page. Map.Dirty only becomes 1 when that munch is later
  evicted (dirty-victim writeback) or `Flush←`'d.

### Implementation in our emulator

Implemented in `dorado/src/memory.c` and `dorado/include/memory.h`:

- `dorado_cache_row mem.cache[DM_CACHE_ROWS]` — 64 rows.
- `dorado_cache_lookup(mem, va, *out_way)` — 1 if cached, with way
  index. Used by tests.
- `dorado_storage_at_va(mem, va)` — bypasses cache; translates via
  Map and reads storage directly. For tests probing physical state.
- The cache-address-section flag latch records the row/way selected
  by the most recent memory reference. `CFlags<-A'` updates Dirty,
  Vacant, WP, and BeingLoaded for that selected entry unless `DisCF`
  is set in MCR; `B<-Pipe5` exposes the selected entry's cache flags
  alongside MapBufBusy.
- `cache_pick_victim`, `cache_writeback_line`, `cache_fill`,
  `cache_invalidate_no_writeback` are internal helpers.

Tests (`tests/test_memory.c`):
`test_cache_hit`, `test_cache_miss_fill`,
`test_cache_store_no_map_dirty`, `test_cache_lru_eviction`,
`test_cache_dirty_victim_writeback`, `test_cache_flush_clean`,
`test_iostore_cache_invalidate`, `test_cflags_load_visible_in_pipe5`,
`test_discf_blocks_cflags_and_pipe5_flags`. All passing.

### Not yet modeled (Phase B/C)

- **Hold timing.** Real cache misses Hold the engine for ~28 cycles
  (Table 15: dirty miss). Our model is atomic — every reference
  completes in one virtual cycle.
- **ECC over munches.** HM page 56: munches are divided into 4
  quadwords × (64 data + 8 check bits). We always succeed.
- **Cache parity** (the cache address memory has its own parity).
- **`MapPE` reporting in Pipe4** (we always report no-error).
- `NextVictim` in Pipe5 is not yet modeled.

## The Map (HM §5.5–5.7, pages 44–48)

The Map is **virtual page → real page**. Combined with the page-offset
bits of VA you get the real-word address that indexes storage. The
Map IC is a separate fast SRAM, not a section of main DRAM.

### Configurations (HM Table 16)

The Map IC size, page size, and VA layout are configurable at machine
build time. The seven supported configurations:

| Map IC size | Page size (words) | VA bits used as map index | Page-offset bits | VM size |
|---:|---:|---:|---:|---:|
| 16 K   | 256  | VA[10:23]  | VA[24:31] (low 8)  | 4 M words |
| 16 K   | 1024 | VA[8:21]   | VA[22:31] (low 10) | 16 M words |
| 16 K   | 4096 | VA[6:19]   | VA[20:31] (low 12) | 64 M words |
| 64 K   | 256  | VA[6:21]   | VA[24:31] (low 8)  | 16 M words |
| 64 K   | 1024 | VA[6:21]   | VA[22:31] (low 10) | 64 M words |
| 64 K   | 4096 | VA[6:21]   | VA[20:31] (low 12) | 256 M words |
| 256 K  | 256  | VA[4:21]   | VA[24:31] (low 8)  | 64 M words |

(The C emulator currently picks **16 K-entry × 256-word page**, giving
VM = 2^22 words = 4 MW. This matches the smallest/default cache-map
configuration and Initial's observed map setup loop.)

The total addressable VM is `IC_size × page_size`. For our config:
16384 × 256 = 4 MW = 8 MB.

### Map entry format (HM page 45)

Each map entry is one Map IC word:

```
  ┌─────────────────────────────┬────┬─────┬────┐
  │   RP  (16 bits real page)    │ WP │  D  │ R  │
  └─────────────────────────────┴────┴─────┴────┘
```

- **RP** — 16-bit real page number. Concatenated with the page-offset
  bits of VA to form the real-word address `(RP << page_offset_bits) |
  page_offset(VA)`.
- **WP** — write-protected. `Store←` and `IOStore←` to a WP entry
  fault.
- **Dirty** — modified since last clean. Set unconditionally by
  `IOStore←` and by the dirty-victim writeback in cache replacement.
  Notably **`Store←` does not set Dirty until that munch is chosen as
  the victim** — until then the cache holds the dirty data, the map
  doesn't know yet.
- **Ref** — referenced. Set on every storage reference except `Map←`.
  Cleared by `Map←`. Page-replacement microcode reads + clears Ref
  periodically.
- **Vacant** — encoded as `WP=1` AND `Dirty=1`. Any reference to a
  Vacant entry causes a page fault. Map memory is initialized to all-
  Vacant at power-up; microcode populates entries via `Map←` during
  startup.

### Faults (HM page 46)

| Fault | Trigger |
|---|---|
| **Page fault** | Reference to a Vacant entry (WP=1 AND Dirty=1) |
| **Write-protect fault** | `Store←` / `IOStore←` to entry with WP=1 (and not Dirty=1, else it'd be Page fault) |
| **Map trouble** | Parity error reading the Map IC (we don't model this) |

A fault wakes the fault task (task 15) on real hardware; in our
single-task model we record `last_fault` and `last_fault_va` for
inspection, and the caller (cpu.c) decides whether to halt.

The Pipe **always** receives the offending reference's entry — so
fault microcode can read Pipe0/Pipe1 to recover the VA that faulted.
This is why `pipe_push()` runs before `va_translate()` in the
emulator.

### `Map←` write semantics (HM page 46)

A `Map←B` reference loads a map entry. The entry written is the one
indexed by VA (same map-index extraction as a normal reference). The
data comes from B and TIOA:

- **RP ← B[0:15]** — full 16-bit real page number.
- **WP ← TIOA[0]** — top bit of TIOA in manual MSB-first numbering
  (i.e. C-LSB bit 7 of the 8-bit TIOA register).
- **Dirty ← TIOA[1]** — second bit of TIOA (C-LSB bit 6).
- **Ref ← 0** — Map← unconditionally zeroes the Ref bit.

`Map←` never faults. Used by emulator microcode and the fault task
during page-fault handling and at startup.

### Reading the Map: `RMap` (HM page 47)

There is no direct way to read a map entry into B. Instead, the Pipe
records the **old** Map flags whenever a reference touches that entry.
Microcode reads the Pipe via `Pipe3'` to recover Ref/Dirty for an
arbitrary VA: issue a `DummyRef←` (which doesn't change the entry,
since DummyRef is pipe-only), then read `Pipe3'`.

Important consequence: **Ref and Dirty changes appear in the Pipe with
their pre-reference values** — i.e. the Pipe captures the entry as it
was *before* this reference updated it. So a sequence
`Store←R; ...read Pipe3' for R...` returns Dirty=0 if R's entry
wasn't dirty before.

### Cache/Map interaction (HM page 47)

- The cache holds **VAs**, not real addresses, so two different VAs
  that map to the same RP would alias. **It is illegal for two map
  entries to point at the same RP unless both are write-protected.**
  Microcode that wants to share a page mounts both entries with WP=1.
- The Map is consulted **only on cache miss**. A reference that hits
  the cache does not update Ref/Dirty in the Map — the cache holds
  its own dirty bit per munch, and the Map's Dirty is updated lazily
  when the dirty munch is finally evicted.
- A `Flush←` invalidates a cache munch and writes it back if dirty;
  this is when Map.Dirty for that VA finally goes high.

### Hold conditions (HM pages 41–42)

The Memory Section can stall the Processor by asserting **Hold**.
The processor freezes the entire pipeline (microengine, IFU) for the
holding cycles. Causes:

1. **Md not ready** — microcode reads `Md` before the fetch has
   landed. Hold until cache delivers the word.
2. **Address Section busy** — see below.
3. **DBuf write conflict** — back-to-back `Store←` references where
   DBuf hasn't been emptied to the cache.
4. **Reference issued during write-back** — cache is using its
   storage port for a dirty victim write.

The C emulator currently treats every reference as atomic (no Hold);
adding Hold modeling is a Phase B/C task once IFU + tasking are in.

### Address Section busy (HM pages 42–43)

The Address Section ("AS") arbitrates VA generation and Map lookup.
It is **busy** in any of six situations, in which case a new
reference is held off:

1. AS is currently translating a previous reference's VA through the
   Map (one cycle per ref).
2. A storage transfer (cache munch fill or write-back) is in flight
   and the AS is locked to that transfer's bookkeeping.
3. The Pipe slot for the next reference is still occupied by an in-
   flight reference (the Pipe head can't advance).
4. A `Map←` write is in progress (the Map IC is single-ported).
5. A `Flush←` is sequencing a dirty-victim write.
6. The IFU is using the AS for an opcode-fetch reference (BR[31]+IP).

Item 6 is why we expose `Pipe5'` for IFU references too — the IFU's
fetches share the Pipe.

### Timing of a dirty miss (HM Table 15, pages 43–44)

When a Fetch misses the cache and the row's victim is dirty, the
sequence is:

```
t0   Microcode issues Fetch←
t1   VA = BR[MemBase] + Mar; cache CAM lookup begins
t3   Cache reports MISS; victim selected (LRU); Map lookup begins
t6   Map result back; check WP/Vacant
t8   Storage row select: write-back (dirty victim) starts
t12  Write-back occupies Sout for 4 cycles (16 words × DRAM bandwidth)
t16  Read of new munch starts on Sin
t20  First word of new munch arrives in cache
t24  Munch fully filled; Md valid for the requested word
```

The clean-miss path skips t8..t15 (no write-back), so it's t16-ish
to first word. Cache hit is t3.

These cycle counts are approximate (Table 15 has the exact values);
the C emulator's "observable cycle accuracy" target means microcode
that times itself by waiting cycles must see Md ready at the right
cycle, but the internals (which row, when Sout vs. Sin is busy) can
be opaque.

## The Pipe (HM §5.8, pages 51–53)

A 16-entry ring of recent storage references. Each entry holds 6
"words" worth of state, addressed by 4-bit SRN (Storage Reference
Number). Microcode reads entries via `B←Pipe0..Pipe5` FF decodes
**after** loading `ProcSRN←B` to select which slot to read.

### SRN allocation  ✓ MODELED

- **ProcSRN** (default 0) — used by task 0 (emulator) and task 15
  (fault task) for non-prefetch references. Conventionally
  microcode keeps ProcSRN=0 (emulator) or 1 (fault task) to avoid
  clobbering each other.
- **ASRN** (2..15) — automatically-allocated 14-slot ring used by
  I/O tasks and emulator PreFetch-with-miss. Advances after any
  reference that "starts the map" (i.e., goes to storage); held
  otherwise. Wraps 15 → 2.
- **PreFetch hit** uses ProcSRN; **PreFetch miss** uses ASRN.

In the C emulator: `dorado_memory.proc_srn` and `dorado_memory.asrn`
are 4-bit fields. `dorado_memory_ref` selects ASRN for IOFetch /
IOStore / PreFetch-with-miss; everything else uses ProcSRN. ASRN
advances after any ASRN-using reference (all our ASRN cases start
the map). Tests:

- `test_pipe_records` — 3 IOFetches land in ASRN slots 2, 3, 4
  with asrn advancing 2→3→4→5
- `test_pipe_wraps` — 14 IOFetches fill slots 2..15; the 15th
  wraps and overwrites slot 2; ProcSRN slots 0/1 stay untouched
- `test_proc_srn_overwrite` — successive Fetches all overwrite
  the same ProcSRN slot; `ProcSRN←B` retargets it
- `test_prefetch_srn_split` — PreFetch miss → ASRN, PreFetch hit
  → ProcSRN

Microcode loads the slot to read with `ProcSRN←B` (FA=1 FB=2 FC=7,
B[12:15] → ProcSRN), then reads `B←Pipei` to fetch fields of that
slot. We honor that in cpu.c — `B←Pipe0/Pipe1/Pipe3'/Pipe4'/Pipe5`
all read from `pipe[mem.proc_srn]`.

`Map<-` models the hardware MapBufBusy delay that Initial polls in
`WAITFORMAPBUF`: the map write marks the addressed pipe slot busy for
9 CPU ticks, and `B←Pipe5` exposes that busy state as the sign bit.
This matches HM §5's MapBuf timing and the MEMX/MEMC schematic names
around Mapbuf/HoldMapbuf/Pipe5. The actual map RAM update is still
performed immediately by the emulator; only the observable busy timing
is delayed.

`CFlags<-A'` and the cache-flag portion of `B<-Pipe5` are now modeled
well enough for initialization/diagnostic code: they operate on the
cache row of the last memory reference and the selected hit/victim
column, and `DisCF` forces reads to zero and suppresses writes. The
exact Figure 10 bit positions should still be checked against MEMC/MEMX
before relying on obscure diagnostics.

### Pipe entry contents

Approximate field layout (HM Figure 10 has the exact bit positions;
this table summarizes the user-visible reads):

| Read | Sense | Contents |
|---|---|---|
| `B←Pipe0` | high-true | VaHi = VA[0:15] (bits 0:15 of the 28-bit VA, MSB-first) |
| `B←Pipe1` | high-true | VaLo = VA[16:31] |
| `B←Pipe2'` | low-true | EmulatorFault, NFaults, SRNFirstFault. **Same data as `B←FaultInfo'`** — Pipe2' is a "convenient decode" for the same FaultInfo register. |
| `B←Pipe3'` | low-true | Map flags **as they were before this reference**: WP, Dirty, Ref, BeingLoaded, NextVictim (+ RP) |
| `B←Pipe4` | mixed | Errors: syndrome bits, correctable bit, etc. XOR with `0150361₈` to get high-true. |
| `B←Pipe5` | high-true | MapBufBusy in manual bit 0 plus selected cache-address-section flags in manual bits 8..11 (Dirty, Vacant, WP, BeingLoaded). |

`B←FaultInfo'` (`FA=1 FB=6 FC=0`) returns:
- B[8:11] = SRN of 1st fault (4 bits, inverted)
- B[12:15] = NFaults count (4 bits, inverted)
- B[7] (or similar) = EmulatorFault flag (was the first fault from
  task 0?)
- All bits inverted on the bus → "no faults" reads as `0xFFFF`.

### Pipe3' bit layout (as we model it)

We record three flags in each pipe slot, snapshotted **before** the
reference modified them:

```
  Pipe3 (high-true, internal):
    bit 0 (LSB)  WP
    bit 1        Dirty
    bit 2        Ref
    bits 3..15   reserved (zero)

  B←Pipe3' (on the bus): ~Pipe3
```

Real hardware also carries `BeingLoaded`, `NextVictim`, and the RP
in this word; we leave those zero until the cache is modeled.

### Reference→Pipe handshake

- Every reference (Fetch, Store, Map, IFetch, Prefetch, IOFetch,
  IOStore, Flush, DummyRef) pushes one Pipe entry. **Faulting refs
  push too** — fault microcode reads `Pipe0/Pipe1` to recover the VA.
- The snapshot of map flags captured into Pipe3' is the value
  *before* this reference (so a `Store←` that sets Ref appears in
  the next-cycle pipe entry with Ref=0 — the *new* Ref will only
  show up on the *next* reference's Pipe3' snapshot).
- `Map←` itself returns the previous map contents in the pipe,
  same way (HM page 47: "Reading the Map: Every storage reference
  causes mapping and returns old contents of the relevant map
  entry in the pipe").
- It is illegal to issue `B←Pipei` in the same instruction as a
  reference — Hold isn't computed properly. Microcode must `Md←` or
  similar to drain in-flight refs before reading the pipe.

### `B←FaultInfo'` semantics in our emulator

We track:
- `fault_count` — number of unacknowledged faults (NFaults)
- `fault_first_srn` — SRN of the first uncleared fault
- `fault_emulator` — always 1 in single-task mode
- `last_fault_va`, `last_fault` (kind) — for diagnostic halts

`dorado_fault_clear(mem)` resets these. Real microcode acknowledges
faults by some means (TBD — the manual implies the fault task
reads-and-clears).

### Figure 10 (the canonical Pipe layout)

We've not yet transcribed Figure 10 in full. When we do, this
section gets the exact bit-field assignments for each Pipe word.

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

1. ~~**Mar + base-register addition**~~ — done. `BR[MemBase] + Mar`
   computed in `cpu.c`'s memory-ref dispatch.
2. ~~**Flat 16-bit "fake" memory**~~ — done. `dorado_memory.storage`
   is a flat `uint16_t[DM_STORAGE_WORDS]` (4 MW). Refs translate VA
   through the Map and index it directly.
3. **Md timing model** — TBD. Currently atomic: Md is valid in the
   same cycle the fetch issues. Need to add Hold and per-cycle Md-
   ready tracking before tasking-driven microcode is correct.
4. ~~**Pipe**~~ — done. 16-entry ring, every ref pushes regardless of
   fault. `dorado_pipe_va(n)` returns slot relative to head.
5. **Cache** — TBD. Once Mesa's microcode starts Long-Fetching across
   page boundaries we'll want 4-way LRU semantics, mostly because
   eviction affects when Pipe entries (the Pipe is ref-driven, not
   cache-driven, so it's actually fine?) and dirty-victim timing
   matters.
6. ~~**Map**~~ — done. 16K entries × {RP, WP, Dirty, Ref}. Vacant at
   init. `Map←` writes RP from B and WP/Dirty from TIOA[0:1]. Faults
   surface as `dorado_fault_kind` return values plus `last_fault*`
   state on the memory struct.
7. **ECC** — TBD. Only needed if microcode reads `Pipe3'` error
   fields; stubbed (always "no error") for the C emulator. Verilog
   target will need it.
8. **Hold + Address-Section busy** — TBD. Phase C work.

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

- The cache LRU mechanism's exact bit layout (Phase B.3).
- How `LongFetch←` actually combines T[4:15] with Rtemp — looks like
  a concatenation but the numbering is non-obvious.
- The interplay between Map fault, Pipe recycling, and the fault
  task's wakeup latency. We push to Pipe before translating, so the
  faulting VA lands in Pipe0; but the Pipe **also** captures the OLD
  Map flags into `Pipe3'`. Real hardware does this in one cycle; we
  don't currently snapshot the pre-ref flags.
- The IFU's interaction with the cache (uses BR[31], runs in parallel
  with the processor — Figure 8 implies a separate cache port).
- ECC syndrome layout (`Pipe3'` low bits) — which Hamming code Xerox
  used. HM §5.7 says SEC + DED extension over 16 data bits = 8 EC
  bits per word; the precise generator-matrix is TBD.
- What "AS busy" item 3 (Pipe slot occupied) means in cycles — the
  Pipe is conceptually a 16-deep ring so it should always have room,
  but the manual's wording suggests an in-flight reference can hold
  the head briefly.

These are TBD-after-microcode-driven-need.
