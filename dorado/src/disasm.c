#include "disasm.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * Dorado microinstruction disassembly. See include/disasm.h for the
 * field layout and the model-1 storage shuffle.
 *
 * Decode tables are from the Hardware Manual (Sep 1981):
 *   Table 7  — BSEL                              §3.5
 *   Table 8a — ASEL when FF "ok"                 §3.6
 *   Table 8b — ASEL when FF "not ok"             §3.6
 *   Table 9  — ALUFM control values              §3.7
 *   Table 10 — LC                                §3.8
 *   Tables 11a-e — FF function decodes           §3.9
 */

void dorado_decode_model1(const uint16_t s[4], dorado_uinstr *u)
{
    /*
     * Inverse of mddump.bcpl's DMachine==2 shuffle:
     *   stored[0] = (orig_iw2 & 0x8000) | (orig_iw0 >> 1)
     *   stored[1] = ((orig_iw0 & 1) << 15) | (orig_iw1 >> 1)
     *   stored[2] = ((orig_iw1 & 1) << 15) | (orig_iw2 & 0x7FFF)
     *   stored[3] = awd
     */
    uint16_t iw0 = (uint16_t)(((s[0] << 1) & 0xFFFE) | ((s[1] >> 15) & 1));
    uint16_t iw1 = (uint16_t)(((s[1] << 1) & 0xFFFE) | ((s[2] >> 15) & 1));
    uint16_t iw2 = (uint16_t)((s[0] & 0x8000) | (s[2] & 0x7FFF));

    u->iw0 = iw0;
    u->iw1 = iw1;
    u->iw2 = iw2;
    u->awd = s[3];

    /* Field extraction. C bit ordering (bit 15 = MSB). */
    u->rstk  = (uint8_t)((((iw2 >> 15) & 1) << 3) | ((iw0 >> 13) & 7));
    u->aluf  = (uint8_t)((iw0 >> 9) & 0xF);
    u->bsel  = (uint8_t)((iw0 >> 6) & 7);
    u->lc    = (uint8_t)((iw0 >> 3) & 7);
    u->asel  = (uint8_t)(iw0 & 7);
    u->block = (uint8_t)((iw1 >> 15) & 1);
    u->ff    = (uint8_t)((iw1 >> 7) & 0xFF);
    u->jcn   = (uint8_t)(((iw1 & 0x7F) << 1) | ((iw2 >> 14) & 1));
    u->brk_p = (uint8_t)((iw2 >> 12) & 3);
}

/* HM Table 7 — BSEL primary sources (when no external B source via FF). */
static const char *bsel_primary[] = {
    "Md", "RM/STK", "T", "Q",
    "0,,FF", "377,,FF", "FF,,0", "FF,,377"
};

const char *dorado_bsel_name(uint8_t bsel)
{
    return bsel_primary[bsel & 7];
}

/* HM Table 8b — ASEL when FF is not available for the memory subdecode.
 * Used when the instruction also encodes a long goto/call, a BSEL constant,
 * or an FF-controlled shift. We prefer this table for naming because it
 * always yields a single symbolic value per ASEL code (Table 8a needs FF). */
static const char *asel_simple[] = {
    "Store←RM/STK",
    "Fetch←RM/STK",
    "Store←T",
    "Fetch←T",
    "A←RM/STK",
    "A←Id",
    "A←T",
    "Shift"
};

const char *dorado_asel_name(uint8_t asel)
{
    return asel_simple[asel & 7];
}

/* HM Table 10 — LC. */
static const char *lc_names[] = {
    "NoLoad",
    "T←Pd",
    "T←Md, RM/STK←Pd",
    "T←Md",
    "RM/STK←Md",
    "T←Pd, RM/STK←Md",
    "RM/STK←Pd",
    "T←Pd, RM/STK←Pd"
};

const char *dorado_lc_name(uint8_t lc)
{
    return lc_names[lc & 7];
}

/* JCN classification (model 1).
 *
 * From mdfields.d: 8-bit JCN field is sub-encoded into one of four
 * classes by the high bits. Type tags:
 *   JC1longV   = 0   (4-bit tag)
 *   JC1localV  = 2   (2-bit tag)
 *   JC1globalV = 3   (2-bit tag)
 *   JC1fastV   = 0   (1-bit tag, conditional / fast branches)
 *
 * Fields in JCN (8 bits, MSB→LSB):
 *   local:   [JC1local:2][JN1local2thru6:5][JN1bit7:1]
 *   global:  [JC1global:2][JN1global2thru6:5][JN1bit7:1]
 *   long:    [JC1long:4][JN1long4thru6:3][JN1bit7:1]
 *   fast:    [JC1fast:1][JN1fast2thru3:2][JN1fast5thru6:2][JC1cond0thru1:2][JC1cond2:1]
 *            (= JN1bit7)
 *
 * The mutually-exclusive tag values give us:
 *   high 1 bit  = 0   → fast/conditional (JC1fast=0)  — uses bits in interleaved layout
 *   high 2 bits = 10  → local            (JC1local=2)
 *   high 2 bits = 11  → global           (JC1global=3)
 *   high 4 bits = 0xxx and not fast → long
 *
 * Tag-bit ordering: BCPL "bit 0" of JC1local is the MSB of the 8-bit JCN.
 */
const char *dorado_jcn_class(uint8_t jcn)
{
    /* Tag bits in BCPL ordering (MSB first), per mdfields.d:
     *   JC1fastV   = 0      (1-bit tag at bit 7 of our jcn byte)
     *   JC1longV   = 0      (4-bit tag at bits 7..4)
     *   JC1localV  = 2 (10) (2-bit tag at bits 7..6)
     *   JC1globalV = 3 (11) (2-bit tag at bits 7..6)
     *
     * Precedence:
     *   - bit 7 == 1  →  local (top2=10) or global (top2=11)
     *   - bit 7 == 0  →  long if bits 6..4 also zero, else fast/cond
     */
    if (((jcn >> 7) & 1) == 0)
        return ((jcn >> 4) & 0xF) == 0 ? "long" : "fast";
    return ((jcn >> 6) & 3) == 2 ? "local" : "global";
}

size_t dorado_format(const dorado_uinstr *u, char *buf, size_t buflen)
{
    int n = snprintf(buf, buflen,
        "RSTK=%02o ALUF=%02o BSEL=%-7s LC=%-18s ASEL=%-13s "
        "FF=%03o JCN=%03o(%s)%s",
        u->rstk, u->aluf,
        dorado_bsel_name(u->bsel),
        dorado_lc_name(u->lc),
        dorado_asel_name(u->asel),
        u->ff, u->jcn, dorado_jcn_class(u->jcn),
        u->block ? " BLOCK" : "");
    return (n < 0) ? 0 : (size_t)n;
}
