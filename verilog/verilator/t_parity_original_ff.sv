// PARC EclDict's F10000 parallel-register path at ProcH/ProcL g16.
// Mode/serial/reset pins are physically unconnected at these two packages.
// No claim is made about unused F10000 modes. Fixture load is oracle seeding.
`timescale 1ps/1ps
`default_nettype none
module original_F10000 #(parameter integer DELAY_PS=1000)(
 input wire p4,p13,p11,p10,p9,p7,p6,p12,p5,
 output wire p14,p15,p2,p3,input wire fixture_load,
 input wire [3:0] fixture_state,output reg [3:0] state_q);
 wire clk=p4|p13;
 always @(posedge clk or posedge fixture_load) begin
   if(fixture_load) state_q<=fixture_state;
   else begin
     if(p4 || p5 || p6 || p12) $fatal(1,"g16 parallel reference used outside original pin contract");
     state_q<=#(DELAY_PS) {p11,p10,p9,p7};
   end
 end
 assign {p14,p15,p2,p3}=state_q;
endmodule
`default_nettype wire
