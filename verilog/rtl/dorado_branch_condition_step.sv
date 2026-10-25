// ProcH sheet11: per-task branch-condition file and visible flag/TIOA stages.
// HM §3 ALUF/ALU Operations: preserve carry/overflow on logical operations.
// Experimental component; ALU, common-owner and native phases remain external.
// Q[63:0]=e13, [67:64]=f17, [73:68]=g11, [79:74]=h11.
// The generated cone retains original gate wiring. Independently timed original
// storage is the capture-order oracle. No production reset or initial state.
`default_nettype none
module dorado_branch_condition_step #(parameter integer PINS=38,WAVES=3)(
 input wire [79:0] state_q,input wire clock_before,clock_after,
 input wire [PINS-1:0] pins_before,pins_after,
 output wire [79:0] state_next,output wire [3:0] clock_events,
 output wire unsettled);
 wire [79:0] q[0:WAVES];wire [3:0] before_clocks;
 wire [23:0] pre_data;
 branch_condition_cone before_view(.fixture_load(1'b0),.fixture_state(state_q),
  .base(clock_before),.start_a(1'b0),.en(1'b0),.pins(pins_before),.clocks(before_clocks));
 branch_condition_cone data_view(.fixture_load(1'b0),.fixture_state(state_q),
  .base(clock_before),.start_a(1'b0),.en(1'b0),.pins(pins_after),.capture_data(pre_data));
 assign q[0]=state_q;
 wire [3:0] final_clocks;
 for(genvar wave=0;wave<WAVES;wave++)begin: transfer
  wire [3:0] clocks;wire [23:0] data;reg [79:0] n;
  branch_condition_cone view(.fixture_load(1'b0),.fixture_state(q[wave]),
   .base(clock_after),.start_a(1'b0),.en(1'b0),.pins(pins_after),
   .clocks(clocks),.capture_data(data));
  always @*begin
   n=q[wave];
   if(wave==0)begin
    if(clocks[0] && !before_clocks[0])n[73:68]=pre_data[5:0];
    if(clocks[1] && !before_clocks[1])n[79:74]=pre_data[11:6];
   end
   // Original e13 has no wired chip-disable input. Retain its full LOW
   // write window and the data/address then present, including same-window
   // updates. Native publication still requires legal address/WE ordering.
   if(!clocks[3])n[4*data[19:16]+:4]=data[23:20];
   if(!clocks[2])n[67:64]=data[15:12];
  end
  assign q[wave+1]=n;
  if(wave==WAVES-1)assign final_clocks=clocks;
 end
 assign state_next=q[WAVES];
 assign clock_events=final_clocks & ~before_clocks;
 assign unsettled=(q[WAVES]!=q[WAVES-1]);
endmodule
`default_nettype wire
