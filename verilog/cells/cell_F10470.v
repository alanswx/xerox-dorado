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

`default_nettype none

module cell_F10470 (
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
  wire [11:0] a = {p14, p13, p12, p11, p10, p8, p7, p6, p5, p4, p3, p2};
  always @* if (!p16 && !p15) mem[a] = p17;                 // async write
  assign p1 = (!p16) ? mem[a] : 1'b0;


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p9, 1'b0};

endmodule

`default_nettype wire
