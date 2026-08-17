// cell_F10145A -- 16 x 4 Register File (64-bit RAM)
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 105 package
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

module cell_F10145A (
    input  wire sys_clk,
    input  wire p10,
    input  wire p9,
    input  wire p7,
    input  wire p6,
    input  wire p5,
    input  wire p4,
    input  wire p11,
    input  wire p12,
    input  wire p3,
    input  wire p13,
    output wire p2,
    output wire p1,
    output wire p15,
    output wire p14,
    input  wire p8
);

  // A0-A3 = 10,9,7,6 address; D0-D3 = 5,4,11,12 data in;
  // Q0-Q3 = 2,1,15,14 data out; CE'=3 chip enable, WE'=13 write enable.
  reg [3:0] mem [0:15];
  wire [3:0] a = {p6, p7, p9, p10};
  always @(posedge sys_clk)
    if (!p3 && !p13) mem[a] <= {p12, p11, p4, p5};          // WE' and CE'
  wire [3:0] q = mem[a];
  assign {p14, p15, p1, p2} = (!p3) ? q : 4'b0000;          // CE' gates the read


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p8, 1'b0};

endmodule

`default_nettype wire
