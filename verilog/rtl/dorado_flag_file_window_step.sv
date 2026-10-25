// Native F10145A storage transfer, with its existing write-window history.
// Inputs are the actual pins BEFORE the enabled fabric edge. Window layout
// is the native cell's {ce_lat,d_lat,a_lat,we_d}; memory is physical slot order.
// Stateless: the common owner must retain all74 existing storage/history bits.
// This does not implement the independent original level-write RAM model.
`default_nettype none
module dorado_flag_file_window_step(
 input wire [63:0] memory_before,input wire [9:0] window_before,
 input wire advance,input wire [3:0] address,data,input wire chip_n,write_n,
 output reg [63:0] memory_after,output reg [9:0] window_after);
 always @*begin
  memory_after=memory_before;window_after=window_before;
  if(advance)begin
   window_after[0]=write_n;
   if(!write_n)begin
    window_after[4:1]=address;
    window_after[8:5]=data;
    window_after[9]=~chip_n;
   end
   if(write_n && !window_before[0] && window_before[9])
    memory_after[4*window_before[4:1]+:4]=window_before[8:5];
  end
 end
endmodule
`default_nettype wire
