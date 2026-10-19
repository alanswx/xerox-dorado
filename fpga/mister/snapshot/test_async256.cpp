#include "Vasync256_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vasync256_fixture d;
static unsigned edges, held_edges, requests, ordinary_writes_mb, ordinary_writes_f;
static uint32_t rng = 0x2567071;
static std::array<unsigned,256> mb{}, f{};
static unsigned random_word() {
    rng ^= rng<<13; rng ^= rng>>17; rng ^= rng<<5; return rng;
}
static void check(bool ok,const char *why) {
    if (!ok) { std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why); std::exit(1); }
}
static void memory_check(bool reference,const char *why) {
    for (unsigned a=0;a<256;++a) {
        check(((d.memory_mb[a/8]>>(4*(a%8)))&15)==mb[a],why);
        check(((d.memory_f[a/32]>>(a%32))&1)==f[a],why);
    }
    if (reference) {
        for (unsigned i=0;i<32;++i)
            check(d.memory_mb[i]==d.ref_memory_mb[i],"original MB7071H differs");
        for (unsigned i=0;i<8;++i)
            check(d.memory_f[i]==d.ref_memory_f[i],"original F10414 differs");
    }
}
static void output_check(bool reference) {
    check(d.q_mb==(d.select_mb?0:mb[d.address])&&d.q_f==(d.ce_f?0:f[d.address]),
          "asynchronous output differs");
    if (reference) check(d.q_mb==d.ref_q_mb&&d.q_f==d.ref_q_f,"original output differs");
}
static void edge(bool advance,bool reference) {
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
    d.advance=advance;d.eval(); // late actual enable, with no preparatory host edge
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
static void ordinary(bool advance=true,bool reference=true) {
    if (advance) {
        if (!d.select_mb&&!d.we_mb_n) { mb[d.address]=d.data; ++ordinary_writes_mb; }
        if (!d.ce_f&&!d.we_f_n) { f[d.address]=d.data&1; ++ordinary_writes_f; }
    } else ++held_edges;
    edge(advance,reference);
    if (d.request) check(!d.done,"running request accepted");
    memory_check(reference,advance?"ordinary oracle differs":"held guest state changed");
    output_check(reference);
}
static void pins() {
    d.address=random_word()&255;d.data=random_word()&15;
    // Exercise all four independent block selects and frequent real writes.
    d.select_mb=(random_word()&1)?0:(random_word()&15);
    d.ce_f=random_word()&1;d.we_mb_n=random_word()&1;d.we_f_n=random_word()&1;
}
static void access(unsigned index,bool write,unsigned data=0,unsigned data_f=16) {
    const bool valid=index<256;
    const unsigned old_mb=valid?mb[index]:0,old_f=valid?f[index]:0;
    d.index=index;d.write=write;d.state_data=data;
    d.state_data_f=data_f<16?data_f:data;d.request=1;
    // Assert advance adversarially: held freeze must still dominate writes.
    edge(true,false);++requests;
    check(d.done==3,"missing response");
    check(d.error==(valid?0:3),"invalid request accepted");
    check(d.result_mb==(write?0:old_mb)&&d.result_f==(write?0:old_f),"read/write response differs");
    if (write&&valid) { mb[index]=data&15;f[index]=d.state_data_f&1; }
    memory_check(false,"held state differs");
}

int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    d.freeze=0;d.held=0;d.request=0;d.write=0;
    d.select_mb=0;d.ce_f=0;d.we_mb_n=0;d.we_f_n=0;
    for (unsigned a=0;a<256;++a) { d.address=a;d.data=(a^(a>>4)^7)&15;ordinary(); }
    // Running requests and freeze without the caller's hold cannot grant access.
    d.request=1;d.write=1;d.index=7;d.state_data=15;
    ordinary();check(!d.done,"running request accepted");
    d.freeze=1;ordinary();check(!d.done&&!d.frozen,"unheld freeze accepted");
    d.freeze=0;d.request=0;
    for (unsigned i=0;i<8000;++i) {
        pins();bool advance=(random_word()&3)!=0;ordinary(advance,advance);
    }
    const auto saved_mb=mb,saved_f=f;
    d.address=91;d.data=6;d.select_mb=0;d.ce_f=0;d.we_mb_n=0;d.we_f_n=0;
    d.freeze=1;d.held=1;d.request=1;d.write=1;d.index=0;d.state_data=15;
    edge(true,false);check(d.frozen==3&&!d.done,"entry request accepted");
    memory_check(true,"held state differs");
    for (unsigned a=0;a<256;++a) access(a,false);
    // Walk every physical bit, including the width1 RAM's padding behavior.
    for (unsigned a=0;a<256;++a) {
        for (unsigned bit=0;bit<4;++bit) {
            access(a,true,saved_mb[a]^(1u<<bit));
            access(a,false);
        }
        access(a,true,saved_mb[a]);
    }
    for (unsigned a=256;a<512;++a) { access(a,true,15);access(a,false); }
    for (unsigned a=0;a<256;++a) access(a,true,saved_mb[a],saved_f[a]);
    check(mb==saved_mb&&f==saved_f,"bit walk restore failed");
    memory_check(true,"held state differs");
    d.freeze=0;d.request=0;edge(false,false);d.held=0;
    output_check(true); // combinational guest read port restored before guest edge
    for (unsigned i=0;i<3000;++i) { pins();ordinary(true,false); }
    check(mb!=saved_mb&&f!=saved_f,"destructive continuation failed");
    d.freeze=1;d.held=1;d.request=0;edge(false,false);
    for (unsigned a=0;a<256;++a) access(a,true,saved_mb[a],saved_f[a]);
    memory_check(true,"restored state differs from original");
    d.address=91;d.data=6;d.select_mb=0;d.ce_f=0;d.we_mb_n=0;d.we_f_n=0;
    d.freeze=0;d.request=1;d.write=1;d.index=0;d.state_data=15;d.eval();
    output_check(true);
    edge(false,false);check(!d.done&&!d.frozen,"release request accepted");
    d.request=0;d.held=0;
    for (unsigned i=0;i<17;++i) ordinary(false,false);
    ordinary(); // first actual guest edge performs exactly one level-write
    for (unsigned i=0;i<20000;++i) {
        pins();bool advance=(random_word()&3)!=0;ordinary(advance,advance);
    }
    std::printf("ASYNC256 PASS host=%u held=%u requests=%u bits=1280 invalid_indices=256 "
                "ordinary_writes_mb=%u ordinary_writes_f=%u post_restore=20000\n",
                edges,held_edges,requests,ordinary_writes_mb,ordinary_writes_f);
    return 0;
}
