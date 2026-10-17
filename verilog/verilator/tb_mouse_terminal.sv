`timescale 1ns/1ps
module tb_mouse_terminal;
 reg clk=0; always #1 clk=~clk;
 reg reset=1,line_tick=0;
 reg [24:0] ps2=0;
 wire [15:0] keys;
 wire signed [8:0] dx,dy,sx,sy;
 wire pending,taken,data;
 dorado_mouse m(.clk(clk),.reset(reset),.ps2_mouse(ps2),.key_word4(keys),
  .mouse_dx(dx),.mouse_dy(dy),.mouse_pending(pending),
  .mouse_taken(taken),.mouse_sent_dx(sx),.mouse_sent_dy(sy));
 dorado_terminal t(.sys_clk(clk),.reset(reset),.line_tick(line_tick),
  .key_word0(16'hffff),.key_word1(16'hffff),.key_word2(16'hffff),.key_word3(16'hffff),
  .key_word4(keys),.mouse_dx(dx),.mouse_dy(dy),.mouse_pending(pending),
  .mouse_taken(taken),.mouse_sent_dx(sx),.mouse_sent_dy(sy),.OISData(data));
 reg [31:0] got;
 task grab;
  begin
   got=0;
   for(integer i=0;i<32;i=i+1)begin
    @(negedge clk);line_tick=1;@(posedge clk);got={got[30:0],data};
    @(negedge clk);line_tick=0;
   end
  end
 endtask
 initial begin
  repeat(4)@(negedge clk);reset=0;repeat(3)@(negedge clk);
  grab();if(got!==32'h81ffff80)$fatal(1,"initial terminal refresh");
  // Left+middle click and +200 X / +17 PS/2 Y (up).
  ps2={1'b1,8'd17,8'd200,8'h0d};repeat(3)@(negedge clk);
  grab();grab();
  if(got!==32'h85fffa80)$fatal(1,"button transition must precede motion, got %h",got);
  grab();if(got!==32'h86ff6f80)$fatal(1,"first mouse chunk got %h",got);
  grab();if(got!==32'h86c98080)$fatal(1,"remaining mouse chunk got %h",got);
  grab();if(got[30:24]==6)$fatal(1,"motion repeated after acknowledgement");
  ps2={1'b0,8'd0,8'd0,8'h08};repeat(3)@(negedge clk);
  grab();grab();if(got!==32'h85ffff80)$fatal(1,"mouse release packet got %h",got);
  $display("tb_mouse_terminal: PASS -- real serial framing, buttons before motion, exact excess-200B chunks and release");
  $finish;
 end
endmodule
