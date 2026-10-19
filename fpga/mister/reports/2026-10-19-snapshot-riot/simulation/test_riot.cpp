#include "Vriot_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

using State=std::array<uint32_t,35>;
static uint32_t rnd(){static uint32_t s=0x65321097;s^=s<<13;s^=s>>17;s^=s<<5;return s;}
template<class T>static State state(const T&v){State n{};for(unsigned i=0;i<35;i++)n[i]=v[i];return n;}
static unsigned get(const State&n,unsigned lo,unsigned width){unsigned v=0;for(unsigned i=0;i<width;i++)v|=((n[(lo+i)/32]>>((lo+i)%32))&1)<<i;return v;}
static void put(State&n,unsigned lo,unsigned width,unsigned value){for(unsigned i=0;i<width;i++){unsigned b=lo+i;uint32_t mask=1u<<(b%32);n[b/32]=(n[b/32]&~mask)|(((value>>i)&1)?mask:0);}}
static unsigned scan(const State&n,unsigned index){if(index<128)return get(n,index*8,8);if(index<133)return get(n,1024+(index-128)*18,index==132?1:18);return 0;}
static void store(State&n,unsigned index,unsigned value){if(index<128)put(n,index*8,8,value);else if(index<133)put(n,1024+(index-128)*18,index==132?1:18,value);}

struct Model{
 std::array<uint8_t,128>ram{};
 uint8_t dout=0,a=0,b=0,da=0,db=0,intr=0,timer=0,incr=0,en=0;
 uint16_t pre=0;bool edge=false,roll=false,oldpa7=false;
 State pack()const{
  State n{};for(unsigned i=0;i<128;i++)put(n,i*8,8,ram[i]);
  put(n,1024,8,dout);put(n,1032,8,a);put(n,1040,8,b);put(n,1048,8,da);put(n,1056,8,db);
  put(n,1064,8,intr);put(n,1072,8,timer);put(n,1080,10,pre);put(n,1090,2,incr);put(n,1092,2,en);
  put(n,1094,1,edge);put(n,1095,1,roll);put(n,1096,1,oldpa7);return n;
 }
 void unpack(const State&n){
  for(unsigned i=0;i<128;i++)ram[i]=get(n,i*8,8);
  dout=get(n,1024,8);a=get(n,1032,8);b=get(n,1040,8);da=get(n,1048,8);db=get(n,1056,8);
  intr=get(n,1064,8);timer=get(n,1072,8);pre=get(n,1080,10);incr=get(n,1090,2);en=get(n,1092,2);
  edge=get(n,1094,1);roll=get(n,1095,1);oldpa7=get(n,1096,1);
 }
 unsigned outputs(uint64_t p)const{
  unsigned pa=uint8_t(a|~da),pb=uint8_t(b|~db);bool irq=!(((intr&128)&&(en&2))||((intr&64)&&(en&1)));
  bool selected=((p>>19)&1)&&!((p>>20)&1);return dout|(pa<<8)|(pb<<16)|(unsigned(irq)<<24)|(unsigned(selected&&((p>>9)&1))<<25);
 }
 void tick(uint64_t p){
  Model old=*this;bool ce=p&1,res=(p>>1)&1,read=(p>>9)&1,io=(p>>18)&1;
  bool selected=((p>>19)&1)&&!((p>>20)&1);unsigned addr=(p>>2)&127,data=(p>>10)&255;
  unsigned pa=(p>>21)&255,pb=(p>>29)&255;
  if(!res){ram.fill(0);dout=255;a=b=da=db=intr=en=0;edge=false;incr=2;timer=255;pre=0;roll=false;return;}
  if(!ce)return;
  if(selected&&read){
   if(!io)dout=old.ram[addr];
   else if(!(addr&4)){
    switch(addr&3){case 0:dout=pa&uint8_t(old.a|~old.da);break;case 1:dout=old.da;break;case 2:dout=pb&uint8_t(old.b|~old.db);break;case 3:dout=old.db;break;}
   }else dout=(addr&1)?old.intr&192:old.timer;
  }
  static const unsigned period[4]={1,8,64,1024};bool tick=old.roll||(old.pre%period[old.incr]==0);
  pre=(old.pre+1)&1023;if(tick)timer=uint8_t(old.timer-1);
  if(selected){
   if(!io){if(!read)ram[addr]=data;}
   else if(!(addr&4)){
    if(!read)switch(addr&3){case 0:a=data;break;case 1:da=data;break;case 2:b=data;break;case 3:db=data;break;}
   }else if(!read){
    if(addr&16){pre=0;roll=false;intr&=127;incr=addr&3;timer=data;en=(en&1)|((addr&8)?2:0);}
    else{en=(en&2)|((addr&2)?1:0);edge=addr&1;}
   }else if(!(addr&1)){en=(en&1)|((addr&8)?2:0);roll=false;intr&=127;}
   else intr&=191;
  }
  // Underflow and PA7 events win over same-edge flag clearing, matching
  // the vendor's observed sequential priority, not a host-side repair.
  if(tick&&!old.timer){intr|=128;roll=true;}
  bool pin=(old.da&128)?bool(old.a&128):bool(pa&128);
  oldpa7=pin;if((old.edge&&!old.oldpa7&&pin)||(!old.edge&&old.oldpa7&&!pin))intr|=64;
 }
};

