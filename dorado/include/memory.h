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

typedef struct dorado_memory {
    /* 32 base registers, each 28-bit. Stored as uint32_t; the high 4
     * bits are unused. HM §5: BR addresses 1 of 32 28-bit registers,
     * subtask[0:1] OR'd with MemBase[3:4] for io tasks. */
    uint32_t br[DM_BR_COUNT];

    /* 16-bit memory data register. HM page 39: "Md remains valid
     * until and during the next fetch by the task." */
    uint16_t md;

    /* Pipe ring — most recent storage references. Reads via
     * B←Pipe0..5 FF functions look at the head and step backwards. */
    struct {
        uint32_t        va;
        dorado_ref_kind kind;
    } pipe[DM_PIPE_DEPTH];
    int pipe_head;          /* next slot to write */

    /* Main storage — flat array, 16-bit words. We don't model ECC
     * bits yet; HM §5.7 will add them in Phase B. */
    uint16_t *storage;
    size_t    storage_words;
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
 * `b` is the B-bus value (data for stores).
 * `out_md` is set to the new Md value for fetches; left alone
 * otherwise. NULL means caller doesn't care.
 */
void dorado_memory_ref(dorado_memory *mem, dorado_ref_kind kind,
                       uint32_t va, uint16_t b);

/* Helpers for FF functions that load/read BR. */
void     dorado_br_lo_load(dorado_memory *mem, int membase, uint16_t a);
void     dorado_br_hi_load(dorado_memory *mem, int membase, uint16_t a);
uint32_t dorado_br_get(const dorado_memory *mem, int membase);

/* Pipe access — `n` is the slot relative to head: 0 = most recent,
 * 1 = previous, etc. Returns 0 if pipe hasn't been touched yet. */
uint32_t dorado_pipe_va(const dorado_memory *mem, int n);

#endif
