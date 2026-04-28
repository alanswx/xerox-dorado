#include "microcode.h"

#include <string.h>

const char *dorado_microcode_status_str(dorado_microcode_status s)
{
    switch (s) {
    case DM_OK:                       return "ok";
    case DM_ERR_NOT_DORADO_MODEL_1:   return "only Dorado model 1 supported";
    case DM_ERR_REAL_ADDR_OOR:        return "real address out of range";
    case DM_ERR_IMAGE_OOR:            return "image address out of range";
    case DM_ERR_NO_IM:                return "no IM in file";
    }
    return "unknown error";
}

dorado_microcode_status dorado_microcode_load(const mb_file *mb,
                                              dorado_microcode *out)
{
    memset(out, 0, sizeof *out);
    out->mb = mb;

    if (mb->dmachine != 2) return DM_ERR_NOT_DORADO_MODEL_1;
    if (mb->im_id < 0)     return DM_ERR_NO_IM;

    const mb_memory *mim = &mb->mems[mb->im_id];

    /* IM: image-ordered. For each present image entry, decode and
     * place at real address from awd. */
    for (int img = 0; img < mim->max_addr; img++) {
        if (!mim->present[img]) continue;
        if (img >= IM_SIZE) return DM_ERR_IMAGE_OOR;

        const uint16_t *slot = &mim->data[(size_t)img * mim->width_words];
        uint16_t awd = slot[3];

        int real = awd & 0xFFF;
        if (real >= IM_SIZE) return DM_ERR_REAL_ADDR_OOR;

        dorado_decode_model1(slot, &out->im[real]);
        out->im_awd[real]        = awd;
        out->im_present[real]    = 1;
        out->im_emulator[real]   = (awd >> 12) & 1;
        /* brkP is encoded as two adjacent bits (both 1 when set). */
        out->im_breakpoint[real] = ((awd >> 14) & 3) ? 1 : 0;

        out->image_to_real[img]  = (uint16_t)real;
        out->image_present[img]  = 1;
        out->n_instructions++;
    }

    /* RM: each entry is one 16-bit value. */
    if (mb->rm_id >= 0) {
        const mb_memory *m = &mb->mems[mb->rm_id];
        int n = m->max_addr < RM_SIZE ? m->max_addr : RM_SIZE;
        for (int a = 0; a < n; a++) {
            if (!m->present[a]) continue;
            out->rm[a]         = m->data[(size_t)a * m->width_words];
            out->rm_present[a] = 1;
        }
    }

    /* ALUFM: each entry is one byte (6 bits used). */
    if (mb->alufm_id >= 0) {
        const mb_memory *m = &mb->mems[mb->alufm_id];
        int n = m->max_addr < ALUFM_SIZE ? m->max_addr : ALUFM_SIZE;
        for (int a = 0; a < n; a++) {
            if (!m->present[a]) continue;
            /* ALUFM storage is 1 word per entry; the 6-bit value lives
             * in the high byte (per mdlist1.bcpl: "ALUFM!i rshift 8"). */
            out->alufm[a]         = (uint8_t)((m->data[(size_t)a * m->width_words] >> 8) & 0x3F);
            out->alufm_present[a] = 1;
        }
    }

    /* IFUM: 2 words per entry, captured raw for now. */
    if (mb->ifum_id >= 0) {
        const mb_memory *m = &mb->mems[mb->ifum_id];
        int n = m->max_addr < IFUM_SIZE ? m->max_addr : IFUM_SIZE;
        for (int a = 0; a < n; a++) {
            if (!m->present[a]) continue;
            const uint16_t *p   = &m->data[(size_t)a * m->width_words];
            out->ifum_lo[a]     = p[0];
            out->ifum_hi[a]     = m->width_words >= 2 ? p[1] : 0;
            out->ifum_present[a] = 1;
        }
    }

    return DM_OK;
}

const char *dorado_microcode_symbol_at_real(const dorado_microcode *mc,
                                            int real_addr)
{
    if (real_addr < 0 || real_addr >= IM_SIZE) return NULL;
    if (!mc->im_present[real_addr])             return NULL;
    if (!mc->mb || mc->mb->im_id < 0)           return NULL;

    /* Find image such that image_to_real[image] == real_addr. */
    for (int img = 0; img < IM_SIZE; img++) {
        if (mc->image_present[img] && mc->image_to_real[img] == real_addr) {
            return mb_lookup_symbol(mc->mb, mc->mb->im_id, img);
        }
    }
    return NULL;
}
