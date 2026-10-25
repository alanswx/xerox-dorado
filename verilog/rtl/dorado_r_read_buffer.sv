// Auxiliary registered RM/STK read frontier. HM 2.3/3.1: prepare the physical
// read between t0 and t1; the consumer still chooses its architectural capture.
// Keep each9-bit bank separate so simultaneous RM/STK selects retain wired OR.
// Own committed writes refresh the response AFTER an edge; a consumer on that
// edge still sees its pre-write data. No RAM, guest reset or second owner here.
`default_nettype none
module dorado_r_read_buffer(
 input wire clk,reset,request,input wire [31:0] read_address,input wire [3:0] read_select_n,
 input wire [35:0] physical_bank_data,
 input wire write_commit,input wire [1:0] write_n,input wire [31:0] write_address,
 input wire [3:0] write_select_n,input wire [15:0] write_data,
 output reg valid=1'b0,output wire ready,output wire [17:0] response,
 output reg [35:0] response_tag,output reg [35:0] bank_data);
 wire [31:0] normalized_address;
 genvar bank;
 generate for(bank=0;bank<4;bank=bank+1)begin:normalize
  assign normalized_address[8*bank+:8]=read_select_n[bank] ? 8'b0:read_address[8*bank+:8];
 end endgenerate
 wire [35:0] live_tag={read_select_n,normalized_address};
 assign ready=valid && !reset && (response_tag==live_tag);
 wire [8:0] qh=(response_tag[32] ? 9'b0:bank_data[8:0]) |
                (response_tag[33] ? 9'b0:bank_data[17:9]);
 wire [8:0] ql=(response_tag[34] ? 9'b0:bank_data[26:18]) |
                (response_tag[35] ? 9'b0:bank_data[35:27]);
 assign response={ql[8],qh[8],ql[7:0],qh[7:0]};
 reg [35:0] next_data;reg [31:0] capture_address;reg [7:0] byte_data;
 always @*begin
  next_data=request ? physical_bank_data:bank_data;
  capture_address=request ? read_address:response_tag[31:0];byte_data=0;
  for(integer i=0;i<4;i=i+1)begin
   byte_data=i<2 ? write_data[7:0]:write_data[15:8];
   if(write_commit && !write_n[i/2] && !write_select_n[i] &&
      write_address[8*i+:8]==capture_address[8*i+:8])
    next_data[9*i+:9]={~^byte_data,byte_data};
  end
 end
 always @(posedge clk)begin
  if(reset)valid<=0;
  else begin
   if(request)begin valid<=1;response_tag<=live_tag;end
   bank_data<=next_data;
  end
 end
endmodule
`default_nettype wire
