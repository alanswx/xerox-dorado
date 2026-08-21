// tb_muffler -- the BaseBoard's own MUFFLER NUMBER, and the strap that sets it.
//
// The BaseBoard answers muffler reads addressed to itself and passes the rest
// down the backplane. Which address is "itself" is not in the wire list: it is
// set by a pair of resistor packs at l49/l50, both sitting on the same five
// nets Midas.00..Midas.04, l50 pulling every one DOWN and l49 pulling some UP.
// Which ones is a property of the PART FITTED, and only the schematic records
// it. Basebd04.sil ("Muffler and Manifold", PDF page 8) labels l49
//
//     Terminator with all pins cut except 1, 3, 6, & 8
//
// and the stuffing sheet BaseBd17.sil says the same thing the other way round,
// "Location l49 must be loaded with a SIP having pins 2,4,5,&7 cut". Pins 1
// and 8 are the VCC commons, so the legs that survive are pin 3 = Midas.01 and
// pin 6 = Midas.04.
//
// k20, an SN74LS85, compares the incoming muffler address DMD.04..01 against
// that strap and drives `BaseMuf'` through the k21b inverter. `BaseMuf'` is
// the enable on k22 -- the LS151 that sources `CPDMuxData`, i.e. the data the
// BaseBoard sends BACK -- and on the k19 LS138. So with the strap unmodelled
// the comparator can never match, `BaseMuf'` never asserts, and the BaseBoard
// is incapable of answering a muffler read at all.
//
// This checks three things:
//   1. the strap reads 1001 = 9, the number the fitted part encodes;
//   2. `BaseMuf'` is the comparator's complement of (DMD.04..01 == 9) at
//      EVERY step, not merely at the end -- a property, not a sample; and
//   3. exactly one of the sixteen addresses selects the board.
//
// The strap VALUE is what has to be checked, not just that `BaseMuf'` is
// well-behaved. Left open these nets do not go X -- Verilator starts a wire
// at 0 -- so the comparator simply sits at a strap of 0 and answers to the
// wrong address, quietly. Mutation-tested both ways: cutting l49's pin 6 as
// well reads 1, and dropping the strap resolution reads 0; the value check
// catches both and the property check catches neither.

`default_nettype none

`define BB m.u_machine.b_BaseBd

module tb_muffler;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  dorado_baseboard_machine m (.sys_clk(sys_clk));

  // k20's B inputs, as the LS85 orders them: pin 9 = B3 .. pin 1 = B0.
  wire [3:0] strap = {`BB.Midas_04, `BB.Midas_03, `BB.Midas_02, `BB.Midas_01};
  // k20's A inputs: pin 10 = A3 .. pin 15 = A0.
  wire [3:0] addr  = {`BB.DMD_04, `BB.DMD_03, `BB.DMD_02, `BB.DMD_01};
  wire       basemuf_n = `BB.BaseMuf_p_;

  integer i, violations, undefined, asserts, prev_asserted, n_match;

  initial begin
    violations = 0; undefined = 0; asserts = 0; prev_asserted = 0;

    // Settle, then read the strap.
    for (i = 0; i < 200; i = i + 1) @(posedge sys_clk);

    $display("tb_muffler: Midas strap = %b (Midas.04..01) = %0d", strap, strap);
    if (strap !== 4'b1001) begin
      $display("tb_muffler: FAIL -- strap is %b, schematic says 1001 (l49 keeps pins 3 and 6)", strap);
      $fatal(1);
    end

    // Run the firmware and hold the comparator to its definition throughout.
    for (i = 0; i < 400000; i = i + 1) begin
      @(posedge sys_clk);
      if (basemuf_n === 1'bx || basemuf_n === 1'bz) undefined = undefined + 1;
      else begin
        if (basemuf_n !== ~(addr == strap)) violations = violations + 1;
        if (basemuf_n === 1'b0) begin
          if (!prev_asserted) asserts = asserts + 1;
          prev_asserted = 1;
        end else prev_asserted = 0;
      end
    end

    $display("tb_muffler: BaseMuf' undefined on %0d steps, %0d definition violations, %0d assertions",
             undefined, violations, asserts);

    // The firmware does not happen to address muffler 9 in that window, so
    // the match itself is still untested. Drive the comparator's A inputs
    // through all sixteen addresses and require it to select exactly one.
    n_match = 0;
    for (i = 0; i < 16; i = i + 1) begin
      force `BB.DMD_04 = i[3];
      force `BB.DMD_03 = i[2];
      force `BB.DMD_02 = i[1];
      force `BB.DMD_01 = i[0];
      @(posedge sys_clk); @(posedge sys_clk);
      if (basemuf_n === 1'b0) begin
        n_match = n_match + 1;
        $display("tb_muffler:   address %2d -> BaseMuf' ASSERTED", i);
        if (i != 9) begin
          $display("tb_muffler: FAIL -- asserted at %0d, not the strapped 9", i);
          $fatal(1);
        end
      end
    end
    release `BB.DMD_04; release `BB.DMD_03;
    release `BB.DMD_02; release `BB.DMD_01;
    if (n_match != 1) begin
      $display("tb_muffler: FAIL -- %0d of 16 addresses select the BaseBoard, want exactly 1", n_match);
      $fatal(1);
    end

    if (undefined != 0) begin
      $display("tb_muffler: FAIL -- BaseMuf' is not a defined level; the strap is open");
      $fatal(1);
    end
    if (violations != 0) begin
      $display("tb_muffler: FAIL -- BaseMuf' is not ~(DMD.04..01 == strap)");
      $fatal(1);
    end
    $display("tb_muffler: PASS -- muffler number 9, selected by that address alone");
    $finish;
  end

endmodule
