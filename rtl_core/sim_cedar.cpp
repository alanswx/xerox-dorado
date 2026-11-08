// =============================================================================
// sim_cedar.cpp — Cedar checkpoint lock-step harness for dorado_rtl_top
// =============================================================================
// Restores the C emulator's Cedar desktop checkpoint into a full C machine
// (dorado_machine: CPU, memory, devices AND the host services -- germ disk
// bridge, Pilot timer, Cedar keyboard/field notify, Ethernet and the STP
// server), copies the machine state into the RTL, and runs the two in
// lock-step one microinstruction at a time.
//
// The split is the one the FPGA will use: the RTL is the Dorado, the C
// machine supplies everything outside it.
//   - Slow I/O: each C device's answer to this microinstruction (Input data
//     and parity, IOAtten, whether the addressed device takes Output) is
//     captured by wrapping the C I/O registry and handed to the RTL CPU on
//     its external I/O port (io_bridge_en = 0).
//   - Host writes: every host-side write into storage or the cache
//     (dorado_host_write_hook) is mirrored into the RTL.
//   - Host register pokes (the Pilot timer ORs NWW = RM[0]; device wakeups
//     land in wakeup_pending): before each step, any CPU register that
//     differs from the RTL can only have been changed by the host since the
//     last compare, so it is copied over and counted.
// The counts printed at the end are the host port's requirements.
//
// Usage (from rtl_core/):
//   obj_cedar/Vdorado_rtl_top [steps] [snap] [pdi]
// Paths are relative to dorado/ (DORADO_DIR overrides). CEDAR_LS=1 drives
// the verify-cedar-ls input: click the CommandTool, then type a List of a
// directory on the in-process file server, which exercises the Ethernet
// and STP host services.
// =============================================================================

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>

#include <verilated.h>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"
#include "sim_census.h"

extern "C" {
#include "cpu.h"
#include "memory.h"
#include "microcode.h"
#include "mb.h"
#include "disasm.h"
#include "io.h"
#include "machine.h"
#include "typetext.h"
#include "ethernet.h"
#include "disk.h"
}

#include "cosim_sync.h"
#include "Vdorado_rtl_top__Dpi.h"

static Vdorado_rtl_top *top;
static dorado_machine  *M;
static dorado_cpu      *C;
static dorado_memory   *MEM;
static dorado_io       *IO;
// SIM_DISK_RTL=1: the RTL's own Trident controller is the disk (see below)
static bool            disk_rtl = false;
static const uint16_t  DSK_BIT = 1u << 014;   // the DSK task's wakeup
static dorado_disk_controller *CD = nullptr;  // the C machine's Trident controller (Alto worlds)
static bool sync_disk();

// ---- slow-I/O capture --------------------------------------------------------
struct io_cap {
    bool     read_valid;
    uint16_t read_val;
    int      read_bad;
    bool     atten_valid;
    int      atten;
};
static io_cap cap;

struct io_wrap {
    dorado_io_device        dev;
    const dorado_io_device *orig;
};

static std::map<std::string, uint64_t> io_use;   // "read|write|atten name task tioa" -> count
static void io_count(const char *kind, const io_wrap *w, int task, uint8_t tioa) {
    char k[96];
    snprintf(k, sizeof k, "%-6s %-10s task %2d tioa 0%03o", kind, w->dev.name ? w->dev.name : "?", task, tioa);
    io_use[k]++;
}
static uint16_t wrap_read(void *ctx, int task, int subtask, uint8_t tioa, int *bad) {
    io_wrap *w = (io_wrap *)ctx;
    io_count("read", w, task, tioa);
    int b = 0;
    uint16_t v = w->orig->read(w->orig->ctx, task, subtask, tioa, &b);
    if (bad) *bad = b;
    cap.read_valid = true;
    cap.read_val = v;
    cap.read_bad = b;
    return v;
}
static void wrap_write(void *ctx, int task, int subtask, uint8_t tioa, uint16_t data) {
    io_wrap *w = (io_wrap *)ctx;
    io_count("write", w, task, tioa);
    w->orig->write(w->orig->ctx, task, subtask, tioa, data);
}
static int wrap_atten(void *ctx, int task, uint8_t tioa) {
    io_wrap *w = (io_wrap *)ctx;
    int a = w->orig->attention(w->orig->ctx, task, tioa);
    io_count("atten", w, task, tioa);
    cap.atten_valid = true;
    cap.atten = a ? 1 : 0;
    return a;
}

static std::map<const dorado_io_device *, io_wrap *> wraps;
static void wrap_io_registry() {
    for (int t = 0; t < DORADO_IO_TASKS; t++) {
        for (int a = 0; a < DORADO_IO_ADDRS; a++) {
            const dorado_io_device *d = IO->cells[t][a];
            if (!d) continue;
            bool is_wrap = false;
            for (auto &kv : wraps) if (&kv.second->dev == d) is_wrap = true;
            if (is_wrap) continue;
            io_wrap *w = wraps[d];
            if (!w) {
                w = new io_wrap();
                w->orig = d;
                w->dev.read = d->read ? wrap_read : nullptr;
                w->dev.write = d->write ? wrap_write : nullptr;
                w->dev.attention = d->attention ? wrap_atten : nullptr;
                w->dev.ctx = w;
                w->dev.name = d->name;
                wraps[d] = w;
            }
            IO->cells[t][a] = &w->dev;
        }
    }
}

// ---- host writes -------------------------------------------------------------
struct host_write { uint32_t va; uint16_t value; int kind; };
static std::vector<host_write> host_writes;
static void host_write_hook(struct dorado_memory *, uint32_t va, uint16_t value, int kind) {
    host_writes.push_back({va, value, kind});
}

