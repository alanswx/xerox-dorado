#include "disk.h"
#include "io.h"
#include "pdi.h"

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

#define TAG_DRIVE    0x8000u
#define TAG_CYLINDER 0x4000u
#define TAG_HEAD     0x2000u
#define TAG_CONTROL  0x1000u

#define MUFF_CLEAR_INDEX_TW   0x0800u
#define MUFF_CLEAR_SECTOR_TW  0x0400u
#define MUFF_CLEAR_SEEKTAG_TW 0x0200u
#define MUFF_CLEAR_COMPARE_ERR 0x2000u
#define MUFF_CLEAR_ERRORS     0x0100u

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
    dorado_disk_geometry tiny = { 2, 2, 2, 0, 0, 0 };
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

    /* DiskMuff input — select EnableRun (muffler address 010 in the
     * low byte) and verify the selected signal is returned on Dorado
     * IOB[15], represented as C bit 15 / low bit in this emulator. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    010);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT(v == 0x0001, "muff EnableRun = 0x%X (expected IOB[15])", v);
    ctl.tag_tw = 1;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    002);
    EXPECT(ctl.muff_addr == 002, "muff_addr = 0o%o", ctl.muff_addr);
    EXPECT(ctl.tag_tw == 1, "address select must not clear TagTW");
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    MUFF_CLEAR_SEEKTAG_TW);
    EXPECT(ctl.tag_tw == 0, "native clearSeekTagTW should clear TagTW");

    /* The block-mode KSTATE signals are primed in the hardware manual:
     * idle reads high, and the matching active block mode reads low. */
    ctl.active = 0;
    ctl.control = 0;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    014);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT(v == 0x0001, "idle CheckBlock' = 0x%X", v);

    ctl.active = 1;
    ctl.control = DORADO_DISK_OP_RDCHK << DORADO_DISK_CTRL_OP1_SHIFT;
    ctl.current_block_op = DORADO_DISK_OP_RDCHK;
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT(v == 0x0000, "active CheckBlock' = 0x%X", v);

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    012);
    ctl.current_block_op = DORADO_DISK_OP_RDCHK;
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT(v == 0x0001, "non-read RdOnlyBlock' = 0x%X", v);

    printf("PASS  test_controller_io_routing (control=0x%X, format ram, "
           "tag, FIFO 2 push/pop, muff readout)\n", ctl.control);
    return 0;
}

/* test_diskcontrol_active_abort — HM page 97 says a DiskControl
 * output while Active aborts the sector transfer; the next output
 * loads the control register. */
static int test_diskcontrol_active_abort(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    ctl.active = 1;
    ctl.read_stream_active = 1;
    ctl.read_stream_index = 7;
    ctl.fifo_count = 3;
    ctl.fifo_head = 3;
    ctl.rd_fifo_tw = 1;
    ctl.control = 0x0123;
    ctl.format_ram_addr = 9;

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    DORADO_DISK_CTRL_CLR_ENABLE_RUN);
    EXPECT(ctl.active == 0, "active should clear on abort");
    EXPECT(ctl.read_stream_active == 0, "stream should stop on abort");
    EXPECT(ctl.fifo_count == 0, "FIFO should clear on abort");
    EXPECT(ctl.rd_fifo_tw == 0, "RdFifoTW should clear on abort");
    EXPECT(ctl.control == 0x0123,
           "aborting output must not load control, got 0x%X", ctl.control);
    EXPECT(ctl.format_ram_addr == 9,
           "aborting output must not reset RAM addr, got %d",
           ctl.format_ram_addr);

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    DORADO_DISK_CTRL_BLOCK_TILL_INDEX);
    EXPECT(ctl.control == DORADO_DISK_CTRL_BLOCK_TILL_INDEX,
           "second output should load control, got 0x%X", ctl.control);
    EXPECT(ctl.format_ram_addr == 0,
           "second output should zero RAM addr, got %d",
           ctl.format_ram_addr);
    EXPECT(ctl.block_till_index == 1, "BlockTillIndex should set");

    printf("PASS  test_diskcontrol_active_abort (abort then load)\n");
    return 0;
}

static int test_rd_fifo_muffler_does_not_start_transfer(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_DIABLO) == 0,
           "create Diablo pack");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    ctl.enable_run = 0;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    (uint16_t)(DORADO_DISK_OP_RDCHK <<
                               DORADO_DISK_CTRL_OP1_SHIFT));
    EXPECT(ctl.xfer_pending == 1, "transfer should be pending");

    int bad = -1;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF, 004);
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKMUFF, &bad);
    EXPECT(v == 0, "RdFifoTW poll before sector should be 0, got 0x%X", v);
    EXPECT(ctl.read_stream_active == 0,
           "RdFifoTW poll must not start a read stream");
    EXPECT(ctl.active == 0, "RdFifoTW poll must not set Active");
    EXPECT(ctl.xfer_pending == 1, "pending transfer should remain armed");
    EXPECT(ctl.read_stream_muff_starts == 0,
           "muffler-start counter should stay zero");

    ctl.enable_run = 1;
    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.read_stream_active == 1,
           "sector pulse should start the pending transfer");
    EXPECT(ctl.active == 1, "sector-started transfer should set Active");
    EXPECT(ctl.xfer_pending == 0, "transfer should no longer be pending");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_rd_fifo_muffler_does_not_start_transfer\n");
    return 0;
}

