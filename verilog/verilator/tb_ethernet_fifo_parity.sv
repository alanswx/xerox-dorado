// Ether07: stored odd byte parity must not set the transmit error latch.
`timescale 1ns/1ps
module tb_ethernet_fifo_parity;
reg sys_clk=0; always #1 sys_clk=~sys_clk;
reg [15:0] data_word=0;
reg [1:0] parity=0;
reg read_fifo=0, tx_off=1, board_clock=0;
DskEth_m_Rev_m_Cf d (.sys_clk(sys_clk));
task tick_board;
 begin
  board_clock=0; repeat(3) @(negedge sys_clk);
  board_clock=1; repeat(3) @(negedge sys_clk);
  board_clock=0; repeat(3) @(negedge sys_clk);
 end
endtask
initial begin
 force d.TxFifo_00=data_word[15]; force d.TxFifo_01=data_word[14];
 force d.TxFifo_02=data_word[13]; force d.TxFifo_03=data_word[12];
 force d.TxFifo_04=data_word[11]; force d.TxFifo_05=data_word[10];
 force d.TxFifo_06=data_word[9];  force d.TxFifo_07=data_word[8];
 force d.TxFifo_08=data_word[7];  force d.TxFifo_09=data_word[6];
 force d.TxFifo_10=data_word[5];  force d.TxFifo_11=data_word[4];
 force d.TxFifo_12=data_word[3];  force d.TxFifo_13=data_word[2];
 force d.TxFifo_14=data_word[1];  force d.TxFifo_15=data_word[0];
 force d.TxFifo_16=parity[1]; force d.TxFifo_17=parity[0];
 force d.ECLTrueD=1;
 force d.TxReadFifo=read_fifo;
 force d.TxOff=tx_off;
 force d.Clock1_p_Dd=board_clock;
 for(integer b=0;b<256;b=b+1) begin
  data_word={8'(b),8'(255-b)};
  parity={~^data_word[15:8],~^data_word[7:0]};
  tx_off=1; tick_board(); tx_off=0; read_fifo=1; tick_board();
  if(d.Ether07_sil_pl_10!==0 || d.TxFifoPE!==0)
   $fatal(1,"valid Ethernet FIFO word %h parity=%b rejected: checker=%b latch=%b",
          data_word,parity,d.Ether07_sil_pl_10,d.TxFifoPE);
  for(integer p=0;p<2;p=p+1) begin
   parity[p]=~parity[p];
   // An idle corrupt FIFO output must not latch an error until consumed.
   tx_off=1; read_fifo=0; tick_board(); tx_off=0; tick_board();
   if(d.TxFifoPE!==0) $fatal(1,"unconsumed word set TxFifoPE");
   read_fifo=1; tick_board();
   if(d.TxFifoPE!==1) $fatal(1,"bad Ethernet FIFO byte parity accepted");
   parity[p]=~parity[p];
  end
 end
 $display("tb_ethernet_fifo_parity: PASS -- valid bytes transmit; corrupt bytes latch TxFifoPE only on read");
 $finish;
end
endmodule
