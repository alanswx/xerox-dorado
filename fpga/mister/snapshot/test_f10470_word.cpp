#include "Vf10470_word_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vf10470_word_fixture d;
static constexpr unsigned cases=6,transport_mask=(1u<<18)-1;
static constexpr std::array<unsigned,cases> widths={1,3,5,6,9,18};
static std::array<std::array<unsigned,4096>,cases> memory{},saved{};
static std::array<unsigned,cases> pipeline{},saved_pipeline{};
static uint32_t rng=0x2710415;
static unsigned edges,holds,requests,walked_bits,collisions;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static unsigned mask(unsigned k){return (1u<<widths[k])-1;}
static bool selected(unsigned k){return !d.ce_n;}
static void check(bool ok,const char *why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
static void equal(bool reference,const char *why){
    for(unsigned k=0;k<cases;++k){
        check(d.q[k]==(selected(k)?pipeline[k]:0),why);
        if(reference)check(d.q[k]==d.original_q[k],"untouched original output differs");
    }
}
static void edge(bool run,bool reference){
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
    d.advance=run;d.eval(); // no preparatory edge for the actual advance
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
static void ordinary(bool run=true,bool reference=true){
    if(run){
        for(unsigned k=0;k<cases;++k){
            pipeline[k]=memory[k][d.address];
            if(selected(k)&&!d.we_n){memory[k][d.address]=d.data&mask(k);++collisions;}
        }
    }else ++holds;
    edge(run,reference);
    check(!d.done,"running state request accepted");
    equal(reference,run?"ordinary old-data oracle differs":"backend hold changed read pipeline");
}
static void pins(){d.address=random_word()&4095;d.data=random_word()&transport_mask;d.ce_n=random_word()&1;d.we_n=random_word()&1;}
static void access(unsigned index,bool write,bool valid=true){
    d.word_index=index;d.write=write;d.request=1;edge(true,false);++requests;
    check(d.frozen==63&&d.done==63,"missing frozen state response");
    check(d.error==(valid?0:63),"invalid index accepted or valid index rejected");
}
static void read_image(unsigned a,const std::array<std::array<unsigned,4096>,cases>&image,
                       const std::array<unsigned,cases>&pipe){
    access(a,false);
    for(unsigned k=0;k<cases;++k)check(d.result[k]==(a==4096?pipe[k]:image[k][a]),"RAM/pipeline export or padding differs");
}
int main(int argc,char **argv){
    Verilated::commandArgs(argc,argv);
    d.freeze=0;d.held=0;d.request=1;d.write=1;d.word_index=0;d.ce_n=0;d.we_n=1;
    ordinary();d.freeze=1;ordinary();check(!d.frozen,"unheld freeze accepted");
    d.freeze=0;d.request=0;d.we_n=0;
    for(unsigned a=0;a<4096;++a){d.address=a;d.data=random_word()&transport_mask;ordinary();}
    for(unsigned i=0;i<10000;++i){pins();bool run=(random_word()&3)!=0;ordinary(run,run);}
    // Deliberately separate RAM[0] from its raw read pipeline for every width.
    d.ce_n=0;d.we_n=0;d.address=0;d.data=0x15555;ordinary();d.data=0x2aaaa;ordinary();
    for(unsigned k=0;k<cases;++k)check(memory[k][0]!=pipeline[k],"collision setup failed");
    saved=memory;saved_pipeline=pipeline;d.ce_n=1;d.freeze=1;d.held=1;
    d.request=1;d.write=1;d.word_index=0;for(auto &x:d.state_data)x=transport_mask;
    edge(true,false);check(d.frozen==63&&!d.done,"entry request accepted");equal(true,"freeze changed pipeline");
    for(unsigned a=0;a<4097;++a)read_image(a,saved,saved_pipeline);
    // Every actual bit plus every transport-padding bit is independently
    // toggled/read/restored, including the distinct raw-pipeline word.
    for(unsigned a=0;a<4097;++a){
        for(unsigned bit=0;bit<18;++bit){
            for(unsigned k=0;k<cases;++k)d.state_data[k]=(a==4096?saved_pipeline[k]:saved[k][a])^(1u<<bit);
            access(a,true);access(a,false);
            for(unsigned k=0;k<cases;++k){
                unsigned expected=((a==4096?saved_pipeline[k]:saved[k][a])^(1u<<bit))&mask(k);
                check(d.result[k]==expected,"bit/padding write readback differs");
                if(bit<widths[k])++walked_bits;
            }
            for(unsigned k=0;k<cases;++k)d.state_data[k]=a==4096?saved_pipeline[k]:saved[k][a];
            access(a,true);
        }
    }
    for(unsigned a=4097;a<8192;++a){for(auto &x:d.state_data)x=transport_mask;access(a,true,false);access(a,false,false);}
    for(unsigned a=0;a<4097;++a)read_image(a,saved,saved_pipeline);
    d.ce_n=0;d.eval();equal(true,"scan changed logical pipeline");
    d.freeze=0;d.request=0;edge(false,false);d.held=0;
    // Candidate-only execution destroys RAM and pipeline while originals stop.
    for(unsigned a=0;a<4096;++a){d.address=a;d.data=transport_mask;ordinary(true,false);}
    d.address=0;d.data=0x2aaaa;ordinary(true,false);ordinary(true,false);
    for(unsigned k=0;k<cases;++k)check(pipeline[k]!=saved_pipeline[k],"candidate-only pipeline unchanged");
    d.freeze=1;d.held=1;edge(false,false);
    for(unsigned a=0;a<4097;++a){
        for(unsigned k=0;k<cases;++k)d.state_data[k]=a==4096?saved_pipeline[k]:saved[k][a];
        access(a,true);
    }
    // Repeated pipeline writes must retain pending restore through scans.
    access(4096,true);memory=saved;pipeline=saved_pipeline;
    for(unsigned a=0;a<4097;++a)read_image(a,saved,saved_pipeline);
    equal(true,"restored raw pipeline differs");
    d.freeze=0;d.request=0;edge(false,false);d.held=0;
    for(unsigned i=0;i<17;++i){pins();ordinary(false,false);}
    d.freeze=1;d.held=1;edge(false,false);read_image(4096,saved,saved_pipeline);
    d.freeze=0;d.request=0;edge(false,false);d.held=0;
    for(unsigned i=0;i<20000;++i){pins();bool run=i==0 || (random_word()&3)!=0;ordinary(run,run);}
    check(walked_bits==172074&&holds>7000&&collisions>10000,"insufficient coverage");
    std::printf("F10470 WORD PASS host=%u held=%u requests=%u walked_bits=%u collisions=%u widths=1,3,5,6,9,18 connected_CE first_edge_resume post_restore=20000\n",edges,holds,requests,walked_bits,collisions);
}
