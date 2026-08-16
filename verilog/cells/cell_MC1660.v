// cell_MC1660 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 28 package position(s) across the sixteen boards.
//
// Dual 4-Input OR/NOR (MECL III). Function: Motorola MECL Pocket Book,
// see cells/PARTS.md. Gate grouping from EclDict's own pin names:
// a_IN0-3 = 4,5,6,7 -> a outputs 3 and 15; b_IN0-3 = 10,11,12,13 -> b
// outputs 14 and 2.
//
// POLARITY follows the rule cell_MC10101 states and derives: the pin the
// dictionary marks `out` carries the INVERTING (NOR) sense, the pin marked
// `nout` the non-inverting (OR) sense. Here that is p3/p14 = NOR and
// p15/p2 = OR.
//
// The circuit confirms it. BaseBd g05 takes one input (p7) and drives both
// a-outputs: p3 to `BaseBd11.sil+1` and p2 to `+2`, which clock the two
// halves of the MC1690 divider. Those have to be ANTI-PHASE for a two-phase
// clock generator, and opposite senses is exactly what makes them so.
//
// Unused MECL inputs have on-chip pulldowns and read LOW; Verilator drives an
// unconnected input to 0, which matches.

`default_nettype none

module cell_MC1660 (
    output wire p2,  // a_OUTN
    output wire p3,  // a_OUT
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // a_IN2
    input  wire p7,  // a_IN3
    input  wire p8,  // (no name in EclDict)
    input  wire p10,  // b_IN0
    input  wire p11,  // b_IN1
    input  wire p12,  // b_IN2
    input  wire p13,  // b_IN3
    output wire p14,  // b_OUT
    output wire p15// b_OUTN
);

  wire a = p4 | p5 | p6 | p7;
  wire b = p10 | p11 | p12 | p13;
  assign p3  = ~a;      // a_OUT   (dictionary `out`  = inverting)
  assign p15 =  a;      // a_OUTN  (dictionary `nout` = non-inverting)
  assign p14 = ~b;      // b_OUT
  assign p2  =  b;      // b_OUTN

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p8, 1'b0};
endmodule

`default_nettype wire
