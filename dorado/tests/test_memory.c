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
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
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
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Mount the touched pages identity-mapped, not WP, not dirty. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    dorado_map_set(&mem, dorado_map_index(0x100), /*rp=*/1, 0, 0);
    dorado_map_set(&mem, dorado_map_index(0x200), /*rp=*/2, 0, 0);

    /* Store 0xCAFE at VA = 0x100. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x100, 0xCAFE, 0);

    /* Fetch from VA = 0x100; Md should be 0xCAFE. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x100, 0, 0);
    EXPECT(mem.md == 0xCAFE, "Md = 0x%04X, expected 0xCAFE", mem.md);

    /* Fetch from a different address; Md updates. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x200, 0xDEAD, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x200, 0, 0);
    EXPECT(mem.md == 0xDEAD, "Md = 0x%04X, expected 0xDEAD", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_fetch_store\n");
    return 0;
}

/* Test 3: Pipe records ASRN refs in distinct slots (HM page 51-52).
 * IOFetch uses ASRN (advances 2..15 after each ref); 3 successive
 * IOFetches land in slots 2, 3, 4 respectively. */
static int test_pipe_records(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    for (uint32_t i = 0; i < 3; i++)
        dorado_map_set(&mem, i, /*rp=*/i, /*wp=*/0, /*dirty=*/0);

    dorado_memory_ref(&mem, DM_REF_IOFETCH, 0 * DM_PAGE_SIZE + 0x11, 0, 0);  /* slot 2 */
    dorado_memory_ref(&mem, DM_REF_IOFETCH, 1 * DM_PAGE_SIZE + 0x22, 0, 0);  /* slot 3 */
    dorado_memory_ref(&mem, DM_REF_IOFETCH, 2 * DM_PAGE_SIZE + 0x33, 0, 0);  /* slot 4 */

    /* SRN-addressed: each VA is in its own slot. */
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x011,
           "pipe[srn=2] = 0x%X, expected 0x011", dorado_pipe_va_at(&mem, 2));
    EXPECT(dorado_pipe_va_at(&mem, 3) == 0x122,
           "pipe[srn=3] = 0x%X, expected 0x122", dorado_pipe_va_at(&mem, 3));
    EXPECT(dorado_pipe_va_at(&mem, 4) == 0x233,
           "pipe[srn=4] = 0x%X, expected 0x233", dorado_pipe_va_at(&mem, 4));
    /* Most-recent (relative-to-head) is the just-written slot. */
    EXPECT(dorado_pipe_va(&mem, 0) == 0x233,
           "most-recent = 0x%X, expected 0x233", dorado_pipe_va(&mem, 0));
    /* ASRN should now be 5 (advanced 2→3→4→5). */
    EXPECT(mem.asrn == 5, "asrn = %d, expected 5", (int)mem.asrn);

    dorado_memory_free(&mem);
    printf("PASS  test_pipe_records\n");
    return 0;
}

static int test_pipe2_records_task_and_ref_type(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_memory_ref_task(&mem, DM_REF_FETCH, 0x10, 0, 0,
                           /*task=*/1, /*subtask=*/2);
    uint16_t p2 = (uint16_t)~dorado_pipe2_at(&mem, 2);
    EXPECT(((p2 >> 14) & 3) == 1,
           "Pipe2 refType = %u, expected storage read",
           (unsigned)((p2 >> 14) & 3));
    EXPECT(((p2 >> 12) & 3) == 2,
           "Pipe2 subtask = %u, expected 2",
           (unsigned)((p2 >> 12) & 3));
    EXPECT(((p2 >> 8) & 017) == 1,
           "Pipe2 task = %u, expected 1",
           (unsigned)((p2 >> 8) & 017));
    EXPECT(((p2 >> 4) & 7) == 7,
           "Pipe2 nFaults = %u, expected no-fault sentinel 7",
           (unsigned)((p2 >> 4) & 7));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe2_records_task_and_ref_type\n");
    return 0;
}

/* Test 4: ASRN ring wraps over slots 2..15 (14 slots) after 14 refs.
 * The 15th ASRN ref reuses slot 2, evicting the first ref. */
static int test_pipe_wraps(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    for (uint32_t i = 0; i < 15; i++)
        dorado_map_set(&mem, i, /*rp=*/i, /*wp=*/0, /*dirty=*/0);

    /* 14 IOFetch misses fill slots 2..15. */
    for (uint32_t i = 0; i < 14; i++) {
        dorado_memory_ref(&mem, DM_REF_IOFETCH,
                          i * DM_PAGE_SIZE + (i & 0xF), 0, 0);
    }
    /* After 14 refs, ASRN should have wrapped from 15 back to 2. */
    EXPECT(mem.asrn == 2, "asrn = %d after 14 refs, expected 2",
           (int)mem.asrn);
    /* Slot 2 has the first ref's VA (0x1000); slot 15 has the 14th
     * ref's VA (0x100D). */
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x0000,
           "pipe[2] = 0x%X, expected 0x0000", dorado_pipe_va_at(&mem, 2));
    EXPECT(dorado_pipe_va_at(&mem, 15) == 0x0D0D,
           "pipe[15] = 0x%X, expected 0x0D0D", dorado_pipe_va_at(&mem, 15));

    /* 15th ref evicts slot 2. */
    dorado_memory_ref(&mem, DM_REF_IOFETCH,
                      14 * DM_PAGE_SIZE + 0x0E, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x0E0E,
           "after wrap: pipe[2] = 0x%X, expected 0x0E0E (evicted 0x0000)",
           dorado_pipe_va_at(&mem, 2));

    /* ProcSRN slots (0, 1) should be untouched by ASRN traffic. */
    EXPECT(dorado_pipe_va_at(&mem, 0) == 0,
           "pipe[0] (ProcSRN) should be untouched: got 0x%X",
           dorado_pipe_va_at(&mem, 0));
    EXPECT(dorado_pipe_va_at(&mem, 1) == 0,
           "pipe[1] (ProcSRN) should be untouched: got 0x%X",
           dorado_pipe_va_at(&mem, 1));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe_wraps\n");
    return 0;
}

/* Test 5: PreFetch and DummyRef don't touch Md or storage but DO push
 * a pipe entry. */
static int test_prefetch_dummyref(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* VAs 0x500/0x600/0x700 fall in distinct 256-word pages. */
    dorado_map_set(&mem, dorado_map_index(0x500), /*rp=*/5, 0, 0);
    dorado_map_set(&mem, dorado_map_index(0x600), /*rp=*/6, 0, 0);
    dorado_map_set(&mem, dorado_map_index(0x700), /*rp=*/7, 0, 0);

    /* Plant a sentinel in storage at VA=0x500. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x500, 0xABCD, 0);
    /* Set Md to a different value via Fetch. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x500, 0, 0);
    EXPECT(mem.md == 0xABCD, "Md = 0x%04X, expected 0xABCD", mem.md);

    /* PreFetch should not change Md. */
    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0x600, 0, 0);
    EXPECT(mem.md == 0xABCD,
           "PreFetch clobbered Md: 0x%04X", mem.md);
    EXPECT(dorado_pipe_va(&mem, 0) == 0x600,
           "PreFetch didn't push to pipe");

    /* DummyRef should not store anything but should push to pipe. */
    dorado_memory_ref(&mem, DM_REF_DUMMYREF, 0x700, 0xFFFF, 0);
    EXPECT(dorado_pipe_va(&mem, 0) == 0x700, "DummyRef didn't push");
    /* Storage at 0x700 should still be 0. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x700, 0, 0);
    EXPECT(mem.md == 0, "DummyRef wrote storage: Md = 0x%04X", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_prefetch_dummyref\n");
    return 0;
}

/* memMisc aMapTest resets a resident map entry, then uses PreFetch to
 * set the entry's Ref bit before reading it back through Pipe4'. */
