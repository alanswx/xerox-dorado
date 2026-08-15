// cell_MU10164 -- SECOND SOURCE for MC10164 (the dictionary alias table records them as the same part)
// cell_MC10164 -- 8-Line Multiplexer
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 19 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.

`default_nettype none

module cell_MU10164 (
    input  wire p6,  // D0,
    input  wire p5,  // D1,
    input  wire p4,  // D2,
    input  wire p3,  // D3,
    input  wire p11,  // D4,
    input  wire p12,  // D5,
    input  wire p13,  // D6,
    input  wire p14,  // D7,
    input  wire p7,   // S1 select bit 0,
    input  wire p9,   // S2 select bit 1,
    input  wire p10,  // S4 select bit 2,
    input  wire p2,   // E' enable, active LOW,
    output wire p15,// Z,
    // Pins the boards wire that the dictionary does not name --
    // power (VEE/VCC on a 16-pin MECL package) and the like,
    // declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // 8-line multiplexer with active-low enable.
  wire [2:0] sel = {p10, p9, p7};              // S4,S2,S1
  wire [7:0] d   = {p14, p13, p12, p11, p3, p4, p5, p6};  // D7..D0
  assign p15 = (~p2) & d[sel];

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
