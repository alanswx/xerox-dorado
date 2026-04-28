#include "cpu.h"
#include "baseboard.h"

#include <stdio.h>
#include <string.h>

/*
 * Dorado microengine. See include/cpu.h for scope.
 *
 * Per-cycle structure (HM §3, §4 + Figure 1):
 *
 *   1. Fetch uinstr from IM[real_PC]. Halt if absent.
 *   2. Compute A bus per ASEL (HM Table 8a/8b).
 *   3. Compute B bus per BSEL + FF (HM Table 7 + Tables 11a-e).
 *   4. ALU = ALUFM[ALUF] applied to (A, B) (HM Table 9).
 *   5. Pd = ALU output (or shifter output for ASEL=7 — TBD).
 *   6. LC writes Pd to T and/or RM/STK (HM Table 10).
 *   7. Apply FF function (memory ops, register transfers, …) (Tables 11a-e).
 *   8. Compute next real_PC from JCN (HM §4.3, Table 13, mdfields.d tags).
 *
 * Most of the dispatch logic is currently stubbed. Each unhandled
 * combination is a halt with a diagnostic message rather than a silent
 * fall-through; that keeps coverage gaps obvious and lets us prioritize
 * what real microcode actually exercises.
 */

const char *cpu_halt_reason_str(cpu_halt_reason r)
{
    switch (r) {
    case CPU_HALT_NONE:               return "(running)";
    case CPU_HALT_OK:                 return "halt: ok";
    case CPU_HALT_NO_CODE:            return "halt: no code at PC";
    case CPU_HALT_UNSUPPORTED_FF:     return "halt: unsupported FF";
    case CPU_HALT_UNSUPPORTED_JCN:    return "halt: unsupported JCN";
    case CPU_HALT_UNSUPPORTED_ASEL:   return "halt: unsupported ASEL";
    case CPU_HALT_UNSUPPORTED_BSEL:   return "halt: unsupported BSEL";
    case CPU_HALT_BREAKPOINT:         return "halt: breakpoint";
    case CPU_HALT_BAD_RM:             return "halt: RM out of range";
    case CPU_HALT_USER:               return "halt: user";
    }
    return "halt: unknown";
}

void dorado_cpu_init(dorado_cpu *cpu, const dorado_microcode *mc,
                     uint16_t start_real)
{
    memset(cpu, 0, sizeof *cpu);
    cpu->mc      = mc;
    cpu->real_PC = start_real;
    /* Preload RM with initial values from the .MB file. */
    if (mc) {
        for (int i = 0; i < 256; i++) {
            if (mc->rm_present[i]) cpu->RM[i] = mc->rm[i];
        }
    }
}

void dorado_cpu_trace(dorado_cpu *cpu, void *fp)
{
    cpu->trace = fp != NULL;
    cpu->trace_fp = fp;
}

/*
 * RM/STK addressing (HM §3.1). For an emulator instruction:
 *   if BLOCK == 1 → STK at StkP (with -4..+3 modification by RSTK)
 *   else          → RM at (RBase[0:3] || RSTK[0:3])
 *
 * This minimal model treats RSTK as a 4-bit literal extension only,
 * matching the BLOCK==0 case (RM-only). We'll add stack semantics when
 * we hit microcode that uses BLOCK=1.
 */
/*
 * RM/STK addressing (HM §3.1 + Table 6, page 11).
 *
 *   BLOCK = 0 (or non-emulator task):
 *     RM read address  = RBase[0:3] || RSTK[0:3]   (8 bits → 256 entries)
 *     RM write address = same as read, unless an FF function modifies
 *     it (RBase←SC, Pointers←B, etc. are tracked elsewhere).
 *
 *   BLOCK = 1 (emulator task only — HM page 10):
 *     Read address  = unadjusted StkP (current StkP[0:7]).
 *     Write address = unadjusted StkP, unless ModStkPBeforeW (FA=0
 *                     FB=2 FC=7) is in effect — then ADJUSTED StkP.
 *
 *   Per Table 6, RSTK[1:3] is a signed 3-bit value that updates StkP
 *   AFTER the access:
 *     0    no change      4   StkP -= 4
 *     1    StkP += 1      5   StkP -= 3
 *     2    StkP += 2      6   StkP -= 2
 *     3    StkP += 3      7   StkP -= 1
 *
 *   RSTK[0]: 0 = no underflow check; 1 = underflow if StkP originally
 *   0 OR finally 0. Underflow / overflow set StkUnd / StkOvf flags
 *   (read via Pd←Pointers) and would HOLD + wake fault task 15 on
 *   real hardware — we just track the flags and document the sticky
 *   bit semantics.
 *
 *   StkP[0:1] selects one of 4 stack regions (each 100₈ words).
 *   StkP[2:7] is the per-region offset, valid range 1..77₈.
 *   StkP[2:7] = 0 denotes empty stack.
 *
 * Returns a positive RM index, or 0x100..0x1FF for STK indices (so
 * callers can use one shared index space). >= 0x200 is "invalid".
 */
#define CPU_RMSTK_RM_BASE   0x000
#define CPU_RMSTK_STK_BASE  0x100
#define CPU_RMSTK_INVALID   0x200

static int rm_address(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    /* Read address — for STK this is always unadjusted StkP. The
     * write-side ModStkPBeforeW case is handled in apply_lc via
     * stk_write_address(). */
    if (u->block) return CPU_RMSTK_STK_BASE | (cpu->StkP & 0xFF);
    return ((cpu->RBase & 0xF) << 4) | (u->rstk & 0xF);
}

/* Decode RSTK[1:3] as a signed delta in [-4, +3]. RSTK[1:3] are the
 * low 3 bits of u->rstk in C convention (since u->rstk packs all 4
 * bits with bit 3 = manual MSB = RSTK[0], bit 0 = manual LSB =
 * RSTK[3]). The "1:3" range in manual = "bits at position 1, 2, 3"
 * from MSB = bits 2, 1, 0 in C-LSB = u->rstk & 0x7. The sign-bit is
 * the MSB of those 3 bits (bit 2 of u->rstk = manual RSTK[1]).
 *
 * Encoding (Table 6 row "RSTK[1:3] Meaning"):
 *   0 (000) →  0     4 (100) → -4
 *   1 (001) → +1     5 (101) → -3
 *   2 (010) → +2     6 (110) → -2
 *   3 (011) → +3     7 (111) → -1
 *
 * So the value is a 3-bit two's-complement number. */
static int stk_signed_delta(const dorado_uinstr *u)
{
    int v = u->rstk & 0x7;
    return (v & 0x4) ? (v - 8) : v;
}

static int stk_underflow_check(const dorado_uinstr *u)
{
    return (u->rstk >> 3) & 1;        /* RSTK[0] manual = u->rstk bit 3 */
}

/* Compute the WRITE address for an RM/STK write. Same as rm_address
 * for everything except STK with the ModStkPBeforeW FF function in
 * play, which routes the write to the *adjusted* (post-delta) StkP. */
