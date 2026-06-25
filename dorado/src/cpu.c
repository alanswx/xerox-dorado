#include "cpu.h"
#include "baseboard.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Harness-driven trace window gate (see tests/test_cpu.c
 * DORADO_TRACE_GATE). Nonzero only inside the requested cycle
 * window; debug traces that lack a cycle counter check this. */
int dorado_trace_gate = 0;

/* Harness-maintained cycle counter for cpu-level debug prints. */
unsigned long long dorado_trace_cycle = 0;

/* Cached boolean test of a trace environment variable. dorado_trace_flag() rescans the
 * whole environment on every call; with a populated shell environment the
 * per-microinstruction trace checks below cost ~2.4x of total run time. Trace
 * flags never change during a run, so cache the result keyed by the call
 * site's (string-literal) name pointer -- a pointer compare, no rescan. Pass
 * only string literals (boolean DORADO_*_TRACE flags); value-returning
 * dorado_trace_flag() calls must stay direct. */
int dorado_trace_flag(const char *name)
{
    enum { TRACE_FLAG_CACHE = 128 };
    static const char *keys[TRACE_FLAG_CACHE];
    static signed char vals[TRACE_FLAG_CACHE];
    static int n = 0;
    for (int i = 0; i < n; i++)
        if (keys[i] == name) return vals[i];
    int v = getenv(name) ? 1 : 0;
    if (n < TRACE_FLAG_CACHE) { keys[n] = name; vals[n] = (signed char)v; n++; }
    return v;
}

/* Debug: see memory.c STORE_VA trace. */
extern int dorado_mem_trace_pc;
extern int dorado_mem_trace_pcx;
extern int dorado_mem_trace_br31;
extern int dorado_mem_trace_op;
extern int dorado_mem_trace_membase;
extern int dorado_mem_trace_br;
extern int dorado_mem_trace_mar;

#define DORADO_JUNK_TASK          2
#define DORADO_JUNK_TICK_CYCLES   533   /* 32 us / 60 ns microcycle */
#define DORADO_TASKSIM_TASK       012   /* HM §3.12 TASKSIM wakeup target: task 0o12
                                         * (=10 dec), backplane-jumpered; the kernel
                                         * TestTW + Postamble "Task12" simulator */
#define DORADO_B15_MASK           0x0001u

/* Trace-only raw RM slots that have been useful during Cedar bring-up.
 * Do not treat these as architectural aliases: Mesa/Cedar Midas labels are
 * bank-relative and RBase-sensitive. */
#define DORADO_RM_CAND_000        0000
#define DORADO_RM_CAND_006        0006
#define DORADO_RM_CAND_025        0025
#define DORADO_RM_CAND_026        0026
#define DORADO_RM_RT_CLOCK        0165
#define DORADO_RM_RTC_DELTA_LO    0166
#define DORADO_RM_RTC_430         0167
#define DORADO_RM_WAKEUP_TIME     0274

/* Forward declarations for IFU helpers (defined later in this file). */
static uint16_t ifu_consume_id(dorado_cpu *cpu);
static uint16_t ifu_peek_id(const dorado_cpu *cpu);
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
    if (dorado_trace_flag("DORADO_JUNK_TRACE") &&
        cpu->junk_tw_enabled != (enable ? 1 : 0)) {
        static long n = 0;
        if (n++ < 200)
            fprintf(stderr, "JUNK_EN %d task=%o pc=0o%o\n",
                    enable ? 1 : 0, cpu->ctask, cpu->real_PC);
    }
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
    /* HM §8.3: IFUTest←B *dismisses* the periodic junk request: IFUTest.15
     * (Dorado bit 15 = the low-order C bit) DISABLES it when set. It must NOT
     * *enable* the periodic timer when clear — enabling is done by the junk
     * task itself (AckJunkTW←B) / the IFU Pendulum, e.g. AEmu's
     * `Wakeup[JNK]`+`AckJunkTW` self-sustain. Treating IFUTest←B(0) as an
     * enable spuriously woke the parked junk task in the Ifu/eventCounters
     * diagnostics (which reset/configure the IFU before setting up a junk
     * handler) → it ran from an empty IM park slot and halted. */
    if (b & DORADO_B15_MASK) junk_timer_enable(cpu, 0);
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

static int rtc_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_RTC_TRACE") ? 1 : 0;
    return cached;
}

static int resched_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_RESCHED_TRACE") ? 1 : 0;
    return cached;
}

static int timer_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_TIMER_TRACE") ? 1 : 0;
    return cached;
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
    cpu->task_alu_zero[t] = cpu->alu_zero;
    cpu->task_alu_lt0[t] = cpu->alu_lt0;
    cpu->task_alu_carry[t] = cpu->alu_carry;
    cpu->task_alu_overflow[t] = cpu->alu_overflow;
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
    cpu->alu_zero = cpu->task_alu_zero[t];
    cpu->alu_lt0 = cpu->task_alu_lt0[t];
    cpu->alu_carry = cpu->task_alu_carry[t];
    cpu->alu_overflow = cpu->task_alu_overflow[t];
}

/* End-of-instruction task scheduling. `next_pc` is the JCN-computed
 * resume PC of the currently-executing task. `block_in_non_emulator`
 * is 1 if this instruction was BLOCK=1 in a non-emulator task — that
 * clears Ready[ctask] and forces a switch. Returns the chosen new
 * value of `real_PC` (which the caller assigns to advance the
 * engine). */
/* HM p27 2-cycle wakeup latency for microcode Wakeup[n]. Default on (it is
 * the hardware behavior and is required for preemption correctness — see the
 * kernel TaskTest diagnostic). DORADO_WAKE_IMMEDIATE reverts to the old
 * single-cycle model for A/B timing debugging. Device wakeups
 * (dorado_cpu_wakeup) are intentionally NOT delayed here: the boot is
 * co-tuned to their current cadence, and correcting those is separate
 * (the cycle-accurate-timing project), not this fix. */
static int wake_immediate(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_WAKE_IMMEDIATE") ? 1 : 0;
    return cached;
}

/* HM §4.11 event-counter per-cycle event flags (cpu->evc_events). */
#define EVC_EV_HOLD        (1u << 0)
#define EVC_EV_PROCREF     (1u << 1)
#define EVC_EV_IFUREF      (1u << 2)
#define EVC_EV_IFUJUMP     (1u << 3)
#define EVC_EV_MISS        (1u << 4)
#define EVC_EV_IFUNOTREADY (1u << 5)

/* Did the 3-bit event selector `event` fire this cycle, given `flags`?
 * is_b distinguishes counter B (event 2 = IfuRef, 3 = IfuNotReady) from
 * counter A (event 2 = ProcRef, 3 = IfuJump). Events 5..7 are back-panel
 * signals brought in over external cables — never asserted here. */
static int event_counter_hit(uint8_t event, uint8_t flags, int is_b)
{
    switch (event & 7) {
    case 0: return 1;                              /* True — every cycle */
    case 1: return (flags & EVC_EV_HOLD) != 0;     /* Hold */
    case 2: return (flags & (is_b ? EVC_EV_IFUREF : EVC_EV_PROCREF)) != 0;
    case 3: return (flags & (is_b ? EVC_EV_IFUNOTREADY : EVC_EV_IFUJUMP)) != 0;
    case 4: return (flags & EVC_EV_MISS) != 0;     /* Miss */
    default: return 0;                             /* back-panel events */
    }
}

