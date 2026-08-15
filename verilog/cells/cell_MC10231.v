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
  reg qa, qb;
  always @(posedge p6 or posedge p4 or posedge p5) begin
    if (p5)      qa <= 1'b1;      // S
    else if (p4) qa <= 1'b0;      // R
    else         qa <= p7;        // D
  end
  always @(posedge p11 or posedge p13 or posedge p12) begin
    if (p12)      qb <= 1'b1;
    else if (p13) qb <= 1'b0;
    else          qb <= p10;
  end
  assign p2 = qa, p3 = ~qa;
  assign p15 = qb, p14 = ~qb;
  wire _unused = &{1'b0, p9, 1'b0};   // CC: see note above

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
