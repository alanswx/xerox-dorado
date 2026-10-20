// Slot11: index[10:8] selects one of eight MSAs; index[7:0] is word0..149.
// All eight members and every foreign consumer must be held. The selected
// member is latched through completion; a parser reset cannot release it.
// This router does not implement native memory, input or ARM pause barriers.
`default_nettype none
module dorado_msa_state_group(
 input wire clk,system_reset,transport_reset,scan_abort,
 input wire request,write,input wire [23:0] index,input wire [17:0] data,
 input wire [7:0] unit_frozen,unit_done,unit_error,unit_hold_required,unit_fault,
 input wire [143:0] unit_result,
 output wire frozen,hold_required,fault,
 output reg done=1'b0,error=1'b0,output reg [17:0] result=0,
 output wire [7:0] unit_request,output wire [23:0] unit_index,
 output wire unit_write,output wire [17:0] unit_data
);
 reg pending=1'b0,was_write=1'b0,local_fault=1'b0;
 reg [2:0] owner=0;
 reg [7:0] route;
 always_comb begin
  case(index[10:8])
   0:route=8'h01;1:route=8'h02;2:route=8'h04;3:route=8'h08;
   4:route=8'h10;5:route=8'h20;6:route=8'h40;7:route=8'h80;
   default:route=0;
  endcase
 end
 wire valid_index=(index[23:11]===13'b0) && ((index[7:0]<8'd150)===1'b1) && (|route);
 wire children_holding=(unit_hold_required!==8'b0);
 wire children_faulted=(unit_fault!==8'b0);
 assign frozen=(unit_frozen===8'hff);
 assign hold_required=pending || local_fault || children_holding || children_faulted;
 assign fault=local_fault || children_faulted;
 wire admit=request && frozen && !pending && !local_fault && !children_holding &&
            !children_faulted && !scan_abort && !system_reset && !transport_reset;
 assign unit_request=(admit && valid_index)?route:8'b0;
 assign unit_index={16'b0,index[7:0]};
 assign unit_write=write;
 assign unit_data=data;
 always @(posedge clk)begin
  done<=0;
  if(system_reset)begin
   pending<=0;was_write<=0;local_fault<=0;owner<=0;done<=0;error<=0;result<=0;
  end else if(transport_reset)begin
   if(pending || local_fault || children_holding || children_faulted)begin
    pending<=0;local_fault<=1;done<=1;error<=1;result<=0;
   end
  end else if(children_faulted || (pending && (!frozen || scan_abort)))begin
   local_fault<=1;
   if(pending)begin pending<=0;done<=1;error<=1;result<=0;end
  end else if(pending)begin
   if(unit_done[owner])begin
    pending<=0;done<=1;error<=unit_error[owner];
    result<=(was_write || unit_error[owner])?18'b0:unit_result[owner*18+:18];
   end
  end else if(admit)begin
   error<=0;result<=0;
   if(!valid_index)begin done<=1;error<=1;end
   else begin pending<=1;was_write<=write;owner<=index[10:8];end
  end
 end
endmodule
`default_nettype wire
