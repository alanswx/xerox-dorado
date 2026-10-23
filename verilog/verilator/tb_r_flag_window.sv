`timescale 1ps/1ps
`default_nettype none
module tb_r_flag_window;
 reg [3:0] held,cb,ca,db,sb,sa;
 wire [3:0] next_held,physical,closing,original;
 reg load;
 reg [3:0] fixture;
 integer checks=0;
 integer coverage[0:3][0:127];
 dorado_r_flag_window_step dut(held,cb,ca,db,sb,sa,next_held,physical,closing);
 original_MC1668 #(.DELAY_PS(1)) h(1'b0,sb[0],db[0],cb[0],cb[1],db[1],sb[1],1'b0,
   original[0],,,original[1],load,fixture[1:0],);
 original_MC1668 #(.DELAY_PS(1)) l(1'b0,sb[2],db[2],cb[2],cb[3],db[3],sb[3],1'b0,
   original[2],,,original[3],load,fixture[3:2],);
 function automatic [3:0] cv(input integer x);cv=4'(x);endfunction
 function automatic [3:0] dv(input integer x);dv=4'(x>>2);endfunction
 function automatic [3:0] sv(input integer x);sv={2'(x),2'(x>>4)};endfunction
 task automatic check;
  begin
   checks++;
   if(physical!==original)$fatal(1,"R flag physical mismatch check=%0d actual=%b original=%b held=%b C=%b D=%b S=%b",checks,physical,original,held,cb,db,sb);
  end
 endtask
 task automatic move(input [3:0] c,d,s,input bit record_coverage);
  reg [3:0] saved;
  integer key;
  begin
   if(record_coverage)for(integer b=0;b<4;b++)begin
    key={held[b],cb[b],db[b],sb[b],c[b],d[b],s[b]};coverage[b][key]++;
   end
   ca=c;sa=s;#2;saved=next_held;
   if(closing!==(cb & ~c))$fatal(1,"R flag closing event mismatch");
   held=saved;cb=c;db=d;sb=s;#4;check();
   if(next_held!==held)$fatal(1,"R flag owner commit is not idempotent check=%0d",checks);
  end
 endtask
 initial begin
  for(integer b=0;b<4;b++)for(integer k=0;k<128;k++)coverage[b][k]=0;
  held=0;cb=0;ca=0;db=0;sb=0;sa=0;load=1;fixture=0;#4;load=0;#4;
  for(integer seed=0;seed<16;seed++)begin
   for(integer before_no=0;before_no<64;before_no++)begin
    for(integer after_no=0;after_no<64;after_no++)begin
     load=1;held=4'(seed);fixture=4'(seed);cb=cv(before_no);ca=cb;db=dv(before_no);sb=sv(before_no);sa=sb;#4;
     load=0;#4;check();
     move(cv(after_no),dv(after_no),sv(after_no),1);
     // A late D while open, a closing edge with changed D, then removal
     // of closed Set: both transparent tracking and persistence are checked.
     move(cb,~db,sb,0);
     move(0,~db,4'b0101,0);
     move(0,db,0,0);
    end
   end
  end
  for(integer b=0;b<4;b++)for(integer k=0;k<128;k++)
   if(coverage[b][k]==0)$fatal(1,"R flag transition coverage missing bit=%0d key=%0d",b,k);
  $display("R_FLAG_WINDOW PASS checks=%0d transitions=65536 per_bit_states=128",checks);
  $finish;
 end
endmodule
`default_nettype wire
