// Passive counters, in FABRIC EDGE units. These are not instructions.
// machine_step is the falling-edge gate decision shared with the cable;
// each asserted sample at a host rising edge permits one machine rising edge.
`default_nettype none
module dorado_speed_monitor #(
    parameter integer BITS=32,
    parameter integer TRACK_CAUSES=0
) (
    input wire clk,reset,machine_step,
    input wire reset_requested,pack_ready,storage_ready,storage_initialized,
    input wire [8:0] storage_events,
    input wire [2:0] pack_host_phase,
    input wire [31:0] pack_reads,pack_writes,
    output wire [24*BITS-1:0] performance,
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
    // Match altclkctrl/network_step's falling-edge decision. Sampling the
    // raw readiness at the following rising edge can misattribute a pause.
    genvar exported_count;
    generate if(TRACK_CAUSES)begin:causes
        reg reset_q=1,pack_q=0,storage_q=0,initialized_q=0;
        always @(negedge clk)begin
            reset_q<=reset_requested;pack_q<=pack_ready;
            storage_q<=storage_ready;initialized_q<=storage_initialized;
        end
        reg [BITS-1:0] count[2:21];
        reg [BITS-1:0] ddr_length=0,read_length=0,write_length=0;
        reg read_pending=0;
        for(exported_count=2;exported_count<22;exported_count=exported_count+1)begin:export_count
            initial count[exported_count]=0;
            assign performance[exported_count*BITS+:BITS]=count[exported_count];
        end
        assign performance[0*BITS+:BITS]=host_edges;
        assign performance[1*BITS+:BITS]=machine_edges;
        assign performance[22*BITS+:BITS]=BITS'(pack_reads);
        assign performance[23*BITS+:BITS]=BITS'(pack_writes);
        always @(posedge clk)begin
            if(reset)begin
                for(integer n=2;n<22;n=n+1)count[n]<=0;
                ddr_length<=0;read_length<=0;write_length<=0;read_pending<=0;
            end else begin
                // 1(enabled),2(reset),3(pack only),4(DDR only),5(both)
                // partition every host edge. Counter6 is an init subset.
                if(!machine_step)begin
                    if(reset_q)count[2]<=count[2]+1'b1;
                    else if(!pack_q && !storage_q)count[5]<=count[5]+1'b1;
                    else if(!pack_q)count[3]<=count[3]+1'b1;
                    else if(!storage_q)count[4]<=count[4]+1'b1;
                end
                if(!initialized_q && !machine_step)count[6]<=count[6]+1'b1;
                if(storage_events[0])count[7]<=count[7]+1'b1;
                if(storage_events[1])count[8]<=count[8]+1'b1;
                if(storage_events[2])count[9]<=count[9]+1'b1;
                if(storage_events[3])count[10]<=count[10]+1'b1;
                if(storage_events[8])count[11]<=count[11]+1'b1;
                if(storage_events[5])count[12]<=count[12]+1'b1;
                if(read_pending)count[13]<=count[13]+1'b1;
                if(storage_events[4])read_pending<=0;
                else if(storage_events[2])read_pending<=1;
                if(storage_events[7])count[14]<=count[14]+1'b1;
                if(pack_host_phase==3)count[15]<=count[15]+1'b1;
                if(pack_host_phase==4)count[16]<=count[16]+1'b1;
                if(pack_host_phase==1)count[17]<=count[17]+1'b1;
                if(pack_host_phase==2)count[18]<=count[18]+1'b1;
                if(!reset_q && !storage_q)begin
                    ddr_length<=ddr_length+1'b1;
                    if(ddr_length+1'b1>count[19])count[19]<=ddr_length+1'b1;
                end else ddr_length<=0;
                if(pack_host_phase==3 || pack_host_phase==4)begin
                    read_length<=read_length+1'b1;
                    if(read_length+1'b1>count[20])count[20]<=read_length+1'b1;
                end else read_length<=0;
                if(pack_host_phase==1 || pack_host_phase==2)begin
                    write_length<=write_length+1'b1;
                    if(write_length+1'b1>count[21])count[21]<=write_length+1'b1;
                end else write_length<=0;
            end
        end
    end else begin:no_causes
        assign performance='0;
    end endgenerate
endmodule
`default_nettype wire
