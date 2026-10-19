#pragma once
// Independent RAM/port/timer/interrupt model used by the MCS6532 wrapper test.
// Derived from the separately qualified direct-RIOT test oracle; no RTL imports.
#include <array>
#include <cstdint>
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
