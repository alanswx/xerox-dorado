// Parallel F10000 at the two original stack-save positions. Its two clock
// inputs are ORed. Pin4 is the real StkPSaveEn' gate; the mode/reset/serial
// pins are unconnected here. Fixture load belongs only to the test oracle.
`timescale 1ps/1ps
`default_nettype none
module original_F10000_gated #(parameter integer DELAY_PS=1000)(
 input wire p4,p13,p11,p10,p9,p7,p6,p12,p5,
 output wire p14,p15,p2,p3,input wire fixture_load,
 input wire [3:0] fixture_state,output reg [3:0] state_q);
 wire clk=p4|p13;
 always @(posedge clk or posedge fixture_load) begin
   if(fixture_load) state_q<=fixture_state;
   else begin
     if(p5 || p6 || p12) $fatal(1,"stack-save parallel reference used outside original pin contract");
     state_q<=#(DELAY_PS) {p11,p10,p9,p7};
   end
 end
 assign {p14,p15,p2,p3}=state_q;
endmodule
`default_nettype wire
