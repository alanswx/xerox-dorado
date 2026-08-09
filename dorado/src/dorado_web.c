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

#include "dispm.h"
#include "machine.h"
#include "display.h"
#include "typetext.h"

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
#define WEB_CEDAR_DEMO_SNAPSHOT    "/worlds/cedar-demo.snap"
#define WEB_CEDAR_DEMO_PDI         "/worlds/cedar-demo.pdi"

/* The CORPUS pack: the same Cedar desktop, but running from a volume that
 * carries the recovered PARC file corpus (repacked into Cedar's layout with
 * room to run; see tools/make_corpus_volume.sh). List in the CommandTool
 * shows real 1985 files. */
#define WEB_CEDAR_CORPUS_SNAPSHOT  "/worlds/cedar-corpus.snap"
#define WEB_CEDAR_CORPUS_PDI       "/worlds/cedar-corpus.pdi"
/* Pruned STP release tree (CedarChest6.1 + font directories) preloaded
 * into the wasm FS so the in-process server can answer Bringovers and
 * demand-fetches from the browser. */
#define WEB_STP_ROOT               "/stp"

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
#define WEB_LISP_PACK  "/worlds/lisp-lispusers.pack"
#define WEB_LISP_SNAPSHOT "/worlds/lisp-lispusers.snap"
/* The Interlisp served tree (285 .LCOMs, display fonts, the IRM), unpacked
 * from lisp-src.tar.gz by the shell when the Lyric world is chosen.  The
 * guest reaches it as the {DORADO} device over Leaf/STP. */
#define WEB_LISP_FTP_ROOT "/lisp-stp"
/* The FULL-library Lyric world: Released-Full.sysout!2 (Jan-88) already
 * loaded into a 19,000-page VMEM.  A separate world from the one above, not
 * a replacement: this one carries ~2,300 more preloaded library pages and
 * has no on-pack package set, where the other has the served tree and the
 * reference manual.  See docs/lisp-leaf-handoff.md 6.18 for why it is
 * Released-Full.sysout!2 and not Full.sysout!6. */
#define WEB_LISP_FULL_PACK     "/worlds/lisp-lyric-full.pack"
#define WEB_LISP_FULL_SNAPSHOT "/worlds/lisp-lyric-full.snap"
#define WEB_SMALLTALK_EB       "/worlds/SmalltalkDorado.eb"
#define WEB_SMALLTALK_PACK     "/worlds/smalltalk76.pack"
#define WEB_SMALLTALK_SNAPSHOT "/worlds/smalltalk76.snap"

/* Browser pacing. The per-frame display render + blit cost (~100ms) dwarfs the
 * 60ns microcycle, so when the chunk is small the browser spends most of its
 * time presenting, not emulating -- a world whose prompt is ~155M cycles in
 * (Mesa) or ~640M (Cedar) then takes a minute to appear. Run large cycle
 * chunks during boot (few renders => fast time-to-prompt), then drop to a small
 * chunk on the first keystroke so interaction stays responsive. */
#define WEB_CYCLES_BOOT         4000000u
/* Real time. cycles_per_frame x refresh IS the emulated speed, so the old
 * 400,000 pinned the page at 400,000 x 60 = 24.0 M cycles/s = 6.5 M
 * microinstructions/s = 0.39x a real Dorado -- below what the wasm core can
 * now sustain (0.68x). 16.666 M microinstr/s x 3.70 cycles/microinstr / 60 Hz
 * = 1,028,000. The cost of raising it is frame RATE, never emulated speed:
 * when a chunk takes longer than a refresh the page presents less often
 * (~41 fps at 0.68x) while the machine runs as fast as the core allows. */
#define WEB_CYCLES_INTERACTIVE  1028000u
/* Emulated cycles to keep the fast-boot chunk for, regardless of what the
 * screen shows. Covers the slowest cold boot in the menu (Cedar ~640 M). */
