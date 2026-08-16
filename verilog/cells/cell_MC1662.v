// cell_MC1662 -- Quad 2-Input OR (MECL III)
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 33 package position(s).

`default_nettype none

module cell_MC1662 (
    input  wire p4,
    input  wire p5,
    output wire p2,
    input  wire p6,
    input  wire p7,
    output wire p3,
    input  wire p10,
    input  wire p11,
    output wire p14,
    input  wire p12,
    input  wire p13,
    output wire p15
);

  assign p2  = p4  | p5;
  assign p3  = p6  | p7;
  assign p14 = p10 | p11;
  assign p15 = p12 | p13;


endmodule

`default_nettype wire
