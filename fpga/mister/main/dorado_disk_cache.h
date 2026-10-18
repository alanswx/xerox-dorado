#ifndef DORADO_DISK_CACHE_H
#define DORADO_DISK_CACHE_H

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <sys/types.h>
#include <vector>

// ARM image cache only. Main owns SPI; callbacks here must never touch SPI.
// One fixed-size image, one worker job, and one bounded write snapshot.
class DoradoDiskCache {
public:
    static constexpr size_t MaxImage = 64u * 1024 * 1024;
    static constexpr size_t Sector = 512;
    static constexpr size_t Run = 64u * 1024;
    enum class State { Empty, Loading, Ready, Error };
    enum class Result { Done, Wait, Invalid, Failed };
    struct Status {
        State state;
        uint64_t generation, size, dirty_sectors, accepted_bytes, written_bytes;
        bool job_pending, needs_sync;
        int error;
    };
    struct IO {
        std::function<ssize_t(int, void *, size_t, off_t)> read;
        std::function<ssize_t(int, const void *, size_t, off_t)> write;
        std::function<int(int)> sync;
    };
    using Submit = std::function<bool(std::function<void()>)>;

    explicit DoradoDiskCache(Submit submit, IO io = posix_io());
    ~DoradoDiskCache();
    DoradoDiskCache(const DoradoDiskCache &) = delete;
    DoradoDiskCache &operator=(const DoradoDiskCache &) = delete;

    // Main-thread API. Takes ownership only on success; caller has already
    // checked a regular, fixed-size file. Advertise the image only after Ready.
    bool start(int owned_fd, uint64_t size, bool writable);
    void poll();
    Result read(uint64_t generation, uint64_t offset, void *dst, size_t bytes);
    Result write(uint64_t generation, uint64_t offset, const void *src, size_t bytes);
    // Reserve RAM/error ownership before Main acknowledges an FPGA write.
    // capture executes bounded SPI capture only, while the short cache lock
    // prevents a worker error from invalidating an already consumed request.
    Result capture_write(uint64_t generation, uint64_t offset, size_t bytes,
                         const std::function<void(uint8_t *)> &capture);
    Status status() const;
    // Explicit recovery access after an IO error, never normal disk service.
    Result read_retained(uint64_t generation, uint64_t offset, void *dst, size_t bytes);
    // Explicit lifecycle boundary, never the regular polling path. Must run
    // BEFORE offload_stop(), FPGA programming or descriptor replacement.
    // False retains the image and all failed dirty data; caller must abort.
    bool drain();
    bool close();
    static IO posix_io();

private:
    void load_job();
    void write_job();
    void sync_job();
    void finish_error(int error);
    Result check(uint64_t generation, uint64_t offset, size_t bytes) const;
    Submit submit_;
    IO io_;
    mutable std::mutex lock_;
    std::condition_variable changed_;
    State state_ = State::Empty;
    int fd_ = -1, error_ = 0;
    bool writable_ = false, queued_ = false, needs_sync_ = false;
    uint64_t generation_ = 0, size_ = 0, dirty_count_ = 0;
    uint64_t accepted_ = 0, written_ = 0;
    std::vector<uint8_t> image_, dirty_;
};
#endif
