// =============================================================================
// sim_cedar.cpp — Cedar checkpoint lock-step harness for dorado_rtl_top
// =============================================================================
// Restores the C emulator's Cedar desktop checkpoint into a full C machine
// (dorado_machine: CPU, memory, devices AND the host services -- germ disk
// bridge, Pilot timer, Cedar keyboard/field notify, Ethernet and the STP
// server), copies the machine state into the RTL, and runs the two in
// lock-step one microinstruction at a time.
//
// The split is the one the FPGA will use: the RTL is the Dorado, the C
// machine supplies everything outside it.
//   - Slow I/O: each C device's answer to this microinstruction (Input data
//     and parity, IOAtten, whether the addressed device takes Output) is
//     captured by wrapping the C I/O registry and handed to the RTL CPU on
//     its external I/O port (io_bridge_en = 0).
//   - Host writes: every host-side write into storage or the cache
//     (dorado_host_write_hook) is mirrored into the RTL.
//   - Host register pokes (the Pilot timer ORs NWW = RM[0]; device wakeups
//     land in wakeup_pending): before each step, any CPU register that
//     differs from the RTL can only have been changed by the host since the
//     last compare, so it is copied over and counted.
// The counts printed at the end are the host port's requirements.
//
// Usage (from rtl_core/):
//   obj_cedar/Vdorado_rtl_top [steps] [snap] [pdi]
// Paths are relative to dorado/ (DORADO_DIR overrides). CEDAR_LS=1 drives
// the verify-cedar-ls input: click the CommandTool, then type a List of a
// directory on the in-process file server, which exercises the Ethernet
// and STP host services.
// =============================================================================

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <map>
#include <vector>
#include <unistd.h>

#include <verilated.h>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"

extern "C" {
#include "cpu.h"
#include "memory.h"
#include "microcode.h"
#include "mb.h"
#include "disasm.h"
#include "io.h"
#include "machine.h"
#include "typetext.h"
}

#include "cosim_sync.h"

static Vdorado_rtl_top *top;
static dorado_machine  *M;
static dorado_cpu      *C;
static dorado_memory   *MEM;
static dorado_io       *IO;

// ---- slow-I/O capture --------------------------------------------------------
struct io_cap {
    bool     read_valid;
    uint16_t read_val;
    int      read_bad;
    bool     atten_valid;
    int      atten;
};
static io_cap cap;

struct io_wrap {
    dorado_io_device        dev;
    const dorado_io_device *orig;
};

static uint16_t wrap_read(void *ctx, int task, int subtask, uint8_t tioa, int *bad) {
    io_wrap *w = (io_wrap *)ctx;
    int b = 0;
    uint16_t v = w->orig->read(w->orig->ctx, task, subtask, tioa, &b);
    if (bad) *bad = b;
    cap.read_valid = true;
    cap.read_val = v;
    cap.read_bad = b;
    return v;
}
static void wrap_write(void *ctx, int task, int subtask, uint8_t tioa, uint16_t data) {
    io_wrap *w = (io_wrap *)ctx;
    w->orig->write(w->orig->ctx, task, subtask, tioa, data);
}
static int wrap_atten(void *ctx, int task, uint8_t tioa) {
    io_wrap *w = (io_wrap *)ctx;
    int a = w->orig->attention(w->orig->ctx, task, tioa);
    cap.atten_valid = true;
    cap.atten = a ? 1 : 0;
    return a;
}

static std::map<const dorado_io_device *, io_wrap *> wraps;
static void wrap_io_registry() {
    for (int t = 0; t < DORADO_IO_TASKS; t++) {
        for (int a = 0; a < DORADO_IO_ADDRS; a++) {
            const dorado_io_device *d = IO->cells[t][a];
            if (!d) continue;
            bool is_wrap = false;
            for (auto &kv : wraps) if (&kv.second->dev == d) is_wrap = true;
            if (is_wrap) continue;
            io_wrap *w = wraps[d];
            if (!w) {
                w = new io_wrap();
                w->orig = d;
                w->dev.read = d->read ? wrap_read : nullptr;
                w->dev.write = d->write ? wrap_write : nullptr;
                w->dev.attention = d->attention ? wrap_atten : nullptr;
                w->dev.ctx = w;
                w->dev.name = d->name;
                wraps[d] = w;
            }
            IO->cells[t][a] = &w->dev;
        }
    }
}

