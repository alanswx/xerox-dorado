// cell_MC1662 -- Quad 2-Input NOR (MECL III)
//
// Pins: PARC's EclDict.Analyze. Used in 33 package position(s), across ContA,
// ContB, IFU, MemC, MemX, ProcH and ProcL.
//
// THIS WAS A COPY OF cell_MC1664 AND COMPUTED OR. MC1662 and MC1664 are a
// complementary pair with the SAME pinout and the same `[G]` summary --
// `(4 5)>2, (6 7)>3, (10 11)>14, (12 13)>15` for both -- and the dictionary
// separates them by role letter alone, which here is exactly what it is for:
//
//     MC1662   a,IN,4,5 > a,OUT,2   ...   OUT -> inverting -> NOR
//     MC1664   a,IN,4,5 > a,o,2     ...   o   -> non-inverting -> OR
//
// `cell_MC1664`'s own comment already says so ("the MECL III pairing where
// MC1662 is the NOR part"); this file had not been given the other half.
// The data book is no help -- the MC1662 sheet was DELETED from DL122 rev 7.
//
// The circuit settles it too. ContB f21/g21/h20 are the IM ADDRESS
// multiplexer: `dRA.nn' = gate(TNIA.mm, SWa) wired-OR gate(SW'a, BNPC.mm)`,
// which is a 2:1 select only if the gate is a NOR. As an OR it degenerates --
// `SW'a` is 1 whenever the machine is not task-switching, so every `dRA'`
// sticks high and IM is addressed at 0 no matter what TNIA holds. That is
// precisely the symptom that led here: every jammed Write-IM landed at IM[0].

`default_nettype none

module cell_MC1662 (
    input  wire p4,
    input  wire p5,
    output wire p2,
    input  wire p6,
    input  wire p7,
    output wire p3,
    input  wire p10,
    input  wire p11,
    output wire p14,
    input  wire p12,
    input  wire p13,
    output wire p15
);

  assign p2  = ~(p4  | p5);
  assign p3  = ~(p6  | p7);
  assign p14 = ~(p10 | p11);
  assign p15 = ~(p12 | p13);


endmodule

`default_nettype wire
