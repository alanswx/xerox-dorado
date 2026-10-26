`timescale 1ns/1ps
module tb_riot_blockram;
 reg clk=0,guest_advance=0,ce=0,res_n=0,RW_n=1,RS_n=0,CS1=1,CS2_n=0;
 reg [6:0] addr=0;reg [7:0] d_in=0,PA_in=8'hff,PB_in=8'hff;
 wire [7:0] old_d_out,new_d_out,old_PA_out,new_PA_out,old_PB_out,new_PB_out;
 wire old_IRQ_n,new_IRQ_n,old_oe,new_oe;
 M6532_ce reference_chip(.d_out(old_d_out),.PA_out(old_PA_out),.PB_out(old_PB_out),.IRQ_n(old_IRQ_n),.oe(old_oe),.*);
 dorado_riot_blockram_ce candidate_chip(.d_out(new_d_out),.PA_out(new_PA_out),.PB_out(new_PB_out),.IRQ_n(new_IRQ_n),.oe(new_oe),.*);
 integer checks=0,reads=0,writes=0,resets=0,paused=0;
 task automatic compare_outputs;
  if({new_d_out,new_PA_out,new_PB_out,new_IRQ_n,new_oe}!==
     {old_d_out,old_PA_out,old_PB_out,old_IRQ_n,old_oe})
   $fatal(1,"RIOT_COMPONENT output mismatch check=%0d old=%h new=%h",checks,old_d_out,new_d_out);
 endtask
 task automatic cycle;
  begin
   #1;compare_outputs;clk=1;#1;compare_outputs;
   if({candidate_chip.out_a,candidate_chip.out_b,candidate_chip.dir_a,candidate_chip.dir_b,
       candidate_chip.interrupt,candidate_chip.timer,candidate_chip.prescaler,
       candidate_chip.incr,candidate_chip.rollover,candidate_chip.irq_en,candidate_chip.edge_detect,candidate_chip.riot_stuff.old_pa7}!==
      {reference_chip.out_a,reference_chip.out_b,reference_chip.dir_a,reference_chip.dir_b,
       reference_chip.interrupt,reference_chip.timer,reference_chip.prescaler,
       reference_chip.incr,reference_chip.rollover,reference_chip.irq_en,reference_chip.edge_detect,reference_chip.riot_stuff.old_pa7})
    $fatal(1,"RIOT_COMPONENT non-RAM state mismatch check=%0d",checks);
   for(integer i=0;i<128;i++)
    if(reference_chip.riot_ram[i] !==
       (candidate_chip.native_ram.written[i] ? candidate_chip.native_ram.backing[i] : 8'b0))
     $fatal(1,"RIOT_COMPONENT RAM mismatch check=%0d address=%0d",checks,i);
   checks++;
   if(guest_advance===1'b1)begin
    if(res_n===1'b0)resets++;
    if(ce===1'b1 && CS1===1'b1 && CS2_n===1'b0)begin
     if(RW_n===1'b1)reads++;
     else if(RW_n===1'b0)writes++;
    end
   end else paused++;
   clk=0;#1;
  end
 endtask
 function automatic logic four(input integer n);
  case(n)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 reg [31:0] rng=32'h78d1b603;
 initial begin
  cycle;guest_advance=1;cycle;res_n=1;ce=1;
  for(integer i=0;i<128;i++)begin RW_n=0;addr=7'(i);d_in=8'(i+23);cycle;end
  RW_n=1;
  for(integer i=0;i<128;i++)begin addr=7'(i);cycle;end
  res_n=0;cycle;res_n=1;
  for(integer i=0;i<128;i++)begin addr=7'(i);cycle;end
  for(integer i=0;i<60000;i++)begin
   rng={rng[30:0],rng[31]^rng[21]^rng[1]^rng[0]};
   guest_advance=rng[0]|rng[2];ce=rng[1];res_n=(i%983)!=0;
   RW_n=rng[3];RS_n=rng[4];CS1=rng[5]|rng[7];CS2_n=rng[6]&rng[8];
   addr=rng[15:9];d_in=rng[23:16];PA_in=rng[31:24];PB_in=rng[12:5];cycle;
  end
`ifndef VERILATOR
  for(integer profile=0;profile<2;profile++)for(integer n=0;n<16384;n++)begin
   guest_advance=four(n%4);ce=four((n/4)%4);res_n=four((n/16)%4);
   RW_n=four((n/64)%4);RS_n=four((n/256)%4);CS1=four((n/1024)%4);CS2_n=four((n/4096)%4);
   addr=profile==1 ? 7'b1x0z01x : 7'd45;d_in=n[1] ? 8'b10xz10zx : 8'h6b;cycle;
  end
`endif
  if(reads<1000 || writes<1000 || resets<20 || paused<1000)$fatal(1,"RIOT_COMPONENT insufficient coverage");
  $display("RIOT_COMPONENT_PASS checks=%0d reads=%0d writes=%0d resets=%0d paused=%0d",checks,reads,writes,resets,paused);
  $finish;
 end
endmodule
