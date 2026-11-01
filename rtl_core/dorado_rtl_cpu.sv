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
    parameter bit          READAHEAD     = 1'b1       // C_FETCH may execute a read-ahead instruction
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
    // The clock in which a microinstruction completes through C_FINISH or
    // C_SCHED: the I/O module ticks in it (dorado_rtl_top), so its results
    // are in place when the next one executes, even in the next clock.
    output logic           io_tick_early,
    input  logic           io_busy,    // I/O not ready for the next microinstruction

    // Loader assist port (dorado_rtl_assist): while ap_hold is high the CPU
    // stays between microinstructions, and services one ap_req at a time:
    // a word written to (or read from) storage at the mapped VA, optionally
    // also into a resident cache line, or a word written to STK. This is what
    // the cosimulation harness does by poking arrays; see dorado_rtl_assist.
    // ap_op selects the host-port operations (AP_* in dorado_rtl_pkg) that
    // stand for the C machine's host services (Cedar on the HPS); 0 is the
    // assist's own storage/STK access described above.
    input  logic           ap_hold,
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
        C_FETCH, C_DEC, C_EXEC, C_PREF, C_NEXTRD, C_NEXT, C_SCHED,
        C_IFU_OPW, C_IFU_DECW, C_IFU_DEC, C_IFU_AW, C_IFU_BETA, C_IFU_BW, C_IFU_END,
        C_FINISH,
        M_ROW, M_MAPA, M_MAPV, M_BOOK, A_DO, A_WAIT, M_WRA, M_WRV, M_WB, M_FILL, M_RD, M_WR,
        M_IOC, M_IOS, M_IOW, M_STR, M_STW
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
    logic           x_assist;      // the M_ROW/M_MAPA/M_MAPV pass serves an assist request
    logic           ap_hit;
    logic [1:0]     ap_way;
    logic [31:0]    ap_phys;
    logic           x_rmor;        // an AP_RM_OR request is in A_WAIT
    logic           x_ifum_wr, x_ifum_hi;
    logic [9:0]     x_ifum_waddr;
    logic [15:0]    x_ifum_wd;


    // =========================================================================
    // Memory Subsystem (BR, Map, Cache, Pipe, MSA) -- merged from
    // dorado_rtl_mem.sv so that one clocked block owns every register.
    // Derived from dorado/src/memory.c. HM section 5.
    //
    // The bookkeeping state (tags, flags, LRU, Pipe, FaultInfo, BR, MCR) lives
    // in registers and is updated in one clock by mem_book_task(). The large
    // arrays -- the Map (64K entries), cache data (4K words) and storage --
    // are single-port block RAMs, and the line moves the C model does in zero
    // time (writeback, fill, munch transfer) are sequenced over several clocks
    // by the M_* states while the microinstruction waits. That is invisible to
    // the microcode: it changes how many fabric clocks a microinstruction
    // takes, not what it computes.
    // =========================================================================
    localparam logic [15:0] PIPE5_MAPBUF_BUSY  = 16'h8000;
    localparam logic [15:0] PIPE5_DIRTY        = 16'h0080;
    localparam logic [15:0] PIPE5_VACANT       = 16'h0040;
    localparam logic [15:0] PIPE5_WP           = 16'h0020;
    localparam logic [15:0] PIPE5_BEING_LOAD   = 16'h0010;
    localparam int          PIPE5_COLUMN_SHIFT = 8;

    localparam logic [7:0]  PIPE4_ERR_MAP_TROUBLE = 8'h01;
    localparam logic [7:0]  PIPE4_ERR_MEM_ERROR   = 8'h02;
    localparam logic [7:0]  PIPE4_ERR_EC_FAULT    = 8'h04;

    localparam int CHIP_TYPE  = 3;   // 64K chips
    localparam int PAGE_SHIFT = 8;   // 256 words/page
    localparam int MAP_IC_K   = 64;  // 64K map entries
    localparam int CACHE_WORDS = DM_CACHE_ROWS * DM_CACHE_WAYS * DM_CACHE_LINE_W;

    logic [27:0] br [0:DM_BR_COUNT-1];

    // Map: block RAM, one port (see map_* port signals below)
    (* ramstyle = "M10K" *) logic [15:0] map_rp    [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_wp    [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_dirty [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_ref   [0:DM_MAP_ENTRIES-1];

    // Cache tags/flags/LRU: registers. Cache data: block RAM indexed
    // {row, way, word}.
    logic [17:0] cache_tag          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_valid        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_dirty        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_wp           [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_vacant       [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_being_loaded [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic [1:0]  cache_lru          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    (* ramstyle = "M10K" *) logic [15:0] cache_data         [0:CACHE_WORDS-1];

    dm_ref_kind_t pipe_kind          [0:DM_PIPE_DEPTH-1];
    logic [3:0]   pipe_task          [0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_subtask       [0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_ref_type      [0:DM_PIPE_DEPTH-1];
    logic [27:0]  pipe_va            [0:DM_PIPE_DEPTH-1];
    logic [15:0]  pipe_map_rp_pre    [0:DM_PIPE_DEPTH-1];
    logic [2:0]   pipe_map_flags_pre [0:DM_PIPE_DEPTH-1];
    logic         pipe_mapbuf_busy   [0:DM_PIPE_DEPTH-1];
    logic [15:0]  pipe_cache_flags   [0:DM_PIPE_DEPTH-1];
    logic [7:0]   pipe_pipe4_errors  [0:DM_PIPE_DEPTH-1];
    logic [7:0]   pipe_pipe4_syndrome[0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_pipe4_quadword[0:DM_PIPE_DEPTH-1];
    int           pipe_head;

    logic [3:0]     proc_srn;
    logic [3:0]     asrn;
    logic [3:0]     fault_count;
    logic [3:0]     fault_first_srn;
    logic           fault_emulator;
    dm_fault_kind_t last_fault;
    logic [27:0]    last_fault_va;
    logic [3:0]     last_fault_task;
    logic [1:0]     last_fault_subtask;
    dm_ref_kind_t   last_fault_ref_kind;

    logic [27:0]    mar;
    logic [15:0]    md;
    logic [15:0]    dbuf;
    logic [15:0]    mcr;
    logic [11:0]    dmux_addr;
    logic           dmux_pending;

    int             mapbuf_busy_slot;
    int             mapbuf_busy_cycles;
    int             last_cache_row;
    int             last_cache_way;

    dm_ref_kind_t   last_ref_kind;
    logic [27:0]    last_ref_va;
    logic [15:0]    last_ref_b;
    logic [3:0]     last_ref_task;
    logic [1:0]     last_ref_subtask;
    logic           last_ref_miss;
    int             last_ref_latency;

    // Backing store: the munch port (sto_*), with the munch in flight
    logic           sto_ack;
    logic [255:0]   sto_rline;
    logic           line_ok;       // sto_rline holds the munch the state asked for
    cstate_t        sto_ret;       // state to resume when the storage op is acked
    int             sto_stage;     // mem_advance stage after a storage write
    logic [15:0]    ap_cq;         // assist: the cache word read in A_DO

    // ---- Memory-engine request (set when a reference is launched) ----------
    dm_ref_kind_t   m_kind;
    logic [27:0]    m_va;
    logic [15:0]    m_b;
    logic [7:0]     m_tioa;
    logic [3:0]     m_task;
    logic [1:0]     m_subtask;
    dm_fault_kind_t m_fault;
    logic           m_byte_sel;   // IFU byte-fetch: which byte of the word

    // ---- Map entries read for the reference in flight ---------------------
    logic [15:0]    m_idx_a, m_idx_v;
    int             m_wbway;
    logic [15:0]    mA_rp, mV_rp;
    logic           mA_wp, mA_dirty, mA_ref, mA_chg;
    logic           mV_wp, mV_dirty, mV_ref, mV_chg;

    // ---- Data operations queued by mem_book_task ---------------------------
    logic           op_wb, op_fill, op_rd, op_wr, op_ioc, op_ios, op_iow;
    logic [5:0]     wb_row, fill_row, word_row, io_row;
    logic [1:0]     wb_way, fill_way, word_way, io_way;
    logic [31:0]    wb_phys, fill_phys, io_base;
    logic [3:0]     word_off;
    logic [15:0]    word_data;
    logic [15:0]    munch_in [0:15];
    logic [4:0]     mcnt;

    // ---- The cache row of the reference in flight (loaded in M_ROW, written
    //      back in M_BOOK): every bookkeeping access is to this one row.
    logic [17:0]    rw_tag   [0:DM_CACHE_WAYS-1];
    logic           rw_valid [0:DM_CACHE_WAYS-1];
    logic           rw_dirty [0:DM_CACHE_WAYS-1];
    logic           rw_wp    [0:DM_CACHE_WAYS-1];
    logic           rw_vacant[0:DM_CACHE_WAYS-1];
    logic           rw_bl    [0:DM_CACHE_WAYS-1];
    logic [1:0]     rw_lru   [0:DM_CACHE_WAYS-1];
    logic           bk_hit;       // the reference hits the row (computed once)
    int             bk_hitway;
    logic           rd_spec;       // the hit word was read speculatively in the booking clock
    logic           md_from_q;     // Md takes cache_q (that read) at the start of this clock
    logic           x_pref_pending; // a processor reference returned: its C_PREF work is due
    // Read-ahead (C_NEXT -> C_FINISH -> C_FETCH executes): see pred_* below.
    logic           pred_ok;        // C_NEXT: the next PC is predicted (comb)
    logic [15:0]    pred_np;
    logic           x_pred_fresh;   // C_FINISH: IM holds the predicted instruction
    logic [15:0]    x_pred_np;
    logic           rm_pre_ok;      // C_FETCH: ex_u and its RM/STK operand were read ahead
    logic [7:0]     fin_rbase;      // C_FINISH: RBase and StkP after it (comb)
    logic [7:0]     fin_stkp;
    logic [7:0]     fin_d8;
    logic [15:0]    pe_tnia, pe_tnia_r, pe_tnia_d;
    logic           pe_flt;
    logic [15:0]    pe_trap0;
    logic           fexec;          // C_FETCH executes the read-ahead instruction this clock
    logic           do_exec;        // exec_task runs this clock
    logic           x_rowload;      // mem_row_task runs this clock (after the case)
    logic [7:0]     fx_stkp;        // StkP and subtask the condition read (below) used
    logic [1:0]     fx_sub;

    // ---- Block-RAM read data -----------------------------------------------
    logic [15:0]    map_rp_q;
    logic           map_wp_q, map_dirty_q, map_ref_q;
    logic [15:0]    cache_q;

    // -------------------------------------------------------------------------
    // MCR Predicates (matching memory.c:702-893)
    // -------------------------------------------------------------------------
    function automatic logic mcr_is_initial_nowake();
        return (mcr == 16'hFEE7);
    endfunction
    function automatic logic mcr_dvavic();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[15];
    endfunction
    function automatic logic mcr_fdmiss();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[14];
    endfunction
    function automatic logic mcr_usemcrv();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[13];
    endfunction
    function automatic logic [1:0] mcr_victim();
        return mcr[12:11];
    endfunction
    function automatic logic mcr_disbr();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[8];
    endfunction
    function automatic logic mcr_discf();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[7];
    endfunction
    function automatic logic mcr_dishold();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[6];
    endfunction
    function automatic logic mcr_noref();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[5];
    endfunction
    function automatic logic mcr_nowake();
        return mcr[0];
    endfunction

    // -------------------------------------------------------------------------
    // Address & Map Helpers
    // -------------------------------------------------------------------------
    function automatic logic [15:0] map_index(input logic [27:0] va);
        return va[23:8];
    endfunction
    function automatic logic [5:0] va_cache_row(input logic [27:0] va);
        return va[9:4];
    endfunction
    function automatic logic [17:0] va_cache_tag(input logic [27:0] va);
        return va[27:10];
    endfunction
    function automatic logic [27:0] va_munch_base(input logic [27:0] va);
        return {va[27:4], 4'b0000};
    endfunction
    function automatic logic [11:0] cache_addr(input logic [5:0] r, input logic [1:0] w,
                                               input logic [3:0] i);
        return {r, w, i};
    endfunction

    function automatic logic [1:0] pipe2_ref_type_fn(input dm_ref_kind_t k);
        case (k)
            DM_REF_FETCH, DM_REF_IFETCH, DM_REF_LONGFETCH,
            DM_REF_PREFETCH, DM_REF_IOFETCH: return 2'd1;
            DM_REF_STORE, DM_REF_IOSTORE:    return 2'd2;
            DM_REF_MAP, DM_REF_RMAP,
            DM_REF_DUMMYREF, DM_REF_FLUSH:   return 2'd3;
            default:                         return 2'd0;
        endcase
    endfunction

    // -------------------------------------------------------------------------
    // Readback Queries (FaultInfo, Config, Pipe0..5, ReadMap)
    // -------------------------------------------------------------------------
    function automatic logic [15:0] get_fault_info();
        logic [2:0] cnt;
        cnt = 3'(fault_count - 4'd1);
        return {proc_srn, asrn, fault_emulator, cnt, fault_first_srn};
    endfunction

    function automatic logic [15:0] get_config_word();
        // 1 module of 64K chips -> module_mask = 0200, chip_type = 3
        return ({4'd0, asrn, 8'd0} | 16'o0200 | (16'(CHIP_TYPE) << 2));
    endfunction

    function automatic logic [15:0] get_pipe2_at(input logic [3:0] srn);
        logic [2:0]  nfaults;
        logic [15:0] ht;
        nfaults = 3'(fault_count - 4'd1);
        ht = {pipe_ref_type[srn], pipe_subtask[srn], pipe_task[srn],
              fault_emulator, nfaults, 4'd0};
        if (fault_count != 4'd0) ht |= {12'd0, fault_first_srn};
        return ~ht;
    endfunction

    function automatic logic [15:0] get_pipe4_at(input logic [3:0] srn);
        logic [15:0] ht, raw;
        logic [7:0]  e;
        logic [2:0]  mf;
        ht = 16'd0;
        e  = pipe_pipe4_errors[srn];
        mf = pipe_map_flags_pre[srn]; // bit0=wp, bit1=dirty, bit2=ref
        if (mf[2]) ht |= 16'h8000;
        if (mf[0]) ht |= 16'h2000;
        if (mf[1]) ht |= 16'h1000;
        if ((e & PIPE4_ERR_MEM_ERROR) != 8'd0) ht |= 16'h0800;
        if ((e & PIPE4_ERR_EC_FAULT) != 8'd0)  ht |= 16'h0400;
        ht |= {6'd0, pipe_pipe4_quadword[srn], pipe_pipe4_syndrome[srn]};
        raw = 16'o170361 ^ ht;
        if ((e & PIPE4_ERR_MAP_TROUBLE) != 8'd0) raw |= 16'h4000;
        else                                     raw &= ~16'h4000;
        return raw;
    endfunction

    function automatic logic [15:0] get_pipe5_at(input logic [3:0] srn);
        logic [15:0] v;
        v = pipe_cache_flags[srn];
        if (pipe_mapbuf_busy[srn]) v |= PIPE5_MAPBUF_BUSY;
        return v;
    endfunction

    // The Map entry at map_index(mar) is read into map_*_q during C_FETCH.
    function automatic logic [15:0] get_read_map();
        return {map_wp_q, map_dirty_q, map_ref_q, map_rp_q[12:0]};
    endfunction

    function automatic logic [15:0] cache_line_pipe5_flags(
        input logic [27:0] va,
        input int          way
    );
        logic [5:0]  row_idx;
        logic [15:0] v;
        row_idx = va_cache_row(va);
        v = (16'(way & 3) << PIPE5_COLUMN_SHIFT);
        if (way < 0 || way >= DM_CACHE_WAYS) v = 16'd0;
        else if (!mcr_discf()) begin
            if (rw_dirty[way])  v |= PIPE5_DIRTY;
            if (rw_vacant[way]) v |= PIPE5_VACANT;
            if (rw_wp[way])     v |= PIPE5_WP;
            if (rw_bl[way])     v |= PIPE5_BEING_LOAD;
        end
        return v;
    endfunction

    // -------------------------------------------------------------------------
    // VA Translation (against an explicit Map entry) & Cache Lookup
    // -------------------------------------------------------------------------
    function automatic dm_fault_kind_t va_translate_e(
        input  logic [27:0] va,
        input  logic        is_write,
        input  logic [15:0] rp,
        input  logic        wp,
        input  logic        dirty,
        output logic [31:0] out_phys
    );
        logic [31:0] phys;
        phys     = ({16'd0, rp} << PAGE_SHIFT) + {24'd0, va[7:0]};
        out_phys = 32'd0;
        if (wp && dirty)               return DM_FAULT_PAGE;
        if (is_write && wp)            return DM_FAULT_WRITE_PROTECT;
        if (phys >= STORAGE_WORDS)     return DM_FAULT_STORAGE_ERROR;
        out_phys = phys;
        return DM_FAULT_NONE;
    endfunction

    // First (lowest-numbered) way of the row that hits tag t.
    function automatic logic row_lookup(input logic [17:0] t, output int out_way);
        logic hit;
        hit     = 1'b0;
        out_way = -1;
        for (int w = DM_CACHE_WAYS - 1; w >= 0; w--) begin
            if (rw_valid[w] && !rw_vacant[w] && rw_tag[w] == t) begin
                out_way = w;
                hit     = 1'b1;
            end
        end
        return hit;
    endfunction

    // The reference's own lookup, computed once in M_BOOK (the bookkeeping
    // asks it several times, always before changing the row).
    function automatic logic cache_lookup(input logic [27:0] va, output int out_way);
        out_way = bk_hitway;
        return bk_hit;
    endfunction

    function automatic int cache_pick_victim(input logic [27:0] va);
        int v;
        v = int'(rw_lru[DM_CACHE_WAYS - 1]);
        for (int w = DM_CACHE_WAYS - 1; w >= 0; w--) begin
            if (!rw_valid[w]) v = w;
        end
        if (mcr_usemcrv()) v = int'(mcr_victim());
        return v;
    endfunction

    // -------------------------------------------------------------------------
    // Map-entry copies for the reference in flight. Every Map access a
    // reference makes is to its own page (index a) or to the page of the line
    // it writes back (index v); when they coincide both copies are kept equal
    // and only copy A is written back.
    // -------------------------------------------------------------------------
    task automatic map_set_ref_a(input logic val);
        mA_ref = val; mA_chg = 1'b1;
        if (m_idx_v == m_idx_a) mV_ref = val;
    endtask
    task automatic map_set_dirty_a(input logic val);
        mA_dirty = val; mA_chg = 1'b1;
        if (m_idx_v == m_idx_a) mV_dirty = val;
    endtask
    task automatic map_set_v_ref_dirty();
        mV_ref = 1'b1; mV_dirty = 1'b1; mV_chg = 1'b1;
        if (m_idx_v == m_idx_a) begin
            mA_ref = 1'b1; mA_dirty = 1'b1; mA_chg = 1'b1;
        end
    endtask

    // -------------------------------------------------------------------------
    // State Mutation Tasks
    // -------------------------------------------------------------------------
    task automatic reset_memory();
        rd_spec = 1'b0;
        md_from_q = 1'b0;
        x_pref_pending = 1'b0;
        x_pred_fresh = 1'b0;
        x_pred_np = 16'd0;
        rm_pre_ok = 1'b0;
        // The Map, cache data and storage are block RAMs: they are not
        // cleared here (cache lines are invalidated by their valid bits; the
        // Map and storage take their contents from initialisation or the
        // loader).
        for (int i = 0; i < DM_BR_COUNT; i++) br[i] = 28'd0;
        for (int r = 0; r < DM_CACHE_ROWS; r++) begin
            for (int w = 0; w < DM_CACHE_WAYS; w++) begin
                cache_tag[r][w]          = 18'd0;
                cache_valid[r][w]        = 1'b0;
                cache_dirty[r][w]        = 1'b0;
                cache_wp[r][w]           = 1'b0;
                cache_vacant[r][w]       = 1'b0;
                cache_being_loaded[r][w] = 1'b0;
                cache_lru[r][w]          = 2'(w);
            end
        end
        for (int s = 0; s < DM_PIPE_DEPTH; s++) begin
            pipe_kind[s]           = DM_REF_NONE;
            pipe_task[s]           = 4'd0;
            pipe_subtask[s]        = 2'd0;
            pipe_ref_type[s]       = 2'd0;
            pipe_va[s]             = 28'd0;
            pipe_map_rp_pre[s]     = 16'd0;
            pipe_map_flags_pre[s]  = 3'd0;
            pipe_mapbuf_busy[s]    = 1'b0;
            pipe_cache_flags[s]    = 16'd0;
            pipe_pipe4_errors[s]   = 8'd0;
            pipe_pipe4_syndrome[s] = 8'd0;
            pipe_pipe4_quadword[s] = 2'd0;
        end
        pipe_head           = 0;
        proc_srn            = 4'd0;
        asrn                = 4'd2;
        fault_count         = 4'd0;
        fault_first_srn     = 4'd0;
        fault_emulator      = 1'b0;
        last_fault          = DM_FAULT_NONE;
        last_fault_va       = 28'd0;
        last_fault_task     = 4'd0;
        last_fault_subtask  = 2'd0;
        last_fault_ref_kind = DM_REF_NONE;
        mar                 = 28'd0;
        md                  = 16'd0;
        dbuf                = 16'd0;
        mcr                 = 16'd0;
        dmux_addr           = 12'd0;
        dmux_pending        = 1'b0;
        mapbuf_busy_slot    = -1;
        mapbuf_busy_cycles  = 0;
        last_cache_row      = -1;
        last_cache_way      = -1;
        last_ref_kind       = DM_REF_NONE;
        last_ref_va         = 28'd0;
        last_ref_b          = 16'd0;
        last_ref_task       = 4'd0;
        last_ref_subtask    = 2'd0;
        last_ref_miss       = 1'b0;
        last_ref_latency    = 0;
        fastio_stb          = 1'b0;
        fastio_kind         = DM_REF_NONE;
        fastio_task         = 4'd0;
        fastio_subtask      = 2'd0;
        fastio_va           = 28'd0;
        for (int i = 0; i < 16; i++) begin
            fastio_out_munch[i] = 16'd0;
            munch_in[i]         = 16'd0;
        end
        m_kind = DM_REF_NONE; m_va = 28'd0; m_b = 16'd0; m_tioa = 8'd0;
        m_task = 4'd0; m_subtask = 2'd0; m_fault = DM_FAULT_NONE; m_byte_sel = 1'b0;
        m_idx_a = 16'd0; m_idx_v = 16'd0; m_wbway = 0;
        mA_rp = 16'd0; mA_wp = 1'b0; mA_dirty = 1'b0; mA_ref = 1'b0; mA_chg = 1'b0;
        mV_rp = 16'd0; mV_wp = 1'b0; mV_dirty = 1'b0; mV_ref = 1'b0; mV_chg = 1'b0;
        op_wb = 1'b0; op_fill = 1'b0; op_rd = 1'b0; op_wr = 1'b0;
        op_ioc = 1'b0; op_ios = 1'b0; op_iow = 1'b0;
        wb_row = 6'd0; fill_row = 6'd0; word_row = 6'd0; io_row = 6'd0;
        wb_way = 2'd0; fill_way = 2'd0; word_way = 2'd0; io_way = 2'd0;
        wb_phys = 32'd0; fill_phys = 32'd0; io_base = 32'd0;
        word_off = 4'd0; word_data = 16'd0; mcnt = 5'd0;
`ifdef DORADO_CHECKPOINT
        // A checkpoint build powers up in a saved machine state (reset_cpu
        // runs first, so this covers both): sim_cedar.cpp CEDAR_DUMP_INIT.
`include "checkpoint_cpu.svh"
`endif
    endtask

    task automatic fault_clear();
        fault_count         = 4'd0;
        fault_first_srn     = 4'd0;
        fault_emulator      = 1'b0;
        last_fault          = DM_FAULT_NONE;
        last_fault_va       = 28'd0;
        last_fault_task     = 4'd0;
        last_fault_subtask  = 2'd0;
        last_fault_ref_kind = DM_REF_NONE;
        last_ref_kind       = DM_REF_NONE;
        last_ref_va         = 28'd0;
        last_ref_b          = 16'd0;
        last_ref_task       = 4'd0;
        last_ref_subtask    = 2'd0;
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

    task automatic cflags_load(input logic [15:0] a_val);
        logic [15:0] flags;
        flags = ~a_val;
        if (!mcr_discf() &&
            last_cache_row >= 0 && last_cache_row < DM_CACHE_ROWS &&
            last_cache_way >= 0 && last_cache_way < DM_CACHE_WAYS) begin
            cache_dirty[last_cache_row][last_cache_way]        = (flags & PIPE5_DIRTY)      != 0;
            cache_vacant[last_cache_row][last_cache_way]       = (flags & PIPE5_VACANT)     != 0;
            cache_wp[last_cache_row][last_cache_way]           = (flags & PIPE5_WP)         != 0;
            cache_being_loaded[last_cache_row][last_cache_way] = (flags & PIPE5_BEING_LOAD) != 0;
        end
    endtask

    task automatic cache_touch_lru(input logic [5:0] row_idx, input int way);
        int seen;
        seen = -1;
        for (int i = DM_CACHE_WAYS - 1; i >= 0; i--) begin
            if (int'(rw_lru[i]) == way) seen = i;
        end
        if (seen >= 0) begin
            for (int i = DM_CACHE_WAYS - 1; i > 0; i--) begin
                if (i <= seen) rw_lru[i] = rw_lru[i - 1];
            end
            rw_lru[0] = 2'(way);
        end
    endtask

    task automatic cache_select(input logic [27:0] va, input int way, input logic [3:0] srn);
        last_cache_row = int'(va_cache_row(va));
        last_cache_way = way;
        pipe_cache_flags[srn] = cache_line_pipe5_flags(va, way);
    endtask

    task automatic pipe_push(
        input logic [3:0]   srn,
        input dm_ref_kind_t kind,
        input logic [27:0]  va,
        input logic [15:0]  rp_pre,
        input logic [2:0]   flags_pre,
        input logic [3:0]   task_id,
        input logic [1:0]   subtask_id
    );
        if (!(fault_count > 4'd0 &&
              (kind == DM_REF_IFETCH || kind == DM_REF_PREFETCH) &&
              srn == fault_first_srn &&
              srn == proc_srn)) begin
            pipe_kind[srn]           = kind;
            pipe_task[srn]           = task_id;
            pipe_subtask[srn]        = subtask_id;
            pipe_ref_type[srn]       = pipe2_ref_type_fn(kind);
            pipe_va[srn]             = va;
            pipe_map_rp_pre[srn]     = rp_pre;
            pipe_map_flags_pre[srn]  = flags_pre;
            pipe_mapbuf_busy[srn]    = 1'b0;
            pipe_cache_flags[srn]    = 16'd0;
            pipe_pipe4_errors[srn]   = 8'd0;
            pipe_pipe4_syndrome[srn] = 8'd0;
            pipe_pipe4_quadword[srn] = 2'd0;
            pipe_head                = (int'(srn) + 1) % DM_PIPE_DEPTH;
        end
    endtask

    // Write back line (row_idx, way) if valid & dirty: queue the data move,
    // mark the Map entry of its page referenced+dirty.
    task automatic cache_writeback_line(
        input  logic [5:0]     row_idx,
        input  int             way,
        output dm_fault_kind_t out_fault
    );
        logic [27:0]    va_base;
        logic [31:0]    phys;
        dm_fault_kind_t f;
        out_fault = DM_FAULT_NONE;
        if (rw_valid[way] && rw_dirty[way]) begin
            va_base = {rw_tag[way], row_idx, 4'b0000};
            f = va_translate_e(va_base, 1'b1, mV_rp, mV_wp, mV_dirty, phys);
            if (f == DM_FAULT_NONE) begin
                op_wb   = 1'b1;
                wb_row  = row_idx;
                wb_way  = 2'(way);
                wb_phys = phys;
                map_set_v_ref_dirty();
            end
            rw_dirty[way] = 1'b0;
            out_fault = f;
        end
    endtask

    task automatic record_writeback_fault(
        input dm_fault_kind_t f,
        input logic [3:0]     srn
    );
        if (f != DM_FAULT_NONE) begin
            if (fault_count == 4'd0) begin
                fault_first_srn = srn;
                fault_emulator  = 1'b1;
            end
            if (fault_count < 4'hF) fault_count = fault_count + 4'd1;
            last_fault = f;
            pipe_pipe4_errors[srn] |= PIPE4_ERR_MAP_TROUBLE;
        end
    endtask

    task automatic cache_fill(input logic [27:0] va, input int way);
        logic [5:0]     r;
        logic [31:0]    phys;
        dm_fault_kind_t f;
        f = va_translate_e(va_munch_base(va), 1'b0, mA_rp, mA_wp, mA_dirty, phys);
        if (f == DM_FAULT_NONE) begin
            r = va_cache_row(va);
            rw_tag[way]          = va_cache_tag(va);
            rw_valid[way]        = 1'b1;
            rw_dirty[way]        = 1'b0;
            rw_wp[way]           = mA_wp;
            rw_vacant[way]       = 1'b0;
            rw_bl[way] = 1'b0;
            op_fill   = 1'b1;
            fill_row  = r;
            fill_way  = 2'(way);
            fill_phys = phys;
            map_set_ref_a(1'b1);
            cache_touch_lru(r, way);
        end
    endtask

    task automatic queue_word_read(input logic [5:0] r, input int way, input logic [27:0] va);
        op_rd    = 1'b1;
        word_row = r;
        word_way = 2'(way);
        word_off = va[3:0];
    endtask

    task automatic queue_word_write(input logic [5:0] r, input int way, input logic [27:0] va,
                                    input logic [15:0] val);
        op_wr     = 1'b1;
        word_row  = r;
        word_way  = 2'(way);
        word_off  = va[3:0];
        word_data = val;
    endtask

    // -------------------------------------------------------------------------
    // Memory-reference bookkeeping (matching dorado_memory_ref_task). Runs in
    // one clock on the request in m_* and the Map entries in mA_*/mV_*; the
    // word/line data moves it implies are queued in op_* for the M_* states.
    // -------------------------------------------------------------------------
    task automatic mem_book_task();
        dm_ref_kind_t   kind;
        logic [27:0]    va;
        logic [15:0]    b_val;
        logic [7:0]     tioa_val;
        logic [3:0]     task_id;
        logic [1:0]     subtask_id;
        logic [15:0]    rp_pre;
        logic [2:0]     flags_pre;
        logic           io_task_uses_asrn, use_asrn;
        logic [3:0]     srn;
        int             way, victim, asrn_advance_count;
        logic [31:0]    phys;
        dm_fault_kind_t f, wbf;
        logic [5:0]     r;
        logic           dvavic_done;
        logic           dirty_victim;
        int             dummy_way;

        kind       = m_kind;
        va         = m_va;
        b_val      = m_b;
        tioa_val   = m_tioa;
        task_id    = m_task;
        subtask_id = m_subtask;

        op_wb = 1'b0; op_fill = 1'b0; op_rd = 1'b0; op_wr = 1'b0;
        op_ioc = 1'b0; op_ios = 1'b0; op_iow = 1'b0;
        mA_chg = 1'b0; mV_chg = 1'b0;

        last_ref_kind    = kind;
        last_ref_va      = va;
        last_ref_b       = b_val;
        last_ref_task    = task_id;
        last_ref_subtask = subtask_id;
        last_ref_miss    = 1'b0;

        mar       = va;
        rp_pre    = mA_rp;
        flags_pre = {mA_ref, mA_dirty, mA_wp};

        io_task_uses_asrn = (task_id != 4'd0 && task_id != 4'o17);
        use_asrn = io_task_uses_asrn ||
                   (kind == DM_REF_IOFETCH) || (kind == DM_REF_IOSTORE) ||
                   (kind == DM_REF_IFETCH);

        if (kind == DM_REF_PREFETCH) begin
            if (!mcr_noref() && (mcr_fdmiss() || !cache_lookup(va, dummy_way))) begin
                use_asrn = 1'b1;
            end
        end

        srn = use_asrn ? asrn : proc_srn;
        r   = va_cache_row(va);
        way = 0;
        f   = DM_FAULT_NONE;
        asrn_advance_count = 0;
        dvavic_done = 1'b0;

        if (mcr_dvavic() &&
            kind != DM_REF_MAP && kind != DM_REF_RMAP &&
            kind != DM_REF_DUMMYREF && kind != DM_REF_NONE) begin
            logic [27:0] pipe_va_victim;
            way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
            pipe_va_victim = rw_valid[way] ? {rw_tag[way], r, 4'b0000} : 28'd0;
            pipe_push(srn, kind, pipe_va_victim, rp_pre, flags_pre, task_id, subtask_id);
            cache_select(va, way, srn);
            dvavic_done = 1'b1;
        end

        if (!dvavic_done) begin
            pipe_push(srn, kind, va, rp_pre, flags_pre, task_id, subtask_id);

            case (kind)
                DM_REF_FETCH, DM_REF_IFETCH, DM_REF_LONGFETCH: begin
                    last_ref_latency = 3;
                    if (mcr_noref()) begin
                        if (!(!mcr_fdmiss() && cache_lookup(va, way))) begin
                            way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                        end
                        cache_select(va, way, srn);
                    end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                        queue_word_read(r, way, va);
                        cache_touch_lru(r, way);
                        cache_select(va, way, srn);
                    end else begin
                        last_ref_miss = 1'b1;
                        if (use_asrn) asrn_advance_count = 1;
                        f = va_translate_e(va, 1'b0, mA_rp, mA_wp, mA_dirty, phys);
                        if (f == DM_FAULT_NONE) begin
                            victim       = cache_pick_victim(va);
                            dirty_victim = rw_dirty[victim];
                            last_ref_latency = dirty_victim ? 24 : 16;
                            if (use_asrn && dirty_victim) asrn_advance_count = 2;
                            cache_writeback_line(r, victim, wbf);
                            record_writeback_fault(wbf, srn);
                            cache_fill(va, victim);
                            queue_word_read(r, victim, va);
                            cache_select(va, victim, srn);
                        end
                    end
                end

                DM_REF_PREFETCH: begin
                    if (!mcr_noref()) begin
                        f = va_translate_e(va, 1'b0, mA_rp, mA_wp, mA_dirty, phys);
                        if (f == DM_FAULT_NONE || f == DM_FAULT_STORAGE_ERROR) begin
                            map_set_ref_a(1'b1);
                        end
                        if (f == DM_FAULT_NONE && (mcr_fdmiss() || !cache_lookup(va, dummy_way))) begin
                            last_ref_miss = 1'b1;
                            victim        = cache_pick_victim(va);
                            dirty_victim  = rw_dirty[victim];
                            if (use_asrn) asrn_advance_count = dirty_victim ? 2 : 1;
                            cache_writeback_line(r, victim, wbf);
                            record_writeback_fault(wbf, srn);
                            cache_fill(va, victim);
                            cache_select(va, victim, srn);
                        end else if (f == DM_FAULT_STORAGE_ERROR ||
                                     f == DM_FAULT_PAGE ||
                                     f == DM_FAULT_WRITE_PROTECT) begin
                            if (use_asrn) asrn_advance_count = 1;
                        end
                    end
                    f = DM_FAULT_NONE;
                end

                DM_REF_STORE: begin
                    dbuf = b_val;
                    if (mcr_noref()) begin
                        if (mcr_usemcrv() && mcr_fdmiss()) begin
                            way = int'(mcr_victim());
                            rw_tag[way]   = va_cache_tag(va);
                            rw_valid[way] = 1'b1;
                        end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                            // comparator-only
                        end else begin
                            way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                        end
                        cache_select(va, way, srn);
                    end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                        if (rw_wp[way]) begin
                            cache_touch_lru(r, way);
                            cache_select(va, way, srn);
                        end else begin
                            queue_word_write(r, way, va, b_val);
                            rw_dirty[way]  = 1'b1;
                            rw_vacant[way] = 1'b0;
                            cache_touch_lru(r, way);
                            cache_select(va, way, srn);
                        end
                    end else begin
                        f = va_translate_e(va, 1'b1, mA_rp, mA_wp, mA_dirty, phys);
                        if (use_asrn) asrn_advance_count = 1;
                        if (f == DM_FAULT_NONE) begin
                            last_ref_miss = 1'b1;
                            way           = cache_pick_victim(va);
                            dirty_victim  = rw_dirty[way];
                            if (use_asrn && dirty_victim) asrn_advance_count = 2;
                            cache_writeback_line(r, way, wbf);
                            record_writeback_fault(wbf, srn);
                            cache_fill(va, way);
                            queue_word_write(r, way, va, b_val);
                            rw_dirty[way]  = 1'b1;
                            rw_vacant[way] = 1'b0;
                            cache_touch_lru(r, way);
                            cache_select(va, way, srn);
                        end
                    end
                end

                DM_REF_IOFETCH: begin
                    if (mcr_noref()) begin
                        way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                        cache_select(va, way, srn);
                    end else begin
                        f = va_translate_e(va, 1'b0, mA_rp, mA_wp, mA_dirty, phys);
                        if (f == DM_FAULT_NONE) begin
                            map_set_ref_a(1'b1);
                            if (use_asrn) asrn_advance_count = 1;
                            if (cache_lookup(va, way)) begin
                                op_ioc = 1'b1;
                                io_row = r;
                                io_way = 2'(way);
                                cache_touch_lru(r, way);
                                cache_select(va, way, srn);
                            end else begin
                                op_ios  = 1'b1;
                                io_base = phys & ~32'hF;
                            end
                            fastio_kind    = DM_REF_IOFETCH;
                            fastio_task    = task_id;
                            fastio_subtask = subtask_id;
                            fastio_va      = va_munch_base(va);
                        end
                    end
                end

                DM_REF_IOSTORE: begin
                    if (mcr_noref()) begin
                        way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                        cache_select(va, way, srn);
                    end else begin
                        f = va_translate_e(va, 1'b1, mA_rp, mA_wp, mA_dirty, phys);
                        if (f == DM_FAULT_NONE) begin
                            if (cache_lookup(va, way)) begin
                                rw_valid[way]  = 1'b0;
                                rw_dirty[way]  = 1'b0;
                                rw_vacant[way] = 1'b1;
                            end
                            map_set_ref_a(1'b1);
                            map_set_dirty_a(1'b1);
                            if (use_asrn) asrn_advance_count = 1;
                            op_iow  = 1'b1;
                            io_base = phys & ~32'hF;
                            for (int i = 0; i < 16; i++) munch_in[i] = fastio_in_munch[i];
                            fastio_kind    = DM_REF_IOSTORE;
                            fastio_task    = task_id;
                            fastio_subtask = subtask_id;
                            fastio_va      = va_munch_base(va);
                        end
                    end
                end

                DM_REF_MAP: begin
                    mA_rp    = b_val;
                    mA_wp    = tioa_val[7];
                    mA_dirty = tioa_val[6];
                    mA_ref   = 1'b0;
                    mA_chg   = 1'b1;
                    if (m_idx_v == m_idx_a) begin
                        mV_rp = b_val; mV_wp = tioa_val[7]; mV_dirty = tioa_val[6]; mV_ref = 1'b0;
                    end
                    pipe_mapbuf_busy[srn]   = 1'b1;
                    mapbuf_busy_slot        = int'(srn);
                    mapbuf_busy_cycles      = 9;
                end

                DM_REF_FLUSH: begin
                    if (cache_lookup(va, way)) begin
                        cache_writeback_line(r, way, wbf);
                        record_writeback_fault(wbf, srn);
                        if (use_asrn) asrn_advance_count = 1;
                        rw_valid[way]  = 1'b0;
                        rw_vacant[way] = 1'b1;
                        cache_select(va, way, srn);
                    end else begin
                        way = cache_pick_victim(va);
                        cache_select(va, way, srn);
                    end
                end

                DM_REF_DUMMYREF: begin
                    if (!mcr_dvavic() && mcr_usemcrv() && mcr_fdmiss()) begin
                        way = int'(mcr_victim());
                    end else if (cache_lookup(va, way)) begin
                        // comparator hit
                    end else begin
                        way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                    end
                    cache_select(va, way, srn);
                end

                default: ; // DM_REF_RMAP: the Pipe entry already holds rp_pre/flags_pre
            endcase

            if (f != DM_FAULT_NONE) begin
                if (f == DM_FAULT_PAGE || f == DM_FAULT_WRITE_PROTECT) begin
                    pipe_pipe4_errors[srn] |= PIPE4_ERR_MAP_TROUBLE;
                end
                last_fault          = f;
                last_fault_va       = va;
                last_fault_task     = task_id;
                last_fault_subtask  = subtask_id;
                last_fault_ref_kind = kind;
                if (kind != DM_REF_IFETCH) begin
                    if (fault_count == 4'd0) fault_first_srn = srn;
                    if (task_id == 4'd0) fault_emulator = 1'b1;
                    if (fault_count < 4'hF) fault_count = fault_count + 4'd1;
                end
            end

            if (use_asrn) begin
                for (int i = 0; i < 2; i++) begin
                    if (i < asrn_advance_count) begin
                        if (asrn == 4'd15) asrn = 4'd2;
                        else               asrn = asrn + 4'd1;
                    end
                end
            end
        end

        m_fault = f;
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
        return md;
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
        if (bb_present && bb_ss_inject) cur_u = bb_mir_uinstr;
        else                            cur_u = unpack_uinstr(im_q0, im_q1, im_q2);

        cur_task_md     = get_task_md();
        ridx            = rm_address_fn(cur_u, ctask, RBase, task_subtask[ctask], StkP);
        cur_rm_stk_read = rmq_is_stk ? stk_q : rm_q;   // read in C_DEC
        cur_id_peek     = id_r0;
    end

    // ex_u holds the instruction C_EXEC executes: captured by the sequencer
    // while it is read (C_FETCH, C_DEC), so the IM read data does not feed
    // the ALU directly.
    logic ex_pres, ex_brk;

    dorado_rtl_shifter u_shifter (
        .u             (ex_u),
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
                pva          = pipe_va[psrn];
                b_overridden = 1'b1;
                case (ff_fc(ex_u.ff))
                    3'd0: b_val = ~get_fault_info();
                    3'd1: b_val = {4'd0, pva[27:16]};
                    3'd2: b_val = pva[15:0];
                    3'd3: b_val = get_pipe2_at(psrn);
                    3'd4: b_val = ~pipe_map_rp_pre[psrn];
                    3'd5: b_val = get_pipe4_at(psrn);
                    3'd6: b_val = ~get_config_word();
                    3'd7: b_val = get_pipe5_at(psrn);
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
                    3'd7: b_val = b_link_read_valid ? b_link_read : Link;
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
                    3'd2: b_val = T;
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
                    a_val = T;
                end else begin
                    case (ex_u.ff[7:6])
                        2'd0: a_val = cur_task_md;
                        2'd1: begin
                            a_val         = id_r0;
                            idcnt_after_a = ifu_idcnt + 8'd1;
                        end
                        2'd2: a_val = Q;
                        2'd3: a_val = T;
                    endcase
                end
            end
            3'd5: begin
                a_val         = id_r0;
                idcnt_after_a = ifu_idcnt + 8'd1;
            end
            3'd6: a_val = T;
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
                        3'd1: begin a_val = T;           a_overridden = 1'b1; end
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
        id_peek_post_a = (idcnt_after_a == ifu_idcnt) ? id_r0 : id_r1;
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
            comb_alufm_entry[5] ^= alu_carry;
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
    task automatic mem_launch(
        input dm_ref_kind_t kind,
        input logic [27:0]  va,
        input logic [15:0]  b_val,
        input logic [7:0]   tioa_val,
        input logic [3:0]   task_id,
        input logic [1:0]   subtask_id,
        input cstate_t      ret
    );
        m_kind    = kind;
        m_va      = va;
        m_b       = b_val;
        m_tioa    = tioa_val;
        m_task    = task_id;
        m_subtask = subtask_id;
        m_ret     = ret;
        st        = M_MAPA;
    endtask

    // M_MAPA's work: the Map entry for the reference's page is read this
    // clock (map_addr), and the reference's cache row is loaded into the row
    // registers. The IFU's own fetches do it in the clock that launches them
    // (ifu_launch_fetch), their address being known from registers.
    // After a processor memory reference (formerly the state C_PREF): Md for
    // the task, and the fault wakeup.
    task automatic mem_pref_task();
        logic recorded_fault;
        recorded_fault = (m_fault != DM_FAULT_NONE) ||
                         (fault_count != x_fcnt_before);
        evc_events |= EVC_EV_PROCREF;
        if (last_ref_miss) evc_events |= EVC_EV_MISS;

        if (m_fault == DM_FAULT_NONE &&
            (x_kind == DM_REF_FETCH || x_kind == DM_REF_IFETCH ||
             x_kind == DM_REF_LONGFETCH)) begin
            task_md[ctask]       = md;
            task_md_valid[ctask] = 1'b1;
            task_md_ready[ctask] = cycles + 64'(last_ref_latency);
        end

        if (recorded_fault && x_kind != DM_REF_IFETCH && !mcr_nowake()) begin
            wakeup_pending[15] = 1'b1;
        end
    endtask

    // A Fetch/IFetch/LongFetch that will hit (mem_book_task's own test, with
    // none of the MCR test modes that change it): no victim, no writeback.
    function automatic logic fetch_hit_fn();
        int w;
        return (m_kind == DM_REF_FETCH || m_kind == DM_REF_IFETCH ||
                m_kind == DM_REF_LONGFETCH) &&
               !mcr_noref() && !mcr_fdmiss() && !mcr_dvavic() && cache_lookup(m_va, w);
    endfunction

    task automatic mem_row_task();
        logic [5:0] r;
        r       = va_cache_row(m_va);
        for (int w = 0; w < DM_CACHE_WAYS; w++) begin
            rw_tag[w]    = cache_tag[r][w];
            rw_valid[w]  = cache_valid[r][w];
            rw_dirty[w]  = cache_dirty[r][w];
            rw_wp[w]     = cache_wp[r][w];
            rw_vacant[w] = cache_vacant[r][w];
            rw_bl[w]     = cache_being_loaded[r][w];
            rw_lru[w]    = cache_lru[r][w];
        end
        m_idx_a = map_index(m_va);
        if (x_assist) begin
            // the harness tests valid and tag only
            ap_hit = 1'b0;
            ap_way = 2'd0;
            for (int w = DM_CACHE_WAYS - 1; w >= 0; w--) begin
                if (rw_valid[w] && rw_tag[w] == va_cache_tag(m_va)) begin
                    ap_hit = 1'b1;
                    ap_way = 2'(w);
                end
            end
        end
        bk_hit  = row_lookup(va_cache_tag(m_va), bk_hitway);
        if (m_kind == DM_REF_FLUSH && bk_hit) m_wbway = bk_hitway;
        else                                  m_wbway = cache_pick_victim(m_va);
        m_idx_v = map_index({rw_tag[m_wbway], r, 4'b0000});
        st      = M_MAPV;
    endtask

    // IFU byte fetch at byte PC `pc` (HM section 6: opcode bytes come from the
    // code base register, BR 37 octal).
    function automatic logic [27:0] ifu_va_fn(input logic [15:0] pc);
        return (br[31] + {13'd0, pc[15:1]}) & 28'h0FFFFFFF;
    endfunction
    // The launching state (C_NEXT, C_IFU_DEC, C_IFU_BETA) presents the Map
    // address for this pc (map_addr below), so M_MAPA's work is done here.
    task automatic ifu_launch_fetch(input logic [15:0] pc, input cstate_t ret);
        m_byte_sel = pc[0];
        mem_launch(DM_REF_IFETCH, ifu_va_fn(pc), 16'd0, 8'd0, 4'd0, 2'd0, ret);
        x_rowload = 1'b1;                    // mem_row_task, once, after the case
    endtask

    // Result of the IFU byte fetch that just returned.
    function automatic logic [7:0] ifu_byte_of(input logic [15:0] w);
        logic high_byte;
        high_byte = (ifu_insset < 2'd2) ? (m_byte_sel == 1'b0) : (m_byte_sel == 1'b1);
        return high_byte ? w[15:8] : w[7:0];
    endfunction
    function automatic logic [7:0] ifu_fetched_byte();
        return ifu_byte_of(md);
    endfunction

    // After mem_book_task: go to the first data operation still to do, or
    // back to the requesting state.
    task automatic mem_advance(input int done_stage);
        mcnt = 5'd0;
        if      (done_stage < 1 && mA_chg)                        st = M_WRA;
        else if (done_stage < 2 && mV_chg && m_idx_v != m_idx_a)  st = M_WRV;
        else if (done_stage < 3 && op_wb)                         st = M_WB;
        else if (done_stage < 4 && op_fill)                       st = M_FILL;
        else if (done_stage < 5 && op_rd)                         st = M_RD;
        else if (done_stage < 6 && op_wr)                         st = M_WR;
        else if (done_stage < 7 && op_ioc)                        st = M_IOC;
        else if (done_stage < 8 && op_ios)                        st = M_IOS;
        else if (done_stage < 9 && op_iow)                        st = M_IOW;
        else                                                      st = m_ret;
    endtask

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
        $readmemh("init/map_rp.hex", map_rp);
        $readmemh("init/map_wp.hex", map_wp);
        $readmemh("init/map_dirty.hex", map_dirty);
        $readmemh("init/map_ref.hex", map_ref);
`ifdef DORADO_CHECKPOINT
        $readmemh("init/cache_data.hex", cache_data);
`endif
    end
`endif

    // =========================================================================
    // Block RAMs. Each has one port, addressed combinationally from the state
    // register; read data appears one clock later.
    // =========================================================================
    logic [11:0] im_addr;
    logic        im_we;
    logic [15:0] im_wd0, im_wd1, im_wd2;

    logic [9:0]  ifum_addr_p;
    logic        ifum_we_hi, ifum_we_lo;
    logic [15:0] ifum_wd;

    logic [15:0] map_addr;
    logic        map_we;
    logic [15:0] map_wd_rp;
    logic        map_wd_wp, map_wd_dirty, map_wd_ref;

    logic [11:0] cache_addr_p;
    logic        cache_we;
    logic [15:0] cache_wd;


    always_comb begin
        // IM: the instruction at real_PC during C_FETCH; the word at Link
        // (for Read IM / Write IM) afterwards.
        // In C_FINISH (unless it commits a Write IM) the read is at the
        // next PC the instruction chose, ahead of scheduling: if the
        // scheduler keeps it, C_FETCH already has the instruction (spec_hit).
        im_addr = (st == C_FETCH || st == C_DEC) ? real_PC[11:0]
                : (st == C_EXEC)  ? Link[11:0]
                : ((st == C_NEXT || st == C_IFU_END || st == C_IFU_DEC ||
                    st == C_IFU_AW || st == C_IFU_BW) && pred_ok) ? pred_np[11:0]
                : (st == C_FINISH && !x_im_wr) ? x_np[11:0]
                :                   link_at_issue[11:0];
        im_we   = (st == C_FINISH) && x_im_wr;
        im_wd0  = x_im_wd0;
        im_wd1  = x_im_wd1;
        im_wd2  = x_im_wd2;

        // IFUM: always the entry for the current {InsSet, opcode}, except
        // in the clock that commits an IFUMRH/IFUMLH write.
        // In C_IFU_OPW the entry is read for the opcode arriving this clock
        // (Md, or the hit word itself when it is taken now: md_from_q), so
        // C_IFU_DEC can follow directly.
        ifum_addr_p = x_ifum_wr ? x_ifum_waddr
                    : (st == C_IFU_OPW) ? {ifu_insset, ifu_byte_of(md_from_q ? cache_q : md)}
                    : {ifu_insset, ifu_opcode};
        ifum_we_hi  = x_ifum_wr && x_ifum_hi;
        ifum_we_lo  = x_ifum_wr && !x_ifum_hi;
        ifum_wd     = x_ifum_wd;

        // Map
        map_we       = 1'b0;
        map_wd_rp    = mA_rp;
        map_wd_wp    = mA_wp;
        map_wd_dirty = mA_dirty;
        map_wd_ref   = mA_ref;
        case (st)
            M_MAPA:  map_addr = map_index(m_va);
            // the IFU fetch these states may launch (ifu_launch_fetch)
            C_NEXT:     map_addr = map_index(ifu_va_fn(ifu_pcf));
            C_IFU_DEC:  map_addr = map_index(ifu_va_fn(ifu_pcf + 16'd1));
            C_IFU_BETA: map_addr = map_index(ifu_va_fn(x_pc_after));
            C_IFU_AW:   map_addr = map_index(ifu_va_fn(x_pc_after + 16'd1));
            M_MAPV:  map_addr = m_idx_v;
            M_WRA:   begin map_addr = m_idx_a; map_we = 1'b1; end
            M_WRV:   begin
                map_addr     = m_idx_v;
                map_we       = 1'b1;
                map_wd_rp    = mV_rp;
                map_wd_wp    = mV_wp;
                map_wd_dirty = mV_dirty;
                map_wd_ref   = mV_ref;
            end
            default: map_addr = map_index(mar);
        endcase

        // Cache data
        cache_we     = 1'b0;
        cache_wd     = sto_rline[16 * mcnt[3:0] +: 16];
        cache_addr_p = cache_addr(word_row, word_way, word_off);
        case (st)
            M_WB:   cache_addr_p = cache_addr(wb_row, wb_way, mcnt[3:0]);
            M_FILL: begin
                cache_addr_p = cache_addr(fill_row, fill_way, mcnt[3:0]);
                cache_we     = line_ok;
            end
            M_WR:   begin
                cache_we = 1'b1;
                cache_wd = word_data;
            end
            M_IOC:  cache_addr_p = cache_addr(io_row, io_way, mcnt[3:0]);
            // speculative read of the hit word, for M_RD
            M_MAPV, M_BOOK: cache_addr_p = cache_addr(va_cache_row(m_va), bk_hitway, m_va[3:0]);
            A_DO:   begin
                cache_addr_p = cache_addr(m_va[9:4], ap_way, m_va[3:0]);
                cache_we     = ap_we && ap_cache && ap_hit;
                cache_wd     = ap_wd;
            end
            default: ;
        endcase
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
        rv   = rmq_is_stk ? stk_q : rm_q;
        case (cond)
            3'd0: return alu_zero;
            3'd1: return alu_lt0;
            3'd2: return ~alu_carry;
            3'd3: return (Cnt == 16'd0);
            3'd4: return (rm_a < 512) && rv[15];
            3'd5: return (rm_a < 512) && rv[0];
            3'd6: return (ctask == 4'd0) ? reschedule_cond : ~io_atten_at_issue;
            default: return ~alu_overflow;
        endcase
    endfunction

    always_comb begin
        dorado_uinstr_t u;
        logic [7:0]     jcn;
        logic           is_long, ffc;
        u       = x_u;
        jcn     = u.jcn;
        is_long = (jcn[7:4] == 4'd0);
        pred_ok = 1'b0;
        pred_np = real_PC + 16'd1;
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
                pred_np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) | {10'd0, jcn[5:0]} |
                          {15'd0, ffc} | dispatch_or;
            else                        // Global Call
                pred_np = (real_PC & ~(16'(CPU_QUADRANT_SIZE - 1))) |
                          ({10'd0, jcn[5:0]} * 16'(CPU_PAGE_SIZE)) | {15'd0, ffc} | dispatch_or;
        end else if (jcn == 8'o045 && u.ff == 8'o147) begin
            pred_ok = 1'b0;             // LoadRam End-item exit
        end else if (is_long) begin     // Long Jump/Call
            pred_ok = 1'b1;
            pred_np = (real_PC & ~(16'(CPU_QUADRANT_SIZE - 1))) | {4'd0, u.ff, jcn[3:0]} | dispatch_or;
        end else if (jcn[2:0] == 3'd7) begin
            if (jcn[7:5] != 3'd1 && jcn[6] && jcn[5:3] == 3'd0) begin   // Return
                pred_ok = 1'b1;
                pred_np = {4'd0, link_at_issue[11:0]} | {15'd0, ffc} | dispatch_or;
            end
        end else begin                  // Conditional Jump/Call
            pred_ok = 1'b1;
            pred_np = (real_PC & ~(16'(CPU_PAGE_SIZE - 1))) |
                      {10'd0, jcn[6:5], 1'b0, jcn[4:3], cond_pure_fn(u, jcn[2:0])} | dispatch_or;
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
        pe_flt    = x_op_faulted || (m_fault != DM_FAULT_NONE);
        pe_trap0  = ifu_trap_addr_fn(6'o00, x_n_slot);
        if (st == C_IFU_DEC && ifum_q_pres && {6'd0, ~ifum_q_hi[11:10]} < 8'd2) begin
            pred_ok = 1'b1;                          // one-byte opcode
            pred_np = {4'd0, pe_tnia_d[11:0]};
        end
        if ((st == C_IFU_AW && (!(ifu_length >= 8'd3 && !pe_flt) ||
                                (x_test_feed && ifu_test_count > 4'd0))) ||
            st == C_IFU_BW) begin
            pred_ok = 1'b1;
            pred_np = pe_flt ? {4'd0, pe_trap0[11:0]} : {4'd0, pe_tnia_r[11:0]};
        end

        fexec = READAHEAD && (st == C_FETCH) && !(ap_req && !ap_done_q) && !ap_hold &&
                spec_hit && rm_pre_ok && !io_busy && !jcn_reads_im_at_link_fn(cur_u);

        // what C_FINISH leaves in RBase and StkP (no task switch)
        fin_rbase = (ifu_dispatch_rbase >= 0) ? 8'(ifu_dispatch_rbase) : x_rbase_after_ff;
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
        if (st == C_FINISH && x_pred_fresh)         // the next instruction's operand
            rms_raddr = rm_address_fn(cur_u, ctask, fin_rbase, task_subtask[ctask], fin_stkp);
        // C_FETCH executing: its operand is already here, so the port reads
        // the branch-condition register C_NEXTRD would (RBase is the issue
        // RBase in this clock), and C_NEXT can follow directly.
        if (fexec)
            rms_raddr = rm_address_fn(cur_u, ctask, RBase, task_subtask[ctask], StkP);
        rms_waddr = lc_write_address_fn(x_u, ctask, x_rbase_at_issue, task_subtask[ctask], StkP);
        rms_we    = (st == C_FINISH) && !halted &&
                    (x_u.lc == 3'd2 || x_u.lc == 3'd4 || x_u.lc == 3'd5 ||
                     x_u.lc == 3'd6 || x_u.lc == 3'd7);
        rms_wd    = (x_u.lc == 3'd4 || x_u.lc == 3'd5) ? x_md_at_issue : x_pd;
        if (st == C_FETCH && ap_req && ap_stk && !ap_done_q) begin
            rms_waddr = 256 + int'(ap_va[7:0]);
            rms_we    = 1'b1;
            rms_wd    = ap_wd;
        end
        if (st == A_WAIT && x_rmor) begin           // RM[a] |= wd, read in C_FETCH
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
    end

    always_ff @(posedge clk) begin
        if (map_we) begin
            map_rp[map_addr]    <= map_wd_rp;
            map_wp[map_addr]    <= map_wd_wp;
            map_dirty[map_addr] <= map_wd_dirty;
            map_ref[map_addr]   <= map_wd_ref;
        end
        map_rp_q    <= map_rp[map_addr];
        map_wp_q    <= map_wp[map_addr];
        map_dirty_q <= map_dirty[map_addr];
        map_ref_q   <= map_ref[map_addr];
    end

    always_ff @(posedge clk) begin
        if (cache_we) cache_data[cache_addr_p] <= cache_wd;
        cache_q <= cache_data[cache_addr_p];
    end

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
        if (!io_busy) begin
        io_read_stb       = 1'b0;
        io_write_stb      = 1'b0;
        fastio_stb        = 1'b0;
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
                            if (ff_fc(u.ff) == 3'd0) fault_clear();
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
                                    3'd1: pd = get_read_map();
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
                                    3'd2: cflags_load(a);
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
                    st    = next_reads_rm_fn(u) ? C_NEXTRD : C_NEXT;

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

                                x_fcnt_before = fault_count;
                                x_kind        = kind;
                                // returns straight to C_NEXT(RD); C_PREF's
                                // bookkeeping runs at the top of that clock
                                x_pref_pending = 1'b1;
                                mem_launch(kind, va, b, TIOA, ctask,
                                           task_subtask[ctask],
                                           next_reads_rm_fn(u) ? C_NEXTRD : C_NEXT);
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
    assign io_tick_early  = clk_en && !halted && (st == C_FINISH || st == C_SCHED);
    assign dbg_ifu_active = ifu_active;
    assign dbg_st         = st;
    always @(posedge clk) begin
        // From the IFU state at the start of this clock: in use in the next
        // one, which is always correct because the clock before an execute
        // (C_FINISH, C_DEC, or C_FETCH waiting) never changes the IFU state.
        id_r0 = ifu_id_at_fn(ifu_idcnt);
        id_r1 = ifu_id_at_fn(ifu_idcnt + 8'd1);
        x_ifum_wr = 1'b0;
        step_done = 1'b0;
        ap_done_q = ap_done;
        if (st != C_FETCH && st != A_WAIT) ap_done = 1'b0;
        if (rst) begin
            reset_cpu();
            reset_memory();
            ap_done  = 1'b0;
            ap_done_q = 1'b0;
            spec_hit = 1'b0;
            sto_req  = 1'b0;
            sto_we   = 1'b0;
            sto_addr = 32'd0;
            sto_mask = 16'd0;
            line_ok  = 1'b0;
            ap_rd    = 16'd0;
            x_assist = 1'b0;
            x_rmor   = 1'b0;
            st      = C_FETCH;
            m_ret   = C_FETCH;
            x_im_wr = 1'b0;
        end else if (clk_en && !halted) begin
            do_exec   = (st == C_EXEC) || fexec;
            x_rowload = 1'b0;
            if (md_from_q) begin
                md        = cache_q;             // the hit word read in the booking clock
                md_from_q = 1'b0;
            end
            if (x_pref_pending && (st == C_NEXT || st == C_NEXTRD)) begin
                mem_pref_task();
                x_pref_pending = 1'b0;
            end
            case (st)
            // -----------------------------------------------------------------
            C_FETCH: begin
                // IM, IFUM and the Map entry at Mar are being read this clock.
                ex_u    = cur_u;
                ex_pres = im_q_pres;
                ex_brk  = im_q_brk;
                ap_done = 1'b0;
                if (ap_req && !ap_done_q) begin
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
                        x_rmor = 1'b1;           // RM read this clock, written in A_WAIT
                        st     = A_WAIT;
                    end else begin
                        x_assist = 1'b1;
                        m_va     = ap_va;
                        st       = M_MAPA;
                    end
                end else if (!ap_hold) begin
                    if (fexec) begin
                        // read ahead: ex_u (latched in C_FINISH) and its RM/STK
                        // operand are here; it executes this clock (below)
                        spec_hit  = 1'b0;
                        rm_pre_ok = 1'b0;
                        fx_stkp   = StkP;
                        fx_sub    = task_subtask[ctask];
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
                if (!io_busy) st = C_EXEC;
            end

            // -----------------------------------------------------------------
            C_EXEC: ;                            // executes below

            // -----------------------------------------------------------------
            C_PREF: begin
                mem_pref_task();
                st = next_reads_rm_fn(x_u) ? C_NEXTRD : C_NEXT;
            end

            // RM/STK are being read at the branch-condition address this clock.
            C_NEXTRD: st = C_NEXT;

            // -----------------------------------------------------------------
            // 5. Next-PC & IFUJump Evaluation (next_pc)
            // -----------------------------------------------------------------
            C_NEXT: begin
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
                                end else begin
                                    ifu_launch_fetch(ifu_pcf, C_IFU_OPW);
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
            end

            // -----------------------------------------------------------------
            // IFUJump: opcode fetch returned
            // -----------------------------------------------------------------
            C_IFU_OPW: begin
                if (last_ref_miss) evc_events |= EVC_EV_MISS;
                if (m_fault != DM_FAULT_NONE) begin
                    x_np = ifu_trap_addr_fn(6'o00, x_n_slot);
                    if (!ff_loads_link_fn(x_u)) Link = real_PC + 16'd1;
                    st = C_FINISH;
                end else begin
                    ifu_opcode = ifu_fetched_byte();
                    st         = C_IFU_DEC;          // IFUM was read for it this clock
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

                    // ifu_decode_lh
                    ifu_sign       = x_lh[15];
                    ifu_length     = {6'd0, ~x_lh[11:10]};
                    ifu_type_pause = ~x_lh[5];
                    ifu_type_jump  = ~x_lh[4];
                    ifu_n          = x_lh[3:0];
                    if (x_lh[8] == 1'b0) MemBase = {4'd0, MemBX, x_lh[7:6]};
                    else                 MemBase = 8'o034 + {6'd0, x_lh[7:6]};
                    RBase              = {7'd0, ~x_lh[9]};
                    ifu_dispatch_rbase = int'(RBase);

                    // ifu_decode_rh
                    ifu_packed_a = x_rh[10];
                    x_ifaddr     = {6'd0, ~x_rh[9:0]};

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
                if (last_ref_miss) evc_events |= EVC_EV_MISS;
                if (m_fault != DM_FAULT_NONE) x_op_faulted = 1'b1;
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
                if (last_ref_miss) evc_events |= EVC_EV_MISS;
                if (m_fault != DM_FAULT_NONE) x_op_faulted = 1'b1;
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
            C_FINISH, C_SCHED: begin
                logic do_sched, spec_ok, pre;
                logic [3:0] ct0;
                do_sched = 1'b1;
                spec_ok  = (st == C_FINISH) && !x_im_wr;
                ct0      = ctask;
                pre      = (st == C_FINISH) && x_pred_fresh;
                rm_pre_ok = 1'b0;
                if (pre) begin                    // the instruction read in C_NEXT
                    ex_u    = cur_u;
                    ex_pres = im_q_pres;
                    ex_brk  = im_q_brk;
                end
                if (st == C_FINISH) begin
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
                    end else begin
                        real_PC  = chosen;
                        spec_hit = spec_ok && (chosen == x_np);
                        // the read-ahead holds if it was the instruction chosen,
                        // in the same task, at the RBase and StkP now in place
                        rm_pre_ok = pre && spec_hit && (x_pred_np == x_np) && (ctask == ct0) &&
                                    (RBase == fin_rbase) && (StkP == fin_stkp);
                        if (x_from_im) begin
                            cycles = cycles + 64'd1;
                            memory_tick();
                        end
                        if (ifu_active && ifu_warmup > 8'd0) ifu_warmup = ifu_warmup - 8'd1;
                    end
                    step_done = 1'b1;
                    st        = C_FETCH;
                end
                x_pred_fresh = 1'b0;
            end

            // -----------------------------------------------------------------
            // Memory engine
            // -----------------------------------------------------------------
            M_ROW, M_MAPA: x_rowload = 1'b1;     // mem_row_task, after the case
            M_MAPV, M_BOOK: begin
                // M_MAPV: the reference page's Map entry arrives (mA) while the
                // possible writeback page's entry is read. That entry (mV) is
                // used only to write back a valid dirty victim, or by a Map
                // reference; otherwise the bookkeeping runs in this clock and
                // M_BOOK is skipped (with mV standing in as mA, unused).
                logic do_book;
                do_book = 1'b1;
                if (st == M_MAPV) begin
                    mA_rp = map_rp_q; mA_wp = map_wp_q; mA_dirty = map_dirty_q; mA_ref = map_ref_q;
                    if (x_assist) begin
                        ap_phys = ({16'd0, map_rp_q} << PAGE_SHIFT) + {24'd0, m_va[7:0]};
                        // machine_store_va: a vacant entry has no translation,
                        // so the word goes to storage[va] (pre-map planting)
                        if (ap_op == AP_STORE && map_wp_q && map_dirty_q)
                            ap_phys = {4'd0, m_va};
                        st      = A_DO;
                        do_book = 1'b0;
                    end else if ((rw_valid[m_wbway] && rw_dirty[m_wbway] && !fetch_hit_fn()) ||
                                 m_kind == DM_REF_MAP || m_kind == DM_REF_FLUSH) begin
                        // the victim's Map entry is read in M_BOOK -- needed
                        // only for a writeback, which a fetch that hits never does
                        st      = M_BOOK;
                        do_book = 1'b0;
                    end else begin
                        mV_rp = mA_rp; mV_wp = mA_wp; mV_dirty = mA_dirty; mV_ref = mA_ref;
                    end
                end else if (m_idx_v == m_idx_a) begin
                    mV_rp = mA_rp; mV_wp = mA_wp; mV_dirty = mA_dirty; mV_ref = mA_ref;
                end else begin
                    mV_rp = map_rp_q; mV_wp = map_wp_q; mV_dirty = map_dirty_q; mV_ref = map_ref_q;
                end
                if (do_book) begin
                    mem_book_task();
                    begin
                        logic [5:0] r;
                        r = va_cache_row(m_va);
                        for (int w = 0; w < DM_CACHE_WAYS; w++) begin
                            cache_tag[r][w]          = rw_tag[w];
                            cache_valid[r][w]        = rw_valid[w];
                            cache_dirty[r][w]        = rw_dirty[w];
                            cache_wp[r][w]           = rw_wp[w];
                            cache_vacant[r][w]       = rw_vacant[w];
                            cache_being_loaded[r][w] = rw_bl[w];
                            cache_lru[r][w]          = rw_lru[w];
                        end
                    end
                    mem_advance(0);
                    // The cache data was read at the hit way this clock
                    // (spec address below); M_RD can take it at once.
                    rd_spec = (st == M_RD);
                    // And when nothing follows the read (a plain Fetch hit)
                    // M_RD is skipped: the reference returns now and Md takes
                    // cache_q at the start of the next clock (md_from_q).
                    if (rd_spec && word_row == va_cache_row(m_va) && word_way == bk_hitway &&
                        word_off == m_va[3:0] && !op_wr && !op_ioc && !op_ios && !op_iow) begin
                        rd_spec   = 1'b0;
                        md_from_q = 1'b1;
                        mcnt      = 5'd0;
                        st        = m_ret;
                    end
                end
            end
            // Assist: the cache word is written or read this clock, and the
            // storage word goes to the munch port (one word of the mask).
            A_DO: begin
                // dorado_storage_store_at_va drops the resident line instead
                if (ap_op == AP_STORE_INV && ap_hit) begin
                    cache_valid[m_va[9:4]][ap_way]  = 1'b0;
                    cache_vacant[m_va[9:4]][ap_way] = 1'b0;
                end
                if (ap_phys < STORAGE_WORDS) begin
                    sto_req   = 1'b1;
                    sto_we    = ap_we;
                    sto_addr  = ap_phys;
                    sto_mask  = 16'd1 << ap_phys[3:0];
                    sto_wline = {16{ap_wd}};
                end
                st = A_WAIT;
            end
            A_WAIT: begin
                if (x_rmor) begin
                    ap_rd   = rm_q;
                    ap_done = 1'b1;
                    x_rmor  = 1'b0;
                    st      = C_FETCH;
                end else begin
                if (!line_ok) begin
                    ap_cq   = cache_q;           // read in A_DO
                    line_ok = 1'b1;
                end
                if (!sto_req || sto_ack) begin
                    sto_req  = 1'b0;
                    line_ok  = 1'b0;
                    ap_rd    = ap_hit ? ap_cq
                             : (ap_phys < STORAGE_WORDS ? sto_rline[16 * ap_phys[3:0] +: 16] : 16'd0);
                    ap_done  = 1'b1;
                    x_assist = 1'b0;
                    st       = C_FETCH;
                end
                end
            end
            // A storage read in flight: resume sto_ret with the munch in sto_rline.
            M_STR: begin
                if (sto_ack) begin
                    sto_req = 1'b0;
                    line_ok = 1'b1;
                    st      = sto_ret;
                end
            end
            // A storage write in flight: then continue after stage sto_stage.
            M_STW: begin
                if (sto_ack) begin
                    sto_req = 1'b0;
                    if (sto_stage == 9) fastio_stb = 1'b1;
                    mem_advance(sto_stage);
                end
            end
            M_WRA: mem_advance(1);
            M_WRV: mem_advance(2);
            M_WB: begin
                // cache line -> munch, one word per clock (the read leads),
                // then one storage write
                if (mcnt != 5'd0) sto_wline[16 * (mcnt - 5'd1) +: 16] = cache_q;
                if (mcnt == 5'd16) begin
                    sto_req   = 1'b1;
                    sto_we    = 1'b1;
                    sto_addr  = wb_phys;
                    sto_mask  = 16'hFFFF;
                    sto_stage = 3;
                    st        = M_STW;
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            M_FILL: begin
                // one storage read, then munch -> cache line, one word per clock
                if (!line_ok) begin
                    sto_req  = 1'b1;
                    sto_we   = 1'b0;
                    sto_addr = fill_phys;
                    sto_ret  = M_FILL;
                    st       = M_STR;
                end else if (mcnt == 5'd15) begin
                    line_ok = 1'b0;
                    mem_advance(4);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            M_RD: begin
                if (mcnt == 5'd1 ||
                    (rd_spec && word_row == va_cache_row(m_va) && word_way == bk_hitway &&
                     word_off == m_va[3:0])) begin
                    rd_spec = 1'b0;
                    md = cache_q;
                    mem_advance(5);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            M_WR: mem_advance(6);
            M_IOC: begin
                if (mcnt != 5'd0) fastio_out_munch[4'(mcnt - 5'd1)] = cache_q;
                if (mcnt == 5'd16) begin
                    fastio_stb = 1'b1;
                    mem_advance(7);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            M_IOS: begin
                if (!line_ok) begin
                    sto_req  = 1'b1;
                    sto_we   = 1'b0;
                    sto_addr = io_base;
                    sto_ret  = M_IOS;
                    st       = M_STR;
                end else begin
                    for (int i = 0; i < 16; i++) fastio_out_munch[i] = sto_rline[16 * i +: 16];
                    line_ok    = 1'b0;
                    fastio_stb = 1'b1;
                    mem_advance(8);
                end
            end
            M_IOW: begin
                for (int i = 0; i < 16; i++) sto_wline[16 * i +: 16] = munch_in[i];
                sto_req   = 1'b1;
                sto_we    = 1'b1;
                sto_addr  = io_base;
                sto_mask  = 16'hFFFF;
                sto_stage = 9;
                st        = M_STW;
            end
            default: st = C_FETCH;
            endcase

            // One instance of the execution logic, for C_EXEC and for a
            // C_FETCH that executes (fexec): calling it from both case items
            // built the whole datapath twice.
            // One instance of M_MAPA's row load, for M_MAPA itself and for the
            // states that launch an IFU fetch (ifu_launch_fetch).
            if (x_rowload) begin
                mem_row_task();
                x_rowload = 1'b0;
            end

            if (do_exec) begin
                exec_task();
                // fexec read the branch-condition register this clock (unless
                // the instruction moved StkP or the subtask)
                if (fexec && st == C_NEXTRD && StkP == fx_stkp && task_subtask[ctask] == fx_sub)
                    st = C_NEXT;
            end
        end
    end

endmodule
/* verilator lint_on BLKSEQ */
