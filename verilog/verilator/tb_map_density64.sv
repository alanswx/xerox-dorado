// Map geometry only: original MemX address translators and density straps.
// RAS/CAS/WE and data are component stimuli; full Map protocol is separate.
`timescale 1ns/1ps
module tb_map_density64;
 reg clk=0;always #1 clk=~clk;
 reg ras=1,cas=1,we=1,high_bit=0;
 reg [20:0]data=0;wire [20:0]readback;
 MemX_m_Rev_m_Ch #(.SYSPER(4)) m(.sys_clk(clk),.MapAd_1(high_bit),.MapAd_0(1'b0),.MapAd_2(1'b0),.MapAd_3(1'b0),.MapAd_4(1'b0),.MapAd_5(1'b0),.MapAd_6(1'b0),.MapAd_7(1'b0),.MapAd_8(1'b0));
 initial begin
 force m.RTMapRAS_p_a=ras;force m.RTMapRAS_p_b=ras;
 force m.RTMapCAS_p_a=cas;force m.RTMapCAS_p_b=cas;
 force m.RTMapWE_p_a=we;force m.RTMapWE_p_b=we;
 force m.TDirtyWE_p_=we;force m.TRefWE_p_=we;
 force m.u_a04.p2=data[20];
 force m.u_a05.p2=data[19];
 force m.u_a06.p2=data[18];
 force m.u_a07.p2=data[17];
 force m.u_a08.p2=data[16];
 force m.u_a09.p2=data[15];
 force m.u_a10.p2=data[14];
 force m.u_a11.p2=data[13];
 force m.u_a12.p2=data[12];
 force m.u_a13.p2=data[11];
 force m.u_a14.p2=data[10];
 force m.u_d04.p2=data[9];
 force m.u_d05.p2=data[8];
 force m.u_d06.p2=data[7];
 force m.u_d07.p2=data[6];
 force m.u_d08.p2=data[5];
 force m.u_d09.p2=data[4];
 force m.u_d10.p2=data[3];
 force m.u_d11.p2=data[2];
 force m.u_d12.p2=data[1];
 force m.u_d13.p2=data[0];
 end
 assign readback={m.u_a04.p14,m.u_a05.p14,m.u_a06.p14,m.u_a07.p14,m.u_a08.p14,m.u_a09.p14,m.u_a10.p14,m.u_a11.p14,m.u_a12.p14,m.u_a13.p14,m.u_a14.p14,m.u_d04.p14,m.u_d05.p14,m.u_d06.p14,m.u_d07.p14,m.u_d08.p14,m.u_d09.p14,m.u_d10.p14,m.u_d11.p14,m.u_d12.p14,m.u_d13.p14};
 function automatic [20:0]pattern(input integer q);
 case(q)0:return 21'h100001;1:return 21'h012345;2:return 21'h155aaa;default:return 21'h00baad;endcase
 endfunction
 task ticks(input integer n);repeat(n)@(negedge clk);endtask
 task access(input integer q,input bit write_mode);
  ras=1;cas=1;we=1;ticks(8);
  high_bit=q[1];ticks(4);ras=0;ticks(8);
  high_bit=q[0];data=pattern(q);ticks(4);we=!write_mode;cas=0;ticks(16);
  if(!write_mode&&readback!==pattern(q))$fatal(1,"MemX64 quadrant%0d actual%h expected%h",q,readback,pattern(q));
  ras=1;cas=1;we=1;ticks(8);
 endtask
 initial begin
  ticks(20);
  for(integer q=0;q<4;q++)access(q,1);
  for(integer q=0;q<4;q++)access(q,0);
  $display("MAP64 BOARD PASS: actual MemX address/strap paths retain four distinct21-bit entries");$finish;
 end
endmodule
