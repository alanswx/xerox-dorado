// =============================================================================
// sim_cosim.cpp — Lock-Step Verilator Cosimulation Harness for dorado_rtl_top
// =============================================================================
// Steps the Verilated 1-clock-per-microinstruction SystemVerilog Dorado core
// (`dorado_rtl_top`) in cycle-by-cycle lock-step against the schematic-audited
// C reference (`dorado/src/cpu.c` and `dorado/src/memory.c`).
//
// Verifies:
//   1. ALU operations (arithmetic, logical, Carry20, Overflow, ALUFM table)
//   2. Barrel Shifter (L/R/C shifts, WF/RF masking, ShC, field descriptors)
//   3. RM/STK addressing, ModStkPBeforeW, Return + explicit Link<- override
//   4. Memory subsystem (BR<-, Map<-, Fetch<-, Store<-, DummyRef<-, Pipe0..5,
//      4-way Cache hits/misses, Hold stall recovery, TgetsMd)
//   5. IFU instruction prefetch & IFUJump dispatch across instruction sets
//   6. 16-way hardware task switching (task_schedule, Block, Wakeup, Junk)
//   7. Real Xerox Dorado .mb microcode lock-step execution (Bootstrap.mb,
//      Initial.mb, kernel.mb, AEmu.mb) + wall-clock speed benchmark
// =============================================================================

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>

#include <verilated.h>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"

extern "C" {
#include "cpu.h"
#include "memory.h"
#include "microcode.h"
#include "mb.h"
#include "disasm.h"
#include "ethernet.h"
}

static constexpr uint32_t COSIM_STORAGE_WORDS = 65536;

#define R_CPU(field) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##field)
#define R_MEM(field) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##field)
#define CACHE_IDX(r, w, k) ((((r) * 4) + (w)) * 16 + (k))
#define R_IO(field)  (top->rootp->dorado_rtl_top__DOT__u_io__DOT__##field)

static void pack_uinstr_words(dorado_uinstr *u) {
    u->iw0 = (uint16_t)(((u->rstk & 7) << 13) |
                        ((u->aluf & 0xF) << 9) |
                        ((u->bsel & 7) << 6) |
                        ((u->lc & 7) << 3) |
                        (u->asel & 7));
    u->iw1 = (uint16_t)(((u->block & 1) << 15) |
                        ((u->ff & 0xFF) << 7) |
                        ((u->jcn >> 1) & 0x7F));
    u->iw2 = (uint16_t)((((u->rstk >> 3) & 1) << 15) |
                        ((u->jcn & 1) << 14) |
                        ((u->brk_p & 3) << 12));
}

// -----------------------------------------------------------------------------
// Synchronize C reference state -> Verilated RTL state
// -----------------------------------------------------------------------------

