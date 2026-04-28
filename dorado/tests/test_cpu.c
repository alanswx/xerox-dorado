#include "baseboard.h"
#include "cpu.h"
#include "disasm.h"
#include "mb.h"
#include "microcode.h"

#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

/*
 * The CPU is single-task, no-IFU, no-memory. Tests construct
 * dorado_microcode structs directly with hand-built dorado_uinstr
 * entries — no need to go through MB encoding/decoding.
 *
 * Field layouts (HM Tables 5–13):
 *   RSTK  4 bits   ALUF  4 bits  BSEL  3 bits  LC  3 bits
 *   ASEL  3 bits   BLOCK 1 bit   FF    8 bits  JCN 8 bits
 *
 * BSEL codes:  0=Md  1=RM/STK  2=T  3=Q  4=0,,FF  5=0xFF,,FF  6=FF,,0  7=FF,,0xFF
 * ASEL codes:  4=A←RM/STK  6=A←T  (others stub for memory/shifter)
 * LC codes:    0=NoLoad  1=T←Pd  6=RM/STK←Pd  7=T←Pd, RM/STK←Pd
 * ALUFM op:    bit 5 = carry-in, bits 4..0 = operation (Table 9):
 *                0o25 = B (logical "B")
 *                0o14 = A+B (no carry)
 *                0o00 = A (with bit5=1 → A+1)
 *                0o31 = all-zeros
 * JCN: bits 7..6 = 10 → local (target = (PC & ~63) | (JCN & 63))
 */

/* Build a dorado_uinstr from explicit field values. */
static dorado_uinstr make_uinstr(int rstk, int aluf, int bsel, int lc,
                                 int asel, int block, int ff, int jcn)
{
    dorado_uinstr u = {0};
    u.rstk  = (uint8_t)rstk;
    u.aluf  = (uint8_t)aluf;
    u.bsel  = (uint8_t)bsel;
    u.lc    = (uint8_t)lc;
    u.asel  = (uint8_t)asel;
    u.block = (uint8_t)block;
    u.ff    = (uint8_t)ff;
    u.jcn   = (uint8_t)jcn;
    return u;
}

/* JCN byte that encodes a local branch within the current page. */
static uint8_t jcn_local(int target_in_page)
{
    /* High 2 bits = 10 (local tag), low 6 = address within page. */
    return (uint8_t)(0x80 | (target_in_page & 0x3F));
}

/* Test 1: T ← 0o123 via 0,,FF constant, then loop.
 * One cycle:
 *   ASEL=6 (A←T)         A bus = T (don't care, ALU op is "B")
 *   BSEL=4 (0,,FF)       B bus = 0o123
 *   ALUF=N → ALUFM[N] = 0o25 (B)
 *   LC=1 (T←Pd)
 *   JCN=local(0) → jump back to PC=0 forever
 */