static int stk_write_address(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    if (!u->block) return rm_address(cpu, u);

    /* ModStkPBeforeW FF = FA=0 FB=2 FC=7 = 0o27 = 0x17 */
    int fa = (u->ff >> 6) & 3;
    int fb = (u->ff >> 3) & 7;
    int fc = u->ff & 7;
    int mod_before_w = (fa == 0 && fb == 2 && fc == 7);
    if (!mod_before_w) return rm_address(cpu, u);

    int adjusted = (cpu->StkP + stk_signed_delta(u)) & 0xFF;
    return CPU_RMSTK_STK_BASE | adjusted;
}

/* Apply the post-instruction StkP update + underflow/overflow check.
 * Called once per instruction at the very end of execute_uinstr,
 * AFTER reads and writes have used the unadjusted (or modified)
 * indices. HM page 11:
 *
 *   StkError = (BLOCK==1) AND emulator AND
 *              [(StkP[2:7] + RSTK[1:3] < 0) OR
 *               (StkP[2:7] + RSTK[1:3] > 77₈) OR
 *               (RSTK[0]==1 AND ((StkP[2:7] + RSTK[1:3]) == 0))]
 *
 * StkUnd / StkOvf get set on the appropriate side. They're *sticky*
 * until the next stack operation by the emulator (HM page 11
 * "These get cleared (i.e., recomputed) when the next stack
 * operation is executed by the emulator").
 *
 * For now we don't model the HOLD + fault-task wake — we just set
 * the flags so Pd←Pointers can read them. */
static void stk_apply_post(dorado_cpu *cpu, const dorado_uinstr *u)
{
    if (!u->block) return;
    int delta = stk_signed_delta(u);
    if (delta == 0 && !stk_underflow_check(u)) return;

    /* Compute the new in-region offset (StkP[2:7], 6 bits) and check
     * for over/underflow. The region selector StkP[0:1] is preserved
     * across normal pushes/pops — overflow signals at the boundary. */
    int region = cpu->StkP & 0xC0;     /* StkP[0:1] in bits 7,6 */
    int offset = cpu->StkP & 0x3F;     /* StkP[2:7] in bits 5..0 */
    int new_off = offset + delta;
    int underflow = 0, overflow = 0;
    if (new_off < 0) underflow = 1;
    if (new_off > 077) overflow = 1;
    if (stk_underflow_check(u) && (new_off & 0x3F) == 0 && new_off >= 0)
        underflow = 1;

    /* StkUnd / StkOvf are sticky-recomputed: if this op didn't trip
     * either, clear them. */
    cpu->stk_und = (uint8_t)underflow;
    cpu->stk_ovf = (uint8_t)overflow;

    /* Always update StkP, even on overflow/underflow (HM doesn't say
     * to suppress; the fault task is what makes it visible). */
    cpu->StkP = (uint8_t)(region | (new_off & 0x3F));
}

static uint16_t rm_stk_read(const dorado_cpu *cpu, int idx)
{
    if (idx >= CPU_RMSTK_INVALID) return 0;
    if (idx >= CPU_RMSTK_STK_BASE) return cpu->STK[idx & 0xFF];
    return cpu->RM[idx & 0xFF];
}

static void rm_stk_write(dorado_cpu *cpu, int idx, uint16_t value)
{
    if (idx >= CPU_RMSTK_INVALID) return;
    if (idx >= CPU_RMSTK_STK_BASE) cpu->STK[idx & 0xFF] = value;
    else                            cpu->RM[idx & 0xFF]  = value;
}

/*
 * Decode FF into FA / FB / FC (HM §3.9 — "the 8-bit FF field is shown
 * below as a two-bit field FA (= FF[0:1]) and two 3-bit fields FB (=
 * FF[2:4]) and FC (= FF[5:7])"). BCPL bit numbering: FF[0] = MSB.
 */
static inline int ff_fa(uint8_t ff) { return (ff >> 6) & 3; }
static inline int ff_fb(uint8_t ff) { return (ff >> 3) & 7; }
static inline int ff_fc(uint8_t ff) { return ff & 7; }

/*
 * Many FF values specify an *external* B source (Pipe, Link, CPReg,
 * etc.) that overrides the BSEL primary source. Returns 1 if FF
 * supplied an override and `*b` was set; 0 if BSEL should be used as
 * normal.
 *
 * Coverage tracks what real microcode actually exercises. FF=0o077
 * (FA=0 FB=7 FC=7) is "Reserved as a no-op" so we ignore it without
 * halting. Unknown FF values that *would* override B fall through to
 * the BSEL path (i.e., we silently miss the override) — TODO: tighten.
 */
static int ff_override_b(dorado_cpu *cpu, const dorado_uinstr *u,
                         uint16_t *b)
{
    int fa = ff_fa(u->ff), fb = ff_fb(u->ff), fc = ff_fc(u->ff);

    /* Most external-B sources live at FA=1, FB=6 or 7 (HM Table 11b/c). */
    if (fa != 1) return 0;

    if (fb == 6) {
        /* B ← Pipe / FaultInfo / Config — memory subsystem state.
         * No memory model yet; return 0 as a stub so microcode that
         * peeks the pipe doesn't stall the engine. */
        *b = 0;
        return 1;
    }
    if (fb == 7) {
        switch (fc) {
        case 0: *b = 0;          break;  /* B ← PCX'           — IFU PC */
        case 1: *b = 0;          break;  /* B ← EventCntA'     — stub */
        case 2: *b = 0;          break;  /* B ← IFUMRH'        — stub */
        case 3: *b = 0;          break;  /* B ← IFUMLH'        — stub */
        case 4: *b = 0;          break;  /* B ← EventCntB'     — stub */
        case 5: *b = 0;          break;  /* B ← DBuf           — stub */
        case 6:                             /* B ← RWCPReg */
            /* HM page 31: "B←RWCPReg = Link←B, B←CPReg'." */
            if (cpu->baseboard) {
                *b = (uint16_t)~baseboard_dorado_read_cpreg(cpu->baseboard);
            } else {
                /* Counter stub for tests that don't wire a BaseBoard. */
                *b = cpu->cpreg;
                cpu->cpreg = (uint16_t)(cpu->cpreg + 1);
            }
            cpu->Link = *b;
            break;
        case 7: *b = cpu->Link;  break;  /* B ← Link */
        default: return 0;
        }
        /* HM Table 7 asterisk: "BSEL decode for Q←B is needed in
         * initializing Dorado from the baseboard or Alto." So when
         * an external B source is in play AND BSEL=3, the external
         * value lands in Q (not just on the bus). */
        if (u->bsel == 3) cpu->Q = *b;
        return 1;
    }
    return 0;
}

/*
 * Side effects of FF that happen after ALU computation (register loads,
 * Pd overrides, tasking flags, etc.). Returns the (possibly modified)
 * Pd value. Sets *halt to a CPU_HALT_* code if FF requests something
 * we don't yet handle.
 *
 * `b` is the B-bus value, `alu` is the raw ALU output (= initial Pd).
 */
