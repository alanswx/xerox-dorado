#include "Vtb_hps_persistence.h"
#include "hps_disk_test.h"
#include <cstdio>
#include <memory>
#include <string>

static void tick(Vtb_hps_persistence&t,HpsDiskTest&d,VerilatedContext&ctx,bool host=true){
 if(host)d.before(t);else{t.sd_ack=0;t.sd_buff_wr=0;}
 t.sys_clk=0;t.clk_host=0;t.eval();bool run=t.pack_ready;
 t.clk_host=1;t.sys_clk=run;t.eval();if(host)d.after(t);ctx.timeInc(10);
}
static std::unique_ptr<Vtb_hps_persistence> model(VerilatedContext&ctx,bool verify=false){
 auto t=std::make_unique<Vtb_hps_persistence>(&ctx);
 t->sys_clk=0;t->clk_host=0;t->verify_only=verify;t->corrupt=0;t->read_only=0;
 t->attached=1;t->remounted=0;t->sd_ack=0;t->sd_buff_wr=0;t->sd_buff_addr=0;t->sd_buff_dout=0;t->eval();return t;
}
static void save(const std::string&path,const std::vector<uint8_t>&bytes){
 std::ofstream f(path,std::ios::binary);f.exceptions(std::ios::badbit|std::ios::failbit);
 f.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());
}
static void check(bool ok,const char*what){if(!ok)throw std::runtime_error(what);}
int main(int argc,char**argv){try{
 if(argc!=3)throw std::runtime_error("usage: hps-persistence-test BASE_IMAGE OUTPUT_PREFIX");
 HpsDiskTest source(argv[1]);auto original=source.media,expected=original;
 // Change data word19 in cylinder4/head4/sector0. No labels or adjacent
 // tracks change. The reference medium generates the corresponding Fire code.
 const unsigned track=24*29*267,word=track+11+19;
 expected.at(2*word)^=0xa5;expected.at(2*word+1)^=0x5a;
 std::string prefix=argv[2],variant=prefix+"-source.pack",persisted=prefix+"-persisted.pack";
 save(variant,expected);std::string plus="+pack="+variant;
 const char*args[]={argv[0],plus.c_str()};
 {
  VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(2,args);
  auto t=model(ctx);HpsDiskTest d(argv[1]);
  for(unsigned i=0;i<1000000&&!t->done&&!t->pack_fault;i++)tick(*t,d,ctx);
  check(t->done&&!t->pack_fault,"checked write did not flush without a seek");
  check(d.reads==1&&d.writes==1,"unexpected seek/read or missing automatic flush");
  check(d.media==expected,"persisted bytes differ or neighboring data was damaged");
  save(persisted,d.media);t->final();
 }
 {
  VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(2,args);
  // A fresh controller and a file reopened from disk must return the data
  // just written, including its serial framing/CRC, without saved RAM state.
  auto t=model(ctx,true);HpsDiskTest d(persisted.c_str());
  for(unsigned i=0;i<1000000&&!t->done&&!t->pack_fault;i++)tick(*t,d,ctx);
  check(t->done&&!t->pack_fault&&t->compared>5000,"fresh-medium serial readback failed");
  check(d.reads==1&&d.writes==0,"readback unexpectedly wrote the image");
  // Switch a clean mounted image: the old track cache must be invalidated.
  d.media=original;t->remounted=1;tick(*t,d,ctx);t->remounted=0;
  for(unsigned i=0;i<20000&&(d.reads<2||!t->pack_ready);i++)tick(*t,d,ctx);
  check(!t->pack_fault&&d.reads==2&&t->pack_ready,"clean remount did not refresh its cache");
  t->sd_buff_addr=(track&255)+11+19;tick(*t,d,ctx);tick(*t,d,ctx);
  check(t->sd_buff_din==uint16_t(original[2*word]|original[2*word+1]<<8),"remount returned stale cached bytes");
  t->final();
 }
 for(int mode=0;mode<4;mode++){
  VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(2,args);
  auto t=model(ctx);HpsDiskTest d(argv[1]);
  t->corrupt=mode==0;t->read_only=mode==1;
  for(unsigned i=0;i<1000000&&!t->pack_fault;i++){
   if(mode>=2&&t->pack_dirty){if(mode==2)t->remounted=1;else t->attached=0;}
   tick(*t,d,ctx);
  }
  check(t->pack_fault&&!t->pack_ready,"unsafe write/remount/unmount failed to stop");
  check(d.writes==0&&d.media==original,"invalid or interrupted data reached backing image");t->final();
 }
 {
  VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(2,args);
  auto t=model(ctx);HpsDiskTest d(argv[1]);
  for(unsigned i=0;i<21000&&!t->pack_fault;i++)tick(*t,d,ctx,false);
  check(t->pack_fault&&!t->pack_ready&&!t->sd_rd&&!t->sd_wr,"missing-host timeout did not stop cleanly");t->final();
 }
 printf("HPS-PERSISTENCE PASS: checked final-track write, reopened serial readback, clean remount, corrupt/readonly/interrupted writes and host timeout\n");
 return 0;
}catch(const std::exception&e){fprintf(stderr,"HPS-PERSISTENCE FAIL: %s\n",e.what());return 1;}}
