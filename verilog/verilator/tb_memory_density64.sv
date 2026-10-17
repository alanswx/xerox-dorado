// Independent high-row/high-column address retention in both RAM cells.
`timescale 1ns/1ps
module tb_memory_density64;
 reg clk=0;always #1 clk=~clk;
 reg ras=1,cas=1,we=1,data=0,high_bit=0;
 wire map_q,storage_q;wire [15:0]address;
 cell_MosRam #(.ADDR_BITS(16)) map_ram(.sys_clk(clk),.p2(data),.p3(we),.p4(ras),.p15(cas),.p9(high_bit),.p14(map_q),.p10(1'b0),.p11(1'b0),.p12(1'b0),.p6(1'b0),.p7(1'b0),.p5(1'b0),.p13(1'b0));
 cell_MK4096P_addr #(.ROW_BITS(8)) front(.sys_clk(clk),.p4(ras),.p15(cas),.a({high_bit,7'b0}),.addr(address));
 cell_MK4096P_6 #(.ADDR_BITS(16)) storage(.sys_clk(clk),.p2(data),.p3(we),.p4(ras),.p15(cas),.addr_i(address),.p14(storage_q));
 task ticks(input integer n);repeat(n)@(negedge clk);endtask
 task access(input integer quadrant,input bit write_mode,input bit value);
  ras=1;cas=1;we=1;ticks(4);
  high_bit=quadrant[1];ras=0;ticks(4);
  high_bit=quadrant[0];data=value;we=!write_mode;cas=0;ticks(12);
  if(!write_mode&&(map_q!==value||storage_q!==value))$fatal(1,"64K quadrant%0d: map%h storage%h expected%b",quadrant,map_q,storage_q,value);
  ras=1;cas=1;we=1;ticks(4);
 endtask
 initial begin
  ticks(10);
  for(integer q=0;q<4;q++)access(q,1,q[0]^q[1]);
  for(integer q=0;q<4;q++)access(q,0,q[0]^q[1]);
  $display("DENSITY64 PASS: row/column high bits retain four distinct map/storage addresses");$finish;
 end
endmodule
