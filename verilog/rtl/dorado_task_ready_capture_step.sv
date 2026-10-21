// Couple actual Ready/IsNext to the original task/processor event evaluator.
// Q[87:0] = task/capture component; [103:88] = four Ready F10000s.
// Ready and arbitration consume pre-event task/data Qs at the shared clock.
// This is a component, not native integration or a 60ns board proof.
`default_nettype none
module dorado_task_ready_capture_step(
 input wire [103:0] state_q,input wire clock_before,clock_after,
 // [75:0] prior task interface; old Ready/IsNext inputs [64:35] are unused.
 // [79:76]=bFF.7..4,[80]=PreEmpting',[81]=FF=Notify',[82]=ClrReady;
 // [84:83]=unrelated d08 gate inputs;[85] reserved.
 input wire [85:0] pins_before,pins_after,
 output wire [103:0] state_next,
 output wire [26:0] clock_events,repeated_events,
 output wire unsettled,output wire [29:0] ready_flags
);
 function automatic [3:0] current_task(input [87:0] q);
  current_task={q[78],q[79],q[76],q[77]};
 endfunction
 function automatic [27:0] ready_pins(input [87:0] q,input [85:0] p);
  reg [27:0] result;
  begin
   result=0;result[27:24]=q[67:64];result[3:0]=q[71:68];result[7:4]=current_task(q);
   result[11:8]=p[79:76];result[12]=!q[72];result[13]=p[80];
   result[14]=p[81];result[15]=p[82];result[23:22]=p[84:83];
   ready_pins=result;
  end
 endfunction
 wire [27:0] before_data=ready_pins(state_q[87:0],pins_before);
 wire [27:0] capture_data=ready_pins(state_q[87:0],pins_after);
 wire [14:0] before_ready,before_mask,pre_ready,pre_mask;
 // Combinational pre-event views: there is no capture edge in these calls.
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) before_view(state_q[103:88],before_data,before_data,
   ,,,before_mask,before_ready);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) pre_view(state_q[103:88],capture_data,capture_data,
   ,,,pre_mask,pre_ready);
 reg [75:0] core_before,core_after;
 always @* begin
  core_before=pins_before[75:0];core_after=pins_after[75:0];
  core_before[49:35]=before_ready;core_before[64:50]=before_mask;
  core_after[49:35]=pre_ready;core_after[64:50]=pre_mask;
 end
 dorado_task_capture_step core(state_q[87:0],clock_before,clock_after,
   core_before,core_after,state_next[87:0],clock_events[24:0],repeated_events[24:0],unsettled);
 // ContA d07's two Ready clocks and j08/j09's gated task clocks are the
 // same logical edge (independently checked in the extracted circuit).
 // Translate that qualified event into the Ready module's capture enable;
 // task/data sources remain the pre-event epoch, not newly loaded BNT/CTask.
 wire [27:0] edge_data=capture_data | (28'(clock_events[20])<<18);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) capture(state_q[103:88],capture_data,edge_data,
   state_next[103:88],clock_events[26:25],,,);
 assign repeated_events[26:25]={2{repeated_events[20]}};
 wire [27:0] settled_data=ready_pins(state_next[87:0],pins_after);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) settled_view(state_next[103:88],settled_data,settled_data,
   ,,,ready_flags[29:15],ready_flags[14:0]);
endmodule
`default_nettype wire
