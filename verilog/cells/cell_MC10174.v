// cell_MC10174 -- Dual 4-to-1 Multiplexer
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 106 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.

`default_nettype none

module cell_MC10174 (
    input  wire p3,   // X0,
    input  wire p5,   // X1,
    input  wire p4,   // X2,
    input  wire p6,   // X3,
    input  wire p13,  // Y0,
    input  wire p11,  // Y1,
    input  wire p12,  // Y2,
    input  wire p10,  // Y3,
    input  wire p7,   // S1 select bit 0,
    input  wire p9,   // S2 select bit 1,
    input  wire p14,  // E' enable, active LOW,
    output wire p2,   // OX,
    output wire p15,// OY,
    // Pins the boards wire that the dictionary does not name --
    // power (VEE/VCC on a 16-pin MECL package) and the like,
    // declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p16
);

  // Dual 4-to-1 multiplexer, common select and common active-low enable.
  wire [1:0] sel = {p9, p7};                       // S2,S1
  wire       en  = ~p14;                           // E' active low
  assign p2  = en & ((sel == 2'd0) ? p3  :
                     (sel == 2'd1) ? p5  :
                     (sel == 2'd2) ? p4  : p6);
  assign p15 = en & ((sel == 2'd0) ? p13 :
                     (sel == 2'd1) ? p11 :
                     (sel == 2'd2) ? p12 : p10);

  wire _unused_pins = &{1'b0, p1, p16, 1'b0};
endmodule

`default_nettype wire
