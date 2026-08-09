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
 *     --ftp-root DIR    read-only Cedar release tree served over Pup STP
 *     --germ PATH       Pilot germ image to plant into VM for the Cedar
 *                       germ-boot (Route B; e.g. Dorado.germ!4)
 *     --pilot-disk PATH Pilot/Cedar PDI disk image to mount as drive 0
 *     --germ-netboot-bfn OCTAL
 *                       seed the planted germ's request as Ethernet inLoad
 *     --out PATH        snapshot PGM path (default dorado-screen.pgm)
 *     --shot-prefix P   signal snapshot prefix (default dorado-signal-shot)
 *     --shot-every N    write PREFIX-CYCLE.pgm every N cycles
 *     --type-at N --mouse-delta DX,DY
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
#include "typetext.h"
#include "trace.h"
#include "io.h"
#include "dispm.h"

#include <stdint.h>
#include <signal.h>
#include <time.h>
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

/* char_to_key lives in src/typetext.c now (dorado_char_to_key). */


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

#define DRAG_MAX_WAYPOINTS 6

typedef struct click_event {
    int x, y;
    uint64_t at;
    int done;
    int button;   /* DORADO_MOUSE_LEFT/MIDDLE/RIGHT (0 = left) */
    int menu;     /* 1 = press-and-HOLD, screenshot mid-hold, then release
                   * (captures a Cedar pop-up menu, which the yellow/middle
                   * button raises while held) */
    int move_only;/* 1 = move the pointer, press nothing. Pointer MOTION is
                   * its own input path: it is what sets mouse_present and
                   * makes the machine seed the host cursor into guest cells,
                   * and it broke MesaNetExec (2026-07-30) with no button
                   * ever pressed. A gate that only clicks cannot see it. */
    int delta_only; /* 1 = x/y are terminal motion deltas, not coordinates */
    int drag_x, drag_y;
    /* Extra waypoints after (drag_x,drag_y), travelled in order with the
     * button still down. A straight line cannot express a gesture that has
     * to stay on one menu row: Interlisp opens a non-popup submenu when the
     * pointer "rolled out the right side of an item" (MENU!29 MENU.HANDLER),
     * so the path must reach the item and only then move horizontally out of
     * the menu. Interpolating a single segment crosses the neighbouring rows
     * on the way and deselects. */
    int way_x[DRAG_MAX_WAYPOINTS], way_y[DRAG_MAX_WAYPOINTS];
    int way_n;
    int drag;     /* 1 = press at (x,y), travel to (drag_x,drag_y), release.
                   * Interlisp-D tools do not open a window where you click:
                   * FileBrowser and Sketch call GETREGION, which asks you to
                   * sweep out the rectangle -- press at one corner, drag to
                   * the opposite one, release. A single click leaves them
                   * waiting with no window, no output and no error, which
                   * reads exactly like "the function did nothing". */
} click_event;

/* Driving an Iago install from the command line needs a lot of these: each
 * of Create Physical Volume, Create Logical Volume, Erase, the three
 * Installs and Create VM Backing File asks three to five questions, so the
 * volume-build recipe alone is 22 events.  The array is a few hundred bytes;
 * 16 was simply the number nobody had needed to exceed yet. */