// The munch at `va` in the RTL: storage behind it and the cache row holding it.
static void mirror_host_write(uint32_t va) {
    const dorado_map_entry *e = dorado_map_get(MEM, dorado_map_index(va));
    size_t phys = (e->wp && e->dirty)
                ? (size_t)va
                : (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
    size_t base = phys & ~(size_t)15;
    for (size_t i = 0; i < 16; i++) {
        if (base + i < MEM->storage_words && base + i < COSIM_STORAGE_WORDS)
            R_STOR(storage)[base + i] = MEM->storage[base + i];
    }
    int r = (int)((va >> 4) & DM_CACHE_ROW_MASK);
    for (int w = 0; w < DM_CACHE_WAYS; w++) {
        const dorado_cache_line *l = &MEM->cache[r].ways[w];
        R_MEM(cache_tag)[r][w] = l->tag & 0x3FFFFu;
        R_MEM(cache_valid)[r][w] = l->valid ? 1 : 0;
        R_MEM(cache_dirty)[r][w] = l->dirty ? 1 : 0;
        R_MEM(cache_wp)[r][w] = l->wp ? 1 : 0;
        R_MEM(cache_vacant)[r][w] = l->vacant ? 1 : 0;
        R_MEM(cache_being_loaded)[r][w] = l->being_loaded ? 1 : 0;
        R_MEM(cache_lru)[r][w] = MEM->cache[r].lru[w] & 3;
        for (int k = 0; k < DM_CACHE_LINE_W; k++)
            R_CW(w)[CW_IDX(r, k)] = l->data[k];
    }
}

// ---- host register pokes -----------------------------------------------------
struct poke_stats {
    uint64_t rm = 0, stk = 0, wakeup = 0, ready = 0, resched = 0, junk = 0;
    uint64_t host_write_words = 0, host_write_steps = 0;
    std::map<int, uint64_t> rm_by_addr;
};
static poke_stats ps;

static void copy_host_pokes() {
    for (int i = 0; i < RM_SIZE; i++) {
        if (R_CPU(RM)[i] != C->RM[i]) {
            R_CPU(RM)[i] = C->RM[i];
            ps.rm++;
            ps.rm_by_addr[i]++;
        }
    }
    for (int i = 0; i < STK_SIZE; i++) {
        if (R_CPU(STK)[i] != C->STK[i]) { R_CPU(STK)[i] = C->STK[i]; ps.stk++; }
    }
    {
        // with the RTL disk, its own wakeup (task 014) is the RTL's to raise
        const uint16_t keep = disk_rtl ? DSK_BIT : 0;
        const uint16_t want = (uint16_t)((C->wakeup_pending & ~keep) | (R_CPU(wakeup_pending) & keep));
        if (R_CPU(wakeup_pending) != want) {
            R_CPU(wakeup_pending) = want;
            ps.wakeup++;
        }
    }
    if (R_CPU(ready) != C->ready) { R_CPU(ready) = C->ready; ps.ready++; }
    if (R_CPU(reschedule_pending) != C->reschedule_pending) {
        R_CPU(reschedule_pending) = C->reschedule_pending;
        ps.resched++;
    }
    if (R_CPU(junk_tw_enabled) != (C->junk_tw_enabled ? 1 : 0) ||
        R_CPU(junk_tw_countdown) != C->junk_tw_countdown) {
        R_CPU(junk_tw_enabled) = C->junk_tw_enabled ? 1 : 0;
        R_CPU(junk_tw_countdown) = C->junk_tw_countdown;
        ps.junk++;
    }
}

static bool     failed = false;

// ---- the host port ------------------------------------------------------------
// By default the host's changes reach the RTL through its host port (hp_*,
// the AP_* operations in dorado_rtl_pkg), as the HPS will drive it on the
// MiSTer; CEDAR_BACKDOOR=1 copies them into the arrays instead. Each C
// store routine has its own cache effect, so a write is classified by what
// it did: a resident line that the C dropped was dorado_storage_store_at_va
// (AP_STORE_INV); otherwise machine_store_va / dorado_poke_va (AP_STORE). A
// line the C filled (dorado_memory_host_io_write) has no port operation.
enum { AP_STORE = 1, AP_STORE_INV = 2, AP_RM_OR = 4, AP_WAKEUP_OR = 5, AP_RESCHED = 6 };
struct port_stats {
    uint64_t ops = 0, clocks = 0, store = 0, store_inv = 0, rm_or = 0, wakeup = 0, resched = 0;
    uint64_t unported = 0;          // changes the port cannot express (backdoor-copied)
};
static port_stats pst;
static bool use_backdoor = false;

static uint16_t hp_op(int op, uint32_t va, uint16_t wd, int cache, int we = 1) {
    top->hp_hold = 1;
    top->hp_req = 1;
    top->hp_op = op;
    top->hp_we = we;
    top->hp_cache = cache;
    top->hp_va = va & 0x0FFFFFFFu;
    top->hp_wd = wd;
    for (int i = 0; i < 4096; i++) {
        top->clk = 1; top->eval(); top->clk = 0; top->eval();
        pst.clocks++;
        if (top->hp_done) {
            top->hp_req = 0;
            pst.ops++;
            return top->hp_rd;
        }
    }
    fprintf(stderr, "[cedar] host port: op %d va 0%o never done\n", op, va);
    failed = true;
    top->hp_req = 0;
    return 0;
}

// Each write in the order the C made it, with the cache effect of the
// routine that made it (memory.h DORADO_HOST_WRITE_*).
static void port_host_write(const host_write &w) {
    switch (w.kind) {
    case DORADO_HOST_WRITE_INVALIDATE:
        hp_op(AP_STORE_INV, w.va, w.value, 0);
        pst.store_inv++;
        break;
    case DORADO_HOST_WRITE_UPDATE:
        hp_op(AP_STORE, w.va, w.value, 1);
        pst.store++;
        break;
    default:                                // a fill: no port operation
        pst.unported++;
        mirror_host_write(w.va);
        break;
    }
}

static void port_host_pokes() {
    for (int i = 0; i < RM_SIZE; i++) {
        uint16_t r = R_CPU(RM)[i], c = C->RM[i];
        if (r == c) continue;
        ps.rm++;
        ps.rm_by_addr[i]++;
        if (r & ~c) { R_CPU(RM)[i] = c; pst.unported++; continue; }   // a bit cleared
        hp_op(AP_RM_OR, (uint32_t)i, (uint16_t)(c & ~r), 0);
        pst.rm_or++;
    }
    for (int i = 0; i < STK_SIZE; i++) {
        if (R_CPU(STK)[i] != C->STK[i]) { R_CPU(STK)[i] = C->STK[i]; ps.stk++; pst.unported++; }
    }
    if (((R_CPU(wakeup_pending) ^ C->wakeup_pending) & ~(disk_rtl ? DSK_BIT : 0)) != 0) {
        const uint16_t keep = disk_rtl ? DSK_BIT : 0;
        uint16_t r = R_CPU(wakeup_pending) & ~keep, c = C->wakeup_pending & ~keep;
        ps.wakeup++;
        if (r & ~c) { R_CPU(wakeup_pending) = (uint16_t)(c | (R_CPU(wakeup_pending) & keep)); pst.unported++; }
        else { hp_op(AP_WAKEUP_OR, 0, (uint16_t)(c & ~r), 0); pst.wakeup++; }
    }
    if (R_CPU(reschedule_pending) != C->reschedule_pending) {
        ps.resched++;
        hp_op(AP_RESCHED, 0, (uint16_t)C->reschedule_pending, 0);
        pst.resched++;
    }
    if (R_CPU(ready) != C->ready) { R_CPU(ready) = C->ready; ps.ready++; pst.unported++; }
    if (R_CPU(junk_tw_enabled) != (C->junk_tw_enabled ? 1 : 0) ||
        R_CPU(junk_tw_countdown) != C->junk_tw_countdown) {
        R_CPU(junk_tw_enabled) = C->junk_tw_enabled ? 1 : 0;
        R_CPU(junk_tw_countdown) = C->junk_tw_countdown;
        ps.junk++;
        pst.unported++;
    }
}

// ---- split mode (CEDAR_SPLIT=1) -----------------------------------------------
// The MiSTer arrangement, in simulation: the RTL is the whole Dorado, with its
// own slow-I/O devices (io_bridge_en = 1), and the C machine never steps its
// CPU (DORADO_STEP_HOOK_SKIP) -- it supplies only the host services and the
// Pup/STP server. They meet only at the host port and the Ethernet receive
// buffer:
//   - host reads (dorado_visible_word_at_va) come back through hp_* (the
//     read hook), host writes go through it in order with their kind;
//   - RM and reschedule_pending: the host's changes are pushed as AP_RM_OR /
//     AP_RESCHED. Before each host pass the C copies of RM[0] (NWW) and
//     reschedule_pending are refreshed from the RTL, since the timer tests
//     NWW before ORing; that read is a backdoor here and AP_RM_OR with mask 0
//     on the HPS;
//   - each packet the C server queues is copied into the RTL's receive buffer
//     (hx_rx_*) once the C's interpacket hold has run out, and the C sees the
//     RTL's read position, so it queues the next when the RTL has drained it;
//   - a packet the RTL transmits goes to the C server.
// There is no reference to compare against; the run is judged by what Cedar
// does (the listing over STP, the screen).
static bool split = false;
static uint64_t steps = 0, max_steps = 0;
static uint16_t split_rm_prev[RM_SIZE];
static uint8_t  split_resched_prev;
static unsigned long eth_shipped_gen = 0;
static bool     eth_shipped = false;
static uint64_t eth_rx_shipped = 0, eth_tx_forwarded = 0, split_reads = 0;
static uint64_t rtl_tx_seen = 0;
static int      eth_debug_left = 0;

static uint16_t split_read_hook(const struct dorado_memory *, uint32_t va) {
    split_reads++;
    return hp_op(0 /* AP_ASSIST read */, va, 0, 1, 0);
}

static void split_refresh_regs() {
    C->RM[0] = R_CPU(RM)[0];
    C->reschedule_pending = R_CPU(reschedule_pending);
    memcpy(split_rm_prev, C->RM, sizeof split_rm_prev);
    split_resched_prev = C->reschedule_pending;
}

static void split_push_regs() {
    for (int i = 0; i < RM_SIZE; i++) {
        uint16_t d = (uint16_t)(C->RM[i] & ~split_rm_prev[i]);
        if (split_rm_prev[i] & ~C->RM[i]) pst.unported++;   // the host cleared a bit
        if (d) {
            hp_op(AP_RM_OR, (uint32_t)i, d, 0);
            pst.rm_or++;
            ps.rm++;
            ps.rm_by_addr[i]++;
        }
    }
    if (C->reschedule_pending != split_resched_prev) {
        hp_op(AP_RESCHED, 0, (uint16_t)C->reschedule_pending, 0);
        pst.resched++;
        ps.resched++;
    }
}

static void clock_once() {
    top->clk = 1; top->eval(); top->clk = 0; top->eval();
    pst.clocks++;
}

static void split_ethernet() {
    dorado_ethernet *eth = dorado_machine_ethernet(M);
    if (dorado_ethernet_rx_generation != eth_shipped_gen &&
        eth->rx_count > 0 && eth->rx_hold == 0) {
        size_t n = eth->rx_count < 4096 ? eth->rx_count : 4096;
        top->hp_hold = 1;
        for (size_t i = 0; i < n; i++) {
            top->hx_rx_wr_en = 1;
            top->hx_rx_wr_addr = (uint16_t)i;
            top->hx_rx_wr_data = eth->rx_words[i];
            top->hx_rx_wr_att = eth->rx_attention[i] ? 1 : 0;
            clock_once();
        }
        top->hx_rx_wr_en = 0;
        top->hx_rx_load = 1;
        top->hx_rx_load_count = (uint16_t)n;
        clock_once();
        top->hx_rx_load = 0;
        eth_shipped_gen = dorado_ethernet_rx_generation;
        eth_shipped = true;
        eth_rx_shipped++;
        eth_debug_left = getenv("CEDAR_ETH_DEBUG") ? 40 : 0;
        if (eth_debug_left)
            fprintf(stderr, "[eth] step %llu: shipped %zu words (type 0%o), rx_on=%d hold=%u no_wake=%d\n",
                    (unsigned long long)steps, n, n > 3 ? eth->rx_words[3] : 0,
                    (int)R_IO(eth_rx_on), (unsigned)R_IO(eth_rx_hold), (int)R_IO(eth_no_wakeups));
    }
    if (eth_debug_left && steps % 2000 == 0) {
        eth_debug_left--;
        fprintf(stderr, "[eth]   step %llu: rx %d/%d on=%d hold=%u wire=%u no_wake=%d ctask=%u "
                "wakeup=%04x ready=%04x TPC7=0%o TPC6=0%o\n",
                (unsigned long long)steps, (int)R_IO(eth_rx_pos), (int)R_IO(eth_rx_count),
                (int)R_IO(eth_rx_on), (unsigned)R_IO(eth_rx_hold), (unsigned)R_IO(eth_rx_wire_timer),
                (int)R_IO(eth_no_wakeups), (unsigned)top->dbg_ctask,
                (unsigned)R_CPU(wakeup_pending), (unsigned)R_CPU(ready),
                (unsigned)R_CPU(task_tpc)[7], (unsigned)R_CPU(task_tpc)[6]);
    }
    if (eth_shipped && dorado_ethernet_rx_generation == eth_shipped_gen) {
        size_t pos = (size_t)R_IO(eth_rx_pos);
        bool was = eth->rx_pos >= eth->rx_count;
        eth->rx_pos = pos < eth->rx_count ? pos : eth->rx_count;
        if (!was && eth->rx_pos >= eth->rx_count)
            dorado_ethernet_rx_consumed(eth);   // what eth_read does at the last word
    }
    // A packet the RTL's transmitter finished goes to the server.
    uint64_t txp = R_IO(eth_tx_packets);
    if (txp != rtl_tx_seen) {
        rtl_tx_seen = txp;
        int n = top->eth_tx_packet_words;     // eth_tx_count is zeroed at end of packet
        if (n > 1024) n = 1024;
        std::vector<uint16_t> pkt((size_t)n);
        for (int i = 0; i < n; i++) pkt[(size_t)i] = R_IO(eth_tx_words)[i];
        dorado_ethernet_direct_transmit(eth, pkt.data(), (size_t)n);
        eth_tx_forwarded++;
    }
}

// The C display controller's state (what Cedar's boot microcode loaded into
// the DDC: HRam, NLCB, the terminal task, raster control, the line
// sequencer), into the RTL's -- or, with ck_f open, into checkpoint_io.svh.
static FILE *ck_f;
static void ds_emit(const char *n, int i, uint64_t v) {
    if (i < 0) fprintf(ck_f, "            %s = 64'h%llx;\n", n, (unsigned long long)v);
    else       fprintf(ck_f, "            %s[%d] = 64'h%llx;\n", n, i, (unsigned long long)v);
}
#define DS(n, v)     do { if (ck_f) ds_emit(#n, -1, (uint64_t)(v)); else R_IO(n) = (v); } while (0)
#define DSA(n, i, v) do { if (ck_f) ds_emit(#n, (i), (uint64_t)(v)); else R_IO(n)[i] = (v); } while (0)
static void sync_display() {
    const dorado_display *d = dorado_machine_display(M);
    DS(ddc_statics, d->statics);
    DS(ddc_riob, d->riob);
    DS(ddc_terminal_task, d->terminal_task & 15);
    DS(ddc_ram_keep, d->ram_keep ? 1 : 0);
    DS(ddc_ram_addr, d->ram_addr);
    for (int i = 0; i < 512; i++) DSA(ddc_hram, i, d->hram[i] & 7);
    for (int i = 0; i < 16; i++) DSA(ddc_nlcb, i, d->nlcb[0][i]);
    DS(ddc_nlcb_line, d->nlcb_line);
    DS(ddc_nlcb_field_odd, d->nlcb_field_odd ? 1 : 0);
    DS(ddc_nlcb_cursor_x, d->nlcb_cursor_x);
    DS(ddc_nlcb_cursor_lo, d->nlcb_cursor_lo);
    DS(ddc_scan_line, d->scan_line);
    DS(ddc_scan_pixel, d->scan_pixel);
    DS(ddc_frame_count, d->frame_count);
    for (int c = 0; c < 2; c++) {
        DSA(ddc_next_wcb_flag, c, d->next_wcb_flag[c] ? 1 : 0);
        DSA(ddc_current_wcb_flag, c, d->current_wcb_flag[c] ? 1 : 0);
        DSA(ddc_wcb_draw_x, c, d->wcb_draw_x[c]);
        DSA(ddc_wcb_draw_y, c, d->wcb_draw_y[c]);
        const dorado_display_ddc_line *pl = &d->ddc_pending_line[c], *cl = &d->ddc_current_line[c];
        DSA(ddc_pend_valid, c, pl->valid ? 1 : 0);
        DSA(ddc_pend_odd, c, pl->odd ? 1 : 0);
        DSA(ddc_pend_line, c, pl->line);
        DSA(ddc_pend_lmarg, c, pl->lmarg);
        DSA(ddc_pend_width, c, pl->width);
        DSA(ddc_pend_ptr, c, pl->ptr);
        DSA(ddc_pend_scan, c, pl->scan);
        DSA(ddc_cur_valid, c, 0);            // the half-gathered line's words are not carried
        DSA(ddc_cur_odd, c, cl->odd ? 1 : 0);
        DSA(ddc_cur_abs_y, c, cl->absolute_y ? 1 : 0);
        DSA(ddc_cur_y, c, cl->y);
        DSA(ddc_cur_line, c, cl->line);
        DSA(ddc_cur_lmarg, c, cl->lmarg);
        DSA(ddc_cur_width, c, cl->width);
        DSA(ddc_cur_ptr, c, cl->ptr);
        DSA(ddc_cur_scan, c, cl->scan);
        DSA(ddc_cur_word_count, c, 0);
    }
    DS(ddc_fifo_a_head, d->fifo_a_head);
    DS(ddc_fifo_a_tail, d->fifo_a_tail);
    DS(ddc_fifo_b_head, d->fifo_b_head);
    DS(ddc_fifo_b_tail, d->fifo_b_tail);
    DS(ddc_raster_taskcmd, d->raster_taskcmd);
    DS(ddc_raster_lt_enabled, d->raster_lt_enabled ? 1 : 0);
    DS(ddc_raster_next_channel, d->raster_next_channel & 3);
    for (int c = 0; c < 4; c++) {
        DSA(ddc_raster_next_wt_flag, c, d->raster_next_wt_flag[c] ? 1 : 0);
        DSA(ddc_raster_current_wt_flag, c, d->raster_current_wt_flag[c] ? 1 : 0);
    }
    DS(ddc_boot_button_scanlines, d->boot_button_scanlines);
    for (int i = 0; i < 5; i++) DSA(ddc_kbd_words, i, d->keyboard_words[i]);
    if (!ck_f)
        printf("[cedar] display: terminal task 0%o, statics 0%o, raster lt %d, frame %llu\n",
               d->terminal_task, d->statics, d->raster_lt_enabled,
               (unsigned long long)d->frame_count);
}
#undef DS
#undef DSA

// The C Ethernet device's state at the checkpoint, into the RTL's.
static void split_sync_ethernet() {
    dorado_ethernet *eth = dorado_machine_ethernet(M);
    R_IO(eth_tx_on) = eth->tx_on ? 1 : 0;
    R_IO(eth_tx_eop) = eth->tx_eop ? 1 : 0;
    R_IO(eth_tx_cntdwn) = eth->tx_cntdwn ? 1 : 0;
    R_IO(eth_rx_on) = eth->rx_on ? 1 : 0;
    R_IO(eth_no_wakeups) = eth->no_wakeups ? 1 : 0;
    R_IO(eth_rx_hold) = eth->rx_hold;
    R_IO(eth_rx_wire_timer) = eth->rx_wire_timer;
    R_IO(eth_world_rx_words) = eth->world_rx_words;
    size_t n = eth->rx_count < 4096 ? eth->rx_count : 4096;
    for (size_t i = 0; i < n; i++) {
        R_IO(eth_rx_words)[i] = eth->rx_words[i];
        R_IO(eth_rx_attention)[i] = eth->rx_attention[i] ? 1 : 0;
    }
    R_IO(eth_rx_count) = (int)n;
    R_IO(eth_rx_pos) = (int)(eth->rx_pos < n ? eth->rx_pos : n);
    eth_shipped_gen = dorado_ethernet_rx_generation;
    eth_shipped = n > 0;
    rtl_tx_seen = R_IO(eth_tx_packets);
    printf("[cedar] split: Ethernet rx_on=%d tx_on=%d rx %zu/%zu hold=%u local host 0%o\n",
           eth->rx_on, eth->tx_on, eth->rx_pos, eth->rx_count, eth->rx_hold, eth->local_host);
}

static int split_hook(dorado_machine *, int after) {
    if (!after) {
        if (!host_writes.empty()) {
            ps.host_write_steps++;
            ps.host_write_words += host_writes.size();
            for (const host_write &w : host_writes) port_host_write(w);
            host_writes.clear();
        }
        split_push_regs();
        return DORADO_STEP_HOOK_SKIP;
    }
    split_ethernet();
    top->hp_hold = 0;
    rtl_step(top);
    steps++;
    if (top->dbg_halted) {
        fprintf(stderr, "[cedar] split: RTL halted (reason %u) at pc=0%o task=%u\n",
                (unsigned)top->dbg_halt_reason, (unsigned)top->dbg_real_pc, (unsigned)top->dbg_ctask);
        failed = true;
        return 1;
    }
    split_refresh_regs();
    if (getenv("CEDAR_ETH_TRACE")) {          // the RTL device, as DORADO_ETH_TRACE shows the C one
        static int p_pos = -1, p_on = -1, p_txon = -1, p_nw = -1;
        int pos = R_IO(eth_rx_pos), on = R_IO(eth_rx_on), txon = R_IO(eth_tx_on), nw = R_IO(eth_no_wakeups);
        if (pos != p_pos && pos > 0 && pos <= R_IO(eth_rx_count))
            fprintf(stderr, "RTLETH step=%llu read word %d/%d = %06o att=%d hold=%u\n",
                    (unsigned long long)steps, pos, (int)R_IO(eth_rx_count),
                    (unsigned)R_IO(eth_rx_words)[pos - 1], (int)R_IO(eth_rx_attention)[pos - 1],
                    (unsigned)R_IO(eth_rx_hold));
        if (on != p_on || txon != p_txon || nw != p_nw)
            fprintf(stderr, "RTLETH step=%llu ctl rx_on=%d tx_on=%d no_wake=%d rx %d/%d\n",
                    (unsigned long long)steps, on, txon, nw, pos, (int)R_IO(eth_rx_count));
        p_pos = pos; p_on = on; p_txon = txon; p_nw = nw;
    }
    if (steps % 1000000 == 0) {
        fprintf(stderr, "  [split] %llu steps, pc=0%o task=%u, host writes %llu, reads %llu, rx %llu, tx %llu\n",
                (unsigned long long)steps, (unsigned)top->dbg_real_pc, (unsigned)top->dbg_ctask,
                (unsigned long long)ps.host_write_words, (unsigned long long)split_reads,
                (unsigned long long)eth_rx_shipped, (unsigned long long)eth_tx_forwarded);
    }
    return steps >= max_steps ? 1 : 0;
}

// ---- the FPGA checkpoint (CEDAR_DUMP_INIT=<dir>) --------------------------------
// The RTL state just restored, for a MiSTer build that powers up in it
// (dorado_rtl_cpu / dorado_rtl_io `ifdef DORADO_CHECKPOINT): the block RAMs as
// $readmemh images, and every register sync_c_to_rtl sets -- plus the
// Ethernet device's -- as assignments the reset tasks include. Storage is not
// written: the HPS program restores the same snapshot and loads it into DDR.
static void ck_emit(const char *name, int i, int j, uint64_t v) {
    // the enum-typed registers take a cast
    const char *ty = "64";
    if (!strcmp(name, "pipe_kind") || !strcmp(name, "last_fault_ref_kind") ||
        !strcmp(name, "last_ref_kind"))
        ty = "dm_ref_kind_t";
    else if (!strcmp(name, "last_fault"))
        ty = "dm_fault_kind_t";
    char val[48];
    if (!strcmp(ty, "64")) snprintf(val, sizeof val, "64'h%llx", (unsigned long long)v);
    else                   snprintf(val, sizeof val, "%s'(%llu)", ty, (unsigned long long)v);
    if (i < 0)      fprintf(ck_f, "        %s = %s;\n", name, val);
    else if (j < 0) fprintf(ck_f, "        %s[%d] = %s;\n", name, i, val);
    else            fprintf(ck_f, "        %s[%d][%d] = %s;\n", name, i, j, val);
}
#define D0(mac, n)       ck_emit(#n, -1, -1, (uint64_t)mac(n))
#define D1(mac, n, i)    ck_emit(#n, i, -1, (uint64_t)mac(n)[i])
#define D2(mac, n, r, w) ck_emit(#n, r, w, (uint64_t)mac(n)[r][w])
static void dump_checkpoint(const char *dir) {
    auto dump = [&](const char *name, auto get, int n, int digits) {
        std::string path = std::string(dir) + "/" + name;
        FILE *f = fopen(path.c_str(), "w");
        if (!f) { perror(path.c_str()); exit(1); }
        for (int i = 0; i < n; i++) fprintf(f, "%0*x\n", digits, (unsigned)get(i));
        fclose(f);
    };
    dump("im_iw0.hex",       [&](int i){ return R_CPU(im_iw0)[i]; }, IM_SIZE, 4);
    dump("im_iw1.hex",       [&](int i){ return R_CPU(im_iw1)[i]; }, IM_SIZE, 4);
    dump("im_iw2.hex",       [&](int i){ return R_CPU(im_iw2)[i]; }, IM_SIZE, 4);
    dump("im_present.hex",   [&](int i){ return R_CPU(im_present)[i]; }, IM_SIZE, 1);
    dump("ifum_lo.hex",      [&](int i){ return R_CPU(ifum_lo)[i]; }, IFUM_SIZE, 4);
    dump("ifum_hi.hex",      [&](int i){ return R_CPU(ifum_hi)[i]; }, IFUM_SIZE, 4);
    dump("ifum_present.hex", [&](int i){ return R_CPU(ifum_present)[i]; }, IFUM_SIZE, 1);
    dump("rm.hex",           [&](int i){ return R_CPU(RM)[i]; }, RM_SIZE, 4);
    dump("stk.hex",          [&](int i){ return R_CPU(STK)[i]; }, STK_SIZE, 4);
    dump("alufm.hex",        [&](int i){ return R_CPU(alufm)[i]; }, ALUFM_SIZE, 2);
    dump("alufm_present.hex",[&](int i){ return R_CPU(alufm_present)[i]; }, ALUFM_SIZE, 1);
    dump("map_rp.hex",       [&](int i){ return R_MEM(map_rp)[i]; }, DM_MAP_ENTRIES, 4);
    dump("map_wp.hex",       [&](int i){ return R_MEM(map_wp)[i]; }, DM_MAP_ENTRIES, 1);
    dump("map_dirty.hex",    [&](int i){ return R_MEM(map_dirty)[i]; }, DM_MAP_ENTRIES, 1);
    dump("map_ref.hex",      [&](int i){ return R_MEM(map_ref)[i]; }, DM_MAP_ENTRIES, 1);
    dump("cache_w0.hex",     [&](int i){ return R_MEM(cache_w0)[i]; }, DM_CACHE_ROWS * DM_CACHE_LINE_W, 4);
    dump("cache_w1.hex",     [&](int i){ return R_MEM(cache_w1)[i]; }, DM_CACHE_ROWS * DM_CACHE_LINE_W, 4);
    dump("cache_w2.hex",     [&](int i){ return R_MEM(cache_w2)[i]; }, DM_CACHE_ROWS * DM_CACHE_LINE_W, 4);
    dump("cache_w3.hex",     [&](int i){ return R_MEM(cache_w3)[i]; }, DM_CACHE_ROWS * DM_CACHE_LINE_W, 4);

    std::string path = std::string(dir) + "/checkpoint_cpu.svh";
    ck_f = fopen(path.c_str(), "w");
    if (!ck_f) { perror(path.c_str()); exit(1); }
    fprintf(ck_f, "        // generated by rtl_core/sim_cedar.cpp (CEDAR_DUMP_INIT); do not edit\n");
    D0(R_CPU, real_PC);
    D0(R_CPU, prev_PC);
    D0(R_CPU, T);
    D0(R_CPU, Q);
    D0(R_CPU, Link);
    D0(R_CPU, Cnt);
    D0(R_CPU, ShC);
    D0(R_CPU, RBase);
    D0(R_CPU, MemBase);
    D0(R_CPU, MemBX);
    D0(R_CPU, TIOA);
    D0(R_CPU, StkP);
    D0(R_CPU, stk_ovf);
    D0(R_CPU, stk_und);
    D0(R_CPU, cpreg);
    D0(R_CPU, io_bad_parity);
    D0(R_CPU, io_atten_at_issue);
    D0(R_CPU, b_link_read);
    D0(R_CPU, b_link_read_valid);
    D0(R_CPU, b_link_read_age);
    D0(R_CPU, link_at_issue);
    D0(R_CPU, alu_zero);
    D0(R_CPU, alu_lt0);
    D0(R_CPU, alu_carry);
    D0(R_CPU, alu_overflow);
    D0(R_CPU, ctask);
    D0(R_CPU, ready);
    D0(R_CPU, wakeup_pending);
    for (int i = 0; i < 2; i++) D1(R_CPU, wakeup_pipe, i);
    D0(R_CPU, tasking_on);
    D0(R_CPU, tasking_resume_delay);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_t, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_tpc, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_link, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_membase, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_rbase, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_tioa, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_alu_zero, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_alu_lt0, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_alu_carry, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_alu_overflow, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_subtask, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_dispatch, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_md, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_md_valid, i);
    for (int i = 0; i < 16; i++) D1(R_CPU, task_md_ready, i);
    D0(R_CPU, ifu_pcf);
    D0(R_CPU, ifu_pcx);
    D0(R_CPU, ifu_opcode);
    D0(R_CPU, ifu_alpha);
    D0(R_CPU, ifu_beta);
    D0(R_CPU, ifu_insset);
    D0(R_CPU, ifu_idcnt);
    D0(R_CPU, ifu_length);
    D0(R_CPU, ifu_sign);
    D0(R_CPU, ifu_packed_a);
    D0(R_CPU, ifu_type_pause);
    D0(R_CPU, ifu_type_jump);
    D0(R_CPU, ifu_n);
    D0(R_CPU, ifu_active);
    D0(R_CPU, ifu_warmup);
    D0(R_CPU, ifu_test);
    D0(R_CPU, ifu_test_pending);
    D0(R_CPU, ifu_test_count);
    for (int i = 0; i < 8; i++) D1(R_CPU, ifu_test_queue, i);
    D0(R_CPU, ifu_saved_stkp);
    D0(R_CPU, ifu_save_stkp_pending);
    D0(R_CPU, ifu_dispatch_rbase);
    D0(R_CPU, ifu_dispatch_count);
    D0(R_CPU, reschedule_pending);
    D0(R_CPU, reschedule_cond);
    D0(R_CPU, brk_opcode);
    D0(R_CPU, brk_pending);
    D0(R_CPU, event_cnt_a);
    D0(R_CPU, event_cnt_b);
    D0(R_CPU, event_cnt_ctrl_hi);
    D0(R_CPU, event_cnt_ctrl_lo);
    D0(R_CPU, gen_io_mode);
    D0(R_CPU, evc_a_enable);
    D0(R_CPU, evc_a_event);
    D0(R_CPU, evc_a_tasksall);
    D0(R_CPU, evc_b_enable);
    D0(R_CPU, evc_b_event);
    D0(R_CPU, evc_b_tasksall);
    D0(R_CPU, evc_events);
    D0(R_CPU, tasksim);
    D0(R_CPU, tasksim_fired);
    D0(R_CPU, tasksim_loaded);
    D0(R_CPU, holdsim);
    D0(R_CPU, holdsim_loaded);
    D0(R_CPU, holdsim_delay);
    D0(R_CPU, holdsim_hold);
    D0(R_CPU, junk_tw_enabled);
    D0(R_CPU, junk_tw_countdown);
    D0(R_CPU, cycles);
    D0(R_CPU, halted);
    D0(R_CPU, halt_reason);
    for (int i = 0; i < DM_BR_COUNT; i++) D1(R_CPU, br, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_CPU, pipe_mapbuf_busy, i);
    D0(R_CPU, proc_srn);
    D0(R_CPU, mcr);
    D0(R_CPU, dmux_addr);
    D0(R_CPU, dmux_pending);
    D0(R_CPU, mapbuf_busy_slot);
    D0(R_CPU, mapbuf_busy_cycles);
    fclose(ck_f);

    // the memory section's (dorado_rtl_mem reset_memory)
    path = std::string(dir) + "/checkpoint_mem.svh";
    ck_f = fopen(path.c_str(), "w");
    if (!ck_f) { perror(path.c_str()); exit(1); }
    fprintf(ck_f, "        // generated by rtl_core/sim_cedar.cpp (CEDAR_DUMP_INIT); do not edit\n");
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_tag, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_valid, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_dirty, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_wp, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_vacant, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_being_loaded, r, w);
    for (int r = 0; r < DM_CACHE_ROWS; r++) for (int w = 0; w < DM_CACHE_WAYS; w++) D2(R_MEM, cache_lru, r, w);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_kind, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_task, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_subtask, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_ref_type, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_va, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_map_rp_pre, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_map_flags_pre, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_cache_flags, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_pipe4_errors, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_pipe4_syndrome, i);
    for (int i = 0; i < DM_PIPE_DEPTH; i++) D1(R_MEM, pipe_pipe4_quadword, i);
    D0(R_MEM, pipe_head);
    D0(R_MEM, asrn);
    D0(R_MEM, fault_count);
    D0(R_MEM, fault_first_srn);
    D0(R_MEM, fault_emulator);
    D0(R_MEM, last_fault);
    D0(R_MEM, last_fault_va);
    D0(R_MEM, last_fault_task);
    D0(R_MEM, last_fault_subtask);
    D0(R_MEM, last_fault_ref_kind);
    D0(R_MEM, mar);
    ck_emit("md_q", -1, -1, (uint64_t)R_MEM(md_q));
    D0(R_MEM, dbuf);
    D0(R_MEM, last_cache_row);
    D0(R_MEM, last_cache_way);
    D0(R_MEM, last_ref_kind);
    D0(R_MEM, last_ref_va);
    D0(R_MEM, last_ref_b);
    D0(R_MEM, last_ref_task);
    D0(R_MEM, last_ref_subtask);
    D0(R_MEM, last_ref_miss);
    D0(R_MEM, last_ref_latency);
    fclose(ck_f);

    path = std::string(dir) + "/checkpoint_io.svh";
    ck_f = fopen(path.c_str(), "w");
    if (!ck_f) { perror(path.c_str()); exit(1); }
    fprintf(ck_f, "            // generated by rtl_core/sim_cedar.cpp (CEDAR_DUMP_INIT); do not edit\n");
