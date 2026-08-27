/*
 * im_image -- emit a whole microcode world as IM field bits, for the RTL to
 * PRELOAD.
 *
 * WHY THIS EXISTS. Walking 4,096 microinstructions into IM over the
 * control-processor bus is not viable in simulation: boot0-test does sixteen
 * and exec-test four hunks. A bench that wants to RUN a real world must write
 * the arrays directly. That skips nothing unproven -- boot0-test already gates
 * the CP-bus path -- but it must be VERIFIED by reading the arrays back, which
 * is what this file's output is compared against.
 *
 * THE TARGET LAYOUT, from tools/sil_im_map.py (gated by im-map-check):
 *   144 F10415A on ContB = 36 field bits x 4 banks, each 1024 x 1
 *   field bits  RSTK.0-3 ALUF.0-3 BSEL.0-2 LC.0-2 ASEL.0-2 FF.0-7 JCN.0-7
 *               Block' IMLH IMRH  = 36
 *   bank  = {addr[11], addr[0]}   the TOP and BOTTOM address bits
 *   index = addr[10:1]
 *
 * The two parity bits are emitted as zero. The machine checks IM parity and
 * stops on a mismatch, so a preloaded run must clear the parity enables the way
 * exec-test does -- computing real parity here would be a second, unverified
 * model of something the hardware already generates.
 *
 * OUTPUT, one line per PRESENT address:
 *   IM <addr> <rstk> <aluf> <bsel> <lc> <asel> <block> <ff> <jcn>
 * all hex, no 0x. Absent addresses are skipped, so the count tells you how much
 * of the world is really there.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mb.h"
#include "microcode.h"
#include "disasm.h"

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "chm/dorado/AEmu.mb!2";

    static mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        fprintf(stderr, "im_image: cannot load %s\n", path);
        return 1;
    }
    static dorado_microcode mc;
    dorado_microcode_status st = dorado_microcode_load(&mb, &mc);
    if (st != DM_OK) {
        fprintf(stderr, "im_image: cannot decode %s: %s\n", path,
                dorado_microcode_status_str(st));
        return 1;
    }

    int n = 0;
    printf("# %s -- IM field bits for RTL preload\n", path);
    for (int a = 0; a < IM_SIZE; a++) {
        if (!mc.im_present[a]) continue;
        const dorado_uinstr *u = &mc.im[a];
        printf("IM %03x %x %x %x %x %x %x %02x %02x\n",
               a, u->rstk & 0xF, u->aluf & 0xF, u->bsel & 7, u->lc & 7,
               u->asel & 7, u->block & 1, u->ff & 0xFF, u->jcn & 0xFF);
        n++;
    }
    /* ALUFM -- the sixteen ALU operations this world uses. ALUF is a POINTER
     * into this memory, not an opcode, so a world without it has whatever the
     * array powered up with. One line per present entry:
     *
     *   ALUFM <idx> <6-bit entry>
     *
     * The entry is bit 5 = B.08 (the ALU's carry in) and bits 4..0 = B.11-15,
     * which is HM Table 11d's "ALUFMEM <- B.8, B[11:15]". */
    int na = 0;
    for (int a = 0; a < 16; a++) {
        if (!mc.alufm_present[a]) continue;
        printf("ALUFM %x %02x\n", a, mc.alufm[a] & 077);
        na++;
    }
    /* IFUM -- the opcode decode tables, 256 entries per instruction set. The
     * IFU cannot dispatch without them, so a world loaded with IM alone takes
     * the IFU fault handlers and never reaches an opcode implementation.
     * Two words per entry, exactly as the .MB stores them and as the board's
     * two write enables (DecHi'/DecLo') take them:
     *
     *   IFUM <addr> <word0 = ifum_lo> <word1 = ifum_hi>
     */
    int ni = 0;
    for (int a = 0; a < IFUM_SIZE; a++) {
        if (!mc.ifum_present[a]) continue;
        printf("IFUM %03x %04x %04x\n", a, mc.ifum_lo[a] & 0xFFFF,
               mc.ifum_hi[a] & 0xFFFF);
        ni++;
    }
    fprintf(stderr,
            "im_image: %d of %d addresses present in %s; %d ALUFM, %d IFUM\n",
            n, IM_SIZE, path, na, ni);
    return 0;
}
