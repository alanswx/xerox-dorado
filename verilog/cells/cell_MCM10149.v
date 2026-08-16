// cell_MCM10149 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 15 package position(s) across the sixteen boards.
//

`default_nettype none

module cell_MCM10149 #(
    parameter INIT_FILE = ""
) (
    input  wire p2,  // A1
    input  wire p3,  // A2
    input  wire p4,  // A0
    input  wire p5,  // A6
    input  wire p6,  // A5
    input  wire p7,  // A7
    input  wire p9,  // A3
    input  wire p10,  // A4
    output wire p11,  // Q3
    output wire p12,  // Q2
    input  wire p13,  // CE'
    output wire p14,  // Q1
    output wire p15// Q0
);

  // 256 x 4 ECL PROM (Motorola MCM10149; MCM10150 is pin-compatible and the
  // sources say either works). A0 is the most significant address bit and Q0
  // the most significant output bit -- see cell_SG10139 for why.
  //
  // DoradoProms.defs states the output order outright:
  //     structure MCM149[ Pin15 bit; Pin14 bit; Pin12 bit; Pin11 bit; ... ]
  // and Pin15 is Q0.
  reg [3:0] mem [0:255];
  initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);
  wire [7:0] a = {p4, p2, p3, p9, p10, p6, p5, p7};    // A0..A7
  assign {p15, p14, p12, p11} = (!p13) ? mem[a] : 4'b0;          // CE' low = enabled
endmodule

`default_nettype wire
