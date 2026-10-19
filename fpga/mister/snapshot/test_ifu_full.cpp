#include "Vifu_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vifu_full_fixture d;
static constexpr unsigned transport_mask=(1u<<18)-1;
static std::array<std::array<uint32_t,1024>,BANKS> memory{};
static std::array<uint32_t,BANKS> pipeline{};
static std::array<unsigned,BANKS>writes{};
static uint32_t rng=0x28653f04;
static unsigned edges,holds,requests,late_changes,walked_bits;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char *why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
template<class W>static unsigned get(const W&bus,unsigned start,unsigned width){
    unsigned value=0;for(unsigned i=0;i<width;++i)value|=((bus[(start+i)/32]>>((start+i)%32))&1u)<<i;return value;
}
static unsigned raw(unsigned packed,unsigned bank){return (packed>>BANK_OFFSETS[bank])&((1u<<BANK_WIDTHS[bank])-1);}
static void equal(){
    for(unsigned i=0;i<FF_COUNT;++i)check(get(d.original_ff,FF_OFFSETS[i],FF_WIDTHS[i])==get(d.candidate_ff,FF_OFFSETS[i],FF_WIDTHS[i]),"board FF state differs");
    check(d.original_raw==d.candidate_raw,"board raw pipeline differs");
    for(unsigned i=0;i<BANKS;++i)check(raw(d.original_raw,i)==pipeline[i],"independent RAM read pipeline differs");
    for(unsigned i=0;i<(OUTPUTS+31)/32;++i)check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
}
static void pins(){
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=(edges%257<32)?0:(edges%257<64?0xffffffffu:random_word());
    // Verilator requires unused high bits in a wide C++ input container
    // to be clear. They are outside the Verilog stimulus port's width.
    if constexpr(INPUTS%32)d.stimulus[INPUTS/32]&=(1u<<(INPUTS%32))-1;
}
static void cycle(bool run,bool reference,bool late=false,bool compare=true){
    d.run=run;d.run_ref=reference;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.run_ref=!reference;d.eval();++late_changes;}
    if(d.ref_advance)for(unsigned i=0;i<BANKS;++i){
        unsigned a=get(d.addresses,i*10,10);pipeline[i]=memory[i][a];
        // CE remains omitted in both original and adapter. In this2-state
        // model it reads low; no four-state or explicit tie-off claim.
        if(!((d.we_n>>i)&1)){memory[i][a]=raw(d.data_bits,i);++writes[i];}
    }
    if(!d.advance)++holds;
    d.host_clk=1;d.eval();++edges;if(compare)equal();
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool error=false,bool compare=true){
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");check(bool(d.error)==error,"invalid state index accepted or valid rejected");return d.result;
}
static unsigned ram(unsigned bank,unsigned word,bool write,unsigned data=0,bool compare=true){return access((BANK_PACKAGES[bank]<<11)|word,write,data,false,compare);}
int main(int argc,char **argv){
    Verilated::commandArgs(argc,argv);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1024;++a)memory[b][a]=(uint32_t(a*1103515245u+12345u+b*7919u))&((1u<<BANK_WIDTHS[b])-1);
    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;d.request=1;d.write=1;d.state_index=0;d.state_data=transport_mask;d.eval();
    for(unsigned i=0;i<20000;++i){pins();bool run=(random_word()&3)!=0;cycle(run,run,i%3==0);check(!d.done,"running request accepted");}
    for(auto count:writes)check(count>0,"original RAM writer not exercised");
    d.request=0;cycle(false,false);d.freeze=1;d.held=0;cycle(false,false);check(!d.frozen,"unheld freeze accepted");
    d.held=1;d.request=1;d.write=1;d.state_index=0;cycle(false,false);check(d.frozen&&!d.done,"entry request accepted");
    auto saved_inputs=d.stimulus;
    std::array<unsigned,FF_COUNT>ff_image{};
    std::array<std::array<unsigned,1025>,BANKS>image{};
    for(unsigned i=0;i<FF_COUNT;++i){
        ff_image[i]=access(FF_WORDS[i],false);
        unsigned expected=get(d.original_ff,FF_OFFSETS[i],FF_WIDTHS[i]);
        if(ff_image[i]!=expected)std::fprintf(stderr,"export i=%u word=%u width=%u actual=%x expected=%x\n",i,FF_WORDS[i],FF_WIDTHS[i],ff_image[i],expected);
        check(ff_image[i]==expected,"FF state export differs");
    }
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a){image[b][a]=ram(b,a,false);check(image[b][a]==(a==1024?pipeline[b]:memory[b][a]),"RAM/pipeline state export differs");}
    for(unsigned i=0;i<FF_COUNT;++i)for(unsigned bit=0;bit<FF_WIDTHS[i];++bit){
        unsigned value=ff_image[i]^(1u<<bit);access(FF_WORDS[i],true,value,false,false);
        check(access(FF_WORDS[i],false,0,false,false)==value,"FF bit write/readback differs");
        for(unsigned j=0;j<FF_COUNT;++j)check(get(d.candidate_ff,FF_OFFSETS[j],FF_WIDTHS[j])==(j==i?value:ff_image[j]),"FF write changed another package");
        check(d.original_raw==d.candidate_raw,"FF write changed RAM pipeline");
        access(FF_WORDS[i],true,ff_image[i]);++walked_bits;
    }
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)for(unsigned bit=0;bit<BANK_WIDTHS[b];++bit){
        unsigned value=image[b][a]^(1u<<bit),wmask=(1u<<BANK_WIDTHS[b])-1;
        ram(b,a,true,value|(transport_mask^wmask),false);
        check(ram(b,a,false,0,false)==value,"RAM bit write/readback differs");
        unsigned neighbor=(b+1)%BANKS;
        check(ram(neighbor,a,false,0,false)==image[neighbor][a],"RAM write changed another bank");
        check(ram(b,(a+1)%1025,false,0,false)==image[b][(a+1)%1025],"RAM write changed another row");
        ram(b,a,true,image[b][a]);++walked_bits;
    }
    std::array<bool,INDEX_COUNT>valid{};
    for(unsigned i=0;i<FF_COUNT;++i)valid[FF_WORDS[i]]=true;
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)valid[(BANK_PACKAGES[b]<<11)|a]=true;
    for(unsigned i=0;i<INDEX_COUNT;++i)if(!valid[i])access(i,true,random_word()&transport_mask,true);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)check(ram(b,a,false)==image[b][a],"invalid write mutated RAM/pipeline");
    for(unsigned i=0;i<FF_COUNT;++i)check(access(FF_WORDS[i],false)==ff_image[i],"invalid write mutated FF");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<10000;++i){pins();cycle(true,false,false,false);}
    bool changed=false;
    for(unsigned i=0;i<FF_COUNT;++i){changed|=get(d.candidate_ff,FF_OFFSETS[i],FF_WIDTHS[i])!=ff_image[i];check(get(d.original_ff,FF_OFFSETS[i],FF_WIDTHS[i])==ff_image[i],"reference advanced during candidate continuation");}
    check(changed,"candidate-only continuation did not change state");
    d.stimulus=saved_inputs;cycle(false,false,false,false);d.freeze=1;d.held=1;cycle(false,false,false,false);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)ram(b,a,true,image[b][a]^((1u<<BANK_WIDTHS[b])-1),false);
    for(unsigned i=0;i<FF_COUNT;++i)access(FF_WORDS[i],true,ff_image[i]^((1u<<FF_WIDTHS[i])-1),false,false);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)check(ram(b,a,false,0,false)==(image[b][a]^((1u<<BANK_WIDTHS[b])-1)),"destructive RAM state write differs");
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<1025;++a)ram(b,a,true,image[b][a],false);
    for(unsigned i=0;i<FF_COUNT;++i)access(FF_WORDS[i],true,ff_image[i],false,false);
    equal(); // restored inputs/state must agree before any guest edge
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<17;++i)cycle(false,false);
    d.freeze=1;d.held=1;cycle(false,false);
    for(unsigned b=0;b<BANKS;++b)check(ram(b,1024,false)==pipeline[b],"pending restore recapture lost raw pipeline");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<20000;++i){pins();bool run=i==0 || (random_word()&3)!=0;cycle(run,run,i%3==0);}
    check(walked_bits==STATE_BITS,"incomplete modeled bit walk");
    std::printf("FULL IFU RESTORE PASS host=%u held=%u requests=%u late_changes=%u bits=%u invalid_index_space=%u writers=",edges,holds,requests,late_changes,walked_bits,INDEX_COUNT);
    for(unsigned b=0;b<BANKS;++b)std::printf("%s%u",b?",":"",writes[b]);std::puts(" post_restore=20000");
}
