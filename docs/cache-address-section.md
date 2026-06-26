# Cache Address Section (MEMC) — schematic findings + CATUP fix

> **RESOLVED (2026-06-25): the bug was NOT the cache-address presentation.**
> The `dVA←Victim`→Pipe0/Pipe1 round-trip is correct (`getPipeCacheABits` of
> `(tag<<10)|(row<<4)` recovers the tag; the diagnostic constants are octal, so
> there was no 2-bit offset). The real cause was the §3.12 **TASKSIM** tick
> firing without honoring the `0o10` enable bit: after `cacheAddrTest` calls
> `disableConditionalTask`, a stale re-arm value kept waking the memory-simulator
> task (0o12), whose `noRef`+`useMcrV` STORE clobbered the just-zeroed cache.
> Fixed in `dorado/src/cpu.c` (gate the tick on the enable bit). memA now reaches
> DONE. **2026-06-26 update:** the broader diagnostics now pass with the
> harness commands in [`running-diagnostics.md`](running-diagnostics.md); older
> references in this note to kernel/eventCounters failures are superseded. Games
> (test_snapshot digest) and Cedar were unchanged by the CAT fix. The CAT source is mirrored at
> `chm/doradosource/diagnostics/memASource/memRWc.mc`. The schematic notes below
> are retained as a correct reference for the cache-address datapath, but they
> were not load-bearing for this fix.

**Date: 2026-06-24.** Working notes for fixing memA's `CATUPADDRERR` (the Cache
Address Test). The bug is in how a `dVA←Victim` read presents the cache address
in Pipe0/Pipe1; this captures what the MEMC schematic and the diagnostic source
say so the fix can be tried/confirmed without re-deriving it.

## Sources

- **Schematic:** `DoradoDocs/schematics/MEMC.pdf` — the cache + cache-address
  board (38 pages; `MemC01.sil` "Main data paths" is the key sheet). The PDFs
  are **vector / text-extractable**: `pdftotext -layout MEMC.pdf` gives signal
  names (the 2-D layout scrambles, but names + bit numbers survive). Render
  pieces with `pdftoppm -png -r 300 -f <pg> -l <pg> -x -y -W -H MEMC.pdf out`
  (page 2 = MemC01; full page ≈ 1880×2425 px at 220 dpi).
- **Diagnostic:** `chm/doradosource/diagnostics/diagnosticSubrs/memSubrsC.mc`
  (`getPipeCacheABits`, `setBrCacheABits`, `chkPipeRow`), `memDefs.mc` (the
  cache-geometry constants), `memSubrsX.mc` (`xGetConfig`).
- Bit numbering: HM is **MSB-first** (VA bit 4 = MSB of the 28-bit VA, bit 31 =
  LSB). Our C model is **C-LSB** (bit 0 = LSB). So **HM bit N = our bit (31−N)**.

## Cache geometry (memDefs.mc) — memA expects 4K, which we already model

```
cacheShift     = 4      ; 16-word munch (offset = 4 bits)
nBitsInRow     = 6      ; 64 rows  => 4K cache (NOT 16K)
cacheRowMask   = 1760₈  ; = our C bits 4..9  (matches our (va>>4)&0x3F)
nBitsInCache   = 16     ; cache address is 16 bits (VA[6:21])
skipCacheShift = 10     ; cacheShift + nBitsInRow
```

So a 4K↔16K cache config knob is **not** needed for CATUP — memA is 4K and our
`DM_CACHE_ROWS=64` already matches. (A config knob is still faithful future work;
the 16K strap is "remove chips for Aad bits 0-1", Aad = VA[20:27].)

## Cache-address section (MemC01.sil), as read from the rendered sheet

- **Cache A Memory:** 256 rows × 4 cols (4K = 64×4), MB071 chips. Each entry
  stores **`CVA{0..3}.6/21` = VA[6:21]** (16 bits), addressed by
  **`Aad.0..7` = VA[20:27]**, write-enable `WE'`, column selects `BS0'..BS3'`.
