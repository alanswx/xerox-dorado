/*
 * boot0_hunks -- pack real microcode into PARC's 17-byte hunks, and say what
 * IM should hold once they are loaded.
 *
 * The RTL's Boot0 loader (verilog/verilator/tb_boot0.sv) reads what this
 * prints, walks the hunks in through the BaseBoard's control-processor bus,
 * and compares IM against the expected words. Both sides therefore come from
 * REAL Xerox microcode by different routes: this one through the C emulator's
 * .MB loader, the other through 4,096 words of modelled ECL RAM.
 *
 * HUNK FORMAT, from doradoboot.masm's SendAHunk: seventeen bytes, being one
 * ExtraBits byte and then eight half-microinstructions of two bytes each --
 * eight halves of seventeen bits is 136 bits. Four microinstructions per hunk.
 * ExtraBits carries the halves' secondary bits, MSB first, and each half's two
 * data bytes go HIGH FIRST.
 *
 * WHAT A HALF CARRIES is cpu.c's Write-IM, read the other way round:
 *
 *   left  half   B = iw0                          secondary = iw2 bit 15 (RSTK[0])
 *   right half   B = (iw1[14:0] << 1) | iw2[14]   secondary = iw1 bit 15 (BLOCK)
 *
 * Output is one line per hunk:
 *
 *   HUNK <imaddr> <17 bytes, hex> <8 half-words, hex>  # iw0/iw1/iw2 ...
 *
 * The eight half-words are what the array holds and so what the RTL can read
 * back; the trailing comment carries iw0/iw1/iw2 for checking against mbdis.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mb.h"
#include "microcode.h"
#include "disasm.h"

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "../chm/dorado/AEmu.mb!2";
    int first = (argc > 2) ? (int)strtol(argv[2], NULL, 0) : -1;
    int nhunks = (argc > 3) ? (int)strtol(argv[3], NULL, 0) : 4;

    static mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        fprintf(stderr, "boot0_hunks: cannot load %s\n", path);
        return 1;
    }
    static dorado_microcode mc;
    dorado_microcode_status st = dorado_microcode_load(&mb, &mc);
    if (st != DM_OK) {
        fprintf(stderr, "boot0_hunks: cannot decode %s: %s\n", path,
                dorado_microcode_status_str(st));
        return 1;
    }

    /* Default: the first address with four consecutive present words, so the
     * hunk is real microcode rather than holes. */
    if (first < 0) {
        for (int r = 0; r + 3 < IM_SIZE; r++) {
            int ok = 1;
            for (int k = 0; k < 4; k++) if (!mc.im_present[r + k]) { ok = 0; break; }
            if (ok) { first = r; break; }
        }
    }
    if (first < 0) { fprintf(stderr, "boot0_hunks: no four consecutive words\n"); return 1; }

    printf("# %s  first=0%o nhunks=%d\n", path, first, nhunks);
    for (int h = 0; h < nhunks; h++) {
        int base = first + h * 4;
        if (base + 3 >= IM_SIZE) break;
        unsigned char hunk[17];
        unsigned extra = 0;
        int bit = 7;                       /* ExtraBits is shifted MSB first */
        int p = 1;
        for (int k = 0; k < 4; k++) {
            const dorado_uinstr *u = &mc.im[base + k];
            unsigned bl = u->iw0;                              /* left  half */
            unsigned sl = (u->iw2 >> 15) & 1;
            unsigned br = (unsigned)(((u->iw1 & 0x7FFFu) << 1) |
                                     ((u->iw2 >> 14) & 1));    /* right half */
            unsigned sr = (u->iw1 >> 15) & 1;
            hunk[p++] = (unsigned char)(bl >> 8); hunk[p++] = (unsigned char)(bl & 0xFF);
            if (sl) extra |= (1u << bit); bit--;
            hunk[p++] = (unsigned char)(br >> 8); hunk[p++] = (unsigned char)(br & 0xFF);
            if (sr) extra |= (1u << bit); bit--;
        }
        hunk[0] = (unsigned char)extra;

        /* The eight HALF-WORDS as the array will hold them -- that is what
         * the RTL can read back. iw0/iw1/iw2 follow as a comment so a human
         * can check against mbdis. */
        printf("HUNK %04x", base);
        for (int i = 0; i < 17; i++) printf(" %02x", hunk[i]);
        for (int k = 0; k < 4; k++) {
            const dorado_uinstr *u = &mc.im[base + k];
            printf(" %04x %04x", u->iw0,
                   (unsigned)(((u->iw1 & 0x7FFFu) << 1) | ((u->iw2 >> 14) & 1)));
        }
        printf("  #");
        for (int k = 0; k < 4; k++)
            printf(" %04x/%04x/%04x", mc.im[base + k].iw0,
                   mc.im[base + k].iw1, mc.im[base + k].iw2);
        printf("\n");
    }
    return 0;
}
