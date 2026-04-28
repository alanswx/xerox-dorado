#include "memory.h"

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

/* Test 1: BR load via BrLo + BrHi gives the expected 28-bit VA base. */
static int test_br_load(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* BrLo loads BR[0][16:31] ← A[0:15] = 0x1234.
     * BrHi loads BR[0][4:15]  ← A[4:15] (low 12 bits of A) = 0x0567 → 0x567.
     * Combined BR[0] = 0x05670000 | 0x00001234 = 0x05671234. */
    dorado_br_lo_load(&mem, /*membase=*/0, 0x1234);
    dorado_br_hi_load(&mem, /*membase=*/0, 0x0567);

    uint32_t br = dorado_br_get(&mem, 0);
    EXPECT(br == 0x05671234,
           "BR[0] = 0x%08X, expected 0x05671234", br);

    /* Loading another MemBase should not affect BR[0]. */
    dorado_br_lo_load(&mem, /*membase=*/5, 0xABCD);
    EXPECT(dorado_br_get(&mem, 0) == 0x05671234,
           "BR[0] changed after writing BR[5]");
    EXPECT((dorado_br_get(&mem, 5) & 0xFFFF) == 0xABCD,
           "BR[5] low half = 0x%04X, expected 0xABCD",
           (uint32_t)(dorado_br_get(&mem, 5) & 0xFFFF));

    dorado_memory_free(&mem);
    printf("PASS  test_br_load\n");
    return 0;
}

/* Test 2: Fetch+Store round-trip. */
static int test_fetch_store(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Store 0xCAFE at VA = 0x100. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x100, 0xCAFE);

    /* Fetch from VA = 0x100; Md should be 0xCAFE. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x100, 0);
    EXPECT(mem.md == 0xCAFE, "Md = 0x%04X, expected 0xCAFE", mem.md);

    /* Fetch from a different address; Md updates. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x200, 0xDEAD);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x200, 0);
    EXPECT(mem.md == 0xDEAD, "Md = 0x%04X, expected 0xDEAD", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_fetch_store\n");
    return 0;
}

/* Test 3: Pipe records VAs in reverse order. */
static int test_pipe_records(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_memory_ref(&mem, DM_REF_FETCH, 0x111, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x222, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x333, 0);

    /* head should be 3 now. pipe[2].va = 0x333 (most recent). */
    EXPECT(dorado_pipe_va(&mem, 0) == 0x333,
           "pipe[0] = 0x%X, expected 0x333", dorado_pipe_va(&mem, 0));
    EXPECT(dorado_pipe_va(&mem, 1) == 0x222,
           "pipe[1] = 0x%X, expected 0x222", dorado_pipe_va(&mem, 1));
    EXPECT(dorado_pipe_va(&mem, 2) == 0x111,
           "pipe[2] = 0x%X, expected 0x111", dorado_pipe_va(&mem, 2));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe_records\n");
    return 0;
}

/* Test 4: Pipe wraps after 16 entries. */
static int test_pipe_wraps(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Issue 17 references; the first should be evicted. */
    for (uint32_t i = 0; i < 17; i++) {
        dorado_memory_ref(&mem, DM_REF_FETCH, 0x1000 + i, 0);
    }

    /* Most recent is i=16. The 17 references span pipe slots in some
     * rotation; pipe_va(0)=most recent (0x1010), pipe_va(15)=oldest
     * still in pipe (0x1001), pipe_va(16) wraps and equals pipe_va(0)
     * because the depth is 16. */
    EXPECT(dorado_pipe_va(&mem, 0) == 0x1010,
           "pipe[0] = 0x%X, expected 0x1010", dorado_pipe_va(&mem, 0));
    EXPECT(dorado_pipe_va(&mem, 15) == 0x1001,
           "pipe[15] = 0x%X, expected 0x1001 (oldest)",
           dorado_pipe_va(&mem, 15));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe_wraps\n");
    return 0;
}

/* Test 5: PreFetch and DummyRef don't touch Md or storage but DO push
 * a pipe entry. */
static int test_prefetch_dummyref(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Plant a sentinel in storage at VA=0x500. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x500, 0xABCD);
    /* Set Md to a different value via Fetch. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x500, 0);
    EXPECT(mem.md == 0xABCD, "Md = 0x%04X, expected 0xABCD", mem.md);

    /* PreFetch should not change Md. */
    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0x600, 0);
    EXPECT(mem.md == 0xABCD,
           "PreFetch clobbered Md: 0x%04X", mem.md);
    EXPECT(dorado_pipe_va(&mem, 0) == 0x600,
           "PreFetch didn't push to pipe");

    /* DummyRef should not store anything but should push to pipe. */
    dorado_memory_ref(&mem, DM_REF_DUMMYREF, 0x700, 0xFFFF);
    EXPECT(dorado_pipe_va(&mem, 0) == 0x700, "DummyRef didn't push");
    /* Storage at 0x700 should still be 0. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x700, 0);
    EXPECT(mem.md == 0, "DummyRef wrote storage: Md = 0x%04X", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_prefetch_dummyref\n");
    return 0;
}

/* Test 6: Storage wraps modulo storage_words. */
static int test_storage_wrap(void)
{
    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Store at low address, then read at high address that wraps to
     * the same physical word. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x42, 0xBEEF);
    /* DM_STORAGE_WORDS = 4 MW = 0x400000. So VA 0x400042 wraps to 0x42. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400042, 0);
    EXPECT(mem.md == 0xBEEF,
           "wrap fetch returned Md = 0x%04X, expected 0xBEEF", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_storage_wrap\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_br_load();
    rc |= test_fetch_store();
    rc |= test_pipe_records();
    rc |= test_pipe_wraps();
    rc |= test_prefetch_dummyref();
    rc |= test_storage_wrap();
    if (rc == 0) printf("\nAll memory tests passed.\n");
    return rc;
}
