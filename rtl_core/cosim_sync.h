// =============================================================================
// cosim_sync.h — C reference <-> Verilated RTL state copy and compare
// =============================================================================
// Shared by sim_cosim.cpp (the lock-step suites) and sim_cedar.cpp (the
// Cedar checkpoint harness). Include after verilated.h, the Vdorado_rtl_top
// headers and the C model's cpu.h/memory.h/microcode.h/disasm.h.
// =============================================================================
#pragma once

#define R_CPU(field) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##field)
#define R_MEM(field) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__u_mem__DOT__##field)
// the cache data: one RAM per way, indexed {row, word}
#define R_CW(w)      ((w) == 0 ? R_MEM(cache_w0) : (w) == 1 ? R_MEM(cache_w1) : (w) == 2 ? R_MEM(cache_w2) : R_MEM(cache_w3))
#define CW_IDX(r, k) ((r) * 16 + (k))
#define R_STOR(field) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__g_storage__DOT__u_stor__DOT__##field)
#define CACHE_IDX(r, w, k) ((((r) * 4) + (w)) * 16 + (k))
#define R_IO(field)  (top->rootp->dorado_rtl_top__DOT__u_io__DOT__##field)

#ifndef COSIM_STORAGE_WORDS
#define COSIM_STORAGE_WORDS 65536u
#endif
// All of storage is compared every (mask+1) steps.
static uint64_t cosim_storage_check_mask = 255;


#include "uinstr_pack.h"

// -----------------------------------------------------------------------------
// Synchronize C reference state -> Verilated RTL state
// -----------------------------------------------------------------------------

// One microinstruction of the RTL: clock until step_done (or a halt). The RTL
// takes several fabric clocks per microinstruction; the C model takes one call.
static uint64_t rtl_fabric_clocks = 0;
static bool rtl_step_seen = false;        // a retire seen during rtl_settle
static bool g_fsm_trace = false;          // RTL_FSM_TRACE for a step range (the harness sets it)
static void rtl_step(Vdorado_rtl_top *top) {
    if (rtl_step_seen) { rtl_step_seen = false; return; }   // retired during the last settle
    for (int i = 0; i < 4096; i++) {
        top->clk = 1;
        top->eval();
        top->clk = 0;
        top->eval();
        rtl_fabric_clocks++;
        static const bool fsm_trace_env = getenv("RTL_FSM_TRACE") != nullptr;
        const bool fsm_trace = fsm_trace_env || g_fsm_trace;
        if (fsm_trace) fprintf(stderr, "  clk st=%d p=%d%d%d pc=0%o step_done=%d x_np=0%o pred=0%o fresh=%d pre_ok=%d spec=%d\n",
                               (int)R_CPU(st), (int)R_CPU(p_w), (int)R_CPU(p_x), (int)R_CPU(p_d),
                               (unsigned)R_CPU(real_PC), (int)top->step_done,
                               (unsigned)R_CPU(x_np), (unsigned)R_CPU(x_pred_np), (int)R_CPU(x_pred_fresh),
                               (int)R_CPU(rm_pre_ok), (int)R_CPU(spec_hit));
        if (fsm_trace) fprintf(stderr, "      ex_u jcn=0%o iw0=%06o iw1=%06o iw2=%06o  x_u jcn=0%o  im_q=%06o %06o %06o\n",
                               (unsigned)((R_CPU(ex_u)[1] >> 18) & 0xFF), (unsigned)(R_CPU(ex_u)[1] & 0xFFFF),
                               (unsigned)(R_CPU(ex_u)[0] >> 16), (unsigned)(R_CPU(ex_u)[0] & 0xFFFF),
                               (unsigned)((R_CPU(x_u)[1] >> 18) & 0xFF),
                               (unsigned)R_CPU(im_q0), (unsigned)R_CPU(im_q1), (unsigned)R_CPU(im_q2));
        if (fsm_trace) fprintf(stderr, "      pf: pcf=0%o(ifu 0%o) st=%d v0=%d v1=%d ev=%d pres=%d w0=%06o w1=%06o lh=%06o rh=%06o va0=%o nx_fast=%d bk_n=%d req=%d/%d req2=%d\n",
            (unsigned)R_CPU(pf_pcf), (unsigned)R_CPU(ifu_pcf), (int)R_CPU(pf_st), (int)R_CPU(pf_v0), (int)R_CPU(pf_v1),
            (int)R_CPU(pf_ev), (int)R_CPU(pf_pres), (unsigned)R_CPU(pf_w0), (unsigned)R_CPU(pf_w1),
            (unsigned)R_CPU(pf_lh), (unsigned)R_CPU(pf_rh), (unsigned)R_CPU(pf_va0), (int)R_CPU(nx_fast),
            (int)R_CPU(bk_n), (int)R_CPU(mem_req), (int)R_CPU(mem_req_tag), (int)R_CPU(mem_req2));
        if (top->step_done || top->dbg_halted) return;  // the instruction retired
    }
    fprintf(stderr, "[RTL] no step_done after 4096 clocks (pc=0%o)\n",
            (unsigned)top->dbg_real_pc);
}
// The references an IFUJump's dispatch owes are booked after the instruction
// retires, while the next executes (if it does not touch the section): clock
// until they are (cmp_done). The lock-step holds the next instruction while it
// does (hp_hold, the host's between-instructions hold): its I/O answers and
// the host's pokes come from the C model's NEXT step, which has not run yet.
// The run-ahead itself is exercised by the free-running harnesses
// (sim_cosim's boot, sim_hps), whose step sequence must not change.
static void rtl_settle(Vdorado_rtl_top *top) {
    struct hold_guard {
        Vdorado_rtl_top *t; uint8_t prev;
        ~hold_guard() { t->hp_hold = prev; t->eval(); }
    } g{top, (uint8_t)top->hp_hold};
    top->hp_hold = 1;
    for (int i = 0; i < 4096; i++) {
        if (top->cmp_done || top->dbg_halted) return;
        top->clk = 1;
        top->eval();
        top->clk = 0;
        top->eval();
        rtl_fabric_clocks++;
        static const bool fsm_trace = getenv("RTL_FSM_TRACE") != nullptr;
        if (fsm_trace) fprintf(stderr, "  settle st=%d pc=0%o step_done=%d cmp_done=%d bk_n=%d req=%d/%d\n",
                               (int)R_CPU(st), (unsigned)R_CPU(real_PC), (int)top->step_done,
                               (int)top->cmp_done, (int)R_CPU(bk_n), (int)R_CPU(mem_req), (int)R_CPU(mem_req_tag));
        if (top->step_done) rtl_step_seen = true;
    }
    fprintf(stderr, "[RTL] no cmp_done after 4096 clocks (pc=0%o)\n",
            (unsigned)top->dbg_real_pc);
}

