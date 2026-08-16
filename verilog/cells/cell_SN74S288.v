// cell_SN74S288 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//

`default_nettype none

module cell_SN74S288 #(
    parameter INIT_FILE = ""
) (
    output wire p3,  // Q5
    output wire p4,  // Q4
    output wire p5,  // Q3
    output wire p6,  // Q2
    output wire p7,  // Q1
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // Q0
    input  wire p10,  // A4
    input  wire p11,  // A3
    input  wire p12,  // A2
    input  wire p13,  // A1
    input  wire p14,  // A0
    input  wire p15,  // CS'
    input  wire p16// (no name in EclDict)
);

  // 32 x 8 TTL PROM, the one non-ECL PROM in the machine: the disk
  // drive-select table at DskEth d05.
  //
  // Only SIX outputs are wired (Q0..Q5 = p9,p7,p6,p5,p4,p3), and that is
  // consistent rather than odd: DiskProms.bcpl MakeDriveSelect puts the four
  // unary selects at bits 7..4 and the binary drive number at bits 3:2, and
  // never touches bits 1:0. With Q0 as the most significant bit, the six
  // wired outputs are exactly the six bits the table uses.
  reg [7:0] mem [0:31];
  initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);
  wire [4:0] a = {p14, p13, p12, p11, p10};            // A0..A4
  wire [7:0] d = mem[a];
  assign {p9, p7, p6, p5, p4, p3} = (!p15) ? d[7:2] : 6'b0;      // CS' low = enabled
endmodule

`default_nettype wire
