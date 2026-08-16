// cell_MB7071H -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 70 package position(s) across the sixteen boards.
//
// 256 x 4 RAM, and it is the machine's REGISTER FILE. EclDict names every
// pin (A0-A7, WE', BS0'-BS3', and four data pairs b/c/d/e), and the boards
// say what it holds: ProcH h06 is addressed by `RbAdr.0-7` with `SelectRm'a`
// and `RbWrite'a` -- that is RM. i06 is `StkAdr.0-7` with `SelectStk'a` --
// that is STK, the emulator stack. Four packages per processor board give
// the 16-bit width, which is why ProcH and ProcL have four each.
//
// The other 62 packages are the same part doing the same job elsewhere:
// MemC's cache tags and DispM's colour tables.
//
// FPGA: SYNCHRONOUS WRITE, ASYNCHRONOUS READ -- which is exactly what a
// distributed/LUT RAM gives you, and what the part does. The write has to be
// clocked (no FPGA has a level-sensitive write port), so it lands on
// `sys_clk` like every other clocked cell here; the read stays combinational
// because the surrounding ECL logic reads within a microcycle.
//
// BIT ORDER does not need the MSB-first rule the PROMs follow: this array is
// written and read through the same address pins by the same board, so any
// self-consistent order is correct. A0 is taken as the most significant for
// consistency with the PROM cells.
//
// The four block selects are separate pins but the boards tie them together
// (ProcH h06 has SelectRm'a on all of 3,4,5,6). Active low, so the part is
// selected when all four are low.

`default_nettype none

module cell_MB7071H (
    input  wire sys_clk,
    input  wire p2,  // WE'
    input  wire p3,  // BS3'
    input  wire p4,  // BS2'
    input  wire p5,  // BS1'
    input  wire p6,  // BS0'
    input  wire p7,  // A0
    input  wire p8,  // A1
    input  wire p9,  // A2
    input  wire p10,  // A3
    input  wire p11,  // A4
    input  wire p12,  // A5
    input  wire p13,  // A6
    input  wire p14,  // A7
    input  wire p15,  // (no name in EclDict)
    input  wire p16,  // e_IN0
    input  wire p17,  // d_IN0
    input  wire p18,  // c_IN0
    input  wire p19,  // b_IN0
    output wire p20,  // e_OUT
    output wire p21,  // d_OUT
    output wire p22,  // c_OUT
    output wire p23// b_OUT
);

  wire [7:0] a   = {p7, p8, p9, p10, p11, p12, p13, p14};   // A0..A7
  wire       sel = ~(p3 | p4 | p5 | p6);                    // BS0'-BS3'
  wire       we  = ~p2;                                     // WE'

  reg [3:0] mem [0:255];
  always @(posedge sys_clk)
    if (sel & we) mem[a] <= {p16, p17, p18, p19};   // e, d, c, b

  wire [3:0] q = mem[a];
  assign {p20, p21, p22, p23} = sel ? q : 4'b0000; // e, d, c, b; ECL reads low

  wire _unused_pins = &{1'b0, p15, 1'b0};
endmodule

`default_nettype wire