#define WEB_BOOT_FAST_CYCLES     800000000ull

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
    /* What a restore has to put back. dorado_machine_restore CLOBBERS the
     * ethernet state with the BAKE-TIME ftp root -- a native path that does
     * not exist in MEMFS -- so every restore path re-applies this. Recording
     * it here lets the generic save/resume below do the same without knowing
     * which world is loaded. */
    const char      *ftp_root;
    const char      *world;      /* short tag, for the download's filename */
} app;

static uint32_t pixels[DORADO_DISPLAY_W * DORADO_DISPLAY_H];

/* Clipboard paste queue (see dorado_web_paste below); cleared on
 * every machine teardown so a half-typed paste never leaks into a
 * freshly booted world. */
static dorado_typequeue paste_queue;

/* The browser can request a board mode before the next world is created.
 * AUTO preserves each world's safe default: no board for Alto/Mesa/Cedar/
 * Smalltalk, standard DispM for Lyric. This is frontend state, not snapshot
 * state; the machine config consumes it during creation. */
static dorado_dispm_type web_dispm_override = DORADO_DISPM_AUTO;

static dorado_dispm_type web_selected_dispm(dorado_dispm_type world_default)
{
    return web_dispm_override == DORADO_DISPM_AUTO
         ? world_default : web_dispm_override;
}

EMSCRIPTEN_KEEPALIVE
int dorado_web_set_dispm(int mode)
{
    if (mode < DORADO_DISPM_AUTO || mode > DORADO_DISPM_HIGHRES)
        return 1;
    web_dispm_override = (dorado_dispm_type)mode;
    return 0;
}

/* Present the monochrome RGBA pixels onto the combined display canvas.
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
    Module.webMonoW = w;
    Module.webMonoH = h;
    var cw = Module.webColorW || 0;
    var ch = Module.webColorH || 0;
    var view = Module.webViewMode || 0; // 0 both, 1 color, 2 monochrome
    var showColor = view !== 2 && cw > 0;
    var showMono = view !== 1 || !showColor;
    var colorLeft = !Module.webColorRight;
    var totalW = (showMono ? w : 0) + (showColor ? cw : 0);
    var totalH = Math.max(showMono ? h : 0, showColor ? ch : 0);
    if (c.width !== totalW) c.width = totalW;
    if (c.height !== totalH) c.height = totalH;
    if (!Module['ctx2d']) Module['ctx2d'] = c.getContext('2d');
    if (showMono) {
      var bytes = HEAPU8.slice(px, px + w * h * 4);
      var monoX = showColor && colorLeft ? cw : 0;
      Module['ctx2d'].putImageData(
          new ImageData(new Uint8ClampedArray(bytes.buffer), w, h), monoX, 0);
    }
});

/* Boot progress for the status line. A world that paints its first line at
 * 30 M cycles but only prompts near 300 M looks hung for the ~20 s in
 * between, so say what is happening instead of leaving a still screen. The
 * shell shows this text while a boot is running. */
EM_JS(void, js_boot_progress, (int millions), {
    var el = document.getElementById('status');
    if (!el || !el.dataset || el.dataset.booting !== '1') return;
    var base = el.dataset.bootText || el.textContent;
    el.dataset.bootText = base;
    el.textContent = base + '  [' + millions + 'M cycles]';
});

/* Key encoding shared with web_shell.html's keydown/keyup wiring:
 * printable keys arrive as their unshifted ASCII code ('a'..'z', '0'..'9',
 * punctuation, ' ', '\r', '\b', '\t', 27, 127); modifiers and function
 * keys as the WEB_KEY_* values below. Keep the two sides in sync. */
#define WEB_KEY_LSHIFT 0x1001
#define WEB_KEY_RSHIFT 0x1002
#define WEB_KEY_CTRL   0x1003
#define WEB_KEY_F1     0x1004
/* The rest of the Alto's 61 keys, which have no modern keytop. Keep in sync
 * with web_shell.html's KEYCODES table (Alto HW Manual Figure 6, doc p.27). */
