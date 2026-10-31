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
    parameter int STORAGE_WORDS = 4 * 1024 * 1024
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
    input  logic        display_wake_gate,

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
    input  logic [16:0] fb_rd_addr,
    output logic [7:0]  fb_rd_data,

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
    output logic        step_done      // pulses one clock after each microinstruction
);
    logic           io_busy;
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
        for (int i = 0; i < 5; i++) kbd_words_default[i] = 16'hFFFF;
        for (int i = 0; i < 16; i++) begin
            cpu_fastio_in_munch[i] = io_bridge_en ? fastio_in_munch[i] : 16'd0;
        end
    end

    dorado_rtl_cpu #(
        .STORAGE_WORDS (STORAGE_WORDS)
    ) u_cpu (
        .clk                (clk),
        .rst                (rst),
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
        .io_busy            (io_bridge_en && io_busy)
    );

    dorado_rtl_io u_io (
        .clk                     (clk),
        .rst                     (rst),
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

        .display_wake_gate       (display_wake_gate),
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
        .io_busy                 (io_busy)
    );

endmodule
