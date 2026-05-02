#include "cpu.h"
#include "baseboard.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DORADO_JUNK_TASK          2
#define DORADO_JUNK_TICK_CYCLES   1000  /* 32 us / 32 ns */
#define DORADO_B15_MASK           0x0001u

/* Forward declarations for IFU helpers (defined later in this file). */
static uint16_t ifu_consume_id(dorado_cpu *cpu);
static uint8_t  ifu_fetch_byte(dorado_cpu *cpu, uint16_t pc, int *out_faulted);
static uint16_t task_md(const dorado_cpu *cpu);

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
    case CPU_HALT_IFU_NOT_READY:      return "halt: IFU not ready";
    case CPU_HALT_IFU_NO_ENTRY:       return "halt: IFUM entry missing";
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
    /* Tasking. CTask=0 (emulator); task 0 is always Ready (HM page 26
     * "task 0 always awake"). Other tasks start un-ready and have no
     * pending wakeup until microcode or a device asserts one.
     * Tasking is on by default; TaskingOff disables it. */
    cpu->ctask     = 0;
    cpu->ready     = 0x0001;     /* task 0 always ready */
    cpu->tasking_on = 1;
}

void dorado_cpu_wakeup(dorado_cpu *cpu, int task)
{
    cpu->wakeup_pending |= (uint16_t)(1u << (task & 0xF));
}

void dorado_cpu_set_task_tpc(dorado_cpu *cpu, int task, uint16_t real_pc)
{
    int t = task & 0xF;
    cpu->task_tpc[t] = real_pc;
    /* If we're setting the current task's TPC, also update the live
     * PC so the next instruction fetch sees it. */
    if (t == cpu->ctask) cpu->real_PC = real_pc;
}

uint16_t dorado_cpu_get_task_tpc(const dorado_cpu *cpu, int task)
{
    int t = task & 0xF;
    return (t == cpu->ctask) ? cpu->real_PC : cpu->task_tpc[t];
}

void dorado_cpu_set_subtask(dorado_cpu *cpu, int task, uint8_t subtask)
{
    cpu->task_subtask[task & 0xF] = (uint8_t)(subtask & 3);
}

/* Best Next Task — the highest-priority bit set in (ready|wakeup).
 * Task 0 (emulator) is always available, so worst case BNT = 0. */
static int task_bnt(uint16_t avail)
{
    avail |= 0x0001;   /* task 0 always ready */
    for (int i = 15; i > 0; i--) {
        if (avail & (uint16_t)(1u << i)) return i;
    }
    return 0;
}

static void junk_timer_enable(dorado_cpu *cpu, int enable)
{
    cpu->wakeup_pending &= (uint16_t)~(1u << DORADO_JUNK_TASK);
    cpu->junk_tw_enabled = enable ? 1 : 0;
    if (cpu->junk_tw_enabled && cpu->junk_tw_countdown == 0) {
        cpu->junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
    }
}

static void junk_timer_ack_control(dorado_cpu *cpu, uint16_t b)
{
    /* HM §12.1 and Kernel5.mc: AckJunkTW←B dismisses the current
     * wakeup; B[15]=1 enables periodic junk wakeups and B[15]=0
     * disables them. Dorado bit 15 is the low-order C bit. */
    junk_timer_enable(cpu, (b & DORADO_B15_MASK) != 0);
}

static void junk_timer_ifutest_control(dorado_cpu *cpu, uint16_t b)
{
    /* HM §8.3: IFUTest←B also dismisses the junk wakeup, but the IFU
     * test register uses the opposite polarity: IFUTest.15 disables
     * the periodic junk request. Dorado bit 15 is the low-order C bit. */
    junk_timer_enable(cpu, (b & DORADO_B15_MASK) == 0);
}

static void junk_timer_tick(dorado_cpu *cpu)
{
    if (!cpu->junk_tw_enabled) return;
    if (cpu->junk_tw_countdown == 0) {
        cpu->junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
    }
    if (--cpu->junk_tw_countdown == 0) {
        cpu->wakeup_pending |= (uint16_t)(1u << DORADO_JUNK_TASK);
        cpu->junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
    }
}

/* Save the current task's live state into task_*[ctask], using
 * `next_pc` as the resume point (the just-computed JCN target). */
static void task_save(dorado_cpu *cpu, uint16_t next_pc)
{
    int t = cpu->ctask;
    cpu->task_t[t]       = cpu->T;
    cpu->task_tpc[t]     = next_pc;
    cpu->task_link[t]    = cpu->Link;
    cpu->task_membase[t] = (uint8_t)cpu->MemBase;
    cpu->task_rbase[t]   = (uint8_t)cpu->RBase;
    cpu->task_tioa[t]    = (uint8_t)cpu->TIOA;
}

/* Load `task`'s saved state into the live registers. After this
 * call, `cpu->real_PC` is the task's saved TPC. */
static void task_load(dorado_cpu *cpu, int task)
{
    int t = task & 0xF;
    cpu->ctask   = (uint8_t)t;
    cpu->T       = cpu->task_t[t];
    cpu->real_PC = cpu->task_tpc[t];
    cpu->Link    = cpu->task_link[t];
    cpu->MemBase = cpu->task_membase[t];
    cpu->RBase   = cpu->task_rbase[t];
    cpu->TIOA    = cpu->task_tioa[t];
}

/* End-of-instruction task scheduling. `next_pc` is the JCN-computed
 * resume PC of the currently-executing task. `block_in_non_emulator`
 * is 1 if this instruction was BLOCK=1 in a non-emulator task — that
 * clears Ready[ctask] and forces a switch. Returns the chosen new
 * value of `real_PC` (which the caller assigns to advance the
 * engine). */
static uint16_t task_schedule(dorado_cpu *cpu, uint16_t next_pc,
                              int block_in_non_emulator)
{
    /* TaskingOn delay: TaskingOn doesn't take effect until two more
     * instructions of the same task have executed (HM page 27). */
    if (cpu->tasking_resume_delay > 0) {
        cpu->tasking_resume_delay--;
        if (cpu->tasking_resume_delay == 0) cpu->tasking_on = 1;
    }

    /* If the current task is non-emulator and blocked, clear Ready. */
    if (block_in_non_emulator) {
        cpu->ready &= (uint16_t)~(1u << cpu->ctask);
        /* A terminal `Block, Branch[.]` has consumed the wakeup it was
         * servicing. If a synthetic device asserted the bit again just
         * before the terminal block, leaving it pending immediately
         * re-enters code that expects to be asleep. Do not clear ordinary
         * inter-word blocks; Ethernet input relies on those wakeups. */
        if (next_pc == cpu->real_PC) {
            cpu->wakeup_pending &= (uint16_t)~(1u << cpu->ctask);
        }
    }

    /* If tasking is off, no switch — current task keeps running. */
    if (!cpu->tasking_on) return next_pc;

    /* Find Best Next Task. */
    uint16_t avail = cpu->ready | cpu->wakeup_pending;
    int bnt = task_bnt(avail);

    /* Switch iff BNT > CTASK (higher priority woke up), or the
     * non-emulator current task is BLOCKing. */
    int should_switch = (bnt > (int)cpu->ctask) || block_in_non_emulator;

    if (block_in_non_emulator && bnt == (int)cpu->ctask) {
        /* CTASK just blocked but BNT picked it again — that means
         * no other ready task. Pick again from a mask that excludes
         * ctask. */
        uint16_t avail2 = (cpu->ready | cpu->wakeup_pending)
                          & (uint16_t)~(1u << cpu->ctask);
        bnt = task_bnt(avail2);
    }

    if (!should_switch || bnt == (int)cpu->ctask) return next_pc;

    /* Save current state, then jump to BNT. The wakeup is
     * acknowledged (cleared) and the new task is now Ready. */
    task_save(cpu, next_pc);
    cpu->wakeup_pending &= (uint16_t)~(1u << bnt);
    cpu->ready          |= (uint16_t)(1u << bnt);
    task_load(cpu, bnt);
    return cpu->real_PC;
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
    if (u->block && cpu->ctask == 0) {
        return CPU_RMSTK_STK_BASE | (cpu->StkP & 0xFF);
    }
    /* HM page 88: "the processor OR's SubTask[0:1] into RBase[2:3]"
     * — the low 2 bits of RBase. Selects a sub-region of RM for
     * I/O tasks driven by multiple sub-devices. */
    uint8_t rbase = cpu->RBase & 0xF;
    if (cpu->ctask != 0) {
        rbase |= cpu->task_subtask[cpu->ctask] & 3;
    }
    return ((uint32_t)rbase << 4) | (u->rstk & 0xF);
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
    if (!u->block || cpu->ctask != 0) return rm_address(cpu, u);

    /* ModStkPBeforeW FF = FA=0 FB=2 FC=7 = 0o27 = 0x17 */
    int fa = (u->ff >> 6) & 3;
    int fb = (u->ff >> 3) & 7;
    int fc = u->ff & 7;
    int mod_before_w = (fa == 0 && fb == 2 && fc == 7);
    if (!mod_before_w) return rm_address(cpu, u);

    int adjusted = (cpu->StkP + stk_signed_delta(u)) & 0xFF;
    return CPU_RMSTK_STK_BASE | adjusted;
}

