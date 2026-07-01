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
 *   - Provide a simple framebuffer that DWT/AWT IOFetch← writes pixel
 *     bytes into (skipping the mixer/HSync/VBlank waveform machinery)
 *   - Dump a PPM/PGM snapshot of the framebuffer for visual inspection
 *
 * Display geometry: the framebuffer is sized to the largest monitor the
 * Dorado drives -- 1024×808 monochrome, the Cedar "lf" (large-format /
 * full-page) monitor (TerminalHeadDorado.mesa: lfWordsPerLine=64 -> 1024 px,
 * lfScanLines=808). The Alto-on-Dorado world uses a smaller 808×606 raster
 * and renders into the top-left; render_display_list reports the per-world
 * active extent in active_w/active_h so the frontends present at the world's
 * native size rather than padding the smaller world. 1024 / 8 = 128 bytes
 * per row, 128 × 808 = 103424 bytes total.
 *
 * NOT modeled yet:
 *   - Pixel clock generation (we use a synthetic raster/vblank clock)
 *   - Real HSync/VSync waveform timing
 *   - Full 7-wire interface back-channel keyboard/mouse encoding
 *   - 24Bit color mode (3-channel via AMap+BMap+CMap)
 *   - Mixer modes A8B2 / BBypass / dDAC outputs
 *   - Interlace (OddField)
 */

#define DORADO_DISPLAY_W            1024  /* framebuffer max (Cedar lf monitor) */
#define DORADO_DISPLAY_H            808
#define DORADO_DISPLAY_ROW_BYTES    ((DORADO_DISPLAY_W + 7) / 8)
#define DORADO_DISPLAY_FB_BYTES     (DORADO_DISPLAY_ROW_BYTES * DORADO_DISPLAY_H)
/* The Alto-on-Dorado world's raster (smaller than the framebuffer). */
#define DORADO_DISPLAY_ALTO_W       808
#define DORADO_DISPLAY_ALTO_H       606
/* Raw NLCB CursorX value corresponding to screen x = 0 (the microcode
 * sends a hardware-biased, descending value; calibrate against a known
 * cursor position). */
#define DORADO_DISPLAY_CURSOR_X_BIAS 2063

#define DORADO_DISPLAY_NLCB_WORDS   16
#define DORADO_DISPLAY_HRAM_WORDS   1024
#define DORADO_DISPLAY_MIXER_WORDS  1024  /* 1024 × 24 (HM page 112) */
#define DORADO_DISPLAY_KEY_WORDS    5      /* Alto-compatible boot words */

#define DORADO_DISPLAY_TASK_DHT     003
#define DORADO_DISPLAY_TASK_AHT     004
#define DORADO_DISPLAY_TASK_AWT     011
#define DORADO_DISPLAY_TASK_DWT     013

#define DORADO_DISPLAY_TIOA_DHTFLAG 0374
#define DORADO_DISPLAY_TIOA_DWTFLAG 0373
#define DORADO_DISPLAY_TIOA_AHTFLAG 0364
#define DORADO_DISPLAY_TIOA_AWTFLAG 0363
#define DORADO_DISPLAY_TIOA_NLCB    0376
#define DORADO_DISPLAY_TIOA_TNLCB   0366
#define DORADO_DISPLAY_TIOA_STATICS 0377
#define DORADO_DISPLAY_TIOA_TSTATICS 0367
#define DORADO_DISPLAY_TIOA_HRAM    0375
#define DORADO_DISPLAY_TIOA_DDCSTATUS 0370
#define DORADO_DISPLAY_TIOA_TSTATUS 0360

/* Monterey raster controller (`RastDefs.mc!5`). LT/WT are task-compatible
 * with DHT/DWT but use their own command block. */
#define DORADO_DISPLAY_TIOA_RAST_SELCMD  0320
#define DORADO_DISPLAY_TIOA_RAST_ADDRCMD 0321
#define DORADO_DISPLAY_TIOA_RAST_DATACMD 0322
#define DORADO_DISPLAY_TIOA_RAST_TASKCMD 0323
#define DORADO_DISPLAY_RAST_CHAN_WANTS_WT 0100000u
#define DORADO_DISPLAY_RAST_WT_SHUTUP     0100000u
#define DORADO_DISPLAY_RAST_LT_SHUTUP     0040000u

