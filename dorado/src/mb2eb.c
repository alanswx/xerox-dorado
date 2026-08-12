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
 * A .eb may hold SEVERAL such segments back to back, each terminated by
 * its own End item carrying its own start address and its own balancing
 * checksum. That is what Xerox's LoadMB does when given more than one
 * .mb (chm/doradomicrocode/loadmb/LoadMB.mesa!7.txt DoLoadMb: the
 * FOR srcF loop calls AppendEndItem[nextItemAddr, srcF.startAddr] per
 * file and flushes the item array after each), and it is how the 1983
 * release script built the Smalltalk boot file
 * (chm/dorado/dsemu-src/DSemuRelease.cm):
 *
 *   LoadMB/e DoradoSmalltalk.eb/o        DSemu.mb 1076/s
 *   LoadMB/e DoradoInitialSmalltalk.eb/o InitialSelect.mb 406/s DSemu.mb 1076/s
 *
 * A numeric argument there is OCTAL and sets the start address of the
 * PRECEDING file (LoadMB.mesa: Convert.CardFromRope[this, 8] into
 * lastSrcFile.startAddr; unset means defaultStartAddress = 1076B).
 *
 * The consumer loads one segment and jumps to its start address, so the
 * second segment is not loaded by the boot path at all -- 0406 is
 * InitialSelect's InitialOverlayEntry, which per
 * chm/doradosource/BootstrapSources.dm!12_/InitialSelectMain.mc
 * "loads the next microcode overlay from the same .eb file that the
 * currently-running microcode came from". The overlay IS segment 2.
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

/* LoadMB.mesa!7 NotOddParity, transcribed literally. Returns
 * ParityWord.parity, which is Mesa bit 0:15 -- i.e. the LSB. */
static unsigned not_odd_parity(uint16_t w1, uint16_t w2)
{
    uint16_t value = (uint16_t)(w1 ^ w2);
    value ^= (uint16_t)(value >> 8);
    value ^= (uint16_t)(value >> 4);
    unsigned shft = value & 017u;
    uint16_t final = (uint16_t)~(uint16_t)(0113151u >> shft);
    return final & 1u;
}

/* Masks defining each word's contribution to each IFUM parity bit
 * (LoadMB.mesa!7 ifumW0P0..ifumW1P2). */
#define IFUM_W0_P0 01400u
#define IFUM_W1_P0 0317u
#define IFUM_W0_P1 0377u
#define IFUM_W1_P1 0u
#define IFUM_W0_P2 02000u
#define IFUM_W1_P2 0107460u

/* IFUMWord1 places sgn at Mesa 0:0 and p0/p1/p2 at 0:1, 0:2, 0:3 --
 * bits 14, 13, 12 counting from the LSB. LoadMB computes them into the
 * word before emitting the item, with the comment: "Despite what the
 * manual says, the hardware really wants to see even parity - hence the
 * 'not' in the following statements". */
static uint16_t ifum_word1_with_parity(uint16_t w0, uint16_t w1)
{
    unsigned p0 = not_odd_parity((uint16_t)(w0 & IFUM_W0_P0),
                                 (uint16_t)(w1 & IFUM_W1_P0));
    unsigned p1 = not_odd_parity((uint16_t)(w0 & IFUM_W0_P1),
                                 (uint16_t)(w1 & IFUM_W1_P1));
    unsigned p2 = not_odd_parity((uint16_t)(w0 & IFUM_W0_P2),
                                 (uint16_t)(w1 & IFUM_W1_P2));
    return (uint16_t)((w1 & ~0x7000u) | (p0 << 14) | (p1 << 13) | (p2 << 12));
}

/* Emit one segment: every item of `mc`, then the End item whose checksum
 * balances THIS segment (Initial's CheckChecksumAndLoad sums until End).
 * Mirrors LoadMB's ReadMBFile + AppendEndItem, which rebuild the item
 * array from its base for each source file. */
static int write_segment(FILE *o, const dorado_microcode *mc, unsigned start,
                         const char *label)
{
    size_t cap = 1u << 20, n = 0;
    uint16_t *buf = malloc(cap * sizeof *buf);
    if (!buf) { fprintf(stderr, "mb2eb: oom\n"); return -1; }
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
        if (!mc->im_present[a]) continue;
        if (a >= 07600 && a <= 07677) { n_skip++; continue; }
        const dorado_uinstr *u = &mc->im[a];
        unsigned rstk0 = (u->rstk >> 3) & 1;
        /* CSItem control nibble, MSB..LSB = pe020, rstk0, pe2131, blk
         * (LoadMB.mesa!7 CSItem). LoadMB copies pe020/pe2131 straight
         * out of the .MB (IMWord 2:2 and 2:3, which the model-1
         * unshuffle puts at iw2 bits 13 and 12 = brk_p) -- these mark a
         * microinstruction stored with DELIBERATELY BAD PARITY, which is
         * how a trap slot is armed. Emitting 0 here silently disarms it. */
        unsigned pe020  = (u->brk_p >> 1) & 1;
        unsigned pe2131 = u->brk_p & 1;
        unsigned extra = (pe020 << 3) | (rstk0 << 2) | (pe2131 << 1)
                       | (u->block & 1);
        ITEM((extra << 12) | 0, a, u->iw0, ((unsigned)u->ff << 8) | u->jcn);
        n_im++;
    }
    /* IFUM items. */
    for (int a = 0; a < 1024; a++) {
        if (!mc->ifum_present[a]) continue;
        /* Canonical LoadRam.mc item order: word0 = address half (LH,
         * IFUMLH←word0 → ifum_lo), word1 = fields half (RH,
         * IFUMRH←word1 → ifum_hi). LoadMB recomputes the three IFUM
         * parity bits into word1 before emitting; the .MB's own values
         * are not used. */
        ITEM((0 << 12) | 1, a, mc->ifum_lo[a],
             ifum_word1_with_parity(mc->ifum_lo[a], mc->ifum_hi[a]));
        n_ifum++;
    }
    /* RM items. */
    for (int a = 0; a < 256; a++) {
        if (!mc->rm_present[a]) continue;
        ITEM((0 << 12) | 3, a, mc->rm[a], 0);
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

    for (size_t i = 0; i < n; i++) put_be(o, buf[i]);

    fprintf(stderr,
            "mb2eb: segment %s  IM=%d IFUM=%d RM=%d (skipped %d LoadRamPage), "
            "%zu payload words, cksum=0o%o start=0o%o\n",
            label, n_im, n_ifum, n_rm, n_skip, n, cksum, start);
    free(buf);
    return 0;
#undef ITEM
#undef PUSH
}

