#include "display.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

/* test_init_zeroes — fresh display has zero framebuffer + zero counters. */
static int test_init_zeroes(void)
{
    static dorado_display d;
    dorado_display_init(&d);
    EXPECT(d.output_count == 0, "output_count = %llu",
           (unsigned long long)d.output_count);
    EXPECT(d.iofetch_count == 0, "iofetch_count = %llu",
           (unsigned long long)d.iofetch_count);
    for (int i = 0; i < DORADO_DISPLAY_FB_BYTES; i++) {
        EXPECT(d.fb[i] == 0, "fb[%d] = 0x%02X (expected 0)", i, d.fb[i]);
    }
    EXPECT(d.ram_keep == 1, "ram_keep should default to 1 (video owns)");
    printf("PASS  test_init_zeroes (FB=%dx%d, %d bytes)\n",
           DORADO_DISPLAY_W, DORADO_DISPLAY_H, DORADO_DISPLAY_FB_BYTES);
    return 0;
}

/* test_set_pixel — round-trip pixels through set_pixel and verify
 * the byte/bit packing matches the documented (MSB = leftmost) layout. */
static int test_set_pixel(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    /* Set the leftmost pixel of row 0. With MSB=leftmost packing,
     * fb[0] should have bit 7 set = 0x80. */
    dorado_display_set_pixel(&d, 0, 0, 1);
    EXPECT(d.fb[0] == 0x80, "fb[0] = 0x%02X (expected 0x80)", d.fb[0]);

    /* Pixel (7, 0) → fb[0] bit 0 → adds 0x01. */
    dorado_display_set_pixel(&d, 7, 0, 1);
    EXPECT(d.fb[0] == 0x81, "fb[0] = 0x%02X (expected 0x81)", d.fb[0]);

    /* Pixel (8, 0) → fb[1] bit 7 → fb[1] = 0x80. */
    dorado_display_set_pixel(&d, 8, 0, 1);
    EXPECT(d.fb[1] == 0x80, "fb[1] = 0x%02X (expected 0x80)", d.fb[1]);

    /* Bottom-right corner. */
    dorado_display_set_pixel(&d, DORADO_DISPLAY_W - 1, DORADO_DISPLAY_H - 1, 1);
    int last_byte = DORADO_DISPLAY_FB_BYTES - 1;
    /* 808 / 8 = 101, so the last byte holds pixels 800..807. Pixel 807
     * is bit 0 of the last byte. */
    EXPECT(d.fb[last_byte] == 0x01,
           "fb[%d] = 0x%02X (expected 0x01)", last_byte, d.fb[last_byte]);

    /* Clear and re-test. */
    dorado_display_set_pixel(&d, 0, 0, 0);
    EXPECT(d.fb[0] == 0x01, "fb[0] = 0x%02X (expected 0x01 after clear)",
           d.fb[0]);

    /* Out-of-bounds: silent no-op. */
    dorado_display_set_pixel(&d, -1, 0, 1);
    dorado_display_set_pixel(&d, DORADO_DISPLAY_W, 0, 1);
    dorado_display_set_pixel(&d, 0, -1, 1);
    dorado_display_set_pixel(&d, 0, DORADO_DISPLAY_H, 1);

    printf("PASS  test_set_pixel (MSB-leftmost packing verified)\n");
    return 0;
}

