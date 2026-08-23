// cell_MosRam -- Dynamic RAM socket (density set by board straps)
//
// No entry in PARC's EclDict or TtlDict, so the pinout is read from how the
// SIXTEEN BOARDS wire it -- which is unambiguous here, because the nets carry
// their own names: RAS'/CAS'/WE' strobes and A0'..A6' address lines.
// Used in 165 package position(s).
//
// RAM INFERENCE: this cell is FULLY SYNCHRONOUS and infers block RAM. The
// strobes are oversampled on `sys_clk` and used as one-cycle enables (see
// below), so there is no asynchronous write and no gated clock -- which is
// what an FPGA needs. An earlier version of this header said the opposite,
// that "async/strobe-driven write does not infer block RAM ... this wants an
// altsyncram or a clock-recovery wrapper", and pointed at verilog/README.md
// for a deferred discussion. That was true of the first draft and stopped
// being true when the strobes became enables; the header simply outlived it.

`default_nettype none

module cell_MosRam (
    input  wire sys_clk,
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
    output wire p14,
    input  wire p1,
    input  wire p8,
    input  wire p9,
    input  wire p16
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


  // FPGA: ONE CLOCK, and the strobes become ENABLES.
  //
  // The Dorado drives this part with RAS'/CAS' strobes off the memory board.
  // Taking those literally gives the fabric edge-triggered logic on
  // combinational nets, and a level-sensitive write that lands in registers
  // rather than block RAM -- 165 MosRam plus 144 MK4096 packages of it. So
  // the strobes are oversampled on `sys_clk` and used as one-cycle enables.
  reg ras_d, cas_d;
  always @(posedge sys_clk) begin ras_d <= p4; cas_d <= p15; end
  always @(posedge sys_clk) begin
    if (~p4  & ras_d) row <= a;                      // RAS' falling
    if (~p15 & cas_d) col <= a;                      // CAS' falling
  end

  wire [13:0] addr = {row, col};

  // Synchronous read and write, so this INFERS BLOCK RAM.
  always @(posedge sys_clk) begin
    if (!p4 && !p15 && !p3) mem[addr[11:0]] <= p2;
    if (!p4 && !p15 &&  p3) dout <= mem[addr[11:0]];
  end
  assign p14 = dout;

  // REFRESH IS NOT MODELLED. A real DRAM loses its contents without periodic
  // RAS-only cycles; this array does not, which is the right simplification
  // for RTL simulation (and for an FPGA, where the memory is static). If the
  // Dorado's refresh logic is ever under test, that logic is on the board --
  // it is only the CELL that ignores refresh.

  // The board names two pins `RamA0orVEE` (1) and `RamA1orVCC` (9): straps
  // that either supply extra address bits or tie to a rail, so one socket
  // takes several DRAM densities.
  //
  // MODELLED AT 4K, AND THAT IS PROBABLY WRONG FOR A LATE MACHINE.
  // MemProms.bcpl carries two DRAM TIMING proms, `4k-Mem` (MX4k-j14) and
  // `16k-Mem` (MX16k-j13), and its header records the switch:
  //
  //     "change change tomemx-16k-j13 from -j14. comment-out the memx-4k
  //      option.  September 26, 1979"
  //
  // The 4k call site is commented out in the source, so from late 1979 the
  // memory boards were built with 16K parts and only the 16K timing PROM was
  // blown. The two strap pins here are exactly how one socket takes both.
  // Raising this to 16K x 1 needs the extra address bits routed from those
  // straps, which is a board-level question -- flagged rather than guessed.


  // Board-wired pins not named above (power, density straps)
  wire _unused_pins = &{1'b0, p1, p8, p9, p16, 1'b0};

endmodule

`default_nettype wire
