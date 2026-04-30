#ifndef DORADO_MEMORY_H
#define DORADO_MEMORY_H

#include <stdint.h>
#include <stddef.h>

/*
 * Dorado memory subsystem — Phase A.2 stub.
 *
 * HM §5 ("Memory Section"). The full memory system is cache + Map +
 * Pipe + BR + storage; this stub provides just enough for the
 * processor to issue Fetch / Store references and for Md to deliver
 * data on the next cycle. No cache, no Map, no Hold modeling: refs
 * are atomic and Md is available immediately.
 *
 * Address model:
 *   VA = BR[MemBase] + Mar     (28-bit virtual; HM page 36)
 *   Phys ≡ VA[4:31] truncated to `storage_words` (no map yet)
 *
 * Reference kinds (HM Table 8a + page 37):
 *   PreFetch     — load munch into cache; no Md update.
 *   Fetch        — single-word read; Md ← mem[VA] available next cycle.
 *   IFetch       — like Fetch but BR[24:31] is replaced by Id from IFU.
 *   LongFetch    — 28-bit VA = (B[4:31] || Mar) + BR[MemBase].
 *   Store        — DBuf ← B in same instruction; mem[VA] ← DBuf.
 *   IOFetch      — fast-output munch; io tasks only.
 *   IOStore      — fast-input munch; io tasks only.
 *   Map          — load map entry from B; emulator/fault tasks only.
 *   Flush        — remove munch from cache (write-back if dirty).
 *   DummyRef     — pipe-record only; no actual access.
 *
 * The Pipe is a 16-entry ring of recent storage references. Each
 * entry records VA and the kind. Microcode reads recent entries via
 * `B←Pipe0..5` FF functions (HM Table 11c).
 */

typedef enum {
    DM_REF_NONE = 0,
    DM_REF_PREFETCH,
    DM_REF_MAP,             /* emulator/fault */
    DM_REF_IOFETCH,         /* io task */
    DM_REF_LONGFETCH,
    DM_REF_STORE,
    DM_REF_DUMMYREF,
    DM_REF_FLUSH,           /* emulator/fault */
    DM_REF_IOSTORE,         /* io task */
    DM_REF_IFETCH,
    DM_REF_FETCH,
} dorado_ref_kind;

#define DM_BR_COUNT      32
#define DM_PIPE_DEPTH    16
#define DM_STORAGE_WORDS (4 * 1024 * 1024)   /* 4 MW = 8 MB */

/*
 * Cache (HM §5.11). 4096-word cache organized as 64 rows × 4 ways
 * (columns) × 16 words/line. The cache holds VAs (not real
 * addresses), so the Map is consulted only on miss.
 *
 * Address split (10 LSB bits of VA):
 *   bits 0..3  = word offset within a 16-word munch
 *   bits 4..9  = row index (one of 64)
 * The remaining VA bits form the per-way tag. (HM says "compared
 * with VA[4:19]" = 16 bits, but in our 1024-page config the actual
 * tag is wider — we store the full upper VA bits.)
 *
 * Replacement: LRU per row. ways[lru[0]] is the most-recently-used
 * way; ways[lru[3]] is the next victim.
 *
 * Flags per line:
 *   valid — line currently holds a fetched munch
 *   dirty — line has been written; must be flushed back on eviction
 */
#define DM_CACHE_ROWS      64
#define DM_CACHE_WAYS      4
#define DM_CACHE_LINE_W    16
#define DM_CACHE_LINE_MASK (DM_CACHE_LINE_W - 1)
#define DM_CACHE_ROW_MASK  (DM_CACHE_ROWS - 1)

typedef struct {
    uint32_t tag;     /* VA bits above the row+offset */
    uint8_t  valid;
    uint8_t  dirty;
    uint16_t data[DM_CACHE_LINE_W];
} dorado_cache_line;

typedef struct {
    dorado_cache_line ways[DM_CACHE_WAYS];
    /* LRU permutation: lru[0] = MRU way index, lru[DM_CACHE_WAYS-1] = LRU. */
    uint8_t lru[DM_CACHE_WAYS];
} dorado_cache_row;

