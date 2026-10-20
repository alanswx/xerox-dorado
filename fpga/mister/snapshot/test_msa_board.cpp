#include "Vmsa_board_fixture.h"
#include "verilated.h"
#include "parameters.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

static Vmsa_board_fixture d;
using State=std::array<uint32_t,STATE_WORDS>;
struct Backend{bool pending=false,write=false;unsigned address=0,data=0,delay=0;std::vector<uint16_t> memory=std::vector<uint16_t>(65536,0);};
static std::array<Backend,16> backend;
static uint32_t rng=0x2690866a;
static unsigned long long edges=0,checks=0,guest_edges=0,raw_requests=0,raw_writes=0,state_requests=0,scan_edges=0;
static unsigned rnd(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static unsigned bit(unsigned value,unsigned b){return(value>>b)&1;}
static void check(bool ok,const char*why){++checks;if(!ok)throw std::runtime_error(std::string(why)+" at edge "+std::to_string(edges));}
static State physical(){State s{};for(unsigned i=0;i<STATE_WORDS;++i)s[i]=d.dut_state[i];return s;}
static State reference(){State s{};for(unsigned i=0;i<STATE_WORDS;++i)s[i]=d.reference_state[i];return s;}
static void same(const State&s,const char*why){for(unsigned i=0;i<STATE_WORDS;++i)check(d.dut_state[i]==s[i],why);}
static unsigned word(const State&s,unsigned index){
 unsigned value=0;
 for(unsigned b=0;b<18&&index*18+b<STATE_BITS;++b){unsigned p=canonical_bit[index*18+b];value|=bit(s[p/32],p%32)<<b;}
 return value;
}
static void setword(State&s,unsigned index,unsigned value){
 for(unsigned b=0;b<18&&index*18+b<STATE_BITS;++b){unsigned p=canonical_bit[index*18+b];s[p/32]=(s[p/32]&~(1u<<(p%32)))|(bit(value,b)<<(p%32));}
}
static void pins(){
 auto*p=reinterpret_cast<unsigned char*>(&d.stimulus);
 for(unsigned i=0;i<sizeof(d.stimulus);++i)p[i]=i<(STIM_BITS+7)/8?rnd():0;
 if(STIM_BITS%8)p[STIM_BITS/8]&=(1u<<(STIM_BITS%8))-1;
}
static void compare(){
 same(reference(),"ordinary physical state differs");check(d.dut_outputs==d.reference_outputs,"ordinary board outputs differ");
 check(d.storage_ready==d.ref_storage_ready&&d.req_valid==d.ref_req_valid&&d.req_write==d.ref_req_write&&d.rsp_ready==d.ref_rsp_ready,"ordinary raw controls differ");
 check(bool(d.drained)==bool(d.ref_storage_ready&&!d.ref_req_valid),"memory drain boundary differs");
 for(unsigned i=0;i<16;++i)check(d.req_addr[i]==d.ref_req_addr[i]&&d.req_data[i]==d.ref_req_data[i],"ordinary raw payload differs");
}
static void prepare(){
 d.clk=d.ref_clk=0;d.req_ready=d.rsp_valid=0;
 for(unsigned i=0;i<16;++i){
  auto&b=backend[i];if(!b.pending&&(rnd()&3)!=0)d.req_ready|=1u<<i;
  if(b.pending&&!b.delay)d.rsp_valid|=1u<<i;
  d.rsp_data[i]=b.memory[b.address];
 }
 d.eval();
}
static void cycle(bool advance,bool normal){
 d.advance=advance;prepare();
 if(normal){
  compare();if(advance)check(d.storage_ready&&d.ref_storage_ready,"fixture advanced stalled MSA");
  for(unsigned i=0;i<16;++i){
   auto&b=backend[i];bool accept=bit(d.req_valid&d.req_ready,i),response=bit(d.rsp_valid&d.rsp_ready,i);
   if(b.pending&&b.delay)--b.delay;
   if(response){if(b.write){b.memory[b.address]=b.data;++raw_writes;}b.pending=false;}
   if(accept){check(!b.pending,"fixture backend overlap");b.pending=true;b.write=bit(d.req_write,i);b.address=d.req_addr[i];b.data=d.req_data[i];b.delay=1+(rnd()%7);++raw_requests;}
  }
 }else{
  d.req_ready=rnd()&65535;d.rsp_valid=rnd()&65535;for(auto&v:d.rsp_data)v=rnd();d.eval();
  check(!d.storage_ready&&!d.req_valid&&!d.rsp_ready,"held raw request/response leak");
 }
 const bool shifting=d.scan_step;
 d.clk=1;d.ref_clk=normal;d.eval();++edges;if(normal&&advance)++guest_edges;
 if(shifting)++scan_edges;
 if(normal)compare();
 d.clk=d.ref_clk=0;d.eval();
}
static void settle(){
 for(unsigned n=0;n<2000;++n){d.advance=0;prepare();if(d.storage_ready&&d.ref_storage_ready)return;cycle(false,true);}
 check(false,"fixture MSA raw service timeout");
}
static void step(){pins();settle();d.request=rnd()&1;d.write=rnd()&1;d.state_index=rnd()&0xffffff;d.state_data=rnd()&0x3ffff;cycle(true,true);}
static void enter(){
 d.request=d.write=0;d.freeze=d.held=1;
 cycle(false,false);cycle(false,false);
 check(d.ready&&d.frozen&&d.helper_frozen==65535,"hold acknowledgments missing");
}
static unsigned access(unsigned index,bool write,unsigned data=0,bool invalid=false){
 check(d.ready,"access before ready");d.state_index=index;d.write=write;d.state_data=data&0x3ffff;d.request=1;
 cycle(false,false);++state_requests;unsigned waited=0;
 while(!d.done){
  // A competing valid request cannot change the latched operation.
  d.request=d.write=1;d.state_index=149;d.state_data=0x3ffff;
  cycle(false,false);check(++waited<=STATE_BITS+1,"MSA scan completion timeout");
 }
 d.request=0;check(bool(d.error)==invalid,"invalid/valid state index differs");
 if(!invalid)check(waited==STATE_BITS,"MSA scan rotation length differs");
 check(!d.fault&&!d.hold_required,"complete scan retained fault/hold");
 return d.result;
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.eval();compare();
  for(unsigned n=0;n<20000;++n)step();settle();d.request=d.write=0;
  const auto saved=reference();auto saved_pins=d.stimulus;
  // Only forced global hold owns this phase; raw freeze/held are both zero.
  d.global_hold=1;d.freeze=d.held=0;
  for(unsigned n=0;n<32;++n){pins();d.reset=n&1;d.request=d.write=1;d.state_index=n;d.state_data=rnd()&0x3ffff;cycle(true,false);same(saved,"held physical state differs");check(!d.ready&&!d.done&&!d.fault,"forced hold admitted access");}
  d.reset=d.request=d.write=d.global_hold=0;std::memcpy(&d.stimulus,&saved_pins,sizeof(d.stimulus));d.eval();compare();
  enter();same(saved,"hold changed physical state");
  for(unsigned i=0;i<SCAN_WORDS;++i){check(access(i,false)==word(saved,i),"read word differs");same(saved,"read rotation changed physical state");}
  for(unsigned index:{150u,255u,65535u,0xffffffu}){check(access(index,true,0x3ffff,true)==0,"invalid result not zero");same(saved,"invalid write changed physical state");}
  State shadow=saved;
  for(unsigned i=0;i<SCAN_WORDS;++i){unsigned value=(~word(saved,i))&0x3ffff;access(i,true,value);setword(shadow,i,value);same(shadow,"write changed wrong physical bits");}
  for(unsigned i=0;i<SCAN_WORDS;++i){check(access(i,false)==word(shadow,i),"destructive read word differs");same(shadow,"read rotation changed physical state");}
  for(unsigned i=0;i<SCAN_WORDS;++i){access(i,true,word(saved,i));setword(shadow,i,word(saved,i));same(shadow,"restore changed wrong physical bits");}
  same(saved,"full restore differs");
  for(unsigned i=0;i<SCAN_WORDS;++i){check(access(i,false)==word(saved,i),"restored read word differs");same(saved,"read rotation changed physical state");}
  d.request=d.write=d.freeze=d.held=0;std::memcpy(&d.stimulus,&saved_pins,sizeof(d.stimulus));d.eval();compare();
  for(unsigned n=0;n<9;++n)cycle(false,true);settle();cycle(true,true);
  for(unsigned n=0;n<20000;++n)step();settle();d.request=d.write=0;
  // Unheld freeze cannot admit an operation or stop normal host service.
  d.freeze=1;d.held=0;d.request=d.write=1;d.state_index=0;
  for(unsigned n=0;n<12;++n){cycle(false,true);check(!d.ready&&!d.fault&&!d.hold_required,"unheld freeze admitted access");}
  d.freeze=d.request=d.write=0;
  const auto final_saved=reference();std::memcpy(&saved_pins,&d.stimulus,sizeof(d.stimulus));
  enter();d.global_hold=1;d.state_index=0;d.state_data=0x12345;d.request=d.write=1;cycle(false,false);d.request=0;
  for(unsigned n=0;n<7;++n)cycle(false,false);
  d.global_abort=1;cycle(false,false);check(d.fault&&d.hold_required&&d.done&&d.error,"abort did not quarantine");
  const auto partial=physical();d.freeze=d.held=d.global_hold=d.global_abort=0;
  for(unsigned n=0;n<32;++n){pins();d.request=d.write=1;d.state_index=n;cycle(true,false);same(partial,"held physical state differs");check(d.fault&&d.hold_required&&d.frozen&&d.helper_frozen==65535&&!d.ready,"quarantine released");}
  // This is local reset re-entry under an independent global hold. A native
  // SYSTEM reset still must mean actual machine reseed, not a parser reset.
  d.global_hold=d.global_abort=d.freeze=d.held=d.system_reset=1;cycle(false,false);
  check(!d.fault&&!d.hold_required&&!d.done&&!d.ready&&!d.frozen,"system reset admitted access");same(partial,"reset changed held physical state");
  d.system_reset=0;for(unsigned n=0;n<4;++n){cycle(true,false);same(partial,"abort admission changed state");check(!d.ready&&!d.busy&&!d.done,"abort admitted access");}
  d.request=d.write=d.global_abort=0;cycle(false,false);cycle(false,false);check(d.ready,"reset hold re-entry failed");
  for(unsigned i=0;i<SCAN_WORDS;++i)access(i,true,word(final_saved,i));same(final_saved,"quarantine recovery restore differs");
  d.global_hold=d.freeze=d.held=0;std::memcpy(&d.stimulus,&saved_pins,sizeof(d.stimulus));d.eval();compare();settle();cycle(true,true);
  check(raw_requests&&raw_writes&&guest_edges>40000,"missing ordinary MSA coverage");
  std::printf("MSA BOARD PASS host=%llu checks=%llu guest=%llu raw_requests=%llu raw_writes=%llu state_requests=%llu scan_edges=%llu physical=2690 fields=458 packages=78 frontends=16 words=150 last=8 post_restore=20000\n",edges,checks,guest_edges,raw_requests,raw_writes,state_requests,scan_edges);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
