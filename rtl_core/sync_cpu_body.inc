// sync_cpu_body.inc -- the C machine's processor and memory-section state into the
// RTL's registers, by name. Included where c_cpu, c_mem, mc are in scope and
// R_CPU(n) / R_MEM(n) name an assignable register (the Verilated model in
// cosim_sync.h; a name table in hps/hps_host.cpp, which makes a snapshot's
// scan chain and load records from it). R_CW, if defined, takes the cache words.
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
#ifdef R_CW
                for (int k = 0; k < DM_CACHE_LINE_W; k++) {
                    R_CW(w)[CW_IDX(r, k)] = c_mem->cache[r].ways[w].data[k];
                }
#endif
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
    }