#define DORADO_DISPLAY_RAM_KEEP     0100000u
#define DORADO_DISPLAY_RAM_LOADADDR 0040000u
#define DORADO_DISPLAY_RAM_WRITE    0020000u

#define DORADO_DISPLAY_STATICS_DHT_SHUTUP 0001
#define DORADO_DISPLAY_STATICS_DWT_SHUTUP 0002
#define DORADO_DISPLAY_STATICS_ALL_SHUTUP \
    (DORADO_DISPLAY_STATICS_DHT_SHUTUP | DORADO_DISPLAY_STATICS_DWT_SHUTUP)

typedef enum {
    DORADO_KEY_NONE = 0,
    DORADO_KEY_5,
    DORADO_KEY_4,
    DORADO_KEY_6,
    DORADO_KEY_E,
    DORADO_KEY_7,
    DORADO_KEY_D,
    DORADO_KEY_U,
    DORADO_KEY_V,
    DORADO_KEY_0,
    DORADO_KEY_K,
    DORADO_KEY_MINUS,
    DORADO_KEY_P,
    DORADO_KEY_FSLASH,
    DORADO_KEY_BSLASH,
    DORADO_KEY_LF,
    DORADO_KEY_BS,
    DORADO_KEY_3,
    DORADO_KEY_2,
    DORADO_KEY_W,
    DORADO_KEY_Q,
    DORADO_KEY_S,
    DORADO_KEY_A,
    DORADO_KEY_9,
    DORADO_KEY_I,
    DORADO_KEY_X,
    DORADO_KEY_O,
    DORADO_KEY_L,
    DORADO_KEY_COMMA,
    DORADO_KEY_QUOTE,
    DORADO_KEY_RBRACKET,
    DORADO_KEY_BLANKMIDDLE,
    DORADO_KEY_BLANKTOP,
    DORADO_KEY_1,
    DORADO_KEY_ESC,
    DORADO_KEY_TAB,
    DORADO_KEY_F,
    DORADO_KEY_CTRL,
    DORADO_KEY_C,
    DORADO_KEY_J,
    DORADO_KEY_B,
    DORADO_KEY_Z,
    DORADO_KEY_LSHIFT,
    DORADO_KEY_PERIOD,
    DORADO_KEY_SEMICOLON,
    DORADO_KEY_RETURN,
    DORADO_KEY_ARROW,
    DORADO_KEY_DEL,
    DORADO_KEY_R,
    DORADO_KEY_T,
    DORADO_KEY_G,
    DORADO_KEY_Y,
    DORADO_KEY_H,
    DORADO_KEY_8,
    DORADO_KEY_N,
    DORADO_KEY_M,
    DORADO_KEY_LOCK,
    DORADO_KEY_SPACE,
    DORADO_KEY_LBRACKET,
    DORADO_KEY_PLUS,
    DORADO_KEY_RSHIFT,
    DORADO_KEY_BLANKBOTTOM,
    DORADO_KEY_LAST
} dorado_display_key;

