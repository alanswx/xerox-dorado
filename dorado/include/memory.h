#ifndef DORADO_MEMORY_H
#define DORADO_MEMORY_H

#include <stdint.h>
#include <stddef.h>

/*
 * Dorado memory subsystem.
 *
 * HM §5 ("Memory Section"). The modeled system includes BRs, Map,
 * cache address/data state, Pipe, main storage, and enough MCR control
 * for boot bring-up. Timing is still simplified: refs are atomic and
 * Md is available immediately; full Hold/deferred-reference behavior
 * is not modeled yet.
 *
 * Address model:
 *   VA = BR[MemBase] + Mar     (28-bit virtual; HM page 36)
 *   Phys = Map[page(VA)].RP || page_offset(VA), checked against installed
 *          storage modules
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
    /* RMap← (HM page 46-47): read a map entry. Encoded like Map← (ASEL=0,
     * FF[0:1]=1) but carries the ReadMap function (FB=3, FC=1), which
     * makes it a *read* — it snapshots the entry's previous contents into
     * the pipe (Map'/Errors') and never modifies the entry or faults.
     * Map← without that function is the write. Kept as a distinct kind so
     * the memory model does not corrupt the scanned entries (e.g.
     * PilotBoot.FindEndMappedVM's `RMap_ RTemp0` map scan). */
    DM_REF_RMAP,
} dorado_ref_kind;

#define DM_BR_COUNT      32
#define DM_PIPE_DEPTH    16
#define DM_STORAGE_MODULE_WORDS (4 * 1024 * 1024)  /* 4 MW = 8 MB */
#define DM_STORAGE_MODULES_DEFAULT 4
#define DM_STORAGE_WORDS (DM_STORAGE_MODULE_WORDS * DM_STORAGE_MODULES_DEFAULT)
#define DM_STORAGE_CHIP_TYPE_DEFAULT 3

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
 *   wp/vacant/being_loaded — cache-address-section flags visible via
 *           Pipe5 and writable via CFlags←A during diagnostics/init
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
    uint8_t  wp;
    uint8_t  vacant;
    uint8_t  being_loaded;
    uint16_t data[DM_CACHE_LINE_W];
} dorado_cache_line;

typedef struct {
    dorado_cache_line ways[DM_CACHE_WAYS];
    /* LRU permutation: lru[0] = MRU way index, lru[DM_CACHE_WAYS-1] = LRU. */
    uint8_t lru[DM_CACHE_WAYS];
} dorado_cache_row;

/*
 * Map (HM §5 "The Map", page 44 ff.). We pick the 64K-map ×
 * 256-word page configuration: VA[8:23] indexes the map (16 page
 * bits), VA[24:31] is the page offset. Total VM = 2^24 words = 16 MW.
 * (HM Table 16 lists other configurations — page sizes 256/1024/4096
 * with map IC sizes 16K/64K/256K.) The Cedar/Mesa world relocates the
 * Pilot germ to virtual page 0o174010 (= 64008), which exceeds a 16K
 * map and would alias onto a low VA; the 64K map gives it a distinct
 * index. InitMem.mc GetMemConfig is told MapIs64K (VirtualBanks=400C
 * = 256 banks = 65536 entries) so the cold map-init loop enumerates
 * exactly DM_MAP_ENTRIES entries and still terminates.
 *
 * Each map entry is a 16-bit real page number (RP) plus three flags:
 *   - WP    write-protected
 *   - Dirty modified
 *   - Ref   referenced (set on any storage ref except Map←; cleared
 *           by Map←)
 *   - Vacant (encoded as WP=1, Dirty=1) — page fault on any access.
 *
 * Real address = (RP << 8) | VA[24:31]   (256-word pages).
 */