#define DI(n) fprintf(ck_f, "            %s = 64'h%llx;\n", #n, (unsigned long long)R_IO(n))
    DI(eth_tx_on); DI(eth_tx_eop); DI(eth_tx_cntdwn); DI(eth_rx_on); DI(eth_no_wakeups);
    DI(eth_rx_hold); DI(eth_rx_wire_timer); DI(eth_world_rx_words);
    DI(eth_rx_count); DI(eth_rx_pos);
#undef DI
    sync_display();
    if (CD) sync_disk();       // an Alto-world (Trident) disk: Smalltalk
    fclose(ck_f);
    dump("eth_rx_words.hex",     [&](int i){ return R_IO(eth_rx_words)[i]; }, 4096, 4);
    dump("eth_rx_attention.hex", [&](int i){ return R_IO(eth_rx_attention)[i]; }, 4096, 1);

    // the storage the core powers up beside, as raw little-endian words: the
    // ARM's dorado-hps loads DDR from it (its own restore is a wasm32 bake of
    // the world, which need not match this native one word for word)
    path = std::string(dir) + "/storage.bin";
    if (FILE *sf = fopen(path.c_str(), "wb")) {
        fwrite(MEM->storage, sizeof(uint16_t), MEM->storage_words, sf);
        fclose(sf);
    }

    path = std::string(dir) + "/start.txt";
    FILE *f = fopen(path.c_str(), "w");
    fprintf(f, "real_PC=0%o cycles=%llu ctask=%u ready=0x%x tasking_on=%u checkpoint=1\n",
            (unsigned)R_CPU(real_PC), (unsigned long long)R_CPU(cycles),
            (unsigned)R_CPU(ctask), (unsigned)R_CPU(ready), (unsigned)R_CPU(tasking_on));
    fclose(f);
    ck_f = nullptr;
    printf("[cedar] checkpoint for the FPGA written to %s (real_PC=0%o, cycles %llu)\n",
           dir, (unsigned)R_CPU(real_PC), (unsigned long long)R_CPU(cycles));
}
#undef D0
#undef D1
#undef D2

