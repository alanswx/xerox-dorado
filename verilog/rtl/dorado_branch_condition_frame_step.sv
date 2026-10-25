// Experimental coupled-processor transfer. An early frame carries direct
// input arrivals with the pre-edge registered producers; the late frame
// carries their settled post-edge values. These are combinational epochs,
// not extra guest cycles. Native producer arrivals still require qualification.
// Q layout matches dorado_branch_condition_step; no reset or initial state.
`default_nettype none
module dorado_branch_condition_frame_step #(parameter integer PINS=38,WAVES=3)(
 input wire [79:0] state_q,input wire clock_before,clock_after,
 input wire [PINS-1:0] pins_before,pins_early,pins_after,
 output wire [79:0] state_next,output wire [3:0] clock_events,
 output wire unsettled);
 wire [79:0] q[0:WAVES];wire [3:0] before_clocks,final_clocks;
 wire [23:0] early_data;
 branch_condition_cone before_view(.fixture_load(1'b0),.fixture_state(state_q),
  .base(clock_before),.start_a(1'b0),.en(1'b0),.pins(pins_before),.clocks(before_clocks));
 branch_condition_cone early_view(.fixture_load(1'b0),.fixture_state(state_q),
  .base(clock_before),.start_a(1'b0),.en(1'b0),.pins(pins_early),.capture_data(early_data));
 assign q[0]=state_q;
 for(genvar wave=0;wave<WAVES;wave++)begin:transfer
  wire [3:0] clocks;wire [23:0] data;reg [79:0] n;
  branch_condition_cone late_view(.fixture_load(1'b0),.fixture_state(q[wave]),
   .base(clock_after),.start_a(1'b0),.en(1'b0),.pins(pins_after),
   .clocks(clocks),.capture_data(data));
  always @*begin
   n=q[wave];
   if(wave==0)begin
    if(clocks[0] && !before_clocks[0])n[73:68]=early_data[5:0];
    if(clocks[1] && !before_clocks[1])n[79:74]=early_data[11:6];
    // Direct D can arrive while f17 is still transparent, before its C
    // closes. Late task-address/file changes must not replace that data.
    if(!before_clocks[2])n[67:64]=early_data[15:12];
   end
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
