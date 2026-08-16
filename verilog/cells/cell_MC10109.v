// cell_MC10109 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 21 package position(s) across the sixteen boards.
//
// Dual 4-5-Input OR/NOR (MECL 10K), and the datasheet notes it is pin
// compatible with the MECL III MC1660 -- which the dictionary agrees with,
// giving both parts the same roles at the same pins.
//
// EclDict: a,IN,4,5,6,7 > a,OUT,3 > a,o,2
//          b,IN,9,10,11,12,13 > b,OUT,14 > b,o,15
//
// POLARITY, confirmed from the datasheets (2026-08-16): EclDict's role `OUT`
// is the INVERTING (NOR) output and `o` the non-inverting (OR) one. Motorola's
// MC10101 sheet labels the four `OUT` pins A-bar-OUT..D-bar-OUT and the four
// `o` pins AOUT..DOUT; MC10212's labels its `OUT` pins 3,4,12,13 with bars and
// its `o` pins 2,14 without. Eight gates, two parts, unanimous. See
// docs/verilog-handoff.md.

`default_nettype none

module cell_MC10109 (
    output wire p2,  // a_OUTN
    output wire p3,  // a_OUT
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // a_IN2
    input  wire p7,  // a_IN3
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // b_IN0
    input  wire p10,  // b_IN1
    input  wire p11,  // b_IN2
    input  wire p12,  // b_IN3
    input  wire p13,  // b_IN4
    output wire p14,  // b_OUT
    output wire p15// b_OUTN
);

  wire a = p4 | p5 | p6 | p7;
  wire b = p9 | p10 | p11 | p12 | p13;
  assign p3  = ~a;   // a_OUT   NOR
  assign p2  =  a;   // a_OUTN  OR
  assign p14 = ~b;   // b_OUT   NOR
  assign p15 =  b;   // b_OUTN  OR

  wire _unused_pins = &{1'b0, p8, 1'b0};
endmodule

`default_nettype wire
