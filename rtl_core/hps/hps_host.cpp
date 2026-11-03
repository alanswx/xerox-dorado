// =============================================================================
// hps_host.cpp — The C host services for the FPGA Dorado, over the DDR mailbox
// =============================================================================
// A whole C dorado_machine is restored from the same snapshot the FPGA image
// was built from (sim_cedar.cpp CEDAR_DUMP_INIT), but its CPU never runs: the
// step hook answers DORADO_STEP_HOOK_SKIP, so dorado_machine_run_until runs
// only its host services (germ disk and Ethernet bridges, Pilot timer,
// Cedar keyboard and field notify) and the Pup/STP server. Everything they
// do to the machine goes to the FPGA through the mailbox
// (dorado_rtl_mailbox.sv):
//   - guest-memory reads (dorado_host_read_hook): a blocking read command;
//   - guest-memory writes (dorado_host_write_hook): AP_STORE / AP_STORE_INV,
//     queued in order;
//   - NWW (RM[0]) and reschedule_pending: the host's changes, as AP_RM_OR and
//     AP_RESCHED. NWW is re-read from the FPGA each pass (AP_RM_OR with mask
//     0), since the Pilot timer ORs only when its bit is clear;
//   - each packet the server queues goes into the receive buffer area and an
//     RXLOAD command, once the FPGA has finished the last one and the
//     guest has read it (status: rx loads, rx_drained).
// The host follows the FPGA's microinstruction count (status beat 8) at 3.70
// BaseBoard cycles per microinstruction, in steps of cfg.quantum.
// =============================================================================
#include "hps_host.h"
#include <ctime>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
#include "cpu.h"
#include "memory.h"
#include "io.h"
#include "display.h"
#include "ethernet.h"
#include "machine.h"
}

