// Snapshot preparation only: hold guest state, drain raw work, then freeze
// frontends. Keep SDRAM physical refresh enabled in every phase. A true
// machine reseed may clear quarantine; parser reset or PLL loss must not.
`default_nettype none
module dorado_memory_pause_barrier (
    input wire clk,machine_reseed,owner_hold,guest_held,abort,
    input wire frontends_drained,arbiter_idle,raw_service_idle,
    input wire frontends_frozen,
    output wire guest_hold,frontend_hold,ready,fault
);
    localparam RUN=2'd0,DRAIN=2'd1,FROZEN=2'd2,QUARANTINE=2'd3;
    // Both phase bits are REQUIRED wrapper state, not guest board state.
    reg [1:0] phase=RUN;
    wire drained=(guest_held===1'b1) && (frontends_drained===1'b1) &&
                 (arbiter_idle===1'b1) && (raw_service_idle===1'b1);
    assign guest_hold=owner_hold || phase!=RUN;
    assign frontend_hold=phase==FROZEN || phase==QUARANTINE;
    assign fault=phase==QUARANTINE;
    // Do not recheck frontend data while a serial read rotates those bits.
    // The retained hold prevents them from issuing new raw transactions.
    assign ready=phase==FROZEN && (owner_hold===1'b1) && (guest_held===1'b1) &&
                 (frontends_frozen===1'b1) && (arbiter_idle===1'b1) &&
                 (raw_service_idle===1'b1) && (abort===1'b0);
    always @(posedge clk)begin
        if(machine_reseed)phase<=RUN;
        else if(abort && (owner_hold || phase!=RUN))phase<=QUARANTINE;
        else case(phase)
            RUN:if(owner_hold)phase<=DRAIN;
            DRAIN:if(!owner_hold)phase<=RUN;
                  else if(drained)phase<=FROZEN;
            FROZEN:if(!owner_hold)phase<=RUN;
                   else if(guest_held!==1'b1 || arbiter_idle!==1'b1 ||
                           raw_service_idle!==1'b1)phase<=QUARANTINE;
            QUARANTINE:phase<=QUARANTINE;
            default:phase<=QUARANTINE;
        endcase
    end
endmodule
`default_nettype wire
