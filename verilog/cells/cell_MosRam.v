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

// MemX's 64K stuffing uses socket pin9 as the eighth address bit.
// Default14 preserves the archived 16K configuration.
module cell_MosRam #(parameter integer ADDR_BITS = 14) (
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
  reg [ADDR_BITS/2-1:0] row, col;
  // ONE BIT WIDE (the part is N x 1). This was `reg [11:0]` while storing a
  // single bit -- functionally right (the write zero-extends, the read takes
  // bit 0) and TWELVE TIMES the memory, across 309 packages. Quartus is the
  // one that notices: it could not fit the un-inferred ones into registers
  // and stopped Analysis & Synthesis outright.
  //
  // 16K DEEP, BECAUSE SEVEN ADDRESS PINS ARE WIRED. This was `mem [0:4095]`
  // with `addr[11:0]` -- a 4K x 1 part -- while the boards name A0'..A6' on
  // the socket, and a 4K x 1 DRAM (MK4096) has six address pins; the cell
  // was silently dropping the top two ROW bits. On MemX that is the map: a
  // 16K-entry map (MapIs16K, VirtualBanks = 100B in InitMem.mc) folded 4:1,
  // so page p and page p+4096 shared an entry and the later map write won.
  // Found 2026-09-08 while chasing InitMap's map walk. The two strap pins
  // (1 and 9, below) would make it 64K; they are unjumpered on MemX.
  reg mem [0:(1<<ADDR_BITS)-1];
  reg        dout;

  wire [ADDR_BITS/2-1:0] a = {(ADDR_BITS == 16 ? p9 : 1'b0),
                             p10, p11, p12, p6, p7, p5, p13};


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

  wire [ADDR_BITS-1:0] addr = {row, col};

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
  // Data-in strobes on the later falling edge of CAS' and WRITE'. Defer
  // the array update until the column latch above contains this cycle's
  // address, retaining the data sampled at the strobe.
  reg we_d, write_pending, write_data;
  always @(posedge sys_clk) begin
    we_d <= p3;
    write_pending <= !p4 && !p15 && !p3 && (cas_d || we_d);
    if (!p4 && !p15 && !p3 && (cas_d || we_d)) write_data <= p2;
  end
  always @(posedge sys_clk) begin
    dout_r <= mem[addr];
    if (write_pending) mem[addr] <= write_data;
  end
  always @(posedge sys_clk)
    if (!p4 && !p15 && p3) dout <= dout_r;
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
  // MODELLED AT 16K x 1, the size the seven wired address pins give.
  // MemProms.bcpl carries two DRAM TIMING proms, `4k-Mem` (MX4k-j14) and
  // `16k-Mem` (MX16k-j13), and its header records the switch:
  //
  //     "change change tomemx-16k-j13 from -j14. comment-out the memx-4k
  //      option.  September 26, 1979"
  //
  // The 4k call site is commented out in the source, so from late 1979 the
  // memory boards were built with 16K parts and only the 16K timing PROM was
  // blown. The two strap pins here are how one socket also takes a 64K
  // part. ADDR_BITS=16 is selected only by the explicit64K MemX
  // generator configuration, which routes RTMapAd.1 through those straps.


  // Board-wired pins not named above (power, density straps)
  wire _unused_pins = &{1'b0, p1, p8, p9, p16, 1'b0};

endmodule

`default_nettype wire
