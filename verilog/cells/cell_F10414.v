// cell_F10414 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// 256 x 1 RAM (Fairchild F10K). EclDict states it as a memory:
//
//   [M 13 {B-1.0 2.0}>15 : CLK 14 (...) ADDR (1 2 3 4 9 10 11 12) (...)
//                          CE (5 6 7) (...)]
//
// so pin 13 is the data in, pin 15 the data out, pin 14 the write, the eight
// address pins are A0..A7 = 1, 2, 3, 4, 9, 10, 11, 12 -- MSB-first, as PARC
// numbers everything -- and the three chip selects are 5, 6, 7.
//
// The board wires only CS2' (pin 7); the other two are tied low, so they are
// not declared here.
//
// Like its 1024 x 1 sibling F10415A: the write is level-sensitive on the real
// part and is SYNCHRONOUS here, on `sys_clk` with the part's own levels as
// enables. A transparent write makes every read-modify-write path in the
// machine a combinational loop, and a latch does not synthesise; the read
// stays combinational, which is what the timing assumes and what distributed
// RAM gives you. An ECL RAM's open-emitter output goes low when deselected.

`default_nettype none

module cell_F10414 (
    input  wire sys_clk,
    input  wire p1,  // A0
    input  wire p2,  // A1
    input  wire p3,  // A2
    input  wire p4,  // A3
    input  wire p7,  // CS2'
    input  wire p9,  // A4
    input  wire p10,  // A5
    input  wire p11,  // A6
    input  wire p12,  // A7
    input  wire p13,  // b_IN0
    input  wire p14,  // WE'
    output wire p15// b_OUT
);

  wire [7:0] a  = {p1, p2, p3, p4, p9, p10, p11, p12};   // A0..A7, MSB first
  wire       sel = ~p7;                                  // CS2'

  reg mem [0:255];
  always @(posedge sys_clk)
    if (sel && !p14) mem[a] <= p13;                      // WE'

  assign p15 = sel ? mem[a] : 1'b0;
endmodule

`default_nettype wire
