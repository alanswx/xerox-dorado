#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Debug: real IM address of the microinstruction issuing the
 * current ref (set by cpu.c each instruction; trace-only). */
int dorado_mem_trace_pc = 0;
extern unsigned long long dorado_trace_cycle;
int dorado_mem_trace_pcx = 0;
int dorado_mem_trace_br31 = 0;
int dorado_mem_trace_op = 0;
int dorado_mem_trace_membase = 0;
int dorado_mem_trace_br = 0;
int dorado_mem_trace_mar = 0;

static uint32_t va_cache_row(uint32_t va);
static int cache_pick_victim(dorado_memory *mem, uint32_t va);

static int map_trace_enabled(void)
{
    static int cached = -1;
    if (cached < 0) {
        const char *env = getenv("DORADO_MAP_TRACE");
        cached = (env && env[0] && env[0] != '0') ? 1 : 0;
    }
    return cached;
}

static int map_trace_index_filter(uint32_t idx)
{
    static int cached = -2;
    if (cached == -2) {
        const char *env = getenv("DORADO_MAP_TRACE_INDEX");
        if (env && env[0]) {
            char *end = NULL;
            unsigned long v = strtoul(env, &end, 0);
            cached = (end && *end == '\0') ? (int)(v & (DM_MAP_ENTRIES - 1)) : -1;
        } else {
            cached = -1;
        }
    }
    return cached >= 0 && idx == (uint32_t)cached;
}

static int fault_trace_mode(void)
{
    static int cached = -1;
    if (cached < 0) {
        const char *env = getenv("DORADO_FAULT_TRACE");
        cached = 0;
        if (env && env[0] && env[0] != '0') {
            cached = (strcmp(env, "all") == 0) ? 2 : 1;
        }
    }
    return cached;
}

static const char *ref_kind_trace_name(dorado_ref_kind kind)
{
    switch (kind) {
    case DM_REF_NONE:      return "none";
    case DM_REF_PREFETCH:  return "prefetch";
    case DM_REF_MAP:       return "map";
    case DM_REF_IOFETCH:   return "iofetch";
    case DM_REF_LONGFETCH: return "longfetch";
    case DM_REF_STORE:     return "store";
    case DM_REF_DUMMYREF:  return "dummyref";
    case DM_REF_FLUSH:     return "flush";
    case DM_REF_IOSTORE:   return "iostore";
    case DM_REF_IFETCH:    return "ifetch";
    case DM_REF_FETCH:     return "fetch";
    }
    return "unknown";
}

enum {
    PIPE5_MAPBUF_BUSY = 0x8000u,  /* manual Pipe5[0] */
    PIPE5_DIRTY       = 0x0080u,  /* manual Pipe5[8] */
    PIPE5_VACANT      = 0x0040u,  /* manual Pipe5[9] */
    PIPE5_WP          = 0x0020u,  /* manual Pipe5[10] */
    PIPE5_BEING_LOAD  = 0x0010u,  /* manual Pipe5[11] */
    PIPE5_VICTIM_SHIFT      = 2,  /* manual Pipe5[12:13] */
    PIPE5_NEXTVICTIM_SHIFT  = 0,  /* manual Pipe5[14:15] */

    CFLAGS_A_DIRTY      = 0x0080u,  /* manual CFlags input bit 8 */
    CFLAGS_A_VACANT     = 0x0040u,
    CFLAGS_A_WP         = 0x0020u,
    CFLAGS_A_BEING_LOAD = 0x0010u,
};

int dorado_memory_init(dorado_memory *mem)
{
    memset(mem, 0, sizeof *mem);
    mem->storage_words = DM_STORAGE_WORDS;
    /* The modeled storage size sets RealPages (config word). 4 modules
     * (16MW) -> RealPages wraps to 0x0000; the Alto emulator's InitMem
     * then cold-zeroes/enumerates the full 16MW (slow) and the RealPages=0
     * wrap can confuse its map loop. DORADO_STORAGE_MODULES (1..4) lets a
     * caller model a smaller machine; AEmu only needs one 4MW module. */
    {
        const char *m = getenv("DORADO_STORAGE_MODULES");
        if (m) {
            int n = atoi(m);
            if (n >= 1 && n <= 4)
                mem->storage_words = DM_STORAGE_MODULE_WORDS * (size_t)n;
        }
    }
    mem->storage = calloc(mem->storage_words, sizeof(uint16_t));
    if (!mem->storage) return -1;

    /* All map entries start Vacant (WP=1, Dirty=1) per HM page 45.
     * Microcode populates them via Map← during startup. */
    for (int i = 0; i < DM_MAP_ENTRIES; i++) {
        mem->map[i].rp    = 0;
        mem->map[i].wp    = 1;
        mem->map[i].dirty = 1;   /* Vacant */
        mem->map[i].ref   = 0;
    }

    /* Cache: all lines start invalid; LRU ordering 0,1,2,3 within
     * each row (way 0 is MRU initially, but valid=0 so any access
     * fills an invalid way before evicting). */
    for (int r = 0; r < DM_CACHE_ROWS; r++) {
        for (int w = 0; w < DM_CACHE_WAYS; w++) {
            mem->cache[r].lru[w] = (uint8_t)w;
        }
    }

    /* SRN registers. ProcSRN starts at 0 (microcode convention).
     * ASRN starts at 2 (the bottom of the I/O ring). */
    mem->proc_srn = 0;
    mem->asrn     = 2;
    mem->mapbuf_busy_slot = -1;
    mem->mapbuf_busy_cycles = 0;
    mem->last_cache_row = -1;
    mem->last_cache_way = -1;
    return 0;
}

void dorado_memory_free(dorado_memory *mem)
{
    free(mem->storage);
    mem->storage = NULL;
    mem->storage_words = 0;
}

/* Encode the WP/Dirty/Ref bits of a map entry into a 3-bit flag word
 * for the pipe (HM page 47: Pipe3 captures pre-reference Map flags). */
static uint8_t encode_map_flags(const dorado_map_entry *e)
{
    return (uint8_t)((e->wp & 1) | ((e->dirty & 1) << 1) | ((e->ref & 1) << 2));
}

/* Push an entry into pipe[srn]. Updates pipe_head so that
 * `dorado_pipe_va(mem, 0)` returns the just-written slot
 * (regardless of whether that's a ProcSRN or ASRN slot). */
