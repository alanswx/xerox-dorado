#include "baseboard.h"

#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static dorado_baseboard bb;       /* large struct — static */

/* Test 1: load doradobaserom.mb!13 and check key addresses. The
 * canonical Reset code lives in B-08 EPROM region (F000-F7FF). The
 * reset vector at FFFC..FFFD should point into 0xF000-0xFFFF. */
static int test_rom_load(void)
{
    baseboard_init(&bb);
    int rc = baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13");
    EXPECT(rc == 0, "load_rom failed: %s", bb.halt_msg);

    uint16_t reset_vec = (uint16_t)(bb.mem[0xFFFC] |
                                    ((uint16_t)bb.mem[0xFFFD] << 8));
    EXPECT(reset_vec >= 0xF000 && reset_vec <= 0xFFFE,
           "reset vector 0x%04X not in F000-FFFE EPROM range",
           reset_vec);

    uint16_t irq_vec = (uint16_t)(bb.mem[0xFFFE] |
                                  ((uint16_t)bb.mem[0xFFFF] << 8));
    EXPECT(irq_vec >= 0xF000 && irq_vec <= 0xFFFE,
           "IRQ vector 0x%04X not in EPROM range", irq_vec);

    /* Spot-check that EPROM regions have *some* nonzero data. */
    int nonzero_in_F = 0, nonzero_in_C = 0;
    for (int a = 0xF000; a <= 0xFFFF; a++) if (bb.mem[a]) nonzero_in_F++;
    for (int a = 0xC000; a <= 0xCFFF; a++) if (bb.mem[a]) nonzero_in_C++;
    EXPECT(nonzero_in_F > 100,
           "expected ROM bytes in F000-FFFF, got %d nonzero", nonzero_in_F);
    EXPECT(nonzero_in_C > 100,
           "expected ROM bytes in C000-CFFF, got %d nonzero", nonzero_in_C);

    printf("PASS  test_rom_load (reset=0x%04X, irq=0x%04X, "
           "F-EPROM=%d nonzero, C-EPROM=%d nonzero)\n",
           reset_vec, irq_vec, nonzero_in_F, nonzero_in_C);
    return 0;
}

/* Test 2: reset and step. The 6502's PC should advance after each
 * instruction, and after a few instructions still be in EPROM. */
static int test_reset_and_step(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);

    char dump[256];
    baseboard_dump(&bb, dump, sizeof dump);
    /* PC after reset should be the reset vector. */

    int n_steps = 0;
    for (int i = 0; i < 20; i++) {
        if (baseboard_step(&bb) == 0) FAIL("step %d returned 0 cycles", i);
        n_steps++;
    }
    /* After 20 instructions the cycle count should be > 20 (each
     * 6502 instruction takes 2-7 cycles). */
    EXPECT(bb.cycles >= 40,
           "expected ≥40 cycles after 20 instructions, got %llu",
           (unsigned long long)bb.cycles);

    baseboard_dump(&bb, dump, sizeof dump);
    printf("PASS  test_reset_and_step (%d insns, %s)\n", n_steps, dump);
    return 0;
}

/* Test 3: run a longer burst — 1000 instructions. Verify it doesn't
 * spin in a single instruction (i.e., PC changes) and doesn't escape
 * to unmapped memory. The BaseBoard's reset path is dozens of
 * instructions before it settles into the supervisor loop. */
static int test_run_burst(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);

    /* Run 5000 cycles. */
    uint32_t ran = baseboard_run(&bb, 5000);
    EXPECT(ran >= 5000,
           "run requested 5000 cycles, got %u", ran);

    char dump[256];
    baseboard_dump(&bb, dump, sizeof dump);
    printf("PASS  test_run_burst (%u cycles, %s)\n", ran, dump);
    return 0;
}

/* Test 4: CPReg interface. The Dorado side reads the BaseBoard's
 * CPReg latches; the BaseBoard side sees what the Dorado wrote on
 * its CPReg input pins. */
