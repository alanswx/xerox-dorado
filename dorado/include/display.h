#ifndef DORADO_DISPLAY_H_
#define DORADO_DISPLAY_H_

#include <stddef.h>
#include <stdint.h>

#include "io.h"

/*
 * Dorado Display Controller (HM §11).
 *
 * The Display Data Channel (DDC) is a Fast-Output device that pulls
 * pixel data from main storage via IOFetch← and pushes it through a
 * mixer/MiniMixer to the monitor (Alto/LF mono via 7-wire interface,
 * or DispM directly). Two channels A and B can run independently
 * (for stereo, dual monitor, or windowed setups).
 *
 * Tasks (HM Table 22):
 *   DHT  task 3   — Display Horizontal Task (low priority).
 *                   Once per scan line: compute next-line params,
 *                   load NLCB, set αNextWCBFlag.
 *   DWT  task 13₈ — Display Word Task (high priority). Issues
 *                   IOFetch← per munch.
 *   AHT  task 4   — DispM terminal-interface horizontal task.
 *   AWT  task 11₈ — DispM terminal-interface word task.
 *
 * Slow-IO control (HM §11 page 119):
 *   DDC catches Output←B at TIOA-selected addresses, accumulates
 *   into NLCB/HRam/Mixer/CMap/BMap RAMs and various flipflops
 *   (PixelClk, Statics with DHTShutUp/DWTShutUp, MufAddr).
 *
 * For Phase 1 (this header) we model just enough to:
 *   - Accept all DDC slow-IO writes without halting
 *   - Track NLCB / HRam / MixerModes / VCW state
 *   - Provide a simple framebuffer that DWT's IOFetch← writes pixel
 *     bytes into (skipping the mixer/HSync/VBlank waveform machinery)
 *   - Dump a PPM/PGM snapshot of the framebuffer for visual inspection
 *
 * Display geometry: 808×606 monochrome (Alto-style). 808 / 8 = 101
 * bytes per row, 101 × 606 = 61206 bytes total.
 *
 * NOT modeled yet:
 *   - Pixel clock generation (we render full frames synchronously)
 *   - Real HSync/VSync waveform timing
 *   - 7-wire interface back-channel keyboard/mouse encoding
 *   - 24Bit color mode (3-channel via AMap+BMap+CMap)
 *   - Mixer modes A8B2 / BBypass / dDAC outputs
 *   - Interlace (OddField)
 */

#define DORADO_DISPLAY_W            808
#define DORADO_DISPLAY_H            606
#define DORADO_DISPLAY_ROW_BYTES    ((DORADO_DISPLAY_W + 7) / 8)
#define DORADO_DISPLAY_FB_BYTES     (DORADO_DISPLAY_ROW_BYTES * DORADO_DISPLAY_H)

#define DORADO_DISPLAY_NLCB_WORDS   16
#define DORADO_DISPLAY_HRAM_WORDS   1024
#define DORADO_DISPLAY_MIXER_WORDS  1024  /* 1024 × 24 (HM page 112) */

