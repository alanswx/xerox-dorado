#ifndef DORADO_CPU_H
#define DORADO_CPU_H

#include <stddef.h>
#include <stdint.h>

#include "disasm.h"
#include "microcode.h"
#include "io.h"

/*
 * Dorado microengine — minimal skeleton.
 *
 * Scope of this initial cut (HM §3, §4):
 *   - Single task (task 0). No tasking, no Hold, no IFU, no main memory,
 *     no map / cache / pipe / BR. No I/O.
 *   - Per-cycle: read IM[real_PC], evaluate A and B buses, execute ALU
 *     via ALUFM, route Pd through LC, advance microPC.
 *   - JCN: fall-through (.+1) and unconditional local/long/global jumps.
 *     Conditional branches and IFU jumps are stubs that panic for now.
 *   - FF: a small subset (constants, simple register sources). The
 *     dispatch is table-driven so we can extend without rewriting.
 *
 * Out of scope:
 *   - Md / Id / Mar / shifter / multiply-divide / parity / breakpoints.
 *   - Real-time semantics: this is a single-step interpreter, not a
 *     cycle-accurate simulator.
 *
 * Goal: be able to single-step Bootstrap.mb from its entry point and
 * use TRACE to discover which FF / JCN combinations real microcode
 * uses, so we can prioritize what to implement next.
 */

#define CPU_PAGE_SIZE       0100   /* 64-word IM page (HM §4.3) */
/* HM §4.3: a "quadrant" is a 4K-word IM region. Global Calls target
 * any of 64 pages within the current quadrant; Long branches address
 * 12 bits within the current quadrant. With our 4K IM, the entire
 * store is one quadrant — quadrant masking effectively no-ops, but
 * we must not mask off any address bits or globals/longs would
 * point outside the 12-bit IM range. The constant is the next power
 * of two beyond IM_SIZE so that `~(QUADRANT_SIZE-1)` is 0 in 12 bits. */
#define CPU_QUADRANT_SIZE   010000 /* 4K-word IM quadrant (full IM today) */

