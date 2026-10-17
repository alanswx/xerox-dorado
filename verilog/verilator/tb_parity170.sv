// MC10170 datasheet p.370: A = XOR(D[0:8]), B = A XOR controls.
// In particular control 13=1,14=0 makes B a bad-odd-parity indication.
module tb_parity170;
  reg [10:0] v;
  wire a, b;
  cell_MC10170 #(.LEGACY_B_INVERT(0)) dut (
    .p3(v[0]), .p4(v[1]), .p5(v[2]), .p6(v[3]), .p7(v[4]),
    .p9(v[5]), .p10(v[6]), .p11(v[7]), .p12(v[8]),
    .p13(v[9]), .p14(v[10]), .p2(a), .p15(b),
    .p1(1'b0), .p8(1'b0), .p16(1'b0));
  initial begin
    for (integer k = 0; k < 2048; k = k + 1) begin
      v = 11'(k);
      #1;
      if (a !== (^v[8:0]) || b !== (^v))
        $fatal(1, "MC10170 truth table mismatch: inputs=%h A=%b B=%b", v, a, b);
    end
    $display("tb_parity170: PASS -- all 2048 data/control combinations");
    $finish;
  end
endmodule
