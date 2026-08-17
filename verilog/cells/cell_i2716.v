// cell_i2716 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 8 package position(s) across the sixteen boards.
//
// 2K x 8 EPROM. PARC's TtlDict gives the pinout:
//
//   i2716, i2758
//   a,A0,19 > a,A1,22 > a,A2,23 > a,A3,1 > a,A4,2 > a,A5,3 > a,A6,4
//   a,A7,5  > a,A8,6  > a,A9,7  > a,A10,8
//   a,CS',20 > a,PD',18 > a,VPP,21
//   a,Q0,9 > a,Q1,10 > a,Q2,11 > a,Q3,13 > a,Q4,14 > a,Q5,15 > a,Q6,16 > a,Q7,17
//
// Eight sockets on the BaseBoard (b60 b61 c60 c61 e60 f60 h61 i61), chip-
// selected by `Rom0'`..`Rom7'` from the SN74LS138 at g11 and addressed over
// the MICROCODE bus -- every socket takes `MCA.00` on A10, not a 6502 address
// line. These are the store the BaseBoard shifts into the Dorado at boot.
//
// CONTENTS ARE NOT WIRED IN YET, and the reason is worth stating: we HOLD the
// dumps (`firmware/`, five 2048-byte images from Nov 1987) and the C
// emulator's memory map says where they live -- C-08/C-10 at 0xC000-0xCFFF,
// C-12 at 0xD000-0xD7FF, B-08/B-10 at 0xF000-0xFFFF (dorado/include/
// baseboard.h) -- but which SOCKET holds which image needs the RSA.0-2
// decode traced back to whatever drives it. Eight sockets, five images: three
// are empty, and guessing which would be worse than leaving it.
//
// INIT_FILE is here so that mapping is a one-line change per socket when it
// is established, the same way the PROM packages are wired.

`default_nettype none

module cell_i2716 #(
    parameter INIT_FILE = ""
) (
    input  wire p1,  // A3
    input  wire p2,  // A4
    input  wire p3,  // A5
    input  wire p4,  // A6
    input  wire p5,  // A7
    input  wire p6,  // A8
    input  wire p7,  // A9
    input  wire p8,  // A10
    output wire p9,  // Q0
    output wire p10,  // Q1
    output wire p11,  // Q2
    input  wire p12,  // (no name in EclDict)
    output wire p13,  // Q3
    output wire p14,  // Q4
    output wire p15,  // Q5
    output wire p16,  // Q6
    output wire p17,  // Q7
    input  wire p18,  // PD'
    input  wire p19,  // A0
    input  wire p20,  // CS'
    input  wire p21,  // VPP
    input  wire p22,  // A1
    input  wire p23,  // A2
    input  wire p24// (no name in EclDict)
);

  wire [10:0] a = {p8, p7, p6, p5, p4, p3, p2, p1, p23, p22, p19};  // A10..A0

  reg [7:0] mem [0:2047];
  initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);

  wire [7:0] d = mem[a];
  wire       en = ~p20 & ~p18;            // CS' and PD' both low
  assign {p17, p16, p15, p14, p13, p11, p10, p9} = en ? d : 8'h00;

  wire _unused_pins = &{1'b0, p12, p21, p24, 1'b0};   // GND, VPP, VCC
endmodule

`default_nettype wire