static uint16_t task_schedule(dorado_cpu *cpu, uint16_t next_pc,
                              int block_in_non_emulator)
{
    /* Promote the staged microcode wakeups one step down the 2-stage pipe
     * before this instruction's scheduling decision. A Wakeup[n] set in the
     * current instruction lands in pipe[0]; it reaches wakeup_pending only
     * after this shift runs on the NEXT instruction, so the woken task's
     * first instruction is 2 cycles after the Wakeup (HM p27). This lets a
     * task that wakes a higher-priority task execute the instruction after
     * the Wakeup (e.g. move a subroutine return out of a shared RM scratch
     * register into its per-task Link) before being preempted. */
    if (!wake_immediate()) {
        cpu->wakeup_pending |= cpu->wakeup_pipe[1];
        cpu->wakeup_pipe[1] = cpu->wakeup_pipe[0];
        cpu->wakeup_pipe[0] = 0;
    }

    /* HM §3.12 TASKSIM tick (once per cycle). TASKSIM is a 4-bit register
     * occupying taskFreq's bits in the Hold&TaskSim word's LEFT byte (the
     * sim.taskShift=8/sim.taskMask=0o177400 field): the diagnostic framework's
     * postamble describes it as "taskSim[0] enables the task simulator and
     * taskSim[1:3] form a counter ... when hardware counts to 17[octal] it
     * awakens." chkRunSimulators (postamble.mc) constructs taskFreq as
     * `(taskFreq+1) or 10b` cycling 0o12..0o17 — the 0o10 bit is the ENABLE
     * bit. With the simulator disabled (disableConditionalTask → resetHold
     * jams Hold&TaskSim=0, or a re-arm value whose 0o10 bit is clear) the
     * counter must NOT run, so the jumpered simulator task (task 12) is not
     * woken. tasksim here is (B>>8)&0x7F; the enable bit is 0o10. Counting up
     * from the loaded value, the wakeup fires on the cycle the 0o10 bit clears
     * (i.e. 0o17→0o20 overflow), and stays asserted until TASKSIM is reloaded.
     * Without the enable gate, a stale re-arm value like 0o104 (enable clear)
     * would spuriously keep waking task 12 and corrupt the cache-addressing
     * test's zeroed cache (memA CATUPADDRERR). */
    if (cpu->tasksim_fired) {
        cpu->wakeup_pending |= (uint16_t)(1u << DORADO_TASKSIM_TASK);
    } else if (cpu->tasksim_loaded) {
        cpu->tasksim_loaded = 0;            /* the load consumed this clock */
    } else if (cpu->tasksim & 010) {        /* taskSim[0] enable bit set */
        cpu->tasksim = (uint8_t)((cpu->tasksim + 1) & 0x7F);
        if (!(cpu->tasksim & 010)) {        /* counted 0o17→0o20: enable cleared */
            cpu->tasksim_fired = 1;
            cpu->wakeup_pending |= (uint16_t)(1u << DORADO_TASKSIM_TASK);
        }
    }

    /* HM §4.11 event counters (once per cycle). Each enabled counter ticks
     * when its selected event occurred during the previous cycle (accumulated
     * in evc_events) AND the cycle belongs to a counted task. By default
     * (tasksAll=0) only emulator (task 0) and fault-task (0o17) cycles count
     * ("EmuOrFT"); tasksAll=1 counts every task. True (event 0) ticks every
     * counted cycle. Counters stay at 0 unless a diagnostic enables them, so
     * this is inert for games/Cedar. */
    {
        int emu_or_ft = (cpu->ctask == 0) || ((cpu->ctask & 017) == 017);
        if (cpu->evc_a_enable && (cpu->evc_a_tasksall || emu_or_ft) &&
            event_counter_hit(cpu->evc_a_event, cpu->evc_events, 0))
            cpu->event_cnt_a++;
        if (cpu->evc_b_enable && (cpu->evc_b_tasksall || emu_or_ft) &&
            event_counter_hit(cpu->evc_b_event, cpu->evc_events, 1))
            cpu->event_cnt_b++;
    }
    cpu->evc_events = 0;

    /* TaskingOn delay: TaskingOn doesn't take effect until two more
     * instructions of the same task have executed (HM page 27). */
    if (cpu->tasking_resume_delay > 0) {
        cpu->tasking_resume_delay--;
        if (cpu->tasking_resume_delay == 0) cpu->tasking_on = 1;
    }

    /* If the current task is non-emulator and blocked, clear Ready.
     * HM page 27: BLOCK also clears the device section's wakeup-
     * request flip-flop for this task. Level-type requests (Ethernet,
     * display) re-assert on the next cycle's device poll if service
     * is still wanted, so the unconditional clear is safe — and
     * required: EOT's `Output_ TurnOffTx, Block` (AltoEther.mc
     * EOLast) must not see a wakeup latched earlier in the same
     * instruction, or the output task spuriously re-enters EOIdle and
     * retransmits the completed packet (the BCPL driver then finds
     * eOCLoc=0, the microcode posts CountZero, and EtherInterrupt
     * SysErrs with ecBadEtherStatus). */
    if (block_in_non_emulator) {
        cpu->ready &= (uint16_t)~(1u << cpu->ctask);
        cpu->wakeup_pending &= (uint16_t)~(1u << cpu->ctask);
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
    if (dorado_trace_flag("DORADO_TASK_TRACE") &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
        fprintf(stderr,
                "TASK_SWITCH cyc=%llu from=%o to=%o next=0o%o "
                "ready=%04X wake=%04X tpc_to=0o%o\n",
                (unsigned long long)dorado_trace_cycle,
                cpu->ctask & 017, bnt & 017, next_pc & 07777,
                cpu->ready, cpu->wakeup_pending,
                cpu->task_tpc[bnt] & 07777);
    }
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
static int ff_decode_ok(const dorado_uinstr *u);
static int ff_full_function_ok(const dorado_uinstr *u);
static uint16_t field_desc_to_shc(uint16_t a, int is_write);

/* True when FF decodes to the Table 11a function `code` (low 6 bits),
 * either as a full function with FA=0 or via the memory-reference
 * subdecode (ASEL 0..3), where "FA is decoded as zero" (HM §3.9) and
 * FF[0:1] selects the memory variant instead. */
static int ff_is_table11a(const dorado_uinstr *u, uint8_t code, uint8_t mask)
{
    if (!ff_decode_ok(u)) return 0;
    if ((u->ff & mask) != code) return 0;
    return ((u->ff >> 6) & 3) == 0 || u->asel <= 3;
}

static int stk_write_address(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    if (!u->block || cpu->ctask != 0) return rm_address(cpu, u);

    /* ModStkPBeforeW FF = FA=0 FB=2 FC=7 = 0o27 */
    if (!ff_is_table11a(u, 0027, 0077)) return rm_address(cpu, u);

    int adjusted = (cpu->StkP + stk_signed_delta(u)) & 0xFF;
    return CPU_RMSTK_STK_BASE | adjusted;
}

static int lc_write_address(const dorado_cpu *cpu, const dorado_uinstr *u)
{
    int fa = (u->ff >> 6) & 3;
    int fb = (u->ff >> 3) & 7;

    /* Two FF groups change the RM address for the write portion of an
     * instruction (HM §3.1 page 12); both force the write into RM even
     * when the instruction read STK, and SubTask OR does not apply.
     *
     * HM Table 11a (FA=0 FB=4-5): "Replace RMaddr[0:3] by RBase[0:3]
     * and RMaddr[4:7] by FF[4:7]" — same group of 16, register chosen
     * by FF. Also reachable through the memory-reference subdecode. */
    if (ff_is_table11a(u, 0040, 0060)) {
        return ((cpu->RBase & 0xF) << 4) | (u->ff & 0xF);
    }
    /* HM Table 11d (FA=2 FB=2-3): "Replace RMaddr[0:3] by FF[4:7] for
     * write of RM" — i.e. the complete write address is
     * FF[4:7],,RSTK[0:3]: an arbitrary RM bank without loading RBase
     * first. AEmu's EndOfStorage uses this (FF=0o221) to write
     * EmuBRHiReg/EmuXMBRHiReg in AEmRegs (bank 1) while RBase=EORegs.
     * Full-function form only (FA=2 cannot appear in the memory-ref
     * subdecode, where FA is decoded as zero). */
    if (ff_full_function_ok(u) && fa == 2 && (fb == 2 || fb == 3)) {
        return ((u->ff & 0xF) << 4) | (u->rstk & 0xF);
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
 * We model the observable fault-task wake by raising task 15 when
 * either flag is set; the fault microcode reads the flags via
 * Pd←Pointers. */
static int stk_error_check(const dorado_cpu *cpu, const dorado_uinstr *u,
                           int *underflow_out, int *overflow_out)
{
    int underflow = 0, overflow = 0;
    if (!u->block || cpu->ctask != 0) {
        if (underflow_out) *underflow_out = 0;
        if (overflow_out) *overflow_out = 0;
        return 0;
    }
    int delta = stk_signed_delta(u);
    if (delta == 0 && !stk_underflow_check(u)) {
        if (underflow_out) *underflow_out = 0;
        if (overflow_out) *overflow_out = 0;
        return 0;
    }

    /* Compute the new in-region offset (StkP[2:7], 6 bits) and check
     * for over/underflow. The region selector StkP[0:1] is preserved
     * across normal pushes/pops — overflow signals at the boundary. */
    int offset = cpu->StkP & 0x3F;     /* StkP[2:7] in bits 5..0 */
    int new_off = offset + delta;
    if (new_off < 0) underflow = 1;
    if (new_off > 077) overflow = 1;
    if (stk_underflow_check(u) && (new_off & 0x3F) == 0 && new_off >= 0)
        underflow = 1;
    if (underflow_out) *underflow_out = underflow;
    if (overflow_out) *overflow_out = overflow;
    return underflow || overflow;
}

static void stk_signal_error(dorado_cpu *cpu, int underflow, int overflow)
{
    cpu->stk_und = (uint8_t)underflow;
    cpu->stk_ovf = (uint8_t)overflow;
    cpu->wakeup_pending |= (uint16_t)(1u << 15);
}

static void stk_apply_post(dorado_cpu *cpu, const dorado_uinstr *u)
{
    if (!u->block || cpu->ctask != 0) return;
    int delta = stk_signed_delta(u);
    if (delta == 0 && !stk_underflow_check(u)) return;

    int underflow = 0, overflow = 0;
    (void)stk_error_check(cpu, u, &underflow, &overflow);

    /* StkUnd / StkOvf are sticky-recomputed: if this op didn't trip
     * either, clear them. */
    cpu->stk_und = (uint8_t)underflow;
    cpu->stk_ovf = (uint8_t)overflow;

    int region = cpu->StkP & 0xC0;     /* StkP[0:1] in bits 7,6 */
    int offset = cpu->StkP & 0x3F;     /* StkP[2:7] in bits 5..0 */
    int new_off = offset + delta;
    uint8_t old_stkp = cpu->StkP;
    cpu->StkP = (uint8_t)(region | (new_off & 0x3F));
    if (dorado_trace_flag("DORADO_STKP_TRACE") &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
        fprintf(stderr,
                "STKP_POST cyc=%llu pc=0o%o rstk=%02o delta=%+d old=%03o new=%03o und=%d ovf=%d\n",
                (unsigned long long)dorado_trace_cycle,
                cpu->real_PC, u->rstk & 017, delta,
                old_stkp & 0377, cpu->StkP & 0377,
                underflow, overflow);
    }
}

static uint16_t rm_stk_read(const dorado_cpu *cpu, int idx)
{
    if (idx >= CPU_RMSTK_INVALID) return 0;
    if (idx >= CPU_RMSTK_STK_BASE) return cpu->STK[idx & 0xFF];
    return cpu->RM[idx & 0xFF];
}

static int trace_membase_matches(int membase)
{
    static int cached = -2;
    if (cached == -2) {
        const char *w = getenv("DORADO_BR_TRACE_MB");
        cached = w ? (int)strtol(w, NULL, 8) : -1;
    }
    return cached < 0 || ((membase & 0x1F) == (cached & 0x1F));
}

static int rm_watch_matches(int idx)
{
    static int parsed = 0;
    static int count = 0;
    static long watch[32];

    if (!parsed) {
        const char *w = getenv("DORADO_RM_WATCH");
        parsed = 1;
        while (w && *w && count < (int)(sizeof watch / sizeof watch[0])) {
            char *end = NULL;
            watch[count] = strtol(w, &end, 8);
            if (end == w) break;
            count++;
            w = (*end == ',') ? end + 1 : end;
            if (*w != ',') {
                while (*w == ' ' || *w == '\t') w++;
                if (*w == ',') w++;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if ((idx & 0xFF) == (watch[i] & 0xFF)) return 1;
    }
    return 0;
}

static void rm_stk_write(dorado_cpu *cpu, int idx, uint16_t value)
{
    if (idx >= CPU_RMSTK_INVALID) return;
    if (idx >= CPU_RMSTK_STK_BASE) cpu->STK[idx & 0xFF] = value;
    else {
        if (rm_watch_matches(idx) &&
            (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
            static long n = 0;
            static long limit = -2;
            if (limit == -2) {
                const char *w = getenv("DORADO_RM_WATCH_LIMIT");
                limit = w ? strtol(w, NULL, 0) : 100;
            }
            if (limit <= 0 || n++ < limit) {
                fprintf(stderr,
                        "RM_WATCH cyc=%llu RM[%03o]=%06o task=%o "
                        "pc=0o%o rb=%02o pcx=0o%o pcf=0o%o op=%03o\n",
                        dorado_trace_cycle, idx & 0377,
                        value & 0177777, cpu->ctask,
                        cpu->real_PC, cpu->RBase & 017,
                        cpu->ifu_pcx, cpu->ifu_pcf,
                        cpu->ifu_opcode);
            }
        }
        cpu->RM[idx & 0xFF] = value;
    }
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

static int ff_fa0_fb2_fc_ok(const dorado_uinstr *u, int fc)
{
    if (ff_full_function_ok(u)) {
        return ff_fa(u->ff) == 0 && ff_fb(u->ff) == 2 && ff_fc(u->ff) == fc;
    }

    if (ff_decode_ok(u) && u->asel <= 3) {
        return ((u->ff >> 3) & 7) == 2 && (u->ff & 7) == fc;
    }

    return 0;
}

static int ff_is_output_b_memory_form(const dorado_uinstr *u)
{
    return ff_decode_ok(u) && u->asel <= 3 && ((u->ff & 077) == 0036);
}

static int ff_is_output_b_long_call_form(const dorado_uinstr *u)
{
    /* InitialDisk.mc/PilotDisk.mc emit `Output_ KCmmd,
     * Call[UpdateSector]`; the compiled form is a long-call JCN, so
     * ff_full_function_ok() deliberately rejects the normal FF
     * function decode. The source-level Output side effect is still
     * real and must reach DiskControl before the call. Keep the match
     * to the observed non-memory shape instead of relaxing long-call
     * FF decoding globally. */
    return u->asel == 4 &&
           u->bsel == 1 &&
           u->lc == 6 &&
           u->aluf == 013 &&
           u->ff == 0327 &&
           u->jcn == 0017;
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
/* FF-selected A sources (HM 3.5 p16: "The FF field can be used to
 * select any of the following sources: FF[4:7] (small constant),
 * RM/STK, Q, T, Md ... with references, these functions overrule
 * RM/STK as the source"). Table 11a FB=0-1 is the small constant;
 * FB=2 FC=0-3 selects RM/STK, T, Md, Q (the manual's Table 11a line
 * prints FC=3 as "A<-0", but 3.5 lists Q and AEmu's `ETemp_ Q+1`
 * (FF=0o23, ALUF=0o12) compiles through it — it is A<-Q).
 * Returns 1 and sets *out when an override applies. */
static int ff_a_override(const dorado_cpu *cpu, const dorado_uinstr *u,
                         uint16_t *out)
{
    if (!ff_decode_ok(u)) return 0;

    /* ASEL=7 selects the barrel shifter as the A-bus source (HM §3.11), so an
     * FF A-source override (A<-T/Md/Q, or A[12:15]<-FF[4:7]) cannot apply --
     * the shifter owns A. The FF field on a shift carries shift control / a
     * carry modifier instead. Without this guard a ShC-controlled shift whose
     * FF happens to decode as FA=0 FB=2 FC=1..3 (e.g. ShMdBothMasks with
     * FF=023 -> "A<-Q") clobbered the cycled word with Q, so the Mesa BitBlt /
     * text scan-converter (DMesaMiscOps) wrote (NOT 0) OR B = all-ones and
     * every glyph came out as a solid block. */
    if (u->asel == 7) return 0;

    uint8_t f6;
    if (u->asel == 0 || u->asel == 1) {
        /* Memory-reference subdecode: FF[0:1] select the reference
         * kind; the low six bits decode as functions 0..63. */
        f6 = (uint8_t)(u->ff & 077);
    } else if (ff_full_function_ok(u) && ff_fa(u->ff) == 0) {
        f6 = (uint8_t)(u->ff & 077);
    } else {
        return 0;
    }

    int fb = (f6 >> 3) & 7, fc = f6 & 7;
    if (fb <= 1) { *out = (uint16_t)(u->ff & 0xF); return 1; }
    if (fb == 2) {
        switch (fc) {
        case 0: *out = 0; return 0;       /* A<-RM/STK — the default */
        case 1: *out = cpu->T;        return 1;   /* A<-T  */
        case 2: *out = task_md(cpu);  return 1;   /* A<-Md */
        case 3: *out = cpu->Q;        return 1;   /* A<-Q  */
        }
    }
    return 0;
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
         * EmulatorFault). No-fault signature: the 3-bit FaultCnt
         * field (b9:11) reads -1, i.e. high-true 0x0070. */
        case 0: *b = (uint16_t)~finfo;             /* FaultInfo' */
                if (dorado_trace_flag("DORADO_FAULTREG_TRACE") &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "FAULTREG cyc=%llu task=%o pc=0o%o src=FaultInfo' "
                            "finfo=%04X b=%04X clear=1\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            finfo, *b);
                }
                dorado_fault_clear(cpu->mem);
                break;
        case 1: *b = (uint16_t)((va >> 16) & 0x0FFF);
                if (dorado_trace_gate && dorado_trace_flag("DORADO_PIPEVA_TRACE"))
                    fprintf(stderr, "PIPEVA pc=0o%o psrn=%d va=%07X (Pipe0/VaHi)\n",
                            cpu->real_PC, psrn, va);
                break; /* Pipe0 = VaHi */
        case 2: *b = (uint16_t)(va & 0xFFFF);
                if (dorado_trace_gate && dorado_trace_flag("DORADO_PIPEVA_TRACE"))
                    fprintf(stderr, "PIPEVA pc=0o%o psrn=%d va=%07X (Pipe1/VaLo)\n",
                            cpu->real_PC, psrn, va);
                break;  /* Pipe1 = VaLo */
        /* Pipe2' is the same data as FaultInfo' (HM page 51:
         * "B←Pipe2' is simply a convenient decode for reading
         * [FaultInfo] back"). */
        case 3: *b = (uint16_t)~finfo;
                if (dorado_trace_flag("DORADO_FAULTREG_TRACE") &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "FAULTREG cyc=%llu task=%o pc=0o%o src=Pipe2' "
                            "finfo=%04X b=%04X clear=0\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            finfo, *b);
                }
                break;  /* Pipe2' */
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
                if (dorado_trace_flag("DORADO_FAULTREG_TRACE") &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "FAULTREG cyc=%llu task=%o pc=0o%o src=Pipe4' "
                            "psrn=%o b=%04X va=0o%o kind=%d "
                            "oldrp=0o%o oldflags=%o last=%d lastva=0o%o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            psrn & 017, *b, va,
                            (int)cpu->mem->pipe[psrn & 017].kind,
                            dorado_pipe_map_rp_at(cpu->mem, psrn),
                            dorado_pipe_map_flags_at(cpu->mem, psrn) & 07,
                            (int)cpu->mem->last_fault,
                            cpu->mem->last_fault_va & 0x0FFFFFFFu);
                }
                break;                              /* Pipe4' (errors) */
        case 6: *b = (uint16_t)~dorado_memory_config_word(cpu->mem);
                break;                             /* Config' */
        case 7: *b = dorado_pipe5_at(cpu->mem, psrn); break; /* Pipe5 */
        default: *b = 0;                           break;
        }
        /* HM Table 7 asterisk: when an external B source is in play AND
         * BSEL=3, the external value also lands in Q. This applies to the
         * pipe sources too (same as the FB=7 block below) -- the Mesa
         * `Q_ VALo` idiom (XferFree, `Q_ B<-Pipe1`) relies on it; without
         * it the freed frame's VA-lo stayed in the old Q (=1), so the
         * free linked garbage (AV[fsi]=Q+1=2) instead of the real frame. */
        if (u->bsel == 3) cpu->Q = *b;
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
        case 1: /* B ← EventCntA' / GenIn' (HM §4.11; FF256[171]). Active-low.
                 * In GenIO mode (counters disabled) the IFU-board plug ties
                 * GenIn to GenOut, so GenIn' reads back EventCntB (=GenOut);
                 * otherwise it reads the real EventCntA. (eventCounters1.mc
                 * genIO test: write GenOut, read it back via GenIn'.) */
                *b = (uint16_t)~(cpu->gen_io_mode ? cpu->event_cnt_b
                                                  : cpu->event_cnt_a);
                break;
        case 2: /* B ← IFUMRH' (fields half — Sign,,IPar,,Length',,
                 * RBaseB',,MemB,,TPause',,TJump',,N — inverted). Per
                 * LoadRam.mc the RH is the fields half (item word1),
                 * stored in ifum_hi. Address = InsSet||Opcode (set by
                 * InsSetorEvent←B and BrkIns←B). */
                if (cpu->mc) {
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    *b = (uint16_t)~cpu->mc->ifum_hi[addr];
                } else {
                    *b = 0xFFFF;
                }
                break;
        case 3: /* B ← IFUMLH' (address half — PackedAlpha,,IFaddr' —
                 * inverted). Per LoadRam.mc the LH is the address half
                 * (item word0), stored in ifum_lo. */
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
        case 7:                          /* B ← Link */
            /* HM §4.8: after RdTPC / Read IM, the read data is delivered on
             * the alternate B←Link path for one cycle (Link itself holds
             * CIA+1). Consume the latch if pending, else read Link. */
            if (cpu->b_link_read_valid) {
                *b = cpu->b_link_read;
                cpu->b_link_read_valid = 0;
            } else {
                *b = cpu->Link;
            }
            break;
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

    if (ff_is_output_b_long_call_form(u) &&
        cpu->io &&
        dorado_io_has_write(cpu->io, cpu->ctask, (uint8_t)cpu->TIOA)) {
        dorado_io_write_subtask(cpu->io, cpu->ctask,
                                cpu->task_subtask[cpu->ctask],
                                (uint8_t)cpu->TIOA, b);
    }

    /* When BSEL produces a constant (BSEL >= 4), or when FF[0:1] picks
     * an alternate A source for memory references (ASEL=0..3 with
     * specific FF[0:1]), the FF field is *not* interpreted as a
     * function. Per HM §3.9: "FF interpreted as a function iff (BSEL
     * not selecting a constant) and (JCN does not select a 'long'
     * goto/call)." */
    int ff_is_function = ff_full_function_ok(u);
    if (!ff_is_function) {
        if (ff_decode_ok(u) && u->asel <= 3) {
            /* HM §3.9 (page 15): "When FF is ok and ASEL = 0 to 3,
             * the decoding of FF as a function is forced to be in the
             * range 0 to 63" — FF[0:1] selects the memory-reference
             * variant, and the remaining six bits execute as the FULL
             * Table 11a function set. LoadRam's `Q_ MD` (FF=0o373),
             * `BDispatch_ Q` (FF=0o371) and AEmu's JSR-indirect
             * `Fetch_ T, FlipMemBase` (FF=0o337) all depend on this;
             * dropping any of them silently corrupts microcode flow
             * (JSRix without the flip loads the jump target into the
             * MDS base register). Re-dispatch through the FA=0 cases
             * below with the low six bits. */
            fa = 0;
            fb = (u->ff >> 3) & 7;
            fc = u->ff & 7;
        } else {
            return pd;
        }
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
            case 4: /* XorCarry — applied to the ALU carry-in pre-ALU  */
            case 5: /* XorSavedCarry — applied to carry-in pre-ALU     */
            case 6: /* Carry20 — force carry into bit 12 (still TBD)   */
            case 7: /* ModStkPBeforeW (handled in stk_write_address)   */
                return pd;
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
                        ? dorado_io_read_subtask(cpu->io, cpu->ctask,
                                                cpu->task_subtask[cpu->ctask],
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
                        ? dorado_io_read_subtask(cpu->io, cpu->ctask,
                                                cpu->task_subtask[cpu->ctask],
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
                    dorado_io_write_subtask(cpu->io, cpu->ctask,
                                            cpu->task_subtask[cpu->ctask],
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
            case 0: /* BigBDispatch ← B. HM §4.4 p32: loads Link
                     * from B, then ORs Link[8:15] into TNIA during
                     * the next instruction for this task. */
                cpu->Link = b;
                cpu->task_dispatch[cpu->ctask] = b & 0xFF;
                return pd;
            case 1: /* BDispatch ← B (8-way; Link[13:15]) */
                cpu->Link = b;
                cpu->task_dispatch[cpu->ctask] = b & 0x7;
                return pd;
            case 2: /* Multiply (HM §4.4 p32): "OR's Q[14] into
                     * TNIA[14]. The value of Q[14] is captured in a
                     * flipflop at t2 of the instruction containing
                     * the Multiply function and is OR'ed into
                     * TNIA[14] during the next instruction for the
                     * same task." Manual bit 14 of Q = C-LSB bit 1;
                     * TNIA[14] = bit 1 of the 12-bit address. AEmu's
                     * Nova MUL microcode steps on this dispatch; with
                     * it stubbed, every BCPL multiply returned
                     * garbage (NetExec's keyword-table size collapsed
                     * and its entries overran into the first
                     * coroutine's CTX, crashing startup into Swat). */
                cpu->task_dispatch[cpu->ctask] |=
                    (uint16_t)(cpu->Q & 0x0002);
                /* HM p23, the other two effects:
                 *   Result <- ALUcarry,,ALU/2
                 *   Q      <- ALU[15],,Q/2     (manual bit 15 = LSB)
                 * The Q[14] dispatch capture above uses the PRE-shift
                 * Q (captured at t2, before the t3 write). */
                cpu->Q = (uint16_t)(((alu & 1) << 15) | (cpu->Q >> 1));
                return (uint16_t)(((uint16_t)(alu_carry & 1) << 15) |
                                  (alu >> 1));
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
                if (cpu->mem) {
                    if (dorado_trace_flag("DORADO_BR_TRACE") &&
                        trace_membase_matches(cpu->MemBase) &&
                        (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate)) {
                        fprintf(stderr,
                                "BRLO_FF task=%o pc=0o%o mb=%02o a=%06o "
                                "T=%06o md=%06o\n",
                                cpu->ctask & 017, cpu->real_PC,
                                cpu->MemBase & 0x1F, a, cpu->T,
                                cpu->mem->md & 0177777);
                    }
                    dorado_br_lo_load(cpu->mem, cpu->MemBase, a);
                }
                return pd;
            case 4: /* BrHi ← A — BR[MemBase][4:15] ← A[4:15]
                     * (HM Table 11c FA=1 FB=2 FC=4). */
                if (cpu->mem) {
                    if (dorado_trace_flag("DORADO_BR_TRACE") &&
                        trace_membase_matches(cpu->MemBase) &&
                        (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate)) {
                        fprintf(stderr,
                                "BRHI_FF task=%o pc=0o%o mb=%02o a=%06o "
                                "rb=%02o EmuBRHi=0o%o EmuXMBRHi=0o%o "
                                "Q=%06o\n",
                                cpu->ctask & 017, cpu->real_PC,
                                cpu->MemBase & 0x1F, a, cpu->RBase & 017,
                                cpu->RM[030], cpu->RM[031], cpu->Q);
                    }
                    dorado_br_hi_load(cpu->mem, cpu->MemBase, a);
                }
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
                    if (dorado_trace_flag("DORADO_INSSET_TRACE")) {
                        fprintf(stderr,
                                "INSSET task=%o pc=0o%o b=%06o insset %u->%u "
                                "link=0o%o T=%06o pcf=0o%o active=%u\n",
                                cpu->ctask & 017, cpu->real_PC, b,
                                cpu->ifu_insset & 3, (b >> 8) & 3,
                                cpu->Link, cpu->T, cpu->ifu_pcf,
                                cpu->ifu_active);
                    }
                    cpu->ifu_insset = (uint8_t)((b >> 8) & 3);
                } else {
                    cpu->event_cnt_ctrl_hi = (uint8_t)((b >> 8) & 0x0F);
                    cpu->event_cnt_ctrl_lo = (uint8_t)(b & 0xFF);
                    /* MOS control load (the "mos←" FF). GenIO mode is active
                     * when both counter enables are clear (ctr.Aenable=B[4]=
                     * native bit 11, ctr.Benable=B[5]=native bit 10, mask
                     * 0x0C00). `mos←useGenIO` (B=0) enters it; loading enable
                     * bits for the counter tests leaves it. */
                    cpu->gen_io_mode = ((b & 0x0C00) == 0);
                    /* HM §4.11 event-counter control (eventCounters1.mc):
                     * A enable=B[4]=bit11, A event=(b>>5)&7; B enable=B[5]=
                     * bit10, B event=(b>>1)&7. */
                    cpu->evc_a_enable   = (uint8_t)((b >> 11) & 1);
                    cpu->evc_a_event    = (uint8_t)((b >> 5) & 7);
                    cpu->evc_a_tasksall = (uint8_t)((b >> 4) & 1);
                    cpu->evc_b_enable   = (uint8_t)((b >> 10) & 1);
                    cpu->evc_b_event    = (uint8_t)((b >> 1) & 7);
                    cpu->evc_b_tasksall = (uint8_t)(b & 1);
                }
                return pd;
            case 1: /* EventCntB ← B / GenOut ← B (HM §4.11; FF=0o131). EventCntB
                     * is the IFU board's GenOut register (genOut←). */
                cpu->event_cnt_b = b;
                return pd;
            case 2: /* Reschedule (HM Table 20). Cause a reschedule
                     * trap on the second OR third successful IFUJump,
                     * and set the (emulator-only) Reschedule branch
                     * condition. */
                cpu->reschedule_pending = 2;
                cpu->reschedule_cond = 1;
                if (resched_trace_enabled() &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "RESCHED_SET cyc=%llu task=%o pc=0o%o "
                            "rb=%02o T=%06o pd=%06o "
                            "rm000=%06o rm006=%06o rm025=%06o rm026=%06o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            cpu->RBase & 017, cpu->T & 0177777,
                            pd & 0177777,
                            cpu->RM[DORADO_RM_CAND_000] & 0177777,
                            cpu->RM[DORADO_RM_CAND_006] & 0177777,
                            cpu->RM[DORADO_RM_CAND_025] & 0177777,
                            cpu->RM[DORADO_RM_CAND_026] & 0177777);
                }
                return pd;
            case 3: /* NoReschedule. Turn off the Reschedule trap AND
                     * branch condition (HM Table 20). */
                if (resched_trace_enabled() &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE")) &&
                    (cpu->reschedule_pending || cpu->reschedule_cond ||
                    dorado_trace_flag("DORADO_RESCHED_CLR_TRACE"))) {
                    fprintf(stderr,
                            "RESCHED_CLR cyc=%llu task=%o pc=0o%o "
                            "pending=%u cond=%u rb=%02o T=%06o "
                            "rm000=%06o rm006=%06o rm025=%06o rm026=%06o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            cpu->reschedule_pending, cpu->reschedule_cond,
                            cpu->RBase & 017, cpu->T & 0177777,
                            cpu->RM[DORADO_RM_CAND_000] & 0177777,
                            cpu->RM[DORADO_RM_CAND_006] & 0177777,
                            cpu->RM[DORADO_RM_CAND_025] & 0177777,
                            cpu->RM[DORADO_RM_CAND_026] & 0177777);
                }
                cpu->reschedule_pending = 0;
                cpu->reschedule_cond = 0;
                return pd;
            case 4: /* IFUMRH ← B (HM Table 11c): the FIELDS half —
                     * Sign←B.0, PE[0:2]←B[1:3], Length'←B[4:5],
                     * RBaseB'←B.6, MemB←B[7:9], TPause'←B.10,
                     * TJump←B.11, N←B[12:15]. Stored in ifum_hi
                     * (= .MB IFUM word 1). Per LoadRam.mc, IFUMRH← is
                     * loaded from item word1 (the fields half). */
                if (cpu->mc) {
                    dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    mc_w->ifum_hi[addr] = b;
                    mc_w->ifum_present[addr] = 1;
                }
                return pd;
            case 5: /* IFUMLH ← B (HM Table 11c): the ADDRESS half —
                     * PackedAlpha←B.5, IFaddr'←B[6:15]. Stored in
                     * ifum_lo (= .MB IFUM word 0). Per LoadRam.mc,
                     * IFUMLH← is loaded from item word0 (the address
                     * half); AEmu's LDAipc proves word 0 carries
                     * IFaddr'. */
                if (cpu->mc) {
                    dorado_microcode *mc_w = (dorado_microcode *)cpu->mc;
                    int addr = ((cpu->ifu_insset & 3) << 8) | cpu->ifu_opcode;
                    mc_w->ifum_lo[addr] = b;
                    mc_w->ifum_present[addr] = 1;
                }
                return pd;
            case 6: /* IFUReset — halt and clear the IFU pipeline, clear
                     * BrkPending/BrkIns state, and load the IFU test-control
                     * register with 1 (HM Table 20 / §6.8). Reschedule and
                     * InsSet are explicitly not cleared.
                     *
                     * Preserve the current X-level opcode context. HM §6.2
                     * says PCF<-B does not affect PCX, B<-PCX' keeps reading
                     * the current opcode PC until IFUJump, and <-Id returns
                     * Length after the operands are consumed. Cedar's map
                     * restart path depends on that context surviving
                     * IFUReset:
                     *   T_ ID, IFUReset       * ID = instruction length
                     *   T_ T-(PCX')-1
                     *   PCF_ T; IFUJump       * restart at successor
                     */
                cpu->ifu_active = 0;
                cpu->ifu_warmup = 0;
                cpu->ifu_pcf = 0;
                cpu->brk_pending = 0;
                cpu->brk_opcode = 0;
                /* IFUReset clears the IFU pipeline (HM p79) but in this
                 * model does NOT change the junk-task wakeup enable. The
                 * periodic junk wakeup is started explicitly by the
                 * program that wants it: AEmu does `LdTPC_ T, Wakeup[JNK]`
                 * then the junk task self-sustains via `AckJunkTW_ T`
                 * (Junk.mc) — so AEmu's RTClock keeps running regardless of
                 * IFUReset. A program that never wakes the junk task (e.g.
                 * the memA diagnostic, whose junk handler TPC is never set)
                 * leaves it disabled, so it does not spuriously starve the
                 * main task. The previous code enabled the junk timer on
                 * every IFUReset, which fired the handler-less junk task in
                 * memA and starved task 0. */
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
            case 0: /* UseDMD — strobe the loaded DMux address into the
                     * BaseBoard manifold as a WRITE (Initial
                     * WriteManifold: InitialMain.mc/InitialSelectMain.mc).
                     * Consumes the pending DMux address so it does not
                     * leak into a later ALUFMem read. */
                if (cpu->mem) dorado_memory_dmux_use_dmd(cpu->mem);
                return pd;
            case 1: /* MidasStrobe ← B — serially shift the DMux/muffler
                     * address (Various.mc SetDMuxAddress;
                     * Initial WriteManifold). Capture the address on the
                     * leading strobe of a sequence. */
                if (cpu->mem) dorado_memory_dmux_strobe(cpu->mem, b);
                return pd;
            case 2: /* TaskingOff (HM page 27) — atomic; effective
                     * immediately. Subsequent instructions of the
                     * same task run without task switches. */
                cpu->tasking_on = 0;
                cpu->tasking_resume_delay = 0;
                return pd;
            case 3: /* TaskingOn — not immediately effective; HM p27:
                     * "at least two more instructions will be executed
                     * by the same task before task switching can occur."
                     * CAVEAT (timing is approximate, not cycle-exact):
                     * task_schedule() also decrements on THIS instruction
                     * so the effective gate is one instruction, not two;
                     * and the emulator does NOT model the separate HM
                     * wakeup->execution latency ("two cycles", HM p27) --
                     * dorado_cpu_wakeup() makes a wakeup eligible on the
                     * very next instruction. These two approximations are
                     * a CONSISTENT PAIR: the boot is tuned to them, so
                     * changing one alone desyncs them (raising this to 3
                     * spins the I/O tasks -- disk wakeups 170K -> 27M --
                     * and the boot never reaches NetExec). A cycle-exact
                     * model would fix BOTH the TaskingOn gate and the
                     * 2-cycle wakeup latency together; that is the right
                     * place to look for residual async-timing bugs. */
                cpu->tasking_resume_delay = 2;
                return pd;
            case 4: /* StkP ← B[8:15] */
                cpu->StkP = b & 0xFF;              return pd;
            case 5: /* RestoreStkP (HM §3.1): reload the StkP value saved
                     * when the current opcode was dispatched by the IFU, so
                     * a trap can restart an opcode after partial stack motion
                     * (e.g. SFC popping its destination link). */
                if (dorado_trace_flag("DORADO_STKP_TRACE") &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "RESTORE_STKP cyc=%llu pc=0o%o old=%03o saved=%03o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->real_PC, cpu->StkP & 0377,
                            cpu->ifu_saved_stkp & 0377);
                }
                cpu->StkP = cpu->ifu_saved_stkp;    return pd;
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
            case 4: /* Hold&TaskSim ← B (HM §3.12 hardware-checkout sim).
                     * "loads HOLDSIM[0:7] from B[8:14],,0 and TASKSIM[0:6]
                     * from B[1:7]." In C-LSB: TASKSIM = B[1:7] = (B>>8)&0x7F;
                     * HOLDSIM = B[8:14],,0 = B&0xFE. Loading clears the
                     * fired latch (a new load re-arms the counter). TASKSIM
                     * is the 7-bit task-wakeup counter; HOLDSIM is the
                     * recirculating hold shift register (stored; its HOLD
                     * injection is not yet modeled — the diagnostics' first
                     * pass runs with HOLDSIM=0). */
                cpu->tasksim = (uint8_t)((b >> 8) & 0x7F);
                cpu->holdsim = (uint8_t)(b & 0xFE);
                cpu->tasksim_fired = 0;
                cpu->tasksim_loaded = 1;  /* don't also tick this cycle */
                if (getenv("DORADO_TASKSIM_TRACE"))
                    fprintf(stderr, "TASKSIM load task=%o pc=0o%o b=%06o tasksim=%03o holdsim=%03o\n",
                            cpu->ctask & 017, cpu->real_PC, b & 0177777,
                            cpu->tasksim, cpu->holdsim);
                return pd;
            case 5: /* WF ← A (load ShC with write-field controls) */
                cpu->ShC = field_desc_to_shc(a, /*is_write=*/1);
                return pd;
            case 6: /* RF ← A (load ShC with read-field controls) */
                cpu->ShC = field_desc_to_shc(a, /*is_write=*/0);
                return pd;
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
            /* HM Table 11d: write address becomes FF[4:7],,RSTK[0:3]
             * (force RM write). Handled in lc_write_address(); RBase
             * itself is NOT loaded. */
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
                /* HM Table 11d (FA=2): MemBase[0]←0, MemBase[1:2]←MemBX[0:1],
                 * MemBase[3:4]←FF[6:7]. In C-LSB the 5-bit MemBase is
                 * ((MemBX[0:1])<<2) | FF[6:7], with the top bit (0x10)
                 * forced 0. This is the same construction the IFU uses to
                 * load a MemBX-relative MemBase (see the MemB[0]==0 case
                 * below). The previous code reused the OLD MemBase bits,
                 * so `MemBase_ L` (FF=0o250, MemBX=0) yielded 0o33 from MDS
                 * instead of 0 (=L), corrupting the local frame base reg. */
                cpu->MemBase = (uint8_t)(((cpu->MemBX & 0x3) << 2) |
                                         (u->ff & 0x03));
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
                /* Diagnostic-muffler read (Various.mc SetDMuxAddress's
                 * trailing `T_ XTemp17_ ALUFMem`): when a DMux address
                 * was just loaded via MidasStrobe and not consumed by a
                 * UseDMD manifold write, this ALUFMem read returns the
                 * selected muffler bit in the SIGN, not the raw ALUFM
                 * word. InitMem.mc GetMemConfig reads DMux 0o1512/0o1511
                 * this way to size VirtualBanks. Intercept ONLY this
                 * genuine muffler read; normal Pd<-ALUFMRW/ALUFMem ALUFM
                 * access (Bootstrap/Initial/emulator startup) is
                 * untouched because dmux_pending is set only by a
                 * preceding SetDMuxAddress shift. */
                if (cpu->mem && dorado_memory_dmux_pending(cpu->mem)) {
                    pd = dorado_memory_dmux_read(cpu->mem);
                    return pd;
                }
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
                if (dorado_trace_flag("DORADO_FAULTREG_TRACE") &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "FAULTREG cyc=%llu task=%o pc=0o%o src=Pointers "
                            "pd=%04X ovf=%u und=%u mb=%02o rb=%02o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->ctask & 017, cpu->real_PC & 07777,
                            pd, cpu->stk_ovf & 1, cpu->stk_und & 1,
                            cpu->MemBase & 037, cpu->RBase & 017);
                }
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
            case 0: /* Pd ← ALU rsh 1 */
                /* HM p.22: logical right shift, 0 onto Pd[0].
                 * MesaInterrupt uses this to walk WDC's wakeup bits. */
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
            case 6: { /* Divide (HM p23): Pd <- ALU[1:15],,Q[0];
                       * Q <- Q[1:15],,ALUcarry. One non-restoring
                       * divide step: the double-length value shifts
                       * left through Pd and the quotient bit (the
                       * carry of the T-divisor ALU op) enters Q[15].
                       * AEmu's Nova DIV runs DivSub (Various.mc) on
                       * this function; with it stubbed every BCPL
                       * divide returned its input - InitPupLevel1's
                       * lenPup=(pupOvBytes+pupDataBytes)/2 stayed
                       * unhalved, 25 PBIs of 564 words exhausted
                       * NetExec's sysZone, and Title's display-line
                       * Allocate died with Alloc error 1801. */
                uint16_t q0 = (uint16_t)((cpu->Q >> 15) & 1);
                cpu->Q = (uint16_t)((cpu->Q << 1) | (alu_carry & 1));
                return (uint16_t)((alu << 1) | q0);
            }
            case 7: { /* CDivide: as Divide but Q[15] <- ALUcarry'. */
                uint16_t q0 = (uint16_t)((cpu->Q >> 15) & 1);
                cpu->Q = (uint16_t)((cpu->Q << 1) |
                                    ((alu_carry ^ 1) & 1));
                return (uint16_t)((alu << 1) | q0);
            }
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
            if (wake_immediate())
                cpu->wakeup_pending |= (uint16_t)(1u << task);
            else
                /* HM p27 2-cycle latency: stage, promote in task_schedule. */
                cpu->wakeup_pipe[0] |= (uint16_t)(1u << task);
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

/* Does this microinstruction consume Md? The Memory Section asserts Hold
 * when microcode reads Md before the fetch has landed, so the engine needs
 * to know which µinstrs touch Md to decide whether to Hold:
 *   - A bus  = Md  (ASEL=2, HM Table 8)
 *   - B bus  = Md  (BSEL=0, HM Table 7)
 *   - LC routes Md to T/RM/STK (LC 2,3,4,5 = T<-Md / RM<-Md, HM Table 10) */
static int uinstr_reads_md(const dorado_uinstr *u)
{
    if (u->asel == 2) return 1;
    if (u->bsel == 0) return 1;
    if (u->lc >= 2 && u->lc <= 5) return 1;
    return 0;
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
    case DM_REF_RMAP:      return "rmap";
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
    if ((off >= 0001u && off <= 0010u) ||
        (off >= 0420u && off <= 0440u) ||
        (off >= 0x0490u && off <= 0x0524u) ||
        (off >= 0x0F70u && off <= 0x1020u)) {
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

static int initmap_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_INITMAP_TRACE") ? 1 : 0;
    return cached;
}

static int map_cpu_trace_index(uint32_t idx)
{
    static int cached = -2;
    if (cached == -2) {
        const char *env = getenv("DORADO_MAP_TRACE_INDEX");
        if (env && env[0]) {
            char *end = NULL;
            unsigned long v = strtoul(env, &end, 0);
            cached = (end && *end == '\0') ? (int)(v & 0x3FFFu) : -1;
        } else {
            cached = -1;
        }
    }
    return cached >= 0 && idx == (uint32_t)cached;
}

static void latch_task_md_from_memory(dorado_cpu *cpu)
{
    if (cpu->mem) {
        int task = cpu->ctask & 0xF;
        cpu->task_md[task] = cpu->mem->md;
        cpu->task_md_valid[task] = 1;
        /* Md value is computed atomically, but the engine may not *consume*
         * it until the fetch latency has elapsed (Hold model). */
        cpu->task_md_ready[task] =
            cpu->cycles + (uint64_t)cpu->mem->last_ref_latency;
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
static uint16_t shifter_output(const dorado_cpu *cpu, const dorado_uinstr *u,
                               uint16_t *out_mask, uint16_t *out_fill)
{
    int rm_a   = rm_address(cpu, u);
    uint16_t r = (rm_a < CPU_RMSTK_INVALID) ? rm_stk_read(cpu, rm_a) : 0;
    uint16_t t = cpu->T;
    uint16_t sha, shb;
    int count, rmask_n, lmask_n;

    if (u->bsel & 4) {
        /* FF-controlled shift (HM §3.11 + p14): "BSEL = 4 to 7 will cause the
         * shifter controls to come directly from FF rather than from ShC."
         * BSEL[1:2] (the low two bits of BSEL) select the SHA/SHB source the
         * way ShC[2:3] do in the standard form: bit set => T, clear => RM/STK.
         * Confirmed against real microcode: in kernel.mb the Rlsh test (shift
         * RM/STK) compiles to BSEL=4 (0,,FF) and the Tlsh test (shift T) to
         * BSEL=7 (FF,,377); in BootstrapMain.mc `BTemp_ LDF[T,3,10]` (shift T)
         * is BSEL=7 and the following `BTemp_ LSH[BTemp,1]` (shift the RM reg
         * BTemp) is BSEL=4. The whole FF-controlled range is handled by this
         * one rule -- no per-opcode special cases. */
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

    if (dorado_trace_flag("DORADO_SHIFT_TRACE"))
        fprintf(stderr,
                "SHIFT pc=0o%o sha=%06o shb=%06o cnt=%d lm=%d rm=%d "
                "aluf=%02o ffc=%d out=%06o\n",
                cpu->real_PC, sha, shb, count, lmask_n, rmask_n,
                u->aluf, (u->bsel & 4) ? 1 : 0, lo16);

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

    /* HM §3.11: the shifter places the UNMASKED cycled word (complemented)
     * on the A bus; the LMask/RMask/Md merge happens in the Pd multiplexer
     * AFTER the ALU. Return the unmasked word for the A bus / ALU branch
     * conditions, and hand the mask + fill back so the caller can apply
     * them to Pd post-ALU. */
    if (out_mask) *out_mask = mask;
    if (out_fill) *out_fill = with_md ? task_md(cpu) : 0;
    return lo16;
}

/*
 * WF←A / RF←A: load ShC from a Mesa field descriptor (HM §3.8 "Shifter",
 * FF 5/5=WFfoA, 5/6=RFfoA). The descriptor is A[8:15] = (P<<4)|S where
 * P = position (A[8:11]) and S = size = width-1 (A[12:15]); ShC[2:3] (the
 * SHA/SHB T-vs-RM/STK selects) load straight from A[2:3].
 *
 * The exact bit transform is the "Shift Register Control" hardware on
 * ProcL sheet 18 (ProcL18.sil), which tabulates:
 *        Shift Count   Right Mask    Left Mask
 *   RF   P + S + 1     (don't care)  16 - S - 1
 *   WF   16 - P - S - 1 16 - P - S - 1  P
 * (16-x-1 = 15-x). ShC packing matches shifter_output(): C bit13=ShC[2],
 * bit12=ShC[3], bits[11:8]=count, bits[7:4]=RMask, bits[3:0]=LMask.
 */
static uint16_t field_desc_to_shc(uint16_t a, int is_write)
{
    unsigned P = (a >> 4) & 0xF;        /* A[8:11]  position */
    unsigned S = a & 0xF;               /* A[12:15] size = width-1 */
    unsigned shc23 = (a >> 12) & 3;     /* A[2:3] -> ShC[2:3] */
    unsigned count, lmask, rmask;
    if (is_write) {
        count = (15u - P - S) & 0xF;
        rmask = (15u - P - S) & 0xF;
        lmask = P & 0xF;
    } else {
        count = (P + S + 1u) & 0xF;
        rmask = 0;                      /* don't care for RF (ShiftLMask) */
        lmask = (15u - S) & 0xF;
    }
    return (uint16_t)((shc23 << 12) | (count << 8) | (rmask << 4) | lmask);
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
        case 1:
            if (io_task) return DM_REF_IOFETCH;
            /* Distinguish RMap← (read) from Map← (write). Both are the
             * ASEL=0/FF[0:1]=1 map reference; the read additionally
             * carries the ReadMap function (FA forced 0, FB=3, FC=1 — the
             * FF[2:7] subfield decodes to 0o31), e.g. Cedar's
             * NewReadMapPage `RMap_ RTemp0` (FF=0o131) vs InitMem's map
             * write `Map_ 0S, MapBuf_…` (FF=0o100). HM page 46-47: Map←
             * writes B/TIOA into the entry; RMap← only returns previous
             * contents in the pipe and must not modify the entry. */
            if (((u->ff >> 3) & 7) == 3 && (u->ff & 7) == 1)
                return DM_REF_RMAP;
            return DM_REF_MAP;
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
    case 7: { /* Shift. A bus = ~(unmasked cycled word) (low-true). The
               * mask/Md merge is deferred to the Pd mux (post-ALU) per
               * HM §3.11 so the ALU branch conditions see unmasked data. */
        uint16_t sh_mask = 0, sh_fill = 0;
        uint16_t sh = shifter_output(cpu, u, &sh_mask, &sh_fill);
        cpu->shift_active = 1;
        cpu->shift_mask = sh_mask;
        cpu->shift_fill = sh_fill;
        *out = (uint16_t)~sh;
        return 0;
    }
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
                       int carry20,
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
        uint8_t low_carry = (uint8_t)(((a & 0xFu) + carry_in) >> 4);
        uint32_t r = (uint32_t)a + carry_in;
        if (carry20 && !low_carry) r += 0x10u;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        /* HM §3.7 Overflow for all arithmetic ops (QW7): same-sign
         * operands yielding a different-sign result. The increment
         * (carry_in, 0/1) has sign bit 0. */
        uint16_t sa = a & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == 0 && sa != sr) ? 1 : 0;
        break;
    }
    case 006: { /* 2*A (+1 if carry_in) */
        uint8_t low_carry = (uint8_t)((((a & 0xFu) << 1) + carry_in) >> 4);
        uint32_t r = (uint32_t)a + a + carry_in;
        if (carry20 && !low_carry) r += 0x10u;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        /* 2A = A+A: both addends share A's sign (QW7). */
        uint16_t sa = a & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa != sr) ? 1 : 0;
        break;
    }
    case 014: { /* A + B (+1 if carry_in) */
        uint8_t low_carry = (uint8_t)(((a & 0xFu) + (b & 0xFu) + carry_in) >> 4);
        uint32_t r = (uint32_t)a + b + carry_in;
        if (carry20 && !low_carry) r += 0x10u;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        /* signed-overflow: HM §3.7 "Overflow ... iff a signed
         * arithmetic operation yields an incorrect result" */
        uint16_t sa = a & 0x8000, sb = b & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == sb && sa != sr) ? 1 : 0;
        break;
    }
    case 022: { /* A - B - 1 + carry_in (so carry=1 → A-B) */
        uint8_t low_carry = (uint8_t)(((a & 0xFu) + ((uint16_t)~b & 0xFu) + carry_in) >> 4);
        uint32_t r = (uint32_t)a + (uint16_t)~b + carry_in;
        if (carry20 && !low_carry) r += 0x10u;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        uint16_t sa = a & 0x8000, sb = (~b) & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == sb && sa != sr) ? 1 : 0;
        break;
    }
    case 036: { /* A - 1 (+ carry_in) */
        uint8_t low_carry = (uint8_t)(((a & 0xFu) + 0xFu + carry_in) >> 4);
        uint32_t r = (uint32_t)a + 0xFFFF + carry_in;
        if (carry20 && !low_carry) r += 0x10u;
        result = (uint16_t)r;
        carry_out = (r >> 16) & 1;
        /* A + (-1): second addend 0xFFFF has sign bit set (QW7). */
        uint16_t sa = a & 0x8000, sr = (uint16_t)r & 0x8000;
        overflow = (sa == 0x8000 && sa != sr) ? 1 : 0;
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
            /* Emulator: Reschedule branch condition — HIGH-true,
             * unlike the non-emulator IOAtten' (HM Table 13 names it
             * "IOAtten' (non-emulator) or ReSchedule (emulator)" —
             * only IOAtten carries the prime). Set by the Reschedule
             * FF, cleared only by NoReschedule; RescheduleNow does
             * not affect it. AEmu's BLT exit test depends on the
             * sense: inverted, every BLT word bails to
             * AEmuReschedule and the sweep never advances. */
            r = cpu->reschedule_cond ? 1 : 0;
        } else {
            /* Level sampled at instruction issue (see execute_uinstr):
             * a same-instruction Pd←Input must not shift the word
             * IOAtten refers to. */
            r = cpu->io_atten_at_issue ? 0 : 1;
        }
        break;
    case 7:
        /* Overflow' (HM Table 13 FF=067). The branch-condition RAM
         * latches the low-true overflow signal from the ALU board; the
         * Cedar Mesa jump microcode names this condition `Overflow'` and
         * depends on the no-overflow case selecting the true branch in the
         * signed-compare repair path. Keep cpu->alu_overflow itself
         * positive-true for diagnostics/tests, but expose the primed
         * hardware branch condition here. */
        r = cpu->alu_overflow ? 0 : 1;
        break;
    default: r = 0;                                       break;
    }
    if (dorado_trace_flag("DORADO_COND_TRACE") &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
        uint16_t rv = (rm_a < CPU_RMSTK_INVALID) ? rm_stk_read(cpu, rm_a) : 0;
        fprintf(stderr,
                "COND cyc=%llu pc=0o%o cond=%d r=%d alu0=%u alu<0=%u "
                "carry=%u ovf=%u cnt=%06o rm_a=%03o rv=%06o stkp=%03o\n",
                (unsigned long long)dorado_trace_cycle, cpu->real_PC,
                cond & 7, r, cpu->alu_zero, cpu->alu_lt0,
                cpu->alu_carry, cpu->alu_overflow, cpu->Cnt,
                rm_a & 0377, rv, cpu->StkP & 0377);
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
    /* notLength (mdfields.d TIFUM): the 2-bit field holds the 1's
     * complement of the opcode length, i.e. notLength = ~length & 3.
     * So length = ~notLength & 3: notLength 2→len 1, 1→len 2, 0→len 3
     * (3 unused). The earlier `lpr+1` form was only correct for the
     * length-2 case (notLength=1), which is why it survived the Alto
     * gate (Alto/Nova opcodes are 2 bytes); it mis-sized every length-1
     * and length-3 opcode — fatal for the PrincOps/Mesa instruction set
     * (e.g. LFC4/NOOP are 1 byte), where the over-long length makes ←Id
     * read α as the "instruction length" and corrupts the saved Mesa PC
     * in SavePCInFrame. */
    uint8_t lpr = (uint8_t)((lh >> 10) & 3);
    cpu->ifu_length     = (uint8_t)((~lpr) & 3);
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
    /* RBaseB' is low-true; RBase ← RBaseB. Recorded separately:
     * execute_uinstr saves/restores RBase around next_pc for the
     * at-issue branch semantics, which would silently wipe this
     * load (RCLK exits with RBase[RTClock]; the next opcode's CRY
     * writes then landed on RTCDeltaLo, zeroing it and turning on
     * the junk task's PC-sampling memory spray). */
    cpu->RBase = !((lh >> 9) & 1);
    cpu->ifu_dispatch_rbase = (int16_t)cpu->RBase;
}

/* Decode an IFUM entry's address half (IFUMRH, HM Table 20):
 *   bit 5 = Packed-α
 *   bits 6:15 = IFaddr' (10 bits, LOW-TRUE)
 * Returns the true entry-vector base IFaddr = ~IFaddr'. Verified
 * against AEmu.mb: opcode 045 (LDAipc) stores RH=0o1772; ~0o1772 &
 * 0x3FF = 5, and LDAIPC's entry vector is at IM[5*4] = 0o24. Sets
 * ifu_packed_a. */
static uint16_t ifu_decode_rh(dorado_cpu *cpu, uint16_t rh)
{
    cpu->ifu_packed_a = (rh >> 10) & 1;
    /* IFaddr' bits 6:15 (MSB) = bits 9..0 (LSB), inverted. */
    return (uint16_t)(~rh & 0x3FF);
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
static uint16_t ifu_id_at(const dorado_cpu *cpu, uint8_t cnt)
{
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

/* Deliver the next ←Id operand byte and advance the cursor. */
static uint16_t ifu_consume_id(dorado_cpu *cpu)
{
    return ifu_id_at(cpu, cpu->ifu_idcnt++);
}

/* Peek the current ←Id operand byte WITHOUT advancing. Used by IFetch
 * (HM page 38: "the IFU does not advance to the next item of ←Id for
 * IFetch←") and by the TisId/RisId bus substitution, which reference the
 * same Id item the accompanying advance consumes. */
static uint16_t ifu_peek_id(const dorado_cpu *cpu)
{
    return ifu_id_at(cpu, cpu->ifu_idcnt);
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
             * IFUJump (past NotReady). Note it does NOT return here:
             * "The trap instruction is executed as though it were the
             * first instruction of the rescheduled opcode, and <-Id
             * and IFUJump will work as though that opcode were in
             * progress" — so the IFU must still consume the next
             * opcode (advance PCX/PCF, latch operands) exactly as for
             * a normal dispatch; only the dispatch TARGET is replaced
             * by the trap vector. AEmuReschedule then resumes via
             * T_ NOT(PCX') -> PCF, re-entering the held-back opcode. */
            int resched_trap = 0;
            if (cpu->reschedule_pending > 0) {
                if (resched_trace_enabled() &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "RESCHED_IFU cyc=%llu pc=0o%o pcf=0o%o "
                            "pending_before=%u cond=%u "
                            "rm000=%06o rm006=%06o rm025=%06o rm026=%06o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->real_PC & 07777, cpu->ifu_pcf & 0177777,
                            cpu->reschedule_pending, cpu->reschedule_cond,
                            cpu->RM[DORADO_RM_CAND_000] & 0177777,
                            cpu->RM[DORADO_RM_CAND_006] & 0177777,
                            cpu->RM[DORADO_RM_CAND_025] & 0177777,
                            cpu->RM[DORADO_RM_CAND_026] & 0177777);
                }
                cpu->reschedule_pending--;
                if (cpu->reschedule_pending == 0) resched_trap = 1;
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

            if (dorado_trace_flag("DORADO_IFUDISP_TRACE") &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                fprintf(stderr,
                        "IFUDISP pc=0o%o pcf=0o%o br31=%05X op=%03o "
                        "len=%u alpha=%03o beta=%03o flt=%d n=%d "
                        "insset=%u rh=%06o lh=%06o vec=0o%o "
                        "pc_after=0o%o stkp=%03o "
                        "acs=%06o,%06o,%06o,%06o "
                        "aacs=%06o,%06o,%06o,%06o rtrap=%d\n",
                        cpu->real_PC, cpu->ifu_pcf,
                        cpu->mem ? dorado_br_get(cpu->mem, 31) : 0,
                        opcode, length, cpu->ifu_alpha, cpu->ifu_beta,
                        fetch_faulted, n_slot, cpu->ifu_insset & 3,
                        rh, lh, (unsigned)(ifaddr << 2), pc_after,
                        cpu->StkP & 0xFF,
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[(cpu->StkP + 1) & 0xFF],
                        cpu->STK[(cpu->StkP + 2) & 0xFF],
                        cpu->STK[(cpu->StkP + 3) & 0xFF],
                        /* aacs: the FIXED Alto AC window AC0..3 = STK[1..4]
                         * (Start.mc). The StkP-relative acs above is wrong
                         * for comparison once StkP moves off 1. */
                        cpu->STK[1], cpu->STK[2], cpu->STK[3], cpu->STK[4],
                        /* rtrap=1: this IFUJump traps to AEmuReschedule
                         * (the held-back opcode is re-dispatched next, not
                         * executed here). A plain Alto has no such trap, so
                         * differential tools must NOT count this as an
                         * executed opcode. */
                        resched_trap);
            }
            /* Defer a cycle-aligned per-opcode AC dump to after apply_lc()
             * (the writeback that finishes the opcode whose IFUJump this is). */
            if (dorado_trace_flag("DORADO_ALTOAC_TRACE") &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                cpu->altoac_pending = 1;
                cpu->altoac_op = opcode;
                cpu->altoac_alpha = cpu->ifu_alpha;
                cpu->altoac_insset = cpu->ifu_insset & 3;
            }
            if (opcode == 0365 && cpu->mem &&
                dorado_trace_flag("DORADO_BITBLT_TRACE") &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                uint32_t mds = dorado_br_get(cpu->mem, 036);
                fprintf(stderr,
                        "BITBLT_DISP cyc=%llu pc=0o%o pcf=0o%o "
                        "br30=0o%o br31=0o%o br36=0o%o mb=%02o rb=%02o "
                        "stkp=%03o acs=%06o,%06o,%06o,%06o,%06o,%06o\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->real_PC, cpu->ifu_pcf,
                        dorado_br_get(cpu->mem, 30),
                        dorado_br_get(cpu->mem, 31), mds,
                        cpu->MemBase & 037, cpu->RBase & 017,
                        cpu->StkP & 0xFF,
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[(cpu->StkP + 1) & 0xFF],
                        cpu->STK[(cpu->StkP + 2) & 0xFF],
                        cpu->STK[(cpu->StkP + 3) & 0xFF],
                        cpu->STK[(cpu->StkP + 4) & 0xFF],
                        cpu->STK[(cpu->StkP + 5) & 0xFF]);

                for (int s = 0; s < 4; s++) {
                    uint16_t ptr = cpu->STK[(cpu->StkP + s) & 0xFF];
                    uint32_t va = (mds + ptr) & 0x0FFFFFFFu;
                    fprintf(stderr, "BITBLT_BBT stk+%d ptr=0o%o va=0o%o:",
                            s, ptr, va);
                    for (int i = 0; i < 12; i++) {
                        fprintf(stderr, " %06o",
                                dorado_visible_word_at_va(cpu->mem,
                                                          va + (uint32_t)i));
                    }
                    fprintf(stderr, "\n");
                }
                fprintf(stderr, "BITBLT_LOW @0o426:");
                for (uint32_t i = 0; i < 24; i++) {
                    fprintf(stderr, " %06o",
                            dorado_visible_word_at_va(cpu->mem, 0426u + i));
                }
                fprintf(stderr, "\n");
            }
            if (opcode == 0364 && cpu->ifu_alpha == 0011 &&
                dorado_trace_flag("DORADO_RCLK_TRACE") &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                fprintf(stderr,
                        "RCLK_DISP cyc=%llu pc=0o%o pcf=0o%o "
                        "rtclock=%06o rtc_delta=%06o rtc430=%06o "
                        "stkp=%03o acs=%06o,%06o,%06o,%06o\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->real_PC, cpu->ifu_pcf,
                        cpu->RM[0260], cpu->RM[0261], cpu->RM[0262],
                        cpu->StkP & 0xFF,
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[(cpu->StkP + 1) & 0xFF],
                        cpu->STK[(cpu->StkP + 2) & 0xFF],
                        cpu->STK[(cpu->StkP + 3) & 0xFF]);
            }
            if (opcode == 0364 && cpu->ifu_alpha == 0255 &&
                timer_trace_enabled() &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                fprintf(stderr,
                        "SETIT_DISP cyc=%llu pc=0o%o pcf=0o%o "
                        "br31=%05X rtclock=%06o wake=%06o rtc430=%06o "
                        "stkp=%03o acs=%06o,%06o,%06o,%06o\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->real_PC, cpu->ifu_pcf,
                        cpu->mem ? dorado_br_get(cpu->mem, 31) : 0,
                        cpu->RM[DORADO_RM_RT_CLOCK],
                        cpu->RM[DORADO_RM_WAKEUP_TIME],
                        cpu->RM[DORADO_RM_RTC_430],
                        cpu->StkP & 0xFF,
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[(cpu->StkP + 1) & 0xFF],
                        cpu->STK[(cpu->StkP + 2) & 0xFF],
                        cpu->STK[(cpu->StkP + 3) & 0xFF]);
            }
            if (opcode == 0364 && cpu->ifu_alpha == 0010 &&
                dorado_trace_flag("DORADO_SETMP_TRACE") &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                uint32_t mds = cpu->mem ? dorado_br_get(cpu->mem, 036) : 0;
                fprintf(stderr,
                        "SETMP_DISP cyc=%llu pc=0o%o pcf=0o%o "
                        "br31=%05X stkp=%03o code=%06o "
                        "acs=%06o,%06o,%06o,%06o\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->real_PC, cpu->ifu_pcf,
                        cpu->mem ? dorado_br_get(cpu->mem, 31) : 0,
                        cpu->StkP & 0xFF,
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[cpu->StkP & 0xFF],
                        cpu->STK[(cpu->StkP + 1) & 0xFF],
                        cpu->STK[(cpu->StkP + 2) & 0xFF],
                        cpu->STK[(cpu->StkP + 3) & 0xFF]);
                if (dorado_trace_flag("DORADO_SETMP_TRACE_DETAIL")) {
                    fprintf(stderr,
                            "SETMP_DETAIL cyc=%llu MDS=0o%o MemBase=%02o "
                            "RBase=%02o T=%06o Q=%06o Cnt=%06o Link=0o%o "
                            "BR[030]=0o%o BR[031]=0o%o "
                            "BR[036]=0o%o BR[037]=0o%o STK:",
                            (unsigned long long)dorado_trace_cycle,
                            mds, cpu->MemBase & 037, cpu->RBase & 017,
                            cpu->T, cpu->Q, cpu->Cnt, cpu->Link,
                            cpu->mem ? dorado_br_get(cpu->mem, 030) : 0,
                            cpu->mem ? dorado_br_get(cpu->mem, 031) : 0,
                            mds,
                            cpu->mem ? dorado_br_get(cpu->mem, 037) : 0);
                    for (int i = -8; i <= 8; i++) {
                        uint8_t sp = (uint8_t)(cpu->StkP + i);
                        fprintf(stderr, " [%03o]=%06o", sp, cpu->STK[sp]);
                    }
                    fprintf(stderr, "\n");
                    if (cpu->mem) {
                        uint16_t lf = cpu->STK[(cpu->StkP + 1) & 0xFF];
                        uint32_t lfv = (mds + lf) & 0x0FFFFFFFu;
                        fprintf(stderr, "SETMP_LF @0o%o:", lfv);
                        for (uint32_t i = 0; i < 16; i++) {
                            fprintf(stderr, " %06o",
                                    dorado_visible_word_at_va(cpu->mem,
                                                              lfv + i));
                        }
                        fprintf(stderr, "\n");
                    }
                }
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

            /* A real opcode dispatch is now committed (past NotReady /
             * warmup / fault). Count it so differential harnesses can
             * detect the one-opcode boundary. RestoreStkP must see the
             * stack state handed to the dispatched opcode, including this
             * IFUJump instruction's own RSTK update; defer the capture
             * until after stk_apply_post(). */
            cpu->ifu_save_stkp_pending = 1;
            cpu->ifu_dispatch_count++;

            /* Compute TNIA: TNIA[4:13] = IFaddr', TNIA[14:15] = n.
             * Conditional IFUJump OR's the condition into TNIA[15],
             * routing to entry n|1 of the vector. */
            int n_eff = n_slot | (cond_true ? 1 : 0);
            uint16_t tnia = (uint16_t)((ifaddr << 2) | n_eff);
            if (resched_trap) {
                tnia = ifu_trap_addr(0014, n_slot, cpu->ifu_insset);
                if (resched_trace_enabled() &&
                    (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                    fprintf(stderr,
                            "RESCHED_TRAP cyc=%llu pc=0o%o tnia=0o%o "
                            "insset=%u n=%d "
                            "rm000=%06o rm006=%06o rm025=%06o rm026=%06o\n",
                            (unsigned long long)dorado_trace_cycle,
                            cpu->real_PC & 07777, tnia & 07777,
                            cpu->ifu_insset & 3, n_slot,
                            cpu->RM[DORADO_RM_CAND_000] & 0177777,
                            cpu->RM[DORADO_RM_CAND_006] & 0177777,
                            cpu->RM[DORADO_RM_CAND_025] & 0177777,
                            cpu->RM[DORADO_RM_CAND_026] & 0177777);
                }
            }

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
                /* HM §4.8: "The Link register itself is smashed with CIA+1."
                 * The IM write address came from link_at_issue, so the old
                 * Link is no longer needed. */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
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
                /* HM §4.8: Link is smashed with CIA+1; the ~tpc data rides
                 * the alternate B←Link path for the next cycle. (The
                 * microcode's `B←Link, T←~B` reads it there.) */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
                cpu->b_link_read = (uint16_t)~tpc;
                cpu->b_link_read_valid = 1;
                cpu->b_link_read_age = 0;
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 5) {
                /* LdTPC←B (HM page 34): write task_tpc[B[12:15]]
                 * from Link. "Any pending dispatch conditions for
                 * that task are cleared" (HM p32 note). */
                uint16_t b = 0;
                if (!ff_override_b(cpu, u, &b)) {
                    int rc = b_bus(cpu, u, &b);
                    if (rc != 0) return rc;
                }
                int task = b & 0xF;
                cpu->task_dispatch[task] = 0;
                /* "An attempt to set TPC of the running task is
                 * unpredictable" — we just no-op the write in that
                 * case; for other tasks, write the saved-TPC slot
                 * so when we switch to that task it picks up there. */
                if (task != cpu->ctask) {
                    cpu->task_tpc[task] = cpu->Link;
                    if (dorado_trace_flag("DORADO_TASK_TRACE") &&
                        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
                        fprintf(stderr,
                                "TPC_WRITE cyc=%llu task=%o value=0o%o "
                                "pc=0o%o ctask=%o\n",
                                (unsigned long long)dorado_trace_cycle,
                                task & 017, cpu->Link & 07777,
                                cpu->real_PC & 07777, cpu->ctask & 017);
                    }
                }
                /* HM §4.8: Link is smashed with CIA+1 after the TPC write
                 * (the write above consumed the old Link). */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
                *next = (uint16_t)(cpu->real_PC + 1);
                return 0;
            }
            if (fn == 6) {
                /* Read IM (HM §4.8). Stubbed: real implementation
                 * reads IM[Link[7:15]] byte-by-byte selected by
                 * RSTK[2:3], placing inverted data on the alternate
                 * B←Link path. Not yet exercised by tests. Link is
                 * smashed with CIA+1 like the other IM/TPC accesses. */
                cpu->Link = (uint16_t)(cpu->real_PC + 1);
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

    if (dorado_trace_flag("DORADO_PCDIS") && (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate)) {
        static long lo = -2, hi = 0, count = 0, limit = -2;
        if (lo == -2) { const char *w = getenv("DORADO_PCDIS");
                        lo = 0; hi = 0; if (w) sscanf(w, "%lo,%lo", &lo, &hi); }
        if (limit == -2) { const char *w = getenv("DORADO_PCDIS_LIMIT");
                           limit = w ? strtol(w, NULL, 0) : 0; }
        if ((long)cpu->real_PC >= lo && (long)cpu->real_PC <= hi) {
            if (limit <= 0 || count < limit) {
                char buf[160];
                dorado_format(u, buf, sizeof buf);
                fprintf(stderr,
                        "PCDIS cyc=%llu task=%o pc=%04o pcf=0o%o "
                        "rb=%02o mb=0o%x stkp=%03o T=%06o Q=%06o "
                        "rm000=%06o rm006=%06o rm025=%06o rm026=%06o : %s\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->ctask & 017, cpu->real_PC,
                        cpu->ifu_pcf & 0177777, cpu->RBase & 017,
                        cpu->MemBase & 0xf, cpu->StkP & 0377,
                        cpu->T & 0177777, cpu->Q & 0177777,
                        cpu->RM[DORADO_RM_CAND_000] & 0177777,
                        cpu->RM[DORADO_RM_CAND_006] & 0177777,
                        cpu->RM[DORADO_RM_CAND_025] & 0177777,
                        cpu->RM[DORADO_RM_CAND_026] & 0177777,
                        buf);
            }
            count++;
        }
    }

    if (dorado_trace_flag("DORADO_ALLOC_TRACE") &&
        (cpu->real_PC == 03301 || cpu->real_PC == 03302 ||
         cpu->real_PC == 01071 || cpu->real_PC == 01072) &&
        (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate)) {
        static long alloc_trace_count = 0;
        static long alloc_trace_limit = -2;
        if (alloc_trace_limit == -2) {
            const char *w = getenv("DORADO_ALLOC_TRACE_LIMIT");
            alloc_trace_limit = w ? strtol(w, NULL, 0) : 256;
        }
        if (alloc_trace_limit < 0 || alloc_trace_count++ < alloc_trace_limit) {
            uint32_t br = cpu->mem ? dorado_br_get(cpu->mem, cpu->MemBase & 037) : 0;
            fprintf(stderr,
                    "ALLOC_TRACE cyc=%llu task=%o pc=0o%o T=0o%o Q=0o%o "
                    "md=0o%o mar=0o%o va=0o%o mb=0o%o br=0o%o "
                    "br3=0o%o br36=0o%o rb=0o%o stkp=%03o\n",
                    (unsigned long long)dorado_trace_cycle, cpu->ctask & 017,
                    cpu->real_PC, cpu->T & 0177777, cpu->Q & 0177777,
                    cpu->mem ? (cpu->mem->md & 0177777) : 0,
                    cpu->mem ? (cpu->mem->mar & 0x0FFFFFFFu) : 0,
                    cpu->mem ? ((br + cpu->mem->mar) & 0x0FFFFFFFu) : 0,
                    cpu->MemBase & 037, br,
                    cpu->mem ? dorado_br_get(cpu->mem, 3) : 0,
                    cpu->mem ? dorado_br_get(cpu->mem, 036) : 0,
                    cpu->RBase & 017, cpu->StkP & 0377);
        }
    }

    if (cpu->mc->im_breakpoint[cpu->real_PC]) {
        cpu->halted = 1;
        cpu->halt_reason = CPU_HALT_BREAKPOINT;
        return 1;
    }

    return execute_uinstr(cpu, u, 1 /* from IM */);
}

static int desc_trace_pc(uint16_t pc)
{
    switch (pc) {
    case 01264: case 01265: case 01300:
    case 03210: case 03253: case 03256:
    case 03266: case 03267: case 03271:
    case 03272: case 03273: case 03275:
    case 03276: case 03277:
    case 03301: case 03302: case 03304:
    case 03306: case 03307: case 03311:
    case 03312: case 03314:
        return 1;
    default:
        return 0;
    }
}

static int check_trace_pc(uint16_t pc)
{
    switch (pc) {
    case 00414: case 00530: case 00600: case 00725:
    case 01065: case 01400:
    case 01501: case 01502: case 01503: case 01504:
    case 01505: case 01506: case 01507: case 01510:
    case 01511: case 01512:
    case 03211: case 03212: case 03213: case 03216:
    case 03317: case 03322: case 03323: case 03326:
    case 03327: case 03331: case 03332: case 03335:
    case 03336:
    case 03422: case 03423: case 03424: case 03425:
    case 03426: case 03427: case 03430: case 03431:
    case 03432: case 03433: case 03434: case 03435:
    case 03441: case 03442: case 03445:
    case 03453: case 03455:
        return 1;
    default:
        return 0;
    }
}

static int execute_uinstr(dorado_cpu *cpu, const dorado_uinstr *u, int from_im)
{
    dorado_mem_trace_pc = cpu->real_PC;
    dorado_mem_trace_pcx = cpu->ifu_pcx;
    dorado_mem_trace_br31 = cpu->mem ? (int)dorado_br_get(cpu->mem, 31) : 0;
    dorado_mem_trace_op = ((int)cpu->ifu_opcode << 8) | cpu->ifu_alpha;

    /* Trace the ethernet tasks' microcode PC (EOT=6, EIT=7) -- diagnostic for
     * the EOT transmit-completion path (why a deferred completion diverts the
     * EOT to EXINIT/idle instead of its SendEOP->poll->EPOST post path; see
     * docs/ethernet-faithful-receiver.md). Gated by DORADO_EOT_PC_TRACE + the
     * cycle window so it stays quiet by default. */
    if ((cpu->ctask == 6 || cpu->ctask == 7) &&
        dorado_trace_flag("DORADO_EOT_PC_TRACE") &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
        fprintf(stderr, "ETPC cyc=%llu t=%o pc=0o%o\n",
                (unsigned long long)dorado_trace_cycle, cpu->ctask,
                cpu->real_PC & 07777);
    }

    /* Memory Hold (HM §5 pp.41-42; docs/memory-architecture.md). If this
     * microinstruction consumes Md before the fetch's latency has elapsed,
     * the Memory Section asserts Hold. The Hold *signal* is what HM §4.11's
     * event counter counts (eventCounters2.mc eventHold counts MDI holds), and
     * it asserts independently of mcr.disHold — so flag it for the counters
     * even when disHold suppresses the stall. (Harmless for games/boot: the
     * counters only tick when a diagnostic has enabled them.) */
    int md_not_ready =
        from_im && cpu->mem && uinstr_reads_md(u) &&
        (uint64_t)cpu->cycles < cpu->task_md_ready[cpu->ctask & 0xF];
    if (md_not_ready) cpu->evc_events |= EVC_EV_HOLD;
    /* The engine's *response* to Hold — freeze (convert to jump-to-self this
     * cycle, let a higher-priority task run, re-run when next selected) — is
     * suppressed when mcr.disHold is set (boot/init microcode sets it); that
     * gate is real hardware behavior, not an emulator approximation. */
    if (md_not_ready && !dorado_mcr_dishold(cpu->mem)) {
        cpu->real_PC = task_schedule(cpu, cpu->real_PC, 0);
        cpu->cycles++;
        if (cpu->baseboard && cpu->baseboard_cycles_per_uop > 0)
            baseboard_run(cpu->baseboard, cpu->baseboard_cycles_per_uop);
        return 0;
    }

    /* BBT dump (env DORADO_BBT_TRACE): at AEmu BitBltA (real 0o13124)
     * the BitBlt-table pointer is AC2 = STK[StkP+2]; dump the table
     * (DBCA=w2, DBMR=w3, DLX=w4, DTY=w5, DW=w6, DH=w7) from the MDS bank
     * so a corrupting BitBlt can be compared against a healthy one. */
    if (cpu->real_PC == 03124 && cpu->ctask == 0 && cpu->mem &&
        dorado_trace_flag("DORADO_BBT_TRACE")) {
        uint16_t bbt = cpu->STK[(cpu->StkP + 2) & 0xFF];
        uint32_t mds = dorado_br_get(cpu->mem, 036);
        uint16_t w[8];
        for (int i = 0; i < 8; i++)
            w[i] = dorado_visible_word_at_va(cpu->mem,
                       (mds + bbt + (uint32_t)i) & 0x0FFFFFFFu);
        fprintf(stderr,
            "BBT cyc=%llu bbt=0o%o func=0o%o DBCA=0o%o DBMR=0o%o "
            "DLX=0o%o DTY=0o%o DW=0o%o DH=0o%o\n",
            (unsigned long long)dorado_trace_cycle, bbt, w[0],
            w[2], w[3], w[4], w[5], w[6], w[7]);
    }

    /* Route B diagnostic: trace the Mesa-VM XFER/trap path. Gated on
     * DORADO_XFER_TRACE + the cycle window (DORADO_TRACE_GATE) + task 0,
     * so it is inert for the Alto worlds and normal runs. Prints the
     * real PC plus the memory state (last-ref VA + MD) so the SD/GFT/
     * frame fetches in Xfer are visible. */
    if (dorado_trace_gate && dorado_trace_flag("DORADO_XFER_TRACE") && cpu->ctask == 0) {
        fprintf(stderr,
                "XFER cyc=%llu pc=0o%o iset=%u T=0o%o Q=0o%o "
                "md=0o%o lva=0o%o mb=0o%o rb=%02o\n",
                (unsigned long long)dorado_trace_cycle, cpu->real_PC,
                cpu->ifu_insset & 3, cpu->T, cpu->Q,
                cpu->mem ? cpu->mem->md : 0,
                cpu->mem ? (unsigned)cpu->mem->last_ref_va : 0,
                cpu->MemBase & 037, cpu->RBase & 017);
    }

    /* Route B / disk D4 diagnostic: trace the DSK task's (14₈) microcode PC,
     * so PilotDisk.mc's idle/seek/WaitForSector loop is visible under
     * --disk-real. Gated on DORADO_DSK_PC_TRACE (+ optional cycle window). */
    if (dorado_trace_flag("DORADO_DSK_PC_TRACE") && cpu->ctask == 014) {
        fprintf(stderr, "DSKPC pc=0o%o T=0o%o Q=0o%o md=0o%o lva=0o%o mb=0o%o\n",
                cpu->real_PC, cpu->T, cpu->Q,
                cpu->mem ? cpu->mem->md : 0,
                cpu->mem ? (unsigned)cpu->mem->last_ref_va : 0,
                cpu->MemBase & 037);
    }
    /* Which task executes the boot-transfer region? (DORADO_BOOTXFER_TRACE) */
    if (dorado_trace_flag("DORADO_BOOTXFER_TRACE") &&
        cpu->real_PC >= 07000 && cpu->real_PC <= 07015) {
        fprintf(stderr, "BOOTXFER task=%o pc=0o%o T=0o%o md=0o%o lva=0o%o mb=0o%o\n",
                cpu->ctask, cpu->real_PC, cpu->T,
                cpu->mem ? cpu->mem->md : 0,
                cpu->mem ? (unsigned)cpu->mem->last_ref_va : 0,
                cpu->MemBase & 037);
    }

    /* Snapshot Link before FF can modify it. Write IM (in next_pc) and
     * Subroutine Return both consume Link at instruction-issue time;
     * B←RWCPReg / Link←B / B-dispatch all overwrite Link via FF. The
     * pipelined hardware reads Link at t1 and writes at t3, so the
     * consumer sees the *old* value while the new value lands for the
     * next instruction. We model this by capturing the entry value. */
    if (dorado_trace_gate && dorado_trace_flag("DORADO_RBASE_TRACE")) {
        static uint8_t prev_rb = 0xFF;
        static int prev_task = -1;
        if (cpu->ctask == 0 &&
            (cpu->RBase != prev_rb || prev_task != 0)) {
            fprintf(stderr, "RBASE task=0 pc=0o%o rb=%02o\n",
                    cpu->real_PC, cpu->RBase & 017);
            prev_rb = (uint8_t)cpu->RBase;
        }
        prev_task = cpu->ctask;
    }

    int stk_underflow = 0, stk_overflow = 0;
    if (stk_error_check(cpu, u, &stk_underflow, &stk_overflow)) {
        stk_signal_error(cpu, stk_underflow, stk_overflow);
        if (dorado_trace_flag("DORADO_STKP_TRACE") &&
            (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
            fprintf(stderr,
                    "STK_ERROR_HOLD cyc=%llu pc=0o%o rstk=%02o "
                    "stkp=%03o und=%d ovf=%d\n",
                    (unsigned long long)dorado_trace_cycle,
                    cpu->real_PC, u->rstk & 017, cpu->StkP & 0377,
                    stk_underflow, stk_overflow);
        }

        cpu->prev_PC = cpu->real_PC;
        if (from_im) junk_timer_tick(cpu);

        cpu->real_PC = task_schedule(cpu, cpu->real_PC, 0);

        if (from_im) {
            cpu->cycles++;
            if (cpu->mem) dorado_memory_tick(cpu->mem);
            if (cpu->baseboard && cpu->baseboard_cycles_per_uop > 0) {
                baseboard_run(cpu->baseboard,
                              cpu->baseboard_cycles_per_uop);
            }
        }

        if (cpu->ifu_active && cpu->ifu_warmup > 0) {
            cpu->ifu_warmup--;
        }
        return 0;
    }

    cpu->link_at_issue = cpu->Link;
    /* Age the alternate B←Link latch: it is valid only for the single
     * instruction following the RdTPC/Read IM that set it. If that
     * instruction did not consume it (no B←Link), expire it now. */
    if (cpu->b_link_read_valid) {
        if (cpu->b_link_read_age) cpu->b_link_read_valid = 0;
        else cpu->b_link_read_age = 1;
    }
    cpu->dispatch_or = cpu->task_dispatch[cpu->ctask];
    cpu->task_dispatch[cpu->ctask] = 0;
    uint16_t md_at_issue = task_md(cpu);
    uint8_t rbase_at_issue = cpu->RBase;
    /* Memory references select their BR from the t1 (at-issue)
     * MemBase; a same-instruction FF MemBase change (FlipMemBase is
     * the reachable one on refs) lands at t3 for LATER instructions.
     * AEmu's JSR-indirect `Fetch_ T, FlipMemBase` fetches the pointer
     * via MDS and flips to CODE for the following BrLo_ T. */
    uint8_t membase_at_issue = (uint8_t)(cpu->MemBase & 0x1F);
    /* IOAttention is a level tested at t1, BEFORE this instruction's
     * Pd←Input consumes the bus-register word (HM page 30; §11: "IOAtten
     * branches when a status word is present in the receiver bus
     * register"). AltoEther.mc relies on both phases: `EITemp2_ Input,
     * Branch[EIPLZ, IOAtten]` tests the word being read, while the
     * store in the input main loop tests the word the NEXT Input will
     * read. Snapshot with the at-issue TIOA. */
    cpu->io_atten_at_issue = (cpu->io && cpu->ctask != 0)
        ? (uint8_t)dorado_io_attention(cpu->io, cpu->ctask,
                                       (uint8_t)cpu->TIOA)
        : 0;
    if (dorado_trace_flag("DORADO_ATTEN_TRACE") && cpu->ctask == 7 &&
        cpu->TIOA == 015) {
        fprintf(stderr, "ATTEN pc=0o%o atten=%d\n",
                cpu->real_PC, cpu->io_atten_at_issue);
    }

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
    cpu->shift_active = 0;   /* set by a_bus() only for a barrel shift */
    if ((rc = a_bus(cpu, u, &a)) != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }
    /* HM Table 11a (FA=0 FB=0/1): A[12:15] ← FF[4:7] (gap B6).
     * The override fires only when FF is interpreted as a function
     * (ASEL > 3, BSEL not constant, JCN not long). */
    int a_is_rmstk_default = 0;
    {
        uint16_t ovr = 0;
        if (ff_a_override(cpu, u, &ovr)) {
            /* The FF-selected source fully replaces A (HM §3.5
             * "overrule RM/STK as the source"), both in the
             * full-function form (`ETemp0_ (3S)+MD`, `ETemp_ Q+1`)
             * and on memory references (`Store_ EBLoc`). */
            a = ovr;
        } else if (u->asel == 0 || u->asel == 1) {
            /* Memory reference (asel 0/1) whose A bus / Mar source was
             * NOT overridden by FF -- i.e. the Mar comes from RM/STK
             * (a_bus default). RisId substitutes the Id onto this Mar
             * (see the RisId block below). */
            a_is_rmstk_default = 1;
        }
    }

    /* IFetch← Id (HM page 38): an IFetch← is "a fetch for which BR[24:31]
     * are replaced by Id from the IFU." This replacement happens for
     * EVERY IFetch← -- the Id (the current IFU operand byte, typically a
     * field offset) is mixed into the base register's low byte. "The IFU
     * does not advance to the next item of ←Id for IFetch←," so an
     * accompanying TisId/RisId is needed only to ADVANCE the cursor, NOT
     * to make the BR replacement happen. We therefore PEEK the current Id
     * unconditionally here and apply it below for any DM_REF_IFETCH.
     *
     * Previously the replacement was gated on a TisId/RisId function being
     * present, so a bare `IFetch_ X` (no TisId) got NO Id mix-in and read
     * at offset 0. That is exactly the Mesa read-double / read-field path
     * (e.g. RDB: `T_ (IFetch_ Stack)+1`, where the field offset α rides in
     * via Id): the germ-6.1 `frame.code` codebase read landed at the
     * global frame's G+0 (the GFI word) instead of G+1 (codebase-low),
     * faulting on a vacant bank. See docs/CONTINUE-HERE.md session 19.
     *
     * TisId / RisId also substitute the Id onto a register bus source
     * (HM page 24 + Table 11a FA=0 FB=3 FC=4/5):
     *   TisId (FC=5): Id replaces T      in A←T / B←T
     *   RisId (FC=4): Id replaces RM/STK in A←/B←RM/STK
     * The cursor advance is done once, post-ALU, in ff_apply_post; here we
     * only peek, so both the BR replacement and the bus sub see the same
     * byte the advance later consumes. */
    uint16_t ifetch_id = ifu_peek_id(cpu);
    int ifetch_id_valid = 1;
    {
        int s_fa = -1, s_fb = 0, s_fc = 0;
        if (ff_full_function_ok(u)) {
            s_fa = ff_fa(u->ff); s_fb = ff_fb(u->ff); s_fc = ff_fc(u->ff);
        } else if (ff_decode_ok(u) && u->asel <= 3) {
            s_fa = 0; s_fb = (u->ff >> 3) & 7; s_fc = u->ff & 7;
        }
        if (s_fa == 0 && s_fb == 3 && (s_fc == 4 || s_fc == 5)) {
            uint16_t idb = ifetch_id;        /* same peeked Id */
            if (s_fc == 5) {                 /* TisId: Id replaces T */
                if (u->bsel == 2) b = idb;   /* B←T */
                if (u->asel == 6) a = idb;   /* A←T */
            } else {                         /* RisId: Id replaces RM/STK */
                if (u->bsel == 1) b = idb;   /* B←RM/STK */
                if (u->asel == 4) a = idb;   /* A←RM/STK (explicit form) */
                /* Memory-reference ASEL (0/1): the A bus IS the Mar (the
                 * reference displacement). When that Mar comes from RM/STK
                 * (a_is_rmstk_default), RisId replaces it with the Id too.
                 * This is the LADRB/GADRB case `DummyRef_ StackNoUfl&+1,
                 * RisID` (DMesaXfer.mc) -- the ONLY memory-ref users of
                 * RisId. `Push[L+alpha]` computes the local/global address
                 * as BR[L/G] + Id(=alpha); without the Mar substitution the
                 * DummyRef used the stale StackNoUfl value, so @s landed at
                 * frame+StackNoUfl instead of frame+alpha and the Mesa
                 * `s _ state^` BLT overran the caller frame's return link
                 * with UnboundLink -> sUnbound -> germERROR. See
                 * docs/CONTINUE-HERE.md session 19. */
                if (a_is_rmstk_default) a = idb;
            }
        }
    }

    /* TEMP-INSTRUMENT: in the Xfer tag-dispatch region (real pc
     * 0o1700..0o1721), the control link being dispatched rides the B bus
     * (T_ BDispatch_ RTemp0). Print it to locate the null DLink that
     * ZeroDest->ControlFaults during head-startup. */
    if (dorado_trace_gate && dorado_trace_flag("DORADO_XLINK_TRACE") &&
        cpu->ctask == 0 && cpu->real_PC >= 01700u && cpu->real_PC <= 01730u) {
        fprintf(stderr, "XLINK cyc=%llu pc=0o%o T=0o%o a=0o%o b=0o%o md=0o%o\n",
                (unsigned long long)dorado_trace_cycle, cpu->real_PC,
                cpu->T, a, b, cpu->mem ? cpu->mem->md : 0);
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
    /* XorCarry / XorSavedCarry / Carry20 (HM Table 11a, FA=0 FB=2):
     * modify this instruction's ALU carry handling.
     *
     * XorCarry / XorSavedCarry (FC=4/5) modify
     * this instruction's ALU carry-in for multi-precision arithmetic.
     * XorSavedCarry XORs in the SAVED (previous instruction's) carry --
     * the high-word add/subtract consumes the low word's carry-out. The
     * BitBlt bottom-to-top base adjustment (T_ T-1, XorSavedCarry) and
     * BBNormal (T_ T+EmuBRHiReg, XorSavedCarry) both rely on it; with it
     * stubbed those 32-bit base computations dropped the inter-word
     * carry. XorCarry flips the carry-in unconditionally. The carry bit
     * is bit 5 of the ALUFM entry.
     *
     * Carry20 (FC=6) forces the internal carry into the octal-020 bit;
     * NewSetBRAndFlushPage depends on it to step across 020-word cache
     * munches while using B for unrelated data. */
    int carry20 = ff_fa0_fb2_fc_ok(u, 6);
    {
        int xfa, xfb, xfc, xok = 0;
        if (ff_full_function_ok(u)) {
            xfa = ff_fa(u->ff); xfb = ff_fb(u->ff); xfc = ff_fc(u->ff);
            xok = 1;
        } else if (ff_decode_ok(u) && u->asel <= 3) {
            xfa = 0; xfb = (u->ff >> 3) & 7; xfc = u->ff & 7; xok = 1;
        } else {
            xfa = xfb = xfc = 0;
        }
        if (xok && xfa == 0 && xfb == 2) {
            if (xfc == 5)       /* XorSavedCarry */
                alufm_entry ^= (uint8_t)((cpu->alu_carry & 1) << 5);
            else if (xfc == 4)  /* XorCarry */
                alufm_entry ^= (uint8_t)(1u << 5);
        }
    }
    uint8_t new_carry = cpu->alu_carry, new_ovf = cpu->alu_overflow;
    uint8_t is_arith = 0;
    uint16_t alu = alu_op(alufm_entry, a, b, carry20,
                          &new_carry, &new_ovf, &is_arith);
    /* The live ALU carry-out of THIS instruction (0 for logical ops):
     * this is the "ALUcarry" consumed by the Multiply function and
     * the brsh-1 LC form (HM p23). It is distinct from the latched
     * Carry' branch condition below. MulSub's entry steps are
     * `XTemp17_ A0, Multiply` where A0 is the *logical* ZERO op —
     * feeding the latched carry there seeds the partial product with
     * a phantom 0x8000 whenever the previous arithmetic op carried. */
    uint8_t alu_live_carry = is_arith ? new_carry : 0;
    /* HM page 30: "Carry' and Overflow are the result of the last
     * *arithmetic* ALU operation". Logical ops must NOT update them —
     * preserve the previous values so subsequent Carry'/Overflow
     * branches see the last arithmetic result. */
    if (!is_arith) {
        new_carry = cpu->alu_carry;
        new_ovf   = cpu->alu_overflow;
    }

    if (rtc_trace_enabled() &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE")) &&
        cpu->ctask == DORADO_JUNK_TASK &&
        cpu->real_PC >= 06600u && cpu->real_PC <= 06620u) {
        fprintf(stderr,
                "RTC_TRACE cyc=%llu pc=0o%o rb=%02o mb=%02o "
                "rstk=%02o lc=%o ff=%03o jcn=%03o "
                "a=%06o b=%06o T=%06o alu=%06o "
                "rt=%06o wake=%06o rtc430=%06o rm025=%06o "
                "ready=%04X wakepend=%04X\n",
                (unsigned long long)dorado_trace_cycle,
                cpu->real_PC & 07777, cpu->RBase & 017,
                cpu->MemBase & 037, u->rstk & 017, u->lc & 7,
                u->ff & 0377, u->jcn & 0377, a & 0177777,
                b & 0177777, cpu->T & 0177777, alu & 0177777,
                cpu->RM[DORADO_RM_RT_CLOCK] & 0177777,
                cpu->RM[DORADO_RM_WAKEUP_TIME] & 0177777,
                cpu->RM[DORADO_RM_RTC_430] & 0177777,
                cpu->RM[DORADO_RM_CAND_025] & 0177777,
                cpu->ready, cpu->wakeup_pending);
    }

    /*
     * HM page 18 + page 30: branch conditions ALU=0, ALU<0, Carry',
     * and Overflow are LOADED INTO RAM AT t3 of this instruction, then
     * tested at t1 of the *next* instruction. So the cpu->alu_* flags
     * must keep the PREVIOUS instruction's values until next_pc has
     * read them; we capture the new values now and commit at the end
     * of execute_uinstr. */
    uint8_t new_alu_zero = 0;
    uint8_t new_alu_lt0  = 0;

    /* Pd = ALU output (FF may then override or transform Pd, e.g. via
     * ALUFMRW or Pd←Cnt). FF post-effects also fire register loads
     * like Cnt←B, Link←B, RBase←FF[4:7], etc. */
    int ff_halt = 0;
    uint16_t pd = ff_apply_post(cpu, u, a, b, alu, alu_live_carry, &ff_halt);
    if (ff_halt) {
        cpu->halted = 1;
        cpu->halt_reason = ff_halt;
        return 1;
    }
    if (dorado_trace_gate && dorado_trace_flag("DORADO_STACK_TRACE") &&
        cpu->ctask == 0 &&
        ((cpu->real_PC >= 01020u && cpu->real_PC <= 01064u) ||
         (cpu->real_PC >= 01360u && cpu->real_PC <= 01475u) ||
         (cpu->real_PC >= 01700u && cpu->real_PC <= 01730u) ||
         (cpu->real_PC >= 00120u && cpu->real_PC <= 00155u) ||
         (cpu->real_PC >= 00220u && cpu->real_PC <= 00275u) ||
         (cpu->real_PC >= 00330u && cpu->real_PC <= 00500u) ||
         (cpu->real_PC >= 00500u && cpu->real_PC <= 00540u) ||
         (cpu->real_PC >= 00600u && cpu->real_PC <= 00615u) ||
         (cpu->real_PC >= 02020u && cpu->real_PC <= 02030u) ||
         (cpu->real_PC >= 02360u && cpu->real_PC <= 02375u) ||
         (cpu->real_PC >= 03600u && cpu->real_PC <= 04100u) ||
         (cpu->real_PC >= 07030u && cpu->real_PC <= 07045u))) {
        int ridx = rm_address(cpu, u);
        int widx = lc_write_address(cpu, u);
        fprintf(stderr,
                "STACK_TRACE cyc=%llu pc=0o%o rstk=%02o block=%u "
                "sp=%03o r=%03o w=%03o a=%06o b=%06o alu=%06o pd=%06o "
                "T=%06o md=%06o stkp0=%06o stkp1=%06o stkm1=%06o "
                "stk0=%06o stk1=%06o stk2=%06o stk3=%06o stk4=%06o "
                "stk5=%06o stk6=%06o stk7=%06o stk10=%06o "
                "rm03=%06o rm04=%06o rm13=%06o rm14=%06o rm15=%06o "
                "rm16=%06o rm17=%06o\n",
                (unsigned long long)dorado_trace_cycle,
                cpu->real_PC, u->rstk & 017, u->block & 1,
                cpu->StkP & 0377, ridx & 0777, widx & 0777,
                a & 0177777, b & 0177777, alu & 0177777, pd & 0177777,
                cpu->T & 0177777, task_md(cpu) & 0177777,
                cpu->STK[cpu->StkP & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 1) & 0377] & 0177777,
                cpu->STK[(cpu->StkP - 1) & 0377] & 0177777,
                cpu->STK[000] & 0177777, cpu->STK[001] & 0177777,
                cpu->STK[002] & 0177777, cpu->STK[003] & 0177777,
                cpu->STK[004] & 0177777, cpu->STK[005] & 0177777,
                cpu->STK[006] & 0177777, cpu->STK[007] & 0177777,
                cpu->STK[010] & 0177777,
                cpu->RM[003] & 0177777, cpu->RM[004] & 0177777,
                cpu->RM[013] & 0177777, cpu->RM[014] & 0177777,
                cpu->RM[015] & 0177777,
                cpu->RM[016] & 0177777, cpu->RM[017] & 0177777);
    }
    /* Barrel-shifter Pd-mux stage (HM §3.11): for a shift, the LMask/RMask/
     * Md merge is applied to the ALU output HERE (after the ALU), so it
     * affects only the value routed to T/RM via Pd -- NOT the ALU branch
     * conditions below, which see the unmasked ALU output. (a_bus put the
     * unmasked cycled word on A and recorded the mask/fill.) */
    if (cpu->shift_active) {
        pd = (uint16_t)((pd & ~cpu->shift_mask) |
                        (cpu->shift_fill & cpu->shift_mask));
    }
    /* HM page 30: the ALU=0 / ALU<0 branch conditions reflect the
     * ALU output itself, NOT the value Pd delivers after an FF
     * transform. Divide is the case that exposes the difference:
     * `T-(XTemp17), Divide` latches the SIGN of the subtract for the
     * next instruction's DblBranch[FinalAdd, OKExit, ALU<0] while Pd
     * carries the left-shifted remainder (DivSub's final correction
     * was skipped when these were latched from Pd). */
    new_alu_zero = (alu == 0) ? 1 : 0;
    new_alu_lt0  = (alu & 0x8000) ? 1 : 0;

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
            if (ff_is_output_b_memory_form(u) &&
                kind != DM_REF_IOFETCH &&
                cpu->io &&
                dorado_io_has_write(cpu->io, cpu->ctask,
                                    (uint8_t)cpu->TIOA)) {
                if (cpu->ctask != 0) goto memory_ref_done;
            }
            /* A slow-I/O Output is always the Output<-B FF function
             * (FB=3,FC=6 -> ff_is_output_b_memory_form, handled above) or the
             * IOStore memory-ref variant (DM_REF_IOSTORE). A plain DM_REF_STORE
             * is a real main-storage write and must NOT be diverted to I/O,
             * even on a non-emulator task whose TIOA still selects a device.
             * The earlier "any no-LC store -> I/O" heuristic mis-routed the
             * DSK task's AltoDiabloDisk completion stores (Store_ T/KPtr,
             * DBuf_ ... at ACmmdEnd2; FF=0o360, not the Output form) while
             * TIOA was left at DiskMuff by a preceding DoMuffOutput -- the boot
             * KCB status at VM 432 was never written and DiskBoot timed out. */
            if (kind == DM_REF_IOSTORE &&
                cpu->io &&
                dorado_io_has_write(cpu->io, cpu->ctask,
                                    (uint8_t)cpu->TIOA)) {
                dorado_io_write_subtask(cpu->io, cpu->ctask,
                                        cpu->task_subtask[cpu->ctask],
                                        (uint8_t)cpu->TIOA, b);
                if (cpu->ctask != 0) goto memory_ref_done;
            }
            /* SubTask OR's into MemBase[2:3] (HM page 88). MemBase[2:3]
             * in MSB-first 5-bit MemBase = LSB bits 2..1. So OR
             * (subtask & 3) << 1. Only effective for non-emulator.
             * Use the at-issue MemBase: FF loads (FlipMemBase) in the
             * same instruction must not redirect this reference. */
            uint8_t membase = membase_at_issue;
            if (cpu->ctask != 0) {
                membase |= (uint8_t)((cpu->task_subtask[cpu->ctask] & 3) << 1);
            }
            uint16_t mar = a;
            /* Map references address the map entry for BR + Mar where
             * Mar = the A bus like any other ref. The A bus already
             * honors the memory-subdecode FF A-source overrides
             * (ff_a_override): InitMem's `Map_ 0S, MapBuf_ ITemp1`
             * has FF=100 (A <- small constant 0, so Mar=0 and the BR
             * walks the pages), while XMFaultTask's `Map_ T,
             * MapBuf_ FaultMapVal` has FF=121 (A <- T = the faulting
             * VA, so the WP flags land on the right page). Forcing
             * Mar=0 here (an old workaround predating ff_a_override)
             * sent the XM write-protect clear to page 0, looping the
             * fault task forever on NetExec's bank-register stores. */
            /* DummyRef Mar = the A bus (Mar = a), exactly like any other
             * processor ref. The displacement source is carried by the
             * FF memory-subdecode A-source override (ff_a_override),
             * which already landed in `a` above, NOT by a blanket
             * asel/lc test:
             *   - InitMem.mc NextMapEntry `DummyRef_ T, T_ MD` compiles
             *     to asel=1,lc=3,FF=021 (FA=0 FB=2 FC=1 => A<-T), so the
             *     override gives a = T and the memory system adds one page
             *     to BR, reporting the VA via VALo/VAHi (Pipe1/Pipe0).
             *   - DMesaXfer.mc XferProc `DummyRef_ 0S, T_ MD, StkP+1`
             *     compiles to asel=1,lc=3,FF=000 (small const 0 => A<-0),
             *     so a = 0 and the ref is BR[G]+0 = the global frame VA
             *     (VALo = G, stored as L[0]). The old `mar=cpu->T` blanket
             *     override caught BOTH (same asel/lc) and forced Mar=T for
             *     XferProc, corrupting L[0] to G+T (0o4634+0o3354=0o10210)
             *     and breaking the frame-return LoadGC. Microcode-grounded
             *     (InitMem.mc!1 / DMesaXfer.mc!1) + HM Table 8a DummyRef. */
            uint16_t data = b;
            uint32_t br = dorado_mcr_disbr(cpu->mem)
                        ? 0
                        : dorado_br_get(cpu->mem, membase);
            /* IFetch← replaces BR[24:31] (the low byte of the base
             * register) with the IFU Id byte (HM page 38). When BR's low
             * byte is already 0 this is just BR + Mar + Id; in general it
             * overwrites the low byte. The Id is the operand byte the
             * accompanying TisId/RisId advances past (captured pre-ALU). */
            if (kind == DM_REF_IFETCH && ifetch_id_valid)
                br = (br & ~0xFFu) | (ifetch_id & 0xFFu);
            /* VA = BR[MemBase] + D, D unsigned (HM "Memory Addressing",
             * page 37). For all references except LongFetch, D is the
             * 16-bit Mar (= A bus). LongFetch forms the FULL 28-bit
             * displacement: "the complete 28-bit VA is
             * (B[4:15]^Mar[0:15]) + BR[MemBase]" (HM "Processor Memory
             * References", LongFetch entry; also "LongFetch takes the
             * low 16 bits of address from RM/STK and high 8 bits from
             * B"). The high 12 bits of D come from B[4:15] (the low 12
             * bits of the B register, B & 0o7777) and land in VA[4:15].
             * GetLinkID (DMesaXfer.mc) reads code-segment external links
             * at negative offsets C-(ID+1) via `LongFetch_ ~ID, B_ -1`:
             * B=0o177777 supplies D[4:15]=0o7777 so D is a 28-bit -5 and
             * VA = CP-5, instead of the 16-bit Mar (-5 as 0o177773)
             * wrapping a bank-carry into VA[4:15] and reading the wrong
             * page. Without B's high bits EFC/external calls fault. */
            uint32_t disp = (kind == DM_REF_LONGFETCH)
                          ? (((uint32_t)(b & 07777) << 16) |
                             (uint32_t)(mar & 0xFFFFu))
                          : (uint32_t)(mar & 0xFFFFu);
            uint32_t va = (br + disp) & 0x0FFFFFFFu;
            if (dorado_trace_gate && dorado_trace_flag("DORADO_DESC_TRACE") &&
                cpu->ctask == 0 && desc_trace_pc(cpu->real_PC)) {
                fprintf(stderr,
                        "DESC_TRACE cyc=%llu pc=0o%o kind=%s va=0o%o "
                        "mb=0o%o br=0o%o mar=0o%o disp=0o%o "
                        "T=0o%o Q=0o%o a=0o%o b=0o%o md=0o%o "
                        "rb=0o%o stkp=%03o "
                        "stk[-2..4]=%06o,%06o,%06o,%06o,%06o,%06o,%06o "
                        "br0=0o%o br1=0o%o br2=0o%o br3=0o%o "
                        "br34=0o%o br35=0o%o br36=0o%o br37=0o%o "
                        "br31=0o%o\n",
                        (unsigned long long)dorado_trace_cycle,
                        cpu->real_PC, ref_kind_name(kind),
                        va & 0x0FFFFFFFu, membase & 037,
                        br & 0x0FFFFFFFu, mar & 0177777,
                        disp & 0x0FFFFFFFu, cpu->T & 0177777,
                        cpu->Q & 0177777, a & 0177777, b & 0177777,
                        cpu->mem->md & 0177777, cpu->RBase & 017,
                        cpu->StkP & 0377,
                        cpu->STK[(cpu->StkP - 2) & 0377] & 0177777,
                        cpu->STK[(cpu->StkP - 1) & 0377] & 0177777,
                        cpu->STK[cpu->StkP & 0377] & 0177777,
                        cpu->STK[(cpu->StkP + 1) & 0377] & 0177777,
                        cpu->STK[(cpu->StkP + 2) & 0377] & 0177777,
                        cpu->STK[(cpu->StkP + 3) & 0377] & 0177777,
                        cpu->STK[(cpu->StkP + 4) & 0377] & 0177777,
                        dorado_br_get(cpu->mem, 0),
                        dorado_br_get(cpu->mem, 1),
                        dorado_br_get(cpu->mem, 2),
                        dorado_br_get(cpu->mem, 3),
                        dorado_br_get(cpu->mem, 034),
                        dorado_br_get(cpu->mem, 035),
                        dorado_br_get(cpu->mem, 036),
                        dorado_br_get(cpu->mem, 037),
                        dorado_br_get(cpu->mem, 031));
            }
            /* TEMP-INSTRUMENT: in the Xfer tag-dispatch region (real pc
             * 0o1700..0o1721), print the link being dispatched (A=RTemp0,
             * B) + the fetched dest, to locate the null DLink that
             * ZeroDest->ControlFaults during head-startup. */
            if (dorado_trace_gate && dorado_trace_flag("DORADO_XLINK_TRACE") &&
                cpu->ctask == 0 && cpu->real_PC >= 01700u &&
                cpu->real_PC <= 01721u) {
                fprintf(stderr,
                        "XLINK cyc=%llu pc=0o%o kind=%d va=0o%o "
                        "T=0o%o a=0o%o b=0o%o md=0o%o mb=0o%o\n",
                        (unsigned long long)dorado_trace_cycle, cpu->real_PC,
                        (int)kind, (unsigned)va, cpu->T, a, b,
                        cpu->mem ? cpu->mem->md : 0, membase);
            }
            dorado_mem_trace_membase = membase;
            dorado_mem_trace_br = (int)br;
            dorado_mem_trace_mar = mar;
            int subtask = (int)(cpu->task_subtask[cpu->ctask] & 3);
            uint8_t fault_count_before = cpu->mem->fault_count;
            dorado_fault_kind ref_fault =
                dorado_memory_ref_task(cpu->mem, kind, va, data, cpu->TIOA,
                                       (int)cpu->ctask, subtask);
            int recorded_fault =
                (ref_fault != DM_FAULT_NONE) ||
                (cpu->mem->fault_count != fault_count_before);
            if (ref_fault == DM_FAULT_NONE && ref_kind_loads_md(kind)) {
                latch_task_md_from_memory(cpu);
            }
            /* DORADO_FETCH_TRACE: emulator-task (task 0) memory reads with
             * VA + the value latched into Md. Gated by DORADO_TRACE_GATE so
             * it can pinpoint the word a stalled Alto poll loop reads. */
            if (dorado_trace_gate && cpu->ctask == 0 &&
                ref_kind_loads_md(kind) && dorado_trace_flag("DORADO_FETCH_TRACE")) {
                fprintf(stderr, "FETCH pc=0o%o kind=%d va=%07X md=0o%o "
                        "br=%07X mar=%04X\n", cpu->real_PC, (int)kind, va,
                        cpu->mem ? cpu->mem->md & 0177777 : 0,
                        br & 0x0FFFFFFFu, mar);
            }
            if (kind == DM_REF_MAP && map_cpu_trace_index(dorado_map_index(va))) {
                fprintf(stderr,
                        "MAP_CPU cyc=%llu task=%o pc=0o%o br=%07X mar=%04X "
                        "va=%07X idx=%04X data=%06o tioa=%03o "
                        "T=%04X A=%04X B=%04X Pd=%04X Link=0o%o "
                        "rb=%02o mb=%02o sub=%o iw=%06o/%06o/%06o\n",
                        (unsigned long long)cpu->cycles, cpu->ctask & 017,
                        cpu->real_PC, br & 0x0FFFFFFFu, mar,
                        va & 0x0FFFFFFFu, dorado_map_index(va),
                        data & 0177777, cpu->TIOA & 0377,
                        cpu->T, a, b, pd, cpu->Link,
                        cpu->RBase & 017, membase & 037, subtask & 3,
                        u->iw0 & 0777777, u->iw1 & 0777777,
                        u->iw2 & 0777777);
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
                    uint32_t bases[4] = {
                        0,
                        dorado_br_get(cpu->mem, 030),
                        dorado_br_get(cpu->mem, 036),
                        dorado_br_get(cpu->mem, 031),
                    };
                    const char *names[4] = {
                        "abs", "br30", "br36/mds", "br31/ecbr"
                    };
                    for (int bi = 0; bi < 4; bi++) {
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
                            "LOWCORE cyc=%llu task=%o pc=0o%o kind=%s base=%s:%07X "
                            "off=0o%o va=%07X data=%06o visible=%06o "
                            "mb=%02o rb=%02o sub=%o mar=%04X md=%06o "
                            "tioa=%03o fault=%d\n",
                            (unsigned long long)cpu->cycles,
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
                    (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate) &&
                    (strcmp(ft, "all") == 0 || cpu->ctask != 0)) {
                    fprintf(stderr,
                            "FAULT_CPU cyc=%llu task=%o pc=0o%o mesa_pc=0x%04X "
                            "mb=%02o br=%07X mar=%04X tioa=%03o "
                            "T=%04X B=%04X kind=%d "
                            "asel=%o lc=%o ff=%03o jcn=%03o "
                            "iw=%06o/%06o/%06o\n",
                            (unsigned long long)dorado_trace_cycle,
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
            if (recorded_fault && !dorado_mcr_nowake(cpu->mem)) {
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
    cpu->ifu_dispatch_rbase = -1;
    rc = next_pc(cpu, u, &np);
    /* An IFU dispatch loads RBase from the IFUM entry at t0 of the
     * entry instruction (HM p65) — after any FF RBase change in the
     * IFUJump instruction itself. */
    cpu->RBase = (cpu->ifu_dispatch_rbase >= 0)
               ? (uint8_t)cpu->ifu_dispatch_rbase
               : rbase_after_ff;
    if (rc != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    if (dorado_trace_gate && dorado_trace_flag("DORADO_CHECK_TRACE") &&
        cpu->ctask == 0 && check_trace_pc(cpu->real_PC)) {
        int ridx = rm_address(cpu, u);
        int widx = lc_write_address(cpu, u);
        uint16_t rv = (ridx < CPU_RMSTK_INVALID)
                    ? rm_stk_read(cpu, ridx) : 0;
        fprintf(stderr,
                "CHECK_TRACE cyc=%llu pc=0o%o->0o%o pcf=0o%o op=%03o "
                "rstk=%02o block=%u r=%03o rv=%06o w=%03o "
                "disp_in=%03o disp_next=%03o "
                "sp=%03o a=%06o b=%06o alu=%06o pd=%06o "
                "T=%06o Q=%06o md=%06o "
                "bc_in[z=%u lt=%u cy=%u ov=%u cnt=%06o] "
                "bc_out[z=%u lt=%u cy=%u ov=%u] "
                "stk[-2..5]=%06o,%06o,%06o,%06o,%06o,%06o,%06o,%06o "
                "rm01=%06o rm02=%06o rm03=%06o rm04=%06o "
                "rm05=%06o rm06=%06o rm07=%06o "
                "rm13=%06o rm14=%06o rm15=%06o rm16=%06o "
                "mb=%02o rb=%02o br3=0o%o br36=0o%o br37=0o%o\n",
                (unsigned long long)dorado_trace_cycle,
                cpu->real_PC, np, cpu->ifu_pcf & 0177777,
                cpu->ifu_opcode & 0377, u->rstk & 017, u->block & 1,
                ridx & 0777, rv & 0177777, widx & 0777,
                cpu->dispatch_or & 0777,
                cpu->task_dispatch[cpu->ctask] & 0777,
                cpu->StkP & 0377, a & 0177777, b & 0177777,
                alu & 0177777, pd & 0177777, cpu->T & 0177777,
                cpu->Q & 0177777, task_md(cpu) & 0177777,
                cpu->alu_zero, cpu->alu_lt0, cpu->alu_carry,
                cpu->alu_overflow, cpu->Cnt & 0177777,
                new_alu_zero, new_alu_lt0, new_carry, new_ovf,
                cpu->STK[(cpu->StkP - 2) & 0377] & 0177777,
                cpu->STK[(cpu->StkP - 1) & 0377] & 0177777,
                cpu->STK[cpu->StkP & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 1) & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 2) & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 3) & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 4) & 0377] & 0177777,
                cpu->STK[(cpu->StkP + 5) & 0377] & 0177777,
                cpu->RM[001] & 0177777, cpu->RM[002] & 0177777,
                cpu->RM[003] & 0177777, cpu->RM[004] & 0177777,
                cpu->RM[005] & 0177777, cpu->RM[006] & 0177777,
                cpu->RM[007] & 0177777,
                cpu->RM[013] & 0177777, cpu->RM[014] & 0177777,
                cpu->RM[015] & 0177777, cpu->RM[016] & 0177777,
                cpu->MemBase & 037, cpu->RBase & 017,
                cpu->mem ? dorado_br_get(cpu->mem, 3) : 0,
                cpu->mem ? dorado_br_get(cpu->mem, 036) : 0,
                cpu->mem ? dorado_br_get(cpu->mem, 037) : 0);
    }

    if (initmap_trace_enabled() &&
        ((cpu->real_PC >= 03340 && cpu->real_PC <= 03353) ||
         (cpu->real_PC >= 05550 && cpu->real_PC <= 05725))) {
        fprintf(stderr,
                "INITMAP_TRACE cyc=%llu pc=0o%o->0o%o T=%04X A=%04X B=%04X "
                "Pd=%04X Link=0o%o Cnt=%04X aluZ=%u aluLT=%u "
                "VB=%04X RP=%04X PPM=%04X Mod=%04X I0=%04X I1=%04X I2=%04X I17=%04X "
                "Mar=%05X kind=%s fault=%d\n",
                (unsigned long long)cpu->cycles, cpu->real_PC, np,
                cpu->T, a, b, pd, cpu->Link, cpu->Cnt,
                cpu->alu_zero, cpu->alu_lt0,
                cpu->RM[0x48], cpu->RM[0x49], cpu->RM[0x4A], cpu->RM[0x4B],
                cpu->RM[0x4C], cpu->RM[0x4D], cpu->RM[0x4E], cpu->RM[0x4F],
                cpu->mem ? (cpu->mem->mar & 0x0FFFFFFFu) : 0,
                cpu->mem ? ref_kind_name(cpu->mem->last_ref_kind) : "none",
                cpu->mem ? (int)cpu->mem->last_fault : 0);
    }

    /* LC. The RM/STK WRITE address is latched at instruction issue (the
     * RSEL/RBase are read at t1), so a same-instruction RBase<-FF change
     * (applied above for the *next* instruction) must NOT redirect this
     * instruction's own write. AEmu's BitBlt destination setup
     * `BBTemp_ (BBTemp)+MD, RBase_ RBase[AEmRegs]` depends on this: with
     * the FF-changed RBase the write of (DBCA + displacement) lands in
     * the wrong RM region (AEmRegs instead of BBRegs), so BBTemp keeps
     * its pre-add value and DBCA is dropped -- the BitBlt destination
     * base then misses its bitmap address and the gray fill sprays page
     * zero (corrupting M[0o344]). Use the issue-time RBase for the write,
     * matching the read (which already used it). */
    uint8_t rbase_next_instr = cpu->RBase;
    cpu->RBase = rbase_at_issue;
    rc = apply_lc(cpu, u, pd, md_at_issue);
    cpu->RBase = rbase_next_instr;
    if (rc != 0) {
        cpu->halted = 1;
        cpu->halt_reason = rc;
        return 1;
    }

    /* Cycle-aligned per-opcode AC dump (see cpu.h altoac_*). apply_lc() just
     * finished the opcode whose IFUJump dispatched altoac_op; STK[1..4] now
     * hold the Alto AC0..3 the way salto samples them at the next f2_load_ir.
     * Print IR = (op<<8)|alpha so it diffs directly against the salto trace. */
    if (cpu->altoac_pending) {
        cpu->altoac_pending = 0;
        unsigned ir = ((unsigned)cpu->altoac_op << 8) | cpu->altoac_alpha;
        fprintf(stderr, "ALTOAC cyc=%llu IR=%06o set=%u AC=%06o,%06o,%06o,%06o\n",
                (unsigned long long)dorado_trace_cycle,
                ir, cpu->altoac_insset,
                cpu->STK[1] & 0177777, cpu->STK[2] & 0177777,
                cpu->STK[3] & 0177777, cpu->STK[4] & 0177777);
        /* Full Alto low-memory dump at a chosen booted-opcode index, counted
         * from the boot-loader entry (IR=000345). DORADO_MEMDUMP_AT=<n>; one
         * "MD %06o %06o" line per word for direct diffing against salto. */
        static long mdcount = -1, mdtarget = -2;
        if (mdtarget == -2) { const char *w = getenv("DORADO_MEMDUMP_AT");
                              mdtarget = w ? strtol(w, NULL, 0) : -1; }
        if (mdtarget >= 0) {
            if (mdcount < 0 && ir == 000345) mdcount = 0;
            else if (mdcount >= 0) mdcount++;
            if (mdcount == mdtarget && cpu->mem) {
                for (uint32_t va = 0; va < 0200000u; va++)
                    fprintf(stderr, "MD %06o %06o\n", va,
                            dorado_visible_word_at_va(cpu->mem, va) & 0177777);
            }
        }
    }

    /* Post-instruction StkP update + StkOvf/StkUnd recompute (HM
     * Table 6 / page 11). For BLOCK=0 instructions this is a no-op. */
    stk_apply_post(cpu, u);
    if (cpu->ifu_save_stkp_pending) {
        cpu->ifu_saved_stkp = (uint8_t)(cpu->StkP & 0xFF);
        cpu->ifu_save_stkp_pending = 0;
        if (dorado_trace_flag("DORADO_STKP_TRACE") &&
            (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE"))) {
            fprintf(stderr,
                    "SAVE_STKP cyc=%llu pc=0o%o saved=%03o\n",
                    (unsigned long long)dorado_trace_cycle,
                    cpu->real_PC, cpu->ifu_saved_stkp & 0377);
        }
    }

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
