#include "Vdispy_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
static Vdispy_full_fixture d;
static uint32_t rng=0xd1500057;
static unsigned edges,holds,requests,late_changes,walked;
static std::array<std::array<uint8_t,1024>,RAMS>memory{};
static std::array<unsigned,RAMS>writes{};
static std::array<uint8_t,INDEX_COUNT>valid{};
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool b,const char*w){if(!b){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,w);std::exit(1);}}
static unsigned bits(uint64_t bus,unsigned start,unsigned width){return unsigned(bus>>start)&((1u<<width)-1);}
template<class W>static unsigned bits(const W&bus,unsigned start,unsigned width){
    uint64_t x=uint64_t(bus[start/32])>>(start%32);
    if(start%32+width>32)x|=uint64_t(bus[start/32+1])<<(32-start%32);
    return unsigned(x)&((1u<<width)-1);
}
static void equal(bool outputs=true){
    for(unsigned i=0;i<(STATE_BITS+31)/32;++i)check(d.original_state[i]==d.candidate_state[i],"board state differs");
    for(unsigned i=0;i<RAMS;++i){
        const auto&r=rams[i];unsigned row=d.probe_row&(r.words-1),expected=memory[i][row];
        check(bits(d.original_probe,i*4,4)==expected&&bits(d.candidate_probe,i*4,4)==expected,"independent physical RAM oracle differs");
    }
    if(outputs)check(d.original_outputs==d.candidate_outputs,"board output differs");
}
static void pins(){
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=edges%257<32?0:edges%257<64?0xffffffff:random_word();
    if constexpr(INPUTS%32)d.stimulus[INPUTS/32]&=(1u<<(INPUTS%32))-1;
    d.probe_row=random_word()&1023;
}
static void cycle(bool run,bool reference,bool late=false,bool compare=true){
    d.run=run;d.run_ref=run&&reference;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.run_ref=reference?!run:false;d.eval();++late_changes;}
    if(d.ref_advance)for(unsigned i=0;i<RAMS;++i)if((d.ram_write>>i)&1){
        unsigned address=bits(d.ram_address,i*10,10),data=bits(d.ram_data,i*4,4);
        check(address<rams[i].words,"original RAM address outside geometry");memory[i][address]=data&((1u<<rams[i].width)-1);++writes[i];
    }
    if(!d.advance)++holds;
    d.host_clk=1;d.eval();++edges;if(compare)equal(!(d.freeze&&d.held));
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool invalid=false,bool compare=true){
    d.state_index=index;d.request=1;d.write=write;d.state_data=data&0x3ffff;cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");check(bool(d.error)==invalid,"invalid index accepted");return d.result;
}
static auto state(){std::array<uint32_t,(STATE_BITS+31)/32> x{};for(unsigned i=0;i<x.size();++i)x[i]=d.candidate_state[i];return x;}
static void physical(const std::array<uint32_t,(STATE_BITS+31)/32>&expected){for(unsigned i=0;i<expected.size();++i)check(d.candidate_state[i]==expected[i],"physical write differs");}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);
    for(unsigned i=0;i<RAMS;++i)for(unsigned a=0;a<rams[i].words;++a)memory[i][a]=(a*7+i*3+1)&((1u<<rams[i].width)-1);
    d.eval();equal();
    for(const auto&w:state_words){check(!valid[w.word],"duplicate fixture transport word");valid[w.word]=1;}
    for(unsigned i=0;i<80000;++i){pins();d.request=(random_word()&7)==0;d.write=random_word()&1;d.state_index=random_word()&(INDEX_COUNT-1);d.state_data=random_word()&0x3ffff;
        d.freeze=(random_word()&7)==0;d.held=0;cycle((random_word()&3)!=0,true,i%3==0);check(!d.done,"running request accepted");}
    // 'reference' means the original clock advances with the candidate's actual
    // falling-edge enable, not necessarily the requested run bit.
    d.freeze=d.held=d.request=d.write=1;d.state_data=0x3ffff;cycle(false,false);check(d.frozen&&!d.done,"freeze entry accepted request");
    auto saved=state();auto saved_pins=d.stimulus;unsigned saved_probe=d.probe_row;
    std::array<unsigned,WORDS> image{};
    for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];image[i]=bits(saved,w.offset,w.width);check(access(w.word,false)==image[i],"state export differs");}
    // Unselected consumers remain held; scan read outputs may change.
    for(unsigned i=0;i<INDEX_COUNT;++i)if(!valid[i]){check(access(i,false,0,true)==0,"invalid read padding differs");access(i,true,0x3ffff,true);}
    for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];unsigned mask=(1u<<w.width)-1;
        for(unsigned bit=0;bit<w.width;++bit){auto expected=saved;unsigned pos=w.offset+bit;expected[pos/32]^=1u<<(pos%32);
            unsigned value=image[i]^(1u<<bit);access(w.word,true,value|(~mask&0x3ffff),false,false);physical(expected);
            check(access(w.word,false,0,false,false)==value,"bit read/padding differs");access(w.word,true,image[i],false,false);physical(saved);++walked;}
    }
    check(walked==STATE_BITS,"fixture did not cover every physical bit");
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,~image[i],false,false);
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,image[i],false,false);
    physical(saved);
    d.write=d.request=0;d.freeze=0;cycle(false,false); // release, no guest edge
    d.freeze=1;cycle(false,false);check(d.frozen&&!d.done,"quiet recapture failed");
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==image[i],"quiet recapture state differs");
    d.request=d.write=d.freeze=d.held=0;d.stimulus=saved_pins;d.probe_row=saved_probe;d.eval();equal();
    for(unsigned i=0;i<17;++i)cycle(false,false);
    cycle(true,true); // first resumed instruction must use restored raw pipelines
    for(unsigned i=0;i<20000;++i){pins();cycle((random_word()&3)!=0,true,i%3==0);}
    for(unsigned i=0;i<RAMS;++i){std::printf("writer ram%u=%u\n",i,writes[i]);check(writes[i]>0,"unexercised normal RAM writer");}
    std::printf("FULL DISPLAY RESTORE PASS host=%u held=%u requests=%u late=%u bits=%u indices=%u post_restore=20000\n",edges,holds,requests,late_changes,walked,INDEX_COUNT);
}
