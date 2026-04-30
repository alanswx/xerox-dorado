#include "display.h"

#include <stdio.h>
#include <string.h>

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

/* Phase 1: a single permissive output handler that records the
 * (task, tioa, data) triple but does nothing semantically. The
 * real DDC has six (DispY) or eight (DispM) output devices, each
 * decoded by TIOA. We don't yet know the numeric TIOA values
 * microcode uses; we'll fill them in once we trace probe_aemu's
 * display-task writes. For now everything just gets logged. */
static void display_output_b(void *ctx, int task, uint8_t tioa, uint16_t data)
{
    dorado_display *d = ctx;
    d->output_count++;
    d->riob = data;     /* HM page 119: IOB stays in DDC RIOB until
                         * next output command */
    /* TODO: dispatch by (task, tioa) to NLCB load / HRam load /
     * Mixer load / PixelClk / Statics / etc. */
    (void)task;
    (void)tioa;
}

static uint16_t display_input(void *ctx, int task, uint8_t tioa, int *bad)
{
    dorado_display *d = ctx;
    if (bad) *bad = 0;
    /* Single input register on each board: DDC muffler / terminal
     * back-channel readout (HM page 120, Table 25). Returning the
     * last output word makes Initial's terminal task synthesize false
     * key-down messages. Until the 7-wire back-channel is decoded,
     * report idle/all-ones so boot keys stay "up". */
    (void)task;
    (void)tioa;
    return dorado_display_keyboard_word(d, 0);
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