static int test_prefetch_hit_sets_map_ref(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Fill the cache line, then rewrite the map entry to clear Ref while
     * leaving the cache line resident. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0, 0, 0);
    EXPECT(mem.map[0].ref == 1, "initial fill should set map Ref");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    EXPECT(mem.map[0].ref == 0, "map rewrite should clear map Ref");

    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0, 0, 0);
    EXPECT(mem.map[0].ref == 1,
           "PreFetch hit should set map Ref for diagnostic readback");

    dorado_memory_ref(&mem, DM_REF_RMAP, 0, 0, 0);
    uint16_t pipe4 = dorado_pipe4_at(&mem, mem.proc_srn);
    EXPECT((uint16_t)((pipe4 ^ 0170361u) & 0x8000u) == 0x8000u,
           "RMap Pipe4' previous Ref not set: raw=0x%04X", pipe4);

    /* The map diagnostic also walks mapped pages that can be beyond the
     * installed storage modules. PreFetch does not fault there, but the map
     * reference bit still records that the non-vacant map entry was touched. */
    size_t saved_words = mem.storage_words;
    mem.storage_words = 1;
    dorado_map_set(&mem, 1, /*rp=*/2, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0x100, 0, 0);
    EXPECT(mem.map[1].ref == 1,
           "PreFetch storage-error map lookup should still set map Ref");
    mem.storage_words = saved_words;

    dorado_memory_free(&mem);
    printf("PASS  test_prefetch_hit_sets_map_ref\n");
    return 0;
}

/* Test 6: Real storage does not wrap across module boundaries. */
static int test_storage_bounds(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Map the low VA and the high VA to physical words that would
     * alias if real addresses were masked by the installed size. */
    dorado_map_set(&mem, /*va_page=*/0,      /*rp=*/0,      0, 0);
    dorado_map_set(&mem, dorado_map_index(0x10042), /*rp=*/0x8000, 0, 0);

    /* Store at low address, then try a high address that used to wrap
     * to the same physical word. Flush first so a cache hit cannot
     * hide the physical-storage boundary. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x42, 0xBEEF, 0);
    dorado_memory_ref(&mem, DM_REF_FLUSH, 0x42, 0, 0);
    /* DM_STORAGE_WORDS = 16 MW. Phys 0x800042 is in a different
     * installed module and must not wrap to storage[0x42]. */
    dorado_fault_kind f =
        dorado_memory_ref(&mem, DM_REF_FETCH, 0x10042, 0, 0);
    EXPECT(f == DM_FAULT_NONE,
           "high-module fetch fault = %d, expected no fault", (int)f);
    EXPECT(mem.md != 0xBEEF,
           "high-module fetch aliased low storage: Md = 0x%04X", mem.md);

    size_t saved_words = mem.storage_words;
    mem.storage_words = DM_STORAGE_WORDS - 0x100u;
    dorado_map_set(&mem, dorado_map_index(0x20042), /*rp=*/0xFFFF, 0, 0);
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x20042, 0, 0);
    EXPECT(f == DM_FAULT_STORAGE_ERROR,
           "truncated-storage fetch fault = %d, expected storage error",
           (int)f);
    mem.storage_words = saved_words;

    dorado_memory_free(&mem);
    printf("PASS  test_storage_bounds\n");
    return 0;
}

/* Test 7: Vacant map entry → Page fault on Fetch.
 * HM page 45: All map entries default to Vacant (WP=1, Dirty=1) at
 * power-up. Any reference to a Vacant entry must page-fault. */
static int test_map_vacant_page_fault(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Don't mount any pages. Default = Vacant. */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_FETCH,
                                            0x123, 0, 0);
    EXPECT(f == DM_FAULT_PAGE,
           "expected page fault, got %d", (int)f);
    EXPECT(mem.last_fault == DM_FAULT_PAGE, "last_fault not updated");
    EXPECT(mem.last_fault_va == 0x123,
           "last_fault_va = 0x%X, expected 0x123", mem.last_fault_va);

    /* The Pipe must still record the faulting VA (HM page 46:
     * "fault microcode reads Pipe0/Pipe1 to get the offending VA"). */
    EXPECT(dorado_pipe_va(&mem, 0) == 0x123,
           "pipe[0] = 0x%X, expected 0x123", dorado_pipe_va(&mem, 0));

    /* QW3 / HM Table 17: a Page fault sets the Pipe4 MapTrouble bit. */
    EXPECT(mem.pipe[0].pipe4_errors & PIPE4_ERR_MAP_TROUBLE,
           "page fault should set Pipe4 MapTrouble, errors=0x%X",
           mem.pipe[0].pipe4_errors);

    dorado_memory_free(&mem);
    printf("PASS  test_map_vacant_page_fault\n");
    return 0;
}

/* Test 8: Write-protect fault on Store to WP=1 entry.
 * HM page 46: Store← / IOStore← to a write-protected entry faults. */
static int test_map_write_protect(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Mount page 0 with WP=1 (but not Dirty, else it'd be Vacant). */
    dorado_map_set(&mem, /*va_page=*/0, /*rp=*/0, /*wp=*/1, /*dirty=*/0);

    /* Fetch from a WP page is fine. */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_FETCH,
                                            0x10, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "fetch on WP page should not fault");

    /* QW3: a non-faulting reference must NOT set MapTrouble. */
    EXPECT(!(mem.pipe[0].pipe4_errors & PIPE4_ERR_MAP_TROUBLE),
           "clean fetch must not set MapTrouble, errors=0x%X",
           mem.pipe[0].pipe4_errors);

    /* Store to a WP page faults. */
    f = dorado_memory_ref(&mem, DM_REF_STORE, 0x10, 0xBEEF, 0);
    EXPECT(f == DM_FAULT_WRITE_PROTECT,
           "expected WP fault, got %d", (int)f);

    /* QW3 / HM Table 17: a WP fault sets the Pipe4 MapTrouble bit
     * (same encoding as PageFlt, distinguished by Store'/WP'). */
    EXPECT(mem.pipe[0].pipe4_errors & PIPE4_ERR_MAP_TROUBLE,
           "WP fault should set Pipe4 MapTrouble, errors=0x%X",
           mem.pipe[0].pipe4_errors);

    /* Storage must NOT have been modified. */
    EXPECT(mem.storage[0x10] == 0,
           "WP fault still wrote storage: 0x%04X", mem.storage[0x10]);

    dorado_memory_free(&mem);
    printf("PASS  test_map_write_protect\n");
    return 0;
}

/* Test 9: Map← reference loads RP from B and WP/Dirty from TIOA[0:1].
 * HM page 46: "Map← writes B[0:15]=RP and TIOA[0:1]=WP,Dirty into the
 * map entry indexed by VA. Map← zeroes Ref." */
static int test_map_load(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Mark page 7 with Ref=1 ahead of the Map←, so we can verify it's
     * cleared. dorado_map_set already zeroes Ref, so set it manually. */
    dorado_map_set(&mem, 7, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    /* (No public setter for Ref alone; cycle a fetch to set it.) */
    dorado_memory_ref(&mem, DM_REF_FETCH, 7 * DM_PAGE_SIZE + 0x10, 0, 0);
    EXPECT(dorado_map_get(&mem, 7)->ref == 1, "Ref should be set");

    /* Issue Map← with VA targeting page 7. B = 0xCAFE (RP).
     * TIOA = 0b10000000 (bit 7 set in C-LSB) = 0x80
     *   → manual TIOA[0]=1 (WP), TIOA[1]=0 (not Dirty). */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_MAP,
                                            7 * DM_PAGE_SIZE, 0xCAFE, 0x80);
    EXPECT(f == DM_FAULT_NONE, "Map← should not fault");

    const dorado_map_entry *e = dorado_map_get(&mem, 7);
    EXPECT(e->rp == 0xCAFE, "rp = 0x%04X, expected 0xCAFE", e->rp);
    EXPECT(e->wp == 1, "wp = %d, expected 1", e->wp);
    EXPECT(e->dirty == 0, "dirty = %d, expected 0", e->dirty);
    EXPECT(e->ref == 0, "ref = %d, expected 0 (Map← zeroes Ref)", e->ref);

    /* Now Map← with TIOA = 0xC0 → WP=1, Dirty=1 = Vacant. */
    f = dorado_memory_ref(&mem, DM_REF_MAP, 7 * DM_PAGE_SIZE, 0, 0xC0);
    EXPECT(f == DM_FAULT_NONE, "Map← never faults");
    e = dorado_map_get(&mem, 7);
    EXPECT(e->wp == 1 && e->dirty == 1, "should be Vacant");

    /* And a fetch through page 7 now page-faults. */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 7 * DM_PAGE_SIZE, 0, 0);
    EXPECT(f == DM_FAULT_PAGE, "fetch through Vacant should page-fault");

    dorado_memory_free(&mem);
    printf("PASS  test_map_load\n");
    return 0;
}

/* Test 10: Map translation routes phys = RP*PAGE_SIZE + offset.
 * Two distinct VAs that map to the same RP must alias to the same
 * storage word. (The manual notes this is illegal unless WP=1, but
 * the translator behavior is the same regardless.) */
