`timescale 1ns/1ps
`default_nettype none
module tb_control_terminal_wave;
 reg [954:0] stimulus,known;
 wire [155:0] q=stimulus[954:799];
 wire before_root=stimulus[798],after_root=stimulus[797],refresh=stimulus[0];
 wire [397:0] before_pins=stimulus[796:399],after_pins=stimulus[398:1];
 wire [362:0] expected[0:3],actual[0:3];
 wire [3:0] diagnostics_equal;
 for(genvar i=0;i<4;i++)begin: depths
  localparam integer W=i==0 ? 4 : i==1 ? 5 : i==2 ? 6 : 24;
  original_atomic #(.WAVES(W)) original_step(q,before_root,after_root,before_pins,after_pins,
   expected[i][362:207],expected[i][206:151],expected[i][150:95],expected[i][94],refresh,expected[i][93:0]);
  candidate_atomic #(.WAVES(W)) candidate_step(q,before_root,after_root,before_pins,after_pins,
   actual[i][362:207],actual[i][206:151],actual[i][150:95],actual[i][94],refresh,actual[i][93:0]);
  assign diagnostics_equal[i]=
   {original_step.mir_mask_diff,original_step.mir_mask_high_changed,original_step.mir_mask_low_clear}===
   {candidate_step.mir_mask_diff,candidate_step.mir_mask_high_changed,candidate_step.mir_mask_low_clear};
 end
 reg [31:0] seed=32'h9e3779b9;
 integer checks=0,unknown_checks=0;
 function automatic [31:0] draw();
  begin seed=seed^(seed<<13);seed=seed^(seed>>17);seed=seed^(seed<<5);draw=seed;end
 endfunction
 task automatic draw_known;
  begin
   known=0;
   for(integer chunk=0;chunk<30;chunk++)known=(known<<32)|955'(draw());
  end
 endtask
 task automatic compare(input integer injected_bit,input integer level);
  begin
   for(integer depth=0;depth<4;depth++)begin
    if(expected[depth]!==actual[depth])
     $fatal(1,"CONTROL_TERMINAL four-state result mismatch bit=%0d level=%0d depth=%0d",injected_bit,level,depth);
   end
   if(diagnostics_equal!==4'b1111)
    $fatal(1,"CONTROL_TERMINAL four-state diagnostic mismatch bit=%0d level=%0d",injected_bit,level);
   checks++;
  end
 endtask
 initial begin
  for(integer epoch=0;epoch<64;epoch++)begin
   draw_known();stimulus=known;#1;compare(-1,epoch);
  end
  // Each of the955 independent inputs receives BOTH X and Z in isolation.
  for(integer bit_index=0;bit_index<955;bit_index++)begin
   draw_known();
   for(integer level=0;level<2;level++)begin
    stimulus=known;stimulus[bit_index]=level ? 1'bz : 1'bx;
    #1;compare(bit_index,level);unknown_checks++;
   end
  end
  $display("CONTROL_TERMINAL_FOUR_STATE_PASS checks=%0d unknown_checks=%0d input_bits=955 waves=4/5/6/24 outputs=363 diagnostics=12/15/18/72",checks,unknown_checks);
  $finish;
 end
endmodule
`default_nettype wire
