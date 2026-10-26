// Host preparation only; no architectural Hold, phase or cache owner lives here.
// The native producer cut must prove raw_hold is a function of context. Reuse
// is permitted only for an identical context, including current cache read
// words/selects. A mismatch blocks publication until preparation completes.
// This candidate alone does not establish the original60ns capture schedule.
`default_nettype none
module dorado_memc_hold_buffer #(parameter integer CONTEXT_BITS=132)(
 input wire clk,reset,
 input wire [CONTEXT_BITS-1:0] source_context,
 input wire raw_hold,
 output wire ready,hold_value,
 output reg valid=1'b0,
 output reg [CONTEXT_BITS-1:0] saved_context,
 output reg saved_hold,
 output reg context_known);
 always @*begin
  // Preserve the actual raw four-state producer when it cannot be prepared
  // as a Boolean function. The caller retains every original readiness test.
  case(^{source_context,raw_hold})
   1'b0,1'b1:context_known=1'b1;
   default:context_known=1'b0;
  endcase
 end
 assign ready=!reset && (!context_known || (valid && saved_context==source_context));
 assign hold_value=context_known ? saved_hold:raw_hold;
 always @(posedge clk)begin
  if(reset)valid<=1'b0;
  else if(context_known)begin
   valid<=1'b1;
   saved_context<=source_context;
   saved_hold<=raw_hold;
  end
 end
endmodule
`default_nettype wire