static int lc_write_address(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    int fa = (u->ff >> 6) & 3;
    int fb = (u->ff >> 3) & 7;

    /* HM Table 11a/11d: these FF groups replace the LC destination's
     * low RM address nibble with FF[4:7] while keeping the current
     * RBase high nibble, and force the write into RM even when the
     * instruction read STK. SubTask OR does not apply to this explicit
     * write-region form. */
    if ((fa == 0 && (fb == 4 || fb == 5)) ||
        (fa == 2 && (fb == 2 || fb == 3))) {
        return ((cpu->RBase & 0xF) << 4) | (u->ff & 0xF);
    }

    return stk_write_address(cpu, u);
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
    if (!u->block || cpu->ctask != 0) return;
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

static int ff_decode_ok(const dorado_uinstr *u)
{
    /* HM §3.6: FF is available as a function or memory subdecode only
     * when BSEL is not selecting a constant and JCN is not a long
     * goto/call. */
    if (u->bsel >= 4) return 0;
    uint8_t jcn_top4 = (u->jcn >> 4) & 0xF;
    if (((u->jcn >> 7) & 1) == 0 && jcn_top4 == 0) return 0;
    return 1;
}

static int ff_full_function_ok(const dorado_uinstr *u)
{
    if (!ff_decode_ok(u)) return 0;
    /* HM §3.6 / Table 8a: for ASEL 0..3, FF[0:1] selects the
     * memory-reference variant. FF[2:7] may still encode a branch
     * condition, but the whole 8-bit FF value is not a Table 11
     * side-effect function. Initial's BootMem loop depends on
     * FF=0o363 meaning Store + Cnt=0&-1, not Wakeup[3]. */
    if (u->asel <= 3) return 0;
    return 1;
}

/* HM Table 11a (FA=0 FB=0/1): "A[12:15] ← FF[4:7]" override. The
 * low nibble of the A-bus is replaced by FF[4:7] before the value
 * reaches the ALU.
 *
 * Mapping: FF[4:7] in the manual's MSB-first numbering = the low 4
 * bits of FF in C-LSB. Returns -1 if no override applies, else the
 * 4-bit replacement value (0..15).
 *
 * Gated by ff_full_function_ok so that memory-ref ASEL=0..3 forms
 * (where FF[0:1] is an alt-A-source selector, not a function) are
 * not affected. */
static int ff_a_low_override(const dorado_uinstr *u)
{
    if (!ff_decode_ok(u)) return -1;

    /* For memory-reference ASELs, FF[0:1] select the reference kind
     * and do not participate in the FF function decode; the remaining
     * six bits are decoded as functions 0..63. */
    if (u->asel == 0 || u->asel == 1) {
        uint8_t f = u->ff & 077;
        int fb = (f >> 3) & 7;
        if (fb <= 1) return u->ff & 0xF;
        return -1;
    }

    if (!ff_full_function_ok(u)) return -1;
    int fa = ff_fa(u->ff), fb = ff_fb(u->ff);
    if (fa == 0 && fb <= 1) return u->ff & 0xF;
    return -1;
}

static int ff_loads_link(const dorado_uinstr *u)
{
    if (!ff_full_function_ok(u)) return 0;
    int fa = ff_fa(u->ff), fb = ff_fb(u->ff), fc = ff_fc(u->ff);
    if (fa != 1) return 0;
    return (fb == 4 && fc == 7) ||   /* Link ← B */
           (fb == 7 && fc == 6);     /* B ← RWCPReg = Link ← B */
}

/*
 * Many FF values specify an *external* B source (Pipe, Link, CPReg,
 * etc.) that overrides the BSEL primary source. Returns 1 if FF
 * supplied an override and `*b` was set; 0 if BSEL should be used as
 * normal.
 *
 * Per HM Table 11, all B-source overrides live at FA=1 with FB ∈
 * {6, 7}. Non-FA=1 functions never modify the B bus, so the
 * `if (fa != 1) return 0` short-circuit is complete. The
 * BSEL-constant case is caught upstream by `ff_decode_ok`
 * (BSEL >= 4 → FF not a function), and the ASEL ∈ {0..3} memory-ref
 * case is caught by `ff_full_function_ok`.
 *
 * FF=0o077 (FA=0 FB=7 FC=7) is "Reserved as a no-op". The two switch
 * statements below enumerate all 16 (FB=6/7) × (FC=0..7) cases; the
 * `default` arms are unreachable.
 */
static int ff_override_b(dorado_cpu *cpu, const dorado_uinstr *u,
                         uint16_t *b)
{
    if (!ff_full_function_ok(u)) return 0;

    int fa = ff_fa(u->ff), fb = ff_fb(u->ff), fc = ff_fc(u->ff);

    /* Most external-B sources live at FA=1, FB=6 or 7 (HM Table 11b/c). */
    if (fa != 1) return 0;

    if (fb == 6) {
        /* HM Table 11c FA=1 FB=6: B-source from memory subsystem state.
         *   FC=0  B ← FaultInfo'   (B[8:11]=SRN of 1st fault, B[12:15]=count)
         *   FC=1  B ← Pipe0        (= VaHi of most-recent ref)
         *   FC=2  B ← Pipe1        (= VaLo)
         *   FC=3  B ← Pipe2'
         *   FC=4  B ← Pipe3'       (= B+Map' for the entry)
         *   FC=5  B ← Pipe4'       (= Errors' for the entry)
         *   FC=6  B ← Config'
         *   FC=7  B ← Pipe5'
         *
         * Pipe0..5 read consecutive bits of the most recent storage
         * reference. We stub VA-bearing entries with the most-recent
         * VA's high/low halves; others as 0 (no faults / clean
         * config). The active-low ones return ~0 = 0xFFFF for "no
         * faults" semantics. */
        if (!cpu->mem) {
            *b = (fc == 0 || fc == 6) ? 0xFFFF : 0;
            return 1;
        }
        /* HM page 51: "To read a pipe entry, first ProcSRN←B
         * addresses the pipe entry, then the contents of that entry
         * are read with B←Pipei." So Pipe0/1/3'/4'/5 read from the
         * slot pointed to by ProcSRN, not from "the most recent ref."
         * (FaultInfo'/Pipe2' are not slot-addressed — they read the
         * global FaultInfo register.) */
        int      psrn  = (int)cpu->mem->proc_srn;
        uint32_t va    = dorado_pipe_va_at(cpu->mem, psrn);
        uint16_t finfo = dorado_fault_info(cpu->mem);
        switch (fc) {
        /* HM Table 11c FA=1 FB=6 FC=0 / page 51: FaultInfo' is the
         * inverted FaultInfo register (NFaults, SRNFirstFault,
         * EmulatorFault). 0xFFFF = "no faults". */
        case 0: *b = (uint16_t)~finfo;             /* FaultInfo' */
                dorado_fault_clear(cpu->mem);
                break;
        case 1: *b = (uint16_t)((va >> 16) & 0x0FFF); break; /* Pipe0 = VaHi */
        case 2: *b = (uint16_t)(va & 0xFFFF);      break;  /* Pipe1 = VaLo */
        /* Pipe2' is the same data as FaultInfo' (HM page 51:
         * "B←Pipe2' is simply a convenient decode for reading
         * [FaultInfo] back"). */
        case 3: *b = (uint16_t)~finfo;             break;  /* Pipe2' */
        /* Pipe3' / Map' is the inverted snapshot of the old real page
         * number for the selected SRN. NewMemory.mc reads old map
         * flags separately through Errors' / Pipe4'. */
        case 4: *b = (uint16_t)~dorado_pipe_map_rp_at(cpu->mem, psrn); break;
        /* Pipe4' is mixed-polarity (HM page 51, EMemDefs.mc).
         * `dorado_pipe4_at` (gap C2) composes the slot's per-ref
         * error state with the no-error baseline `0o150361`.
         * MapTrouble from a dirty-victim writeback (gap C4) shows
         * up here. */
        case 5: *b = dorado_pipe4_at(cpu->mem, psrn);
                break;                              /* Pipe4' (errors) */
        case 6: *b = (uint16_t)~dorado_memory_config_word(cpu->mem);
                break;                             /* Config' */
        case 7: *b = dorado_pipe5_at(cpu->mem, psrn); break; /* Pipe5 */
        default: *b = 0;                           break;
        }
        return 1;
    }
    if (fb == 7) {
        switch (fc) {
        case 0: /* B ← PCX' (HM Table 11c FA=1 FB=7 FC=0). Active-low.
                 * Returns the inverted IFU instruction-pointer of the
                 * currently-executing opcode (16-bit; bit 0 = byte
                 * selector, bits 1:15 = word offset relative to
                 * BR[31]). Used by emulator microcode that needs to
                 * know the IFU PC for branch arithmetic. */
                *b = (uint16_t)~cpu->ifu_pcx;
                break;
        case 1: /* B ← EventCntA' (HM §4.11). Active-low. */
                *b = (uint16_t)~cpu->event_cnt_a;
                break;
        case 2: /* B ← IFUMRH' (field half, inverted). Address =
                 * InsSet||Opcode (set by InsSetorEvent←B and
                 * BrkIns←B). */
                if (cpu->mc) {
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    *b = (uint16_t)~cpu->mc->ifum_hi[addr];
                } else {
                    *b = 0xFFFF;
                }
                break;
        case 3: /* B ← IFUMLH' (PackedAlpha,,IFaddr' half, inverted). */
                if (cpu->mc) {
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    *b = (uint16_t)~cpu->mc->ifum_lo[addr];
                } else {
                    *b = 0xFFFF;
                }
                break;
        case 4: /* B ← EventCntB' (HM §4.11). Active-low. */
                *b = (uint16_t)~cpu->event_cnt_b;
                break;
        case 5: /* B ← DBuf (HM Table 11c FA=1 FB=7 FC=5).
                 * Returns the data buffer holding the most recent
                 * Store's B value. Used by Midas. */
                *b = cpu->mem ? cpu->mem->dbuf : 0;
                break;
        case 6:                             /* B ← RWCPReg */
            /* HM page 31: "B←RWCPReg = Link←B, B←CPReg'." On real
             * hardware B is the complement of the CPReg latch, but
             * empirically the BB ROM streams data such that the
             * Bootstrap protocol composes correct values when the
             * Dorado-side reads CPReg directly (un-inverted) during
             * normal running. The BB itself appears to apply the
             * inversion at the strobe boundary or pre-invert the data
             * in ROM. The legacy-stub path (no real BB) returns
             * ~cpreg per the manual semantics so synthetic tests can
             * hold a stable CPReg value through wait loops.
             *
             * Empirical: with this asymmetry, Bootstrap composes the
             * correct Loc=0o6100 and decodes the 4-byte-per-instruction
             * stream into IM[0o6100..0o7124]. Reverting to always-
             * invert in BB-running mode triggers Bootstrap to compose
             * Loc with bit 15 set, sending writes into Boot0 region
             * (0o7763..0o7777), corrupting Bootstrap itself. */
            if (cpu->baseboard) {
                uint16_t v = baseboard_dorado_read_cpreg(cpu->baseboard);
                if (cpu->baseboard->dorado_running &&
                    !cpu->baseboard->dorado_ss_pending) {
                    *b = v;
                } else {
                    *b = (uint16_t)~v;
                }
            } else {
                *b = (uint16_t)~cpu->cpreg;
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
 * `alu_carry` is the carry generated by that same ALU evaluation.
 */
static uint16_t ff_apply_post(dorado_cpu *cpu, const dorado_uinstr *u,
                              uint16_t a, uint16_t b, uint16_t alu,
                              uint8_t alu_carry, int *halt)
{
    (void)a;  /* FA=1 FB=2 BrLo/BrHi will use this */
    int fa = ff_fa(u->ff), fb = ff_fb(u->ff), fc = ff_fc(u->ff);
    uint16_t pd = alu;
    *halt = 0;

    /* When BSEL produces a constant (BSEL >= 4), or when FF[0:1] picks
     * an alternate A source for memory references (ASEL=0..3 with
     * specific FF[0:1]), the FF field is *not* interpreted as a
     * function. Per HM §3.9: "FF interpreted as a function iff (BSEL
     * not selecting a constant) and (JCN does not select a 'long'
     * goto/call)." */
    int ff_is_function = ff_full_function_ok(u);
    if (!ff_is_function) {
        if (ff_decode_ok(u) && u->asel <= 3) {
            /* HM Table 8a memory-reference forms use FF[0:1] for the
             * memory source/operation select, but the lower six bits still
             * carry simple Table-11a functions. LoadRam relies on this for
             * `Q_ MD` (FF=0o373) and `BDispatch_ Q` (FF=0o371) while
             * issuing Fetches. Keep this narrow so memory-ref branch
             * encodings such as Initial's BootMem loop are not misdecoded as
             * full 8-bit FF side effects. */
            switch (u->ff & 077) {
            case 070: /* BigBDispatch ← B */
                cpu->dispatch_pending = b & 0xFF;
                break;
            case 071: /* BDispatch ← B */
                cpu->dispatch_pending = b & 0x7;
                break;
            case 073: /* Q ← B */
                cpu->Q = b;
                break;
            case 075: /* TgetsMd */
                return task_md(cpu);
            default:
                break;
            }
        }
        return pd;
    }

    if (fa == 0) {
        if (fb <= 1) {
            /* A[12:15] ← FF[4:7] — applied at A-bus time by
             * ff_a_low_override(); no post-ALU side effect. */
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
            case 1: /* ReadMap (HM page 41 / Table 11a FA=0 FB=3 FC=1).
                     * Delivers the Map[Mar] entry on Pd. The previous
                     * memory reference's VA is in mem->mar. The 16-bit
                     * encoding (HM Table 16):
                     *   bit 0 (MSB)  : WP
                     *   bit 1        : Dirty
                     *   bit 2        : Ref
                     *   bits 3..15   : RP[0:12] (real page number)
                     * In C-LSB ordering with manual MSB at bit 15:
                     *   bit 15 = WP, bit 14 = Dirty, bit 13 = Ref,
                     *   bits 12..0 = RP[0..12]. */
                if (cpu->mem) {
                    uint32_t idx = dorado_map_index(cpu->mem->mar);
                    const dorado_map_entry *e = &cpu->mem->map[idx];
                    uint16_t v = 0;
                    v |= (uint16_t)((e->wp    & 1) << 15);
                    v |= (uint16_t)((e->dirty & 1) << 14);
                    v |= (uint16_t)((e->ref   & 1) << 13);
                    v |= (uint16_t)(e->rp & 0x1FFF);
                    return v;
                }
                return pd;
            case 2: /* Pd ← Input (HM §7 p. 86): reads IOB data with
                     * parity check. Dispatches through the slow-IO
                     * routing table indexed by (ctask, TIOA). With no
                     * io table or no device mapped, returns 0xFFFF —
                     * the floating-bus default (real hardware: IOB
                     * pulls high when no device asserts) — and sets
                     * io_bad_parity so a parity-fault handler (TBD)
                     * can react. */
                {
                    int bad = 0;
                    uint16_t v = cpu->io
                        ? dorado_io_read(cpu->io, cpu->ctask,
                                         (uint8_t)cpu->TIOA, &bad)
                        : (uint16_t)0xFFFF;
                    if (!cpu->io) bad = 1;
                    cpu->io_bad_parity = (uint8_t)bad;
                    return v;
                }
            case 3: /* Pd ← InputNoPE — same data path, no parity check.
                     * Microcode uses this to probe device presence:
                     * absent device → IOB floats high → InputNoPE
                     * delivers 0xFFFF without trapping. */
                {
                    int bad = 0;
                    uint16_t v = cpu->io
                        ? dorado_io_read(cpu->io, cpu->ctask,
                                         (uint8_t)cpu->TIOA, &bad)
                        : (uint16_t)0xFFFF;
                    (void)bad;
                    return v;
                }
            case 4: /* RIsId — Id replaces RM/STK in A←RM/STK,
                     * B←RM/STK, and shifter (HM Table 11a FA=0 FB=3
                     * FC=4). For our minimal model: just consume one
                     * ←Id to advance the operand cursor. The actual
                     * substitution into RM/STK reads happens elsewhere
                     * — Phase C.4 polish. */
                (void)ifu_consume_id(cpu);
                return pd;
            case 5: /* TIsId — Id replaces T in A←T, B←T, and shifter
                     * (FC=5). Same caveat as RIsId. */
                (void)ifu_consume_id(cpu);
                return pd;
            case 6: /* Output ← B (HM §7 p. 86): drive 16 bits + parity
                     * onto IOB to the device addressed by TIOA. Routed
                     * through the slow-IO table; no-op if unmapped. */
                if (cpu->io) {
                    dorado_io_write(cpu->io, cpu->ctask,
                                    (uint8_t)cpu->TIOA, b);
                }
                return pd;
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
            case 0: /* BigBDispatch ← B */
                cpu->dispatch_pending = b & 0xFF;
                return pd;
            case 1: /* BDispatch ← B */
                cpu->dispatch_pending = b & 0x7;
                return pd;
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
            case 0: /* PCF ← B (HM Table 11c FA=1 FB=0 FC=0).
                     * Loads PCF and starts the IFU pipeline. PCF is
                     * a 16-bit value: bits 0:14 (MSB) = word
                     * displacement relative to BR[31], bit 15 (MSB) =
                     * byte selector. In C-LSB: byte sel = bit 0,
                     * word offset = (b >> 1) & 0x7FFF.
                     *
                     * The pipeline needs ~5 cycles to fill before
                     * the first IFUJump can succeed (HM page 67).
                     * Until then, IFUJump traps to *34-37 NotReady. */
                cpu->ifu_pcf     = b;
                cpu->ifu_pcx     = b;     /* until first IFUJump */
                cpu->ifu_idcnt   = 0;
                cpu->ifu_active  = 1;
                cpu->ifu_warmup  = 5;     /* HM page 67 */
                return pd;
            case 1: /* IFUTest ← B */
                junk_timer_ifutest_control(cpu, b);
                return pd;
            case 2: /* IFUTick */                  return pd;
            case 3: /* RescheduleNow (HM Table 20). Trap the next
                     * successful IFUJump (so long as it appears in
                     * the second cycle after RescheduleNow or later).
                     * The Reschedule branch condition is NOT affected. */
                cpu->reschedule_pending = 1;
                return pd;
            case 4: /* AckJunkTW ← B */
                junk_timer_ack_control(cpu, b);
                return pd;
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
            case 2: /* CFlags ← A' */
                if (cpu->mem) dorado_cflags_load(cpu->mem, a);
                return pd;
            case 3: /* BrLo ← A — BR[MemBase][16:31] ← A[0:15]
                     * (HM Table 11c FA=1 FB=2 FC=3). */
                if (cpu->mem) dorado_br_lo_load(cpu->mem, cpu->MemBase, a);
                return pd;
            case 4: /* BrHi ← A — BR[MemBase][4:15] ← A[4:15]
                     * (HM Table 11c FA=1 FB=2 FC=4). */
                if (cpu->mem) dorado_br_hi_load(cpu->mem, cpu->MemBase, a);
                return pd;
            case 5: /* LoadTestSyndrome */         return pd;
            case 6: /* LoadMcr[A,B] */
                if (cpu->mem) dorado_mcr_load(cpu->mem, a, b);
                return pd;
            case 7: /* ProcSRN ← B[12:15] (HM Table 11c FA=1 FB=2 FC=7).
                     * B[12:15] in MSB-first numbering = low 4 bits in
                     * C-LSB. */
                if (cpu->mem) dorado_proc_srn_set(cpu->mem, (uint8_t)(b & 0xF));
                return pd;
            }
        }
        if (fb == 3) {
            switch (fc) {
            case 0: /* InsSetorEvent ← B (HM Table 11c FA=1 FB=3 FC=0).
                     * If B[0]=1: B[6:7] are loaded into IFU's
                     * InsSet[0:1]. If B[0]=0: B[4:15] are
                     * EventCntA/EventCntB controls (HM §4.11). We
                     * record the low 12 bits in two halves so
                     * microcode round-tripping the controls observes
                     * the value it wrote (gap B11 stub). */
                if ((b >> 15) & 1) {
                    cpu->ifu_insset = (uint8_t)((b >> 8) & 3);
                } else {
                    cpu->event_cnt_ctrl_hi = (uint8_t)((b >> 8) & 0x0F);
                    cpu->event_cnt_ctrl_lo = (uint8_t)(b & 0xFF);
                }
                return pd;
            case 1: /* EventCntB ← B (HM §4.11). */
                cpu->event_cnt_b = b;
                return pd;
            case 2: /* Reschedule (HM Table 20). Cause a reschedule
                     * trap on the second OR third successful IFUJump. */
                cpu->reschedule_pending = 2;
                return pd;
            case 3: /* NoReschedule. Clear the pending Reschedule. */
                cpu->reschedule_pending = 0;
                return pd;
            case 4: /* IFUMRH ← B (field half of IFUM entry).
                     * Writes 16 bits: Sign←B.0, PE←B[1:3], Length'←B[4:5],
                     * RBaseB'←B.6, MemB←B[7:9], TPause'←B.10,
                     * TJump'←B.11, N←B[12:15]. */
                if (cpu->mc) {
                    dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    mc_w->ifum_hi[addr] = b;
                    mc_w->ifum_present[addr] = 1;
                }
                return pd;
            case 5: /* IFUMLH ← B (PackedAlpha,,IFaddr' half).
                     * Writes 16 bits: Packed-α←B.5 and IFaddr'←B[6:15].
                     * LoadRam.mc writes EB word0 through IFUMLH and word1
                     * through IFUMRH; the in-memory form keeps those as
                     * ifum_lo and ifum_hi respectively because dispatch
                     * decodes IFaddr from lo and fields from hi. */
                if (cpu->mc) {
                    dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    mc_w->ifum_lo[addr] = b;
                    mc_w->ifum_present[addr] = 1;
                }
                return pd;
            case 6: /* IFUReset — clear IFU pipeline state and load the
                     * IFU test-control register with 1 (HM §8.3), which
                     * disables junk wakeups in the normal non-test state. */
                cpu->ifu_insset = 0;
                cpu->ifu_opcode = 0;
                junk_timer_ifutest_control(cpu, DORADO_B15_MASK);
                return pd;
            case 7: /* BrkIns ← B (HM §4.10). Opcode ← B[0:7] and set
                     * BrkPending; the next IFU dispatch will trap to
                     * the breakpoint vector. We record the state slot
                     * (gap B11) so microcode round-trips it; we do
                     * not yet trigger the trap. */
                cpu->ifu_opcode = (uint8_t)((b >> 8) & 0xFF);
                cpu->brk_opcode = cpu->ifu_opcode;
                cpu->brk_pending = 1;
                return pd;
            }
        }
        if (fb == 4) {
            switch (fc) {
            case 0: /* UseDMD */                   return pd;
            case 1: /* MidasStrobe ← B */          return pd;
            case 2: /* TaskingOff (HM page 27) — atomic; effective
                     * immediately. Subsequent instructions of the
                     * same task run without task switches. */
                cpu->tasking_on = 0;
                cpu->tasking_resume_delay = 0;
                return pd;
            case 3: /* TaskingOn — not immediately effective; "at
                     * least two more instructions will be executed
                     * by the same task before task switching can
                     * occur" (HM page 27). */
                cpu->tasking_resume_delay = 2;
                return pd;
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
            /* TIOA[5:7] ← FF[5:7] (TIOA[0:4] unchanged).
             * All three low selector bits are significant; disk uses
             * 010..014 here, including DiskTag at low value 4. */
            cpu->TIOA = (cpu->TIOA & 0xF8) | (u->ff & 7);
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
            case 3: /* Pd ← ALUFMEM */
                /* Read current value to Pd. FC=2 also writes a new
                 * value from B; FC=3 is read-only. This distinction is
                 * visible in Mesa's SETDLP path, which reads ALUFM[0]
                 * with FF=0o263 and expects the table to survive.
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
                    /* HM Table 11d: "ALUFMEM ← B.8, B[11:15]". Manual
                     * bit numbering MSB-first. Decoded:
                     *   ALUFM manual bit 0 (carry, MSB)  ← B[8]  manual = B_C[7]
                     *   ALUFM manual bit 1               ← B[11] manual = B_C[4]
                     *   ALUFM manual bit 2               ← B[12] manual = B_C[3]
                     *   ALUFM manual bit 3               ← B[13] manual = B_C[2]
                     *   ALUFM manual bit 4               ← B[14] manual = B_C[1]
                     *   ALUFM manual bit 5 (LSB of op)   ← B[15] manual = B_C[0]
                     *
                     * Our `alu_op` consumes the 6-bit entry as
                     * (carry << 5) | op[4:0] in C-LSB. So:
                     *   entry C-bit 5 = ALUFM manual bit 0 = B_C[7]
                     *   entry C-bit 4 = ALUFM manual bit 1 = B_C[4]
                     *   entry C-bit 3 = ALUFM manual bit 2 = B_C[3]
                     *   entry C-bit 2 = ALUFM manual bit 3 = B_C[2]
                     *   entry C-bit 1 = ALUFM manual bit 4 = B_C[1]
                     *   entry C-bit 0 = ALUFM manual bit 5 = B_C[0]
                     *
                     * Verified against Bootstrap.MB at IM[0o7771] which
                     * does Pd←ALUFMRW with ALUF=10 (=8 dec) and B=T=0x13.
                     * Bootstrap.MB pre-declares ALUFM[8] = 0o23 (A XOR
                     * B); the runtime write should reproduce that.
                     * 0x13 = 0b 0001 0011: B_C[7]=0, B_C[4]=1, B_C[3]=0,
                     * B_C[2]=0, B_C[1]=1, B_C[0]=1 → entry =
                     * 0_1_0_0_1_1 = 0o23 ✓.
                     */
                    if (fc == 2) {
                        uint8_t alufm =
                            (uint8_t)((((b >> 7) & 1) << 5) |
                                      (((b >> 4) & 1) << 4) |
                                      (((b >> 3) & 1) << 3) |
                                      (((b >> 2) & 1) << 2) |
                                      (((b >> 1) & 1) << 1) |
                                      (((b >> 0) & 1) << 0));
                        mc_w->alufm[idx] = alufm;
                        mc_w->alufm_present[idx] = 1;
                    }
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
            /* HM Table 11d: one-bit shifts/cycles of the current ALU
             * output. Manual bit 0 is the sign/MSB, so these map to
             * normal C right/left shifts over the uint16_t value. */
            switch (fc) {
            case 0: /* Pd ← ALU rsh 1, Pd[0] ← 0 */
                return (uint16_t)(alu >> 1);
            case 1: /* Pd ← ALU rcy 1, Pd[0] ← ALU[15] */
                return (uint16_t)((alu >> 1) | (alu << 15));
            case 2: /* Pd ← ALU brsh 1, Pd[0] ← ALUcarry */
                return (uint16_t)((alu >> 1) |
                                  ((uint16_t)(alu_carry & 1) << 15));
            case 3: /* Pd ← ALU arsh 1, preserve sign */
                return (uint16_t)((alu >> 1) | (alu & 0x8000u));
            case 4: /* Pd ← ALU lsh 1 */
                return (uint16_t)(alu << 1);
            case 5: /* Pd ← ALU lcy 1 */
                return (uint16_t)((alu << 1) | (alu >> 15));
            case 6: /* Divide — partial placeholder until needed. */
            case 7: /* CDivide — partial placeholder until needed. */
                return pd;
            }
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
            /* Wakeup[n] — initiate wakeup request for task FF[4:7]
             * (HM Table 11e). FF[4:7] in MSB-first numbering = low 4
             * bits of FF in C-LSB. */
            int task = u->ff & 0xF;
            cpu->wakeup_pending |= (uint16_t)(1u << task);
            return pd;
        }
    }

    return pd;
}

static uint16_t task_md(const dorado_cpu *cpu)
{
    int task = cpu->ctask & 0xF;
    if (cpu->task_md_valid[task]) {
        return cpu->task_md[task];
    }
    return cpu->mem ? cpu->mem->md : 0;
}

static int ref_kind_loads_md(dorado_ref_kind kind)
{
    return kind == DM_REF_FETCH ||
           kind == DM_REF_IFETCH ||
           kind == DM_REF_LONGFETCH;
}

static const char *ref_kind_name(dorado_ref_kind kind)
{
    switch (kind) {
    case DM_REF_NONE:      return "none";
    case DM_REF_FETCH:     return "fetch";
    case DM_REF_STORE:     return "store";
    case DM_REF_IFETCH:    return "ifetch";
    case DM_REF_PREFETCH:  return "prefetch";
    case DM_REF_MAP:       return "map";
    case DM_REF_DUMMYREF:  return "dummyref";
    case DM_REF_FLUSH:     return "flush";
    case DM_REF_LONGFETCH: return "longfetch";
    case DM_REF_IOFETCH:   return "iofetch";
    case DM_REF_IOSTORE:   return "iostore";
    default:               return "?";
    }
}

static int eth_mem_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_ETH_MEM_TRACE") ? 1 : 0;
    return cached;
}

static int lowcore_trace_mode(void)
{
    static int cached = -1;
    if (cached < 0) {
        const char *env = getenv("DORADO_LOWCORE_TRACE");
        cached = 0;
        if (env && env[0] && env[0] != '0') {
            cached = (strcmp(env, "all") == 0) ? 2 : 1;
        }
    }
    return cached;
}

static int lowcore_trace_offset(uint32_t va, uint32_t base,
                                uint16_t *out_off)
{
    uint32_t off = (va - base) & 0x0FFFFFFFu;
    if ((off >= 0420u && off <= 0440u) ||
        (off >= 0521u && off <= 0523u)) {
        if (out_off) *out_off = (uint16_t)off;
        return 1;
    }
    return 0;
}

static int csb_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_CSB_TRACE") ? 1 : 0;
    return cached;
}

static int csb_trace_offset(uint32_t va, uint32_t iobr,
                            uint16_t *out_off)
{
    uint32_t off = (va - iobr) & 0x0FFFFFFFu;
    if (off >= 0177520u && off <= 0177523u) {
        if (out_off) *out_off = (uint16_t)off;
        return 1;
    }
    return 0;
}

static int rm_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_RM_TRACE") ? 1 : 0;
    return cached;
}

static void latch_task_md_from_memory(dorado_cpu *cpu)
{
    if (cpu->mem) {
        int task = cpu->ctask & 0xF;
        cpu->task_md[task] = cpu->mem->md;
        cpu->task_md_valid[task] = 1;
    }
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
    case 0: /* Md (memory data). HM page 39/40: Md is task-specific
             * and remains valid until the next fetch by this task. */
        *out = task_md(cpu);
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
        if (u->bsel == 4 && u->aluf == 4 && u->ff == 1) {
            /* MicroD emits this compact FF-controlled form for
             * LDF[T,3,10] in Bootstrap. It extracts the 3-bit
             * BaseBoard dispatch from CPReg high-byte bits 5..7.
             * This .MB form feeds BigBDispatch directly, so return
             * the already-spread even target offset. */
            return (uint16_t)(((t >> 8) & 0x7) << 1);
        }
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

    uint16_t fill = with_md ? task_md(cpu) : 0;
    return (uint16_t)((lo16 & ~mask) | (fill & mask));
}

/*
 * Decode (ASEL, FF[0:1]) into a memory reference kind per HM Table 8a.
 * ASEL 0..3 are memory ops; the table picks the kind based on
 * ASEL × FF[0:1]:
 *
 *   ASEL  FF[0:1]  Kind
 *   0     0        PreFetch←RM/STK
 *   0     1        Map←RM/STK   -or- IOFetch←RM (io task)
 *   0     2        LongFetch←RM/STK
 *   0     3        Store←RM/STK
 *   1     0        DummyRef←RM/STK
 *   1     1        Flush←RM/STK -or- IOStore←RM (io task)
 *   1     2        IFetch←RM/STK
 *   1     3        Fetch←RM/STK
 *   2     0..3     Store←{Md, Id, Q, T}
 *   3     0..3     Fetch←{Md, Id, Q, T}
 *
 * For ASEL 0/1, the address (Mar) comes from RM/STK[rstk_a].
 * For ASEL 2/3, the selected alternate source drives A and Mar.
 *
 * io-task variants (Map↔IOFetch, Flush↔IOStore) are switched by the
 * task ID. Without tasking, we always pick the emulator/fault variant
 * (Map / Flush). */
static dorado_ref_kind decode_ref_kind(const dorado_uinstr *u, int io_task)
{
    int ff_ok = ff_decode_ok(u);
    int ff01 = ff_ok ? ((u->ff >> 6) & 3) : 0;  /* FF[0:1] if Table 8a */
    switch (u->asel) {
    case 0:
        if (!ff_ok) return DM_REF_STORE;         /* HM Table 8b */
        switch (ff01) {
        case 0: return DM_REF_PREFETCH;
        case 1: return io_task ? DM_REF_IOFETCH : DM_REF_MAP;
        case 2: return DM_REF_LONGFETCH;
        case 3: return DM_REF_STORE;
        }
        break;
    case 1:
        if (!ff_ok) return DM_REF_FETCH;         /* HM Table 8b */
        switch (ff01) {
        case 0: return DM_REF_DUMMYREF;
        case 1: return io_task ? DM_REF_IOSTORE : DM_REF_FLUSH;
        case 2: return DM_REF_IFETCH;
        case 3: return DM_REF_FETCH;
        }
        break;
    case 2:
        (void)ff01;
        (void)io_task;
        return DM_REF_STORE;
    case 3:
        (void)ff01;
        (void)io_task;
        return DM_REF_FETCH;
    }
    return DM_REF_NONE;
}

static uint16_t alt_mem_source(dorado_cpu *cpu, const dorado_uinstr *u)
{
    if (!ff_decode_ok(u)) return cpu->T;  /* HM Table 8b: Store/Fetch<-T */
    int ff01 = (u->ff >> 6) & 3;     /* FF[0:1]: Md, Id, Q, T */
    switch (ff01) {
    case 0: return task_md(cpu);
    case 1: return ifu_consume_id(cpu);
    case 2: return cpu->Q;
    case 3: return cpu->T;
    }
    return 0;
}

/* Compute A bus value (HM Table 8a/8b). */
static int a_bus(dorado_cpu *cpu, const dorado_uinstr *u, uint16_t *out)
{
    int rm_a = rm_address(cpu, u);
    switch (u->asel) {
    case 0: case 1:
        /* Memory references with RM/STK as the Mar source. A = RM/STK
         * (so it can also drive Mar on the backplane — HM page 36).
         * For BLOCK=1 in tasks 0..14 (non-emulator), the MIR's BLOCK
         * bit means "block io task unless wakeup is waiting" — not a
         * STK access; rm_a is then RM-style. */
        if (rm_a >= CPU_RMSTK_INVALID) return CPU_HALT_UNSUPPORTED_ASEL;
        *out = rm_stk_read(cpu, rm_a);
        return 0;
    case 2: case 3:
        /* Store/Fetch from alternate A sources. The selected source
         * drives both the ALU A input and Mar (HM Table 8a/8b). */
        *out = alt_mem_source(cpu, u);
        return 0;
    case 4: /* A←RM/STK */
        if (rm_a >= CPU_RMSTK_INVALID) return CPU_HALT_UNSUPPORTED_ASEL;
        *out = rm_stk_read(cpu, rm_a);
        return 0;
    case 5: /* A ← Id (IFU operand byte). Consumes one ←Id from
             * the pipeline. */
        *out = ifu_consume_id(cpu);
        return 0;
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
/* Returns 1 if the ALU op `op` is arithmetic (updates Carry'/Overflow
 * branch conditions), 0 if logical (Carry'/Overflow preserved from the
 * last arithmetic op per HM page 30). The 16 arithmetic op codes are
 * the even ones in HM Table 9. */
static int alu_op_is_arithmetic(uint8_t op)
{
    /* HM Table 9: even op codes (low bit = 0) are arithmetic; odd ones
     * (low bit = 1) are logical. */
    return (op & 1) == 0;
}

static uint16_t alu_op(uint8_t alufm_entry, uint16_t a, uint16_t b,
                       uint8_t *out_carry, uint8_t *out_overflow,
                       uint8_t *out_is_arith)
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
    if (out_is_arith) *out_is_arith = (uint8_t)alu_op_is_arithmetic(op);
    return (uint16_t)result;
}

/* Apply LC: deliver Pd to T and/or RM/STK (HM Table 10). Md loads see
 * the Md latch as it was at instruction issue; a concurrent Fetch updates
 * Md for later instructions. */
static int apply_lc(dorado_cpu *cpu, const dorado_uinstr *u, uint16_t pd,
                    uint16_t md_at_issue)
{
    /* Use the *write* address for STK accesses — that may differ from
     * the read address when ModStkPBeforeW is in effect (HM page 11). */
    int rm_a = lc_write_address(cpu, u);
    int has_rm = (rm_a < CPU_RMSTK_INVALID);
    switch (u->lc) {
    case 0: /* No Action */
        break;
    case 1: /* T←Pd */
        cpu->T = pd;
        break;
    case 2: /* T←Md, RM/STK←Pd */
    {
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        if (rm_trace_enabled() && rm_a >= 0 && rm_a < 0x100) {
            fprintf(stderr,
                    "RM_WRITE task=%o pc=0o%o addr=%03o old=%06o new=%06o "
                    "via=LC2 pd=%06o md=%06o rb=%02o rstk=%02o\n",
                    cpu->ctask & 017, cpu->real_PC, rm_a & 0377,
                    cpu->RM[rm_a] & 0177777, pd & 0177777,
                    pd & 0177777, md_at_issue & 0177777,
                    cpu->RBase & 017, u->rstk & 017);
        }
        rm_stk_write(cpu, rm_a, pd);
        cpu->T = md_at_issue;
        break;
    }
    case 3: /* T←Md */
        cpu->T = md_at_issue;
        break;
    case 4: /* RM/STK←Md */
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        if (rm_trace_enabled() && rm_a >= 0 && rm_a < 0x100) {
            fprintf(stderr,
                    "RM_WRITE task=%o pc=0o%o addr=%03o old=%06o new=%06o "
                    "via=LC4 pd=%06o md=%06o rb=%02o rstk=%02o\n",
                    cpu->ctask & 017, cpu->real_PC, rm_a & 0377,
                    cpu->RM[rm_a] & 0177777, md_at_issue & 0177777,
                    pd & 0177777, md_at_issue & 0177777,
                    cpu->RBase & 017, u->rstk & 017);
        }
        rm_stk_write(cpu, rm_a, md_at_issue);
        break;
    case 5: /* T←Pd, RM/STK←Md */
        cpu->T = pd;
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        if (rm_trace_enabled() && rm_a >= 0 && rm_a < 0x100) {
            fprintf(stderr,
                    "RM_WRITE task=%o pc=0o%o addr=%03o old=%06o new=%06o "
                    "via=LC5 pd=%06o md=%06o rb=%02o rstk=%02o\n",
                    cpu->ctask & 017, cpu->real_PC, rm_a & 0377,
                    cpu->RM[rm_a] & 0177777, md_at_issue & 0177777,
                    pd & 0177777, md_at_issue & 0177777,
                    cpu->RBase & 017, u->rstk & 017);
        }
        rm_stk_write(cpu, rm_a, md_at_issue);
        break;
    case 6: /* RM/STK←Pd */
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        if (rm_trace_enabled() && rm_a >= 0 && rm_a < 0x100) {
            fprintf(stderr,
                    "RM_WRITE task=%o pc=0o%o addr=%03o old=%06o new=%06o "
                    "via=LC6 pd=%06o md=%06o rb=%02o rstk=%02o\n",
                    cpu->ctask & 017, cpu->real_PC, rm_a & 0377,
                    cpu->RM[rm_a] & 0177777, pd & 0177777,
                    pd & 0177777, md_at_issue & 0177777,
                    cpu->RBase & 017, u->rstk & 017);
        }
        rm_stk_write(cpu, rm_a, pd);
        break;
    case 7: /* T←Pd, RM/STK←Pd */
        cpu->T = pd;
        if (!has_rm) return CPU_HALT_UNSUPPORTED_ASEL;
        if (rm_trace_enabled() && rm_a >= 0 && rm_a < 0x100) {
            fprintf(stderr,
                    "RM_WRITE task=%o pc=0o%o addr=%03o old=%06o new=%06o "
                    "via=LC7 pd=%06o md=%06o rb=%02o rstk=%02o\n",
                    cpu->ctask & 017, cpu->real_PC, rm_a & 0377,
                    cpu->RM[rm_a] & 0177777, pd & 0177777,
                    pd & 0177777, md_at_issue & 0177777,
                    cpu->RBase & 017, u->rstk & 017);
        }
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
    case 6:
        /* IOAtten' (non-emulator) / Reschedule (emulator).
         * HM page 30: "tests the task-specific IOAttention signal
         * optionally generated by the io device associated with the
         * current (non-emulator) task." Active-low: r=1 means
         * "no I/O attention pending."
         *
         * For the emulator (task 0), this condition is "Reschedule"
         * — set true when a Reschedule FF function fired in a recent
         * instruction. We don't yet model device-driven I/O attention,
         * so check the task's own pending wakeup as a proxy: if the
         * current task has a pending wakeup not yet acknowledged, it
         * counts as "I/O attention pending" (r=0 after inversion).
         *
         * For now: emulator always returns 1 (no reschedule); other
         * tasks check their wakeup_pending bit. */
        if (cpu->ctask == 0) {
            /* Emulator: Reschedule branch condition. True (= r=0
             * after inversion to active-low) if the Reschedule
             * flipflop is set. HM Table 20: "Also set the
             * Reschedule branch condition (emulator only) to true."
             * Per HM, RescheduleNow does NOT affect this branch
             * condition, only the trap. So we report true only if
             * pending was set by Reschedule (count=2 at issue). */
            r = (cpu->reschedule_pending >= 2) ? 0 : 1;
        } else {
            int io_attention = cpu->io
                ? dorado_io_attention(cpu->io, cpu->ctask,
                                      (uint8_t)cpu->TIOA)
                : 0;
            r = io_attention ? 0 : 1;
        }
        break;
    default: r = 0;                                       break;
    }
    return r;
}

/* Fetch a single byte from the IFU code stream at the given pc
 * (16-bit; bit 0 = byte selector, bits 1:15 = word offset relative
 * to BR[31]). Uses the memory subsystem to read the word, then
 * extracts the byte per the instruction-set kludge (HM page 64:
 * sets 0/1 use byte 0 = bits 0:7 (high half), sets 2/3 reverse).
 *
 * Returns the byte. If memory isn't wired, returns 0xFF. Caller is
 * responsible for handling map faults via the standard fault path —
 * this helper just delivers data. */
static uint8_t ifu_fetch_byte(dorado_cpu *cpu, uint16_t pc, int *out_faulted)
{
    if (out_faulted) *out_faulted = 0;
    if (!cpu->mem) return 0xFF;
    uint16_t word_off    = (uint16_t)(pc >> 1);
    uint8_t  byte_sel    = (uint8_t)(pc & 1);
    uint32_t br31        = dorado_br_get(cpu->mem, 31);
    uint32_t va          = (br31 + word_off) & 0x0FFFFFFFu;

    /* IFU fetches use a separate cache port in real hardware; here
     * we issue a normal Fetch via the cache + Map. The result lands
     * in mem->md. We use IFETCH so the pipe entry kind is correct.
     * If the page is Vacant or WP-protected (= IFU map fault on real
     * HW), we report it back to the caller; the IFU must trap to
     * *0-3 (HM Table 14, page 33). */
    dorado_fault_kind f = dorado_memory_ref(cpu->mem, DM_REF_IFETCH,
                                            va, 0, 0);
    if (f != DM_FAULT_NONE && out_faulted) *out_faulted = 1;
    uint16_t word = cpu->mem->md;
    /* Sets 0/1 (the only ones we model so far): byte 0 = high byte. */
    int high_byte = (cpu->ifu_insset < 2) ? (byte_sel == 0) : (byte_sel == 1);
    return high_byte ? (uint8_t)((word >> 8) & 0xFF) : (uint8_t)(word & 0xFF);
}

/* Decode an IFUM entry's high half (Table 18). Sets the relevant
 * fields on cpu->ifu_*. The 16-bit raw word is laid out per HM
 * Table 20 (IFUMLH←B description):
 *   bit 0  = Sign
 *   bits 1..3 = IPar (parity bits, ignored in our model)
 *   bits 4..5 = Length' (low-true; opcode length is ~Length' + 1)
 *   bit 6  = RBaseB' (low-true)
 *   bits 7..9 = MemB
 *   bit 10 = TPause' (low-true)
 *   bit 11 = TJump' (low-true)
 *   bits 12..15 = N
 * "MSB-first 0..15"; in C-LSB the MSB is bit 15 of the uint16_t. */
static void ifu_decode_lh(dorado_cpu *cpu, uint16_t lh)
{
    /* C-LSB reverses bit order: LSB-bit n = MSB-bit (15-n). */
    cpu->ifu_sign       = (lh >> 15) & 1;
    /* Length' is low-true: 00→1 byte, 01→2, 10→3, 11→illegal. */
    uint8_t lpr = (uint8_t)((lh >> 10) & 3);
    cpu->ifu_length     = (uint8_t)(lpr + 1);
    cpu->ifu_type_pause = !((lh >> 5) & 1);   /* TPause' low-true */
    cpu->ifu_type_jump  = !((lh >> 4) & 1);   /* TJump' low-true */
    cpu->ifu_n          = (uint8_t)(lh & 0xF);
    /* MemB[0:2] in MSB-first = bits 7..9. In C-LSB: bits 8..6. */
    uint8_t memb = (uint8_t)((lh >> 6) & 7);
    /* HM page 65: at t0 of the entry instruction,
     *   if MemB[0]==0: MemBase ← 0..MemBX[0:1]..MemB[1:2]
     *   if MemB[0]==1: MemBase ← 34₈ + MemB[1:2]. */
    uint8_t memb0 = (memb >> 2) & 1;
    uint8_t memb12 = memb & 3;
    if (memb0 == 0) {
        cpu->MemBase = (uint8_t)(((cpu->MemBX & 3) << 2) | memb12);
    } else {
        cpu->MemBase = (uint8_t)(034 + memb12);
    }
    /* RBaseB' is low-true; RBase ← RBaseB. */
    cpu->RBase = !((lh >> 9) & 1);
}

/* Decode an IFUM entry's low half (Table 20):
 *   bit 5 = Packed-α
 *   bits 6:15 = IFaddr' (10 bits — the 10-bit entry-vector base)
 * Returns IFaddr'. Sets ifu_packed_a. */
static uint16_t ifu_decode_rh(dorado_cpu *cpu, uint16_t rh)
{
    cpu->ifu_packed_a = (rh >> 10) & 1;
    /* IFaddr' bits 6:15 (MSB) = bits 9..0 (LSB) = rh & 0x3FF. */
    return (uint16_t)(rh & 0x3FF);
}

/* Compute an IFU trap address. HM Table 14 footnote: "Ifu traps
 * OR the 1's complement of the instruction set into bits 8:9 of
 * the trap address". In MSB-first 16-bit numbering, bits 8:9 of
 * a 16-bit microstore address. For our 12-bit microstore (4K), the
 * relevant bits are at LSB positions 6:7 (verified by the manual's
 * own example — Reschedule trap actual addresses 14, 114, 214, 314).
 *
 * trap_base: low 5 bits = base address (e.g., 0o34 for NotReady,
 *            0o14 for Reschedule, 0o74 for IFUM parity, 0o0 for IFU
 *            map fault, 0o4 for IFU data parity).
 * n_slot:    JCN[3:4] from the IFUJump instruction (0..3) — selects
 *            which entry of the 4-long trap vector to dispatch to.
 * insset:    current InsSet (0..3) — its 1's complement is OR'd
 *            into bits 6:7. */
static uint16_t ifu_trap_addr(int trap_base, int n_slot, int insset)
{
    uint16_t base = (uint16_t)((trap_base & 0x3C) | (n_slot & 3));
    uint16_t isn  = (uint16_t)(((~insset) & 3) << 6);
    return (uint16_t)(base | isn);
}

/* Deliver the next operand byte for ←Id (HM Table 19, regular &
 * pause opcodes). Increments ifu_idcnt. After all operand bytes are
 * consumed, ←Id delivers Length forever (used by jump-fallthrough
 * calculations). For jump opcodes, we don't yet model the sequence
 * (it's normally consumed inside IFUJump). */
static uint16_t ifu_consume_id(dorado_cpu *cpu)
{
    uint8_t cnt   = cpu->ifu_idcnt++;
    uint8_t len   = cpu->ifu_length;
    uint8_t n_supplied = (cpu->ifu_n != 017);

    /* Operand sequence: N (if supplied), then α (or α[0:3] / α[4:7]
     * when Packed-α=1), then β (length=3 with Packeda=0), then
     * Length forever. */
    int op_idx = (int)cnt;
    if (n_supplied) {
        if (op_idx == 0) return (uint16_t)cpu->ifu_n;
        op_idx--;
    }
    if (len == 2 && cpu->ifu_packed_a) {
        /* α split into nibbles: α[0:3] then α[4:7]. */
        if (op_idx == 0) return (uint16_t)((cpu->ifu_alpha >> 4) & 0xF);
        if (op_idx == 1) return (uint16_t)(cpu->ifu_alpha & 0xF);
    } else if (len >= 2) {
        if (op_idx == 0) {
            /* α; sign-extended to 16 bits if Sign=1. */
            uint16_t a = cpu->ifu_alpha;
            if (cpu->ifu_sign && (a & 0x80)) a |= 0xFF00;
            return a;
        }
        op_idx--;
    }
    if (len == 3 && cpu->ifu_packed_a) {
        /* α[0:3], α[4:7], then β. */
        if (op_idx == 0) return (uint16_t)((cpu->ifu_alpha >> 4) & 0xF);
        if (op_idx == 1) return (uint16_t)(cpu->ifu_alpha & 0xF);
        if (op_idx == 2) return (uint16_t)cpu->ifu_beta;
    } else if (len == 3) {
        if (op_idx == 0) return (uint16_t)cpu->ifu_beta;
        op_idx--;
    }
    /* Out of operands → return Length. */
    return (uint16_t)cpu->ifu_length;
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

    /* HM page 29: an FF-encoded branch condition (FA=0 FB=6) ORs
     * its result into TNIA[15] for ANY JCN encoding except long
     * branch (where FF supplies address bits). Compute it once
     * here so all paths can OR it in. */
    int ff_cond_or = 0;
    {
        int ff_is_available = (u->bsel < 4);
        /* Long branch: JCN top4 = 0 (and top1=0). FF disabled. */
        int is_long = (top1 == 0) && (((jcn >> 4) & 0xF) == 0);
        if (ff_is_available && !is_long) {
            if (u->asel <= 3) {
                int ff_low6 = u->ff & 077;
                if ((ff_low6 & 070) == 060) {
                    ff_cond_or = eval_branch_condition(cpu, u, u->ff & 7);
                }
            } else {
                int fa_c = ff_fa(u->ff), fb_c = ff_fb(u->ff);
                if (fa_c == 0 && fb_c == 6) {
                    ff_cond_or = eval_branch_condition(cpu, u, u->ff & 7);
                }
            }
        }
    }

    if (top1 == 1) {
        /* Local or Global. */
        uint16_t page_addr = jcn & 0x3F;             /* JCN[2:7] */
        if (top2 == 2) {
            /* Local Jump/Call. */
            uint16_t page = cpu->real_PC & ~(uint16_t)(CPU_PAGE_SIZE - 1);
            *next = (uint16_t)((page | page_addr) |
                               ff_cond_or | cpu->dispatch_or);
            if ((page_addr & 0xF) == 0 && !ff_loads_link(u)) { /* TNIA[12:15] = 0 → Call */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
            }
            return 0;
        }
        /* Global Call: low 6 bits forced 0, so TNIA[12:15] always 0. */
        uint16_t quadrant = cpu->real_PC & ~(uint16_t)(CPU_QUADRANT_SIZE - 1);
        *next = (uint16_t)((quadrant | (uint16_t)(page_addr * CPU_PAGE_SIZE))
                           | ff_cond_or | cpu->dispatch_or);
        if (!ff_loads_link(u)) cpu->Link = (uint16_t)(cpu->real_PC + 1);
        return 0;
    }

    /* top1 == 0 — long, conditional, return-class, IFU jump, or undefined. */
    uint8_t low3 = jcn & 7;

    /* LoadRam.mc hand-places its End-item exit as:
     *   LRFlag, Link_ MD, BRGO@[0] RETCL@[2] JCN[45]
     * MicroD uses BRGO/RETCL control bits that are not otherwise exposed
     * in our decoded fields. For the complete-replacement path
     * (LRFlag even), the freshly-loaded Link is the emulator start PC. */
    if (jcn == 0045 && u->ff == 0147) {
        int rm_a = rm_address(cpu, u);
        uint16_t flag = (rm_a < CPU_RMSTK_INVALID) ? rm_stk_read(cpu, rm_a) : 0;
        if ((flag & 1u) == 0) {
            *next = (uint16_t)(cpu->Link & 0x0FFFu);
        } else {
            *next = (uint16_t)((cpu->real_PC & ~(uint16_t)(CPU_PAGE_SIZE - 1))
                               | 0021u);
        }
        return 0;
    }

    if (((jcn >> 4) & 0xF) == 0) {
        /* Long Jump/Call. FF disabled as a function.
         * HM Figure 6: TNIA = CIA[2:3] || FF[0:7] || JCN[4:7].
         * In our 12-bit IM address, FF supplies the high 8 bits and
         * the low JCN nibble supplies the low 4 bits. */
        uint16_t quadrant = cpu->real_PC & ~(uint16_t)(CPU_QUADRANT_SIZE - 1);
        uint16_t addr12   = (uint16_t)(((uint16_t)u->ff << 4) |
                                       (jcn & 0xF));
        *next = (uint16_t)(quadrant | addr12 | cpu->dispatch_or);
        if ((jcn & 0xF) == 0 && !ff_loads_link(u)) {
            cpu->Link = (uint16_t)(cpu->real_PC + 1);
        }
        return 0;
    }

    if (low3 == 7) {
        /* Return / TPC / IM, or IFU Jump. */
        uint8_t bit1 = (jcn >> 6) & 1;     /* JCN[1] */
        uint8_t bits02 = (jcn >> 5) & 7;   /* JCN[0:2] */
        if (bits02 == 1) {
            /* IFU Jump (0 0 1 _ _ 1 1 1). JCN[3:4] = the entry-vector
             * slot n (0..3). */
            int n_slot = (jcn >> 3) & 3;   /* JCN[3:4] */

            /* IFU not active (PCF←B not yet issued, or a pause
             * opcode explicitly halted fetching) → trap to the same
             * NotReady vector used while the pipeline is warming. The
             * emulator sources include retry handlers at these vectors
             * (for example AEmuNotReady), so this is a recoverable
             * microcode path rather than an emulator-level halt. */
            if (!cpu->ifu_active) {
                *next = ifu_trap_addr(0034, n_slot, cpu->ifu_insset);
                if (!ff_loads_link(u)) cpu->Link = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }

            /* IFU still warming up after PCF←B → trap to *34-37
             * NotReady vector. The trap entries themselves should
             * contain IFUJump[n] so they retry until the pipeline
             * fills. Link is loaded with CIA+1 as for any IFUJump. */
            if (cpu->ifu_warmup > 0) {
                *next = ifu_trap_addr(0034, n_slot, cpu->ifu_insset);
                if (!ff_loads_link(u)) cpu->Link = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }

            /* Reschedule trap (HM Table 20). Fires on a successful
             * IFUJump (past NotReady) — even before the IFUM lookup
             * is examined. */
            if (cpu->reschedule_pending > 0) {
                cpu->reschedule_pending--;
                if (cpu->reschedule_pending == 0) {
                    *next = ifu_trap_addr(0014, n_slot, cpu->ifu_insset);
                    if (!ff_loads_link(u)) cpu->Link = (uint16_t)(cpu->real_PC + 1);
                    return 0;
                }
            }

            /* Conditional IFUJump (HM page 33): if an FF-encoded
             * branch condition is true in the same instruction as
             * IFUJump, IFU advance is disabled and the dispatch
             * goes to entry n|1 (TNIA[15] OR'd) of the entry vector
             * — but in the context of the *current* opcode (next
             * one is held back). */
            int cond_true = 0;
            int fa_c = (u->ff >> 6) & 3, fb_c = (u->ff >> 3) & 7;
            int ff_is_function_local = (u->bsel < 4);
            if (ff_is_function_local && fa_c == 0 && fb_c == 6) {
                cond_true = eval_branch_condition(cpu, u, u->ff & 7);
            }

            /* Read opcode at PCF. Save PCX (pc of currently-executing
             * opcode = the one we're about to dispatch to). If the
             * fetch hits a Vacant/WP-protected page (Map fault on
             * the IFU port), trap to *0-3 (HM page 33: "Map faults
             * on IFU fetches are reported instead to the IFU, which
             * buffers the fault until an IFUJump occurs"). */
            cpu->ifu_pcx     = cpu->ifu_pcf;
            int fetch_faulted = 0;
            uint8_t opcode   = ifu_fetch_byte(cpu, cpu->ifu_pcf, &fetch_faulted);
            if (fetch_faulted) {
                *next = ifu_trap_addr(0000, n_slot, cpu->ifu_insset);
                if (!ff_loads_link(u)) cpu->Link = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }

            /* Look up IFUM[InsSet||opcode]. If the entry isn't
             * present, halt — microcode mis-setup (or a real
             * machine would map-fault on the IFU port). */
            int ifum_addr = ((cpu->ifu_insset & 3) << 8) | opcode;
            if (cpu->mc && !cpu->mc->ifum_present[ifum_addr]) {
                return CPU_HALT_IFU_NO_ENTRY;
            }
            uint16_t lh = cpu->mc ? cpu->mc->ifum_hi[ifum_addr] : 0;
            uint16_t rh = cpu->mc ? cpu->mc->ifum_lo[ifum_addr] : 0;
            ifu_decode_lh(cpu, lh);
            uint16_t ifaddr = ifu_decode_rh(cpu, rh);

            /* Capture operand bytes if any (HM Table 19 for the full
             * sequence; here we just snapshot α and β so ←Id can
             * deliver them). For sets 0/1 (Alto-style), α follows
             * the opcode in the byte stream. */
            uint8_t length = cpu->ifu_length;
            uint16_t pc_after = (uint16_t)(cpu->ifu_pcf + 1);
            int op_faulted = 0;
            if (length >= 2) {
                cpu->ifu_alpha = ifu_fetch_byte(cpu, pc_after, &op_faulted);
                pc_after = (uint16_t)(pc_after + 1);
            }
            if (length >= 3 && !op_faulted) {
                cpu->ifu_beta = ifu_fetch_byte(cpu, pc_after, &op_faulted);
                pc_after = (uint16_t)(pc_after + 1);
            }
            if (op_faulted) {
                /* Operand fetch faulted — same trap as opcode fetch. */
                *next = ifu_trap_addr(0000, n_slot, cpu->ifu_insset);
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }

            /* Compute the next PCF for the *successor* opcode.
             * - regular: PCF = pc_after (next byte after operands)
             * - jump:    PCF = displacement-computed
             * - pause:   PCF stays put; microcode must issue PCF←B
             * If a conditional IFUJump's condition is true, the IFU
             * does NOT advance — PCF stays at the current opcode's
             * byte (HM page 33). */
            if (cond_true) {
                /* Hold pipeline. Don't update PCF or warmup. */
            } else if (cpu->ifu_type_pause) {
                cpu->ifu_active = 0;        /* halt fetching */
            } else if (cpu->ifu_type_jump) {
                /* HM page 66 jump-displacement formulas — minimal
                 * implementation: length=1 uses 6-bit signed
                 * (Sign||Packeda||N), length=2 uses α (sign-extended
                 * if Sign). */
                int16_t disp;
                if (length == 1) {
                    int8_t s = (int8_t)((cpu->ifu_sign << 5)
                                       | (cpu->ifu_packed_a << 4)
                                       | cpu->ifu_n);
                    if (s & 0x20) s = (int8_t)(s | 0xC0);  /* sign-extend 6→8 */
                    disp = (int16_t)s;
                } else {
                    int16_t a16 = (int16_t)cpu->ifu_alpha;
                    if (cpu->ifu_sign && (cpu->ifu_alpha & 0x80))
                        a16 = (int16_t)(a16 | 0xFF00);
                    disp = a16;
                }
                cpu->ifu_pcf = (uint16_t)(cpu->ifu_pcx + disp);
            } else {
                cpu->ifu_pcf = pc_after;
            }
            cpu->ifu_idcnt = 0;

            /* Compute TNIA: TNIA[4:13] = IFaddr', TNIA[14:15] = n.
             * Conditional IFUJump OR's the condition into TNIA[15],
             * routing to entry n|1 of the vector. */
            int n_eff = n_slot | (cond_true ? 1 : 0);
            uint16_t tnia = (uint16_t)((ifaddr << 2) | n_eff);

            *next = (uint16_t)(tnia & 0xFFF);
            /* IFUJump always loads Link with CIA+1 (HM page 33). */
            cpu->Link = (uint16_t)(cpu->real_PC + 1);
            return 0;
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
                *next = (uint16_t)((cpu->link_at_issue & 0xFFF) |
                                   ff_cond_or | cpu->dispatch_or);
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

                if (cpu->dbg_writeim_log && cpu->dbg_writeim_n < 32) {
                    cpu->dbg_writeim_addr[cpu->dbg_writeim_n] = addr;
                    cpu->dbg_writeim_half[cpu->dbg_writeim_n] = (uint8_t)half_lh;
                    cpu->dbg_writeim_sec[cpu->dbg_writeim_n]  = (uint8_t)secondary;
                    cpu->dbg_writeim_b[cpu->dbg_writeim_n]    = b;
                    cpu->dbg_writeim_t[cpu->dbg_writeim_n]    = cpu->T;
                    cpu->dbg_writeim_pc[cpu->dbg_writeim_n]   = cpu->real_PC;
                    cpu->dbg_writeim_n++;
                }

                if (half_lh) {
                    dst->iw0 = b;
                    /* iw2 bit 15 = manual RSTK[0] of destination. */
                    dst->iw2 = (uint16_t)((dst->iw2 & ~0x8000u) |
                                          ((uint16_t)secondary << 15));
                } else {
                    /* The right half is 17 bits wide. RSTK[2] supplies
                     * destination BLOCK; B carries iw1[14:0] followed by
                     * JCN[7] in its low bit (the same display form used
                     * in listings and BB boot streams). */
                    dst->iw1 = (uint16_t)(((uint16_t)secondary << 15) |
                                          ((b >> 1) & 0x7FFFu));
                    dst->iw2 = (uint16_t)((dst->iw2 & ~0x4000u) |
                                          ((b & 1u) << 14));
                }
                dorado_redecode_fields(dst);
                mc_w->im_present[addr] = 1;
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 4) {
                /* RdTPC←B (HM page 34): read task_tpc[B[12:15]] into
                 * Link, 1's complemented. Microcode then reads
                 * `B←Link, T←~B` to undo the inversion. The protocol
                 * leaves Link with 1's in bits 0:6 so the operation
                 * is a clean ~. Used by emulator/fault microcode to
                 * retrieve another task's saved PC. */
                uint16_t b = 0;
                if (!ff_override_b(cpu, u, &b)) {
                    int rc = b_bus(cpu, u, &b);
                    if (rc != 0) return rc;
                }
                int task = b & 0xF;
                uint16_t tpc = (task == cpu->ctask)
                             ? cpu->real_PC
                             : cpu->task_tpc[task];
                cpu->Link = (uint16_t)~tpc;
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 5) {
                /* LdTPC←B (HM page 34): write task_tpc[B[12:15]]
                 * from Link. "Dispatch-pending conditions for a
                 * task whose TPC is loaded by LdTPC← are cleared"
                 * — we don't model dispatches yet, so no-op there. */
                uint16_t b = 0;
                if (!ff_override_b(cpu, u, &b)) {
                    int rc = b_bus(cpu, u, &b);
                    if (rc != 0) return rc;
                }
                int task = b & 0xF;
                /* "An attempt to set TPC of the running task is
                 * unpredictable" — we just no-op the write in that
                 * case; for other tasks, write the saved-TPC slot
                 * so when we switch to that task it picks up there. */
                if (task != cpu->ctask) {
                    cpu->task_tpc[task] = cpu->Link;
                }
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 6) {
                /* Read IM (HM §4.8). Stubbed: real implementation
                 * reads IM[Link[7:15]] byte-by-byte selected by
                 * RSTK[2:3], placing inverted data on B in the next
                 * instruction. Not yet exercised by tests. */
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
    *next = (uint16_t)(page | offset | cpu->dispatch_or);
    /* Call vs Jump on conditional: it's a Call iff TNIA[12:15] = 0
     * before R is OR'd, i.e. when JCN[3:4] = 0. We stash CIA+1 in
     * Link in that case (per HM Figure 6 / §4.5). */
    if (page_low == 0 && !ff_loads_link(u)) {
        cpu->Link = (uint16_t)(cpu->real_PC + 1);
    }
    return 0;
}

/* Forward declare so the BB-injected single-step path (below) can
 * call into the same execution body the normal IM-fetch path uses. */
static int execute_uinstr(dorado_cpu *cpu, const dorado_uinstr *u, int from_im);

static int jcn_is_write_im(const dorado_uinstr *u)
{
    uint8_t jcn = u->jcn;
    if ((jcn & 7) != 7) return 0;
    if (((jcn >> 6) & 1) == 0) return 0;
    return ((jcn >> 3) & 7) == 7;
}

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
            if (cpu->mem) dorado_memory_tick(cpu->mem);
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
    cpu->dispatch_or = cpu->dispatch_pending;
    cpu->dispatch_pending = 0;
    uint16_t md_at_issue = task_md(cpu);
    uint8_t rbase_at_issue = cpu->RBase;

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
    /* HM Table 11a (FA=0 FB=0/1): A[12:15] ← FF[4:7] (gap B6).
     * The override fires only when FF is interpreted as a function
     * (ASEL > 3, BSEL not constant, JCN not long). */
    {
        int ovr = ff_a_low_override(u);
        if (ovr >= 0) a = (uint16_t)((a & 0xFFF0u) | (uint16_t)ovr);
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
    uint8_t is_arith = 0;
    uint16_t alu = alu_op(alufm_entry, a, b, &new_carry, &new_ovf, &is_arith);
    /* HM page 30: "Carry' and Overflow are the result of the last
     * *arithmetic* ALU operation". Logical ops must NOT update them —
     * preserve the previous values so subsequent Carry'/Overflow
     * branches see the last arithmetic result. */
    if (!is_arith) {
        new_carry = cpu->alu_carry;
        new_ovf   = cpu->alu_overflow;
    }

    /*
     * HM page 18 + page 30: branch conditions ALU=0, ALU<0, Carry',
     * and Overflow are LOADED INTO RAM AT t3 of this instruction, then
     * tested at t1 of the *next* instruction. So the cpu->alu_* flags
     * must keep the PREVIOUS instruction's values until next_pc has
     * read them; we capture the new values now and commit at the end
     * of execute_uinstr. */
    uint8_t new_alu_zero = (alu == 0) ? 1 : 0;
    uint8_t new_alu_lt0  = (alu & 0x8000) ? 1 : 0;

    /* Pd = ALU output (FF may then override or transform Pd, e.g. via
     * ALUFMRW or Pd←Cnt). FF post-effects also fire register loads
     * like Cnt←B, Link←B, RBase←FF[4:7], etc. */
    int ff_halt = 0;
    uint16_t pd = ff_apply_post(cpu, u, a, b, alu, new_carry, &ff_halt);
    if (ff_halt) {
        cpu->halted = 1;
        cpu->halt_reason = ff_halt;
        return 1;
    }

    /*
     * Memory references. ASEL = 0..3 dispatch a memory operation
     * with VA = BR[MemBase] + Mar (Mar = A bus on processor refs).
     * No memory subsystem attached → silently skip (we already
     * treat unhandled refs as no-ops).
     */
    if (cpu->mem && u->asel <= 3 && !jcn_is_write_im(u)) {
        /* io_task = 1 for non-emulator tasks (HM Table 8a:
         * Map↔IOFetch and Flush↔IOStore variants). */
        int io_task = (cpu->ctask != 0) ? 1 : 0;
        dorado_ref_kind kind = decode_ref_kind(u, io_task);
        if (kind != DM_REF_NONE) {
            /* MicroD's `Output_ <source>` form appears in display and disk
             * task code as a store-shaped instruction with no destination
             * load. Source listings make clear it drives the slow-I/O bus,
             * not main storage; real stores that write memory also load
             * DBuf. Route no-LC stores to a registered TIOA device before
             * issuing a spurious memory store. */
            if ((kind == DM_REF_STORE || kind == DM_REF_IOSTORE) &&
                u->lc == 0 &&
                cpu->io &&
                dorado_io_has_write(cpu->io, cpu->ctask,
                                    (uint8_t)cpu->TIOA)) {
                dorado_io_write(cpu->io, cpu->ctask,
                                (uint8_t)cpu->TIOA, b);
                goto memory_ref_done;
            }
            /* SubTask OR's into MemBase[2:3] (HM page 88). MemBase[2:3]
             * in MSB-first 5-bit MemBase = LSB bits 2..1. So OR
             * (subtask & 3) << 1. Only effective for non-emulator. */
            uint8_t membase = (uint8_t)(cpu->MemBase & 0x1F);
            if (cpu->ctask != 0) {
                membase |= (uint8_t)((cpu->task_subtask[cpu->ctask] & 3) << 1);
            }
            uint16_t mar = a;
            /* InitMem.mc's NextMapEntry emits `DummyRef_ T, T_ MD`
             * to make the memory system add one page to the current
             * BR and report the resulting VA through VALo/VAHi
             * (Pipe1/Pipe0). In the compiled Table-8a DummyRef shape
             * this is the ASEL=1, LC=T<-Md form; use T as Mar for
             * that source-level construct instead of RM/STK. */
            if (kind == DM_REF_DUMMYREF && u->asel == 1 && u->lc == 3) {
                mar = cpu->T;
            }
            uint16_t data = b;
            uint32_t br = dorado_mcr_disbr(cpu->mem)
                        ? 0
                        : dorado_br_get(cpu->mem, membase);
            uint32_t va = (br + mar) & 0x0FFFFFFFu;
            int subtask = (int)(cpu->task_subtask[cpu->ctask] & 3);
            dorado_fault_kind ref_fault =
                dorado_memory_ref_task(cpu->mem, kind, va, data, cpu->TIOA,
                                       (int)cpu->ctask, subtask);
            if (ref_fault == DM_FAULT_NONE && ref_kind_loads_md(kind)) {
                latch_task_md_from_memory(cpu);
            }
            {
                int lowcore_mode = lowcore_trace_mode();
                int lowcore_interesting =
                    lowcore_mode == 2 ||
                    kind == DM_REF_STORE || kind == DM_REF_IOSTORE;
                uint16_t lowcore_off = 0;
                const char *lowcore_base_name = NULL;
                uint32_t lowcore_base = 0;
                if (lowcore_mode && lowcore_interesting) {
                    uint32_t bases[3] = {
                        0,
                        dorado_br_get(cpu->mem, 036),
                        dorado_br_get(cpu->mem, 031),
                    };
                    const char *names[3] = { "abs", "mds", "iobr" };
                    for (int bi = 0; bi < 3; bi++) {
                        if (lowcore_trace_offset(va, bases[bi],
                                                 &lowcore_off)) {
                            lowcore_base = bases[bi] & 0x0FFFFFFFu;
                            lowcore_base_name = names[bi];
                            break;
                        }
                    }
                }
                if (lowcore_base_name) {
                    fprintf(stderr,
                            "LOWCORE task=%o pc=0o%o kind=%s base=%s:%07X "
                            "off=0o%o va=%07X data=%06o visible=%06o "
                            "mb=%02o rb=%02o sub=%o mar=%04X md=%06o "
                            "tioa=%03o fault=%d\n",
                            cpu->ctask & 017, cpu->real_PC,
                            ref_kind_name(kind), lowcore_base_name,
                            lowcore_base, lowcore_off, va & 0x0FFFFFFFu,
                            data & 0177777,
                            dorado_visible_word_at_va(cpu->mem, va) & 0177777,
                            membase & 037, cpu->RBase & 017, subtask & 3,
                            mar, cpu->mem->md & 0177777, cpu->TIOA & 0377,
                            (int)ref_fault);
                }
            }
            if (csb_trace_enabled()) {
                uint32_t iobr = dorado_br_get(cpu->mem, 031);
                uint16_t csb_off = 0;
                if (csb_trace_offset(va, iobr, &csb_off)) {
                    fprintf(stderr,
                            "CSB task=%o pc=0o%o kind=%s off=0o%o va=%07X "
                            "data=%06o visible=%06o mb=%02o rb=%02o sub=%o "
                            "mar=%04X md=%06o tioa=%03o fault=%d\n",
                            cpu->ctask & 017, cpu->real_PC,
                            ref_kind_name(kind), csb_off,
                            va & 0x0FFFFFFFu, data & 0177777,
                            dorado_visible_word_at_va(cpu->mem, va) & 0177777,
                            membase & 037, cpu->RBase & 017, subtask & 3,
                            mar, cpu->mem->md & 0177777, cpu->TIOA & 0377,
                            (int)ref_fault);
                }
            }
            if (eth_mem_trace_enabled()) {
                int packet_va = (va >= 0177400u && va <= 0177416u);
                int eth_task = (cpu->ctask == 6 || cpu->ctask == 7);
                if (packet_va || eth_task) {
                    fprintf(stderr,
                            "ETH_MEM task=%o pc=0o%o kind=%s rb=%02o mb=%02o "
                            "sub=%o br=%07X mar=%04X va=%07X "
                            "data=%06o md=%06o task_md=%06o "
                            "tioa=%03o fault=%d\n",
                            cpu->ctask & 017, cpu->real_PC,
                            ref_kind_name(kind), cpu->RBase & 017,
                            membase & 037,
                            subtask & 3, br & 0x0FFFFFFFu, mar,
                            va & 0x0FFFFFFFu, data & 0177777,
                            cpu->mem->md & 0177777,
                            task_md(cpu) & 0177777,
                            cpu->TIOA & 0377, (int)ref_fault);
                }
            }
            if (ref_fault != DM_FAULT_NONE) {
                cpu->mem->last_fault_pc = (uint16_t)(cpu->ifu_pcx & 0xFFFFu);
                cpu->mem->last_fault_real_pc = cpu->real_PC;
                cpu->mem->last_fault_membase = membase;
                cpu->mem->last_fault_tioa = cpu->TIOA;
                const char *ft = getenv("DORADO_FAULT_TRACE");
                if (ft && ft[0] &&
                    (strcmp(ft, "all") == 0 || cpu->ctask != 0)) {
                    fprintf(stderr,
                            "FAULT_CPU task=%o pc=0o%o mesa_pc=0x%04X "
                            "mb=%02o br=%07X mar=%04X tioa=%03o "
                            "T=%04X B=%04X kind=%d "
                            "asel=%o lc=%o ff=%03o jcn=%03o "
                            "iw=%06o/%06o/%06o\n",
                            cpu->ctask & 017, cpu->real_PC,
                            cpu->ifu_pcx & 0xFFFFu, membase & 037,
                            br & 0x0FFFFFFFu, mar, cpu->TIOA & 0377,
                            cpu->T, b, (int)kind,
                            u->asel & 017, u->lc & 017,
                            u->ff & 0377, u->jcn & 0377,
                            u->iw0 & 0777777, u->iw1 & 0777777,
                            u->iw2 & 0777777);
                }
            }
            /* HM page 46: a memory fault wakes up the fault task
             * (task 15). We propagate via wakeup_pending; the next
             * end-of-instruction scheduling round picks task 15 if
             * tasking is on. With tasking off (Bootstrap / early
             * Initial), the bit accumulates and fires later when
             * tasking turns on. Tests that read FaultInfo on the
             * faulting task must disable tasking before the fault
             * (test_cpu_fault_info_visible). */
            if (ref_fault != DM_FAULT_NONE && !dorado_mcr_nowake(cpu->mem)) {
                cpu->wakeup_pending |= (uint16_t)(1u << 15);
            }
        }
    }
memory_ref_done: ;

    /*
     * Branch decision uses the *pre-LC* register values per HM §4.4
     * ("the branch conditions are ordinarily loaded into the RAM at
     * t3" — i.e., latched from the register file before this cycle's
     * register write). Compute next PC first, then commit the LC write.
     */
    uint16_t np = (uint16_t)(cpu->real_PC + 1);
    uint8_t rbase_after_ff = cpu->RBase;
    cpu->RBase = rbase_at_issue;
    rc = next_pc(cpu, u, &np);
    cpu->RBase = rbase_after_ff;
    if (rc != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* LC. */
    if ((rc = apply_lc(cpu, u, pd, md_at_issue)) != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* Post-instruction StkP update + StkOvf/StkUnd recompute (HM
     * Table 6 / page 11). For BLOCK=0 instructions this is a no-op. */
    stk_apply_post(cpu, u);

    /* Commit branch-condition flags (HM page 30, "loaded into the RAM
     * at t3"). After this, the next instruction's next_pc reads these
     * via eval_branch_condition. FreezeBC (FA=0 FB=7 FC=6) suppresses
     * the load so the previous instruction's flags survive — checked
     * here against the FF field. */
    int fa_bc = (u->ff >> 6) & 3;
    int fb_bc = (u->ff >> 3) & 7;
    int fc_bc = u->ff & 7;
    int freezebc = ff_full_function_ok(u) &&
                   (fa_bc == 0 && fb_bc == 7 && fc_bc == 6);
    if (!freezebc) {
        cpu->alu_zero     = new_alu_zero;
        cpu->alu_lt0      = new_alu_lt0;
        cpu->alu_carry    = new_carry;
        cpu->alu_overflow = new_ovf;
    }

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

    if (from_im) junk_timer_tick(cpu);

    /* Tasking: at end of instruction, decide whether to switch tasks.
     * BLOCK=1 in a non-emulator task means "block this task" (HM
     * page 27); in the emulator (ctask=0) BLOCK=1 means StackSelect
     * for STK addressing — handled elsewhere, not a task block. */
    int block_in_non_emul = (u->block && cpu->ctask != 0);
    cpu->real_PC = task_schedule(cpu, np, block_in_non_emul);

    /* Cycle accounting + BB stepping only happen on IM-fetched
     * instructions. The injected-MIR caller (dorado_cpu_step) does
     * its own cycle++/baseboard_run after we return — it has to,
     * because we got here without any cpu->real_PC change. */
    if (from_im) {
        cpu->cycles++;
        if (cpu->mem) dorado_memory_tick(cpu->mem);
        if (cpu->baseboard && cpu->baseboard_cycles_per_uop > 0) {
            baseboard_run(cpu->baseboard, cpu->baseboard_cycles_per_uop);
        }
    }

    /* IFU warmup tick. Each cycle the pipeline is filling, this
     * counts down. When it reaches 0, IFUJump can dispatch. */
    if (cpu->ifu_active && cpu->ifu_warmup > 0) {
        cpu->ifu_warmup--;
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