// ---- host writes -------------------------------------------------------------
static std::vector<uint32_t> host_writes;
static void host_write_hook(struct dorado_memory *, uint32_t va) {
    host_writes.push_back(va);
}

// The munch at `va` in the RTL: storage behind it and the cache row holding it.
static void mirror_host_write(uint32_t va) {
    const dorado_map_entry *e = dorado_map_get(MEM, dorado_map_index(va));
    size_t phys = (e->wp && e->dirty)
                ? (size_t)va
                : (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
    size_t base = phys & ~(size_t)15;
    for (size_t i = 0; i < 16; i++) {
        if (base + i < MEM->storage_words && base + i < COSIM_STORAGE_WORDS)
            R_STOR(storage)[base + i] = MEM->storage[base + i];
    }
    int r = (int)((va >> 4) & DM_CACHE_ROW_MASK);
    for (int w = 0; w < DM_CACHE_WAYS; w++) {
        const dorado_cache_line *l = &MEM->cache[r].ways[w];
        R_MEM(cache_tag)[r][w] = l->tag & 0x3FFFFu;
        R_MEM(cache_valid)[r][w] = l->valid ? 1 : 0;
        R_MEM(cache_dirty)[r][w] = l->dirty ? 1 : 0;
        R_MEM(cache_wp)[r][w] = l->wp ? 1 : 0;
        R_MEM(cache_vacant)[r][w] = l->vacant ? 1 : 0;
        R_MEM(cache_being_loaded)[r][w] = l->being_loaded ? 1 : 0;
        R_MEM(cache_lru)[r][w] = MEM->cache[r].lru[w] & 3;
        for (int k = 0; k < DM_CACHE_LINE_W; k++)
            R_MEM(cache_data)[CACHE_IDX(r, w, k)] = l->data[k];
    }
}

// ---- host register pokes -----------------------------------------------------
struct poke_stats {
    uint64_t rm = 0, stk = 0, wakeup = 0, ready = 0, resched = 0, junk = 0;
    uint64_t host_write_words = 0, host_write_steps = 0;
    std::map<int, uint64_t> rm_by_addr;
};
static poke_stats ps;

static void copy_host_pokes() {
    for (int i = 0; i < RM_SIZE; i++) {
        if (R_CPU(RM)[i] != C->RM[i]) {
            R_CPU(RM)[i] = C->RM[i];
            ps.rm++;
            ps.rm_by_addr[i]++;
        }
    }
    for (int i = 0; i < STK_SIZE; i++) {
        if (R_CPU(STK)[i] != C->STK[i]) { R_CPU(STK)[i] = C->STK[i]; ps.stk++; }
    }
    if (R_CPU(wakeup_pending) != C->wakeup_pending) {
        R_CPU(wakeup_pending) = C->wakeup_pending;
        ps.wakeup++;
    }
    if (R_CPU(ready) != C->ready) { R_CPU(ready) = C->ready; ps.ready++; }
    if (R_CPU(reschedule_pending) != C->reschedule_pending) {
        R_CPU(reschedule_pending) = C->reschedule_pending;
        ps.resched++;
    }
    if (R_CPU(junk_tw_enabled) != (C->junk_tw_enabled ? 1 : 0) ||
        R_CPU(junk_tw_countdown) != C->junk_tw_countdown) {
        R_CPU(junk_tw_enabled) = C->junk_tw_enabled ? 1 : 0;
        R_CPU(junk_tw_countdown) = C->junk_tw_countdown;
        ps.junk++;
    }
}

// ---- the step hook -----------------------------------------------------------
static uint64_t steps = 0, max_steps = 0;
static uint16_t pc_before;
static uint8_t  task_before, tioa_before;
static bool     failed = false;
static uint64_t rtl_storage_check_every = 0;

static int step_hook(dorado_machine *, int after) {
    if (!after) {
        if (!host_writes.empty()) {
            ps.host_write_steps++;
            ps.host_write_words += host_writes.size();
            for (uint32_t va : host_writes) mirror_host_write(va);
            host_writes.clear();
        }
        copy_host_pokes();
        pc_before = C->real_PC;
        task_before = C->ctask;
        tioa_before = (uint8_t)C->TIOA;
        cap = io_cap{};
        return 0;
    }
    // The C step has run; hand its device answers to the RTL and step it.
    top->io_attached = 1;
    top->io_read_data = cap.read_valid ? cap.read_val : 0xFFFF;
    top->io_read_bad_parity = cap.read_valid ? (cap.read_bad ? 1 : 0) : 1;
    top->io_has_write_in = dorado_io_has_write(IO, task_before, tioa_before) ? 1 : 0;
    top->io_atten_in = cap.atten_valid ? cap.atten : 0;
    rtl_step(top);
    steps++;
    if (!compare_c_rtl(C, top, steps, "cedar", pc_before, task_before)) {
        failed = true;
        return 1;
    }
    if (steps % 1000000 == 0) {
        fprintf(stderr, "  [cedar] %llu steps lock-step, pc=0%o task=%u, host writes %llu words\n",
                (unsigned long long)steps, (unsigned)C->real_PC, (unsigned)C->ctask,
                (unsigned long long)ps.host_write_words);
    }
    return steps >= max_steps ? 1 : 0;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    setvbuf(stdout, nullptr, _IOLBF, 0);
    max_steps = argc > 1 ? strtoull(argv[1], nullptr, 0) : 2000000;
    const char *snap = argc > 2 ? argv[2] : "build/good-packs/cedar-desktop.snap";
    const char *pdi  = argc > 3 ? argv[3] : "build/good-packs/cedar-desktop.pdi";
    const char *ddir = getenv("DORADO_DIR") ? getenv("DORADO_DIR") : "../dorado";
    if (chdir(ddir) != 0) { perror(ddir); return 2; }
    if (const char *m = getenv("CEDAR_STORAGE_CHECK_MASK"))
        cosim_storage_check_mask = strtoull(m, nullptr, 0);
    else
        cosim_storage_check_mask = (1u << 20) - 1;

    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    cfg.boot_keys_count = 0;
    cfg.boot_keys[cfg.boot_keys_count++] = DORADO_KEY_NONE;   // --boot-reason disk
    cfg.alto_ether_boot = 0;                                   // --no-alto-boot
    cfg.eth_boot_110 = "../chm/dorado/CedarDorado.eb!6";
    cfg.germ_path = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
    cfg.pilot_disk_pdi[0] = pdi;
    cfg.ftp_root = "../chm/cedar/stp-root";

    M = dorado_machine_create(&cfg);
    if (!M) { fprintf(stderr, "machine create failed\n"); return 1; }
    if (dorado_machine_restore(M, snap) != 0) { fprintf(stderr, "restore %s failed\n", snap); return 1; }
    dorado_machine_set_ftp_source(M, cfg.ftp_sysout, cfg.ftp_root);
    (void)dorado_machine_set_pilot_disk(M, 0, pdi);

    C = (dorado_cpu *)dorado_machine_cpu(M);
    MEM = dorado_machine_memory(M);
    IO = dorado_machine_io(M);
    printf("[cedar] restored %s at BaseBoard cycle %llu: cpu cycles %llu, storage %zu words, pc=0%o task=%u\n",
           snap, (unsigned long long)dorado_machine_cycles(M), (unsigned long long)C->cycles, (size_t)MEM->storage_words,
           (unsigned)C->real_PC, (unsigned)C->ctask);
    if (MEM->storage_words > COSIM_STORAGE_WORDS) {
        fprintf(stderr, "[cedar] C storage %zu words > RTL %u\n",
                (size_t)MEM->storage_words, (unsigned)COSIM_STORAGE_WORDS);
        return 1;
    }

    top = new Vdorado_rtl_top;
    sync_c_to_rtl(C, top);
    top->io_bridge_en = 0;
    if (!compare_c_rtl(C, top, 0, "cedar-sync", C->real_PC, C->ctask)) {
        fprintf(stderr, "[cedar] state differs right after the copy\n");
        return 1;
    }

    // CEDAR_C_ONLY=1: run the C machine alone (same script), to check the
    // input against the native emulator quickly. CEDAR_UNTIL=<BaseBoard
    // cycle> ends the run there.
    const bool c_only = getenv("CEDAR_C_ONLY") != nullptr;
    uint64_t until = getenv("CEDAR_UNTIL") ? strtoull(getenv("CEDAR_UNTIL"), nullptr, 0) : ~0ull;
    if (c_only) max_steps = ~0ull;
    if (!c_only) {
        wrap_io_registry();
        dorado_host_write_hook = host_write_hook;
        dorado_machine_step_hook = step_hook;
    }

    // Scripted input, as dorado.c drives verify-cedar-ls: 2M-cycle frames,
    // the click at --type-at (+100M) run inline (move, 2M, press, hold
    // --key-hold 1.6M, release), the paste queued at --paste-at (+300M) and
    // pumped once per frame.
    const bool ls = getenv("CEDAR_LS") != nullptr;
    const uint64_t c0 = dorado_machine_cycles(M);
    const uint64_t frame = 2000000ull;
    bool clicked = false, typed = false;
    static dorado_typequeue tq;
    auto run_to = [&](uint64_t target) {
        while (!failed && steps < max_steps && dorado_machine_cycles(M) < target) {
            uint64_t before = dorado_machine_uinstructions(M);
            dorado_machine_run_until(M, target);
            if (dorado_machine_uinstructions(M) == before) break;
        }
    };

    auto t0 = std::chrono::steady_clock::now();
    while (!failed && steps < max_steps && dorado_machine_cycles(M) < until) {
        uint64_t target = dorado_machine_cycles(M) + frame;
        uint64_t before = dorado_machine_uinstructions(M);
        run_to(target);
        if (dorado_machine_uinstructions(M) == before) {
            fprintf(stderr, "[cedar] the C machine made no progress\n");
            break;
        }
        uint64_t now = dorado_machine_cycles(M);
        if (!ls) continue;
        if (typed) dorado_typequeue_pump(&tq, M);
        if (!clicked && now >= c0 + 100000000ull) {
            clicked = true;
            printf("[cedar] click (700,733) at cycle %llu, step %llu\n",
                   (unsigned long long)now, (unsigned long long)steps);
            dorado_machine_set_mouse(M, 700, 733, 0);
            run_to(dorado_machine_cycles(M) + 2000000ull);
            dorado_machine_set_mouse(M, 700, 733, DORADO_MOUSE_LEFT);
            run_to(dorado_machine_cycles(M) + 1600000ull);
            dorado_machine_set_mouse(M, 700, 733, 0);
        }
        if (!typed && now >= c0 + 300000000ull) {
            typed = true;
            printf("[cedar] typing at cycle %llu, step %llu\n",
                   (unsigned long long)now, (unsigned long long)steps);
            dorado_typequeue_start(&tq, "List [Cedar]<Cedar6.1>VersionMap>*\n", 800000ull, now);
        }
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();

    printf("[cedar] %llu microinstructions in lock-step, %.1f s (%.0f/s), %.2f fabric clocks each\n",
           (unsigned long long)steps, secs, steps / secs,
           steps ? (double)rtl_fabric_clocks / steps : 0.0);
    printf("[cedar] host writes: %llu words over %llu steps\n",
           (unsigned long long)ps.host_write_words, (unsigned long long)ps.host_write_steps);
    printf("[cedar] host register pokes: RM %llu, STK %llu, wakeup_pending %llu, ready %llu, "
           "reschedule %llu, junk %llu\n",
           (unsigned long long)ps.rm, (unsigned long long)ps.stk,
           (unsigned long long)ps.wakeup, (unsigned long long)ps.ready,
           (unsigned long long)ps.resched, (unsigned long long)ps.junk);
    for (auto &kv : ps.rm_by_addr)
        printf("         RM[0%03o] poked %llu times\n", kv.first, (unsigned long long)kv.second);
    if (!failed && !c_only) {
        // Final full storage compare.
        for (size_t i = 0; i < MEM->storage_words; i++) {
            if (MEM->storage[i] != R_STOR(storage)[i]) {
                printf("[cedar] storage[0%zo] C=0%o RTL=0%o\n", i, MEM->storage[i], R_STOR(storage)[i]);
                failed = true;
                break;
            }
        }
    }
    if (const char *pgm = getenv("CEDAR_PGM")) {
        int px = dorado_machine_render_display_list(M);
        dorado_display_snapshot_pgm(dorado_machine_display(M), pgm);
        printf("[cedar] cycle %llu: %d display-list pixels -> %s\n",
               (unsigned long long)dorado_machine_cycles(M), px, pgm);
    }
    printf(failed ? "  [FAIL] Cedar lock-step\n" : "  [PASS] Cedar lock-step\n");
    (void)rtl_storage_check_every;
    return failed ? 1 : 0;
}