#define WEB_KEY_ARROW  0x1005   /* the Alto's left-arrow, host Backquote  */
#define WEB_KEY_BLANKT 0x1006
#define WEB_KEY_BLANKM 0x1007
#define WEB_KEY_BLANKB 0x1008
#define WEB_KEY_LF     0x1009
#define WEB_KEY_LOCK   0x100A

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
    case WEB_KEY_ARROW:  return DORADO_KEY_ARROW;
    case WEB_KEY_BLANKT: return DORADO_KEY_BLANKTOP;
    case WEB_KEY_BLANKM: return DORADO_KEY_BLANKMIDDLE;
    case WEB_KEY_BLANKB: return DORADO_KEY_BLANKBOTTOM;
    case WEB_KEY_LF:     return DORADO_KEY_LF;
    case WEB_KEY_LOCK:   return DORADO_KEY_LOCK;
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

/* ---- The colour screen (DispM) -----------------------------------------
 *
 * A SECOND surface, not a mode of the first: the Dorado's b/w display is
 * 1024x808 at one bit per pixel and colour is a separate board driving its
 * own 640x480 or 1024x768 monitor. The page draws it on its own canvas.
 *
 * dorado_web_color_frame() repaints from the ColorCSB chain and returns a
 * pointer to RGBA the page can blit straight into ImageData; width and height
 * come back through dorado_web_color_w/h. Returns 0 when no board is
 * installed or the guest has not armed the chain, which is the page's cue to
 * keep the canvas hidden. */
static uint8_t *web_color_rgba;
static int web_color_w, web_color_h;

EMSCRIPTEN_KEEPALIVE
int dorado_web_color_w(void) { return web_color_w; }

EMSCRIPTEN_KEEPALIVE
int dorado_web_color_h(void) { return web_color_h; }

EMSCRIPTEN_KEEPALIVE
int dorado_web_color_display_right(void)
{
    return app.m ? dorado_machine_color_display_right(app.m) : -1;
}

EMSCRIPTEN_KEEPALIVE
uintptr_t dorado_web_color_frame(void)
{
    if (!app.m || dorado_dispm_installed() == DORADO_DISPM_NONE) return 0;
    if (dorado_dispm_render(dorado_machine_read_visible_word, app.m) <= 0)
        return 0;
    int w = 0, h = 0;
    const uint8_t *rgb = dorado_dispm_rgb(&w, &h);
    if (!rgb || w <= 0 || h <= 0) return 0;
    if (w != web_color_w || h != web_color_h) {
        free(web_color_rgba);
        web_color_rgba = (uint8_t *)malloc((size_t)w * (size_t)h * 4u);
        web_color_w = w; web_color_h = h;
    }
    if (!web_color_rgba) { web_color_w = web_color_h = 0; return 0; }
    for (int i = 0; i < w * h; i++) {
        web_color_rgba[i * 4 + 0] = rgb[i * 3 + 0];
        web_color_rgba[i * 4 + 1] = rgb[i * 3 + 1];
        web_color_rgba[i * 4 + 2] = rgb[i * 3 + 2];
        web_color_rgba[i * 4 + 3] = 255;
    }
    return (uintptr_t)web_color_rgba;
}

/* ---- Front panel ---------------------------------------------------------
 *
 * The browser gets the SAME panel as the SDL window, but drawn as HTML
 * instead of microui: the page already has real chrome -- a dropdown,
 * buttons, a status line -- and a bitmap toolkit inside the canvas would be
 * worse to use and more code to keep working.
 *
 * What must not diverge is the DATA, so both frontends read it through the
 * one dorado_machine_get_panel() accessor. This just marshals that into JSON,
 * which is the cheapest thing for JS to consume and needs no new export per
 * lamp.
 */
