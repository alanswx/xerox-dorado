#include "Vmcs6532_fixture.h"
#include "verilated.h"
#include "riot_oracle.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>

static unsigned wrapper_scan(const State&s,unsigned index){return index==133?get(s,1097,1):scan(s,index);}
static void wrapper_store(State&s,unsigned index,unsigned value){if(index==133)put(s,1097,1,value);else store(s,index,value);}

struct Test{
 Vmcs6532_fixture v;Model model;bool phase=false,was_frozen=false;
 uint64_t clocks=0,holds=0,requests=0,writes=0,rises=0,late=0;
 Test(){v.host_clk=0;v.ref_clk=0;v.advance=0;v.freeze=0;v.held=0;v.request=0;v.write=0;v.index=0;v.data=0;v.pins=0;v.eval();}
 State physical()const{State s=model.pack();put(s,1097,1,phase);return s;}
 [[noreturn]]void fail(const char*s){throw std::runtime_error(std::string(s)+" at host edge "+std::to_string(clocks));}
 void step(bool ref=true,bool compare=true){
  v.host_clk=0;v.ref_clk=0;v.eval();
  bool hold=v.freeze&&v.held,accept=hold&&was_frozen&&v.request,valid=v.index<134;
  State expected=physical();unsigned read=wrapper_scan(expected,v.index);
  if(accept&&valid&&v.write){wrapper_store(expected,v.index,v.data);model.unpack(expected);phase=get(expected,1097,1);}
  if(!hold&&v.advance){
   bool p=(v.pins>>37)&1;bool rise=p&&!phase;
   model.tick((v.pins&~1ull)|uint64_t(rise));phase=p;if(rise)++rises;
  }
  if(!hold&&v.advance&&ref)v.ref_clk=1;
  v.host_clk=1;v.eval();++clocks;if(hold)++holds;if(accept)++requests;if(accept&&valid&&v.write)++writes;
  if(bool(v.frozen)!=hold||bool(v.done)!=accept||bool(v.error)!=(accept&&!valid))fail("wrapper request handshake differs");
  if(state(v.physical)!=physical())fail("wrapper state oracle differs");
  unsigned outputs=model.outputs(v.pins);if(!(outputs&(1u<<25)))outputs&=~255u;
  if(v.outputs!=outputs)fail("wrapper output oracle differs");
  if(accept&&(!valid||!v.write)&&v.result!=(valid?read:0))fail("wrapper read response differs");
  if(compare&&(state(v.physical)!=state(v.original_state)||v.outputs!=v.original_outputs))fail("original wrapper continuation differs");
  was_frozen=hold;v.host_clk=0;v.ref_clk=0;v.eval();
 }
 uint64_t pins(){return uint64_t(rnd())|(uint64_t(rnd()&63)<<32);}
 void access(unsigned index,bool write,unsigned data=0){v.pins=pins();v.advance=rnd()&1;v.index=index;v.write=write;v.data=data;v.request=1;++late;step(false,false);v.request=0;}
 void bus(unsigned addr,unsigned data,bool read,bool io,unsigned cs=1,unsigned pa=0,unsigned pb=0){
  v.pins=2|(uint64_t(addr&127)<<2)|(uint64_t(read)<<9)|(uint64_t(data&255)<<10)|(uint64_t(io)<<18)|(uint64_t(cs&3)<<19)|(uint64_t(pa&255)<<21)|(uint64_t(pb&255)<<29);
  v.advance=1;step();v.pins|=1ull<<37;step();step();step();v.pins&=~(1ull<<37);step();
 }
 void random(unsigned n,bool ref=true,bool compare=true){for(unsigned i=0;i<n;i++){v.pins=pins();v.advance=(rnd()&7)!=0;step(ref,compare);}}
};

int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  Test t;t.v.advance=1;t.step();
  for(unsigned pattern=0;pattern<4;pattern++){
   for(unsigned i=0;i<128;i++)t.bus(i,pattern==0?i:pattern==1?255-i:pattern==2?0x55:0xaa,false,false);
   for(unsigned i=0;i<128;i++)t.bus(i,0,true,false);
  }
  for(unsigned cs=0;cs<4;cs++)for(unsigned read=0;read<2;read++)for(unsigned io=0;io<2;io++)for(unsigned a=0;a<128;a++)t.bus(a,rnd(),read,io,cs,rnd(),rnd());
  for(unsigned gran=0;gran<4;gran++){t.bus(0x1c|gran,2,false,true);for(unsigned i=0;i<2110;i++)t.bus(0,0,true,false,1,i&255,255-(i&255));}
  t.random(5000);
  for(unsigned i=0;i<1500;i++){
   t.v.pins=t.pins();t.v.advance=rnd()&1;t.v.freeze=rnd()&1;t.v.held=rnd()&1;t.v.request=rnd()&1;t.v.write=0;t.v.index=rnd()&255;t.step();
  }
  t.v.freeze=0;t.v.held=0;t.v.request=0;
  for(unsigned i=0;i<128;i++)t.bus(i,rnd(),false,false);
  t.bus(0,0xda,false,true);t.bus(1,0xa5,false,true);t.bus(2,0xc3,false,true);t.bus(3,0x3c,false,true);
  t.bus(0x1f,0xe1,false,true);for(unsigned i=0;i<333;i++)t.bus(0,0,true,false,1,128,85);
  t.v.pins|=1ull<<37;t.step(); // Save a HIGH phase, not just reset/low.
  State saved=t.physical();t.v.advance=0;t.v.freeze=1;t.v.held=1;t.v.request=1;t.v.write=1;t.v.index=0;t.v.data=255;t.step(false);t.v.request=0;
  for(unsigned i=0;i<134;i++)t.access(i,true,0);
  for(unsigned bit=0;bit<1098;bit++){
   unsigned index=bit<1024?bit/8:bit==1097?133:128+(bit-1024)/18;
   unsigned pos=bit<1024?bit%8:bit==1097?0:(bit-1024)%18;
   t.access(index,true,1u<<pos);t.access(index,false);t.access(index,true,0);
  }
  // Explicit back-to-back child/phase/invalid response routing, with the
  // previous child reply intentionally distinct from the phase reply.
  t.access(0,true,0x5a);t.access(0,false);t.access(133,true,1);t.access(133,false);
  t.access(0,false);t.access(255,false);t.access(133,false);t.access(132,false);t.access(133,false);
  for(unsigned i=134;i<256;i++){t.access(i,true,0x3ffff);t.access(i,false);}
  for(unsigned i=0;i<134;i++)t.access(i,true,wrapper_scan(saved,i));
  t.v.freeze=0;t.v.held=0;t.v.advance=1;t.v.request=0;t.random(1000,false,false);
  t.v.freeze=1;t.v.held=1;t.step(false,false);
  for(unsigned i=0;i<134;i++)t.access(i,true,wrapper_scan(saved,i));
  for(unsigned i=0;i<134;i++)t.access(i,false);
  // High phase after restore must not create a synthetic RIOT rising edge.
  t.v.pins|=1ull<<37;t.v.freeze=0;t.v.held=0;t.v.advance=0;t.v.request=1;t.v.write=1;t.v.index=133;t.v.data=0;t.step();
  t.v.request=0;t.v.advance=1;t.step();t.random(20000);
  if(t.rises<10000)t.fail("missing phase-edge coverage");
  std::printf("MCS6532 PASS: %llu host/%llu held/%llu requests/%llu writes/%llu actualphase rises/%llu late changes; all1098bits/all256indices/child-phase replies/highphase restore/firstedge/20000more\n",t.clocks,t.holds,t.requests,t.writes,t.rises,t.late);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
