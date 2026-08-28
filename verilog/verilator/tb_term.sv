// Does the terminal microcomputer send HM Table 24 messages, and in the
// priority the manual states? Framing, arbitration and the excess-200B mouse
// encoding, checked against dorado/src/display.c's model of the same thing.
`default_nettype none
module tb_term;
  reg sys_clk = 0; always #1 sys_clk = ~sys_clk;
  reg reset = 1, line_tick = 0;
  reg [15:0] w0=16'hFFFF, w1=16'hFFFF, w2=16'hFFFF, w3=16'hFFFF, w4=16'hFFFF;
  reg signed [8:0] mdx=0, mdy=0; reg mpend=0;
  wire mtaken; wire signed [8:0] msdx, msdy;
  wire ois, ois_n;

  dorado_terminal u (.sys_clk(sys_clk), .reset(reset), .line_tick(line_tick),
    .key_word0(w0), .key_word1(w1), .key_word2(w2), .key_word3(w3), .key_word4(w4),
    .mouse_dx(mdx), .mouse_dy(mdy), .mouse_pending(mpend),
    .mouse_taken(mtaken), .mouse_sent_dx(msdx), .mouse_sent_dy(msdy),
    .OISData(ois), .OISData_n(ois_n));

  integer i, bad = 0; reg [31:0] got;
  // Shift 32 bits out, MSB first, one per line_tick -- exactly how DispY's
  // c22 samples it (clocked by RamdHBlank).
  // RACE-FREE, and it has to be said explicitly. The first version drove
  // `line_tick` with a NON-BLOCKING assign and sampled `ois` on the same
  // posedge that consumed it. `ois` is combinational from the message
  // register, so whether the sample sees the bit before or after the tick is
  // scheduler-dependent: Verilator 5.050 gave the right 32 bits and 5.044 gave
  // all of them shifted one place, `(want << 1) | 1`. The RTL was identical.
  // So: move the stimulus to the NEGEDGE, away from the sampling edge, and
  // sample at the posedge, which reads pre-NBA values by definition.
  task grab; begin
    got = 32'd0;
    for (i = 0; i < 32; i = i + 1) begin
      @(negedge sys_clk); line_tick = 1'b1;
      @(posedge sys_clk); got = {got[30:0], ois};
      @(negedge sys_clk); line_tick = 1'b0;
    end
  end endtask

  task chk(input [31:0] want, input [255:0] what); begin
    if (got !== want) begin
      $display("tb_term: FAIL %0s -- got %08x want %08x", what, got, want);
      bad = bad + 1;
    end else $display("tb_term: ok   %0s = %08x", what, got);
  end endtask

  initial begin
    repeat (4) @(posedge sys_clk); reset <= 0; repeat (4) @(posedge sys_clk);
    // IDLE: round-robin refresh, types 1..5, bodies all-ones (nothing pressed).
    grab; chk({1'b1, 7'd1, 16'hFFFF, 1'b1, 7'd0}, "idle refresh word0 (type 1)");
    grab; chk({1'b1, 7'd2, 16'hFFFF, 1'b1, 7'd0}, "idle refresh word1 (type 2)");
    grab; chk({1'b1, 7'd3, 16'hFFFF, 1'b1, 7'd0}, "idle refresh word2 (type 3)");
    grab; chk({1'b1, 7'd4, 16'hFFFF, 1'b1, 7'd0}, "idle refresh word3 (type 4)");
    grab; chk({1'b1, 7'd5, 16'hFFFF, 1'b1, 7'd0}, "idle refresh word4 (type 5)");

    // A SERIAL DEVICE CANNOT ANSWER MID-MESSAGE. The body is latched when a
    // message starts, so a key that changes while one is in flight is
    // reported in the NEXT one -- exactly as dorado/src/display.c does it, and
    // the reason that C header warns against re-reading the body per bit. So
    // each stimulus below is followed by one throwaway grab: the message
    // already on the wire.
    // A KEY GOES DOWN (active low) -- word 2, bit 5.
    w2 = 16'hFFDF; repeat (4) @(posedge sys_clk);
    grab;   // the message already in flight
    grab; chk({1'b1, 7'd3, 16'hFFDF, 1'b1, 7'd0}, "key down reported as type 3");

    // A KEYBOARD TRANSITION OUTRANKS MOUSE MOTION -- the manual's own rule.
    mdx = 9'sd10; mdy = -9'sd5; mpend = 1'b1;
    w1 = 16'hFFFE; repeat (4) @(posedge sys_clk);
    grab;   // in flight
    grab; chk({1'b1, 7'd2, 16'hFFFE, 1'b1, 7'd0}, "keyboard beats mouse");

    // ...and with no transition pending the mouse goes, excess-200B.
    grab; chk({1'b1, 7'd6, 16'h8A7B, 1'b1, 7'd0},
                 "mouse 06B in excess-200B");
    if (msdx !== 9'sd10 || msdy !== -9'sd5) begin
      $display("tb_term: FAIL -- mouse_sent %0d,%0d not 10,-5", msdx, msdy); bad = bad + 1;
    end

    // CLAMPING: a big move is carried in pieces, not wrapped.
    mdx = 9'sd200; mdy = 9'sd0;
    grab;   // in flight
    grab; chk({1'b1, 7'd6, 16'hFF80, 1'b1, 7'd0}, "mouse clamps to +127");

    if (bad) $fatal(1, "tb_term: %0d checks failed", bad);
    $display("tb_term: PASS -- Table 24 framing, priority and excess-200B");
    $finish;
  end
endmodule
`default_nettype wire
