# JCN field — encoding reference

Distilled from Hardware Manual §4.3 + Figure 6 (the "Next Address
Formation" diagram, page 161 of the September 1981 manual) and
cross-checked against `chm/microd/mdfields.d`. This is the only place
we keep the canonical bit positions; both the disassembler and the
microengine consult the rules below.

## Notation

The JCN field is **8 bits** total: 7 bits packed into `iw1[6:0]` plus
1 extra bit (`JN1bit7` in the BCPL source) packed into `iw2[14]`.

Two bit-numbering conventions are in play:

| Convention            | Bit 0 = | Bit 7 = |
|-----------------------|---------|---------|
| Hardware Manual / BCPL `mdfields.d` | MSB    | LSB     |
| Our C source (typical) | LSB    | MSB     |

This document uses the **manual's** convention (BCPL — bit 0 = MSB).
The C extraction expressions invert as needed (`(jcn >> 7) & 1` in C
== JCN[0] in the manual).

## Encoding table

The JCN field is split into seven mutually-exclusive encodings,
discriminated by the high bits of JCN:

| JCN[0:7]                       | Encoding              | Selected by             |
|--------------------------------|-----------------------|-------------------------|
| `1 0 a a a a a a`              | **Local Jump/Call**   | JCN[0:1] = `10`         |
| `1 1 a a a a a a`              | **Global Call**       | JCN[0:1] = `11`         |
| `0 0 0 0 a a a a`              | **Long Jump/Call**    | JCN[0:3] = `0000`       |
| `0 a a b b c c c` (a..b ≠ 0, c ≠ 7) | **Conditional Jump/Call** | JCN[0]=0 and not Long/Return |
| `0 1 f f f 1 1 1`              | **Return / IM / TPC** | JCN[1]=1 and JCN[5:7] = `111` |
| `0 0 1 n n 1 1 1`              | **IFU Jump**          | JCN[0:2]=`001` and JCN[5:7]=`111` |
| `0 0 0 1 x 1 1 1`              | undefined             | (unused encoding)       |

Note: encodings whose JCN[5:7] is `111` overlap with conditional —
condition `7` is therefore reserved.

## Next address formulas (TNIA)

`CIA` is the current 16-bit instruction address (the address of *this*
microinstruction). `TNIA` is the 16-bit "Tentative Next Instruction
Address". Bits 4:15 of TNIA are the 12-bit IM address actually used
for a 4 K-word microstore; bits 0:3 are reserved for a future 16 K
expansion.

### Local Jump/Call
```
TNIA = CIA[2:9]  ‖  JCN[2:7]
       (8 bits)     (6 bits)
```
Branch within the current 64-word page. Page boundary = `CIA & ~63`.

### Global Call
```
TNIA = CIA[2:3]  ‖  JCN[2:7]  ‖  000000
       (2 bits)     (6 bits)     (6 bits = 0)
```
Branch to location 0 of any of 64 pages within the current 4 K-word
quadrant. The 6 trailing zeros force the target onto a 64-word
boundary.

### Long Jump/Call
```
TNIA = CIA[2:3]  ‖  FF[0:7]  ‖  JCN[4:7]
       (2 bits)     (8 bits)     (4 bits)
```
12-bit absolute address within the current 4 K-word quadrant. **FF's
normal interpretation is disabled** — it provides 8 of the 12 address
bits.

### Conditional Jump/Call
```
TNIA = CIA[2:9]   ‖  JCN[1:2]  ‖  0   ‖  JCN[3:4]  ‖  R
       (8 bits)      (2 bits)     (1)    (2 bits)     (1)
```
Branch within the current page. Forces TNIA[12] = 0, so the page-
relative offset has the form `aa0bbR`. Out of 64 page slots only 16
are reachable as false targets; with the LSB = R, true targets are
the adjacent odd locations.

`R` is the **branch condition result** (1 = condition true → take
the true / odd target; 0 = false → even target).

### Return / IM / TPC

A "return-class" JCN (JCN[1]=1, JCN[5:7]=111) selects between several
operations via JCN[2:4]:

| JCN[2:4] | Operation                 |
|----------|---------------------------|
| 0        | Subroutine Return → TNIA = Link[2:15] |
| 1, 2, 3  | unused                    |
| 4        | Read TPC                  |
| 5        | Write TPC                 |
| 6        | Read IM                   |
| 7        | Write IM                  |

For Subroutine Return, the next address is the contents of the Link
register (which was loaded with `CIA + 1` by the most recent Call /
IFUJump / Return).

For TPC and IM access, the addressing flow is unusual — see HM §4.8
and Figure 7 (Instruction Timing). These take 6 cycles and use the
RSTK field as a sub-decode.

### IFU Jump
```
TNIA = CIA[2:3]  ‖  InstrAddr[4:13]  ‖  JCN[3:4]
       (2 bits)     (10 bits, from IFU)   (2 bits)
```
Used at the last cycle of an opcode to pop the IFU's prefetched entry
vector and jump to the start of the next opcode's microcode.
`InstrAddr` is supplied by the IFU section, not by the microinstruction.

JCN[3:4] selects one of 4 entry-vector slots (`IFUJump[0..3]`).

## Branch conditions (Table 13)

Selected by JCN[5:7] (= condition 0..6) **or** by FF = `60..67`. When
both encode conditions, the test results are **OR'd**.

| JCN[5:7] | FF   | Condition          | Comment                                   |
|----------|------|--------------------|-------------------------------------------|
| 0        | 0o60 | `ALU = 0`          | result of last ALU op for this task       |
| 1        | 0o61 | `ALU < 0`          | sign bit of last ALU result               |
| 2        | 0o62 | `Carry'`           | complement of saved carry-out             |
| 3        | 0o63 | `Cnt = 0 & -1`     | true iff Cnt was 0; Cnt decrements **after** testing |
| 4        | 0o64 | `R < 0`            | sign bit of selected RM/STK register      |
| 5        | 0o65 | `R odd`            | LSB of selected RM/STK register           |
| 6        | 0o66 | `IOAtten' / Reschedule` | for non-emulator / emulator tasks    |
| (n/a)    | 0o67 | `Overflow`         | last *arithmetic* op overflowed (FF only) |

All but `Cnt = 0&-1` are loaded into a RAM at t₃ for use by the next
instruction; `FreezeBC` (a separate FF function) can hold the previous
cycle's flags for one extra cycle.

## Call vs Jump

> "A long, local, or conditional branch is a *call* iff, before any
> modification of TNIA by branch conditions or dispatches,
> TNIA[12:15] is 0; otherwise, it is a jump."  — HM Figure 6

That is: if the unmodified target is `0 mod 16` (4-word boundary),
treat the branch as a Call (load Link with CIA+1); otherwise it's a
Jump (Link unchanged).

For conditional branches, the unmodified target's TNIA[12:15] is
`0 ‖ JCN[3:4] ‖ 0` (R is excluded), so a conditional-call requires
JCN[3:4] = `00`.

## Reserved IM locations (Table 14)

These addresses are reserved targets for hardware-injected control
flow. Avoid placing user microcode here when possible.

| Range  | Reason                              |
|--------|-------------------------------------|
| 0o0–3  | IFU map fault                       |
| 0o4–7  | IFU data parity error               |
| 0o14–17 | Reschedule request                 |
| 0o34–37 | IFU not ready                      |
| 0o74–77 | IFUM parity error                  |
| 0o7776  | Midas Call                         |
| 0o7777  | Midas Crash detect                 |

All trap addresses except Midas have the IFU's instruction-set number
OR'd into bits 8:9, so the actual reserved ranges are 0o14-17,
0o114-117, 0o214-217, 0o314-317, etc. (one per instruction set).

## C extraction expressions

Given a `uint8_t jcn` in our C representation (bit 7 = MSB):

```c
int top1   = (jcn >> 7) & 1;            /* JCN[0]  = JC1fast tag */
int top2   = (jcn >> 6) & 3;            /* JCN[0:1] = local/global tag */
int top4   = (jcn >> 4) & 0xF;          /* JCN[0:3] = long tag */
int low3   = jcn & 7;                   /* JCN[5:7] = condition or "111" tag */
int retfn  = (jcn >> 3) & 7;            /* JCN[2:4] = return/TPC/IM function */
int ifu_n  = (jcn >> 3) & 3;            /* JCN[3:4] = IFU jump entry-vector index */

/* Local: 6-bit page offset = JCN[2:7] = jcn & 0x3F */
/* Global: 6-bit page selector = JCN[2:7] = jcn & 0x3F */
/* Long: 4-bit low addr = JCN[4:7] = jcn & 0xF (FF supplies bits 4:11) */

/* Conditional address bits (within current page):
 *   page_offset = (JCN[1:2] << 4) | (JCN[3:4] << 1) | R
 *               = (((jcn >> 5) & 3) << 4) | (((jcn >> 3) & 3) << 1) | R   */
```

## Decision tree (decoder pseudocode)

```c
if (top1 == 1) {
    return (top2 == 2) ? LOCAL
         : (top2 == 3) ? GLOBAL_CALL
         : INVALID;
}
/* top1 == 0 */
if (top4 == 0)              return LONG_JUMP_OR_CALL;
if (low3 == 7) {
    if ((jcn & 0xC0) == 0x40)               return RETURN_CLASS;  /* JCN[1]=1 */
    if ((jcn & 0xE0) == 0x20)               return IFU_JUMP;       /* JCN[0:2]=001 */
    return UNDEFINED;       /* 0001x111 */
}
return CONDITIONAL_JUMP_OR_CALL;
```