EMSCRIPTEN_KEEPALIVE
const char *dorado_web_panel(void)
{
    static char json[256];
    dorado_machine_panel p;
    dorado_machine_get_panel(app.m, &p);
    snprintf(json, sizeof json,
             "{\"lamp\":%d,\"booted\":%d,\"cycles\":%llu,\"uinstr\":%llu,"
             "\"disk\":%llu,\"net\":%llu,\"frames\":%llu,\"paused\":%d,"
             "\"world\":\"%s\",\"root\":\"%s\"}",
             p.lamp_on, p.booted,
             (unsigned long long)p.cycles, (unsigned long long)p.uinstructions,
             (unsigned long long)p.disk_activity,
             (unsigned long long)p.net_activity,
             (unsigned long long)p.display_frames, app.paused,
             app.world ? app.world : "dorado",
             app.ftp_root ? app.ftp_root : "");
    return json;
}

/* The machine's own front-panel boot button. Held for a number of scan
 * lines: the Booting memo's MinimumPush is 8 ms and pushes shorter than
 * 10 ms are contact bounce, so give it a comfortable press -- the same
 * 600 the SDL panel uses. */
EMSCRIPTEN_KEEPALIVE
void dorado_web_boot_button(void)
{
    if (app.m) dorado_display_boot_button(dorado_machine_display(app.m), 600);
}

/* ---- Save and resume in the browser --------------------------------------
 *
 * The emulator already has a snapshot format and the whole project runs on
 * baked checkpoints; what the browser lacked was any way to keep one. These
 * two entry points bridge it to MEMFS, and web_shell.html turns that into a
 * download and a file picker.
 *
 * SCOPE, stated because it is a real limit and not a bug: a snapshot restores
 * into a machine that was CREATED for the same world. dorado_machine_create
 * takes the microcode world, germ and disk images, and the snapshot carries
 * none of that -- so the page must be booted to the same dropdown entry
 * before resuming. The shell puts the world in the download's filename and
 * checks it on the way back in, which is the honest amount of protection
 * available: a mismatched pair has the same struct sizes, so
 * dorado_machine_restore's ABI check cannot catch it.
 *
 * Anything else in this file that restores also re-applies the served root,
 * because dorado_machine_restore clobbers the ethernet state with the
 * bake-time one. This does the same from app.ftp_root.
 */
#define WEB_SAVE_PATH "/dorado-state.snap"

EMSCRIPTEN_KEEPALIVE
int dorado_web_save_state(void)
{
    if (!app.m) return -1;
    if (dorado_machine_snapshot(app.m, WEB_SAVE_PATH) != 0) return -1;
    FILE *f = fopen(WEB_SAVE_PATH, "rb");
    if (!f) return -1;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return -1; }
    long n = ftell(f);
    fclose(f);
    return n > 0 ? (int)n : -1;
}

EMSCRIPTEN_KEEPALIVE
int dorado_web_restore_state(void)
{
    if (!app.m) return -1;
    if (dorado_machine_restore(app.m, WEB_SAVE_PATH) != 0) return -1;
    if (app.ftp_root)
        dorado_machine_set_ftp_source(app.m, NULL, app.ftp_root);
    app.disp = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused = 0;
    paste_queue.active = 0;
    return 0;
}

/* The world tag, for naming the saved file and for refusing an obvious
 * mismatch on the way back in. */
EMSCRIPTEN_KEEPALIVE
const char *dorado_web_world_tag(void)
{
    return app.world ? app.world : "dorado";
}

EMSCRIPTEN_KEEPALIVE
void dorado_web_mouse(int x, int y, int buttons)
{
    if (!app.m) return;
    app.mouse_buttons = buttons;
    dorado_machine_set_mouse(app.m, x / app.scale, y / app.scale, buttons);
}

EMSCRIPTEN_KEEPALIVE
void dorado_web_mouse_delta(int dx, int dy)
{
    if (!app.m) return;
    dorado_machine_mouse_delta(app.m, dx / app.scale, dy / app.scale);
}

EMSCRIPTEN_KEEPALIVE
int dorado_web_mouse_delta_active(void)
{
    return app.m ? dorado_machine_mouse_delta_active(app.m) : 0;
}

