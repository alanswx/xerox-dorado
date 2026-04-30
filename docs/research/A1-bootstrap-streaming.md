---
gap: A1
status: research (corrected after reading BootstrapMain.mc)
---

# A1 — Bootstrap streaming corruption

`probe_full_boot_with_bootstrap` produces an Initial image with ~768
of 896 IM half-writes wrong. Currently bypassed by substituting
canonical `Initial.MB` at BOOTSTAGE2.

> **Important update (post-D2):** an earlier draft of this doc
> conflated two layers. The 17-byte hunks at `$C016–$D7FF` are the
> BB's *internal ROM storage* of Boot1Data, NOT the byte stream
> that flows over CPReg. The over-CPReg stream is documented in
> `BootstrapMain.mc` lines 56–83 — **2 CPReg reads per
> half-microinstruction, 4 reads per full microinstruction**.
> `SendAHunk` unpacks the hunk bytes on the fly into the
> 2-byte-per-half CPReg stream. The 17-byte hunk format remains a
> useful reference for what the BB has in ROM but is not what
> Bootstrap.mc decodes. Use `BootstrapMain.mc` as the authoritative
> oracle.

## Authoritative spec — `BootstrapMain.mc` lines 56–83

> Bootstrap reads data from the Baseboard by reading the CPReg.
> First it reads a starting address value (two bytes) and a count
> (two bytes). Then it reads instructions (four bytes each) and
> writes them into IM. The last two bytes the Baseboard transmits
> to Bootstrap contain a 16 bit checksum that the "Initial"
> microcode checks. Whenever the Baseboard transmits bytes to the
> Dorado, Bootstrap assembles the bytes left to right. The
> Baseboard provides IM bytes in the following format:
>
>     0 <7 bits><left half byte>
>     1 <skip 4 bits><3 bit dispatch><right half byte>
>
> The dispatch encodes the following information:
>
>     x00 left half, 17th bit is 0
>     x01 right half, 17th bit is 1
>     x10 IM right half, 17th bit is 0
>     x11 IM right half, 17th bit is 1
>
> If x = 0 THEN use correct parity ELSE write bad parity.

The Dorado-side decoder is `BootByteL` (lines 167–177) and
`Write000`..`Write111` (lines 181–204).

## Source citations

### 1. BB-side hunk format — `doradoboot.masm` (CHM `doradobaserom.dm!12_/`)

Comment block immediately above `SendAHunk:` in the BB ROM source
(file is single-line; quoted with line breaks added):

```
A hunk is a sequence of 17 bytes containing four microinstructions
packed as follows, from high- to low-order bit in each byte:

  Byte 0:  0RSTK.0  0BLOCK  1RSTK.0  1BLOCK  2RSTK.0  2BLOCK  3RSTK.0  3BLOCK
  Byte 1:  0RSTK.1  0RSTK.2  0RSTK.3  0ALUF.0  0ALUF.1  0ALUF.2  0ALUF.3  0BSEL.0
  Byte 2:  0BSEL.1  0BSEL.2  0LC.0    0LC.1    0LC.2    0ASEL.0  0ASEL.1  0ASEL.2
  Byte 3:  0FF.0    0FF.1    0FF.2    0FF.3    0FF.4    0FF.5    0FF.6    0FF.7
  Byte 4:  0JCN.0   0JCN.1   0JCN.2   0JCN.3   0JCN.4   0JCN.5   0JCN.6   0JCN.7
  Bytes 5..8   = instr 1 (RSTK.1..7, ALUF, BSEL, LC, ASEL, FF, JCN — no RSTK.0/BLOCK)
  Bytes 9..12  = instr 2 (same)
  Bytes 13..16 = instr 3 (same)
```

> "We assume without checking that the Dorado will take the data as fast
> as we slap it into the CP register. The presence of new data is
> signalled by a change in the high-order bit of the CP register. The
> Dorado will have a minimum of 25 µsec from a change in the high-order
> bit of CPReg until the data in the low-order byte of CPReg goes away.
> Data will be sent at about 200 µsec per microinstruction."

### 2. BB-side per-half streaming — `SendAHalfMicroInstruction`

Same file. Streams **two CPReg bytes per microinstruction-half**:

```
SendAHalfMicroInstruction:
  ; — first CPReg byte (raw hunk byte) —
  LDA@Y Hunk          ; pick up next hunk byte
  INY
  STA  MCPBusH        ; payload high
  LDAI 80+ABMux1      ; good parity, latch via ABMux1
  STA  MCPBusL
  INC  MCPBusL        ; strobe → CPReg[low]
  DEC  MCPBusL
  LDAI 0
  STA  MCPBusH
  LDAI ABMux0         ; latch via ABMux0
  STA  MCPBusL
  INC  MCPBusL        ; strobe → CPReg[high]
  DEC  MCPBusL
  ; — second CPReg byte (next raw hunk byte) —
  LDA@Y Hunk
  INY
  STA  MCPBusH
  LDAI 80+ABMux1
  STA  MCPBusL
  INC  MCPBusL
  DEC  MCPBusL
  LDA  MicroHalf      ; control byte: 40 left, 41 right, OR 02 if breakpoint
  ASL  ExtraBits      ; pulls one parity bit per call from a packed byte
  ROLA
  STA  MCPBusH
  LDAI ABMux0
  STA  MCPBusL
  INC  MCPBusL
  DEC  MCPBusL
  RTS
```

So per microinstruction-**half** the CPReg sees four ABMux events:

```
  ABMux1 ← raw_byte_n      ; first low-byte payload
  ABMux0 ← 0               ; CPRegH clear / not-ready phase
  ABMux1 ← raw_byte_n+1    ; second low-byte payload
  ABMux0 ← ((MicroHalf << 1)|extra_bit) ; CPRegH control phase; signals ready
```

Eight ABMux events per full microinstruction. The `0x80` on ABMux1 is
the MCPBusL SetSS/parity bit used by the BaseBoard-side strobe; it is
not written into CPReg. The Dorado-visible data-ready transition is the
final ABMux0 write, where CPRegH receives `(MicroHalf << 1) | extra_bit`
(`0x80`/`0x82`, plus one shifted bit from `ExtraBits`). After bytes 0..4 the
hunk's first instruction has been fully delivered (LH then RH);
bytes 5..8 deliver instruction 1, etc. Instruction 0's RSTK.0+BLOCK
came from the shared byte 0, so the four instructions in a hunk
share one byte-0 carrier.

### 3. Dorado-side decode — `BootstrapMain.mc`

The Bootstrap microcode reads `T ← ~CPReg` (B←RWCPReg) and assembles
each microinstruction-half from two CPReg bytes via shifter ops.
Specifically: ReadBB1 → LSH[T,10] → ReadBB2 → LDF[T,10,0] → XOR Byte1
in our notes. Need to walk this side-by-side with the BB stream
in §2 to confirm bit alignment. (Detail captured in `handoff.md`
§2a.)

### 4. Hunk source addresses

Boot0 and Boot1 hunk tables live in `BootBlocks` / `BootCode`
.LOC sections of `doradoboot.masm`. The actual byte streams come
from constants `Boot0Data`, `Boot1Data` declared elsewhere — likely
in `doradobaserom-files.cm` or similar build script. **TODO**:
locate which build-time-bound binary supplies these (must look at
either the `.cm` files or an absolute layout map produced by the
6502 assembler).

The BB ROM image we have is the assembled output: bytes
**$C000–$D7FF** in `chm/dorado/doradobaserom.mb!13` are the data
hunks. Bytes **$F000–$FD2A** are the BB program code; the rest of
F000–FFFF is zero-filled padding ending in the reset/IRQ vectors at
$FFFA–$FFFF.

Verified by `bbdis`:

```
$ ./build/bbdis --vectors chm/dorado/doradobaserom.mb!13
; NMI vector   ($FFFA): $F000
; RESET vector ($FFFC): $F3A7
; IRQ vector   ($FFFE): $F2A2
```

## Spec (provisional)

The CHM source confirms the BB packs **4 microinstructions per 17-byte
hunk**, sharing RSTK[0]/BLOCK in the leading byte. Our current
Bootstrap probe almost certainly computes IM addresses correctly per
microinstruction, but the *per-microinstruction byte stream* the
Dorado-side Bootstrap.mc decodes assumes a different field packing
than what the BB streams when one resolves the hunk's byte 0 across
all four instructions.

