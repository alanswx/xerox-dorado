# Schematic vs. Emulator Audit — Processor data path

Scope of this pass: I read the high-resolution board drawings in
`DoradoDocs/doradodrawings/` as images and cross-checked them against the
C emulator (`dorado/src/cpu.c`, `disasm.c`) and the Sep-1981 Hardware
Manual (both the PDF and the markdown at
`DoradoDocs/dorado-hardware-manual/Dorado_Hardware_Manual.md`).

Sheets read as raster images and analyzed signal-by-signal:
- **ProcL sheet 14** (page 16) — "A" Multiplexor & Mar mux control
- **ProcL sheet 17** (page 19) — Q Register & Count Register / Task Simulator
- **ProcL sheet 18** (page 20) — Shift Register Control (the ShC transform table)
- **ProcH sheet 10** (page 12) — Arithmetic Logic Unit (carry chain, SignedCarry, fast-branch on Rm<0)
- **ContA sheet 03** (page 5) — JCN Decoding
- **ContA sheet 23** (page 25) — Ready Logic
- **ContA sheet 24** (page 26) — Wakeup Priority Encoder
- **ContA sheet 25** (page 27) — Task Switch Generator

(Plus a Memory pass against `memory.c` + the HM Memory Addressing/Map
sections; MemX/MemD/MemC raster not yet read.) The two processor findings
below remain the only suspected discrepancies; the Control and Memory
circuits checked this pass were all consistent (see the second-pass
section near the end).

(Page = sheet + 2 in the upper-numbered region; verified by reading title
blocks.) I also used the text-layer TOCs of `DoradoDocs/schematics/ProcL.pdf`,
`ProcH.pdf`, `IFU.pdf` to navigate, and Hardware Manual §3.7 (ALU), §3.11
(Shifter), Tables 9/12/13.

**Top suspicions, highest first:**
1. The `Overflow` branch condition (FF=067) is unimplemented — it silently
   evaluates **false always**, even though the ALU overflow flag is computed.
2. The barrel-shifter applies its mask **before** the ALU (folded into the A
   bus), whereas the hardware masks in the Pd multiplexer **after** the ALU;
   this makes the ALU=0 / ALU<0 branch conditions see masked data (manual
   says they must see unmasked data), and also mis-orders masking vs. the
   ALUFM-17 computed-op shift path used by BitBlt.

The processor A-mux/Mar decode, the ShC field-descriptor transform, the
masker bit-numbering, the ALU op table, the Multiply/Divide/CDivide Pd&Q
effects, and the B-bus constant forms were all checked against the drawings
and found consistent (see the last section).

---

## Finding 1 — `Overflow` branch condition is never evaluated (always false)

