// Independent cyclic-scan scoreboard for the balanced request/payload tree.
`timescale 1ns/1ps
`default_nettype none
module tb_storage_arb #(parameter integer GROUPS=128,PREFETCH_NEXT=0,INTERLEAVE_GROUPS=0,
    GROUP_ADDR_BITS=8,RAW_ADDR_BITS=24);
    localparam A=GROUP_ADDR_BITS,R=RAW_ADDR_BITS;
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
    dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(A),.RAW_ADDR_BITS(R),
        .PREFETCH_NEXT(PREFETCH_NEXT),.INTERLEAVE_GROUPS(INTERLEAVE_GROUPS)) d(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(wr),.req_ready(accepted),
        .req_addr(addr),.req_data(data),.rsp_valid(response),.rsp_ready(accept_response),.rsp_data(replies),
        .raw_req_valid(raw_valid),.raw_req_write(raw_wr),.raw_req_ready(raw_ready),
        .raw_req_addr(raw_addr),.raw_req_data(raw_data),.raw_rsp_valid(raw_response),
        .raw_rsp_ready(raw_accept_response),.raw_rsp_data(reply_data));
    integer next_owner=0,transactions=0,masks=0,request_stalls=0,response_stalls=0;
    integer host_cycles=0,reset_cases=0;
    always @(posedge clk)host_cycles<=host_cycles+1;
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
    task serve(input [GROUPS-1:0] mask,input [GROUPS-1:0] late_mask='0);
        integer owner;
        reg [R-1:0] expected_address;
        reg [15:0] expected_data;
        reg expected_write;
        reg [GROUPS-1:0] only_owner;
        reg [GROUPS-1:0] late_pending;
        begin
            valid=mask;late_pending=late_mask;masks++;
            while(valid!='0)begin
                owner=expected_owner(valid,next_owner);
                only_owner='0;only_owner[owner]=1;
                expected_address=INTERLEAVE_GROUPS ?
                    R'(int'(addr[owner*A+:A])*(2**$clog2(GROUPS))+owner) :
                    R'(owner*(2**A)+int'(addr[owner*A+:A]));
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
                // New producers, including the previous owner, may offer
                // requests while this response is outstanding. The golden
                // cyclic scan must still start AFTER the completed owner.
                valid=valid|late_pending;late_pending='0;
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
    task cancel_at_reset(input integer stage);
        reg [GROUPS-1:0] one;
        begin
            one='0;one[GROUPS-1]=1;serve(one);
            one='0;one[0]=1;
            valid='1;raw_ready=0;raw_response=0;accept_response=0;
            wait(raw_valid);
            tick(); // drive acceptance before a full following rising edge
            if(stage>0)begin
                raw_ready=1;tick();raw_ready=0;valid[0]=0;
                raw_response=1;
                if(stage>1)begin
                    // Consume the response with another group pending;
                    // the optimized implementation has selected its next
                    // request, but reset must cancel that selection too.
                    accept_response=one;#0.1;
                    if(!raw_accept_response)$fatal(1,"reset case did not consume its response");
                    tick();accept_response=0;
                    if(PREFETCH_NEXT && GROUPS>1 && !raw_valid)
                        $fatal(1,"pending successor did not avoid idle bubble");
                end
            end
            reset=1;#0.1;
            if(raw_valid || raw_accept_response || accepted!='0 || response!='0)
                $fatal(1,"traffic escaped reset stage=%0d",stage);
            repeat(2)tick();valid=0;raw_response=0;reset=0;next_owner=0;
            repeat(2)tick();
            if(raw_valid || accepted!='0 || response!='0)$fatal(1,"stale request after reset");
            reset_cases++;
            serve(one);
        end
    endtask
    reg [GROUPS-1:0] mask,prime,late_mask;
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
        // Force an origin of1, select the last group, then introduce both
        // 0 and1 while its response is pending. A stale selection origin
        // incorrectly chooses1; the independent golden requires0 first.
        if(GROUPS>2)begin
            prime='0;prime[0]=1;serve(prime);
            mask='0;mask[GROUPS-1]=1;
            late_mask='0;late_mask[0]=1;late_mask[GROUPS>1 ? 1 : 0]=1;
            serve(mask,late_mask);
        end
        // Re-offer the accepted producer with new payload while waiting.
        mask='0;mask[GROUPS-1]=1;
        late_mask='0;late_mask[0]=1;late_mask[GROUPS-1]=1;
        serve(mask,late_mask);
        for(integer stage=0;stage<3;stage++)cancel_at_reset(stage);
        if(transactions<GROUPS || request_stalls<transactions || response_stalls<transactions)
            $fatal(1,"insufficient arbitration coverage");
        $display("STORAGE-ARB PASS groups=%0d prefetch=%0d masks=%0d transactions=%0d request_stalls=%0d response_stalls=%0d resets=%0d host_cycles=%0d interleave=%0d",
            GROUPS,PREFETCH_NEXT,masks,transactions,request_stalls,response_stalls,reset_cases,host_cycles,INTERLEAVE_GROUPS);
        $finish;
    end
    initial begin #10000000;$fatal(1,"arbiter timeout G=%0d",GROUPS);end
endmodule
`default_nettype wire
