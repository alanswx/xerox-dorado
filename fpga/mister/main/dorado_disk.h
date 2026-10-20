#ifndef DORADO_DISK_H
#define DORADO_DISK_H
#include <cstdint>
#include "dorado_disk_cache.h"
struct fileTYPE;
// Opt-in DORADO_DISK_CACHE=1 and original config-string name Dorado only.
bool dorado_disk_enabled();
bool dorado_disk_before_mount(unsigned index);
bool dorado_disk_mount(fileTYPE *file, unsigned index, bool writable, bool io_ver);
void dorado_disk_poll();
// 0: generic Main handler; 1: handled; -1: deferred/error, end this poll pass.
int dorado_disk_service(int disk, int op, uint64_t lba, unsigned block_size,
                        unsigned bytes, int ack, int wide);
// An explicit drain failure prevents programming/remount/restart/reboot.
bool dorado_disk_barrier(const char *reason);
// Signal-handler-safe request only; Main performs the drain in its poll loop.
bool dorado_disk_defer_exit(int signal);
// Also used by DDR networking when no cached disk has been mounted yet.
bool dorado_disk_prepare_exit();
// Snapshot coordinator API for a configured, mounted cache. The caller must
// first stop guest disk activity and finish already-issued FPGA disk requests,
// then hold the image-selection lifecycle. Otherwise deferring new SPI service
// could block a response the FPGA still needs. No file IO or SPI in these calls;
// poll drains accepted ARM work. Read is bounded to Cache::Run bytes.
DoradoDiskCache::Status dorado_disk_snapshot_status();
DoradoDiskCache::Result dorado_disk_snapshot_pause(uint64_t generation);
DoradoDiskCache::Result dorado_disk_snapshot_read(uint64_t generation, uint64_t offset,
                                                void *dst, size_t bytes);
DoradoDiskCache::Result dorado_disk_snapshot_continue(uint64_t generation);
#endif
