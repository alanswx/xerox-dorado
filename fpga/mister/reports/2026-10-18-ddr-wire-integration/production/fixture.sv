`define DORADO_ETHERNET_DDR
module dorado_ddr_wire_fixture(
 input wire clk_sys,pll_locked,machine_enable,ethernet_tx_n,
 output wire ethernet_rx,
 input wire io_enable,io_strobe,input wire [15:0] io_din,
 output wire [15:0] io_dout,output wire io_selected,
 input wire DDRAM_BUSY,DDRAM_DOUT_READY,input wire [63:0] DDRAM_DOUT,
 output wire [28:0] DDRAM_ADDR,output wire DDRAM_RD,DDRAM_WE,
 output wire [7:0] DDRAM_BURSTCNT,DDRAM_BE,output wire [63:0] DDRAM_DIN,
 output wire stopped,initialized,owned,enabled,rx_ready,tx_valid,wire_step,
 output wire [31:0] fault,errors,good,bad,dropped,rx_bad,
 output wire [63:0] epoch
);
localparam MACHINE_SYSPER=4;
wire [35:0] ethernet_ext_bus;
assign ethernet_ext_bus[35:33]={1'b0,io_enable,io_strobe};
assign ethernet_ext_bus[31:16]=io_din;
assign io_dout=ethernet_ext_bus[15:0];assign io_selected=ethernet_ext_bus[32];
wire net_rx_ready, net_tx_valid;
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
    .machine_debug(pack_fault ? pack_fault_debug : speed_monitor_active ? count_debug : native_key_overflow ?
        {16'h4b51,7'd0,native_key_queued} :
        (processor_debug[31] ? baseboard_debug : receive_debug)), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(speed_monitor_active ? count_debug : receive_debug), .machine_status(machine_status),
    .processor_debug(speed_monitor_active ? count_reference : processor_debug),
`endif
`else
`ifdef DORADO_PACK_FAULT_MONITOR
    .machine_debug(pack_fault ? pack_fault_debug : speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(speed_monitor_active ? count_reference : processor_debug),
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


assign stopped=net_ddr_stopped;assign initialized=net_ddr_initialized;
assign owned=net_ddr_owned;assign enabled=net_ddr_enable;
assign fault=net_ddr_fault;assign errors=net_ddr_control_errors;
assign epoch=net_ddr_epoch;assign rx_ready=net_rx_ready;assign tx_valid=net_tx_valid;
assign good=net_tx_good;assign bad=net_tx_bad;assign dropped=net_tx_dropped;
assign rx_bad=net_rx_bad;assign wire_step=network_step;
endmodule