static void sync_c_to_rtl(const dorado_cpu *c_cpu, Vdorado_rtl_top *top) {
    const dorado_memory *c_mem = c_cpu->mem;
    const dorado_microcode *mc = c_cpu->mc;

    // Reset RTL first so internal state starts clean
    top->clk = 0;
    top->rst = 1;
    top->clk_en = 1;
    top->bb_present = 0;
    top->bb_running = 1;
    top->bb_ss_inject = 0;
    top->bb_mir_iw0 = 0;
    top->bb_mir_iw1 = 0;
    top->bb_mir_iw2 = 0;
    top->io_bridge_en = 0;
    top->ov_hold = 1;                    // per-step compares (sim_cedar may lift it)
    top->display_wake_gate = 0;
    top->kbd_words = ~0ULL;
    top->mouse_buttons = 0; top->mouse_present = 0; top->mouse_x = 0; top->mouse_y = 0;
    top->eth_local_host = 042;
    rtl_step_seen = false;               // a retire seen by a previous run's settle
    top->io_attached = 0;
    top->io_read_data = 0;
    top->io_read_bad_parity = 0;
    top->io_has_write_in = 0;
    top->io_atten_in = 0;
    top->eval();
    top->clk = 1;
    top->eval();
    top->clk = 0;
    top->eval();
    top->rst = 0;
    top->eval();

#include "sync_cpu_body.inc"
    if (c_mem) {
        uint32_t limit = c_mem->storage_words < COSIM_STORAGE_WORDS
                       ? (uint32_t)c_mem->storage_words : COSIM_STORAGE_WORDS;
        for (uint32_t i = 0; i < limit; i++) {
            R_STOR(storage)[i] = c_mem->storage[i];
        }
    }
    top->eval();
}

