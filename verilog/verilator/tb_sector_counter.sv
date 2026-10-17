// TriconD06.sil: 1.2 us pulses count; a 4.5 us index reloads via a
// 3.0 us SN74123. Test the actual one-shot/LS169 feedback circuit.
`timescale 1ns/1ps
module tb_sector_counter;
  reg sys_clk=0; always #1 sys_clk=~sys_clk;
  reg pulse_n=1;
  wire reload_n, sector_n;
  cell_SN74123 #(.SYSPER(16)) timer (
    .sys_clk(sys_clk),.p1(pulse_n),.p2(1'b1),.p3(sector_n),.p13(reload_n),
    .p9(1'b1),.p10(1'b0),.p11(1'b0));
  cell_SN74LS169 counter (
    .sys_clk(sys_clk),.p1(1'b0),.p2(pulse_n),
    .p3(1'b1),.p4(1'b1),.p5(1'b0),.p6(1'b0),
    .p7(pulse_n),.p9(reload_n),.p10(pulse_n),.p15(sector_n));
  task pulse(input integer ns,input [3:0] expected);
    @(negedge sys_clk); pulse_n=0;
    repeat(ns*16/30) @(negedge sys_clk);
    pulse_n=1; repeat(10) @(negedge sys_clk);
    if(counter.q!==expected)
      $fatal(1,"%0d ns pulse left counter %h, expected %h",ns,counter.q,expected);
    repeat(3000) @(negedge sys_clk);
  endtask
  initial begin
    repeat(10) @(negedge sys_clk);
    pulse(1200,3); // terminal-count feedback loads 3
    pulse(1200,2); // ordinary sector counts down
    pulse(4500,3); // index must reload, even when NOT at terminal count
    pulse(1200,2); pulse(1200,1); pulse(1200,0); pulse(1200,3);
    $display("tb_sector_counter: PASS -- sector counts, index reloads, divide by four");
    $finish;
  end
endmodule