static int test_read_check_wakeup_and_error_latches(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_DIABLO) == 0,
           "create Diablo pack");
    dorado_disk_sector *sec = dorado_disk_pack_sector(&pack, 0, 0, 1);
    EXPECT(sec != NULL, "sector exists");
    sec->header[0] = 012345;
    sec->header[1] = 054321;
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    /* Alto Diablo format RAM counts: header=2, label=8, data=256. */
    uint16_t fram[DORADO_DISK_FORMAT_RAM_WORDS] = {
        0001, 0007, 0377, 0000,
        0104, 0204, 0004, 0000,
        0033, 0006, 0011, 0002,
        0002, 0001, 0000, 0000,
    };
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) {
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKRAM,
                        fram[i]);
    }
    EXPECT(ctl.enable_run == 1, "Format RAM load should set EnableRun");

    ctl.drive[0].cur_sector = 1;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    (uint16_t)(DORADO_DISK_OP_RDCHK <<
                               DORADO_DISK_CTRL_OP1_SHIFT));
    EXPECT(ctl.drive[0].cur_sector == 1,
           "DiskControl should start in the current sector");
    EXPECT(ctl.current_block == 0, "current block=%u", ctl.current_block);
    EXPECT(ctl.current_block_op == DORADO_DISK_OP_RDCHK,
           "current op=%u", ctl.current_block_op);
    EXPECT(ctl.current_block_words == 2,
           "header count should come from Format RAM, got %u",
           ctl.current_block_words);
    EXPECT(ctl.compare_err == 1, "check block should set CompareErr");
    EXPECT(ctl.rd_fifo_tw == 1,
           "read+check should wake with one or more FIFO words");
    EXPECT(ctl.read_data_err == 0,
           "ReadDataErr latch should stay clear until CompareErr is missed");
    EXPECT(dorado_disk_controller_dmux_read(&ctl, 02031, NULL) == 0x8000,
           "ReadDataErr muffler should report a pending CompareErr");

    int bad = -1;
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 012345, "framed header first word=0o%o", v);

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    MUFF_CLEAR_COMPARE_ERR);
    EXPECT(ctl.compare_err == 0, "clearCompareErr should clear CompareErr");
    EXPECT(dorado_disk_controller_dmux_read(&ctl, 02031, NULL) == 0,
           "ReadDataErr should be clear after successful compare");

    ctl.read_data_err = 1;
    EXPECT(dorado_disk_controller_dmux_read(&ctl, 02031, NULL) == 0x8000,
           "ReadDataErr should appear via ReadDataErr muffler");
    EXPECT(dorado_disk_controller_dmux_read(&ctl, 02037, NULL) == 0x8000,
           "ReadDataErr should appear via ReadErr muffler");

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKMUFF,
                    MUFF_CLEAR_ERRORS);
    EXPECT(ctl.compare_err == 0, "clearErrors should clear CompareErr");
    EXPECT(ctl.read_data_err == 0, "clearErrors should clear ReadDataErr");

    ctl.compare_err = 1;
    ctl.read_data_err = 0;
    ctl.read_stream_active = 1;
    ctl.active = 1;
    ctl.read_block_framing = 1;
    ctl.current_block = 0;
    ctl.current_block_op = DORADO_DISK_OP_RDCHK;
    ctl.current_block_words = 0;
    ctl.current_block_pos = 0;
    ctl.current_block_trailer = 4;
    ctl.fifo_head = ctl.fifo_tail = ctl.fifo_count = 0;
    ctl.control = 0;
    dorado_disk_controller_refill_fifo(&ctl);
    EXPECT(ctl.read_data_err == 0,
           "missed ClearCompareErr should not latch ReadDataErr early");
    EXPECT(ctl.compare_err == 1,
           "missed ClearCompareErr should leave CompareErr pending");
    EXPECT(dorado_disk_controller_dmux_read(&ctl, 02031, NULL) == 0x8000,
           "pending CompareErr should remain visible through ReadDataErr");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_read_check_wakeup_and_error_latches\n");
    return 0;
}

