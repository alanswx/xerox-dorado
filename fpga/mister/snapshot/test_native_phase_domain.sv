`timescale 1ns/1ps
module tb;
 reg clk=1,reset=0,pack_ready=1,storage_initialized=1,snapshot_frontends_cached=1;
 reg pll_locked=1,snapshot_system_reset=0,snapshot_transport_reset=0;
 reg snapshot_hold_update=0,snapshot_hold_value=0,snapshot_request=0,snapshot_write=0;
 reg [3:0] snapshot_board=0;reg [23:0] snapshot_index=0;reg [17:0] snapshot_data=0;
 reg [4:0] permits=31;
 reg snapshot_frontends_ready=1,snapshot_arbiter_idle=1,snapshot_raw_idle=1;
 reg snapshot_network_idle=1,snapshot_network_parser_idle=1,snapshot_disk_idle=1;
 reg [3:0] net_ddr_fault=0,net_ddr_control_errors=0;
 reg pack_fault=0,pack_mounted=0,late_hold=0;
 reg snapshot_network_parser_held=0,snapshot_disk_held=0;
 reg [7:0] frontend_frozen=0;
 wire snapshot_hold,snapshot_owner_hold,snapshot_owner_fault,snapshot_fault,snapshot_scan_abort;
 wire snapshot_memory_fault,snapshot_frontend_hold,snapshot_memory_drained;
 wire snapshot_network_hold,snapshot_network_parser_hold,snapshot_network_drained,snapshot_network_fault;
 wire snapshot_disk_hold,snapshot_disk_request,snapshot_disk_drained,snapshot_disk_fault;
 wire snapshot_service_hold,snapshot_service_fault;
 wire snapshot_command_hold,snapshot_control_error,snapshot_ready,snapshot_done;
 wire [2:0] snapshot_error;wire [17:0] snapshot_result;
 wire [4:0] snapshot_services_held={permits[4:3],permits[2] && snapshot_network_drained,permits[1:0]};
 wire [4:0] snapshot_services_status;
 wire [11:0] snapshot_frozen,snapshot_board_request,snapshot_board_done,snapshot_board_error,snapshot_board_hold,snapshot_board_fault;
 wire [215:0] snapshot_board_result;
 wire [7:0] snapshot_msa_request,snapshot_msa_frozen,snapshot_msa_done,snapshot_msa_error,snapshot_msa_hold,snapshot_msa_fault;
 wire [143:0] snapshot_msa_result;wire [23:0] snapshot_msa_index;
 wire snapshot_msa_write;wire [17:0] snapshot_msa_data;
 wire [18:0] local_step,leaf_scan,leaf_hold;
 wire machine_enable=~reset && pack_ready && storage_initialized && snapshot_frontends_cached && !snapshot_hold;
 reg network_step=0;
 wire snapshot_guest_held=snapshot_hold && !network_step;
 reg hold_at_falling=0;
 integer checks=0,edges=0,scan_edges=0,guest_edges=0,requests=0;
 dorado_board_phase_enable copies(.clk(clk),.permit(machine_enable),.advance(local_step));
 always @(negedge clk)begin network_step<=machine_enable;hold_at_falling=snapshot_hold;end
 assign snapshot_service_hold=snapshot_network_hold;
 assign snapshot_service_fault=snapshot_network_fault;
 always @(posedge clk)begin
  snapshot_network_parser_held<=snapshot_network_parser_hold;
  snapshot_disk_held<=snapshot_disk_request;
  frontend_frozen<={8{snapshot_frontend_hold}};
  if(snapshot_hold!==hold_at_falling)$fatal(1,"native hold changed after falling edge");
  if(local_step!=={19{network_step}})$fatal(1,"native local phase differs");
  checks+=2;
 end
 // ACTUAL_NATIVE_CONTROL
 for(genvar b=0;b<19;b++)begin:leaves
  wire requested;
  wire [23:0] index;
  wire [17:0] data;
  wire writing,frozen,done,error,fault;
  wire [17:0] result;
  reg boundary=0;
  reg [37:0] original_bank=38'h348a19372,candidate_bank=38'h348a19372;
  wire state_hold=snapshot_hold || (snapshot_hold && snapshot_guest_held) || leaf_hold[b];
  if(b<11)begin
   assign requested=snapshot_board_request[b];assign index=snapshot_index;
   assign data=snapshot_data;assign writing=snapshot_write;
   assign snapshot_frozen[b]=frozen;assign snapshot_board_done[b]=done;
   assign snapshot_board_error[b]=error;assign snapshot_board_hold[b]=leaf_hold[b];
   assign snapshot_board_fault[b]=fault;assign snapshot_board_result[b*18+:18]=result;
   assign frozen=boundary;
  end else begin
   assign requested=snapshot_msa_request[b-11];assign index=snapshot_msa_index;
   assign data=snapshot_msa_data;assign writing=snapshot_msa_write;
   assign snapshot_msa_frozen[b-11]=frozen;assign snapshot_msa_done[b-11]=done;
   assign snapshot_msa_error[b-11]=error;assign snapshot_msa_hold[b-11]=leaf_hold[b];
   assign snapshot_msa_fault[b-11]=fault;assign snapshot_msa_result[(b-11)*18+:18]=result;
   assign frozen=boundary && frontend_frozen[b-11];
  end
  wire serial_in;
  dorado_ff_scan_controller #(.STATE_BITS(38)) scan(
   .clk(clk),.reset(snapshot_system_reset),.freeze(snapshot_hold && !snapshot_scan_abort),
   .guest_held(snapshot_guest_held),.request(requested && frozen && !snapshot_scan_abort),
   .write(writing),.index(index),.data(data),.serial_out(candidate_bank[0]),
   .serial_in(serial_in),.scan_step(leaf_scan[b]),.hold_required(leaf_hold[b]),
   .ready(),.busy(),.done(done),.error(error),.fault(fault),.result(result));
  always @(posedge clk)begin
   boundary<=state_hold && !snapshot_system_reset;
   if(network_step===1'b1 && state_hold!==1'b0)$fatal(1,"native advance/hold contract violated");
   if(leaf_scan[b]===1'b1 && state_hold!==1'b1)$fatal(1,"native scan/hold contract violated");
   if(state_hold)begin
    if(leaf_scan[b])original_bank<={serial_in,original_bank[37:1]};
   end else if(network_step)original_bank<=original_bank+1'b1;
   if(leaf_scan[b])candidate_bank<={serial_in,candidate_bank[37:1]};
   else if(local_step[b])candidate_bank<=candidate_bank+1'b1;
  end
  always @(negedge clk)begin
   if(original_bank!==candidate_bank)$fatal(1,"native model leaf state differs");
  end
 end
 task check(input bit ok,input string message);
  checks++;if(!ok)$fatal(1,"%s edge=%0d",message,edges);
 endtask
 task tick;
  #4;clk=0;#1;
  // Deliberately change external controls after the falling edge. Actual
  // native holds must still change only at the following positive edge.
  late_hold=~late_hold;
  #4;clk=1;#1;edges++;
  if(network_step===1'b1)guest_edges++;
  if(|leaf_scan)scan_edges++;
 endtask
 task reseed;
  snapshot_request=0;snapshot_hold_update=0;snapshot_transport_reset=0;
  reset=0;pll_locked=1;pack_ready=1;storage_initialized=1;snapshot_frontends_cached=1;
  snapshot_frontends_ready=1;snapshot_arbiter_idle=1;snapshot_raw_idle=1;
  snapshot_network_idle=1;snapshot_network_parser_idle=1;snapshot_disk_idle=1;
  net_ddr_fault=0;net_ddr_control_errors=0;pack_fault=0;pack_mounted=0;permits=31;
  snapshot_system_reset=1;repeat(4)tick();snapshot_system_reset=0;repeat(4)tick();
  check(!snapshot_hold,"native control reseed failed");
 endtask
 task enter;
  snapshot_hold_update=1;snapshot_hold_value=1;tick();snapshot_hold_update=0;
  for(integer age=0;!snapshot_ready;age++)begin tick();check(age<30,"native pause deadline");end
 endtask
 task leave;
  snapshot_hold_update=1;snapshot_hold_value=0;tick();snapshot_hold_update=0;
  repeat(16)tick();check(!snapshot_hold,"native release failed");
 endtask
 function automatic four(input integer value);
  case(value&3)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 initial begin
  reseed();repeat(64)tick();
  for(integer b=0;b<19;b++)begin
   enter();snapshot_board=b<11?b:11;snapshot_index=b<11?0:((b-11)<<8);
   snapshot_data=18'h293bc;snapshot_write=1;snapshot_request=1;tick();snapshot_request=0;requests++;
   for(integer age=0;!snapshot_done;age++)begin tick();check(age<50,"native scan deadline");end
   check(snapshot_error==0,"native scan failed");leave();
  end
  for(integer n=0;n<1024;n++)begin
   reset=four(n);pack_ready=four(n>>2);storage_initialized=four(n>>4);
   snapshot_frontends_cached=four(n>>6);tick();
  end
  reseed();
  if($test$plusargs("random"))begin
  for(integer n=0;n<8192;n++)begin
   snapshot_hold_update=n[2];snapshot_hold_value=four(n>>3);
   snapshot_transport_reset=n[6] && n[5];snapshot_request=n[7];
   snapshot_board=n%12;snapshot_index=0;snapshot_write=n[4];snapshot_data=n;
   permits[0]=four(n>>2);snapshot_frontends_ready=four(n>>4);
   snapshot_arbiter_idle=four(n>>6);snapshot_raw_idle=four(n>>8);
   net_ddr_fault[0]=four(n>>10);pack_fault=four(n>>12);tick();
  end
  end else begin
  enter();snapshot_board=11;snapshot_index=24'h300;snapshot_request=1;tick();snapshot_request=0;
  repeat(7)tick();check(|leaf_hold,"native partial import not active");
  snapshot_transport_reset=1;tick();snapshot_transport_reset=0;
  repeat(64)begin tick();check(snapshot_hold && local_step===19'b0,"native quarantine escaped");end
  end
  check(guest_edges>100 && scan_edges>700 && requests==19,"native phase coverage missing");
  $display("NATIVE PHASE DOMAIN PASS checks=%0d edges=%0d guest=%0d scan=%0d requests=%0d leaves=19 fourstate=1 partial_import_quarantine=%0d",
   checks,edges,guest_edges,scan_edges,requests,!$test$plusargs("random"));$finish;
 end
 initial begin #1000000;$fatal(1,"native phase test deadline");end
endmodule
