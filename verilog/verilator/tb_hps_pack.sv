// Compare the synthesizable medium with the qualified file-backed serial
// medium, including all header/label/data bits and Fire-code bits. Loop one
// sector back into its write channel, seek, and verify actual HPS writeback.
module tb_hps_pack (
 input wire clk_host, sys_clk,
 input wire sd_ack, sd_buff_wr,
 input wire [12:0] sd_buff_addr,
 input wire [15:0] sd_buff_dout,
 output wire [15:0] sd_buff_din,
 output wire [31:0] sd_lba,
 output wire [5:0] sd_blk_cnt,
 output wire sd_rd, sd_wr, pack_ready, pack_fault,
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
 wire cyl_first=cycles>=8&&cycles<16;
 wire head_first=cycles>=24&&cycles<32;
 wire seek_second=pulse==8&&bit_count>=20&&bit_count<24;
 wire writing=pulse>=4&&pulse<8;
 wire [11:0] tag_n=cyl_first ? ~12'd4 : head_first ? ~12'd4 : seek_second ? ~12'd5 : writing ? ~12'h084 : 12'hfff;
 wire cyltag_n=~(cyl_first||seek_second),headtag_n=~head_first,conttag_n=~writing;
 wire reference_bit,media_bit;
 dorado_pack reference_pack (.sys_clk(sys_clk),.attached(1'b1),.tagbus_n(tag_n),
  .cyltag_n(cyltag_n),.headtag_n(headtag_n),.drivetag_n(1'b1),.conttag_n(conttag_n),
  .pulse(pulse),.data_adv(data_adv),.data_bit(reference_bit),
  .selected(1'b0),.write_data_p(1'b0),.write_data_m(1'b0));
 dorado_pack_hps #(.INITIAL_CYL(4),.INITIAL_HEAD(4)) u_medium (
  .clk_sys(clk_host),.machine_clk(sys_clk),.reset(1'b0),.attached(1'b1),.readonly(1'b0),
  .tagbus_n(tag_n),.cyltag_n(cyltag_n),.headtag_n(headtag_n),.drivetag_n(1'b1),.conttag_n(conttag_n),
  .selected(1'b1),.data_adv(data_adv),.pulse(pulse),.write_data_p(~reference_bit),.write_data_m(reference_bit),
  .data_bit(media_bit),.ready(pack_ready),.fault(pack_fault),
  .sd_lba(sd_lba),.sd_blk_cnt(sd_blk_cnt),.sd_rd(sd_rd),.sd_wr(sd_wr),.sd_ack(sd_ack),
  .sd_buff_addr(sd_buff_addr),.sd_buff_dout(sd_buff_dout),.sd_buff_din(sd_buff_din),.sd_buff_wr(sd_buff_wr));
 always @(posedge sys_clk)if(data_adv&&((pulse>=4&&pulse<8)||pulse>=12))begin
  if(reference_bit!==media_bit)$fatal(1,"serial medium mismatch pulse=%d bit=%d reference=%b medium=%b",pulse,bit_count,reference_bit,media_bit);
  compared<=compared+1'b1;
 end
 assign done=pulse>=16;
endmodule