static int test_load_constant(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[0] = 0o25 (logical "B"). */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    /* IM[0]: T ← 0o123 via 0,,FF; then jump back to 0. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0123,
                           /*jcn=*/jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(cpu.T == 0, "T should start zero");
    EXPECT(cpu.real_PC == 0, "PC should start 0");

    int rc = dorado_cpu_step(&cpu);
    EXPECT(rc == 0, "step failed: %s", cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0123, "expected T=0o123, got 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 0, "expected PC=0 (self-loop), got 0o%o", cpu.real_PC);
    EXPECT(cpu.cycles == 1, "cycles=%d", cpu.cycles);

    /* Second iteration just rewrites T with the same value. */
    rc = dorado_cpu_step(&cpu);
    EXPECT(rc == 0, "second step failed");
    EXPECT(cpu.T == 0123, "T still 0o123");
    EXPECT(cpu.cycles == 2, "cycles=%d", cpu.cycles);

    printf("PASS  test_load_constant\n");
    return 0;
}

/* Test 2: increment a register, branch back to .
 *   IM[0]: T ← T + 1, jump to PC=1
 *   IM[1]: jump back to PC=0 (so we go 0 → 1 → 0 → 1 → ...)
 *
 *   IM[0]: ASEL=6 (A←T), BSEL=4 (0,,FF) FF=0,
 *          ALUF=1 → ALUFM[1] = 0o40 (carry-in=1, op=0 → A+1),
 *          LC=1 (T←Pd), JCN=local(1)
 *   IM[1]: NoOp, JCN=local(0)
 */
static int test_increment_loop(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[1] = 0o40 (bit 5 = 1 = carry-in, op = 0 = A) → A+1. */
    mc.alufm[1] = 040;
    mc.alufm_present[1] = 1;
    /* ALUFM[0] = 0o25 (B) — used by IM[1]'s NoOp pass-through. */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    mc.im[0] = make_uinstr(0, /*aluf=*/1, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: NoOp. ASEL=6 (A←T), BSEL=2 (T), ALUF=0 (B → loads B which is T),
     * LC=0 (NoLoad), JCN=local(0). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/6, 0, 0, jcn_local(0));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0;
    mc.image_to_real[1] = 1;
    mc.image_present[0] = mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Cycle 1: T ← 0+1 = 1, PC: 0 → 1 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 1, "after step 1, T = 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 1, "after step 1, PC = 0o%o", cpu.real_PC);

    /* Cycle 2: NoOp, PC: 1 → 0 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 1, "T unchanged through NoOp: 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 0, "after step 2, PC = 0o%o", cpu.real_PC);

    /* Run until total cycle count reaches 20. We've already done 2 manual
     * steps, so run does 18 more = 9 (increment, noop) pairs. T grows
     * from 1 to 1+9 = 0o12. */
    cpu_halt_reason r = dorado_cpu_run(&cpu, 20);
    EXPECT(r == CPU_HALT_USER, "expected to time out at 20 cycles, got %s",
           cpu_halt_reason_str(r));
    EXPECT(cpu.cycles == 20, "expected 20 cycles total, got %d", cpu.cycles);
    EXPECT(cpu.T == 012,
           "after 9 increments + initial 1, T = 0o%o (expected 0o12)", cpu.T);

    printf("PASS  test_increment_loop (T reached 0o%o after %d cycles)\n",
           cpu.T, cpu.cycles);
    return 0;
}

/* Test 3: write into RM, read it back.
 *   IM[0]: RM[5] ← 0o4321  (BSEL=4 to drive constant on B; LC=6 RM/STK←Pd)
 *          RBase=0, RSTK=5 → RM address = 5
 *          jump to IM[1].
 *   IM[1]: T ← RM[5]  (ASEL=4 A←RM/STK; ALUF=1 A+1? no, just B-pass via ALU.
 *                      Use BSEL=1 RM/STK; ALUF=0 → 0o25 = B; LC=1)
 *          jump back to IM[0].
 */
static int test_rm_roundtrip(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[0] = 0o25 (B) for both writes. */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    /* IM[0]: RM[5] ← 0o4321.  ASEL=4 (A←RM/STK reads RM[5] but we
     * don't care about A); BSEL=4, FF=0o21; LC=6.
     * Actually 0,,FF only puts 8 bits — we want 0o4321 (16 bits).
     * Use BSEL=6 (FF,,0) with FF=0o43 to get 0o43*256 = 0o42600 then
     * we'd need LC=6 + a different mux. Simplest: write low byte only,
     * use 0o21 instead. */
    mc.im[0] = make_uinstr(/*rstk=*/5, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0321, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: T ← RM[5].  ASEL=6 (A←T); BSEL=1 (RM/STK reads RM[5]);
     * ALUF=0 (B); LC=1 (T←Pd). */
    mc.im[1] = make_uinstr(/*rstk=*/5, 0, /*bsel=*/1, /*lc=*/1,
                           /*asel=*/6, 0, 0, jcn_local(0));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0;
    mc.image_to_real[1] = 1;
    mc.image_present[0] = mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.RM[5] == 0321, "RM[5] = 0o%o (expected 0321)", cpu.RM[5]);
    EXPECT(cpu.real_PC == 1, "PC after step 1 = 0o%o", cpu.real_PC);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0321, "T = 0o%o (expected 0321)", cpu.T);
    EXPECT(cpu.real_PC == 0, "PC after step 2 = 0o%o", cpu.real_PC);

    printf("PASS  test_rm_roundtrip (RM[5]=0o%o, T=0o%o)\n", cpu.RM[5], cpu.T);
    return 0;
}

/*
 * Test 4: Conditional branch on ALU=0.
 *
 * Build:
 *   IM[0]: T ← FF=value via BSEL=4. ALUF=0 → ALUFM[0]=0o25 (B-pass).
 *          The value 5 makes alu_zero=0; the value 0 makes alu_zero=1.
 *          Jump to IM[2].
 *   IM[2]: Conditional branch, condition 0 (ALU = 0).
 *          JCN encoded so even target = page+4, odd = page+5.
 *   IM[4]: false-target marker — set RM[10] = 0o4444. Self-loop.
 *   IM[5]: true-target marker  — set RM[10] = 0o5555. Self-loop.
 *
 * For the conditional-branch JCN: with our 8-bit byte (bit 7 = MSB):
 *   bit 7 = 0          (not local/global)
 *   bits 6..5 = 00     (JCN[1:2] = 0)
 *   bits 4..3 = 10     (JCN[3:4] = 2 → page_offset bit2 = 1)
 *   bits 2..0 = 000    (condition 0 = ALU = 0)
 *  → jcn = 0b00010000 = 0o20.
 *  Address calc: offset = (0<<4) | (2<<1) | R = 4 + R.
 */
