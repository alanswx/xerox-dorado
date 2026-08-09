#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "dispm.h"

/* Host mouse motion is an input queue, not machine state. Keep it outside
 * dorado_display because that struct is part of the snapshot ABI. */
static int display_mouse_dx, display_mouse_dy;
static int display_mouse_pending;
static uint16_t display_mouse_body;

static int display_trace_limit(const char *name, unsigned default_limit,
                               unsigned *limit)
{
    /* This is a HOT path -- called once per display word the DDC fetches,
     * via dorado_display_iofetch_word. A raw getenv() here was costing ~2%
     * of total runtime (sample, 2026-08-04): exactly the trap
     * dorado/CLAUDE.md warns about ("Never a raw getenv() in a hot path"),
     * sitting in the one file that already carries the warning.
     *
     * dorado_trace_flag() is the inlined "is ANY DORADO_* variable set"
     * test, so with a clean environment this returns immediately and the
     * getenv disappears. Only when some DORADO_* is set do we pay a real
     * getenv to read the limit VALUE -- which dorado_trace_flag cannot give
     * us, because it memoizes booleans keyed by string-literal pointer.
     * Behaviour is identical: an unset variable answered 0 before too. */
    if (!dorado_trace_flag(name))
        return 0;
    const char *env = getenv(name);
    if (!env || !*env || *env == '0')
        return 0;
    char *end = NULL;
    unsigned long parsed = strtoul(env, &end, 0);
    *limit = (end && *end == '\0' && parsed > 1)
        ? (unsigned)parsed
        : default_limit;
    return 1;
}

static int display_dispm_present(void)
{
    /* DisplayInitConfig's presence probe is the same board-selection fact as
     * the EMULATOR-task DispM device. Keep one source of truth: the board is
     * present iff machine creation installed it. This removes a getenv from
     * a hot I/O path and prevents the old PRESENT/COLOR env split. */
    return dorado_dispm_installed() != DORADO_DISPM_NONE;
}

static uint16_t display_ddc_status(void)
{
    static int cached = -1;
    static uint16_t value = 0;
    if (cached < 0) {
        const char *v = getenv("DORADO_DDCSTATUS");
        cached = 1;
        if (v && *v) {
            char *end = NULL;
            unsigned long parsed = strtoul(v, &end, 0);
            if (end && *end == '\0')
                value = (uint16_t)parsed;
        }
    }
    return value;
}

typedef struct {
    int word;
    uint16_t mask;
} display_key_map;

static const display_key_map key_map[DORADO_KEY_LAST] = {
    [DORADO_KEY_NONE]        = { -1, 0x0000 },
    [DORADO_KEY_5]           = { 0, 0x8000 },
    [DORADO_KEY_4]           = { 0, 0x4000 },
    [DORADO_KEY_6]           = { 0, 0x2000 },
    [DORADO_KEY_E]           = { 0, 0x1000 },
    [DORADO_KEY_7]           = { 0, 0x0800 },
    [DORADO_KEY_D]           = { 0, 0x0400 },
    [DORADO_KEY_U]           = { 0, 0x0200 },
    [DORADO_KEY_V]           = { 0, 0x0100 },
    [DORADO_KEY_0]           = { 0, 0x0080 },
    [DORADO_KEY_K]           = { 0, 0x0040 },
    [DORADO_KEY_MINUS]       = { 0, 0x0020 },
    [DORADO_KEY_P]           = { 0, 0x0010 },
    [DORADO_KEY_FSLASH]      = { 0, 0x0008 },
    [DORADO_KEY_BSLASH]      = { 0, 0x0004 },
    [DORADO_KEY_LF]          = { 0, 0x0002 },
    [DORADO_KEY_BS]          = { 0, 0x0001 },
    [DORADO_KEY_3]           = { 1, 0x8000 },
    [DORADO_KEY_2]           = { 1, 0x4000 },
    [DORADO_KEY_W]           = { 1, 0x2000 },
    [DORADO_KEY_Q]           = { 1, 0x1000 },
    [DORADO_KEY_S]           = { 1, 0x0800 },
    [DORADO_KEY_A]           = { 1, 0x0400 },
    [DORADO_KEY_9]           = { 1, 0x0200 },
    [DORADO_KEY_I]           = { 1, 0x0100 },
    [DORADO_KEY_X]           = { 1, 0x0080 },
    [DORADO_KEY_O]           = { 1, 0x0040 },
    [DORADO_KEY_L]           = { 1, 0x0020 },
    [DORADO_KEY_COMMA]       = { 1, 0x0010 },
    [DORADO_KEY_QUOTE]       = { 1, 0x0008 },
    [DORADO_KEY_RBRACKET]    = { 1, 0x0004 },
    [DORADO_KEY_BLANKMIDDLE] = { 1, 0x0002 },
    [DORADO_KEY_BLANKTOP]    = { 1, 0x0001 },
    [DORADO_KEY_1]           = { 2, 0x8000 },
    [DORADO_KEY_ESC]         = { 2, 0x4000 },
    [DORADO_KEY_TAB]         = { 2, 0x2000 },
    [DORADO_KEY_F]           = { 2, 0x1000 },
    [DORADO_KEY_CTRL]        = { 2, 0x0800 },
    [DORADO_KEY_C]           = { 2, 0x0400 },
    [DORADO_KEY_J]           = { 2, 0x0200 },
    [DORADO_KEY_B]           = { 2, 0x0100 },
    [DORADO_KEY_Z]           = { 2, 0x0080 },
    [DORADO_KEY_LSHIFT]      = { 2, 0x0040 },
    [DORADO_KEY_PERIOD]      = { 2, 0x0020 },
    [DORADO_KEY_SEMICOLON]   = { 2, 0x0010 },
    [DORADO_KEY_RETURN]      = { 2, 0x0008 },
    [DORADO_KEY_ARROW]       = { 2, 0x0004 },
    [DORADO_KEY_DEL]         = { 2, 0x0002 },
    [DORADO_KEY_R]           = { 3, 0x8000 },
    [DORADO_KEY_T]           = { 3, 0x4000 },
    [DORADO_KEY_G]           = { 3, 0x2000 },
    [DORADO_KEY_Y]           = { 3, 0x1000 },
    [DORADO_KEY_H]           = { 3, 0x0800 },
    [DORADO_KEY_8]           = { 3, 0x0400 },
    [DORADO_KEY_N]           = { 3, 0x0200 },
    [DORADO_KEY_M]           = { 3, 0x0100 },
    [DORADO_KEY_LOCK]        = { 3, 0x0080 },
    [DORADO_KEY_SPACE]       = { 3, 0x0040 },
    [DORADO_KEY_LBRACKET]    = { 3, 0x0020 },
    [DORADO_KEY_PLUS]        = { 3, 0x0010 },
    [DORADO_KEY_RSHIFT]      = { 3, 0x0008 },
    [DORADO_KEY_BLANKBOTTOM] = { 3, 0x0004 },
};

