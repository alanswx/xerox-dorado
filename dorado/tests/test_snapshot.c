/*
 * test_snapshot — bit-identical validation of machine snapshot/restore
 * (docs/cycle-accurate-timing-plan.md Phase 0, step 3: "snapshot at cycle
 * N, restore, run — must be bit-identical to not-snapshotting").
 *
 * Boots Galaxian (the known-good regression world), snapshots the running
 * game at N1, restores it into a freshly-created machine, and checks two
 * things:
 *
 *   1. Restore fidelity — the restored machine's state digest at N1 equals
 *      the original's (the snapshot captured everything that matters).
 *   2. Forward determinism — running both forward to N2 lands on the same
 *      digest (a restored game evolves identically to one that never
 *      snapshotted).
 *
 * If the boot fixture is absent (a bare checkout without chm/), the test
 * SKIPs and passes, matching the other integration tests.
 */

#include "machine.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAME "../chm/bootfiles/Galaxian.boot!1"
#define SNAP_PATH "build/test_snapshot.snap"

/* Snapshot point (post world-load, which happens ~32M) and a forward
 * window long enough to exercise the scheduler + display + RTC cadences.
 * Overridable via env for bisecting a divergence. */
static unsigned long long N1 = 40000000ULL;
static unsigned long long N2 = 45000000ULL;

static void load_window(void)
{
    const char *a = getenv("DORADO_SNAP_N1");
    const char *b = getenv("DORADO_SNAP_N2");
    if (a) N1 = strtoull(a, NULL, 10);
    if (b) N2 = strtoull(b, NULL, 10);
}

static int have_fixture(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

static dorado_machine *make_game(void)
{
    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.eftp_boot = GAME;
    return dorado_machine_create(&cfg);
}

int main(void)
{
    load_window();
    if (!have_fixture(GAME)) {
        printf("SKIP test_snapshot (missing %s)\n", GAME);
        return 0;
    }

    /* --- Original: boot, run to N1, snapshot. --- */
    dorado_machine *a = make_game();
    if (!a) {
        fprintf(stderr, "FAIL: could not create machine A\n");
        return 1;
    }
    dorado_machine_run_until(a, N1);
    uint64_t a_cyc1 = dorado_machine_cycles(a);

    if (dorado_machine_snapshot(a, SNAP_PATH) != 0) {
        fprintf(stderr, "FAIL: snapshot failed\n");
        return 1;
    }

    /* --- Restore into a fresh machine and check fidelity at N1. --- */
    dorado_machine *b = make_game();
    if (!b) {
        fprintf(stderr, "FAIL: could not create machine B\n");
        return 1;
    }
    if (dorado_machine_restore(b, SNAP_PATH) != 0) {
        fprintf(stderr, "FAIL: restore failed\n");
        return 1;
    }

    /* Recompute the framebuffer from memory on both so the digest's fb
     * component is a deterministic function of the (now-identical) state. */
    dorado_machine_render_display_list(a);
    dorado_machine_render_display_list(b);
    uint64_t dA1 = dorado_machine_state_digest(a);
    uint64_t dB1 = dorado_machine_state_digest(b);
    uint64_t b_cyc1 = dorado_machine_cycles(b);

    if (b_cyc1 != a_cyc1) {
        fprintf(stderr, "FAIL: restore cycle mismatch: A=%llu B=%llu\n",
                (unsigned long long)a_cyc1, (unsigned long long)b_cyc1);
        return 1;
    }
    if (dA1 != dB1) {
        fprintf(stderr,
                "FAIL: restore not faithful at N1: A=%016llx B=%016llx\n",
                (unsigned long long)dA1, (unsigned long long)dB1);
        return 1;
    }
    printf("PASS restore_fidelity (cyc=%llu digest=%016llx)\n",
           (unsigned long long)a_cyc1, (unsigned long long)dA1);

    /* --- Forward determinism: run both to N2, digests must match. --- */
    dorado_machine_run_until(a, N2);
    dorado_machine_run_until(b, N2);
    dorado_machine_render_display_list(a);
    dorado_machine_render_display_list(b);
    uint64_t dA2 = dorado_machine_state_digest(a);
    uint64_t dB2 = dorado_machine_state_digest(b);

    if (dorado_machine_cycles(a) != dorado_machine_cycles(b)) {
        fprintf(stderr, "FAIL: forward cycle mismatch: A=%llu B=%llu\n",
                (unsigned long long)dorado_machine_cycles(a),
                (unsigned long long)dorado_machine_cycles(b));
        return 1;
    }
    if (dA2 != dB2) {
        fprintf(stderr,
                "FAIL: forward divergence after restore: A=%016llx B=%016llx\n",
                (unsigned long long)dA2, (unsigned long long)dB2);
        return 1;
    }
    if (dA2 == dA1) {
        fprintf(stderr, "FAIL: state did not advance N1->N2 "
                "(digest unchanged) — test window is a no-op\n");
        return 1;
    }
    printf("PASS forward_determinism (cyc=%llu digest=%016llx)\n",
           (unsigned long long)dorado_machine_cycles(a),
           (unsigned long long)dA2);

    dorado_machine_destroy(a);
    dorado_machine_destroy(b);
    remove(SNAP_PATH);

    printf("All snapshot tests passed.\n");
    return 0;
}
