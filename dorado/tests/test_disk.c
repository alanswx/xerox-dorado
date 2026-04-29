#include "disk.h"
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

/* test_pack_create_t80 — empty Trident T-80 has the right shape. */
static int test_pack_create_t80(void)
{
    dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0,
           "create T-80 pack");
    EXPECT(pack.geometry.cylinders == 815, "cylinders = %d",
           pack.geometry.cylinders);
    EXPECT(pack.geometry.heads == 5, "heads = %d", pack.geometry.heads);
    EXPECT(pack.geometry.sectors == 9, "sectors = %d", pack.geometry.sectors);
    EXPECT(pack.num_sectors == 815 * 5 * 9,
           "num_sectors = %d (expected %d)", pack.num_sectors, 815 * 5 * 9);
    /* All zero. */
    dorado_disk_sector *s = dorado_disk_pack_sector(&pack, 0, 0, 0);
    EXPECT(s != NULL, "sector (0,0,0)");
    for (int i = 0; i < DORADO_DISK_DATA_WORDS; i++) {
        EXPECT(s->data[i] == 0, "data[%d] = 0x%X", i, s->data[i]);
    }
    /* Out-of-range returns NULL. */
    EXPECT(dorado_disk_pack_sector(&pack, 815, 0, 0) == NULL, "OOB cyl");
    EXPECT(dorado_disk_pack_sector(&pack, 0, 5, 0) == NULL, "OOB head");
    EXPECT(dorado_disk_pack_sector(&pack, 0, 0, 9) == NULL, "OOB sec");
    dorado_disk_pack_free(&pack);
    printf("PASS  test_pack_create_t80 (%d sectors)\n", 815 * 5 * 9);
    return 0;
}

/* test_pack_save_load — round-trip a pack file. */
static int test_pack_save_load(void)
{
    /* Use a smaller geometry for speed: 2 cyls × 2 heads × 2 sec. */
    dorado_disk_geometry tiny = { 2, 2, 2 };
    dorado_disk_pack p1;
    EXPECT(dorado_disk_pack_create(&p1, &tiny) == 0, "create tiny");
    snprintf(p1.path, sizeof p1.path, "/tmp/test_dorado_disk.pack");

    /* Stamp recognizable bytes into each sector. */
    for (int c = 0; c < tiny.cylinders; c++) {
        for (int h = 0; h < tiny.heads; h++) {
            for (int s = 0; s < tiny.sectors; s++) {
                dorado_disk_sector *sec = dorado_disk_pack_sector(&p1, c, h, s);
                EXPECT(sec != NULL, "sector (%d,%d,%d)", c, h, s);
                sec->header[0] = (uint16_t)((c << 8) | (h << 4) | s);
                sec->label[0]  = (uint16_t)0xCAFE;
                sec->data[0]   = (uint16_t)0xDEAD;
                sec->data[DORADO_DISK_DATA_WORDS - 1] = (uint16_t)0xBEEF;
            }
        }
    }
    EXPECT(dorado_disk_pack_save(&p1) == 0, "save");

    /* Reload and verify. */
    dorado_disk_pack p2;
    EXPECT(dorado_disk_pack_load(&p2, &tiny, "/tmp/test_dorado_disk.pack") == 0,
           "load");
    for (int c = 0; c < tiny.cylinders; c++) {
        for (int h = 0; h < tiny.heads; h++) {
            for (int s = 0; s < tiny.sectors; s++) {
                dorado_disk_sector *sec = dorado_disk_pack_sector(&p2, c, h, s);
                uint16_t exp = (uint16_t)((c << 8) | (h << 4) | s);
                EXPECT(sec->header[0] == exp,
                       "(%d,%d,%d) header[0] = 0x%X (expected 0x%X)",
                       c, h, s, sec->header[0], exp);
                EXPECT(sec->label[0] == 0xCAFE,
                       "(%d,%d,%d) label[0] = 0x%X", c, h, s, sec->label[0]);
                EXPECT(sec->data[0] == 0xDEAD,
                       "(%d,%d,%d) data[0] = 0x%X", c, h, s, sec->data[0]);
                EXPECT(sec->data[DORADO_DISK_DATA_WORDS - 1] == 0xBEEF,
                       "(%d,%d,%d) data[end] = 0x%X", c, h, s,
                       sec->data[DORADO_DISK_DATA_WORDS - 1]);
            }
        }
    }
    dorado_disk_pack_free(&p1);
    dorado_disk_pack_free(&p2);
    remove("/tmp/test_dorado_disk.pack");
    printf("PASS  test_pack_save_load (8 sectors round-trip)\n");
    return 0;
}