EMSCRIPTEN_KEEPALIVE
void dorado_web_mouse_buttons(int buttons)
{
    if (!app.m) return;
    app.mouse_buttons = buttons;
    dorado_machine_set_mouse_buttons(app.m, buttons);
}

/* (Re)create the machine booting `eftp_path`. When dir_all is set, every
 * Alto B-format file beside it is registered as a NetExec menu entry. Called
 * once at startup and again from JS each time the game dropdown changes.
 * Returns 0 on success. Exported (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot(const char *eftp_path, int dir_all)
{
    app.world = "alto";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
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
    app.world = "cedar-login";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
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
    cfg.ftp_root     = WEB_STP_ROOT;  /* in-process STP server file tree */

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
        paste_queue.active = 0;
            app.m = NULL;
            return 1;
        }
        restored = 1;
        dorado_machine_set_ftp_source(app.m, NULL, WEB_STP_ROOT);
    app.ftp_root = WEB_STP_ROOT;
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
 * restores with its matched (install-mutated) PDI mounted as drive 0. The
 * in-process STP server serves the pruned tree preloaded at /stp
 * (CedarChest6.1 + fonts), so post-restore Bringovers and demand-fetches
 * (e.g. AISViewer image content) work in the browser. Exported
 * (KEEPALIVE) so JS can ccall it. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar_desktop(void)
{
    app.world = "cedar-desktop";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
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
    cfg.ftp_root     = WEB_STP_ROOT;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Cedar desktop machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_CEDAR_DESKTOP_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore the desktop snapshot\n");
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        return 1;
    }
    dorado_machine_set_ftp_source(app.m, NULL, WEB_STP_ROOT);
    app.ftp_root = WEB_STP_ROOT;
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored the Cedar 6.1 Viewers desktop\n");
    return 0;
}

/* (Re)create the machine at the saved APPS-DEMO desktop: a clean Cedar
 * desktop with ChessHack and Clock parked as icons, both modules already
 * loaded and the Chess40 font pre-cached in memory. Opening ChessHack
 * (click its icon) paints the chess board with NO network. Same restore
 * contract as the desktop; the pruned /stp tree is still served so the
 * picture-viewer .cm launchers keep working. Exported for JS ccall. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar_demo(void)
{
    app.world = "cedar-demo";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_CEDAR_EB;
    cfg.germ_path    = WEB_CEDAR_GERM;
    cfg.pilot_disk_pdi[0] = WEB_CEDAR_DEMO_PDI;
    cfg.eftp_boot    = NULL;
    cfg.alto_ether_boot = 0;
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;
    cfg.boot_keys_count = 1;
    cfg.ftp_root     = WEB_STP_ROOT;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Cedar apps-demo machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_CEDAR_DEMO_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore the apps-demo snapshot\n");
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        return 1;
    }
    dorado_machine_set_ftp_source(app.m, NULL, WEB_STP_ROOT);
    app.ftp_root = WEB_STP_ROOT;
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored the Cedar 6.1 apps demo (ChessHack + Clock)\n");
    return 0;
}

/* (Re)create the machine at the saved CORPUS desktop: a Cedar desktop whose
 * disk is the recovered PARC file corpus. Same restore contract as the
 * desktop pack (snapshot + its matched PDI). Exported (KEEPALIVE). */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_cedar_corpus(void)
{
    app.world = "cedar-corpus";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_CEDAR_EB;
    cfg.germ_path    = WEB_CEDAR_GERM;
    cfg.pilot_disk_pdi[0] = WEB_CEDAR_CORPUS_PDI;
    cfg.eftp_boot    = NULL;
    cfg.alto_ether_boot = 0;
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;
    cfg.boot_keys_count = 1;
    cfg.ftp_root     = WEB_STP_ROOT;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create the corpus machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_CEDAR_CORPUS_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore the corpus snapshot\n");
        dorado_machine_destroy(app.m);
        app.m = NULL;
        return 1;
    }
    dorado_machine_set_ftp_source(app.m, NULL, WEB_STP_ROOT);
    app.ftp_root = WEB_STP_ROOT;
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored the Cedar corpus desktop\n");
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
    app.world = "mesa";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
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
    app.world = "alto-disk";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
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
/* Shared by both Lyric worlds (dorado_web_boot_lisp and
 * dorado_web_boot_lisp_full): identical machine setup and restore, differing
 * only in which checkpoint and pack are used.  `label` is for the log line. */