static int test_map_translation(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Map page 5 → RP 100, page 6 → RP 100 (both write-protected to
     * stay legal). Read-aliasing test. */
    dorado_map_set(&mem, 5, /*rp=*/100, /*wp=*/1, /*dirty=*/0);
    dorado_map_set(&mem, 6, /*rp=*/100, /*wp=*/1, /*dirty=*/0);

    /* Plant a value at the underlying storage location via a
     * non-WP path: temporarily mount page 5 RW, store, Flush←
     * (so the dirty line is written back to storage rather than
     * cached as VA 5*PAGE_SIZE+0x42), then re-mount WP. */
    dorado_map_set(&mem, 5, /*rp=*/100, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_STORE, 5 * DM_PAGE_SIZE + 0x42, 0xFACE, 0);
    dorado_memory_ref(&mem, DM_REF_FLUSH, 5 * DM_PAGE_SIZE + 0x42, 0, 0);
    dorado_map_set(&mem, 5, /*rp=*/100, /*wp=*/1, /*dirty=*/0);

    /* Fetch through page 6 — must miss cache (different VA tag),
     * read from storage, and see the planted value. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 6 * DM_PAGE_SIZE + 0x42, 0, 0);
    EXPECT(mem.md == 0xFACE,
           "alias fetch Md = 0x%04X, expected 0xFACE", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_map_translation\n");
    return 0;
}

/* Test 11: Map← never faults even on a Vacant entry, and PreFetch
 * never faults either (HM page 39). */
static int test_no_fault_refs(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Page 9 is still Vacant. Map← should still succeed. */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_MAP,
                                            9 * DM_PAGE_SIZE, 0x1234, 0);
    EXPECT(f == DM_FAULT_NONE, "Map← into Vacant should not fault");

    /* Page 11 still Vacant. PreFetch should also not fault. */
    f = dorado_memory_ref(&mem, DM_REF_PREFETCH, 11 * DM_PAGE_SIZE, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "PreFetch into Vacant should not fault");

    /* DummyRef also pipe-only, so it should never fault either. */
    f = dorado_memory_ref(&mem, DM_REF_DUMMYREF, 12 * DM_PAGE_SIZE, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "DummyRef should not fault");

    dorado_memory_free(&mem);
    printf("PASS  test_no_fault_refs\n");
    return 0;
}

/* Test 12: FaultInfo' tracks NFaults + SRN of first fault.
 * Field positions per diagnostic memDefs.mc and HM §5 fault handling:
 * b0:3 = ProcSRN, b4:7 = ASRN, b8 (0x0080) = EmulatorFault, b9:11
 * (0x0070) = FaultCnt where the 3-bit field holds (pending faults - 1)
 * — all ones means none — and b12:15 = FirstFaultSRN. */
static int test_fault_info(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* No faults yet → FaultCnt field reads -1 (all ones). */
    EXPECT(dorado_fault_info(&mem) == 0x0270,
           "fresh fault_info should be 0x0270, got 0x%04X",
           dorado_fault_info(&mem));
    dorado_proc_srn_set(&mem, 3);
    EXPECT(dorado_fault_info(&mem) == 0x3270,
           "fault_info should include ProcSRN, got 0x%04X",
           dorado_fault_info(&mem));
    dorado_proc_srn_set(&mem, 0);

    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref_task(&mem, DM_REF_IOFETCH, 0, 0, 0,
                           /*task=*/1, /*subtask=*/0);
    EXPECT(mem.asrn == 3, "live ASRN should advance to 3");
    EXPECT(dorado_fault_info(&mem) == 0x0370,
           "FaultInfo should report live ASRN after IOFetch, got 0x%04X",
           dorado_fault_info(&mem));

    /* All map entries are Vacant → first fetch faults. The pipe slot
     * just before the fetch is the fault SRN. Before this ref,
     * pipe_head=0; after pipe_push, head=1, so fault SRN = 0. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x100, 0, 0);
    EXPECT(mem.fault_count == 1, "NFaults = %d, expected 1",
           (int)mem.fault_count);
    EXPECT(mem.fault_first_srn == 0,
           "first fault SRN = %d, expected 0", (int)mem.fault_first_srn);

    /* Second fault — count goes up, but first_srn stays at 0. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x200, 0, 0);
    EXPECT(mem.fault_count == 2, "NFaults = %d, expected 2",
           (int)mem.fault_count);
    EXPECT(mem.fault_first_srn == 0,
           "first fault SRN should still be 0, got %d",
           (int)mem.fault_first_srn);

    /* Two emulator faults pending: ASRN is live in b4..b7, EmulatorFault
     * (0x0080) is set, FaultCnt field = 2-1 = 1 (0x0010), FirstFaultSRN = 0. */
    EXPECT(dorado_fault_info(&mem) == 0x0390,
           "fault_info = 0x%04X, expected 0x0390",
           dorado_fault_info(&mem));

    /* Clear → back to no-fault. */
    dorado_fault_clear(&mem);
    EXPECT(dorado_fault_info(&mem) == 0x0370, "fault_info should be cleared");
    EXPECT(mem.last_fault == DM_FAULT_NONE, "last_fault should be cleared");

    dorado_memory_free(&mem);
    printf("PASS  test_fault_info\n");
    return 0;
}

/* Test 13: Pipe3' / Map' returns the snapshot of the real page BEFORE the ref,
 * while the flags snapshot is available through the Pipe4/Errors path.
 * HM page 47: "Every storage reference causes mapping and returns
 * old contents of the relevant map entry in the pipe." */
static int test_pipe3_map_entry_snapshot(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Mount page 0 at a non-identity real page. Then fetch — Ref will
     * become 1 in the entry, but the pipe should still see Ref=0 in
     * the pre-ref flags while Map'/Pipe3' sees the old RP. */
    dorado_map_set(&mem, 0, /*rp=*/0x1234, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);

    EXPECT(dorado_pipe_map_rp_at(&mem, 0) == 0x1234,
           "pipe[0].rp_pre = 0x%04X, expected 0x1234",
           dorado_pipe_map_rp_at(&mem, 0));
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 0,
           "pipe[0].flags_pre = 0x%X, expected 0 (WP=0,D=0,R=0)",
           dorado_pipe_map_flags(&mem, 0));

    /* But the entry now has Ref=1. The NEXT ref's snapshot should
     * see that. */
    EXPECT(dorado_map_get(&mem, 0)->ref == 1, "Ref should be set now");
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);
    /* New slot: WP=0, Dirty=0, Ref=1 → 0b100 = 4. */
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 4,
           "pipe[0].flags_pre after 2nd ref = 0x%X, expected 4 (Ref pre-set)",
           dorado_pipe_map_flags(&mem, 0));

    /* WP entry: old RP preserved, WP=1, Dirty=0, Ref=0 → flags=1. */
    dorado_map_set(&mem, dorado_map_index(0x400), /*rp=*/0x4321, /*wp=*/1, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);
    EXPECT(dorado_pipe_map_rp_at(&mem, 0) == 0x4321,
           "WP page rp_pre = 0x%04X, expected 0x4321",
           dorado_pipe_map_rp_at(&mem, 0));
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 1,
           "WP-only flags = 0x%X, expected 1",
           dorado_pipe_map_flags(&mem, 0));

    /* Vacant entry: WP=1, Dirty=1, Ref=0 → flags=3. */
    /* The page containing 0x800 is still default Vacant. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 3,
           "Vacant flags = 0x%X, expected 3 (WP+Dirty)",
           dorado_pipe_map_flags(&mem, 0));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe3_map_entry_snapshot\n");
    return 0;
}

/* Test 14: Cache hit fast path — second Fetch sees cached data
 * even after we directly clobber the underlying storage word. */