/*
 * Map (HM §5 "The Map", page 44 ff.). We pick the 16K-map ×
 * 1024-word page configuration: VA[8:21] indexes the map, VA[22:31]
 * is the page offset. Total VM = 2^24 words. (HM Table 16 lists
 * other configurations — page sizes 256/1024/4096 with map IC sizes
 * 16K/64K/256K. Our choice matches what Mesa typically expects.)
 *
 * Each map entry is a 16-bit real page number (RP) plus three flags:
 *   - WP    write-protected
 *   - Dirty modified
 *   - Ref   referenced (set on any storage ref except Map←; cleared
 *           by Map←)
 *   - Vacant (encoded as WP=1, Dirty=1) — page fault on any access.
 *
 * Real address = (RP << 10) | VA[22:31]   (1024-word pages).
 */
#define DM_MAP_ENTRIES   (16 * 1024)         /* 16K map entries */
#define DM_PAGE_SIZE     1024                /* 1024-word pages */

typedef struct {
    uint16_t rp;       /* 16-bit real page number */
    uint8_t  wp;       /* 1 = write-protected */
    uint8_t  dirty;    /* 1 = modified */
    uint8_t  ref;      /* 1 = referenced */
} dorado_map_entry;

/*
 * Fault kinds (HM page 46 "Faults"). Real Dorado wakes the fault
 * task (task 15) on these; without tasking, we record the kind and
 * VA, optionally halt.
 */
typedef enum {
    DM_FAULT_NONE = 0,
    DM_FAULT_PAGE,           /* reference to vacant map entry */
    DM_FAULT_WRITE_PROTECT,  /* Store←/IOStore←/dirty-victim with WP=1 */
    DM_FAULT_MAP_TROUBLE,    /* parity error on map read (we don't model this) */
} dorado_fault_kind;