Specifically:
- Bytes 1–4 of a hunk are missing RSTK.0/BLOCK; those come from
  hunk byte 0 bits {7,6}.
- Bytes 5–8 missing RSTK.0/BLOCK; come from hunk byte 0 bits {5,4}.
- Bytes 9–12: bits {3,2}.
- Bytes 13–16: bits {1,0}.

Bootstrap.mc must reconstruct the full 34-bit microinstruction by
combining the shared byte-0 bits with the per-instruction 4-byte
packet. Our model probably treats every received microinstruction's
RSTK.0 and BLOCK as zero. Those bits do not live in the two normal
per-half payload bytes; they come from the shared hunk byte 0 and are
selected by the CPRegH control byte written through ABMux0. That is
exactly what the observed `LH=0x0044` artifact looks like
(default-decode with RSTK.0=0, BLOCK=0).

## Cross-check / deltas

- HM §4.8 Write IM specifies IM-half write semantics from
  `Link[4:15]`, `B[0:15]` per call. The BB-side packing exists at a
  *layer above* Write IM: Bootstrap.mc reconstructs (RSTK.0, BLOCK)
  from the shared byte-0 bits and uses a *different* IRTable entry
  for each (LH, RH) × (RSTK.0, BLOCK) combo:
  - `IMLHRSTK.0Is0#`, `IMLHRSTK.0Is1#` (LH variants)
  - `IMRHBLOCKIs0#`, `IMRHBLOCKIs1#` (RH variants)
  These are visible in `doradoboot.masm`'s `IRTable`. Our model
  treats Write IM as one operation; the BB-driven path uses **four
  distinct microinstructions** depending on RSTK.0/BLOCK.
- Confirmed in `BasicCedarDorado.pb` Boot0 layouts that hunk byte 0
  shares those bits.

## Implementation (sketch)

### When to investigate further

After **Phase 1** lands. Phase 1 items (B6, B7, C4, C5, G1, D1, B11)
are local edits that don't depend on this. After they're in, return
here with the BB ROM disassembly + the Bootstrap.mc walkthrough.

### Plan

1. Locate `Boot0Data`/`Boot1Data` source in the BB build chain
   (`doradobaserom-compile.cm`, `doradobaserom-files.cm`).
2. Disassemble the byte stream that produces Boot1 in `chm/dorado/
   doradobaserom.mb!13` $C000–$D7FF, decoded as 17-byte hunks.
3. Produce the canonical microinstruction sequence the BB will deliver
   into IM[$0o6100..$0o7124].
4. Compare to canonical `Initial.mb` decoded fields. If they match
   bit-exact, the bug is purely in our **decode side**: the
   per-half decode in cpu.c's Write IM path doesn't merge the shared
   RSTK.0/BLOCK bits from the hunk's byte 0.
5. Fix: cpu.c handles Write IM correctly *given a 16-bit B value*.
   The fix is at the **Bootstrap.mc execution level** — our IRTable
   IMLH/IMRH dispatch needs to honor RSTK.0/BLOCK from the right
   place. Trace one full BootByteL iteration with the BB stream
   captured cycle-by-cycle.

## Verification

- **Synthetic test**: hand-construct a 17-byte hunk of 4 known
  instructions (with non-trivial RSTK.0/BLOCK), feed it through the
  BB→Bootstrap path with traced IM writes, expect bit-exact match
  to canonical decoded form.
- **Probe**: `probe_full_boot_with_bootstrap` should produce 896
  IM half-writes that match `Initial.mb` byte-for-byte. After fix,
  remove the BOOTSTAGE2 substitution shim from `tests/test_cpu.c`.

## Artifacts

- `chm/disassembly/bb_C000-D7FF.s` — Boot0/Boot1 data region (4808
  lines). Use this to read out the hunk bytes.
- `chm/disassembly/bb_F000-FFFF.s` — BB program code (2336 lines).
  `SendAHunk` and `SendAHalfMicroInstruction` are visible here.