static void pipe_push(dorado_memory *mem, int srn, dorado_ref_kind kind,
                      uint32_t va, uint16_t rp_pre, uint8_t flags_pre)
{
    srn &= (DM_PIPE_DEPTH - 1);
    /* Preserve the first-faulting ProcSRN reference's pipe entry until the
     * fault handler reads it. On real Dorado consecutive references get
     * distinct SRNs (a 16-entry ring), so the faulting reference's slot
     * survives any later reference made before the fault task runs. ASRN
     * (I/O) references already cycle through distinct slots here, but the
     * emulator and fault tasks share a single fixed ProcSRN slot, so without
     * this guard the IFU prefetch of the next opcode (also task 0, ProcSRN)
     * would clobber pipe[ProcSRN] (=SRNFirstFault) before AEmu's XMFaultTask
     * Fault0 reads VAHi/VALo — making it take the page-377 store path to
     * MapFault (an unhandled-fault hang) instead of IgnoreStore. Restricted
     * to the ProcSRN slot so the ASRN ring still wraps normally. Released
     * when the fault is cleared (dorado_fault_clear). */
    if (mem->fault_count > 0 &&
        srn == (int)(mem->fault_first_srn & (DM_PIPE_DEPTH - 1)) &&
        srn == (int)(mem->proc_srn & (DM_PIPE_DEPTH - 1))) {
        return;
    }
    mem->pipe[srn].kind          = kind;
    mem->pipe[srn].va            = va;
    mem->pipe[srn].map_rp_pre    = rp_pre;
    mem->pipe[srn].map_flags_pre = flags_pre;
    mem->pipe[srn].mapbuf_busy   = 0;
    mem->pipe[srn].cache_flags   = 0;
    /* C2: each new reference clears the slot's accumulated error
     * state. Errors raised by *this* reference are deposited later
     * via dorado_pipe4_set_error. */
    mem->pipe[srn].pipe4_errors    = 0;
    mem->pipe[srn].pipe4_syndrome  = 0;
    mem->pipe[srn].pipe4_quadword  = 0;
    mem->pipe_head = (srn + 1) % DM_PIPE_DEPTH;
}

uint32_t dorado_pipe_va(const dorado_memory *mem, int n)
{
    /* n=0 returns the just-written slot (= head-1).
     * n=1 returns "one slot back in the ring". With non-sequential
     * SRN allocation (ProcSRN refs always overwrite the same slot,
     * ASRN refs cycle through 2..15), values for n>0 reflect ring
     * order, not strict temporal order. */
    int slot = (mem->pipe_head - 1 - n) % DM_PIPE_DEPTH;
    if (slot < 0) slot += DM_PIPE_DEPTH;
    return mem->pipe[slot].va;
}

uint8_t dorado_pipe_map_flags(const dorado_memory *mem, int n)
{
    int slot = (mem->pipe_head - 1 - n) % DM_PIPE_DEPTH;
    if (slot < 0) slot += DM_PIPE_DEPTH;
    return mem->pipe[slot].map_flags_pre;
}

uint32_t dorado_pipe_va_at(const dorado_memory *mem, int srn)
{
    return mem->pipe[srn & (DM_PIPE_DEPTH - 1)].va;
}

uint16_t dorado_pipe_map_rp_at(const dorado_memory *mem, int srn)
{
    return mem->pipe[srn & (DM_PIPE_DEPTH - 1)].map_rp_pre;
}

uint8_t dorado_pipe_map_flags_at(const dorado_memory *mem, int srn)
{
    return mem->pipe[srn & (DM_PIPE_DEPTH - 1)].map_flags_pre;
}

uint16_t dorado_pipe5_at(const dorado_memory *mem, int srn)
{
    int slot = srn & (DM_PIPE_DEPTH - 1);
    /* Figure 10 shows Pipe5 high-true. Initial's WAITFORMAPBUF reads
     * Pipe5 and branches on ALU<0, so expose MapBufBusy as bit 0 in
     * manual numbering, i.e. the C sign bit. Cache-address-section
     * flags are latched from the hit/victim entry selected by that
     * storage reference. */
    uint16_t v = mem->pipe[slot].cache_flags;
    if (mem->pipe[slot].mapbuf_busy) v |= PIPE5_MAPBUF_BUSY;
    return v;
}

/* `B<-Pipe4'` (gap C2). Mixed-polarity per HM page 51 / EMemDefs.mc.
 * Bit positions, manual MSB-first numbering:
 *   b0  = ref               (high-true: 1 = this slot was a real ref)
 *   b1  = notMapTrouble     (active-low)
 *   b2  = wProtect          (high-true)
 *   b3  = dirty             (high-true)
 *   b4  = notMemError       (active-low: single-bit ECC corrected)
 *   b5  = notEcFault        (active-low: uncorrectable ECC)
 *   b6:7 = quadWord         (which 16-word pair within the munch)
 *   b8:15 = syndrome        (8-bit ECC syndrome)
 * In LSB ordering (bit `i` LSB = bit `15-i` manual):
 *   bit 15: ref            (b0)
 *   bit 14: notMapTrouble  (b1)
 *   bit 13: wProtect       (b2)
 *   bit 12: dirty          (b3)
 *   bit 11: notMemError    (b4)
 *   bit 10: notEcFault     (b5)
 *   bits 9..8: quadWord    (b6:7)
 *   bits 7..0: syndrome    (b8:15)
 * The `0o150361 XOR Pipe4'` invariant gives the no-error baseline
 * `0o150361` = `0xD0F1`. */
uint16_t dorado_pipe4_at(const dorado_memory *mem, int srn)
{
    int slot = srn & (DM_PIPE_DEPTH - 1);
    /* Compose the high-true value first, then XOR with the
     * 0o150361 baseline mask to produce the raw Pipe4' the
     * processor sees. (HM page 51: `0o150361 XOR Pipe4' = high
     * true`.) Bit positions are LSB-mapped from EMemDefs.mc:
     *   bit 15 = ref,    14 = MapTrouble,  13 = wProtect,
     *   bit 12 = dirty,  11 = MemError,    10 = EcFault,
     *   bits 9..8 = quadword,  7..0 = syndrome.
     * wProtect/dirty are the old Map flags snapshotted for the
     * reference; Mesa's NewMemory.mc reads them through Errors'. */
    uint16_t ht = 0;
    uint8_t  e  = mem->pipe[slot].pipe4_errors;
    uint8_t  mf = mem->pipe[slot].map_flags_pre;
    if (mem->pipe[slot].kind != DM_REF_NONE) ht |= (uint16_t)(1u << 15);
    if (e & PIPE4_ERR_MAP_TROUBLE)           ht |= (uint16_t)(1u << 14);
    if (mf & 1u)                              ht |= (uint16_t)(1u << 13);
    if (mf & 2u)                              ht |= (uint16_t)(1u << 12);
    if (e & PIPE4_ERR_MEM_ERROR)             ht |= (uint16_t)(1u << 11);
    if (e & PIPE4_ERR_EC_FAULT)              ht |= (uint16_t)(1u << 10);
    ht |= (uint16_t)((mem->pipe[slot].pipe4_quadword & 3u) << 8);
    ht |= (uint16_t)mem->pipe[slot].pipe4_syndrome;
    return (uint16_t)(0150361u ^ ht);
}

void dorado_pipe4_set_error(dorado_memory *mem, int srn,
                            uint8_t err_flags,
                            uint8_t syndrome, uint8_t quadword)
{
    int slot = srn & (DM_PIPE_DEPTH - 1);
    mem->pipe[slot].pipe4_errors  |= err_flags;
    if (err_flags & (PIPE4_ERR_MEM_ERROR | PIPE4_ERR_EC_FAULT)) {
        mem->pipe[slot].pipe4_syndrome  = syndrome;
        mem->pipe[slot].pipe4_quadword  = (uint8_t)(quadword & 3u);
    }
}

