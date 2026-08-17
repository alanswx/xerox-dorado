// cell_SN74LS163 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Synchronous 4-Bit Counter with SYNCHRONOUS clear (TTL). TtlDict:
//
//   a,B0,6 > a,B1,5 > a,B2,4 > a,B3,3   parallel data, MSB-first as ever:
//                                       PARC's B0 is pin 6, the sheet's D
//   a,EP,7 > a,ET,10                    the two count enables
//   a,CL',1 > a,CK,2 > a,LD',9          clear, clock, load
//   a,CO,15                             carry out
//   a,H0,11 > a,H1,12 > a,H2,13 > a,H3,14   outputs, H0 the most significant
//
// Priority is clear, then load, then count -- and on the '163 the clear is
// SYNCHRONOUS, which is the whole difference from the '161.
//
// BaseBd e07 is one of these: clocked by the crystal oscillator, its carry
// out `Max` clocks g08 to make the 6502's `MCPreClk`.

// FPGA: on `sys_clk` with the part's own clock as an ENABLE, and the
// asynchronous clear level-tested on the same edge -- see cell_MC10176.

`default_nettype none

module cell_SN74LS163 (
    input  wire sys_clk,
    input  wire p1,  // CL'
    input  wire p2,  // CK
    input  wire p3,  // B3
    input  wire p4,  // B2
    input  wire p5,  // B1
    input  wire p6,  // B0
    input  wire p7,  // EP
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // LD'
    input  wire p10,  // ET
    output wire p15,  // CO
    input  wire p16// (no name in EclDict)
);

  wire [3:0] d = {p6, p5, p4, p3};        // B0..B3 = MSB..LSB

  reg ck_d;
  always @(posedge sys_clk) ck_d <= p2;
  wire ck_en = p2 & ~ck_d;

  reg [3:0] q;
  always @(posedge sys_clk)
    if (ck_en) begin
      if (!p1)            q <= 4'd0;        // CL', synchronous
      else if (!p9)       q <= d;           // LD'
      else if (p7 & p10)  q <= q + 4'd1;    // EP and ET
    end

  // Only the carry out is wired: e07 divides the crystal down and hands
  // `Max` to the flip-flop that makes the 6502's clock. The four count
  // outputs go nowhere, which is why they are not declared.
  assign p15 = p10 & (q == 4'hF);           // CO, gated by ET

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
