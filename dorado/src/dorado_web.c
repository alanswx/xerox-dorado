/*
 * dorado_web - Emscripten/WebAssembly frontend for the Dorado emulator.
 *
 * Mirrors src/dorado_sdl.c, but restructured for the browser: the blocking
 * `while (running)` loop becomes a single frame() callback driven by
 * emscripten_set_main_loop(), since a WASM main thread may not block.
 *
 * No SDL. Under emsdk 6 the SDL2 port's renderer never presented to the
 * canvas and its keyboard events never reached SDL_PollEvent, so this
 * frontend talks to the browser directly: frames go to the canvas 2d
 * context via js_present(), and web_shell.html feeds keyboard/mouse
 * events in through the exported dorado_web_key()/dorado_web_mouse().
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
#define WEB_CEDAR_SNAPSHOT "/worlds/cedar-login.snap"

/* The Cedar Viewers DESKTOP checkpoint (2026-07-15 milestone): a snapshot
 * taken after the full Guest install with its MATCHED PDI (the guest
 * mutates the disk during the install, so the pair is inseparable). */
#define WEB_CEDAR_DESKTOP_SNAPSHOT "/worlds/cedar-desktop.snap"
#define WEB_CEDAR_DESKTOP_PDI      "/worlds/cedar-desktop.pdi"

/* Alto/Mesa world: the full Mesa VM microcode (vs aemu.eb, Alto/Nova only).
 * Boots the Mesa Network Executive (a Mesa/Pilot environment, sibling of
 * Cedar) over EFTP. */
#define WEB_MESA_EB    "/worlds/AltoMesaDorado.eb"
#define WEB_MESA_BOOT  WEB_BOOTDIR "/MesaNetExec.boot!1"

/* Alto disk boot: the AEmu world boots a real Alto OS from a Diablo-on-Trident
 * pack (dsk2trident of a ContrAlto games disk) instead of EFTP -- reaches the
 * BCPL Executive prompt; type a program name (e.g. `chess`, `mazewar`) + Return
 * to run it off the disk, the way an Alto did. Preloaded by `make web`. */
#define WEB_ALTO_PACK  "/worlds/games-trident.pack"
#define WEB_LISP_PACK  "/worlds/lisp-lyric-xcl.pack"
#define WEB_LISP_SNAPSHOT "/worlds/lisp-lyric-xcl.snap"

/* Browser pacing. The per-frame display render + blit cost (~100ms) dwarfs the
 * 60ns microcycle, so when the chunk is small the browser spends most of its
 * time presenting, not emulating -- a world whose prompt is ~155M cycles in
 * (Mesa) or ~640M (Cedar) then takes a minute to appear. Run large cycle
 * chunks during boot (few renders => fast time-to-prompt), then drop to a small
 * chunk on the first keystroke so interaction stays responsive. */
#define WEB_CYCLES_BOOT         4000000u
#define WEB_CYCLES_INTERACTIVE   400000u

/* Single global app state - the frame() callback has no other way to reach
 * it under emscripten_set_main_loop. */
static struct {
    dorado_machine  *m;
    dorado_display  *disp;
    int              scale;
    uint64_t         cycles_per_frame;
    int              mouse_buttons;
    int              paused;
    int              announced;
    long             frame;
} app;

static uint32_t pixels[DORADO_DISPLAY_W * DORADO_DISPLAY_H];

/* Present RGBA pixels straight onto Module.canvas with a 2d context.
 * SDL is kept for input only: under emsdk 6 the SDL2 port's renderer
 * (SDL_CreateRenderer + SDL_UpdateTexture + SDL_RenderPresent) creates a
 * WebGL context on the canvas but its draws never reach it, leaving the
 * page permanently black. A plain putImageData avoids the renderer
 * entirely and needs no GL. The pixel words are copied out of the wasm
 * heap first: HEAPU8.buffer is a *resizable* ArrayBuffer under
 * ALLOW_MEMORY_GROWTH, and Chrome rejects ImageData views over resizable
 * buffers (same class of failure as the TextDecoder crash that killed
 * the previously deployed build). */
EM_JS(void, js_present, (const void *px, int w, int h), {
    var c = Module['canvas'];
    if (c.width !== w) c.width = w;
    if (c.height !== h) c.height = h;
    if (!Module['ctx2d']) Module['ctx2d'] = c.getContext('2d');
    var bytes = HEAPU8.slice(px, px + w * h * 4);
    Module['ctx2d'].putImageData(
        new ImageData(new Uint8ClampedArray(bytes.buffer), w, h), 0, 0);
});

