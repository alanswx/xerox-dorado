// cell_SN74S174 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 28 package position(s) across the sixteen boards.
//
// Hex D Flip-Flop with direct clear (TTL, Schottky). PARC's TtlDict gives the whole
// part in one line -- data to output, clock and clear:
//
//   [FF 3>2, 4>5, 6>7, 11>10, 13>12, 14>15 : CLK 9 RS 1]
//
// which is the standard 74174 pinout: CLR' on pin 1, CLK on 9, six D/Q pairs.
// Positive-edge triggered, clear active LOW and asynchronous.
//
// FPGA: on `sys_clk` with the clock as an enable, and the clear level-tested
// on the same edge -- see cell_MC10176 for why.

`default_nettype none

module cell_SN74S174 (
    input  wire sys_clk,
    input  wire p1,  // CL'
    output wire p2,  // Q0
    input  wire p3,  // D0
    input  wire p4,  // D1
    output wire p5,  // Q1
    input  wire p6,  // D2
    output wire p7,  // Q2
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CK
    output wire p10,  // Q3
    input  wire p11,  // D3
    output wire p12,  // Q4
    input  wire p13,  // D4
    input  wire p14,  // D5
    output wire p15,  // Q5
    input  wire p16// (no name in EclDict)
);

  reg ck_d;
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;

  reg [5:0] q;
  always @(posedge sys_clk) begin
    if (!p1)        q <= 6'd0;                        // CLR', asynchronous
    else if (ck_en) q <= {p14, p13, p11, p6, p4, p3}; // D6..D1
  end

  assign {p15, p12, p10, p7, p5, p2} = q;             // Q6..Q1

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};         // GND, VCC
endmodule

`default_nettype wire
