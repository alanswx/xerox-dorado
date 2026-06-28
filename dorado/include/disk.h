#ifndef DORADO_DISK_H_
#define DORADO_DISK_H_

#include <stddef.h>
#include <stdint.h>

#include "io.h"

struct dorado_pdi;

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
 *   │     T-80:  815 cyl × 5 heads × 9 sectors × 1024 words/sector  │
 *   │     T-300: 815 cyl × 19 heads × 9 sectors × 1024 words/sector │
 *   │     pack image stored as one file: header(2w)+label(10w)+     │
 *   │     data(1024w)+dummy(1w) per sector, in CHS order            │
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

#define DORADO_DISK_HEADER_WORDS   2    /* ContrAlto/Bitsavers Trident */
#define DORADO_DISK_LABEL_WORDS    10
#define DORADO_DISK_DATA_WORDS     1024 /* 2048-byte Alto Trident data */
#define DORADO_DISK_DUMMY_WORDS    1    /* Bitsavers/ContrAlto extra */
#define DORADO_DISK_SECTOR_WORDS \
    (DORADO_DISK_HEADER_WORDS + DORADO_DISK_LABEL_WORDS + \
     DORADO_DISK_DATA_WORDS + DORADO_DISK_DUMMY_WORDS)
#define DORADO_DISK_SECTOR_BYTES   (DORADO_DISK_SECTOR_WORDS * 2)

typedef struct {
    int cylinders;
    int heads;
    int sectors;            /* sectors per track */
    /* Per-sector word counts. 0 means "use the native-Trident defaults"
     * (DORADO_DISK_{HEADER,LABEL,DATA}_WORDS) so existing Trident/Pilot packs
     * and tests are unchanged; the Alto-Diablo-on-Trident format sets them
     * explicitly (2/8/256). The in-memory sector struct is always max-sized;
     * only the on-disk layout and the controller framing use these counts. */
    int header_words;
    int label_words;
    int data_words;
} dorado_disk_geometry;

/* Standard Trident geometries from HM §9 / ContrAlto2. */
extern const dorado_disk_geometry DORADO_DISK_T80;
extern const dorado_disk_geometry DORADO_DISK_T300;

/* Alto Diablo emulated on one Trident surface (AltoDiabloDisk.mc): the Trident
 * is low-level formatted as 29 short sectors/track of 2 header + 8 label + 256
 * data words over the full 815-cylinder T-80 range. AEmu maps emulated Diablo
 * drive 0 at cyl+3 and drive 1 at cyl+0406+3. */
extern const dorado_disk_geometry DORADO_DISK_DIABLO;

/* Effective per-sector word counts for a geometry (0 -> native defaults). */
int dorado_disk_geom_header_words(const dorado_disk_geometry *g);
int dorado_disk_geom_label_words(const dorado_disk_geometry *g);
int dorado_disk_geom_data_words(const dorado_disk_geometry *g);

/* ─── Fire Code ECC (HM §9.10) ──────────────────────────────────────
 * P(X) = X^32 + X^23 + X^21 + X^11 + X^2 + 1, a 32-stage LFSR. Generates the
 * 2-word check value for a block (header / label / data) and checks it.
 * Returns 0 = no error, 1 = mismatch. */
void dorado_disk_ecc_compute(const uint16_t *words, int n,
                             uint16_t *ecc_hi, uint16_t *ecc_lo);
int  dorado_disk_ecc_check(const uint16_t *words, int n,
                           uint16_t ecc_hi, uint16_t ecc_lo);

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
    const struct dorado_pdi *pdi;    /* Pilot/Cedar PDI media, or NULL */
    int               online;        /* drive is spun up + Ready */
    int               read_only;
    int               selected;      /* this drive is currently selected */

    /* Head position. Updated by Tag commands (CYL, HEAD, SEEK) issued
     * by the controller. The drive exposes status flags (SeekInc',
     * HeadOvfl, NotOnLine, NotReady, Index') back to the controller. */
    int  cur_cyl;
    int  cur_head;
    int  cur_sector;
    int  subsector_count;            /* Tag[4:9], sector pulse every N+1
                                      * of the 117 drive subsector pulses */
    int  sectors_per_revolution;     /* controller sector-pulse cadence;
                                      * distinct from pack image sectors */

    int  seek_in_progress;
    int  index_pulse;               /* set briefly on each rev */
    int  head_overflow;             /* invalid head selected */
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