static uint16_t ff_apply_post(dorado_cpu *cpu, const dorado_uinstr *u,
                              uint16_t b, uint16_t alu, int *halt)
{
    int fa = ff_fa(u->ff), fb = ff_fb(u->ff), fc = ff_fc(u->ff);
    uint16_t pd = alu;
    *halt = 0;

    /* When BSEL produces a constant (BSEL >= 4), or when FF[0:1] picks
     * an alternate A source for memory references (ASEL=0..3 with
     * specific FF[0:1]), the FF field is *not* interpreted as a
     * function. Per HM §3.9: "FF interpreted as a function iff (BSEL
     * not selecting a constant) and (JCN does not select a 'long'
     * goto/call)." */
    int ff_is_function = (u->bsel < 4);
    /* Long branch consumes FF for address bits (JCN tag = 0000). */
    uint8_t jcn_top4 = (u->jcn >> 4) & 0xF;
    if (((u->jcn >> 7) & 1) == 0 && jcn_top4 == 0) ff_is_function = 0;
    if (!ff_is_function) return pd;

    if (fa == 0) {
        if (fb <= 1) {
            /* A[12:15] ← FF[4:7] — already handled at A-bus time, but
             * harmless here. (We don't actually wire that override
             * yet; tracked as a known gap.) */
            return pd;
        }
        if (fb == 2) {
            /* A-source overrides for memory ops. Mostly we skip these
             * since memory isn't modeled yet. */
            switch (fc) {
            case 4: /* XorCarry — modifies ALUFM carry bit */
            case 5: /* XorSavedCarry */
            case 6: /* Carry20 — force carry into bit 12 */
            case 7: /* ModStkPBeforeW */
                return pd;       /* stub: silently honor */
            }
            return pd;
        }
        if (fb == 3) {
            switch (fc) {
            case 0: /* — */              return pd;
            case 1: /* ReadMap */         return pd;  /* stub */
            case 2: /* Pd ← Input */      return pd;  /* stub */
            case 3: /* Pd ← InputNoPE */  return pd;  /* stub */
            case 4: /* RIsId */           return pd;  /* needs IFU */
            case 5: /* TIsId */           return pd;  /* needs IFU */
            case 6: /* Output ← B */      return pd;  /* I/O, stub */
            case 7: /* FlipMemBase */
                cpu->MemBase ^= 1;
                return pd;
            }
        }
        if (fb >= 4 && fb <= 5) {
            /* Replace RMaddr[0:3] with RBase[0:3], RMaddr[4:7] with
             * FF[4:7], force RM write even if STK was read. Affects
             * destination of the LC store; for now we silently honor. */
            return pd;
        }
        if (fb == 6) {
            /* Branch conditions (already handled in next_pc). */
            return pd;
        }
        if (fb == 7) {
            switch (fc) {
            case 0: /* BigBDispatch ← B */         return pd;  /* IFU TBD */
            case 1: /* BDispatch ← B */            return pd;
            case 2: /* Multiply */                 return pd;  /* TBD */
            case 3: /* Q ← B */
                cpu->Q = b;
                return pd;
            case 4: /* unused */                   return pd;
            case 5: /* TgetsMd */                  return pd;  /* memory TBD */
            case 6: /* FreezeBC */                 return pd;
            case 7: /* Reserved as no-op */        return pd;
            }
        }
    }

    if (fa == 1) {
        if (fb == 0) {
            switch (fc) {
            case 0: /* PCF ← B */                  cpu->TPC = b;        return pd;
            case 1: /* IFUTest ← B */              return pd;
            case 2: /* IFUTick */                  return pd;
            case 3: /* RescheduleNow */            return pd;
            case 4: /* AckJunkTW ← B */            return pd;
            case 5: /* MemBase ← B[3:7] */
                cpu->MemBase = (b >> 8) & 0x1F;    return pd;
            case 6: /* RBase ← B[12:15] */
                cpu->RBase = b & 0xF;              return pd;
            case 7: /* Pointers ← B (MemBase ← B[3:7], RBase ← B[12:15]) */
                cpu->MemBase = (b >> 8) & 0x1F;
                cpu->RBase = b & 0xF;              return pd;
            }
        }
        if (fb == 2) {
            switch (fc) {
            case 2: /* CFlags ← A' */              return pd;  /* memory TBD */
            case 3: /* BrLo ← A */                 return pd;  /* BR TBD */
            case 4: /* BrHi ← A */                 return pd;
            case 5: /* LoadTestSyndrome */         return pd;
            case 6: /* LoadMcr[A,B] */             return pd;
            case 7: /* ProcSRN ← B[12:15] */       return pd;
            }
        }
        if (fb == 3) {
            switch (fc) {
            case 0: /* InsSetorEvent ← B */        return pd;
            case 1: /* EventCntB ← B */            return pd;
            case 2: /* Reschedule */               return pd;
            case 3: /* NoReschedule */             return pd;
            case 4: /* IFUMRH ← B */               return pd;
            case 5: /* IFUMLH ← B */               return pd;
            case 6: /* IFUReset */                 return pd;
            case 7: /* BrkIns ← B */               return pd;
            }
        }
        if (fb == 4) {
            switch (fc) {
            case 0: /* UseDMD */                   return pd;
            case 1: /* MidasStrobe ← B */          return pd;
            case 2: /* TaskingOff */               return pd;
            case 3: /* TaskingOn */                return pd;
            case 4: /* StkP ← B[8:15] */
                cpu->StkP = b & 0xFF;              return pd;
            case 5: /* RestoreStkP */              return pd;
            case 6: /* Cnt ← B */
                cpu->Cnt = b;                      return pd;
            case 7: /* Link ← B */
                cpu->Link = b;                     return pd;
            }
        }
        if (fb == 5) {
            switch (fc) {
            case 0: /* Q lsh 1 */
                cpu->Q = (uint16_t)(cpu->Q << 1);  return pd;
            case 1: /* Q rsh 1 */
                cpu->Q = (uint16_t)(cpu->Q >> 1);  return pd;
            case 2: /* TIOA[0:7] ← B[0:7] */
                cpu->TIOA = (b >> 8) & 0xFF;       return pd;
            case 3: /* — */                        return pd;
            case 4: /* Hold&TaskSim ← B */         return pd;
            case 5: /* WF ← A */                   return pd;  /* shifter ctrl TBD */
            case 6: /* RF ← A */                   return pd;
            case 7: /* ShC ← B */
                cpu->ShC = b;                      return pd;
            }
        }
        /* fb == 6, fb == 7 → B sources already handled in
         * ff_override_b; no post-ALU side effect. */
        if (fb == 6 || fb == 7) return pd;
    }

    if (fa == 2) {
        if (fb <= 1) {
            /* RBase ← FF[4:7] (the immediate decode). */
            cpu->RBase = u->ff & 0xF;
            return pd;
        }
        if (fb >= 2 && fb <= 3) {
            /* Replace RMaddr[0:3] with RBase[0:3], RMaddr[4:7] with
             * FF[4:7] — force RM write. We silently honor. */
            return pd;
        }
        if (fb == 4) {
            switch (fc) {
            case 0: case 1: case 2: case 3:
                /* TIOA[5:7] ← FF[5:7] (TIOA[0:4] unchanged) */
                cpu->TIOA = (cpu->TIOA & 0xF8) | (u->ff & 7);
                return pd;
            }
            return pd;
        }
        if (fb == 5) {
            switch (fc) {
            case 0: case 1: case 2: case 3:  /* MemBaseX ← FF[6:7] */
                cpu->MemBase = (cpu->MemBase & 0x18) |
                               ((cpu->MemBase >> 1) & 0x03) |
                               ((u->ff & 0x03) << 3);
                return pd;
            case 4: case 5: case 6: case 7:  /* MemBX ← FF[6:7] — TBD */
                return pd;
            }
        }
        if (fb == 6) {
            switch (fc) {
            case 0: case 1: /* — */          return pd;
            case 2: /* Pd ← ALUFMRW */
            case 3: /* Pd ← ALUFMEM (with ALUFMEM ← B.8, B[11:15]) */
                /* Read current value to Pd, write new value from B.
                 *
                 * HM Table 11d: "ALUFMEM ← B.8, B[11:15]". The 6-bit
                 * entry's bits map to B as follows (manual MSB-first):
                 *   ALUFM[0] (MSB / carry) = B[8]
                 *   ALUFM[1]                = B[11]
                 *   ...
                 *   ALUFM[5] (LSB)          = B[15]
                 *
                 * In C-LSB convention (alufm_entry as uint8_t with bit
                 * 5 = MSB, bit 0 = LSB), this means:
                 *   alufm[5] = B_C[7]  (= B[8] manual)
                 *   alufm[4] = B_C[4]  (= B[11])
                 *   alufm[3] = B_C[3]  (= B[12])
                 *   alufm[2] = B_C[2]  (= B[13])
                 *   alufm[1] = B_C[1]  (= B[14])
                 *   alufm[0] = B_C[0]  (= B[15])
                 *
                 * Empirical validation: real BB firmware writes CPReg
                 * = (Y << 8) | 0o25 with Y stale (typically 4) before
                 * jamming QFromCPReg + ALUFM[0]FromQ, expecting
                 * ALUFM[0] = 0o25. With this mapping, Q = ~CPReg =
                 * 0xFBEA gives ALUFM[0] = 0x15 = 0o25 ✓.
                 */
                {
                    int idx = u->aluf & 0xF;
                    uint8_t cur = (cpu->mc->alufm_present[idx])
                                  ? cpu->mc->alufm[idx] : 0;
                    pd = cur;  /* placeholder: real Pd would be 6-bit */
                    dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                    /* ALUFM_manual bit position → B manual bit:
                     *   0 (carry, MSB) ← B[15] (= B_C[0])
                     *   1              ← B[14] (= B_C[1])
                     *   2              ← B[13] (= B_C[2])
                     *   3              ← B[12] (= B_C[3])
                     *   4              ← B[11] (= B_C[4])
                     *   5 (LSB of op)  ← B[8]  (= B_C[7])
                     * In C-LSB ALUFM (bit 5 = manual MSB, bit 0 = LSB).
                     */
                    uint8_t alufm =
                        (uint8_t)((((b >> 0) & 1) << 5) |
                                  (((b >> 1) & 1) << 4) |
                                  (((b >> 2) & 1) << 3) |
                                  (((b >> 3) & 1) << 2) |
                                  (((b >> 4) & 1) << 1) |
                                  (((b >> 7) & 1) << 0));
                    mc_w->alufm[idx] = alufm;
                    mc_w->alufm_present[idx] = 1;
                }
                return pd;
            case 4: /* Pd ← Cnt */
                pd = cpu->Cnt;                     return pd;
            case 5: /* Pd ← Pointers (HM Table 11d FA=2 FB=6 FC=5).
                     *   Pd[1:2]   ← MemBX
                     *   Pd[3:7]   ← MemBase
                     *   Pd[8]     ← StkOvf
                     *   Pd[9]     ← StkUnd
                     *   Pd[12:15] ← RBase
                     * In C-LSB Pd: bit 14 = manual Pd[1], bit 0 = manual
                     * Pd[15]. */
                pd = 0;
                /* MemBX (2 bits) at manual Pd[1:2] = C-LSB bits 14..13 */
                pd |= (uint16_t)((cpu->MemBX & 0x3) << 13);
                /* MemBase (5 bits) at manual Pd[3:7] = C-LSB bits 12..8 */
                pd |= (uint16_t)((cpu->MemBase & 0x1F) << 8);
                /* StkOvf at manual Pd[8] = C-LSB bit 7 */
                pd |= (uint16_t)((cpu->stk_ovf & 1) << 7);
                /* StkUnd at manual Pd[9] = C-LSB bit 6 */
                pd |= (uint16_t)((cpu->stk_und & 1) << 6);
                /* RBase (4 bits) at manual Pd[12:15] = C-LSB bits 3..0 */
                pd |= (uint16_t)(cpu->RBase & 0xF);
                return pd;
            case 6: /* Pd ← TIOA&StkP */
                pd = (uint16_t)((cpu->TIOA << 8) | cpu->StkP);
                return pd;
            case 7: /* Pd ← ShC */
                pd = cpu->ShC;                     return pd;
            }
        }
        if (fb == 7) {
            /* Pd ← ALU shifted/cycled by 1. We don't currently expose
             * the ALU's pre-shift value; stub by returning alu. */
            return pd;
        }
    }

    if (fa == 3) {
        if (fb <= 3) {
            /* MemBase ← FF[3:7] (alt encoding) */
            cpu->MemBase = u->ff & 0x1F;
            return pd;
        }
        if (fb == 4 || fb == 5) {
            /* Cnt ← small constant (1..16). FF[4:7] of 0 means 16. */
            int n = u->ff & 0xF;
            cpu->Cnt = (uint16_t)(n == 0 ? 16 : n);
            return pd;
        }
        if (fb == 6 || fb == 7) {
            /* Wakeup[n] — initiate wakeup for task FF[4:7]. No tasking
             * model yet; stub. */
            return pd;
        }
    }

    return pd;
}

