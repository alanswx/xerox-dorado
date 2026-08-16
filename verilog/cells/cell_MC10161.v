// cell_MC10161 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 21 package position(s) across the sixteen boards.
//
// Binary to 1-8 Decoder (LOW), per cells/PARTS.md. EclDict names every pin,
// so this needs no interpretation of output polarity: `S1`=7, `S2`=9, `S4`=14
// are the binary select with their own weights; `Q0'`..`Q7'` are the eight
// outputs, PRIMED because the part is the low-output variant -- the selected
// line goes LOW and the other seven stay HIGH; `E'` appears at both 2 and 15,
// two active-low enables that must both be low.
//
// Disabled, every output is high. That is the reset state a decoder wants and
// it falls out of the same expression.

`default_nettype none

module cell_MC10161 (
    input  wire p2,  // E'
    output wire p3,  // Q3'
    output wire p4,  // Q2'
    output wire p5,  // Q1'
    output wire p6,  // Q0'
    input  wire p7,  // S1
    input  wire p9,  // S2
    output wire p10,  // Q7'
    output wire p11,  // Q6'
    output wire p12,  // Q5'
    output wire p13,  // Q4'
    input  wire p14,  // S4
    input  wire p15// E'
);

  wire [2:0] sel = {p14, p9, p7};          // S4, S2, S1
  wire       en  = ~(p2 | p15);            // both E' low
  wire [7:0] q   = en ? ~(8'd1 << sel) : 8'hFF;

  assign p6  = q[0];   // Q0'
  assign p5  = q[1];   // Q1'
  assign p4  = q[2];   // Q2'
  assign p3  = q[3];   // Q3'
  assign p13 = q[4];   // Q4'
  assign p12 = q[5];   // Q5'
  assign p11 = q[6];   // Q6'
  assign p10 = q[7];   // Q7'
endmodule

`default_nettype wire
