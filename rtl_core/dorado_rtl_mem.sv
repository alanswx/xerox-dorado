// =============================================================================
// dorado_rtl_mem.sv — Xerox Dorado Memory Subsystem (BR, Map, Cache, Pipe, MSA)
// =============================================================================
// Derived from the schematic-audited C reference (dorado/src/memory.c).
// Models the 32 Base Registers (28-bit), 64K-entry Virtual-to-Physical Map,
// 4K-word 4-way set-associative Cache (64 rows x 4 ways x 16 words),
// 16-entry Storage Reference Pipe (ProcSRN / ASRN), FaultInfo, MCR, and DMux.
// =============================================================================

module dorado_rtl_mem
    import dorado_rtl_pkg::*;
#(
    parameter int STORAGE_WORDS = 4 * 1024 * 1024, // 1 module of 64K chips = 4M words
    parameter int CHIP_TYPE     = 3,               // 3 = 64K chips (4MW/module)
    parameter int PAGE_SHIFT    = 8,               // 256 words/page
    parameter int MAP_IC_K      = 64               // 64K map entries
) (
    input  logic           clk,
    input  logic           rst,

    // Fast I/O munch interface (IOFetch<- / IOStore<-)
    output logic           fastio_stb,
    output dm_ref_kind_t   fastio_kind,
    output logic [3:0]     fastio_task,
    output logic [1:0]     fastio_subtask,
    output logic [27:0]    fastio_va,
    output logic [15:0]    fastio_out_munch [0:15],
    input  logic [15:0]    fastio_in_munch  [0:15],

    // Disk DMux status bits (addresses 02000..02177)
    input  logic           dmux_ext_handled,
    input  logic [15:0]    dmux_ext_val,
    output logic [11:0]    dmux_cur_addr
);
    // -------------------------------------------------------------------------
    // Constants matching memory.c
    // -------------------------------------------------------------------------
    localparam logic [15:0] PIPE5_MAPBUF_BUSY  = 16'h8000;
    localparam logic [15:0] PIPE5_DIRTY        = 16'h0080;
    localparam logic [15:0] PIPE5_VACANT       = 16'h0040;
    localparam logic [15:0] PIPE5_WP           = 16'h0020;
    localparam logic [15:0] PIPE5_BEING_LOAD   = 16'h0010;
    localparam int          PIPE5_COLUMN_SHIFT = 8;

    localparam logic [7:0]  PIPE4_ERR_MAP_TROUBLE = 8'h01;
    localparam logic [7:0]  PIPE4_ERR_MEM_ERROR   = 8'h02;
    localparam logic [7:0]  PIPE4_ERR_EC_FAULT    = 8'h04;

    // -------------------------------------------------------------------------
    // Architectural Memory State
    // -------------------------------------------------------------------------
    logic [27:0] br [0:DM_BR_COUNT-1];

    // 64K-entry Virtual-to-Physical Map
    logic [15:0] map_rp    [0:DM_MAP_ENTRIES-1];
    logic        map_wp    [0:DM_MAP_ENTRIES-1];
    logic        map_dirty [0:DM_MAP_ENTRIES-1];
    logic        map_ref   [0:DM_MAP_ENTRIES-1];

    // 4-way Set-Associative Cache (64 rows x 4 ways x 16 words)
    logic [17:0] cache_tag          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_valid        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_dirty        [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_wp           [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_vacant       [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic        cache_being_loaded [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];
    logic [15:0] cache_data         [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1][0:DM_CACHE_LINE_W-1];
    logic [1:0]  cache_lru          [0:DM_CACHE_ROWS-1][0:DM_CACHE_WAYS-1];

    // 16-entry Storage Reference Pipe
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

    // Control & Fault Registers
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

    // Backing Store (Main Storage Array)
    logic [15:0]    storage [0:STORAGE_WORDS-1];

    // -------------------------------------------------------------------------
    // MCR Predicates (matching memory.c:702-893)
    // -------------------------------------------------------------------------
    function automatic logic mcr_is_initial_nowake();
        return (mcr == 16'hFEE7);
    endfunction

    function automatic logic mcr_dvavic();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[15];
    endfunction

    function automatic logic mcr_fdmiss();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[14];
    endfunction

    function automatic logic mcr_usemcrv();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[13];
    endfunction

    function automatic logic [1:0] mcr_victim();
        return mcr[12:11];
    endfunction

    function automatic logic [1:0] mcr_nextvictim();
        return mcr[10:9];
    endfunction

    function automatic logic mcr_disbr();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[8];
    endfunction

    function automatic logic mcr_discf();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[7];
    endfunction

    function automatic logic mcr_dishold();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[6];
    endfunction

    function automatic logic mcr_noref();
        if (mcr_is_initial_nowake()) return 1'b0;
        return mcr[5];
    endfunction

    function automatic logic mcr_nowake();
        return mcr[0];
    endfunction

    // -------------------------------------------------------------------------
    // Address & Map Helpers
    // -------------------------------------------------------------------------
    function automatic logic [15:0] map_index(input logic [27:0] va);
        return 16'((va >> PAGE_SHIFT) & (DM_MAP_ENTRIES - 1));
    endfunction

    function automatic logic [27:0] va_page_offset(input logic [27:0] va);
        return va & ((28'd1 << PAGE_SHIFT) - 28'd1);
    endfunction

    function automatic logic [3:0] va_cache_offset(input logic [27:0] va);
        return va[3:0];
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

    // -------------------------------------------------------------------------
    // Readback Queries (FaultInfo, Config, Pipe0..5, ReadMap)
    // -------------------------------------------------------------------------
    function automatic logic [15:0] get_fault_info();
        logic [2:0] cnt;
        cnt = 3'(fault_count - 4'd1);
        return {proc_srn, asrn, fault_emulator, cnt, fault_first_srn};
    endfunction

    function automatic logic [15:0] get_config_word();
        // 1 module of 64K chips -> module_mask = 0200 (0x80), chip_type = 3
        logic [15:0] module_mask;
        module_mask = 16'o0200;
        return ({4'd0, asrn, 8'd0} | module_mask | (16'(CHIP_TYPE) << 2));
    endfunction

    function automatic logic [15:0] get_pipe2_at(input logic [3:0] srn);
        logic [2:0]  nfaults;
        logic [15:0] ht;
        nfaults = 3'(fault_count - 4'd1);
        ht = 16'd0;
        ht |= {pipe_ref_type[srn], 14'd0};
        ht |= {2'd0, pipe_subtask[srn], 12'd0};
        ht |= {4'd0, pipe_task[srn], 8'd0};
        ht |= {8'd0, fault_emulator, 7'd0};
        ht |= {9'd0, nfaults, 4'd0};
        if (fault_count != 4'd0) begin
            ht |= {12'd0, fault_first_srn};
        end
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
        if (e & PIPE4_ERR_MEM_ERROR) ht |= 16'h0800;
        if (e & PIPE4_ERR_EC_FAULT)  ht |= 16'h0400;
        ht |= {6'd0, pipe_pipe4_quadword[srn], pipe_pipe4_syndrome[srn]};
        raw = 16'o170361 ^ ht;
        if (e & PIPE4_ERR_MAP_TROUBLE) raw |= 16'h4000;
        else                           raw &= ~16'h4000;
        return raw;
    endfunction

    function automatic logic [15:0] get_pipe5_at(input logic [3:0] srn);
        logic [15:0] v;
        v = pipe_cache_flags[srn];
        if (pipe_mapbuf_busy[srn]) v |= PIPE5_MAPBUF_BUSY;
        return v;
    endfunction

    function automatic logic [15:0] get_read_map();
        logic [15:0] idx;
        idx = map_index(mar);
        return {map_wp[idx], map_dirty[idx], map_ref[idx], map_rp[idx][12:0]};
    endfunction

    function automatic logic [15:0] cache_line_pipe5_flags(
        input logic [27:0] va,
        input int          way
    );
        logic [5:0]  row_idx;
        logic [15:0] v;
        if (way < 0 || way >= DM_CACHE_WAYS) return 16'd0;
        row_idx = va_cache_row(va);
        v = (16'(way & 3) << PIPE5_COLUMN_SHIFT);
        if (mcr_discf()) return v;
        if (cache_dirty[row_idx][way])        v |= PIPE5_DIRTY;
        if (cache_vacant[row_idx][way])       v |= PIPE5_VACANT;
        if (cache_wp[row_idx][way])           v |= PIPE5_WP;
        if (cache_being_loaded[row_idx][way]) v |= PIPE5_BEING_LOAD;
        return v;
    endfunction

    // -------------------------------------------------------------------------
    // VA Translation & Cache Lookup
    // -------------------------------------------------------------------------
    function automatic dm_fault_kind_t va_translate(
        input  logic [27:0] va,
        input  logic        is_write,
        output int unsigned out_phys
    );
        logic [15:0] idx;
        int unsigned phys;
        idx = map_index(va);
        out_phys = 0;
        if (map_wp[idx] && map_dirty[idx]) begin
            return DM_FAULT_PAGE;
        end
        if (is_write && map_wp[idx]) begin
            return DM_FAULT_WRITE_PROTECT;
        end
        phys = (int'(map_rp[idx]) << PAGE_SHIFT) + int'(va_page_offset(va));
        if (phys >= STORAGE_WORDS) begin
            return DM_FAULT_STORAGE_ERROR;
        end
        out_phys = phys;
        return DM_FAULT_NONE;
    endfunction

    function automatic logic cache_lookup(
        input  logic [27:0] va,
        output int          out_way
    );
        logic [5:0]  r;
        logic [17:0] t;
        r = va_cache_row(va);
        t = va_cache_tag(va);
        for (int w = 0; w < DM_CACHE_WAYS; w++) begin
            if (cache_valid[r][w] && !cache_vacant[r][w] && cache_tag[r][w] == t) begin
                out_way = w;
                return 1'b1;
            end
        end
        out_way = -1;
        return 1'b0;
    endfunction

    function automatic int cache_pick_victim(input logic [27:0] va);
        logic [5:0] r;
        if (mcr_usemcrv()) return int'(mcr_victim());
        r = va_cache_row(va);
        for (int w = 0; w < DM_CACHE_WAYS; w++) begin
            if (!cache_valid[r][w]) return w;
        end
        return int'(cache_lru[r][DM_CACHE_WAYS - 1]);
    endfunction

    // -------------------------------------------------------------------------
    // State Mutation Tasks (invoked sequentially within CPU posedge clk)
    // -------------------------------------------------------------------------
    task automatic reset_memory();
        for (int i = 0; i < DM_BR_COUNT; i++) br[i] = 28'd0;
        for (int i = 0; i < DM_MAP_ENTRIES; i++) begin
            map_rp[i]    = 16'd0;
            map_wp[i]    = 1'b1;
            map_dirty[i] = 1'b1; // Vacant
            map_ref[i]   = 1'b0;
        end
        for (int r = 0; r < DM_CACHE_ROWS; r++) begin
            for (int w = 0; w < DM_CACHE_WAYS; w++) begin
                cache_tag[r][w]          = 18'd0;
                cache_valid[r][w]        = 1'b0;
                cache_dirty[r][w]        = 1'b0;
                cache_wp[r][w]           = 1'b0;
                cache_vacant[r][w]       = 1'b0;
                cache_being_loaded[r][w] = 1'b0;
                cache_lru[r][w]          = 2'(w);
                for (int i = 0; i < DM_CACHE_LINE_W; i++) begin
                    cache_data[r][w][i] = 16'd0;
                end
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
        end
    endtask

    assign dmux_cur_addr = dmux_addr;

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

    task automatic dmux_use_dmd();
        dmux_pending = 1'b0;
    endtask

    task automatic dmux_read(output logic [15:0] val);
        logic [11:0] a;
        a = dmux_addr;
        dmux_pending = 1'b0;
        if (dmux_ext_handled) begin
            val = dmux_ext_val;
            return;
        end
        case (a)
            12'o1510: val = (MAP_IC_K == 16)  ? 16'h8000 : 16'h0000;
            12'o1511: val = (MAP_IC_K == 64)  ? 16'h8000 : 16'h0000;
            12'o1512: val = (MAP_IC_K == 256) ? 16'h8000 : 16'h0000;
            default:  val = 16'h0000;
        endcase
    endtask

    task automatic cflags_load(input logic [15:0] a_val);
        logic [15:0] flags;
        if (mcr_discf()) return;
        if (last_cache_row < 0 || last_cache_row >= DM_CACHE_ROWS) return;
        if (last_cache_way < 0 || last_cache_way >= DM_CACHE_WAYS) return;
        flags = ~a_val;
        cache_dirty[last_cache_row][last_cache_way]        = (flags & PIPE5_DIRTY)      != 0;
        cache_vacant[last_cache_row][last_cache_way]       = (flags & PIPE5_VACANT)     != 0;
        cache_wp[last_cache_row][last_cache_way]           = (flags & PIPE5_WP)         != 0;
        cache_being_loaded[last_cache_row][last_cache_way] = (flags & PIPE5_BEING_LOAD) != 0;
    endtask

    task automatic br_lo_load(input logic [4:0] membase, input logic [15:0] a_val);
        if (mcr_disbr()) return;
        br[membase][15:0] = a_val;
    endtask

    task automatic br_hi_load(input logic [4:0] membase, input logic [15:0] a_val);
        if (mcr_disbr()) return;
        br[membase][27:16] = a_val[11:0];
    endtask

    task automatic mcr_load(input logic [15:0] a_val, input logic [15:0] b_val);
        mcr = (a_val & 16'hFFE0) | (b_val & 16'h0007);
    endtask

    task automatic cache_touch_lru(input logic [5:0] row_idx, input int way);
        int seen;
        seen = -1;
        for (int i = 0; i < DM_CACHE_WAYS; i++) begin
            if (int'(cache_lru[row_idx][i]) == way) begin
                seen = i;
                break;
            end
        end
        if (seen < 0) return;
        for (int i = seen; i > 0; i--) begin
            cache_lru[row_idx][i] = cache_lru[row_idx][i - 1];
        end
        cache_lru[row_idx][0] = 2'(way);
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
        if (fault_count > 4'd0 &&
            (kind == DM_REF_IFETCH || kind == DM_REF_PREFETCH) &&
            srn == fault_first_srn &&
            srn == proc_srn) begin
            return;
        end
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
    endtask

    task automatic cache_writeback_line(
        input  logic [5:0]     row_idx,
        input  int             way,
        output dm_fault_kind_t out_fault
    );
        logic [27:0] va_base;
        int unsigned phys;
        dm_fault_kind_t f;
        logic [15:0] idx;

        if (!cache_valid[row_idx][way] || !cache_dirty[row_idx][way]) begin
            out_fault = DM_FAULT_NONE;
            return;
        end
        va_base = {cache_tag[row_idx][way], row_idx, 4'b0000};
        f = va_translate(va_base, 1'b1, phys);
        if (f == DM_FAULT_NONE) begin
            for (int i = 0; i < DM_CACHE_LINE_W; i++) begin
                if ((phys + i) < STORAGE_WORDS) begin
                    storage[phys + i] = cache_data[row_idx][way][i];
                end
            end
            idx = map_index(va_base);
            map_ref[idx]   = 1'b1;
            map_dirty[idx] = 1'b1;
        end
        cache_dirty[row_idx][way] = 1'b0;
        out_fault = f;
    endtask

    task automatic record_writeback_fault(
        input dm_fault_kind_t f,
        input logic [3:0]     srn
    );
        if (f == DM_FAULT_NONE) return;
        if (fault_count == 4'd0) begin
            fault_first_srn = srn;
            fault_emulator  = 1'b1;
        end
        if (fault_count < 4'hF) fault_count = fault_count + 4'd1;
        last_fault = f;
        pipe_pipe4_errors[srn] |= PIPE4_ERR_MAP_TROUBLE;
    endtask

    task automatic cache_fill(input logic [27:0] va, input int way);
        logic [5:0]  r;
        logic [15:0] idx;
        int unsigned phys;
        dm_fault_kind_t f;

        f = va_translate(va_munch_base(va), 1'b0, phys);
        if (f != DM_FAULT_NONE) return;

        r   = va_cache_row(va);
        idx = map_index(va);
        cache_tag[r][way]          = va_cache_tag(va);
        cache_valid[r][way]        = 1'b1;
        cache_dirty[r][way]        = 1'b0;
        cache_wp[r][way]           = map_wp[idx];
        cache_vacant[r][way]       = 1'b0;
        cache_being_loaded[r][way] = 1'b0;

        for (int i = 0; i < DM_CACHE_LINE_W; i++) begin
            cache_data[r][way][i] = ((phys + i) < STORAGE_WORDS) ? storage[phys + i] : 16'd0;
        end
        map_ref[idx] = 1'b1;
        cache_touch_lru(r, way);
    endtask

    // Complete processor/IFU memory reference (matching dorado_memory_ref_task)
    task automatic memory_ref_task(
        input  dm_ref_kind_t   kind,
        input  logic [27:0]    va,
        input  logic [15:0]    b_val,
        input  logic [7:0]     tioa_val,
        input  logic [3:0]     task_id,
        input  logic [1:0]     subtask_id,
        output dm_fault_kind_t out_fault
    );
        logic [15:0]    idx_snap, rp_pre;
        logic [2:0]     flags_pre;
        logic           io_task_uses_asrn, use_asrn;
        logic [3:0]     srn;
        int             way, victim, asrn_advance_count;
        int unsigned    phys;
        dm_fault_kind_t f, wbf;
        logic [5:0]     r;

        last_ref_kind    = kind;
        last_ref_va      = va;
        last_ref_b       = b_val;
        last_ref_task    = task_id;
        last_ref_subtask = subtask_id;
        last_ref_miss    = 1'b0;

        mar       = va;
        idx_snap  = map_index(va);
        rp_pre    = map_rp[idx_snap];
        flags_pre = {map_ref[idx_snap], map_dirty[idx_snap], map_wp[idx_snap]};

        io_task_uses_asrn = (task_id != 4'd0 && task_id != 4'o17);
        use_asrn = io_task_uses_asrn ||
                   (kind == DM_REF_IOFETCH) || (kind == DM_REF_IOSTORE) ||
                   (kind == DM_REF_IFETCH);

        if (kind == DM_REF_PREFETCH) begin
            int dummy_way;
            if (!mcr_noref() && (mcr_fdmiss() || !cache_lookup(va, dummy_way))) begin
                use_asrn = 1'b1;
            end
        end

        srn = use_asrn ? asrn : proc_srn;
        r   = va_cache_row(va);

        if (mcr_dvavic() &&
            kind != DM_REF_MAP && kind != DM_REF_RMAP &&
            kind != DM_REF_DUMMYREF && kind != DM_REF_NONE) begin
            logic [27:0] pipe_va_victim;
            way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
            pipe_va_victim = cache_valid[r][way] ? {cache_tag[r][way], r, 4'b0000} : 28'd0;
            pipe_push(srn, kind, pipe_va_victim, rp_pre, flags_pre, task_id, subtask_id);
            cache_select(va, way, srn);
            out_fault = DM_FAULT_NONE;
            return;
        end

        pipe_push(srn, kind, va, rp_pre, flags_pre, task_id, subtask_id);

        f = DM_FAULT_NONE;
        asrn_advance_count = 0;

        case (kind)
            DM_REF_FETCH, DM_REF_IFETCH, DM_REF_LONGFETCH: begin
                last_ref_latency = 3;
                if (mcr_noref()) begin
                    if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                        // comparator hit
                    end else begin
                        way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                    end
                    cache_select(va, way, srn);
                    f = DM_FAULT_NONE;
                end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                    md = cache_data[r][way][va_cache_offset(va)];
                    cache_touch_lru(r, way);
                    cache_select(va, way, srn);
                end else begin
                    last_ref_miss = 1'b1;
                    if (use_asrn) asrn_advance_count = 1;
                    f = va_translate(va, 1'b0, phys);
                    if (f == DM_FAULT_NONE) begin
                        logic dirty_victim;
                        victim       = cache_pick_victim(va);
                        dirty_victim = cache_dirty[r][victim];
                        last_ref_latency = dirty_victim ? 24 : 16;
                        if (use_asrn && dirty_victim) asrn_advance_count = 2;
                        cache_writeback_line(r, victim, wbf);
                        record_writeback_fault(wbf, srn);
                        cache_fill(va, victim);
                        md = cache_data[r][victim][va_cache_offset(va)];
                        cache_select(va, victim, srn);
                    end
                end
            end

            DM_REF_PREFETCH: begin
                if (!mcr_noref()) begin
                    int dummy_way;
                    f = va_translate(va, 1'b0, phys);
                    if (f == DM_FAULT_NONE || f == DM_FAULT_STORAGE_ERROR) begin
                        map_ref[idx_snap] = 1'b1;
                    end
                    if (f == DM_FAULT_NONE && (mcr_fdmiss() || !cache_lookup(va, dummy_way))) begin
                        logic dirty_victim;
                        last_ref_miss = 1'b1;
                        victim        = cache_pick_victim(va);
                        dirty_victim  = cache_dirty[r][victim];
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
                        cache_tag[r][way]   = va_cache_tag(va);
                        cache_valid[r][way] = 1'b1;
                    end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                        // comparator-only
                    end else begin
                        way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                    end
                    cache_select(va, way, srn);
                    f = DM_FAULT_NONE;
                end else if (!mcr_fdmiss() && cache_lookup(va, way)) begin
                    if (cache_wp[r][way]) begin
                        cache_touch_lru(r, way);
                        cache_select(va, way, srn);
                    end else begin
                        cache_data[r][way][va_cache_offset(va)] = b_val;
                        cache_dirty[r][way]  = 1'b1;
                        cache_vacant[r][way] = 1'b0;
                        cache_touch_lru(r, way);
                        cache_select(va, way, srn);
                    end
                end else begin
                    f = va_translate(va, 1'b1, phys);
                    if (use_asrn) asrn_advance_count = 1;
                    if (f == DM_FAULT_NONE) begin
                        logic dirty_victim;
                        last_ref_miss = 1'b1;
                        way           = cache_pick_victim(va);
                        dirty_victim  = cache_dirty[r][way];
                        if (use_asrn && dirty_victim) asrn_advance_count = 2;
                        cache_writeback_line(r, way, wbf);
                        record_writeback_fault(wbf, srn);
                        cache_fill(va, way);
                        cache_data[r][way][va_cache_offset(va)] = b_val;
                        cache_dirty[r][way]  = 1'b1;
                        cache_vacant[r][way] = 1'b0;
                        cache_touch_lru(r, way);
                        cache_select(va, way, srn);
                    end
                end
            end

            DM_REF_IOFETCH: begin
                if (mcr_noref()) begin
                    way = mcr_usemcrv() ? int'(mcr_victim()) : cache_pick_victim(va);
                    cache_select(va, way, srn);
                    f = DM_FAULT_NONE;
                end else begin
                    f = va_translate(va, 1'b0, phys);
                    if (f == DM_FAULT_NONE) begin
                        int unsigned base;
                        map_ref[idx_snap] = 1'b1;
                        if (use_asrn) asrn_advance_count = 1;
                        if (cache_lookup(va, way)) begin
                            for (int i = 0; i < 16; i++) begin
                                fastio_out_munch[i] = cache_data[r][way][i];
                            end
                            cache_touch_lru(r, way);
                            cache_select(va, way, srn);
                        end else begin
                            base = phys & ~32'hF;
                            for (int i = 0; i < 16; i++) begin
                                fastio_out_munch[i] = ((base + i) < STORAGE_WORDS) ? storage[base + i] : 16'd0;
                            end
                        end
                        fastio_stb     = 1'b1;
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
                    f = DM_FAULT_NONE;
                end else begin
                    f = va_translate(va, 1'b1, phys);
                    if (f == DM_FAULT_NONE) begin
                        int unsigned base;
                        if (cache_lookup(va, way)) begin
                            cache_valid[r][way]  = 1'b0;
                            cache_dirty[r][way]  = 1'b0;
                            cache_vacant[r][way] = 1'b1;
                        end
                        map_ref[idx_snap]   = 1'b1;
                        map_dirty[idx_snap] = 1'b1;
                        if (use_asrn) asrn_advance_count = 1;
                        base = phys & ~32'hF;
                        for (int i = 0; i < 16; i++) begin
                            if ((base + i) < STORAGE_WORDS) begin
                                storage[base + i] = fastio_in_munch[i];
                            end
                        end
                        fastio_stb     = 1'b1;
                        fastio_kind    = DM_REF_IOSTORE;
                        fastio_task    = task_id;
                        fastio_subtask = subtask_id;
                        fastio_va      = va_munch_base(va);
                    end
                end
            end

            DM_REF_MAP: begin
                map_rp[idx_snap]        = b_val;
                map_wp[idx_snap]        = tioa_val[7];
                map_dirty[idx_snap]     = tioa_val[6];
                map_ref[idx_snap]       = 1'b0;
                pipe_mapbuf_busy[srn]   = 1'b1;
                mapbuf_busy_slot        = int'(srn);
                mapbuf_busy_cycles      = 9;
            end

            DM_REF_FLUSH: begin
                if (cache_lookup(va, way)) begin
                    cache_writeback_line(r, way, wbf);
                    record_writeback_fault(wbf, srn);
                    if (use_asrn) asrn_advance_count = 1;
                    cache_valid[r][way]  = 1'b0;
                    cache_vacant[r][way] = 1'b1;
                    cache_select(va, way, srn);
                end else begin
                    way = cache_pick_victim(va);
                    cache_select(va, way, srn);
                end
            end

            DM_REF_RMAP: begin
                // Pipe entry already captured rp_pre / flags_pre
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

            default: ;
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
                if (fault_count == 4'd0) begin
                    fault_first_srn = srn;
                end
                if (task_id == 4'd0) fault_emulator = 1'b1;
                if (fault_count < 4'hF) fault_count = fault_count + 4'd1;
            end
        end

        if (use_asrn) begin
            for (int i = 0; i < asrn_advance_count; i++) begin
                if (asrn == 4'd15) asrn = 4'd2;
                else               asrn = asrn + 4'd1;
            end
        end

        out_fault = f;
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

endmodule
