`timescale 1ns/1ps
module tb_ethernet_ddr_uio;
  reg clk=0;always #1 clk=~clk;
  reg reset=1,enable=0,strobe=0;
  reg [15:0] din=0;
  wire [35:0] bus;
  assign bus[35:33]={1'b0,enable,strobe};assign bus[31:16]=din;
  reg stopped=1,initialized=0;
  reg [31:0] fault=0;
  wire ddr_enable,owned,tx_ack,rx_wr,rx_commit;
  wire [63:0] epoch;
  wire [31:0] errors,aborted;
  integer acks=0,writes=0,commits=0;
  always @(posedge clk)begin
    if(tx_ack)acks<=acks+1;
    if(rx_wr)writes<=writes+1;
    if(rx_commit)commits<=commits+1;
  end
  dorado_ethernet_uio #(.MAX_WORDS(1024),.DDR_CAPABLE(1)) dut(
    .clk(clk),.reset(reset),.bus(bus),.rx_ready(1'b1),.tx_valid(1'b1),
    .tx_words(11'd2),.tx_data(16'h1357),.tx_good(0),.tx_bad(0),.tx_dropped(0),.rx_bad(0),
    .machine_debug(0),.processor_debug(0),.machine_status(16'd0),
    .tx_addr(),.rx_addr(),.tx_ack(tx_ack),.rx_wr(rx_wr),.rx_commit(rx_commit),
    .rx_data(),.rx_words(),.aborted(aborted),
    .ddr_stopped(stopped),.ddr_initialized(initialized),.ddr_fault(fault),
    .ddr_enable(ddr_enable),.ddr_epoch(epoch),.ddr_control_errors(errors),.ddr_owned(owned));
  task tick;@(negedge clk);endtask
  task word(input [15:0] w,output [15:0] response);
    din=w;strobe=1;tick();strobe=0;tick();response=bus[15:0];
  endtask
  task deselect;enable=0;repeat(3)tick();endtask
  reg [15:0] r;
  localparam [63:0] FIRST=64'h1234abcd89ef5678,SECOND=64'hfedc9876543210ab;
  task control(input [15:0] mode,input [63:0] value);
    enable=1;word(16'h75,r);if(r!==16'hd0d4 || !bus[32])$fatal(1,"DDR control capability");
    word(mode,r);word(value[15:0],r);word(value[31:16],r);word(value[47:32],r);word(value[63:48],r);
  endtask
  initial begin
    repeat(3)tick();reset=0;tick();
    if(ddr_enable || epoch || errors || owned)$fatal(1,"not safely disabled at power-up");
    enable=1;word(16'h74,r);if(r!==16'hd0d4 || !bus[32])$fatal(1,"DDR capability missing");
    word(0,r);if(r!==1)$fatal(1,"version");
    word(0,r);if(r!==0)$fatal(1,"base low");word(0,r);if(r!==16'h1ff0)$fatal(1,"base high");
    word(0,r);if(r!==0)$fatal(1,"size low");word(0,r);if(r!==1)$fatal(1,"size high");
    word(0,r);if(r!==1)$fatal(1,"initial flags");deselect();
    for(integer n=1;n<6;n++)begin
      enable=1;word(16'h75,r);
      for(integer i=1;i<n;i++)word(1,r);
      deselect();
      if(ddr_enable || epoch || errors!==n)$fatal(1,"partial command changed control: %0d",n);
    end
    control(1,FIRST);word(0,r);deselect();
    if(ddr_enable || epoch || errors!==6)$fatal(1,"overlong control accepted");
    control(2,FIRST);deselect();if(ddr_enable || epoch || errors!==7)$fatal(1,"invalid mode");
    control(1,0);deselect();if(ddr_enable || epoch || errors!==8)$fatal(1,"zero epoch");
    control(1,FIRST);
    if(ddr_enable || epoch)$fatal(1,"control changed before deselection");
    deselect();if(!ddr_enable || epoch!==FIRST || !owned || errors!==8)$fatal(1,"valid start");
    stopped=0;initialized=1;
    enable=1;word(16'h74,r);fault=32'hdeadbeef;stopped=1;
    repeat(5)word(0,r);
    word(0,r);if(r!==16'h16)$fatal(1,"torn DDR state snapshot");
    word(0,r);if(r!==0)$fatal(1,"torn fault low");word(0,r);if(r!==0)$fatal(1,"torn fault high");
    word(0,r);if(r!==FIRST[15:0])$fatal(1,"epoch low");
    word(0,r);if(r!==FIRST[31:16])$fatal(1,"epoch second");
    word(0,r);if(r!==FIRST[47:32])$fatal(1,"epoch third");
    word(0,r);if(r!==FIRST[63:48])$fatal(1,"epoch high");
    word(0,r);if(r!==2048)$fatal(1,"truncated payload capacity");
    word(0,r);if(r!==8)$fatal(1,"TX slots");word(0,r);if(r!==16)$fatal(1,"RX slots");
    word(0,r);if(r!==8)$fatal(1,"control error snapshot");deselect();
    fault=0;stopped=0;
    enable=1;word(16'h70,r);word(0,r);if(r!==12)$fatal(1,"SPI flags exposed during DDR");deselect();
    enable=1;word(16'h71,r);if(r!==0)$fatal(1,"SPI TX length exposed");
    word(0,r);if(r!==0)$fatal(1,"SPI TX data exposed");deselect();
    enable=1;word(16'h72,r);deselect();if(acks)$fatal(1,"SPI ACK stole DDR packet");
    enable=1;word(16'h73,r);if(r!==0)$fatal(1,"SPI RX offered during DDR");
    word(2,r);word(1,r);word(2,r);deselect();
    if(writes || commits || aborted!==1)$fatal(1,"SPI RX modified DDR-owned staging");
    control(1,SECOND);deselect();if(epoch!==FIRST || errors!==9)$fatal(1,"start replaced live owner");
    control(0,0);deselect();if(ddr_enable || !owned || epoch!==FIRST)$fatal(1,"stop switched owner before drain");
    enable=1;word(16'h72,r);deselect();if(acks)$fatal(1,"SPI ACK before DDR drain");
    initialized=0;stopped=1;tick();if(owned)$fatal(1,"SPI owner not released after drain");
    enable=1;word(16'h70,r);word(0,r);if(r!==15)$fatal(1,"fallback flags");deselect();
    control(1,FIRST);deselect();if(ddr_enable || errors!==10)$fatal(1,"stale epoch restarted");
    control(1,SECOND);deselect();if(!ddr_enable || epoch!==SECOND)$fatal(1,"fresh epoch restart");
    enable=1;word(16'h75,r);word(1,r);reset=1;tick();reset=0;deselect();
    if(ddr_enable || epoch || errors)$fatal(1,"reset left a live/partial control");
    enable=1;word(16'h76,r);if(bus[32])$fatal(1,"intercepted another command");deselect();
    $display("DDR-UIO PASS: complete atomic control, coherent capability/epoch/errors, partial/overlong/mode/stale rejection, ownership until drain, SPI isolation/fallback/reset");
    $finish;
  end
endmodule