static int test_conditional_branch_alu_zero(void)
{
    /* Build the microcode common to both runs. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;        /* B-pass */

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[2] = 2; mc.image_present[2] = 1;
    mc.image_to_real[4] = 4; mc.image_present[4] = 1;
    mc.image_to_real[5] = 5; mc.image_present[5] = 1;
    mc.n_instructions = 4;

    /* IM[2]: conditional branch on ALU=0 (cond 0). */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6,
                           0, 0, /*jcn=*/0020);
    mc.im_present[2] = 1;

    /* IM[4]: RM[10] ← 0o4444, self-loop. */
    mc.im[4] = make_uinstr(/*rstk=*/012, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0344, jcn_local(4));
    mc.im_present[4] = 1;

    /* IM[5]: RM[10] ← 0o5555, self-loop. */
    mc.im[5] = make_uinstr(/*rstk=*/012, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0355, jcn_local(5));
    mc.im_present[5] = 1;

    /* --- Run A: T ← 5, expect false branch (ALU != 0 → goto IM[4]). --- */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6,
                           0, /*ff=*/05, jcn_local(2));
    mc.im_present[0] = 1;
    {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, 0);
        dorado_cpu_run(&cpu, 5);
        EXPECT(cpu.real_PC == 4,
               "Run A: expected to reach IM[4] (false branch), got 0o%o",
               cpu.real_PC);
        EXPECT(cpu.RM[012] == 0344,
               "Run A: RM[10] = 0o%o (expected 0o4444)", cpu.RM[012]);
    }

    /* --- Run B: T ← 0, expect true branch (ALU == 0 → goto IM[5]). --- */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6,
                           0, /*ff=*/0, jcn_local(2));
    {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, 0);
        dorado_cpu_run(&cpu, 5);
        EXPECT(cpu.real_PC == 5,
               "Run B: expected to reach IM[5] (true branch), got 0o%o",
               cpu.real_PC);
        EXPECT(cpu.RM[012] == 0355,
               "Run B: RM[10] = 0o%o (expected 0o5555)", cpu.RM[012]);
    }

    printf("PASS  test_conditional_branch_alu_zero (both branches taken)\n");
    return 0;
}

/*
 * Test 5: Shifter byte-cycle (HM §3.11).
 *
 * Set T ← 0o123 (= 0x53), then run a shift with:
 *   ASEL=7 (shift), BSEL=7 (FF-controlled, SHA=T, SHB=T → input = T..T)
 *   FF=0o010 → count = FF[4:7] = 8, LMask = FF[0:3] = 0, RMask = FF[4:7] = 8
 *   ALUF[0:2] = 0  → ShiftNoMask
 *   ALUF[3]   = 0  → use ALUFM[14] (= NOT A by convention)
 *
 * Expected:
 *   input32         = T..T          = 0x00530053
 *   cycled_left_8   = 0x53005300
 *   low 16          = 0x5300
 *   A bus           = ~0x5300       = 0xACFF (low-true)
 *   ALU = NOT A     = 0x5300
 *   Pd → RM[5]      = 0x5300        = 0o51400
 */
static int test_shifter_byte_cycle(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    mc.alufm[0]  = 025;             mc.alufm_present[0]  = 1;  /* B-pass */
    mc.alufm[14] = 001;             mc.alufm_present[14] = 1;  /* NOT A */

    /* IM[0]: T ← 0o123 via 0,,FF. */
    mc.im[0] = make_uinstr(0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0123, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: RM[5] ← shifter(T..T cycled 8). */
    mc.im[1] = make_uinstr(/*rstk=*/5, /*aluf=*/0, /*bsel=*/7,
                           /*lc=*/6, /*asel=*/7, 0,
                           /*ff=*/0010, jcn_local(1));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[1] = 1; mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step 1: T ← 0o123 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0123, "T = 0o%o (expected 0o123)", cpu.T);

    /* Step 2: shift */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.RM[5] == 051400,
           "RM[5] = 0o%o (expected 0o51400 = byte-swap of 0o123)", cpu.RM[5]);

    printf("PASS  test_shifter_byte_cycle (RM[5] = 0o%o)\n", cpu.RM[5]);
    return 0;
}

