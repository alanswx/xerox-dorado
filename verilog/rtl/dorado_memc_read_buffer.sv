// cell_MB7071H -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 70 package position(s) across the sixteen boards.
//
// 256 x 4 RAM, and it is the machine's REGISTER FILE. EclDict names every
// pin (A0-A7, WE', BS0'-BS3', and four data pairs b/c/d/e), and the boards
// say what it holds: ProcH h06 is addressed by `RbAdr.0-7` with `SelectRm'a`
// and `RbWrite'a` -- that is RM. i06 is `StkAdr.0-7` with `SelectStk'a` --
// that is STK, the emulator stack. Four packages per processor board give
// the 16-bit width, which is why ProcH and ProcL have four each.
//
// The other 62 packages are the same part doing the same job elsewhere:
// MemC's cache tags and DispM's colour tables.
//
// FPGA: SYNCHRONOUS WRITE, ASYNCHRONOUS READ -- which is exactly what a
// distributed/LUT RAM gives you, and what the part does. The write has to be
// clocked (no FPGA has a level-sensitive write port), so it lands on
// `sys_clk` like every other clocked cell here; the read stays combinational
// because the surrounding ECL logic reads within a microcycle.
//
// BIT ORDER does not need the MSB-first rule the PROMs follow: this array is
// written and read through the same address pins by the same board, so any
// self-consistent order is correct. A0 is taken as the most significant for
// consistency with the PROM cells.
//
// The four block selects are separate pins but the boards tie them together
// (ProcH h06 has SelectRm'a on all of 3,4,5,6). Active low, so the part is
// selected when all four are low.

`default_nettype none

module dorado_memc_read_buffer (
    input wire state_host_clk, state_advance, state_freeze, state_guest_held,
    input wire state_request, state_write,
    input wire [8:0] state_index,
    input wire [3:0] state_data,
    output reg state_frozen = 1'b0,
    output reg state_done = 1'b0, state_error = 1'b0,
    output reg [3:0] state_result,
    input  wire sys_clk,
    input  wire p2,  // WE'
    input  wire p3,  // BS3'
    input  wire p4,  // BS2'
    input  wire p5,  // BS1'
    input  wire p6,  // BS0'
    input  wire p7,  // A0
    input  wire p8,  // A1
    input  wire p9,  // A2
    input  wire p10,  // A3
    input  wire p11,  // A4
    input  wire p12,  // A5
    input  wire p13,  // A6
    input  wire p14,  // A7
    input  wire p15,  // (no name in EclDict)
    input  wire p16,  // e_IN0
    input  wire p17,  // d_IN0
    input  wire p18,  // c_IN0
    input  wire p19,  // b_IN0
    output wire p20,  // e_OUT
    output wire p21,  // d_OUT
    output wire p22,  // c_OUT
    output wire p23,output wire read_ready// b_OUT; auxiliary address match
);

  wire [7:0] a   = {p7, p8, p9, p10, p11, p12, p13, p14};   // A0..A7
  wire       sel = ~(p3 | p4 | p5 | p6);                    // BS0'-BS3'
  wire       we  = ~p2;                                     // WE'

  // Cyclone V MLAB supports this asynchronous read. Without the hint,
  // Quartus 17 expands these register files into individual flip-flops.
  (* ramstyle = "MLAB, no_rw_check" *) reg [3:0] mem [0:255];
  // One physical RAM/read port; state scans temporarily borrow its address.
  wire state_hold = state_freeze && state_guest_held;
  wire state_accept = state_hold && state_frozen && state_request;
  wire state_valid = !state_index[8];
  wire [7:0] state_address = state_hold ? state_index[7:0] : a;
  wire [3:0] state_read = mem[state_address];
  wire state_restore = state_accept && state_write && state_valid;
  wire state_guest_write = !state_hold && state_advance && (sel & we);
  wire [3:0] state_write_data = state_hold ? state_data : {p16, p17, p18, p19};
  always @(posedge state_host_clk) begin
    state_frozen <= state_hold;
    state_done <= state_accept;
    state_error <= state_accept && !state_valid;
    if (state_accept) begin
      state_result <= state_valid && !state_write ? state_read : 4'b0;
    end
    if (state_restore || state_guest_write)
      mem[state_address] <= state_write_data;
  end

  // Auxiliary read pipeline only. The original RAM is the sole owner.
  reg read_valid=1'b0;
  reg [7:0] read_address_q;
  reg [3:0] read_data_q;
  always @(posedge state_host_clk) begin
    read_valid<=1'b1;
    read_address_q<=state_address;
    // Own write on this edge becomes visible after this edge, like the RAM.
    read_data_q<=(state_restore || state_guest_write) ? state_write_data:state_read;
  end
  assign read_ready=state_hold || (read_valid && read_address_q==state_address);
  wire [3:0] q = state_hold ? state_read:read_data_q;
  assign {p20, p21, p22, p23} = sel ? q : 4'b0000; // e, d, c, b; ECL reads low

  wire _unused_pins = &{1'b0, p15, 1'b0};
endmodule

`default_nettype wire
