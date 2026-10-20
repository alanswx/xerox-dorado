`timescale 1ns/1ps
module tb;
 localparam [63:0] BUILD=64'hb49c378fd137862a,SCHEMA=64'haf0b647cb43a3a62;
 reg clk=0,machine_reseed=0,transport_reset=0;
 reg enable=0,strobe=0;reg [15:0] din=0;
 wire [35:0] bus;assign bus[35:33]={1'b0,enable,strobe};assign bus[31:16]=din;
 reg command_hold=0,guest_held=0,global_hold=0,board_busy=0,board_fault=0;
 reg [4:0] local_available=7,local_held=0;
 wire host_hold,host_fault,hold_update,hold_value;wire [4:0] services;
 wire [63:0] epoch;wire [1:0] phase;wire [31:0] errors;
 dorado_snapshot_host_control #(.BUILD_ID(BUILD),.SCHEMA_ID(SCHEMA)) dut(
  .clk(clk),.machine_reseed(machine_reseed),.transport_reset(transport_reset),.bus(bus),
  .command_hold(command_hold),.guest_held(guest_held),.global_hold(global_hold),
  .board_busy(board_busy),.board_fault(board_fault),.local_available(local_available),.local_held(local_held),
  .host_hold(host_hold),.host_fault(host_fault),.hold_update(hold_update),.hold_value(hold_value),
  .services_permitted(services),.epoch(epoch),.phase(phase),.errors(errors));
 always @(posedge clk)if(hold_update)command_hold<=hold_value;
 integer checks=0,packets=0;reg [15:0] words[0:63],status[0:21];
 task tick;#2;clk=1;#2;clk=0;#1;endtask
 task check(input bit ok,input string message);checks++;if(!ok)$fatal(1,"%s checks=%0d",message,checks);endtask
 task word(input [15:0] value);
  strobe=1;din=value;tick();strobe=0;tick();
 endtask
 task packet(input integer size);
  enable=1;for(integer i=0;i<size;i++)word(words[i]);enable=0;repeat(3)tick();packets++;
 endtask
 task prepare(input [15:0] command,input [63:0] nonce,input [4:0] permits);
  for(integer i=0;i<64;i++)words[i]=0;
  words[0]=command;words[1]=16'hc04d;words[2]=1;
  for(integer i=0;i<4;i++)begin words[3+i]=16'(BUILD>>(i*16));words[7+i]=16'(SCHEMA>>(i*16));words[11+i]=16'(nonce>>(i*16));end
  words[15]={11'b0,permits};
 endtask
 task action(input [15:0] command,input [63:0] nonce,input [4:0] permits);
  prepare(command,nonce,permits);packet(16);
 endtask
 task reseed;
  enable=0;machine_reseed=1;transport_reset=0;command_hold=0;guest_held=0;global_hold=0;
  board_busy=0;board_fault=0;local_available=7;local_held=0;repeat(3)tick();machine_reseed=0;tick();
  check(phase==0&&!host_hold&&!host_fault&&epoch==0&&services==0,"RBF initialization differs");
 endtask
 task enter(input [63:0] nonce);
  command_hold=1;guest_held=1;global_hold=1;tick();action(16'h7b,nonce,0);
  check(phase==1&&host_hold&&!host_fault&&epoch==nonce&&services==0,"host lease did not enter held phase");
 endtask
 task read_status(input bit mutate);
  enable=1;word(16'h7a);status[0]=bus[15:0];
  if(mutate)begin local_available=0;local_held=0;end
  for(integer i=1;i<22;i++)begin word(0);status[i]=bus[15:0];end
  enable=0;repeat(3)tick();
 endtask
 function automatic four(input integer n);
  case(n&3)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 initial begin
  reseed();read_status(0);
  check(status[0]==16'hd0c0&&status[1]==1&&status[2]==0&&status[3]==5,"host status capability differs");
  check({status[7],status[6],status[5],status[4]}==BUILD&&{status[11],status[10],status[9],status[8]}==SCHEMA,
        "host status build/schema differs");
  action(16'h7b,1,0);check(epoch==0&&phase==0&&errors==1,"unheld Begin accepted");
  enter(1);action(16'h7c,1,7);check(services==0&&errors==2,"unacknowledged local service granted");
  local_held=7;tick();action(16'h7c,1,3);check(services==3,"qualified services not granted");
  action(16'h7c,1,31);check(services==3,"unavailable input/video service granted");
  action(16'h7c,2,0);check(services==3&&epoch==1,"stale lease changed grants");
  board_busy=1;action(16'h7c,1,0);check(services==3,"busy scan lost a service permission");board_busy=0;
  for(integer damaged=1;damaged<16;damaged++)begin
   prepare(16'h7c,1,0);words[damaged]^=16'h8000;packet(16);
   check(services==3&&phase==1&&epoch==1,"damaged identity/nonce/padding changed lease");
  end
  for(integer length=1;length<64;length++)if(length!=16)begin
   prepare(16'h7c,1,0);packet(length);check(services==3&&phase==1,"partial/overlong grant committed");
  end
  prepare(16'h7c,1,0);words[15][0]=1'bx;packet(16);check(services===5'b00011,"unknown grant bit committed");
  action(16'h7c,1,0);check(services==0,"zero permission revocation failed");
  for(integer available=0;available<4;available++)for(integer ack=0;ack<4;ack++)begin
   local_available={4'b0,four(available)};local_held={4'b0,four(ack)};tick();
   action(16'h7c,1,1);
   check(services==((available==1&&ack==1)?5'd1:5'd0),"four-state ACK granted authority");
   action(16'h7c,1,0);
  end
  local_available=7;local_held=7;tick();read_status(1);
  check(status[17]==7&&status[18]==7,"status ACK words mixed observations");
  local_available=7;local_held=7;tick();action(16'h7c,1,3);
  action(16'h7d,1,0);check(phase==2&&!host_hold&&services==0&&!command_hold,"host release did not clear owner command");
  action(16'h7e,1,0);check(phase==2,"unfinished native release finalized");
  global_hold=0;guest_held=0;tick();action(16'h7e,1,0);check(phase==0&&!host_hold,"completed host release did not finish");
  command_hold=1;guest_held=1;global_hold=1;tick();action(16'h7b,1,0);check(phase==0,"completed nonce reused");
  enter(2);local_available=7;local_held=7;action(16'h7c,2,1);
  local_held[0]=1'bx;tick();check(phase==3&&host_hold&&host_fault&&services==0,"ACK revocation did not quarantine");
  local_held=7;read_status(0);check(status[2]==3,"quarantine status unreadable");
  transport_reset=1;repeat(3)tick();transport_reset=0;repeat(4)tick();
  check(phase==3&&host_hold&&epoch==2,"parser reset released quarantine");
  action(16'h7e,2,0);check(phase==3&&host_hold,"Finish released quarantine");
  reseed();enter(3);transport_reset=1'bx;tick();transport_reset=0;tick();
  check(phase==3&&host_fault&&host_hold,"unknown parser reset thawed host lease");
  reseed();enter(4);command_hold=0;tick();check(phase==3&&host_hold,"foreign hold release escaped");
  reseed();enter(5);action(16'h7d,5,0);global_hold=0;guest_held=0;tick();
  board_busy=1;action(16'h7f,5,0);check(phase==3&&host_hold&&command_hold,"Main fault could not stop released/busy machine");
  board_fault=1;read_status(0);check(status[2]==3,"device fault blocked status parser");
  $display("HOST SERVICE CONTROL PASS checks=%0d packets=%0d local_ack_fourstate=16 partial_lengths=62 quarantine=6",checks,packets);$finish;
 end
 initial begin #1000000;$fatal(1,"host-service test deadline");end
endmodule
