// cell_SN74LS169 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// Synchronous 4-Bit Up/Down Counter (TTL). TtlDict:
//   a,UD,1 > a,CK,2 > a,LD',9 > a,EP',7 > a,ET',10
//   a,B0,6 > a,B1,5 > a,B2,4 > a,B3,3      parallel data, MSB-first: PARC's
//                                          B0 is pin 6, the sheet's D
//   a,H0,11 > a,H1,12 > a,H2,13 > a,H3,14  outputs, H0 the most significant
//   a,RC',15                               ripple carry out
//
// `UD` high counts UP. Load is SYNCHRONOUS, and takes priority over counting;
// both enables must be low to count. RC' goes low when ET' is low and the
// counter is at terminal count -- 15 counting up, 0 counting down -- and it is
// a GATE, which is what lets the next stage in a chain see the carry on the
// same edge. (Do not read TtlDict's `[FF ... >15 : CLK 2 (3.0 30.8)]` as
// saying otherwise: that is a timing arc from the clock, and the 30.8 ns
// against 16.5 for the Q outputs IS this gate. See cell_F10016.)
//
// On DskEth only RC' is wired: four of these cascade as a divider and nothing
// reads the count, which is why no Q pin is declared.

// FPGA: on `sys_clk` with the part's own clock as an ENABLE -- see
// cell_MC10176.

`default_nettype none

module cell_SN74LS169 (
    input  wire sys_clk,
    input  wire p1,  // UD
    input  wire p2,  // CK
    input  wire p3,  // B3
    input  wire p4,  // B2
    input  wire p5,  // B1
    input  wire p6,  // B0
    input  wire p7,  // EP'
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // LD'
    input  wire p10,  // ET'
    output wire p15,  // RC'
    input  wire p16// (no name in EclDict)
);

  wire [3:0] d = {p6, p5, p4, p3};        // B0..B3 = MSB..LSB

  reg ck_d;
  always @(posedge sys_clk) ck_d <= p2;
  wire ck_en = p2 & ~ck_d;

  // THE ENABLES AND THE LOAD ARE READ IN THEIR SETUP WINDOW, not at the edge.
  // DskEth's subsector counters (a01/a02/a06/a07) take CK, EP' and ET' from
  // ONE net -- the unit's sector pulse, active low -- so at the rising edge
  // that clocks them the enables are rising too. The real part counts there
  // because it sampled them low a setup time earlier; reading them at the
  // edge, as this cell did, never counts (2026-09-04: one sector wakeup in
  // seven revolutions). One sys_clk of history is that setup time.
  reg p7_d = 1'b1, p9_d = 1'b1, p10_d = 1'b1;
  always @(posedge sys_clk) begin p7_d <= p7; p9_d <= p9; p10_d <= p10; end
  reg [3:0] q = 4'd0;
  always @(posedge sys_clk)
    if (ck_en) begin
      if (!p9_d)              q <= d;               // LD', synchronous
      else if (!p7_d && !p10_d)   q <= p1 ? q + 4'd1    // UD high counts up
                                      : q - 4'd1;
    end

  // RC' -- active low at terminal count, gated by ET'.
  assign p15 = ~(~p10 & (p1 ? (q == 4'hF) : (q == 4'h0)));

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
