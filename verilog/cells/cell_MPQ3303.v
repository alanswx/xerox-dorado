// cell_MPQ3303 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// A TRANSISTOR QUAD, and the machine's master oscillator. The single
// package (BaseBd h06) sits in a relaxation loop with the MC1660 at h05:
// `VCOPhase0`/`VCOPhase1` are both driven by the quad AND by that gate, and
// the control voltages `VCOCtrl0`/`VCOCtrl1` arrive from a cable. It is an
// analog VCO.
//
// THIS IS A SUBSTITUTION, not a model, and it is the one place in the design
// where that is true. An analog oscillator has no digital behaviour to
// transcribe -- resolved as logic the loop simply latches -- and an FPGA has
// no VCO either: a clock comes from a pin or a PLL. So the part is replaced
// by a divider off the fabric clock, which is what the FPGA would use.
//
// Everything downstream is then AUTHENTIC: h05 shapes the phases, g05 makes
// the two anti-phase clocks, and the four MC1690s divide them into
// `StartClockPulse'` and `EndClockPulse` exactly as the board does.
//
// Divide by four so the MC1690 stages, which detect an EDGE of their clock
// under oversampling, always see one.
//
// The quad ALSO carries `PwrGood`, and that is not part of the oscillator: it
// is the comparator that says the supplies have reached their thresholds. It
// is the machine's POWER-ON RESET -- `PwrGood` is the clear input of the
// flip-flop at j08 that produces `MCReset'` -- so it has to come up LOW and
// rise, the way a supply ramps. Asserting it true from the first cycle, which
// is what this cell did, released the 6502 before it had run a reset sequence
// at all: it came up executing BRK from an all-zero data bus and never
// fetched a reset vector.

`default_nettype none

module cell_MPQ3303 (
    input  wire sys_clk,
    output wire p1,  // (no name in EclDict)
    input  wire p2,  // (no name in EclDict)
    output wire p3,  // (no name in EclDict)
    output wire p5,  // (no name in EclDict)
    input  wire p6,  // (no name in EclDict)
    output wire p7,  // (no name in EclDict)
    output wire p8,  // (no name in EclDict)
    input  wire p9,  // (no name in EclDict)
    output wire p10,  // (no name in EclDict)
    output wire p12,  // (no name in EclDict)
    input  wire p13,  // (no name in EclDict)
    output wire p14// (no name in EclDict)
);

  reg [1:0] vco = 2'd0;
  always @(posedge sys_clk) vco <= vco + 2'd1;
  wire osc = vco[1];

  assign p7 =  osc;     // VCOPhase0
  assign p1 = ~osc;     // VCOPhase1

  // PwrGood. POR_CYCLES is in fabric clocks and only has to be long against
  // the 6502's clock, which the board divides down from the crystal; the real
  // comparator trips tens of milliseconds after power is applied, which is
  // enormous by comparison.
  localparam POR_CYCLES = 4096;
  reg [15:0] por = 16'd0;
  always @(posedge sys_clk) if (por != POR_CYCLES[15:0]) por <= por + 16'd1;
  assign p14 = (por == POR_CYCLES[15:0]);

  // The quad's remaining collectors go to ground or to the power-sequencing
  // cable; nothing in the digital design reads them as logic.
  assign p3 = 1'b0, p5 = 1'b0, p8 = 1'b0, p10 = 1'b0, p12 = 1'b0;

  wire _unused = &{1'b0, p2, p6, p9, p13, 1'b0};
endmodule

`default_nettype wire
