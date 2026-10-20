// Keep every guest/memory consumer held while the disk's host latch releases.
// Only the actual HPS medium acknowledges drain; Main's image lease is a
// separate permission. Device reset/revocation cannot release quarantined state.
`default_nettype none
module dorado_disk_pause_barrier (
    input wire clk,machine_reseed,owner_hold,guest_held,abort,device_reset,
    input wire device_idle,device_held,device_fault,
    output wire hold,device_hold,ready,fault
);
    localparam RUN=3'd0,DRAIN=3'd1,FROZEN=3'd2,RELEASE=3'd3,QUARANTINE=3'd4;
    // Three REQUIRED wrapper implementation bits, included by native inventory.
    reg [2:0] phase=RUN;
    wire requested=(owner_hold!==1'b0);
    wire drained=(guest_held===1'b1) && (device_idle===1'b1) && (device_held===1'b1);
    wire unhealthy=(abort!==1'b0) || (device_reset!==1'b0) || (device_fault!==1'b0);
    assign hold=requested || phase!=RUN;
    assign device_hold=requested || (phase!=RUN && phase!=RELEASE);
    assign fault=phase==QUARANTINE;
    assign ready=phase==FROZEN && (owner_hold===1'b1) && drained && !unhealthy;
    always @(posedge clk)begin
        if(machine_reseed)phase<=RUN;
        else if((requested || phase!=RUN) && unhealthy)phase<=QUARANTINE;
        else case(phase)
            RUN:if(requested)phase<=DRAIN;
            DRAIN:if(owner_hold===1'b0)phase<=RELEASE;
                  else if(owner_hold===1'b1 && drained)phase<=FROZEN;
            FROZEN:if(!drained || owner_hold!==1'b0 && owner_hold!==1'b1)phase<=QUARANTINE;
                   else if(owner_hold===1'b0)phase<=RELEASE;
            RELEASE:if(requested)phase<=DRAIN;
                    else if(device_held===1'b0)phase<=RUN;
            QUARANTINE:phase<=QUARANTINE;
            default:phase<=QUARANTINE;
        endcase
    end
endmodule
`default_nettype wire