/*
 * Display Controller — minimal Phase 1 model.
 *
 * Goal: be a permissive sink for slow-IO writes from DDC microcode so
 * probe_aemu and probe_full_boot can run display-touching code without
 * halting; record state into a debug-friendly form; let synthetic
 * tests poke pixels into the framebuffer and snapshot the result.
 *
 * Real DDC behavior (HM §11) is much richer — pixel clock, mixer
 * modes, A8B2/BBypass/24Bit, MiniMixer, 7-wire interface, back
 * channel — and will land in later phases.
 */

void dorado_display_init(dorado_display *d)
{
    memset(d, 0, sizeof *d);
    display_mouse_dx = display_mouse_dy = 0;
    display_mouse_pending = 0;
    display_mouse_body = 0;
    d->fifo_a_head = d->fifo_a_tail = 0;
    d->fifo_b_head = d->fifo_b_tail = 0;
    /* DisplayDefs.mc defines Statics bit 0 as DHTShutUp and bit 1
     * as DWTShutUp. IOReset leaves the display quiet until THT init
     * explicitly writes Statics=0 near the end of initialization. */
    d->statics = DORADO_DISPLAY_STATICS_ALL_SHUTUP;
    /* HM §11: Keep' is a flipflop that defaults to "video owns" (true).
     * Dorado must take ownership before loading RAM. */
    d->ram_keep = 1;
    dorado_display_keyboard_all_up(d);
}

/* ─── Slow-IO catch-all callbacks ────────────────────────────────── */

void dorado_display_keyboard_all_up(dorado_display *d)
{
    if (!d) return;
    for (int i = 0; i < DORADO_DISPLAY_KEY_WORDS; i++) {
        d->keyboard_words[i] = 0xFFFFu;
    }
}

void dorado_display_keyboard_set_word(dorado_display *d, int word,
                                      uint16_t value)
{
    if (!d || word < 0 || word >= DORADO_DISPLAY_KEY_WORDS) return;
    d->keyboard_words[word] = value;
}

uint16_t dorado_display_keyboard_word(const dorado_display *d, int word)
{
    if (!d || word < 0 || word >= DORADO_DISPLAY_KEY_WORDS) return 0xFFFFu;
    return d->keyboard_words[word];
}

void dorado_display_keyboard_set_bit(dorado_display *d, int word,
                                     int bit, int down)
{
    if (!d || word < 0 || word >= DORADO_DISPLAY_KEY_WORDS) return;
    if (bit < 0 || bit >= 16) return;
    uint16_t mask = (uint16_t)(1u << bit);
    if (down) d->keyboard_words[word] &= (uint16_t)~mask;
    else      d->keyboard_words[word] |= mask;
}

void dorado_display_keyboard_set_key(dorado_display *d,
                                     dorado_display_key key,
                                     int down)
{
    if (!d || key <= DORADO_KEY_NONE || key >= DORADO_KEY_LAST) return;
    int word = key_map[key].word;
    uint16_t mask = key_map[key].mask;
    if (word < 0 || word >= DORADO_DISPLAY_KEY_WORDS || mask == 0) return;
    if (down) d->keyboard_words[word] &= (uint16_t)~mask;
    else      d->keyboard_words[word] |= mask;
}

/* Boot-key chord name -> dorado_display_key. Covers the keys relevant to
 * the Dorado/Alto boot-reason chord (HM/Booting memo: BS selects the
 * Ethernet software boot, Quote selects NetExec) plus the common control
 * keys; single-character names fall through to char_to_key-style mapping. */
dorado_display_key dorado_display_key_from_name(const char *name)
{
    if (!name || !name[0]) return DORADO_KEY_NONE;
    static const struct { const char *n; dorado_display_key k; } names[] = {
        { "bs",     DORADO_KEY_BS },
        { "quote",  DORADO_KEY_QUOTE },
        { "ctrl",   DORADO_KEY_CTRL },
        { "shift",  DORADO_KEY_LSHIFT },
        { "lshift", DORADO_KEY_LSHIFT },
        { "rshift", DORADO_KEY_RSHIFT },
        { "lock",   DORADO_KEY_LOCK },
        { "return", DORADO_KEY_RETURN },
        { "esc",    DORADO_KEY_ESC },
        { "tab",    DORADO_KEY_TAB },
        { "del",    DORADO_KEY_DEL },
        { "lf",     DORADO_KEY_LF },
        { "space",  DORADO_KEY_SPACE },
        { "arrow",  DORADO_KEY_ARROW },
    };
    for (size_t i = 0; i < sizeof names / sizeof names[0]; i++)
        if (!strcmp(name, names[i].n)) return names[i].k;

    /* Single character: letter or digit. */
    if (name[1] == '\0') {
        char c = name[0];
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
        switch (c) {
        case '0': return DORADO_KEY_0; case '1': return DORADO_KEY_1;
        case '2': return DORADO_KEY_2; case '3': return DORADO_KEY_3;
        case '4': return DORADO_KEY_4; case '5': return DORADO_KEY_5;
        case '6': return DORADO_KEY_6; case '7': return DORADO_KEY_7;
        case '8': return DORADO_KEY_8; case '9': return DORADO_KEY_9;
        case 'a': return DORADO_KEY_A; case 'b': return DORADO_KEY_B;
        case 'c': return DORADO_KEY_C; case 'd': return DORADO_KEY_D;
        case 'e': return DORADO_KEY_E; case 'f': return DORADO_KEY_F;
        case 'g': return DORADO_KEY_G; case 'h': return DORADO_KEY_H;
        case 'i': return DORADO_KEY_I; case 'j': return DORADO_KEY_J;
        case 'k': return DORADO_KEY_K; case 'l': return DORADO_KEY_L;
        case 'm': return DORADO_KEY_M; case 'n': return DORADO_KEY_N;
        case 'o': return DORADO_KEY_O; case 'p': return DORADO_KEY_P;
        case 'q': return DORADO_KEY_Q; case 'r': return DORADO_KEY_R;
        case 's': return DORADO_KEY_S; case 't': return DORADO_KEY_T;
        case 'u': return DORADO_KEY_U; case 'v': return DORADO_KEY_V;
        case 'w': return DORADO_KEY_W; case 'x': return DORADO_KEY_X;
        case 'y': return DORADO_KEY_Y; case 'z': return DORADO_KEY_Z;
        default: break;
        }
    }
    return DORADO_KEY_NONE;
}

void dorado_display_boot_button(dorado_display *d, uint32_t scanlines)
{
    if (!d) return;
    d->boot_button_scanlines = scanlines;
}

/* ---- Mouse MOTION on the terminal back channel (HM Table 24 msg 06B) ----
 *
 * The terminal microcomputer reports the mouse as DELTAS in excess-200B, not
 * as a position. That distinction is load-bearing, not pedantry: Cedar's
 * InterminalImpl is the accumulator that turns motion into a position, and it
 * keeps a SEPARATE 0-based coordinate space per screen (color^ is [xMin: 0,
 * xMax: colorWidth-1], right^/left^ are [xMin: 0, xMax: bwWidth-1]) with
 * crossing detected by the pointer pushing past an edge by more than an
 * escape threshold. Inject an absolute position instead -- which is what
 * machine.c does at 0424/0425 -- and there is no input shape in which a
 * second screen can ever work: the value is clamped into whichever display is
 * current, and any attempt to reach past its xMax is either clamped or
 * triggers a crossing that immediately resets the position to the new
 * screen's origin.
 *
 * File-scope statics, not dorado_display members: the display struct is
 * snapshotted whole and a new member breaks every baked checkpoint. */
