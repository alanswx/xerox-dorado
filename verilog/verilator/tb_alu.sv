// tb_alu -- does the MC10181 compute what its datasheet says?
//
// This is the Dorado's ALU: two slices on ProcH and two on ProcL make the
// 16-bit datapath. cell_MC10181 is a transcription of the datasheet's
// function table (DoradoDocs/datasheets/MC10181.pdf), written as a 16-way
// mux, and a transcription deserves a structural check rather than a reading.
//
// Every check below is against an INDEPENDENT expression of the same
// function -- `A + B`, `A - B`, `A & B` written directly -- not against the
// table the cell was built from. Exhaustive over both operands and both
// carry-in values, which is only 512 cases per row.
//
// It also pins the two things a transcription gets wrong quietly: the MSB-
// first pin mapping (EclDict's D0 is the datasheet's A3) and the sense of Cn,
// which turns "A minus B minus 1" into "A minus B".

`default_nettype none

module tb_alu;

  integer errors = 0, checks = 0;

  reg [3:0] a, b, s;
  reg       m, cin;
  wire [3:0] f;
  wire       cout, gg, pg;

  // Pin mapping straight from EclDict, MSB-first: D0=pin10=A3 ... D3=pin21=A0
  cell_MC10181 u (
    .p10(a[3]), .p16(a[2]), .p18(a[1]), .p21(a[0]),   // A3..A0
    .p9 (b[3]), .p11(b[2]), .p19(b[1]), .p20(b[0]),   // B3..B0
    .p13(s[3]), .p15(s[2]), .p17(s[1]), .p14(s[0]),   // S3..S0
    .p23(m), .p22(cin),
    .p6 (f[3]), .p7 (f[2]), .p3 (f[1]), .p2 (f[0]),   // F3..F0
    .p5 (cout), .p4(gg), .p8(pg)
  );

  task check(input [4:0] got, input [4:0] want, input string what);
    begin
      checks = checks + 1;
      if (got !== want) begin
        errors = errors + 1;
        if (errors <= 12)
          $display("  FAIL %-30s a=%h b=%h cin=%b -> %h want %h",
                   what, a, b, cin, got, want);
      end
    end
  endtask

  // Drive one case and settle.
  task drive(input [3:0] sel, input md, input ci, input [3:0] av,
             input [3:0] bv);
    begin s = sel; m = md; cin = ci; a = av; b = bv; #1; end
  endtask

  integer i, j, c;
  reg [4:0] want;
  // NOTE: `~j[3:0]` in a 5-bit context is extended BEFORE it is inverted and
  // becomes 5'h1F. Invert at the operand width, in its own 4-bit reg.
  reg [3:0] nb;

  initial begin
    // ---- arithmetic, M low. The table is stated for Cn low, and a high Cn
    // adds one -- that is the whole difference between "A minus B minus 1"
    // and "A minus B".
    for (c = 0; c < 2; c = c + 1)
      for (i = 0; i < 16; i = i + 1)
        for (j = 0; j < 16; j = j + 1) begin
          // S=6: F = A plus B
          drive(4'd6, 1'b0, c[0], i[3:0], j[3:0]);
          want = i[3:0] + j[3:0] + c[0];
          check({cout, f}, want, "S6 A plus B");

          // S=9: F = A minus B minus 1, so with Cn it is A minus B
          drive(4'd9, 1'b0, c[0], i[3:0], j[3:0]);
          nb   = ~j[3:0];
          want = i[3:0] + nb + c[0];
          check({cout, f}, want, "S9 A minus B minus 1");

          // S=3: F = A times 2
          drive(4'd3, 1'b0, c[0], i[3:0], j[3:0]);
          want = i[3:0] + i[3:0] + c[0];
          check({cout, f}, want, "S3 A times 2");

          // S=15: F = A minus 1
          drive(4'd15, 1'b0, c[0], i[3:0], j[3:0]);
          want = i[3:0] + 4'hF + c[0];
          check({cout, f}, want, "S15 A minus 1");

          // S=0: F = A
          drive(4'd0, 1'b0, c[0], i[3:0], j[3:0]);
          want = i[3:0] + c[0];
          check({cout, f}, want, "S0 A");
        end

    // S=12: F = minus 1, whatever the operands are
    for (i = 0; i < 16; i = i + 1) begin
      drive(4'd12, 1'b0, 1'b0, i[3:0], 4'h5);
      check({1'b0, f}, {1'b0, 4'hF}, "S12 minus 1");
    end

    // ---- logic, M high. Carry plays no part.
    for (i = 0; i < 16; i = i + 1)
      for (j = 0; j < 16; j = j + 1) begin
        drive(4'd14, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, i[3:0] & j[3:0]}, "S14 logic A.B");
        drive(4'd11, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, i[3:0] | j[3:0]}, "S11 logic A+B");
        drive(4'd9, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, i[3:0] ^ j[3:0]}, "S9 logic A xor B");
        drive(4'd6, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, ~(i[3:0] ^ j[3:0])}, "S6 logic A xnor B");
        drive(4'd0, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, ~i[3:0]}, "S0 logic not A");
        drive(4'd15, 1'b1, 1'b0, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, i[3:0]}, "S15 logic A");
        // Logic mode must not let the carry chain change the answer.
        drive(4'd14, 1'b1, 1'b1, i[3:0], j[3:0]);
        check({1'b0, f}, {1'b0, i[3:0] & j[3:0]}, "S14 logic, Cn ignored");
      end

    // ---- the look-ahead pair: Cn+4 must equal GG + PG.Cn, which is what
    // lets four of these chain into a 16-bit ALU.
    for (i = 0; i < 16; i = i + 1)
      for (j = 0; j < 16; j = j + 1)
        for (c = 0; c < 2; c = c + 1) begin
          drive(4'd6, 1'b0, c[0], i[3:0], j[3:0]);
          check({4'd0, cout}, {4'd0, (gg | (pg & c[0]))}, "Cn+4 = GG + PG.Cn");
        end

    $display("tb_alu: %0d checks, %0d failures", checks, errors);
    if (errors != 0) $fatal(1, "the ALU does not match its datasheet");
    $finish;
  end

endmodule

`default_nettype wire