- **Board / sheet:** ProcH sheet 10 (page 12), "Arithmetic Logic Unit" —
  the `SignedCarry` net out of MC170 (d13), fed by aluF0, alua.00,
  alub.00a, alu.00, aluCout. `SignedCarry` is the hardware Overflow
  signal latched into the branch-condition RAM (ProcH sheet 11, "Branch
  condition Ram").
- **What the hardware does:** HM Table 13 lists eight branch conditions
  selected by `JCN[5:7]` / `FF[5:7]`:
  `0 ALU=0, 1 ALU<0, 2 ALUcarry', 3 Cnt=0&-1, 4 R<0, 5 R Odd,
  6 IOAtten'/ReSchedule, 7 Overflow` (FF=067). Overflow is *only*
  reachable through the FF encoding `FF=067` (FA=0, FB=6, FC=7), because
  JCN low-3 = 7 is consumed by the return/IFU/long class — so a microprogram
  that wants Overflow must encode it in FF.
- **What the emulator does:** `cpu.c:2143 eval_branch_condition()` handles
  only `case 0..6` and falls to `default: r = 0`. There is no `case 7`.
  The Overflow flag itself *is* computed and committed:
  `cpu.c:2015/2023 overflow = (sa == sb && sa != sr)`, stored to
  `cpu->alu_overflow` (`cpu.c:3463`) and saved/restored per task
  (`cpu.c:190,208`) — but `alu_overflow` is **never read** anywhere
  (grep confirms only writes). The FF-condition OR path in `next_pc`
  (`cpu.c:2406` and `cpu.c:2411`, the `FB==6` test) does call
  `eval_branch_condition(cpu, u, u->ff & 7)` for `FF=067`, but that
  returns the `default 0`.
- **Possible error / discrepancy:** Any microcode branch on Overflow
  (`FF=067`) always takes the false/fall-through target. Mesa integer
  arithmetic opcodes that trap on signed overflow, and BitBlt/computed
  paths, would silently never detect overflow. This is a clean,
  high-confidence gap: the value is already maintained; only the
  `eval_branch_condition` dispatch is missing.
- **Confidence:** **High.** Confirmed by HM Table 13 (Overflow = FF=067)
  and by grep showing `alu_overflow` has no readers. To confirm impact,
  scan the emulator microcode (`chm/dorado/*.mb`) for FF=067 uses.

---

## Finding 2 — Shifter mask applied before the ALU instead of in the Pd mux

- **Board / sheet:** ProcL sheet 18 (page 20) "Shift Register Control"
  and ProcL sheet 19 "Shifter multiplexors"; HM §3.11.
- **What the hardware does:** The 32→16 barrel shifter output is placed
  **complemented** on the A bus *unmasked*. The ALU runs (normally
  `ALUFM[16]` = NOT A, recovering the unmasked shifted data; `ALUFM[17]`
  is a *computed* op for BitBlt). The **masker lives in the Pd input
  multiplexer**, *after* the ALU: HM §3.11, "ALU output passes to the
  masking logic… Masked data is routed onto Pd." HM is explicit twice:
  - "the Pd input multiplexor does not affect the result of these branch
    conditions" (ALU=0/ALU<0/Carry'/Overflow) — HM p18/§3.7.
  - "On a shift the ALU branch conditions apply to the **unmasked** ALU
    output." — HM §3.11 (md manual line 1335).
- **What the emulator does:** `cpu.c:1713 shifter_output()` computes the
  cycled word `lo16` and then **applies the mask inside the shifter**:
  `return (lo16 & ~mask) | (fill & mask);` (`cpu.c:1773`). The A bus is
  then `~shifter_output` (`cpu.c:1922`), and the ALU output `alu` (used for
  `new_alu_zero`/`new_alu_lt0` at `cpu.c:3131-3132`) is therefore the
  **masked** value.
- **Possible error / discrepancy:**
  1. **Branch conditions:** On any masking shift (`ShiftLMask`,
     `ShiftRMask`, `ShiftBothMasks`, `ShMd*`), `ALU=0`/`ALU<0` reflect the
     masked field, not the unmasked shifted word — directly contradicting
     HM §3.11. E.g. a left-shift (`ShiftRMask`) that masks off the high bits
     would report `ALU<0 = 0` even when the unmasked shifted MSB is 1.
  2. **Computed-op shift (BitBlt):** when `ALUF[3]=1` selects
     `ALUFM[17]` (a computed op combining A and B, per HM §3.11 "used by
     BitBlt"), the emulator feeds the ALU an A input that already has the
     mask (and Md fill) merged in (`cpu.c:3054` forces
     `aluf_idx = 14 + (ALUF & 1)`, and A = `~shifter_output`). The hardware
     masks the ALU *output*, so for any non-"NOT A" ALUFM[17] op the Pd
     result can differ. The current code happens to be correct only when
     ALUFM[16/17] = NOT A (mask-then-invert == invert-then-mask for the
     identity-after-invert case).
- **Confidence:** **Medium-high** for the branch-condition half (HM is
  explicit). **Medium** for the BitBlt/ALUFM-17 half (depends on what op
  BitBlt loads into ALUFM[17]; verify with `AEmu.mb` BitBlt setup). A
  faithful model would have `shifter_output` return the **unmasked** cycled
  word for the A bus / branch conditions, and apply the LMask/RMask/Md
  merge as a separate Pd-mux stage after `alu_op`.

---

## Circuits checked and found consistent

These I read on the drawings and matched line-for-line to the emulator:

- **A-mux source decode — ProcL sheet 14 (page 16).** The drawing's "AMux
  decoding" table (`FF=0-17 → FF small const; FF=020 → R; 021 → T; 022 →
  Md; 023 → Q`) matches `ff_a_override()` (`cpu.c:579`) exactly
  (`fb<=1 → FF[4:7]`; `fb==2`: `0→R, 1→T, 2→Md, 3→Q`). The alternate
  memory-source select for `ASEL=2/3` ("ASEL=2or3 & FAmem: 0→Md, 1→IFU,
  2→Q, 3→T") matches `alt_mem_source()` (`cpu.c:1878`: FA `0→Md, 1→Id,
  2→Q, 3→T`). `ASEL=6→A←T`, `ASEL=5→A←Id`, `ASEL=4→A←R` all match
  `a_bus()` (`cpu.c:1892`).

- **ShC field-descriptor transform — ProcL sheet 18 (page 20).** The
  drawing's "These alu's perform" table —
  `RF: ShiftCount=P+S+1, RightMask=don't-care, LeftMask=16-S-1(=S');`
  `WF: ShiftCount=16-P-S-1, RightMask=16-P-S-1, LeftMask=P` with
  `P=A[8:11], S=A[12:15]` — matches `field_desc_to_shc()` (`cpu.c:1790`)
  exactly: WF `count=rmask=15-P-S, lmask=P`; RF `count=P+S+1,
  lmask=15-S, rmask=don't-care`. `ShC[2:3]` loaded from `A[2:3]` matches.

- **Masker bit-numbering and ops — HM §3.11 / Table 12.** "LMask = N ones
  starting at bit 0 (MSB); RMask = N ones starting at bit 15 (LSB)" matches
  `cpu.c:1754-1755` (`lmask = 0xFFFF << (16-N)` high bits; `rmask =
  (1<<N)-1` low bits). The eight `ALUF[0:2]` mask ops (NoMask/LMask/RMask/
  BothMasks, with `ShMd*` filling masked bits with Md) match
  `cpu.c:1760-1770`. The "first three ALUFM address bits forced to 1 →
  ALUFM 16/17, ALUF[3] selects" matches `cpu.c:3054`
  (`aluf_idx = 14 + (ALUF & 1)`, i.e. 0o16/0o17).

- **ALU op table — ProcH sheet 10 (page 12) + HM Table 9.** The 16 logical
  codes and 5 arithmetic codes in `alu_op()` (`cpu.c:1966`) match Table 9
  octal selectors; even codes = arithmetic, odd = logical
  (`alu_op_is_arithmetic`, `cpu.c:1959`) matches the manual. Carry-in =
  ALUFM bit 5 (`cpu.c:1970`). `Carry'` polarity (`eval_branch_condition`
  case 2 returns `!carry`, `cpu.c:2151`) matches "ALUcarry' = saved carry
  out'". Carry'/Overflow preserved on logical ops (`cpu.c:3099`) matches
  HM "change only on arithmetic ALU operations". Overflow computed only
  for A+B/A-B-1 (`cpu.c:2012,2022`) matches HM's explicit note that
  "Overflow is implemented correctly only for A+B, A+B+1, A-B, A-B-1".

- **Q register: Multiply / Divide / CDivide — ProcL sheet 17 (page 19) +
  HM §3.7.** Drawing shows QshiftR/QshiftL driven by FF=multiply / Rshift
  / DivideA / DivideS, with aluCout (gated by FFdly.7) entering the Q LSB
  on Divide/CDivide. Emulator matches HM p23 forms: Multiply
  `Result=ALUcarry,,ALU/2, Q=ALU[15],,Q/2` (`cpu.c:1004-1006`); Divide
  `Pd=ALU[1:15],,Q[0], Q=Q[1:15],,ALUcarry` (`cpu.c:1477-1479`); CDivide
  same with `ALUcarry'` into Q[15] (`cpu.c:1482-1484`). The Multiply
  `Q[14]→TNIA[14]` dispatch capture (`cpu.c:997`) matches §4.4.

- **B-bus constant forms — HM Table 7.** `b_bus()` cases 4-7
  (`cpu.c:1669-1680`): `0,,FF / 0377,,FF / FF,,0 / FF,,0377` match the
  disassembler names and Table 7. `BSEL[0]=1 with ASEL=7 forces B=Q`
  (`cpu.c:1649`) matches §3.11 "the B source is forced to be Q".

- **LongFetch VA formation — HM "Processor Memory References".** VA
  displacement `(B[4:15],,Mar)` for LongFetch (`cpu.c:3233`,
  `((b & 07777)<<16) | (mar & 0xFFFF)`) matches the manual's 28-bit form;
  ordinary refs use the 16-bit Mar. (Adder carry width checked at the
  cpu.c level only; the MemD/MemX BR-adder raster sheets were not read
  this pass.)

## Second pass — Control (ContA) and Memory, checked consistent

Raster sheets read this pass: **ContA 03** (page 5, JCN Decoding),
**ContA 23** (page 25, Ready Logic), **ContA 24** (page 26, Wakeup
Priority Encoder), **ContA 25** (page 27, Task Switch Generator). Memory
cross-checked against `memory.c` + the HM "Memory Addressing"/"The Map"
sections (MemX/MemD/MemC raster not yet read).

- **Wakeup priority encoder — ContA sheet 24 (page 26).** The encoder
  takes `ToPE.01..ToPE.17` (octal task numbers 1–15; "REQUEST NUMBERS IN
  FONT 2 ARE OCTAL") and emits `PEnc.0:3` = the **highest-numbered** task
  asserting ToPE. Matches `task_bnt()` (`cpu.c:123`) which scans
  `for (i=15; i>0; i--)` returning the highest set bit, default 0. Task 0
  has no ToPE input (always-lowest), matching `avail |= 0x0001`.

- **Ready logic / ToPE — ContA sheet 23 (page 25).** Each task has a
  `Ready.NN` latch (F00) whose input `SetReady.NN` is a 3-way wire-OR of
  Notify (wakeup), preEmpting, and Feedback (CTask). The priority-encoder
  input `ToPE.NN` is `Ready.NN` OR'd (MC1664) with the task-wakeup-request
  `TWReq.NN`. This matches the emulator's `avail = cpu->ready |
  cpu->wakeup_pending` (`cpu.c:247`).

- **Task switch decision — ContA sheet 25 (page 27).** `SwitchUp` fires
  when `PEncGtTrueNext'` (PEnc > the NEXT register) and not StopTasks/
  FreezeAC; `SwitchDown` fires "after Hold" when the current task asserts
  `Block'`. This matches `task_schedule()`'s
  `should_switch = (bnt > ctask) || block_in_non_emulator` (`cpu.c:252`),
  including the block→recompute-BNT-excluding-ctask path (`cpu.c:254-261`)
  for the switch-down-to-lower-priority case. **Caveat (already known):**
  the hardware compares against a *pipelined* NEXT register precomputed one
  cycle ahead, while the emulator recomputes BNT vs CTASK live each
  instruction. The emulator documents this and the wakeup-latency
  approximation as a tuned pair at `cpu.c:1231-1248`; not re-flagged.

- **BR / VA adder — HM §5.1.** `va = (br + disp) & 0x0FFFFFFF`
  (`cpu.c:3237`) with 28-bit `br` matches HM "The full virtual address
  VA[4:31] is BR[MemBase] + D, D unsigned" (md manual line 1902-1903).
  `BrLo←A` loads BR[16:31] and `BrHi←A` loads BR[4:15] from A[4:15]
  (`memory.c:1304,1322`) — consistent with HM p37. `IFetch←` replacing
  BR[24:31] with the IFU Id byte (`cpu.c:3216-3217`) matches HM
  "IFetch← … BR[24:31] are replaced by Id".

- **Map geometry — HM §5.1 / "The Map".** `dorado_map_index(va) =
  (va>>8) & 0xFFFF` = VA[8:23], 64K entries (`memory.c:613`) with a
  256-word page (offset VA[24:31]) is the **2^24-word (16 MW)**
  configuration. HM explicitly says "cache and map geometry limit usable
  virtual memory to only 2^22 or 2^24 words in most configurations" (md
  line 1907); the 2^22 config would index by VA[10:23] (16K entries, the
  value on md line 385). The emulator's choice is internally consistent
  (matches its `VirtualBanks=400₈` config word) — consistent, not a bug.

- **Cache geometry — HM §5.5.** 64 rows × 4 ways × 16-word lines = 4 KW
  (`memory.h:87-90`; row = VA[4:9], offset = VA[0:3], tag = VA[10:27] via
  `va_cache_row/offset/tag`, `memory.c:653-655`) matches "4K-word 4-way
  cache, 16-word munches".

- **Vacant map encoding — HM §5.** `va_translate` treats `WP=1 AND
  Dirty=1` as Vacant → `DM_FAULT_PAGE` (`memory.c:638`), and init sets all
  entries WP=1/Dirty=1 (`memory.c:118-119`). Consistent with the Table 16
  encoding the ReadMap packer cites (`cpu.c:893-901`).

## Third pass — IFU IFUM decode and Map real-page width

Verified against the canonical HM Table 18 (IFUM Fields), Table 19 (Operand
Sequence for ←Id), and §6.2 (the t0-of-opcode MemBase/RBase init rule).
(The IFU raster sheets implement these tables; the manual is project canon,
so this pass is manual + code. The IFUM bit layout is additionally
round-trip-validated by `LoadRam` and `B←IFUMRH'`/`B←IFUMLH'`.)

- **IFUM field decode — `ifu_decode_lh` (`cpu.c:2240`) + `ifu_decode_rh`
  (`cpu.c:2288`).** All seven fields and their bit positions match Table 18
  / Table 20 (IFUMRH←B layout): Sign(MSB bit0→C15), Length'(bits4:5→C11:10),
  RBaseB'(bit6→C9), MemB(bits7:9→C8:6), TPause'(bit10→C5), TJump'(bit11→C4),
  N(bits12:15→C3:0); and IFaddr' = `~rh & 0x3FF` (bits6:15, low-true),
  Packeda = rh bit5. Consistent.
- **Length' = notLength — confirmed against Table 18.** Table 18 calls the
  field `Length'` (low-true), legal lengths 1/2/3, 0 illegal. The emulator's
  `length = (~lpr) & 3` (`cpu.c:2255`) gives stored→length of 2→1, 1→2,
  0→3, 3→illegal — the field is `~length`, matching the recent notLength
  fix. Consistent.
- **MemBase / RBase init at t0 — confirmed against §6.2 (md line 3300).**
  HM: "MemBase to 0..MemBX[0:1]..MemB[1:2] if MemB[0]=0, or to 34₈ +
  MemB[1:2] if MemB[0]=1; RBase to RBaseB (0 or 1)." Emulator
  `ifu_decode_lh` (`cpu.c:2264-2277`):
  `MemBase = ((MemBX&3)<<2)|MemB[1:2]` else `034 + MemB[1:2]`;
  `RBase = !RBaseB'`. Exact match.
- **Jump displacement — confirmed against §6.2.** length=1 uses the 6-bit
  signed `Sign.Packeda.N` (range PC−40₈..+37₈); length=2 uses α
  (sign-extended iff Sign); length=3 jump illegal. Matches `cpu.c:2620-2632`.
- **←Id operand sequence — confirmed against Table 19.** `ifu_id_at`
  (`cpu.c:2321`) delivers `N (if N≠17₈), then α / α[0:3],α[4:7] (Packeda),
  then β (length 3), then Length forever`, with α sign-extended on the
  non-packed regular forms. Matches Table 19 row-for-row.

### Low-confidence note — ReadMap-on-Pd truncates RP to 13 bits
Earlier (second pass) I flagged a possible RP-width problem; the third pass
mostly resolves it. The map entry **does** hold a 16-bit real page
(`memory.h:133 uint16_t rp`; HM §5 says the map RAM is 19 bits = 16 RP +
WP + Dirty + Ref, md line 3466), and the hardware-faithful readback
`B←Pipe3'`/Map' (`dorado_pipe_map_rp_at`) returns all 16 bits — both
correct. The only truncation is the `ReadMap`-on-Pd path (FA=0 FB=3 FC=1,
`cpu.c:900`), which packs `WP,Dirty,Ref,RP[0:12]` and masks `e->rp & 0x1FFF`,
dropping the top 3 RP bits. Caveats that lower severity: (a) HM's *ReadMap*
is really a `Map←` modifier whose result is read from the pipe (16-bit,
correct), so delivering the entry on Pd is an emulator convenience; (b)
real storage ≤ 2 MW keeps RP within 13 bits anyway. Only bites if a world
maps real pages ≥ 2^13 *and* reads them via the Pd path. **Low confidence.**

## Fourth pass — MemX Map RAM (sheets 14–15) and IFU pipeline (sheet 01)

Raster sheets read: **MemX 14** (page 16, Map address and control),
**MemX 15** (page 17, Map address mux), **IFU 01** (page 3, Mem Req Logic).

- **Map RAM index width — MemX sheet 15 (page 17). Resolves the earlier
  open question definitively.** The map RAM address is **9 multiplexed
  bits** (`MapAd.0..MapAd.8`, muxed with `RfshAd.*` through MC158), and the
  map chips are "16k, 64k, or 256k × 1 MOS RAMs" (HM §5 / MemX note). So the
  number of significant map-index bits is **config-dependent: 14 (16K), 16
  (64K), or 18 (256K) bits**. The emulator's `dorado_map_index = VA[8:23]`
  → 16 bits → **64K entries** (`memory.c:613`, `DM_MAP_ENTRIES=64K`) is
  exactly the **64K×1 chip configuration**. The HM-prose "VA[10:23]" (14
  bits) is the 16K×1 configuration. Both are valid; the emulator models the
  middle one. **Consistent — no discrepancy.** (This supersedes the
  earlier "VA[8:23] vs VA[10:23]" worry.)
- **Map Dirty/WP interaction — MemX sheet 14 (page 16).** `MapFnc` decodes
  `0=Map←, 1=Write, 2=Read, 3=Refresh`; `DirtyWE` is gated so Dirty is not
  set on a store to a write-protected page ("Don't set Dirty on a store if
  the page is write-protected", WriteInMap' · MapWP → MC124). The emulator
  faults (`DM_FAULT_WRITE_PROTECT`) on a store through a WP entry
  (`memory.c:642`) before any Dirty update, so the invariant holds.
  Consistent.

### Known approximation — IFU pipeline (IFU sheet 01, page 3)
The real IFU is a multi-level byte pipeline (F/G/H/J levels feeding M/X)
with per-level valid bits (`FDv/GDv/HDv/JDv/MdV`) and **per-level fault
latches** (`FFault`, `GFault`), an `IncPcF` request engine
(`WantIfuRef`, `RefOutstanding`, `IfuMemAck`), and jump handling that
"discards any bytes in F, G, H and refills these pipe levels." The
emulator collapses all of this into: `ifu_active` + a fixed `ifu_warmup=5`
countdown (`cpu.c:1035`), direct `ifu_pcf` recompute on jumps
(`cpu.c:2633`), and a single `fetch_faulted` evaluated at IFUJump
dispatch (`cpu.c:2542`). This is functionally faithful to HM §6.2 ("the
pipeline fills up when it is five or six bytes ahead") and is enough for
opcode dispatch, but it is **not cycle-accurate**: the staggered F/G/H/J
fill/drain timing and the per-pipe-level buffering of IFU map faults
(`FFault`/`GFault` buffered until IFUJump) are not modeled. Not a
correctness bug for the current boot path, but the place to look if IFU
fault timing or precise prefetch-vs-branch interleaving ever matters.
**Low/medium — an approximation, not a discrepancy.**

## Coverage / what was reached

All five CPU boards now have at least one raster sheet read and compared:
- **Processor** — ProcL 14/17/18, ProcH 10 (deep; 2 findings).
- **Control** — ContA 03/23/24/25 (JCN decode, Ready logic, Wakeup
  Priority Encoder, Task Switch Generator) — consistent.
- **Memory** — MemX 14/15 (Map control + index width) plus code/manual for
  the BR/VA adder, cache geometry, and Vacant encoding — consistent.
- **IFU** — IFU 01 (Mem Req Logic) plus the canonical Table 18/19/§6.2 for
  the IFUM decode — consistent (pipeline-timing approximation noted).

The two **processor** findings at the top remain the only suspected
discrepancies after four passes.

Not raster-read (lower priority, boot chain already exercises them): the
**ContA/ContB control-store / LinkX sheets** (exact ECL polarity of the
JCN `fn` sub-decode on ContA03's MC161 — Return/RdTPC/LdTPC/RdIM/WrIM);
the **MemX/MemD BR-adder bit-slice** (carry width already matches at the
`va = br + disp` level); and the **MemC cache/Pipe** raster. These would
add confidence but no finding is presently suspected in them.

---

# Session-2 sweep

Continuation of the read-only audit (separate session; resolves the
"ContA03 fn-decode not raster-read" item left open above, and adds the
MemC cache/Pipe and MemX SRN/Fault sheets). Newly raster-read here:
**ContA 03** (re-read deeper: the MC161 fn-decoder + Link+CIAInc gating),
**ContB 06** (RAM Addressing), **ContB 14** (D1 Control Store data RAMs),
**MemC 04** (Victim + NextVictim), **MemC 14** (Carry logic / Pipe + BR
addressing), **MemX 03** (SRNs and Fault Info). Cross-checked against
`memory.c`, `cpu.c`, and HM §4.8 / §5.7 / §5.11 / Figure 10.

**One new suspected discrepancy (high confidence): the cache replacement
policy.** The emulator implements *true 4-way LRU*; the hardware (MemC04)
and HM §5.7 specify a *2-pointer Victim/NextVictim (VNV) pseudo-LRU* that
is explicitly "not quite LRU." Plus two minor/benign divergences (Link
smash after IM/TPC ops; StkError→fault-task wake already acknowledged
in-code).

## Finding 3 — Cache replacement is true-LRU; hardware uses the VNV pseudo-LRU

- **Board / sheet:** MemC sheet 04 (page 6), "Victim + NextVictim"
  (MemC04.sil, Lampson, 7/01/79). The VNV memory is a pair of 2-bit
  RAMs (`VicMem'`/`NVmem'`, MB071 at c22f, addressed by the cache row
  `Aad.0:7`). The on-sheet annotation states the update rule verbatim:
  - `New Vic ← old NV if miss or hit Vic (vic used) | Col if flush |
    unchanged otherwise`
  - `New NV ← something not = old Vic or old NV if miss or hit Vic
    (NV used) or hit NV | unchanged otherwise`
  with the note: *"The Victim memory is written on a CacheRef, PreFetch
  or IfuRef that misses, and on any FlushStore or Flush←."*
- **What the hardware does:** HM §5.7 (md manual lines 2980-2989) pins
  the algorithm down exactly: *"The VNV memory contains two two-bit
  entries for each row… When a miss or a hit in Victim occurs,
  Victim←NextV is done. When a miss, hit in Victim, or hit in NextV
  occurs, NextV←Victim.0'~NextV.1' is done (i.e., NextV is loaded with a
  value different from both the original NextV and Victim). This strategy
  is not quite [LRU]."* Only **two** of the four columns' recency is
  tracked, via two 2-bit pointers — not a full 4-deep recency order.
- **What the emulator does:** `cache_pick_victim()` (`memory.c:761`)
  returns `row->lru[DM_CACHE_WAYS-1]` (the 4th entry of a *true* LRU
  order), and `cache_touch_lru()` (`memory.c:661`) maintains a full
  4-way most-recently-used list (move-to-front). The in-code comment is
  the tell — `memory.c:659`: *"HM doesn't pin down the exact LRU
  encoding; what matters is that the next victim is whatever way hasn't
  been touched in longest."* That premise is **incorrect**: HM §5.7 +
  MemC04 do pin it down, and VNV differs from true LRU.
- **Possible error / discrepancy:**
  1. **Victim selection diverges.** On a miss the column displaced can
     differ from hardware. For ordinary fetch/store this is *functionally*
     safe (any line may be evicted), so it is not a data-correctness bug.
  2. **Pipe5 readback diverges (observable).** `cache_line_pipe5_flags()`
     (`memory.c:367-369`) reports `NextVictim = lru[3]` when not in
     forced-victim (`usemcrv`) mode. Microcode/diagnostics reading
     `B←Pipe5` Victim/NextVictim (HM Figure 10 fields, used by Midas and
     cache self-test) see emulator-LRU values, not hardware VNV contents.
  3. **Bug/cycle parity.** Microcode depending on the deterministic VNV
     order (cache sweep/flush tests; `Flush←` clobbering Victim/NextV per
     HM line 2107 and MemC04's "On a dirty hit, a FlushStore follows,
     smashing Victim and NextV again") will diverge from real hardware.
- **Confidence:** **High** that the algorithm differs (HM §5.7 explicit;
  MemC04 implements exactly it). **Medium** on functional impact: benign
  for data correctness, but breaks Pipe5 Victim/NextVictim parity and any
  victim-order-dependent diagnostic. Faithful fix: replace the `lru[4]`
  order with a per-row `{victim:2, nextv:2}` pair updated by `Victim←NextV`
  / `NextV←(~Victim&2)|(~NextV&1)` on the miss/hit-Vic/hit-NV events; have
  `cache_pick_victim` return `victim` and Pipe5 read `victim`/`nextv`
  directly. To gauge impact, scan emulator microcode for `B←Pipe5`
  Victim/NextV reads and `Flush←` sequences.

## Finding 4 — Link not smashed with CIA+1 after Write IM / Read IM / LdTPC; RdTPC leaves data (not CIA+1) in Link

- **Board / sheet:** ContA sheet 03 (page 5), "JCN Decoding". The
  `Link+CIAInc` net (bottom-right, MC195 buffers k18b/k18h) is driven by
  `Return'a!0` **OR** `IFUNext'a!0` only. The MC161 (d20) decoder's
  `WIM'`/`RIM'`/`WTPC'`/`RTPC'` outputs feed `RIMorRTPCdly`/`RWTPC'`/
  `RWTPCorWIM` (the IM/TPC access controls), not the Link+CIAInc gate.
- **What the hardware does:** HM §4.8 (md lines 1766-1774) is explicit
  that for *every* IM or TPC read **or write**, *"control passes to… CIA+1…
  CIA+1 also winds up in Link,"* and the Note: *"The Link register itself
  is smashed with CIA+1."* For reads, the data is delivered via an
  alternate `B←Link` path, **not** by parking data in Link.
- **What the emulator does:** in `next_pc`'s return-class block —
  - **Write IM** (fn=7, `cpu.c:2763`): `*next = real_PC+1`, never touches
    `cpu->Link`. Hardware would set Link=CIA+1.
  - **LdTPC** (fn=5, `cpu.c:2804`) and **Read IM** (fn=6, `cpu.c:2812`):
    same — next=PC+1, Link untouched.
  - **RdTPC** (fn=4, `cpu.c:2782`): sets `cpu->Link = ~tpc`, parking the
    *data* in Link. Hardware parks CIA+1 in Link and delivers ~tpc on the
    alternate B path. The documented `B←Link` next cycle yields ~tpc
    either way, but a *second* `B←Link` (without another RdTPC) reads ~tpc
    in the emulator vs CIA+1 on hardware.
- **Possible error / discrepancy:** microcode that reads `Link` after a
  Write IM / Read IM / TPC op **without reloading it** diverges. The
  Boot0/Bootstrap loaders reload Link (the IM write address) before each
  `Write IM`, so this is benign for the boot chain; flagged because it
  contradicts the manual's stated semantics.
- **Confidence:** **Low-medium.** Clear vs the manual, no current path
  known to depend on it. Fix is one line per fn branch
  (`cpu->Link = real_PC+1`), with RdTPC keeping `~tpc` in an internal
  "B←Link alternate" latch rather than in Link itself.

## Finding 5 (minor, already acknowledged) — StkError does not wake the fault task

- **Board / sheet:** MemX sheet 03 (page 5), "SRNs and Fault Info". Bottom
  right: `StkError → StkWake` (clk0/g23c) OR'd with the `Faults` term into
  `TWReq15` (h3a/h3b, MC104) → wakeup of task 15 (the fault task).
- **What the emulator does:** memory faults do wake task 15
  (`cpu.c:3412-3413`, `wakeup_pending |= 1<<15`, gated by `!nowake` —
  consistent with `Faults`→TWReq15). But **StkError** only sets the sticky
  StkUnd/StkOvf flags; `stk_apply_post` (`cpu.c:442`) states *"we don't
  model the HOLD + fault-task wake — we just set the flags."* So a stack
  under/overflow does not raise the TWReq15 wakeup MemX03 shows.
- **Confidence:** High that it's missing, but **already documented** as a
  known gap in-code; recorded here only to tie it to the schematic.

## Circuits checked and found consistent (Session-2)

- **JCN fn-decode — ContA sheet 03 (page 5), MC161 (d20).** Select inputs
  are the *complemented* JCN bits: `S4=JCN.2'a` (pin14), `S2=JCN.3'a`
  (pin9), `S1=JCN.4'` (pin7); the decoder is enabled by the return-class
  detect (`Return'a!2`, JCN[1]=1 & JCN[5:7]=111). So `Q0=WIM'` ⇒
  JCN[2:4]=111 → **fn=7 Write IM**, `Q1=RIM'`→fn=6, `Q2=WTPC'`→fn=5,
  `Q3=RTPC'`→fn=4, and Return is the enable case → fn=0. Matches
  `next_pc`'s dispatch exactly (`cpu.c:2687-2816`: fn=0 Return, 4 RdTPC,
  5 LdTPC, 6 ReadIM, 7 WriteIM; fn=1/2/3 → `CPU_HALT_UNSUPPORTED_JCN`).
  Confirms `docs/jcn-encoding.md` against silicon.
- **Link load on Return/IFUJump — ContA sheet 03.** `Link+CIAInc` driven
  by `Return'a!0 OR IFUNext'a!0` matches the emulator loading `Link=CIA+1`
  on Subroutine Return (`cpu.c:2698`) and every IFUJump (`cpu.c:2681`).
- **Per-task Link & TPC — HM §4.1/§4.8.** HM lines 1394-1397 (*"Each task
  has its own program counter and subroutine return link, stored in the
  (task specific) TPC and TLINK registers"*) and line 441 (*"TPC… addressed
  from B[12:15]"*) match the per-task arrays `task_tpc[]`/`task_link[]`
  saved in `task_save` (`cpu.c:182-183`), restored in `task_load`
  (`cpu.c:200-201`), and RdTPC/LdTPC addressing by `B[12:15]`
  (`cpu.c:2778,2795`).
- **RAM-address mux — ContB sheet 06 (page 8).** Each control-store
  address bit `dRA.NN'` is a 2:1 mux (MC1662) between `TNIA.NN` and
  `BNPC.NN`, selected by `SWa/SW'a`; `BNPC` is the buffered/repeat PC used
  on Hold/RepeatCurrent. Architecturally consistent with the emulator's
  recompute-TNIA-each-step + Hold approximation. (TNIA/BNPC bits 02/03 are
  wired to "RESERVED FOR 16K EXPANSION" pads — matches TNIA[0:3] reserved.)
- **BR / VA adder — MemC sheet 14 (page 16).** The displacement adder is a
  chain of F181K 4-bit ALU slices spanning bits 4-31 with full carry
  propagation (`CO8'`,`CO12'`,`CO16'`,`CO20'`,`CO24'` via MC179/MC117
  lookahead). Control: `D=RBMux/Mar'`, `E=BR`, `Add ⇒ D+E`. This is the
  28-bit `VA = BR + D` the emulator computes as
  `va = (br + disp) & 0x0FFFFFFF` (`cpu.c:3237`), confirming the earlier
  cpu-level-only check. (Unread nuance: the sheet shows a `−D+E` function
  code on bits 16-31; no emulator path uses a subtractive displacement and
  no HM reference was found — left unverified.)
- **SRN / Fault-info latching — MemX sheet 03 (page 5).** `FaultSrn.0:3` is
  loaded from the Ec2-stage SRN by `LoadFltSrn` gated on `FaultSrn=0'` (the
  first fault latches the SRN, later faults don't overwrite). Matches
  `memory.c:1263-1264` (`if (fault_count==0) fault_first_srn = srn`).
  `Srn0Fault` (MC135) → EmulatorFault matches `if (task==0)
  mem->fault_emulator=1` (`memory.c:1266`). `Faults`→`TWReq15` (wakes task
  15) matches the fault-wake at `cpu.c:3412`. The saturating `FaultCnt.0:3`
  matches `if (fault_count<0xF) fault_count++` (`memory.c:1267`).
- **ASRN I/O ring — HM §5.11 (page 52).** The `UseAsrn`/`ProcSrn` mux on
  MemX03 and the ring-advance match `memory.c:919-930` (task 0/15 use
  ProcSRN; I/O tasks + IOFetch/IOStore + PreFetch-miss use ASRN) and the
  2..15 wrap at `memory.c:1279-1281`.

## Coverage / what was not reached (Session-2)

Newly raster-read: **ContA 03** (deeper), **ContB 06**, **ContB 14**,
**MemC 04**, **MemC 14**, **MemX 03**. Combined with the prior four passes,
the Control JCN/fn path, the per-task Link/TPC model, the BR/VA adder, the
cache geometry, the Map control/index width, and the SRN/fault-info
latching are now schematic-checked.

Still **not** raster-read (highest-value first):
- **MemC 15-19** — Miss/Hold, Ref decoding, Pair, Next, FF decoding (the
  cache hit/miss + Hold state machine the emulator approximates).
- **MemX 05-10** — Store-Transport / Map / Memory / Ec1 / Ec2 automatons
  (the staged memory pipeline + SRN hand-off MemX03 feeds; the emulator
  collapses these into an immediate `pipe_push`).
- **MemX 12/13/16/17** — Map Real Page slices, Map Trouble, Map chips (the
  MapTrouble/MapPE encoding at silicon level).
- **MemD entire board** — Hamming generator/checker, Error corrector +
  Pipe4 (ECC syndrome/EcFault — emulator gap C2, Pipe4 unmodeled).
- **IFU raster bit-slices** — Instruction/Byte/PC slices (05-09), Pc Pipe
  Control (10), IF Data Selector PROM (23): the F/G/H/J fill timing the
  emulator approximates with the fixed ~5-cycle warmup.
- **DskEth, DispM, DispY** — disk/ethernet/display controllers vs
  `disk.c`/`ethernet.c`/`display.c` (lower priority).
