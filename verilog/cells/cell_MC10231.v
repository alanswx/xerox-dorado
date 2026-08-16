// cell_MC10231 -- High-Speed Dual Type D Master-Slave Flip-Flop
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 152 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// NOTE: the dictionary lists a common-clock pin CC (9) alongside the two
// per-flop clocks. The MECL part is documented as a plain dual D flip-flop,
// so CC is left unmodelled and explicitly marked unused rather than guessed
// at -- if a board drives it, that shows up as a real signal going nowhere.

`default_nettype none

module cell_MC10231 (
    input  wire sys_clk,
    input  wire p6,   // C  clock A,
    input  wire p7,   // D  data A,
    input  wire p4,   // R  reset A,
    input  wire p5,   // S  set A,
    output wire p2,   // Q  A,
    output wire p3,   // Q' A,
    input  wire p11,  // C  clock B,
    input  wire p10,  // D  data B,
    input  wire p13,  // R  reset B,
    input  wire p12,  // S  set B,
    output wire p15,  // Q  B,
    output wire p14,  // Q' B,
    input  wire p9,// CC common clock (dictionary marks it shared),
    // Pins the boards wire that the dictionary does not name --
    // power (VEE/VCC on a 16-pin MECL package) and the like,
    // declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // Dual type-D master-slave flip-flop with asynchronous Set and Reset.
  // MECL S/R are ACTIVE HIGH and dominate the clock.

  // FPGA: ONE CLOCK, and the ECL clock net becomes an ENABLE.
  //
  // The Dorado clocks this part from a distributed ECL clock net (CLK.ph'
  // and friends, fanned out by the BaseBoard). Taking that literally gives
  // the fabric a gated clock off combinational logic -- 1,201 packages across
  // the machine do it -- which no FPGA can route. So every flip-flop here
  // runs on `sys_clk` and transfers on the sys_clk edge FOLLOWING an edge of
  // the modelled clock net. That is the usual oversampling transform, and it
  // matches the part as long as sys_clk is faster than the clock net, which
  // dorado_machine guarantees by dividing.
  //
  // Asynchronous inputs (MR, S/R, CL') are LEVEL-tested every sys_clk edge --
  // the same treatment, and it keeps them out of the fabric's reset network.
  reg cka_d, ckb_d;
  always @(posedge sys_clk) begin cka_d <= p6; ckb_d <= p11; end
  wire cka_en = p6 & ~cka_d;
  wire ckb_en = p11 & ~ckb_d;

  reg qa, qb;
  always @(posedge sys_clk) begin
    if (p5)          qa <= 1'b1;      // S
    else if (p4)     qa <= 1'b0;      // R
    else if (cka_en) qa <= p7;        // D
  end
  always @(posedge sys_clk) begin
    if (p12)         qb <= 1'b1;
    else if (p13)    qb <= 1'b0;
    else if (ckb_en) qb <= p10;
  end
  assign p2 = qa, p3 = ~qa;
  assign p15 = qb, p14 = ~qb;
  wire _unused = &{1'b0, p9, 1'b0};   // CC: see note above

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
