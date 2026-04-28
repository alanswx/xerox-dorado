#include "mb.h"
#include "disasm.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

/*
 * Smoke-test the model-1 unshuffle by feeding hand-built storage
 * patterns and checking the recovered logical iw0/iw1/iw2.
 */
static int test_unshuffle_known(void)
{
    /* From ftest.MB IM[0] (BEGIN), as observed via mbdis: the four
     * stored words are 0o42, 0o20040, 0o40000, 0o3. Per ftest.DLS the
     * displayed W0 = 0o104 and W1 = 0o100201. Our unshuffle recovers
     * orig_iw0 = 0o104 and orig_iw1 = 0o40100. The .DLS W1 column is
     * (orig_iw1 << 1) | JN1bit7 = (0o40100 << 1) | 1 = 0o100201. */
    uint16_t s0[4] = { 0042, 020040, 040000, 03 };
    dorado_uinstr u;
    dorado_decode_model1(s0, &u);
    EXPECT(u.iw0 == 0104,
           "BEGIN iw0: expected 0o104, got 0o%o", u.iw0);
    EXPECT(u.iw1 == 040100,
           "BEGIN iw1: expected 0o40100, got 0o%o", u.iw1);

    /* Confirm the ".DLS displayed W1" reconstruction:
     *    displayed = (iw1 << 1) | JN1bit7
     * where JN1bit7 lives in u->iw2 bit 14 (0x4000), which equals the
     * LSB of the JCN field. */
    uint8_t jn1bit7 = (u.iw2 >> 14) & 1;
    uint32_t displayed = ((uint32_t)u.iw1 << 1) | jn1bit7;
    EXPECT(displayed == 0100201,
           "BEGIN displayed W1: expected 0o100201, got 0o%o", displayed);

    /* Image 1 (FETCH1's predecessor at image 1, real 1):
     *    stored = 0o60046, 0o7750, 0, 0o1
     *    .DLS  W0 = 0o140114, W1 = 0o37640
     *    expected: iw0 = 0o140114, iw1 = 0o17720,
     *              displayed_W1 = (iw1 << 1) | 0 = 0o37640. */
    uint16_t s1[4] = { 060046, 07750, 0, 01 };
    dorado_decode_model1(s1, &u);
    EXPECT(u.iw0 == 0140114,
           "image 1 iw0: expected 0o140114, got 0o%o", u.iw0);
    EXPECT(u.iw1 == 017720,
           "image 1 iw1: expected 0o17720, got 0o%o", u.iw1);
    uint32_t displayed1 = ((uint32_t)u.iw1 << 1) | ((u.iw2 >> 14) & 1);
    EXPECT(displayed1 == 037640,
           "image 1 displayed W1: expected 0o37640, got 0o%o", displayed1);

    printf("PASS  test_unshuffle_known\n");
    return 0;
}

/*
 * Feed every IM entry from ftest.MB through the decoder and check that
 * (iw1 << 1) | JN1bit7 always equals the reading we'd get from the
 * MicroD listing's W1 column. We don't have the .DLS parsed here, so
 * we just sanity-check that the field decoder doesn't trip on any real
 * microcode and that BLOCK and brk_p stay within their declared widths.
 */
static int test_decode_all_ftest(void)
{
    const char *path = "../chm/dorado/expanded/ftest.dm!1_/ftest.MB";
    mb_file mb;
    mb_init(&mb);
    EXPECT(mb_load(&mb, path) == MB_OK, "load %s", path);

    int im = mb_find_mem(&mb, "IM");
    EXPECT(im >= 0, "no IM");
    const mb_memory *m = &mb.mems[im];

    int n_ok = 0;
    for (int a = 0; a < m->max_addr; a++) {
        if (!m->present[a]) continue;
        const uint16_t *slot = &m->data[(size_t)a * m->width_words];
        dorado_uinstr u;
        dorado_decode_model1(slot, &u);
        EXPECT(u.rstk < 16,  "rstk out of range at IM #%o: %u",  a, u.rstk);
        EXPECT(u.aluf < 16,  "aluf out of range at IM #%o: %u",  a, u.aluf);
        EXPECT(u.bsel < 8,   "bsel out of range at IM #%o: %u",  a, u.bsel);
        EXPECT(u.lc < 8,     "lc out of range at IM #%o: %u",    a, u.lc);
        EXPECT(u.asel < 8,   "asel out of range at IM #%o: %u",  a, u.asel);
        EXPECT(u.block < 2,  "block out of range at IM #%o: %u", a, u.block);
        EXPECT(u.brk_p < 4,  "brk_p out of range at IM #%o: %u", a, u.brk_p);
        n_ok++;
    }

    mb_free(&mb);
    printf("PASS  test_decode_all_ftest (%d instructions)\n", n_ok);
    return 0;
}

