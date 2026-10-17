// MemD03: j18b's buffered SpecialClk2 must preserve setup at g22.
// Reproduces the cache store lost at sample146495496 after a hold.
`timescale 1ns/1ps
`default_nettype none
module tb_memory_write_clock;
  localparam integer S = `SYSPER_OVERRIDE;
  reg sys_clk = 0;
  always #1 sys_clk = ~sys_clk;
  integer cycles = 0;
  reg board_clock = 1;
  always @(posedge sys_clk) begin
    cycles <= cycles + 1;
    if ((cycles % S) == S - 1) board_clock <= ~board_clock;
  end
  reg fast_store = 0;
  MemD_m_Rev_m_Ca #(.SYSPER(S)) m (
    .sys_clk(sys_clk), .CLK_md_p_(board_clock), .MemSH(1'b0),
    .FastD_u_Dbuf(fast_store), .MakeMD_u_D_p_(1'b1)
  );
  integer writes0 = 0, writes1 = 0;
  reg previous0 = 0, previous1 = 0;
  always @(posedge sys_clk) begin
    if (!m.WriteD0_p_a && !previous0) writes0 <= writes0 + 1;
    if (!m.WriteD1_p_c && !previous1) writes1 <= writes1 + 1;
    previous0 <= !m.WriteD0_p_a;
    previous1 <= !m.WriteD1_p_c;
  end
  initial begin
    repeat (100*S) @(negedge sys_clk);
    if (m.u_g22.qa !== 0 || m.u_g22.qb !== 0)
      $fatal(1, "unexpected cache write before stimulus");
    // The hit/FastD path arrives just after the unbuffered clock input.
    // The schematic gives the write-enable register the buffered output.
    @(posedge m.preClk2);
    @(negedge sys_clk); fast_store = 1;
    repeat (S) @(negedge sys_clk);
    fast_store = 0;
    repeat (4*S) @(negedge sys_clk);
    if (writes0 != 1 || writes1 != 1)
      $fatal(1, "late store lost: D0/D1 pulses %0d/%0d", writes0, writes1);
    // A steady input held through the next sampling point must also
    // produce one pulse. Hold for two full clock intervals: starting at
    // the falling pre-clock edge must not end before the buffered edge.
    @(negedge m.preClk2); fast_store = 1;
    repeat (2*S) @(negedge sys_clk);
    fast_store = 0;
    repeat (4*S) @(negedge sys_clk);
    if (writes0 != 2 || writes1 != 2)
      $fatal(1, "steady store changed: D0/D1 pulses %0d/%0d", writes0, writes1);
    // A transient old-hit request clears when the new address misses.
    // It must not write the old column. The held store is retried later.
    @(negedge m.preClk2);
    repeat (S-1) @(negedge sys_clk);
    fast_store = 1;
    @(posedge m.preClk2);
    @(negedge sys_clk); fast_store = 0;
    repeat (4*S) @(negedge sys_clk);
    if (writes0 != 2 || writes1 != 2)
      $fatal(1, "stale hit pulse wrote the old column: D0/D1 pulses %0d/%0d", writes0, writes1);
    $display("tb_memory_write_clock: PASS -- late/steady stores retained, stale hit pulse rejected");
    $finish;
  end
endmodule
`default_nettype wire
