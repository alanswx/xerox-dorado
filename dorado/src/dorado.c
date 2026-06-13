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

/* Map an ASCII char to an Alto key (+ whether shift is needed), for the
 * headless --type self-test of the keyboard path. Returns DORADO_KEY_NONE
 * if unmapped. */
static dorado_display_key char_to_key(char c, int *shift)
{
    *shift = 0;
    if (c >= 'A' && c <= 'Z') { *shift = 1; c = (char)(c - 'A' + 'a'); }
    if (c >= 'a' && c <= 'z') {
        static const dorado_display_key L[26] = {
            DORADO_KEY_A, DORADO_KEY_B, DORADO_KEY_C, DORADO_KEY_D,
            DORADO_KEY_E, DORADO_KEY_F, DORADO_KEY_G, DORADO_KEY_H,
            DORADO_KEY_I, DORADO_KEY_J, DORADO_KEY_K, DORADO_KEY_L,
            DORADO_KEY_M, DORADO_KEY_N, DORADO_KEY_O, DORADO_KEY_P,
            DORADO_KEY_Q, DORADO_KEY_R, DORADO_KEY_S, DORADO_KEY_T,
            DORADO_KEY_U, DORADO_KEY_V, DORADO_KEY_W, DORADO_KEY_X,
            DORADO_KEY_Y, DORADO_KEY_Z };
        return L[c - 'a'];
    }
    switch (c) {
    case '0': return DORADO_KEY_0;  case '1': return DORADO_KEY_1;
    case '2': return DORADO_KEY_2;  case '3': return DORADO_KEY_3;
    case '4': return DORADO_KEY_4;  case '5': return DORADO_KEY_5;
    case '6': return DORADO_KEY_6;  case '7': return DORADO_KEY_7;
    case '8': return DORADO_KEY_8;  case '9': return DORADO_KEY_9;
    case ' ':  return DORADO_KEY_SPACE;
    case '\n': case '\r': return DORADO_KEY_RETURN;
    default: return DORADO_KEY_NONE;
    }
}

int main(int argc, char **argv)
{
    uint64_t cycles = 130000000ull;   /* ~just before the BitBlt page-zero
                                       * crash; NetExec is up with its
                                       * banner display list built. */
    const char *out = "dorado-screen.pgm";
    int progress = 0;
    const char *type_str = NULL;     /* keyboard self-test input */
    uint64_t key_hold = 600000;      /* cycles to hold each key down/up */

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
        } else if (!strcmp(a, "--boot-file-number") && i + 1 < argc) {
            cfg.boot_file_number = (uint16_t)strtoul(argv[++i], NULL, 8);
        } else if (!strcmp(a, "--boot-dir") && i + 1 < argc) {
            if (cfg.boot_dir_count <
                (int)(sizeof cfg.boot_dir / sizeof cfg.boot_dir[0]))
                cfg.boot_dir[cfg.boot_dir_count++] = argv[++i];
            else { fprintf(stderr, "dorado: too many --boot-dir\n"); i++; }
        } else if (!strcmp(a, "--out") && i + 1 < argc) {
            out = argv[++i];
        } else if (!strcmp(a, "--quote")) {
            cfg.alto_ether_quote = 1;
        } else if (!strcmp(a, "--no-alto-boot")) {
            cfg.alto_ether_boot = 0;
        } else if (!strcmp(a, "--progress")) {
            progress = 1;
        } else if (!strcmp(a, "--type") && i + 1 < argc) {
            type_str = argv[++i];
        } else if (!strcmp(a, "--key-hold") && i + 1 < argc) {
            key_hold = parse_u64(argv[++i], key_hold);
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            printf("usage: %s [--cycles N] [--eb PATH] [--eftp PATH] "
                   "[--boot-file-number OCTAL] [--boot-dir NAME=BFN=PATH] "
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
    int typed = 0;
    while (dorado_machine_cycles(m) < cycles) {
        uint64_t target = dorado_machine_cycles(m) + frame;
        if (target > cycles) target = cycles;
        uint64_t now = dorado_machine_run_until(m, target);
        if (!announced_boot && dorado_machine_booted(m)) {
            announced_boot = 1;
            printf("dorado: Alto/Mesa world loaded at cycle %llu\n",
                   (unsigned long long)now);
        }
        /* Keyboard self-test: once NetExec is interactive, settle then
         * "type" the string, holding each key down/up for --key-hold
         * cycles so its command loop registers the keystroke. */
        if (type_str && !typed && dorado_machine_interactive(m) &&
            dorado_machine_cycles(m) >= 110000000ull) {
            typed = 1;
            printf("dorado: typing \"%s\" at cyc %llu\n", type_str,
                   (unsigned long long)dorado_machine_cycles(m));
            int nk = 0;
            for (const char *p = type_str; *p; p++) {
                int shift = 0;
                dorado_display_key k = char_to_key(*p, &shift);
                if (k == DORADO_KEY_NONE) continue;
                if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 1);
                dorado_machine_set_key(m, k, 1);
                dorado_machine_run_until(m,
                    dorado_machine_cycles(m) + key_hold);
                dorado_machine_set_key(m, k, 0);
                if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 0);
                dorado_machine_run_until(m,
                    dorado_machine_cycles(m) + key_hold);
                /* Sustained-typing stress: idle a while after every 5
                 * keys (batches), so the run spans well past the old
                 * ~124M crash point with quiet gaps between bursts. */
                if (++nk % 5 == 0) {
                    printf("dorado: typed %d keys @cyc %llu\n", nk,
                           (unsigned long long)dorado_machine_cycles(m));
                    dorado_machine_run_until(m,
                        dorado_machine_cycles(m) + 3000000ull);
                }
            }
            printf("dorado: typed %d keys total, last @cyc %llu\n", nk,
                   (unsigned long long)dorado_machine_cycles(m));
        }
        if (progress) {
            dorado_machine_debug(m);
            fflush(stderr);
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