/*
 * Test 6: Shifter with ShiftRMask.
 *
 * Same setup as test 5, but ALUF=4:
 *   ALUF[0:2] = 010 = 2  → ShiftRMask (mask right RMask bits with 0)
 *   ALUF[3]   = 0        → ALUFM[14]
 * Expected:
 *   shifted = 0x5300; rmask of 8 bits → keep top byte, zero bottom byte
 *   result = 0x5300 (already had 0 in low byte) → 0x5300
 *
 * Use FF=0o210 (count=8, LMask=2, RMask=8) and T=0xFFFF to get a more
 * distinctive output. Then T..T = 0xFFFFFFFF, cycle 8 = 0xFFFFFFFF,
 * low 16 = 0xFFFF, ShiftRMask(0xFFFF) with rmask=8 = 0xFF00.
 *
 * But T=0xFFFF requires a 16-bit constant. Use FF=0o377, BSEL=5 (= 0xFFFF)
 * via 0o377,,FF route.
 */
static int test_shifter_rmask(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025;             mc.alufm_present[0]  = 1;
    mc.alufm[14] = 001;             mc.alufm_present[14] = 1;

    /* IM[0]: T ← 0o377,,FF with FF=0o377 → 0xFFFF. */
    mc.im[0] = make_uinstr(0, /*aluf=*/0, /*bsel=*/5, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0377, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: ShiftRMask. ALUF=4 → ALUF[0:2]=010=2 (ShiftRMask), ALUF[3]=0. */
    mc.im[1] = make_uinstr(/*rstk=*/5, /*aluf=*/4, /*bsel=*/7,
                           /*lc=*/6, /*asel=*/7, 0,
                           /*ff=*/0010, jcn_local(1));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[1] = 1; mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1");
    EXPECT(cpu.T == 0xFFFF, "T = 0o%o (expected 0xFFFF)", cpu.T);
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    /* 0xFFFF cycled 8 = 0xFFFF. RMask 8 → mask = 0x00FF. result = 0xFF00. */
    EXPECT(cpu.RM[5] == 0xFF00,
           "RM[5] = 0o%o (expected 0xFF00 = 0o177400)", cpu.RM[5]);

    printf("PASS  test_shifter_rmask (RM[5] = 0o%o)\n", cpu.RM[5]);
    return 0;
}

/* Test 7: Verify an unsupported FF/JCN/ASEL halts cleanly with a
 * specific reason rather than crashing or silently advancing. */
static int test_unsupported_halts(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* IM[0]: ASEL=5 (A←Id) — not implemented (no IFU). */
    mc.im[0] = make_uinstr(0, 0, 2, 0, /*asel=*/5, 0, 0, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    int rc = dorado_cpu_step(&cpu);
    EXPECT(rc != 0, "expected halt on unsupported ASEL=5");
    EXPECT(cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL,
           "expected UNSUPPORTED_ASEL, got %s",
           cpu_halt_reason_str(cpu.halt_reason));

    printf("PASS  test_unsupported_halts (%s)\n",
           cpu_halt_reason_str(cpu.halt_reason));
    return 0;
}

/* Diagnostic: step into real Bootstrap microcode and see where the
 * CPU stops. This is not a pass/fail test — it just reports what real
 * microcode hits so we can prioritize what to implement next.
 *
 * Now wires up a real BaseBoard 6502 to serve CPReg, instead of the
 * counter stub. */
static int probe_bootstrap(void)
{
    const char *path = "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb";
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        printf("SKIP  probe_bootstrap (file not loadable)\n");
        return 0;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mb, &mc) != DM_OK) {
        printf("SKIP  probe_bootstrap (microcode load failed)\n");
        mb_free(&mb);
        return 0;
    }

    /* Stand up a BaseBoard, simulate a 3-push boot (CoolBoot — reload
     * microcode), and let it run far enough to be ready to upload
     * Initial via CPReg.
     *
     * Per the booting memo: press 0.25-2.5 s, gap <1.5 s, then wait
     * 1.5 s after final release. At our nominal 1 MHz simulated rate,
     * that's 250k-2.5M cycles per press, <1.5M cycle gap, 1.5M cycle
     * final wait. */
    static dorado_baseboard bb;
    int have_bb = 0;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0) {
        baseboard_reset(&bb);
        baseboard_run(&bb, 1000000);   /* warm up: 1 sec */
        printf("       BB after warmup: PC=0x%04X\n", baseboard_pc(&bb));

        for (int i = 0; i < 3; i++) {
            baseboard_boot_button(&bb, 1);
            baseboard_run(&bb, 400000);   /* press ~400 ms */
            baseboard_boot_button(&bb, 0);
            baseboard_run(&bb, 600000);   /* release ~600 ms */
        }
        printf("       BB after 3 presses: PC=0x%04X\n", baseboard_pc(&bb));

        /* Final wait: 1.5s for CheckBootButton dispatch, then ~10s for
         * RebootDorado to traverse CheckVCC → DiskOK → SuppliesAllUp →
         * Delay(1) → SetClockSpeed → Delay(2) → LoadDoradoCode. With
         * the analog comparator model in place, the BB reaches
         * Continuous (the steady-state polling loop) around cycle 11M. */
        baseboard_run(&bb, 12000000);
        printf("       BB after final wait: PC=0x%04X (cycles=%llu)\n",
               baseboard_pc(&bb), (unsigned long long)bb.cycles);

        have_bb = 1;
    }

    int real_start = mc.image_present[0] ? mc.image_to_real[0] : 0;
    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)real_start);
    if (have_bb) {
        cpu.baseboard = &bb;
        cpu.baseboard_cycles_per_uop = 1;
    }

    cpu_halt_reason r = dorado_cpu_run(&cpu, 1000);
    printf("PROBE  bootstrap entry=0o%o, BB=%s (PC=0x%04X, %llu cycles), "
           "Dorado ran %d cycles, halt: %s at PC=0o%o\n",
           real_start, have_bb ? "real-6502" : "counter-stub",
           have_bb ? baseboard_pc(&bb) : 0,
           have_bb ? (unsigned long long)bb.cycles : 0ULL,
           cpu.cycles, cpu_halt_reason_str(r),
           cpu.halted ? cpu.real_PC : 0);

    /* Print the offending uinstr if we halted on something we don't
     * yet handle. */
    if (cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_BSEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_FF ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_JCN) {
        const dorado_uinstr *u = &mc.im[cpu.real_PC];
        char dis[256];
        dorado_format(u, dis, sizeof dis);
        printf("       offending uinstr: %s\n", dis);
    }

    mb_free(&mb);
    return 0;  /* informational — never fail the test run */
}

