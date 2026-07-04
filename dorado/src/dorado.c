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
 *     --ftp-sysout PATH Pup FTP sysout served to Lisp RemoteVmemInit
 *     --germ PATH       Pilot germ image to plant into VM for the Cedar
 *                       germ-boot (Route B; e.g. Dorado.germ!4)
 *     --pilot-disk PATH Pilot/Cedar PDI disk image to mount as drive 0
 *     --germ-netboot-bfn OCTAL
 *                       seed the planted germ's request as Ethernet inLoad
 *     --out PATH        snapshot PGM path (default dorado-screen.pgm)
 *     --shot-prefix P   signal snapshot prefix (default dorado-signal-shot)
 *     --shot-every N    write PREFIX-CYCLE.pgm every N cycles
 *     --snapshot-in P   restore machine state from P after create
 *     --snapshot-out P  save machine state to P after the run
 *     --quote           hold the DDC "quote" boot key
 *     --no-alto-boot    do not drive the Stage-2 Alto ether boot
 *     --progress        print a cycle/boot progress line each frame
 *
 * Send SIGUSR1 to a running headless emulator to write
 * <shot-prefix>-<cycle>.pgm at the next safe frame boundary.
 * On systems that define SIGINFO, that signal does the same thing.
 */

#include "machine.h"
#include "display.h"

#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile sig_atomic_t signal_snapshot_requests;

static void request_signal_snapshot(int sig)
{
    (void)sig;
    signal_snapshot_requests++;
}

static void install_signal_snapshot_handlers(void)
{
    signal(SIGUSR1, request_signal_snapshot);
#ifdef SIGINFO
    signal(SIGINFO, request_signal_snapshot);
#endif
}

static uint64_t parse_u64(const char *s, uint64_t def)
{
    if (!s || !*s) return def;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 0);
    return (end && *end == '\0') ? (uint64_t)v : def;
}

static char *decode_type_text_arg(const char *s)
{
    size_t n = strlen(s ? s : "");
    char *out = malloc(n + 1);
    if (!out) return NULL;
    char *d = out;
    for (size_t i = 0; i < n; i++) {
        if (s[i] != '\\' || i + 1 >= n) {
            *d++ = s[i];
            continue;
        }
        char c = s[++i];
        switch (c) {
        case 'n': *d++ = '\n'; break;
        case 'r': *d++ = '\r'; break;
        case 't': *d++ = '\t'; break;
        case '\\': *d++ = '\\'; break;
        case 'x': {
            /* \xNN — two hex digits, for control characters (e.g. \x04 =
             * Control-D, Interlisp RAID/break flush-to-top-level). */
            int hi = (i + 1 < n) ? (unsigned char)s[i + 1] : 0;
            int lo = (i + 2 < n) ? (unsigned char)s[i + 2] : 0;
            int hv = (hi >= '0' && hi <= '9') ? hi - '0'
                   : (hi >= 'a' && hi <= 'f') ? hi - 'a' + 10
                   : (hi >= 'A' && hi <= 'F') ? hi - 'A' + 10 : -1;
            int lv = (lo >= '0' && lo <= '9') ? lo - '0'
                   : (lo >= 'a' && lo <= 'f') ? lo - 'a' + 10
                   : (lo >= 'A' && lo <= 'F') ? lo - 'A' + 10 : -1;
            if (hv >= 0 && lv >= 0) {
                *d++ = (char)((hv << 4) | lv);
                i += 2;
            } else {
                *d++ = '\\';
                *d++ = 'x';
            }
            break;
        }
        default:
            *d++ = '\\';
            *d++ = c;
            break;
        }
    }
    *d = '\0';
    return out;
}

/* Parse a comma-separated boot-key chord (e.g. "bs" or "bs,quote") into
 * cfg->boot_keys, replacing any prior chord. Returns 0 on success. */
static int parse_boot_keys(const char *list, dorado_machine_config *cfg)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s", list ? list : "");
    cfg->boot_keys_count = 0;
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        dorado_display_key k = dorado_display_key_from_name(tok);
        if (k == DORADO_KEY_NONE) {
            fprintf(stderr, "dorado: unknown boot key '%s'\n", tok);
            return 2;
        }
        if (cfg->boot_keys_count <
            (int)(sizeof cfg->boot_keys / sizeof cfg->boot_keys[0]))
            cfg->boot_keys[cfg->boot_keys_count++] = k;
    }
    return 0;
}

