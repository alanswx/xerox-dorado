#include "Vtb_hps_pack.h"
#include "hps_disk_test.h"
#include <cstdio>
int main(int argc,char**argv){try{
 if(argc<2)throw std::runtime_error("usage: hps-pack-test IMAGE +pack=IMAGE");
 VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(argc,argv);Vtb_hps_pack t(&ctx);HpsDiskTest disk(argv[1]);auto original=disk.media;
 t.sys_clk=0;t.clk_host=0;t.sd_ack=0;t.sd_buff_wr=0;t.eval();
 for(unsigned long i=0;i<1000000&&!t.done;i++){
  disk.before(t);t.sys_clk=0;t.clk_host=0;t.eval();bool run=t.pack_ready;
  t.clk_host=1;t.sys_clk=run;t.eval();disk.after(t);ctx.timeInc(10);
  if(t.pack_fault)throw std::runtime_error("serial write decoder reported a fault");
 }
 if(!t.done||t.compared<10000||disk.reads<2||disk.writes<1)throw std::runtime_error("test did not complete all read/write/seek checks");
 if(disk.media!=original)throw std::runtime_error("loopback write damaged disk bytes");
 printf("HPS-PACK PASS: %u serial bits identical, %u reads, %u writes, full image unchanged after loopback\n",t.compared,disk.reads,disk.writes);
 t.final();return 0;
}catch(const std::exception&e){fprintf(stderr,"HPS-PACK FAIL: %s\n",e.what());return 1;}}
