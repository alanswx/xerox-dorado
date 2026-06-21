#include "disk.h"

#include "pdi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cached per-name trace flag (cpu.c). Keep disk trace checks off the hot
 * path's raw getenv() — see dorado/CLAUDE.md "Don't regress". */
extern int dorado_trace_flag(const char *name);
extern int dorado_trace_gate;
extern unsigned long long dorado_trace_cycle;

const dorado_disk_geometry DORADO_DISK_T80  = { 815, 5, 9, 0, 0, 0 };
const dorado_disk_geometry DORADO_DISK_T300 = { 815, 19, 9, 0, 0, 0 };
/* Diablo-on-Trident: full T-80 cylinder range with 29 short sectors.
 * AltoDiabloDisk.mc maps Diablo drive 0 to cyl+3 and drive 1 to cyl+0406+3,
 * so a 206-cylinder truncated pack fails as soon as Alto software addresses
 * the second emulated Diablo drive. */
const dorado_disk_geometry DORADO_DISK_DIABLO = { 815, 5, 29, 2, 8, 256 };

int dorado_disk_geom_header_words(const dorado_disk_geometry *g)
{ return g->header_words ? g->header_words : DORADO_DISK_HEADER_WORDS; }
int dorado_disk_geom_label_words(const dorado_disk_geometry *g)
{ return g->label_words ? g->label_words : DORADO_DISK_LABEL_WORDS; }
int dorado_disk_geom_data_words(const dorado_disk_geometry *g)
{ return g->data_words ? g->data_words : DORADO_DISK_DATA_WORDS; }

/* On-disk bytes per sector for a geometry: dummy(1w) + hdr + lbl + data. */
static long disk_geom_sector_bytes(const dorado_disk_geometry *g)
{
    return (long)(DORADO_DISK_DUMMY_WORDS +
                  dorado_disk_geom_header_words(g) +
                  dorado_disk_geom_label_words(g) +
                  dorado_disk_geom_data_words(g)) * 2;
}

/* ─── Fire Code ECC (HM §9.10, DskEth sheet 16) ─────────────────────
 * Polynomial P(X) = X^32 + X^23 + X^21 + X^11 + X^2 + 1 implemented as a
 * 32-stage LFSR. The feedback taps (X powers below 32) are at bits
 * {0, 2, 11, 21, 23}. Data is fed MSB-first; after the data the register
 * holds the 2-word check value. We generate-and-check (the on-disk packs we
 * have carry no stored ECC), so the implementation is self-consistent rather
 * than bit-matched to a physical drive; single-burst recovery (HM §9.10
 * Chinese-Remainder) is deferred until microcode faults on an injected error
 * (plan D3). */
#define DORADO_DISK_ECC_TAPS \
    ((1u << 0) | (1u << 2) | (1u << 11) | (1u << 21) | (1u << 23))

void dorado_disk_ecc_compute(const uint16_t *words, int n,
                             uint16_t *ecc_hi, uint16_t *ecc_lo)
{
    uint32_t reg = 0;
    for (int i = 0; i < n; i++) {
        uint16_t w = words[i];
        for (int b = 15; b >= 0; b--) {
            int bit = (w >> b) & 1;
            int fb  = bit ^ (int)((reg >> 31) & 1u);
            reg <<= 1;
            if (fb) reg ^= DORADO_DISK_ECC_TAPS;
        }
    }
    if (ecc_hi) *ecc_hi = (uint16_t)(reg >> 16);
    if (ecc_lo) *ecc_lo = (uint16_t)(reg & 0xFFFFu);
}

int dorado_disk_ecc_check(const uint16_t *words, int n,
                          uint16_t ecc_hi, uint16_t ecc_lo)
{
    uint16_t chi = 0, clo = 0;
    dorado_disk_ecc_compute(words, n, &chi, &clo);
    return (chi == ecc_hi && clo == ecc_lo) ? 0 : 1;   /* 0 = ok, 1 = error */
}

#define DORADO_DISK_SUBSECTOR_PULSES_PER_REV 117

#define DORADO_DISK_TAG_DRIVE    0x8000u
#define DORADO_DISK_TAG_CYLINDER 0x4000u
#define DORADO_DISK_TAG_HEAD     0x2000u
#define DORADO_DISK_TAG_CONTROL  0x1000u
#define DORADO_DISK_TAG_BUS      0x0FFFu

#define DORADO_DISK_MUFF_CLEAR_COMPARE_ERR 0x2000u
#define DORADO_DISK_MUFF_SET_CHECKSUM_ERR  0x1000u
#define DORADO_DISK_MUFF_CLEAR_INDEX_TW    0x0800u
#define DORADO_DISK_MUFF_CLEAR_SECTOR_TW   0x0400u
#define DORADO_DISK_MUFF_CLEAR_SEEKTAG_TW  0x0200u
#define DORADO_DISK_MUFF_CLEAR_ERRORS      0x0100u
#define DORADO_DISK_MUFF_ADDR              0x00FFu

static void disk_set_subsector_count(dorado_disk_drive *d, int count);
static void disk_update_fifo_tws(dorado_disk_controller *ctl);

static int disk_trace_enabled(const char *name)
{
    if (!dorado_trace_flag(name)) return 0;
    return !dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate;
}

/* ─── Pack image I/O ────────────────────────────────────────────── */

static int sector_index(const dorado_disk_geometry *g,
                        int cyl, int head, int sec)
{
    if (cyl < 0 || cyl >= g->cylinders) return -1;
    if (head < 0 || head >= g->heads) return -1;
    if (sec < 0 || sec >= g->sectors) return -1;
    return ((cyl * g->heads) + head) * g->sectors + sec;
}

dorado_disk_sector *dorado_disk_pack_sector(dorado_disk_pack *pack,
                                            int cyl, int head, int sec)
{
    int idx = sector_index(&pack->geometry, cyl, head, sec);
    if (idx < 0 || idx >= pack->num_sectors) return NULL;
    return &pack->sectors[idx];
}

int dorado_disk_pack_create(dorado_disk_pack *pack,
                            const dorado_disk_geometry *geom)
{
    memset(pack, 0, sizeof *pack);
    pack->geometry = *geom;
    pack->num_sectors = geom->cylinders * geom->heads * geom->sectors;
    pack->sectors = calloc(pack->num_sectors, sizeof(dorado_disk_sector));
    if (!pack->sectors) {
        memset(pack, 0, sizeof *pack);
        return -1;
    }
    return 0;
}

/* Load a pack image. Layout per ContrAlto2 / Bitsavers:
 *   for each (cyl, head, sec):
 *     2 dummy bytes + header (2 words = 4 bytes) + label (10w = 20 b)
 *       + data (1024w = 2048 b)   = 2074 bytes/sector
 * Words are stored little-endian on disk.
 */
int dorado_disk_pack_load(dorado_disk_pack *pack,
                          const dorado_disk_geometry *geom,
                          const char *path)
{
    if (dorado_disk_pack_create(pack, geom) != 0) return -1;
    snprintf(pack->path, sizeof pack->path, "%s", path);

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        dorado_disk_pack_free(pack);
        return -1;
    }

    int hw = dorado_disk_geom_header_words(geom);
    int lw = dorado_disk_geom_label_words(geom);
    int dw = dorado_disk_geom_data_words(geom);
    long sector_bytes = disk_geom_sector_bytes(geom);

    /* Validate file size. */
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); dorado_disk_pack_free(pack); return -1; }
    long expected = (long)pack->num_sectors * sector_bytes;
    long actual   = ftell(fp);
    if (actual != expected) {
        fclose(fp);
        dorado_disk_pack_free(pack);
        return -1;
    }
    rewind(fp);

    uint8_t buf[DORADO_DISK_SECTOR_BYTES];   /* max-sized scratch */
    for (int i = 0; i < pack->num_sectors; i++) {
        if (fread(buf, 1, (size_t)sector_bytes, fp) != (size_t)sector_bytes) {
            fclose(fp);
            dorado_disk_pack_free(pack);
            return -1;
        }
        dorado_disk_sector *s = &pack->sectors[i];
        /* Skip the dummy word(s) at offset 0. */
        int off = DORADO_DISK_DUMMY_WORDS * 2;
        for (int w = 0; w < hw; w++) {
            s->header[w] = (uint16_t)(buf[off] | (buf[off+1] << 8));
            off += 2;
        }
        for (int w = 0; w < lw; w++) {
            s->label[w] = (uint16_t)(buf[off] | (buf[off+1] << 8));
            off += 2;
        }
        for (int w = 0; w < dw; w++) {
            s->data[w] = (uint16_t)(buf[off] | (buf[off+1] << 8));
            off += 2;
        }
        s->modified = 0;
    }
    fclose(fp);
    return 0;
}

