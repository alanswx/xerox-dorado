#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    drv->online  = (pack != NULL) ? 1 : 0;
    drv->cur_cyl = 0;
    drv->cur_head = 0;
    drv->cur_sector = 0;
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

static int disk_sector_pulse_count(const dorado_disk_drive *d)
{
    if (d->sectors_per_revolution > 0) return d->sectors_per_revolution;
    if (d->pack && d->pack->geometry.sectors > 0) return d->pack->geometry.sectors;
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

void dorado_disk_controller_refill_fifo(dorado_disk_controller *ctl)
{
    if (!ctl || !ctl->read_stream_active) return;
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!d->pack) {
        ctl->read_stream_active = 0;
        return;
    }
    dorado_disk_sector *s = dorado_disk_pack_sector(
        d->pack, d->cur_cyl, d->cur_head, disk_media_sector(d));
    if (!s) {
        ctl->read_stream_active = 0;
        return;
    }

    const int total = DORADO_DISK_HEADER_WORDS +
                      DORADO_DISK_LABEL_WORDS +
                      DORADO_DISK_DATA_WORDS;
    while (ctl->fifo_count < DORADO_DISK_FIFO_WORDS &&
           ctl->read_stream_index < total) {
        ctl->fifo[ctl->fifo_head] = disk_sector_word(s, ctl->read_stream_index);
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
    if (!d->pack ||
        !dorado_disk_pack_sector(d->pack, d->cur_cyl, d->cur_head,
                                 disk_media_sector(d))) {
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
    return ctl->read_stream_active || ctl->fifo_count > 0;
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

static int disk_format_ram_requests_read(const dorado_disk_controller *ctl)
{
    if (!ctl) return 0;
    /* Format RAM word 4 is the read-op control tag command (HM §9,
     * read sequence PROM step 03). The native 12-bit command used by
     * Initial is 0104 octal: Control tag with Read set. */
    return (ctl->format_ram[4] & (1u << 6)) != 0;
}

void dorado_disk_controller_advance_sector(dorado_disk_controller *ctl)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!d->pack) return;
    d->cur_sector = (d->cur_sector + 1) % disk_sector_pulse_count(d);
    int at_index = (d->cur_sector == 0);

    if (d->seek_in_progress > 0) {
        d->seek_in_progress--;
        if (at_index) {
            d->seek_in_progress = 0;
            ctl->tag_tw = 1;
        }
    }

    if (at_index) {
        d->index_pulse = 1;
        ctl->index_tw = 1;
        ctl->sector_tw = 1;
        ctl->block_till_index = 0;
    } else {
        d->index_pulse = 0;
        if (ctl->block_till_index) return;
        ctl->sector_tw = 1;
    }

    /* If a transfer is active or queued by DiskControl, load the next
     * sector's data. Phase 2 simplification: real hardware sequences
     * this via the read PROM. */
    if ((ctl->active || ctl->enable_run) &&
        (disk_control_has_transfer_op(ctl->control) ||
         disk_format_ram_requests_read(ctl))) {
        /* Some op other than Done; reload FIFO with new sector. */
        if (disk_begin_read_stream(ctl)) {
            ctl->active = 1;
            ctl->read_stream_sector_starts++;
        }
    }
}

/* ─── Slow-IO command dispatch ───────────────────────────────────── */

static void disk_output_b(void *ctx, int task, uint8_t tioa, uint16_t data)
{
    dorado_disk_controller *ctl = ctx;
    ctl->output_count++;
    ctl->output_tioa_count[tioa & 0x0F]++;
    ctl->last_output_tioa = tioa;
    ctl->last_output_data = data;
    (void)task;

    switch (tioa) {
    case DORADO_DISK_TIOA_DISKCONTROL:
        /* Output to control register zeros the format-RAM address
         * register (HM page 98) so subsequent DiskRam writes start
         * at index 0. ClearEnableRun is honored. SetDebugMode is
         * honored. SetBlockTillIndex is honored. */
        ctl->control = data;
        ctl->format_ram_addr = 0;
        if (data & DORADO_DISK_CTRL_CLR_ENABLE_RUN) ctl->enable_run = 0;
        if (data & DORADO_DISK_CTRL_SET_DEBUG_MODE) ctl->debug_mode = 1;
        if (data & DORADO_DISK_CTRL_BLOCK_TILL_INDEX) ctl->block_till_index = 1;
        /* If ops 1..4 are non-zero AND EnableRun is set, schedule a
         * sector transfer at the next sector pulse. We don't run the
         * timing model in Phase 1 — record the request. */
        break;

    case DORADO_DISK_TIOA_DISKMUFF:
        /* DiskMuff output. DiskDefs.mc maps muffAddr to the low
         * byte and the wakeup/error clear controls to native octal
         * constants 04000, 02000, 01000, and 00400. */
        ctl->muff_addr = (uint8_t)(data & DORADO_DISK_MUFF_ADDR);
        if (data & DORADO_DISK_MUFF_CLEAR_INDEX_TW) ctl->index_tw = 0;
        if (data & DORADO_DISK_MUFF_CLEAR_SECTOR_TW) ctl->sector_tw = 0;
        if (data & DORADO_DISK_MUFF_CLEAR_SEEKTAG_TW) ctl->tag_tw = 0;
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
                if (d->pack && head < d->pack->geometry.heads) {
                    d->cur_head = head;
                } else if (d->pack) {
                    /* Invalid head — would set HeadOvfl on hardware. */
                    d->cur_head = head;  /* still record */
                }
                ctl->tag_tw = 1;          /* tag-completion wakeup */
                break;
            }
            case 2: {
                /* Cylinder Tag (HM page 100): Tag[4:15] = 12-bit
                 * cylinder number (LSB 0..11). */
                int cyl = bus;
                dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                if (d->pack && cyl < d->pack->geometry.cylinders) {
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
                if (!(bus & (1u << 1))) ctl->tag_tw = 1;
                break;
            }
            default:
                break;
            }
        }
        break;
    }
}

