// Fake only Main's SPI boundary. The peer is the real standalone Pup server.
#include "../dorado_net.h"
#include "spi.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <unistd.h>
const char *test_core_name="DoradoNetwork_20261017";
const char *test_original_core_name="Dorado";
static std::vector<uint16_t> request={042,01000,026,0264,1,0110,0,0,4,042,1,1,0177777};
static std::vector<uint16_t> incoming,received;
static unsigned command,word_index,acks,reads,frames,length,transactions;
static bool large_frame=false;
static bool pending=true,ready=false,ended=false;
static void check(bool ok,const char*what){if(!ok)throw std::runtime_error(what);}
uint16_t spi_uio_cmd_cont(uint16_t c){
 transactions++;
 command=c;word_index=0;incoming.clear();
 switch(c){case 0x70:return 0xd074;case 0x71:reads++;return request.size();
 case 0x72:check(pending,"duplicate TX acknowledgement");pending=false;acks++;return 0xd074;
 case 0x73:return ready;default:throw std::runtime_error("unexpected command");}
}
uint16_t spi_w(uint16_t w){
 if(command==0x70){word_index++;return word_index==1 ? (pending?2:0)|(ready?1:0)|(large_frame?4:0) : request.size();}
 if(command==0x71){check(word_index<request.size(),"TX read overrun");return request[word_index++];}
 check(command==0x73 && ready,"RX write while busy");
 if(word_index++==0)length=w;else incoming.push_back(w);
 return 0;
}
void DisableIO(){
 if(command==0x73){
  check(incoming.size()==length && length>=13,"partial packet submitted");
  if(large_frame){check(incoming==request,"large frame changed across Main");received=incoming;frames++;ended=true;command=0;return;}
  if(incoming[1]==0602 || (incoming[1]==01000 && incoming[3]==0201)){
   command=0;return; // legitimate idle boot/routing broadcasts
  }
  check(incoming[1]==01000 && incoming[3]==0265,"unexpected server packet");
  check(incoming[5]==frames,"packet dropped, duplicated or reordered");
  check(length*2==incoming[2]+4 && incoming.back()==0177777,"wrong wire packet boundary");
  frames++;if(length==13)ended=true;
  received.insert(received.end(),incoming.begin()+12,incoming.end()-1);
 }
 command=0;
}
uint16_t spi_uio_cmd(uint16_t c){auto r=spi_uio_cmd_cont(c);DisableIO();return r;}
int main(int argc,char**argv){try{
 check(argc==2,"usage: main-transport-test AEMU.eb");
 large_frame=getenv("DORADO_TEST_LARGE")!=nullptr;
 std::vector<uint16_t> expected;
 if(large_frame){
  request.resize(1024);for(unsigned i=0;i<request.size();i++)request[i]=(i*1511)^0xa55a;
  expected=request;
 }else{
  std::ifstream f(argv[1],std::ios::binary);check(bool(f),"missing microcode fixture");f.seekg(512);
  for(int hi;(hi=f.get())!=EOF;){int lo=f.get();check(lo!=EOF,"odd file");expected.push_back((hi<<8)|lo);}
 }
 for(unsigned i=0;i<10000&&!ended;i++){
  ready=i>=100;dorado_net_poll();usleep(500);
 }
 check(ended && received==expected,"Main did not deliver the byte-exact microcode stream");
 check(acks==1 && reads==1,"TX was acknowledged before acceptance or reread unnecessarily");
 if(large_frame){dorado_net_reset();printf("MAIN-LARGE PASS: 1024 words each direction, held RX backpressure, explicit TX ack\n");return 0;}
 dorado_net_reset();test_core_name="Dorado";test_original_core_name="MENU";
 unsigned previous_transactions=transactions;dorado_net_poll();
 check(transactions==previous_transactions,"launcher name caused another core to be probed");
 // Let the server discover the closed socket during an idle broadcast.
 usleep(2200000);test_core_name="DoradoNetwork_20261017";test_original_core_name="Dorado";pending=true;ended=false;frames=0;received.clear();
 for(unsigned i=0;i<10000&&!ended;i++){dorado_net_poll();usleep(500);}
 check(ended && received==expected && acks==2 && reads==2,"core reload/reconnect failed");
 dorado_net_reset();
 printf("MAIN-TRANSPORT PASS: %zu microcode bytes, %u frames, held RX backpressure, explicit TX ack, core-change reconnect\n",received.size()*2,frames);
 return 0;
}catch(const std::exception&e){fprintf(stderr,"MAIN-TRANSPORT FAIL: %s\n",e.what());return 1;}}
