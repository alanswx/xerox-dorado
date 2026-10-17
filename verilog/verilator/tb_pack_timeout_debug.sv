module tb_pack_timeout_debug;
reg clk=0;always #1 clk=~clk;
reg reset=1;wire fault;wire[31:0]detail,extra;
dorado_pack_hps #(.LABEL_WORDS(10),.INITIAL_CYL(3),.HOST_TIMEOUT_CLOCKS(16)) dut(
 .clk_sys(clk),.machine_clk(clk),.reset(reset),.attached(1'b1),.readonly(1'b0),.remounted(1'b0),
 .tagbus_n(12'hfff),.cyltag_n(1'b1),.headtag_n(1'b1),.drivetag_n(1'b1),.conttag_n(1'b1),
 .selected(1'b1),.data_adv(1'b0),.pulse(16'd0),.write_data_p(1'b1),.write_data_m(1'b0),
 .fault(fault),.fault_debug(detail),.fault_extra(extra),.sd_ack(1'b0),.sd_buff_addr(13'd0),.sd_buff_dout(16'd0),.sd_buff_wr(1'b0));
initial begin
 repeat(3)@(negedge clk);reset=0;repeat(30)@(negedge clk);
 if(!fault||detail[31:28]!=8||extra!=457)$fatal(1,"Timeout diagnostic: %h %h",detail,extra);
 repeat(10)@(negedge clk);
 if(detail[31:28]!=8||extra!=457)$fatal(1,"First timeout changed");
 $display("PACK-TIMEOUT PASS: reason8 and original requested LBA457 retained; no completion fabricated");$finish;
end
endmodule