void dorado_display_mouse_delta(dorado_display *d, int dx, int dy)
{
    (void)d;
    if (!dx && !dy) return;
    display_mouse_dx += dx;
    display_mouse_dy += dy;
    display_mouse_pending = 1;
}

int dorado_display_take_mouse_delta(dorado_display *d, int *dx, int *dy)
{
    (void)d;
    if (!dx || !dy || !display_mouse_pending)
        return 0;
    *dx = display_mouse_dx;
    *dy = display_mouse_dy;
    display_mouse_dx = display_mouse_dy = 0;
    display_mouse_pending = 0;
    /* Kept as a low-level test/helper drain; the running Dorado path lets
     * DisplayAux.mc consume the pending motion through ReadTerminal. */
    display_mouse_body = 0;
    return *dx != 0 || *dy != 0;
}

static uint16_t display_terminal_keyboard_bit(dorado_display *d)
{
    if (!d) return 0;

    uint8_t word = (uint8_t)(d->terminal_msg_word % DORADO_DISPLAY_KEY_WORDS);
    uint8_t type = (word < 4u) ? (uint8_t)(word + 1u) : 5u;
    uint16_t body = d->keyboard_words[word];

    /* A pending motion pre-empts the keyboard rotation at a message boundary.
     * Table 24: type 06B, body = dx and dy each biased by 200B into a byte.
     * Clamp to what a byte can carry and keep the remainder for the next
     * message, so a fast host movement becomes several small deltas rather
     * than one wrapped one. */
    if (display_mouse_pending && d->terminal_msg_bit == 0) {
        int dx = display_mouse_dx, dy = display_mouse_dy;
        if (dx >  127) dx =  127; else if (dx < -128) dx = -128;
        if (dy >  127) dy =  127; else if (dy < -128) dy = -128;
        display_mouse_dx -= dx;
        display_mouse_dy -= dy;
        if (!display_mouse_dx && !display_mouse_dy) display_mouse_pending = 0;
        type = 6u;
        body = (uint16_t)((((unsigned)(dx + 0200) & 0377u) << 8) |
                           ((unsigned)(dy + 0200) & 0377u));
        uint32_t m = (1u << 31) | ((uint32_t)type << 24) |
                     ((uint32_t)body << 8) | (1u << 7);
        uint16_t b0 = (uint16_t)((m >> 31) & 1u);
        if (dorado_trace_flag("DORADO_MOUSE_DELTA_TRACE"))
            fprintf(stderr, "TMOUSE dx=%d dy=%d body=%06o\n", dx, dy, body);
        d->terminal_bits++;
        d->terminal_msg_bit = 1;
        display_mouse_body = body;       /* rest of this message uses it */
        return b0 ? 0x8000u : 0u;
    }
    if (d->terminal_msg_bit != 0 && display_mouse_body) {
        type = 6u;
        body = display_mouse_body;
    }
    uint32_t msg = (1u << 31) | ((uint32_t)type << 24) |
                   ((uint32_t)body << 8) | (1u << 7);
    uint16_t bit = (uint16_t)((msg >> (31u - (d->terminal_msg_bit & 31u))) & 1u);
    if (dorado_trace_flag("DORADO_TSTATUS_TRACE")) {
        fprintf(stderr,
                "[tstatus] word=%u type=%u bit=%u val=%u body=%06o "
                "bits=%llu msgs=%llu\n",
                word, type, d->terminal_msg_bit, bit, body,
                (unsigned long long)d->terminal_bits,
                (unsigned long long)d->terminal_messages);
    }

    d->terminal_bits++;
    d->terminal_msg_bit++;
    if (d->terminal_msg_bit >= 32u) {
        d->terminal_msg_bit = 0;
        if (display_mouse_body) {
            display_mouse_body = 0;       /* motion message complete */
        } else {
            d->terminal_msg_word =
                (uint8_t)((word + 1u) % DORADO_DISPLAY_KEY_WORDS);
        }
        d->terminal_messages++;
    }
    /* HM Table 25 / DispY18,21, DispM10,21: the terminal serial back-
     * channel bit (OISRcvdData) is gated onto IOB.00 (bit 0 = MSB),
     * not the muffler line IOB.15. ReadTerminal (DisplayAux.mc) reads
     * "Data = IOB[0]" and LCY-accumulates the 32-bit message MSB-first,
     * so the live bit must sit at 0x8000. */
    return bit ? 0x8000u : 0u;
}

static int ddc_decode_width_pixels(uint16_t raw)
{
    int pixels = (int)((0u - (uint32_t)(raw & 0x0FFFu)) & 0x0FFFu) - 255;
    if (pixels < 0) pixels = 0;
    if (pixels > DORADO_DISPLAY_W) pixels = DORADO_DISPLAY_W;
    return pixels;
}

static void display_draw_word(dorado_display *d, int x, int y, uint16_t word);

static void ddc_snapshot_line(dorado_display *d, int channel)
{
    dorado_display_ddc_line *line = &d->ddc_pending_line[channel & 1];
    int base = channel ? 010 : 0;

    memset(line, 0, sizeof *line);
    line->valid = 1;
    line->odd = (uint8_t)(d->nlcb_field_odd & 1u);
    line->line = d->nlcb_line;
    line->lmarg = d->nlcb[0][base + 001] & 0x0FFFu;
    line->width = d->nlcb[0][base + 002] & 0x0FFFu;
    line->ptr = d->nlcb[0][base + 003] & 0x0FFFu;
    line->scan = d->nlcb[0][base + 004] & 0x0FFFu;
}

static void ddc_start_line(dorado_display *d, int channel, int y)
{
    dorado_display_ddc_line *cur = &d->ddc_current_line[channel & 1];
    *cur = d->ddc_pending_line[channel & 1];
    cur->absolute_y = 1;
    cur->y = (uint16_t)y;
    cur->word_count = 0;
    cur->overflow = 0;
}

static void ddc_render_line(dorado_display *d, int channel)
{
    dorado_display_ddc_line *line = &d->ddc_current_line[channel & 1];
    uint16_t nonzero_words = 0;
    if (!line->valid || line->word_count == 0)
        return;

    int width_pixels = ddc_decode_width_pixels(line->width);
    if (width_pixels <= 0)
        return;

    int y = line->absolute_y
        ? (int)line->y
        : (int)line->line * 2 + (int)(line->odd & 1u);
    if (y < 0 || y >= DORADO_DISPLAY_H)
        return;

    int x = 0;
    int visible_words = (width_pixels + 15) / 16;
    int start = (int)(line->ptr & 017u);
    int line_pixels = 0;

    for (int i = 0; i < line->word_count; i++) {
        if (line->words[i])
            nonzero_words++;
    }
    d->ddc_lines_rendered++;
    d->ddc_last_width = line->width;
    d->ddc_last_ptr = line->ptr;
    d->ddc_last_line = line->line;
    d->ddc_last_word_count = line->word_count;
    d->ddc_last_nonzero_words = nonzero_words;
    d->ddc_last_overflow = line->overflow;
    if (nonzero_words == 0)
        d->ddc_zero_word_lines++;
    if (line->word_count < (uint16_t)(start + visible_words))
        d->ddc_short_lines++;

    for (int px = 0; px < width_pixels && x + px < DORADO_DISPLAY_W; px++)
        dorado_display_set_pixel(d, x + px, y, 0);

    for (int i = 0; i < visible_words && x < DORADO_DISPLAY_W; i++) {
        int src = start + i;
        uint16_t word = (src < (int)line->word_count) ? line->words[src] : 0;
        int remaining = width_pixels - (i * 16);
        if (remaining > 16) remaining = 16;
        for (int b = 0; b < remaining && x < DORADO_DISPLAY_W; b++, x++) {
            int bit = (word >> (15 - b)) & 1;
            dorado_display_set_pixel(d, x, y, bit);
            if (bit)
                line_pixels++;
        }
    }
    d->ddc_pixels_rendered += (uint64_t)line_pixels;
}

