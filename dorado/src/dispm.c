/*
 * DispM — the Dorado colour display board. See include/dispm.h for the
 * sources this is transcribed from; every constant below is quoted from
 * ColorDisplayDorado.mesa or ColorDisplayHeadDorado.mesa rather than guessed.
 *
 * Xerox bit numbering throughout: bit 0 is the MSB of a 16-bit word, so a
 * field declared (0:4..7) occupies bits 11..8 in C.
 */

#include "dispm.h"
#include "trace.h"

#include <stdio.h>
#include <string.h>

/* File-scope, not a dorado_machine member — see the header. */
static struct {
    uint8_t           present;
    dorado_dispm_type type;
    uint16_t          width, height;

    /* RAM-load state. ColorDisplayDorado.mesa gives MixerDatum and BCDatum
     * as keep/write/load flags plus a body that is either an address or
     * data; the head drives each RAM with the sequence
     *   keep:T                    -- take the RAM away from the video system
     *   keep:T load:T addr:...    -- set the load address
     *   keep:T write:T data:...   -- write, address auto-increments
     *   keep:F                    -- give it back
     * "keep" is inverted in the wire encoding: T(0), F(1). */
    uint8_t  mixer_keep, bmap_keep, cmap_keep;
    uint16_t mixer_addr, bmap_addr, cmap_addr;
    uint8_t  mixer_sel_hi;
    uint16_t mixer[DORADO_DISPM_MIXER_WORDS][2];   /* [addr][lo,hi] */
    uint8_t  bmap[DORADO_DISPM_BC_ENTRIES];
    uint8_t  cmap[DORADO_DISPM_BC_ENTRIES];

    uint64_t reads_board, reads_type;
    uint64_t writes_mixer, writes_bmap, writes_cmap;
    uint64_t renders;
    uint16_t last_scan_control;
    uint32_t last_bitmap;
    uint32_t last_table_a;
    unsigned atable_nonzero;

    int      rgb_w, rgb_h;
    uint8_t  rgb[DORADO_DISPM_MAX_W * DORADO_DISPM_MAX_H * 3];
} dm;

void dorado_dispm_reset(void)
{
    memset(&dm, 0, sizeof dm);
}

void dorado_dispm_install(dorado_dispm_type type)
{
    dm.type = type;
    dm.present = (type != DORADO_DISPM_NONE);
    /* SetDisplayType, verbatim. "must be a multiple of 32" / "of 2". */
    switch (type) {
    case DORADO_DISPM_STANDARD: dm.width = 640;  dm.height = 480;  break;
    case DORADO_DISPM_HIGHRES:  dm.width = 1024; dm.height = 768;  break;
    default:                    dm.width = 0;    dm.height = 0;    break;
    }
}

dorado_dispm_type dorado_dispm_installed(void) { return dm.type; }

/* ---- Slow I/O ----------------------------------------------------------- */

/* The head's presence test:
 *
 *   colorBoardData _ InputNoPE[360B];
 *   IF colorBoardData#0 THEN {                    -- board is installed
 *     colorDeviceData _ InputNoPE[361B];
 *     SELECT colorDeviceData.type FROM            -- type is bits 0..3
 *       17B => displayType _ standard;
 *       16B => displayType _ highResolution;
 *       ENDCASE;                                  -- leaves it `none`
 *
 * With no board, both must read ZERO. That matters more than it looks: the
 * head uses InputNoPE precisely because it expects a floating bus, and our
 * io.c returns 0xFFFF for an unregistered cell — which is non-zero AND has
 * 17B in its top four bits, so an unmodelled read would announce "a standard
 * 640x480 colour board is installed" by accident. Registering this device and
 * answering 0 is what makes "absent" mean absent. */
static uint16_t dispm_input(void *ctx, int task, int subtask, uint8_t tioa,
                            int *out_bad_parity)
{
    (void)ctx; (void)task; (void)subtask;
    if (out_bad_parity) *out_bad_parity = 0;
    switch (tioa) {
    case DORADO_DISPM_TIOA_BOARD:
        dm.reads_board++;
        return dm.present ? 0177777u : 0u;
    case DORADO_DISPM_TIOA_MIXER: {
        dm.reads_type++;
        if (!dm.present) return 0;
        unsigned code = (dm.type == DORADO_DISPM_HIGHRES) ? 016u : 017u;
        return (uint16_t)(code << 12);           /* type = bits 0..3 */
    }
    default:
        return 0;
    }
}

