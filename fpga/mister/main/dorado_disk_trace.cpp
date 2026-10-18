#include "dorado_disk_trace.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

DoradoDiskTrace::DoradoDiskTrace() {
    const char *option = getenv("DORADO_DISK_TRACE");
    if (!option || !*option || !strcmp(option,"0")) return;
    char path[128];
    if (!strcmp(option,"1")) {
        snprintf(path,sizeof path,"/tmp/dorado-disk-%ld.jsonl",long(getpid()));
        option = path;
    }
    fd_ = open(option,O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600);
    if (fd_ < 0) fprintf(stderr,"Dorado disk: trace open failed: %s\n",strerror(errno));
    else printf("Dorado disk: monotonic trace %s\n",option);
}
DoradoDiskTrace::~DoradoDiskTrace() { if (fd_ >= 0) ::close(fd_); }
uint64_t DoradoDiskTrace::now() {
    timespec t; clock_gettime(CLOCK_MONOTONIC,&t);
    return uint64_t(t.tv_sec)*1000000000ull+t.tv_nsec;
}
void DoradoDiskTrace::record(const char *kind, uint64_t offset, uint64_t bytes,
                             int64_t result, uint64_t duration) {
    if (fd_ < 0) return;
    const uint64_t time = now();
    std::lock_guard<std::mutex> guard(lock_);
    if (count_ == Capacity) { head_=(head_+1)%Capacity; --count_; ++dropped_; }
    events_[(head_+count_)%Capacity]={time,offset,bytes,duration,result,kind}; ++count_;
}
void DoradoDiskTrace::flush() {
    if (fd_ < 0) return;
    std::array<Event,Capacity> copy;
    size_t count; uint64_t dropped;
    {
        std::lock_guard<std::mutex> guard(lock_);
        count=count_; dropped=dropped_; dropped_=0;
        for (size_t i=0;i<count;++i) copy[i]=events_[(head_+i)%Capacity];
        head_=(head_+count)%Capacity; count_=0;
    }
    char line[320];
    for (size_t i=0;i<count+(dropped?1u:0u);++i) {
        const Event e=i<count?copy[i]:Event{now(),0,dropped,0,0,"trace_dropped"};
        int size=snprintf(line,sizeof line,
            "{\"ns\":%llu,\"event\":\"%s\",\"offset\":%llu,\"bytes\":%llu,\"result\":%lld,\"duration_ns\":%llu}\n",
            (unsigned long long)e.time,e.kind,(unsigned long long)e.offset,
            (unsigned long long)e.bytes,(long long)e.result,(unsigned long long)e.duration);
        size_t done=0;
        while (done<size_t(size)) {
            ssize_t n=::write(fd_,line+done,size-done);
            if (n<0 && errno==EINTR) continue;
            if (n<=0) return; // Diagnostic failure cannot change disk success.
            done+=n;
        }
    }
}
