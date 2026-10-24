`timescale 1ns/1ps
`default_nettype none
module tb_memory_hold_cofactor;
 reg [155:0] q;
 reg before_root,after_root,refresh;
 reg [397:0] before_pins,after_pins;
 wire [362:0] expected5,actual5,expected24,actual24;
 original_atomic #(.WAVES(5)) original5(q,before_root,after_root,before_pins,after_pins,
  expected5[362:207],expected5[206:151],expected5[150:95],expected5[94],refresh,expected5[93:0]);
 candidate_atomic #(.WAVES(5)) candidate5(q,before_root,after_root,before_pins,after_pins,
  actual5[362:207],actual5[206:151],actual5[150:95],actual5[94],refresh,actual5[93:0]);
 original_atomic #(.WAVES(24)) original24(q,before_root,after_root,before_pins,after_pins,
  expected24[362:207],expected24[206:151],expected24[150:95],expected24[94],refresh,expected24[93:0]);
 candidate_atomic #(.WAVES(24)) candidate24(q,before_root,after_root,before_pins,after_pins,
  actual24[362:207],actual24[206:151],actual24[150:95],actual24[94],refresh,actual24[93:0]);
 reg [31:0] seed=32'h9e3779b9;
 integer checks=0,unknown_checks=0;
 function automatic [31:0] draw();
  begin seed=seed^(seed<<13);seed=seed^(seed>>17);seed=seed^(seed<<5);draw=seed;end
 endfunction
 initial begin
  for(integer epoch=0;epoch<32;epoch++)begin
   q={draw(),draw(),draw(),draw(),draw()};
   before_pins={draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw()};
   after_pins={draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw(),draw()};
   before_root=1'(epoch);after_root=1'(epoch>>1);refresh=1'(epoch>>2);
   if(epoch%4==0)begin q[17]=1'bx;before_pins[20]=1'bz;after_pins[24]=1'bx;end
   for(integer level=0;level<4;level++)begin
    case(level)0:after_pins[386]=0;1:after_pins[386]=1;2:after_pins[386]=1'bx;3:after_pins[386]=1'bz;endcase
    #1;
    if(expected5!==actual5 || expected24!==actual24)
     $fatal(1,"MEMORY_HOLD four-state result mismatch epoch=%0d selector=%b",epoch,after_pins[386]);
    if({original5.mir_mask_diff,original5.mir_mask_high_changed,original5.mir_mask_low_clear}!==
       {candidate5.mir_mask_diff,candidate5.mir_mask_high_changed,candidate5.mir_mask_low_clear} ||
       {original24.mir_mask_diff,original24.mir_mask_high_changed,original24.mir_mask_low_clear}!==
       {candidate24.mir_mask_diff,candidate24.mir_mask_high_changed,candidate24.mir_mask_low_clear})
     $fatal(1,"MEMORY_HOLD four-state diagnostic mismatch epoch=%0d selector=%b",epoch,after_pins[386]);
    checks++;if(level>=2)unknown_checks++;
   end
  end
  $display("MEMORY_HOLD_FOUR_STATE_PASS checks=%0d unknown_checks=%0d waves=5/24 outputs=363 diagnostics=15/72",checks,unknown_checks);
  $finish;
 end
endmodule
`default_nettype wire
