/*
 * altoref - a spec-derived Nova/Alto opcode reference, the ORACLE half of
 * the differential harness. Computes the architecturally-correct result of
 * one Alto emulator instruction per the Alto Hardware Manual (May 1979),
 * Section 3 "Emulator" -- the carry generator, function generator, shifter,
 * and skip sensor (Figure 4), and the M/J-group memory instructions.
 * Independent of the Dorado microcode, so diffing it against altodiff-dorado
 * localizes Dorado opcode-emulation bugs.
 */
#ifndef DORADO_ALTOREF_H
#define DORADO_ALTOREF_H

#include <stdint.h>

typedef struct {
    uint16_t ac[4];      /* accumulators after the instruction        */
    int      cry;        /* carry bit after (0/1)                     */
    int      skip;       /* 1 if the instruction skipped (PC += 1)    */
    int      wrote;      /* 1 if it wrote memory                      */
    uint16_t wr_addr;    /* address written (when wrote)              */
    uint16_t wr_val;     /* value written                             */
    int      is_jump;    /* 1 for JMP/JSR (PC <- E)                   */
    uint16_t jump_target;/* E, when is_jump                           */
    int      ok;         /* 1 = a recognized/handled opcode           */
} alto_ref_result;

/*
 * Execute one opcode against page-zero memory `mem` (0..0o377). `ac_in`,
 * `cry_in` are the input accumulators/carry; `pc` is the instruction's
 * own address (for PC-relative effective addresses). `mem` is read for
 * LDA/ISZ/DSZ/indirect and updated for STA/ISZ/DSZ.
 */
alto_ref_result alto_ref_exec(uint16_t op, const uint16_t ac_in[4],
                              int cry_in, uint16_t *mem, uint16_t pc);

#endif