static int mcr_is_initial_nowake(const dorado_memory *mem)
{
    /* InitialSubrs.mc's SetMCR loads mcr.noWake as 0xFEE7 in the current
     * source-level encoding. Treat it as "normal references, no fault
     * wakeups" while the full MCR active-low/schematic decode is resolved. */
    return mem->mcr == 0xFEE7u;
}

static int dorado_mcr_discf(const dorado_memory *mem)
{
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 7) & 1;      /* manual Mcr[8] */
}

static int dorado_mcr_dvavic(const dorado_memory *mem)
{
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 15) & 1;     /* manual Mcr[0] */
}

static int dorado_mcr_usemcrv(const dorado_memory *mem)
{
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 13) & 1;     /* manual Mcr[2] */
}

static int dorado_mcr_victim(const dorado_memory *mem)
{
    return (mem->mcr >> 11) & 3;     /* manual Mcr[3:4] */
}

static int dorado_mcr_nextvictim(const dorado_memory *mem)
{
    return (mem->mcr >> 9) & 3;      /* manual Mcr[5:6] */
}

static uint16_t cache_line_pipe5_flags(const dorado_memory *mem, uint32_t va,
                                       int way)
{
    if (way < 0 || way >= DM_CACHE_WAYS) return 0;

    uint32_t row_idx = va_cache_row(va);
    int next_way = dorado_mcr_usemcrv(mem)
                 ? dorado_mcr_nextvictim(mem)
                 : mem->cache[row_idx].lru[DM_CACHE_WAYS - 1];
    uint16_t v = (uint16_t)(((way & 3) << PIPE5_VICTIM_SHIFT) |
                            ((next_way & 3) << PIPE5_NEXTVICTIM_SHIFT));
    if (dorado_mcr_discf(mem)) return v;

    const dorado_cache_line *line = &mem->cache[row_idx].ways[way];
    if (line->dirty)        v |= PIPE5_DIRTY;
    if (line->vacant)       v |= PIPE5_VACANT;
    if (line->wp)           v |= PIPE5_WP;
    if (line->being_loaded) v |= PIPE5_BEING_LOAD;
    return v;
}

void dorado_cflags_load(dorado_memory *mem, uint16_t a)
{
    if (dorado_mcr_discf(mem)) return;
    if (mem->last_cache_row < 0 || mem->last_cache_row >= DM_CACHE_ROWS) return;
    if (mem->last_cache_way < 0 || mem->last_cache_way >= DM_CACHE_WAYS) return;

    dorado_cache_line *line =
        &mem->cache[mem->last_cache_row].ways[mem->last_cache_way];
    uint16_t flags = (uint16_t)~a;   /* CFlags←A' */
    line->dirty        = (flags & CFLAGS_A_DIRTY) ? 1 : 0;
    line->vacant       = (flags & CFLAGS_A_VACANT) ? 1 : 0;
    line->wp           = (flags & CFLAGS_A_WP) ? 1 : 0;
    line->being_loaded = (flags & CFLAGS_A_BEING_LOAD) ? 1 : 0;
}

void dorado_memory_tick(dorado_memory *mem)
{
    if (mem->mapbuf_busy_cycles <= 0) return;
    mem->mapbuf_busy_cycles--;
    if (mem->mapbuf_busy_cycles == 0) {
        int slot = mem->mapbuf_busy_slot & (DM_PIPE_DEPTH - 1);
        mem->pipe[slot].mapbuf_busy = 0;
        mem->mapbuf_busy_slot = -1;
    }
}

void dorado_proc_srn_set(dorado_memory *mem, uint8_t srn)
{
    mem->proc_srn = (uint8_t)(srn & (DM_PIPE_DEPTH - 1));
}

uint16_t dorado_fault_info(const dorado_memory *mem)
{
    /* High-true FaultInfo register. Field positions per the microcode
     * (AEmu EMemDefs.mc: fi.emuFault = b8 = 0x0080, fi.numfaults =
     * b9:11 = 0x0070; FirstFaultSRN in B[12:15]) and HM §5 "Fault
     * Handling": FaultCnt is a 3-bit counter that reads -1 (all ones)
     * when no faults are pending — "FirstFaultSRN ... is loaded if
     * FaultCnt is -1 (indicating no faults)" — i.e. the field holds
     * (number of pending faults - 1). XMFaultTask.mc depends on all
     * three: numfaults all-ones => no fault, all-zeros => exactly one,
     * emuFault set => the emulator's reference faulted. */
    uint16_t srn   = (uint16_t)(mem->fault_first_srn & 0xF);
    uint16_t cnt   = (uint16_t)(((unsigned)mem->fault_count - 1u) & 7u);
    uint16_t efl   = (uint16_t)((mem->fault_emulator & 1) << 7);
    return (uint16_t)(efl | (cnt << 4) | srn);
}

void dorado_fault_clear(dorado_memory *mem)
{
    mem->fault_count      = 0;
    mem->fault_first_srn  = 0;
    mem->fault_emulator   = 0;
    mem->last_fault       = DM_FAULT_NONE;
    mem->last_fault_va    = 0;
    mem->last_fault_task  = 0;
    mem->last_fault_subtask = 0;
    mem->last_fault_ref_kind = DM_REF_NONE;
    mem->last_fault_pc    = 0;
    mem->last_fault_real_pc = 0;
    mem->last_fault_membase = 0;
    mem->last_fault_tioa  = 0;
    mem->last_ref_kind = DM_REF_NONE;
    mem->last_ref_va = 0;
    mem->last_ref_b = 0;
    mem->last_ref_task = 0;
    mem->last_ref_subtask = 0;
}

void dorado_mcr_load(dorado_memory *mem, uint16_t a, uint16_t b)
{
    /* Manual bit numbering is MSB-first. In C order:
     *   manual Mcr[0:10]  = bits 15..5 from A
     *   manual Mcr[13:15] = bits 2..0 from B
     * Mcr[11:12] are not loaded by this function and are kept clear. */
    mem->mcr = (uint16_t)((a & 0xFFE0u) | (b & 0x0007u));
}

uint16_t dorado_mcr_get(const dorado_memory *mem)
{
    return mem->mcr;
}

int dorado_mcr_disbr(const dorado_memory *mem)
{
    /* Per EMemDefs.mc: `mcr.disBR = b8`. Manual MSB-bit numbering
     * makes b8 = LSB bit 7 — the SAME bit as disCF. The two field
     * names are aliases for one hardware bit; setting it disables
     * both BR-relative virtual addressing and the cache-flag
     * machinery (gap C6). */
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 7) & 1;      /* manual Mcr[8] */
}