/* Compute B bus value (HM Table 7, primary sources). FF-driven external
 * sources (Pipe, Link, FaultInfo, CPReg, …) are handled by
 * ff_override_b before this fires. */
static int b_bus(const dorado_cpu *cpu, const dorado_uinstr *u, uint16_t *out)
{
    /* HM §3.11: when BSEL controls a shift (BSEL[0]=1 with ASEL=7),
     * "the B source is forced to be Q." */
    if (u->asel == 7 && (u->bsel & 4)) {
        *out = cpu->Q;
        return 0;
    }
    int rm_a = rm_address(cpu, u);
    switch (u->bsel) {
    case 0: /* Md (memory data) — no memory subsystem yet, return 0 */
        *out = 0;
        return 0;
    case 1: /* RM/STK */
        if (rm_a >= CPU_RMSTK_INVALID) return CPU_HALT_UNSUPPORTED_BSEL;
        *out = rm_stk_read(cpu, rm_a);
        return 0;
    case 2: /* T */
        *out = cpu->T;
        return 0;
    case 3: /* Q (or Q←B with external) */
        *out = cpu->Q;
        return 0;
    case 4: /* 0,,FF — high byte 0, low byte FF */
        *out = (uint16_t)u->ff;
        return 0;
    case 5: /* 0o377,,FF — high byte 0xFF, low byte FF */
        *out = (uint16_t)(0xFF00 | u->ff);
        return 0;
    case 6: /* FF,,0 — high byte FF, low byte 0 */
        *out = (uint16_t)((uint16_t)u->ff << 8);
        return 0;
    case 7: /* FF,,0o377 — high byte FF, low byte 0xFF */
        *out = (uint16_t)(((uint16_t)u->ff << 8) | 0xFF);
        return 0;
    }
    return CPU_HALT_UNSUPPORTED_BSEL;
}

