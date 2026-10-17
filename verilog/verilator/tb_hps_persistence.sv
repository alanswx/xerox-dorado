// Drive real checked serial writes, then leave the head on the SAME track.
module tb_hps_persistence (
 input wire clk_host,sys_clk,verify_only,corrupt,read_only,attached,remounted,
 input wire sd_ack,sd_buff_wr,
 input wire [12:0] sd_buff_addr,
 input wire [15:0] sd_buff_dout,
 output wire [15:0] sd_buff_din,
 output wire [31:0] sd_lba,
 output wire [5:0] sd_blk_cnt,
 output wire sd_rd,sd_wr,pack_ready,pack_fault,pack_dirty,
 output wire [31:0] reads,writes,
 output reg [31:0] compared=0,
 output wire done
);
 reg [31:0] cycles=0;
 reg [2:0] div=0;
 reg data_adv=0;
 reg [15:0] pulse=0;
 reg [10:0] bit_count=0;
 always @(posedge sys_clk) begin
  cycles<=cycles+1'b1;div<=div+1'b1;data_adv<=div==6;
  if(data_adv)begin
   if(bit_count==1377)begin bit_count<=0;pulse<=pulse+1'b1;end
   else bit_count<=bit_count+1'b1;
  end
 end
 wire writing=!verify_only&&pulse>=4&&pulse<8;
 wire first_cyl=cycles>=8&&cycles<16;
 wire first_head=cycles>=24&&cycles<32;
 wire [11:0] tag_n=first_cyl ? ~12'd4 : first_head ? ~12'd4 : writing ? ~12'h084 : 12'hfff;
 wire reference_bit;
 wire write_bit=reference_bit ^ (corrupt&&pulse==6&&bit_count==500);
 dorado_pack reference_pack (.sys_clk(sys_clk),.attached(1'b1),.tagbus_n(tag_n),
  .cyltag_n(~first_cyl),.headtag_n(~first_head),.drivetag_n(1'b1),.conttag_n(~writing),
  .pulse(pulse),.data_adv(data_adv),.data_bit(reference_bit),
  .selected(1'b0),.write_data_p(1'b0),.write_data_m(1'b0));
 wire media_bit;
 dorado_pack_hps #(.INITIAL_CYL(4),.INITIAL_HEAD(4),.HOST_TIMEOUT_CLOCKS(20000)) u_medium (
  .clk_sys(clk_host),.machine_clk(sys_clk),.reset(1'b0),.attached(attached),.readonly(read_only),.remounted(remounted),
  .tagbus_n(tag_n),.cyltag_n(~first_cyl),.headtag_n(~first_head),.drivetag_n(1'b1),.conttag_n(~writing),
  .selected(1'b1),.data_adv(data_adv),.pulse(pulse),.write_data_p(~write_bit),.write_data_m(write_bit),
  .data_bit(media_bit),.ready(pack_ready),.fault(pack_fault),.dirty(pack_dirty),.reads(reads),.writes(writes),
  .sd_lba(sd_lba),.sd_blk_cnt(sd_blk_cnt),.sd_rd(sd_rd),.sd_wr(sd_wr),.sd_ack(sd_ack),
  .sd_buff_addr(sd_buff_addr),.sd_buff_dout(sd_buff_dout),.sd_buff_din(sd_buff_din),.sd_buff_wr(sd_buff_wr));
 always @(posedge sys_clk)if(verify_only&&data_adv&&pulse>=4&&pulse<8)begin
  if(reference_bit!==media_bit)$fatal(1,"persisted serial readback differs at pulse=%d bit=%d",pulse,bit_count);
  compared<=compared+1'b1;
 end
 assign done=verify_only ? pulse>=8 : writes>=1&&!pack_dirty&&pack_ready;
endmodule
