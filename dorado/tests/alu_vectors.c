/*
 * alu_vectors -- dump ALU test vectors from the C emulator's own alu_op(),
 * for the RTL to be diffed against.
 *
 * WHY THIS EXISTS. The Verilog cell_MC10181 is transcribed from Motorola's
 * datasheet; cpu.c's alu_op() was written from the Hardware Manual's Table 9.
 * Two independent derivations of the same 1979 part, 45 years apart, and the
 * whole point of generating RTL from PARC's netlists is that they can now
 * check each other. This program is the C half of that: it prints what the
 * emulator computes, and verilog/verilator/tb_alu_vs_c.sv feeds the same
 * inputs through four chained MC10181 slices -- the way ProcH and ProcL
 * actually build the 16-bit datapath -- and compares.
 *
 * It also exercises something a single-slice test cannot: the CARRY RIPPLE
 * across slice boundaries, which is the only place a 16-bit answer can differ
 * from four correct 4-bit ones.
 *
 * ALUFM ENCODING, confirmed against the datasheet: the 6-bit entry is
 * {Cn, S3, S2, S1, S0, M} -- carry-in in the top bit, then the part's four
 * select lines, then the mode bit (0 = arithmetic, 1 = logic). All 21
 * operations alu_op() implements agree with the datasheet's function table
 * under that mapping.
 *
 * alu_op() is static, so this includes cpu.c directly rather than linking it
 * -- which is deliberate: the test must run the emulator's REAL code, not a
 * copy of it that could drift.
 *
 * Output: one line per case, "entry a b result carry", all hex but the carry.
 */

#include "../src/cpu.c"

#include <stdio.h>

int main(void)
{
    /* The 21 operations alu_op() implements: five arithmetic, sixteen
     * logical. The part does sixteen of each; the emulator implements the
     * ones the Dorado's microcode actually loads into ALUFM. */
    static const uint8_t ops[] = {
        0000, 0006, 0014, 0022, 0036,                      /* arithmetic */
        0001, 0003, 0005, 0007, 0011, 0013, 0015, 0017,    /* logical */
        0021, 0023, 0025, 0027, 0031, 0033, 0035, 0037,
    };
    /* Operands chosen for the edges that matter: zero, one, the nibble and
     * byte boundaries where a carry crosses a slice, both signs, and the
     * alternating patterns that catch a swapped bit. */
    static const uint16_t vals[] = {
        0x0000, 0x0001, 0x0002, 0x000F, 0x0010, 0x00FF, 0x0100, 0x1234,
        0x7FFF, 0x8000, 0x8001, 0xABCD, 0xFFFE, 0xFFFF, 0x5555, 0xAAAA,
    };

    for (unsigned o = 0; o < sizeof ops / sizeof *ops; o++)
        for (int cy = 0; cy < 2; cy++)
            for (unsigned i = 0; i < sizeof vals / sizeof *vals; i++)
                for (unsigned j = 0; j < sizeof vals / sizeof *vals; j++) {
                    uint8_t entry = (uint8_t)((cy << 5) | ops[o]);
                    uint8_t carry = 0, ovf = 0, arith = 0;
                    uint16_t r = alu_op(entry, vals[i], vals[j], 0,
                                        &carry, &ovf, &arith);
                    /* ovf is the SIGNED OVERFLOW alu_op() already computes.
                     * It used to be passed in and thrown away; the RTL side
                     * now checks ProcH d13 (an MC10170 wired as an overflow
                     * detector) against it, which is the independent oracle
                     * the IM-parity question needs. */
                    printf("%02x %04x %04x %04x %d %d\n",
                           entry, vals[i], vals[j], r, carry, ovf);
                }
    return 0;
}
