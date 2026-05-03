#include "display.h"

#include <stdio.h>
#include <string.h>

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

void dorado_display_boot_button(dorado_display *d, uint32_t scanlines)
{
    if (!d) return;
    d->boot_button_scanlines = scanlines;
}

static uint16_t display_terminal_keyboard_bit(dorado_display *d)
{
    if (!d) return 0;

    uint8_t word = (uint8_t)(d->terminal_msg_word & 3u);
    uint8_t type = (uint8_t)(word + 1u);
    uint16_t body = d->keyboard_words[word];
    uint32_t msg = (1u << 31) | ((uint32_t)type << 24) |
                   ((uint32_t)body << 8) | (1u << 7);
    uint16_t bit = (uint16_t)((msg >> (31u - (d->terminal_msg_bit & 31u))) & 1u);

    d->terminal_bits++;
    d->terminal_msg_bit++;
    if (d->terminal_msg_bit >= 32u) {
        d->terminal_msg_bit = 0;
        d->terminal_msg_word = (uint8_t)((word + 1u) & 3u);
        d->terminal_messages++;
    }
    return bit;
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
    d->output_tioa_count[tioa]++;
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
        if (task == d->terminal_task) {
            if (data & 0002u) d->next_wcb_flag[0] = 1;
        } else {
            if (data & 0002u) d->next_wcb_flag[0] = 1;
            if (data & 0004u) d->next_wcb_flag[1] = 1;
        }
        d->nlcb_writes++;
    } else if (tioa == DORADO_DISPLAY_TIOA_DWTFLAG ||
               tioa == DORADO_DISPLAY_TIOA_AWTFLAG) {
        int channel = (subtask & 2) ? 1 : 0;
        uint8_t cur = (uint8_t)(data & 1u);
        d->current_wcb_flag[channel] = cur;
        if (cur) d->next_wcb_flag[channel] = 0;
    }
    /* TODO: dispatch by (task, tioa) to NLCB load / HRam load /
     * Mixer load / PixelClk / Statics / etc. */
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
        if (d && d->boot_button_scanlines > 0) {
            d->boot_button_scanlines--;
            return 1;
        }
        if (d && (task == DORADO_DISPLAY_TASK_DHT ||
                  task == DORADO_DISPLAY_TASK_AHT)) {
            return display_terminal_keyboard_bit(d);
        }
        return 0;
    }
    if (tioa == DORADO_DISPLAY_TIOA_DDCSTATUS) return 0;
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
     * Without yet-known TIOA assignments, register the device on every
     * TIOA address for these tasks. Refine once we observe specific
     * (task, TIOA) pairs in microcode traces. */
    int display_tasks[] = {3, 4, 011, 013};
    for (size_t i = 0; i < sizeof display_tasks / sizeof display_tasks[0]; i++) {
        for (int tioa = 0; tioa < 256; tioa++) {
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

int dorado_display_fifo_push(dorado_display *d, int subtask, uint16_t word)
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
    int next = (*head + 1) % cap;
    if (next == *tail) return -1;       /* full */
    buf[*head] = word;
    *head = next;
    d->iofetch_count++;
    return 0;
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
    return mask;
}

int dorado_display_dwt_wakeup(dorado_display *d, int *subtask)
{
    if (!d) return 0;
    if (d->statics & DORADO_DISPLAY_STATICS_DWT_SHUTUP) return 0;

    for (int ch = 0; ch < 2; ch++) {
        if (d->next_wcb_flag[ch] && !d->current_wcb_flag[ch]) {
            d->next_wcb_flag[ch] = 0;
            d->current_wcb_flag[ch] = 1;
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