/*
 * Shifter (HM §3.11 + Figure 4).
 *
 * 32-bit barrel shifter taking SHA (high 16) || SHB (low 16) as input,
 * cycling left by `count` (0..15), then masking with LMask/RMask
 * according to ALUF[0:2]. The low 16 bits go on A complemented (HM:
 * "the low order 16 bits of shifted data are placed *complemented* on
 * A"). ALUFM[14] = "NOT A" by convention completes the inversion so
 * the ALU ultimately delivers the shifter output.
 *
 * Two control paths:
 *   - Standard (BSEL[0]=0): SHA, SHB, count, masks all from ShC.
 *     ShC bits in BCPL ordering (bit 0 = MSB):
 *       ShC[2]    : SHA select (0=RM/STK, 1=T)         → C bit 13
 *       ShC[3]    : SHB select (0=RM/STK, 1=T)         → C bit 12
 *       ShC[4:7]  : shift count                         → C bits 11..8
 *       ShC[8:11] : RMask count                         → C bits 7..4
 *       ShC[12:15]: LMask count                         → C bits 3..0
 *
 *   - FF-controlled (BSEL[0]=1): SHA = BSEL[1], SHB = BSEL[2],
 *     count = FF[4:7], RMask = FF[4:7], LMask = FF[0:3].
 *     B is forced to be Q in this mode (we do that in b_bus).
 *
 * Mask ops (ALUF[0:2], from HM Table 12):
 *   0 ShiftNoMask    1 ShiftLMask    2 ShiftRMask    3 ShiftBothMasks
 *   4 (unused)       5 ShMdLMask     6 ShMdRMask     7 ShMdBothMasks
 *   "Sh" variants replace masked bits with 0; "ShMd" with Md (memory data).
 */
static uint16_t shifter_output(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    int rm_a   = rm_address(cpu, u);
    uint16_t r = (rm_a < CPU_RMSTK_INVALID) ? rm_stk_read(cpu, rm_a) : 0;
    uint16_t t = cpu->T;
    uint16_t sha, shb;
    int count, rmask_n, lmask_n;

    if (u->bsel & 4) {
        /* FF-controlled — high bit of BSEL set. */
        sha = (u->bsel & 2) ? t : r;
        shb = (u->bsel & 1) ? t : r;
        count   = u->ff & 0xF;          /* FF[4:7] */
        rmask_n = u->ff & 0xF;          /* FF[4:7] */
        lmask_n = (u->ff >> 4) & 0xF;   /* FF[0:3] */
    } else {
        /* ShC-controlled. */
        sha = ((cpu->ShC >> 13) & 1) ? t : r;
        shb = ((cpu->ShC >> 12) & 1) ? t : r;
        count   = (cpu->ShC >> 8) & 0xF;
        rmask_n = (cpu->ShC >> 4) & 0xF;
        lmask_n = cpu->ShC & 0xF;
    }

    /* Cycle 32-bit (sha||shb) left by `count`. */
    uint32_t in32 = ((uint32_t)sha << 16) | shb;
    uint32_t cyc;
    if (count == 0) cyc = in32;
    else            cyc = (in32 << count) | (in32 >> (32 - count));
    uint16_t lo16 = (uint16_t)(cyc & 0xFFFF);

    /* Build the actual mask words. LMask: N ones at high end (bits
     * 15..16-N). RMask: N ones at low end (bits N-1..0). */
    uint16_t lmask = lmask_n ? (uint16_t)(0xFFFF << (16 - lmask_n)) : 0;
    uint16_t rmask = rmask_n ? (uint16_t)((1u << rmask_n) - 1)      : 0;

    int op = (u->aluf >> 1) & 7;        /* ALUF[0:2] in BCPL = high 3 bits */
    uint16_t mask;
    int with_md;
    switch (op) {
    case 0: mask = 0;             with_md = 0; break;   /* ShiftNoMask */
    case 1: mask = lmask;         with_md = 0; break;   /* ShiftLMask */
    case 2: mask = rmask;         with_md = 0; break;   /* ShiftRMask */
    case 3: mask = lmask | rmask; with_md = 0; break;   /* ShiftBothMasks */
    case 4: mask = 0;             with_md = 1; break;   /* ShMdNoMask (unused) */
    case 5: mask = lmask;         with_md = 1; break;   /* ShMdLMask */
    case 6: mask = rmask;         with_md = 1; break;   /* ShMdRMask */
    case 7: mask = lmask | rmask; with_md = 1; break;   /* ShMdBothMasks */
    default: mask = 0; with_md = 0; break;
    }

    /* Md substitution: we don't model memory yet — Md = 0 stub. When
     * memory lands this becomes (with_md ? cpu->Md : 0). */
    uint16_t fill = with_md ? 0 : 0;
    return (uint16_t)((lo16 & ~mask) | (fill & mask));
}

/* Compute A bus value (HM Table 8a/8b primary sources only). */
static int a_bus(const dorado_cpu *cpu, const dorado_uinstr *u, uint16_t *out)
{
    int rm_a = rm_address(cpu, u);
    switch (u->asel) {
    case 0: case 1: case 2: case 3:
        /* Memory references (Store←/Fetch←/...): A = RM/STK or T or Md
         * or Q depending on the FF[0:1] sub-decode. No memory subsystem
         * yet — return 0 and let the instruction be a no-op. Real
         * microcode does sometimes use these as "implicit no-ops"
         * (e.g., Boot0 trap reservations). */
        *out = 0;
        return 0;
    case 4: /* A←RM/STK */
        if (rm_a >= CPU_RMSTK_INVALID) return CPU_HALT_UNSUPPORTED_ASEL;
        *out = rm_stk_read(cpu, rm_a);
        return 0;
    case 5: /* A←Id (IFU operand byte) — no IFU yet */
        return CPU_HALT_UNSUPPORTED_ASEL;
    case 6: /* A←T */
        *out = cpu->T;
        return 0;
    case 7: /* Shift. A bus = ~shifter_output (low-true). */
        *out = (uint16_t)~shifter_output(cpu, u);
        return 0;
    }
    return CPU_HALT_UNSUPPORTED_ASEL;
}

/*
 * ALU operation per ALUFM[ALUF] (HM Table 9).
 *
 * ALUFM is 6 bits wide:
 *   bit 0 (MSB)   → carry-in for arithmetic ops
 *   bits 1..5     → 5-bit operation selector (logical / arithmetic)
 *
 * The 5-bit selector values are listed in Table 9 (octal):
 *   *01 NOT A          15 A XNOR B
 *   *03 (NOT A) OR (NOT B)
 *    05 (NOT A) OR B
 *    07 all-ones
 *    11 (NOT A) AND (NOT B)
 *   *13 NOT B
 *    15 (already)      17 A OR (NOT B)
 *    21 (NOT A) AND B
 *   *23 A XOR B
 *   *25 B
 *   *27 A OR B
 *    31 all-zeroes
 *  **33 A AND (NOT B)
 *   *35 A AND B
 *    37 A
 *
 *  For arithmetic with carry-in = 0:  *0 A, *6 2A, *14 A+B, *22 A-B-1, *36 A-1
 *  With carry-in = 1:                 *0 A+1, 6 2A+1, *14 A+B+1, *22 A-B, 36 A
 */
