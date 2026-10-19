#include "Vmemc_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vmemc_full_fixture d;
static uint32_t rng=0x1353ca49;
static unsigned edges,holds,late_changes,requests,walked_bits;
static std::array<unsigned,WRITERS>writes{};
static uint32_t random_word() { rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng; }
static void check(bool ok,const char *why) {
    if (!ok) { std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1); }
}
template<class W>static unsigned get(const W&bus,unsigned start,unsigned width) {
    unsigned value=0;
    for (unsigned i=0;i<width;++i)value|=((bus[(start+i)/32]>>((start+i)%32))&1u)<<i;
    return value;
}
static void equal() {
    for (const auto &w:state_words)
        check(get(d.original_state,w.offset,w.width)==get(d.candidate_state,w.offset,w.width),"board physical state differs");
    if (!(d.freeze&&d.held))for (unsigned i=0;i<(OUTPUTS+31)/32;++i)
        check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
}
static void pins() {
    for (unsigned i=0;i<(INPUTS+31)/32;++i)
        d.stimulus[i]=(edges%257<32)?0:(edges%257<64?0xffffffffu:random_word());
    if constexpr(INPUTS%32)d.stimulus[INPUTS/32]&=(1u<<(INPUTS%32))-1;
}
static void cycle(bool run,bool reference,bool late=false,bool compare=true) {
    d.run=run;d.run_ref=reference;d.host_clk=0;d.eval();
    if (late) { d.run=!run;d.run_ref=!reference;d.eval();++late_changes; }
    if (d.ref_advance)for (unsigned i=0;i<WRITERS;++i)if ((d.ref_write>>i)&1)++writes[i];
    if (!d.advance)++holds;
    d.host_clk=1;d.eval();++edges;
    if (compare)equal();
}
static unsigned access(unsigned word,bool write,unsigned data=0,bool error=false,bool compare=true) {
    d.state_index=word;d.write=write;d.state_data=data;d.request=1;
    cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");
    check(bool(d.error)==error,"invalid state access accepted");
    return d.result;
}