typedef struct dorado_memory {
    /* 32 base registers, each 28-bit. Stored as uint32_t; the high 4
     * bits are unused. HM §5: BR addresses 1 of 32 28-bit registers,
     * subtask[0:1] OR'd with MemBase[3:4] for io tasks. */
    uint32_t br[DM_BR_COUNT];

    /* 16-bit memory data register. HM page 39: "Md remains valid
     * until and during the next fetch by the task." */
    uint16_t md;

    /* Pipe — addressed by 4-bit SRN (Storage Reference Number).
     * Each ref is assigned an SRN at issue time; the entry is
     * written to pipe[srn]. Microcode reads pipe entries via
     * `B←Pipei` after loading `ProcSRN←B` to address the slot.
     *
     * SRN allocation (HM page 51-52):
     *   ProcSRN  — task 0/15 (emulator + fault) refs except
     *              PreFetch-with-miss. Conventionally 0 or 1.
     *   ASRN     — I/O task refs and emulator PreFetch-with-miss.
     *              Ring buffer over slots 2..15. Advanced after a
     *              reference that "starts the map" (i.e., goes to
     *              storage); held otherwise.
     *
     * `map_flags_pre` snapshots the map entry's WP/Dirty/Ref bits
     * as they were *before* the reference updated them. HM page 47:
     * "Every storage reference causes mapping and returns old
     * contents of the relevant map entry in the pipe."
     *   bit 0 = WP, bit 1 = Dirty, bit 2 = Ref. */
    struct {
        uint32_t        va;
        dorado_ref_kind kind;
        uint8_t         map_flags_pre;
    } pipe[DM_PIPE_DEPTH];
    int     pipe_head;       /* index just past the just-written slot, wrapped */
    uint8_t proc_srn;        /* 4-bit; emulator + fault tasks. Default 0. */
    uint8_t asrn;            /* 4-bit; I/O ring. 2..15. Default 2. */

    /* Main storage — flat array, 16-bit words. We don't model ECC
     * bits yet; HM §5.7 will add them in Phase B. */
    uint16_t *storage;
    size_t    storage_words;

    /* Memory Control Register (MCR). Stored in normal C bit order for
     * the 16-bit value that microcode loads with LoadMcr[A,B]. */
    uint16_t mcr;

    /* Cache — interposed between processor refs and storage for
     * Fetch/Store/PreFetch/Flush. IOFetch/IOStore bypass the cache
     * (see HM page 39 IOFetch/IOStore semantics). */
    dorado_cache_row cache[DM_CACHE_ROWS];

    /*
     * Map. All entries default to Vacant (WP=1, Dirty=1) at init; the
     * microcode populates entries via Map← references during startup.
     */
    dorado_map_entry map[DM_MAP_ENTRIES];

    /* Most recent fault state — set by dorado_memory_ref when a
     * reference faults. The microcode would normally see this via
     * the fault task's wake; in our single-task model we expose it
     * for diagnostics + halt-on-fault tests. */
    dorado_fault_kind last_fault;
    uint32_t          last_fault_va;

    /* Mar register — the most recent reference's VA. Used by
     * `ReadMap` (FA=0 FB=3 FC=1) to address the Map for read-back
     * (HM page 41: ReadMap delivers map[Mar] to B in the next
     * instruction). Updated on every memory_ref. */
    uint32_t          mar;

    /* FaultInfo register state — readable by microcode via
     * B←FaultInfo' / B←Pipe2' (HM Table 11c FA=1 FB=6 FC=0).
     * Both reads return the same 16-bit register, inverted.
     *   - fault_count       (NFaults) — number of unacknowledged faults
     *   - fault_first_srn   (SRN of first uncleared fault)
     *   - fault_emulator    (was first fault from task 0/15? — always 1
     *                        in single-task mode)
     * Cleared by dorado_fault_clear(). */
    uint8_t  fault_count;
    uint8_t  fault_first_srn;
    uint8_t  fault_emulator;

    /*
     * Fast-I/O callback (HM §8). When the microengine issues
     * IOFetch← (memory → device, Fin bus) or IOStore← (device →
     * memory, Fout bus), memory_ref calls this callback with the
     * task ID, subtask, VA, kind, and a 16-word munch buffer.
     *
     *   IOFetch:  memory has just READ 16 words from storage[VA..VA+15]
     *             into the munch buffer. Callback delivers them to the
     *             receiving device (e.g., display FIFO for DWT).
     *   IOStore:  memory is about to WRITE 16 words to storage[VA..
     *             VA+15]. Callback is responsible for FILLING the
     *             munch buffer from the source device (e.g., disk
     *             controller's read FIFO for DSK).
     *
     * If `fast_io_cb` is NULL, IOFetch/IOStore behave as data-less
     * pipe-and-cache-only operations (the legacy stub behavior).
     */
    void (*fast_io_cb)(struct dorado_memory *mem, dorado_ref_kind kind,
                       int task, int subtask, uint32_t va,
                       uint16_t munch[16], void *ctx);
    void *fast_io_ctx;
} dorado_memory;

/* Initialize: allocate storage array, zero registers + pipe.
 * Returns 0 on success, -1 on allocation failure. */
int  dorado_memory_init(dorado_memory *mem);
void dorado_memory_free(dorado_memory *mem);

/* Reference dispatch. Called from the microengine when ASEL is a
 * memory reference (ASEL = 0..3 with FF[0:1] decoding the kind).
 *
 * `va` is the full 28-bit virtual address (already computed as
 * BR[MemBase] + Mar by the caller).
 * `b` is the B-bus value (data for stores or Map← writes).
 * `tioa` is the TIOA register, used by Map← to supply WP/Dirty
 *        bits (TIOA[0]=WP, TIOA[1]=Dirty per HM page 46).
 *
 * Returns DM_FAULT_NONE on success, or a fault kind. The caller
 * (cpu.c) decides whether to halt or just record the fault.
 */
dorado_fault_kind dorado_memory_ref(dorado_memory *mem, dorado_ref_kind kind,
                                    uint32_t va, uint16_t b, uint16_t tioa);

/* Variant that supplies (task, subtask) — used for fast-IO references
 * to dispatch to the right device. Other refs ignore task/subtask. */
dorado_fault_kind dorado_memory_ref_task(dorado_memory *mem,
                                         dorado_ref_kind kind,
                                         uint32_t va, uint16_t b,
                                         uint16_t tioa,
                                         int task, int subtask);

