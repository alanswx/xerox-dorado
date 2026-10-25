// Compare the original ContA Ready capture using parallel capture cases.
// HM 4.9: the late capture event selects a comparison, rather than passing
// through the15 task updates first. No owner, clock or memory barrier changes.
`default_nettype none
module dorado_ready_compare_step(
 input wire [15:0] state_q,target,original_capture,
 input wire [27:0] data,input wire capture_event,
 output reg mismatch);
 wire [27:0] base_data;
 assign base_data=data & ~(28'h1<<15 | 28'h1<<17 | 28'h1<<18 | 28'h1<<20);
 wire [15:0] capture_cases[0:3];
 (* keep *) wire [3:0] capture_mismatch;
 genvar c;
 generate for(c=0;c<4;c=c+1)begin:cases
  wire [27:0] before_data=(base_data & ~(28'h3<<13)) | (28'(c)<<13);
  dorado_task_ready_step #(.INDEPENDENT_BNT(1)) possible_capture(
   state_q,before_data,before_data | (28'h1<<18),capture_cases[c],,,,);
  assign capture_mismatch[c]=|(capture_cases[c]^target);
 end endgenerate
 wire hold_mismatch=|(state_q^target),clear_mismatch=|target;
 wire event_eligible=capture_event && !(data[17]|data[18]|data[20]);
 always @*begin
  // The exact original comparison remains for every unknown owned/input bit.
  mismatch=|(original_capture^target);
  case(^{state_q,target,original_capture,data,capture_event})
   1'b0,1'b1:begin
    if(data[15])mismatch=clear_mismatch;
    else if(event_eligible)mismatch=capture_mismatch[data[14:13]];
    else mismatch=hold_mismatch;
   end
   default:begin end
  endcase
 end
endmodule
`default_nettype wire
