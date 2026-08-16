// cell_MC1668 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 12 package position(s) across the sixteen boards.
//
// Dual clocked flip-flop with asynchronous Set and Reset (MECL III). EclDict
// names every pin -- S, R, C, D, Q, Q' for each half -- so the function is
// unambiguous without a datasheet, and there is no OR/NOR polarity question.
//
// MECL S/R are ACTIVE HIGH and dominate the clock, as cell_MC10231 records
// for the 10K equivalent; the two parts have the same shape and the same
// treatment here.
//
// FPGA: on `sys_clk` with C as an enable, like every other clocked cell --
// see cell_MC10176 for why.

`default_nettype none

module cell_MC1668 (
    input  wire sys_clk,
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p4,  // R
    input  wire p5,  // S
    input  wire p6,  // D
    input  wire p7,  // C
    input  wire p9,  // C
    input  wire p11,  // D
    input  wire p12,  // S
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15// Q
);

  reg cka_d, ckb_d;
  always @(posedge sys_clk) begin cka_d <= p7; ckb_d <= p9; end
  wire cka_en = p7 & ~cka_d;
  wire ckb_en = p9 & ~ckb_d;

  reg qa, qb;
  always @(posedge sys_clk) begin
    if (p5)          qa <= 1'b1;      // S
    else if (p4)     qa <= 1'b0;      // R
    else if (cka_en) qa <= p6;        // D
  end
  always @(posedge sys_clk) begin
    if (p12)         qb <= 1'b1;
    else if (p13)    qb <= 1'b0;
    else if (ckb_en) qb <= p11;
  end

  assign p2 =  qa;  assign p3  = ~qa;
  assign p15 = qb;  assign p14 = ~qb;
endmodule

`default_nettype wire
