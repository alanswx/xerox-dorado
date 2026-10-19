#include "Vcrc9401_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
static Vcrc9401_fixture d;
static std::array<unsigned,2> state{0x50000,0x50000};
static std::array<std::array<unsigned,8>,2> shifts{};
static uint32_t rng=0xf9401058;
static unsigned edges,holds,requests,restores;
static bool frozen;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool x,const char*w){if(!x){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,w);std::exit(1);}}
static unsigned bit(unsigned x,unsigned b){return(x>>b)&1;}
static unsigned next(unsigned s,unsigned p,unsigned mode){
    const unsigned masks[]={0x8004,0x4002,0xa096,0x8004,0x8004,0x8004,0x1020,0x0810};
    unsigned r=s&65535,select=(p>>3)&7;
    bool data=bit(mode?s:p,mode?17:7),cwe=bit(mode?s:p,mode?18:6);
    if(bit(p,2))r=0;
    else if(!bit(p,1))r=65535;
    else if(bit(s,16)&&!bit(p,0)){
        bool feedback=cwe&&(data^bit(r,15));r=((r<<1)&65535)^(feedback?(masks[select]|1):0);++shifts[mode][select];
    }
    return r|(bit(p,0)<<16)|(bit(p,7)<<17)|(bit(p,6)<<18);
}
static void observe(bool reference){
    unsigned output=0;
    for(unsigned i=0;i<2;++i){
        check(d.physical[i]==state[i],"state oracle differs");
        output|=(bit(state[i],15)|((state[i]&65535?1:0)<<1))<<(2*i);
        if(reference)check(d.original_state[i]==state[i],"original state differs");
    }
    check(d.outputs==output,"output oracle differs");
    if(reference)check(d.outputs==d.original_outputs,"original outputs differ");
}
static void edge(bool advance,bool reference){
    d.host_clk=d.ref_clk=d.advance=0;d.eval();d.advance=advance;d.eval();
    bool hold=d.freeze&&d.held,accept=hold&&frozen&&d.request,valid=d.index<2;
    std::array<unsigned,2> result{};
    if(hold)++holds;
    if(accept){++requests;
        for(unsigned i=0;i<2;++i){
            unsigned data=d.individual?d.restore_data[i]:d.data;
            if(valid){
                if(d.write){++restores;state[i]=d.index?((state[i]&0x3ffff)|((data&1)<<18)):((state[i]&0x40000)|(data&0x3ffff));}
                else result[i]=d.index?state[i]>>18:state[i]&0x3ffff;
            }
        }
    }
    if(!hold&&advance)for(unsigned i=0;i<2;++i)state[i]=next(state[i],d.pins,i);
    frozen=hold;d.host_clk=1;d.ref_clk=reference&&advance&&!hold;d.eval();++edges;
    check(d.frozen==(hold?3:0)&&d.done==(accept?3:0)&&d.error==(accept&&!valid?3:0),"request handshake differs");
    if(accept)for(unsigned i=0;i<2;++i)check(d.result[i]==result[i],"read/padding differs");
    observe(reference);d.host_clk=d.ref_clk=d.advance=0;d.eval();
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);d.eval();observe(true);
    for(unsigned i=0;i<80000;++i){
        d.pins=random_word()&255;d.request=random_word()&1;d.write=random_word()&1;d.index=random_word()&3;d.data=random_word()&0x3ffff;
        d.freeze=random_word()&1;d.held=0;bool run=(random_word()&3)!=0;edge(run,true);
    }
    for(unsigned repeat=0;repeat<8;++repeat)for(unsigned p=0;p<256;++p){d.pins=p;edge(true,true);}
    auto saved=state;unsigned saved_pins=d.pins;
    d.freeze=d.held=d.request=d.write=1;d.data=0x3ffff;edge(true,true); // entry ignored
    for(unsigned b=0;b<19;++b){
        unsigned value=1u<<b;
        for(unsigned w=0;w<2;++w){d.index=w;d.data=w?(value>>18)|0x3fffe:value&0x3ffff;edge(true,false);}
        d.write=0;for(unsigned w=0;w<4;++w){d.index=w;edge(true,false);}d.write=1;
    }
    for(unsigned i=0;i<1000;++i){d.pins=random_word()&255;d.index=random_word()&3;d.write=random_word()&1;d.data=random_word()&0x3ffff;edge(true,false);}
    d.freeze=d.held=d.request=0;for(unsigned i=0;i<5000;++i){d.pins=random_word()&255;edge(true,false);}
    d.freeze=d.held=1;edge(false,false);d.individual=d.write=d.request=1;
    for(unsigned w=0;w<2;++w){d.index=w;for(unsigned i=0;i<2;++i)d.restore_data[i]=w?(saved[i]>>18)|0x3fffe:saved[i]&0x3ffff;edge(true,w==1);}
    d.write=0;for(unsigned w=0;w<4;++w){d.index=w;edge(true,true);}
    d.freeze=d.request=0;edge(false,true);d.freeze=1;edge(false,true);d.freeze=d.held=d.write=0;d.pins=saved_pins;d.eval();observe(true);
    for(unsigned i=0;i<17;++i)edge(false,true);edge(true,true);
    for(unsigned i=0;i<20000;++i){d.pins=random_word()&255;bool run=(random_word()&3)!=0;edge(run,true);}
    for(const auto&mode:shifts)for(unsigned count:mode)check(count>0,"polynomial not exercised");
    std::printf("CRC9401 PASS host=%u held=%u requests=%u restored=%u modes=2 polynomials=8 bits_per_mode=19 post_restore=20000\n",edges,holds,requests,restores);
}
