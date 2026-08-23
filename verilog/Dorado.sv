//============================================================================
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//============================================================================

module emu
(
	//Master input clock
	input         CLK_50M,

	//Async reset from top-level module.
	//Can be used as initial reset.
	input         RESET,

	//Must be passed to hps_io module
	inout  [45:0] HPS_BUS,

	//Base video clock. Usually equals to CLK_SYS.
	output        CLK_VIDEO,

	//Multiple resolutions are supported using different CE_PIXEL rates.
	//Must be based on CLK_VIDEO
	output        CE_PIXEL,

	//Video aspect ratio for HDMI.
	output [12:0] VIDEO_ARX,
	output [12:0] VIDEO_ARY,

	output  [7:0] VGA_R,
	output  [7:0] VGA_G,
	output  [7:0] VGA_B,
	output        VGA_HS,
	output        VGA_VS,
	output        VGA_DE,    // = ~(VBlank | HBlank)
	output        VGA_F1,
	output [1:0]  VGA_SL,
	output        VGA_SCALER,  // Force VGA scaler
	output        VGA_DISABLE,

	input  [11:0] HDMI_WIDTH,
	input  [11:0] HDMI_HEIGHT,
	output        HDMI_FREEZE,
	output        HDMI_BLACKOUT,
	output        HDMI_BOB_DEINT,



`ifdef MISTER_FB
	// Use framebuffer in DDRAM (USE_FB=1 in qsf)
	// FB_FORMAT:
	//    [2:0] : 011=8bpp(palette) 100=16bpp 101=24bpp 110=32bpp
	//    [3]   : 0=16bits 565 1=16bits 1555
	//    [4]   : 0=RGB  1=BGR (for 16/24/32 modes)
	//
	// FB_STRIDE either 0 (rounded to 256 bytes) or multiple of pixel size (in bytes)
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
	// Palette control for 8bit modes.
	// Ignored for other video modes.
	output        FB_PAL_CLK,
	output  [7:0] FB_PAL_ADDR,
	output [23:0] FB_PAL_DOUT,
	input  [23:0] FB_PAL_DIN,
	output        FB_PAL_WR,
`endif
`endif

	output        LED_USER,  // 1 - ON, 0 - OFF.

	// b[1]: 0 - LED status is system status OR'd with b[0]
	//       1 - LED status is controled solely by b[0]
	// hint: supply 2'b00 to let the system control the LED.
	output  [1:0] LED_POWER,
	output  [1:0] LED_DISK,

	// I/O board button press simulation (active high)
	// b[1]: user button
	// b[0]: osd button
	output  [1:0] BUTTONS,

	input         CLK_AUDIO, // 24.576 MHz
	output [15:0] AUDIO_L,
	output [15:0] AUDIO_R,
	output        AUDIO_S,   // 1 - signed audio samples, 0 - unsigned
	output  [1:0] AUDIO_MIX, // 0 - no mix, 1 - 25%, 2 - 50%, 3 - 100% (mono)

	//ADC
	inout   [3:0] ADC_BUS,

	//SD-SPI
	output        SD_SCK,
	output        SD_MOSI,
	input         SD_MISO,
	output        SD_CS,
	input         SD_CD,

	//High latency DDR3 RAM interface
	//Use for non-critical time purposes
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

	//SDRAM interface with lower latency
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
	//Secondary SDRAM
	//Set all output SDRAM_* signals to Z ASAP if SDRAM2_EN is 0
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

	// Open-drain User port.
	// 0 - D+/RX
	// 1 - D-/TX
	// 2..6 - USR2..USR6
	// Set USER_OUT to 1 to read from USER_IN.
	input   [6:0] USER_IN,
	output  [6:0] USER_OUT,

	input         OSD_STATUS
);

////////////////////////////////////////////////////////////////////////////
// THE DORADO CORE -- the MiSTer `emu` wrapper.
//
// Everything below the PLL is generated from PARC's own Sil wire lists (see
// docs/verilog-from-sil.md): sixteen boards wired by NAME across a backplane
// that is not straight-through, of which `dorado_machine` instantiates the
// eleven of a working monochrome machine plus its clock distribution.
//
// The framework parts -- CONF_STR and hps_io -- are kept deliberately even
// though nothing consumes them yet. The Dorado needs the HPS for all three of
// its real input paths: DISK images for the Trident packs, the KEYBOARD (the
// Alto 61-key matrix, audited against three sources -- see
// docs/parc-feedback-todo.md), and the MOUSE, whose buttons the guest reads
// from UTILIN at 0177030. Wiring them up is the I/O work; having the bus
// present from the start is what makes that a wiring job rather than a
// rebuild.
////////////////////////////////////////////////////////////////////////////

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_TXD, UART_DTR} = 0;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML,
        SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;
assign {DDRAM_CLK, DDRAM_BURSTCNT, DDRAM_ADDR, DDRAM_DIN, DDRAM_BE,
        DDRAM_RD, DDRAM_WE} = '0;

assign VGA_SL   = 0;
assign VGA_F1   = 0;
assign VGA_SCALER = 0;
assign VGA_DISABLE = 0;
assign HDMI_FREEZE = 0;
assign HDMI_BLACKOUT = 0;
assign HDMI_BOB_DEINT = 0;

assign AUDIO_S   = 0;
assign AUDIO_L   = 0;
assign AUDIO_R   = 0;
assign AUDIO_MIX = 0;

assign LED_DISK  = 0;
assign LED_POWER = 0;
assign BUTTONS   = 0;

// The Dorado's monochrome display is 1024 x 808 (docs/hardware-specs.md).
// Its colour board, DispM, is a SEPARATE SCREEN at its own raster -- not a
// colourisation of this one -- so it is not an aspect-ratio question here.
assign VIDEO_ARX = 13'd1024;
assign VIDEO_ARY = 13'd808;

localparam CONF_STR = {
	"Dorado;;",
	"-;",
	"F1,MB,Load microcode;",
	"S0,PACK,Trident pack;",
	"-;",
	"O[2],Aspect ratio,Original,Full Screen;",
	"-;",
	"R[0],Reset;",
	"J,Button;",
	"jn,A;",
	"V,v",`BUILD_DATE
};

