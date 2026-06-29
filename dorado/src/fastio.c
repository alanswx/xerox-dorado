#include "fastio.h"

void dorado_fastio_router_init(dorado_fastio_router *r,
                               dorado_display *display,
                               dorado_disk_controller *disk_ctl)
{
    r->display  = display;
    r->disk_ctl = disk_ctl;
}

/*
 * Fast-IO dispatch. Called by memory_ref on IOFetch/IOStore.
 *
 * For IOFetch (memory→device, Fin bus): the `munch` buffer already
 * contains 16 words read from storage. Deliver them to the device.
 *
 * For IOStore (device→memory, Fout bus): fill `munch` with 16 words
 * the device wants to write. memory will then write them to storage.
 */
void dorado_fastio_dispatch(struct dorado_memory *mem,
                            dorado_ref_kind kind,
                            int task, int subtask, uint32_t va,
                            uint16_t munch[16], void *ctx)
{
    dorado_fastio_router *r = ctx;
    (void)mem; (void)va;

    switch (task) {
    case DORADO_DISPLAY_TASK_AWT:
    case DORADO_DISPLAY_TASK_DWT:
        if (kind == DM_REF_IOFETCH && r->display) {
            if (r->display->iofetch_count == 0) {
                r->display->first_iofetch_va = va;
                for (int i = 0; i < 16; i++) {
                    r->display->first_iofetch_words[i] = munch[i];
                }
            }
            r->display->last_iofetch_va = va;
            for (int i = 0; i < 16; i++) {
                r->display->last_iofetch_words[i] = munch[i];
            }
            /* Push 16 words into the display FIFO for the channel
             * selected by subtask (0 = A, 2 = B). DWT/AWT then
             * pump these into the framebuffer via the mixer or the
             * DispM terminal interface. */
            for (int i = 0; i < 16; i++) {
                if (dorado_display_iofetch_word(r->display, subtask,
                                                va + (uint32_t)i,
                                                munch[i]) != 0) {
                    /* FIFO full — drop the rest of the munch. Real
                     * hardware would Hold the processor (HM §8 /
                     * gap B1); we count the drop. */
                    r->drops_display_fifo_full++;
                    break;
                }
            }
        } else if (kind == DM_REF_IOSTORE) {
            /* IOStore from display word tasks is not used by DDC. */
            r->drops_unrouted_iostore++;
        }
        break;

    case 014:  /* DSK — disk task */
        if (kind == DM_REF_IOFETCH && r->disk_ctl) {
            /* IOFetch by DSK = controller pulls memory data for a
             * disk WRITE. Push 16 words into the controller's
             * write-FIFO. */
            for (int i = 0; i < 16; i++) {
                if (r->disk_ctl->fifo_count >= DORADO_DISK_FIFO_WORDS) {
                    r->drops_disk_fifo_full++;
                    break;
                }
                r->disk_ctl->fifo[r->disk_ctl->fifo_head] = munch[i];
                r->disk_ctl->fifo_head =
                    (r->disk_ctl->fifo_head + 1) % DORADO_DISK_FIFO_WORDS;
                r->disk_ctl->fifo_count++;
                r->disk_ctl->fifo_writes++;
            }
        } else if (kind == DM_REF_IOSTORE && r->disk_ctl) {
            /* IOStore by DSK = controller pushes disk-read data into
             * memory. Drain 16 words from the controller's read FIFO
             * into the munch buffer. */
            for (int i = 0; i < 16; i++) {
                if (r->disk_ctl->fifo_count > 0) {
                    munch[i] = r->disk_ctl->fifo[r->disk_ctl->fifo_tail];
                    r->disk_ctl->fifo_tail =
                        (r->disk_ctl->fifo_tail + 1) % DORADO_DISK_FIFO_WORDS;
                    r->disk_ctl->fifo_count--;
                    r->disk_ctl->fifo_reads++;
                    dorado_disk_controller_refill_fifo(r->disk_ctl);
                } else {
                    munch[i] = 0;     /* FIFO empty — pad with zeros */
                    r->drops_disk_fifo_empty++;
                }
            }
        }
        break;

    default:
        /* Other tasks aren't routed for fast IO yet (Ethernet, terminal
         * interface, etc.). Count the unrouted ref so probes can
         * detect missing routes per gap G1. */
        if (kind == DM_REF_IOFETCH) r->drops_unrouted_iofetch++;
        else if (kind == DM_REF_IOSTORE) r->drops_unrouted_iostore++;
        break;
    }
}