static int test_mixed_read_write_retargets_latched_sector(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_DIABLO) == 0,
           "create Diablo pack");
    dorado_disk_sector *stale = dorado_disk_pack_sector(&pack, 0, 0, 0);
    dorado_disk_sector *target = dorado_disk_pack_sector(&pack, 0, 0, 1);
    EXPECT(stale && target, "test sectors exist");
    stale->label[0] = 0111;
    target->label[0] = 0222;
    target->header[0] = 012345;
    target->header[1] = 054321;
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    uint16_t fram[DORADO_DISK_FORMAT_RAM_WORDS] = {
        0001, 0007, 0377, 0000,
        0104, 0204, 0004, 0000,
        0033, 0006, 0011, 0002,
        0002, 0001, 0000, 0000,
    };
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) {
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKRAM,
                        fram[i]);
    }

    ctl.write_stream_sector = stale;
    ctl.drive[0].cur_sector = 1;
    uint16_t ctrl =
        (uint16_t)((DORADO_DISK_OP_RDCHK << DORADO_DISK_CTRL_OP1_SHIFT) |
                   (DORADO_DISK_OP_WRITE << DORADO_DISK_CTRL_OP2_SHIFT));
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    ctrl);
    EXPECT(ctl.write_stream_sector == NULL,
           "new DiskControl transfer should clear stale write sector");

    EXPECT(ctl.read_stream_sector == target,
           "read phase should latch target sector");

    int bad = -1;
    for (int i = 0; i < 2 + 4; i++)
        (void)dorado_io_read(&io, DORADO_DISK_TASK,
                             DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(ctl.write_stream_active == 1, "write block should be active");
    EXPECT(ctl.write_stream_sector == target,
           "write block should retarget to read sector");

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA,
                    0201); /* sync */
    for (int i = 0; i < 8; i++) {
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA,
                        (uint16_t)(01000 + i));
    }
    EXPECT(target->label[0] == 01007 && target->label[7] == 01000,
           "mixed write should update target label, got [%o,%o]",
           target->label[0], target->label[7]);
    EXPECT(stale->label[0] == 0111,
           "stale sector label should be unchanged, got %o",
           stale->label[0]);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_mixed_read_write_retargets_latched_sector\n");
    return 0;
}

static int test_restore_header_check_bit(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_DIABLO) == 0,
           "create Diablo pack");
    dorado_disk_sector *sec = dorado_disk_pack_sector(&pack, 0, 0, 1);
    EXPECT(sec != NULL, "sector exists");
    sec->header[0] = 012340;
    sec->header[1] = 052524;
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    uint16_t fram[DORADO_DISK_FORMAT_RAM_WORDS] = {
        0001, 0007, 0377, 0000,
        0104, 0204, 0004, 0000,
        0033, 0006, 0011, 0002,
        0002, 0001, 0000, 0000,
    };
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) {
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKRAM,
                        fram[i]);
    }

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 1))); /* ReZero */
    EXPECT(ctl.restore_pending == 1, "ReZero should mark restore pending");

    ctl.drive[0].cur_sector = 1;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKCONTROL,
                    (uint16_t)(DORADO_DISK_OP_RDCHK <<
                               DORADO_DISK_CTRL_OP1_SHIFT));
    EXPECT(ctl.restore_header_check == 1,
           "DiskControl should carry restore into the immediate transfer");

    int bad = -1;
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 012341, "restore header first word = 0o%o", v);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 052524, "restore tolerance must not alter header word 1");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_restore_header_check_bit\n");
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

/* test_dmux_muffler_read — DiskHeadDorado reads the disk muffler
 * through DMux addresses 02000..02037 during controller discovery. */
static int test_dmux_muffler_read(void)
{
    dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);

    int handled = -1;
    uint16_t v = dorado_disk_controller_dmux_read(&ctl, 01777, &handled);
    EXPECT(handled == 0, "DMux 01777 should not be handled");
    EXPECT(v == 0, "unhandled DMux read should return 0, got 0x%X", v);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02023, &handled);
    EXPECT(handled == 1, "DMux 02023 should be handled");
    EXPECT(v == 0x8000, "offline drive NotSelected' = 0x%X", v);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02024, &handled);
    EXPECT(handled == 1, "DMux 02024 should be handled");
    EXPECT(v == 0x8000, "offline drive NotOnline' = 0x%X", v);

    dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0,
           "create T-80");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02024, &handled);
    EXPECT(handled == 1, "DMux 02024 online should be handled");
    EXPECT(v == 0x8000, "unselected online drive NotOnline' = 0x%X", v);

    dorado_io io;
    dorado_io_init(&io);
    dorado_disk_controller_attach_to_io(&ctl, &io);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG, 020);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    TAG_DRIVE | 020);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG, 020);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02024, &handled);
    EXPECT(handled == 1, "DMux 02024 selected online should be handled");
    EXPECT(v == 0x0000, "online drive NotOnline' = 0x%X", v);

    ctl.enable_run = 1;
    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02010, &handled);
    EXPECT(handled == 1, "DMux 02010 should be handled");
    EXPECT(v == 0x8000, "EnableRun DMux bit = 0x%X", v);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02021, &handled);
    EXPECT(handled == 1, "DMux 02021 should be handled");
    EXPECT(v == 0x0000, "HeadOvfl starts clear = 0x%X", v);

    handled = 0;
    v = dorado_disk_controller_dmux_read(&ctl, 02037, &handled);
    EXPECT(handled == 1, "DMux 02037 should be handled");
    EXPECT(v == 0x0000, "ReadError DMux bit 02037 = 0x%X", v);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_dmux_muffler_read (DMux 02000..02037 disk bits)\n");
    return 0;
}

