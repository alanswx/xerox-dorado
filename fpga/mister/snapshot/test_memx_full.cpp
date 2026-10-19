#include "Vmemx_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vmemx_full_fixture d;
static constexpr unsigned transport_mask=(1u<<18)-1;
struct MapState{unsigned row=0,col=0;bool ras=0,cas=0,dout=0,pipe=0,we=0,pending=0,data=0;};
static std::array<MapState,MAPS>map_state{};
static std::array<std::array<uint8_t,65536>,MAPS>memory{},saved_memory{};
static std::array<unsigned,MAPS>writes{};
static std::array<unsigned,SMALLS>small_writes{};
static std::array<uint8_t,INDEX_COUNT>valid{};
static uint32_t rng=0x1378312a;
static unsigned edges,holds,requests,late_changes,walked_bits;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char*why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
template<class W>static unsigned get(const W&bus,unsigned start,unsigned width){
    unsigned shift=start%32;uint64_t value=uint64_t(bus[start/32])>>shift;
    if(shift+width>32)value|=uint64_t(bus[start/32+1])<<(32-shift);
    return unsigned(value)&((1u<<width)-1);
}
static unsigned pack(const MapState&s){return s.row|(s.col<<8)|(unsigned(s.ras)<<16)|(unsigned(s.cas)<<17)|
    (unsigned(s.dout)<<18)|(unsigned(s.pipe)<<19)|(unsigned(s.we)<<20)|(unsigned(s.pending)<<21)|(unsigned(s.data)<<22);}
