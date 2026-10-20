// Actual Main network/manager/client and C control service; SPI is a strict
// Main-thread spy, and the real mapped ring becomes inaccessible while paused.
#include <thread>
#include <vector>
#include <algorithm>
#include "dorado_net.h"
static bool forbid_spi=false;
static unsigned snapshot_spi_calls=0;
static std::thread::id spi_owner;
#define main legacy_ddr_main
#include "legacy_helpers.cpp"
#undef main
#include "dorado_net.h"
bool dorado_disk_prepare_exit(){return true;}
using Result=DoradoDDRSnapshotResult;

struct Fixture {
    char directory[64]="/tmp/dorado-main-snapshot-XXXXXX";
    dorado_ddr_control service={};
    std::mutex lock;
    std::atomic<bool> running{true},serving{true};
    std::atomic<unsigned> events{0};
    std::thread worker;
    uint64_t epoch=0;
    Fixture(){
        spi_owner=std::this_thread::get_id();
        check(mkdtemp(directory),"snapshot temporary directory");
        char path[108];snprintf(path,sizeof path,"%s/control",directory);
        setenv("DORADO_NET_CONTROL",path,1);setenv("DORADO_NET_DDR","1",1);
        memory=static_cast<unsigned char *>(mmap(nullptr,65536,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
        check(memory!=MAP_FAILED,"snapshot guard mapping");
        dorado_ddr_mapper mapper={map,unmap,&service};
        check(!dorado_ddr_control_open(&service,path,mapper),"snapshot open actual service");
        worker=std::thread([&]{
            while(running){
                if(serving){std::lock_guard<std::mutex> guard(lock);events|=dorado_ddr_control_poll(&service);}
                usleep(100);
            }
            std::lock_guard<std::mutex> guard(lock);check(!dorado_ddr_control_close(&service),"snapshot service cleanup");
        });
        until([&]{dorado_net_poll();epoch=dorado_ddr_snapshot_epoch();return epoch!=0;},"snapshot acquire timeout");
        check(opens==1 && starts==1 && dorado_ddr_needs_barrier(),"snapshot initial ownership");
    }
    ~Fixture(){
        running=false;worker.join();check(!munmap(memory,65536) && !rmdir(directory),"snapshot fixture cleanup");
    }
    bool paused(){std::lock_guard<std::mutex> guard(lock);return service.granted && service.ring.active && service.ring.paused;}
    void protect(int flags){std::lock_guard<std::mutex> guard(lock);check(!mprotect(memory,65536,flags),"snapshot mprotect");}
    void begin(){
        check(dorado_ddr_snapshot_pause(epoch)==Result::Wait,"snapshot pause did not start");forbid_spi=true;
        until([&]{dorado_net_poll();return dorado_ddr_snapshot_pause(epoch)==Result::Done;},"snapshot pause ACK timeout");
        check(paused() && dorado_ddr_snapshot_pending(),"snapshot PAUSE did not preserve active ring");
    }
    void assert_held(){
        const unsigned before=snapshot_spi_calls;
        check(!dorado_net_barrier("snapshot lifecycle test") && !dorado_net_reset(),"snapshot allowed lifecycle mutation");
        check(dorado_ddr_snapshot_pending() && dorado_ddr_needs_barrier() && before==snapshot_spi_calls,
              "snapshot discarded ownership or probed frozen FPGA");
#ifdef DORADO_TEST_LIFECYCLE
        const unsigned mutations=lifecycle_mutations;
        check(fpga_load_rbf("snapshot",nullptr,nullptr)==-1,"snapshot allowed actual loader prologue");
        reboot(0);app_restart(nullptr,nullptr,nullptr);user_io_init(nullptr,nullptr);
        check(lifecycle_mutations==mutations,"snapshot allowed actual lifecycle mutation");
#endif
    }
};
static void normal(){
    Fixture f;const uint64_t epoch=f.epoch;
    check(dorado_ddr_snapshot_pause(0)==Result::Invalid && dorado_ddr_snapshot_pause(epoch+1)==Result::Invalid,
          "stale pause accepted");
    check(dorado_ddr_snapshot_continue(epoch)==Result::Invalid,"continue before pause accepted");
    const unsigned char payload[]={0x3a,0x17,0x42,0xef,0x91,0x6c};
    std::vector<unsigned char> saved;
    int connection;
    {
        std::lock_guard<std::mutex> guard(f.lock);
        put32(memory+0x100,sizeof payload);put32(memory+0x104,0);put64(memory+0x108,epoch);
        memcpy(memory+0x1000,payload,sizeof payload);put32(memory+0x10,1);
        check(!dorado_ddr_send(&f.service.ring,payload,sizeof payload),"stage actual RX ring");
        saved.assign(memory,memory+65536);connection=f.service.client;
    }
    // The manager already has an ordinary STATUS in flight when pause starts.
    f.serving=false;usleep(1010000);check(dorado_ddr_poll(12),"status before snapshot fell back");
    check(dorado_ddr_snapshot_pause(epoch)==Result::Wait,"pause behind STATUS rejected");forbid_spi=true;
    check(dorado_ddr_snapshot_continue(epoch)==Result::Wait,"early continue result");
    const auto initial=ms();unsigned polls=0;double max_call=0;
    while(ms()-initial<120){
        auto before=std::chrono::steady_clock::now();dorado_net_poll();++polls;
        max_call=std::max(max_call,std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-before).count());
        check(max_call<100 && dorado_ddr_snapshot_pause(epoch)==Result::Wait,"delayed ACK blocked or completed prematurely");usleep(200);
    }
    f.serving=true;
    until([&]{dorado_net_poll();return dorado_ddr_snapshot_pause(epoch)==Result::Done;},"PAUSE behind STATUS never completed");
    check(polls>100 && f.paused(),"missing bounded pending-record coverage");f.assert_held();
    check(starts==1 && opens==1 && closes==0,"PAUSE restarted/rebound daemon");
    f.protect(PROT_NONE);
    for(unsigned i=0;i<1000;i++){dorado_net_poll();check(dorado_ddr_snapshot_pause(epoch)==Result::Done,"paused lease lost");usleep(100);}
    // A paused STATUS heartbeat must also complete before CONTINUE is framed.
    f.serving=false;usleep(1010000);dorado_net_poll();
    check(dorado_ddr_snapshot_pause(epoch)==Result::Done,"heartbeat withdrew confirmed pause");
    check(dorado_ddr_snapshot_continue(epoch+1)==Result::Invalid,"stale continue accepted");
    check(dorado_ddr_snapshot_continue(epoch)==Result::Wait,"CONTINUE did not wait for heartbeat");
    for(unsigned i=0;i<30;i++){dorado_net_poll();check(dorado_ddr_snapshot_pending(),"snapshot lease released before CONTINUE ACK");usleep(100);}
    f.protect(PROT_READ|PROT_WRITE);f.serving=true;
    const uint64_t limit=ms()+2000;
    for(;;){
        dorado_net_poll();const auto result=dorado_ddr_snapshot_continue(epoch);
        check(dorado_ddr_snapshot_pending() || result==Result::Done,"snapshot lease released before CONTINUE ACK");
        if(result==Result::Done)break;
        check(result==Result::Wait && ms()<limit,"CONTINUE failed/timed out");usleep(100);
    }
    check(dorado_ddr_snapshot_continue(epoch)==Result::Done,"CONTINUE not idempotent");
    {
        std::lock_guard<std::mutex> guard(f.lock);
        check(!f.service.ring.paused && f.service.client==connection && f.service.last_epoch==epoch,"CONTINUE reset connection/epoch");
        check(f.service.ring.tx_cons==0 && f.service.ring.rx_prod==1 && !memcmp(memory,saved.data(),saved.size()),"CONTINUE changed queued bytes/cursors");
        unsigned char received[2048];check(dorado_ddr_receive(&f.service.ring,received,sizeof received)==int(sizeof payload) &&
            !memcmp(received,payload,sizeof payload),"continued actual TX payload");
        check(!memcmp(memory+0x5000,payload,sizeof payload),"continued actual RX payload");
    }
    check(starts==1 && opens==1 && closes==0,"CONTINUE used lifecycle RESUME");
    forbid_spi=false;check(dorado_net_barrier("finished snapshot") && dorado_net_reset(),"post-snapshot lifecycle blocked");
    check(!dorado_ddr_snapshot_epoch() && dorado_ddr_snapshot_continue(epoch)==Result::Invalid,"reset retained old snapshot completion");
    printf("MAIN SNAPSHOT PASS polls=%u max_call_ms=%.3f queued_bytes=12 same_epoch=1 same_connection=1 sole_spi_owner=1\n",polls,max_call);
}
static void failure(const char *mode){
    Fixture f;f.begin();f.assert_held();
    if(!strcmp(mode,"core-change"))test_original_core_name="MENU";
    else if(!strcmp(mode,"stale-ring")){
        std::lock_guard<std::mutex> guard(f.lock);put64(memory+8,f.epoch+1);
        check(dorado_ddr_snapshot_continue(f.epoch)==Result::Wait,"stale ring CONTINUE begin");
    }else if(!strcmp(mode,"eof")){
        std::lock_guard<std::mutex> guard(f.lock);check(!dorado_ddr_control_close(&f.service),"close paused peer");
        check(dorado_ddr_snapshot_continue(f.epoch)==Result::Wait,"closed peer CONTINUE begin");
    }else if(!strcmp(mode,"lost-pause")){
        std::lock_guard<std::mutex> guard(f.lock);check(!dorado_ddr_continue(&f.service.ring),"inject lost daemon pause");
    }else if(!strcmp(mode,"timeout")){f.protect(PROT_NONE);f.serving=false;}
    else check(false,"unknown failure case");
    const uint64_t limit=ms()+(!strcmp(mode,"timeout")?7500:3000);
    while(dorado_ddr_snapshot_pause(f.epoch)!=Result::Failed && ms()<limit){dorado_net_poll();usleep(200);}
    check(dorado_ddr_snapshot_pause(f.epoch)==Result::Failed,"lost daemon pause/session was accepted");
    check(dorado_ddr_snapshot_continue(f.epoch)==Result::Failed,"fault continued snapshot");f.assert_held();
    printf("MAIN SNAPSHOT FAILURE PASS mode=%s retained_lease=1 no_SPI=1\n",mode);
}
static void unexpected_pause(){
    Fixture f;
    {std::lock_guard<std::mutex> guard(f.lock);check(!dorado_ddr_pause(&f.service.ring),"inject unexpected pause");}
    usleep(1010000);
    const uint64_t limit=ms()+2000;
    while(dorado_ddr_snapshot_epoch() && ms()<limit){dorado_net_poll();usleep(200);}
    check(!dorado_ddr_snapshot_epoch() && !dorado_ddr_snapshot_pending() && dorado_ddr_needs_barrier(),"normal manager accepted unexpected pause");
    check(dorado_net_barrier("unexpected daemon pause") && dorado_net_reset(),"unleased fault did not drain");
    puts("MAIN SNAPSHOT UNEXPECTED PASS: unsolicited paused reply rejects normal active state");
}
int main(int argc,char **argv){
    if(argc==1)normal();else if(!strcmp(argv[1],"unexpected-pause"))unexpected_pause();else failure(argv[1]);return 0;
}