static int test_keyboard_words(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    for (int i = 0; i < DORADO_DISPLAY_KEY_WORDS; i++) {
        EXPECT(dorado_display_keyboard_word(&d, i) == 0xFFFF,
               "keyboard word %d should initialize all-up", i);
    }

    dorado_display_keyboard_set_bit(&d, 1, 4, 1);
    EXPECT(dorado_display_keyboard_word(&d, 1) == 0xFFEF,
           "key down clears bit 4, got 0x%04X",
           dorado_display_keyboard_word(&d, 1));

    dorado_display_keyboard_set_bit(&d, 1, 4, 0);
    EXPECT(dorado_display_keyboard_word(&d, 1) == 0xFFFF,
           "key up sets bit 4, got 0x%04X",
           dorado_display_keyboard_word(&d, 1));

    dorado_display_keyboard_set_key(&d, DORADO_KEY_RETURN, 1);
    EXPECT(dorado_display_keyboard_word(&d, 2) == 0xFFF7,
           "RETURN down clears word 2 mask 0x0008, got 0x%04X",
           dorado_display_keyboard_word(&d, 2));
    dorado_display_keyboard_set_key(&d, DORADO_KEY_RETURN, 0);
    EXPECT(dorado_display_keyboard_word(&d, 2) == 0xFFFF,
           "RETURN up restores word 2, got 0x%04X",
           dorado_display_keyboard_word(&d, 2));

    dorado_display_keyboard_set_key(&d, DORADO_KEY_M, 1);
    EXPECT(dorado_display_keyboard_word(&d, 3) == 0xFEFF,
           "M down clears word 3 mask 0x0100, got 0x%04X",
           dorado_display_keyboard_word(&d, 3));
    dorado_display_keyboard_set_key(&d, DORADO_KEY_BS, 1);
    EXPECT(dorado_display_keyboard_word(&d, 0) == 0xFFFE,
           "BS down clears word 0 mask 0x0001, got 0x%04X",
           dorado_display_keyboard_word(&d, 0));

    dorado_display_keyboard_set_word(&d, 3, 0x7FFF);
    EXPECT(dorado_display_keyboard_word(&d, 3) == 0x7FFF,
           "set keyboard word 3");
    dorado_display_keyboard_all_up(&d);
    EXPECT(dorado_display_keyboard_word(&d, 3) == 0xFFFF,
           "all_up resets word 3");

    printf("PASS  test_keyboard_words (complemented Alto key words)\n");
    return 0;
}

/* test_attach_to_io — registering the display intercepts slow-IO
 * outputs from display tasks (DHT/DWT/AHT/AWT) and counts them. */
static int test_attach_to_io(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_display d;
    dorado_display_init(&d);
    dorado_display_attach_to_io(&d, &io);
    EXPECT(d.attached == 1, "display.attached should be 1 after attach");

    /* DHT (task 3) drives an Output←B at TIOA=0x42 with data 0xCAFE. */
    dorado_io_write(&io, /*task=*/3, /*tioa=*/0x42, 0xCAFE);
    EXPECT(d.output_count == 1, "output_count = %llu (expected 1)",
           (unsigned long long)d.output_count);
    EXPECT(d.riob == 0xCAFE, "riob = 0x%X (expected 0xCAFE)", d.riob);

    /* DWT (task 13₈ = 11 dec) drives another. */
    dorado_io_write(&io, /*task=*/011, /*tioa=*/0x10, 0x1234);
    EXPECT(d.output_count == 2, "output_count after second write = %llu",
           (unsigned long long)d.output_count);

    /* Non-display task (e.g. EMU=0) is NOT routed to the display.
     * Verify by writing to it — it should be a no-op since no other
     * device is registered there. */
    dorado_io_write(&io, /*task=*/0, /*tioa=*/0x42, 0xFFFF);
    EXPECT(d.output_count == 2, "non-display task should not bump count, "
           "got %llu", (unsigned long long)d.output_count);

    /* Pd←Input from DDC reports idle single-bit terminal/status data. */
    int bad = -1;
    uint16_t v = dorado_io_read(&io, 3, 0x42, &bad);
    EXPECT(v == 0x0000, "input = 0x%X (expected idle 0)", v);
    EXPECT(bad == 0, "parity should be good for mapped device, got bad=%d", bad);

    v = dorado_io_read(&io, 0, DORADO_DISPLAY_TIOA_TSTATUS, &bad);
    EXPECT(v == 0x0000, "EMU TStatus input = 0x%X (expected idle 0)", v);
    EXPECT(bad == 0, "EMU TStatus should be routed for DisplayInitConfig");

    dorado_display_boot_button(&d, 2);
    v = dorado_io_read(&io, DORADO_DISPLAY_TASK_DHT,
                       DORADO_DISPLAY_TIOA_TSTATUS, &bad);
    EXPECT(v == 0x0001, "boot TStatus first bit = 0x%X", v);
    v = dorado_io_read(&io, DORADO_DISPLAY_TASK_DHT,
                       DORADO_DISPLAY_TIOA_TSTATUS, &bad);
    EXPECT(v == 0x0001, "boot TStatus second bit = 0x%X", v);
    v = dorado_io_read(&io, DORADO_DISPLAY_TASK_DHT,
                       DORADO_DISPLAY_TIOA_TSTATUS, &bad);
    EXPECT(v == 0x0001, "keyboard serial stream should start after hold");
    EXPECT(d.terminal_bits == 1, "terminal_bits = %llu (expected 1)",
           (unsigned long long)d.terminal_bits);

    printf("PASS  test_attach_to_io (4 tasks routed, 2 writes counted, "
           "input=0x%X)\n", v);
    return 0;
}

