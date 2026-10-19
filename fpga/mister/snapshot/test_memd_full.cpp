#include "Vmemd_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vmemd_full_fixture d;
static constexpr unsigned transport_mask=(1u<<18)-1;
static std::array<std::array<uint32_t,4096>,BANKS> memory{};
static std::array<uint32_t,BANKS> pipeline{};
static std::array<unsigned,BANKS>writes{};
static std::array<unsigned,SMALLS>small_writes{};
static std::array<unsigned,2>storage_words{};
static bool storage_history=false;
static unsigned storage_writes;
static uint32_t rng=0x296554d0;
static unsigned edges,holds,requests,late_changes,walked_bits;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char *why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
template<class W>static unsigned get(const W&bus,unsigned start,unsigned width){
    unsigned value=0;for(unsigned i=0;i<width;++i)value|=((bus[(start+i)/32]>>((start+i)%32))&1u)<<i;return value;
}
template<class W>static unsigned bank(const W&bus,unsigned b){return get(bus,BANK_OFFSETS[b],BANK_WIDTHS[b]);}
static unsigned mask(unsigned b){return(1u<<BANK_WIDTHS[b])-1;}
static void equal(){
    for(const auto&w:state_words)check(get(d.original_ff,w.offset,w.width)==get(d.candidate_ff,w.offset,w.width),"board auxiliary state differs");
    for(unsigned i=0;i<3;++i)check(d.original_raw[i]==d.candidate_raw[i],"board raw pipeline differs");
    for(unsigned b=0;b<BANKS;++b){
        check(bank(d.original_raw,b)==pipeline[b],"independent RAM read pipeline differs");
        check(bank(d.original_probe,b)==memory[b][d.probe_row],"independent original RAM probe differs");
        check(bank(d.candidate_probe,b)==memory[b][d.probe_row],"candidate RAM probe differs");
    }
    for(unsigned i=0;i<(OUTPUTS+31)/32;++i)check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
    for(unsigned i=0;i<2;++i)check(get(d.original_ff,state_words[WORDS-3+i].offset,18)==storage_words[i],"independent storage input pipeline differs");
    check(get(d.original_ff,state_words[WORDS-1].offset,1)==unsigned(storage_history),"independent storage input clock history differs");
}
static void pins(){
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=(edges%257<32)?0:(edges%257<64?0xffffffffu:random_word());
    if constexpr(INPUTS%32)d.stimulus[INPUTS/32]&=(1u<<(INPUTS%32))-1;
    d.probe_row=random_word()&4095;
}
static void pin(unsigned pos,bool value){d.stimulus[pos/32]=(d.stimulus[pos/32]&~(1u<<(pos%32)))|(unsigned(value)<<(pos%32));}
static void cycle(bool run,bool reference,bool late=false,bool compare=true){
    d.run=run;d.run_ref=reference;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.run_ref=!reference;d.eval();++late_changes;}
    if(d.ref_advance){
        if(d.storage_clock&&!storage_history){storage_words[1]=storage_words[0];storage_words[0]=d.storage_data;++storage_writes;}
        storage_history=d.storage_clock;
        for(unsigned b=0;b<BANKS;++b){
            unsigned a=get(d.addresses,b*12,12);pipeline[b]=memory[b][a];
            if(!((d.we_n>>b)&1)&&!((d.ce_n>>b)&1)){memory[b][a]=bank(d.data_bits,b);++writes[b];}
        }
        for(unsigned i=0;i<SMALLS;++i)if((d.small_write>>i)&1)++small_writes[i];
    }
    if(!d.advance)++holds;
    d.host_clk=1;d.eval();++edges;if(compare)equal();
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool error=false,bool compare=true){
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");check(bool(d.error)==error,"invalid state index accepted or valid rejected");return d.result;
}
static unsigned ram(unsigned b,unsigned word,bool write,unsigned data=0,bool compare=true){return access((BANK_PACKAGES[b]<<13)|word,write,data,false,compare);}
static void physical(unsigned b,unsigned a,unsigned value){
    d.probe_row=a&4095;d.eval();
    for(unsigned j=0;j<BANKS;++j){
        unsigned actual=a==4096?bank(d.candidate_raw,j):bank(d.candidate_probe,j);
        check(actual==(j==b?value:(a==4096?pipeline[j]:memory[j][a])),"physical RAM target/other bank differs");
        check(bank(d.original_probe,j)==memory[j][d.probe_row],"held original RAM changed");
    }
    if(a<4096){
        d.probe_row=a^1;d.eval();
        for(unsigned j=0;j<BANKS;++j)check(bank(d.candidate_probe,j)==memory[j][a^1],"physical RAM neighbor differs");
    }
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4096;++a)
        memory[b][a]=((a*1103515245u+12345u+b*7919u)^(a>>3)^(a>>7)^(a>>11))&mask(b);
    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;d.request=1;d.write=1;d.state_index=0;d.state_data=transport_mask;d.eval();
    for(unsigned i=0;i<20000;++i){pins();bool run=(random_word()&3)!=0;cycle(run,run,i%3==0);check(!d.done,"running request accepted");}
    // Stable normal inputs and board-clock pulses exercise multi-edge write
    // windows that independent white-noise pins alone can miss.
    for(unsigned i=0;i<160000;++i){
        if(i%64==0)pins();pin(PIN_CLK_md_p_,(i/4)&1);cycle(true,true);check(!d.done,"running request accepted");
    }
    for(unsigned b=0;b<BANKS;++b)std::printf("writer bank%u=%u\n",b,writes[b]);
    for(unsigned i=0;i<SMALLS;++i)std::printf("writer small%u=%u\n",i,small_writes[i]);
    for(auto count:writes)check(count>0,"original cache RAM writer not exercised");
    for(auto count:small_writes)check(count>0,"original small RAM writer not exercised");
    check(storage_writes>1,"original storage input writer not exercised");
    d.request=0;cycle(false,false);d.freeze=1;d.held=0;d.request=1;d.write=1;
    cycle(false,false);check(!d.frozen&&!d.done,"unheld freeze accepted");
    d.held=1;d.request=1;d.write=1;d.state_index=0;cycle(false,false);check(d.frozen&&!d.done,"entry request accepted");
    auto saved_inputs=d.stimulus;
    std::array<unsigned,WORDS>aux_image{};
    std::array<std::array<unsigned,4097>,BANKS>image{};
    for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];aux_image[i]=access(w.word,false);check(aux_image[i]==get(d.original_ff,w.offset,w.width),"aux state export differs");}
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a){image[b][a]=ram(b,a,false);check(image[b][a]==(a==4096?pipeline[b]:memory[b][a]),"RAM/pipeline state export differs");}
    for(unsigned i=0;i<WORDS;++i){
        const auto&w=state_words[i];
        for(unsigned bit=0;bit<w.width;++bit){
            unsigned value=aux_image[i]^(1u<<bit);access(w.word,true,value,false,false);
            check(access(w.word,false,0,false,false)==value,"aux bit write/readback differs");
            for(unsigned j=0;j<WORDS;++j)check(get(d.candidate_ff,state_words[j].offset,state_words[j].width)==(i==j?value:aux_image[j]),"aux write changed another package/row");
            access(w.word,true,aux_image[i]);++walked_bits;
        }
        access(w.word,true,aux_image[i]|(transport_mask^((1u<<w.width)-1)));
        check(access(w.word,false)==aux_image[i],"aux padding changed state");
    }
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a)for(unsigned bit=0;bit<BANK_WIDTHS[b];++bit){
        unsigned value=image[b][a]^(1u<<bit);ram(b,a,true,value|(transport_mask^mask(b)),false);
        check(ram(b,a,false,0,false)==value,"RAM bit write/readback differs");physical(b,a,value);
        ram(b,a,true,image[b][a]);++walked_bits;
    }
    std::array<bool,INDEX_COUNT>valid{};
    for(const auto&w:state_words)valid[w.word]=true;
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a)valid[(BANK_PACKAGES[b]<<13)|a]=true;
    for(unsigned i=0;i<INDEX_COUNT;++i)if(!valid[i])access(i,true,random_word()&transport_mask,true);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a)check(ram(b,a,false)==image[b][a],"invalid write mutated RAM/pipeline");
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==aux_image[i],"invalid write mutated aux");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<10000;++i){pins();cycle(true,false,false,false);}
    bool changed=false;
    for(unsigned i=0;i<WORDS;++i){changed|=get(d.candidate_ff,state_words[i].offset,state_words[i].width)!=aux_image[i];check(get(d.original_ff,state_words[i].offset,state_words[i].width)==aux_image[i],"reference advanced during candidate continuation");}
    check(changed,"candidate-only continuation did not change state");
    d.stimulus=saved_inputs;cycle(false,false,false,false);d.freeze=1;d.held=1;cycle(false,false,false,false);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a)ram(b,a,true,image[b][a]^mask(b),false);
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,aux_image[i]^((1u<<state_words[i].width)-1),false,false);
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a){
        check(ram(b,a,false,0,false)==(image[b][a]^mask(b)),"destructive RAM state write differs");
        if(a<4096){d.probe_row=a;d.eval();check(bank(d.candidate_probe,b)==(memory[b][a]^mask(b)),"destructive physical RAM probe differs");}
    }
    for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false,0,false,false)==(aux_image[i]^((1u<<state_words[i].width)-1)),"destructive aux state write differs");
    for(unsigned b=0;b<BANKS;++b)for(unsigned a=0;a<4097;++a)ram(b,a,true,image[b][a],false);
    for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,aux_image[i],false,false);
    for(unsigned a=0;a<4096;++a){d.probe_row=a;d.eval();equal();}
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<17;++i){pins();cycle(false,false);}
    d.freeze=1;d.held=1;cycle(false,false);
    for(unsigned b=0;b<BANKS;++b)check(ram(b,4096,false)==pipeline[b],"pending restore recapture lost raw pipeline");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<20000;++i){pins();bool run=i==0||(random_word()&3)!=0;cycle(run,run,i%3==0);}
    check(walked_bits==STATE_BITS,"incomplete modeled bit walk");
    std::printf("FULL MemD RESTORE PASS host=%u held=%u requests=%u late_changes=%u bits=%u invalid_index_space=%u storage_writes=%u post_restore=20000\n",edges,holds,requests,late_changes,walked_bits,INDEX_COUNT,storage_writes);
}
