`timescale 1ns/1ps
`default_nettype none
module tb_r_front_cone;
 reg load=0;
 reg [735:0] fixture=0;
 reg [235:0] pins=0;
 wire [735:0] reference,sampled;
 wire [201:0] ref_source,source;
 wire [7:0] ref_fa,fa;
 wire [43:0] ref_data,data;
 wire [56:0] ref_clocks,clocks;
 r_front_reference original(.fixture_load(load),.fixture_state(fixture),.base(1'b1),
   .pins(pins),.state_q(reference),.source_pins(ref_source),.fa_n(ref_fa),
   .front_data(ref_data),.clocks(ref_clocks));
 dorado_r_front_cone dut(.state_q(reference),.base(1'b1),.pins(pins),.sampled_q(sampled),
   .source_pins(source),.fa_n(fa),.front_data(data),.clocks(clocks));
 integer checks=0;
 reg [7:0] expected;
 integer group_no;
 reg [31:0] rng=32'hd03ad0a1;
 function automatic [31:0] random_next(input [31:0] v);
   reg [31:0] x;
   begin x=v^(v<<13);x=x^(x>>17);random_next=x^(x<<5);end
 endfunction
 task automatic check;
   #100;
   if(sampled!==reference) $fatal(1,"R front sampled state adapter mismatch");
   if({source,fa,data,clocks}!=={ref_source,ref_fa,ref_data,ref_clocks})
     $fatal(1,"R front combinational mismatch check=%0d source=%h/%h fa=%h/%h data=%h/%h clock=%h/%h",checks,source,ref_source,fa,ref_fa,data,ref_data,clocks,ref_clocks);
   checks=checks+1;
 endtask
 initial begin
   pins[8:0]=9'h128;pins[205]=1;
   #100;load=1;#100;
   for(integer asel=0;asel<8;asel=asel+1)
     for(integer ff=0;ff<256;ff=ff+1)
       for(integer disabled=0;disabled<2;disabled=disabled+1) begin
         pins[204:202]=~{asel[0],asel[1],asel[2]};
         pins[19:12]=8'(ff);pins[209]=disabled[0];
         check();
         group_no=(asel<4) ? 0 : (ff>>6);
         expected=disabled ? 8'hff : (8'hff^(8'h1<<group_no)^(8'h1<<(group_no+4)));
         if(fa!==expected) $fatal(1,"R front physical FA decode mismatch ASEL=%0d FF=%o disabled=%0d expected=%h actual=%h",asel,ff,disabled,expected,fa);
       end
   // Sample every adapter bit at varied original Q states. Q is observed
   // from the physical oracle, so its real asynchronous priorities still win.
   for(integer n=0;n<256;n=n+1) begin
     load=0;#100;
     for(integer word_no=0;word_no<23;word_no=word_no+1) begin
       rng=random_next(rng);fixture[32*word_no+:32]=rng;
     end
     fixture[31:0]=32'h60000007;
     load=1;#100;
     rng=random_next(rng);pins[235:208]={rng[27:0]};
     rng=random_next(rng);pins[207:176]=rng;
     rng=random_next(rng);pins[175:144]=rng;
     check();
   end
   $display("R_FRONT_CONE PASS checks=%0d raw_function_cases=4096 sampled_Q_cases=256",checks);
   $finish;
 end
endmodule
`default_nettype wire