static int test_display_wcb_flag_protocol(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_display d;
    dorado_display_init(&d);
    dorado_display_attach_to_io(&d, &io);
    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_STATICS, 0000);

    dorado_io_write(&io, DORADO_DISPLAY_TASK_AHT,
                    DORADO_DISPLAY_TIOA_AHTFLAG, 0002);
    EXPECT(d.next_wcb_flag[0] == 1, "channel A NextWCB should be set");
    EXPECT(d.nlcb_writes == 1, "nlcb_writes = %llu (expected 1)",
           (unsigned long long)d.nlcb_writes);

    int subtask = -1;
    EXPECT(dorado_display_dwt_wakeup(&d, &subtask) == 1,
           "DWT wakeup should be pending after NextWCB");
    EXPECT(subtask == 0, "DWT subtask = %d (expected A/subtask 0)", subtask);
    EXPECT(d.next_wcb_flag[0] == 0, "NextWCB should be consumed");
    EXPECT(d.current_wcb_flag[0] == 1, "CurrentWCB should be set");

    EXPECT(dorado_display_dwt_wakeup(&d, &subtask) == 1,
           "DWT wakeup should continue while CurrentWCB and FIFO space");
    EXPECT(subtask == 0, "DWT subtask = %d (expected A/subtask 0)", subtask);

    dorado_io_write(&io, DORADO_DISPLAY_TASK_DWT,
                    DORADO_DISPLAY_TIOA_DWTFLAG, 0000);
    EXPECT(d.current_wcb_flag[0] == 0, "DWT Output_0 clears CurrentWCB");
    EXPECT(dorado_display_dwt_wakeup(&d, &subtask) == 0,
           "DWT wakeup should stop when no WCB flags are set");

    dorado_io_write_subtask(&io, DORADO_DISPLAY_TASK_DWT, 0,
                            DORADO_DISPLAY_TIOA_DWTFLAG, 0177777);
    EXPECT(d.current_wcb_flag[0] == 1,
           "DWT Output_-1 on subtask 0 should set channel A current");
    EXPECT(d.current_wcb_flag[1] == 0,
           "DWT Output_-1 on subtask 0 must not select channel B by sign bit");
    dorado_io_write_subtask(&io, DORADO_DISPLAY_TASK_DWT, 0,
                            DORADO_DISPLAY_TIOA_DWTFLAG, 0000);
    EXPECT(d.current_wcb_flag[0] == 0,
           "DWT Output_0 on subtask 0 clears channel A current");

    d.terminal_task = 0;
    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_DHTFLAG, 0004);
    EXPECT(d.next_wcb_flag[1] == 1, "channel B NextWCB should be set");
    EXPECT(dorado_display_dwt_wakeup(&d, &subtask) == 1,
           "DWT wakeup should be pending for channel B");
    EXPECT(subtask == 2, "DWT subtask = %d (expected B/subtask 2)", subtask);
    dorado_io_write_subtask(&io, DORADO_DISPLAY_TASK_DWT, 2,
                            DORADO_DISPLAY_TIOA_DWTFLAG, 0000);
    EXPECT(d.current_wcb_flag[1] == 0,
           "DWT Output_0 on subtask 2 clears channel B current");

    printf("PASS  test_display_wcb_flag_protocol "
           "(DHT/AHT flag outputs drive DWT wakeups)\n");
    return 0;
}

