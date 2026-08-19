// cell_F10415A -- 1024 x 1 Random Access Memory
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 174 package
// position(s) across the sixteen Dorado boards.
//
// RAM INFERENCE. ECL RAMs are ASYNCHRONOUS -- no clock, write is level
// sensitive on WE'. Written that way here because that is the part, and it
// simulates correctly. On an FPGA an async-write array does NOT infer block
// RAM: Quartus will give distributed/LUT RAM or registers, which for the
// larger arrays here is not viable. When the design goes to Quartus this
// wants either an altsyncram instance or a wrapper that recovers a clock
// edge from WE'. Deferred deliberately -- see verilog/README.md.

// SYNCHRONOUS WRITE on the fabric clock, with the part's own write level as an
// enable -- the convention this whole design uses for a clocked element, and
// the same change the two DRAM cells already carry.
//
// The part's write is ASYNCHRONOUS: it is level-sensitive on the write and
// chip enables, so while they are asserted the stored bit follows the data
// input. That makes a COMBINATIONAL LOOP wherever the machine writes back
// something it computed from the same location -- read, gate, write, read --
// which is most of a datapath. A latch is also not synthesisable on an FPGA;
// a registered write infers memory. The READ stays combinational, which is
// what the machine's timing assumes and what distributed RAM gives you.

`default_nettype none

module cell_F10415A (
    input  wire sys_clk,
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p15,
    input  wire p14,
    input  wire p13,
    output wire p1
);

  // A0-A9 = 2,3,4,5,6,7,9,10,11,12; DI=15, DO=1, CE'=14, WE'=13.
  reg mem [0:1023];
  //
  // ADDRESS BIT ORDER: A0 IS THE MOST SIGNIFICANT. PARC numbers every field
  // MSB-first -- `RSTK.0` is the top RSTK bit, `TNIA.04` the top address bit --
  // and the dictionary's A-numbering follows suit. `cell_F10414`, the same
  // family on the same board, says so in its own comment ("A0..A7, MSB
  // first"), and `cell_i2716` was corrected for exactly this ("PARC names
  // those MSB-first: the dictionary's A0 is pin 19, the sheet's A10 ... a
  // reversed address would have scrambled the image silently").
  //
  // For THIS part it is not a convention argument, it is measured. The board
  // wires pin 2 to `RA.01a` and pin 12 to `RA.10a`; `RA.01a` comes from
  // `dRA.01'`, which ContB's multiplexer takes from `TNIA.05` -- the SECOND
  // MOST significant address bit -- while `RA.10a` traces to `TNIA.14`, the
  // second least. So pin 2 carries the more significant bit. Reversed, a
  // Write-IM at address 195 deposited at 780, which is 195 with its ten bits
  // turned round.
  wire [9:0] a = {p2, p3, p4, p5, p6, p7, p9, p10, p11, p12};
  always @(posedge sys_clk)
    if (!p14 && !p13) mem[a] <= p15;                        // CE' and WE'
  assign p1 = (!p14) ? mem[a] : 1'b0;


endmodule

`default_nettype wire