static uint16_t alu_op(uint8_t alufm_entry, uint16_t a, uint16_t b,
                       uint8_t *out_carry, uint8_t *out_overflow)
{
    uint8_t carry_in = (alufm_entry >> 5) & 1;   /* high bit */
    uint8_t op = alufm_entry & 0x1F;             /* low 5 bits */
    uint32_t result;
    uint8_t carry_out = 0;
    uint8_t overflow = 0;

    switch (op) {
    /* Logical ops (ignore carry_in). */
    case 001: result = (uint16_t)~a; break;
    case 003: result = (uint16_t)(~a | ~b); break;
    case 005: result = (uint16_t)(~a | b); break;
    case 007: result = 0xFFFF; break;
    case 011: result = (uint16_t)(~a & ~b); break;
    case 013: result = (uint16_t)~b; break;
    case 015: result = (uint16_t)(~(a ^ b)); break;
    case 017: result = (uint16_t)(a | ~b); break;
    case 021: result = (uint16_t)(~a & b); break;
    case 023: result = (uint16_t)(a ^ b); break;
    case 025: result = b; break;
    case 027: result = (uint16_t)(a | b); break;
    case 031: result = 0; break;
    case 033: result = (uint16_t)(a & ~b); break;
    case 035: result = (uint16_t)(a & b); break;
    case 037: result = a; break;

    /* Arithmetic. carry_in is added. */
    case 000: { /* A (+1 if carry_in) */
        uint32_t r = (uint32_t)a + carry_in;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        break;
    }
    case 006: { /* 2*A (+1 if carry_in) */
        uint32_t r = (uint32_t)a + a + carry_in;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        break;
    }
    case 014: { /* A + B (+1 if carry_in) */
        uint32_t r = (uint32_t)a + b + carry_in;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        /* signed-overflow: HM §3.7 "Overflow ... iff a signed
         * arithmetic operation yields an incorrect result" */
        uint16_t sa = a & 0x8000, sb = b & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == sb && sa != sr) ? 1 : 0;
        break;
    }
    case 022: { /* A - B - 1 + carry_in (so carry=1 → A-B) */
        uint32_t r = (uint32_t)a + (uint16_t)~b + carry_in;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        uint16_t sa = a & 0x8000, sb = (~b) & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == sb && sa != sr) ? 1 : 0;
        break;
    }
    case 036: { /* A - 1 (+ carry_in) */
        uint32_t r = (uint32_t)a + 0xFFFF + carry_in;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        break;
    }
    default:
        result = 0;
        break;
    }

    if (out_carry)    *out_carry    = carry_out;
    if (out_overflow) *out_overflow = overflow;
    return (uint16_t)result;
}

/* Apply LC: deliver Pd to T and/or RM/STK (HM Table 10). */
static int apply_lc(dorado_cpu *cpu, const dorado_uinstr *u, uint16_t pd)
{
    /* Use the *write* address for STK accesses — that may differ from
     * the read address when ModStkPBeforeW is in effect (HM page 11). */
    int rm_a = stk_write_address(cpu, u);
    int has_rm = (rm_a < CPU_RMSTK_INVALID);
    switch (u->lc) {
    case 0: /* No Action */
        break;
    case 1: /* T←Pd */
        cpu->T = pd;
        break;
    case 2: /* T←Md, RM/STK←Pd — Md not implemented */
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        rm_stk_write(cpu, rm_a, pd);
        /* T←Md skipped; will be handled when memory lands */
        break;
    case 3: /* T←Md — not implemented */
        return CPU_HALT_UNSUPPORTED_ASEL;
    case 4: /* RM/STK←Md — not implemented */
        return CPU_HALT_UNSUPPORTED_ASEL;
    case 5: /* T←Pd, RM/STK←Md — partial: write T only for now */
        cpu->T = pd;
        break;
    case 6: /* RM/STK←Pd */
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        rm_stk_write(cpu, rm_a, pd);
        break;
    case 7: /* T←Pd, RM/STK←Pd */
        cpu->T = pd;
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        rm_stk_write(cpu, rm_a, pd);
        break;
    }
    return 0;
}

/*
 * Evaluate the branch condition selected by `cond` (3-bit value 0..6).
 * Returns 1 if the condition is true, 0 if false. Mutates Cnt for
 * condition 3 (Cnt=0&-1 decrements after testing).
 *
 * See HM Table 13 and docs/jcn-encoding.md.
 */
static int eval_branch_condition(dorado_cpu *cpu,
                                 const dorado_uinstr *u, int cond)
{
    int rm_a = rm_address(cpu, u);
    int r = 0;
    switch (cond) {
    case 0:  r = cpu->alu_zero;                          break;  /* ALU = 0 */
    case 1:  r = cpu->alu_lt0;                           break;  /* ALU < 0 */
    case 2:  r = cpu->alu_carry ? 0 : 1;                 break;  /* Carry' */
    case 3:  r = (cpu->Cnt == 0) ? 1 : 0;                          /* Cnt = 0&-1 */
             cpu->Cnt = (uint16_t)(cpu->Cnt - 1);                  /* …decrement after test */
             break;
    case 4:  r = (rm_a < CPU_RMSTK_INVALID && (rm_stk_read(cpu, rm_a) & 0x8000)) ? 1 : 0; break; /* R<0 */
    case 5:  r = (rm_a < CPU_RMSTK_INVALID && (rm_stk_read(cpu, rm_a) & 0x0001)) ? 1 : 0; break; /* R odd */
    case 6:  r = 1;                                       break;  /* IOAtten' / Resched — stub: no I/O attention pending (active-low) */
    default: r = 0;                                       break;
    }
    return r;
}

/*
 * Compute next real_PC from JCN (HM §4.3, Table 13, Figure 6).
 *
 * Encoding discriminator (high bits of JCN, MSB-first):
 *   1 0 . . . . . .   →  Local Jump/Call
 *   1 1 . . . . . .   →  Global Call
 *   0 0 0 0 . . . .   →  Long Jump/Call (FF supplies 8 of 12 addr bits)
 *   0 1 . . . 1 1 1   →  Return / TPC / IM
 *   0 0 1 . . 1 1 1   →  IFU Jump
 *   0 . . . . c c c   →  Conditional Jump/Call (c = condition 0..6, ≠ 7)
 *
 * See docs/jcn-encoding.md for the full reference.
 */
