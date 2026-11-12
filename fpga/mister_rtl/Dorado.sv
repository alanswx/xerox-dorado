//============================================================================
//  Xerox Dorado (rtl_core) for MiSTer
//
//  The clean RTL core (rtl_core/, one microinstruction per ~10 fabric
//  clocks) inside the MiSTer framework. It powers up with the Alto emulator
//  microcode (AEmu) preloaded, boots an Alto boot file over its own Ethernet
//  controller from the in-FPGA boot server (Breath of Life, Mayday, EFTP),
//  and shows the display controller's 1024x808 framebuffer.
//
//  Built with DORADO_CEDAR (build.sh CORE=cedar) it is instead the Cedar
//  checkpoint core: it powers up in the Cedar desktop checkpoint baked into
//  the image (rtl_core/init_cedar), with 4M words of storage in DDR3, and
//  waits for dorado-hps on the ARM (rtl_core/hps) to load storage and say
//  go; the host services and the file server then reach it through the
//  mailbox in DDR3 (dorado_rtl_mailbox).
//============================================================================

module emu
(
	input         CLK_50M,
	input         RESET,
	inout  [48:0] HPS_BUS,
	output        CLK_VIDEO,
	output        CE_PIXEL,
	output [12:0] VIDEO_ARX,
	output [12:0] VIDEO_ARY,
	output  [7:0] VGA_R,
	output  [7:0] VGA_G,
	output  [7:0] VGA_B,
	output        VGA_HS,
	output        VGA_VS,
	output        VGA_DE,
	output        VGA_F1,
	output [1:0]  VGA_SL,
	output        VGA_SCALER,
	output        VGA_DISABLE,
	input  [11:0] HDMI_WIDTH,
	input  [11:0] HDMI_HEIGHT,
	output        HDMI_FREEZE,
	output        HDMI_BLACKOUT,
	output        HDMI_BOB_DEINT,
`ifdef MISTER_FB
	output        FB_EN,
	output  [4:0] FB_FORMAT,
	output [11:0] FB_WIDTH,
	output [11:0] FB_HEIGHT,
	output [31:0] FB_BASE,
	output [13:0] FB_STRIDE,
	input         FB_VBL,
	input         FB_LL,
	output        FB_FORCE_BLANK,
`ifdef MISTER_FB_PALETTE
	output        FB_PAL_CLK,
	output  [7:0] FB_PAL_ADDR,
	output [23:0] FB_PAL_DOUT,
	input  [23:0] FB_PAL_DIN,
	output        FB_PAL_WR,
`endif
`endif
	output        LED_USER,
	output  [1:0] LED_POWER,
	output  [1:0] LED_DISK,
	output  [1:0] BUTTONS,
	input         CLK_AUDIO,
	output [15:0] AUDIO_L,
	output [15:0] AUDIO_R,
	output        AUDIO_S,
	output  [1:0] AUDIO_MIX,
	inout   [3:0] ADC_BUS,
	output        SD_SCK,
	output        SD_MOSI,
	input         SD_MISO,
	output        SD_CS,
	input         SD_CD,
	output        DDRAM_CLK,
	input         DDRAM_BUSY,
	output  [7:0] DDRAM_BURSTCNT,
	output [28:0] DDRAM_ADDR,
	input  [63:0] DDRAM_DOUT,
	input         DDRAM_DOUT_READY,
	output        DDRAM_RD,
	output [63:0] DDRAM_DIN,
	output  [7:0] DDRAM_BE,
	output        DDRAM_WE,
	output        SDRAM_CLK,
	output        SDRAM_CKE,
	output [12:0] SDRAM_A,
	output  [1:0] SDRAM_BA,
	inout  [15:0] SDRAM_DQ,
	output        SDRAM_DQML,
	output        SDRAM_DQMH,
	output        SDRAM_nCS,
	output        SDRAM_nCAS,
	output        SDRAM_nRAS,
	output        SDRAM_nWE,
`ifdef MISTER_DUAL_SDRAM
	input         SDRAM2_EN,
	output        SDRAM2_CLK,
	output [12:0] SDRAM2_A,
	output  [1:0] SDRAM2_BA,
	inout  [15:0] SDRAM2_DQ,
	output        SDRAM2_nCS,
	output        SDRAM2_nCAS,
	output        SDRAM2_nRAS,
	output        SDRAM2_nWE,
`endif
	input         UART_CTS,
	output        UART_RTS,
	input         UART_RXD,
	output        UART_TXD,
	output        UART_DTR,
	input         UART_DSR,
	input   [6:0] USER_IN,
	output  [6:0] USER_OUT,
	input         OSD_STATUS
);

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_DTR} = 0;
// Non-DDR debug sideband: stream dbg_vec continuously out UART_TXD (8N1,
// 115200 at 44 MHz), read on the ARM at /dev/ttyS1. This survives a DDR
// master wedge -- which freezes every status beat (all core->HPS state
// rides the one DDRAM master) -- because it reads registers directly. Each
// frame is AA 55, then dbg_vec MSB-first (16 bytes: ddr_dbg[63:0] = the
// storage_ddr state, then cpu_st/real_PC/ctask/cycles[31:0]), then 0A.
dorado_uart_dbg #(.DIV(382)) u_uart_dbg (.clk(clk_sys), .vec(dbg_vec), .tx(UART_TXD));
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML, SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;
assign DDRAM_CLK = clk_sys;   // storage (dorado_rtl_storage_ddr) runs on the core clock

