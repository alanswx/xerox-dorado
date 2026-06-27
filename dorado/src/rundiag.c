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
 *   - set MCR per the recipe, default task 0 / RBase 0,
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
#include <strings.h>
#include "mb.h"
#include "microcode.h"
#include "cpu.h"
#include "memory.h"
#include "disasm.h"
#include "disk.h"
#include "io.h"

/* Resolve a source label to its post-placement (real) IM address, or -1. */
static int sym_real(const mb_file *mb, const dorado_microcode *mc,
                    int im_id, const char *name)
{
    int img = mb_find_symbol_addr(mb, im_id, name);
    if (img < 0 || img >= IM_SIZE) return -1;
    return (int)mc->image_to_real[img];
}

static void apply_rm_symbol_overrides(const mb_file *mb, dorado_cpu *cpu)
{
    const char *env = getenv("RUNDIAG_RM_SYMBOL");
    if (!env || !*env) return;
    int rm_id = mb_find_mem(mb, "RM");
    if (rm_id < 0) {
        fprintf(stderr, "rundiag: RUNDIAG_RM_SYMBOL ignored; no RM memory\n");
        return;
    }

    char buf[512];
    snprintf(buf, sizeof buf, "%s", env);
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '=');
        if (!eq) {
            fprintf(stderr, "rundiag: bad RUNDIAG_RM_SYMBOL token '%s'\n", tok);
            continue;
        }
        *eq++ = '\0';
        int addr = mb_find_symbol_addr(mb, rm_id, tok);
        if (addr < 0 || addr >= 256) {
            fprintf(stderr, "rundiag: RM symbol '%s' not found\n", tok);
            continue;
        }
        cpu->RM[addr] = (uint16_t)(strtol(eq, NULL, 0) & 0xFFFFu);
        fprintf(stderr, "rundiag: RM[%s=0o%o] <- 0o%o\n",
                tok, addr, cpu->RM[addr] & 0177777u);
    }
}

static void apply_imrh_symbol_overrides(const mb_file *mb, dorado_microcode *mc)
{
    const char *env = getenv("RUNDIAG_IMRH_SYMBOL");
    if (!env || !*env) return;
    int im_id = mb_find_mem(mb, "IM");
    if (im_id < 0) {
        fprintf(stderr, "rundiag: RUNDIAG_IMRH_SYMBOL ignored; no IM memory\n");
        return;
    }

    char buf[512];
    snprintf(buf, sizeof buf, "%s", env);
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '=');
        if (!eq) {
            fprintf(stderr, "rundiag: bad RUNDIAG_IMRH_SYMBOL token '%s'\n", tok);
            continue;
        }
        *eq++ = '\0';
        int img = mb_find_symbol_addr(mb, im_id, tok);
        if (img < 0 || img >= IM_SIZE || !mc->image_present[img]) {
            fprintf(stderr, "rundiag: IM symbol '%s' not found\n", tok);
            continue;
        }
        int real = mc->image_to_real[img] & (IM_SIZE - 1);
        uint16_t rh = (uint16_t)(strtol(eq, NULL, 0) & 0xFFFFu);
        dorado_uinstr *dst = &mc->im[real];
        dst->iw1 = (uint16_t)((dst->iw1 & 0x8000u) | ((rh >> 1) & 0x7FFFu));
        dst->iw2 = (uint16_t)((dst->iw2 & ~0x4000u) | ((rh & 1u) << 14));
        dorado_redecode_fields(dst);
        mc->im_present[real] = 1;
        fprintf(stderr, "rundiag: IMRH[%s image=0o%o real=0o%o] <- 0o%o\n",
                tok, img, real, rh & 0177777u);
    }
}

static void dump_rm_symbols(const mb_file *mb, const dorado_cpu *cpu)
{
    const char *env = getenv("RUNDIAG_DUMP_RM_SYMBOLS");
    if (!env || !*env) return;
    int rm_id = mb_find_mem(mb, "RM");
    if (rm_id < 0) {
        fprintf(stderr, "rundiag: RUNDIAG_DUMP_RM_SYMBOLS ignored; no RM memory\n");
        return;
    }

    char buf[512];
    snprintf(buf, sizeof buf, "%s", env);
    fprintf(stderr, "  RM symbols:\n");
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        int addr = (tok[0] >= '0' && tok[0] <= '9')
                 ? (int)strtol(tok, NULL, 0)
                 : mb_find_symbol_addr(mb, rm_id, tok);
        if (addr < 0 || addr >= 256) {
            fprintf(stderr, "    %s: not found\n", tok);
            continue;
        }
        fprintf(stderr, "    %-16s RM[0o%03o] = 0o%06o\n",
                tok, addr, cpu->RM[addr] & 0177777u);
    }
}

