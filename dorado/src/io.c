#include "io.h"
#include "trace.h"

#include <stdio.h>
#include <string.h>

/* ---- Output<-B census, ACROSS ALL TASKS AND INCLUDING UNROUTED WRITES ----
 *
 * dorado_io_write_subtask() drops a write with no registered device on the
 * floor, silently. That is correct behaviour -- real hardware has nothing
 * listening either -- but it makes the machine opaque exactly where you most
 * want to see it: an address WE do not model looks identical to an address
 * the guest never touched.
 *
 * This cost a wrong answer on 2026-08-08. `DORADO_DDC_TIOA` counted inside
 * display.c, so it could only ever see writes that had already been routed to
 * the display -- and display.c registers TIOA 0360..0377 for the DISPLAY
 * TASKS only (3, 4, 011, 013). Cedar's ColorDisplayHeadDorado is Mesa code
 * calling DoradoInputOutput.Output from the EMULATOR task, so its writes to
 * the colour registers (361B/362B/365B) went to an unregistered cell and
 * vanished. The instrument reported "no world programs the colour side" when
 * what it had actually established was "no DISPLAY TASK does".
 *
 * File-scope statics, not dorado_io members: a new struct member changes the
 * snapshot ABI and every baked checkpoint fails to restore. Reset in
 * dorado_io_init. */
static uint64_t io_out_count[DORADO_IO_TASKS][256];
static uint16_t io_out_first[DORADO_IO_TASKS][256];
static uint16_t io_out_last[DORADO_IO_TASKS][256];
static uint8_t  io_out_routed[DORADO_IO_TASKS][256];

void dorado_io_init(dorado_io *io)
{
    memset(io, 0, sizeof *io);
    memset(io_out_count, 0, sizeof io_out_count);
    memset(io_out_first, 0, sizeof io_out_first);
    memset(io_out_last, 0, sizeof io_out_last);
    memset(io_out_routed, 0, sizeof io_out_routed);
}

void dorado_io_dump_output_census(void)
{
    fprintf(stderr, "[io] Output<-B census by (task, TIOA), all tasks:\n");
    unsigned unrouted = 0;
    for (int t = 0; t < DORADO_IO_TASKS; t++) {
        for (int a = 0; a < 256; a++) {
            if (io_out_count[t][a] == 0) continue;
            if (!io_out_routed[t][a]) unrouted++;
            fprintf(stderr,
                    "[io]   task %02o  tioa %03o  %12llu  first=%06o "
                    "last=%06o  %s\n",
                    t, a, (unsigned long long)io_out_count[t][a],
                    io_out_first[t][a], io_out_last[t][a],
                    io_out_routed[t][a] ? "" : "<- NO DEVICE (we drop these)");
        }
    }
    fprintf(stderr,
            "[io] %u (task,tioa) pair(s) written with no device registered. "
            "DispM colour lives at 361B/362B/365B.\n", unrouted);
}

void dorado_io_register(dorado_io *io, int task, uint8_t tioa,
                        const dorado_io_device *dev)
{
    if (task < 0 || task >= DORADO_IO_TASKS) return;
    io->cells[task][tioa] = dev;
}

void dorado_io_register_all_tasks(dorado_io *io, uint8_t tioa,
                                  const dorado_io_device *dev)
{
    for (int t = 0; t < DORADO_IO_TASKS; t++) {
        io->cells[t][tioa] = dev;
    }
}

uint16_t dorado_io_read(dorado_io *io, int task, uint8_t tioa,
                        int *out_bad_parity)
{
    return dorado_io_read_subtask(io, task, 0, tioa, out_bad_parity);
}

uint16_t dorado_io_read_subtask(dorado_io *io, int task, int subtask,
                                uint8_t tioa, int *out_bad_parity)
{
    if (out_bad_parity) *out_bad_parity = 0;
    subtask &= 3;
    if (!io || task < 0 || task >= DORADO_IO_TASKS) {
        if (out_bad_parity) *out_bad_parity = 1;
        return 0xFFFF;
    }
    const dorado_io_device *dev = io->cells[task][tioa];
    if (!dev || !dev->read) {
        /* Floating bus: real hardware pulls IOB high; with no device
         * driving parity, parity is undefined → flag as bad. HM §7
         * page 86: "IOB has bad parity if a nonexistent register is
         * selected." Boot probes use Pd←InputNoPE specifically to
         * tolerate this. */
        if (out_bad_parity) *out_bad_parity = 1;
        return 0xFFFF;
    }
    return dev->read(dev->ctx, task, subtask, tioa, out_bad_parity);
}

void dorado_io_write(dorado_io *io, int task, uint8_t tioa, uint16_t data)
{
    dorado_io_write_subtask(io, task, 0, tioa, data);
}

void dorado_io_write_subtask(dorado_io *io, int task, int subtask,
                             uint8_t tioa, uint16_t data)
{
    if (!io || task < 0 || task >= DORADO_IO_TASKS) return;
    subtask &= 3;
    const dorado_io_device *dev = io->cells[task][tioa];
    /* Census BEFORE the routing test, so an address we do not model is
     * distinguishable from one the guest never wrote. See the note above. */
    if (dorado_trace_flag("DORADO_IO_CENSUS")) {
        if (io_out_count[task][tioa] == 0) io_out_first[task][tioa] = data;
        io_out_count[task][tioa]++;
        io_out_last[task][tioa] = data;
        if (dev && dev->write) io_out_routed[task][tioa] = 1;
    }
    if (!dev || !dev->write) return;
    dev->write(dev->ctx, task, subtask, tioa, data);
}

int dorado_io_has_write(const dorado_io *io, int task, uint8_t tioa)
{
    if (!io || task < 0 || task >= DORADO_IO_TASKS) return 0;
    const dorado_io_device *dev = io->cells[task][tioa];
    return dev && dev->write;
}

int dorado_io_attention(dorado_io *io, int task, uint8_t tioa)
{
    if (!io || task < 0 || task >= DORADO_IO_TASKS) return 0;
    const dorado_io_device *dev = io->cells[task][tioa];
    if (!dev || !dev->attention) return 0;
    return dev->attention(dev->ctx, task, tioa) ? 1 : 0;
}