static int test_cache_hit(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Plant 0xCAFE in storage at phys 0x10. */
    mem.storage[0x10] = 0xCAFE;
    /* First Fetch: cache miss → fill from storage → Md = 0xCAFE.
     * After this the line containing VA 0x10 is in the cache. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    EXPECT(mem.md == 0xCAFE, "first fetch Md = 0x%04X", mem.md);
    EXPECT(dorado_cache_lookup(&mem, 0x10, NULL),
           "VA 0x10 should be in the cache after first fetch");

    /* Now clobber storage directly. A second Fetch should still see
     * the cached value, not the new storage word. */
    mem.storage[0x10] = 0xBEEF;
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    EXPECT(mem.md == 0xCAFE,
           "cached fetch Md = 0x%04X, expected 0xCAFE (cache should hide storage change)",
           mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_cache_hit\n");
    return 0;
}

/* Test 15: Cache miss fills the whole 16-word munch. */
static int test_cache_miss_fill(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Plant a sentinel pattern in storage words 0..15. */
    for (int i = 0; i < 16; i++) mem.storage[i] = (uint16_t)(0xA000 + i);

    /* Fetch from VA 0x5 (within the munch [0..15]). The fill should
     * pull all 16 words into one cache line. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x5, 0, 0);
    EXPECT(mem.md == 0xA005, "Md = 0x%04X, expected 0xA005", mem.md);

    /* Without further storage activity, fetches at the other words in
     * the munch should hit cache. Clobber storage to prove it. */
    for (int i = 0; i < 16; i++) mem.storage[i] = 0;
    for (int i = 0; i < 16; i++) {
        dorado_memory_ref(&mem, DM_REF_FETCH, (uint32_t)i, 0, 0);
        EXPECT(mem.md == (uint16_t)(0xA000 + i),
               "word %d Md = 0x%04X, expected 0x%04X",
               i, mem.md, (uint16_t)(0xA000 + i));
    }

    dorado_memory_free(&mem);
    printf("PASS  test_cache_miss_fill\n");
    return 0;
}

/* Test 16: Store hit doesn't set Map.Dirty (HM page 45). */
static int test_cache_store_no_map_dirty(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Bring the line into the cache (clean) via Fetch. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);

    /* Store. Cache line goes dirty; Map.Dirty stays clean. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x10, 0xBEEF, 0);
    EXPECT(dorado_map_get(&mem, 0)->dirty == 0,
           "Map.Dirty should be 0 after Store hit (set lazily on evict)");
    EXPECT(dorado_visible_word_at_va(&mem, 0x10) == 0xBEEF,
           "visible word after dirty store = 0x%04X, expected 0xBEEF",
           dorado_visible_word_at_va(&mem, 0x10));
    EXPECT(dorado_storage_at_va(&mem, 0x10) != 0xBEEF,
           "storage should not see dirty cache store before flush");

    /* Flush← writes the dirty line back; NOW Map.Dirty must be set
     * (HM page 47: dirty-victim writeback sets Map.Ref AND Map.Dirty). */
    dorado_memory_ref(&mem, DM_REF_FLUSH, 0x10, 0, 0);
    EXPECT(dorado_map_get(&mem, 0)->dirty == 1,
           "Map.Dirty should be 1 after Flush← of dirty line");

    /* And the writeback should have updated storage. */
    EXPECT(dorado_storage_at_va(&mem, 0x10) == 0xBEEF,
           "storage[0x10] after flush = 0x%04X, expected 0xBEEF",
           dorado_storage_at_va(&mem, 0x10));

    dorado_memory_free(&mem);
    printf("PASS  test_cache_store_no_map_dirty\n");
    return 0;
}

/* Test 17: 4-way LRU eviction — fill 5 distinct munches that all
 * map to the same cache row; the first should be evicted. */
static int test_cache_lru_eviction(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    /* Pick 5 munches all at row 0 by offsetting in 1024-word strides
     * (= 64 rows × 16 words/munch). In 256-word map mode these VAs
     * live in map pages 0, 4, 8, 12, and 16. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Plant identifiable values at the start of each munch. */
    mem.storage[0x000] = 0x1111;   /* munch 0 */
    mem.storage[0x400] = 0x2222;   /* munch (0x400 >> 4) = 64 → row 0 again */
    mem.storage[0x800] = 0x3333;   /* row 0 (0x800 >> 4 = 128, mod 64 = 0) */
    mem.storage[0xC00] = 0x4444;   /* row 0 */
    mem.storage[0x1000] = 0x5555;  /* row 0 */

    /* Same row but different tags. Tag = va >> 10.
     * Row 0 means low 10 bits = 0..15. Tag varies via va >> 10.
     * So VAs that all map to row 0 with distinct tags:
     *   0x000 (tag 0), 0x400 (tag 1), 0x800 (tag 2),
     *   0xC00 (tag 3), 0x1000 (tag 4). */
    dorado_map_set(&mem, dorado_map_index(0x400),  /*rp=*/4,  0, 0);
    dorado_map_set(&mem, dorado_map_index(0x800),  /*rp=*/8,  0, 0);
    dorado_map_set(&mem, dorado_map_index(0xC00),  /*rp=*/12, 0, 0);
    dorado_map_set(&mem, dorado_map_index(0x1000), /*rp=*/16, 0, 0);

    dorado_memory_ref(&mem, DM_REF_FETCH, 0x000, 0, 0);  /* fills way A */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);  /* way B */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);  /* way C */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0xC00, 0, 0);  /* way D */

    /* All four ways are now occupied. The LRU is way A (VA 0x000). */
    EXPECT(dorado_cache_lookup(&mem, 0x000, NULL), "0x000 should be cached");

    /* The 5th fetch evicts the LRU (VA 0x000). */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x1000, 0, 0);
    EXPECT(!dorado_cache_lookup(&mem, 0x000, NULL),
           "VA 0x000 should be evicted after 5th distinct fill");
    EXPECT(dorado_cache_lookup(&mem, 0x1000, NULL),
           "VA 0x1000 should be cached now");

    dorado_memory_free(&mem);
    printf("PASS  test_cache_lru_eviction\n");
    return 0;
}

/* Test 18: Dirty-victim writeback — evicting a dirty line writes
 * data back to storage and sets Map.Ref + Map.Dirty for that page. */
static int test_cache_dirty_victim_writeback(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Same trick as the LRU test: 5 munches in cache row 0. */
    const uint32_t vas[5] = { 0x000, 0x400, 0x800, 0xC00, 0x1000 };
    for (int i = 0; i < 5; i++) {
        uint32_t va = vas[i];
        dorado_map_set(&mem, dorado_map_index(va),
                       /*rp=*/(uint16_t)(va >> 8), /*wp=*/0, /*dirty=*/0);
    }

    /* Store at VA 0x000 — dirties cache row 0 way A. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x000, 0xCAFE, 0);
    EXPECT(dorado_map_get(&mem, 0)->dirty == 0,
           "Map.Dirty should still be 0 after Store hit");

    /* Fill 4 other munches in the same row to evict VA 0x000. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0xC00, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x1000, 0, 0);

    /* VA 0x000 should be evicted. The dirty-victim writeback should
     * have set Map.Ref AND Map.Dirty for page 0 (HM page 47). */
    EXPECT(!dorado_cache_lookup(&mem, 0x000, NULL),
           "VA 0x000 should be evicted");
    EXPECT(dorado_map_get(&mem, 0)->dirty == 1,
           "Map.Dirty should be 1 after dirty-victim writeback");
    EXPECT(dorado_map_get(&mem, 0)->ref == 1,
           "Map.Ref should be 1 after dirty-victim writeback");
    EXPECT(dorado_storage_at_va(&mem, 0x000) == 0xCAFE,
           "storage[0x000] after evict = 0x%04X, expected 0xCAFE",
           dorado_storage_at_va(&mem, 0x000));

    dorado_memory_free(&mem);
    printf("PASS  test_cache_dirty_victim_writeback\n");
    return 0;
}

/*
 * test_dirty_victim_wp_fault (gap C4) — when a dirty cache line's
 * map entry has been changed to WP=1 since the fill, the writeback
 * triggered by eviction must report a fault into FaultInfo. The
 * triggering reference (which forced the eviction) succeeds; the
 * fault is credited to its SRN.
 *
 * This is the "dirty-victim WP fault" path described in HM §5.
 */
static int test_dirty_victim_wp_fault(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    const uint32_t vas[5] = { 0x000, 0x400, 0x800, 0xC00, 0x1000 };
    for (int i = 0; i < 5; i++) {
        uint32_t va = vas[i];
        dorado_map_set(&mem, dorado_map_index(va),
                       /*rp=*/(uint16_t)(va >> 8), /*wp=*/0, /*dirty=*/0);
    }

    /* Dirty page 0. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x000, 0xCAFE, 0);

    /* Now WP-protect page 0 — emulating microcode rewriting the map. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/1, /*dirty=*/0);

    /* Capture FaultInfo state before. */
    uint8_t pre_count = mem.fault_count;

    /* Force eviction. The 4 fetches push way containing 0x000 to LRU,
     * and the 5th miss evicts it — triggering writeback against the
     * now-WP page. The fetches themselves do NOT fault (they're reads,
     * and the pages are RP'd RW). */
    dorado_fault_kind f;
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "0x400 fetch should not fault");
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "0x800 fetch should not fault");
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0xC00, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "0xC00 fetch should not fault");
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x1000, 0, 0);
    EXPECT(f == DM_FAULT_NONE,
           "0x1000 fetch (the evictor) should still succeed; "
           "writeback fault is recorded into FaultInfo, not propagated");

    /* fault_count must have advanced past the writeback failure. */
    EXPECT(mem.fault_count > pre_count,
           "fault_count = %d (was %d) — dirty-victim fault not recorded",
           mem.fault_count, pre_count);

    /* The dirty data was dropped (WP says page is read-only). */
    EXPECT(dorado_storage_at_va(&mem, 0x000) == 0,
           "storage[0x000] = 0x%04X — WP victim should not have committed",
           dorado_storage_at_va(&mem, 0x000));

    dorado_memory_free(&mem);
    printf("PASS  test_dirty_victim_wp_fault (gap C4)\n");
    return 0;
}

