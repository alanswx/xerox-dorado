#include "Voscillator_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
static Voscillator_fixture d;
static std::array<uint64_t,10> state{};
static uint32_t rng=0x1115a057;
static unsigned edges,advances,requests,holds,wraps;
static bool frozen;
static uint32_t random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool b,const char*w){if(!b){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,w);std::exit(1);}}
static unsigned sp(unsigned i){return i==9?8:i/3==0?2:i/3==1?4:16;}
static unsigned freq(unsigned i){return i==9?23530:i%3==0?10000:i%3==1?20000:50000;}
static uint64_t next(uint64_t s,unsigned i){
    const uint32_t sum=uint32_t(s)+2*freq(i),limit=sp(i)*266667/16;
    const bool carry=int32_t(sum)>=int32_t(limit);
    if(int32_t(uint32_t(s))>=0&&int32_t(sum)<0)++wraps;
    return uint64_t(carry?sum-limit:sum)|((((s>>32)^unsigned(carry))&1)<<32);
}
static void observe(bool reference){
    for(unsigned i=0;i<10;++i){
        check(d.physical[i]==state[i],"state oracle differs");
        check(((d.outputs>>i)&1)==(state[i]>>32),"output oracle differs");
        if(reference)check(d.original_state[i]==state[i],"original state differs");
    }
    if(reference)check(d.outputs==d.original_outputs,"original pin differs");
}
static void edge(bool advance,bool reference){
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
    d.advance=advance;d.eval(); // actual enable arrives without a prep edge
    const bool hold=d.freeze&&d.held,accept=hold&&frozen&&d.request,valid=d.index<2;
    std::array<uint32_t,10> result{};
    if(hold)++holds;
    if(accept){++requests;
        for(unsigned i=0;i<10;++i){
            unsigned data=d.individual?d.restore_data[i]:d.data;
            if(valid){
                if(d.write){
                    if(!d.index)state[i]=(state[i]&~uint64_t(0x3ffff))|(data&0x3ffff);
                    else state[i]=(state[i]&0x3ffff)|(uint64_t(data&0x7fff)<<18);
                }else result[i]=d.index?unsigned(state[i]>>18):unsigned(state[i]&0x3ffff);
            }
        }
    }
    if(!hold&&advance){++advances;for(unsigned i=0;i<10;++i)state[i]=next(state[i],i);}
    frozen=hold;d.host_clk=1;d.ref_clk=reference&&advance&&!hold;d.eval();++edges;
    check(d.frozen==(hold?1023:0)&&d.done==(accept?1023:0)&&d.error==(accept&&!valid?1023:0),"request handshake differs");
    if(accept)for(unsigned i=0;i<10;++i)check(d.result[i]==result[i],"state read/padding differs");
    observe(reference);d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
static void store(uint64_t v){d.freeze=d.held=d.request=d.write=1;d.individual=0;
    for(unsigned w=0;w<2;++w){d.index=w;d.data=(unsigned(v>>(w*18))| (w?0x38000:0))&0x3ffff;edge(true,false);}}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);d.eval();observe(true);
    for(unsigned i=0;i<120000;++i){
        d.request=random_word()&1;d.write=random_word()&1;d.data=random_word()&0x3ffff;d.index=random_word()&3;
        d.freeze=i%7==0;d.held=0;bool advance=(random_word()&3)!=0;edge(advance,true);
    }
    auto saved=state;
    d.freeze=d.held=d.request=d.write=1;d.data=0x3ffff;d.index=1;edge(true,true); // freeze entry ignores request
    for(unsigned b=0;b<33;++b){store(uint64_t(1)<<b);d.write=0;for(unsigned w=0;w<4;++w){d.index=w;edge(true,false);}}
    for(unsigned k=0;k<2000;++k){d.request=1;d.write=random_word()&1;d.index=random_word()&3;d.data=random_word()&0x3ffff;edge(true,false);}
    // Resume from every oscillator's exact comparison boundary, and from
    // negative and wrapping signed accumulator values. No host arithmetic UB.
    for(unsigned k=0;k<4;++k){
        d.individual=1;d.write=d.request=d.freeze=d.held=1;
        for(unsigned w=0;w<2;++w){d.index=w;for(unsigned i=0;i<10;++i){
            uint32_t acc=k==0?sp(i)*266667/16-2*freq(i):k==1?0x80000000:k==2?0x7fffffff:0xffffffff;
            d.restore_data[i]=(w?((uint64_t(acc)|uint64_t(k&1)<<32)>>18):acc)&0x3ffff;
        }edge(true,false);}
        d.freeze=d.held=0;d.request=0;edge(true,false);edge(true,false);
        d.freeze=d.held=1;edge(false,false);
    }
    d.freeze=d.held=0;d.request=0;for(unsigned i=0;i<10000;++i)edge(true,false);
    d.freeze=d.held=1;edge(false,false);d.request=d.write=d.individual=1;
    for(unsigned w=0;w<2;++w){d.index=w;for(unsigned i=0;i<10;++i)d.restore_data[i]=(unsigned(saved[i]>>(18*w))|(w?0x38000:0))&0x3ffff;edge(true,w==1);}
    d.write=0;for(unsigned w=0;w<4;++w){d.index=w;edge(true,true);}
    // Quiet release/recapture retains raw accumulator/output before first advance.
    d.freeze=0;d.request=1;d.write=1;edge(false,true);
    d.freeze=1;edge(false,true);d.request=0;edge(false,true);d.freeze=d.held=d.write=0;
    for(unsigned i=0;i<29;++i)edge(false,true);
    edge(true,true);
    for(unsigned i=0;i<20000;++i){bool run=(random_word()&3)!=0;edge(run,true);}
    check(wraps>0,"signed wrap not exercised");
    std::printf("OSCILLATOR PASS host=%u advances=%u requests=%u held=%u signed_wraps=%u modes=10 bits_per_mode=33 post_restore=20000\n",edges,advances,requests,holds,wraps);
}
