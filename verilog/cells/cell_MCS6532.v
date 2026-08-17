// cell_MCS6532 -- the BaseBoard's RIOT (RAM + I/O + Timer)
//
// Pin numbers/names: PARC's TtlDict.Analyze. The Dorado BaseBoard carries
// FIVE of these (c62, f62, f63, i62, l62) around one MCS6502 -- exactly the
// arrangement dorado/src/baseboard.c models and runs the real EPROMs on.
//
// Core: MiSTer's Atari 7800 RIOT (rtl/RIOT/M6532.sv, k7800 by Jamie Blanks),
// vendored under verilog/vendor/riot with a Verilator patch. The 7800's is
// SystemVerilog; the 2600 core's RIOT is VHDL, which Verilator cannot take.
//
// *** LICENCE: the vendored core is CC BY-NC 4.0 (NONCOMMERCIAL), which is
// more restrictive than the rest of this repository. See
// verilog/vendor/LICENSES.md. ***
//
// The 6532's D0-D7 are bidirectional; PARC's wire list marks them per board.
// The core presents d_in/d_out separately with an `oe`, which suits that.

`default_nettype none

module cell_MCS6532 (
    input  wire p39,  // CK2   phase-2 clock
    input  wire p34,  // RESET'
    input  wire p38,  // CS    chip select 1 (active high)
    input  wire p37,  // CS'   chip select 2 (active low)
    input  wire p36,  // RS'   RAM select
    input  wire p35,  // R/W
    output wire p25,  // IRQ'

    input  wire p7,   // A00
    input  wire p6,   // A01
    input  wire p5,   // A02
    input  wire p4,   // A03
    input  wire p3,   // A04
    input  wire p2,   // A05
    input  wire p40,  // A06

    output wire p33,  // D0
    output wire p32,  // D1
    output wire p31,  // D2
    output wire p30,  // D3
    output wire p29,  // D4
    output wire p28,  // D5
    output wire p27,  // D6
    output wire p26,  // D7

    output wire p8,   // PA0
    output wire p9,   // PA1
    output wire p10,  // PA2
    output wire p11,  // PA3
    output wire p12,  // PA4
    output wire p13,  // PA5
    output wire p14,  // PA6
    output wire p15,  // PA7

    output wire p24,  // PB0
    output wire p23,  // PB1
    output wire p22,  // PB2
    output wire p21,  // PB3
    output wire p19,  // PB4
    output wire p18,  // PB5
    output wire p17,  // PB6
    output wire p16,  // PB7

    input  wire [7:0] d_in,   // not package pins -- see the bus note above
    input  wire [7:0] pa_in,
    input  wire [7:0] pb_in,

    input  wire p1,   // GND1
    input  wire p20   // VCC1
);


  wire [7:0] d_out, pa_out, pb_out;
  wire       irq_n, oe;

  M6532 u_riot (
      .clk   (p39),
      .ce    (1'b1),
      .res_n (p34),
      .addr  ({p40, p2, p3, p4, p5, p6, p7}),   // A06..A00
      .RW_n  (p35),
      .d_in  (d_in),
      .d_out (d_out),
      .RS_n  (p36),
      .IRQ_n (irq_n),
      .CS1   (p38),
      .CS2_n (p37),
      .PA_in (pa_in),
      .PA_out(pa_out),
      .PB_in (pb_in),
      .PB_out(pb_out),
      .oe    (oe)
  );

  // `oe` is the core's own statement that it is driving the bus this cycle
  // -- chip selected, RW a read. Off the bus it contributes zero; see
  // cell_MCS6502 for why that is the shape a shared TTL bus takes here.
  assign {p26,p27,p28,p29,p30,p31,p32,p33} = oe ? d_out : 8'h00;
  assign {p15,p14,p13,p12,p11,p10,p9,p8}   = pa_out;
  assign {p16,p17,p18,p19,p21,p22,p23,p24} = pb_out;
  assign p25 = irq_n;

  wire _unused = &{1'b0, p1, p20, 1'b0};

endmodule

`default_nettype wire
