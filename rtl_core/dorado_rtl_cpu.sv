// =============================================================================
// dorado_rtl_cpu.sv — Xerox Dorado 1-Clock-Per-Microinstruction CPU & IFU Core
// =============================================================================
// Derived from the schematic-audited C reference (dorado/src/cpu.c).
// Executes one complete 60 ns Dorado microinstruction per clock edge when
// clk_en=1 and !hold. Instantiates dorado_rtl_alu, dorado_rtl_shifter, and
// dorado_rtl_mem.
// =============================================================================

/* verilator lint_off BLKSEQ */
module dorado_rtl_cpu
    import dorado_rtl_pkg::*;
#(
    parameter int STORAGE_WORDS = 4 * 1024 * 1024
) (
    input  logic           clk,
    input  logic           rst,
    input  logic           clk_en,

    // BaseBoard / MIR single-step injection interface
    input  logic           bb_present,
    input  logic           bb_running,
    input  logic           bb_ss_inject,
    input  dorado_uinstr_t bb_mir_uinstr,

    // Slow I/O bus interface (optional external device attachment)
    input  logic           io_attached,
    input  logic [15:0]    io_read_data,
    input  logic           io_read_bad_parity,
    input  logic           io_has_write_in,
    input  logic           io_atten_in,
    output logic [3:0]     io_read_task,
    output logic [1:0]     io_read_subtask,
    output logic [7:0]     io_read_tioa,
    output logic           io_read_stb,
    output logic [3:0]     io_read_stb_task,
    output logic [1:0]     io_read_stb_subtask,
    output logic [7:0]     io_read_stb_tioa,
    output logic           io_write_stb,
    output logic [3:0]     io_write_task,
    output logic [1:0]     io_write_subtask,
    output logic [7:0]     io_write_tioa,
    output logic [15:0]    io_write_data,

    // Fast I/O munch interface (IOFetch<- / IOStore<-)
    output logic           fastio_stb,
    output dm_ref_kind_t   fastio_kind,
    output logic [3:0]     fastio_task,
    output logic [1:0]     fastio_subtask,
    output logic [27:0]    fastio_va,
    output logic [15:0]    fastio_out_munch [0:15],
    input  logic [15:0]    fastio_in_munch  [0:15],

    // Disk DMux status interface
    input  logic           dmux_ext_handled,
    input  logic [15:0]    dmux_ext_val,
    output logic [11:0]    dmux_cur_addr,

    // External device task wakeups and subtask control
    input  logic [15:0]    ext_wakeup_req,
    input  logic           ext_subtask_we,
    input  logic [3:0]     ext_subtask_task,
    input  logic [1:0]     ext_subtask_val,

    // Observability outputs
    output logic [11:0]    dbg_real_pc,
    output logic [3:0]     dbg_ctask,
    output logic [15:0]    dbg_t,
    output logic [15:0]    dbg_q,
    output logic [15:0]    dbg_link,
    output logic [15:0]    dbg_cnt,
    output logic [7:0]     dbg_stkp,
    output logic [63:0]    dbg_cycles,
    output logic           dbg_halted,
    output logic [3:0]     dbg_halt_reason
);
    // -------------------------------------------------------------------------
    // Constants
    // -------------------------------------------------------------------------
    localparam int DORADO_JUNK_TASK        = 2;
    localparam int DORADO_JUNK_TICK_CYCLES = 533;
    localparam int DORADO_TASKSIM_TASK     = 8'o12;
    localparam int CPU_PAGE_SIZE           = 64;
    localparam int CPU_QUADRANT_SIZE       = 4096;

    localparam logic [7:0] EVC_EV_HOLD        = 8'h01;
    localparam logic [7:0] EVC_EV_PROCREF     = 8'h02;
    localparam logic [7:0] EVC_EV_IFUREF      = 8'h04;
    localparam logic [7:0] EVC_EV_IFUJUMP     = 8'h08;
    localparam logic [7:0] EVC_EV_MISS        = 8'h10;
    localparam logic [7:0] EVC_EV_IFUNOTREADY = 8'h20;

    localparam logic [15:0] IFUTEST_EN = 16'h0002;

    // -------------------------------------------------------------------------
    // Control Store (IM), IFUM, RM, STK, ALUFM
    // -------------------------------------------------------------------------
    logic [15:0] im_iw0        [0:IM_SIZE-1];
    logic [15:0] im_iw1        [0:IM_SIZE-1];
    logic [15:0] im_iw2        [0:IM_SIZE-1];
    logic        im_present    [0:IM_SIZE-1];
    logic        im_breakpoint [0:IM_SIZE-1];

    logic [15:0] ifum_lo       [0:IFUM_SIZE-1];
    logic [15:0] ifum_hi       [0:IFUM_SIZE-1];
    logic        ifum_present  [0:IFUM_SIZE-1];

    logic [15:0] RM            [0:RM_SIZE-1];
    logic [15:0] STK           [0:STK_SIZE-1];

    logic [5:0]  alufm         [0:ALUFM_SIZE-1];
    logic        alufm_present [0:ALUFM_SIZE-1];

    // -------------------------------------------------------------------------
    // Architectural CPU State
    // -------------------------------------------------------------------------
    logic [15:0] real_PC;
    logic [15:0] prev_PC;
    logic [15:0] T;
    logic [15:0] Q;
    logic [15:0] Link;
    logic [15:0] Cnt;
    logic [15:0] ShC;
    logic [7:0]  RBase;
    logic [7:0]  MemBase;
    logic [1:0]  MemBX;
    logic [7:0]  TIOA;
    logic [7:0]  StkP;
    logic        stk_ovf;
    logic        stk_und;
    logic [15:0] cpreg;
    logic        io_bad_parity;
    logic        io_atten_at_issue;
    logic [15:0] b_link_read;
    logic        b_link_read_valid;
    logic        b_link_read_age;
    logic [15:0] link_at_issue;

    // ALU Branch Condition Flags
    logic        alu_zero;
    logic        alu_lt0;
    logic        alu_carry;
    logic        alu_overflow;

    // 16-Way Hardware Tasking State
    logic [3:0]  ctask;
    logic [15:0] ready;
    logic [15:0] wakeup_pending;
    logic [15:0] wakeup_pipe [0:1];
    logic        tasking_on;
    logic [7:0]  tasking_resume_delay;

    logic [15:0] task_t            [0:NUM_TASKS-1];
    logic [15:0] task_tpc          [0:NUM_TASKS-1];
    logic [15:0] task_link         [0:NUM_TASKS-1];
    logic [7:0]  task_membase      [0:NUM_TASKS-1];
    logic [7:0]  task_rbase        [0:NUM_TASKS-1];
    logic [7:0]  task_tioa         [0:NUM_TASKS-1];
    logic        task_alu_zero     [0:NUM_TASKS-1];
    logic        task_alu_lt0      [0:NUM_TASKS-1];
    logic        task_alu_carry    [0:NUM_TASKS-1];
    logic        task_alu_overflow [0:NUM_TASKS-1];
    logic [1:0]  task_subtask      [0:NUM_TASKS-1];
    logic [15:0] task_dispatch     [0:NUM_TASKS-1];
    logic [15:0] dispatch_or;

    // Per-Task Md Latches
    logic [15:0] task_md       [0:NUM_TASKS-1];
    logic        task_md_valid [0:NUM_TASKS-1];
    logic [63:0] task_md_ready [0:NUM_TASKS-1];

    // IFU State
    logic [15:0] ifu_pcf;
    logic [15:0] ifu_pcx;
    logic [7:0]  ifu_opcode;
    logic [7:0]  ifu_alpha;
    logic [7:0]  ifu_beta;
    logic [1:0]  ifu_insset;
    logic [7:0]  ifu_idcnt;
    logic [7:0]  ifu_length;
    logic        ifu_sign;
    logic        ifu_packed_a;
    logic        ifu_type_pause;
    logic        ifu_type_jump;
    logic [3:0]  ifu_n;
    logic        ifu_active;
    logic [7:0]  ifu_warmup;
    logic [15:0] ifu_test;
    logic        ifu_test_pending;
    logic [7:0]  ifu_test_queue [0:7];
    logic [3:0]  ifu_test_count;
    logic [7:0]  ifu_saved_stkp;
    logic        ifu_save_stkp_pending;
    int          ifu_dispatch_rbase;
    logic [63:0] ifu_dispatch_count;

    logic [7:0]  reschedule_pending;
    logic        reschedule_cond;
    logic [7:0]  brk_opcode;
    logic        brk_pending;

    // Event Counters & Hardware Checkout Simulators (TASKSIM / HOLDSIM)
    logic [15:0] event_cnt_a;
    logic [15:0] event_cnt_b;
    logic [3:0]  event_cnt_ctrl_hi;
    logic [7:0]  event_cnt_ctrl_lo;
    logic        gen_io_mode;
    logic        evc_a_enable;
    logic [2:0]  evc_a_event;
    logic        evc_a_tasksall;
    logic        evc_b_enable;
    logic [2:0]  evc_b_event;
    logic        evc_b_tasksall;
    logic [7:0]  evc_events;

    logic [6:0]  tasksim;
    logic [7:0]  holdsim;
    logic        tasksim_fired;
    logic        tasksim_loaded;
    logic        holdsim_loaded;
    logic        holdsim_delay;
    logic        holdsim_hold;

    // Junk Task Timer
    logic        junk_tw_enabled;
    logic [15:0] junk_tw_countdown;

    // Execution Counters & Halt Status
    logic [63:0] cycles;
    logic        halted;
    logic [3:0]  halt_reason;

    assign dbg_real_pc     = real_PC[11:0];
    assign dbg_ctask       = ctask;
    assign dbg_t           = T;
    assign dbg_q           = Q;
    assign dbg_link        = Link;
    assign dbg_cnt         = Cnt;
    assign dbg_stkp        = StkP;
    assign dbg_cycles      = cycles;
    assign dbg_halted      = halted;
    assign dbg_halt_reason = halt_reason;

    assign io_read_task    = ctask;
    assign io_read_subtask = task_subtask[ctask];
    assign io_read_tioa    = TIOA;

    // -------------------------------------------------------------------------
    // Memory Subsystem Instance
    // -------------------------------------------------------------------------
    dorado_rtl_mem #(
        .STORAGE_WORDS (STORAGE_WORDS)
    ) u_mem (
        .clk              (clk),
        .rst              (rst),
        .fastio_stb       (fastio_stb),
        .fastio_kind      (fastio_kind),
        .fastio_task      (fastio_task),
        .fastio_subtask   (fastio_subtask),
        .fastio_va        (fastio_va),
        .fastio_out_munch (fastio_out_munch),
        .fastio_in_munch  (fastio_in_munch),
        .dmux_ext_handled (dmux_ext_handled),
        .dmux_ext_val     (dmux_ext_val),
        .dmux_cur_addr    (dmux_cur_addr)
    );

    // -------------------------------------------------------------------------
    // Combinational Instruction Fetch & Shifter / ALU Instantiations
    // -------------------------------------------------------------------------
    dorado_uinstr_t cur_u;
    logic [15:0]    cur_task_md;
    logic [15:0]    cur_rm_stk_read;
    logic [15:0]    cur_id_peek;
    logic [15:0]    shifter_unmasked_lo16;
    logic [15:0]    shifter_out_mask;
    logic [15:0]    shifter_out_fill;

    logic [5:0]     comb_alufm_entry;
    logic [15:0]    comb_alu_a;
    logic [15:0]    comb_alu_b;
    logic           comb_carry20;
    logic [15:0]    alu_result;
    logic           alu_carry_out;
    logic           alu_overflow_out;
    logic           alu_is_arith_out;

    // IFU trap address (HM Table 14, p.33): the trap vectors for instruction
    // set i live at {~i[1:0], base[5:2], n[1:0]}, i.e. ~InsSet lands in TNIA
    // bits 7:6 -- for the Alto set 0 that is 0300-0377 (AEmuReschedule = 0314).
    // Mirrors cpu.c ifu_trap_addr().
    function automatic logic [15:0] ifu_trap_addr_fn(input logic [5:0] base, input logic [1:0] n);
        return {8'd0, ~ifu_insset, base[5:2], n};
    endfunction

    // Pure helper for current task's Md
    function automatic logic [15:0] get_task_md();
        if (task_md_valid[ctask]) return task_md[ctask];
        return u_mem.md;
    endfunction

    // Pure helper for IFU operand byte at index cnt
    function automatic logic [15:0] ifu_id_at_fn(input logic [7:0] cnt);
        logic [7:0] len;
        logic       n_supplied, n_first;
        int         op_idx;
        len        = ifu_length;
        n_supplied = (ifu_n != 4'o17);
        n_first    = n_supplied;

        if (ifu_type_jump) return {8'd0, len};
        if (len == 8'd1 && ifu_packed_a) n_first = 1'b0;

        op_idx = int'(cnt);
        if (n_first) begin
            if (op_idx == 0) return {12'd0, ifu_n};
            op_idx--;
        end
        if (len == 8'd3 && ifu_packed_a) begin
            if (op_idx == 0) return {12'd0, ifu_alpha[7:4]};
            if (op_idx == 1) return {12'd0, ifu_alpha[3:0]};
            if (op_idx == 2) return {8'd0, ifu_beta};
            op_idx -= 3;
        end else if (len >= 8'd2 && ifu_packed_a) begin
            if (op_idx == 0) return {12'd0, ifu_alpha[7:4]};
            if (op_idx == 1) return {12'd0, ifu_alpha[3:0]};
            op_idx -= 2;
        end else if (len >= 8'd2) begin
            if (op_idx == 0) begin
                logic [15:0] a_ext;
                a_ext = {8'd0, ifu_alpha};
                if (ifu_sign && ifu_alpha[7]) a_ext |= 16'hFF00;
                return a_ext;
            end
            op_idx--;
        end
        if (len == 8'd3 && !ifu_packed_a) begin
            if (op_idx == 0) return {8'd0, ifu_beta};
            op_idx--;
        end
        return {8'd0, ifu_length};
    endfunction

    // Pure helper for RM/STK read address (0..255 = RM, 256..511 = STK)
    function automatic int rm_address_fn(
        input dorado_uinstr_t u,
        input logic [3:0]     cur_task,
        input logic [7:0]     cur_rbase,
        input logic [1:0]     cur_subtask,
        input logic [7:0]     cur_stkp
    );
        logic [3:0] rbase_eff;
        if (u.block && cur_task == 4'd0) begin
            return 256 + int'(cur_stkp);
        end
        rbase_eff = cur_rbase[3:0];
        if (cur_task != 4'd0) begin
            rbase_eff |= {2'b00, cur_subtask};
        end
        return int'({rbase_eff, u.rstk});
    endfunction

    function automatic logic [15:0] rm_stk_read_fn(input int idx);
        if (idx >= 512 || idx < 0) return 16'd0;
        if (idx >= 256) return STK[idx & 255];
        return RM[idx & 255];
    endfunction

    always_comb begin
        int ridx;
        if (bb_present && bb_ss_inject) begin
            cur_u = bb_mir_uinstr;
        end else if (real_PC < IM_SIZE) begin
            cur_u = unpack_uinstr(im_iw0[real_PC[11:0]],
                                  im_iw1[real_PC[11:0]],
                                  im_iw2[real_PC[11:0]]);
        end else begin
            cur_u = '0;
        end

        cur_task_md     = get_task_md();
        ridx            = rm_address_fn(cur_u, ctask, RBase, task_subtask[ctask], StkP);
        cur_rm_stk_read = rm_stk_read_fn(ridx);
        cur_id_peek     = ifu_id_at_fn(ifu_idcnt);
    end

    dorado_rtl_shifter u_shifter (
        .u             (cur_u),
        .t_in          (T),
        .r_in          (cur_rm_stk_read),
        .id_peek       (cur_id_peek),
        .shc           (ShC),
        .task_md_in    (cur_task_md),
        .unmasked_lo16 (shifter_unmasked_lo16),
        .out_mask      (shifter_out_mask),
        .out_fill      (shifter_out_fill)
    );

    dorado_rtl_alu u_alu (
        .alufm_entry (comb_alufm_entry),
        .a           (comb_alu_a),
        .b           (comb_alu_b),
        .carry20     (comb_carry20),
        .result      (alu_result),
        .carry_out   (alu_carry_out),
        .overflow    (alu_overflow_out),
        .is_arith    (alu_is_arith_out)
    );

    // -------------------------------------------------------------------------
    // Helper Functions matching cpu.c
    // -------------------------------------------------------------------------
    function automatic logic [15:0] field_desc_to_shc(
        input logic [15:0] a_val,
        input logic        is_write
    );
        logic [3:0] P, S, count, rmask, lmask;
        logic [1:0] shc23;
        P     = a_val[7:4];
        S     = a_val[3:0];
        shc23 = a_val[13:12];
        if (is_write) begin
            count = 4'(4'd15 - P - S);
            rmask = 4'(4'd15 - P - S);
            lmask = P;
        end else begin
            count = 4'(P + S + 4'd1);
            rmask = 4'd0;
            lmask = 4'(4'd15 - S);
        end
        return {2'b00, shc23, count, rmask, lmask};
    endfunction

    function automatic logic ff_loads_link_fn(input dorado_uinstr_t u);
        if (!ff_full_function_ok(u)) return 1'b0;
        if (ff_fa(u.ff) != 2'd1) return 1'b0;
        return (ff_fb(u.ff) == 3'd4 && ff_fc(u.ff) == 3'd7) ||
               (ff_fb(u.ff) == 3'd7 && ff_fc(u.ff) == 3'd6);
    endfunction

    function automatic logic jcn_is_write_im_fn(input dorado_uinstr_t u);
        return (u.jcn[2:0] == 3'd7) && (u.jcn[6] == 1'b1) && (u.jcn[5:3] == 3'd7);
    endfunction

    function automatic dm_ref_kind_t decode_ref_kind_fn(
        input dorado_uinstr_t u,
        input logic           io_task
    );
        logic       ff_ok;
        logic [1:0] ff01;
        ff_ok = ff_decode_ok(u);
        ff01  = ff_ok ? u.ff[7:6] : 2'd0;
        case (u.asel)
            3'd0: begin
                if (!ff_ok) return DM_REF_STORE;
                case (ff01)
                    2'd0: return DM_REF_PREFETCH;
                    2'd1: begin
                        if (io_task) return DM_REF_IOFETCH;
                        if (u.ff[5:3] == 3'd3 && u.ff[2:0] == 3'd1) return DM_REF_RMAP;
                        return DM_REF_MAP;
                    end
                    2'd2: return DM_REF_LONGFETCH;
                    2'd3: return DM_REF_STORE;
                endcase
            end
            3'd1: begin
                if (!ff_ok) return DM_REF_FETCH;
                case (ff01)
                    2'd0: return DM_REF_DUMMYREF;
                    2'd1: return io_task ? DM_REF_IOSTORE : DM_REF_FLUSH;
                    2'd2: return DM_REF_IFETCH;
                    2'd3: return DM_REF_FETCH;
                endcase
            end
            3'd2: return DM_REF_STORE;
            3'd3: return DM_REF_FETCH;
            default: return DM_REF_NONE;
        endcase
    endfunction

    function automatic int lc_write_address_fn(
        input dorado_uinstr_t u,
        input logic [3:0]     cur_task,
        input logic [7:0]     cur_rbase,
        input logic [1:0]     cur_subtask,
        input logic [7:0]     cur_stkp
    );
        if (ff_is_table11a(u, 8'o040, 8'o060)) begin
            return int'({cur_rbase[3:0], u.ff[3:0]});
        end
        if (ff_full_function_ok(u) && ff_fa(u.ff) == 2'd2 &&
            (ff_fb(u.ff) == 3'd2 || ff_fb(u.ff) == 3'd3)) begin
            return int'({u.ff[3:0], u.rstk});
        end
        if (u.block && cur_task == 4'd0) begin
            logic [7:0] idx;
            idx = cur_stkp;
            if (ff_is_mod_stkp_before_w(u)) begin
                idx = 8'(cur_stkp + stk_signed_delta(u.rstk));
            end
            return 256 + int'(idx);
        end
        return rm_address_fn(u, cur_task, cur_rbase, cur_subtask, cur_stkp);
    endfunction

    function automatic logic uinstr_reads_md_fn(input dorado_uinstr_t u);
        if (ff_decode_ok(u) && u.asel != 3'd7) begin
            logic [5:0] f6;
            if (u.asel == 3'd0 || u.asel == 3'd1) begin
                f6 = u.ff[5:0];
            end else if (u.asel == 3'd2 || u.asel == 3'd3) begin
                if (u.ff[7:6] == 2'd0) return 1'b1;
                f6 = 6'd0;
            end else if (ff_full_function_ok(u) && ff_fa(u.ff) == 2'd0) begin
                f6 = u.ff[5:0];
            end else begin
                f6 = 6'd0;
            end
            if (f6[5:3] == 3'd2 && f6[2:0] == 3'd2) return 1'b1;
        end
        if (u.bsel == 3'd0) return 1'b1;
        if (u.lc >= 3'd2 && u.lc <= 3'd5) return 1'b1;
        return 1'b0;
    endfunction

    function automatic logic [3:0] task_bnt_fn(input logic [15:0] avail_in);
        logic [15:0] avail;
        avail = avail_in | 16'h0001;
        for (int i = 15; i > 0; i--) begin
            if (avail[i]) return 4'(i);
        end
        return 4'd0;
    endfunction

    function automatic logic event_counter_hit_fn(
        input logic [2:0] event_sel,
        input logic [7:0] flags,
        input logic       is_b
    );
        case (event_sel)
            3'd0: return !(flags & EVC_EV_HOLD);
            3'd1: return (flags & EVC_EV_HOLD) != 8'd0;
            3'd2: return (flags & (is_b ? EVC_EV_IFUREF : EVC_EV_PROCREF)) != 8'd0;
            3'd3: return (flags & (is_b ? EVC_EV_IFUNOTREADY : EVC_EV_IFUJUMP)) != 8'd0;
            3'd4: return (flags & EVC_EV_MISS) != 8'd0;
            default: return 1'b0;
        endcase
    endfunction

    // -------------------------------------------------------------------------
    // Combinational A-Bus, B-Bus, and ALU Input Network
    // -------------------------------------------------------------------------
    always_comb begin
        logic [15:0] b_val, a_val;
        logic        b_overridden;
        logic        a_overridden;
        logic        a_is_rmstk_default;
        logic [7:0]  idcnt_after_a;
        logic [15:0] id_peek_post_a;
        logic [3:0]  aluf_idx;

        b_val        = 16'd0;
        a_val        = 16'd0;
        b_overridden = 1'b0;

        // 1. Check FF override for B bus (FA=1, FB=6/7)
        if (ff_full_function_ok(cur_u) && ff_fa(cur_u.ff) == 2'd1) begin
            if (ff_fb(cur_u.ff) == 3'd6) begin
                logic [3:0]  psrn;
                logic [27:0] pva;
                psrn         = u_mem.proc_srn;
                pva          = u_mem.pipe_va[psrn];
                b_overridden = 1'b1;
                case (ff_fc(cur_u.ff))
                    3'd0: b_val = ~u_mem.get_fault_info();
                    3'd1: b_val = {4'd0, pva[27:16]};
                    3'd2: b_val = pva[15:0];
                    3'd3: b_val = u_mem.get_pipe2_at(psrn);
                    3'd4: b_val = ~u_mem.pipe_map_rp_pre[psrn];
                    3'd5: b_val = u_mem.get_pipe4_at(psrn);
                    3'd6: b_val = ~u_mem.get_config_word();
                    3'd7: b_val = u_mem.get_pipe5_at(psrn);
                endcase
            end else if (ff_fb(cur_u.ff) == 3'd7) begin
                logic [9:0] ifum_addr;
                ifum_addr    = {ifu_insset, ifu_opcode};
                b_overridden = 1'b1;
                case (ff_fc(cur_u.ff))
                    3'd0: b_val = ~ifu_pcx;
                    3'd1: b_val = ~(gen_io_mode ? event_cnt_b : event_cnt_a);
                    3'd2: b_val = ~ifum_hi[ifum_addr];
                    3'd3: b_val = ~({ifu_idcnt[2:0], ifu_insset, ifum_lo[ifum_addr][10:0]});
                    3'd4: b_val = ~event_cnt_b;
                    3'd5: b_val = u_mem.dbuf;
                    3'd6: b_val = ~cpreg;
                    3'd7: b_val = b_link_read_valid ? b_link_read : Link;
                endcase
            end
        end

        // Standard BSEL if not overridden by FF
        if (!b_overridden) begin
            if (cur_u.asel == 3'd7 && cur_u.bsel[2]) begin
                b_val = Q;
            end else begin
                case (cur_u.bsel)
                    3'd0: b_val = cur_task_md;
                    3'd1: b_val = cur_rm_stk_read;
                    3'd2: b_val = T;
                    3'd3: b_val = Q;
                    3'd4: b_val = {8'h00, cur_u.ff};
                    3'd5: b_val = {8'hFF, cur_u.ff};
                    3'd6: b_val = {cur_u.ff, 8'h00};
                    3'd7: b_val = {cur_u.ff, 8'hFF};
                endcase
            end
        end

        // 2. A-bus primary selection (HM Table 8a/8b)
        idcnt_after_a = ifu_idcnt;
        case (cur_u.asel)
            3'd0, 3'd1, 3'd4: a_val = cur_rm_stk_read;
            3'd2, 3'd3: begin
                if (!ff_decode_ok(cur_u)) begin
                    a_val = T;
                end else begin
                    case (cur_u.ff[7:6])
                        2'd0: a_val = cur_task_md;
                        2'd1: begin
                            a_val         = ifu_id_at_fn(ifu_idcnt);
                            idcnt_after_a = ifu_idcnt + 8'd1;
                        end
                        2'd2: a_val = Q;
                        2'd3: a_val = T;
                    endcase
                end
            end
            3'd5: begin
                a_val         = ifu_id_at_fn(ifu_idcnt);
                idcnt_after_a = ifu_idcnt + 8'd1;
            end
            3'd6: a_val = T;
            3'd7: a_val = ~shifter_unmasked_lo16;
        endcase

        // 3. FF A-source override (FA=0, FB=0..2)
        a_overridden       = 1'b0;
        a_is_rmstk_default = 1'b0;
        if (ff_decode_ok(cur_u) && cur_u.asel != 3'd7) begin
            logic [5:0] f6;
            logic       f6_valid;
            f6       = cur_u.ff[5:0];
            f6_valid = (cur_u.asel <= 3'd1) ||
                       (ff_full_function_ok(cur_u) && ff_fa(cur_u.ff) == 2'd0);
            if (f6_valid) begin
                if (f6[5:3] <= 3'd1) begin
                    a_val        = {12'd0, cur_u.ff[3:0]};
                    a_overridden = 1'b1;
                end else if (f6[5:3] == 3'd2) begin
                    case (f6[2:0])
                        3'd1: begin a_val = T;           a_overridden = 1'b1; end
                        3'd2: begin a_val = cur_task_md; a_overridden = 1'b1; end
                        3'd3: begin a_val = Q;           a_overridden = 1'b1; end
                        default: ;
                    endcase
                end
            end
        end
        if (!a_overridden && (cur_u.asel == 3'd0 || cur_u.asel == 3'd1)) begin
            a_is_rmstk_default = 1'b1;
        end

        // 4. TisId / RisId bus substitutions
        id_peek_post_a = ifu_id_at_fn(idcnt_after_a);
        if (ff_is_t_is_id(cur_u)) begin
            if (cur_u.bsel == 3'd2) b_val = id_peek_post_a;
            if (cur_u.asel == 3'd6) a_val = id_peek_post_a;
        end else if (ff_is_r_is_id(cur_u)) begin
            if (cur_u.bsel == 3'd1) b_val = id_peek_post_a;
            if (cur_u.asel == 3'd4 || a_is_rmstk_default) a_val = id_peek_post_a;
        end

        // 5. ALUFM lookup & Carry modifiers (XorCarry, XorSavedCarry, Carry20)
        aluf_idx         = (cur_u.asel == 3'd7) ? (4'd14 + {3'd0, cur_u.aluf[0]}) : cur_u.aluf;
        comb_alufm_entry = alufm_present[aluf_idx] ? alufm[aluf_idx] : 6'd0;
        comb_carry20     = ff_is_table11a(cur_u, 8'o026, 8'o077);

        if (ff_is_table11a(cur_u, 8'o025, 8'o077)) begin
            // XorSavedCarry
            comb_alufm_entry[5] ^= alu_carry;
        end else if (ff_is_table11a(cur_u, 8'o024, 8'o077)) begin
            // XorCarry
            comb_alufm_entry[5] ^= 1'b1;
        end

        comb_alu_a = a_val;
        comb_alu_b = b_val;
    end

    // -------------------------------------------------------------------------
    // Internal Execution Tasks (invoked on posedge clk)
    // -------------------------------------------------------------------------
    task automatic reset_cpu();
        real_PC               = 16'd0;
        prev_PC               = 16'd0;
        T                     = 16'd0;
        Q                     = 16'd0;
        Link                  = 16'd0;
        Cnt                   = 16'd0;
        ShC                   = 16'd0;
        RBase                 = 8'd0;
        MemBase               = 8'd0;
        MemBX                 = 2'd0;
        TIOA                  = 8'd0;
        StkP                  = 8'd0;
        stk_ovf               = 1'b0;
        stk_und               = 1'b0;
        cpreg                 = 16'd0;
        io_bad_parity         = 1'b0;
        io_atten_at_issue     = 1'b0;
        b_link_read           = 16'd0;
        b_link_read_valid     = 1'b0;
        b_link_read_age       = 1'b0;
        link_at_issue         = 16'd0;
        alu_zero              = 1'b0;
        alu_lt0               = 1'b0;
        alu_carry             = 1'b0;
        alu_overflow          = 1'b0;
        ctask                 = 4'd0;
        ready                 = 16'h0001;
        wakeup_pending        = 16'h0000;
        wakeup_pipe[0]        = 16'h0000;
        wakeup_pipe[1]        = 16'h0000;
        tasking_on            = 1'b1;
        tasking_resume_delay  = 8'd0;
        dispatch_or           = 16'd0;

        for (int i = 0; i < NUM_TASKS; i++) begin
            task_t[i]            = 16'd0;
            task_tpc[i]          = 16'd0;
            task_link[i]         = 16'd0;
            task_membase[i]      = 8'd0;
            task_rbase[i]        = 8'd0;
            task_tioa[i]         = 8'd0;
            task_alu_zero[i]     = 1'b0;
            task_alu_lt0[i]      = 1'b0;
            task_alu_carry[i]    = 1'b0;
            task_alu_overflow[i] = 1'b0;
            task_subtask[i]      = 2'd0;
            task_dispatch[i]     = 16'd0;
            task_md[i]           = 16'd0;
            task_md_valid[i]     = 1'b0;
            task_md_ready[i]     = 64'd0;
        end

        ifu_pcf               = 16'd0;
        ifu_pcx               = 16'd0;
        ifu_opcode            = 8'd0;
        ifu_alpha             = 8'd0;
        ifu_beta              = 8'd0;
        ifu_insset            = 2'd0;
        ifu_idcnt             = 8'd0;
        ifu_length            = 8'd0;
        ifu_sign              = 1'b0;
        ifu_packed_a          = 1'b0;
        ifu_type_pause        = 1'b0;
        ifu_type_jump         = 1'b0;
        ifu_n                 = 4'd0;
        ifu_active            = 1'b0;
        ifu_warmup            = 8'd0;
        ifu_test              = 16'd0;
        ifu_test_pending      = 1'b0;
        for (int i = 0; i < 8; i++) ifu_test_queue[i] = 8'd0;
        ifu_test_count        = 4'd0;
        ifu_saved_stkp        = 8'd0;
        ifu_save_stkp_pending = 1'b0;
        ifu_dispatch_rbase    = -1;
        ifu_dispatch_count    = 64'd0;
        reschedule_pending    = 8'd0;
        reschedule_cond       = 1'b0;
        brk_opcode            = 8'd0;
        brk_pending           = 1'b0;

        event_cnt_a           = 16'd0;
        event_cnt_b           = 16'd0;
        event_cnt_ctrl_hi     = 4'd0;
        event_cnt_ctrl_lo     = 8'd0;
        gen_io_mode           = 1'b0;
        evc_a_enable          = 1'b0;
        evc_a_event           = 3'd0;
        evc_a_tasksall        = 1'b0;
        evc_b_enable          = 1'b0;
        evc_b_event           = 3'd0;
        evc_b_tasksall        = 1'b0;
        evc_events            = 8'd0;

        tasksim               = 7'd0;
        holdsim               = 8'd0;
        tasksim_fired         = 1'b0;
        tasksim_loaded        = 1'b0;
        holdsim_loaded        = 1'b0;
        holdsim_delay         = 1'b0;
        holdsim_hold          = 1'b0;

        junk_tw_enabled       = 1'b0;
        junk_tw_countdown     = 16'd0;

        cycles                = 64'd0;
        halted                = 1'b0;
        halt_reason           = 4'd0;
        io_read_stb           = 1'b0;
        io_read_stb_task      = 4'd0;
        io_read_stb_subtask   = 2'd0;
        io_read_stb_tioa      = 8'd0;
        io_write_stb          = 1'b0;
        io_write_task         = 4'd0;
        io_write_subtask      = 2'd0;
        io_write_tioa         = 8'd0;
        io_write_data         = 16'd0;
    endtask

    task automatic junk_timer_enable_task(input logic en);
        wakeup_pending[DORADO_JUNK_TASK] = 1'b0;
        junk_tw_enabled = en;
        if (junk_tw_enabled && junk_tw_countdown == 16'd0) begin
            junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
        end
    endtask

    task automatic junk_timer_tick_task();
        if (!junk_tw_enabled) return;
        if (junk_tw_countdown == 16'd0) begin
            junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
        end
        junk_tw_countdown = junk_tw_countdown - 16'd1;
        if (junk_tw_countdown == 16'd0) begin
            wakeup_pending[DORADO_JUNK_TASK] = 1'b1;
            junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
        end
    endtask

    task automatic ifu_test_tick_task();
        logic loads_fg;
        if (!ifu_test_pending) return;
        ifu_test_pending = 1'b0;
        loads_fg = (real_PC[11:0] == 12'o5042) ||
                   (real_PC[11:0] == 12'o5052) ||
                   (real_PC[11:0] == 12'o5056);
        if ((ifu_test & IFUTEST_EN) != 16'd0 && ((ifu_test[15:8] != 8'd0) || loads_fg)) begin
            if (ifu_test_count >= 4'd8) begin
                for (int i = 0; i < 7; i++) ifu_test_queue[i] = ifu_test_queue[i + 1];
                ifu_test_queue[7] = ifu_test[15:8];
            end else begin
                ifu_test_queue[ifu_test_count[2:0]] = ifu_test[15:8];
                ifu_test_count = ifu_test_count + 4'd1;
            end
        end
    endtask

    task automatic ifu_test_pop_byte_task(output logic [7:0] out_byte);
        if (ifu_test_count == 4'd0) begin
            out_byte = 8'd0;
            return;
        end
        out_byte = ifu_test_queue[0];
        ifu_test_count = ifu_test_count - 4'd1;
        for (int i = 0; i < 7; i++) begin
            if (i < int'(ifu_test_count)) ifu_test_queue[i] = ifu_test_queue[i + 1];
        end
    endtask

    task automatic ifu_fetch_byte_task(
        input  logic [15:0] pc,
        output logic [7:0]  out_byte,
        output logic        out_faulted
    );
        logic [15:0]    word_off;
        logic           byte_sel;
        logic [27:0]    br31, va;
        dm_fault_kind_t f;
        logic [15:0]    word;
        logic           high_byte;

        out_faulted = 1'b0;
        word_off    = {1'b0, pc[15:1]};
        byte_sel    = pc[0];
        br31        = u_mem.br[31];
        va          = (br31 + {12'd0, word_off}) & 28'h0FFFFFFF;

        u_mem.memory_ref_task(DM_REF_IFETCH, va, 16'd0, 8'd0, 4'd0, 2'd0, f);
        if (u_mem.last_ref_miss) evc_events |= EVC_EV_MISS;
        if (f != DM_FAULT_NONE) out_faulted = 1'b1;

        word      = u_mem.md;
        high_byte = (ifu_insset < 2'd2) ? (byte_sel == 1'b0) : (byte_sel == 1'b1);
        out_byte  = high_byte ? word[15:8] : word[7:0];
    endtask

    task automatic eval_branch_condition_task(
        input  dorado_uinstr_t u,
        input  logic [2:0]     cond,
        output logic           out_r
    );
        int          rm_a;
        logic [15:0] rv;
        rm_a = rm_address_fn(u, ctask, RBase, task_subtask[ctask], StkP);
        rv   = rm_stk_read_fn(rm_a);
        case (cond)
            3'd0: out_r = alu_zero;
            3'd1: out_r = alu_lt0;
            3'd2: out_r = ~alu_carry;
            3'd3: begin
                out_r = (Cnt == 16'd0);
                Cnt   = Cnt - 16'd1;
            end
            3'd4: out_r = (rm_a < 512) && rv[15];
            3'd5: out_r = (rm_a < 512) && rv[0];
            3'd6: begin
                if (ctask == 4'd0) out_r = reschedule_cond;
                else               out_r = ~io_atten_at_issue;
            end
            3'd7: out_r = ~alu_overflow;
        endcase
    endtask

    task automatic task_schedule_task(
        input  logic [15:0] next_pc_in,
        input  logic        block_in_non_emul,
        input  logic        freeze,
        output logic [15:0] chosen_pc
    );
        logic        trig;
        logic        emu_or_ft;
        logic [15:0] avail;
        logic [3:0]  bnt;
        logic        should_switch;

        // 1. Promote 2-stage microcode wakeup pipe
        wakeup_pending |= wakeup_pipe[1];
        wakeup_pipe[1]  = wakeup_pipe[0];
        wakeup_pipe[0]  = 16'd0;

        // 2. TASKSIM tick
        if (tasksim_fired) begin
            wakeup_pending[DORADO_TASKSIM_TASK] = 1'b1;
        end else if (tasksim_loaded) begin
            tasksim_loaded = 1'b0;
        end else if (tasksim != 7'd0) begin
            if (tasksim == 7'o177) begin
                tasksim_fired = 1'b1;
                wakeup_pending[DORADO_TASKSIM_TASK] = 1'b1;
            end else begin
                tasksim = tasksim + 7'd1;
            end
        end

        // 3. HOLDSIM tick
        trig = 1'b0;
        if (holdsim_loaded) begin
            holdsim_loaded = 1'b0;
            trig           = holdsim[0];
        end else if (holdsim != 8'd0) begin
            trig    = holdsim[0];
            holdsim = {holdsim[6:0], holdsim[7]};
        end
        holdsim_hold  = holdsim_delay;
        holdsim_delay = trig;

        // 4. Event counters tick
        emu_or_ft = (ctask == 4'd0) || (ctask == 4'o17);
        if (evc_a_enable && (evc_a_tasksall || emu_or_ft) &&
            event_counter_hit_fn(evc_a_event, evc_events, 1'b0)) begin
            event_cnt_a = event_cnt_a + 16'd1;
        end
        if (evc_b_enable && (evc_b_tasksall || emu_or_ft) &&
            event_counter_hit_fn(evc_b_event, evc_events, 1'b1)) begin
            event_cnt_b = event_cnt_b + 16'd1;
        end
        evc_events = 8'd0;

        // 5. TaskingOn delay countdown
        if (tasking_resume_delay > 8'd0) begin
            tasking_resume_delay = tasking_resume_delay - 8'd1;
            if (tasking_resume_delay == 8'd0) tasking_on = 1'b1;
        end

        if (freeze) begin
            chosen_pc = next_pc_in;
            return;
        end

        if (block_in_non_emul) begin
            ready[ctask]          = 1'b0;
            wakeup_pending[ctask] = 1'b0;
        end

        if (!tasking_on) begin
            chosen_pc = next_pc_in;
            return;
        end

        avail         = ready | wakeup_pending;
        bnt           = task_bnt_fn(avail);
        should_switch = (bnt > ctask) || block_in_non_emul;

        if (block_in_non_emul && bnt == ctask) begin
            logic [15:0] avail2;
            avail2 = (ready | wakeup_pending) & ~(16'd1 << ctask);
            bnt    = task_bnt_fn(avail2);
        end

        if (!should_switch || bnt == ctask) begin
            chosen_pc = next_pc_in;
            return;
        end

        // Save current task state
        task_t[ctask]            = T;
        task_tpc[ctask]          = next_pc_in;
        task_link[ctask]         = Link;
        task_membase[ctask]      = MemBase;
        task_rbase[ctask]        = RBase;
        task_tioa[ctask]         = TIOA;
        task_alu_zero[ctask]     = alu_zero;
        task_alu_lt0[ctask]      = alu_lt0;
        task_alu_carry[ctask]    = alu_carry;
        task_alu_overflow[ctask] = alu_overflow;

        // Acknowledge wakeup & load BNT state
        wakeup_pending[bnt]      = 1'b0;
        ready[bnt]               = 1'b1;
        ctask                    = bnt;
        T                        = task_t[bnt];
        chosen_pc                = task_tpc[bnt];
        Link                     = task_link[bnt];
        MemBase                  = task_membase[bnt];
        RBase                    = task_rbase[bnt];
        TIOA                     = task_tioa[bnt];
        alu_zero                 = task_alu_zero[bnt];
        alu_lt0                  = task_alu_lt0[bnt];
        alu_carry                = task_alu_carry[bnt];
        alu_overflow             = task_alu_overflow[bnt];
    endtask

    // -------------------------------------------------------------------------
    // Main Synchronous 1-Tick-Per-Microinstruction Execution Block
    // -------------------------------------------------------------------------
    always @(posedge clk) begin
        if (rst) begin
            reset_cpu();
            u_mem.reset_memory();
        end else if (clk_en && !halted) begin
            dorado_uinstr_t u;
            logic           from_im;
            logic           step_valid;

            io_read_stb       = 1'b0;
            io_write_stb      = 1'b0;
            u_mem.fastio_stb  = 1'b0;
            wakeup_pending   |= ext_wakeup_req;
            if (ext_subtask_we) begin
                task_subtask[ext_subtask_task] = ext_subtask_val;
            end
            u                 = cur_u;
            from_im           = 1'b1;
            step_valid        = 1'b1;

            if (bb_present) begin
                if (bb_ss_inject) begin
                    from_im = 1'b0;
                end else if (!bb_running) begin
                    cycles     = cycles + 64'd1;
                    step_valid = 1'b0;
                end
            end

            if (step_valid && from_im) begin
                if (real_PC >= IM_SIZE || !im_present[real_PC[11:0]]) begin
                    halted      = 1'b1;
                    halt_reason = 4'd2; // CPU_HALT_NO_CODE
                    step_valid  = 1'b0;
                end else if (im_breakpoint[real_PC[11:0]]) begin
                    halted      = 1'b1;
                    halt_reason = 4'd7; // CPU_HALT_BREAKPOINT
                    step_valid  = 1'b0;
                end
            end

            if (step_valid) begin
                logic        md_not_ready, hold_asserted;
                logic        stk_underflow, stk_overflow, stk_err;
                int          stk_delta, stk_off, stk_new_off;
                logic [15:0] md_at_issue;
                logic [7:0]  rbase_at_issue, rbase_after_ff, rbase_next_instr;
                logic [4:0]  membase_at_issue;
                logic [15:0] a, b, alu, pd, np;
                logic [15:0] ifetch_id;
                logic        alu_live_carry, new_carry, new_ovf, new_alu_zero, new_alu_lt0;
                logic        shift_active_cur;
                logic [1:0]  fa;
                logic [2:0]  fb, fc;
                logic        ff_exec_ok;

                // Hold check
                md_not_ready  = from_im && uinstr_reads_md_fn(u) &&
                                (cycles < task_md_ready[ctask]);
                hold_asserted = md_not_ready || holdsim_hold;
                if (hold_asserted) evc_events |= EVC_EV_HOLD;

                if (holdsim_hold && !u_mem.mcr_dishold()) begin
                    logic [15:0] dummy_pc;
                    if (from_im) junk_timer_tick_task();
                    task_schedule_task(real_PC, 1'b0, 1'b1, dummy_pc);
                    cycles = cycles + 64'd1;
                end else begin
                    // Stack underflow/overflow pre-check (cpu.c:5531)
                    stk_underflow = 1'b0;
                    stk_overflow  = 1'b0;
                    stk_err       = 1'b0;
                    if (u.block && ctask == 4'd0) begin
                        stk_delta = int'(signed'(stk_signed_delta(u.rstk)));
                        if (stk_delta != 0 || (u.rstk[3] && !ff_is_dont_modify_stkp(u))) begin
                            stk_off     = int'(StkP[5:0]);
                            stk_new_off = stk_off + stk_delta;
                            if (stk_new_off < 0)    stk_underflow = 1'b1;
                            if (stk_new_off > 8'o77) stk_overflow  = 1'b1;
                            if (u.rstk[3] && !ff_is_dont_modify_stkp(u) &&
                                ((stk_new_off & 63) == 0) && stk_new_off >= 0) begin
                                stk_underflow = 1'b1;
                            end
                            stk_err = stk_underflow || stk_overflow;
                        end
                    end

                    if (stk_err) begin
                        stk_und            = stk_underflow;
                        stk_ovf            = stk_overflow;
                        wakeup_pending[15] = 1'b1;
                        prev_PC            = real_PC;
                        if (from_im) junk_timer_tick_task();
                        task_schedule_task(real_PC, 1'b0, 1'b0, real_PC);
                        if (from_im) begin
                            cycles = cycles + 64'd1;
                            u_mem.memory_tick();
                        end
                        if (ifu_active && ifu_warmup > 8'd0) begin
                            ifu_warmup = ifu_warmup - 8'd1;
                        end
                    end else begin
                        // -----------------------------------------------------
                        // 1. Issue-Time Snapshots
                        // -----------------------------------------------------
                        link_at_issue = Link;
                        if (b_link_read_valid) begin
                            if (b_link_read_age) b_link_read_valid = 1'b0;
                            else                 b_link_read_age   = 1'b1;
                        end
                        dispatch_or          = task_dispatch[ctask];
                        task_dispatch[ctask] = 16'd0;
                        md_at_issue          = cur_task_md;
                        rbase_at_issue       = RBase;
                        membase_at_issue     = MemBase[4:0];
                        io_atten_at_issue    = (io_attached && ctask != 4'd0) ? io_atten_in : 1'b0;

                        // -----------------------------------------------------
                        // 2. Capture Combinational Bus & ALU Results + Side Effects
                        // -----------------------------------------------------
                        a                = comb_alu_a;
                        b                = comb_alu_b;
                        alu              = alu_result;
                        shift_active_cur = (u.asel == 3'd7);

                        // Apply ff_override_b side effects (FaultInfo clear, Link<-B, Q<-B)
                        if (ff_full_function_ok(u) && ff_fa(u.ff) == 2'd1) begin
                            if (ff_fb(u.ff) == 3'd6) begin
                                if (ff_fc(u.ff) == 3'd0) u_mem.fault_clear();
                                if (u.bsel == 3'd3) Q = b;
                            end else if (ff_fb(u.ff) == 3'd7) begin
                                if (ff_fc(u.ff) == 3'd6) Link = b;
                                if (ff_fc(u.ff) == 3'd7) b_link_read_valid = 1'b0;
                                if (u.bsel == 3'd3) Q = b;
                            end
                        end

                        // Advance IFU idcnt if A bus consumed Id
                        if (u.asel == 3'd5 ||
                            ((u.asel == 3'd2 || u.asel == 3'd3) &&
                             ff_decode_ok(u) && u.ff[7:6] == 2'd1)) begin
                            ifu_idcnt = ifu_idcnt + 8'd1;
                        end
                        ifetch_id = ifu_id_at_fn(ifu_idcnt);

                        alu_live_carry = alu_is_arith_out ? alu_carry_out : 1'b0;
                        new_carry      = alu_is_arith_out ? alu_carry_out : alu_carry;
                        new_ovf        = alu_is_arith_out ? alu_overflow_out : alu_overflow;
                        new_alu_zero   = (alu == 16'd0);
                        new_alu_lt0    = alu[15];

                        // -----------------------------------------------------
                        // 3. FF Post-ALU Side Effects (ff_apply_post)
                        // -----------------------------------------------------
                        pd         = alu;
                        fa         = ff_fa(u.ff);
                        fb         = ff_fb(u.ff);
                        fc         = ff_fc(u.ff);
                        ff_exec_ok = ff_full_function_ok(u);

                        if (!ff_exec_ok && ff_decode_ok(u) && u.asel <= 3'd3) begin
                            fa         = 2'd0;
                            fb         = u.ff[5:3];
                            fc         = u.ff[2:0];
                            ff_exec_ok = 1'b1;
                        end

                        // Special long-call Output<-B form (cpu.c:1892)
                        if (u.asel == 3'd4 && u.bsel == 3'd1 && u.lc == 3'd6 &&
                            u.aluf == 4'o13 && u.ff == 8'o327 && u.jcn == 8'o017 &&
                            io_attached && io_has_write_in) begin
                            io_write_stb     = 1'b1;
                            io_write_task    = ctask;
                            io_write_subtask = task_subtask[ctask];
                            io_write_tioa    = TIOA;
                            io_write_data    = b;
                        end

                        if (ff_exec_ok) begin
                            if (fa == 2'd0) begin
                                if (fb == 3'd3) begin
                                    case (fc)
                                        3'd1: pd = u_mem.get_read_map();
                                        3'd2: begin
                                            pd            = io_attached ? io_read_data : 16'hFFFF;
                                            io_bad_parity = io_attached ? io_read_bad_parity : 1'b1;
                                            if (io_attached) begin
                                                io_read_stb         = 1'b1;
                                                io_read_stb_task    = ctask;
                                                io_read_stb_subtask = task_subtask[ctask];
                                                io_read_stb_tioa    = TIOA;
                                            end
                                        end
                                        3'd3: begin
                                            pd            = io_attached ? io_read_data : 16'hFFFF;
                                            if (io_attached) begin
                                                io_read_stb         = 1'b1;
                                                io_read_stb_task    = ctask;
                                                io_read_stb_subtask = task_subtask[ctask];
                                                io_read_stb_tioa    = TIOA;
                                            end
                                        end
                                        3'd4, 3'd5: ifu_idcnt = ifu_idcnt + 8'd1; // RisId / TisId
                                        3'd6: begin
                                            if (io_attached) begin
                                                io_write_stb     = 1'b1;
                                                io_write_task    = ctask;
                                                io_write_subtask = task_subtask[ctask];
                                                io_write_tioa    = TIOA;
                                                io_write_data    = b;
                                            end
                                        end
                                        3'd7: MemBase = MemBase ^ 8'h01; // FlipMemBase
                                        default: ;
                                    endcase
                                end else if (fb == 3'd7) begin
                                    case (fc)
                                        3'd0: begin // BigBDispatch <- B
                                            Link                 = b;
                                            task_dispatch[ctask] = {8'd0, b[7:0]};
                                        end
                                        3'd1: begin // BDispatch <- B
                                            Link                 = b;
                                            task_dispatch[ctask] = {13'd0, b[2:0]};
                                        end
                                        3'd2: begin // Multiply
                                            task_dispatch[ctask] |= (Q & 16'h0002);
                                            Q  = {alu[0], Q[15:1]};
                                            pd = {alu_live_carry, alu[15:1]};
                                        end
                                        3'd3: Q = b;
                                        default: ;
                                    endcase
                                end
                            end else if (fa == 2'd1) begin
                                if (fb == 3'd0) begin
                                    case (fc)
                                        3'd0: begin // PCF <- B
                                            ifu_pcf    = b;
                                            ifu_idcnt  = 8'd0;
                                            ifu_active = 1'b1;
                                            ifu_warmup = 8'd5;
                                        end
                                        3'd1: begin // IFUTest <- B
                                            ifu_test         = b;
                                            ifu_test_pending = 1'b1;
                                            if (b[0]) junk_timer_enable_task(1'b0);
                                        end
                                        3'd2: ifu_test_tick_task(); // IFUTick
                                        3'd3: reschedule_pending = 8'd1; // RescheduleNow
                                        3'd4: junk_timer_enable_task(b[0]); // AckJunkTW <- B
                                        3'd5: MemBase = {3'd0, b[12:8]};
                                        3'd6: RBase   = {4'd0, b[3:0]};
                                        3'd7: begin
                                            MemBase = {3'd0, b[12:8]};
                                            RBase   = {4'd0, b[3:0]};
                                        end
                                    endcase
                                end else if (fb == 3'd2) begin
                                    case (fc)
                                        3'd2: u_mem.cflags_load(a);
                                        3'd3: u_mem.br_lo_load(MemBase[4:0], a);
                                        3'd4: u_mem.br_hi_load(MemBase[4:0], a);
                                        3'd6: u_mem.mcr_load(a, b);
                                        3'd7: u_mem.proc_srn = b[3:0];
                                        default: ;
                                    endcase
                                end else if (fb == 3'd3) begin
                                    case (fc)
                                        3'd0: begin // InsSetorEvent <- B
                                            if (b[15]) begin
                                                ifu_insset = b[9:8];
                                            end else begin
                                                event_cnt_ctrl_hi = b[11:8];
                                                event_cnt_ctrl_lo = b[7:0];
                                                gen_io_mode       = ((b & 16'h0C00) == 16'd0);
                                                evc_a_enable      = b[11];
                                                evc_a_event       = b[7:5];
                                                evc_a_tasksall    = b[4];
                                                evc_b_enable      = b[10];
                                                evc_b_event       = b[3:1];
                                                evc_b_tasksall    = b[0];
                                            end
                                        end
                                        3'd1: event_cnt_b = b;
                                        3'd2: begin
                                            reschedule_pending = 8'd2;
                                            reschedule_cond    = 1'b1;
                                        end
                                        3'd3: begin
                                            reschedule_pending = 8'd0;
                                            reschedule_cond    = 1'b0;
                                        end
                                        3'd4: begin // IFUMRH <- B
                                            logic [9:0] ia;
                                            ia               = {ifu_insset, ifu_opcode};
                                            ifum_hi[ia]      = b;
                                            ifum_present[ia] = 1'b1;
                                        end
                                        3'd5: begin // IFUMLH <- B
                                            logic [9:0] ia;
                                            ia               = {ifu_insset, ifu_opcode};
                                            ifum_lo[ia]      = b;
                                            ifum_present[ia] = 1'b1;
                                        end
                                        3'd6: begin // IFUReset
                                            ifu_active       = 1'b0;
                                            ifu_warmup       = 8'd0;
                                            ifu_pcf          = 16'd0;
                                            ifu_test         = 16'd1;
                                            ifu_test_pending = 1'b0;
                                            ifu_test_count   = 4'd0;
                                            brk_pending      = 1'b0;
                                            brk_opcode       = 8'd0;
                                        end
                                        3'd7: begin // BrkIns <- B
                                            ifu_opcode  = b[15:8];
                                            brk_opcode  = b[15:8];
                                            brk_pending = 1'b1;
                                        end
                                    endcase
                                end else if (fb == 3'd4) begin
                                    case (fc)
                                        3'd0: u_mem.dmux_use_dmd();
                                        3'd1: u_mem.dmux_strobe(b);
                                        3'd2: begin
                                            tasking_on           = 1'b0;
                                            tasking_resume_delay = 8'd0;
                                        end
                                        3'd3: tasking_resume_delay = 8'd2;
                                        3'd4: StkP = b[7:0];
                                        3'd5: StkP = ifu_saved_stkp;
                                        3'd6: Cnt  = b;
                                        3'd7: Link = b;
                                    endcase
                                end else if (fb == 3'd5) begin
                                    case (fc)
                                        3'd0: Q    = Q << 1;
                                        3'd1: Q    = Q >> 1;
                                        3'd2: TIOA = b[15:8];
                                        3'd4: begin // Hold&TaskSim <- B
                                            tasksim                             = b[14:8];
                                            holdsim                             = {b[7:1], 1'b0};
                                            tasksim_fired                       = 1'b0;
                                            tasksim_loaded                      = 1'b1;
                                            holdsim_loaded                      = 1'b1;
                                            wakeup_pending[DORADO_TASKSIM_TASK] = 1'b0;
                                            wakeup_pipe[0][DORADO_TASKSIM_TASK] = 1'b0;
                                            wakeup_pipe[1][DORADO_TASKSIM_TASK] = 1'b0;
                                        end
                                        3'd5: ShC = field_desc_to_shc(a, 1'b1);
                                        3'd6: ShC = field_desc_to_shc(a, 1'b0);
                                        3'd7: ShC = b;
                                        default: ;
                                    endcase
                                end
                            end else if (fa == 2'd2) begin
                                if (fb <= 3'd1) begin
                                    RBase = {4'd0, u.ff[3:0]};
                                end else if (fb == 3'd4) begin
                                    TIOA = {TIOA[7:3], u.ff[2:0]};
                                end else if (fb == 3'd5) begin
                                    if (fc <= 3'd3) begin
                                        MemBase = {4'd0, MemBX, u.ff[1:0]};
                                    end
                                end else if (fb == 3'd6) begin
                                    case (fc)
                                        3'd2, 3'd3: begin // ALUFMRW / ALUFMEM
                                            if (u_mem.dmux_pending) begin
                                                u_mem.dmux_read(pd);
                                            end else begin
                                                logic [3:0] idx;
                                                idx = u.aluf;
                                                pd  = alufm_present[idx] ? {10'd0, alufm[idx]} : 16'd0;
                                                if (fc == 3'd2) begin
                                                    alufm[idx]         = {b[7], b[4:0]};
                                                    alufm_present[idx] = 1'b1;
                                                end
                                            end
                                        end
                                        3'd4: pd = Cnt;
                                        3'd5: pd = {1'b0, MemBX, MemBase[4:0], stk_ovf, stk_und, 2'b00, RBase[3:0]};
                                        3'd6: pd = {TIOA, StkP};
                                        3'd7: pd = ShC;
                                        default: ;
                                    endcase
                                end else if (fb == 3'd7) begin
                                    case (fc)
                                        3'd0: pd = alu >> 1;
                                        3'd1: pd = (alu >> 1) | (alu << 15);
                                        3'd2: pd = {alu_live_carry, alu[15:1]};
                                        3'd3: pd = {alu[15], alu[15:1]};
                                        3'd4: pd = alu << 1;
                                        3'd5: pd = (alu << 1) | (alu >> 15);
                                        3'd6: begin // Divide
                                            logic q0;
                                            q0 = Q[15];
                                            Q  = {Q[14:0], alu_live_carry};
                                            pd = {alu[14:0], q0};
                                        end
                                        3'd7: begin // CDivide
                                            logic q0;
                                            q0 = Q[15];
                                            Q  = {Q[14:0], ~alu_live_carry};
                                            pd = {alu[14:0], q0};
                                        end
                                    endcase
                                end
                            end else if (fa == 2'd3) begin
                                if (fb <= 3'd3) begin
                                    MemBase = {3'd0, u.ff[4:0]};
                                end else if (fb == 3'd4 || fb == 3'd5) begin
                                    Cnt = (u.ff[3:0] == 4'd0) ? 16'd16 : {12'd0, u.ff[3:0]};
                                end else if (fb == 3'd6 || fb == 3'd7) begin
                                    wakeup_pipe[0][u.ff[3:0]] = 1'b1;
                                end
                            end
                        end

                        // Barrel-shifter Pd-mux stage (post-ALU)
                        if (shift_active_cur) begin
                            pd = (pd & ~shifter_out_mask) | (shifter_out_fill & shifter_out_mask);
                        end

                        // -----------------------------------------------------
                        // 4. Processor Memory Reference (ASEL = 0..3)
                        // -----------------------------------------------------
                        if (u.asel <= 3'd3 && !jcn_is_write_im_fn(u)) begin
                            logic           io_task;
                            dm_ref_kind_t   kind;
                            logic           skip_to_done;

                            io_task      = (ctask != 4'd0);
                            kind         = decode_ref_kind_fn(u, io_task);
                            skip_to_done = 1'b0;

                            if (kind != DM_REF_NONE) begin
                                if (kind == DM_REF_IOSTORE && io_attached && io_has_write_in) begin
                                    io_write_stb     = 1'b1;
                                    io_write_task    = ctask;
                                    io_write_subtask = task_subtask[ctask];
                                    io_write_tioa    = TIOA;
                                    io_write_data    = b;
                                    if (ctask != 4'd0) skip_to_done = 1'b1;
                                end

                                if (!skip_to_done) begin
                                    logic [4:0]     mb_eff;
                                    logic [27:0]    br_val, disp, va;
                                    logic [3:0]     fcnt_before;
                                    dm_fault_kind_t ref_fault;
                                    logic           recorded_fault;

                                    mb_eff = membase_at_issue;
                                    if (ctask != 4'd0) begin
                                        mb_eff |= {2'b00, task_subtask[ctask], 1'b0};
                                    end
                                    br_val = u_mem.mcr_disbr() ? 28'd0 : u_mem.br[mb_eff];
                                    if (kind == DM_REF_IFETCH) begin
                                        br_val = {br_val[27:8], ifetch_id[7:0]};
                                    end
                                    disp = (kind == DM_REF_LONGFETCH)
                                         ? {b[11:0], a}
                                         : {12'd0, a};
                                    va = (br_val + disp) & 28'h0FFFFFFF;

                                    fcnt_before = u_mem.fault_count;
                                    u_mem.memory_ref_task(kind, va, b, TIOA, ctask,
                                                          task_subtask[ctask], ref_fault);
                                    recorded_fault = (ref_fault != DM_FAULT_NONE) ||
                                                     (u_mem.fault_count != fcnt_before);
                                    evc_events |= EVC_EV_PROCREF;
                                    if (u_mem.last_ref_miss) evc_events |= EVC_EV_MISS;

                                    if (ref_fault == DM_FAULT_NONE &&
                                        (kind == DM_REF_FETCH || kind == DM_REF_IFETCH ||
                                         kind == DM_REF_LONGFETCH)) begin
                                        task_md[ctask]       = u_mem.md;
                                        task_md_valid[ctask] = 1'b1;
                                        task_md_ready[ctask] = cycles + 64'(u_mem.last_ref_latency);
                                    end

                                    if (recorded_fault && kind != DM_REF_IFETCH &&
                                        !u_mem.mcr_nowake()) begin
                                        wakeup_pending[15] = 1'b1;
                                    end
                                end
                            end
                        end

                        // -----------------------------------------------------
                        // 5. Next-PC & IFUJump Evaluation (next_pc)
                        // -----------------------------------------------------
                        np                 = real_PC + 16'd1;
                        rbase_after_ff     = RBase;
                        RBase              = rbase_at_issue;
                        ifu_dispatch_rbase = -1;

                        begin
                            logic [7:0] jcn;
                            logic       top1;
                            logic [1:0] top2;
                            logic       ff_cond_or;
                            logic       is_long;

                            jcn        = u.jcn;
                            top1       = jcn[7];
                            top2       = jcn[7:6];
                            ff_cond_or = 1'b0;
                            is_long    = (jcn[7:4] == 4'd0);

                            if (u.bsel < 3'd4 && !is_long) begin
                                if (u.asel <= 3'd3) begin
                                    if ((u.ff[5:0] & 6'o70) == 6'o60) begin
                                        eval_branch_condition_task(u, u.ff[2:0], ff_cond_or);
                                    end
                                end else if (ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) begin
                                    eval_branch_condition_task(u, u.ff[2:0], ff_cond_or);
                                end
                            end

                            if (top1 == 1'b1) begin
                                logic [15:0] page_addr;
                                page_addr = {10'd0, jcn[5:0]};
                                if (top2 == 2'd2) begin
                                    // Local Jump/Call
                                    logic [15:0] page;
                                    page = real_PC & ~(16'(CPU_PAGE_SIZE - 1));
                                    np   = page | page_addr | {15'd0, ff_cond_or} | dispatch_or;
                                    if (page_addr[3:0] == 4'd0 && !ff_loads_link_fn(u)) begin
                                        Link = real_PC + 16'd1;
                                    end
                                end else begin
                                    // Global Call
                                    logic [15:0] quadrant;
                                    quadrant = real_PC & ~(16'(CPU_QUADRANT_SIZE - 1));
                                    np       = quadrant | (page_addr * 16'(CPU_PAGE_SIZE)) |
                                               {15'd0, ff_cond_or} | dispatch_or;
                                    if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                                end
                            end else begin
                                logic [2:0] low3;
                                low3 = jcn[2:0];

                                if (jcn == 8'o045 && u.ff == 8'o147) begin
                                    // LoadRam End-item exit
                                    int          rm_a;
                                    logic [15:0] flag;
                                    rm_a = rm_address_fn(u, ctask, RBase, task_subtask[ctask], StkP);
                                    flag = rm_stk_read_fn(rm_a);
                                    if (!flag[0]) np = {4'd0, Link[11:0]};
                                    else          np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) | 16'o021;
                                end else if (jcn[7:4] == 4'd0) begin
                                    // Long Jump/Call
                                    logic [15:0] quadrant, addr12;
                                    quadrant = real_PC & ~(16'(CPU_QUADRANT_SIZE - 1));
                                    addr12   = {4'd0, u.ff, jcn[3:0]};
                                    np       = quadrant | addr12 | dispatch_or;
                                    if (jcn[3:0] == 4'd0 && !ff_loads_link_fn(u)) begin
                                        Link = real_PC + 16'd1;
                                    end
                                end else if (low3 == 3'd7) begin
                                    if (jcn[7:5] == 3'd1) begin
                                        // IFUJump (0 0 1 n n 1 1 1)
                                        logic [1:0] n_slot;
                                        n_slot = jcn[4:3];
                                        if (!ifu_active || ifu_warmup > 8'd0) begin
                                            evc_events |= EVC_EV_IFUNOTREADY;
                                            np = ifu_trap_addr_fn(6'o34, n_slot);
                                            if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                                        end else begin
                                            logic        resched_trap, cond_true, fetch_faulted, test_feed;
                                            logic [7:0]  opcode;
                                            logic [9:0]  ifum_addr;
                                            logic [15:0] lh, rh, ifaddr, pc_after;

                                            resched_trap = 1'b0;
                                            if (reschedule_pending > 8'd0) begin
                                                reschedule_pending = reschedule_pending - 8'd1;
                                                if (reschedule_pending == 8'd0) resched_trap = 1'b1;
                                            end

                                            cond_true = 1'b0;
                                            if (u.bsel < 3'd4 && ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) begin
                                                eval_branch_condition_task(u, u.ff[2:0], cond_true);
                                            end

                                            ifu_pcx       = ifu_pcf;
                                            fetch_faulted = 1'b0;
                                            test_feed     = (ifu_test_count > 4'd0);

                                            if (brk_pending) begin
                                                opcode      = brk_opcode;
                                                brk_pending = 1'b0;
                                                test_feed   = 1'b0;
                                            end else if (test_feed) begin
                                                ifu_test_pop_byte_task(opcode);
                                            end else begin
                                                ifu_fetch_byte_task(ifu_pcf, opcode, fetch_faulted);
                                            end

                                            if (fetch_faulted) begin
                                                np = ifu_trap_addr_fn(6'o00, n_slot);
                                                if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                                            end else begin
                                                ifu_opcode = opcode;
                                                ifum_addr  = {ifu_insset, opcode};
                                                if (!ifum_present[ifum_addr]) begin
                                                    halted      = 1'b1;
                                                    halt_reason = 4'd11; // CPU_HALT_IFU_NO_ENTRY
                                                end else begin
                                                    logic op_faulted;
                                                    lh = ifum_hi[ifum_addr];
                                                    rh = ifum_lo[ifum_addr];

                                                    // ifu_decode_lh
                                                    ifu_sign       = lh[15];
                                                    ifu_length     = {6'd0, ~lh[11:10]};
                                                    ifu_type_pause = ~lh[5];
                                                    ifu_type_jump  = ~lh[4];
                                                    ifu_n          = lh[3:0];
                                                    if (lh[8] == 1'b0) begin
                                                        MemBase = {4'd0, MemBX, lh[7:6]};
                                                    end else begin
                                                        MemBase = 8'o034 + {6'd0, lh[7:6]};
                                                    end
                                                    RBase              = {7'd0, ~lh[9]};
                                                    ifu_dispatch_rbase = int'(RBase);

                                                    // ifu_decode_rh
                                                    ifu_packed_a = rh[10];
                                                    ifaddr       = {6'd0, ~rh[9:0]};

                                                    pc_after   = ifu_pcf + 16'd1;
                                                    op_faulted = 1'b0;
                                                    if (ifu_length >= 8'd2) begin
                                                        if (test_feed && ifu_test_count > 4'd0) begin
                                                            ifu_test_pop_byte_task(ifu_alpha);
                                                        end else begin
                                                            ifu_fetch_byte_task(pc_after, ifu_alpha, op_faulted);
                                                        end
                                                        pc_after = pc_after + 16'd1;
                                                    end
                                                    if (ifu_length >= 8'd3 && !op_faulted) begin
                                                        if (test_feed && ifu_test_count > 4'd0) begin
                                                            ifu_test_pop_byte_task(ifu_beta);
                                                        end else begin
                                                            ifu_fetch_byte_task(pc_after, ifu_beta, op_faulted);
                                                        end
                                                        pc_after = pc_after + 16'd1;
                                                    end

                                                    if (op_faulted) begin
                                                        np   = ifu_trap_addr_fn(6'o00, n_slot);
                                                        Link = real_PC + 16'd1;
                                                    end else begin
                                                        logic emu_or_ft;
                                                        logic [1:0] n_eff;
                                                        logic [15:0] tnia;
                                                        emu_or_ft = (ctask == 4'd0) || (ctask == 4'o17);
                                                        if (evc_b_enable && (evc_b_tasksall || emu_or_ft) &&
                                                            evc_b_event == 3'd2) begin
                                                            event_cnt_b = event_cnt_b + 16'd3;
                                                        end
                                                        evc_events |= EVC_EV_IFUJUMP;

                                                        if (cond_true) begin
                                                            // Hold pipeline
                                                        end else if (ifu_type_pause) begin
                                                            ifu_active = 1'b0;
                                                        end else if (ifu_type_jump) begin
                                                            logic signed [15:0] disp;
                                                            if (ifu_length == 8'd1) begin
                                                                logic [5:0] s6;
                                                                s6   = {ifu_sign, ifu_packed_a, ifu_n};
                                                                disp = {{10{s6[5]}}, s6};
                                                            end else begin
                                                                disp = {ifu_sign ? {8{ifu_alpha[7]}} : 8'd0, ifu_alpha};
                                                            end
                                                            ifu_pcf = ifu_pcx + 16'(disp);
                                                        end else begin
                                                            ifu_pcf = pc_after;
                                                        end

                                                        ifu_idcnt             = 8'd0;
                                                        ifu_save_stkp_pending = 1'b1;
                                                        ifu_dispatch_count    = ifu_dispatch_count + 64'd1;

                                                        n_eff = n_slot | {1'b0, cond_true};
                                                        tnia  = (ifaddr << 2) | {14'd0, n_eff};
                                                        if (resched_trap) begin
                                                            tnia = ifu_trap_addr_fn(6'o14, n_slot);
                                                        end
                                                        np   = {4'd0, tnia[11:0]};
                                                        Link = real_PC + 16'd1;
                                                    end
                                                end
                                            end
                                        end
                                    end else if (jcn[6] == 1'b1) begin
                                        // Return / Read TPC / Write TPC / Read IM / Write IM
                                        case (jcn[5:3])
                                            3'd0: begin // Return
                                                np = {4'd0, link_at_issue[11:0]} |
                                                     {15'd0, ff_cond_or} | dispatch_or;
                                                if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                                            end
                                            3'd4: begin // RdTPC <- B
                                                logic [3:0]  t_sel;
                                                logic [15:0] tpc;
                                                t_sel             = b[3:0];
                                                tpc               = (t_sel == ctask) ? real_PC : task_tpc[t_sel];
                                                Link              = real_PC + 16'd1;
                                                b_link_read       = ~tpc;
                                                b_link_read_valid = 1'b1;
                                                b_link_read_age   = 1'b0;
                                                np                = real_PC + 16'd1;
                                            end
                                            3'd5: begin // LdTPC <- B
                                                logic [3:0] t_sel;
                                                t_sel = b[3:0];
                                                task_dispatch[t_sel] = 16'd0;
                                                if (t_sel != ctask) task_tpc[t_sel] = Link;
                                                Link = real_PC + 16'd1;
                                                np   = real_PC + 16'd1;
                                            end
                                            3'd6: begin // Read IM
                                                logic [11:0] im_addr;
                                                logic [1:0]  sel;
                                                logic [15:0] lane;
                                                im_addr = link_at_issue[11:0];
                                                sel     = u.rstk[1:0];
                                                case (sel)
                                                    2'd0: lane = {7'd0, im_iw2[im_addr][15], im_iw0[im_addr][15:8]};
                                                    2'd1: lane = {8'd0, im_iw0[im_addr][7:0]};
                                                    2'd2: lane = {7'd0, im_iw1[im_addr][15], im_iw1[im_addr][14:7]};
                                                    2'd3: lane = {8'd0, im_iw1[im_addr][6:0], im_iw2[im_addr][14]};
                                                endcase
                                                b_link_read       = ~lane;
                                                b_link_read_valid = 1'b1;
                                                b_link_read_age   = 1'b0;
                                                Link              = real_PC + 16'd1;
                                                np                = real_PC + 16'd1;
                                            end
                                            3'd7: begin // Write IM
                                                logic [11:0] im_addr;
                                                logic        half_lh, secondary;
                                                im_addr   = link_at_issue[11:0];
                                                half_lh   = u.rstk[0];
                                                secondary = u.rstk[1];
                                                if (half_lh) begin
                                                    im_iw0[im_addr]     = b;
                                                    im_iw2[im_addr][15] = secondary;
                                                end else begin
                                                    im_iw1[im_addr]     = {secondary, b[15:1]};
                                                    im_iw2[im_addr][14] = b[0];
                                                end
                                                im_present[im_addr] = 1'b1;
                                                Link                = real_PC + 16'd1;
                                                np                  = real_PC + 16'd1;
                                            end
                                            default: begin
                                                halted      = 1'b1;
                                                halt_reason = 4'd4; // CPU_HALT_UNSUPPORTED_JCN
                                            end
                                        endcase
                                    end else begin
                                        halted      = 1'b1;
                                        halt_reason = 4'd4;
                                    end
                                end else begin
                                    // Conditional Jump/Call
                                    logic        cond_val;
                                    logic [15:0] page, offset;
                                    eval_branch_condition_task(u, low3, cond_val);
                                    offset = {10'd0, jcn[6:5], 1'b0, jcn[4:3], cond_val};
                                    page   = real_PC & ~(16'(CPU_PAGE_SIZE - 1));
                                    np     = page | offset | dispatch_or;
                                    if (jcn[4:3] == 2'd0 && !ff_loads_link_fn(u)) begin
                                        Link = real_PC + 16'd1;
                                    end
                                end
                            end
                        end

                        RBase = (ifu_dispatch_rbase >= 0) ? 8'(ifu_dispatch_rbase) : rbase_after_ff;

                        if (!halted) begin
                            // -------------------------------------------------
                            // 6. LC Writeback (apply_lc) using rbase_at_issue
                            // -------------------------------------------------
                            int rm_w;
                            rbase_next_instr = RBase;
                            RBase            = rbase_at_issue;
                            rm_w             = lc_write_address_fn(u, ctask, RBase,
                                                                   task_subtask[ctask], StkP);
                            case (u.lc)
                                3'd0: ;
                                3'd1: T = pd;
                                3'd2: begin
                                    if (rm_w >= 256) STK[rm_w & 255] = pd;
                                    else             RM[rm_w & 255]  = pd;
                                    T = md_at_issue;
                                end
                                3'd3: T = md_at_issue;
                                3'd4: begin
                                    if (rm_w >= 256) STK[rm_w & 255] = md_at_issue;
                                    else             RM[rm_w & 255]  = md_at_issue;
                                end
                                3'd5: begin
                                    T = ff_is_t_gets_md(u) ? md_at_issue : pd;
                                    if (rm_w >= 256) STK[rm_w & 255] = md_at_issue;
                                    else             RM[rm_w & 255]  = md_at_issue;
                                end
                                3'd6: begin
                                    if (rm_w >= 256) STK[rm_w & 255] = pd;
                                    else             RM[rm_w & 255]  = pd;
                                end
                                3'd7: begin
                                    T = pd;
                                    if (rm_w >= 256) STK[rm_w & 255] = pd;
                                    else             RM[rm_w & 255]  = pd;
                                end
                            endcase
                            RBase = rbase_next_instr;

                            // -------------------------------------------------
                            // 7. Post-Instruction StkP Update & FreezeBC Commit
                            // -------------------------------------------------
                            if (u.block && ctask == 4'd0) begin
                                logic [7:0] d8;
                                d8 = ff_is_dont_modify_stkp(u) ? 8'd0 : stk_signed_delta(u.rstk);
                                if (d8 != 8'd0 || (u.rstk[3] && !ff_is_dont_modify_stkp(u))) begin
                                    stk_und = stk_underflow;
                                    stk_ovf = stk_overflow;
                                    StkP    = {StkP[7:6], 6'(StkP[5:0] + d8[5:0])};
                                end
                            end
                            if (ifu_save_stkp_pending) begin
                                ifu_saved_stkp        = StkP;
                                ifu_save_stkp_pending = 1'b0;
                            end

                            if (!(ff_full_function_ok(u) && ff_fa(u.ff) == 2'd0 &&
                                  ff_fb(u.ff) == 3'd7 && ff_fc(u.ff) == 3'd6)) begin
                                alu_zero     = new_alu_zero;
                                alu_lt0      = new_alu_lt0;
                                alu_carry    = new_carry;
                                alu_overflow = new_ovf;
                            end

                            // -------------------------------------------------
                            // 8. End-of-Instruction Task Scheduling & Ticks
                            // -------------------------------------------------
                            prev_PC = real_PC;
                            if (from_im) junk_timer_tick_task();

                            task_schedule_task(np, (u.block && ctask != 4'd0), 1'b0, real_PC);

                            if (from_im) begin
                                cycles = cycles + 64'd1;
                                u_mem.memory_tick();
                            end

                            if (ifu_active && ifu_warmup > 8'd0) begin
                                ifu_warmup = ifu_warmup - 8'd1;
                            end
                        end
                    end
                end
            end
        end
    end

endmodule
/* verilator lint_on BLKSEQ */
