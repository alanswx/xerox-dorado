// cell_MCS6502 -- the BaseBoard's 6502, wrapping a netlist-derived core
//
// Pin numbers/names: PARC's TtlDict.Analyze. The Dorado BaseBoard carries one
// MCS6502 at position f61 plus five MCS6532 RIOTs, which is exactly what the
// C emulator models (dorado/src/baseboard.c) -- it runs the real BaseBoard
// EPROMs and drives the boot handshake.
//
// The core is Andrew Holme's DIRECT NETLIST CONVERSION of the MOS6502
// (aholme.co.uk/6502/Main.htm, used with permission), taken from jotego's
// jtcores, which consolidated it into one file and made it Verilator-friendly.
// 1,725 nodes -- it is the chip, not a behavioural model of it.
//
// That choice is deliberate and matches this whole effort: the Dorado RTL is
// generated from PARC's own wire lists rather than reimplemented, so its CPU
// should be the netlist too. Where the C emulator uses a behavioural 6502
// (vendor/6502/fake6502), the RTL can afford the real thing.
//
// BUS DIRECTION. The 6502's D0-D7 are bidirectional on the real part, but
// PARC's wire list marks them `o` on this board -- the BaseBoard drives the
// data bus from the CPU's side through separate receivers. The core presents
// dbi/dbo separately, which suits that exactly: dbo goes to the board's MCD
// nets, and dbi comes back in through the pins the wire list marks `i`.

`default_nettype none

module cell_MCS6502 (
    input  wire p40,  // RESET'
    input  wire p4,   // IRQ'
    input  wire p6,   // NMI'
    input  wire p2,   // RDY
    input  wire p38,  // SO
    input  wire p37,  // CK2i  phase-2 clock in
    output wire p3,   // CK1o
    output wire p39,  // CK2o
    output wire p7,   // SYNC
    output wire p34,  // R/W'

    output wire p9,   // A00
    output wire p10,  // A01
    output wire p11,  // A02
    output wire p12,  // A03
    output wire p13,  // A04
    output wire p14,  // A05
    output wire p15,  // A06
    output wire p16,  // A07
    output wire p17,  // A08
    output wire p18,  // A09
    output wire p19,  // A10
    output wire p20,  // A11
    output wire p22,  // A12
    output wire p23,  // A13
    output wire p24,  // A14
    output wire p25,  // A15

    output wire p33,  // D0
    output wire p32,  // D1
    output wire p31,  // D2
    output wire p30,  // D3
    output wire p29,  // D4
    output wire p28,  // D5
    output wire p27,  // D6
    output wire p26,  // D7

    input  wire [7:0] dbi,  // data IN -- not a package pin; see note above

    input  wire p1,   // GND1
    input  wire p8,   // VCC1
    input  wire p21   // GND2
);

  wire [15:0] ab;
  wire [7:0]  dbo;
  wire        rw, sync;

  // The core wants a free-running FPGA clock plus the 6502 phase clock. On
  // this board CK2i is the phase; the harness supplies the fast clock.
  chip_6502 u_cpu (
      .clk  (p37),
      .phi  (p37),
      .res  (p40),
      .so   (p38),
      .rdy  (p2),
      .nmi  (p6),
      .irq  (p4),
      .dbi  (dbi),
      .dbo  (dbo),
      .rw   (rw),
      .sync (sync),
      .ab   (ab)
  );

  assign {p25,p24,p23,p22,p20,p19,p18,p17,
          p16,p15,p14,p13,p12,p11,p10,p9} = ab;
  assign {p26,p27,p28,p29,p30,p31,p32,p33} = dbo;
  assign p34 = rw;
  assign p7  = sync;

  // CK1o/CK2o are the part's own generated clock outputs. The BaseBoard
  // derives its timing from them; the netlist core does not model the clock
  // generator, so they mirror the incoming phase rather than being invented.
  assign p3  = ~p37;
  assign p39 =  p37;

  wire _unused_pins = &{1'b0, p1, p8, p21, 1'b0};

endmodule

`default_nettype wire