// One microinstruction of the RTL: clock until step_done (or a halt). The RTL
// takes several fabric clocks per microinstruction; the C model takes one call.
static uint64_t rtl_fabric_clocks = 0;
static void rtl_step(Vdorado_rtl_top *top) {
    for (int i = 0; i < 4096; i++) {
        top->clk = 1;
        top->eval();
        top->clk = 0;
        top->eval();
        rtl_fabric_clocks++;
        if (top->step_done || top->dbg_halted) return;
    }
    fprintf(stderr, "[RTL] no step_done after 4096 clocks (pc=0%o)\n",
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
    top->display_wake_gate = 0;
    top->eth_local_host = 042;
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

    // Copy IM, IFUM, ALUFM from mc
    if (mc) {
        for (int i = 0; i < IM_SIZE; i++) {
            dorado_uinstr u = mc->im[i];
            if (u.iw0 == 0 && u.iw1 == 0 && u.iw2 == 0 && mc->im_present[i]) {
                pack_uinstr_words(&u);
            }
            R_CPU(im_iw0)[i] = u.iw0;
            R_CPU(im_iw1)[i] = u.iw1;
            R_CPU(im_iw2)[i] = u.iw2;
            R_CPU(im_present)[i] = mc->im_present[i] ? 1 : 0;
            R_CPU(im_breakpoint)[i] = mc->im_breakpoint[i] ? 1 : 0;
        }
        for (int i = 0; i < IFUM_SIZE; i++) {
            R_CPU(ifum_lo)[i] = mc->ifum_lo[i];
            R_CPU(ifum_hi)[i] = mc->ifum_hi[i];
            R_CPU(ifum_present)[i] = mc->ifum_present[i] ? 1 : 0;
        }
        for (int i = 0; i < ALUFM_SIZE; i++) {
            R_CPU(alufm)[i] = mc->alufm[i] & 0x3F;
            R_CPU(alufm_present)[i] = mc->alufm_present[i] ? 1 : 0;
        }
    }

    // Copy RM & STK
    for (int i = 0; i < RM_SIZE; i++) {
        R_CPU(RM)[i] = c_cpu->RM[i];
    }
    for (int i = 0; i < STK_SIZE; i++) {
        R_CPU(STK)[i] = c_cpu->STK[i];
    }

    // Copy CPU registers
    R_CPU(real_PC) = c_cpu->real_PC;
    R_CPU(prev_PC) = c_cpu->prev_PC;
    R_CPU(T) = c_cpu->T;
    R_CPU(Q) = c_cpu->Q;
    R_CPU(Link) = c_cpu->Link;
    R_CPU(Cnt) = c_cpu->Cnt;
    R_CPU(ShC) = c_cpu->ShC;
    R_CPU(RBase) = (uint8_t)c_cpu->RBase;
    R_CPU(MemBase) = (uint8_t)c_cpu->MemBase;
    R_CPU(MemBX) = (uint8_t)(c_cpu->MemBX & 3);
    R_CPU(TIOA) = (uint8_t)c_cpu->TIOA;
    R_CPU(StkP) = (uint8_t)c_cpu->StkP;
    R_CPU(stk_ovf) = c_cpu->stk_ovf ? 1 : 0;
    R_CPU(stk_und) = c_cpu->stk_und ? 1 : 0;
    R_CPU(cpreg) = c_cpu->cpreg;
    R_CPU(io_bad_parity) = c_cpu->io_bad_parity ? 1 : 0;
    R_CPU(io_atten_at_issue) = c_cpu->io_atten_at_issue ? 1 : 0;
    R_CPU(b_link_read) = c_cpu->b_link_read;
    R_CPU(b_link_read_valid) = c_cpu->b_link_read_valid ? 1 : 0;
    R_CPU(b_link_read_age) = c_cpu->b_link_read_age ? 1 : 0;
    R_CPU(link_at_issue) = c_cpu->link_at_issue;

    R_CPU(alu_zero) = c_cpu->alu_zero ? 1 : 0;
    R_CPU(alu_lt0) = c_cpu->alu_lt0 ? 1 : 0;
    R_CPU(alu_carry) = c_cpu->alu_carry ? 1 : 0;
    R_CPU(alu_overflow) = c_cpu->alu_overflow ? 1 : 0;

    R_CPU(ctask) = c_cpu->ctask & 0xF;
    R_CPU(ready) = c_cpu->ready;
    R_CPU(wakeup_pending) = c_cpu->wakeup_pending;
    R_CPU(wakeup_pipe)[0] = c_cpu->wakeup_pipe[0];
    R_CPU(wakeup_pipe)[1] = c_cpu->wakeup_pipe[1];
    R_CPU(tasking_on) = c_cpu->tasking_on ? 1 : 0;
    R_CPU(tasking_resume_delay) = c_cpu->tasking_resume_delay;

    for (int t = 0; t < 16; t++) {
        R_CPU(task_t)[t] = c_cpu->task_t[t];
        R_CPU(task_tpc)[t] = c_cpu->task_tpc[t];
        R_CPU(task_link)[t] = c_cpu->task_link[t];
        R_CPU(task_membase)[t] = c_cpu->task_membase[t];
        R_CPU(task_rbase)[t] = c_cpu->task_rbase[t];
        R_CPU(task_tioa)[t] = c_cpu->task_tioa[t];
        R_CPU(task_alu_zero)[t] = c_cpu->task_alu_zero[t] ? 1 : 0;
        R_CPU(task_alu_lt0)[t] = c_cpu->task_alu_lt0[t] ? 1 : 0;
        R_CPU(task_alu_carry)[t] = c_cpu->task_alu_carry[t] ? 1 : 0;
        R_CPU(task_alu_overflow)[t] = c_cpu->task_alu_overflow[t] ? 1 : 0;
        R_CPU(task_subtask)[t] = c_cpu->task_subtask[t] & 3;
        R_CPU(task_dispatch)[t] = c_cpu->task_dispatch[t];
        R_CPU(task_md)[t] = c_cpu->task_md[t];
        R_CPU(task_md_valid)[t] = c_cpu->task_md_valid[t] ? 1 : 0;
        R_CPU(task_md_ready)[t] = c_cpu->task_md_ready[t];
    }

    R_CPU(ifu_pcf) = c_cpu->ifu_pcf;
    R_CPU(ifu_pcx) = c_cpu->ifu_pcx;
    R_CPU(ifu_opcode) = c_cpu->ifu_opcode;
    R_CPU(ifu_alpha) = c_cpu->ifu_alpha;
    R_CPU(ifu_beta) = c_cpu->ifu_beta;
    R_CPU(ifu_insset) = c_cpu->ifu_insset & 3;
    R_CPU(ifu_idcnt) = c_cpu->ifu_idcnt;
    R_CPU(ifu_length) = c_cpu->ifu_length;
    R_CPU(ifu_sign) = c_cpu->ifu_sign ? 1 : 0;
    R_CPU(ifu_packed_a) = c_cpu->ifu_packed_a ? 1 : 0;
    R_CPU(ifu_type_pause) = c_cpu->ifu_type_pause ? 1 : 0;
    R_CPU(ifu_type_jump) = c_cpu->ifu_type_jump ? 1 : 0;
    R_CPU(ifu_n) = c_cpu->ifu_n & 0xF;
    R_CPU(ifu_active) = c_cpu->ifu_active ? 1 : 0;
    R_CPU(ifu_warmup) = c_cpu->ifu_warmup;
    R_CPU(ifu_test) = c_cpu->ifu_test;
    R_CPU(ifu_test_pending) = c_cpu->ifu_test_pending ? 1 : 0;
    R_CPU(ifu_test_count) = c_cpu->ifu_test_count & 0xF;
    for (int i = 0; i < 8; i++) {
        R_CPU(ifu_test_queue)[i] = c_cpu->ifu_test_queue[i];
    }
    R_CPU(ifu_saved_stkp) = c_cpu->ifu_saved_stkp;
    R_CPU(ifu_save_stkp_pending) = c_cpu->ifu_save_stkp_pending ? 1 : 0;
    R_CPU(ifu_dispatch_rbase) = c_cpu->ifu_dispatch_rbase;
    R_CPU(ifu_dispatch_count) = c_cpu->ifu_dispatch_count;

    R_CPU(reschedule_pending) = c_cpu->reschedule_pending;
    R_CPU(reschedule_cond) = c_cpu->reschedule_cond ? 1 : 0;
    R_CPU(brk_opcode) = c_cpu->brk_opcode;
    R_CPU(brk_pending) = c_cpu->brk_pending ? 1 : 0;

    R_CPU(event_cnt_a) = c_cpu->event_cnt_a;
    R_CPU(event_cnt_b) = c_cpu->event_cnt_b;
    R_CPU(event_cnt_ctrl_hi) = c_cpu->event_cnt_ctrl_hi & 0xF;
    R_CPU(event_cnt_ctrl_lo) = c_cpu->event_cnt_ctrl_lo;
    R_CPU(gen_io_mode) = c_cpu->gen_io_mode ? 1 : 0;
    R_CPU(evc_a_enable) = c_cpu->evc_a_enable ? 1 : 0;
    R_CPU(evc_a_event) = c_cpu->evc_a_event & 7;
    R_CPU(evc_a_tasksall) = c_cpu->evc_a_tasksall ? 1 : 0;
    R_CPU(evc_b_enable) = c_cpu->evc_b_enable ? 1 : 0;
    R_CPU(evc_b_event) = c_cpu->evc_b_event & 7;
    R_CPU(evc_b_tasksall) = c_cpu->evc_b_tasksall ? 1 : 0;
    R_CPU(evc_events) = c_cpu->evc_events;

    R_CPU(tasksim) = c_cpu->tasksim & 0x7F;
    R_CPU(tasksim_fired) = c_cpu->tasksim_fired ? 1 : 0;
    R_CPU(tasksim_loaded) = c_cpu->tasksim_loaded ? 1 : 0;
    R_CPU(holdsim) = c_cpu->holdsim;
    R_CPU(holdsim_loaded) = c_cpu->holdsim_loaded ? 1 : 0;
    R_CPU(holdsim_delay) = c_cpu->holdsim_delay ? 1 : 0;
    R_CPU(holdsim_hold) = c_cpu->holdsim_hold ? 1 : 0;

    R_CPU(junk_tw_enabled) = c_cpu->junk_tw_enabled ? 1 : 0;
    R_CPU(junk_tw_countdown) = c_cpu->junk_tw_countdown;

    R_CPU(cycles) = c_cpu->cycles;
    R_CPU(halted) = c_cpu->halted ? 1 : 0;
    R_CPU(halt_reason) = (uint8_t)c_cpu->halt_reason & 0xF;

    // Copy Memory state if attached
    if (c_mem) {
        for (int i = 0; i < DM_BR_COUNT; i++) {
            R_MEM(br)[i] = c_mem->br[i] & 0x0FFFFFFFu;
        }
        for (int i = 0; i < DM_MAP_ENTRIES; i++) {
            R_MEM(map_rp)[i] = c_mem->map[i].rp;
            R_MEM(map_wp)[i] = c_mem->map[i].wp ? 1 : 0;
            R_MEM(map_dirty)[i] = c_mem->map[i].dirty ? 1 : 0;
            R_MEM(map_ref)[i] = c_mem->map[i].ref ? 1 : 0;
        }
        for (int r = 0; r < DM_CACHE_ROWS; r++) {
            for (int w = 0; w < DM_CACHE_WAYS; w++) {
                R_MEM(cache_tag)[r][w] = c_mem->cache[r].ways[w].tag & 0x3FFFFu;
                R_MEM(cache_valid)[r][w] = c_mem->cache[r].ways[w].valid ? 1 : 0;
                R_MEM(cache_dirty)[r][w] = c_mem->cache[r].ways[w].dirty ? 1 : 0;
                R_MEM(cache_wp)[r][w] = c_mem->cache[r].ways[w].wp ? 1 : 0;
                R_MEM(cache_vacant)[r][w] = c_mem->cache[r].ways[w].vacant ? 1 : 0;
                R_MEM(cache_being_loaded)[r][w] = c_mem->cache[r].ways[w].being_loaded ? 1 : 0;
                R_MEM(cache_lru)[r][w] = c_mem->cache[r].lru[w] & 3;
                for (int k = 0; k < DM_CACHE_LINE_W; k++) {
                    R_MEM(cache_data)[CACHE_IDX(r, w, k)] = c_mem->cache[r].ways[w].data[k];
                }
            }
        }
        for (int s = 0; s < DM_PIPE_DEPTH; s++) {
            R_MEM(pipe_kind)[s] = (uint8_t)c_mem->pipe[s].kind;
            R_MEM(pipe_task)[s] = c_mem->pipe[s].task & 0xF;
            R_MEM(pipe_subtask)[s] = c_mem->pipe[s].subtask & 3;
            R_MEM(pipe_ref_type)[s] = c_mem->pipe[s].ref_type & 3;
            R_MEM(pipe_va)[s] = c_mem->pipe[s].va & 0x0FFFFFFFu;
            R_MEM(pipe_map_rp_pre)[s] = c_mem->pipe[s].map_rp_pre;
            R_MEM(pipe_map_flags_pre)[s] = c_mem->pipe[s].map_flags_pre & 7;
            R_MEM(pipe_mapbuf_busy)[s] = c_mem->pipe[s].mapbuf_busy ? 1 : 0;
            R_MEM(pipe_cache_flags)[s] = c_mem->pipe[s].cache_flags;
            R_MEM(pipe_pipe4_errors)[s] = c_mem->pipe[s].pipe4_errors;
            R_MEM(pipe_pipe4_syndrome)[s] = c_mem->pipe[s].pipe4_syndrome;
            R_MEM(pipe_pipe4_quadword)[s] = c_mem->pipe[s].pipe4_quadword & 3;
        }
        R_MEM(pipe_head) = c_mem->pipe_head;
        R_MEM(proc_srn) = c_mem->proc_srn & 0xF;
        R_MEM(asrn) = c_mem->asrn & 0xF;
        R_MEM(fault_count) = c_mem->fault_count & 0xF;
        R_MEM(fault_first_srn) = c_mem->fault_first_srn & 0xF;
        R_MEM(fault_emulator) = c_mem->fault_emulator ? 1 : 0;
        R_MEM(last_fault) = (uint8_t)c_mem->last_fault;
        R_MEM(last_fault_va) = c_mem->last_fault_va & 0x0FFFFFFFu;
        R_MEM(last_fault_task) = c_mem->last_fault_task & 0xF;
        R_MEM(last_fault_subtask) = c_mem->last_fault_subtask & 3;
        R_MEM(last_fault_ref_kind) = (uint8_t)c_mem->last_fault_ref_kind;
        R_MEM(mar) = c_mem->mar & 0x0FFFFFFFu;
        R_MEM(md) = c_mem->md;
        R_MEM(dbuf) = c_mem->dbuf;
        R_MEM(mcr) = c_mem->mcr;
        R_MEM(dmux_addr) = c_mem->dmux_addr & 0xFFF;
        R_MEM(dmux_pending) = c_mem->dmux_pending ? 1 : 0;
        R_MEM(mapbuf_busy_slot) = c_mem->mapbuf_busy_slot;
        R_MEM(mapbuf_busy_cycles) = c_mem->mapbuf_busy_cycles;
        R_MEM(last_cache_row) = c_mem->last_cache_row;
        R_MEM(last_cache_way) = c_mem->last_cache_way;
        R_MEM(last_ref_kind) = (uint8_t)c_mem->last_ref_kind;
        R_MEM(last_ref_va) = c_mem->last_ref_va & 0x0FFFFFFFu;
        R_MEM(last_ref_b) = c_mem->last_ref_b;
        R_MEM(last_ref_task) = c_mem->last_ref_task & 0xF;
        R_MEM(last_ref_subtask) = c_mem->last_ref_subtask & 3;
        R_MEM(last_ref_miss) = c_mem->last_ref_miss ? 1 : 0;
        R_MEM(last_ref_latency) = c_mem->last_ref_latency;

        uint32_t limit = c_mem->storage_words < COSIM_STORAGE_WORDS
                       ? (uint32_t)c_mem->storage_words : COSIM_STORAGE_WORDS;
        for (uint32_t i = 0; i < limit; i++) {
            R_MEM(storage)[i] = c_mem->storage[i];
        }
    }
    top->eval();
}

// -----------------------------------------------------------------------------
// Step both C reference and Verilated RTL by 1 cycle and verify lock-step match
// -----------------------------------------------------------------------------
static bool step_and_compare(dorado_cpu *c_cpu, Vdorado_rtl_top *top,
                             uint64_t step_idx, const char *test_name) {
    uint16_t pc_before = c_cpu->real_PC;
    uint8_t  task_before = c_cpu->ctask;

    // 1. Step C reference
    dorado_cpu_step(c_cpu);

    // 2. Step RTL by 1 clock edge
    rtl_step(top);

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

    if (c_cpu->real_PC != R_CPU(real_PC)) fail("real_PC", c_cpu->real_PC, R_CPU(real_PC));
    if (c_cpu->ctask   != R_CPU(ctask))   fail("ctask",   c_cpu->ctask,   R_CPU(ctask));
    if (c_cpu->ready   != R_CPU(ready))   fail("ready",   c_cpu->ready,   R_CPU(ready));
    if (c_cpu->T       != R_CPU(T))       fail("T",       c_cpu->T,       R_CPU(T));
    if (c_cpu->Q       != R_CPU(Q))       fail("Q",       c_cpu->Q,       R_CPU(Q));
    if (c_cpu->Link    != R_CPU(Link))    fail("Link",    c_cpu->Link,    R_CPU(Link));
    if (c_cpu->Cnt     != R_CPU(Cnt))     fail("Cnt",     c_cpu->Cnt,     R_CPU(Cnt));
    if (c_cpu->ShC     != R_CPU(ShC))     fail("ShC",     c_cpu->ShC,     R_CPU(ShC));
    if (c_cpu->StkP    != R_CPU(StkP))    fail("StkP",    c_cpu->StkP,    R_CPU(StkP));
    if (c_cpu->RBase   != R_CPU(RBase))   fail("RBase",   c_cpu->RBase,   R_CPU(RBase));
    if (c_cpu->MemBase != R_CPU(MemBase)) fail("MemBase", c_cpu->MemBase, R_CPU(MemBase));
    if (c_cpu->MemBX   != R_CPU(MemBX))   fail("MemBX",   c_cpu->MemBX,   R_CPU(MemBX));

    if ((c_cpu->alu_zero ? 1 : 0)     != R_CPU(alu_zero))     fail("alu_zero",     c_cpu->alu_zero,     R_CPU(alu_zero));
    if ((c_cpu->alu_lt0 ? 1 : 0)      != R_CPU(alu_lt0))      fail("alu_lt0",      c_cpu->alu_lt0,      R_CPU(alu_lt0));
    if ((c_cpu->alu_carry ? 1 : 0)    != R_CPU(alu_carry))    fail("alu_carry",    c_cpu->alu_carry,    R_CPU(alu_carry));
    if ((c_cpu->alu_overflow ? 1 : 0) != R_CPU(alu_overflow)) fail("alu_overflow", c_cpu->alu_overflow, R_CPU(alu_overflow));

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
        if (c_mem->md  != R_MEM(md))  fail("mem.md",  c_mem->md,  R_MEM(md));
        if (c_mem->mar != R_MEM(mar)) fail("mem.mar", c_mem->mar, R_MEM(mar));
        if (c_mem->mcr != R_MEM(mcr)) fail("mem.mcr", c_mem->mcr, R_MEM(mcr));
        if (c_mem->proc_srn != R_MEM(proc_srn)) fail("mem.proc_srn", c_mem->proc_srn, R_MEM(proc_srn));
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
                    if (c_mem->cache[r].ways[w].data[k] != R_MEM(cache_data)[CACHE_IDX(r, w, k)]) {
                        char name[48];
                        snprintf(name, sizeof(name), "cache_data[%d][%d][%d]", r, w, k);
                        fail(name, c_mem->cache[r].ways[w].data[k], R_MEM(cache_data)[CACHE_IDX(r, w, k)]);
                        break;
                    }
                }
            }
        }
        if ((step_idx & 255) == 255) {
            uint32_t lim = c_mem->storage_words < COSIM_STORAGE_WORDS
                         ? (uint32_t)c_mem->storage_words : COSIM_STORAGE_WORDS;
            for (uint32_t i = 0; i < lim; i++) {
                if (c_mem->storage[i] != R_MEM(storage)[i]) {
                    char name[32];
                    snprintf(name, sizeof(name), "storage[0%o]", i);
                    fail(name, c_mem->storage[i], R_MEM(storage)[i]);
                    break;
                }
            }
        }
        for (int i = 0; i < DM_BR_COUNT; i++) {
            if (c_mem->br[i] != R_MEM(br)[i]) {
                char name[32];
                snprintf(name, sizeof(name), "mem.br[%d]", i);
                fail(name, c_mem->br[i], R_MEM(br)[i]);
            }
        }
    }

    return ok;
}

