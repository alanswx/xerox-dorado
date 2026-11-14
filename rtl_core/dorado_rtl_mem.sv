// dorado_rtl_mem.sv -- the memory section beside the processor: Map, cache,
// Pipe, FaultInfo, the fast-I/O munch transfers and the storage port
// (HM section 5; dorado/src/memory.c). The bookkeeping (mem_book_task) is
// the C model's, in one clock; the line moves it does in zero time
// (writeback, fill, munch transfer) take their clocks here, in states the
// processor does not wait in unless it needs the section.
//
// A reference: the processor presents the address alone in the clock it
// may issue one (spec_*), so the Map entry, the cache row and the four ways'
// words are read then, before it has committed; the reference itself
// (req_*) follows in the next clock and, if its address is the one read, is
// booked in that clock -- a hit is complete then (done_now, md). That is
// the Dorado's own timing (HM 5.4: the cache answers a reference in the
// cycle after it, and a deferred <-Md in the next instruction is never
// held). A reference whose address was not read ahead is booked one clock
// later.
//
// Clocks after the request: hit 1; a miss or a dirty victim or a Map write
// as many as the storage takes. The processor keeps the Md register file,
// the fault wakeup and the MapBuf-busy count (the C model ties that to its
// instruction count), from the results reported here.
module dorado_rtl_mem
    import dorado_rtl_pkg::*;
