// =============================================================================
// sim_fpga.cpp — Run dorado_rtl_top exactly as the FPGA build does
// =============================================================================
// No C model and no array pokes: the CPU powers up from init/ (DORADO_INIT,
// written by `RTL_DUMP_INIT=init ./obj_dir/Vdorado_rtl_top`), the in-FPGA boot
// server answers the Ethernet from roms/, and dorado_rtl_assist does the
// loader assists. Reports the boot and writes the framebuffer as a PBM.
//
//   ./obj_fpga/Vdorado_rtl_top [microinstructions] [out.pbm]
// =============================================================================
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"
#include "verilated.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>

#define R_IO(f)  (top->rootp->dorado_rtl_top__DOT__u_io__DOT__##f)
#define R_SRV(f) (top->rootp->dorado_rtl_top__DOT__u_bootsrv__DOT__##f)

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
    for (int i = 0; i < 4; i++) { top->clk = !top->clk; top->eval(); }
    top->rst = 0;

    auto t0 = std::chrono::steady_clock::now();
    unsigned long long clocks = 0;
    while ((long long)top->dbg_cycles < want && !top->dbg_halted) {
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        clocks++;
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
