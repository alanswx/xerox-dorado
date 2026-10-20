// Direct-enable next-state contract for ContA27's instruction phase chain.
// This is not connected to the native core. It neither generates clocks nor
// replaces Run/Hold/Freeze control. Capture enables must describe UPCOMING
// original clock-pin edges, not delayed observations of those pins.
//
// Original FFs: k03.B (pre-start), k04.B (start), l07.B/A (phase0/1),
// k05.B/A (phase2/3), j01.B (phase4). See check_control_phase_wiring.py.
// Seven state bits are the original Q outputs, not seven extra stages.
// phase0_init preserves the original asynchronous set/reset priority at an
// evaluated event boundary; integration must also schedule asynchronous events.
`default_nettype none
module dorado_control_phase_next (
    input wire [6:0] state_q, // {preStartCyclea,StartCycle,Phase0..Phase4}
    input wire capture_pre,  // rising preclk2'Bb, k03.B
    input wire capture_main, // rising clk2'B{c,e,f}, other six FFs
    input wire phase0_init,  // original sPhase0, not a new machine reset
    input wire im_tpc,       // original RWTPCorRWIM decode
    output wire d_start_cycle,
    output reg [6:0] state_next
);
    wire phase0 = state_q[4];
    wire phase4 = state_q[0];
    // Keep the pin equation, including its behavior with unknown state.
    // j05.A is OR-AND-INVERT; i04.B qualifies its non-inverting output.
    wire phase_term = (im_tpc || !phase0) && !phase4;
    assign d_start_cycle = !phase_term;
    wire d_phase1 = phase0 && phase_term;

    always @* begin
        state_next = state_q;
        if (phase0_init)
            state_next = 7'b0010000;
        else begin
            if (capture_pre) state_next[6] = d_start_cycle;
            if (capture_main)
                state_next[5:0] = {d_start_cycle,state_q[5],d_phase1,
                                   state_q[3],state_q[2],state_q[1]};
        end
    end
endmodule
`default_nettype wire
