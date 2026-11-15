// =============================================================================
// dorado_rtl_top.sv — Top-Level Wrapper for Dorado 1-Cycle RTL Core + I/O
// =============================================================================
// Wraps dorado_rtl_cpu and dorado_rtl_io for Verilator cosimulation and FPGA
// integration. When io_bridge_en=1, connects the internal RTL I/O controller
// subsystem (dorado_rtl_io) directly to the CPU's Slow I/O, Fast I/O, DMux,
// and external task wakeup buses.
// =============================================================================

module dorado_rtl_top
    import dorado_rtl_pkg::*;
#(
    parameter int STORAGE_WORDS = 4 * 1024 * 1024,
    parameter logic [15:0] RESET_PC = 16'd0,
    parameter int BOOT_WORDS    = 28619,
    parameter     BOOT_HEX      = "roms/bootfile.hex",
    parameter     BOL_HEX       = "roms/bol.hex",
    parameter bit EXT_STORAGE   = 1'b0,          // 1: storage on the sto_* port (MiSTer: DDR3)
    // 1 (with EXT_STORAGE): the HPS drives the host port and the Ethernet
    // receive buffer through a mailbox in DDR3 (dorado_rtl_mailbox) instead
    // of the hp_* / hx_rx_* ports, storage is not cleared at reset (the HPS
    // loads it), and the core is held until the HPS says go.
    parameter bit HPS_MAILBOX   = 1'b0,
    parameter bit PIPELINE      = 1'b0           // the CPU's C_PIPE (dorado_rtl_cpu)
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        clk_en,
    // the MiSTer wrapper's PLL lock and CLK_50M count (status beat 13)
    input  logic        pll_locked,
    input  logic [31:0] ref_cnt,

    // BaseBoard / MIR single-step injection interface
    input  logic        bb_present,
    input  logic        bb_running,
    input  logic        bb_ss_inject,
    input  logic [15:0] bb_mir_iw0,
    input  logic [15:0] bb_mir_iw1,
    input  logic [15:0] bb_mir_iw2,

    // Mode select: 1 = use internal dorado_rtl_io bridge; 0 = external/disconnected
    input  logic        io_bridge_en,
    // 1 = the internal Trident controller alone is the disk (task 014, TIOA
    // 010-014, its DMux bits, its IOStore munch and its wakeup), whatever
    // io_bridge_en says about the other devices. A lock-step harness sets it
    // to check the RTL disk against the C one while the C answers the rest.
    input  logic        disk_bridge_en,
    // 1 = rotate the disk on disk_time instead of the microinstruction count
    // (the C emulator's disk clock is bb.cycles, ~3.70 per microinstruction)
    input  logic        disk_time_ext_en,
    input  logic [63:0] disk_time,
    // 1 = the in-FPGA boot server (dorado_rtl_bootsrv) answers the Ethernet
    input  logic        bootsrv_en,
    // 1 = dorado_rtl_assist performs the loader assists (cold-Alto init,
    //     keyboard words, display gate) instead of the testbench
    input  logic        assist_en,
    input  logic        display_wake_gate,
    input  logic [5:0]  display_top_lines,   // see dorado_rtl_io
    input  logic [9:0]  display_cursor_dx,   // see dorado_rtl_io
    // Live keyboard, the four Alto keyboard words 177034B-177037B, active
    // low (Alto HW Manual: depressed = 0): word i = kbd_words[16*i +: 16].
    input  logic [63:0] kbd_words,
    // Mouse: buttons (1 = down; bit 2 Red/left, 1 Blue/right, 0 Yellow/middle, as
    // machine_seed_utilin), present once the host has seen a mouse, and the
    // Alto MOUSELOC position.
    input  logic [2:0]  mouse_buttons,
    input  logic        mouse_present,
    input  logic [15:0] mouse_x,
    input  logic [15:0] mouse_y,
    // Mouse motion for the terminal back channel (HM Table 24 message 06B):
    // signed deltas, X right and Y down positive, one pulse of
    // mouse_delta_stb per report. The display microcode turns them into the
    // guest's mouse position (Cedar's DisplayAux); the Alto worlds take the
    // absolute position above instead and leave this idle.
    input  logic signed [15:0] mouse_dx,
    input  logic signed [15:0] mouse_dy,
    input  logic        mouse_delta_stb,

    // Host port: the C machine's host services (Cedar's disk, network, timer
    // and keyboard on the HPS) reach the core here, one operation at a time
    // between microinstructions, while hp_hold keeps the CPU there. hp_op is
    // an AP_* code (dorado_rtl_pkg). Used when the loader assist is off.
    input  logic        hp_hold,
    input  logic        ov_hold,        // 1: no C_PIPE overlap (the per-step lock-step)
    input  logic        hp_req,
    input  logic [2:0]  hp_op,
    input  logic        hp_we,
    input  logic        hp_cache,
    input  logic [27:0] hp_va,
    input  logic [15:0] hp_wd,
    output logic        hp_done,
    output logic [15:0] hp_rd,
    output logic        hp_between,    // the CPU is between microinstructions

    // Ethernet receive buffer fill from the host (the HPS's Pup/STP server),
    // used when the boot server is off: words (with their attention flags)
    // at hx_rx_wr_addr, then hx_rx_load with the packet length. hx_rx_on and
    // hx_rx_drained report the receiver (RxOn; every loaded word read).
    input  logic        hx_rx_wr_en,
    input  logic [11:0] hx_rx_wr_addr,
    input  logic [15:0] hx_rx_wr_data,
    input  logic        hx_rx_wr_att,
    input  logic        hx_rx_load,
    input  logic [12:0] hx_rx_load_count,
    output logic        hx_rx_on,
    output logic        hx_rx_drained,

    // External Slow I/O override (used when io_bridge_en=0)
    input  logic        io_attached,
    input  logic [15:0] io_read_data,
    input  logic        io_read_bad_parity,
    input  logic        io_has_write_in,
    input  logic        io_atten_in,
    output logic        io_write_stb,
    output logic [3:0]  io_write_task,
    output logic [1:0]  io_write_subtask,
    output logic [7:0]  io_write_tioa,
    output logic [15:0] io_write_data,

    // External I/O Peripheral Signals (for u_io when io_bridge_en=1)
    input  logic [7:0]  eth_local_host,
    output logic        eth_tx_packet_stb,
    output logic [10:0] eth_tx_packet_words,

    output logic [1:0]  disk_selected_drive_out,
    output logic [11:0] disk_cur_cyl_out,
    output logic [5:0]  disk_cur_head_out,
    output logic [6:0]  disk_cur_sector_out,
    output logic        disk_sector_read_req,
    output logic        disk_sector_write_req,

    // Video scan-out of the DDC framebuffer (1024x808, 1 bpp, MSB = leftmost)
    input  logic [19:0] fb_rd_addr,      // y * 1024 + x
    output logic        fb_rd_data,      // 1 = pixel set

    // Observability outputs
    output logic [11:0] dbg_real_pc,
    output logic [3:0]  dbg_ctask,
    output logic [15:0] dbg_t,
    output logic [15:0] dbg_q,
    output logic [15:0] dbg_link,
    output logic [15:0] dbg_cnt,
    output logic [7:0]  dbg_stkp,
    output logic [63:0] dbg_cycles,
    output logic [31:0] world_cfg,      // the world's presentation (dorado_rtl_mailbox, control beat 1)
    output logic        dbg_halted,
    output logic [3:0]  dbg_halt_reason,
    output logic        step_done,     // pulses one clock after each microinstruction
    output logic        cmp_done,      // ...and its dispatch's byte references are booked: the compare point
    // Board-level debug: {DDR engine (see dorado_rtl_storage_ddr), CPU state,
    // real PC, task, low cycle count}
    output logic [127:0] dbg_vec,

    // Storage in DDR3 when EXT_STORAGE (dorado_rtl_storage_ddr): the MiSTer
    // framework's DDRAM port. The core is held in reset until the storage
    // region has been cleared.
    input  logic         DDRAM_BUSY,
    output logic [7:0]   DDRAM_BURSTCNT,
    output logic [28:0]  DDRAM_ADDR,
    input  logic [63:0]  DDRAM_DOUT,
    input  logic         DDRAM_DOUT_READY,
    output logic         DDRAM_RD,
    output logic [63:0]  DDRAM_DIN,
    output logic [7:0]   DDRAM_BE,
    output logic         DDRAM_WE
);
    logic           io_busy;
    logic           io_jobs_busy;
    logic           srv_stall;
    logic           srv_rx_wr_en, srv_rx_wr_att, srv_rx_load;
    logic [11:0]    srv_rx_wr_addr;
    logic [15:0]    srv_rx_wr_data;
    logic [12:0]    srv_rx_load_count;
    logic           rx_wr_en, rx_wr_att, rx_load;
    logic [11:0]    rx_wr_addr;
    logic [15:0]    rx_wr_data;
    logic [12:0]    rx_load_count;
    wire            srv_on = bootsrv_en && io_bridge_en;
    assign rx_wr_en      = srv_on ? srv_rx_wr_en      : HPS_MAILBOX ? mb_rx_wr_en      : hx_rx_wr_en;
    assign rx_wr_addr    = srv_on ? srv_rx_wr_addr    : HPS_MAILBOX ? mb_rx_wr_addr    : hx_rx_wr_addr;
    assign rx_wr_data    = srv_on ? srv_rx_wr_data    : HPS_MAILBOX ? mb_rx_wr_data    : hx_rx_wr_data;
    assign rx_wr_att     = srv_on ? srv_rx_wr_att     : HPS_MAILBOX ? mb_rx_wr_att     : hx_rx_wr_att;
    assign rx_load       = srv_on ? srv_rx_load       : HPS_MAILBOX ? mb_rx_load       : hx_rx_load;
    assign rx_load_count = srv_on ? srv_rx_load_count : HPS_MAILBOX ? mb_rx_load_count : hx_rx_load_count;
    assign hx_rx_on      = io_rx_on;
    assign hx_rx_drained = io_rx_drained;
    logic [9:0]     srv_tx_rd_addr;
    logic [15:0]    io_tx_rd_data;
    logic           io_rx_on, io_rx_drained;
    logic [14:0]    io_rx_loads;
    logic [15:0]    srv_eftp_max_seq, srv_bol_sent;
    logic           ap_hold, ap_req, ap_we, ap_stk, ap_cache, ap_done;
    logic [2:0]     ap_op;
    logic [27:0]    ap_va;
    logic [15:0]    ap_wd, ap_rd;
    logic           as_hold, as_req, as_we, as_stk, as_cache;
    logic [27:0]    as_va;
    logic [15:0]    as_wd;
    wire            assist_on = assist_en && io_bridge_en;
    // the host port and receive fill: from the pins, or the mailbox
    logic           h_hold, h_req, h_we, h_cache;
    logic [2:0]     h_op;
    logic [27:0]    h_va;
    logic [15:0]    h_wd;
    logic           mb_hold, mb_req, mb_we, mb_cache, mb_go;
    // the state scan chain: the mailbox -> u_io -> u_cpu (its memory section, then itself)
    logic           mb_scan_ph, mb_scan_en, mb_scan_in, scan_out_io, scan_out_cpu;
    logic           mb_ld_we;
    logic [63:0]    mb_ld_d;
    logic [2:0]     mb_op;
    logic [27:0]    mb_va;
    logic [15:0]    mb_wd;
    logic           mb_rx_wr_en, mb_rx_wr_att, mb_rx_load;
    logic [11:0]    mb_rx_wr_addr;
    logic [15:0]    mb_rx_wr_data;
    logic [12:0]    mb_rx_load_count;
    logic [9:0]     mb_tx_rd_addr;
    logic           aux_req, aux_we, aux_ack;
    logic           mb_aux_req, mb_aux_we, mb_aux_ack;