int dorado_disk_pack_save(const dorado_disk_pack *pack)
{
    if (pack->path[0] == '\0') return -1;
    if (pack->read_only) return -1;
    FILE *fp = fopen(pack->path, "wb");
    if (!fp) return -1;
    int hw = dorado_disk_geom_header_words(&pack->geometry);
    int lw = dorado_disk_geom_label_words(&pack->geometry);
    int dw = dorado_disk_geom_data_words(&pack->geometry);
    long sector_bytes = disk_geom_sector_bytes(&pack->geometry);
    uint8_t buf[DORADO_DISK_SECTOR_BYTES];   /* max-sized scratch */
    for (int i = 0; i < pack->num_sectors; i++) {
        const dorado_disk_sector *s = &pack->sectors[i];
        memset(buf, 0, sizeof buf);
        int off = DORADO_DISK_DUMMY_WORDS * 2;   /* dummy word(s) */
        for (int w = 0; w < hw; w++) {
            buf[off++] = (uint8_t)(s->header[w] & 0xFF);
            buf[off++] = (uint8_t)(s->header[w] >> 8);
        }
        for (int w = 0; w < lw; w++) {
            buf[off++] = (uint8_t)(s->label[w] & 0xFF);
            buf[off++] = (uint8_t)(s->label[w] >> 8);
        }
        for (int w = 0; w < dw; w++) {
            buf[off++] = (uint8_t)(s->data[w] & 0xFF);
            buf[off++] = (uint8_t)(s->data[w] >> 8);
        }
        if (fwrite(buf, 1, (size_t)sector_bytes, fp) != (size_t)sector_bytes) {
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

void dorado_disk_pack_free(dorado_disk_pack *pack)
{
    free(pack->sectors);
    memset(pack, 0, sizeof *pack);
}

/* ─── Drive ─────────────────────────────────────────────────────── */

void dorado_disk_drive_init(dorado_disk_drive *drv)
{
    memset(drv, 0, sizeof *drv);
    drv->sectors_per_revolution = 0;
}

void dorado_disk_drive_attach_pack(dorado_disk_drive *drv,
                                   dorado_disk_pack *pack)
{
    drv->pack    = pack;
    drv->pdi     = NULL;
    drv->online  = (pack != NULL) ? 1 : 0;
    drv->read_only = pack ? pack->read_only : 0;
    drv->cur_cyl = 0;
    drv->cur_head = 0;
    drv->cur_sector = 0;
    drv->head_overflow = 0;
    if (pack && drv->sectors_per_revolution <= 0) {
        drv->sectors_per_revolution = pack->geometry.sectors;
    }
}

/* ─── Controller ────────────────────────────────────────────────── */

void dorado_disk_controller_init(dorado_disk_controller *ctl)
{
    memset(ctl, 0, sizeof *ctl);
    for (int i = 0; i < DORADO_DISK_NUM_DRIVES; i++) {
        dorado_disk_drive_init(&ctl->drive[i]);
    }
    ctl->selected_drive = 0;
    ctl->drive[0].selected = 1;
    /* PilotDisk/Initial treat drive 0 as the boot drive and load
     * subsector count 3 (four 117-pulse subsectors per sector). We
     * seed that convention here until the full drive-select timing path
     * through the controller sequence logic is modeled. */
    disk_set_subsector_count(&ctl->drive[0], 3);
}

void dorado_disk_controller_attach_drive(dorado_disk_controller *ctl,
                                         int slot, dorado_disk_pack *pack)
{
    if (slot < 0 || slot >= DORADO_DISK_NUM_DRIVES) return;
    dorado_disk_drive_attach_pack(&ctl->drive[slot], pack);
}

void dorado_disk_controller_attach_pdi(dorado_disk_controller *ctl,
                                       int slot,
                                       const dorado_pdi *pdi)
{
    if (!ctl || slot < 0 || slot >= DORADO_DISK_NUM_DRIVES) return;
    dorado_disk_drive *d = &ctl->drive[slot];
    d->pack = NULL;
    d->pdi = pdi;
    d->online = (pdi != NULL) ? 1 : 0;
    d->read_only = 1;
    d->cur_cyl = 0;
    d->cur_head = 0;
    d->cur_sector = 0;
    d->head_overflow = 0;
    if (pdi) d->sectors_per_revolution = 28;
}

/* Per-media block framing. A real Trident/Diablo pack carries its own
 * header/label/data word counts in its geometry (so a Diablo-on-Trident pack
 * frames as 2/8/256 while a native Cedar Trident frames as 2/10/1024); PDI
 * media is self-describing via label_words/data_words. */
static int disk_drive_header_words(const dorado_disk_drive *d)
{
    if (d->pack) return dorado_disk_geom_header_words(&d->pack->geometry);
    return DORADO_DISK_HEADER_WORDS;
}

static int disk_drive_label_words(const dorado_disk_drive *d)
{
    if (d->pack) return dorado_disk_geom_label_words(&d->pack->geometry);
    if (d->pdi)  return d->pdi->label_words;
    return DORADO_DISK_LABEL_WORDS;
}

static int disk_drive_data_words(const dorado_disk_drive *d)
{
    if (d->pack) return dorado_disk_geom_data_words(&d->pack->geometry);
    if (d->pdi)  return d->pdi->data_words;
    return DORADO_DISK_DATA_WORDS;
}

static uint16_t disk_sector_word(const dorado_disk_drive *d,
                                 const dorado_disk_sector *s, int idx)
{
    int hw = disk_drive_header_words(d);
    int lw = disk_drive_label_words(d);
    int dw = disk_drive_data_words(d);
    if (idx < hw) {
        return s->header[idx];
    }
    idx -= hw;
    if (idx < lw) {
        return s->label[idx];
    }
    idx -= lw;
    if (idx < dw) {
        return s->data[idx];
    }
    return 0;
}

/* Per-block trailing words the real controller writes after each of the
 * header/label/data blocks: 2 garbage words + 2 ECC words (HM §9.10;
 * AltoDiabloDisk.mc DoAltoCmmd/ReadECC). The DSK-task read drains them between
 * blocks, so the FIFO stream must include them or the per-block Input loops
 * misalign. We emit zeros: the read loop's ReadECC terminates when the second
 * ECC word reads zero, and the real pass/fail is reported via the read-error
 * muffler, not these words. */
#define DORADO_DISK_BLOCK_TRAILER_WORDS 4

static int disk_read_stream_total(const dorado_disk_controller *ctl,
                                  const dorado_disk_drive *d)
{
    if (d->pdi)
        return DORADO_DISK_HEADER_WORDS + d->pdi->label_words + d->pdi->data_words;
    int base = disk_drive_header_words(d) + disk_drive_label_words(d) +
               disk_drive_data_words(d);
    if (ctl->read_block_framing)
        base += 3 * DORADO_DISK_BLOCK_TRAILER_WORDS;
    return base;
}

static int disk_drive_has_media(const dorado_disk_drive *d)
{
    return d && (d->pack || d->pdi);
}

static int disk_drive_heads(const dorado_disk_drive *d)
{
    if (d->pack) return d->pack->geometry.heads;
    if (d->pdi) return 5; /* Dorado Cedar PDI is a T-80-style Pilot volume. */
    return 0;
}

static int disk_drive_sectors(const dorado_disk_drive *d)
{
    if (d->pack) return d->pack->geometry.sectors;
    if (d->pdi) return 28; /* DiskHeadDorado modelSectors for SA4000. */
    return 0;
}

static uint32_t disk_pdi_page_number(const dorado_disk_drive *d)
{
    int sectors = disk_drive_sectors(d);
    if (sectors <= 0) sectors = 28;

    /* DiskHeadDorado exposes drive 0 as a "system80": one logical head
     * whose cylinder number is physicalCylinder*5+head. BootFile links in
     * the PDI root are raw virtual disk addresses, so this maps the tag state
     * back to that linear Pilot page number. */
    return (uint32_t)d->cur_cyl * (uint32_t)sectors +
           (uint32_t)(d->cur_sector % sectors);
}

static uint16_t disk_pdi_word(const dorado_disk_drive *d, int idx)
{
    if (!d || !d->pdi) return 0;
    uint32_t page = disk_pdi_page_number(d);
    const uint16_t *label = dorado_pdi_page_label(d->pdi, page);
    const uint16_t *data = dorado_pdi_page_data(d->pdi, page);
    if (!label || !data) return 0;

    if (idx < DORADO_DISK_HEADER_WORDS) {
        return (idx == 0) ? (uint16_t)page : 0;
    }
    idx -= DORADO_DISK_HEADER_WORDS;
    if (idx < d->pdi->label_words) return label[idx];
    idx -= d->pdi->label_words;
    if (idx < d->pdi->data_words) return data[idx];
    return 0;
}

static int disk_sector_pulse_count(const dorado_disk_drive *d)
{
    if (d->sectors_per_revolution > 0) return d->sectors_per_revolution;
    if (d->pack && d->pack->geometry.sectors > 0) return d->pack->geometry.sectors;
    if (d->pdi) return 28;
    return 1;
}

static int disk_media_sector(const dorado_disk_drive *d)
{
    if (!d->pack || d->pack->geometry.sectors <= 0) return d->cur_sector;
    return d->cur_sector % d->pack->geometry.sectors;
}

static void disk_set_subsector_count(dorado_disk_drive *d, int count)
{
    if (!d) return;
    d->subsector_count = count & 0x3F;
    int divisor = d->subsector_count + 1;
    if (divisor <= 0) divisor = 1;
    d->sectors_per_revolution =
        DORADO_DISK_SUBSECTOR_PULSES_PER_REV / divisor;
    if (d->sectors_per_revolution <= 0) d->sectors_per_revolution = 1;
    if (d->cur_sector >= d->sectors_per_revolution) d->cur_sector = 0;
}

/* Structured controller-sequence trace (DORADO_DISK_SEQ=1). One line per
 * key controller event — the workhorse for bringing up the read/write
 * sequence-PROM path (plan phases D2/D5). Cheap: gated by the cached flag. */
static void disk_seq_trace(const dorado_disk_controller *ctl, const char *ev)
{
    if (!disk_trace_enabled("DORADO_DISK_SEQ")) return;
    const dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    fprintf(stderr,
            "[diskseq] cyc=%llu %-10s drv=%d chs=%d/%d/%d spr=%d ssc=%d ctrl=0o%o "
            "run=%d act=%d fifo=%d/%d rdtw=%d wrtw=%d sectw=%d idxtw=%d "
            "tagtw=%d stream=%d@%d seek=%d\n",
            dorado_trace_cycle, ev, ctl->selected_drive, d->cur_cyl,
            d->cur_head, d->cur_sector,
            d->sectors_per_revolution, d->subsector_count,
            ctl->control, ctl->enable_run, ctl->active,
            ctl->fifo_count, DORADO_DISK_FIFO_WORDS,
            ctl->rd_fifo_tw, ctl->wr_fifo_tw, ctl->sector_tw,
            ctl->index_tw, ctl->tag_tw,
            ctl->read_stream_active, ctl->read_stream_index,
            d->seek_in_progress);
}

static unsigned disk_control_block_op(uint16_t control, int block)
{
    static const int shifts[4] = {
        DORADO_DISK_CTRL_OP1_SHIFT,
        DORADO_DISK_CTRL_OP2_SHIFT,
        DORADO_DISK_CTRL_OP3_SHIFT,
        DORADO_DISK_CTRL_OP4_SHIFT,
    };
    if (block < 0 || block >= 4) return DORADO_DISK_OP_DONE;
    return (control >> shifts[block]) & DORADO_DISK_CTRL_OP_MASK;
}

static int disk_block_default_words(const dorado_disk_drive *d, int block)
{
    switch (block) {
    case 0: return disk_drive_header_words(d);
    case 1: return disk_drive_label_words(d);
    case 2: return disk_drive_data_words(d);
    default: return 0;
    }
}

static uint16_t disk_block_word(const dorado_disk_drive *d,
                                const dorado_disk_sector *s,
                                int block, int pos)
{
    if (!s || pos < 0) return 0;
    switch (block) {
    case 0:
        return (pos < disk_drive_header_words(d)) ? s->header[pos] : 0;
    case 1:
        return (pos < disk_drive_label_words(d)) ? s->label[pos] : 0;
    case 2:
        return (pos < disk_drive_data_words(d)) ? s->data[pos] : 0;
    default:
        return 0;
    }
}

static uint16_t disk_format_block_words(const dorado_disk_controller *ctl,
                                        const dorado_disk_drive *d,
                                        int block)
{
    if (block < 0 || block >= 4) return 0;
    uint16_t n = (uint16_t)(ctl->format_ram[block] + 1u);
    if (ctl->format_ram_writes == 0 && n == 1)
        n = (uint16_t)disk_block_default_words(d, block);
    return n;
}

static int disk_drive_is_diablo_pack(const dorado_disk_drive *d)
{
    if (!d || !d->pack) return 0;
    const dorado_disk_geometry *g = &d->pack->geometry;
    return g->sectors == 29 &&
           dorado_disk_geom_header_words(g) == 2 &&
           dorado_disk_geom_label_words(g) == 8 &&
           dorado_disk_geom_data_words(g) == 256;
}

static int disk_block_start_index(const dorado_disk_drive *d, int block)
{
    int idx = 0;
    for (int b = 0; b < block; b++)
        idx += disk_block_default_words(d, b);
    return idx;
}

static void disk_arm_write_block(dorado_disk_controller *ctl,
                                 const dorado_disk_drive *d,
                                 int block)
{
    ctl->read_stream_active = 0;
    ctl->rd_fifo_tw = 0;
    ctl->write_stream_active = 1;
    ctl->write_stream_index = disk_block_start_index(d, block);
    ctl->current_block = (uint8_t)block;
    ctl->current_block_op = DORADO_DISK_OP_WRITE;
    ctl->current_block_words = disk_format_block_words(ctl, d, block);
    ctl->current_block_pos = 0;
    ctl->current_block_trailer = 0;
    ctl->active = 1;
    disk_update_fifo_tws(ctl);
    disk_seq_trace(ctl, "write-block");
}

static int disk_advance_to_next_read_block(dorado_disk_controller *ctl)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    while (ctl->current_block < 4) {
        unsigned op = disk_control_block_op(ctl->control, ctl->current_block);
        if (op == DORADO_DISK_OP_READ || op == DORADO_DISK_OP_RDCHK) {
            ctl->current_block_op = (uint8_t)op;
            ctl->current_block_words =
                disk_format_block_words(ctl, d, ctl->current_block);
            ctl->current_block_pos = 0;
            ctl->current_block_trailer = 0;
            if (op == DORADO_DISK_OP_RDCHK) ctl->compare_err = 1;
            return 1;
        }
        ctl->current_block++;
    }
    ctl->current_block_op = DORADO_DISK_OP_DONE;
    ctl->current_block_words = 0;
    ctl->current_block_pos = 0;
    ctl->current_block_trailer = 0;
    return 0;
}

static void disk_finish_current_read_block(dorado_disk_controller *ctl)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    /* For a read-check block, hardware raises CompareErr at block start and
     * the DSK microcode clears it only after comparing the block words
     * (AltoDiabloDisk.mc ACheckLast tail). The FIFO stream can reach the
     * block boundary in the emulator before that tail runs, so do not latch a
     * permanent read error here. Report an uncleared CompareErr when
     * muffReadError/DeviceCheck is sampled instead. */
    ctl->current_block++;
    if (ctl->current_block < 4 &&
        disk_control_block_op(ctl->control, ctl->current_block) ==
            DORADO_DISK_OP_WRITE) {
        disk_arm_write_block(ctl, d, ctl->current_block);
        return;
    }
    if (!disk_advance_to_next_read_block(ctl)) {
        ctl->current_block_op = DORADO_DISK_OP_DONE;
        ctl->current_block_words = 0;
        ctl->current_block_pos = 0;
        ctl->current_block_trailer = 0;
    }
}

static void disk_update_fifo_tws(dorado_disk_controller *ctl)
{
    if (ctl->read_stream_active) {
        int threshold = 3;
        if (ctl->current_block_op == DORADO_DISK_OP_RDCHK ||
            ctl->current_block_op == DORADO_DISK_OP_DONE)
            threshold = 1;
        ctl->rd_fifo_tw = (ctl->fifo_count >= threshold) ? 1 : 0;
    } else {
        ctl->rd_fifo_tw = 0;
    }
    if (ctl->write_stream_active) {
        ctl->wr_fifo_tw =
            ((DORADO_DISK_FIFO_WORDS - ctl->fifo_count) >= 4) ? 1 : 0;
    } else {
        ctl->wr_fifo_tw = 0;
    }
}

static void disk_check_trace(const dorado_disk_controller *ctl,
                             const char *ev, uint16_t word)
{
    if (!disk_trace_enabled("DORADO_DISK_CHECK_TRACE")) return;
    const dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    fprintf(stderr,
            "[diskcheck] cyc=%llu %-10s drv=%d chs=%d/%d/%d media_sec=%d "
            "ctrl=0o%o blk=%u op=%u pos=%u/%u trail=%u fifo=%d/%d "
            "rdtw=%u act=%u cmp=%u rderr=%u idx=%d word=%06o\n",
            dorado_trace_cycle, ev, ctl->selected_drive, d->cur_cyl,
            d->cur_head, d->cur_sector,
            disk_media_sector(d), ctl->control, ctl->current_block,
            ctl->current_block_op, ctl->current_block_pos,
            ctl->current_block_words, ctl->current_block_trailer,
            ctl->fifo_count, DORADO_DISK_FIFO_WORDS, ctl->rd_fifo_tw,
            ctl->active, ctl->compare_err, ctl->read_data_err,
            ctl->read_stream_index, word);
}

void dorado_disk_controller_refill_fifo(dorado_disk_controller *ctl)
{
    if (!ctl || !ctl->read_stream_active) return;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!disk_drive_has_media(d)) {
        ctl->read_stream_active = 0;
        return;
    }
    /* Serve the sector latched at stream start (begin_read_stream), not the
     * drive's current sector -- a single read transfers one sector's data
     * field, but the emulated drive's cur_sector advances during the long
     * (Block-per-word) microcode drain. */
    const dorado_disk_sector *s = ctl->read_stream_sector;
    if (d->pack && !s) {
        ctl->read_stream_active = 0;
        return;
    }

    const int total = disk_read_stream_total(ctl, d);
    const int sequenced = ctl->read_block_framing && d->pack;
    while (ctl->fifo_count < DORADO_DISK_FIFO_WORDS) {
        uint16_t w = 0;
        if (sequenced) {
            if (ctl->current_block_op == DORADO_DISK_OP_DONE)
                break;
            if (ctl->current_block_pos < ctl->current_block_words) {
                uint16_t block_pos = ctl->current_block_pos;
                w = disk_block_word(d, s, ctl->current_block,
                                    block_pos);
                if (ctl->restore_header_check &&
                    ctl->current_block == 0 &&
                    ctl->current_block_op == DORADO_DISK_OP_RDCHK &&
                    block_pos == 0) {
                    /* AltoDiabloDisk.mc treats KCB disk-address bit 15 as a
                     * restore request. The on-disk Diablo header does not
                     * carry that command-only bit, but restore retries reuse
                     * the address word in the header check buffer. Real AEmu
                     * tolerates that in the header compare path; mirror it
                     * only for the first word of a Diablo header read-check. */
                    w |= 1u;
                }
                ctl->current_block_pos++;
            } else if (ctl->current_block_trailer <
                       DORADO_DISK_BLOCK_TRAILER_WORDS) {
                w = 0;
                ctl->current_block_trailer++;
            } else {
                if (ctl->fifo_count > 0) break;
                disk_finish_current_read_block(ctl);
                if (!ctl->read_stream_active) break;
                continue;
            }
        } else {
            if (ctl->read_stream_index >= total) break;
            w = d->pdi ? disk_pdi_word(d, ctl->read_stream_index)
                       : disk_sector_word(d, s, ctl->read_stream_index);
        }
        ctl->fifo[ctl->fifo_head] = w;
        ctl->fifo_head = (ctl->fifo_head + 1) % DORADO_DISK_FIFO_WORDS;
        ctl->fifo_count++;
        ctl->read_stream_index++;
    }
    disk_update_fifo_tws(ctl);
    if ((ctl->read_stream_index >= total ||
         (sequenced && ctl->current_block_op == DORADO_DISK_OP_DONE)) &&
        ctl->fifo_count == 0) {
        ctl->read_stream_active = 0;
        ctl->active = 0;
        ctl->rd_fifo_tw = 0;
        ctl->tag_tw = 1;
        ctl->tag_tw_sets++;
    }
}