/* MixerDatum / BCDatum share a control prefix:
 *   keep (0:0)  T(0) F(1)      -- Dorado owns the RAM while keep is T
 *   write(0:1)  T(0) F(1)
 *   load (0:2)  F(0) T(1)
 *   unused(0:3)
 * then the body. For BCDatum the body is 4 zero bits + an 8-bit addr or
 * data; for MixerDatum it is a 12-bit field, with the address form carrying
 * a 10-bit addr plus a lo/hi select. */
#define CTL_KEEP(w)   ((((w) >> 15) & 1u) == 0u)   /* T(0) means "keep" */
#define CTL_WRITE(w)  ((((w) >> 14) & 1u) == 0u)
#define CTL_LOAD(w)   ((((w) >> 13) & 1u) != 0u)

static void dispm_output(void *ctx, int task, int subtask, uint8_t tioa,
                         uint16_t data)
{
    (void)ctx; (void)task; (void)subtask;
    int keep = CTL_KEEP(data), wr = CTL_WRITE(data), ld = CTL_LOAD(data);

    switch (tioa) {
    case DORADO_DISPM_TIOA_MIXER:
        dm.writes_mixer++;
        dm.mixer_keep = (uint8_t)keep;
        if (ld) {
            /* addr => [x: [0..1], addr: [0..1777B], select: {lo,hi}] */
            dm.mixer_addr  = (uint16_t)((data >> 1) & 01777u);
            dm.mixer_sel_hi = (uint8_t)(data & 1u);
        } else if (wr) {
            dm.mixer[dm.mixer_addr % DORADO_DISPM_MIXER_WORDS]
                    [dm.mixer_sel_hi & 1] = (uint16_t)(data & 07777u);
            if (dm.mixer_sel_hi)
                dm.mixer_addr = (uint16_t)((dm.mixer_addr + 1u)
                                           % DORADO_DISPM_MIXER_WORDS);
        }
        break;
    case DORADO_DISPM_TIOA_BMAP:
    case DORADO_DISPM_TIOA_CMAP: {
        int is_b = (tioa == DORADO_DISPM_TIOA_BMAP);
        uint8_t  *ram  = is_b ? dm.bmap : dm.cmap;
        uint16_t *addr = is_b ? &dm.bmap_addr : &dm.cmap_addr;
        uint8_t  *kp   = is_b ? &dm.bmap_keep : &dm.cmap_keep;
        if (is_b) dm.writes_bmap++; else dm.writes_cmap++;
        *kp = (uint8_t)keep;
        if (ld) {
            *addr = (uint16_t)(data & 0377u);
        } else if (wr) {
            ram[*addr & 0377u] = (uint8_t)(data & 0377u);
            *addr = (uint16_t)((*addr + 1u) & 0377u);   /* auto-increment */
        }
        break;
    }
    default:
        break;
    }
    if (dorado_trace_flag("DORADO_DISPM_TRACE"))
        fprintf(stderr, "DISPM out tioa=%03o data=%06o keep=%d wr=%d ld=%d "
                "maddr=%04o baddr=%03o caddr=%03o\n",
                tioa, data, keep, wr, ld, dm.mixer_addr,
                dm.bmap_addr, dm.cmap_addr);
}

void dorado_dispm_attach_to_io(dorado_io *io)
{
    static dorado_io_device dev;
    dev.read  = dispm_input;
    dev.write = dispm_output;
    dev.ctx   = NULL;
    dev.name  = "DispM(colour)";
    /* The EMULATOR task only. ColorDisplayHeadDorado is Mesa code, so its
     * Input/Output execute in task 0; display.c owns the same TIOAs for the
     * display tasks and the two must not tread on each other. */
    dorado_io_register(io, 0, DORADO_DISPM_TIOA_BOARD, &dev);
    dorado_io_register(io, 0, DORADO_DISPM_TIOA_MIXER, &dev);
    dorado_io_register(io, 0, DORADO_DISPM_TIOA_CMAP,  &dev);
    dorado_io_register(io, 0, DORADO_DISPM_TIOA_BMAP,  &dev);
}

