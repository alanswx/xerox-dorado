#include "Vsnapshot_host_top.h"
#include "verilated.h"
#include "dorado_snapshot_client.h"
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

const char *test_core_name="MGL display override";
const char *test_original_core_name="Dorado";
static Vsnapshot_host_top d;
static unsigned checks=0,transactions=0,word_index=0,max_words=0;
static bool selected=false,absent=false;
static int corrupt_word=-1;static uint16_t corrupt_xor=0;
static const auto spi_owner=std::this_thread::get_id();
static constexpr uint64_t BUILD=0xb49c378fd137862aULL,SCHEMA=0xaf0b647cb43a3a62ULL;
using Client=DoradoSnapshotClient;using Result=Client::Result;using Phase=Client::Phase;
static std::vector<uint16_t> frame;
static void check(bool ok,const char *message){++checks;if(!ok)throw std::runtime_error(message);}
static void tick(){d.clk=0;d.eval();d.clk=1;d.eval();d.clk=0;d.eval();}
static uint16_t transfer(uint16_t value){
 check(std::this_thread::get_id()==spi_owner,"worker used SPI");
 d.din=value;d.strobe=1;tick();d.strobe=0;tick();frame.push_back(value);
 uint16_t output=d.dout;
 if(int(word_index)==corrupt_word)output^=corrupt_xor;
 ++word_index;return absent?0:output;
}
uint16_t spi_uio_cmd_cont(uint16_t command){
 check(!selected,"nested SPI transaction");selected=true;word_index=0;frame.clear();
 d.enable=1;return transfer(command);
}
uint16_t spi_w(uint16_t value){check(selected,"SPI word without select");return transfer(value);}
void DisableIO(){
 check(selected,"SPI deselect without select");selected=false;d.enable=0;
 tick();tick();tick();++transactions;if(word_index>max_words)max_words=word_index;
}
uint16_t spi_uio_cmd(uint16_t command){auto result=spi_uio_cmd_cont(command);DisableIO();return result;}
static void reseed(){
 d.enable=0;d.strobe=0;d.machine_reseed=1;d.transport_reset=0;d.set_hold=0;
 d.guest_held=0;d.global_hold=0;d.board_busy=0;d.board_fault=0;d.local_available=7;d.local_held=0;
 tick();tick();d.machine_reseed=0;tick();check(d.phase==0&&!d.host_hold,"RBF init differs");
}
static void hold(Client &client){
 check(client.request_hold()==Result::Done,"Main board hold request failed");
 check(d.command_hold,"actual board UIO did not latch Main hold");
 d.global_hold=1;d.guest_held=1;tick();tick();
}
static Client::Status status(Client &client){
 Client::Status s;check(client.read_status(s)==Result::Done,"Main failed to decode actual RTL status");
 check(!selected,"status leaked SPI selection");return s;
}
int main(int argc,char **argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.eval();reseed();Client client(BUILD,SCHEMA,Client::main_io());
  auto s=status(client);check(s.phase==Phase::Idle&&s.build==BUILD&&s.schema==SCHEMA&&!s.epoch,"identity decoded incorrectly");
  auto before=transactions;test_original_core_name="MacPlus";
  check(client.read_status(s)==Result::Unavailable&&client.begin(1)==Result::Unavailable&&transactions==before,"other core touched SPI");
  test_original_core_name="Dorado";absent=true;
  check(client.read_status(s)==Result::Unavailable&&!selected&&word_index==1,"old-core probe did not stop at capability");
  check(client.begin(1)==Result::Unavailable&&!selected&&word_index==1,"old-core command sent payload");absent=false;
  before=transactions;check(client.begin(0)==Result::Invalid&&client.grant(1,32)==Result::Invalid&&transactions==before,"invalid caller argument used SPI");
  Client wrong(BUILD^1,SCHEMA,Client::main_io());check(wrong.read_status(s)==Result::Incompatible,"mismatched build accepted");
  hold(client);check(client.begin(1)==Result::Done,"Begin transmit failed");
  check(frame.size()==16&&frame[0]==0x7b&&frame[1]==0xc04d&&frame[2]==1&&frame[11]==1&&frame[15]==0,"Main action framing differs");
  s=status(client);check(s.phase==Phase::Held&&s.epoch==1&&s.host_hold()&&s.command_hold()&&s.guest_held()&&s.global_hold(),"Begin was not confirmed");
  auto errors=s.errors;client.grant(1,3);s=status(client);
  check(s.errors==errors+1&&s.services==0,"unacknowledged grant silently accepted");
  d.local_held=7;tick();client.grant(1,3);s=status(client);check(s.services==3,"actual local ACK did not permit grant");
  errors=s.errors;client.grant(2,0);s=status(client);check(s.services==3&&s.errors==errors+1,"stale nonce changed grants");
  errors=s.errors;client.grant(1,31);s=status(client);check(s.services==3&&s.errors==errors+1,"unavailable domains granted");
  errors=s.errors;wrong.grant(1,0);s=status(client);check(s.services==3&&s.errors==errors+1,"wrong-build packet altered state");
  for(auto mutation:std::vector<std::pair<int,uint16_t>>{{1,1},{2,4},{3,1},{4,1},{8,1},{16,32},{17,32},{18,32},{19,64}}){
   corrupt_word=mutation.first;corrupt_xor=mutation.second;Client::Status sentinel;sentinel.epoch=12345;
   auto result=client.read_status(sentinel);
   check((result==Result::Incompatible||result==Result::Malformed)&&sentinel.epoch==12345&&!selected,"malformed status published authority");
  }
  corrupt_word=-1;corrupt_xor=0;
  client.release(1);s=status(client);check(s.phase==Phase::Releasing&&s.services==0&&!s.host_hold()&&!s.command_hold()&&s.global_hold(),"native release did not remain staged");
  errors=s.errors;client.finish(1);s=status(client);check(s.phase==Phase::Releasing&&s.errors==errors+1,"early Finish accepted");
  d.global_hold=0;d.guest_held=0;tick();client.finish(1);s=status(client);check(s.phase==Phase::Idle&&s.epoch==1,"completed release not confirmed");
  hold(client);errors=s.errors;client.begin(1);s=status(client);check(s.phase==Phase::Idle&&s.errors==errors+1,"nonce was reused");
  client.begin(2);s=status(client);check(s.phase==Phase::Held&&s.epoch==2,"next snapshot lease failed");
  client.grant(2,1);d.local_held=6;tick();s=status(client);check(s.phase==Phase::Fault&&s.host_hold()&&!s.services,"local ACK loss did not quarantine");
  d.transport_reset=1;tick();d.transport_reset=0;tick();s=status(client);check(s.phase==Phase::Fault&&s.epoch==2,"parser reset cleared active lease");
  reseed();hold(client);client.begin(3);client.release(3);d.global_hold=0;d.guest_held=0;tick();
  d.board_busy=1;client.quarantine(3);s=status(client);check(s.phase==Phase::Fault&&s.host_hold()&&s.command_hold(),"Main could not quarantine after release");
  d.board_fault=1;s=status(client);check(s.phase==Phase::Fault&&s.board_fault(),"fault status unavailable");
  check(max_words==22&&!selected,"unbounded or unfinished SPI transaction");
  std::printf("MAIN SNAPSHOT CLIENT RTL PASS checks=%u transactions=%u max_words=%u actual_main_adapter=1 actual_service_rtl=1\n",checks,transactions,max_words);return 0;
 }catch(const std::exception &e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
