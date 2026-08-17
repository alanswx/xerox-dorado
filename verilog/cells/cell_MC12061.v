// cell_MC12061 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Crystal Oscillator (MECL). EclDict names its pins:
//   Xtal1,5  Xtal2,6   the crystal
//   EclQ,13  EclQ',12  ECL outputs      TTtlQ,10  the TTL-level output
//   SINo,2   SINo',3   SINi,15  SINi'   the sustaining-amplifier loop
//
// A SUBSTITUTION, like the VCO on the same board: a crystal has no digital
// behaviour, and an FPGA takes its clock from a pin or a PLL. So this is a
// divider off the fabric clock.
//
// Everything downstream is authentic. `TTtlQ` clocks the SN74LS163 at e07,
// whose carry out `Max` clocks the SN74LS74 at g08, which makes `MCPreClk` --
// the 6502's clock. The sustaining loop (SINo/SINi) is driven low rather than
// modelled: it is the analog half, and resolving it as logic would latch.

`default_nettype none

module cell_MC12061 (
    input  wire sys_clk,
    input  wire p1,  // VCC1
    output wire p2,  // SINo
    output wire p3,  // SINo'
    input  wire p4,  // AGC
    input  wire p5,  // Xtal1
    input  wire p6,  // Xtal2
    input  wire p7,  // BIAS
    input  wire p8,  // VEE1
    input  wire p9,  // TVEE
    output wire p10,  // TTtlQ
    input  wire p11,  // TVCC
    input  wire p14,  // SINi'
    input  wire p15,  // SINi
    input  wire p16// EVCC
);

  reg [2:0] osc_div = 3'd0;
  always @(posedge sys_clk) osc_div <= osc_div + 3'd1;
  wire osc = osc_div[2];

  assign p10 =  osc;      // TTtlQ, to the LS163
  assign p2  = 1'b0;      // SINo  -- the sustaining loop, not modelled
  assign p3  = 1'b0;      // SINo'

  wire _unused = &{1'b0, p1, p4, p5, p6, p7, p8, p9, p11, p14, p15, p16, 1'b0};
endmodule

`default_nettype wire
