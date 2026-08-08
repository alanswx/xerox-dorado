/* Front panel for the SDL frontend: microui drawn with SDL_Renderer.
 *
 * WHY THIS EXISTS. Every host control used to be a stolen key chord --
 * Cmd/Ctrl+V to paste, Cmd/Ctrl+Q to quit, F1 to pause -- and each one is a
 * key the GUEST can then never have. That came to a head when a Cedar author
 * reported the middle button fighting between Cedar's "select word" and our
 * clipboard paste (docs/parc-feedback-todo.md sections J and K). A panel
 * takes the argument away: nothing needs a modifier the guest might want.
 *
 * It also exposes hardware the real machine had and a windowed user could
 * not reach at all -- most importantly the BOOT BUTTON, which is the single
 * most authentic control on a Dorado, and the green status LED the BaseBoard
 * has been modelling all along with nowhere to show it.
 *
 * WHY microui. The core emulator is C99 with no external dependencies and
 * SDL2 is already the frontend's one exception; microui is ~1,100 lines of
 * public-domain C that vendors beside fake6502.h and needs no build-system
 * change. It is immediate-mode, so the panel holds no widget state that
 * could drift out of step with the machine -- every frame is drawn from a
 * fresh dorado_machine_get_panel() sample.
 *
 * The renderer is deliberately small: microui emits a command list of rects,
 * text, icons and clips, and SDL_Renderer draws all four directly. The font
 * is microui's own 128x128 alpha atlas, uploaded once as a texture.
 */

#include <stdio.h>
#include <string.h>

#include <SDL.h>

#include "ui_panel.h"
#include "microui.h"
#include "atlas.inl"

/* ---- microui -> SDL_Renderer ------------------------------------------- */

static SDL_Renderer *ui_ren;
static SDL_Texture  *ui_atlas;

static int ui_text_width(mu_Font font, const char *text, int len)
{
    (void)font;
    if (len < 0) len = (int)strlen(text);
    int w = 0;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)text[i];
        if ((c & 0xc0) == 0x80) continue;       /* UTF-8 continuation */
        if (c >= 127) c = '?';
        w += atlas[ATLAS_FONT + c].w;
    }
    return w;
}

static int ui_text_height(mu_Font font)
{
    (void)font;
    return 18;
}

static void ui_set_color(mu_Color c)
{
    SDL_SetTextureColorMod(ui_atlas, c.r, c.g, c.b);
    SDL_SetTextureAlphaMod(ui_atlas, c.a);
    SDL_SetRenderDrawColor(ui_ren, c.r, c.g, c.b, c.a);
}

static void ui_blit(mu_Rect src, mu_Rect dst, mu_Color color)
{
    SDL_Rect s = { src.x, src.y, src.w, src.h };
    SDL_Rect d = { dst.x, dst.y, dst.w, dst.h };
    ui_set_color(color);
    SDL_RenderCopy(ui_ren, ui_atlas, &s, &d);
}

static void ui_draw_text(const char *text, mu_Vec2 pos, mu_Color color)
{
    mu_Rect dst = { pos.x, pos.y, 0, 0 };
    for (const char *p = text; *p; p++) {
        unsigned char c = (unsigned char)*p;
        if ((c & 0xc0) == 0x80) continue;
        if (c >= 127) c = '?';
        mu_Rect src = atlas[ATLAS_FONT + c];
        dst.w = src.w;
        dst.h = src.h;
        ui_blit(src, dst, color);
        dst.x += dst.w;
    }
}

static void ui_draw_icon(int id, mu_Rect rect, mu_Color color)
{
    mu_Rect src = atlas[id];
    int x = rect.x + (rect.w - src.w) / 2;
    int y = rect.y + (rect.h - src.h) / 2;
    mu_Rect dst = { x, y, src.w, src.h };
    ui_blit(src, dst, color);
}

/* ---- panel state ------------------------------------------------------- */

/* File-scope, like the key queue and the display-active cache: a
 * dorado_machine member would change the snapshot ABI and every baked
 * checkpoint would stop restoring. */
static mu_Context ui_ctx;
static int        ui_ready;

/* Activity lamps. Each holds a counter reading and a decay; the lamp is lit
 * while the decay runs, so a burst of disk sectors reads as a flicker rather
 * than a value that is already stale by the time it is drawn. */
typedef struct { uint64_t last; int decay; } ui_lamp;
static ui_lamp lamp_disk, lamp_net, lamp_run;

static void lamp_step(ui_lamp *l, uint64_t now)
{
    if (now != l->last) { l->last = now; l->decay = 6; }
    else if (l->decay > 0) l->decay--;
}