/* Map manipulation helpers, mostly for tests. */
void dorado_map_set(dorado_memory *mem, uint32_t va_page,
                    uint16_t rp, int wp, int dirty);
const dorado_map_entry *dorado_map_get(const dorado_memory *mem,
                                       uint32_t va_page);

/* Helpers for FF functions that load/read BR. */
void     dorado_br_lo_load(dorado_memory *mem, int membase, uint16_t a);
void     dorado_br_hi_load(dorado_memory *mem, int membase, uint16_t a);
uint32_t dorado_br_get(const dorado_memory *mem, int membase);

/* Pipe access — `n` is the slot relative to head: 0 = most recent,
 * 1 = previous, etc. Returns 0 if pipe hasn't been touched yet.
 * Useful for tests that want temporal-order recall; microcode would
 * use the SRN-based accessors below. */
uint32_t dorado_pipe_va(const dorado_memory *mem, int n);

/* SRN-based pipe accessors. `srn` is a 4-bit slot index (0..15) —
 * the value microcode would have loaded into ProcSRN before reading
 * `B←Pipei`. */
uint32_t dorado_pipe_va_at(const dorado_memory *mem, int srn);
uint8_t  dorado_pipe_map_flags_at(const dorado_memory *mem, int srn);

/* Set ProcSRN (the slot index used for non-PreFetch-miss task-0/15
 * references). FF function `ProcSRN←B[12:15]` (HM Table 11c FA=1
 * FB=2 FC=7) calls this with B[12:15] = low 4 bits of B. */
void     dorado_proc_srn_set(dorado_memory *mem, uint8_t srn);

/* Snapshot of map flags for the pipe entry at slot `n` (relative to
 * head). Bit layout (high-true): bit 0=WP, bit 1=Dirty, bit 2=Ref.
 * On the B bus this would be read inverted as `B←Pipe3'`. */
uint8_t  dorado_pipe_map_flags(const dorado_memory *mem, int n);

/* Compute the high-true 16-bit FaultInfo register value. The B bus
 * receives `~fault_info` as `B←FaultInfo'`. Layout (MSB-first):
 *   B[0:7]   reserved (zero internally)
 *   B[8:11]  SRN of first fault
 *   B[12:15] NFaults count
 * Microcode acknowledges by some mechanism (TBD); our model exposes
 * `dorado_fault_clear()` for tests. */
uint16_t dorado_fault_info(const dorado_memory *mem);
void     dorado_fault_clear(dorado_memory *mem);

/* High-true internal value for the hardware B←Config' source. The CPU
 * puts the complement of this on the B bus. */
uint16_t dorado_memory_config_word(const dorado_memory *mem);

/* Load/read MCR. LoadMcr[A,B] takes Mcr[0:10] from A/MarMux and
 * Mcr[13:15] from B/BMux; manual bit numbers are MSB-first. */
void     dorado_mcr_load(dorado_memory *mem, uint16_t a, uint16_t b);
uint16_t dorado_mcr_get(const dorado_memory *mem);
int      dorado_mcr_disbr(const dorado_memory *mem);
int      dorado_mcr_noref(const dorado_memory *mem);
int      dorado_mcr_fdmiss(const dorado_memory *mem);
int      dorado_mcr_nowake(const dorado_memory *mem);

/* Cache inspection helpers, for tests and diagnostics.
 *
 * dorado_cache_lookup() returns 1 if the munch containing `va` is
 * cached, with *out_way (if non-NULL) set to the way index.
 *
 * dorado_storage_at_va() bypasses the cache: translates VA via the
 * Map and returns the storage word. Used by tests that need to probe
 * "what's actually in storage" without going through the cache. The
 * map must be mounted; if the entry is Vacant the function returns
 * 0xFFFF (no fault is signaled — this is a probe, not a reference).
 */
int      dorado_cache_lookup(const dorado_memory *mem, uint32_t va,
                             int *out_way);
uint16_t dorado_storage_at_va(const dorado_memory *mem, uint32_t va);

#endif
