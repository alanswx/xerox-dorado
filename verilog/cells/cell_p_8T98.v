// cell_p_8T98 -- MECL model for the Xerox Dorado  (part 8T98)
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// Hex INVERTING Buffer with three-state outputs (Signetics 8T98).
//
// TtlDict has no 8T98 entry, but it has the 8T96 -- the same family and the
// same pinout, which matches this cell's generated port list exactly:
//
//   IN 2, 4, 6, 10, 12, 14  ->  OUT 3, 5, 7, 9, 11, 13, and enables on 1, 15
//
// The two parts differ in how those enables are split. The 8T96 has ONE
// enable group, which is how the dictionary states it (`g,EN',1,15`); the
// 8T98 splits them FOUR AND TWO -- pin 1 over the first four buffers, pin 15
// over the last two.
//
// The board says which is fitted, and it is the split. DskEth's g23 and g24
// tie both pins to one net, so they cannot tell; but g22 drives pin 1 from
// `TriconD04.sil+9` and grounds pin 15, which leaves `DriveTag'` and
// `TtlDriveTag'` permanently enabled while `ContTag'`, `HeadTag'` and
// `CylinderTag'` are gated. That only means anything if the two enables
// cover different buffers.
//
// The enables are active low, and a disabled output contributes ZERO rather
// than high impedance; see cell_MCS6502 for why that is the shape here.

`default_nettype none

module cell_p_8T98 (
    input  wire p1,  // (no name in EclDict)
    input  wire p2,  // (no name in EclDict)
    output wire p3,  // (no name in EclDict)
    input  wire p4,  // (no name in EclDict)
    output wire p5,  // (no name in EclDict)
    input  wire p6,  // (no name in EclDict)
    output wire p7,  // (no name in EclDict)
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // (no name in EclDict)
    input  wire p10,  // (no name in EclDict)
    output wire p11,  // (no name in EclDict)
    input  wire p12,  // (no name in EclDict)
    output wire p13,  // (no name in EclDict)
    input  wire p14,  // (no name in EclDict)
    input  wire p15,  // (no name in EclDict)
    input  wire p16// (no name in EclDict)
);

  assign p3  = ~p1  & ~p2;    // enable 1: buffers on outputs 3, 5, 7, 9
  assign p5  = ~p1  & ~p4;
  assign p7  = ~p1  & ~p6;
  assign p9  = ~p1  & ~p10;

  assign p11 = ~p15 & ~p12;   // enable 2: outputs 11 and 13
  assign p13 = ~p15 & ~p14;

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