int dorado_ui_init(SDL_Renderer *ren)
{
    if (!ren) return -1;
    ui_ren = ren;

    /* microui's atlas is 8-bit alpha; SDL wants RGBA. */
    static Uint32 pixels[ATLAS_WIDTH * ATLAS_HEIGHT];
    for (int i = 0; i < ATLAS_WIDTH * ATLAS_HEIGHT; i++)
        pixels[i] = 0x00FFFFFFu | ((Uint32)atlas_texture[i] << 24);
    ui_atlas = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STATIC,
                                 ATLAS_WIDTH, ATLAS_HEIGHT);
    if (!ui_atlas) return -1;
    SDL_SetTextureBlendMode(ui_atlas, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(ui_atlas, NULL, pixels, ATLAS_WIDTH * (int)sizeof pixels[0]);

    mu_init(&ui_ctx);
    ui_ctx.text_width = ui_text_width;
    ui_ctx.text_height = ui_text_height;
    ui_ready = 1;
    return 0;
}

void dorado_ui_shutdown(void)
{
    if (ui_atlas) SDL_DestroyTexture(ui_atlas);
    ui_atlas = NULL;
    ui_ren = NULL;
    ui_ready = 0;
}

int dorado_ui_height(void) { return DORADO_UI_HEIGHT; }

/* Feed one SDL event to microui. Returns 1 when the panel consumed it, so
 * the caller does NOT also deliver it to the guest -- a click on the Boot
 * button must not also land in the Cedar viewer underneath. */
int dorado_ui_handle_event(const SDL_Event *e)
{
    if (!ui_ready) return 0;
    switch (e->type) {
    case SDL_MOUSEMOTION:
        mu_input_mousemove(&ui_ctx, e->motion.x, e->motion.y);
        return e->motion.y < DORADO_UI_HEIGHT;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
        if (e->button.y >= DORADO_UI_HEIGHT && e->type == SDL_MOUSEBUTTONDOWN)
            return 0;
        int b = e->button.button == SDL_BUTTON_LEFT   ? MU_MOUSE_LEFT
              : e->button.button == SDL_BUTTON_RIGHT  ? MU_MOUSE_RIGHT
              : e->button.button == SDL_BUTTON_MIDDLE ? MU_MOUSE_MIDDLE : 0;
        if (!b) return 0;
        if (e->type == SDL_MOUSEBUTTONDOWN)
            mu_input_mousedown(&ui_ctx, e->button.x, e->button.y, b);
        else
            mu_input_mouseup(&ui_ctx, e->button.x, e->button.y, b);
        return e->button.y < DORADO_UI_HEIGHT;
    }
    default:
        return 0;
    }
}

/* A message is shown for a few seconds after it CHANGES, then the readout
 * comes back. Tracked here rather than in the frontend so that every caller
 * that sets a message gets the behaviour without remembering to time it. */
#define UI_MESSAGE_MS 8000
static int ui_message_fresh(const char *msg)
{
    static char last[128];
    static Uint32 since;
    if (!msg || !msg[0]) return 0;
    if (strcmp(last, msg) != 0) {
        snprintf(last, sizeof last, "%s", msg);
        since = SDL_GetTicks();
    }
    return (Uint32)(SDL_GetTicks() - since) < UI_MESSAGE_MS;
}

/* Draw the panel and return which button, if any, the user pressed this
 * frame. One action per frame is plenty and keeps the caller a switch. */
