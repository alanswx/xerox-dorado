// Serial board-state owner. SYSTEM reset is a machine lifecycle event;
// transport reset only resets a parser and must never release shifted state.
// Domain acknowledgments must come from actual quiescent implementations.
// This contract alone does not snapshot native wrappers, RAM or ARM services.
`default_nettype none
module dorado_board_state_fabric_serial #(
 parameter integer RESPONSE_LIMIT=2048,
 parameter integer SERVICE_DOMAINS=5
)(
 input wire clk,system_reset,transport_reset,
 input wire hold_update,hold_value,
 input wire local_held,input wire [SERVICE_DOMAINS-1:0] services_held,
 output wire hold,scan_abort,
 output reg command_hold=1'b0,control_error=1'b0,
 input wire request,write,input wire [3:0] board,input wire [23:0] index,
 input wire [17:0] data,output wire ready,
 output reg done=1'b0,output reg [2:0] error=0,output reg [17:0] result=0,
 output reg fault=1'b0,
 input wire [10:0] board_frozen,board_done,board_error,
 input wire [10:0] board_hold_required,board_fault,
 input wire [197:0] board_result,
 output wire [10:0] board_request
);
 localparam integer AGE_BITS=RESPONSE_LIMIT<2?1:$clog2(RESPONSE_LIMIT);
 reg pending=1'b0,was_write=1'b0;
 reg [3:0] owner=0;
 reg [AGE_BITS-1:0] age=0;
 // Case equality fails closed on unknown acknowledgments in four-state
 // simulation; on the FPGA these are ordinary positive-level predicates.
 wire boundary=(local_held===1'b1) && (board_frozen===11'h7ff) &&
               (services_held==={SERVICE_DOMAINS{1'b1}});
 wire children_holding=|board_hold_required;
 wire children_faulted=|board_fault;
 wire expired=pending && age==RESPONSE_LIMIT-1 && !board_done[owner];
 assign hold=command_hold || pending || fault || children_holding || children_faulted;
 // Integration must apply hold to EVERY consumer and scan_abort to the
 // scan controller only. Aborting must not unfreeze RAM/FF helpers.
 assign scan_abort=fault || children_faulted ||
                   (pending && (!boundary || expired)) ||
                   (transport_reset && hold);
 assign ready=command_hold && boundary && !pending && !fault &&
              !children_holding && !children_faulted && !hold_update &&
              !system_reset && !transport_reset;
 wire accept=request && ready;
 reg index_valid;
 always_comb begin
  case(board)
   0:index_valid=!(|index[23:12]);
   1:index_valid=!(|index[23:15]);
   2,3:index_valid=!(|index[23:16]);
   4:index_valid=!(|index[23:18]);
   5:index_valid=!(|index[23:16]);
   6:index_valid=!(|index[23:20]);
   7:index_valid=1'b1;
   8:index_valid=!(|index[23:13]);
   9:index_valid=!(|index[23:19]);
   10:index_valid=!(|index[23:17]);
   default:index_valid=1'b0;
  endcase
 end
 assign board_request=(accept && board<11 && index_valid)?(11'b1<<board):11'b0;
 // synthesis translate_off
 initial begin
  if(RESPONSE_LIMIT<2 || SERVICE_DOMAINS<1 || SERVICE_DOMAINS>16)
   $fatal(1,"Invalid serial state owner parameters");
 end
 // synthesis translate_on
 always @(posedge clk)begin
  done<=0;control_error<=0;
  if(system_reset)begin
   command_hold<=0;pending<=0;done<=0;error<=0;result<=0;fault<=0;
   control_error<=0;owner<=0;was_write<=0;age<=0;
  end else if(transport_reset)begin
   // A parser reset is never a thaw or recovery operation.
   if(hold)begin
    fault<=1;pending<=0;done<=1;error<=3'd6;result<=0;
   end
  end else begin
   if(hold_update)begin
    if(fault || children_faulted || pending || children_holding)
     control_error<=1;
    else command_hold<=hold_value;
   end
   if(children_faulted)begin
    fault<=1;
    if(pending)begin pending<=0;done<=1;error<=3'd5;result<=0;end
   end else if(pending)begin
    if(!boundary)begin
     pending<=0;done<=1;error<=3'd5;result<=0;fault<=1;
    end else if(board_done[owner])begin
     pending<=0;done<=1;error<=board_error[owner]?3'd3:3'd0;
     result<=(board_error[owner] || was_write)?18'b0:board_result[owner*18+:18];
    end else if(age==RESPONSE_LIMIT-1)begin
     pending<=0;done<=1;error<=3'd4;result<=0;fault<=1;
    end else age<=age+1'b1;
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