/* test_pdi_fifo_read — Pilot/Cedar PDI pages are exposed as
 * header+label+256 data words, using DiskHeadDorado's system80 page
 * address mapping (logical cylinder * 28 + sector). */
static int test_pdi_fifo_read(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    dorado_pdi pdi;
    memset(&pdi, 0, sizeof pdi);
    pdi.version = 1;
    pdi.fs_family = DORADO_PDI_FS_PILOT;
    pdi.page_count = 140;
    pdi.label_words = DORADO_PILOT_LABEL_WORDS;
    pdi.data_words = DORADO_PILOT_DATA_WORDS;
    pdi.labels = calloc((size_t)pdi.page_count * pdi.label_words,
                        sizeof(uint16_t));
    pdi.data = calloc((size_t)pdi.page_count * pdi.data_words,
                      sizeof(uint16_t));
    EXPECT(pdi.labels && pdi.data, "alloc synthetic PDI");

    const uint32_t page = 119;
    pdi.labels[(size_t)page * pdi.label_words + 0] = 0xCAFE;
    pdi.labels[(size_t)page * pdi.label_words + 9] = 0xBABE;
    pdi.data[(size_t)page * pdi.data_words + 0] = 0x1234;
    pdi.data[(size_t)page * pdi.data_words + 255] = 0x5678;

    dorado_disk_controller_attach_pdi(&ctl, 0, &pdi);
    EXPECT(ctl.drive[0].online == 1, "PDI drive online");
    EXPECT(ctl.drive[0].read_only == 1, "PDI drive read-only");
    int handled = 0;
    uint16_t hv = dorado_disk_controller_dmux_read(&ctl, 02021, &handled);
    EXPECT(handled == 1 && hv == 0x8000,
           "PDI drive should classify as T-80 HeadOvfl, handled=%d v=0x%X",
           handled, hv);

    ctl.drive[0].cur_cyl = 4;
    ctl.drive[0].cur_sector = 7;
    dorado_io_write(&io, DORADO_DISK_TASK,
                    DORADO_DISK_TIOA_DISKCONTROL,
                    (uint16_t)(3u << DORADO_DISK_CTRL_OP1_SHIFT));
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 6)));

    int bad = -1;
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == page, "PDI synthetic header word0=0x%X", v);
    (void)dorado_io_read(&io, DORADO_DISK_TASK,
                         DORADO_DISK_TIOA_DISKDATA, &bad);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xCAFE, "PDI label[0]=0x%X", v);
    for (int i = 1; i < DORADO_PILOT_LABEL_WORDS; i++) {
        v = dorado_io_read(&io, DORADO_DISK_TASK,
                           DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    EXPECT(v == 0xBABE, "PDI label[9]=0x%X", v);
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0x1234, "PDI data[0]=0x%X", v);
    for (int i = 1; i < DORADO_PILOT_DATA_WORDS; i++) {
        v = dorado_io_read(&io, DORADO_DISK_TASK,
                           DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    EXPECT(v == 0x5678, "PDI data[255]=0x%X", v);

    free(pdi.labels);
    free(pdi.data);
    printf("PASS  test_pdi_fifo_read (PDI page -> header/label/data FIFO)\n");
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

    /* Plain bus preload should not execute without the native strobe. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((1u << 5) | (3u << 6)));
    EXPECT(ctl.drive[0].subsector_count == 3,
           "unstrobed bus should not alter subsector_count=%d",
           ctl.drive[0].subsector_count);

    /* Native DriveTag strobe: select drive 0 and load subsector count. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_DRIVE | (1u << 5) | (3u << 6) |
                               (1u << 4)));
    EXPECT(ctl.selected_drive == 0, "selected_drive=%d", ctl.selected_drive);
    EXPECT(ctl.drive[0].selected == 1, "drive 0 selected");

    /* Native CylinderTag strobe: seek to cylinder 137. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CYLINDER | 137u));
    EXPECT(ctl.drive[0].cur_cyl == 137, "cur_cyl=%d", ctl.drive[0].cur_cyl);
    EXPECT(ctl.drive[0].seek_in_progress > 0,
           "seek_in_progress should be raised after Cylinder Tag");
    EXPECT(ctl.tag_tw == 1,
           "Cylinder Tag should raise TagTW after tag timing");
    ctl.tag_tw = 0;
    for (int i = 0; i < ctl.drive[0].sectors_per_revolution; i++) {
        dorado_disk_controller_advance_sector(&ctl);
    }
    EXPECT(ctl.drive[0].seek_in_progress == 0,
           "seek should complete at index");
    ctl.index_tw = 0;
    ctl.sector_tw = 0;

    /* Native HeadTag strobe: select head 3. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_HEAD | 3u));
    EXPECT(ctl.drive[0].cur_head == 3, "cur_head=%d",
           ctl.drive[0].cur_head);

    /* Native ControlTag strobe: Read = bit 6, HeadSelect = bit 2. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 6) | (1u << 2)));
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
                    (uint16_t)(TAG_CYLINDER | 2u));      /* cyl 2 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_HEAD | 3u));      /* head 3 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 6))); /* Read */
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

    /* Skip any remaining header words, then read label[0]. */
    for (int i = 2; i < DORADO_DISK_HEADER_WORDS; i++) {
        dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    v = dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xBEEF, "label[0] = 0x%X (expected 0xBEEF)", v);
    ctl.tag_tw = 0;

    /* ReZero (control tag bit 1) should reset cyl=0, head=0, sec=0. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 1)));
    EXPECT(ctl.drive[0].cur_cyl == 0, "cur_cyl after ReZero = %d",
           ctl.drive[0].cur_cyl);
    EXPECT(ctl.drive[0].cur_head == 0, "cur_head after ReZero = %d",
           ctl.drive[0].cur_head);
    EXPECT(ctl.drive[0].cur_sector == 0, "cur_sector after ReZero = %d",
           ctl.drive[0].cur_sector);
    EXPECT(ctl.drive[0].seek_in_progress > 0,
           "seek_in_progress should be raised after ReZero");
    EXPECT(ctl.tag_tw == 1, "ReZero should raise TagTW after tag timing");

    ctl.drive[0].cur_cyl = 123;
    ctl.drive[0].cur_head = 4;
    ctl.drive[0].cur_sector = 5;
    ctl.tag_tw = 0;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | 0x000A));
    EXPECT(ctl.drive[0].cur_cyl == 0,
           "native control-tag ReZero cur_cyl = %d", ctl.drive[0].cur_cyl);
    EXPECT(ctl.drive[0].cur_head == 0,
           "native control-tag ReZero cur_head = %d", ctl.drive[0].cur_head);
    EXPECT(ctl.drive[0].cur_sector == 0,
           "native control-tag ReZero cur_sector = %d",
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
                    (uint16_t)(TAG_CONTROL | (1u << 6))); /* Control: Read */
    /* Drain FIFO header+label = 24 words, then read data[0]. The
     * controller refills the 16-word FIFO from the active sector stream. */
    int bad = -1;
    for (int i = 0; i < DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS;
         i++) {
        dorado_io_read(&io, DORADO_DISK_TASK,
                       DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    uint16_t v = dorado_io_read(&io, DORADO_DISK_TASK,
                                DORADO_DISK_TIOA_DISKDATA, &bad);
    EXPECT(v == 0xA000, "sector 0 data[0]=0x%X", v);

    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 1,
           "physical rotation should advance cur_sector=%d",
           ctl.drive[0].cur_sector);

    ctl.active = 0;
    ctl.read_stream_active = 0;
    ctl.fifo_head = ctl.fifo_tail = ctl.fifo_count = 0;
    ctl.rd_fifo_tw = 0;
    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 2, "cur_sector=%d after advance",
           ctl.drive[0].cur_sector);
    EXPECT(ctl.sector_tw == 1, "sector_tw should be set");

    /* Boot drive 0 uses Pilot's 29-position controller sector cadence;
     * media lookup maps those positions onto the 9-sector image. */
    int sectors_per_rev = ctl.drive[0].sectors_per_revolution;
    for (int i = 2; i < sectors_per_rev; i++) dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 0,
           "cur_sector wraps to 0, got %d", ctl.drive[0].cur_sector);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_advance_sector (active hold, counter advances + wraps)\n");
    return 0;
}

/* test_block_till_index — BlockTillIndex suppresses new sector wakeups
 * until the selected drive reaches index, where IndexTW and SectorTW
 * assert together and the latch clears. */
static int test_block_till_index(void)
{
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0, "create");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    ctl.block_till_index = 1;
    ctl.drive[0].cur_sector = 0;
    ctl.drive[0].seek_in_progress = ctl.drive[0].sectors_per_revolution;

    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 1, "cur_sector=%d after advance",
           ctl.drive[0].cur_sector);
    EXPECT(ctl.block_till_index == 1, "block_till_index should remain set");
    EXPECT(ctl.sector_tw == 0, "sector_tw should be masked before index");
    EXPECT(ctl.index_tw == 0, "index_tw should stay clear before index");
    EXPECT(ctl.drive[0].seek_in_progress > 0,
           "seek should still be in progress before index");
    EXPECT(!dorado_disk_controller_wakeup_pending(&ctl),
           "no wakeup should be pending before index");

    for (int i = 1; i < ctl.drive[0].sectors_per_revolution; i++) {
        dorado_disk_controller_advance_sector(&ctl);
    }
    EXPECT(ctl.drive[0].cur_sector == 0, "cur_sector should wrap to index");
    EXPECT(ctl.block_till_index == 0, "block_till_index should clear");
    EXPECT(ctl.sector_tw == 1, "index pulse should also set sector_tw");
    EXPECT(ctl.index_tw == 1, "index_tw should be set at index");
    EXPECT(ctl.drive[0].seek_in_progress == 0,
           "seek should complete at index");
    EXPECT(dorado_disk_controller_wakeup_pending(&ctl),
           "index wakeup should be pending");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_block_till_index (masked sectors, index wakeup)\n");
    return 0;
}

