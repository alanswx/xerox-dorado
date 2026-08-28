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
	.status_menumask(0),

	// The framework decodes the USB/PS2 keyboard for us and hands over one
	// event at a time: {strobe_toggle, pressed, extended, code[7:0]}. The
	// strobe TOGGLES, so an edge on it is an event -- a repeat of the same
	// code is a distinct event, which matters here because the Dorado's
	// terminal reports TRANSITIONS.
	.ps2_key(ps2_key)
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
// (fpga/quartus/reports), so SYSPER=2 at 33.3 MHz is the real-time target --
// and SYSPER=2 IS NOW GATED. Every whole-machine bench passes at 16x, 8x, 4x
// and 2x alike: the memory section, the IFU, the disk, the display, the
// storage array, and the full nine-board world running PARC's own microcode,
// boot chain included. It is a real ratio change, not a relabelled one -- over
// an unchanged 20,000-cycle budget tb_exec executes 1,242 microinstructions at
// 16x and 9,994 at 2x, a clean doubling per halving, with identical decoded
// results. cell_CLOCKGEN's header says the oversampled detector cascade "is
// what forces 16 sys_clk per microinstruction"; for every section gated so
// far, it does not.

// CLK_VIDEO MUST COME FROM A PLL, not a clock pin -- the framework's clock
// select blocks reject a pin driver outright ("inclk[3] ... must be driven by
// a PLL's output clock"). So the PLL is not optional dressing here.
//
// build.sh retunes the template PLL from 20 MHz to 33.333333 MHz, which at
// SYSPER=2 is 16.67 MHz of microinstructions -- a 60 ns cycle, REAL DORADO
// TIME -- and still 32% below the 48.99 MHz Fmax. (The old 20 MHz at SYSPER=16
// was 1.25 MHz, 0.075x.) Going faster than real time is possible up to Fmax,
// but authentic speed is the right default for a machine whose software was
// written against a 60 ns cycle.
wire clk_sys, pll_locked;
pll pll
(
	.refclk   (CLK_50M),
	.rst      (1'b0),
	.outclk_0 (clk_sys),
	.locked   (pll_locked)
);

wire reset = RESET | status[0] | buttons[1] | ~pll_locked;

// DispY's pixel clock is an INPUT to the machine: its crystal (a05, 50 MHz on
// the built Rev Cl sheet) sits outside the generated netlist, so the core
// drives it -- right by construction rather than approximation.
wire pixel_clk = clk_sys;

// Declared here because the terminal's bit clock below is derived from
// `hblank`, which the machine drives further down.
wire alto_video, alto_hsync, alto_vsync_n, hblank, vblank;

//////////////////  Keyboard, and the terminal that carries it  //////////////
//
// The Dorado's keyboard does not reach the machine as parallel bits. A
// microcomputer in the TERMINAL serialises keyboard, keyset and mouse into
// the 32-bit messages of HM Table 24 and sends them back up the 7-wire cable,
// arriving at the BaseBoard as the OISData differential pair:
//
//     BaseBd OISData/OISData' -> MC10125 -> TKeyboardData -> MC10124
//         -> KeyboardData -> backplane -> DispY c22 -> OISRcvdData -> IOB.00
//
// c22 is clocked by RamdHBlank, so the machine samples that line ONCE PER
// SCAN LINE and a message takes 32 lines. A real terminal is a separate box
// whose only timing reference is the video being sent to it, so it clocks its
// reply off that -- which is exactly what `hblank` is doing here. The rate is
// right by construction rather than by a constant to keep in step.

wire [10:0] ps2_key;
wire [15:0] kw0, kw1, kw2, kw3;

dorado_keyboard u_kbd
(
	.sys_clk   (clk_sys),
	.reset     (reset),
	.ps2_key   (ps2_key),
	.key_word0 (kw0),
	.key_word1 (kw1),
	.key_word2 (kw2),
	.key_word3 (kw3)
);

// One pulse per horizontal blanking -- the terminal's bit clock.
reg hblank_d;
always @(posedge clk_sys) hblank_d <= hblank;
wire line_tick = hblank & ~hblank_d;

wire ois_data, ois_data_n;

dorado_terminal u_term
(
	.sys_clk       (clk_sys),
	.reset         (reset),
	.line_tick     (line_tick),
	.key_word0     (kw0),
	.key_word1     (kw1),
	.key_word2     (kw2),
	.key_word3     (kw3),
	// Word 4 is mouse buttons and the keyset (Alto 177033B). No mouse is
	// wired yet, and the cell is ACTIVE LOW -- an unseeded one reads as every
	// button and keyset key HELD, which is what once stopped Smalltalk with
	// "the keyset is stuck". So it idles all-ones deliberately.
	.key_word4     (16'hFFFF),
	.mouse_dx      (9'sd0),
	.mouse_dy      (9'sd0),
	.mouse_pending (1'b0),
	.mouse_taken   (),
	.mouse_sent_dx (),
	.mouse_sent_dy (),
	.OISData       (ois_data),
	.OISData_n     (ois_data_n)
);

//////////////////  The machine  //////////////////

// SYSPER=2 -- one microinstruction per two sys_clk. The parameter defaults to
// 16 and MUST be passed: leaving it default with the retuned PLL would give
// 33.3/16 = 2.08 MHz of microinstructions, i.e. an eighth of real time, and
// nothing would look wrong.
dorado_backplane #(.SYSPER(2)) u_dorado
(
	.sys_clk      (clk_sys),
	.RawPixelClk  (pixel_clk),
	.PixelClkVCO  (pixel_clk),

	.AltoTTLVideo (alto_video),
	.AltoHSync    (alto_hsync),
	.AltoVSync_p_ (alto_vsync_n),
	.HBlank       (hblank),
	.VBlank       (vblank),

	// The terminal's return channel, at the BaseBoard.
	.OISData      (ois_data),
	.OISData_p_   (ois_data_n)

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

// THE MACHINE HAS NO RESET PORT and that is correct: the BaseBoard generates
// its own, through the g21/g22/g23 watchdog chain that firmware-probe
// measures. So `reset` here is the FRAMEWORK's reset, and until it is wired
// into the BaseBoard's boot button it is only shown, not applied.
assign LED_USER = reset;

endmodule
