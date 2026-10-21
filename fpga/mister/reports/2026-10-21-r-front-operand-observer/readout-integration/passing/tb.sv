`timescale 1ns/1ps
module tb;
reg clk_sys=0; always #1 clk_sys=~clk_sys;
reg pll_locked=0,enable=0,strobe=0;
reg [15:0] din=0;
wire [35:0] ethernet_ext_bus;
assign ethernet_ext_bus[35:33]={1'b0,enable,strobe};
assign ethernet_ext_bus[31:16]=din;
reg [31:0] status=0;
reg pack_fault=0,speed_monitor_active=0,native_key_overflow=0;
reg [8:0] native_key_queued=0;
reg [31:0] pack_fault_debug=32'hfa170001,pack_fault_extra=32'hfa170002;
reg [31:0] count_debug=32'hc0100001,count_reference=32'hc0100002;
reg [31:0] processor_debug=32'h01234567,baseboard_debug=32'hbb000001,receive_debug=32'haabbccdd;
reg [127:0] pins=128'h11112222333344445555666677778888;
reg [15:0] machine_status=16'h9876;
wire net_rx_ready=1'b1,net_tx_valid=1'b0;
wire [10:0] net_tx_words=0;
wire [15:0] net_tx_data=0;
wire [31:0] net_tx_good=0,net_tx_bad=0,net_tx_dropped=0,net_rx_bad=0;
wire [9:0] net_spi_tx_addr,net_spi_rx_addr;
wire net_spi_tx_ack,net_spi_rx_wr,net_spi_rx_commit;
wire [15:0] net_spi_rx_data;
wire [10:0] net_spi_rx_words;
wire net_ddr_stopped=1'b1,net_ddr_initialized=1'b1;
wire [31:0] net_ddr_fault=0;
wire [31:0] net_ddr_control_errors;
wire net_ddr_enable,net_ddr_owned;
wire [63:0] net_ddr_epoch;
`include "actual-top-readout.svh"
assign native_operand_debug=pins;
task tick; @(negedge clk_sys); endtask
task word(input [15:0] value,output [15:0] reply);
din=value;strobe=1;tick();strobe=0;tick();reply=ethernet_ext_bus[15:0];
endtask
integer checks=0;
reg [15:0] reply;
task observe(input [31:0] expected_machine,input [31:0] expected_cpu);
reg [15:0] saved_status;
begin
saved_status=machine_status; enable=1;word(16'h70,reply);
if(reply!==16'hd074 || !ethernet_ext_bus[32])$fatal(1,"command70 missing");
// Every subsequent word must retain the same command-time observation.
pins=~pins;processor_debug=~processor_debug;status=~status;
pack_fault=~pack_fault;speed_monitor_active=~speed_monitor_active;
count_debug=~count_debug;count_reference=~count_reference;machine_status=~machine_status;
repeat(7)word(0,reply);
word(0,reply);if(reply!==expected_machine[15:0])$fatal(1,"machine low %h expected %h",reply,expected_machine);
word(0,reply);if(reply!==expected_machine[31:16])$fatal(1,"machine high %h expected %h",reply,expected_machine);
word(0,reply);if(reply!==saved_status)$fatal(1,"status not atomic");
word(0,reply);if(reply!==expected_cpu[15:0])$fatal(1,"CPU low %h expected %h",reply,expected_cpu);
word(0,reply);if(reply!==expected_cpu[31:16])$fatal(1,"CPU high %h expected %h",reply,expected_cpu);
enable=0;repeat(3)tick();checks=checks+1;
end endtask
task setup;
begin
pins=128'h11112222333344445555666677778888;processor_debug=32'h01234567;
status=0;pack_fault=0;speed_monitor_active=0;
count_debug=32'hc0100001;count_reference=32'hc0100002;machine_status=16'h9876;
end endtask
initial begin
repeat(3)tick();pll_locked=1;tick();
setup();observe(32'haabbccdd,32'h01234567);
setup();speed_monitor_active=1;observe(32'hc0100001,32'hc0100002);
setup();status=32'h8000;speed_monitor_active=1;observe(32'h11112222,32'h01234567);
setup();status=32'h8800;observe(32'h33334444,32'h01234567);
setup();status=32'h9000;observe(32'h55556666,32'h01234567);
setup();status=32'h9800;observe(32'h77778888,32'h01234567);
for(integer page=4;page<16;page=page+1)begin
setup();status=(16+page)<<11;observe(32'hd0d0bad0,32'h01234567);
end
setup();status=32'hf800;pack_fault=1;speed_monitor_active=1;
observe(32'hfa170001,32'hfa170002);
setup();processor_debug=32'h81234567;observe(32'hbb000001,32'h81234567);
$display("ACTUAL_TOP_OPERAND_UIO_PASS observations=%0d",checks);$finish;
end
endmodule
