/*
 * Dorado BaseBoard — 6502 + 5 × 6532 RIOT chips + EPROM.
 *
 * The 6502 core is C-Chads/MyLittle6502 (Mike Chambers fake6502 with
 * BCD/ADC/SBC/BIT/IRQ-mask fixes), CC0 / public domain. We include
 * its single-file implementation directly so the read6502/write6502
 * callbacks land in this translation unit.
 *
 * See include/baseboard.h for the memory map.
 */

#include "baseboard.h"
#include "mb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dorado_baseboard *baseboard_active = NULL;

/* fake6502 declares globals at file scope — that's why we can only
 * have one active BaseBoard. Provide the read/write callbacks it
 * expects before #include'ing the implementation. */
typedef unsigned short ushort;
typedef unsigned char  uint8;
typedef unsigned int   uint32;

extern uint8 read6502(ushort address);
extern void  write6502(ushort address, uint8 value);

/* Forward-declare ourselves so the 6502 core can see the symbols. */
static uint8 bb_read(ushort address);
static void  bb_write(ushort address, uint8 value);

uint8 read6502(ushort address)            { return bb_read(address); }
void  write6502(ushort address, uint8 v)  { bb_write(address, v);    }

/* Pull in fake6502's full implementation here. */
#include "fake6502.h"

/* ─── 6532 RIOT helpers ───────────────────────────────────────── */

/*
 * 6532 (RIOT) register addressing within each chip's 0x80-byte window:
 *
 *   offset    write target              read target
 *   0x00      PA latch                  PA pins (latch & DDR | ext & ~DDR)
 *   0x01      PA DDR                    PA DDR
 *   0x02      PB latch                  PB pins
 *   0x03      PB DDR                    PB DDR
 *   0x04      —                         IntFlags (clear on read)
 *   0x05      —                         IntFlags (clear on read)
 *   0x14-17   timer load, IRQ disabled  timer value
 *   0x1C-1F   timer load, IRQ enabled   timer value
 *
 * Prescaler is selected by the low 2 bits of the write offset:
 *   .14/.1C → ÷1   .15/.1D → ÷8   .16/.1E → ÷64   .17/.1F → ÷1024
 *
 * Real RIOTs alias many addresses depending on A2/A3/A4 decoding; we
 * cover the canonical set the BaseBoard firmware uses.
 */

/* Compute the timer's apparent value at the current cycle count. */
static uint8_t riot_timer_current(const riot_chip *r, uint64_t now)
{
    uint64_t elapsed = now - r->timer_load_cycle;
    if (!r->timer_underflowed) {
        uint64_t ticks = elapsed / r->timer_prescaler;
        if (ticks <= r->timer_load_value) {
            return (uint8_t)(r->timer_load_value - ticks);
        }
        /* Past underflow point — value continues at no-prescaler rate. */
        uint64_t under = ticks - r->timer_load_value;
        /* Subtracting from 0xFF then wrapping. */
        return (uint8_t)(0xFFu - (under & 0xFFu));
    }
    /* Already in underflow mode (no prescaler). */
    return (uint8_t)(r->timer_value - (uint8_t)(elapsed & 0xFF));
}

/* Advance internal timer state to `now` and update underflow / flags. */
static void riot_timer_tick(riot_chip *r, uint64_t now)
{
    if (r->timer_underflowed) return;
    uint64_t elapsed = now - r->timer_load_cycle;
    uint64_t ticks   = elapsed / r->timer_prescaler;
    if (ticks > r->timer_load_value) {
        r->timer_underflowed = 1;
        r->int_flags |= 0x80;     /* TimerFlag */
        /* Re-anchor for underflow-mode counting (1 cycle / decrement). */
        r->timer_value      = (uint8_t)(0xFFu);
        r->timer_load_cycle = now;
    }
}

