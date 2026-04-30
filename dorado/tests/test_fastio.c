#include "fastio.h"
#include "memory.h"
#include "display.h"
#include "disk.h"

#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

/*
 * test_dsk_iostore_to_memory — disk pack → controller FIFO →
 * IOStore(DSK) → main memory.
 *
 * Setup: stamp recognizable bytes in a sector. Issue a disk Read tag
 * (= load FIFO). Then call dorado_memory_ref_task with IOStore from
 * task DSK; the dispatcher should drain the FIFO into the munch and
 * memory writes it to storage.
 */
static int test_dsk_iostore_to_memory(void)
{
    static dorado_memory mem;
    static dorado_display display;
    static dorado_disk_controller disk;
    static dorado_disk_pack pack;

    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_display_init(&display);
    dorado_disk_controller_init(&disk);
    EXPECT(dorado_disk_pack_create(&pack, &DORADO_DISK_T80) == 0, "pack");

    /* Stamp sector (5, 2, 3) with recognizable data: each word = its
     * index OR'd with 0x4000. */
    dorado_disk_sector *sec = dorado_disk_pack_sector(&pack, 5, 2, 3);
    EXPECT(sec != NULL, "sector pointer");
    sec->header[0] = 0xFEED;
    sec->header[1] = 0xBEEF;
    for (int w = 0; w < DORADO_DISK_LABEL_WORDS; w++) {
        sec->label[w] = (uint16_t)(0x2000 | w);
    }
    for (int w = 0; w < 32; w++) {
        sec->data[w] = (uint16_t)(0x4000 | w);
    }
    dorado_disk_controller_attach_drive(&disk, 0, &pack);

    static dorado_fastio_router router;
    dorado_fastio_router_init(&router, &display, &disk);
    mem.fast_io_cb  = dorado_fastio_dispatch;
    mem.fast_io_ctx = &router;

    /* Map the touched page RW and identity-mapped. */
    dorado_map_set(&mem, dorado_map_index(0x200), /*rp=*/2, /*wp=*/0, /*dirty=*/0);

    /* Drive the disk: select drive 0, seek (5, 2, 3), Read. */
    static dorado_io io;
    dorado_io_init(&io);
    dorado_disk_controller_attach_to_io(&disk, &io);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG, 0);
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((2u << 12) | 5u));   /* Cyl 5 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((1u << 12) | 2u));   /* Head 2 */
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 6))); /* Read */
    /* But we need to set cur_sector = 3 first. Easiest: advance from 0. */
    /* The Read above loaded sector (5,2,0) into FIFO. Drain it and
     * advance to sector 3 to load (5,2,3). */
    int bad = -1;
    while (disk.fifo_count > 0) {
        dorado_io_read(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKDATA, &bad);
    }
    /* Re-issue Read with cur_sector=3 (manually set since we don't
     * have a sector-pulse driver yet). */
    disk.drive[0].cur_sector = 3;
    dorado_io_write(&io, DORADO_DISK_TASK, DORADO_DISK_TIOA_DISKTAG,
                    (uint16_t)((3u << 12) | (1u << 6))); /* Read */
    EXPECT(disk.fifo_count == DORADO_DISK_FIFO_WORDS,
           "FIFO loaded with sector data, count=%d", disk.fifo_count);

    /* Now do IOStore(DSK) at VA = 0o1000 (= 512 dec). The dispatcher
     * pulls 16 words from the disk FIFO into memory[VA..VA+15]. */
    uint32_t va = 0x200;        /* aligned to 16-word boundary */
    dorado_fault_kind f = dorado_memory_ref_task(
        &mem, DM_REF_IOSTORE, va, /*b=*/0, /*tioa=*/0,
        DORADO_DISK_TASK, /*subtask=*/0);
    EXPECT(f == DM_FAULT_NONE, "IOStore should succeed (fault=%d)", f);

    /* Verify mem[0x200..0x20F] now contains the FIFO contents (=
     * sector header + label + start of data). The first 2 words are
     * the Trident header, the next 10 are the label. */
    EXPECT(mem.storage[0x200] == 0xFEED,
           "storage[0x200] = 0x%X (expected 0xFEED)", mem.storage[0x200]);
    EXPECT(mem.storage[0x201] == 0xBEEF,
           "storage[0x201] = 0x%X (expected 0xBEEF)", mem.storage[0x201]);
    EXPECT(mem.storage[0x202] == 0x2000,
           "storage[0x202] = 0x%X (expected 0x2000 = label[0])",
           mem.storage[0x202]);
    EXPECT(mem.storage[0x20B] == 0x2009,
           "storage[0x20B] = 0x%X (expected 0x2009 = label[9])",
           mem.storage[0x20B]);
    EXPECT(mem.storage[0x20C] == 0x4000,
           "storage[0x20C] = 0x%X (expected 0x4000 = data[0])",
           mem.storage[0x20C]);
    EXPECT(mem.storage[0x20F] == 0x4003,
           "storage[0x20F] = 0x%X (expected 0x4003 = data[3])",
           mem.storage[0x20F]);

    /* The router drained one 16-word munch from the controller. A
     * sector stream may immediately refill the FIFO for the next
     * munch, so assert the transfer counter rather than empty state. */
    EXPECT(disk.fifo_reads >= DORADO_DISK_FIFO_WORDS,
           "FIFO reads = %llu (expected at least %d)",
           (unsigned long long)disk.fifo_reads, DORADO_DISK_FIFO_WORDS);

    dorado_disk_pack_free(&pack);
    dorado_memory_free(&mem);
    printf("PASS  test_dsk_iostore_to_memory (sector → FIFO → memory)\n");
    return 0;
}