#define MAX_TYPE_EVENTS 64
#define MAX_KEY_CHORD_EVENTS 16
#define MAX_CLICK_EVENTS 64

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
        dorado_display_key k = dorado_char_to_key(tc, &shift);
        if (k == DORADO_KEY_NONE) continue;
        if (ctrl) dorado_machine_set_key(m, DORADO_KEY_CTRL, 1);
        if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 1);
        /* LLKEY applies simultaneous matrix transitions in key-number order.
         * Let a modifier transition be observed before pressing the base key;
         * otherwise scripted '(' is decoded as '9', for example. */
        if (ctrl || shift)
            dorado_machine_run_until(m,
                dorado_machine_cycles(m) + key_hold);
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
    for (int i = 0; i < nkeys; i++) {
        dorado_machine_set_key(m, keys[i], 1);
        if (i + 1 < nkeys)
            dorado_machine_run_until(m,
                dorado_machine_cycles(m) + key_hold);
    }
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
    if (dorado_dispm_installed() != DORADO_DISPM_NONE) {
        char cpath[512];
        int px = dorado_dispm_render(dorado_machine_read_visible_word, m);
        if (px > 0 &&
            snprintf(cpath, sizeof cpath, "%s-%llu.color.ppm",
                     prefix ? prefix : "dorado-signal-shot",
                     (unsigned long long)cyc) > 0 &&
            dorado_dispm_snapshot_ppm(cpath) == 0) {
            printf("dorado: %s colour screenshot at cycle %llu -> %s\n",
                   reason ? reason : "periodic", (unsigned long long)cyc,
                   cpath);
        }
    }
}

static void trace_mouse_state(dorado_machine *m, const char *kind,
                              int x, int y)
{
    if (!dorado_trace_flag("DORADO_MOUSE_TRACE")) return;
    uint16_t gx = dorado_machine_read_visible_word(m, 0424u);
    uint16_t gy = dorado_machine_read_visible_word(m, 0425u);
    fprintf(stderr, "[mouse] %s %d,%d -> guest 0424=%06o 0425=%06o "
                    "delta=%d cyc=%llu\n", kind, x, y,
            gx, gy, dorado_machine_mouse_delta_active(m),
            (unsigned long long)dorado_machine_cycles(m));
}

/* Run to `target` while still honouring --shot-every.
 *
 * The click, drag and menu paths below drive the machine with their own
 * dorado_machine_run_until() calls, which bypassed the periodic-screenshot
 * path in the main loop.  That made the one moment worth watching
 * invisible: a pop-up menu exists only while the button is held, and a
 * latency run at --shot-every 4000000 had a 151,000,000-cycle hole in its
 * frame sequence covering the entire press.  Submenu behaviour cannot be
 * judged without frames from inside the hold. */
