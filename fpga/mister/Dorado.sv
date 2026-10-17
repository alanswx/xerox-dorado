//============================================================================
//  Xerox Dorado for MiSTer -- processor and controllers generated from
//  PARC's Sil wire lists, wrapped in the MiSTer framework.
//
//  DORADO_CHECKPOINT continues actual saved RTL state; DORADO_HPS_PACK adds
//  the mounted serial Trident medium. The separate BaseBoard cold-boot
//  configuration below remains experimental. See README.md for both paths.
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
`ifndef DORADO_CHECKPOINT
assign VGA_F1 = 0;
`endif
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

`ifdef DORADO_HPS_PACK
assign LED_DISK  = {1'b0, sd_rd[0] | sd_wr[0] | pack_dirty};
`else
assign LED_DISK  = 0;
`endif
assign LED_POWER = 0;
assign BUTTONS   = 0;

//////////////////  Aspect ratio  //////////////////
// DispY supports the 1024x808 large format and the Alto portrait display.
// This checkpoint is the Alto, approximately 3:4.

wire [1:0] ar = status[122:121];
`ifdef DORADO_CHECKPOINT
assign VIDEO_ARX = (!ar) ? 12'd3 : (ar - 1'd1);
`else
assign VIDEO_ARX = (!ar) ? 12'd5 : (ar - 1'd1);
`endif
assign VIDEO_ARY = (!ar) ? 12'd4 : 12'd0;

`include "build_id.v"
localparam CONF_STR = {
	"Dorado;;",
	"-;",
	// A Trident pack. The framework mounts the file and reports its size;
	// `img_size` being non-zero is what tells the drive it exists at all.
`ifdef DORADO_HPS_PACK
	"S0,PACKDSK,Mount Trident;",
`elsif DORADO_CHECKPOINT
	"-Executive checkpoint;",
`else
	"S0,PACKDSK,Mount Trident;",
`endif
	"-;",
	"O[122:121],Aspect ratio,Original,Full Screen,[ARC1],[ARC2];",
	"-;",
`ifdef DORADO_CHECKPOINT
	"O[0],Pause,Off,On;",
`else
	"R[0],Reset;",
`endif
	"J,;",
	"V,v",`BUILD_DATE
};

//////////////////  HPS  //////////////////

wire [127:0] status;
wire   [1:0] buttons;
wire         forced_scandoubler;
wire [10:0] ps2_key;
wire [24:0] ps2_mouse;
`ifdef DORADO_HPS_PACK
wire [31:0] sd_lba [1];
wire [5:0] sd_blk_cnt [1];
wire [0:0] sd_rd, sd_wr, sd_ack;
wire [12:0] sd_buff_addr;
wire [15:0] sd_buff_dout, sd_buff_din [1];
wire sd_buff_wr, pack_ready, pack_fault, pack_dirty;
`endif

hps_io #(.CONF_STR(CONF_STR), .WIDE(1)) hps_io
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
	.ps2_key(ps2_key),
	.ps2_mouse(ps2_mouse),
`ifdef DORADO_HPS_PACK
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_din(sd_buff_din), .sd_buff_wr(sd_buff_wr),
`endif

	// The virtual drive. `img_size` is non-zero once the user mounts a pack,
	// which is what tells the Trident a drive is there at all.
	.img_mounted(img_mounted),
	.img_readonly(img_readonly),
	.img_size(img_size)
);

wire [1:0]  img_mounted;
wire        img_readonly;
wire [63:0] img_size;

//////////////////  Clocks  //////////////////
//
// SYSPER counts fabric samples per 30 ns CLOCK. Two clocks execute one
// 60 ns microinstruction (HM 2.3), so the instruction rate is
// clk_sys / (2 * MACHINE_SYSPER). At 33.333 MHz and SYSPER=4 this is
// 4.167 M microinstructions/s, one quarter of a real Dorado's rate.
//
// The checkpoint/HPS fit (2026-10-16) reported 48.06 MHz core Fmax.
// The current fresh-boot and interactive reference uses SYSPER=4.
localparam integer MACHINE_SYSPER = 4;

// CLK_VIDEO MUST COME FROM A PLL, not a clock pin -- the framework's clock
// select blocks reject a pin driver outright ("inclk[3] ... must be driven by
// a PLL's output clock"). So the PLL is not optional dressing here.
//
// build.sh retunes the template PLL from 20 MHz to 33.333333 MHz.
wire clk_sys, pll_locked;
pll pll
(
	.refclk   (CLK_50M),
	.rst      (1'b0),
	.outclk_0 (clk_sys),
	.locked   (pll_locked)
);

wire reset = RESET | status[0] | buttons[1] | ~pll_locked;

// USB events arrive even while a disk transfer/pause stops the machine.
// Retain those deltas here, then acknowledge only actual terminal reports.
wire [15:0] mouse_keys;
wire signed [8:0] mouse_dx, mouse_dy, mouse_sent_dx, mouse_sent_dy;
wire mouse_pending, mouse_taken;
`ifdef DORADO_CHECKPOINT
wire mouse_reset = ~pll_locked;
`else
wire mouse_reset = reset;
`endif
dorado_mouse u_mouse (
    .clk(clk_sys), .reset(mouse_reset), .ps2_mouse(ps2_mouse),
    .key_word4(mouse_keys), .mouse_dx(mouse_dx), .mouse_dy(mouse_dy),
    .mouse_pending(mouse_pending), .mouse_taken(mouse_taken),
    .mouse_sent_dx(mouse_sent_dx), .mouse_sent_dy(mouse_sent_dy), .motion_overflow()
);

