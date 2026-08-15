// cell_MC10176 -- Hex D Master-Slave Flip-Flop
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 307 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.

`default_nettype none

module cell_MC10176 (
    input  wire p9,   // C   clock, common to all six,
    input  wire p5,  // D0,
    input  wire p6,  // D1,
    input  wire p7,  // D2,
    input  wire p10,  // D3,
    input  wire p11,  // D4,
    input  wire p12,  // D5,
    output wire p2,  // Q0,
    output wire p3,  // Q1,
    output wire p4,  // Q2,
    output wire p13,  // Q3,
    output wire p14,  // Q4,
    output wire p15,// Q5,
    // Pins the boards wire that the dictionary does not name --
    // power (VEE/VCC on a 16-pin MECL package) and the like,
    // declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // Hex D master-slave flip-flop: six D-type stages on one common clock,
  // data transferred on the POSITIVE-going clock edge.
  reg [5:0] q;
  always @(posedge p9) q <= {p12, p11, p10, p7, p6, p5};
  assign {p15, p14, p13, p4, p3, p2} = q;

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
