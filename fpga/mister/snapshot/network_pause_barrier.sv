// Native network service boundary. Engine hold follows the final global guest
// hold; parser hold is released earlier so shared SPI framing can settle before
// the memory coordinator permits the first guest edge. Physical DDR visibility
// and the Main/daemon lease are separate requirements, never inferred here.
`default_nettype none
module dorado_network_pause_barrier (
    input wire clk,machine_reseed,owner_hold,guest_held,abort,device_reset,
    input wire engine_idle,parser_idle,parser_held,device_fault,
    output wire hold,parser_hold,ready,fault
);
    localparam RUN=3'd0,DRAIN=3'd1,SETTLE=3'd2,FROZEN=3'd3,
               RELEASE=3'd4,QUARANTINE=3'd5;
    // All five bits are required top-level implementation state.
    reg [2:0] phase=RUN;
    reg [1:0] settle_count=0;
    wire requested=(owner_hold!==1'b0);
    wire drained=(guest_held===1'b1) && (engine_idle===1'b1) &&
                 (parser_idle===1'b1) && (parser_held===1'b1);
    wire unhealthy=(abort!==1'b0) || (device_reset!==1'b0) || (device_fault!==1'b0);
    assign hold=requested || phase!=RUN;
    assign parser_hold=requested || (phase!=RUN && phase!=RELEASE);
    assign fault=phase==QUARANTINE;
    assign ready=phase==FROZEN && (owner_hold===1'b1) && drained && !unhealthy;
    always @(posedge clk)begin
        if(machine_reseed)begin phase<=RUN;settle_count<=0;end
        else if((requested || phase!=RUN) && unhealthy)phase<=QUARANTINE;
        else case(phase)
            RUN:if(requested)begin phase<=DRAIN;settle_count<=0;end
            DRAIN:if(owner_hold===1'b0)phase<=RELEASE;
                  else if(owner_hold===1'b1 && drained)begin phase<=SETTLE;settle_count<=0;end
            SETTLE:if(owner_hold===1'b0)begin phase<=RELEASE;settle_count<=0;end
                   else if(!drained || owner_hold!==1'b1)begin phase<=DRAIN;settle_count<=0;end
                   else if(settle_count==1)phase<=FROZEN;
                   else settle_count<=settle_count+1'b1;
            FROZEN:if(owner_hold===1'b0)phase<=RELEASE;
                   else if(owner_hold!==1'b1 || !drained)phase<=QUARANTINE;
            RELEASE:if(requested)begin phase<=DRAIN;settle_count<=0;end
                    else if(parser_held===1'b0)begin phase<=RUN;settle_count<=0;end
            QUARANTINE:phase<=QUARANTINE;
            default:phase<=QUARANTINE;
        endcase
    end
endmodule
`default_nettype wire
