#include "dorado_disk_cache.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <new>
#include <unistd.h>

constexpr size_t DoradoDiskCache::MaxImage;
constexpr size_t DoradoDiskCache::Sector;
constexpr size_t DoradoDiskCache::Run;

DoradoDiskCache::IO DoradoDiskCache::posix_io() {
    return {[](int fd, void *p, size_t n, off_t o) { return ::pread(fd, p, n, o); },
            [](int fd, const void *p, size_t n, off_t o) { return ::pwrite(fd, p, n, o); },
            [](int fd) { return ::fsync(fd); }};
}

DoradoDiskCache::DoradoDiskCache(Submit submit, IO io)
    : submit_(std::move(submit)), io_(std::move(io)) {}

DoradoDiskCache::~DoradoDiskCache() {
    // The owner must explicitly drain before destroying/replacing the cache.
    // Error data is retained throughout its lifetime; never silently drain or
    // discard acknowledged writes as a side effect of ordinary polling.
    assert(!queued_);
    assert(state_ == State::Empty || state_ == State::Error);
    if (fd_ >= 0) ::close(fd_);
}

bool DoradoDiskCache::start(int owned_fd, uint64_t size, bool writable) {
    std::lock_guard<std::mutex> guard(lock_);
    if (state_ != State::Empty || snapshot_requested_ || owned_fd < 0 || !size || size > MaxImage || size % Sector)
        return false;
    fd_ = owned_fd;
    size_ = size;
    writable_ = writable;
    error_ = 0;
    dirty_count_ = accepted_ = written_ = 0;
    needs_sync_ = false;
    ++generation_;
    state_ = State::Loading;
    return true;
}

void DoradoDiskCache::poll() {
    std::lock_guard<std::mutex> guard(lock_);
    if (queued_ || state_ == State::Empty || state_ == State::Error) return;
    std::function<void()> job;
    if (state_ == State::Loading) job = [this] { load_job(); };
    else if (dirty_count_) job = [this] { write_job(); };
    else if (needs_sync_) job = [this] { sync_job(); };
    else return;
    queued_ = true;
    if (!submit_(std::move(job))) queued_ = false;
}

void DoradoDiskCache::finish_error(int error) {
    error_ = error ? error : EIO;
    state_ = State::Error;
    queued_ = false;
    changed_.notify_all();
}

void DoradoDiskCache::load_job() {
    std::vector<uint8_t> image, dirty;
    int failure = 0;
    try {
        image.resize(size_);
        dirty.resize(size_ / Sector, 0);
        size_t done = 0;
        while (done < size_) {
            const size_t wanted = std::min<size_t>(Run, size_ - done);
            ssize_t n = io_.read(fd_, image.data() + done, wanted, done);
            if (n < 0 && errno == EINTR) continue;
            if (n <= 0 || size_t(n) > wanted) { failure = n < 0 ? errno : EIO; break; }
            done += n;
        }
    } catch (const std::bad_alloc &) { failure = ENOMEM; }
    std::lock_guard<std::mutex> guard(lock_);
    if (failure) { finish_error(failure); return; }
    image_ = std::move(image);
    dirty_ = std::move(dirty);
    state_ = State::Ready;
    queued_ = false;
    changed_.notify_all();
}

DoradoDiskCache::Result DoradoDiskCache::check(uint64_t generation, uint64_t offset, size_t bytes) const {
    if (generation != generation_ || !bytes || offset > size_ || bytes > size_ - offset)
        return Result::Invalid;
    if (state_ == State::Error) return Result::Failed;
    if (state_ != State::Ready) return Result::Wait;
    return Result::Done;
}

DoradoDiskCache::Result DoradoDiskCache::read(uint64_t generation, uint64_t offset, void *dst, size_t bytes) {
    std::lock_guard<std::mutex> guard(lock_);
    Result r = check(generation, offset, bytes);
    if (r != Result::Done) return r;
    if (!dst || bytes > Run) return Result::Invalid;
    if (snapshot_requested_) return Result::Wait;
    std::memcpy(dst, image_.data() + offset, bytes);
    return Result::Done;
}

DoradoDiskCache::Result DoradoDiskCache::write(uint64_t generation, uint64_t offset, const void *src, size_t bytes) {
    if (!src) return Result::Invalid;
    return capture_write(generation, offset, bytes,
                         [src, bytes](uint8_t *dst) { std::memcpy(dst, src, bytes); });
}

DoradoDiskCache::Result DoradoDiskCache::capture_write(uint64_t generation, uint64_t offset, size_t bytes,
                                                     const std::function<void(uint8_t *)> &capture) {
    std::lock_guard<std::mutex> guard(lock_);
    Result r = check(generation, offset, bytes);
    if (r != Result::Done) return r;
    if (!writable_ || !capture || bytes > Run) return Result::Invalid;
    if (snapshot_requested_) return Result::Wait;
    // Full-sector snapshots preserve unchanged neighboring bytes even when
    // the caller's write is unaligned. Dirty RAM itself reserves all capacity.
    capture(image_.data() + offset);
    const size_t first = offset / Sector, last = (offset + bytes - 1) / Sector;
    for (size_t i = first; i <= last; ++i)
        if (!dirty_[i]) { dirty_[i] = 1; ++dirty_count_; }
    accepted_ += bytes;
    return Result::Done;
}