/* Returns 1 if any RIOT is currently asserting IRQ. */
static int baseboard_any_irq(dorado_baseboard *bb)
{
    for (int i = 0; i < BASEBOARD_NUM_RIOTS; i++) {
        const riot_chip *r = &bb->riot[i];
        if (r->timer_int_enabled && (r->int_flags & 0x80)) return 1;
    }
    return 0;
}
static uint8_t riot_read_pa(const riot_chip *r)
{
    return (uint8_t)((r->pa_latch & r->pa_ddr) |
                     (r->pa_external & (uint8_t)~r->pa_ddr));
}
static uint8_t riot_read_pb(const riot_chip *r)
{
    return (uint8_t)((r->pb_latch & r->pb_ddr) |
                     (r->pb_external & (uint8_t)~r->pb_ddr));
}

/* Which RIOT chip serves a given address? Returns NULL if none. */
static riot_chip *riot_for(dorado_baseboard *bb, uint16_t addr)
{
    for (int i = 0; i < BASEBOARD_NUM_RIOTS; i++) {
        riot_chip *r = &bb->riot[i];
        if (addr >= r->base_addr && addr < (uint16_t)(r->base_addr + 0x80))
            return r;
    }
    return NULL;
}

static uint8_t riot_register_read(dorado_baseboard *bb, riot_chip *r,
                                  uint8_t offset)
{
    /* The 6532's read decoding folds the 0x80 window down to PA, DDRA,
     * PB, DDRB, IntFlags, and the timer. */
    switch (offset & 0x07) {
    case 0:  return riot_read_pa(r);                    /* PA pins */
    case 1:  return r->pa_ddr;                          /* DDRA */
    case 2:  return riot_read_pb(r);                    /* PB pins */
    case 3:  return r->pb_ddr;                          /* DDRB */
    case 4: case 6:                                     /* timer read */
        return riot_timer_current(r, bb->cycles);
    case 5: case 7: {                                   /* IntFlags */
        uint8_t f = r->int_flags;
        r->int_flags = 0;
        return f;
    }
    }
    return 0;
}

static void riot_register_write(dorado_baseboard *bb, riot_chip *r,
                                uint8_t offset, uint8_t value)
{
    /* Standard 6532 write decoding: low 4 bits select function. */
    if (offset < 0x04) {
        switch (offset) {
        case 0: r->pa_latch = value; break;
        case 1: r->pa_ddr   = value; break;
        case 2: r->pb_latch = value; break;
        case 3: r->pb_ddr   = value; break;
        }
        return;
    }
    /* 0x14-0x17 = timer load (IRQ disabled), 0x1C-0x1F = timer load
     * (IRQ enabled). Low 2 bits select prescaler. */
    if ((offset & 0x14) == 0x14) {
        static const uint16_t prescaler_table[4] = { 1, 8, 64, 1024 };
        r->timer_prescaler   = prescaler_table[offset & 0x03];
        r->timer_load_value  = value;
        r->timer_value       = value;
        r->timer_load_cycle  = bb->cycles;
        r->timer_underflowed = 0;
        r->int_flags        &= (uint8_t)~0x80;   /* clear TimerFlag */
        r->timer_int_enabled = (offset & 0x08) ? 1 : 0;
    }
    /* Other offsets: silently ignore. */
}

/* ─── Bus dispatcher ──────────────────────────────────────────── */

static uint8 bb_read(ushort addr)
{
    dorado_baseboard *bb = baseboard_active;
    if (!bb) return 0xFF;

    /* I/O region: 0x0400..0x067F (5 chips × 0x80 bytes each). */
    if (addr >= 0x0400 && addr <= 0x067F) {
        riot_chip *r = riot_for(bb, addr);
        if (r) return riot_register_read(bb, r, (uint8_t)(addr - r->base_addr));
        return 0xFF;
    }

    /* RAM / EPROM / unmapped — backed by mem[]. */
    return bb->mem[addr];
}

/*
 * Write watchpoints. When non-NULL, calls into the per-address hook
 * before storing — useful for debugging boot-counter transitions.
 */
typedef void (*bb_write_hook_fn)(uint16_t addr, uint8_t old, uint8_t new_, uint16_t pc);
bb_write_hook_fn baseboard_write_hook = NULL;