typedef struct {
    /* Per-channel CLCB/NLCB. HM page 113: αPolarity, αResolution,
     * αItemSize, αLeftMargin, αWidth, αFifoAddr, MixerModes, VCW,
     * Cursor, CursorX. We keep them as 16 12-bit words per channel
     * since that's how microcode loads them via slow I/O.
     *
     * Index 0 = channel A (subtask 0), index 1 = channel B (subtask 2). */
    uint16_t nlcb[2][DORADO_DISPLAY_NLCB_WORDS];
    uint16_t clcb[2][DORADO_DISPLAY_NLCB_WORDS];

    uint8_t  hram[DORADO_DISPLAY_HRAM_WORDS];   /* 3 bits used per word */
    uint16_t mixer[DORADO_DISPLAY_MIXER_WORDS]; /* 24-bit entries; we
                                                 * pack low-12 here */
    uint16_t pixel_clk;          /* PixelClk register */
    uint16_t statics;            /* DHTShutUp/DWTShutUp + FakePClk +
                                  * UseFakePClk + MufAddr */
    uint16_t riob;               /* last IOB seen — HM page 119 says
                                  * the buffer holds the value until
                                  * the next output command */

    /* Internal RAM-loading state. The DDC's HRam/Mixer/BMap/CMap
     * have a Keep'/LoadAddr/Write' protocol; an output command sets
     * Keep' (false → Dorado owns RAM), then subsequent commands
     * load address or write data with auto-increment. */
    uint16_t ram_addr;           /* current Dorado-side RAM address */
    uint8_t  ram_keep;           /* 0 = Dorado owns RAM; 1 = display owns */

    /* Cursor position (loaded from CursorX in CLCB). */
    uint16_t cursor_data[16];    /* 16-bit cursor sprite per scan line */

    /* Framebuffer: 808×606 mono, packed 8 pixels per byte, MSB = leftmost.
     * Phase 1: DWT's IOFetch← drops words here as a backdoor; eventually
     * the mixer + waveform generator will populate this from the FIFO. */
    uint8_t  fb[DORADO_DISPLAY_FB_BYTES];

    /* Scan position for synthetic time. Each Dorado clock advances
     * by `pixels_per_dorado_clock` (default 1 for now). Wraps at
     * (W * H) for full-frame redraw. */
    uint32_t scan_line;          /* current scan line 0..H-1 */
    uint32_t scan_pixel;         /* current pixel within line 0..W-1 */
    uint64_t frame_count;        /* full frames rendered */

    /* Per-channel "munch-fetch" state (HM page 117 αNextAddr / αNextCount). */
    uint16_t next_addr_lo[2];
    uint16_t next_addr_hi[2];
    uint16_t next_count[2];
    uint8_t  next_wcb_flag[2];   /* set by DHT, cleared by DWT */
    uint8_t  current_wcb_flag[2];

    /* DWT FIFO (per channel). 16 words × ~16 entries per spec; we use
     * 256 to be generous. Filled by IOFetch←, drained by mixer (we
     * draw to FB from here). */
    uint16_t fifo_a[256];
    uint16_t fifo_b[256];
    int      fifo_a_head, fifo_a_tail;
    int      fifo_b_head, fifo_b_tail;

    /* Slow-IO routing — registered by display_attach_to_io. */
    int      attached;

    /* Diagnostic counters. */
    uint64_t output_count;       /* total Output←B writes seen */
    uint64_t iofetch_count;      /* total IOFetch← munches received */
    uint64_t nlcb_writes;        /* DHT NLCB outputs */
} dorado_display;

void dorado_display_init(dorado_display *d);

/*
 * Attach this display to a slow-IO routing table. Registers a single
 * device on every TIOA address used by the DHT, DWT, AHT, AWT tasks,
 * with a single decode callback that handles the full DDC command
 * set. The exact TIOA-to-command map isn't yet pinned down (HM §11
 * is silent on the numeric values; we'll discover them by watching
 * what real microcode emits). For now everything routes through one
 * handler that records the (task, tioa, data) triple.
 *
 * Pass `task_a`/`task_b`/etc. = -1 to skip a task. By default register
 * on all four display tasks (DHT=3, AHT=4, AWT=11₈, DWT=13₈).
 */
void dorado_display_attach_to_io(dorado_display *d, dorado_io *io);

/*
 * Push a 16-bit word from main storage into the display FIFO. Used by
 * the IOFetch← path when DWT is the destination task.
 *
 * `subtask` (0 or 2) selects channel A or B. `word` is the data.
 * Returns 0 on success, -1 if FIFO full.
 */
int dorado_display_fifo_push(dorado_display *d, int subtask, uint16_t word);

/*
 * Snapshot the framebuffer as a PGM (mono) file. Returns 0 on success.
 * Useful for visual verification.
 */
int dorado_display_snapshot_pgm(const dorado_display *d, const char *path);

/*
 * Direct framebuffer pixel write — used for synthetic tests that don't
 * route through DWT. (x, y) is upper-left = (0, 0). pix = 0 (black) or
 * 1 (white). Real Alto convention: 0 = white, 1 = black; we use the
 * literal bit value and the snapshot inverts.
 */
void dorado_display_set_pixel(dorado_display *d, int x, int y, int pix);

/*
 * Drain the per-channel FIFO and render pixels into the framebuffer.
 * Phase 2 simplified: assumes 1-bit-per-pixel (Alto monitor /
 * αItemSize=1). Each FIFO word delivers 16 pixels, MSB = leftmost.
 *
 * `subtask` selects channel (0 = A, 2 = B). The function scans
 * across the framebuffer line by line, consuming FIFO words until
 * either the FIFO is empty or `dst_y` reaches the bottom of the
 * framebuffer. `dst_y` is incremented by the function as it goes.
 *
 * The mixer / waveform / pixel-clock machinery is NOT modeled; this
 * is just a "dump FIFO contents into FB at (0, dst_y)..(end, dst_y)
 * one row at a time" helper, useful for synthetic tests and for
 * loading a snapshot of what microcode put into the FIFO.
 *
 * Returns the number of pixels rendered.
 */
int dorado_display_render_fifo(dorado_display *d, int subtask, int *dst_y);

#endif
