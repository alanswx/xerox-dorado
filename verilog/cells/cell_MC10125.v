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

  // A level translator: logically a differential buffer. Each gate takes a
  // signal and its reference/complement and drives the TTL side. Modelled as
  // a buffer of the signal input -- the LEVELS are what the real part
  // changes, and levels do not exist in RTL.
  assign p4  = p3;
  assign p5  = p7;
  assign p12 = p11;
  wire _unused_ref = &{1'b0, p2, p6, p10, 1'b0};


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p9, p14, p15, p16, 1'b0};
  assign p13 = 1'b0;   // not modelled

endmodule

`default_nettype wire
