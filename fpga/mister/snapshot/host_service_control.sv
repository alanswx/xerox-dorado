// Clocked Main-owned service permissions. Local device ACKs remain independent.
// Exact deselected packets bind build, board schema and a fresh lease nonce.
// A parser/PLL reset never releases an active or partially restored machine.
`default_nettype none
module dorado_snapshot_host_control #(
 parameter [63:0] BUILD_ID=0,
 parameter [63:0] SCHEMA_ID=0
)(
 input wire clk,machine_reseed,transport_reset,
 inout wire [35:0] bus,
 input wire command_hold,guest_held,global_hold,board_busy,board_fault,
 input wire [4:0] local_available,local_held,
 output wire host_hold,host_fault,
 output reg hold_update=0,hold_value=0,
 output reg [4:0] services_permitted=0,
 output reg [63:0] epoch=0,
 output reg [1:0] phase=0,
 output reg [31:0] errors=0
);
 localparam IDLE=2'd0,HELD=2'd1,RELEASING=2'd2,FAULT=2'd3;
 wire enable=bus[34],strobe=bus[33];wire [15:0] din=bus[31:16];
 reg [15:0] command=0,dout=0;reg [5:0] count=0;
 reg valid_packet=0;reg [63:0] pending_epoch=0;reg [4:0] pending_services=0;
 reg [1:0] phase_snapshot=0;reg [63:0] epoch_snapshot=0;
 reg [4:0] services_snapshot=0,available_snapshot=0,local_snapshot=0;
 reg [5:0] boundary_snapshot=0;reg [31:0] errors_snapshot=0;
 wire action=command>=16'h7b && command<=16'h7f;
 wire packet_ok=(valid_packet===1'b1) && count==16;
 wire safe_held=(command_hold===1'b1) && (guest_held===1'b1) &&
                (global_hold===1'b1) && (board_busy===1'b0) && (board_fault===1'b0);
 wire safe_released=(command_hold===1'b0) && (global_hold===1'b0) &&
                    (board_busy===1'b0) && (board_fault===1'b0);
 wire qualified_services=((pending_services & local_available)===pending_services) &&
                         ((pending_services & local_held)===pending_services);
 wire unhealthy=(phase==HELD && (command_hold!==1'b1 || guest_held!==1'b1)) ||
                (phase!=IDLE && board_fault!==1'b0) ||
                (phase==HELD && (((services_permitted & local_available)!==services_permitted) ||
                                ((services_permitted & local_held)!==services_permitted)));
 assign host_hold=phase==HELD || phase==FAULT;
 assign host_fault=phase==FAULT;
 assign bus[32]=enable && command>=16'h7a && command<=16'h7f;
 assign bus[15:0]=dout;
 // synthesis translate_off
 initial if(BUILD_ID==0 || SCHEMA_ID==0)$fatal(1,"Explicit snapshot build/schema required");
 // synthesis translate_on
 always @(posedge clk)begin
  hold_update<=0;
  if(machine_reseed===1'b1)begin
   phase<=IDLE;services_permitted<=0;epoch<=0;errors<=0;hold_value<=0;
   command<=0;count<=0;valid_packet<=0;pending_epoch<=0;pending_services<=0;dout<=0;
   phase_snapshot<=0;epoch_snapshot<=0;services_snapshot<=0;available_snapshot<=0;
   local_snapshot<=0;boundary_snapshot<=0;errors_snapshot<=0;
  end else if(transport_reset!==1'b0)begin
   command<=0;count<=0;valid_packet<=0;pending_epoch<=0;pending_services<=0;dout<=0;
   if(phase!=IDLE)begin
    phase<=FAULT;services_permitted<=0;
    if(command_hold!==1'b1)begin hold_update<=1;hold_value<=1;end
   end
  end else if(!enable)begin
   if(action)begin
    if(!packet_ok)errors<=errors+1'b1;
    else case(command)
     16'h7b:begin // Begin only after the original board hold is established.
      if(phase==IDLE && safe_held && pending_epoch!=0 && pending_epoch!=epoch)begin
       epoch<=pending_epoch;phase<=HELD;services_permitted<=0;
      end else errors<=errors+1'b1;
     end
     16'h7c:begin
      if(phase==HELD && safe_held && pending_epoch==epoch && qualified_services)services_permitted<=pending_services;
      else errors<=errors+1'b1;
     end
     16'h7d:begin // Retained native coordinators finish the physical release.
      if(phase==HELD && safe_held && pending_epoch==epoch)begin
       phase<=RELEASING;services_permitted<=0;hold_update<=1;hold_value<=0;
      end else errors<=errors+1'b1;
     end
     16'h7e:begin // Main sends this only after daemon/disk Continue ACKs.
      if(phase==RELEASING && safe_released && pending_epoch==epoch)phase<=IDLE;
      else errors<=errors+1'b1;
     end
     16'h7f:begin
      if(phase!=IDLE && pending_epoch==epoch)begin
       phase<=FAULT;services_permitted<=0;hold_update<=1;hold_value<=1;
      end
      else errors<=errors+1'b1;
     end
     default:errors<=errors+1'b1;
    endcase
   end
   command<=0;count<=0;valid_packet<=0;dout<=0;
  end else if(strobe)begin
   if(count!=63)count<=count+1'b1;
   if(count==0)begin
    command<=din;valid_packet<=1;pending_epoch<=0;pending_services<=0;
    if(din>=16'h7a && din<=16'h7f)dout<=16'hd0c0;else dout<=0;
    if(din==16'h7a)begin
     phase_snapshot<=phase;epoch_snapshot<=epoch;services_snapshot<=services_permitted;
     available_snapshot<=local_available;local_snapshot<=local_held & local_available;
     boundary_snapshot<={board_fault,board_busy,global_hold,guest_held,command_hold,host_hold};
     errors_snapshot<=errors;
    end
   end else if(command==16'h7a)begin
    case(count)
     1:dout<=1;
     2:dout<={14'b0,phase_snapshot};
     3:dout<=16'd5;
     4:dout<=BUILD_ID[15:0];5:dout<=BUILD_ID[31:16];6:dout<=BUILD_ID[47:32];7:dout<=BUILD_ID[63:48];
     8:dout<=SCHEMA_ID[15:0];9:dout<=SCHEMA_ID[31:16];10:dout<=SCHEMA_ID[47:32];11:dout<=SCHEMA_ID[63:48];
     12:dout<=epoch_snapshot[15:0];13:dout<=epoch_snapshot[31:16];14:dout<=epoch_snapshot[47:32];15:dout<=epoch_snapshot[63:48];
     16:dout<={11'b0,services_snapshot};17:dout<={11'b0,available_snapshot};18:dout<={11'b0,local_snapshot};
     19:dout<={10'b0,boundary_snapshot};20:dout<=errors_snapshot[15:0];21:dout<=errors_snapshot[31:16];
     default:dout<=0;
    endcase
   end else if(action)begin
    if((^din)===1'bx)valid_packet<=0;
    case(count)
     1:if(din!==16'hc04d)valid_packet<=0;
     2:if(din!==16'd1)valid_packet<=0;
     3:if(din!==BUILD_ID[15:0])valid_packet<=0;
     4:if(din!==BUILD_ID[31:16])valid_packet<=0;
     5:if(din!==BUILD_ID[47:32])valid_packet<=0;
     6:if(din!==BUILD_ID[63:48])valid_packet<=0;
     7:if(din!==SCHEMA_ID[15:0])valid_packet<=0;
     8:if(din!==SCHEMA_ID[31:16])valid_packet<=0;
     9:if(din!==SCHEMA_ID[47:32])valid_packet<=0;
     10:if(din!==SCHEMA_ID[63:48])valid_packet<=0;
     11:pending_epoch[15:0]<=din;12:pending_epoch[31:16]<=din;
     13:pending_epoch[47:32]<=din;14:pending_epoch[63:48]<=din;
     15:begin
      pending_services<=din[4:0];
      if((|din[15:5])!==1'b0 || (command!=16'h7c && din!==16'b0))valid_packet<=0;
     end
     default:valid_packet<=0;
    endcase
   end
  end
  // Fault retention has final priority while leaving status readable. Parser
  // reset above still discards the current packet and never clears a lease.
  if(machine_reseed!==1'b1 && transport_reset===1'b0 && unhealthy)begin
   phase<=FAULT;services_permitted<=0;hold_update<=0;
   if(command_hold!==1'b1)begin hold_update<=1;hold_value<=1;end
  end
 end
endmodule
`default_nettype wire
