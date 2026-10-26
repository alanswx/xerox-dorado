// Unconnected native RIOT RAM implementation candidate.
// Keep the current M6532 model's reset-visible zeros without resetting the
// physical memory array. The per-byte validity mask is the only bulk-clear
// state. A reset does not publish or initialize the hidden backing payload.
// Registered reads sample the OLD word/validity, including a same-edge write.
// advance is the native virtual-clock enable, not the 6502 PHI2 enable.
`default_nettype none
module dorado_riot_ram (
 input wire clk,advance,reset,read_enable,write_enable,
 input wire [6:0] address,
 input wire [7:0] write_data,
 output wire [7:0] read_data
);
 (* ramstyle = "M10K" *) reg [7:0] backing [0:127];
 reg [127:0] written;
 reg [7:0] read_payload;
 reg read_written;

 always @(posedge clk) if(advance) begin
  if(reset) written<=128'b0;
  else if(write_enable) written[address]<=1'b1;
 end
 // Deliberately no reset assignment to the backing array or read register.
 // The procedural reset branch also preserves the reference's X/Z behavior:
 // an unknown reset takes the else branch rather than suppressing a write.
 always @(posedge clk) if(advance) begin
  if(reset) begin end
  else if(write_enable) backing[address]<=write_data;
  if(read_enable) read_payload<=backing[address];
 end
 always @(posedge clk) if(advance && read_enable)
  read_written<=written[address];

 assign read_data=read_written ? read_payload : 8'b0;
endmodule
`default_nettype wire
