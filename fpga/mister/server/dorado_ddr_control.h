#ifndef DORADO_DDR_CONTROL_H
#define DORADO_DDR_CONTROL_H
#include "dorado_ddr_ring.h"

/* Local fixed32-byte stream records, all integers little-endian:
 * magic64, command32, sequence32, epoch64, base32, size32.
 * Reply: same magic, command|0x80000000, sequence, epoch, errno32, active32.
 * A socket disconnect quiesces before another controller can take over. */
#define DORADO_CTRL_MAGIC UINT64_C(0x00314c5254434444) /* DDCTRL1 NUL */
#define DORADO_CTRL_STOP 1u
#define DORADO_CTRL_RESUME 2u
#define DORADO_CTRL_STATUS 3u
#define DORADO_CTRL_ACK UINT32_C(0x80000000)
#define DORADO_CTRL_BYTES 32u
#define DORADO_CTRL_RESUMED_EVENT 1u
#define DORADO_CTRL_STOPPED_EVENT 2u

typedef struct {
    /* Production callbacks validate/map the fixed physical aperture. Tests
     * supply guarded ordinary memory; no runtime fake-map environment flag. */
    int (*open)(void *opaque, volatile void **mapping);
    int (*close)(void *opaque, volatile void *mapping);
    void *opaque;
} dorado_ddr_mapper;

typedef struct {
    int listener,client,granted;
    char path[108];
    unsigned used,sent,pending,last_valid;
    unsigned char request[32],reply[32],last_request[32],last_reply[32];
    uint64_t last_epoch;
    volatile void *mapped;
    dorado_ddr_mapper mapper;
    dorado_ddr_ring ring;
} dorado_ddr_control;

int dorado_ddr_control_open(dorado_ddr_control *,const char *,dorado_ddr_mapper);
/* Call in the daemon's sole thread. Bounded to one request/pending reply.
 * A STOP reply is queued only after quiesce/MMIO completion and unmapping.
 * Mapping or cleanup errors are returned in the reply, never a success ACK. */
unsigned dorado_ddr_control_poll(dorado_ddr_control *);
int dorado_ddr_control_close(dorado_ddr_control *);
#endif
