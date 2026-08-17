// cell_SN74LS175 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// Quad D Flip-Flop with clear (TTL). TtlDict:
//   a,D0,4 > a,D1,5 > a,D2,12 > a,D3,13 > a,CK,9 > a,CL',1
//   a,Q0,2 > a,Q0',3 > a,Q1,7 > a,Q1',6 > a,Q2,10 > a,Q2',11 > a,Q3,15 > a,Q3',14
// Clear is active low and asynchronous; both senses of each output are
// brought out.
//
// Only the pins the BOARDS wire are declared here, which is why the D2/D3
// halves are absent.

// FPGA: on `sys_clk` with the part's own clock as an ENABLE, and the
// asynchronous clear level-tested on the same edge -- see cell_MC10176.

`default_nettype none

module cell_SN74LS175 (
    input  wire sys_clk,
    input  wire p1,  // CL'
    output wire p2,  // Q0
    input  wire p4,  // D0
    input  wire p5,  // D1
    output wire p7,  // Q1
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CK
    input  wire p16// (no name in EclDict)
);

  reg ck_d;
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;

  reg [1:0] q;
  always @(posedge sys_clk) begin
    if (!p1)        q <= 2'd0;          // CL'
    else if (ck_en) q <= {p5, p4};      // D1, D0
  end

  assign p2 = q[0];  assign p7 = q[1];   // Q0, Q1 -- the only two wired

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
