// cell_F10470 -- 4096 x 1 Random Access Memory
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 72 package
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

module cell_F10470 (
    input  wire sys_clk,
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p8,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p17,
    input  wire p16,
    input  wire p15,
    output wire p1,
    input  wire p9
);

  // A0-A11 = 2..8,10..14; DI=17, DO=1, CE'=16, WE'=15.
  reg mem [0:4095];
  //
  // ADDRESS BIT ORDER: A0 IS THE MOST SIGNIFICANT. PARC numbers every field
  // MSB-first -- `RSTK.0` is the top RSTK bit, `TNIA.04` the top address bit --
  // and the dictionary's A-numbering follows suit. `cell_F10414`, the same
  // family on the same board, says so in its own comment ("A0..A7, MSB
  // first"), and `cell_i2716` was corrected for exactly this ("PARC names
  // those MSB-first: the dictionary's A0 is pin 19, the sheet's A10 ... a
  // reversed address would have scrambled the image silently").
  wire [11:0] a = {p2, p3, p4, p5, p6, p7, p8, p10, p11, p12, p13, p14};
  always @(posedge sys_clk)
    if (!p16 && !p15) mem[a] <= p17;                        // CE' and WE'
  assign p1 = (!p16) ? mem[a] : 1'b0;


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p9, 1'b0};

endmodule

`default_nettype wire