#define DM_MAP_ENTRIES   (64 * 1024)         /* 64K map entries (VirtualBanks=400C) */
#define DM_PAGE_SIZE     256                 /* 256-word pages */
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
    DM_FAULT_STORAGE_ERROR,  /* storage data/module error (MemError) */
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
     * `map_rp_pre` and `map_flags_pre` snapshot the map entry as it
     * was *before* the reference updated it. HM page 47:
     * "Every storage reference causes mapping and returns old
     * contents of the relevant map entry in the pipe." Mesa map
     * primitives read old RP via `Map'` / `Pipe3'` and old flags via
     * the error/Pipe4 path.
     *   map_flags_pre bit 0 = WP, bit 1 = Dirty, bit 2 = Ref. */
    struct {
        uint32_t        va;
        dorado_ref_kind kind;
        uint8_t         task;
        uint8_t         subtask;
        uint8_t         ref_type;
        uint16_t        map_rp_pre;
        uint8_t         map_flags_pre;
        uint8_t         mapbuf_busy;
        uint16_t        cache_flags;
        /* Per-slot Pipe4 syndrome bits (gap C2). Bit positions match
         * the manual / EMemDefs.mc — high-true internal storage:
         *   bit 0 = MapTrouble       (cache fault during fill)
         *   bit 1 = MemError         (single-bit ECC, corrected)
         *   bit 2 = EcFault          (uncorrectable)
         * `pipe4_syndrome` is the 8-bit ECC syndrome (only meaningful
         * when MemError or EcFault is set). `pipe4_quadword` (2 bits)
         * identifies which 16-word pair within a 4x16-word munch the
         * error landed on. The `B<-Pipe4'` accessor encodes these
         * to the active-low form expected by microcode. */
        uint8_t         pipe4_errors;
        uint8_t         pipe4_syndrome;
        uint8_t         pipe4_quadword;
    } pipe[DM_PIPE_DEPTH];
    int     pipe_head;       /* index just past the just-written slot, wrapped */
    uint8_t proc_srn;        /* 4-bit; emulator + fault tasks. Default 0. */
    uint8_t asrn;            /* 4-bit; I/O ring. 2..15. Default 2. */

    /* MapBufBusy timing. Map← shares MapBuf with LoadMcr, ProcSRN, and
     * LoadTestSyndrome; HM §5 says Map← turns MapBufBusy on in its pipe
     * entry and clears it about 9 cycles later. */
    int mapbuf_busy_slot;
    int mapbuf_busy_cycles;

    /* Cache-address-section entry selected by the most recent memory
     * reference. CFlags←A and Pipe5 use this row/column. */
    int last_cache_row;
    int last_cache_way;

    /* Main storage — flat array, 16-bit words. We don't model ECC
     * bits yet; HM §5.7 will add them in Phase B. */
    uint16_t *storage;
    size_t    storage_words;
    int       storage_chip_type;  /* Config.icType, 0..3; default 3 = 4MW/module */

    /* Optional protected cell: stores reaching physical word
     * protect_phys are forced to protect_val while protect_active. A
     * machine-level guard. */
    int       protect_active;
    uint32_t  protect_phys;
    uint16_t  protect_val;

    /* Memory Control Register (MCR). Stored in normal C bit order for
     * the 16-bit value that microcode loads with LoadMcr[A,B]. */
    uint16_t mcr;

    /* DBuf — the data buffer holding the most recent Store's B value.
     * Microcode reads it via `B<-DBuf` (HM Table 11c FA=1 FB=7 FC=5);
     * Midas uses it to inspect pending stores. */
    uint16_t dbuf;

    /* Diagnostic multiplexer ("DMux") address register and muffler
     * read-back (HM testing/diagnostic facility; docs/io-systems-
     * architecture.md "Muffler"). Microcode shifts a DMux address into
     * this register MSB-first via `MidasStrobe<-B` (B[4] per strobe),
     * then either:
     *   - issues `UseDMD` to strobe the address into the BaseBoard
     *     manifold as a WRITE (Initial WriteManifold:
     *     BootstrapSources InitialMain.mc / InitialSelectMain.mc), or
     *   - reads the selected muffler bit back as the SIGN of an
     *     `ALUFMem` read (Various.mc SetDMuxAddress, used by
     *     InitMem.mc GetMemConfig to size VirtualBanks).
     * `dmux_pending` distinguishes "address just loaded, awaiting a
     * UseDMD write or an ALUFMem muffler read" so the muffler override
     * fires ONLY for a genuine SetDMuxAddress read and never for normal
     * ALUFM (Pd<-ALUFMRW) accesses. `dmux_addr` holds the 12-bit
     * DMux address (T[4:15]) captured on the leading strobe. */
    uint16_t dmux_addr;
    int      dmux_pending;

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
    uint8_t           last_fault_task;
    uint8_t           last_fault_subtask;
    dorado_ref_kind   last_fault_ref_kind;
    uint16_t          last_fault_pc;
    uint16_t          last_fault_real_pc;
    uint8_t           last_fault_membase;
    uint8_t           last_fault_tioa;

    /* Most recent storage reference, for bring-up diagnostics. */
    dorado_ref_kind   last_ref_kind;
    uint32_t          last_ref_va;
    uint16_t          last_ref_b;
    uint8_t           last_ref_task;
    uint8_t           last_ref_subtask;
    uint8_t           last_ref_miss;     /* Last reference took a cache-miss
                                          * path that starts storage/map work. */
    int               last_ref_latency;  /* Md-ready latency of the last fetch:
                                          * 3 cache-hit / 16 clean-miss / 24
                                          * dirty-victim miss (HM Table 15,
                                          * docs/memory-architecture.md). Read
                                          * by the engine Hold model. */

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

    /* Optional diagnostic-muffler provider for non-memory DMux addresses
     * such as the disk controller's DskEth status manifold. The callback
     * returns a word with the selected muffler bit in the sign position
     * and sets *handled when it recognizes the address. */
    uint16_t (*dmux_cb)(uint16_t addr, int *handled, void *ctx);
    void *dmux_ctx;
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
uint32_t dorado_map_index(uint32_t va);
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
uint16_t dorado_pipe_map_rp_at(const dorado_memory *mem, int srn);
uint8_t  dorado_pipe_map_flags_at(const dorado_memory *mem, int srn);
uint16_t dorado_pipe2_at(const dorado_memory *mem, int srn);
uint16_t dorado_pipe5_at(const dorado_memory *mem, int srn);
/* Encoded `B<-Pipe4'` for slot `srn`. Mixed-polarity active-low
 * form per HM page 51 / EMemDefs.mc — `0o150361 XOR Pipe4'` yields
 * high-true. The no-error baseline is therefore `0o150361`. (gap C2) */