// -----------------------------------------------------------------------------
// Helper to plant an instruction in dorado_microcode IM
// -----------------------------------------------------------------------------
static dorado_uinstr make_uinstr(int rstk, int aluf, int bsel, int lc,
                                 int asel, int block, int ff, int jcn) {
    dorado_uinstr u;
    memset(&u, 0, sizeof(u));
    u.rstk  = (uint8_t)rstk;
    u.aluf  = (uint8_t)aluf;
    u.bsel  = (uint8_t)bsel;
    u.lc    = (uint8_t)lc;
    u.asel  = (uint8_t)asel;
    u.block = (uint8_t)block;
    u.ff    = (uint8_t)ff;
    u.jcn   = (uint8_t)jcn;
    pack_uinstr_words(&u);
    return u;
}

static uint8_t jcn_local(int target_in_page) {
    return (uint8_t)(0x80 | (target_in_page & 0x3F));
}

static void plant_im(dorado_microcode *mc, uint16_t addr, dorado_uinstr u) {
    pack_uinstr_words(&u);
    mc->im[addr & 0xFFF] = u;
    mc->im_present[addr & 0xFFF] = 1;
}

static dorado_uinstr make_nop_local(uint16_t next_in_page) {
    // ASEL=6 (A<-T), BSEL=2 (B<-T), ALUF=0, LC=0 (NoLoad), FF=0 (Nop)
    return make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(next_in_page));
}

