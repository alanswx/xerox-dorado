#include "offload.cpp"
#include "dorado_disk.h"
#include "file_io.h"
#include "spi.h"
#include "user_io.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <string>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>
#include <vector>

using Clock=std::chrono::steady_clock;
const char *disk_test_core="Dorado";
// Disk-only test. The DDR manager has separate actual-service/SPI-spy gates.
bool dorado_net_barrier(const char *) { return true; }
static pthread_t main_thread;
static bool selected=false;
static unsigned command=0, notifications=0, captures=0, replies=0, size_parts=0;
static uint64_t image_size=0, advertised_size=0;
static std::vector<uint8_t> incoming,outgoing;
static std::atomic<bool> slow_entered{false}, inject_error{false};
static std::atomic<unsigned> write_calls{0};
// Exact patched function extracted from pinned input.cpp by the test driver.
static int mwd=-1,mfd=-1;
#include "input_handler.inc"
static void check(bool ok,const char *why) {
    if(!ok){fprintf(stderr,"DISK-ADAPTER FAIL: %s\n",why);std::_Exit(23);}
}
static void spi_owner(){check(pthread_equal(pthread_self(),main_thread),"worker accessed SPI");}
void EnableIO(){spi_owner();check(!selected,"nested SPI");selected=true;}
void DisableIO(){spi_owner();check(selected,"deselect inactive SPI");selected=false;}
uint8_t spi8(uint8_t c){spi_owner();check(selected,"command without select");command=c;size_parts=0;image_size=0;return 0;}
uint16_t spi_w(uint16_t c){spi_owner();check(selected,"word without select");command=c;return 0;}
uint32_t spi32_w(uint32_t x){spi_owner();check(selected&&command==UIO_SET_SDINFO,"size protocol");image_size|=uint64_t(x)<<(32*size_parts++);return 0;}
void spi32_b(uint32_t x){spi32_w(x);}
uint8_t spi_uio_cmd8(uint8_t c,uint8_t){spi_owner();check(!selected&&c==UIO_SET_SDSTAT&&size_parts==2,"notify protocol");++notifications;advertised_size=image_size;return 0;}
void spi_block_read(uint8_t *dst,int,int n){spi_owner();check(selected&&command==UIO_SECTOR_WR&&incoming.size()==size_t(n),"capture protocol/size");memcpy(dst,incoming.data(),n);++captures;}
void spi_block_write(const uint8_t *src,int,int n){spi_owner();check(selected&&command==UIO_SECTOR_RD,"reply protocol");outgoing.assign(src,src+n);++replies;}

extern "C" ssize_t __real_pwrite64(int,const void *,size_t,off64_t);
extern "C" ssize_t __wrap_pwrite64(int fd,const void *p,size_t n,off64_t o){
    if(!write_calls++) {slow_entered=true;std::this_thread::sleep_for(std::chrono::milliseconds(4200));}
    if(inject_error){errno=ENOSPC;return -1;}
    return __real_pwrite64(fd,p,n,o);
}

template<typename P> static void until(P done,const char *why){
    auto end=Clock::now()+std::chrono::seconds(12);
    while(!done()){dorado_disk_poll();check(Clock::now()<end,why);std::this_thread::sleep_for(std::chrono::milliseconds(1));}
}