/* Phase 1: a single permissive output handler that records the
 * (task, tioa, data) triple but does nothing semantically. The
 * real DDC has six (DispY) or eight (DispM) output devices, each
 * decoded by TIOA. We don't yet know the numeric TIOA values
 * microcode uses; we'll fill them in once we trace probe_aemu's
 * display-task writes. For now everything just gets logged. */
static void display_output_b(void *ctx, int task, int subtask,
                             uint8_t tioa, uint16_t data)
{
    dorado_display *d = ctx;
    int t = task & 0xF;
    uint8_t old_ram_keep = d->ram_keep;

    d->output_count++;
    d->output_task_count[t]++;
    if (d->output_tioa_count[tioa] == 0)
        d->output_tioa_first[tioa] = data;
    d->output_tioa_count[tioa]++;
    d->output_tioa_last[tioa] = data;
    d->riob = data;     /* HM page 119: IOB stays in DDC RIOB until
                         * next output command */
    if ((t == DORADO_DISPLAY_TASK_DHT || t == DORADO_DISPLAY_TASK_AHT) &&
        (tioa == DORADO_DISPLAY_TIOA_STATICS ||
         tioa == DORADO_DISPLAY_TIOA_TSTATICS)) {
        if (d->terminal_task == 0) d->terminal_task = t;
    }
    if (tioa == DORADO_DISPLAY_TIOA_STATICS ||
        tioa == DORADO_DISPLAY_TIOA_TSTATICS) {
        d->statics = data;
        if (data & DORADO_DISPLAY_STATICS_DWT_SHUTUP) {
            memset(d->next_wcb_flag, 0, sizeof d->next_wcb_flag);
            memset(d->current_wcb_flag, 0, sizeof d->current_wcb_flag);
            memset(d->ddc_pending_line, 0, sizeof d->ddc_pending_line);
            memset(d->ddc_current_line, 0, sizeof d->ddc_current_line);
            d->fifo_a_head = d->fifo_a_tail = 0;
            d->fifo_b_head = d->fifo_b_tail = 0;
        }
        if (data & DORADO_DISPLAY_STATICS_DHT_SHUTUP) {
            d->raster_lt_enabled = 0;
            memset(d->raster_next_wt_flag, 0, sizeof d->raster_next_wt_flag);
            memset(d->raster_current_wt_flag, 0,
                   sizeof d->raster_current_wt_flag);
        }
    } else if (tioa == DORADO_DISPLAY_TIOA_HRAM) {
        /* HM §10.7/§10.9: HRam uses the DDC RAM-loading protocol.
         * Keep' is latched by every command; while it is low, Dorado
         * owns the RAM. LoadAddr and Write' take effect only if the
         * previous command already had Keep' low. Write' is active low
         * and auto-increments the Dorado-side address. */
        if (!old_ram_keep) {
            if (data & DORADO_DISPLAY_RAM_LOADADDR) {
                d->ram_addr = (uint16_t)(data & (DORADO_DISPLAY_HRAM_WORDS - 1));
            }
            if (!(data & DORADO_DISPLAY_RAM_WRITE)) {
                d->hram[d->ram_addr & (DORADO_DISPLAY_HRAM_WORDS - 1)] =
                    (uint8_t)(data & 7u);
                d->ram_addr =
                    (uint16_t)((d->ram_addr + 1u) & (DORADO_DISPLAY_HRAM_WORDS - 1));
            }
        }
        d->ram_keep = (data & DORADO_DISPLAY_RAM_KEEP) ? 1u : 0u;
    }

    /* DisplayMain.mc:
     *   DHT/THT writes ANextWCBFlag or BNextWCBFlag to DHTFlag/AHTFlag.
     *   DWT/TWT writes 1 when starting a scan line and 0 when it is
     *   exhausted. Model those WCB flags independent of whether terminal
     *   emulation is using DispY (DHT/DWT) or DispM (AHT/AWT).
     */
    if (tioa == DORADO_DISPLAY_TIOA_DHTFLAG ||
        tioa == DORADO_DISPLAY_TIOA_AHTFLAG) {
        if ((data & 0002u) && dorado_trace_flag("DORADO_WCB_TRACE")) {
            static unsigned wcb_trace_count = 0;
            if (wcb_trace_count++ < 800) {
                fprintf(stderr,
                        "[display] WCB A line=%u odd=%u lmarg=%04o "
                        "width=%04o ptr=%04o scan=%04o raw=%06o\n",
                        d->nlcb_line, d->nlcb_field_odd,
                        d->nlcb[0][001], d->nlcb[0][002],
                        d->nlcb[0][003], d->nlcb[0][004], data);
            }
        }
        if (task == d->terminal_task) {
            if (data & 0002u) {
                ddc_snapshot_line(d, 0);
                d->next_wcb_flag[0] = 1;
            }
        } else {
            if (data & 0002u) {
                ddc_snapshot_line(d, 0);
                d->next_wcb_flag[0] = 1;
            }
            if (data & 0004u) {
                ddc_snapshot_line(d, 1);
                d->next_wcb_flag[1] = 1;
            }
        }
        d->nlcb_writes++;
    } else if (tioa == DORADO_DISPLAY_TIOA_DWTFLAG ||
               tioa == DORADO_DISPLAY_TIOA_AWTFLAG) {
        /* DispY15/27 DWT-command decode (DisplayMain.mc DWTStart /
         * DWTAdjustWCBFlags). The DWT loop issues exactly three values:
         *   Output_ 20 (0020) -- IOFetch pacing pulse ("a new munch is on
         *                        its way"): do NOT touch the WCB flags.
         *   Output_ 1  (0001) -- start of scan line: set CurrentWCB and
         *                        clear NextWCB.
         *   Output_ 0  (0000) -- scan line exhausted: clear CurrentWCB.
         * Previously this did current = (data & 1), so every IOFetch
         * pulse (0020, bit 0 clear) wrongly cleared CurrentWCB (QW2). */
        int channel = (subtask & 2) ? 1 : 0;
        if (data & 0020u) {
            /* IOFetch pacing: leave WCB flags untouched. */
        } else if (data & 0001u) {
            int draw_y = (int)d->scan_line;
            if (d->nlcb_line > 0) {
                draw_y = ((int)d->nlcb_line - 1) * 2 +
                         (int)(d->nlcb_field_odd & 1u);
            }
            if (dorado_trace_flag("DORADO_DDC_RENDER_SEQ")) {
                draw_y = d->ddc_seq_y++;
                if (d->ddc_seq_y >= DORADO_DISPLAY_H)
                    d->ddc_seq_y = 0;
            }
            d->current_wcb_flag[channel] = 1;
            d->next_wcb_flag[channel] = 0;
            d->wcb_draw_x[channel] = 0;
            d->wcb_draw_y[channel] = (uint16_t)draw_y;
            ddc_start_line(d, channel, draw_y);
            d->dwt_trace_active[channel] = 1;
            d->dwt_trace_words[channel] = 0;
            d->dwt_trace_nonzero[channel] = 0;
            d->dwt_trace_first_va[channel] = 0;
            d->dwt_trace_last_va[channel] = 0;
        } else {
            unsigned dwt_trace_limit = 0;
            static unsigned dwt_trace_lines_this_process = 0;
            if (d->dwt_trace_active[channel] &&
                display_trace_limit("DORADO_DWT_TRACE", 400,
                                    &dwt_trace_limit) &&
                dwt_trace_lines_this_process < dwt_trace_limit) {
                fprintf(stderr,
                        "[display] DWT line %llu ch=%d scan=%u words=%u "
                        "nonzero=%u first=0x%05x/0o%o last=0x%05x/0o%o\n",
                        (unsigned long long)d->dwt_trace_lines, channel,
                        d->scan_line, d->dwt_trace_words[channel],
                        d->dwt_trace_nonzero[channel],
                        d->dwt_trace_first_va[channel],
                        d->dwt_trace_first_va[channel],
                        d->dwt_trace_last_va[channel],
                        d->dwt_trace_last_va[channel]);
                dwt_trace_lines_this_process++;
            }
            if (d->dwt_trace_active[channel])
                d->dwt_trace_lines++;
            if (!dorado_trace_flag("DORADO_DDC_RENDER_DIRECT") &&
                !dorado_trace_flag("DORADO_DDC_RENDER_VA")) {
                ddc_render_line(d, channel);
            }
            d->dwt_trace_active[channel] = 0;
            d->current_wcb_flag[channel] = 0;
        }
    } else if (tioa == DORADO_DISPLAY_TIOA_RAST_TASKCMD) {
        d->raster_taskcmd = data;
        if (t == DORADO_DISPLAY_TASK_DHT) {
            d->raster_lt_enabled =
                (data & DORADO_DISPLAY_RAST_LT_SHUTUP) ? 0u : 1u;
            if (data & DORADO_DISPLAY_RAST_CHAN_WANTS_WT) {
                int ch = d->raster_next_channel & 3;
                d->raster_next_wt_flag[ch] = 1;
                d->raster_next_channel = (uint8_t)((ch + 1) & 3);
            }
        } else if (t == DORADO_DISPLAY_TASK_DWT) {
            int ch = subtask & 3;
            if (data & 1u) {
                d->raster_current_wt_flag[ch] = 0;
            }
            if (data & DORADO_DISPLAY_RAST_WT_SHUTUP) {
                d->raster_next_wt_flag[ch] = 0;
                d->raster_current_wt_flag[ch] = 0;
            }
        }
    }
    if (tioa == DORADO_DISPLAY_TIOA_NLCB ||
        tioa == DORADO_DISPLAY_TIOA_TNLCB) {
        /* DispY next-line control block (DisplayDefs.mc NLCB address
         * constants): register select = data[0:3] (top 4 bits). */
        unsigned reg = (data >> 12) & 0xF;
        unsigned val = data & 0x0FFF;
        if (reg < DORADO_DISPLAY_NLCB_WORDS)
            d->nlcb[0][reg] = (uint16_t)val;
        /* dorado_trace_flag, not raw getenv: this sits in display_output_b,
         * which the microcode drives per NLCB write, and it was the THIRD
         * raw getenv found on a hot path in this one file (after
         * display_trace_limit and display_dispm_present). */
        if (dorado_trace_flag("DORADO_NLCB_TRACE")) {
            static long n = 0;
            if (reg != 0 && n++ < 600)
                fprintf(stderr, "NLCB reg=%02o val=%04o raw=%06o\n",
                        reg, val, data);
        }
        switch (reg) {
        case 000:                       /* VCW */
            if (data & 0002u) {         /* VSync: field restart */
                d->nlcb_line = 0;
                d->nlcb_field_odd = (uint8_t)(data & 1u);
                d->ddc_seq_y = (uint16_t)(data & 1u);
                memset(d->ddc_pending_line, 0, sizeof d->ddc_pending_line);
                memset(d->ddc_current_line, 0, sizeof d->ddc_current_line);
            }
            break;
        case 013:                       /* BPointer / CursorX */
            /* The per-field CursorX output uses an IM-derived
             * constant (DisplayMain.mc ReadIM[2]/[3] calibration)
             * whose top bits land on register 13B in our decode; the
             * BPointer reset writes value 0 through the same select,
             * which maps harmlessly off-screen. */
            /* fallthrough */
        case 015:                       /* CursorX */
            if (reg == 013 && val == 0) break;
            d->nlcb_cursor_x = (uint16_t)val;
            break;
        case 016:                       /* CursorLo */
            /* Every visible scanline ends with a CursorLo-tagged
             * output (DisplayMain.mc: the NoCursor path emits the
             * bare tag, the in-cursor path the data byte), so this is
             * the per-line marker. CursorHi follows only on lines the
             * cursor intersects. */
            d->nlcb_cursor_lo = (uint8_t)(val & 0xFF);
            d->nlcb_line++;
            break;
        case 017: {                     /* CursorHi: draw this line */
            uint16_t row = (uint16_t)(((val & 0xFF) << 8) |
                                      d->nlcb_cursor_lo);
            if (row) {
                /* The microcode sends X biased by a hardware constant
                 * (DisplayMain.mc computes base - cursorX); resolve to
                 * a screen X empirically: the bias puts cursorX=0 at
                 * the raw value CURSOR_X_BIAS, with X increasing as
                 * the raw value decreases. */
                int x = (int)(DORADO_DISPLAY_CURSOR_X_BIAS
                              - (int)d->nlcb_cursor_x);
                int line = (int)d->nlcb_line - 1;
                int y = line * 2 + (d->nlcb_field_odd & 1);
                for (int b2 = 0; b2 < 16; b2++) {
                    if ((row >> (15 - b2)) & 1) {
                        dorado_display_set_pixel(d, x + b2, y, 1);
                    }
                }
                d->cursor_rows_drawn++;
            }
            d->nlcb_cursor_lo = 0;
            break;
        }
        default:
            break;
        }
    }
    /* TODO: dispatch by (task, tioa) to HRam load / Mixer load /
     * PixelClk / Statics / etc. */
}

