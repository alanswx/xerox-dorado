//============================================================================
//  Xerox Dorado for MiSTer -- processor and controllers generated from
//  PARC's Sil wire lists, wrapped in the MiSTer framework.
//
//  DORADO_CHECKPOINT continues actual saved RTL state; DORADO_HPS_PACK adds
//  the mounted serial Trident medium. The separate BaseBoard cold-boot
//  configuration below remains experimental. See README.md for both paths.
//============================================================================

`ifdef DORADO_NATIVE
`define DORADO_CLOCKED_MACHINE
`elsif DORADO_CHECKPOINT
`define DORADO_CLOCKED_MACHINE
`endif

module emu
(
	`include "sys/emu_ports.vh"
);

`ifdef DORADO_ETHERNET_DDR
`ifndef DORADO_NATIVE
initial $error("DDR frame transport requires a native machine");
`endif
`ifndef DORADO_STORAGE_SDRAM
initial $error("DDR frame transport currently requires bulk storage in SDRAM");
`endif
`ifndef DORADO_ETHERNET
initial $error("DDR frame transport requires the original Ethernet wire adapter");
`endif
`endif

//////////////////  Unused framework outputs  //////////////////

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_TXD, UART_DTR} = 0;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
`ifndef DORADO_STORAGE_SDRAM
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML,
        SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;
`else
wire [15:0] storage_dq_out;
wire storage_dq_oe;
assign SDRAM_CLK=~clk_sys;
assign SDRAM_DQ=storage_dq_oe ? storage_dq_out : 16'hzzzz;
`endif
`ifndef DORADO_NATIVE
assign {DDRAM_CLK, DDRAM_BURSTCNT, DDRAM_ADDR, DDRAM_DIN,
        DDRAM_BE, DDRAM_RD, DDRAM_WE} = '0;
`else
assign DDRAM_CLK = clk_sys;
`endif

assign VGA_SL = 0;
`ifndef DORADO_CHECKPOINT
`ifndef DORADO_DISPLAY_FIELD
assign VGA_F1 = 0;
`endif
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
`ifdef DORADO_ETHERNET_HOST_PORT
	"O[2:1],Ethernet station,042,001,377,000;",
`endif
	"-;",
`ifdef DORADO_CLOCKED_MACHINE
	"O[0],Pause,Off,On;",
`else
	"R[0],Reset;",
`endif
`ifdef DORADO_NATIVE
	"O[3],Boot button,Released,Pressed;",
	"O[4],Display,Large Format,Alto;",
	"O[8:5],Debug monitor,CPU,Clock counts,DDR pauses,ARM pauses,Combined pauses,DDR reads-writes,Read cache,DDR bus waits,ARM read waits,ARM write waits,Max DDR pause,Max ARM pauses,Raw requests,Raw replies,Init pause,Pack transfers;",
`ifdef DORADO_INSTRUCTION_MONITOR
	"O[10:9],Instruction monitor,Off,Issued per host clock,Held per cycle,Clocks per issue;",
`endif
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
`ifdef DORADO_ETHERNET
wire [35:0] ethernet_ext_bus;
`endif
`ifdef DORADO_HPS_PACK
wire [31:0] sd_lba [1];
wire [5:0] sd_blk_cnt [1];
wire [0:0] sd_rd, sd_wr, sd_ack;
wire [12:0] sd_buff_addr;
wire [15:0] sd_buff_dout, sd_buff_din [1];
wire sd_buff_wr, pack_ready, pack_fault, pack_dirty;
wire pack_attached;
`ifdef DORADO_PACK_FAULT_MONITOR
wire [31:0] pack_fault_debug,pack_fault_extra;
`endif
dorado_pack_mount u_pack_mount (.clk(clk_sys),.reset(~pll_locked),
    .size_nonzero(|img_size),.mounted(img_mounted[0]),.attached(pack_attached));
