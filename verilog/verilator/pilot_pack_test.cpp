// Decode only serial cable bits, then compare with the mounted raw image.
// The selected sector is physical (3,0,16), Pilot germ's virtual (3,20).
#include "Vtb_pilot_pack.h"
#include <verilated.h>
#include "hps_disk_test.h"
#include <cstdio>
#include <cstdlib>
int main(int argc,char**argv){
 if(argc!=2)return 2;
 VerilatedContext ctx;ctx.commandArgs(argc,argv);Vtb_pilot_pack top(&ctx);HpsDiskTest disk(argv[1]);
 top.clk_host=top.sys_clk=0;top.reset=1;top.attached=top.data_adv=0;
 top.sd_ack=top.sd_buff_wr=0;top.sd_buff_addr=top.sd_buff_dout=0;top.eval();
 unsigned record=0,bits=0,word=0,crc=0,shift=0,steps=0,words=0;
 const unsigned lengths[]={2,10,256},bases[]={1,3,13};
 bool syncing=true;
 for(unsigned host=0;host<200000 && record<3;host++){
  top.clk_host=top.sys_clk=0;top.eval();top.reset=host<4;top.attached=host>=5;
  top.data_adv=steps%8==6;disk.before(top);top.eval();bool run=top.pack_ready&&!top.reset;
  top.clk_host=1;top.sys_clk=run;top.eval();disk.after(top);ctx.timeInc(1);
  if(top.pack_fault)throw std::runtime_error("Pilot medium fault");
  if(!run)continue;
  if(top.data_adv){
   unsigned bit=top.media_bit;shift=((shift<<1)|bit)&65535;
   if(syncing){
    if(shift==0201){syncing=false;bits=0;word=0;crc=0;
     for(int b=15;b>=0;b--)crc=(crc<<1)^((((0201>>b)&1)^(crc>>31))?0x00a00805u:0u);
    }
   }else{
    crc=(crc<<1)^((bit^(crc>>31))?0x00a00805u:0u);bits++;
    if(bits%16==0 && bits<=lengths[record]*16){
     uint64_t offset=uint64_t((3*5*29+16)*269+bases[record]+word)*2;
     if(offset+1>=disk.media.size())throw std::runtime_error("short Pilot image");
     unsigned expected=disk.media[offset]|(unsigned(disk.media[offset+1])<<8);
     if(record==0 && expected!=(word==0?3u:20u))throw std::runtime_error("wrong Pilot virtual header at germ physical location");
     if(shift!=expected){fprintf(stderr,"record %u word %u got %04x expected %04x\n",record,word,shift,expected);return 1;}
     word++;words++;
    }
    if(bits==lengths[record]*16+32){
     if(crc)throw std::runtime_error("Pilot serial Fire-code residue");
     record++;syncing=true;shift=0;
    }
   }
  }
  steps++;
 }
 if(record!=3 || words!=268 || disk.reads!=1 || disk.writes)throw std::runtime_error("incomplete Pilot serial read");
 printf("PILOT-PACK PASS: header 2, label 10, data 256 words byte-exact; all three wire Fire codes checked; one HPS read\n");
 top.final();
}
