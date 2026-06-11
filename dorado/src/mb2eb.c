/*
 * mb2eb — convert a MicroD .MB microcode file into an Ethernet-boot .eb
 * image that the real Initial microprogram can load via LoadRam.
 *
 * This is the "correct" way to bring up a standalone emulator (e.g. the
 * Alto emulator AEmu.mb) over the Ethernet boot path: rather than a
 * boot-bypass that jumps straight to STARTEMULATOR with hand-planted
 * state, we let real Initial load the emulator and hand off through
 * InitMap (0o1076), which establishes MDS / BRs / the map / RTClock.
 *
 * .eb file layout (consumed by the in-process Pup boot server, which
 * checks word 0 == version 1 and then skips the first 256 words):
 *   words 0..255 : header. word 0 = 1 (microcode version). rest 0.
 *   words 256..  : a LoadRam "Item" array, then an End item.
 *
 * LoadRam Item format (chm/.../LoadRam.mc!1): each Item is 4 words
 *   [ control, addr, word0, word1 ]
 * where control = (extraIM << 12) | type, type in {IM=0, IFUM=1,
 * End=2, RM=3}, and extraIM nibble (MSB..LSB) =
 * {LHparityBad, RSTK[0], RHparityBad, BLOCK}.
 *   IM   : addr=real IM address, word0=iw0, word1=(FF<<8)|JCN.
 *          Ignored by LoadRam if addr is in LoadRamPage [7600..7677].
 *   IFUM : addr=IFUM index, word0=LH (fields = .MB word 1),
 *          word1=RH (PackedA,,IFaddr' = .MB word 0). LoadRam.mc:
 *          "IFUMLH_ Q (Load LH from word0) ... IFUMRH_ LRTemp1 (Load
 *          RH from word1)".
 *   RM   : addr=RM index, word0=RM value, word1=0.
 *   End  : word0=checksum, word1=start address.
 *
 * Checksum (Initial CheckChecksumAndLoad): the 16-bit sum of every
 * loaded payload word (all items incl. the End item) must be 0, so the
 * End item's checksum = -(sum of all other payload words).
 */
#include "mb.h"
#include "microcode.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void put_be(FILE *f, uint16_t w)
{
    fputc((w >> 8) & 0xFF, f);
    fputc(w & 0xFF, f);
}

int main(int argc, char **argv)
{
    /* Two modes:
     *   mb2eb in.mb out.eb [start_octal=1076]
     *   mb2eb -l out.eb start_octal layer1.mb layer2.mb ...
     * The layered mode merges several .mb files (base layer first, later
     * layers override) into one complete world, exactly as probe_aemu does
     * (dorado_microcode_load for the base, dorado_microcode_layer_load for
     * the rest). This is what is needed to serialize a *complete* Alto-
     * emulator world (Initial+kernel+memMisc+IfuComplex+AEmu) so real
     * Initial -> LoadRam installs a runnable world, not just one layer. */
    int layered = (argc >= 2 && strcmp(argv[1], "-l") == 0);
    if (!layered && argc < 3) {
        fprintf(stderr,
                "usage: mb2eb in.mb out.eb [start_addr_octal=1076]\n"
                "       mb2eb -l out.eb start_octal layer1.mb [layer2.mb ...]\n");
        return 2;
    }

    const char *out_path;
    unsigned start;
    static dorado_microcode mc;

    if (layered) {
        if (argc < 5) {
            fprintf(stderr, "mb2eb -l: need out.eb, start, >=1 layer\n");
            return 2;
        }
        out_path = argv[2];
        start = (unsigned)strtol(argv[3], NULL, 0);
        for (int i = 4; i < argc; i++) {
            mb_file mb;
            mb_init(&mb);
            if (mb_load(&mb, argv[i]) != MB_OK) {
                fprintf(stderr, "mb2eb: cannot load layer %s\n", argv[i]);
                return 1;
            }
            dorado_microcode_status st = (i == 4)
                ? dorado_microcode_load(&mb, &mc)
                : dorado_microcode_layer_load(&mb, &mc);
            if (st != DM_OK) {
                fprintf(stderr, "mb2eb: placement failed for %s\n", argv[i]);
                mb_free(&mb);
                return 1;
            }
            fprintf(stderr, "mb2eb: merged layer %s\n", argv[i]);
            mb_free(&mb);
        }
    } else {
        out_path = argv[2];
        start = (argc > 3) ? (unsigned)strtol(argv[3], NULL, 0) : 01076;
        mb_file mb;
        mb_init(&mb);
        if (mb_load(&mb, argv[1]) != MB_OK) {
            fprintf(stderr, "mb2eb: cannot load %s\n", argv[1]);
            return 1;
        }
        if (dorado_microcode_load(&mb, &mc) != DM_OK) {
            fprintf(stderr, "mb2eb: microcode placement failed\n");
            mb_free(&mb);
            return 1;
        }
        mb_free(&mb);
    }

    size_t cap = 1u << 20, n = 0;
    uint16_t *buf = malloc(cap * sizeof *buf);
    if (!buf) { fprintf(stderr, "mb2eb: oom\n"); return 1; }
#define PUSH(w) do { \
        if (n + 1 > cap) { cap *= 2; buf = realloc(buf, cap * sizeof *buf); } \
        buf[n++] = (uint16_t)(w); \
    } while (0)