static int disk_begin_read_stream(dorado_disk_controller *ctl)
{
    if (!ctl) return 0;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!disk_drive_has_media(d) ||
        (d->pack &&
         !dorado_disk_pack_sector(d->pack, d->cur_cyl, d->cur_head,
                                  disk_media_sector(d))) ||
        (d->pdi && !dorado_pdi_page_data(d->pdi, disk_pdi_page_number(d)))) {
        ctl->read_stream_start_failures++;
        return 0;
    }
    ctl->fifo_count = 0;
    ctl->fifo_head = 0;
    ctl->fifo_tail = 0;
    ctl->read_stream_index = 0;
    ctl->read_stream_active = 1;
    ctl->current_block = 0;
    ctl->current_block_op = DORADO_DISK_OP_DONE;
    ctl->current_block_words = 0;
    ctl->current_block_pos = 0;
    ctl->current_block_trailer = 0;
    ctl->read_stream_starts++;
    ctl->restore_header_check =
        (ctl->transfer_restore && disk_drive_is_diablo_pack(d)) ? 1 : 0;
    ctl->transfer_restore = 0;
    /* Latch the sector being read: the drain spans many emulated sector pulses,
     * so refills must keep serving this sector, not the advancing cur_sector. */
    ctl->read_stream_sector = d->pack
        ? dorado_disk_pack_sector(d->pack, d->cur_cyl, d->cur_head,
                                  disk_media_sector(d))
        : NULL;
    if (ctl->read_block_framing && d->pack)
        disk_advance_to_next_read_block(ctl);
    dorado_disk_controller_refill_fifo(ctl);
    disk_seq_trace(ctl, "read-start");
    if (dorado_trace_flag("DORADO_DISK_HDR_TRACE")) {
        const dorado_disk_sector *s = ctl->read_stream_sector;
        fprintf(stderr, "[diskhdr] read CHS=(%d,%d,%d) media_sec=%d "
                "header=%06o,%06o label0=%06o data0=%06o framing=%d\n",
                d->cur_cyl, d->cur_head, d->cur_sector, disk_media_sector(d),
                s ? s->header[0] : 0, s ? s->header[1] : 0,
                s ? s->label[0] : 0, s ? s->data[0] : 0,
                ctl->read_block_framing);
    }
    return ctl->read_stream_active || ctl->fifo_count > 0;
}

