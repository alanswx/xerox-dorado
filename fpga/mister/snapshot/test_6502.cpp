#include "Vnetlist6502_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

using Nodes=std::array<uint32_t,54>;
static uint32_t random_word(){static uint32_t s=0x65021725;s^=s<<13;s^=s>>17;s^=s<<5;return s;}
template<class T>static Nodes nodes(const T&v){Nodes n{};for(unsigned i=0;i<54;i++)n[i]=v[i];return n;}
static unsigned bit(const Nodes&n,unsigned i){return(n[i/32]>>(i%32))&1;}
static uint32_t word(const Nodes&n,unsigned index){uint32_t v=0;for(unsigned k=0;k<18&&index*18+k<1725;k++)v|=bit(n,index*18+k)<<k;return v;}
static void write_word(Nodes&n,unsigned index,uint32_t data){for(unsigned k=0;k<18&&index*18+k<1725;k++){unsigned b=index*18+k;uint32_t mask=1u<<(b%32);n[b/32]=(n[b/32]&~mask)|(((data>>k)&1)?mask:0);}}

struct Test{
 Vnetlist6502_fixture v;Nodes oracle{};bool was_frozen=false;
 uint64_t clocks=0,held_clocks=0,requests=0,writes=0;
 Test(){v.host_clk=0;v.ref_clk=0;v.advance=0;v.freeze=0;v.held=0;v.request=0;v.write=0;v.index=0;v.data=0;v.pins=0;v.eval();oracle=nodes(v.physical);}
 [[noreturn]]void fail(const char*s){throw std::runtime_error(std::string(s)+" at host edge "+std::to_string(clocks));}
 void step(bool ref=true,bool compare=true){
  v.host_clk=0;v.ref_clk=0;v.eval();
  bool hold=v.freeze&&v.held,accept=hold&&was_frozen&&v.request,valid=v.index<96;
  Nodes expected=oracle;uint32_t read=0;
  if(accept&&valid){read=word(oracle,v.index);if(v.write)write_word(expected,v.index,v.data);}
  if(!hold&&v.advance)expected=nodes(v.next_state);
  if(!hold&&v.advance&&ref)v.ref_clk=1;
  v.host_clk=1;v.eval();++clocks;if(hold)++held_clocks;if(accept)++requests;if(accept&&valid&&v.write)++writes;
  if(nodes(v.physical)!=expected)fail("node oracle differs");
  if(bool(v.frozen)!=hold||bool(v.done)!=accept||bool(v.error)!=(accept&&!valid))fail("request handshake differs");
  if(accept&&(!valid||!v.write)&&v.result!=(valid?read:0))fail("read result differs");
  if(compare&&(nodes(v.physical)!=nodes(v.original_state)||v.outputs!=v.original_outputs))fail("original continuation differs");
  oracle=expected;was_frozen=hold;v.host_clk=0;v.ref_clk=0;v.eval();
 }
 void access(unsigned i,bool write,uint32_t data=0){v.index=i;v.write=write;v.data=data;v.request=1;step(false,false);v.request=0;}
 void normal(unsigned n){v.freeze=0;v.held=0;v.request=0;for(unsigned i=0;i<n;i++){v.pins=random_word()&0x3fff;v.advance=(random_word()&3)!=0;step();}}
};

int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  Test t;t.normal(2000);
  // Exhaust every external pin vector and preserve the real relaxation
  // result at each advance, including gaps in the host clock enable.
  for(unsigned p=0;p<16384;p++){t.v.pins=p;t.v.advance=(p%7)!=0;t.step();}
  // Read requests may arrive with freeze/held missing, during entry, or
  // while genuine holds are active. Original CPU advances only on real edges.
  for(unsigned i=0;i<5000;i++){
   t.v.pins=random_word()&0x3fff;t.v.advance=random_word()&1;
   t.v.freeze=random_word()&1;t.v.held=random_word()&1;
   t.v.request=random_word()&1;t.v.write=0;t.v.index=random_word()&127;t.step();
  }
  t.v.request=0;t.v.freeze=0;t.v.held=0;t.v.advance=0;t.step();
  Nodes saved=t.oracle;
  t.v.freeze=1;t.v.held=1;t.v.request=1;t.v.write=1;t.v.index=0;t.v.data=0x3ffff;
  t.step(false); // entry write must not be accepted
  t.v.request=0;
  for(unsigned i=0;i<96;i++)t.access(i,true,0);
  // Every node is the sole set bit at least once. Check the complete
  // physical vector after each operation, independently of the scan result.
  for(unsigned b=0;b<1725;b++){
   t.access(b/18,true,1u<<(b%18));t.access(b/18,false);t.access(b/18,true,0);
  }
  for(unsigned pattern=0;pattern<5;pattern++){
   for(unsigned i=0;i<96;i++)t.access(i,true,pattern==0?0x3ffff:pattern==1?0x15555:pattern==2?0x2aaaa:random_word()&0x3ffff);
   for(unsigned i=0;i<96;i++)t.access(i,false);
  }
  for(unsigned i=96;i<128;i++){t.access(i,true,0x3ffff);t.access(i,false);}
  // Restore then destructively continue only the candidate. The original
  // stays at the saved boundary. Restore again and compare the first edge.
  for(unsigned i=0;i<96;i++)t.access(i,true,word(saved,i));
  t.v.freeze=0;t.v.held=0;t.v.request=0;t.v.advance=1;
  for(unsigned i=0;i<1000;i++){t.v.pins=random_word()&0x3fff;t.step(false,false);}
  t.v.freeze=1;t.v.held=1;t.step(false,false);
  for(unsigned i=0;i<96;i++)t.access(i,true,word(saved,i));
  for(unsigned i=0;i<96;i++)t.access(i,false);
  t.v.freeze=0;t.v.held=0;t.v.advance=0;t.v.request=1;t.v.write=1;t.v.index=0;t.v.data=0x3ffff;t.step();
  t.v.request=0;t.v.advance=1;t.step();
  t.normal(20000);
  std::printf("NETLIST6502 PASS: %llu host/%llu held/%llu accepted/%llu writes; all1725bits/all128indices/exhaust16384pins/destructive restore/firstedge/20000more\n",t.clocks,t.held_clocks,t.requests,t.writes);
  return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
