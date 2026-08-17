// cell_SN74LS155 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Dual 2-to-4 Decoder / 1-to-4 Demultiplexer (TTL). TtlDict:
//   a,S1,13 > a,S2,3                     the shared select, S1 the lower bit
//   a,DQ,1  > a,EQ',2  > a,Q0',7 > a,Q1',6 > a,Q2',5 > a,Q3',4
//   a,DR',15 > a,ER',14 > a,R0',9 > a,R1',10 > a,R2',11 > a,R3',12
//
// Outputs are active LOW. The two halves differ in their DATA polarity, which
// is what the dictionary's priming says and what the data sheet's truth table
// gives: the first half passes its data input, the second passes it inverted,
// so tying the two data pins together makes a 1-of-8 decoder.

`default_nettype none

module cell_SN74LS155 (
    input  wire p1,  // DQ
    input  wire p2,  // EQ'
    input  wire p3,  // S2
    output wire p4,  // Q3'
    output wire p5,  // Q2'
    output wire p6,  // Q1'
    output wire p7,  // Q0'
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // R0'
    output wire p10,  // R1'
    output wire p11,  // R2'
    output wire p12,  // R3'
    input  wire p13,  // S1
    input  wire p14,  // ER'
    input  wire p15,  // DR'
    input  wire p16// (no name in EclDict)
);

  wire [1:0] sel = {p3, p13};             // S2, S1

  assign p7  = ~((sel == 2'd0) & ~p2 & p1);   // Q0'
  assign p6  = ~((sel == 2'd1) & ~p2 & p1);
  assign p5  = ~((sel == 2'd2) & ~p2 & p1);
  assign p4  = ~((sel == 2'd3) & ~p2 & p1);

  assign p9  = ~((sel == 2'd0) & ~p14 & ~p15);  // R0'
  assign p10 = ~((sel == 2'd1) & ~p14 & ~p15);
  assign p11 = ~((sel == 2'd2) & ~p14 & ~p15);
  assign p12 = ~((sel == 2'd3) & ~p14 & ~p15);

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
