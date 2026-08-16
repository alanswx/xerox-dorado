// cell_i2125 -- 1024 x 1 Static RAM (Intel 2125 / F93415A / F93425A)
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 32 package position(s).
//
// RAM INFERENCE: async write, same caveat as the other memories -- will not
// infer block RAM under Quartus without an altsyncram or a clock-recovery
// wrapper. See verilog/README.md.

`default_nettype none

module cell_i2125 (
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p1,
    input  wire p15,
    input  wire p14,
    output wire p7,
    input  wire p8,
    input  wire p16
);

  // A0-A9 = 2,3,4,5,6,9,10,11,12,13; CS'=1, DI=15, WE'=14, DO=7.
  reg mem [0:1023];
  wire [9:0] a = {p13,p12,p11,p10,p9,p6,p5,p4,p3,p2};
  always @* if (!p1 && !p14) mem[a] = p15;
  assign p7 = (!p1) ? mem[a] : 1'b0;


  wire _unused_pins = &{1'b0, p8, p16, 1'b0};

endmodule

`default_nettype wire
