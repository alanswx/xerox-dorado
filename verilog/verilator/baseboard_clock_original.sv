// Independent event-driven package oracle for the original BaseBoard wiring.
// Test fixture loading imports the six physical Qs; it is not a guest reset.
`timescale 1ns/1ps
`default_nettype none
module baseboard_clock_original(
 input wire fixture_load,input wire [5:0] fixture_state,
 // One packet gives all j02 inputs equal fixture depth at simultaneous arrival.
 input wire [6:0] source_and_debug,
 output wire [5:0] state_q,output wire root,max_clock,mcpre,aux_n,
 output wire [223:0] counts
);
 reg [3:0] e07;
 reg g08a,g08b;
 wire load_n;
 wire crystal=source_and_debug[4];
 cell_SN74LS04 c08(.p1(max_clock),.p2(load_n),.p3(1'b0),.p5(1'b0),.p9(1'b0),.p11(1'b0),.p13(1'b0),.p7(1'b0),.p14(1'b1));
 // j02 is ONE three-input OR gate. Two binary | operators in a continuous
 // assignment create a two-gate delta cascade and a zero-time fixture runt.
 // Preserve its original truth function with the corresponding gate primitive.
 // Real pin skew and analog runt-pulse qualification remain separate.
 or j02(root, source_and_debug[0], source_and_debug[1], source_and_debug[6]);
 // LS163 synchronous active-low load, EP/ET/clear tied high; parallel D=1011.
 always @(posedge crystal or posedge fixture_load)
  if(fixture_load)e07<=fixture_state[3:0];
  else if(!load_n)e07<=4'b1011;
  else e07<=e07+1'b1;
 assign max_clock=&e07;
 // Both halves' preset/clear inputs are tied high on this clock cone.
 always @(posedge max_clock or posedge fixture_load)
  if(fixture_load)g08a<=fixture_state[4];else g08a<=mcpre;
 assign mcpre=~g08a;
 always @(posedge mcpre or posedge fixture_load)
  if(fixture_load)g08b<=fixture_state[5];else g08b<=aux_n;
 assign aux_n=~g08b;
 assign state_q={g08b,g08a,e07};
 integer e07_edges=0,g08a_edges=0,g08b_edges=0,root_rises=0,root_falls=0,pre_falls=0,pre_rises=0;
 always @(posedge crystal)if(!fixture_load)e07_edges=e07_edges+1;
 always @(posedge max_clock)if(!fixture_load)g08a_edges=g08a_edges+1;
 always @(posedge mcpre)if(!fixture_load)begin g08b_edges=g08b_edges+1;pre_rises=pre_rises+1;end
 always @(negedge mcpre)if(!fixture_load)pre_falls=pre_falls+1;
 always @(posedge root)if(!fixture_load)root_rises=root_rises+1;
 always @(negedge root)if(!fixture_load)root_falls=root_falls+1;
 assign counts={32'(pre_rises),32'(pre_falls),32'(root_falls),32'(root_rises),32'(g08b_edges),32'(g08a_edges),32'(e07_edges)};
endmodule
`default_nettype wire
