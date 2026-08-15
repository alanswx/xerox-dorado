// MiSTer-shaped top level for the Dorado RTL.
//
// The module is called `emu` and carries the MiSTer core interface on
// purpose: the same RTL that runs under this Verilator harness drops into a
// MiSTer core without a second port. The signals here are the subset of
// MiSTer's `emu` that this design needs today; the rest can be added as the
// design grows, and their names must not drift from MiSTer's.
//
// What it instantiates is GENERATED RTL: `../generated/*.v`, produced by
// tools/sil_to_verilog.py from PARC's own Sil wire lists. Those boards
// currently elaborate but do not compute -- their cells are correct ports
// with TODO bodies -- so this wrapper's job today is to give the harness
// something real to clock, reset and inspect while the cell library is
// filled in.

`default_nettype none

module emu (
    input  wire        CLK_50M,
    input  wire        RESET,

    // Dorado microcycle clock. The real machine runs a 60 ns microinstruction
    // (16.67 MHz); the harness drives this directly so the sim can step one
    // microcycle at a time.
    input  wire        clk_sys,

    output wire [7:0]  VGA_R,
    output wire [7:0]  VGA_G,
    output wire [7:0]  VGA_B,
    output wire        VGA_HS,
    output wire        VGA_VS,
    output wire        VGA_DE,

    output wire [15:0] AUDIO_L,
    output wire [15:0] AUDIO_R,

    input  wire [31:0] joystick_0,
    input  wire [10:0] ps2_key,

    // Observation port: which board signal the harness is watching, and its
    // value. Keeps the imgui panel from having to reach into Verilator
    // internals for the common case.
    input  wire [15:0] probe_sel,
    output wire [31:0] probe_val,

    output wire [31:0] status_out
);

  // ------------------------------------------------------------------
  // Reset synchroniser. The Dorado's own power-up is the BaseBoard's job
  // (a 6502 plus five 6532 RIOTs, per BaseBd-Rev-Am); this is just the
  // harness-level reset until that board has behaviour.
  // ------------------------------------------------------------------
  reg [3:0] rst_cnt = 4'hF;
  wire      rst = |rst_cnt;
  always @(posedge clk_sys) begin
    if (RESET) rst_cnt <= 4'hF;
    else if (|rst_cnt) rst_cnt <= rst_cnt - 4'd1;
  end

  // ------------------------------------------------------------------
  // Microcycle counter -- something real to watch before the boards
  // compute, and the unit every Dorado timing figure is quoted in.
  // ------------------------------------------------------------------
  reg [31:0] ucycle = 32'd0;
  always @(posedge clk_sys) begin
    if (rst) ucycle <= 32'd0;
    else     ucycle <= ucycle + 32'd1;
  end

  // ------------------------------------------------------------------
  // Generated boards.
  //
  // Deliberately NOT instantiated yet: every board module currently has
  // hundreds of ports (its backplane nets) and no internal behaviour, so
  // wiring them here would be inventing a backplane before the cells can
  // use it. The backplane is its own piece of source data -- Backplane in
  // DoradoDocs/schematics -- and connecting boards to each other is the
  // step AFTER the cells compute. `make lint` proves they elaborate.
  //
  // When that lands it looks like:
  //     ProcH_Rev_Ce u_proch ( .Ain_00(Ain_00), ... );
  //     ProcL_Rev_Ci u_procl ( ... );
  // ------------------------------------------------------------------

  assign probe_val  = (probe_sel == 16'd0) ? ucycle :
                      (probe_sel == 16'd1) ? {31'd0, rst} :
                                             32'hDEAD_0000 | {16'd0, probe_sel};
  assign status_out = ucycle;

  // Nothing drives the video or audio yet; hold them quiet rather than
  // leaving them floating, so the harness shows black rather than noise.
  assign VGA_R = 8'd0;
  assign VGA_G = 8'd0;
  assign VGA_B = 8'd0;
  assign VGA_HS = 1'b0;
  assign VGA_VS = 1'b0;
  assign VGA_DE = 1'b0;
  assign AUDIO_L = 16'd0;
  assign AUDIO_R = 16'd0;

  // Keep the unused MiSTer inputs referenced so lint stays quiet about them
  // without a blanket waiver.
  wire _unused = &{1'b0, CLK_50M, joystick_0, ps2_key, 1'b0};

endmodule

`default_nettype wire
