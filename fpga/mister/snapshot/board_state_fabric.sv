// Bounded access to all eleven qualified board adapters. This freezes board
// state only; wrapper/backend/input/ARM state requires the later global barrier.
`default_nettype none
module dorado_board_state_fabric (
 input wire clk,reset,freeze,guest_held,
 input wire request,write,input wire [3:0] board,input wire [23:0] index,
 input wire [17:0] data,output wire ready,
 output reg done=1'b0,output reg [2:0] error=0,output reg [17:0] result=0,
 output reg fault=1'b0,
 input wire [10:0] board_frozen,board_done,board_error,
 input wire [197:0] board_result,
 output wire [10:0] board_request
);
 reg pending=1'b0,was_write=1'b0;reg [3:0] owner=0,age=0;
 wire boundary=freeze && guest_held && (&board_frozen);
 assign ready=boundary && !pending && !fault && !reset;
 wire accept=request && ready;
 reg index_valid;
 always_comb begin
  case(board)
   0:index_valid=!(|index[23:12]); // ContA
   1:index_valid=!(|index[23:15]); // ContB
   2,3:index_valid=!(|index[23:16]); // ProcH/ProcL
   4:index_valid=!(|index[23:18]); // IFU
   5:index_valid=!(|index[23:16]); // MemC
   6:index_valid=!(|index[23:20]); // MemD
   7:index_valid=1'b1; // MemX uses the entire24bit address
   8:index_valid=!(|index[23:13]); // DskEth
   9:index_valid=!(|index[23:19]); // DispY
   10:index_valid=!(|index[23:17]); // BaseBoard
   default:index_valid=1'b0;
  endcase
 end
 assign board_request=(accept && board<11 && index_valid)?(11'b1<<board):11'b0;
 always @(posedge clk)begin
  done<=0;
  if(reset)begin pending<=0;done<=0;error<=0;result<=0;fault<=0;owner<=0;was_write<=0;age<=0;end
  else begin
   if(!freeze)fault<=0;
   if(pending)begin
    age<=age+1'b1;
    if(!boundary)begin
     pending<=0;done<=1;error<=3'd5;result<=0;fault<=1;
    end else if(board_done[owner])begin
     pending<=0;done<=1;error<=board_error[owner]?3'd3:3'd0;
     result<=(board_error[owner] || was_write)?18'b0:board_result[owner*18+:18];
    end else if(age==15)begin
     pending<=0;done<=1;error<=3'd4;result<=0;fault<=1;
    end
   end else if(accept)begin
    error<=0;result<=0;
    if(board>=11)begin done<=1;error<=3'd1;end
    else if(!index_valid)begin done<=1;error<=3'd2;end
    else begin owner<=board;was_write<=write;pending<=1;age<=0;end
   end
  end
 end
endmodule
`default_nettype wire
