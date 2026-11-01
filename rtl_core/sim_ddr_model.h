// sim_ddr_model.h — the MiSTer DDRAM port model shared by sim_fpga.cpp and
// sim_hps.cpp. Include after the Verilated top's header.
#pragma once
// The MiSTer DDRAM port (Avalon-MM, 64-bit beats) in front of a byte array:
// BUSY asserted pseudo-randomly, reads answered after a latency, one beat
// per clock -- enough variety to exercise dorado_rtl_storage_ddr's handshake.
#include <deque>
#include <vector>
struct ddr_model {
    std::vector<uint64_t> mem = std::vector<uint64_t>(1u << 24);   // 128 MB window
    struct rd { uint64_t due; uint32_t addr; int n; };
    std::deque<rd> reads;
    uint32_t w_addr = 0; int w_left = 0;
    uint64_t t = 0, rng = 12345, beats_rd = 0, beats_wr = 0;
    bool cur_busy = false, cur_ready = false;
    static uint32_t idx(uint32_t a) { return (a - 0x6000000u) & ((1u << 24) - 1); }
    // Drive the inputs the RTL samples at the coming edge.
    void drive(Vdorado_rtl_top *top) {
        rng = rng * 6364136223846793005ULL + 1442695040888963407ULL;
        cur_busy = ((rng >> 33) % 4) == 0;
        top->DDRAM_BUSY = cur_busy;
        cur_ready = !reads.empty() && reads.front().due <= t;
        top->DDRAM_DOUT_READY = cur_ready;
        if (cur_ready) top->DDRAM_DOUT = mem[idx(reads.front().addr)];
    }
    // Take what the RTL presented at this edge.
    void edge(Vdorado_rtl_top *top) {
        if (cur_ready) {
            beats_rd++;
            rd &r = reads.front();
            r.addr++; r.due = t + 1;
            if (--r.n == 0) reads.pop_front();
        }
        if (!cur_busy && (top->DDRAM_RD || top->DDRAM_WE) && top->DDRAM_BURSTCNT != 1) {
            fprintf(stderr, "[DDR] burst of %d at %llu (bursts fail on the MiSTer port)\n",
                    top->DDRAM_BURSTCNT, (unsigned long long)t);
            exit(2);
        }
        if (!cur_busy && top->DDRAM_RD && top->DDRAM_BE != 0xFF) {
            fprintf(stderr, "[DDR] read with BE=%02x at %llu\n", top->DDRAM_BE, (unsigned long long)t);
            exit(2);
        }
        if (!cur_busy && top->DDRAM_RD)
            reads.push_back({t + 12, (uint32_t)top->DDRAM_ADDR, (int)top->DDRAM_BURSTCNT});
        if (!cur_busy && top->DDRAM_WE) {
            if (w_left == 0) { w_addr = top->DDRAM_ADDR; w_left = top->DDRAM_BURSTCNT; }
            uint64_t &m = mem[idx(w_addr)];
            for (int b = 0; b < 8; b++)
                if (top->DDRAM_BE & (1u << b))
                    m = (m & ~(0xFFULL << (8 * b))) | (top->DDRAM_DIN & (0xFFULL << (8 * b)));
            w_addr++; w_left--; beats_wr++;
        }
        t++;
    }
};