// DispY's pixel clock is an INPUT to the machine: its crystal (a05, 50 MHz on
// the built Rev Cl sheet) sits outside the generated netlist, so the core
// drives it -- right by construction rather than approximation.
wire pixel_clk;
`ifndef DORADO_CHECKPOINT
cell_K1115A #(.FREQ_KHZ(50000), .SYSPER(2 * MACHINE_SYSPER)) u_pixclk
(
    .sys_clk(clk_sys), .p8(pixel_clk), .p7(1'b0), .p14(1'b1)
);
`endif

// Declared here because the terminal's bit clock below is derived from
// `hblank`, which the machine drives further down.
wire alto_video, alto_hsync, alto_vsync_n, hblank, vblank;

`ifdef DORADO_CHECKPOINT
// Freeze every saved register until the PLL is locked. A dedicated global
// clock-control block avoids a LUT-gated fabric clock and starts the whole
// continuation together. Framework reset pauses this first hardware test;
// reloading the RBF restores its saved power-up state.
wire machine_clk;
`ifdef DORADO_HPS_PACK
wire machine_enable = ~reset && pack_ready;
`else
wire machine_enable = ~reset;
`endif
altclkctrl #(.clock_type("Global Clock"), .ena_register_mode("falling edge"),
             .intended_device_family("Cyclone V")) machine_clock
    (.inclk({3'b000, clk_sys}), .clkselect(2'b00), .ena(machine_enable),
     .outclk(machine_clk));
dorado_checkpoint_machine u_checkpoint (
    .sys_clk(machine_clk), .ps2_key(ps2_key), .pixel_clk(pixel_clk),
    .mouse_keys(mouse_keys), .mouse_dx(mouse_dx), .mouse_dy(mouse_dy),
    .mouse_pending(mouse_pending), .mouse_taken(mouse_taken),
    .mouse_sent_dx(mouse_sent_dx), .mouse_sent_dy(mouse_sent_dy),
`ifdef DORADO_HPS_PACK
    .clk_host(clk_sys), .pack_reset(~pll_locked), .pack_attached(|img_size), .pack_readonly(img_readonly), .pack_mounted(img_mounted[0]),
    .sd_lba(sd_lba[0]), .sd_blk_cnt(sd_blk_cnt[0]), .sd_rd(sd_rd[0]), .sd_wr(sd_wr[0]), .sd_ack(sd_ack[0]),
    .sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_din(sd_buff_din[0]), .sd_buff_wr(sd_buff_wr),
    .pack_ready(pack_ready), .pack_fault(pack_fault), .pack_dirty(pack_dirty),
`endif
    .video(alto_video), .hsync(alto_hsync), .vsync_n(alto_vsync_n),
    .hblank(hblank), .vblank(vblank), .halfline()
);
`else
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
	.key_word4     (mouse_keys),
	.mouse_dx      (mouse_dx),
	.mouse_dy      (mouse_dy),
	.mouse_pending (mouse_pending),
	.mouse_taken   (mouse_taken),
	.mouse_sent_dx (mouse_sent_dx),
	.mouse_sent_dy (mouse_sent_dy),
	.OISData       (ois_data),
	.OISData_n     (ois_data_n)
);

//////////////////  The disk  //////////////////
//
// Everything past the DskEth connector is a separate box, so a drive has to
// be modelled and plugged in. `dorado_trident` presents Ready/OnLine/TERM
// together (TERM is part of being on line -- c24 wired-ORs them), Selected
// separately, and SecIndx' rotating at a real 3600 RPM.
//
// The cable lines are INPUTS to the machine now. They used to be outputs,
// because their only on-board driver is a SIP pull-up and the generator read
// that as the board driving them -- backwards, since the far end is a drive.
//
// `attached` comes from the HPS: a drive exists when an image is mounted.
// With nothing mounted every line idles DEASSERTED, which is what "no drive"
// has to look like -- six of these once read asserted and invented one.
wire disk_attached = |img_size;

wire dsk_ready_n, dsk_online_n, dsk_term_n, dsk_sel0_n, dsk_secidx0_n;
wire dsk_dp, dsk_dm, dsk_cp, dsk_cm, dsk_adv;

// THE PACK'S CONTENTS. Nothing fills this yet -- the HPS block-read path is
// the next piece -- so the drive reads a blank pack, which is a truthful
// state rather than a fabricated one. `data_adv` is where a real buffer
// would be advanced.
wire dsk_bit = 1'b0;

dorado_trident #(.SYSPER(2 * MACHINE_SYSPER)) u_disk0
(
	.sys_clk    (clk_sys),
	.reset      (reset),
	.attached   (disk_attached),
	// Unit selection comes from the controller's tag lines; until those are
	// decoded here, unit 0 answers whenever a pack is mounted.
	.selected   (disk_attached),
	.TtlReady_n (dsk_ready_n),
	.TtlOnLine_n(dsk_online_n),
	.TtlTerm_n  (dsk_term_n),
	.Selected_n (dsk_sel0_n),
	.SecIndx_n  (dsk_secidx0_n),
	.data_bit   (dsk_bit),
	.data_adv   (dsk_adv),
	.DataP      (dsk_dp), .DataM(dsk_dm),
	.ClockP     (dsk_cp), .ClockM(dsk_cm)
);

//////////////////  The machine  //////////////////

// The processor counts samples per clock; the drive above counts samples
// per microinstruction, hence its factor of two.
dorado_backplane #(.SYSPER(MACHINE_SYSPER)) u_dorado
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
	.OISData_p_   (ois_data_n),

	// The drive cable. Unit 0 is modelled; the rest of the cable idles
	// DEASSERTED, which is ACTIVE LOW and therefore 1 -- tying any of these
	// to 0 fabricates a drive.
	.TtlReady_p_    (dsk_ready_n),
	.TtlOnLine_p_   (dsk_online_n),
	.TtlTerm_p_     (dsk_term_n),
	.Selected0_p_   (dsk_sel0_n),
	.SecIndx0_p_    (dsk_secidx0_n),
	.TtlReadOnly_p_ (1'b1),
	.TtlEndOfCyl_p_ (1'b1),
	.TtlIndex_p_    (1'b1),
	.Selected1_p_   (1'b1), .Selected2_p_(1'b1), .Selected3_p_(1'b1),
	.SecIndx1_p_    (1'b1), .SecIndx2_p_ (1'b1), .SecIndx3_p_ (1'b1),

	// The read stream. These are the CABLE's contribution to a wired-OR the
	// machine also drives (the controller's SN74125 tri-states, for writes),
	// so they arrive on `__in` and the boards read the OR.
	.DataP0__in     (dsk_dp), .DataM0__in (dsk_dm),
	.ClockP0__in    (dsk_cp), .ClockM0__in(dsk_cm),
	.DataP1__in     (1'b0), .DataM1__in (1'b0),
	.DataP2__in     (1'b0), .DataM2__in (1'b0),
	.DataP3__in     (1'b0), .DataM3__in (1'b0),
	.ClockP1__in    (1'b0), .ClockM1__in(1'b0),
	.ClockP2__in    (1'b0), .ClockM2__in(1'b0),
	.ClockP3__in    (1'b0), .ClockM3__in(1'b0)

	// Every other port is left unconnected on purpose -- see the header.
);
`endif

//////////////////  Video  //////////////////
//
// The Dorado's monochrome display is genuinely ONE BIT PER PIXEL (the colour
// board, DispM, is a separate screen at its own raster and is not in this
// configuration). So the pixel expands to white or black.

assign CLK_VIDEO = clk_sys;
reg pixel_d = 1'b0;
always @(posedge clk_sys) pixel_d <= pixel_clk;
assign CE_PIXEL  = pixel_clk & ~pixel_d;

assign VGA_R  = {8{alto_video}};
assign VGA_G  = {8{alto_video}};
assign VGA_B  = {8{alto_video}};
assign VGA_HS = alto_hsync;
assign VGA_VS = ~alto_vsync_n;      // the machine's VSync is active low
assign VGA_DE = ~(hblank | vblank);
`ifdef DORADO_CHECKPOINT
// Alto video is two interlaced fields; the HPS scaler needs the field ID to
// weave the fields back into the portrait raster. This saved continuation
// starts in the odd field. Starting F1 at zero swaps every adjacent pair of
// rows; hardware captures matched the reference exactly after that swap.
reg field_id = 1, field_vs_d = 1;
always @(posedge clk_sys) begin
    field_vs_d <= alto_vsync_n;
    if (field_vs_d && !alto_vsync_n) field_id <= ~field_id;
end
assign VGA_F1 = field_id;
`endif

// THE MACHINE HAS NO RESET PORT and that is correct: the BaseBoard generates
// its own, through the g21/g22/g23 watchdog chain that firmware-probe
// measures. So `reset` here is the FRAMEWORK's reset, and until it is wired
// into the BaseBoard's boot button it is only shown, not applied.
`ifdef DORADO_HPS_PACK
assign LED_USER = reset | pack_fault;
`else
assign LED_USER = reset;
`endif

endmodule
