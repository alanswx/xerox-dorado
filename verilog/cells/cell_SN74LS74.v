// cell_SN74LS74 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// Dual D Flip-Flop with preset and clear (TTL). TtlDict names every pin:
//
//   a,D,2 > a,C,3 > a,R',1 > a,S',4 > a,Q,5 > a,Q',6
//   b,D,12 > b,C,11 > b,R',13 > b,S',10 > b,Q,9 > b,Q',8
//
// Preset and clear are ACTIVE LOW and asynchronous; the transfer is on the
// rising clock edge.
//
// Two of the three on the BaseBoard are why the 6502 can run at all: g08
// divides `Max` into `MCPreClk`, the processor's clock, and j08 turns
// `PwrGood` into `MCReset'`.

// FPGA: on `sys_clk` with the part's own clock as an ENABLE, and the
// asynchronous clear level-tested on the same edge -- see cell_MC10176.

`default_nettype none

module cell_SN74LS74 (
    input  wire sys_clk,
    input  wire p1,  // R'
    input  wire p2,  // D
    input  wire p3,  // C
    input  wire p4,  // S'
    output wire p5,  // Q
    output wire p6,  // Q'
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // Q'
    input  wire p10,  // S'
    input  wire p11,  // C
    input  wire p12,  // D
    input  wire p13,  // R'
    input  wire p14// (no name in EclDict)
);

  reg cka_d, ckb_d;
  always @(posedge sys_clk) begin cka_d <= p3; ckb_d <= p11; end
  wire cka_en = p3  & ~cka_d;
  wire ckb_en = p11 & ~ckb_d;

  reg qa, qb;
  always @(posedge sys_clk) begin
    if (!p4)         qa <= 1'b1;      // S' preset
    else if (!p1)    qa <= 1'b0;      // R' clear
    else if (cka_en) qa <= p2;        // D
  end
  always @(posedge sys_clk) begin
    if (!p10)        qb <= 1'b1;
    else if (!p13)   qb <= 1'b0;
    else if (ckb_en) qb <= p12;
  end

  assign p5 =  qa;  assign p6 = ~qa;
  assign p8 = ~qb;                       // Q' only: no board wires b's Q

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
