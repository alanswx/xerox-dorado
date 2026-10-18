#include "../dorado_control_client.h"
extern "C" {
#include "dorado_ddr_control.h"
}
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>

static void check(bool good,const char *why){if(!good){fprintf(stderr,"MAIN CONTROL FAIL: %s\n",why);std::_Exit(23);}}
static unsigned sends,receives,rx_offset;
static bool corrupt_sequence;
static ssize_t limited_send(int fd,const void *p,size_t n,int flags){
    if(++sends%7==0){errno=EINTR;return -1;}
    return send(fd,p,n>3?3:n,flags);
}
static ssize_t limited_receive(int fd,void *p,size_t n,int flags){
    if(++receives%11==0){errno=EAGAIN;return -1;}
    ssize_t count=recv(fd,p,n>5?5:n,flags);
    if(count>0){
        if(corrupt_sequence && rx_offset<=12 && rx_offset+unsigned(count)>12)
            static_cast<unsigned char *>(p)[12-rx_offset]^=1;
        rx_offset=(rx_offset+unsigned(count))%32;
    }
    return count;
}
struct Fixture {
    dorado_ddr_control service={};
    unsigned char *memory;
    char directory[64]="/tmp/dorado-main-control-XXXXXX",path[108];
    std::atomic<bool> running{true},bad_cleanup{false},fail_close{false};
    std::atomic<unsigned> opens{0},closes{0},writes{0},delay_ms{0},seed_ready{0};
    std::thread worker;
    static void put32(unsigned char *p,uint32_t n){for(unsigned i=0;i<4;i++)p[i]=(unsigned char)(n>>(8*i));}
    static void put64(unsigned char *p,uint64_t n){put32(p,uint32_t(n));put32(p+4,uint32_t(n>>32));}
    static int map(void *opaque,volatile void **p){
        auto *f=static_cast<Fixture *>(opaque);check(f->seed_ready.load()!=0,"map before seed synchronization");
        check(!mprotect(f->memory,65536,PROT_READ|PROT_WRITE),"map oracle");*p=f->memory;f->opens++;return 0;
    }
    static int unmap(void *opaque,volatile void *p){
        auto *f=static_cast<Fixture *>(opaque);check(p==f->memory,"wrong cleanup mapping");
        if(f->service.granted || f->service.ring.active || f->service.ring.memory)f->bad_cleanup=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(f->delay_ms.load()));
        check(!mprotect(f->memory,65536,PROT_NONE),"protect stopped mapping");f->closes++;
        return f->fail_close?EIO:0;
    }
    Fixture(){
        memory=static_cast<unsigned char *>(mmap(nullptr,65536,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
        check(memory!=MAP_FAILED && mkdtemp(directory),"fixture allocation");
        snprintf(path,sizeof path,"%s/control.sock",directory);
        dorado_ddr_mapper mapper={map,unmap,this};
        check(!dorado_ddr_control_open(&service,path,mapper),"service open");
        worker=std::thread([this]{
            while(running){
                dorado_ddr_control_poll(&service);
                if(service.granted && service.ring.active){
                    const unsigned char bytes[4]={0x13,0x57,0x9b,0xdf};
                    int result=dorado_ddr_send(&service.ring,bytes,4);
                    if(!result)writes++;
                    else check(result==-EAGAIN,"worker ring fault");
                }
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
            check(!dorado_ddr_control_close(&service),"service close");
        });
    }
    void seed(uint64_t epoch){
        check(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"seed map");memset(memory,0,65536);
        put64(memory,UINT64_C(0x31474e49524f4444));put64(memory+8,epoch);
        put32(memory+0x30,1);put64(memory+0x38,UINT64_C(0x0001080000100008));
        check(!mprotect(memory,65536,PROT_NONE),"seal oracle");seed_ready++;
    }
    ~Fixture(){running=false;worker.join();check(!bad_cleanup,"cleanup preceded quiesce");
        check(!rmdir(directory) && !munmap(memory,65536),"fixture cleanup");}
};
static void finish(DoradoControlClient &client){
    auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(1);
    for(;;){int result=client.pump();if(result){check(result==1,"client pump failure");return;}
        check(std::chrono::steady_clock::now()<deadline,"pump deadline");
        std::this_thread::sleep_for(std::chrono::microseconds(50));}
}
int main(){
    signal(SIGPIPE,SIG_IGN);Fixture f;
    DoradoControlClient client({limited_send,limited_receive});
    check(!client.open(f.path),"client open");
    constexpr uint64_t epoch=UINT64_C(0x9376ab650021ddce);
    f.seed(epoch);check(!client.begin(DORADO_CTRL_RESUME,epoch),"start resume");
    check(client.pump()==0 && client.pending(),"partial request fixture");
    // stop must finish that partial request and get its reply BEFORE emitting
    // the STOP record; replacing its bytes would corrupt the control stream.
    check(!client.stop(1000),"stop after partial resume");
    check(f.opens==1 && f.closes==1,"partial resume skipped or incomplete stop");
    unsigned previous=f.writes.load();std::this_thread::sleep_for(std::chrono::milliseconds(20));
    check(f.writes==previous,"MMIO after successful STOP ACK");
    f.seed(epoch+1);check(!client.begin(DORADO_CTRL_RESUME,epoch+1),"second resume");finish(client);
    check(!client.reply().error && client.reply().active,"resume reply");
    f.delay_ms=150;
    check(client.stop(10)==-ETIMEDOUT && client.pending(),"timeout granted core-load permission or lost pending stream");
    check(!client.stop(1000),"retry did not finish real stop acknowledgement");
    f.delay_ms=0;f.seed(epoch+2);check(!client.begin(DORADO_CTRL_RESUME,epoch+2),"third resume");finish(client);
    f.fail_close=true;check(client.stop(1000)==-EIO,"cleanup failure reported safe");
    f.fail_close=false;check(!client.stop(1000),"failed-cleanup retry");
    f.seed(epoch+3);corrupt_sequence=true;
    check(!client.begin(DORADO_CTRL_RESUME,epoch+3),"corrupt-reply resume");
    int result=0;
    for(unsigned i=0;i<2000 && !result;i++){result=client.pump();std::this_thread::sleep_for(std::chrono::microseconds(50));}
    check(result==-EPROTO && !client.connected(),"wrong reply sequence accepted");
    check(client.stop(10)==-ENOTCONN,"disconnect granted reload permission");
    corrupt_sequence=false;
    // A reconnect is an explicit new controller session; require a new real
    // STOP ACK after the old socket's EOF rather than treating EOF as an ACK.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    check(!client.open(f.path) && !client.stop(1000),"fresh controller stop");
    client.close();
    printf("MAIN CONTROL CLIENT PASS: actual daemon service,3/5-byte fragments, EINTR/EAGAIN, pending request then STOP, delayed ACK refusal/retry, cleanup failure, wrong reply rejection and reconnect; opens=%u closes=%u\n",f.opens.load(),f.closes.load());
}
