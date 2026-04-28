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
    uint16_t ShC;               /* shifter control */
    uint16_t MemBase;           /* 5-bit BR selector */
    uint16_t Link;              /* subroutine return address */
    uint16_t TPC;               /* task PC (saved on switch) */

    /* The "current instruction address" — what the manual calls CIA.
     * After step() runs, this advances to the next instruction. */
    uint16_t real_PC;
    uint16_t prev_PC;           /* for tracing */

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

#endif
