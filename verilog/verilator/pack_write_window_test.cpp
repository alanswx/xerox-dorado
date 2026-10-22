// Decode only serial cable bits, then compare with the mounted raw image.
// The selected sector is physical (3,0,16), Pilot germ's virtual (3,20).
#include "Vtb_pack_write_window.h"
#include <verilated.h>
#include "hps_disk_test.h"
#include <cstdio>
#include <cstdlib>
int main(int argc,char**argv){
 if(argc!=4)return 2;
 const bool window=std::atoi(argv[2]);
 const std::string scenario=argv[3];
 if(scenario!="normal"&&scenario!="readonly"&&scenario!="remount"&&scenario!="timeout"&&scenario!="late-readonly")return 2;
 VerilatedContext ctx;ctx.commandArgs(argc,argv);Vtb_pack_write_window top(&ctx);HpsDiskTest disk(argv[1]);
 // Make the two extra label words distinctive in the in-memory fixture.
 // Neither the source pack nor the production boot medium is modified.
 const uint64_t extra_label=uint64_t((3*5*29+16)*269+3+8)*2;
 for(unsigned i=0;i<4;i++)disk.media.at(extra_label+i)=uint8_t(0x24681357u>>(8*i));
 const auto original=disk.media;
 std::vector<unsigned char> cable;
 std::vector<unsigned> payload_offsets;
 top.clk_host=top.sys_clk=0;top.reset=1;top.attached=top.data_adv=0;
 top.writing=top.write_bit=top.readonly=top.remounted=top.seek=top.seek_cyl=0;top.pulse=68;
 top.sd_ack=top.sd_buff_wr=0;top.sd_buff_addr=top.sd_buff_dout=0;top.eval();
 unsigned record=0,bits=0,word=0,crc=0,shift=0,steps=0,words=0;
 const unsigned lengths[]={2,10,256},bases[]={1,3,13};
 bool syncing=true;
 unsigned host=0;bool expect_fault=false,stall_write=false;
 struct Request{unsigned lba,blocks;};std::vector<Request> write_requests;
 auto host_before=[&](){
  if(!disk.active&&!disk.closing&&top.sd_wr)
   write_requests.push_back({top.sd_lba,unsigned(top.sd_blk_cnt)+1});
  if(stall_write&&top.sd_wr){top.sd_ack=top.sd_buff_wr=0;return;}
  disk.before(top);
 };
 auto tick=[&](){
  top.clk_host=top.sys_clk=0;top.eval();top.reset=host<4;top.attached=host>=5;
  top.data_adv=steps%8==6;host_before();top.eval();bool run=top.pack_ready&&!top.reset;
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
 if(scenario=="readonly"){top.readonly=1;expect_fault=true;}
 // Restart this sector's physical waveform, then send the changed records
 // into its differential write input, including all three Fire codes.
 top.pulse=64;tick();top.pulse=68;tick();top.writing=1;
 unsigned sent=0;
 while(sent<cable.size() && host<400000){
  top.write_bit=cable[sent];if(tick())sent++;
  if(top.pack_fault)break;
 }
 if(scenario=="readonly") {
  if(!top.pack_fault||top.pack_dirty||(top.fault_debug>>28)!=3||disk.writes||disk.media!=original)throw std::runtime_error("readonly changed media or missed reason3");
  puts("WINDOW-READONLY PASS: serial write rejected, full image unchanged");top.final();return 0;
 }
 if(sent!=cable.size())throw std::runtime_error("incomplete Pilot write stream");
 if(scenario=="remount") {
  expect_fault=true;top.remounted=1;
  for(unsigned i=0;i<20;i++)tick();
  if(!top.pack_fault||!top.pack_dirty||(top.fault_debug>>28)!=5||disk.writes||disk.media!=original)throw std::runtime_error("dirty remount wrote media or missed reason5");
  puts("WINDOW-REMOUNT PASS: dirty remount rejected, full image unchanged");top.final();return 0;
 }
 if(scenario=="timeout"){stall_write=true;expect_fault=true;}
 if(scenario=="late-readonly"){top.readonly=1;expect_fault=true;}
 top.writing=0;
 while(host<500000){
  tick();
  if(expect_fault&&top.pack_fault)break;
  if(disk.writes && !disk.active && top.pack_ready && !top.sd_wr)break;
 }
 if(scenario=="late-readonly"){
  if(!top.pack_fault||!top.pack_dirty||(top.fault_debug>>28)!=6||disk.writes||disk.media!=original)throw std::runtime_error("late readonly lost dirty data or wrote host");
  puts("WINDOW-LATE-READONLY PASS: host write rejected, dirty retained, image unchanged");top.final();return 0;
 }
 if(scenario=="timeout"){
  if(!top.pack_fault||!top.pack_dirty||(top.fault_debug>>28)!=8||disk.writes||disk.media!=original)throw std::runtime_error("missing timeout/no-write guarantee");
  puts("WINDOW-TIMEOUT PASS: absent host not completed, full image unchanged");top.final();return 0;
 }
 if(top.pack_dirty||disk.writes!=1 || disk.active || !top.pack_ready)throw std::runtime_error("Pilot HPS writeback did not finish");
 if(disk.media!=expected)throw std::runtime_error("Pilot writeback lost changed words or damaged neighboring bytes");
 const unsigned track_word=3*5*29*269,first_word=track_word%256+16*269+1,last_word=first_word+267;
 const unsigned expected_lba=track_word/256+(window?first_word/256:0);
 const unsigned expected_blocks=window?(last_word/256-first_word/256+1):((track_word%256+29*269+255)/256);
 if(write_requests.size()!=1||write_requests[0].lba!=expected_lba||write_requests[0].blocks!=expected_blocks)
  throw std::runtime_error("wrong write window LBA/count (old whole-track control must fail window expectation)");
 printf("WINDOW-RANGE PASS: mode=%u lba=%u blocks=%u, first buffer block=%u\n",window,expected_lba,expected_blocks,first_word/256);
 printf("PILOT-PACK PASS: header 2, label 10, data 256 words; three Fire codes; changed labels/data persisted through HPS; neighboring bytes intact\n");
 // A later generation must reset the range after the preceding ACK. Then
 // two separated complete sectors under one gate must accumulate one range.
 auto encode=[&](unsigned sector,unsigned records){
  auto result=cable;
  for(unsigned r=0;r<records;r++){
   uint32_t check=0;
   auto divide=[&](unsigned bit){check=(check<<1)^((bit^(check>>31))?0x00a00805u:0u);};
   for(int b=15;b>=0;b--)divide((0201>>b)&1);
   for(unsigned w=0;w<lengths[r];w++){
    uint64_t at=uint64_t((3*5*29+sector)*269+bases[r]+w)*2;
    unsigned value=expected[at]|(unsigned(expected[at+1])<<8);
    for(unsigned b=0;b<16;b++){
     unsigned bit=(value>>(15-b))&1;result[payload_offsets[r]+w*16+b]=bit;divide(bit);
    }
   }
   for(unsigned b=0;b<32;b++)result[payload_offsets[r]+lengths[r]*16+b]=(check>>(31-b))&1;
  }
  result.resize(payload_offsets[records-1]+lengths[records-1]*16+32);
  return result;
 };
 auto send_sector=[&](unsigned sector,const std::vector<unsigned char>& wave){
  top.pulse=sector*4;tick();top.pulse=((sector+1)%29)*4;tick();
  unsigned pos=0;while(pos<wave.size()&&host<800000){top.write_bit=wave[pos];if(tick())pos++;}
  if(pos!=wave.size())throw std::runtime_error("incomplete additional write");
 };
 auto finish_write=[&](unsigned n,unsigned first,unsigned last){
  top.writing=0;
  while(host<900000){tick();if(disk.writes==n&&!disk.active&&top.pack_ready&&!top.sd_wr)break;}
  if(top.pack_dirty||disk.writes!=n||disk.active||!top.pack_ready||disk.media!=expected)throw std::runtime_error("later write lost changed bytes/neighbors");
  unsigned lba=track_word/256+(window?first/256:0);
  unsigned blocks=window?last/256-first/256+1:((track_word%256+29*269+255)/256);
  if(write_requests.size()!=n||write_requests.back().lba!=lba||write_requests.back().blocks!=blocks)throw std::runtime_error("later range reset/accumulation incorrect");
 };
 expected[uint64_t((3*5*29+16)*269+1)*2]^=1;
 top.writing=1;send_sector(16,encode(16,1));finish_write(2,first_word,first_word+1);
 for(unsigned sector:{16u,20u})expected[uint64_t((3*5*29+sector)*269+13+255)*2]^=0x5a;
 top.writing=1;send_sector(16,encode(16,3));send_sector(20,encode(20,3));
 finish_write(3,first_word,track_word%256+20*269+268);
 expected[uint64_t((3*5*29+28)*269+268)*2]^=0xa5;
 top.writing=1;send_sector(28,encode(28,3));
 finish_write(4,track_word%256+28*269+1,track_word%256+29*269-1);
 puts("WINDOW-TRACK-END PASS: sector28 wraps physical pulse0, last partial host block preserves following track");
 puts("WINDOW-GENERATIONS PASS: second ACK resets range, separated sectors coalesce, full-image oracle passes");
 // A window offset must not leak into the next full-track read port.
 auto seek=[&](unsigned cylinder,unsigned reads){
  top.seek_cyl=cylinder;top.seek=1;
  do{tick();}while(host<1000000&&(disk.reads<reads||disk.active||!top.pack_ready));
  if(disk.reads!=reads||disk.active||!top.pack_ready)throw std::runtime_error("new track read did not finish");
  top.seek=0;tick();tick();
 };
 seek(4,2);
 top.pulse=64;tick();top.pulse=68;tick();
 record=bits=word=crc=shift=words=0;syncing=true;
 while(record<3&&host<1100000)if(tick()){
  unsigned bit=top.media_bit;shift=((shift<<1)|bit)&65535;
  if(syncing){if(shift==0201){syncing=false;bits=word=crc=0;
   for(int b=15;b>=0;b--)crc=(crc<<1)^((((0201>>b)&1)^(crc>>31))?0x00a00805u:0u);
  }}else{
   crc=(crc<<1)^((bit^(crc>>31))?0x00a00805u:0u);bits++;
   if(bits%16==0&&bits<=lengths[record]*16){
    uint64_t at=uint64_t((4*5*29+16)*269+bases[record]+word)*2;
    unsigned want=expected[at]|(unsigned(expected[at+1])<<8);
    if(shift!=want)throw std::runtime_error("new track serial payload differs from full-image oracle");
    word++;words++;
   }
   if(bits==lengths[record]*16+32){if(crc)throw std::runtime_error("new track serial Fire code");record++;syncing=true;shift=0;}
  }
 }
 if(record!=3||words!=268||disk.media!=expected)throw std::runtime_error("new track serial read incomplete");
 seek(3,3);
 puts("WINDOW-READBACK PASS: new full track after window write has exact independent serial words/Fire codes");
 // A damaged controller Fire code must stop the medium and retain its
 // first-fault reason/residue; observation must not invent a successful write.
 expect_fault=true;top.pulse=64;tick();top.pulse=68;tick();top.writing=1;
 cable[payload_offsets[0]+lengths[0]*16]^=1;
 sent=0;
 while(!top.pack_fault && sent<cable.size()){
  top.write_bit=cable[sent];if(tick())sent++;
 }
 if(!top.pack_fault || top.pack_dirty || (top.fault_debug>>28)!=4 || !top.fault_extra)
  throw std::runtime_error("missing first bad-CRC diagnostic");
 auto debug=top.fault_debug,extra=top.fault_extra;
 top.writing=0;for(unsigned i=0;i<100;i++)tick();
 if(top.fault_debug!=debug || top.fault_extra!=extra || disk.media!=expected)
  throw std::runtime_error("first-fault evidence changed or bad block persisted");
 printf("PACK-FAULT PASS: damaged Fire code stops, reason4/residue retained, no host write invented\n");
 top.final();
}