int dorado_mcr_dishold(const dorado_memory *mem)
{
    /* Per EMemDefs.mc: `mcr.disHold = b9`. Manual b9 = LSB bit 6.
     * When set, the engine does not Hold on cache miss / Pipe
     * conflict / IFU fault (HM §4 Hold). Initial uses
     * `mcr.noRefHold = b9 | b10` to suppress both Hold AND
     * storage refs during map init; we mirror that intent here.
     * Hold itself is gap B1 — currently a no-op — so this getter
     * exists for completeness pending B1's landing. */
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 6) & 1;      /* manual Mcr[9] */
}

int dorado_mcr_noref(const dorado_memory *mem)
{
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 5) & 1;      /* manual Mcr[10] */
}

int dorado_mcr_fdmiss(const dorado_memory *mem)
{
    if (mcr_is_initial_nowake(mem)) return 0;
    return (mem->mcr >> 14) & 1;     /* manual Mcr[1] */
}

int dorado_mcr_nowake(const dorado_memory *mem)
{
    return mem->mcr & 1;             /* manual Mcr[15] */
}

uint16_t dorado_memory_config_word(const dorado_memory *mem)
{
    /* EMemDefs.mc / B←Config':
     *   ChipSize = b12,b13     (C-LSB bits 3..2)
     *   M0..M3   = 0200,0100,0040,0020
     *
     * The CPU reads Config' active-low, so this helper returns the
     * high-true internal value and cpu.c complements it for B←Config'.
     * Report the 64K-chip configuration for the default backing
     * store. InitMem.mc derives pages-per-module as 0400 << (2*T);
     * T=3 gives 040000 256-word pages, matching each 4MW module.
     */
    enum {
        chip_size_64kx1 = 3,
    };

    size_t modules = mem->storage_words / DM_STORAGE_MODULE_WORDS;
    if (modules == 0 && mem->storage_words != 0) modules = 1;
    if (modules > 4) modules = 4;

    static const uint16_t module_bits[4] = { 0200u, 0100u, 0040u, 0020u };
    uint16_t module_mask = 0;
    for (size_t i = 0; i < modules; i++) module_mask |= module_bits[i];

    return (uint16_t)(module_mask | (chip_size_64kx1 << 2));
}

/* Map index from VA: page-number portion for our 16K-map /
 * 256-word-page configuration. Keep this shared with cpu.c's ReadMap
 * path; Initial depends on ReadMap observing the same entry Map<- wrote. */
uint32_t dorado_map_index(uint32_t va)
{
    return (va >> 8) & (DM_MAP_ENTRIES - 1);   /* page-number portion */
}

/* Page-offset extraction: VA[24:31] = low 8 bits of VA in 256-word
 * page mode. */
static uint32_t va_page_offset(uint32_t va)
{
    return va & (DM_PAGE_SIZE - 1);
}

/* Pure VA→phys translation. Returns the fault kind (PAGE if Vacant,
 * WRITE_PROTECT if writing through a WP=1 entry); on success sets
 * *out_phys to the storage offset. References beyond the installed
 * storage modules report a storage/data error instead of aliasing
 * back into low memory (HM page 59: each module supplies a fixed
 * quarter of real memory and firmware cannot remap the range).
 * **Does not modify Map flags** —
 * Ref and Dirty are managed at higher levels (cache fill, IOStore,
 * dirty-victim writeback) per HM page 47. */
static dorado_fault_kind va_translate(const dorado_memory *mem, uint32_t va,
                                      int is_write, size_t *out_phys)
{
    uint32_t idx = dorado_map_index(va);
    const dorado_map_entry *e = &mem->map[idx];

    /* Vacant: WP=1 AND Dirty=1. */
    if (e->wp && e->dirty) return DM_FAULT_PAGE;
    /* Write-protect violation (only meaningful for writes). */
    if (is_write && e->wp) return DM_FAULT_WRITE_PROTECT;

    uint32_t phys = (uint32_t)(e->rp) * DM_PAGE_SIZE + va_page_offset(va);
    if ((size_t)phys >= mem->storage_words) return DM_FAULT_STORAGE_ERROR;
    *out_phys = (size_t)phys;
    return DM_FAULT_NONE;
}

/* ---- Cache (HM §5.11, page 47 cache/Map invariants) ---- */

static uint32_t va_cache_offset(uint32_t va) { return va & DM_CACHE_LINE_MASK; }
static uint32_t va_cache_row   (uint32_t va) { return (va >> 4) & DM_CACHE_ROW_MASK; }
static uint32_t va_cache_tag   (uint32_t va) { return va >> 10; }
static uint32_t va_munch_base  (uint32_t va) { return va & ~(uint32_t)DM_CACHE_LINE_MASK; }

/* Move `way` to the front of the LRU list for row `r` (becomes MRU).
 * HM doesn't pin down the exact LRU encoding; what matters is that
 * the next victim is whatever way hasn't been touched in longest. */
static void cache_touch_lru(dorado_cache_row *row, int way)
{
    int seen = -1;
    for (int i = 0; i < DM_CACHE_WAYS; i++) {
        if (row->lru[i] == way) { seen = i; break; }
    }
    if (seen < 0) return;
    /* Shift everything before `seen` one slot back, then put `way` at index 0. */
    for (int i = seen; i > 0; i--) row->lru[i] = row->lru[i - 1];
    row->lru[0] = (uint8_t)way;
}

static void cache_select(dorado_memory *mem, uint32_t va, int way, int srn)
{
    mem->last_cache_row = (int)va_cache_row(va);
    mem->last_cache_way = way;
    mem->pipe[srn & (DM_PIPE_DEPTH - 1)].cache_flags =
        cache_line_pipe5_flags(mem, va, way);
}

static void cache_address_write(dorado_memory *mem, uint32_t va, int way)
{
    if (way < 0 || way >= DM_CACHE_WAYS) return;

    dorado_cache_line *line = &mem->cache[va_cache_row(va)].ways[way];
    line->tag = va_cache_tag(va);
    line->valid = 1;
}

static uint32_t cache_address_va(const dorado_memory *mem, uint32_t va, int way)
{
    if (way < 0 || way >= DM_CACHE_WAYS) return 0;
    uint32_t row_idx = va_cache_row(va);
    const dorado_cache_line *line = &mem->cache[row_idx].ways[way];
    if (!line->valid) return 0;
    return (line->tag << 10) | (row_idx << 4);
}

/* Lookup VA in the cache. Returns 1 if any way matches; sets *out_way
 * if non-NULL. Does NOT update LRU (caller decides). */
int dorado_cache_lookup(const dorado_memory *mem, uint32_t va, int *out_way)
{
    const dorado_cache_row *row = &mem->cache[va_cache_row(va)];
    uint32_t tag = va_cache_tag(va);
    for (int w = 0; w < DM_CACHE_WAYS; w++) {
        if (row->ways[w].valid && !row->ways[w].vacant &&
            row->ways[w].tag == tag) {
            if (out_way) *out_way = w;
            return 1;
        }
    }
    if (out_way) *out_way = -1;
    return 0;
}

