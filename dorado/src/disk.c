#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const dorado_disk_geometry DORADO_DISK_T80  = { 815, 5, 9 };
const dorado_disk_geometry DORADO_DISK_T300 = { 815, 19, 9 };

#define DORADO_DISK_SUBSECTOR_PULSES_PER_REV 117

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
        (DORADO_DISK_SUBSECTOR_PULSES_PER_REV + divisor - 1) / divisor;
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

static void disk_begin_read_stream(dorado_disk_controller *ctl)
{
    ctl->fifo_count = 0;
    ctl->fifo_head = 0;
    ctl->fifo_tail = 0;
    ctl->read_stream_index = 0;
    ctl->read_stream_active = 1;
    dorado_disk_controller_refill_fifo(ctl);
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

void dorado_disk_controller_advance_sector(dorado_disk_controller *ctl)
{
    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
    if (!d->pack) return;
    d->cur_sector = (d->cur_sector + 1) % disk_sector_pulse_count(d);
    int at_index = (d->cur_sector == 0);

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
        disk_control_has_transfer_op(ctl->control)) {
        /* Some op other than Done; reload FIFO with new sector. */
        ctl->active = 1;
        disk_begin_read_stream(ctl);
    }
}

/* ─── Slow-IO command dispatch ───────────────────────────────────── */

static void disk_output_b(void *ctx, int task, uint8_t tioa, uint16_t data)
{
    dorado_disk_controller *ctl = ctx;
    ctl->output_count++;
    ctl->output_tioa_count[tioa & 0x0F]++;
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
        /* Output selects the muffler address in the byte produced by
         * FF,,0 constants (e.g. 0x0200 selects SectorTW at address 002).
         * The low byte carries one-shot operations such as clear TWs. */
        ctl->muff_addr = (uint8_t)((data >> 8) & 0xFF);
        if (data & (1u << 0)) ctl->index_tw = 0;
        if (data & (1u << 1)) ctl->sector_tw = 0;
        if (data & (1u << 2)) ctl->tag_tw = 0;
        if (data & (1u << 3)) {
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
        /* Tag commands drive the daisy-chain to the selected drive.
         * Decode Tag[0:3] (HM page 99-101). Per the manual, Tag[0:3]
         * is the high 4 bits of the data word in MSB-first numbering
         * = bits 12:15 in our LSB convention. */
        {
            int tag_type = (data >> 12) & 0xF;  /* MSB-first 0:3 = LSB 12..15 */
            switch (tag_type) {
            case 0: {
                /* Drive Select / subsector count (HM page 100).
                 * Tag[11:15] = drive select (5 bits, but bits 11:15
                 * in MSB = bits 0..4 in LSB).
                 * Tag[10] = "load subsector count" (LSB bit 5).
                 * Tag[4:9] = subsector count (LSB bits 6..11). */
                int drv_select = data & 0x1F;
                if (data & (1u << 5)) {
                    int count = (data >> 6) & 0x3F;
                    disk_set_subsector_count(&ctl->drive[ctl->selected_drive],
                                             count);
                }
                if (drv_select <= 3) {
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
                int head = data & 0x3F;
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
                int cyl = data & 0xFFF;
                dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                if (d->pack && cyl < d->pack->geometry.cylinders) {
                    d->cur_cyl = cyl;
                    d->cur_sector = 0;       /* lose sector sync on seek */
                    d->seek_in_progress = 0; /* simulated as instant */
                }
                ctl->tag_tw = 1;
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
                if (data & (1u << 1)) {
                    /* ReZero */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_cyl = 0;
                    d->cur_head = 0;
                    d->cur_sector = 0;
                }
                if (data & (1u << 0)) {
                    /* HeadAdvance */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    d->cur_head++;
                }
                if (data & (1u << 6)) {
                    /* Read — populate FIFO from current sector. */
                    dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
                    if (d->pack) {
                        if (dorado_disk_pack_sector(d->pack, d->cur_cyl,
                                                    d->cur_head,
                                                    d->cur_sector)) {
                            disk_begin_read_stream(ctl);
                            ctl->active = 1;
                        }
                    }
                }
                if (data & (1u << 7)) {
                    /* Write — clear FIFO, mark write-active. Microcode
                     * will pump words via DiskData FIFO; we'll commit
                     * to the disk pack when a SectorOvfl-or-block-end
                     * marker is seen. Phase 2 stub: just enable
                     * WrFifoTW so microcode can write. */
                    ctl->wr_fifo_tw = 1;
                    ctl->active = 1;
                }
                ctl->tag_tw = 1;
                break;
            }
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
            return v;
        }
        return 0xFFFF;

    case DORADO_DISK_TIOA_DISKMUFF: {
        int bit = 0;
        dorado_disk_drive *d = &ctl->drive[ctl->selected_drive];
        switch (ctl->muff_addr) {
        case 001: bit = ctl->index_tw; break;
        case 002: bit = ctl->sector_tw; break;
        case 003: bit = ctl->tag_tw; break;
        case 004: bit = ctl->rd_fifo_tw; break;
        case 005: bit = ctl->wr_fifo_tw; break;
        case 010: bit = ctl->enable_run; break;
        case 011: bit = ctl->debug_mode; break;
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
        /* HM pages 101-102: selected muffler signal is returned on
         * IOB[15]. Manual bit 15 is the low C bit in this codebase. */
        return bit ? 0x0001 : 0x0000;
    }
    }

    /* Unknown TIOA — return floating bus. */
    if (bad) *bad = 1;
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
