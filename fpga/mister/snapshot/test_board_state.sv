module board_state_fixture;
 reg clk=0,reset=1,enable=0,strobe=0;reg [15:0] din=0;
 wire [35:0] hps,eth_bus,state_bus;
 assign hps[35:33]={1'b0,enable,strobe};assign hps[31:16]=din;
 assign eth_bus[35:33]=hps[35:33];assign eth_bus[31:16]=hps[31:16];
 assign state_bus[35:33]=hps[35:33];assign state_bus[31:16]=hps[31:16];
 assign hps[32]=eth_bus[32]|state_bus[32];
 assign hps[15:0]=eth_bus[32]?eth_bus[15:0]:state_bus[32]?state_bus[15:0]:16'b0;
 wire freeze,port_request,port_write,port_ready,port_done,port_fault,busy,complete;
 wire [3:0] selected;wire [23:0] index;wire [17:0] data,port_result;wire [2:0] port_error;
 wire [31:0] txn_sequence,control_errors;
 reg guest_advance=0;always @(negedge clk)guest_advance<=!freeze;
 wire guest_held=freeze && !guest_advance;
 reg [10:0] board_frozen=0,board_done=0,board_error=0;
 reg [10:0] suppress_frozen=0,suppress_reply=0,inject_error=0;
 reg [197:0] board_result=0;wire [10:0] board_request;
 reg [17:0] content[0:10];integer calls[0:10],delay_cycles[0:10],countdown[0:10];reg [10:0] delayed=0;
 reg [17:0] delayed_value[0:10];reg [31:0] normal_steps=0;
 dorado_board_state_uio uio(clk,reset,state_bus,freeze,guest_held,board_frozen,
  port_request,port_write,selected,index,data,port_ready,port_done,port_error,port_result,port_fault,busy,complete,txn_sequence,control_errors);
 dorado_board_state_fabric fabric(clk,reset,freeze,guest_held,port_request,port_write,selected,index,data,port_ready,port_done,port_error,port_result,port_fault,
  board_frozen,board_done,board_error,board_result,board_request);
 dorado_ethernet_uio #(.MAX_WORDS(1024),.DDR_CAPABLE(1)) ethernet(
 .clk(clk),.reset(reset),.bus(eth_bus),.rx_ready(1'b1),.tx_valid(1'b1),.tx_words(11'd2),.tx_data(16'h1234),
 .tx_good(32'h12345678),.tx_bad(32'b0),.tx_dropped(32'b0),.rx_bad(32'b0),
 .machine_debug(32'hdeadbeef),.processor_debug(32'h01234567),.machine_status(16'h4321),
 .tx_addr(),.rx_addr(),.tx_ack(),.rx_wr(),.rx_commit(),.rx_data(),.rx_words(),.aborted(),
 .ddr_stopped(1'b1),.ddr_initialized(1'b0),.ddr_fault(32'b0),.ddr_enable(),.ddr_epoch(),.ddr_control_errors(),.ddr_owned());
 integer k;
 always @(posedge clk)begin
  board_done<=0;board_error<=0;board_frozen<=({11{freeze&&guest_held}}&~suppress_frozen);
  if(guest_advance)normal_steps<=normal_steps+1;
  if((board_request & (board_request-1))!=0)$fatal(1,"nononehot child request");
  if(reset)begin
   board_frozen<=0;delayed<=0;normal_steps<=0;
   for(k=0;k<11;k=k+1)begin content[k]<=k*19;calls[k]<=0;countdown[k]<=0;end
  end else for(k=0;k<11;k=k+1)begin
   if(!freeze)delayed[k]<=0;
   if(board_request[k])begin
    calls[k]<=calls[k]+1;
    if(!guest_held || !freeze)$fatal(1,"unheld child request");
    if(port_write&&!inject_error[k])content[k]<=data;
    delayed_value[k]<=content[k]^index[17:0]^{12'b0,index[23:18]}^(k*18'h1111);
    if(!suppress_reply[k])begin
     if(delay_cycles[k]==0)begin board_done[k]<=1;board_error[k]<=inject_error[k];board_result[k*18+:18]<=content[k]^index[17:0]^{12'b0,index[23:18]}^(k*18'h1111);end
     else begin delayed[k]<=1;countdown[k]<=delay_cycles[k];end
    end
   end else if(delayed[k])begin
    if(countdown[k]==0)begin delayed[k]<=0;board_done[k]<=1;board_error[k]<=inject_error[k];board_result[k*18+:18]<=delayed_value[k];end
    else countdown[k]<=countdown[k]-1;
   end
  end
 end
 reg [31:0] seed=32'hb04d0011;
 function [31:0] random_word;
  begin seed=seed^(seed<<13);seed=seed^(seed>>17);seed=seed^(seed<<5);random_word=seed;end
 endfunction
 integer ticks=0,packets=0,checks=0;
 task tick;
  begin clk=0;#2;clk=1;#2;ticks=ticks+1;if(eth_bus[32]&&state_bus[32])$fatal(1,"EXT_BUS ownership collision");end
 endtask
 task idle(input integer n);integer t;begin for(t=0;t<n;t=t+1)tick();end endtask
 task word(input [15:0] value);begin enable=1;din=value;strobe=1;tick();strobe=0;tick();end endtask
 task finish_packet;begin enable=0;strobe=0;tick();packets=packets+1;end endtask
 task control(input integer mode);begin word(16'h77);word(16'hb04d);word(mode);word(1);finish_packet();idle(4);end endtask
 task send_request(input integer wr,input integer board,input [23:0] row,input [17:0] value);
  begin word(16'h78);word(16'hb04d);word(wr);word(board);word(row[15:0]);word({8'b0,row[23:16]});word(value[15:0]);word({14'b0,value[17:16]});finish_packet();end
 endtask
 reg [15:0] status_words[0:11];
 task status;
  integer p;begin word(16'h79);status_words[0]=hps[15:0];for(p=1;p<12;p=p+1)begin word(0);status_words[p]=hps[15:0];end finish_packet();end
 endtask
 task malformed_request(input integer which);
  begin word(16'h78);word(which==0?16'h0000:16'hb04d);word(which==1?2:0);word(which==2?16'h10:0);word(0);word(which==3?16'h100:0);word(0);word(which==4?4:0);if(which==5)word(0);finish_packet();idle(4);end
 endtask
 task expect_result(input integer err,input [17:0] value);
  begin idle(5);status();checks=checks+1;
   if(status_words[0]!==16'hd0b0||status_words[1]!==1||status_words[3]!==11||!status_words[2][8])$fatal(1,"board capability differs");
   if(!status_words[2][4]||status_words[9]!==err||{status_words[8][1:0],status_words[7]}!==value)$fatal(1,"result differs err=%0d expected=%0d got=%x expected=%x flags=%x seq=%x calls=%0d errors=%0d complete=%b busy=%b ready=%b",status_words[9],err,{status_words[8][1:0],status_words[7]},value,status_words[2],txn_sequence,total_calls(),control_errors,complete,busy,port_ready);
   if({status_words[6],status_words[5]}!==txn_sequence)$fatal(1,"transaction sequence differs");
  end
 endtask
 function integer total_calls;integer i;begin total_calls=0;for(i=0;i<11;i=i+1)total_calls=total_calls+calls[i];end endfunction
 integer i,j,before_calls,before_errors,before_steps;reg [17:0] expected,value;reg [23:0] row;integer width[0:10];
 initial begin
  width[0]=12;width[1]=15;width[2]=16;width[3]=16;width[4]=18;width[5]=16;width[6]=20;width[7]=24;width[8]=13;width[9]=19;width[10]=17;
  for(i=0;i<11;i=i+1)delay_cycles[i]=0;
  idle(2);reset=0;idle(3);
  // Unrelated traffic retains Ethernet behavior and cannot mutate board state.
  word(16'h70);if(hps[15:0]!==16'hd074)$fatal(1,"ethernet identity changed");word(0);word(0);word(0);if(hps[15:0]!==16'h5678)$fatal(1,"ethernet reply changed");finish_packet();
  send_request(1,0,0,18'h3ffff);idle(4);if(txn_sequence!=0||total_calls()!=0||control_errors!=1)$fatal(1,"running command accepted");
  // Every short control/request packet and an overlong packet must be inert.
  for(j=1;j<4;j=j+1)begin before_errors=control_errors;word(16'h77);for(i=1;i<j;i=i+1)word(i==1?16'hb04d:1);finish_packet();if(freeze||control_errors!=before_errors+1)$fatal(1,"short control accepted");end
  word(16'h77);word(16'hb04d);word(1);word(1);word(0);finish_packet();if(freeze)$fatal(1,"long control accepted");
  word(16'h77);word(16'h0000);word(1);word(1);finish_packet();if(freeze)$fatal(1,"bad control key accepted");
  control(1);if(!guest_held||!port_ready||board_frozen!==11'h7ff)$fatal(1,"board boundary missing");
  before_steps=normal_steps;idle(20);if(normal_steps!=before_steps)$fatal(1,"guest advanced while held");
  for(j=1;j<8;j=j+1)begin before_calls=total_calls();word(16'h78);for(i=1;i<j;i=i+1)word(i==1?16'hb04d:0);finish_packet();idle(4);if(total_calls()!=before_calls||txn_sequence!=0)$fatal(1,"short request accepted");end
  for(j=0;j<6;j=j+1)begin before_calls=total_calls();before_errors=control_errors;malformed_request(j);if(total_calls()!=before_calls||control_errors!=before_errors+1||txn_sequence!=0)$fatal(1,"malformed request accepted");end
  // Read/write every board at zero and its complete address-width boundary.
  for(i=0;i<11;i=i+1)for(j=0;j<8;j=j+1)begin
   row=j==0?0:j==1?((24'hffffff)>>(24-width[i])):random_word()&((24'hffffff)>>(24-width[i]));value=random_word()&18'h3ffff;
   send_request(1,i,row,value);expect_result(0,0);expected=value^row[17:0]^{12'b0,row[23:18]}^(i*18'h1111);
   send_request(0,i,row,0);expect_result(0,expected);
  end
  before_calls=total_calls();
  for(i=11;i<16;i=i+1)begin send_request(0,i,0,0);expect_result(1,0);end
  for(i=0;i<11;i=i+1)if(i!=7)begin send_request(1,i,24'b1<<width[i],18'h3ffff);expect_result(2,0);end
  if(total_calls()!=before_calls)$fatal(1,"out-of-range request aliased a child");
  inject_error[3]=1;value=content[3];send_request(1,3,0,18'h3ffff);expect_result(3,0);if(content[3]!=value)$fatal(1,"child error ignored");inject_error=0;
  suppress_frozen[5]=1;idle(3);before_calls=total_calls();send_request(1,0,0,18'h3ffff);idle(3);if(total_calls()!=before_calls)$fatal(1,"incomplete boundary accepted");suppress_frozen=0;idle(3);
  // Delayed replies retain ownership, release while busy is rejected.
  delay_cycles[2]=10;send_request(0,2,1,0);before_errors=control_errors;control(0);if(!freeze||control_errors!=before_errors+1)$fatal(1,"busy boundary release accepted");expect_result(0,content[2]^18'd1^(2*18'h1111));delay_cycles[2]=0;
  // A timeout blocks further requests until explicit thaw/re-entry.
  suppress_reply[4]=1;send_request(0,4,0,0);idle(25);expect_result(4,0);if(!port_fault||port_ready)$fatal(1,"timeout did not quarantine port");
  before_calls=total_calls();send_request(1,0,0,0);idle(4);if(total_calls()!=before_calls)$fatal(1,"faulted port accepted request");
  control(0);idle(3);if(freeze||port_fault)$fatal(1,"explicit thaw did not recover port");suppress_reply=0;control(1);send_request(0,4,0,0);expect_result(0,content[4]^(4*18'h1111));
  // Completion during status must not mix new data with the captured busy flag.
  send_request(1,1,0,18'h12345);expect_result(0,0);
  send_request(0,1,9,0);status();
  if(!status_words[2][3]||status_words[2][4]||{status_words[8][1:0],status_words[7]}!==18'b0)$fatal(1,"incoherent status snapshot");
  expect_result(0,18'h12345^18'd9^18'h1111);
  // Private protocol-counter preload tests rollover without guest-state edits.
  uio.txn_sequence=32'hffff_fffe;send_request(0,0,0,0);expect_result(0,content[0]);send_request(0,0,0,0);expect_result(0,content[0]);if(txn_sequence!==0)$fatal(1,"sequence rollover differs");
  // Losing one consumer's boundary during an outstanding request fails closed.
  delay_cycles[6]=10;send_request(0,6,0,0);idle(1);suppress_frozen[0]=1;idle(3);expect_result(5,0);if(!port_fault||port_ready)$fatal(1,"lost boundary did not quarantine port");
  suppress_frozen=0;delay_cycles[6]=0;control(0);control(1);send_request(0,6,0,0);expect_result(0,content[6]^(6*18'h1111));
  control(0);before_steps=normal_steps;idle(5);if(normal_steps<=before_steps)$fatal(1,"guest did not resume");
  // Reset always cancels control state without invoking a child.
  control(1);reset=1;idle(2);reset=0;idle(3);if(freeze||busy||complete||txn_sequence||control_errors)$fatal(1,"reset failed to clear protocol");
  $display("BOARD STATE PASS ticks=%0d packets=%0d checks=%0d all11boards/widths/short-long/key/running/held/delay/timeout/reset/Ethernet coexistence",ticks,packets,checks);$finish;
 end
endmodule