`endif

hps_io #(.CONF_STR(CONF_STR), .WIDE(1)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
`ifdef DORADO_ETHERNET
	.EXT_BUS(ethernet_ext_bus),
`else
	.EXT_BUS(),
`endif
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
// The deployed Native26 fit reported 41.08 MHz core Fmax, limited by a
// half-cycle storage-ready -> network_step path. Re-fit/review each candidate.
// The current fresh-boot and interactive reference uses SYSPER=4.
localparam integer MACHINE_SYSPER = 4;

// CLK_VIDEO MUST COME FROM A PLL, not a clock pin -- the framework's clock
// select blocks reject a pin driver outright ("inclk[3] ... must be driven by
// a PLL's output clock"). So the PLL is not optional dressing here.
//
// build.sh retunes the template PLL; default33.333333 MHz, configurable
// with DORADO_SYS_MHZ. Native29's40 MHz is experimental until qualified.
wire clk_sys, pll_locked;
pll pll
(
	.refclk   (CLK_50M),
	.rst      (1'b0),
	.outclk_0 (clk_sys),
	.locked   (pll_locked)
);

`ifdef DORADO_NATIVE
wire reset = RESET | status[0] | ~pll_locked;
`else
wire reset = RESET | status[0] | buttons[1] | ~pll_locked;
`endif

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
`ifndef DORADO_CLOCKED_MACHINE
cell_K1115A #(.FREQ_KHZ(50000), .SYSPER(2 * MACHINE_SYSPER)) u_pixclk
(
    .sys_clk(clk_sys), .p8(pixel_clk), .p7(1'b0), .p14(1'b1)
);
`endif

// Declared here because the terminal's bit clock below is derived from
// `hblank`, which the machine drives further down.
wire alto_video, alto_hsync, alto_vsync_n, hblank, vblank;

`ifdef DORADO_CLOCKED_MACHINE
// Freeze every saved register until the PLL is locked. A dedicated global
// clock-control block avoids a LUT-gated fabric clock and starts the whole
// continuation together. Framework reset pauses this first hardware test;
// reloading the RBF restores its saved power-up state.
wire machine_clk;
`ifdef DORADO_NATIVE
wire storage_ready,storage_initialized;
wire [31:0] baseboard_debug,processor_debug;
`ifdef DORADO_RX_MONITOR
wire [31:0] receive_debug;
`endif
`ifdef DORADO_CONTROL_ERROR_MONITOR
wire [9:0] control_debug;
`else
wire [9:0] control_debug=10'd0;
`endif
wire machine_enable = ~reset && pack_ready && storage_ready;
reg [28:0] boot_samples=0;
always @(posedge machine_clk)if(boot_samples<29'd240000000)boot_samples<=boot_samples+1'b1;
wire baseboard_boot_n=!(status[3] || buttons[1] ||
    (boot_samples>=29'd200000000 && boot_samples<29'd240000000));
wire [15:0] machine_status={control_debug,baseboard_boot_n,pack_fault,storage_initialized,
                           storage_ready,pack_ready,machine_enable};
`elsif DORADO_HPS_PACK
wire machine_enable = ~reset && pack_ready;
`else
wire machine_enable = ~reset;
`endif
`ifdef DORADO_ETHERNET
wire ethernet_rx, ethernet_tx_n, net_rx_ready, net_tx_valid;
wire net_tx_ack, net_rx_wr, net_rx_commit;
wire [9:0] net_tx_addr, net_rx_addr;
wire [15:0] net_tx_data, net_rx_data;
wire [10:0] net_tx_words, net_rx_words;
wire [31:0] net_tx_good,net_tx_bad,net_tx_dropped,net_rx_bad;
`ifdef DORADO_ETHERNET_DDR
wire net_ddr_enable,net_ddr_owned,net_ddr_stopped,net_ddr_initialized;
wire [63:0] net_ddr_epoch;
wire [31:0] net_ddr_fault,net_ddr_control_errors;
wire [9:0] net_spi_tx_addr,net_spi_rx_addr,net_ddr_tx_addr,net_ddr_rx_addr;
wire net_spi_tx_ack,net_spi_rx_wr,net_spi_rx_commit;
wire net_ddr_tx_ack,net_ddr_rx_wr,net_ddr_rx_commit;
wire [15:0] net_spi_rx_data,net_ddr_rx_data;
wire [10:0] net_spi_rx_words,net_ddr_rx_words;
assign net_tx_addr=net_ddr_owned ? net_ddr_tx_addr : net_spi_tx_addr;
assign net_tx_ack=net_ddr_owned ? net_ddr_tx_ack : net_spi_tx_ack;
assign net_rx_addr=net_ddr_owned ? net_ddr_rx_addr : net_spi_rx_addr;
assign net_rx_wr=net_ddr_owned ? net_ddr_rx_wr : net_spi_rx_wr;
assign net_rx_commit=net_ddr_owned ? net_ddr_rx_commit : net_spi_rx_commit;
assign net_rx_data=net_ddr_owned ? net_ddr_rx_data : net_spi_rx_data;
assign net_rx_words=net_ddr_owned ? net_ddr_rx_words : net_spi_rx_words;
// The SDRAM raw backend leaves this host DDR port unused. This engine runs
// independently of machine_enable; only the original wire advances with it.
dorado_ethernet_ddr u_network_ddr (
    .clk(clk_sys),.reset(~pll_locked),.enable(net_ddr_enable),.epoch(net_ddr_epoch),
    .stopped(net_ddr_stopped),.initialized(net_ddr_initialized),.fault(net_ddr_fault),
    .tx_valid(net_tx_valid),.tx_words(net_tx_words),.tx_data(net_tx_data),.rx_ready(net_rx_ready),
    .tx_addr(net_ddr_tx_addr),.tx_ack(net_ddr_tx_ack),.rx_addr(net_ddr_rx_addr),
    .rx_wr(net_ddr_rx_wr),.rx_commit(net_ddr_rx_commit),.rx_data(net_ddr_rx_data),.rx_words(net_ddr_rx_words),
    .ddram_busy(DDRAM_BUSY),.ddram_rvalid(DDRAM_DOUT_READY),.ddram_rdata(DDRAM_DOUT),
    .ddram_addr(DDRAM_ADDR),.ddram_read(DDRAM_RD),.ddram_write(DDRAM_WE),
    .ddram_burstcount(DDRAM_BURSTCNT),.ddram_be(DDRAM_BE),.ddram_wdata(DDRAM_DIN));
`endif
// Match the global clock gate's falling-edge enable latch. Host transfers
// continue while the cable's emulated time pauses with the rest of DskEth.
reg network_step = 0;
always @(negedge clk_sys) network_step <= machine_enable;
`ifdef DORADO_NATIVE
wire [31:0] host_edges,machine_edges;
wire [8:0] storage_events;
wire [2:0] pack_host_phase;
wire [31:0] pack_reads,pack_writes;
wire [767:0] performance;
`ifdef DORADO_INSTRUCTION_MONITOR
wire [4:0] instruction_signals;
wire [31:0] instructions_issued,instruction_holds,instruction_cycles,instruction_clocks;
dorado_instruction_monitor u_instruction_monitor(.clk(clk_sys),.reset(~pll_locked),
    .start_cycle(instruction_signals[4]),.repeat_current(instruction_signals[3]),
    .clock2(instruction_signals[2]),.running(instruction_signals[1]),.jam(instruction_signals[0]),
    .issued(instructions_issued),.held(instruction_holds),
    .cycles(instruction_cycles),.clocks(instruction_clocks));
`endif
dorado_speed_monitor #(.TRACK_CAUSES(1)) u_speed_monitor(.clk(clk_sys),.reset(~pll_locked),
    .machine_step(network_step),.host_edges(host_edges),.machine_edges(machine_edges),
    .reset_requested(reset),.pack_ready(pack_ready),.storage_ready(storage_ready),
    .storage_initialized(storage_initialized),.storage_events(storage_events),
    .pack_host_phase(pack_host_phase),.pack_reads(pack_reads),.pack_writes(pack_writes),
    .performance(performance));
reg [31:0] count_debug,count_reference;
always @*begin
    count_debug=machine_edges;count_reference=host_edges;
    case(status[8:5])
    2:count_debug=performance[4*32+:32];
    3:count_debug=performance[3*32+:32];
    4:count_debug=performance[5*32+:32];
    5:begin count_debug=performance[9*32+:32];count_reference=performance[10*32+:32];end
    6:begin count_debug=performance[11*32+:32];count_reference=performance[7*32+:32];end
    7:begin count_debug=performance[12*32+:32];count_reference=performance[13*32+:32];end
    8:begin count_debug=performance[15*32+:32];count_reference=performance[16*32+:32];end
    9:begin count_debug=performance[17*32+:32];count_reference=performance[18*32+:32];end
    10:count_debug=performance[19*32+:32];
    11:begin count_debug=performance[20*32+:32];count_reference=performance[21*32+:32];end
    12:begin count_debug=performance[7*32+:32];count_reference=performance[8*32+:32];end
    13:count_debug=performance[14*32+:32];
    14:begin count_debug=performance[6*32+:32];count_reference=performance[2*32+:32];end
    15:begin count_debug=performance[22*32+:32];count_reference=performance[23*32+:32];end
    default:begin end
    endcase
`ifdef DORADO_INSTRUCTION_MONITOR
    case(status[10:9])
    1:begin count_debug=instructions_issued;count_reference=host_edges;end
    2:begin count_debug=instruction_holds;count_reference=instruction_cycles;end
    3:begin count_debug=instruction_clocks;count_reference=instructions_issued;end
    default:begin end
    endcase
`endif
end
// In count mode Main's existing machine/cpu words become enabled/host
// fabric edges. u_network_uio snapshots both on the same command70 edge.
wire speed_monitor_active = ((|status[8:5])
`ifdef DORADO_INSTRUCTION_MONITOR
    || (|status[10:9])
`endif
    ) && !pack_fault;
`endif
// Read-only operand monitor. The guest never reads this selector or payload.
`ifdef DORADO_OPERAND_MONITOR
wire [127:0] native_operand_debug;
wire operand_monitor_active=status[15] && !pack_fault;
wire [31:0] operand_word=status[14:11]<4
    ? native_operand_debug[127-32*status[14:11]-:32] : 32'hd0d0bad0;
`else
wire operand_monitor_active=1'b0;
wire [31:0] operand_word=32'd0;
`endif
dorado_ethernet_uio #(.MAX_WORDS(1024)
`ifdef DORADO_ETHERNET_DDR
    ,.DDR_CAPABLE(1)
`endif
) u_network_uio (
    .clk(clk_sys), .reset(~pll_locked), .bus(ethernet_ext_bus),
    .rx_ready(net_rx_ready), .tx_valid(net_tx_valid), .tx_words(net_tx_words), .tx_data(net_tx_data),
    .tx_good(net_tx_good), .tx_bad(net_tx_bad), .tx_dropped(net_tx_dropped), .rx_bad(net_rx_bad),
`ifdef DORADO_NATIVE
`ifdef DORADO_RX_MONITOR
`ifdef DORADO_PACK_FAULT_MONITOR
    .machine_debug(pack_fault ? pack_fault_debug : operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : native_key_overflow ?
        {16'h4b51,7'd0,native_key_queued} :
        (processor_debug[31] ? baseboard_debug : receive_debug)), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : receive_debug), .machine_status(machine_status),
    .processor_debug(operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`endif
`else
`ifdef DORADO_PACK_FAULT_MONITOR
    .machine_debug(pack_fault ? pack_fault_debug : operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`endif
`endif
`else
    .machine_debug(32'd0), .machine_status(16'd0), .processor_debug(32'd0),
`endif
`ifdef DORADO_ETHERNET_DDR
    .tx_addr(net_spi_tx_addr), .tx_ack(net_spi_tx_ack), .rx_wr(net_spi_rx_wr), .rx_commit(net_spi_rx_commit),
    .rx_addr(net_spi_rx_addr), .rx_data(net_spi_rx_data), .rx_words(net_spi_rx_words),
    .ddr_stopped(net_ddr_stopped),.ddr_initialized(net_ddr_initialized),.ddr_fault(net_ddr_fault),
    .ddr_enable(net_ddr_enable),.ddr_epoch(net_ddr_epoch),.ddr_control_errors(net_ddr_control_errors),.ddr_owned(net_ddr_owned),
`else
    .tx_addr(net_tx_addr), .tx_ack(net_tx_ack), .rx_wr(net_rx_wr), .rx_commit(net_rx_commit),
    .rx_addr(net_rx_addr), .rx_data(net_rx_data), .rx_words(net_rx_words),
    .ddr_stopped(1'b1),.ddr_initialized(1'b0),.ddr_fault(32'd0),
    .ddr_enable(),.ddr_epoch(),.ddr_control_errors(),.ddr_owned(),
`endif
    .aborted()
);
dorado_ethernet_wire #(.SYSPER(MACHINE_SYSPER),.MAX_WORDS(1024)) u_network_wire (
    .clk(clk_sys), .reset(~pll_locked), .step(network_step),
    .tx_line(~ethernet_tx_n), .rx_line(ethernet_rx),
    .rx_wr(net_rx_wr), .rx_commit(net_rx_commit), .rx_addr(net_rx_addr),
    .rx_data(net_rx_data), .rx_words(net_rx_words), .rx_ready(net_rx_ready),
    .tx_addr(net_tx_addr), .tx_data(net_tx_data), .tx_ack(net_tx_ack), .tx_valid(net_tx_valid), .tx_words(net_tx_words),
    .tx_good(net_tx_good), .tx_bad(net_tx_bad), .tx_dropped(net_tx_dropped), .rx_bad(net_rx_bad)
);
`endif
altclkctrl #(.clock_type("Global Clock"), .ena_register_mode("falling edge"),
             .intended_device_family("Cyclone V")) machine_clock
    (.inclk({3'b000, clk_sys}), .clkselect(2'b00), .ena(machine_enable),
     .outclk(machine_clk));
`ifdef DORADO_NATIVE
// USB events continue while machine_clk is stopped for disk/DDR service.
// Capture them on clk_sys, then expose transitions for complete source
// fields so the terminal wire and Cedar's retrace watcher can observe them.
wire [10:0] native_key;
wire native_key_overflow;
wire [8:0] native_key_queued;
dorado_key_queue u_key_queue (
    .clk_host(clk_sys), .reset(~pll_locked), .ps2_key(ps2_key),
    .vsync_n(alto_vsync_n), .machine_key(native_key),
    .overflow(native_key_overflow), .queued(native_key_queued)
);
dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (
`ifdef DORADO_OPERAND_MONITOR
    .operand_debug(native_operand_debug),
`endif
    .contb_advance(network_step),
`ifdef DORADO_RX_MONITOR
    .receive_debug(receive_debug),
`endif
`ifdef DORADO_DISPLAY_FIELD
    // HM p.115: the real display VCW selects the interlaced field phase.
    .video_field(VGA_F1),
`endif
    .reset(~pll_locked), .baseboard_boot_n(baseboard_boot_n), .large_format(~status[4]),
    .storage_ready(storage_ready), .storage_initialized(storage_initialized),
`ifdef DORADO_STORAGE_SDRAM
    .sdram_a(SDRAM_A),.sdram_ba(SDRAM_BA),.sdram_ncs(SDRAM_nCS),
    .sdram_nras(SDRAM_nRAS),.sdram_ncas(SDRAM_nCAS),.sdram_nwe(SDRAM_nWE),
    .sdram_dqml(SDRAM_DQML),.sdram_dqmh(SDRAM_DQMH),.sdram_cke(SDRAM_CKE),
    .sdram_dq_out(storage_dq_out),.sdram_dq_oe(storage_dq_oe),.sdram_dq_in(SDRAM_DQ),
`endif
    .storage_events(storage_events),.pack_host_phase(pack_host_phase),
`ifdef DORADO_INSTRUCTION_MONITOR
    .instruction_signals(instruction_signals),
`endif
    .pack_reads(pack_reads),.pack_writes(pack_writes),
    .baseboard_debug(baseboard_debug), .processor_debug(processor_debug),
`ifdef DORADO_CONTROL_ERROR_MONITOR
    .control_debug(control_debug),
`endif
`ifdef DORADO_ETHERNET_DDR
    .ddram_busy(1'b1),.ddram_rvalid(1'b0),.ddram_rdata(64'd0),
    .ddram_burstcount(),.ddram_be(),.ddram_addr(),
    .ddram_read(),.ddram_write(),.ddram_wdata(),
`else
    .ddram_busy(DDRAM_BUSY), .ddram_rvalid(DDRAM_DOUT_READY), .ddram_rdata(DDRAM_DOUT),
    .ddram_burstcount(DDRAM_BURSTCNT), .ddram_be(DDRAM_BE), .ddram_addr(DDRAM_ADDR),
    .ddram_read(DDRAM_RD), .ddram_write(DDRAM_WE), .ddram_wdata(DDRAM_DIN),
`endif
`else
dorado_checkpoint_machine u_checkpoint (
`endif
    .sys_clk(machine_clk),
`ifdef DORADO_NATIVE
    .ps2_key(native_key),
`else
    .ps2_key(ps2_key),
`endif
    .pixel_clk(pixel_clk),
`ifdef DORADO_ETHERNET_HOST_PORT
    .ethernet_host(status[2:1]==0 ? 8'o042 : status[2:1]==1 ? 8'o001 :
                   status[2:1]==2 ? 8'o377 : 8'o000),
`endif
    .mouse_keys(mouse_keys), .mouse_dx(mouse_dx), .mouse_dy(mouse_dy),
    .mouse_pending(mouse_pending), .mouse_taken(mouse_taken),
    .mouse_sent_dx(mouse_sent_dx), .mouse_sent_dy(mouse_sent_dy),
`ifdef DORADO_ETHERNET
    .ethernet_rx(ethernet_rx), .ethernet_tx_n(ethernet_tx_n),
`endif
`ifdef DORADO_HPS_PACK
    .clk_host(clk_sys),
`ifndef DORADO_NATIVE
    .pack_reset(~pll_locked),
`endif
    .pack_attached(pack_attached), .pack_readonly(img_readonly), .pack_mounted(img_mounted[0]),
    .sd_lba(sd_lba[0]), .sd_blk_cnt(sd_blk_cnt[0]), .sd_rd(sd_rd[0]), .sd_wr(sd_wr[0]), .sd_ack(sd_ack[0]),
    .sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_din(sd_buff_din[0]), .sd_buff_wr(sd_buff_wr),
    .pack_ready(pack_ready), .pack_fault(pack_fault), .pack_dirty(pack_dirty),
`ifdef DORADO_PACK_FAULT_MONITOR
    .pack_fault_debug(pack_fault_debug), .pack_fault_extra(pack_fault_extra),
`endif
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
`ifdef DORADO_NATIVE
assign LED_USER = reset | pack_fault | native_key_overflow;
`else
assign LED_USER = reset | pack_fault;
`endif
`else
assign LED_USER = reset;
`endif

endmodule
