// Native-facing, stateless processor flag transfer. The common owner supplies
// physical pre-event Q, the new external arrivals, and the before/settled flag
// boundary frames. Source root and derived flag clock are deliberately separate.
// The generated read-only ALU view and branch cone must come from the qualified
// original1198-bit graph. No architectural reset, clock or storage is added here.
`default_nettype none
module dorado_processor_flag_capture(
 input wire [1197:0] physical_before,
 input wire [431:0] producer_pins_after,
 input wire source_clock_before,flag_clock_before,flag_clock_after,
 input wire [37:0] flag_pins_before,flag_pins_after,
 output wire [79:0] flags_next,
 output wire [3:0] clock_events,
 output wire unsettled,
 output wire [37:0] flag_pins_early);

 alu_projection_processor_alu_reference early_view(
  .fixture_load(1'b0),.fixture_state(physical_before),
  .base(source_clock_before),.pins(producer_pins_after),
  .branch_pins(flag_pins_early));

 dorado_branch_condition_frame_step capture(
  .state_q(physical_before[923:844]),
  .clock_before(flag_clock_before),.clock_after(flag_clock_after),
  .pins_before(flag_pins_before),.pins_early(flag_pins_early),.pins_after(flag_pins_after),
  .state_next(flags_next),.clock_events(clock_events),.unsettled(unsettled));
endmodule
`default_nettype wire
