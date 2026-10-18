// Real pthread offload worker + actual temporary files. No FPGA/SPI mocking
// can qualify Main integration; these tests qualify the cache/worker boundary.
#include "offload.cpp"
#include "dorado_disk_cache.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <thread>
#include <unistd.h>
#include <vector>

using Cache = DoradoDiskCache;
using Clock = std::chrono::steady_clock;
using Result = Cache::Result;
static void check(bool ok, const char *why) {
    if (!ok) { fprintf(stderr, "DISK-CACHE FAIL: %s\n", why); std::_Exit(23); }
}
static Cache::Submit submit = [](std::function<void()> job) { return offload_try_add_work(std::move(job)); };
static std::vector<uint8_t> pattern(size_t bytes) {
    std::vector<uint8_t> p(bytes);
    uint32_t x = 0x12fa963b;
    for (auto &v : p) { x ^= x << 13; x ^= x >> 17; x ^= x << 5; v = x; }
    return p;
}
struct File {
    char path[64] = "/tmp/dorado-disk-cache-XXXXXX";
    int fd;
    explicit File(const std::vector<uint8_t> &p) {
        fd = mkstemp(path); check(fd >= 0, "mkstemp");
        check(::write(fd, p.data(), p.size()) == ssize_t(p.size()), "seed write");
    }
    explicit File(size_t size) {
        fd = mkstemp(path); check(fd >= 0 && !ftruncate(fd, size), "sparse image");
    }
    ~File() { ::close(fd); ::unlink(path); }
    int owned() { int f = dup(fd); check(f >= 0, "dup"); return f; }
    void verify(const std::vector<uint8_t> &want) {
        std::vector<uint8_t> got(want.size());
        check(pread(fd, got.data(), got.size(), 0) == ssize_t(got.size()), "physical readback");
        check(got == want, "physical image differs from independent oracle");
    }
};
template <typename P> static void until(Cache &cache, P done, const char *why) {
    const auto end = Clock::now() + std::chrono::seconds(12);
    while (!done()) {
        cache.poll();
        check(Clock::now() < end, why);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
static uint64_t ready(Cache &c) {
    until(c, [&] { return c.status().state == Cache::State::Ready; }, "load timeout");
    return c.status().generation;
}
static void verify_ram(Cache &c, uint64_t g, const std::vector<uint8_t> &want) {
    std::array<uint8_t, Cache::Run> got;
    for (size_t o = 0; o < want.size(); o += got.size()) {
        size_t n = std::min(got.size(), want.size() - o);
        check(c.read(g, o, got.data(), n) == Result::Done, "RAM read");
        check(!memcmp(got.data(), want.data() + o, n), "RAM/neighbor coherence");
    }
}

static void slow_write_test() {
    auto oracle = pattern(256 * 1024);
    File file(oracle);
    std::atomic<bool> entered{false};
    auto io = Cache::posix_io();
    io.write = [&](int fd, const void *p, size_t n, off_t o) -> ssize_t {
        if (!entered.exchange(true)) {
            std::vector<uint8_t> owned(static_cast<const uint8_t *>(p), static_cast<const uint8_t *>(p) + n);
            std::this_thread::sleep_for(std::chrono::milliseconds(4200));
            check(!memcmp(p, owned.data(), n), "worker write buffer changed after submission");
        }
        return pwrite(fd, p, n, o);
    };
    Cache c(submit, io);
    check(c.start(file.owned(), oracle.size(), true), "start");
    auto g = ready(c);
    auto data = pattern(1024);
    check(c.write(g, 0, data.data(), data.size()) == Result::Done, "initial write");
    std::copy(data.begin(), data.end(), oracle.begin());
    until(c, [&] { return entered.load(); }, "write did not start");
    unsigned polls = 0; double max_call = 0, max_gap = 0;
    const auto start = Clock::now(); auto previous = start;
    while (Clock::now() - start < std::chrono::milliseconds(4000)) {
        auto before = Clock::now();
        // Odd offsets and overlapping changes cover bytes on both sides of
        // sector boundaries while the worker owns an older immutable snapshot.
        const size_t offset = (polls * 997u) % (oracle.size() - data.size());
        data[polls % data.size()] ^= uint8_t(polls | 1);
        check(c.capture_write(g, offset, data.size(), [&](uint8_t *dst) {
            memcpy(dst, data.data(), data.size());
        }) == Result::Done, "capture blocked/rejected during slow file write");
        std::copy(data.begin(), data.end(), oracle.begin() + offset);
        std::array<uint8_t, 1024> got;
        check(c.read(g, offset, got.data(), got.size()) == Result::Done &&
              !memcmp(got.data(), data.data(), got.size()), "read behind slow write");
        c.poll();
        auto after = Clock::now();
        max_call = std::max(max_call, std::chrono::duration<double, std::milli>(after-before).count());
        max_gap = std::max(max_gap, std::chrono::duration<double, std::milli>(before-previous).count());
        previous = before; ++polls;
        check(max_call < 100 && max_gap < 100, "4.2s file IO blocked polling");
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    check(polls > 100, "heartbeat did not advance");
    verify_ram(c, g, oracle);
    check(c.close(), "slow drain"); file.verify(oracle);
    // Reopen is actual file IO into a new generation, not a cached read.
    check(c.start(file.owned(), oracle.size(), true), "remount");
    auto g2 = ready(c); check(g2 != g, "generation reused");
    bool consumed = false;
    check(c.capture_write(g, 0, 512, [&](uint8_t *) { consumed = true; }) == Result::Invalid && !consumed,
          "stale completion consumed/ACKed data");
    verify_ram(c, g2, oracle); check(c.close(), "remount close");
    printf("DISK-CACHE SLOW PASS: 4.2s write, %u poll/read/write heartbeats, max call %.3fms, max gap %.3fms; full file close/remount oracle\n",
           polls, max_call, max_gap);
}

static void queue_and_coalescing_test() {
    auto oracle = pattern(256 * 1024); File file(oracle);
    std::atomic<bool> entered{false}, release{false};
    check(submit([&] { entered = true; while (!release) std::this_thread::yield(); }), "queue blocker");
    while (!entered) std::this_thread::yield();
    for (int i = 0; i < 7; ++i) check(submit([] {}), "fill queue");
    Cache c(submit); check(c.start(file.owned(), oracle.size(), true), "queue start");
    c.poll(); auto s = c.status();
    check(s.state == Cache::State::Loading && !s.job_pending, "full queue submitted load");
    bool consumed = false;
    check(c.capture_write(s.generation, 0, 512, [&](uint8_t *) { consumed = true; }) == Result::Wait && !consumed,
          "loading image consumed an FPGA write");
    release = true; auto g = ready(c);
    const int rejected_fd = file.owned();
    check(!c.start(rejected_fd, oracle.size(), true), "remount replaced live descriptor"); ::close(rejected_fd);
    auto track = pattern(31 * 512);
    for (unsigned i = 0; i < 100; ++i) {
        track[i] ^= 0xff;
        check(c.write(g, 6400, track.data(), track.size()) == Result::Done, "coalesced write");
    }
    std::copy(track.begin(), track.end(), oracle.begin()+6400);
    check(c.drain(), "coalesced drain");
    s = c.status();
    check(s.accepted_bytes == 100 * track.size() && s.written_bytes == 32 * 512 && !s.needs_sync,
          "repeated-track coalescing/unaligned sector coverage");
    verify_ram(c, g, oracle); check(c.close(), "coalesced close"); file.verify(oracle);
    printf("DISK-CACHE QUEUE PASS: full eight-slot queue defers load/ACK; 100 overlapping track writes -> 32 physical sectors; live remount rejected\n");
}

static void short_io_and_sync_test() {
    auto oracle = pattern(128 * 1024); File file(oracle);
    auto io = Cache::posix_io();
    unsigned reads = 0, writes = 0;
    std::atomic<bool> syncing{false}, release{false};
    std::atomic<unsigned> syncs{0};
    io.read = [&](int fd, void *p, size_t n, off_t o) -> ssize_t {
        if (!reads++) { errno = EINTR; return -1; }
        return pread(fd, p, std::min<size_t>(n, 137), o);
    };
    io.write = [&](int fd, const void *p, size_t n, off_t o) -> ssize_t {
        if (!writes++) { errno = EINTR; return -1; }
        return pwrite(fd, p, std::min<size_t>(n, 257), o);
    };
    io.sync = [&](int fd) {
        if (!syncs++) { syncing = true; while (!release) std::this_thread::yield(); }
        return fsync(fd);
    };
    Cache c(submit, io); check(c.start(file.owned(), oracle.size(), true), "short IO start"); auto g=ready(c);
    uint8_t value = 0xee;
    check(c.write(g, 511, &value, 1) == Result::Done, "short IO write"); oracle[511] = value;
    until(c, [&] { return syncing.load(); }, "sync not started");
    value = 0x72;
    check(c.write(g, 512, &value, 1) == Result::Done, "write during sync blocked"); oracle[512] = value;
    release = true;
    check(c.close() && syncs >= 2, "write during sync lost/durable barrier missing"); file.verify(oracle);
    printf("DISK-CACHE SHORT IO PASS: EINTR/partial reads+writes, write during sync, repeated durable barrier\n");
}

static void error_test(bool sync_error) {
    auto oracle = pattern(64 * 1024); File file(oracle); auto io = Cache::posix_io();
    unsigned calls = 0;
    if (sync_error) io.sync = [](int) { errno = EIO; return -1; };
    else io.write = [&](int fd, const void *p, size_t n, off_t o) -> ssize_t {
        if (!calls++) return pwrite(fd, p, std::min<size_t>(n, 33), o);
        errno = ENOSPC; return -1;
    };
    Cache c(submit, io); check(c.start(file.owned(), oracle.size(), true), "error start"); auto g=ready(c);
    auto data = pattern(1031); std::fill(data.begin(), data.end(), 0x9c);
    check(c.write(g, 505, data.data(), data.size()) == Result::Done, "error prewrite");
    std::copy(data.begin(), data.end(), oracle.begin()+505);
    check(!c.drain() && !c.close(), "IO error silently reported as durable/closed");
    auto s = c.status();
    check(s.state == Cache::State::Error && !s.job_pending && s.error == (sync_error ? EIO : ENOSPC), "latched IO error");
    check(sync_error ? s.needs_sync : s.dirty_sectors == 3, "failed data not retained dirty");
    std::vector<uint8_t> recovered(oracle.size());
    check(c.read_retained(g, 0, recovered.data(), recovered.size()) == Result::Done && recovered == oracle,
          "acknowledged error data cannot be recovered");
    bool consumed = false;
    check(c.capture_write(g, 0, 512, [&](uint8_t *) { consumed = true; }) == Result::Failed && !consumed,
          "failed image ACKed another write");
    int replacement = file.owned();
    check(!c.start(replacement, oracle.size(), true), "failed dirty image replaced"); ::close(replacement);
    printf("DISK-CACHE ERROR PASS: %s preserves recovery image and rejects ACK/remount/drain success\n", sync_error ? "fsync EIO" : "partial write ENOSPC");
}

static void bounds_and_readonly_test() {
    auto oracle = pattern(64 * 1024); File file(oracle); Cache c(submit);
    int rejected = file.owned();
    check(!c.start(rejected, Cache::MaxImage+512, true) && !c.start(rejected, 513, true) &&
          !c.start(rejected, 0, true), "image capacity/geometry accepted"); ::close(rejected);
    check(c.start(file.owned(), oracle.size(), false), "readonly start"); auto g=ready(c);
    uint8_t b=0;
    check(c.read(g, UINT64_MAX, &b, 1) == Result::Invalid &&
          c.read(g, oracle.size(), &b, 1) == Result::Invalid &&
          c.read(g, 0, &b, 0) == Result::Invalid, "range/overflow accepted");
    bool consumed = false;
    check(c.capture_write(g, 0, 512, [&](uint8_t *) { consumed = true; }) == Result::Invalid && !consumed,
          "readonly ACKed a write");
    verify_ram(c,g,oracle); check(c.close(), "readonly close"); file.verify(oracle);
    printf("DISK-CACHE BOUNDS PASS: readonly, stale/out-of-range/overflow/zero/capacity/geometry rejected before capture\n");
}

static void capacity_test() {
    File file(Cache::MaxImage); Cache c(submit);
    check(c.start(file.owned(), Cache::MaxImage, true), "maximum image rejected"); auto g=ready(c);
    std::array<uint8_t, Cache::Run> p; p.fill(0x93);
    // No poll during these writes: every byte can become dirty without an
    // unbounded request queue, additional snapshots, or any file IO in Main.
    for (size_t o = 0; o < Cache::MaxImage; o += p.size())
        check(c.write(g,o,p.data(),p.size()) == Result::Done, "full-image dirty capacity");
    check(c.status().dirty_sectors == Cache::MaxImage/512, "dirty bound");
    check(c.close(), "full-image drain");
    std::array<uint8_t, Cache::Run> got;
    for (size_t o = 0; o < Cache::MaxImage; o += got.size())
        check(pread(file.fd,got.data(),got.size(),o) == ssize_t(got.size()) && got == p, "full-image durable data");
    printf("DISK-CACHE CAPACITY PASS: 64MiB image, all 131072 sectors dirty, bounded one-job/64KiB snapshot, complete physical oracle\n");
}

int main() {
    offload_start();
    slow_write_test(); queue_and_coalescing_test(); short_io_and_sync_test();
    error_test(false); error_test(true); bounds_and_readonly_test(); capacity_test();
    offload_stop();
    puts("DISK-CACHE PASS: cache/actual worker qualified; Main/SPI/lifecycle integration and FPGA Cedar stress remain pending");
}
