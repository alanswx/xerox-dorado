// tb_alu_vs_c -- the 1979 netlist against the software model.
//
// Four cell_MC10181 slices chained into the 16-bit ALU, exactly as ProcH and
// ProcL build it, fed the vectors dorado/tests/alu_vectors.c dumps from the C
// emulator's own alu_op(). Neither side was derived from the other: the cell
// is a transcription of Motorola's datasheet, alu_op() was written from the
// Hardware Manual's Table 9, and this is where they check each other.
//
// The ALUFM entry decodes as {Cn, S3, S2, S1, S0, M}.
//
// What only this test can catch: the CARRY RIPPLE between slices. Four
// correct 4-bit adders still give a wrong 16-bit answer if the carry chain is
// wired backwards, and no single-slice test can see it.
//
// Run with +vectors=<file>; the make target does that.

`default_nettype none

module tb_alu_vs_c;

  reg [15:0] A, B;
  integer vo;   // the C emulator's signed-overflow answer
  integer arith_n = 0, sc_same = 0, sc_inv = 0;
  integer per_same [0:63], per_tot [0:63]; integer ei;
  reg [3:0]  S;
  reg        M, Cn;

  wire [15:0] F;
  wire [4:0]  carry;          // carry[0] is Cn in, carry[4] the ALU's carry out
  assign carry[0] = Cn;

  // Slice k covers bits [4k+3:4k]. Pin mapping is EclDict's, MSB-first:
  // D0 = pin 10 is the slice's most significant operand bit.
  // ProcH d13 -- SIGNED OVERFLOW, and the reason this bench grew a sixth
  // column. d13 is an MC10170 (a 9-bit parity generator with two control
  // inputs) wired by PARC as an overflow detector; the wire list gives it
  // exactly the operand sign bits, the result sign, the ALU's carry out, and
  // aluF0:
  //
  //     pin  3  alub.00a  B operand bit 00 -- PARC is MSB-first, so B[15]
  //     pin  4  alua.00   A operand sign            -> A[15]
  //     pin  9  aluF0     = S3: it lands on pin 13 of BOTH MC10181s, which
  //                       this bench already maps to S[3]
  //     pin 13  alu.00    the ALU result's MSB      -> F[15]
  //     pin 14  aluCout   the ALU's carry out       -> carry[4]
  //     pin 15  SignedCarry
  //
  // Its other six data pins are unconnected on the board, so they are 0 here.
  // Use the physical XOR selected by the generated ProcH d13 instance.
  // signed-compare-test also checks that board parameter and ContA's gate;
  // this bench checks the detector against the C arithmetic vectors.
  wire signed_carry;
  cell_MC10170 #(.LEGACY_B_INVERT(0)) u_d13 (
      .p3(B[15]), .p4(A[15]), .p5(1'b0), .p6(1'b0), .p7(1'b0),
      .p9(S[3]),  .p10(1'b0), .p11(1'b0), .p12(1'b0),
      .p13(F[15]), .p14(carry[4]),
      .p2(), .p15(signed_carry),
      .p1(1'b0), .p8(1'b0), .p16(1'b0)
  );

  genvar k;
  generate
    for (k = 0; k < 4; k = k + 1) begin : slice
      cell_MC10181 u (
        .p10(A[4*k+3]), .p16(A[4*k+2]), .p18(A[4*k+1]), .p21(A[4*k+0]),
        .p9 (B[4*k+3]), .p11(B[4*k+2]), .p19(B[4*k+1]), .p20(B[4*k+0]),
        .p13(S[3]), .p15(S[2]), .p17(S[1]), .p14(S[0]),
        .p23(M), .p22(carry[k]),
        .p6 (F[4*k+3]), .p7 (F[4*k+2]), .p3 (F[4*k+1]), .p2 (F[4*k+0]),
        .p5 (carry[k+1]), .p4(), .p8()
      );
    end
  endgenerate

  integer fd, n, cases, errors, shown;
  reg [7:0]  entry;
  reg [15:0] va, vb, vr;
  integer    vc;
  string     path;

  initial begin
    if (!$value$plusargs("vectors=%s", path)) path = "alu.vec";
    fd = $fopen(path, "r");
    if (fd == 0) begin
      $display("tb_alu_vs_c: cannot open %s", path);
      $fatal(1, "no vectors");
    end

    cases = 0; errors = 0; shown = 0;
    for (ei = 0; ei < 64; ei = ei + 1) begin per_same[ei]=0; per_tot[ei]=0; end
    // A `forever` with `break` here counted correctly inside the loop and
    // read back zero after it; a plain while does not.
    n = $fscanf(fd, "%h %h %h %h %d %d\n", entry, va, vb, vr, vc, vo);
    while (n == 6) begin
      Cn = entry[5];
      S  = entry[4:1];
      M  = entry[0];
      A  = va;
      B  = vb;
      #1;

      cases = cases + 1;
      if (F !== vr) begin
        errors = errors + 1;
        if (shown < 10) begin
          shown = shown + 1;
          $display("  FAIL entry=%02h A=%04h B=%04h : RTL %04h, emulator %04h",
                   entry, va, vb, F, vr);
        end
      end
      // The emulator only reports a carry for arithmetic ops; in logic mode
      // the datasheet calls the carry out don't-care, so it is not compared.
      else if (M == 1'b0 && carry[4] !== vc[0]) begin
        errors = errors + 1;
        if (shown < 10) begin
          shown = shown + 1;
          $display("  FAIL entry=%02h A=%04h B=%04h : RTL carry %b, emulator %b",
                   entry, va, vb, carry[4], vc[0]);
        end
      end
      // Overflow is defined for the signed add/subtract operations below.
      // These used to be informational tallies, allowing the legacy polarity
      // to pass the gate even though Cedar's signed jumps depend on it.
      if (M == 1'b0) begin
        arith_n = arith_n + 1;
        per_tot[entry[5:0]]  = per_tot[entry[5:0]] + 1;
        if (signed_carry === vo[0]) per_same[entry[5:0]] = per_same[entry[5:0]] + 1;
        if (signed_carry === vo[0])  sc_same = sc_same + 1;
        else                         sc_inv  = sc_inv  + 1;
      end
      n = $fscanf(fd, "%h %h %h %h %d %d\n", entry, va, vb, vr, vc, vo);
    end
    $fclose(fd);
    $display("tb_alu_vs_c: SignedCarry (ProcH d13) vs the emulator's ovf over %0d arithmetic vectors -- agrees %0d, disagrees %0d",
             arith_n, sc_same, sc_inv);
    // HM3.7 specifies overflow for A+B, A+B+1, A-B-1 and A-B.
    // Keep per-entry tallies for the other arithmetic functions, whose flag
    // meaning is less general, and gate the four signed operations.
    for (ei = 0; ei < 64; ei = ei + 1)
      if (ei == 6'o14 || ei == 6'o54 || ei == 6'o22 || ei == 6'o62)
        if (per_tot[ei] == 0 || per_same[ei] != per_tot[ei]) begin
          errors = errors + 1;
          $display("tb_alu_vs_c: FAIL signed overflow entry %02o: %0d/%0d", ei, per_same[ei], per_tot[ei]);
        end

    for (ei = 0; ei < 64; ei = ei + 1)
      if (per_tot[ei] != 0)
        $display("tb_alu_vs_c:   entry %02o : agrees %0d of %0d", ei, per_same[ei], per_tot[ei]);

    $display("tb_alu_vs_c: %0d vectors from the C emulator, %0d mismatches",
             cases, errors);
    if (cases == 0) $fatal(1, "no vectors were read");
    if (errors != 0) $fatal(1, "the RTL and the C emulator disagree");
    $finish;
  end

endmodule

`default_nettype wire