namespace {

// Mailbox layout, in beats (dorado_rtl_mailbox.sv)
enum : size_t {
    B_CTRL = 0, B_STATUS = 8, B_CMD = 256, B_RES = 512, B_RXW = 1024, B_RXA = 2048,
    B_TXLEN = 2112, B_TX = 4096, B_END = 6144
};
const uint32_t MB_MAGIC = 0x444F5241u;
const uint64_t HOST_CHUNK_BB = 1000000;   // BaseBoard cycles: 270K microinstructions
enum { AP_READ = 0, AP_STORE = 1, AP_STORE_INV = 2, AP_RM_OR = 4, AP_RESCHED = 6, OP_RXLOAD = 7 };

hps_transport T;
hps_config    CFG;
hps_stats     S;
dorado_machine *M;
dorado_cpu     *C;
dorado_ethernet *ETH;

uint64_t issued, completed;    // commands
uint64_t cyc0, bb0;            // the FPGA's and the host's clocks at the start
uint16_t rm_prev[RM_SIZE];
uint16_t status_rxloads, my_rxloads;
bool     status_drained;
uint16_t status_tx, last_tx;
unsigned long shipped_gen;
bool     shipped;

inline void barrier() { __sync_synchronize(); }

uint8_t seq_of(uint64_t n) { return (uint8_t)((n / 256) % 255 + 1); }

// Retire finished commands; returns once fewer than `room` are outstanding.
void retire(uint64_t room) {
    for (;;) {
        while (completed < issued) {
            uint64_t r = T.mb[B_RES + (completed & 255)];
            if ((uint8_t)(r >> 56) != seq_of(completed)) break;
            completed++;
        }
        if (issued - completed < room) return;
        T.wait(T.ctx);
    }
}

uint64_t issue(int op, uint32_t va, uint16_t wd, int we, int cache) {
    retire(255);
    uint64_t n = issued;
    uint64_t e = ((uint64_t)seq_of(n) << 56) | ((uint64_t)(cache & 1) << 48) |
                 ((uint64_t)(we & 1) << 47) | ((uint64_t)(op & 7) << 44) |
                 ((uint64_t)(va & 0x0FFFFFFFu) << 16) | wd;
    volatile uint32_t *w = (volatile uint32_t *)&T.mb[B_CMD + (n & 255)];
    w[0] = (uint32_t)e;          // the half without seq first
    barrier();
    w[1] = (uint32_t)(e >> 32);
    barrier();
    issued++;
    S.ops++;
    return n;
}

uint16_t result(uint64_t n) {
    for (;;) {
        uint64_t r = T.mb[B_RES + (n & 255)];
        if ((uint8_t)(r >> 56) == seq_of(n)) {
            if (completed == n) completed++;
            return (uint16_t)r;
        }
        T.wait(T.ctx);
    }
}

uint16_t read_hook(const struct dorado_memory *, uint32_t va) {
    S.reads++;
    return result(issue(AP_READ, va, 0, 0, 1));
}

void write_hook(struct dorado_memory *, uint32_t va, uint16_t value, int kind) {
    if (kind == DORADO_HOST_WRITE_INVALIDATE) {
        issue(AP_STORE_INV, va, value, 1, 0);
    } else {
        if (kind != DORADO_HOST_WRITE_UPDATE) S.unported++;
        issue(AP_STORE, va, value, 1, 1);
    }
    S.stores++;
}

static double mono_s() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void read_status() {
    barrier();
    S.t_status = mono_s();
    uint64_t b8 = T.mb[B_STATUS], b9 = T.mb[B_STATUS + 1];
    uint64_t b10 = T.mb[B_STATUS + 2], b11 = T.mb[B_STATUS + 3];
    S.rtl_cycles = b8 & 0xFFFFFFFFFFull;
    S.status_reads++;
    S.rtl_clocks64 = T.mb[B_STATUS + 4];
    {
        uint64_t b13 = T.mb[B_STATUS + 5];
        uint32_t r = (uint32_t)b13;
        S.ref_clocks += (uint32_t)(r - S.last_ref32);
        S.last_ref32 = r;
        S.pll_locked = (int)((b13 >> 32) & 1);
    }
    {
        uint16_t n = (uint16_t)b10;
        S.status_rounds += (uint16_t)(n - S.last_status_n);
        S.last_status_n = n;
    }
    for (int i = 0; i < 16; i++) S.st_cnt[i] = T.mb[B_STATUS + 8 + i] & 0xFFFFFFFFFFull;
    {
        uint32_t c = (uint32_t)(b8 >> 40) & 0xFFFFFFu;
        S.rtl_clocks += (c - S.last_clk24) & 0xFFFFFFu;
        S.last_clk24 = c;
    }
    status_drained = (b10 >> 17) & 1;
    status_tx      = (uint16_t)(b11 >> 16);
    status_rxloads = (uint16_t)(b11 >> 32);
    // DORADO_HPS_KEYTRACE=1: the core's keyboard words and mouse beat, each
    // time they change (the input path from the MiSTer's devices)
    {
        static const bool kt = getenv("DORADO_HPS_KEYTRACE") != nullptr;
        static uint64_t k9 = ~0ull, k10 = 0;
        const uint64_t m10 = b10 & 0xFFFFFFFF03C00000ull;   // x, y, present, buttons
        if (kt && (b9 != k9 || m10 != k10)) {
            printf("[hps] input: kbd %016llx  mouse x %d y %d buttons %d present %d\n",
                   (unsigned long long)b9, (int)(int16_t)(b10 >> 48), (int)(int16_t)((b10 >> 32) & 0xFFFF),
                   (int)((b10 >> 22) & 7), (int)((b10 >> 25) & 1));
            k9 = b9; k10 = m10;
        }
    }
    if (CFG.keyboard) {
        dorado_display *d = dorado_machine_display(M);
        for (int i = 0; i < 4; i++)
            dorado_display_keyboard_set_word(d, i, (uint16_t)(b9 >> (16 * i)));
        // beat 10: {x[63:48], y[47:32], 6'0, present[25], buttons[24:22], 4'0, ...}
        int present = (b10 >> 25) & 1;
        if (present)
            dorado_machine_set_mouse(M, (int)(b10 >> 48), (int)((b10 >> 32) & 0xFFFF),
                                     (int)((b10 >> 22) & 7));
    }
}

void push_regs() {
    if (!C->reschedule_pending && memcmp(C->RM, rm_prev, sizeof rm_prev) == 0) return;
    for (int i = 0; i < RM_SIZE; i++) {
        uint16_t d = (uint16_t)(C->RM[i] & ~rm_prev[i]);
        if (rm_prev[i] & ~C->RM[i]) S.unported++;
        if (d) { issue(AP_RM_OR, (uint32_t)i, d, 1, 0); S.rm_or++; }
        rm_prev[i] = C->RM[i];
    }
    if (C->reschedule_pending) {
        issue(AP_RESCHED, 0, C->reschedule_pending, 1, 0);
        C->reschedule_pending = 0;
        S.resched++;
    }
}

void ethernet_bridge() {
    if (CFG.quantum > 1 && ETH->rx_hold)
        ETH->rx_hold = ETH->rx_hold > CFG.quantum - 1 ? ETH->rx_hold - (CFG.quantum - 1) : 0;
    bool fpga_done = status_rxloads == my_rxloads;   // the last fill is finished
    if (dorado_ethernet_rx_generation != shipped_gen && ETH->rx_count > 0 &&
        ETH->rx_hold == 0 && fpga_done) {
        size_t n = ETH->rx_count < 4096 ? ETH->rx_count : 4096;
        for (size_t b = 0; b < (n + 3) / 4; b++) {
            uint64_t v = 0;
            for (size_t k = 0; k < 4 && 4 * b + k < n; k++)
                v |= (uint64_t)ETH->rx_words[4 * b + k] << (16 * k);
            T.mb[B_RXW + b] = v;
        }
        for (size_t b = 0; b < (n + 63) / 64; b++) {
            uint64_t v = 0;
            for (size_t k = 0; k < 64 && 64 * b + k < n; k++)
                if (ETH->rx_attention[64 * b + k]) v |= 1ull << k;
            T.mb[B_RXA + b] = v;
        }
        barrier();
        issue(OP_RXLOAD, 0, (uint16_t)n, 1, 0);
        my_rxloads++;
        shipped_gen = dorado_ethernet_rx_generation;
        shipped = true;
        status_drained = false;
        S.rx_packets++;
    }
    if (shipped && dorado_ethernet_rx_generation == shipped_gen) {
        bool was = ETH->rx_pos >= ETH->rx_count;
        ETH->rx_pos = (status_rxloads == my_rxloads && status_drained) ? ETH->rx_count : 0;
        if (!was && ETH->rx_pos >= ETH->rx_count)
            dorado_ethernet_rx_consumed(ETH);   // what eth_read does at the last word
    }
}

int step_hook(dorado_machine *, int after) {
    if (!after) {
        push_regs();
        return DORADO_STEP_HOOK_SKIP;
    }
    ethernet_bridge();
    return 0;
}

}  // namespace

