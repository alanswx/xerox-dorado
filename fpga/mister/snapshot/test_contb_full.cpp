#include "Vcontb_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static constexpr uint32_t mask=(1u<<18)-1;
static Vcontb_full_fixture d;
static std::array<std::array<uint32_t,1024>,8> memory;
static std::array<uint32_t,8> pipeline{};
static std::array<unsigned,8> writes{};
static unsigned edges,held_edges,requests,late_changes,bit_checks;
static uint32_t rng=0xcf001041;
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
static void random_inputs() {
    for(unsigned i=0;i<(INPUTS+31)/32;++i)d.stimulus[i]=random_word();
}
static void equal() {
    for(unsigned i=0;i<FF_COUNT;++i)
        check(get(d.candidate_ff,FF_OFFSETS[i],FF_WIDTHS[i])==get(d.original_ff,FF_OFFSETS[i],FF_WIDTHS[i]),
              "board FF state differs");
    for(unsigned i=0;i<8;++i) {
        uint32_t reference=get(d.original_raw,i*18,18);
        check(get(d.candidate_raw,i*18,18)==reference,"board raw pipeline differs");
        check(reference==pipeline[i],"independent read pipeline differs");
    }
    for(unsigned i=0;i<(OUTPUTS+31)/32;++i)
        check(d.original_outputs[i]==d.candidate_outputs[i],"board output differs");
}
static void cycle(bool run,bool reference,bool late=false,bool compare=true) {
    d.run=run;d.run_ref=reference;d.host_clk=0;d.eval();
    if(late){d.run=!run;d.run_ref=!reference;d.eval();++late_changes;}
    if(d.ref_advance) {
        for(unsigned i=0;i<8;++i) {
            unsigned address=get(d.addresses,i*10,10);
            pipeline[i]=memory[i][address];
            if(!((d.ce_n>>i)&1)&&!((d.we_n>>i)&1)) {
                memory[i][address]=get(d.data_bits,i*18,18);++writes[i];
            }
        }
    }
    if(!d.advance)++held_edges;
    d.host_clk=1;d.eval();++edges;
    if(compare)equal();
}
static uint32_t access(unsigned index,bool write,uint32_t data,
                       bool error=false,bool compare=true) {
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;
    cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");
    check(bool(d.error)==error,"invalid state address accepted or valid rejected");
    return d.result;
}
static uint32_t ram_access(unsigned bank,unsigned word,bool write,uint32_t data,
                          bool error=false,bool compare=true) {
    return access((bank<<11)|word,write,data,error,compare);
}
static uint32_t ff_access(unsigned cell,bool write,uint32_t data,bool compare=true) {
    return access((8<<11)|cell,write,data,false,compare);
}

