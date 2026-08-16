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
  reg [3:0]  S;
  reg        M, Cn;

  wire [15:0] F;
  wire [4:0]  carry;          // carry[0] is Cn in, carry[4] the ALU's carry out
  assign carry[0] = Cn;

  // Slice k covers bits [4k+3:4k]. Pin mapping is EclDict's, MSB-first:
  // D0 = pin 10 is the slice's most significant operand bit.
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
    // A `forever` with `break` here counted correctly inside the loop and
    // read back zero after it; a plain while does not.
    n = $fscanf(fd, "%h %h %h %h %d\n", entry, va, vb, vr, vc);
    while (n == 5) begin
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
      n = $fscanf(fd, "%h %h %h %h %d\n", entry, va, vb, vr, vc);
    end
    $fclose(fd);

    $display("tb_alu_vs_c: %0d vectors from the C emulator, %0d mismatches",
             cases, errors);
    if (cases == 0) $fatal(1, "no vectors were read");
    if (errors != 0) $fatal(1, "the RTL and the C emulator disagree");
    $finish;
  end

endmodule

`default_nettype wire
