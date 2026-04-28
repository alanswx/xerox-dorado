#include "baseboard.h"
#include "cpu.h"
#include "disasm.h"
#include "mb.h"
#include "microcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * mctrace — load a Dorado .MB, run from a chosen entry point, and
 * print a per-cycle execution trace.
 *
 * Useful for figuring out which FF / JCN / ASEL combinations real
 * microcode actually exercises so we can prioritize implementation
 * effort. Stops at the first halt (a feature), or after N cycles.
 *
 * Usage:
 *   mctrace [-n N] [-e SYMBOL] [-p ADDR] file.MB
 *     -n N        cycle budget (default 100)
 *     -e SYMBOL   start at the real address of this symbol
 *     -p ADDR     start at this real address (octal)
 *
 * Default entry: image-0's real address (typically the entry point
 * label of the microprogram, e.g. BOOTSTRAP for Bootstrap.MB).
 */

static void usage(const char *prog)
{
    fprintf(stderr,
            "usage: %s [-n N] [-e SYMBOL] [-p ADDR_OCTAL] file.MB\n"
            "  -n N          cycle budget (default 100)\n"
            "  -e SYMBOL     start at the real address of this symbol\n"
            "  -p ADDR       start at this octal real address\n",
            prog);
}

static int find_real_for_symbol(const dorado_microcode *mc, const char *sym)
{
    for (int real = 0; real < IM_SIZE; real++) {
        const char *s = dorado_microcode_symbol_at_real(mc, real);
        if (s && !strcmp(s, sym)) return real;
    }
    return -1;
}

int main(int argc, char **argv)
{
    int budget = 100;
    const char *symbol = NULL;
    int explicit_pc = -1;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "-n")) {
            if (++i >= argc) { usage(argv[0]); return 2; }
            budget = atoi(argv[i]);
        } else if (!strcmp(a, "-e")) {
            if (++i >= argc) { usage(argv[0]); return 2; }
            symbol = argv[i];
        } else if (!strcmp(a, "-p")) {
            if (++i >= argc) { usage(argv[0]); return 2; }
            explicit_pc = (int)strtol(argv[i], NULL, 8);
        } else if (a[0] == '-') {
            usage(argv[0]); return 2;
        } else if (!path) {
            path = a;
        } else {
            usage(argv[0]); return 2;
        }
    }
    if (!path) { usage(argv[0]); return 2; }

    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        fprintf(stderr, "%s: load failed\n", path);
        return 1;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mb, &mc) != DM_OK) {
        fprintf(stderr, "%s: microcode_load failed\n", path);
        mb_free(&mb);
        return 1;
    }

    int start_pc = explicit_pc;
    if (start_pc < 0 && symbol) {
        start_pc = find_real_for_symbol(&mc, symbol);
        if (start_pc < 0) {
            fprintf(stderr, "%s: symbol '%s' not found\n", path, symbol);
            mb_free(&mb);
            return 1;
        }
    }
    if (start_pc < 0) {
        /* Image 0 → real entry. */
        start_pc = mc.image_present[0] ? mc.image_to_real[0] : 0;
    }

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)start_pc);
    dorado_cpu_trace(&cpu, stdout);

    printf("# %s, start PC=0o%o (%s), budget=%d cycles\n",
           path, start_pc,
           dorado_microcode_symbol_at_real(&mc, start_pc) ?: "(no label)",
           budget);

    cpu_halt_reason r = dorado_cpu_run(&cpu, budget);
    printf("# halted after %d cycles: %s\n", cpu.cycles,
           cpu_halt_reason_str(r));

    if (r == CPU_HALT_UNSUPPORTED_ASEL ||
        r == CPU_HALT_UNSUPPORTED_BSEL ||
        r == CPU_HALT_UNSUPPORTED_FF ||
        r == CPU_HALT_UNSUPPORTED_JCN ||
        r == CPU_HALT_NO_CODE) {
        printf("# halted at PC=0o%o\n", cpu.real_PC);
        if (cpu.real_PC < IM_SIZE && mc.im_present[cpu.real_PC]) {
            char dis[256];
            dorado_format(&mc.im[cpu.real_PC], dis, sizeof dis);
            printf("# offending uinstr: %s\n", dis);
        }
    }

    mb_free(&mb);
    return 0;
}