struct Test{
 Vriot_fixture v;Model model;bool was_frozen=false;uint64_t clocks=0,holds=0,requests=0,writes=0,underflows=0,edges=0;
 Test(){v.host_clk=0;v.ref_clk=0;v.advance=0;v.freeze=0;v.held=0;v.request=0;v.write=0;v.index=0;v.data=0;v.pins=0;v.eval();}
 [[noreturn]]void fail(const char*s){throw std::runtime_error(std::string(s)+" at host edge "+std::to_string(clocks));}
 void step(bool ref=true,bool compare=true){
  v.host_clk=0;v.ref_clk=0;v.eval();bool hold=v.freeze&&v.held,accept=hold&&was_frozen&&v.request,valid=v.index<133;
  State expected=model.pack();unsigned read=scan(expected,v.index);
  if(!hold&&v.advance&&(v.pins&3)==3){
   static const unsigned period[4]={1,8,64,1024};
   if(!model.timer&&(model.roll||model.pre%period[model.incr]==0))++underflows;
   bool pin=(model.da&128)?bool(model.a&128):bool((v.pins>>28)&1);
   if((model.edge&&!model.oldpa7&&pin)||(!model.edge&&model.oldpa7&&!pin))++edges;
  }
  if(accept&&valid&&v.write){store(expected,v.index,v.data);model.unpack(expected);}
  if(!hold&&v.advance)model.tick(v.pins);
  if(!hold&&v.advance&&ref)v.ref_clk=1;v.host_clk=1;v.eval();++clocks;if(hold)++holds;if(accept)++requests;if(accept&&valid&&v.write)++writes;
  if(state(v.physical)!=model.pack())fail("RIOT state oracle differs");
  if(v.outputs!=model.outputs(v.pins))fail("RIOT output oracle differs");
  if(bool(v.frozen)!=hold||bool(v.done)!=accept||bool(v.error)!=(accept&&!valid))fail("request handshake differs");
  if(accept&&(!valid||!v.write)&&v.result!=(valid?read:0))fail("read response differs");
  if(compare&&(state(v.physical)!=state(v.original_state)||v.outputs!=v.original_outputs))fail("original continuation differs");
  was_frozen=hold;v.host_clk=0;v.ref_clk=0;v.eval();
 }
 void access(unsigned i,bool write,unsigned data=0){v.pins=(uint64_t(rnd())|(uint64_t(rnd()&31)<<32));v.advance=rnd()&1;v.index=i;v.write=write;v.data=data;v.request=1;step(false,false);v.request=0;}
 void bus(unsigned addr,unsigned data,bool read,bool io,bool ce=true,unsigned pa=0,unsigned pb=0){
  v.pins=uint64_t(ce)|2|(uint64_t(addr&127)<<2)|(uint64_t(read)<<9)|(uint64_t(data&255)<<10)|(uint64_t(io)<<18)|(1ull<<19)|(uint64_t(pa&255)<<21)|(uint64_t(pb&255)<<29);
  v.advance=1;step();
 }
 void random(unsigned n,bool ref=true,bool compare=true){for(unsigned i=0;i<n;i++){v.pins=(uint64_t(rnd())|(uint64_t(rnd()&31)<<32));v.advance=(rnd()&7)!=0;step(ref,compare);}}
};

