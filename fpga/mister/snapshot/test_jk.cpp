#include "Vjk_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vjk_fixture d;
static std::array<unsigned,4> state{};
static uint32_t rng=0x10135049;
static unsigned edges,held,restored,transitions;
static uint32_t random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char *why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
static unsigned bit(unsigned x,unsigned n){return (x>>n)&1;}
static unsigned next(unsigned s,unsigned p,unsigned mode){
    const unsigned ja=1-bit((mode&1)?(s>>2):p,0);
    const unsigned ka=1-bit((mode&1)?(s>>2):p,1);
    const unsigned jb=1-bit((mode&2)?(s>>2):p,2);
    const unsigned kb=1-bit((mode&2)?(s>>2):p,3);
    const bool clock=bit(p,8)&&!bit(s,6);
    unsigned a=bit(s,0),b=bit(s,1);
    // Pins[4:7] are A reset/set and B reset/set; set wins both halves.
    if(bit(p,5))a=1;else if(bit(p,4))a=0;else if(clock)a=(ja&(!a))|((!ka)&a);
    if(bit(p,7))b=1;else if(bit(p,6))b=0;else if(clock)b=(jb&(!b))|((!kb)&b);
    return a|(b<<1)|((p&15)<<2)|(bit(p,8)<<6);
}
static void observe(bool reference){
    for(unsigned i=0;i<4;++i){
        check(d.result[i]==state[i],"state oracle differs");
        check(((d.outputs>>(i*4))&15)==(bit(state[i],0)|((!bit(state[i],0))<<1)|(bit(state[i],1)<<2)|((!bit(state[i],1))<<3)),"pin oracle differs");
        if(reference)check(d.candidate_state[i]==d.original_state[i],"original state differs");
    }
    if(reference)check(d.outputs==d.original_outputs,"original outputs differ");
}
static void edge(bool advance,bool reference,bool compare=true){
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
    d.advance=advance;d.eval(); // actual enable may arrive without a prep edge
    if(d.hold){++held;if(d.write)for(auto &s:state)s=d.data&127;}
    else if(advance){for(unsigned i=0;i<4;++i)state[i]=next(state[i],d.pins,i);++transitions;}
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;
    observe(compare);
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);
    d.hold=0;d.write=0;d.pins=0;d.data=0;d.eval();observe(true);
    for(unsigned i=0;i<60000;++i){d.pins=random_word()&511;bool run=(random_word()&3)!=0;edge(run,run);}
    // All 512 pin combinations, including simultaneous set/reset and clock
    // high/low, are exercised repeatedly in each independent PRECAP mode.
    for(unsigned repeat=0;repeat<4;++repeat)for(unsigned p=0;p<512;++p){d.pins=p;edge(true,true);}
    auto saved=state;unsigned saved_pins=d.pins;
    d.hold=1;d.write=1;
    for(unsigned value=0;value<128;++value){d.data=value|0x3ff80;d.pins=random_word()&511;edge(true,false,false);++restored;}
    // Restore each independent parameterized instance, then compare the
    // untouched original before any guest clock can repair lost history.
    d.write=0;d.hold=0;d.pins=saved_pins;
    for(unsigned i=0;i<10000;++i){d.pins=random_word()&511;edge(true,false,false);}
    d.hold=1;d.write=1;d.restore_individual=1;
    for(unsigned i=0;i<4;++i){d.restore_data[i]=saved[i];state[i]=saved[i];}
    // Individual restore uses the same data port selected per fixture cell.
    d.host_clk=0;d.ref_clk=0;d.advance=1;d.eval();d.host_clk=1;d.eval();++edges;
    d.host_clk=0;d.advance=0;d.eval();d.pins=saved_pins;d.eval();observe(true);
    d.hold=0;d.write=0;d.restore_individual=0;d.eval();observe(true);
    for(unsigned i=0;i<17;++i)edge(false,false);
    edge(true,true);
    for(unsigned i=0;i<20000;++i){d.pins=random_word()&511;bool run=(random_word()&3)!=0;edge(run,run);}
    std::printf("JK PASS host=%u held=%u transitions=%u all_state_words=%u modes=4 post_restore=20000\n",edges,held,transitions,restored);
}
