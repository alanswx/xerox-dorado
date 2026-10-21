// Independent captured-request/image check for the simulation packet adapter.
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
extern "C" {
int native_host_open(const char*,const char*,const char*,const char*,const char*);
void native_net_tx_word(unsigned,unsigned);
void native_net_tx(unsigned);
void native_net_tick(unsigned long long);
unsigned native_net_rx();
unsigned native_net_rx_word(unsigned);
unsigned native_boot_request();
}
int main(int argc,char**argv){
 assert(argc==6);assert(native_host_open(argv[1],argv[2],argv[3],argv[4],argv[5]));
 // Actual original Cedar request captured on MiSTer14, including nil checksum.
 const uint16_t request[]={000042,001000,000026,000264,000001,000113,000000,000000,000004,000042,000001,000001,0177777};
 for(unsigned i=0;i<13;i++)native_net_tx_word(i,request[i]);native_net_tx(13);
 assert(native_boot_request()==0113);
 std::ifstream f(argv[3],std::ios::binary);
 std::vector<unsigned char> expected((std::istreambuf_iterator<char>(f)),{}),actual;
 assert(expected.size()>512);expected.erase(expected.begin(),expected.begin()+512);
 unsigned sequence=0,frames=0;bool ended=false;
 for(unsigned tick=1;tick<1000000 && !ended;tick++){
  native_net_tick(uint64_t(tick)*32000);unsigned n=native_net_rx();if(!n)continue;
  assert(n>=13 && native_net_rx_word(3)==0265 && native_net_rx_word(5)==sequence);
  assert(native_net_rx_word(n-1)==0xffff);
  for(unsigned i=12;i+1<n;i++){unsigned word=native_net_rx_word(i);actual.push_back(word>>8);actual.push_back(word&255);}
  frames++;sequence++;ended=n==13;
 }
 assert(ended && actual==expected);
 printf("NATIVE_HOST_CAPTURED_CEDAR_PASS frames=%u payload_bytes=%zu exact_original_image=1\n",frames,actual.size());
}