int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  Test t;t.v.advance=1;t.step(); // synchronous reset despite ce=0
  for(unsigned pattern=0;pattern<4;pattern++){
   for(unsigned i=0;i<128;i++)t.bus(i,pattern==0?i:pattern==1?255-i:pattern==2?0x55:0xaa,false,false);
   for(unsigned i=0;i<128;i++)t.bus(i,0,true,false);
  }
  for(unsigned cs=0;cs<4;cs++)for(unsigned ce=0;ce<2;ce++)for(unsigned read=0;read<2;read++)for(unsigned io=0;io<2;io++)for(unsigned a=0;a<128;a++){
   t.bus(a,rnd(),read,io,ce,rnd(),rnd());t.v.pins=(t.v.pins&~(3ull<<19))|(uint64_t(cs)<<19);t.step();
  }
  for(unsigned gran=0;gran<4;gran++){
   t.bus(0x1c|gran,2,false,true);
   for(unsigned j=0;j<2110;j++)t.bus(0,0,true,false,true,j&255,255-(j&255));
  }
  t.random(10000);
  for(unsigned i=0;i<1500;i++){
   t.v.pins=(uint64_t(rnd())|(uint64_t(rnd()&31)<<32));t.v.advance=rnd()&1;
   t.v.freeze=rnd()&1;t.v.held=rnd()&1;t.v.request=rnd()&1;t.v.write=0;t.v.index=rnd()&255;t.step();
  }
  t.v.freeze=0;t.v.held=0;t.v.request=0;
  for(unsigned i=0;i<128;i++)t.bus(i,rnd(),false,false);
  t.bus(0,0xda,false,true);t.bus(1,0xa5,false,true);t.bus(2,0xc3,false,true);t.bus(3,0x3c,false,true);
  t.bus(0x1f,0xe1,false,true);for(unsigned i=0;i<333;i++)t.bus(0,0,true,false,true,128,85);
  t.v.advance=0;t.v.freeze=0;t.v.held=0;t.v.request=0;t.step();State saved=t.model.pack();
  t.v.freeze=1;t.v.held=1;t.v.request=1;t.v.write=1;t.v.index=0;t.v.data=255;t.step(false);t.v.request=0;
  for(unsigned i=0;i<133;i++)t.access(i,true,0);
  for(unsigned bit=0;bit<1097;bit++){
   unsigned index=bit<1024?bit/8:128+(bit-1024)/18;unsigned pos=bit<1024?bit%8:(bit-1024)%18;
   t.access(index,true,1u<<pos);t.access(index,false);t.access(index,true,0);
  }
  for(unsigned i=133;i<256;i++){t.access(i,true,0x3ffff);t.access(i,false);}
  for(unsigned i=0;i<133;i++)t.access(i,true,scan(saved,i));
  t.v.freeze=0;t.v.held=0;t.v.advance=1;t.v.request=0;t.random(1000,false,false);
  t.v.freeze=1;t.v.held=1;t.step(false,false);
  for(unsigned i=0;i<133;i++)t.access(i,true,scan(saved,i));
  for(unsigned i=0;i<133;i++)t.access(i,false);
  t.v.freeze=0;t.v.held=0;t.v.advance=0;t.v.request=1;t.v.write=1;t.v.index=0;t.v.data=255;t.step();
  t.v.request=0;t.v.advance=1;t.step();t.random(20000);
  if(!t.underflows||!t.edges)t.fail("missing timer/PA7 coverage");
  std::printf("RIOT PASS: %llu host/%llu held/%llu accepted/%llu writes/%llu underflows/%llu PA7events; all1097bits/all256indices/all128RAMwords/fourprescalers/destructive restore/firstedge/20000more\n",t.clocks,t.holds,t.requests,t.writes,t.underflows,t.edges);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
