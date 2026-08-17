// cell_MC10125 -- Quad MECL to MTTL Translator
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 66 package
// position(s) across the sixteen Dorado boards.

`default_nettype none

module cell_MC10125 (
    input  wire p3,
    input  wire p2,
    output wire p4,
    input  wire p7,
    input  wire p6,
    output wire p5,
    input  wire p11,
    input  wire p10,
    output wire p12,
    input  wire p1,
    input  wire p8,
    input  wire p9,
    output wire p13,
    input  wire p14,
    input  wire p15,
    input  wire p16
);

  // Quad MECL-to-TTL Translator, and the inputs are DIFFERENTIAL PAIRS. The
  // data book's pin assignment (DL122 rev 7, MC10125) gives AIN/A-bar-IN on
  // pins 3 and 2, BIN on 7 and 6, CIN on 11 and 10, DIN on 15 and 14, with
  // one TTL output each: [G (2 3)>4, (6 7)>5, (10 11)>12, (14 15)>13].
  // EclDict marks the true input `IN` and its partner `c`.
  //
  // The output follows whichever input is higher. Driven properly the
  // complement is the inverse of the true input, so this reduces to the true
  // input; driven single-ended against VBB (pin 1) the partner sits at a
  // reference the digital model reads as 0, which gives the same answer.
  //
  // Before this, each channel read only its true input -- and the fourth
  // channel was not modelled at all, its output tied to zero.
  assign p4  = p3  & ~p2;
  assign p5  = p7  & ~p6;
  assign p12 = p11 & ~p10;
  assign p13 = p15 & ~p14;

  wire _unused_pins = &{1'b0, p1, p9, p16, 1'b0};  // VBB, VCC, GND

endmodule

`default_nettype wire
