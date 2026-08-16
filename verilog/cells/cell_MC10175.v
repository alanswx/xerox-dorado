// cell_MC10175 -- Quint Latch
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 18 package position(s).

`default_nettype none

module cell_MC10175 (
    input  wire p10,
    input  wire p12,
    input  wire p13,
    input  wire p9,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p11,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    output wire p4
);

  // D0-D4 = 10,12,13,9,5; Q0-Q4 = 14,15,2,3,4; CC = 6,7 clock (two pins,
  // wire-ORed); MR=11 master reset.
  wire clk = p6 | p7;
  reg [4:0] q;
  always @(posedge clk or posedge p11) begin
    if (p11) q <= 5'd0;
    else     q <= {p5, p9, p13, p12, p10};
  end
  assign {p4, p3, p2, p15, p14} = q;


endmodule

`default_nettype wire