static void disk_abort_active_transfer(dorado_disk_controller *ctl)
{
    ctl->active = 0;
    ctl->read_stream_active = 0;
    ctl->read_stream_index = 0;
    ctl->current_block = 0;
    ctl->current_block_op = DORADO_DISK_OP_DONE;
    ctl->current_block_words = 0;
    ctl->current_block_pos = 0;
    ctl->current_block_trailer = 0;
    ctl->write_stream_active = 0;
    ctl->write_stream_index = 0;
    ctl->fifo_head = ctl->fifo_tail = ctl->fifo_count = 0;
    disk_update_fifo_tws(ctl);
}

int dorado_disk_controller_wakeup_pending(const dorado_disk_controller *ctl)
{
    if (!ctl) return 0;
    return ctl->index_tw || ctl->sector_tw || ctl->tag_tw ||
           ctl->rd_fifo_tw || ctl->wr_fifo_tw;
}

static int disk_control_has_transfer_op(uint16_t control)
{
    return (((control >> DORADO_DISK_CTRL_OP1_SHIFT) & DORADO_DISK_CTRL_OP_MASK) !=
            DORADO_DISK_OP_DONE) ||
           (((control >> DORADO_DISK_CTRL_OP2_SHIFT) & DORADO_DISK_CTRL_OP_MASK) !=
            DORADO_DISK_OP_DONE) ||
           (((control >> DORADO_DISK_CTRL_OP3_SHIFT) & DORADO_DISK_CTRL_OP_MASK) !=
            DORADO_DISK_OP_DONE) ||
           (((control >> DORADO_DISK_CTRL_OP4_SHIFT) & DORADO_DISK_CTRL_OP_MASK) !=
            DORADO_DISK_OP_DONE);
}