static void run_until_shots(dorado_machine *m, uint64_t target,
                            const char *prefix, uint64_t shot_every,
                            uint64_t *next_shot)
{
    if (!shot_every || !next_shot) {
        dorado_machine_run_until(m, target);
        return;
    }
    while (dorado_machine_cycles(m) < target) {
        uint64_t stop = (*next_shot < target) ? *next_shot : target;
        uint64_t now = dorado_machine_run_until(m, stop);
        if (dorado_machine_cycles(m) >= *next_shot) {
            write_snapshot(m, prefix, "periodic");
            do {
                *next_shot += shot_every;
            } while (dorado_machine_cycles(m) >= *next_shot);
        }
        if (now < stop) break;          /* halted */
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
    static dorado_typequeue paste_queue;  /* --paste-at: the frontends' */
    struct { const char *text; uint64_t at; int done; }
        paste_events[MAX_TYPE_EVENTS];    /* paced clipboard-typing queue */
    int paste_event_count = 0;
    key_chord_event key_chord_events[MAX_KEY_CHORD_EVENTS];
    int key_chord_event_count = 0;
    click_event click_events[MAX_CLICK_EVENTS];
    int click_event_count = 0;
    int last_type_event = -1;
    int last_type_can_update = 0;
    int pending_type_at = 0;
    uint64_t key_hold = 600000;      /* cycles to hold each key down/up */
    /* Button hold for --click/--drag/--menu. Separate from --key-hold
     * because they are different physical acts with different durations: a
     * keystroke is tens of milliseconds, a drag is a second or more, and
     * some guest interactions are sensitive to how long the button is held
     * (Interlisp's region sweep). Sharing one knob also made bisecting
     * impossible -- raising --key-hold to lengthen a drag also slowed every
     * keystroke, which moved the whole timeline and confounded the run.
     * 0 = follow --key-hold, which is the historical behaviour. */
    uint64_t drag_hold = 0;
    /* --drag-dwell: cycles to WAIT, button already down, before the
     * pointer starts travelling.  A pop-up menu does not exist the
     * instant the button goes down; without a dwell the travel is
     * over before the menu appears and the pointer can never move
     * WITHIN the menu, which is exactly what selecting a submenu
     * item requires. */
    uint64_t drag_dwell = 0;
    /* --menu-button: 0 means leave each option's own default
     * (--menu presses middle/yellow, --click and --drag press
     * left/red).  Setting it overrides BOTH, so an Interlisp
     * window menu -- which is on the RIGHT/blue button -- can be
     * opened and then dragged into. */
    int menu_button = 0;
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
        } else if (!strcmp(a, "--dispm") && i + 1 < argc) {
            const char *mode = argv[++i];
            if (!strcasecmp(mode, "none")) cfg.dispm_type = DORADO_DISPM_NONE;
            else if (!strcasecmp(mode, "standard"))
                cfg.dispm_type = DORADO_DISPM_STANDARD;
            else if (!strcasecmp(mode, "highres") || !strcasecmp(mode, "high"))
                cfg.dispm_type = DORADO_DISPM_HIGHRES;
            else {
                fprintf(stderr, "dorado: --dispm needs none, standard, or highres\n");
                return 2;
            }
        } else if (!strcmp(a, "--eftp") && i + 1 < argc) {
            cfg.eftp_boot = argv[++i];
        } else if (!strcmp(a, "--ftp-sysout") && i + 1 < argc) {
            cfg.ftp_sysout = argv[++i];
        } else if (!strcmp(a, "--ftp-root") && i + 1 < argc) {
            cfg.ftp_root = argv[++i];
        } else if (!strcmp(a, "--germ") && i + 1 < argc) {
            cfg.germ_path = argv[++i];
        } else if (!strcmp(a, "--pilot-disk") && i + 1 < argc) {
            const char *spec = argv[++i];
            const char *eq = strchr(spec, '=');
            if (!eq) {
                cfg.pilot_disk_pdi[0] = spec;
            } else {
                int slot = (eq != spec) ? atoi(spec) : -1;
                if (slot >= 0 && slot < 4) cfg.pilot_disk_pdi[slot] = eq + 1;
                else fprintf(stderr, "dorado: --pilot-disk needs PATH or "
                                     "SLOT=PATH (SLOT 0..3)\n");
            }
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
        } else if (!strcmp(a, "--boot-switches") && i + 1 < argc) {
            cfg.boot_switches = argv[++i];
        } else if (!strcmp(a, "--storage-modules") && i + 1 < argc) {
            /* 1..4 4MW storage modules.  AEmu needs only one; the Smalltalk
             * microcode is the "XM" (extended memory) build, so it is worth
             * being able to hand it more. */
            cfg.storage_modules = atoi(argv[++i]);
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
        } else if (!strcmp(a, "--click") && i + 1 < argc) {
            /* --click X,Y — press+release the left mouse button at display
             * coordinates (X,Y) at the pending --type-at cycle. Drives the
             * Interlisp-D desktop (e.g. click the Exec window for TTY
             * focus) without the SDL frontend. */
            if (click_event_count >= MAX_CLICK_EVENTS) {
                fprintf(stderr, "dorado: too many --click events (max %d)\n",
                        MAX_CLICK_EVENTS);
                return 2;
            }
            int cx = 0, cy = 0;
            if (sscanf(argv[++i], "%d,%d", &cx, &cy) != 2) {
                fprintf(stderr, "dorado: --click wants X,Y (decimal)\n");
                return 2;
            }
            click_events[click_event_count] =
                (click_event){ .x = cx, .y = cy, .at = type_at, .done = 0 };
            click_event_count++;
            last_type_can_update = 0;
            pending_type_at = 0;
        } else if (!strcmp(a, "--mouse") && i + 1 < argc) {
            /* --mouse X,Y — move the pointer to (X,Y) at the pending
             * --type-at cycle, pressing nothing. */
            if (click_event_count >= MAX_CLICK_EVENTS) {
                fprintf(stderr, "dorado: too many click/mouse events (max %d)\n",
                        MAX_CLICK_EVENTS);
                return 2;
            }
            int cx = 0, cy = 0;
            if (sscanf(argv[++i], "%d,%d", &cx, &cy) != 2) {
                fprintf(stderr, "dorado: --mouse wants X,Y (decimal)\n");
                return 2;
            }
            click_events[click_event_count] =
                (click_event){ .x = cx, .y = cy, .at = type_at, .done = 0,
                               .move_only = 1 };
            click_event_count++;
            last_type_can_update = 0;
            pending_type_at = 0;
        } else if (!strcmp(a, "--mouse-delta") && i + 1 < argc) {
            /* --mouse-delta DX,DY -- inject one terminal msg 06B motion
             * event. This is deliberately separate from --mouse, whose
             * absolute coordinates cannot exercise Cedar's two-display
             * edge-push path. */
            if (click_event_count >= MAX_CLICK_EVENTS) {
                fprintf(stderr, "dorado: too many click/mouse events (max %d)\n",
                        MAX_CLICK_EVENTS);
                return 2;
            }
            int dx = 0, dy = 0;
            if (sscanf(argv[++i], "%d,%d", &dx, &dy) != 2) {
                fprintf(stderr, "dorado: --mouse-delta wants DX,DY (decimal)\n");
                return 2;
            }
            click_events[click_event_count++] =
                (click_event){ .x = dx, .y = dy, .at = type_at, .done = 0,
                               .move_only = 1, .delta_only = 1 };
            last_type_can_update = 0;
            pending_type_at = 0;
        } else if (!strcmp(a, "--drag") && i + 1 < argc) {
            /* --drag X1,Y1,X2,Y2 — sweep out a rectangle: press at the first
             * corner, travel to the second, release. This is how Interlisp-D
             * asks for a window (GETREGION), so it is what opens FileBrowser
             * and Sketch. */
            if (click_event_count >= MAX_CLICK_EVENTS) {
                fprintf(stderr, "dorado: too many click/mouse events (max %d)\n",
                        MAX_CLICK_EVENTS);
                return 2;
            }
            /* X1,Y1,X2,Y2[,X3,Y3...] -- extra pairs are waypoints travelled
             * in order with the button still down (see click_event.way_x). */
            int pt[2 * (2 + DRAG_MAX_WAYPOINTS)];
            int npt = 0;
            for (const char *p = argv[++i]; *p && npt < (int)(sizeof pt / sizeof pt[0]);) {
                char *end = NULL;
                long v = strtol(p, &end, 10);
                if (end == p) break;
                pt[npt++] = (int)v;
                p = (*end == ',') ? end + 1 : end;
            }
            if (npt < 4 || (npt & 1)) {
                fprintf(stderr, "dorado: --drag wants X1,Y1,X2,Y2[,X3,Y3...] "
                                "(decimal, at most %d extra waypoints)\n",
                        DRAG_MAX_WAYPOINTS);
                return 2;
            }
            click_events[click_event_count] =
                (click_event){ .x = pt[0], .y = pt[1], .at = type_at, .done = 0,
                               .button = menu_button,  /* 0 -> left */
                               .drag = 1, .drag_x = pt[2], .drag_y = pt[3] };
            for (int k = 4; k + 1 < npt; k += 2) {
                click_events[click_event_count].way_x[
                    click_events[click_event_count].way_n] = pt[k];
                click_events[click_event_count].way_y[
                    click_events[click_event_count].way_n] = pt[k + 1];
                click_events[click_event_count].way_n++;
            }
            click_event_count++;
            last_type_can_update = 0;
            pending_type_at = 0;
        } else if (!strcmp(a, "--menu-button") && i + 1 < argc) {
            /* Which button subsequent --menu events press. Interlisp and
             * Cedar put menus on different buttons (red/yellow/blue =
             * left/middle/right), so a fixed choice cannot test both. */
            const char *b = argv[++i];
            if (!strcmp(b, "left") || !strcmp(b, "red"))
                menu_button = DORADO_MOUSE_LEFT;
            else if (!strcmp(b, "middle") || !strcmp(b, "yellow"))
                menu_button = DORADO_MOUSE_MIDDLE;
            else if (!strcmp(b, "right") || !strcmp(b, "blue"))
                menu_button = DORADO_MOUSE_RIGHT;
            else {
                fprintf(stderr, "dorado: --menu-button wants left|middle|right "
                                "(or red|yellow|blue)\n");
                return 2;
            }
        } else if (!strcmp(a, "--menu") && i + 1 < argc) {
            /* --menu X,Y — press+HOLD a button at (X,Y), screenshot the
             * pop-up menu while held, then release. Proves the
             * mouse->context-menu path renders. Button: --menu-button,
             * default middle (yellow). */
            if (click_event_count >= MAX_CLICK_EVENTS) {
                fprintf(stderr, "dorado: too many click/menu events (max %d)\n",
                        MAX_CLICK_EVENTS);
                return 2;
            }
            int cx = 0, cy = 0;
            if (sscanf(argv[++i], "%d,%d", &cx, &cy) != 2) {
                fprintf(stderr, "dorado: --menu wants X,Y (decimal)\n");
                return 2;
            }
            click_events[click_event_count] =
                (click_event){ .x = cx, .y = cy, .at = type_at, .done = 0,
                               .button = menu_button ? menu_button
                                                     : DORADO_MOUSE_MIDDLE,
                               .menu = 1 };
            click_event_count++;
            last_type_can_update = 0;
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
        } else if (!strcmp(a, "--drag-dwell") && i + 1 < argc) {
            drag_dwell = parse_u64(argv[++i], drag_dwell);
        } else if (!strcmp(a, "--drag-hold") && i + 1 < argc) {
            drag_hold = parse_u64(argv[++i], drag_hold);
            last_type_can_update = 0;
        } else if (!strcmp(a, "--paste-at") && i + 3 < argc &&
                   !strcmp(argv[i + 2], "--paste")) {
            /* --paste-at CYCLES --paste TEXT: exercise the frontends'
             * clipboard queue (dorado_typequeue) headlessly -- unlike
             * --type, typing is paced across the run loop's frames. */
            if (paste_event_count >= MAX_TYPE_EVENTS) {
                fprintf(stderr, "dorado: too many --paste events (max %d)\n",
                        MAX_TYPE_EVENTS);
                return 2;
            }
            paste_events[paste_event_count].at = parse_u64(argv[++i], 0);
            i++;                          /* the --paste flag */
            paste_events[paste_event_count].text =
                decode_type_text_arg(argv[++i]);
            paste_events[paste_event_count].done = 0;
            paste_event_count++;
        } else if (!strcmp(a, "--type-at") && i + 1 < argc) {
            type_at = parse_u64(argv[++i], type_at);
            if (last_type_can_update && last_type_event >= 0) {
                type_events[last_type_event].at = type_at;
                last_type_can_update = 0;
            } else {
                pending_type_at = 1;
            }
        } else if (!strcmp(a, "--print-abi")) {
            /* Report the snapshot header's struct sizes and exit. A
             * checkpoint whose header disagrees is refused by restore, so
             * verify-snapshot-abi diffs the shipped ones against this
             * rather than booting each. Native and wasm32 differ; each
             * build reports its own. */
            dorado_machine_print_abi(stdout);
            return 0;
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            printf("usage: %s [--cycles N] [--eb PATH] [--eftp PATH] "
                   "[--ftp-sysout PATH] "
                   "[--ftp-root DIR] "
                   "[--germ PATH] [--germ-netboot-bfn OCTAL] "
                   "[--pilot-disk PATH] [--disk SLOT=PATH] [--disk-real] "
                   "[--boot-file-number OCTAL] [--boot-dir NAME=BFN=PATH] "
                   "[--boot-switches LETTERS] "
                   "[--boot-dir-all] [--no-boot-dir-all] "
                   "[--out PATH] [--shot-prefix PREFIX] [--shot-every N] "
                   "[--snapshot-in PATH] [--snapshot-out PATH] "
                   "[--quote] [--boot-keys K[,K...]] "
                   "[--boot-reason ethernet|netexec|disk] "
                   "[--no-alto-boot] [--progress] "
                   "[--type-at CYCLES --type TEXT] [--mouse X,Y]... "
                   "[--type-at CYCLES --mouse-delta DX,DY]... "
                   "[--type-at CYCLES --key-chord K[,K...]]...\n"
                   "  --drag-hold N: cycles to hold the mouse button for "
                   "--click/--drag/--menu (default: --key-hold). Interlisp's "
                   "GETREGION needs a long hold\n"
                   "  --boot-keys: boot-selection chord held down (default "
                   "bs, +quote with --quote); e.g. bs,quote\n"
                   "  --boot-reason: alias for the chord (ethernet=bs, "
                   "netexec=bs,quote, disk=none)\n"
                   "  --boot-switches: Pilot boot switches for the world, as "
                   "typed at the herald's \"Switches:\" prompt\n"
                   "                   (GermSwap.Switch: 0-9 and a-z); e.g. "
                   "'l' runs Iago at boot\n"
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
    /* A snapshot includes live controller state, but command-line network
     * sources describe host-side resources rather than emulated state.  Put
     * explicitly requested sources back after restore so a fast checkpoint
     * can be paired with a new/read-only Cedar release tree. */
    if (snapshot_in && (cfg.ftp_sysout || cfg.ftp_root))
        dorado_machine_set_ftp_source(m, cfg.ftp_sysout, cfg.ftp_root);
    if (snapshot_in)
        for (int s = 0; s < 4; s++)
            if (cfg.pilot_disk_pdi[s])
                (void)dorado_machine_set_pilot_disk(m, s,
                                                     cfg.pilot_disk_pdi[s]);

    printf("dorado: booting (target %llu cycles)...\n",
           (unsigned long long)cycles);
    clock_t run_start_clock = clock();
    /* Baseline AFTER any --snapshot-in: cpu->cycles is part of the
     * snapshotted state, so a restored checkpoint arrives carrying every
     * microinstruction its bake executed. Charging those to this run's
     * wall time reported a restored Cedar desktop at "14.7x real
     * hardware". Only the delta is this run's work. */
    uint64_t run_start_uops = dorado_machine_uinstructions(m);
    uint64_t run_start_cycles = dorado_machine_cycles(m);
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
            for (int pe = 0; pe < paste_event_count; pe++) {
                if (paste_events[pe].done ||
                    dorado_machine_cycles(m) < paste_events[pe].at) continue;
                paste_events[pe].done = 1;
                printf("dorado: pasting %zu chars\n",
                       strlen(paste_events[pe].text));
                dorado_typequeue_start(&paste_queue, paste_events[pe].text,
                                       800000ull, dorado_machine_cycles(m));
            }
            dorado_typequeue_pump(&paste_queue, m);
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
            for (int ce = 0; ce < click_event_count; ce++) {
                if (!click_events[ce].done &&
                    dorado_machine_cycles(m) >= click_events[ce].at) {
                    /* Button hold: --drag-hold when given, else --key-hold. */
                    uint64_t btn_hold = drag_hold ? drag_hold : key_hold;
                    click_events[ce].done = 1;
                    int btn = click_events[ce].button
                                  ? click_events[ce].button
                                  : DORADO_MOUSE_LEFT;
                    printf("dorado: %s (%d,%d) at cyc %llu\n",
                           click_events[ce].move_only ? "mouse" :
                           click_events[ce].drag ? "drag" :
                           click_events[ce].menu ? "menu" : "click",
                           click_events[ce].x, click_events[ce].y,
                           (unsigned long long)dorado_machine_cycles(m));
                    if (click_events[ce].move_only) {
                        if (click_events[ce].delta_only) {
                            dorado_machine_mouse_delta(m, click_events[ce].x,
                                                       click_events[ce].y);
                            trace_mouse_state(m, "delta", click_events[ce].x,
                                              click_events[ce].y);
                            run_until_shots(
                                m, dorado_machine_cycles(m) + 300000ull,
                                shot_prefix, shot_every, &next_shot);
                            trace_mouse_state(m, "delta-after",
                                              click_events[ce].x,
                                              click_events[ce].y);
                            if (shot_prefix)
                                write_snapshot(m, shot_prefix, "mouse-delta");
                            continue;
                        }
                        /* Motion only: several steps, as a hand would, so
                         * the guest sees the cursor travel rather than jump. */
                        for (int step = 1; step <= 8; step++) {
                            dorado_machine_set_mouse(
                                m, click_events[ce].x * step / 8,
                                click_events[ce].y * step / 8, 0);
                            trace_mouse_state(m, "absolute",
                                              click_events[ce].x * step / 8,
                                              click_events[ce].y * step / 8);
                            run_until_shots(
                                m, dorado_machine_cycles(m) + 300000ull, shot_prefix, shot_every, &next_shot);
                        }
                        if (shot_prefix)
                            write_snapshot(m, shot_prefix, "mouse");
                        continue;
                    }
                    if (click_events[ce].drag) {
                        /* Sweep: arrive at the first corner, press, travel in
                         * steps so the rubber-band feedback tracks it the way
                         * it would under a hand, then release at the far
                         * corner. */
                        int x1 = click_events[ce].x, y1 = click_events[ce].y;
                        int x2 = click_events[ce].drag_x;
                        int y2 = click_events[ce].drag_y;
                        dorado_machine_set_mouse(m, x1, y1, 0);
                        run_until_shots(m,
                            dorado_machine_cycles(m) + 2000000ull, shot_prefix, shot_every, &next_shot);
                        dorado_machine_set_mouse(m, x1, y1, btn);
                        run_until_shots(m,
                            dorado_machine_cycles(m) + 2000000ull, shot_prefix, shot_every, &next_shot);
                        /* Let a pop-up menu actually appear before moving. */
                        if (drag_dwell)
                            run_until_shots(
                                m, dorado_machine_cycles(m) + drag_dwell, shot_prefix, shot_every, &next_shot);
                        int cx = x1, cy = y1;
                        for (int leg = 0; leg <= click_events[ce].way_n; leg++) {
                            int lx = (leg == 0) ? x2 : click_events[ce].way_x[leg - 1];
                            int ly = (leg == 0) ? y2 : click_events[ce].way_y[leg - 1];
                            for (int step = 1; step <= 12; step++) {
                                dorado_machine_set_mouse(
                                    m, cx + (lx - cx) * step / 12,
                                    cy + (ly - cy) * step / 12, btn);
                                run_until_shots(
                                    m, dorado_machine_cycles(m) + 400000ull, shot_prefix, shot_every, &next_shot);
                            }
                            cx = lx; cy = ly;
                            /* Dwell at each intermediate waypoint too: a
                             * submenu roll-out only counts if the guest
                             * sampled the pointer on the item first. */
                            if (leg < click_events[ce].way_n && drag_dwell)
                                run_until_shots(
                                    m, dorado_machine_cycles(m) + drag_dwell, shot_prefix, shot_every, &next_shot);
                        }
                        run_until_shots(m,
                            dorado_machine_cycles(m) + btn_hold, shot_prefix, shot_every, &next_shot);
                        dorado_machine_set_mouse(m, cx, cy, 0);
                        continue;
                    }
                    /* Move first so the tracking software sees the cursor
                     * arrive, then press the button. */
                    if (dorado_machine_mouse_delta_active(m))
                        dorado_machine_set_mouse_buttons(m, 0);
                    else
                        dorado_machine_set_mouse(m, click_events[ce].x,
                                                 click_events[ce].y, 0);
                    run_until_shots(m,
                        dorado_machine_cycles(m) + 2000000ull, shot_prefix, shot_every, &next_shot);
                    if (dorado_machine_mouse_delta_active(m))
                        dorado_machine_set_mouse_buttons(m, btn);
                    else
                        dorado_machine_set_mouse(m, click_events[ce].x,
                                                 click_events[ce].y, btn);
                    run_until_shots(m,
                        dorado_machine_cycles(m) + btn_hold, shot_prefix, shot_every, &next_shot);
                    if (click_events[ce].menu) {
                        /* A pop-up menu is up now, under the cursor and still
                         * held. Give it several fields to paint, capturing a
                         * shot at each so timing can't hide it, then release. */
                        for (int k = 0; k < 6; k++) {
                            run_until_shots(m,
                                dorado_machine_cycles(m) + 4000000ull, shot_prefix, shot_every, &next_shot);
                            dorado_machine_render_display_list(m);
                            write_snapshot(m, "dorado-menu", "menu");
                        }
                    }
                    if (dorado_machine_mouse_delta_active(m))
                        dorado_machine_set_mouse_buttons(m, 0);
                    else
                        dorado_machine_set_mouse(m, click_events[ce].x,
                                                 click_events[ce].y, 0);
                    run_until_shots(m,
                        dorado_machine_cycles(m) + 1000000ull, shot_prefix, shot_every, &next_shot);
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
    if (dorado_trace_flag("DORADO_DDC_TIOA"))
        dorado_display_dump_tioa_use(disp);
    if (dorado_trace_flag("DORADO_IO_CENSUS"))
        dorado_io_dump_output_census();
    if (dorado_dispm_installed() != DORADO_DISPM_NONE) {
        /* Paint the colour board from the ColorCSB chain and write it beside
         * the monochrome snapshot. A second monitor is a second image: the
         * Dorado's colour display is a separate screen (ColorDisplay left |
         * right places viewers on one side or the other), not an overlay. */
        int px = dorado_dispm_render(dorado_machine_read_visible_word, m);
        char cpath[1024];
        snprintf(cpath, sizeof cpath, "%s.color.ppm", out);
        if (px > 0 && dorado_dispm_snapshot_ppm(cpath) == 0)
            printf("dorado: DispM colour: %d px; wrote %s\n", px, cpath);
        dorado_dispm_dump();
    }
    dorado_display_vblank(disp);
    if (dorado_display_snapshot_pgm(disp, out) == 0) {
        printf("dorado: %d display-list pixels; wrote %s\n", pixels, out);
        /* Honest speed report, in the machine's own unit. The headline
         * number here is emulated-Dorado-seconds per wall second: the real
         * machine ran 16.666 M microinstructions/s (60 ns), so >1.0 means
         * this emulator outruns a real Dorado and <1.0 means it does not.
         * Quoting bb.cycles/s instead overstates it by ~3.7x, which is how
         * "1.75x the real Dorado" came to be published for something that
         * is actually slower than the hardware. */
        uint64_t uops = dorado_machine_uinstructions(m) - run_start_uops;
        double wall = (double)(clock() - run_start_clock) / CLOCKS_PER_SEC;
        if (uops && wall > 0.0) {
            double emulated_s = (double)uops / 16.666e6;
            printf("dorado: %llu microinstructions = %.2f s of Dorado time "
                   "in %.2f s CPU = %.2fx real hardware "
                   "(%.1f M uinstr/s; %llu BB-6502 cycles, %.2f per uinstr)\n",
                   (unsigned long long)uops, emulated_s, wall,
                   emulated_s / wall, (double)uops / wall / 1e6,
                   (unsigned long long)(dorado_machine_cycles(m) - run_start_cycles),
                   (double)(dorado_machine_cycles(m) - run_start_cycles) / (double)uops);
        }
    } else {
        fprintf(stderr, "dorado: failed to write %s\n", out);
    }

    dorado_machine_destroy(m);
    return 0;
}
