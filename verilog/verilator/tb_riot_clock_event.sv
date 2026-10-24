// Compare explicit event capture with the original PHI2-clocked vendor part.
// All73 live scalar bits and all128 RAM bytes are compared after EVERY host
// edge, including no-event/reset/transport-held edges. Inputs are pre-edge
// bus values; output-enable remains combinational, like the original part.
`timescale 1ns/1ps
module tb_riot_clock_event;
 reg clk=0,reference_phi=0,consume=1,phi_rise=0;
 reg reset_n=0,rw=1,ram_n=1,cs1=1,cs2_n=0;
 reg [6:0] address=0;
 reg [7:0] data=0,pain=255,pbin=255;
 wire [7:0] dout,pa,pb,ref_data,ref_pa,ref_pb;
 wire irq,oe,ref_irq,ref_oe;
 dorado_riot_clock_event dut(clk,consume,phi_rise,reset_n,address,
  rw,ram_n,cs1,cs2_n,data,pain,pbin,dout,pa,pb,irq,oe);
 M6532 reference(.clk(reference_phi),.ce(1'b1),.res_n(reset_n),
  .addr(address),.RW_n(rw),.RS_n(ram_n),.CS1(cs1),.CS2_n(cs2_n),
  .d_in(data),.PA_in(pain),.PB_in(pbin),.d_out(ref_data),
  .PA_out(ref_pa),.PB_out(ref_pb),.IRQ_n(ref_irq),.oe(ref_oe));
 integer host_edges=0,events=0,holds=0,idles=0,irq_events=0,reset_events=0;
 integer ram_checks=0,state_checks=0;
 reg [31:0] random_state=32'h536532ad;
 task fabric(input bit permitted,event_present);
  begin
   clk=0;reference_phi=0;consume=permitted;phi_rise=event_present;#2;
   if(permitted && event_present)begin
    reference_phi=1;events=events+1;if(!reset_n)reset_events=reset_events+1;
   end else if(!permitted)holds=holds+1;else idles=idles+1;
   clk=1;#2;
   if({dout,pa,pb,irq,oe} !== {ref_data,ref_pa,ref_pb,ref_irq,ref_oe})
    $fatal(1,"RIOT_EVENT output host=%0d event=%0d consume=%b rise=%b reset_n=%b",host_edges,events,consume,phi_rise,reset_n);
   if({dut.u_riot.out_a,dut.u_riot.out_b,dut.u_riot.dir_a,dut.u_riot.dir_b,
       dut.u_riot.d_out,dut.u_riot.interrupt,dut.u_riot.timer,dut.u_riot.prescaler,
       dut.u_riot.incr,dut.u_riot.rollover,dut.u_riot.irq_en,dut.u_riot.edge_detect,
       dut.u_riot.riot_stuff.old_pa7} !==
      {reference.out_a,reference.out_b,reference.dir_a,reference.dir_b,
       reference.d_out,reference.interrupt,reference.timer,reference.prescaler,
       reference.incr,reference.rollover,reference.irq_en,reference.edge_detect,
       reference.riot_stuff.old_pa7})
    $fatal(1,"RIOT_EVENT scalar state host=%0d event=%0d",host_edges,events);
   state_checks=state_checks+1;
   for(integer a=0;a<128;a++)begin
    if(dut.u_riot.riot_ram[a] !== reference.riot_ram[a])
     $fatal(1,"RIOT_EVENT RAM host=%0d event=%0d byte=%0d",host_edges,events,a);
    ram_checks=ram_checks+1;
   end
   if(permitted && event_present && !irq)irq_events=irq_events+1;
   host_edges=host_edges+1;clk=0;reference_phi=0;#1;
  end
 endtask
 task phase;
  begin fabric(1,0);fabric(1,1);fabric(1,0);end
 endtask
 task write_register(input [6:0] a,input [7:0] value);
  begin address=a;data=value;ram_n=1;rw=0;phase;end
 endtask
 initial begin
  // Synchronous reset belongs to PHI2, including when the transport pauses.
  fabric(1,1);fabric(0,1);reset_n=1;
  write_register(1,8'hff);write_register(0,8'h55);
  write_register(3,8'hf0);write_register(2,8'ha5);
  for(integer i=0;i<128;i++)begin
   ram_n=0;rw=0;address=7'(i);data=8'(i*73)^8'hc5;phase;
  end
  rw=1;
  for(integer i=0;i<128;i++)begin address=7'(i);phase;end
  // A reset pulse with no original edge must leave all state/RAM unchanged.
  reset_n=0;fabric(1,0);fabric(0,1);fabric(0,0);reset_n=1;
  ram_n=1;
  for(integer i=0;i<4;i++)begin
   write_register(7'h1c+7'(i),8'd2);cs1=0;rw=1;
   repeat(4200)phase;
   cs1=1;address=7'h0c;phase;address=7'h0d;phase;
  end
  for(integer i=0;i<10000;i++)begin
   random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
   address=random_state[6:0];data=random_state[15:8];
   ram_n=random_state[16];rw=random_state[17];cs1=random_state[18];cs2_n=random_state[19];
   pain=random_state[31:24];pbin=~pain;
   // Change bus pins on held/no-event host ticks and before actual captures.
   if(i%11==0)begin reset_n=0;fabric(0,1);reset_n=1;end
   if(i%17==0)begin reset_n=0;fabric(1,0);reset_n=1;end
   phase;
  end
  reset_n=0;fabric(0,1);fabric(1,1);reset_n=1;phase;
  if(irq_events<100 || holds<900 || reset_events!=2 || events<27000)
   $fatal(1,"RIOT_EVENT incomplete coverage");
  $display("RIOT_EVENT_PASS host=%0d events=%0d holds=%0d idles=%0d states=%0d RAM=%0d irq_events=%0d reset_events=%0d scalar_bits=73 RAM_bytes=128",host_edges,events,holds,idles,state_checks,ram_checks,irq_events,reset_events);
  $finish;
 end
endmodule