static void equal(){
    for(unsigned i=0;i<(AUX_BITS+31)/32;++i)check(d.original_ff[i]==d.candidate_ff[i],"board auxiliary state differs");
    for(unsigned b=0;b<MAPS;++b){
        const auto&lo=state_words[MAP_AUX_START+b*2];const auto&hi=state_words[MAP_AUX_START+b*2+1];
        unsigned actual=get(d.original_ff,lo.offset,18)|(get(d.original_ff,hi.offset,5)<<18);
        check(actual==pack(map_state[b]),"independent map history differs");
        check(((d.original_probe>>b)&1)==memory[b][d.probe_row],"independent original map RAM differs");
        check(((d.candidate_probe>>b)&1)==memory[b][d.probe_row],"candidate physical map RAM differs");
    }
    for(unsigned i=0;i<(OUTPUTS+31)/32;++i)check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
}
static void pins(){
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=(edges%257<32)?0:(edges%257<64?0xffffffffu:random_word());
    if constexpr(INPUTS%32)d.stimulus[INPUTS/32]&=(1u<<(INPUTS%32))-1;
    d.probe_row=random_word()&65535;
}
static void pin(unsigned pos,bool v){d.stimulus[pos/32]=(d.stimulus[pos/32]&~(1u<<(pos%32)))|(unsigned(v)<<(pos%32));}
static void cycle(bool run,bool reference,bool late=false,bool compare=true){
    d.run=run;d.run_ref=reference;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.run_ref=!reference;d.eval();++late_changes;}
    if(d.ref_advance){
        for(unsigned b=0;b<MAPS;++b){
            MapState old=map_state[b],next=old;unsigned a=(old.row<<8)|old.col;
            bool ras=(d.map_ras_n>>b)&1,cas=(d.map_cas_n>>b)&1,we=(d.map_we_n>>b)&1;
            next.pipe=memory[b][a];
            if(old.pending){memory[b][a]=old.data;++writes[b];}
            if(!ras&&old.ras)next.row=get(d.map_addresses,b*8,8);
            if(!cas&&old.cas)next.col=get(d.map_addresses,b*8,8);
            next.ras=ras;next.cas=cas;next.we=we;next.pending=!ras&&!cas&&!we&&(old.cas||old.we);
            if(next.pending)next.data=(d.map_data>>b)&1;
            if(!ras&&!cas&&we)next.dout=old.pipe;
            map_state[b]=next;
        }
        for(unsigned i=0;i<SMALLS;++i)if((d.small_write>>i)&1)++small_writes[i];
    }
    if(!d.advance)++holds;
    d.host_clk=1;d.eval();++edges;if(compare)equal();
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool error=false,bool compare=true){
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");check(bool(d.error)==error,"invalid state access accepted");return d.result;
}
static unsigned ram(unsigned b,unsigned a,bool write,unsigned data=0,bool compare=true){return access((MAP_PACKAGES[b]<<17)|a,write,data,false,compare);}
static void physical(unsigned b,unsigned a,unsigned value){
    d.probe_row=a;d.eval();
    for(unsigned j=0;j<MAPS;++j){
        check(((d.candidate_probe>>j)&1)==(j==b?value:saved_memory[j][a]),"physical map target/other socket differs");
        check(((d.original_probe>>j)&1)==saved_memory[j][a],"held original map changed");
    }
    d.probe_row=a^1;d.eval();
    for(unsigned j=0;j<MAPS;++j)check(((d.candidate_probe>>j)&1)==saved_memory[j][a^1],"physical map neighbor differs");
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a)
        memory[b][a]=((a*1103515245u+12345u+b*7919u)^(a>>3)^(a>>7)^(a>>11)^(a>>15))&1;
    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;d.request=1;d.write=1;d.state_index=0;d.state_data=transport_mask;d.eval();
    for(unsigned i=0;i<20000;++i){pins();bool run=(random_word()&3)!=0;cycle(run,run,i%3==0);check(!d.done,"running request accepted");}
    for(unsigned i=0;i<160000;++i){
        if(i%64==0)pins();pin(PIN_CLK_mx_p_,(i/4)&1);cycle(true,true);check(!d.done,"running request accepted");
    }
    for(unsigned b=0;b<MAPS;++b)std::printf("writer map%u=%u\n",b,writes[b]);
    for(unsigned i=0;i<SMALLS;++i)std::printf("writer small%u=%u\n",i,small_writes[i]);
    for(auto n:writes)check(n>0,"original map writer not exercised");
    for(auto n:small_writes)check(n>0,"original small-file writer not exercised");
    bool pending=false;
    for(unsigned i=0;i<10000&&!pending;++i){pins();cycle(true,true);for(const auto&s:map_state)pending|=s.pending;}
    check(pending,"no pending map write captured");
    d.request=0;cycle(false,false);d.freeze=1;d.held=0;d.request=1;d.write=1;cycle(false,false);check(!d.frozen&&!d.done,"unheld freeze accepted");
    d.held=1;cycle(false,false);check(d.frozen&&!d.done,"entry request accepted");
    auto saved_inputs=d.stimulus;std::array<unsigned,WORDS>image{};saved_memory=memory;
    for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];valid[w.word]=1;image[i]=access(w.word,false);check(image[i]==get(d.original_ff,w.offset,w.width),"aux state export differs");}
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a){valid[(MAP_PACKAGES[b]<<17)|a]=1;check(ram(b,a,false)==memory[b][a],"map RAM export differs");}
    for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];for(unsigned bit=0;bit<w.width;++bit){
        unsigned value=image[i]^(1u<<bit);access(w.word,true,value,false,false);
        check(access(w.word,false,0,false,false)==value,"aux bit write/readback differs");
        for(unsigned j=0;j<WORDS;++j)check(get(d.candidate_ff,state_words[j].offset,state_words[j].width)==(i==j?value:image[j]),"aux write changed another field");
        access(w.word,true,image[i]);++walked_bits;
    }access(w.word,true,image[i]|(transport_mask^((1u<<w.width)-1)));check(access(w.word,false)==image[i],"aux padding changed state");}
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a){
        unsigned value=memory[b][a]^1;ram(b,a,true,value|(transport_mask^1u),false);
        check(ram(b,a,false,0,false)==value,"map bit write/readback differs");physical(b,a,value);
        ram(b,a,true,memory[b][a]);++walked_bits;
    }
    for(unsigned i=0;i<INDEX_COUNT;++i)if(!valid[i])access(i,true,random_word()&transport_mask,true);
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a)check(ram(b,a,false)==memory[b][a],"invalid write mutated map RAM");
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==image[i],"invalid write mutated aux");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<10000;++i){pins();cycle(true,false,false,false);}
    bool changed=false;
    for(unsigned i=0;i<WORDS;++i){changed|=get(d.candidate_ff,state_words[i].offset,state_words[i].width)!=image[i];check(get(d.original_ff,state_words[i].offset,state_words[i].width)==image[i],"reference advanced during continuation");}
    check(changed,"candidate continuation did not change state");
    d.stimulus=saved_inputs;cycle(false,false,false,false);d.freeze=1;d.held=1;cycle(false,false,false,false);
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a)ram(b,a,true,!memory[b][a],false);
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,image[i]^((1u<<state_words[i].width)-1),false,false);
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a){
        check(ram(b,a,false,0,false)==!memory[b][a],"destructive map write differs");d.probe_row=a;d.eval();check(((d.candidate_probe>>b)&1)==!memory[b][a],"destructive physical map differs");
    }
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false,0,false,false)==(image[i]^((1u<<state_words[i].width)-1)),"destructive aux write differs");
    for(unsigned b=0;b<MAPS;++b)for(unsigned a=0;a<65536;++a)ram(b,a,true,memory[b][a],false);
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,image[i],false,false);
    for(unsigned a=0;a<65536;++a){d.probe_row=a;d.eval();equal();}
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<17;++i){pins();cycle(false,false);}
    d.freeze=1;d.held=1;cycle(false,false);
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==image[i],"recapture before guest advance lost aux");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<20000;++i){pins();bool run=i==0||(random_word()&3)!=0;cycle(run,run,i%3==0);}
    check(walked_bits==STATE_BITS,"incomplete modeled state bit walk");
    std::printf("FULL MemX RESTORE PASS host=%u held=%u requests=%u late_changes=%u bits=%u invalid_index_space=%u post_restore=20000\n",edges,holds,requests,late_changes,walked_bits,INDEX_COUNT);
}
