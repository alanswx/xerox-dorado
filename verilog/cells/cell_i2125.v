// cell_i2125 -- 1024 x 1 Static RAM (Intel 2125 / F93415A / F93425A)
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 32 package position(s).
//
// RAM INFERENCE: async write, same caveat as the other memories -- will not
// infer block RAM under Quartus without an altsyncram or a clock-recovery
// wrapper. See verilog/README.md.

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

module cell_i2125 (
    input  wire sys_clk,
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
  always @(posedge sys_clk)
    if (!p1 && !p14) mem[a] <= p15;                         // CS' and WE'
  // REGISTERED READ, so this infers M10K -- see cell_F10415A for the full
  // reasoning. Worth recording what these 32 packages cost when they did not:
  // ONE of them took 1,445 ALUTs built out of logic, and 32 of them were
  // essentially the whole BaseBoard's 47,775 -- 73% of the entire machine's
  // combinational logic, for a 1024x1 static RAM on a support board. The
  // latency added is one sys_clk, which is 1/16 of a microinstruction.
  reg dout_r;
  always @(posedge sys_clk) dout_r <= mem[a];
  assign p7 = (!p1) ? dout_r : 1'b0;


  wire _unused_pins = &{1'b0, p8, p16, 1'b0};

endmodule

`default_nettype wire