/* Key encoding shared with web_shell.html's keydown/keyup wiring:
 * printable keys arrive as their unshifted ASCII code ('a'..'z', '0'..'9',
 * punctuation, ' ', '\r', '\b', '\t', 27, 127); modifiers and function
 * keys as the WEB_KEY_* values below. Keep the two sides in sync. */
#define WEB_KEY_LSHIFT 0x1001
#define WEB_KEY_RSHIFT 0x1002
#define WEB_KEY_CTRL   0x1003
#define WEB_KEY_F1     0x1004

static dorado_display_key map_key(int k)
{
    if (k >= 'a' && k <= 'z') {
        static const dorado_display_key letters[26] = {
            DORADO_KEY_A, DORADO_KEY_B, DORADO_KEY_C, DORADO_KEY_D,
            DORADO_KEY_E, DORADO_KEY_F, DORADO_KEY_G, DORADO_KEY_H,
            DORADO_KEY_I, DORADO_KEY_J, DORADO_KEY_K, DORADO_KEY_L,
            DORADO_KEY_M, DORADO_KEY_N, DORADO_KEY_O, DORADO_KEY_P,
            DORADO_KEY_Q, DORADO_KEY_R, DORADO_KEY_S, DORADO_KEY_T,
            DORADO_KEY_U, DORADO_KEY_V, DORADO_KEY_W, DORADO_KEY_X,
            DORADO_KEY_Y, DORADO_KEY_Z,
        };
        return letters[k - 'a'];
    }
    if (k >= '0' && k <= '9') {
        static const dorado_display_key digits[10] = {
            DORADO_KEY_0, DORADO_KEY_1, DORADO_KEY_2, DORADO_KEY_3,
            DORADO_KEY_4, DORADO_KEY_5, DORADO_KEY_6, DORADO_KEY_7,
            DORADO_KEY_8, DORADO_KEY_9,
        };
        return digits[k - '0'];
    }
    switch (k) {
    case ' ':  return DORADO_KEY_SPACE;
    case '\r': return DORADO_KEY_RETURN;
    case '\b': return DORADO_KEY_BS;
    case '\t': return DORADO_KEY_TAB;
    case 27:   return DORADO_KEY_ESC;
    case 127:  return DORADO_KEY_DEL;
    case '-':  return DORADO_KEY_MINUS;
    case '=':  return DORADO_KEY_PLUS;
    case '[':  return DORADO_KEY_LBRACKET;
    case ']':  return DORADO_KEY_RBRACKET;
    case ';':  return DORADO_KEY_SEMICOLON;
    case '\'': return DORADO_KEY_QUOTE;
    case ',':  return DORADO_KEY_COMMA;
    case '.':  return DORADO_KEY_PERIOD;
    case '/':  return DORADO_KEY_FSLASH;
    case '\\': return DORADO_KEY_BSLASH;
    case WEB_KEY_LSHIFT: return DORADO_KEY_LSHIFT;
    case WEB_KEY_RSHIFT: return DORADO_KEY_RSHIFT;
    case WEB_KEY_CTRL:   return DORADO_KEY_CTRL;
    default: return DORADO_KEY_NONE;
    }
}

/* Browser input entry points, called from web_shell.html's plain JS event
 * listeners (ccall). Replaces the SDL event queue. */
EMSCRIPTEN_KEEPALIVE
void dorado_web_key(int key, int down)
{
    if (!app.m) return;
    if (down && key == WEB_KEY_F1) {
        app.paused = !app.paused;
        return;
    }
    dorado_display_key dk = map_key(key);
    if (dk != DORADO_KEY_NONE)
        dorado_machine_set_key(app.m, dk, down);
}

EMSCRIPTEN_KEEPALIVE
void dorado_web_mouse(int x, int y, int buttons)
{
    if (!app.m) return;
    app.mouse_buttons = buttons;
    dorado_machine_set_mouse(app.m, x / app.scale, y / app.scale, buttons);
}