wire        forced_scandoubler;
wire [21:0] gamma_bus;
wire  [1:0] buttons;
wire [127:0] status;
wire [15:0] status_menumask = 0;
wire [127:0] status_in = 0;
wire        status_set = 0;

wire [10:0] ps2_key;
wire [24:0] ps2_mouse;

wire        ioctl_download;
wire  [7:0] ioctl_index;
wire        ioctl_wr;
wire [24:0] ioctl_addr;
wire  [7:0] ioctl_data;

// Disk: the Dorado boots from Trident packs, so the SD interface is here from
// the start even though the disk controller is not wired to it yet.
wire [31:0] sd_lba[1];
wire  [5:0] sd_blk_cnt[1];
wire        sd_rd, sd_wr, sd_ack;
wire  [7:0] sd_buff_addr;
wire [15:0] sd_buff_dout, sd_buff_din[1];
wire        sd_buff_wr;
wire        img_mounted, img_readonly;
wire [63:0] img_size;

hps_io #(.CONF_STR(CONF_STR), .VDNUM(1)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
	.EXT_BUS(),
	.gamma_bus(gamma_bus),

	.forced_scandoubler(forced_scandoubler),

	.ioctl_download(ioctl_download),
	.ioctl_index(ioctl_index),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_data),

	.buttons(buttons),
	.status(status),
	.status_in(status_in),
	.status_set(status_set),
	.status_menumask(status_menumask),

	.sd_lba(sd_lba),
	.sd_blk_cnt(sd_blk_cnt),
	.sd_rd(sd_rd),
	.sd_wr(sd_wr),
	.sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr),
	.sd_buff_dout(sd_buff_dout),
	.sd_buff_din(sd_buff_din),
	.sd_buff_wr(sd_buff_wr),
	.img_mounted(img_mounted),
	.img_readonly(img_readonly),
	.img_size(img_size),

	.ps2_key(ps2_key),
	.ps2_mouse(ps2_mouse)
);

///////////////////////   CLOCKS   ///////////////////////////////

wire clk_sys;
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys)
);

////////////////////////   THE MACHINE   /////////////////////////
//
// `sys_clk` is NOT the Dorado's 16.67 MHz microinstruction rate. The cells
// recover each distributed ECL clock net by OVERSAMPLING it --
//
//     always @(posedge sys_clk) ck_d <= p7;
//     wire ck_en = p7 & ~ck_d;        // an edge on the modelled clock net
//
// -- the transform that let 1,201 clocked packages run on one fabric clock
// instead of 1,201 gated ones. So sys_clk only has to resolve the machine's
// clock PHASES, and HOW MUCH FASTER IS STILL OPEN: the Verilator benches use
// 16x, which is a testbench constant rather than a derived requirement, and
// 16 x 16.67 MHz = 267 MHz is not reachable on a Cyclone V. Lowering it also
// speeds simulation nearly proportionally, since 15 of every 16 evaluations
// currently do nothing but divide the clock. Measure the minimum that keeps
// the 32 gates green before fixing the PLL.

wire [31:0] probe_val;
wire [15:0] probe_words;

dorado_machine u_dorado
(
	.sys_clk(clk_sys),
	.probe_sel(status[31:16]),
	.probe_val(probe_val),
	.probe_words(probe_words)
);

// KEEP THE MACHINE REACHABLE, or there is nothing to measure.
//
// With `probe_sel` tied to a constant and only one probe bit reaching an LED,
// Quartus swept the whole design away and fitted an empty shell: 1,024 ALMs
// of 41,910, 2%, which is not a Dorado. `probe_sel` comes from the OSD status
// word now and every bit of `probe_val` reaches a pin, so the probe mux --
// and everything feeding it -- has to be built.
//
// This is scaffolding, not the design. It goes when the display controller
// lands and consumes the machine's state for real.
assign LED_USER = ^probe_val;

assign CLK_VIDEO = clk_sys;
assign CE_PIXEL  = 1'b1;
assign VGA_R  = probe_val[7:0];
assign VGA_G  = probe_val[15:8];
assign VGA_B  = probe_val[23:16];
assign VGA_HS = probe_val[24];
assign VGA_VS = probe_val[25];
assign VGA_DE = probe_val[26];

endmodule
