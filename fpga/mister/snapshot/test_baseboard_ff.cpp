#include "Vbaseboard_ff_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
static Vbaseboard_ff_fixture d;
static std::array<uint32_t,9> state{};
static constexpr std::array<unsigned,9> widths{8,5,3,4,2,3,25,18,2};
static uint32_t rng=0x27908770;
static uint64_t edges=0,holds=0,writes=0,por_wraps=0,por_stops=0,counter_wraps=0;
static unsigned rnd(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static unsigned bit(unsigned v,unsigned b){return(v>>b)&1;}
static unsigned pin(unsigned v,unsigned p){return bit(v,p-1);}
static unsigned mask(unsigned w){return(1u<<w)-1;}
static void check(bool ok,const char*why){if(!ok)throw std::runtime_error(std::string(why)+" at edge "+std::to_string(edges));}
static unsigned next(unsigned i,unsigned s,unsigned p){
 if(i==0){if(!pin(p,15))return 0;if(!pin(p,14)){unsigned b=p&7;return(s&~(1u<<b))|(pin(p,13)<<b);}return s;}
 if(i==1){unsigned q=s&15;if(pin(p,2)&&!bit(s,4)){if(!pin(p,1))q=0;else if(!pin(p,9))q=(p>>2)&15;else if(pin(p,7)&&pin(p,10))q=(q+1)&15;}return q|(pin(p,2)<<4);}
 if(i==2){unsigned q=s&3;if(!pin(p,1))q=0;else if(pin(p,9)&&!bit(s,2))q=(p>>3)&3;return q|(pin(p,9)<<2);}
 if(i==3){unsigned a=s&1,b=bit(s,1);if(!pin(p,4))a=1;else if(!pin(p,1))a=0;else if(pin(p,3)&&!bit(s,2))a=pin(p,2);
  if(!pin(p,10))b=1;else if(!pin(p,13))b=0;else if(pin(p,11)&&!bit(s,3))b=pin(p,12);return a|(b<<1)|(pin(p,3)<<2)|(pin(p,11)<<3);}
 if(i==4){unsigned q=s&1;if(pin(p,7)&&!bit(s,1))q=pin(p,11);return q|(pin(p,7)<<1);}
 if(i==5)return(s+1)&7;
 if(i==6){unsigned q=s&0xffffff;if(pin(p,2))q=0;else if(!pin(p,6)&&bit(s,24)){if(q==0xffffff)++counter_wraps;q=(q+1)&0xffffff;}return q|(pin(p,6)<<24);}
 if(i==7){unsigned q=(s>>2)&65535;if(q!=4096){if(q==65535)++por_wraps;q=(q+1)&65535;}else ++por_stops;return((s+1)&3)|(q<<2);}
 return(s+1)&3;
}
static unsigned output(unsigned i,unsigned s,unsigned p){
 if(i==0)return bit(s,1)|(bit(s,3)<<1);
 if(i==1)return pin(p,10)&&((s&15)==15);
 if(i==2)return s&3;
 if(i==3)return bit(s,0)|((1^bit(s,0))<<1)|((1^bit(s,1))<<2);
 if(i==4)return(s&1)|((1^(s&1))<<1);
 if(i==5)return bit(s,2)<<2;
 if(i==6)return bit(s,20);
 if(i==7)return(1^bit(s,1))|(bit(s,1)<<3)|((((s>>2)&65535)==4096)<<7);
 return(!(s>=1&&s<3))|((s>=2)<<1);
}
static void observe(bool reference){
 for(unsigned i=0;i<9;++i){
  if(d.physical[i]!=state[i])std::fprintf(stderr,"cell=%u expected=%08x actual=%08x hold=%u advance=%u selected=%u row=%u data=%x\n",i,state[i],d.physical[i],d.hold,d.advance,d.selected,d.index,d.data);check(d.physical[i]==state[i],"state oracle differs");
  unsigned row=d.index,low=row*18;unsigned expected=low<widths[i]?(state[i]>>low)&0x3ffff:0;
  check(d.result[i]==expected,"read/padding differs");check(d.outputs[i]==output(i,state[i],d.pins[i]),"output oracle differs");
  if(reference){check(d.original_state[i]==state[i],"original state differs");check(d.original_outputs[i]==d.outputs[i],"original output differs");}
 }
}
static void edge(bool advance,bool reference){
 d.host_clk=d.ref_clk=0;d.advance=advance;d.eval();
 if(d.hold){++holds;unsigned i=d.selected,row=d.index;if(d.write&&i<9&&row*18<widths[i]){unsigned n=widths[i]-row*18;if(n>18)n=18;unsigned m=mask(n)<<(row*18);state[i]=(state[i]&~m)|((d.data&mask(n))<<(row*18));++writes;}}
 else if(advance)for(unsigned i=0;i<9;++i)state[i]=next(i,state[i],d.pins[i]);
 d.host_clk=1;d.ref_clk=reference&&advance&&!d.hold;d.eval();++edges;observe(reference);
 d.host_clk=d.ref_clk=0;d.eval();
}
static void pins(){for(auto&p:d.pins)p=rnd()&65535;}
static void access(unsigned cell,unsigned row,bool write,unsigned value){pins();d.selected=cell;d.index=row;d.write=write;d.data=value&0x3ffff;edge(rnd()&1,false);}
static void restore(const std::array<unsigned,9>&saved){d.hold=1;for(unsigned i=0;i<9;++i)for(unsigned row=0;row<(widths[i]+17)/18;++row)access(i,row,true,(saved[i]>>(row*18))|(0x3ffff&~mask(widths[i]-row*18<18?widths[i]-row*18:18)));d.write=0;observe(true);}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.eval();observe(true);
  for(unsigned n=0;n<80000;++n){pins();d.write=rnd()&1;d.selected=rnd()&15;d.index=rnd()&3;d.data=rnd()&0x3ffff;edge((rnd()&3)!=0,true);}
  for(unsigned p=0;p<65536;++p){for(auto&v:d.pins)v=p;d.index=p&3;edge(true,true);}
  // Save each history while its actual clock pin is HIGH and reset inactive.
  pins();d.pins[1]|=(1u<<1)|1u|(1u<<8);d.pins[2]|=(1u<<8)|1u;
  d.pins[3]|=(1u<<2)|(1u<<10)|1u|(1u<<3)|(1u<<9)|(1u<<12);d.pins[4]|=1u<<6;
  d.pins[6]=(d.pins[6]|(1u<<5))&~(1u<<1);edge(true,true);
  const auto saved=state;std::array<unsigned,9> saved_pins{};for(unsigned i=0;i<9;++i)saved_pins[i]=d.pins[i];
  d.hold=1;d.write=0;for(unsigned n=0;n<100;++n){pins();edge(rnd()&1,true);}
  for(unsigned i=0;i<9;++i){for(unsigned row=0;row<2;++row)access(i,row,true,0);for(unsigned b=0;b<widths[i];++b){access(i,b/18,true,1u<<(b%18));access(i,b/18,false,0);access(i,b/18,true,0);}for(unsigned row=0;row<4;++row){access(i,row,true,0x3ffff);access(i,row,false,0);}for(unsigned row=(widths[i]+17)/18;row<4;++row){access(i,row,true,(~state[i])&0x3ffff);access(i,row,false,0);}}
  restore(saved);d.hold=0;d.write=1;d.index=0;d.selected=0;d.data=0;
  for(unsigned i=0;i<9;++i)d.pins[i]=saved_pins[i];for(unsigned n=0;n<17;++n)edge(false,true);edge(true,true);
  for(unsigned n=0;n<2000;++n){pins();edge(true,true);}
  const auto second=state;d.hold=1;d.write=0;edge(true,true);d.hold=0;for(unsigned n=0;n<2000;++n){pins();edge(true,false);}
  // Arbitrary saved counter values exercise high bits, wrap and POR stop.
  d.hold=1;access(6,0,true,0x3ffff);access(6,1,true,0x7f);access(7,0,true,0x3fffc);d.hold=0;d.write=0;d.pins[6]&=~((1u<<5)|(1u<<1));edge(true,false);
  d.hold=1;access(7,0,true,4095u<<2);d.hold=0;d.write=0;edge(true,false);edge(true,false);
  restore(second);d.hold=0;d.write=0;for(unsigned n=0;n<13;++n)edge(false,true);edge(true,true);
  for(unsigned n=0;n<20000;++n){pins();d.index=rnd()&3;edge((rnd()&3)!=0,true);}
  check(por_wraps&&por_stops&&counter_wraps,"missing counter boundary coverage");
  std::printf("BASEBOARD_FF PASS: %llu host/%llu held/%llu writes;9families/70bits/allindices/pins/high-history/partialwords/restore/firstedge/20000more;por_wrap=%llu por_stop=%llu count_wrap=%llu\n",edges,holds,writes,por_wraps,por_stops,counter_wraps);return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