static int disk_control_has_op(uint16_t control, unsigned op)
{
    return (((control >> DORADO_DISK_CTRL_OP1_SHIFT) & DORADO_DISK_CTRL_OP_MASK) == op) ||
           (((control >> DORADO_DISK_CTRL_OP2_SHIFT) & DORADO_DISK_CTRL_OP_MASK) == op) ||
           (((control >> DORADO_DISK_CTRL_OP3_SHIFT) & DORADO_DISK_CTRL_OP_MASK) == op) ||
           (((control >> DORADO_DISK_CTRL_OP4_SHIFT) & DORADO_DISK_CTRL_OP_MASK) == op);
}

void dorado_disk_controller_advance_sector(dorado_disk_controller *ctl)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!disk_drive_has_media(d)) return;

    /* Real disk transfers finish within their sector window. In this emulator
     * the DSK task drains the FIFO over many host-scheduled microinstructions;
     * if rotation keeps advancing, SectorTW collapses many pulses into one and
     * AltoDiabloDisk's software Sector counter falls behind the selected media
     * sector. Hold rotation while a sector transfer is visible as Active. */
    if (ctl->active || ctl->read_stream_active || ctl->write_stream_active)
        return;

    d->cur_sector = (d->cur_sector + 1) % disk_sector_pulse_count(d);
    disk_seq_trace(ctl, "sector+");
    int at_index = (d->cur_sector == 0);

    if (d->seek_in_progress > 0) {
        d->seek_in_progress--;
        if (at_index) {
            d->seek_in_progress = 0;
            ctl->tag_tw = 1;
            ctl->tag_tw_sets++;
        }
    }

    if (at_index) {
        d->index_pulse = 1;
        ctl->index_tw = 1;
        ctl->sector_tw = 1;
        ctl->index_tw_sets++;
        ctl->sector_tw_sets++;
        ctl->block_till_index = 0;
    } else {
        d->index_pulse = 0;
        if (ctl->block_till_index) return;
        ctl->sector_tw = 1;
        ctl->sector_tw_sets++;
    }

    /* If a transfer is active or queued by DiskControl, load the next
     * sector's data. Phase 2 simplification: real hardware sequences
     * this via the read PROM. */
    if (!ctl->active && ctl->enable_run && ctl->xfer_pending &&
        disk_control_has_transfer_op(ctl->control)) {
        /* A transfer command is pending: stream this sector into the FIFO,
         * ONCE. The DSK-task disk drivers drain each sector's blocks
         * (header/label/data, each with trailing 2 garbage + 2 ECC words) with
         * Block/Input loops; re-starting the stream on every sector pulse while
         * a read is still draining would reset read_stream_index mid-block,
         * misalign the per-block Input loops, exhaust the FIFO early, and abort
         * the read with AReadBadTW. So start one-shot per command and let the
         * stream drain fully (refill_fifo tops up the SAME sector); a new
         * sector read needs a fresh DiskControl command (xfer_pending). */
        if (disk_control_has_op(ctl->control, DORADO_DISK_OP_READ) ||
            disk_control_has_op(ctl->control, DORADO_DISK_OP_RDCHK)) {
            ctl->read_block_framing = 1;
            if (disk_begin_read_stream(ctl)) {
                ctl->active = 1;
                ctl->xfer_pending = 0;
                ctl->read_stream_sector_starts++;
            }
        } else if (disk_control_has_op(ctl->control, DORADO_DISK_OP_WRITE)) {
            ctl->write_stream_active = 1;
            ctl->write_stream_index = 0;
            ctl->current_block = 0;
            ctl->current_block_op = DORADO_DISK_OP_WRITE;
            ctl->current_block_words = disk_format_block_words(ctl, d, 0);
            ctl->current_block_pos = 0;
            ctl->current_block_trailer = 0;
            ctl->fifo_head = ctl->fifo_tail = ctl->fifo_count = 0;
            ctl->active = 1;
            ctl->xfer_pending = 0;
            disk_update_fifo_tws(ctl);
            disk_seq_trace(ctl, "write-arm");
        }
    }
}

