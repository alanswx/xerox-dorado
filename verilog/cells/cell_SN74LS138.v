// cell_SN74LS138 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 7 package position(s) across the sixteen boards.
//
// 3-to-8 Decoder (TTL). PARC's TtlDict names every pin:
//
//   LS138, S138
//   a,S4,3 > a,S2,2 > a,S1,1 > a,E',4,5 > a,E,6
//   a,Q0',15 > a,Q1',14 > a,Q2',13 > a,Q3',12 > a,Q4',11 > a,Q5',10
//   a,Q6',9  > a,Q7',7
//
// S1/S2/S4 carry their own weights, the outputs are active low, and there are
// three enables: two active low (4, 5) and one active high (6).
//
// The BaseBoard's g11 is one of these, and it is the ROM decoder -- its eight
// outputs are `Rom0'`..`Rom7'`, the chip selects of the eight 2716 sockets.

`default_nettype none

module cell_SN74LS138 (
    input  wire p1,  // S1
    input  wire p2,  // S2
    input  wire p3,  // S4
    input  wire p4,  // E'
    input  wire p5,  // E'
    input  wire p6,  // E
    output wire p7,  // Q7'
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // Q6'
    output wire p10,  // Q5'
    output wire p11,  // Q4'
    output wire p12,  // Q3'
    output wire p13,  // Q2'
    output wire p14,  // Q1'
    output wire p15,  // Q0'
    input  wire p16// (no name in EclDict)
);

  wire [2:0] sel = {p3, p2, p1};          // S4, S2, S1
  wire       en  = p6 & ~p4 & ~p5;        // E and both E'
  wire [7:0] q   = en ? ~(8'd1 << sel) : 8'hFF;

  assign p15 = q[0];  assign p14 = q[1];  assign p13 = q[2];
  assign p12 = q[3];  assign p11 = q[4];  assign p10 = q[5];
  assign p9  = q[6];  assign p7  = q[7];

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};   // GND, VCC
endmodule

`default_nettype wire