typedef struct {
    uint8_t  valid;
    uint8_t  odd;
    uint8_t  absolute_y;
    uint16_t line;
    uint16_t y;
    uint16_t lmarg;
    uint16_t width;
    uint16_t ptr;
    uint16_t scan;
    uint16_t words[256];
    uint16_t word_count;
    uint16_t overflow;
} dorado_display_ddc_line;

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

    /* Terminal keyboard state in the Alto-compatible complemented
     * word format used by Initial: 1 = key up, 0 = key down. Word
     * 0..3 are the normal boot keyboard words; word 4 is Star-only. */
    uint16_t keyboard_words[DORADO_DISPLAY_KEY_WORDS];
    uint32_t boot_button_scanlines;  /* remaining terminal-status 1 bits */
    uint8_t  terminal_msg_word;      /* next keyboard word message, 0..3 */
    uint8_t  terminal_msg_bit;       /* next serial bit within 32-bit msg */
    uint64_t terminal_bits;          /* serial keyboard bits served */
    uint64_t terminal_messages;      /* complete keyboard messages served */

    /* Framebuffer: up to 1024×808 mono, packed 8 pixels per byte, MSB =
     * leftmost. Phase 1: DWT/AWT IOFetch← drops words here as a backdoor;
     * eventually the mixer + waveform generator will populate this
     * from the FIFO. */
    uint8_t  fb[DORADO_DISPLAY_FB_BYTES];

    /* Active raster extent of the most recently rendered world, so the
     * frontends present at the world's native size (Alto 808×606, Cedar lf
     * 1024×808) instead of padding to the full framebuffer. Set by
     * dorado_machine_render_display_list; 0 until the first render. */
    int      active_w;
    int      active_h;

    /* Scan position for synthetic time. The GUI may inspect/render the
     * framebuffer at any point, including mid-frame. frame_count only
     * advances when the raster crosses the modeled vertical blank
     * boundary, or when a headless harness explicitly marks vblank. */
    uint32_t scan_line;          /* current scan line 0..H-1 */
    uint32_t scan_pixel;         /* current pixel within line 0..W-1 */
    uint64_t frame_count;        /* completed frames at vblank */

    /* Per-channel "munch-fetch" state (HM page 117 αNextAddr / αNextCount). */
    uint16_t next_addr_lo[2];
    uint16_t next_addr_hi[2];
    uint16_t next_count[2];
    uint8_t  next_wcb_flag[2];   /* set by DHT, cleared by DWT */
    uint8_t  current_wcb_flag[2];
    uint16_t wcb_draw_x[2];       /* direct-render cursor for live WCB data */
    uint16_t wcb_draw_y[2];
    uint8_t  ddc_va_base_valid;   /* debug VA-derived DDC renderer state */
    uint32_t ddc_va_base;
    uint16_t ddc_seq_y;           /* debug DDC sequential field renderer */
    dorado_display_ddc_line ddc_pending_line[2];
    dorado_display_ddc_line ddc_current_line[2];

    /* Monterey raster LT/WT handoff (`RastMain.mc!6`). The raster word
     * task supports four channels by SubTask; for now the renderer keeps
     * A/B FIFOs, but preserving the four wake channels prevents the LT
     * command protocol from collapsing into terminal-display state. */
    uint8_t  raster_lt_enabled;
    uint8_t  raster_next_wt_flag[4];
    uint8_t  raster_current_wt_flag[4];
    uint8_t  raster_next_channel;
    uint16_t raster_taskcmd;

    /* Display word-task FIFO (per channel). 16 words × ~16 entries per
     * spec; we use 256 to be generous. Filled by IOFetch←, drained by
     * mixer (we draw to FB from here). */
    uint16_t fifo_a[256];
    uint16_t fifo_b[256];
    int      fifo_a_head, fifo_a_tail;
    int      fifo_b_head, fifo_b_tail;

    /* Slow-IO routing — registered by display_attach_to_io. */
    int      attached;
    int      terminal_task;      /* DHT or AHT selected by Initial */

    /* Diagnostic counters. */
    uint64_t output_count;       /* total Output←B writes seen */
    uint64_t output_task_count[16]; /* Output←B writes by task */
    uint64_t output_tioa_count[256]; /* Output←B writes by TIOA */
    uint16_t output_tioa_first[256]; /* first data word written by TIOA */
    uint16_t output_tioa_last[256];  /* most recent data word by TIOA */
    uint64_t iofetch_count;      /* total IOFetch← munches received */
    uint32_t first_iofetch_va;   /* diagnostic: VA of first display IOFetch */
    uint32_t last_iofetch_va;    /* diagnostic: VA of most recent IOFetch */
    uint16_t first_iofetch_words[16];
    uint16_t last_iofetch_words[16];
    uint64_t nlcb_writes;        /* DHT NLCB outputs */
    uint64_t ddc_va_words_drawn;  /* debug: words drawn by VA-derived path */
    uint64_t ddc_lines_rendered;  /* default line renderer diagnostics */
    uint64_t ddc_pixels_rendered;
    uint64_t ddc_zero_word_lines;
    uint64_t ddc_short_lines;
    uint16_t ddc_last_width;
    uint16_t ddc_last_ptr;
    uint16_t ddc_last_line;
    uint16_t ddc_last_word_count;
    uint16_t ddc_last_nonzero_words;
    uint16_t ddc_last_overflow;
    uint8_t  dwt_trace_active[2];
    uint16_t dwt_trace_words[2];
    uint16_t dwt_trace_nonzero[2];
    uint32_t dwt_trace_first_va[2];
    uint32_t dwt_trace_last_va[2];
    uint64_t dwt_trace_lines;

    /* DispY NLCB cursor rendering (DisplayDefs.mc: register select in
     * the top 4 bits of each NLCB output, data in the low 12).
     * Reg 0 = VCW (bit1 VSync, bit2 VBlank, bit0 field); reg 15B =
     * CursorX; regs 16B/17B = CursorLo/CursorHi, one pair per visible
     * scanline (DisplayMain.mc SendCursor / BeyondCursor — every line
     * outputs a pair, zero when the cursor does not intersect). A
     * CursorHi write closes the line: if the assembled 16-bit row is
     * nonzero it is OR-drawn into the framebuffer at the tracked
     * scanline, then the line counter advances. */
    uint16_t nlcb_cursor_x;      /* last CursorX value (raw 12 bits) */
    uint8_t  nlcb_cursor_lo;     /* pending CursorLo byte */
    uint16_t nlcb_line;          /* scanlines since VSync */
    uint8_t  nlcb_field_odd;     /* VCW bit 0 at VSync */
    uint64_t cursor_rows_drawn;  /* nonzero cursor rows rendered */
    uint64_t scanline_ticks;     /* synthetic horizontal timing ticks */
    uint64_t terminal_wakeups;   /* DHT/AHT wakeups requested */
    uint64_t dwt_wakeups;        /* DWT wakeups requested */
} dorado_display;

