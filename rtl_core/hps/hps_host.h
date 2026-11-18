// =============================================================================
// hps_host.h — The C host services for the FPGA Dorado, over the DDR mailbox
// =============================================================================
// The same code runs on the MiSTer's ARM (hps_main.cpp: DDR through /dev/mem)
// and in the Verilator rehearsal (sim_hps.cpp: the DDR model's array). See
// dorado_rtl_mailbox.sv for the layout and docs/clean-rtl-handoff.md 8.3.
// =============================================================================
#pragma once
#include <cstddef>
#include <cstdint>

struct hps_transport {
    volatile uint64_t *mb;        // the mailbox, beat 0 = control
    volatile uint16_t *storage;   // storage word 0
    size_t             storage_words;
    volatile uint64_t *pack;      // the Trident pack image (dorado_rtl_io's sector engine), or null
    size_t             pack_beats;
    void (*wait)(void *ctx);      // called while waiting on the FPGA
    void *ctx;
};

struct hps_config {
    const char *snapshot;         // paths are relative to the dorado/ directory
    const char *pdi;              // Pilot PDI (Cedar), or null
    const char *pack;             // Trident pack on drive 0 (an Alto-world disk: Smalltalk), or null
    const char *storage_image;    // raw storage words to load instead of the snapshot's (the
                                  // native run the core's init came from), or null
    const char *ftp_root;
    const char *eb;
    const char *germ;
    unsigned    quantum;          // microinstructions per host step
    uint64_t    germ_poll;        // BaseBoard cycles between germ IOCB polls
    unsigned    world_cfg;        // the world's presentation, for a scan core (mailbox control beat 1)
    int         keyboard;         // take the keyboard and mouse from the status block
    int         derive;           // a snapshot with no .regs/.rams beside it: make them from the snapshot (a scan core)
    int         cold;             // ...as a cold start (the machine at the LoadRam handoff, or as the germ was planted)
    int         need_core;        // hps_start returns 3, having written nothing else, if no core answers the restart
};

struct hps_stats {
    uint64_t ops, reads, stores, rm_or, resched, rx_packets, tx_packets, tx_unsupported, unported;
    uint64_t rtl_cycles, host_bb_cycles, host_target;   // host_target: where the FPGA is, in BaseBoard cycles
    uint64_t rtl_clocks;              // fabric clocks (status beat 8, top 24 bits, unwrapped)
    uint64_t status_reads;            // read_status calls (the unwrap needs one per 0.3 s at least)
    uint64_t rtl_clocks64;            // the whole fabric clock count (status beat 12)
    uint64_t status_rounds;           // status rounds (one per 4096 clocks: an independent clock measure)
    uint16_t last_status_n;
    uint64_t st_cnt[16];              // clocks by sequencer state (status beats 16-31)
    uint32_t last_clk24;
    uint64_t ref_clocks;              // CLK_50M edges (status beat 13, unwrapped): the fabric clock's reference
    uint32_t last_ref32;
    int      pll_locked;              // the core PLL's lock (beat 13 bit 32)
    // drop accounting (each should stay 0 but nww_posts)
    uint64_t nww_posts;               // NWW edges sent (AP_RM_OR to RM[0])
    uint64_t result_tears;            // result beats read half old, half new (re-read)
    uint64_t rx_lost;                 // receive fills sent and not applied (reports)
    uint16_t rx_applied;              // the receiver's count of fills applied (status beat 11)
    double   t_status;                // CLOCK_MONOTONIC at the last status read (rates are per read interval, not per report)
};

// Restore the snapshot, load storage, start the core. 0 on success.
int  hps_start(const hps_config *cfg, const hps_transport *t);
// The same over a running core, in this process (the simulation's rehearsal
// of starting the program again on the board).
int  hps_restart(const hps_config *cfg);
// One pass: catch the host up with the FPGA's cycle count. Returns the
// FPGA's microinstruction count.
uint64_t hps_poll(void);
int hps_save(const char *path);         // the running machine as a snapshot: <path>, <path>.regs, <path>.rams (a scan core)
void     hps_sync_rm(unsigned n);
// Render the screen the way the C emulator does (dorado_machine_render_
// display_list, reading through the mailbox) to a PGM file; pixels set.
int  hps_snapshot_pgm(const char *path);
const hps_stats *hps_get_stats(void);
// The C machine (for a harness that drives input), as a dorado_machine *.
void *hps_machine(void);