#define ITEM(ctl, addr, w0, w1) do { \
        PUSH(ctl); PUSH(addr); PUSH(w0); PUSH(w1); \
    } while (0)

    int n_im = 0, n_ifum = 0, n_rm = 0, n_skip = 0;

    /* IM items (indexed by real address; skip LoadRamPage 7600..7677). */
    for (int a = 0; a < 4096; a++) {
        if (!mc.im_present[a]) continue;
        if (a >= 07600 && a <= 07677) { n_skip++; continue; }
        const dorado_uinstr *u = &mc.im[a];
        unsigned rstk0 = (u->rstk >> 3) & 1;
        unsigned extra = (rstk0 << 2) | (u->block & 1);  /* parity bits 0 */
        ITEM((extra << 12) | 0, a, u->iw0, ((unsigned)u->ff << 8) | u->jcn);
        n_im++;
    }
    /* IFUM items. */
    for (int a = 0; a < 1024; a++) {
        if (!mc.ifum_present[a]) continue;
        ITEM((0 << 12) | 1, a, mc.ifum_hi[a], mc.ifum_lo[a]);
        n_ifum++;
    }
    /* RM items. */
    for (int a = 0; a < 256; a++) {
        if (!mc.rm_present[a]) continue;
        ITEM((0 << 12) | 3, a, mc.rm[a], 0);
        n_rm++;
    }

    /* End item with the balancing checksum. */
    uint16_t sum = 0;
    for (size_t i = 0; i < n; i++) sum += buf[i];
    sum += 2;                       /* End control */
    sum += 0;                       /* End addr */
    sum += (uint16_t)start;         /* End word1 */
    uint16_t cksum = (uint16_t)(0 - sum);
    ITEM(2, 0, cksum, (uint16_t)start);

    FILE *o = fopen(out_path, "wb");
    if (!o) { perror("mb2eb: out"); free(buf); return 1; }
    put_be(o, 1);                                   /* version */
    for (int i = 1; i < 256; i++) put_be(o, 0);     /* rest of header */
    for (size_t i = 0; i < n; i++) put_be(o, buf[i]);
    fclose(o);

    fprintf(stderr,
            "mb2eb: wrote %s  IM=%d IFUM=%d RM=%d (skipped %d LoadRamPage), "
            "%zu payload words, cksum=0o%o start=0o%o\n",
            out_path, n_im, n_ifum, n_rm, n_skip, n, cksum, start);
    free(buf);
    return 0;
}
