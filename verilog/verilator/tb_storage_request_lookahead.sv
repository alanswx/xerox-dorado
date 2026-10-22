// External protocol scoreboards. Offers occur between sampling edges; an
// unaccepted request retains its payload. No DUT state drives the oracle.
`timescale 1ns/1ps
module lookahead_arrivals #(parameter GROUPS=5)(input wire clk,output reg done=0);
    localparam ID_BITS=$clog2(GROUPS), RAW_BITS=8+ID_BITS;
    reg reset=1;
    reg [GROUPS-1:0] need=0,waiting=0,writes=0;
    reg [GROUPS*8-1:0] addresses=0;
    reg [GROUPS*16-1:0] data=0;
    wire [GROUPS-1:0] accepts,responses;
    wire [GROUPS*16-1:0] replies;
    reg hold_request=0,hold_response=0,fast=1;
    reg [GROUPS-1:0] consumers;
    wire rv,rr,rw,sv,sr;
    wire [RAW_BITS-1:0] ra;
    wire [15:0] wd;
    reg pending=0;
    reg [15:0] result=0;
    integer latency=0,ticks=0,accepted=0,returned=0,origin=0;
    integer overlaps=0,request_stalls=0,response_stalls=0;
    integer order[0:8191];
    integer age[0:GROUPS-1];
    reg [15:0] expected[0:GROUPS-1];
    reg held=0,held_response=0;
    reg [RAW_BITS+16:0] held_payload;
    reg [15:0] held_result;
    reg directed_pass=0,held_pass=0,reset_pass=0;

    assign sv=pending && latency==0 && !reset;
    assign rr=!reset && !hold_request && (!pending || (sv && sr)) &&
        (fast || ticks%7!=3);
    always @* for(integer g=0;g<GROUPS;g++)
        consumers[g]=!hold_response && (fast || (ticks+g)%9<6);
    dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(8),.RAW_ADDR_BITS(RAW_BITS),
        .PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)) dut(
        .clk(clk),.reset(reset),.req_valid(need),.req_write(writes),.req_ready(accepts),
        .req_addr(addresses),.req_data(data),.rsp_valid(responses),.rsp_ready(consumers),.rsp_data(replies),
        .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(result));

    function automatic [15:0] reply(input [7:0] address,input [15:0] value,input bit write_op);
        reply={address,address}^value^(write_op ? 16'h9137 : 16'ha56c);
    endfunction
    task automatic offer(input integer g,serial);
        if(need[g] || waiting[g])$fatal(1,"LOOK offer before previous completion");
        addresses[8*g+:8]=8'(g*19+serial*97);
        data[16*g+:16]=16'(g*37+serial*131)^16'hb00b;
        writes[g]=(g+serial)%2;
        expected[g]=reply(addresses[8*g+:8],data[16*g+:16],writes[g]);
        need[g]=1;
    endtask
    task automatic drain;
        while((|need) || (|waiting) || pending)@(negedge clk);
        repeat(2)@(negedge clk);
    endtask
    always @(posedge clk)begin
        if(reset)begin
            pending<=0;latency<=0;ticks<=0;need<=0;waiting<=0;
            accepted=0;returned=0;origin=0;held=0;held_response=0;
            overlaps=0;request_stalls=0;response_stalls=0;
            for(integer g=0;g<GROUPS;g++)age[g]=0;
        end else begin
            ticks<=ticks+1;
            if(pending && latency>0)latency<=latency-1;
            if(held && (!rv || {ra,wd,rw}!==held_payload))
                $fatal(1,"LOOK raw payload changed while held");
            held=rv && !rr;held_payload={ra,wd,rw};
            if(held_response && (!sv || result!==held_result))
                $fatal(1,"LOOK response changed while held");
            held_response=sv && !sr;held_result=result;
            if(rv && !rr)request_stalls++;
            if(sv && !sr)response_stalls++;
            if(sv && sr)begin pending<=0;returned++;end
            if(rv && rr)begin : take_raw
                integer owner,winner;
                owner=int'(ra)&((1<<ID_BITS)-1);winner=-1;
                if(owner>=GROUPS || !need[owner] || !accepts[owner])
                    $fatal(1,"LOOK raw acceptance without matching producer");
                if((ra>>ID_BITS)!==RAW_BITS'(addresses[8*owner+:8]) ||
                   wd!==data[16*owner+:16] || rw!==writes[owner])
                    $fatal(1,"LOOK accepted payload mismatch");
                if(accepts!==(GROUPS'(1)<<owner))
                    $fatal(1,"LOOK more than one producer accepted");
                if(sv && sr)begin
                    // A simultaneous successor commits on THIS edge, so its
                    // winner must account for every currently pending offer.
                    for(integer off=0;off<GROUPS;off++)
                        if(winner<0 && need[(origin+off)%GROUPS])winner=(origin+off)%GROUPS;
                    if(owner!=winner)$fatal(1,"LOOK strict overlap priority mismatch");
                    overlaps++;
                end
                order[accepted]=owner;accepted++;origin=(owner+1)%GROUPS;
                pending<=1;latency<=fast ? 0 : accepted%5;
                result<=expected[owner];
                for(integer g=0;g<GROUPS;g++)begin
                    if(need[g])age[g]++;else age[g]=0;
                    if(age[g]>2*GROUPS+4)$fatal(1,"LOOK bounded fairness violated");
                end
            end
            if(accepted-returned<0 || accepted-returned>1)
                $fatal(1,"LOOK more than one accepted raw operation outstanding");
            for(integer g=0;g<GROUPS;g++)begin
                if(need[g] && accepts[g])begin
                    if(waiting[g])$fatal(1,"LOOK duplicate producer acceptance");
                    need[g]<=0;waiting[g]<=1;
                end
                if(responses[g] && consumers[g])begin
                    if(!waiting[g] || replies[16*g+:16]!==expected[g])
                        $fatal(1,"LOOK response data or ownership mismatch");
                    waiting[g]<=0;
                end
            end
        end
    end
    initial begin : stimulus
        integer before_count,serial,g;
        repeat(4)@(negedge clk);reset=0;
        // Cache group2, then introduce group1 on the very edge that releases
        // group0's held response. A stale candidate must NOT bypass group1.
        hold_response=1;offer(0,1);offer(2,2);
        while(accepted<1)@(negedge clk);
        repeat(2)@(negedge clk);offer(1,3);hold_response=0;
        drain();
        if(accepted!=3 || order[0]!=0 || order[1]!=1 || order[2]!=2)
            $fatal(1,"LOOK late-arrival canonical order mismatch");
        directed_pass=1;

        // REQUEST is already committed while the backend is stalled. A new
        // higher-priority offer must not change that held request's payload.
        before_count=accepted;hold_request=1;offer(2,4);
        while(!rv)@(negedge clk);
        repeat(2)@(negedge clk);offer(1,5);
        repeat(3)@(negedge clk);hold_request=0;drain();
        if(order[before_count]!=2 || order[before_count+1]!=1)
            $fatal(1,"LOOK held request reservation changed");
        held_pass=1;

        // Reset during a held response with another unaccepted producer.
        hold_response=1;offer(0,6);offer(GROUPS-1,7);
        while(!sv)@(negedge clk);
        reset=1;repeat(3)@(negedge clk);
        if(rv || sr || (|responses) || (|accepts))$fatal(1,"LOOK reset did not quarantine interfaces");
        reset=0;hold_response=0;offer(GROUPS-1,99);drain();
        if(accepted!=1 || returned!=1 || order[0]!=GROUPS-1)
            $fatal(1,"LOOK stale operation after reset");
        reset_pass=1;

        // Different arrival schedules, backend latency, refresh-like ready
        // stalls, and consumer backpressure. Never change a live payload.
        fast=0;serial=100;
        for(integer i=0;i<2500;i++)begin
            @(negedge clk);g=(i*17+i/7)%GROUPS;
            if(!need[g] && !waiting[g] && i%3!=0)begin offer(g,serial);serial++;end
            g=(g+GROUPS/2)%GROUPS;
            if(!need[g] && !waiting[g] && i%11==0)begin offer(g,serial);serial++;end
        end
        drain();
        if(accepted!=returned || accepted<100 || overlaps<10 ||
           request_stalls==0 || response_stalls==0 || !directed_pass || !held_pass || !reset_pass)
            $fatal(1,"LOOK arrival/backpressure coverage missing");
        $display("LOOKAHEAD_ARRIVALS PASS groups=%0d accepted=%0d overlaps=%0d request_stalls=%0d response_stalls=%0d late_priority=1 held_request=1 reset=1",
            GROUPS,accepted,overlaps,request_stalls,response_stalls);
        done=1;
    end
endmodule

module tb_storage_request_lookahead;
    reg clk=0;always #5 clk=~clk;
    wire [1:0] done;
    lookahead_arrivals #(.GROUPS(5)) five_groups(clk,done[0]);
    lookahead_arrivals #(.GROUPS(128)) native(clk,done[1]);
    initial begin wait(&done);$display("LOOKAHEAD_DYNAMIC PASS");$finish;end
    initial begin #1000000;$fatal(1,"LOOK dynamic deadline");end
endmodule