static int test_hram_load_protocol(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_display d;
    dorado_display_init(&d);
    dorado_display_attach_to_io(&d, &io);

    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_HRAM, 000000);
    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_HRAM,
                    DORADO_DISPLAY_RAM_LOADADDR | DORADO_DISPLAY_RAM_WRITE | 0123);
    EXPECT(d.ram_addr == 0123, "HRam address = 0o%o (expected 0123)",
           d.ram_addr);

    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_HRAM, 0005);
    EXPECT(d.hram[0123] == 5, "HRam[0123] = %u", d.hram[0123]);
    EXPECT(d.ram_addr == 0124, "HRam address did not auto-increment");

    dorado_io_write(&io, DORADO_DISPLAY_TASK_DHT,
                    DORADO_DISPLAY_TIOA_HRAM, DORADO_DISPLAY_RAM_KEEP);
    EXPECT(d.ram_keep == 1, "HRam release did not restore video ownership");

    printf("PASS  test_hram_load_protocol\n");
    return 0;
}

/* test_fifo_push — IOFetch← path drops words into per-channel FIFO. */
static int test_fifo_push(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    EXPECT(dorado_display_fifo_push(&d, 0, 0xDEAD) == 0, "push channel A");
    EXPECT(dorado_display_fifo_push(&d, 0, 0xBEEF) == 0, "push channel A again");
    EXPECT(dorado_display_fifo_push(&d, 2, 0x1234) == 0, "push channel B");

    EXPECT(d.iofetch_count == 3, "iofetch_count = %llu (expected 3)",
           (unsigned long long)d.iofetch_count);
    EXPECT(d.fifo_a_head == 2, "channel A head = %d (expected 2)",
           d.fifo_a_head);
    EXPECT(d.fifo_b_head == 1, "channel B head = %d (expected 1)",
           d.fifo_b_head);
    EXPECT(d.fifo_a[0] == 0xDEAD, "fifo_a[0] = 0x%X", d.fifo_a[0]);
    EXPECT(d.fifo_a[1] == 0xBEEF, "fifo_a[1] = 0x%X", d.fifo_a[1]);
    EXPECT(d.fifo_b[0] == 0x1234, "fifo_b[0] = 0x%X", d.fifo_b[0]);

    printf("PASS  test_fifo_push (channel A: 2 words, B: 1 word)\n");
    return 0;
}

/* test_snapshot_pgm — draw a recognizable test pattern, write PGM,
 * and verify the file header. */
static int test_snapshot_pgm(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    /* Draw a diagonal line + framing rectangle so the snapshot is
     * visually verifiable if anyone opens it. */
    for (int i = 0; i < DORADO_DISPLAY_H; i++) {
        dorado_display_set_pixel(&d, 0, i, 1);
        dorado_display_set_pixel(&d, DORADO_DISPLAY_W - 1, i, 1);
    }
    for (int i = 0; i < DORADO_DISPLAY_W; i++) {
        dorado_display_set_pixel(&d, i, 0, 1);
        dorado_display_set_pixel(&d, i, DORADO_DISPLAY_H - 1, 1);
    }
    int diag_len = DORADO_DISPLAY_H < DORADO_DISPLAY_W
                 ? DORADO_DISPLAY_H : DORADO_DISPLAY_W;
    for (int i = 0; i < diag_len; i++) {
        dorado_display_set_pixel(&d, i, i, 1);
    }

    const char *path = "/tmp/test_dorado_display.pgm";
    EXPECT(dorado_display_snapshot_pgm(&d, path) == 0,
           "snapshot_pgm to %s", path);

    /* Verify file header. */
    FILE *fp = fopen(path, "rb");
    EXPECT(fp != NULL, "reopen %s", path);
    char hdr[32];
    if (!fgets(hdr, sizeof hdr, fp)) { fclose(fp); FAIL("header read"); }
    EXPECT(strcmp(hdr, "P5\n") == 0, "PGM magic = %s", hdr);
    int w, h, maxv;
    EXPECT(fscanf(fp, "%d %d\n%d", &w, &h, &maxv) == 3,
           "header dims+maxv");
    EXPECT(w == DORADO_DISPLAY_W && h == DORADO_DISPLAY_H,
           "dims = %dx%d (expected %dx%d)", w, h,
           DORADO_DISPLAY_W, DORADO_DISPLAY_H);
    EXPECT(maxv == 255, "maxv = %d", maxv);
    fclose(fp);

    printf("PASS  test_snapshot_pgm (PGM at %s, %dx%d)\n",
           path, DORADO_DISPLAY_W, DORADO_DISPLAY_H);
    return 0;
}

