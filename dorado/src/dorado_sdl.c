/*
 * dorado_sdl — live windowed Dorado emulator frontend (SDL2).
 *
 * Boots the machine (BaseBoard -> Bootstrap -> Initial -> Ethernet
 * software boot of the BCPL Net Executive by default) and presents the
 * Dorado display in a window, rasterizing the Alto display list from
 * memory each frame. Host keyboard input is translated to the Alto key
 * matrix and delivered to the running world once it is interactive.
 *
 *   dorado-sdl [--eb PATH] [--eftp PATH] [--ftp-sysout PATH] [--germ PATH]
 *              [--pilot-disk PATH] [--quote] [--no-alto-boot]
 *              [--scale N] [--speed CYCLES]
 *
 * Controls: type normally once NetExec is up. F1 pauses/resumes the
 * emulation clock; Esc-via-window-close or Cmd/Ctrl+Q quits.
 */

#include "machine.h"
#include "display.h"
#include "typetext.h"

#include <SDL.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Map an SDL keycode to a Dorado/Alto key. Returns DORADO_KEY_NONE for
 * keys we do not model. */
static dorado_display_key map_key(SDL_Keycode k)
{
    if (k >= SDLK_a && k <= SDLK_z) {
        static const dorado_display_key letters[26] = {
            DORADO_KEY_A, DORADO_KEY_B, DORADO_KEY_C, DORADO_KEY_D,
            DORADO_KEY_E, DORADO_KEY_F, DORADO_KEY_G, DORADO_KEY_H,
            DORADO_KEY_I, DORADO_KEY_J, DORADO_KEY_K, DORADO_KEY_L,
            DORADO_KEY_M, DORADO_KEY_N, DORADO_KEY_O, DORADO_KEY_P,
            DORADO_KEY_Q, DORADO_KEY_R, DORADO_KEY_S, DORADO_KEY_T,
            DORADO_KEY_U, DORADO_KEY_V, DORADO_KEY_W, DORADO_KEY_X,
            DORADO_KEY_Y, DORADO_KEY_Z,
        };
        return letters[k - SDLK_a];
    }
    switch (k) {
    case SDLK_0: return DORADO_KEY_0;
    case SDLK_1: return DORADO_KEY_1;
    case SDLK_2: return DORADO_KEY_2;
    case SDLK_3: return DORADO_KEY_3;
    case SDLK_4: return DORADO_KEY_4;
    case SDLK_5: return DORADO_KEY_5;
    case SDLK_6: return DORADO_KEY_6;
    case SDLK_7: return DORADO_KEY_7;
    case SDLK_8: return DORADO_KEY_8;
    case SDLK_9: return DORADO_KEY_9;
    case SDLK_SPACE:        return DORADO_KEY_SPACE;
    case SDLK_RETURN:       return DORADO_KEY_RETURN;
    case SDLK_KP_ENTER:     return DORADO_KEY_RETURN;
    case SDLK_BACKSPACE:    return DORADO_KEY_BS;
    case SDLK_TAB:          return DORADO_KEY_TAB;
    case SDLK_ESCAPE:       return DORADO_KEY_ESC;
    case SDLK_DELETE:       return DORADO_KEY_DEL;
    case SDLK_MINUS:        return DORADO_KEY_MINUS;
    case SDLK_EQUALS:       return DORADO_KEY_PLUS;
    case SDLK_LEFTBRACKET:  return DORADO_KEY_LBRACKET;
    case SDLK_RIGHTBRACKET: return DORADO_KEY_RBRACKET;
    case SDLK_SEMICOLON:    return DORADO_KEY_SEMICOLON;
    case SDLK_QUOTE:        return DORADO_KEY_QUOTE;
    case SDLK_COMMA:        return DORADO_KEY_COMMA;
    case SDLK_PERIOD:       return DORADO_KEY_PERIOD;
    case SDLK_SLASH:        return DORADO_KEY_FSLASH;
    case SDLK_BACKSLASH:    return DORADO_KEY_BSLASH;
    case SDLK_LSHIFT:       return DORADO_KEY_LSHIFT;
    case SDLK_RSHIFT:       return DORADO_KEY_RSHIFT;
    case SDLK_LCTRL: case SDLK_RCTRL: return DORADO_KEY_CTRL;
    default: return DORADO_KEY_NONE;
    }
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
            fprintf(stderr, "dorado-sdl: unknown boot key '%s'\n", tok);
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
        cfg->boot_keys[cfg->boot_keys_count++] = DORADO_KEY_NONE;
    } else {
        fprintf(stderr, "dorado-sdl: unknown --boot-reason '%s' "
                "(ethernet|netexec|disk)\n", r);
        return 2;
    }
    return 0;
}

