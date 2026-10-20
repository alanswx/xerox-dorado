`default_nettype none
module snapshot_host_top(
 input wire clk,machine_reseed,transport_reset,enable,strobe,
 input wire [15:0] din,output wire [15:0] dout,output wire ack,
 input wire set_hold,manual_hold,guest_held,global_hold,board_busy,board_fault,
 input wire [4:0] local_available,local_held,
 output reg command_hold=0,
 output wire host_hold,host_fault,
 output wire [4:0] services,output wire [63:0] epoch,
 output wire [1:0] phase,output wire [31:0] errors
);
 wire [35:0] bus,legacy_bus;
 assign bus[35:33]={1'b0,enable,strobe};assign bus[31:16]=din;
 assign legacy_bus[35:33]={1'b0,enable,strobe};assign legacy_bus[31:16]=din;
 assign dout=bus[32]?bus[15:0]:legacy_bus[32]?legacy_bus[15:0]:16'b0;
 assign ack=bus[32] || legacy_bus[32];
 wire hold_update,hold_value;
 wire legacy_hold_update,legacy_hold_value;
 always @(posedge clk)begin
  if(machine_reseed)command_hold<=0;
  else if(set_hold)command_hold<=manual_hold;
  else if(hold_update)command_hold<=hold_value;
  else if(legacy_hold_update)command_hold<=legacy_hold_value;
 end
 dorado_board_state_uio_msa #(.SCHEMA_ID(64'haf0b647cb43a3a62),.LOCAL_SERVICES_MASK(5'b00111)) legacy(
  .clk(clk),.system_reset(machine_reseed),.transport_reset(transport_reset),.bus(legacy_bus),
  .hold_update(legacy_hold_update),.hold_value(legacy_hold_value),
  .hold(global_hold),.command_hold(command_hold),.local_held(guest_held),
  .services_held(services),.local_services_held(local_held),.control_error(1'b0),
  .board_frozen({12{guest_held}}),.request(),.write(),.board(),.index(),.data(),
  .ready(command_hold && global_hold && (&services)),.done(1'b0),.error(3'b0),.result(18'b0),
  .fault(board_fault),.busy(),.complete(),.txn_sequence(),.control_errors());
 dorado_snapshot_host_control #(.BUILD_ID(64'hb49c378fd137862a),.SCHEMA_ID(64'haf0b647cb43a3a62)) controller(
  .clk(clk),.machine_reseed(machine_reseed),.transport_reset(transport_reset),.bus(bus),
  .command_hold(command_hold),.guest_held(guest_held),.global_hold(global_hold),
  .board_busy(board_busy),.board_fault(board_fault),.local_available(local_available),.local_held(local_held),
  .host_hold(host_hold),.host_fault(host_fault),.hold_update(hold_update),.hold_value(hold_value),
  .services_permitted(services),.epoch(epoch),.phase(phase),.errors(errors));
endmodule
`default_nettype wire
