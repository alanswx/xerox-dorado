// cell_SG10139 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 15 package position(s) across the sixteen boards.
//

`default_nettype none

module cell_SG10139 #(
    parameter INIT_FILE = ""
) (
    output wire p1,  // Q0
    output wire p2,  // Q1
    output wire p3,  // Q2
    output wire p4,  // Q3
    output wire p5,  // Q4
    output wire p6,  // Q5
    output wire p7,  // Q6
    output wire p9,  // Q7
    input  wire p10,  // A4
    input  wire p11,  // A3
    input  wire p12,  // A2
    input  wire p13,  // A1
    input  wire p14,  // A0
    input  wire p15// CE'
);

  // 32 x 8 ECL PROM (Signetics 10139 / Motorola MC10139).
  //
  // BIT ORDER -- PROVEN FROM THE DATA SHEET, not from convention. The
  // Signetics 10139 appears in the 1977 Bipolar and MOS Memory data book at
  // document page 93 (PDF page 94),
  // DoradoDocs/datasheets/1977_Signetics_Bipolar_and_MOS_Memory.pdf, and its
  // PIN CONFIGURATION gives:
  //
  //     pin 10 = A0 (LSB)   11 = A1   12 = A2   13 = A3   14 = A4 (MSB)
  //     pin  1 = O0   2 = O1 ... 7 = O6   9 = O7        pin 15 = CE'
  //
  // EclDict names those address pins `A0,14 > A1,13 > A2,12 > A3,11 > A4,10`
  // -- the EXACT REVERSAL, exactly as it does for the F10016's counter pins.
  // A CELL IMPLEMENTS THE DATA SHEET'S FUNCTION PER PIN NUMBER, so pin 14 is
  // the most significant address bit and `{p14,p13,p12,p11,p10}` below is the
  // silicon's own A4..A0.
  //
  // The OUTPUT axis is not reversed and does not need to be: a PROM's eight
  // columns carry no arithmetic weight, so the byte order is set purely by how
  // PARC writes the table. DiskProms says `Pin1 = #200` -- bit 7 -- so pin 1
  // holds the byte's MSB.
  //
  // The data sheet also states the disable behaviour this cell relies on:
  // "Outputs go to the 0 (low) state when the chip enable input is high,
  // allowing wired-OR output connections."
  //
  // Contents come from tools/dorado_proms.py --emit-packages, already sliced
  // to this package's bits. Empty INIT_FILE leaves the array X, which is the
  // honest state for a PROM whose contents we do not have.
  reg [7:0] mem [0:31];
  initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);
  wire [4:0] a = {p14, p13, p12, p11, p10};            // A0..A4
  wire [7:0] d = mem[a];
  assign {p1, p2, p3, p4, p5, p6, p7, p9} = (!p15) ? d : 8'b0;   // CE' low = enabled
endmodule

`default_nettype wire
