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
//     AP_RESCHED. Every NWW bit a host service sets is sent, as an edge: the
//     host's copy of NWW is cleared after each push, never loaded from the
//     FPGA. (It used to be re-read from the FPGA each pass and only the bits
//     the copy did not already have were sent, so a completion whose bit was
//     still pending in that copy -- while the guest had already taken and
//     cleared it on the FPGA -- was never sent, and the guest waited for ever
//     on a disk or Ethernet completion. An OR into a bit still pending is what
//     the hardware does too: nothing.);
//   - each packet the server queues goes into the receive buffer area and an
//     RXLOAD command, once the FPGA has finished the last one and the
//     guest has read it (status: rx loads, rx_drained).
// The host follows the FPGA's microinstruction count (status beat 8) at 3.70
// BaseBoard cycles per microinstruction, in steps of cfg.quantum.
// =============================================================================
#include "hps_host.h"
#include <ctime>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include "scan_map.h"
#include "scan_ld.h"
#include <algorithm>

extern "C" {
#include "cpu.h"
#include "memory.h"
#include "io.h"
#include "display.h"
#include "ethernet.h"
#include "machine.h"
#include "disk.h"
#include "microcode.h"
#include "disasm.h"
}
#include "../uinstr_pack.h"

namespace {

// Mailbox layout, in beats (dorado_rtl_mailbox.sv)
enum : size_t {
    B_CTRL = 0, B_STATUS = 8, B_CMD = 256, B_RES = 512, B_RXW = 1024, B_RXA = 2048,
    B_TXLEN = 2112, B_TX = 4096, B_END = 6144,
    B_SCAN = 16384,              // the state scan bits (a DORADO_SCAN core)
    B_LOAD = 32768, B_LOAD_END = 1 << 20     // ...and its load records, to the mailbox's end
};
const uint32_t MB_MAGIC = 0x444F5241u;
const uint32_t MB_RESTART = 0x52534554u;  // "RSET": the core back to waiting for a go
const uint64_t HOST_CHUNK_BB = 1000000;   // BaseBoard cycles: 270K microinstructions
enum { AP_READ = 0, AP_STORE = 1, AP_STORE_INV = 2, AP_PEEK = 3, AP_RM_OR = 4, AP_WAKEUP_OR = 5, AP_RESCHED = 6, OP_RXLOAD = 7 };

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
uint64_t last40;               // the status beat's 40-bit count, last read
bool     have40;
bool     shipped;

inline void barrier() { __sync_synchronize(); }

uint8_t seq_of(uint64_t n) { return (uint8_t)((n / 256) % 255 + 1); }

static double mono_s();

// A command the core has not answered: every 2 s of waiting, print the
// mailbox's stall report (dorado_rtl_mailbox stall_tick; status beats 14-15)
// if it has written a new one, else say there is none -- which itself says
// the core's DDR client is not serving the aux port.
void stall_watch(double &since, uint64_t n) {
    double now = mono_s();
    if (since == 0) { since = now; return; }
    if (now - since < 2.0) return;
    since = now;
    static uint16_t last_n;
    barrier();
    uint64_t b14 = T.mb[B_STATUS + 6], b15 = T.mb[B_STATUS + 7];
    uint64_t cmd = T.mb[B_CMD + (n & 255)];
    if ((uint16_t)b15 == last_n) {
        printf("[hps] STALL: command %llu (%016llx) unanswered; no new stall report (%u so far)\n",
               (unsigned long long)n, (unsigned long long)cmd, (unsigned)last_n);
        return;
    }
    last_n = (uint16_t)b15;
    // dbg_stall: {mem st[63:59], cpu st[58:54], ifu_bk_busy, ap_req, ap_done_q, ap_hold,
    //   mem_ap_req, mem_req, mem_req_tag[2], mem_inflight, bk_n[2], sto_req, ov_hold,
    //   io_busy_tick, x_pref_pending, mem_fin, ctask[4], 4'0, real_PC[12], 18'0}
    auto f = [&](int hi, int w) { return (unsigned)((b14 >> (hi - w + 1)) & ((1ull << w) - 1)); };
    printf("[hps] STALL %u: command %llu (%016llx) unanswered; cpu st %u mem st %u ifu_bk_busy %u "
           "ap_req %u ap_done_q %u ap_hold %u mem_ap_req %u mem_req %u tag %u inflight %u bk_n %u "
           "sto_req %u ov_hold %u io_busy %u pref_pending %u mem_fin %u ctask %u pc %04o\n",
           (unsigned)last_n, (unsigned long long)n, (unsigned long long)cmd,
           f(58, 5), f(63, 5), f(53, 1), f(52, 1), f(51, 1), f(50, 1), f(49, 1), f(48, 1),
           f(47, 2), f(45, 1), f(44, 2), f(42, 1), f(41, 1), f(40, 1), f(39, 1), f(38, 1),
           f(37, 4), f(29, 12));
    // storage_ddr dbg[63:16]: {0, st[3], ready, BUSY, RD, WE, DOUT_READY, rd cmds[12],
    //   rd beats[16], wr beats[15:5]}
    auto g = [&](int hi, int w) { return (unsigned)((b15 >> (hi - w + 1)) & ((1ull << w) - 1)); };
    printf("[hps] STALL %u: ddr st %u ready %u BUSY %u RD %u WE %u DOUT_READY %u rd cmds %u "
           "rd beats %u (low 12: %u)\n", (unsigned)last_n,
           g(62, 3), g(59, 1), g(58, 1), g(57, 1), g(56, 1), g(55, 1), g(54, 12), g(42, 16),
           g(42, 16) & 0xFFF);
}

// Retire finished commands; returns once fewer than `room` are outstanding.
void retire(uint64_t room) {
    double since = 0;
    for (;;) {
        while (completed < issued) {
            uint64_t r = T.mb[B_RES + (completed & 255)];
            if ((uint8_t)(r >> 56) != seq_of(completed)) break;
            completed++;
        }
        if (issued - completed < room) return;
        stall_watch(since, completed);
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
    double since = 0;
    for (;;) {
        uint64_t r = T.mb[B_RES + (n & 255)];
        if ((uint8_t)(r >> 56) == seq_of(n)) {
            // The ARM reads a beat as two 32-bit halves, so the seq (high
            // half) can be the new one while rd (low half) is still last lap's.
            // The FPGA writes the beat once; read it again until it holds.
            barrier();
            uint64_t r2 = T.mb[B_RES + (n & 255)];
            if (r2 != r) { S.result_tears++; continue; }
            if (completed == n) completed++;
            return (uint16_t)r;
        }
        stall_watch(since, n);
        T.wait(T.ctx);
    }
}

uint16_t read_hook(const struct dorado_memory *, uint32_t va) {
    // DORADO_HPS_READHIST=1: count the host's guest reads by VA (each is a
    // mailbox round trip on the board); the top ones print at exit
    static const bool rh = getenv("DORADO_HPS_READHIST") != nullptr;
    if (rh) {
        static std::map<uint32_t, uint64_t> *hist = nullptr;
        if (!hist) {
            hist = new std::map<uint32_t, uint64_t>;
            atexit([] {
                std::vector<std::pair<uint64_t, uint32_t>> v;
                for (auto &kv : *hist) v.push_back({kv.second, kv.first});
                std::sort(v.rbegin(), v.rend());
                for (size_t k = 0; k < v.size() && k < 20; k++)
                    fprintf(stderr, "[hps] read VA %08o: %llu\n", v[k].second, (unsigned long long)v[k].first);
            });
        }
        (*hist)[va]++;
    }
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

static double mono_s(void) {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void read_status() {
    barrier();
    S.t_status = mono_s();
    uint64_t b8 = T.mb[B_STATUS], b9 = T.mb[B_STATUS + 1];
    uint64_t b10 = T.mb[B_STATUS + 2], b11 = T.mb[B_STATUS + 3];
    {
        // The status beat carries 40 bits of the microinstruction count,
        // which wrap after 2^40 = 18.3 hours of a Dorado. Extend them here:
        // a count that steps back leaves every "due at cycle N" test in the
        // host (the live input seeding among them) waiting for a day.
        const uint64_t c40 = b8 & 0xFFFFFFFFFFull;
        if (!have40) { S.rtl_cycles = c40; have40 = true; }
        else S.rtl_cycles += (c40 - last40) & 0xFFFFFFFFFFull;
        last40 = c40;
    }
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
    // the receive state, all in beat 11's high half (one 32-bit read)
    status_drained = (b11 >> 48) & 1;
    status_tx      = (uint16_t)(b11 >> 16);
    status_rxloads = (uint16_t)(b11 >> 32);
    S.rx_applied   = (uint16_t)(b11 >> 49);
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
    // NWW: each bit set since the last push, as an edge (see the header)
    uint16_t nww = (uint16_t)(C->RM[0] & ~rm_prev[0]);
    if (nww) { issue(AP_RM_OR, 0, nww, 1, 0); S.rm_or++; S.nww_posts++; }
    C->RM[0] = 0;
    rm_prev[0] = 0;
    if (!C->reschedule_pending && memcmp(C->RM, rm_prev, sizeof rm_prev) == 0) return;
    for (int i = 1; i < RM_SIZE; i++) {
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

// DORADO_HPS_RXCHECK=1: end-to-end check of each received packet. At the
// fill, note the input IOCB Cedar has armed (CSB nextInput, 177600B) and
// the words sent; once the receive microcode has completed that IOCB, read
// its buffer back out of guest memory and compare. Reports a packet that
// arrived damaged, a short or long one, and an IOCB never completed.
// Guest reads go through the mailbox, so this slows the host a little.
const uint32_t CSB_VA = 0177600u;          // machine.c GERM_ETH_CSB_VA
struct rx_check {
    bool     armed;
    uint16_t iocb;                         // nextInput at the fill
    std::vector<uint16_t> words;           // as sent
    double   t;                            // CLOCK_MONOTONIC at the fill
    unsigned n;                            // fill number
} RXC;
uint64_t rxc_ok, rxc_bad;

uint16_t guest(uint32_t va) { return dorado_visible_word_at_va(dorado_machine_memory(M), va); }

void rx_check_poll() {
    if (!RXC.armed) return;
    uint16_t done = guest((uint32_t)RXC.iocb + 1u);
    if (done == 0) {
        if (mono_s() - RXC.t > 2.0) {
            printf("[hps] RXCHECK %u: iocb %06o not completed 2 s after the fill (nextInput now %06o)\n",
                   RXC.n, RXC.iocb, guest(CSB_VA));
            RXC.armed = false;
            rxc_bad++;
        }
        return;
    }
    RXC.armed = false;
    uint16_t used = guest((uint32_t)RXC.iocb + 2u);
    uint32_t buf  = ((uint32_t)guest((uint32_t)RXC.iocb + 6u) << 16) | guest((uint32_t)RXC.iocb + 5u);
    size_t n = RXC.words.size() >= 2 ? RXC.words.size() - 2 : RXC.words.size();   // less the CRC/status trailer
    size_t cmp = used < n ? used : n;
    size_t bad = 0, first = (size_t)-1;
    for (size_t i = 0; i < cmp; i++)
        if (guest(buf + (uint32_t)i) != RXC.words[i]) { if (first == (size_t)-1) first = i; bad++; }
    if (bad || used != n) {
        rxc_bad++;
        printf("[hps] RXCHECK %u: iocb %06o completion %06o used %u (sent %zu) buffer %08o: %zu words differ",
               RXC.n, RXC.iocb, done, used, n, buf, bad);
        if (first != (size_t)-1)
            printf(", first at %zu: guest %06o sent %06o", first, guest(buf + (uint32_t)first), RXC.words[first]);
        printf("; Pup type %06o\n", n > 3 ? RXC.words[3] : 0);
    } else {
        rxc_ok++;
    }
}

void ethernet_bridge() {
    static const bool rxcheck = getenv("DORADO_HPS_RXCHECK") != nullptr;
    if (rxcheck) rx_check_poll();
    if (CFG.quantum > 1 && ETH->rx_hold)
        ETH->rx_hold = ETH->rx_hold > CFG.quantum - 1 ? ETH->rx_hold - (CFG.quantum - 1) : 0;
    bool fpga_done = status_rxloads == my_rxloads;   // the last fill is finished
    // An Alto world's posted input buffer (EICLOC, VM 604B): the emulator
    // keeps its size in world_rx_words (machine.c, a poll of storage) and
    // holds a packet that would overflow it. The core's copy of that size is
    // the one it was restored with -- it cannot look at storage -- so a cold
    // start is given 177777 there (gen_scan.py --cold) and the packet is
    // held here instead. (A germ world: 177777, no hold.)
    const bool room = ETH->rx_count <= (size_t)ETH->world_rx_words + 2;
    if (dorado_ethernet_rx_generation != shipped_gen && ETH->rx_count > 0 &&
        ETH->rx_hold == 0 && fpga_done && room) {
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
        if (rxcheck) {
            if (RXC.armed) rx_check_poll();
            if (RXC.armed) {
                printf("[hps] RXCHECK %u: iocb %06o still not completed at the next fill\n", RXC.n, RXC.iocb);
                rxc_bad++;
            }
            RXC.armed = true;
            RXC.iocb  = guest(CSB_VA);
            RXC.words.assign(ETH->rx_words, ETH->rx_words + n);
            RXC.t = mono_s();
            RXC.n = (unsigned)my_rxloads + 1;
            if (RXC.iocb == 0 || guest((uint32_t)RXC.iocb + 1u) != 0) {
                printf("[hps] RXCHECK %u: filled with no input IOCB armed (nextInput %06o, its completion %06o); "
                       "Pup type %06o -- the receiver will drop it\n", RXC.n, RXC.iocb,
                       RXC.iocb ? guest((uint32_t)RXC.iocb + 1u) : 0, n > 3 ? ETH->rx_words[3] : 0);
                rxc_bad++;
                RXC.armed = false;
            }
        }
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

std::vector<uint16_t> written_pages;     // Map entries of the dirty lines written back at the start

// <snapshot>.regs into the scan region; returns the number of bits (0: no
// file, or one that does not cover the chain).
// A cold start of a germ world (Cedar; the .regs file has io.ddc_wake_held
// set: the machine as the germ was planted). The core holds the display
// tasks' wakeups until the loaded world's terminal task drives the
// controller, and Cedar's does not until it is woken: the emulator starts
// the scan-line wakeups of a germ world when the world has installed a
// display list (machine.c, machine_alto_display_active). The core cannot
// look at storage, so the host does, a field apart, and wakes the terminal
// task -- whose first output to the controller ends the hold. (An Alto
// world's display microcode starts its own tasks; no wake from here.)
bool     cold_held;
uint64_t cold_next;
unsigned cold_wakes;

// A snapshot with no .regs/.rams beside it (the stock desktops, the cold
// starts): the same registers and load records made here from the restored C
// machine, by the copy the simulation's checkpoint dump uses
// (../sync_*_body.inc; sim_cedar.cpp CEDAR_DUMP_INIT + tools/gen_scan.py
// --regs/--rams, which this replaces on the board). A save keeps its own
// files: they are the core's registers read back, and carry what the C
// machine has no copy of.
struct reg_cell {
    uint64_t v = 0;
    std::vector<reg_cell> sub;        // (an array's entries; the board is short of memory: no map)
    reg_cell &operator[](int i) { if ((size_t)i >= sub.size()) sub.resize((size_t)i + 1); return sub[(size_t)i]; }
    template <class X> reg_cell &operator=(X x) { v = (uint64_t)x; return *this; }
    operator uint64_t() const { return v; }
};
std::map<std::string, reg_cell> RG;
#define R_CPU(n) RG["cpu." #n]
#define R_MEM(n) RG["mem." #n]
#define R_IO(n)  RG["io." #n]
#define DS(n, v)     R_IO(n) = (v)
#define DSA(n, i, v) R_IO(n)[i] = (v)

// The Trident pack is the core's: its controller reads and writes the
// sectors in DDR. The emulator's own pack structure holds every sector at
// the largest size any geometry has (2,074 bytes for a 532-byte Diablo
// sector: 245 MB for the 63 MB Lisp and Smalltalk packs, half the MiSTer's
// memory), so the host does not load it: the file goes to DDR a sector at a
// time, and the C controller is given a pack of the right geometry with no
// sectors, which is all the register copy (sync_disk_body.inc) reads.
static dorado_disk_pack PK;                  // geometry and path; no sectors
static int PK_SECTORS;                       // the pack's true sector count
static size_t pack_stride() {                // 64-bit beats a sector in DDR
    const dorado_disk_geometry *g = &PK.geometry;
    return (size_t)(dorado_disk_geom_header_words(g) + dorado_disk_geom_label_words(g) +
                    dorado_disk_geom_data_words(g) + 3) / 4;
}
// The geometry is told from the file's size, in machine.c's order.
static int pack_open(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); return -1; }
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    fclose(f);
    const dorado_disk_geometry *gs[] = {&DORADO_DISK_T80, &DORADO_DISK_T300, &DORADO_DISK_DIABLO};
    for (const dorado_disk_geometry *g : gs) {
        const long n = (long)g->cylinders * g->heads * g->sectors;
        const long bytes = 2L * (DORADO_DISK_DUMMY_WORDS + dorado_disk_geom_header_words(g) +
                                 dorado_disk_geom_label_words(g) + dorado_disk_geom_data_words(g));
        if (size != n * bytes) continue;
        memset(&PK, 0, sizeof PK);
        PK.geometry = *g;
        snprintf(PK.path, sizeof PK.path, "%s", path);
        PK_SECTORS = (int)n;
        dorado_disk_controller_attach_drive(dorado_machine_disk(M), 0, &PK);
        return 0;
    }
    fprintf(stderr, "[hps] '%s' is not a T-80/T-300/Diablo-Trident pack\n", path);
    return -1;
}
// A pack file from the DDR beats (dorado_disk_pack_save's layout).
static int pack_write(const char *path, const uint64_t *beats) {
    FILE *f = fopen(path, "wb");
    if (!f) { perror(path); return -1; }
    const dorado_disk_geometry *g = &PK.geometry;
    const size_t words = (size_t)(dorado_disk_geom_header_words(g) + dorado_disk_geom_label_words(g) +
                                  dorado_disk_geom_data_words(g));
    const size_t stride = pack_stride();
    std::vector<uint8_t> buf(2 * (words + DORADO_DISK_DUMMY_WORDS), 0);
    int rc = 0;
    for (int i = 0; i < PK_SECTORS && !rc; i++) {
        uint8_t *q = buf.data() + 2 * DORADO_DISK_DUMMY_WORDS;
        for (size_t k = 0; k < words; k++) {
            const uint16_t w = (uint16_t)(beats[(size_t)i * stride + k / 4] >> (16 * (k % 4)));
            q[2*k] = (uint8_t)w;
            q[2*k+1] = (uint8_t)(w >> 8);
        }
        if (fwrite(buf.data(), 1, buf.size(), f) != buf.size()) rc = -1;
    }
    if (fclose(f) != 0) rc = -1;
    return rc;
}

void derive_regs(std::map<std::string, uint64_t> &v, std::vector<uint64_t> &recs) {
    RG.clear();
    const dorado_cpu *c_cpu = C;
    const dorado_memory *c_mem = c_cpu->mem;
    const dorado_microcode *mc = c_cpu->mc;
#include "../sync_cpu_body.inc"
    {
        const dorado_ethernet *eth = ETH;
        R_IO(eth_tx_on) = eth->tx_on ? 1 : 0;
        R_IO(eth_tx_eop) = eth->tx_eop ? 1 : 0;
        R_IO(eth_tx_cntdwn) = eth->tx_cntdwn ? 1 : 0;
        R_IO(eth_rx_on) = eth->rx_on ? 1 : 0;
        R_IO(eth_no_wakeups) = eth->no_wakeups ? 1 : 0;
        R_IO(eth_rx_hold) = eth->rx_hold;
        R_IO(eth_rx_wire_timer) = eth->rx_wire_timer;
        // (a cold start: the receiver's copy of the Alto world's input buffer
        // size, which nothing in the core refreshes -- no limit there; the
        // host holds the packet)
        R_IO(eth_world_rx_words) = CFG.cold ? 0xffffu : eth->world_rx_words;
        const size_t n = eth->rx_count < 4096 ? eth->rx_count : 4096;
        for (size_t i = 0; i < n; i++) {
            R_IO(eth_rx_words)[(int)i] = eth->rx_words[i];
            R_IO(eth_rx_attention)[(int)i] = eth->rx_attention[i] ? 1 : 0;
        }
        R_IO(eth_rx_count) = (int)n;
        R_IO(eth_rx_pos) = (int)(eth->rx_pos < n ? eth->rx_pos : n);
    }
    {
        const dorado_display *d = dorado_machine_display(M);
#include "../sync_display_body.inc"
    }
    // an Alto-world (Trident) disk; unarmed, so the core arms the rotation on
    // its own microinstruction count
    if (CFG.pack) {
        const dorado_disk_controller *CD = dorado_machine_disk(M);
        if (CD->read_stream_active || CD->write_stream_active)
            fprintf(stderr, "[hps] a disk transfer is in progress in the snapshot; the controller starts as reset leaves it\n");
        else {
#include "../sync_disk_body.inc"
            DS(disk_next_sector_cycle, 0);
        }
    }
    // registers of the RTL alone (gen_scan.py EXTRA): the terminal's serializer
    // at rest; a cold start holds the display tasks' wakeups until the loaded
    // world drives the controller
    R_IO(ddc_term_msg_bit) = 0; R_IO(ddc_term_msg_word) = 0; R_IO(ddc_term_msg_type) = 0;
    R_IO(ddc_term_msg_body) = 0; R_IO(ddc_term_msg_active) = 0;
    R_IO(ddc_wake_held) = CFG.cold ? 1 : 0;

    v.clear();
    for (auto &kv : RG) {
        if (kv.second.sub.empty()) { v[kv.first] = kv.second.v; continue; }
        // (the register files are 16 or 64 entries; the memories go by load record)
        if (kv.second.sub.size() > 64) continue;
        for (size_t e = 0; e < kv.second.sub.size(); e++)
            if (kv.second.sub[e].sub.empty()) v[kv.first + "[" + std::to_string(e) + "]"] = kv.second.sub[e].v;
    }
    // the load records (dorado_rtl_mailbox.sv), in gen_scan.py --rams' order
    auto A = [&](const char *n, int i) -> uint64_t {
        auto it = RG.find(n);
        if (it == RG.end()) return 0;
        return (size_t)i < it->second.sub.size() ? it->second.sub[(size_t)i].v : 0;
    };
    recs.clear();
    for (uint64_t a = 0; a < IM_SIZE; a++)
        if (A("cpu.im_present", (int)a))
            recs.push_back(1ull << 60 | a << 48 | (A("cpu.im_iw0", (int)a) & 0xffff) << 32 |
                           (A("cpu.im_iw1", (int)a) & 0xffff) << 16 | (A("cpu.im_iw2", (int)a) & 0xffff));
    for (uint64_t a = 0; a < IFUM_SIZE; a++)
        if (A("cpu.ifum_present", (int)a)) {
            recs.push_back(2ull << 60 | a << 16 | (A("cpu.ifum_lo", (int)a) & 0xffff));
            recs.push_back(3ull << 60 | a << 16 | (A("cpu.ifum_hi", (int)a) & 0xffff));
        }
    for (uint64_t a = 0; a < 256; a++) recs.push_back(4ull << 60 | a << 16 | (A("cpu.RM", (int)a) & 0xffff));
    for (uint64_t a = 0; a < 256; a++) recs.push_back(4ull << 60 | (256 + a) << 16 | (A("cpu.STK", (int)a) & 0xffff));
    for (uint64_t a = 0; a < ALUFM_SIZE; a++)
        if (A("cpu.alufm_present", (int)a)) recs.push_back(5ull << 60 | a << 16 | (A("cpu.alufm", (int)a) & 0xff));
    for (uint64_t a = 0; a < DM_MAP_ENTRIES; a++)
        recs.push_back(6ull << 60 | a << 32 | (A("mem.map_wp", (int)a) & 1) << 18 | (A("mem.map_dirty", (int)a) & 1) << 17 |
                       (A("mem.map_ref", (int)a) & 1) << 16 | (A("mem.map_rp", (int)a) & 0xffff));
    for (uint64_t a = 0; a < 4096; a++)
        recs.push_back(7ull << 60 | (A("io.eth_rx_attention", (int)a) & 1) << 32 | a << 16 | (A("io.eth_rx_words", (int)a) & 0xffff));
    recs.push_back(15ull << 60);
    RG.clear();
}
#undef R_CPU
#undef R_MEM
#undef R_IO
#undef DS
#undef DSA

uint64_t load_scan_regs() {
    std::string path = std::string(CFG.snapshot) + ".regs";
    std::map<std::string, uint64_t> v;
    std::vector<uint64_t> derived;
    FILE *f = fopen(path.c_str(), "r");
    if (!f && !CFG.derive) return 0;
    if (!f) {
        derive_regs(v, derived);
        path = std::string(CFG.snapshot) + " (registers and load records made from the snapshot)";
        // DORADO_HPS_WRITE_DERIVED=1: beside the snapshot too, as <snapshot>.regs.made / .rams.made
        if (getenv("DORADO_HPS_WRITE_DERIVED")) {
            if (FILE *o = fopen((std::string(CFG.snapshot) + ".regs.made").c_str(), "w")) {
                for (auto &kv : v) fprintf(o, "%s %llx\n", kv.first.c_str(), (unsigned long long)kv.second);
                fclose(o);
            }
            if (FILE *o = fopen((std::string(CFG.snapshot) + ".rams.made").c_str(), "wb")) {
                fwrite(derived.data(), 8, derived.size(), o);
                fclose(o);
            }
        }
    }
    char line[256], mod[16] = "";
    while (f && fgets(line, sizeof line, f)) {
        char name[128];
        unsigned long long x;
        if (line[0] == '@') { sscanf(line + 1, "%15s", mod); continue; }
        if (sscanf(line, "%127s %llx", name, &x) == 2) v[std::string(mod) + "." + name] = x;
    }
    if (f) fclose(f);
    cold_held = CFG.germ && v.count("io.ddc_wake_held") && v["io.ddc_wake_held"] != 0;
    cold_next = 0; cold_wakes = 0;
    std::vector<uint64_t> beats((SCAN_BITS + 63) / 64, 0);
    size_t bit = 0, missing = 0;
    for (int i = 0; i < SCAN_REGS; i++) {
        const bool pad = !strcmp(scan_map[i].mod, "pad");     // a module's scan_out register
        auto it = pad ? v.end() : v.find(std::string(scan_map[i].mod) + "." + scan_map[i].name);
        // (a world saved without the Trident controller, Cedar: the
        // controller's registers as reset leaves them, which is 0)
        // (ddc_wake_held: a save from before a cold start could be loaded; 0)
        if (it == v.end() && !pad && strncmp(scan_map[i].name, "disk_", 5) &&
            strcmp(scan_map[i].name, "ddc_wake_held")) missing++;
        const uint64_t x = it == v.end() ? 0 : it->second;
        for (int b = scan_map[i].bits - 1; b >= 0; b--, bit++)
            if (b < 64 && ((x >> b) & 1)) beats[bit / 64] |= 1ull << (63 - bit % 64);
    }
    if (missing) {
        fprintf(stderr, "[hps] %s: %zu of the scan chain's %d registers are not in it; not used\n",
                path.c_str(), missing, SCAN_REGS);
        return 0;
    }
    // <snapshot>.rams: the load records (IM, IFUM, RM/STK, ALUFM, the Map,
    // the Ethernet receive buffer), as the mailbox takes them
    {
        std::string rpath = std::string(CFG.snapshot) + ".rams";
        FILE *r = derived.empty() ? fopen(rpath.c_str(), "rb") : nullptr;
        size_t n = 0, di = 0;
        uint64_t rec = 0;
        bool ended = false;
        while (B_LOAD + n < B_LOAD_END &&
               (r ? fread(&rec, 8, 1, r) == 1 : di < derived.size() && (rec = derived[di++], true))) {
            // a Map entry whose page took a writeback at the start: Ref and
            // Dirty, as the memory section's own writeback sets them
            if ((rec >> 60) == 6)
                for (uint16_t pg : written_pages)
                    if (pg == (uint16_t)(rec >> 32)) { rec |= 3ull << 16; break; }
            if ((rec >> 60) == 15) { ended = true; break; }
            T.mb[B_LOAD + n++] = rec;
        }
        if (r) fclose(r);
        // the register files (scan_ld.h): an entry a record, from the .regs values
        for (int i = 0; ended && i < SCAN_LD_FILES; i++)
            for (int e = 0; e < scan_ld[i].n; e++) {
                auto it = v.find(std::string(scan_ld[i].mod) + "." + scan_ld[i].name + "[" + std::to_string(e) + "]");
                // (the file rotates an entry a record: all of them, first to last)
                if (it == v.end() && strncmp(scan_ld[i].name, "disk_", 5)) missing++;
                T.mb[B_LOAD + n++] = (uint64_t)scan_ld[i].kind << 60 | (uint64_t)scan_ld[i].id << 52 |
                                     (uint64_t)e << 32 | (it == v.end() ? 0u : (uint32_t)it->second);
            }
        if (missing) {
            fprintf(stderr, "[hps] %s: %zu register-file entries are not in it; not used\n", path.c_str(), missing);
            return 0;
        }
        T.mb[B_LOAD + n++] = 15ull << 60;
        if (!ended) {
            fprintf(stderr, "[hps] %s: no load records (or no end record); the registers are not used\n",
                    rpath.c_str());
            return 0;
        }
        fprintf(stderr, "[hps] %s: %zu load records\n", rpath.c_str(), n);
    }
    for (size_t i = 0; i < beats.size(); i++) T.mb[B_SCAN + i] = beats[i];
    fprintf(stderr, "[hps] %s: %d registers, %d scan bits\n", path.c_str(), SCAN_REGS, SCAN_BITS);
    return SCAN_BITS;
}

}  // namespace

static uint64_t issue_raw(uint64_t bits) {
    retire(255);
    const uint64_t n = issued;
    const uint64_t e = ((uint64_t)seq_of(n) << 56) | bits;
    volatile uint32_t *w = (volatile uint32_t *)&T.mb[B_CMD + (n & 255)];
    w[0] = (uint32_t)e;
    barrier();
    w[1] = (uint32_t)(e >> 32);
    barrier();
    issued++;
    return n;
}

// A save of the running machine, as a snapshot this program restores:
// <path> (the host's machine, with the core's storage), <path>.regs and
// <path>.rams. The CPU is held between microinstructions for all of it.
//  - the registers and register files: hps_save_regs
//  - RM and STK: read through the host port; the Map and the cache's rows:
//    AP_PEEK; a dirty line's words are read by address (the cache answers)
//    and put in the storage image, as a writeback would
//  - storage: the core's, from DDR
//  - IM, IFUM, ALUFM and the Ethernet receive buffer are NOT read back:
//    their records are the ones this run was loaded from (a world that has
//    rewritten its microcode since is not saved correctly)
int hps_save_regs(const char *path);
// a register of the chain as the last save left it in the scan region
static uint64_t scan_reg(const char *mod, const char *name) {
    size_t bit = 0;
    for (int i = 0; i < SCAN_REGS; i++) {
        if (!strcmp(scan_map[i].mod, mod) && !strcmp(scan_map[i].name, name)) {
            uint64_t x = 0;
            for (int b = scan_map[i].bits - 1; b >= 0; b--, bit++)
                if (b < 64 && ((T.mb[B_SCAN + bit / 64] >> (63 - bit % 64)) & 1)) x |= 1ull << b;
            return x;
        }
        bit += scan_map[i].bits;
    }
    return 0;
}
// hps_save_steps: which parts run (a diagnosis: 1 the freeze, 2 the chain,
// 4 RM/STK, 8 the Map, 16 the cache's lines, 32 the files written)
unsigned hps_save_steps = 63;
int hps_save(const char *path) {
    const unsigned st = hps_save_steps;
    if (st & 1) result(issue_raw(3ull << 53));           // freeze
    std::string snap = path;
    // Not with a sector in flight: the Trident controller's sector buffer is
    // not saved (a restore's engine starts idle), so a save taken inside a
    // sector's read or write would lose it. The streams are in the chain;
    // while either is active the machine runs a little longer and the save
    // starts again (a sector is 5.6 ms of a Dorado).
    for (int tries = 0; (st & 2); tries++) {
        if (hps_save_regs(snap.c_str()) != 0) { result(issue_raw(1ull << 54)); return -1; }
        const bool busy = scan_reg("io", "disk_read_stream_active") || scan_reg("io", "disk_write_stream_active");
        if (!busy || !(st & 1)) { if (tries) printf("[hps] save: waited for the disk %d times\n", tries); break; }
        if (tries == 400) { printf("[hps] save: the disk never came to rest; saving with a sector in flight\n"); break; }
        result(issue_raw(1ull << 54));                   // let it go...
        const double t0 = mono_s();
        for (int k = 0; k < 2000 || mono_s() - t0 < 0.005; k++) T.wait(T.ctx);
        result(issue_raw(3ull << 53));                   // ...and hold it again
    }
    std::vector<uint64_t> recs;
    std::vector<uint64_t> q;
    // The control store, as it is now -- a world can load microcode after it
    // starts (lisp.run does: a cold-started Lisp saved with the loaded
    // file's IM came back with the Alto emulator's alone). AP_PEEK with
    // ap_wd[3], answered by the processor (dorado_rtl_cpu x_pk).
    for (unsigned base = 0; (st & 4) && base < 4096; base += 16) {
        q.clear();
        for (unsigned a = base; a < base + 16; a++)
            for (unsigned k = 0; k < 4; k++) q.push_back(issue(AP_PEEK, a, (uint16_t)(8 | k), 0, 0));
        for (unsigned i = 0; i < 16; i++) {
            const uint64_t w0 = result(q[4 * i]), w1 = result(q[4 * i + 1]), w2 = result(q[4 * i + 2]);
            if (result(q[4 * i + 3]) & 1)
                recs.push_back(1ull << 60 | (uint64_t)(base + i) << 48 | w0 << 32 | w1 << 16 | w2);
        }
    }
    for (unsigned base = 0; (st & 4) && base < 1024; base += 16) {
        q.clear();
        for (unsigned a = base; a < base + 16; a++)
            for (unsigned k = 4; k < 7; k++) q.push_back(issue(AP_PEEK, a, (uint16_t)(8 | k), 0, 0));
        for (unsigned i = 0; i < 16; i++) {
            const uint64_t lo = result(q[3 * i]), hi = result(q[3 * i + 1]);
            if (result(q[3 * i + 2]) & 1) {
                recs.push_back(2ull << 60 | (uint64_t)(base + i) << 16 | lo);
                recs.push_back(3ull << 60 | (uint64_t)(base + i) << 16 | hi);
            }
        }
    }
    {
        q.clear();
        for (unsigned a = 0; a < 16; a++) q.push_back(issue(AP_PEEK, a, 8 | 7, 0, 0));
        for (unsigned a = 0; a < 16; a++) {
            const uint16_t v = result(q[a]);
            if ((st & 4) && (v & 0100)) recs.push_back(5ull << 60 | (uint64_t)a << 16 | (v & 077));
        }
    }
    {   // the memory this does not read (the Ethernet receive buffer): from
        // the loaded file -- and the control store, if this core has no look at it
        FILE *r = fopen((std::string(CFG.snapshot) + ".rams").c_str(), "rb");
        uint64_t rec;
        const bool have = !recs.empty();
        while (r && fread(&rec, 8, 1, r) == 1) {
            const unsigned k = (unsigned)(rec >> 60);
            if (k == 7 || (!have && (k == 1 || k == 2 || k == 3 || k == 5))) recs.push_back(rec);
        }
        if (r) fclose(r);
    }
    for (unsigned base = 0; (st & 4) && base < 512; base += 64) {    // (in runs the command ring holds)
        q.clear();
        for (unsigned a = base; a < base + 64; a++) q.push_back(issue(AP_RM_OR, a, 0, 1, 0));
        for (unsigned i = 0; i < 64; i++) recs.push_back(4ull << 60 | (uint64_t)(base + i) << 16 | result(q[i]));
    }
    std::vector<uint16_t> rp(65536), fl(65536);
    for (unsigned base = 0; (st & 8) && base < 65536; base += 64) {  // (in runs the command ring holds)
        q.clear();
        for (unsigned pg = base; pg < base + 64; pg++) {
            q.push_back(issue(AP_PEEK, pg << 8, 0, 0, 0));
            q.push_back(issue(AP_PEEK, pg << 8, 1, 0, 0));
        }
        for (unsigned i = 0; i < 64; i++) { rp[base + i] = result(q[2 * i]); fl[base + i] = result(q[2 * i + 1]); }
    }
    dorado_memory *mem = dorado_machine_memory(M);
    for (size_t i = 0; i < mem->storage_words; i++) mem->storage[i] = T.storage[i];
    unsigned lines = 0;
    for (unsigned r = 0; (st & 16) && r < DM_CACHE_ROWS; r++)
        for (unsigned w = 0; w < DM_CACHE_WAYS; w++) {
            const uint16_t t0 = result(issue(AP_PEEK, r << 4, (uint16_t)(2 | w << 4), 0, 0));
            const uint16_t t1 = result(issue(AP_PEEK, r << 4, (uint16_t)(3 | w << 4), 0, 0));
            const bool valid = t1 & 4, dirty = t1 & 8, vacant = t1 & 16;
            if (!valid || !dirty || vacant) continue;
            const uint32_t va = ((uint32_t)(t1 & 3) << 16 | t0) << 10 | r << 4;
            const uint32_t phys = (uint32_t)rp[va >> 8] << 8 | (va & 0xF0);
            q.clear();
            for (unsigned i = 0; i < DM_CACHE_LINE_W; i++) q.push_back(issue(AP_READ, va + i, 0, 0, 1));
            for (unsigned i = 0; i < DM_CACHE_LINE_W; i++) {
                const uint16_t d = result(q[i]);
                if (phys + i < mem->storage_words) mem->storage[phys + i] = d;
            }
            fl[va >> 8] |= 3;                            // Ref and Dirty, as the writeback sets them
            lines++;
        }
    for (unsigned pg = 0; pg < 65536; pg++)
        recs.push_back(6ull << 60 | (uint64_t)pg << 32 | (uint64_t)(fl[pg] & 7) << 16 | rp[pg]);
    recs.push_back(15ull << 60);
    if (!(st & 32)) {
        if (st & 1) result(issue_raw(1ull << 54));
        printf("[hps] save steps %u done (no files)\n", st);
        return 0;
    }
    // the host's machine, with the core's storage and no cache of its own
    for (int r = 0; r < DM_CACHE_ROWS; r++)
        for (int w = 0; w < DM_CACHE_WAYS; w++) mem->cache[r].ways[w].valid = 0;
    // The disk as it is now goes beside the snapshot (hps_start takes
    // <snapshot>.pack / .pdi in the world's own disk's place): the Trident
    // pack out of DDR, where the core's controller has written it; the
    // Pilot volume is the host's (its germ bridge holds it).
    // The host holds no copy of the pack (pack_open): the beats are taken
    // out of DDR here, while the machine is stopped, and the writer puts
    // them in the pack's file layout.
    const bool pk = CFG.pack && T.pack && PK_SECTORS;
    std::vector<uint64_t> pack_now;
    if (pk) pack_now.assign(T.pack, T.pack + (size_t)PK_SECTORS * pack_stride());
    // Everything is in this process's memory now. The files (the pack is
    // 63 MB: a minute on the MiSTer's SD card) are written by a child, a
    // copy of this process at this instant, and the machine goes on at
    // once. <snap>.writing is there until the last file is whole, and
    // hps_start waits for it to go. The simulation (and a fork that fails)
    // writes them here instead.
    static pid_t writer;
    if (writer > 0 && waitpid(writer, nullptr, WNOHANG) == 0) {
        printf("[hps] save: the last save's files are still being written; not saved\n");
        result(issue_raw(1ull << 54));
        return -1;
    }
    fflush(stdout);
    fflush(stderr);
    const std::string mark = snap + ".writing";
    if (FILE *m = fopen(mark.c_str(), "w")) fclose(m);
    writer = CFG.need_core ? fork() : -1;
    if (writer > 0) {
        std::vector<uint64_t>().swap(pack_now);          // (the child has its own copy)
        result(issue_raw(1ull << 54));                   // let it go
        printf("[hps] save: the machine goes on; %s is being written\n", snap.c_str());
        return 0;
    }
    if (writer == 0) signal(SIGTERM, SIG_IGN);           // (a new start's killall: the save is finished first)
    int rc = 0;
    {
        FILE *r = fopen((snap + ".rams").c_str(), "wb");
        if (!r) { perror((snap + ".rams").c_str()); rc = -1; }
        else {
            if (fwrite(recs.data(), 8, recs.size(), r) != recs.size()) rc = -1;
            fclose(r);
        }
    }
    if (dorado_machine_snapshot(M, snap.c_str()) != 0) rc = -1;
    const char *disk = "";
    if (pk) {
        if (pack_write((snap + ".pack").c_str(), pack_now.data()) != 0) rc = -1;
        disk = ", .pack";
    } else if (dorado_machine_has_pilot_disk(M, 0)) {
        char err[200] = "";
        if (dorado_machine_save_pilot_disk(M, 0, (snap + ".pdi").c_str(), err, sizeof err) != 0) {
            printf("[hps] save: the Pilot disk: %s\n", err);
            rc = -1;
        }
        disk = ", .pdi";
    }
    remove(mark.c_str());
    printf("[hps] saved the machine -> %s (+ .regs, .rams%s): %zu records, %u dirty cache lines into storage%s\n",
           snap.c_str(), disk, recs.size(), lines, rc ? "; A WRITE FAILED" : "");
    if (writer == 0) { fflush(stdout); _exit(rc ? 1 : 0); }
    result(issue_raw(1ull << 54));                       // let it go
    return rc;
}

// A save of the running machine's registers (a DORADO_SCAN core): the
// mailbox holds the CPU between microinstructions with nothing in flight
// and takes the scan chain round once, out to the scan region and back in;
// the machine carries on. <path>.regs is what load_scan_regs reads -- the
// chain's registers only: the register files and the memories are not read
// back yet.
int hps_save_regs(const char *path) {
    retire(1);
    // the register files: a record an entry, which the mailbox fills in
    {
        size_t k = 0;
        for (int i = 0; i < SCAN_LD_FILES; i++)
            for (int e = 0; e < scan_ld[i].n; e++)
                T.mb[B_LOAD + k++] = (uint64_t)scan_ld[i].kind << 60 | (uint64_t)scan_ld[i].id << 52;
        T.mb[B_LOAD + k] = 15ull << 60;
        barrier();
    }
    const uint64_t n = issued;
    const uint64_t e = ((uint64_t)seq_of(n) << 56) | (1ull << 55) | (uint64_t)SCAN_BITS;
    volatile uint32_t *w = (volatile uint32_t *)&T.mb[B_CMD + (n & 255)];
    w[0] = (uint32_t)e;
    barrier();
    w[1] = (uint32_t)(e >> 32);
    barrier();
    issued++;
    if (result(n) != (uint16_t)SCAN_BITS) return -1;
    std::string p = std::string(path) + ".regs";
    FILE *f = fopen(p.c_str(), "w");
    if (!f) { perror(p.c_str()); return -1; }
    const char *mod = "";
    size_t bit = 0;
    auto files = [&](const char *m) {        // the module's register files, after its registers
        size_t k = 0;
        for (int i = 0; i < SCAN_LD_FILES; i++)
            for (int e = 0; e < scan_ld[i].n; e++, k++)
                if (!strcmp(scan_ld[i].mod, m))
                    fprintf(f, "%s[%d] %x\n", scan_ld[i].name, e, (unsigned)(uint32_t)T.mb[B_LOAD + k]);
    };
    for (int i = 0; i < SCAN_REGS; i++) {
        uint64_t x = 0;
        for (int b = scan_map[i].bits - 1; b >= 0; b--, bit++)
            if (b < 64 && ((T.mb[B_SCAN + bit / 64] >> (63 - bit % 64)) & 1)) x |= 1ull << b;
        if (!strcmp(scan_map[i].mod, "pad")) continue;
        if (strcmp(mod, scan_map[i].mod)) {
            if (*mod) files(mod);
            fprintf(f, "@%s\n", mod = scan_map[i].mod);
        }
        fprintf(f, "%s %llx\n", scan_map[i].name, (unsigned long long)x);
    }
    if (*mod) files(mod);
    fclose(f);
    printf("[hps] saved %d registers (%d bits) at %llu microinstructions -> %s\n",
           SCAN_REGS, SCAN_BITS, (unsigned long long)S.rtl_cycles, p.c_str());
    return 0;
}

// Another world, or a save, in the running one's place without a reload of
// the core: what a new run of this program does, in this one (the simulation;
// on the board DoradoOne.sh starts the program again).
int hps_restart(const hps_config *cfg) {
    const hps_transport t = T;
    if (M) dorado_machine_destroy(M);
    M = nullptr;
    issued = completed = 0;
    cyc0 = bb0 = 0;
    status_rxloads = my_rxloads = status_tx = last_tx = 0;
    status_drained = shipped = false;
    shipped_gen = 0;
    memset(&S, 0, sizeof S);
    written_pages.clear();
    have40 = false;
    // (the hooks of the run before would send the new start's own writes to a stopped core)
    dorado_host_read_hook = nullptr;
    dorado_host_write_hook = nullptr;
    dorado_machine_step_hook = nullptr;
    dorado_machine_host_only = 0;
    dorado_machine_display_microcode = 0;
    return hps_start(cfg, &t);
}

int hps_start(const hps_config *cfg, const hps_transport *t) {
    CFG = *cfg;
    T = *t;
    // a save whose files are still being written (hps_save's child): wait for it
    {
        const std::string mark = std::string(CFG.snapshot) + ".writing";
        for (int i = 0; i < 1800 && access(mark.c_str(), F_OK) == 0; i++) {
            if (i == 0) fprintf(stderr, "[hps] %s is still being written; waiting\n", CFG.snapshot);
            usleep(100000);
        }
    }
    // a save's own disk (hps_save), in the world's disk's place
    {
        static std::string own;
        own = std::string(CFG.snapshot) + (CFG.pack ? ".pack" : ".pdi");
        if (FILE *f = (CFG.pack || CFG.pdi) ? fopen(own.c_str(), "rb") : nullptr) {
            fclose(f);
            (CFG.pack ? CFG.pack : CFG.pdi) = own.c_str();
            fprintf(stderr, "[hps] the disk is the save's own: %s\n", own.c_str());
        }
    }
    if (!CFG.quantum) CFG.quantum = 1;
    // registers and load records made from this snapshot: storage is its own too
    if (CFG.derive && access((std::string(CFG.snapshot) + ".regs").c_str(), R_OK) != 0) CFG.storage_image = nullptr;
    dorado_machine_config mc;
    dorado_machine_config_default(&mc);
    mc.boot_keys_count = 0;
    mc.boot_keys[mc.boot_keys_count++] = DORADO_KEY_NONE;
    mc.alto_ether_boot = 0;
    mc.eth_boot_110 = CFG.eb;
    mc.germ_path = CFG.germ;
    mc.pilot_disk_pdi[0] = CFG.pdi;
    mc.disk_pack[0] = nullptr;                           // (pack_open: the host holds no copy of it)
    mc.ftp_root = CFG.ftp_root;
    M = dorado_machine_create(&mc);
    if (!M) { fprintf(stderr, "[hps] machine create failed\n"); return 1; }
    if (CFG.pack && pack_open(CFG.pack) != 0) return 1;
    if (dorado_machine_restore(M, CFG.snapshot) != 0) {
        fprintf(stderr, "[hps] restore %s failed\n", CFG.snapshot);
        return 1;
    }
    if (CFG.ftp_root) dorado_machine_set_ftp_source(M, mc.ftp_sysout, mc.ftp_root);
    if (CFG.pdi) (void)dorado_machine_set_pilot_disk(M, 0, CFG.pdi);
    C = (dorado_cpu *)dorado_machine_cpu(M);
    ETH = dorado_machine_ethernet(M);
    dorado_memory *mem = dorado_machine_memory(M);
    if (mem->storage_words > T.storage_words) {
        fprintf(stderr, "[hps] snapshot storage %zu words > %zu\n",
                (size_t)mem->storage_words, T.storage_words);
        return 1;
    }

    // The core may be running a world already (this program started again
    // without a reload of the core): ask the mailbox to stop it and wait for
    // a go, which it answers by clearing the control beat. Nothing of the
    // new world is written before that -- the old one is still using storage.
    // A core just loaded answers the same way; one without the restart (an
    // older bitstream) does not, and is taken to be waiting.
    {
        T.mb[B_CTRL] = MB_RESTART;
        barrier();
        const double t0 = mono_s();
        unsigned long waits = 0;
        while ((uint32_t)T.mb[B_CTRL] != 0 && (mono_s() - t0 < 3.0 || waits < 200000)) { T.wait(T.ctx); waits++; }
        if ((uint32_t)T.mb[B_CTRL] != 0 && CFG.need_core) {
            fprintf(stderr, "[hps] no core answered the restart (not loaded, or not this core)\n");
            T.mb[B_CTRL] = 0;
            return 3;
        }
        if ((uint32_t)T.mb[B_CTRL] != 0)
            fprintf(stderr, "[hps] the core did not answer the restart; taken to be waiting for a go\n");
        else
            fprintf(stderr, "[hps] the core is stopped and waits for a go (%.0f ms)\n", (mono_s() - t0) * 1e3);
    }
    // The mailbox starts clear (a stale seq could look like a command), and
    // storage holds the snapshot's; then the core may go.
    for (size_t b = 0; b < B_END; b++) T.mb[b] = 0;
    // storage: the native run's image when given (the core's init/ came from
    // that run; this restore may be a wasm32 bake that differs in a word),
    // else the snapshot's -- and the host's own copy follows it either way
    if (CFG.storage_image) {
        FILE *sf = fopen(CFG.storage_image, "rb");
        if (!sf) { perror(CFG.storage_image); return 1; }
        size_t n = fread(mem->storage, sizeof(uint16_t), mem->storage_words, sf);
        fclose(sf);
        if (n != mem->storage_words) {
            fprintf(stderr, "[hps] %s: %zu of %zu words\n", CFG.storage_image, n, (size_t)mem->storage_words);
            return 1;
        }
        fprintf(stderr, "[hps] storage from %s\n", CFG.storage_image);
    }
    // A core that takes the saved registers by scan starts with an EMPTY
    // cache (its row arrays are not in the chain: tools/gen_scan.py), so
    // the words the snapshot holds only in dirty cache lines go to storage
    // here, as a writeback would put them.
    if (CFG.derive || access((std::string(CFG.snapshot) + ".regs").c_str(), R_OK) == 0) {
        unsigned lines = 0;
        for (int r = 0; r < DM_CACHE_ROWS; r++)
            for (int w = 0; w < DM_CACHE_WAYS; w++) {
                dorado_cache_line line = mem->cache[r].ways[w];      // (a copy: the store invalidates)
                if (!line.valid || !line.dirty) continue;
                const uint32_t va = (line.tag << 10) | ((uint32_t)r << 4);
                for (int i = 0; i < DM_CACHE_LINE_W; i++)
                    dorado_storage_store_at_va(mem, va + (uint32_t)i, line.data[i]);
                written_pages.push_back((uint16_t)(va >> 8));
                lines++;
            }
        fprintf(stderr, "[hps] %u dirty cache lines written to storage (the core starts with an empty cache)\n", lines);
    }
    for (size_t i = 0; i < mem->storage_words; i++) T.storage[i] = mem->storage[i];
    // The pack, as dorado_rtl_io's sector engine reads it: a record per sector
    // in the pack's flat CHS order, header | label | data, ceil(words / 4)
    // beats each, word 4b+l in lane l (drive 0 only).
    if (CFG.pack) {
        if (!T.pack) { fprintf(stderr, "[hps] no pack window\n"); return 1; }
        const dorado_disk_geometry *g = &PK.geometry;
        const int hw = dorado_disk_geom_header_words(g);
        const int lw = dorado_disk_geom_label_words(g);
        const int dw = dorado_disk_geom_data_words(g);
        const size_t stride = pack_stride();
        if ((size_t)PK_SECTORS * stride > T.pack_beats) {
            fprintf(stderr, "[hps] pack %d sectors x %zu beats > the %zu-beat window\n",
                    PK_SECTORS, stride, T.pack_beats);
            return 1;
        }
        FILE *f = fopen(CFG.pack, "rb");
        if (!f) { perror(CFG.pack); return 1; }
        const size_t words = (size_t)(hw + lw + dw);
        std::vector<uint8_t> buf(2 * (words + DORADO_DISK_DUMMY_WORDS));
        for (int i = 0; i < PK_SECTORS; i++) {
            if (fread(buf.data(), 1, buf.size(), f) != buf.size()) {
                fprintf(stderr, "[hps] pack %s: short read at sector %d\n", CFG.pack, i);
                fclose(f);
                return 1;
            }
            uint16_t rec[4 * 260] = {0};
            const uint8_t *q = buf.data() + 2 * DORADO_DISK_DUMMY_WORDS;
            for (size_t k = 0; k < words; k++) rec[k] = (uint16_t)(q[2*k] | q[2*k+1] << 8);
            for (size_t b = 0; b < stride; b++)
                T.pack[(size_t)i * stride + b] = (uint64_t)rec[4*b] | (uint64_t)rec[4*b+1] << 16 |
                                                 (uint64_t)rec[4*b+2] << 32 | (uint64_t)rec[4*b+3] << 48;
        }
        fclose(f);
        fprintf(stderr, "[hps] pack %s: %d sectors (%d/%d/%d words, %zu beats each) loaded\n",
                CFG.pack, PK_SECTORS, hw, lw, dw, stride);
    }
    // DORADO_HPS_RM_OR="addr:bits,addr:bits" (octal): OR bits into RM
    // registers once. Queued before the go, so the core applies them at its
    // first microinstruction: a display-mode change (hps_main, Smalltalk)
    // then takes effect at the end of the checkpoint's own field, before
    // any whole field is drawn the old way.
    if (const char *e = getenv("DORADO_HPS_RM_OR")) {
        while (*e) {
            char *end;
            unsigned a = (unsigned)strtoul(e, &end, 8);
            if (*end != ':') break;
            unsigned v = (unsigned)strtoul(end + 1, &end, 8);
            if (v) issue(AP_RM_OR, a, (uint16_t)v, 1, 0);
            if (*end != ',') break;
            e = end + 1;
        }
    }
    // The saved machine's registers, for a core that does not have them
    // compiled in (DORADO_SCAN): <snapshot>.regs (tools/gen_scan.py --regs)
    // packed in the scan chain's order (scan_map.h), first register's top
    // bit first, into the scan region; the control word says how many bits.
    // A core with the registers compiled in ignores the count.
    uint64_t scan_bits = load_scan_regs();
    barrier();
    T.mb[B_CTRL + 1] = CFG.world_cfg;
    barrier();
    T.mb[B_CTRL] = MB_MAGIC | (scan_bits << 32);
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
    // DORADO_HPS_SAVE0=<path>: save at once (a restore's fidelity: against the save it loaded)
    if (const char *s0 = getenv("DORADO_HPS_SAVE0")) hps_save(s0);
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
    // what DORADO_HPS_RM_OR (below, queued before go) left in the registers
    if (const char *e = getenv("DORADO_HPS_RM_OR")) {
        while (*e) {
            char *end;
            unsigned a = (unsigned)strtoul(e, &end, 8);
            if (*end != ':') break;
            strtoul(end + 1, &end, 8);
            fprintf(stderr, "[hps] RM[%o] is %06o\n", a, result(issue(AP_RM_OR, a, 0, 1, 0)));
            if (*end != ',') break;
            e = end + 1;
        }
    }
    return 0;
}

// The FPGA's RM[0..n-1] into the host's copy (diagnostics: the PDA dump
// reads the current PSB and NWW from it). An OR of 0 is a read.
void hps_sync_rm(unsigned n) {
    for (unsigned i = 0; i < n && i < RM_SIZE; i++) {
        C->RM[i] = result(issue(AP_RM_OR, i, 0, 1, 0));
        rm_prev[i] = C->RM[i];          // (NWW too: the next push sends none of it)
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
    // Every receive fill sent must arrive: the receiver counts the fills it
    // applied (15 bits). Once the mailbox has finished one, the receiver
    // takes it at once unless a WaitForBOP walk holds it, which is
    // microseconds, so a lag across 50 ms of polls is a lost fill.
    {
        static double lag_since;
        bool lag = status_rxloads == my_rxloads &&
                   (uint16_t)((S.rx_applied ^ my_rxloads) & 0x7FFF) != 0;
        double now = mono_s();
        if (!lag) lag_since = 0;
        else if (lag_since == 0) lag_since = now;
        else if (now - lag_since > 0.05) {
            S.rx_lost++;
            printf("[hps] RX LOST: %u fills sent, the receiver applied %u (15 bits)\n",
                   (unsigned)my_rxloads, (unsigned)S.rx_applied);
            lag_since = now + 10.0;          // report again in 10 s, not every poll
        }
    }
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
    if (cold_held && S.host_bb_cycles >= cold_next) {
        cold_next = S.host_bb_cycles + 1000000;             // about a field
        const unsigned tt = dorado_machine_display(M)->terminal_task;
        // (Cedar's own: the CSB at LONG[420B] names a DCB with the long-bitmap
        // seal, TerminalHeadDorado.mesa -- what the emulator paints the login
        // from, where the board must have the display microcode run)
        const uint16_t dcb = guest(0420);
        if (dorado_machine_alto_display_active(M) || (dcb > 1 && guest((uint32_t)dcb + 2) == 0177423)) {
            if (!cold_wakes)
                fprintf(stderr, "[hps] cold start: the world has a display list at BaseBoard cycle %llu; waking task %o\n",
                        (unsigned long long)S.host_bb_cycles, tt);
            if (tt == DORADO_DISPLAY_TASK_DHT || tt == DORADO_DISPLAY_TASK_AHT)
                issue(AP_WAKEUP_OR, 0, (uint16_t)(1u << tt), 1, 0);
            if (++cold_wakes >= 8) cold_held = false;
        }
    }
    retire(1);                              // let queued commands finish
    // DORADO_HPS_SAVE=<microinstructions from the start>,<path>: save the machine once there
    {
        static const char *sv = getenv("DORADO_HPS_SAVE");
        static int left = 1;
        unsigned long long at = 0; char sp[400];
        if (sv && left && sscanf(sv, "%llu,%399s", &at, sp) == 2 && S.rtl_cycles - cyc0 >= at) {
            left--;
            hps_save(sp);
        }
    }
    return S.rtl_cycles;
}

int hps_snapshot_pgm(const char *path) {
    int px = dorado_machine_render_display_list(M);
    dorado_display_snapshot_pgm(dorado_machine_display(M), path);
    return px;
}

const hps_stats *hps_get_stats(void) { return &S; }
void *hps_machine(void) { return M; }