// -----------------------------------------------------------------------------
// Suite 1: ALU, Carry20, Overflow, Shifter, WF/RF, RM/STK, ModStkPBeforeW, Return+Link<-
// -----------------------------------------------------------------------------
static bool test_alu_shifter_rm_stk(Vdorado_rtl_top *top) {
    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    mc->alufm[0] = 031; mc->alufm_present[0] = 1; // 0
    mc->alufm[1] = 014; mc->alufm_present[1] = 1; // A + B
    mc->alufm[2] = 054; mc->alufm_present[2] = 1; // A + B + 1
    mc->alufm[3] = 062; mc->alufm_present[3] = 1; // A - B
    mc->alufm[4] = 022; mc->alufm_present[4] = 1; // A - B - 1
    mc->alufm[5] = 023; mc->alufm_present[5] = 1; // A ^ B
    mc->alufm[6] = 035; mc->alufm_present[6] = 1; // A & B
    mc->alufm[7] = 027; mc->alufm_present[7] = 1; // A | B
    mc->alufm[8] = 025; mc->alufm_present[8] = 1; // B
    mc->alufm[9] = 006; mc->alufm_present[9] = 1; // 2*A

    // 0100: T <- T + RM[0], RM[0] <- Pd (ASEL=6(A<-T), BSEL=1(RM/STK), rstk=0, aluf=1, lc=7(T<-Pd, RM/STK<-Pd))
    plant_im(mc, 0100, make_uinstr(0, 1, 1, 7, 6, 0, 0, jcn_local(001)));

    // 0101: T <- RM[4] + T with Carry20 (FA=0 FB=2 FC=6 -> FF=0026; ASEL=4(A<-RM/STK), rstk=4, BSEL=2(B<-T), aluf=1, lc=1(T<-Pd))
    plant_im(mc, 0101, make_uinstr(4, 1, 2, 1, 4, 0, 0026, jcn_local(002)));

    // 0102: Load T <- RM[2] (0x7FFF) (ASEL=6, BSEL=1, rstk=2, aluf=8(B), lc=1(T<-Pd))
    plant_im(mc, 0102, make_uinstr(2, 8, 1, 1, 6, 0, 0, jcn_local(003)));

    // 0103: T <- T + RM[3] (0x7FFF + 1 -> 0x8000, Overflow=1)
    plant_im(mc, 0103, make_uinstr(3, 1, 1, 1, 6, 0, 0, jcn_local(004)));

    // 0104: Conditional branch on Overflow (condition 3 = Overflow; JCN[4:3]=2 -> offset 4+R -> 0104/0105)
    // Wait: let's jump to 0106 if no overflow, 0107 if overflow (JCN = (0<<5)|(3<<3)|3 = 0033)
    plant_im(mc, 0104, make_uinstr(0, 0, 2, 0, 6, 0, 0, 0033));

    // 0106: False target (should be skipped) -> load T <- 0xEE
    plant_im(mc, 0106, make_uinstr(0, 8, 4, 1, 6, 0, 0xEE, jcn_local(010)));

    // 0107: True target (Overflow=1!) -> jump to 0110
    plant_im(mc, 0107, make_nop_local(010));

    // 0110: Shifter WF (ASEL=5 in HM Table 8: WF is ASEL=5! Wait: let's check ASEL codes:
    // ASEL=5 is WF, ASEL=4 is A<-RM/STK, ASEL=6 is A<-T, ASEL=7 is Shift/RF!)
    plant_im(mc, 0110, make_uinstr((1 << 2) | 2, 8, 1, 6, 5, 0, (1 << 6) | (2 << 4) | 8, jcn_local(011)));

    // 0111: StkP push (+1) + ModStkPBeforeW (FF=0021): STK[StkP+1] <- 2*T (ASEL=6, BSEL=1, rstk=011(push 1), aluf=9, lc=6)
    plant_im(mc, 0111, make_uinstr(011, 9, 1, 6, 6, 0, 0021, jcn_local(012)));

    // 0112: Local Call to 0120 (JCN[7:6]=11 -> 0xC0 | 020 = 0320; Link <- 0113)
    plant_im(mc, 0112, make_uinstr(0, 0, 2, 0, 6, 0, 0, 0320));

    // 0120: Return (JCN=0006) WITH explicit Link<-B (FF=0073) — Cedar float bug regression test!
    plant_im(mc, 0120, make_uinstr(0, 0, 1, 0, 6, 0, 0073, 0006));

    // 0113: Nop loop
    plant_im(mc, 0113, make_nop_local(013));

    dorado_cpu_init(cpu, mc, 0100);
    cpu->mem   = mem;
    cpu->RBase = 0;
    cpu->StkP  = 010;
    cpu->T     = 0x1234;
    cpu->Q     = 0x5678;
    cpu->RM[0] = 0x1111;
    cpu->RM[1] = 0x2222;
    cpu->RM[2] = 0x7FFF;
    cpu->RM[3] = 0x0001;
    cpu->RM[4] = 0x000F;
    cpu->STK[010] = 0xAAAA;
    cpu->STK[011] = 0xBBBB;

    sync_c_to_rtl(cpu, top);
    for (int step = 0; step < 12; step++) {
        if (!step_and_compare(cpu, top, step, "test_alu_shifter_rm_stk")) {
            dorado_memory_free(mem); free(mem); free(cpu); free(mc);
            return false;
        }
    }

    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
    printf("  [PASS] Suite 1: ALU, Carry20, Overflow, WF/RF Shifter, ModStkPBeforeW, Return+Link<-\n");
    return true;
}

// -----------------------------------------------------------------------------
// Suite 2: Memory Subsystem (BR<-, Map<-, Fetch<-, Store<-, DummyRef<-, Pipe0..5, Hold)
// -----------------------------------------------------------------------------
static bool test_memory_subsystem(Vdorado_rtl_top *top) {
    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    mc->alufm[0] = 031; mc->alufm_present[0] = 1; // 0
    mc->alufm[1] = 000; mc->alufm_present[1] = 1; // A
    mc->alufm[2] = 025; mc->alufm_present[2] = 1; // B
    mc->alufm[3] = 014; mc->alufm_present[3] = 1; // A + B

    mem->mcr = 0x0000;
    mem->map[0x0010].rp    = 0x0002;
    mem->map[0x0010].wp    = 0;
    mem->map[0x0010].dirty = 1;
    mem->map[0x0010].ref   = 1;
    for (int i = 0; i < 256; i++) {
        mem->storage[0x0200 + i] = (uint16_t)(0xA000 + i);
    }

    // 0200: Fetch<- T (ASEL=3: Fetch<- when FF[1:0]=0; A=T(via ASEL=3? Wait: ASEL=2 is A<-T for memref!))
    // In HM Table 8: ASEL=2 is Fetch/Store/etc with Mar<-ALU, A=T! FF[1:0]=3 is Fetch<-!
    // Let's use ASEL=2, ALUF=1 (ALU=A=T), BSEL=1, RSTK=0, LC=0, FF=0003 (Fetch<-)
    plant_im(mc, 0200, make_uinstr(0, 1, 1, 0, 2, 0, 0003, jcn_local(001)));

    // 0201: RM[1] <- Md, T <- Md (TgetsMd: BSEL=0(Md), RSTK=1, LC=5(RM/STK<-Md), FF=0075(TgetsMd))
    plant_im(mc, 0201, make_uinstr(1, 1, 0, 5, 6, 0, 0075, jcn_local(002)));

    // 0202: Store<- T, Data = RM[0] (ASEL=1: Store/Flush/etc, FF[1:0]=1 is Store<-, BSEL=1(RM[0]), ALUF=1(A=T))
    plant_im(mc, 0202, make_uinstr(0, 1, 1, 0, 1, 0, 0001, jcn_local(003)));

    // 0203: Fetch<- T again (now a cache hit with 0xCAFE!)
    plant_im(mc, 0203, make_uinstr(2, 1, 1, 0, 2, 0, 0003, jcn_local(004)));

    // 0204: RM[2] <- Md (LC=5, RSTK=2, BSEL=1)
    plant_im(mc, 0204, make_uinstr(2, 2, 1, 5, 6, 0, 0, jcn_local(005)));

    // 0205: DummyRef<- T (ASEL=1, FF[1:0]=2 is DummyRef<-)
    plant_im(mc, 0205, make_uinstr(0, 1, 1, 0, 1, 0, 0002, jcn_local(006)));

    // 0206: Read Pipe0' (VALo') into T (FF=0150: B<-Pipe0', ALUF=2(B), LC=1(T<-Pd))
    plant_im(mc, 0206, make_uinstr(0, 2, 1, 1, 6, 0, 0150, jcn_local(007)));

    // 0207: Read Pipe3' (Map data') into Q (FF=0153: B<-Pipe3', ALUF=2(B), LC=2(Q<-Pd))
    plant_im(mc, 0207, make_uinstr(0, 2, 1, 2, 6, 0, 0153, jcn_local(010)));

    plant_im(mc, 0210, make_nop_local(010));

    dorado_cpu_init(cpu, mc, 0200);
    cpu->mem     = mem;
    cpu->MemBase = 0;
    mem->br[0]   = 0x001000;
    cpu->T       = 0x0005;
    cpu->RM[0]   = 0xCAFE;
    cpu->RM[1]   = 0x0000;

    sync_c_to_rtl(cpu, top);
    for (int step = 0; step < 30; step++) {
        if (!step_and_compare(cpu, top, step, "test_memory_subsystem")) {
            dorado_memory_free(mem); free(mem); free(cpu); free(mc);
            return false;
        }
    }

    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
    printf("  [PASS] Suite 2: Memory Subsystem (Fetch/Store/DummyRef/Cache Miss Hold/TgetsMd/Pipe0..5)\n");
    return true;
}