int dorado_disk_controller_read_page(dorado_disk_controller *ctl,
                                     uint32_t page,
                                     uint16_t *label, int label_n,
                                     uint16_t *data, int data_n)
{
    if (!ctl) return -1;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!disk_drive_has_media(d)) return -1;

    int sectors = disk_drive_sectors(d);
    if (sectors <= 0) sectors = 28;
    /* Position the drive at the requested logical page, the way a real seek +
     * head/sector select would (the read stream serves the selected drive's
     * current CHS). */
    d->cur_cyl    = (int)(page / (uint32_t)sectors);
    d->cur_head   = 0;
    d->cur_sector = (int)(page % (uint32_t)sectors);

    /* The bridge drains header+label+data contiguously (no per-block ECC
     * trailing words), so stream contiguously regardless of any prior framed
     * DSK-task read. */
    ctl->read_block_framing = 0;
    if (!disk_begin_read_stream(ctl)) return -1;

    /* Drain header + label + data words out of the controller FIFO, exactly
     * as the disk task would with Pd<-Input on DiskData. This routes the boot
     * read through the real controller read path (FIFO + framing). */
    int hw = disk_drive_header_words(d);
    int total = hw + label_n + data_n;
    int idx = 0, got = 0;
    while (idx < total) {
        if (ctl->fifo_count == 0) {
            dorado_disk_controller_refill_fifo(ctl);
            if (ctl->fifo_count == 0) break;   /* stream exhausted */
        }
        uint16_t w = ctl->fifo[ctl->fifo_tail];
        ctl->fifo_tail = (ctl->fifo_tail + 1) % DORADO_DISK_FIFO_WORDS;
        ctl->fifo_count--;
        ctl->fifo_reads++;
        if (idx < hw) {
            /* header words discarded (address only) */
        } else if (idx < hw + label_n) {
            if (label) label[idx - hw] = w;
        } else {
            if (data) data[idx - hw - label_n] = w;
        }
        idx++; got++;
        dorado_disk_controller_refill_fifo(ctl);
    }
    /* Read consumed; quiesce the stream. */
    ctl->read_stream_active = 0;
    ctl->active = 0;
    ctl->rd_fifo_tw = 0;
    ctl->current_block = 0;
    ctl->current_block_op = DORADO_DISK_OP_DONE;
    ctl->current_block_words = 0;
    ctl->current_block_pos = 0;
    ctl->current_block_trailer = 0;
    return (got == total) ? 0 : -1;
}

int dorado_disk_controller_write_page(dorado_disk_controller *ctl,
                                      uint32_t page,
                                      const uint16_t *label, int label_n,
                                      const uint16_t *data, int data_n)
{
    if (!ctl) return -1;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    /* Only a real Trident pack is writable; PDI media is read-only. */
    if (!d->pack || d->read_only || d->pack->read_only) return -1;

    int sectors = d->pack->geometry.sectors;
    if (sectors <= 0) return -1;
    d->cur_cyl    = (int)(page / (uint32_t)sectors);
    d->cur_head   = 0;
    d->cur_sector = (int)(page % (uint32_t)sectors);

    dorado_disk_sector *s =
        dorado_disk_pack_sector(d->pack, d->cur_cyl, d->cur_head,
                                disk_media_sector(d));
    if (!s) return -1;

    if (label) {
        for (int i = 0; i < label_n && i < disk_drive_label_words(d); i++)
            s->label[i] = label[i];
    }
    if (data) {
        for (int i = 0; i < data_n && i < disk_drive_data_words(d); i++)
            s->data[i] = data[i];
    }
    /* Header records the on-disk address (cyl/head/sec) for the read-back
     * header compare. */
    s->header[0] = (uint16_t)d->cur_cyl;
    if (disk_drive_header_words(d) > 1)
        s->header[1] = (uint16_t)((d->cur_head << 8) | (d->cur_sector & 0xFF));
    s->modified = 1;
    ctl->fifo_writes++;
    disk_seq_trace(ctl, "write-page");
    return 0;
}

/* Commit one DiskData word to the current sector during a write op (F3): the
 * write sequence streams header+label+data words from the FIFO onto the pack.
 * Ends the stream when a full sector has been written. */
static void disk_write_stream_word(dorado_disk_controller *ctl, uint16_t w)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!d->pack || d->read_only || d->pack->read_only) {
        ctl->write_stream_active = 0;
        return;
    }
    dorado_disk_sector *s =
        dorado_disk_pack_sector(d->pack, d->cur_cyl, d->cur_head,
                                disk_media_sector(d));
    if (!s) { ctl->write_stream_active = 0; return; }

    int hw = disk_drive_header_words(d);
    int lw = disk_drive_label_words(d);
    int dw = disk_drive_data_words(d);
    int idx = ctl->write_stream_index;
    int total = hw + lw + dw;
    if (idx < hw) {
        s->header[idx] = w;
    } else if (idx < hw + lw) {
        s->label[idx - hw] = w;
    } else if (idx < total) {
        s->data[idx - hw - lw] = w;
    }
    s->modified = 1;
    ctl->write_stream_index++;
    ctl->current_block_pos++;
    if (ctl->write_stream_index >= total ||
        ctl->current_block_pos >= ctl->current_block_words) {
        int completed_block = ctl->current_block;
        ctl->write_stream_active = 0;
        ctl->current_block++;
        if (ctl->current_block < 4 &&
            disk_control_block_op(ctl->control, ctl->current_block) ==
                DORADO_DISK_OP_WRITE) {
            disk_arm_write_block(ctl, d, ctl->current_block);
        } else {
            ctl->active = 0;
            ctl->current_block_op = DORADO_DISK_OP_DONE;
            ctl->tag_tw = 1;          /* block-end tag wakeup */
            ctl->tag_tw_sets++;
            disk_update_fifo_tws(ctl);
            disk_seq_trace(ctl, completed_block == 0 ? "write-done" :
                           "write-blk-done");
        }
    }
}

int dorado_disk_controller_tick(dorado_disk_controller *ctl,
                                uint64_t now_cycles)
{
    if (!ctl) return 0;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    /* A real Trident pack is always clock-driven. PDI media is normally
     * completed at the IOCB level by the machine shim (ticking it would inject
     * spurious DSK wakeups), but under --disk-real (allow_pdi_timing) the DSK
     * task drives PilotDisk.mc against the controller and the PDI must spin
     * too. A spun-down/empty drive is idle. */
    if (!d->online) return 0;
    if (!d->pack && !(d->pdi && ctl->allow_pdi_timing)) return 0;

    int spr = disk_sector_pulse_count(d);
    if (spr <= 0) return 0;
    uint64_t cps = DORADO_DISK_CYCLES_PER_REV / (uint64_t)spr;
    if (cps == 0) cps = 1;

    /* Arm on first tick relative to the current cycle. */
    if (ctl->next_sector_cycle == 0) {
        ctl->next_sector_cycle = now_cycles + cps;
        return 0;
    }

    int advanced = 0;
    /* Bounded catch-up: if the caller jumped far ahead (headless
     * fast-forward), emit a few pulses then resync rather than storm. */
    int guard = 0;
    while (now_cycles >= ctl->next_sector_cycle && guard++ < 64) {
        dorado_disk_controller_advance_sector(ctl);
        ctl->timing_advances++;
        ctl->next_sector_cycle += cps;
        advanced = 1;
    }
    if (now_cycles >= ctl->next_sector_cycle) {
        ctl->next_sector_cycle = now_cycles + cps;   /* fell behind: resync */
    }
    return advanced;
}

/* ─── Slow-IO command dispatch ───────────────────────────────────── */

