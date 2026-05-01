#include "io.h"

#include <string.h>

void dorado_io_init(dorado_io *io)
{
    memset(io, 0, sizeof *io);
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
    if (out_bad_parity) *out_bad_parity = 0;
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
    return dev->read(dev->ctx, task, tioa, out_bad_parity);
}

void dorado_io_write(dorado_io *io, int task, uint8_t tioa, uint16_t data)
{
    if (!io || task < 0 || task >= DORADO_IO_TASKS) return;
    const dorado_io_device *dev = io->cells[task][tioa];
    if (!dev || !dev->write) return;
    dev->write(dev->ctx, task, tioa, data);
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
