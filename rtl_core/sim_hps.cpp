// =============================================================================
// sim_hps.cpp — The MiSTer Cedar configuration, rehearsed under Verilator
// =============================================================================
// dorado_rtl_top as the MiSTer Cedar build has it -- storage in DDR3
// (EXT_STORAGE), the HPS mailbox (HPS_MAILBOX), and powering up in the Cedar
// desktop checkpoint (DORADO_INIT + DORADO_CHECKPOINT, from
// rtl_core/init_cedar: sim_cedar.cpp CEDAR_DUMP_INIT) -- in front of the DDR
// model, with the HPS program (hps/hps_host.cpp) running in this process
// against the model's array. Nothing here reaches into the RTL: the only
// connection is DDR, as on the board.
//
// Usage (from rtl_core/): obj_hps/Vdorado_rtl_top [microinstructions]
//   CEDAR_LS=1   the verify-cedar-ls input (click, type a List over STP)
//   CEDAR_PGM=f  render the screen to f at the end
//   HPS_QUANTUM=n, HPS_GERM_POLL=n  the host's step and poll interval
//   HPS_CLICK=x,y  click there instead of CEDAR_LS's (700,733), and type nothing
//   HPS_PCHIST_FROM=n  from n microinstructions on, histogram task 0's
//                IM address per clock; the top 40 at the end
// =============================================================================

#include <algorithm>
#include <chrono>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <verilated.h>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"
#include "sim_ddr_model.h"
#include "sim_census.h"
#include "hps/hps_host.h"

extern "C" {
#include "machine.h"
#include "display.h"
#include "memory.h"
#include "typetext.h"
}

static Vdorado_rtl_top *top;
static ddr_model *ddr;
static uint64_t clocks;
static int mouse_tx, mouse_ty, mouse_tb;   // scripted pointer target and buttons
static bool     hist_on;
static uint64_t hist[32];              // fabric clocks per CPU state
static uint64_t trans[32][32], same[32];  // state changes; clocks a state repeated
static const char *stname[] = {"C_FETCH","C_DEC","C_EXEC","C_NF","C_NEXTRD","C_NEXT","C_SCHED",
    "C_IFU_W","C_IFU_DECW","C_IFU_DEC","C_IFU_AW","C_IFU_BETA","C_IFU_BW","C_IFU_END","C_FINISH","C_APW",
    "C_PIPE","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?"};