static uint16_t display_input(void *ctx, int task, int subtask,
                              uint8_t tioa, int *bad)
{
    dorado_display *d = ctx;
    (void)task;
    (void)subtask;
    if (bad) *bad = 0;
    /* Single input register on each board: DDC muffler / terminal
     * back-channel readout (HM page 120, Table 25).
     *
     * DisplayAux.mc:DisplayInitConfig probes TStatus for DispM
     * presence, then DDCStatus/MufAddr=106 for monitor type. These
     * are single status bits, not direct Alto keyboard words. EMU task
     * probes should see no DispM board; DHT/AHT terminal tasks receive
     * one serial keyboard bit per scan line. During a boot-button hold
     * the terminal jams the serial data bit to 1; DisplayAux.mc
     * interprets that as message type 17 and times the duration. */
    if (tioa == DORADO_DISPLAY_TIOA_TSTATUS) {
        /* Only the terminal H task's per-scanline serial-bit read sees
         * the keyboard stream (and the boot-button jam). The EMU
         * task's DisplayInitConfig probes this register to detect a
         * DispM board ("Nonzero => DispM board installed"); a jammed
         * serial bit must not make a missing board appear, or the
         * terminal gets assigned to the DispM AHT task and never runs. */
        if (d && (task == DORADO_DISPLAY_TASK_DHT ||
                  task == DORADO_DISPLAY_TASK_AHT)) {
            if (d->boot_button_scanlines > 0) {
                d->boot_button_scanlines--;
                return 0x8000u;     /* boot-button jam on IOB.00 (MSB) */
            }
            return display_terminal_keyboard_bit(d);
        }
        if (display_dispm_present())
            return 0x8000u;         /* DisplayInitConfig: DispM installed. */
        return 0;
    }
    if (tioa == DORADO_DISPLAY_TIOA_DDCSTATUS) {
        uint16_t status = display_ddc_status();
        if (dorado_trace_flag("DORADO_DDCSTATUS_TRACE")) {
            static unsigned n = 0;
            if (n++ < 256) {
                fprintf(stderr,
                        "[ddcstatus] task=%o sub=%o status=%06o\n",
                        task & 017, subtask & 017, status);
            }
        }
        return status;
    }
    return 0;
}