/*
 * test_pipe4_error_encoding (gap C2) — verifies dorado_pipe4_at()
 * encodes per-slot error state into the mixed-polarity Pipe4' word.
 * Previous map flags use the 0o170361 XOR transform; notMapTrouble is
 * asserted in the raw polarity consumed by DMesaFaults.mc.
 */
static int test_pipe4_error_encoding(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, 0, 0);

    /* The pipe captures the PREVIOUS map entry's flags (ref/wProtect/dirty,
     * EMemDefs.mc m1pipe4.wpdref = b0,b2,b3). The first Fetch misses and the
     * cache fill sets Map.Ref=1; a second Fetch then snapshots ref=1 into the
     * pipe. The corrected Pipe4' baseline is 0o170361 (sets the wProtect/
     * dirty/ref positions so the high-true map flags read back complemented;
     * see dorado_pipe4_at). */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0, 0, 0);

    uint16_t pipe4_no_err = dorado_pipe4_at(&mem, 0);
    /* Previous-flag high-true after XOR: bit 15 (ref) set. The raw
     * notMapTrouble bit is 0 for "not a map-trouble fault". */
    EXPECT((uint16_t)((pipe4_no_err ^ 0170361u) & ~(1u << 14)) == 0x8000,
           "Pipe4' XOR previous flags = 0x%04X, expected 0x8000 (ref only)",
           (uint16_t)((pipe4_no_err ^ 0170361u) & ~(1u << 14)));
    EXPECT((pipe4_no_err & (1u << 14)) == 0,
           "Pipe4' raw notMapTrouble = 0x%04X, expected bit clear",
           pipe4_no_err);

    /* Inject a MapTrouble. */
    dorado_pipe4_set_error(&mem, 0, PIPE4_ERR_MAP_TROUBLE, 0, 0);
    uint16_t pipe4_mt = dorado_pipe4_at(&mem, 0);
    EXPECT((uint16_t)((pipe4_mt ^ 0170361u) & ~(1u << 14)) == 0x8000,
           "Pipe4' XOR previous flags = 0x%04X, expected 0x8000",
           (uint16_t)((pipe4_mt ^ 0170361u) & ~(1u << 14)));
    EXPECT((pipe4_mt & (1u << 14)) != 0,
           "Pipe4' raw notMapTrouble = 0x%04X, expected bit set",
           pipe4_mt);

    /* Inject MemError + syndrome. */
    dorado_pipe4_set_error(&mem, 0, PIPE4_ERR_MEM_ERROR, 0x55, 0);
    uint16_t pipe4_me = dorado_pipe4_at(&mem, 0);
    /* High-true previous/error fields after XOR, excluding the raw
     * notMapTrouble bit: bit 15 (ref), bit 11 (MemError), syndrome 0x55. */
    EXPECT((uint16_t)((pipe4_me ^ 0170361u) & ~(1u << 14)) == 0x8855,
           "Pipe4' XOR fields = 0x%04X, expected 0x8855",
           (uint16_t)((pipe4_me ^ 0170361u) & ~(1u << 14)));
    EXPECT((pipe4_me & (1u << 14)) != 0,
           "Pipe4' raw notMapTrouble = 0x%04X, expected bit set",
           pipe4_me);

    dorado_memory_free(&mem);
    printf("PASS  test_pipe4_error_encoding (gap C2)\n");
    return 0;
}

/*
 * test_dirty_victim_pipe4_map_trouble (gap C2 + C4 integration) —
 * dirty-victim WP fault should set MapTrouble in the triggering ref's
 * Pipe4 slot.
 */
static int test_dirty_victim_pipe4_map_trouble(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    const uint32_t vas[5] = { 0x000, 0x400, 0x800, 0xC00, 0x1000 };
    for (int i = 0; i < 5; i++) {
        dorado_map_set(&mem, dorado_map_index(vas[i]),
                       (uint16_t)(vas[i] >> 8), 0, 0);
    }

    /* Dirty page 0; mark WP=1; force eviction by 4 fetches. */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x000, 0xCAFE, 0);
    dorado_map_set(&mem, 0, 0, /*wp=*/1, 0);

    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0xC00, 0, 0);
    /* This 5th miss evicts page 0 → writeback hits WP fault → records
     * MapTrouble for *this* ref's slot (proc_srn = 0). */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x1000, 0, 0);

    uint16_t pipe4 = dorado_pipe4_at(&mem, mem.proc_srn);
    EXPECT(pipe4 & (1u << 14),
           "Pipe4' raw notMapTrouble = 0x%04X — expected MapTrouble bit set",
           pipe4);

    dorado_memory_free(&mem);
    printf("PASS  test_dirty_victim_pipe4_map_trouble (gap C2)\n");
    return 0;
}

/*
 * test_dbuf_captures_store_b (gap B3 sub-item) — the data buffer
 * captures the most-recent Store's B value. Used by Midas via
 * `B<-DBuf` (HM Table 11c FA=1 FB=7 FC=5).
 */
static int test_dbuf_captures_store_b(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, 0, 0, 0);

    dorado_memory_ref(&mem, DM_REF_STORE, 0x10, 0xCAFE, 0);
    EXPECT(mem.dbuf == 0xCAFE, "dbuf=0x%04X, expected 0xCAFE", mem.dbuf);

    dorado_memory_ref(&mem, DM_REF_STORE, 0x20, 0xBEEF, 0);
    EXPECT(mem.dbuf == 0xBEEF, "dbuf=0x%04X, expected 0xBEEF", mem.dbuf);

    /* A Fetch should NOT change dbuf. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    EXPECT(mem.dbuf == 0xBEEF, "dbuf changed by Fetch: 0x%04X", mem.dbuf);

    dorado_memory_free(&mem);
    printf("PASS  test_dbuf_captures_store_b (gap B3)\n");
    return 0;
}

/* Test 19: Flush← on a clean hit — invalidates without storage
 * activity; Map.Dirty stays 0. */
static int test_cache_flush_clean(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Fetch to fill (clean line). */
    mem.storage[0x10] = 0xABCD;
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    EXPECT(dorado_cache_lookup(&mem, 0x10, NULL), "should be cached");

    /* Flush a clean hit — line is invalidated; Map.Dirty unchanged. */
    dorado_memory_ref(&mem, DM_REF_FLUSH, 0x10, 0, 0);
    EXPECT(!dorado_cache_lookup(&mem, 0x10, NULL),
           "VA 0x10 should be invalidated after Flush");
    EXPECT(dorado_map_get(&mem, 0)->dirty == 0,
           "Map.Dirty should still be 0 (clean flush, no writeback)");

    dorado_memory_free(&mem);
    printf("PASS  test_cache_flush_clean\n");
    return 0;
}

/* Test 20: IOStore unconditionally invalidates cache + sets
 * Map.Ref AND Map.Dirty (HM page 40, page 45). */