static const dorado_disk_geometry *rundiag_disk_geometry(const char *name)
{
    if (!name || !*name || strcasecmp(name, "t80") == 0) {
        return &DORADO_DISK_T80;
    }
    if (strcasecmp(name, "t300") == 0) {
        return &DORADO_DISK_T300;
    }
    if (strcasecmp(name, "diablo") == 0) {
        return &DORADO_DISK_DIABLO;
    }
    return NULL;
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
    apply_imrh_symbol_overrides(&mb, &mc);
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
    apply_rm_symbol_overrides(&mb, &cpu);
    dorado_io io;
    dorado_io_init(&io);
    dorado_disk_controller disk;
    dorado_disk_pack disk_pack;
    int disk_pack_attached = 0;
    if (getenv("RUNDIAG_DISK")) {
        dorado_disk_controller_init(&disk);
        const char *media = getenv("RUNDIAG_DISK_MEDIA");
        const char *disk_path = getenv("RUNDIAG_DISK_PATH");
        if (media && *media) {
            const dorado_disk_geometry *geom = rundiag_disk_geometry(media);
            if (!geom) {
                fprintf(stderr,
                        "rundiag: unknown RUNDIAG_DISK_MEDIA '%s' "
                        "(use t80, t300, or diablo)\n",
                        media);
                return 1;
            }
            if (disk_path && *disk_path) {
                if (dorado_disk_pack_load(&disk_pack, geom, disk_path) != 0) {
                    fprintf(stderr, "rundiag: cannot load %s media from %s\n",
                            media, disk_path);
                    return 1;
                }
                fprintf(stderr, "rundiag: attached %s media from %s on drive 0\n",
                        media, disk_path);
            } else if (dorado_disk_pack_create(&disk_pack, geom) != 0) {
                fprintf(stderr, "rundiag: cannot create %s disk media\n",
                        media);
                return 1;
            } else {
                fprintf(stderr, "rundiag: attached blank %s media on drive 0\n",
                        media);
            }
            dorado_disk_controller_attach_drive(&disk, 0, &disk_pack);
            disk_pack_attached = 1;
        }
        dorado_disk_controller_attach_to_io(&disk, &io);
        cpu.io = &io;
    }

    /* kernel.midas recipe: MCR=1 (disable mem stack over/underflow wakeups),
     * default task 0. RBase starts 0: the diagnostics keep their pre-loaded
     * registers (R1, RM1, R10, …) in RBase 0 (the .mb RM data: RB0RM0/R1/RM1 at
     * RM[0..]), and the framework sets RBASE[defaultRegion]=0 before use. (The
     * "rbase 17" in kernel.midas is a Midas display default, not machine
     * state.) An optional override is provided for experiments. */
    dorado_mcr_load(&mem, 0, 1);
    {
        const char *task = getenv("RUNDIAG_TASK");
        cpu.ctask = task ? (uint8_t)(strtol(task, NULL, 8) & 0xF) : 0;
        cpu.ready |= (uint16_t)(1u << cpu.ctask);
        cpu.real_PC = (uint16_t)entry;
    }
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
    uint16_t trT[TRAIL], trRB[TRAIL]; uint8_t trF[TRAIL]; uint8_t trTask[TRAIL];
    for (; steps < maxsteps; steps++) {
        uint16_t pc = cpu.real_PC;
        int t = tn % TRAIL;
        trail[t] = pc; trT[t] = cpu.T; trRB[t] = cpu.RBase;
        trTask[t] = (uint8_t)cpu.ctask;
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
        if (disk_pack_attached) {
            (void)dorado_disk_controller_tick(&disk, (uint64_t)steps + 1);
            if (dorado_disk_controller_wakeup_pending(&disk) &&
                cpu.task_tpc[DORADO_DISK_TASK] != 0177777) {
                if (getenv("RUNDIAG_DISK_WAKE_TRACE")) {
                    fprintf(stderr,
                            "rundiag: disk wake step=%ld ctask=%o pc=0o%o "
                            "ready=%04x wake=%04x dsk_tpc=0o%o\n",
                            steps, cpu.ctask & 017, cpu.real_PC & 07777,
                            cpu.ready, cpu.wakeup_pending,
                            cpu.task_tpc[DORADO_DISK_TASK] & 07777);
                }
                dorado_cpu_wakeup(&cpu, DORADO_DISK_TASK);
            }
        }
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
            fprintf(stderr, "    tk=%02o 0o%-5o T=%06o rb=%02o %c%c%c%c %-14s %s\n",
                    trTask[k], pc, trT[k], trRB[k],
                    (trF[k]&1)?'Z':'.', (trF[k]&2)?'N':'.',
                    (trF[k]&4)?'C':'.', (trF[k]&8)?'V':'.',
                    s ? s : "", dis);
        }
    }
    if (outcome != 0) dump_rm_symbols(&mb, &cpu);

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
    if (disk_pack_attached) dorado_disk_pack_free(&disk_pack);
    mb_free(&mb);
    return outcome == 0 ? 0 : 1;
}