static void bb_write(ushort addr, uint8 value)
{
    dorado_baseboard *bb = baseboard_active;
    if (!bb) return;

    if (addr >= 0x0400 && addr <= 0x067F) {
        riot_chip *r = riot_for(bb, addr);
        if (r) riot_register_write(bb, r, (uint8_t)(addr - r->base_addr), value);
        return;
    }

    /* EPROM regions are read-only — silently drop writes. */
    if (addr >= 0xC000 && addr <= 0xFFFF) return;
    if (addr >= 0xD000 && addr <= 0xD7FF) return;

    if (baseboard_write_hook) {
        uint8_t old = bb->mem[addr];
        if (old != value) baseboard_write_hook(addr, old, value, pc);
    }
    bb->mem[addr] = value;
}

/* ─── Lifecycle ──────────────────────────────────────────────── */

void baseboard_init(dorado_baseboard *bb)
{
    memset(bb, 0, sizeof *bb);

    /* RIOT chip layout per doradoio.mdefs. Chip names just for
     * debugging — they don't affect behavior. */
    static const struct { uint16_t base; const char *name; } riot_layout[] = {
        { 0x0400, "DAC/CPIBus" },
        { 0x0480, "Comparators/MiscByte" },
        { 0x0500, "CPReg" },              /* the Dorado interface */
        { 0x0580, "MCPBus" },
        { 0x0600, "Watchdog" },
    };
    for (int i = 0; i < BASEBOARD_NUM_RIOTS; i++) {
        bb->riot[i].base_addr = riot_layout[i].base;
    }

    /* Pre-set the boot-button line. Boot' is active-low (per the
     * MiscByte 0x40 bit named "Boot'"); start with button NOT
     * pressed → bit set. The MiscByte chip is RIOT #2 (0x480). */
    bb->riot[1].pb_external = 0x40;       /* Boot' = 1 (not pressed) */

    /* MCPBusL has SkipDiskWait' (bit 0x02) — when LOW (jumper installed),
     * the reset code SKIPS WaitForInitialBoot and goes straight to
     * RebootDorado. We want the normal boot path: jumper NOT installed,
     * so set this bit HIGH. RIOT #4 (MCPBus) is at base 0x580. */
    bb->riot[3].pb_external = 0x02;       /* SkipDiskWait' = 1 (no jumper) */
}

int baseboard_load_rom(dorado_baseboard *bb, const char *mb_path)
{
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, mb_path) != MB_OK) {
        snprintf(bb->halt_msg, sizeof bb->halt_msg,
                 "could not load BaseBoard ROM: %s", mb_path);
        return 1;
    }

    /* The .mb file declares one memory called "$ABSOLUTE" with width
     * 8 bits. Each entry is one 16-bit storage word; the byte value
     * lives in the high byte (left-justified per MicroD's writer). */
    int memid = -1;
    for (int i = 0; i < MB_MAX_MEMS; i++) {
        if (mb.mems[i].defined &&
            (strcmp(mb.mems[i].name, "$ABSOLUTE") == 0 ||
             strcmp(mb.mems[i].name, "ABSOLUTE")  == 0)) {
            memid = i;
            break;
        }
    }
    if (memid < 0) {
        snprintf(bb->halt_msg, sizeof bb->halt_msg,
                 "BaseBoard ROM has no $ABSOLUTE memory");
        mb_free(&mb);
        return 1;
    }

    const mb_memory *m = &mb.mems[memid];
    int n_loaded = 0;
    for (int a = 0; a < m->max_addr && a < BASEBOARD_MEM_SIZE; a++) {
        if (!m->present[a]) continue;
        bb->mem[a] = (uint8_t)((m->data[a] >> 8) & 0xFF);
        n_loaded++;
    }

    mb_free(&mb);
    snprintf(bb->halt_msg, sizeof bb->halt_msg,
             "loaded %d bytes from %s", n_loaded, mb_path);
    return 0;
}

void baseboard_reset(dorado_baseboard *bb)
{
    baseboard_active = bb;
    bb->cycles = 0;
    bb->halted = 0;
    reset6502();
}

