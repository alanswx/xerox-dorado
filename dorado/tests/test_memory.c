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

    /* Mount map page 0 → real page 0 (identity), not WP, not dirty. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

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

    dorado_memory_ref(&mem, DM_REF_IOFETCH, 0x111, 0, 0);  /* slot 2 */
    dorado_memory_ref(&mem, DM_REF_IOFETCH, 0x222, 0, 0);  /* slot 3 */
    dorado_memory_ref(&mem, DM_REF_IOFETCH, 0x333, 0, 0);  /* slot 4 */

    /* SRN-addressed: each VA is in its own slot. */
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x111,
           "pipe[srn=2] = 0x%X, expected 0x111", dorado_pipe_va_at(&mem, 2));
    EXPECT(dorado_pipe_va_at(&mem, 3) == 0x222,
           "pipe[srn=3] = 0x%X, expected 0x222", dorado_pipe_va_at(&mem, 3));
    EXPECT(dorado_pipe_va_at(&mem, 4) == 0x333,
           "pipe[srn=4] = 0x%X, expected 0x333", dorado_pipe_va_at(&mem, 4));
    /* Most-recent (relative-to-head) is the just-written slot. */
    EXPECT(dorado_pipe_va(&mem, 0) == 0x333,
           "most-recent = 0x%X, expected 0x333", dorado_pipe_va(&mem, 0));
    /* ASRN should now be 5 (advanced 2→3→4→5). */
    EXPECT(mem.asrn == 5, "asrn = %d, expected 5", (int)mem.asrn);

    dorado_memory_free(&mem);
    printf("PASS  test_pipe_records\n");
    return 0;
}

/* Test 4: ASRN ring wraps over slots 2..15 (14 slots) after 14 refs.
 * The 15th ASRN ref reuses slot 2, evicting the first ref. */
