//============================================================================
//  Xerox Dorado for MiSTer -- the eleven-board machine, generated from PARC's
//  own Sil wire lists, wrapped in the MiSTer framework.
//
//  This is the FIRST core: it instantiates the machine, drives its clocks, and
//  brings the monochrome video out. Disk, keyboard and main storage are the
//  next pieces and are deliberately absent rather than stubbed misleadingly --
//  see fpga/mister/README.md for what each one needs.
//
//  WHY THE MACHINE'S UNUSED PORTS ARE LEFT UNCONNECTED. dorado_backplane has
//  386 ports because every cable leaving the backplane is one, and they are
//  GENERATED from the wire lists -- they are the machine's real interface, not
//  a convenience. Trimming them would fork the netlist. Leaving them dangling
//  lets Quartus strip the logic behind them, which is what makes an unwired
//  DskEth cable cost nothing. Simulation loses nothing by it: the Verilator
//  benches reach into the hierarchy directly (m.b_ContA.RepeatCur and the
//  like) and need no ports at all.
//
//  CAUTION when reading a fit after this: the standalone virtual-pin project
//  (fpga/quartus) measured 25,644 ALMs with NOTHING stripped, which is the
//  upper bound. A large drop here is ambiguous -- it could be peripherals
//  correctly removed, or something load-bearing silently removed. Compare
//  against that baseline.
//============================================================================

module emu
(
	`include "sys/emu_ports.vh"
);

//////////////////  Unused framework outputs  //////////////////

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_TXD, UART_DTR} = 0;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML,
        SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;
assign {DDRAM_CLK, DDRAM_BURSTCNT, DDRAM_ADDR, DDRAM_DIN,
        DDRAM_BE, DDRAM_RD, DDRAM_WE} = '0;

assign VGA_SL = 0;
assign VGA_F1 = 0;
assign VGA_SCALER  = 0;
assign VGA_DISABLE = 0;
assign HDMI_FREEZE = 0;
assign HDMI_BLACKOUT = 0;
assign HDMI_BOB_DEINT = 0;

// The Dorado has no sound. The 1981 Hardware Manual's I/O section lists disk,
// ethernet and display; audio is not among them.
assign AUDIO_S = 0;
assign AUDIO_L = 0;
assign AUDIO_R = 0;
assign AUDIO_MIX = 0;

assign LED_DISK  = 0;
assign LED_POWER = 0;
assign BUTTONS   = 0;

//////////////////  Aspect ratio  //////////////////
// The DispY monochrome display is 1024x808 -- very nearly 5:4, not 4:3.

wire [1:0] ar = status[122:121];
assign VIDEO_ARX = (!ar) ? 12'd5 : (ar - 1'd1);
assign VIDEO_ARY = (!ar) ? 12'd4 : 12'd0;

`include "build_id.v"
localparam CONF_STR = {
	"Dorado;;",
	"-;",
	"O[122:121],Aspect ratio,Original,Full Screen,[ARC1],[ARC2];",
	"-;",
	"R[0],Reset;",
	"J,;",
	"V,v",`BUILD_DATE
};

//////////////////  HPS  //////////////////

wire [127:0] status;
wire   [1:0] buttons;
wire         forced_scandoubler;

hps_io #(.CONF_STR(CONF_STR)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
	.EXT_BUS(),
	.gamma_bus(),
	.forced_scandoubler(forced_scandoubler),
	.buttons(buttons),
	.status(status),
	.status_menumask(0)
);

//////////////////  Clocks  //////////////////
//
// THE MACHINE'S CLOCK IS NOT ITS MICROINSTRUCTION RATE. The RTL recovers each
// distributed ECL clock net by OVERSAMPLING it SYSPER times per
// microinstruction, so a real 60 ns cycle needs sys_clk = SYSPER x 16.67 MHz:
//
//     SYSPER=16 -> 266.7 MHz     SYSPER=4 -> 66.7 MHz
//     SYSPER=8  -> 133.3 MHz     SYSPER=2 -> 33.3 MHz
//
// Measured Fmax for the eleven-board machine on this device is 48.99 MHz
// (fpga/quartus/reports), so SYSPER=2 at 33.3 MHz is the real-time target and
// is what a PLL should eventually produce. This first core runs sys_clk from
// CLK_50M directly: it is deliberate and honest -- 50 MHz is just above Fmax,
// so the timing report will say so, and that is the measurement this build
// exists to take. Do not read a small negative slack here as a surprise.

wire clk_sys = CLK_50M;
wire reset = RESET | status[0] | buttons[1];

// DispY's pixel clock is an INPUT to the machine: its crystal (a05, 50 MHz on
// the built Rev Cl sheet) is outside the generated netlist. Driving it from
// CLK_50M is right by construction rather than approximation.
wire pixel_clk = CLK_50M;

//////////////////  The machine  //////////////////

wire alto_video, alto_hsync, alto_vsync_n, hblank, vblank;

dorado_backplane u_dorado
(
	.sys_clk      (clk_sys),
	.RawPixelClk  (pixel_clk),
	.PixelClkVCO  (pixel_clk),

	.AltoTTLVideo (alto_video),
	.AltoHSync    (alto_hsync),
	.AltoVSync_p_ (alto_vsync_n),
	.HBlank       (hblank),
	.VBlank       (vblank)

	// Every other port is left unconnected on purpose -- see the header.
);

//////////////////  Video  //////////////////
//
// The Dorado's monochrome display is genuinely ONE BIT PER PIXEL (the colour
// board, DispM, is a separate screen at its own raster and is not in this
// configuration). So the pixel expands to white or black.

assign CLK_VIDEO = clk_sys;
assign CE_PIXEL  = 1'b1;

assign VGA_R  = {8{alto_video}};
assign VGA_G  = {8{alto_video}};
assign VGA_B  = {8{alto_video}};
assign VGA_HS = alto_hsync;
assign VGA_VS = ~alto_vsync_n;      // the machine's VSync is active low
assign VGA_DE = ~(hblank | vblank);

assign LED_USER = 1'b0;

endmodule
