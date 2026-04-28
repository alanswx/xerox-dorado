#include "mb.h"
#include "disasm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * mbdis -- print a .MB file in MicroD-listing-like form.
 *
 * Stage 1 (this file): raw dump. We list every memory's contents in
 * octal, marking symbol names alongside their addresses. This is what
 * matches MicroD's "Imag/Real/W0/W1/Symbol" listing in ftest.DLS,
 * minus the symbolic field decoding (that's stage 2).
 */

static void print_octal(uint16_t v)
{
    /* MicroD listings show 6-octal-digit numbers right-justified. */
    printf("%6o", v);
}

static void dump_memory(const mb_file *mb, int mem_id, int start, int end,
                        int symbolic)
{
    if (mem_id < 0) return;
    const mb_memory *m = &mb->mems[mem_id];
    if (!m->defined) return;

    printf("\n%s [#%d] width=%d bits, %d words/entry, %d entries used:\n",
           m->name, mem_id, m->width_bits, m->width_words, m->max_addr);

    int is_im = (mem_id == mb->im_id);
    int do_decode = symbolic && is_im &&
                    mb->dmachine == 2 && m->width_words == 4;

    int hi = (end < 0 || end > m->max_addr) ? m->max_addr : end;
    for (int a = start; a < hi; a++) {
        if (!m->present[a]) continue;
        printf("  %4o:", a);
        const uint16_t *slot = &m->data[(size_t)a * m->width_words];
        for (int w = 0; w < m->width_words; w++) {
            putchar(' ');
            print_octal(slot[w]);
        }
        const char *sym = mb_lookup_symbol(mb, mem_id, a);
        if (sym) printf("   %s", sym);
        if (do_decode) {
            dorado_uinstr u;
            dorado_decode_model1(slot, &u);
            char dis[256];
            dorado_format(&u, dis, sizeof dis);
            printf("\n        %s", dis);
        }
        putchar('\n');
    }
}

static void print_summary(const mb_file *mb)
{
    printf("=== .MB summary ===\n");
    printf("DMachine inferred: %d  (%s)\n",
           mb->dmachine,
           mb->dmachine == 0 ? "D0 (Dolphin)" :
           mb->dmachine == 1 ? "Dorado model 0" :
           mb->dmachine == 2 ? "Dorado model 1" : "unknown");

    int n_defined = 0;
    for (int i = 0; i < MB_MAX_MEMS; i++)
        if (mb->mems[i].defined) n_defined++;
    printf("Memories declared: %d\n", n_defined);

    for (int i = 0; i < MB_MAX_MEMS; i++) {
        const mb_memory *m = &mb->mems[i];
        if (!m->defined) continue;
        printf("  #%d  %-8s  width=%d bits  words/entry=%d  used=%d\n",
               i, m->name, m->width_bits, m->width_words, m->max_addr);
    }

    int n_sym = 0;
    for (mb_symbol *s = mb->symbols; s; s = s->next) n_sym++;
    int n_fix = 0;
    for (mb_ext_fixup *f = mb->ext_fixups; f; f = f->next) n_fix++;
    printf("Symbols: %d   external fixups: %d\n", n_sym, n_fix);
}

static void usage(const char *prog)
{
    fprintf(stderr,
            "usage: %s [options] <file.MB>\n"
            "  -s, --summary       print only the per-memory summary\n"
            "  -m, --memory NAME   dump only the named memory (e.g. IM, RM)\n"
            "  -d, --disasm        also print symbolic decoding for IM entries\n"
            "  -y, --symbols       list every symbol (mem, addr, name)\n"
            "  -h, --help          this help\n",
            prog);
}

static void print_symbols(const mb_file *mb)
{
    /* Symbols are stored as a singly-linked list in load order; reverse
     * so output is in declaration order. */
    int n = 0;
    for (mb_symbol *s = mb->symbols; s; s = s->next) n++;
    mb_symbol **arr = calloc((size_t)n, sizeof *arr);
    int i = 0;
    for (mb_symbol *s = mb->symbols; s; s = s->next) arr[i++] = s;
    printf("=== Symbols (%d total) ===\n", n);
    for (int k = n - 1; k >= 0; k--) {
        const mb_memory *m = (arr[k]->mem_id < MB_MAX_MEMS) ?
                             &mb->mems[arr[k]->mem_id] : NULL;
        printf("  mem=%d (%s)  addr=0x%04X (%6o)  %s\n",
               arr[k]->mem_id,
               m && m->defined ? m->name : "?",
               arr[k]->addr, arr[k]->addr, arr[k]->name);
    }
    free(arr);
}

int main(int argc, char **argv)
{
    int summary_only = 0;
    int symbolic = 0;
    int list_symbols = 0;
    const char *only_mem = NULL;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "-s") || !strcmp(a, "--summary")) summary_only = 1;
        else if (!strcmp(a, "-d") || !strcmp(a, "--disasm")) symbolic = 1;
        else if (!strcmp(a, "-y") || !strcmp(a, "--symbols")) list_symbols = 1;
        else if (!strcmp(a, "-m") || !strcmp(a, "--memory")) {
            if (++i >= argc) { usage(argv[0]); return 2; }
            only_mem = argv[i];
        }
        else if (!strcmp(a, "-h") || !strcmp(a, "--help")) {
            usage(argv[0]); return 0;
        }
        else if (a[0] == '-') { usage(argv[0]); return 2; }
        else if (!path) path = a;
        else { usage(argv[0]); return 2; }
    }
    if (!path) { usage(argv[0]); return 2; }

    mb_file mb;
    mb_init(&mb);

    mb_status s = mb_load(&mb, path);
    if (s != MB_OK) {
        fprintf(stderr, "%s: load failed: %s\n", path, mb_status_str(s));
        mb_free(&mb);
        return 1;
    }

    print_summary(&mb);

    if (list_symbols) {
        print_symbols(&mb);
        mb_free(&mb);
        return 0;
    }

    if (!summary_only) {
        if (only_mem) {
            int id = mb_find_mem(&mb, only_mem);
            if (id < 0) {
                fprintf(stderr, "%s: no memory named '%s' in this file\n",
                        path, only_mem);
                mb_free(&mb);
                return 1;
            }
            dump_memory(&mb, id, 0, -1, symbolic);
        } else {
            /* Dump every defined memory in canonical order: IM, RM, IFUM, ALUFM, STK, BR, then any others. */
            int seen[MB_MAX_MEMS] = {0};
            int order[] = { mb.im_id, mb.rm_id, mb.ifum_id, mb.alufm_id, mb.stk_id, mb.br_id };
            for (size_t k = 0; k < sizeof(order)/sizeof(order[0]); k++) {
                int id = order[k];
                if (id < 0 || seen[id]) continue;
                dump_memory(&mb, id, 0, -1, symbolic);
                seen[id] = 1;
            }
            for (int i = 0; i < MB_MAX_MEMS; i++) {
                if (mb.mems[i].defined && !seen[i]) {
                    dump_memory(&mb, i, 0, -1, symbolic);
                    seen[i] = 1;
                }
            }
        }
    }

    mb_free(&mb);
    return 0;
}
