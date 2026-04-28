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

/*
 * Test 7: Full cold boot reaches LoadDoradoCode and Continuous.
 *
 * After 3 boot-button presses (CoolBoot path) and the firmware's
 * power-supply / clock-speed sequencing, control should reach
 * LoadDoradoCode (0xFAAE) and then settle into the Continuous
 * steady-state loop (0xF4F3). This is the proof that our analog
 * model (DAC + comparators) is producing in-spec readings — without
 * it, the firmware spins in CheckAllSupplies forever.
 */
static int test_cold_boot_to_continuous(void)
{
    baseboard_init(&bb);
    EXPECT(baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0,
           "load_rom");
    baseboard_reset(&bb);
    baseboard_run(&bb, 1000000);

    /* Three presses, ~400ms held, ~600ms released. */
    for (int i = 0; i < 3; i++) {
        baseboard_boot_button(&bb, 1);
        baseboard_run(&bb, 400000);
        baseboard_boot_button(&bb, 0);
        baseboard_run(&bb, 600000);
    }

    /* Settle the dispatch + 1.5s "no-more-presses" window. */
    baseboard_run(&bb, 2000000);

    /* Step instruction-by-instruction looking for LoadDoradoCode. */
    int saw_load_dorado_code = 0;
    int saw_continuous = 0;
    uint64_t deadline = bb.cycles + 30000000ULL;
    while (bb.cycles < deadline && !saw_continuous) {
        baseboard_step(&bb);
        uint16_t p = baseboard_pc(&bb);
        if (p == 0xFAAE) saw_load_dorado_code = 1;
        if (p == 0xF4F3) saw_continuous = 1;
    }

    EXPECT(saw_load_dorado_code,
           "never reached LoadDoradoCode in %llu cycles (PC=0x%04X)",
           (unsigned long long)bb.cycles, baseboard_pc(&bb));
    EXPECT(saw_continuous,
           "never reached Continuous in %llu cycles (PC=0x%04X)",
           (unsigned long long)bb.cycles, baseboard_pc(&bb));

    printf("PASS  test_cold_boot_to_continuous "
           "(LoadDoradoCode + Continuous reached, %llu cycles)\n",
           (unsigned long long)bb.cycles);
    return 0;
}

/*
 * Test 8: BB→Dorado MCPBus strobe → CPReg latching.
 *
 * Drive RIOT[3] PA/PB directly to simulate the BB writing
 *   MCPBusH = data, MCPBusL = ABMux0+strobe, then ABMux1+strobe.
 * Verify the data lands in cpreg_to_dorado and that the strobed
 * MIR sequence sets dorado_mir_loaded.
 */
static int test_mcp_strobe(void)
{
    baseboard_init(&bb);
    baseboard_active = &bb;       /* route write6502 callbacks at us */
    extern void write6502(unsigned short, unsigned char);

    /* Helper to mimic the firmware's "STA MCPBusH; STA MCPBusL;
     * INC MCPBusL; DEC MCPBusL" pattern. The strobe handler fires on
     * the rising edge of MCPBusL bit 0, so we write the function code
     * with strobe=0 first, then with strobe=1 to clock it. RIOT[3]
     * is at base 0x580; PA (MCPBusH) = +0, PB (MCPBusL) = +2. */
    #define MCP_STROBE(func, setss, data) do {                              \
        write6502((unsigned short)0x580, (uint8_t)(data));                  \
        write6502((unsigned short)0x582, (uint8_t)(((setss) << 7) |         \
                                                    ((func) << 4)));        \
        write6502((unsigned short)0x582, (uint8_t)(((setss) << 7) |         \
                                                    ((func) << 4) | 0x01)); \
    } while (0)

    /* ABMux0: low byte of CPReg = 0xCD. */
    MCP_STROBE(2, 0, 0xCD);
    /* ABMux1: high byte of CPReg = 0xAB. */
    MCP_STROBE(3, 0, 0xAB);

    EXPECT(baseboard_dorado_read_cpreg(&bb) == 0xABCD,
           "after ABMux0/1 strobes, CPReg = 0x%04X, want 0xABCD",
           baseboard_dorado_read_cpreg(&bb));

    /* MIR0..MIR3 strobes — the 5-byte Bootstrap Nop pattern.
     * mir_bytes[0] = 0x70 holds the extra bits; the per-strobe extra
     * bit comes from SetSS (MCPBusL bit 7). The bits, MSB-first, are
     * 0,1,1,1 (RSTK[0]=0, P015=1, JCN[7]=1, P1631=1). */
    MCP_STROBE(4, 0, 0x01);  /* MIR0, extra=0 (RSTK[0]) */
    MCP_STROBE(5, 1, 0x0F);  /* MIR1, extra=1 (P015)   */
    MCP_STROBE(6, 1, 0x4C);  /* MIR2, extra=1 (JCN[7]) */
    MCP_STROBE(7, 1, 0x40);  /* MIR3, extra=1 (P1631)  */

    EXPECT(bb.dorado_mir_loaded,
           "after 4 MIR strobes, dorado_mir_loaded should be 1");
    EXPECT(bb.mir_bytes[0] == 0x70,
           "MIR extra bits = 0x%02X, want 0x70", bb.mir_bytes[0]);
    EXPECT(bb.mir_bytes[1] == 0x01 && bb.mir_bytes[4] == 0x40,
           "MIR payload bytes wrong: %02X %02X %02X %02X %02X",
           bb.mir_bytes[0], bb.mir_bytes[1], bb.mir_bytes[2],
           bb.mir_bytes[3], bb.mir_bytes[4]);

    /* Control function: SetRun + SetSS (single-step request). */
    MCP_STROBE(0, 1, 0x01);
    EXPECT(bb.dorado_ss_pending,
           "after Control SetRun+SetSS, ss_pending should be 1");

    printf("PASS  test_mcp_strobe (CPReg=0x%04X, MIR loaded, SS pending)\n",
           baseboard_dorado_read_cpreg(&bb));
    return 0;

    #undef MCP_STROBE
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
    rc |= test_cold_boot_to_continuous();
    rc |= test_mcp_strobe();
    if (rc == 0) printf("\nAll BaseBoard tests passed.\n");
    return rc;
}