int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;
    d.request=1;d.write=0;d.state_index=0;d.state_data=0;d.eval();
    for (unsigned i=0;i<20000;++i) {
        pins();bool advance=(random_word()&3)!=0;
        cycle(advance,advance,i%3==0);
        check(!d.done,"running request accepted");
    }
    // Let board decoders and sampled phases settle around actual clock
    // pulses. White-noise pins alone miss some multi-edge write sequences.
    for(unsigned i=0;i<160000;++i){
        if(i%64==0)pins();
        d.stimulus[PIN_MemBase_0/32]|=1u<<(PIN_MemBase_0%32);
        unsigned pos=PIN_CLK_mc_p_;
        d.stimulus[pos/32]=(d.stimulus[pos/32]&~(1u<<(pos%32)))|(((i/4)&1u)<<(pos%32));
        cycle(true,true);
        check(!d.done,"running request accepted");
    }
    auto pin=[](unsigned pos,bool value){d.stimulus[pos/32]=(d.stimulus[pos/32]&~(1u<<(pos%32)))|(unsigned(value)<<(pos%32));};
    for(unsigned mode=0;mode<8;++mode){
        for(unsigned word=0;word<(INPUTS+31)/32;++word)d.stimulus[word]=0;
        for(unsigned i=0;i<4096;++i){
            pin(PIN_CLK_mc_p_,(i/4)&1);
            pin(PIN_MemSH,mode&1);
            pin(PIN_CLKEnable_p_b,mode&2);
            pin(PIN_MemClkEnable_p_a,mode&4);
            pin(PIN_DMuxClk,(i/16)&1);
            // Load zero into configuration through the normal DMux pins.
            pin(PIN_DMuxData,0);
            cycle(true,true);
        }
    }
    const unsigned decode_pins[]={PIN_FA_eq_1_p_,PIN_FF_0mem_p_,PIN_FF_1mem,PIN_FF_2,PIN_FF_3,PIN_FF_4,PIN_FF_5,PIN_FF_6,PIN_FF_7};
    for(unsigned mode=0;mode<1024;++mode){
        for(unsigned word=0;word<(INPUTS+31)/32;++word)d.stimulus[word]=0;
        for(unsigned p=PIN_MAR_00_p_;p<=PIN_MAR_15_p_;++p)pin(p,1);
        for(unsigned b=0;b<9;++b)pin(decode_pins[b],(mode>>b)&1);
        pin(PIN_MemBase_0,(mode>>9)&1);
        for(unsigned i=0;i<256;++i){
            pin(PIN_CLK_mc_p_,(i/4)&1);pin(PIN_MemSH,(i/64)&1);
            cycle(true,true);check(!d.done,"running request accepted");
        }
    }
    d.request=0;cycle(false,false);
    d.freeze=1;d.held=0;cycle(false,false);check(!d.frozen,"unheld freeze accepted");
    d.held=1;d.request=1;d.write=1;d.state_index=0;d.state_data=(1u<<18)-1;
    cycle(false,false);check(d.frozen&&!d.done,"entry request accepted");
    auto saved_inputs=d.stimulus;
    std::array<unsigned,WORDS>image{};
    std::array<bool,INDEX_COUNT>valid{};
    for (unsigned i=0;i<WORDS;++i) {
        const auto &w=state_words[i];valid[w.word]=true;
        image[i]=access(w.word,false);
        check(image[i]==get(d.original_state,w.offset,w.width),"state export differs");
    }
    for (unsigned i=0;i<WORDS;++i) {
        const auto &w=state_words[i];
        for (unsigned bit=0;bit<w.width;++bit) {
            unsigned value=image[i]^(1u<<bit);
            access(w.word,true,value,false,false);
            check(access(w.word,false,0,false,false)==value,"state bit write/readback differs");
            for (unsigned j=0;j<WORDS;++j)
                check(get(d.candidate_state,state_words[j].offset,state_words[j].width)==(i==j?value:image[j]),"state write affected another package/row");
            access(w.word,true,image[i]);++walked_bits;
        }
        // Unused high response bits are zero; write padding cannot create
        // hidden state or clobber another package's register.
        access(w.word,true,image[i]|(((1u<<18)-1)^((1u<<w.width)-1)));
        check(access(w.word,false)==image[i],"padding changed physical state");
    }
    for (unsigned word=0;word<INDEX_COUNT;++word)if (!valid[word])access(word,true,random_word()&((1u<<18)-1),true);
    for (unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==image[i],"invalid access mutated state");
    // Reference remains untouched at the saved real-board boundary while
    // the candidate alone executes its original feedback and strobes.
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for (unsigned i=0;i<10000;++i) { pins();cycle(true,false,false,false); }
    bool changed=false;
    for (unsigned i=0;i<WORDS;++i) {
        changed|=get(d.candidate_state,state_words[i].offset,state_words[i].width)!=image[i];
        check(get(d.original_state,state_words[i].offset,state_words[i].width)==image[i],"reference advanced during continuation");
    }
    check(changed,"candidate-only continuation did not change state");
    d.stimulus=saved_inputs;cycle(false,false,false,false);
    d.freeze=1;d.held=1;cycle(false,false,false,false);check(d.frozen,"second freeze failed");
    for (unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,image[i]^((1u<<state_words[i].width)-1),false,false);
    for (unsigned i=0;i<WORDS;++i)
        check(access(state_words[i].word,false,0,false,false)==(image[i]^((1u<<state_words[i].width)-1)),"destructive state write differs");
    for (unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,image[i],false,false);
    equal();
    d.freeze=0;d.request=0;cycle(false,false);check(!d.frozen,"release failed");
    for (unsigned i=0;i<20;++i) { pins();cycle(false,false); }
    d.freeze=1;cycle(false,false);
    for (unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==image[i],"recapture differs before resumed guest edge");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    // No warm-up guest edge. The first real edge and20,000 further clocks
    // must match the original, including actual pending write windows.
    for (unsigned i=0;i<20000;++i) {
        pins();bool advance=(random_word()&3)!=0;cycle(advance,advance,i%3==0);
    }
    check(walked_bits==STATE_BITS&&late_changes>10000&&holds>10000,"insufficient coverage");
    for(unsigned i=0;i<WRITERS;++i)if(!writes[i])std::fprintf(stderr,"UNEXERCISED writer=%u\n",i);
    for (auto n:writes)check(n>0,"unexercised original RAM writer");
    std::printf("FULL MEMC RESTORE PASS: %u host edges, %u held, %u late-enable changes, %u requests; all%u modeled bits,%u state words,%u output bits; real writes=",edges,holds,late_changes,requests,STATE_BITS,WORDS,OUTPUTS);
    for (auto n:writes)std::printf(" %u",n);
    std::puts("; no machine/physical qualification");
}