- **Read-out:** the victim column is muxed (MC174) to **`VictAd.6/21 → pipe`**.
- **Hit compare:** "1 out of 16 bits (**7-21** or 4-19)".
- **VA bit wiring:** VA bit *N* (HM) ← `RMar.(N−16)` for the low half
  (`dVA.21←Mar.05`, `.27←Mar.11`, `.31←Mar.15`); high bits 4-15 from BR.
- **THE KEY (top of the data-path sheet):**
  > **"PipeVA.4-15 are wire-ored with PipeVA.20-31"**

  i.e. the pipe VA bus folds: PipeVA bits 4-15 share wires with bits 20-31. In
  Pipe0/Pipe1 terms (Pipe0 = VA[0:15], Pipe1 = VA[16:31]) that is
  **Pipe0[4:15] (VA[4:15]) wire-ORed with Pipe1[4:15] (VA[20:31])**. This fold is
  why a plain VA-split pipe gives the wrong cache-address bits.
- **Config straps:** "cut the 173 leg … for 4k pages"; "For 256-wd pages
  20-23+Hi / 1k pages 20-21+Hi"; "remove chips for bits 0-1 if cache is 4k".
  The tag/row boundary and the high-VA placement in PipeVA move with page+cache
  size. *Open: which page-size strap is this machine — 256-word ("20-23+Hi")?*

## How the diagnostic reads the cache address (memSubrsC.mc)

```
getPipeCacheABits:                 setBrCacheABits (T = 15-bit cache addr):
  t   = pipe0 AND 377₈               rscr  = rsh[t,6] AND 377₈     -> BrHi
  rscr= pipe1 AND 176000₈            rscr2 = lsh[t,12] AND 176000₈ -> BrLo
  t   = lsh[t,6]                    chkPipeRow (T = row):
  rscr= rsh[rscr,12]                 rscr = cacheRowMask0|1   (= bits 4..9)
  t   = t OR rscr                    check  PIPE1 row-bits == row<<cacheShift(4)
  return t AND 37777₈
```
Constants: `CABitsInPipe0Mask=377₈` (8 bits), `CABitsInPipe1=6`,
`CABitsInPipe1Mask=176000₈` (bits 10-15), `CABitsInPipe1Shift=12`,
`CABitsMaskC=37777₈` (14 bits). So: Pipe0 low byte = cache-addr HIGH bits;
Pipe1 bits 10-15 = cache-addr LOW bits; **the row lives at Pipe1[4:9]**.

## The bug, measured

Instrumenting our `dVA←Victim` path (memory.c ~1007) on a live memA run, entry
[row 0, way 0]:

| our pipe_va (munch VA) | stored tag (va>>10) | Pipe0 | Pipe1 | getPipeCacheABits |
|---|---|---|---|---|
| `0010000` | **4** | 0 | `010000` | **1** |
| `0130000` | **54** | 0 | `130000` | **013** |

Our reconstruction yields `pipe_va>>12`; the test expects the tag `pipe_va>>10`
— **off by 2 bits**, which is the observed `Errors` = 4 (bit 2). The cause is the
missing **PipeVA[4:15]⊕[20:31] wire-OR** (and possibly the page-size strap): we
present an un-folded VA, so `getPipeCacheABits` lands on the wrong bits.

## Fix plan (empirical, memA is the oracle)

In `dorado_memory_ref_task`'s `dVA←Victim` branch, build the pipe VA **with the
wire-OR fold** so `getPipeCacheABits` reconstructs VA[6:21] and `chkPipeRow` sees
the row at Pipe1[4:9]. Candidate transforms to try against memA:

1. Pack directly as `setBrCacheABits(CA)` with `CA = (va>>10)&0x3FFF`, plus row
   at Pipe1[4:9]: `pipe0=(CA>>6)&0xFF`, `pipe1=((CA<<12)&0xFC00)|((va>>4)&0x3F)<<4`.
2. Apply the explicit fold: `pipeVA = VA | foldbits`, where VA[20:31] is OR-ed
   onto VA[4:15], then split Pipe0=VA[0:15]/Pipe1=VA[16:31] as today.
3. Vary the page-size strap offset (256-word "20-23+Hi" vs 1k "20-21+Hi").

Validate each by: memA `CATUPADDRERR` clears or its `Errors` bit moves; then
confirm **InitMem/boot** still read cache addresses through the same path
(`ClearCacheFlags`, `dVA<-Victim`) and the regression gates stay green.

