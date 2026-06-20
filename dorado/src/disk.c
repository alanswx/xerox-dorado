#include "disk.h"

#include "pdi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cached per-name trace flag (cpu.c). Keep disk trace checks off the hot
 * path's raw getenv() — see dorado/CLAUDE.md "Don't regress". */
extern int dorado_trace_flag(const char *name);

const dorado_disk_geometry DORADO_DISK_T80  = { 815, 5, 9 };
const dorado_disk_geometry DORADO_DISK_T300 = { 815, 19, 9 };

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

    /* Validate file size. */
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); dorado_disk_pack_free(pack); return -1; }
    long expected = (long)pack->num_sectors * DORADO_DISK_SECTOR_BYTES;
    long actual   = ftell(fp);
    if (actual != expected) {
        fclose(fp);
        dorado_disk_pack_free(pack);
        return -1;
    }
    rewind(fp);

    uint8_t buf[DORADO_DISK_SECTOR_BYTES];
    for (int i = 0; i < pack->num_sectors; i++) {
        if (fread(buf, 1, DORADO_DISK_SECTOR_BYTES, fp)
            != DORADO_DISK_SECTOR_BYTES) {
            fclose(fp);
            dorado_disk_pack_free(pack);
            return -1;
        }
        dorado_disk_sector *s = &pack->sectors[i];
        /* Skip dummy 2 bytes at offset 0. */
        int off = 2;
        for (int w = 0; w < DORADO_DISK_HEADER_WORDS; w++) {
            s->header[w] = (uint16_t)(buf[off] | (buf[off+1] << 8));
            off += 2;
        }
        for (int w = 0; w < DORADO_DISK_LABEL_WORDS; w++) {
            s->label[w] = (uint16_t)(buf[off] | (buf[off+1] << 8));
            off += 2;
        }
        for (int w = 0; w < DORADO_DISK_DATA_WORDS; w++) {
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
    uint8_t buf[DORADO_DISK_SECTOR_BYTES];
    for (int i = 0; i < pack->num_sectors; i++) {
        const dorado_disk_sector *s = &pack->sectors[i];
        memset(buf, 0, sizeof buf);
        int off = 2;                 /* dummy 2 bytes */
        for (int w = 0; w < DORADO_DISK_HEADER_WORDS; w++) {
            buf[off++] = (uint8_t)(s->header[w] & 0xFF);
            buf[off++] = (uint8_t)(s->header[w] >> 8);
        }
        for (int w = 0; w < DORADO_DISK_LABEL_WORDS; w++) {
            buf[off++] = (uint8_t)(s->label[w] & 0xFF);
            buf[off++] = (uint8_t)(s->label[w] >> 8);
        }
        for (int w = 0; w < DORADO_DISK_DATA_WORDS; w++) {
            buf[off++] = (uint8_t)(s->data[w] & 0xFF);
            buf[off++] = (uint8_t)(s->data[w] >> 8);
        }
        if (fwrite(buf, 1, DORADO_DISK_SECTOR_BYTES, fp)
            != DORADO_DISK_SECTOR_BYTES) {
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

static uint16_t disk_sector_word(const dorado_disk_sector *s, int idx)
{
    if (idx < DORADO_DISK_HEADER_WORDS) {
        return s->header[idx];
    }
    idx -= DORADO_DISK_HEADER_WORDS;
    if (idx < DORADO_DISK_LABEL_WORDS) {
        return s->label[idx];
    }
    idx -= DORADO_DISK_LABEL_WORDS;
    if (idx < DORADO_DISK_DATA_WORDS) {
        return s->data[idx];
    }
    return 0;
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
    if (!dorado_trace_flag("DORADO_DISK_SEQ")) return;
    const dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    fprintf(stderr,
            "[diskseq] %-10s drv=%d chs=%d/%d/%d ctrl=0o%o run=%d act=%d "
            "fifo=%d/%d rdtw=%d wrtw=%d sectw=%d idxtw=%d tagtw=%d "
            "stream=%d@%d seek=%d\n",
            ev, ctl->selected_drive, d->cur_cyl, d->cur_head, d->cur_sector,
            ctl->control, ctl->enable_run, ctl->active,
            ctl->fifo_count, DORADO_DISK_FIFO_WORDS,
            ctl->rd_fifo_tw, ctl->wr_fifo_tw, ctl->sector_tw,
            ctl->index_tw, ctl->tag_tw,
            ctl->read_stream_active, ctl->read_stream_index,
            d->seek_in_progress);
}

void dorado_disk_controller_refill_fifo(dorado_disk_controller *ctl)
{
    if (!ctl || !ctl->read_stream_active) return;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!disk_drive_has_media(d)) {
        ctl->read_stream_active = 0;
        return;
    }
    dorado_disk_sector *s = NULL;
    if (d->pack) {
        s = dorado_disk_pack_sector(
            d->pack, d->cur_cyl, d->cur_head, disk_media_sector(d));
    }
    if (d->pack && !s) {
        ctl->read_stream_active = 0;
        return;
    }

    const int total = d->pdi ?
        (DORADO_DISK_HEADER_WORDS + d->pdi->label_words + d->pdi->data_words) :
        (DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS +
         DORADO_DISK_DATA_WORDS);
    while (ctl->fifo_count < DORADO_DISK_FIFO_WORDS &&
           ctl->read_stream_index < total) {
        ctl->fifo[ctl->fifo_head] = d->pdi ?
            disk_pdi_word(d, ctl->read_stream_index) :
            disk_sector_word(s, ctl->read_stream_index);
        ctl->fifo_head = (ctl->fifo_head + 1) % DORADO_DISK_FIFO_WORDS;
        ctl->fifo_count++;
        ctl->read_stream_index++;
    }
    ctl->rd_fifo_tw = (ctl->fifo_count > 0) ? 1 : 0;
    if (ctl->read_stream_index >= total && ctl->fifo_count == 0) {
        ctl->read_stream_active = 0;
        ctl->active = 0;
        ctl->tag_tw = 1;
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
    ctl->read_stream_starts++;
    dorado_disk_controller_refill_fifo(ctl);
    disk_seq_trace(ctl, "read-start");
    return ctl->read_stream_active || ctl->fifo_count > 0;
}

static void disk_abort_active_transfer(dorado_disk_controller *ctl)
{
    ctl->active = 0;
    ctl->read_stream_active = 0;
    ctl->read_stream_index = 0;
    ctl->fifo_head = ctl->fifo_tail = ctl->fifo_count = 0;
    ctl->rd_fifo_tw = 0;
    ctl->wr_fifo_tw = 0;
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
    if ((ctl->active || ctl->enable_run) &&
        disk_control_has_transfer_op(ctl->control)) {
        /* Some op other than Done; reload FIFO with new sector. */
        if (disk_begin_read_stream(ctl)) {
            ctl->active = 1;
            ctl->read_stream_sector_starts++;
        }
    }
}

int dorado_disk_controller_tick(dorado_disk_controller *ctl,
                                uint64_t now_cycles)
{
    if (!ctl) return 0;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    /* Only a real Trident pack is clock-driven. PDI media (Cedar boot) is
     * completed at the IOCB level by the machine shim; ticking it would inject
     * spurious DSK wakeups into that path. A spun-down/empty drive is idle. */
    if (!d->pack || !d->online) return 0;

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
        if (disk_control_has_transfer_op(data)) ctl->control_transfer_loads++;
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
        if (data & DORADO_DISK_MUFF_CLEAR_ERRORS) {
            ctl->rd_fifo_tw = 0;
            ctl->wr_fifo_tw = 0;
        }
        break;

    case DORADO_DISK_TIOA_DISKDATA:
        /* Write FIFO data when the controller is in write mode. We
         * push into our software FIFO; the sequence-PROM execution
         * (Phase 2) drains it onto the disk. */
        if (ctl->fifo_count < DORADO_DISK_FIFO_WORDS) {
            ctl->fifo[ctl->fifo_head] = data;
            ctl->fifo_head = (ctl->fifo_head + 1) % DORADO_DISK_FIFO_WORDS;
            ctl->fifo_count++;
            ctl->fifo_writes++;
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
                if (bus & (1u << 1)) {
                    /* ReZero */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_cyl = 0;
                    d->cur_head = 0;
                    d->cur_sector = 0;
                    d->seek_in_progress = disk_sector_pulse_count(d);
                }
                if (bus & (1u << 0)) {
                    /* HeadAdvance */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_head++;
                }
                if (bus & (1u << 6)) {
                    /* Read — populate FIFO from current sector. */
                    if (disk_begin_read_stream(ctl)) {
                        ctl->active = 1;
                        ctl->read_stream_tag_starts++;
                    }
                }
                if (bus & (1u << 7)) {
                    /* Write — clear FIFO, mark write-active. Microcode
                     * will pump words via DiskData FIFO; we'll commit
                     * to the disk pack when a SectorOvfl-or-block-end
                     * marker is seen. Phase 2 stub: just enable
                     * WrFifoTW so microcode can write. */
                    ctl->wr_fifo_tw = 1;
                    ctl->active = 1;
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
    case 004:
        if (!ctl->rd_fifo_tw && !ctl->active && ctl->enable_run &&
            disk_control_has_transfer_op(ctl->control) &&
            disk_drive_has_media(d)) {
            if (disk_begin_read_stream(ctl)) {
                ctl->active = 1;
                ctl->read_stream_muff_starts++;
            }
        }
        return ctl->rd_fifo_tw;
    case 005: return ctl->wr_fifo_tw;
    case 010: return ctl->enable_run;
    case 011: return ctl->debug_mode;
    case 012:
        return !(ctl->active &&
                 disk_control_has_op(ctl->control, DORADO_DISK_OP_READ));
    case 013:
        return !(ctl->active &&
                 disk_control_has_op(ctl->control, DORADO_DISK_OP_WRITE));
    case 014:
        return !(ctl->active &&
                 disk_control_has_op(ctl->control, DORADO_DISK_OP_RDCHK));
    case 015: return ctl->active;
    case 016: return ctl->selected_drive & 1;
    case 017: return (ctl->selected_drive >> 1) & 1;
    case 021:
        /* DiskHeadDorado.Initialize classifies a T-80 by selecting head 5
         * and then reading HeadOvfl through DMux. PDI media represents the
         * Dorado Cedar boot volume as a 5-head T-80-style SA4000 volume, so
         * report the classification overflow even though the high-level PDI
         * backend does not model the physical head-select transient. */
        return d->head_overflow || (d->pdi != NULL);
    case 023: return !d->selected;
    case 024: return !d->online;
    case 025: return d->seek_in_progress || !d->online;
    case 032: return d->read_only;
    case 036:
    case 037: return 0;
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
            ctl->last_input_data = v;
            return v;
        }
        ctl->last_input_data = 0xFFFF;
        return 0xFFFF;

    case DORADO_DISK_TIOA_DISKMUFF: {
        int bit = disk_muffler_bit(ctl, ctl->muff_addr);
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
