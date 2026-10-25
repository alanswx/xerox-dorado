// One physical R SRAM port, read at BEFORE and interior capture epochs.
// Component experiment. The caller freezes owner, pins and port descriptors
// from start until release. It may publish the machine edge only when ready.
// Reset invalidates this auxiliary transaction; it never resets guest state.
// Ordinary need=0 transactions pass through without an extra clock cycle.
`default_nettype none
module dorado_r_interior_read_stage(
 input wire clk,reset,start,need,read_accept,finish,
 input wire [31:0] before_address,interior_address,
 input wire [3:0] before_select,interior_select,
 input wire [17:0] port_data,
 output wire [31:0] port_address,
 output wire [3:0] port_select,
 output wire active,select_interior,capture_valid,ready,
 output reg [17:0] before_data,capture_data);
 localparam [1:0] IDLE=0,READ=1,DONE=2;
 reg [1:0] phase=IDLE;
 assign active=(phase!=IDLE);
 assign select_interior=(phase==READ);
 assign capture_valid=(phase==DONE);
 assign ready=(phase==DONE) || ((phase==IDLE) && !need);
 assign port_address=select_interior ? interior_address:before_address;
 assign port_select=select_interior ? interior_select:before_select;
 always @(posedge clk)begin
  if(reset)phase<=IDLE;
  else case(phase)
   IDLE:if(start && need)begin before_data<=port_data;phase<=READ;end
   READ:if(read_accept)begin capture_data<=port_data;phase<=DONE;end
   DONE:if(finish)phase<=IDLE;
   default:phase<=IDLE;
  endcase
 end
endmodule
`default_nettype wire