/*
 * probe_full_boot — proper interleaved BB+Dorado cold boot.
 *
 * The pre-baked probe_bootstrap loads Bootstrap.MB into IM, runs the
 * BB for 16 M cycles in isolation (long after MIR strobes have flown),
 * then runs the Dorado for 1000 cycles. That misses every BB-driven
 * MIR injection.
 *
 * This probe instead runs them tick-by-tick from cycle 0, with empty
 * IM (the way real hardware comes up). The flow we expect:
 *
 *   1. BB cold-boots into WaitForInitialBoot (~5 M BB cycles).
 *   2. We press the boot button 3× → CoolBoot dispatch → RebootDorado.
 *   3. RebootDorado walks through PowerUp checks, reaches LoadDoradoCode.
 *   4. LoadDoradoCode jams Boot0 microcode into IM via DoDoradoMicroInst
 *      (MIR strobes + SetSS) and Bootstrap's IM-write IRTable entries.
 *   5. With Boot0 loaded, BB sets Link = Boot0GoLoc and jams Return →
 *      Dorado starts running Boot0 from IM.
 *   6. Boot0 streams Boot1 via CPReg (BB's SendIMBlockToDorado, ViaCP=1).
 *
 * We don't yet expect to make it all the way through, but the probe
 * tells us how far each piece gets and where the next real wall is.
 */
