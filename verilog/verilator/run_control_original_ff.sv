// Ideal event-driven sequential pin models for the extracted Xerox graph.
// These are test references, not synthesis replacements for library cells.
`timescale 1ns/1ps
`default_nettype none
module original_MC10231 (
    input wire p1,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p16,
    output wire p2,p3,p14,p15,
    input wire fixture_load,input wire [1:0] fixture_state,
    output reg [1:0] state_q
);
    wire ca=p9|p6,cb=p9|p11;
    always @(posedge ca or posedge p5 or posedge p4 or posedge fixture_load)
        if(p5) state_q[0]<=1;else if(p4) state_q[0]<=0;
        else if(fixture_load) state_q[0]<=fixture_state[0];else state_q[0]<=p7;
    always @(posedge cb or posedge p12 or posedge p13 or posedge fixture_load)
        if(p12) state_q[1]<=1;else if(p13) state_q[1]<=0;
        else if(fixture_load) state_q[1]<=fixture_state[1];else state_q[1]<=p10;
    assign p2=state_q[0],p3=~state_q[0],p15=state_q[1],p14=~state_q[1];
endmodule

module original_SE10231 (
    input wire p6,p7,p9,p10,p11,p13,
    output wire p2,p3,p14,p15,
    input wire fixture_load,input wire [1:0] fixture_state,
    output reg [1:0] state_q
);
    wire ca=p9|p6,cb=p9|p11;
    always @(posedge ca or posedge fixture_load)
        if(fixture_load) state_q[0]<=fixture_state[0];else state_q[0]<=p7;
    always @(posedge cb or posedge p13 or posedge fixture_load)
        if(p13) state_q[1]<=0;else if(fixture_load) state_q[1]<=fixture_state[1];
        else state_q[1]<=p10;
    assign p2=state_q[0],p3=~state_q[0],p15=state_q[1],p14=~state_q[1];
endmodule

module original_MC10176 (
    input wire p1,p5,p6,p7,p8,p9,p10,p11,p12,p16,
    output wire p2,p3,p4,p13,p14,p15,
    input wire fixture_load,input wire [5:0] fixture_state,
    output reg [5:0] state_q
);
    always @(posedge p9 or posedge fixture_load)
        if(fixture_load) state_q<=fixture_state;
        else state_q<={p12,p11,p10,p7,p6,p5};
    assign {p15,p14,p13,p4,p3,p2}=state_q;
endmodule
`default_nettype wire
