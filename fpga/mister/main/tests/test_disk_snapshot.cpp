// Reuse the actual-worker adapter's sole-SPI-owner spies and physical IO.
// This is a host boundary gate, not an FPGA pause or complete snapshot.
#define main disk_adapter_legacy_main
#include "legacy_adapter.cpp"
#undef main
#include <array>

using Cache=DoradoDiskCache;
using Result=Cache::Result;
static std::atomic<bool> sync_entered{false},allow_sync{false},sync_failure{false};
extern "C" int __real_fsync(int);
extern "C" int __wrap_fsync(int fd){
    sync_entered=true;
    while(!allow_sync)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if(sync_failure){errno=EIO;return -1;}
    return __real_fsync(fd);
}

int main(int argc,char **argv){
    main_thread=pthread_self();
    const bool fail_write=argc>1&&!strcmp(argv[1],"write-error");
    const bool fail_sync=argc>1&&!strcmp(argv[1],"sync-error");
    inject_error=fail_write;sync_failure=fail_sync;
    setenv("DORADO_DISK_CACHE","1",1);unsetenv("DORADO_DISK_TRACE");offload_start();
    check(dorado_disk_snapshot_pause(0)==Result::Invalid,"unmounted pause accepted");
    char path[]="/tmp/dorado-disk-snapshot-XXXXXX";int fd=mkstemp(path);check(fd>=0,"snapshot temp image");unlink(path);
    std::vector<uint8_t> oracle(256*1024);
    for(size_t i=0;i<oracle.size();++i)oracle[i]=(i*41+i/509)&255;
    check(::write(fd,oracle.data(),oracle.size())==ssize_t(oracle.size()),"snapshot seed");
    fileTYPE file={fdopen(fd,"r+"),nullptr,0,int64_t(oracle.size())};check(file.filp,"snapshot fdopen");
    check(dorado_disk_mount(&file,0,true,true),"snapshot mount");
    until([&]{return advertised_size==oracle.size();},"snapshot preload timeout");
    auto status=dorado_disk_snapshot_status();const uint64_t generation=status.generation;
    check(status.state==Cache::State::Ready&&!status.snapshot_requested&&!status.snapshot_ready,"initial lease flags");
    disk_test_core="Other";check(dorado_disk_snapshot_pause(generation)==Result::Invalid,"foreign core pause accepted");disk_test_core="Dorado";
    incoming.assign(31*512,0x6a);
    check(dorado_disk_service(0,2,1,512,incoming.size(),0,1)==1,"snapshot seed capture");
    std::copy(incoming.begin(),incoming.end(),oracle.begin()+512);
    until([]{return slow_entered.load();},"snapshot slow write timeout");
    auto before=Clock::now();
    check(dorado_disk_snapshot_pause(generation)==Result::Wait,"snapshot acknowledged unfinished write");
    check(std::chrono::duration<double,std::milli>(Clock::now()-before).count()<100,"snapshot begin blocked Main");
    check(dorado_disk_snapshot_pause(generation+1)==Result::Invalid,"stale pause generation accepted");
    check(dorado_disk_snapshot_continue(generation)==Result::Wait,"unfinished snapshot released");
    check(dorado_disk_snapshot_status().snapshot_requested,"unfinished snapshot lost lease");
    check(!dorado_disk_before_mount(0)&&!dorado_disk_barrier("snapshot lease test"),"leased image replaced/drained as lifecycle");
    const auto saved_captures=captures,saved_replies=replies;
    std::array<uint8_t,512> saved; saved.fill(0xa5);
    check(dorado_disk_snapshot_read(generation,0,saved.data(),saved.size())==Result::Wait,"unfinished snapshot read accepted");
    check(std::all_of(saved.begin(),saved.end(),[](uint8_t v){return v==0xa5;}),"deferred snapshot changed destination");
    unsigned polls=0;double max_gap=0,max_call=0;auto previous=Clock::now();
    const auto deadline=previous+std::chrono::seconds(12);
    for(;;){
        const auto start=Clock::now();max_gap=std::max(max_gap,std::chrono::duration<double,std::milli>(start-previous).count());previous=start;
        check(dorado_disk_service(0,1,1,512,512,0,1)<0&&dorado_disk_service(0,2,1,512,512,0,1)<0,"snapshot served normal SPI request");
        check(captures==saved_captures&&replies==saved_replies,"snapshot consumed/ACKed SPI data");
        dorado_disk_poll();status=dorado_disk_snapshot_status();++polls;
        max_call=std::max(max_call,std::chrono::duration<double,std::milli>(Clock::now()-start).count());
        check(max_gap<100&&max_call<100,"snapshot wait blocked regular poll heartbeat");
        check(!status.snapshot_ready&&status.snapshot_requested,"snapshot ready before durable worker boundary");
        if(status.state==Cache::State::Error||sync_entered)break;
        check(Clock::now()<deadline,"snapshot worker progress timeout");
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    check(polls>100,"snapshot did not exercise delayed file work");
    if(!fail_write){
        check(sync_entered&&status.needs_sync&&status.job_pending,"snapshot sync boundary not active");
        check(dorado_disk_snapshot_pause(generation)==Result::Wait&&dorado_disk_snapshot_continue(generation)==Result::Wait,
              "snapshot skipped pending sync");
        allow_sync=true;
        until([&]{status=dorado_disk_snapshot_status();return status.snapshot_ready||status.state==Cache::State::Error;},"snapshot sync completion timeout");
    }
    if(fail_write||fail_sync){
        status=dorado_disk_snapshot_status();
        check(status.state==Cache::State::Error&&status.snapshot_requested&&!status.snapshot_ready,"failed snapshot lost error/lease");
        check(fail_write?status.dirty_sectors>0:status.needs_sync,"failed snapshot lost dirty recovery state");
        check(dorado_disk_snapshot_pause(generation)==Result::Failed&&dorado_disk_snapshot_continue(generation)==Result::Failed,
              "failed snapshot continued");
        check(dorado_disk_snapshot_read(generation,0,saved.data(),saved.size())==Result::Failed,"failed snapshot exported data");
        check(!dorado_disk_before_mount(0),"failed leased generation replaced");
        printf("DISK SNAPSHOT ERROR PASS mode=%s polls=%u max_gap_ms=%.3f dirty=%llu needs_sync=%d\n",fail_write?"write":"sync",polls,max_gap,
            (unsigned long long)status.dirty_sectors,status.needs_sync);
    }else{
        check(status.generation==generation&&status.snapshot_requested&&status.snapshot_ready&&!status.job_pending&&!status.needs_sync&&!status.dirty_sectors,
              "durable snapshot flags differ");
        check(dorado_disk_snapshot_pause(generation)==Result::Done,"repeated ready pause not idempotent");
        std::array<uint8_t,Cache::Run> chunk;
        for(size_t offset=0;offset<oracle.size();offset+=chunk.size()){
            check(dorado_disk_snapshot_read(generation,offset,chunk.data(),chunk.size())==Result::Done,"ready snapshot read failed");
            check(!memcmp(chunk.data(),oracle.data()+offset,chunk.size()),"snapshot bytes differ from oracle");
        }
        std::vector<uint8_t> physical(oracle.size());
        check(pread(fd,physical.data(),physical.size(),0)==ssize_t(physical.size())&&physical==oracle,"snapshot not durable in actual file");
        check(dorado_disk_snapshot_read(generation+1,0,chunk.data(),512)==Result::Invalid,"stale snapshot read accepted");
        check(dorado_disk_snapshot_read(generation,UINT64_MAX,chunk.data(),512)==Result::Invalid,"snapshot bounds accepted");
        check(!dorado_disk_before_mount(0),"ready leased generation replaced");
        check(dorado_disk_snapshot_continue(generation)==Result::Done&&dorado_disk_snapshot_continue(generation)==Result::Done,"continue not idempotent");
        check(dorado_disk_service(0,1,1,512,512,0,1)==1,"continued disk did not serve SPI");
        check(dorado_disk_before_mount(0)&&dorado_disk_mount(&file,0,true,true),"snapshot close/remount failed");
        until([&]{return advertised_size==oracle.size();},"snapshot cold remount timeout");
        check(dorado_disk_snapshot_status().generation!=generation,"snapshot generation reused");
        check(dorado_disk_snapshot_pause(generation)==Result::Invalid,"old lease accepted after remount");
        for(size_t lba=0;lba<oracle.size()/512;lba++){
            check(dorado_disk_service(0,1,lba,512,512,0,1)==1&&!memcmp(outgoing.data(),oracle.data()+lba*512,512),"snapshot cold remount oracle differs");
        }
        check(dorado_disk_before_mount(0),"snapshot final close");
        printf("DISK SNAPSHOT PASS polls=%u max_gap_ms=%.3f max_call_ms=%.3f bytes=%zu captures_held=%u actual_worker=1 sole_spi_owner=1\n",
            polls,max_gap,max_call,oracle.size(),saved_captures);
    }
    fclose(file.filp);offload_stop();return 0;
}