- `dorado/build/bbdis` — disassembler tool. Re-run with
  `--start XXXX --end YYYY chm/dorado/doradobaserom.mb!13` to dump
  any region.

## Diagnostic findings (2026-04-30)

A new diagnostic in `probe_full_boot_with_bootstrap` measures
streamed-vs-canonical IM content **before** the BOOTSTAGE2
substitution. Baseline result with the current "no-invert during
BB free-run" band-aid:

```
A1 stream-vs-canonical (pre-substitution):
  canon=4031 streamed=896 match=95 differ=800 first_diff=0o6100
```

So 95/896 streamed entries match canonical Initial.mb. First
divergence is at 0o6100 — the very first written address.

### Sample diffs

```
[0o6100] streamed iw0=0x0044 iw1=0x7341 iw2=0x0000  canon iw0=0x0084 iw1=0x6005 iw2=0x0000
[0o6101] streamed iw0=0x0044 iw1=0xB3C5 iw2=0xC000  canon iw0=0x00B4 iw1=0x2F47 iw2=0x4000
[0o6102] streamed iw0=0x0044 iw1=0x19C1 iw2=0x0000  canon iw0=0xE0B4 iw1=0x2340 iw2=0xC000
```

Streamed iw0 is the default-decode `0x0044` (NoOp) for many
addresses — what `dorado_decode_model1` produces from a near-zero
byte.

### Loc-computation analysis

The first 4 ReadBB calls Bootstrap makes set `Loc` (start IM
address) and `Cnt` (hunk count). Per BootstrapMain.mc:

```
Loc_ LSH[T, 10], Call[ReadBB];     * Bytes 0, 1 = loc
T_ LDF[T, 10, 0];
Loc_ (Loc) XOR T, Call[ReadBB];
```

Reading the .mb-compiled forms via `mbdis --disasm bootstrap.mb`
and walking through `cpu.c::shifter_output`: `LSH[T, 10]` (octal
10 = 8 bits) compiles to `T << 8`, `LDF[T, 10, 0]` to `T & 0xFF`.

So `Loc = (T1 << 8) XOR (T2 & 0xFF)`.

The BB sends Boot1IMLoc = `0x0C40` (= 0o6100) as 2 bytes per
`SendIMBlockToDorado`:
- `Hunk[0] = 0x40` (low byte of address)
- `Hunk[1] = 0x0C` (high byte)

With current no-invert path:
- T1 = CPReg = 0x0040 (sync=0, data=0x40).
- T2 = CPReg = 0x800C (sync=1, data=0x0C).
- Loc = (0x0040 << 8) XOR (0x800C & 0xFF)
      = 0x4000 XOR 0x0C
      = **0x400C** — but we want **0x0C40**.

Bytes are *byte-swapped* relative to the desired result. With
always-invert (= spec), Loc = 0xBFF3 — also wrong.

### Hypothesis

Three candidates, in decreasing likelihood:

1. **BB byte order is opposite from the .masm read.** The
   `LDA IMAddress,#HighAddrByte; STA Hunk+1; LDA IMAddress,#LowAddrByte;
   STA Hunk` sequence in `doradoboot.masm` may have semantics
   different from "Hunk[0] = low, Hunk[1] = high" when the assembler's
   `#HighAddrByte` / `#LowAddrByte` macros are taken into account.
   Need to find the macro definitions.
2. **Our LSH/LDF semantics for `LSH[T, 10]` and `LDF[T, 10, 0]`
   are inverted.** The first argument `10` may be interpreted in
   the wrong direction. Cross-check by examining BootstrapMain.mc
   uses of these macros against the disassembled bootstrap.mb.
3. **Both, partially compensating** — and the no-invert path
   produces 95 correct entries by coincidence (e.g., entries whose
   bytes are palindromic or whose XOR partners cancel).

### Next actionable step

Per-cycle trace one full `BootByteL` iteration with:
- T pre/post each microinstruction
- CPReg value at each ReadBB
- the resulting IM write address + iw0/iw1/iw2

Compare line-by-line to BootstrapMain.mc lines 167-204. The first
divergence identifies the bug.

The 95-vs-896 diagnostic line lives in `tests/test_cpu.c`
post-this-session, so the metric is observable directly on every
run.
