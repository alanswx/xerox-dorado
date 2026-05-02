#ifndef DORADO_FASTIO_H_
#define DORADO_FASTIO_H_

#include "memory.h"
#include "display.h"
#include "disk.h"

/*
 * Fast I/O dispatcher (HM §8). One callback that memory_ref invokes
 * on every IOFetch / IOStore. Dispatches to the right device based
 * on task ID:
 *   task 11₈ (AWT) / 13₈ (DWT) → display FIFO (subtask 0/2)
 *   task 14₈ (DSK) → disk controller FIFO
 *   anything else  → no-op
 *
 * Wire it via:
 *
 *   dorado_fastio_router router;
 *   dorado_fastio_router_init(&router, &display, &disk_ctl);
 *   mem.fast_io_cb  = dorado_fastio_dispatch;
 *   mem.fast_io_ctx = &router;
 */

typedef struct {
    dorado_display         *display;
    dorado_disk_controller *disk_ctl;

    /* Diagnostics (gap G1). Real hardware Holds the processor when a
     * destination FIFO is full or there is no destination at all.
     * We don't model Hold yet — these counters at least make silent
     * drops detectable. */
    uint32_t drops_display_fifo_full;   /* DWT/AWT IOFetch: display FIFO full */
    uint32_t drops_disk_fifo_full;      /* DSK IOFetch: disk FIFO full */
    uint32_t drops_disk_fifo_empty;     /* DSK IOStore: disk FIFO empty (zero-padded) */
    uint32_t drops_unrouted_iofetch;    /* IOFetch from a task with no device */
    uint32_t drops_unrouted_iostore;    /* IOStore from a task with no device */
} dorado_fastio_router;

void dorado_fastio_router_init(dorado_fastio_router *r,
                               dorado_display *display,
                               dorado_disk_controller *disk_ctl);

void dorado_fastio_dispatch(struct dorado_memory *mem,
                            dorado_ref_kind kind,
                            int task, int subtask, uint32_t va,
                            uint16_t munch[16], void *ctx);

#endif
