// Qualification-only DSO. Loaded explicitly into the exact pinned Main.
// A bounded real-worker flush job, with a deliberate latency interval. Never
// accesses SPI or changes image bytes. Keep the DSO loaded until Main exits.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <memory>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

namespace {
std::atomic<unsigned> claimed{0};
uint64_t now() {
    timespec t; clock_gettime(CLOCK_MONOTONIC,&t);
    return uint64_t(t.tv_sec)*1000000000ull+t.tv_nsec;
}
void pause_ms(unsigned ms) {
    timespec remaining={time_t(ms/1000),long(ms%1000)*1000000};
    while(nanosleep(&remaining,&remaining)<0 && errno==EINTR) {}
}
struct Job {
    int fd=-1;
    unsigned delay_ms=0,arm_timeout_ms=0;
    char arm[192]={},events[192]={};
    ~Job(){if(fd>=0)::close(fd);}
    void record(int out,const char *phase,int result,int error,uint64_t duration=0) {
        char line[256];
        int size=snprintf(line,sizeof line,
            "{\"phase\":\"%s\",\"ns\":%llu,\"result\":%d,\"error\":%d,\"duration_ns\":%llu}\n",
            phase,(unsigned long long)now(),result,error,(unsigned long long)duration);
        size_t done=0;
        while(done<size_t(size)) {
            ssize_t n=::write(out,line+done,size-done);
            if(n<0 && errno==EINTR)continue;
            if(n<=0)return;
            done+=n;
        }
    }
    void run() {
        int out=open(events,O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600);
        if(out<0){claimed=2;return;}
        record(out,"waiting",0,0);
        const uint64_t deadline=now()+uint64_t(arm_timeout_ms)*1000000;
        while(access(arm,F_OK)) {
            if(now()>=deadline) {
                record(out,"timeout",-1,ETIMEDOUT);::close(out);claimed=2;return;
            }
            pause_ms(1);
        }
        const uint64_t start=now();record(out,"begin",0,0);
        pause_ms(delay_ms);
        int result;
        do{result=fsync(fd);}while(result<0 && errno==EINTR);
        const int error=result<0?errno:0;
        record(out,"end",result,error,now()-start);
        ::close(out);claimed=2;
    }
};
}

// Caller supplies the audited Thumb entry for offload_try_add_work in Main.
// The pinned GCC/libstdc++ ABI must match the process; test that exact bridge
// before loading. The callback owns its descriptor and buffers until return.
extern "C" int dorado_queue_slow_fsync(uintptr_t submit_address,int source_fd,
    const char *arm_path,const char *events_path,unsigned delay_ms,unsigned arm_timeout_ms) {
    if(!submit_address || !arm_path || !events_path ||
       strncmp(arm_path,"/tmp/",5) || strncmp(events_path,"/tmp/",5) ||
       strlen(arm_path)>=192 || strlen(events_path)>=192 ||
       !delay_ms || delay_ms>10000 || !arm_timeout_ms || arm_timeout_ms>30000)
        return EINVAL;
    struct stat st;
    if(fstat(source_fd,&st))return errno;
    if(!S_ISREG(st.st_mode))return EINVAL;
    unsigned expected=0;
    if(!claimed.compare_exchange_strong(expected,1))return EALREADY;
    if(!access(arm_path,F_OK) || !access(events_path,F_OK)){claimed=0;return EEXIST;}
    std::shared_ptr<Job> job;
    try{job=std::make_shared<Job>();}catch(const std::bad_alloc &){claimed=0;return ENOMEM;}
    job->fd=fcntl(source_fd,F_DUPFD_CLOEXEC,3);
    if(job->fd<0){int error=errno;claimed=0;return error;}
    job->delay_ms=delay_ms;job->arm_timeout_ms=arm_timeout_ms;
    strcpy(job->arm,arm_path);strcpy(job->events,events_path);
    auto submit=reinterpret_cast<bool(*)(std::function<void()>)>(submit_address);
    try{if(!submit([job]{job->run();})){claimed=0;return EAGAIN;}}
    catch(const std::bad_alloc &){claimed=0;return ENOMEM;}
    return 0;
}
