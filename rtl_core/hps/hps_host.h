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
    void (*wait)(void *ctx);      // called while waiting on the FPGA
    void *ctx;
};

struct hps_config {
    const char *snapshot;         // paths are relative to the dorado/ directory
    const char *pdi;
    const char *ftp_root;
    const char *eb;
    const char *germ;
    unsigned    quantum;          // microinstructions per host step
    uint64_t    germ_poll;        // BaseBoard cycles between germ IOCB polls
    int         keyboard;         // take the keyboard and mouse from the status block
};

struct hps_stats {
    uint64_t ops, reads, stores, rm_or, resched, rx_packets, tx_packets, tx_unsupported, unported;
    uint64_t rtl_cycles, host_bb_cycles, host_target;   // host_target: where the FPGA is, in BaseBoard cycles
    uint64_t rtl_clocks;              // fabric clocks (status beat 8, top 24 bits, unwrapped)
    uint32_t last_clk24;
};

// Restore the snapshot, load storage, start the core. 0 on success.
int  hps_start(const hps_config *cfg, const hps_transport *t);
// One pass: catch the host up with the FPGA's cycle count. Returns the
// FPGA's microinstruction count.
uint64_t hps_poll(void);
// Render the screen the way the C emulator does (dorado_machine_render_
// display_list, reading through the mailbox) to a PGM file; pixels set.
int  hps_snapshot_pgm(const char *path);
const hps_stats *hps_get_stats(void);
// The C machine (for a harness that drives input), as a dorado_machine *.
void *hps_machine(void);