static int test_iostore_cache_invalidate(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Bring a line into cache via Fetch + Store (so it's dirty). */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    dorado_memory_ref(&mem, DM_REF_STORE, 0x10, 0xBEEF, 0);
    EXPECT(dorado_cache_lookup(&mem, 0x10, NULL), "should be cached");

    /* IOStore: cache line should be discarded (no writeback), and
     * Map.Ref + Map.Dirty must be set. */
    dorado_memory_ref(&mem, DM_REF_IOSTORE, 0x10, 0, 0);
    EXPECT(!dorado_cache_lookup(&mem, 0x10, NULL),
           "IOStore should invalidate the cache line");
    EXPECT(dorado_map_get(&mem, 0)->ref == 1, "Map.Ref should be set");
    EXPECT(dorado_map_get(&mem, 0)->dirty == 1, "Map.Dirty should be set");

    dorado_memory_free(&mem);
    printf("PASS  test_iostore_cache_invalidate\n");
    return 0;
}

typedef struct {
    int called;
    dorado_ref_kind kind;
    int task;
    int subtask;
    uint32_t va;
    uint16_t munch[16];
} test_fastio_capture;

static void capture_fastio_cb(struct dorado_memory *mem,
                              dorado_ref_kind kind,
                              int task, int subtask, uint32_t va,
                              uint16_t munch[16], void *ctx)
{
    (void)mem;
    test_fastio_capture *cap = ctx;
    cap->called++;
    cap->kind = kind;
    cap->task = task;
    cap->subtask = subtask;
    cap->va = va;
    memcpy(cap->munch, munch, sizeof cap->munch);
}

/* Test 21: IOFetch sends the cached munch, including dirty words, when
 * the line is present in cache. The callback receives the aligned munch
 * VA rather than the unaligned reference VA. */
static int test_iofetch_uses_cached_dirty_munch(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    test_fastio_capture cap; memset(&cap, 0, sizeof cap);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    for (int i = 0; i < 16; i++)
        mem.storage[0x10 + i] = (uint16_t)(0xA000u + (uint16_t)i);

    /* Fill the cache line for VA 0x10..0x1f, then dirty one cached word. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x15, 0, 0);
    dorado_memory_ref(&mem, DM_REF_STORE, 0x15, 0xCAFE, 0);

    /* Make backing storage stale. IOFetch must not use these words while
     * the cache still contains the line. */
    for (int i = 0; i < 16; i++)
        mem.storage[0x10 + i] = (uint16_t)(0xB000u + (uint16_t)i);

    mem.fast_io_cb = capture_fastio_cb;
    mem.fast_io_ctx = &cap;
    dorado_memory_ref_task(&mem, DM_REF_IOFETCH, 0x15, 0, 0,
                           /*task=*/013, /*subtask=*/2);

    EXPECT(cap.called == 1, "fast IO callback called %d times", cap.called);
    EXPECT(cap.kind == DM_REF_IOFETCH, "callback kind=%d", cap.kind);
    EXPECT(cap.task == 013 && cap.subtask == 2,
           "callback task/subtask=%o.%d", cap.task, cap.subtask);
    EXPECT(cap.va == 0x10,
           "callback VA=0x%X, expected aligned munch VA 0x10", cap.va);
    EXPECT(cap.munch[0] == 0xA000,
           "munch[0]=0x%04X, expected cached 0xA000", cap.munch[0]);
    EXPECT(cap.munch[5] == 0xCAFE,
           "munch[5]=0x%04X, expected dirty cached 0xCAFE", cap.munch[5]);
    EXPECT(cap.munch[15] == 0xA00F,
           "munch[15]=0x%04X, expected cached 0xA00F", cap.munch[15]);

    dorado_memory_free(&mem);
    printf("PASS  test_iofetch_uses_cached_dirty_munch\n");
    return 0;
}

/* Test 22: ProcSRN ref overwrites a single pipe slot.
 * HM page 51: emulator (task 0) refs use ProcSRN (default 0), so
 * three successive Fetches all land in slot 0 — only the most
 * recent VA remains. ProcSRN←B can move ProcSRN to a different slot. */
static int test_proc_srn_overwrite(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* ProcSRN starts at 0. Three Fetches all go to slot 0. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x30, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 0) == 0x30,
           "pipe[0] = 0x%X, expected 0x30 (last Fetch)",
           dorado_pipe_va_at(&mem, 0));

    /* Switch ProcSRN to slot 1; next Fetch goes there. Slot 0 keeps
     * its old value. */
    dorado_proc_srn_set(&mem, 1);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x40, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 0) == 0x30,
           "pipe[0] should retain 0x30, got 0x%X",
           dorado_pipe_va_at(&mem, 0));
    EXPECT(dorado_pipe_va_at(&mem, 1) == 0x40,
           "pipe[1] = 0x%X, expected 0x40", dorado_pipe_va_at(&mem, 1));

    /* ASRN slots remain untouched by these ProcSRN refs. */
    for (int s = 2; s < DM_PIPE_DEPTH; s++) {
        EXPECT(dorado_pipe_va_at(&mem, s) == 0,
               "pipe[%d] should be 0 (untouched), got 0x%X",
               s, dorado_pipe_va_at(&mem, s));
    }

    dorado_memory_free(&mem);
    printf("PASS  test_proc_srn_overwrite\n");
    return 0;
}

/* Test 22: PreFetch-with-miss uses ASRN, but PreFetch-hit uses ProcSRN.
 * HM page 52: "all task 0 and task 15 references except PreFetch←
 * with miss ... use the SRN contained in ProcSRN exclusively". */
static int test_prefetch_srn_split(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* PreFetch on a fresh cache: miss → uses ASRN (slot 2). */
    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0x10, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x10,
           "PreFetch miss should land in ASRN slot 2: got 0x%X",
           dorado_pipe_va_at(&mem, 2));
    EXPECT(mem.asrn == 3, "asrn = %d after PreFetch miss, expected 3",
           (int)mem.asrn);

    /* Second PreFetch on same VA: hit → uses ProcSRN (slot 0). */
    dorado_memory_ref(&mem, DM_REF_PREFETCH, 0x10, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 0) == 0x10,
           "PreFetch hit should land in ProcSRN slot 0: got 0x%X",
           dorado_pipe_va_at(&mem, 0));
    EXPECT(mem.asrn == 3, "asrn should still be 3 (no advance on hit)");

    dorado_memory_free(&mem);
    printf("PASS  test_prefetch_srn_split\n");
    return 0;
}

static int test_config_word_reports_storage(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    uint16_t cfg = dorado_memory_config_word(&mem);
    EXPECT(((cfg >> 2) & 0x3) == 3,
           "Config chip size = 0x%X, expected 3", (cfg >> 2) & 0x3);
    EXPECT((cfg & 0200u) != 0,
           "Config M0 bit missing: cfg=0o%06o", cfg);
    EXPECT(((cfg >> 8) & 0xF) == 2,
           "Config ASRN = %u, expected initial ASRN 2",
           (unsigned)((cfg >> 8) & 0xF));

    mem.asrn = 017;
    cfg = dorado_memory_config_word(&mem);
    EXPECT(((cfg >> 8) & 0xF) == 017,
           "Config ASRN = %u, expected ASRN 017",
           (unsigned)((cfg >> 8) & 0xF));

    mem.storage_chip_type = 1;
    mem.storage_words = (DM_STORAGE_MODULE_WORDS >> 4) * 2u;
    cfg = dorado_memory_config_word(&mem);
    EXPECT(((cfg >> 2) & 0x3) == 1,
           "Config chip size = 0x%X, expected 1", (cfg >> 2) & 0x3);
    EXPECT((cfg & 0300u) == 0300u,
           "Config module bits = 0o%03o, expected M0+M1",
           cfg & 0360u);

    dorado_memory_free(&mem);
    printf("PASS  test_config_word_reports_storage\n");
    return 0;
}

static int test_mcr_load_and_controls(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_mcr_load(&mem, 0xFFE0, 0x0007);
    EXPECT(dorado_mcr_get(&mem) == 0xFFE7,
           "MCR = 0x%04X, expected 0xFFE7", dorado_mcr_get(&mem));
    EXPECT(dorado_mcr_disbr(&mem), "DisBR should be set");
    EXPECT(dorado_mcr_noref(&mem), "NoRef should be set");
    EXPECT(dorado_mcr_fdmiss(&mem), "FDMiss should be set");
    EXPECT(dorado_mcr_nowake(&mem), "NoWake should be set");

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_load_and_controls\n");
    return 0;
}

static int test_mcr_disbr_blocks_br_writes(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_br_lo_load(&mem, 0, 0x1234);
    dorado_mcr_load(&mem, 0x0080, 0);  /* manual Mcr[8] = DisCF only */
    dorado_br_lo_load(&mem, 0, 0x5678);
    EXPECT((dorado_br_get(&mem, 0) & 0xFFFF) == 0x5678,
           "DisCF alone should not suppress BrLo writes, BR=0x%08X",
           dorado_br_get(&mem, 0));

    dorado_mcr_load(&mem, 0x0100, 0);  /* manual Mcr[7] = DisBR */
    dorado_br_lo_load(&mem, 0, 0xABCD);
    EXPECT((dorado_br_get(&mem, 0) & 0xFFFF) == 0x5678,
           "DisBR should suppress BrLo writes, BR=0x%08X",
           dorado_br_get(&mem, 0));

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_disbr_blocks_br_writes\n");
    return 0;
}