static int test_frame_clock(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    EXPECT(dorado_display_frame(&d) == 0,
           "initial frame should be 0");
    EXPECT(dorado_display_advance_pixels(&d, DORADO_DISPLAY_W - 1) == 0,
           "not enough pixels for a frame");
    EXPECT(d.scan_line == 0 && d.scan_pixel == DORADO_DISPLAY_W - 1,
           "scan position should advance within first line");

    EXPECT(dorado_display_advance_pixels(&d, 1) == 0,
           "one full scan line is not a frame");
    EXPECT(d.scan_line == 1 && d.scan_pixel == 0,
           "scan position should wrap to line 1");

    uint32_t remaining =
        (uint32_t)(DORADO_DISPLAY_W * (DORADO_DISPLAY_H - 1));
    EXPECT(dorado_display_advance_pixels(&d, remaining) == 1,
           "remaining visible pixels should complete one frame");
    EXPECT(dorado_display_frame(&d) == 1,
           "frame count should be 1, got %llu",
           (unsigned long long)dorado_display_frame(&d));
    EXPECT(d.scan_line == 0 && d.scan_pixel == 0,
           "scan should reset at frame boundary");

    dorado_display_vblank(&d);
    EXPECT(dorado_display_frame(&d) == 2,
           "vblank should increment to 2");

    printf("PASS  test_frame_clock (vblank frame counter)\n");
    return 0;
}

static int test_scanline_tick_wakes_terminal_task(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_display d;
    dorado_display_init(&d);
    dorado_display_attach_to_io(&d, &io);

    EXPECT(dorado_display_scanline_tick(&d) == -1,
           "scanline should not wake DHT before terminal output");
    EXPECT(d.scanline_ticks == 1, "scanline tick should still count");
    dorado_io_write(&io, DORADO_DISPLAY_TASK_AHT,
                    DORADO_DISPLAY_TIOA_TSTATICS, 0000);

    dorado_io_write(&io, DORADO_DISPLAY_TASK_AHT, 0x20, 0x1234);
    EXPECT(d.terminal_task == DORADO_DISPLAY_TASK_AHT,
           "AHT output should select terminal task");

    uint16_t mask = dorado_display_scanline_wakeup_mask(&d);
    EXPECT((mask & (1u << DORADO_DISPLAY_TASK_DHT)) == 0,
           "scanline mask should not wake DHT after AHT selection");
    EXPECT(mask & (1u << DORADO_DISPLAY_TASK_AHT),
           "scanline mask should wake AHT");
    EXPECT(d.terminal_wakeups == 1,
           "terminal wakeups = %llu",
           (unsigned long long)d.terminal_wakeups);

    printf("PASS  test_scanline_tick_wakes_terminal_task (AHT)\n");
    return 0;
}

/* test_render_fifo — push pixel words into FIFO, render them, verify
 * the framebuffer matches. */
