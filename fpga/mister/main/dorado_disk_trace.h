#ifndef DORADO_DISK_TRACE_H
#define DORADO_DISK_TRACE_H
#include <array>
#include <cstdint>
#include <mutex>

// Bounded, monotonic trace. Main only appends in RAM. The disk worker flushes
// records to the optional file; file latency never enters Main's poll path.
class DoradoDiskTrace {
public:
    DoradoDiskTrace();
    ~DoradoDiskTrace();
    static uint64_t now();
    void record(const char *kind, uint64_t offset=0, uint64_t bytes=0,
                int64_t result=0, uint64_t duration=0);
    void flush(); // Worker or an already drained explicit lifecycle boundary.
private:
    struct Event { uint64_t time, offset, bytes, duration; int64_t result; const char *kind; };
    static constexpr size_t Capacity = 8192;
    std::array<Event, Capacity> events_;
    std::mutex lock_;
    size_t head_ = 0, count_ = 0;
    uint64_t dropped_ = 0;
    int fd_ = -1;
};
#endif
