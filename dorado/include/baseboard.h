#ifndef DORADO_BASEBOARD_H
#define DORADO_BASEBOARD_H

#include <stddef.h>
#include <stdint.h>

/*
 * Dorado BaseBoard model.
 *
 * The BaseBoard is a small 6502 system that supervises the Dorado
 * mainframe: power sequencing, temperature, the boot button, the
 * green status LED, the boot ROM upload over CPReg, the watchdog,
 * and the Midas debug back-channel.
 *
 * Source: chm/dorado/expanded/doradobaserom.dm!12_/ (BCPL .masm files)
 * Canonical ROM image: chm/dorado/doradobaserom.mb!13 (MB-format dump
 * of the entire 64 K 6502 address space, post-link).
 *
 *   Address range     Contents
 *   ---------------------------------------------------
 *   0x0000–0x03FF     RAM (data sections + zero page + stack)
 *   0x0400–0x047F     6532 #1: DAC + CPIBus
 *   0x0480–0x04FF     6532 #2: Comparators + MiscByte (boot button, lamp)
 *   0x0500–0x057F     6532 #3: CPRegH/L  ★ Dorado interface
 *   0x0580–0x05FF     6532 #4: MCPBus (microcode bus to Dorado)
 *   0x0600–0x067F     6532 #5: Watchdog
 *   0xC000–0xCFFF     EPROMs: BootBlockTable, Boot1, Boot1BPAT
 *   0xD000–0xD7FF     EPROM C-12 (extension, 1987 add-on)
 *   0xF000–0xFFFF     EPROMs: BaseBoard 6502 code (Reset, Interrupt, …)
 *
 * The 6532 (RIOT — RAM/IO/Timer combo) chips have:
 *   - 128 bytes of internal RAM
 *   - Two 8-bit I/O ports (PA, PB) with direction registers
 *   - A timer with a divide-by-1/8/64/1024 prescaler
 *   - An interrupt-flag register
 *
 * For our model we ignore the chip-internal RAM (the BaseBoard ROM
 * appears to use a separate RAM region) and only emulate the I/O port
 * and timer behavior needed to keep the firmware happy.
 */

#define BASEBOARD_MEM_SIZE   0x10000   /* 64K 6502 address space */
#define BASEBOARD_NUM_RIOTS  5

typedef struct {
    /* Two 8-bit I/O ports. `external` represents what the world is
     * driving onto the input pins; `latch` represents what the CPU
     * has stored in the output latch. The visible value of an I/O
     * port is `(latch & ddr) | (external & ~ddr)`. */
    uint8_t pa_ddr, pa_latch, pa_external;
    uint8_t pb_ddr, pb_latch, pb_external;

    /* Timer: counts down at the configured prescaler. We model it
     * loosely — return whatever current_value the firmware expects
     * and decrement on baseboard_step(). */
    uint8_t  timer_value;
    uint16_t timer_prescaler;     /* 1, 8, 64, or 1024 */
    uint64_t timer_last_tick;
    uint8_t  int_flags;           /* TimerFlag, PA7Flag */

    /* Base address of this RIOT in the 6502 address space. */
    uint16_t base_addr;
} riot_chip;

typedef struct dorado_baseboard {
    /* Direct memory: RAM (low) + EPROM (high). I/O regions are
     * trapped via the bus dispatcher. */
    uint8_t mem[BASEBOARD_MEM_SIZE];

    /* The five 6532 chips. */
    riot_chip riot[BASEBOARD_NUM_RIOTS];

    /* CPReg — the back-channel between BaseBoard and Dorado main CPU.
     *
     * Per doradoio.mdefs: CPRegH is one 6532's PA, CPRegL is the same
     * chip's PB. The Dorado side reads a 16-bit value via FF←RWCPReg,
     * combining the two halves.
     *
     * We keep both directions: `to_dorado` is what the BaseBoard last
     * wrote and the Dorado will see; `from_dorado` is what the Dorado
     * last wrote (the BaseBoard reads via the same chip's PA/PB). */
    uint16_t cpreg_to_dorado;
    uint16_t cpreg_from_dorado;
    int      cpreg_dorado_has_data;
    int      cpreg_baseboard_has_data;

    /* External state — what the world is doing to the BaseBoard.
     *  - `boot_pressed`: nonzero when the boot button is held down.
     *  - `lamp_on`: derived; the BaseBoard sets this via MiscByte. */
    int boot_pressed;
    int lamp_on;

    /* Total cycles run. */
    uint64_t cycles;

    /* Last reason for halt / panic, for tests to inspect. */
    int  halted;
    char halt_msg[128];
} dorado_baseboard;

/* Pointer to the active baseboard. fake6502's read6502/write6502
 * callbacks dispatch through this. We can only have one active
 * BaseBoard at a time (the underlying 6502 emulator uses globals). */
extern dorado_baseboard *baseboard_active;

/* Initialize an empty BaseBoard. RAM is zeroed; EPROM regions and
 * I/O regs are unmapped until you call baseboard_load_rom. */
void baseboard_init(dorado_baseboard *bb);

/* Load the BaseBoard ROM from a doradobaserom.mb-format file. The
 * file is loaded as the canonical 64K image — every address with
 * data in the .mb populates the corresponding byte. RAM and I/O
 * regions are skipped. Returns 0 on success. */
int baseboard_load_rom(dorado_baseboard *bb, const char *mb_path);

/* Reset the 6502 — fetches PC from FFFC/FFFD via our memory map. */
void baseboard_reset(dorado_baseboard *bb);

/* Run the BaseBoard for `cycles` 6502 ticks (or fewer if it halts).
 * Returns the number of cycles actually executed. */
uint32_t baseboard_run(dorado_baseboard *bb, uint32_t cycles);

/* Step the BaseBoard exactly one 6502 instruction. Returns ticks. */
uint32_t baseboard_step(dorado_baseboard *bb);

/* CPReg interface (the Dorado main CPU calls these). */
uint16_t baseboard_dorado_read_cpreg(dorado_baseboard *bb);
void     baseboard_dorado_write_cpreg(dorado_baseboard *bb, uint16_t value);

/* Press / release the boot button. `count` = number of presses; the
 * firmware uses press-counting to distinguish 1-/2-/3-/4-push boots
 * (per the booting memo). */
void baseboard_press_boot(dorado_baseboard *bb, int count);

/* Diagnostic: dump current state to a string. */
void baseboard_dump(const dorado_baseboard *bb, char *buf, size_t buflen);

#endif
