#include "Vf10145_snapshot_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vf10145_snapshot_fixture d;
static unsigned edges,held_edges,accesses;
static uint32_t rng=0xf10145a;
static unsigned random_word() {
    rng^=rng<<13; rng^=rng>>17; rng^=rng<<5; return rng;
}
static void check(bool ok,const char *why) {
    if (!ok) { std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why); std::exit(1); }
}
struct Oracle {
    uint64_t memory=0;
    unsigned we=0,address=0,data=0,ce=0;
    unsigned window() const { return we|(address<<1)|(data<<5)|(ce<<9); }
    void edge() {
        if (d.we_n&&!we&&ce) {
            memory=(memory&~(uint64_t(15)<<(address*4)))|(uint64_t(data)<<(address*4));
        }
        if (!d.we_n) { address=d.address; data=d.data; ce=!d.ce_n; }
        we=d.we_n;
    }
    void check_state(const char *why) const {
        check(d.window==window()&&d.memory==memory,why);
        check(d.q==(d.ce_n?0:((memory>>(d.address*4))&15)),"asynchronous output differs");
    }
};
static Oracle oracle;
static void edge(bool advance,bool reference) {
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
    d.advance=advance;d.eval(); // late actual enable, no preparatory host edge
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
static void ordinary(bool advance=true,bool reference=true) {
    if (advance)oracle.edge(); else ++held_edges;
    edge(advance,reference);
    oracle.check_state(advance?"ordinary oracle state differs":"held guest state changed");
    if (reference)check(d.memory==d.ref_memory&&d.window==d.ref_window&&d.q==d.ref_q,"original cell differs");
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool error=false) {
    d.index=index;d.write=write;d.state_data=data;d.request=1;
    edge(false,false);++accesses;
    check(d.done,"missing response");check(bool(d.error)==error,"invalid request accepted");
    return d.result;
}
static void pins() {
    d.address=random_word()&15;d.data=random_word()&15;
    d.ce_n=random_word()&1;d.we_n=random_word()&1;
}

int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    d.ce_n=1;d.we_n=1;d.freeze=0;d.held=0;
    d.request=1;d.write=1;d.index=0;d.state_data=15;
    oracle.edge();edge(true,true);check(!d.done,"running request accepted");
    oracle.check_state("ordinary oracle state differs");
    d.freeze=1;ordinary();check(!d.frozen&&!d.done,"unheld freeze accepted");
    d.freeze=0;d.request=0;
    for (unsigned a=0;a<16;++a) {
        d.address=a;d.data=a^9;d.ce_n=0;d.we_n=0;ordinary();
        // The END of WE uses captured address/data/CE, not these new pins.
        d.address=(a+3)&15;d.data=a^5;d.ce_n=1;d.we_n=1;ordinary();
    }
    for (unsigned i=0;i<12000;++i) {
        pins();bool advance=(random_word()&3)!=0;ordinary(advance,advance);
    }
    // Freeze inside a low WE window. Current pins then change before the
    // pending trailing edge; restoring only RAM cannot reproduce that edge.
    d.address=7;d.data=((oracle.memory>>28)&15)^15;
    d.ce_n=0;d.we_n=0;ordinary();
    Oracle saved=oracle;
    d.address=12;d.data=3;d.ce_n=1;d.we_n=1;d.eval();
    check(!saved.we&&saved.ce,"pending-write fixture failed");
    d.held=1;d.freeze=1;d.request=1;d.write=1;d.index=0;d.state_data=15;
    edge(false,false);check(d.frozen&&!d.done,"entry request accepted");
    oracle.check_state("entry changed guest state");
    std::array<unsigned,17> image{};
    for (unsigned i=0;i<17;++i) {
        image[i]=access(i,false);
        check(image[i]==(i==16?saved.window():((saved.memory>>(4*i))&15)),"exported state differs");
    }
    for (unsigned bit=0;bit<10;++bit) {
        access(16,true,1u<<bit);
        check(access(16,false)==(1u<<bit),"write-window bit walk differs");
    }
    access(16,true,image[16]);
    for (unsigned a=0;a<16;++a) {
        for (unsigned bit=0;bit<4;++bit) {
            access(a,true,image[a]^(1u<<bit));
            check(access(a,false)==(image[a]^(1u<<bit)),"RAM bit walk differs");
            access(a,true,image[a]);
        }
        access(a,true,0x3f0|image[a]);
        check(access(a,false)==image[a],"RAM padding was stored");
    }
    for (unsigned a=17;a<32;++a) {
        access(a,true,0x3ff,true);check(access(a,false,0,true)==0,"invalid result differs");
    }
    for (unsigned a=0;a<17;++a)
        check(access(a,false)==image[a],"invalid write mutated state");
    oracle.check_state("bit walk failed to restore state");
    // Candidate-only execution must actually alter RAM and its write window.
    // The original clock stays stopped at the saved pending-write boundary.
    d.freeze=0;d.request=0;edge(false,false);check(!d.frozen,"release failed");d.held=0;
    for (unsigned a=0;a<16;++a) {
        d.address=a;d.data=image[a]^15;d.ce_n=0;d.we_n=0;ordinary(true,false);
        d.we_n=1;ordinary(true,false);
    }
    for (unsigned i=0;i<3000;++i) { pins();ordinary(true,false); }
    check(d.memory!=saved.memory&&d.window!=saved.window(),"destructive continuation failed");
    check(d.ref_memory==saved.memory&&d.ref_window==saved.window(),"reference advanced during continuation");
    d.held=1;d.freeze=1;d.request=0;edge(false,false);
    for (unsigned a=0;a<17;++a)access(a,true,image[a]);
    oracle=saved;
    d.address=12;d.data=3;d.ce_n=1;d.we_n=1;d.eval();
    oracle.check_state("restored physical state differs");
    check(d.memory==d.ref_memory&&d.window==d.ref_window&&d.q==d.ref_q,"immediate restore differs from original");
    // Release itself must not perform the saved trailing-edge write.
    d.freeze=0;d.request=1;d.write=1;d.index=0;d.state_data=15;
    edge(false,false);check(!d.done&&!d.frozen,"release request accepted");
    d.request=0;d.held=0;
    for (unsigned i=0;i<9;++i) ordinary(false,false);
    check(d.memory==saved.memory,"pending write committed during host-only edges");
    ordinary();
    check(((d.memory>>28)&15)==saved.data,"first real edge lost pending write");
    for (unsigned i=0;i<20000;++i) {
        pins();bool advance=(random_word()&3)!=0;ordinary(advance,advance);
    }
    check(held_edges>7000,"insufficient backend holds");
    std::printf("F10145 SNAPSHOT PASS: %u host edges, %u held edges, %u state requests; original cell + independent write-window oracle, all74 physical bits, pending write and first-edge resume\n",edges,held_edges,accesses);
}