`ifdef DORADO_DISK_DPI
    localparam bit          DISK_ON_AUX    = 1'b0;
    localparam logic [28:0] DISK_PACK_BASE = 29'd0;
`else
    localparam bit          DISK_ON_AUX    = EXT_STORAGE;
    localparam logic [28:0] DISK_PACK_BASE = EXT_STORAGE ? 29'h0620_0000 : 29'd0;   // byte 0x3100_0000
`endif
    // the disk engine's pack memory port (see dorado_rtl_io's sector data engine)
    logic        disk_mem_req, disk_mem_we, disk_mem_ack;
    logic [28:0] disk_mem_addr;
    logic [63:0] disk_mem_wdata, disk_mem_rdata;
    logic [7:0]  disk_mem_be;
    logic [28:0]    mb_aux_addr;
    logic [63:0]    mb_aux_wdata;
    logic [7:0]     mb_aux_be;
    logic [28:0]    aux_addr;
    logic [63:0]    aux_wdata, aux_rdata;
    logic [7:0]     aux_be;
    assign h_hold  = HPS_MAILBOX ? mb_hold  : hp_hold;
    assign h_req   = HPS_MAILBOX ? mb_req   : hp_req;
    assign h_op    = HPS_MAILBOX ? mb_op    : hp_op;
    assign h_we    = HPS_MAILBOX ? mb_we    : hp_we;
    assign h_cache = HPS_MAILBOX ? mb_cache : hp_cache;
    assign h_va    = HPS_MAILBOX ? mb_va    : hp_va;
    assign h_wd    = HPS_MAILBOX ? mb_wd    : hp_wd;
    assign ap_hold  = assist_on ? as_hold  : h_hold;
    assign ap_req   = assist_on ? as_req   : h_req;
    assign ap_op    = assist_on ? AP_ASSIST : h_op;
    assign ap_we    = assist_on ? as_we    : h_we;
    assign ap_stk   = assist_on ? as_stk   : 1'b0;
    assign ap_cache = assist_on ? as_cache : h_cache;
    assign ap_va    = assist_on ? as_va    : h_va;
    assign ap_wd    = assist_on ? as_wd    : h_wd;
    assign hp_done  = !assist_on && ap_done;
    assign hp_rd    = ap_rd;
    assign hp_between = cpu_between;
    logic           cpu_between, cpu_quiet, dbg_ifu_active;
    logic           kbd0_poke;
    logic [15:0]    kbd0_val;
    logic           assist_display_gate, io_next_wcb_any;
    dorado_uinstr_t bb_mir_uinstr;
    assign bb_mir_uinstr = unpack_uinstr(bb_mir_iw0, bb_mir_iw1, bb_mir_iw2);

    // Interconnect between CPU/Mem and I/O Bridge
    logic [3:0]     cpu_io_read_task;
    logic [1:0]     cpu_io_read_subtask;
    logic [7:0]     cpu_io_read_tioa;
    // The CPU takes this microinstruction's slow-I/O answer from the internal
    // controller: every device (io_bridge_en), or the disk alone.
    wire            io_int_sel = io_bridge_en ||
                                 (disk_bridge_en && cpu_io_read_task == 4'o14 &&
                                  cpu_io_read_tioa >= 8'o010 && cpu_io_read_tioa <= 8'o014);
    logic           cpu_io_read_stb;
    logic [3:0]     cpu_io_read_stb_task;
    logic [1:0]     cpu_io_read_stb_subtask;
    logic [7:0]     cpu_io_read_stb_tioa;

    logic [15:0]    bridge_io_read_data;
    logic           bridge_io_read_bad_parity;
    logic           bridge_io_has_write;
    logic           bridge_io_atten;

    logic           fastio_stb;
    dm_ref_kind_t   fastio_kind;
    logic [3:0]     fastio_task;
    logic [1:0]     fastio_subtask;
    logic [27:0]    fastio_va;
    logic [15:0]    fastio_out_munch [0:15];
    logic [15:0]    fastio_in_munch  [0:15];
    logic [15:0]    cpu_fastio_in_munch [0:15];

    logic           dmux_ext_handled;
    logic [15:0]    dmux_ext_val;
    logic [11:0]    dmux_cur_addr;

    logic [15:0]    ext_wakeup_req;
    logic           ext_subtask_we;
    logic [3:0]     ext_subtask_task;
    logic [1:0]     ext_subtask_val;

    logic [15:0]    kbd_words_default [0:4];
    always_comb begin
        for (int i = 0; i < 4; i++) kbd_words_default[i] = kbd_words[16 * i +: 16];
        // Word 4 is the terminal's mouse/keyset message (HM Table 24, 05B),
        // active low, bit 2 Red/left, 1 Blue/right, 0 Yellow/middle: as the
        // C model drives it from the mouse (dorado_machine_set_mouse), so the
        // display microcode, a second writer of 177033B, agrees with the host.
        kbd_words_default[4] = ~{13'd0, mouse_buttons};
        for (int i = 0; i < 16; i++) begin
            cpu_fastio_in_munch[i] = (io_bridge_en || (disk_bridge_en && fastio_task == 4'o14)) ? fastio_in_munch[i] : 16'd0;
        end
    end

    // ---------------------------------------------------------------------
    // Storage: block RAM inside the CPU, or DDR3 (EXT_STORAGE)
    // ---------------------------------------------------------------------
    logic         sto_req, sto_we, sto_ack;
    logic [31:0]  sto_addr;
    logic [15:0]  sto_mask;
    logic [255:0] sto_wline, sto_rline;
    logic         sto_ready;
    logic         core_rst;
    logic [63:0]  ddr_dbg;
    logic [4:0]   cpu_st;
    logic [63:0]  cpu_stall;
    assign dbg_vec = {ddr_dbg, 3'd0, cpu_st, 4'd0, dbg_real_pc, 4'd0, dbg_ctask, dbg_cycles[31:0]};

    generate
        if (EXT_STORAGE) begin : g_ddr
            dorado_rtl_storage_ddr #(.STORAGE_WORDS(STORAGE_WORDS), .CLEAR(!HPS_MAILBOX)) u_ddr (
                .clk              (clk),
                .rst              (rst),
                .ready            (sto_ready),
                .aux_req          (aux_req),
                .aux_we           (aux_we),
                .aux_addr         (aux_addr),
                .aux_wdata        (aux_wdata),
                .aux_be           (aux_be),
                .aux_ack          (aux_ack),
                .aux_rdata        (aux_rdata),
                .req              (sto_req),
                .we               (sto_we),
                .addr             (sto_addr),
                .mask             (sto_mask),
                .wline            (sto_wline),
                .ack              (sto_ack),
                .rline            (sto_rline),
                .dbg              (ddr_dbg),
                .DDRAM_BUSY       (DDRAM_BUSY),
                .DDRAM_BURSTCNT   (DDRAM_BURSTCNT),
                .DDRAM_ADDR       (DDRAM_ADDR),
                .DDRAM_DOUT       (DDRAM_DOUT),
                .DDRAM_DOUT_READY (DDRAM_DOUT_READY),
                .DDRAM_RD         (DDRAM_RD),
                .DDRAM_DIN        (DDRAM_DIN),
                .DDRAM_BE         (DDRAM_BE),
                .DDRAM_WE         (DDRAM_WE)
            );
        end else begin : g_no_ddr
            assign aux_ack        = 1'b0;
            assign aux_rdata      = 64'd0;
            assign ddr_dbg        = 64'd0;
            assign sto_ready      = 1'b1;
            assign sto_ack        = 1'b0;
            assign sto_rline      = '0;
            assign DDRAM_BURSTCNT = 8'd0;
            assign DDRAM_ADDR     = 29'd0;
            assign DDRAM_RD       = 1'b0;
            assign DDRAM_DIN      = 64'd0;
            assign DDRAM_BE       = 8'd0;
            assign DDRAM_WE       = 1'b0;
        end
    endgenerate
    generate
        if (EXT_STORAGE && HPS_MAILBOX) begin : g_mailbox
            dorado_rtl_mailbox u_mb (
                .clk           (clk),
                .rst           (rst || !sto_ready),
                .pll_locked    (pll_locked),
                .ref_cnt       (ref_cnt),
                .go            (mb_go),
                .scan_ph       (mb_scan_ph),
                .scan_en       (mb_scan_en),
                .scan_in       (mb_scan_in),
                .scan_out      (scan_out_cpu),
                .ld_we         (mb_ld_we),
                .ld_d          (mb_ld_d),
                .world_cfg     (world_cfg),
                .aux_req       (mb_aux_req),
                .aux_we        (mb_aux_we),
                .aux_addr      (mb_aux_addr),
                .aux_wdata     (mb_aux_wdata),
                .aux_be        (mb_aux_be),
                .aux_ack       (mb_aux_ack),
                .aux_rdata     (aux_rdata),
                .hp_hold       (mb_hold),
                .hp_req        (mb_req),
                .hp_op         (mb_op),
                .hp_we         (mb_we),
                .hp_cache      (mb_cache),
                .hp_va         (mb_va),
                .hp_wd         (mb_wd),
                .hp_done       (ap_done && !assist_on),
                .hp_rd         (ap_rd),
                .hp_between    (cpu_between),
                .hp_quiet      (cpu_quiet),
                .rx_wr_en      (mb_rx_wr_en),
                .rx_wr_addr    (mb_rx_wr_addr),
                .rx_wr_data    (mb_rx_wr_data),
                .rx_wr_att     (mb_rx_wr_att),
                .rx_load       (mb_rx_load),
                .rx_load_count (mb_rx_load_count),
                .rx_on         (io_rx_on),
                .rx_drained    (io_rx_drained),
                .rx_loads      (io_rx_loads),
                .tx_packet_stb (eth_tx_packet_stb),
                .tx_packet_words(eth_tx_packet_words),
                .tx_rd_addr    (mb_tx_rd_addr),
                .tx_rd_data    (io_tx_rd_data),
                .cycles        (dbg_cycles),
                .cpu_st        (cpu_st),
                .cpu_stall     (cpu_stall),
                .ddr_dbg       (ddr_dbg),
                .kbd_words     (kbd_words),
                .mouse_buttons (mouse_buttons),
                .mouse_present (mouse_present),
                .mouse_x       (mouse_x),
                .mouse_y       (mouse_y)
            );
        end else begin : g_no_mailbox
            assign mb_go = 1'b1;
            assign {mb_scan_ph, mb_scan_en, mb_scan_in} = '0;
            assign {mb_ld_we, mb_ld_d} = '0;
            assign world_cfg = 32'd0;
            assign {mb_hold, mb_req, mb_we, mb_cache, mb_op, mb_va, mb_wd} = '0;
            assign {mb_rx_wr_en, mb_rx_wr_att, mb_rx_load, mb_rx_wr_addr, mb_rx_wr_data, mb_rx_load_count} = '0;
            assign {mb_aux_req, mb_aux_we, mb_aux_addr, mb_aux_wdata, mb_aux_be} = '0;
            assign mb_tx_rd_addr = '0;
        end
    endgenerate
    // The storage engine's aux port: the mailbox and the disk engine, one
    // beat at a time (each holds its request until its one-clock ack).
    logic [1:0] aux_owner;          // 0 free, 1 mailbox, 2 disk engine
    always_ff @(posedge clk) begin
        if (rst)                                aux_owner <= 2'd0;
        else if (aux_owner != 2'd0)             begin if (aux_ack) aux_owner <= 2'd0; end
        else if (mb_aux_req)                    aux_owner <= 2'd1;
        else if (DISK_ON_AUX && disk_mem_req)   aux_owner <= 2'd2;
    end
    assign aux_req   = aux_owner == 2'd1 ? mb_aux_req   : aux_owner == 2'd2 ? disk_mem_req   : 1'b0;
    assign aux_we    = aux_owner == 2'd1 ? mb_aux_we    : aux_owner == 2'd2 ? disk_mem_we    : 1'b0;
    assign aux_addr  = aux_owner == 2'd1 ? mb_aux_addr  : aux_owner == 2'd2 ? disk_mem_addr  : 29'd0;
    assign aux_wdata = aux_owner == 2'd1 ? mb_aux_wdata : aux_owner == 2'd2 ? disk_mem_wdata : 64'd0;
    assign aux_be    = aux_owner == 2'd1 ? mb_aux_be    : aux_owner == 2'd2 ? disk_mem_be    : 8'd0;
    assign mb_aux_ack = aux_owner == 2'd1 && aux_ack;
`ifndef DORADO_DISK_DPI
    generate
        if (EXT_STORAGE) begin : g_disk_aux
            assign disk_mem_ack   = aux_owner == 2'd2 && aux_ack;
            assign disk_mem_rdata = aux_rdata;
        end
    endgenerate
