// cell_AUGATCG16 -- Passive package (resistor network / pull-up), NOT logic
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 41 package position(s).

`default_nettype none

module cell_AUGATCG16 (
    input  wire p1,
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p8,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p15,
    input  wire p16
);

  // The dictionary lists this with the same pinout as PULLUP and the PLATnn
  // entries, so it is a resistor network -- a terminator or pull-up pack,
  // not a gate. There is nothing to compute: it sets DC levels, which do not
  // exist in RTL. Declared so the boards' connections land somewhere, and
  // left deliberately empty rather than being given invented behaviour.


endmodule

`default_nettype wire