uint16_t dorado_pipe4_at(const dorado_memory *mem, int srn);

/* Set Pipe4 error bits for the given slot. Errors persist until the
 * next reference to the same slot overwrites them (i.e., until
 * pipe_push runs again with the same SRN). (gap C2)
 *   PIPE4_ERR_MAP_TROUBLE = cache fill failed against the map
 *   PIPE4_ERR_MEM_ERROR   = single-bit ECC, corrected
 *   PIPE4_ERR_EC_FAULT    = uncorrectable ECC */
#define PIPE4_ERR_MAP_TROUBLE  (1u << 0)
#define PIPE4_ERR_MEM_ERROR    (1u << 1)
#define PIPE4_ERR_EC_FAULT     (1u << 2)
void dorado_pipe4_set_error(dorado_memory *mem, int srn,
                            uint8_t err_flags,
                            uint8_t syndrome, uint8_t quadword);
void     dorado_memory_tick(dorado_memory *mem);
void     dorado_cflags_load(dorado_memory *mem, uint16_t a);

/* Set ProcSRN (the slot index used for non-PreFetch-miss task-0/15
 * references). FF function `ProcSRN←B[12:15]` (HM Table 11c FA=1
 * FB=2 FC=7) calls this with B[12:15] = low 4 bits of B. */
void     dorado_proc_srn_set(dorado_memory *mem, uint8_t srn);

/* Snapshot of map flags for the pipe entry at slot `n` (relative to
 * head). Bit layout (high-true): bit 0=WP, bit 1=Dirty, bit 2=Ref.
 * On the B bus the previous RP is read inverted as `B←Pipe3'` /
 * `Map'`; flags are exposed through `B←Pipe4'` / Errors'. */
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

