// MemD's odd-bank word counters must retain setup address/control when
// clk1 also releases the memory hold and advances MemC's dDad mux.
// Actual boot: request FD25 should select word-pair 2, not stale pair 0.
`timescale 1ns/1ps
module tb_cache_word_address_capture;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg clock_in=0, next_clock=0;
  reg [2:0] address_in=0, next_address=0;
  reg clear=1, dont_load=0;
  always @(posedge sys_clk) begin
    clock_in<=next_clock;
    address_in<=next_address;
  end
  MemD_m_Rev_m_Ca #(.SYSPER(`SYSPER_OVERRIDE)) d
    (.sys_clk(sys_clk), .CLK_md_p_(1'b0));
  task check(input [2:0] expected);
    if(d.u_b09.q[2:0]!==expected || d.u_h07.q[2:0]!==expected ||
       d.u_k09.q[2:0]!==expected)
      $fatal(1,"odd-bank word address: %h/%h/%h expected %h",
        d.u_b09.q[2:0],d.u_h07.q[2:0],d.u_k09.q[2:0],expected);
  endtask
  task load(input [2:0] setup_address, input [2:0] following_address);
    begin
      @(negedge sys_clk); next_clock=0; next_address=setup_address;
      repeat(5) @(negedge sys_clk);
      next_clock=1; next_address=following_address;
      repeat(3) @(negedge sys_clk);
      check(setup_address);
      next_clock=0; repeat(3) @(negedge sys_clk);
    end
  endtask
  initial begin
    force d.clk1_p_Ab=clock_in;
    force d.clk1_p_Ba=clock_in;
    force d.clk1_p_Bb=clock_in;
    force d.dDad_10=address_in[2];
    force d.dDad_11=address_in[1];
    force d.dDad_12=address_in[0];
    force d.ClearWA=clear;
    force d.DontLoad1=dont_load;
    repeat(5) @(negedge sys_clk); clear=0;
    load(3'd2,3'd0);
    load(3'd5,3'd7);
    load(3'd7,3'd1);
    // The same counters serialize a line while parallel loading is off.
    dont_load=1;
    repeat(4) @(negedge sys_clk);
    for(integer i=0;i<8;i=i+1) begin
      next_clock=1; repeat(3) @(negedge sys_clk);
      check(i[2:0]);
      next_clock=0; repeat(3) @(negedge sys_clk);
    end
    $display("tb_cache_word_address_capture: PASS -- setup address and full line count");
    $finish;
  end
endmodule
