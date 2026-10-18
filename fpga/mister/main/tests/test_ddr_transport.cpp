// Actual Main manager/client and daemon control service. Only physical SPI,
// DDR mapping and packet-engine latency are replaced with guarded oracles.
#include "../dorado_ddr.h"
extern "C" {
#include "dorado_ddr_control.h"
}
#include "spi.h"
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <sys/mman.h>
#include <unistd.h>

const char *test_core_name="Dorado DDR test",*test_original_core_name="Dorado";
static void check(bool yes,const char *why){if(!yes){fprintf(stderr,"DDR MANAGER FAIL: %s\n",why);std::_Exit(23);}}
static uint64_t ms(){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();}
static std::atomic<bool> capable{true},enabled{false},initialized{false},daemon_stopped{true};
static std::atomic<unsigned> fault_code{0},close_delay{0},drain_delay{0},opens{0},closes{0};
static bool stuck=false,bad_abi=false;
static uint64_t cookie=0,drain_until=0;
static unsigned starts=0,stops=0,command=0,position=0;
static uint16_t reply[18]={},control_words[5]={};
static unsigned char *memory;
static void put32(unsigned char *p,uint32_t n){for(unsigned i=0;i<4;i++)p[i]=uint8_t(n>>(8*i));}
static void put64(unsigned char *p,uint64_t n){put32(p,uint32_t(n));put32(p+4,uint32_t(n>>32));}
static bool stopped(){return !enabled && !stuck && ms()>=drain_until;}
uint16_t spi_uio_cmd_cont(uint16_t c){
    check(!command,"nested SPI");command=c;position=0;
    if(c==0x70)return 0xd074;
    if(c==0x75)return 0xd0d4;
    check(c==0x74 && capable,"unexpected packet command/capability access");
    memset(reply,0,sizeof reply);reply[1]=bad_abi?2:1;reply[3]=0x1ff0;reply[5]=1;
    reply[6]=uint16_t(stopped()|(initialized?2:0)|(enabled?4:0)|(fault_code?8:0)|((enabled||!stopped())?16:0));
    reply[7]=uint16_t(fault_code);reply[13]=2048;reply[14]=8;reply[15]=16;
    for(unsigned i=0;i<4;i++)reply[9+i]=uint16_t(cookie>>(16*i));
    return 0xd0d4;
}
uint16_t spi_w(uint16_t w){
    check(command,"SPI without select");
    if(command==0x70)return ++position==1?uint16_t(capable?12:4):0;
    if(command==0x74){check(position<17,"capability overread");return reply[++position];}
    check(command==0x75 && position<5,"overlong control");control_words[position++]=w;return 0;
}
void DisableIO(){
    if(command==0x75){
        check(position==5,"partial command");
        if(!control_words[0]){
            check(daemon_stopped,"FPGA stopped before daemon STOP ACK/cleanup");
            if(enabled){drain_until=ms()+drain_delay;enabled=false;}
            initialized=false;stops++;
        }else{
            check(stopped() && daemon_stopped,"grant before drain");
            uint64_t n=0;for(unsigned i=0;i<4;i++)n|=uint64_t(control_words[i+1])<<(16*i);
            check(n && n!=cookie,"reused epoch");cookie=n;starts++;
            check(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"initialize mapping");
            memset(memory,0,65536);put64(memory,DORADO_DDR_MAGIC);put64(memory+8,cookie);
            put32(memory+0x30,1);put64(memory+0x38,DORADO_DDR_ABI);
            enabled=true;initialized=true;
        }
    }
    command=0;
}
uint16_t spi_uio_cmd(uint16_t c){uint16_t r=spi_uio_cmd_cont(c);DisableIO();return r;}
static int map(void *,volatile void **p){
    check(enabled && initialized,"daemon maps before FPGA initialization");
    *p=memory;daemon_stopped=false;opens++;return 0;
}
static int unmap(void *opaque,volatile void *p){
    auto *service=static_cast<dorado_ddr_control *>(opaque);
    check(p==memory && !service->granted && !service->ring.active && !service->ring.memory,"cleanup before quiesce");
    std::this_thread::sleep_for(std::chrono::milliseconds(close_delay.load()));
    check(!mprotect(memory,65536,PROT_NONE),"protect stopped memory");
    daemon_stopped=true;closes++;return 0;
}
template<class P> static void until(P predicate,const char *why){
    uint64_t limit=ms()+2000;
    while(!predicate()){check(ms()<limit,why);std::this_thread::sleep_for(std::chrono::milliseconds(1));}
}
#ifdef DORADO_TEST_LIFECYCLE
static unsigned lifecycle_mutations=0;
static bool disk_safe=true;
bool dorado_net_barrier(const char *reason){return dorado_ddr_barrier(reason,10);}
bool dorado_disk_barrier(const char *){return disk_safe;}
bool dorado_disk_before_mount(unsigned){return disk_safe;}
bool dorado_net_reset(){return dorado_ddr_reset();}
#include "lifecycle_hooks.inc"
#endif
int main(){
    char directory[]="/tmp/dorado-ddr-main-XXXXXX";check(mkdtemp(directory),"temporary directory");
    char path[108];snprintf(path,sizeof path,"%s/control",directory);
    setenv("DORADO_NET_CONTROL",path,1);setenv("DORADO_NET_DDR","1",1);
    memory=static_cast<unsigned char *>(mmap(nullptr,65536,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
    check(memory!=MAP_FAILED,"guard mapping");
    capable=false;check(!dorado_ddr_poll(4),"legacy core denied fallback");
    capable=true;unsetenv("DORADO_NET_DDR");check(!dorado_ddr_poll(12),"disabled fresh core denied SPI");
    setenv("DORADO_NET_DDR","1",1);
    dorado_ddr_control service={};dorado_ddr_mapper mapper={map,unmap,&service};
    check(!dorado_ddr_control_open(&service,path,mapper),"open actual service");
    std::atomic<bool> running{true};std::thread worker([&]{while(running){dorado_ddr_control_poll(&service);usleep(100);}
        check(!dorado_ddr_control_close(&service),"close service");});
    auto activate=[&]{unsigned wanted=opens+1;
        until([&]{check(dorado_ddr_poll(12),"SPI fallback during acquisition");return opens>=wanted;},"grant timeout");
        for(unsigned i=0;i<10;i++){check(dorado_ddr_poll(12),"SPI fallback after grant");usleep(1000);}
        check(starts==opens && dorado_ddr_needs_barrier(),"grant count/ownership");};
    activate();
    // Let a status exchange complete; inactive/mismatched replies must never
    // silently switch a live FPGA to SPI.
    usleep(1010000);for(unsigned i=0;i<10;i++){check(dorado_ddr_poll(12),"status fallback");usleep(1000);}
    unsigned before=stops;close_delay=150;
#ifdef DORADO_TEST_LIFECYCLE
    check(fpga_load_rbf("test",nullptr,nullptr)==-1,"actual loader ignored failed STOP");
    reboot(0);app_restart(nullptr,nullptr,nullptr);user_io_init(nullptr,nullptr);
    check(!lifecycle_mutations,"actual lifecycle passed a late daemon ACK");
#endif
    check(!dorado_ddr_barrier("delayed daemon STOP",10),"late ACK allowed core programming");
    check(stops==before && dorado_ddr_needs_barrier(),"FPGA stopped before late ACK");
    check(!dorado_ddr_reset(),"reset discarded active owner");
    check(dorado_ddr_barrier("retry delayed STOP",1000),"retry failed");
    check(!dorado_ddr_needs_barrier() && dorado_ddr_poll(12),"quiesced transport restarted before init");
    check(dorado_ddr_reset(),"reset after barrier");close_delay=0;
    activate();drain_delay=120;
    check(!dorado_ddr_barrier("slow FPGA drain",10),"undrained FPGA allowed core programming");
    check(dorado_ddr_needs_barrier() && !dorado_ddr_reset(),"lost undrained ownership");
    check(dorado_ddr_barrier("retry FPGA drain",1000),"drain retry failed");
    drain_delay=0;check(dorado_ddr_reset(),"reset second grant");
    // A replacement Main has no manager state but must still stop an inherited
    // FPGA lease, even if its environment asks for SPI.
    unsetenv("DORADO_NET_DDR");enabled=true;initialized=true;cookie++;
    until([&]{return !dorado_ddr_poll(12);},"disabled transport did not drain inherited lease");
    check(stopped() && !dorado_ddr_needs_barrier(),"inherited FPGA still running");
    check(dorado_ddr_reset(),"reset disabled transport");
    enabled=true;initialized=true;cookie++;
    check(dorado_ddr_barrier("new Main before first poll"),"startup barrier ignored inherited engine");
    check(stopped() && dorado_ddr_reset(),"startup barrier did not drain");
    setenv("DORADO_NET_DDR","1",1);activate();fault_code=7;
    usleep(1010000);check(dorado_ddr_poll(12),"fault allowed fallback");
    check(!dorado_ddr_reset(),"fault discarded ownership");
    check(dorado_ddr_barrier("fault shutdown"),"fault prevented safe drain");fault_code=0;
    check(dorado_ddr_reset(),"reset fault transport");
    bad_abi=true;check(dorado_ddr_poll(12),"invalid ABI allowed ownership");
    check(!dorado_ddr_barrier("invalid ABI"),"unverifiable state allowed programming");
    bad_abi=false;check(dorado_ddr_barrier("recovered status") && dorado_ddr_reset(),"recover barrier");
#ifdef DORADO_TEST_LIFECYCLE
    disk_safe=false;
    check(fpga_load_rbf("test",nullptr,nullptr)==-1,"loader ignored disk failure");
    reboot(0);app_restart(nullptr,nullptr,nullptr);user_io_init(nullptr,nullptr);
    check(!lifecycle_mutations,"lifecycle ignored disk failure");disk_safe=true;
    check(fpga_load_rbf("test",nullptr,nullptr)==0,"safe loader blocked");
    reboot(0);app_restart(nullptr,nullptr,nullptr);user_io_init(nullptr,nullptr);
    check(lifecycle_mutations==4,"safe lifecycle action count");
#endif
    setenv("DORADO_NET_CONTROL","/tmp/dorado-ddr-manager-no-such-service/control",1);
    enabled=true;initialized=true;cookie++;
    before=stops;check(dorado_ddr_poll(12),"absent daemon allowed fallback");
    check(!dorado_ddr_barrier("missing controller",10) && stops==before,"missing controller granted STOP");
    setenv("DORADO_NET_CONTROL",path,1);
    check(dorado_ddr_barrier("reconnected controller") && dorado_ddr_reset(),"reconnect drain");
    running=false;worker.join();check(!rmdir(directory) && !munmap(memory,65536),"fixture cleanup");
    printf("DDR MANAGER PASS: actual daemon ACK/grant ordering, guarded mapping, delayed STOP refusal/retry, physical drain, inherited leases, status/fault/ABI, missing daemon, legacy fallback; grants=%u stops=%u\n",starts,stops);
}