/* (Re)create the machine booting `eftp_path`. When dir_all is set, every
 * Alto B-format file beside it is registered as a NetExec menu entry. Called
 * once at startup and again from JS each time the game dropdown changes.
 * Returns 0 on success. Exported (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot(const char *eftp_path, int dir_all)
{
    unsetenv("DORADO_DISPM_PRESENT");
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
    app.cycles_per_frame = WEB_CYCLES_BOOT;
    printf("dorado_web: booting %s (menu=%d)\n", cfg.eftp_boot, cfg.boot_dir_all);
    return 0;
}

/* (Re)create the machine as the native Cedar/Pilot world: Initial netboots the
 * Cedar microcode, the matched Pilot germ is planted, and the Pilot/Cedar PDI
 * is mounted as drive 0. Boots (no boot-key chord = disk) to Cedar 6.1's
 * SimpleTerminal login prompt -- type at the Name: prompt once it appears.
 * Exported (KEEPALIVE) so JS can ccall it. The browser normally expands a
 * wasm32-native checkpoint at the login prompt before calling this function;
 * if the checkpoint is absent, retain the full-boot fallback. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar(void)
{
    unsetenv("DORADO_DISPM_PRESENT");
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
    cfg.pilot_disk_pdi[0] = WEB_CEDAR_PDI;
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
    int restored = 0;
    FILE *snapshot = fopen(WEB_CEDAR_SNAPSHOT, "rb");
    if (snapshot) {
        fclose(snapshot);
        if (dorado_machine_restore(app.m, WEB_CEDAR_SNAPSHOT) != 0) {
            fprintf(stderr, "dorado_web: failed to restore Cedar snapshot\n");
            dorado_machine_destroy(app.m);
            app.m = NULL;
            return 1;
        }
        restored = 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = restored;
    app.frame     = 0;
    app.cycles_per_frame = restored ? WEB_CYCLES_INTERACTIVE : WEB_CYCLES_BOOT;
    printf("dorado_web: %s Cedar 6.1 (Pilot disk)\n",
           restored ? "restored" : "booting");
    return 0;
}

/* (Re)create the machine at the saved Cedar Viewers DESKTOP: the snapshot
 * restores with its matched (install-mutated) PDI mounted as drive 0. There
 * is no STP server in the browser, so post-restore Bringovers error
 * gracefully; the installed desktop itself is complete. Exported
 * (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar_desktop(void)
{
    unsetenv("DORADO_DISPM_PRESENT");
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
    cfg.pilot_disk_pdi[0] = WEB_CEDAR_DESKTOP_PDI;
    cfg.eftp_boot    = NULL;
    cfg.alto_ether_boot = 0;
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;
    cfg.boot_keys_count = 1;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Cedar desktop machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_CEDAR_DESKTOP_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore the desktop snapshot\n");
        dorado_machine_destroy(app.m);
        app.m = NULL;
        return 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored the Cedar 6.1 Viewers desktop\n");
    return 0;
}

/* (Re)create the machine as the Alto/Mesa world booting a Mesa/Pilot program
 * over EFTP -- a non-Cedar Mesa environment. `eftp_path` selects the boot file
 * (NULL/empty defaults to the Mesa Network Executive); the same AltoMesaDorado
 * world also runs Mesa games/tools (PPong, MazeWar, PupWatch, TriEx). Exported
 * (KEEPALIVE) so JS can ccall it with a string argument. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_mesa(const char *eftp_path)
{
    unsetenv("DORADO_DISPM_PRESENT");
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
    cfg.eftp_boot    = (eftp_path && *eftp_path) ? eftp_path : WEB_MESA_BOOT;
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
    app.cycles_per_frame = WEB_CYCLES_BOOT;
    printf("dorado_web: booting Mesa world %s\n", cfg.eftp_boot);
    return 0;
}

/* (Re)create the machine booting the AEmu (Alto-on-Dorado) world from a real
 * Alto disk: Initial netboots aemu.eb, then -- with no boot-key chord = disk
 * boot reason -- the Alto OS boots from the mounted Diablo-on-Trident pack and
 * reaches the BCPL Executive prompt. Type a program name + Return at the prompt
 * to run it off the disk. Exported (KEEPALIVE) so JS can ccall it. The Exec
 * appears ~120M cycles in, so allow ~20-30s in the browser. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_disk(void)
{
    unsetenv("DORADO_DISPM_PRESENT");
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
    cfg.eth_boot_110 = WEB_EB_WORLD;     /* netboot the AEmu world (Initial)  */
    cfg.eftp_boot    = NULL;             /* boot from disk, not EFTP          */
    cfg.disk_pack[0] = WEB_ALTO_PACK;    /* mount the Alto disk on drive 0    */
    cfg.boot_keys[0] = DORADO_KEY_NONE;  /* no chord = disk boot reason       */
    cfg.boot_keys_count = 1;
    cfg.boot_dir_all = 0;
    /* alto_ether_boot stays 1 (default): Initial's ether boot loads the world. */

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Alto-disk machine\n");
        return 1;
    }
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 0;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_BOOT;
    printf("dorado_web: booting Alto OS from disk (%s)\n", WEB_ALTO_PACK);
    return 0;
}

