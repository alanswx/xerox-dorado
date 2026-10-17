// cell_MK4096P_addr -- the row/column address latch, hoisted out of the DRAM.
//
// NOT A PART. This is the multiplexed-address front end that a 16K x 1 dynamic
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

// ROW_BITS=8 selects the extra multiplexed address line for 64K chips.
module cell_MK4096P_addr #(parameter integer ROW_BITS = 7) (
    input  wire        sys_clk,
    input  wire        p4,          // RAS'
    input  wire        p15,         // CAS'
    input  wire  [ROW_BITS-1:0] a,
    output wire [2*ROW_BITS-1:0] addr
);

  reg [ROW_BITS-1:0] row, col;
  reg       ras_d, cas_d;

  // The strobes are oversampled on sys_clk and used as one-cycle enables --
  // the same transform as everywhere else in this machine, and the reason a
  // level-sensitive DRAM write can land in block RAM at all.
  always @(posedge sys_clk) begin ras_d <= p4; cas_d <= p15; end
  always @(posedge sys_clk) begin
    if (~p4  & ras_d) row <= a;                      // RAS' falling
    if (~p15 & cas_d) col <= a;                      // CAS' falling
  end

  // {row, col} is fourteen bits, and ALL FOURTEEN address the array. This
  // used to keep the low twelve ("the array is 4K"), dropping the two top
  // ROW bits -- but a 4K x 1 DRAM has six address pins and the board wires
  // seven (A0'..A6'), and HM p.10 sizes a module as "256k x 16-bit ...
  // (using 16k-bit storage chips)". Seven pins latched twice is a 16K part.
  // With two bits dropped, storage folded 4:1 and a word written to page p
  // was read back from page p+4096's cell. Fixed 2026-09-08 with the same
  // defect in cell_MosRam (the map).
  assign addr = {row, col};

endmodule

`default_nettype wire