/*
 * Step one 6502 instruction, then advance per-RIOT timers and assert
 * IRQ if any chip is currently flagging a timer underflow with
 * interrupt-enable set. Returns ticks consumed.
 *
 * We step instruction-by-instruction (not in big exec6502 batches) so
 * that timer underflows are detected within a few microseconds rather
 * than being missed when an interrupt-enabled timer fires inside a
 * 1000-cycle batch.
 */
uint32_t baseboard_step(dorado_baseboard *bb)
{
    baseboard_active = bb;
    uint32_t t = step6502();
    bb->cycles += t;

    /* Tick timers — sets TimerFlag if an underflow happened during
     * this instruction. */
    for (int i = 0; i < BASEBOARD_NUM_RIOTS; i++) {
        riot_timer_tick(&bb->riot[i], bb->cycles);
    }

    /* If any timer is asserting IRQ, fire it. fake6502 internally
     * checks the I flag and ignores the call when interrupts are
     * masked, so it's safe to call every instruction.
     *
     * We also detect the moment fake6502 actually accepts the
     * interrupt (i.e. PC jumps to the IRQ vector) by checking that
     * the I flag was clear before. */
    if (baseboard_any_irq(bb) && (status & 0x04) == 0) {
        irq6502();
        bb->irq_count++;
    }
    return t;
}

uint32_t baseboard_run(dorado_baseboard *bb, uint32_t cycles)
{
    uint32_t total = 0;
    while (total < cycles) {
        uint32_t t = baseboard_step(bb);
        if (t == 0) break;
        total += t;
    }
    return total;
}

/* ─── Dorado-side CPReg interface ─────────────────────────────── */

uint16_t baseboard_dorado_read_cpreg(dorado_baseboard *bb)
{
    /* Combine the CPReg chip's PA (high byte) and PB (low byte)
     * latches to form the 16-bit value the Dorado sees. */
    riot_chip *cp = &bb->riot[2];        /* CPReg chip is RIOT #2 (index 2) */
    uint16_t hi = riot_read_pa(cp);
    uint16_t lo = riot_read_pb(cp);
    return (uint16_t)((hi << 8) | lo);
}

void baseboard_dorado_write_cpreg(dorado_baseboard *bb, uint16_t value)
{
    /* Dorado writes appear on the BaseBoard's external pins. */
    riot_chip *cp = &bb->riot[2];
    cp->pa_external = (uint8_t)(value >> 8);
    cp->pb_external = (uint8_t)(value & 0xFF);
    bb->cpreg_from_dorado = value;
    bb->cpreg_dorado_has_data = 1;
}

void baseboard_boot_button(dorado_baseboard *bb, int pressed)
{
    bb->boot_pressed = pressed ? 1 : 0;
    if (pressed) {
        /* Boot' is active-low: clear bit 0x40 of RIOT #2 PB. */
        bb->riot[1].pb_external &= (uint8_t)~0x40;
    } else {
        bb->riot[1].pb_external |= 0x40;
    }
}

uint16_t baseboard_pc(const dorado_baseboard *bb)     { (void)bb; return pc; }
uint8_t  baseboard_a(const dorado_baseboard *bb)      { (void)bb; return a; }
uint8_t  baseboard_x(const dorado_baseboard *bb)      { (void)bb; return x; }
uint8_t  baseboard_y(const dorado_baseboard *bb)      { (void)bb; return y; }
uint8_t  baseboard_sp(const dorado_baseboard *bb)     { (void)bb; return sp; }
uint8_t  baseboard_status(const dorado_baseboard *bb) { (void)bb; return status; }

void baseboard_dump(const dorado_baseboard *bb, char *buf, size_t buflen)
{
    snprintf(buf, buflen,
             "PC=%04X A=%02X X=%02X Y=%02X SP=%02X P=%02X "
             "cycles=%llu lamp=%d",
             pc, a, x, y, sp, status,
             (unsigned long long)bb->cycles,
             bb->lamp_on);
}
