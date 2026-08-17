// cell_MC10162 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 10 package position(s) across the sixteen boards.
//
// Binary to 1-8 Decoder (HIGH) -- the sibling of MC10161, same pins, opposite
// output sense. EclDict names them Q0..Q7 here where the 10161 has Q0'..Q7',
// which is the whole difference: the selected line goes HIGH and the other
// seven stay LOW.
//
//   S1=7, S2=9, S4=14 select; E' at 2 and 15, two active-low enables;
//   Q0..Q7 = 6,5,4,3,13,12,11,10.

`default_nettype none

module cell_MC10162 (
    input  wire p2,  // E'
    output wire p3,  // Q3
    output wire p4,  // Q2
    output wire p5,  // Q1
    output wire p6,  // Q0
    input  wire p7,  // S1
    input  wire p9,  // S2
    output wire p10,  // Q7
    output wire p11,  // Q6
    output wire p12,  // Q5
    output wire p13,  // Q4
    input  wire p14,  // S4
    input  wire p15// E'
);

  wire [2:0] sel = {p14, p9, p7};          // S4, S2, S1
  wire       en  = ~(p2 | p15);            // both E' low
  wire [7:0] q   = en ? (8'd1 << sel) : 8'h00;

  assign p6  = q[0];   assign p5  = q[1];   assign p4  = q[2];
  assign p3  = q[3];   assign p13 = q[4];   assign p12 = q[5];
  assign p11 = q[6];   assign p10 = q[7];
endmodule

`default_nettype wire
