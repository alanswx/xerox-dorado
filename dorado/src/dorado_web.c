/*
 * dorado_web - Emscripten/WebAssembly frontend for the Dorado emulator.
 *
 * Mirrors src/dorado_sdl.c, but restructured for the browser: the blocking
 * `while (running)` loop becomes a single frame() callback driven by
 * emscripten_set_main_loop(), since a WASM main thread may not block.
 *
 * Boot files live in the Emscripten virtual filesystem (MEMFS), preloaded
 * at the same paths the machine code fopen()s:
 *   /worlds/aemu.eb          - the Alto-emulator-on-Dorado microcode world
 *   /chm/bootfiles/*.boot    - Alto B-format boot files (games + NetExec)
 *
 * The page boots NetExec by default (with every game registered as a boot
 * menu entry via boot_dir_all). The JS dropdown calls dorado_web_boot() to
 * tear down and recreate the machine pointed straight at one game's boot
 * file, or back at NetExec.
 */

#include "machine.h"
#include "display.h"

#include <SDL.h>
#include <emscripten.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Virtual-FS paths (preloaded by the `make web` emcc invocation). The
 * BaseBoard/bootstrap microcode the machine loads at create time lives under
 * /ucode; the Alto world + boot files under /worlds and /chm/bootfiles. */
#define WEB_EB_WORLD   "/worlds/aemu.eb"
#define WEB_BOOTDIR    "/chm/bootfiles"
#define WEB_NETEXEC    WEB_BOOTDIR "/NETEXEC.BOOT!8"
#define WEB_BB_ROM     "/ucode/doradobaserom.mb!13"
#define WEB_BOOTSTRAP  "/ucode/Bootstrap.mb"
#define WEB_INITIAL    "/ucode/Initial.mb"
#define WEB_KERNEL     "/ucode/kernel.mb"
#define WEB_MEMMISC    "/ucode/memMisc.mb"
#define WEB_IFU        "/ucode/IfuComplex.mb"

/* Route B - native Cedar/Pilot: the Cedar microcode world, the matched Pilot
 * germ, and the bootable Pilot/Cedar PDI disk image (preloaded by `make web`).
 * Boots to Cedar 6.1's SimpleTerminal login prompt. */
#define WEB_CEDAR_EB   "/worlds/CedarDorado.eb"
#define WEB_CEDAR_GERM "/worlds/Dorado.germ-6.1.6"
#define WEB_CEDAR_PDI  "/worlds/CedarDorado-boot.pdi"

/* Alto/Mesa world: the full Mesa VM microcode (vs aemu.eb, Alto/Nova only).
 * Boots the Mesa Network Executive (a Mesa/Pilot environment, sibling of
 * Cedar) over EFTP. */
#define WEB_MESA_EB    "/worlds/AltoMesaDorado.eb"
#define WEB_MESA_BOOT  WEB_BOOTDIR "/MesaNetExec.boot!1"

/* Single global app state - the frame() callback has no other way to reach
 * it under emscripten_set_main_loop. */
static struct {
    dorado_machine  *m;
    dorado_display  *disp;
    SDL_Window      *win;
    SDL_Renderer    *ren;
    SDL_Texture     *tex;
    int              scale;
    uint64_t         cycles_per_frame;
    int              mouse_buttons;
    int              paused;
    int              announced;
    long             frame;
} app;

static uint32_t pixels[DORADO_DISPLAY_W * DORADO_DISPLAY_H];

/* Map an SDL keycode to a Dorado/Alto key (identical table to dorado_sdl.c). */
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
    case SDLK_LCTRL:
    case SDLK_RCTRL: return DORADO_KEY_CTRL;
    default: return DORADO_KEY_NONE;
    }
}

/* (Re)create the machine booting `eftp_path`. When dir_all is set, every
 * Alto B-format file beside it is registered as a NetExec menu entry. Called
 * once at startup and again from JS each time the game dropdown changes.
 * Returns 0 on success. Exported (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot(const char *eftp_path, int dir_all)
{
    if (app.m) {
        dorado_machine_destroy(app.m);
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_EB_WORLD;
    cfg.eftp_boot    = (eftp_path && *eftp_path) ? eftp_path : WEB_NETEXEC;
    cfg.boot_dir_all = dir_all ? 1 : 0;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create machine for %s\n",
                cfg.eftp_boot);
        return 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 0;
    app.frame     = 0;
    app.cycles_per_frame = 400000;
    printf("dorado_web: booting %s (menu=%d)\n", cfg.eftp_boot, cfg.boot_dir_all);
    return 0;
}

/* (Re)create the machine as the native Cedar/Pilot world: Initial netboots the
 * Cedar microcode, the matched Pilot germ is planted, and the Pilot/Cedar PDI
 * is mounted as drive 0. Boots (no boot-key chord = disk) to Cedar 6.1's
 * SimpleTerminal login prompt -- type at the Name: prompt once it appears.
 * Exported (KEEPALIVE) so JS can ccall it. Reaching the prompt is ~640M
 * cycles, so allow a minute or two in the browser. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar(void)
{
    if (app.m) {
        dorado_machine_destroy(app.m);
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_CEDAR_EB;
    cfg.germ_path    = WEB_CEDAR_GERM;
    cfg.pilot_disk_pdi = WEB_CEDAR_PDI;
    cfg.eftp_boot    = NULL;          /* Cedar boots from the PDI, not EFTP */
    cfg.alto_ether_boot = 0;          /* native Cedar, not Alto-on-Dorado   */
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;  /* no chord = disk boot reason */
    cfg.boot_keys_count = 1;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Cedar machine\n");
        return 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 0;
    app.frame     = 0;
    app.cycles_per_frame = 400000;
    printf("dorado_web: booting Cedar 6.1 (Pilot disk)\n");
    return 0;
}