/* Diagnostic-multiplexer ("DMux") / muffler model (see the dmux_addr /
 * dmux_pending fields above).
 *   _strobe: `MidasStrobe<-B`. Captures the DMux address (T[4:15]) on
 *            the leading strobe of a shift sequence and arms a pending
 *            read/write.
 *   _use_dmd: `UseDMD`. Consumes the pending DMux address as a manifold
 *            WRITE (Initial WriteManifold) -- no muffler read follows.
 *   _pending: true iff a DMux address was loaded and not yet consumed.
 *   _read:   consume the pending address as a muffler READ and return
 *            the muffler word, with the selected bit in the SIGN
 *            position (C bit 15) so InitMem.mc GetMemConfig's
 *            Branch[...,R<0] evaluates correctly. Our memory models the
 *            64K-map / VirtualBanks=400C (=65536 entries) configuration,
 *            so MapIs64K (DMux 0o1511) reads sign-SET and MapIs256K
 *            (DMux 0o1512) sign-CLEAR, making GetMemConfig select
 *            VirtualBanks=400C -- the size that matches DM_MAP_ENTRIES
 *            and lets the cold InitMem loop terminate. */
void     dorado_memory_dmux_strobe(dorado_memory *mem, uint16_t b);
void     dorado_memory_dmux_use_dmd(dorado_memory *mem);
int      dorado_memory_dmux_pending(const dorado_memory *mem);
uint16_t dorado_memory_dmux_read(dorado_memory *mem);

/* Load/read MCR. LoadMcr[A,B] takes Mcr[0:10] from A/MarMux and
 * Mcr[13:15] from B/BMux; manual bit numbers are MSB-first. */
void     dorado_mcr_load(dorado_memory *mem, uint16_t a, uint16_t b);
uint16_t dorado_mcr_get(const dorado_memory *mem);
int      dorado_mcr_disbr(const dorado_memory *mem);
int      dorado_mcr_dishold(const dorado_memory *mem);
int      dorado_mcr_noref(const dorado_memory *mem);
int      dorado_mcr_fdmiss(const dorado_memory *mem);
int      dorado_mcr_nowake(const dorado_memory *mem);

/* Cache inspection helpers, for tests and diagnostics.
 *
 * dorado_cache_lookup() returns 1 if the munch containing `va` is
 * cached, with *out_way (if non-NULL) set to the way index.
 *
 * dorado_visible_word_at_va() reads the value a normal cached fetch
 * would see if the munch is present in cache; otherwise it falls back
 * to storage. It is a non-mutating diagnostic/helper path.
 *
 * dorado_storage_at_va() bypasses the cache: translates VA via the
 * Map and returns the backing storage word. Used by tests that need
 * to probe "what's actually in storage" without going through the
 * cache. The map must be mounted; if the entry is Vacant the function
 * returns 0xFFFF (no fault is signaled — this is a probe, not a
 * reference).
 */
int      dorado_cache_lookup(const dorado_memory *mem, uint32_t va,
                             int *out_way);
uint16_t dorado_visible_word_at_va(const dorado_memory *mem, uint32_t va);

/* Inject one word supplied by an emulated memory-mapped I/O device into the
 * virtually addressed cache without writing its mapped backing page. */
void dorado_memory_host_io_write(dorado_memory *mem, uint32_t va,
                                 uint16_t value);
uint16_t dorado_storage_at_va(const dorado_memory *mem, uint32_t va);

/* Coherent debug poke (storage + cache); diagnostic only, not the datapath. */
void     dorado_poke_va(dorado_memory *mem, uint32_t va, uint16_t value);

/* dorado_storage_store_at_va() is the write counterpart of
 * dorado_storage_at_va(): it translates VA via the Map, writes `val`
 * into backing storage, and invalidates any stale cache line holding
 * VA so a subsequent fetch sees the new value. Used by the machine to
 * deposit a boot image (e.g. the Pilot germ) straight into VM. The map
 * must be mounted; returns 0 on success, -1 if the map entry is Vacant
 * (no fault is signaled — this is a deposit, not a reference). */
int      dorado_storage_store_at_va(dorado_memory *mem, uint32_t va,
                                    uint16_t val);

#endif
