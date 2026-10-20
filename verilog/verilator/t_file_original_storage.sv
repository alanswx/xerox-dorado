// Test references: original level-sensitive mux latches and 16x4 SRAM.
// No fabric-clock sampling or imported machine state. Fixture load is oracle
// seeding only. DELAY_PS is an illustrative arrival parameter, not a part spec.
`timescale 1ps/1ps
`default_nettype none
module original_MC10173 #(parameter integer DELAY_PS=1000)(
 input wire p5,p3,p12,p10,p6,p4,p13,p11,p9,p7,
 output wire p1,p2,p15,p14,input wire fixture_load,
 input wire [3:0] fixture_state,output reg [3:0] state_q);
 always @(fixture_load or fixture_state or p7 or p9 or p5 or p3 or p12 or p10 or p6 or p4 or p13 or p11) begin
   if(fixture_load) state_q<=fixture_state;
   else if(!p7) state_q<=#(DELAY_PS) (p9 ? {p11,p13,p4,p6} : {p10,p12,p3,p5});
 end
 assign {p14,p15,p2,p1}=state_q;
endmodule

module original_F10145A(
 input wire p10,p9,p7,p6,p5,p4,p11,p12,p3,p13,p8,
 output wire p2,p1,p15,p14,input wire fixture_load,
 input wire [63:0] fixture_state,output reg [63:0] state_q);
 wire [3:0] address={p10,p9,p7,p6};
 always @(fixture_load or fixture_state or p3 or p13 or p10 or p9 or p7 or p6 or p5 or p4 or p11 or p12) begin
   if(fixture_load) state_q=fixture_state;
   else if(!p3 && !p13) state_q[4*address+:4]={p12,p11,p4,p5};
 end
 assign {p14,p15,p1,p2}=p3 ? 4'b0 : state_q[4*address+:4];
endmodule
`default_nettype wire