static void write_eb_header(FILE *o)
{
    put_be(o, 1);                                   /* version */
    for (int i = 1; i < 256; i++) put_be(o, 0);     /* rest of header */
}

int main(int argc, char **argv)
{
    /* Three modes:
     *   mb2eb in.mb out.eb [start_octal=1076]
     *   mb2eb -l out.eb start_octal layer1.mb layer2.mb ...
     *   mb2eb -s out.eb in1.mb start1_octal [in2.mb start2_octal ...]
     *
     * The layered mode MERGES several .mb files (base layer first, later
     * layers override) into ONE segment, exactly as probe_aemu does
     * (dorado_microcode_load for the base, dorado_microcode_layer_load for
     * the rest). This is what is needed to serialize a *complete* Alto-
     * emulator world (Initial+kernel+memMisc+IfuComplex+AEmu) so real
     * Initial -> LoadRam installs a runnable world, not just one layer.
     *
     * The segment mode CONCATENATES them instead: one independent load
     * per .mb, each with its own start address and its own End item, the
     * way LoadMB writes a multi-file .eb. -l and -s are not
     * interchangeable; see the file header for which artifact needs which.
     * Starts are octal here to match LoadMB's `NNN/s`. */
    int layered = (argc >= 2 && strcmp(argv[1], "-l") == 0);
    int segmented = (argc >= 2 && strcmp(argv[1], "-s") == 0);
    if (!layered && !segmented && argc < 3) {
        fprintf(stderr,
                "usage: mb2eb in.mb out.eb [start_addr_octal=1076]\n"
                "       mb2eb -l out.eb start_octal layer1.mb [layer2.mb ...]\n"
                "       mb2eb -s out.eb in1.mb start1_octal [in2.mb start2_octal ...]\n");
        return 2;
    }

    const char *out_path;
    unsigned start;
    static dorado_microcode mc;

    if (segmented) {
        /* -s out.eb (file start)+ : one segment per pair, in order. */
        if (argc < 5 || ((argc - 3) % 2) != 0) {
            fprintf(stderr, "mb2eb -s: need out.eb then (file.mb start_octal) pairs\n");
            return 2;
        }
        out_path = argv[2];
        FILE *o = fopen(out_path, "wb");
        if (!o) { perror("mb2eb: out"); return 1; }
        write_eb_header(o);
        for (int i = 3; i + 1 < argc; i += 2) {
            const char *path = argv[i];
            /* Octal, as LoadMB parses it: Convert.CardFromRope[this, 8]. */
            unsigned seg_start = (unsigned)strtol(argv[i + 1], NULL, 8);
            mb_file mb;
            mb_init(&mb);
            if (mb_load(&mb, path) != MB_OK) {
                fprintf(stderr, "mb2eb: cannot load %s\n", path);
                fclose(o);
                return 1;
            }
            memset(&mc, 0, sizeof mc);   /* ReadMBFile rebuilds from the base */
            if (dorado_microcode_load(&mb, &mc) != DM_OK) {
                fprintf(stderr, "mb2eb: placement failed for %s\n", path);
                mb_free(&mb);
                fclose(o);
                return 1;
            }
            mb_free(&mb);
            if (write_segment(o, &mc, seg_start, path) != 0) {
                fclose(o);
                return 1;
            }
        }
        long bytes = ftell(o);
        fclose(o);
        fprintf(stderr, "mb2eb: wrote %s  %ld bytes, %d segments\n",
                out_path, bytes, (argc - 3) / 2);
        return 0;
    }

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

    FILE *o = fopen(out_path, "wb");
    if (!o) { perror("mb2eb: out"); return 1; }
    write_eb_header(o);
    if (write_segment(o, &mc, start, out_path) != 0) {
        fclose(o);
        return 1;
    }
    long bytes = ftell(o);
    fclose(o);

    fprintf(stderr, "mb2eb: wrote %s  %ld bytes\n", out_path, bytes);
    return 0;
}
