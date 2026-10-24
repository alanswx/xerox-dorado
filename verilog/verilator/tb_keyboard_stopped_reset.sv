// Native reset is asserted while its machine clock is gated off. Exercise
// the real queue, active-low decoder and HM Table 24 serial return together.
`timescale 1ns/1ps
`default_nettype none
module tb_keyboard_stopped_reset;
  reg host=0; always #5 host=~host;
  reg enabled=0, vsync_n=1, line_tick=0;
`ifdef DORADO_RESET_START_HIGH
  reg reset=1;
`else
  reg reset=0;
`endif
  wire machine=host & enabled;
  reg [10:0] raw=0;
  wire [10:0] buffered;
  wire overflow;
  wire [3:0] queued;
  wire [15:0] w0,w1,w2,w3;
  wire data;
  dorado_key_queue #(.ADDRESS_BITS(3)) q(
    .clk_host(host),.reset(reset),.ps2_key(raw),.vsync_n(vsync_n),
    .machine_key(buffered),.overflow(overflow),.queued(queued));
  dorado_keyboard k(.sys_clk(machine),.reset(reset),.ps2_key(buffered),
    .key_word0(w0),.key_word1(w1),.key_word2(w2),.key_word3(w3));
  dorado_terminal t(.sys_clk(machine),.reset(reset),.line_tick(line_tick),
    .key_word0(w0),.key_word1(w1),.key_word2(w2),.key_word3(w3),
    .key_word4(16'hffff),.mouse_dx(9'sd0),.mouse_dy(9'sd0),
    .mouse_pending(1'b0),.OISData(data),.OISData_n(),
    .mouse_taken(),.mouse_sent_dx(),.mouse_sent_dy());
  reg [31:0] packet;
  integer source_edges=0;
  always @(posedge machine) source_edges=source_edges+1;
  task expect_idle;
    if ({w0,w1,w2,w3} !== 64'hffffffffffffffff)
      $fatal(1,"stopped reset matrix %h %h %h %h",w0,w1,w2,w3);
    if(t.msg !== 32'h81ffff80 || t.msg_bit !== 0 || t.msg_word !== 1)
      $fatal(1,"stopped reset terminal %h bit=%0d word=%0d",t.msg,t.msg_bit,t.msg_word);
  endtask
  task field;
    @(negedge host);vsync_n=0;
    repeat(4)@(negedge host);vsync_n=1;
    repeat(4)@(negedge host);
  endtask
  task grab;
    packet=0;
    for(integer i=0;i<32;i=i+1)begin
      @(negedge host);line_tick=1;
      @(posedge host);packet={packet[30:0],data};
      @(negedge host);line_tick=0;
    end
  endtask
  initial begin
    // No source edge is available at configuration. Test both initial
    // reset levels; native cold configuration starts with reset high.
    repeat(4)@(negedge host);expect_idle;
    reset=0;
    repeat(2)@(negedge host);reset=1;
    repeat(4)@(negedge host);
    if(source_edges!=0)$fatal(1,"fixture advanced stopped machine");
    expect_idle;
    reset=0;
    @(negedge host);raw=11'h621; // C make: only word2 bit10 goes low.
    repeat(4)@(negedge host);
    if(queued!=1 || overflow)$fatal(1,"C event not queued");
    enabled=1;
    field;
    if({w0,w1,w2,w3} !== 64'hfffffffffbffffff)
      $fatal(1,"C boot matrix %h %h %h %h",w0,w1,w2,w3);
    grab;
    if(packet!==32'h81ffff80)$fatal(1,"first idle serial packet %h",packet);
    grab;
    if(packet!==32'h83fbff80)$fatal(1,"C serial packet %h",packet);
    // Reset again with a held key and a previously running serializer.
    @(negedge host);enabled=0;
    repeat(3)@(negedge host);reset=1;
    repeat(4)@(negedge host);expect_idle;
    reset=0;enabled=1;
    grab;
    if(packet!==32'h81ffff80)$fatal(1,"warm reset serial packet %h",packet);
    $display("tb_keyboard_stopped_reset: PASS -- no reset clock edges, idle matrix, queued C, exact serial packets and stopped warm reset");
    $finish;
  end
endmodule
`default_nettype wire
