#include "Vsnapshot_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static constexpr uint64_t mask = (1ULL << 34) - 1;
static Vsnapshot_fixture d;
static uint64_t rng = 0x20415415;
static unsigned edges, requests;
static uint64_t random_word() {
    rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17;
    return rng & mask;
}
static void check(bool ok, const char *why) {
    if (!ok) { std::fprintf(stderr, "FAIL edge=%u: %s\n", edges, why); std::exit(1); }
}
static void edge(bool reference) {
    d.clk = 0; d.ref_clk = 0; d.eval();
    d.clk = 1; d.ref_clk = reference; d.eval();
    ++edges;
    d.clk = 0; d.ref_clk = 0; d.eval();
}
static void equal() { check(d.q == d.original_q, "original cell output differs"); }
static uint64_t access(unsigned word, bool write, uint64_t data,
                   bool expect_error = false) {
    d.word_index = word; d.write = write;
    d.state_data = data; d.request = 1;
    edge(false); ++requests;
    check(d.done, "missing frozen response");
    check(bool(d.error) == expect_error, "invalid state access accepted or valid access rejected");
    return d.result;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    std::array<uint64_t, 1024> memory{}, saved{};
    uint64_t pipeline = 0;
    d.freeze = 0; d.ce_n = 1; d.we_n = 1;
    // Requests while running must have no state-access effect or response.
    d.request = 1; d.write = 1; d.word_index = 0; d.state_data = 1;
    edge(true); check(!d.done, "state access accepted while running");
    d.request = 0; d.ce_n = 0; d.we_n = 0;
    for (unsigned a = 0; a != 1024; ++a) {
        d.address = a; d.data = random_word();
        edge(true); memory[a] = d.data; equal();
    }
    // Random ordinary operation is checked against both original cells and
    // a separate array model, including same-address read/write collisions.
    for (unsigned i = 0; i != 10000; ++i) {
        d.address = random_word() & 1023; d.data = random_word();
        d.ce_n = (random_word() & 7) == 0; d.we_n = random_word() & 1;
        pipeline = memory[d.address];
        if (!d.ce_n && !d.we_n) memory[d.address] = d.data;
        edge(true); equal();
        check(d.q == (d.ce_n ? 0 : pipeline), "independent RAM/read pipeline differs");
    }
    // Leave the registered read deliberately different from current RAM[0].
    d.address = 0; d.ce_n = 0; d.we_n = 0;
    pipeline = memory[0]; d.data = pipeline ^ mask;
    edge(true); memory[0] = d.data; equal();
    check(d.q == pipeline && pipeline != memory[0], "collision fixture failed");
    saved = memory;

    // Freeze BEFORE the next guest edge; writes and a request on the entry
    // edge must not happen. Reference cell clock now remains physically held.
    d.freeze = 1; d.request = 1; d.write = 1;
    d.word_index = 0; d.state_data = memory[0] ^ mask;
    edge(false); check(d.frozen && !d.done, "freeze entry boundary differs"); equal();
    std::array<uint64_t, 1025> image{};
    for (unsigned w = 0; w != 1025; ++w) {
        image[w] = access(w, false, 0);
        check(image[w] == (w == 1024 ? pipeline : saved[w]),
              "exported RAM or saved read pipeline differs");
        equal();
        if ((w & 63) == 0) {
            d.request = 0; edge(false); check(!d.done, "response did not clear"); equal();
        }
    }
    // Invalid writes must not alias valid addresses or the read pipeline.
    for (unsigned w = 1025; w != 2048; ++w) {
        access(w, true, random_word(), true); equal();
    }
    for (unsigned w = 0; w != 1025; ++w)
        check(access(w, false, 0) == image[w], "invalid write changed saved state");

    // Continue only the candidate, changing EVERY RAM word and its pipeline.
    // Original cells stay at the captured state as an independent restore oracle.
    d.freeze = 0; d.request = 1; d.write = 1; d.ce_n = 0; d.we_n = 0;
    for (unsigned a = 0; a != 1024; ++a) {
        d.address = a; d.data = saved[a] ^ mask;
        edge(false); check(!d.frozen && !d.done, "resume accepted a state request");
    }
    d.address = 0; d.data = saved[0] ^ 1; edge(false); edge(false);
    d.freeze = 1; d.request = 0; edge(false);
    check(d.q != pipeline, "destructive continuation did not change the read pipeline");
    for (unsigned w = 0; w != 1025; ++w)
        access(w, true, image[w]);
    equal(); check(d.q == pipeline, "restored saved read pipeline differs");
    for (unsigned w = 0; w != 1025; ++w)
        check(access(w, false, 0) == image[w], "restored state readback differs");

    // Resume without warm-up edges: the first edge and subsequent trace must
    // agree, including CE output gating and old-before-write behavior.
    d.freeze = 0; d.request = 0;
    memory = saved;
    for (unsigned i = 0; i != 20000; ++i) {
        d.address = i == 0 ? 0 : random_word() & 1023;
        d.data = random_word(); d.ce_n = i == 0 ? 0 : (random_word() & 7) == 0;
        d.we_n = i == 0 ? 0 : random_word() & 1;
        pipeline = memory[d.address];
        if (!d.ce_n && !d.we_n) memory[d.address] = d.data;
        edge(true); equal();
        check(d.q == (d.ce_n ? 0 : pipeline), "post-restore independent pipeline differs");
    }
    std::printf("SNAPSHOT RAM PASS: %u edges, %u accesses; 34 original cells, all RAM/read-pipeline bits, invalid indices, first-edge resume and 30000 random steps\n", edges, requests);
}
