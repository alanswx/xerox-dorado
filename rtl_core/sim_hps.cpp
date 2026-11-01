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
// =============================================================================

#include <chrono>
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
static const char *stname[] = {"C_FETCH","C_DEC","C_EXEC","C_PREF","C_NEXTRD","C_NEXT","C_SCHED",
    "C_IFU_W","C_IFU_DECW","C_IFU_DEC","C_IFU_AW","C_IFU_BETA","C_IFU_BW","C_IFU_END","C_FINISH","C_APW",
    "?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?"};

static void clock_n(int n) {
    for (int i = 0; i < n; i++) {
        ddr->drive(top);
        top->clk = 0; top->eval();
        ddr->edge(top);
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        clocks++;
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
            };
            if (phase == 0 && u >= 1000000) { show("before"); phase = 1; }
            if (phase == 1 && u >= 2000000) {
                top->kbd_words = ~0ULL & ~(1ULL << (32 + 10));      // A: word 2, 175777
                top->mouse_dx = 30; top->mouse_dy = 20; top->mouse_delta_stb = 1;
                clock_n(1); top->mouse_delta_stb = 0;
                phase = 2;
            }
            if (phase == 2 && u >= 4000000) { show("A down"); top->kbd_words = ~0ULL; phase = 3; }
            if (phase == 3 && u >= 6000000) { show("A up"); phase = 4; }
        }
        if (S->rtl_cycles - start >= next_report) {
            next_report += 10000000;
            fprintf(stderr, "  [hps] %llu microinstructions, host cycle %llu; %llu ops (%llu reads, %llu stores), "
                    "%llu packets in\n",
                    (unsigned long long)(S->rtl_cycles - start), (unsigned long long)now,
                    (unsigned long long)S->ops, (unsigned long long)S->reads,
                    (unsigned long long)S->stores, (unsigned long long)S->rx_packets);
        }
        if (!ls) continue;
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
        if (click == 0 && now >= c0 + 100000000ull) {
            printf("[hps] click (700,733) at cycle %llu\n", (unsigned long long)now);
            mouse_tx = 700; mouse_ty = 733;
            click = 1; click_at = now;
        } else if (click == 1 && now >= click_at + 20000000ull) {   // after the pointer arrives
            mouse_tb = DORADO_MOUSE_LEFT;
            click = 2; click_at = now;
        } else if (click == 2 && now >= click_at + 1600000ull) {
            mouse_tb = 0;
            click = 3;
        }
        if (!typed && now >= c0 + 300000000ull) {
            typed = true;
            printf("[hps] typing at cycle %llu\n", (unsigned long long)now);
            dorado_typequeue_start(&tq, "List [Cedar]<Cedar6.1>VersionMap>*\n", 800000ull, now);
        }
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    printf("[hps] %llu microinstructions on the RTL in %.1f s, %llu clocks; host at cycle %llu\n",
           (unsigned long long)(S->rtl_cycles - start), secs, (unsigned long long)clocks,
           (unsigned long long)dorado_machine_cycles(M));
    printf("[hps] mailbox: %llu operations -- %llu reads, %llu stores, %llu NWW ors, %llu reschedules; "
           "%llu packets in, %llu out, %llu transmits lost, %llu inexpressible\n",
           (unsigned long long)S->ops, (unsigned long long)S->reads, (unsigned long long)S->stores,
           (unsigned long long)S->rm_or, (unsigned long long)S->resched,
           (unsigned long long)S->rx_packets, (unsigned long long)S->tx_packets,
           (unsigned long long)S->tx_unsupported,
           (unsigned long long)S->unported);
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
