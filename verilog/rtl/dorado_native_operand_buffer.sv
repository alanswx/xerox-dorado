// Prepare the native pre-event transparent operand/flag view on a host edge.
// Its88 projected bits depend only on saved owners/before pins/root/SRAM word,
// proved against the complete native single function. No guest owner or RAM.
// The actual raw view remains the X/Z and paired-path fallback. A tag mismatch
// prevents publication until the new view has been prepared; no raw bypass in
// the ordinary binary path. Fit into original t0..t1; this alone is not60ns.
`default_nettype none
module dorado_native_operand_buffer(
 input wire clk,reset,request,use_pair,context_known,
 input wire [843:0] held_q,input wire before_root,input wire [400:0] before_pins,
 input wire [17:0] before_data,input wire [843:0] raw_view,
 output wire ready,output wire [843:0] physical_view,
 output reg valid=1'b0,output reg [1263:0] saved_tag,output reg [87:0] saved_fields);
 localparam [1263:0] OPERAND_TAG_SUPPORT=1264'hfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff800000000000000000000000000000000000000000000000000000003c00002800001ff803fffffffc0000000000000000000ffff;
 wire [1263:0] current_tag={held_q,before_root,before_pins,before_data} & OPERAND_TAG_SUPPORT;
 wire [843:0] prepared_view={held_q[843:596],saved_fields[87:80],held_q[587:572],
   saved_fields[79:48],held_q[539:404],saved_fields[47:16],held_q[371:116],
   saved_fields[15:0],held_q[99:0]};
 assign physical_view=(use_pair || !context_known) ? raw_view:prepared_view;
 assign ready=!reset && (use_pair || !context_known || (valid && saved_tag==current_tag));
 always @(posedge clk)begin
  if(reset)valid<=0;
  else if(request && !use_pair && context_known)begin
   valid<=1;saved_tag<=current_tag;
   saved_fields<={raw_view[595:588],raw_view[571:540],raw_view[403:372],raw_view[115:100]};
  end
 end
endmodule
`default_nettype wire
