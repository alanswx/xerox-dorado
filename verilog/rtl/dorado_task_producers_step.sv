// Couple the original CP maintenance registers to Ready's asynchronous clear.
// Component only: task/phase/MIR inputs remain explicit producer boundaries.
// Q[15:0] = Ready packages; Q[27:16] = j02/i02 maintenance packages.
`default_nettype none
module dorado_task_producers_step(
 input wire [27:0] state_q,
 // [34:24] = CP bus; [23:0] = Ready component pins, with [15] unused:
 // ClrReady comes from the actual i02.Q3 rather than this external bit.
 input wire [34:0] pins_before,pins_after,
 output wire [27:0] state_next,
 output wire [3:0] clock_events,clocks_after,
 output wire [14:0] is_next_n,ready_after
);
 wire [11:0] cp_next;
 wire [23:0] ready_before={pins_before[23:16],state_q[25],pins_before[14:0]};
 wire [23:0] ready_pins={pins_after[23:16],cp_next[9],pins_after[14:0]};
 dorado_cp_control_step cp(state_q[27:16],pins_before[34:24],pins_after[34:24],
   cp_next,clock_events[3:2],clocks_after[3:2]);
 dorado_task_ready_step ready(state_q[15:0],ready_before,ready_pins,
   state_next[15:0],clock_events[1:0],clocks_after[1:0],is_next_n,ready_after);
 assign state_next[27:16]=cp_next;
endmodule
`default_nettype wire