static void disk_output_b(void *ctx, int task, int subtask,
                          uint8_t tioa, uint16_t data)
{
    dorado_disk_controller *ctl = ctx;
    ctl->output_count++;
    ctl->output_tioa_count[tioa & 0x0F]++;
    ctl->last_output_tioa = tioa;
    ctl->last_output_data = data;
    (void)task;
    (void)subtask;
    if (dorado_trace_flag("DORADO_DISK_TRACE"))
        fprintf(stderr, "[disk] OUT tioa=0o%o data=0o%o (tk=%d)\n",
                tioa, data, task);

    switch (tioa) {
    case DORADO_DISK_TIOA_DISKCONTROL:
        /* Output to control register zeros the format-RAM address
         * register (HM page 98) so subsequent DiskRam writes start
         * at index 0. ClearEnableRun is honored. SetDebugMode is
         * honored. SetBlockTillIndex is honored.
         *
         * HM page 97: while Active, the first DiskControl output
         * aborts the current sector transfer; the following output
         * loads the register. Do not treat the aborting output as a
         * new command load. */
        if (ctl->active) {
            disk_abort_active_transfer(ctl);
            ctl->control_abort_edges++;
            break;
        }
        ctl->control = data;
        ctl->control_loads++;
        if (disk_control_has_transfer_op(data)) {
            ctl->control_transfer_loads++;
            ctl->xfer_pending = 1;   /* arm a one-shot read-stream start */
            ctl->transfer_restore = ctl->restore_pending;
            ctl->restore_pending = 0;
        }
        ctl->format_ram_addr = 0;
        if (data & DORADO_DISK_CTRL_CLR_ENABLE_RUN) ctl->enable_run = 0;
        if (data & DORADO_DISK_CTRL_SET_DEBUG_MODE) ctl->debug_mode = 1;
        if (data & DORADO_DISK_CTRL_BLOCK_TILL_INDEX) ctl->block_till_index = 1;
        disk_seq_trace(ctl, "ctrl-load");
        /* If ops 1..4 are non-zero AND EnableRun is set, schedule a
         * sector transfer at the next sector pulse. We don't run the
         * timing model in Phase 1 — record the request. */
        break;

    case DORADO_DISK_TIOA_DISKMUFF:
        /* DiskMuff output. DiskDefs.mc maps muffAddr to the low
         * byte and the wakeup/error clear controls to native octal
         * constants 04000, 02000, 01000, and 00400. */
        ctl->muff_addr = (uint8_t)(data & DORADO_DISK_MUFF_ADDR);
        if (data & DORADO_DISK_MUFF_CLEAR_INDEX_TW) {
            if (ctl->index_tw) ctl->index_tw_clears++;
            ctl->index_tw = 0;
        }
        if (data & DORADO_DISK_MUFF_CLEAR_SECTOR_TW) {
            if (ctl->sector_tw) ctl->sector_tw_clears++;
            ctl->sector_tw = 0;
        }
        if (data & DORADO_DISK_MUFF_CLEAR_SEEKTAG_TW) {
            if (ctl->tag_tw) ctl->tag_tw_clears++;
            ctl->tag_tw = 0;
        }
        if (data & DORADO_DISK_MUFF_CLEAR_COMPARE_ERR) {
            ctl->compare_err = 0;
            disk_check_trace(ctl, "clr-cmp", 0);
        }
        if (data & DORADO_DISK_MUFF_SET_CHECKSUM_ERR) {
            ctl->read_data_err = 1;
            disk_check_trace(ctl, "set-rderr", 0);
        }
        if (data & DORADO_DISK_MUFF_CLEAR_ERRORS) {
            ctl->compare_err = 0;
            ctl->read_data_err = 0;
            ctl->fifo_underflow = 0;
            ctl->fifo_overflow = 0;
            disk_check_trace(ctl, "clr-errs", 0);
        }
        break;

    case DORADO_DISK_TIOA_DISKDATA:
        /* Write FIFO data. During an active write op (F3), commit the word to
         * the current sector via the write stream; otherwise buffer it in the
         * software FIFO. */
        if (ctl->write_stream_active) {
            disk_write_stream_word(ctl, data);
            ctl->fifo_writes++;
            disk_update_fifo_tws(ctl);
        } else if (ctl->fifo_count < DORADO_DISK_FIFO_WORDS) {
            ctl->fifo[ctl->fifo_head] = data;
            ctl->fifo_head = (ctl->fifo_head + 1) % DORADO_DISK_FIFO_WORDS;
            ctl->fifo_count++;
            ctl->fifo_writes++;
            disk_update_fifo_tws(ctl);
        } else {
            ctl->fifo_overflow = 1;
        }
        break;

    case DORADO_DISK_TIOA_DISKRAM:
        /* Write into the Format RAM at the current address, then
         * post-increment. Loading the *last* word of Format RAM (15)
         * sets EnableRun (HM page 98). */
        if (ctl->format_ram_addr < DORADO_DISK_FORMAT_RAM_WORDS) {
            ctl->format_ram[ctl->format_ram_addr] = data;
            ctl->format_ram_writes++;
            if (ctl->format_ram_addr == DORADO_DISK_FORMAT_RAM_WORDS - 1) {
                ctl->enable_run = 1;
            }
            ctl->format_ram_addr++;
        }
        break;

    case DORADO_DISK_TIOA_DISKTAG:
        ctl->tag = data;
        ctl->tag_writes++;
        disk_seq_trace(ctl, "tag");
        /* Native Dorado DiskTag commands are strobed on one of the
         * upper four bits. SendDriveTag deliberately writes bus,
         * bus|tagDrive, then bus again; only the strobed word should
         * execute a drive command. */
        {
            uint16_t bus = data & DORADO_DISK_TAG_BUS;
            int tag_type = -1;
            if (data & DORADO_DISK_TAG_DRIVE) {
                tag_type = 0;
            } else if (data & DORADO_DISK_TAG_HEAD) {
                tag_type = 1;
            } else if (data & DORADO_DISK_TAG_CYLINDER) {
                tag_type = 2;
            } else if (data & DORADO_DISK_TAG_CONTROL) {
                tag_type = 3;
            }
            switch (tag_type) {
            case 0: {
                /* Drive Select / subsector count (HM page 100).
                 * Only tagBus[0:11] is presented to the drive. Pilot
                 * also carries KSelect bookkeeping in bit 11
                 * (0x0800), so mask the fields instead of treating the
                 * whole low 12 bits as the subsector count. */
                int drv_select = bus & 0x0F;
                if (bus & (1u << 5)) {
                    int count = (bus >> 6) & 0x0F;
                    disk_set_subsector_count(&ctl->drive[ctl->selected_drive],
                                             count);
                }
                if ((bus & (1u << 4)) &&
                    drv_select <= 3 && ctl->drive[drv_select].online) {
                    ctl->selected_drive = drv_select;
                    for (int i = 0; i < DORADO_DISK_NUM_DRIVES; i++) {
                        ctl->drive[i].selected = (i == drv_select) ? 1 : 0;
                    }
                }
                break;
            }
            case 1: {
                /* Head Tag (HM page 100): Tag[10:15] = head number
                 * (LSB 0..5). Tag[8] = OffCylinder (LSB 7).
                 * Tag[9] = direction (LSB 6). */
                int head = bus & 0x3F;
                dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                int heads = disk_drive_heads(d);
                if (heads > 0 && head < heads) {
                    d->cur_head = head;
                    d->head_overflow = 0;
                } else if (disk_drive_has_media(d)) {
                    d->cur_head = head;  /* still record */
                    d->head_overflow = 1;
                }
                ctl->tag_tw = 1;          /* tag-completion wakeup */
                ctl->tag_tw_sets++;
                break;
            }
            case 2: {
                /* Cylinder Tag (HM page 100): Tag[4:15] = 12-bit
                 * cylinder number (LSB 0..11). */
                int cyl = bus;
                dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                if (disk_drive_has_media(d)) {
                    d->cur_cyl = cyl;
                    d->cur_sector = 0;       /* lose sector sync on seek */
                    d->seek_in_progress = disk_sector_pulse_count(d);
                }
                break;
            }
            case 3: {
                /* Control Tag (HM page 101). Tag bits (LSB):
                 *   bit 0 = HeadAdvance
                 *   bit 1 = ReZero
                 *   bit 2 = HeadSelect
                 *   bit 3 = DeviceCheckReset
                 *   bit 4 = ResetHead
                 *   bit 6 = Read
                 *   bit 7 = Write
                 *   bit 8 = StrobeEarly
                 *   bit 9 = StrobeLate
                 *   bit 11 = AltoLeader
                 * (Bits 5, 10 unused.) */
                if (bus & (1u << 3)) {
                    /* tagDiskReset (DiskDefs tagDiskReset=0o10): reset errors
                     * latched in the drive. The disk-task init selects an
                     * illegal head (5) to probe T-80 vs AMS-315, sees the
                     * HeadOverflow error, then issues this to clear it; without
                     * the clear the latched overflow makes every later read
                     * report a hardware error and the boot stalls. */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->head_overflow = 0;
                }
                if (bus & (1u << 1)) {
                    /* ReZero (tagReZero=0o2): restore to cylinder 0; also clears
                     * the drive's latched error state. */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_cyl = 0;
                    d->cur_head = 0;
                    d->cur_sector = 0;
                    d->head_overflow = 0;
                    d->seek_in_progress = disk_sector_pulse_count(d);
                    ctl->restore_pending = 1;
                }
                if (bus & (1u << 0)) {
                    /* HeadAdvance */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_head++;
                }
                if (bus & (1u << 6)) {
                    /* Read — populate FIFO from current sector. */
                    ctl->read_block_framing = 0;
                    if (disk_begin_read_stream(ctl)) {
                        ctl->active = 1;
                        ctl->read_stream_tag_starts++;
                    }
                }
                if (bus & (1u << 7)) {
                    /* Write — begin committing FIFO words to the current
                     * sector (F3). Microcode pumps header+label+data via
                     * DiskData output; disk_write_stream_word() commits them to
                     * the pack and ends the op at the sector boundary. Only a
                     * writable real pack accepts the stream. */
                    dorado_disk_drive *wd =
                        &ctl->drive[ctl->selected_drive];
                    if (wd->pack && !wd->read_only && !wd->pack->read_only) {
                        ctl->write_stream_active = 1;
                        ctl->write_stream_index = 0;
                        ctl->current_block = 0;
                        ctl->current_block_op = DORADO_DISK_OP_WRITE;
                        ctl->current_block_words =
                            disk_drive_header_words(wd) +
                            disk_drive_label_words(wd) +
                            disk_drive_data_words(wd);
                        ctl->current_block_pos = 0;
                        ctl->current_block_trailer = 0;
                        ctl->fifo_count = ctl->fifo_head = ctl->fifo_tail = 0;
                    }
                    ctl->active = 1;
                    disk_update_fifo_tws(ctl);
                    disk_seq_trace(ctl, "write-start");
                }
                if (!(bus & (1u << 1))) {
                    ctl->tag_tw = 1;
                    ctl->tag_tw_sets++;
                }
                break;
            }
            default:
                break;
            }
        }
        break;
    }
}