/* Map a friendly --boot-reason to the boot-key chord (Booting memo: no
 * key = disk, BS = Ethernet software boot, BS+Quote = NetExec). */
static int parse_boot_reason(const char *r, dorado_machine_config *cfg)
{
    cfg->boot_keys_count = 0;
    if (!strcmp(r, "ethernet")) {
        cfg->boot_keys[cfg->boot_keys_count++] = DORADO_KEY_BS;
    } else if (!strcmp(r, "netexec")) {
        cfg->boot_keys[cfg->boot_keys_count++] = DORADO_KEY_BS;
        cfg->boot_keys[cfg->boot_keys_count++] = DORADO_KEY_QUOTE;
    } else if (!strcmp(r, "disk")) {
        cfg->boot_keys[cfg->boot_keys_count++] = DORADO_KEY_NONE; /* no chord */
    } else {
        fprintf(stderr, "dorado: unknown --boot-reason '%s' "
                "(ethernet|netexec|disk)\n", r);
        return 2;
    }
    return 0;
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
    case '?': *shift = 1; return DORADO_KEY_FSLASH;
    case '/': return DORADO_KEY_FSLASH;
    case '.': return DORADO_KEY_PERIOD;
    case ',': return DORADO_KEY_COMMA;
    case '-': return DORADO_KEY_MINUS;
    case '=': return DORADO_KEY_PLUS;
    case '+': *shift = 1; return DORADO_KEY_PLUS;
    case '[': return DORADO_KEY_LBRACKET;
    case '{': *shift = 1; return DORADO_KEY_LBRACKET;
    case ']': return DORADO_KEY_RBRACKET;
    case '}': *shift = 1; return DORADO_KEY_RBRACKET;
    case ';': return DORADO_KEY_SEMICOLON;
    case ':': *shift = 1; return DORADO_KEY_SEMICOLON;
    case '\'': return DORADO_KEY_QUOTE;
    case '"': *shift = 1; return DORADO_KEY_QUOTE;
    case '\\': return DORADO_KEY_BSLASH;
    case '|': *shift = 1; return DORADO_KEY_BSLASH;
    default: return DORADO_KEY_NONE;
    }
}

typedef struct type_event {
    const char *text;
    uint64_t at;
    int typed;
} type_event;

typedef struct key_chord_event {
    char spec[256];
    uint64_t at;
    int typed;
} key_chord_event;

#define MAX_TYPE_EVENTS 16
#define MAX_KEY_CHORD_EVENTS 16

static void type_text(dorado_machine *m, const char *text, uint64_t key_hold)
{
    printf("dorado: typing \"%s\" at cyc %llu\n", text,
           (unsigned long long)dorado_machine_cycles(m));
    int nk = 0;
    for (const char *p = text; *p; p++) {
        int shift = 0, ctrl = 0;
        char tc = *p;
        /* ASCII control codes (except CR/LF/TAB) become CTRL+letter
         * chords: \x0E = Ctrl-N (e.g. Interlisp RAID's exit-with-NIL). */
        if (tc > 0 && tc <= 0x1A && tc != '\n' && tc != '\r' && tc != '\t') {
            ctrl = 1;
            tc = (char)(tc - 1 + 'a');
        }
        dorado_display_key k = char_to_key(tc, &shift);
        if (k == DORADO_KEY_NONE) continue;
        if (ctrl) dorado_machine_set_key(m, DORADO_KEY_CTRL, 1);
        if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 1);
        dorado_machine_set_key(m, k, 1);
        dorado_machine_run_until(m, dorado_machine_cycles(m) + key_hold);
        dorado_machine_set_key(m, k, 0);
        if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 0);
        if (ctrl) dorado_machine_set_key(m, DORADO_KEY_CTRL, 0);
        dorado_machine_run_until(m, dorado_machine_cycles(m) + key_hold);
        /* Sustained-typing stress: idle a while after every 5 keys
         * (batches), so long scripts still leave quiet gaps between
         * command bursts. */
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

static int parse_key_chord_spec(const char *spec, dorado_display_key *keys,
                                int *nkeys)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s", spec ? spec : "");
    *nkeys = 0;
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        dorado_display_key k = dorado_display_key_from_name(tok);
        if (k == DORADO_KEY_NONE) {
            fprintf(stderr, "dorado: unknown key '%s'\n", tok);
            return 2;
        }
        if (*nkeys >= 8) {
            fprintf(stderr, "dorado: too many keys in chord '%s'\n", spec);
            return 2;
        }
        keys[(*nkeys)++] = k;
    }
    return 0;
}

