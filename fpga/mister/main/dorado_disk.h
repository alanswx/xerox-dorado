#ifndef DORADO_DISK_H
#define DORADO_DISK_H
#include <cstdint>
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
#endif
