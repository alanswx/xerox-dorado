// Main-thread SPI adapter. Only the cache's IO callbacks execute on the worker.
#include "dorado_disk.h"
#include "dorado_net.h"
#include "dorado_disk_cache.h"
#include "dorado_disk_trace.h"
#include "file_io.h"
#include "offload.h"
#include "spi.h"
#include "user_io.h"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

namespace {
DoradoDiskTrace &trace() { static auto *t = new DoradoDiskTrace; return *t; }
DoradoDiskCache &cache() {
    // Lifetime is Main's process, including failed dirty data. Explicit hooks
    // drain before fork/reexec; no static destructor can race an offload job.
    static auto *c = new DoradoDiskCache([](std::function<void()> work) {
        return offload_try_add_work(std::move(work));
    }, {
        [](int fd, void *p, size_t n, off_t offset) {
            auto &t=trace(); t.record("pread_begin",offset,n); t.flush();
            const auto start=t.now(); ssize_t r=::pread(fd,p,n,offset); int error=errno;
            t.record("pread_end",offset,n,r,t.now()-start);
            if (r<0) t.record("pread_error",offset,n,error);
            t.flush(); errno=error; return r;
        },
        [](int fd, const void *p, size_t n, off_t offset) {
            auto &t=trace(); t.record("pwrite_begin",offset,n); t.flush();
            const auto start=t.now(); ssize_t r=::pwrite(fd,p,n,offset); int error=errno;
            t.record("pwrite_end",offset,n,r,t.now()-start);
            if (r<0) t.record("pwrite_error",offset,n,error);
            t.flush(); errno=error; return r;
        },
        [](int fd) {
            auto &t=trace(); t.record("fsync_begin"); t.flush();
            const auto start=t.now(); int r=::fsync(fd); int error=errno;
            t.record("fsync_end",0,0,r,t.now()-start);
            if (r<0) t.record("fsync_error",0,0,error);
            t.flush(); errno=error; return r;
        }
    });
    return *c;
}
bool mounted = false, announced = false, writable_image = false, version = false;
bool reported_error = false;
volatile sig_atomic_t signal_active=0, pending_exit=0;
void request_exit(int signal) { pending_exit=signal; }
bool install_exit_handler() {
    if (signal_active) return true;
    struct sigaction action={}; action.sa_handler=request_exit; sigemptyset(&action.sa_mask);
    if (sigaction(SIGTERM,&action,nullptr)) return false;
    signal_active=1;
    return true;
}
void notify(uint64_t size, bool writable) {
    EnableIO();
    spi8(UIO_SET_SDINFO);
    if (version) { spi32_w(size); spi32_w(size >> 32); }
    else { spi32_b(size); spi32_b(size >> 32); }
    DisableIO();
    spi_uio_cmd8(UIO_SET_SDSTAT, 1 | (writable ? 0 : 0x80));
}
}

bool dorado_disk_enabled() {
    const char *option = getenv("DORADO_DISK_CACHE");
    return option && !strcmp(option, "1") && !strcmp(user_io_get_core_name(1), "Dorado");
}

bool dorado_disk_defer_exit(int signal) {
    if (!signal_active) return false;
    pending_exit=signal;
    return true;
}

bool dorado_disk_prepare_exit() { return install_exit_handler(); }

bool dorado_disk_barrier(const char *reason) {
    if (!mounted) return true;
    if (cache().status().snapshot_requested) {
        fprintf(stderr, "Dorado disk: refusing %s: snapshot owns the image generation\n", reason);
        return false;
    }
    trace().record("barrier_begin");
    if (!cache().drain()) {
        auto s = cache().status();
        trace().record("barrier_error",s.generation,s.dirty_sectors,s.error); trace().flush();
        fprintf(stderr, "Dorado disk: refusing %s: IO error %d, generation %llu, dirty %llu; cache retained\n",
                reason, s.error, (unsigned long long)s.generation, (unsigned long long)s.dirty_sectors);
        return false;
    }
    trace().record("barrier_end"); trace().flush();
    return true;
}

DoradoDiskCache::Status dorado_disk_snapshot_status() { return cache().status(); }

DoradoDiskCache::Result dorado_disk_snapshot_pause(uint64_t generation) {
    if (!mounted || !dorado_disk_enabled()) return DoradoDiskCache::Result::Invalid;
    return cache().snapshot_pause(generation);
}

