#ifndef DORADO_SERVICE_CLOCK_H
#define DORADO_SERVICE_CLOCK_H
#include <stdint.h>

/* Local service time for retry/probe/broadcast deadlines. Paused wall time
 * never becomes retry time after CONTINUE. This is not the guest clock or
 * the Pup wall-clock time service. A restored process must rebase wall_ns. */
typedef struct {
    uint64_t wall_ns,logical_ns;
    int initialized,paused;
} dorado_service_clock;

static inline uint64_t dorado_service_clock_sample(dorado_service_clock *c,
                                                   uint64_t wall_ns,int paused) {
    if(c->initialized && !c->paused && wall_ns>=c->wall_ns)
        c->logical_ns+=wall_ns-c->wall_ns;
    c->wall_ns=wall_ns;c->paused=!!paused;c->initialized=1;
    return c->logical_ns;
}
#endif