static int disk_muffler_bit(dorado_disk_controller *ctl, uint8_t addr)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    switch (addr) {
    case 001: return ctl->index_tw;
    case 002: return ctl->sector_tw;
    case 003: return ctl->tag_tw;
    case 004: return ctl->rd_fifo_tw;
    case 005: return ctl->wr_fifo_tw;
    case 010: return ctl->enable_run;
    case 011: return ctl->debug_mode;
    case 012:
        return !(ctl->active &&
                 ctl->current_block_op == DORADO_DISK_OP_READ);
    case 013:
        return !(ctl->active &&
                 ctl->current_block_op == DORADO_DISK_OP_WRITE);
    case 014:
        return !(ctl->active &&
                 ctl->current_block_op == DORADO_DISK_OP_RDCHK);
    case 015: return ctl->active;
    case 016: return ctl->selected_drive & 1;
    case 017: return (ctl->selected_drive >> 1) & 1;
    case 021:
        /* DiskHeadDorado.Initialize classifies a T-80 by selecting head 5
         * and then reading HeadOvfl through DMux. On the shim path the PDI
         * backend doesn't model the head-select transient, so report the
         * classification overflow unconditionally. On the real path
         * (--disk-real) PilotDisk does real head-selects, so report the actual
         * head_overflow flag (set by the head tag when head >= heads) -- a
         * persistent spurious overflow would make every read look like a head
         * error and stall the setup loop (plan D4). */
        return d->head_overflow ||
               (d->pdi != NULL && !ctl->allow_pdi_timing);
    case 023: return !d->selected;
    case 024: return !d->online;
    case 025: return d->seek_in_progress || !d->online;
    case 027: return ctl->fifo_underflow;
    case 030: return ctl->fifo_overflow;
    case 031: return ctl->read_data_err || ctl->compare_err;
    case 032: return d->read_only;
    case 036:
        return d->head_overflow || d->read_only || ctl->fifo_underflow ||
               ctl->fifo_overflow || ctl->read_data_err || ctl->compare_err;
    case 037:
        return d->head_overflow || ctl->fifo_underflow ||
               ctl->fifo_overflow || ctl->read_data_err || ctl->compare_err;
    default: return 0;
    }
}

static uint16_t disk_input(void *ctx, int task, int subtask,
                           uint8_t tioa, int *bad)
{
    dorado_disk_controller *ctl = ctx;
    if (bad) *bad = 0;
    ctl->input_count++;
    ctl->input_tioa_count[tioa & 0x0F]++;
    ctl->last_input_tioa = tioa;
    (void)task;
    (void)subtask;

    switch (tioa) {
    case DORADO_DISK_TIOA_DISKDATA:
        /* Read from FIFO (during a read). Phase 1: pop or return
         * 0xFFFF if empty (signals "no data"). */
        if (ctl->fifo_count > 0) {
            uint16_t v = ctl->fifo[ctl->fifo_tail];
            ctl->fifo_tail = (ctl->fifo_tail + 1) % DORADO_DISK_FIFO_WORDS;
            ctl->fifo_count--;
            ctl->fifo_reads++;
            dorado_disk_controller_refill_fifo(ctl);
            disk_update_fifo_tws(ctl);
            ctl->last_input_data = v;
            disk_check_trace(ctl, "data-in", v);
            return v;
        }
        ctl->fifo_underflow = 1;
        disk_update_fifo_tws(ctl);
        ctl->last_input_data = 0xFFFF;
        disk_check_trace(ctl, "underflow", 0xFFFF);
        return 0xFFFF;

    case DORADO_DISK_TIOA_DISKMUFF: {
        int bit = disk_muffler_bit(ctl, ctl->muff_addr);
        if (disk_trace_enabled("DORADO_DISK_MUFF_TRACE"))
            fprintf(stderr, "[diskmuff] cyc=%llu addr=0o%o -> %d\n",
                    dorado_trace_cycle, ctl->muff_addr, bit);
        /* HM pages 101-102: the selected muffler signal is driven on
         * IOB[15]. In this emulator's C word layout, Dorado bit 15 is
         * the low bit; DiskSubrs.mc tests Read1Muff with R odd. */
        uint16_t v = bit ? 0x0001 : 0x0000;
        ctl->last_input_data = v;
        return v;
    }
    }

    /* Unknown TIOA — return floating bus. */
    if (bad) *bad = 1;
    ctl->last_input_data = 0xFFFF;
    return 0xFFFF;
}

uint16_t dorado_disk_controller_dmux_read(dorado_disk_controller *ctl,
                                          uint16_t addr, int *handled)
{
    if (handled) *handled = 0;
    if (!ctl || addr < 02000 || addr > 02037) return 0;
    if (handled) *handled = 1;
    return disk_muffler_bit(ctl, (uint8_t)(addr - 02000)) ? 0x8000u : 0x0000u;
}

void dorado_disk_controller_attach_to_io(dorado_disk_controller *ctl,
                                         dorado_io *io)
{
    static dorado_io_device dev;
    dev.read  = disk_input;
    dev.write = disk_output_b;
    dev.ctx   = ctl;
    dev.name  = "disk(Trident)";

    /* Disk uses task 14₈ exclusively, on TIOA 10₈-14₈. Register
     * each one explicitly. */
    static const uint8_t tioas[] = {
        DORADO_DISK_TIOA_DISKCONTROL,
        DORADO_DISK_TIOA_DISKMUFF,
        DORADO_DISK_TIOA_DISKDATA,
        DORADO_DISK_TIOA_DISKRAM,
        DORADO_DISK_TIOA_DISKTAG,
    };
    for (size_t i = 0; i < sizeof tioas / sizeof tioas[0]; i++) {
        dorado_io_register(io, DORADO_DISK_TASK, tioas[i], &dev);
    }
}