DoradoDiskCache::Result dorado_disk_snapshot_read(uint64_t generation, uint64_t offset,
                                                void *dst, size_t bytes) {
    if (!mounted || !dorado_disk_enabled()) return DoradoDiskCache::Result::Invalid;
    return cache().snapshot_read(generation, offset, dst, bytes);
}

DoradoDiskCache::Result dorado_disk_snapshot_continue(uint64_t generation) {
    if (!mounted || !dorado_disk_enabled()) return DoradoDiskCache::Result::Invalid;
    return cache().snapshot_continue(generation);
}

bool dorado_disk_before_mount(unsigned index) {
    if (!mounted) return true;
    if (index != 0) return true;
    if (!dorado_disk_barrier("image replacement") || !cache().close()) return false;
    mounted = announced = reported_error = false;
    return true;
}

bool dorado_disk_mount(fileTYPE *file, unsigned index, bool writable, bool io_ver) {
    version = io_ver;
    // No fallback for unsupported images in the opt-in path: falling through
    // would silently restore blocking file IO during normal polling.
    struct stat st;
    if (mounted || index != 0 || !file->filp || file->zip || file->type == 2 ||
        fstat(fileno(file->filp), &st) || !S_ISREG(st.st_mode) ||
        file->size <= 0 || uint64_t(file->size) != uint64_t(st.st_size) ||
        uint64_t(file->size) > DoradoDiskCache::MaxImage || file->size % 512) {
        fprintf(stderr, "Dorado disk: cache needs one regular sector-aligned image <=64MiB on slot0\n");
        return false;
    }
    const int fd = dup(fileno(file->filp));
    if (fd < 0) return false;
    if (!install_exit_handler()) { ::close(fd); return false; }
    if (!cache().start(fd, file->size, writable)) { ::close(fd); return false; }
    mounted = true; announced = reported_error = false; writable_image = writable;
    trace().record("mount",cache().status().generation,file->size,writable);
    notify(0, false); // No guest request can time out while async preload runs.
    cache().poll();
    return true;
}

void dorado_disk_poll() {
    if (pending_exit) {
        int signal=pending_exit; pending_exit=0;
        if (dorado_disk_barrier("process termination") && dorado_net_barrier("process termination")) {
            offload_stop();
            ::_exit(128+signal);
        }
    }
    if (!mounted) return;
    static uint64_t last=0, next=0;
    const uint64_t now=DoradoDiskTrace::now();
    if (last && now-last > 20000000) trace().record("poll_gap",0,0,0,now-last);
    if (now>=next) { trace().record("poll"); next=now+1000000000; }
    last=now;
    cache().poll();
    const auto s = cache().status();
    if (s.state == DoradoDiskCache::State::Ready && !announced) {
        notify(s.size, writable_image);
        announced = true;
        printf("Dorado disk: image cache ready, %llu bytes, generation %llu\n",
               (unsigned long long)s.size, (unsigned long long)s.generation);
    }
    if (s.state == DoradoDiskCache::State::Error && !reported_error) {
        reported_error = true;
        fprintf(stderr, "Dorado disk: IO error %d; requests held, dirty data retained, lifecycle barriers blocked\n", s.error);
    }
}

int dorado_disk_service(int disk, int op, uint64_t lba, unsigned block_size,
                        unsigned bytes, int ack, int wide) {
    if (!dorado_disk_enabled()) return 0;
    if (!mounted || !announced || disk != 0 || !op) return -1;
    if (block_size != 512 || !bytes || bytes > 16384 || lba > UINT64_MAX / block_size) return -1;
    const auto s = cache().status();
    const uint64_t offset = lba * block_size;
    using Result = DoradoDiskCache::Result;
    if (op == 2) {
        Result r = cache().capture_write(s.generation, offset, bytes, [&](uint8_t *dst) {
            trace().record("write_request",offset,bytes);
            EnableIO();
            spi_w(UIO_SECTOR_WR | ack);
            spi_block_read(dst, wide, bytes);
            DisableIO();
            trace().record("write_ack",offset,bytes);
        });
        return r == Result::Done ? 1 : -1;
    }
    if (op & 1) {
        std::array<uint8_t, 16384> buffer;
        if (cache().read(s.generation, offset, buffer.data(), bytes) != Result::Done) return -1;
        trace().record("read_request",offset,bytes);
        EnableIO();
        spi_w(UIO_SECTOR_RD | ack);
        spi_block_write(buffer.data(), wide, bytes);
        DisableIO();
        trace().record("read_ack",offset,bytes);
        return 1;
    }
    return -1;
}