typedef struct {
    /* Working registers (HM Table 2). One task, so no replication. */
    uint16_t T;                 /* primary working register */
    uint16_t Q;                 /* shift register */
    uint16_t Cnt;               /* loop counter */
    uint16_t RBase;             /* 4-bit RM page selector */
    uint16_t StkP;              /* 8-bit stack pointer */
    uint8_t  stk_ovf;           /* StkOvf flipflop, set by post-instruction
                                 * StkP-update overflow (HM page 11);
                                 * read via Pd←Pointers FF (FA=2 FB=6 FC=5). */
    uint8_t  stk_und;           /* StkUnd flipflop — same idea, underflow side. */
    uint16_t ShC;               /* shifter control */
    uint16_t MemBase;           /* 5-bit BR selector */
    uint16_t MemBX;             /* 2-bit MemBase extension (HM §6 IFU
                                 * supplies MemBX-relative MemBase per
                                 * opcode for the emulator task). */
    uint16_t Link;              /* subroutine return address */
    uint16_t link_at_issue;     /* Link snapshot at instruction issue;
                                 * Write IM and Subroutine Return read
                                 * this instead of Link directly so a
                                 * same-instruction Link←B (e.g.,
                                 * B←RWCPReg) doesn't clobber the
                                 * value the consumer needs. */
    uint16_t b_link_read;        /* HM §4.8: RdTPC / Read IM deliver their
                                  * data on the "alternate B←Link path" the
                                  * NEXT cycle, while Link itself is smashed
                                  * with CIA+1. This latch holds that data. */
    uint8_t  b_link_read_valid;  /* 1 while b_link_read overrides B←Link */
    uint8_t  b_link_read_age;    /* one-instruction lifetime aging */
    uint16_t dispatch_or;        /* active one-cycle TNIA dispatch OR */
    uint16_t task_dispatch[16];  /* per-task pending dispatch OR. HM
                                  * §4.4 p32: the B dispatches load
                                  * Link from B and OR Link bits into
                                  * TNIA "during the next instruction
                                  * for the task. Since Link is task-
                                  * specific, this works correctly
                                  * across task switching." A global
                                  * latch would let an interleaved I/O
                                  * task consume the emulator's
                                  * dispatch (it did: AEmu Nova skips
                                  * died whenever the EIT was busy). */
    uint8_t io_atten_at_issue;   /* IOAttention level sampled at t1 of
                                  * the current instruction (with the
                                  * at-issue TIOA); the IOAtten' branch
                                  * condition reads this so a same-
                                  * instruction Pd←Input doesn't move
                                  * the word the test refers to. */
    uint16_t TPC;               /* task PC (saved on switch) */
    uint16_t TIOA;              /* live 8-bit I/O address (Slow IO) */

    /*
     * IFU state — Phase C.1+C.2 (HM §6, Tables 18, 20).
     *
     * IFUM is a 1024 × 24-bit (+3 parity) RAM addressed by
     * InsSet[0:1] || Opcode[0:7] = 10 bits.
     *
     * For loading IFUM (Phase C.1):
     *   InsSetorEvent←B   (B[0]=1, B[6:7]→InsSet[0:1])
     *   BrkIns←B          (B[0:7]→Opcode)
     *   IFUMLH/RH←B       (write the 32-bit IFUM entry's halves)
     * and reading back via B←IFUMLH'/RH'.
     *
     * For dispatching opcodes (Phase C.2 — minimal):
     *   PCF←B starts the byte stream at PC = B (bit 0 of B is byte
     *   selector, bits 1:15 are word displacement relative to BR[31]).
     *   IFUJump[n] (JCN encoding) reads the opcode byte at PC, looks up
     *   IFUM[InsSet||opcode], dispatches to the entry vector slot, and
     *   sets MemBase/RBase per the IFUM entry. The IFU then has the
     *   alpha/beta operand bytes available for ←Id (TIsId/RIsId/A←Id).
     *
     * Simplifications vs. real hardware:
     *   - No multi-stage F/G→J→H→M pipeline; bytes are fetched on
     *     demand. NotReady traps don't fire (we always have a byte).
     *   - No timing model; IFUJump completes in one cycle.
     *   - Hold + IFU map fault interactions deferred.
     */
    uint8_t  ifu_insset;        /* 2-bit instruction set selector */
    uint8_t  ifu_opcode;        /* 8-bit opcode register (write addressing) */

    /* IFU byte-stream cursor and decoded current-opcode state. */
    uint16_t ifu_pcf;           /* byte cursor: bit 0=byte selector, bits 1:15=word */
    uint16_t ifu_pcx;           /* PC of the currently-executing opcode */
    uint8_t  ifu_idcnt;         /* count of ←Id deliveries this opcode */
    uint8_t  ifu_active;        /* 1 = PCF set, ready to dispatch */

    /* IFU warmup counter (HM page 67). After PCF←B, the pipeline
     * needs ~5 cycles before the first opcode is in M-level and
     * IFUJump can succeed. Earlier IFUJumps trap to the "IFU not
     * ready" vector at *34-37 (with InsSet OR'd into the trap
     * address per HM Table 14). */
    uint8_t  ifu_warmup;

    /* Reschedule countdown (HM Table 20).
     * `Reschedule` FF function sets reschedule_pending = 2: the
     *   second OR third successful IFUJump traps to *14-17.
     * `RescheduleNow` FF sets reschedule_pending = 1: the next
     *   successful IFUJump traps.
     * `NoReschedule` clears it. Decrements on each successful
     * (non-trapping) IFUJump; when it counts down to 1, that
     * IFUJump traps. Also sets the IOAtten'/Reschedule branch
     * condition true for the emulator (task 0). */
    uint8_t  reschedule_pending;
    uint8_t  reschedule_cond;    /* emulator Reschedule branch condition
                                  * (HM Table 13/20): set by Reschedule,
                                  * cleared only by NoReschedule. */

    /* Junk-task timer wakeup (HM §12.1). The IFU board raises the
     * Junk task wakeup every 32 us while enabled; AckJunkTW/IFUTest
     * dismisses and controls re-enable. */
    uint8_t  junk_tw_enabled;
    uint16_t junk_tw_countdown;

    /* Operand bytes captured at IFUJump time (for ←Id delivery). */
    uint8_t  ifu_alpha;
    uint8_t  ifu_beta;
    uint8_t  ifu_length;        /* 1, 2, or 3 */
    uint8_t  ifu_n;             /* 4-bit operand from IFUM */
    uint8_t  ifu_packed_a;
    uint8_t  ifu_sign;
    uint8_t  ifu_type_jump;     /* derived from TJump' */
    uint8_t  ifu_type_pause;    /* derived from TPause' */
    uint8_t  ifu_saved_stkp;    /* post-IFUJump StkP saved for RestoreStkP */
    uint8_t  ifu_save_stkp_pending;

    /* DORADO_ALTOAC_TRACE: a per-Alto-opcode AC trace cycle-aligned with the
     * salto reference. next_pc()'s IFUJump dispatch runs BEFORE apply_lc()'s
     * writeback, so the dispatch-time AC snapshot lags one opcode. We instead
     * capture the dispatched opcode here and print STK[1..4] after apply_lc,
     * matching salto's f2_load_ir sample (IR_next with the AC that the
     * just-finished opcode left). */
    uint8_t  altoac_pending;
    uint8_t  altoac_op;
    uint8_t  altoac_alpha;
    uint8_t  altoac_insset;

    /*
     * Breakpoints / parity / event counters — gap B11.
     *
     * HM §4.10 describes a `BrkPending` flipflop set by BrkIns←B and
     * cleared when the corresponding instruction is fetched from IM.
     * We model the state slot but do not yet trigger a breakpoint
     * trap (Midas, the Alto-side debugger, would consume it).
     *
     * EventCntA / EventCntB are 16-bit performance counters that
     * tick on selected events programmed via InsSetorEvent←B (B[0]=0
     * mode). We provide read/write access so microcode using them
     * round-trips correctly; we do not yet drive any tick events.
     *
     * `parity_error` is a single bit set by IFUM/IM/RM parity
     * mismatches. We do not yet model parity at the bit level, so
     * the bit only ever sets if the slow-IO path reports
     * `io_bad_parity`.
     */
    uint8_t  brk_pending;       /* HM §4.10 BrkPending flipflop */
    uint8_t  brk_opcode;        /* opcode from BrkIns←B (high 8 bits of B) */
    uint16_t event_cnt_a;       /* HM §4.11 EventCntA (= GenIn pins, IFU board) */
    uint16_t event_cnt_b;       /* HM §4.11 EventCntB (= GenOut register, IFU board) */
    uint8_t  event_cnt_ctrl_lo; /* B[4:15] saved when B[0]=0 in InsSetorEvent←B */
    uint8_t  event_cnt_ctrl_hi;
    /* GenIO mode: the eventCounters diagnostic's genIO test assumes a plug
     * connecting GenOut to GenIn, so reading GenIn' (=EventCntA') returns what
     * was written to GenOut (=EventCntB'). Set when the MOS control loaded via
     * InsSetorEvent←B (B[0]=0) has both counter-enable bits clear (counters
     * disabled). When set, EventCntA' reads back EventCntB (the loopback);
     * otherwise it reads the real EventCntA counter. (eventCounters1.mc genIO.) */
    uint8_t  gen_io_mode;
    uint8_t  parity_error;      /* set when IOAtten parity fault propagates */

    /*
     * Memory subsystem. When non-NULL, processor memory references
     * (Fetch / Store / etc.) are dispatched to it; B←Md reads
     * mem->md. Pipe reads (B←Pipe0..5) come from mem->pipe[]. */
    struct dorado_memory *mem;

    /*
     * BaseBoard interface. When non-NULL, the Dorado's `B←RWCPReg`
     * reads come from the BaseBoard's CPReg latches (RIOT #3 PA/PB)
     * and writes drive the BaseBoard's CPReg input pins. The
     * BaseBoard 6502 is stepped at a configurable ratio per Dorado
     * microcycle.
     *
     * If `baseboard` is NULL, B←RWCPReg returns the legacy `cpreg`
     * counter stub (each read returns a different value).
     */
    struct dorado_baseboard *baseboard;
    int      baseboard_cycles_per_uop; /* how many 6502 cycles per Dorado
                                        * microinstruction. 0 = don't step. */
    uint16_t cpreg;             /* legacy stub when baseboard == NULL */

    /*
     * Slow I/O routing (HM §7).
     *
     * When non-NULL, Pd←Input, Pd←InputNoPE and Output←B dispatch
     * through the device table indexed by (ctask, TIOA). Default
     * (NULL) returns 0xFFFF on read (floating bus) and discards
     * writes — same behavior as the legacy stub.
     */
    dorado_io *io;
    uint8_t  io_bad_parity;     /* set by Pd←Input on floating-bus or
                                 * device-flagged parity error; consumed
                                 * by parity-fault microcode (TBD). */

    /* The "current instruction address" — what the manual calls CIA.
     * After step() runs, this advances to the next instruction. */
    uint16_t real_PC;
    uint16_t prev_PC;           /* for tracing */

    /*
     * Tasking (HM §4.1, page 26-27).
     *
     * 16 priority-scheduled tasks. Task 15 = highest (fault task);
     * task 0 = lowest (emulator, always awake). Each task has its
     * own T, TPC, Link, MemBase, RBase, TIOA, and Md saved across switches;
     * Q, ALUFM, StkP, ShC, and Cnt are not per-task (HM §4.1,
     * §7).
     *
     * Wakeup model:
     *   `wakeup_pending` — devices/microcode raised the wakeup line
     *   `ready`          — task is runnable (woken and not yet
     *                      blocked). Task 0 is always ready.
     *   BNT (Best Next Task) = highest priority bit in
     *                          (ready | wakeup_pending).
     *   A task switch happens at the end of an instruction iff
     *   BNT > CTASK, or BLOCK=1 in a non-emulator task.
     *   A task that BLOCKs has its Ready bit cleared.
     *
     * Tasking can be disabled with the TaskingOff FF function and
     * re-enabled with TaskingOn (atomic; takes effect after 2 more
     * instructions per HM page 27).
     */
    uint8_t  ctask;             /* current task (0..15) */
    uint16_t wakeup_pending;    /* bitmask: device/microcode wake requests */
    /* HM p27: "a minimum of two cycles elapses after the instruction
     * containing Wakeup before the task executes its first instruction."
     * A microcode Wakeup[n] is staged here and promoted into
     * wakeup_pending after the next task_schedule, so the woken task's
     * first instruction lands 2 cycles later. This is required for
     * preemption correctness: it lets a task that notifies a higher task
     * finish the instruction after the Wakeup (e.g. move a subroutine
     * return out of a shared RM scratch reg into its per-task Link)
     * before being preempted. See the kernel TaskTest diagnostic.
     * (DORADO_WAKE_IMMEDIATE reverts to the old single-cycle model.) */
    uint16_t wakeup_pipe[2];    /* 2-stage delay for microcode Wakeup[n] */
    uint16_t ready;             /* bitmask: tasks runnable */
    uint8_t  tasking_on;        /* 1 = tasking enabled */
    uint8_t  tasking_resume_delay; /* >0: countdown after TaskingOn before switching */

    /* HM §3.12 Hold & Task Simulator (hardware-checkout debug feature,
     * exercised by the kernel TestTW diagnostic via Hold&TaskSim←B,
     * FF=0o154). TASKSIM: a 7-bit counter loaded non-zero counts up each
     * cycle and, on overflow past 0o177, raises a wakeup for the
     * simulator task (task 12, backplane-jumpered), held until reloaded.
     * HOLDSIM: an 8-bit recirculating shift register (stored; HOLD
     * injection not yet modeled). */
    uint8_t  tasksim;           /* 7-bit wakeup counter (0 = disabled) */
    uint8_t  tasksim_fired;     /* 1 once overflowed; held until reload */
    uint8_t  tasksim_loaded;    /* transient: load took this cycle's clock */
    uint8_t  holdsim;           /* 8-bit recirculating hold shift register */

    /* Per-task saved state. Loaded into the live registers when
     * `ctask` becomes that task. */
    uint16_t task_t[16];
    uint16_t task_tpc[16];      /* indexed by task number; for the
                                 * running task, the live PC is in
                                 * real_PC and task_tpc[ctask] is
                                 * stale until the next switch. */
    uint16_t task_link[16];
    uint8_t  task_membase[16];
    int16_t  ifu_dispatch_rbase; /* RBase loaded by the IFU dispatch
                                  * inside next_pc (HM p65: "RBase is
                                  * loaded with 0 or 1 when the IFU
                                  * dispatches to the first instruction
                                  * of an opcode"); -1 = no dispatch
                                  * this instruction. execute_uinstr
                                  * applies it AFTER restoring the
                                  * post-FF RBase, so the at-issue
                                  * branch-condition juggling cannot
                                  * wipe it out. */
    uint8_t  task_rbase[16];
    uint8_t  task_tioa[16];
    uint16_t task_md[16];
    uint8_t  task_md_valid[16];
    uint64_t task_md_ready[16]; /* cpu->cycles at which this task's pending
                                 * fetch's Md becomes consumable. The engine
                                 * Holds (jumps to self, yielding to higher-
                                 * priority tasks) if the task reads Md before
                                 * this cycle. HM §5 pp.41-42. */
    uint8_t  task_alu_zero[16];
    uint8_t  task_alu_lt0[16];
    uint8_t  task_alu_carry[16];
    uint8_t  task_alu_overflow[16];

    /* SubTask (HM page 88). When an I/O device wakes a task, it
     * may present a 2-bit SubTask. The processor OR's
     * SubTask[0:1] into RBase[2:3] and MemBase[2:3] when the task
     * runs, allowing one task's microcode to serve multiple
     * "sub-channels" with separate RM regions and BR pairs.
     *
     * Each task has its own SubTask register; the I/O device
     * driving that task chooses the SubTask value per wakeup.
     * Without modeled devices, all SubTasks default to 0. */
    uint8_t  task_subtask[16];

    /* ALU branch-condition flags from the previous instruction.
     * Updated whenever an ALU operation runs (HM Table 13). These are
     * task-local in hardware; the live fields below are saved/restored
     * through task_alu_* on task switches. */
    uint8_t  alu_zero;          /* ALU = 0 */
    uint8_t  alu_lt0;           /* ALU < 0 (high bit set) */
    uint8_t  alu_carry;         /* saved carry-out */
    uint8_t  alu_overflow;      /* signed overflow */

    /* Transient (per-instruction) barrel-shifter Pd-mux state. HM §3.11:
     * the shifter places its UNMASKED cycled word on the A bus; the
     * mask/Md merge happens in the Pd multiplexer AFTER the ALU, so the
     * ALU branch conditions see unmasked data. a_bus() records the mask
     * and fill here for a shift (ASEL=7); execute_uinstr applies them to
     * Pd after the ALU. */
    uint8_t  shift_active;      /* this instruction is a barrel shift */
    uint16_t shift_mask;        /* Pd-mux mask (1 = take from fill) */
    uint16_t shift_fill;        /* masked-area source (0 or Md) */

    /* Hardware memories. RM, STK live in the CPU; IM/IFUM/ALUFM come
     * from the loaded microcode. */
    uint16_t RM[256];
    uint16_t STK[256];
    const dorado_microcode *mc; /* IM, IFUM, ALUFM (loaded via microcode) */

    /* Termination / debug. */
    int      halted;            /* nonzero = stop stepping */
    int      halt_reason;
    int      cycles;            /* cycles executed */

    /* Count of successful IFU opcode dispatches (each real IFUJump that
     * reads an opcode and dispatches to its handler). Incremented in
     * next_pc; NOT incremented for NotReady/warmup/fault traps. Used by
     * differential-test harnesses (build/altodiff-dorado) to detect the
     * "one opcode executed" boundary. */
    uint64_t ifu_dispatch_count;

    /* Trace control. */
    int      trace;             /* if nonzero, print each step to trace_fp */
    void    *trace_fp;          /* FILE* (kept opaque to avoid forcing stdio.h) */

    /* Write-IM diagnostic capture (set dbg_writeim_log = 1 to enable). */
    int      dbg_writeim_log;
    int      dbg_writeim_n;
    uint16_t dbg_writeim_addr[32];
    uint8_t  dbg_writeim_half[32];
    uint8_t  dbg_writeim_sec[32];
    uint16_t dbg_writeim_b[32];
    uint16_t dbg_writeim_t[32];
    uint16_t dbg_writeim_pc[32];
} dorado_cpu;

