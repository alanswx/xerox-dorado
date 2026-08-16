// cell_MC10118 -- Dual 2-Wide 3-Input OR-AND
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 9 package position(s).

`default_nettype none

module cell_MC10118 (
    input  wire p6,
    input  wire p7,
    input  wire p10,
    input  wire p11,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p9,
    output wire p2,
    output wire p15
);

  // OR each input group, then AND the groups (the part's own shape).
  wire y = (p6 | p7) & (p10 | p11) & (p3 | p4 | p5) & (p12 | p13 | p14) & p9;
  assign p2  =  y;
  assign p15 = ~y;


endmodule

`default_nettype wire