void dorado_display_attach_to_io(dorado_display *d, dorado_io *io)
{
    static dorado_io_device dev;
    dev.read  = display_input;
    dev.write = display_output_b;
    dev.ctx   = d;
    dev.name  = "display(DDC)";

    /* Display-related tasks per HM Table 22:
     *   3   = DHT  (display horizontal task)
     *   4   = AHT  (DispM terminal interface horizontal task)
     *   011₈ = AWT (DispM terminal interface word task)
     *   013₈ = DWT (display word task)
     *
     * DisplayDefs.mc puts the DDC/terminal devices at 0360..0377, and
     * RastDefs.mc puts Monterey raster commands at 0320..0323. Do not
     * claim unrelated TIOAs just because a task number is shared with
     * display microcode; Mesa also retargets these task slots for helper
     * code that can issue non-display Output commands. */
    int display_tasks[] = {3, 4, 011, 013};
    for (size_t i = 0; i < sizeof display_tasks / sizeof display_tasks[0]; i++) {
        for (int tioa = 0360; tioa <= 0377; tioa++) {
            dorado_io_register(io, display_tasks[i], (uint8_t)tioa, &dev);
        }
        for (int tioa = DORADO_DISPLAY_TIOA_RAST_SELCMD;
             tioa <= DORADO_DISPLAY_TIOA_RAST_TASKCMD;
             tioa++) {
            dorado_io_register(io, display_tasks[i], (uint8_t)tioa, &dev);
        }
    }

    /* DisplayAux.mc:DisplayInitConfig runs in the emulator task before
     * THT/DWT are selected. Route just the display-probe registers it
     * touches, not the whole EMU slow-I/O space. */
    dorado_io_register(io, 0, DORADO_DISPLAY_TIOA_TSTATUS, &dev);
    dorado_io_register(io, 0, DORADO_DISPLAY_TIOA_DDCSTATUS, &dev);
    dorado_io_register(io, 0, DORADO_DISPLAY_TIOA_STATICS, &dev);
    d->attached = 1;
}

/* ─── FIFO and framebuffer ───────────────────────────────────────── */

static void display_draw_word(dorado_display *d, int x, int y, uint16_t word)
{
    for (int b = 0; b < 16; b++) {
        if (x < DORADO_DISPLAY_W && y < DORADO_DISPLAY_H) {
            int bit = (word >> (15 - b)) & 1;
            dorado_display_set_pixel(d, x, y, bit);
        }
        x++;
    }
}

static uint32_t display_ddc_va_base(dorado_display *d, uint32_t va)
{
    if (!d->ddc_va_base_valid) {
        const char *base = getenv("DORADO_DDC_RENDER_VA_BASE");
        if (base && *base) {
            d->ddc_va_base = (uint32_t)strtoul(base, NULL, 0);
        } else {
            d->ddc_va_base = va & ~0x7fu;
        }
        d->ddc_va_base_valid = 1;
        if (dorado_trace_flag("DORADO_DWT_TRACE")) {
            fprintf(stderr,
                    "[display] DDC VA render base=0x%05x/0o%o\n",
                    d->ddc_va_base, d->ddc_va_base);
        }
    }
    return d->ddc_va_base;
}

int dorado_display_iofetch_word(dorado_display *d, int subtask,
                                uint32_t va, uint16_t word)
{
    int *head, *tail;
    uint16_t *buf;
    int cap = (int)(sizeof d->fifo_a / sizeof d->fifo_a[0]);
    int channel = (subtask & 2) ? 1 : 0;

    if (d->current_wcb_flag[channel]) {
        unsigned ddc_fetch_limit = 0;
        static unsigned ddc_fetch_traced_words = 0;
        if (display_trace_limit("DORADO_DDC_FETCH_TRACE", 512,
                                &ddc_fetch_limit) &&
            ddc_fetch_traced_words < ddc_fetch_limit &&
            (word || !dorado_trace_flag("DORADO_DDC_FETCH_TRACE_NONZERO"))) {
            dorado_display_ddc_line *line = &d->ddc_current_line[channel];
            fprintf(stderr,
                    "[display] DDC fetch #%u ch=%d sub=%d "
                    "va=0x%05x/0o%o word=%06o "
                    "line_valid=%u y=%u line=%u width=%04o ptr=%04o "
                    "scan=%04o words=%u nz=%u\n",
                    ddc_fetch_traced_words, channel, subtask,
                    va, va, word,
                    line->valid, line->y, line->line, line->width,
                    line->ptr, line->scan, line->word_count,
                    d->dwt_trace_nonzero[channel]);
            ddc_fetch_traced_words++;
        }
        if (d->dwt_trace_active[channel]) {
            if (d->dwt_trace_words[channel] == 0)
                d->dwt_trace_first_va[channel] = va;
            d->dwt_trace_last_va[channel] = va;
            d->dwt_trace_words[channel]++;
            if (word)
                d->dwt_trace_nonzero[channel]++;
        }

        if (va != UINT32_MAX && dorado_trace_flag("DORADO_DDC_RENDER_VA")) {
            uint32_t base = display_ddc_va_base(d, va);
            uint32_t off = va - base;
            int x = (int)((off & 0x7fu) * 16u);
            int y = (int)(off >> 7);
            display_draw_word(d, x, y, word);
            d->ddc_va_words_drawn++;
            d->iofetch_count++;
            return 0;
        }

        if (!dorado_trace_flag("DORADO_DDC_RENDER_DIRECT")) {
            dorado_display_ddc_line *line = &d->ddc_current_line[channel];
            if (line->valid &&
                line->word_count < (sizeof line->words / sizeof line->words[0])) {
                line->words[line->word_count++] = word;
            } else if (line->valid) {
                line->overflow++;
            }
            d->iofetch_count++;
            return 0;
        }

        int x = d->wcb_draw_x[channel];
        int y = d->wcb_draw_y[channel];
        display_draw_word(d, x, y, word);
        x += 16;
        while (x >= DORADO_DISPLAY_W) {
            x -= DORADO_DISPLAY_W;
            y++;
        }
        d->wcb_draw_x[channel] = (uint16_t)x;
        d->wcb_draw_y[channel] = (uint16_t)y;
        d->iofetch_count++;
        return 0;
    }

    if (subtask == 0) {
        head = &d->fifo_a_head;
        tail = &d->fifo_a_tail;
        buf  = d->fifo_a;
    } else {
        head = &d->fifo_b_head;
        tail = &d->fifo_b_tail;
        buf  = d->fifo_b;
    }
    int next = (*head + 1) % cap;
    if (next == *tail) return -1;       /* full */
    buf[*head] = word;
    *head = next;
    d->iofetch_count++;
    return 0;
}

