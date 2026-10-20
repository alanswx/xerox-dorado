// Original physical SRAM reference: asynchronous read and level-sensitive
// write. Fixture state is test-only initialization/observation, not core I/O.
// Delays are explicit illustrative arrival controls, not device qualification.
`timescale 1ps/1ps
`default_nettype none
module original_MB7071H #(parameter integer READ_PS=1000)(
 input wire p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,
 output wire p20,p21,p22,p23,input wire fixture_load,
 input wire [1023:0] fixture_state,output reg [1023:0] state_q);
 wire [7:0] address={p7,p8,p9,p10,p11,p12,p13,p14};
 wire selected=!(p3|p4|p5|p6);
 always @(fixture_load or fixture_state or p2 or p3 or p4 or p5 or p6 or
          p7 or p8 or p9 or p10 or p11 or p12 or p13 or p14 or p16 or p17 or p18 or p19)
   if(fixture_load) state_q=fixture_state;
   else if(selected && !p2) state_q[4*address+:4]={p16,p17,p18,p19};
 assign #(READ_PS) {p20,p21,p22,p23}=selected ? state_q[4*address+:4] : 4'b0;
endmodule

module original_F10414 #(parameter integer READ_PS=1000)(
 input wire p1,p2,p3,p4,p7,p9,p10,p11,p12,p13,p14,
 output wire p15,input wire fixture_load,
 input wire [255:0] fixture_state,output reg [255:0] state_q);
 wire [7:0] address={p1,p2,p3,p4,p9,p10,p11,p12};
 always @(fixture_load or fixture_state or p1 or p2 or p3 or p4 or p7 or
          p9 or p10 or p11 or p12 or p13 or p14)
   if(fixture_load) state_q=fixture_state;
   else if(!p7 && !p14) state_q[address]=p13;
 assign #(READ_PS) p15=p7 ? 1'b0 : state_q[address];
endmodule
`default_nettype wire
