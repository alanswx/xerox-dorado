// cell_SN7438 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// Quad 2-Input NAND BUFFER with OPEN-COLLECTOR outputs (TTL).
// TtlDict groups this pinout as
//   a,IN,1,2 > a,OUT,3   b,IN,4,5 > b,OUT,6
//   c,IN,9,10 > c,OUT,8  d,IN,12,13 > d,OUT,11
// Only the gates the board wires are declared.
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

module cell_SN7438 (
    input  wire p1,  // a_IN0
    input  wire p2,  // a_IN1
    output wire p3,  // a_OUT
    input  wire p4,  // b_IN0
    input  wire p5,  // b_IN1
    output wire p6,  // b_OUT
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // c_OUT
    input  wire p9,  // c_IN0
    input  wire p10,  // c_IN1
    output wire p11,  // d_OUT
    input  wire p12,  // d_IN0
    input  wire p13,  // d_IN1
    input  wire p14// (no name in EclDict)
);

  assign p3  = ~(p1  & p2);
  assign p6  = ~(p4  & p5);
  assign p8  = ~(p9  & p10);
  assign p11 = ~(p12 & p13);

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
