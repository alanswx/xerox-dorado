// Experimental BaseBoard phase/enable source; not connected to a machine.
// HM section 2.3: one clock is 30 ns, two clocks per ordinary instruction.
// BaseBd11.sil: g04/h04 Johnson divider on one VCO edge; g03/h03 on
// the opposite edge. SCP'=g04.Q', ECP=h03.Q. BaseBd12 j02 ORs these.
//
// Eight slots preserve the divider's 1/8-period ECP lead and 3/8-period
// low window. This is a correctness prototype, not a full-speed solution:
// at one master tick per slot, original speed would require 266.667 MHz.
// Board-clock qualification, debug pulses, Run/Hold/Step, IM/TPC extension,
// and transparent latch windows must be translated separately before use.
// Level outputs are diagnostic logic, not FPGA clock-routing primitives.
`default_nettype none
module dorado_clock_phases #(
    parameter integer TICKS_PER_SLOT = 1
) (
    input  wire clk,
    input  wire reset,
    // Prototype scheduler freeze, NOT the original processor Hold input.
    input  wire advance,
    output reg  [2:0] phase,
    output wire scp_n,
    output wire ecp,
    output wire slot_clock,
    // Each enable describes the transition at the UPCOMING clk edge.
    // Consumers use always @(posedge clk) if (enable) q <= d, sampling
    // pre-edge data without ck_d/d_pre or a later edge-detection tick.
    output wire scp_rise,
    output wire scp_fall,
    output wire ecp_rise,
    output wire ecp_fall,
    output wire slot_rise,
    output wire slot_fall
);
    localparam integer CW = TICKS_PER_SLOT <= 1 ? 1 : $clog2(TICKS_PER_SLOT);
    reg [CW-1:0] subtick;
    wire tick = advance && (subtick == CW'(TICKS_PER_SLOT-1));
    wire [2:0] next_phase = phase + 3'd1;

    // One rotation of the steady-state original divider, not its analog
    // power-up transient. Phase zero establishes an arbitrary time origin.
    function automatic scp_at(input [2:0] p);
        scp_at = p < 3'd2 || p >= 3'd6;
    endfunction
    function automatic ecp_at(input [2:0] p);
        ecp_at = p == 3'd0 || p >= 3'd5;
    endfunction
    assign scp_n = scp_at(phase);
    assign ecp = ecp_at(phase);
    // Inactive dStartClockPulse only; this is not all of BaseBoard j02.
    assign slot_clock = scp_n | ecp;
    wire next_scp = scp_at(next_phase);
    wire next_ecp = ecp_at(next_phase);
    wire next_slot = next_scp | next_ecp;
    wire transfer = tick && !reset;
    assign scp_rise = transfer && !scp_n && next_scp;
    assign scp_fall = transfer && scp_n && !next_scp;
    assign ecp_rise = transfer && !ecp && next_ecp;
    assign ecp_fall = transfer && ecp && !next_ecp;
    assign slot_rise = transfer && !slot_clock && next_slot;
    assign slot_fall = transfer && slot_clock && !next_slot;

    always @(posedge clk) begin
        if (reset) begin
            phase <= 3'd0;
            subtick <= '0;
        end else if (advance) begin
            if (tick) begin
                phase <= next_phase;
                subtick <= '0;
            end else
                subtick <= subtick + 1'b1;
        end
    end

    initial begin
        if (TICKS_PER_SLOT < 1)
            $fatal(1, "TICKS_PER_SLOT must be positive");
    end
endmodule
`default_nettype wire
