#include "Vdsketh_ff_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
static Vdsketh_ff_fixture d;
static std::array<unsigned,5> state{0,0,0x70,0,0};
static constexpr std::array<unsigned,5> widths{2,12,8,13,13};
static unsigned edges,held_edges,writes;
static uint32_t rng=0x16501036;
static unsigned rnd(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static unsigned b(unsigned v,unsigned i){return(v>>i)&1;}
static void check(bool ok,const char*why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
static unsigned nibble(unsigned p,unsigned l){return(b(p,l)<<3)|(b(p,l+1)<<2)|(b(p,l+2)<<1)|b(p,l+3);}
static unsigned next(unsigned i,unsigned s,unsigned p){
 if(i==0){
  unsigned a=b(s,0),c=b(s,1);
  if(b(p,0)&&b(p,1)!=b(p,2))a=b(p,1);
  if(b(p,3)&&b(p,4)!=b(p,5))c=b(p,4);
  return a|(c<<1);
 }
 if(i==1){
  unsigned q=s&15;
  if(b(p,0)&&!b(s,11)){
   if(!b(s,8)&&!b(s,9))q=(s>>4)&15;
   else if(!b(s,8)&&b(s,9)&&!b(s,10))q=(q+1)&15;
   else if(b(s,8)&&!b(s,9)&&!b(s,10))q=(q+15)&15;
  }
  return q|(nibble(p,1)<<4)|(b(p,5)<<8)|(b(p,6)<<9)|(b(p,7)<<10)|(b(p,0)<<11);
 }
 if(i==2){
  unsigned q=s&15;
  if(b(p,0)&&!b(s,7)){
   if(!b(s,5))q=nibble(p,5);
   else if(!b(s,4)&&!b(s,6))q=(q+(b(p,1)?1:15))&15;
  }
  return q|(b(p,3)<<4)|(b(p,2)<<5)|(b(p,4)<<6)|(b(p,0)<<7);
 }
 unsigned q=s&63;
 if(!b(p,1))q=0;
 else if(b(p,0)&&!b(s,12))q=i==4?(s>>6)&63:(p>>2)&63;
 return q|(((p>>2)&63)<<6)|(b(p,0)<<12);
}
static unsigned output(unsigned i,unsigned s,unsigned p){
 if(i==0)return s&3;
 if(i>=3)return s&63;
 if(i==2)return !( !b(p,4)&&(b(p,1)?(s&15)==15:(s&15)==0));
 unsigned q=s&15;bool preset=!b(p,5)&&!b(p,6),up=!b(p,5)&&b(p,6),down=b(p,5)&&!b(p,6);
 return q|((! (preset||(!b(p,7)&&((up&&q==15)||(down&&q==0)))))<<4);
}
static void observe(bool reference){
 for(unsigned i=0;i<5;++i){
  check(d.physical[i]==state[i],"state oracle differs");
  check(d.result[i]==state[i],"read/padding differs");
  check(d.outputs[i]==output(i,state[i],d.pins),"output oracle differs");
  if(reference){check(d.original_state[i]==state[i],"original state differs");check(d.outputs[i]==d.original_outputs[i],"original output differs");}
 }
}
static void edge(bool advance,bool reference){
 d.host_clk=d.ref_clk=d.advance=0;d.eval();d.advance=advance;d.eval();
 if(d.hold){++held_edges;if(d.write){for(unsigned i=0;i<5;++i){state[i]=(d.individual?d.restore_data[i]:d.data)&((1u<<widths[i])-1);++writes;}}}
 else if(advance)for(unsigned i=0;i<5;++i)state[i]=next(i,state[i],d.pins);
 d.host_clk=1;d.ref_clk=reference&&advance&&!d.hold;d.eval();++edges;observe(reference);
 d.host_clk=d.ref_clk=d.advance=0;d.eval();
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);d.eval();observe(true);
 for(unsigned i=0;i<80000;++i){d.pins=rnd()&4095;d.write=rnd()&1;d.data=rnd()&0x3ffff;edge((rnd()&3)!=0,true);}
 for(unsigned p=0;p<4096;++p){d.pins=p;edge(true,true);}
 auto saved=state;unsigned saved_pins=d.pins;
 d.hold=1;d.write=0;
 for(unsigned i=0;i<100;++i){d.pins=rnd()&4095;edge(true,true);}
 d.write=1;
 for(unsigned bit=0;bit<18;++bit){d.data=1u<<bit;edge(true,false);d.write=0;d.pins=rnd()&4095;edge(false,false);d.write=1;}
 for(unsigned i=0;i<2000;++i){d.pins=rnd()&4095;d.data=rnd()&0x3ffff;d.write=rnd()&1;edge(true,false);}
 d.hold=d.write=0;for(unsigned i=0;i<5000;++i){d.pins=rnd()&4095;edge(true,false);}
 d.hold=d.write=d.individual=1;
 for(unsigned i=0;i<5;++i)d.restore_data[i]=saved[i]|(0x3ffff&~((1u<<widths[i])-1));
 edge(true,true);d.write=0;for(unsigned i=0;i<20;++i){d.pins=rnd()&4095;edge(true,true);}
 d.hold=0;d.pins=saved_pins;for(unsigned i=0;i<17;++i)edge(false,true);edge(true,true);
 for(unsigned i=0;i<20000;++i){d.pins=rnd()&4095;edge((rnd()&3)!=0,true);}
 std::printf("DSKETH_FF PASS host=%u held=%u cell_writes=%u families=4 modes=5 bits=2/12/8/13/13 post_restore=20000\n",edges,held_edges,writes);
}
