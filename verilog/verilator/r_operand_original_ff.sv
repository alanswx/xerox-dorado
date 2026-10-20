// Physical-pin MC1668 HIGH-transparent latch reference. Motorola 1978 data
// book p235 /4-30: C=1 follows D regardless of S/R; C=0 enables S/R or hold.
// S=R=1,C=0 is undefined and excluded from qualification.
`timescale 1ps/1ps
`default_nettype none
module original_MC1668 #(parameter integer DELAY_PS=1000)(
 input wire p4,p5,p6,p7,p9,p11,p12,p13,
 output wire p2,p3,p14,p15,input wire fixture_load,
 input wire [1:0] fixture_state,output reg [1:0] state_q);
 always @(p7 or p4 or p5 or p6 or fixture_load or fixture_state)
   if(fixture_load) state_q[0]<=fixture_state[0];
   else if(p7) state_q[0]<=#(DELAY_PS) p6;
   else if(p5) state_q[0]<=#(DELAY_PS) 1'b1;
   else if(p4) state_q[0]<=#(DELAY_PS) 1'b0;
 always @(p9 or p12 or p13 or p11 or fixture_load or fixture_state)
   if(fixture_load) state_q[1]<=fixture_state[1];
   else if(p9) state_q[1]<=#(DELAY_PS) p11;
   else if(p12) state_q[1]<=#(DELAY_PS) 1'b1;
   else if(p13) state_q[1]<=#(DELAY_PS) 1'b0;
 assign p2=state_q[0];assign p3=~state_q[0];
 assign p15=state_q[1];assign p14=~state_q[1];
endmodule
`default_nettype wire
