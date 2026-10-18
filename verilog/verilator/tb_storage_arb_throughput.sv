// Independent fixed-latency backend: all request payloads and reply owners
// follow a prescribed round robin stream. Count cycles only after correctness
// checks pass. No artificial producer stalls hide the response-boundary bubble.
`timescale 1ns/1ps
`default_nettype none
module storage_arb_stream #(
    parameter integer GROUPS=3,PREFETCH_NEXT=0,PER_GROUP=32
)(input wire clk,reset,output reg finished=0,
  output integer host_cycles=0,output integer received=0);
    localparam A=8,R=24;
    reg [31:0] sequence_number[0:GROUPS-1];
    wire [GROUPS-1:0] valid,wr,accepted,response;
    wire [GROUPS*A-1:0] addr;
    wire [GROUPS*16-1:0] data,replies;
    for(genvar g=0;g<GROUPS;g++)begin:producer
        assign valid[g]=sequence_number[g]<PER_GROUP;
        assign addr[g*A+:A]=A'(sequence_number[g]*7+32'(g));
        assign data[g*16+:16]=16'(16'h9000+g*32+sequence_number[g]);
        assign wr[g]=sequence_number[g][0];
    end
    wire raw_valid,raw_wr,raw_accept_response;
    wire [R-1:0] raw_addr;
    wire [15:0] raw_data;
    reg busy=0;
    reg [2:0] delay_count=0;
    reg [15:0] reply_data=0;
    wire raw_ready=!busy;
    wire raw_response=busy && delay_count==0;
    dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(A),.RAW_ADDR_BITS(R),
        .PREFETCH_NEXT(PREFETCH_NEXT)) d(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(wr),.req_ready(accepted),
        .req_addr(addr),.req_data(data),.rsp_valid(response),.rsp_ready({GROUPS{1'b1}}),.rsp_data(replies),
        .raw_req_valid(raw_valid),.raw_req_write(raw_wr),.raw_req_ready(raw_ready),
        .raw_req_addr(raw_addr),.raw_req_data(raw_data),.raw_rsp_valid(raw_response),
        .raw_rsp_ready(raw_accept_response),.raw_rsp_data(reply_data));
    integer sent=0;
    integer expected_owner,expected_sequence;
    reg [GROUPS-1:0] only_owner;
    reg [R-1:0] expected_address;
    reg [15:0] expected_data;
    always @(posedge clk)begin
        if(reset)begin
            host_cycles<=0;received<=0;sent<=0;busy<=0;finished<=0;
            for(integer g=0;g<GROUPS;g++)sequence_number[g]<=0;
        end else if(!finished)begin
            host_cycles<=host_cycles+1;
            for(integer g=0;g<GROUPS;g++)
                if(accepted[g] && valid[g])sequence_number[g]<=sequence_number[g]+1;
            if(raw_valid && raw_ready)begin
                expected_owner=sent%GROUPS;expected_sequence=sent/GROUPS;
                expected_address=(R'(expected_owner)<<A)|R'((expected_sequence*7+expected_owner)&((1<<A)-1));
                expected_data=16'(16'h9000+expected_owner*32+expected_sequence);
                only_owner='0;only_owner[expected_owner]=1;
                if(raw_addr!==expected_address || raw_data!==expected_data ||
                   raw_wr!==expected_sequence[0] || accepted!==only_owner)
                    $fatal(1,"stream request/order/data mode=%0d sent=%0d addr=%h/%h data=%h/%h wr=%b/%b accepted=%b/%b",PREFETCH_NEXT,sent,raw_addr,expected_address,raw_data,expected_data,raw_wr,expected_sequence[0],accepted,only_owner);
                sent<=sent+1;busy<=1;delay_count<=2;
                reply_data<=16'(raw_addr)^raw_data;
            end else if(busy && delay_count!=0)delay_count<=delay_count-1'b1;
            else if(raw_response && raw_accept_response)begin
                expected_owner=received%GROUPS;expected_sequence=received/GROUPS;
                expected_address=(R'(expected_owner)<<A)|R'((expected_sequence*7+expected_owner)&((1<<A)-1));
                expected_data=16'(16'h9000+expected_owner*32+expected_sequence);
                only_owner='0;only_owner[expected_owner]=1;
                if(response!==only_owner || replies[expected_owner*16+:16]!==
                   (16'(expected_address)^expected_data))
                    $fatal(1,"stream response owner/data mode=%0d received=%0d",PREFETCH_NEXT,received);
                received<=received+1;busy<=0;
                if(received+1==GROUPS*PER_GROUP)begin
                    if(sent!=GROUPS*PER_GROUP)$fatal(1,"stream lost/duplicated request");
                    finished<=1;
                end
            end
        end
    end
endmodule
module tb_storage_arb_throughput #(parameter integer GROUPS=3);
    reg clk=0;always #1 clk=~clk;
    reg reset=1;
    wire baseline_done,prefetch_done;
    wire [31:0] baseline_cycles,prefetch_cycles,baseline_received,prefetch_received;
    storage_arb_stream #(.GROUPS(GROUPS),.PREFETCH_NEXT(0)) baseline(
        .clk(clk),.reset(reset),.finished(baseline_done),.host_cycles(baseline_cycles),.received(baseline_received));
    storage_arb_stream #(.GROUPS(GROUPS),.PREFETCH_NEXT(1)) prefetch(
        .clk(clk),.reset(reset),.finished(prefetch_done),.host_cycles(prefetch_cycles),.received(prefetch_received));
    initial begin
        repeat(3)@(negedge clk);reset=0;
        wait(baseline_done && prefetch_done);@(negedge clk);
        if(baseline_received!=GROUPS*32 || prefetch_received!=baseline_received)
            $fatal(1,"stream incomplete");
        if(baseline_cycles-prefetch_cycles!=GROUPS*32-1)
            $fatal(1,"expected one saved bubble per waiting successor: cycles=%0d/%0d",
                baseline_cycles,prefetch_cycles);
        $display("STORAGE-ARB-THROUGHPUT PASS groups=%0d requests=%0d baseline_cycles=%0d prefetch_cycles=%0d saved=%0d; synthetic fixed-latency backend, NOT FPGA throughput",
            GROUPS,baseline_received,baseline_cycles,prefetch_cycles,baseline_cycles-prefetch_cycles);
        $finish;
    end
    initial begin #1000000;$fatal(1,"stream deadline");end
endmodule
`default_nettype wire
