// Rev Am Basebd01.sil: d08 10 MHz -> e07 reload1011 (/5) -> g08 (/2).
// Count external package outputs against the manual's60ns CPU instruction
// and1MHz MCPreClk. No 6502/processor state is initialized by this fixture.
`timescale 1ns/1ps
module baseboard_crystal_case #(parameter BOARD_SYSPER=4, MODE=0)
  (input wire clk, output reg done=0);
  localparam CRYSTAL_SYSPER=(MODE==1)?0:(MODE==2)?BOARD_SYSPER:2*BOARD_SYSPER;
  localparam RELOAD=(MODE==4)?0:11;
  wire xtal, max_count, load_n, mcpre;
  reg clear_n=0;
  cell_MC12061 #(.SYSPER(CRYSTAL_SYSPER),.FREQ_KHZ((MODE==3)?20000:10000)) d08
    (.sys_clk(clk),.p1(1'b1),.p2(),.p3(),.p4(1'b0),.p5(1'b0),.p6(1'b0),
     .p7(1'b1),.p8(1'b0),.p9(1'b0),.p10(xtal),.p11(1'b1),
     .p14(1'b0),.p15(1'b0),.p16(1'b1));
  cell_SN74LS04 c08(.p1(max_count),.p2(load_n),.p3(1'b0),.p4(),
    .p5(1'b0),.p6(),.p7(1'b0),.p8(),.p9(1'b0),.p10(),.p11(1'b0),
    .p12(),.p13(1'b0),.p14(1'b1));
  cell_SN74LS163 e07(.sys_clk(clk),.p1(clear_n),.p2(xtal),
    .p3(1'(RELOAD)),.p4(1'(RELOAD>>1)),.p5(1'(RELOAD>>2)),.p6(1'(RELOAD>>3)),
    .p7(1'b1),.p8(1'b0),.p9(load_n),.p10(1'b1),.p15(max_count),.p16(1'b1));
  cell_SN74LS74 g08(.sys_clk(clk),.p1(clear_n),.p2(mcpre),.p3(max_count),
    .p4(1'b1),.p5(),.p6(mcpre),.p7(1'b0),.p8(),.p10(1'b1),
    .p11(1'b0),.p12(1'b0),.p13(clear_n),.p14(1'b1));

  // Hold the unchanged default to its original waveform independently of
  // the scaled case and of the board-frequency acceptance criterion.
  wire legacy;
  cell_MC12061 old_d08(.sys_clk(clk),.p1(1'b1),.p2(),.p3(),.p4(1'b0),
    .p5(1'b0),.p6(1'b0),.p7(1'b1),.p8(1'b0),.p9(1'b0),.p10(legacy),
    .p11(1'b1),.p14(1'b0),.p15(1'b0),.p16(1'b1));
  reg [2:0] legacy_reference=0;
  always @(posedge clk) legacy_reference<=legacy_reference+3'd1;
  always @(negedge clk) if(legacy!==legacy_reference[2])
    $fatal(1,"Legacy default waveform changed");

  integer n,rises=0,xtal_rises=0;
  reg previous,previous_xtal;
  initial begin
    // Normal package clear pins establish a phase for a standalone TTL
    // fixture. Measure only after the counter has reached its reload loop.
    repeat(64) @(negedge clk);clear_n=1;
    repeat(4096) @(negedge clk);
    previous=mcpre;previous_xtal=xtal;
    for(n=0;n<2*BOARD_SYSPER*10000;n=n+1)begin
      @(negedge clk);
      if(mcpre && !previous)rises=rises+1;
      if(xtal && !previous_xtal)xtal_rises=xtal_rises+1;
      previous=mcpre;previous_xtal=xtal;
    end
    // 10,000 ordinary instructions*60ns=600us:600 MC clocks,6000 crystals.
    $display("BASEBOARD_CRYSTAL sysper=%0d instructions=10000 xtal=%0d mcpre=%0d mode=%0d",
      BOARD_SYSPER,xtal_rises,rises,MODE);
    if(rises<599 || rises>601 || xtal_rises<5999 || xtal_rises>6001)
      $fatal(1,"Original10MHz crystal/1MHz BaseBoard ratio violated");
    done=1;
  end
endmodule

module tb_baseboard_crystal #(parameter MODE=0);
  reg clk=0;always #1 clk=~clk;
  wire [3:0] done;
  baseboard_crystal_case #(.BOARD_SYSPER(2),.MODE(MODE)) a(clk,done[0]);
  baseboard_crystal_case #(.BOARD_SYSPER(4),.MODE(MODE)) b(clk,done[1]);
  baseboard_crystal_case #(.BOARD_SYSPER(8),.MODE(MODE)) c(clk,done[2]);
  baseboard_crystal_case #(.BOARD_SYSPER(16),.MODE(MODE)) d(clk,done[3]);
  initial begin
    wait(&done);$display("PASS_BASEBOARD_CRYSTAL_RATIO_AND_LEGACY_DEFAULT");$finish;
  end
endmodule