/*
 * Decode every IM entry of every Dorado microcode file and just check
 * the decoder doesn't crash and field widths stay valid.
 */
static int test_decode_all_microcode(void)
{
    static const char *paths[] = {
        "../chm/dorado/Mesa.mb!3",
        "../chm/dorado/Cedar.mb!6",
        "../chm/dorado/AEmu.mb!2",
        "../chm/dorado/DSemu.mb!1",
        "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb",
        "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb",
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
            printf("FAIL  decode %s (load failed)\n", paths[i]);
            mb_free(&mb);
            continue;
        }
        int im = mb_find_mem(&mb, "IM");
        if (im < 0) { mb_free(&mb); continue; }
        const mb_memory *m = &mb.mems[im];
        int total = 0, valid = 1;
        for (int a = 0; a < m->max_addr && valid; a++) {
            if (!m->present[a]) continue;
            const uint16_t *slot = &m->data[(size_t)a * m->width_words];
            dorado_uinstr u;
            dorado_decode_model1(slot, &u);
            if (u.rstk >= 16 || u.aluf >= 16 || u.bsel >= 8 ||
                u.lc >= 8 || u.asel >= 8 || u.block >= 2) {
                valid = 0;
            }
            total++;
        }
        if (valid) {
            printf("PASS  decode %-58s (%d uinstrs)\n", paths[i], total);
            passed++;
        } else {
            printf("FAIL  decode %s (field out of range)\n", paths[i]);
        }
        mb_free(&mb);
    }
    return passed == n ? 0 : 1;
}

/*
 * Verify dorado_decode_mir against a known Bootstrap IRTable entry.
 * Nop# from doradoboot.masm:
 *   RSTK[0],ALUF[0],BSEL[1],LC[0],ASEL[4],FF[77],JCN[201]
 * Encoded as the 5-byte microinstruction format for DoDoradoMicroInst:
 *   byte 0: 01^6.+(11^4.) = 0x70  (RSTK.0=0, P015=1, JCN.7=1, P1631=1)
 *   byte 1: 0000^4.+0001  = 0x01
 *   byte 2: 0000^4.+1111  = 0x0F
 *   byte 3: 0100^4.+1100  = 0x4C
 *   byte 4: 0100^4.+0000  = 0x40
 */
static int test_decode_mir_nop(void)
{
    uint8_t mir[5] = { 0x70, 0x01, 0x0F, 0x4C, 0x40 };
    dorado_uinstr u = {0};
    dorado_decode_mir(mir, &u);

    EXPECT(u.rstk  == 0,    "Nop RSTK = %u, expected 0",  u.rstk);
    EXPECT(u.aluf  == 0,    "Nop ALUF = %u, expected 0",  u.aluf);
    EXPECT(u.bsel  == 1,    "Nop BSEL = %u, expected 1",  u.bsel);
    EXPECT(u.lc    == 0,    "Nop LC = %u, expected 0",    u.lc);
    EXPECT(u.asel  == 4,    "Nop ASEL = %u, expected 4",  u.asel);
    EXPECT(u.block == 0,    "Nop BLOCK = %u, expected 0", u.block);
    EXPECT(u.ff    == 0x3F, "Nop FF = 0x%02X, expected 0x3F (0o77)", u.ff);
    EXPECT(u.jcn   == 0x81, "Nop JCN = 0x%02X, expected 0x81 (0o201)", u.jcn);

    /* Re-encoded iw0/iw1/iw2 should round-trip back to the same fields
     * after another redecode. */
    dorado_uinstr v = u;
    dorado_redecode_fields(&v);
    EXPECT(v.rstk == u.rstk && v.aluf == u.aluf && v.bsel == u.bsel &&
           v.lc   == u.lc   && v.asel == u.asel && v.block == u.block &&
           v.ff   == u.ff   && v.jcn  == u.jcn,
           "round-trip lost fields after redecode");

    printf("PASS  test_decode_mir_nop\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_unshuffle_known();
    rc |= test_decode_all_ftest();
    rc |= test_decode_all_microcode();
    rc |= test_decode_mir_nop();
    if (rc == 0) printf("\nAll disasm tests passed.\n");
    return rc;
}