#(
    parameter int STORAGE_WORDS = 4 * 1024 * 1024
) (
    input  logic           clk,
    input  logic           rst,
    input  logic           clk_en,

    // ---- the reference ----------------------------------------------------
    input  logic           spec_req,      // read the row, Map entry and words at spec_va now
    input  logic [27:0]    spec_va,
    input  logic           req,           // the reference (held until accepted)
    input  dm_ref_kind_t   req_kind,
    input  logic [27:0]    req_va,
    input  logic [15:0]    req_b,
    input  logic [7:0]     req_tioa,
    input  logic [3:0]     req_task,
    input  logic [1:0]     req_subtask,
    input  logic [1:0]     req_tag,       // who asked (echoed in res_tag)
    output logic           accepted,      // req is taken this clock
    output logic [1:0]     res_tag,
    output logic [1:0]     res_way,       // the way a hit was in (with done_now)
    // A second port: the IFU's peeks -- the word at a VA if it is in the
    // cache, with no side effect at all (no booking, no Md, no Map read).
    // Served when the first port has nothing; always complete in the clock
    // after accept (peek_done: peek_hit with md and res_way, or a miss).
    input  logic           spec2_req,
    input  logic [27:0]    spec2_va,
    input  logic           req2,
    input  logic [27:0]    req2_va,
    output logic           accepted2,
    output logic           peek_done,     // the peek is taken this clock, and:
    output logic           peek_hit,      //   the word is on md (res_way its way)
    output logic           peek_retry,    //   its address was not read ahead: ask again
    // The snoop for a prefetched word: a line in this row changes this clock
    // (valid, vacant or tag -- any booking there but a plain hit, a CFlags<-,
    // the assist), and a word is written this clock.
    output logic           snoop_row_stb,
    output logic [5:0]     snoop_row,
    output logic           snoop_wr,
    output logic [9:0]     snoop_wr_addr, // {row, word}
    output logic [1:0]     snoop_wr_way,
    output logic [5:0]     ref_row,       // the row of the reference in flight (registered)
    output logic           inflight,      // a reference is being processed (not idle, done or the assist)
    output logic [1:0]     inflight_tag,
    output logic           sp_hit,        // (registered) the address read ahead last clock is in the
                                          // cache, valid, in no MCR test mode: a Fetch, IFetch or
                                          // LongFetch of it now completes this clock, and a Store
                                          // unless the line is write-protected (sp_wp)
    output logic           sp_wp,
    output logic           idle,          // nothing in flight (registered)
    output logic           done,          // the accepted reference is complete: a level
                                          // from completion until the next accept, or
                                          // (done_now) this clock
    output logic           done_now,
    output dm_fault_kind_t res_fault,
    output logic           res_miss,
    output logic [7:0]     res_latency,
    // the result as registers, one clock after a hit completes (the
    // processor consumes its own from these: the hit test must not reach
    // the scheduler in one clock)
    output logic           res_valid_r,   // complete, until the next accept
    output logic [1:0]     res_tag_r,
    output logic           res_miss_r,
    output logic [7:0]     res_latency_r,
    output logic [15:0]    md,            // the fetched word (with done_now, this clock's)
    output logic [15:0]    md_r,          // ...as registered: for the execute logic, which
                                          // never runs in a completing clock (timing)
    output dm_fault_kind_t res_fault_r,   // the registered fault, likewise

    // ---- the booking, as it happens (for the processor's MapBuf count,
    //      which it updates at the end of the clock: nothing reads these
    //      combinationally)
    output logic           book_now,      // a reference is booked this clock
    output logic [3:0]     book_srn,      // ...into this Pipe entry
    output logic           book_push,     // ...which is (re)written (pipe_push)
    output logic           book_is_map,   // ...by a Map<-
    output logic [1:0]     book_tag,      // ...for this requester

    // ---- the assist (host port): a word read or written at a VA ----------
    input  logic           ap_req,
    input  logic [2:0]     ap_op,
    input  logic           ap_we,
    input  logic           ap_cache,
    input  logic [27:0]    ap_va,
    input  logic [15:0]    ap_wd,
    output logic           ap_done,       // one clock; ap_rd holds
    output logic [15:0]    ap_rd,

    // ---- processor registers the bookkeeping reads ------------------------
    input  logic [15:0]    mcr,
    input  logic [3:0]     proc_srn,
    // ---- register functions -----------------------------------------------
    input  logic           fault_clear_stb,
    input  logic           cflags_stb,
    input  logic [15:0]    cflags_val,
    // ---- readback ---------------------------------------------------------
    output logic [3:0]     fault_count,
    output logic [15:0]    fault_info,
    output logic [15:0]    config_word,
    output logic [15:0]    read_map,      // the Map entry at map_index(Mar), read while idle
    output logic [15:0]    dbuf,
    output logic [27:0]    pipe_va_p,     // the Pipe entry at proc_srn
    output logic [15:0]    pipe_rp_p,
    output logic [15:0]    pipe2_p,
    output logic [15:0]    pipe4_p,
    output logic [15:0]    pipe5_p,       // without MapBufBusy

    // ---- fast I/O ---------------------------------------------------------
    output logic           fastio_set,    // one clock: a munch transfer completed
    output dm_ref_kind_t   fastio_kind,
    output logic [3:0]     fastio_task,
    output logic [1:0]     fastio_subtask,
    output logic [27:0]    fastio_va,
    output logic [15:0]    fastio_out_munch [0:15],
    input  logic [15:0]    fastio_in_munch  [0:15],

    // ---- storage (the munch port) -----------------------------------------
    output logic           sto_req,
    output logic           sto_we,
    output logic [31:0]    sto_addr,
    output logic [15:0]    sto_mask,
    output logic [255:0]   sto_wline,
    input  logic           sto_ack,
    input  logic [255:0]   sto_rline,

    output logic [4:0]     dbg_st
);
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
    localparam int LINE_WORDS = DM_CACHE_ROWS * DM_CACHE_LINE_W;   // words per way

    typedef enum logic [4:0] {
        S_IDLE, S_BOOK1, S_BOOK2, S_WRA, S_WRV, S_WB, S_FILL, S_RD, S_WR,
        S_IOC, S_IOS, S_IOW, S_STR, S_STW, S_DONE, S_ABOOK, S_AWAIT, S_ADONE
    } mstate_t;
    mstate_t ms;
    assign dbg_st = ms;

    // =========================================================================
    // State
    // =========================================================================
    // Map: block RAM, one port
    (* ramstyle = "M10K" *) logic [15:0] map_rp    [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_wp    [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_dirty [0:DM_MAP_ENTRIES-1];
    (* ramstyle = "M10K" *) logic        map_ref   [0:DM_MAP_ENTRIES-1];

    // Cache tags/flags/LRU: registers. Cache data: one block RAM per way,
    // indexed {row, word}, the four read together.
    logic [17:0] cache_tag          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_valid        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_dirty        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_wp           [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_vacant       [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_being_loaded [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic [1:0]  cache_lru          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    (* ramstyle = "M10K" *) logic [15:0] cache_w0 [0:LINE_WORDS-1];
    (* ramstyle = "M10K" *) logic [15:0] cache_w1 [0:LINE_WORDS-1];
    (* ramstyle = "M10K" *) logic [15:0] cache_w2 [0:LINE_WORDS-1];
    (* ramstyle = "M10K" *) logic [15:0] cache_w3 [0:LINE_WORDS-1];

    dm_ref_kind_t pipe_kind          [0:DM_PIPE_DEPTH-1];
    logic [3:0]   pipe_task          [0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_subtask       [0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_ref_type      [0:DM_PIPE_DEPTH-1];
    logic [27:0]  pipe_va            [0:DM_PIPE_DEPTH-1];
    logic [15:0]  pipe_map_rp_pre    [0:DM_PIPE_DEPTH-1];
    logic [2:0]   pipe_map_flags_pre [0:DM_PIPE_DEPTH-1];
    logic [15:0]  pipe_cache_flags   [0:DM_PIPE_DEPTH-1];
    logic [7:0]   pipe_pipe4_errors  [0:DM_PIPE_DEPTH-1];
    logic [7:0]   pipe_pipe4_syndrome[0:DM_PIPE_DEPTH-1];
    logic [1:0]   pipe_pipe4_quadword[0:DM_PIPE_DEPTH-1];
    int           pipe_head;

    logic [3:0]     asrn;
    logic [3:0]     fault_first_srn;
    logic           fault_emulator;
    dm_fault_kind_t last_fault;
    logic [27:0]    last_fault_va;
    logic [3:0]     last_fault_task;
    logic [1:0]     last_fault_subtask;
    dm_ref_kind_t   last_fault_ref_kind;

    logic [27:0]    mar;
    logic [15:0]    md_q;
    int             last_cache_row;
    int             last_cache_way;

    dm_ref_kind_t   last_ref_kind;
    logic [27:0]    last_ref_va;
    logic [15:0]    last_ref_b;
    logic [3:0]     last_ref_task;
    logic [1:0]     last_ref_subtask;
    logic           last_ref_miss;
    int             last_ref_latency;

    logic           res_valid;     // the accepted reference has completed
    logic           line_ok;       // sto_rline holds the munch the state asked for
    mstate_t        sto_ret;       // state to resume when the storage op is acked
    int             sto_stage;     // mem_advance stage after a storage write

    // ---- the request in flight --------------------------------------------
    dm_ref_kind_t   m_kind;
    logic [27:0]    m_va;
    logic [15:0]    m_b;
    logic [7:0]     m_tioa;
    logic [3:0]     m_task;
    logic [1:0]     m_subtask;
    dm_fault_kind_t m_fault;
    logic           m_assist;      // the request is the assist's
    logic           m_peek;        // the request is a peek (second port)
    logic [1:0]     m_tag;

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

    // ---- The cache row of the reference in flight (loaded when the address
    //      is read, written back when it is booked) --------------------------
    logic [17:0]    rw_tag   [0:DM_CACHE_WAYS-1];
    logic           rw_valid [0:DM_CACHE_WAYS-1];
    logic           rw_dirty [0:DM_CACHE_WAYS-1];
    logic           rw_wp    [0:DM_CACHE_WAYS-1];
    logic           rw_vacant[0:DM_CACHE_WAYS-1];
    logic           rw_bl    [0:DM_CACHE_WAYS-1];
    logic [1:0]     rw_lru   [0:DM_CACHE_WAYS-1];

    // ---- the assist ---------------------------------------------------------
    logic           ap_hit;
    logic [1:0]     ap_way;
    logic [31:0]    ap_phys;
    logic [15:0]    ap_cq;

    // ---- the speculative address read ----------------------------------------
    logic           spec_done_q;   // last clock read the row, Map entry and words
    logic [27:0]    spec_va_q;     // ...at this address
    logic           spec_peek_q;   // ...for the second port (no Map entry)

    // ---- Block-RAM ports ------------------------------------------------------
    logic [15:0]    map_addr;
    logic           map_we;
    logic [15:0]    map_wd_rp;
    logic           map_wd_wp, map_wd_dirty, map_wd_ref;
    logic [15:0]    map_rp_q;
    logic           map_wp_q, map_dirty_q, map_ref_q;
    logic [9:0]     cw_addr;
    logic [3:0]     cw_we;
    logic [15:0]    cw_wd;
    logic [15:0]    cw_q [0:3];

    // Census (docs/rtl-core-speed-plan.md): read by sim_census.h
    logic [63:0]    cen_miss     /* verilator public_flat_rd */;
    logic [63:0]    cen_ifu_miss /* verilator public_flat_rd */;

    // =========================================================================
    // MCR Predicates (matching memory.c:702-893)
    // =========================================================================
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
    function automatic logic mcr_discf();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[7];
    endfunction
    function automatic logic mcr_noref();
        return mcr_is_initial_nowake() ? 1'b0 : mcr[5];
    endfunction

    // =========================================================================
    // Address & Map Helpers
    // =========================================================================
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

    // =========================================================================
    // Readback (FaultInfo, Config, Pipe2/4/5, ReadMap)
    // =========================================================================
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

    function automatic logic [15:0] cache_line_pipe5_flags(
        input logic [27:0] va,
        input int          way
    );
        logic [15:0] v;
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

    // =========================================================================
    // VA Translation (against an explicit Map entry) & Cache Lookup
    // =========================================================================
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

    // The reference's own lookup (bk_*, below: the row against the address
    // in flight), which the bookkeeping asks several times before changing
    // the row.
    logic bk_hit;
    int   bk_hitway;
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

    // =========================================================================
    // Map-entry copies for the reference in flight. Every Map access a
    // reference makes is to its own page (index a) or to the page of the line
    // it writes back (index v); when they coincide both copies are kept equal
    // and only copy A is written back.
    // =========================================================================
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

    // =========================================================================
    // State Mutation Tasks
    // =========================================================================
    task automatic reset_memory();
        // The Map, cache data and storage are block RAMs: they are not
        // cleared here (cache lines are invalidated by their valid bits; the
        // Map and storage take their contents from initialisation or the
        // loader).
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
            pipe_cache_flags[s]    = 16'd0;
            pipe_pipe4_errors[s]   = 8'd0;
            pipe_pipe4_syndrome[s] = 8'd0;
            pipe_pipe4_quadword[s] = 2'd0;
        end
        pipe_head           = 0;
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
        md_q                = 16'd0;
        dbuf                = 16'd0;
        last_cache_row      = -1;
        last_cache_way      = -1;
        last_ref_kind       = DM_REF_NONE;
        last_ref_va         = 28'd0;
        last_ref_b          = 16'd0;
        last_ref_task       = 4'd0;
        last_ref_subtask    = 2'd0;
        last_ref_miss       = 1'b0;
        last_ref_latency    = 0;
        fastio_kind         = DM_REF_NONE;
        fastio_task         = 4'd0;
        fastio_subtask      = 2'd0;
        fastio_va           = 28'd0;
        for (int i = 0; i < 16; i++) begin
            fastio_out_munch[i] = 16'd0;
            munch_in[i]         = 16'd0;
        end
        m_kind = DM_REF_NONE; m_va = 28'd0; m_b = 16'd0; m_tioa = 8'd0;
        m_task = 4'd0; m_subtask = 2'd0; m_fault = DM_FAULT_NONE; m_assist = 1'b0;
        m_peek = 1'b0; m_tag = 2'd0;
        m_idx_a = 16'd0; m_idx_v = 16'd0; m_wbway = 0;
        mA_rp = 16'd0; mA_wp = 1'b0; mA_dirty = 1'b0; mA_ref = 1'b0; mA_chg = 1'b0;
        mV_rp = 16'd0; mV_wp = 1'b0; mV_dirty = 1'b0; mV_ref = 1'b0; mV_chg = 1'b0;
        op_wb = 1'b0; op_fill = 1'b0; op_rd = 1'b0; op_wr = 1'b0;
        op_ioc = 1'b0; op_ios = 1'b0; op_iow = 1'b0;
        wb_row = 6'd0; fill_row = 6'd0; word_row = 6'd0; io_row = 6'd0;
        wb_way = 2'd0; fill_way = 2'd0; word_way = 2'd0; io_way = 2'd0;
        wb_phys = 32'd0; fill_phys = 32'd0; io_base = 32'd0;
        word_off = 4'd0; word_data = 16'd0; mcnt = 5'd0;
        for (int w = 0; w < DM_CACHE_WAYS; w++) begin
            rw_tag[w] = 18'd0; rw_valid[w] = 1'b0; rw_dirty[w] = 1'b0; rw_wp[w] = 1'b0;
            rw_vacant[w] = 1'b0; rw_bl[w] = 1'b0; rw_lru[w] = 2'(w);
        end
        ap_hit = 1'b0; ap_way = 2'd0; ap_phys = 32'd0; ap_cq = 16'd0; ap_rd = 16'd0;
        spec_done_q = 1'b0; spec_va_q = 28'd0; spec_peek_q = 1'b0;
        sp_hit = 1'b0; sp_wp = 1'b0;
        res_valid = 1'b0; line_ok = 1'b0; sto_ret = S_IDLE; sto_stage = 0;
        cen_miss = 64'd0; cen_ifu_miss = 64'd0;
`ifdef DORADO_CHECKPOINT
        // A checkpoint build powers up in a saved machine state: sim_cedar.cpp
        // CEDAR_DUMP_INIT.
`include "checkpoint_mem.svh"
`endif
    endtask

    task automatic fault_clear_task();
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
            // the row read ahead last clock, if it is this one (CFlags<- and
            // a reference in one instruction)
            if (spec_done_q && int'(va_cache_row(spec_va_q)) == last_cache_row) begin
                rw_dirty[last_cache_way]  = (flags & PIPE5_DIRTY)      != 0;
                rw_vacant[last_cache_way] = (flags & PIPE5_VACANT)     != 0;
                rw_wp[last_cache_way]     = (flags & PIPE5_WP)         != 0;
                rw_bl[last_cache_way]     = (flags & PIPE5_BEING_LOAD) != 0;
            end
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

    // Whether a reference writes its Pipe entry (the processor's MapBuf
    // count follows the same rule, from book_push).
    function automatic logic pipe_push_ok(input logic [3:0] srn, input dm_ref_kind_t kind);
        return !(fault_count > 4'd0 &&
                 (kind == DM_REF_IFETCH || kind == DM_REF_PREFETCH) &&
                 srn == fault_first_srn &&
                 srn == proc_srn);
    endfunction

    task automatic pipe_push(
        input logic [3:0]   srn,
        input dm_ref_kind_t kind,
        input logic [27:0]  va,
        input logic [15:0]  rp_pre,
        input logic [2:0]   flags_pre,
        input logic [3:0]   task_id,
        input logic [1:0]   subtask_id
    );
        if (pipe_push_ok(srn, kind)) begin
            pipe_kind[srn]           = kind;
            pipe_task[srn]           = task_id;
            pipe_subtask[srn]        = subtask_id;
            pipe_ref_type[srn]       = pipe2_ref_type_fn(kind);
            pipe_va[srn]             = va;
            pipe_map_rp_pre[srn]     = rp_pre;
            pipe_map_flags_pre[srn]  = flags_pre;
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

    // =========================================================================
    // Memory-reference bookkeeping (matching dorado_memory_ref_task). Runs in
    // one clock on the request in m_* and the Map entries in mA_*/mV_*; the
    // word/line data moves it implies are queued in op_* for the states.
    // =========================================================================
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
                        if (kind == DM_REF_IFETCH) cen_ifu_miss = cen_ifu_miss + 64'd1;
                        else                       cen_miss     = cen_miss + 64'd1;
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
                            cen_miss      = cen_miss + 64'd1;
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
                            cen_miss      = cen_miss + 64'd1;
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
                    // MapBufBusy: the processor counts it (book_is_map)
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

    // After mem_book_task: go to the first data operation still to do, or
    // finish.
    task automatic mem_advance(input int done_stage);
        mcnt = 5'd0;
        if      (done_stage < 1 && mA_chg)                        ms = S_WRA;
        else if (done_stage < 2 && mV_chg && m_idx_v != m_idx_a)  ms = S_WRV;
        else if (done_stage < 3 && op_wb)                         ms = S_WB;
        else if (done_stage < 4 && op_fill)                       ms = S_FILL;
        else if (done_stage < 5 && op_rd)                         ms = S_RD;
        else if (done_stage < 6 && op_wr)                         ms = S_WR;
        else if (done_stage < 7 && op_ioc)                        ms = S_IOC;
        else if (done_stage < 8 && op_ios)                        ms = S_IOS;
        else if (done_stage < 9 && op_iow)                        ms = S_IOW;
        else begin
            // Complete. A hit completes in its booking clock (done_now) and
            // the section is free at once; anything else shows its result
            // for one clock (S_DONE) before it can take a request, so a
            // request already waiting cannot hide the result.
            ms        = done_now ? S_IDLE : S_DONE;
            res_valid = 1'b1;
        end
    endtask

    // The row registers back into the arrays.
    task automatic store_row();
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
    endtask

    // =========================================================================
    // The clock's decisions, combinational: what the booking clock sees.
    // c_*: the reference being booked -- the request itself when it is
    // booked in its accept clock (its address read ahead), else m_*.
    // =========================================================================
    logic           ld_en;         // the row at ld_va is loaded this clock (one site)
    logic [27:0]    ld_va;
    logic [17:0]    nr_tag   [0:DM_CACHE_WAYS-1];   // that row, as the arrays stood at the start of the clock
    logic           nr_valid [0:DM_CACHE_WAYS-1];
    logic           nr_dirty [0:DM_CACHE_WAYS-1];
    logic           nr_wp    [0:DM_CACHE_WAYS-1];
    logic           nr_vacant[0:DM_CACHE_WAYS-1];
    logic           nr_bl    [0:DM_CACHE_WAYS-1];
    logic [1:0]     nr_lru   [0:DM_CACHE_WAYS-1];
    logic           accept_now;    // req is taken this clock
    logic           spec_ok;       // ...and its address was read last clock
    logic           accept2_now;   // a peek is taken this clock
    logic           spec2_ok;
    logic           ap_accept;     // the assist is taken this clock
    logic           spec_take;     // the speculative read runs this clock
    logic           spec_take2;    // ...for the second port
    logic           b1_now;        // the first booking clock is this one
    logic           c_peek;        // ...of a peek
    logic [1:0]     c_tag;
    dm_ref_kind_t   c_kind;
    logic [27:0]    c_va;
    logic [15:0]    c_b;
    logic [3:0]     c_task;
    int             c_wbway;       // the row's victim (or the flushed way)
    logic [15:0]    c_idx_v;
    logic           c_hit_no_wb;   // a Fetch/IFetch/LongFetch/Store that hits: no victim
    logic           c_need_book2;  // the victim's Map entry is wanted first
    logic           c_fetch_hit;   // ...and completes now, with the word
    logic           c_store_hit;   // ...and completes now (the word written if not wp)
    logic           c_store_write;
    logic           c_use_asrn;
    logic [3:0]     c_srn;
    logic [15:0]    md_now;
    logic           bk_now_c, bk_push_c, bk_map_c;   // the booking this clock (comb)
    logic [3:0]     bk_srn_c;
    logic [1:0]     bk_tag_c;
    logic           c_ap_hit;      // the assist's own test: valid and tag only
    logic [1:0]     c_ap_way;

    always_comb begin
        int w;
        accept_now  = (ms == S_IDLE) && req;
        spec_ok     = spec_done_q && !spec_peek_q && (spec_va_q == req_va);
        accept2_now = (ms == S_IDLE) && !req && !ap_req && req2;
        spec2_ok    = spec_done_q && spec_peek_q && (spec_va_q == req2_va);
        ap_accept   = (ms == S_IDLE) && !req && ap_req;
        b1_now      = (accept_now && spec_ok) || (accept2_now && spec2_ok) || (ms == S_BOOK1);

        if (accept2_now) begin
            c_kind = DM_REF_IFETCH; c_va = req2_va; c_b = 16'd0; c_task = 4'd0;
            c_peek = 1'b1; c_tag = 2'd0;
        end else if (ms == S_IDLE) begin
            c_kind = req_kind; c_va = req_va; c_b = req_b; c_task = req_task;
            c_peek = 1'b0; c_tag = req_tag;
        end else begin
            c_kind = m_kind; c_va = m_va; c_b = m_b; c_task = m_task;
            c_peek = m_peek; c_tag = m_tag;
        end

        bk_hit  = row_lookup(va_cache_tag(c_va), bk_hitway);
        if (c_kind == DM_REF_FLUSH && bk_hit) c_wbway = bk_hitway;
        else                                  c_wbway = cache_pick_victim(c_va);
        c_idx_v = map_index({rw_tag[c_wbway], va_cache_row(c_va), 4'b0000});

        c_hit_no_wb  = (c_kind == DM_REF_FETCH || c_kind == DM_REF_IFETCH ||
                        c_kind == DM_REF_LONGFETCH || c_kind == DM_REF_STORE) &&
                       !mcr_noref() && !mcr_fdmiss() && !mcr_dvavic() && bk_hit;
        c_need_book2 = (rw_valid[c_wbway] && rw_dirty[c_wbway] && !c_hit_no_wb) ||
                       c_kind == DM_REF_MAP || c_kind == DM_REF_FLUSH;
        c_fetch_hit  = b1_now && !c_need_book2 && c_hit_no_wb && c_kind != DM_REF_STORE && !c_peek;
        c_store_hit  = b1_now && !c_need_book2 && c_hit_no_wb && c_kind == DM_REF_STORE && !c_peek;
        c_store_write = c_store_hit && !rw_wp[bk_hitway[1:0]];

        // the Pipe entry the booking uses (mem_book_task's own rule)
        c_use_asrn = (c_task != 4'd0 && c_task != 4'o17) ||
                     (c_kind == DM_REF_IOFETCH) || (c_kind == DM_REF_IOSTORE) ||
                     (c_kind == DM_REF_IFETCH) ||
                     (c_kind == DM_REF_PREFETCH && !mcr_noref() && (mcr_fdmiss() || !bk_hit));
        c_srn      = c_use_asrn ? asrn : proc_srn;

        md_now   = cw_q[bk_hitway[1:0]];
        done_now = c_fetch_hit || c_store_hit;
        // a peek: the word if the line is here (as a Fetch would see it), else
        // nothing; complete in its accept clock either way (never in the
        // section's booking state, so a reference never waits behind one),
        // and no booking
        peek_done  = accept2_now;
        peek_hit   = accept2_now && spec2_ok && c_hit_no_wb;
        peek_retry = accept2_now && !spec2_ok;

        c_ap_hit = 1'b0;
        c_ap_way = 2'd0;
        for (w = DM_CACHE_WAYS - 1; w >= 0; w--) begin
            if (rw_valid[w] && rw_tag[w] == va_cache_tag(m_va)) begin
                c_ap_hit = 1'b1;
                c_ap_way = 2'(w);
            end
        end

        // the booking clock: the first one when no second is needed, else that
        bk_now_c  = ((b1_now && !c_need_book2) || (ms == S_BOOK2)) && !c_peek;
        bk_srn_c  = c_srn;
        bk_push_c = pipe_push_ok(c_srn, c_kind);
        bk_map_c  = (c_kind == DM_REF_MAP);
        bk_tag_c  = c_tag;
    end

    // (apart, so the decisions above do not read spec_req: nothing of theirs
    // depends on it, and Verilator would otherwise see a loop through the
    // processor's own speculative address)
    // ...and in the clock a hit completes (the Map port and the row are free
    // then, and the words' port unless a store writes), so references can
    // follow each other every clock. The second port's read-ahead has no Map
    // read; it yields to the first's.
    assign spec_take   = spec_req && !ap_req &&
                         ((ms == S_IDLE && !req) ||
                          (b1_now && done_now && !c_store_write) ||
                          (b1_now && c_peek));
    assign spec_take2  = spec2_req && !spec_req && !ap_req &&
                         ((ms == S_IDLE && !req && !req2) ||
                          (b1_now && done_now && !c_store_write) ||
                          (b1_now && c_peek));
    // The one row load of the clock, from the arrays as they stood at its
    // start (no bypass): when the row is the one a booking this clock writes
    // back, the row registers already hold it, as booked, and are kept.
    assign ld_va = spec_take ? spec_va : spec_take2 ? spec2_va : accept_now ? req_va : ap_va;
    assign ld_en = (spec_take || spec_take2 || (accept_now && !spec_ok) || ap_accept) &&
                   !(bk_now_c && va_cache_row(ld_va) == va_cache_row(c_va));

    // ---- outputs --------------------------------------------------------------
    assign accepted    = accept_now;
    assign accepted2   = accept2_now;
    assign res_tag     = done_now ? c_tag : m_tag;
    assign res_way     = bk_hitway[1:0];
    assign ref_row     = va_cache_row(m_va);
    assign inflight    = !(ms == S_IDLE || ms == S_DONE || ms == S_ABOOK || ms == S_AWAIT || ms == S_ADONE);
    assign inflight_tag = m_tag;
    // the snoop: any booking in the row but a plain hit (misses change a
    // tag, Flush/IOStore/CFlags<- the flags, the assist a valid bit)
    assign snoop_row_stb = (bk_now_c && !(c_fetch_hit || c_store_hit)) ||
                           (ms == S_ABOOK) || cflags_stb;
    assign snoop_row     = cflags_stb ? 6'(last_cache_row) :
                           (ms == S_ABOOK) ? va_cache_row(m_va) : va_cache_row(c_va);
    assign snoop_wr      = |cw_we;
    assign snoop_wr_addr = cw_addr;
    assign snoop_wr_way  = cw_we[1] ? 2'd1 : cw_we[2] ? 2'd2 : cw_we[3] ? 2'd3 : 2'd0;
    assign idle        = (ms == S_IDLE);
    assign done        = (res_valid && !accept_now) || done_now;
    assign res_fault   = done_now ? DM_FAULT_NONE : m_fault;
    assign res_miss    = done_now ? 1'b0 : last_ref_miss;
    assign res_latency = done_now ? 8'd3 : 8'(last_ref_latency);
    assign md          = (done_now || peek_hit) ? md_now : md_q;   // a peek's word too
    assign md_r        = md_q;
    assign res_fault_r = m_fault;
    assign book_now    = bk_now_c;
    assign book_srn    = bk_srn_c;
    assign book_push   = bk_push_c;
    assign book_is_map = bk_map_c;
    assign book_tag    = bk_tag_c;
    assign res_valid_r   = res_valid;
    assign res_tag_r     = m_tag;
    assign res_miss_r    = last_ref_miss;
    assign res_latency_r = 8'(last_ref_latency);
    assign fault_info  = get_fault_info();
    assign config_word = get_config_word();
    assign read_map    = {map_wp_q, map_dirty_q, map_ref_q, map_rp_q[12:0]};
    assign pipe_va_p   = pipe_va[proc_srn];
    assign pipe_rp_p   = pipe_map_rp_pre[proc_srn];
    assign pipe2_p     = get_pipe2_at(proc_srn);
    assign pipe4_p     = get_pipe4_at(proc_srn);
    assign pipe5_p     = pipe_cache_flags[proc_srn];

    // =========================================================================
    // Block-RAM ports, addressed combinationally
    // =========================================================================
    always_comb begin
        // Map: the entry of the address presented (spec/req/assist) while
        // idle; the possible victim's entry in the first booking clock; the
        // writes; else the entry at Mar (ReadMap).
        map_we       = 1'b0;
        map_wd_rp    = mA_rp;
        map_wd_wp    = mA_wp;
        map_wd_dirty = mA_dirty;
        map_wd_ref   = mA_ref;
        map_addr     = map_index(mar);
        if (spec_take)                 map_addr = map_index(spec_va);
        else if (b1_now && !c_peek)    map_addr = c_idx_v;
        else if (ms == S_IDLE) begin
            if (accept_now)            map_addr = map_index(req_va);
            else if (ap_accept)        map_addr = map_index(ap_va);
        end else if (ms == S_WRA) begin
            map_addr = m_idx_a; map_we = 1'b1;
        end else if (ms == S_WRV) begin
            map_addr     = m_idx_v;
            map_we       = 1'b1;
            map_wd_rp    = mV_rp;
            map_wd_wp    = mV_wp;
            map_wd_dirty = mV_dirty;
            map_wd_ref   = mV_ref;
        end

        // Cache data: the four ways at {row, word}. The reference's own word
        // while it is in flight (so cw_q holds it for the booking), the
        // address presented while idle, the line moves in their states.
        cw_we   = 4'd0;
        cw_wd   = sto_rline[16 * mcnt[3:0] +: 16];
        cw_addr = {va_cache_row(m_va), m_va[3:0]};
        if (ms == S_IDLE) begin
            if (accept_now)       cw_addr = {va_cache_row(req_va), req_va[3:0]};
            else if (accept2_now) cw_addr = {va_cache_row(req2_va), req2_va[3:0]};
            else if (ap_accept)   cw_addr = {va_cache_row(ap_va), ap_va[3:0]};
        end
        if (spec_take)            cw_addr = {va_cache_row(spec_va), spec_va[3:0]};
        else if (spec_take2)      cw_addr = {va_cache_row(spec2_va), spec2_va[3:0]};
        if (c_store_write) begin
            cw_addr = {va_cache_row(c_va), c_va[3:0]};
            cw_we[bk_hitway[1:0]] = 1'b1;
            cw_wd   = c_b;
        end
        case (ms)
            S_WB:   cw_addr = {wb_row, mcnt[3:0]};
            S_FILL: begin
                cw_addr          = {fill_row, mcnt[3:0]};
                cw_we[fill_way]  = line_ok;
            end
            S_RD:   cw_addr = {word_row, word_off};
            S_WR:   begin
                cw_addr          = {word_row, word_off};
                cw_we[word_way]  = 1'b1;
                cw_wd            = word_data;
            end
            S_IOC:  cw_addr = {io_row, mcnt[3:0]};
            S_ABOOK: begin
                cw_addr          = {va_cache_row(m_va), m_va[3:0]};
                cw_we[c_ap_way]  = ap_we && ap_cache && c_ap_hit;
                cw_wd            = ap_wd;
            end
            default: ;
        endcase
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
        if (cw_we[0]) cache_w0[cw_addr] <= cw_wd;
        if (cw_we[1]) cache_w1[cw_addr] <= cw_wd;
        if (cw_we[2]) cache_w2[cw_addr] <= cw_wd;
        if (cw_we[3]) cache_w3[cw_addr] <= cw_wd;
        cw_q[0] <= cache_w0[cw_addr];
        cw_q[1] <= cache_w1[cw_addr];
        cw_q[2] <= cache_w2[cw_addr];
        cw_q[3] <= cache_w3[cw_addr];
    end

`ifdef DORADO_INIT
    initial begin
        $readmemh("init/map_rp.hex", map_rp);
        $readmemh("init/map_wp.hex", map_wp);
        $readmemh("init/map_dirty.hex", map_dirty);
        $readmemh("init/map_ref.hex", map_ref);
`ifdef DORADO_CHECKPOINT
        $readmemh("init/cache_w0.hex", cache_w0);
        $readmemh("init/cache_w1.hex", cache_w1);
        $readmemh("init/cache_w2.hex", cache_w2);
        $readmemh("init/cache_w3.hex", cache_w3);
`endif
    end
`endif

    // =========================================================================
    // The first booking clock: the reference's own Map entry has arrived
    // (mA); the row and its words are in rw_*/cw_q. A hit is booked and
    // complete now. A Map<-, a Flush, or a reference whose victim is dirty
    // reads the victim's entry (mV) this clock and books in the next.
    // =========================================================================
    task automatic book1_task();
        if (c_peek) begin
            ms = S_IDLE;                 // nothing changes; the word is on md (peek_hit)
            return;
        end
        mA_rp = map_rp_q; mA_wp = map_wp_q; mA_dirty = map_dirty_q; mA_ref = map_ref_q;
        m_idx_a = map_index(m_va);
        m_wbway = c_wbway;
        m_idx_v = c_idx_v;
        if (c_need_book2) begin
            ms = S_BOOK2;                // mV arrives next clock
        end else begin
            mV_rp = mA_rp; mV_wp = mA_wp; mV_dirty = mA_dirty; mV_ref = mA_ref;
            book_task();
        end
    endtask

    task automatic book_task();
        mem_book_task();
        store_row();
        // the hit word was read with the row: take it now, the Fetch is done
        if (c_fetch_hit && op_rd) begin
            md_q  = cw_q[bk_hitway[1:0]];
            op_rd = 1'b0;
        end
        // the stored word was written this clock (cw_we)
        if (c_store_write) op_wr = 1'b0;
        mem_advance(0);
    endtask

    // =========================================================================
    // The state machine
    // =========================================================================
    always @(posedge clk) begin
        fastio_set = 1'b0;
        ap_done    = 1'b0;
        if (rst) begin
            reset_memory();
            ms       = S_IDLE;
            sto_req  = 1'b0;
            sto_we   = 1'b0;
            sto_addr = 32'd0;
            sto_mask = 16'd0;
            sto_wline = 256'd0;
        end else if (clk_en) begin
            if (fault_clear_stb) fault_clear_task();
            if (cflags_stb)      cflags_load(cflags_val);
            begin                            // the row to load, as the arrays stand now
                logic [5:0] r;
                r = va_cache_row(ld_va);
                for (int w = 0; w < DM_CACHE_WAYS; w++) begin
                    nr_tag[w]    = cache_tag[r][w];
                    nr_valid[w]  = cache_valid[r][w];
                    nr_dirty[w]  = cache_dirty[r][w];
                    nr_wp[w]     = cache_wp[r][w];
                    nr_vacant[w] = cache_vacant[r][w];
                    nr_bl[w]     = cache_being_loaded[r][w];
                    nr_lru[w]    = cache_lru[r][w];
                end
            end

            spec_done_q = 1'b0;
            case (ms)
            S_IDLE: begin
                if (accept_now) begin
                    res_valid = 1'b0;
                    m_kind    = req_kind;
                    m_va      = req_va;
                    m_b       = req_b;
                    m_tioa    = req_tioa;
                    m_task    = req_task;
                    m_subtask = req_subtask;
                    m_assist  = 1'b0;
                    m_peek    = 1'b0;
                    m_tag     = req_tag;
                    if (spec_ok) begin
                        book1_task();        // the address was read last clock
                    end else begin
                        ms = S_BOOK1;        // the row loads now (ld_en); book next clock
                    end
                end else if (ap_accept) begin
                    m_va     = ap_va;
                    m_assist = 1'b1;
                    ms = S_ABOOK;            // the row loads now (ld_en)
                end
                // a peek (accept2_now) changes nothing: its answer is on the
                // outputs this clock
            end

            S_BOOK1: book1_task();

            S_BOOK2: begin
                if (m_idx_v == m_idx_a) begin
                    mV_rp = mA_rp; mV_wp = mA_wp; mV_dirty = mA_dirty; mV_ref = mA_ref;
                end else begin
                    mV_rp = map_rp_q; mV_wp = map_wp_q; mV_dirty = map_dirty_q; mV_ref = map_ref_q;
                end
                book_task();
            end

            S_DONE: ms = S_IDLE;
            S_WRA: mem_advance(1);
            S_WRV: mem_advance(2);
            S_WB: begin
                // cache line -> munch, one word per clock (the read leads),
                // then one storage write
                if (mcnt != 5'd0) sto_wline[16 * (mcnt - 5'd1) +: 16] = cw_q[wb_way];
                if (mcnt == 5'd16) begin
                    sto_req   = 1'b1;
                    sto_we    = 1'b1;
                    sto_addr  = wb_phys;
                    sto_mask  = 16'hFFFF;
                    sto_stage = 3;
                    ms        = S_STW;
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            S_FILL: begin
                // one storage read, then munch -> cache line, one word per clock
                if (!line_ok) begin
                    sto_req  = 1'b1;
                    sto_we   = 1'b0;
                    sto_addr = fill_phys;
                    sto_ret  = S_FILL;
                    ms       = S_STR;
                end else if (mcnt == 5'd15) begin
                    line_ok = 1'b0;
                    mem_advance(4);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            S_RD: begin
                // the word's address was presented last clock
                if (mcnt == 5'd1) begin
                    md_q = cw_q[word_way];
                    mem_advance(5);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            S_WR: mem_advance(6);
            S_IOC: begin
                if (mcnt != 5'd0) fastio_out_munch[4'(mcnt - 5'd1)] = cw_q[io_way];
                if (mcnt == 5'd16) begin
                    fastio_set = 1'b1;
                    mem_advance(7);
                end else begin
                    mcnt = mcnt + 5'd1;
                end
            end
            S_IOS: begin
                if (!line_ok) begin
                    sto_req  = 1'b1;
                    sto_we   = 1'b0;
                    sto_addr = io_base;
                    sto_ret  = S_IOS;
                    ms       = S_STR;
                end else begin
                    for (int i = 0; i < 16; i++) fastio_out_munch[i] = sto_rline[16 * i +: 16];
                    line_ok    = 1'b0;
                    fastio_set = 1'b1;
                    mem_advance(8);
                end
            end
            S_IOW: begin
                for (int i = 0; i < 16; i++) sto_wline[16 * i +: 16] = munch_in[i];
                sto_req   = 1'b1;
                sto_we    = 1'b1;
                sto_addr  = io_base;
                sto_mask  = 16'hFFFF;
                sto_stage = 9;
                ms        = S_STW;
            end
            // A storage read in flight: resume sto_ret with the munch in sto_rline.
            S_STR: begin
                if (sto_ack) begin
                    sto_req = 1'b0;
                    line_ok = 1'b1;
                    ms      = sto_ret;
                end
            end
            // A storage write in flight: then continue after stage sto_stage.
            S_STW: begin
                if (sto_ack) begin
                    sto_req = 1'b0;
                    if (sto_stage == 9) fastio_set = 1'b1;   // the IOStore's munch is in storage
                    mem_advance(sto_stage);
                end
            end

            // The assist: the word at m_va (the Map entry in map_*_q, the
            // row in rw_*, the words in cw_q). The cache word is read or
            // written this clock (cw_*), the storage word goes to the munch
            // port (one word of the mask).
            S_ABOOK: begin
                ap_hit  = c_ap_hit;
                ap_way  = c_ap_way;
                ap_phys = ({16'd0, map_rp_q} << PAGE_SHIFT) + {24'd0, m_va[7:0]};
                // machine_store_va: a vacant entry has no translation, so the
                // word goes to storage[va] (pre-map planting)
                if (ap_op == AP_STORE && map_wp_q && map_dirty_q) ap_phys = {4'd0, m_va};
                ap_cq = cw_q[ap_way];
                // dorado_storage_store_at_va drops the resident line instead
                if (ap_op == AP_STORE_INV && ap_hit) begin
                    cache_valid[va_cache_row(m_va)][ap_way]  = 1'b0;
                    cache_vacant[va_cache_row(m_va)][ap_way] = 1'b0;
                end
                if (ap_phys < STORAGE_WORDS) begin
                    sto_req   = 1'b1;
                    sto_we    = ap_we;
                    sto_addr  = ap_phys;
                    sto_mask  = 16'd1 << ap_phys[3:0];
                    sto_wline = {16{ap_wd}};
                end
                ms = S_AWAIT;
            end
            S_AWAIT: begin
                if (!sto_req || sto_ack) begin
                    sto_req  = 1'b0;
                    ap_rd    = ap_hit ? ap_cq
                             : (ap_phys < STORAGE_WORDS ? sto_rline[16 * ap_phys[3:0] +: 16] : 16'd0);
                    ap_done  = 1'b1;          // seen by the processor in S_ADONE's clock,
                    m_assist = 1'b0;          // when it drops ap_req
                    ms       = S_ADONE;
                end
            end
            S_ADONE: ms = S_IDLE;
            default: ms = S_IDLE;
            endcase

            if (ld_en) begin                 // the clock's one row load
                for (int w = 0; w < DM_CACHE_WAYS; w++) begin
                    rw_tag[w]    = nr_tag[w];
                    rw_valid[w]  = nr_valid[w];
                    rw_dirty[w]  = nr_dirty[w];
                    rw_wp[w]     = nr_wp[w];
                    rw_vacant[w] = nr_vacant[w];
                    rw_bl[w]     = nr_bl[w];
                    rw_lru[w]    = nr_lru[w];
                end
            end
            // the read-ahead address against the row as the row registers
            // will hold it next clock (ld_en: the arrays' row; else the
            // registers, kept: a hit booked this clock moves no tag)
            sp_hit = 1'b0;
            sp_wp  = 1'b0;
            if (spec_take && !mcr_noref() && !mcr_fdmiss() && !mcr_dvavic()) begin
                for (int w = DM_CACHE_WAYS - 1; w >= 0; w--) begin
                    if (rw_valid[w] && !rw_vacant[w] && rw_tag[w] == va_cache_tag(spec_va)) begin
                        sp_hit = 1'b1;
                        sp_wp  = rw_wp[w];
                    end
                end
            end
            if (spec_take) begin
                spec_done_q = 1'b1;
                spec_va_q   = spec_va;
                spec_peek_q = 1'b0;
            end else if (spec_take2) begin
                spec_done_q = 1'b1;
                spec_va_q   = spec2_va;
                spec_peek_q = 1'b1;
            end
        end
    end
endmodule
