// Independent DDR memory peer and word-level scoreboard. The bus counters
// check useful-lane reuse; the scoreboard does not model the cache/hash.
`timescale 1ns/1ps
module tb_storage_linecache;
    parameter integer CACHE_BITS=3;
    reg clk=0;always #1 clk=~clk;
    reg reset=0,valid=0,wr=0,ready=0;
    reg [7:0] address=0;
    reg [15:0] word_data=0;
    wire accepted,response;
    wire [15:0] result;
    wire [28:0] da;
    wire [7:0] be,burst;
    wire rd,dw;
    wire [63:0] wd;
    reg [63:0] mem [0:63];
    reg [15:0] expected [0:255];
    reg force_busy=0,zero_latency=0;
    reg [15:0] rng=16'hace1;
    reg pending=0;
    integer countdown=0,read_delay=5;
    reg [63:0] pending_data=0;
    wire busy=force_busy || rng[2:0]==0;
    wire immediate=zero_latency && rd && !busy;
    wire rvalid=immediate || (pending && countdown==0);
    wire [63:0] rdata=immediate ? mem[da-29'h6000000] : pending_data;
    integer reads=0,writes=0,transactions=0;
    dorado_storage_ddram #(.ADDR_BITS(8),.LINE_CACHE_BITS(CACHE_BITS),.GROUP_ADDR_BITS(4)) d(
        .clk(clk),.reset(reset),.req_valid(valid),.req_ready(accepted),
        .req_write(wr),.req_addr(address),.req_data(word_data),
        .rsp_valid(response),.rsp_ready(ready),.rsp_data(result),
        .ddram_busy(busy),.ddram_burstcount(burst),.ddram_addr(da),
        .ddram_read(rd),.ddram_write(dw),.ddram_wdata(wd),.ddram_be(be),
        .ddram_rdata(rdata),.ddram_rvalid(rvalid));
    always @(posedge clk)begin
        rng<={rng[14:0],rng[15]^rng[13]^rng[12]^rng[10]};
        if(pending)begin
            if(countdown==0)pending<=0;
            else countdown<=countdown-1;
        end
        if(!busy && (rd || dw))begin
            if(rd && dw || burst!=1 || da<29'h6000000 || da>=29'h6000040)
                $fatal(1,"invalid DDR command/window");
            if(dw)begin
                for(integer lane=0;lane<8;lane++)if(be[lane])
                    mem[da-29'h6000000][lane*8+:8]<=wd[lane*8+:8];
                writes<=writes+1;
            end else begin
                if(pending)$fatal(1,"multiple DDR reads outstanding");
                reads<=reads+1;
                if(!zero_latency)begin
                    pending<=1;countdown<=read_delay;
                    pending_data<=mem[da-29'h6000000];
                end
            end
        end
    end
    task tick;@(negedge clk);endtask
    task request(input bit write_op,input [7:0] a,input [15:0] v);
        while(!accepted)tick();
        wr=write_op;address=a;word_data=v;valid=1;
        tick();valid=0;address=~a;word_data=~v;wr=~write_op;
    endtask
    task finish_request(input bit write_op,input [7:0] a,input [15:0] v);
        integer watchdog;
        watchdog=0;
        while(!response)begin
            tick();watchdog++;
            if(watchdog>100)$fatal(1,"request timed out addr=%h state=%0d",a,d.state);
        end
        if(!write_op && result!==expected[a])
            $fatal(1,"word scoreboard addr=%h got=%h expected=%h",a,result,expected[a]);
        if(write_op)expected[a]=v;
        begin
            reg [15:0] held;
            held=result;
            repeat(3)begin
                tick();
                if(!response || accepted || result!==held || rd || dw)
                    $fatal(1,"response/backpressure contract");
            end
        end
        ready=1;tick();ready=0;transactions++;
    endtask
    task access_word(input bit write_op,input [7:0] a,input [15:0] v);
        request(write_op,a,v);finish_request(write_op,a,v);
    endtask
    initial begin
        for(integer i=0;i<256;i++)expected[i]=16'(i*257+16'h1234);
        for(integer i=0;i<64;i++)for(integer lane=0;lane<4;lane++)
            mem[i][lane*16+:16]=expected[4*i+lane];
        tick();
        // All eight concurrent chip groups survive interleaved accesses to
        // the four lanes. Only eight64-bit reads are necessary, not32.
        for(integer lane=0;lane<4;lane++)for(integer group_id=0;group_id<8;group_id++)
            access_word(0,8'(16*group_id+lane),0);
        if(reads!=8)$fatal(1,"discarded useful DDR lanes: reads=%0d expected8",reads);
        // A proven hit must complete even when DDR cannot accept anything.
        force_busy=1;access_word(0,8'd1,0);force_busy=0;
        // Each byte-lane write must reach DDR and invalidate old line data.
        for(integer lane=0;lane<4;lane++)begin
            force_busy=1;request(1,8'(lane),16'(16'h9000+lane));
            repeat(2)tick();
            repeat(5)begin
                if(!dw || rd || response || da!=29'h6000000 ||
                   be!=(8'h03<<(2*lane)) || wd!={4{16'(16'h9000+lane)}})
                    $fatal(1,"write acknowledged early or changed under DDR stall");
                tick();
            end
            force_busy=0;finish_request(1,8'(lane),16'(16'h9000+lane));
            for(integer other=0;other<4;other++)access_word(0,8'(other),0);
        end
        if(writes!=4)$fatal(1,"write was hidden or duplicated");
        // Eviction/collision with a different full tag must not return an
        // older word. The scoreboard has no knowledge of index selection.
        access_word(0,8'd128,0);access_word(0,8'd0,0);
        for(integer i=0;i<400;i++)begin
            read_delay=(i%9)+1;zero_latency=(i%7)==0;
            access_word((i%4)==0,8'((i*73)^(i>>2)),16'(i*971));
        end
        // Reset invalidates previously filled data; change backing memory
        // while reset is asserted, as a new seed/initialization may do.
        access_word(0,8'd0,0);reset=1;tick();
        mem[0][15:0]=16'hbaba;expected[0]=16'hbaba;
        reset=0;access_word(0,8'd0,0);
        // An issued miss must drain through reset without acknowledging it
        // or warming the new cache with an orphan's old data.
        zero_latency=0;read_delay=12;
        request(0,8'd236,0);
        while(!pending)tick();
        reset=1;tick();reset=0;
        while(pending)begin
            if(accepted || response)$fatal(1,"orphan fabricated completion");
            tick();
        end
        repeat(3)tick();
        if(!accepted || response)$fatal(1,"orphan not drained");
        mem[59][15:0]=16'hdcba;expected[236]=16'hdcba;
        access_word(0,8'd236,0);
        for(integer i=0;i<256;i++)if(mem[i>>2][(i%4)*16+:16]!==expected[i])
            $fatal(1,"backing memory corruption addr=%0d",i);
        $display("STORAGE-LINECACHE PASS: %0d transactions;8 fills for32 interleaved lanes, writes/coherence/collisions/backpressure/reset drain; reads=%0d writes=%0d",transactions,reads,writes);
        $finish;
    end
    initial begin repeat(40000)tick();$fatal(1,"timeout");end
endmodule