typedef enum {
    CPU_HALT_NONE = 0,
    CPU_HALT_OK,                /* ordinary stop (e.g., breakpoint) */
    CPU_HALT_NO_CODE,           /* PC pointed at empty IM slot */
    CPU_HALT_UNSUPPORTED_FF,    /* FF function we don't handle yet */
    CPU_HALT_UNSUPPORTED_JCN,   /* JCN encoding we don't handle yet */
    CPU_HALT_UNSUPPORTED_ASEL,
    CPU_HALT_UNSUPPORTED_BSEL,
    CPU_HALT_BREAKPOINT,        /* uinstr.brk_p was set */
    CPU_HALT_BAD_RM,            /* RM access out of range */
    CPU_HALT_USER,              /* user requested via API */
    CPU_HALT_IFU_NOT_READY,     /* IFUJump before PCF←B / before warmup */
    CPU_HALT_IFU_NO_ENTRY,      /* IFUJump on opcode whose IFUM entry is absent */
} cpu_halt_reason;

const char *cpu_halt_reason_str(cpu_halt_reason r);

/*
 * Initialize CPU state. RM is preloaded from mc->rm, register state
 * is zeroed, real_PC is set to start_real (typically the post-placement
 * address of the entry-point label).
 */
void dorado_cpu_init(dorado_cpu *cpu, const dorado_microcode *mc,
                     uint16_t start_real);

