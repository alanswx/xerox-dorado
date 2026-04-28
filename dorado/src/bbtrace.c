/*
 * bbtrace — load doradobaserom.mb!13, run the BaseBoard 6502 with
 * boot-button presses, and report the boot-counter variables along
 * the way. Used to debug the CoolBoot dispatch path.
 *
 * Counter addresses (per `mbdis -y` on the ROM):
 *   0x52  Timer100ms
 *   0x9C  SubTicks   (50 = 1 timer tick)
 *   0x68  Problems   (Shutdown bit blocks WaitForInitialBoot fall-through)
 *   0xA1  BootSubticksOn
 *   0xA2  BootTicksOn
 *   0xA3  BootSubticksOff
 *   0xA4  BootTicksOff (≥15 → CheckBootButton dispatches)
 *   0xA5  BootPushCount (1 HotBoot, 2 WarmBoot, 3-6 CoolBoot)
 */

#include "baseboard.h"

#include <stdio.h>
#include <string.h>

static dorado_baseboard bb;

/* Watch writes to a small set of zero-page boot-state variables. */
extern void (*baseboard_write_hook)(uint16_t addr, uint8_t old, uint8_t new_, uint16_t pc);

static void watch(uint16_t addr, uint8_t old_v, uint8_t new_v, uint16_t pc)
{
    static const struct { uint16_t a; const char *name; } interesting[] = {
        { 0x52, "Timer100ms" },
        { 0x68, "Problems"   },
        { 0x9C, "SubTicks"   },
        { 0xA1, "BSOn"       },
        { 0xA2, "BTOn"       },
        { 0xA3, "BSOff"      },
        { 0xA4, "BTOff"      },
        { 0xA5, "BPush"      },
    };
    for (size_t i = 0; i < sizeof interesting / sizeof interesting[0]; i++) {
        if (interesting[i].a == addr) {
            printf("    @%llu PC=%04X  %s [%02X] %02X → %02X\n",
                   (unsigned long long)bb.cycles, pc,
                   interesting[i].name, addr, old_v, new_v);
            return;
        }
    }
}

static void dump_state(const char *tag)
{
    printf("[%-20s] cycles=%-9llu PC=%04X  "
           "Timer100=%-3u SubT=%-3u Probs=%02X  "
           "BSOn=%u BTOn=%u BSOff=%u BTOff=%-2u BPush=%u  IRQ=%llu\n",
           tag, (unsigned long long)bb.cycles, baseboard_pc(&bb),
           bb.mem[0x52], bb.mem[0x9C], bb.mem[0x68],
           bb.mem[0xA1], bb.mem[0xA2], bb.mem[0xA3], bb.mem[0xA4], bb.mem[0xA5],
           (unsigned long long)bb.irq_count);
    printf("    POS=%02X  V[VDD/VCC/VTT/VEE]=%02X %02X %02X %02X  "
           "I[IDD/ICC/ITT/IEE]=%02X %02X %02X %02X  DAC=%02X\n",
           bb.mem[0x01],
           bb.mem[0x02], bb.mem[0x03], bb.mem[0x04], bb.mem[0x05],
           bb.mem[0x06], bb.mem[0x07], bb.mem[0x08], bb.mem[0x09],
           bb.riot[0].pa_latch);
}

/* PC histogram: count entry into each 16-byte chunk. */
static uint32_t pc_hist[0x10000 / 16];

static void run_with_hist(uint32_t cycles)
{
    uint64_t end = bb.cycles + cycles;
    while (bb.cycles < end) {
        baseboard_step(&bb);
        pc_hist[baseboard_pc(&bb) / 16]++;
    }
}