uint16_t dorado_storage_at_va(const dorado_memory *mem, uint32_t va)
{
    size_t phys;
    if (va_translate(mem, va, /*is_write=*/0, &phys) != DM_FAULT_NONE)
        return 0xFFFF;
    return mem->storage[phys];
}

uint16_t dorado_visible_word_at_va(const dorado_memory *mem, uint32_t va)
{
    int way;
    if (dorado_cache_lookup(mem, va, &way)) {
        const dorado_cache_line *line = &mem->cache[va_cache_row(va)].ways[way];
        return line->data[va_cache_offset(va)];
    }
    return dorado_storage_at_va(mem, va);
}

/* Pick a way for a new fill in the row containing `va`. Prefer an
 * invalid way; otherwise the LRU way. Returns way index. Caller is
 * responsible for writing back the victim if it was dirty (call
 * cache_writeback_victim first). */
static int cache_pick_victim(dorado_memory *mem, uint32_t va)
{
    if (dorado_mcr_usemcrv(mem)) return dorado_mcr_victim(mem);

    dorado_cache_row *row = &mem->cache[va_cache_row(va)];
    /* Prefer invalid. */
    for (int w = 0; w < DM_CACHE_WAYS; w++) {
        if (!row->ways[w].valid) return w;
    }
    /* Otherwise the LRU. */
    return row->lru[DM_CACHE_WAYS - 1];
}

/* Compute the storage offset of the first word of the munch containing
 * `va`, via the Map. Returns DM_FAULT_NONE on success. The phys offset
 * always points to a 16-word-aligned storage word (since `va & ~0xF`
 * stays within one Map page in any of HM's configurations). */
static dorado_fault_kind munch_phys_base(const dorado_memory *mem,
                                         uint32_t va, size_t *out_phys)
{
    return va_translate(mem, va_munch_base(va), /*is_write=*/0, out_phys);
}

/* Write back the dirty contents of a cache line to storage, then
 * mark Map.Ref and Map.Dirty for that line's page (HM page 47:
 * "If the victim for the miss ... is dirty, Ref and Dirty for its
 * map entry also get set"). The line itself is not invalidated.
 *
 * Returns DM_FAULT_NONE on success, or the fault kind if translate
 * fails. Real hardware reports a *dirty-victim* fault (HM §5):
 * the line's data is dropped, Pipe4 records the syndrome for the
 * triggering reference, and FaultInfo is updated. We don't model
 * Pipe4 syndrome bits yet (gap C2), but we propagate the kind so
 * the caller can advance fault_count / fault_first_srn (gap C4). */
static dorado_fault_kind cache_writeback_line(dorado_memory *mem,
                                              int row_idx, int way)
{
    dorado_cache_line *line = &mem->cache[row_idx].ways[way];
    if (!line->valid || !line->dirty) return DM_FAULT_NONE;
    /* Reconstruct the VA of the line: tag||row||0_offset. */
    uint32_t va_base = (line->tag << 10) | ((uint32_t)row_idx << 4);
    size_t phys;
    /* Use is_write=1 so a now-WP page reports DM_FAULT_WRITE_PROTECT
     * rather than succeeding with a stale phys. */
    dorado_fault_kind f = va_translate(mem, va_base, /*is_write=*/1, &phys);
    if (f == DM_FAULT_NONE) {
        for (int i = 0; i < DM_CACHE_LINE_W; i++) {
            size_t p = phys + (size_t)i;
            if (p < mem->storage_words) mem->storage[p] = line->data[i];
        }
        /* HM: dirty-victim write sets Map.Ref AND Map.Dirty. */
        uint32_t idx = dorado_map_index(va_base);
        mem->map[idx].ref   = 1;
        mem->map[idx].dirty = 1;
    }
    line->dirty = 0;
    return f;
}

/* Note (gap C4): a dirty-victim writeback whose Map says the page is
 * now WP or Vacant is reported via this return value. Record it as a
 * fault on the *triggering* reference's SRN. The Pipe4 syndrome is
 * also updated so microcode reading `B<-Pipe4'` sees MapTrouble for
 * that slot (gap C2). */
static void record_writeback_fault(dorado_memory *mem,
                                   dorado_fault_kind f, int srn)
{
    if (f == DM_FAULT_NONE) return;
    if (mem->fault_count == 0) {
        mem->fault_first_srn = (uint8_t)(srn & 0xF);
        mem->fault_emulator  = 1;
    }
    if (mem->fault_count < 0xF) mem->fault_count++;
    mem->last_fault    = f;
    /* mem->last_fault_va is left as the triggering ref's VA — that's
     * what microcode reads via Pipe0/Pipe1. The dirty-victim VA is
     * recoverable from cache state if needed. */
    dorado_pipe4_set_error(mem, srn, PIPE4_ERR_MAP_TROUBLE, 0, 0);
}

/* Fill `way` in the row of `va` with the 16 words of the munch
 * containing `va`. Translates VA via the Map (caller has already
 * checked for fault). Sets Map.Ref (HM page 47: "any cache reference
 * or PreFetch← that misses ... sets Ref in the map"). */
static void cache_fill(dorado_memory *mem, uint32_t va, int way)
{
    dorado_cache_line *line = &mem->cache[va_cache_row(va)].ways[way];
    size_t phys;
    if (munch_phys_base(mem, va, &phys) != DM_FAULT_NONE) return;

    line->tag   = va_cache_tag(va);
    line->valid = 1;
    line->dirty = 0;
    line->wp = mem->map[dorado_map_index(va)].wp;
    line->vacant = 0;
    line->being_loaded = 0;
    for (int i = 0; i < DM_CACHE_LINE_W; i++) {
        size_t p = phys + (size_t)i;
        line->data[i] = (p < mem->storage_words) ? mem->storage[p] : 0;
    }
    /* Cache miss → Map.Ref set on the page. */
    mem->map[dorado_map_index(va)].ref = 1;

    cache_touch_lru(&mem->cache[va_cache_row(va)], way);
}

/* Invalidate any cache entry for the munch containing `va`. Used by
 * IOStore (HM page 40: "a munch in the cache is unconditionally
 * removed (without being stored if dirty)"). Returns 1 if a line was
 * present (and discarded), 0 otherwise. */
static int cache_invalidate_no_writeback(dorado_memory *mem, uint32_t va)
{
    int way;
    if (!dorado_cache_lookup(mem, va, &way)) return 0;
    mem->cache[va_cache_row(va)].ways[way].valid = 0;
    mem->cache[va_cache_row(va)].ways[way].dirty = 0;
    mem->cache[va_cache_row(va)].ways[way].vacant = 1;
    return 1;
}

dorado_fault_kind dorado_memory_ref(dorado_memory *mem, dorado_ref_kind kind,
                                    uint32_t va, uint16_t b, uint16_t tioa)
{
    return dorado_memory_ref_task(mem, kind, va, b, tioa, 0, 0);
}