void DoradoDiskCache::write_job() {
    std::array<uint8_t, Run> snapshot;
    size_t first, count;
    {
        std::lock_guard<std::mutex> guard(lock_);
        first = std::find(dirty_.begin(), dirty_.end(), uint8_t(1)) - dirty_.begin();
        assert(first < dirty_.size());
        count = 0;
        while (first + count < dirty_.size() && count < Run / Sector && dirty_[first + count]) {
            dirty_[first + count++] = 0;
            --dirty_count_;
        }
        std::memcpy(snapshot.data(), image_.data() + first * Sector, count * Sector);
    }
    size_t done = 0, bytes = count * Sector;
    int failure = 0;
    while (done < bytes) {
        const size_t wanted = bytes - done;
        ssize_t n = io_.write(fd_, snapshot.data() + done, wanted, first * Sector + done);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0 || size_t(n) > wanted) { failure = n < 0 ? errno : EIO; break; }
        done += n;
    }
    std::lock_guard<std::mutex> guard(lock_);
    needs_sync_ = true;
    if (failure) {
        for (size_t i = first; i < first + count; ++i)
            if (!dirty_[i]) { dirty_[i] = 1; ++dirty_count_; }
        finish_error(failure);
        return;
    }
    written_ += bytes;
    queued_ = false;
    changed_.notify_all();
}

void DoradoDiskCache::sync_job() {
    int r;
    do { r = io_.sync(fd_); } while (r < 0 && errno == EINTR);
    const int failure = r ? errno : 0;
    std::lock_guard<std::mutex> guard(lock_);
    if (r) { finish_error(failure); return; }
    // Writes accepted while syncing are dirty and will require a later sync.
    needs_sync_ = false;
    queued_ = false;
    changed_.notify_all();
}

DoradoDiskCache::Status DoradoDiskCache::status() const {
    std::lock_guard<std::mutex> guard(lock_);
    return {state_, generation_, size_, dirty_count_, accepted_, written_, queued_, needs_sync_, error_,
            snapshot_requested_, snapshot_boundary(generation_) == Result::Done};
}

DoradoDiskCache::Result DoradoDiskCache::snapshot_boundary(uint64_t generation) const {
    if (generation != generation_ || state_ == State::Empty) return Result::Invalid;
    if (state_ == State::Error) return Result::Failed;
    if (!snapshot_requested_) return Result::Invalid;
    return state_ == State::Ready && !queued_ && !dirty_count_ && !needs_sync_
        ? Result::Done : Result::Wait;
}

DoradoDiskCache::Result DoradoDiskCache::snapshot_pause(uint64_t generation) {
    std::lock_guard<std::mutex> guard(lock_);
    if (generation != generation_ || state_ == State::Empty) return Result::Invalid;
    if (state_ == State::Error) return Result::Failed;
    snapshot_requested_ = true;
    return snapshot_boundary(generation);
}

DoradoDiskCache::Result DoradoDiskCache::snapshot_ready(uint64_t generation) const {
    std::lock_guard<std::mutex> guard(lock_);
    return snapshot_boundary(generation);
}

DoradoDiskCache::Result DoradoDiskCache::snapshot_read(uint64_t generation, uint64_t offset,
                                                     void *dst, size_t bytes) const {
    std::lock_guard<std::mutex> guard(lock_);
    Result r = check(generation, offset, bytes);
    if (r != Result::Done) return r;
    if (!dst || bytes > Run) return Result::Invalid;
    r = snapshot_boundary(generation);
    if (r != Result::Done) return r;
    std::memcpy(dst, image_.data() + offset, bytes);
    return Result::Done;
}

DoradoDiskCache::Result DoradoDiskCache::snapshot_continue(uint64_t generation) {
    std::lock_guard<std::mutex> guard(lock_);
    if (generation != generation_ || state_ == State::Empty) return Result::Invalid;
    if (state_ == State::Error) return Result::Failed;
    if (!snapshot_requested_) return state_ == State::Ready ? Result::Done : Result::Wait;
    Result r = snapshot_boundary(generation);
    if (r == Result::Done) snapshot_requested_ = false;
    return r;
}

DoradoDiskCache::Result DoradoDiskCache::read_retained(uint64_t generation, uint64_t offset, void *dst, size_t bytes) {
    std::lock_guard<std::mutex> guard(lock_);
    if (state_ != State::Error || image_.size() != size_) return Result::Failed;
    if (generation != generation_ || !dst || !bytes || bytes > Run ||
        offset > size_ || bytes > size_ - offset) return Result::Invalid;
    std::memcpy(dst, image_.data() + offset, bytes);
    return Result::Done;
}

bool DoradoDiskCache::drain() {
    for (;;) {
        poll();
        std::unique_lock<std::mutex> guard(lock_);
        if (state_ == State::Error) return false;
        if (state_ == State::Empty || (state_ == State::Ready && !queued_ && !dirty_count_ && !needs_sync_))
            return true;
        // Also retry a rejected enqueue after unrelated offload work completes.
        changed_.wait_for(guard, std::chrono::milliseconds(10));
    }
}

bool DoradoDiskCache::close() {
    {
        std::lock_guard<std::mutex> guard(lock_);
        if (snapshot_requested_) return false; // Retain snapshot file/generation ownership.
    }
    if (!drain()) return false;
    std::lock_guard<std::mutex> guard(lock_);
    if (fd_ >= 0) ::close(fd_);
    fd_ = -1;
    image_.clear(); image_.shrink_to_fit();
    dirty_.clear(); dirty_.shrink_to_fit();
    state_ = State::Empty;
    size_ = 0;
    return true;
}
