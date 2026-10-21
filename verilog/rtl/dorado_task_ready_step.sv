// Original ContA Ready storage, all three SetReady drivers and IsNext decoders.
// No host task arbitration. Q ordering is the four F10000 package words.
// Component only: upstream MIR/phase/task sources must meet capture setup.
`default_nettype none
module dorado_task_ready_step #(parameter integer INDEPENDENT_BNT=0)(
 input wire [15:0] state_q,
 // [3:0]=BNT.3..0, [7:4]=CTask.3..0, [11:8]=bFF.7..4;
 // [12]=Switch'b, [13]=PreEmpting', [14]=FF=Notify', [15]=ClrReady;
 // [16]=CAHold,[17]=FreezeAC,[18]=preclk0'Aa,[19]=preclk2'Aa,
 // [20]=RepeatCur,[21]=SCorFreezeb; [23:22] retain unrelated d08 gate inputs.
 // When INDEPENDENT_BNT=1, [27:24] are actual separately stored BNT.3'..0'.
 input wire [23+4*INDEPENDENT_BNT:0] pins_before,pins_after,
 output reg [15:0] state_next,
 output wire [1:0] clock_events,clocks_after,
 output reg [14:0] is_next_n,ready_after
);
 function automatic [3:0] task_number(input [3:0] raw);
  task_number={raw[0],raw[1],raw[2],raw[3]};
 endfunction
 function automatic root_clock(input [23+4*INDEPENDENT_BNT:0] pins);
  root_clock=pins[17]|pins[18]|pins[20];
 endfunction
 assign clocks_after={2{root_clock(pins_after)}};
 assign clock_events=clocks_after & ~{2{root_clock(pins_before)}};
 integer bit_no;
 reg [15:0] capture_data;
 reg notify_task,preempt_task;
 reg [3:0] selected_task;
 wire [27:0] extended_pins=pins_after;
 always @* begin
  capture_data=0;is_next_n=0;ready_after=0;state_next=state_q;
  for(integer task_no=1;task_no<16;task_no=task_no+1) begin
   bit_no=4*((task_no-1)/4)+3-((task_no-1)%4);
   // BNT and BNT' are two separately stored MC10141 packages. Their
   // normal complementary relation is not a power-up/maintenance invariant.
   selected_task=pins_after[3:0];
   if(INDEPENDENT_BNT && task_no>=8) selected_task=~extended_pins[27:24];
   is_next_n[task_no-1]=pins_after[12] || task_number(selected_task)!=4'(task_no);
   notify_task=!pins_after[14] && task_number(pins_after[11:8])==4'(task_no);
   preempt_task=!pins_after[13] && task_number(pins_after[7:4])==4'(task_no);
   capture_data[bit_no]=(state_q[bit_no] && is_next_n[task_no-1]) || notify_task || preempt_task;
  end
  // F10000 MR is asynchronous. Unwired d09.D3 remains zero on load.
  if(pins_after[15]) state_next=0;
  else if(clock_events[0]) state_next=capture_data;
  for(integer task_no=1;task_no<16;task_no=task_no+1) begin
   bit_no=4*((task_no-1)/4)+3-((task_no-1)%4);
   ready_after[task_no-1]=state_next[bit_no];
  end
 end
endmodule
`default_nettype wire