/* Drive rotation: Trident spins at 3600 RPM = 16.67 ms/rev; at the 60 ns
 * Dorado microcycle that is ~277778 cycles/rev. Sector pulses divide this by
 * the drive's sectors-per-revolution (HM §9.2). Used by the clock-driven
 * timing model (dorado_disk_controller_tick). */
#define DORADO_DISK_CYCLES_PER_REV    277778u

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
    uint8_t  muff_addr;              /* DiskMuff-selected diagnostic signal */

    uint16_t tag;                    /* last Tag command */
    uint16_t fifo[DORADO_DISK_FIFO_WORDS];
    int      fifo_head, fifo_tail;
    int      fifo_count;
    uint8_t  read_stream_active;     /* current sector is streaming into FIFO */
    int      read_stream_index;      /* word index in header+label+data */
    const dorado_disk_sector *read_stream_sector; /* sector latched at
                                      * stream start. A read transfers ONE
                                      * sector's data field; the microcode drain
                                      * (Block per word + task switching) spans
                                      * many emulated sector pulses, so refills
                                      * must keep serving the sector the read
                                      * began on, not the drive's advancing
                                      * cur_sector (else the data block mixes
                                      * adjacent sectors). NULL for PDI media. */
    uint8_t  read_block_framing;     /* 1 = interleave per-block trailing words
                                      * (2 garbage + 2 ECC) after each of the
                                      * header/label/data blocks, as the real
                                      * controller does and the DSK-task disk
                                      * drivers (AltoDiabloDisk ReadECC) drain.
                                      * 0 = contiguous header+label+data, for the
                                      * Cedar germ read_page bridge shortcut. */
    uint8_t  write_stream_active;    /* a write op is committing FIFO->pack */
    uint8_t  write_block_framing;    /* 1 = DiskControl write blocks include a
                                      * leading sync word generated by the DSK
                                      * microcode before each block's data. */
    int      write_stream_index;     /* word index in header+label+data */
    dorado_disk_sector *write_stream_sector; /* sector latched at write start;
                                      * writes span many sector pulses just like
                                      * reads, so all blocks in one command must
                                      * commit to the sector selected when the
                                      * controller sequence began. */
    uint8_t  xfer_pending;           /* a DiskControl transfer command has been
                                      * loaded but not yet started. The read
                                      * stream auto-starts once (at the next
                                      * sector pulse), then clears this. Without
                                      * the one-shot, enable_run + a leftover
                                      * transfer op make advance_sector restart
                                      * the stream every sector, leaving the
                                      * controller spuriously Active so the next
                                      * DSK command Output aborts (AReadBadTW). */
    uint8_t  restore_pending;         /* a ReZero/restore tag was just issued */
    uint8_t  transfer_restore;        /* pending transfer follows restore */
    uint8_t  restore_header_check;    /* Diablo header check should tolerate
                                      * the restore bit in the address word */

    uint8_t  current_block;          /* active DiskControl block, 0..3 */
    uint8_t  current_block_op;       /* DORADO_DISK_OP_* for current_block */
    uint16_t current_block_words;    /* data words in current block */
    uint16_t current_block_pos;      /* words transferred within block */
    uint8_t  current_block_prefix;   /* leading sync word still pending */
    uint8_t  current_block_trailer;  /* 2 garbage + 2 ECC words after block */
    uint8_t  compare_err;            /* compare error latch, HM §9 */
    uint8_t  read_data_err;          /* ReadDataErr latch */
    uint8_t  fifo_underflow;         /* FIFO underflow latch */
    uint8_t  fifo_overflow;          /* FIFO overflow latch */

    /* Drives. Drive 0 is the boot drive on real hardware. */
    dorado_disk_drive drive[DORADO_DISK_NUM_DRIVES];
    int      selected_drive;         /* index into drive[] */

    /* Wakeup conditions (HM page 95–97). The microcode reads these
     * via the muffler system (DiskMuff input) to determine why it
     * was woken. */
    /* Clock-driven sector/index timing (dorado_disk_controller_tick).
     * next_sector_cycle == 0 means the model is unarmed. */
    uint64_t next_sector_cycle;
    uint64_t timing_advances;        /* diagnostic: clock-driven pulses */
    uint64_t write_sectors_committed; /* diagnostic: complete sectors written */
    uint8_t  allow_pdi_timing;       /* tick PDI media too (--disk-real, D4) */

    uint8_t  index_tw;               /* index pulse since last clear */
    uint8_t  sector_tw;              /* sector pulse since last clear */
    uint8_t  tag_tw;                 /* tag command completed */
    uint8_t  rd_fifo_tw;             /* read FIFO has data */
    uint8_t  wr_fifo_tw;             /* write FIFO has space */

    /* Diagnostic counters. */
    uint64_t output_count;
    uint64_t input_count;
    uint64_t output_tioa_count[16];
    uint64_t input_tioa_count[16];
    uint64_t format_ram_writes;
    uint64_t tag_writes;
    uint64_t fifo_writes;
    uint64_t fifo_reads;
    uint64_t read_stream_starts;
    uint64_t read_stream_start_failures;
    uint64_t read_stream_sector_starts;
    uint64_t read_stream_tag_starts;
    uint64_t read_stream_muff_starts;
    uint64_t control_loads;
    uint64_t control_transfer_loads;
    uint64_t control_abort_edges;
    uint64_t index_tw_sets;
    uint64_t index_tw_clears;
    uint64_t sector_tw_sets;
    uint64_t sector_tw_clears;
    uint64_t tag_tw_sets;
    uint64_t tag_tw_clears;
    uint8_t  last_output_tioa;
    uint16_t last_output_data;
    uint8_t  last_input_tioa;
    uint16_t last_input_data;
} dorado_disk_controller;

