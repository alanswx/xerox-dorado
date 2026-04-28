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
 * RIOT register offsets (relative to the chip's base address). Per
 * doradoio.mdefs:
 *   PA       = 0    Port A data
 *   DDR      = 1    DDR for the most-recently-touched port
 *   PB       = 2    Port B data
 *   IntFlags = 5    interrupt-flag register (TimerFlag, PA7Flag)
 *   Timer    = 14   timer value (with prescaler set by which
 *                   write address is used)
 *
 * On real hardware the addressing is more nuanced — some registers
 * appear at multiple offsets to encode prescaler / interrupt-enable
 * state. Our model is loose: we recognize the offsets the BaseBoard
 * firmware actually exercises and stub the rest.
 */
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
    /* Real RIOTs alias each register across the 0x80-byte window with
     * various A2/A4 bits selecting timer-prescaler / interrupt-enable
     * variants. We only handle the canonical offsets. */
    switch (offset) {
    case 0:  return riot_read_pa(r);                    /* PA */
    case 1:  return r->pa_ddr;                          /* DDRA */
    case 2:  return riot_read_pb(r);                    /* PB */
    case 3:  return r->pb_ddr;                          /* DDRB */
    case 4: case 0x0C:                                  /* timer read */
        return r->timer_value;
    case 5: case 0x0D: {                                /* IntFlags */
        uint8_t f = r->int_flags;
        r->int_flags = 0;            /* read clears flags */
        (void)bb;
        return f;
    }
    default:
        /* Many RIOT register addresses overlap PA/PB depending on
         * decode; alias odd offsets to DDR/Port. Return 0 for
         * anything we don't know. */
        return 0;
    }
}

static void riot_register_write(dorado_baseboard *bb, riot_chip *r,
                                uint8_t offset, uint8_t value)
{
    (void)bb;
    switch (offset) {
    case 0:  r->pa_latch = value; break;
    case 1:  r->pa_ddr   = value; break;
    case 2:  r->pb_latch = value; break;
    case 3:  r->pb_ddr   = value; break;
    case 0x14: r->timer_prescaler = 1;     r->timer_value = value; break;
    case 0x15: r->timer_prescaler = 8;     r->timer_value = value; break;
    case 0x16: r->timer_prescaler = 64;    r->timer_value = value; break;
    case 0x17: r->timer_prescaler = 1024;  r->timer_value = value; break;
    default:
        break;   /* ignore writes to undecoded offsets */
    }
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

    /* RAM / unmapped — accept the write into mem[]. */
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

uint32_t baseboard_run(dorado_baseboard *bb, uint32_t cycles)
{
    baseboard_active = bb;
    uint32_t ran = exec6502(cycles);
    bb->cycles += ran;
    return ran;
}

uint32_t baseboard_step(dorado_baseboard *bb)
{
    baseboard_active = bb;
    uint32_t t = step6502();
    bb->cycles += t;
    return t;
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

void baseboard_press_boot(dorado_baseboard *bb, int count)
{
    /* For now, just drop the Boot' bit (active-low) once. A more
     * faithful model would time press/release per the booting memo's
     * 0.25–2.5 second windows. */
    (void)count;
    bb->boot_pressed = 1;
    bb->riot[1].pb_external &= (uint8_t)~0x40;   /* Boot' → 0 */
}

void baseboard_dump(const dorado_baseboard *bb, char *buf, size_t buflen)
{
    snprintf(buf, buflen,
             "PC=%04X A=%02X X=%02X Y=%02X SP=%02X P=%02X "
             "cycles=%llu lamp=%d",
             pc, a, x, y, sp, status,
             (unsigned long long)bb->cycles,
             bb->lamp_on);
}