/*
 * test_dwt_iofetch_to_display — main memory → IOFetch(DWT) → display
 * FIFO → framebuffer.
 *
 * Setup: plant pixel data in memory. Do IOFetch as DWT subtask 0. The
 * dispatcher pushes 16 words into display.fifo_a. Then render the
 * FIFO and verify pixels appear in the framebuffer.
 */
static int test_dwt_iofetch_to_display(void)
{
    static dorado_memory mem;
    static dorado_display display;
    static dorado_disk_controller disk;

    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_display_init(&display);
    dorado_disk_controller_init(&disk);

    /* Plant 16 words of pixel data at VA 0x100 = phys 0x100. The
     * pattern: alternating 0xAAAA / 0x5555. */
    dorado_map_set(&mem, dorado_map_index(0x100), /*rp=*/1, /*wp=*/0, /*dirty=*/0);
    for (int i = 0; i < 16; i++) {
        mem.storage[0x100 + i] = (i & 1) ? 0x5555 : 0xAAAA;
    }

    static dorado_fastio_router router;
    dorado_fastio_router_init(&router, &display, &disk);
    mem.fast_io_cb  = dorado_fastio_dispatch;
    mem.fast_io_ctx = &router;

    /* IOFetch(DWT, subtask=0). Dispatcher reads 16 words from
     * mem[0x100..0x10F] and pushes into display.fifo_a. */
    dorado_fault_kind f = dorado_memory_ref_task(
        &mem, DM_REF_IOFETCH, 0x100, 0, 0,
        /*task=*/013 /*DWT*/, /*subtask=*/0);
    EXPECT(f == DM_FAULT_NONE, "IOFetch should succeed (fault=%d)", f);
    /* FIFO should have 16 words. */
    int expected_count = 16;
    int actual_count = (display.fifo_a_head - display.fifo_a_tail + 256) % 256;
    EXPECT(actual_count == expected_count,
           "display FIFO A has %d words (expected %d)",
           actual_count, expected_count);
    EXPECT(display.fifo_a[0] == 0xAAAA, "fifo_a[0] = 0x%X",
           display.fifo_a[0]);
    EXPECT(display.fifo_a[1] == 0x5555, "fifo_a[1] = 0x%X",
           display.fifo_a[1]);

    /* Render the FIFO into the framebuffer. */
    int dst_y = 0;
    int rendered = dorado_display_render_fifo(&display, /*subtask=*/0,
                                              &dst_y);
    EXPECT(rendered == 16 * 16,
           "rendered %d pixels (expected %d)", rendered, 16 * 16);
    /* fb[0] should be 0xAA (first 8 pixels of word 0xAAAA). */
    EXPECT(display.fb[0] == 0xAA,
           "fb[0] = 0x%02X (expected 0xAA from 0xAAAA word 0)",
           display.fb[0]);
    /* fb[2] = high 8 bits of 0x5555 = 0x55. */
    EXPECT(display.fb[2] == 0x55,
           "fb[2] = 0x%02X (expected 0x55 from 0x5555 word 1)",
           display.fb[2]);

    dorado_memory_free(&mem);
    printf("PASS  test_dwt_iofetch_to_display (memory → display FIFO → FB)\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_dsk_iostore_to_memory();
    rc |= test_dwt_iofetch_to_display();
    if (rc == 0) printf("\nAll fastio tests passed.\n");
    return rc;
}
