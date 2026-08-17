// cell_SN74LS259 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// 8-Bit Addressable Latch (TTL). TtlDict:
//   a,CL',15 > a,D,13 > a,EN',14 > a,S1,1 > a,S2,2 > a,S4,3
//   a,Q0,4 > a,Q1,5 > a,Q2,6 > a,Q3,7 > a,Q4,9 > a,Q5,10 > a,Q6,11 > a,Q7,12
//
// With the enable low the addressed latch follows D; clear is active low and
// resets all eight. Only the pins the board wires are declared.

// EDGE-TRIGGERED on the fabric clock with the latch enable as an ENABLE, the
// convention this design uses for every clocked element.
//
// The part is a TRANSPARENT latch: while its enable is asserted the output
// follows the input, which is a combinational path straight through it and
// closes a loop wherever the machine feeds a latch from something downstream
// of it. sys_clk heavily oversamples every signal on the board, so the
// behaviour while transparent is the same to a fabric clock's precision -- and
// unlike a latch it synthesises.

`default_nettype none

module cell_SN74LS259 (
    input  wire sys_clk,
    input  wire p1,  // S1
    input  wire p2,  // S2
    input  wire p3,  // S4
    output wire p5,  // Q1
    output wire p7,  // Q3
    input  wire p8,  // (no name in EclDict)
    input  wire p13,  // D
    input  wire p14,  // EN'
    input  wire p15,  // CL'
    input  wire p16// (no name in EclDict)
);

  wire [2:0] sel = {p3, p2, p1};        // S4, S2, S1

  reg [7:0] q;
  always @(posedge sys_clk) begin
    if (!p15)      q <= 8'd0;           // CL'
    else if (!p14) q[sel] <= p13;       // EN' low: the addressed latch follows D
  end

  assign p5 = q[1];  assign p7 = q[3];

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
