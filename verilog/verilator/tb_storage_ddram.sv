// Variable-latency Avalon peer, byte-lane preservation, backpressure and
// reset with a read in flight. No CPU, memory map or guest I/O is emulated.
`timescale 1ns/1ps
module tb_storage_ddram;
    reg clk=0;always #1 clk=~clk;
    reg reset=0,valid=0,wr=0,ready=0,busy=1,rvalid=0;
    reg [7:0] address=0;
    reg [15:0] data_word=0;
    wire accepted,response;
    wire [15:0] result;
    wire [28:0] da;
    wire [7:0] be,burst;
    wire rd,dw;
    wire [63:0] wd;
    reg [63:0] q=0;
    dorado_storage_ddram #(.ADDR_BITS(8)) d(
        .clk(clk),.reset(reset),.req_valid(valid),.req_ready(accepted),
        .req_write(wr),.req_addr(address),.req_data(data_word),
        .rsp_valid(response),.rsp_ready(ready),.rsp_data(result),
        .ddram_busy(busy),.ddram_burstcount(burst),.ddram_addr(da),
        .ddram_read(rd),.ddram_write(dw),.ddram_wdata(wd),.ddram_be(be),
        .ddram_rdata(q),.ddram_rvalid(rvalid));
    reg [63:0] mem [0:63];
    integer writes=0,reads=0;
    always @(posedge clk) if(!busy && (rd || dw)) begin
        if(burst!=1 || da<29'h6000000 || da>=29'h6000040)
            $fatal(1,"wrong DDR window/units/burst: %h",da);
        if(rd && dw)$fatal(1,"simultaneous read/write");
        if(dw)begin
            for(integer lane=0;lane<8;lane++)if(be[lane])
                mem[da-29'h6000000][lane*8+:8]<=wd[lane*8+:8];
            writes<=writes+1;
        end else reads<=reads+1;
    end
    task tick;@(negedge clk);endtask
    task request(input bit write_op,input [7:0] a,input [15:0] word_data);
        wait(accepted);wr=write_op;address=a;data_word=word_data;valid=1;
        tick();valid=0;
    endtask
    task read_result(input [15:0] expected);
        wait(response);
        if(result!==expected)$fatal(1,"wrong read data: %h wanted %h",result,expected);
        repeat(4)begin tick();if(!response || accepted)$fatal(1,"lost response under backpressure");end
        ready=1;tick();ready=0;
    endtask
    initial begin
        for(integer i=0;i<64;i++)mem[i]=64'hfedcba9876543210;
        tick();
        for(integer lane=0;lane<4;lane++) begin
            busy=1;request(1,8'(12+lane),16'(16'h1000+lane));
            address=8'hff;data_word=0; // caller may change after acceptance
            repeat(5)begin
                tick();
                if(!dw || rd || da!=29'h6000003 || be!=(8'h03<<(2*lane)))
                    $fatal(1,"write changed while stalled");
            end
            busy=0;wait(response);tick();ready=1;tick();ready=0;
            for(integer other=0;other<4;other++)begin
                if(other<=lane)begin
                    if(mem[3][other*16+:16]!==16'(16'h1000+other))$fatal(1,"write lane lost");
                end else if(mem[3][other*16+:16] !== (64'hfedcba9876543210>>(other*16) & 16'hffff))
                    $fatal(1,"neighbor lane changed");
            end
        end
        for(integer lane=0;lane<4;lane++)begin
            busy=1;request(0,8'(12+lane),0);
            repeat(3)tick();if(!rd)$fatal(1,"read not held while busy");
            busy=0;tick();repeat(7)tick();
            q=mem[3];rvalid=1;tick();rvalid=0;
            read_result(16'(16'h1000+lane));
        end
        // Reset after acceptance: wait for the old data, discard it, and
        // only then permit another request with its own response.
        request(0,8'd0,0);tick();reset=1;tick();reset=0;
        repeat(5)begin tick();if(accepted || response)$fatal(1,"reset fabricated completion");end
        q=64'h1111222233334444;rvalid=1;tick();rvalid=0;
        repeat(3)tick();if(!accepted || response)$fatal(1,"orphan read not drained");
        request(0,8'd7,0);tick();q=64'h5555666677778888;rvalid=1;tick();rvalid=0;
        read_result(16'h5555);
        // A zero-latency peer may return data on the accepting bus edge.
        busy=1;request(0,8'd2,0);q=64'h1111222233334444;rvalid=1;
        busy=0;tick();rvalid=0;read_result(16'h2222);
        if(mem[2]!==64'hfedcba9876543210 || mem[4]!==64'hfedcba9876543210)
            $fatal(1,"neighbor word changed");
        if(writes!=4 || reads!=7)$fatal(1,"duplicate/lost bus operations: writes %d reads %d",writes,reads);
        $display("STORAGE-DDRAM PASS: byte lanes, stalls, delayed reads, response backpressure, reset drain");
        $finish;
    end
    initial begin repeat(1000)tick();$fatal(1,"timeout state=%d reads=%d writes=%d valid=%b ready=%b rsp=%b busy=%b",d.state,reads,writes,valid,ready,response,busy);end
endmodule
