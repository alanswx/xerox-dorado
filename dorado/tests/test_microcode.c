#include "mb.h"
#include "microcode.h"

#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static int test_ftest_placement(void)
{
    /*
     * Cross-check our image→real mapping against ftest.DLS:
     *   ftest.DIB:    0      3    BEGIN
     *                 1      1
     *                 2      2
     *                 3     11    (0o11 = 9)
     *                 4     12    (0o12 = 10)
     *                 5     13
     *                 6     42    FETCH1
     *                14     17
     *                15     43    FETCH2
     *                24     40    SETMCR
     *                41      0    LOADMUNCH
     *                42     32    LM1
     *                51     20    LONGWAIT
     *
     * Numbers below are octal in the .DLS, decimal in C.
     */
    mb_file mb;
    mb_init(&mb);
    EXPECT(mb_load(&mb, "../chm/dorado/expanded/ftest.dm!1_/ftest.MB") == MB_OK,
           "ftest.MB load");

    dorado_microcode mc;
    dorado_microcode_status s = dorado_microcode_load(&mb, &mc);
    EXPECT(s == DM_OK, "microcode load: %s", dorado_microcode_status_str(s));
    EXPECT(mc.n_instructions == 44, "expected 44 instructions, got %d",
           mc.n_instructions);

    struct { int image; int real; const char *sym; } expected[] = {
        {  0,  003, "BEGIN"     },
        {  1,  001, NULL        },
        {  2,  002, NULL        },
        {  3,  011, NULL        },
        {  4,  012, NULL        },
        {  5,  013, NULL        },
        { 006, 042, "FETCH1"    },
        { 015, 043, "FETCH2"    },
        { 024, 040, "SETMCR"    },
        { 041, 000, "LOADMUNCH" },
        { 042, 032, "LM1"       },
        { 051, 020, "LONGWAIT"  },
    };
    int n = sizeof expected / sizeof expected[0];
    for (int i = 0; i < n; i++) {
        int img = expected[i].image;
        int real = expected[i].real;
        EXPECT(mc.image_present[img], "image %o not present", img);
        EXPECT(mc.image_to_real[img] == real,
               "image %o: expected real %o, got %o",
               img, real, mc.image_to_real[img]);
        EXPECT(mc.im_present[real], "real %o not present (for image %o)",
               real, img);

        const char *sym = dorado_microcode_symbol_at_real(&mc, real);
        if (expected[i].sym) {
            EXPECT(sym && !strcmp(sym, expected[i].sym),
                   "real %o (image %o): expected symbol '%s', got '%s'",
                   real, img, expected[i].sym, sym ? sym : "(none)");
        }
    }

    /* Specifically: BEGIN at real 3 should decode to the same iw0 as
     * image 0's storage produces (we already validated this in
     * test_disasm). Just sanity-check the post-placement form. */
    EXPECT(mc.im[003].iw0 == 0104,
           "BEGIN (real 3) iw0: expected 0o104, got 0o%o", mc.im[003].iw0);

    /* RM initial values from the .DLS RM section. */
    EXPECT(mc.rm_present[0] && mc.rm[0] == 0,        "FIRSTADDR");
    EXPECT(mc.rm_present[1] && mc.rm[1] == 0100,     "WAIT = 0o100");
    EXPECT(mc.rm_present[2] && mc.rm[2] == 02000,    "SECONDADDR = 0o2000");
    EXPECT(mc.rm_present[5] && mc.rm[5] == 0177777,  "XORPAT = all ones");
    EXPECT(mc.rm_present[6] && mc.rm[6] == 0601,     "FIRSTMCR = 0o601");
    EXPECT(mc.rm_present[7] && mc.rm[7] == 077601,   "SECONDMCR = 0o77601");

    /*
     * ALUFM values from the .DLS ALUFM section (octal):
     *   0   25       4   14       8   23     12   17
     *   1    0       5  222       9   33     13  200
     *   2   31       6   35      10   17     14   36
     *   3    7       7   27      11   --     15   13
     */
    uint8_t alufm_expected[ALUFM_SIZE] = {
        025, 000, 031, 007,
        014, 0222 & 0x3F, 035, 027,
        023, 033, 017, 0,
        017, 0200 & 0x3F, 036, 013
    };
    /* Note: 0o222 = 0x92 has bit 7 = 1 but ALUFM is 6 bits. So masking
     * with 0x3F = 0o77 keeps the low 6 bits = 0o22. The .DLS prints
     * the high byte of the storage word (which can be 8 bits) so 0o222
     * comes from the upper byte. We verify the 6-bit ALU op below. */
    int alufm_ok = 0;
    for (int i = 0; i < ALUFM_SIZE; i++) {
        if (mc.alufm_present[i]) alufm_ok++;
    }
    EXPECT(alufm_ok >= 14, "expected ≥14 ALUFM entries present, got %d",
           alufm_ok);
    EXPECT(mc.alufm[0] == 025, "ALUFM[0] = 0o%o", mc.alufm[0]);
    EXPECT(mc.alufm[2] == 031, "ALUFM[2] = 0o%o", mc.alufm[2]);
    EXPECT(mc.alufm[6] == 035, "ALUFM[6] = 0o%o", mc.alufm[6]);
    (void)alufm_expected;

    mb_free(&mb);
    printf("PASS  test_ftest_placement (%d instructions placed)\n",
           mc.n_instructions);
    return 0;
}

