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
        static const bool fsm_trace = getenv("RTL_FSM_TRACE") != nullptr;
        if (fsm_trace) fprintf(stderr, "  clk st=%d pc=0%o step_done=%d x_np=0%o pred=0%o fresh=%d pre_ok=%d spec=%d\n",
                               (int)R_CPU(st), (unsigned)R_CPU(real_PC), (int)top->step_done,
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
        if (top->cmp_done || top->dbg_halted) return;   // the instruction and what its dispatch owes
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
    top->display_wake_gate = 0;
    top->kbd_words = ~0ULL;
    top->mouse_buttons = 0; top->mouse_present = 0; top->mouse_x = 0; top->mouse_y = 0;
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
            R_CPU(br)[i] = c_mem->br[i] & 0x0FFFFFFFu;
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
                    R_CW(w)[CW_IDX(r, k)] = c_mem->cache[r].ways[w].data[k];
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
            R_CPU(pipe_mapbuf_busy)[s] = c_mem->pipe[s].mapbuf_busy ? 1 : 0;
            R_MEM(pipe_cache_flags)[s] = c_mem->pipe[s].cache_flags;
            R_MEM(pipe_pipe4_errors)[s] = c_mem->pipe[s].pipe4_errors;
            R_MEM(pipe_pipe4_syndrome)[s] = c_mem->pipe[s].pipe4_syndrome;
            R_MEM(pipe_pipe4_quadword)[s] = c_mem->pipe[s].pipe4_quadword & 3;
        }
        R_MEM(pipe_head) = c_mem->pipe_head;
        R_CPU(proc_srn) = c_mem->proc_srn & 0xF;
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
        R_MEM(md_q) = c_mem->md;
        R_MEM(dbuf) = c_mem->dbuf;
        R_CPU(mcr) = c_mem->mcr;
        R_CPU(dmux_addr) = c_mem->dmux_addr & 0xFFF;
        R_CPU(dmux_pending) = c_mem->dmux_pending ? 1 : 0;
        R_CPU(mapbuf_busy_slot) = c_mem->mapbuf_busy_slot;
        R_CPU(mapbuf_busy_cycles) = c_mem->mapbuf_busy_cycles;
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
            R_STOR(storage)[i] = c_mem->storage[i];
        }
    }
    top->eval();
}

// -----------------------------------------------------------------------------
// Step both C reference and Verilated RTL by 1 cycle and verify lock-step match
// -----------------------------------------------------------------------------
static bool compare_c_rtl(const dorado_cpu *c_cpu, Vdorado_rtl_top *top,
                          uint64_t step_idx, const char *test_name,
                          uint16_t pc_before, uint8_t task_before) {
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
        if (c_mem->md  != R_MEM(md_q))  fail("mem.md",  c_mem->md,  R_MEM(md_q));
        if (c_mem->mar != R_MEM(mar)) fail("mem.mar", c_mem->mar, R_MEM(mar));
        if (c_mem->mcr != R_CPU(mcr)) fail("mem.mcr", c_mem->mcr, R_CPU(mcr));
        if (c_mem->proc_srn != R_CPU(proc_srn)) fail("mem.proc_srn", c_mem->proc_srn, R_CPU(proc_srn));
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
        for (int i = 0; i < DM_BR_COUNT; i++) {
            if (c_mem->br[i] != R_CPU(br)[i]) {
                char name[32];
                snprintf(name, sizeof(name), "mem.br[%d]", i);
                fail(name, c_mem->br[i], R_CPU(br)[i]);
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

    // 2. Step RTL by one microinstruction
    rtl_step(top);

    return compare_c_rtl(c_cpu, top, step_idx, test_name, pc_before, task_before);
}