static int test_pipe_wraps(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* 14 IOFetches fill slots 2..15. */
    for (uint32_t i = 0; i < 14; i++) {
        dorado_memory_ref(&mem, DM_REF_IOFETCH, 0x1000 + i, 0, 0);
    }
    /* After 14 refs, ASRN should have wrapped from 15 back to 2. */
    EXPECT(mem.asrn == 2, "asrn = %d after 14 refs, expected 2",
           (int)mem.asrn);
    /* Slot 2 has the first ref's VA (0x1000); slot 15 has the 14th
     * ref's VA (0x100D). */
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x1000,
           "pipe[2] = 0x%X, expected 0x1000", dorado_pipe_va_at(&mem, 2));
    EXPECT(dorado_pipe_va_at(&mem, 15) == 0x100D,
           "pipe[15] = 0x%X, expected 0x100D", dorado_pipe_va_at(&mem, 15));

    /* 15th ref evicts slot 2. */
    dorado_memory_ref(&mem, DM_REF_IOFETCH, 0x100E, 0, 0);
    EXPECT(dorado_pipe_va_at(&mem, 2) == 0x100E,
           "after wrap: pipe[2] = 0x%X, expected 0x100E (evicted 0x1000)",
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

    /* VAs 0x500/0x600/0x700 all fall in map page 1 (>>10 = 1). */
    dorado_map_set(&mem, 1, /*rp=*/1, /*wp=*/0, /*dirty=*/0);

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

/* Test 6: Storage wraps modulo storage_words. */
static int test_storage_wrap(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Map page 0 (covers VA 0x42) and map page 0x1000 (covers
     * VA 0x400042). Both identity-mapped: page i → RP i. */
    dorado_map_set(&mem, /*va_page=*/0,      /*rp=*/0,      0, 0);
    dorado_map_set(&mem, /*va_page=*/0x1000, /*rp=*/0x1000, 0, 0);

    /* Store at low address, then read at high address that wraps to
     * the same physical word. With cache modeling, the Store sits in
     * the cache for VA 0x42; we Flush← it back to storage before
     * reading via the aliased VA (HM page 47: distinct VAs → same RP
     * is illegal unless both WP, so the canonical pattern is
     * Flush←-then-read). */
    dorado_memory_ref(&mem, DM_REF_STORE, 0x42, 0xBEEF, 0);
    dorado_memory_ref(&mem, DM_REF_FLUSH, 0x42, 0, 0);
    /* DM_STORAGE_WORDS = 4 MW = 0x400000. So phys 0x400042 wraps to 0x42. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400042, 0, 0);
    EXPECT(mem.md == 0xBEEF,
           "wrap fetch returned Md = 0x%04X, expected 0xBEEF", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_storage_wrap\n");
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

    /* Store to a WP page faults. */
    f = dorado_memory_ref(&mem, DM_REF_STORE, 0x10, 0xBEEF, 0);
    EXPECT(f == DM_FAULT_WRITE_PROTECT,
           "expected WP fault, got %d", (int)f);

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
    dorado_memory_ref(&mem, DM_REF_FETCH, 7 * 1024 + 0x10, 0, 0);
    EXPECT(dorado_map_get(&mem, 7)->ref == 1, "Ref should be set");

    /* Issue Map← with VA targeting page 7. B = 0xCAFE (RP).
     * TIOA = 0b10000000 (bit 7 set in C-LSB) = 0x80
     *   → manual TIOA[0]=1 (WP), TIOA[1]=0 (not Dirty). */
    dorado_fault_kind f = dorado_memory_ref(&mem, DM_REF_MAP,
                                            7 * 1024, 0xCAFE, 0x80);
    EXPECT(f == DM_FAULT_NONE, "Map← should not fault");

    const dorado_map_entry *e = dorado_map_get(&mem, 7);
    EXPECT(e->rp == 0xCAFE, "rp = 0x%04X, expected 0xCAFE", e->rp);
    EXPECT(e->wp == 1, "wp = %d, expected 1", e->wp);
    EXPECT(e->dirty == 0, "dirty = %d, expected 0", e->dirty);
    EXPECT(e->ref == 0, "ref = %d, expected 0 (Map← zeroes Ref)", e->ref);

    /* Now Map← with TIOA = 0xC0 → WP=1, Dirty=1 = Vacant. */
    f = dorado_memory_ref(&mem, DM_REF_MAP, 7 * 1024, 0, 0xC0);
    EXPECT(f == DM_FAULT_NONE, "Map← never faults");
    e = dorado_map_get(&mem, 7);
    EXPECT(e->wp == 1 && e->dirty == 1, "should be Vacant");

    /* And a fetch through page 7 now page-faults. */
    f = dorado_memory_ref(&mem, DM_REF_FETCH, 7 * 1024, 0, 0);
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
     * cached as VA 5*1024+0x42), then re-mount WP. */
    dorado_map_set(&mem, 5, /*rp=*/100, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_STORE, 5 * 1024 + 0x42, 0xFACE, 0);
    dorado_memory_ref(&mem, DM_REF_FLUSH, 5 * 1024 + 0x42, 0, 0);
    dorado_map_set(&mem, 5, /*rp=*/100, /*wp=*/1, /*dirty=*/0);

    /* Fetch through page 6 — must miss cache (different VA tag),
     * read from storage, and see the planted value. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 6 * 1024 + 0x42, 0, 0);
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
                                            9 * 1024, 0x1234, 0);
    EXPECT(f == DM_FAULT_NONE, "Map← into Vacant should not fault");

    /* Page 11 still Vacant. PreFetch should also not fault. */
    f = dorado_memory_ref(&mem, DM_REF_PREFETCH, 11 * 1024, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "PreFetch into Vacant should not fault");

    /* DummyRef also pipe-only, so it should never fault either. */
    f = dorado_memory_ref(&mem, DM_REF_DUMMYREF, 12 * 1024, 0, 0);
    EXPECT(f == DM_FAULT_NONE, "DummyRef should not fault");

    dorado_memory_free(&mem);
    printf("PASS  test_no_fault_refs\n");
    return 0;
}

/* Test 12: FaultInfo' tracks NFaults + SRN of first fault.
 * HM Table 11c FA=1 FB=6 FC=0 / page 51: FaultInfo' (and the
 * equivalent Pipe2') are the inverted FaultInfo register —
 * 0xFFFF when no faults, otherwise encodes count and SRN. */
