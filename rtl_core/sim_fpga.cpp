// =============================================================================
// sim_fpga.cpp — Run dorado_rtl_top exactly as the FPGA build does
// =============================================================================
// No C model and no array pokes: the CPU powers up from init/ (DORADO_INIT,
// written by `RTL_DUMP_INIT=init ./obj_dir/Vdorado_rtl_top`), the in-FPGA boot
// server answers the Ethernet from roms/, and dorado_rtl_assist does the
// loader assists. Reports the boot and writes the framebuffer as a PBM.
//
//   ./obj_fpga/Vdorado_rtl_top [microinstructions] [out.pbm]
//
// SIM_KEYS="start:word:bit:len[,...]" holds Alto keyboard word `word` bit
// `bit` down (active low) from microinstruction `start` for `len`. Word 4
// is the mouse buttons (bit 2 Red/left, 1 Blue/right, 0 Yellow/middle).
// =============================================================================
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"
#include "verilated.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define R_IO(f)  (top->rootp->dorado_rtl_top__DOT__u_io__DOT__##f)
#define R_SRV(f) (top->rootp->dorado_rtl_top__DOT__u_bootsrv__DOT__##f)
#define R_AS(f)  (top->rootp->dorado_rtl_top__DOT__u_assist__DOT__##f)
#define R_CPU(f) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##f)

struct key_ev { long long start, len; int word, bit; };

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    long long want = argc > 1 ? atoll(argv[1]) : 80000000LL;
    const char *pbm = argc > 2 ? argv[2] : nullptr;
    Vdorado_rtl_top *top = new Vdorado_rtl_top;

    top->clk = 0; top->rst = 1; top->clk_en = 1;
    top->bb_present = 0; top->bb_running = 1; top->bb_ss_inject = 0;
    top->io_bridge_en = 1; top->bootsrv_en = 1; top->assist_en = 1;
    top->display_wake_gate = 0; top->eth_local_host = 042;
    top->io_attached = 0; top->fb_rd_addr = 0;
    top->kbd_words = ~0ULL;
    top->mouse_buttons = 0; top->mouse_present = 0; top->mouse_x = 0; top->mouse_y = 0;

    key_ev keys[16]; int nkeys = 0;
    if (const char *ks = getenv("SIM_KEYS")) {
        while (*ks && nkeys < 16) {
            key_ev &k = keys[nkeys];
            if (sscanf(ks, "%lld:%d:%d:%lld", &k.start, &k.word, &k.bit, &k.len) != 4) break;
            nkeys++;
            const char *c = strchr(ks, ',');
            if (!c) break;
            ks = c + 1;
        }
    }
    for (int i = 0; i < 4; i++) { top->clk = !top->clk; top->eval(); }
    top->rst = 0;

    auto t0 = std::chrono::steady_clock::now();
    unsigned long long clocks = 0;
    // IFU-idle statistics once the boot file is in: the keyboard assist
    // writes only while the IFU is idle.
    static const char *stname[] = {"C_FETCH","C_DEC","C_EXEC","C_PREF","C_NEXTRD","C_NEXT","C_SCHED",
        "C_IFU_OPW","C_IFU_DECW","C_IFU_DEC","C_IFU_AW","C_IFU_BETA","C_IFU_BW","C_IFU_END","C_FINISH",
        "M_ROW","M_MAPA","M_MAPV","M_BOOK","A_DO","A_WAIT","M_WRA","M_WRV","M_WB","M_FILL","M_RD","M_WR",
        "M_IOC","M_IOS","M_IOW"};
    unsigned long long hist[32] = {0}, fetch_wait = 0, hist_clocks = 0, hist_steps = 0;
    const bool want_hist = getenv("SIM_HIST") != nullptr;
    unsigned long long idle_steps = 0, run_steps = 0, max_gap = 0, gap = 0;
    while ((long long)top->dbg_cycles < want && !top->dbg_halted) {
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        clocks++;
        if (want_hist && R_SRV(eftp_max_seq) >= 113) {
            unsigned st = R_CPU(st) & 31;
            hist[st]++; hist_clocks++;
            if (st == 0 && top->step_done == 0 && R_CPU(io_busy)) fetch_wait++;
            if (top->step_done) hist_steps++;
        }
        if (top->step_done) {
            long long cyc = (long long)top->dbg_cycles;
            uint64_t w = ~0ULL;
            unsigned btn = 0;
            for (int i = 0; i < nkeys; i++)
                if (cyc >= keys[i].start && cyc < keys[i].start + keys[i].len) {
                    if (keys[i].word == 4) btn |= 1u << keys[i].bit;
                    else w &= ~(1ULL << (16 * keys[i].word + keys[i].bit));
                }
            top->kbd_words = w;
            top->mouse_buttons = btn;
            static uint64_t last_written = ~0ULL;
            if (R_AS(kbd_written) != last_written) {
                last_written = R_AS(kbd_written);
                printf("  [kbd] at %lld assist wrote %016llx\n", cyc, (unsigned long long)last_written);
            }
            if (R_SRV(eftp_max_seq) >= 113) {
                run_steps++;
                if (!R_CPU(dbg_ifu_active)) {
                    idle_steps++;
                    if (gap > max_gap) max_gap = gap;
                    gap = 0;
                } else gap++;
            }
        }
    }
    double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();

    int fb_px = 0;
    for (int i = 0; i < 1024 * 808; i++) fb_px += R_IO(fb)[i] & 1;
    printf("[FPGA-SIM] %llu microinstructions, %llu clocks (%.2f clocks/uinstr), %.1fs\n",
           (unsigned long long)top->dbg_cycles, clocks, (double)clocks / top->dbg_cycles, s);
    printf("           halted=%u pc=0%o bol=%u eftp_max_seq=%u eth_tx=%llu ddc_lines=%llu fb_px=%d\n",
           top->dbg_halted, top->dbg_real_pc, (unsigned)R_SRV(bol_sent),
           (unsigned)R_SRV(eftp_max_seq), (unsigned long long)R_IO(eth_tx_packets),
           (unsigned long long)R_IO(ddc_lines_rendered), fb_px);
    if (want_hist && hist_steps) {
        printf("  state histogram after boot (%llu clocks, %llu steps, %.2f clocks/step):\n",
               hist_clocks, hist_steps, (double)hist_clocks / hist_steps);
        for (int i = 0; i < 30; i++) if (hist[i])
            printf("    %-11s %6.3f per step\n", stname[i], (double)hist[i] / hist_steps);
        printf("    (FETCH clocks with io_busy: %.3f per step)\n", (double)fetch_wait / hist_steps);
    }
    printf("           BR:");
    for (int i = 0; i < 32; i++) if (R_CPU(br)[i]) printf(" [%o]=%o", i, (unsigned)R_CPU(br)[i]);
    printf("\n");
    printf("           after boot: %llu steps, IFU idle on %llu, longest IFU-busy run %llu\n",
           run_steps, idle_steps, max_gap);
    if (pbm) {
        FILE *f = fopen(pbm, "wb");
        fprintf(f, "P4\n1024 808\n");
        for (int i = 0; i < 1024 * 808 / 8; i++) {
            int byte = 0;
            for (int k = 0; k < 8; k++) byte = (byte << 1) | (R_IO(fb)[i * 8 + k] & 1);
            fputc(byte, f);
        }
        fclose(f);
    }
    bool ok = R_SRV(eftp_max_seq) >= 113 && fb_px >= 1000;
    printf("  [%s] FPGA configuration boot\n", ok ? "PASS" : "FAIL");
    delete top;
    return ok ? 0 : 1;
}
