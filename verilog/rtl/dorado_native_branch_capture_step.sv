// Exact native storage/history transfer for ProcH e13/f17/g11/h11.
// [79:0] keeps the original physical flag layout. [89:80] is e13's existing
// write window; [96:90]/[103:97] are g11/h11 {ck_d,d_pre}, respectively.
// clocks/data are the actual package pins BEFORE the enabled fabric edge,
// including native clock buffers. This block adds no state, reset or clock.
`default_nettype none
module dorado_native_branch_capture_step(
 input wire [103:0] state_before,input wire advance,input wire [3:0] clocks,
 // Same data grouping as the original branch cone: g11,h11,selected f17,
 // physical file address, physical file data, low group first.
 input wire [23:0] data,output reg [103:0] state_after);
 wire [63:0] memory_next;wire [9:0] window_next;
 dorado_flag_file_window_step file_step(state_before[63:0],state_before[89:80],advance,
  data[19:16],data[23:20],1'b0,clocks[3],memory_next,window_next);
 always @*begin
  state_after=state_before;
  state_after[63:0]=memory_next;state_after[89:80]=window_next;
  if(advance)begin
   if(!clocks[2])state_after[67:64]=data[15:12];
   if(clocks[0] && !state_before[96])state_after[73:68]=state_before[95:90];
   if(clocks[1] && !state_before[103])state_after[79:74]=state_before[102:97];
   state_after[96:90]={clocks[0],data[5:0]};
   state_after[103:97]={clocks[1],data[11:6]};
  end
 end
endmodule
`default_nettype wire
