module snapshot_enable_fixture(
 input wire guest_advance,host_clk,ref_clk,input wire [9:0] address,
 input wire [17:0] data,input wire ce_n,we_n,
 output wire [17:0] q,original_q,
 input wire freeze,guest_held,request,write,
 input wire [10:0] word_index,input wire [17:0] state_data,
 output wire frozen,done,error,output wire [17:0] result);
 dorado_snapshot_f10415_enable_bank candidate(host_clk,guest_advance,address,data,
   ce_n,we_n,q,freeze,guest_held,frozen,request,write,word_index,state_data,
   done,error,result);
 genvar i;
 generate for(i=0;i<18;i=i+1)begin: original
   cell_F10415A bit_cell(ref_clk,address[9],address[8],address[7],address[6],
     address[5],address[4],address[3],address[2],address[1],address[0],
     data[i],ce_n,we_n,original_q[i]);
 end endgenerate
endmodule