// -----------------------------------------------------------------------------
// Suite 3: IFU Instruction Prefetch & IFUJump Bytecode Dispatch
// -----------------------------------------------------------------------------
static bool test_ifu_and_tasks(Vdorado_rtl_top *top) {
    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    mc->alufm[0] = 031; mc->alufm_present[0] = 1; // 0
    mc->alufm[1] = 000; mc->alufm_present[1] = 1; // A
    mc->alufm[2] = 025; mc->alufm_present[2] = 1; // B
    mc->alufm[3] = 014; mc->alufm_present[3] = 1; // A + B

    mem->mcr = 0x0000;
    mem->map[0x0020].rp    = 0x0004;
    mem->map[0x0020].wp    = 0;
    mem->map[0x0020].dirty = 1;
    mem->map[0x0020].ref   = 1;

    mem->storage[0x0400] = 0x1020;
    mem->storage[0x0401] = 0x3005;

    auto set_ifum = [&](uint8_t op, uint16_t ifuad, uint8_t n, uint8_t len) {
        uint16_t inv_ad = (~ifuad) & 0xFFF;
        uint8_t  inv_n  = (~n) & 0xF;
        uint8_t  inv_len = (len ^ 3) & 3;
        mc->ifum_lo[op] = (uint16_t)((inv_len << 14) | (1 << 13) | (1 << 11) | (inv_n << 5) | (inv_ad & 0x1F));
        mc->ifum_hi[op] = (uint16_t)((inv_ad >> 5) & 0x7F);
        mc->ifum_present[op] = 1;
    };
    set_ifum(0x10, 0300, 3, 1);
    set_ifum(0x20, 0310, 5, 2);

    // 0250: PCF <- B (FF=0064: PCF<-B, B=RM[0]=0)
    plant_im(mc, 0250, make_uinstr(0, 2, 1, 0, 6, 0, 0064, jcn_local(051)));
    plant_im(mc, 0251, make_nop_local(052));
    plant_im(mc, 0252, make_nop_local(053));
    plant_im(mc, 0253, make_nop_local(054));
    plant_im(mc, 0254, make_nop_local(055));
    plant_im(mc, 0255, make_nop_local(056));
    // 0256: IFUJump(0) (JCN = 0004: IFUJump[0])
    plant_im(mc, 0256, make_uinstr(0, 0, 2, 0, 6, 0, 0, 0004));

    // 0300: Handler for opcode 0x10: T <- Id (TisId: FF=0036, ASEL=6 -> A=Id), RM[5]<-Pd, then IFUJump(0)!
    plant_im(mc, 0300, make_uinstr(5, 1, 1, 7, 6, 0, 0036, 0004));

    // 0310: Handler for opcode 0x20 (len=2): RM[6] <- Id via TisId (FF=0036)
    plant_im(mc, 0310, make_uinstr(6, 1, 1, 6, 6, 0, 0036, jcn_local(011)));
    plant_im(mc, 0311, make_nop_local(011));

    dorado_cpu_init(cpu, mc, 0250);
    cpu->mem     = mem;
    mem->br[31]  = 0x002000;
    mem->br[0]   = 0x002000;
    cpu->MemBase = 0;
    cpu->T       = 0;
    cpu->RM[0]   = 0;

    sync_c_to_rtl(cpu, top);
    for (int step = 0; step < 25; step++) {
        if (!step_and_compare(cpu, top, step, "test_ifu_and_tasks")) {
            dorado_memory_free(mem); free(mem); free(cpu); free(mc);
            return false;
        }
    }

    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
    printf("  [PASS] Suite 3: IFU Prefetch, PCF<-B, IFUJump Dispatch & TisId Byte Stream\n");
    return true;
}

// -----------------------------------------------------------------------------
// Suite 4: Real Xerox Dorado Microcode (.mb) Lock-Step Verification
// -----------------------------------------------------------------------------
static bool test_real_microcode_mb(Vdorado_rtl_top *top, const char *mb_path,
                                   uint16_t start_pc, int num_cycles) {
    FILE *check = fopen(mb_path, "rb");
    if (!check) {
        printf("  [SKIP] %s not found\n", mb_path);
        return true;
    }
    fclose(check);

    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, mb_path) != MB_OK) {
        fprintf(stderr, "  [FAIL] Failed to load %s\n", mb_path);
        return false;
    }

    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    if (dorado_microcode_load(&mb, mc) != DM_OK) {
        fprintf(stderr, "  [FAIL] dorado_microcode_load failed on %s\n", mb_path);
        mb_free(&mb);
        free(mc);
        return false;
    }
    if (!mc->alufm_present[0]) {
        static const uint8_t standard_alufm[ALUFM_SIZE] = {
            025, 000, 014, 054, 062, 022, 035, 027,
            023, 031, 040, 036, 013, 033, 001, 006,
        };
        for (int i = 0; i < ALUFM_SIZE; i++) {
            mc->alufm[i] = standard_alufm[i];
            mc->alufm_present[i] = 1;
        }
    }
    uint16_t real_entry = (start_pc == 0xFFFF && mc->image_present[0])
                        ? mc->image_to_real[0]
                        : start_pc;

    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    dorado_cpu_init(cpu, mc, real_entry);
    cpu->mem = mem;
    cpu->cpreg = 0x8000;

    sync_c_to_rtl(cpu, top);

    int steps_done = 0;
    for (int step = 0; step < num_cycles; step++) {
        if (!step_and_compare(cpu, top, step, mb_path)) {
            mb_free(&mb);
            dorado_memory_free(mem); free(mem); free(cpu); free(mc);
            return false;
        }
        steps_done++;
        if (cpu->halted) break;
    }

    printf("  [PASS] Real .mb Lock-Step (%d cycles, 0 mismatches): %s\n",
           steps_done, mb_path);

    mb_free(&mb);
    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
    return true;
}

// -----------------------------------------------------------------------------
// Suite 5: Pure RTL Simulation Speed Benchmark
// -----------------------------------------------------------------------------
static void benchmark_rtl_speed(Vdorado_rtl_top *top, const char *mb_path,
                                uint16_t start_pc, int bench_cycles) {
    FILE *check = fopen(mb_path, "rb");
    if (!check) return;
    fclose(check);

    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, mb_path) != MB_OK) return;

    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    if (dorado_microcode_load(&mb, mc) != DM_OK) {
        mb_free(&mb); free(mc);
        return;
    }
    if (!mc->alufm_present[0]) {
        static const uint8_t standard_alufm[ALUFM_SIZE] = {
            025, 000, 014, 054, 062, 022, 035, 027,
            023, 031, 040, 036, 013, 033, 001, 006,
        };
        for (int i = 0; i < ALUFM_SIZE; i++) {
            mc->alufm[i] = standard_alufm[i];
            mc->alufm_present[i] = 1;
        }
    }

    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    dorado_cpu_init(cpu, mc, start_pc);
    cpu->mem = mem;

    sync_c_to_rtl(cpu, top);

    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < bench_cycles; i++) {
        rtl_step(top);
    }
    auto t1 = std::chrono::steady_clock::now();
    double elapsed_s = std::chrono::duration<double>(t1 - t0).count();
    double mhz = (bench_cycles / 1e6) / elapsed_s;
    double dorado_ratio = mhz / 16.666667;

    printf("  [BENCH] Pure Verilated RTL speed (%d cycles of %s): %.2f MHz (%.2fx real Dorado)\n",
           bench_cycles, mb_path, mhz, dorado_ratio);

    mb_free(&mb);
    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
}