/* test_drive_select_subsector_count — Drive Select Tag[10] loads the
 * selected drive's subsector divider. TriconD verifies that count 3 produces
 * 30 controller sector wakeups per 117-pulse revolution. */
static int test_drive_select_subsector_count(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0, "create");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_DRIVE | (1u << 5) | (3u << 6)));
    EXPECT(ctl.drive[0].subsector_count == 3,
           "subsector_count=%d", ctl.drive[0].subsector_count);
    EXPECT(ctl.drive[0].sectors_per_revolution == 30,
           "sectors_per_revolution=%d",
           ctl.drive[0].sectors_per_revolution);

    /* Pilot keeps a non-even-sector flag in KSelect bit 4, which is
     * carried as native bit 11 (0x0800) in the DriveTag word. It is
     * not part of tagSubSector and must not turn count 3 into 35. */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_DRIVE | 0x0800u | (1u << 5) | (3u << 6) |
                               (1u << 4)));
    EXPECT(ctl.drive[0].subsector_count == 3,
           "KSelect-flagged subsector_count=%d",
           ctl.drive[0].subsector_count);
    EXPECT(ctl.selected_drive == 0, "selected_drive=%d", ctl.selected_drive);

    for (int i = 0; i < 29; i++) dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 29,
           "cur_sector=%d before index", ctl.drive[0].cur_sector);
    EXPECT(ctl.index_tw == 0, "index_tw should not assert before wrap");

    dorado_disk_controller_advance_sector(&ctl);
    EXPECT(ctl.drive[0].cur_sector == 0,
           "cur_sector should wrap at 30, got %d", ctl.drive[0].cur_sector);
    EXPECT(ctl.index_tw == 1, "index_tw should assert at wrapped sector");
    EXPECT(ctl.sector_tw == 1, "index pulse should also assert sector_tw");

    dorado_disk_pack_free(&pack);
    printf("PASS  test_drive_select_subsector_count (count 3 -> 30 pulses/rev)\n");
    return 0;
}