static int test_render_fifo(void)
{
    static dorado_display d;
    dorado_display_init(&d);

    /* Push 51 words to fill one scan line (50 full words = 800 pixels
     * + 8 leftover pixels in the 51st word's first 8 bits). Use a
     * recognizable pattern: alternating 0xAAAA / 0x5555 (= odd/even
     * pixels). */
    int row_words = (DORADO_DISPLAY_W + 15) / 16;       /* 51 */
    for (int w = 0; w < row_words; w++) {
        uint16_t pat = (w & 1) ? 0x5555 : 0xAAAA;
        EXPECT(dorado_display_fifo_push(&d, /*subtask=*/0, pat) == 0,
               "push word %d", w);
    }

    int dst_y = 0;
    int rendered = dorado_display_render_fifo(&d, 0, &dst_y);
    /* Should render at least one full row (= 808 pixels) plus 8 more
     * from the 51st word. dst_y should be 1 (advanced past row 0). */
    EXPECT(rendered >= DORADO_DISPLAY_W,
           "rendered %d pixels (expected ≥ %d)", rendered, DORADO_DISPLAY_W);
    EXPECT(dst_y >= 1, "dst_y advanced to %d", dst_y);

    /* Row 0 should now contain pattern: words 0,2,4,... = 0xAAAA,
     * words 1,3,5,... = 0x5555. So pixels at even positions in even
     * words = 1, odd positions = 0; alternates per word. */
    /* Pixel (0, 0) is bit 7 of fb[0]. Word 0 = 0xAAAA → MSB = 1. */
    EXPECT((d.fb[0] >> 7) & 1, "pixel (0,0) should be 1");
    /* Pixel (1, 0) = bit 6 of fb[0]. Word 0 bit 14 = 0 (0xAAAA = 1010..). */
    EXPECT(!((d.fb[0] >> 6) & 1), "pixel (1,0) should be 0");
    /* fb[0] should be 0xAA. */
    EXPECT(d.fb[0] == 0xAA, "fb[0] = 0x%02X (expected 0xAA)", d.fb[0]);
    /* fb[2] is the start of word 1 = 0x5555 → high byte = 0x55. */
    EXPECT(d.fb[2] == 0x55, "fb[2] = 0x%02X (expected 0x55)", d.fb[2]);

    /* Subsequent rows should be untouched. */
    EXPECT(d.fb[DORADO_DISPLAY_ROW_BYTES] == 0,
           "row 1 byte 0 should still be 0");

    /* Push more rows and render. */
    for (int r = 0; r < 4; r++) {
        for (int w = 0; w < row_words; w++) {
            uint16_t pat = ((r + w) & 1) ? 0x5555 : 0xAAAA;
            dorado_display_fifo_push(&d, 0, pat);
        }
    }
    rendered = dorado_display_render_fifo(&d, 0, &dst_y);
    EXPECT(rendered >= 4 * DORADO_DISPLAY_W,
           "after 4 more rows, rendered %d (expected ≥ %d)",
           rendered, 4 * DORADO_DISPLAY_W);
    EXPECT(dst_y >= 5, "dst_y advanced past 5, got %d", dst_y);

    /* Empty FIFO render = no-op. */
    int saved_y = dst_y;
    rendered = dorado_display_render_fifo(&d, 0, &dst_y);
    EXPECT(rendered == 0, "empty FIFO rendered %d pixels", rendered);
    EXPECT(dst_y == saved_y, "dst_y advanced from empty FIFO");

    /* Snapshot for visual inspection. */
    dorado_display_snapshot_pgm(&d, "/tmp/test_dorado_render.pgm");
    printf("PASS  test_render_fifo (5 rows × 808 px rendered, "
           "dst_y=%d after; PGM at /tmp/test_dorado_render.pgm)\n",
           dst_y);
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_init_zeroes();
    rc |= test_set_pixel();
    rc |= test_keyboard_words();
    rc |= test_attach_to_io();
    rc |= test_display_wcb_flag_protocol();
    rc |= test_hram_load_protocol();
    rc |= test_fifo_push();
    rc |= test_snapshot_pgm();
    rc |= test_frame_clock();
    rc |= test_scanline_tick_wakes_terminal_task();
    rc |= test_render_fifo();
    if (rc == 0) printf("\nAll display tests passed.\n");
    return rc;
}
