`timescale 1ns/1ps
module tb;
 reg clk=1,root_hold=1,desired=0,abort=0,reset=0,request=0,writing=0;
 reg [23:0] index=0;reg [17:0] data=0;
 reg [15:0] pins=0;reg default_advance=0,default_hold=0,default_write=0;
 reg [17:0] default_data=0;
 wire serial_in,scan_step,scan_ready,scan_hold,done,error,fault;
 integer checks=0,edges=0,requests=0,phase_transitions=0;
 wire global_hold=root_hold || scan_hold;
 wire permit=desired && !global_hold;
 reg global_step=0;
 always @(negedge clk)global_step<=permit;
 wire [18:0] local_step;
 dorado_board_phase_enable distribution(.clk(clk),.permit(permit),.advance(local_step));
 wire [37:0] bank,reference_bank,default_bank,default_reference_bank;
 wire [37:0] next_bank={serial_in,bank[37:1]};
 wire reference_serial_in=controller.writing && controller.in_window ?
       controller.payload[controller.offset] : reference_bank[0];
 wire [37:0] reference_next={reference_serial_in,reference_bank[37:1]};
 wire guest_held=global_hold && !global_step;
 wire [17:0] result;
 dorado_ff_scan_controller #(.STATE_BITS(38)) controller(
  .clk(clk),.reset(reset),.freeze(global_hold && !abort),.guest_held(guest_held),
  .request(request),.write(writing),.index(index),.data(data),
  .serial_out(bank[0]),.serial_in(serial_in),.scan_step(scan_step),
  .ready(scan_ready),.hold_required(scan_hold),.done(done),.error(error),.fault(fault),.result(result));
 // INSTANCES
 reg previous_step=0;reg [31:0] rng=32'h9c174ba3;
 function automatic [31:0] random_next(input [31:0] value);
  reg [31:0] v;v=value^(value<<13);v=v^(v>>17);return v^(v<<5);
 endfunction
 function automatic four(input integer value);
  case(value&3)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 task check(input bit ok,input string message);
  checks++;if(!ok)$fatal(1,"%s edge=%0d",message,edges);
 endtask
 task compare;
  check(bank===reference_bank,"phase payload physical state differs");
  check(default_bank===default_reference_bank,"default-off physical state differs");
 endtask
 task cycle(input bit move_demand=0);
  #4;clk=0;#1;
  check(local_step==={19{global_step}},"local board advance phase differs");
  if(global_step!==previous_step)phase_transitions++;
  previous_step=global_step;
  if(move_demand)desired=~desired;
`ifdef LATE_HOLD
  if(edges==4)root_hold=1;
`endif
  #4;clk=1;#1;edges++;
  compare();check(local_step==={19{global_step}},"local board advance phase differs");
 endtask
 task random_inputs(input integer n);
  rng=random_next(rng);
  for(integer b=0;b<16;b++)pins[b]=four((rng>>(b%15))&3);
  default_hold=four(n);default_advance=four(n>>2);default_write=four(n>>4);
  for(integer b=0;b<18;b++)default_data[b]=four((rng>>(b%15))&3);
 endtask
 task enter;
  request=0;root_hold=1;desired=0;abort=0;
  repeat(4)cycle();check(scan_ready,"phase scan not ready");
 endtask
 task access(input integer word,input bit write_word,input [17:0] value);
  check(scan_ready,"phase scan access before ready");
  index=word;writing=write_word;data=value;request=1;cycle();request=0;requests++;
  for(integer age=0;!done;age++)begin cycle();check(age<42,"phase scan timeout");end
  check(!error&&!fault&&!scan_hold,"phase complete scan fault");
 endtask
 task save_restore;
  reg [37:0] saved;
  enter();saved=bank;
  for(integer i=0;i<3;i++)begin
   access(i,0,0);check(result===(18'(saved>>(i*18))),"phase scan read differs");
   check(bank===saved,"phase read rotation changed state");
  end
  for(integer i=0;i<3;i++)access(i,1,~18'(saved>>(i*18)));
  check(bank===(~saved),"phase destructive import differs");
  for(integer i=0;i<3;i++)access(i,1,18'(saved>>(i*18)));
  check(bank===saved,"phase complete restore differs");
  root_hold=0;desired=1;
  for(integer n=0;n<500;n++)begin random_inputs(n);cycle(n[0]);end
 endtask
 initial begin
  reset=1;repeat(3)cycle();reset=0;
`ifdef LATE_HOLD
  root_hold=0;desired=1;repeat(3)cycle();$fatal(1,"late hold contract was not checked");
`endif
`ifdef UNHELD_WRITE
  root_hold=0;desired=0;repeat(3)cycle();$fatal(1,"unheld write contract was not checked");
`endif
  enter();access(0,1,18'h2b713);access(1,1,18'h1a9c5);access(2,1,18'h2);
  // Exercise every local copy through deterministic permission transitions
  // before random scan traffic holds the machine for long intervals.
  root_hold=0;
  for(integer n=0;n<2048;n++)begin
   random_inputs(n);desired=n[0];cycle();
  end
  for(integer n=0;n<16384;n++)begin
   random_inputs(n);root_hold=four(n>>6);desired=four(n>>8);
   request=n[9];writing=n[10];index=24'(n%3);data=default_data;
   cycle(n[0]);
  end
  enter();while(controller.busy)cycle();repeat(3)cycle();
  save_restore();save_restore();
  enter();index=0;data=18'h31bc5;writing=1;request=1;cycle();request=0;repeat(7)cycle();
  check(controller.busy,"phase partial import was not active");
  abort=1;cycle();check(fault&&scan_hold,"phase partial import did not quarantine");
  begin:quarantine
   reg [37:0] partial;partial=bank;root_hold=0;desired=1;abort=0;
   for(integer n=0;n<64;n++)begin
    random_inputs(n);cycle();
    check(bank===partial && local_step===19'b0 && global_step===1'b0,
          "phase partial import quarantine escaped");
   end
  end
  root_hold=1;reset=1;repeat(3)cycle();reset=0;repeat(4)cycle();save_restore();
  check(phase_transitions>1000 && requests>20,"phase payload coverage missing");
  $display("PHASE PAYLOAD PASS checks=%0d edges=%0d requests=%0d transitions=%0d physical=38 local_registers=19 default_fourstate=16384 partial_import_quarantine=1",
           checks,edges,requests,phase_transitions);$finish;
 end
 initial begin #2000000;$fatal(1,"phase payload test deadline");end
endmodule
