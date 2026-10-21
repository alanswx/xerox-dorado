// Independent receiver-edge and launch-time oracle, checked against a Micron-authored model.
`timescale 1ns/1ps
module tb_sdram_cas_reference;
reg clk=0;always #10 clk=~clk;
reg [2:0] command=7;
reg [12:0] address=0;
reg driving=0;reg[15:0] value=0;
wire[15:0] dq;
assign dq=driving ? value : 16'hzzzz;
`ifdef MICRON_VENDOR
mt48lc16m16a2 #(.addr_bits(13),.col_bits(10),.mem_sizes(1023),.tAC(6.0)) chip(
 .Clk(clk),.Cke(1'b1),.Cs_n(1'b0),.Ras_n(command[2]),.Cas_n(command[1]),
 .We_n(command[0]),.Ba(2'b0),.Addr(address),.Dqm(2'b0),.Dq(dq));
`else
as4c32m16sb_6tin_chip_model #(.CHIP_NAME("CAS_REFERENCE"),.tAC_MAX(6.0),
 .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0)) chip(
 .Clk(clk),.Cke(1'b1),.Cs_n(1'b0),.Ras_n(command[2]),.Cas_n(command[1]),
 .We_n(command[0]),.Ba(2'b0),.Addr(address),.Ldqm(1'b0),.Udqm(1'b0),.Dq(dq));
`endif
realtime read_time=0,first_time=0;
reg tracking=0;
int errors=0;
always @(dq)if(tracking && dq===16'ha59 && first_time==0)begin
 first_time=$realtime;
 $display("CAS_FIRST_LAUNCH delay_ns=%0.3f",first_time-read_time);
end
int cycles=0;
always @(posedge clk)cycles<=cycles+1;
task automatic issue(input reg[2:0] cmd,input reg[12:0] addr);
 @(negedge clk);command=cmd;address=addr;
 @(negedge clk);command=7;address=0;
endtask
initial begin
 int read_cycle;
 repeat(10002)@(negedge clk);
 issue(2,13'h400);repeat(4)@(negedge clk);
 repeat(8)begin issue(1,0);repeat(6)@(negedge clk);end
 for(int cas=2;cas<=3;cas++)begin
  issue(2,13'h400);repeat(4)@(negedge clk);
  issue(0,13'('h203|(cas<<4)));repeat(4)@(negedge clk);
  issue(3,0);repeat(4)@(negedge clk);
  for(int lane=0;lane<8;lane++)begin
   @(negedge clk);command=4;address=13'(lane);driving=1;value=16'(lane^'ha59);
   @(negedge clk);command=7;driving=0;repeat(2)@(negedge clk);
  end
  @(negedge clk);command=5;address=0;
  @(posedge clk);read_cycle=cycles;read_time=$realtime;first_time=0;tracking=1;
  @(negedge clk);command=7;
  repeat(cas-2)@(negedge clk);
  for(int lane=0;lane<8;lane++)begin
   @(negedge clk);#1;
   if(dq!==16'(lane^'ha59))begin errors++;$display("VENDOR_LATENCY cas=%d lane=%d got=%h expected=%h cycles=%d",cas,lane,dq,16'(lane^'ha59),cycles-read_cycle);end
   if(lane==0)$display("VENDOR_FIRST_WORD cas=%0d command_cycle=%0d sample_cycle=%0d value=%h",cas,read_cycle,cycles,dq);
  end
  tracking=0;
  if(first_time-read_time != (cas-1)*20.0+6.0)begin
   errors++;$display("CAS_LAUNCH_MISMATCH cas=%0d delta=%0.3f",cas,first_time-read_time);
  end
  repeat(4)@(negedge clk);
 end
 `ifndef MICRON_VENDOR
 if(chip.error_count || chip.warning_count)$fatal(1,"CAS_REFERENCE protocol warnings/errors");
`endif
 if(errors)$fatal(1,"CAS_REFERENCE mismatch count=%0d",errors);
 $display("CAS_REFERENCE PASS CAS2_AND_CAS3");$finish;
end
initial begin #500000;$fatal(1,"deadline");end
endmodule