// -----------------------------------------------------------------------------
// Suite 4b: Real Xerox Dorado .eb Microcode World Lock-Step Verification
// -----------------------------------------------------------------------------
static bool test_real_microcode_eb(Vdorado_rtl_top *top, const char *eb_path,
                                   int num_cycles) {
    FILE *fp = fopen(eb_path, "rb");
    if (!fp) {
        printf("  [SKIP] %s not found\n", eb_path);
        return true;
    }
    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return false;
    }

    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    static const uint8_t standard_alufm[ALUFM_SIZE] = {
        025, 000, 014, 054, 062, 022, 035, 027,
        023, 031, 040, 036, 013, 033, 001, 006,
    };
    for (int i = 0; i < ALUFM_SIZE; i++) {
        mc->alufm[i] = standard_alufm[i];
        mc->alufm_present[i] = 1;
    }

    uint16_t start_pc = 0;
    for (;;) {
        uint16_t w[4];
        bool eof = false;
        for (int i = 0; i < 4; i++) {
            int hi = fgetc(fp);
            int lo = fgetc(fp);
            if (hi == EOF || lo == EOF) { eof = true; break; }
            w[i] = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
        }
        if (eof) break;
        int type = w[0] & 7;
        uint16_t addr = w[1];
        if (type == 2) {
            start_pc = w[3];
            break;
        } else if (type == 0) {
            uint16_t a = addr & 0x0FFFu;
            if (a >= 07600 && a < 07700) continue;
            uint16_t extra = (uint16_t)(w[0] >> 12);
            uint16_t rstk0 = (uint16_t)((extra >> 2) & 1u);
            uint16_t block = (uint16_t)(extra & 1u);
            dorado_uinstr *u = &mc->im[a];
            u->iw0 = w[2];
            u->iw1 = (uint16_t)((block << 15) | ((w[3] >> 1) & 0x7FFFu));
            u->iw2 = (uint16_t)((rstk0 << 15) | ((w[3] & 1u) << 14));
            u->awd = a;
            dorado_redecode_fields(u);
            mc->im_present[a] = 1;
        } else if (type == 1) {
            uint16_t a = addr & 0x03FFu;
            mc->ifum_lo[a] = w[2];
            mc->ifum_hi[a] = w[3];
            mc->ifum_present[a] = 1;
        } else if (type == 3) {
            uint16_t a = addr & 0x00FFu;
            mc->rm[a] = w[2];
            mc->rm_present[a] = 1;
        }
    }
    fclose(fp);

    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    dorado_cpu_init(cpu, mc, start_pc);
    cpu->mem = mem;

    sync_c_to_rtl(cpu, top);

    int steps_done = 0;
    for (int step = 0; step < num_cycles; step++) {
        if (!step_and_compare(cpu, top, step, eb_path)) {
            dorado_memory_free(mem); free(mem); free(cpu); free(mc);
            return false;
        }
        steps_done++;
        if (cpu->halted) break;
    }

    printf("  [PASS] Real .eb Lock-Step (%d cycles, 0 mismatches): %s\n",
           steps_done, eb_path);

    dorado_memory_free(mem); free(mem); free(cpu); free(mc);
    return true;
}

