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

## Coverage / what was not reached

Thorough on the **Processor** (ProcL/ProcH data path). For **Control**
I read the ALU/branch sheet (ProcH 10) but did not raster-read the ContA/
ContB JCN-decode or tasking priority-encoder sheets — the tasking model in
`task_schedule()` already carries documented timing approximations
(`cpu.c:1231-1248`), which are the place to look next. **IFU** and
**Memory** (MemC/MemD/MemX BR adder, Map, Pipe) were cross-checked only
against the manual + code, not the raster drawings; the IFUM length/MemB/
RBaseB decode and the BR adder carry behavior are the highest-value
remaining raster targets.