dorado_fault_kind dorado_memory_ref_task(dorado_memory *mem,
                                         dorado_ref_kind kind,
                                         uint32_t va, uint16_t b,
                                         uint16_t tioa,
                                         int task, int subtask)
{
    (void)subtask;     /* used only for fast-IO branches below */
    mem->last_ref_kind = kind;
    mem->last_ref_va = va;
    mem->last_ref_b = b;
    mem->last_ref_task = (uint8_t)(task & 017);
    mem->last_ref_subtask = (uint8_t)(subtask & 3);
    /* Update Mar (most-recent reference VA). ReadMap (HM page 41)
     * uses this to look up the map entry. */
    mem->mar = va;
    /* Snapshot the map entry's pre-reference flags into the pipe slot.
     * HM page 47: "Every storage reference causes mapping and returns
     * old contents of the relevant map entry in the pipe." */
    uint32_t idx_snapshot = dorado_map_index(va);
    uint16_t rp_pre       = mem->map[idx_snapshot].rp;
    uint8_t  flags_pre    = encode_map_flags(&mem->map[idx_snapshot]);

    /* SRN selection (HM page 51-52). The pipe is a single 16-entry memory
     * addressed by SRN; the emulator (task 0) and fault task (task 15) use
     * ProcSRN, while all I/O tasks (1..14) use ASRN. This keeps an I/O
     * task's references from clobbering the emulator's pipe entry when it
     * preempts the emulator mid-reference-readback. Without this, an I/O
     * task's Fetch/Store (kinds that are NOT IOFetch/IOStore) would land in
     * pipe[ProcSRN] and corrupt e.g. InitMem's NextMapEntry VALo/VAHi
     * readback (the DummyRef VA), hanging the map enumeration. PreFetch-
     * with-miss also uses ASRN (handled below). */
    int io_task_uses_asrn = (task != 0 && task != 017);
    int use_asrn = io_task_uses_asrn ||
                   (kind == DM_REF_IOFETCH) || (kind == DM_REF_IOSTORE);
    int prefetch_was_miss = 0;
    if (kind == DM_REF_PREFETCH) {
        if (!dorado_mcr_noref(mem) &&
            (dorado_mcr_fdmiss(mem) || !dorado_cache_lookup(mem, va, NULL))) {
            use_asrn = 1;
            prefetch_was_miss = 1;
        }
    }
    int srn = use_asrn ? mem->asrn : mem->proc_srn;

    if (dorado_mcr_dvavic(mem) &&
        kind != DM_REF_MAP && kind != DM_REF_DUMMYREF && kind != DM_REF_NONE) {
        int way = dorado_mcr_usemcrv(mem) ? dorado_mcr_victim(mem)
                                          : cache_pick_victim(mem, va);
        uint32_t pipe_va = cache_address_va(mem, va, way);
        pipe_push(mem, srn, kind, pipe_va, rp_pre, flags_pre);
        cache_select(mem, va, way, srn);
        return DM_FAULT_NONE;
    }

    /* Pipe entries are pushed for *every* reference, including ones
     * that fault — so fault microcode can read the offending VA
     * from Pipe0/Pipe1. */
    pipe_push(mem, srn, kind, va, rp_pre, flags_pre);

    size_t phys = 0;
    dorado_fault_kind f = DM_FAULT_NONE;
    int started_map = 0;   /* set true if the ref goes to storage */

    /* unused in the no-cache miss path below; declared once for all cases. */
    (void)phys;

    switch (kind) {
    case DM_REF_FETCH:
    case DM_REF_IFETCH:
    case DM_REF_LONGFETCH: {
        int way;
        if (dorado_mcr_noref(mem)) {
            way = dorado_mcr_usemcrv(mem) ? dorado_mcr_victim(mem)
                                          : cache_pick_victim(mem, va);
            cache_select(mem, va, way, srn);
            f = DM_FAULT_NONE;
        } else if (!dorado_mcr_fdmiss(mem) &&
                   dorado_cache_lookup(mem, va, &way)) {
            /* Hit: deliver Md directly from the cache line. No
             * Map.Ref update — HM page 47: only misses set Ref. */
            mem->md = mem->cache[va_cache_row(va)].ways[way]
                          .data[va_cache_offset(va)];
            cache_touch_lru(&mem->cache[va_cache_row(va)], way);
            cache_select(mem, va, way, srn);
        } else {
            /* Miss: translate (fault check), then fill. */
            f = va_translate(mem, va, /*is_write=*/0, &phys);
            if (f == DM_FAULT_NONE) {
                int victim = cache_pick_victim(mem, va);
                {
                    dorado_fault_kind wbf =
                        cache_writeback_line(mem, va_cache_row(va), victim);
                    record_writeback_fault(mem, wbf, srn);
                }
                cache_fill(mem, va, victim);
                mem->md = mem->cache[va_cache_row(va)].ways[victim]
                              .data[va_cache_offset(va)];
                cache_select(mem, va, victim, srn);
            }
        }
        break;
    }
    case DM_REF_PREFETCH: {
        /* HM page 39: "PreFetch← does not clobber Md and never causes
         * a map fault." Walks the Map silently; on Vacant, no-op. */
        if (!dorado_mcr_noref(mem) &&
            (dorado_mcr_fdmiss(mem) || !dorado_cache_lookup(mem, va, NULL))) {
            size_t phys_pf;
            if (va_translate(mem, va, /*is_write=*/0, &phys_pf) == DM_FAULT_NONE) {
                int victim = cache_pick_victim(mem, va);
                {
                    dorado_fault_kind wbf =
                        cache_writeback_line(mem, va_cache_row(va), victim);
                    record_writeback_fault(mem, wbf, srn);
                }
                cache_fill(mem, va, victim);
                cache_select(mem, va, victim, srn);
            }
            (void)phys_pf;
        }
        f = DM_FAULT_NONE;
        break;
    }
    case DM_REF_STORE: {
        int way;
        /* DBuf <- B (HM page 45 / EMemDefs.mc). Captured here so the
         * `B<-DBuf` FF source can return the most-recent Store's
         * value (gap B3 sub-item). */
        mem->dbuf = b;
        {
            static long lo = -1, hi = -1;
            if (lo == -1) {
                const char *w = getenv("DORADO_STORE_TRACE_VA");
                lo = 0; hi = 0;
                if (w) sscanf(w, "%lo,%lo", &lo, &hi);
            }
            if (hi && va >= (uint32_t)lo && va <= (uint32_t)hi) {
                fprintf(stderr,
                        "STORE_VA cyc=%llu task=%o pc=0o%o va=%07o "
                        "data=%06o pcx=0o%o br31=0o%o op=0o%o\n",
                        dorado_trace_cycle, task & 017,
                        dorado_mem_trace_pc,
                        va & 0x0FFFFFFFu, b & 0177777,
                        dorado_mem_trace_pcx, dorado_mem_trace_br31,
                        dorado_mem_trace_op);
            }
        }
        /* Hit *or* miss, the WP check happens via Map (translate).
         * Per HM page 45: Store-hit does NOT set Map.Dirty — that
         * only happens when the dirty munch is later chosen as
         * victim. */
        if (dorado_mcr_noref(mem)) {
            way = dorado_mcr_usemcrv(mem) ? dorado_mcr_victim(mem)
                                          : cache_pick_victim(mem, va);
            cache_address_write(mem, va, way);
            cache_select(mem, va, way, srn);
            f = DM_FAULT_NONE;
            break;
        }
        f = va_translate(mem, va, /*is_write=*/1, &phys);
        {
            static long tp = -1;
            if (tp == -1) {
                const char *w = getenv("DORADO_STORE_TRACE_PHYS");
                tp = w ? strtol(w, NULL, 8) : 0;
            }
            if (tp && (uint32_t)phys == (uint32_t)tp)
                fprintf(stderr,
                    "STORE_PHYS cyc=%llu task=%o pc=0o%o va=%07o "
                    "phys=%07o data=0o%o mb=%o br=%07o mar=%06o\n",
                    dorado_trace_cycle, task & 017, dorado_mem_trace_pc,
                    va & 0x0FFFFFFFu, (unsigned)phys, b & 0177777,
                    dorado_mem_trace_membase, (unsigned)dorado_mem_trace_br,
                    dorado_mem_trace_mar & 0177777);
        }
        /* Protected cell (machine bring-up guard): force the protected
         * PHYSICAL word to its held value, regardless of which bank/VA
         * reaches it. Holds the OS divide-transfer vector M[0o344]
         * against a page-zero BitBlt gray-fill that otherwise corrupts
         * it and crashes the booted world. NOTE: the BitBlt destination
         * arithmetic is CORRECT and faithful to hardware (the
         * bottom-to-top base-high decrement to -1 plus the 28-bit BR
         * adder wrap lands the gray fill exactly where the BBT says);
         * the real defect is upstream -- a malformed BBT whose
         * destination base is already in page zero, traced to a corrupt
         * BCPL context stack pointer (see docs/CONTINUE-HERE.md). */
        if (mem->protect_active && (uint32_t)phys == mem->protect_phys) {
            if (b != mem->protect_val && getenv("DORADO_M344_WATCH"))
                fprintf(stderr,
                    "M344_WRITE cyc=%llu task=%o pc=0o%o va=%07o "
                    "phys=%07o data=0o%o (held 0o%o)\n",
                    dorado_trace_cycle, task & 017, dorado_mem_trace_pc,
                    va & 0x0FFFFFFFu, (unsigned)phys, b & 0177777,
                    mem->protect_val & 0177777);
            b = mem->protect_val;
        }
        if (f == DM_FAULT_NONE) {
            if (dorado_mcr_fdmiss(mem) ||
                !dorado_cache_lookup(mem, va, &way)) {
                /* Miss: write-allocate. Fill, then write into the line. */
                way = cache_pick_victim(mem, va);
                {
                    dorado_fault_kind wbf =
                        cache_writeback_line(mem, va_cache_row(va), way);
                    record_writeback_fault(mem, wbf, srn);
                }
                cache_fill(mem, va, way);
            }
            dorado_cache_line *line =
                &mem->cache[va_cache_row(va)].ways[way];
            line->data[va_cache_offset(va)] = b;
            line->dirty = 1;
            line->vacant = 0;
            cache_touch_lru(&mem->cache[va_cache_row(va)], way);
            cache_select(mem, va, way, srn);
        }
        break;
    }
    case DM_REF_IOFETCH: {
        /* Fast-IO munch out (HM page 39). Reference passes to
         * storage; the 16-word munch travels on the Fin bus to the
         * receiving device. If the line is in cache and dirty, real
         * HW sends the dirty version — we just read storage (close
         * enough until we model dirty-line bypass). No cache entry
         * is created. Sets Map.Ref.
         *
         * If a fast_io_cb is registered, gather the munch from
         * storage (16-word aligned) and hand it to the device via
         * the callback. */
        if (dorado_mcr_noref(mem)) {
            int way = dorado_mcr_usemcrv(mem) ? dorado_mcr_victim(mem)
                                              : cache_pick_victim(mem, va);
            cache_select(mem, va, way, srn);
            f = DM_FAULT_NONE;
            break;
        }
        f = va_translate(mem, va, /*is_write=*/0, &phys);
        if (f == DM_FAULT_NONE) {
            mem->map[dorado_map_index(va)].ref = 1;
            if (mem->fast_io_cb && mem->storage) {
                uint16_t munch[16];
                uint32_t base = phys & ~(uint32_t)0xF;
                for (int i = 0; i < 16; i++) {
                    munch[i] = (base + i) < mem->storage_words
                             ? mem->storage[base + i] : 0;
                }
                mem->fast_io_cb(mem, kind, task, subtask, va, munch,
                                mem->fast_io_ctx);
            }
        }
        break;
    }
    case DM_REF_IOSTORE: {
        /* HM page 40: "a munch in the cache is unconditionally
         * removed (without being stored if dirty)". HM page 45:
         * IOStore sets Map.Dirty.
         *
         * If a fast_io_cb is registered, the device fills a 16-word
         * munch buffer; we then write it to storage. */
        if (dorado_mcr_noref(mem)) {
            int way = dorado_mcr_usemcrv(mem) ? dorado_mcr_victim(mem)
                                              : cache_pick_victim(mem, va);
            cache_select(mem, va, way, srn);
            f = DM_FAULT_NONE;
            break;
        }
        f = va_translate(mem, va, /*is_write=*/1, &phys);
        if (f == DM_FAULT_NONE) {
            cache_invalidate_no_writeback(mem, va);
            mem->map[dorado_map_index(va)].ref   = 1;
            mem->map[dorado_map_index(va)].dirty = 1;
            if (mem->fast_io_cb && mem->storage) {
                uint16_t munch[16] = {0};
                /* Callback fills munch from device. */
                mem->fast_io_cb(mem, kind, task, subtask, va, munch,
                                mem->fast_io_ctx);
                uint32_t base = phys & ~(uint32_t)0xF;
                for (int i = 0; i < 16; i++) {
                    if ((base + i) < mem->storage_words) {
                        mem->storage[base + i] = munch[i];
                    }
                }
            }
        }
        break;
    }
    case DM_REF_MAP: {
        /* Map← writes B[0:15] = RP and TIOA[0:1] = WP, Dirty into
         * the map entry indexed by VA. Per HM page 46: "Map← zeroes
         * Ref". Map← never faults. The previous map contents land
         * in the pipe (we approximate by leaving it in the entry
         * being overwritten — real hardware copies the OLD entry
         * into the pipe). */
        uint32_t idx = dorado_map_index(va);
        dorado_map_entry *e = &mem->map[idx];
        e->rp    = b;
        /* TIOA[0:1] in manual = bits 0 (MSB) and 1 = C-LSB bits 7,6
         * of an 8-bit TIOA. */
        e->wp    = (tioa >> 7) & 1;
        e->dirty = (tioa >> 6) & 1;
        e->ref   = 0;
        if (map_trace_enabled() &&
            (map_trace_index_filter(idx) ||
             idx < 2 || idx == 0x52 || idx == 0x56 ||
             idx == 0xFF || idx == 0x200 || idx == 0x201 ||
             idx == 0x2FE || idx == 0x2FF)) {
            fprintf(stderr,
                    "MAP_TRACE task=%o va=%05X idx=%04X rp=%04X "
                    "wp=%u dirty=%u tioa=%02X\n",
                    task & 017, va & 0x0FFFFFFFu, idx, e->rp,
                    e->wp, e->dirty, tioa & 0xFFu);
        }
        mem->pipe[srn & (DM_PIPE_DEPTH - 1)].mapbuf_busy = 1;
        mem->mapbuf_busy_slot = srn & (DM_PIPE_DEPTH - 1);
        mem->mapbuf_busy_cycles = 9;
        break;
    }
    case DM_REF_FLUSH: {
        /* HM page 40: Flush← unconditionally removes a munch
         * containing VA from the cache, storing it first if dirty.
         * No-op on a clean miss. Dirty hit triggers a FlushStore
         * reference: writes back, sets Map.Ref AND Map.Dirty. */
        int way;
        if (dorado_cache_lookup(mem, va, &way)) {
            dorado_fault_kind wbf =
                cache_writeback_line(mem, va_cache_row(va), way);
            record_writeback_fault(mem, wbf, srn);
            mem->cache[va_cache_row(va)].ways[way].valid = 0;
            mem->cache[va_cache_row(va)].ways[way].vacant = 1;
            cache_select(mem, va, way, srn);
        } else {
            way = cache_pick_victim(mem, va);
            cache_select(mem, va, way, srn);
        }
        break;
    }
    case DM_REF_DUMMYREF:
        /* Pipe-only — already pushed above. */
        break;
    case DM_REF_NONE:
        break;
    }

    if (f != DM_FAULT_NONE) {
        mem->last_fault          = f;
        mem->last_fault_va       = va;
        mem->last_fault_task     = (uint8_t)(task & 017);
        mem->last_fault_subtask  = (uint8_t)(subtask & 3);
        mem->last_fault_ref_kind = kind;
        int fault_trace = fault_trace_mode();
        if (fault_trace && (fault_trace > 1 || task != 0)) {
            uint32_t idx = dorado_map_index(va);
            const dorado_map_entry *e = &mem->map[idx];
            fprintf(stderr,
                    "FAULT_TRACE kind=%s task=%o sub=%u va=%05X idx=%04X "
                    "rp=%04X wp=%u dirty=%u ref=%u tioa=%02X srn=%u\n",
                    ref_kind_trace_name(kind), task & 017, subtask & 3,
                    va & 0x0FFFFFFFu, idx, e->rp, e->wp, e->dirty,
                    e->ref, tioa & 0xFFu, srn & 0xF);
        }

        /* Update FaultInfo register state. The first uncleared fault
         * locks in fault_first_srn; EmulatorFault is set when the
         * emulator's reference faulted (XMFaultTask.mc dispatches
         * NotEmuFault when it is clear). */
        if (mem->fault_count == 0) {
            mem->fault_first_srn = (uint8_t)(srn & 0xF);
        }
        if (task == 0) mem->fault_emulator = 1;
        if (mem->fault_count < 0xF) mem->fault_count++;
    }

    /* ASRN advancement (HM page 52). For ASRN-using refs, advance
     * iff the reference "starts the map" — i.e., goes to storage.
     * In our model: IOFetch, IOStore, and PreFetch-miss always
     * start the map (Vacant PreFetch is a silent no-op but still
     * starts the map per the spec). */
    if (use_asrn) {
        (void)prefetch_was_miss;
        (void)started_map;   /* All ASRN-using refs start the map. */
        /* Ring 2..15 = 14 slots. Advance with wrap. */
        uint8_t next = (uint8_t)(mem->asrn + 1);
        if (next > (DM_PIPE_DEPTH - 1)) next = 2;
        mem->asrn = next;
    }
    return f;
}