// -----------------------------------------------------------------------------
// Step both C reference and Verilated RTL by 1 cycle and verify lock-step match
// -----------------------------------------------------------------------------
// part 0: the processor (at step_done); part 1: the memory section (at
// cmp_done, once the dispatch's references are booked); 2: both.
static bool compare_c_rtl(const dorado_cpu *c_cpu, Vdorado_rtl_top *top,
                          uint64_t step_idx, const char *test_name,
                          uint16_t pc_before, uint8_t task_before, int part = 2) {
    const dorado_memory *c_mem = c_cpu->mem;

    bool ok = true;
    auto fail = [&](const char *field, uint64_t exp, uint64_t got) {
        if (ok) {
            char dis[256] = {0};
            if (c_cpu->mc) {
                dorado_format(&c_cpu->mc->im[pc_before & 0xFFF], dis, sizeof(dis));
            }
            fprintf(stderr,
                    "\n[COSIM MISMATCH] test=%s step=%llu pc_before=0%04o task=%u (%s)\n",
                    test_name, (unsigned long long)step_idx,
                    pc_before, task_before, dis);
        }
        fprintf(stderr, "  %-20s : C=0x%llX (0%llo) != RTL=0x%llX (0%llo)\n",
                field,
                (unsigned long long)exp, (unsigned long long)exp,
                (unsigned long long)got, (unsigned long long)got);
        ok = false;
    };

  if (part != 1) {
    // After an instruction whose finish shared its clock with the next one's
    // execute (C_PIPE), the processor as the finish left it is in snap_*.
    const bool sv = R_CPU(snap_valid) != 0;
#define RSNAP(f) (sv ? (uint64_t)R_CPU(snap_##f) : (uint64_t)R_CPU(f))
    const uint8_t fl = R_CPU(snap_flags);
    if (c_cpu->real_PC != RSNAP(real_PC)) fail("real_PC", c_cpu->real_PC, RSNAP(real_PC));
    if (c_cpu->ctask   != RSNAP(ctask))   fail("ctask",   c_cpu->ctask,   RSNAP(ctask));
    if (c_cpu->ready   != RSNAP(ready))   fail("ready",   c_cpu->ready,   RSNAP(ready));
    if (c_cpu->T       != RSNAP(T))       fail("T",       c_cpu->T,       RSNAP(T));
    if (c_cpu->Q       != RSNAP(Q))       fail("Q",       c_cpu->Q,       RSNAP(Q));
    if (c_cpu->Link    != RSNAP(Link))    fail("Link",    c_cpu->Link,    RSNAP(Link));
    if (c_cpu->Cnt     != RSNAP(Cnt))     fail("Cnt",     c_cpu->Cnt,     RSNAP(Cnt));
    if (c_cpu->ShC     != RSNAP(ShC))     fail("ShC",     c_cpu->ShC,     RSNAP(ShC));
    if (c_cpu->StkP    != RSNAP(StkP))    fail("StkP",    c_cpu->StkP,    RSNAP(StkP));
    if (c_cpu->RBase   != RSNAP(RBase))   fail("RBase",   c_cpu->RBase,   RSNAP(RBase));
    if (c_cpu->MemBase != RSNAP(MemBase)) fail("MemBase", c_cpu->MemBase, RSNAP(MemBase));
    if (c_cpu->MemBX   != RSNAP(MemBX))   fail("MemBX",   c_cpu->MemBX,   RSNAP(MemBX));
    {
        const unsigned rz = sv ? (fl >> 3) & 1 : R_CPU(alu_zero), rl = sv ? (fl >> 2) & 1 : R_CPU(alu_lt0);
        const unsigned rc = sv ? (fl >> 1) & 1 : R_CPU(alu_carry), ro = sv ? fl & 1 : R_CPU(alu_overflow);
        if ((c_cpu->alu_zero ? 1u : 0u)     != rz) fail("alu_zero",     c_cpu->alu_zero,     rz);
        if ((c_cpu->alu_lt0 ? 1u : 0u)      != rl) fail("alu_lt0",      c_cpu->alu_lt0,      rl);
        if ((c_cpu->alu_carry ? 1u : 0u)    != rc) fail("alu_carry",    c_cpu->alu_carry,    rc);
        if ((c_cpu->alu_overflow ? 1u : 0u) != ro) fail("alu_overflow", c_cpu->alu_overflow, ro);
    }

    for (int i = 0; i < RM_SIZE; i++) {
        if (c_cpu->RM[i] != R_CPU(RM)[i]) {
            char name[32];
            snprintf(name, sizeof(name), "RM[0%03o]", i);
            fail(name, c_cpu->RM[i], R_CPU(RM)[i]);
        }
    }
    for (int i = 0; i < STK_SIZE; i++) {
        if (c_cpu->STK[i] != R_CPU(STK)[i]) {
            char name[32];
            snprintf(name, sizeof(name), "STK[0%03o]", i);
            fail(name, c_cpu->STK[i], R_CPU(STK)[i]);
        }
    }
    for (int t = 0; t < 16; t++) {
        if (c_cpu->task_tpc[t] != R_CPU(task_tpc)[t]) {
            char name[32];
            snprintf(name, sizeof(name), "task_tpc[%d]", t);
            fail(name, c_cpu->task_tpc[t], R_CPU(task_tpc)[t]);
        }
    }

    if (c_mem) {
        if (c_mem->mcr != RSNAP(mcr)) fail("mem.mcr", c_mem->mcr, RSNAP(mcr));
        if (c_mem->proc_srn != RSNAP(proc_srn)) fail("mem.proc_srn", c_mem->proc_srn, RSNAP(proc_srn));
        for (int i = 0; i < DM_BR_COUNT; i++) {
            const uint64_t rb = sv ? (uint64_t)R_CPU(snap_br)[i] : (uint64_t)R_CPU(br)[i];
            if (c_mem->br[i] != rb) {
                char name[32];
                snprintf(name, sizeof(name), "mem.br[%d]", i);
                fail(name, c_mem->br[i], rb);
            }
        }
    }
  }
#undef RSNAP
  if (part != 0 && c_mem) {
    {
        if (c_mem->md  != R_MEM(md_q))  fail("mem.md",  c_mem->md,  R_MEM(md_q));
        if (c_mem->mar != R_MEM(mar)) fail("mem.mar", c_mem->mar, R_MEM(mar));
        if (c_mem->asrn     != R_MEM(asrn))     fail("mem.asrn",     c_mem->asrn,     R_MEM(asrn));
        // Data paths (the RTL moves lines over several clocks): every valid
        // cache line every step, all of storage every 256 steps.
        for (int r = 0; r < DM_CACHE_ROWS && ok; r++) {
            for (int w = 0; w < DM_CACHE_WAYS && ok; w++) {
                if (!c_mem->cache[r].ways[w].valid) continue;
                if (c_mem->cache[r].ways[w].tag != R_MEM(cache_tag)[r][w]) {
                    fail("cache_tag", c_mem->cache[r].ways[w].tag, R_MEM(cache_tag)[r][w]);
                    break;
                }
                for (int k = 0; k < DM_CACHE_LINE_W; k++) {
                    if (c_mem->cache[r].ways[w].data[k] != R_CW(w)[CW_IDX(r, k)]) {
                        char name[48];
                        snprintf(name, sizeof(name), "cache_data[%d][%d][%d]", r, w, k);
                        fail(name, c_mem->cache[r].ways[w].data[k], R_CW(w)[CW_IDX(r, k)]);
                        break;
                    }
                }
            }
        }
        if ((step_idx & cosim_storage_check_mask) == cosim_storage_check_mask) {
            uint32_t lim = c_mem->storage_words < COSIM_STORAGE_WORDS
                         ? (uint32_t)c_mem->storage_words : COSIM_STORAGE_WORDS;
            for (uint32_t i = 0; i < lim; i++) {
                if (c_mem->storage[i] != R_STOR(storage)[i]) {
                    char name[32];
                    snprintf(name, sizeof(name), "storage[0%o]", i);
                    fail(name, c_mem->storage[i], R_STOR(storage)[i]);
                    break;
                }
            }
        }
    }
  }

    return ok;
}

static bool step_and_compare(dorado_cpu *c_cpu, Vdorado_rtl_top *top,
                             uint64_t step_idx, const char *test_name) {
    uint16_t pc_before = c_cpu->real_PC;
    uint8_t  task_before = c_cpu->ctask;

    // 1. Step C reference
    dorado_cpu_step(c_cpu);

    // 2. Step RTL by one microinstruction: the processor at its retire, the
    //    memory section once the dispatch's references are booked
    //    RTL_OV=1 (a PIPELINE build): the next instruction may execute beside
    //    this one's finish (C_PIPE); the processor is then compared as the
    //    finish left it, and the memory section at the next retire without one.
    static const bool ov = getenv("RTL_OV") != nullptr;
    top->ov_hold = ov ? 0 : 1;
    rtl_step(top);
    if (!compare_c_rtl(c_cpu, top, step_idx, test_name, pc_before, task_before, 0)) return false;
    if (R_CPU(snap_valid)) return true;
    rtl_settle(top);
    return compare_c_rtl(c_cpu, top, step_idx, test_name, pc_before, task_before, 1);
}