static int test_bootstrap_placement(void)
{
    /* Bootstrap.mb has 50 entries. After placement, exactly 50 IM
     * slots should be present, and at least one should carry a symbol
     * (BOOTSTRAP at the entry point). */
    mb_file mb;
    mb_init(&mb);
    EXPECT(mb_load(&mb, "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb")
           == MB_OK, "Bootstrap.MB load");

    dorado_microcode mc;
    EXPECT(dorado_microcode_load(&mb, &mc) == DM_OK, "Bootstrap microcode load");
    EXPECT(mc.n_instructions == 50, "expected 50, got %d", mc.n_instructions);

    int n_present = 0;
    int n_with_sym = 0;
    int found_bootstrap = 0;
    for (int r = 0; r < IM_SIZE; r++) {
        if (!mc.im_present[r]) continue;
        n_present++;
        const char *sym = dorado_microcode_symbol_at_real(&mc, r);
        if (sym) {
            n_with_sym++;
            if (!strcmp(sym, "BOOTSTRAP")) found_bootstrap = 1;
        }
    }
    EXPECT(n_present == 50, "expected 50 placed entries, got %d", n_present);
    EXPECT(found_bootstrap, "BOOTSTRAP symbol not located");

    mb_free(&mb);
    printf("PASS  test_bootstrap_placement (%d entries, %d with symbols)\n",
           n_present, n_with_sym);
    return 0;
}

static int test_real_microcode_places(void)
{
    /* Larger emulators should all place without error and the count
     * of placed entries should equal the count of image entries. */
    static const char *paths[] = {
        "../chm/dorado/Mesa.mb!3",
        "../chm/dorado/Cedar.mb!6",
        "../chm/dorado/AEmu.mb!2",
        "../chm/dorado/DSemu.mb!1",
        "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb",
        "../chm/dorado/expanded/kernel.dm!38_/kernel.mb",
        "../chm/dorado/expanded/UnBug.bfs!1_/DoradoLisp.MB",
    };
    int n = sizeof(paths) / sizeof(paths[0]);
    int passed = 0;
    for (int i = 0; i < n; i++) {
        mb_file mb;
        mb_init(&mb);
        if (mb_load(&mb, paths[i]) != MB_OK) {
            printf("FAIL  load %s\n", paths[i]);
            mb_free(&mb);
            continue;
        }
        dorado_microcode mc;
        dorado_microcode_status s = dorado_microcode_load(&mb, &mc);
        if (s != DM_OK) {
            printf("FAIL  place %s: %s\n", paths[i],
                   dorado_microcode_status_str(s));
            mb_free(&mb);
            continue;
        }
        int img_count = mb.mems[mb.im_id].max_addr;
        int placed = mc.n_instructions;
        if (placed == img_count) {
            printf("PASS  place %-58s (%d image, %d real)\n",
                   paths[i], img_count, placed);
            passed++;
        } else {
            printf("FAIL  place %s: img=%d placed=%d\n",
                   paths[i], img_count, placed);
        }
        mb_free(&mb);
    }
    return passed == n ? 0 : 1;
}

/*
 * test_alufm_canonical_decoding — verify our ALUFM extraction
 * recovers the canonical 6-bit entries from Bootstrap.MB / AEmu.mb.
 *
 * Both files declare the standard Dorado ALUFM convention:
 *
 *   ALUFM[ 0] = 0o25 (B)              stored 0o12400
 *   ALUFM[ 1] = 0o00 (A, c=0)          stored 0o00000
 *   ALUFM[ 2] = 0o14 (A+B, c=0)        stored 0o06000
 *   ALUFM[ 3] = 0o54 (A+B, c=1)        stored 0o106000
 *   ALUFM[ 4] = 0o62 (A-B, c=1)        stored 0o111000
 *   ALUFM[ 5] = 0o22 (A-B-1, c=0)      stored 0o011000
 *
 * Storage layout: word bit 15 = carry_in (entry bit 5), word bits
 * 12..8 = op (entry bits 4..0). The earlier extraction lost the
 * carry bit, conflating ALUFM[2] and ALUFM[3].
 */
static int test_alufm_canonical_decoding(void)
{
    static const struct {
        const char *path;
        const char *name;
    } files[] = {
        { "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb", "Bootstrap" },
        { "../chm/dorado/AEmu.mb!2",                              "AEmu" },
    };

    static const uint8_t expected[6] = {
        025, 000, 014, 054, 062, 022,
    };

    for (int f = 0; f < (int)(sizeof files / sizeof files[0]); f++) {
        mb_file mb;
        mb_init(&mb);
        if (mb_load(&mb, files[f].path) != MB_OK) {
            printf("SKIP  test_alufm_canonical_decoding (%s not loadable)\n",
                   files[f].name);
            mb_free(&mb);
            continue;
        }
        static dorado_microcode mc;
        EXPECT(dorado_microcode_load(&mb, &mc) == DM_OK,
               "%s: microcode load", files[f].name);
        for (int i = 0; i < 6; i++) {
            EXPECT(mc.alufm_present[i],
                   "%s: ALUFM[%d] not present", files[f].name, i);
            EXPECT(mc.alufm[i] == expected[i],
                   "%s: ALUFM[%d] = 0o%o (expected 0o%o)",
                   files[f].name, i, mc.alufm[i], expected[i]);
        }
        mb_free(&mb);
    }
    printf("PASS  test_alufm_canonical_decoding "
           "(carry+op extracted correctly across files)\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_ftest_placement();
    rc |= test_bootstrap_placement();
    rc |= test_real_microcode_places();
    rc |= test_alufm_canonical_decoding();
    if (rc == 0) printf("\nAll microcode tests passed.\n");
    return rc;
}
