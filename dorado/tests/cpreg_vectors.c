/*
 * cpreg_vectors -- dump control-processor strobe vectors from the C
 * emulator's own apply_mcp_strobe(), for the RTL Control board to be diffed
 * against.
 *
 * WHY THIS EXISTS. The BaseBoard drives the Dorado through a nine-bit bus, a
 * three-bit function code and a strobe. The C emulator models that at the
 * PROTOCOL level -- `apply_mcp_strobe` decodes the function code and updates
 * `cpreg_to_dorado`, `mir_bytes[]` and the run/step flags -- and it was
 * written from PARC's `doradoio.mdefs` and `doradocpint.masm`. The RTL has the
 * actual wires, generated from PARC's netlist: `CPOut.0-8`, `CPAddr.0'-2'`,
 * `CPStrb'` cross the backplane to ContA, where an MC10161 decodes the
 * function into `CPStrb0'`..`CPStrb3'` and four MC10176 hex flip-flops latch
 * the bytes.
 *
 * Two independent derivations of one interface, so they can check each other.
 * That is the same arrangement as alu_vectors.c and the ALU, and it is the
 * whole point of generating the RTL from the netlists.
 *
 * WHAT THE NETLIST ALREADY CONFIRMED, before a single vector ran:
 *
 *   - The function code's bit order. ContA's MC10161 takes `CPAddr.0'` on its
 *     MOST significant select, and the C emulator computes the function as
 *     `(mcpbusl >> 4) & 7` over a RIOT port where bit 6 is `MCPABus.0`. Both
 *     make `.0` the most significant, independently.
 *   - Function 2 is the HIGH byte and function 3 the low. ContA's `CPStrb2'`
 *     clocks `CPReg.00`-`.07` and `CPStrb3'` clocks `CPReg.08`-`.15`, and PARC
 *     numbers MSB-first, so `.00`-`.07` IS the high byte -- which is what the
 *     C emulator's ABMux0/ABMux1 comment says.
 *   - `SetRun` comes from data bit 0. The netlist latches `TSetRun` from
 *     `TCPBus.07`, and MSB-first over `.00`-`.07` makes `.07` the least
 *     significant bit; the C emulator tests `data & 0x01`.
 *
 * And two things the netlist adds, which are gaps in the C model rather than
 * disagreements:
 *
 *   - Function 1, which the C emulator calls "Clock" and ignores, really
 *     latches three control bits on ContA: `CP=UseCPReg`, `ClrReady` and
 *     `GetTLink`, from CPOut.2, .3, .4 and .7.
 *   - A REMOTE host can drive this bus instead of the local 6502. Three
 *     SN74LS157 multiplexers on the BaseBoard select between `MCPBus`/
 *     `MCPABus`/`MCPStrb` and `RCPBus`/`RCPABus`, which arrive on AM2615 line
 *     receivers from the `ACP*` cable, under a software-set `AHasCP` latch.
 *     The C emulator models only the local path.
 *
 * `apply_mcp_strobe` is static, so this includes baseboard.c directly rather
 * than linking it -- deliberately, and for the same reason alu_vectors.c
 * includes cpu.c: the test must run the emulator's REAL code, not a copy of it
 * that could drift.
 *
 * Output: one line per strobe, "mcpbusl data cpreg mir0 mir1 mir2 mir3 mir4
 * running ss mirloaded", all hex but the three flags.
 */

#include "../src/baseboard.c"

#include <stdio.h>
#include <string.h>

int main(void)
{
    /* The strobes a boot actually issues, plus the edges worth pinning:
     * every function code, both bytes of CPReg, all four MIR slots, the
     * extra bit high and low, and the Control bits that start and stop the
     * machine. Data patterns are the ones that catch a swapped or dropped
     * bit. */
    static const uint8_t data_patterns[] = {
        0x00, 0x01, 0x02, 0x80, 0x7F, 0xFF, 0x55, 0xAA, 0x0F, 0xF0, 0x3C
    };

    static dorado_baseboard bb;
    memset(&bb, 0, sizeof bb);

    for (unsigned fn = 0; fn < 8; fn++)
        for (int extra = 0; extra < 2; extra++)
            for (unsigned d = 0;
                 d < sizeof data_patterns / sizeof *data_patterns; d++) {
                /* MCPBusL: function code in bits 6:4, the ninth bit in bit 7,
                 * the strobe itself in bit 0 (which apply_mcp_strobe is
                 * called on the rising edge of, so it is set here). */
                uint8_t mcpbusl = (uint8_t)((fn << 4) | (extra << 7) | 0x01);
                bb.riot[3].pa_latch = data_patterns[d];
                apply_mcp_strobe(&bb, mcpbusl);
                printf("%02x %02x %04x %02x %02x %02x %02x %02x %d %d %d\n",
                       mcpbusl, data_patterns[d], bb.cpreg_to_dorado,
                       bb.mir_bytes[0], bb.mir_bytes[1], bb.mir_bytes[2],
                       bb.mir_bytes[3], bb.mir_bytes[4],
                       bb.dorado_running, bb.dorado_ss_pending,
                       bb.dorado_mir_loaded);
            }
    return 0;
}
