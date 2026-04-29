#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const dorado_disk_geometry DORADO_DISK_T80  = { 815, 5, 9 };
const dorado_disk_geometry DORADO_DISK_T300 = { 815, 19, 9 };

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
 *     2 dummy bytes + header (4 words = 8 bytes) + label (20w = 40 b)
 *       + data (2048w = 4096 b)   = 4146 bytes/sector
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
}

void dorado_disk_drive_attach_pack(dorado_disk_drive *drv,
                                   dorado_disk_pack *pack)
{
    drv->pack    = pack;
    drv->online  = (pack != NULL) ? 1 : 0;
    drv->cur_cyl = 0;
    drv->cur_head = 0;
    drv->cur_sector = 0;
}

/* ─── Controller ────────────────────────────────────────────────── */

void dorado_disk_controller_init(dorado_disk_controller *ctl)
{
    memset(ctl, 0, sizeof *ctl);
    for (int i = 0; i < DORADO_DISK_NUM_DRIVES; i++) {
        dorado_disk_drive_init(&ctl->drive[i]);
    }
}

void dorado_disk_controller_attach_drive(dorado_disk_controller *ctl,
                                         int slot, dorado_disk_pack *pack)
{
    if (slot < 0 || slot >= DORADO_DISK_NUM_DRIVES) return;
    dorado_disk_drive_attach_pack(&ctl->drive[slot], pack);
}

/* ─── Slow-IO command dispatch ───────────────────────────────────── */

static void disk_output_b(void *ctx, int task, uint8_t tioa, uint16_t data)
{
    dorado_disk_controller *ctl = ctx;
    ctl->output_count++;
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
        /* Output sets the muffler address (which DDC-style signal we
         * read on the DiskMuff input). Phase 1: just record. */
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
         * Phase 1 records the tag; Phase 2 will decode (Cyl/Head/
         * Strobe/Op/Drive-select/etc.) and apply seek/read/write. */
        break;
    }
}

static uint16_t disk_input(void *ctx, int task, uint8_t tioa, int *bad)
{
    dorado_disk_controller *ctl = ctx;
    if (bad) *bad = 0;
    ctl->input_count++;
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
            return v;
        }
        return 0xFFFF;

    case DORADO_DISK_TIOA_DISKMUFF: {
        /* Muffler readout per HM §9. Returns one of the controller's
         * internal status bits selected by the previous DiskMuff
         * output. Phase 1 stub: pack the wakeup TWs into a status
         * word so microcode can at least see SOMETHING. */
        uint16_t v = 0;
        if (ctl->index_tw)    v |= (1u << 0);
        if (ctl->sector_tw)   v |= (1u << 1);
        if (ctl->tag_tw)      v |= (1u << 2);
        if (ctl->rd_fifo_tw)  v |= (1u << 3);
        if (ctl->wr_fifo_tw)  v |= (1u << 4);
        if (ctl->enable_run)  v |= (1u << 5);
        if (ctl->active)      v |= (1u << 6);
        return v;
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
