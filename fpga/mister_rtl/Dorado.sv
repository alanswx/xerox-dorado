//============================================================================
//  Xerox Dorado (rtl_core) for MiSTer
//
//  The clean RTL core (rtl_core/, one microinstruction per ~10 fabric
//  clocks) inside the MiSTer framework. It powers up with the Alto emulator
//  microcode (AEmu) preloaded, boots an Alto boot file over its own Ethernet
//  controller from the in-FPGA boot server (Breath of Life, Mayday, EFTP),
//  and shows the display controller's 1024x808 framebuffer.
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
assign {UART_RTS, UART_TXD, UART_DTR} = 0;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML, SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;
assign {DDRAM_CLK, DDRAM_BURSTCNT, DDRAM_ADDR, DDRAM_DIN, DDRAM_BE, DDRAM_RD, DDRAM_WE} = '0;

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

// The framebuffer is 1024 x 808 square pixels.
assign VIDEO_ARX = 13'd1024;
assign VIDEO_ARY = 13'd808;

`include "build_id.v"
localparam CONF_STR = {
	"Dorado;;",
	"-;",
	"T[0],Reset;",
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

wire clk_sys;   // CLK_MHZ (build.sh), 40 MHz by default
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys)
);

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
reg         mouse_present = 1'b0;
reg   [2:0] mouse_buttons = 3'd0;
reg  [15:0] mouse_x = 16'd303, mouse_y = 16'd404;
reg         mouse_stb_q = 1'b0;
wire signed [9:0] ms_dx = {ps2_mouse[4], ps2_mouse[4], ps2_mouse[15:8]};
wire signed [9:0] ms_dy = {ps2_mouse[5], ps2_mouse[5], ps2_mouse[23:16]};
wire signed [17:0] ms_nx = $signed({2'b00, mouse_x}) + ms_dx;
wire signed [17:0] ms_ny = $signed({2'b00, mouse_y}) - ms_dy;
always @(posedge clk_sys) begin
	mouse_stb_q <= ps2_mouse[24];
	if (ps2_mouse[24] != mouse_stb_q) begin
		mouse_present <= 1'b1;
		mouse_buttons <= {ps2_mouse[0], ps2_mouse[1], ps2_mouse[2]};
		mouse_x <= (ms_nx < 0) ? 16'd0 : (ms_nx > 605) ? 16'd605 : ms_nx[15:0];
		mouse_y <= (ms_ny < 0) ? 16'd0 : (ms_ny > 807) ? 16'd807 : ms_ny[15:0];
	end
end

wire [19:0] fb_rd_addr;
wire        fb_rd_data;
wire [63:0] cycles;
wire        halted;

dorado_rtl_top #(
	.STORAGE_WORDS (65536),
	.RESET_PC      (`DORADO_RESET_PC),
	.BOOT_WORDS    (`DORADO_BOOT_WORDS),
	.BOOT_HEX      ("roms/bootfile.hex"),
	.BOL_HEX       ("roms/bol.hex")
) dorado
(
	.clk                (clk_sys),
	.rst                (reset),
	.clk_en             (1'b1),
	.bb_present         (1'b0),
	.bb_running         (1'b1),
	.bb_ss_inject       (1'b0),
	.bb_mir_iw0         (16'd0),
	.bb_mir_iw1         (16'd0),
	.bb_mir_iw2         (16'd0),
	.io_bridge_en       (1'b1),
	.bootsrv_en         (1'b1),
	.assist_en          (1'b1),
	.display_wake_gate  (1'b0),
	.kbd_words          ({key_word3, key_word2, key_word1, key_word0}),
	.mouse_buttons      (mouse_buttons),
	.mouse_present      (mouse_present),
	.mouse_x            (mouse_x),
	.mouse_y            (mouse_y),
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
	.step_done          ()
);

///////////////////////   VIDEO   ////////////////////////////////
// 1024 x 808 visible, one pixel per clk_sys (40 MHz): 1344 x 838 total,
// about 35.5 frames/s. The MiSTer scaler converts it for HDMI.

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
always @(posedge clk_sys) begin
	de1  <= (hc < H_VIS) && (vc < V_VIS);
	hs1  <= (hc >= H_VIS + H_FP) && (hc < H_VIS + H_FP + H_SYNC);
	vs1  <= (vc >= V_VIS + V_FP) && (vc < V_VIS + V_FP + V_SYNC);
	de2  <= de1;
	hs2  <= hs1;
	vs2  <= vs1;
	// a set bit is black, as the C emulator's display shows it
	pix2 <= fb_rd_data ? 8'h00 : 8'hFF;
end

assign CLK_VIDEO = clk_sys;
assign CE_PIXEL  = 1'b1;
assign VGA_DE    = de2;
assign VGA_HS    = hs2;
assign VGA_VS    = vs2;
assign VGA_R     = de2 ? pix2 : 8'd0;
assign VGA_G     = de2 ? pix2 : 8'd0;
assign VGA_B     = de2 ? pix2 : 8'd0;

// LED: lit while the machine runs (blinks on the microinstruction count).
assign LED_USER = !halted && cycles[22];

endmodule