dorado_ui_action dorado_ui_frame(const dorado_machine_panel *p,
                                 const dorado_ui_status *st,
                                 int window_w)
{
    if (!ui_ready) return DORADO_UI_NONE;
    dorado_ui_action action = DORADO_UI_NONE;

    lamp_step(&lamp_disk,  p->disk_activity);
    lamp_step(&lamp_net,   p->net_activity);
    lamp_step(&lamp_run,   p->uinstructions);

    mu_begin(&ui_ctx);
    if (mu_begin_window_ex(&ui_ctx, "panel",
                           mu_rect(0, 0, window_w, DORADO_UI_HEIGHT),
                           MU_OPT_NOTITLE | MU_OPT_NORESIZE |
                           MU_OPT_NOCLOSE | MU_OPT_NOSCROLL)) {
        /* One row: buttons, then lamps, then the honest speed readout. */
        static const int widths[] = { 60, 52, 56, 52, 72, 46, 46, 46, 46, -1 };
        mu_layout_row(&ui_ctx, 10, widths, 0);

        if (mu_button(&ui_ctx, st->paused ? "Run" : "Pause"))
            action = DORADO_UI_PAUSE;
        if (mu_button(&ui_ctx, "Boot"))   action = DORADO_UI_BOOT;
        if (mu_button(&ui_ctx, "Paste"))  action = DORADO_UI_PASTE;
        if (mu_button(&ui_ctx, "Save"))   action = DORADO_UI_SAVE;
        if (mu_button(&ui_ctx, "Add file")) action = DORADO_UI_ADDFILE;

        /* Lamps.
         *
         * PWR is the machine's REAL status LED -- the BaseBoard's MiscByte
         * bit 7, driven by the 6502 from two three-instruction routines in
         * the ROM (F37D sets it, F38C clears it). Traced over a boot it
         * BLINKS, about every 313,000 BaseBoard cycles, and then stops: the
         * BaseBoard is deliberately suppressed once the machine is up (a
         * 19.7% speedup), so the lamp freezes at whatever phase it was in --
         * measured, the last of 187 transitions is at 145 M cycles of a
         * 900 M run.
         *
         * That is faithful but useless as "is it alive?", so RUN answers
         * that separately: it follows the microinstruction counter, which
         * only advances when the machine is actually executing. */
        dorado_ui_lamp(&ui_ctx, "RUN", lamp_run.decay > 0, 0x40, 0xE0, 0x50);
        dorado_ui_lamp(&ui_ctx, "PWR", p->lamp_on, 0x40, 0xE0, 0x50);
        dorado_ui_lamp(&ui_ctx, "DSK", lamp_disk.decay > 0, 0xF0, 0xB0, 0x40);
        dorado_ui_lamp(&ui_ctx, "NET", lamp_net.decay > 0, 0x60, 0xB0, 0xF0);

        /* The last cell is either the machine's vitals or the last thing
         * that happened, for a few seconds.
         *
         * It used to be BOTH, with the message on a second row -- which was
         * outside the 30-pixel band and therefore clipped away. A file
         * dropped on the window was copied, served, and reported into a
         * string nobody could see; the user concluded the drop had done
         * nothing, which is exactly what a silent success looks like. If a
         * panel writes a message, the panel has to show it. */
        char line[200];
        if (ui_message_fresh(st->message)) {
            snprintf(line, sizeof line, "%s", st->message);
        } else {
            snprintf(line, sizeof line, "%s   %.2fx real   %llu uinstr",
                     st->world && st->world[0] ? st->world : "Dorado",
                     st->speed_ratio,
                     (unsigned long long)p->uinstructions);
        }
        mu_label(&ui_ctx, line);
        mu_end_window(&ui_ctx);
    }
    mu_end(&ui_ctx);

    /* Render the command list. */
    SDL_Rect full = { 0, 0, window_w, DORADO_UI_HEIGHT };
    SDL_RenderSetClipRect(ui_ren, &full);
    SDL_SetRenderDrawColor(ui_ren, 26, 28, 32, 255);
    SDL_RenderFillRect(ui_ren, &full);
    SDL_SetRenderDrawBlendMode(ui_ren, SDL_BLENDMODE_BLEND);

    mu_Command *cmd = NULL;
    while (mu_next_command(&ui_ctx, &cmd)) {
        switch (cmd->type) {
        case MU_COMMAND_TEXT:
            ui_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
            break;
        case MU_COMMAND_RECT: {
            SDL_Rect r = { cmd->rect.rect.x, cmd->rect.rect.y,
                           cmd->rect.rect.w, cmd->rect.rect.h };
            ui_set_color(cmd->rect.color);
            SDL_RenderFillRect(ui_ren, &r);
            break;
        }
        case MU_COMMAND_ICON:
            ui_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
            break;
        case MU_COMMAND_CLIP: {
            SDL_Rect r = { cmd->clip.rect.x, cmd->clip.rect.y,
                           cmd->clip.rect.w, cmd->clip.rect.h };
            /* microui's "no clip" is a huge rect; keep it inside the band so
             * the panel can never paint over the guest's display. */
            if (r.y < 0) { r.h += r.y; r.y = 0; }
            if (r.h > DORADO_UI_HEIGHT - r.y) r.h = DORADO_UI_HEIGHT - r.y;
            if (r.h < 0) r.h = 0;
            SDL_RenderSetClipRect(ui_ren, &r);
            break;
        }
        default: break;
        }
    }
    SDL_RenderSetClipRect(ui_ren, NULL);
    return action;
}

/* A round-ish indicator with a caption, drawn straight into the command list
 * so it participates in microui's clipping like any widget. */
void dorado_ui_lamp(mu_Context *ctx, const char *name, int on,
                    int r, int g, int b)
{
    mu_Rect box = mu_layout_next(ctx);
    int d = 9;
    mu_Rect dot = mu_rect(box.x + 2, box.y + (box.h - d) / 2, d, d);
    mu_Color c = on ? mu_color(r, g, b, 255)
                    : mu_color((r * 55) / 255, (g * 55) / 255,
                               (b * 55) / 255, 255);
    mu_draw_rect(ctx, dot, c);
    /* Left-aligned is microui's default: it has ALIGNCENTER and ALIGNRIGHT
     * options and no ALIGNLEFT, because 0 already means left. */
    mu_draw_control_text(ctx, name,
                         mu_rect(box.x + d + 4, box.y, box.w - d - 4, box.h),
                         MU_COLOR_TEXT, 0);
}