// ---- the step hook -----------------------------------------------------------
static uint16_t pc_before;
static uint8_t  task_before, tioa_before;
static uint64_t rtl_storage_check_every = 0;
static uint64_t overlapped_steps = 0;    // retires with the next execute beside them (C_PIPE)
struct step_note { uint64_t step; uint16_t pc; uint8_t task; bool ov, poked; uint16_t rtl_pc; };
static step_note notes[16];
static bool poked_now = false;
static void print_notes() {
    for (int i = 0; i < 16; i++) {
        const step_note &n = notes[(steps + i) & 15];
        if (n.step) fprintf(stderr, "  step %llu pc 0%o task %u ov %d poked %d rtl_pc 0%o\n",
                            (unsigned long long)n.step, n.pc, n.task, n.ov, n.poked, n.rtl_pc);
    }
}

// The RTL runs a step once the C machine has also run its devices after it
// (the next call's !after half): by then the host's pokes and writes before
// the next step are known, and the RTL may execute the next instruction
// beside this one's finish (C_PIPE) only when there are none -- they belong
// before it. The C processor is compared as it was right after its step
// (c_after), before the devices ran.
static bool        rtl_pending = false;
static dorado_cpu  c_after;
static uint16_t    pend_rd, pend_pc;
static uint8_t     pend_bad, pend_has_write, pend_atten, pend_task;
static bool        ov_hold_env = false;