static int probe_full_boot(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    /* ALUFM[0] = "B" (logical pass-through) so ALUF=0 instructions
     * with LC=1 (T←Pd) deposit B onto T. The firmware sets this up
     * itself via ALUFM[0]FromQ# but we pre-load to be safe. */
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    /* ALUFM[15] (used by ALUF=17 — most IRTable entries): also "B".
     * Firmware doesn't preload; we fill in so undefined-ALUFM doesn't
     * matter when the SAME instruction's Q←B side effect is the work
     * being done. */
    mc.alufm[15] = 025; mc.alufm_present[15] = 1;

    static dorado_baseboard bb;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") != 0) {
        printf("SKIP  probe_full_boot (BB ROM not loadable)\n");
        return 0;
    }
    baseboard_reset(&bb);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.baseboard = &bb;
    cpu.baseboard_cycles_per_uop = 1;

    /* Schedule of boot-button events, in BB cycles. The Dorado-cycle
     * counter advances 1:1 with the BB once held, and at most a few
     * cycles ahead of BB once running. */
    const uint64_t T_PRESS1_DOWN  =  1000000;
    const uint64_t T_PRESS1_UP    =  1400000;
    const uint64_t T_PRESS2_DOWN  =  2000000;
    const uint64_t T_PRESS2_UP    =  2400000;
    const uint64_t T_PRESS3_DOWN  =  3000000;
    const uint64_t T_PRESS3_UP    =  3400000;
    const uint64_t T_GIVEUP       = 60000000;

    int  pressed = 0;
    int  saw_load_dorado_code = 0;
    int  saw_continuous = 0;
    int  saw_first_im_write = 0;
    int  saw_first_dorado_uop = 0;
    int  saw_first_imfetch = 0;
    int  injected_count = 0;
    int  imfetch_count = 0;
    int  dorado_held_count = 0;
    uint16_t first_im_write_addr = 0xFFFF;

    while (bb.cycles < T_GIVEUP) {
        /* Boot-button schedule. */
        if (!pressed && bb.cycles >= T_PRESS1_DOWN && bb.cycles < T_PRESS1_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS1_UP && bb.cycles < T_PRESS2_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS2_DOWN && bb.cycles < T_PRESS2_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS2_UP && bb.cycles < T_PRESS3_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS3_DOWN && bb.cycles < T_PRESS3_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS3_UP) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        }

        /* Watch for landmark BB PCs. */
        uint16_t pc = baseboard_pc(&bb);
        if (!saw_load_dorado_code && pc == 0xFAAE) saw_load_dorado_code = 1;
        if (!saw_continuous && pc == 0xF4F3)       saw_continuous = 1;

        /* Classify this Dorado step before stepping. */
        int will_inject = bb.dorado_ss_pending && bb.dorado_mir_loaded;
        int will_hold   = !will_inject && !bb.dorado_running;

        if (dorado_cpu_step(&cpu)) {
            printf("       Dorado halted: %s at PC=0o%o, BB cycle %llu (BB PC=0x%04X)\n",
                   cpu_halt_reason_str(cpu.halt_reason),
                   cpu.real_PC, (unsigned long long)bb.cycles,
                   baseboard_pc(&bb));
            if (cpu.real_PC < 4096 && mc.im_present[cpu.real_PC]) {
                const dorado_uinstr *u = &mc.im[cpu.real_PC];
                char dis[256];
                dorado_format(u, dis, sizeof dis);
                printf("       offending uinstr: %s\n", dis);
            }
            break;
        }

        if (will_hold) {
            dorado_held_count++;
        } else if (will_inject) {
            injected_count++;
            if (!saw_first_dorado_uop) {
                saw_first_dorado_uop = 1;
                printf("       first injected uop at BB cycle %llu (BB PC=0x%04X)\n",
                       (unsigned long long)bb.cycles, baseboard_pc(&bb));
            }
        } else {
            imfetch_count++;
            if (!saw_first_imfetch) {
                saw_first_imfetch = 1;
                printf("       first IM-fetched uop at BB cycle %llu, Dorado PC=0o%o\n",
                       (unsigned long long)bb.cycles, cpu.real_PC);
            }
        }

        /* Watch IM gradually filling up. */
        for (int a = 0; a < 4096; a++) {
            if (mc.im_present[a] && !saw_first_im_write) {
                saw_first_im_write = 1;
                first_im_write_addr = (uint16_t)a;
                printf("       first IM write at addr 0o%o, BB cycle %llu\n",
                       a, (unsigned long long)bb.cycles);
                break;
            }
        }
    }

    /* Final IM occupancy count + dump of zero-vs-nonzero pattern. */
    int im_filled = 0;
    printf("       IM zero addrs:");
    for (int a = 0; a < 4096; a++) {
        if (!mc.im_present[a]) continue;
        im_filled++;
        const dorado_uinstr *u = &mc.im[a];
        if (u->iw0 == 0 && u->iw1 == 0 && u->iw2 == 0) {
            printf(" 0o%o", a);
        }
    }
    putchar('\n');

    printf("PROBE  full-boot: BB ended at PC=0x%04X (cycles=%llu)\n",
           baseboard_pc(&bb), (unsigned long long)bb.cycles);
    printf("       LoadDoradoCode reached: %s    Continuous reached: %s\n",
           saw_load_dorado_code ? "yes" : "no",
           saw_continuous ? "yes" : "no");
    printf("       Dorado: %d injected, %d IM-fetched, %d held, ss=%d mir=%d\n",
           injected_count, imfetch_count, dorado_held_count,
           bb.dorado_ss_pending, bb.dorado_mir_loaded);
    printf("       IM: %d entries written (first=0o%o), final Dorado PC=0o%o\n",
           im_filled, first_im_write_addr, cpu.real_PC);
    return 0;  /* informational */
}

/*
 * Test 8: Write IM round-trip. The Bootstrap loader uses Write IM to
 * deposit Initial into IM via four IRTable entries (LH/RH × secondary
 * 0/1). Each Write IM: address from cpu->Link, 16 bits from B,
 * half-select + secondary bit from RSTK[2:3].
 *
 * We exercise all four variants and verify both the raw iw0/iw1/iw2
 * and the re-decoded fields end up where they belong.
 */
