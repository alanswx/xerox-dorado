// MemD b07 distributes preClk1'A to ordinary clk1 and to Fclk via the
// F<-D gate. With that gate enabled these outputs have the same phase.
`timescale 1ns/1ps
module tb_ifu_f_clock;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg preclock=0, next_preclock=0, checking=0;
  always @(posedge sys_clk) preclock<=next_preclock;
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk),.CLK_md_p_(preclock),
    .MemSH(1'b1),.MemClkEnable_p_a(1'b0));
  integer edges=0;
  reg previous_f=0;
  always @(negedge sys_clk) begin
    if(checking && d.Fclk_p_a!=previous_f) edges=edges+1;
    previous_f=d.Fclk_p_a;
    if(checking && (d.Fclk_p_a!==d.clk1_p_Aa || d.Fclk_p_b!==d.clk1_p_Ac))
      $fatal(1,"F-register clock split from sibling clk1 taps: Fclk=%b%b clk1=%b%b",
        d.Fclk_p_a,d.Fclk_p_b,d.clk1_p_Aa,d.clk1_p_Ac);
  end
  initial begin
    force d.F_u_D_p_=0;
    repeat(20) @(negedge sys_clk);
    next_preclock=1; repeat(8) @(negedge sys_clk);
    next_preclock=0; repeat(8) @(negedge sys_clk);
    checking=1;
    repeat(8) begin
      @(negedge sys_clk); next_preclock=!next_preclock;
      repeat(5) @(negedge sys_clk);
    end
    repeat(5) @(negedge sys_clk);
    if(edges<8) $fatal(1,"clock stimulus did not reach F registers: %0d transitions",edges);
    $display("tb_ifu_f_clock: PASS -- enabled F clocks track sibling clk1 outputs of MemD b07");
    $finish;
  end
endmodule
