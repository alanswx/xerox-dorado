// uinstr_pack.h -- a decoded microinstruction back into the three IM words the RTL holds
#pragma once
static void pack_uinstr_words(dorado_uinstr *u) {
    u->iw0 = (uint16_t)(((u->rstk & 7) << 13) |
                        ((u->aluf & 0xF) << 9) |
                        ((u->bsel & 7) << 6) |
                        ((u->lc & 7) << 3) |
                        (u->asel & 7));
    u->iw1 = (uint16_t)(((u->block & 1) << 15) |
                        ((u->ff & 0xFF) << 7) |
                        ((u->jcn >> 1) & 0x7F));
    u->iw2 = (uint16_t)((((u->rstk >> 3) & 1) << 15) |
                        ((u->jcn & 1) << 14) |
                        ((u->brk_p & 3) << 12));
}