// -----------------------------------------------------------------------------
// Suite 6: Full RTL I/O Bridge Boot Test (AEmu.eb + EBoot + Galaxian over EFTP)
// -----------------------------------------------------------------------------
static bool test_rtl_io_boot(Vdorado_rtl_top *top, const char *eb_path,
                             const char *boot_file_path, int max_cycles) {
    FILE *fp = fopen(eb_path, "rb");
    if (!fp) {
        printf("  [SKIP] %s not found\n", eb_path);
        return true;
    }
    FILE *bf = fopen(boot_file_path, "rb");
    if (!bf) {
        fclose(fp);
        printf("  [SKIP] %s not found\n", boot_file_path);
        return true;
    }
    fseek(bf, 0, SEEK_END);
    long boot_bytes = ftell(bf);
    fclose(bf);
    // EFTP carries the file in 512-byte data packets, then an End packet.
    const unsigned eftp_expected_seq = (unsigned)((boot_bytes + 511) / 512) + 1;

    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return false;
    }

    dorado_microcode *mc = (dorado_microcode *)calloc(1, sizeof(dorado_microcode));
    static const uint8_t standard_alufm[ALUFM_SIZE] = {
        025, 000, 014, 054, 062, 022, 035, 027,
        023, 031, 040, 036, 013, 033, 001, 006,
    };
    for (int i = 0; i < ALUFM_SIZE; i++) {
        mc->alufm[i] = standard_alufm[i];
        mc->alufm_present[i] = 1;
    }

    uint16_t start_pc = 0;
    for (;;) {
        uint16_t w[4];
        bool eof = false;
        for (int i = 0; i < 4; i++) {
            int hi = fgetc(fp);
            int lo = fgetc(fp);
            if (hi == EOF || lo == EOF) { eof = true; break; }
            w[i] = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
        }
        if (eof) break;
        int type = w[0] & 7;
        uint16_t addr = w[1];
        if (type == 2) {
            start_pc = w[3];
            break;
        } else if (type == 0) {
            uint16_t a = addr & 0x0FFFu;
            if (a >= 07600 && a < 07700) continue;
            uint16_t extra = (uint16_t)(w[0] >> 12);
            uint16_t rstk0 = (uint16_t)((extra >> 2) & 1u);
            uint16_t block = (uint16_t)(extra & 1u);
            dorado_uinstr *u = &mc->im[a];
            u->iw0 = w[2];
            u->iw1 = (uint16_t)((block << 15) | ((w[3] >> 1) & 0x7FFFu));
            u->iw2 = (uint16_t)((rstk0 << 15) | ((w[3] & 1u) << 14));
            u->awd = a;
            dorado_redecode_fields(u);
            mc->im_present[a] = 1;
        } else if (type == 1) {
            uint16_t a = addr & 0x03FFu;
            mc->ifum_lo[a] = w[2];
            mc->ifum_hi[a] = w[3];
            mc->ifum_present[a] = 1;
        } else if (type == 3) {
            uint16_t a = addr & 0x00FFu;
            mc->rm[a] = w[2];
            mc->rm_present[a] = 1;
        }
    }
    fclose(fp);

    dorado_cpu *cpu = (dorado_cpu *)calloc(1, sizeof(dorado_cpu));
    dorado_memory *mem = (dorado_memory *)calloc(1, sizeof(dorado_memory));
    dorado_memory_init(mem);
    mem->storage_words = COSIM_STORAGE_WORDS;

    dorado_cpu_init(cpu, mc, start_pc);
    cpu->mem = mem;

    // Seed RM from .eb if present
    for (int i = 0; i < RM_SIZE; i++) {
        if (mc->rm_present[i]) cpu->RM[i] = mc->rm[i];
    }

    // Identity-map installed storage and Alto MDS aliases (matching InitMap.mc)
    // Also map virtual bank 017776 (0x3FFE00) -> page 0..255 because EIBR/EOBR
    // use negative indexing from {EmuBRHiReg-1,,ptr+count} when EmuBRHiReg==0!
    uint32_t pages = COSIM_STORAGE_WORDS / DM_PAGE_SIZE;
    for (uint32_t pg = 0; pg < pages; pg++) {
        dorado_map_set(mem, pg, (uint16_t)pg, 0, 0);
        dorado_map_set(mem, 0x200u + pg, (uint16_t)pg, 0, 0);
        dorado_map_set(mem, 0xFF00u + pg, (uint16_t)pg, 0, 0);
        dorado_map_set(mem, (0x3500000u >> 8) + pg, (uint16_t)pg, 0, 0);
    }

    sync_c_to_rtl(cpu, top);

    // Enable the internal RTL I/O bridge!
    top->io_bridge_en = 1;
    top->display_wake_gate = 0;
    top->eth_local_host = 042;
    top->eval();

    // Hold BS key (word 0, bit 15 -> 0xFFFE) so AEmu's Start.mc selects EBoot
    R_IO(ddc_kbd_words)[0] = 0xFFFE;
    R_IO(ddc_kbd_words)[1] = 0xFFFF;
    R_IO(ddc_kbd_words)[2] = 0xFFFF;
    R_IO(ddc_kbd_words)[3] = 0xFFFF;
    R_IO(ddc_kbd_words)[4] = 0xFFFF;

    // Initialize C Ethernet server to serve the Alto boot file over Mayday/EFTP
    dorado_ethernet eth;
    dorado_ethernet_init(&eth);
    dorado_ethernet_set_eftp_boot_file(&eth, boot_file_path);

    auto sync_eth_rx_to_rtl = [&]() {
        if (eth.rx_words && eth.rx_attention && eth.rx_pos < eth.rx_count &&
            R_IO(eth_rx_pos) >= R_IO(eth_rx_count)) {
            int n = (int)(eth.rx_count - eth.rx_pos);
            if (n > 4096) n = 4096;
            for (int i = 0; i < n; i++) {
                R_IO(eth_rx_words)[i] = eth.rx_words[eth.rx_pos + i];
                R_IO(eth_rx_attention)[i] = eth.rx_attention[eth.rx_pos + i] ? 1 : 0;
            }
            R_IO(eth_rx_pos) = 0;
            R_IO(eth_rx_count) = n;
            eth.rx_pos = eth.rx_count;
        }
    };

    bool cold_ac_done = false;
    const bool trace_disp = getenv("RTL_IFUDISP_TRACE") != nullptr;
    uint32_t next_bol_cycle = 100000;
    uint64_t eit_cycles = 0;
    uint16_t eit_pcs[128];
    int eit_pc_n = 0;
    auto read_rtl_va = [&](uint32_t va) -> uint16_t {
        uint32_t row = (va >> 4) & 0x3Fu;
        uint32_t tag = va >> 10;
        uint32_t off = va & 0xFu;
        for (int w = 0; w < 4; w++) {
            if (R_MEM(cache_valid)[row][w] && R_MEM(cache_tag)[row][w] == tag)
                return R_MEM(cache_data)[CACHE_IDX(row, w, off)];
        }
        uint32_t vp = (va >> 8) & 0xFFFFu;
        uint32_t rp = R_MEM(map_rp)[vp];
        uint32_t pa = (rp << 8) | (va & 0xFFu);
        return (pa < COSIM_STORAGE_WORDS) ? R_MEM(storage)[pa] : 0;
    };
    auto t0 = std::chrono::steady_clock::now();

    for (int cyc = 0; cyc < max_cycles; cyc++) {
        uint16_t pre_pc = R_CPU(real_PC);
        uint8_t  ctask  = R_CPU(ctask);
        if (ctask == 7 && pre_pc != 02424 && pre_pc != 02417) {
            if (eit_pc_n < 128 && (eit_pc_n == 0 || eit_pcs[eit_pc_n - 1] != pre_pc))
                eit_pcs[eit_pc_n++] = pre_pc;
        }
        if (ctask == 7) eit_cycles++;
        if (ctask == 6 && eit_pc_n < 120) {
            if (eit_pc_n == 0 || eit_pcs[eit_pc_n - 1] != (010000 | pre_pc))
                eit_pcs[eit_pc_n++] = (010000 | pre_pc);
        }

        // Cold-Alto AC & I/O page initialization when AEmu reaches EBoot (02006)
        if (!cold_ac_done && ctask == 0 && (pre_pc == 02005 || pre_pc == 02006)) {
            R_CPU(STK)[1] = 0;
            R_CPU(STK)[2] = 0;
            R_CPU(STK)[3] = 0;
            R_CPU(STK)[4] = 0;
            for (uint32_t va = 0177000u; va <= 0177777u; va++) {
                uint32_t vp = (va >> 8) & 0xFFFFu;
                uint32_t rp = R_MEM(map_rp)[vp];
                uint32_t pa = (rp << 8) | (va & 0xFFu);
                if (pa < COSIM_STORAGE_WORDS) R_MEM(storage)[pa] = 0177777u;
            }
            for (uint32_t va = 0177740u; va <= 0177757u; va++) {
                uint32_t vp = (va >> 8) & 0xFFFFu;
                uint32_t rp = R_MEM(map_rp)[vp];
                uint32_t pa = (rp << 8) | (va & 0xFFu);
                if (pa < COSIM_STORAGE_WORDS) R_MEM(storage)[pa] = 0177760u;
            }
            cold_ac_done = true;
        }

        // Seed Alto keyboard words into VM 0177034..0177037 (matching machine.c)
        if ((cyc & 0xFF) == 0 && !R_CPU(ifu_active)) {
            if (eth.eftp_max_seq > 0) {
                R_IO(ddc_kbd_words)[0] = 0xFFFF; // Release BS once EFTP starts
            } else {
                R_IO(ddc_kbd_words)[0] = 0xFFFE; // Hold BS during ABoot
            }
            for (int k = 0; k < 4; k++) {
                uint32_t va = 0177034u + (uint32_t)k;
                uint32_t vp = (va >> 8) & 0xFFFFu;
                uint32_t rp = R_MEM(map_rp)[vp];
                uint32_t pa = (rp << 8) | (va & 0xFFu);
                if (pa < COSIM_STORAGE_WORDS) {
                    R_MEM(storage)[pa] = R_IO(ddc_kbd_words)[k];
                    // Also update cache if resident!
                    uint32_t row = (va >> 4) & 0x3Fu;
                    uint32_t tag = va >> 10;
                    uint32_t off = va & 0xFu;
                    for (int w = 0; w < 4; w++) {
                        if (R_MEM(cache_valid)[row][w] && R_MEM(cache_tag)[row][w] == tag) {
                            R_MEM(cache_data)[CACHE_IDX(row, w, off)] = R_IO(ddc_kbd_words)[k];
                        }
                    }
                }
            }
            // Enable display scanline/DWT wakeups once DASTART (VM 0420) is non-zero
            uint32_t vp0 = 0;
            uint32_t rp0 = R_MEM(map_rp)[vp0];
            uint32_t pa_dastart = (rp0 << 8) | (0420u & 0xFFu);
            uint16_t dastart = (pa_dastart < COSIM_STORAGE_WORDS) ? R_MEM(storage)[pa_dastart] : 0;
            uint32_t row_da = (0420u >> 4) & 0x3Fu;
            for (int w = 0; w < 4; w++) {
                if (R_MEM(cache_valid)[row_da][w] && R_MEM(cache_tag)[row_da][w] == 0) {
                    dastart = R_MEM(cache_data)[CACHE_IDX(row_da, w, 0420u & 0xFu)];
                }
            }
            top->display_wake_gate = (dastart != 0 || R_IO(ddc_next_wcb_flag)[0] || R_IO(ddc_next_wcb_flag)[1]) ? 1 : 0;
        }

        // Step RTL by 1 clock cycle
        uint64_t pre_disp = R_CPU(ifu_dispatch_count);
        uint32_t pre_br31 = R_MEM(br)[037];  // BR[Code, 37] -- the IFU code base
        uint16_t pre_ac[4] = { R_CPU(STK)[1], R_CPU(STK)[2], R_CPU(STK)[3], R_CPU(STK)[4] };
        rtl_step(top);

        // RTL_IFUDISP_TRACE=1: one line per IFU dispatch, comparable with the
        // C emulator's DORADO_IFUDISP_TRACE (pcx, opcode, Alto AC0..3 = STK[1..4]).
        // RTL_MEMDUMP=<octal opva>: dump VA 0..0777 at the first dispatch of that
        // Alto PC (memory as the CPU sees it, i.e. through the cache).
        static const long memdump_pc = getenv("RTL_MEMDUMP") ? strtol(getenv("RTL_MEMDUMP"), nullptr, 8) : -1;
        static bool memdump_done = false;
        if (memdump_pc >= 0 && !memdump_done && R_CPU(ifu_dispatch_count) != pre_disp &&
            ((pre_br31 + (uint16_t)(R_CPU(ifu_pcx) >> 1)) & 0x0FFFFFFFu) == (uint32_t)memdump_pc) {
            memdump_done = true;
            for (uint32_t va = 0; va < 01000; va++)
                fprintf(stderr, "RTLMEM %04o %06o\n", va, read_rtl_va(va));
        }
        // RTL_UTRACE=<octal opva>: after the first dispatch of that Alto PC, print
        // 40 microinstructions (task, real PC, T, last ref VA, Md, AC0..3).
        static const long utrace_pc = getenv("RTL_UTRACE") ? strtol(getenv("RTL_UTRACE"), nullptr, 8) : -1;
        static int utrace_left = -1;
        if (utrace_pc >= 0 && utrace_left < 0 && R_CPU(ifu_dispatch_count) != pre_disp &&
            ((pre_br31 + (uint16_t)(R_CPU(ifu_pcx) >> 1)) & 0x0FFFFFFFu) == (uint32_t)utrace_pc)
            utrace_left = 40;
        if (utrace_left > 0) {
            utrace_left--;
            fprintf(stderr, "UTRACE cyc=%d task=%o pc_before=0%04o pc_now=0%04o T=%06o va=%07o md=%06o ac=%06o,%06o,%06o,%06o\n",
                    cyc, ctask, pre_pc, R_CPU(real_PC), R_CPU(T), R_MEM(last_ref_va), R_MEM(md),
                    R_CPU(STK)[1], R_CPU(STK)[2], R_CPU(STK)[3], R_CPU(STK)[4]);
        }
        if (trace_disp && R_CPU(ifu_dispatch_count) != pre_disp) {
            uint32_t opva = (pre_br31 + (uint16_t)(R_CPU(ifu_pcx) >> 1)) & 0x0FFFFFFFu;
            fprintf(stderr, "RTLDISP n=%llu cyc=%d opva=%07o br31=%07o op=%03o aacs=%06o,%06o,%06o,%06o\n",
                    (unsigned long long)R_CPU(ifu_dispatch_count), cyc,
                    opva, R_MEM(br)[037], R_CPU(ifu_opcode),
                    pre_ac[0], pre_ac[1], pre_ac[2], pre_ac[3]);
        }

        // Check if RTL Ethernet Controller transmitted a packet
        if (top->eth_tx_packet_stb) {
            int nwords = top->eth_tx_packet_words;
            uint16_t pkt[1024];
            if (nwords > 1024) nwords = 1024;
            for (int i = 0; i < nwords; i++) {
                pkt[i] = R_IO(eth_tx_words)[i];
            }
            eth.rx_on = R_IO(eth_rx_on);
            dorado_ethernet_direct_transmit(&eth, pkt, (size_t)nwords);
            sync_eth_rx_to_rtl();
        }

        // Breath-of-Life / Time broadcast & EFTP retransmission
        if ((uint32_t)cyc >= next_bol_cycle) {
            eth.rx_on = R_IO(eth_rx_on);
            if (R_IO(eth_rx_pos) >= R_IO(eth_rx_count)) {
                eth.rx_pos = eth.rx_count;
                int sent = (eth.eftp_max_seq > 0)
                         ? dorado_ethernet_time_broadcast(&eth)
                         : dorado_ethernet_breath_of_life(&eth);
                if (sent) sync_eth_rx_to_rtl();
                next_bol_cycle = (uint32_t)cyc + (sent ? 500000u : 50000u);
            } else {
                next_bol_cycle = (uint32_t)cyc + 50000u;
            }
        }
        if ((cyc & 0x3FF) == 0 && R_IO(eth_rx_pos) >= R_IO(eth_rx_count)) {
            eth.rx_on = R_IO(eth_rx_on);
            eth.rx_pos = eth.rx_count;
            (void)dorado_ethernet_wakeup_mask(&eth);
            sync_eth_rx_to_rtl();
        }
    }

    auto t1 = std::chrono::steady_clock::now();
    double elapsed_s = std::chrono::duration<double>(t1 - t0).count();
    double mhz = (max_cycles / 1e6) / elapsed_s;

    uint64_t tx_pkts    = R_IO(eth_tx_packets);
    uint64_t rx_pkts    = R_IO(eth_rx_packets);
    uint64_t ddc_lines  = R_IO(ddc_lines_rendered);
    uint64_t ddc_pixels = R_IO(ddc_pixels_rendered);
    uint64_t ifu_disp   = R_CPU(ifu_dispatch_count);

    // Count non-zero pixels in RTL framebuffer u_io.fb
    int fb_pixels = 0;
    for (int i = 0; i < (1024 * 808) / 8; i++) {
        uint8_t b = R_IO(fb)[i];
        while (b) {
            fb_pixels += (b & 1);
            b >>= 1;
        }
    }

    // RTL_FB_OUT=path.pbm: write the 1024x808 framebuffer (MSB = leftmost pixel).
    if (const char *fbp = getenv("RTL_FB_OUT")) {
        if (FILE *pf = fopen(fbp, "wb")) {
            fprintf(pf, "P4\n1024 808\n");
            for (int i = 0; i < (1024 * 808) / 8; i++) fputc(R_IO(fb)[i], pf);
            fclose(pf);
        }
    }

    printf("  [BOOT] %s + %s (%d cycles in %.2fs = %.2f MHz):\n",
           eb_path, boot_file_path, max_cycles, elapsed_s, mhz);
    printf("         cold_ac=%d eftp_seq=%u eth_tx=%llu eth_rx=%llu ifu_disp=%llu ddc_lines=%llu ddc_px=%llu fb_px=%d\n",
           cold_ac_done ? 1 : 0, eth.eftp_max_seq,
           (unsigned long long)tx_pkts, (unsigned long long)rx_pkts,
           (unsigned long long)ifu_disp, (unsigned long long)ddc_lines,
           (unsigned long long)ddc_pixels, fb_pixels);
    printf("         end_pc=0%04o ifu_pcf=0%06o ctask=%o halted=%u halt_reason=%u ready=0%06o eth_rx_on=%u eth_tx_on=%u bol_q=%llu\n",
           R_CPU(real_PC), R_CPU(ifu_pcf), R_CPU(ctask), R_CPU(halted), R_CPU(halt_reason),
           R_CPU(ready), R_IO(eth_rx_on), R_IO(eth_tx_on),
           (unsigned long long)eth.bol_queued);
    printf("         eth_rx_pos=%d eth_rx_count=%d eth_rx_hold=%u eth_world_rx_words=%u eth_no_wake=%u eit_tpc=0%04o eot_tpc=0%04o\n",
           R_IO(eth_rx_pos), R_IO(eth_rx_count), R_IO(eth_rx_hold),
           R_IO(eth_world_rx_words), R_IO(eth_no_wakeups),
           R_CPU(task_tpc)[7], R_CPU(task_tpc)[6]);
    printf("         BR31=0%07o BR32=0%07o BR33=0%07o BR36=0%07o\n",
           R_MEM(br)[031], R_MEM(br)[032], R_MEM(br)[033], R_MEM(br)[036]);
    printf("         ECBR(600..610): %06o %06o %06o %06o %06o %06o %06o %06o %06o | M[1..4]: %06o %06o %06o %06o\n",
           read_rtl_va(0600), read_rtl_va(0601), read_rtl_va(0602), read_rtl_va(0603),
           read_rtl_va(0604), read_rtl_va(0605), read_rtl_va(0606), read_rtl_va(0607),
           read_rtl_va(0610), read_rtl_va(1), read_rtl_va(2), read_rtl_va(3), read_rtl_va(4));
    printf("         eit_cycles=%llu eit_pcs:", (unsigned long long)eit_cycles);
    for (int i = 0; i < eit_pc_n; i++) printf(" 0%04o", eit_pcs[i]);
    printf("\n");

    dorado_ethernet_free(&eth);
    dorado_memory_free(mem); free(mem); free(cpu); free(mc);

    // Pass = the whole boot file arrived over EFTP AND the loaded program painted
    // a screen. 1000 lit pixels is well under Galaxian's title + formation and
    // well over any stray scanline.
    if (!cold_ac_done || eth.eftp_max_seq < eftp_expected_seq || ddc_lines == 0 || fb_pixels < 1000) {
        fprintf(stderr, "  [FAIL] Suite 6: RTL I/O boot: eftp_seq=%u (want >= %u), ddc_lines=%llu, fb_px=%d (want >= 1000)\n",
                eth.eftp_max_seq, eftp_expected_seq, (unsigned long long)ddc_lines, fb_pixels);
        return false;
    }
    printf("  [PASS] Suite 6: Full RTL I/O Bridge Boot (EBoot + EFTP + Display DDC Rendering)\n");
    return true;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Vdorado_rtl_top *top = new Vdorado_rtl_top;

    printf("====================================================================\n");
    printf(" Dorado Clean 1-Cycle RTL — Verilator Lock-Step Cosimulation Suite\n");
    printf("====================================================================\n");

    // RTL_BOOT_ONLY=N: skip suites 1-5 and run only the Suite 6 boot, for N cycles.
    if (const char *b = getenv("RTL_BOOT_ONLY")) {
        int n = atoi(b);
        return test_rtl_io_boot(top, "../dorado/worlds/aemu.eb", "../chm/bootfiles/Galaxian.boot!1",
                                n > 0 ? n : 15000000) ? 0 : 1;
    }

    if (!test_alu_shifter_rm_stk(top)) return 1;
    if (!test_memory_subsystem(top))   return 1;
    if (!test_ifu_and_tasks(top))      return 1;

    if (!test_real_microcode_mb(top, "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb", 0xFFFF, 2000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb",   0xFFFF, 5000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/expanded/kernel.dm!38_/kernel.mb",       0xFFFF, 5000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb",     0xFFFF, 5000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb",      0xFFFF, 5000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/AEmu.mb!2",                              01076,  10000)) return 1;
    if (!test_real_microcode_mb(top, "../chm/dorado/DSemu.mb!1",                             01076,  10000)) return 1;
    if (!test_real_microcode_eb(top, "../chm/dorado/CedarDorado.eb!6",                       10000)) return 1;

    benchmark_rtl_speed(top, "../chm/dorado/AEmu.mb!2", 01076, 1000000);

    if (!test_rtl_io_boot(top, "../dorado/worlds/aemu.eb", "../chm/bootfiles/Galaxian.boot!1", 80000000)) return 1;

    printf("====================================================================\n");
    printf(" ALL RTL LOCK-STEP COSIMULATION TESTS PASSED!\n");
    printf("====================================================================\n");

    delete top;
    return 0;
}
