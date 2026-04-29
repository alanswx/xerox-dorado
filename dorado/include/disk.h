#ifndef DORADO_DISK_H_
#define DORADO_DISK_H_

#include <stddef.h>
#include <stdint.h>

#include "io.h"

/*
 * Dorado Disk Controller (HM §9) — Trident T-80 / T-300 SMD.
 *
 * Architecture (port from ContrAlto2 with Dorado-specific TIOA wiring):
 *
 *   ┌────────────────────────────────────────────────────────────┐
 *   │  Dorado microcode (DSK task 14₈)                            │
 *   │     Output←B / Pd←Input on TIOA 10₈-14₈ (slow IO)           │
 *   └─────┬────────────────────────────────────────────┬─────────┘
 *         │ slow IO                                    │ fast IO
 *         ▼                                            ▼
 *   ┌──────────────────┐                       ┌──────────────────┐
 *   │  DiskController  │  16-word FIFO         │  Fast-IO transp  │
 *   │  • DiskControl   ├──────────────────────►│  Fin/Fout munch  │
 *   │  • DiskMuff      │                       └────────┬─────────┘
 *   │  • DiskData      │                                │
 *   │  • DiskRam       │                                ▼
 *   │  • DiskTag       │                       ┌──────────────────┐
 *   │  • Format RAM    │                       │  main storage    │
 *   │  • Sequence PROMs│                       └──────────────────┘
 *   │  • Tag register  │
 *   └──────┬───────────┘
 *          │ daisy chain (16 control tags + 9 status signals)
 *          │ + radial cable per drive
 *          ▼
 *   ┌──────────────────────────────────────────────────────────────┐
 *   │  TridentDrive[0..3]  — Century Data Trident SMD               │
 *   │     T-80:  815 cyl × 5 heads × 9 sectors × 2048 words/sector  │
 *   │     T-300: 815 cyl × 19 heads × 9 sectors × 2048 words/sector │
 *   │     pack image stored as one file: header(4w)+label(20w)+     │
 *   │     data(2048w)+dummy(1w) per sector, in CHS order            │
 *   └──────────────────────────────────────────────────────────────┘
 *
 * Phase 1 (this header):
 *   • Pack image read/write (Trident format from ContrAlto2)
 *   • Drive struct: cylinder/head/sector, ready/online/select state
 *   • Controller: TIOA 10₈-14₈ slow-IO catch-all registers; logs
 *     writes, accumulates state into the format RAM and tag register
 *   • DSK task wakeup stub (we don't drive it from a real timing
 *     model yet — synthetic tests poke wakeups)
 *
 * Phase 2 (later):
 *   • Sequence PROM execution (read/write tracks per format RAM)
 *   • Fast-IO FIFO transport into main storage
 *   • Fire Code ECC: P(X) = X^32 + X^23 + X^21 + X^11 + X^2 + 1
 *   • Realistic sector-pulse timing → wakeup generation
 */

/* ─── Geometry ──────────────────────────────────────────────────── */

#define DORADO_DISK_HEADER_WORDS   4    /* HM page 94 */
#define DORADO_DISK_LABEL_WORDS    20
#define DORADO_DISK_DATA_WORDS     2048 /* T-80 / T-300 1024-word
                                         * sectors with Alto Trident
                                         * format = 9 sectors/rev. */
#define DORADO_DISK_DUMMY_WORDS    1    /* Bitsavers/ContrAlto extra */
#define DORADO_DISK_SECTOR_WORDS \
    (DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS + \
     DORADO_DISK_DATA_WORDS + DORADO_DISK_DUMMY_WORDS)
#define DORADO_DISK_SECTOR_BYTES   (DORADO_DISK_SECTOR_WORDS * 2)

typedef struct {
    int cylinders;
    int heads;
    int sectors;            /* sectors per track */
} dorado_disk_geometry;

/* Standard Trident geometries from HM §9 / ContrAlto2. */
extern const dorado_disk_geometry DORADO_DISK_T80;
extern const dorado_disk_geometry DORADO_DISK_T300;

/* ─── Disk pack ─────────────────────────────────────────────────── */

typedef struct {
    uint16_t header[DORADO_DISK_HEADER_WORDS];
    uint16_t label [DORADO_DISK_LABEL_WORDS];
    uint16_t data  [DORADO_DISK_DATA_WORDS];
    uint8_t  modified;
} dorado_disk_sector;

typedef struct {
    dorado_disk_geometry geometry;
    /* Flat sector array in CHS order:
     *   index = ((cyl * heads + head) * sectors + sec) */
    dorado_disk_sector  *sectors;
    int                  num_sectors;
    char                 path[256];     /* source file path, "" if synthetic */
    int                  read_only;
} dorado_disk_pack;

/* Create an empty pack of the given geometry. */
int dorado_disk_pack_create(dorado_disk_pack *pack,
                            const dorado_disk_geometry *geom);

/* Load a pack from a file in ContrAlto2 / Bitsavers format. */
int dorado_disk_pack_load(dorado_disk_pack *pack,
                          const dorado_disk_geometry *geom,
                          const char *path);

/* Save a pack back to its source file (path must be set). */
int dorado_disk_pack_save(const dorado_disk_pack *pack);

void dorado_disk_pack_free(dorado_disk_pack *pack);

/* Sector accessors. Returns NULL if (c,h,s) out of range. */
dorado_disk_sector *dorado_disk_pack_sector(dorado_disk_pack *pack,
                                            int cyl, int head, int sec);

/* ─── Drive ─────────────────────────────────────────────────────── */

