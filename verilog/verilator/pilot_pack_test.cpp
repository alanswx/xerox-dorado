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
 // Make the two extra label words distinctive in the in-memory fixture.
 // Neither the source pack nor the production boot medium is modified.
 const uint64_t extra_label=uint64_t((3*5*29+16)*269+3+8)*2;
 for(unsigned i=0;i<4;i++)disk.media.at(extra_label+i)=uint8_t(0x24681357u>>(8*i));
 const auto original=disk.media;
 std::vector<unsigned char> cable;
 std::vector<unsigned> payload_offsets;
 top.clk_host=top.sys_clk=0;top.reset=1;top.attached=top.data_adv=0;
 top.writing=top.write_bit=0;top.pulse=68;
 top.sd_ack=top.sd_buff_wr=0;top.sd_buff_addr=top.sd_buff_dout=0;top.eval();
 unsigned record=0,bits=0,word=0,crc=0,shift=0,steps=0,words=0;
 const unsigned lengths[]={2,10,256},bases[]={1,3,13};
 bool syncing=true;
 unsigned host=0;bool expect_fault=false;
 auto tick=[&](){
  top.clk_host=top.sys_clk=0;top.eval();top.reset=host<4;top.attached=host>=5;
  top.data_adv=steps%8==6;disk.before(top);top.eval();bool run=top.pack_ready&&!top.reset;
  bool advance=run&&top.data_adv;
  top.clk_host=1;top.sys_clk=run;top.eval();disk.after(top);ctx.timeInc(1);
  if(top.pack_fault&&!expect_fault)throw std::runtime_error("Pilot medium fault");
  host++;if(run)steps++;
  return advance;
 };
 for(;host<200000 && record<3;){
  if(tick()){
   unsigned bit=top.media_bit;shift=((shift<<1)|bit)&65535;
   cable.push_back(bit);
   if(syncing){
    if(shift==0201){syncing=false;bits=0;word=0;crc=0;
     payload_offsets.push_back(cable.size());
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
 }
 if(record!=3 || words!=268 || disk.reads!=1 || disk.writes)throw std::runtime_error("incomplete Pilot serial read");
 // Change both extra label words and both ends of the data block. An ignored
 // write must fail, unlike a loopback that merely rewrites existing bytes.
 auto expected=original;
 const unsigned changed_words[]={3+8,3+9,13,13+255};
 for(unsigned i=0;i<4;i++){
  uint64_t at=uint64_t((3*5*29+16)*269+changed_words[i])*2;
  expected[at]^=uint8_t(0xa55a>>(8*(i&1)));
  expected[at+1]^=uint8_t(0x5aa5>>(8*(i&1)));
 }
 // Rebuild payload/check bits independently from the expected disk bytes.
 for(unsigned r=0;r<3;r++){
  unsigned check=0;
  auto divide=[&](unsigned bit){check=(check<<1)^((bit^(check>>31))?0x00a00805u:0u);};
  for(int b=15;b>=0;b--)divide((0201>>b)&1);
  for(unsigned w=0;w<lengths[r];w++){
   uint64_t at=uint64_t((3*5*29+16)*269+bases[r]+w)*2;
   unsigned value=expected[at]|(unsigned(expected[at+1])<<8);
   for(unsigned b=0;b<16;b++){
    unsigned bit=(value>>(15-b))&1;
    cable[payload_offsets[r]+w*16+b]=bit;divide(bit);
   }
  }
  for(unsigned b=0;b<32;b++)cable[payload_offsets[r]+lengths[r]*16+b]=(check>>(31-b))&1;
 }
 // Restart this sector's physical waveform, then send the changed records
 // into its differential write input, including all three Fire codes.
 top.pulse=64;tick();top.pulse=68;tick();top.writing=1;
 unsigned sent=0;
 while(sent<cable.size() && host<400000){
  top.write_bit=cable[sent];if(tick())sent++;
 }
 if(sent!=cable.size())throw std::runtime_error("incomplete Pilot write stream");
 top.writing=0;
 while(host<500000){
  tick();
  if(disk.writes && !disk.active && top.pack_ready && !top.sd_wr)break;
 }
 if(disk.writes!=1 || disk.active || !top.pack_ready)throw std::runtime_error("Pilot HPS writeback did not finish");
 if(disk.media!=expected)throw std::runtime_error("Pilot writeback lost changed words or damaged neighboring bytes");
 printf("PILOT-PACK PASS: header 2, label 10, data 256 words; three Fire codes; changed labels/data persisted through HPS; neighboring bytes intact\n");
 // A damaged controller Fire code must stop the medium and retain its
 // first-fault reason/residue; observation must not invent a successful write.
 expect_fault=true;top.pulse=64;tick();top.pulse=68;tick();top.writing=1;
 cable[payload_offsets[0]+lengths[0]*16]^=1;
 sent=0;
 while(!top.pack_fault && sent<cable.size()){
  top.write_bit=cable[sent];if(tick())sent++;
 }
 if(!top.pack_fault || (top.fault_debug>>28)!=4 || !top.fault_extra)
  throw std::runtime_error("missing first bad-CRC diagnostic");
 auto debug=top.fault_debug,extra=top.fault_extra;
 top.writing=0;for(unsigned i=0;i<100;i++)tick();
 if(top.fault_debug!=debug || top.fault_extra!=extra || disk.media!=expected)
  throw std::runtime_error("first-fault evidence changed or bad block persisted");
 printf("PACK-FAULT PASS: damaged Fire code stops, reason4/residue retained, no host write invented\n");
 top.final();
}
