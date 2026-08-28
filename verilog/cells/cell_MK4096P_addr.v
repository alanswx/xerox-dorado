// cell_MK4096P_addr -- the row/column address latch, hoisted out of the DRAM.
//
// NOT A PART. This is the multiplexed-address front end that a 4K x 1 dynamic
// RAM has inside it -- row latched on RAS' falling, column on CAS' falling --
// factored out so that chips DRIVEN BY THE SAME STROBES AND THE SAME ADDRESS
// PINS share one copy of it.
//
// WHY, with the numbers. `cell_MK4096P_6` used to carry its own `row[6:0]`,
// `col[6:0]`, `ras_d` and `cas_d`: sixteen registers, times the 144 chips of
// one storage module, times however many modules a machine has. Grouping the
// msa board's 144 instances by their FULL control tuple -- RAS', CAS' and all
// seven address pins -- gives exactly SIXTEEN distinct groups (12 chips in
// each of the largest), so 128 of the 144 copies are redundant. That is 2,048
// registers on one board, and the storage module is what took the DE10-Nano
// fit from 82% to 97% of its ALMs.
//
// It is a faithful factoring, not an approximation: every chip in a group
// sees identical inputs on every one of those nine pins, so the latch they
// would each compute is bit-for-bit the one they now share. Nothing about the
// ARRAY moves -- each chip keeps its own `mem`, and the benches that seed
// `b_msa.u_b05.mem[...]` are untouched.
//
// The address ORDER is the board's, read off its own net names and stated in
// cell_MK4096P_6: A0'=13 A1'=5 A2'=7 A3'=6 A4'=12 A5'=11 A6'=10.
//
// Grouping is done by tools/sil_to_verilog.py, which emits one of these per
// group and passes its output to the members' `addr_i`.

`default_nettype none

module cell_MK4096P_addr (
    input  wire        sys_clk,
    input  wire        p4,          // RAS'
    input  wire        p15,         // CAS'
    input  wire  [6:0] a,           // A6..A0, as the DRAM cell assembles them
    output wire [11:0] addr
);

  reg [6:0] row, col;
  reg       ras_d, cas_d;

  // The strobes are oversampled on sys_clk and used as one-cycle enables --
  // the same transform as everywhere else in this machine, and the reason a
  // level-sensitive DRAM write can land in block RAM at all.
  always @(posedge sys_clk) begin ras_d <= p4; cas_d <= p15; end
  always @(posedge sys_clk) begin
    if (~p4  & ras_d) row <= a;                      // RAS' falling
    if (~p15 & cas_d) col <= a;                      // CAS' falling
  end

  // {row, col} is fourteen bits and the array is 4K, so the low twelve are
  // the ones that address it. Kept exactly as the cell computed it.
  wire [13:0] full = {row, col};
  assign addr = full[11:0];

endmodule

`default_nettype wire