/* ---- The control-block chain -------------------------------------------- */

/* ColorCSB at 177414B -> MonitorControlBlock -> channels + colour tables.
 * This is the twin of the monochrome DCB chain at 0420 that DHT/DWT walk;
 * the Mesa driver only arms it, the hardware follows it every field. */
int dorado_dispm_render(uint16_t (*read_word)(void *ctx, uint32_t va),
                        void *ctx)
{
    if (!dm.present || !read_word) return 0;

    uint16_t mcb = read_word(ctx, DORADO_DISPM_CSB_VA);
    if (mcb <= 1u) return 0;                       /* RNIL */

    uint16_t seal = read_word(ctx, mcb + 0u);
    if (seal != DORADO_DISPM_MCB_SEAL) return 0;   /* not armed yet */

    uint16_t chan_a = read_word(ctx, mcb + 2u);
    uint16_t color  = read_word(ctx, mcb + 4u);
    if (chan_a <= 1u || color <= 1u) return 0;

    /* ChannelControlBlock, 8 words:
     *   0 link, 1 wordsPerLine, 2-3 address (LONG POINTER), 4 linesPerField,
     *   5 pixelsPerLine, 6 leftMargin, 7 scanControl */
    uint16_t wpl        = read_word(ctx, chan_a + 1u);
    uint32_t bitmap     = (uint32_t)read_word(ctx, chan_a + 2u)
                        | ((uint32_t)read_word(ctx, chan_a + 3u) << 16);
    uint16_t lines      = read_word(ctx, chan_a + 4u);
    uint16_t ppl        = read_word(ctx, chan_a + 5u);
    uint16_t scan       = read_word(ctx, chan_a + 7u);
    dm.last_scan_control = scan;
    dm.last_bitmap = bitmap;

    /* ScanControl (0:0..5 unused, 6 mode24, 7 aChannelOnly, 8 bBypass,
     * 9 pixelMode, 10..11 resolution, 12..15 bitsPerPixel). */
    unsigned bpp = scan & 017u;
    if (bpp != 1 && bpp != 2 && bpp != 4 && bpp != 8) bpp = 8;

    /* ColorControlBlock: tableA is the first LONG POINTER. */
    uint32_t table_a = (uint32_t)read_word(ctx, color + 0u)
                     | ((uint32_t)read_word(ctx, color + 1u) << 16);
    if (table_a == 0) return 0;

    /* pixelsPerLine carries pixelsPerLineOffset (400B); linesPerField is
     * height/2 because the monitor is interlaced. */
    int w = (int)ppl - (int)DORADO_DISPM_PPL_OFFSET;
    int h = (int)lines * 2;
    if (w <= 0 || h <= 0) { w = dm.width; h = dm.height; }
    if (w > DORADO_DISPM_MAX_W) w = DORADO_DISPM_MAX_W;
    if (h > DORADO_DISPM_MAX_H) h = DORADO_DISPM_MAX_H;

    unsigned per_word = 16u / bpp;
    unsigned mask = (bpp >= 16) ? 0xFFFFu : ((1u << bpp) - 1u);
    int painted = 0;

    for (int y = 0; y < h; y++) {
        uint32_t row = bitmap + (uint32_t)y * (uint32_t)wpl;
        for (int x = 0; x < w; x++) {
            unsigned wi = (unsigned)x / per_word;
            unsigned sh = 16u - bpp * (((unsigned)x % per_word) + 1u);
            uint16_t word = read_word(ctx, row + wi);
            unsigned idx = (word >> sh) & mask;

            /* AEntry, 2 words:
             *   w0: zeroL(0:0..3) redL(0:4..7) blue(0:8..15)
             *   w1: zeroH(1:0..3) green(1:4..11) redH(1:12..15) */
            uint32_t ea = table_a + (uint32_t)idx * 2u;
            uint16_t e0 = read_word(ctx, ea + 0u);
            uint16_t e1 = read_word(ctx, ea + 1u);
            unsigned red = (unsigned)(((e1 & 017u) << 4) | ((e0 >> 8) & 017u));
            unsigned grn = (unsigned)((e1 >> 4) & 0377u);
            unsigned blu = (unsigned)(e0 & 0377u);

            uint8_t *p = &dm.rgb[((size_t)y * (size_t)w + (size_t)x) * 3u];
            p[0] = (uint8_t)red; p[1] = (uint8_t)grn; p[2] = (uint8_t)blu;
            painted++;
        }
    }
    /* How much of the colour map is actually loaded. An all-black image has
     * two very different causes and this separates them: an EMPTY colour
     * screen (nothing moved onto it yet, so every pixel indexes ATable[0]
     * which is legitimately black) versus a table we are not reading at all
     * (wrong pointer, wrong stride). Count the non-zero entries once per
     * render rather than guessing from the picture. */
    dm.atable_nonzero = 0;
    for (unsigned i = 0; i < DORADO_DISPM_MIXER_WORDS; i++) {
        if (read_word(ctx, table_a + i * 2u) ||
            read_word(ctx, table_a + i * 2u + 1u))
            dm.atable_nonzero++;
    }
    dm.last_table_a = table_a;
    dm.rgb_w = w; dm.rgb_h = h; dm.renders++;
    return painted;
}

