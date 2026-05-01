#ifndef DORADO_IO_H
#define DORADO_IO_H

#include <stdint.h>

/*
 * Slow I/O bus routing layer (HM §7).
 *
 * The Dorado has a single IOB (16-bit data + 2 parity) bus shared
 * across all peripherals. Each task has its own TIOA register (8 bits)
 * which selects "this task's" device address. The microcode functions
 *
 *   Pd ← Input        (FA=0 FB=3 FC=2)  — read IOB, check parity
 *   Pd ← InputNoPE    (FA=0 FB=3 FC=3)  — read IOB, no parity check
 *   Output ← B        (FA=0 FB=3 FC=6)  — drive IOB from B
 *
 * dispatch through this layer.
 *
 * Devices register handlers for (task, TIOA) cells. With no device
 * mapped, reads return 0xFFFF (real hardware: IOB pulls high) and
 * `bad_parity = 1` (no device → no valid parity bits driven).
 *
 * HM §7 says TIOA is task-specific. Most devices belong to a single
 * task (Disk → 14₈, Display → DDC tasks, BaseBoard fault registers →
 * various). We index the table by (task, TIOA). A handler can be
 * shared across tasks by registering for each.
 *
 * Sequencing rule (HM p. 86): Pd←Input / Pd←InputNoPE / Output←B can
 * be issued in the instruction immediately following TIOA←B. We don't
 * enforce this — microcode is responsible for correct sequencing.
 */

#define DORADO_IO_TASKS  16
#define DORADO_IO_ADDRS  256

typedef uint16_t (*dorado_io_read_fn)(void *ctx, int task, uint8_t tioa,
                                      int *out_bad_parity);
typedef void     (*dorado_io_write_fn)(void *ctx, int task, uint8_t tioa,
                                       uint16_t data);
typedef int      (*dorado_io_attention_fn)(void *ctx, int task, uint8_t tioa);

typedef struct {
    dorado_io_read_fn  read;
    dorado_io_write_fn write;
    dorado_io_attention_fn attention;
    void              *ctx;
    const char        *name;
} dorado_io_device;

typedef struct dorado_io {
    /* Indexed [task][tioa]. Sparse — most cells are NULL. */
    const dorado_io_device *cells[DORADO_IO_TASKS][DORADO_IO_ADDRS];
} dorado_io;

void dorado_io_init(dorado_io *io);

/* Register a device on (task, tioa). Pass dev=NULL to unregister.
 * The device pointer must outlive the io. */
void dorado_io_register(dorado_io *io, int task, uint8_t tioa,
                        const dorado_io_device *dev);

/* Convenience: register a device on every task at the given TIOA.
 * Used by the BaseBoard which responds regardless of the running task
 * (the BB drives MCPBus / CPReg, not slow-IO via TIOA, but for fault
 * registers and similar we route this way). */
void dorado_io_register_all_tasks(dorado_io *io, uint8_t tioa,
                                  const dorado_io_device *dev);

/* Read IOB at (task, TIOA). Returns 16-bit data; sets *out_bad_parity
 * to 1 if no device responded (floating bus) or the device flagged a
 * parity error. */
uint16_t dorado_io_read(dorado_io *io, int task, uint8_t tioa,
                        int *out_bad_parity);

/* Write IOB at (task, TIOA). No-op if no device is registered. */
void dorado_io_write(dorado_io *io, int task, uint8_t tioa, uint16_t data);

/* True if a device with a write handler is registered at (task, TIOA). */
int dorado_io_has_write(const dorado_io *io, int task, uint8_t tioa);

/* True if the device currently asserts IOAttention for (task, TIOA). */
int dorado_io_attention(dorado_io *io, int task, uint8_t tioa);

#endif
