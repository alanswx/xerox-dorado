// Original Control-A MIR/CP storage feeding Ready's real Notify/preemption path.
// ContA a20/b16/f24 decode Notify; c22 combines Block', Hold and SwitchUp.
// Q[15:0] Ready, [27:16] CP, [49:28] the physical MIR map.
// Digital setup contract: CP transactions/data settle before phase-clock edges.
// Ready samples the pre-event MIR epoch when the two clock families rise together.
// HM section 2.3 full cadence, IM arrivals and native integration remain open.
`default_nettype none
module dorado_mir_ready_step(
 input wire [49:0] state_q,
 // [23:0] prior Ready layout; [68:24] prior MIR layout; [79:69] CP bus.
 // Ready operand/Notify/preemption/clear and MIR brMIR/clocks are INTERNAL.
 // [80] ASEL.0'a, [81] BSEL.0', [82] Hold, [83] SwitchUp;
 // [84] RepeatCurC, [85] StopMIRClk, [86] preclk0'Cb;
 // [90:87] independent BNT.3'..0' (not inferred from BNT positive storage).
 input wire [90:0] pins_before,pins_after,
 output wire [49:0] state_next,
 output wire [9:0] clock_events,clocks_after,
 output wire [38:0] flags
);
 function automatic [8:0] decode(input [21:0] q,input [90:0] p);
  reg ffok_n,notify_n,preempt_n;
  begin
   // Use both d23 Qs for the wired-OR, not the separate d24 normal-data copy.
   ffok_n=!p[81] | !(q[9] | q[8] | q[12] | q[13]);
   notify_n=p[80] | ffok_n | !q[3] | !q[2] | !q[1] | !q[0];
   preempt_n=!(p[82] | q[17]) | !p[83];
   decode={q[21],q[17],q[4],q[5],q[6],q[7],ffok_n,preempt_n,notify_n};
  end
 endfunction
 wire [11:0] cp_next;
 dorado_cp_control_step cp(state_q[27:16],pins_before[79:69],pins_after[79:69],
  cp_next,clock_events[3:2],clocks_after[3:2]);
 wire mir_clock_before=pins_before[84] | pins_before[85] | pins_before[86];
 wire mir_clock_after=pins_after[84] | pins_after[85] | pins_after[86];
 wire [44:0] mir_before={{6{mir_clock_before}},{3{state_q[20]}},pins_before[59:24]};
 wire [44:0] mir_after={{6{mir_clock_after}},{3{cp_next[4]}},pins_after[59:24]};
 wire [21:0] mir_next;
 dorado_mir_capture_step mir(state_q[49:28],mir_before,mir_after,mir_next,clock_events[9:4]);
 assign clocks_after[9:4]={6{mir_clock_after}};
 wire [8:0] before_decode=decode(state_q[49:28],pins_before);
 wire [8:0] capture_decode=decode(state_q[49:28],pins_after);
 wire [8:0] settled_decode=decode(mir_next,pins_after);
 reg [27:0] ready_before,ready_after;
 always @* begin
  ready_before={pins_before[90:87],pins_before[23:0]};
  ready_after={pins_after[90:87],pins_after[23:0]};
  ready_before[11:8]=before_decode[6:3];ready_after[11:8]=capture_decode[6:3];
  ready_before[13]=before_decode[1];ready_after[13]=capture_decode[1];
  ready_before[14]=before_decode[0];ready_after[14]=capture_decode[0];
  ready_before[15]=state_q[25];ready_after[15]=cp_next[9];
 end
 wire [14:0] is_next_n,ready_q;
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) ready(state_q[15:0],ready_before,ready_after,
  state_next[15:0],clock_events[1:0],clocks_after[1:0],is_next_n,ready_q);
 assign state_next[27:16]=cp_next;
 assign state_next[49:28]=mir_next;
 assign flags={settled_decode,ready_q,is_next_n};
endmodule
`default_nettype wire