static int test_write_im(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025; mc.alufm_present[0]  = 1;  /* ALUF[0] = B */
    mc.alufm[1]  = 025; mc.alufm_present[1]  = 1;
    mc.alufm[2]  = 025; mc.alufm_present[2]  = 1;
    mc.alufm[3]  = 025; mc.alufm_present[3]  = 1;

    /* Four Write IM instructions, targeting four different IM addrs.
     *   IM[0]: RSTK=1 → LH, secondary=0 → iw0 = B,        iw2[15] = 0
     *   IM[1]: RSTK=3 → LH, secondary=1 → iw0 = B,        iw2[15] = 1
     *   IM[2]: RSTK=0 → RH, secondary=0 → iw1 = B,        iw2[14] = 0
     *   IM[3]: RSTK=2 → RH, secondary=1 → iw1 = B,        iw2[14] = 1
     *
     * B comes from BSEL=4 (0,,FF) with FF=0xAA → B = 0x00AA. */
    mc.im[0] = make_uinstr(/*rstk=*/1, 0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[1] = make_uinstr(/*rstk=*/3, 1, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[2] = make_uinstr(/*rstk=*/0, 2, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[3] = make_uinstr(/*rstk=*/2, 3, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    for (int i = 0; i < 4; i++) {
        mc.im_present[i] = 1;
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 4;

    /* Run each Write IM separately, pointing Link at a fresh target
     * each time (200, 201, 202, 203). */
    for (int i = 0; i < 4; i++) {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, (uint16_t)i);
        cpu.Link = (uint16_t)(0x200 + i);

        int rc = dorado_cpu_step(&cpu);
        EXPECT(rc == 0, "case %d step: %s", i,
               cpu_halt_reason_str(cpu.halt_reason));

        uint16_t addr = (uint16_t)(0x200 + i);
        EXPECT(mc.im_present[addr],
               "case %d: IM[0x%03X] not marked present", i, addr);
    }

    /* LH writes land in iw0; RH writes land in iw1. */
    EXPECT(mc.im[0x200].iw0 == 0x00AA,
           "LH/sec0: iw0 = 0x%04X, want 0x00AA", mc.im[0x200].iw0);
    EXPECT((mc.im[0x200].iw2 & 0x8000) == 0,
           "LH/sec0: iw2[15] should be 0");
    EXPECT(mc.im[0x201].iw0 == 0x00AA,
           "LH/sec1: iw0 = 0x%04X, want 0x00AA", mc.im[0x201].iw0);
    EXPECT((mc.im[0x201].iw2 & 0x8000) != 0,
           "LH/sec1: iw2[15] should be 1");

    EXPECT(mc.im[0x202].iw1 == 0x00AA,
           "RH/sec0: iw1 = 0x%04X, want 0x00AA", mc.im[0x202].iw1);
    EXPECT((mc.im[0x202].iw2 & 0x4000) == 0,
           "RH/sec0: iw2[14] should be 0");
    EXPECT(mc.im[0x203].iw1 == 0x00AA,
           "RH/sec1: iw1 = 0x%04X, want 0x00AA", mc.im[0x203].iw1);
    EXPECT((mc.im[0x203].iw2 & 0x4000) != 0,
           "RH/sec1: iw2[14] should be 1");

    /* The decoded fields should reflect the new iw0/iw1/iw2. For
     * iw0=0x00AA: ASEL=iw0[2:0]=2, LC=iw0[5:3]=5, BSEL=iw0[8:6]=2,
     * ALUF=iw0[12:9]=0, RSTK[2:0]=iw0[15:13]=0. */
    EXPECT(mc.im[0x201].asel == 2,
           "decoded ASEL = %d, want 2", mc.im[0x201].asel);
    EXPECT(mc.im[0x201].lc == 5,
           "decoded LC = %d, want 5", mc.im[0x201].lc);
    EXPECT(mc.im[0x201].bsel == 2,
           "decoded BSEL = %d, want 2", mc.im[0x201].bsel);
    /* RSTK = (iw2[15] << 3) | iw0[15:13] = (1 << 3) | 0 = 8. */
    EXPECT(mc.im[0x201].rstk == 8,
           "decoded RSTK = %d, want 8 (secondary bit lifted to MSB)",
           mc.im[0x201].rstk);

    printf("PASS  test_write_im (4 variants, decoded fields refresh)\n");
    return 0;
}

/*
 * STK addressing tests (HM Table 6 / page 11).
 *
 * Encoding of u->rstk in C-LSB:
 *   bit 3 = manual RSTK[0] (underflow-check flag)
 *   bit 2 = manual RSTK[1] (sign of delta)
 *   bit 1 = manual RSTK[2]
 *   bit 0 = manual RSTK[3] (LSB of delta)
 *
 * RSTK[1:3] encodes a 3-bit signed delta in [-4, +3]:
 *   0 (000) →  0     4 (100) → -4
 *   1 (001) → +1     5 (101) → -3
 *   2 (010) → +2     6 (110) → -2
 *   3 (011) → +3     7 (111) → -1
 */

/* Construct a single instruction that does:
 *   ASEL=A←RM/STK (4)   — read RM/STK[StkP] onto A
 *   BSEL=RM/STK   (1)   — same value onto B
 *   ALUF=N → ALUFM[N]=B → ALU = B
 *   LC=RM/STK←Pd  (6)   — write Pd back to RM/STK
 *   BLOCK=1            — STK access
 *   JCN=local(0)        — stay at PC=0
 * with the given RSTK encoding. */
static dorado_uinstr stk_uinstr(int rstk, int ff)
{
    return make_uinstr(rstk, /*aluf=*/0, /*bsel=*/1, /*lc=*/6,
                       /*asel=*/4, /*block=*/1, ff,
                       /*jcn=*/jcn_local(0));
}

static int test_stk_no_change(void)
{
    /* RSTK[1:3] = 0 → StkP unchanged, RSTK[0] = 0 → no underflow check.
     * u->rstk = 0b0000 = 0. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/00, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 5;
    cpu.STK[5] = 0xBEEF;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 5, "StkP changed: %u (expected 5)", cpu.StkP);
    EXPECT(cpu.STK[5] == 0xBEEF, "STK[5] = 0x%04X (expected 0xBEEF)", cpu.STK[5]);
    EXPECT(cpu.stk_ovf == 0 && cpu.stk_und == 0,
           "stk flags should be clear: ovf=%u und=%u",
           cpu.stk_ovf, cpu.stk_und);

    printf("PASS  test_stk_no_change\n");
    return 0;
}

static int test_stk_push(void)
{
    /* RSTK[1:3] = 1 → StkP+=1. RSTK[0] = 0. u->rstk = 0b0001 = 1. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/01, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 5;
    cpu.STK[5] = 0xCAFE;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 6, "after push, StkP = %u (expected 6)", cpu.StkP);
    /* Read used unadjusted StkP=5; LC wrote to STK[5] (default — no
     * ModStkPBeforeW). Value written = ALU = B = STK[5] = 0xCAFE.
     * So STK[5] = 0xCAFE still. */
    EXPECT(cpu.STK[5] == 0xCAFE, "STK[5] = 0x%04X (expected 0xCAFE)", cpu.STK[5]);

    printf("PASS  test_stk_push (StkP 5 → 6)\n");
    return 0;
}

static int test_stk_pop(void)
{
    /* RSTK[1:3] = 7 → StkP-=1 (signed -1). u->rstk = 0b0111 = 7. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/07, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 10;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 9, "after pop, StkP = %u (expected 9)", cpu.StkP);

    printf("PASS  test_stk_pop (StkP 10 → 9)\n");
    return 0;
}

static int test_stk_pop_minus_4(void)
{
    /* RSTK[1:3] = 4 → StkP-=4. u->rstk = 0b0100 = 4. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/04, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 020;     /* octal 20 = 16 decimal */

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 014, "after pop -4, StkP = 0o%o (expected 0o14)",
           cpu.StkP);

    printf("PASS  test_stk_pop_minus_4 (StkP 0o20 → 0o14)\n");
    return 0;
}

static int test_stk_overflow(void)
{
    /* StkP[2:7] = 077 (max in region), RSTK[1:3]=1 → would go to 0o100,
     * which crosses into next region. Real hardware sets StkOvf and
     * generates StkError; we just set the flag. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/01, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 077;     /* octal 77 = 63 decimal, top of region 0 */

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.stk_ovf == 1, "StkOvf should be set on push past 077");
    EXPECT(cpu.stk_und == 0, "StkUnd should NOT be set");

    printf("PASS  test_stk_overflow (077 + 1 sets StkOvf)\n");
    return 0;
}

static int test_stk_underflow_check(void)
{
    /* RSTK[0] = 1 → underflow check enabled.
     * StkP starts at 1, RSTK[1:3] = 7 (-1) → final StkP = 0 → underflow.
     * u->rstk = 0b1111 = 0o17. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/017, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 1;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.stk_und == 1, "StkUnd should be set on -1 to StkP=1");

    printf("PASS  test_stk_underflow_check (StkP=1, -1, RSTK[0]=1 → StkUnd)\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_load_constant();
    rc |= test_increment_loop();
    rc |= test_rm_roundtrip();
    rc |= test_conditional_branch_alu_zero();
    rc |= test_shifter_byte_cycle();
    rc |= test_shifter_rmask();
    rc |= test_unsupported_halts();
    rc |= test_write_im();
    rc |= test_stk_no_change();
    rc |= test_stk_push();
    rc |= test_stk_pop();
    rc |= test_stk_pop_minus_4();
    rc |= test_stk_overflow();
    rc |= test_stk_underflow_check();
    rc |= probe_bootstrap();
    rc |= probe_full_boot();
    if (rc == 0) printf("\nAll CPU tests passed.\n");
    return rc;
}
