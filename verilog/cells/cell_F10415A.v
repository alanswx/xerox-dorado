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
  wire [9:0] a = {p12, p11, p10, p9, p7, p6, p5, p4, p3, p2};
  always @(posedge sys_clk)
    if (!p14 && !p13) mem[a] <= p15;                        // CE' and WE'
  assign p1 = (!p14) ? mem[a] : 1'b0;


endmodule

`default_nettype wire