static bool host_pokes_pending() {
    if (!host_writes.empty()) return true;
    if (memcmp(C->RM, c_after.RM, sizeof(C->RM)) != 0) return true;
    if (memcmp(C->STK, c_after.STK, sizeof(C->STK)) != 0) return true;
    if (((C->wakeup_pending ^ c_after.wakeup_pending) & ~(disk_rtl ? DSK_BIT : 0)) != 0) return true;
    return C->ready != c_after.ready ||
           C->reschedule_pending != c_after.reschedule_pending ||
           C->junk_tw_enabled != c_after.junk_tw_enabled ||
           C->junk_tw_countdown != c_after.junk_tw_countdown;
}

static int rtl_catch_up(bool hold_next) {
    rtl_pending = false;
    top->io_attached = 1;
    top->io_read_data = pend_rd;
    top->io_read_bad_parity = pend_bad;
    top->io_has_write_in = pend_has_write;
    top->io_atten_in = pend_atten;
    top->ov_hold = (ov_hold_env || hold_next) ? 1 : 0;
    rtl_step(top);
    steps++;
    notes[steps & 15] = step_note{steps, pend_pc, pend_task, R_CPU(snap_valid) != 0, poked_now,
                                  (uint16_t)R_CPU(real_PC)};
    if (!compare_c_rtl(&c_after, top, steps, "cedar", pend_pc, pend_task, 0)) {
        print_notes();
        failed = true;
        return 1;
    }
    // C_PIPE: the next instruction executed beside this one's finish. Its
    // reference is in the memory section already, so the section is compared
    // at the next retire that had no execute beside it. The host's writes
    // (made by the C's devices since) are compared after they reach the RTL.
    if (R_CPU(snap_valid)) {
        overlapped_steps++;
    } else {
        rtl_settle(top);
        if (host_writes.empty() &&
            !compare_c_rtl(C, top, steps, "cedar", pend_pc, pend_task, 1)) {
            print_notes();
            failed = true;
            return 1;
        }
    }
    if (steps % 1000000 == 0) {
        fprintf(stderr, "  [cedar] %llu steps lock-step, pc=0%o task=%u, host writes %llu words\n",
                (unsigned long long)steps, (unsigned)C->real_PC, (unsigned)C->ctask,
                (unsigned long long)ps.host_write_words);
    }
    return 0;
}

// ---- the RTL Trident controller (SIM_DISK_RTL=1) ------------------------------
// The RTL's own disk answers the disk's slow I/O (disk_bridge_en) while the C
// devices answer the rest, and rotates on the C disk's clock (bb.cycles). Its
// sectors come from a private copy of the C machine's pack through DPI (the
// load-at-start / commit-at-end protocol a board sector engine implements),
// so the two disks never share media and a wrong write shows as a difference.
// The C disk's DSK wakeup is not poked into the RTL; the RTL raises its own.
// After every step the two controllers are compared field by field.
static dorado_disk_pack rtl_pack[DORADO_DISK_NUM_DRIVES];
static bool rtl_pack_ok[DORADO_DISK_NUM_DRIVES];
static uint64_t rtl_disk_loads = 0, rtl_disk_words_put = 0, disk_compares = 0;

// The pack image as the engine addresses it (dorado_rtl_io, the sector data
// engine): drive d at DISK_DRIVE_BEATS * d (DISK_PACK_BEAT_BASE is 0 here),
// a record per sector in the pack's flat CHS order, header | label | data,
// ceil(words / 4) beats each, word 4b+l in lane l.
static const uint32_t SIM_DRIVE_BEATS = 0x00800000u;
static bool rtl_beat_word(uint32_t addr, uint32_t lane, uint16_t **wp) {
    *wp = nullptr;
    uint32_t dv = addr / SIM_DRIVE_BEATS, off = addr % SIM_DRIVE_BEATS;
    if (dv >= (uint32_t)DORADO_DISK_NUM_DRIVES || !rtl_pack_ok[dv]) return false;
    dorado_disk_pack *p = &rtl_pack[dv];
    const int hw = dorado_disk_geom_header_words(&p->geometry);
    const int lw = dorado_disk_geom_label_words(&p->geometry);
    const int dw = dorado_disk_geom_data_words(&p->geometry);
    const uint32_t stride = (uint32_t)(hw + lw + dw + 3) / 4;
    const uint32_t rec = off / stride, wi = (off % stride) * 4 + lane;
    if (rec >= (uint32_t)p->num_sectors) return false;
    dorado_disk_sector *s = &p->sectors[rec];
    if (wi < (uint32_t)hw) *wp = &s->header[wi];
    else if (wi < (uint32_t)(hw + lw)) *wp = &s->label[wi - hw];
    else if (wi < (uint32_t)(hw + lw + dw)) *wp = &s->data[wi - hw - lw];
    return true;
}
extern "C" long long dpi_disk_beat_read(int addr) {
    uint64_t v = 0;
    rtl_disk_loads++;
    for (uint32_t l = 0; l < 4; l++) {
        uint16_t *w;
        if (rtl_beat_word((uint32_t)addr, l, &w) && w) v |= (uint64_t)*w << (16 * l);
    }
    return (long long)v;
}
extern "C" void dpi_disk_beat_write(int addr, long long data, int be) {
    for (uint32_t l = 0; l < 4; l++) {
        if (((unsigned)be >> (2 * l) & 3) != 3) continue;
        uint16_t *w;
        if (rtl_beat_word((uint32_t)addr, l, &w) && w) {
            *w = (uint16_t)((uint64_t)data >> (16 * l));
            rtl_disk_words_put++;
        }
    }
}

// A deep copy of the C machine's pack, as it stands after the restore.
static void copy_rtl_packs() {
    for (int d = 0; d < DORADO_DISK_NUM_DRIVES; d++) {
        const dorado_disk_pack *src = CD->drive[d].pack;
        rtl_pack_ok[d] = false;
        if (!src || !src->sectors) continue;
        rtl_pack[d] = *src;
        rtl_pack[d].path[0] = '\0';   // never saved
        size_t n = (size_t)src->num_sectors * sizeof(dorado_disk_sector);
        rtl_pack[d].sectors = (dorado_disk_sector *)malloc(n);
        memcpy(rtl_pack[d].sectors, src->sectors, n);
        rtl_pack_ok[d] = true;
    }
}

#define DS(n, v)     do { if (ck_f) ds_emit(#n, -1, (uint64_t)(v)); else R_IO(n) = (v); } while (0)
#define DSA(n, i, v) do { if (ck_f) ds_emit(#n, (i), (uint64_t)(v)); else R_IO(n)[i] = (v); } while (0)
// The C controller's state into the RTL's (dorado_rtl_io's disk_* registers),
// or, with ck_f open, into checkpoint_io.svh.
static bool sync_disk() {
    if (CD->read_stream_active || CD->write_stream_active) {
        fprintf(stderr, "[disk] a transfer is in progress at the checkpoint; not synced\n");
        return false;
    }
    DS(disk_selected_drive, CD->selected_drive & 3);
    for (int i = 0; i < 4; i++) {
        const dorado_disk_drive *d = &CD->drive[i];
        const dorado_disk_geometry *g = d->pack ? &d->pack->geometry : nullptr;
        DSA(disk_drv_online, i, d->online ? 1 : 0);
        DSA(disk_drv_selected, i, d->selected ? 1 : 0);
        DSA(disk_drv_read_only, i, d->read_only ? 1 : 0);
        DSA(disk_drv_head_ovfl, i, d->head_overflow ? 1 : 0);
        DSA(disk_drv_media, i, d->pack ? 1 : 0);
        DSA(disk_drv_cyls, i, g ? (g->cylinders & 0xFFF) : 0);
        if (g) {
            DSA(disk_drv_heads, i, g->heads & 0x3F);
            DSA(disk_drv_sectors, i, g->sectors & 0x7F);
            DSA(disk_drv_hdr_words, i, dorado_disk_geom_header_words(g) & 0xF);
            DSA(disk_drv_lbl_words, i, dorado_disk_geom_label_words(g) & 0x1F);
            DSA(disk_drv_data_words, i, dorado_disk_geom_data_words(g) & 0x7FF);
            DSA(disk_drv_is_diablo, i, (g->sectors == 29 && dorado_disk_geom_header_words(g) == 2 &&
                                            dorado_disk_geom_label_words(g) == 8 &&
                                            dorado_disk_geom_data_words(g) == 256) ? 1 : 0);
        }
        DSA(disk_drv_sectors_per_rev, i, d->sectors_per_revolution & 0x7F);
        DSA(disk_drv_subsector_cnt, i, d->subsector_count & 0x3F);
        DSA(disk_drv_cur_cyl, i, d->cur_cyl & 0xFFF);
        DSA(disk_drv_cur_head, i, d->cur_head & 0x3F);
        DSA(disk_drv_cur_sector, i, d->cur_sector & 0x7F);
        DSA(disk_drv_seek_in_prog, i, d->seek_in_progress);
        DSA(disk_drv_index_pulse, i, d->index_pulse ? 1 : 0);
    }
    DS(disk_control, CD->control);
    DS(disk_tag, CD->tag);
    DS(disk_muff_addr, CD->muff_addr);
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) DSA(disk_format_ram, i, CD->format_ram[i]);
    DS(disk_format_ram_addr, CD->format_ram_addr & 0xF);
    DS(disk_enable_run, CD->enable_run);
    DS(disk_debug_mode, CD->debug_mode);
    DS(disk_block_till_index, CD->block_till_index);
    DS(disk_active, CD->active);
    DS(disk_xfer_pending, CD->xfer_pending);
    DS(disk_restore_pending, CD->restore_pending);
    DS(disk_transfer_restore, CD->transfer_restore);
    DS(disk_restore_hdr_check, CD->restore_header_check);
    DS(disk_index_tw, CD->index_tw);
    DS(disk_sector_tw, CD->sector_tw);
    DS(disk_tag_tw, CD->tag_tw);
    DS(disk_rd_fifo_tw, CD->rd_fifo_tw);
    DS(disk_wr_fifo_tw, CD->wr_fifo_tw);
    DS(disk_compare_err, CD->compare_err);
    DS(disk_read_data_err, CD->read_data_err);
    DS(disk_fifo_underflow, CD->fifo_underflow);
    DS(disk_fifo_overflow, CD->fifo_overflow);
    for (int i = 0; i < DORADO_DISK_FIFO_WORDS; i++) DSA(disk_fifo, i, CD->fifo[i]);
    DS(disk_fifo_head, CD->fifo_head);
    DS(disk_fifo_tail, CD->fifo_tail);
    DS(disk_fifo_count, CD->fifo_count);
    DS(disk_read_stream_active, 0);
    DS(disk_read_block_framing, CD->read_block_framing);
    DS(disk_read_stream_index, CD->read_stream_index);
    DS(disk_write_stream_active, 0);
    DS(disk_write_block_framing, CD->write_block_framing);
    DS(disk_write_stream_index, CD->write_stream_index);
    DS(disk_current_block, CD->current_block & 7);
    DS(disk_current_block_op, CD->current_block_op & 3);
    DS(disk_current_block_words, CD->current_block_words);
    DS(disk_current_block_pos, CD->current_block_pos);
    DS(disk_current_block_prefix, CD->current_block_prefix);
    DS(disk_current_block_trailer, CD->current_block_trailer & 0xF);
    // dumping a checkpoint for the board: unarmed, so the RTL arms on its own
    // microinstruction count at its real 3600 RPM (C's clock is bb.cycles)
    DS(disk_next_sector_cycle, ck_f ? 0 : CD->next_sector_cycle);
    return true;
}

