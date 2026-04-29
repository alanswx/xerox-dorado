#ifndef DORADO_MICROCODE_H
#define DORADO_MICROCODE_H

#include <stddef.h>
#include <stdint.h>

#include "disasm.h"
#include "mb.h"

/*
 * Hardware-ready microcode image.
 *
 * mb_file gives us the on-disk view of a .MB file: instructions in
 * IMAGE order (the source declaration order), with real placement
 * addresses encoded in the per-entry "awd" word.
 *
 * dorado_microcode is the in-memory form ready for execution: each
 * memory is indexed by hardware address (REAL for IM), with the
 * image→real permutation captured separately for symbol lookup.
 *
 * Decoding of awd, per mddump.bcpl and mddecl.d (W0word field):
 *   bits 11..0   IM.W0    (assigned absolute address — the "real" addr)
 *   bit 12       emulator (this is an emulator-task instruction)
 *   bits 14..15  brkP     (breakpoint pair — both bits set together)
 */

#define IM_SIZE     4096
#define IFUM_SIZE   1024
#define ALUFM_SIZE  16
#define RM_SIZE     256
#define STK_SIZE    256

typedef struct {
    /* Hardware IM, indexed by real address 0..IM_SIZE-1. */
    dorado_uinstr   im[IM_SIZE];
    uint16_t        im_awd[IM_SIZE];
    uint8_t         im_present[IM_SIZE];
    uint8_t         im_emulator[IM_SIZE];
    uint8_t         im_breakpoint[IM_SIZE];

    /* Image→real permutation. image_to_real[image] = real placement. */
    uint16_t        image_to_real[IM_SIZE];
    uint8_t         image_present[IM_SIZE];
    int             n_instructions;

    /* IFUM: 1024 entries × 32 bits. We keep the raw two storage words
     * here; field decoding (PA, IFAD, MemB, N, etc.) happens later
     * when we wire up the IFU. */
    uint16_t        ifum_lo[IFUM_SIZE];
    uint16_t        ifum_hi[IFUM_SIZE];
    uint8_t         ifum_present[IFUM_SIZE];

    /* ALUFM: 16 × 6-bit values. */
    uint8_t         alufm[ALUFM_SIZE];
    uint8_t         alufm_present[ALUFM_SIZE];

    /* RM: 256 × 16-bit register file initial values. */
    uint16_t        rm[RM_SIZE];
    uint8_t         rm_present[RM_SIZE];

    /* Pointer back to the mb_file so callers can resolve symbols. */
    const mb_file  *mb;
} dorado_microcode;

typedef enum {
    DM_OK = 0,
    DM_ERR_NOT_DORADO_MODEL_1,
    DM_ERR_REAL_ADDR_OOR,
    DM_ERR_IMAGE_OOR,
    DM_ERR_NO_IM,
} dorado_microcode_status;

const char *dorado_microcode_status_str(dorado_microcode_status s);

/*
 * Convert a loaded mb_file into a hardware-ready microcode image. The
 * mb_file must remain valid for the lifetime of `out` — `out->mb`
 * points back to it for symbol lookups.
 */
dorado_microcode_status dorado_microcode_load(const mb_file *mb,
                                              dorado_microcode *out);

/*
 * Layer a second .MB into an already-loaded microcode image. Does
 * NOT reset existing state — instead overlays new IM/RM/ALUFM/IFUM
 * entries on top of those already present. The order matches the
 * real boot chain: Initial.mb is loaded first, then AEmu.mb (or
 * Mesa.mb / Cedar.mb / etc.) is layered on top.
 *
 * If a real address has both layers' IM entries present, the new
 * layer wins. mb_layered's mb_file pointer replaces `out->mb` for
 * symbol lookups (so symbols of the most-recently-loaded .MB win).
 */
dorado_microcode_status dorado_microcode_layer_load(const mb_file *mb_layered,
                                                    dorado_microcode *out);

/*
 * Look up a symbol by real address (i.e., the post-placement IM
 * address). Returns NULL if no symbol attaches to that real address.
 * Internally translates real → image via image_to_real and queries the
 * mb_file's symbol table.
 */
const char *dorado_microcode_symbol_at_real(const dorado_microcode *mc,
                                            int real_addr);

#endif
