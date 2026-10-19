#include "Vf100181_fixture.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vf100181_fixture d;
static unsigned state,edges,held,transitions,restores;
static uint32_t rng=0xf100181;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char* why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
static unsigned bit(unsigned n,unsigned b){return(n>>b)&1;}
struct ALU {unsigned f,carry;};
static ALU alu(unsigned pins){
    unsigned a=pins&15,b=(pins>>4)&15,s=(pins>>8)&15,cin=!bit(pins,12);
    unsigned generate=0,propagate=0,f=0;
    // Truth table per bit; ripple independently computes the group carry.
    unsigned carry=cin,carry_zero=0,all_p=1;
    for(unsigned i=0;i<4;++i){
        unsigned x=bit(a,i),y=bit(b,i),g=0,p=0,v=0;
        if(s<8){
            switch(s&3){case 0:break;case 1:y=!y;break;case 2:x=!x;break;case 3:x=0;y=!y;break;}
            g=x&y;p=x|y;v=x^y^carry;
        }else{
            switch(s){
                case 8:v=x&y;g=x&y;p=x|y;break;
                case 9:v=x^y;g=x&y;p=x|y;break;
                case 10:v=x|y;g=x&y;p=x|y;break;
                case 11:v=x;g=x;p=x;break;
                case 12:v=!y;p=!y;break;
                case 13:v=y;p=y;break;
                case 14:v=(!x)&y;g=v;p=(!x)|y;break;
                case 15:p=1;break;
            }
        }
        f|=v<<i;carry=g|(p&carry);carry_zero=g|(p&carry_zero);all_p&=p;
        generate|=g<<i;propagate|=p<<i;
    }
    (void)generate;(void)propagate;
    return {f,(!carry)|((!all_p)<<1)|((!carry_zero)<<2)};
}
static void observe(bool ref){
    check(d.result==state,"state oracle differs");
    check(d.outputs==(state|(alu(d.pins).carry<<4)),"pin oracle differs");
    if(ref){check(d.original_state==state,"original state differs");check(d.outputs==d.original_outputs,"original outputs differ");}
}
static void edge(bool advance,bool reference,bool compare=true){
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();d.advance=advance;d.eval();
    if(d.hold){++held;if(d.write){state=d.data&15;++restores;}}
    else if(advance){if(!bit(d.pins,13))state=alu(d.pins).f;++transitions;}
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;observe(compare);
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);d.eval();observe(true);
    for(unsigned i=0;i<50000;++i){d.pins=random_word()&16383;d.write=random_word()&1;d.data=random_word()&0x3ffff;bool run=random_word()%4;edge(run,run);}
    // Every A/B/function/carry/latch-enable combination, not only MemC's binary use.
    for(unsigned p=0;p<16384;++p){d.pins=p;edge(true,true);}
    const unsigned saved=state,saved_pins=d.pins;
    d.hold=1;d.write=0;
    for(unsigned i=0;i<257;++i){d.pins=random_word()&16383;edge(true,false,false);}
    d.write=1;
    for(unsigned s=0;s<16;++s){d.data=s|0x3fff0;d.pins=random_word()&16383;edge(true,false,false);}
    d.hold=0;d.write=0;
    for(unsigned i=0;i<10000;++i){d.pins=random_word()&16383;edge(true,false,false);}
    d.hold=1;d.write=1;d.data=saved;d.pins=saved_pins;edge(true,false,true);
    d.hold=0;d.write=0;d.eval();observe(true);
    for(unsigned i=0;i<19;++i)edge(false,false);
    edge(true,true);
    for(unsigned i=0;i<20000;++i){d.pins=random_word()&16383;d.write=random_word()&1;d.data=random_word()&0x3ffff;bool run=random_word()%4;edge(run,run);}
    std::printf("F100181 PASS host=%u held=%u transitions=%u restores=%u pin_space=16384 post_restore=20000\n",edges,held,transitions,restores);
}
