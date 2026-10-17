// TriconD09/10: check stored FIFO byte parity and regenerate serial parity.
`timescale 1ns/1ps
module tb_disk_fifo_parity;
reg sys_clk=0; always #1 sys_clk=~sys_clk;
reg [15:0] word_=0; reg [1:0] parity=0;
DskEth_m_Rev_m_Cf d (.sys_clk(sys_clk));
initial begin
 force d.ShiftReg_00=word_[15];
 force d.ShiftReg_01=word_[14];
 force d.ShiftReg_02=word_[13];
 force d.ShiftReg_03=word_[12];
 force d.ShiftReg_04=word_[11];
 force d.ShiftReg_05=word_[10];
 force d.ShiftReg_06=word_[9];
 force d.ShiftReg_07=word_[8];
 force d.ShiftReg_08=word_[7];
 force d.ShiftReg_09=word_[6];
 force d.ShiftReg_10=word_[5];
 force d.ShiftReg_11=word_[4];
 force d.ShiftReg_12=word_[3];
 force d.ShiftReg_13=word_[2];
 force d.ShiftReg_14=word_[1];
 force d.ShiftReg_15=word_[0];
 force d.OutPar_16=parity[1]; force d.OutPar_17=parity[0];
 force d.ECLTrueA=1;
 for(integer b=0;b<256;b=b+1) begin
  word_={8'(b),8'(255-b)}; parity={~^word_[15:8],~^word_[7:0]};
  repeat(2) @(negedge sys_clk);
  if(d.dFifoParityErr!==0) $fatal(1,"valid FIFO word %h parity=%b rejected",word_,parity);
  if({d.ShiftReg_16,d.ShiftReg_17}!==parity) $fatal(1,"serial byte parity generation mismatch");
  for(integer p=0;p<2;p=p+1) begin
   parity[p]=~parity[p]; repeat(2) @(negedge sys_clk);
   if(d.dFifoParityErr!==1) $fatal(1,"bad FIFO byte parity accepted");
   parity[p]=~parity[p];
  end
 end
$display("tb_disk_fifo_parity: PASS -- both FIFO checkers reject corrupt parity and accept all byte values");
$finish;
end
endmodule
