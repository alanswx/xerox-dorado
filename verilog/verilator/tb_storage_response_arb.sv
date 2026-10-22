// Independent interface/round-robin scoreboard, including simultaneous retire/
// accept, consumer backpressure and full native128-group address/data routing.
`timescale 1ns/1ps
module response_arb_channel #(parameter GROUPS=128, OVERLAP=0)(
    input wire clk,reset,start,
    input wire [7:0] wave,
    output wire done
);
    localparam ID_BITS=$clog2(GROUPS), RAW_BITS=16+ID_BITS;
    reg [GROUPS-1:0] need=0,waiting=0,completed=0;
    wire [GROUPS-1:0] accept,response;
    reg [GROUPS-1:0] consumer_ready;
    wire [GROUPS*16-1:0] addresses,data,replies;
    wire [GROUPS-1:0] writes;
    wire rv,rr,rw,sv,sr;
    wire [RAW_BITS-1:0] ra;
    wire [15:0] wd,sd;
    integer ticks=0,accepted=0,returned=0,origin=0;
    integer overlaps=0,request_stalls=0,response_stalls=0;
    reg pending=0;
    integer latency=0;
    reg [15:0] result=0;
    reg held=0;
    reg [RAW_BITS+16:0] held_request;
    function automatic [15:0] address_for(input integer g,input [7:0] w);
        address_for=16'(int'(w)*97+g*19)^16'h9137;
    endfunction
    function automatic [15:0] data_for(input integer g,input [7:0] w);
        data_for=16'(int'(w)*131+g*37)^16'ha56c;
    endfunction
    function automatic [15:0] reply_for(input integer g,input [7:0] w);
        reply_for=address_for(g,w)^data_for(g,w)^(((int'(w)+g)&1) ? 16'hb00b : 16'hd00d);
    endfunction
    function automatic bit active(input integer g);
        active=(wave%4==0) || (wave%4==1 && (g*17+int'(wave)*11)%7!=0) ||
            (wave%4==2 && (g==0 || g==GROUPS-1)) || (wave%4==3 && g%3==1);
    endfunction
    genvar g;
    generate for(g=0;g<GROUPS;g++)begin
        // Explicit wave arguments also expose continuous-assignment
        // sensitivity to Icarus; a function's hidden global does not.
        assign addresses[16*g+:16]=address_for(g,wave);
        assign data[16*g+:16]=data_for(g,wave);
        assign writes[g]=(int'(wave)+g)&1;
    end endgenerate
    always @*for(integer n=0;n<GROUPS;n++)consumer_ready[n]=(ticks+n)%9<6;
    assign done=&completed;
    assign sv=pending && latency==0;
    assign sd=result;
    assign rr=(!pending || (sv && sr)) && ticks%7!=3 && !reset;
    dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(RAW_BITS),
        .PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(OVERLAP)) dut(
        .clk(clk),.reset(reset),.req_valid(need),.req_write(writes),.req_ready(accept),
        .req_addr(addresses),.req_data(data),.rsp_valid(response),.rsp_ready(consumer_ready),.rsp_data(replies),
        .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
    always @(posedge clk)begin
        if(reset)begin
            ticks<=0;pending<=0;latency<=0;need<=0;waiting<=0;completed<=0;
            accepted=0;returned=0;origin=0;held=0;
            overlaps=0;request_stalls=0;response_stalls=0;
        end else begin
            // Handshake controls use the preceding edge's tick throughout
            // this sampling region, independent of simulator process order.
            ticks<=ticks+1;
            if(start)begin
                if(pending || (|waiting) || (|need))$fatal(1,"ARB start before drain");
                for(integer n=0;n<GROUPS;n++)begin need[n]<=active(n);completed[n]<=!active(n);end
            end
            if(pending && latency>0)latency<=latency-1;
            if(sv && sr)begin pending<=0;returned=returned+1;end
            if(held && (!rv || {ra,wd,rw}!==held_request))
                $fatal(1,"ARB request changed under backpressure");
            held=rv && !rr;held_request={ra,wd,rw};
            if(rv && !rr)request_stalls=request_stalls+1;
            if(sv && !sr)response_stalls=response_stalls+1;
            if(rv && rr && sv && sr)overlaps=overlaps+1;
            if(rv && rr)begin : raw_accept
                integer owner,expected_owner;
                owner=int'(ra)&(GROUPS-1);expected_owner=-1;
                for(integer offset=0;offset<GROUPS;offset++)
                    if(expected_owner<0 && need[(origin+offset)%GROUPS])expected_owner=(origin+offset)%GROUPS;
                if(owner!=expected_owner || !need[owner])$fatal(1,"ARB fairness/owner mismatch");
                if((ra>>ID_BITS)!==RAW_BITS'(address_for(owner,wave)) ||
                   wd!==data_for(owner,wave) || rw!==writes[owner])$fatal(1,"ARB payload mismatch");
                if(!accept[owner])$fatal(1,"ARB raw acceptance without group acceptance");
                pending<=1;latency<=accepted%4;result<=reply_for(owner,wave);
                accepted=accepted+1;origin=(owner+1)%GROUPS;
            end
            if(accepted-returned<0 || accepted-returned>1)$fatal(1,"ARB multiple outstanding");
            for(integer n=0;n<GROUPS;n++)begin
                if(need[n] && accept[n])begin
                    if(waiting[n])$fatal(1,"ARB duplicate group acceptance");
                    need[n]<=0;waiting[n]<=1;
                end
                if(response[n] && consumer_ready[n])begin
                    if(!waiting[n] || replies[16*n+:16]!==reply_for(n,wave))
                        $fatal(1,"ARB response data/owner mismatch");
                    waiting[n]<=0;completed[n]<=1;
                end
            end
        end
    end
endmodule

module tb_storage_response_arb;
    parameter GROUPS=128;
    reg clk=0;always #5 clk=~clk;
    reg reset=1,start=0;
    reg [7:0] wave=0;
    wire [1:0] done;
    response_arb_channel #(.GROUPS(GROUPS),.OVERLAP(0)) old_path(clk,reset,start,wave,done[0]);
    response_arb_channel #(.GROUPS(GROUPS),.OVERLAP(1)) new_path(clk,reset,start,wave,done[1]);
    initial begin
        repeat(4)@(negedge clk);reset=0;
        for(integer w=0;w<16;w++)begin
            wave=8'(w);start=1;@(negedge clk);start=0;
            while(done!==2'b11)@(negedge clk);
            repeat(2)@(negedge clk);
        end
        if(old_path.accepted!=old_path.returned || new_path.accepted!=new_path.returned ||
           old_path.accepted!=new_path.accepted || old_path.accepted<1000)
            $fatal(1,"ARB incomplete/mismatched totals");
        if(old_path.overlaps!=0 || new_path.overlaps<100 ||
           old_path.request_stalls==0 || new_path.request_stalls==0 ||
           old_path.response_stalls==0 || new_path.response_stalls==0)
            $fatal(1,"ARB overlap/backpressure coverage missing");
        $display("RESPONSE_ARBITER PASS groups=%0d waves=16 old_accept=%0d new_accept=%0d",GROUPS,old_path.accepted,new_path.accepted);
        $display("RESPONSE_ARBITER coverage old_overlap=%0d new_overlap=%0d request_stalls=%0d/%0d response_stalls=%0d/%0d",
            old_path.overlaps,new_path.overlaps,old_path.request_stalls,new_path.request_stalls,
            old_path.response_stalls,new_path.response_stalls);
        $finish;
    end
    initial begin #5000000;$fatal(1,"ARB test deadline");end
endmodule
