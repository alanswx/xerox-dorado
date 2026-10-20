// Original MC10141 event model for task Curr/Last capture. Test oracle only.
`timescale 1ns/1ps
`default_nettype none
module original_MC10141(input wire p1,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,
 output wire p2,p3,p14,p15,input wire fixture_load,input wire [3:0] fixture_state,
 output reg [3:0] state_q);
 always @(posedge p4 or posedge fixture_load)
   if(fixture_load) state_q<=fixture_state;
   else case({p10,p7})
     2'b00:state_q<={p6,p9,p11,p12};
     2'b01:state_q<={p5,state_q[3:1]};
     2'b10:state_q<={state_q[2:0],p13};
     2'b11:state_q<=state_q;
   endcase
 assign {p3,p2,p15,p14}=state_q;
endmodule
`default_nettype wire