const uint8_t *dorado_dispm_rgb(int *out_w, int *out_h)
{
    if (out_w) *out_w = dm.rgb_w;
    if (out_h) *out_h = dm.rgb_h;
    return dm.rgb_w ? dm.rgb : NULL;
}

int dorado_dispm_snapshot_ppm(const char *path)
{
    if (!dm.rgb_w || !dm.rgb_h) return -1;
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fprintf(f, "P6\n%d %d\n255\n", dm.rgb_w, dm.rgb_h);
    fwrite(dm.rgb, 3u, (size_t)dm.rgb_w * (size_t)dm.rgb_h, f);
    fclose(f);
    return 0;
}

void dorado_dispm_dump(void)
{
    static const char *tn[] = { "none", "standard 640x480",
                                "highResolution 1024x768" };
    fprintf(stderr,
            "[dispm] board=%s type=%s  presence reads: 360B=%llu 361B=%llu\n"
            "[dispm] RAM writes: mixer=%llu bmap=%llu cmap=%llu  "
            "keep(m/b/c)=%d/%d/%d\n"
            "[dispm] renders=%llu last scanControl=%06o bitmap=%07o  "
            "image=%dx%d\n",
            dm.present ? "installed" : "absent",
            tn[dm.type <= DORADO_DISPM_HIGHRES ? dm.type : 0],
            (unsigned long long)dm.reads_board,
            (unsigned long long)dm.reads_type,
            (unsigned long long)dm.writes_mixer,
            (unsigned long long)dm.writes_bmap,
            (unsigned long long)dm.writes_cmap,
            dm.mixer_keep, dm.bmap_keep, dm.cmap_keep,
            (unsigned long long)dm.renders,
            dm.last_scan_control, dm.last_bitmap, dm.rgb_w, dm.rgb_h);
    if (dm.renders) {
        /* ScanControl, Xerox bit order: 0..5 unused, 6 mode24, 7
         * aChannelOnly, 8 bBypass, 9 pixelMode, 10..11 resolution,
         * 12..15 bitsPerPixel. */
        uint16_t sc = dm.last_scan_control;
        fprintf(stderr,
                "[dispm] scanControl: bpp=%u res=%u pixelMode=%s bBypass=%d "
                "aChannelOnly=%d mode24=%d\n",
                (unsigned)(sc & 017u), (unsigned)((sc >> 4) & 3u),
                ((sc >> 6) & 1u) ? "a8b2" : "a6b4",
                (int)((sc >> 7) & 1u), (int)((sc >> 8) & 1u),
                (int)((sc >> 9) & 1u));
        fprintf(stderr,
                "[dispm] ATable at %07o: %u/%u entries non-zero%s\n",
                dm.last_table_a, dm.atable_nonzero,
                DORADO_DISPM_MIXER_WORDS,
                dm.atable_nonzero == 0
                  ? "  <- the colour map is EMPTY: either nothing has been"
                    " moved onto the colour screen, or the tableA pointer is"
                    " wrong"
                  : "");
    }
    if (dm.reads_board == 0)
        fprintf(stderr, "[dispm] the guest never asked whether a board is "
                        "present -- ColorDisplayHeadDorado has not started, "
                        "or this is not a Cedar world.\n");
}
