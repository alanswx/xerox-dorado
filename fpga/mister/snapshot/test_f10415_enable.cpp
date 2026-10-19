#include "Vsnapshot_enable_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static constexpr uint32_t mask = (1u << 18) - 1;
static Vsnapshot_enable_fixture d;
static uint32_t rng = 0xa5181041;
static unsigned edges, requests, paused;
static uint32_t word() {
    rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
    return rng & mask;
}
static void check(bool ok, const char *why) {
    if (!ok) { std::fprintf(stderr, "FAIL host=%u: %s\n", edges, why); std::exit(1); }
}
static void edge(bool guest, bool reference) {
    d.host_clk = 0; d.guest_advance = 0; d.ref_clk = 0; d.eval();
    d.host_clk = 1; d.guest_advance = guest; d.ref_clk = reference; d.eval();
    ++edges;
    d.host_clk = 0; d.guest_advance = 0; d.ref_clk = 0; d.eval();
}
static void equal(const char *why = "original cell output differs") { check(d.q == d.original_q, why); }
static uint32_t access(unsigned index, bool write, uint32_t data, bool error = false) {
    d.word_index = index; d.write = write; d.state_data = data; d.request = 1;
    edge(false, false); ++requests;
    check(d.done, "missing frozen response");
    check(bool(d.error) == error, "invalid state access accepted or valid access rejected");
    return d.result;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    std::array<uint32_t, 1024> memory{}, saved{};
    uint32_t pipeline = 0;
    d.freeze = 0; d.guest_held = 0; d.ce_n = 1; d.we_n = 1;
    d.request = 1; d.write = 1; d.word_index = 7; d.state_data = mask;
    edge(true, true); check(!d.done, "state access accepted while running");
    d.freeze = 1; edge(true, true);
    check(!d.frozen && !d.done, "unheld freeze accepted");
    d.freeze = 0; d.request = 0; d.ce_n = 0; d.we_n = 0;
    for (unsigned a = 0; a != 1024; ++a) {
        d.address = a; d.data = word(); edge(true, true); memory[a] = d.data; equal();
    }
    for (unsigned i = 0; i != 10000; ++i) {
        // Both clocks stop for a backend hold, but the host clock and RAM
        // address/data pins may continue changing. The guest read must hold.
        d.request = 0; d.guest_held = 0;
        for (unsigned n = word() % 5; n; --n) {
            d.address = word() & 1023; d.data = word();
            d.ce_n = word() & 1; d.we_n = word() & 1;
            edge(false, false); ++paused;
            equal("original cell output differs during backend hold");
            check(d.q == (d.ce_n ? 0 : pipeline), "independent held read differs");
        }
        d.address = word() & 1023; d.data = word();
        d.ce_n = (word() & 7) == 0; d.we_n = word() & 1;
        pipeline = memory[d.address];
        if (!d.ce_n && !d.we_n) memory[d.address] = d.data;
        edge(true, true); equal();
        check(d.q == (d.ce_n ? 0 : pipeline), "independent ordinary read differs");
    }
    d.address = 0; d.ce_n = 0; d.we_n = 0;
    pipeline = memory[0]; d.data = pipeline ^ mask;
    edge(true, true); memory[0] = d.data; equal();
    check(pipeline != memory[0], "collision fixture failed"); saved = memory;
    // Read-pipeline export is raw state, even when chip enable hides it.
    d.ce_n = 1;
    d.guest_held = 1; d.freeze = 1; d.request = 1; d.write = 1;
    d.word_index = 0; d.state_data = 1;
    edge(false, false); check(d.frozen && !d.done, "freeze entry accepted request"); equal();
    std::array<uint32_t, 1025> image{};
    for (unsigned a = 0; a != 1025; ++a) {
        image[a] = access(a, false, 0);
        check(image[a] == (a == 1024 ? pipeline : saved[a]), "exported RAM/pipeline differs"); equal();
    }
    d.ce_n = 0; d.eval(); equal();
    for (unsigned a = 1025; a != 2048; ++a) { access(a, true, word(), true); equal(); }
    for (unsigned a = 0; a != 1025; ++a)
        check(access(a, false, 0) == image[a], "invalid write changed exported state");

    // Candidate alone continues, corrupting all RAM and the original output
    // pipeline; the unchanged reference remains at the captured boundary.
    d.freeze = 0; d.request = 0; edge(false, false);
    check(!d.frozen, "freeze did not release"); d.guest_held = 0;
    d.ce_n = 0; d.we_n = 0;
    for (unsigned a = 0; a != 1024; ++a) {
        d.address = a; d.data = saved[a] ^ mask; edge(true, false);
    }
    d.address = 0; d.data = saved[0] ^ 1; edge(true, false); edge(true, false);
    check(d.q != pipeline, "destructive continuation left pipeline unchanged");
    d.guest_held = 1; d.freeze = 1; edge(false, false);
    for (unsigned a = 0; a != 1025; ++a) access(a, true, image[a]);
    // Two pipeline writes must still leave a pending restore, not toggle it away.
    access(1024, true, image[1024]);
    check(d.q == pipeline, "restored pipeline differs"); equal("restored pipeline differs");
    for (unsigned a = 0; a != 1025; ++a)
        check(access(a, false, 0) == image[a], "restored RAM readback differs");
    d.freeze = 0; d.request = 0; edge(false, false); check(!d.frozen, "release failed");
    // No warm-up guest edge is allowed. The pending pipeline must survive
    // host-only clocks AND another save taken before the first resumed edge.
    for (unsigned i = 0; i != 8; ++i) {
        d.address = i; edge(false, false); equal("restored pipeline differs before first guest edge");
    }
    d.freeze = 1; edge(false, false);
    check(access(1024, false, 0) == pipeline, "recapture lost pending restored pipeline");
    d.freeze = 0; d.request = 0; edge(false, false); d.guest_held = 0;
    memory = saved;
    for (unsigned i = 0; i != 20000; ++i) {
        d.address = i == 0 ? 0 : word() & 1023; d.data = word();
        d.ce_n = i == 0 ? 0 : (word() & 7) == 0; d.we_n = i == 0 ? 0 : word() & 1;
        pipeline = memory[d.address];
        if (!d.ce_n && !d.we_n) memory[d.address] = d.data;
        edge(true, true); equal();
        check(d.q == (d.ce_n ? 0 : pipeline), "post-restore independent read differs");
    }
    check(paused > 15000, "insufficient backend hold coverage");
    std::printf("ENABLED SNAPSHOT RAM PASS: %u host edges, %u held edges, %u state requests; 18 original cells, all RAM/pipeline bits, old-data, repeated restore and first-edge resume\n", edges, paused, requests);
}