void dorado_disk_controller_init(dorado_disk_controller *ctl);
void dorado_disk_controller_attach_to_io(dorado_disk_controller *ctl,
                                         dorado_io *io);
void dorado_disk_controller_attach_drive(dorado_disk_controller *ctl,
                                         int slot, dorado_disk_pack *pack);
void dorado_disk_controller_attach_pdi(dorado_disk_controller *ctl,
                                       int slot,
                                       const struct dorado_pdi *pdi);

/* Refill the read FIFO from the current sector stream. Fast-IO drains
 * full munches directly, bypassing DiskData input, so the router calls
 * this after each pop. */
void dorado_disk_controller_refill_fifo(dorado_disk_controller *ctl);

/* True when a controller wakeup flip-flop should wake task DSK. */
int dorado_disk_controller_wakeup_pending(const dorado_disk_controller *ctl);

/* Advance the selected drive's sector counter (= simulated subsector
 * pulse arrival) and load the next sector's header+label+data into
 * the FIFO if the controller is in a read state. Synthetic helper —
 * real hardware sequences this from the format-RAM sequence PROM. */
void dorado_disk_controller_advance_sector(dorado_disk_controller *ctl);

/* Read one logical page through the real controller read path (positions the
 * selected drive, runs the read stream, and drains header+label+data out of
 * the FIFO). Fills label[0..label_n) and data[0..data_n). Returns 0 on a full
 * read, -1 otherwise. Used by the --disk-real boot bridge so Cedar's disk
 * reads flow through the controller (FIFO + framing) rather than a direct copy
 * (plan D4). */
int dorado_disk_controller_read_page(dorado_disk_controller *ctl,
                                     uint32_t page,
                                     uint16_t *label, int label_n,
                                     uint16_t *data, int data_n);

/* Write one logical page to a real Trident pack through the controller
 * (positions the selected drive, writes header/label/data to the sector, marks
 * it modified). Only writes a real pack (->pack), never read-only media; PDI is
 * read-only. Returns 0 on success, -1 otherwise. Plan D5 (write path, F3). */
int dorado_disk_controller_write_page(dorado_disk_controller *ctl,
                                      uint32_t page,
                                      const uint16_t *label, int label_n,
                                      const uint16_t *data, int data_n);

/* Clock-driven timing model: advance the selected drive's sector/index
 * pulses based on the elapsed cycle count (3600 RPM, see
 * DORADO_DISK_CYCLES_PER_REV). Call once per machine step with the current
 * cycle count. Only ticks for a real Trident pack (->pack); PDI media is
 * completed at the IOCB level by the machine shim and must not be perturbed.
 * Returns nonzero if at least one sector pulse fired this call. */
int dorado_disk_controller_tick(dorado_disk_controller *ctl,
                                uint64_t now_cycles);

/* Diagnostic DMux read for DiskHeadDorado's RWMufMan path. Handles
 * DMux addresses 02000 + DiskHeadDorado.MufflerAddress and returns the
 * selected bit in the sign position. */
uint16_t dorado_disk_controller_dmux_read(dorado_disk_controller *ctl,
                                          uint16_t addr, int *handled);

#endif
