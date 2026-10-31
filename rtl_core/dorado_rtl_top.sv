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
    parameter bit EXT_STORAGE   = 1'b0           // 1: storage on the sto_* port (MiSTer: DDR3)
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        clk_en,

    // BaseBoard / MIR single-step injection interface
    input  logic        bb_present,
    input  logic        bb_running,
    input  logic        bb_ss_inject,
    input  logic [15:0] bb_mir_iw0,
    input  logic [15:0] bb_mir_iw1,
    input  logic [15:0] bb_mir_iw2,

    // Mode select: 1 = use internal dorado_rtl_io bridge; 0 = external/disconnected
    input  logic        io_bridge_en,
    // 1 = the in-FPGA boot server (dorado_rtl_bootsrv) answers the Ethernet
    input  logic        bootsrv_en,
    // 1 = dorado_rtl_assist performs the loader assists (cold-Alto init,
    //     keyboard words, display gate) instead of the testbench
    input  logic        assist_en,
    input  logic        display_wake_gate,
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
    output logic        dbg_halted,
    output logic [3:0]  dbg_halt_reason,
    output logic        step_done,     // pulses one clock after each microinstruction
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
    logic           srv_stall;
    logic           srv_rx_wr_en, srv_rx_wr_att, srv_rx_load;
    logic [11:0]    srv_rx_wr_addr;
    logic [15:0]    srv_rx_wr_data;
    logic [12:0]    srv_rx_load_count;
    logic [9:0]     srv_tx_rd_addr;
    logic [15:0]    io_tx_rd_data;
    logic           io_rx_on, io_rx_drained;
    logic [15:0]    srv_eftp_max_seq, srv_bol_sent;
    logic           ap_hold, ap_req, ap_we, ap_stk, ap_cache, ap_done;
    logic [27:0]    ap_va;
    logic [15:0]    ap_wd, ap_rd;
    logic           cpu_between, dbg_ifu_active;
    logic           kbd0_poke;
    logic [15:0]    kbd0_val;
    logic           assist_display_gate, io_next_wcb_any;
    dorado_uinstr_t bb_mir_uinstr;
    assign bb_mir_uinstr = unpack_uinstr(bb_mir_iw0, bb_mir_iw1, bb_mir_iw2);

    // Interconnect between CPU/Mem and I/O Bridge
    logic [3:0]     cpu_io_read_task;
    logic [1:0]     cpu_io_read_subtask;
    logic [7:0]     cpu_io_read_tioa;
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
        kbd_words_default[4] = 16'hFFFF;
        for (int i = 0; i < 16; i++) begin
            cpu_fastio_in_munch[i] = io_bridge_en ? fastio_in_munch[i] : 16'd0;
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
    assign dbg_vec = {ddr_dbg, 3'd0, cpu_st, 4'd0, dbg_real_pc, 4'd0, dbg_ctask, dbg_cycles[31:0]};

    generate
        if (EXT_STORAGE) begin : g_ddr
            dorado_rtl_storage_ddr #(.STORAGE_WORDS(STORAGE_WORDS)) u_ddr (
                .clk              (clk),
                .rst              (rst),
                .ready            (sto_ready),
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
    assign core_rst = rst || !sto_ready;

    dorado_rtl_cpu #(
        .STORAGE_WORDS (STORAGE_WORDS),
        .RESET_PC      (RESET_PC),
        .EXT_STORAGE   (EXT_STORAGE)
    ) u_cpu (
        .clk                (clk),
        .rst                (core_rst),
        .clk_en             (clk_en),
        .bb_present         (bb_present),
        .bb_running         (bb_running),
        .bb_ss_inject       (bb_ss_inject),
        .bb_mir_uinstr      (bb_mir_uinstr),

        .io_attached        (io_bridge_en ? 1'b1 : io_attached),
        .io_read_data       (io_bridge_en ? bridge_io_read_data : io_read_data),
        .io_read_bad_parity (io_bridge_en ? bridge_io_read_bad_parity : io_read_bad_parity),
        .io_has_write_in    (io_bridge_en ? bridge_io_has_write : io_has_write_in),
        .io_atten_in        (io_bridge_en ? bridge_io_atten : io_atten_in),
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

        .dmux_ext_handled   (io_bridge_en ? dmux_ext_handled : 1'b0),
        .dmux_ext_val       (io_bridge_en ? dmux_ext_val : 16'd0),
        .dmux_cur_addr      (dmux_cur_addr),

        .ext_wakeup_req     (io_bridge_en ? ext_wakeup_req : 16'd0),
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
        .io_busy            (io_bridge_en && (io_busy || srv_stall)),
        .ap_hold            (ap_hold),
        .ap_req             (ap_req),
        .ap_we              (ap_we),
        .ap_stk             (ap_stk),
        .ap_cache           (ap_cache),
        .ap_va              (ap_va),
        .ap_wd              (ap_wd),
        .ap_done            (ap_done),
        .ap_rd              (ap_rd),
        .cpu_between        (cpu_between),
        .dbg_ifu_active     (dbg_ifu_active),
        .dbg_st             (cpu_st),
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
        .en                (assist_en && io_bridge_en),
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
        .ap_hold           (ap_hold),
        .ap_req            (ap_req),
        .ap_we             (ap_we),
        .ap_stk            (ap_stk),
        .ap_cache          (ap_cache),
        .ap_va             (ap_va),
        .ap_wd             (ap_wd),
        .ap_done           (ap_done),
        .ap_rd             (ap_rd),
        .kbd0_poke         (kbd0_poke),
        .kbd0_val          (kbd0_val),
        .display_wake_gate (assist_display_gate)
    );

    dorado_rtl_io u_io (
        .clk                     (clk),
        .rst                     (core_rst),
        .clk_en                  (clk_en && io_bridge_en && step_done),
        .cycles                  (dbg_cycles),

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
        .kbd_words_in            (kbd_words_default),
        .mouse_dx_in             (16'sd0),
        .mouse_dy_in             (16'sd0),
        .mouse_delta_stb         (1'b0),

        .eth_local_host          (eth_local_host),
        .eth_tx_packet_stb       (eth_tx_packet_stb),
        .eth_tx_packet_words     (eth_tx_packet_words),

        .disk_selected_drive_out (disk_selected_drive_out),
        .disk_cur_cyl_out        (disk_cur_cyl_out),
        .disk_cur_head_out       (disk_cur_head_out),
        .disk_cur_sector_out     (disk_cur_sector_out),
        .disk_sector_read_req    (disk_sector_read_req),
        .disk_sector_write_req   (disk_sector_write_req),
        .fb_rd_addr              (fb_rd_addr),
        .fb_rd_data              (fb_rd_data),
        .io_busy                 (io_busy),
        .rx_wr_en                (srv_rx_wr_en),
        .rx_wr_addr              (srv_rx_wr_addr),
        .rx_wr_data              (srv_rx_wr_data),
        .rx_wr_att               (srv_rx_wr_att),
        .rx_load                 (srv_rx_load),
        .rx_load_count           (srv_rx_load_count),
        .eth_tx_rd_addr          (srv_tx_rd_addr),
        .eth_tx_rd_data          (io_tx_rd_data),
        .eth_rx_on_out           (io_rx_on),
        .eth_rx_drained          (io_rx_drained),
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
