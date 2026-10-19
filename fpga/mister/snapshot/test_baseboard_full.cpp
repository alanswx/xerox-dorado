#include "Vbaseboard_full_fixture.h"
#include "verilated.h"
#include "ports.h"
#include "riot_oracle.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
static Vbaseboard_full_fixture d;
using Image=std::array<uint32_t,(STATE_BITS+31)/32>;
static std::array<std::array<uint8_t,1024>,4> memory{};
static std::array<unsigned,4> ram_writes{},raw_pipeline{};
static std::array<Model,5> riots{};static std::array<bool,5> phase{};
static std::array<uint8_t,INDEX_COUNT> valid{};
static uint64_t edges=0,holds=0,requests=0,late_changes=0,walked=0,riot_rises=0;
static void check(bool b,const char*w){if(!b)throw std::runtime_error(std::string(w)+" at edge "+std::to_string(edges));}
static unsigned bits(uint64_t bus,unsigned start,unsigned width){return unsigned(bus>>start)&((1u<<width)-1);}
static unsigned bits(uint32_t bus,unsigned start,unsigned width){return bits(uint64_t(bus),start,width);}
template<class W>static unsigned bits(const W&bus,unsigned start,unsigned width){
 uint64_t x=uint64_t(bus[start/32])>>(start%32);
 if(start%32+width>32)x|=uint64_t(bus[start/32+1])<<(32-start%32);
 return unsigned(x)&((1u<<width)-1);
}
static Image image(){Image x{};for(unsigned i=0;i<x.size();++i)x[i]=d.candidate_state[i];return x;}
static void equal(bool compare_outputs=true){
 if(std::memcmp(d.original_state.data(),d.candidate_state.data(),sizeof(Image))){
  for(unsigned i=0;i<Image{}.size();++i)if(d.original_state[i]!=d.candidate_state[i]){std::fprintf(stderr,"first differing bitword=%u original=%08x candidate=%08x\n",i,d.original_state[i],d.candidate_state[i]);break;}
  check(false,"board state differs");
 }
 for(unsigned i=0;i<4;++i){
  unsigned expected=memory[i][d.probe_row&1023];
  check(bits(d.original_probe,i*8,8)==expected&&bits(d.candidate_probe,i*8,8)==expected,"independent RAM oracle differs");
  check(bits(d.original_state,state_words[ram_start[i]+1024].offset,8)==raw_pipeline[i],"independent read pipeline differs");
 }
 for(unsigned i=0;i<5;++i){
  State s=riots[i].pack();
  for(unsigned row=0;row<133;++row){const auto&w=state_words[riot_start[i]+row];if(bits(d.original_state,w.offset,w.width)!=scan(s,row))std::fprintf(stderr,"RIOT%u row%u original=%x expected=%x pins=%llx\n",i,row,bits(d.original_state,w.offset,w.width),scan(s,row),d.riot_pins[i]);check(bits(d.original_state,w.offset,w.width)==scan(s,row),"independent RIOT oracle differs");}
  check(bits(d.original_state,state_words[riot_start[i]+133].offset,1)==unsigned(phase[i]),"independent RIOT phase differs");
 }
 if(compare_outputs)check(!std::memcmp(d.original_outputs.data(),d.candidate_outputs.data(),sizeof(d.original_outputs)),"board output differs");
}
static void physical(const Image&expected){check(!std::memcmp(d.candidate_state.data(),expected.data(),sizeof(Image)),"physical write differs");}
static void pins(){
 static_assert(INPUTS==64,"actual BaseBoard stimulus width changed");
 d.stimulus=edges%257<32?0:edges%257<64?~0ull:(uint64_t(rnd())<<32)|rnd();
 d.probe_row=rnd()&1023;
}
static void pin(unsigned index,bool value){uint64_t mask=1ull<<index;d.stimulus=value?d.stimulus|mask:d.stimulus&~mask;}
static void cycle(bool run,bool reference,bool late=false,bool compare=true){
 d.run=run;d.run_ref=run&&reference;d.host_clk=0;d.eval();
 if(late){d.run=!run;d.run_ref=reference?!run:false;d.eval();++late_changes;}
 if(d.ref_advance){
  for(unsigned i=0;i<4;++i){unsigned a=bits(d.ram_address,i*10,10);raw_pipeline[i]=memory[i][a];if((d.ram_write>>i)&1){memory[i][a]=bits(d.ram_data,i*8,8);++ram_writes[i];}}
  for(unsigned i=0;i<5;++i){uint64_t p=d.riot_pins[i];bool now=(p>>37)&1,rise=now&&!phase[i];riots[i].tick((p&~1ull)|uint64_t(rise));phase[i]=now;if(rise)++riot_rises;}
 }
 if(!d.advance||d.freeze&&d.held)++holds;
 d.host_clk=1;d.eval();++edges;
 if(compare)equal();
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool invalid=false,bool compare=true){
 d.state_index=index;d.request=1;d.write=write;d.state_data=data&0x3ffff;pins();cycle(rnd()&1,false,false,compare);++requests;
 check(d.done,"missing state response");check(bool(d.error)==invalid,"invalid index accepted");return d.result;
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  for(unsigned i=0;i<4;++i)for(unsigned a=0;a<1024;++a)memory[i][a]=(a*73+i*49)&255;
  d.eval();equal();for(const auto&w:state_words){check(!valid[w.word],"duplicate fixture word");valid[w.word]=1;}
  // Original firmware/power-on progression, no reset or RAM/node patches.
  d.stimulus=0;pin(PIN_baseboard_boot_n,true);pin(PIN_BootMC_p_,true);pin(PIN_TTLIOReset_p_,true);
  for(unsigned i=0;i<20000;++i)cycle(true,true);
  for(unsigned i=0;i<80000;++i){pins();d.request=rnd()&1;d.write=rnd()&1;d.state_index=rnd()&(INDEX_COUNT-1);d.state_data=rnd()&0x3ffff;
   d.freeze=rnd()&1;d.held=0;cycle((rnd()&3)!=0,true,i%3==0);check(!d.done,"running request accepted");}
  d.freeze=0;d.held=1;d.request=0;for(unsigned i=0;i<128;++i){pins();cycle(true,true);}
  // Every primitive must stop together despite a still-high advance input.
  d.freeze=d.held=d.request=d.write=1;d.state_data=0x3ffff;cycle(true,false);check(d.frozen&&!d.done,"freeze entry accepted request");
  auto saved=image();auto saved_pins=d.stimulus;unsigned saved_probe=d.probe_row;std::array<unsigned,WORDS> snapshot{};
  for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];snapshot[i]=bits(saved,w.offset,w.width);check(access(w.word,false)==snapshot[i],"state export differs");}
  unsigned poison=(~bits(saved,0,18))&0x3ffff;
  for(unsigned i=0;i<INDEX_COUNT;++i)if(!valid[i]){check(access(i,false,0,true)==0,"invalid read padding differs");access(i,true,poison,true);}
  for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];unsigned mask=(1u<<w.width)-1;
   for(unsigned b=0;b<w.width;++b){auto expected=saved;unsigned pos=w.offset+b;expected[pos/32]^=1u<<(pos%32);unsigned value=snapshot[i]^(1u<<b);
    access(w.word,true,value|(~mask&0x3ffff),false,false);physical(expected);check(access(w.word,false,0,false,false)==value,"bit read/padding differs");access(w.word,true,snapshot[i],false,false);physical(saved);++walked;}
  }
  check(walked==STATE_BITS,"not every physical state bit covered");
  auto shadow=saved;
  for(unsigned pass=0;pass<2;++pass)for(unsigned i=0;i<WORDS;++i){const auto&w=state_words[i];unsigned value=pass?snapshot[i]:~snapshot[i];
   for(unsigned b=0;b<w.width;++b){unsigned p=w.offset+b,mask=1u<<(p%32);shadow[p/32]=((value>>b)&1)?shadow[p/32]|mask:shadow[p/32]&~mask;}
   access(w.word,true,value,false,false);physical(shadow);
  }
  physical(saved);d.request=d.write=0;d.freeze=0;cycle(false,false);d.freeze=1;cycle(false,false);check(d.frozen&&!d.done,"quiet recapture failed");
  for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==snapshot[i],"quiet recapture differs");
  // Destructive candidate-only execution followed by the same complete image.
  d.request=d.write=d.freeze=d.held=0;for(unsigned i=0;i<3000;++i){pins();cycle(true,false,false,false);}
  d.freeze=d.held=1;cycle(true,false,false,false);
  for(unsigned i=0;i<WORDS;++i)access(state_words[i].word,true,snapshot[i],false,false);physical(saved);
  for(unsigned i=0;i<WORDS;++i)check(access(state_words[i].word,false)==snapshot[i],"restored image differs");
  d.request=d.write=d.freeze=d.held=0;d.stimulus=saved_pins;d.probe_row=saved_probe;d.eval();equal();
  for(unsigned i=0;i<17;++i)cycle(false,false);cycle(true,true);
  for(unsigned i=0;i<20000;++i){pins();cycle((rnd()&3)!=0,true,i%3==0);}
  for(unsigned i=0;i<4;++i)std::printf("normal SRAM writer%u=%u\n",i,ram_writes[i]);
  std::printf("FULL BASEBOARD RESTORE PASS host=%llu held=%llu requests=%llu late=%llu bits=%llu indices=%u words=%u riot_rises=%llu post_restore=20000\n",edges,holds,requests,late_changes,walked,INDEX_COUNT,WORDS,riot_rises);return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
