// cell_SE10231 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 10 package position(s) across the sixteen boards.
//
// High-Speed Dual Type D Master-Slave Flip-Flop (Signetics second source for
// the Motorola MC10231 -- EclDict's alias table groups them). Same part, same
// treatment as cell_MC10231: MECL S/R are ACTIVE HIGH and dominate the clock.
//
// FEWER PINS THAN ITS TWIN, because a cell declares only what the BOARDS
// wire. This socket has no Set on either half and a Reset on the b half only
// (p13), so those are the only asynchronous inputs modelled. p9 is the CC
// common-clock pin, which the boards leave unused here.
//
// FPGA: on `sys_clk` with C as an enable -- see cell_MC10176.

`default_nettype none

module cell_SE10231 (
    input  wire sys_clk,
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p6,  // C
    input  wire p7,  // D
    input  wire p9,  // CC
    input  wire p10,  // D
    input  wire p11,  // C
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15// Q
);

  reg cka_d, ckb_d;
  always @(posedge sys_clk) begin cka_d <= p6; ckb_d <= p11; end
  wire cka_en = p6  & ~cka_d;
  wire ckb_en = p11 & ~ckb_d;

  reg qa, qb;
  always @(posedge sys_clk)
    if (cka_en) qa <= p7;                        // D, no async on this half
  always @(posedge sys_clk) begin
    if (p13)         qb <= 1'b0;                 // R
    else if (ckb_en) qb <= p10;                  // D
  end

  assign p2  =  qa;  assign p3  = ~qa;
  assign p15 =  qb;  assign p14 = ~qb;

  wire _unused = &{1'b0, p9, 1'b0};   // CC, unused on these boards
endmodule

`default_nettype wire
