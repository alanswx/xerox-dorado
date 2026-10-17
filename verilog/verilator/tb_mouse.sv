`timescale 1ns/1ps
module tb_mouse;
 reg clk=0; always #1 clk=~clk;
 reg reset=1, taken=0;
 reg [24:0] ps2=0;
 reg signed [8:0] sx=0,sy=0;
 wire signed [8:0] dx,dy;
 wire [15:0] keys;
 wire pending,overflow;
 dorado_mouse #(.ACCUM_BITS(12)) d(.clk(clk),.reset(reset),.ps2_mouse(ps2),
  .mouse_taken(taken),.mouse_sent_dx(sx),.mouse_sent_dy(sy),
  .key_word4(keys),.mouse_dx(dx),.mouse_dy(dy),.mouse_pending(pending),.motion_overflow(overflow));
 task clear;
  begin reset=1;taken=0;repeat(3)@(negedge clk);reset=0;repeat(2)@(negedge clk);end
 endtask
 task packet(input integer x,y,input [2:0] b);
  reg [7:0] flags;
  begin
   flags={2'b00,(y<0),(x<0),1'b1,b};
   ps2={~ps2[24],8'(y),8'(x),flags};repeat(3)@(negedge clk);
  end
 endtask
 task consume(input integer x,y);
  begin
   sx=9'(x);sy=9'(y);taken=1;repeat(3)@(negedge clk);
   taken=0;repeat(3)@(negedge clk);
  end
 endtask
 task drain(input integer want_x,want_y);
  integer tx,ty,n,x,y;
  begin
   tx=0;ty=0;n=0;
   while(pending && n<40) begin
    x=$signed(dx);y=$signed(dy);tx=tx+x;ty=ty+y;n=n+1;
    if(x < -128 || x > 127 || y < -128 || y > 127)$fatal(1,"terminal range");
    consume(x,y);
   end
   if(pending || tx!=want_x || ty!=want_y)
    $fatal(1,"motion not conserved: got %0d,%0d expected %0d,%0d",tx,ty,want_x,want_y);
  end
 endtask
 initial begin
  clear();if(keys!==16'hffff || pending)$fatal(1,"idle mouse");
  // Every nine-bit PS/2 value, including sign independent of byte bit7.
  for(integer x=-256;x<=255;x=x+1) begin
   packet(x, -1-x, 0);drain(x,1+x);
  end
  if(overflow)$fatal(1,"ordinary motion overflowed");
  for(integer b=0;b<8;b=b+1) begin
   packet(0,0,3'(b));
   if(keys!==16'(16'hffff ^ ((b&1)*4+(b&2)+(b&4)/4)))$fatal(1,"button order %0d",b);
   if(pending)$fatal(1,"button-only event invented motion");
  end
  // A stopped terminal can leave its acknowledgement high for many clocks.
  packet(255,255,0);sx=127;sy=-128;taken=1;
  repeat(20)@(negedge clk);
  if(dx!==127 || dy!==-127)$fatal(1,"held acknowledgement drained motion repeatedly");
  taken=0;repeat(3)@(negedge clk);drain(128,-127);
  // Packet and acknowledgement on the same host edge must both survive.
  packet(-256,0,0);sx=-128;sy=0;taken=1;
  packet(10,0,0);taken=0;repeat(3)@(negedge clk);drain(-118,0);
  // Accumulate while the machine is paused, including direction reversal.
  for(integer n=0;n<6;n=n+1)packet(200,-100,0);
  packet(-50,10,0);drain(1150,590);
  // Saturate, rather than wrap, if a very long pause exceeds capacity.
  for(integer n=0;n<16;n=n+1)packet(255,255,0);
  if(!overflow)$fatal(1,"missing saturation indication");drain(2047,-2048);
  clear();ps2={~ps2[24],8'h00,8'h00,8'h48};repeat(3)@(negedge clk);
  if(!overflow)$fatal(1,"missing PS/2 overflow indication");drain(255,0);
  $display("tb_mouse: PASS -- signed packets, Y direction, buttons, paused/overlapping reports and saturation");
  $finish;
 end
endmodule
