#include "memory.h"

#include <stdlib.h>
#include <string.h>

int dorado_memory_init(dorado_memory *mem)
{
    memset(mem, 0, sizeof *mem);
    mem->storage_words = DM_STORAGE_WORDS;
    mem->storage = calloc(mem->storage_words, sizeof(uint16_t));
    if (!mem->storage) return -1;
    return 0;
}

void dorado_memory_free(dorado_memory *mem)
{
    free(mem->storage);
    mem->storage = NULL;
    mem->storage_words = 0;
}

/* Push a new entry on the pipe ring. The "head" points at the next
 * slot to write, so after push, the most recent entry is at
 * (head - 1) mod depth. */
static void pipe_push(dorado_memory *mem, dorado_ref_kind kind, uint32_t va)
{
    mem->pipe[mem->pipe_head].kind = kind;
    mem->pipe[mem->pipe_head].va   = va;
    mem->pipe_head = (mem->pipe_head + 1) % DM_PIPE_DEPTH;
}

uint32_t dorado_pipe_va(const dorado_memory *mem, int n)
{
    /* n=0 returns the most recently pushed entry (= head-1).
     * n=1 returns the one before, etc. */
    int slot = (mem->pipe_head - 1 - n) % DM_PIPE_DEPTH;
    if (slot < 0) slot += DM_PIPE_DEPTH;
    return mem->pipe[slot].va;
}

/* Translate a 28-bit VA to a storage offset. Stub — no Map, no
 * faults; just modulo storage size. */
static size_t va_to_phys(const dorado_memory *mem, uint32_t va)
{
    return (size_t)(va & 0x0FFFFFFFu) & (mem->storage_words - 1);
}

void dorado_memory_ref(dorado_memory *mem, dorado_ref_kind kind,
                       uint32_t va, uint16_t b)
{
    /* Pipe entries are pushed for *every* reference that touches
     * storage, plus DummyRef. The HM page 37 says PreFetch and
     * DummyRef "moves the 16-word munch containing VA to the cache"
     * / "writes VA into the pipe entry for the reference without
     * initiating cache, map, or storage activity"; both push to
     * the pipe. */
    pipe_push(mem, kind, va);

    switch (kind) {
    case DM_REF_FETCH:
    case DM_REF_IFETCH:
    case DM_REF_LONGFETCH:
        mem->md = mem->storage[va_to_phys(mem, va)];
        break;
    case DM_REF_PREFETCH:
        /* No Md update. Real cache would prime; no cache, no-op. */
        break;
    case DM_REF_STORE:
        mem->storage[va_to_phys(mem, va)] = b;
        break;
    case DM_REF_IOFETCH:
    case DM_REF_IOSTORE:
        /* Fast-IO munches — issued by io tasks for device transfers.
         * Without device modeling, treat as no-op. */
        break;
    case DM_REF_MAP:
        /* Map write — would update the page table entry. Without
         * Map modeling, no-op. */
        break;
    case DM_REF_FLUSH:
        /* Cache flush — remove a munch (writeback if dirty). No
         * cache, so no-op. */
        break;
    case DM_REF_DUMMYREF:
        /* Pipe-only — already done by pipe_push. */
        break;
    case DM_REF_NONE:
        break;
    }
}

/* BrLo←A loads BR[MemBase][16:31] ← A[0:15] (HM page 37, Table 11c
 * FA=1 FB=2 FC=3). The "lo" half of the 28-bit BR. */
void dorado_br_lo_load(dorado_memory *mem, int membase, uint16_t a)
{
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
    uint32_t cur = mem->br[membase & 0x1F];
    /* low 12 bits of A (manual A[4:15]) into bits 27..16 of BR. */
    cur = (cur & 0x0000FFFFu) | (((uint32_t)a & 0x0FFFu) << 16);
    mem->br[membase & 0x1F] = cur & 0x0FFFFFFFu;
}

uint32_t dorado_br_get(const dorado_memory *mem, int membase)
{
    return mem->br[membase & 0x1F] & 0x0FFFFFFFu;
}
