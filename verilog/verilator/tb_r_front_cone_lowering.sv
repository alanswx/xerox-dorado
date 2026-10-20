`timescale 1ns/1ps
`default_nettype none
module tb_r_front_cone_lowering;
 reg [735:0] q=0;
 reg [235:0] pins=0;
 reg base=0;
 wire [735:0] original_q,lowered_q;
 wire [56:0] original_clocks,lowered_clocks;
 wire [201:0] original_source,lowered_source;
 wire [7:0] original_fa,lowered_fa;
 wire [43:0] original_data,lowered_data;
 wire [31:0] original_address,lowered_address;
 wire [3:0] original_select,lowered_select;
 wire [1:0] original_forward,lowered_forward;
 original_form_dorado_r_front_cone original(.state_q(q),.base(base),.pins(pins),
   .sampled_q(original_q),.clocks(original_clocks),.source_pins(original_source),
   .fa_n(original_fa),.front_data(original_data),.address(original_address),
   .select_n(original_select),.forwarded_bypass(original_forward));
 dorado_r_front_cone lowered(.state_q(q),.base(base),.pins(pins),
   .sampled_q(lowered_q),.clocks(lowered_clocks),.source_pins(lowered_source),
   .fa_n(lowered_fa),.front_data(lowered_data),.address(lowered_address),
   .select_n(lowered_select),.forwarded_bypass(lowered_forward));
 // All outputs consumed by the five cone views in dorado_r_front_step,
 // plus every sampled adapter bit. No clock or state update occurs here.
 wire [1084:0] original_value={original_q,original_clocks,original_source,original_fa,
   original_data,original_address,original_select,original_forward};
 wire [1084:0] lowered_value={lowered_q,lowered_clocks,lowered_source,lowered_fa,
   lowered_data,lowered_address,lowered_select,lowered_forward};
 reg [31:0] rng=32'hd03af101;
 function automatic [31:0] random_next(input [31:0] v);
   reg [31:0] x;
   begin x=v^(v<<13);x=x^(x>>17);random_next=x^(x<<5);end
 endfunction
 integer roots[0:1];
 initial begin
   roots[0]=0;roots[1]=0;
   for(integer sample=0;sample<16384;sample=sample+1) begin
     for(integer word_no=0;word_no<23;word_no=word_no+1) begin
       rng=random_next(rng);q[word_no*32+:32]=rng;
     end
     for(integer word_no=0;word_no<8;word_no=word_no+1) begin
       rng=random_next(rng);
       for(integer bit_no=0;bit_no<32;bit_no=bit_no+1)
         if(word_no*32+bit_no<236) pins[word_no*32+bit_no]=rng[bit_no];
     end
     base=rng[0];roots[base]=roots[base]+1;
     #1;
     if((^original_value)===1'bx) $fatal(1,"R front lowering unknown original output sample=%0d",sample);
     if(original_value!==lowered_value)
       $fatal(1,"R front lowering mismatch sample=%0d root=%b",sample,base);
   end
   if(!roots[0]||!roots[1]) $fatal(1,"R front lowering missing root polarity");
   $display("R_FRONT_LOWERING PASS checks=16384 observed_bits=1085 roots_low=%0d roots_high=%0d",roots[0],roots[1]);
   $finish;
 end
endmodule
`default_nettype wire