#undef DS
#undef DSA

// The disk engine works between ticks (io_busy holds the CPU's next
// instruction until it is done): clock it to idle before comparing, holding
// the CPU as rtl_settle does.
static void rtl_drain_disk() {
    if (!R_IO(disk_eng_busy) && !R_IO(disk_stage)) return;
    const uint8_t prev = top->hp_hold;
    top->hp_hold = 1;
    for (int i = 0; i < 65536 && (R_IO(disk_eng_busy) || R_IO(disk_stage)); i++) {
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        rtl_fabric_clocks++;
        if (top->step_done) rtl_step_seen = true;
    }
    if (R_IO(disk_eng_busy) || R_IO(disk_stage)) fprintf(stderr, "[disk] the engine is still busy after 65536 clocks\n");
    top->hp_hold = prev;
    top->eval();
}

// The two controllers field by field; prints every difference.
static bool compare_disk(uint64_t step) {
    disk_compares++;
    bool ok = true;
    auto chk = [&](const char *f, uint64_t c, uint64_t r) {
        if (c == r) return;
        if (ok) fprintf(stderr, "[disk] step %llu: the RTL disk differs from the C disk (pc_before=0%o task=%u):\n",
                        (unsigned long long)step, (unsigned)pend_pc, (unsigned)pend_task);
        fprintf(stderr, "    %-22s C=0%llo RTL=0%llo\n", f, (unsigned long long)c, (unsigned long long)r);
        ok = false;
    };
    const int sd = CD->selected_drive & 3;
    const dorado_disk_drive *d = &CD->drive[sd];
    chk("selected_drive", sd, R_IO(disk_selected_drive));
    chk("drv.cur_cyl", d->cur_cyl & 0xFFF, R_IO(disk_drv_cur_cyl)[sd]);
    chk("drv.cur_head", d->cur_head & 0x3F, R_IO(disk_drv_cur_head)[sd]);
    chk("drv.cur_sector", d->cur_sector & 0x7F, R_IO(disk_drv_cur_sector)[sd]);
    chk("drv.sectors_per_rev", d->sectors_per_revolution & 0x7F, R_IO(disk_drv_sectors_per_rev)[sd]);
    chk("drv.seek_in_progress", (uint64_t)(int64_t)d->seek_in_progress, (uint64_t)(int64_t)(int32_t)R_IO(disk_drv_seek_in_prog)[sd]);
    chk("drv.head_overflow", d->head_overflow ? 1 : 0, R_IO(disk_drv_head_ovfl)[sd]);
    chk("control", CD->control, R_IO(disk_control));
    chk("tag", CD->tag, R_IO(disk_tag));
    chk("muff_addr", CD->muff_addr, R_IO(disk_muff_addr));
    chk("format_ram_addr", CD->format_ram_addr & 0xF, R_IO(disk_format_ram_addr));
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) {
        char nm[32]; snprintf(nm, sizeof nm, "format_ram[%d]", i);
        chk(nm, CD->format_ram[i], R_IO(disk_format_ram)[i]);
    }
    chk("enable_run", CD->enable_run, R_IO(disk_enable_run));
    chk("debug_mode", CD->debug_mode, R_IO(disk_debug_mode));
    chk("block_till_index", CD->block_till_index, R_IO(disk_block_till_index));
    chk("active", CD->active, R_IO(disk_active));
    chk("xfer_pending", CD->xfer_pending, R_IO(disk_xfer_pending));
    chk("restore_pending", CD->restore_pending, R_IO(disk_restore_pending));
    chk("transfer_restore", CD->transfer_restore, R_IO(disk_transfer_restore));
    chk("index_tw", CD->index_tw, R_IO(disk_index_tw));
    chk("sector_tw", CD->sector_tw, R_IO(disk_sector_tw));
    chk("tag_tw", CD->tag_tw, R_IO(disk_tag_tw));
    chk("rd_fifo_tw", CD->rd_fifo_tw, R_IO(disk_rd_fifo_tw));
    chk("wr_fifo_tw", CD->wr_fifo_tw, R_IO(disk_wr_fifo_tw));
    chk("compare_err", CD->compare_err, R_IO(disk_compare_err));
    chk("read_data_err", CD->read_data_err, R_IO(disk_read_data_err));
    chk("fifo_underflow", CD->fifo_underflow, R_IO(disk_fifo_underflow));
    chk("fifo_overflow", CD->fifo_overflow, R_IO(disk_fifo_overflow));
    chk("fifo_count", (uint64_t)CD->fifo_count, (uint64_t)(int64_t)(int32_t)R_IO(disk_fifo_count));
    if (CD->fifo_count == (int)R_IO(disk_fifo_count)) {
        for (int i = 0; i < CD->fifo_count; i++) {
            char nm[32]; snprintf(nm, sizeof nm, "fifo+%d", i);
            chk(nm, CD->fifo[(CD->fifo_tail + i) % DORADO_DISK_FIFO_WORDS],
                R_IO(disk_fifo)[(R_IO(disk_fifo_tail) + i) % DORADO_DISK_FIFO_WORDS]);
        }
    }
    chk("read_stream_active", CD->read_stream_active, R_IO(disk_read_stream_active));
    chk("read_stream_index", (uint64_t)CD->read_stream_index, (uint64_t)(int64_t)(int32_t)R_IO(disk_read_stream_index));
    chk("write_stream_active", CD->write_stream_active, R_IO(disk_write_stream_active));
    chk("write_stream_index", (uint64_t)CD->write_stream_index, (uint64_t)(int64_t)(int32_t)R_IO(disk_write_stream_index));
    chk("current_block", CD->current_block & 7, R_IO(disk_current_block));
    chk("current_block_op", CD->current_block_op & 3, R_IO(disk_current_block_op));
    chk("current_block_words", CD->current_block_words, R_IO(disk_current_block_words));
    chk("current_block_pos", CD->current_block_pos, R_IO(disk_current_block_pos));
    chk("current_block_prefix", CD->current_block_prefix, R_IO(disk_current_block_prefix));
    chk("current_block_trailer", CD->current_block_trailer & 0xF, R_IO(disk_current_block_trailer));
    chk("next_sector_cycle", CD->next_sector_cycle, R_IO(disk_next_sector_cycle));
    return ok;
}

// The two packs, sector by sector (at the end of a run).
static uint64_t compare_packs() {
    uint64_t bad = 0;
    for (int dv = 0; dv < DORADO_DISK_NUM_DRIVES; dv++) {
        if (!rtl_pack_ok[dv] || !CD->drive[dv].pack) continue;
        const dorado_disk_pack *c = CD->drive[dv].pack;
        for (int i = 0; i < c->num_sectors; i++) {
            const dorado_disk_sector *a = &c->sectors[i], *b = &rtl_pack[dv].sectors[i];
            if (memcmp(a->header, b->header, sizeof a->header) || memcmp(a->label, b->label, sizeof a->label) ||
                memcmp(a->data, b->data, sizeof a->data)) {
                if (bad < 8) fprintf(stderr, "[disk] drive %d sector %d differs between the C and the RTL pack\n", dv, i);
                bad++;
            }
        }
    }
    return bad;
}

