// cell_MC1668 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 12 package position(s) across the sixteen boards.
//
// Dual HIGH-transparent clocked latch (MECL III). Motorola's 1978 MECL data
// book, PDF p235 / printed4-30, gives C=1: Q follows D, ignoring S/R;
// C=0: set/reset or hold. PARC's timing dictionary independently marks the
// data arcs as HIGH-transparent. Treating this part as an edge-triggered FF
// lost updates within the R read/bypass window.
//
// S/R are active high while CLOSED. S=R=1,C=0 is unspecified by the sheet;
// preserve deterministic set priority for that undefined input combination.
//
// FPGA: sample on sys_clk throughout the high level, as the other latch cells
// do. This does not qualify the surrounding board's fabric-sampling cadence.

`default_nettype none

module cell_MC1668 (
    input  wire sys_clk,
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p4,  // R
    input  wire p5,  // S
    input  wire p6,  // D
    input  wire p7,  // C
    input  wire p9,  // C
    input  wire p11,  // D
    input  wire p12,  // S
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15// Q
);

  reg qa, qb;
  always @(posedge sys_clk) begin
    if (p7)          qa <= p6;        // transparent while C high
    else if (p5)     qa <= 1'b1;      // S while closed
    else if (p4)     qa <= 1'b0;      // R
  end
  always @(posedge sys_clk) begin
    if (p9)          qb <= p11;
    else if (p12)    qb <= 1'b1;
    else if (p13)    qb <= 1'b0;
  end

  assign p2 =  qa;  assign p3  = ~qa;
  assign p15 = qb;  assign p14 = ~qb;
endmodule

`default_nettype wire