static int next_pc(dorado_cpu *cpu, const dorado_uinstr *u, uint16_t *next)
{
    uint8_t jcn = u->jcn;
    uint8_t top1 = (jcn >> 7) & 1;
    uint8_t top2 = (jcn >> 6) & 3;

    if (top1 == 1) {
        /* Local or Global. */
        uint16_t page_addr = jcn & 0x3F;             /* JCN[2:7] */
        if (top2 == 2) {
            /* Local Jump/Call. */
            uint16_t page = cpu->real_PC & ~(uint16_t)(CPU_PAGE_SIZE - 1);
            *next = page | page_addr;
            if ((page_addr & 0xF) == 0) {            /* TNIA[12:15] = 0 → Call */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
            }
            return 0;
        }
        /* Global Call: low 6 bits forced 0, so TNIA[12:15] always 0. */
        uint16_t quadrant = cpu->real_PC & ~(uint16_t)(CPU_QUADRANT_SIZE - 1);
        *next = quadrant | (uint16_t)(page_addr * CPU_PAGE_SIZE);
        cpu->Link = (uint16_t)(cpu->real_PC + 1);
        return 0;
    }

    /* top1 == 0 — long, conditional, return-class, IFU jump, or undefined. */
    uint8_t low3 = jcn & 7;

    if (((jcn >> 4) & 0xF) == 0) {
        /* Long Jump/Call. FF disabled as a function.
         * TNIA[12:15] = JCN[4:7] = (jcn & 0xF). */
        uint16_t quadrant = cpu->real_PC & ~(uint16_t)(CPU_QUADRANT_SIZE - 1);
        uint16_t addr12   = (uint16_t)(((jcn & 0xF) << 8) | u->ff);
        *next = quadrant | addr12;
        if ((jcn & 0xF) == 0) {
            cpu->Link = (uint16_t)(cpu->real_PC + 1);
        }
        return 0;
    }

    if (low3 == 7) {
        /* Return / TPC / IM, or IFU Jump. */
        uint8_t bit1 = (jcn >> 6) & 1;     /* JCN[1] */
        uint8_t bits02 = (jcn >> 5) & 7;   /* JCN[0:2] */
        if (bits02 == 1) {
            /* IFU Jump (0 0 1 _ _ 1 1 1). No IFU yet. */
            return CPU_HALT_UNSUPPORTED_JCN;
        }
        if (bit1 == 1) {
            /* Return / Read TPC / Write TPC / Read IM / Write IM
             * (0 1 f f f 1 1 1) where f = JCN[2:4]. */
            uint8_t fn = (jcn >> 3) & 7;   /* JCN[2:4] */
            if (fn == 0) {
                /* Subroutine Return: TNIA = Link[2:15]. Use the
                 * issue-time snapshot so a same-instruction Link←B
                 * (which can't appear in a "Return#" template but is
                 * possible in other return-class encodings) doesn't
                 * change where we go this cycle. */
                *next = (uint16_t)(cpu->link_at_issue & 0xFFF);
                /* Per HM §4.5: Link is reloaded with CIA+1 by Return,
                 * supporting CoReturn. */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 7) {
                /* Write IM (HM §4.8). Address from Link[4:15] (12-bit
                 * IM address); 16 bits of data from B; 2 bits from
                 * RSTK select the half-word and a "secondary" bit:
                 *
                 *   RSTK[3] (LSB of u->rstk in C) — half-select.
                 *     1 = LH (writes iw0 + RSTK[0] of the destination)
                 *     0 = RH (writes iw1 + JCN[7] of the destination)
                 *   RSTK[2] — value of the secondary bit.
                 *   RSTK[1] — parity bit (we ignore it; we don't model
                 *             parity errors).
                 *   RSTK[0] — unused for Write IM.
                 *
                 * Per the manual, "Tasking-off must be in force" and
                 * Hold is illegal during Write IM. We don't enforce
                 * either; emulator microcode that uses this path
                 * (Bootstrap, Boot0 loader) is single-task and never
                 * generates Hold during the access window.
                 */
                uint16_t addr = (uint16_t)(cpu->link_at_issue & 0xFFF);
                uint16_t b = 0;
                /* Try the FF override first — IMLH/IMRH instructions
                 * pair Write IM with B←RWCPReg (BSEL=Md), and BSEL=0
                 * by itself would otherwise halt with UNSUPPORTED_BSEL
                 * because we don't model the memory subsystem yet. */
                if (!ff_override_b(cpu, u, &b)) {
                    int rc = b_bus(cpu, u, &b);
                    if (rc != 0) return rc;
                }

                dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                dorado_uinstr *dst = &mc_w->im[addr];
                int half_lh   = (u->rstk & 1);         /* RSTK[3] manual */
                int secondary = (u->rstk >> 1) & 1;    /* RSTK[2] manual */

                if (half_lh) {
                    dst->iw0 = b;
                    /* iw2 bit 15 = manual RSTK[0] of destination. */
                    dst->iw2 = (uint16_t)((dst->iw2 & ~0x8000u) |
                                          ((uint16_t)secondary << 15));
                } else {
                    dst->iw1 = b;
                    /* iw2 bit 14 = manual JCN[7] of destination. */
                    dst->iw2 = (uint16_t)((dst->iw2 & ~0x4000u) |
                                          ((uint16_t)secondary << 14));
                }
                dorado_redecode_fields(dst);
                mc_w->im_present[addr] = 1;
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 4 || fn == 5 || fn == 6) {
                /* Read TPC (4), Write TPC (5), Read IM (6).
                 *
                 * Read IM places inverted data on B in the *next*
                 * instruction (selected by Link[7:15], byte by RSTK[2:3]).
                 * TPC ops similarly reach across instructions. None of
                 * these are exercised by Bootstrap's CPReg-based loader,
                 * so we just advance past the access for now and revisit
                 * if microcode that uses them needs it.
                 */
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            /* fn = 1, 2, 3 are unused. */
            return CPU_HALT_UNSUPPORTED_JCN;
        }
        /* Undefined (0 0 0 1 x 1 1 1). */
        return CPU_HALT_UNSUPPORTED_JCN;
    }

    /* Conditional Jump/Call.
     *
     *   page_offset = (JCN[1:2] << 4) | (JCN[3:4] << 1) | R
     *   where R = condition result.
     */
    int cond_value = eval_branch_condition(cpu, u, low3);
    uint16_t page_high = (jcn >> 5) & 3;          /* JCN[1:2] */
    uint16_t page_low  = (jcn >> 3) & 3;          /* JCN[3:4] */
    uint16_t offset    = (uint16_t)((page_high << 4) | (page_low << 1) | cond_value);
    uint16_t page      = cpu->real_PC & ~(uint16_t)(CPU_PAGE_SIZE - 1);
    *next = page | offset;
    /* Call vs Jump on conditional: it's a Call iff TNIA[12:15] = 0
     * before R is OR'd, i.e. when JCN[3:4] = 0. We stash CIA+1 in
     * Link in that case (per HM Figure 6 / §4.5). */
    if (page_low == 0) {
        cpu->Link = (uint16_t)(cpu->real_PC + 1);
    }
    return 0;
}

/* Forward declare so the BB-injected single-step path (below) can
 * call into the same execution body the normal IM-fetch path uses. */
static int execute_uinstr(dorado_cpu *cpu, const dorado_uinstr *u, int from_im);

int dorado_cpu_step(dorado_cpu *cpu)
{
    if (cpu->halted) return 1;

    /*
     * If a BaseBoard is wired up and is driving the control bus,
     * fold its state into our step decision:
     *
     *   ss_pending + mir_loaded → BB has just MIR-injected one
     *     microinstruction and asserted SetSS+SetRun. Execute that
     *     uinstr (not from IM) and clear ss_pending.
     *
     *   running == 0 → BB hasn't released the Dorado; the microengine
     *     is held. Step is a no-op (we still tick the BB so it can
     *     eventually issue SetRun).
     *
     *   running == 1 → free-running; fetch from IM as usual.
     */
    if (cpu->baseboard) {
        if (cpu->baseboard->dorado_ss_pending && cpu->baseboard->dorado_mir_loaded) {
            dorado_uinstr injected;
            dorado_decode_mir(cpu->baseboard->mir_bytes, &injected);
            /* Edge-triggered: clear *both* mir_loaded and ss_pending
             * so the BB's follow-up SetRun+SetSS strobes (which the
             * firmware uses to "release" the SS line in BasicStopDorado)
             * don't re-execute the same microinstruction. The next
             * single-step requires a fresh MIR0..MIR3 strobe sequence. */
            cpu->baseboard->dorado_ss_pending = 0;
            cpu->baseboard->dorado_mir_loaded = 0;
            int rc = execute_uinstr(cpu, &injected, 0 /* not from IM */);
            cpu->cycles++;
            if (cpu->baseboard_cycles_per_uop > 0) {
                baseboard_run(cpu->baseboard, cpu->baseboard_cycles_per_uop);
            }
            return rc;
        }
        if (!cpu->baseboard->dorado_running) {
            /* Held — tick the BB and count this as a wasted cycle.
             * Increment cpu->cycles so dorado_cpu_run terminates after
             * max_cycles even if the BB never releases Run. */
            if (cpu->baseboard_cycles_per_uop > 0) {
                baseboard_run(cpu->baseboard, cpu->baseboard_cycles_per_uop);
            }
            cpu->cycles++;
            return 0;
        }
    }

    if (cpu->real_PC >= IM_SIZE || !cpu->mc->im_present[cpu->real_PC]) {
        cpu->halted = 1;
        cpu->halt_reason = CPU_HALT_NO_CODE;
        return 1;
    }

    const dorado_uinstr *u = &cpu->mc->im[cpu->real_PC];

    if (cpu->mc->im_breakpoint[cpu->real_PC]) {
        cpu->halted = 1;
        cpu->halt_reason = CPU_HALT_BREAKPOINT;
        return 1;
    }

    return execute_uinstr(cpu, u, 1 /* from IM */);
}

static int execute_uinstr(dorado_cpu *cpu, const dorado_uinstr *u, int from_im)
{
    (void)from_im;

    /* Snapshot Link before FF can modify it. Write IM (in next_pc) and
     * Subroutine Return both consume Link at instruction-issue time;
     * B←RWCPReg / Link←B / B-dispatch all overwrite Link via FF. The
     * pipelined hardware reads Link at t1 and writes at t3, so the
     * consumer sees the *old* value while the new value lands for the
     * next instruction. We model this by capturing the entry value. */
    cpu->link_at_issue = cpu->Link;

    /* B and A buses. FF may override B (Pipe / Link / CPReg / …). */
    uint16_t b = 0, a = 0;
    int rc;
    if (!ff_override_b(cpu, u, &b)) {
        if ((rc = b_bus(cpu, u, &b)) != 0) {
            cpu->halted = 1;
            cpu->halt_reason = rc;
            return 1;
        }
    }
    if ((rc = a_bus(cpu, u, &a)) != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* ALU. */
    /* On a barrel shift, the first three ALUFM address bits are forced
     * to 1 (HM §3.11), so the index becomes 14 or 15 (= 0o16 or 0o17)
     * with ALUF[3] selecting between them. ALUFM[14] holds "NOT A" by
     * convention, completing the shifter's complement-on-A path. */
    int aluf_idx = (u->asel == 7) ? (14 + (u->aluf & 1)) : u->aluf;
    uint8_t alufm_entry = cpu->mc->alufm_present[aluf_idx]
                          ? cpu->mc->alufm[aluf_idx]
                          : 0;
    uint8_t new_carry = cpu->alu_carry, new_ovf = cpu->alu_overflow;
    uint16_t alu = alu_op(alufm_entry, a, b, &new_carry, &new_ovf);
    cpu->alu_zero = (alu == 0) ? 1 : 0;
    cpu->alu_lt0  = (alu & 0x8000) ? 1 : 0;
    cpu->alu_carry = new_carry;
    cpu->alu_overflow = new_ovf;

    /* Pd = ALU output (FF may then override or transform Pd, e.g. via
     * ALUFMRW or Pd←Cnt). FF post-effects also fire register loads
     * like Cnt←B, Link←B, RBase←FF[4:7], etc. */
    int ff_halt = 0;
    uint16_t pd = ff_apply_post(cpu, u, b, alu, &ff_halt);
    if (ff_halt) {
        cpu->halted = 1;
        cpu->halt_reason = ff_halt;
        return 1;
    }

    /*
     * Branch decision uses the *pre-LC* register values per HM §4.4
     * ("the branch conditions are ordinarily loaded into the RAM at
     * t3" — i.e., latched from the register file before this cycle's
     * register write). Compute next PC first, then commit the LC write.
     */
    uint16_t np = (uint16_t)(cpu->real_PC + 1);
    rc = next_pc(cpu, u, &np);
    if (rc != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* LC. */
    if ((rc = apply_lc(cpu, u, pd)) != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* Post-instruction StkP update + StkOvf/StkUnd recompute (HM
     * Table 6 / page 11). For BLOCK=0 instructions this is a no-op. */
    stk_apply_post(cpu, u);

    if (cpu->trace && cpu->trace_fp) {
        FILE *fp = cpu->trace_fp;
        char line[256];
        dorado_format(u, line, sizeof line);
        fprintf(fp,
                "  PC=%04o T=%06o Q=%06o A=%06o B=%06o ALU=%06o  → PC=%04o\n"
                "    %s\n",
                cpu->real_PC, cpu->T, cpu->Q, a, b, alu, np, line);
    }

    cpu->prev_PC = cpu->real_PC;
    cpu->real_PC = np;

    /* Cycle accounting + BB stepping only happen on IM-fetched
     * instructions. The injected-MIR caller (dorado_cpu_step) does
     * its own cycle++/baseboard_run after we return — it has to,
     * because we got here without any cpu->real_PC change. */
    if (from_im) {
        cpu->cycles++;
        if (cpu->baseboard && cpu->baseboard_cycles_per_uop > 0) {
            baseboard_run(cpu->baseboard, cpu->baseboard_cycles_per_uop);
        }
    }
    return 0;
}

cpu_halt_reason dorado_cpu_run(dorado_cpu *cpu, int max_cycles)
{
    while (!cpu->halted && cpu->cycles < max_cycles) {
        if (dorado_cpu_step(cpu) != 0) break;
    }
    if (!cpu->halted && cpu->cycles >= max_cycles) {
        cpu->halted = 1;
        cpu->halt_reason = CPU_HALT_USER;
    }
    return cpu->halt_reason;
}
