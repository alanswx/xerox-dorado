// Compare the passive enabled-edge count with an actual gated clock.
`timescale 1ns/1ps
`default_nettype none
module tb_speed_monitor;
    reg host=0;always #5 host=~host;
    reg reset=1,requested=0,step=0;
    always @(negedge host)step<=requested;
    wire machine=host && step;
    wire [5:0] hosts,machines;
    dorado_speed_monitor #(.BITS(6)) d(.clk(host),.reset(reset),.machine_step(step),
        .host_edges(hosts),.machine_edges(machines));
    integer expected_host=0,expected_machine=0,checks=0;
    always @(posedge machine) if(!reset)expected_machine++;
    always @(posedge host)begin
        if(reset)begin expected_host=0;expected_machine=0;end
        else expected_host++;
        #1;
        if(hosts!==6'(expected_host) || machines!==6'(expected_machine))
            $fatal(1,"monitor disagrees with actual gated edges: %d/%d expected %d/%d",
                hosts,machines,expected_host%64,expected_machine%64);
        checks++;
    end
    initial begin
        repeat(3)@(negedge host);#1;reset=0;
        for(integer n=0;n<400;n++)begin
            @(negedge host);#1;
            requested=(n%17<5);
            if(n==200)begin reset=1;requested=0;end
            if(n==205)reset=0;
        end
        repeat(3)@(negedge host);#1;
        if(checks<400 || expected_machine<40)$fatal(1,"incomplete counter coverage");
        $display("SPEED-MONITOR PASS: actual gated edges, pauses, reset and wrap; units=fabric edges");
        $finish;
    end
endmodule
`default_nettype wire
