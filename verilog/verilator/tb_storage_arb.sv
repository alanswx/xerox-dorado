// Independent cyclic-scan scoreboard for the balanced request/payload tree.
`timescale 1ns/1ps
`default_nettype none
module tb_storage_arb #(parameter integer GROUPS=128);
    localparam A=8,R=24;
    reg clk=0;always #1 clk=~clk;
    reg reset=1;
    reg [GROUPS-1:0] valid=0,wr=0,accept_response=0;
    wire [GROUPS-1:0] accepted,response;
    reg [GROUPS*A-1:0] addr=0;
    reg [GROUPS*16-1:0] data=0;
    wire [GROUPS*16-1:0] replies;
    wire raw_valid,raw_wr,raw_accept_response;
    reg raw_ready=0,raw_response=0;
    wire [R-1:0] raw_addr;
    wire [15:0] raw_data;
    reg [15:0] reply_data=0;
    dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(A),.RAW_ADDR_BITS(R)) d(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(wr),.req_ready(accepted),
        .req_addr(addr),.req_data(data),.rsp_valid(response),.rsp_ready(accept_response),.rsp_data(replies),
        .raw_req_valid(raw_valid),.raw_req_write(raw_wr),.raw_req_ready(raw_ready),
        .raw_req_addr(raw_addr),.raw_req_data(raw_data),.raw_rsp_valid(raw_response),
        .raw_rsp_ready(raw_accept_response),.raw_rsp_data(reply_data));
    integer next_owner=0,transactions=0,masks=0,request_stalls=0,response_stalls=0;
    reg [31:0] random_state=32'h7be91d43;
    task tick;@(negedge clk);endtask
    // Behavioral golden: walk from the remembered next pointer, modulo G.
    // This does not use the candidate's tree, preferred flags or payload mux.
    function automatic integer expected_owner(input [GROUPS-1:0] pending,input integer start);
        integer candidate;
        begin
            expected_owner=-1;
            for(integer offset=0;offset<GROUPS;offset++)begin
                candidate=(start+offset)%GROUPS;
                if(expected_owner<0 && pending[candidate])expected_owner=candidate;
            end
        end
    endfunction
    task serve(input [GROUPS-1:0] mask);
        integer owner;
        reg [R-1:0] expected_address;
        reg [15:0] expected_data;
        reg expected_write;
        reg [GROUPS-1:0] only_owner;
        begin
            valid=mask;masks++;
            while(valid!='0)begin
                owner=expected_owner(valid,next_owner);
                only_owner='0;only_owner[owner]=1;
                expected_address=(R'(owner)<<A)|addr[owner*A+:A];
                expected_data=data[owner*16+:16];expected_write=wr[owner];
                wait(raw_valid);
                // Request and ownership must survive an arbitrarily stalled
                // DDR port, independently of changing response inputs.
                repeat(1+(transactions%4))begin
                    tick();
                    if(!raw_valid || raw_addr!==expected_address || raw_data!==expected_data ||
                       raw_wr!==expected_write || accepted!='0 || response!='0)
                        $fatal(1,"request/order/payload changed G=%0d owner=%0d addr=%h/%h",GROUPS,owner,raw_addr,expected_address);
                    request_stalls++;
                end
                raw_ready=1;#0.1;
                if(accepted!==only_owner)$fatal(1,"request accepted by wrong group");
                tick();raw_ready=0;valid[owner]=0;
                // Only the accepted producer may change its payload now.
                addr[owner*A+:A]=~addr[owner*A+:A];data[owner*16+:16]=~data[owner*16+:16];
                wr[owner]=~wr[owner];
                reply_data=16'(transactions^16'h8a51);
                repeat(1+(transactions%3))begin
                    tick();
                    if(raw_valid || raw_addr!==expected_address || raw_data!==expected_data || response!='0)
                        $fatal(1,"owner/payload changed while response delayed");
                end
                raw_response=1;#0.1;
                repeat(3)begin
                    if(response!==only_owner || raw_accept_response || replies[owner*16+:16]!==reply_data)
                        $fatal(1,"response ownership/backpressure failed");
                    response_stalls++;tick();
                end
                accept_response=only_owner;#0.1;
                if(!raw_accept_response)$fatal(1,"response acceptance not forwarded");
                tick();raw_response=0;accept_response='0;
                next_owner=owner==GROUPS-1 ? 0 : owner+1;transactions++;
            end
            repeat(2)tick();
            if(raw_valid || accepted!='0 || response!='0)$fatal(1,"empty queue generated traffic");
        end
    endtask
    reg [GROUPS-1:0] mask,prime;
    initial begin
        repeat(3)tick();reset=0;
        for(integer group=0;group<GROUPS;group++)begin
            addr[group*A+:A]=A'(group*17+9);
            data[group*16+:16]=16'(group*523+71);wr[group]=group[0];
        end
        // Establish every pointer through real transactions, without forcing
        // any internal register. Non-power-of-two padding must never win.
        for(integer pointer=0;pointer<GROUPS;pointer++)begin
            if(GROUPS<=8)begin
                for(integer pattern=0;pattern<(1<<GROUPS);pattern++)begin
                    prime='0;prime[(pointer+GROUPS-1)%GROUPS]=1;
                    serve(prime);
                    if(next_owner!=pointer)$fatal(1,"pointer priming failed");
                    serve(GROUPS'(pattern));
                end
            end else begin
                prime='0;prime[(pointer+GROUPS-1)%GROUPS]=1;serve(prime);
                mask='0;
                for(integer bit_number=0;bit_number<GROUPS;bit_number++)begin
                    random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
                    mask[bit_number]=random_state[0];
                end
                serve(mask);
            end
        end
        if(transactions<GROUPS || request_stalls<transactions || response_stalls<transactions)
            $fatal(1,"insufficient arbitration coverage");
        $display("STORAGE-ARB PASS groups=%0d masks=%0d transactions=%0d request_stalls=%0d response_stalls=%0d",
            GROUPS,masks,transactions,request_stalls,response_stalls);
        $finish;
    end
    initial begin #10000000;$fatal(1,"arbiter timeout G=%0d",GROUPS);end
endmodule
`default_nettype wire
