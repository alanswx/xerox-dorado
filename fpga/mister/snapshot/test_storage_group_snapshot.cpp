#include "Vstorage_snapshot_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

static Vstorage_snapshot_fixture d;
static constexpr std::array<unsigned,2> widths{8,12},bits{111,123};
using Word=std::array<uint32_t,4>;
using Physical=std::array<Word,2>;
struct Backend{bool pending=false,write=false;unsigned addr=0,data=0,delay=0;std::vector<uint16_t> memory=std::vector<uint16_t>(65536,0);};
static std::array<Backend,2> back;
static uint32_t rng=0x8661824a;
static unsigned long long edges=0,checks=0,guests=0,requests=0,writes=0,scans=0;
static unsigned snapshots=0,pending_reads=0,pending_writes=0,committed_writes=0,resets=0;
static unsigned rnd(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static unsigned bit(unsigned value,unsigned index){return(value>>index)&1;}
static void check(bool ok,const char*why){++checks;if(!ok)throw std::runtime_error(std::string(why)+" at edge "+std::to_string(edges));}
static Physical physical(){Physical p{};for(unsigned i=0;i<2;++i)for(unsigned j=0;j<4;++j)p[i][j]=d.physical[i][j];return p;}
static void same_physical(const Physical&p,const char*why){for(unsigned i=0;i<2;++i)for(unsigned j=0;j<4;++j)check(d.physical[i][j]==p[i][j],why);}
static void compare(){
 for(unsigned i=0;i<2;++i){
  for(unsigned j=0;j<4;++j)check(d.physical[i][j]==d.original_state[i][j],"physical state differs");
  check(d.data_out[i]==d.ref_data_out[i],"original output differs");
  check(d.data_out[i]==d.chip_data_out[i],"original DRAM chip output differs");
  check(d.req_addr[i]==d.ref_req_addr[i]&&d.req_data[i]==d.ref_req_data[i],"raw payload differs");
 }
 check(d.ready==d.ref_ready&&d.req_valid==d.ref_req_valid&&d.req_write==d.ref_req_write&&d.rsp_ready==d.ref_rsp_ready,"raw control differs");
 check(d.drained==unsigned(d.ref_ready&~d.ref_req_valid&3),"drain boundary differs");
}
static void prepare(){
 d.clk=d.ref_clk=0;d.req_ready=d.rsp_valid=0;
 for(unsigned i=0;i<2;++i){
  auto&b=back[i];
  if(!b.pending&&(rnd()&3)!=0)d.req_ready|=1u<<i;
  if(b.pending&&b.delay==0)d.rsp_valid|=1u<<i;
  d.rsp_data[i]=b.memory[b.addr];
 }
 d.eval();
}
static void cycle(bool advance,bool normal){
 d.advance=advance;prepare();
 if(normal){
  compare();if(advance)check(d.ready==3,"test advanced stalled storage");
 }else{
  // Foreign pins, reset and handshakes may change while all physical state is held.
  d.req_ready=rnd()&3;d.rsp_valid=rnd()&3;d.rsp_data[0]=rnd();d.rsp_data[1]=rnd();d.eval();
  check(d.ready==0&&d.req_valid==0&&d.rsp_ready==0,"held raw request/response leak");
 }
 if(normal)for(unsigned i=0;i<2;++i){
  auto&b=back[i];bool accept=bit(d.req_valid&d.req_ready,i),response=bit(d.rsp_valid&d.rsp_ready,i);
  if(b.pending&&b.delay)--b.delay;
  if(response){if(b.write){b.memory[b.addr]=b.data;++writes;}b.pending=false;}
  if(accept){check(!b.pending,"test backend overlap");b.pending=true;b.write=bit(d.req_write,i);b.addr=d.req_addr[i];b.data=d.req_data[i];b.delay=1+(rnd()%7);++requests;}
 }
 d.clk=1;d.ref_clk=normal;d.eval();++edges;if(normal&&advance)++guests;
 if(normal)compare();
 d.clk=d.ref_clk=0;d.eval();
}
static void shift(unsigned selected,bool value,Physical&expected){
 d.selected=selected;d.scan_step=1;d.serial_in=value;
 auto&w=expected[selected];
 for(unsigned j=0;j<4;++j)w[j]=(w[j]>>1)|(j<3?(w[j+1]&1u)<<31:0);
 w[(bits[selected]-1)/32]|=unsigned(value)<<((bits[selected]-1)%32);
 for(unsigned b=bits[selected];b<128;++b)w[b/32]&=~(1u<<(b%32));
 cycle(rnd()&1,false);++scans;same_physical(expected,"serial physical oracle differs");
}
static void load(unsigned selected,const Word&word,Physical&expected){for(unsigned b=0;b<bits[selected];++b)shift(selected,bit(word[b/32],b%32),expected);}
static void snapshot(bool walking){
 ++snapshots;
 for(unsigned i=0;i<2;++i){
  if(back[i].pending){if(back[i].write)++pending_writes;else ++pending_reads;}
  // write_done is just above cache_valid in the exact compact physical layout.
  if(bit(d.physical[i][(4+3*widths[i])/32],(4+3*widths[i])%32))++committed_writes;
 }
 const auto saved=physical();auto expected=saved;
 unsigned r=d.ras_n,c=d.cas_n,w=d.we_n;
 std::array<unsigned,2> addresses{},data{};
 for(unsigned i=0;i<2;++i){addresses[i]=d.next_addr[i];data[i]=d.data_in[i];}
 d.hold=1;d.scan_step=0;
 for(unsigned n=0;n<4;++n){d.reset=n&1;d.ras_n=rnd()&3;d.cas_n=rnd()&3;d.we_n=rnd()&3;for(unsigned i=0;i<2;++i){d.next_addr[i]=rnd();d.data_in[i]=rnd();}cycle(true,false);same_physical(saved,"held physical state differs");}
 check(d.frozen==3,"missing hold acknowledgment");d.reset=0;
 for(unsigned i=0;i<2;++i){
  for(unsigned b=0;b<bits[i];++b){bool value=bit(d.serial_out,i);check(value==bit(saved[i][b/32],b%32),"serial read order differs");shift(i,value,expected);}
  same_physical(saved,"read rotation changed state");
 }
 for(unsigned i=0;i<2;++i){
  Word zero{};load(i,zero,expected);
  if(walking)for(unsigned b=0;b<bits[i];++b){Word one{};one[b/32]=1u<<(b%32);load(i,one,expected);load(i,zero,expected);}
  Word ones{};ones.fill(0xffffffff);load(i,ones,expected);
 }
 for(unsigned i=0;i<2;++i)load(i,saved[i],expected);
 same_physical(saved,"restore changed physical state");d.scan_step=0;
 for(unsigned n=0;n<5;++n){cycle(rnd()&1,false);same_physical(saved,"held post-restore state differs");}
 d.ras_n=r;d.cas_n=c;d.we_n=w;
 for(unsigned i=0;i<2;++i){d.next_addr[i]=addresses[i];d.data_in[i]=data[i];}
 d.hold=0;d.advance=0;d.eval();compare();
 // Resume host completion first. The next legal guest edge is checked by step().
 cycle(false,true);
}
static void settle(){
 for(unsigned n=0;n<1000;++n){d.advance=0;prepare();if(d.ready==3)return;cycle(false,true);}
 check(false,"test backend timeout");
}
static void step(unsigned r,unsigned c,unsigned w,unsigned a,unsigned value){
 d.ras_n=r;d.cas_n=c;d.we_n=w;
 d.next_addr[0]=a&65535;d.next_addr[1]=(a^0xa581)&65535;
 d.data_in[0]=value&255;d.data_in[1]=(value^0x721)&4095;
 settle();d.scan_step=1;d.serial_in=rnd()&1;cycle(true,true);d.scan_step=0;
}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.ras_n=d.cas_n=d.we_n=3;d.eval();compare();
  step(3,3,3,0xffff,0);step(0,0,0,0xffff,0x5ab);step(0,0,3,0xffff,0);step(0,0,3,0xffff,0);
  // Force snapshots in accepted read, accepted write and write-done-before-guest-commit phases.
  step(3,3,3,0x8765,0);
  for(unsigned n=0;n<100;++n){cycle(false,true);if(back[0].pending&&!back[0].write){snapshot(true);break;}}
  step(0,0,0,0x8765,0x9d3);
  for(unsigned n=0;n<100;++n){cycle(false,true);if(back[0].pending&&back[0].write){snapshot(false);break;}}
  settle();snapshot(false);step(0,0,3,0x8765,0);step(0,0,3,0x8765,0);
  for(unsigned n=0;n<5000;++n){
   step(rnd()&3,rnd()&3,rnd()&3,rnd(),rnd());
   if(n%997==0){settle();snapshot(false);}
   if(n%701==0){settle();d.reset=1;for(unsigned k=0;k<3;++k)cycle(false,true);d.reset=0;++resets;}
  }
  step(3,3,3,0,0);step(3,3,3,0,0);settle();
  for(unsigned a=0;a<65536;++a){d.probe_addr=a;d.eval();for(unsigned i=0;i<2;++i)check(d.chip_word[i]==back[i].memory[a],"full original chip backing differs");}
  check(pending_reads&&pending_writes&&committed_writes,"missing pending-operation snapshot coverage");
  check(writes&&requests&&resets,"missing backend/reset coverage");
  std::printf("STORAGE SNAPSHOT PASS host=%llu checks=%llu guest=%llu requests=%llu writes=%llu scan=%llu snapshots=%u pending_read=%u pending_write=%u write_done=%u resets=%u physical=111/123 raw_bits=1310720\n",edges,checks,guests,requests,writes,scans,snapshots,pending_reads,pending_writes,committed_writes,resets);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
