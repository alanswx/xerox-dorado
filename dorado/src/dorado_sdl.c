/*
 * dorado_sdl — live windowed Dorado emulator frontend (SDL2).
 *
 * Boots the machine (BaseBoard -> Bootstrap -> Initial -> Ethernet
 * software boot of the BCPL Net Executive by default) and presents the
 * Dorado display in a window, rasterizing the Alto display list from
 * memory each frame. Host keyboard input is translated to the Alto key
 * matrix and delivered to the running world once it is interactive.
 *
 *   dorado-sdl [--eb PATH] [--eftp PATH] [--quote] [--no-alto-boot]
 *              [--scale N] [--speed CYCLES]
 *
 * Controls: type normally once NetExec is up. F1 pauses/resumes the
 * emulation clock; Esc-via-window-close or Cmd/Ctrl+Q quits.
 */

#include "machine.h"
#include "display.h"

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

int main(int argc, char **argv)
{
    int scale = 1;
    uint64_t cycles_per_frame = 400000;   /* emulated cycles per redraw */
    long shots[64];                       /* frame numbers to snapshot   */
    int n_shots = 0;
    long max_shot = -1;
    const char *shot_prefix = "dorado-frame";

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--eb") && i + 1 < argc)        cfg.eth_boot_110 = argv[++i];
        else if (!strcmp(a, "--eftp") && i + 1 < argc) cfg.eftp_boot = argv[++i];
        else if (!strcmp(a, "--boot-file-number") && i + 1 < argc)
            cfg.boot_file_number = (uint16_t)strtoul(argv[++i], NULL, 8);
        else if (!strcmp(a, "--quote"))                cfg.alto_ether_quote = 1;
        else if (!strcmp(a, "--no-alto-boot"))         cfg.alto_ether_boot = 0;
        else if (!strcmp(a, "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
        else if (!strcmp(a, "--speed") && i + 1 < argc)
            cycles_per_frame = parse_u64(argv[++i], cycles_per_frame);
        else if (!strcmp(a, "--shot-prefix") && i + 1 < argc)
            shot_prefix = argv[++i];
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
            printf("usage: %s [--eb PATH] [--eftp PATH] "
                   "[--boot-file-number OCTAL] [--quote] "
                   "[--no-alto-boot] [--scale N] [--speed CYCLES]\n"
                   "          [--screenshot F1,F2,...] [--shot-prefix NAME]\n",
                   argv[0]);
            return 0;
        } else {
            fprintf(stderr, "dorado-sdl: unknown option '%s'\n", a);
            return 2;
        }
    }
    if (scale < 1) scale = 1;

    dorado_machine *m = dorado_machine_create(&cfg);
    if (!m) {
        fprintf(stderr, "dorado-sdl: failed to create machine\n");
        return 1;
    }

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
                int bit = e.button.button == SDL_BUTTON_LEFT   ? DORADO_MOUSE_LEFT
                        : e.button.button == SDL_BUTTON_MIDDLE ? DORADO_MOUSE_MIDDLE
                        : e.button.button == SDL_BUTTON_RIGHT  ? DORADO_MOUSE_RIGHT
                        : 0;
                if (e.type == SDL_MOUSEBUTTONDOWN) mouse_buttons |= bit;
                else                               mouse_buttons &= ~bit;
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
            dorado_machine_run_until(m, now + cycles_per_frame);
            if (!announced && dorado_machine_booted(m)) {
                announced = 1;
                printf("dorado-sdl: Alto/Mesa world loaded at cycle %llu\n",
                       (unsigned long long)dorado_machine_cycles(m));
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
            uint32_t *px = pixels;
            for (int y = 0; y < DORADO_DISPLAY_H; y++) {
                const uint8_t *row = fb + y * DORADO_DISPLAY_ROW_BYTES;
                uint32_t *out = px + y * DORADO_DISPLAY_W;
                for (int x = 0; x < DORADO_DISPLAY_W; x++) {
                    int bit = (row[x >> 3] >> (7 - (x & 7))) & 1;
                    out[x] = bit ? 0xFF000000u : 0xFFFFFFFFu;
                }
            }
            SDL_UpdateTexture(tex, NULL, pixels,
                              DORADO_DISPLAY_W * (int)sizeof(uint32_t));
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, tex, NULL, NULL);
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

    if (tex) SDL_DestroyTexture(tex);
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
    dorado_machine_destroy(m);
    return 0;
}
