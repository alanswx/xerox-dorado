// The host-number backplane straps are positive logic, read through the
// real DskEth status multiplexers. Zero makes AEmu's SIONop report no NIC.
`timescale 1ns/1ps
module tb_ethernet_host;
  reg clk=0; always #1 clk=~clk;
  reg [1:0] div=0; always @(posedge clk) div<=div+1'b1;
  reg [7:0] host=0;
  wire [7:0] read_host={d.IOB_00__drv,d.IOB_01__drv,d.IOB_02__drv,d.IOB_03__drv,
                        d.IOB_04__drv,d.IOB_05__drv,d.IOB_06__drv,d.IOB_07__drv};
  DskEth_m_Rev_m_Cf #(.SYSPER(4)) d(
    .sys_clk(clk),.CLK_disk_p_(div[1]),.CLKEnable_p_a(1'b0),.IOReset(1'b0),
    .Host_0(host[7]),.Host_1(host[6]),.Host_2(host[5]),.Host_3(host[4]),
    .Host_4(host[3]),.Host_5(host[2]),.Host_6(host[1]),.Host_7(host[0]),
    .TIOA_0(1'b0),.TIOA_1(1'b0),.TIOA_2(1'b0),.TIOA_3(1'b0),
    .TIOA_4(1'b1),.TIOA_5(1'b1),.TIOA_6(1'b1),.TIOA_7(1'b0));
  initial begin
    repeat(1000) @(negedge clk);
    $display("zero straps: status host=%h",read_host);
    if(read_host!==0)$fatal(1,"zero straps did not read zero");
    host=8'hff;repeat(1000) @(negedge clk);
    $display("377 straps: status host=%h",read_host);
    if(read_host!==8'hff)$fatal(1,"host straps do not reach status");
    host=8'h2a;repeat(1000) @(negedge clk);
    if(read_host!==8'h2a)$fatal(1,"host strap bit order reversed: %h",read_host);
    $display("ETHERNET-HOST PASS: real status mux reads positive host straps 00/ff/2a");
    $finish;
  end
endmodule
