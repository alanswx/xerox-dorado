// Passive control-board progress, not a host-edge speed estimate.
// HM Timing/Figure7: StartCycle begins each two-clock ordinary cycle;
// IM/TPC accesses insert four clocks. RepeatCur marks a held cycle.
// The issued count excludes held cycles and CP maintenance/stop activity.
// This is instruction ISSUE, not a pipeline-drained retirement counter.
`default_nettype none
module dorado_instruction_monitor #(
    parameter integer BITS=32
) (
    input wire clk,reset,
    input wire start_cycle,repeat_current,clock2,
    input wire running,jam,
    output reg [BITS-1:0] issued=0,held=0,cycles=0,clocks=0
);
    reg start_d=0,clock_d=0;
    always @(posedge clk)begin
        start_d<=start_cycle;clock_d<=clock2;
        if(reset)begin issued<=0;held<=0;cycles<=0;clocks<=0;end
        else if(running && !jam)begin
            if(clock2 && !clock_d)clocks<=clocks+1'b1;
            if(start_cycle && !start_d)begin
                cycles<=cycles+1'b1;
                if(repeat_current)held<=held+1'b1;
                else issued<=issued+1'b1;
            end
        end
    end
endmodule
`default_nettype wire
