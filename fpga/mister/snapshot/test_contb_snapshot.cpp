#include "Vcontb_snapshot_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static constexpr uint32_t mask=(1u<<18)-1;
static Vcontb_snapshot_fixture d;
static std::array<std::array<uint32_t,1024>,8> memory;
static std::array<uint32_t,8> pipeline{};
static std::array<unsigned,8> writes{},read_seen{};
static unsigned edges,held_edges,requests,late_changes;
static uint32_t rng=0xc07b1041;
static uint32_t random_word() {
    rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;
}
static void check(bool ok,const char *why) {
    if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}
}
template<class W> static uint32_t get(const W& bus,unsigned start,unsigned width) {
    uint32_t value=0;
    for(unsigned i=0;i<width;++i)value|=((bus[(start+i)/32]>>((start+i)%32))&1u)<<i;
    return value;
}
static void input(unsigned n,bool value) {
    uint32_t bit=1u<<(n%32);
    d.stimulus[n/32]=(d.stimulus[n/32]&~bit)|(value?bit:0);
}
static void random_inputs() {
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=random_word();
}
static void equal(bool independent=true) {
    for(unsigned i=0;i<8;++i) {
        uint32_t reference=get(d.original_raw,i*18,18);
        check(get(d.candidate_raw,i*18,18)==reference,"board raw pipeline differs");
        if(independent)check(reference==pipeline[i],"independent read pipeline differs");
    }
    for(unsigned i=0;i<(OUTPUTS+31)/32;++i)
        check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
}
static void cycle(bool run,bool late=false,bool compare=true) {
    // run settles before the falling edge as in altclkctrl. A deliberately
    // late change tests that the RAM uses the gate's latched enable, rather
    // than the asynchronous/current request. Inputs already settle pre-edge.
    d.run=run;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.eval();++late_changes;}
    if(d.advance) {
        for(unsigned i=0;i<8;++i) {
            unsigned address=get(d.addresses,i*10,10);
            pipeline[i]=memory[i][address];++read_seen[i];
            if(!((d.ce_n>>i)&1)&&!((d.we_n>>i)&1)) {
                memory[i][address]=get(d.data_bits,i*18,18);++writes[i];
            }
        }
    } else ++held_edges;
    d.host_clk=1;d.eval();++edges;
    if(compare)equal();
}
static uint32_t access(unsigned bank,unsigned word,bool write,uint32_t data,
                       bool error=false,bool compare=true) {
    d.state_index=(bank<<11)|word;d.write=write;d.state_data=data;d.request=1;
    cycle(false,false,compare);++requests;
    check(d.done,"missing state response");
    check(bool(d.error)==error,"invalid state address accepted or valid rejected");
    return d.result;
}

int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    for(unsigned i=0;i<8;++i)for(unsigned a=0;a<1024;++a)
        memory[i][a]=(uint32_t(a*1103515245u+12345u+i*7919u))&mask;
    d.host_clk=1;d.run=0;d.freeze=0;d.held=0;
    d.request=1;d.write=1;d.state_index=1;d.state_data=mask;d.eval();
    for(unsigned i=0;i<20000;++i) {
        random_inputs();
        // The real board chooses its own half, address, parity and write
        // data. Keep all decoder/control inputs live; do not force internal
        // WE or address nets to make the comparison easier.
        cycle((random_word()&3)!=0,(i%3)==0);
        check(!d.done,"state access accepted while running");
    }
    for(unsigned i=0;i<8;++i)check(writes[i]>50,"insufficient real half-bank write coverage");
    d.request=0;cycle(false);
    d.freeze=1;d.held=0;cycle(false);
    check(!d.frozen,"unheld freeze accepted");
    d.held=1;d.request=1;d.write=1;d.state_index=0;d.state_data=mask;
    cycle(false);check(d.frozen&&!d.done,"freeze entry accepted request");
    std::array<std::array<uint32_t,1025>,8> image;
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i) {
        image[i][a]=access(i,a,false,0);
        check(image[i][a]==(a==1024?pipeline[i]:memory[i][a]),"export differs");
    }
    // Include every invalid row in every half-bank, with interleaved bank
    // changes; none may alias RAM or the pipeline.
    for(unsigned a=1025;a<2048;++a)for(unsigned i=0;i<8;++i)
        access(i,a,true,random_word()&mask,true);
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(access(i,a,false,0)==image[i][a],"invalid write mutated state");
    // Destructively change ALL RAM and pipeline bits through the candidate's
    // state port while both boards' other FFs remain held, then restore. A
    // full-board continue/restore requires exporting those other FFs too.
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        access(i,a,true,image[i][a]^mask,false,false);
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(access(i,a,false,0,false,false)==(image[i][a]^mask),"destructive state write failed");
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        access(i,a,true,image[i][a],false,false);
    equal();
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(access(i,a,false,0)==image[i][a],"restored export differs");
    d.freeze=0;d.request=0;cycle(false);
    check(!d.frozen,"freeze release failed");
    for(unsigned i=0;i<20;++i){random_inputs();cycle(false);}
    // Recapture before the first actual guest edge must retain pending raw
    // read state even though board address/CE pins changed while held.
    d.freeze=1;cycle(false);
    for(unsigned i=0;i<8;++i)check(access(i,1024,false,0)==pipeline[i],"recapture differs");
    d.freeze=0;d.request=0;cycle(false);d.held=0;
    for(unsigned i=0;i<20000;++i) {
        random_inputs();cycle((random_word()&3)!=0,(i%3)==0);
    }
    check(held_edges>15000&&late_changes>10000,"hold/clock-gate coverage insufficient");
    std::printf("ACTUAL CONTB IM PASS: %u host edges, %u held edges, %u late-enable changes, %u state accesses; %u board outputs, all8 halves including parity/raw read pipelines; real-strobe writes=",
                edges,held_edges,late_changes,requests,OUTPUTS);
    for(auto n:writes)std::printf(" %u",n);
    std::puts("; complete machine restore/timing not qualified");
}