/* (Re)create the machine as the Alto/Mesa world booting the Mesa Network
 * Executive over EFTP -- a non-Cedar Mesa/Pilot environment. Exported
 * (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_mesa(void)
{
    if (app.m) {
        dorado_machine_destroy(app.m);
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_MESA_EB;
    cfg.eftp_boot    = WEB_MESA_BOOT;
    cfg.boot_dir_all = 0;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Mesa machine\n");
        return 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 0;
    app.frame     = 0;
    app.cycles_per_frame = 400000;
    printf("dorado_web: booting Mesa NetExec (AltoMesaDorado)\n");
    return 0;
}

/* One animation frame: drain input, advance the emulator, blit the display. */
static void frame(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_MOUSEMOTION:
            dorado_machine_set_mouse(app.m, e.motion.x / app.scale,
                                     e.motion.y / app.scale, app.mouse_buttons);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int bit = e.button.button == SDL_BUTTON_LEFT   ? DORADO_MOUSE_LEFT
                    : e.button.button == SDL_BUTTON_MIDDLE ? DORADO_MOUSE_MIDDLE
                    : e.button.button == SDL_BUTTON_RIGHT  ? DORADO_MOUSE_RIGHT
                    : 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) app.mouse_buttons |= bit;
            else                               app.mouse_buttons &= ~bit;
            dorado_machine_set_mouse(app.m, e.button.x / app.scale,
                                     e.button.y / app.scale, app.mouse_buttons);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            int down = (e.type == SDL_KEYDOWN);
            SDL_Keycode k = e.key.keysym.sym;
            if (down && k == SDLK_F1 && !e.key.repeat) {
                app.paused = !app.paused;
                break;
            }
            dorado_display_key dk = map_key(k);
            if (dk != DORADO_KEY_NONE)
                dorado_machine_set_key(app.m, dk, down);
            break;
        }
        default: break;
        }
    }

    if (!app.paused) {
        uint64_t now = dorado_machine_cycles(app.m);
        dorado_machine_run_until(app.m, now + app.cycles_per_frame);
        if (!app.announced && dorado_machine_booted(app.m)) {
            app.announced = 1;
            printf("dorado_web: world loaded at cycle %llu\n",
                   (unsigned long long)dorado_machine_cycles(app.m));
        }
    }

    dorado_machine_render_display_list(app.m);
    const uint8_t *fb = app.disp->fb;

    /* Present the active world's native raster (Alto 808x606, Cedar lf
     * 1024x808); resize the canvas/window when the world changes size. */
    static int win_w = 0, win_h = 0;
    int aw = app.disp->active_w ? app.disp->active_w : DORADO_DISPLAY_W;
    int ah = app.disp->active_h ? app.disp->active_h : DORADO_DISPLAY_H;
    if (aw != win_w || ah != win_h) {
        SDL_SetWindowSize(app.win, aw, ah);
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
    SDL_UpdateTexture(app.tex, NULL, pixels,
                      DORADO_DISPLAY_W * (int)sizeof(uint32_t));
    SDL_Rect src = { 0, 0, aw, ah };
    SDL_RenderClear(app.ren);
    SDL_RenderCopy(app.ren, app.tex, &src, NULL);
    SDL_RenderPresent(app.ren);
    app.frame++;
}

int main(void)
{
    app.scale = 1;
    app.cycles_per_frame = 400000;

    if (dorado_web_boot(WEB_NETEXEC, 1) != 0)
        return 1;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "dorado_web: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    app.win = SDL_CreateWindow("Xerox Dorado", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, DORADO_DISPLAY_W,
                               DORADO_DISPLAY_H, 0);
    app.ren = SDL_CreateRenderer(app.win, -1, SDL_RENDERER_ACCELERATED);
    app.tex = SDL_CreateTexture(app.ren, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                DORADO_DISPLAY_W, DORADO_DISPLAY_H);
    if (!app.win || !app.ren || !app.tex) {
        fprintf(stderr, "dorado_web: SDL setup failed: %s\n", SDL_GetError());
        return 1;
    }

    /* 0 fps => use requestAnimationFrame; 1 => keep looping (don't return). */
    emscripten_set_main_loop(frame, 0, 1);
    return 0;
}