static int test_mcr_noref_suppresses_storage_access(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_mcr_load(&mem, 0x0020, 0);  /* manual Mcr[10] = NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_FETCH, 0, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "NoRef fetch should not fault");
    EXPECT(mem.fault_count == 0, "NoRef fetch should not record fault");

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_noref_suppresses_storage_access\n");
    return 0;
}

static int test_mcr_dvavic_reads_cache_address_without_storage(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    uint32_t cached_va = 0x2340;
    dorado_map_set(&mem, dorado_map_index(cached_va),
                   /*rp=*/(uint16_t)(cached_va >> 8), /*wp=*/0, /*dirty=*/0);
    mem.storage[cached_va] = 0xCAFE;

    /* Fill cache way 2 for the target row. */
    dorado_mcr_load(&mem, 0x3000, 0);  /* UseMcrV + McrV=2 */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_FETCH,
                                            cached_va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "priming fetch should not fault");
    EXPECT(mem.md == 0xCAFE, "priming fetch Md=0x%04X", mem.md);

    /* dVA<-Victim prevents map/storage activity and returns the cache
     * address memory contents for row(cached_va), way 2 through Pipe0/1. */
    dorado_mcr_load(&mem, 0xB000, 0);  /* dVA<-Victim + UseMcrV + McrV=2 */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, cached_va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim fetch should not fault");
    EXPECT(dorado_pipe_va_at(&mem, mem.proc_srn) == cached_va,
           "pipe VA=0x%X, expected cached VA 0x%X",
           dorado_pipe_va_at(&mem, mem.proc_srn), cached_va);

    /* Even an unmapped VA should not start the map/storage automata. */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x9000, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim must suppress map faults");
    EXPECT(mem.fault_count == 0, "dVA<-Victim should not record faults");

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_dvavic_reads_cache_address_without_storage\n");
    return 0;
}

static int test_mcr_noref_store_writes_cache_address(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* InitialSubrs ClearCacheFlags uses NoRef+UseMcrV stores as a
     * cache-address write: CacheA[VA, selected column] <- VA. */
    dorado_mcr_load(&mem, 0x7030, 0);  /* FDMiss + UseMcrV + Victim=2 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE,
                                            0x12340, 0xA5A5, 0);
    EXPECT(f == DM_FAULT_NONE, "NoRef Store should not fault");
    EXPECT(mem.fault_count == 0, "NoRef Store should not record fault");

    dorado_mcr_load(&mem, 0xB000, 0);  /* dVA<-Victim + UseMcrV + Victim=2 */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x12340, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim fetch should not fault");
    EXPECT(dorado_pipe_va(&mem, 0) == 0x12340,
           "CacheA readback VA=0x%05X, expected 0x12340",
           (unsigned)dorado_pipe_va(&mem, 0));

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_noref_store_writes_cache_address\n");
    return 0;
}

static int test_store_hit_does_not_translate_vacant_map(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* memRWd's presetCache writes the cache address RAM under NoRef and then
     * clears CFlags before testing the cache data RAM. A following Store-hit
     * must not translate through the still-vacant Map. */
    dorado_mcr_load(&mem, 0x6030, 0);  /* FDMiss + UseMcrV + Victim=0 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE,
                                            0x0000, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "CacheA preset should not fault");
    dorado_cflags_load(&mem, 0xFFFFu); /* CFlags <- 0 after A' hardware path */

    dorado_mcr_load(&mem, 0, 0);
    f = dorado_memory_ref(&mem, DM_REF_STORE, 0x0000, 0xCAFE, 0);
    EXPECT(f == DM_FAULT_NONE, "Store-hit should not fault on vacant Map");
    EXPECT(mem.fault_count == 0, "Store-hit should not record a fault");
    EXPECT(dorado_visible_word_at_va(&mem, 0x0000) == 0xCAFE,
           "Store-hit data not visible in cache");
    EXPECT(dorado_storage_at_va(&mem, 0x0000) == 0xFFFF,
           "Store-hit should not write through to vacant backing storage");

    dorado_memory_free(&mem);
    printf("PASS  test_store_hit_does_not_translate_vacant_map\n");
    return 0;
}

