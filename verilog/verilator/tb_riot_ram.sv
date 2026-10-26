`timescale 1ns/1ps
module tb_riot_ram;
 reg clk=0,advance=0,reset=0,read_enable=0,write_enable=0;
 reg [6:0] address=0;reg [7:0] write_data=0;
 wire [7:0] read_data;
 dorado_riot_ram dut(.*);
 reg [7:0] reference_ram[0:127];
 reg [7:0] reference_read;
 integer checks=0,reads=0,writes=0,resets=0,paused=0,collisions=0;
 // Same reset-all-at-once/read-before-write behavior as the current RIOT.
 always @(posedge clk)if(advance)begin
  if(read_enable)reference_read<=reference_ram[address];
  if(reset)begin
   for(integer i=0;i<128;i++)reference_ram[i]<=8'b0;
  end else if(write_enable)reference_ram[address]<=write_data;
 end
 task automatic cycle;
  begin
   #1;clk=1;#1;
   if(read_data!==reference_read)
    $fatal(1,"RIOT_RAM read mismatch check=%0d expected=%h actual=%h",checks,reference_read,read_data);
   for(integer i=0;i<128;i++)
    if((dut.written[i] ? dut.backing[i] : 8'b0)!==reference_ram[i])
     $fatal(1,"RIOT_RAM logical memory mismatch check=%0d address=%0d",checks,i);
   checks++;
   if(advance===1'b1)begin
    if(read_enable===1'b1)reads++;
    if(reset===1'b1)resets++;
    else if(write_enable===1'b1)writes++;
    if(read_enable===1'b1 && write_enable===1'b1 && reset===1'b0)collisions++;
   end else paused++;
   clk=0;#1;
  end
 endtask
 function automatic logic four(input integer n);
  case(n)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 reg [31:0] rng=32'h3ac92b17;
 initial begin
  // Read unknown power-on state; then reset all bytes at once.
  advance=1;read_enable=1;cycle;
  reset=1;cycle;reset=0;
  // Fill every backing cell nonzero, then reset while a read is enabled.
  for(integer i=0;i<128;i++)begin address=7'(i);write_data=8'(i+1);write_enable=1;cycle;end
  write_enable=0;reset=1;address=7'd53;cycle;reset=0;
  if(dut.backing[53]!==8'd54)$fatal(1,"RIOT_RAM reset altered backing storage");
  for(integer i=0;i<128;i++)begin address=7'(i);cycle;end
  // Paused reset/write/read must not change any published or logical state.
  advance=0;reset=1;write_enable=1;cycle;reset=0;cycle;advance=1;
  for(integer i=0;i<40000;i++)begin
   rng={rng[30:0],rng[31]^rng[21]^rng[1]^rng[0]};
   advance=rng[0]|rng[3];reset=(i%997)==0;
   read_enable=rng[1];write_enable=rng[2];address=rng[14:8];write_data=rng[23:16];
   cycle;
  end
`ifndef VERILATOR
  // Four-state procedural enables/reset, including unknown addresses and data.
  // Verilator's binary tests above deliberately do not claim X/Z coverage.
  for(integer profile=0;profile<4;profile++)
   for(integer n=0;n<256;n++)begin
    advance=four(n%4);reset=four((n/4)%4);
    read_enable=four((n/16)%4);write_enable=four((n/64)%4);
    address=profile==0 ? 7'd45 : profile==1 ? 7'bx01xz10 : profile==2 ? 7'bzzzzzzz : 7'd17;
    write_data=profile==3 ? 8'b10xz01zx : 8'ha7;cycle;
   end
`endif
  if(reads<1000 || writes<1000 || paused<1000 || collisions<1000 || resets<20)
   $fatal(1,"RIOT_RAM insufficient coverage");
  $display("RIOT_RAM_PASS checks=%0d reads=%0d writes=%0d resets=%0d paused=%0d collisions=%0d",checks,reads,writes,resets,paused,collisions);
  $finish;
 end
endmodule
