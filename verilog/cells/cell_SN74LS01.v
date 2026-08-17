// cell_SN74LS01 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// Quad 2-Input NAND with OPEN-COLLECTOR outputs (TTL). TtlDict:
//   a,IN,2,3 > a,OUT,1   b,IN,5,6 > b,OUT,4
//   c,IN,8,9 > c,OUT,10  d,IN,11,12 > d,OUT,13
//
// BaseBd j17 gate c holds `BootMC'`, the 6502's reset: with the gate
// unmodelled that net sat low and the processor never started.
//
// OPEN COLLECTOR, and that is not in the dictionary: TtlDict groups parts by
// PINOUT, so this sits with the totem-pole '02 and '28, and only the part
// NUMBER says the output is a pull-down with no pull-up. It matters because
// the whole FPGA transformation here resolves a shared net as an OR of its
// drivers -- right for MECL emitter followers, which pull UP -- and these
// pull DOWN onto a net a resistor holds high, which is an AND.
//
// The cell drives the gate's true function, which is what a single
// open-collector output plus its pull-up computes. Where several are tied
// together the generator resolves them with an AND; see WIRED_AND.

`default_nettype none

module cell_SN74LS01 (
    output wire p1,  // a_OUT
    input  wire p2,  // a_IN0
    input  wire p3,  // a_IN1
    output wire p4,  // b_OUT
    input  wire p5,  // b_IN0
    input  wire p6,  // b_IN1
    input  wire p7,  // (no name in EclDict)
    input  wire p8,  // c_IN0
    input  wire p9,  // c_IN1
    output wire p10,  // c_OUT
    input  wire p14// (no name in EclDict)
);

  assign p1  = ~(p2 & p3);
  assign p4  = ~(p5 & p6);
  assign p10 = ~(p8 & p9);

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
