// The census the CPU keeps of the events that decide a pipeline's
// throughput (docs/rtl-core-speed-plan.md, Phase 0), and what they would
// cost a core that retires one microinstruction per clock. Shared by the
// simulations: HPS_CENSUS, CEDAR_CENSUS, FPGA_CENSUS.
#pragma once
#include <cstdio>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"

static inline void census_report(Vdorado_rtl_top *top) {
#define C(n) ((double)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__cen_##n)
#define CM(n) ((double)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__u_mem__DOT__cen_##n)
    double n = C(retired);
    if (n <= 0) return;
    double per = 100.0 / n;
    printf("[census] %.0f retired, %.1f%% by the emulator task; per 100 instructions:\n", n, per * C(task0));
    printf("         references: Fetch/Store/LongFetch %.1f (Store %.1f), other %.1f; misses %.2f (%.1f per 1000); "
           "IFU fetches %.1f, misses %.2f\n",
           per * C(ref_fs), per * C(store), per * C(ref_other), per * CM(miss),
           1000.0 * CM(miss) / (C(ref_fs) + C(ref_other) + 1e-9), per * C(ifu_fetch), per * CM(ifu_miss));
    printf("         <-Md: %.1f -- the instruction after the reference: immediate %.2f, deferred %.2f; "
           "two after: immediate %.2f, deferred %.2f; later %.1f\n",
           per * C(md), per * C(md_imm1), per * C(md_def1), per * C(md_imm2), per * C(md_def2), per * C(md_3p));
    const auto &hl = top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__cen_ifu_hl;
    const auto &ln = top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__cen_ifu_len;
    double j = C(ifujump);
    printf("         IFUJumps %.1f (one per %.1f instructions); instructions since the last: 1: %.1f%%  2: %.1f%%  3: %.1f%%  "
           "4: %.1f%%  5-8: %.1f%%  9-16: %.1f%%  17+: %.1f%%\n",
           per * j, n / (j + 1e-9), 100.0 * hl[0] / j, 100.0 * hl[1] / j, 100.0 * hl[2] / j, 100.0 * hl[3] / j,
           100.0 * hl[4] / j, 100.0 * hl[5] / j, 100.0 * hl[6] / j);
    printf("         opcodes: 1 byte %.1f%%, 2 bytes %.1f%%, 3 bytes %.1f%%; at an odd byte %.1f%%; jumps %.1f%%; "
           "NotReady traps %.2f per 100 instr\n",
           100.0 * ln[1] / j, 100.0 * ln[2] / j, 100.0 * ln[3] / j, 100.0 * C(ifu_odd) / j,
           100.0 * C(ifu_jump) / j, per * C(ifu_notready));
    printf("         PCF<-B restarts %.2f; conditional branches %.1f; Read/Write IM %.2f; I/O instructions %.1f; BLOCKs %.1f; "
           "task switches %.1f; holds %.2f; clocks waiting: I/O engines %.2f, host port %.2f\n",
           per * C(pcf), per * C(cond), per * C(imrw), per * C(io), per * C(block), per * C(switch), per * C(hold),
           per * C(iobusy), per * C(apwait));
    // Stall budget of a one-instruction-per-clock pipeline.
    //   memory, 2 clocks after issue (address section, then cache section):
    //     immediate <-Md one after: 2; deferred one after: 1; immediate two after: 1
    //   memory, the Dorado's own timing (HM 5.4): immediate <-Md one after: 1, the rest free
    //   IFU: steady state the prefetcher keeps up (HM 6.3); a restart (PCF<-B) is physically
    //        about 4 clocks, hidden behind the modeled 5-instruction warmup; the upper bound
    //        charges a 1-instruction handler 2 clocks and a 2-instruction handler 1
    //   Read/Write IM: 3 (HM Table 4: three cycles); holds and the two waits as counted
    double md2  = 2 * C(md_imm1) + C(md_def1) + C(md_imm2);
    double md1  = C(md_imm1);
    double ifu  = 2 * hl[0] + hl[1];
    double rest = 3 * C(imrw) + C(hold) + C(iobusy) + C(apwait);
    printf("[census] pipeline stall clocks per instruction: Md %.3f with a 2-clock memory, %.3f with the Dorado's; "
           "IFU 0 to %.3f; IM r/w + holds + waits %.3f (I/O-engine waits %.3f of that)\n",
           md2 / n, md1 / n, ifu / n, rest / n, C(iobusy) / n);
    printf("[census] predicted clocks per instruction: %.2f (Dorado-timed memory, IFU keeping up) to %.2f (2-clock memory, IFU bound)\n",
           1 + (md1 + rest) / n, 1 + (md2 + ifu + rest) / n);
#undef C
#undef CM
}
