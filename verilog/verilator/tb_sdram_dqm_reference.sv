// Independent receiver-edge and launch-time oracle, checked against a Micron-authored model.
`timescale 1ns/1ps
module tb_sdram_dqm_reference;
reg clk=0;always #10 clk=~clk;
reg [2:0] command=7;
reg [12:0] address=0;
reg driving=0;reg[15:0] value=0;
reg [1:0] dqm=0;
wire[15:0] dq;
assign dq=driving ? value : 16'hzzzz;
`ifdef MICRON_VENDOR
mt48lc16m16a2 #(.addr_bits(13),.col_bits(10),.mem_sizes(1023),.tAC(6.0)) chip(
 .Clk(clk),.Cke(1'b1),.Cs_n(1'b0),.Ras_n(command[2]),.Cas_n(command[1]),
 .We_n(command[0]),.Ba(2'b0),.Addr(address),.Dqm(dqm),.Dq(dq));
`else
as4c32m16sb_6tin_chip_model #(.CHIP_NAME("CAS_REFERENCE"),.tAC_MAX(6.0),
 .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0)) chip(
 .Clk(clk),.Cke(1'b1),.Cs_n(1'b0),.Ras_n(command[2]),.Cas_n(command[1]),
 .We_n(command[0]),.Ba(2'b0),.Addr(address),.Ldqm(dqm[0]),.Udqm(dqm[1]),.Dq(dq));
`endif
int errors=0,checks=0;
function automatic [1:0] mask_at(input integer offset);
 case(offset)
 -2:mask_at=3;
 -1:mask_at=1;
 0:mask_at=0;
 1:mask_at=2;
 2:mask_at=1;
 3:mask_at=3;
 4:mask_at=0;
 5:mask_at=1;
 6:mask_at=2;
 default:mask_at=0;
 endcase
endfunction
task automatic issue(input reg[2:0] cmd,input reg[12:0] addr);
 @(negedge clk);command=cmd;address=addr;
 @(negedge clk);command=7;address=0;
endtask
initial begin
 reg [15:0] expected;
 reg [1:0] mask;
 int lane;
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
  // Receiver at k+1 observes data launched at k+tAC. The manufacturer's
  // two receiver-clock DQM latency therefore uses the input sampled at k-1.
  // Changing both lanes around READ distinguishes that from an extra stage.
  for(int k=-2;k<cas+8;k++)begin
   @(negedge clk);command=(k==0) ? 5 : 7;address=0;dqm=mask_at(k);
   @(posedge clk);#7;
   if(k>=cas-1 && k<cas+7)begin
    lane=k-(cas-1);mask=mask_at(k-1);expected=16'(lane^'ha59);
`ifdef MICRON_VENDOR
    if(mask[0])expected[7:0]='z;
    if(mask[1])expected[15:8]='z;
`else
    // Two-state local simulation maps a masked Hi-Z byte to zero.
    if(mask[0])expected[7:0]=0;
    if(mask[1])expected[15:8]=0;
`endif
    checks++;
    $display("DQM_EDGE cas=%0d lane=%0d mask=%b got=%h expected=%h",cas,lane,mask,dq,expected);
    if(dq!==expected)begin errors++;$display("DQM_MISMATCH cas=%0d lane=%0d",cas,lane);end
   end
  end
  @(negedge clk);dqm=0;
  repeat(4)@(negedge clk);
 end
 `ifndef MICRON_VENDOR
 if(chip.error_count || chip.warning_count)$fatal(1,"DQM_REFERENCE protocol warnings/errors");
`endif
 if(errors)$fatal(1,"DQM_REFERENCE mismatch count=%0d",errors);
 $display("DQM_REFERENCE PASS CAS2_AND_CAS3 checks=%0d",checks);$finish;
end
initial begin #500000;$fatal(1,"deadline");end
endmodule
