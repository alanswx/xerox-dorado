#include "mb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Minimal smoke tests for the .MB loader.
 *
 * These are integration tests: they load real microcode binaries from
 * the chm/ tree and assert structural facts that we know from the
 * MicroD listing (e.g. ftest.DLS).
 */

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static int test_ftest_load(void)
{
    const char *path = "../chm/dorado/expanded/ftest.dm!1_/ftest.MB";

    mb_file mb;
    mb_init(&mb);

    mb_status s = mb_load(&mb, path);
    EXPECT(s == MB_OK, "ftest.MB load: %s", mb_status_str(s));

    /* From ftest.DLS:
     *   - Title: FTEST-MODEL1.MC  → DMachine should be 2 (Dorado model 1)
     *   - 54 octal = 44 decimal IM instructions used
     *   - RM: 12 octal = 10 decimal entries used (FIRSTADDR, WAIT, ..., RSCR2)
     *   - ALUFM: 17 octal = 15 entries used (0..#16; ALC at 16)
     */
    EXPECT(mb.dmachine == 2, "expected DMachine=2 (model 1), got %d", mb.dmachine);

    int im_id = mb_find_mem(&mb, "IM");
    EXPECT(im_id >= 0, "no IM in file");
    /* ftest.DLS reports 54 (octal) = 44 (decimal) instructions used. */
    EXPECT(mb.mems[im_id].max_addr == 054,
           "expected %d (decimal) IM entries, got %d", 054, mb.mems[im_id].max_addr);

    int rm_id = mb_find_mem(&mb, "RM");
    EXPECT(rm_id >= 0, "no RM in file");
    /* RM is sparse; max_addr is one past the highest written. RSCR2 is at #11.
     * The "RLC" entry at #12 in the listing is a symbol pointer, not a value. */
    EXPECT(mb.mems[rm_id].max_addr >= 0xa,
           "expected RM max_addr >= 10 (0o12), got %d", mb.mems[rm_id].max_addr);

    int alufm_id = mb_find_mem(&mb, "ALUFM");
    EXPECT(alufm_id >= 0, "no ALUFM in file");
    /* ftest.DLS shows ALUFM addresses 0..16 (octal) = 0..14 (decimal) used,
     * so max_addr (= highest written + 1) should be 15. */
    EXPECT(mb.mems[alufm_id].max_addr == 15,
           "expected ALUFM max_addr=15, got %d", mb.mems[alufm_id].max_addr);

    /* MicroD's .MB stores instructions in IMAGE order (source declaration
     * order), with the REAL placed address only appearing inside encoded
     * branch targets in iw1/iw2. So labels live at image indices, which
     * is the .DLS "Imag" column. From ftest.DLS:
     *   image 0  → BEGIN
     *   image 6  → FETCH1   (0o6 = 6)
     *   image 15 → FETCH2   (0o15 = 13 decimal)
     *   image 24 → SETMCR   (0o24 = 20 decimal)
     *   image 41 → LOADMUNCH (0o41 = 33 decimal)
     *   image 42 → LM1      (0o42 = 34 decimal)
     *   image 51 → LONGWAIT (0o51 = 41 decimal)
     */
    struct { int addr; const char *name; } expected[] = {
        { 0,    "BEGIN" },
        { 006,  "FETCH1" },
        { 015,  "FETCH2" },
        { 024,  "SETMCR" },
        { 041,  "LOADMUNCH" },
        { 042,  "LM1" },
        { 051,  "LONGWAIT" },
    };
    for (size_t i = 0; i < sizeof expected / sizeof expected[0]; i++) {
        const char *sym = mb_lookup_symbol(&mb, im_id, expected[i].addr);
        EXPECT(sym != NULL && !strcmp(sym, expected[i].name),
               "expected symbol %s at IM #%o, got %s",
               expected[i].name, expected[i].addr, sym ? sym : "(none)");
    }

    /* Save these for the printf below before mb_free */
    int im_used = mb.mems[im_id].max_addr;
    int rm_used = mb.mems[rm_id].max_addr;
    int alufm_used = mb.mems[alufm_id].max_addr;
    int dm = mb.dmachine;
    mb_free(&mb);
    printf("PASS  test_ftest_load (DMachine=%d, IM=%d, RM=%d, ALUFM=%d)\n",
           dm, im_used, rm_used, alufm_used);
    return 0;
}

static int test_real_microcode_loads(void)
{
    /* These should load without error. We don't yet know exact entry
     * counts to assert; just that loading completes. */
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
        mb_status s = mb_load(&mb, paths[i]);
        if (s == MB_OK) {
            int im = mb_find_mem(&mb, "IM");
            printf("PASS  load %-58s  IM=%d entries (model %d)\n",
                   paths[i],
                   im >= 0 ? mb.mems[im].max_addr : -1,
                   mb.dmachine);
            passed++;
        } else {
            printf("FAIL  load %-58s  %s\n", paths[i], mb_status_str(s));
        }
        mb_free(&mb);
    }
    return passed == n ? 0 : 1;
}

int main(void)
{
    int rc = 0;
    rc |= test_ftest_load();
    rc |= test_real_microcode_loads();
    if (rc == 0) printf("\nAll tests passed.\n");
    return rc;
}
