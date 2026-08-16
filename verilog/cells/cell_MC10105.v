// cell_MC10105 -- Triple 2-3-2 Input OR/NOR
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 34 package position(s).

//
// POLARITY, confirmed from the datasheets (2026-08-16): EclDict's role `OUT`
// is the INVERTING (NOR) output and `o` the non-inverting (OR) one. Motorola's
// MC10101 sheet labels the four `OUT` pins A-bar-OUT..D-bar-OUT and the four
// `o` pins AOUT..DOUT; MC10212's labels its `OUT` pins 3,4,12,13 with bars and
// its `o` pins 2,14 without. Eight gates, two parts, unanimous. See
// docs/verilog-handoff.md.

`default_nettype none

module cell_MC10105 (
    input  wire p4,
    input  wire p5,
    output wire p3,
    output wire p2,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    output wire p6,
    output wire p7,
    input  wire p12,
    input  wire p13,
    output wire p14,
    output wire p15,
    input  wire p1
);

  // EclDict: a,OUT,3 > a,o,2 | b,OUT,6 > b,o,7 | c,OUT,14 > c,o,15.
  // OUT is the NOR. These were the other way round until the datasheets
  // settled it -- 34 packages, 31 of them in the machine.
  assign p3  = ~(p4 | p5);          assign p2  =  (p4 | p5);
  assign p6  = ~(p9 | p10 | p11);   assign p7  =  (p9 | p10 | p11);
  assign p14 = ~(p12 | p13);        assign p15 =  (p12 | p13);


  wire _unused_pins = &{1'b0, p1, 1'b0};

endmodule

`default_nettype wire
