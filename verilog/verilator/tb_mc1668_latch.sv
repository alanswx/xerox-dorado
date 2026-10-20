`timescale 1ns/1ps
module tb_mc1668_latch;
 reg sys_clk=0;
 always #5 sys_clk=~sys_clk;
 reg [3:0] a=0,b=0; // C,S,R,D in Motorola's truth-table order
 wire qa,qan,qb,qbn;
 cell_MC1668 dut(.sys_clk(sys_clk),.p7(a[3]),.p5(a[2]),.p4(a[1]),.p6(a[0]),
   .p9(b[3]),.p12(b[2]),.p13(b[1]),.p11(b[0]),.p2(qa),.p3(qan),.p15(qb),.p14(qbn));
 reg expected_a=0,expected_b=0;
 integer checks=0,open_changes=0,closed_checks=0,clock_priority=0,independent=0;
 function automatic truth(input [3:0] row,input old_q);
   // Transcription of manufacturer table. Closed S=R=1 is unspecified.
   casez(row)
     4'b1??0:truth=0;
     4'b1??1:truth=1;
     4'b010?:truth=1;
     4'b001?:truth=0;
     4'b000?:truth=old_q;
     default:truth=1'bx;
   endcase
 endfunction
 task automatic drive(input [3:0] next_a,next_b);
   @(negedge sys_clk);
   if(a[3]&&next_a[3]&&a[0]!=next_a[0]) open_changes=open_changes+1;
   if(b[3]&&next_b[3]&&b[0]!=next_b[0]) open_changes=open_changes+1;
   a=next_a;b=next_b;
   expected_a=truth(a,expected_a);expected_b=truth(b,expected_b);
   @(posedge sys_clk);#1;
   if({qb,qbn,qa,qan}!=={expected_b,~expected_b,expected_a,~expected_a})
     $fatal(1,"MC1668 truth table mismatch A=%b B=%b actual=%b%b expected=%b%b",a,b,qb,qa,expected_b,expected_a);
   checks=checks+1;
   if(!a[3]&&!a[2:1])closed_checks=closed_checks+1;
   if((a[3]&&a[2:1])||(b[3]&&b[2:1]))clock_priority=clock_priority+1;
   if(a[3]!=b[3])independent=independent+1;
 endtask
 initial begin
   drive(4'b0010,4'b0010); // defined reset before any observations of hold
   for(integer pass=0;pass<4;pass=pass+1)
     for(integer first=0;first<16;first=first+1)
       for(integer second=0;second<16;second=second+1)
         if(!(first<8&&((first>>1)&3)==3)&&!(second<8&&((second>>1)&3)==3)) begin
           drive(4'(first),4'(second));
           // Change D under an unchanged high C. The old edge-only cell
           // cannot follow this sequence, even when its first sample agrees.
           if(first>=8||second>=8)drive(4'(first)^4'b0001,4'(second)^4'b0001);
         end
   if(open_changes<100||closed_checks<10||clock_priority<100||independent<100)$fatal(1,"MC1668 incomplete truth-table coverage");
   $display("MC1668_LATCH PASS checks=%0d open_changes=%0d closed=%0d clock_priority=%0d independent=%0d",checks,open_changes,closed_checks,clock_priority,independent);
   $finish;
 end
endmodule