static int test_fault_info(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* No faults yet → FaultInfo high-true value is 0. */
    EXPECT(dorado_fault_info(&mem) == 0,
           "fresh fault_info should be 0, got 0x%04X",
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

    /* High-true FaultInfo: B[8:11]=SRN=0, B[12:15]=NFaults=2,
     * B[7]=EmulatorFault=1. In C-LSB: bit 8 = 0x100, SRN<<4 = 0,
     * count = 2 → 0x102. */
    EXPECT(dorado_fault_info(&mem) == 0x102,
           "fault_info = 0x%04X, expected 0x102",
           dorado_fault_info(&mem));

    /* Clear → back to no-fault. */
    dorado_fault_clear(&mem);
    EXPECT(dorado_fault_info(&mem) == 0, "fault_info should be cleared");
    EXPECT(mem.last_fault == DM_FAULT_NONE, "last_fault should be cleared");

    dorado_memory_free(&mem);
    printf("PASS  test_fault_info\n");
    return 0;
}

/* Test 13: Pipe3' returns the snapshot of map flags BEFORE the ref.
 * HM page 47: "Every storage reference causes mapping and returns
 * old contents of the relevant map entry in the pipe." */
static int test_pipe3_map_flags(void)
{
    static dorado_memory mem; memset(&mem, 0, sizeof mem);
    EXPECT(dorado_memory_init(&mem) == 0, "init");

    /* Mount page 0: WP=0, Dirty=0, Ref=0. Then fetch — Ref will
     * become 1 in the entry, but the pipe should still see Ref=0
     * (pre-ref snapshot). */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x10, 0, 0);

    /* Snapshot in pipe slot 0: WP=0, Dirty=0, Ref=0 → flags=0. */
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

    /* WP entry: WP=1, Dirty=0, Ref=0 → flags=1. */
    dorado_map_set(&mem, 1, /*rp=*/1, /*wp=*/1, /*dirty=*/0);
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x400, 0, 0);
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 1,
           "WP-only flags = 0x%X, expected 1",
           dorado_pipe_map_flags(&mem, 0));

    /* Vacant entry: WP=1, Dirty=1, Ref=0 → flags=3. */
    /* Page 2 is still default Vacant. */
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x800, 0, 0);
    EXPECT(dorado_pipe_map_flags(&mem, 0) == 3,
           "Vacant flags = 0x%X, expected 3 (WP+Dirty)",
           dorado_pipe_map_flags(&mem, 0));

    dorado_memory_free(&mem);
    printf("PASS  test_pipe3_map_flags\n");
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
    /* All five VAs use page 0 → row index = (va >> 4) & 0x3F. We pick
     * 5 munches all at row 0 by offsetting in 1024-word strides
     * (= 64 rows × 16 words/munch). */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    /* Plant identifiable values at the start of each munch. */
    mem.storage[0x000] = 0x1111;   /* munch 0 */
    mem.storage[0x400] = 0x2222;   /* munch (0x400 >> 4) = 64 → row 0 again */
    mem.storage[0x800] = 0x3333;   /* row 0 (0x800 >> 4 = 128, mod 64 = 0) */
    mem.storage[0xC00] = 0x4444;   /* row 0 */
    mem.storage[0x1000] = 0x5555;  /* row 0 — but this would need page 1 mounted */

    /* Use VAs all within page 0 for simplicity. Different cache rows
     * means we need same row but different tags. Tag = va >> 10.
     * Row 0 means low 10 bits = 0..15. Tag varies via va >> 10.
     * So VAs that all map to row 0 with distinct tags:
     *   0x000 (tag 0), 0x400 (tag 1), 0x800 (tag 2),
     *   0xC00 (tag 3), 0x1000 (tag 4). */
    /* Need a separate map page for VAs 0x400, 0x800, 0xC00, 0x1000 if
     * they fall in different map pages. Map page = va >> 10.
     *   0x000 → page 0; 0x400 → page 1; 0x800 → page 2;
     *   0xC00 → page 3; 0x1000 → page 4.
     * Mount each pointing at its own RP so the storage layout works. */
    dorado_map_set(&mem, 1, /*rp=*/1, 0, 0);  /* RP1 starts at storage 0x400 */
    dorado_map_set(&mem, 2, /*rp=*/2, 0, 0);
    dorado_map_set(&mem, 3, /*rp=*/3, 0, 0);
    dorado_map_set(&mem, 4, /*rp=*/4, 0, 0);

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
    for (int p = 0; p < 5; p++) {
        dorado_map_set(&mem, /*va_page=*/(uint32_t)p, /*rp=*/(uint16_t)p,
                       /*wp=*/0, /*dirty=*/0);
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

/* Test 21: ProcSRN ref overwrites a single pipe slot.
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

int main(void)
{
    int rc = 0;
    rc |= test_br_load();
    rc |= test_fetch_store();
    rc |= test_pipe_records();
    rc |= test_pipe_wraps();
    rc |= test_prefetch_dummyref();
    rc |= test_storage_wrap();
    rc |= test_map_vacant_page_fault();
    rc |= test_map_write_protect();
    rc |= test_map_load();
    rc |= test_map_translation();
    rc |= test_no_fault_refs();
    rc |= test_fault_info();
    rc |= test_pipe3_map_flags();
    rc |= test_cache_hit();
    rc |= test_cache_miss_fill();
    rc |= test_cache_store_no_map_dirty();
    rc |= test_cache_lru_eviction();
    rc |= test_cache_dirty_victim_writeback();
    rc |= test_cache_flush_clean();
    rc |= test_iostore_cache_invalidate();
    rc |= test_proc_srn_overwrite();
    rc |= test_prefetch_srn_split();
    if (rc == 0) printf("\nAll memory tests passed.\n");
    return rc;
}
