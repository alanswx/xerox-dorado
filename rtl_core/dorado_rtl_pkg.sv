// =============================================================================
// dorado_rtl_pkg.sv — Xerox Dorado Clean RTL Package & Pure Decoders
// =============================================================================
// Derived from the schematic-audited C reference (dorado/src/cpu.c, memory.c).
// Implements 1-clock-per-microinstruction types, constants, and combinational
// instruction/FF/memory decoders matching the Dorado Hardware Manual (HM).
// =============================================================================

package dorado_rtl_pkg;

    // -------------------------------------------------------------------------
    // Architectural Constants
    // -------------------------------------------------------------------------
    localparam int IM_SIZE        = 4096;
    localparam int IFUM_SIZE      = 1024;
    localparam int RM_SIZE        = 256;
    localparam int STK_SIZE       = 256;
    localparam int ALUFM_SIZE     = 16;
    localparam int NUM_TASKS      = 16;
    localparam int DM_BR_COUNT    = 32;
    localparam int DM_MAP_ENTRIES = 65536;
    localparam int DM_PAGE_SIZE   = 256;
    localparam int DM_PAGE_SHIFT  = 8;
    localparam int DM_PIPE_DEPTH  = 16;
    localparam int DM_CACHE_ROWS  = 64;
    localparam int DM_CACHE_WAYS  = 4;
    localparam int DM_CACHE_LINE_W = 16;

    // -------------------------------------------------------------------------
    // Decoded 34-bit Microinstruction (plus packed words for IM parity)
    // -------------------------------------------------------------------------
    typedef struct packed {
        logic [3:0]  rstk;
        logic [3:0]  aluf;
        logic [2:0]  bsel;
        logic [2:0]  lc;
        logic [2:0]  asel;
        logic        block;
        logic [7:0]  ff;
        logic [7:0]  jcn;
        logic [1:0]  brk_p;
        logic [15:0] iw0;
        logic [15:0] iw1;
        logic [15:0] iw2;
    } dorado_uinstr_t;

    // -------------------------------------------------------------------------
    // Memory Reference & Fault Enumerations (matching memory.h)
    // -------------------------------------------------------------------------
    typedef enum logic [3:0] {
        DM_REF_NONE      = 4'd0,
        DM_REF_PREFETCH  = 4'd1,
        DM_REF_MAP       = 4'd2,
        DM_REF_IOFETCH   = 4'd3,
        DM_REF_LONGFETCH = 4'd4,
        DM_REF_STORE     = 4'd5,
        DM_REF_DUMMYREF  = 4'd6,
        DM_REF_FLUSH     = 4'd7,
        DM_REF_IOSTORE   = 4'd8,
        DM_REF_IFETCH    = 4'd9,
        DM_REF_FETCH     = 4'd10,
        DM_REF_RMAP      = 4'd11
    } dm_ref_kind_t;

    typedef enum logic [2:0] {
        DM_FAULT_NONE          = 3'd0,
        DM_FAULT_PAGE          = 3'd1,
        DM_FAULT_WRITE_PROTECT = 3'd2,
        DM_FAULT_MAP_TROUBLE   = 3'd3,
        DM_FAULT_STORAGE_ERROR = 3'd4
    } dm_fault_kind_t;

    // -------------------------------------------------------------------------
    // Pack / Unpack Microinstruction Words (matching disasm.c)
    // -------------------------------------------------------------------------
    function automatic dorado_uinstr_t unpack_uinstr(
        input logic [15:0] iw0,
        input logic [15:0] iw1,
        input logic [15:0] iw2
    );
        dorado_uinstr_t u;
        u.rstk  = {iw2[15], iw0[15:13]};
        u.aluf  = iw0[12:9];
        u.bsel  = iw0[8:6];
        u.lc    = iw0[5:3];
        u.asel  = iw0[2:0];
        u.block = iw1[15];
        u.ff    = iw1[14:7];
        u.jcn   = {iw1[6:0], iw2[14]};
        u.brk_p = iw2[13:12];
        u.iw0   = iw0;
        u.iw1   = iw1;
        u.iw2   = iw2;
        return u;
    endfunction

    // -------------------------------------------------------------------------
    // Bit-Reversal Helpers (MSB-first Dorado manual <-> LSB-first C/SV)
    // -------------------------------------------------------------------------
    function automatic logic [3:0] bit_rev4(input logic [3:0] x);
        return {x[0], x[1], x[2], x[3]};
    endfunction

    function automatic logic [4:0] bit_rev5(input logic [4:0] x);
        return {x[0], x[1], x[2], x[3], x[4]};
    endfunction

    function automatic logic [5:0] bit_rev6(input logic [5:0] x);
        return {x[0], x[1], x[2], x[3], x[4], x[5]};
    endfunction

    // -------------------------------------------------------------------------
    // FF Field Decoding Helpers (matching cpu.c:1441-1510)
    // -------------------------------------------------------------------------
    function automatic logic [1:0] ff_fa(input logic [7:0] ff);
        return ff[7:6];
    endfunction

    function automatic logic [2:0] ff_fb(input logic [7:0] ff);
        return ff[5:3];
    endfunction

    function automatic logic [2:0] ff_fc(input logic [7:0] ff);
        return ff[2:0];
    endfunction

    // Long jump detection: JCN[0]=0 and JCN[1:3]=0 (in C/SV: jcn[7:4]==4'b0000)
    function automatic logic jcn_is_long_jump(input logic [7:0] jcn);
        return (jcn[7:4] == 4'b0000);
    endfunction

    // FF decodes as function iff BSEL < 4 and JCN is not a long jump
    function automatic logic ff_decode_ok(input dorado_uinstr_t u);
        return (u.bsel < 3'd4) && !jcn_is_long_jump(u.jcn);
    endfunction

    // Full 8-bit FF function (FA, FB, FC all available) iff ASEL > 3
    function automatic logic ff_full_function_ok(input dorado_uinstr_t u);
        return ff_decode_ok(u) && (u.asel > 3'd3);
    endfunction

    // Table 11a predicate: matches (ff & mask) == code when FA=0 or when ASEL<=3
    function automatic logic ff_is_table11a(
        input dorado_uinstr_t u,
        input logic [7:0] code,
        input logic [7:0] mask
    );
        if (!ff_decode_ok(u)) return 1'b0;
        if (u.asel > 3'd3) return ((u.ff & (mask | 8'o300)) == (code & 8'o077));
        return ((u.ff & (mask & 8'o077)) == (code & 8'o077));
    endfunction

    // Signed 3-bit two's complement stack delta from RSTK[2:0]
    function automatic logic [7:0] stk_signed_delta(input logic [3:0] rstk);
        case (rstk[2:0])
            3'd0: return 8'd0;
            3'd1: return 8'd1;
            3'd2: return 8'd2;
            3'd3: return 8'd3;
            3'd4: return 8'hFC; // -4
            3'd5: return 8'hFD; // -3
            3'd6: return 8'hFE; // -2
            3'd7: return 8'hFF; // -1
        endcase
    endfunction

    // ModStkPBeforeW = Table 11a 0027 (FA=0, FB=2, FC=7)
    function automatic logic ff_is_mod_stkp_before_w(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o027, 8'o077);
    endfunction

    // Don't-Modify-StkP = Table 11a 0026 (FA=0, FB=2, FC=6)
    function automatic logic ff_is_dont_modify_stkp(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o026, 8'o077);
    endfunction

    // FreezeBC = Table 11a 0076 (FA=0, FB=7, FC=6)
    function automatic logic ff_is_freeze_bc(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o076, 8'o077);
    endfunction

    // TgetsMd = Table 11a 0075 (FA=0, FB=7, FC=5)
    function automatic logic ff_is_t_gets_md(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o075, 8'o077);
    endfunction

    // RisId = Table 11a 0034 (FA=0, FB=3, FC=4)
    function automatic logic ff_is_r_is_id(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o034, 8'o077);
    endfunction

    // TisId = Table 11a 0035 (FA=0, FB=3, FC=5)
    function automatic logic ff_is_t_is_id(input dorado_uinstr_t u);
        return ff_is_table11a(u, 8'o035, 8'o077);
    endfunction

    // Decode Memory Reference Kind (Table 8a + FF=0226 RMap<-)
    function automatic dm_ref_kind_t decode_mem_ref(input dorado_uinstr_t u);
        logic [1:0] hi2;
        if (ff_full_function_ok(u) && u.ff == 8'o226) begin
            return DM_REF_RMAP;
        end
        if (u.asel > 3'd3) begin
            return DM_REF_NONE;
        end
        hi2 = ff_decode_ok(u) ? u.ff[7:6] : 2'd0;
        case ({u.asel[1:0], hi2})
            {2'd0, 2'd0}: return DM_REF_NONE;
            {2'd0, 2'd1}: return DM_REF_PREFETCH;
            {2'd0, 2'd2}: return DM_REF_MAP;
            {2'd0, 2'd3}: return DM_REF_IOFETCH;
            {2'd1, 2'd0}: return DM_REF_LONGFETCH;
            {2'd1, 2'd1}: return DM_REF_STORE;
            {2'd1, 2'd2}: return DM_REF_DUMMYREF;
            {2'd1, 2'd3}: return DM_REF_FLUSH;
            {2'd2, 2'd0}, {2'd2, 2'd1}, {2'd2, 2'd2}, {2'd2, 2'd3}: return DM_REF_IOSTORE;
            {2'd3, 2'd0}, {2'd3, 2'd1}, {2'd3, 2'd2}, {2'd3, 2'd3}: return DM_REF_FETCH;
        endcase
    endfunction

    // Host-port operations (dorado_rtl_cpu ap_op): the C machine's host
    // services, done between microinstructions. Each mirrors one C routine.
    localparam logic [2:0] AP_ASSIST    = 3'd0; // loader assist (ap_we/ap_stk/ap_cache)
    localparam logic [2:0] AP_STORE     = 3'd1; // machine_store_va / dorado_poke_va: storage
                                                // (storage[va] if the entry is vacant) and
                                                // any resident line (drive ap_cache=1)
    localparam logic [2:0] AP_STORE_INV = 3'd2; // dorado_storage_store_at_va: storage, and
                                                // the resident line is dropped
    localparam logic [2:0] AP_PEEK      = 3'd3; // a save's look at the memory section, by ap_wd:
                                                // 0 the Map entry of ap_va's page: RP; 1 its
                                                // {WP, Dirty, Ref}; 2, 3 (way in ap_wd[5:4]) the
                                                // cache line of ap_va's row: tag[15:0];
                                                // {being loaded, WP, vacant, dirty, valid, tag[17:16]}
                                                // with ap_wd[3] (a scan core): the control store,
                                                // from the processor (dorado_rtl_cpu x_pk)
    localparam logic [2:0] AP_RM_OR     = 3'd4; // RM/STK[ap_va[8:0]] |= ap_wd (naked notify; STK at 400B up;
                                                // with ap_wd 0, a read)
    localparam logic [2:0] AP_WAKEUP_OR = 3'd5; // wakeup_pending |= ap_wd (dorado_cpu_wakeup)
    localparam logic [2:0] AP_RESCHED   = 3'd6; // reschedule_pending = ap_wd[7:0]

endpackage
