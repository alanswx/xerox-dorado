// cell_SN74H04 -- Hex Inverter (TTL)
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 28 package position(s), ALL OF THEM ON THE MSA STORAGE BOARD.
//
// FUNCTION: the SN74H04 is the high-speed member of the 7404 hex-inverter
// family -- six independent inverters in the standard pinout, input/output
// pairs (1,2) (3,4) (5,6) (9,8) (11,10) (13,12), pin 7 GND and pin 14 Vcc.
//
// AND THE BOARD PROVES IT WITHOUT APPEAL TO A DATA SHEET. Every one of the
// 28 packages takes a PRIMED net in and drives the UNPRIMED one out --
// msa a06 is `.p1(TtlA0_p_) .p2(A0a0)`, `.p3(TtlA1_p_) .p4(A1a0)`, and so on
// through all six gates. PARC's own naming states the inversion.
//
// WHY IT MATTERS: these are what drive the DRAM ADDRESS LINES. The MSA takes
// its multiplexed address off the backplane as TtlA0'..TtlA6' and inverts it
// here into A0a0..A6a0, which fan out to the 144 MK4096 packages. Left
// unmodelled -- as it was -- every address line on the storage board reads 0
// and no location can be selected.

`default_nettype none

module cell_SN74H04 (
    input  wire p1,  // 1A
    output wire p2,  // 1Y
    input  wire p3,  // 2A
    output wire p4,  // 2Y
    input  wire p5,  // 3A
    output wire p6,  // 3Y
    input  wire p7,  // GND
    output wire p8,  // 4Y
    input  wire p9,  // 4A
    output wire p10, // 5Y
    input  wire p11, // 5A
    output wire p12, // 6Y
    input  wire p13  // 6A
);

  assign p2  = ~p1;
  assign p4  = ~p3;
  assign p6  = ~p5;
  assign p8  = ~p9;
  assign p10 = ~p11;
  assign p12 = ~p13;

  wire _unused_pins = &{1'b0, p7, 1'b0};   // GND

endmodule

`default_nettype wire