int main(void)
{
    baseboard_write_hook = watch;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") != 0) {
        fprintf(stderr, "load_rom failed: %s\n", bb.halt_msg);
        return 1;
    }
    baseboard_reset(&bb);
    dump_state("post-reset");

    /* Settle the reset path. */
    baseboard_run(&bb, 200000);
    dump_state("after 200k warmup");

    baseboard_run(&bb, 800000);
    dump_state("after 1M warmup");

    /* Press 3 times, ~400ms each, ~600ms gap. */
    for (int i = 0; i < 3; i++) {
        char tag[32];
        baseboard_boot_button(&bb, 1);
        snprintf(tag, sizeof tag, "press %d start", i + 1);
        dump_state(tag);
        baseboard_run(&bb, 400000);
        snprintf(tag, sizeof tag, "press %d held", i + 1);
        dump_state(tag);

        baseboard_boot_button(&bb, 0);
        snprintf(tag, sizeof tag, "release %d start", i + 1);
        dump_state(tag);
        baseboard_run(&bb, 600000);
        snprintf(tag, sizeof tag, "release %d held", i + 1);
        dump_state(tag);
    }

    /* Wait 2 seconds for CheckBootButton to dispatch. */
    baseboard_run(&bb, 500000);
    dump_state("+0.5s idle");
    baseboard_run(&bb, 500000);
    dump_state("+1.0s idle");
    baseboard_run(&bb, 500000);
    dump_state("+1.5s idle");
    baseboard_run(&bb, 500000);
    dump_state("+2.0s idle");

    /* Now hunt for LoadDoradoCode (0xFAAE) by stepping until either we
     * reach it or burn 30s of simulated time. */
    int hit_load = 0;
    uint64_t deadline = bb.cycles + 30000000ULL;
    while (bb.cycles < deadline) {
        baseboard_step(&bb);
        if (baseboard_pc(&bb) == 0xFAAE) { hit_load = 1; break; }
    }
    if (hit_load) {
        printf("\n*** Reached LoadDoradoCode at 0xFAAE after %llu cycles\n",
               (unsigned long long)bb.cycles);
        dump_state("at LoadDoradoCode");
    } else {
        printf("\n*** Did not reach LoadDoradoCode in 30M cycles\n");
        dump_state("after 30M idle");
    }

    /* PC histogram for the last 1M cycles. */
    memset(pc_hist, 0, sizeof pc_hist);
    run_with_hist(1000000);

    /* Specifically check key dispatch addresses. */
    printf("\nKey-address hit counts in last 1M cycles:\n");
    printf("  F590..F59F (CheckBootButton entry):  %u\n", pc_hist[0xF59]);
    printf("  F5A0..F5AF                        :  %u\n", pc_hist[0xF5A]);
    printf("  F418..F41F (RebootDorado entry)  :  %u\n", pc_hist[0xF41]);
    printf("  FAAE      (LoadDoradoCode)       :  %u\n", pc_hist[0xFAA]);
    printf("  F4F3      (Continuous)           :  %u\n", pc_hist[0xF4F]);
    printf("  F2A0..F2AF (Interrupt handler)   :  %u\n", pc_hist[0xF2A]);

    /* Show all F500-F6FF chunks (CheckBootButton, dispatch, etc). */
    printf("\nAll non-zero F5XX-F6XX chunks:\n");
    for (int i = 0xF50; i <= 0xF6F; i++) {
        if (pc_hist[i] > 0)
            printf("  PC=%04X..%04X: %u hits\n", i * 16, i * 16 + 15, pc_hist[i]);
    }

    printf("\nTop 25 chunks overall:\n");
    for (int top = 0; top < 25; top++) {
        int best = -1;
        uint32_t bestv = 0;
        for (int i = 0; i < (int)(sizeof pc_hist / sizeof pc_hist[0]); i++) {
            if (pc_hist[i] > bestv) { bestv = pc_hist[i]; best = i; }
        }
        if (best < 0 || bestv == 0) break;
        printf("  PC=%04X..%04X: %u hits\n",
               best * 16, best * 16 + 15, bestv);
        pc_hist[best] = 0;
    }
    return 0;
}