/* char_to_key lives in src/typetext.c now (dorado_char_to_key) — one
 * canonical map shared with the headless CLI and the paste queue. */

typedef struct type_event {
    char *text;
    uint64_t at;
    int typed;
} type_event;

#define MAX_TYPE_EVENTS 16

static void type_text(dorado_machine *m, const char *text, uint64_t key_hold)
{
    printf("dorado-sdl: typing \"%s\" at cyc %llu\n", text,
           (unsigned long long)dorado_machine_cycles(m));
    int nk = 0;
    for (const char *p = text; *p; p++) {
        int shift = 0, ctrl = 0;
        char tc = *p;
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
        if (++nk % 5 == 0)
            dorado_machine_run_until(m,
                dorado_machine_cycles(m) + 3000000ull);
    }
    printf("dorado-sdl: typed %d keys total, last @cyc %llu\n", nk,
           (unsigned long long)dorado_machine_cycles(m));
}

int main(int argc, char **argv)
{
    int scale = 1;
    uint64_t cycles_per_frame = 400000;   /* emulated cycles per redraw */
    long shots[64];                       /* frame numbers to snapshot   */
    int n_shots = 0;
    long max_shot = -1;
    const char *shot_prefix = "dorado-frame";
    int boot_dir_all_opt = -1;            /* -1 auto, 0 off, 1 on */
    type_event type_events[MAX_TYPE_EVENTS];
    static dorado_typequeue paste_queue;  /* Cmd/Ctrl+V clipboard typing */
    int type_event_count = 0;
    int last_type_event = -1;
    int last_type_can_update = 0;
    int pending_type_at = 0;
    uint64_t key_hold = 600000;
    uint64_t type_at = 110000000ull;
    const char *snapshot_in = NULL;
    const char *snapshot_out = NULL;

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--eb") && i + 1 < argc)        cfg.eth_boot_110 = argv[++i];
        else if (!strcmp(a, "--eftp") && i + 1 < argc) cfg.eftp_boot = argv[++i];
        else if (!strcmp(a, "--ftp-sysout") && i + 1 < argc)
            cfg.ftp_sysout = argv[++i];
        else if (!strcmp(a, "--ftp-root") && i + 1 < argc)
            cfg.ftp_root = argv[++i];
        else if (!strcmp(a, "--germ") && i + 1 < argc) cfg.germ_path = argv[++i];
        else if (!strcmp(a, "--pilot-disk") && i + 1 < argc) {
            const char *spec = argv[++i];
            const char *eq = strchr(spec, '=');
            if (!eq) {
                cfg.pilot_disk_pdi[0] = spec;
            } else {
                int slot = (eq != spec) ? atoi(spec) : -1;
                if (slot >= 0 && slot < 4) cfg.pilot_disk_pdi[slot] = eq + 1;
                else fprintf(stderr, "dorado-sdl: --pilot-disk needs PATH "
                                     "or SLOT=PATH (SLOT 0..3)\n");
            }
        }
        else if (!strcmp(a, "--disk-real")) cfg.disk_real = 1;
        else if (!strcmp(a, "--disk") && i + 1 < argc) {
            const char *spec = argv[++i];
            const char *eq = strchr(spec, '=');
            int slot = (eq && eq != spec) ? atoi(spec) : -1;
            if (slot >= 0 && slot < 4) cfg.disk_pack[slot] = eq + 1;
        }
        else if (!strcmp(a, "--germ-netboot-bfn") && i + 1 < argc) {
            cfg.germ_netboot = 1;
            cfg.germ_netboot_bfn = (uint16_t)strtoul(argv[++i], NULL, 8);
        }
        else if (!strcmp(a, "--boot-file-number") && i + 1 < argc)
            cfg.boot_file_number = (uint16_t)strtoul(argv[++i], NULL, 8);
        else if (!strcmp(a, "--boot-dir") && i + 1 < argc) {
            if (cfg.boot_dir_count <
                (int)(sizeof cfg.boot_dir / sizeof cfg.boot_dir[0]))
                cfg.boot_dir[cfg.boot_dir_count++] = argv[++i];
            else { fprintf(stderr, "dorado-sdl: too many --boot-dir\n"); i++; }
        }
        else if (!strcmp(a, "--boot-dir-all"))         boot_dir_all_opt = 1;
        else if (!strcmp(a, "--no-boot-dir-all"))      boot_dir_all_opt = 0;
        else if (!strcmp(a, "--quote"))                cfg.alto_ether_quote = 1;
        else if (!strcmp(a, "--boot-keys") && i + 1 < argc) {
            if (parse_boot_keys(argv[++i], &cfg)) return 2;
        }
        else if (!strcmp(a, "--boot-reason") && i + 1 < argc) {
            if (parse_boot_reason(argv[++i], &cfg)) return 2;
        }
        else if (!strcmp(a, "--no-alto-boot"))         cfg.alto_ether_boot = 0;
        else if (!strcmp(a, "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
        else if (!strcmp(a, "--speed") && i + 1 < argc)
            cycles_per_frame = parse_u64(argv[++i], cycles_per_frame);
        else if (!strcmp(a, "--snapshot-in") && i + 1 < argc)
            snapshot_in = argv[++i];
        else if (!strcmp(a, "--snapshot-out") && i + 1 < argc)
            snapshot_out = argv[++i];
        else if (!strcmp(a, "--shot-prefix") && i + 1 < argc)
            shot_prefix = argv[++i];
        else if (!strcmp(a, "--type") && i + 1 < argc) {
            if (type_event_count >= MAX_TYPE_EVENTS) {
                fprintf(stderr, "dorado-sdl: too many --type events (max %d)\n",
                        MAX_TYPE_EVENTS);
                return 2;
            }
            char *text = decode_type_text_arg(argv[++i]);
            if (!text) {
                fprintf(stderr, "dorado-sdl: could not allocate --type text\n");
                return 2;
            }
            type_events[type_event_count] =
                (type_event){ .text = text, .at = type_at, .typed = 0 };
            last_type_event = type_event_count++;
            last_type_can_update = !pending_type_at;
            pending_type_at = 0;
        }
        else if (!strcmp(a, "--key-hold") && i + 1 < argc) {
            key_hold = parse_u64(argv[++i], key_hold);
            last_type_can_update = 0;
        }
        else if (!strcmp(a, "--type-at") && i + 1 < argc) {
            type_at = parse_u64(argv[++i], type_at);
            if (last_type_can_update && last_type_event >= 0) {
                type_events[last_type_event].at = type_at;
                last_type_can_update = 0;
            } else {
                pending_type_at = 1;
            }
        }
        else if (!strcmp(a, "--screenshot") && i + 1 < argc) {
            /* Comma-separated frame numbers, e.g. --screenshot 10,15,20.
             * A PGM is written when the frame counter hits each one. */
            char *list = argv[++i], *tok = strtok(list, ",");
            while (tok && n_shots < (int)(sizeof shots / sizeof shots[0])) {
                long f = strtol(tok, NULL, 0);
                shots[n_shots++] = f;
                if (f > max_shot) max_shot = f;
                tok = strtok(NULL, ",");
            }
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            printf("usage: %s [--eb PATH] [--eftp PATH] [--ftp-sysout PATH] [--ftp-root DIR] "
                   "[--germ PATH] [--pilot-disk PATH] "
                   "[--germ-netboot-bfn OCTAL] "
                   "[--boot-file-number OCTAL] [--boot-dir NAME=BFN=PATH] "
                   "[--boot-dir-all] [--no-boot-dir-all] "
                   "[--quote] [--boot-keys K[,K...]] "
                   "[--boot-reason ethernet|netexec|disk] "
                   "[--no-alto-boot] [--scale N] [--speed CYCLES]\n"
                   "          [--snapshot-in PATH] [--snapshot-out PATH]\n"
                   "          [--type-at CYCLES --type TEXT]... "
                   "[--key-hold CYCLES]\n"
                   "          [--screenshot F1,F2,...] [--shot-prefix NAME]\n",
                   argv[0]);
            return 0;
        } else {
            fprintf(stderr, "dorado-sdl: unknown option '%s'\n", a);
            return 2;
        }
        if (strcmp(a, "--type") && strcmp(a, "--type-at")) {
            last_type_can_update = 0;
            pending_type_at = 0;
        }
    }
    if (scale < 1) scale = 1;

    /* Auto-register the games as a NetExec boot menu by default (off when an
     * explicit --boot-dir is given); --boot-dir-all/--no-boot-dir-all force
     * it. NETEXEC still boots first; games are served on demand by name. */
    cfg.boot_dir_all = (boot_dir_all_opt >= 0) ? boot_dir_all_opt
                                               : (cfg.boot_dir_count == 0);

    dorado_machine *m = dorado_machine_create(&cfg);
    if (!m) {
        fprintf(stderr, "dorado-sdl: failed to create machine\n");
        return 1;
    }
    if (snapshot_in && dorado_machine_restore(m, snapshot_in) != 0) {
        fprintf(stderr, "dorado-sdl: could not restore snapshot %s\n",
                snapshot_in);
        dorado_machine_destroy(m);
        return 1;
    }
    /* See dorado.c: host file sources are intentionally reapplied after a
     * restored machine state, allowing snapshot launches to serve Cedar's
     * release tree. */
    if (snapshot_in && (cfg.ftp_sysout || cfg.ftp_root))
        dorado_machine_set_ftp_source(m, cfg.ftp_sysout, cfg.ftp_root);
    if (snapshot_in)
        for (int s = 0; s < 4; s++)
            if (cfg.pilot_disk_pdi[s])
                (void)dorado_machine_set_pilot_disk(m, s,
                                                     cfg.pilot_disk_pdi[s]);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "dorado-sdl: SDL_Init: %s\n", SDL_GetError());
        dorado_machine_destroy(m);
        return 1;
    }

    /* Try to open a real window; if that fails (e.g. headless / dummy
     * video driver), fall back to a windowless mode that still runs the
     * machine and writes the requested --screenshot frames. */
    SDL_Window *win = SDL_CreateWindow(
        "Xerox Dorado", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DORADO_DISPLAY_W * scale, DORADO_DISPLAY_H * scale,
        SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *ren = win ? SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : NULL;
    SDL_Texture *tex = ren ? SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        DORADO_DISPLAY_W, DORADO_DISPLAY_H) : NULL;
    int headless = (!win || !ren || !tex);
    if (headless) {
        if (win && !ren) { SDL_DestroyWindow(win); win = NULL; }
        if (n_shots == 0) {
            fprintf(stderr, "dorado-sdl: no display available (%s) and no "
                    "--screenshot frames requested; nothing to do.\n",
                    SDL_GetError());
            dorado_machine_destroy(m);
            SDL_Quit();
            return 1;
        }
        fprintf(stderr, "dorado-sdl: no display; running headless to "
                "capture %d screenshot(s).\n", n_shots);
    }

    dorado_display *disp = dorado_machine_display(m);
    static uint32_t pixels[DORADO_DISPLAY_W * DORADO_DISPLAY_H];

    int running = 1, paused = 0;
    int announced = 0;
    int mouse_buttons = 0;
    long frame = 0;
    int win_w = DORADO_DISPLAY_W, win_h = DORADO_DISPLAY_H;  /* presented size */
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEMOTION:
                dorado_machine_set_mouse(m, e.motion.x / scale,
                                         e.motion.y / scale, mouse_buttons);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                /* Three-button (Red/Yellow/Blue) mouse. A real 3-button
                 * mouse maps directly; for one-button laptops a modified
                 * LEFT click substitutes:
                 *   Option/Alt + click  = Yellow (middle)
                 *   Cmd/Ctrl   + click  = Blue   (right)
                 * The substitution is decided at button-DOWN and
                 * remembered per physical button, so releasing the
                 * modifier before the mouse-up still releases the same
                 * emulated button. */
                static int down_bit[8];
                unsigned pb = e.button.button & 7;
                int bit;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    bit = e.button.button == SDL_BUTTON_LEFT   ? DORADO_MOUSE_LEFT
                        : e.button.button == SDL_BUTTON_MIDDLE ? DORADO_MOUSE_MIDDLE
                        : e.button.button == SDL_BUTTON_RIGHT  ? DORADO_MOUSE_RIGHT
                        : 0;
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        SDL_Keymod mod = SDL_GetModState();
                        if (mod & KMOD_ALT)               bit = DORADO_MOUSE_MIDDLE;
                        else if (mod & (KMOD_GUI | KMOD_CTRL)) bit = DORADO_MOUSE_RIGHT;
                    }
                    down_bit[pb] = bit;
                    mouse_buttons |= bit;
                } else {
                    bit = down_bit[pb];
                    down_bit[pb] = 0;
                    mouse_buttons &= ~bit;
                }
                dorado_machine_set_mouse(m, e.button.x / scale,
                                         e.button.y / scale, mouse_buttons);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int down = (e.type == SDL_KEYDOWN);
                SDL_Keycode k = e.key.keysym.sym;
                if (down && k == SDLK_F1 && !e.key.repeat) {
                    paused = !paused;
                    break;
                }
                if (down && (k == SDLK_q) &&
                    (e.key.keysym.mod & (KMOD_GUI | KMOD_CTRL))) {
                    running = 0;
                    break;
                }
                /* Cmd/Ctrl+V: paste the host clipboard as paced synthetic
                 * keystrokes (swallow the chord — no stray 'v'). */
                if (down && k == SDLK_v &&
                    (e.key.keysym.mod & (KMOD_GUI | KMOD_CTRL))) {
                    char *clip = SDL_GetClipboardText();
                    if (clip && *clip) {
                        printf("dorado-sdl: pasting %zu chars\n",
                               strlen(clip));
                        /* 800000 ~= the machine keyboard buffer's 3-field
                         * drain rate, so paste feeds the buffer without
                         * overrunning it and runs ~2x the old 1.6M pace. */
                        dorado_typequeue_start(&paste_queue, clip,
                                               800000ull,
                                               dorado_machine_cycles(m));
                    }
                    if (clip) SDL_free(clip);
                    break;
                }
                if (!down && k == SDLK_v && paste_queue.active)
                    break;            /* release of the swallowed chord */
                dorado_display_key dk = map_key(k);
                if (dk != DORADO_KEY_NONE)
                    dorado_machine_set_key(m, dk, down);
                break;
            }
            default: break;
            }
        }

        if (!paused) {
            uint64_t now = dorado_machine_cycles(m);
            dorado_typequeue_pump(&paste_queue, m);
            dorado_machine_run_until(m, now + cycles_per_frame);
            dorado_typequeue_pump(&paste_queue, m);
            if (!announced && dorado_machine_booted(m)) {
                announced = 1;
                printf("dorado-sdl: Alto/Mesa world loaded at cycle %llu\n",
                       (unsigned long long)dorado_machine_cycles(m));
            }
            if (dorado_machine_booted(m)) {
                for (int te = 0; te < type_event_count; te++) {
                    if (!type_events[te].typed &&
                        dorado_machine_cycles(m) >= type_events[te].at) {
                        type_events[te].typed = 1;
                        type_text(m, type_events[te].text, key_hold);
                    }
                }
            }
        }

        /* Rasterize the display list into the framebuffer. */
        dorado_machine_render_display_list(m);
        const uint8_t *fb = disp->fb;

        /* Write a snapshot if this frame was requested via --screenshot. */
        for (int s = 0; s < n_shots; s++) {
            if (shots[s] != frame) continue;
            char path[256];
            snprintf(path, sizeof path, "%s-%ld.pgm", shot_prefix, frame);
            if (dorado_display_snapshot_pgm(disp, path) == 0)
                printf("dorado-sdl: frame %ld (%llu cyc) -> %s\n", frame,
                       (unsigned long long)dorado_machine_cycles(m), path);
            else
                fprintf(stderr, "dorado-sdl: failed to write %s\n", path);
        }

        if (!headless) {
            /* Present at the active world's native raster (Alto 808x606,
             * Cedar lf 1024x808) instead of padding to the full framebuffer;
             * resize the window when the world (and thus its size) changes. */
            int aw = disp->active_w ? disp->active_w : DORADO_DISPLAY_W;
            int ah = disp->active_h ? disp->active_h : DORADO_DISPLAY_H;
            if (aw != win_w || ah != win_h) {
                SDL_SetWindowSize(win, aw * scale, ah * scale);
                win_w = aw; win_h = ah;
            }
            uint32_t *px = pixels;
            for (int y = 0; y < ah; y++) {
                const uint8_t *row = fb + y * DORADO_DISPLAY_ROW_BYTES;
                uint32_t *out = px + y * DORADO_DISPLAY_W;
                for (int x = 0; x < aw; x++) {
                    int bit = (row[x >> 3] >> (7 - (x & 7))) & 1;
                    out[x] = bit ? 0xFF000000u : 0xFFFFFFFFu;
                }
            }
            SDL_UpdateTexture(tex, NULL, pixels,
                              DORADO_DISPLAY_W * (int)sizeof(uint32_t));
            SDL_Rect src = { 0, 0, aw, ah };
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, tex, &src, NULL);
            SDL_RenderPresent(ren);

            char title[176];
            snprintf(title, sizeof title,
                     "Xerox Dorado  -  frame %ld  -  %llu cyc  -  %s%s",
                     frame, (unsigned long long)dorado_machine_cycles(m),
                     dorado_machine_interactive(m) ? "running"
                         : (dorado_machine_booted(m) ? "booting OS" : "boot"),
                     paused ? "  [paused]" : "");
            SDL_SetWindowTitle(win, title);
        }

        /* In windowless mode, stop once the last requested frame is done. */
        if (headless && frame >= max_shot) running = 0;
        frame++;
    }

    if (snapshot_out && dorado_machine_snapshot(m, snapshot_out) != 0)
        fprintf(stderr, "dorado-sdl: could not save snapshot %s\n",
                snapshot_out);

    if (tex) SDL_DestroyTexture(tex);
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
    dorado_machine_destroy(m);
    for (int te = 0; te < type_event_count; te++) free(type_events[te].text);
    return 0;
}