/* Restore a WebAssembly-native checkpoint taken at Lyric's Exec (XCL)
 * desktop. Machine snapshots contain raw C structs and are ABI-specific, so
 * this asset is generated by the wasm32 command-line build rather than copied
 * from a native run. The matching writable Trident pack is still required;
 * web_shell.html expands both gzipped assets into MEMFS before calling us. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_lisp(void)
{
    if (app.m) {
        dorado_machine_destroy(app.m);
        app.m = NULL;
        app.disp = NULL;
    }

    setenv("DORADO_DISPM_PRESENT", "1", 1);

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_EB_WORLD;
    cfg.eftp_boot    = NULL;
    cfg.disk_pack[0] = WEB_LISP_PACK;
    cfg.alto_ether_boot = 0;
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;
    cfg.boot_keys_count = 1;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Lisp restore machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_LISP_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore Lisp snapshot\n");
        dorado_machine_destroy(app.m);
        app.m = NULL;
        return 1;
    }

    /* Keep the decompressed checkpoint so selecting Lyric again remains an
     * immediate restore rather than requiring a page reload. */
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored Interlisp-D Lyric XCL at cycle %llu\n",
           (unsigned long long)dorado_machine_cycles(app.m));
    return 0;
}

/* One animation frame: advance the emulator, blit the display. Input
 * arrives asynchronously via dorado_web_key()/dorado_web_mouse(). */
static void frame(void)
{
    if (!app.paused) {
        uint64_t now = dorado_machine_cycles(app.m);
        dorado_machine_run_until(app.m, now + app.cycles_per_frame);
        if (!app.announced && dorado_machine_booted(app.m)) {
            app.announced = 1;
            printf("dorado_web: world loaded at cycle %llu\n",
                   (unsigned long long)dorado_machine_cycles(app.m));
        }
    }

    int rendered_px = dorado_machine_render_display_list(app.m);
    /* Console heartbeat (~every 256 frames): emulated-cycle progress and
     * rendered pixel count, for diagnosing a stalled boot in the browser. */
    {
        static unsigned hb;
        if ((++hb & 0xFF) == 0)
            printf("dorado_web: heartbeat cyc=%llu px=%d chunk=%llu\n",
                   (unsigned long long)dorado_machine_cycles(app.m),
                   rendered_px,
                   (unsigned long long)app.cycles_per_frame);
    }
    /* Stay in fast-boot (large cycle chunks) while the screen is still blank,
     * then switch to a responsive chunk once the world paints its UI (herald /
     * prompt / game field). This is keyboard-independent: typing during the
     * boot never slows the boot, and the prompt always appears on time. */
    if (app.cycles_per_frame != WEB_CYCLES_INTERACTIVE && rendered_px > 700)
        app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    const uint8_t *fb = app.disp->fb;

    /* Present the active world's native raster (Alto 808x606, Cedar lf
     * 1024x808); js_present resizes the canvas when the world changes. */
    int aw = app.disp->active_w ? app.disp->active_w : DORADO_DISPLAY_W;
    int ah = app.disp->active_h ? app.disp->active_h : DORADO_DISPLAY_H;

    /* Expand the 1bpp framebuffer to packed RGBA rows (stride == aw, the
     * layout ImageData expects). 0xAABBGGRR little-endian == RGBA bytes:
     * black ink 0xFF000000, white paper 0xFFFFFFFF. */
    uint32_t *px = pixels;
    for (int y = 0; y < ah; y++) {
        const uint8_t *row = fb + y * DORADO_DISPLAY_ROW_BYTES;
        uint32_t *out = px + y * aw;
        for (int x = 0; x < aw; x++) {
            int bit = (row[x >> 3] >> (7 - (x & 7))) & 1;
            out[x] = bit ? 0xFF000000u : 0xFFFFFFFFu;
        }
    }
    js_present(pixels, aw, ah);
    app.frame++;
}

int main(void)
{
    /* The Cedar PDI worlds need the media-compat label check (File.FP DA
     * hints and file flags are absent from converted/synthetic media).
     * dorado_trace_flag() caches per-name lookups on first use, so this
     * must be set before ANY machine is created — harmless for the
     * non-PDI (Alto/Mesa/Lisp) worlds. */
    setenv("DORADO_PDI_IGNORE_LABEL_FLAGS", "1", 1);

    app.scale = 1;
    app.cycles_per_frame = WEB_CYCLES_BOOT;

    if (dorado_web_boot(WEB_NETEXEC, 1) != 0)
        return 1;

    /* 0 fps => use requestAnimationFrame; 1 => keep looping (don't return). */
    emscripten_set_main_loop(frame, 0, 1);
    return 0;
}