static int type_key_chord(dorado_machine *m, const char *spec,
                          uint64_t key_hold)
{
    dorado_display_key keys[8];
    int nkeys = 0;
    if (parse_key_chord_spec(spec, keys, &nkeys)) return 2;
    printf("dorado: typing key chord \"%s\" at cyc %llu\n", spec,
           (unsigned long long)dorado_machine_cycles(m));
    for (int i = 0; i < nkeys; i++) dorado_machine_set_key(m, keys[i], 1);
    dorado_machine_run_until(m, dorado_machine_cycles(m) + key_hold);
    for (int i = nkeys - 1; i >= 0; i--) dorado_machine_set_key(m, keys[i], 0);
    dorado_machine_run_until(m, dorado_machine_cycles(m) + key_hold);
    printf("dorado: typed key chord \"%s\", last @cyc %llu\n", spec,
           (unsigned long long)dorado_machine_cycles(m));
    return 0;
}

static void write_snapshot(dorado_machine *m, const char *prefix,
                           const char *reason)
{
    uint64_t cyc = dorado_machine_cycles(m);
    dorado_machine_render_display_list(m);
    dorado_display *disp = dorado_machine_display(m);

    char path[512];
    int n = snprintf(path, sizeof path, "%s-%llu.pgm",
                     prefix ? prefix : "dorado-signal-shot",
                     (unsigned long long)cyc);
    if (n < 0 || (size_t)n >= sizeof path) {
        fprintf(stderr, "dorado: screenshot path too long\n");
        return;
    }
    if (dorado_display_snapshot_pgm(disp, path) == 0) {
        printf("dorado: %s screenshot at cycle %llu -> %s\n",
               reason ? reason : "periodic", (unsigned long long)cyc, path);
        fflush(stdout);
    } else {
        fprintf(stderr, "dorado: failed to write screenshot %s\n", path);
    }
}

