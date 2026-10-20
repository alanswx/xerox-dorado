// MSA-capable ABI2: twelve slots, twelve frozen bits, 4096-cycle response budget.
// ABI2: command,key,ABI,schema[15:0],schema[31:16],schema[47:32],schema[63:48].
// 77 appends hold0/1 (8 words total);78 appends operation,board,index_lo,
// index_hi8,data_lo,data_hi2 (13 words). Only exact deselected packets commit.
// 76/79 status reports board-only scope until complete machine state exists.
`default_nettype none
module dorado_board_state_uio_msa #(
 parameter [63:0] SCHEMA_ID=64'b0,
 parameter integer SERVICE_DOMAINS=5,
 parameter integer RESPONSE_LIMIT=4096
)(
 input wire clk,system_reset,transport_reset,inout wire [35:0] bus,
 output reg hold_update=1'b0,hold_value=1'b0,
 input wire hold,command_hold,local_held,
 input wire [SERVICE_DOMAINS-1:0] services_held,
 input wire control_error,input wire [11:0] board_frozen,
 output reg request=1'b0,write=1'b0,output reg [3:0] board=0,
 output reg [23:0] index=0,output reg [17:0] data=0,
 input wire ready,done,input wire [2:0] error,input wire [17:0] result,input wire fault,
 output reg busy=1'b0,complete=1'b0,
 output reg [31:0] txn_sequence=0,control_errors=0
);
 wire enable=bus[34],strobe=bus[33];wire [15:0] din=bus[31:16];
 reg [15:0] command=0,dout=0;reg [11:0] count=0;
 reg valid_packet=0,mode=0,operation=0;reg [3:0] pending_board=0;
 reg [23:0] pending_index=0;reg [17:0] pending_data=0;
 reg [17:0] last_result=0;reg [2:0] last_error=0;
 reg [15:0] flags_snapshot=0;reg [11:0] frozen_snapshot=0;
 reg [31:0] sequence_snapshot=0,errors_snapshot=0;
 reg [17:0] result_snapshot=0;reg [2:0] error_snapshot=0;
 reg [SERVICE_DOMAINS-1:0] services_snapshot=0;
 wire is_status=command==16'h76 || command==16'h79;
 wire is_control=command==16'h77;
 wire is_request=command==16'h78;
 wire control_commit=!enable && is_control && valid_packet && count==8 && !busy && !fault;
 wire request_commit=!enable && is_request && valid_packet && count==13 && !busy && ready;
 wire packet_rejected=!enable && ((is_control && !control_commit) || (is_request && !request_commit));
 assign bus[32]=enable && command>=16'h76 && command<=16'h79;
 assign bus[15:0]=dout;
 // synthesis translate_off
 initial begin
  if(SCHEMA_ID==0 || SERVICE_DOMAINS<1 || SERVICE_DOMAINS>16 || RESPONSE_LIMIT>65535)
   $fatal(1,"Explicit nonzero schema and valid serial state UIO parameters required");
 end
 // synthesis translate_on
 always @(posedge clk)begin
  request<=0;hold_update<=0;
  if(system_reset || transport_reset)begin
   request<=0;hold_update<=0;hold_value<=0;write<=0;board<=0;index<=0;data<=0;
   busy<=0;complete<=0;txn_sequence<=0;control_errors<=0;command<=0;count<=0;
   last_result<=0;last_error<=0;valid_packet<=0;dout<=0;mode<=0;operation<=0;
   pending_board<=0;pending_index<=0;pending_data<=0;
  end else begin
   // A packet rejection and delayed owner rejection can coincide.
   control_errors<=control_errors+{31'b0,packet_rejected}+{31'b0,control_error};
   if(busy && done)begin busy<=0;complete<=1;last_result<=result;last_error<=error;end
   if(!enable)begin
    if(control_commit)begin hold_update<=1;hold_value<=mode;end
    if(request_commit)begin
     request<=1;write<=operation;board<=pending_board;index<=pending_index;data<=pending_data;
     busy<=1;complete<=0;txn_sequence<=txn_sequence+1'b1;last_error<=0;last_result<=0;
    end
    command<=0;count<=0;valid_packet<=0;dout<=0;
   end else if(strobe)begin
    if(count!=12'hfff)count<=count+1'b1;
    if(count==0)begin
     command<=din;valid_packet<=1;mode<=0;operation<=0;pending_board<=0;pending_index<=0;pending_data<=0;
     case(din)
      16'h76,16'h79:begin
       dout<=16'hd0b0;
       flags_snapshot<={5'b0,command_hold,(&services_held),1'b1,ready,fault,(|last_error),complete,busy,(&board_frozen),local_held,hold};
       frozen_snapshot<=board_frozen;sequence_snapshot<=txn_sequence;errors_snapshot<=control_errors;
       result_snapshot<=last_result;error_snapshot<=last_error;services_snapshot<=services_held;
      end
      16'h77,16'h78:dout<=16'hd0b0;
      default:dout<=0;
     endcase
    end else if(is_status)begin
     case(count)
      1:dout<=16'd2;
      2:dout<=flags_snapshot;
      3:dout<=16'd12;
      4:dout<={4'b0,frozen_snapshot};
      5:dout<=sequence_snapshot[15:0];
      6:dout<=sequence_snapshot[31:16];
      7:dout<=result_snapshot[15:0];
      8:dout<={14'b0,result_snapshot[17:16]};
      9:dout<={13'b0,error_snapshot};
      10:dout<=errors_snapshot[15:0];
      11:dout<=errors_snapshot[31:16];
      12:dout<=SCHEMA_ID[15:0];
      13:dout<=SCHEMA_ID[31:16];
      14:dout<=SCHEMA_ID[47:32];
      15:dout<=SCHEMA_ID[63:48];
      16:dout<={{(16-SERVICE_DOMAINS){1'b0}},services_snapshot};
      17:dout<=16'(RESPONSE_LIMIT);
      default:dout<=0;
     endcase
    end else if(is_control || is_request)begin
     case(count)
      1:if(din!=16'hb04d)valid_packet<=0;
      2:if(din!=16'd2)valid_packet<=0;
      3:if(din!=SCHEMA_ID[15:0])valid_packet<=0;
      4:if(din!=SCHEMA_ID[31:16])valid_packet<=0;
      5:if(din!=SCHEMA_ID[47:32])valid_packet<=0;
      6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;
      7:begin
       if(is_control)mode<=din[0];else operation<=din[0];
       if(din>1)valid_packet<=0;
      end
      8:begin pending_board<=din[3:0];if(is_control || |din[15:4])valid_packet<=0;end
      9:begin pending_index[15:0]<=din;if(is_control)valid_packet<=0;end
      10:begin pending_index[23:16]<=din[7:0];if(is_control || |din[15:8])valid_packet<=0;end
      11:begin pending_data[15:0]<=din;if(is_control)valid_packet<=0;end
      12:begin pending_data[17:16]<=din[1:0];if(is_control || |din[15:2])valid_packet<=0;end
      default:valid_packet<=0;
     endcase
    end
   end
  end
 end
endmodule
`default_nettype wire
