// Actual pinned worker + dynamically loaded probe; use the same compiler/ABI.
#include "offload.cpp"
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include <unistd.h>

using Clock=std::chrono::steady_clock;
using Probe=int(*)(uintptr_t,int,const char*,const char*,unsigned,unsigned);
static void check(bool good,const char *why){
    if(!good){fprintf(stderr,"SLOW-SYNC FAIL: %s\n",why);std::_Exit(23);}
}
static std::string read_file(const std::string &path){
    FILE *f=fopen(path.c_str(),"rb");if(!f)return {};
    char data[4096];size_t n=fread(data,1,sizeof data,f);fclose(f);return std::string(data,n);
}
static bool reject(std::function<void()>){return false;}
int main(int argc,char **argv){
    check(argc>=3,"usage DSO event-prefix [timeout]");
    bool timeout=argc>3;
    void *lib=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);check(lib,"dlopen");
    auto probe=reinterpret_cast<Probe>(dlsym(lib,"dorado_queue_slow_fsync"));check(probe,"dlsym");
    const std::string base=argv[2],image=base+".image",arm=base+".arm",events=base+".jsonl";
    int fd=open(image.c_str(),O_CREAT|O_EXCL|O_RDWR|O_SYNC,0600);check(fd>=0,"create image");
    const char bytes[]="bounded worker sync preserves the original image";
    check(write(fd,bytes,sizeof bytes)==ssize_t(sizeof bytes),"seed");
    offload_start();
    check(probe(uintptr_t(&reject),fd,arm.c_str(),events.c_str(),4200,200)==EAGAIN,"queue rejection");
    check(fcntl(fd,F_GETFD)>=0&&access(events.c_str(),F_OK),"rejection consumed descriptor or job");
    check(probe(uintptr_t(&offload_try_add_work),fd,arm.c_str(),events.c_str(),4200,timeout?200:5000)==0,"queue probe");
    check(probe(uintptr_t(&offload_try_add_work),fd,arm.c_str(),events.c_str(),4200,5000)==EALREADY,"duplicate accepted");
    close(fd); // Worker must retain its own old-image descriptor.
    const auto limit=Clock::now()+std::chrono::seconds(8);
    while(read_file(events).find("waiting")==std::string::npos){
        check(Clock::now()<limit,"worker start timeout");std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    if(!timeout){int gate=open(arm.c_str(),O_CREAT|O_EXCL|O_WRONLY,0600);check(gate>=0,"arm");close(gate);}
    auto previous=Clock::now();double max_gap=0;unsigned polls=0;
    for(;;){
        const auto before=Clock::now();
        max_gap=std::max(max_gap,std::chrono::duration<double,std::milli>(before-previous).count());previous=before;
        check(max_gap<100,"worker flush blocked foreground heartbeat");++polls;
        const auto text=read_file(events);
        if(text.find(timeout?"timeout":"\"end\"")!=std::string::npos)break;
        check(before<limit,"probe completion timeout");std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    offload_stop();
    check(read_file(image)==std::string(bytes,sizeof bytes),"image changed");
    check(polls>(timeout?20u:1000u),"heartbeat coverage");
    dlclose(lib); // Only after actual worker drain/destruction of its callback.
    printf("SLOW-SYNC PASS: %s, %u foreground polls, max gap %.3fms; rejection/duplicate/owned-FD/content/drain/DSO ABI\n",
           timeout?"bounded arm timeout":"4.2s real-worker flush work",polls,max_gap);
}