int dorado_display_fifo_push(dorado_display *d, int subtask, uint16_t word)
{
    return dorado_display_iofetch_word(d, subtask, UINT32_MAX, word);
}

static int display_fifo_free(const dorado_display *d, int subtask)
{
    int head, tail, cap;

    if (!d) return 0;
    cap = (int)(sizeof d->fifo_a / sizeof d->fifo_a[0]);
    if (subtask == 0) {
        head = d->fifo_a_head;
        tail = d->fifo_a_tail;
    } else {
        head = d->fifo_b_head;
        tail = d->fifo_b_tail;
    }

    int used = head - tail;
    if (used < 0) used += cap;
    return (cap - 1) - used;
}

/* Words currently queued in the FIFO (0 == empty). A refill wakeup for the
 * Display Word Task is only meaningful while a scan line is actually being
 * output (the display is draining the FIFO). At boot, with no display list,
 * the FIFO is empty (used==0) and the controller is NOT consuming it, so it
 * must not generate a refill wakeup — otherwise the DWT spins forever trying
 * to refill an empty FIFO with no scan line, starving lower-priority tasks
 * (notably the junk task that maintains the VM 430 RTC). The DWT is still
 * started for a real scan line by the next->current WCB/raster edge. */
static int display_fifo_used(const dorado_display *d, int subtask)
{
    int cap = (int)(sizeof d->fifo_a / sizeof d->fifo_a[0]);
    return (cap - 1) - display_fifo_free(d, subtask);
}

void dorado_display_set_pixel(dorado_display *d, int x, int y, int pix)
{
    if (x < 0 || x >= DORADO_DISPLAY_W) return;
    if (y < 0 || y >= DORADO_DISPLAY_H) return;
    int byte_idx = y * DORADO_DISPLAY_ROW_BYTES + (x / 8);
    int bit = 7 - (x % 8);                          /* MSB = leftmost */
    if (pix) d->fb[byte_idx] |=  (uint8_t)(1 << bit);
    else     d->fb[byte_idx] &= (uint8_t)~(1 << bit);
}

/* ─── Snapshot ───────────────────────────────────────────────────── */

int dorado_display_render_fifo(dorado_display *d, int subtask, int *dst_y)
{
    int *head, *tail;
    uint16_t *buf;
    int cap = (int)(sizeof d->fifo_a / sizeof d->fifo_a[0]);

    if (subtask == 0) {
        head = &d->fifo_a_head;
        tail = &d->fifo_a_tail;
        buf  = d->fifo_a;
    } else {
        head = &d->fifo_b_head;
        tail = &d->fifo_b_tail;
        buf  = d->fifo_b;
    }

    int rendered = 0;
    /* Words per scan line: 808 / 16 = 50 (with 8 leftover pixels in
     * the 51st partial word). Many Alto microcode systems pack
     * 51 words/line and ignore the extra 8 bits. We render
     * exactly the visible 808 pixels. */
    int x = 0;
    while (*head != *tail && *dst_y < DORADO_DISPLAY_H) {
        uint16_t w = buf[*tail];
        *tail = (*tail + 1) % cap;
        /* Lay 16 pixels into the framebuffer at (x..x+15, *dst_y).
         * MSB = leftmost. */
        for (int b = 0; b < 16 && x < DORADO_DISPLAY_W; b++) {
            int bit = (w >> (15 - b)) & 1;
            dorado_display_set_pixel(d, x, *dst_y, bit);
            x++;
            rendered++;
        }
        if (x >= DORADO_DISPLAY_W) {
            x = 0;
            (*dst_y)++;
        }
    }
    return rendered;
}

int dorado_display_snapshot_pgm(const dorado_display *d, const char *path)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return -1;
    fprintf(fp, "P5\n%d %d\n255\n", DORADO_DISPLAY_W, DORADO_DISPLAY_H);

    /* Alto convention: bit value 1 = black, 0 = white. We render
     * 1 → 0x00 (black) and 0 → 0xFF (white) so the PGM looks like
     * what would appear on a real monitor. */
    for (int y = 0; y < DORADO_DISPLAY_H; y++) {
        for (int x = 0; x < DORADO_DISPLAY_W; x++) {
            int byte_idx = y * DORADO_DISPLAY_ROW_BYTES + (x / 8);
            int bit = 7 - (x % 8);
            int v = (d->fb[byte_idx] >> bit) & 1;
            unsigned char px = v ? 0x00 : 0xFF;
            if (fputc(px, fp) == EOF) { fclose(fp); return -1; }
        }
    }
    fclose(fp);
    return 0;
}

uint64_t dorado_display_frame(const dorado_display *d)
{
    return d ? d->frame_count : 0;
}

int dorado_display_advance_pixels(dorado_display *d, uint32_t pixels)
{
    if (!d || pixels == 0) return 0;

    const uint64_t pixels_per_frame =
        (uint64_t)DORADO_DISPLAY_W * (uint64_t)DORADO_DISPLAY_H;
    uint64_t pos = (uint64_t)d->scan_line * (uint64_t)DORADO_DISPLAY_W +
                   (uint64_t)d->scan_pixel + (uint64_t)pixels;
    uint64_t frames = pos / pixels_per_frame;
    uint64_t rem = pos % pixels_per_frame;

    d->frame_count += frames;
    d->scan_line = (uint32_t)(rem / DORADO_DISPLAY_W);
    d->scan_pixel = (uint32_t)(rem % DORADO_DISPLAY_W);
    return (int)frames;
}

void dorado_display_vblank(dorado_display *d)
{
    if (!d) return;
    d->frame_count++;
    d->scan_line = 0;
    d->scan_pixel = 0;
}

