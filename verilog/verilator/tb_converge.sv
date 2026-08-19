// tb_converge -- the assembled eleven-board machine settles, and its clock
// runs, under Verilator's EVENT SCHEDULER.
//
// This is the control for `machine-test`, and the pair is the whole finding.
// The same `dorado_machine` that settles here for 20,000 cycles does NOT
// settle under the C++ `eval()` model the imgui harness uses -- and raising
// `--converge-limit` to 2000 does not help, so it is an oscillation rather
// than slow settling. The boards are therefore not obviously at fault; the
// difference is the evaluation model, which is where to start looking.
//
// It also builds in about five seconds against the harness's several minutes
// of imgui C++, so it is the one to iterate on.
//
// `loop-check` still passes, so the candidates are the four structural loops
// it knows about and accepts -- the processor's multiplexer chain feeding
// back, and a counter's carry returning to its own count enable on DispM,
// DispY and MemD. See docs/verilog-handoff.md, "the machine did not settle".

`default_nettype none

module tb_converge;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  wire [31:0] pv;
  wire [15:0] pw;

  // `probe_sel` is swept the way the harness sweeps it, so that is not the
  // difference either -- it was tried.
  reg [15:0] psel = 16'd0;

  dorado_machine m (
      .sys_clk    (sys_clk),
      .probe_sel  (psel),
      .probe_val  (pv),
      .probe_words(pw)
  );

  localparam integer RUN = 20000;
  integer i, n;
  reg prev;

  initial begin
    n = 0;
    prev = m.u_machine.CLK_ph_p_;
    for (i = 0; i < RUN; i = i + 1) begin
      @(posedge sys_clk);
      psel = psel + 16'd1;
      if (psel > 16'd9) psel = 16'd0;
      if (m.u_machine.CLK_ph_p_ !== prev) begin
        n = n + 1;
        prev = m.u_machine.CLK_ph_p_;
      end
    end
    // Reaching here at all is the settling result -- a machine that does not
    // converge aborts rather than finishing. The clock count is the second
    // half: the BaseBoard generates it and fans it out, and if it stops
    // nothing downstream can compute.
    $display("tb_converge: %0d cycles settled, CLK.ph' toggled %0d times", RUN, n);
    if (n < RUN / 16)
      $fatal(1, "the BaseBoard's clock is not reaching the slots");
    $display("tb_converge: the eleven-board machine settles and its clock runs.");
    $finish;
  end

endmodule

`default_nettype wire
