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
  // BIT ORDER, and it is the same rule on both axes: PARC's structures are
  // MSB-FIRST, so A0 is the most significant ADDRESS bit and Q0 the most
  // significant OUTPUT bit. Both are stated in the sources -- EtherProms
  // marks `pdCarrier bit // A0 pin 4` for a field that is address bit 7,
  // and DiskProms has `Pin1 = #200`, bit 7 of a byte, where Pin1 is Q0.
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