void dorado_display_init(dorado_display *d);

/* Headless keyboard API. SDL or another frontend should translate host
 * key events into these same calls; the emulator core only consumes
 * the complemented Alto-style words. */
void     dorado_display_keyboard_all_up(dorado_display *d);
void     dorado_display_keyboard_set_word(dorado_display *d, int word,
                                          uint16_t value);
uint16_t dorado_display_keyboard_word(const dorado_display *d, int word);
void     dorado_display_keyboard_set_bit(dorado_display *d, int word,
                                         int bit, int down);
void     dorado_display_keyboard_set_key(dorado_display *d,
                                         dorado_display_key key,
                                         int down);
/* Map a textual key name (e.g. "bs", "quote", "ctrl", "return", or a
 * single character like "a"/"5") to its dorado_display_key. Returns
 * DORADO_KEY_NONE for an unrecognized name. Used to parse the boot-key
 * chord from the command line. */
dorado_display_key dorado_display_key_from_name(const char *name);
void     dorado_display_boot_button(dorado_display *d, uint32_t scanlines);

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
 * the IOFetch← path when DWT/AWT is the destination task.
 *
 * `subtask` (0 or 2) selects channel A or B. `word` is the data.
 * Returns 0 on success, -1 if FIFO full.
 */
int dorado_display_fifo_push(dorado_display *d, int subtask, uint16_t word);
int dorado_display_iofetch_word(dorado_display *d, int subtask,
                                uint32_t va, uint16_t word);

/*
 * Snapshot the framebuffer as a PGM (mono) file. Returns 0 on success.
 * Useful for visual verification.
 */
int dorado_display_snapshot_pgm(const dorado_display *d, const char *path);

/* Headless frame clock. The framebuffer is always owned by the
 * emulator core; GUI frontends should render this same buffer. These
 * helpers provide a stable frame counter for headless runs such as
 * "run 100 frames, dump frame 100".
 *
 * `advance_pixels` advances the synthetic raster position and returns
 * the number of vertical-blank boundaries crossed. `vblank` forces one
 * complete frame boundary using the current framebuffer contents. */
uint64_t dorado_display_frame(const dorado_display *d);
int      dorado_display_advance_pixels(dorado_display *d, uint32_t pixels);
void     dorado_display_vblank(dorado_display *d);

/* Synthetic horizontal timing. Returns DHT/AHT task number to wake,
 * or -1 if the terminal horizontal task has not been initialized yet.
 * This is a timing shim until the DDC pixel clock and HBlank waveform
 * are modeled from the actual control outputs. */
int      dorado_display_scanline_tick(dorado_display *d);
uint16_t dorado_display_scanline_wakeup_mask(dorado_display *d);
int      dorado_display_dwt_wakeup(dorado_display *d, int *subtask);

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