static uint64_t pchist[4096];
static std::map<uint32_t, uint64_t> vahist;     // (kind << 28 | va) -> references
static bool     pchist_on, watch_on;
static void clock_n(int n) {
    for (int i = 0; i < n; i++) {
        ddr->drive(top);
        top->clk = 0; top->eval();
        ddr->edge(top);
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        clocks++;
        // HPS_OPTRACE=lo,hi[,psb]: task 0's Mesa opcodes in a fabric-clock window
        // (code base BR31, byte PC, opcode, current PSB), one line per change
        {
            static uint64_t olo = 0, ohi = 0;
            static unsigned opsb = 0;
            static bool oparsed = false;
            if (!oparsed) {
                oparsed = true;
                if (const char *o = getenv("HPS_OPTRACE")) sscanf(o, "%llu,%llu,%o", (unsigned long long *)&olo, (unsigned long long *)&ohi, &opsb);
            }
            auto *r = top->rootp;
            if (clocks >= olo && clocks < ohi && r->dorado_rtl_top__DOT__u_cpu__DOT__ctask == 0 &&
                (!opsb || r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1] == opsb)) {
                static uint32_t pb = ~0u; static unsigned pp = ~0u, po = ~0u;
                uint32_t b = r->dorado_rtl_top__DOT__u_cpu__DOT__br[31] & 0x0FFFFFFFu;
                unsigned pc = r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_pcx, op = r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_opcode;
                if (b != pb || pc != pp || op != po)
                    printf("[op] clk %llu cb %09o pc %06o op %03o psb %06o sp %03o\n", (unsigned long long)clocks, b, pc, op,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1], (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__StkP);
                pb = b; pp = pc; po = op;
            }
        }
        // HPS_FLT=lo,hi: each clock in the window where the task, real PC, StkP,
        // the stack flags or the fault count change
        if (const char *fw = getenv("HPS_FLT")) {
            static uint64_t flo, fhi; static bool fp;
            if (!fp) { fp = true; sscanf(fw, "%llu,%llu", (unsigned long long *)&flo, (unsigned long long *)&fhi); }
            auto *r = top->rootp;
            if (clocks >= flo && clocks < fhi) {
                static unsigned pk = ~0u;
                unsigned ct = r->dorado_rtl_top__DOT__u_cpu__DOT__ctask, rp = r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777;
                unsigned sp = r->dorado_rtl_top__DOT__u_cpu__DOT__StkP, fl = (r->dorado_rtl_top__DOT__u_cpu__DOT__stk_ovf << 1) | r->dorado_rtl_top__DOT__u_cpu__DOT__stk_und;
                unsigned fc = (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__fault_count;
                unsigned k = (ct << 28) ^ (rp << 12) ^ (sp << 4) ^ fl ^ (fc << 20);
                if (getenv("HPS_FLT_ALL")) {
                    auto bits = [](const VlWide<3> &w, int lo, int n) -> unsigned {
                        uint64_t v = 0; for (int i = 0; i < n; i++) { int b = lo + i; v |= (uint64_t)((w[b / 32] >> (b % 32)) & 1) << i; } return (unsigned)v; };
                    auto *cu = &r->dorado_rtl_top__DOT__u_cpu__DOT__cur_u, *xu = &r->dorado_rtl_top__DOT__u_cpu__DOT__x_u, *eu = &r->dorado_rtl_top__DOT__u_cpu__DOT__ex_u;
                    printf("      cur ff %03o jcn %03o | x ff %03o jcn %03o | ex ff %03o jcn %03o | im_q_addr %04o pred_np %04o ok %u x_pred_np %04o x_pred_np_x %04o fresh %u pent %u exec_now %u fexec %u p_pc_x %04o pc_p %04o f_np %04o p_d %u\n",
                           bits(*cu,58,8), bits(*cu,50,8), bits(*xu,58,8), bits(*xu,50,8), bits(*eu,58,8), bits(*eu,50,8),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__im_q_addr, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pred_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pred_ok, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_np_x & 07777, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_fresh,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pent, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__exec_now,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__fexec, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_pc_x & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pc_p & 07777, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__f_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_d);
                }
                if (getenv("HPS_FLT_ALL"))
                    printf("[clk] %llu task %u pc %04o st %u np %04o ap_req %u ap_hold %u ap_done_q %u mb_req %u op %u va %07o pcf %06o pfpcf %06o fast %u StkP %03o\n",
                           (unsigned long long)clocks, ct, rp, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__st,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_req, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_hold,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_done_q, (unsigned)r->dorado_rtl_top__DOT__mb_req,
                           (unsigned)r->dorado_rtl_top__DOT__mb_op, (unsigned)r->dorado_rtl_top__DOT__mb_va & 0x0FFFFFFF,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_pcf, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pf_pcf,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__nx_fast, sp);
                if (k != pk)
                    printf("[flt] clk %llu task %u pc %04o StkP %03o ovf/und %u faults %u wake %04x\n", (unsigned long long)clocks, ct, rp, sp, fl, fc,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__wakeup_pending);
                pk = k;
            }
        }
        if (pchist_on && top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__ctask == 0)
            pchist[top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777]++;
        if (watch_on) {
            auto *r = top->rootp;
            auto watched = [](uint32_t va) {
                return va == 0200000u || (va >= 0202154u && va <= 0202157u) || (va >= 0202044u && va <= 0202047u);
            };
            if (r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req && r->dorado_rtl_top__DOT__u_cpu__DOT__mem_accepted) {
                uint32_t va = r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_va & 0x0FFFFFFFu;
                unsigned k = r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_kind;
                static uint64_t wlo = getenv("HPS_WATCH_CLK") ? strtoull(getenv("HPS_WATCH_CLK"), nullptr, 0) : ~0ull;
                bool inwin = clocks >= wlo && clocks < wlo + 400;
                static uint32_t wva = getenv("HPS_WATCH_VA") ? (uint32_t)strtoul(getenv("HPS_WATCH_VA"), nullptr, 8) : 0xFFFFFFFFu;
                if (va == wva && (k == 5 || k == 10 || k == 4))
                    printf("[wva] clk %llu %s b %06o task %u pc %04o psb %06o\n", (unsigned long long)clocks,
                           k == 5 ? "STORE" : "fetch", (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_b,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1]);
                if ((watched(va) || inwin) && (k == 5 || k == 10 || k == 4))
                    printf("[watch] clk %llu %s va %07o b %06o task %u pc %04o\n", (unsigned long long)clocks,
                           k == 5 ? "STORE" : "fetch", va, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_b,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777));
            }
            static bool ap_prev;
            bool apq = r->dorado_rtl_top__DOT__mb_req;
            if (apq && !ap_prev) {
                uint32_t va = r->dorado_rtl_top__DOT__mb_va & 0x0FFFFFFFu;
                unsigned op = r->dorado_rtl_top__DOT__mb_op;
                if (op == 6 || op == 4 || (op <= 2 && watched(va)))
                    printf("[watch] clk %llu AP op %u va %07o wd %06o pc %04o task %u\n", (unsigned long long)clocks, op, va,
                           (unsigned)r->dorado_rtl_top__DOT__mb_wd,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ctask);
            }
            ap_prev = apq;
        }
        if (pchist_on && top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req &&
            top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_accepted &&
            top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task == 0)
            vahist[((uint32_t)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_kind << 28) |
                   (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_va & 0x0FFFFFFFu)]++;
        if (hist_on) {
            unsigned stt = (top->dbg_vec[1] >> 24) & 31;
            hist[stt]++;
            static unsigned prev = 0;
            if (stt != prev) trans[prev][stt]++;
            else             same[stt]++;
            prev = stt;
        }
    }
}
static void wait_fn(void *) { clock_n(4); }
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    setvbuf(stdout, nullptr, _IOLBF, 0);
    uint64_t want = argc > 1 ? strtoull(argv[1], nullptr, 0) : 2000000;
    hist_on = getenv("HPS_HIST") != nullptr;

    // The image's $readmemh files are init/...: build from obj_hps, where
    // init -> ../init_cedar.
    if (chdir(getenv("HPS_INIT_PARENT") ? getenv("HPS_INIT_PARENT") : "obj_hps") != 0) {
        perror("obj_hps");
        return 2;
    }
    top = new Vdorado_rtl_top;
    ddr = new ddr_model;
    top->clk = 0; top->rst = 1; top->clk_en = 1;
    top->bb_present = 0; top->bb_running = 1; top->bb_ss_inject = 0;
    top->io_bridge_en = 1; top->bootsrv_en = 0; top->assist_en = 0;
    // Cedar's own display microcode draws through the display controller
    top->display_wake_gate = 1; top->display_top_lines = 22; top->eth_local_host = 042;
    top->io_attached = 0; top->fb_rd_addr = 0;
    top->kbd_words = ~0ULL;
    top->mouse_buttons = 0; top->mouse_present = 0; top->mouse_x = 0; top->mouse_y = 0;
    clock_n(4);
    top->rst = 0;
    top->ov_hold = getenv("RTL_OV_HOLD") ? 1 : 0;   // (a PIPELINE build: no C_PIPE)
    clock_n(256);       // the mailbox clears the control beat first, as on the board

    const char *ddir = getenv("DORADO_DIR") ? getenv("DORADO_DIR") : "../../dorado";
    if (chdir(ddir) != 0) { perror(ddir); return 2; }

    hps_transport t;
    t.mb = &ddr->mem[0x100000];                    // byte 0x3080_0000
    t.storage = (volatile uint16_t *)&ddr->mem[0];  // byte 0x3000_0000
    t.storage_words = 4u << 20;
    t.wait = wait_fn;
    t.ctx = nullptr;
    hps_config cfg;
    cfg.snapshot = "build/good-packs/cedar-desktop.snap";
    cfg.pdi = "build/good-packs/cedar-desktop.pdi";
    cfg.ftp_root = "../chm/cedar/stp-root";
    cfg.eb = "../chm/dorado/CedarDorado.eb!6";
    cfg.germ = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
    cfg.quantum = getenv("HPS_QUANTUM") ? (unsigned)atoi(getenv("HPS_QUANTUM")) : 1;
    cfg.germ_poll = getenv("HPS_GERM_POLL") ? strtoull(getenv("HPS_GERM_POLL"), nullptr, 0) : 0;
    cfg.keyboard = 0;
    if (hps_start(&cfg, &t) != 0) return 1;
    dorado_machine *M = (dorado_machine *)hps_machine();
    const hps_stats *S = hps_get_stats();

    const bool ls = getenv("CEDAR_LS") != nullptr;
    const uint64_t c0 = dorado_machine_cycles(M);
    uint64_t start = S->rtl_cycles;
    int click = 0;
    uint64_t click_at = 0;
    bool typed = false;
    uint64_t next_frame = c0 + 2000000ull, next_report = 0;
    static dorado_typequeue tq;
    auto t0 = std::chrono::steady_clock::now();
    while (S->rtl_cycles - start < want) {
        clock_n(2048);
        hps_poll();
        uint64_t now = dorado_machine_cycles(M);
        // HPS_KEYTEST: A held at the core's keyboard (terminal back channel)
        // from 2M to 4M microinstructions; KeyBits and the mouse read back
        if (getenv("HPS_KEYTEST")) {
            uint64_t u = S->rtl_cycles - start;
            static int phase = 0;
            auto show = [&](const char *w) {
                dorado_memory *mem = dorado_machine_memory(M);
                printf("[hps] %s at %llu: 177033..37", w, (unsigned long long)u);
                for (unsigned a = 0177033; a <= 0177037; a++) printf(" %06o", dorado_visible_word_at_va(mem, a));
                printf("  mouse %d %d\n", (short)dorado_visible_word_at_va(mem, 0424),
                       (short)dorado_visible_word_at_va(mem, 0425));
#define RIO(x) top->rootp->dorado_rtl_top__DOT__u_io__DOT__##x
                printf("        io: kbd words %04x %04x %04x %04x pending %x  term bits %llu messages %llu  msg type %u bit %u\n",
                       (unsigned)RIO(ddc_kbd_words)[0], (unsigned)RIO(ddc_kbd_words)[1], (unsigned)RIO(ddc_kbd_words)[2],
                       (unsigned)RIO(ddc_kbd_words)[3], (unsigned)RIO(ddc_kbd_pending),
                       (unsigned long long)RIO(ddc_term_bits), (unsigned long long)RIO(ddc_term_messages),
                       (unsigned)RIO(ddc_term_msg_type), (unsigned)RIO(ddc_term_msg_bit));
#undef RIO
            };
            if (phase == 0 && u >= 1000000) { show("before"); phase = 1; }
            if (phase == 1 && u >= 2000000) {
                top->kbd_words = ~0ULL & ~(1ULL << (32 + 10));      // A: word 2, 175777
                top->mouse_dx = 30; top->mouse_dy = 20; top->mouse_delta_stb = 1;
                clock_n(1); top->mouse_delta_stb = 0;
                phase = 2;
            }
            if (phase == 2 && (u == 2500000 || u >= 3000000) && u < 3010000) show("A held");
            if (phase == 2 && u >= 4000000) { show("A down"); top->kbd_words = ~0ULL; phase = 3; }
            if (phase == 3 && u >= 6000000) { show("A up"); phase = 4; }
        }
        if (S->rtl_cycles - start >= next_report) {
            {
                dorado_memory *mem = dorado_machine_memory(M);
                printf("[hps] cursor at %llu: CursorX raw %u, rows %llu, last cursor row x %d y %u (lowest y %u); guest mouse %d %d cursor %d %d\n",
                       (unsigned long long)(S->rtl_cycles - start),
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_nlcb_cursor_x,
                       (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_rows,
                       (int)(int16_t)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_x,
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_y,
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_ymin,
                       (short)dorado_visible_word_at_va(mem, 0424), (short)dorado_visible_word_at_va(mem, 0425),
                       (short)dorado_visible_word_at_va(mem, 0426), (short)dorado_visible_word_at_va(mem, 0427));
            }
            next_report += 10000000;
            fprintf(stderr, "  [hps] %llu microinstructions, host cycle %llu; %llu ops (%llu reads, %llu stores), "
                    "%llu packets in\n",
                    (unsigned long long)(S->rtl_cycles - start), (unsigned long long)now,
                    (unsigned long long)S->ops, (unsigned long long)S->reads,
                    (unsigned long long)S->stores, (unsigned long long)S->rx_packets);
        }
        if (!ls && !getenv("HPS_CLICK")) continue;
        // Input reaches the core as the MiSTer's does: keyboard words and
        // mouse motion into dorado_rtl_top, where Cedar's display microcode
        // takes them from the terminal back channel. The script below drives
        // the C machine's keyboard words (the typing queue) and a target
        // pointer position; they are forwarded here.
        {
            dorado_display *d = dorado_machine_display(M);
            uint64_t kw = 0;
            for (int i = 0; i < 4; i++) kw |= (uint64_t)dorado_display_keyboard_word(d, i) << (16 * i);
            top->kbd_words = kw;
            static int sent_x = 0, sent_y = 0;          // Cedar's pointer starts at 0,0
            if (mouse_tx != sent_x || mouse_ty != sent_y) {
                int dx = mouse_tx - sent_x, dy = mouse_ty - sent_y;
                if (dx > 127) dx = 127; else if (dx < -127) dx = -127;
                if (dy > 127) dy = 127; else if (dy < -127) dy = -127;
                top->mouse_dx = dx; top->mouse_dy = dy; top->mouse_delta_stb = 1;
                clock_n(1);
                top->mouse_delta_stb = 0;
                sent_x += dx; sent_y += dy;
            }
            top->mouse_buttons = mouse_tb;
        }
        // verify-cedar-ls, as sim_cedar.cpp drives it (2M-cycle frames)
        if (now >= next_frame) {
            next_frame += 2000000ull;
            if (typed) dorado_typequeue_pump(&tq, M);
        }
        if (const char *wf = getenv("HPS_WATCH_FROM"))
            watch_on = S->rtl_cycles - start >= strtoull(wf, nullptr, 0);
        if (const char *pf = getenv("HPS_PCHIST_FROM"))
            pchist_on = S->rtl_cycles - start >= strtoull(pf, nullptr, 0);
        if (click == 0 && now >= c0 + 100000000ull) {
            int cx = 700, cy = 733;
            if (const char *hc = getenv("HPS_CLICK")) sscanf(hc, "%d,%d", &cx, &cy);
            printf("[hps] click (%d,%d) at cycle %llu\n", cx, cy, (unsigned long long)now);
            mouse_tx = cx; mouse_ty = cy;
            click = 1; click_at = now;
        } else if (click == 1 && now >= click_at + 20000000ull) {   // after the pointer arrives
            mouse_tb = DORADO_MOUSE_LEFT;
            click = 2; click_at = now;
        } else if (click == 2 && now >= click_at + 1600000ull) {
            mouse_tb = 0;
            click = 3;
        }
        if (!typed && !getenv("HPS_CLICK") && now >= c0 + 300000000ull) {
            typed = true;
            printf("[hps] typing at cycle %llu\n", (unsigned long long)now);
            dorado_typequeue_start(&tq, "List [Cedar]<Cedar6.1>VersionMap>*\n", 800000ull, now);
        }
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    printf("[hps] %llu microinstructions on the RTL in %.1f s, %llu clocks (%.2f each; the status beat's count says %.2f); host at cycle %llu\n",
           (unsigned long long)(S->rtl_cycles - start), secs, (unsigned long long)clocks,
           (double)clocks / (double)(S->rtl_cycles - start),
           (double)S->rtl_clocks64 / (double)(S->rtl_cycles - start),
           (unsigned long long)dorado_machine_cycles(M));
    printf("[hps] cursor: %llu CursorHi writes, %llu rows drawn; CursorX %u\n",
           (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_wr,
           (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_rows,
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_nlcb_cursor_x);
    printf("[hps] mailbox: %llu operations -- %llu reads, %llu stores, %llu NWW ors, %llu reschedules; "
           "%llu packets in, %llu out, %llu transmits lost, %llu inexpressible\n",
           (unsigned long long)S->ops, (unsigned long long)S->reads, (unsigned long long)S->stores,
           (unsigned long long)S->rm_or, (unsigned long long)S->resched,
           (unsigned long long)S->rx_packets, (unsigned long long)S->tx_packets,
           (unsigned long long)S->tx_unsupported,
           (unsigned long long)S->unported);
    printf("[hps] accounting: %llu NWW edges; rx fills %u sent, %u applied; %llu result tears, %llu rx lost\n",
           (unsigned long long)S->nww_posts, (unsigned)(S->rx_packets & 0x7FFF), (unsigned)S->rx_applied,
           (unsigned long long)S->result_tears, (unsigned long long)S->rx_lost);
    if (hist_on) {
        uint64_t tot = 0;
        for (int i = 0; i < 32; i++) tot += hist[i];
        printf("[hps] clocks by CPU state (%llu, %.2f per microinstruction):\n",
               (unsigned long long)tot, (double)tot / (S->rtl_cycles - start));
        for (int i = 0; i < 32; i++)
            if (hist[i] * 200 > tot)
                printf("         %-10s %5.1f%%  %.2f/uinstr  (repeats %.2f/uinstr)\n", stname[i], 100.0 * hist[i] / tot,
                       (double)hist[i] / (S->rtl_cycles - start), (double)same[i] / (S->rtl_cycles - start));
        printf("[hps] state changes, per microinstruction:\n");
        for (int a = 0; a < 32; a++) for (int b = 0; b < 32; b++)
            if (trans[a][b] * 50 > (S->rtl_cycles - start))
                printf("         %-10s -> %-10s %.2f\n", stname[a], stname[b],
                       (double)trans[a][b] / (S->rtl_cycles - start));
    }
    if (pchist_on) {
        std::vector<std::pair<uint64_t, unsigned>> v;
        uint64_t tot = 0;
        for (unsigned a = 0; a < 4096; a++) if (pchist[a]) { v.push_back({pchist[a], a}); tot += pchist[a]; }
        std::sort(v.rbegin(), v.rend());
        printf("[hps] task 0 IM addresses, %llu clocks:\n", (unsigned long long)tot);
        for (size_t i = 0; i < v.size() && i < 40; i++)
            printf("  %04o %6.2f%%\n", v[i].second, 100.0 * (double)v[i].first / (double)tot);
        std::vector<std::pair<uint64_t, uint32_t>> w;
        uint64_t nref = 0;
        for (auto &e : vahist) { w.push_back({e.second, e.first}); nref += e.second; }
        std::sort(w.rbegin(), w.rend());
        printf("[hps] task 0 references, %llu, %zu distinct (kind va):\n", (unsigned long long)nref, w.size());
        for (size_t i = 0; i < w.size() && i < 40; i++)
            printf("  kind %u va %09o  %llu\n", w[i].second >> 28, w[i].second & 0x0FFFFFFFu,
                   (unsigned long long)w[i].first);
    }
    if (getenv("HPS_CENSUS")) census_report(top);
    printf("[hps] DDR: %llu beats read, %llu written\n",
           (unsigned long long)ddr->beats_rd, (unsigned long long)ddr->beats_wr);
    if (const char *rtl = getenv("HPS_PGM_RTL")) {      // what the core's video shows
        FILE *f = fopen(rtl, "wb");
        int px = 0;
        if (f) {
            fprintf(f, "P5\n1024 808\n255\n");
            for (int i = 0; i < 1024 * 808; i++) {
                int b = top->rootp->dorado_rtl_top__DOT__u_io__DOT__fb[i] & 1;
                {   // the cursor, mixed in as the scan-out does
                    unsigned row = top->rootp->dorado_rtl_top__DOT__u_io__DOT__cur_line[i >> 10];
                    unsigned d = (unsigned)((i & 1023) - top->rootp->dorado_rtl_top__DOT__u_io__DOT__cur_x) & 0xFFFF;
                    if (d < 16 && ((row >> (15 - d)) & 1)) b = 1;
                }
                px += b;
                fputc(b ? 0 : 255, f);
            }
            fclose(f);
        }
        printf("[hps] RTL display controller: %d pixels -> %s\n", px, rtl);
    }
    if (const char *pgm = getenv("CEDAR_PGM")) {
        int px = hps_snapshot_pgm(pgm);
        printf("[hps] %d display-list pixels -> %s\n", px, pgm);
    }
    if (top->dbg_halted) {
        printf("  [FAIL] the RTL halted at pc=0%o\n", (unsigned)top->dbg_real_pc);
        return 1;
    }
    printf("  [PASS] MiSTer Cedar configuration ran\n");
    return 0;
}
