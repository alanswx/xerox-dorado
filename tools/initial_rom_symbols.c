/* Static decoder only: no processor execution or saved machine state. */
#include "microcode.h"
#include <stdio.h>

static mb_file mb;
static dorado_microcode mc;

int main(int argc, char **argv)
{
    if (argc != 2) return 2;
    mb_init(&mb);
    if (mb_load(&mb, argv[1]) || dorado_microcode_load(&mb, &mc)) return 1;
    for (mb_symbol *s = mb.symbols; s; s = s->next) {
        if (s->mem_id == mb.im_id)
            printf("SYM %s %x %x\n", s->name, s->addr, mc.image_to_real[s->addr]);
        else if (s->mem_id == mb.rm_id)
            printf("RM %s %x\n", s->name, s->addr);
    }
    for (int a = 0; a < IM_SIZE; ++a) if (mc.im_present[a]) {
        dorado_uinstr *u = &mc.im[a];
        printf("IM %x %x %x %x %x %x %x %x %x\n", a, u->rstk, u->aluf,
               u->bsel, u->lc, u->asel, u->block, u->ff, u->jcn);
    }
    mb_free(&mb);
    return 0;
}