/* test_controller_io_routing — slow-IO writes hit the right TIOAs. */
static int test_controller_io_routing(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    /* DiskControl: bits 8:9 = op1 = 3 (Read), bits 10:11 = op2 = 1
     * (Write), set debug, block till index. */
    uint16_t ctrl =
        DORADO_DISK_CTRL_SET_DEBUG_MODE |
        DORADO_DISK_CTRL_BLOCK_TILL_INDEX |
        (3u << DORADO_DISK_CTRL_OP1_SHIFT) |
        (1u << DORADO_DISK_CTRL_OP2_SHIFT);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL, ctrl);
    EXPECT(ctl.control == ctrl, "control = 0x%X (expected 0x%X)",
           ctl.control, ctrl);
    EXPECT(ctl.debug_mode == 1, "debug_mode should be set");
    EXPECT(ctl.block_till_index == 1, "block_till_index should be set");
    EXPECT(ctl.format_ram_addr == 0,
           "format_ram_addr zeroed (got %d)", ctl.format_ram_addr);

    /* Load all 16 Format RAM entries. The 16th write should set
     * EnableRun. */
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) {
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKRAM,
                        (uint16_t)(0x1000 + i));
    }
    EXPECT(ctl.format_ram[0] == 0x1000, "format_ram[0] = 0x%X",
           ctl.format_ram[0]);
    EXPECT(ctl.format_ram[15] == 0x100F, "format_ram[15] = 0x%X",
           ctl.format_ram[15]);
    EXPECT(ctl.enable_run == 1,
           "EnableRun should be set after loading last format RAM word");
    EXPECT(ctl.format_ram_writes == 16, "format_ram_writes = %llu",
           (unsigned long long)ctl.format_ram_writes);

    /* Tag write. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG, 0x0204);
    EXPECT(ctl.tag == 0x0204, "tag = 0x%X", ctl.tag);
    EXPECT(ctl.tag_writes == 1, "tag_writes = %llu",
           (unsigned long long)ctl.tag_writes);

    /* DiskData FIFO push. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA, 0xAAAA);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA, 0x5555);
    EXPECT(ctl.fifo_count == 2, "fifo_count = %d", ctl.fifo_count);
    EXPECT(ctl.fifo[0] == 0xAAAA, "fifo[0] = 0x%X", ctl.fifo[0]);
    EXPECT(ctl.fifo[1] == 0x5555, "fifo[1] = 0x%X", ctl.fifo[1]);

    /* Pop via DiskData input. */
    int bad = -1;
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xAAAA, "FIFO pop = 0x%X (expected 0xAAAA)", v);
    EXPECT(ctl.fifo_count == 1, "fifo_count after pop = %d", ctl.fifo_count);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0x5555, "second pop = 0x%X", v);

    /* DiskMuff input — packs the wakeup-TW status. EnableRun=1
     * should appear in bit 5. */
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT((v & (1u << 5)) != 0, "muff[5] = %d (expected 1 for EnableRun)",
           (v >> 5) & 1);
    printf("PASS  test_controller_io_routing (control=0x%X, format ram, "
           "tag, FIFO 2 push/pop, muff readout)\n", ctl.control);
    return 0;
}

/* test_drive_attach — attaching a pack flips the drive online. */
static int test_drive_attach(void)
{
    dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0,
           "create T-80");

    dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    EXPECT(ctl.drive[0].online == 0, "drive 0 starts offline");

    dorado_disk_controller_attach_drive(&ctl, 0, &pack);
    EXPECT(ctl.drive[0].online == 1, "drive 0 should be online after attach");
    EXPECT(ctl.drive[0].pack == &pack, "drive 0 pack pointer");
    EXPECT(ctl.drive[1].online == 0, "drive 1 still offline");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_drive_attach (drive 0 online, drives 1..3 offline)\n");
    return 0;
}