void dorado_map_set(dorado_memory *mem, uint32_t va_page,
                    uint16_t rp, int wp, int dirty)
{
    uint32_t idx = va_page & (DM_MAP_ENTRIES - 1);
    mem->map[idx].rp    = rp;
    mem->map[idx].wp    = (uint8_t)(wp ? 1 : 0);
    mem->map[idx].dirty = (uint8_t)(dirty ? 1 : 0);
    mem->map[idx].ref   = 0;
}

const dorado_map_entry *dorado_map_get(const dorado_memory *mem,
                                       uint32_t va_page)
{
    return &mem->map[va_page & (DM_MAP_ENTRIES - 1)];
}

/* BrLo←A loads BR[MemBase][16:31] ← A[0:15] (HM page 37, Table 11c
 * FA=1 FB=2 FC=3). The "lo" half of the 28-bit BR. */
void dorado_br_lo_load(dorado_memory *mem, int membase, uint16_t a)
{
    if (getenv("DORADO_BR_TRACE")) {
        fprintf(stderr, "BRLO mb=%02o a=%06o disbr=%d ref_task=%o\n",
                membase & 0x1F, a, dorado_mcr_disbr(mem),
                mem->last_ref_task & 017);
    }
    if (dorado_mcr_disbr(mem)) return;
    uint32_t cur = mem->br[membase & 0x1F];
    cur = (cur & 0xFFFF0000u) | (uint32_t)a;
    mem->br[membase & 0x1F] = cur & 0x0FFFFFFFu;
}