`endif
    // (the scan phase: a DORADO_SCAN core is out of reset for it, doing
    // nothing but the shift; a core with its registers compiled in stays
    // in reset until go, whatever the host sends)
`ifdef DORADO_SCAN
    assign core_rst = rst || !sto_ready || !(mb_go || mb_scan_ph);
`else
    assign core_rst = rst || !sto_ready || !mb_go;
`endif

    dorado_rtl_cpu #(
        .STORAGE_WORDS (STORAGE_WORDS),
        .RESET_PC      (RESET_PC),
        .EXT_STORAGE   (EXT_STORAGE),
        .PIPELINE      (PIPELINE)
    ) u_cpu (
        .clk                (clk),
        .rst                (core_rst),
        .clk_en             (clk_en && !mb_scan_ph),
        .scan_ph            (mb_scan_ph),
        .scan_en            (mb_scan_en),
        .scan_si            (scan_out_io),
        .ld_we              (mb_ld_we),
        .ld_d               (mb_ld_d),
        .scan_so            (scan_out_cpu),
        .bb_present         (bb_present),
        .bb_running         (bb_running),
        .bb_ss_inject       (bb_ss_inject),
        .bb_mir_uinstr      (bb_mir_uinstr),

        .io_attached        (io_int_sel ? 1'b1 : io_attached),
        .io_read_data       (io_int_sel ? bridge_io_read_data : io_read_data),
        .io_read_bad_parity (io_int_sel ? bridge_io_read_bad_parity : io_read_bad_parity),
        .io_has_write_in    (io_int_sel ? bridge_io_has_write : io_has_write_in),
        .io_atten_in        (io_int_sel ? bridge_io_atten : io_atten_in),
        .io_read_task       (cpu_io_read_task),
        .io_read_subtask    (cpu_io_read_subtask),
        .io_read_tioa       (cpu_io_read_tioa),
        .io_read_stb        (cpu_io_read_stb),
        .io_read_stb_task    (cpu_io_read_stb_task),
        .io_read_stb_subtask (cpu_io_read_stb_subtask),
        .io_read_stb_tioa    (cpu_io_read_stb_tioa),
        .io_write_stb       (io_write_stb),
        .io_write_task      (io_write_task),
        .io_write_subtask   (io_write_subtask),
        .io_write_tioa      (io_write_tioa),
        .io_write_data      (io_write_data),

        .fastio_stb         (fastio_stb),
        .fastio_kind        (fastio_kind),
        .fastio_task        (fastio_task),
        .fastio_subtask     (fastio_subtask),
        .fastio_va          (fastio_va),
        .fastio_out_munch   (fastio_out_munch),
        .fastio_in_munch    (cpu_fastio_in_munch),

        .dmux_ext_handled   ((io_bridge_en || disk_bridge_en) ? dmux_ext_handled : 1'b0),
        .dmux_ext_val       ((io_bridge_en || disk_bridge_en) ? dmux_ext_val : 16'd0),
        .dmux_cur_addr      (dmux_cur_addr),

        .ext_wakeup_req     (io_bridge_en ? ext_wakeup_req :
                             disk_bridge_en ? (ext_wakeup_req & 16'h1000) : 16'd0),   // TASK_DSK = 014
        .ext_subtask_we     (io_bridge_en ? ext_subtask_we : 1'b0),
        .ext_subtask_task   (ext_subtask_task),
        .ext_subtask_val    (ext_subtask_val),

        .dbg_real_pc        (dbg_real_pc),
        .dbg_ctask          (dbg_ctask),
        .dbg_t              (dbg_t),
        .dbg_q              (dbg_q),
        .dbg_link           (dbg_link),
        .dbg_cnt            (dbg_cnt),
        .dbg_stkp           (dbg_stkp),
        .dbg_cycles         (dbg_cycles),
        .dbg_halted         (dbg_halted),
        .dbg_halt_reason    (dbg_halt_reason),
        .step_done          (step_done),
        .cmp_done           (cmp_done),
        .io_tick_early      (io_tick_early),
        .io_busy            ((io_bridge_en || disk_bridge_en) && (io_busy || srv_stall)),
        .io_busy_tick       ((io_bridge_en || disk_bridge_en) && (io_jobs_busy || srv_stall)),
        .ap_hold            (ap_hold),
        .ov_hold            (ov_hold),
        .ap_req             (ap_req),
        .ap_op              (ap_op),
        .ap_we              (ap_we),
        .ap_stk             (ap_stk),
        .ap_cache           (ap_cache),
        .ap_va              (ap_va),
        .ap_wd              (ap_wd),
        .ap_done            (ap_done),
        .ap_rd              (ap_rd),
        .cpu_between        (cpu_between),
        .cpu_quiet          (cpu_quiet),
        .dbg_ifu_active     (dbg_ifu_active),
        .dbg_st             (cpu_st),
        .dbg_stall          (cpu_stall),
        .sto_req            (sto_req),
        .sto_we             (sto_we),
        .sto_addr           (sto_addr),
        .sto_mask           (sto_mask),
        .sto_wline          (sto_wline),
        .sto_ack_ext        (sto_ack),
        .sto_rline_ext      (sto_rline)
    );

    dorado_rtl_assist u_assist (
        .clk               (clk),
        .rst               (core_rst),
        .en                (assist_on),
        .cpu_between       (cpu_between),
        .cycles            (dbg_cycles),
        .real_pc           (dbg_real_pc),
        .ctask             (dbg_ctask),
        .ifu_active        (dbg_ifu_active),
        .eftp_started      (srv_eftp_max_seq != 16'd0),
        .next_wcb_any      (io_next_wcb_any),
        .live_kbd          (kbd_words),
        .live_buttons      (mouse_buttons),
        .mouse_present     (mouse_present),
        .mouse_x           (mouse_x),
        .mouse_y           (mouse_y),
        .ap_hold           (as_hold),
        .ap_req            (as_req),
        .ap_we             (as_we),
        .ap_stk            (as_stk),
        .ap_cache          (as_cache),
        .ap_va             (as_va),
        .ap_wd             (as_wd),
        .ap_done           (ap_done),
        .ap_rd             (ap_rd),
        .kbd0_poke         (kbd0_poke),
        .kbd0_val          (kbd0_val),
        .display_wake_gate (assist_display_gate)
    );

    // Mouse motion arrives as one-clock reports; the I/O block takes input
    // once per microinstruction (its clk_en tick), so motion is summed here
    // until that tick consumes it.
    logic signed [15:0] ms_acc_dx, ms_acc_dy;
    logic               ms_acc_pend;
    // The I/O module ticks once per microinstruction: in the clock it
    // completes (io_tick_early), or, for one that completes otherwise, in
    // the clock after (step_done, as before).
    logic               io_tick_early, io_tick_early_q;
    always_ff @(posedge clk) io_tick_early_q <= core_rst ? 1'b0 : io_tick_early;
    wire                io_tick = clk_en && (io_bridge_en || disk_bridge_en) &&
                                  (io_tick_early || (step_done && !io_tick_early_q));
    always_ff @(posedge clk) begin
        if (core_rst) begin
            ms_acc_dx   <= '0;
            ms_acc_dy   <= '0;
            ms_acc_pend <= 1'b0;
        end else begin
            if (io_tick && ms_acc_pend) begin
                ms_acc_dx   <= mouse_delta_stb ? mouse_dx : 16'sd0;
                ms_acc_dy   <= mouse_delta_stb ? mouse_dy : 16'sd0;
                ms_acc_pend <= mouse_delta_stb;
            end else if (mouse_delta_stb) begin
                ms_acc_dx   <= ms_acc_dx + mouse_dx;
                ms_acc_dy   <= ms_acc_dy + mouse_dy;
                ms_acc_pend <= 1'b1;
            end
        end
    end

    // ---- the disk engine's pack memory ------------------------------------
    // Simulation (DORADO_DISK_DPI): the lock-step harness's copy of the pack.
    // MiSTer (EXT_STORAGE): DDR, the pack at byte 0x3100_0000 (dorado-hps puts
    // it there), shared with the mailbox on the storage engine's aux port.
    // Otherwise every beat reads zero.
`ifdef DORADO_DISK_DPI
    import "DPI-C" function longint dpi_disk_beat_read(input int addr);
    import "DPI-C" function void    dpi_disk_beat_write(input int addr, input longint data, input int be);
    always_ff @(posedge clk) begin
        disk_mem_ack <= 1'b0;
        if (disk_mem_req && !disk_mem_ack) begin
            if (disk_mem_we) dpi_disk_beat_write(int'(disk_mem_addr), disk_mem_wdata, int'(disk_mem_be));
            else             disk_mem_rdata <= dpi_disk_beat_read(int'(disk_mem_addr));
            disk_mem_ack <= 1'b1;
        end
    end
`else
    generate
        if (!EXT_STORAGE) begin : g_disk_zero
            always_ff @(posedge clk) begin
                disk_mem_ack   <= disk_mem_req && !disk_mem_ack;
                disk_mem_rdata <= 64'd0;
            end
        end
    endgenerate
`endif

    dorado_rtl_io #(.DISK_PACK_BEAT_BASE(DISK_PACK_BASE)) u_io (
        .clk                     (clk),
        .rst                     (core_rst),
        .clk_en                  (io_tick && !mb_scan_ph),
        .scan_ph                 (mb_scan_ph),
        .scan_en                 (mb_scan_en),
        .scan_in                 (mb_scan_in),
        .ld_we                   (mb_ld_we),
        .ld_d                    (mb_ld_d),
        .scan_out                (scan_out_io),
        .cycles                  (dbg_cycles),
        .disk_time               (disk_time_ext_en ? disk_time : dbg_cycles),

        .io_read_task            (cpu_io_read_task),
        .io_read_subtask         (cpu_io_read_subtask),
        .io_read_tioa            (cpu_io_read_tioa),
        .io_read_stb             (cpu_io_read_stb),
        .io_read_stb_task        (cpu_io_read_stb_task),
        .io_read_stb_subtask     (cpu_io_read_stb_subtask),
        .io_read_stb_tioa        (cpu_io_read_stb_tioa),
        .io_read_data            (bridge_io_read_data),
        .io_read_bad_parity      (bridge_io_read_bad_parity),
        .io_has_write            (bridge_io_has_write),
        .io_atten                (bridge_io_atten),

        .io_write_stb            (io_write_stb),
        .io_write_task           (io_write_task),
        .io_write_subtask        (io_write_subtask),
        .io_write_tioa           (io_write_tioa),
        .io_write_data           (io_write_data),

        .fastio_stb              (fastio_stb),
        .fastio_kind             (fastio_kind),
        .fastio_task             (fastio_task),
        .fastio_subtask          (fastio_subtask),
        .fastio_va               (fastio_va),
        .fastio_out_munch        (fastio_out_munch),
        .fastio_in_munch         (fastio_in_munch),

        .dmux_cur_addr           (dmux_cur_addr),
        .dmux_ext_handled        (dmux_ext_handled),
        .dmux_ext_val            (dmux_ext_val),

        .ext_wakeup_req          (ext_wakeup_req),
        .ext_subtask_we          (ext_subtask_we),
        .ext_subtask_task        (ext_subtask_task),
        .ext_subtask_val         (ext_subtask_val),

        .display_wake_gate       (assist_en ? assist_display_gate : display_wake_gate),
        .display_top_lines       (display_top_lines),
        .display_cursor_dx       (display_cursor_dx),
        .kbd_words_in            (kbd_words_default),
        .mouse_dx_in             (ms_acc_dx),
        .mouse_dy_in             (ms_acc_dy),
        .mouse_delta_stb         (ms_acc_pend),

        .eth_local_host          (eth_local_host),
        .eth_tx_packet_stb       (eth_tx_packet_stb),
        .eth_tx_packet_words     (eth_tx_packet_words),

        .disk_selected_drive_out (disk_selected_drive_out),
        .disk_cur_cyl_out        (disk_cur_cyl_out),
        .disk_cur_head_out       (disk_cur_head_out),
        .disk_cur_sector_out     (disk_cur_sector_out),
        .disk_sector_read_req    (disk_sector_read_req),
        .disk_sector_write_req   (disk_sector_write_req),
        .disk_mem_req            (disk_mem_req),
        .disk_mem_we             (disk_mem_we),
        .disk_mem_addr           (disk_mem_addr),
        .disk_mem_wdata          (disk_mem_wdata),
        .disk_mem_be             (disk_mem_be),
        .disk_mem_ack            (disk_mem_ack),
        .disk_mem_rdata          (disk_mem_rdata),
        .fb_rd_addr              (fb_rd_addr),
        .fb_rd_data              (fb_rd_data),
        .io_busy                 (io_busy),
        .io_jobs_busy            (io_jobs_busy),
        .rx_wr_en                (rx_wr_en),
        .rx_wr_addr              (rx_wr_addr),
        .rx_wr_data              (rx_wr_data),
        .rx_wr_att               (rx_wr_att),
        .rx_load                 (rx_load),
        .rx_load_count           (rx_load_count),
        .eth_tx_rd_addr          (HPS_MAILBOX ? mb_tx_rd_addr : srv_tx_rd_addr),
        .eth_tx_rd_data          (io_tx_rd_data),
        .eth_rx_on_out           (io_rx_on),
        .eth_rx_drained          (io_rx_drained),
        .eth_rx_loads            (io_rx_loads),
        .kbd0_poke               (kbd0_poke),
        .kbd0_val                (kbd0_val),
        .next_wcb_any            (io_next_wcb_any)
    );

    dorado_rtl_bootsrv #(
        .BOOT_WORDS (BOOT_WORDS),
        .BOOT_HEX   (BOOT_HEX),
        .BOL_HEX    (BOL_HEX)
    ) u_bootsrv (
        .clk            (clk),
        .rst            (core_rst),
        .en             (bootsrv_en && io_bridge_en),
        .cycles         (dbg_cycles),
        .local_host     (eth_local_host),
        .tx_packet_stb  (eth_tx_packet_stb),
        .tx_rd_addr     (srv_tx_rd_addr),
        .tx_rd_data     (io_tx_rd_data),
        .rx_on          (io_rx_on),
        .rx_drained     (io_rx_drained),
        .io_busy        (io_busy),
        .rx_wr_en       (srv_rx_wr_en),
        .rx_wr_addr     (srv_rx_wr_addr),
        .rx_wr_data     (srv_rx_wr_data),
        .rx_wr_att      (srv_rx_wr_att),
        .rx_load        (srv_rx_load),
        .rx_load_count  (srv_rx_load_count),
        .stall          (srv_stall),
        .eftp_max_seq   (srv_eftp_max_seq),
        .bol_sent       (srv_bol_sent)
    );

endmodule