int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    for(unsigned i=0;i<8;++i)for(unsigned a=0;a<1024;++a)
        memory[i][a]=(uint32_t(a*1103515245u+12345u+i*7919u))&mask;
    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;
    d.request=1;d.write=1;d.state_index=8<<11;d.state_data=mask;d.eval();
    for(unsigned i=0;i<20000;++i) {
        random_inputs();bool advance=(random_word()&3)!=0;
        cycle(advance,advance,(i%3)==0);
        check(!d.done,"state access accepted while running");
    }
    for(unsigned i=0;i<8;++i)check(writes[i]>50,"insufficient real half-bank writes");
    d.request=0;cycle(false,false);
    d.freeze=1;d.held=0;cycle(false,false);
    check(!d.frozen,"unheld freeze accepted");
    d.held=1;d.request=1;d.write=1;d.state_index=8<<11;d.state_data=mask;
    cycle(false,false);check(d.frozen&&!d.done,"freeze entry accepted request");
    auto saved_inputs=d.stimulus;
    std::array<std::array<uint32_t,1025>,8> image;
    std::array<uint32_t,FF_COUNT> ff_image;
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i) {
        image[i][a]=ram_access(i,a,false,0);
        check(image[i][a]==(a==1024?pipeline[i]:memory[i][a]),"RAM export differs");
    }
    for(unsigned i=0;i<FF_COUNT;++i) {
        ff_image[i]=ff_access(i,false,0);
        check(ff_image[i]==get(d.original_ff,FF_OFFSETS[i],FF_WIDTHS[i]),"FF export differs");
    }
    // Walk all209 bits independently of natural board activity. Unwritten
    // packages, setup captures and edge histories must remain unchanged.
    for(unsigned i=0;i<FF_COUNT;++i)for(unsigned b=0;b<FF_WIDTHS[i];++b) {
        uint32_t value=ff_image[i]^(1u<<b);
        ff_access(i,true,value,false);
        check(ff_access(i,false,0,false)==value,"FF bit write/readback differs");
        for(unsigned j=0;j<FF_COUNT;++j)
            check(get(d.candidate_ff,FF_OFFSETS[j],FF_WIDTHS[j])==(i==j?value:ff_image[j]),
                  "FF write changed wrong register/package");
        ff_access(i,true,ff_image[i]);++bit_checks;
    }
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(ram_access(i,a,false,0)==image[i][a],"RAM changed by FF write");
    // Every invalid15-bit address, including unimplemented groups, rejects
    // without changing state. Subsequent exhaustive RAM/FF readback checks.
    for(unsigned index=0;index<32768;++index) {
        unsigned group=index>>11,word=index&2047;
        if((group<8&&word<=1024)||(group==8&&word<FF_COUNT))continue;
        access(index,true,random_word()&mask,true);
    }
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(ram_access(i,a,false,0)==image[i][a],"invalid write mutated RAM");
    for(unsigned i=0;i<FF_COUNT;++i)
        check(ff_access(i,false,0)==ff_image[i],"invalid write mutated FF state");
    // Candidate alone continues under its REAL board logic, changing RAM,
    // visible outputs, pre-capture and history. The original remains stopped
    // at the captured boundary; no reference state is patched or imported.
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    for(unsigned i=0;i<10000;++i){random_inputs();cycle(true,false,false,false);}
    bool changed=false;
    for(unsigned i=0;i<FF_COUNT;++i)
        changed|=get(d.candidate_ff,FF_OFFSETS[i],FF_WIDTHS[i])!=ff_image[i];
    check(changed,"candidate-only continuation did not change FF state");
    d.stimulus=saved_inputs;
    cycle(false,false,false,false);d.freeze=1;d.held=1;
    cycle(false,false,false,false);check(d.frozen,"second freeze failed");
    // Destructive all-bit state-port writes additionally cover every RAM bit
    // even if the natural continuation only touched part of the memory.
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        ram_access(i,a,true,image[i][a]^mask,false,false);
    for(unsigned i=0;i<FF_COUNT;++i)
        ff_access(i,true,ff_image[i]^((1u<<FF_WIDTHS[i])-1),false);
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(ram_access(i,a,false,0,false,false)==(image[i][a]^mask),"destructive RAM write failed");
    for(unsigned i=0;i<FF_COUNT;++i)
        check(ff_access(i,false,0,false)==(ff_image[i]^((1u<<FF_WIDTHS[i])-1)),"destructive FF write failed");
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        ram_access(i,a,true,image[i][a],false,false);
    for(unsigned i=0;i<FF_COUNT;++i)ff_access(i,true,ff_image[i],false);
    equal();
    for(unsigned a=0;a<1025;++a)for(unsigned i=0;i<8;++i)
        check(ram_access(i,a,false,0)==image[i][a],"restored RAM export differs");
    for(unsigned i=0;i<FF_COUNT;++i)
        check(ff_access(i,false,0)==ff_image[i],"restored FF export differs");
    d.freeze=0;d.request=0;cycle(false,false);check(!d.frozen,"freeze release failed");
    for(unsigned i=0;i<20;++i){random_inputs();cycle(false,false);}
    d.freeze=1;cycle(false,false);
    for(unsigned i=0;i<8;++i)
        check(ram_access(i,1024,false,0)==pipeline[i],"recapture pipeline differs");
    for(unsigned i=0;i<FF_COUNT;++i)
        check(ff_access(i,false,0)==ff_image[i],"recapture FF state differs");
    d.freeze=0;d.request=0;cycle(false,false);d.held=0;
    // The very FIRST guest edge and subsequent unmodified execution must
    // match. No warm-up edge or host-completed instruction is inserted.
    for(unsigned i=0;i<20000;++i) {
        random_inputs();bool advance=(random_word()&3)!=0;
        cycle(advance,advance,(i%3)==0);
    }
    check(bit_checks==209&&held_edges>30000&&late_changes>10000,"insufficient full-state/hold coverage");
    std::printf("FULL CONTB RESTORE PASS: %u host edges, %u held edges, %u late-enable changes, %u state accesses; all209 FF/history bits,147456 RAM bits,144 read bits,71 outputs; real-strobe writes=",
                edges,held_edges,late_changes,requests);
    for(auto n:writes)std::printf(" %u",n);
    std::puts("; candidate-only continuation, restore and first-edge execution; no machine/physical qualification");
}