/* test_clock_timing — dorado_disk_controller_tick advances sector/index
 * pulses from the cycle clock at the 3600 RPM cadence, and only for a real
 * pack (PDI media stays idle so the IOCB shim path isn't perturbed). */
static int test_clock_timing(void)
{
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);

    static dorado_disk_pack pack;
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0, "create");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);
    ctl.enable_run = 1;

    int spr = ctl.drive[0].sectors_per_revolution;   /* 29 (count 3) */
    EXPECT(spr > 0, "sectors_per_rev=%d", spr);
    uint64_t cps = DORADO_DISK_CYCLES_PER_REV / (uint64_t)spr;

    /* First tick only arms the model (relative to 'now'); no pulse yet. */
    uint64_t now = 1000000;
    EXPECT(dorado_disk_controller_tick(&ctl, now) == 0, "first tick arms only");
    EXPECT(ctl.drive[0].cur_sector == 0, "no advance on arm");

    /* Before the first boundary: still no pulse. */
    EXPECT(dorado_disk_controller_tick(&ctl, now + cps - 1) == 0,
           "no pulse before boundary");

    /* At the boundary: exactly one sector pulse. */
    EXPECT(dorado_disk_controller_tick(&ctl, now + cps) == 1, "pulse at boundary");
    EXPECT(ctl.drive[0].cur_sector == 1, "cur_sector=%d", ctl.drive[0].cur_sector);
    EXPECT(ctl.sector_tw == 1, "sector_tw set");

    /* Run a full revolution worth of cycles; expect a wrap to index. */
    ctl.index_tw = 0;
    uint64_t end = now + (uint64_t)DORADO_DISK_CYCLES_PER_REV + cps;
    for (uint64_t c = now + cps + 1; c <= end; c++)
        dorado_disk_controller_tick(&ctl, c);
    EXPECT(ctl.index_tw == 1, "index_tw should fire within one revolution");
    EXPECT(ctl.timing_advances >= (uint64_t)spr,
           "timing_advances=%llu (>= %d)",
           (unsigned long long)ctl.timing_advances, spr);

    /* PDI media must NOT be clock-driven. */
    static dorado_disk_controller pctl;
    dorado_disk_controller_init(&pctl);
    dorado_pdi pdi;
    memset(&pdi, 0, sizeof pdi);
    pdi.version = 1; pdi.fs_family = DORADO_PDI_FS_PILOT;
    pdi.page_count = 8;
    pdi.label_words = DORADO_PILOT_LABEL_WORDS;
    pdi.data_words = DORADO_PILOT_DATA_WORDS;
    pdi.labels = calloc((size_t)pdi.page_count * pdi.label_words, 2);
    pdi.data = calloc((size_t)pdi.page_count * pdi.data_words, 2);
    EXPECT(pdi.labels && pdi.data, "alloc pdi");
    dorado_disk_controller_attach_pdi(&pctl, 0, &pdi);
    for (uint64_t c = 0; c < 2 * DORADO_DISK_CYCLES_PER_REV; c += 137)
        EXPECT(dorado_disk_controller_tick(&pctl, c) == 0,
               "PDI media must not clock-tick");
    EXPECT(pctl.timing_advances == 0, "PDI timing_advances=%llu",
           (unsigned long long)pctl.timing_advances);
    free(pdi.labels); free(pdi.data);

    dorado_disk_pack_free(&pack);
    printf("PASS  test_clock_timing (3600 RPM sector/index pulses, "
           "PDI idle)\n");
    return 0;
}

