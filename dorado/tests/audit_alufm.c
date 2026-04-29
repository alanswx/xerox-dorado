/*
 * audit_alufm — scan every loadable .MB file's ALUFM data and
 * report the (carry, op) pairs it contains. Cross-reference
 * against alu_op's switch in cpu.c to find uncovered ops.
 *
 * A.4 from docs/boot-bringup-plan.md: "audit `.MB` for ALUFM data
 * words and prints the histogram of (carry, op) pairs we've never
 * seen."
 */

#include "mb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The ops alu_op() in src/cpu.c handles, indexed by 5-bit op value
 * (low 5 bits of the 6-bit ALUFM entry). */
static const int handled[32] = {
    /* 00 */ 1,  /* A (arith) */
    /* 01 */ 1,  /* NOT A */
    /* 02 */ 0,
    /* 03 */ 1,  /* NOT A OR NOT B */
    /* 04 */ 0,
    /* 05 */ 1,  /* NOT A OR B */
    /* 06 */ 1,  /* 2A (arith) */
    /* 07 */ 1,  /* all-ones */
    /* 10 */ 0,
    /* 11 */ 1,  /* NOT A AND NOT B */
    /* 12 */ 0,
    /* 13 */ 1,  /* NOT B */
    /* 14 */ 1,  /* A+B (arith) */
    /* 15 */ 1,  /* A XNOR B */
    /* 16 */ 0,
    /* 17 */ 1,  /* A OR NOT B */
    /* 20 */ 0,
    /* 21 */ 1,  /* NOT A AND B */
    /* 22 */ 1,  /* A-B-1 (arith) */
    /* 23 */ 1,  /* A XOR B */
    /* 24 */ 0,
    /* 25 */ 1,  /* B */
    /* 26 */ 0,
    /* 27 */ 1,  /* A OR B */
    /* 30 */ 0,
    /* 31 */ 1,  /* all-zeros */
    /* 32 */ 0,
    /* 33 */ 1,  /* A AND NOT B */
    /* 34 */ 0,
    /* 35 */ 1,  /* A AND B */
    /* 36 */ 1,  /* A-1 (arith) */
    /* 37 */ 1,  /* A */
};

static int seen[64];          /* (carry << 5) | op */
static int seen_in_file[64];

static void scan_one(const char *path)
{
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        printf("SKIP %s (load failed)\n", path);
        mb_free(&mb);
        return;
    }
    if (mb.alufm_id < 0) {
        mb_free(&mb);
        return;
    }
    memset(seen_in_file, 0, sizeof seen_in_file);
    const mb_memory *m = &mb.mems[mb.alufm_id];
    int n_present = 0, n_unhandled = 0;
    for (int a = 0; a < m->max_addr; a++) {
        if (!m->present[a]) continue;
        n_present++;
        uint8_t v = (uint8_t)((m->data[(size_t)a * m->width_words] >> 8) & 0x3F);
        seen[v]++;
        seen_in_file[v]++;
        if (!handled[v & 0x1F]) {
            n_unhandled++;
            printf("  %s ALUFM[%2d] = 0o%02o (carry=%d op=0o%02o) — UNHANDLED\n",
                   path, a, v, (v >> 5) & 1, v & 0x1F);
        }
    }
    printf("%-60s %d entries, %d unhandled\n",
           path, n_present, n_unhandled);
    mb_free(&mb);
}

int main(int argc, char **argv)
{
    const char *paths[] = {
        "../chm/dorado/Mesa.mb!3",
        "../chm/dorado/Cedar.mb!6",
        "../chm/dorado/AEmu.mb!2",
        "../chm/dorado/DSemu.mb!1",
        "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb",
        "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb",
        "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb",
        "../chm/dorado/expanded/kernel.dm!38_/kernel.mb",
        "../chm/dorado/expanded/UnBug.bfs!1_/DoradoLisp.MB",
    };
    if (argc > 1) {
        for (int i = 1; i < argc; i++) scan_one(argv[i]);
    } else {
        for (size_t i = 0; i < sizeof paths / sizeof paths[0]; i++)
            scan_one(paths[i]);
    }

    printf("\nALUFM histogram (across all scanned files):\n");
    for (int v = 0; v < 64; v++) {
        if (!seen[v]) continue;
        printf("  0o%02o (carry=%d op=0o%02o)  count=%d  %s\n",
               v, (v >> 5) & 1, v & 0x1F, seen[v],
               handled[v & 0x1F] ? "ok" : "UNHANDLED");
    }
    return 0;
}
