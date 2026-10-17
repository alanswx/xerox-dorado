// Actual HPS track buffer and serial medium, with Pilot's ten-word label.
module tb_pilot_pack(
 input wire clk_host,sys_clk,reset,attached,data_adv,writing,write_bit,
 input wire [15:0] pulse,
 input wire sd_ack,sd_buff_wr,
 input wire [12:0] sd_buff_addr,
 input wire [15:0] sd_buff_dout,
 output wire [15:0] sd_buff_din,
 output wire [31:0] sd_lba, fault_debug, fault_extra,
 output wire [5:0] sd_blk_cnt,
 output wire sd_rd,sd_wr,pack_ready,pack_fault,media_bit
);
 dorado_pack_hps #(.INITIAL_CYL(3),.LABEL_WORDS(10)) u_medium(
  .clk_sys(clk_host),.machine_clk(sys_clk),.reset(reset),.attached(attached),
  .readonly(1'b0),.remounted(1'b0),.tagbus_n(writing ? ~12'h084 : 12'hfff),
  .cyltag_n(1'b1),.headtag_n(1'b1),.drivetag_n(1'b1),.conttag_n(~writing),
  .selected(1'b1),.data_adv(data_adv),.pulse(pulse),
  .write_data_p(~write_bit),.write_data_m(write_bit),.data_bit(media_bit),
  .ready(pack_ready),.fault(pack_fault),.fault_debug(fault_debug),.fault_extra(fault_extra),
  .sd_lba(sd_lba),.sd_blk_cnt(sd_blk_cnt),.sd_rd(sd_rd),.sd_wr(sd_wr),.sd_ack(sd_ack),
  .sd_buff_addr(sd_buff_addr),.sd_buff_dout(sd_buff_dout),.sd_buff_wr(sd_buff_wr),.sd_buff_din(sd_buff_din));
endmodule