/* test_fire_code_ecc — the Fire Code ECC generates a stable 2-word check,
 * verifies clean data, and flags a single-bit corruption. */
static int test_fire_code_ecc(void)
{
    /* A representative data block (use the header+label sizes). */
    uint16_t data[1024];
    for (int i = 0; i < 1024; i++) data[i] = (uint16_t)(0x1357u * (i + 1));

    uint16_t hi = 0, lo = 0;
    dorado_disk_ecc_compute(data, 1024, &hi, &lo);

    /* Stable: recomputation matches. */
    uint16_t hi2 = 0, lo2 = 0;
    dorado_disk_ecc_compute(data, 1024, &hi2, &lo2);
    EXPECT(hi == hi2 && lo == lo2, "ECC not stable: 0x%04X%04X vs 0x%04X%04X",
           hi, lo, hi2, lo2);

    /* Clean data checks OK (0). */
    EXPECT(dorado_disk_ecc_check(data, 1024, hi, lo) == 0,
           "clean data should check OK");

    /* A non-trivial block produces non-zero ECC (vanishingly unlikely 0). */
    EXPECT(!(hi == 0 && lo == 0), "ECC should be non-zero for this block");

    /* Single-bit corruption is detected. */
    data[500] ^= 0x0040u;
    EXPECT(dorado_disk_ecc_check(data, 1024, hi, lo) == 1,
           "1-bit corruption should flag an error");
    data[500] ^= 0x0040u;   /* restore */

    /* A different block gives a different ECC. */
    uint16_t hdr[2] = { 0x0001, 0x0002 };
    uint16_t ghi = 0, glo = 0;
    dorado_disk_ecc_compute(hdr, 2, &ghi, &glo);
    EXPECT(!(ghi == hi && glo == lo), "distinct blocks should differ in ECC");
    EXPECT(dorado_disk_ecc_check(hdr, 2, ghi, glo) == 0, "header ECC OK");

    printf("PASS  test_fire_code_ecc (generate/check/corruption, "
           "ecc=0x%04X%04X)\n", hi, lo);
    return 0;
}

/* test_write_path — D5: write a page through the controller, read it back,
 * exercise the FIFO write-stream, persist via save/reload, and reject writes
 * to read-only media. */