assign VGA_SL = 0;
assign VGA_F1 = 0;
assign VGA_SCALER  = 0;
assign VGA_DISABLE = 0;
assign HDMI_FREEZE = 0;
assign HDMI_BLACKOUT = 0;
assign HDMI_BOB_DEINT = 0;

assign AUDIO_S = 0;
assign AUDIO_L = 0;
assign AUDIO_R = 0;
assign AUDIO_MIX = 0;

assign LED_DISK = 0;
assign LED_POWER = 0;
assign BUTTONS = 0;

// The framebuffer is 1024 x 808 square pixels. Each world is presented at
// its own raster, as the C emulator's frontends do: Smalltalk-76's bitmap is
// 640 x 800 (at the top left), so only that much is displayed and the scaler
// centres it, rather than showing the undrawn rest of the frame as white.
`ifdef DORADO_SMALLTALK
localparam int DISP_W = 640, DISP_H = 800;
`else
localparam int DISP_W = 1024, DISP_H = 808;
`endif
assign VIDEO_ARX = 13'(DISP_W);
assign VIDEO_ARY = 13'(DISP_H);

`include "build_id.v"
localparam CONF_STR = {
	"Dorado;;",
	"-;",
	"T[0],Reset;",
	"O[1],Debug overlay,Off,On;",
	"R[0],Reset and close OSD;",
	"V,v",`BUILD_DATE
};

wire   [1:0] buttons;
wire [127:0] status;
wire  [10:0] ps2_key;
wire  [24:0] ps2_mouse;

hps_io #(.CONF_STR(CONF_STR)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
	.EXT_BUS(),
	.gamma_bus(),
	.buttons(buttons),
	.status(status),
	.status_menumask(16'd0),
	.ps2_key(ps2_key),
	.ps2_mouse(ps2_mouse)
);

///////////////////////   CLOCKS   ///////////////////////////////

wire clk_sys;   // CLK_MHZ (build.sh), 50 MHz by default
wire pll_locked;
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys),
	.locked(pll_locked)
);

// The fabric clock measured against the board's 50 MHz: a count of CLK_50M
// edges crosses into clk_sys as a Gray code (a torn sample is off by one),
// and the host divides the two counts' rates. The PLL's own lock beside it.
reg  [31:0] ref_bin = 32'd0;
always @(posedge CLK_50M) ref_bin <= ref_bin + 32'd1;
wire [31:0] ref_gray = ref_bin ^ (ref_bin >> 1);
reg  [31:0] ref_g1, ref_g2;
always @(posedge clk_sys) begin
	ref_g1 <= ref_gray;
	ref_g2 <= ref_g1;
end
wire [31:0] ref_cnt;
genvar gi;
generate for (gi = 0; gi < 32; gi = gi + 1) begin : g2b
	assign ref_cnt[gi] = ^(ref_g2 >> gi);
end endgenerate

// The preloaded memories (IM, IFUM, RM, Map) take their contents only when
// the FPGA is configured; a reset restarts from whatever they hold now, so
// reload the core for a clean boot.
reg [7:0] reset_cnt = 8'hFF;
always @(posedge clk_sys) begin
	if (RESET | status[0] | buttons[1]) reset_cnt <= 8'hFF;
	else if (reset_cnt != 0)            reset_cnt <= reset_cnt - 1'd1;
end
wire reset = (reset_cnt != 0);

///////////////////////   DORADO   ///////////////////////////////

// PS/2 keyboard -> the four Alto keyboard words 177034B-177037B (active low).
// dorado_keyboard holds the 61-key matrix; key-matrix-check compares it
// against dorado/src/display.c.
wire [15:0] key_word0, key_word1, key_word2, key_word3;
dorado_keyboard keyboard
(
	.sys_clk  (clk_sys),
	.reset    (reset),
	.ps2_key  (ps2_key),
	.key_word0(key_word0),
	.key_word1(key_word1),
	.key_word2(key_word2),
	.key_word3(key_word3)
);

// PS/2 mouse -> the Alto mouse: an absolute MOUSELOC position (VA 424/425)
// kept inside the Alto's 606x808 display, and the buttons for UTILIN
// (bit 2 Red = left, bit 1 Blue = right, bit 0 Yellow = middle; machine.c).
// PS/2 packet: byte 0 status (0 L, 1 R, 2 M, 4 X sign, 5 Y sign), 1 X, 2 Y;
// PS/2 Y counts up, the Alto's down.
// CKPT: a checkpoint core (Cedar or Smalltalk): it powers up in a world's
// checkpoint baked into init/, takes storage from dorado-hps over the DDR
// mailbox and waits for its go. Cedar takes mouse motion through the terminal
// (its display microcode); Smalltalk-76, an Alto-family world, takes the
// absolute position as the C emulator gives it (dorado-hps -w smalltalk).
`ifdef DORADO_CEDAR
localparam bit        TRIDENT = 1'b0;
localparam bit        CEDAR = 1'b1;
localparam bit        CKPT  = 1'b1;
localparam int        MOUSE_W = 1024;      // Cedar's screen
`elsif DORADO_SMALLTALK
localparam bit        TRIDENT = 1'b1;
localparam bit        CEDAR = 1'b0;
localparam bit        CKPT  = 1'b1;
localparam int        MOUSE_W = 640;       // Smalltalk-76's bitmap
`elsif DORADO_LISP
// Interlisp-D Lyric: the same kind of world (the Alto emulator on a Trident
// pack, the absolute mouse through dorado-hps -w lisp), on the full screen
localparam bit        TRIDENT = 1'b1;
localparam bit        CEDAR = 1'b0;
localparam bit        CKPT  = 1'b1;
localparam int        MOUSE_W = 1024;
`else
localparam bit        TRIDENT = 1'b0;
localparam bit        CEDAR = 1'b0;
localparam bit        CKPT  = 1'b0;
localparam int        MOUSE_W = 606;       // the Alto's
`endif
reg         mouse_present = 1'b0;
reg   [2:0] mouse_buttons = 3'd0;
// A one-button mouse: Option (Alt) + click gives Yellow (middle) and Command
// (GUI) + click gives Blue (right) -- the convention of the X11 servers on the
// Mac. Neither key is in the Dorado keyboard matrix (dorado_keyboard), so the
// guest never sees them. The modifier is taken when the button goes down and
// held to its release, so letting go of the key mid-drag does not turn a Blue
// press into a Red release.
reg         kb_alt = 1'b0, kb_gui = 1'b0, kb_stb_q = 1'b0;
reg   [1:0] click_as = 2'd0;               // 0 Red, 1 Yellow, 2 Blue
always @(posedge clk_sys) begin
	kb_stb_q <= ps2_key[10];
	if (ps2_key[10] != kb_stb_q) begin
		if (ps2_key[7:0] == 8'h11)                          kb_alt <= ps2_key[9];   // L/R Alt
		if (ps2_key[8] && (ps2_key[7:0] == 8'h1F || ps2_key[7:0] == 8'h27))
			                                                kb_gui <= ps2_key[9];   // L/R GUI
	end
end
reg  [15:0] mouse_x = 16'(MOUSE_W / 2), mouse_y = 16'd404;
reg         mouse_stb_q = 1'b0;
reg  signed [15:0] mouse_dx = 16'sd0, mouse_dy = 16'sd0;   // the last report's motion
reg         mouse_delta_stb = 1'b0;
wire signed [9:0] ms_dx = {ps2_mouse[4], ps2_mouse[4], ps2_mouse[15:8]};
wire signed [9:0] ms_dy = {ps2_mouse[5], ps2_mouse[5], ps2_mouse[23:16]};
wire signed [17:0] ms_nx = $signed({2'b00, mouse_x}) + ms_dx;
wire signed [17:0] ms_ny = $signed({2'b00, mouse_y}) - ms_dy;
always @(posedge clk_sys) begin
	mouse_stb_q <= ps2_mouse[24];
	mouse_delta_stb <= 1'b0;
	if (ps2_mouse[24] != mouse_stb_q) begin
		// Cedar takes motion through the terminal (its display microcode);
		// the Alto world takes the absolute position below
		mouse_dx        <= {{6{ms_dx[9]}}, ms_dx};
		mouse_dy        <= -{{6{ms_dy[9]}}, ms_dy};
		mouse_delta_stb <= CEDAR;
		mouse_present <= 1'b1;
		// {Red, Blue, Yellow} = {left, right, middle}; a left press with a
		// modifier stands in for the missing button
		if (ps2_mouse[0] && !mouse_buttons[2] && !mouse_buttons[1] && !mouse_buttons[0])
			click_as <= kb_gui ? 2'd2 : kb_alt ? 2'd1 : 2'd0;
		begin : remap
			logic [1:0] as;
			as = (ps2_mouse[0] && !(mouse_buttons[2] | mouse_buttons[1] | mouse_buttons[0]))
			     ? (kb_gui ? 2'd2 : kb_alt ? 2'd1 : 2'd0) : click_as;
			mouse_buttons <= {ps2_mouse[0] && as == 2'd0,
			                  ps2_mouse[1] || (ps2_mouse[0] && as == 2'd2),
			                  ps2_mouse[2] || (ps2_mouse[0] && as == 2'd1)};
		end
		mouse_x <= (ms_nx < 0) ? 16'd0 : (ms_nx > MOUSE_W - 1) ? 16'(MOUSE_W - 1) : ms_nx[15:0];
		mouse_y <= (ms_ny < 0) ? 16'd0 : (ms_ny > DISP_H - 1) ? 16'(DISP_H - 1) : ms_ny[15:0];
	end
end

wire [127:0] dbg_vec;
wire [19:0] fb_rd_addr;
wire        fb_rd_data;
wire [63:0] cycles;
wire        halted;

dorado_rtl_top #(
	.STORAGE_WORDS (CKPT ? 4 * 1024 * 1024 : 65536),
	.RESET_PC      (`DORADO_RESET_PC),
	.BOOT_WORDS    (`DORADO_BOOT_WORDS),
	.BOOT_HEX      ("roms/bootfile.hex"),
	.BOL_HEX       ("roms/bol.hex"),
	.EXT_STORAGE   (1'b1),
	.HPS_MAILBOX   (CKPT),
`ifdef DORADO_PIPELINE
	.PIPELINE      (1'b1)
`else
	.PIPELINE      (1'b0)
`endif
) dorado
(
	.clk                (clk_sys),
	.rst                (reset),
	.clk_en             (1'b1),
	.pll_locked         (pll_locked),
	.ref_cnt            (ref_cnt),
	.bb_present         (1'b0),
	.bb_running         (1'b1),
	.bb_ss_inject       (1'b0),
	.bb_mir_iw0         (16'd0),
	.bb_mir_iw1         (16'd0),
	.bb_mir_iw2         (16'd0),
	.io_bridge_en       (1'b1),
	.disk_bridge_en     (1'b0),
	.disk_time_ext_en   (1'b0),
	.disk_time          (64'd0),
	.bootsrv_en         (!CKPT),
	.assist_en          (!CKPT),
	.display_wake_gate  (CKPT),         // the world's own display microcode drives the DDC
	.display_top_lines  (CKPT ? 6'd22 : 6'd0),
	.kbd_words          ({key_word3, key_word2, key_word1, key_word0}),
	.mouse_buttons      (mouse_buttons),
	.mouse_present      (mouse_present),
	.mouse_x            (mouse_x),
	.mouse_y            (mouse_y),
	.mouse_dx           (mouse_dx),
	.mouse_dy           (mouse_dy),
	.mouse_delta_stb    (mouse_delta_stb),
	.hx_rx_wr_en        (1'b0),
	.hx_rx_wr_addr      (12'd0),
	.hx_rx_wr_data      (16'd0),
	.hx_rx_wr_att       (1'b0),
	.hx_rx_load         (1'b0),
	.hx_rx_load_count   (13'd0),
	.hx_rx_on           (),
	.hx_rx_drained      (),
	.hp_hold            (1'b0),
	.ov_hold            (TRIDENT),      // C_PIPE overlap off for the Trident worlds (clean-rtl-handoff 9.5, OPEN)
	.hp_req             (1'b0),
	.hp_op              (3'd0),
	.hp_we              (1'b0),
	.hp_cache           (1'b0),
	.hp_va              (28'd0),
	.hp_wd              (16'd0),
	.hp_done            (),
	.hp_rd              (),
	.hp_between         (),
	.io_attached        (1'b0),
	.io_read_data       (16'd0),
	.io_read_bad_parity (1'b0),
	.io_has_write_in    (1'b0),
	.io_atten_in        (1'b0),
	.io_write_stb       (),
	.io_write_task      (),
	.io_write_subtask   (),
	.io_write_tioa      (),
	.io_write_data      (),
	.eth_local_host     (8'o42),
	.eth_tx_packet_stb  (),
	.eth_tx_packet_words(),
	.disk_selected_drive_out(),
	.disk_cur_cyl_out   (),
	.disk_cur_head_out  (),
	.disk_cur_sector_out(),
	.disk_sector_read_req(),
	.disk_sector_write_req(),
	.DDRAM_BUSY         (DDRAM_BUSY),
	.DDRAM_BURSTCNT     (DDRAM_BURSTCNT),
	.DDRAM_ADDR         (DDRAM_ADDR),
	.DDRAM_DOUT         (DDRAM_DOUT),
	.DDRAM_DOUT_READY   (DDRAM_DOUT_READY),
	.DDRAM_RD           (DDRAM_RD),
	.DDRAM_DIN          (DDRAM_DIN),
	.DDRAM_BE           (DDRAM_BE),
	.DDRAM_WE           (DDRAM_WE),
	.fb_rd_addr         (fb_rd_addr),
	.fb_rd_data         (fb_rd_data),
	.dbg_real_pc        (),
	.dbg_ctask          (),
	.dbg_t              (),
	.dbg_q              (),
	.dbg_link           (),
	.dbg_cnt            (),
	.dbg_stkp           (),
	.dbg_cycles         (cycles),
	.dbg_halted         (halted),
	.dbg_halt_reason    (),
	.step_done          (),
	.cmp_done           (),
	.dbg_vec            (dbg_vec)
);

///////////////////////   VIDEO   ////////////////////////////////
// 1024 x 808 visible, one pixel per clk_sys: 1344 x 838 total, about 44
// frames/s at 50 MHz. The MiSTer scaler converts it for HDMI.

localparam H_VIS = 1024, H_FP = 24, H_SYNC = 136, H_TOT = 1344;
localparam V_VIS = 808,  V_FP = 3,  V_SYNC = 6,   V_TOT = 838;

reg [10:0] hc = 0;
reg  [9:0] vc = 0;
always @(posedge clk_sys) begin
	if (hc == H_TOT - 1) begin
		hc <= 0;
		vc <= (vc == V_TOT - 1) ? 10'd0 : vc + 1'd1;
	end else begin
		hc <= hc + 1'd1;
	end
end

// Stage 0: address pixel (hc, vc); stage 1: data; stage 2: out.
assign fb_rd_addr = {vc, hc[9:0]};
reg        de1, hs1, vs1, de2, hs2, vs2;
reg  [7:0] pix2;

// Debug overlay (OSD "Debug overlay", status[1]): dbg_vec as four rows of
// 32 bit-blocks, 8x8 pixels, MSB at the left, at x 640-895, y 8-39. A set bit is a solid block; every block has
// a black left edge so the positions can be counted from a capture.
// Rows: [127:96] DDR engine state/flags and read commands, [95:64] read and
// write beat counts, [63:32] CPU state, PC, task, [31:0] cycle count.
wire       ov_area = (hc >= 640) && (hc < 896) && (vc >= 8) && (vc < 40);
wire [1:0] ov_row  = 2'((vc - 10'd8) >> 3);
wire [4:0] ov_bit  = 5'd31 - 5'((hc - 11'd640) >> 3);
wire       ov_on   = dbg_vec[{2'd3 - ov_row, ov_bit}];
reg        ov1, ov2, ovb1, ovb2;
always @(posedge clk_sys) begin
	ov1  <= ov_area && status[1];
	ovb1 <= ov_on || (hc[2:0] == 3'd0);
	ov2  <= ov1;
	ovb2 <= ovb1;
end
always @(posedge clk_sys) begin
	de1  <= (hc < DISP_W) && (vc < DISP_H);   // the world's raster
	hs1  <= (hc >= H_VIS + H_FP) && (hc < H_VIS + H_FP + H_SYNC);
	vs1  <= (vc >= V_VIS + V_FP) && (vc < V_VIS + V_FP + V_SYNC);
	de2  <= de1;
	hs2  <= hs1;
	vs2  <= vs1;
	// a set bit is black, as the C emulator's display shows it
	pix2 <= fb_rd_data ? 8'h00 : 8'hFF;
end
wire [7:0] pix_out = ov2 ? (ovb2 ? 8'h00 : 8'hFF) : pix2;

assign CLK_VIDEO = clk_sys;
assign CE_PIXEL  = 1'b1;
assign VGA_DE    = de2;
assign VGA_HS    = hs2;
assign VGA_VS    = vs2;
assign VGA_R     = de2 ? pix_out : 8'd0;
assign VGA_G     = de2 ? pix_out : 8'd0;
assign VGA_B     = de2 ? pix_out : 8'd0;

// LED: lit while the machine runs (blinks on the microinstruction count).
assign LED_USER = !halted && cycles[22];

endmodule

// Continuous 8N1 UART debug streamer (verified standalone). Transmits a
// 19-byte frame: 0xAA, 0x55, `vec` MSB-first (16 bytes), 0x0A, then repeats;
// `vec` is latched at frame start so each frame is coherent.
module dorado_uart_dbg #(parameter int unsigned DIV = 382) (
    input  wire         clk,
    input  wire [127:0] vec,
    output reg          tx
);
    localparam int NB = 19;
    reg [127:0] snap = 128'd0;
    reg [4:0]   bidx = 5'd0;
    reg [9:0]   sh   = 10'h3FF;     // {stop, data[7:0], start}, indexed by bitn
    reg [3:0]   bitn = 4'd0;
    reg [15:0]  baud = 16'd0;
    reg         busy = 1'b0;
    initial tx = 1'b1;

    function [7:0] bsel(input [4:0] i, input [127:0] v);
        case (i)
          5'd0:  bsel = 8'hAA;
          5'd1:  bsel = 8'h55;
          5'd18: bsel = 8'h0A;
          default: bsel = v[8*(15 - (i - 5'd2)) +: 8]; // i=2->v[127:120], i=17->v[7:0]
        endcase
    endfunction

    always @(posedge clk) begin
        if (!busy) begin
            if (bidx == 5'd0) snap <= vec;
            sh   <= {1'b1, bsel(bidx, (bidx == 5'd0) ? vec : snap), 1'b0};
            tx   <= 1'b0;                  // start bit (sh[0])
            bitn <= 4'd0;
            baud <= DIV - 1;
            busy <= 1'b1;
        end else if (baud != 16'd0) begin
            baud <= baud - 16'd1;
        end else begin
            baud <= DIV - 1;
            if (bitn == 4'd9) begin
                busy <= 1'b0;
                tx   <= 1'b1;             // idle
                bidx <= (bidx == NB - 1) ? 5'd0 : bidx + 5'd1;
            end else begin
                bitn <= bitn + 4'd1;
                tx   <= sh[bitn + 1];
            end
        end
    end
endmodule