static int web_boot_lyric(const char *snapshot, const char *pack,
                          const char *label)
{
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_STANDARD);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_EB_WORLD;
    cfg.eftp_boot    = NULL;
    cfg.disk_pack[0] = pack;
    cfg.ftp_root     = WEB_LISP_FTP_ROOT;
    cfg.alto_ether_boot = 0;
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;
    cfg.boot_keys_count = 1;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Lisp restore machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, snapshot) != 0) {
        fprintf(stderr, "dorado_web: failed to restore Lisp snapshot\n");
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        return 1;
    }

    /* The restore clobbers the ethernet state with the BAKE-TIME ftp root
     * (a native path that does not exist in MEMFS); point it back at the
     * unpacked lisp-src tree, the same post-restore re-set every Cedar
     * boot above does with /stp. */
    dorado_machine_set_ftp_source(app.m, NULL, WEB_LISP_FTP_ROOT);
    app.ftp_root = WEB_LISP_FTP_ROOT;

    /* Keep the decompressed checkpoint so selecting Lyric again remains an
     * immediate restore rather than requiring a page reload. */
    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored Interlisp-D Lyric %s at cycle %llu\n",
           label, (unsigned long long)dorado_machine_cycles(app.m));
    return 0;
}

int dorado_web_boot_lisp(void)
{
    app.world = "lisp";
    return web_boot_lyric(WEB_LISP_SNAPSHOT, WEB_LISP_PACK, "XCL");
}

/* The full-library world.  Same restore path; a different checkpoint whose
 * VMEM already holds Released-Full.sysout!2. */
int dorado_web_boot_lisp_full(void)
{
    app.world = "lisp-full";
    return web_boot_lyric(WEB_LISP_FULL_SNAPSHOT, WEB_LISP_FULL_PACK,
                          "full-library Exec");
}

/* Restore a WebAssembly-native checkpoint taken at the Smalltalk-76 desktop
 * (Top View + the Classes browser + a UserView workspace). The world is
 * DSemu -- the Alto emulator PLUS the Smalltalk microcode -- and the medium
 * is the "XM Smalltalk" pack, booted by the Alto Executive's
 * `Bootfrom xmsmall.boot`; that takes ~1.95 B cycles, which is why the
 * browser restores instead of booting. Same ABI rule as the Lyric asset:
 * the snapshot is produced by the wasm32 build, the pack by the native one.
 * web_shell.html expands both gzipped assets into MEMFS before calling us. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_boot_smalltalk(void)
{
    app.world = "smalltalk";
    if (app.m) {
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        app.disp = NULL;
    }

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.dispm_type   = web_selected_dispm(DORADO_DISPM_NONE);
    cfg.bb_rom       = WEB_BB_ROM;
    cfg.bootstrap_mb = WEB_BOOTSTRAP;
    cfg.initial_mb   = WEB_INITIAL;
    cfg.kernel_mb    = WEB_KERNEL;
    cfg.memmisc_mb   = WEB_MEMMISC;
    cfg.ifu_mb       = WEB_IFU;
    cfg.eth_boot_110 = WEB_SMALLTALK_EB;
    cfg.eftp_boot    = NULL;
    cfg.disk_pack[0] = WEB_SMALLTALK_PACK;
    cfg.alto_ether_boot = 0;          /* --no-alto-boot */
    cfg.boot_dir_all = 0;
    cfg.boot_keys[0] = DORADO_KEY_NONE;   /* no chord = disk boot reason */
    cfg.boot_keys_count = 1;

    app.m = dorado_machine_create(&cfg);
    if (!app.m) {
        fprintf(stderr, "dorado_web: failed to create Smalltalk machine\n");
        return 1;
    }
    if (dorado_machine_restore(app.m, WEB_SMALLTALK_SNAPSHOT) != 0) {
        fprintf(stderr, "dorado_web: failed to restore Smalltalk snapshot\n");
        dorado_machine_destroy(app.m);
        paste_queue.active = 0;
        app.m = NULL;
        return 1;
    }

    app.disp      = dorado_machine_display(app.m);
    app.mouse_buttons = 0;
    app.paused    = 0;
    app.announced = 1;
    app.frame     = 0;
    app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    printf("dorado_web: restored Smalltalk-76 desktop at cycle %llu\n",
           (unsigned long long)dorado_machine_cycles(app.m));
    return 0;
}

