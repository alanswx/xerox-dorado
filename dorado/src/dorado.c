/*
 * dorado — standalone Dorado emulator frontend.
 *
 * Boots the machine (BaseBoard -> Bootstrap -> Initial -> Ethernet
 * software boot of the BCPL Net Executive by default) and, at the end
 * of the run, rasterizes the Alto display list to a PGM snapshot.
 *
 * This is the first real runnable binary (everything before lived in the
 * test harness). A windowed live frontend is the next step; for now it
 * runs headless to a cycle budget and writes a framebuffer image.
 *
 *   dorado [options]
 *     --cycles N        run for N BaseBoard cycles (default 130000000)
 *     --eb PATH         boot-file 0110 netboot world (.eb)
 *     --eftp PATH       Stage-2 Alto boot file (default NETEXEC.BOOT)
 *     --out PATH        snapshot PGM path (default dorado-screen.pgm)
 *     --quote           hold the DDC "quote" boot key
 *     --no-alto-boot    do not drive the Stage-2 Alto ether boot
 *     --progress        print a cycle/boot progress line each frame
 */

#include "machine.h"
#include "display.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint64_t parse_u64(const char *s, uint64_t def)
{
    if (!s || !*s) return def;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 0);
    return (end && *end == '\0') ? (uint64_t)v : def;
}

int main(int argc, char **argv)
{
    uint64_t cycles = 130000000ull;   /* ~just before the BitBlt page-zero
                                       * crash; NetExec is up with its
                                       * banner display list built. */
    const char *out = "dorado-screen.pgm";
    int progress = 0;

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--cycles") && i + 1 < argc) {
            cycles = parse_u64(argv[++i], cycles);
        } else if (!strcmp(a, "--eb") && i + 1 < argc) {
            cfg.eth_boot_110 = argv[++i];
        } else if (!strcmp(a, "--eftp") && i + 1 < argc) {
            cfg.eftp_boot = argv[++i];
        } else if (!strcmp(a, "--out") && i + 1 < argc) {
            out = argv[++i];
        } else if (!strcmp(a, "--quote")) {
            cfg.alto_ether_quote = 1;
        } else if (!strcmp(a, "--no-alto-boot")) {
            cfg.alto_ether_boot = 0;
        } else if (!strcmp(a, "--progress")) {
            progress = 1;
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            printf("usage: %s [--cycles N] [--eb PATH] [--eftp PATH] "
                   "[--out PATH] [--quote] [--no-alto-boot] [--progress]\n",
                   argv[0]);
            return 0;
        } else {
            fprintf(stderr, "dorado: unknown option '%s' (try --help)\n", a);
            return 2;
        }
    }

    dorado_machine *m = dorado_machine_create(&cfg);
    if (!m) {
        fprintf(stderr, "dorado: failed to create machine\n");
        return 1;
    }

    printf("dorado: booting (target %llu cycles)...\n",
           (unsigned long long)cycles);

    /* Run in ~2M-cycle frames so we can report progress and, later,
     * present the framebuffer live. */
    const uint64_t frame = 2000000ull;
    int announced_boot = 0;
    while (dorado_machine_cycles(m) < cycles) {
        uint64_t target = dorado_machine_cycles(m) + frame;
        if (target > cycles) target = cycles;
        uint64_t now = dorado_machine_run_until(m, target);
        if (!announced_boot && dorado_machine_booted(m)) {
            announced_boot = 1;
            printf("dorado: Alto/Mesa world loaded at cycle %llu\n",
                   (unsigned long long)now);
        }
        if (progress) {
            printf("  cycle %llu  booted=%d\n",
                   (unsigned long long)now, dorado_machine_booted(m));
            fflush(stdout);
        }
        if (now < target) break;   /* halted */
    }

    if (progress) dorado_machine_debug(m);
    int pixels = dorado_machine_render_display_list(m);
    dorado_display *disp = dorado_machine_display(m);
    dorado_display_vblank(disp);
    if (dorado_display_snapshot_pgm(disp, out) == 0) {
        printf("dorado: %d display-list pixels; wrote %s\n", pixels, out);
    } else {
        fprintf(stderr, "dorado: failed to write %s\n", out);
    }

    dorado_machine_destroy(m);
    return 0;
}