int hps_start(const hps_config *cfg, const hps_transport *t) {
    CFG = *cfg;
    T = *t;
    if (!CFG.quantum) CFG.quantum = 1;
    dorado_machine_config mc;
    dorado_machine_config_default(&mc);
    mc.boot_keys_count = 0;
    mc.boot_keys[mc.boot_keys_count++] = DORADO_KEY_NONE;
    mc.alto_ether_boot = 0;
    mc.eth_boot_110 = CFG.eb;
    mc.germ_path = CFG.germ;
    mc.pilot_disk_pdi[0] = CFG.pdi;
    mc.ftp_root = CFG.ftp_root;
    M = dorado_machine_create(&mc);
    if (!M) { fprintf(stderr, "[hps] machine create failed\n"); return 1; }
    if (dorado_machine_restore(M, CFG.snapshot) != 0) {
        fprintf(stderr, "[hps] restore %s failed\n", CFG.snapshot);
        return 1;
    }
    dorado_machine_set_ftp_source(M, mc.ftp_sysout, mc.ftp_root);
    (void)dorado_machine_set_pilot_disk(M, 0, CFG.pdi);
    C = (dorado_cpu *)dorado_machine_cpu(M);
    ETH = dorado_machine_ethernet(M);
    dorado_memory *mem = dorado_machine_memory(M);
    if (mem->storage_words > T.storage_words) {
        fprintf(stderr, "[hps] snapshot storage %zu words > %zu\n",
                (size_t)mem->storage_words, T.storage_words);
        return 1;
    }

    // The mailbox starts clear (a stale seq could look like a command), and
    // storage holds the snapshot's; then the core may go.
    for (size_t b = 0; b < B_END; b++) T.mb[b] = 0;
    for (size_t i = 0; i < mem->storage_words; i++) T.storage[i] = mem->storage[i];
    barrier();
    T.mb[B_CTRL] = MB_MAGIC;
    barrier();
    uint64_t h = 1469598103934665603ull;          // FNV-1a over storage, to match images
    for (size_t i = 0; i < mem->storage_words; i++) h = (h ^ mem->storage[i]) * 1099511628211ull;
    fprintf(stderr, "[hps] restored %s at BaseBoard cycle %llu (cpu %llu); storage loaded "
            "(%zu words, hash %016llx); go\n",
            CFG.snapshot, (unsigned long long)dorado_machine_cycles(M),
            (unsigned long long)C->cycles, (size_t)mem->storage_words, (unsigned long long)h);
    while ((uint16_t)T.mb[B_STATUS + 2] == 0) T.wait(T.ctx);    // the first status
    read_status();
    cyc0 = S.rtl_cycles;
    S.rtl_clocks = 0;
    bb0 = dorado_machine_cycles(M);
    shipped_gen = dorado_ethernet_rx_generation;   // the checkpoint's packet is in the image
    shipped = ETH->rx_count > 0;
    my_rxloads = status_rxloads;
    last_tx = status_tx;
    memcpy(rm_prev, C->RM, sizeof rm_prev);
    C->reschedule_pending = 0;

    dorado_machine_skip_quantum = CFG.quantum;
    dorado_machine_host_only = 1;
    // live input: Cedar's display microcode on the FPGA posts the field
    // interrupt and takes the keyboard and mouse from the terminal back
    // channel, so the host stays out of both (scripted input, -c/-t, still
    // goes the host's way)
    dorado_machine_display_microcode = CFG.keyboard;
    if (CFG.germ_poll) dorado_machine_germ_poll_interval = CFG.germ_poll;
    dorado_host_read_hook = read_hook;
    dorado_host_write_hook = write_hook;
    dorado_machine_step_hook = step_hook;
    fprintf(stderr, "[hps] FPGA at microinstruction %llu; quantum %u, germ poll %llu\n",
            (unsigned long long)cyc0, CFG.quantum,
            (unsigned long long)dorado_machine_germ_poll_interval);
    return 0;
}

