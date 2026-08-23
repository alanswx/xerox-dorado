// cell_MK4096P_6 -- 4096 x 1 Dynamic RAM (MOSTEK MK4096)
//
// No entry in PARC's EclDict or TtlDict, so the pinout is read from how the
// SIXTEEN BOARDS wire it -- which is unambiguous here, because the nets carry
// their own names: RAS'/CAS'/WE' strobes and A0'..A6' address lines.
// Used in 144 package position(s).
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

module cell_MK4096P_6 (
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
    output wire p14
);

  // Standard 16-pin dynamic RAM: 7 MULTIPLEXED address lines, row latched on
  // the falling edge of RAS', column on the falling edge of CAS'.
  //
  // Address order is read off the BOARD's own net names, not assumed:
  // A0'=13 A1'=5 A2'=7 A3'=6 A4'=12 A5'=11 A6'=10.
  reg [6:0] row, col;
  // ONE BIT WIDE, because the part is 4K x 1. This was `reg [11:0]` while
  // storing a single bit -- functionally right (the write zero-extends, the
  // read takes bit 0) and TWELVE TIMES the memory, across 309 packages.
  // Quartus is the one that notices: it could not fit the un-inferred ones
  // into registers and stopped Analysis & Synthesis outright.
  reg mem [0:4095];
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
  // THE READ MUST BE UNCONDITIONAL FOR QUARTUS TO INFER M10K. This is the
  // canonical MiSTer dual-port template (see any project's rtl/dpram.sv):
  //
  //     always @(posedge clk) begin
  //         q <= mem[addr];                 // UNCONDITIONAL registered read
  //         if (wren) mem[addr] <= data;
  //     end
  //
  // and note it is INFERENCE, not an altsyncram instantiation -- which is why
  // the cell still simulates in Verilator unchanged. A CONDITIONAL registered
  // read is not the template, and Quartus put these 309 packages into
  // registers instead: 1.27 Mbit against a device with ~166K of them, ending
  // Analysis & Synthesis with "Cannot convert all sets of registers into RAM
  // megafunctions". Splitting the read and write into separate always blocks
  // was tried first and did NOT help; it is the condition on the read that
  // matters, not the block structure.
  //
  // The part's own behaviour is preserved by holding the output separately:
  // `dout_r` follows the addressed cell every cycle so the RAM template stays
  // clean, and `dout` only takes it during a read, as a real DRAM's output is
  // only valid then.
  reg dout_r;
  always @(posedge sys_clk) begin
    dout_r <= mem[addr[11:0]];
    if (!p4 && !p15 && !p3) mem[addr[11:0]] <= p2;
  end
  always @(posedge sys_clk)
    if (!p4 && !p15 && p3) dout <= dout_r;
  assign p14 = dout;

  // REFRESH IS NOT MODELLED. A real DRAM loses its contents without periodic
  // RAS-only cycles; this array does not, which is the right simplification
  // for RTL simulation (and for an FPGA, where the memory is static). If the
  // Dorado's refresh logic is ever under test, that logic is on the board --
  // it is only the CELL that ignores refresh.


endmodule

`default_nettype wire