int main(int argc,char **argv){
    main_thread=pthread_self();
    const bool fail=argc>1&&!strcmp(argv[1],"error");
    const bool interrupt=argc>1&&!strcmp(argv[1],"int");
    const bool terminate=interrupt||(argc>1&&!strcmp(argv[1],"term"));
    setenv("DORADO_DISK_CACHE","1",1);
    if(!getenv("DORADO_DISK_TRACE"))setenv("DORADO_DISK_TRACE","1",1);
    offload_start();
    signal(SIGINT,INThandler);
    char path[]="/tmp/dorado-disk-adapter-XXXXXX";
    int fd=terminate?open(argv[2],O_CREAT|O_EXCL|O_RDWR,0600):mkstemp(path);
    check(fd>=0,"temp image");if(!terminate)unlink(path);
    std::vector<uint8_t> oracle(256*1024);for(size_t i=0;i<oracle.size();++i)oracle[i]=(i*37+i/511)&255;
    check(::write(fd,oracle.data(),oracle.size())==ssize_t(oracle.size()),"seed");
    fileTYPE f={fdopen(fd,"r+"),nullptr,0,int64_t(oracle.size())};check(f.filp,"fdopen");
    disk_test_core="Other";check(!dorado_disk_enabled()&&dorado_disk_service(0,1,0,512,512,0,1)==0,"other core intercepted");
    disk_test_core="Dorado";check(dorado_disk_enabled(),"opt in");
    f.zip=&f;check(!dorado_disk_mount(&f,0,true,true),"zip accepted");f.zip=nullptr;
    check(dorado_disk_mount(&f,0,true,true)&&notifications==1&&advertised_size==0,"preload advertised early");
    check(dorado_disk_service(0,1,0,512,512,0,1)<0&&replies==0,"read served before ready");
    until([&]{return advertised_size==oracle.size();},"ready notification timeout");
    check(dorado_disk_service(0,1,0,512,512,0,1)==1&&outgoing==std::vector<uint8_t>(oracle.begin(),oracle.begin()+512),"initial read");
    incoming.assign(31*512,0x6a);
    check(dorado_disk_service(0,2,1,512,incoming.size(),0,1)==1&&captures==1,"first write");
    std::copy(incoming.begin(),incoming.end(),oracle.begin()+512);
    until([]{return slow_entered.load();},"slow write not dispatched");
    unsigned polls=0;double max_gap=0;auto before=Clock::now();const auto start=before;
    while(Clock::now()-start<std::chrono::milliseconds(4000)){
        auto now=Clock::now();max_gap=std::max(max_gap,std::chrono::duration<double,std::milli>(now-before).count());before=now;
        incoming.assign(512,uint8_t(polls));const size_t lba=(polls%400)+1;
        check(dorado_disk_service(0,2,lba,512,512,0,1)==1,"write service blocked");
        std::copy(incoming.begin(),incoming.end(),oracle.begin()+lba*512);
        check(dorado_disk_service(0,1,lba,512,512,0,1)==1&&outgoing==incoming,"stale read during slow IO");
        dorado_disk_poll();++polls;
        check(max_gap<100,"SPI/poll heartbeat blocked by 4.2s file IO");
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    if(fail)inject_error=true;
    if(terminate){
        const std::string expected=std::string(argv[2])+".expected";
        int out=open(expected.c_str(),O_WRONLY|O_CREAT|O_EXCL,0600);
        check(out>=0&&::write(out,oracle.data(),oracle.size())==ssize_t(oracle.size()),"expected file");::close(out);
        kill(getpid(),interrupt?SIGINT:SIGTERM);dorado_disk_poll();
        check(false,"termination request returned without draining/exiting");
    }
    if(fail){
        until([]{return !dorado_disk_barrier("test failed core load");},"error barrier timeout");
        check(!dorado_disk_before_mount(0),"failed dirty image remounted");
        unsigned old=captures;check(dorado_disk_service(0,2,1,512,512,0,1)<0&&captures==old,"error ACKed more data");
        kill(getpid(),SIGTERM);dorado_disk_poll();
        kill(getpid(),SIGINT);dorado_disk_poll();
        puts("DISK-ADAPTER ERROR PASS: file failure retains image, prevents lifecycle change and consumes no further SPI write");
    }else{
        check(dorado_disk_barrier("test core load"),"durable barrier");
        check(dorado_disk_before_mount(0),"close old generation");
        check(dorado_disk_mount(&f,0,true,true),"remount start");
        until([&]{return advertised_size==oracle.size();},"remount ready");
        for(size_t lba=0;lba<oracle.size()/512;++lba){
            check(dorado_disk_service(0,1,lba,512,512,0,1)==1&&
                  !memcmp(outgoing.data(),oracle.data()+lba*512,512),"full cold remount oracle");
        }
        const unsigned old=captures;
        check(dorado_disk_service(0,2,UINT64_MAX,512,512,0,1)<0&&captures==old,"overflow ACKed");
        check(dorado_disk_before_mount(0),"final close");
        printf("DISK-ADAPTER PASS: actual adapter/cache/worker, 4.2s file stall, %u main-thread SPI read/write polls, max gap %.3fms; deferred mount/full cold remount/overflow/sole SPI owner\n",polls,max_gap);
    }
    fclose(f.filp);offload_stop();
}
