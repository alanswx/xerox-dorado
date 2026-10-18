// Passive counters, in FABRIC EDGE units. These are not instructions.
// machine_step is the falling-edge gate decision shared with the cable;
// each asserted sample at a host rising edge permits one machine rising edge.
`default_nettype none
module dorado_speed_monitor #(
    parameter integer BITS=32
) (
    input wire clk,reset,machine_step,
    output reg [BITS-1:0] host_edges=0,
    output reg [BITS-1:0] machine_edges=0
);
    always @(posedge clk) begin
        if(reset) begin host_edges<=0;machine_edges<=0;end
        else begin
            host_edges<=host_edges+1'b1;
            if(machine_step)machine_edges<=machine_edges+1'b1;
        end
    end
endmodule
`default_nettype wire