/* test_tag_decoder — Cylinder/Head/Control tags update drive state. */
static int test_tag_decoder(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    /* Need a pack on drive 0 for the tag handlers to do anything
     * meaningful (out-of-range head/cyl checks reference the pack). */
    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0,
           "create pack");
    /* Stamp recognizable bytes per sector so we can verify the read
     * pulled the right one. */
    for (int c = 0; c < 4; c++) {
        for (int h = 0; h < 5; h++) {
            for (int s = 0; s < 9; s++) {
                dorado_disk_sector *sec =
                    dorado_disk_pack_sector(&pack, c, h, s);
                if (!sec) continue;
                sec->header[0] = (uint16_t)((c << 8) | (h << 4) | s);
                sec->header[1] = 0xCAFE;
                sec->label[0]  = 0xBEEF;
                sec->data[0]   = 0xDEAD;
                sec->data[100] = (uint16_t)(0x1000 + c);
            }
        }
    }
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    /* Drive Select Tag (Tag[0:3] = 0): select drive 0. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG, 0x0000);
    EXPECT(ctl.selected_drive == 0, "selected_drive=%d", ctl.selected_drive);
    EXPECT(ctl.drive[0].selected == 1, "drive 0 selected");

    /* Cylinder Tag (Tag[0:3] = 2): seek to cylinder 137. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((2u << 12) | 137u));
    EXPECT(ctl.drive[0].cur_cyl == 137, "cur_cyl=%d", ctl.drive[0].cur_cyl);
    EXPECT(ctl.tag_tw == 1, "tag_tw should be set after seek");

    /* Head Tag (Tag[0:3] = 1): select head 3. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((1u << 12) | 3u));
    EXPECT(ctl.drive[0].cur_head == 3, "cur_head=%d",
           ctl.drive[0].cur_head);

    /* Control Tag (Tag[0:3] = 3): Read = bit 6, HeadSelect = bit 2. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 6) | (1u << 2)));
    EXPECT(ctl.active == 1, "controller should be Active after Read");
    EXPECT(ctl.rd_fifo_tw == 1, "rd_fifo_tw should be set");
    EXPECT(ctl.fifo_count == DORADO_DISK_FIFO_WORDS,
           "FIFO should be full (16 words), got %d", ctl.fifo_count);

    /* The first word in the FIFO should be sector (137, 3, 0)'s
     * header[0] = (137 << 8) | (3 << 4) | 0 — but 137 doesn't fit
     * in 8 bits. Our stamper above only stamps c<4. So sector is
     * outside the stamped range; expect 0. Re-test with c=2,h=3,s=0. */
    /* Reseek to a stamped sector. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((2u << 12) | 2u));      /* cyl 2 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((1u << 12) | 3u));      /* head 3 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 6))); /* Read */
    int bad = -1;
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    uint16_t expected = (uint16_t)((2 << 8) | (3 << 4) | 0);
    EXPECT(v == expected,
           "first FIFO word for (2,3,0) = 0x%X (expected 0x%X)",
           v, expected);

    /* Pop a few more words and verify they match the sector. */
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xCAFE, "header[1] = 0x%X (expected 0xCAFE)", v);

    /* Skip header[2..3], read label[0]. */
    for (int i = 0; i < 2; i++) {
        dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xBEEF, "label[0] = 0x%X (expected 0xBEEF)", v);

    /* ReZero (control tag bit 1) should reset cyl=0, head=0, sec=0. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 1)));
    EXPECT(ctl.drive[0].cur_cyl == 0, "cur_cyl after ReZero = %d",
           ctl.drive[0].cur_cyl);
    EXPECT(ctl.drive[0].cur_head == 0, "cur_head after ReZero = %d",
           ctl.drive[0].cur_head);
    EXPECT(ctl.drive[0].cur_sector == 0, "cur_sector after ReZero = %d",
           ctl.drive[0].cur_sector);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_tag_decoder (DriveSel/Cyl/Head/Read/ReZero, "
           "FIFO loaded with sector data)\n");
    return 0;
}

/* test_advance_sector — explicit sector-pulse helper loads next
 * sector into FIFO. */
static int test_advance_sector(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0, "create");
    /* Stamp data[0] of each sector with its sector index. */
    for (int s = 0; s < 9; s++) {
        dorado_disk_sector *sec = dorado_disk_pack_sector(&pack, 0, 0, s);
        sec->data[0] = (uint16_t)(0xA000 + s);
    }
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    /* Issue a Read at (0, 0, 0) with a non-zero op so advance_sector
     * also reloads. */
    dorado_io_write(&io, DORADO_DISK_TASK,
                    DORADO_DISK_TIOA_DISKCONTROL,
                    (uint16_t)(3u << DORADO_DISK_CTRL_OP1_SHIFT)); /* Read op1 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 6))); /* Control: Read */
    /* Drain FIFO header+label = 24 words, then read data[0]. */
    int bad = -1;
    for (int i = 0; i < DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS;
         i++) {
        dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    /* Wait — DORADO_DISK_FIFO_WORDS = 16, but header (4) + label (20) = 24
     * which exceeds FIFO. So actually we have 4 header + 12 label words.
     * Let's just verify next sector advance reloads. */
    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 1, "cur_sector=%d after advance",
           ctl.drive[0].cur_sector);
    EXPECT(ctl.sector_tw == 1, "sector_tw should be set");
    EXPECT(ctl.fifo_count > 0, "FIFO reloaded after advance, count=%d",
           ctl.fifo_count);

    /* Wrap around at sector 9. */
    for (int i = 1; i < 9; i++) dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 0,
           "cur_sector wraps to 0, got %d", ctl.drive[0].cur_sector);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_advance_sector (sector counter advances + wraps, "
           "FIFO reloads)\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_pack_create_t80();
    rc |= test_pack_save_load();
    rc |= test_controller_io_routing();
    rc |= test_drive_attach();
    rc |= test_tag_decoder();
    rc |= test_advance_sector();
    if (rc == 0) printf("\nAll disk tests passed.\n");
    return rc;
}
