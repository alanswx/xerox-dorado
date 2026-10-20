#include "Vmsa_ff_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

static Vmsa_ff_fixture d;
static std::array<uint32_t,4> state{};
static constexpr std::array<unsigned,4> widths{9,7,16,18};
static uint32_t random_state=0x15202690;
static uint64_t edges=0,checks=0,writes=0,held=0;
static unsigned loads=0,shifts=0,clears=0,row_captures=0,col_captures=0;
static unsigned rnd(){random_state^=random_state<<13;random_state^=random_state>>17;random_state^=random_state<<5;return random_state;}
static unsigned bit(unsigned v,unsigned b){return(v>>b)&1;}
static unsigned pin(unsigned v,unsigned p){return bit(v,p-1);}
static unsigned mask(unsigned w){return(1u<<w)-1;}
static void check(bool ok,const char*why){++checks;if(!ok)throw std::runtime_error(std::string(why)+" at edge "+std::to_string(edges));}
static unsigned next(unsigned i,unsigned s,unsigned p){
 if(i==0){
  unsigned q=s&255;
  if(!pin(p,9)){q=0;++clears;}
  else if(pin(p,7)&&!bit(s,8)&&!pin(p,6)){
   if(!pin(p,15)){
    constexpr unsigned pins[]={2,3,4,5,10,11,12,14};q=0;
    for(unsigned b=0;b<8;++b)q|=pin(p,pins[b])<<b;
    ++loads;
   }else{q=((q<<1)|pin(p,1))&255;++shifts;}
  }
  return q|(pin(p,7)<<8);
 }
 if(i==1){
  unsigned q=s&63;
  if(!pin(p,1)){q=0;++clears;}
  else if(pin(p,9)&&!bit(s,6)){
   constexpr unsigned pins[]={3,4,6,11,13,14};q=0;
   for(unsigned b=0;b<6;++b)q|=pin(p,pins[b])<<b;
  }
  return q|(pin(p,9)<<6);
 }
 unsigned n=i==2?7:8,m=mask(n),row=s&m,col=(s>>n)&m;
 if(!pin(p,4)&&bit(s,2*n)){row=(p>>16)&m;++row_captures;}
 if(!pin(p,15)&&bit(s,2*n+1)){col=(p>>16)&m;++col_captures;}
 return row|(col<<n)|(pin(p,4)<<(2*n))|(pin(p,15)<<(2*n+1));
}
static unsigned output(unsigned i,unsigned s){
 if(i==0)return bit(s,7);
 if(i==1)return s&63;
 unsigned n=i==2?7:8;
 return((s&mask(n))<<n)|((s>>n)&mask(n));
}
static void observe(bool reference){
 for(unsigned i=0;i<4;++i){
  check(d.physical[i]==state[i],"physical state oracle differs");
  check(d.result[i]==state[i],"read or padding differs");
  check(d.outputs[i]==output(i,state[i]),"output oracle differs");
  if(reference){check(d.original_state[i]==state[i],"original state differs");check(d.original_outputs[i]==d.outputs[i],"original output differs");}
 }
}
static void pins(){for(auto&p:d.pins)p=rnd()&0xffffff;}
static void edge(bool advance,bool reference){
 d.host_clk=d.ref_clk=0;d.advance=advance;d.eval();
 if(d.hold){++held;if(d.write&&d.selected<4){unsigned i=d.selected;state[i]=d.data&mask(widths[i]);++writes;}}
 else if(advance)for(unsigned i=0;i<4;++i)state[i]=next(i,state[i],d.pins[i]);
 d.host_clk=1;d.ref_clk=reference&&advance&&!d.hold;d.eval();++edges;observe(reference);
 d.host_clk=d.ref_clk=0;d.eval();
}
static void access(unsigned i,unsigned data){pins();d.selected=i;d.data=data&0x3ffff;d.write=1;edge(rnd()&1,false);}
static void restore(const std::array<uint32_t,4>&saved){
 d.hold=1;
 for(unsigned i=0;i<4;++i)access(i,saved[i]|(0x3ffff&~mask(widths[i])));
 d.write=0;observe(true);
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.eval();observe(true);
  for(unsigned n=0;n<20000;++n){pins();d.hold=(rnd()&15)==0;d.write=!d.hold;d.selected=rnd()&7;d.data=rnd()&0x3ffff;edge((rnd()&3)!=0,true);}
  d.hold=0;d.write=0;
  // Exhaust every TTL pin combination; upper address data is independently varied.
  for(unsigned p=0;p<65536;++p){for(unsigned i=0;i<4;++i)d.pins[i]=p|((rnd()&255)<<16);edge(true,true);}
  // Save all clock histories HIGH, so restoring data alone creates a false edge.
  for(unsigned i=0;i<4;++i)d.pins[i]=0xffffff;
  edge(true,true);const auto saved=state;
  std::array<unsigned,4> saved_pins{};for(unsigned i=0;i<4;++i)saved_pins[i]=d.pins[i];
  d.hold=1;d.write=0;for(unsigned n=0;n<64;++n){pins();edge(rnd()&1,true);}
  for(unsigned i=0;i<4;++i){
   access(i,0);
   for(unsigned b=0;b<widths[i];++b){access(i,1u<<b);access(i,0);}
   access(i,0x3ffff);
  }
  // Both selected-invalid writes and unheld writes are inert.
  for(unsigned i=4;i<8;++i)access(i,0x3ffff);
  for(unsigned n=0;n<256;++n){access(rnd()&3,rnd());if(n&1){d.hold=0;pins();edge(true,false);d.hold=1;}}
  restore(saved);d.hold=0;d.write=1;d.selected=0;d.data=0;
  for(unsigned i=0;i<4;++i)d.pins[i]=saved_pins[i];
  for(unsigned n=0;n<9;++n)edge(false,true);
  edge(true,true);
  for(unsigned n=0;n<20000;++n){pins();d.hold=(rnd()&31)==0;d.write=!d.hold;edge((rnd()&3)!=0,true);}
  check(loads&&shifts&&clears&&row_captures&&col_captures,"missing ordinary operation coverage");
  std::printf("MSA_FF PASS host=%llu checks=%llu writes=%llu held=%llu families=3 geometries=4 bits=50 loads=%u shifts=%u clears=%u row=%u col=%u\n",edges,checks,writes,held,loads,shifts,clears,row_captures,col_captures);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