/* BrHi←A loads BR[MemBase][4:15] ← A[4:15] (HM page 37, FA=1 FB=2
 * FC=4). The high 12 bits of the 28-bit BR; bits 0:3 are reserved.
 * In manual MSB-first 16-bit A: bits 4:15 = the low 12 bits.
 * In our 28-bit BR layout (low 28 bits used), the BR[4:15] manual
 * corresponds to BR_C bits 27..16 (the upper 12 bits). */
void dorado_br_hi_load(dorado_memory *mem, int membase, uint16_t a)
{
    if (getenv("DORADO_BR_TRACE")) {
        fprintf(stderr, "BRHI mb=%02o a=%06o disbr=%d ref_task=%o\n",
                membase & 0x1F, a, dorado_mcr_disbr(mem),
                mem->last_ref_task & 017);
    }
    if (dorado_mcr_disbr(mem)) return;
    uint32_t cur = mem->br[membase & 0x1F];
    /* low 12 bits of A (manual A[4:15]) into bits 27..16 of BR. */
    cur = (cur & 0x0000FFFFu) | (((uint32_t)a & 0x0FFFu) << 16);
    mem->br[membase & 0x1F] = cur & 0x0FFFFFFFu;
}

uint32_t dorado_br_get(const dorado_memory *mem, int membase)
{
    return mem->br[membase & 0x1F] & 0x0FFFFFFFu;
}
