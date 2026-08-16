// tb_proms -- do the generated PROMs read back what the machine expects?
//
// This is the cheapest of the cross-checks in docs/verilog-handoff.md, and
// the only one that exercises the WIRING rather than the contents. The .mem
// files come from PARC's BCPL; comparing the RTL against them again would be
// circular. So every check here is against something else:
//
//   * LMASK/RMASK against the CLOSED FORM the Hardware Manual describes and
//     the C emulator implements independently (shifter edge masks).
//   * EtherFifo against the definition of a circular buffer.
//   * EtherRcvr against the state machine's own documented behaviour.
//
// What it really tests is bit order, on both axes at once: A0 is the most
// significant ADDRESS bit and Q0 the most significant OUTPUT bit. Get either
// backwards and these all fail. Run from the repository root -- the INIT_FILE
// paths are repo-relative.

`default_nettype none

module tb_proms;

  integer errors = 0;
  integer checks = 0;

  // NOTE: `want` is 64 bits, so a 1-bit `~x` argument is EXTENDED before it
  // is inverted and becomes all-ones. Use `!x` for a complement here.
  task check(input [63:0] got, input [63:0] want, input string what);
    begin
      checks = checks + 1;
      if (got !== want) begin
        errors = errors + 1;
        if (errors <= 10)
          $display("  FAIL %-34s got %h want %h", what, got, want);
      end
    end
  endtask

  // ---- LMASK / RMASK: 32 x 16, split high byte on ProcH, low on ProcL ----
  reg [4:0] sh;
  wire [7:0] lm_hi, lm_lo, rm_hi, rm_lo;

  cell_SG10139 #(.INIT_FILE("verilog/proms/packages/ProcH-b08.mem")) u_lmh (
    .p14(sh[4]), .p13(sh[3]), .p12(sh[2]), .p11(sh[1]), .p10(sh[0]),
    .p15(1'b0),
    .p1(lm_hi[7]), .p2(lm_hi[6]), .p3(lm_hi[5]), .p4(lm_hi[4]),
    .p5(lm_hi[3]), .p6(lm_hi[2]), .p7(lm_hi[1]), .p9(lm_hi[0]));

  cell_SG10139 #(.INIT_FILE("verilog/proms/packages/ProcL-b08.mem")) u_lml (
    .p14(sh[4]), .p13(sh[3]), .p12(sh[2]), .p11(sh[1]), .p10(sh[0]),
    .p15(1'b0),
    .p1(lm_lo[7]), .p2(lm_lo[6]), .p3(lm_lo[5]), .p4(lm_lo[4]),
    .p5(lm_lo[3]), .p6(lm_lo[2]), .p7(lm_lo[1]), .p9(lm_lo[0]));

  cell_SG10139 #(.INIT_FILE("verilog/proms/packages/ProcH-b07.mem")) u_rmh (
    .p14(sh[4]), .p13(sh[3]), .p12(sh[2]), .p11(sh[1]), .p10(sh[0]),
    .p15(1'b0),
    .p1(rm_hi[7]), .p2(rm_hi[6]), .p3(rm_hi[5]), .p4(rm_hi[4]),
    .p5(rm_hi[3]), .p6(rm_hi[2]), .p7(rm_hi[1]), .p9(rm_hi[0]));

  cell_SG10139 #(.INIT_FILE("verilog/proms/packages/ProcL-b07.mem")) u_rml (
    .p14(sh[4]), .p13(sh[3]), .p12(sh[2]), .p11(sh[1]), .p10(sh[0]),
    .p15(1'b0),
    .p1(rm_lo[7]), .p2(rm_lo[6]), .p3(rm_lo[5]), .p4(rm_lo[4]),
    .p5(rm_lo[3]), .p6(rm_lo[2]), .p7(rm_lo[1]), .p9(rm_lo[0]));

  // ---- EtherFifo (256 x 4) and EtherRcvr's top nibble (256 x 4) ----
  reg [7:0] ea;
  wire [3:0] fifo, rcv_hi;

  cell_MCM10149 #(.INIT_FILE("verilog/proms/packages/DskEth-l10.mem")) u_fifo (
    .p4(ea[7]), .p2(ea[6]), .p3(ea[5]), .p9(ea[4]),
    .p10(ea[3]), .p6(ea[2]), .p5(ea[1]), .p7(ea[0]), .p13(1'b0),
    .p15(fifo[3]), .p14(fifo[2]), .p12(fifo[1]), .p11(fifo[0]));

  cell_MCM10149 #(.INIT_FILE("verilog/proms/packages/DskEth-h09.mem")) u_rcv (
    .p4(ea[7]), .p2(ea[6]), .p3(ea[5]), .p9(ea[4]),
    .p10(ea[3]), .p6(ea[2]), .p5(ea[1]), .p7(ea[0]), .p13(1'b0),
    .p15(rcv_hi[3]), .p14(rcv_hi[2]), .p12(rcv_hi[1]), .p11(rcv_hi[0]));

  reg [15:0] want16;
  reg [3:0] wr, rd;
  reg [2:0] st;
  reg carrier;
  reg [1:0] ev;

  initial begin
    // LMASK[n] is the top n bits of a 16-bit word, RMASK[n] the low n.
    // Entries 16..31 saturate: a 32-entry part holding a 16-bit mask.
    for (int n = 0; n < 32; n++) begin
      sh = n[4:0];
      #1;
      want16 = (n == 0) ? 16'h0000
             : (n >= 16) ? 16'hFFFF
             : ~((16'h1 << (16 - n)) - 16'h1);
      check({lm_hi, lm_lo}, want16, $sformatf("LMASK[%0d]", n));
      want16 = (n == 0) ? 16'h0000
             : (n >= 16) ? 16'hFFFF
             : ((16'h1 << n) - 16'h1);
      check({rm_hi, rm_lo}, want16, $sformatf("RMASK[%0d]", n));
    end

    // EtherFifo: full when read-1 == write (mod 16), empty when equal, and
    // each sense is brought out with its complement.
    for (int i = 0; i < 256; i++) begin
      ea = i[7:0];
      #1;
      wr = i[7:4];
      rd = i[3:0];
      check(fifo[3], ((rd - 4'd1) == wr) ? 1'b1 : 1'b0,
            $sformatf("EtherFifo full w=%0d r=%0d", wr, rd));
      check(fifo[2], !fifo[3], "EtherFifo notFull is the complement");
      check(fifo[1], (rd == wr) ? 1'b1 : 1'b0,
            $sformatf("EtherFifo empty w=%0d r=%0d", wr, rd));
      check(fifo[0], !fifo[1], "EtherFifo notEmpty is the complement");
    end

    // EtherRcvr top nibble = nextState[2:0] then the collision bit. Two
    // properties from the source's comments: with no carrier every state
    // returns to idle, and from idle a carrier always advances to `maybe`.
    for (int i = 0; i < 256; i++) begin
      ea = i[7:0];
      #1;
      st = i[7:5];
      carrier = i[3];
      ev = i[2:1];
      if (!carrier && st != 3'd0)
        check(rcv_hi[3:1], 3'd0,
              $sformatf("EtherRcvr state %0d loses carrier -> idle", st));
      if (st == 3'd0)
        check(rcv_hi[3:1], carrier ? 3'd1 : 3'd0,
              $sformatf("EtherRcvr idle, carrier %0d", carrier));
      if (st > 3'd3)
        check(rcv_hi[3:1], 3'd0,
              $sformatf("EtherRcvr unused state %0d -> idle", st));
    end

    $display("tb_proms: %0d checks, %0d failures", checks, errors);
    if (errors != 0) $fatal(1, "PROM readback does not match");
    $finish;
  end

endmodule

`default_nettype wire