static uint16_t disk_input(void *ctx, int task, uint8_t tioa, int *bad)
{
    dorado_disk_controller *ctl = ctx;
    if (bad) *bad = 0;
    ctl->input_count++;
    ctl->input_tioa_count[tioa & 0x0F]++;
    ctl->last_input_tioa = tioa;
    (void)task;

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
        int bit = 0;
        dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
        switch (ctl->muff_addr) {
        case 001: bit = ctl->index_tw; break;
        case 002: bit = ctl->sector_tw; break;
        case 003: bit = ctl->tag_tw; break;
        case 004:
            if (!ctl->rd_fifo_tw && !ctl->active && ctl->enable_run && d->pack) {
                if (disk_begin_read_stream(ctl)) {
                    ctl->active = 1;
                    ctl->read_stream_muff_starts++;
                }
            }
            bit = ctl->rd_fifo_tw;
            break;
        case 005: bit = ctl->wr_fifo_tw; break;
        case 010: bit = ctl->enable_run; break;
        case 011: bit = ctl->debug_mode; break;
        case 012:
            bit = !(ctl->active &&
                    disk_control_has_op(ctl->control, DORADO_DISK_OP_READ));
            break;
        case 013:
            bit = !(ctl->active &&
                    disk_control_has_op(ctl->control, DORADO_DISK_OP_WRITE));
            break;
        case 014:
            bit = !(ctl->active &&
                    disk_control_has_op(ctl->control, DORADO_DISK_OP_RDCHK));
            break;
        case 015: bit = ctl->active; break;
        case 016: bit = ctl->selected_drive & 1; break;
        case 017: bit = (ctl->selected_drive >> 1) & 1; break;
        case 023: bit = !d->selected; break;
        case 024: bit = !d->online; break;
        case 025: bit = d->seek_in_progress || !d->online; break;
        case 032: bit = d->read_only; break;
        case 036:
        case 037: bit = 0; break;
        default: bit = 0; break;
        }
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