int dorado_display_scanline_tick(dorado_display *d)
{
    if (!d) return -1;

    uint16_t mask = dorado_display_scanline_wakeup_mask(d);
    if (mask & (1u << DORADO_DISPLAY_TASK_AHT)) return DORADO_DISPLAY_TASK_AHT;
    if (mask & (1u << DORADO_DISPLAY_TASK_DHT)) return DORADO_DISPLAY_TASK_DHT;
    return -1;
}

uint16_t dorado_display_scanline_wakeup_mask(dorado_display *d)
{
    if (!d) return 0;

    d->scanline_ticks++;
    dorado_display_advance_pixels(d, DORADO_DISPLAY_W);

    uint16_t mask = 0;
    /* The display raster clock is independent of the Dorado, but a
     * reset/shut-up DDC should not wake arbitrary tasks. Until the
     * full Statics DHTShutUp/AHTShutUp decode is modeled, wake only
     * the horizontal task that has actually addressed the display. */
    if (!(d->statics & DORADO_DISPLAY_STATICS_DHT_SHUTUP) &&
        (d->terminal_task == DORADO_DISPLAY_TASK_DHT ||
         d->terminal_task == DORADO_DISPLAY_TASK_AHT)) {
        d->terminal_wakeups++;
        mask |= (uint16_t)(1u << d->terminal_task);
    }
    if (d->raster_lt_enabled) {
        d->terminal_wakeups++;
        mask |= (uint16_t)(1u << DORADO_DISPLAY_TASK_DHT);
    }
    return mask;
}

int dorado_display_dwt_wakeup(dorado_display *d, int *subtask)
{
    if (!d) return 0;
    for (int ch = 0; ch < 4; ch++) {
        int st = ch & 3;
        if (d->raster_next_wt_flag[ch] && !d->raster_current_wt_flag[ch]) {
            d->raster_next_wt_flag[ch] = 0;
            d->raster_current_wt_flag[ch] = 1;
            d->dwt_wakeups++;
            if (subtask) *subtask = st;
            return 1;
        }
        if (d->raster_current_wt_flag[ch] && display_fifo_used(d, st) > 0 &&
            display_fifo_free(d, st) >= 16) {
            d->dwt_wakeups++;
            if (subtask) *subtask = st;
            return 1;
        }
    }

    if (d->statics & DORADO_DISPLAY_STATICS_DWT_SHUTUP) return 0;

    for (int ch = 0; ch < 2; ch++) {
        if (d->next_wcb_flag[ch] && !d->current_wcb_flag[ch]) {
            d->next_wcb_flag[ch] = 0;
            d->current_wcb_flag[ch] = 1;
            d->wcb_draw_x[ch] = 0;
            d->wcb_draw_y[ch] = (uint16_t)d->scan_line;
            d->dwt_wakeups++;
            if (subtask) *subtask = ch ? 2 : 0;
            return 1;
        }
    }
    for (int ch = 0; ch < 2; ch++) {
        int st = ch ? 2 : 0;
        if (d->current_wcb_flag[ch] && display_fifo_free(d, st) >= 16) {
            d->dwt_wakeups++;
            if (subtask) *subtask = st;
            return 1;
        }
    }
    return 0;
}

/* ---- Colour step 0: which DDC output devices does a world actually use? ---
 *
 * The Dorado's display is TWO boards. DispY is monochrome -- Alto video,
 * cursor, MiniMixer -- and DispM is colour: sheets 12-31 are the Mixer,
 * BMap/CMap and the three DACs. The netlist settles it beyond argument
 * (docs/sil-netlist-crosscheck.md): DispM carries DACRed/DACGreen/DACBlue
 * with GNDRed/GNDGreen/GNDBlue and RefIn; DispY carries one AltoTTLVideo.
 *
 * We model DispY's devices and none of DispM's. Before promising colour,
 * the question that decides the whole scope is empirical: does any world we
 * can boot ever PROGRAM the colour side? A world that never writes DispM's
 * addresses cannot display colour no matter what we implement, and one that
 * does tells us exactly which registers to build.
 *
 * This prints every TIOA the DDC saw, so the answer comes off a real run
 * rather than out of the manual. Addresses we decode are named; anything
 * else is flagged UNDECODED, which is where DispM would show up. */
void dorado_display_dump_tioa_use(const dorado_display *d)
{
    static const struct { unsigned tioa; const char *name; } known[] = {
        { DORADO_DISPLAY_TIOA_DHTFLAG,   "DHTFlag"   },
        { DORADO_DISPLAY_TIOA_DWTFLAG,   "DWTFlag"   },
        { DORADO_DISPLAY_TIOA_AHTFLAG,   "AHTFlag"   },
        { DORADO_DISPLAY_TIOA_AWTFLAG,   "AWTFlag"   },
        { DORADO_DISPLAY_TIOA_NLCB,      "NLCB"      },
        { DORADO_DISPLAY_TIOA_TNLCB,     "TNLCB"     },
        { DORADO_DISPLAY_TIOA_STATICS,   "Statics"   },
        { DORADO_DISPLAY_TIOA_TSTATICS,  "TStatics"  },
        { DORADO_DISPLAY_TIOA_HRAM,      "HRam"      },
        { DORADO_DISPLAY_TIOA_DDCSTATUS, "DDCStatus" },
        { DORADO_DISPLAY_TIOA_TSTATUS,   "TStatus"   },
        { DORADO_DISPLAY_TIOA_RAST_SELCMD,  "RastSel"  },
        { DORADO_DISPLAY_TIOA_RAST_ADDRCMD, "RastAddr" },
        { DORADO_DISPLAY_TIOA_RAST_DATACMD, "RastData" },
        { DORADO_DISPLAY_TIOA_RAST_TASKCMD, "RastTask" },
    };
    if (!d) return;
    unsigned undecoded = 0;
    /* WARNING, and it produced a wrong answer on 2026-08-08: these counters
     * live in `dorado_display`, which machine.c snapshots WHOLE
     * (snap_wr(&m->display) / snap_rd(&m->display)). On any run started with
     * --snapshot-in they come back from the BAKE, so this dump reports the
     * checkpoint's history, not what the run just did. It is only run-scoped
     * on a cold boot. For run-scoped truth across all tasks -- including
     * addresses no device is registered for -- use DORADO_IO_CENSUS, whose
     * counters are file-scope statics and therefore not restored. */
    fprintf(stderr, "[ddc] Output<-B by TIOA (total %llu)"
            " -- SNAPSHOT-RESTORED counts if this run used --snapshot-in:\n",
            (unsigned long long)d->output_count);
    for (unsigned t = 0; t < 256; t++) {
        if (d->output_tioa_count[t] == 0) continue;
        const char *name = NULL;
        for (unsigned i = 0; i < sizeof known / sizeof known[0]; i++)
            if (known[i].tioa == t) { name = known[i].name; break; }
        if (!name) undecoded++;
        fprintf(stderr, "[ddc]   %03o %-10s %12llu  first=%06o last=%06o\n",
                t, name ? name : "UNDECODED",
                (unsigned long long)d->output_tioa_count[t],
                d->output_tioa_first[t], d->output_tioa_last[t]);
    }
    fprintf(stderr,
            "[ddc] %u undecoded address(es). DispM (colour) would appear "
            "here; none means this world never programmed the colour side.\n",
            undecoded);
}
