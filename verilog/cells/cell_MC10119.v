// cell_MC10119 -- 4-Wide 4-3-3-3-Input OR-AND
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 10 package position(s).

`default_nettype none

module cell_MC10119 (
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p9,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p15,
    input  wire p10,
    output wire p2
);

  // OR each input group, then AND the groups (the part's own shape).
  wire y = (p3 | p4 | p5 | p6) & (p7 | p9) & (p11 | p12) & (p13 | p14 | p15) & p10;
  assign p2  =  y;


endmodule

`default_nettype wire