static int step_hook(dorado_machine *, int after) {
    if (!after) {
        bool rtl_ahead = false;
        if (rtl_pending) {
            bool hold = host_pokes_pending();
            if (disk_rtl) top->disk_time = dorado_machine_cycles(M);
            if (rtl_catch_up(hold)) return 1;
            if (disk_rtl) {
                // SIM_DISK_TRACE=from,to: both controllers' block state per step
                static long long tr_from = -1, tr_to = -1;
                static bool tr_init = false;
                if (!tr_init) { tr_init = true; if (const char *e = getenv("SIM_DISK_TRACE")) sscanf(e, "%lld,%lld", &tr_from, &tr_to); }
                if ((long long)steps >= tr_from && (long long)steps <= tr_to) {
                    fprintf(stderr, "[dtr] %llu pc=0%o t=%u io:%s tioa=0%o | C blk%u op%u pos%u/%u tr%u pf%u fifo%d rd%u wr%u wi%d act%u | R blk%u op%u pos%u/%u tr%u pf%u fifo%d rd%u wr%u wi%d act%u\n",
                        (unsigned long long)steps, (unsigned)pend_pc, (unsigned)pend_task,
                        cap.read_valid ? "IN" : "--", (unsigned)tioa_before,
                        CD->current_block, CD->current_block_op, CD->current_block_pos, CD->current_block_words,
                        CD->current_block_trailer, CD->current_block_prefix, CD->fifo_count,
                        CD->read_stream_active, CD->write_stream_active, CD->write_stream_index, CD->active,
                        (unsigned)R_IO(disk_current_block), (unsigned)R_IO(disk_current_block_op),
                        (unsigned)R_IO(disk_current_block_pos), (unsigned)R_IO(disk_current_block_words),
                        (unsigned)R_IO(disk_current_block_trailer), (unsigned)R_IO(disk_current_block_prefix),
                        (int)R_IO(disk_fifo_count), (unsigned)R_IO(disk_read_stream_active),
                        (unsigned)R_IO(disk_write_stream_active), (int)R_IO(disk_write_stream_index),
                        (unsigned)R_IO(disk_active));
                }
            }
            if (disk_rtl) rtl_drain_disk();
            if (disk_rtl && !R_CPU(snap_valid) && !compare_disk(steps)) {
                print_notes();
                failed = true;
                return 1;
            }
            // the next instruction executed beside the finish: allowed only
            // when the C's devices changed nothing, so there is nothing to
            // poke -- and a diff of the RTL against the C now would see that
            // execute's own effects (a reschedule cleared, say) as pokes
            rtl_ahead = R_CPU(snap_valid) != 0;
        }
        if (rtl_ahead) {
            poked_now = false;
            pc_before = C->real_PC;
            task_before = C->ctask;
            tioa_before = (uint8_t)C->TIOA;
            cap = io_cap{};
            return 0;
        }
        if (!host_writes.empty()) {
            ps.host_write_steps++;
            ps.host_write_words += host_writes.size();
            if (use_backdoor) {
                for (const host_write &w : host_writes) mirror_host_write(w.va);
            } else {
                for (const host_write &w : host_writes) port_host_write(w);
            }
            host_writes.clear();
        }
        uint64_t ops0 = pst.ops + ps.rm + ps.wakeup + ps.resched + ps.ready + ps.junk + ps.stk;
        if (use_backdoor) {
            copy_host_pokes();
        } else {
            top->hp_hold = 1;
            port_host_pokes();
            top->hp_hold = 0;
        }
        poked_now = (pst.ops + ps.rm + ps.wakeup + ps.resched + ps.ready + ps.junk + ps.stk) != ops0;
        pc_before = C->real_PC;
        task_before = C->ctask;
        tioa_before = (uint8_t)C->TIOA;
        cap = io_cap{};
        return 0;
    }
    // The C step has run: note its device answers for the RTL's step.
    c_after = *C;
    pend_rd = cap.read_valid ? cap.read_val : 0xFFFF;
    pend_bad = cap.read_valid ? (cap.read_bad ? 1 : 0) : 1;
    pend_has_write = dorado_io_has_write(IO, task_before, tioa_before) ? 1 : 0;
    pend_atten = cap.atten_valid ? cap.atten : 0;
    pend_pc = pc_before;
    pend_task = task_before;
    rtl_pending = true;
    if (steps + 1 >= max_steps) {
        rtl_catch_up(true);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    setvbuf(stdout, nullptr, _IOLBF, 0);
    max_steps = argc > 1 ? strtoull(argv[1], nullptr, 0) : 2000000;
    // SIM_WORLD=smalltalk: the Smalltalk-76 desktop (SmalltalkDorado.eb!1 =
    // DSemu, on the XM Smalltalk Trident pack), checkpoint from
    // `dorado --snapshot-out` after `Bootfrom xmsmall.boot`. Argument 3 is
    // then the Trident pack the checkpoint was taken against.
    const char *world = getenv("SIM_WORLD") ? getenv("SIM_WORLD") : "cedar";
    // SIM_WORLD=lisp: Interlisp-D Lyric (worlds/aemu.eb on its Trident pack,
    // the Leaf/STP server on ../chm/lisp/ftp-root), as make verify-lisp-serve
    // runs it; the caller sets DORADO_DISPM_PRESENT=1 and DORADO_FAKE_TIME.
    const bool lisp = !strcmp(world, "lisp");
    const bool smalltalk = !strcmp(world, "smalltalk");
    const bool alto_disk = smalltalk || lisp;     // a Trident-pack world
    if (!smalltalk && !lisp && strcmp(world, "cedar")) {
        fprintf(stderr, "SIM_WORLD=%s: want cedar, smalltalk or lisp\n", world);
        return 2;
    }
    const char *snap = argc > 2 ? argv[2] : smalltalk ? "build/good-packs/smalltalk76.snap"
                                          : lisp ? "build/good-packs/lisp-lyric-full.snap"
                                                 : "build/good-packs/cedar-desktop.snap";
    const char *pdi  = argc > 3 ? argv[3] : smalltalk ? "build/good-packs/smalltalk76.pack"
                                          : lisp ? "build/run-disks/lisp-lockstep.pack"
                                                 : "build/good-packs/cedar-desktop.pdi";
    const char *ddir = getenv("DORADO_DIR") ? getenv("DORADO_DIR") : "../dorado";
    if (chdir(ddir) != 0) { perror(ddir); return 2; }
    if (const char *m = getenv("CEDAR_STORAGE_CHECK_MASK"))
        cosim_storage_check_mask = strtoull(m, nullptr, 0);
    else
        cosim_storage_check_mask = (1u << 20) - 1;

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.boot_keys_count = 0;
    cfg.boot_keys[cfg.boot_keys_count++] = DORADO_KEY_NONE;   // --boot-reason disk
    cfg.alto_ether_boot = 0;                                   // --no-alto-boot
    if (smalltalk) {
        cfg.eth_boot_110 = "../chm/microcode/SmalltalkDorado.eb!1";
        cfg.disk_pack[0] = pdi;                                // --disk 0=PACK
    } else if (lisp) {
        cfg.eth_boot_110 = "worlds/aemu.eb";
        cfg.disk_pack[0] = pdi;
        cfg.ftp_root = "../chm/lisp/ftp-root";
    } else {
        cfg.eth_boot_110 = "../chm/dorado/CedarDorado.eb!6";
        cfg.germ_path = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
        cfg.pilot_disk_pdi[0] = pdi;
        cfg.ftp_root = "../chm/cedar/stp-root";
    }

    M = dorado_machine_create(&cfg);
    if (!M) { fprintf(stderr, "machine create failed\n"); return 1; }
    if (dorado_machine_restore(M, snap) != 0) { fprintf(stderr, "restore %s failed\n", snap); return 1; }
    if (!alto_disk) (void)dorado_machine_set_pilot_disk(M, 0, pdi);
    if (cfg.ftp_root) dorado_machine_set_ftp_source(M, cfg.ftp_sysout, cfg.ftp_root);

    C = (dorado_cpu *)dorado_machine_cpu(M);
    MEM = dorado_machine_memory(M);
    IO = dorado_machine_io(M);
    printf("[cedar] restored %s at BaseBoard cycle %llu: cpu cycles %llu, storage %zu words, pc=0%o task=%u\n",
           snap, (unsigned long long)dorado_machine_cycles(M), (unsigned long long)C->cycles, (size_t)MEM->storage_words,
           (unsigned)C->real_PC, (unsigned)C->ctask);
    if (MEM->storage_words > COSIM_STORAGE_WORDS) {
        fprintf(stderr, "[cedar] C storage %zu words > RTL %u\n",
                (size_t)MEM->storage_words, (unsigned)COSIM_STORAGE_WORDS);
        return 1;
    }

    top = new Vdorado_rtl_top;
    sync_c_to_rtl(C, top);
    ov_hold_env = getenv("RTL_OV_HOLD") != nullptr;   // no C_PIPE overlap (a PIPELINE build)
    top->io_bridge_en = 0;
    use_backdoor = getenv("CEDAR_BACKDOOR") != nullptr;
    if (!compare_c_rtl(C, top, 0, "cedar-sync", C->real_PC, C->ctask)) {
        fprintf(stderr, "[cedar] state differs right after the copy\n");
        return 1;
    }
    if (alto_disk) CD = dorado_machine_disk(M);
    if (getenv("SIM_DISK_RTL")) {
        CD = dorado_machine_disk(M);
        copy_rtl_packs();
        if (!sync_disk()) return 1;
        disk_rtl = true;
        top->disk_bridge_en = 1;
        top->disk_time_ext_en = 1;
        top->disk_time = dorado_machine_cycles(M);
        top->eval();
        if (!compare_disk(0)) {
            fprintf(stderr, "[disk] the RTL disk differs right after the sync\n");
            return 1;
        }
        // SIM_DISPLAY_SHADOW=1: the RTL DDC starts from the C display's state
        // and draws the world's own screen from the display tasks' Outputs and
        // fast-I/O munches, beside the C (its answers and wakeups stay the C's)
        if (getenv("SIM_DISPLAY_SHADOW")) {
            sync_display();
            top->eval();
            printf("[disk] the RTL DDC shadows the display\n");
        }
        printf("[disk] the RTL Trident controller is the disk: drive %d cyl %d head %d sector %d, "
               "%d sectors/rev, enable_run %d, next pulse at %llu\n",
               CD->selected_drive, CD->drive[CD->selected_drive].cur_cyl,
               CD->drive[CD->selected_drive].cur_head, CD->drive[CD->selected_drive].cur_sector,
               CD->drive[CD->selected_drive].sectors_per_revolution, CD->enable_run,
               (unsigned long long)CD->next_sector_cycle);
    }

    // CEDAR_C_ONLY=1: run the C machine alone (same script), to check the
    // input against the native emulator quickly. CEDAR_UNTIL=<BaseBoard
    // cycle> ends the run there.
    const bool c_only = getenv("CEDAR_C_ONLY") != nullptr;
    uint64_t until = getenv("CEDAR_UNTIL") ? strtoull(getenv("CEDAR_UNTIL"), nullptr, 0) : ~0ull;
    if (c_only) max_steps = ~0ull;
    split = getenv("CEDAR_SPLIT") != nullptr;
    if (split && !c_only) {
        top->io_bridge_en = 1;
        top->bootsrv_en = 0;
        top->assist_en = 0;
        // CEDAR_DISPLAY=1: wake the display tasks, so Cedar's own display
        // microcode draws through the RTL's display controller
        top->display_wake_gate = getenv("CEDAR_DISPLAY") ? 1 : 0;
        top->display_top_lines = 22;
        top->eth_local_host = dorado_machine_ethernet(M)->local_host;
        split_sync_ethernet();
        sync_display();
        // a Trident-pack world: the RTL's own disk, as on the board (its pack
        // copy behind the DPI model; the rotation unarmed, so it arms on the
        // RTL's microinstruction count at the real rate)
        if (CD && !disk_rtl) {
            copy_rtl_packs();
            if (sync_disk()) {
                R_IO(disk_next_sector_cycle) = 0;
                printf("[cedar] split: the RTL Trident controller is the disk\n");
            }
        }
        split_refresh_regs();
        if (const char *dir = getenv("CEDAR_DUMP_INIT")) {
            dump_checkpoint(dir);
            if (max_steps == 0) return 0;
        }
        dorado_host_write_hook = host_write_hook;
        dorado_host_read_hook = split_read_hook;
        dorado_machine_step_hook = split_hook;
        dorado_machine_host_only = 1;
    } else if (c_only && getenv("CEDAR_OPTRACE")) {
        // CEDAR_OPTRACE=lo,hi[,psb]: the C machine's task-0 Mesa opcodes in a
        // BaseBoard-cycle window, as sim_hps's HPS_OPTRACE prints the RTL's
        dorado_machine_step_hook = [](dorado_machine *m, int after) -> int {
            static uint64_t lo, hi; static unsigned want = 0; static bool parsed;
            static uint64_t last_disp;
            if (!parsed) { parsed = true; sscanf(getenv("CEDAR_OPTRACE"), "%llu,%llu,%o",
                                                (unsigned long long *)&lo, (unsigned long long *)&hi, &want); }
            if (!after) return 0;
            dorado_cpu *c = (dorado_cpu *)dorado_machine_cpu(m);
            uint64_t now = dorado_machine_cycles(m);
            if (c->ifu_dispatch_count != last_disp && now >= lo && now < hi && c->ctask == 0 &&
                (!want || (c->RM[1] & 0177777) == want))
                printf("[op] cyc %llu cb %09o pc %06o op %03o psb %06o sp %03o\n", (unsigned long long)now,
                       (unsigned)(dorado_machine_memory(m)->br[31] & 0x0FFFFFFFu), (unsigned)c->ifu_pcx,
                       (unsigned)c->ifu_opcode, (unsigned)(c->RM[1] & 0177777), (unsigned)c->StkP);
            last_disp = c->ifu_dispatch_count;
            return 0;
        };
    } else if (!c_only) {
        wrap_io_registry();
        dorado_host_write_hook = host_write_hook;
        dorado_machine_step_hook = step_hook;
    }

    // Scripted input, as dorado.c drives verify-cedar-ls: 2M-cycle frames,
    // the click at --type-at (+100M) run inline (move, 2M, press, hold
    // --key-hold 1.6M, release), the paste queued at --paste-at (+300M) and
    // pumped once per frame.
    // CEDAR_TYPE=<text> types it (\n = Return) at CEDAR_TYPE_AT (an absolute
    // BaseBoard cycle; default 300M past the restore), one key transition
    // every CEDAR_TYPE_PACE cycles (default 800K; the Alto worlds take 3M)
    std::string type_text;
    if (const char *t = getenv("CEDAR_TYPE")) {
        for (const char *q = t; *q; q++) {
            if (q[0] == '\\' && q[1] == 'n') { type_text += '\n'; q++; }
            else type_text += *q;
        }
    }
    const bool want_click = getenv("CEDAR_LS") != nullptr || getenv("CEDAR_CLICK") != nullptr;
    const bool ls = want_click || !type_text.empty();
    int cx = 700, cy = 733;          // CEDAR_CLICK=x,y: click there instead, and type nothing
    if (const char *cc = getenv("CEDAR_CLICK")) sscanf(cc, "%d,%d", &cx, &cy);
    const uint64_t c0 = dorado_machine_cycles(M);
    const uint64_t frame = 2000000ull;
    bool clicked = false, typed = false;
    static dorado_typequeue tq;
    auto run_to = [&](uint64_t target) {
        while (!failed && steps < max_steps && dorado_machine_cycles(M) < target) {
            uint64_t before = dorado_machine_uinstructions(M);
            dorado_machine_run_until(M, target);
            if (dorado_machine_uinstructions(M) == before) break;
        }
    };

    auto t0 = std::chrono::steady_clock::now();
    while (!failed && steps < max_steps && dorado_machine_cycles(M) < until) {
        uint64_t target = dorado_machine_cycles(M) + frame;
        uint64_t before = dorado_machine_uinstructions(M);
        run_to(target);
        if (dorado_machine_uinstructions(M) == before) {
            fprintf(stderr, "[cedar] the C machine made no progress\n");
            break;
        }
        uint64_t now = dorado_machine_cycles(M);
        if (!ls) continue;
        if (typed) dorado_typequeue_pump(&tq, M);
        if (!clicked && want_click && now >= c0 + 100000000ull) {
            clicked = true;
            printf("[cedar] click (%d,%d) at cycle %llu, step %llu\n", cx, cy,
                   (unsigned long long)now, (unsigned long long)steps);
            dorado_machine_set_mouse(M, cx, cy, 0);
            run_to(dorado_machine_cycles(M) + 2000000ull);
            dorado_machine_set_mouse(M, cx, cy, DORADO_MOUSE_LEFT);
            // CEDAR_CLICK_HOLD=<BaseBoard cycles>: how long the button stays
            // down (Smalltalk's browser wants ~40M, as verify-smalltalk-input's
            // --drag-hold; Cedar's --key-hold is 1.6M)
            const char *ch = getenv("CEDAR_CLICK_HOLD");
            run_to(dorado_machine_cycles(M) + (ch ? strtoull(ch, nullptr, 0) : 1600000ull));
            dorado_machine_set_mouse(M, cx, cy, 0);
        }
        const uint64_t type_at = getenv("CEDAR_TYPE_AT") ? strtoull(getenv("CEDAR_TYPE_AT"), nullptr, 0)
                                                          : c0 + 300000000ull;
        if (!typed && (!type_text.empty() || !getenv("CEDAR_CLICK")) && now >= type_at) {
            typed = true;
            printf("[cedar] typing at cycle %llu, step %llu\n",
                   (unsigned long long)now, (unsigned long long)steps);
            const uint64_t pace = getenv("CEDAR_TYPE_PACE") ? strtoull(getenv("CEDAR_TYPE_PACE"), nullptr, 0) : 800000ull;
            static std::string typing;   // the queue keeps a pointer to the text
            typing = type_text.empty() ? std::string("List [Cedar]<Cedar6.1>VersionMap>*\n") : type_text;
            dorado_typequeue_start(&tq, typing.c_str(), pace, now);
        }
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();

    if (getenv("CEDAR_CENSUS")) census_report(top);
    {
        // what is on the screen: the C machine's display list (its memory is
        // the RTL's, compared every step), and the RTL DDC's own framebuffer
        int c_px = dorado_machine_render_display_list(M);
        int rtl_px = 0;
        for (int i = 0; i < 1024 * 808; i++) rtl_px += R_IO(fb)[i] & 1;
        printf("[cedar] screen: %d display-list pixels (C), RTL DDC framebuffer %d lit\n", c_px, rtl_px);
        // RTL_FB_OUT=path.pbm: the RTL DDC framebuffer; C_FB_OUT=path.pgm: the C's
        if (const char *fbp = getenv("RTL_FB_OUT")) {
            if (FILE *pf = fopen(fbp, "wb")) {
                fprintf(pf, "P4\n1024 808\n");
                for (int i = 0; i < (1024 * 808) / 8; i++) {
                    int byte = 0;
                    for (int k = 0; k < 8; k++) byte = (byte << 1) | (R_IO(fb)[i * 8 + k] & 1);
                    fputc(byte, pf);
                }
                fclose(pf);
            }
        }
        if (const char *cp = getenv("C_FB_OUT")) {
            dorado_display_vblank(dorado_machine_display(M));
            dorado_display_snapshot_pgm(dorado_machine_display(M), cp);
        }
    }
    if (disk_rtl) {
        uint64_t bad = compare_packs();
        printf("[disk] RTL disk: %llu beats read, %llu words written, %llu step compares; "
               "C disk: %llu read starts, %llu sectors written; packs %s (%llu sectors differ)\n",
               (unsigned long long)rtl_disk_loads, (unsigned long long)rtl_disk_words_put,
               (unsigned long long)disk_compares, (unsigned long long)CD->read_stream_starts,
               (unsigned long long)CD->write_sectors_committed, bad ? "DIFFER" : "agree",
               (unsigned long long)bad);
        if (bad) failed = true;
    }
    printf("[cedar] %llu microinstructions in lock-step, %.1f s (%.0f/s), %.2f fabric clocks each\n",
           (unsigned long long)steps, secs, steps / secs,
           steps ? (double)rtl_fabric_clocks / steps : 0.0);
    printf("[cedar] host writes: %llu words over %llu steps\n",
           (unsigned long long)ps.host_write_words, (unsigned long long)ps.host_write_steps);
    printf("[cedar] C_PIPE: %llu of the retires had the next execute beside them\n", (unsigned long long)overlapped_steps);
    printf("[cedar] host register pokes: RM %llu, STK %llu, wakeup_pending %llu, ready %llu, "
           "reschedule %llu, junk %llu\n",
           (unsigned long long)ps.rm, (unsigned long long)ps.stk,
           (unsigned long long)ps.wakeup, (unsigned long long)ps.ready,
           (unsigned long long)ps.resched, (unsigned long long)ps.junk);
    if (!use_backdoor)
        printf("[cedar] host port: %llu operations in %llu fabric clocks -- store %llu, store+invalidate %llu, "
               "RM or %llu, wakeup %llu, reschedule %llu; not expressible %llu\n",
               (unsigned long long)pst.ops, (unsigned long long)pst.clocks,
               (unsigned long long)pst.store, (unsigned long long)pst.store_inv,
               (unsigned long long)pst.rm_or, (unsigned long long)pst.wakeup,
               (unsigned long long)pst.resched, (unsigned long long)pst.unported);
    for (auto &kv : io_use)
        printf("         slow I/O %s: %llu\n", kv.first.c_str(), (unsigned long long)kv.second);
    for (auto &kv : ps.rm_by_addr)
        printf("         RM[0%03o] poked %llu times\n", kv.first, (unsigned long long)kv.second);
    if (split)
        printf("[cedar] split: %llu host reads, %llu packets into the RTL, %llu out of it\n",
               (unsigned long long)split_reads, (unsigned long long)eth_rx_shipped,
               (unsigned long long)eth_tx_forwarded);
    if (!failed && !c_only && !split) {
        // Final full storage compare.
        for (size_t i = 0; i < MEM->storage_words; i++) {
            if (MEM->storage[i] != R_STOR(storage)[i]) {
                printf("[cedar] storage[0%zo] C=0%o RTL=0%o\n", i, MEM->storage[i], R_STOR(storage)[i]);
                failed = true;
                break;
            }
        }
    }
    if (const char *rtl = getenv("CEDAR_PGM_RTL")) {   // the RTL display controller's framebuffer
        FILE *f = fopen(rtl, "wb");
        int px = 0;
        if (f) {
            fprintf(f, "P5\n1024 808\n255\n");
            for (int i = 0; i < 1024 * 808; i++) {
                int b = R_IO(fb)[i] & 1;
                px += b;
                fputc(b ? 0 : 255, f);
            }
            fclose(f);
        }
        printf("[cedar] RTL display controller: %d pixels -> %s\n", px, rtl);
    }
    if (const char *raw = getenv("CEDAR_PGM_RAW")) {   // what the display controller drew
        dorado_display_snapshot_pgm(dorado_machine_display(M), raw);
        printf("[cedar] display controller framebuffer -> %s\n", raw);
    }
    if (const char *pgm = getenv("CEDAR_PGM")) {
        int px = dorado_machine_render_display_list(M);
        dorado_display_snapshot_pgm(dorado_machine_display(M), pgm);
        printf("[cedar] cycle %llu: %d display-list pixels -> %s\n",
               (unsigned long long)dorado_machine_cycles(M), px, pgm);
    }
    printf(failed ? "  [FAIL] Cedar lock-step\n" : "  [PASS] Cedar lock-step\n");
    (void)rtl_storage_check_every;
    return failed ? 1 : 0;
}