/* Single-step. Returns 0 on success, nonzero if halted. */
int dorado_cpu_step(dorado_cpu *cpu);

/* Run until halt or `max_cycles` reached. Returns the halt reason. */
cpu_halt_reason dorado_cpu_run(dorado_cpu *cpu, int max_cycles);

/* Enable line-by-line tracing to stderr (or a user FILE*). */
void dorado_cpu_trace(dorado_cpu *cpu, void *fp);

/* Tasking helpers — used by tests and by I/O device modeling once
 * Phase E lands. The `task` argument is 0..15. */
void dorado_cpu_wakeup(dorado_cpu *cpu, int task);
void dorado_cpu_set_task_tpc(dorado_cpu *cpu, int task, uint16_t real_pc);
uint16_t dorado_cpu_get_task_tpc(const dorado_cpu *cpu, int task);

/* SubTask: I/O devices set the task's SubTask before/at wakeup.
 * `subtask` is 2 bits (0..3); higher bits ignored. */
void dorado_cpu_set_subtask(dorado_cpu *cpu, int task, uint8_t subtask);

/* Harness-driven cycle-window gate for env-enabled debug traces
 * (set by tests/test_cpu.c from DORADO_TRACE_GATE="lo,hi"). */
extern int dorado_trace_gate;
extern unsigned long long dorado_trace_cycle;

/* Cached boolean test of a trace environment variable (DORADO_*_TRACE etc.).
 * Caches by string-literal pointer; pass only literals used as on/off flags,
 * never value-returning env vars. See src/cpu.c. */
int dorado_trace_flag(const char *name);

#endif
