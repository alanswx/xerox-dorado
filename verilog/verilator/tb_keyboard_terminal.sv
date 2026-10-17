// Physical key events through the same decoder/serializer as MiSTer.
`timescale 1ns/1ps
`default_nettype none
module tb_keyboard_terminal;
  reg sys_clk=0; always #1 sys_clk=~sys_clk;
  reg reset=1, line_tick=0;
  reg [10:0] ps2_key=0;
  wire [15:0] w0,w1,w2,w3;
  wire data;
  dorado_keyboard k(.sys_clk(sys_clk),.reset(reset),.ps2_key(ps2_key),
    .key_word0(w0),.key_word1(w1),.key_word2(w2),.key_word3(w3));
  dorado_terminal t(.sys_clk(sys_clk),.reset(reset),.line_tick(line_tick),
    .key_word0(w0),.key_word1(w1),.key_word2(w2),.key_word3(w3),.key_word4(16'hffff),
    .mouse_dx(9'sd0),.mouse_dy(9'sd0),.mouse_pending(1'b0),.OISData(data));
  reg [31:0] packet;
  task grab;
    begin
      packet=0;
      for(integer i=0;i<32;i=i+1)begin
        @(negedge sys_clk);line_tick=1;
        @(posedge sys_clk);packet={packet[30:0],data};
        @(negedge sys_clk);line_tick=0;
      end
    end
  endtask
  task event_key(input [7:0] code,input bit down);
    begin
      @(negedge sys_clk);ps2_key={~ps2_key[10],down,1'b0,code};
      repeat(3)@(negedge sys_clk);
    end
  endtask
  task expect_word(input [6:0] kind,input [15:0] word_);
    begin
      // Finish the message already in flight, then receive the transition.
      grab;grab;
      if(packet!=={1'b1,kind,word_,1'b1,7'd0})
        $fatal(1,"keyboard packet %h expected type %0d body %h",packet,kind,word_);
    end
  endtask
  initial begin
    repeat(4)@(negedge sys_clk);reset=0;
    repeat(4)@(negedge sys_clk);
    // HM Table 24: initial refresh is keyboard word 0, all keys released.
    grab;
    if(packet!==32'h81ffff80)$fatal(1,"initial released-key packet %h",packet);
    // Alto matrix: E=word0 bit12, Return=word2 bit3, LeftShift=word2 bit6.
    event_key(8'h24,1);expect_word(1,16'hefff);
    event_key(8'h24,0);expect_word(1,16'hffff);
    event_key(8'h5a,1);event_key(8'h12,1);expect_word(3,16'hffb7);
    event_key(8'h5a,0);event_key(8'h12,0);expect_word(3,16'hffff);
    $display("tb_keyboard_terminal: PASS -- key down/up and chord delivered on serial wire");
    $finish;
  end
endmodule
`default_nettype wire
