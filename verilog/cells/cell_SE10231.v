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


  // THE COMMON CLOCK, pin 9 (CC), was ignored here until 2026-08-16 -- the
  // same class of bug Tim found on the combinational cells, and the reason
  // tools/sil_check_cells.py now reads the dictionary's [FF ...] entries too:
  //   MC231/SE231  [FF 10 ...>(14 15) : CLK (9 11) ...]   [FF 7 ...>(2 3) : CLK (9 6) ...]
  // Pin 9 is in BOTH clocks.
  //
  // The data book (DL122 rev 7, MC10231) says how the two combine: "Each
  // flip-flop may be clocked separately by holding the common clock in the
  // low state and using the enable inputs for the clocking function. If the
  // common clock is to be used to clock the flip-flop, the Clock Enable
  // inputs must be in the low state." Either can clock it while the other is
  // held low -- an OR.
  wire clka = p9 | p6;
  wire clkb = p9 | p11;

  reg cka_d, ckb_d;
  always @(posedge sys_clk) begin cka_d <= clka; ckb_d <= clkb; end
  wire cka_en = clka & ~cka_d;
  wire ckb_en = clkb & ~ckb_d;

  reg qa, qb;
  always @(posedge sys_clk)
    if (cka_en) qa <= p7;                        // D, no async on this half
  always @(posedge sys_clk) begin
    if (p13)         qb <= 1'b0;                 // R
    else if (ckb_en) qb <= p10;                  // D
  end

  assign p2  =  qa;  assign p3  = ~qa;
  assign p15 =  qb;  assign p14 = ~qb;

endmodule

`default_nettype wire