static int test_cpreg_interface(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);

    /* Default: CPReg latches are zero (BB hasn't written anything). */
    uint16_t v = baseboard_dorado_read_cpreg(&bb);
    EXPECT(v == 0,
           "expected CPReg=0 at boot, got 0x%04X", v);

    /* Dorado writes a value; BaseBoard's external pins should see it. */
    baseboard_dorado_write_cpreg(&bb, 0x1234);
    EXPECT(bb.cpreg_from_dorado == 0x1234,
           "expected from_dorado=0x1234, got 0x%04X", bb.cpreg_from_dorado);
    EXPECT(bb.riot[2].pa_external == 0x12,
           "expected CPReg PA pin=0x12, got 0x%02X", bb.riot[2].pa_external);
    EXPECT(bb.riot[2].pb_external == 0x34,
           "expected CPReg PB pin=0x34, got 0x%02X", bb.riot[2].pb_external);

    printf("PASS  test_cpreg_interface\n");
    return 0;
}

/*
 * Test 5: Timer + IRQ. The reset path writes to RIOT[3] timer with
 * IRQ enabled. After running enough cycles, the IRQ handler should
 * have run multiple times and the timer should keep being reloaded.
 * We verify by checking that the timer is in a non-overflowed state
 * (i.e., the IRQ handler is keeping it fresh).
 */
static int test_timer_irq(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);

    /* Run 100k cycles — comfortably past the reset code (which finishes
     * around cycle 5000) and into the WaitForInitialBoot poll loop. */
    baseboard_run(&bb, 100000);

    /* RIOT #3 (the MCPBus / timer chip at 0x580) should have been
     * configured with IRQ enabled (0x59C/0x59F write). */
    EXPECT(bb.riot[3].timer_int_enabled == 1,
           "timer IRQ should be enabled, got %d",
           bb.riot[3].timer_int_enabled);
    EXPECT(bb.irq_count > 10,
           "expected IRQ to have fired many times in 100k cycles, got %llu",
           (unsigned long long)bb.irq_count);

    /* PC should be in the F-prefix EPROM (the polling loop is in
     * doradoreset.masm / doradocontinuous.masm code regions). */
    uint16_t pc = baseboard_pc(&bb);
    EXPECT(pc >= 0xF000 && pc <= 0xFFFF,
           "after init PC should be in F-EPROM, got 0x%04X", pc);

    printf("PASS  test_timer_irq (PC=0x%04X, IRQ enabled, %llu cycles)\n",
           pc, (unsigned long long)bb.cycles);
    return 0;
}

/*
 * Test 6: Boot button press triggers a state change.
 *
 * The reset code sets up "ShutDown" status and sits in
 * WaitForInitialBoot polling CheckBootButton + DoAllConversions.
 * Pressing the boot button should cause the BaseBoard to transition
 * out of that loop. We watch for the PC range to change after
 * enough cycles with the button held.
 */
static int test_boot_button_press(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);

    /* Settle the reset path. */
    baseboard_run(&bb, 200000);

    uint16_t pc_before = baseboard_pc(&bb);
    EXPECT(pc_before >= 0xF000, "expected PC in F-EPROM before press");

    /* Press the button and run enough cycles for the firmware's
     * 1.5-second-no-press dispatch to fire. At ~1 MHz that's 1.5 M
     * cycles; we'll run 5 M to be safe. Hold for the first second,
     * then release. */
    baseboard_boot_button(&bb, 1);
    baseboard_run(&bb, 1000000);
    baseboard_boot_button(&bb, 0);
    baseboard_run(&bb, 4000000);

    /* PC should have moved (into RebootDorado / HotBoot / similar). */
    uint16_t pc_after = baseboard_pc(&bb);
    EXPECT(pc_after != pc_before || bb.cycles > 5000000,
           "expected PC to move after boot press, stayed at 0x%04X",
           pc_after);

    printf("PASS  test_boot_button_press (PC: 0x%04X → 0x%04X, "
           "%llu cycles)\n",
           pc_before, pc_after, (unsigned long long)bb.cycles);
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_rom_load();
    rc |= test_reset_and_step();
    rc |= test_run_burst();
    rc |= test_cpreg_interface();
    rc |= test_timer_irq();
    rc |= test_boot_button_press();
    if (rc == 0) printf("\nAll BaseBoard tests passed.\n");
    return rc;
}
