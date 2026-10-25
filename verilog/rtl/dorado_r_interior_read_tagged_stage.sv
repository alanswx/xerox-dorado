// One-port interior-read stage with descriptor validation. Component only.
// The owner and SRAM writes must share the caller's ready/advance barrier.
// Live port descriptors may change while a read is pending. An obsolete
// snapshot cannot become valid/ready, even before the next host clock edge.
// Descriptor tags/data are auxiliary and uninitialized, not guest owners.
`default_nettype none
module dorado_r_interior_read_tagged_stage(
 input wire clk,reset,start,need,read_accept,finish,
 input wire [31:0] before_address,interior_address,
 input wire [3:0] before_select,interior_select,
 input wire [17:0] port_data,
 output wire [31:0] port_address,
 output wire [3:0] port_select,
 output wire active,select_interior,capture_valid,ready,
 output wire [17:0] before_data,capture_data,
 output wire busy,tag_match);
 wire [71:0] descriptor={before_address,interior_address,before_select,interior_select};
 reg [71:0] descriptor_q;
 wire raw_active,raw_select,raw_valid,raw_ready;
 wire [31:0] raw_address;wire [3:0] raw_port_select;
 assign busy=raw_active;
 assign tag_match=!raw_active || (descriptor_q==descriptor);
 wire obsolete=raw_active && !tag_match;
 assign active=raw_active && tag_match && !reset;
 assign select_interior=raw_select && tag_match && !reset;
 assign capture_valid=raw_valid && tag_match && !reset;
 assign ready=raw_ready && tag_match && !reset;
 // A rejected request exposes the live BEFORE port, never its old buffer.
 assign port_address=active ? raw_address:before_address;
 assign port_select=active ? raw_port_select:before_select;
 always @(posedge clk)
  if(!reset && !raw_active && start && need)descriptor_q<=descriptor;
 dorado_r_interior_read_stage transaction(clk,reset || obsolete,start,need,read_accept,finish,
  before_address,interior_address,before_select,interior_select,port_data,
  raw_address,raw_port_select,raw_active,raw_select,raw_valid,raw_ready,before_data,capture_data);
endmodule
`default_nettype wire
