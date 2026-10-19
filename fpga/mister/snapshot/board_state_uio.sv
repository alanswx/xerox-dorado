// Diagnostic board-only access on MiSTer EXT_BUS, separate from Ethernet70..75.
// 76/79 coherent status.77 boundary control: B04D,mode0/1,ABI1.
// 78 request: B04D,read0/write1,board,index_lo,index_hi8,data_lo,data_hi2.
// Commands mutate state only on deselection after the exact valid word count.
// This is NOT an atomic whole-machine snapshot service.
`default_nettype none
module dorado_board_state_uio (
 input wire clk,reset,inout wire [35:0] bus,
 output reg freeze=1'b0,input wire guest_held,
 input wire [10:0] board_frozen,
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
 reg [15:0] flags_snapshot=0;reg [10:0] frozen_snapshot=0;
 reg [31:0] sequence_snapshot=0,errors_snapshot=0;
 reg [17:0] result_snapshot=0;reg [2:0] error_snapshot=0;
 assign bus[32]=enable && command>=16'h76 && command<=16'h79;
 assign bus[15:0]=dout;
 always @(posedge clk)begin
  request<=0;
  if(reset)begin
   freeze<=0;request<=0;write<=0;board<=0;index<=0;data<=0;
   busy<=0;complete<=0;txn_sequence<=0;control_errors<=0;command<=0;count<=0;
   last_result<=0;last_error<=0;valid_packet<=0;dout<=0;
  end else begin
   if(busy && done)begin busy<=0;complete<=1;last_result<=result;last_error<=error;end
   if(!enable)begin
    if(command==16'h77)begin
     if(valid_packet && count==4 && !busy)freeze<=mode;
     else control_errors<=control_errors+1'b1;
    end
    if(command==16'h78)begin
     if(valid_packet && count==8 && !busy && ready)begin
      request<=1;write<=operation;board<=pending_board;index<=pending_index;data<=pending_data;
      busy<=1;complete<=0;txn_sequence<=txn_sequence+1'b1;last_error<=0;last_result<=0;
     end else control_errors<=control_errors+1'b1;
    end
    command<=0;count<=0;valid_packet<=0;dout<=0;
   end else if(strobe)begin
    if(count!=12'hfff)count<=count+1'b1;
    if(count==0)begin
     command<=din;valid_packet<=1;mode<=0;operation<=0;pending_board<=0;pending_index<=0;pending_data<=0;
     case(din)
      16'h76,16'h79:begin
       dout<=16'hd0b0;
       // Bit8 permanently identifies this restricted board-state scope.
       flags_snapshot<={7'b0,1'b1,1'b0,fault,(|last_error),complete,busy,(&board_frozen),guest_held,freeze};
       frozen_snapshot<=board_frozen;sequence_snapshot<=txn_sequence;errors_snapshot<=control_errors;
       result_snapshot<=last_result;error_snapshot<=last_error;
      end
      16'h77,16'h78:dout<=16'hd0b0;
      default:dout<=0;
     endcase
    end else case(command)
     16'h76,16'h79:case(count)
      1:dout<=16'd1;
      2:dout<=flags_snapshot;
      3:dout<=16'd11;
      4:dout<={5'b0,frozen_snapshot};
      5:dout<=sequence_snapshot[15:0];
      6:dout<=sequence_snapshot[31:16];
      7:dout<=result_snapshot[15:0];
      8:dout<={14'b0,result_snapshot[17:16]};
      9:dout<={13'b0,error_snapshot};
      10:dout<=errors_snapshot[15:0];
      11:dout<=errors_snapshot[31:16];
      default:dout<=0;
     endcase
     16'h77:case(count)
      1:if(din!=16'hb04d)valid_packet<=0;
      2:begin mode<=din[0];if(din>1)valid_packet<=0;end
      3:if(din!=1)valid_packet<=0;
      default:valid_packet<=0;
     endcase
     16'h78:case(count)
      1:if(din!=16'hb04d)valid_packet<=0;
      2:begin operation<=din[0];if(din>1)valid_packet<=0;end
      3:begin pending_board<=din[3:0];if(|din[15:4])valid_packet<=0;end
      4:pending_index[15:0]<=din;
      5:begin pending_index[23:16]<=din[7:0];if(|din[15:8])valid_packet<=0;end
      6:pending_data[15:0]<=din;
      7:begin pending_data[17:16]<=din[1:0];if(|din[15:2])valid_packet<=0;end
      default:valid_packet<=0;
     endcase
     default: ;
    endcase
   end
  end
 end
endmodule
`default_nettype wire
