#ifndef DORADO_CPU_H
#define DORADO_CPU_H

#include <stddef.h>
#include <stdint.h>

#include "disasm.h"
#include "microcode.h"

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
#define CPU_QUADRANT_SIZE   04000  /* 4K-word IM quadrant */

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
    uint16_t TPC;               /* task PC (saved on switch) */
    uint16_t TIOA;              /* 8-bit I/O address (Slow IO) */

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

    /* Operand bytes captured at IFUJump time (for ←Id delivery). */
    uint8_t  ifu_alpha;
    uint8_t  ifu_beta;
    uint8_t  ifu_length;        /* 1, 2, or 3 */
    uint8_t  ifu_n;             /* 4-bit operand from IFUM */
    uint8_t  ifu_packed_a;
    uint8_t  ifu_sign;
    uint8_t  ifu_type_jump;     /* derived from TJump' */
    uint8_t  ifu_type_pause;    /* derived from TPause' */

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

    /* The "current instruction address" — what the manual calls CIA.
     * After step() runs, this advances to the next instruction. */
    uint16_t real_PC;
    uint16_t prev_PC;           /* for tracing */

    /*
     * Tasking (HM §4.1, page 26-27).
     *
     * 16 priority-scheduled tasks. Task 15 = highest (fault task);
     * task 0 = lowest (emulator, always awake). Each task has its
     * own T, TPC, Link, MemBase saved across switches; Q, ALUFM,
     * StkP, ShC, Cnt, RBase are NOT per-task (HM §4.1).
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
    uint16_t ready;             /* bitmask: tasks runnable */
    uint8_t  tasking_on;        /* 1 = tasking enabled */
    uint8_t  tasking_resume_delay; /* >0: countdown after TaskingOn before switching */

    /* Per-task saved state. Loaded into the live registers when
     * `ctask` becomes that task. */
    uint16_t task_t[16];
    uint16_t task_tpc[16];      /* indexed by task number; for the
                                 * running task, the live PC is in
                                 * real_PC and task_tpc[ctask] is
                                 * stale until the next switch. */
    uint16_t task_link[16];
    uint8_t  task_membase[16];

    /* ALU branch-condition flags from the previous instruction.
     * Updated whenever an ALU operation runs (HM Table 13). */
    uint8_t  alu_zero;          /* ALU = 0 */
    uint8_t  alu_lt0;           /* ALU < 0 (high bit set) */
    uint8_t  alu_carry;         /* saved carry-out */
    uint8_t  alu_overflow;      /* signed overflow */

    /* Hardware memories. RM, STK live in the CPU; IM/IFUM/ALUFM come
     * from the loaded microcode. */
    uint16_t RM[256];
    uint16_t STK[256];
    const dorado_microcode *mc; /* IM, IFUM, ALUFM (loaded via microcode) */

    /* Termination / debug. */
    int      halted;            /* nonzero = stop stepping */
    int      halt_reason;
    int      cycles;            /* cycles executed */

    /* Trace control. */
    int      trace;             /* if nonzero, print each step to trace_fp */
    void    *trace_fp;          /* FILE* (kept opaque to avoid forcing stdio.h) */
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

#endif
