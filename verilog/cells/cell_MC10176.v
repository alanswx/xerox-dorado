// cell_MC10176 -- Hex D Master-Slave Flip-Flop
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 307 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.

`default_nettype none

module cell_MC10176 (
    input  wire sys_clk,
    input  wire p9,   // C   clock, common to all six,
    input  wire p5,  // D0,
    input  wire p6,  // D1,
    input  wire p7,  // D2,
    input  wire p10,  // D3,
    input  wire p11,  // D4,
    input  wire p12,  // D5,
    output wire p2,  // Q0,
    output wire p3,  // Q1,
    output wire p4,  // Q2,
    output wire p13,  // Q3,
    output wire p14,  // Q4,
    output wire p15,// Q5,
    // Pins the boards wire that the dictionary does not name --
    // power (VEE/VCC on a 16-pin MECL package) and the like,
    // declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // Hex D master-slave flip-flop: six D-type stages on one common clock,
  // data transferred on the POSITIVE-going clock edge.

  // FPGA: ONE CLOCK, and the ECL clock net becomes an ENABLE.
  //
  // The Dorado clocks this part from a distributed ECL clock net (CLK.ph'
  // and friends, fanned out by the BaseBoard). Taking that literally gives
  // the fabric a gated clock off combinational logic -- 1,201 packages across
  // the machine do it -- which no FPGA can route. So every flip-flop here
  // runs on `sys_clk` and transfers on the sys_clk edge FOLLOWING an edge of
  // the modelled clock net. That is the usual oversampling transform, and it
  // matches the part as long as sys_clk is faster than the clock net, which
  // dorado_machine guarantees by dividing.
  //
  // Asynchronous inputs (MR, S/R, CL') are LEVEL-tested every sys_clk edge --
  // the same treatment, and it keeps them out of the fabric's reset network.
  reg ck_d;
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;

  // PRE-EDGE CAPTURE -- a real flip-flop's SETUP TIME, restored. The
  // oversampling transform detects the modelled clock's edge one sys_clk
  // after it happens, and by then a D that changed ON that same edge has
  // already settled to its NEW value -- so a same-edge coincidence captured
  // data the real part could never have seen (zero setup). Measured on
  // ProcL d17: its SHCP'C edge and its D transition land on the same
  // sys_clk, it captured the NEW enable, ProcL's half of T missed its
  // write, and Bootstrap's checksum read {new high, old low}. Registering
  // D every sys_clk and capturing the PRE-EDGE copy makes every capture see
  // the data as it stood before the detected edge -- the conservative,
  // real-FF resolution of every such coincidence, uniformly.
  reg [5:0] d_pre;
  always @(posedge sys_clk) d_pre <= {p12, p11, p10, p7, p6, p5};

  reg [5:0] q;
  always @(posedge sys_clk) if (ck_en) q <= d_pre;
  assign {p15, p14, p13, p4, p3, p2} = q;

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