static int test_write_path(void)
{
    static dorado_io io;
    dorado_io_init(&io);
    static dorado_disk_controller ctl;
    dorado_disk_controller_init(&ctl);
    dorado_disk_controller_attach_to_io(&ctl, &io);

    static dorado_disk_pack pack;
    dorado_disk_geometry geom = { 2, 1, 9, 0, 0, 0 };   /* small, 9 sectors/track */
    EXPECT(dorado_disk_pack_create(&pack, &geom) == 0, "create pack");
    snprintf(pack.path, sizeof pack.path, "/tmp/test_dorado_write.pack");
    dorado_disk_controller_attach_drive(&ctl, 0, &pack);

    /* 1) write_page round-trip via the controller. */
    uint16_t wl[DORADO_DISK_LABEL_WORDS], wd[DORADO_DISK_DATA_WORDS];
    for (int i = 0; i < DORADO_DISK_LABEL_WORDS; i++) wl[i] = (uint16_t)(0xBE00 + i);
    for (int i = 0; i < DORADO_DISK_DATA_WORDS; i++)  wd[i] = (uint16_t)(0x1000 + i);
    uint32_t page = 5;
    EXPECT(dorado_disk_controller_write_page(&ctl, page, wl,
            DORADO_DISK_LABEL_WORDS, wd, DORADO_DISK_DATA_WORDS) == 0,
           "write_page");

    uint16_t rl[DORADO_DISK_LABEL_WORDS], rd[DORADO_DISK_DATA_WORDS];
    EXPECT(dorado_disk_controller_read_page(&ctl, page, rl,
            DORADO_DISK_LABEL_WORDS, rd, DORADO_DISK_DATA_WORDS) == 0,
           "read_page");
    for (int i = 0; i < DORADO_DISK_LABEL_WORDS; i++)
        EXPECT(rl[i] == wl[i], "label[%d] round-trip 0x%X != 0x%X", i, rl[i], wl[i]);
    for (int i = 0; i < DORADO_DISK_DATA_WORDS; i++)
        EXPECT(rd[i] == wd[i], "data[%d] round-trip 0x%X != 0x%X", i, rd[i], wd[i]);

    /* 2) FIFO write-stream: Control Tag Write, pump words via DiskData. */
    ctl.drive[0].cur_cyl = 0; ctl.drive[0].cur_head = 0; ctl.drive[0].cur_sector = 3;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)(TAG_CONTROL | (1u << 7)));   /* Write */
    EXPECT(ctl.write_stream_active == 1, "write stream should start");
    int total = DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS +
                DORADO_DISK_DATA_WORDS;
    for (int i = 0; i < total; i++)
        dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA,
                        (uint16_t)(0x4000 + i));
    EXPECT(ctl.write_stream_active == 0, "write stream should end at sector");
    dorado_disk_sector *ws = dorado_disk_pack_sector(&pack, 0, 0, 3);
    EXPECT(ws != NULL, "sector (0,0,3)");
    EXPECT(ws->label[0] == (uint16_t)(0x4000 + DORADO_DISK_HEADER_WORDS),
           "fifo write label[0]=0x%X", ws->label[0]);
    EXPECT(ws->data[0] == (uint16_t)(0x4000 + DORADO_DISK_HEADER_WORDS +
                                     DORADO_DISK_LABEL_WORDS),
           "fifo write data[0]=0x%X", ws->data[0]);

    /* 3) Persist: save + reload. */
    EXPECT(dorado_disk_pack_save(&pack) == 0, "save");
    dorado_disk_pack p2;
    EXPECT(dorado_disk_pack_load(&p2, &geom, "/tmp/test_dorado_write.pack") == 0,
           "reload");
    dorado_disk_sector *ps = dorado_disk_pack_sector(&p2, 0, 0, 5);
    EXPECT(ps && ps->data[0] == wd[0] &&
           ps->data[DORADO_DISK_DATA_WORDS - 1] == wd[DORADO_DISK_DATA_WORDS - 1],
           "page 5 persisted across save/reload");
    dorado_disk_pack_free(&p2);

    /* 4) Read-only media rejects writes. */
    pack.read_only = 1; ctl.drive[0].read_only = 1;
    EXPECT(dorado_disk_controller_write_page(&ctl, page, wl,
            DORADO_DISK_LABEL_WORDS, wd, DORADO_DISK_DATA_WORDS) == -1,
           "read-only pack must reject write_page");

    dorado_disk_pack_free(&pack);
    remove("/tmp/test_dorado_write.pack");
    printf("PASS  test_write_path (write_page+FIFO stream round-trip, "
           "save/reload, read-only reject)\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_pack_create_t80();
    rc |= test_fire_code_ecc();
    rc |= test_write_path();
    rc |= test_pack_save_load();
    rc |= test_controller_io_routing();
    rc |= test_diskcontrol_active_abort();
    rc |= test_rd_fifo_muffler_does_not_start_transfer();
    rc |= test_read_check_wakeup_and_error_latches();
    rc |= test_mixed_read_write_retargets_latched_sector();
    rc |= test_restore_header_check_bit();
    rc |= test_drive_attach();
    rc |= test_dmux_muffler_read();
    rc |= test_pdi_fifo_read();
    rc |= test_tag_decoder();
    rc |= test_advance_sector();
    rc |= test_block_till_index();
    rc |= test_drive_select_subsector_count();
    rc |= test_clock_timing();
    if (rc == 0) printf("\nAll disk tests passed.\n");
    return rc;
}
