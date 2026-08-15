// cell_MK4096P_6 -- 4096 x 1 Dynamic RAM (MOSTEK MK4096)
//
// No entry in PARC's EclDict or TtlDict, so the pinout is read from how the
// SIXTEEN BOARDS wire it -- which is unambiguous here, because the nets carry
// their own names: RAS'/CAS'/WE' strobes and A0'..A6' address lines.
// Used in 144 package position(s).
//
// RAM INFERENCE: async/strobe-driven write does not infer block RAM. Quartus
// will produce distributed RAM or registers here, which at this density is
// not viable -- this wants an altsyncram or a clock-recovery wrapper when the
// design goes to Quartus. Deferred; see verilog/README.md.

`default_nettype none

module cell_MK4096P_6 (
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p15,
    output wire p14
);

  // Standard 16-pin dynamic RAM: 7 MULTIPLEXED address lines, row latched on
  // the falling edge of RAS', column on the falling edge of CAS'.
  //
  // Address order is read off the BOARD's own net names, not assumed:
  // A0'=13 A1'=5 A2'=7 A3'=6 A4'=12 A5'=11 A6'=10.
  reg [6:0] row, col;
  reg [11:0] mem [0:4095];
  reg        dout;

  wire [6:0] a = {p10, p11, p12, p6, p7, p5, p13};   // A6..A0

  always @(negedge p4)  row <= a;                      // RAS'
  always @(negedge p15) col <= a;                      // CAS'

  wire [13:0] addr = {row, col};

  // Write is level-sensitive while both strobes are low, as on the part.
  always @* if (!p4 && !p15 && !p3) mem[addr[11:0]] = p2;
  always @* if (!p4 && !p15 &&  p3) dout = mem[addr[11:0]];
  assign p14 = dout;

  // REFRESH IS NOT MODELLED. A real DRAM loses its contents without periodic
  // RAS-only cycles; this array does not, which is the right simplification
  // for RTL simulation (and for an FPGA, where the memory is static). If the
  // Dorado's refresh logic is ever under test, that logic is on the board --
  // it is only the CELL that ignores refresh.


endmodule

`default_nettype wire
