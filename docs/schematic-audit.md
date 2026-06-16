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

## Coverage / what was not reached

Thorough on the **Processor** (ProcL/ProcH data path); architectural raster
pass over **Control** (ContA JCN-decode, Ready logic, Wakeup Priority
Encoder, Task Switch Generator); **Memory** addressing (BR/VA adder, Map/
cache geometry, Vacant encoding) and **IFU** IFUM decode against code +
canon manual (Tables 18/19, §6.2). All three of these passes found the
circuits consistent; the two processor findings at the top remain the only
suspected discrepancies.

Not raster-read: the **ContA/ContB control-store and LinkX/conditional-
branch sheets** (exact ECL polarity of the JCN sub-decode — Return/RdTPC/
LdTPC/RdIM/WrIM fn-values on ContA03's MC161 decoder — would nail down
`next_pc`'s `fn` mapping, though the boot chain already exercises Write IM
and the local/global/long/conditional paths); the **IFU raster** proper
(Pc Pipe Control sheet 10, Instruction Bit Slice sheet 06 — to confirm the
operand-pipeline F/G/H/J fill timing the emulator approximates with a fixed
5-cycle warmup); and the **MemX/MemD** BR-adder and **MemC** cache/Pipe
raster sheets. Highest-value remaining raster targets, in order: ContA03
JCN fn-decode, then MemX VA adder, then the IFU pipe-fill timing.
