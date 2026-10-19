#include "Vi2125_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

static uint32_t rnd(){static uint32_t s=0x21253280;s^=s<<13;s^=s>>17;s^=s<<5;return s;}
using Image=std::array<std::array<uint8_t,1024>,4>;
struct Test{
 Vi2125_fixture v;Image mem{};std::array<uint8_t,4>pipe{};bool was_frozen=false;
 uint64_t clocks=0,holds=0,accepted=0,writes=0,late=0;
 Test(){v.host_clk=v.ref_clk=0;v.advance=v.freeze=v.held=v.request=v.write=0;v.address=v.data=v.ce_n=v.we_n=v.index=v.state_data=0;v.eval();}
 [[noreturn]]void fail(const char*s){throw std::runtime_error(std::string(s)+" at host edge "+std::to_string(clocks));}
 void step(bool ref=true,bool compare=true){
  v.host_clk=0;v.ref_clk=0;v.eval();bool hold=v.freeze&&v.held,accept=hold&&was_frozen&&v.request;
  unsigned bank=v.index>>11,row=v.index&2047;bool valid=row<=1024;unsigned read=row<1024?mem[bank][row]:row==1024?pipe[bank]:0;
  if(accept&&valid&&v.write){if(row<1024)mem[bank][row]=v.state_data&255;else pipe[bank]=v.state_data&255;}
  if(!hold&&v.advance){
   for(unsigned k=0;k<4;k++){
    pipe[k]=mem[k][v.address];
    if(!(v.ce_n&(1u<<k))&&!(v.we_n&(1u<<k)))mem[k][v.address]=v.data;
   }
  }
  if(!hold&&v.advance&&ref)v.ref_clk=1;
  v.host_clk=1;v.eval();++clocks;if(hold)++holds;if(accept)++accepted;if(accept&&valid&&v.write)++writes;
  unsigned expected_done=accept?1u<<bank:0,expected_error=accept&&!valid?1u<<bank:0;
  if(v.frozen!=(hold?15:0)||v.done!=expected_done||v.error!=expected_error)fail("2125 handshake differs");
  for(unsigned k=0;k<4;k++){
   if(std::memcmp(&v.physical[k][0],mem[k].data(),1024))fail("2125 RAM differs");
   if(v.raw_q[k]!=pipe[k])fail("2125 raw pipeline differs");
   unsigned q=!(v.ce_n&(1u<<k))&&(v.we_n&(1u<<k))?pipe[k]:0;
   if(v.q[k]!=q)fail("2125 output differs");
   if(compare&&(v.original_q[k]!=q||v.original_raw[k]!=pipe[k]))fail("untouched 2125 differs");
  }
  if(accept&&(!valid||!v.write)&&v.result[bank]!=(valid?read:0))fail("2125 scan read/padding differs");
  was_frozen=hold;v.host_clk=0;v.ref_clk=0;v.eval();
 }
 void pins(){v.address=rnd()&1023;v.data=rnd()&255;v.ce_n=rnd()&15;v.we_n=rnd()&15;v.advance=rnd()&1;}
 void access(unsigned bank,unsigned row,bool write,unsigned data=0){pins();++late;v.index=(bank<<11)|row;v.request=1;v.write=write;v.state_data=data;step(false,false);v.request=0;}
 void random(unsigned n,bool ref=true,bool compare=true){for(unsigned i=0;i<n;i++){pins();v.request=rnd()&1;v.write=0;v.index=rnd()&8191;step(ref,compare);}v.request=0;}
};

int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  Test t;t.v.advance=1;
  for(unsigned k=0;k<4;k++)for(unsigned a=0;a<1024;a++){
   t.v.address=a;t.v.data=(a*73)^(k*49);t.v.ce_n=15^(1u<<k);t.v.we_n=0;t.step();
  }
  t.v.ce_n=0;t.v.we_n=15;
  for(unsigned a=0;a<1024;a++){t.v.address=a;t.step();}
  for(unsigned cs=0;cs<16;cs++)for(unsigned we=0;we<16;we++){
   t.v.ce_n=cs;t.v.we_n=we;t.v.address=rnd()&1023;t.v.data=rnd()&255;t.v.advance=1;t.step();
   t.v.advance=0;t.v.address=rnd()&1023;t.v.data=rnd()&255;t.step();
  }
  t.random(5000);
  // Restore known dense RAM and a raw read which differs from the live
  // address before freeze, so a lost pipeline cannot hide in current data.
  t.v.advance=1;
  for(unsigned k=0;k<4;k++)for(unsigned a=0;a<1024;a++){
   t.v.address=a;t.v.data=(a*73)^(k*49);t.v.ce_n=15^(1u<<k);t.v.we_n=0;t.step();
  }
  t.v.ce_n=0;t.v.we_n=15;t.v.address=13;t.step();
  t.v.advance=0;t.v.address=37;t.step();
  for(unsigned k=0;k<4;k++)if(t.pipe[k]==t.mem[k][37])t.fail("missing noncurrent pipeline coverage");
  Image saved=t.mem;auto saved_pipe=t.pipe;
  t.v.freeze=1;t.v.held=1;t.v.request=1;t.v.write=1;t.v.index=0;t.v.state_data=255;t.step(false);t.v.request=0;
  for(unsigned k=0;k<4;k++)for(unsigned a=0;a<=1024;a++)t.access(k,a,true,0);
  for(unsigned bit=0;bit<32800;bit++){
   unsigned bank=bit/8200,within=bit%8200,row=within/8;
   t.access(bank,row,true,1u<<(within%8));t.access(bank,row,false);t.access(bank,row,true,0);
  }
  for(unsigned k=0;k<4;k++)for(unsigned a=0;a<=1024;a++)t.access(k,a,true,0x3ff00); // padding only
  for(unsigned k=0;k<4;k++)for(unsigned a=1025;a<2048;a++){t.access(k,a,true,0x3ffff);t.access(k,a,false);}
  for(unsigned k=0;k<4;k++){
   for(unsigned a=0;a<1024;a++)t.access(k,a,true,saved[k][a]);
   t.access(k,1024,true,saved_pipe[k]);
  }
  t.v.freeze=0;t.v.held=0;t.v.request=0;t.v.advance=0;t.v.ce_n=0;t.v.we_n=15;t.v.address=713;t.step();
  t.v.advance=1;t.step(); // first real edge after a different live address
  t.random(1000);
  saved=t.mem;saved_pipe=t.pipe;t.v.freeze=1;t.v.held=1;t.v.request=0;t.step(false);
  t.v.freeze=0;t.v.held=0;t.random(2000,false,false);
  t.v.freeze=1;t.v.held=1;t.v.request=0;t.step(false,false);
  for(unsigned k=0;k<4;k++){
   for(unsigned a=0;a<1024;a++)t.access(k,a,true,saved[k][a]);
   t.access(k,1024,true,saved_pipe[k]);
   for(unsigned a=0;a<=1024;a++)t.access(k,a,false);
  }
  t.v.freeze=0;t.v.held=0;t.v.request=1;t.v.write=1;t.v.index=0;t.v.state_data=0xff;t.v.advance=0;t.step();
  t.v.request=0;t.v.advance=1;t.step();t.random(20000);
  std::printf("I2125 PASS: %llu host/%llu held/%llu accepted/%llu writes/%llu late changes; all32800bits/all8192indices/allCE-WE/noncurrent pipeline/padding/destructive restore/firstedge/20000more\n",t.clocks,t.holds,t.accepted,t.writes,t.late);return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