/* Console X-ray of the running machine (hot PCs per task, IFU dispatch
 * history): Module.ccall('dorado_web_debug') from the browser console. */
EMSCRIPTEN_KEEPALIVE
void dorado_web_debug(void)
{
    if (app.m) dorado_machine_debug(app.m);
}

/* Clipboard paste: web_shell.html's 'paste' listener ccalls this with the
 * clipboard text; the queue types it as paced synthetic keystrokes across
 * the following frames. */
EMSCRIPTEN_KEEPALIVE
int dorado_web_paste(const char *text)
{
    if (!app.m || !text || !*text) return 1;
    printf("dorado_web: pasting %d chars\n", (int)strlen(text));
    /* 800000 ~= the keyboard buffer's 3-field drain rate. */
    dorado_typequeue_start(&paste_queue, text, 800000ull,
                           dorado_machine_cycles(app.m));
    return 0;
}

/* One animation frame: advance the emulator, blit the display. Input
 * arrives asynchronously via dorado_web_key()/dorado_web_mouse(). */
static void frame(void)
{
    if (!app.paused) {
        uint64_t now = dorado_machine_cycles(app.m);
        dorado_typequeue_pump(&paste_queue, app.m);
        dorado_machine_run_until(app.m, now + app.cycles_per_frame);
        dorado_typequeue_pump(&paste_queue, app.m);
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
    /* Stay in fast-boot (large cycle chunks) while the world is still coming
     * up, then switch to a responsive chunk. This is keyboard-independent:
     * typing during the boot never slows the boot.
     *
     * "Painted something" is NOT the same as "is up". Mesa NetExec paints its
     * first herald line (~1,250 px) at ~30 M cycles but only reaches its `>`
     * prompt near 300 M; on the pixel test alone it dropped to 400 K
     * cycles/frame there and then crawled at ~4 M cycles/s, so the prompt
     * never arrived and the world looked hung -- a browser-only failure the
     * native CLI cannot show, because it has no throttle. Require a cycle
     * budget that covers every world's cold boot as well (Alto Executive
     * ~120 M, Alto NetExec ~100 M, Mesa NetExec ~300 M, Smalltalk ~630 M,
     * Cedar ~640 M; the Lyric and Cedar desktop entries restore a checkpoint
     * and start interactive already). */
    if (app.cycles_per_frame != WEB_CYCLES_INTERACTIVE) {
        /* Still booting: show progress so a slow world is visibly alive. */
        if ((app.frame & 0x0F) == 0)
            js_boot_progress((int)(dorado_machine_cycles(app.m) / 1000000ull));
        /* The cycle floor applies to a Mesa outload, whose herald lands
         * ~270 M cycles before its prompt. An Alto B-format world paints
         * when it is ready, so the pixel test alone still governs it and
         * the games stay at their normal pace. */
        if (rendered_px > 700 &&
            (!dorado_machine_boot_is_mesa_outload(app.m) ||
             dorado_machine_cycles(app.m) >= WEB_BOOT_FAST_CYCLES))
            app.cycles_per_frame = WEB_CYCLES_INTERACTIVE;
    }
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