typedef struct {
    dorado_disk_pack *pack;          /* loaded pack, or NULL = no media */
    int               online;        /* drive is spun up + Ready */
    int               read_only;
    int               selected;      /* this drive is currently selected */

    /* Head position. Updated by Tag commands (CYL, HEAD, SEEK) issued
     * by the controller. The drive exposes status flags (SeekInc',
     * HeadOvfl, NotOnLine, NotReady, Index') back to the controller. */
    int  cur_cyl;
    int  cur_head;
    int  cur_sector;

    int  seek_in_progress;
    int  index_pulse;               /* set briefly on each rev */
} dorado_disk_drive;

void dorado_disk_drive_init(dorado_disk_drive *drv);
void dorado_disk_drive_attach_pack(dorado_disk_drive *drv,
                                   dorado_disk_pack *pack);

/* ─── Controller ────────────────────────────────────────────────── */

/* TIOA addresses (HM §9 page 92), all on task 14₈ (DSK). */
#define DORADO_DISK_TIOA_BASE         010
#define DORADO_DISK_TIOA_DISKCONTROL  010
#define DORADO_DISK_TIOA_DISKMUFF     011
#define DORADO_DISK_TIOA_DISKDATA     012
#define DORADO_DISK_TIOA_DISKRAM      013
#define DORADO_DISK_TIOA_DISKTAG      014
#define DORADO_DISK_TASK              014    /* DSK task number */

/* DiskControl bit positions (HM page 97), MSB-first manual numbering
 * (B[5..15] in microcode). Out C-LSB equivalent positions:
 *   B[5]   = bit 10  ClearEnableRun
 *   B[6]   = bit 9   SetDebugMode
 *   B[7]   = bit 8   SetBlockTillIndex
 *   B[8:9] = bits 7..6 Op for 1st block (0=Done, 1=Wr, 2=R+Chk, 3=Rd)
 *   B[10:11] = bits 5..4 Op for 2nd block
 *   B[12:13] = bits 3..2 Op for 3rd block
 *   B[14:15] = bits 1..0 Op for 4th block */
#define DORADO_DISK_CTRL_CLR_ENABLE_RUN   (1u << 10)
#define DORADO_DISK_CTRL_SET_DEBUG_MODE   (1u << 9)
#define DORADO_DISK_CTRL_BLOCK_TILL_INDEX (1u << 8)
#define DORADO_DISK_CTRL_OP1_SHIFT        6
#define DORADO_DISK_CTRL_OP2_SHIFT        4
#define DORADO_DISK_CTRL_OP3_SHIFT        2
#define DORADO_DISK_CTRL_OP4_SHIFT        0
#define DORADO_DISK_CTRL_OP_MASK          0x3u
#define DORADO_DISK_OP_DONE  0
#define DORADO_DISK_OP_WRITE 1
#define DORADO_DISK_OP_RDCHK 2
#define DORADO_DISK_OP_READ  3

#define DORADO_DISK_FORMAT_RAM_WORDS  16
#define DORADO_DISK_FIFO_WORDS        16
#define DORADO_DISK_NUM_DRIVES         4

typedef struct {
    /* Slow-IO state (HM §9). */
    uint16_t control;                /* DiskControl bit field */
    uint8_t  enable_run;             /* set by completing Format RAM load */
    uint8_t  debug_mode;             /* DebugMode flipflop */
    uint8_t  block_till_index;       /* BlockTillIndex flipflop */
    uint8_t  active;                 /* "Active" flipflop — sector
                                      * transfer in progress */

    uint16_t format_ram[DORADO_DISK_FORMAT_RAM_WORDS];
    int      format_ram_addr;        /* auto-incrementing RAM addr,
                                      * zeroed by writing DiskControl */

    uint16_t tag;                    /* last Tag command */
    uint16_t fifo[DORADO_DISK_FIFO_WORDS];
    int      fifo_head, fifo_tail;
    int      fifo_count;

    /* Drives. Drive 0 is the boot drive on real hardware. */
    dorado_disk_drive drive[DORADO_DISK_NUM_DRIVES];
    int      selected_drive;         /* index into drive[] */

    /* Wakeup conditions (HM page 95–97). The microcode reads these
     * via the muffler system (DiskMuff input) to determine why it
     * was woken. */
    uint8_t  index_tw;               /* index pulse since last clear */
    uint8_t  sector_tw;              /* sector pulse since last clear */
    uint8_t  tag_tw;                 /* tag command completed */
    uint8_t  rd_fifo_tw;             /* read FIFO has data */
    uint8_t  wr_fifo_tw;             /* write FIFO has space */

    /* Diagnostic counters. */
    uint64_t output_count;
    uint64_t input_count;
    uint64_t format_ram_writes;
    uint64_t tag_writes;
    uint64_t fifo_writes;
    uint64_t fifo_reads;
} dorado_disk_controller;

void dorado_disk_controller_init(dorado_disk_controller *ctl);
void dorado_disk_controller_attach_to_io(dorado_disk_controller *ctl,
                                         dorado_io *io);
void dorado_disk_controller_attach_drive(dorado_disk_controller *ctl,
                                         int slot, dorado_disk_pack *pack);

/* Advance the selected drive's sector counter (= simulated subsector
 * pulse arrival) and load the next sector's header+label+data into
 * the FIFO if the controller is in a read state. Synthetic helper —
 * real hardware sequences this from the format-RAM sequence PROM. */
void dorado_disk_controller_advance_sector(dorado_disk_controller *ctl);

#endif
