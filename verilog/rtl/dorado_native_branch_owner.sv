// Common owner for the native ProcH e13/f17/g11/h11 storage frontier.
// HM 2.3/4.9: sample actual pre-edge package clocks/data; retain the existing
// PRECAP and write-close history. No guest reset, initialization or extra tick.
`default_nettype none
module dorado_native_branch_owner(
 input wire clk,advance,input wire [3:0] clocks,input wire [23:0] data,
 input wire ff_hold,input wire [2:0] ff_write,input wire [17:0] ff_data,
 input wire file_freeze,file_guest_held,file_request,file_write,
 input wire [4:0] file_index,input wire [9:0] file_data,
 output wire [103:0] physical_state,output wire [3:0] file_read,
 output wire [17:0] f17_result,g11_result,h11_result,
 output reg file_frozen=1'b0,output reg file_done=1'b0,output reg file_error=1'b0,
 output reg [9:0] file_result);
 reg [103:0] q;
 wire [103:0] next_q;
 wire file_hold=file_freeze && file_guest_held;
 wire file_accept=file_hold && file_frozen && file_request;
 wire file_valid=file_index<=5'd16;
 dorado_native_branch_capture_step transfer(q,advance,clocks,data,next_q);
 assign physical_state=q;
 assign file_read=q[4*data[19:16]+:4];
 assign f17_result={14'b0,q[67:64]};
 assign g11_result={5'b0,q[96:90],q[73:68]};
 assign h11_result={5'b0,q[103:97],q[79:74]};
 always @(posedge clk)begin
  file_frozen<=file_hold;file_done<=file_accept;file_error<=file_accept && !file_valid;
  if(file_accept)begin
   file_result<=file_index==5'd16 ? q[89:80]:
                (file_index<5'd16 ? {6'b0,q[4*file_index[3:0]+:4]}:10'b0);
   if(file_write && file_valid)begin
    if(file_index==5'd16)q[89:80]<=file_data;
    else q[4*file_index[3:0]+:4]<=file_data[3:0];
   end
  end
  if(!file_hold && advance)begin q[63:0]<=next_q[63:0];q[89:80]<=next_q[89:80];end
  if(ff_hold)begin
   if(ff_write[0])q[67:64]<=ff_data[3:0];
   if(ff_write[1])begin q[73:68]<=ff_data[5:0];q[96:90]<=ff_data[12:6];end
   if(ff_write[2])begin q[79:74]<=ff_data[5:0];q[103:97]<=ff_data[12:6];end
  end else if(advance)begin
   q[79:64]<=next_q[79:64];q[103:90]<=next_q[103:90];
  end
 end
endmodule
`default_nettype wire
