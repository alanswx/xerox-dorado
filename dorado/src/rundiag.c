/* rundiag — run a Dorado hardware diagnostic on the microengine.
 *
 * The PARC Dorado diagnostics (kernel, Ifu, memA, memMisc, eventCounters;
 * `getdiagnostics.cm`) were written to verify REAL hardware and self-check, so
 * running them on our emulator is real-hardware-grounded validation — it
 * exercises datapath/IFU/memory/tasking the way the hardware designers
 * intended, catching Dorado-specific bugs that an Alto oracle (ContrAlto)
 * cannot (see docs/contralto-oracle-validity.md).
 *
 * Mechanism (from the kernel sources + kernel.midas Midas recipe):
 *   - load the diagnostic .mb into IM,
 *   - set MCR per the recipe, default task 0 / rbase 17,
 *   - jump to the entry label (default BEGIN) and free-run,
 *   - PASS = reach the `done` label (the diagnostic's success point: it
 *     breakpoints, bumps ITERATIONS, then loops to BEGIN),
 *   - FAIL = reach the `ERR` label (the ERROR macro = BRANCH[ERR]),
 *   - HALT = the microengine hit an unimplemented op (reports which),
 *   - TIMEOUT = neither, within the step budget.
 *
 * Usage:
 *   rundiag <diag.mb> [entry=BEGIN] [done=done] [err=ERR] [maxsteps]
 *   rundiag '../chm/dorado/expanded/kernel.dm!38_/kernel.mb'
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mb.h"
#include "microcode.h"
#include "cpu.h"
#include "memory.h"
#include "disasm.h"

/* Resolve a source label to its post-placement (real) IM address, or -1. */
static int sym_real(const mb_file *mb, const dorado_microcode *mc,
                    int im_id, const char *name)
{
    int img = mb_find_symbol_addr(mb, im_id, name);
    if (img < 0 || img >= IM_SIZE) return -1;
    return (int)mc->image_to_real[img];
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr,
            "usage: %s <diag.mb> [entry=BEGIN] [done=done] [err=ERR] [maxsteps]\n",
            argv[0]);
        return 2;
    }
    const char *path     = argv[1];
    const char *entry_nm = argc > 2 ? argv[2] : "BEGIN";
    const char *done_nm  = argc > 3 ? argv[3] : "done";
    const char *err_nm   = argc > 4 ? argv[4] : "ERR";
    long maxsteps        = argc > 5 ? strtol(argv[5], NULL, 0) : 2000000L;

    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        fprintf(stderr, "rundiag: cannot load %s\n", path);
        return 1;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mb, &mc) != DM_OK) {
        fprintf(stderr, "rundiag: microcode placement failed\n");
        return 1;
    }
    int im_id = mb_find_mem(&mb, "IM");

    int entry = sym_real(&mb, &mc, im_id, entry_nm);
    int done  = sym_real(&mb, &mc, im_id, done_nm);
    int err   = sym_real(&mb, &mc, im_id, err_nm);
    if (entry < 0) {
        fprintf(stderr, "rundiag: entry label '%s' not found\n", entry_nm);
        return 1;
    }
    printf("rundiag: %s\n", path);
    printf("  entry %s=0o%o  done %s=0o%o  err %s=0o%o  maxsteps=%ld\n",
           entry_nm, entry, done_nm, done < 0 ? 0 : done,
           err_nm, err < 0 ? 0 : err, maxsteps);

    dorado_memory mem;
    if (dorado_memory_init(&mem) != 0) {
        fprintf(stderr, "rundiag: memory init failed\n");
        return 1;
    }
    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)entry);
    cpu.mem = &mem;

    /* kernel.midas recipe: MCR=1 (disable mem stack over/underflow wakeups),
     * default task 0. RBase starts 0: the diagnostics keep their pre-loaded
     * registers (R1, RM1, R10, …) in RBase 0 (the .mb RM data: RB0RM0/R1/RM1 at
     * RM[0..]), and the framework sets RBASE[defaultRegion]=0 before use. (The
     * "rbase 17" in kernel.midas is a Midas display default, not machine
     * state.) An optional override is provided for experiments. */
    dorado_mcr_load(&mem, 0, 1);
    cpu.ctask = 0;
    {
        const char *rb = getenv("RUNDIAG_RBASE");
        cpu.RBase = rb ? (uint16_t)(strtol(rb, NULL, 8) & 0xF) : 0;
    }

    long steps = 0;
    int outcome = -1;          /* 0 done, 1 err, 2 halt, 3 timeout */
    int halt_rc = 0;
    int reached_done = 0;
    uint16_t last_pc = (uint16_t)entry;
    /* Ring buffer of the last PCs, dumped on a non-PASS outcome so the failing
     * sub-test is visible. */
    enum { TRAIL = 48 };
    uint16_t trail[TRAIL]; int tn = 0;
    uint16_t trT[TRAIL], trRB[TRAIL]; uint8_t trF[TRAIL];
    for (; steps < maxsteps; steps++) {
        uint16_t pc = cpu.real_PC;
        int t = tn % TRAIL;
        trail[t] = pc; trT[t] = cpu.T; trRB[t] = cpu.RBase;
        trF[t] = (uint8_t)((cpu.alu_zero?1:0) | (cpu.alu_lt0?2:0) |
                           (cpu.alu_carry?4:0) | (cpu.alu_overflow?8:0));
        tn++;
        if (done >= 0 && pc == (uint16_t)done) {
            /* The diagnostic loops at `done`; treat the FIRST arrival as a
             * passing iteration and stop (we don't need the outer sim loop). */
            reached_done = 1; outcome = 0; break;
        }
        if (err >= 0 && pc == (uint16_t)err) { outcome = 1; break; }
        int rc = dorado_cpu_step(&cpu);
        if (rc != 0) { outcome = 2; halt_rc = rc; break; }
        last_pc = pc;
    }
    if (outcome < 0) outcome = 3;

    if (outcome != 0 && getenv("RUNDIAG_TRAIL")) {
        int n = tn < TRAIL ? tn : TRAIL;
        fprintf(stderr, "  PC trail (last %d; T/flags are state ON ENTRY to each pc):\n", n);
        for (int i = 0; i < n; i++) {
            int k = (tn - n + i) % TRAIL;
            uint16_t pc = trail[k];
            const char *s = dorado_microcode_symbol_at_real(&mc, pc);
            char dis[160];
            dorado_format(&mc.im[pc & (IM_SIZE-1)], dis, sizeof dis);
            fprintf(stderr, "    0o%-5o T=%06o rb=%02o %c%c%c%c %-14s %s\n",
                    pc, trT[k], trRB[k],
                    (trF[k]&1)?'Z':'.', (trF[k]&2)?'N':'.',
                    (trF[k]&4)?'C':'.', (trF[k]&8)?'V':'.',
                    s ? s : "", dis);
        }
    }

    const char *sym = dorado_microcode_symbol_at_real(&mc, cpu.real_PC);
    switch (outcome) {
    case 0:
        printf("RESULT: PASS — reached %s after %ld steps\n", done_nm, steps);
        break;
    case 1: {
        const char *lsym = dorado_microcode_symbol_at_real(&mc, last_pc);
        printf("RESULT: FAIL — reached %s (ERROR) after %ld steps; "
               "last pc before=0o%o (%s)\n", err_nm, steps, last_pc,
               lsym ? lsym : "?");
        break;
    }
    case 2: {
        char dis[160];
        dorado_format(&mc.im[cpu.real_PC & (IM_SIZE-1)], dis, sizeof dis);
        printf("RESULT: HALT (rc=%d) at pc=0o%o (%s) after %ld steps\n  %s\n",
               halt_rc, cpu.real_PC, sym ? sym : "?", steps, dis);
        break;
    }
    default:
        printf("RESULT: TIMEOUT after %ld steps; pc=0o%o (%s)\n",
               steps, cpu.real_PC, sym ? sym : "?");
        break;
    }
    (void)reached_done;
    mb_free(&mb);
    return outcome == 0 ? 0 : 1;
}