### UPDATE (2026-06-24): the `dVA←Victim` hypothesis is RULED OUT

Tried all three packings above behind `DORADO_CAFIX={1,2,3}`: **every one failed
at the identical step (87553) and PC (`CATUPADDRERR` 0o4232)** — zero effect. So
the failing CAT comparison does **not** read through the `dVA←Victim` pipe path.
The only `dVA←Victim` reads observed near the failure (4 of them, qva=0 way=0)
are not the comparison that sets `Errors`.

Corroborating: `memMemA.mc`'s nearby pipe reads (`sLongFetchErr0-5`) read the
**normal reference's** Pipe0/Pipe1 (a `LongFetch`'s VA), not a cache-address
read. So the CAT test very likely reads cache state through a **normal
reference's pipe** (or a different muffler/`Pipe5`/CFlags path), and the bug is
there — not in `dVA←Victim`.

**Next step (engine-level):** the diagnostic's `Errors` register (RM[6], = 4 =
bit 2 at the failure) is set by a comparison a few instructions before 0o4232
(the trail runs `...6650(shift) 6630 6634 6670 6654(T←RM[6]=4) 6625(BLOCK) 6674
4224 4232`). Instrument the engine to capture what that comparison reads vs.
expects (watch RM[6] writes / the Pipe0/Pipe1 or muffler read feeding it) to
identify the actual readback path before attempting another packing. The
schematic findings above (Cache A = VA[6:21], Aad = VA[20:27], the
PipeVA[4:15]⊕[20:31] wire-OR) remain valid and will inform whichever path it is.

### UPDATE 2 (2026-06-24): sharper — NORMAL-reference Pipe VA, 2-bit offset

Ran the engine traces under rundiag (`dorado_trace_gate=1`) and followed it:

- `Errors` (RM[6]) becomes 4 in the **showS / Pattern3 storage test** region
  (labels `GETSP3ODD/EVEN`, `GETSP4`, `GETCURSPATTERN`, 0o3347-0o3370) — not a
  cache-address-memory test.
- The reads feeding it are **`B←Pipe0` (pc 0o3300)** and **`B←Pipe1`
  (pc 0o3354)** of a **normal reference** (psrn=0; **no DMUX/muffler reads**).
  For the failing entry the pipe VA = `0x1000`.
- Decoding our (correct) pipe with `getPipeCacheABits` (MSB-first:
  `pipe1&176000₈`=VA[16:21], `rsh 12` keeps VA[16:19]; `pipe0&377₈`=VA[8:15],
  `lsh 6`) gives **VA[8:19]** of the va — but the test expects the cache address
  **VA[6:21]**: a **2-bit window offset**. (va `0x1000` = HM bit 19: VA[8:19]
  reads it as value 1, VA[6:21] as value 4 = the bit-2 `Errors`.)

So the bug is the **pipe VA presentation for NORMAL references**, in the bit
window the storage test checks — almost certainly the **page-size strap**: the
sheet says *"For 256-wd pages, 20-23+Hi; for 1k pages, 20-21+Hi"*, and HM 20-21
(= our C bits 10-11) sit exactly in `getPipeCacheABits`'s Pipe1[10:15] read. Our
config is 256-word pages, so the high-VA 20-23 bits should map into PipeVA
shifted vs. our straight `Pipe1 = va&0xFFFF` — the missing 2-bit shift.
**dVA←Victim and cache-address-memory readback are ruled out** (the failing read
is a normal `B←Pipe0/Pipe1`); the Cache A geometry above is correct but off-path.

**Next:** read the PipeVA assembly sheet (MEMC pg 34) for the exact "20-23+Hi"
page-strap wiring (which VA bits land where in Pipe0/Pipe1 for 256-word pages),
apply that to our pipe VA, and verify CATUP clears AND the boot/games (which
read VaHi/VaLo) stay green. NB: changing the normal-reference pipe VA touches
every world — validate the regression gates carefully.

### UPDATE 3 (2026-06-24): ROOT CAUSE — page-size config mismatch

The 2-bit offset is a **page-size mismatch**:

- **memA assumes 1024-word pages:** `memDefs.mc` `set[nBitsInPage, 10]` (2^10).
- **Our model uses 256-word pages:** `memory.h` `DM_PAGE_SIZE = 256`
  (chosen to match the Alto/Mesa world, which reaches real pages valid only on
  the 256-word, 16MW config).

The schematic's page-size strap is exactly this: *"For 256-wd pages 20-23+Hi /
for 1k pages 20-21+Hi"* — the high VA (page-number) bits land in **PipeVA**
shifted by **2 bits** between the two page sizes. `getPipeCacheABits` reads
Pipe1[10:15] = VA[16:21], whose top bits (VA[20:21]) sit right on that boundary,
so a 1k-page diagnostic reading our 256-page pipe VA is off by 2 = the bit-2
`Errors`.

**So this is not an engine bug — it is a configuration the emulator doesn't
support.** memA was built for a 1024-word-page Dorado; the Alto world we boot is
a 256-word-page Dorado. They are different machine builds.

**Recommended fix (ties to the earlier "make it a command-line option" idea —
but for PAGE/MAP size, not cache size):** parameterize the map/page
configuration (HM Table 16: page sizes 256/1024/4096, map IC 16K/64K/256K) the
way storage modules already are, and run memA with its expected **1024-word
page** config. The cache geometry itself (64 rows × 4 × 16) is unchanged; only
the VA↔map↔pipe bit boundaries move. Default stays 256-word (the Alto/Cedar
worlds); a `--page-size`/`DORADO_PAGE_WORDS` knob selects 1024 for the mem
diagnostics. Validate the Alto/Cedar boot is byte-identical at 256-word and that
CATUP (and the later mem subtests) clear at 1024-word.

### UPDATE 4 (2026-06-24): page-size config built; CATUP needs the PIPE strap too

Parameterized the page/map size (`dorado_page_shift()` in memory.c; map index,
offset, phys all derive from it; `DORADO_PAGE_WORDS`, default 256). At that time
the quick gates were byte-identical at 256-word; current ground truth is stricter:
`make test` passes, Galaxian is 121553 px, and memA passes. The later
diagnostic status is maintained in [`running-diagnostics.md`](running-diagnostics.md).

**But running memA at `DORADO_PAGE_WORDS=1024` fails identically** (same step
87553, same `CATUPADDRERR`). So the *map* page-size change alone does NOT fix
CATUP — confirming the bug is specifically the **pipe-VA presentation**, which is
page-size-dependent but separate from the map index. The pipe stores the raw VA;
`getPipeCacheABits` extracts fixed bit positions; the page-size config doesn't
touch those.

Precise characterization of the pipe strap (the remaining piece):
`getPipeCacheABits` = `((pipe0&0xFF)<<6) | ((pipe1&0xFC00)>>12)` produces result
bits **0-3** (from Pipe1) and **6-13** (from Pipe0), with a **2-bit gap at bits
4-5** (the cache-row interleave, checked by `chkPipeRow` at Pipe1[4:9]). So the
hardware's pipe VA is a **non-contiguous, row-interleaved, page-size-strapped**
arrangement — NOT the raw VA we store. Modeling it must reconcile two readers of
the same pipe: `getPipeCacheABits` (cache address) AND `B←VaHi/VaLo` (full VA,
which the boot's fault path reads) — the schematic's `PipeVA.4-15 ⊕ 20-31`
wire-OR is what reconciles them on hardware. That is the load-bearing wiring to
read off MEMC pg 34/the data-path sheet, and the change is **boot-risky** (it
alters VaHi/VaLo for every world), so it needs careful byte-identical validation.

State: page-size config = done (foundation, also needed by memMisc's map test).
CATUP fix = the pipe-VA strap, blocked on the exact MEMC pipe-assembly wiring +
boot validation.

## Open questions for the board owner

1. Confirm the wire-OR direction: **PipeVA[4:15] OR PipeVA[20:31]** (VA[20:31]
   folds down onto the VA[4:15] wires)?
2. Which page-size strap is this machine (256-word `20-23+Hi`)?

(Render `pg 34` "PipeVA assembly" and `pg 8/9` "VictAd" for the exact fold wiring
if the empirical trials don't converge.)
