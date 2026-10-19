// Contract fixture: real 2004-bit scan controller, stub other boards/services.
// Native wrapper/backend/ARM quiesce implementations are not modeled here.
module board_state_serial_fixture;
 localparam [63:0] SCHEMA=64'h79c5a16e42d38b07;
 reg clk=0,system_reset=1,transport_reset=0,enable=0,strobe=0;
 reg [15:0] din=0;
 wire [35:0] hps,eth_bus,state_bus;
 assign hps[35:33]={1'b0,enable,strobe};assign hps[31:16]=din;
 assign eth_bus[35:33]=hps[35:33];assign eth_bus[31:16]=hps[31:16];
 assign state_bus[35:33]=hps[35:33];assign state_bus[31:16]=hps[31:16];
 assign hps[32]=eth_bus[32]|state_bus[32];
 assign hps[15:0]=eth_bus[32]?eth_bus[15:0]:state_bus[32]?state_bus[15:0]:16'b0;
 wire hold,command_hold,scan_abort,hold_update,hold_value,control_error;
 wire u_request,u_write,port_ready,port_done,port_fault,busy,complete;
 wire [3:0] u_board;wire [23:0] u_index;wire [17:0] u_data,port_result;
 wire [2:0] port_error;wire [31:0] txn_sequence,control_errors;
 reg local_held=0;reg [4:0] services_held=0;
 reg [4:0] missing_services=0;reg local_missing=0;
 reg [10:0] suppress_frozen=0,suppress_reply=0,inject_error=0,inject_hold=0,inject_fault=0;
 reg [2003:0] chain=0;wire long_step,long_in,long_ready,long_hold,long_busy,long_done,long_error,long_fault;
 wire [17:0] long_result;
 reg [9:0] stub_done=0,stub_error=0;reg [10:0] board_frozen=0;
 wire [10:0] board_done={long_done,stub_done},board_error={long_error,stub_error};
 wire [10:0] board_holds=inject_hold|{long_hold,10'b0};
 wire [10:0] board_faults=inject_fault|{long_fault,10'b0};
 reg [179:0] stub_result=0;wire [197:0] board_result={long_result,stub_result};
 wire [10:0] board_request;
 reg direct=0,d_request=0,d_write=0,d_hold_update=0,d_hold_value=0;
 reg [3:0] d_board=0;reg [23:0] d_index=0;reg [17:0] d_data=0;
 wire request=direct?d_request:u_request,wr=direct?d_write:u_write;
 wire [3:0] selected=direct?d_board:u_board;
 wire [23:0] index=direct?d_index:u_index;wire [17:0] data=direct?d_data:u_data;
 wire update_hold=direct?d_hold_update:hold_update,value_hold=direct?d_hold_value:hold_value;
 dorado_board_state_uio_serial #(.SCHEMA_ID(SCHEMA)) uio(
  .clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),.bus(state_bus),
  .hold_update(hold_update),.hold_value(hold_value),.hold(hold),.command_hold(command_hold),
  .local_held(local_held),.services_held(services_held),.control_error(control_error),.board_frozen(board_frozen),
  .request(u_request),.write(u_write),.board(u_board),.index(u_index),.data(u_data),
  .ready(port_ready),.done(port_done),.error(port_error),.result(port_result),.fault(port_fault),
  .busy(busy),.complete(complete),.txn_sequence(txn_sequence),.control_errors(control_errors));
 dorado_board_state_fabric_serial fabric(
  .clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),
  .hold_update(update_hold),.hold_value(value_hold),.local_held(local_held),.services_held(services_held),
  .hold(hold),.scan_abort(scan_abort),.command_hold(command_hold),.control_error(control_error),
  .request(request),.write(wr),.board(selected),.index(index),.data(data),
  .ready(port_ready),.done(port_done),.error(port_error),.result(port_result),.fault(port_fault),
  .board_frozen(board_frozen),.board_done(board_done),.board_error(board_error),
  .board_hold_required(board_holds),.board_fault(board_faults),.board_result(board_result),.board_request(board_request));
 dorado_ethernet_uio #(.MAX_WORDS(1024),.DDR_CAPABLE(1)) ethernet(
  .clk(clk),.reset(system_reset),.bus(eth_bus),.rx_ready(1'b1),.tx_valid(1'b1),.tx_words(11'd2),.tx_data(16'h1234),
  .tx_good(32'h12345678),.tx_bad(32'b0),.tx_dropped(32'b0),.rx_bad(32'b0),
  .machine_debug(32'hdeadbeef),.processor_debug(32'h01234567),.machine_status(16'h4321),
  .tx_addr(),.rx_addr(),.tx_ack(),.rx_wr(),.rx_commit(),.rx_data(),.rx_words(),.aborted(),
  .ddr_stopped(1'b1),.ddr_initialized(1'b0),.ddr_fault(32'b0),.ddr_enable(),.ddr_epoch(),.ddr_control_errors(),.ddr_owned());
 dorado_ff_scan_controller #(.STATE_BITS(2004)) long_scan(
  .clk(clk),.reset(system_reset),.freeze(hold && !scan_abort),.guest_held(local_held),
  .request(board_request[10]),.write(wr),.index(index),.data(data),
  .serial_out(chain[0]),.serial_in(long_in),.scan_step(long_step),.ready(long_ready),
  .hold_required(long_hold),.busy(long_busy),.done(long_done),.error(long_error),.fault(long_fault),.result(long_result));
 integer k;reg [17:0] content[0:9];integer calls[0:10];
 integer delay_cycles[0:9],countdown[0:9];reg [9:0] delayed=0;reg [17:0] delayed_value[0:9];
 reg [31:0] normal_steps=0,service_steps[0:4];
 always @(negedge clk)begin
  local_held<=hold && !local_missing;
  services_held<={5{hold}}&~missing_services;
 end
 always @(posedge clk)begin
  stub_done<=0;stub_error<=0;
  board_frozen<={11{hold&&local_held}}&~suppress_frozen;
  if((board_request & (board_request-1))!=0)$fatal(1,"nononehot child request");
  if((|board_request) && fabric.pending)$fatal(1,"busy transaction selected another child");
  if(|board_request)begin
   if(!hold||!local_held||services_held!==5'h1f||board_frozen!==11'h7ff)$fatal(1,"request before complete boundary");
  end
  if(system_reset)begin
   chain<=0;normal_steps<=0;delayed<=0;board_frozen<=0;
   for(k=0;k<11;k=k+1)calls[k]<=0;
   for(k=0;k<10;k=k+1)begin content[k]<=k*19;countdown[k]<=0;end
   for(k=0;k<5;k=k+1)service_steps[k]<=0;
  end else begin
   if(long_step)chain<={long_in,chain[2003:1]};
   else if(!hold)chain<={chain[2002:0],~chain[2003]};
   if(!hold)normal_steps<=normal_steps+1;
   for(k=0;k<5;k=k+1)if(!hold)service_steps[k]<=service_steps[k]+1;
   if(board_request[10])calls[10]<=calls[10]+1;
   for(k=0;k<10;k=k+1)begin
    if(!hold)delayed[k]<=0;
    if(board_request[k])begin
     calls[k]<=calls[k]+1;
     if(wr&&!inject_error[k])content[k]<=data;
     delayed_value[k]<=content[k]^index[17:0]^{12'b0,index[23:18]}^(k*18'h1111);
     if(!suppress_reply[k])begin
      if(delay_cycles[k]==0)begin stub_done[k]<=1;stub_error[k]<=inject_error[k];stub_result[k*18+:18]<=content[k]^index[17:0]^{12'b0,index[23:18]}^(k*18'h1111);end
      else begin delayed[k]<=1;countdown[k]<=delay_cycles[k];end
     end
    end else if(delayed[k])begin
     if(countdown[k]==0)begin delayed[k]<=0;stub_done[k]<=1;stub_error[k]<=inject_error[k];stub_result[k*18+:18]<=delayed_value[k];end
     else countdown[k]<=countdown[k]-1;
    end
   end
  end
 end
 integer ticks=0,packets=0,checks=0;
 task tick;begin clk=0;#2;clk=1;#2;ticks=ticks+1;if(eth_bus[32]&&state_bus[32])$fatal(1,"EXT_BUS ownership collision");end endtask
 task idle(input integer n);integer t;begin for(t=0;t<n;t=t+1)tick();end endtask
 task word(input [15:0] value);begin enable=1;din=value;strobe=1;tick();strobe=0;tick();end endtask
 task finish_packet;begin enable=0;strobe=0;tick();packets=packets+1;end endtask
 task prefix(input [15:0] cmd,input [15:0] abi,input [63:0] schema);
  begin word(cmd);word(16'hb04d);word(abi);word(schema[15:0]);word(schema[31:16]);word(schema[47:32]);word(schema[63:48]);end
 endtask
 task control(input integer value);begin prefix(16'h77,2,SCHEMA);word(value);finish_packet();idle(4);end endtask
 task send_request(input integer operation,input integer board,input [23:0] row,input [17:0] value);
  begin prefix(16'h78,2,SCHEMA);word(operation);word(board);word(row[15:0]);word({8'b0,row[23:16]});word(value[15:0]);word({14'b0,value[17:16]});finish_packet();end
 endtask
 reg [15:0] status_words[0:17];
 task status;integer p;
  begin word(16'h79);status_words[0]=hps[15:0];for(p=1;p<18;p=p+1)begin word(0);status_words[p]=hps[15:0];end finish_packet();end
 endtask
 function integer total_calls;integer t;begin total_calls=0;for(t=0;t<11;t=t+1)total_calls=total_calls+calls[t];end endfunction
 task expect_result(input integer err,input [17:0] value);integer waited;
  begin waited=0;while(busy)begin tick();waited=waited+1;if(waited>2055)$fatal(1,"unbounded transaction");end
   idle(2);status();checks=checks+1;
   if(status_words[0]!==16'hd0b0||status_words[1]!==2||status_words[3]!==11||!status_words[2][8]||
      {status_words[15],status_words[14],status_words[13],status_words[12]}!==SCHEMA||status_words[17]!==2048)$fatal(1,"serial capability/schema differs");
   if(!status_words[2][4]||status_words[9]!==err||{status_words[8][1:0],status_words[7]}!==value)$fatal(1,"result differs err=%d expected=%d value=%x expected=%x",status_words[9],err,{status_words[8][1:0],status_words[7]},value);
  end
 endtask
 task cold_reset;
  begin enable=0;strobe=0;direct=0;inject_hold=0;inject_fault=0;suppress_reply=0;suppress_frozen=0;missing_services=0;local_missing=0;
   system_reset=1;idle(2);system_reset=0;idle(4);
   if(hold||port_fault||busy||complete||txn_sequence||control_errors)$fatal(1,"SYSTEM reset did not clear lifecycle");
  end
 endtask
 reg [2003:0] saved,expected_chain,quarantined;
 reg [31:0] saved_services[0:4];integer i,j,before_calls,before_errors,before_steps,before_ticks;
 integer widths[0:10];reg [23:0] row;reg [17:0] expected,value;
 initial begin
  widths[0]=12;widths[1]=15;widths[2]=16;widths[3]=16;widths[4]=18;widths[5]=16;widths[6]=20;widths[7]=24;widths[8]=13;widths[9]=19;widths[10]=17;
  for(i=0;i<10;i=i+1)delay_cycles[i]=0;
  cold_reset();
  word(16'h70);if(hps[15:0]!==16'hd074)$fatal(1,"Ethernet identity changed");word(0);word(0);word(0);if(hps[15:0]!==16'h5678)$fatal(1,"Ethernet response changed");finish_packet();
  send_request(1,0,0,18'h3ffff);idle(3);if(total_calls()!=0||txn_sequence!=0)$fatal(1,"running request accepted");
  // Every short ABI2 packet, old ABI1 packet and mismatched schema is inert.
  for(j=1;j<8;j=j+1)begin before_errors=control_errors;
   word(16'h77);for(i=1;i<j;i=i+1)word(i==1?16'hb04d:i==2?2:SCHEMA[(i-3)*16+:16]);finish_packet();idle(2);
   if(hold||control_errors!=before_errors+1)$fatal(1,"short control accepted");
  end
  word(16'h77);word(16'hb04d);word(1);word(1);finish_packet();idle(2);if(hold)$fatal(1,"legacy control accepted");
  word(16'h77);word(0);word(2);word(SCHEMA[15:0]);word(SCHEMA[31:16]);word(SCHEMA[47:32]);word(SCHEMA[63:48]);word(1);finish_packet();idle(2);if(hold)$fatal(1,"bad key accepted");
  prefix(16'h77,1,SCHEMA);word(1);finish_packet();idle(2);if(hold)$fatal(1,"bad ABI accepted");
  for(j=0;j<4;j=j+1)begin prefix(16'h77,2,SCHEMA^(64'b1<<(16*j)));word(1);finish_packet();idle(2);if(hold)$fatal(1,"bad schema accepted");end
  prefix(16'h77,2,SCHEMA);word(1);word(0);finish_packet();idle(2);if(hold)$fatal(1,"long control accepted");
  missing_services=1;control(1);if(!hold||port_ready)$fatal(1,"missing service acknowledged");
  before_calls=total_calls();send_request(1,0,0,18'h3ffff);idle(3);if(total_calls()!=before_calls)$fatal(1,"service barrier bypassed");
  missing_services=0;idle(3);if(!port_ready)$fatal(1,"complete boundary unavailable");
  before_steps=normal_steps;for(i=0;i<5;i=i+1)saved_services[i]=service_steps[i];idle(10);
  if(normal_steps!=before_steps)$fatal(1,"held CPU advanced");for(i=0;i<5;i=i+1)if(service_steps[i]!=saved_services[i])$fatal(1,"held service advanced");
  for(j=1;j<13;j=j+1)begin
   before_calls=total_calls();word(16'h78);
   for(i=1;i<j;i=i+1)word(i==1?16'hb04d:i==2?2:i<7?SCHEMA[(i-3)*16+:16]:0);
   finish_packet();idle(3);if(total_calls()!=before_calls)$fatal(1,"short request accepted");
  end
  prefix(16'h78,2,SCHEMA^64'h10000);word(0);word(0);word(0);word(0);word(0);word(0);finish_packet();idle(3);
  if(total_calls()!=before_calls)$fatal(1,"request schema ignored");
  // All board selectors, complete address widths, invalid selectors/padding.
  for(i=0;i<10;i=i+1)begin
   row=(24'hffffff>>(24-widths[i]));value=18'h12340+i;
   send_request(1,i,row,value);expect_result(0,0);
   send_request(0,i,row,0);expect_result(0,value^row[17:0]^{12'b0,row[23:18]}^(i*18'h1111));
  end
  for(i=11;i<16;i=i+1)begin send_request(0,i,0,0);expect_result(1,0);end
  for(i=0;i<11;i=i+1)if(i!=7)begin send_request(1,i,24'b1<<widths[i],18'h3ffff);expect_result(2,0);end
  for(i=0;i<3;i=i+1)begin
   before_calls=total_calls();prefix(16'h78,2,SCHEMA);word(0);word(i==0?16'h10:0);word(0);word(i==1?16'h100:0);word(0);word(i==2?4:0);finish_packet();idle(3);
   if(total_calls()!=before_calls)$fatal(1,"request padding ignored");
  end
  inject_error[3]=1;value=content[3];send_request(1,3,0,18'h3ffff);expect_result(3,0);if(content[3]!=value)$fatal(1,"child error wrote state");inject_error=0;
  // Actual longest chain: ownership is stable through malicious direct input,
  // SPI status traffic and an attempted release for more than 2004 cycles.
  saved=chain;expected=saved[17:0];send_request(0,10,0,0);idle(3);if(!busy||!long_busy)$fatal(1,"long scan not active");
  status();if(!status_words[2][3]||status_words[2][4]||{status_words[8][1:0],status_words[7]}!==0)$fatal(1,"busy status incoherent");
  before_errors=control_errors;control(0);if(!hold||control_errors!=before_errors+1)$fatal(1,"busy release accepted");
  direct=1;d_hold_update=1;d_hold_value=0;tick();
  if(!command_hold||!control_error)$fatal(1,"owner released busy session");
  d_hold_update=0;tick();direct=0;
  direct=1;d_request=1;d_board=0;d_write=1;d_index=24'h000bcd;d_data=18'h3ffff;idle(30);direct=0;
  expect_result(0,expected);if(chain!==saved)$fatal(1,"scan read changed physical chain");
  value=18'h3ffed;expected_chain=saved;expected_chain[2003:1998]=value[5:0];
  send_request(1,10,111,value);expect_result(0,0);if(chain!==expected_chain)$fatal(1,"last scan word/padding differs");
  send_request(0,10,111,0);expect_result(0,{12'b0,value[5:0]});
  delay_cycles[2]=4;send_request(0,2,9,0);status();
  if(!status_words[2][3]||status_words[2][4]||{status_words[8][1:0],status_words[7]}!==0)$fatal(1,"torn status result");
  expect_result(0,content[2]^18'd9^(2*18'h1111));delay_cycles[2]=0;
  // Incomplete/unknown frozen and foreign-domain acknowledgments fail closed.
  suppress_frozen[5]=1;idle(3);before_calls=total_calls();send_request(0,0,0,0);idle(3);if(total_calls()!=before_calls)$fatal(1,"incomplete board boundary accepted");suppress_frozen=0;idle(3);
  missing_services=5'b0000x;idle(3);before_calls=total_calls();send_request(0,0,0,0);idle(3);if(total_calls()!=before_calls||port_ready!==0)$fatal(1,"unknown service boundary accepted");missing_services=0;idle(3);
  // A lost foreign boundary aborts the real scan and keeps every consumer held.
  send_request(1,10,1,18'h24a15);idle(12);missing_services[2]=1;idle(4);expect_result(5,0);
  if(!port_fault||!hold||!scan_abort||!long_fault)$fatal(1,"lost boundary not quarantined");
  quarantined=chain;before_steps=normal_steps;for(i=0;i<5;i=i+1)saved_services[i]=service_steps[i];
  transport_reset=1;idle(2);transport_reset=0;idle(3);control(0);idle(10);
  if(!hold||!port_fault||chain!==quarantined||normal_steps!=before_steps)$fatal(1,"transport reset released quarantine");
  for(i=0;i<5;i=i+1)if(service_steps[i]!=saved_services[i])$fatal(1,"foreign consumer escaped quarantine");
  direct=1;d_hold_update=1;d_hold_value=0;d_request=0;tick();d_hold_update=0;tick();direct=0;
  if(!hold||!port_fault||!command_hold)$fatal(1,"owner allowed fault release");
  cold_reset();control(1);send_request(1,10,50,18'h1248f);idle(12);quarantined=chain;
  transport_reset=1;tick();transport_reset=0;idle(4);
  if(!hold||!command_hold||!port_fault||!long_fault||chain!==quarantined)$fatal(1,"active transport reset lost ownership");
  control(0);idle(4);if(!hold||chain!==quarantined)$fatal(1,"active reset thawed partial state");
  cold_reset();control(1);send_request(0,10,3,0);idle(12);local_missing=1;idle(4);expect_result(5,0);
  if(!hold||!port_fault||!long_fault)$fatal(1,"local boundary loss not quarantined");
  cold_reset();control(1);
  // Timeout is bounded and sticky, even if the missing child later completes.
  suppress_reply[4]=1;before_ticks=ticks;send_request(0,4,0,0);expect_result(4,0);
  if(ticks-before_ticks<2048||!port_fault||!hold||port_ready)$fatal(1,"timeout bound/quarantine differs");
  control(0);if(!hold||!port_fault)$fatal(1,"thaw cleared timeout fault");
  cold_reset();
  // Orphan child ownership immediately holds all consumers independently of
  // a UI command; its own fault escalates to sticky machine ownership.
  inject_hold[2]=1;#1;if(!hold)$fatal(1,"child hold not propagated");idle(3);
  inject_fault[2]=1;idle(2);inject_hold=0;inject_fault=0;idle(2);if(!hold||!port_fault)$fatal(1,"child fault not latched");
  cold_reset();control(1);transport_reset=1;idle(1);transport_reset=0;idle(3);
  if(!hold||!port_fault)$fatal(1,"paused transport reset lost ownership");
  cold_reset();transport_reset=1;idle(2);transport_reset=0;idle(3);if(hold||port_fault)$fatal(1,"idle transport reset trapped running machine");
  // Interrupted packets cannot commit their buffered payload after a parser reset.
  prefix(16'h77,2,SCHEMA);transport_reset=1;tick();transport_reset=0;word(1);finish_packet();idle(3);
  if(hold||port_fault)$fatal(1,"reset partial packet committed");
  uio.txn_sequence=32'hffff_ffff;control(1);send_request(0,0,0,0);expect_result(0,content[0]);if(txn_sequence!==0)$fatal(1,"transaction rollover differs");
  uio.control_errors=32'hffff_ffff;prefix(16'h77,1,SCHEMA);word(1);finish_packet();if(control_errors!==0)$fatal(1,"control counter rollover differs");
  control(1);control(0);before_steps=normal_steps;idle(5);if(hold||normal_steps<=before_steps)$fatal(1,"clean session did not resume");
  $display("SERIAL BOARD STATE PASS ticks=%0d packets=%0d checks=%0d real2004scan/ABI2/schema/all11boards/5services/timeout/sticky-global-hold/transport-vs-SYSTEM-reset/Ethernet",ticks,packets,checks);$finish;
 end
endmodule