static int test_vacant_cache_address_is_not_cache_hit(void)
{
    enum {
        CFLAGS_A_VACANT = 0x0040u,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* InitialSubrs ClearCacheFlags first writes CacheA[VA, ColX],
     * then writes CFlags[VA, ColX] <- Vacant. The address entry must
     * still be readable by dVA<-Victim, but it must not be a cache-data
     * hit until a real fill clears Vacant. */
    dorado_mcr_load(&mem, 0x7030, 0);  /* FDMiss + UseMcrV + Victim=2 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE,
                                            0x12340, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "NoRef Store should not fault");
    dorado_cflags_load(&mem, (uint16_t)~CFLAGS_A_VACANT);

    EXPECT(!dorado_cache_lookup(&mem, 0x12340, NULL),
           "Vacant cache-address entry must not count as a cache hit");

    dorado_mcr_load(&mem, 0xB000, 0);  /* dVA<-Victim + UseMcrV + Victim=2 */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x12340, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim fetch should not fault");
    EXPECT(dorado_pipe_va(&mem, 0) == 0x12340,
           "Vacant CacheA readback VA=0x%05X, expected 0x12340",
           (unsigned)dorado_pipe_va(&mem, 0));

    dorado_memory_free(&mem);
    printf("PASS  test_vacant_cache_address_is_not_cache_hit\n");
    return 0;
}

static int test_mcr_noref_store_uses_comparator_hit(void)
{
    enum {
        PIPE5_COLUMN_MASK  = 0x0300u,
        PIPE5_COLUMN_SHIFT = 8,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Plant a CacheA entry in forced column 0. */
    dorado_mcr_load(&mem, 0x2030, 0);  /* FDMiss + UseMcrV + Victim=0 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE,
                                            0x12340, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "CacheA setup store should not fault");

    /* With NoRef but no UseMcrV/FDMiss, the reference should still use
     * the cache comparators and report the matching column in Pipe5. */
    dorado_mcr_load(&mem, 0x00E0, 1);  /* DisCF + DisHold + NoRef + NoWake */
    f = dorado_memory_ref(&mem, DM_REF_STORE, 0x12340, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "NoRef comparator store should not fault");

    uint16_t p5 = dorado_pipe5_at(&mem, mem.proc_srn);
    EXPECT(((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT) == 0,
           "Pipe5 column = %u, expected comparator hit in column 0",
           (unsigned)((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT));

    dorado_memory_free(&mem);
    printf("PASS  test_mcr_noref_store_uses_comparator_hit\n");
    return 0;
}

static int test_mapbuf_busy_pipe5_timing(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_proc_srn_set(&mem, 0);
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_MAP,
                                            0x00001234u, 0x0042, 0);
    EXPECT(f == DM_FAULT_NONE, "Map ref should not fault");
    EXPECT((dorado_pipe5_at(&mem, 0) & 0x8000u) != 0,
           "MapBufBusy should appear in Pipe5 sign bit");

    for (int i = 0; i < 8; i++) dorado_memory_tick(&mem);
    EXPECT((dorado_pipe5_at(&mem, 0) & 0x8000u) != 0,
           "MapBufBusy cleared too early");
    dorado_memory_tick(&mem);
    EXPECT((dorado_pipe5_at(&mem, 0) & 0x8000u) == 0,
           "MapBufBusy should clear after 9 cycles");

    dorado_memory_free(&mem);
    printf("PASS  test_mapbuf_busy_pipe5_timing\n");
    return 0;
}

static int test_cflags_load_visible_in_pipe5(void)
{
    enum {
        CFLAGS_A_DIRTY = 0x0080u,
        CFLAGS_A_WP    = 0x0020u,
        PIPE5_DIRTY    = 0x0080u,
        PIPE5_WP       = 0x0020u,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);
    dorado_cflags_load(&mem, (uint16_t)~(CFLAGS_A_DIRTY | CFLAGS_A_WP));
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);

    uint16_t p5 = dorado_pipe5_at(&mem, mem.proc_srn);
    EXPECT((p5 & PIPE5_DIRTY) != 0, "Pipe5 missing Dirty after CFlags");
    EXPECT((p5 & PIPE5_WP) != 0, "Pipe5 missing WP after CFlags");

    dorado_memory_free(&mem);
    printf("PASS  test_cflags_load_visible_in_pipe5\n");
    return 0;
}

static int test_pipe5_reports_selected_column(void)
{
    enum {
        PIPE5_COLUMN_MASK  = 0x0300u,
        PIPE5_COLUMN_SHIFT = 8,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    dorado_map_set(&mem, dorado_map_index(0x120), /*rp=*/1, 0, 0);

    /* UseMcrV selects Victim=2. Pipe5 reports the selected cache column
     * in memDefs.mc's pipe5.colBit0/1 field (b6,b7). */
    dorado_mcr_load(&mem, 0x3200, 0);
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_FETCH, 0x120, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "fetch should not fault");

    uint16_t p5 = dorado_pipe5_at(&mem, mem.proc_srn);
    EXPECT(((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT) == 2,
           "Pipe5 column = %u, expected 2",
           (unsigned)((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe5_reports_selected_column\n");
    return 0;
}

static int test_dummyref_latches_pipe5_from_comparator_hit(void)
{
    enum {
        PIPE5_COLUMN_MASK  = 0x0300u,
        PIPE5_COLUMN_SHIFT = 8,
        PIPE5_DIRTY        = 0x0080u,
        PIPE5_WP           = 0x0020u,
        CFLAGS_A_DIRTY     = 0x0080u,
        CFLAGS_A_WP        = 0x0020u,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    uint32_t va_hit = 0x12340u;
    uint32_t va_other = va_hit ^ 0x0400u; /* same row, different tag */

    /* Plant the target CacheA entry in column 1 and give it distinctive
     * flags. Then select column 0 with a different tag so stale Pipe5 state
     * would be observable. */
    dorado_mcr_load(&mem, 0x6830, 0);  /* FDMiss + UseMcrV + Victim=1 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE,
                                            va_hit, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "CacheA setup store should not fault");
    dorado_cflags_load(&mem, (uint16_t)~(CFLAGS_A_DIRTY | CFLAGS_A_WP));

    dorado_mcr_load(&mem, 0x6030, 0);  /* FDMiss + UseMcrV + Victim=0 + NoRef */
    f = dorado_memory_ref(&mem, DM_REF_STORE, va_other, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "stale Pipe5 setup store should not fault");
    dorado_cflags_load(&mem, 0xFFFFu);

    /* memSubrsC.readCflags uses this shape: dPipeVa + FDMiss + UseMcrV +
     * DisBR + NoRefHold, then DummyRef and Pipe5. The comparator hit must
     * win over the MCR victim and over stale Pipe5 state. */
    dorado_mcr_load(&mem, 0xE160, 0);
    f = dorado_memory_ref(&mem, DM_REF_DUMMYREF, va_hit, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "DummyRef should not fault");

    uint16_t p5 = dorado_pipe5_at(&mem, mem.proc_srn);
    EXPECT(((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT) == 1,
           "Pipe5 column = %u, expected DummyRef comparator hit in column 1",
           (unsigned)((p5 & PIPE5_COLUMN_MASK) >> PIPE5_COLUMN_SHIFT));
    EXPECT((p5 & PIPE5_DIRTY) != 0, "Pipe5 missing Dirty from hit column");
    EXPECT((p5 & PIPE5_WP) != 0, "Pipe5 missing WP from hit column");

    dorado_memory_free(&mem);
    printf("PASS  test_dummyref_latches_pipe5_from_comparator_hit\n");
    return 0;
}

static int test_dummyref_forces_cflags_column_without_dvavic(void)
{
    enum {
        PIPE5_DIRTY    = 0x0080u,
        CFLAGS_A_DIRTY = 0x0080u,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    uint32_t va = 0x12340u;

    /* Make the same CacheA tag valid in columns 0 and 1. A plain
     * FDMiss+UseMcrV DummyRef must still select the MCR victim for a
     * subsequent CFlags write; otherwise a comparator-first path would keep
     * rewriting column 0 and memA's xVacateCacheRow/putCFmem would drift. */
    dorado_mcr_load(&mem, 0x6030, 0);  /* FDMiss + UseMcrV + Victim=0 + NoRef */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_STORE, va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "CacheA col0 setup store should not fault");
    dorado_cflags_load(&mem, 0xFFFFu);

    dorado_mcr_load(&mem, 0x6830, 0);  /* FDMiss + UseMcrV + Victim=1 + NoRef */
    f = dorado_memory_ref(&mem, DM_REF_STORE, va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "CacheA col1 setup store should not fault");
    dorado_cflags_load(&mem, 0xFFFFu);

    f = dorado_memory_ref(&mem, DM_REF_DUMMYREF, va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "DummyRef should not fault");
    dorado_cflags_load(&mem, (uint16_t)~CFLAGS_A_DIRTY);

    dorado_mcr_load(&mem, 0xA800, 0);  /* dPipeVa + UseMcrV + Victim=1 */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim col1 fetch should not fault");
    EXPECT((dorado_pipe5_at(&mem, mem.proc_srn) & PIPE5_DIRTY) != 0,
           "CFlags write did not land in MCR-selected column 1");

    dorado_mcr_load(&mem, 0xA000, 0);  /* dPipeVa + UseMcrV + Victim=0 */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, va, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "dVA<-Victim col0 fetch should not fault");
    EXPECT((dorado_pipe5_at(&mem, mem.proc_srn) & PIPE5_DIRTY) == 0,
           "CFlags write also dirtied comparator/stale column 0");

    dorado_memory_free(&mem);
    printf("PASS  test_dummyref_forces_cflags_column_without_dvavic\n");
    return 0;
}

static int test_discf_blocks_cflags_and_pipe5_flags(void)
{
    enum {
        CFLAGS_A_DIRTY = 0x0080u,
        PIPE5_DIRTY    = 0x0080u,
    };

    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);
    dorado_mcr_load(&mem, 0x0080, 0);  /* manual Mcr[8] = DisCF */
    dorado_cflags_load(&mem, (uint16_t)~CFLAGS_A_DIRTY);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x20, 0, 0);
    EXPECT((dorado_pipe5_at(&mem, mem.proc_srn) & PIPE5_DIRTY) == 0,
           "DisCF should hide/prevent Pipe5 cache flags");

    dorado_memory_free(&mem);
    printf("PASS  test_discf_blocks_cflags_and_pipe5_flags\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_br_load();
    rc |= test_fetch_store();
    rc |= test_pipe_records();
    rc |= test_pipe2_records_task_and_ref_type();
    rc |= test_pipe_wraps();
    rc |= test_prefetch_dummyref();
    rc |= test_prefetch_hit_sets_map_ref();
    rc |= test_storage_bounds();
    rc |= test_map_vacant_page_fault();
    rc |= test_map_write_protect();
    rc |= test_map_load();
    rc |= test_map_translation();
    rc |= test_no_fault_refs();
    rc |= test_fault_info();
    rc |= test_pipe3_map_entry_snapshot();
    rc |= test_cache_hit();
    rc |= test_cache_miss_fill();
    rc |= test_cache_store_no_map_dirty();
    rc |= test_cache_lru_eviction();
    rc |= test_cache_dirty_victim_writeback();
    rc |= test_dirty_victim_wp_fault();
    rc |= test_pipe4_error_encoding();
    rc |= test_dirty_victim_pipe4_map_trouble();
    rc |= test_dbuf_captures_store_b();
    rc |= test_cache_flush_clean();
    rc |= test_iostore_cache_invalidate();
    rc |= test_iofetch_uses_cached_dirty_munch();
    rc |= test_proc_srn_overwrite();
    rc |= test_prefetch_srn_split();
    rc |= test_config_word_reports_storage();
    rc |= test_mcr_load_and_controls();
    rc |= test_mcr_disbr_blocks_br_writes();
    rc |= test_mcr_noref_suppresses_storage_access();
    rc |= test_mcr_dvavic_reads_cache_address_without_storage();
    rc |= test_mcr_noref_store_writes_cache_address();
    rc |= test_store_hit_does_not_translate_vacant_map();
    rc |= test_vacant_cache_address_is_not_cache_hit();
    rc |= test_mcr_noref_store_uses_comparator_hit();
    rc |= test_mapbuf_busy_pipe5_timing();
    rc |= test_cflags_load_visible_in_pipe5();
    rc |= test_pipe5_reports_selected_column();
    rc |= test_dummyref_latches_pipe5_from_comparator_hit();
    rc |= test_dummyref_forces_cflags_column_without_dvavic();
    rc |= test_discf_blocks_cflags_and_pipe5_flags();
    if (rc == 0) printf("\nAll memory tests passed.\n");
    return rc;
}
