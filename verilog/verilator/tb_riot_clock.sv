// Compare the oversampled package with the original PHI2-clocked RIOT.
// Exercise the bus, RAM, ports, all timer prescalers and IRQ read/clear.
`timescale 1ns/1ps
module tb_riot_clock;
  reg clk=0; always #1 clk=~clk;
  reg phi=0,reset_n=0,rw=1,ram_n=1,selected=1;
  reg [6:0] address=0;
  reg [7:0] din=0,pain=255,pbin=255;
  wire [7:0] dout,pa,pb,ref_data,ref_pa,ref_pb;
  wire irq,ref_irq,ref_oe;
  cell_MCS6532 d(.sys_clk(clk),.p39(phi),.p34(reset_n),.p35(rw),
    .p36(ram_n),.p38(selected),.p37(1'b0),
    .p40(address[6]),.p2(address[5]),.p3(address[4]),.p4(address[3]),
    .p5(address[2]),.p6(address[1]),.p7(address[0]),
    .d_in(din),.pa_in(pain),.pb_in(pbin),.p25(irq),
    .p26(dout[7]),.p27(dout[6]),.p28(dout[5]),.p29(dout[4]),
    .p30(dout[3]),.p31(dout[2]),.p32(dout[1]),.p33(dout[0]),
    .p15(pa[7]),.p14(pa[6]),.p13(pa[5]),.p12(pa[4]),
    .p11(pa[3]),.p10(pa[2]),.p9(pa[1]),.p8(pa[0]),
    .p16(pb[7]),.p17(pb[6]),.p18(pb[5]),.p19(pb[4]),
    .p21(pb[3]),.p22(pb[2]),.p23(pb[1]),.p24(pb[0]));
  M6532 reference(.clk(phi),.ce(1'b1),.res_n(reset_n),.addr(address),
    .RW_n(rw),.d_in(din),.d_out(ref_data),.RS_n(ram_n),
    .IRQ_n(ref_irq),.CS1(selected),.CS2_n(1'b0),
    .PA_in(pain),.PB_in(pbin),.PA_out(ref_pa),.PB_out(ref_pb),.oe(ref_oe));
  integer cycles=0,irq_cycles=0;
  task phase;
    begin
      @(negedge clk);phi=0;
      repeat(5)@(negedge clk);
      phi=1;
      repeat(3)@(negedge clk);
      cycles++;
      if({dout,pa,pb,irq} !== {ref_oe ? ref_data : 8'h00,ref_pa,ref_pb,ref_irq})
        $fatal(1,"RIOT clock transform differs at cycle %0d addr=%h data=%h/%h pa=%h/%h pb=%h/%h irq=%b/%b",
          cycles,address,dout,ref_data,pa,ref_pa,pb,ref_pb,irq,ref_irq);
      if({d.u_riot.timer,d.u_riot.prescaler,d.u_riot.interrupt,d.u_riot.incr,d.u_riot.rollover} !==
         {reference.timer,reference.prescaler,reference.interrupt,reference.incr,reference.rollover})
        $fatal(1,"RIOT timer/interrupt state differs at cycle %0d",cycles);
      if(!irq)irq_cycles++;
    end
  endtask
  task write_register(input [6:0] a,input [7:0] value);
    begin address=a;din=value;ram_n=1;rw=0;phase();end
  endtask
  integer i;
  reg [31:0] random_state=32'h63f20357;
  initial begin
    phase();phase();reset_n=1;
    write_register(1,8'hff);write_register(0,8'h55);
    write_register(3,8'hf0);write_register(2,8'ha5);
    for(i=0;i<128;i++)begin
      ram_n=0;rw=0;address=i;din=(i*73)^8'hc5;phase();
    end
    rw=1;
    for(i=0;i<128;i++)begin address=i;phase();end
    ram_n=1;
    for(i=0;i<4;i++)begin
      write_register(7'h1c+i,8'd2); // IRQ enabled, divide 1/8/64/1024
      selected=0;rw=1;
      repeat(4200)phase();
      selected=1;address=7'h0c;phase(); // timer read clears/rearms IRQ
      address=7'h0d;phase();
    end
    for(i=0;i<10000;i++)begin
      random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
      address=random_state[6:0];din=random_state[15:8];
      ram_n=random_state[16];rw=random_state[17];selected=random_state[18];
      pain=random_state[31:24];pbin=~pain;phase();
    end
    for(i=0;i<128;i++)if(d.u_riot.riot_ram[i]!==reference.riot_ram[i])
      $fatal(1,"RIOT RAM differs at %0d",i);
    if(irq_cycles<100)$fatal(1,"IRQ behavior not exercised");
    $display("RIOT CLOCK PASS: %0d PHI2 cycles, all 128 RAM bytes, ports/readback, four timer prescalers, %0d IRQ-active cycles",cycles,irq_cycles);
    $finish;
  end
endmodule
