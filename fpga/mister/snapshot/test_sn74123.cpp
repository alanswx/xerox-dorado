#include "Vsn74123_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
using Word=__uint128_t;
static Vsn74123_fixture d;
static std::array<Word,6> state{Word(45)<<64,Word(45)<<64,Word(45)<<64,Word(45)<<64,Word(45)<<64,Word(45)<<64};
static constexpr std::array<unsigned,6> ticks{0,1,200,400,800,1600};
static uint32_t rng=0x7412358;
static unsigned edges,holds,requests,writes,negative_steps,expirations,retriggers;
static bool frozen;
static unsigned rnd(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool b,const char*w){if(!b){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,w);std::exit(1);}}
static unsigned bit(unsigned x,unsigned b){return(x>>b)&1;}
static unsigned timer_next(uint32_t t,unsigned hist,unsigned pins,unsigned tw){
 bool a=bit(pins,0),b=bit(pins,1),c=bit(pins,2);
 bool trigger=(bit(hist,0)&&!a&&b)||(!bit(hist,1)&&b&&!a)||(!bit(hist,2)&&c&&!a&&b);
 if(!c)return 0;
 if(trigger){if(int32_t(t)>0)++retriggers;return tw;}
 if(int32_t(t)<0)++negative_steps;
 if(int32_t(t)>0){if(t==1)++expirations;return t-1;}
 return t;
}
static Word next(Word s,unsigned p,unsigned i){
 unsigned t1=timer_next(uint32_t(s),unsigned(s>>64)&7,p&7,ticks[i]);
 unsigned t2=timer_next(uint32_t(s>>32),unsigned(s>>67)&7,(p>>3)&7,ticks[i]);
 return Word(t1)|(Word(t2)<<32)|(Word(p&63)<<64);
}
static Word unpack(const VlWide<3>&v){return Word(v[0])|(Word(v[1])<<32)|(Word(v[2])<<64);}
static void observe(bool reference){
 unsigned out=0;
 for(unsigned i=0;i<6;++i){
  check(unpack(d.physical[i])==state[i],"state oracle differs");
  out|=(unsigned(int32_t(uint32_t(state[i]))>0)|(unsigned(int32_t(uint32_t(state[i]>>32))>0)<<1))<<(2*i);
  if(reference)check(unpack(d.original_state[i])==state[i],"original state differs");
 }
 check(d.outputs==out,"output oracle differs");if(reference)check(d.outputs==d.original_outputs,"original output differs");
}
static void edge(bool advance,bool reference){
 d.host_clk=d.ref_clk=d.advance=0;d.eval();d.advance=advance;d.eval();
 bool hold=d.freeze&&d.held,accept=hold&&frozen&&d.request,valid=d.index<4;
 std::array<unsigned,6> result{};
 if(hold)++holds;
 if(accept){++requests;
  for(unsigned i=0;i<6;++i){
   unsigned data=d.individual?d.restore_data[i]:d.data;
   if(valid){unsigned n=d.index==3?16:18;Word mask=((Word(1)<<n)-1)<<(18*d.index);
    if(d.write){++writes;state[i]=(state[i]&~mask)|((Word(data)<< (18*d.index))&mask);}
    else result[i]=unsigned(state[i]>>(18*d.index))&((1u<<n)-1);
   }
  }
 }
 if(!hold&&advance)for(unsigned i=0;i<6;++i)state[i]=next(state[i],d.pins,i);
 frozen=hold;d.host_clk=1;d.ref_clk=reference&&advance&&!hold;d.eval();++edges;
 check(d.frozen==(hold?63:0)&&d.done==(accept?63:0)&&d.error==(accept&&!valid?63:0),"request handshake differs");
 if(accept)for(unsigned i=0;i<6;++i)check(d.result[i]==result[i],"read/padding differs");
 observe(reference);d.host_clk=d.ref_clk=d.advance=0;d.eval();
}
static void store(Word value){
 d.freeze=d.held=d.request=d.write=1;d.individual=0;
 for(unsigned w=0;w<4;++w){d.index=w;d.data=(unsigned(value>>(18*w))|(w==3?0x30000:0))&0x3ffff;edge(true,false);}
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);d.eval();observe(true);
 for(unsigned i=0;i<80000;++i){d.pins=rnd()&63;d.request=rnd()&1;d.write=rnd()&1;d.index=rnd()&7;d.data=rnd()&0x3ffff;d.freeze=rnd()&1;d.held=0;edge((rnd()&3)!=0,true);}
 // All pin/history combinations, then let both pulses expire without retrigger.
 for(unsigned repeat=0;repeat<8;++repeat)for(unsigned p=0;p<64;++p){d.pins=p;edge(true,true);}
 d.pins=0b110110;edge(true,true);d.pins=0b100100;
 for(unsigned i=0;i<1700;++i)edge(true,true);
 auto saved=state;unsigned saved_pins=d.pins;
 d.freeze=d.held=d.request=d.write=1;d.data=0x3ffff;edge(true,true); // entry ignores access
 for(unsigned b=0;b<70;++b){store(Word(1)<<b);d.write=0;for(unsigned w=0;w<8;++w){d.index=w;edge(true,false);}}
 // Resume signed negative, maximum, zero and one timers: a frozen state may
 // contain every bit pattern even though normal countdowns are much smaller.
 for(uint32_t t:std::array<uint32_t,5>{0x80000000u,0xffffffffu,0x7fffffffu,0u,1u}){
  store(Word(t)|(Word(t)<<32)|(Word(45)<<64));d.freeze=d.held=d.request=0;d.pins=0b101101;edge(true,false);edge(true,false);d.freeze=d.held=1;edge(false,false);
 }
 for(unsigned i=0;i<2000;++i){d.pins=rnd()&63;d.index=rnd()&7;d.request=1;d.write=rnd()&1;d.data=rnd()&0x3ffff;edge(true,false);}
 d.freeze=d.held=d.request=0;for(unsigned i=0;i<5000;++i){d.pins=rnd()&63;edge(true,false);}
 d.freeze=d.held=1;edge(false,false);d.request=d.write=d.individual=1;
 for(unsigned w=0;w<4;++w){d.index=w;for(unsigned i=0;i<6;++i)d.restore_data[i]=(unsigned(saved[i]>>(18*w))|(w==3?0x30000:0))&0x3ffff;edge(true,w==3);}
 d.write=0;for(unsigned w=0;w<8;++w){d.index=w;edge(true,true);}
 d.freeze=d.request=0;edge(false,true);d.freeze=1;edge(false,true);d.freeze=d.held=d.write=0;d.pins=saved_pins;
 for(unsigned i=0;i<17;++i)edge(false,true);edge(true,true);
 for(unsigned i=0;i<20000;++i){d.pins=rnd()&63;edge((rnd()&3)!=0,true);}
 check(negative_steps&&expirations&&retriggers,"timer branch not exercised");
 std::printf("SN74123 PASS host=%u held=%u requests=%u cell_writes=%u modes=6 bits_per_mode=70 negative=%u expiry=%u retrigger=%u post_restore=20000\n",edges,holds,requests,writes,negative_steps,expirations,retriggers);
}