// The FPGA's RM[0..n-1] into the host's copy (diagnostics: the PDA dump
// reads the current PSB and NWW from it). An OR of 0 is a read.
void hps_sync_rm(unsigned n) {
    for (unsigned i = 0; i < n && i < RM_SIZE; i++) {
        C->RM[i] = result(issue(AP_RM_OR, i, 0, 1, 0));
        rm_prev[i] = C->RM[i];
    }
}

uint64_t hps_poll(void) {
    read_status();
    // Packets the RTL's transmitter sent (Cedar's own output microcode), to
    // the server, in order; the mailbox keeps the last eight.
    while (last_tx != status_tx) {
        if ((uint16_t)(status_tx - last_tx) > 8) {
            S.tx_unsupported += (uint16_t)(status_tx - last_tx) - 8;   // overrun: lost
            last_tx = (uint16_t)(status_tx - 8);
        }
        unsigned slot = last_tx & 7;
        size_t n = (size_t)(T.mb[B_TXLEN + slot] & 0x7FF);
        if (n > 1024) n = 1024;
        std::vector<uint16_t> pkt(n);
        for (size_t i = 0; i < n; i++)
            pkt[i] = (uint16_t)(T.mb[B_TX + slot * 256 + i / 4] >> (16 * (i % 4)));
        dorado_ethernet_direct_transmit(ETH, pkt.data(), n);
        S.tx_packets++;
        last_tx++;
    }
    // NWW as the FPGA has it (the timer tests it before ORing)
    C->RM[0] = result(issue(AP_RM_OR, 0, 0, 1, 0));
    rm_prev[0] = C->RM[0];
    uint64_t target = bb0 + (S.rtl_cycles - cyc0) * 37 / 10;
    S.host_target = target;                 // (reported uncapped: how far behind)
    // At most ~16 ms of guest time per poll: if the host falls behind the
    // FPGA, it catches up over several polls rather than in one ever-longer
    // run, so the transmit ring and the receive fills are still served.
    uint64_t now_bb = dorado_machine_cycles(M);
    if (target > now_bb + HOST_CHUNK_BB) target = now_bb + HOST_CHUNK_BB;
    if (target > now_bb) dorado_machine_run_until(M, target);
    ethernet_bridge();
    S.host_bb_cycles = dorado_machine_cycles(M);
    retire(1);                              // let queued commands finish
    return S.rtl_cycles;
}

int hps_snapshot_pgm(const char *path) {
    int px = dorado_machine_render_display_list(M);
    dorado_display_snapshot_pgm(dorado_machine_display(M), path);
    return px;
}

const hps_stats *hps_get_stats(void) { return &S; }
void *hps_machine(void) { return M; }