int main(int argc, char **argv)
{
    uint64_t cycles = 130000000ull;   /* ~just before the BitBlt page-zero
                                       * crash; NetExec is up with its
                                       * banner display list built. */
    const char *out = "dorado-screen.pgm";
    const char *shot_prefix = "dorado-signal-shot";
    const char *snapshot_in = NULL;
    const char *snapshot_out = NULL;
    uint64_t shot_every = 0;
    uint64_t next_shot = 0;
    int progress = 0;
    type_event type_events[MAX_TYPE_EVENTS];
    int type_event_count = 0;
    key_chord_event key_chord_events[MAX_KEY_CHORD_EVENTS];
    int key_chord_event_count = 0;
    int last_type_event = -1;
    int last_type_can_update = 0;
    int pending_type_at = 0;
    uint64_t key_hold = 600000;      /* cycles to hold each key down/up */
    uint64_t type_at = 110000000ull; /* cycle to begin typing (Alto default;
                                      * Cedar login prompt is ~650M) */
    int boot_dir_all_opt = -1;       /* -1 auto, 0 off, 1 on */

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
        } else if (!strcmp(a, "--ftp-sysout") && i + 1 < argc) {
            cfg.ftp_sysout = argv[++i];
        } else if (!strcmp(a, "--germ") && i + 1 < argc) {
            cfg.germ_path = argv[++i];
        } else if (!strcmp(a, "--pilot-disk") && i + 1 < argc) {
            cfg.pilot_disk_pdi = argv[++i];
        } else if (!strcmp(a, "--disk-real")) {
            cfg.disk_real = 1;
        } else if (!strcmp(a, "--disk") && i + 1 < argc) {
            /* --disk SLOT=PATH : mount a real Trident pack (T-80/T-300) R/W. */
            const char *spec = argv[++i];
            const char *eq = strchr(spec, '=');
            int slot = (eq && eq != spec) ? atoi(spec) : -1;
            if (slot >= 0 && slot < 4) cfg.disk_pack[slot] = eq + 1;
            else fprintf(stderr, "dorado: --disk needs SLOT=PATH (SLOT 0..3)\n");
        } else if (!strcmp(a, "--germ-netboot-bfn") && i + 1 < argc) {
            cfg.germ_netboot = 1;
            cfg.germ_netboot_bfn = (uint16_t)strtoul(argv[++i], NULL, 8);
        } else if (!strcmp(a, "--boot-file-number") && i + 1 < argc) {
            cfg.boot_file_number = (uint16_t)strtoul(argv[++i], NULL, 8);
        } else if (!strcmp(a, "--boot-dir") && i + 1 < argc) {
            if (cfg.boot_dir_count <
                (int)(sizeof cfg.boot_dir / sizeof cfg.boot_dir[0]))
                cfg.boot_dir[cfg.boot_dir_count++] = argv[++i];
            else { fprintf(stderr, "dorado: too many --boot-dir\n"); i++; }
        } else if (!strcmp(a, "--boot-dir-all")) {
            boot_dir_all_opt = 1;
        } else if (!strcmp(a, "--no-boot-dir-all")) {
            boot_dir_all_opt = 0;
        } else if (!strcmp(a, "--out") && i + 1 < argc) {
            out = argv[++i];
        } else if (!strcmp(a, "--shot-prefix") && i + 1 < argc) {
            shot_prefix = argv[++i];
        } else if (!strcmp(a, "--shot-every") && i + 1 < argc) {
            shot_every = parse_u64(argv[++i], 0);
        } else if (!strcmp(a, "--snapshot-in") && i + 1 < argc) {
            snapshot_in = argv[++i];
        } else if (!strcmp(a, "--snapshot-out") && i + 1 < argc) {
            snapshot_out = argv[++i];
        } else if (!strcmp(a, "--quote")) {
            cfg.alto_ether_quote = 1;
        } else if (!strcmp(a, "--boot-keys") && i + 1 < argc) {
            if (parse_boot_keys(argv[++i], &cfg)) return 2;
        } else if (!strcmp(a, "--boot-reason") && i + 1 < argc) {
            if (parse_boot_reason(argv[++i], &cfg)) return 2;
        } else if (!strcmp(a, "--no-alto-boot")) {
            cfg.alto_ether_boot = 0;
        } else if (!strcmp(a, "--progress")) {
            progress = 1;
        } else if (!strcmp(a, "--type") && i + 1 < argc) {
            if (type_event_count >= MAX_TYPE_EVENTS) {
                fprintf(stderr, "dorado: too many --type events (max %d)\n",
                        MAX_TYPE_EVENTS);
                return 2;
            }
            char *text = decode_type_text_arg(argv[++i]);
            if (!text) {
                fprintf(stderr, "dorado: could not allocate --type text\n");
                return 2;
            }
            type_events[type_event_count] =
                (type_event){ .text = text, .at = type_at, .typed = 0 };
            last_type_event = type_event_count++;
            last_type_can_update = !pending_type_at;
            pending_type_at = 0;
        } else if (!strcmp(a, "--key-chord") && i + 1 < argc) {
            if (key_chord_event_count >= MAX_KEY_CHORD_EVENTS) {
                fprintf(stderr,
                        "dorado: too many --key-chord events (max %d)\n",
                        MAX_KEY_CHORD_EVENTS);
                return 2;
            }
            snprintf(key_chord_events[key_chord_event_count].spec,
                     sizeof key_chord_events[key_chord_event_count].spec,
                     "%s", argv[++i]);
            key_chord_events[key_chord_event_count].at = type_at;
            key_chord_events[key_chord_event_count].typed = 0;
            if (parse_key_chord_spec(key_chord_events[key_chord_event_count].spec,
                                     (dorado_display_key[8]){0},
                                     &(int){0}))
                return 2;
            key_chord_event_count++;
            last_type_can_update = 0;
            pending_type_at = 0;
        } else if (!strcmp(a, "--key-hold") && i + 1 < argc) {
            key_hold = parse_u64(argv[++i], key_hold);
            last_type_can_update = 0;
        } else if (!strcmp(a, "--type-at") && i + 1 < argc) {
            type_at = parse_u64(argv[++i], type_at);
            if (last_type_can_update && last_type_event >= 0) {
                type_events[last_type_event].at = type_at;
                last_type_can_update = 0;
            } else {
                pending_type_at = 1;
            }
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            printf("usage: %s [--cycles N] [--eb PATH] [--eftp PATH] "
                   "[--ftp-sysout PATH] "
                   "[--germ PATH] [--germ-netboot-bfn OCTAL] "
                   "[--pilot-disk PATH] [--disk SLOT=PATH] [--disk-real] "
                   "[--boot-file-number OCTAL] [--boot-dir NAME=BFN=PATH] "
                   "[--boot-dir-all] [--no-boot-dir-all] "
                   "[--out PATH] [--shot-prefix PREFIX] [--shot-every N] "
                   "[--snapshot-in PATH] [--snapshot-out PATH] "
                   "[--quote] [--boot-keys K[,K...]] "
                   "[--boot-reason ethernet|netexec|disk] "
                   "[--no-alto-boot] [--progress] "
                   "[--type-at CYCLES --type TEXT]... "
                   "[--type-at CYCLES --key-chord K[,K...]]...\n"
                   "  --boot-keys: boot-selection chord held down (default "
                   "bs, +quote with --quote); e.g. bs,quote\n"
                   "  --boot-reason: alias for the chord (ethernet=bs, "
                   "netexec=bs,quote, disk=none)\n"
                   "  --disk SLOT=PATH: mount a real Trident pack (T-80/T-300) "
                   "R/W on drive SLOT (0..3)\n"
                   "  --disk-real: boot Cedar through the real disk controller "
                   "(read path) instead of the IOCB shim\n"
                   "  --shot-prefix: signal/periodic snapshot prefix "
                   "(writes PREFIX-CYCLE.pgm)\n"
                   "  --shot-every: write a headless snapshot every N cycles\n",
                   argv[0]);
            return 0;
        } else {
            fprintf(stderr, "dorado: unknown option '%s' (try --help)\n", a);
            return 2;
        }
        if (strcmp(a, "--type") && strcmp(a, "--type-at")) {
            last_type_can_update = 0;
            pending_type_at = 0;
        }
    }

    /* Auto-register all the games/utilities as a NetExec boot menu. Default
     * on whenever no explicit --boot-dir was given, so the demo just works;
     * --boot-dir-all/--no-boot-dir-all force it. The games are served only on
     * demand (by name, via Mayday); NETEXEC still boots first. */
    cfg.boot_dir_all = (boot_dir_all_opt >= 0) ? boot_dir_all_opt
                                               : (cfg.boot_dir_count == 0);

    dorado_machine *m = dorado_machine_create(&cfg);
    if (!m) {
        fprintf(stderr, "dorado: failed to create machine\n");
        return 1;
    }
    if (snapshot_in && dorado_machine_restore(m, snapshot_in) != 0) {
        dorado_machine_destroy(m);
        return 1;
    }

    printf("dorado: booting (target %llu cycles)...\n",
           (unsigned long long)cycles);
    install_signal_snapshot_handlers();
    if (shot_every) next_shot = shot_every;

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
        /* Keyboard automation: once the Alto world is interactive, type each
         * scheduled segment. Multiple segments are useful for programs that
         * intentionally ignore destructive-confirmation typeahead. */
        if (dorado_machine_booted(m)) {
            for (int te = 0; te < type_event_count; te++) {
                if (!type_events[te].typed &&
                    dorado_machine_cycles(m) >= type_events[te].at) {
                    type_events[te].typed = 1;
                    type_text(m, type_events[te].text, key_hold);
                }
            }
            for (int ke = 0; ke < key_chord_event_count; ke++) {
                if (!key_chord_events[ke].typed &&
                    dorado_machine_cycles(m) >= key_chord_events[ke].at) {
                    key_chord_events[ke].typed = 1;
                    if (type_key_chord(m, key_chord_events[ke].spec,
                                       key_hold)) {
                        dorado_machine_destroy(m);
                        return 2;
                    }
                }
            }
        }
        if (progress) {
            dorado_machine_debug(m);
            fflush(stderr);
        }
        while (signal_snapshot_requests > 0) {
            signal_snapshot_requests--;
            write_snapshot(m, shot_prefix, "signal");
        }
        if (shot_every && dorado_machine_cycles(m) >= next_shot) {
            write_snapshot(m, shot_prefix, "periodic");
            do {
                next_shot += shot_every;
            } while (dorado_machine_cycles(m) >= next_shot);
        }
        if (now < target) break;   /* halted */
    }

    if (progress || getenv("DORADO_FINAL_DEBUG")) dorado_machine_debug(m);
    if (snapshot_out && dorado_machine_snapshot(m, snapshot_out) != 0) {
        dorado_machine_destroy(m);
        return 1;
    }
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
