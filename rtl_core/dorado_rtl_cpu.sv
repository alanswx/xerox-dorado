// =============================================================================
// dorado_rtl_cpu.sv — Xerox Dorado 1-Clock-Per-Microinstruction CPU & IFU Core
// =============================================================================
// Derived from the schematic-audited C reference (dorado/src/cpu.c).
// Executes one Dorado microinstruction as a short sequence of fabric clocks:
// C_FETCH (IM read), C_EXEC (buses, ALU, FF), optional memory-engine and IFU
// states, C_FINISH (LC writeback, scheduling). step_done pulses once per
// microinstruction. The large memories (IM, IFUM, Map, cache data, storage)
// are single-port block RAMs so the core synthesizes for an FPGA.
// =============================================================================

/* verilator lint_off BLKSEQ */
module dorado_rtl_cpu
    import dorado_rtl_pkg::*;
#(
    parameter int          STORAGE_WORDS = 4 * 1024 * 1024,
    parameter logic [15:0] RESET_PC      = 16'd0,     // FPGA: the preloaded world's start
    parameter bit          EXT_STORAGE   = 1'b0,      // 1: storage is served outside (sto_*)
    parameter bit          READAHEAD     = 1'b1,      // C_FETCH may execute a read-ahead instruction
    parameter bit          PIPELINE      = 1'b0       // C_PIPE: an instruction's finish beside the next one's execute
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
    output logic [3:0]     dbg_halt_reason,

    // Sequencing
    output logic           step_done,  // one clock after each microinstruction
    output logic           cmp_done,   // ...and the byte references its dispatch owes are booked:
                                       // the lock-step's compare point (the next instruction is held until then)
    // The clock in which a microinstruction completes through C_FINISH or
    // C_SCHED: the I/O module ticks in it (dorado_rtl_top), so its results
    // are in place when the next one executes, even in the next clock.
    output logic           io_tick_early,
    input  logic           io_busy,    // I/O not ready for the next microinstruction
    input  logic           io_busy_tick, // ...its jobs still running: the retire (the next tick) waits

    // Loader assist port (dorado_rtl_assist): while ap_hold is high the CPU
    // stays between microinstructions, and services one ap_req at a time:
    // a word written to (or read from) storage at the mapped VA, optionally
    // also into a resident cache line, or a word written to STK. This is what
    // the cosimulation harness does by poking arrays; see dorado_rtl_assist.
    // ap_op selects the host-port operations (AP_* in dorado_rtl_pkg) that
    // stand for the C machine's host services (Cedar on the HPS); 0 is the
    // assist's own storage/STK access described above.
    input  logic           ap_hold,
    input  logic           ov_hold,      // no C_PIPE overlap: each instruction retires before the next executes
    input  logic           ap_req,
    input  logic [2:0]     ap_op,
    input  logic           ap_we,
    input  logic           ap_stk,
    input  logic           ap_cache,
    input  logic [27:0]    ap_va,
    input  logic [15:0]    ap_wd,
    output logic           ap_done,
    output logic [15:0]    ap_rd,
    output logic           cpu_between,  // in C_FETCH: assist conditions are current
    output logic           dbg_ifu_active,
    output logic [4:0]     dbg_st,

    // Storage munch port (dorado_rtl_storage): served by the internal block
    // RAM unless EXT_STORAGE, when the sto_* ports go out (MiSTer: DDR3).
    output logic           sto_req,
    output logic           sto_we,
    output logic [31:0]    sto_addr,
    output logic [15:0]    sto_mask,
    output logic [255:0]   sto_wline,
    input  logic           sto_ack_ext,
    input  logic [255:0]   sto_rline_ext
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
    (* ramstyle = "M10K" *) logic [15:0] im_iw0        [0:IM_SIZE-1];
    (* ramstyle = "M10K" *) logic [15:0] im_iw1        [0:IM_SIZE-1];
    (* ramstyle = "M10K" *) logic [15:0] im_iw2        [0:IM_SIZE-1];
    (* ramstyle = "M10K" *) logic        im_present    [0:IM_SIZE-1];
    (* ramstyle = "M10K" *) logic        im_breakpoint [0:IM_SIZE-1];

    (* ramstyle = "M10K" *) logic [15:0] ifum_lo       [0:IFUM_SIZE-1];
    (* ramstyle = "M10K" *) logic [15:0] ifum_hi       [0:IFUM_SIZE-1];
    (* ramstyle = "M10K" *) logic        ifum_present  [0:IFUM_SIZE-1];

    (* ramstyle = "M10K" *) logic [15:0] RM            [0:RM_SIZE-1];
    (* ramstyle = "M10K" *) logic [15:0] STK           [0:STK_SIZE-1];

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

    // Census of the events that decide a pipeline's throughput
    // (docs/rtl-core-speed-plan.md, Phase 0). Counted as instructions
    // retire; read by sim_hps (HPS_CENSUS). Nothing in the design reads
    // them, so synthesis drops them.
    logic [63:0] cen_retired  /* verilator public_flat_rd */;
    logic [63:0] cen_task0    /* verilator public_flat_rd */;   // retired by the emulator task
    logic [63:0] cen_ref_fs   /* verilator public_flat_rd */;   // Fetch, Store, LongFetch
    logic [63:0] cen_store    /* verilator public_flat_rd */;
    logic [63:0] cen_ref_other/* verilator public_flat_rd */;   // IO, Map, Flush, DummyRef, PreFetch
    logic [63:0] cen_miss     /* verilator public_flat_rd */;   // processor references that missed
    logic [63:0] cen_ifu_fetch/* verilator public_flat_rd */;
    logic [63:0] cen_ifu_miss /* verilator public_flat_rd */;
    logic [63:0] cen_md       /* verilator public_flat_rd */;   // instructions that read Md
    logic [63:0] cen_md_imm1  /* verilator public_flat_rd */;   // ...immediately (A/B/shifter), the instruction after the reference
    logic [63:0] cen_md_def1  /* verilator public_flat_rd */;   // ...deferred (T/RM/STK<-Md), the instruction after
    logic [63:0] cen_md_imm2  /* verilator public_flat_rd */;   // two instructions after
    logic [63:0] cen_md_def2  /* verilator public_flat_rd */;
    logic [63:0] cen_md_3p    /* verilator public_flat_rd */;   // three or more after
    logic [63:0] cen_ifujump  /* verilator public_flat_rd */;
    logic [63:0] cen_ifu_hl [0:6] /* verilator public_flat_rd */; // instructions since the last IFUJump: 1,2,3,4,5-8,9-16,17+
    logic [63:0] cen_ifu_len [0:3] /* verilator public_flat_rd */; // dispatched opcode length 1,2,3
    logic [63:0] cen_ifu_odd  /* verilator public_flat_rd */;   // dispatched opcode at an odd byte
    logic [63:0] cen_ifu_jump /* verilator public_flat_rd */;   // jump opcodes (the IFU restarts)
    logic [63:0] cen_ifu_notready /* verilator public_flat_rd */;
    logic [63:0] cen_pcf      /* verilator public_flat_rd */;   // PCF<-B: the IFU restarts
    logic [63:0] cen_ifu_fast /* verilator public_flat_rd */;   // dispatches from the prefetch buffer
    logic [63:0] cen_nf [0:7] /* verilator public_flat_rd */;   // ...not, because: 0 pending req, 1 pcf/insset, 2 entry, 3 word0, 4 byte1, 5 byte2, 6 hazard, 7 mcr/other
    logic [63:0] cen_bk_wait  /* verilator public_flat_rd */;   // clocks the sequencer waits for the byte references
    logic [63:0] cen_nonf [0:3] /* verilator public_flat_rd */; // the execute went to C_NEXT, not C_NF: 0 RM condition, 1 IFUJump not from the buffer, 2 LoadRam exit, 3 other JCN
    logic [63:0] cen_nospec [0:1] /* verilator public_flat_rd */; // the IM read-ahead was not the instruction chosen: 0 task switch, 1 another PC
    logic [63:0] cen_nopre [0:1] /* verilator public_flat_rd */;  // the RM read-ahead lost: 0 RBase/StkP moved, 1 C_NF's prediction differed from the execute's
    logic [1:0]  pred_why;                                       // comb: why pred_ok is 0
    logic [63:0] cen_ov [0:5] /* verilator public_flat_rd */;    // C_NF retires: 0 all, 1 next is the read-ahead, 2 no switch/plain JCN, 3 next plain, 4 next reads no RM/STK, 5 no I/O job ticked
    logic [63:0] cen_peek     /* verilator public_flat_rd */;   // peeks issued
    logic [63:0] cen_peek_retry /* verilator public_flat_rd */;
    logic [63:0] cen_peek_miss  /* verilator public_flat_rd */;
    logic [63:0] cen_cond     /* verilator public_flat_rd */;   // conditional branches
    logic [63:0] cen_imrw     /* verilator public_flat_rd */;   // Read IM / Write IM
    logic [63:0] cen_io       /* verilator public_flat_rd */;   // slow or fast I/O instructions
    logic [63:0] cen_block    /* verilator public_flat_rd */;
    logic [63:0] cen_switch   /* verilator public_flat_rd */;   // task switches
    logic [63:0] cen_hold     /* verilator public_flat_rd */;   // modeled holds (cycles that retire nothing)
    logic [63:0] cen_iobusy   /* verilator public_flat_rd */;   // clocks waiting for the I/O engines
    logic [63:0] cen_apwait   /* verilator public_flat_rd */;   // clocks on host-port operations
    logic [63:0] cen_last_ref_at [0:NUM_TASKS-1];               // retirement index of the task's last Fetch/Store/LongFetch
    logic [63:0] cen_last_ifuj_at;
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
    // Sequencer state
    // -------------------------------------------------------------------------
    typedef enum logic [4:0] {
        C_FETCH, C_DEC, C_EXEC, C_NF, C_NEXTRD, C_NEXT, C_SCHED,
        C_IFU_W, C_IFU_DECW, C_IFU_DEC, C_IFU_AW, C_IFU_BETA, C_IFU_BW, C_IFU_END,
        C_FINISH, C_APW, C_PIPE
    } cstate_t;
    cstate_t st;
    cstate_t m_ret;

    // Block-RAM read data
    logic [15:0] rm_q, stk_q;      // RM/STK at the address read last clock
    logic [15:0] rm_qr, stk_qr;    // the RAM outputs (rm_q/stk_q add the bypass)
    logic        rms_byp;          // last clock's read was at the address it wrote
    logic [15:0] rms_bypv;
    assign rm_q  = rms_byp ? rms_bypv : rm_qr;
    assign stk_q = rms_byp ? rms_bypv : stk_qr;
    logic        rmq_is_stk;
    logic [15:0] im_q0, im_q1, im_q2;
    logic        im_q_pres, im_q_brk;
    logic [15:0] ifum_q_hi, ifum_q_lo;
    logic        ifum_q_pres;

    // State of the microinstruction in flight, carried between states
    dorado_uinstr_t x_u;
    logic           x_from_im;
    logic [15:0]    x_a, x_b, x_alu, x_pd, x_np;
    logic [15:0]    x_md_at_issue;
    logic [7:0]     x_rbase_at_issue, x_rbase_after_ff;
    logic [4:0]     x_membase_at_issue;
    logic           x_stk_underflow, x_stk_overflow;
    logic           x_alu_live_carry, x_new_carry, x_new_ovf, x_new_alu_zero, x_new_alu_lt0;
    logic [15:0]    x_ifetch_id;
    logic [3:0]     x_fcnt_before;
    dm_ref_kind_t   x_kind;
    logic           x_ff_cond_or, x_cond_true, x_resched_trap, x_test_feed, x_op_faulted;
    logic [1:0]     x_n_slot;
    logic [15:0]    x_lh, x_rh, x_ifaddr, x_pc_after;
    logic           x_im_wr;
    logic [15:0]    x_im_wd0, x_im_wd1, x_im_wd2;
    logic           x_sched_block, x_sched_freeze;
    logic           x_rmor;        // an AP_RM_OR request is in C_APW
    logic           x_ifum_wr, x_ifum_hi;
    logic [9:0]     x_ifum_waddr;
    logic [15:0]    x_ifum_wd;


    // =========================================================================
    // The memory section is dorado_rtl_mem (u_mem, below): Map, cache, Pipe,
    // FaultInfo, the munch transfers and the storage port. The processor
    // keeps what the C model keeps beside the processor's step: the base
    // registers, MCR and ProcSRN (which the section reads), the DMux
    // readback, the MapBuf-busy count (tied to the instruction count), and
    // the per-task Md file.
    // =========================================================================
    localparam logic [15:0] PIPE5_MAPBUF_BUSY  = 16'h8000;
    localparam int MAP_IC_K   = 64;  // 64K map entries

    logic [27:0]    br [0:DM_BR_COUNT-1];
    logic [3:0]     proc_srn;
    logic [15:0]    mcr;
    logic [11:0]    dmux_addr;
    logic           dmux_pending;
    int             mapbuf_busy_slot;
    int             mapbuf_busy_cycles;
    logic           pipe_mapbuf_busy [0:DM_PIPE_DEPTH-1];

    // ---- the section's ports (u_mem) ----------------------------------------
    logic           mem_spec_req;          // comb: the address a reference this clock would use
    logic [27:0]    mem_spec_va;
    logic           mem_req;               // the reference issued, until accepted
    dm_ref_kind_t   mem_req_kind;
    logic [27:0]    mem_req_va;
    logic [15:0]    mem_req_b;
    logic [7:0]     mem_req_tioa;
    logic [3:0]     mem_req_task;
    logic [1:0]     mem_req_subtask;
    logic           mem_accepted, mem_idle, mem_done, mem_done_now;
    logic           mem_fin;               // comb: the reference issued has completed
    dm_fault_kind_t mem_res_fault;
    logic           mem_res_miss;
    logic [7:0]     mem_res_latency;
    logic [15:0]    md;                    // the section's Md, this clock's (the IFU takes it)
    logic [15:0]    md_r;                  // ...registered (the execute logic takes it)
    dm_fault_kind_t mem_res_fault_r;
    logic           mem_res_valid_r, mem_res_miss_r;
    logic [1:0]     mem_res_tag_r, mem_book_tag;
    logic [7:0]     mem_res_latency_r;
    logic           mem_book_now, mem_book_push, mem_book_is_map;
    logic [3:0]     mem_book_srn;
    logic           mem_ap_req, mem_ap_done;
    logic [15:0]    mem_ap_rd;
    logic           mem_fault_clear_stb, mem_cflags_stb;
    logic [15:0]    mem_cflags_val;
    logic [3:0]     fault_count;
    logic [15:0]    mem_fault_info, mem_config_word, mem_read_map, dbuf;
    logic [27:0]    mem_pipe_va;
    logic [15:0]    mem_pipe_rp, mem_pipe2, mem_pipe4, mem_pipe5;
    logic           mem_fastio_set;
    logic           fastio_stb_r;          // the strobe held for the I/O module's tick
    // who a request is from: the section echoes it with the result
    localparam logic [1:0] TAG_PROC = 2'd0;  // the processor's reference (mem_pref_task)
    localparam logic [1:0] TAG_IFU  = 2'd1;  // an IFU byte fetch the sequencer waits for (C_IFU_W)
    localparam logic [1:0] TAG_BK   = 2'd2;  // a dispatched byte's reference, booked after the fact
    logic [1:0]     mem_req_tag, mem_res_tag;
    logic [1:0]     mem_res_way;
    // the second port: the prefetch buffer's peeks
    logic           mem_spec2_req, mem_req2, mem_accepted2, mem_peek_done, mem_peek_hit, mem_peek_retry;
    // an IFU byte fetch launched while a request was still waiting: issued
    // from C_IFU_W once the section has taken that one
    logic           ifu_dl_valid;
    logic [27:0]    ifu_dl_va;
    logic [27:0]    mem_spec2_va, mem_req2_va;
    logic           mem_snoop_row_stb, mem_snoop_wr;
    logic [5:0]     mem_snoop_row, mem_ref_row;
    logic           mem_inflight;
    logic [1:0]     mem_inflight_tag;
    logic           cmp_wait;      // an instruction retired; its compare point is pending
    logic [9:0]     mem_snoop_wr_addr;
    logic [1:0]     mem_snoop_wr_way;

    // ---- the IFU prefetch buffer -----------------------------------------------
    // The words holding the bytes at ifu_pcf, pcf+1 and pcf+2 (the next
    // opcode and its operands) and the opcode's IFUM entry, fetched ahead
    // with peeks: lookups with no side effect, so the C model's references
    // (one per byte, with their Pipe, LRU and Md effects) are still made at
    // the dispatch, by the bookkeeping queue, in their order. A word is
    // dropped if its line or the word changes (the section's snoop), and
    // the buffer restarts when PCF or InsSet changes.
    logic [15:0]    pf_pcf;        // the opcode PC the buffer is for
    logic [1:0]     pf_insset;
    logic           pf_v0, pf_v1;  // the word at pcf, and the next word
    logic [15:0]    pf_w0, pf_w1;
    logic [27:0]    pf_va0, pf_va1;
    logic [5:0]     pf_row0, pf_row1;
    logic [1:0]     pf_way0, pf_way1;
    logic [3:0]     pf_word0, pf_word1;
    logic           pf_ev, pf_pres;        // the IFUM entry for the opcode byte
    logic [15:0]    pf_lh, pf_rh;
    logic [2:0]     pf_st;         // 0 need word0, 1 its peek out, 2 need word1, 3 its peek out, 4 complete, 5 not resident
    logic           pf_ifum_wait;  // IFUM port 2 was addressed last clock
    logic           pf_w0_new;     // word0 was captured this clock (its byte addresses IFUM next clock)
    logic           pf_issue_c;    // comb: a peek is issued this clock (its address read ahead)
    logic           pf_issue1_c;   // comb: word1's, as word0's answers
    logic           pf_restart;    // the buffer restarted this clock
    logic [2:0]     pf_st0;        // pf_st at the start of the clock
    logic [9:0]     ifum_addr2;
    logic [15:0]    ifum_q2_hi, ifum_q2_lo;
    logic           ifum_q2_pres;
    // what the buffer offers the IFUJump in C_NEXT (comb)
    logic           nx_fast;       // dispatch from the buffer this clock
    logic [15:0]    pf_lh_c, pf_rh_c;      // the entry: held, or arriving this clock
    logic           pf_ev_c, pf_pres_c;
    logic [15:0]    pf_w1_c;               // word1: held, or arriving this clock
    logic           pf_v1_c;
    logic [7:0]     pf_len;
    logic [7:0]     pf_b0, pf_b1, pf_b2;
    logic           pf_b1_ok, pf_b2_ok, pf_hazard;
    // the bookkeeping queue: the dispatched bytes' references still to book
    logic [1:0]     bk_n;
    logic [15:0]    bk_pc;
    logic           ifu_bk_busy;   // comb: the processor's next reference waits for them
    logic [4:0]     mem_dbg_st;
    // the munch port (sto_*): the section's, served by g_storage or outside
    logic           sto_ack;
    logic [255:0]   sto_rline;

    logic           m_byte_sel;   // IFU byte-fetch: which byte of the word
    logic           x_pref_pending; // a processor reference was issued: its C_PREF work is due when it completes
    // Read-ahead (C_NEXT -> C_FINISH -> C_FETCH executes): see pred_* below.
    logic           pred_ok;        // C_NEXT: the next PC is predicted (comb)
    logic [15:0]    pred_np;
    logic           x_pred_fresh;   // C_FINISH: IM holds the predicted instruction
    logic [15:0]    x_pred_np;
    logic [15:0]    x_pred_np_x;    // the execute clock's prediction (C_NF: its IM read)
    logic           rm_pre_ok;      // C_FETCH: ex_u and its RM/STK operand were read ahead
    logic [7:0]     fin_rbase;      // C_FINISH: RBase and StkP after it (comb)
    logic [7:0]     fin_stkp;
    logic [7:0]     fin_d8;
    logic [15:0]    pe_tnia, pe_tnia_r, pe_tnia_d;
    logic           pe_flt;
    logic [15:0]    pe_trap0;
    logic           pe_cond_f;
    logic           exec_now;       // the execute runs this clock (the prediction is for ex_u)
    logic [15:0]    cnt_p;          // Cnt as the branch condition sees it
    logic           atten_p;
    logic [1:0]     pe_neff_f;
    logic [15:0]    pe_trap14;
    logic           fexec;          // C_FETCH executes the read-ahead instruction this clock
    logic           do_exec;        // exec_task runs this clock
    cstate_t        st0;            // the state at the start of the clock (the case runs after the execute)
    logic           ifu_bk_gate;    // comb: this execute must wait for the byte references
    logic           touches_io_c;   // comb: the instruction in cur_u reads or strobes I/O state
    logic [7:0]     fx_stkp;        // StkP and subtask the condition read (below) used
    logic [1:0]     fx_sub;

    // C_PIPE (PIPELINE): in one clock the finish of x_u (W), the execute of
    // ex_u (X), the RM/STK read of the instruction in cur_u (D) and the IM
    // read of the one after it (F) -- HM 2.3's overlap. See the C_PIPE item.
    logic           p_w /* verilator public_flat_rd */, p_x /* verilator public_flat_rd */, p_d /* verilator public_flat_rd */;   // x_u awaits its finish; ex_u (operand read) its execute; cur_u is the instruction after ex_u
    logic [15:0]    p_pc_x, p_pc_d;  // the PCs of ex_u and cur_u
    int             p_raddr;         // the RM/STK address ex_u's operand was read at
    logic           pent;            // comb: C_NF enters C_PIPE (IM is read at f_np)
    logic           ovx;             // comb: C_PIPE executes ex_u this clock
    logic           pfwd;            // comb: C_PIPE finishes x_u this clock (its results are forwarded to the execute)
    logic           f_ok;            // comb: f_np is the address of the instruction after cur_u
    logic [8:0]     f_rb;            // comb: cur_u's dispatch RBase (an IFUJump)
    logic [15:0]    f_np;
    logic           d_ok;            // comb: d_raddr is cur_u's operand address after ex_u
    int             d_raddr;
    logic           sw_maybe;        // comb: the scheduler may switch tasks after x_u
    logic [25:0]    ij_w, ij_x, ij_c;   // comb: ifujump_fn of x_u, ex_u, cur_u
    logic           w_disp;          // comb: x_u's finish (beside the execute) dispatches
    logic [7:0]     p_rb_x;          // the RBase ex_u's operand was read at (checked after a dispatch)
    logic [8:0]     p_rb_d;          // cur_u is an IFUJump whose dispatch F predicted: {RBase changes, RBase}
    logic [7:0]     d_rb;            // comb: the RBase cur_u's is read at
    logic [7:0]     d_sp;            // ...and the StkP
    logic           pexec_now;       // exec_task runs for C_PIPE
    logic           p_wonly;         // (unused)
    logic           x_elig, x_host, x_ready;   // comb: see ovx
    logic           p_bub;           // comb: a bubble: x_u's finish runs alone, ex_u and cur_u held
    logic [7:0]     p_sp_x;          // the StkP ex_u's operand was read at
    logic           x_disp_v;        // (PIPELINE) x_u is an IFUJump the buffer could dispatch when it executed...
    logic [7:0]     x_disp_rb;       // ...and the RBase that dispatch leaves
    logic           p_fwd;           // x_u's finish writes the register ex_u's operand was read from
    logic           p_stall;         // comb: x_u's finish waits (a reference, the I/O jobs, the byte references): nothing moves
    logic [15:0]    T_x, Link_x;     // what the execute sees (forwarded from the finish in C_PIPE)
    logic           carry_x, zero_x, lt0_x, ovf_x;
    logic [15:0]    pc_p;            // the PC of the instruction the prediction is for
    logic [3:0]     ctask_x;         // (PIPELINE) the execute's view: ctask, cycles, StkP, MemBase
    dorado_uinstr_t ex_u_q;          // ...and the registers the sequencer's case may change before it
    logic           ex_pres_q, ex_brk_q, blr_valid_q, blr_age_q;
    logic [7:0]     idcnt_q;
    logic [15:0]    Cnt_q;
    logic [63:0]    cycles_x;
    logic [7:0]     sp_wx, membase_x;
    int             dr_raddr;        // comb: a drain re-reads ex_u's operand here, at...
    logic [7:0]     dr_rb, dr_sp;    // ...this RBase and StkP (C_FETCH then executes it)
    // the processor as the finish left it, before the execute beside it
    // (the lock-step compares these when snap_valid)
    logic           snap_valid   /* verilator public_flat_rd */;
    logic [15:0]    snap_real_PC /* verilator public_flat_rd */;
    logic [15:0]    snap_T       /* verilator public_flat_rd */;
    logic [15:0]    snap_Q       /* verilator public_flat_rd */;
    logic [15:0]    snap_Link    /* verilator public_flat_rd */;
    logic [15:0]    snap_Cnt     /* verilator public_flat_rd */;
    logic [15:0]    snap_ShC     /* verilator public_flat_rd */;
    logic [15:0]    snap_mcr     /* verilator public_flat_rd */;
    logic [15:0]    snap_ready   /* verilator public_flat_rd */;
    logic [7:0]     snap_StkP    /* verilator public_flat_rd */;
    logic [7:0]     snap_RBase   /* verilator public_flat_rd */;
    logic [7:0]     snap_MemBase /* verilator public_flat_rd */;
    logic [1:0]     snap_MemBX   /* verilator public_flat_rd */;
    logic [3:0]     snap_ctask   /* verilator public_flat_rd */;
    logic [3:0]     snap_proc_srn /* verilator public_flat_rd */;
    logic [3:0]     snap_flags   /* verilator public_flat_rd */;   // zero, lt0, carry, overflow
    logic [27:0]    snap_br [0:31] /* verilator public_flat_rd */;
    logic [63:0]    cen_pdrain [0:11] /* verilator public_flat_rd */; // why a drain: 0 operand (d_ok), 1 I/O, 2 Id, 3 RM condition, 4 IFUJump/special, 5 section state/counters, 6 reference outstanding, 7 byte refs, 8 I/O jobs, 9 switch, 10 host, 11 other
    logic [63:0]    cen_pnof [0:4] /* verilator public_flat_rd */;    // why no fetch after: 0 conditional jump, 1 FF condition, 2 IFUJump, 3 Return Link, 4 other
    logic [63:0]    cen_pipe [0:9] /* verilator public_flat_rd */;  // C_PIPE: 0 clocks, 1 executes, 2 finish waits, 3 executes handed to the FSM (no fetch after), 4 entries, 5 entries refused, 6 execute left (hold, stack error), 7 executes beside a finish, 8 drains, 9 dispatches the pipeline followed


    // -------------------------------------------------------------------------
    // MCR tests the processor makes (the section has its own, on the same mcr)
    // -------------------------------------------------------------------------
    function automatic logic mcr_is_initial_nowake();
        return (mcr == 16'hFEE7);
    endfunction
    function automatic logic mcr_disbr();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[8];
    endfunction
    function automatic logic mcr_dishold();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[6];
    endfunction
    function automatic logic mcr_nowake();
        return mcr[0];
    endfunction

    // -------------------------------------------------------------------------
    // State Mutation Tasks
    // -------------------------------------------------------------------------
    task automatic reset_memory();
        x_pref_pending = 1'b0;
        x_pred_fresh = 1'b0;
        x_pred_np = 16'd0; x_pred_np_x = 16'd0;
        rm_pre_ok = 1'b0;
        for (int i = 0; i < DM_BR_COUNT; i++) br[i] = 28'd0;
        for (int i = 0; i < DM_PIPE_DEPTH; i++) pipe_mapbuf_busy[i] = 1'b0;
        proc_srn            = 4'd0;
        mcr                 = 16'd0;
        dmux_addr           = 12'd0;
        dmux_pending        = 1'b0;
        mapbuf_busy_slot    = -1;
        mapbuf_busy_cycles  = 0;
        fastio_stb_r        = 1'b0;
        mem_req             = 1'b0;
        mem_req_kind = DM_REF_NONE; mem_req_va = 28'd0; mem_req_b = 16'd0; mem_req_tioa = 8'd0;
        mem_req_task = 4'd0; mem_req_subtask = 2'd0;
        mem_ap_req          = 1'b0;
        mem_req_tag = TAG_PROC; mem_req2 = 1'b0; mem_req2_va = 28'd0;
        bk_n = 2'd0; bk_pc = 16'd0; ifu_dl_valid = 1'b0; ifu_dl_va = 28'd0; cmp_wait = 1'b0;
        pf_pcf = 16'hFFFF; pf_insset = 2'd0; pf_v0 = 1'b0; pf_v1 = 1'b0; pf_ev = 1'b0; pf_pres = 1'b0;
        pf_w0 = 16'd0; pf_w1 = 16'd0; pf_va0 = 28'd0; pf_va1 = 28'd0; pf_row0 = 6'd0; pf_row1 = 6'd0;
        pf_way0 = 2'd0; pf_way1 = 2'd0; pf_word0 = 4'd0; pf_word1 = 4'd0; pf_lh = 16'd0; pf_rh = 16'd0;
        pf_st = 3'd0; pf_ifum_wait = 1'b0; pf_w0_new = 1'b0; pf_restart = 1'b0; pf_st0 = 3'd0;
        mem_fault_clear_stb = 1'b0; mem_cflags_stb = 1'b0; mem_cflags_val = 16'd0;
        m_byte_sel          = 1'b0;
`ifdef DORADO_CHECKPOINT
        // A checkpoint build powers up in a saved machine state (reset_cpu
        // runs first, so this covers both): sim_cedar.cpp CEDAR_DUMP_INIT.
`include "checkpoint_cpu.svh"
`endif
    endtask


    task automatic dmux_strobe(input logic [15:0] b_val);
        if (!dmux_pending) begin
            dmux_addr    = b_val[11:0];
            dmux_pending = 1'b1;
        end
    endtask

    task automatic dmux_read(output logic [15:0] val);
        dmux_pending = 1'b0;
        if (dmux_ext_handled) begin
            val = dmux_ext_val;
        end else begin
            case (dmux_addr)
                12'o1510: val = (MAP_IC_K == 16)  ? 16'h8000 : 16'h0000;
                12'o1511: val = (MAP_IC_K == 64)  ? 16'h8000 : 16'h0000;
                12'o1512: val = (MAP_IC_K == 256) ? 16'h8000 : 16'h0000;
                default:  val = 16'h0000;
            endcase
        end
    endtask


    task automatic memory_tick();
        if (mapbuf_busy_cycles > 0) begin
            mapbuf_busy_cycles = mapbuf_busy_cycles - 1;
            if (mapbuf_busy_cycles == 0 && mapbuf_busy_slot >= 0) begin
                pipe_mapbuf_busy[mapbuf_busy_slot & 15] = 1'b0;
                mapbuf_busy_slot = -1;
            end
        end
    endtask


    // -------------------------------------------------------------------------
    // Combinational Instruction Fetch & Shifter / ALU Instantiations
    // -------------------------------------------------------------------------
    dorado_uinstr_t cur_u;         // decoded from the IM read data (RM/STK read address)
    dorado_uinstr_t ex_u;          // the same, registered for C_EXEC (ALU, shifter, FF)
    logic [15:0]    cur_task_md;
    logic [15:0]    cur_rm_stk_read;
    logic [15:0]    cur_id_peek;
    // The IFU operand at ifu_idcnt and at ifu_idcnt+1, as registers (see the
    // top of the sequencer), so the operand selection is not in the path
    // from ifu_idcnt to Pd.
    logic [15:0]    id_r0, id_r1;
    logic [15:0]    id_x0, id_x1;   // ...as the execute sees them (C_PIPE: a dispatch beside it)
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

    // Current task's Md
    function automatic logic [15:0] get_task_md();
        if (task_md_valid[ctask]) return task_md[ctask];
        return md_r;                     // never a word completing this clock
    endfunction

    // Pure helper for IFU operand byte at index cnt
    function automatic logic [15:0] ifu_id_at_fn(input logic [7:0] cnt);
        return ifu_id_gen_fn(cnt, ifu_length, ifu_n, ifu_type_jump, ifu_packed_a, ifu_sign, ifu_alpha, ifu_beta);
    endfunction

    // Id for an opcode's operands (as ifu_id_at_fn, from given IFU fields)
    function automatic logic [15:0] ifu_id_gen_fn(input logic [7:0] cnt, input logic [7:0] len,
                                                  input logic [3:0] ifu_n, input logic ifu_type_jump,
                                                  input logic ifu_packed_a, input logic ifu_sign,
                                                  input logic [7:0] ifu_alpha, input logic [7:0] ifu_beta);
        logic       n_supplied, n_first;
        int         op_idx;
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
        return {8'd0, len};
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
        if (bb_present && bb_ss_inject) cur_u = bb_mir_uinstr;
        else                            cur_u = unpack_uinstr(im_q0, im_q1, im_q2);

        cur_task_md     = get_task_md();
        ridx            = rm_address_fn(cur_u, ctask, RBase, task_subtask[ctask], StkP);
        cur_id_peek     = id_x0;
    end
    always_comb begin
        cur_rm_stk_read = rmq_is_stk ? stk_q : rm_q;   // read in C_DEC
        // C_PIPE: the finish beside the execute writes the operand's register
        // (p_fwd: the address compare, made the clock before)
        if (pfwd && p_fwd) cur_rm_stk_read = rms_wd;
    end

    // ex_u holds the instruction C_EXEC executes: captured by the sequencer
    // while it is read (C_FETCH, C_DEC), so the IM read data does not feed
    // the ALU directly.
    logic ex_pres, ex_brk;

    dorado_rtl_shifter u_shifter (
        .u             (ex_u),
        .t_in          (T_x),
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

    // The instruction reads or strobes I/O state the engines may be changing:
    // Input<-/InputNoPE/Output<-B (HM Table 11a FA=0 FB=3 FC=2,3,6) and the
    // long-call Output form, TIOA<-B (FA=1 FB=5 FC=2) and TIOA<-FF (FA=2 FB=4),
    // a branch on IOAtten' (condition 6, by JCN or by FF), a fast-I/O
    // reference (IOFetch<-/IOStore<-). Every other execute may overlap the
    // engines; the retire, which ticks them, waits.
    function automatic logic uinstr_touches_io_fn(input dorado_uinstr_t u, input logic io_task);
        logic [1:0]  fa;
        logic [2:0]  fb, fc;
        dm_ref_kind_t kind;
        fa = ff_fa(u.ff); fb = ff_fb(u.ff); fc = ff_fc(u.ff);
        if (fa == 2'd0 && fb == 3'd3 && (fc == 3'd2 || fc == 3'd3 || fc == 3'd6)) return 1'b1;
        if (fa == 2'd1 && fb == 3'd5 && fc == 3'd2) return 1'b1;
        if (fa == 2'd2 && fb == 3'd4) return 1'b1;
        if (u.asel == 3'd4 && u.bsel == 3'd1 && u.lc == 3'd6 && u.aluf == 4'o13 &&
            u.ff == 8'o327 && u.jcn == 8'o017) return 1'b1;
        if (!u.jcn[7] && u.jcn[7:4] != 4'd0 && u.jcn[2:0] == 3'd6) return 1'b1;
        if (u.bsel < 3'd4 && fa == 2'd0 && fb == 3'd6 && fc == 3'd6) return 1'b1;
        if (u.asel <= 3'd3 && !jcn_is_write_im_fn(u)) begin
            kind = decode_ref_kind_fn(u, io_task);
            if (kind == DM_REF_IOFETCH || kind == DM_REF_IOSTORE) return 1'b1;
        end
        return 1'b0;
    endfunction

    // The instruction takes an Id operand: A<-Id (HM Table 8a), the FF
    // A-source Id form (ASEL 2/3), or TisId/RisId.
    function automatic logic uinstr_reads_id_fn(input dorado_uinstr_t u);
        return (u.asel == 3'd5) ||
               ((u.asel == 3'd2 || u.asel == 3'd3) && ff_decode_ok(u) && u.ff[7:6] == 2'd1) ||
               ff_is_t_is_id(u) || ff_is_r_is_id(u);
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
        logic [3:0] r;
        r = 4'd0;
        for (int i = 1; i < 16; i++) begin
            if (avail_in[i]) r = 4'(i);
        end
        return r;
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
        if (ff_full_function_ok(ex_u) && ff_fa(ex_u.ff) == 2'd1) begin
            if (ff_fb(ex_u.ff) == 3'd6) begin
                logic [3:0]  psrn;
                logic [27:0] pva;
                psrn         = proc_srn;
                pva          = mem_pipe_va;             // the section's Pipe entry at ProcSRN
                b_overridden = 1'b1;
                case (ff_fc(ex_u.ff))
                    3'd0: b_val = ~mem_fault_info;
                    3'd1: b_val = {4'd0, pva[27:16]};
                    3'd2: b_val = pva[15:0];
                    3'd3: b_val = mem_pipe2;
                    3'd4: b_val = ~mem_pipe_rp;
                    3'd5: b_val = mem_pipe4;
                    3'd6: b_val = ~mem_config_word;
                    3'd7: b_val = mem_pipe5 | (pipe_mapbuf_busy[psrn] ? PIPE5_MAPBUF_BUSY : 16'd0);
                endcase
            end else if (ff_fb(ex_u.ff) == 3'd7) begin
                logic [9:0] ifum_addr;
                ifum_addr    = {ifu_insset, ifu_opcode};
                b_overridden = 1'b1;
                case (ff_fc(ex_u.ff))
                    3'd0: b_val = ~ifu_pcx;
                    3'd1: b_val = ~(gen_io_mode ? event_cnt_b : event_cnt_a);
                    3'd2: b_val = ~ifum_q_hi;
                    3'd3: b_val = ~({ifu_idcnt[2:0], ifu_insset, ifum_q_lo[10:0]});
                    3'd4: b_val = ~event_cnt_b;
                    3'd5: b_val = dbuf;
                    3'd6: b_val = ~cpreg;
                    3'd7: b_val = b_link_read_valid ? b_link_read : Link_x;
                endcase
            end
        end

        // Standard BSEL if not overridden by FF
        if (!b_overridden) begin
            if (ex_u.asel == 3'd7 && ex_u.bsel[2]) begin
                b_val = Q;
            end else begin
                case (ex_u.bsel)
                    3'd0: b_val = cur_task_md;
                    3'd1: b_val = cur_rm_stk_read;
                    3'd2: b_val = T_x;
                    3'd3: b_val = Q;
                    3'd4: b_val = {8'h00, ex_u.ff};
                    3'd5: b_val = {8'hFF, ex_u.ff};
                    3'd6: b_val = {ex_u.ff, 8'h00};
                    3'd7: b_val = {ex_u.ff, 8'hFF};
                endcase
            end
        end

        // 2. A-bus primary selection (HM Table 8a/8b)
        idcnt_after_a = ifu_idcnt;
        case (ex_u.asel)
            3'd0, 3'd1, 3'd4: a_val = cur_rm_stk_read;
            3'd2, 3'd3: begin
                if (!ff_decode_ok(ex_u)) begin
                    a_val = T_x;
                end else begin
                    case (ex_u.ff[7:6])
                        2'd0: a_val = cur_task_md;
                        2'd1: begin
                            a_val         = id_x0;
                            idcnt_after_a = ifu_idcnt + 8'd1;
                        end
                        2'd2: a_val = Q;
                        2'd3: a_val = T_x;
                    endcase
                end
            end
            3'd5: begin
                a_val         = id_x0;
                idcnt_after_a = ifu_idcnt + 8'd1;
            end
            3'd6: a_val = T_x;
            3'd7: a_val = ~shifter_unmasked_lo16;
        endcase

        // 3. FF A-source override (FA=0, FB=0..2)
        a_overridden       = 1'b0;
        a_is_rmstk_default = 1'b0;
        if (ff_decode_ok(ex_u) && ex_u.asel != 3'd7) begin
            logic [5:0] f6;
            logic       f6_valid;
            f6       = ex_u.ff[5:0];
            f6_valid = (ex_u.asel <= 3'd1) ||
                       (ff_full_function_ok(ex_u) && ff_fa(ex_u.ff) == 2'd0);
            if (f6_valid) begin
                if (f6[5:3] <= 3'd1) begin
                    a_val        = {12'd0, ex_u.ff[3:0]};
                    a_overridden = 1'b1;
                end else if (f6[5:3] == 3'd2) begin
                    case (f6[2:0])
                        3'd1: begin a_val = T_x;           a_overridden = 1'b1; end
                        3'd2: begin a_val = cur_task_md; a_overridden = 1'b1; end
                        3'd3: begin a_val = Q;           a_overridden = 1'b1; end
                        default: ;
                    endcase
                end
            end
        end
        if (!a_overridden && (ex_u.asel == 3'd0 || ex_u.asel == 3'd1)) begin
            a_is_rmstk_default = 1'b1;
        end

        // 4. TisId / RisId bus substitutions
        id_peek_post_a = (idcnt_after_a == ifu_idcnt) ? id_x0 : id_x1;
        if (ff_is_t_is_id(ex_u)) begin
            if (ex_u.bsel == 3'd2) b_val = id_peek_post_a;
            if (ex_u.asel == 3'd6) a_val = id_peek_post_a;
        end else if (ff_is_r_is_id(ex_u)) begin
            if (ex_u.bsel == 3'd1) b_val = id_peek_post_a;
            if (ex_u.asel == 3'd4 || a_is_rmstk_default) a_val = id_peek_post_a;
        end

        // 5. ALUFM lookup & Carry modifiers (XorCarry, XorSavedCarry, Carry20)
        aluf_idx         = (ex_u.asel == 3'd7) ? (4'd14 + {3'd0, ex_u.aluf[0]}) : ex_u.aluf;
        comb_alufm_entry = alufm_present[aluf_idx] ? alufm[aluf_idx] : 6'd0;
        comb_carry20     = ff_is_table11a(ex_u, 8'o026, 8'o077);

        if (ff_is_table11a(ex_u, 8'o025, 8'o077)) begin
            // XorSavedCarry
            comb_alufm_entry[5] ^= carry_x;
        end else if (ff_is_table11a(ex_u, 8'o024, 8'o077)) begin
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
        real_PC               = RESET_PC;
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
        cen_retired = 64'd0; cen_task0 = 64'd0; cen_ref_fs = 64'd0; cen_store = 64'd0;
        cen_ref_other = 64'd0; cen_miss = 64'd0; cen_ifu_fetch = 64'd0; cen_ifu_miss = 64'd0;
        cen_md = 64'd0; cen_md_imm1 = 64'd0; cen_md_def1 = 64'd0; cen_md_imm2 = 64'd0;
        cen_md_def2 = 64'd0; cen_md_3p = 64'd0; cen_ifujump = 64'd0; cen_ifu_odd = 64'd0;
        cen_ifu_jump = 64'd0; cen_ifu_notready = 64'd0; cen_cond = 64'd0; cen_imrw = 64'd0;
        cen_io = 64'd0; cen_block = 64'd0; cen_pcf = 64'd0; cen_ifu_fast = 64'd0; cen_bk_wait = 64'd0;
        cen_peek = 64'd0; cen_peek_retry = 64'd0; cen_peek_miss = 64'd0;
        for (int i = 0; i < 8; i++) cen_nf[i] = 64'd0; cen_switch = 64'd0; cen_hold = 64'd0;
        for (int i = 0; i < 4; i++) cen_nonf[i] = 64'd0;
        for (int i = 0; i < 6; i++) cen_ov[i] = 64'd0;
        for (int i = 0; i < 10; i++) cen_pipe[i] = 64'd0;
        for (int i = 0; i < 12; i++) cen_pdrain[i] = 64'd0;
        for (int i = 0; i < 5; i++) cen_pnof[i] = 64'd0;
        for (int i = 0; i < 2; i++) begin cen_nospec[i] = 64'd0; cen_nopre[i] = 64'd0; end
        cen_iobusy = 64'd0; cen_apwait = 64'd0; cen_last_ifuj_at = 64'd0;
        for (int i = 0; i < 7; i++) cen_ifu_hl[i] = 64'd0;
        for (int i = 0; i < 4; i++) cen_ifu_len[i] = 64'd0;
        for (int i = 0; i < NUM_TASKS; i++) cen_last_ref_at[i] = 64'd0;
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
        if (junk_tw_enabled) begin
            if (junk_tw_countdown == 16'd0) junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
            junk_tw_countdown = junk_tw_countdown - 16'd1;
            if (junk_tw_countdown == 16'd0) begin
                wakeup_pending[DORADO_JUNK_TASK] = 1'b1;
                junk_tw_countdown = DORADO_JUNK_TICK_CYCLES;
            end
        end
    endtask

    task automatic ifu_test_tick_task();
        logic loads_fg;
        if (ifu_test_pending) begin
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
        end
    endtask

    task automatic ifu_test_pop_byte_task(output logic [7:0] out_byte);
        if (ifu_test_count == 4'd0) begin
            out_byte = 8'd0;
        end else begin
            out_byte = ifu_test_queue[0];
            ifu_test_count = ifu_test_count - 4'd1;
            for (int i = 0; i < 7; i++) begin
                if (i < int'(ifu_test_count)) ifu_test_queue[i] = ifu_test_queue[i + 1];
            end
        end
    endtask

    // C_NEXT reads RM/STK (read in C_NEXTRD) only for the branch conditions
    // R<0 and R odd (HM Table 13, conditions 4 and 5) and for the LoadRam
    // End-item exit; every other microinstruction skips C_NEXTRD. A
    // conservative superset of next_pc's uses: any JCN conditional or FF
    // branch condition naming 4 or 5.
    function automatic logic next_reads_rm_fn(input dorado_uinstr_t u);
        logic jcn_cond, ff_cond;
        jcn_cond = (u.jcn[7] == 1'b0) && (u.jcn[7:4] != 4'd0) &&
                   (u.jcn[2:1] == 2'b10);
        ff_cond  = (u.bsel < 3'd4) && (u.ff[2:1] == 2'b10);
        return jcn_cond || ff_cond || (u.jcn == 8'o045 && u.ff == 8'o147);
    endfunction

    task automatic eval_branch_condition_task(
        input  dorado_uinstr_t u,
        input  logic [2:0]     cond,
        output logic           out_r
    );
        int          rm_a;
        logic [15:0] rv;
        rm_a = rm_address_fn(u, ctask, RBase, task_subtask[ctask], StkP);
        rv   = rmq_is_stk ? stk_q : rm_q;                // read in C_NEXTRD
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

        chosen_pc = next_pc_in;
        if (!freeze) begin
            if (block_in_non_emul) begin
                ready[ctask]          = 1'b0;
                wakeup_pending[ctask] = 1'b0;
            end

            if (tasking_on) begin
                avail         = ready | wakeup_pending;
                bnt           = task_bnt_fn(avail);
                should_switch = (bnt > ctask) || block_in_non_emul;

                if (block_in_non_emul && bnt == ctask) begin
                    bnt = task_bnt_fn((ready | wakeup_pending) & ~(16'd1 << ctask));
                end

                if (should_switch && bnt != ctask) begin
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
                    cen_switch               = cen_switch + 64'd1;
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
                end
            end
        end
    endtask

    // Launch a memory reference: the M_* states run it and return to `ret`.
    // A reference: presented to the memory section (mem_req_*, held until
    // accepted). Its address was read ahead in this clock (mem_spec_*, the
    // combinational copy of this VA), so a hit completes next clock.
    task automatic mem_launch(
        input dm_ref_kind_t kind,
        input logic [27:0]  va,
        input logic [15:0]  b_val,
        input logic [7:0]   tioa_val,
        input logic [3:0]   task_id,
        input logic [1:0]   subtask_id,
        input logic [1:0]   tag
    );
        mem_req         = 1'b1;
        mem_req_tag     = tag;
        mem_req_kind    = kind;
        mem_req_va      = va;
        mem_req_b       = b_val;
        mem_req_tioa    = tioa_val;
        mem_req_task    = task_id;
        mem_req_subtask = subtask_id;
    endtask

    // When a processor memory reference completes (formerly the state
    // C_PREF): Md for the task, and the fault wakeup.
    task automatic mem_pref_task();
        logic recorded_fault;
        recorded_fault = (mem_res_fault_r != DM_FAULT_NONE) ||
                         (fault_count != x_fcnt_before);
        evc_events |= EVC_EV_PROCREF;
        if (mem_res_miss_r) evc_events |= EVC_EV_MISS;

        if (mem_res_fault_r == DM_FAULT_NONE &&
            (x_kind == DM_REF_FETCH || x_kind == DM_REF_IFETCH ||
             x_kind == DM_REF_LONGFETCH)) begin
            task_md[ctask]       = md_r;
            task_md_valid[ctask] = 1'b1;
            task_md_ready[ctask] = cycles + 64'(mem_res_latency_r);
        end

        if (recorded_fault && x_kind != DM_REF_IFETCH && !mcr_nowake()) begin
            wakeup_pending[15] = 1'b1;
        end
    endtask

    // IFU byte fetch at byte PC `pc` (HM section 6: opcode bytes come from the
    // code base register, BR 37 octal).
    function automatic logic [27:0] ifu_va_fn(input logic [15:0] pc);
        return (br[31] + {13'd0, pc[15:1]}) & 28'h0FFFFFFF;
    endfunction
    // An IFU byte fetch: the reference goes to the section and the sequencer
    // waits in C_IFU_W, then continues in ret (for the opcode, C_IFU_DEC:
    // C_IFU_W reads IFUM for the byte as it arrives). The launching state
    // presented the address ahead (mem_spec_*).
    task automatic ifu_launch_fetch(input logic [15:0] pc, input cstate_t ret);
        m_byte_sel = pc[0];
        cen_ifu_fetch = cen_ifu_fetch + 64'd1;
        if (mem_req && !mem_accepted) begin
            // a request is still waiting (a dispatched byte's that missed):
            // C_IFU_W issues this one once it is taken
            ifu_dl_valid = 1'b1;
            ifu_dl_va    = ifu_va_fn(pc);
        end else begin
            mem_launch(DM_REF_IFETCH, ifu_va_fn(pc), 16'd0, 8'd0, 4'd0, 2'd0, TAG_IFU);
        end
        m_ret = ret;
        st    = C_IFU_W;
    endtask

    function automatic logic [7:0] byte_at_fn(input logic [15:0] w, input logic sel);
        logic high_byte;
        high_byte = (ifu_insset < 2'd2) ? (sel == 1'b0) : (sel == 1'b1);
        return high_byte ? w[15:8] : w[7:0];
    endfunction
    function automatic logic [7:0] ifu_byte_of(input logic [15:0] w);
        logic high_byte;
        high_byte = (ifu_insset < 2'd2) ? (m_byte_sel == 1'b0) : (m_byte_sel == 1'b1);
        return high_byte ? w[15:8] : w[7:0];
    endfunction
    function automatic logic [7:0] ifu_fetched_byte();
        return ifu_byte_of(md);
    endfunction


    // =========================================================================
    // Power-up contents for the FPGA (DORADO_INIT): the state the cosimulation
    // loads from the C model before Suite 6 -- the world's IM, IFUM and RM,
    // the standard ALUFM, the Map (tools: RTL_DUMP_INIT in sim_cosim.cpp).
    // Storage and cache data power up zero.
    // =========================================================================
`ifdef DORADO_INIT
    initial begin
        $readmemh("init/im_iw0.hex", im_iw0);
        $readmemh("init/im_iw1.hex", im_iw1);
        $readmemh("init/im_iw2.hex", im_iw2);
        $readmemh("init/im_present.hex", im_present);
        for (int i = 0; i < IM_SIZE; i++) im_breakpoint[i] = 1'b0;
        $readmemh("init/ifum_lo.hex", ifum_lo);
        $readmemh("init/ifum_hi.hex", ifum_hi);
        $readmemh("init/ifum_present.hex", ifum_present);
        $readmemh("init/rm.hex", RM);
        $readmemh("init/stk.hex", STK);
        $readmemh("init/alufm.hex", alufm);
        $readmemh("init/alufm_present.hex", alufm_present);
    end
`endif

    // =========================================================================
    // Block RAMs. Each has one port, addressed combinationally from the state
    // register; read data appears one clock later.
    // =========================================================================
    logic [11:0] im_addr;
    logic [11:0] im_q_addr;
    logic        im_we;
    logic [15:0] im_wd0, im_wd1, im_wd2;

    logic [9:0]  ifum_addr_p;
    logic        ifum_we_hi, ifum_we_lo;
    logic [15:0] ifum_wd;



    always_comb begin
        // IM: the instruction at real_PC during C_FETCH; the word at Link
        // (for Read IM / Write IM) afterwards.
        // In C_FINISH (unless it commits a Write IM) the read is at the
        // next PC the instruction chose, ahead of scheduling: if the
        // scheduler keeps it, C_FETCH already has the instruction (spec_hit).
        // The execute reads IM at its prediction (C_NF then has the
        // instruction after); C_NF reads it again for C_FETCH.
        im_addr = pent ? f_np[11:0]
                : (PIPELINE && st == C_PIPE) ? ((p_stall || p_bub) ? p_pc_d[11:0] : !ovx ? p_pc_x[11:0] :
                                                p_d ? f_np[11:0] : pred_np[11:0])
                : (exec_now && pred_ok) ? pred_np[11:0]
                : (st == C_FETCH || st == C_DEC) ? real_PC[11:0]
                : (st == C_EXEC)  ? Link[11:0]
                : ((st == C_NEXT || st == C_NF || st == C_IFU_END || st == C_IFU_DEC ||
                    st == C_IFU_AW || st == C_IFU_BW) && pred_ok) ? pred_np[11:0]
                : (st == C_FINISH && !x_im_wr) ? x_np[11:0]
                :                   link_at_issue[11:0];
        im_we   = (st == C_FINISH) && x_im_wr;
        im_wd0  = x_im_wd0;
        im_wd1  = x_im_wd1;
        im_wd2  = x_im_wd2;

        // IFUM: always the entry for the current {InsSet, opcode}, except
        // in the clock that commits an IFUMRH/IFUMLH write.
        // In C_IFU_W the entry is read for the opcode arriving this clock
        // (the section's Md), so C_IFU_DEC can follow directly.
        ifum_addr_p = x_ifum_wr ? x_ifum_waddr
                    : (st == C_IFU_W && m_ret == C_IFU_DEC) ? {ifu_insset, ifu_byte_of(md)}
                    : {ifu_insset, ifu_opcode};
        ifum_we_hi  = x_ifum_wr && x_ifum_hi;
        ifum_we_lo  = x_ifum_wr && !x_ifum_hi;
        ifum_wd     = x_ifum_wd;
    end

    always_ff @(posedge clk) begin
        if (im_we) begin
            im_iw0[im_addr]     <= im_wd0;
            im_iw1[im_addr]     <= im_wd1;
            im_iw2[im_addr]     <= im_wd2;
            im_present[im_addr] <= 1'b1;
        end
        im_q0      <= im_iw0[im_addr];
        im_q1      <= im_iw1[im_addr];
        im_q2      <= im_iw2[im_addr];
        im_q_pres  <= im_present[im_addr];
        im_q_addr  <= im_addr;              // the address the read data is from
        im_q_brk   <= im_breakpoint[im_addr];
    end

    // =========================================================================
    // Read-ahead. C_NEXT reads IM at the next PC predicted here (a pure
    // function of registers; a wrong or missing prediction only costs the
    // clock it would have saved). C_FINISH, when that read is the next
    // instruction, latches it into ex_u and reads its RM/STK operand at the
    // RBase and StkP C_FINISH leaves behind (fin_*). C_FETCH then executes it
    // in the same clock, if nothing intervened (rm_pre_ok).
    // =========================================================================
    // Read IM and Write IM use the word IM delivers for C_EXEC's read at
    // Link, so they always take the C_EXEC path.
    function automatic logic jcn_reads_im_at_link_fn(input dorado_uinstr_t u);
        return (u.jcn[2:0] == 3'd7) && (u.jcn[7:5] != 3'd1) && u.jcn[6] &&
               (u.jcn[5:3] == 3'd6 || u.jcn[5:3] == 3'd7);
    endfunction

    function automatic logic cond_pure_fn(input dorado_uinstr_t u, input logic [2:0] cond);
        // eval_branch_condition_task without its side effect (Cnt), as C_NEXT
        // evaluates it: RBase = x_rbase_at_issue, RM/STK from C_NEXTRD.
        int          rm_a;
        logic [15:0] rv;
        rm_a = rm_address_fn(u, ctask, x_rbase_at_issue, task_subtask[ctask], StkP);
        rv   = cur_rm_stk_read;                          // (C_PIPE: with the finish's write)
        case (cond)     // (the flags as C_PIPE's finish beside the execute leaves them)
            3'd0: return zero_x;
            3'd1: return lt0_x;
            3'd2: return ~carry_x;
            3'd3: return (cnt_p == 16'd0);
            3'd4: return (rm_a < 512) && rv[15];
            3'd5: return (rm_a < 512) && rv[0];
            3'd6: return (ctask == 4'd0) ? reschedule_cond : ~atten_p;
            default: return ~ovf_x;
        endcase
    endfunction


    // -------------------------------------------------------------------------
    // C_PIPE's stages (PIPELINE). The finish of x_u, the execute of ex_u, the
    // RM/STK read of cur_u and the IM read of the instruction after cur_u run
    // in one clock, as HM 2.3 overlaps them; everything the pipeline does not
    // take (a branch condition, an IFUJump, I/O, Id, task switches, holds, the
    // host's operations) drains it into the FSM above, which stays as it was.
    // -------------------------------------------------------------------------
    // The FF as exec_task decodes it: {ok, FA, FB, FC} (Table 11a by ASEL<=3)
    function automatic logic [8:0] ff_exec_decode_fn(input dorado_uinstr_t u);
        if (ff_full_function_ok(u)) return {1'b1, ff_fa(u.ff), ff_fb(u.ff), ff_fc(u.ff)};
        if (ff_decode_ok(u) && u.asel <= 3'd3) return {1'b1, 2'd0, u.ff[5:3], u.ff[2:0]};
        return 9'd0;
    endfunction

    function automatic logic jcn_ifujump_fn(input dorado_uinstr_t u);
        return u.jcn[2:0] == 3'd7 && u.jcn[7:5] == 3'd1;
    endfunction

    // the execute writes the IFU's state (PCF, IFUTest, IFUTick, the
    // reschedule, InsSet, IFUM, IFUReset, BrkIns: HM Table 11c FA=1 FB=0/3)
    function automatic logic ff_writes_ifu_fn(input dorado_uinstr_t u);
        logic [8:0] d;
        d = ff_exec_decode_fn(u);
        return d[8] && d[7:6] == 2'd1 && (d[5:3] == 3'd0 || d[5:3] == 3'd3);
    endfunction

    // An IFUJump as next_task would take it now: {ok, RBase changes, the
    // RBase the dispatch leaves, the address}. ok = 0 for the slow path, a
    // breakpoint or IFUTest feed, an FF condition (it holds the IFU).
    function automatic logic [25:0] ifujump_fn(input dorado_uinstr_t u);
        logic [15:0] np;
        if (u.bsel < 3'd4 && ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) return 26'd0;
        if (!ifu_active || ifu_warmup > 8'd0) begin
            np = ifu_trap_addr_fn(6'o34, u.jcn[4:3]);                    // NotReady
            return ff_loads_link_fn(u) ? 26'd0 : {1'b1, 1'b0, 8'd0, np};
        end
        if (!nx_fast) return 26'd0;
        np = (reschedule_pending == 8'd1) ? ifu_trap_addr_fn(6'o14, u.jcn[4:3])
                                          : {4'd0, ~pf_rh_c[9:0], u.jcn[4:3]};
        return {1'b1, 1'b1, {7'd0, ~pf_lh_c[9]}, np};
    endfunction

    // A JCN next_task resolves without the IFU or a TPC/IM special
    function automatic logic jcn_plain_fn(input dorado_uinstr_t u);
        if (u.jcn[7] || u.jcn[7:4] == 4'd0) return 1'b1;
        if (u.jcn == 8'o045 && u.ff == 8'o147) return 1'b0;
        if (u.jcn[2:0] != 3'd7) return 1'b1;                                 // conditional
        return (u.jcn[7:5] != 3'd1) && u.jcn[6] && (u.jcn[5:3] == 3'd0);      // Return
    endfunction

    // next_task loads Link with CIA+1 (for a plain JCN)
    function automatic logic jcn_calls_fn(input dorado_uinstr_t u);
        logic [7:0] j;
        j = u.jcn;
        if (jcn_ifujump_fn(u)) return 1'b1;                              // the dispatch (ifu_end_task)
        if (ff_loads_link_fn(u)) return 1'b0;
        if (j[7]) return (j[7:6] == 2'd2) ? (j[3:0] == 4'd0) : 1'b1;   // Local Call / Global Call
        if (j[7:4] == 4'd0) return (j[3:0] == 4'd0);                     // Long Call
        if (j[2:0] == 3'd7) return 1'b1;                                  // Return, IFUJump (ifu_end_task)
        return (j[4:3] == 2'd0);                                          // Conditional Call
    endfunction

    // the execute writes Link from B (an explicit Link<-, or a dispatch)
    function automatic logic ff_writes_link_fn(input dorado_uinstr_t u);
        logic [8:0] d;
        d = ff_exec_decode_fn(u);
        return ff_loads_link_fn(u) || (d[8] && d[7:6] == 2'd0 && d[5:3] == 3'd7 && d[2:0] <= 3'd1);
    endfunction

    // the execute writes the dispatch register (BigBDispatch, BDispatch, Multiply)
    function automatic logic ff_writes_dispatch_fn(input dorado_uinstr_t u);
        logic [8:0] d;
        d = ff_exec_decode_fn(u);
        return d[8] && d[7:6] == 2'd0 && d[5:3] == 3'd7 && d[2:0] <= 3'd2;
    endfunction

    // StkP after the finish (the emulator task's stack BLOCK; finish_task)
    function automatic logic [7:0] stkp_after_fn(input dorado_uinstr_t u, input logic [7:0] sp,
                                                 input logic [3:0] t);
        logic [7:0] d8;
        if (!(u.block && t == 4'd0)) return sp;
        d8 = ff_is_dont_modify_stkp(u) ? 8'd0 : stk_signed_delta(u.rstk);
        if (d8 != 8'd0 || (u.rstk[3] && !ff_is_dont_modify_stkp(u)))
            return {sp[7:6], 6'(sp[5:0] + d8[5:0])};
        return sp;
    endfunction

    // The address of u's successor when no branch condition decides it
    // (next_task): {1, address}, or 0 for a conditional jump, a JCN or FF
    // branch condition, an IFUJump, a TPC/IM special, the LoadRam exit, or a
    // Return whose Link is not known yet (link_ok).
    function automatic logic [16:0] succ_fn(input dorado_uinstr_t u, input logic [15:0] pc,
                                            input logic [15:0] link_in, input logic [15:0] disp_in,
                                            input logic link_ok);
        logic [7:0] j;
        logic       is_long, ffc_used;
        j        = u.jcn;
        is_long  = (j[7:4] == 4'd0);
        ffc_used = (u.bsel < 3'd4) && !is_long &&
                   ((u.asel <= 3'd3) ? ((u.ff[5:0] & 6'o70) == 6'o60)
                                     : (ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6));
        if (j[7]) begin
            if (ffc_used) return 17'd0;
            if (j[7:6] == 2'd2)
                return {1'b1, (pc & ~(16'(CPU_PAGE_SIZE - 1))) | {10'd0, j[5:0]} | disp_in};
            return {1'b1, (pc & ~(16'(CPU_QUADRANT_SIZE - 1))) | ({10'd0, j[5:0]} * 16'(CPU_PAGE_SIZE)) | disp_in};
        end
        if (j == 8'o045 && u.ff == 8'o147) return 17'd0;
        if (is_long) return {1'b1, (pc & ~(16'(CPU_QUADRANT_SIZE - 1))) | {4'd0, u.ff, j[3:0]} | disp_in};
        if (j[2:0] == 3'd7 && j[7:5] != 3'd1 && j[6] && j[5:3] == 3'd0 && !ffc_used && link_ok)
            return {1'b1, {4'd0, link_in[11:0]} | disp_in};
        return 17'd0;
    endfunction

    logic [8:0] p_xd;                 // ex_u's FF as the execute decodes it
    assign p_xd = ff_exec_decode_fn(ex_u);
    assign ij_w   = ifujump_fn(x_u);
    assign ij_x   = ifujump_fn(ex_u);
    assign ij_c   = ifujump_fn(cur_u);
    assign w_disp = 1'b0;   // (an IFUJump's finish runs alone in C_PIPE: nothing to forward)
    always_comb begin
        logic [7:0] len;
        // Id: the dispatch beside the execute sets the operands (next_task)
        len   = {6'd0, ~pf_lh_c[11:10]};
        id_x0 = id_r0;
        id_x1 = id_r1;
        if (w_disp) begin
            id_x0 = ifu_id_gen_fn(8'd0, len, pf_lh_c[3:0], ~pf_lh_c[4], pf_rh_c[10], pf_lh_c[15],
                                  (len >= 8'd2) ? pf_b1 : ifu_alpha, (len >= 8'd3) ? pf_b2 : ifu_beta);
            id_x1 = ifu_id_gen_fn(8'd1, len, pf_lh_c[3:0], ~pf_lh_c[4], pf_rh_c[10], pf_lh_c[15],
                                  (len >= 8'd2) ? pf_b1 : ifu_alpha, (len >= 8'd3) ? pf_b2 : ifu_beta);
        end
    end

    assign ctask_x   = ctask;
    assign ex_u_q      = ex_u;
    assign ex_pres_q   = ex_pres;
    assign ex_brk_q    = ex_brk;
    assign idcnt_q     = ifu_idcnt;
    assign Cnt_q       = Cnt;
    assign blr_valid_q = b_link_read_valid;
    assign blr_age_q   = b_link_read_age;
    assign cycles_x  = pfwd ? cycles + 64'd1 : cycles;
    assign sp_wx     = pfwd ? stkp_after_fn(x_u, StkP, ctask) : StkP;
    assign membase_x = !w_disp ? MemBase
                     : (pf_lh_c[8] == 1'b0) ? {4'd0, MemBX, pf_lh_c[7:6]}
                     : 8'o034 + {6'd0, pf_lh_c[7:6]};
    assign p_stall = PIPELINE && (st == C_PIPE) && p_w &&
                     ((x_pref_pending && !mem_fin) || io_busy_tick || ifu_bk_busy);
    assign p_wonly = 1'b0;
    always_comb begin
        pfwd = PIPELINE && (st == C_PIPE) && p_w;
        // What the execute sees: x_u's finish runs before it in the same clock.
        T_x     = T;
        carry_x = alu_carry;
        zero_x  = alu_zero;
        lt0_x   = alu_lt0;
        ovf_x   = alu_overflow;
        Link_x  = Link;
        if (pfwd) begin
            case (x_u.lc)
                3'd1, 3'd7: T_x = x_pd;
                3'd2, 3'd3: T_x = x_md_at_issue;
                3'd5:       T_x = ff_is_t_gets_md(x_u) ? x_md_at_issue : x_pd;
                default: ;
            endcase
            if (!(ff_full_function_ok(x_u) && ff_fa(x_u.ff) == 2'd0 &&
                  ff_fb(x_u.ff) == 3'd7 && ff_fc(x_u.ff) == 3'd6)) begin
                carry_x = x_new_carry;
                zero_x  = x_new_alu_zero;
                lt0_x   = x_new_alu_lt0;
                ovf_x   = x_new_ovf;
            end
            if (jcn_calls_fn(x_u)) Link_x = real_PC + 16'd1;
        end

    end

    always_comb begin
        logic [15:0] avail;
        // Would the schedule after x_u switch (task_schedule_task, from the
        // registers it starts from; the finish ORs in the I/O tick's wakeups)?
        avail = ready | wakeup_pending | wakeup_pipe[1] | ext_wakeup_req;
        if (tasksim_fired || (!tasksim_loaded && tasksim == 7'o177)) avail[DORADO_TASKSIM_TASK] = 1'b1;
        if (junk_tw_enabled && junk_tw_countdown <= 16'd1) avail[DORADO_JUNK_TASK] = 1'b1;   // the finish's pendulum tick
        // (C_PIPE runs the emulator task alone: a switch is any other task ready)
        sw_maybe = (tasking_on || tasking_resume_delay == 8'd1) &&
                   (avail[15:1] != 15'd0 || ctask != 4'd0);

    end

    always_comb begin
        logic [16:0] sn;
        logic [15:0] link_d;
        logic        link_ok;
        link_ok = 1'b1;
        // F: the instruction after cur_u. In C_NF cur_u executes next (the
        // finish of x_u now loads Link); in C_PIPE it executes after ex_u.
        if (st == C_NF) begin
            link_d  = jcn_calls_fn(x_u) ? real_PC + 16'd1 : Link;
            sn      = succ_fn(cur_u, x_pred_np, link_d, task_dispatch[ctask],
                              jcn_plain_fn(x_u) || (jcn_ifujump_fn(x_u) && !ff_loads_link_fn(x_u)));
        end else begin
            link_d  = jcn_calls_fn(ex_u) ? p_pc_x + 16'd1 : Link_x;
            link_ok = jcn_calls_fn(ex_u) || !ff_writes_link_fn(ex_u);
            sn      = ff_writes_dispatch_fn(ex_u) ? 17'd0 : succ_fn(cur_u, p_pc_d, link_d, 16'd0, link_ok);
            // an IFUJump: the dispatch the buffer offers now, if nothing
            // before its finish moves the IFU (another dispatch, an FF)
            if (jcn_ifujump_fn(cur_u))
                sn = (ij_c[25] && !jcn_ifujump_fn(ex_u) && !ff_writes_ifu_fn(ex_u) &&
                      !(pfwd && jcn_ifujump_fn(x_u))) ? {1'b1, ij_c[15:0]} : 17'd0;
        end
        f_ok = sn[16];
        f_np = sn[15:0];
        f_rb = {ij_c[24], ij_c[23:16]};

    end

    always_comb begin
        logic [8:0]  xd;
        logic [7:0]  rb_d, sp_w, sp_d;
        // D: cur_u's operand, at the RBase and StkP its issue will see (after
        // x_u's finish, ex_u's execute and ex_u's finish)
        xd   = p_xd;
        rb_d = (w_disp ? ij_w[23:16] : RBase);                          // x_u's dispatch (beside)
        if (xd[8] && xd[7:6] == 2'd2 && xd[5:3] <= 3'd1) rb_d = {4'd0, ex_u.ff[3:0]};
        if (jcn_ifujump_fn(ex_u) && p_rb_d[8]) rb_d = p_rb_d[7:0];        // ex_u's dispatch, as F predicted it
        d_rb = rb_d;
        sp_w = pfwd ? stkp_after_fn(x_u, StkP, ctask) : StkP;
        sp_d = stkp_after_fn(ex_u, sp_w, ctask);
        d_sp = sp_d;
        d_ok = !(xd[8] && xd[7:6] == 2'd1 && xd[5:3] == 3'd0 && xd[2:1] == 2'b11) &&   // RBase<-B
               !(xd[8] && xd[7:6] == 2'd1 && xd[5:3] == 3'd4 && xd[2:1] == 2'b10);    // StkP<-
        d_raddr = rm_address_fn(cur_u, ctask, rb_d, task_subtask[ctask], sp_d);
        dr_rb    = w_disp ? ij_w[23:16] : RBase;
        dr_sp    = sp_w;
        dr_raddr = p_w ? rm_address_fn(ex_u, ctask, dr_rb, task_subtask[ctask], dr_sp) : p_raddr;

    end

    always_comb begin
        logic [8:0] xd;
        xd   = p_xd;
        // X: ex_u executes now, x_u finishing beside it. x_elig: ex_u can
        // run in the pipeline at all; x_host: the host's operation wants
        // C_FETCH; x_ready: nothing passing holds it this clock.
        x_elig  = PIPELINE && (st == C_PIPE) && p_x && ctask == 4'd0 &&
                  !bb_present && ex_pres && !ex_brk &&
                  !uinstr_touches_io_fn(ex_u, 1'b0) &&
                  !jcn_reads_im_at_link_fn(ex_u) && (jcn_plain_fn(ex_u) || jcn_ifujump_fn(ex_u)) &&
                  !(next_reads_rm_fn(ex_u) && xd[8] && xd[7:6] == 2'd1 && xd[5:3] == 3'd4 && xd[2:1] == 2'b10) &&
                  !(xd[8] && xd[7:6] == 2'd1 && (xd[5:3] == 3'd6 ||                     // the section's state
                                                (xd[5:3] == 3'd7 && (xd[2:0] == 3'd1 || xd[2:0] == 3'd4)))) &&   // the event counters
                  holdsim == 8'd0 && !holdsim_loaded && !holdsim_delay && !holdsim_hold;
        // (a host operation is served in C_FETCH, and a hold -- the mailbox
        // filling the receive buffer or emptying a transmit slot -- waits
        // there for cpu_between: in C_PIPE it must drain, not bubble, or the
        // mailbox and the pipeline wait on each other for ever)
        x_host  = (ap_req && !ap_done_q) || ap_hold;
        x_ready = // beside a finish: not in the per-step lock-step, not an
                  // IFUJump's (the dispatch is checked after it), not a switch
                  !(p_w && (ov_hold || jcn_ifujump_fn(x_u) || sw_maybe)) &&
                  !p_stall && !(x_pref_pending && !mem_fin) && !io_busy_tick &&
                  // a dispatch's byte references still booking: only an execute
                  // that would meet them waits (as ifu_bk_gate; Id is current in
                  // id_r0, which the clock that dispatched updated)
                  !(ifu_bk_busy && ((ex_u.asel <= 3'd3 && !jcn_is_write_im_fn(ex_u) &&
                                     decode_ref_kind_fn(ex_u, 1'b0) != DM_REF_NONE) ||
                                    uinstr_reads_md_fn(ex_u) || (ff_fa(ex_u.ff) == 2'd1 && ff_fb(ex_u.ff) == 3'd6))) &&
                  // a reference completing this clock is booked (mem_pref_task)
                  // at its top: the Md file the datapath reads is the old one
                  !(x_pref_pending && (uinstr_reads_md_fn(ex_u) || ex_u.asel == 3'd7));
        ovx   = x_elig && !x_host && x_ready;
        // otherwise a bubble: x_u's finish alone, ex_u and cur_u held
        p_bub = x_elig && !x_host && !x_ready && !p_stall;
    end

    always_comb begin
        // C_NF enters C_PIPE: cur_u (read at the execute's prediction) runs
        // next, its operand read now, and IM reads the one after
        // (IM's data must be the instruction at x_pred_np: a C_NF that waited
        // may have read IM elsewhere meanwhile, and C_PIPE does not re-read it)
        pent = PIPELINE && (st == C_NF) && !ov_hold && !ap_hold && ctask == 4'd0 && im_q_addr == x_pred_np[11:0] &&
               !(x_pref_pending && !mem_fin) && !io_busy_tick && !ifu_bk_busy &&
               x_pred_fresh && (x_pred_np_x == pred_np) && !sw_maybe && f_ok &&
               (jcn_plain_fn(x_u) || (jcn_ifujump_fn(x_u) && x_disp_v && !ff_loads_link_fn(x_u))) &&
               !uinstr_touches_io_fn(cur_u, 1'b0) &&
               !jcn_reads_im_at_link_fn(cur_u) && !next_reads_rm_fn(cur_u);
    end

    // C_FETCH executes the read-ahead instruction this clock (registers only:
    // the prediction below reads it)
    assign touches_io_c = uinstr_touches_io_fn(cur_u, ctask != 4'd0);
    assign fexec = READAHEAD && (st == C_FETCH) && !(ap_req && !ap_done_q) && !ap_hold &&
                   spec_hit && rm_pre_ok && !(io_busy && touches_io_c) && !ifu_bk_gate &&
                   !jcn_reads_im_at_link_fn(cur_u);

    always_comb begin
        dorado_uinstr_t u;
        logic [7:0]     jcn;
        logic           is_long, ffc;
        logic [15:0]    link_p, disp_p;
        // In the execute clock the instruction is ex_u and its Link, dispatch
        // register and Cnt are as the execute will leave them (Link<-, the
        // dispatch register cleared, Cnt<-): the branch conditions are the
        // previous instruction's (HM 4.4), already in their registers.
        exec_now = (st == C_EXEC) || fexec || (PIPELINE && st == C_PIPE);   // (C_PIPE: used only when it executes)
        u       = exec_now ? ex_u : x_u;
        link_p  = exec_now ? Link_x : link_at_issue;
        pc_p    = (st == C_PIPE) ? p_pc_x : real_PC;
        disp_p  = exec_now ? task_dispatch[ctask] : dispatch_or;
        cnt_p   = Cnt;
        atten_p = exec_now ? ((io_attached && ctask != 4'd0) ? io_atten_in : 1'b0) : io_atten_at_issue;
        if (exec_now && ff_full_function_ok(u) && ff_fa(u.ff) == 2'd3 &&
            (ff_fb(u.ff) == 3'd4 || ff_fb(u.ff) == 3'd5))
            cnt_p = (u.ff[3:0] == 4'd0) ? 16'd16 : {12'd0, u.ff[3:0]};
        jcn     = u.jcn;
        is_long = (jcn[7:4] == 4'd0);
        pred_ok = 1'b0;
        pred_why = 2'd3;
        pred_np = pc_p + 16'd1;
        ffc     = 1'b0;
        if (u.bsel < 3'd4 && !is_long) begin
            if (u.asel <= 3'd3) begin
                if ((u.ff[5:0] & 6'o70) == 6'o60) ffc = cond_pure_fn(u, u.ff[2:0]);
            end else if (ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) begin
                ffc = cond_pure_fn(u, u.ff[2:0]);
            end
        end
        if (jcn[7]) begin
            pred_ok = 1'b1;
            if (jcn[7:6] == 2'd2)       // Local Jump/Call
                pred_np = (pc_p & ~(16'(CPU_PAGE_SIZE - 1))) | {10'd0, jcn[5:0]} |
                          {15'd0, ffc} | disp_p;
            else                        // Global Call
                pred_np = (pc_p & ~(16'(CPU_QUADRANT_SIZE - 1))) |
                          ({10'd0, jcn[5:0]} * 16'(CPU_PAGE_SIZE)) | {15'd0, ffc} | disp_p;
        end else if (jcn == 8'o045 && u.ff == 8'o147) begin
            pred_ok = 1'b0;             // LoadRam End-item exit
            pred_why = 2'd2;
        end else if (is_long) begin     // Long Jump/Call
            pred_ok = 1'b1;
            pred_np = (pc_p & ~(16'(CPU_QUADRANT_SIZE - 1))) | {4'd0, u.ff, jcn[3:0]} | disp_p;
        end else if (jcn[2:0] == 3'd7) begin
            if (jcn[7:5] != 3'd1 && jcn[6] && jcn[5:3] == 3'd0) begin   // Return
                pred_ok = 1'b1;
                pred_np = {4'd0, link_p[11:0]} | {15'd0, ffc} | disp_p;
            end
        end else begin                  // Conditional Jump/Call
            pred_ok = 1'b1;
            pred_np = (pc_p & ~(16'(CPU_PAGE_SIZE - 1))) |
                      {10'd0, jcn[6:5], 1'b0, jcn[4:3], cond_pure_fn(u, jcn[2:0])} | disp_p;
        end

        // C_IFU_END: the dispatch address (or a trap), from registers
        pe_tnia_r = (x_ifaddr << 2) | {14'd0, x_n_slot | {1'b0, x_cond_true}};
        if (x_resched_trap) pe_tnia_r = ifu_trap_addr_fn(6'o14, x_n_slot);
        pe_tnia = (x_ifaddr << 2) | {14'd0, x_n_slot | {1'b0, x_cond_true}};
        if (x_resched_trap) pe_tnia = ifu_trap_addr_fn(6'o14, x_n_slot);
        if (x_op_faulted)   pe_tnia = ifu_trap_addr_fn(6'o00, x_n_slot);
        if (st == C_IFU_END) begin
            pred_ok = 1'b1;
            pred_np = {4'd0, pe_tnia[11:0]};
        end
        // the states that now end a dispatch themselves
        pe_tnia_d = ({6'd0, ~ifum_q_lo[9:0]} << 2) | {14'd0, x_n_slot | {1'b0, x_cond_true}};
        if (x_resched_trap) pe_tnia_d = ifu_trap_addr_fn(6'o14, x_n_slot);
        pe_flt    = x_op_faulted || (mem_res_fault_r != DM_FAULT_NONE);   // the fetch completed a clock ago
        pe_trap0  = ifu_trap_addr_fn(6'o00, x_n_slot);
        if (st == C_IFU_DEC && ifum_q_pres && {6'd0, ~ifum_q_hi[11:10]} < 8'd2) begin
            pred_ok = 1'b1;                          // one-byte opcode
            pred_np = {4'd0, pe_tnia_d[11:0]};
        end
        // C_NEXT dispatching from the prefetch buffer: the entry's address
        pe_cond_f = (u.bsel < 3'd4 && ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) ? cond_pure_fn(u, u.ff[2:0]) : 1'b0;
        pe_neff_f = jcn[4:3] | {1'b0, pe_cond_f};
        pe_trap14 = ifu_trap_addr_fn(6'o14, jcn[4:3]);
        if ((exec_now || st == C_NEXT || st == C_NF) && nx_fast && !is_long && jcn[2:0] == 3'd7 && jcn[7:5] == 3'd1) begin
            pred_ok = 1'b1;
            pred_np = (reschedule_pending == 8'd1) ? {4'd0, pe_trap14[11:0]} : {4'd0, ~pf_rh_c[9:0], pe_neff_f};
        end
        if (!is_long && jcn[2:0] == 3'd7 && jcn[7:5] == 3'd1 && !pred_ok) pred_why = 2'd1;
        // A condition on RM/STK (RM<0, RM odd; by JCN or by FF) tests the
        // instruction's own operand register: in the execute clock it is in
        // rm_q/stk_q only when the operand was read ahead (fexec, rm_pre_ok).
        // Otherwise C_NEXTRD reads it and C_NEXT branches.
        if (exec_now && next_reads_rm_fn(u) && !(fexec && rm_pre_ok) && !(st == C_PIPE)) begin
            pred_ok  = 1'b0;
            pred_why = 2'd0;
        end
        if ((st == C_IFU_AW && (!(ifu_length >= 8'd3 && !pe_flt) ||
                                (x_test_feed && ifu_test_count > 4'd0))) ||
            st == C_IFU_BW) begin
            pred_ok = 1'b1;
            pred_np = pe_flt ? {4'd0, pe_trap0[11:0]} : {4'd0, pe_tnia_r[11:0]};
        end


        // what C_FINISH leaves in RBase and StkP (no task switch)
        fin_rbase = (ifu_dispatch_rbase >= 0) ? 8'(ifu_dispatch_rbase) : x_rbase_after_ff;
        // (PIPELINE) In C_NF the execute's RBase is in RBase still, or x_u's
        // dispatch sets it (x_disp_*, from the execute's clock)
        if (PIPELINE && st == C_NF)
            fin_rbase = (jcn_ifujump_fn(x_u) && x_disp_v) ? x_disp_rb : RBase;
        fin_stkp  = StkP;
        fin_d8    = ff_is_dont_modify_stkp(x_u) ? 8'd0 : stk_signed_delta(x_u.rstk);
        if (x_u.block && ctask == 4'd0 &&
            (fin_d8 != 8'd0 || (x_u.rstk[3] && !ff_is_dont_modify_stkp(x_u))))
            fin_stkp = {StkP[7:6], 6'(StkP[5:0] + fin_d8[5:0])};
    end

    // RM/STK: read at the decode-time address in C_DEC, at the branch-
    // condition address in C_NEXTRD; written by the LC writeback in C_FINISH.
    int          rms_raddr, rms_waddr;
    logic        rms_we;
    logic [15:0] rms_wd;
    always_comb begin
        if (st == C_NEXTRD)
            rms_raddr = rm_address_fn(x_u, ctask, x_rbase_at_issue, task_subtask[ctask], StkP);
        else
            rms_raddr = rm_address_fn((st == C_FETCH || st == C_DEC) ? cur_u : ex_u,
                                      ctask, RBase, task_subtask[ctask], StkP);
        if (st == C_FETCH && ap_req && ap_op == AP_RM_OR)
            rms_raddr = int'(ap_va[7:0]);
        if ((st == C_FINISH || st == C_NF) && x_pred_fresh)   // the next instruction's operand
            rms_raddr = rm_address_fn(cur_u, ctask, fin_rbase, task_subtask[ctask], fin_stkp);
        // C_FETCH executing: its operand is already here, so the port reads
        // the branch-condition register C_NEXTRD would (RBase is the issue
        // RBase in this clock), and C_NEXT can follow directly.
        if (fexec)
            rms_raddr = rm_address_fn(cur_u, ctask, RBase, task_subtask[ctask], StkP);
        if (PIPELINE && st == C_PIPE)                          // D: the instruction after ex_u
            rms_raddr = (p_stall || p_bub) ? p_raddr        // (waiting: ex_u's operand stays)
                      : !ovx ? dr_raddr                       // (a drain: ex_u's again)
                      : p_d  ? d_raddr
                      :        p_raddr;                         // (no fetch after: ex_u's condition register, as fexec)
    end
    always_comb begin
        rms_waddr = lc_write_address_fn(x_u, ctask, x_rbase_at_issue, task_subtask[ctask], StkP);
        rms_we    = (st == C_FINISH || st == C_NF || (st == C_PIPE && p_w)) && !halted &&
                    (x_u.lc == 3'd2 || x_u.lc == 3'd4 || x_u.lc == 3'd5 ||
                     x_u.lc == 3'd6 || x_u.lc == 3'd7);
        rms_wd    = (x_u.lc == 3'd4 || x_u.lc == 3'd5) ? x_md_at_issue : x_pd;
        if (st == C_FETCH && ap_req && ap_stk && !ap_done_q) begin
            rms_waddr = 256 + int'(ap_va[7:0]);
            rms_we    = 1'b1;
            rms_wd    = ap_wd;
        end
        if (st == C_APW && x_rmor) begin            // RM[a] |= wd, read in C_FETCH
            rms_waddr = int'(ap_va[7:0]);
            rms_we    = 1'b1;
            rms_wd    = rm_q | ap_wd;
        end
    end
    always_ff @(posedge clk) begin
        if (rms_we && rms_waddr >= 256)  STK[rms_waddr[7:0]] <= rms_wd;
        if (rms_we && rms_waddr < 256)   RM[rms_waddr[7:0]]  <= rms_wd;
        rm_qr      <= RM[rms_raddr[7:0]];
        stk_qr     <= STK[rms_raddr[7:0]];
        // write-first, outside the RAMs: C_FINISH reads the next
        // instruction's operand while writing this one's result, possibly at
        // the same address
        rms_byp    <= rms_we && ((rms_waddr < 256) == (rms_raddr < 256)) &&
                      rms_waddr[7:0] == rms_raddr[7:0];
        rms_bypv   <= rms_wd;
        rmq_is_stk <= (rms_raddr >= 256);
    end

    always_ff @(posedge clk) begin
        if (ifum_we_hi) ifum_hi[ifum_addr_p] <= ifum_wd;
        if (ifum_we_lo) ifum_lo[ifum_addr_p] <= ifum_wd;
        if (ifum_we_hi || ifum_we_lo) ifum_present[ifum_addr_p] <= 1'b1;
        ifum_q_hi   <= ifum_hi[ifum_addr_p];
        ifum_q_lo   <= ifum_lo[ifum_addr_p];
        ifum_q_pres <= ifum_present[ifum_addr_p];
        // the prefetch buffer's read of the next opcode's entry
        ifum_q2_hi   <= ifum_hi[ifum_addr2];
        ifum_q2_lo   <= ifum_lo[ifum_addr2];
        ifum_q2_pres <= ifum_present[ifum_addr2];
    end
    // the opcode byte: from the word arriving this clock (a peek's answer),
    // else the one held
    assign ifum_addr2 = {ifu_insset, byte_at_fn((mem_peek_done && mem_peek_hit && pf_st == 3'd1) ? md : pf_w0, pf_pcf[0])};


    // =========================================================================
    // The memory section
    // =========================================================================
    dorado_rtl_mem #(.STORAGE_WORDS(STORAGE_WORDS)) u_mem (
        .clk              (clk),
        .rst              (rst),
        .clk_en           (clk_en),
        .spec_req         (mem_spec_req),
        .spec_va          (mem_spec_va),
        .req              (mem_req),
        .req_kind         (mem_req_kind),
        .req_va           (mem_req_va),
        .req_b            (mem_req_b),
        .req_tioa         (mem_req_tioa),
        .req_task         (mem_req_task),
        .req_subtask      (mem_req_subtask),
        .req_tag          (mem_req_tag),
        .accepted         (mem_accepted),
        .res_tag          (mem_res_tag),
        .res_way          (mem_res_way),
        .spec2_req        (mem_spec2_req),
        .spec2_va         (mem_spec2_va),
        .req2             (mem_req2),
        .req2_va          (mem_req2_va),
        .accepted2        (mem_accepted2),
        .peek_done        (mem_peek_done),
        .peek_hit         (mem_peek_hit),
        .peek_retry       (mem_peek_retry),
        .snoop_row_stb    (mem_snoop_row_stb),
        .snoop_row        (mem_snoop_row),
        .snoop_wr         (mem_snoop_wr),
        .snoop_wr_addr    (mem_snoop_wr_addr),
        .snoop_wr_way     (mem_snoop_wr_way),
        .ref_row          (mem_ref_row),
        .inflight         (mem_inflight),
        .inflight_tag     (mem_inflight_tag),
        .idle             (mem_idle),
        .done             (mem_done),
        .done_now         (mem_done_now),
        .res_fault        (mem_res_fault),
        .res_miss         (mem_res_miss),
        .res_latency      (mem_res_latency),
        .md               (md),
        .md_r             (md_r),
        .res_fault_r      (mem_res_fault_r),
        .res_valid_r      (mem_res_valid_r),
        .res_tag_r        (mem_res_tag_r),
        .res_miss_r       (mem_res_miss_r),
        .res_latency_r    (mem_res_latency_r),
        .book_now         (mem_book_now),
        .book_srn         (mem_book_srn),
        .book_push        (mem_book_push),
        .book_is_map      (mem_book_is_map),
        .book_tag         (mem_book_tag),
        .ap_req           (mem_ap_req),
        .ap_op            (ap_op),
        .ap_we            (ap_we),
        .ap_cache         (ap_cache),
        .ap_va            (ap_va),
        .ap_wd            (ap_wd),
        .ap_done          (mem_ap_done),
        .ap_rd            (mem_ap_rd),
        .mcr              (mcr),
        .proc_srn         (proc_srn),
        .fault_clear_stb  (mem_fault_clear_stb),
        .cflags_stb       (mem_cflags_stb),
        .cflags_val       (mem_cflags_val),
        .fault_count      (fault_count),
        .fault_info       (mem_fault_info),
        .config_word      (mem_config_word),
        .read_map         (mem_read_map),
        .dbuf             (dbuf),
        .pipe_va_p        (mem_pipe_va),
        .pipe_rp_p        (mem_pipe_rp),
        .pipe2_p          (mem_pipe2),
        .pipe4_p          (mem_pipe4),
        .pipe5_p          (mem_pipe5),
        .fastio_set       (mem_fastio_set),
        .fastio_kind      (fastio_kind),
        .fastio_task      (fastio_task),
        .fastio_subtask   (fastio_subtask),
        .fastio_va        (fastio_va),
        .fastio_out_munch (fastio_out_munch),
        .fastio_in_munch  (fastio_in_munch),
        .sto_req          (sto_req),
        .sto_we           (sto_we),
        .sto_addr         (sto_addr),
        .sto_mask         (sto_mask),
        .sto_wline        (sto_wline),
        .sto_ack          (sto_ack),
        .sto_rline        (sto_rline),
        .dbg_st           (mem_dbg_st)
    );

    // The address of the reference this clock may issue, for the section to
    // read ahead (mem_spec_*): the processor's (exec_task's own VA, from the
    // same operands and base register), or the IFU byte the launching state
    // may fetch. A wrong or absent one costs the clock it would have saved.
    always_comb begin
        logic [4:0]   mb_eff;
        logic [27:0]  br_val, disp;
        dm_ref_kind_t kind;
        logic         idinc;
        kind   = decode_ref_kind_fn(ex_u, ctask != 4'd0);
        mb_eff = MemBase[4:0];
        if (ctask != 4'd0) mb_eff |= {2'b00, task_subtask[ctask], 1'b0};
        br_val = mcr_disbr() ? 28'd0 : br[mb_eff];
        idinc  = (ex_u.asel == 3'd5) ||
                 ((ex_u.asel == 3'd2 || ex_u.asel == 3'd3) && ff_decode_ok(ex_u) && ex_u.ff[7:6] == 2'd1);
        if (kind == DM_REF_IFETCH) br_val = {br_val[27:8], (idinc ? id_x1[7:0] : id_x0[7:0])};
        disp   = (kind == DM_REF_LONGFETCH) ? {comb_alu_b[11:0], comb_alu_a} : {12'd0, comb_alu_a};
        mem_spec_va  = (br_val + disp) & 28'h0FFFFFFF;
        case (st)
            C_NEXT, C_NF: mem_spec_va = ifu_va_fn(ifu_pcf);   // a dispatch's first byte
            C_IFU_DEC:  mem_spec_va = ifu_va_fn(ifu_pcf + 16'd1);
            C_IFU_BETA: mem_spec_va = ifu_va_fn(x_pc_after);
            C_IFU_AW:   mem_spec_va = ifu_va_fn(x_pc_after + 16'd1);
            default: ;
        endcase
        if (ifu_dl_valid && (!mem_req || mem_accepted)) mem_spec_va = ifu_dl_va;
        if (bk_n != 2'd0 && (!mem_req || mem_accepted)) mem_spec_va = ifu_va_fn(bk_pc);   // the next byte's reference
    end

    // What the prefetch buffer offers an IFUJump in C_NEXT: the opcode byte,
    // its entry (length from it, as C_IFU_DEC decodes), the operand bytes if
    // the entry wants them, all at the addresses the IFU would fetch now; no
    // MCR test mode that changes what a fetch returns; no reference this
    // clock to the buffer's rows (a change there would come too late to see).
    always_comb begin
        logic mcr_mode;
        mcr_mode  = (mcr == 16'hFEE7) ? 1'b0 : (mcr[15] | mcr[14] | mcr[13] | mcr[5]);
        // the entry read last clock arrives on IFUM's second port now
        pf_ev_c   = pf_ev || (pf_ifum_wait && !pf_w0_new);
        pf_lh_c   = pf_ev ? pf_lh : ifum_q2_hi;
        pf_rh_c   = pf_ev ? pf_rh : ifum_q2_lo;
        pf_pres_c = pf_ev ? pf_pres : ifum_q2_pres;
        pf_v1_c   = pf_v1;               // (not the peek answering now: the hit test would reach the dispatch)
        pf_w1_c   = pf_w1;
        pf_len    = {6'd0, ~pf_lh_c[11:10]};
        pf_b0     = byte_at_fn(pf_w0, pf_pcf[0]);
        pf_b1     = pf_pcf[0] ? byte_at_fn(pf_w1_c, 1'b0) : byte_at_fn(pf_w0, 1'b1);
        pf_b2     = byte_at_fn(pf_w1_c, pf_pcf[0]);
        pf_b1_ok  = pf_pcf[0] ? (pf_v1_c && pf_va1 == ifu_va_fn(pf_pcf + 16'd1)) : pf_v0;
        pf_b2_ok  = pf_v1_c && (pf_va1 == ifu_va_fn(pf_pcf + 16'd2));
        pf_hazard = (mem_accepted && (mem_req_va[9:4] == pf_row0 || mem_req_va[9:4] == pf_row1)) ||
                    (!mem_idle && (mem_ref_row == pf_row0 || mem_ref_row == pf_row1)) ||
                    mem_cflags_stb;
        nx_fast   = ifu_active && ifu_warmup == 8'd0 && !brk_pending && ifu_test_count == 4'd0 &&
                    !(mem_req && !mem_accepted) &&
                    pf_pcf == ifu_pcf && pf_insset == ifu_insset && pf_ev_c && pf_pres_c &&
                    pf_v0 && pf_va0 == ifu_va_fn(ifu_pcf) && !mcr_mode && !pf_hazard &&
                    (pf_len < 8'd2 || pf_b1_ok) && (pf_len < 8'd3 || pf_b2_ok);
    end
    // (apart from the address: the address depends on Md, which the section
    // computes from the request, and a block computing both would look like
    // a loop to Verilator)
    assign mem_spec_req = (bk_n != 2'd0 && (!mem_req || mem_accepted)) ||
                          (ifu_dl_valid && (!mem_req || mem_accepted)) ||
                          ((st == C_NEXT || st == C_NF) && x_u.jcn[7:5] == 3'd1 && x_u.jcn[2:0] == 3'd7) ||
                          (st == C_IFU_DEC || st == C_IFU_BETA || st == C_IFU_AW) ||
                          ((st == C_EXEC || fexec || (PIPELINE && st == C_PIPE && p_x)) && ex_u.asel <= 3'd3 && !jcn_is_write_im_fn(ex_u) &&
                           decode_ref_kind_fn(ex_u, ctask != 4'd0) != DM_REF_NONE);
    // the peek the buffer issues this clock: decided here, from the registers,
    // so the clocked issue and the read-ahead agree
    // (a peek may be issued in the clock the last byte reference is taken:
    // the next opcode's word is then here a clock sooner)
    assign pf_issue_c    = !mem_req2 && (bk_n == 2'd0) && !(mem_inflight && mem_inflight_tag == TAG_BK) &&
                           (!mem_req || (mem_accepted && mem_req_tag == TAG_BK)) && !mem_spec_req &&
                           (pf_st == 3'd0 || pf_st == 3'd2);
    // ...and word1's, in the clock word0's is taken (its address read ahead then)
    assign pf_issue1_c   = mem_peek_done && mem_peek_hit && pf_st == 3'd1 && !mem_spec_req;
    assign mem_spec2_req = pf_issue_c || pf_issue1_c;
    assign mem_spec2_va  = (pf_st == 3'd0) ? ifu_va_fn(pf_pcf) : ((ifu_va_fn(pf_pcf) + 28'd1) & 28'h0FFFFFFF);
    // the reference issued (mem_req) has completed: not before it is
    // accepted, and in the accept clock only if it completes then
    // (registers only: a hit completes in C_NEXT and is consumed in C_FINISH,
    // which retires in the same clock; a pending request of the processor's
    // own means the result shown is an older one)
    assign mem_fin = mem_res_valid_r && (mem_res_tag_r == TAG_PROC) &&
                     !(mem_req && mem_req_tag == TAG_PROC);
    // the dispatched bytes' references are booked before the processor's next
    // instruction executes (or the assist runs, or the lock-step compares);
    assign ifu_bk_busy = (bk_n != 2'd0) || (mem_req && mem_req_tag == TAG_BK) ||
                         (mem_inflight && mem_inflight_tag == TAG_BK);
    assign cmp_done    = cmp_wait && !ifu_bk_busy;
    // An execute that references memory (its request would pass theirs),
    // reads Md, reads the section's state (FaultInfo', Pipe, Config',
    // MapBufBusy: HM Table 11c FA=1 FB=6), or reads Id (id_r0/id_r1 are
    // registered from the IFU state of the clock before, and the dispatch
    // that owes the bookings has just changed it) waits for them; any other goes
    // ahead, as the real processor does beside its IFU (HM 6.3). The RETIRE
    // waits for them (finish_task), so the tick and the next dispatch stay
    // ordered after them; the lock-step compares the processor at step_done
    // and the memory section at cmp_done.
    assign ifu_bk_gate = ifu_bk_busy &&
                         ((cur_u.asel <= 3'd3 && !jcn_is_write_im_fn(cur_u) &&
                           decode_ref_kind_fn(cur_u, ctask != 4'd0) != DM_REF_NONE) ||
                          uinstr_reads_md_fn(cur_u) || uinstr_reads_id_fn(cur_u) ||
                          (ff_fa(cur_u.ff) == 2'd1 && ff_fb(cur_u.ff) == 3'd6));
    assign fastio_stb = fastio_stb_r || mem_fastio_set;

    generate
        if (EXT_STORAGE) begin : g_ext_storage
            assign sto_ack   = sto_ack_ext;
            assign sto_rline = sto_rline_ext;
        end else begin : g_storage
            dorado_rtl_storage #(.STORAGE_WORDS(STORAGE_WORDS)) u_stor (
                .clk   (clk),
                .rst   (rst),
                .req   (sto_req),
                .we    (sto_we),
                .addr  (sto_addr),
                .mask  (sto_mask),
                .wline (sto_wline),
                .ack   (sto_ack),
                .rline (sto_rline)
            );
        end
    endgenerate

    // Md as an A, B or shifter source: the data is wanted at t1 (HM 5.4
    // "immediate <-Md"); the LC-only forms (T<-Md, RM/STK<-Md) want it at
    // t3 ("deferred"). uinstr_reads_md_fn without its LC clause.
    function automatic logic uinstr_md_immediate_fn(input dorado_uinstr_t u);
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
        return 1'b0;
    endfunction

    // The census, at the retirement of u by task ct.
    task automatic cen_retire_task(input dorado_uinstr_t u, input logic [3:0] ct);
        dm_ref_kind_t k;
        logic [63:0]  d;
        logic         fs, imm;
        k  = decode_mem_ref(u);
        fs = (k == DM_REF_FETCH || k == DM_REF_STORE || k == DM_REF_LONGFETCH);
        cen_retired = cen_retired + 64'd1;
        if (ct == 4'd0) cen_task0 = cen_task0 + 64'd1;
        if (fs) cen_ref_fs = cen_ref_fs + 64'd1;
        else if (k != DM_REF_NONE) cen_ref_other = cen_ref_other + 64'd1;
        if (k == DM_REF_STORE) cen_store = cen_store + 64'd1;
        if (uinstr_reads_md_fn(u)) begin
            imm = uinstr_md_immediate_fn(u);
            d   = cen_retired - cen_last_ref_at[ct];
            cen_md = cen_md + 64'd1;
            if (d == 64'd1)      begin if (imm) cen_md_imm1 = cen_md_imm1 + 64'd1; else cen_md_def1 = cen_md_def1 + 64'd1; end
            else if (d == 64'd2) begin if (imm) cen_md_imm2 = cen_md_imm2 + 64'd1; else cen_md_def2 = cen_md_def2 + 64'd1; end
            else                 cen_md_3p = cen_md_3p + 64'd1;
        end
        if (fs) cen_last_ref_at[ct] = cen_retired;
        if (!u.jcn[7] && u.jcn[7:4] != 4'd0 && u.jcn[2:0] != 3'd7) cen_cond = cen_cond + 64'd1;
        if (jcn_reads_im_at_link_fn(u)) cen_imrw = cen_imrw + 64'd1;
        if (io_read_stb || io_write_stb || fastio_stb) cen_io = cen_io + 64'd1;
        if (u.block && ct != 4'd0) cen_block = cen_block + 64'd1;   // in the emulator BLOCK is the StkP modifier
    endtask

    // The census, at an IFUJump dispatch (ifu_end_task).
    task automatic cen_dispatch_task();
        logic [63:0] d;
        d = cen_retired - cen_last_ifuj_at;
        cen_last_ifuj_at = cen_retired;
        cen_ifujump = cen_ifujump + 64'd1;
        if      (d <= 64'd1)  cen_ifu_hl[0] = cen_ifu_hl[0] + 64'd1;
        else if (d == 64'd2)  cen_ifu_hl[1] = cen_ifu_hl[1] + 64'd1;
        else if (d == 64'd3)  cen_ifu_hl[2] = cen_ifu_hl[2] + 64'd1;
        else if (d == 64'd4)  cen_ifu_hl[3] = cen_ifu_hl[3] + 64'd1;
        else if (d <= 64'd8)  cen_ifu_hl[4] = cen_ifu_hl[4] + 64'd1;
        else if (d <= 64'd16) cen_ifu_hl[5] = cen_ifu_hl[5] + 64'd1;
        else                  cen_ifu_hl[6] = cen_ifu_hl[6] + 64'd1;
        if (ifu_length <= 8'd3) cen_ifu_len[ifu_length[1:0]] = cen_ifu_len[ifu_length[1:0]] + 64'd1;
        if (ifu_pcx[0]) cen_ifu_odd = cen_ifu_odd + 64'd1;
        if (ifu_type_jump && !x_cond_true && !ifu_type_pause) cen_ifu_jump = cen_ifu_jump + 64'd1;
    endtask

    // The opcode's IFUM entry (x_lh, x_rh) decoded: C_IFU_DEC, and the
    // dispatch from the prefetch buffer (ifu_decode_lh / ifu_decode_rh).
    task automatic ifu_decode_task();
        ifu_sign       = x_lh[15];
        ifu_length     = {6'd0, ~x_lh[11:10]};
        ifu_type_pause = ~x_lh[5];
        ifu_type_jump  = ~x_lh[4];
        ifu_n          = x_lh[3:0];
        if (x_lh[8] == 1'b0) MemBase = {4'd0, MemBX, x_lh[7:6]};
        else                 MemBase = 8'o034 + {6'd0, x_lh[7:6]};
        RBase              = {7'd0, ~x_lh[9]};
        ifu_dispatch_rbase = int'(RBase);
        ifu_packed_a = x_rh[10];
        x_ifaddr     = {6'd0, ~x_rh[9:0]};
    endtask

    // C_NEXT's work: the next address, the IFU dispatch (from the buffer, or
    // the slow path's first fetch), Link, the IM read for the read-ahead.
    task automatic next_task();
        dorado_uinstr_t u;
        logic [7:0]  jcn;
        logic        top1;
        logic [1:0]  top2;
        logic        is_long;
        logic [15:0] np;

        u                  = x_u;
        np                 = real_PC + 16'd1;
        x_rbase_after_ff   = RBase;
        RBase              = x_rbase_at_issue;
        ifu_dispatch_rbase = -1;
        st                 = C_FINISH;

        jcn        = u.jcn;
        top1       = jcn[7];
        top2       = jcn[7:6];
        x_ff_cond_or = 1'b0;
        is_long    = (jcn[7:4] == 4'd0);

        if (u.bsel < 3'd4 && !is_long) begin
            if (u.asel <= 3'd3) begin
                if ((u.ff[5:0] & 6'o70) == 6'o60) begin
                    eval_branch_condition_task(u, u.ff[2:0], x_ff_cond_or);
                end
            end else if (ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) begin
                eval_branch_condition_task(u, u.ff[2:0], x_ff_cond_or);
            end
        end

        if (top1 == 1'b1) begin
            logic [15:0] page_addr;
            page_addr = {10'd0, jcn[5:0]};
            if (top2 == 2'd2) begin
                // Local Jump/Call
                np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) | page_addr |
                     {15'd0, x_ff_cond_or} | dispatch_or;
                if (page_addr[3:0] == 4'd0 && !ff_loads_link_fn(u)) begin
                    Link = real_PC + 16'd1;
                end
            end else begin
                // Global Call
                np = (real_PC & ~(16'(CPU_QUADRANT_SIZE - 1))) |
                     (page_addr * 16'(CPU_PAGE_SIZE)) |
                     {15'd0, x_ff_cond_or} | dispatch_or;
                if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
            end
        end else begin
            logic [2:0] low3;
            low3 = jcn[2:0];

            if (jcn == 8'o045 && u.ff == 8'o147) begin
                // LoadRam End-item exit
                logic [15:0] flag;
                flag = rmq_is_stk ? stk_q : rm_q;        // read in C_NEXTRD
                if (!flag[0]) np = {4'd0, Link[11:0]};
                else          np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) | 16'o021;
            end else if (jcn[7:4] == 4'd0) begin
                // Long Jump/Call
                np = (real_PC & ~(16'(CPU_QUADRANT_SIZE - 1))) |
                     {4'd0, u.ff, jcn[3:0]} | dispatch_or;
                if (jcn[3:0] == 4'd0 && !ff_loads_link_fn(u)) begin
                    Link = real_PC + 16'd1;
                end
            end else if (low3 == 3'd7) begin
                if (jcn[7:5] == 3'd1) begin
                    // IFUJump (0 0 1 n n 1 1 1)
                    x_n_slot = jcn[4:3];
                    if (!ifu_active || ifu_warmup > 8'd0) begin
                        evc_events |= EVC_EV_IFUNOTREADY;
                        cen_ifu_notready = cen_ifu_notready + 64'd1;
                        np = ifu_trap_addr_fn(6'o34, x_n_slot);
                        if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                    end else begin
                        x_resched_trap = 1'b0;
                        if (reschedule_pending > 8'd0) begin
                            reschedule_pending = reschedule_pending - 8'd1;
                            if (reschedule_pending == 8'd0) x_resched_trap = 1'b1;
                        end

                        x_cond_true = 1'b0;
                        if (u.bsel < 3'd4 && ff_fa(u.ff) == 2'd0 && ff_fb(u.ff) == 3'd6) begin
                            eval_branch_condition_task(u, u.ff[2:0], x_cond_true);
                        end

                        ifu_pcx     = ifu_pcf;
                        x_test_feed = (ifu_test_count > 4'd0);

                        if (brk_pending) begin
                            ifu_opcode  = brk_opcode;
                            brk_pending = 1'b0;
                            x_test_feed = 1'b0;
                            st          = C_IFU_DECW;
                        end else if (x_test_feed) begin
                            ifu_test_pop_byte_task(ifu_opcode);
                            st          = C_IFU_DECW;
                        end else if (nx_fast) begin
                            cen_ifu_fast = cen_ifu_fast + 64'd1;
                            // The prefetch buffer has the opcode, its
                            // entry and its operands: dispatch now. The
                            // bytes' references (the C model's) are
                            // booked after, in order: the first now, the
                            // rest from the queue.
                            ifu_opcode = pf_b0;
                            x_lh = pf_lh_c;
                            x_rh = pf_rh_c;
                            ifu_decode_task();
                            x_pc_after   = ifu_pcf + 16'(ifu_length);
                            x_op_faulted = 1'b0;
                            if (ifu_length >= 8'd2) ifu_alpha = pf_b1;
                            if (ifu_length >= 8'd3) ifu_beta  = pf_b2;
                            // one reference per WORD: two bytes in one word
                            // make the same reference twice in the C model
                            // (the same Pipe entry, LRU touch and Md), so
                            // the second is the first
                            mem_launch(DM_REF_IFETCH, ifu_va_fn(ifu_pcf), 16'd0, 8'd0, 4'd0, 2'd0, TAG_BK);
                            cen_ifu_fetch = cen_ifu_fetch + 64'd1;
                            if (!ifu_pcf[0] && ifu_length >= 8'd2) begin   // byte 1 shares the word
                                bk_pc = ifu_pcf + 16'd2;
                                bk_n  = 2'(ifu_length - 8'd2);
                            end else begin
                                bk_pc = ifu_pcf + 16'd1;
                                bk_n  = 2'(ifu_length - 8'd1);
                            end
                            ifu_end_task();          // sets x_np and Link
                            np = x_np;               // (C_NEXT's x_np = np below)
                            st = C_FINISH;
                        end else begin
                            if (mem_req && !mem_accepted)                      cen_nf[0] = cen_nf[0] + 64'd1;
                            else if (pf_pcf != ifu_pcf || pf_insset != ifu_insset) cen_nf[1] = cen_nf[1] + 64'd1;
                            else if (!pf_v0 || pf_va0 != ifu_va_fn(ifu_pcf))   cen_nf[3] = cen_nf[3] + 64'd1;
                            else if (!pf_ev_c || !pf_pres_c)                   cen_nf[2] = cen_nf[2] + 64'd1;
                            else if (pf_len >= 8'd2 && !pf_b1_ok)              cen_nf[4] = cen_nf[4] + 64'd1;
                            else if (pf_len >= 8'd3 && !pf_b2_ok)              cen_nf[5] = cen_nf[5] + 64'd1;
                            else if (pf_hazard)                                cen_nf[6] = cen_nf[6] + 64'd1;
                            else                                               cen_nf[7] = cen_nf[7] + 64'd1;
                            ifu_launch_fetch(ifu_pcf, C_IFU_DEC);
                        end
                    end
                end else if (jcn[6] == 1'b1) begin
                    // Return / Read TPC / Write TPC / Read IM / Write IM
                    case (jcn[5:3])
                        3'd0: begin // Return
                            np = {4'd0, link_at_issue[11:0]} |
                                 {15'd0, x_ff_cond_or} | dispatch_or;
                            if (!ff_loads_link_fn(u)) Link = real_PC + 16'd1;
                        end
                        3'd4: begin // RdTPC <- B
                            logic [3:0]  t_sel;
                            logic [15:0] tpc;
                            t_sel             = x_b[3:0];
                            tpc               = (t_sel == ctask) ? real_PC : task_tpc[t_sel];
                            Link              = real_PC + 16'd1;
                            b_link_read       = ~tpc;
                            b_link_read_valid = 1'b1;
                            b_link_read_age   = 1'b0;
                            np                = real_PC + 16'd1;
                        end
                        3'd5: begin // LdTPC <- B
                            logic [3:0] t_sel;
                            t_sel = x_b[3:0];
                            task_dispatch[t_sel] = 16'd0;
                            if (t_sel != ctask) task_tpc[t_sel] = Link;
                            Link = real_PC + 16'd1;
                            np   = real_PC + 16'd1;
                        end
                        3'd6: begin // Read IM (word at link_at_issue in im_q*)
                            logic [15:0] lane;
                            case (u.rstk[1:0])
                                2'd0: lane = {7'd0, im_q2[15], im_q0[15:8]};
                                2'd1: lane = {8'd0, im_q0[7:0]};
                                2'd2: lane = {7'd0, im_q1[15], im_q1[14:7]};
                                2'd3: lane = {8'd0, im_q1[6:0], im_q2[14]};
                            endcase
                            b_link_read       = ~lane;
                            b_link_read_valid = 1'b1;
                            b_link_read_age   = 1'b0;
                            Link              = real_PC + 16'd1;
                            np                = real_PC + 16'd1;
                        end
                        3'd7: begin // Write IM (committed in C_FINISH)
                            logic secondary;
                            secondary = u.rstk[1];
                            x_im_wr   = 1'b1;
                            x_im_wd0  = im_q0;
                            x_im_wd1  = im_q1;
                            x_im_wd2  = im_q2;
                            if (u.rstk[0]) begin
                                x_im_wd0     = x_b;
                                x_im_wd2[15] = secondary;
                            end else begin
                                x_im_wd1     = {secondary, x_b[15:1]};
                                x_im_wd2[14] = x_b[0];
                            end
                            Link = real_PC + 16'd1;
                            np   = real_PC + 16'd1;
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
                logic cond_val;
                eval_branch_condition_task(u, low3, cond_val);
                np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) |
                     {10'd0, jcn[6:5], 1'b0, jcn[4:3], cond_val} | dispatch_or;
                if (jcn[4:3] == 2'd0 && !ff_loads_link_fn(u)) begin
                    Link = real_PC + 16'd1;
                end
            end
        end
        x_np = np;
        // IM was read at pred_np this clock; C_FINISH can use it if
        // nothing else comes between
        x_pred_fresh = pred_ok && (st == C_FINISH);
        x_pred_np    = pred_np;
    endtask

    // C_FINISH's work (and C_SCHED's): the LC writeback, StkP, the flags, the
    // read-ahead for the next instruction, the schedule, the retire.
    task automatic finish_task(input logic is_finish, input logic is_sched, input logic from_nf, input logic pre_in);
        logic do_sched, spec_ok, pre;
        logic [3:0] ct0;
      if (is_finish && (io_busy_tick || ifu_bk_busy)) begin
        // the I/O engines are still on the last tick, or the last dispatch's
        // byte references are still owed: wait (the read-ahead stands)
        if (ifu_bk_busy) cen_bk_wait = cen_bk_wait + 64'd1;
      end else if (is_finish && x_pref_pending) begin
        x_pred_fresh = 1'b0;             // the reference is in flight: wait
      end else begin
        do_sched = 1'b1;
        spec_ok  = is_finish && !x_im_wr;
        ct0      = ctask;
        pre      = is_finish && pre_in;
        rm_pre_ok = 1'b0;
        if (pre) begin                    // the instruction read in C_NEXT
            ex_u    = cur_u;
            ex_pres = im_q_pres;
            ex_brk  = im_q_brk;
        end
        if (is_finish) begin
            dorado_uinstr_t u;
            u     = x_u;
            RBase = (ifu_dispatch_rbase >= 0) ? 8'(ifu_dispatch_rbase) : x_rbase_after_ff;

            if (!halted) begin
                logic [7:0]  rbase_next_instr;
                logic [15:0] pd;
                pd               = x_pd;
                rbase_next_instr = RBase;
                RBase            = x_rbase_at_issue;
                // RM/STK writes go through the block-RAM port (rms_*).
                case (u.lc)
                    3'd1: T = pd;
                    3'd2: T = x_md_at_issue;
                    3'd3: T = x_md_at_issue;
                    3'd5: T = ff_is_t_gets_md(u) ? x_md_at_issue : pd;
                    3'd7: T = pd;
                    default: ;
                endcase
                RBase = rbase_next_instr;

                if (u.block && ctask == 4'd0) begin
                    logic [7:0] d8;
                    d8 = ff_is_dont_modify_stkp(u) ? 8'd0 : stk_signed_delta(u.rstk);
                    if (d8 != 8'd0 || (u.rstk[3] && !ff_is_dont_modify_stkp(u))) begin
                        stk_und = x_stk_underflow;
                        stk_ovf = x_stk_overflow;
                        StkP    = {StkP[7:6], 6'(StkP[5:0] + d8[5:0])};
                    end
                end
                if (ifu_save_stkp_pending) begin
                    ifu_saved_stkp        = StkP;
                    ifu_save_stkp_pending = 1'b0;
                end

                if (!(ff_full_function_ok(u) && ff_fa(u.ff) == 2'd0 &&
                      ff_fb(u.ff) == 3'd7 && ff_fc(u.ff) == 3'd6)) begin
                    alu_zero     = x_new_alu_zero;
                    alu_lt0      = x_new_alu_lt0;
                    alu_carry    = x_new_carry;
                    alu_overflow = x_new_ovf;
                end

                prev_PC        = real_PC;
                x_sched_block  = u.block && ctask != 4'd0;
                x_sched_freeze = 1'b0;
            end else begin
                do_sched  = 1'b0;
                step_done = 1'b1;
                cmp_wait  = 1'b1;
                st        = C_FETCH;
            end
        end

        // 8. End-of-instruction task scheduling and ticks.
        if (do_sched) begin
            logic [15:0] chosen;
            if (x_from_im) junk_timer_tick_task();
            task_schedule_task(x_np, x_sched_block, x_sched_freeze, chosen);
            if (x_sched_freeze) begin
                cycles = cycles + 64'd1;         // hold: PC and memory timers stand still
                cen_hold = cen_hold + 64'd1;
            end else begin
                real_PC  = chosen;
                // IM was read at x_np (C_FINISH) or at this clock's prediction (C_NF)
                spec_hit = spec_ok && (chosen == (from_nf ? x_pred_np : x_np));
                // the read-ahead holds if it was the instruction chosen,
                // in the same task, at the RBase and StkP now in place
                rm_pre_ok = pre && spec_hit && (x_pred_np == x_np) && (ctask == ct0) &&
                            (RBase == fin_rbase) && (StkP == fin_stkp);
                if (spec_ok && !spec_hit) begin
                    if (ctask != ct0) cen_nospec[0] = cen_nospec[0] + 64'd1;
                    else              cen_nospec[1] = cen_nospec[1] + 64'd1;
                end
                if (pre && spec_hit && !rm_pre_ok) cen_nopre[0] = cen_nopre[0] + 64'd1;
                if (from_nf && x_pred_fresh && !pre_in) cen_nopre[1] = cen_nopre[1] + 64'd1;
                if (x_from_im) begin
                    cycles = cycles + 64'd1;
                    memory_tick();
                    cen_retire_task(x_u, ct0);
                end
                if (ifu_active && ifu_warmup > 8'd0) ifu_warmup = ifu_warmup - 8'd1;
            end
            step_done = 1'b1;
            cmp_wait  = 1'b1;
            st        = C_FETCH;
        end
        x_pred_fresh = 1'b0;
      end
    endtask

    // The end of an IFUJump dispatch (formerly the state C_IFU_END): the
    // dispatch address, PCF, and the IFU bookkeeping. Called by whichever
    // state finishes the dispatch (C_IFU_DEC, _AW, _BW, _END).
    task automatic ifu_end_task();
        if (x_op_faulted) begin
            x_np = ifu_trap_addr_fn(6'o00, x_n_slot);
            Link = real_PC + 16'd1;
        end else begin
            logic        emu_or_ft;
            logic [1:0]  n_eff;
            logic [15:0] tnia;
            emu_or_ft = (ctask == 4'd0) || (ctask == 4'o17);
            if (evc_b_enable && (evc_b_tasksall || emu_or_ft) &&
                evc_b_event == 3'd2) begin
                event_cnt_b = event_cnt_b + 16'd3;
            end
            evc_events |= EVC_EV_IFUJUMP;

            if (x_cond_true) begin
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
                ifu_pcf = x_pc_after;
            end

            ifu_idcnt             = 8'd0;
            ifu_save_stkp_pending = 1'b1;
            ifu_dispatch_count    = ifu_dispatch_count + 64'd1;
            cen_dispatch_task();

            n_eff = x_n_slot | {1'b0, x_cond_true};
            tnia  = (x_ifaddr << 2) | {14'd0, n_eff};
            if (x_resched_trap) tnia = ifu_trap_addr_fn(6'o14, x_n_slot);
            x_np = {4'd0, tnia[11:0]};
            Link = real_PC + 16'd1;
        end
    endtask

    // =========================================================================
    // The execution of a microinstruction (formerly the body of C_EXEC):
    // buses, ALU, FF functions, the processor memory reference. C_EXEC calls
    // it; so does C_FETCH, when the instruction and its RM/STK operand were
    // read ahead (see rm_pre_ok).
    // =========================================================================
    task automatic exec_task();
        dorado_uinstr_t u;
        logic           step_valid;
        logic           to_finish;

        // The I/O module ticked on step_done during C_FETCH; its
        // engines must be idle before this instruction runs.
        if (pexec_now ? !io_busy_tick : !io_busy) begin
        io_read_stb       = 1'b0;
        io_write_stb      = 1'b0;
        fastio_stb_r      = 1'b0;
        x_im_wr           = 1'b0;
        wakeup_pending   |= ext_wakeup_req;
        if (ext_subtask_we) begin
            task_subtask[ext_subtask_task] = ext_subtask_val;
        end
        u          = ex_u;
        x_u        = ex_u;
        x_from_im  = 1'b1;
        step_valid = 1'b1;
        to_finish  = 1'b0;
        st         = C_FETCH;

        if (bb_present) begin
            if (bb_ss_inject) begin
                x_from_im = 1'b0;
            end else if (!bb_running) begin
                cycles     = cycles + 64'd1;
                step_valid = 1'b0;
                step_done  = 1'b1;
                cmp_wait   = 1'b1;
            end
        end

        if (step_valid && x_from_im) begin
            if (real_PC >= IM_SIZE || !ex_pres) begin
                halted      = 1'b1;
                halt_reason = 4'd2; // CPU_HALT_NO_CODE
                step_valid  = 1'b0;
            end else if (ex_brk) begin
                halted      = 1'b1;
                halt_reason = 4'd7; // CPU_HALT_BREAKPOINT
                step_valid  = 1'b0;
            end
        end

        if (step_valid) begin
            logic        md_not_ready, hold_asserted;
            logic        stk_err;
            int          stk_delta, stk_off, stk_new_off;
            logic [15:0] a, b, alu, pd;
            logic        shift_active_cur;
            logic [1:0]  fa;
            logic [2:0]  fb, fc;
            logic        ff_exec_ok;

            // Hold check
            md_not_ready  = x_from_im && uinstr_reads_md_fn(u) &&
                            (cycles < task_md_ready[ctask]);
            hold_asserted = md_not_ready || holdsim_hold;
            if (hold_asserted) evc_events |= EVC_EV_HOLD;

            if (holdsim_hold && !mcr_dishold()) begin
                x_np           = real_PC;
                x_sched_block  = 1'b0;
                x_sched_freeze = 1'b1;
                st             = C_SCHED;
            end else begin
                // Stack underflow/overflow pre-check (cpu.c:5531)
                x_stk_underflow = 1'b0;
                x_stk_overflow  = 1'b0;
                stk_err         = 1'b0;
                if (u.block && ctask == 4'd0) begin
                    stk_delta = int'(signed'(stk_signed_delta(u.rstk)));
                    if (stk_delta != 0 || (u.rstk[3] && !ff_is_dont_modify_stkp(u))) begin
                        stk_off     = int'(StkP[5:0]);
                        stk_new_off = stk_off + stk_delta;
                        if (stk_new_off < 0)     x_stk_underflow = 1'b1;
                        if (stk_new_off > 8'o77) x_stk_overflow  = 1'b1;
                        if (u.rstk[3] && !ff_is_dont_modify_stkp(u) &&
                            ((stk_new_off & 63) == 0) && stk_new_off >= 0) begin
                            x_stk_underflow = 1'b1;
                        end
                        stk_err = x_stk_underflow || x_stk_overflow;
                    end
                end

                if (stk_err) begin
                    stk_und            = x_stk_underflow;
                    stk_ovf            = x_stk_overflow;
                    wakeup_pending[15] = 1'b1;
                    prev_PC            = real_PC;
                    x_np               = real_PC;
                    x_sched_block      = 1'b0;
                    x_sched_freeze     = 1'b0;
                    st                 = C_SCHED;
                end else begin
                    // -------------------------------------------------
                    // 1. Issue-Time Snapshots
                    // -------------------------------------------------
                    link_at_issue = Link;
                    if (b_link_read_valid) begin
                        if (b_link_read_age) b_link_read_valid = 1'b0;
                        else                 b_link_read_age   = 1'b1;
                    end
                    dispatch_or          = task_dispatch[ctask];
                    task_dispatch[ctask] = 16'd0;
                    x_md_at_issue        = cur_task_md;
                    x_rbase_at_issue     = RBase;
                    x_membase_at_issue   = MemBase[4:0];
                    io_atten_at_issue    = (io_attached && ctask != 4'd0) ? io_atten_in : 1'b0;

                    // -------------------------------------------------
                    // 2. Combinational Bus & ALU Results + Side Effects
                    // -------------------------------------------------
                    a                = comb_alu_a;
                    b                = comb_alu_b;
                    alu              = alu_result;
                    shift_active_cur = (u.asel == 3'd7);

                    if (ff_full_function_ok(u) && ff_fa(u.ff) == 2'd1) begin
                        if (ff_fb(u.ff) == 3'd6) begin
                            if (ff_fc(u.ff) == 3'd0) mem_fault_clear_stb = 1'b1;
                            if (u.bsel == 3'd3) Q = b;
                        end else if (ff_fb(u.ff) == 3'd7) begin
                            if (ff_fc(u.ff) == 3'd6) Link = b;
                            if (ff_fc(u.ff) == 3'd7) b_link_read_valid = 1'b0;
                            if (u.bsel == 3'd3) Q = b;
                        end
                    end

                    if (u.asel == 3'd5 ||
                        ((u.asel == 3'd2 || u.asel == 3'd3) &&
                         ff_decode_ok(u) && u.ff[7:6] == 2'd1)) begin
                        ifu_idcnt = ifu_idcnt + 8'd1;
                    end
                    x_ifetch_id = ifu_id_at_fn(ifu_idcnt);

                    x_alu_live_carry = alu_is_arith_out ? alu_carry_out : 1'b0;
                    x_new_carry      = alu_is_arith_out ? alu_carry_out : alu_carry;
                    x_new_ovf        = alu_is_arith_out ? alu_overflow_out : alu_overflow;
                    x_new_alu_zero   = (alu == 16'd0);
                    x_new_alu_lt0    = alu[15];

                    // -------------------------------------------------
                    // 3. FF Post-ALU Side Effects (ff_apply_post)
                    // -------------------------------------------------
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
                                    3'd1: pd = mem_read_map;
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
                                        pd = {x_alu_live_carry, alu[15:1]};
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
                                        cen_pcf    = cen_pcf + 64'd1;
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
                                    3'd2: begin mem_cflags_stb = 1'b1; mem_cflags_val = a; end
                                    3'd3: if (!mcr_disbr()) br[MemBase[4:0]][15:0]  = a;
                                    3'd4: if (!mcr_disbr()) br[MemBase[4:0]][27:16] = a[11:0];
                                    3'd6: mcr = (a & 16'hFFE0) | (b & 16'h0007);
                                    3'd7: proc_srn = b[3:0];
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
                                    3'd4, 3'd5: begin // IFUMRH / IFUMLH <- B
                                        x_ifum_wr    = 1'b1;
                                        x_ifum_hi    = (fc == 3'd4);
                                        x_ifum_waddr = {ifu_insset, ifu_opcode};
                                        x_ifum_wd    = b;
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
                                    3'd0: dmux_pending = 1'b0; // UseDMD
                                    3'd1: dmux_strobe(b);
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
                                        if (dmux_pending) begin
                                            dmux_read(pd);
                                        end else begin
                                            pd = alufm_present[u.aluf] ? {10'd0, alufm[u.aluf]} : 16'd0;
                                            if (fc == 3'd2) begin
                                                alufm[u.aluf]         = {b[7], b[4:0]};
                                                alufm_present[u.aluf] = 1'b1;
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
                                    3'd2: pd = {x_alu_live_carry, alu[15:1]};
                                    3'd3: pd = {alu[15], alu[15:1]};
                                    3'd4: pd = alu << 1;
                                    3'd5: pd = (alu << 1) | (alu >> 15);
                                    3'd6: begin // Divide
                                        logic q0;
                                        q0 = Q[15];
                                        Q  = {Q[14:0], x_alu_live_carry};
                                        pd = {alu[14:0], q0};
                                    end
                                    3'd7: begin // CDivide
                                        logic q0;
                                        q0 = Q[15];
                                        Q  = {Q[14:0], ~x_alu_live_carry};
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

                    x_a   = a;
                    x_b   = b;
                    x_alu = alu;
                    x_pd  = pd;
                    // to C_NF when IM was read at the prediction this clock
                    // (no branch-condition register to read first)
                    st    = next_reads_rm_fn(u) ? C_NEXTRD : (pred_ok ? C_NF : C_NEXT);
                    if (!pred_ok) cen_nonf[pred_why] = cen_nonf[pred_why] + 64'd1;
                    if (st == C_NF) begin
                        x_pred_fresh = 1'b1;
                        x_pred_np    = pred_np;
                        x_pred_np_x  = pred_np;
                    end else begin
                        x_pred_fresh = 1'b0;
                    end

                    // -------------------------------------------------
                    // 4. Processor Memory Reference (ASEL = 0..3)
                    // -------------------------------------------------
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
                                logic [4:0]  mb_eff;
                                logic [27:0] br_val, disp, va;

                                mb_eff = x_membase_at_issue;
                                if (ctask != 4'd0) begin
                                    mb_eff |= {2'b00, task_subtask[ctask], 1'b0};
                                end
                                br_val = mcr_disbr() ? 28'd0 : br[mb_eff];
                                if (kind == DM_REF_IFETCH) begin
                                    br_val = {br_val[27:8], x_ifetch_id[7:0]};
                                end
                                disp = (kind == DM_REF_LONGFETCH)
                                     ? {b[11:0], a}
                                     : {12'd0, a};
                                va = (br_val + disp) & 28'h0FFFFFFF;

                                x_fcnt_before = mem_fault_clear_stb ? 4'd0 : fault_count;
                                x_kind        = kind;
                                // returns straight to C_NEXT(RD); C_PREF's
                                // bookkeeping runs at the top of that clock
                                x_pref_pending = 1'b1;
                                mem_launch(kind, va, b, TIOA, ctask, task_subtask[ctask], TAG_PROC);
                            end
                        end
                    end
                end
            end
        end
        end // !io_busy
    endtask

    // =========================================================================
    // Main sequencer. One microinstruction is C_FETCH, C_EXEC, optional
    // memory/IFU states, C_FINISH; step_done pulses for one clock after each
    // completed microinstruction (the cosimulation and I/O tick on it).
    // =========================================================================
    logic ap_done_q;
    logic spec_hit;                // IM (read in C_FINISH) holds the instruction at real_PC
    assign cpu_between    = (st == C_FETCH);
    // the clock the instruction completes: C_FINISH, unless it is waiting
    // for its memory reference (x_pref_pending until mem_fin)
    assign io_tick_early  = clk_en && !halted &&
                            (((st == C_FINISH || st == C_NF ||
                               (st == C_PIPE && p_w && (!jcn_ifujump_fn(x_u) || !ifu_active ||
                                                        ifu_warmup > 8'd0 || nx_fast))) &&
                              !(x_pref_pending && !mem_fin) && !io_busy_tick &&
                              !ifu_bk_busy) || st == C_SCHED);
    assign dbg_ifu_active = ifu_active;
    assign dbg_st         = st;
    always @(posedge clk) begin
        x_ifum_wr = 1'b0;
        step_done = 1'b0;
        snap_valid = 1'b0;
        pexec_now  = 1'b0;
        ap_done_q = ap_done;
        if (st != C_FETCH && st != C_APW) ap_done = 1'b0;
        if (rst) begin
            reset_cpu();
            reset_memory();
            ap_done  = 1'b0;
            ap_done_q = 1'b0;
            spec_hit = 1'b0;
            ap_rd    = 16'd0;
            x_rmor   = 1'b0;
            p_w      = 1'b0;
            p_x      = 1'b0;
            p_d      = 1'b0;
            st      = C_FETCH;
            m_ret   = C_FETCH;
            x_im_wr = 1'b0;
        end else if (clk_en && !halted) begin
            do_exec = (st == C_EXEC) || fexec;
            if (((st == C_DEC || st == C_FETCH) && io_busy && touches_io_c) ||
                ((st == C_NF || st == C_FINISH) && io_busy_tick)) cen_iobusy = cen_iobusy + 64'd1;
            if (st == C_FETCH && ap_req && !ap_done_q)      cen_apwait = cen_apwait + 64'd1;
            // The memory section this clock: the request it took, the
            // booking it did (the MapBuf count follows the C model's pipe_push
            // and Map<- order), the reference it completed (C_PREF's work),
            // the munch it moved.
            pf_st0 = pf_st;
            if (cmp_done) cmp_wait = 1'b0;                   // it was seen after last clock
            if (mem_accepted) mem_req = 1'b0;
            if (mem_accepted2) mem_req2 = 1'b0;
            mem_fault_clear_stb = 1'b0;
            mem_cflags_stb      = 1'b0;
            // the next dispatched byte's reference (its address read ahead this clock)
            if (bk_n != 2'd0 && !mem_req) begin
                mem_launch(DM_REF_IFETCH, ifu_va_fn(bk_pc), 16'd0, 8'd0, 4'd0, 2'd0, TAG_BK);
                cen_ifu_fetch = cen_ifu_fetch + 64'd1;
                if (!bk_pc[0] && bk_n >= 2'd2) begin   // the next byte shares the word
                    bk_pc = bk_pc + 16'd2;
                    bk_n  = bk_n - 2'd2;
                end else begin
                    bk_pc = bk_pc + 16'd1;
                    bk_n  = bk_n - 2'd1;
                end
            end
            // the prefetch buffer: a peek's answer, and the lines it holds changing
            if (mem_peek_done) begin
                mem_req2 = 1'b0;
                if (mem_peek_retry) begin
                    cen_peek_retry = cen_peek_retry + 64'd1;
                    if (pf_st == 3'd1) pf_st = 3'd0;
                    if (pf_st == 3'd3) pf_st = 3'd2;
                end else if (pf_st == 3'd1) begin
                    if (mem_peek_hit) begin
                        pf_w0 = md; pf_va0 = mem_req2_va; pf_row0 = mem_req2_va[9:4];
                        pf_way0 = mem_res_way; pf_word0 = mem_req2_va[3:0];
                        pf_v0 = 1'b1; pf_st = 3'd2;
                        pf_ifum_wait = 1'b1;     // IFUM is addressed from it this clock (ifum_addr2)
                        pf_w0_new    = 1'b1;     // ...so the entry arrives next clock, not this one
                        if (pf_issue1_c) begin   // word1's peek, now
                            mem_req2    = 1'b1;
                            mem_req2_va = (mem_req2_va + 28'd1) & 28'h0FFFFFFF;
                            pf_st       = 3'd3;
                            cen_peek    = cen_peek + 64'd1;
                        end
                    end else begin
                        pf_st = 3'd5;            // not resident: the dispatch's own fetch fills it
                        cen_peek_miss = cen_peek_miss + 64'd1;
                    end
                end else if (pf_st == 3'd3) begin
                    if (mem_peek_hit) begin
                        pf_w1 = md; pf_va1 = mem_req2_va; pf_row1 = mem_req2_va[9:4];
                        pf_way1 = mem_res_way; pf_word1 = mem_req2_va[3:0];
                        pf_v1 = 1'b1;
                    end
                    pf_st = 3'd4;
                end
            end
            if (mem_snoop_row_stb) begin
                if (pf_v0 && mem_snoop_row == pf_row0) pf_v0 = 1'b0;
                if (pf_v1 && mem_snoop_row == pf_row1) pf_v1 = 1'b0;
            end
            if (mem_snoop_wr) begin
                if (pf_v0 && mem_snoop_wr_addr == {pf_row0, pf_word0} && mem_snoop_wr_way == pf_way0) pf_v0 = 1'b0;
                if (pf_v1 && mem_snoop_wr_addr == {pf_row1, pf_word1} && mem_snoop_wr_way == pf_way1) pf_v1 = 1'b0;
            end
            if (x_pref_pending && mem_fin) begin
                mem_pref_task();
                x_pref_pending = 1'b0;
            end
            if (mem_fastio_set) fastio_stb_r = 1'b1;

            // The execute, before the sequencer's own work in this clock (so
            // the scheduler, in C_NF, never feeds it: no false path through
            // ctask, MemBase or T into the VA adder). C_FETCH's own item, after,
            // only clears the read-ahead flags when it executed.
            if (fexec) begin
                fx_stkp = StkP;                  // what the condition read used
                fx_sub  = task_subtask[ctask];
            end
            st0 = st;
            if (do_exec && !PIPELINE) begin
                exec_task();
                // fexec read the branch-condition register this clock (unless
                // the instruction moved StkP or the subtask)
                if (fexec && st == C_NEXTRD && StkP == fx_stkp && task_subtask[ctask] == fx_sub) begin
                    // ...and with the operand read ahead the condition was in
                    // rm_q in this clock: the prediction stood and IM was
                    // read at it (pred_ok), so C_NF can follow directly
                    // (not when a reference is outstanding: C_NF would wait,
                    // and its RM port moves on to the next instruction's
                    // operand, so the condition would be re-read wrong)
                    if (pred_ok && !x_pref_pending) begin
                        st           = C_NF;
                        x_pred_fresh = 1'b1;
                        x_pred_np    = pred_np;
                        x_pred_np_x  = pred_np;
                    end else begin
                        st = C_NEXT;
                    end
                end
            end
            case (st0)
            // -----------------------------------------------------------------
            C_FETCH: begin
                // IM, IFUM and the Map entry at Mar are being read this clock.
                ex_u    = cur_u;
                ex_pres = im_q_pres;
                ex_brk  = im_q_brk;
                ap_done = 1'b0;
                if (ap_req && !ap_done_q && !ifu_bk_busy) begin
                    spec_hit  = 1'b0;            // the assist uses the ports
                    rm_pre_ok = 1'b0;            // and may change RM/STK
                    if (ap_stk) begin
                        ap_done = 1'b1;          // the STK port writes this clock
                    end else if (ap_op == AP_WAKEUP_OR) begin
                        wakeup_pending = wakeup_pending | ap_wd;   // dorado_cpu_wakeup
                        ap_done = 1'b1;
                    end else if (ap_op == AP_RESCHED) begin
                        reschedule_pending = ap_wd[7:0];
                        ap_done = 1'b1;
                    end else if (ap_op == AP_RM_OR) begin
                        x_rmor = 1'b1;           // RM read this clock, written in C_APW
                        st     = C_APW;
                    end else begin
                        mem_ap_req = 1'b1;       // the memory section serves it
                        st         = C_APW;
                    end
                end else if (!ap_hold) begin
                    if (fexec) begin
                        // read ahead: ex_u (latched in C_FINISH) and its RM/STK
                        // operand are here; it executed this clock (above)
                        spec_hit  = 1'b0;
                        rm_pre_ok = 1'b0;
                    end else if (ifu_bk_gate) begin
                        // the dispatched bytes' references book first (the
                        // read-ahead stands: the operand is re-read at the
                        // same address)
                        cen_bk_wait = cen_bk_wait + 64'd1;
                    end else begin
                        // With the instruction already read (spec_hit), RM/STK
                        // are read at its address this clock and C_DEC is not
                        // needed.
                        st        = spec_hit ? C_EXEC : C_DEC;
                        spec_hit  = 1'b0;
                        rm_pre_ok = 1'b0;
                    end
                end
            end

            // RM/STK are being read at the decode-time address this clock.
            // The I/O module ticks on step_done during C_FETCH; its engines
            // (io_busy) must be idle before C_EXEC reads I/O state.
            C_DEC: begin
                ex_u    = cur_u;
                ex_pres = im_q_pres;
                ex_brk  = im_q_brk;
                if (!(io_busy && touches_io_c) && !ifu_bk_gate) st = C_EXEC;
                else if (!(io_busy && touches_io_c)) cen_bk_wait = cen_bk_wait + 64'd1;
            end

            // -----------------------------------------------------------------
            C_EXEC: ;                            // executes below

            // RM/STK are being read at the branch-condition address this clock.
            C_NEXTRD: st = C_NEXT;

            // -----------------------------------------------------------------
            // 5. Next-PC & IFUJump Evaluation (next_pc)
            // -----------------------------------------------------------------
            // The next address (next_pc), then C_FINISH; or the IFU slow path.
            C_NEXT: next_task();

            // The next address and the finish in one clock: the execute read IM
            // at its prediction, so the instruction after is here (cur_u) for
            // the read-ahead, and the finish runs now. A reference still in
            // flight holds the whole clock (nothing of it runs twice).
            C_NF: begin
                if (ifu_bk_busy) cen_bk_wait = cen_bk_wait + 64'd1;
                if (!x_pref_pending && !io_busy_tick && !ifu_bk_busy) begin
                    logic pre0;
                    logic [3:0] ct_s;
                    logic       xplain, nplain;
                    pre0 = x_pred_fresh && (x_pred_np_x == pred_np);   // the execute's read is this clock's prediction
                    ct_s   = ctask;
                    xplain = !(x_u.jcn[2:0] == 3'd7 && x_u.jcn[7:4] != 4'd0 && x_u.jcn[7] == 1'b0 &&
                               !(x_u.jcn[7:5] != 3'd1 && x_u.jcn[6] && x_u.jcn[5:3] == 3'd0)) &&
                             !(x_u.jcn == 8'o045 && x_u.ff == 8'o147);
                    nplain = !uinstr_touches_io_fn(cur_u, ctask != 4'd0) && !uinstr_reads_id_fn(cur_u) &&
                             !jcn_reads_im_at_link_fn(cur_u) && !next_reads_rm_fn(cur_u) &&
                             !(ap_req && !ap_done_q) && !ap_hold;
                    next_task();
                    if (st == C_FINISH) finish_task(1'b1, 1'b0, 1'b1, pre0);
                    if (pent) begin
                        // IM was read at the instruction after cur_u: C_PIPE
                        // takes cur_u (latched in ex_u, its operand read now)
                        if (st == C_FETCH && spec_hit && rm_pre_ok) begin
                            st      = C_PIPE;
                            p_w     = 1'b0;
                            p_x     = 1'b1;
                            p_d     = 1'b1;
                            p_pc_x  = real_PC;
                            p_pc_d  = f_np;
                            p_rb_d  = f_rb;
                            p_raddr = rms_raddr;
                            p_rb_x  = fin_rbase;
                            p_sp_x  = fin_stkp;
                            cen_pipe[4] = cen_pipe[4] + 64'd1;
                        end else begin
                            cen_pipe[5] = cen_pipe[5] + 64'd1;
                        end
                        spec_hit  = 1'b0;
                        rm_pre_ok = 1'b0;
                    end
                    if (st == C_FETCH) begin
                        cen_ov[0] = cen_ov[0] + 64'd1;
                        if (spec_hit && rm_pre_ok) begin
                            cen_ov[1] = cen_ov[1] + 64'd1;
                            if (ctask == ct_s && xplain) begin
                                cen_ov[2] = cen_ov[2] + 64'd1;
                                if (nplain) begin
                                    cen_ov[3] = cen_ov[3] + 64'd1;
                                    if (!(cur_u.asel == 3'd0 || cur_u.asel == 3'd1 || cur_u.asel == 3'd4 ||
                                          cur_u.asel == 3'd7 || cur_u.bsel == 3'd1)) begin
                                        cen_ov[4] = cen_ov[4] + 64'd1;
                                    end
                                end
                            end
                        end
                    end
                end
            end


            // The pipeline (PIPELINE): x_u's finish, ex_u's execute (after it,
            // in this clock: the C model's order), cur_u's operand read, the
            // IM read after it. Anything it does not take drains it: the
            // finish runs alone and C_FETCH reads IM at real_PC again.
            C_PIPE: begin
                cen_pipe[0] = cen_pipe[0] + 64'd1;
                if (p_stall) begin
                    cen_pipe[2] = cen_pipe[2] + 64'd1;   // the finish waits: nothing moves
                    if (x_pref_pending && !mem_fin) cen_pdrain[6] = cen_pdrain[6] + 64'd1;
                    else if (ifu_bk_busy)           cen_pdrain[7] = cen_pdrain[7] + 64'd1;
                    else                            cen_pdrain[8] = cen_pdrain[8] + 64'd1;
                end else begin
                    if (p_w) begin
                        // the I/O tick before x_u's finish: its wakeups and
                        // subtask write (cpu.c applies them at the execute,
                        // which here ran beside the tick, before its outputs)
                        wakeup_pending |= ext_wakeup_req;
                        if (ext_subtask_we) task_subtask[ext_subtask_task] = ext_subtask_val;
                        next_task();
                        if (st == C_FINISH) finish_task(1'b1, 1'b0, 1'b0, 1'b0);
                        if (ovx && (x_np != p_pc_x || ctask != 4'd0))
                            $display("[C_PIPE] finish chose 0%o task %0d, the pipeline has 0%o", x_np, ctask, p_pc_x);
                    end
                    if (ovx) begin
                        cen_pipe[1] = cen_pipe[1] + 64'd1;
                        if (p_w) begin
                            cen_pipe[7]   = cen_pipe[7] + 64'd1;
                            snap_valid    = 1'b1;
                            snap_real_PC  = real_PC;  snap_ctask = ctask;  snap_ready = ready;
                            snap_T        = T;        snap_Q     = Q;      snap_Link  = Link;
                            snap_Cnt      = Cnt;      snap_ShC   = ShC;    snap_StkP  = StkP;
                            snap_RBase    = RBase;    snap_MemBase = MemBase; snap_MemBX = MemBX;
                            snap_flags    = {alu_zero, alu_lt0, alu_carry, alu_overflow};
                            snap_mcr      = mcr;      snap_proc_srn = proc_srn;
                            for (int i = 0; i < 32; i++) snap_br[i] = br[i];
                            if (p_fwd != (rms_we && rms_waddr == p_raddr))
                                $display("[C_PIPE] the registered operand forward differs at 0%o", real_PC);
                            // the execute's own view (below) must be what the finish left
                            if (real_PC != pc_p || ctask != 4'd0 || T != T_x || Link != Link_x ||
                                StkP != sp_wx || RBase != dr_rb || MemBase != membase_x || cycles != cycles_x ||
                                {alu_zero, alu_lt0, alu_carry, alu_overflow} != {zero_x, lt0_x, carry_x, ovf_x})
                                $display("[C_PIPE] the forwarded view differs from the finish at 0%o", real_PC);
                        end
                        // the execute runs after the case (below)
                    end else if (p_bub && !p_w) begin
                        cen_pipe[2] = cen_pipe[2] + 64'd1;   // ex_u waits (nothing moves)
                    end else if (p_bub && st == C_FETCH && real_PC == p_pc_x && ctask == 4'd0 &&
                                 RBase == p_rb_x && StkP == p_sp_x) begin
                        // x_u's finish left the PC, task, RBase and StkP the
                        // pipeline read ahead for (an IFUJump's dispatch
                        // went where it fetched): ex_u executes next clock
                        // (its operand and cur_u held this clock)
                        if (jcn_ifujump_fn(x_u)) cen_pipe[9] = cen_pipe[9] + 64'd1;
                        st        = C_PIPE;
                        p_w       = 1'b0;
                        spec_hit  = 1'b0;
                        rm_pre_ok = 1'b0;
                    end else begin
                        // drain: ex_u to C_FETCH, which executes it (fexec)
                        // when the finish left the task and the PC alone; IM
                        // and its operand are read again this clock
                        cen_pipe[8] = cen_pipe[8] + 64'd1;
                        begin
                            int r;
                            if (!p_x) r = 0;
                            else if (ctask != 4'd0) r = 9;
                            else if (uinstr_touches_io_fn(ex_u, 1'b0)) r = 1;
                            else if (jcn_reads_im_at_link_fn(ex_u) || !(jcn_plain_fn(ex_u) || jcn_ifujump_fn(ex_u))) r = 4;
                            else if (next_reads_rm_fn(ex_u)) r = 3;
                            else if (p_xd[8] && p_xd[7:6] == 2'd1 && p_xd[5:3] >= 3'd6) r = 5;
                            else if (x_host) r = 10;
                            else if (!ex_pres || ex_brk || bb_present) r = 2;
                            else r = 11;
                            cen_pdrain[r] = cen_pdrain[r] + 64'd1;
                        end
                        if (st == C_PIPE) st = C_FETCH;
                        spec_hit  = p_x && !p_bub && (st == C_FETCH) && (real_PC == p_pc_x) && (ctask == 4'd0);
                        rm_pre_ok = spec_hit && (!p_w || (RBase == dr_rb && StkP == dr_sp));
                        p_w = 1'b0; p_x = 1'b0; p_d = 1'b0;
                    end
                end
            end

            // -----------------------------------------------------------------
            // IFUJump: opcode fetch returned
            // -----------------------------------------------------------------
            // The IFU's byte is in the section (mem_fin: this clock or
            // earlier). The opcode (m_ret == C_IFU_DEC): IFUM is being read
            // for it this clock; alpha and beta: their states take it.
            C_IFU_W: begin
                if (ifu_dl_valid && !mem_req) begin          // the deferred launch
                    mem_launch(DM_REF_IFETCH, ifu_dl_va, 16'd0, 8'd0, 4'd0, 2'd0, TAG_IFU);
                    ifu_dl_valid = 1'b0;
                end else if (!ifu_dl_valid && mem_done && mem_res_tag == TAG_IFU &&
                             !(mem_req && !mem_accepted)) begin
                    if (mem_res_miss) evc_events |= EVC_EV_MISS;
                    if (m_ret != C_IFU_DEC) begin
                        st = m_ret;
                    end else if (mem_res_fault != DM_FAULT_NONE) begin
                        x_np = ifu_trap_addr_fn(6'o00, x_n_slot);
                        if (!ff_loads_link_fn(x_u)) Link = real_PC + 16'd1;
                        st = C_FINISH;
                    end else begin
                        ifu_opcode = ifu_fetched_byte();
                        st         = C_IFU_DEC;
                    end
                end
            end

            // IFUM is being read at {InsSet, opcode} this clock.
            C_IFU_DECW: st = C_IFU_DEC;

            C_IFU_DEC: begin
                if (!ifum_q_pres) begin
                    halted      = 1'b1;
                    halt_reason = 4'd11; // CPU_HALT_IFU_NO_ENTRY
                    st          = C_FINISH;
                end else begin
                    x_lh = ifum_q_hi;
                    x_rh = ifum_q_lo;
                    ifu_decode_task();

                    x_pc_after   = ifu_pcf + 16'd1;
                    x_op_faulted = 1'b0;
                    st           = C_IFU_END;            // one byte: nothing to fetch
                    if (ifu_length >= 8'd2) begin
                        if (x_test_feed && ifu_test_count > 4'd0) begin
                            ifu_test_pop_byte_task(ifu_alpha);
                            x_pc_after = x_pc_after + 16'd1;
                            st         = C_IFU_BETA;
                        end else begin
                            ifu_launch_fetch(x_pc_after, C_IFU_AW);
                        end
                    end
                    if (st == C_IFU_END) begin           // one byte: end the dispatch now
                        ifu_end_task();
                        st           = C_FINISH;
                        x_pred_fresh = pred_ok;
                        x_pred_np    = pred_np;
                    end
                end
            end

            C_IFU_AW: begin
                if (mem_res_fault != DM_FAULT_NONE) x_op_faulted = 1'b1;
                ifu_alpha  = ifu_fetched_byte();
                x_pc_after = x_pc_after + 16'd1;
                // C_IFU_BETA's work, in this clock (map_addr reads for x_pc_after)
                st = C_IFU_END;
                if (ifu_length >= 8'd3 && !x_op_faulted) begin
                    if (x_test_feed && ifu_test_count > 4'd0) begin
                        ifu_test_pop_byte_task(ifu_beta);
                        x_pc_after = x_pc_after + 16'd1;
                    end else begin
                        ifu_launch_fetch(x_pc_after, C_IFU_BW);
                    end
                end
                if (st == C_IFU_END) begin               // end the dispatch now
                    ifu_end_task();
                    st           = C_FINISH;
                    x_pred_fresh = pred_ok;
                    x_pred_np    = pred_np;
                end
            end

            C_IFU_BETA: begin
                st = C_IFU_END;
                if (ifu_length >= 8'd3 && !x_op_faulted) begin
                    if (x_test_feed && ifu_test_count > 4'd0) begin
                        ifu_test_pop_byte_task(ifu_beta);
                        x_pc_after = x_pc_after + 16'd1;
                    end else begin
                        ifu_launch_fetch(x_pc_after, C_IFU_BW);
                    end
                end
            end

            C_IFU_BW: begin
                if (mem_res_fault != DM_FAULT_NONE) x_op_faulted = 1'b1;
                ifu_beta   = ifu_fetched_byte();
                x_pc_after = x_pc_after + 16'd1;
                ifu_end_task();                          // end the dispatch now
                st           = C_FINISH;
                x_pred_fresh = pred_ok;
                x_pred_np    = pred_np;
            end

            C_IFU_END: begin
                ifu_end_task();
                st = C_FINISH;
                x_pred_fresh = pred_ok;          // IM read at pred_np this clock
                x_pred_np    = pred_np;
            end

            // -----------------------------------------------------------------
            // 6-8. LC writeback, StkP, flags, scheduling (Write IM commits now)
            // -----------------------------------------------------------------
            // C_FINISH runs the end-of-instruction scheduling in the same
            // clock; C_SCHED alone serves the stack-error and HOLDSIM paths.
            C_FINISH, C_SCHED: finish_task(st == C_FINISH, st == C_SCHED, 1'b0, x_pred_fresh);

            // The assist: an RM/STK word (read this clock, x_rmor) or the
            // section's word (mem_ap_done)
            C_APW: begin
                if (x_rmor) begin
                    ap_rd   = rm_q;
                    ap_done = 1'b1;
                    x_rmor  = 1'b0;
                    st      = C_FETCH;
                end else if (mem_ap_done) begin
                    mem_ap_req = 1'b0;
                    ap_rd      = mem_ap_rd;
                    ap_done    = 1'b1;
                    st         = C_FETCH;
                end
            end

            default: st = C_FETCH;
            endcase

            // (PIPELINE) The execute, for C_EXEC, a C_FETCH that executes and
            // C_PIPE, from its own view of the registers: as the clock found
            // them, with C_PIPE's finish forwarded (what the finish left, and
            // the check above says so). The finish, the scheduler and C_NF's
            // work are then not in the execute's paths.
            if (PIPELINE && (do_exec || ovx)) begin
                logic was_ovx;
                was_ovx  = ovx;
                ctask    = ctask_x;
                ex_u     = ex_u_q;      // (a C_FETCH that executes: cur_u, the same word)
                ex_pres  = ex_pres_q;
                ex_brk   = ex_brk_q;
                ifu_idcnt = idcnt_q;
                Cnt      = Cnt_q;
                b_link_read_valid = blr_valid_q;
                b_link_read_age   = blr_age_q;
                real_PC  = pc_p;
                cycles   = cycles_x;
                T        = T_x;
                Link     = Link_x;
                StkP     = sp_wx;
                RBase    = dr_rb;
                MemBase  = membase_x;
                {alu_zero, alu_lt0, alu_carry, alu_overflow} = {zero_x, lt0_x, carry_x, ovf_x};
                if (was_ovx) fx_stkp = StkP;
                pexec_now = was_ovx;
                exec_task();
                pexec_now = 1'b0;
                x_disp_v  = jcn_ifujump_fn(x_u) && nx_fast;
                x_disp_rb = {7'd0, ~pf_lh_c[9]};
                if (fexec && st == C_NEXTRD && StkP == fx_stkp && task_subtask[ctask] == fx_sub) begin
                    if (pred_ok && !x_pref_pending) begin
                        st           = C_NF;
                        x_pred_fresh = 1'b1;
                        x_pred_np    = pred_np;
                        x_pred_np_x  = pred_np;
                    end else begin
                        st = C_NEXT;
                    end
                end
                if (was_ovx) begin
                    if (!p_d) begin
                        begin
                            int r;
                            logic [7:0] j;
                            j = x_u.jcn;      // (exec_task made ex_u x_u)
                            if (j[7:4] != 4'd0 && !j[7] && j[2:0] != 3'd7) r = 0;
                            else if (x_u.bsel < 3'd4 && j[7:4] != 4'd0 &&
                                     ((x_u.asel <= 3'd3) ? ((x_u.ff[5:0] & 6'o70) == 6'o60)
                                                         : (ff_fa(x_u.ff) == 2'd0 && ff_fb(x_u.ff) == 3'd6))) r = 1;
                            else if (j[2:0] == 3'd7 && j[7:5] == 3'd1) r = 2;
                            else if (j[2:0] == 3'd7 && j[6] && j[5:3] == 3'd0) r = 3;
                            else r = 4;
                            cen_pnof[r] = cen_pnof[r] + 64'd1;
                        end
                        // a condition on ex_u's own register: read this
                        // clock (p_raddr), so C_NF can follow, as fexec
                        if (st == C_NEXTRD && StkP == fx_stkp && pred_ok && !x_pref_pending) begin
                            st           = C_NF;
                            x_pred_fresh = 1'b1;
                            x_pred_np    = pred_np;
                            x_pred_np_x  = pred_np;
                        end
                        // the instruction after ex_u was not fetched: the
                        // FSM takes it from here (IM was read at pred_np)
                        cen_pipe[3] = cen_pipe[3] + 64'd1;
                        p_w = 1'b0; p_x = 1'b0;
                    end else if (st == C_NF || st == C_NEXT || st == C_NEXTRD) begin
                        st      = C_PIPE;
                        p_w     = 1'b1;
                        p_x     = d_ok;
                        p_d     = f_ok;
                        ex_u    = cur_u;
                        ex_pres = im_q_pres;
                        ex_brk  = im_q_brk;
                        p_pc_x  = p_pc_d;
                        p_pc_d  = f_np;
                        p_rb_d  = f_rb;
                        p_raddr = d_raddr;
                        p_rb_x  = d_rb;
                        p_sp_x  = d_sp;
                    end else begin
                        cen_pipe[6] = cen_pipe[6] + 64'd1;   // a hold, a stack error, a halt
                        p_w = 1'b0; p_x = 1'b0; p_d = 1'b0;
                        x_pred_fresh = 1'b0;
                    end
                    if (st == C_PIPE) x_pred_fresh = 1'b0;
                    spec_hit  = 1'b0;
                    rm_pre_ok = 1'b0;
                end
            end

            // One instance of the execution logic, for C_EXEC and for a
            // C_FETCH that executes (fexec): calling it from both case items
            // built the whole datapath twice.
            // The prefetch buffer: for ifu_pcf (restarting when it moves), two
            // words by peeks when nothing else wants the section, then the
            // opcode's IFUM entry through the second port.
            pf_restart = 1'b0;
            if (pf_pcf != ifu_pcf || pf_insset != ifu_insset) begin
                // PCF moved (a dispatch, mostly): the words are kept when
                // they still hold the opcode -- word0 if the new PC is in
                // it, word1 moved down if it is there -- and only the entry
                // is re-read (IFUM is addressed from the new byte next clock).
                logic [27:0] nva0;
                nva0      = ifu_va_fn(ifu_pcf);
                pf_pcf    = ifu_pcf;
                pf_insset = ifu_insset;
                pf_ev = 1'b0; pf_ifum_wait = 1'b0;
                pf_restart = 1'b1;
                if (pf_v0 && pf_va0 == nva0 && (pf_st == 3'd2 || pf_st == 3'd3 || pf_st == 3'd4)) begin
                    if (!(pf_v1 && pf_st == 3'd4 && pf_va1 == ((nva0 + 28'd1) & 28'h0FFFFFFF))) pf_v1 = 1'b0;
                    pf_st = pf_v1 ? 3'd4 : 3'd2;
                end else if (pf_v1 && pf_st == 3'd4 && pf_va1 == nva0) begin
                    pf_w0 = pf_w1; pf_va0 = pf_va1; pf_row0 = pf_row1; pf_way0 = pf_way1; pf_word0 = pf_word1;
                    pf_v0 = 1'b1; pf_v1 = 1'b0;
                    pf_st = 3'd2;
                end else begin
                    pf_v0 = 1'b0; pf_v1 = 1'b0;
                    pf_st = 3'd0;
                end
            end
            if (!pf_v0 && (pf_st == 3'd2 || pf_st == 3'd4)) begin   // word0 dropped: start over
                pf_st = 3'd0; pf_ev = 1'b0; pf_v1 = 1'b0; pf_ifum_wait = 1'b0;
            end else if (!pf_v1 && pf_st == 3'd4) begin
                pf_st = 3'd2;                                       // word1 dropped: again
            end
            // the peek decided above (pf_issue_c), unless this clock moved the
            // buffer on (restart, or a word dropped)
            if (pf_issue_c && !pf_restart && pf_st == pf_st0) begin
                mem_req2    = 1'b1;
                mem_req2_va = mem_spec2_va;
                pf_st       = (pf_st == 3'd0) ? 3'd1 : 3'd3;
                cen_peek    = cen_peek + 64'd1;
            end
            if (pf_ifum_wait && !pf_w0_new) begin
                pf_lh = ifum_q2_hi; pf_rh = ifum_q2_lo; pf_pres = ifum_q2_pres;
                pf_ev = 1'b1; pf_ifum_wait = 1'b0;
            end else if (pf_v0 && !pf_ev && !pf_ifum_wait && !pf_restart) begin
                pf_ifum_wait = 1'b1;                                // addressed this clock (ifum_addr2, from the held word)
            end
            pf_w0_new = 1'b0;
            if (x_ifum_wr) begin pf_ev = 1'b0; pf_ifum_wait = 1'b0; end


            // The section's booking this clock, into the MapBuf count, after
            // the sequencer's work and the execute (so nothing of either reads
            // the section's hit test in the same clock; neither runs in a
            // booking's clock anyway, and a tick before the booking leaves the
            // count's final state the same).
            if (mem_book_now) begin
                if (mem_book_push) pipe_mapbuf_busy[mem_book_srn] = 1'b0;
                if (mem_book_is_map) begin
                    pipe_mapbuf_busy[mem_book_srn] = 1'b1;
                    mapbuf_busy_slot   = int'(mem_book_srn);
                    mapbuf_busy_cycles = 9;
                end
            end
        end
        // Id from the IFU state this clock leaves: what the next execute
        // reads (a register, so the operand selection is not in the path from
        // ifu_idcnt to Pd; C_PIPE forwards a dispatch beside the execute)
        id_r0 = ifu_id_at_fn(ifu_idcnt);
        id_r1 = ifu_id_at_fn(ifu_idcnt + 8'd1);
        // the compare C_PIPE's operand forward uses next clock (rms_we and
        // rms_waddr as the finish of x_u will have them)
        p_fwd = !halted && (x_u.lc == 3'd2 || x_u.lc == 3'd4 || x_u.lc == 3'd5 ||
                            x_u.lc == 3'd6 || x_u.lc == 3'd7) &&
                lc_write_address_fn(x_u, ctask, x_rbase_at_issue, task_subtask[ctask], StkP) == p_raddr;
    end

endmodule
/* verilator lint_on BLKSEQ */
