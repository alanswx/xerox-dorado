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

`default_nettype none

module cell_F10415A (
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
  always @* if (!p14 && !p13) mem[a] = p15;                 // async write
  assign p1 = (!p14) ? mem[a] : 1'b0;


endmodule

`default_nettype wire
