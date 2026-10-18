// Compare every logical DRAM output cycle against the original bit chips
// while backing requests take varying numbers of ungated host clocks.
`timescale 1ns/1ps
module tb_storage_group #(parameter integer DEFER_READS=0, ADDR_BITS=14);
    localparam integer WORDS=1<<ADDR_BITS;
    reg clk=0;always #1 clk=~clk;
    reg run=0,reset=0;
    wire machine_clk=clk && run;
    reg ras_n=1,cas_n=1,we_n=1;
    reg [ADDR_BITS-1:0] next_addr=0,addr=0;
    reg [15:0] data_in=0;
    wire [15:0] actual,expected;
    wire ready,valid,rwrite,response_ready;
    wire [ADDR_BITS-1:0] address;
    wire [15:0] data_word;
    reg pending=0,response=0;
    reg [15:0] response_data=0;
    reg [15:0] backing[0:WORDS-1];
    reg saved_write=0;
    reg [ADDR_BITS-1:0] saved_address=0;
    reg [15:0] saved_data=0;
    integer delay_count=0,requests=0,writes=0,steps=0;
    always @(posedge machine_clk)addr<=next_addr;
    dorado_storage_group #(.ADDR_BITS(ADDR_BITS),.DEFER_READS(DEFER_READS)) d(.host_clk(clk),.machine_clk(machine_clk),.reset(reset),
        .ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(addr),.data_in(data_in),
        .data_out(actual),.ready(ready),.req_valid(valid),.req_ready(!pending),
        .req_write(rwrite),.req_addr(address),.req_data(data_word),
        .rsp_valid(response),.rsp_ready(response_ready),.rsp_data(response_data));
    for(genvar bitno=0;bitno<16;bitno++)begin:original
        cell_MK4096P_6 #(.ADDR_BITS(ADDR_BITS)) c(.sys_clk(machine_clk),.p2(data_in[bitno]),.p3(we_n),
            .p4(ras_n),.p15(cas_n),.addr_i(addr),.p14(expected[bitno]));
        initial for(integer a=0;a<WORDS;a++)c.mem[a]=0;
    end
    always @(posedge clk)begin
        if(valid && !pending)begin
            pending<=1;saved_write<=rwrite;saved_address<=address;saved_data<=data_word;
            delay_count<=1+(requests%7);requests<=requests+1;
        end
        if(pending && !response)begin
            if(delay_count!=0)delay_count<=delay_count-1;
            else begin
                if(saved_write)begin backing[saved_address]<=saved_data;writes<=writes+1;end
                response_data<=backing[saved_address];response<=1;
            end
        end
        if(response && response_ready)begin response<=0;pending<=0;end
    end
    task tick;@(negedge clk);endtask
    task step(input bit r,input bit c,input bit w,input [ADDR_BITS-1:0] a,input [15:0] value);
        tick();ras_n=r;cas_n=c;we_n=w;next_addr=a;data_in=value;
        // Controls settle while the logical clock stays stopped. A new read
        // can require the previous pipeline word before this edge may run.
        tick();wait(ready);tick();run=1;
        tick();run=0;steps++;
        if(actual!==expected)$fatal(1,"DRAM cycle differs step %d: external %h chip %h",steps,actual,expected);
    endtask
    reg finished=0;
    integer verified=0;
    for(genvar bitno=0;bitno<16;bitno++)begin:check_plane
        initial begin
            wait(finished);
            for(integer a=0;a<WORDS;a++)
                if(original[bitno].c.mem[a]!==backing[a][bitno])
                    $fatal(1,"backing differs bit %d addr %h",bitno,a);
            verified++;
        end
    end
    reg [31:0] random_state=32'h159e742b;
    function automatic [ADDR_BITS-1:0] seeded_address(input integer n);
        seeded_address=ADDR_BITS'((n*313)+(1<<(ADDR_BITS-1))+16'h1021);
    endfunction
    reg [15:0] seeded[0:63];
    integer resets=0;
    task pause_reset;
        wait(ready);tick();reset=1;
        repeat(3)tick();reset=0;resets++;
    endtask
    initial begin
        for(integer a=0;a<WORDS;a++)backing[a]=0;
        tick();
        // Directed same-address read/write turnaround, then random control
        // changes and addresses across every row/column bit.
        step(1,1,1,{ADDR_BITS{1'b1}},0);
        step(0,0,0,{ADDR_BITS{1'b1}},16'hbd67);
        step(0,0,1,{ADDR_BITS{1'b1}},0);
        repeat(4)step(0,0,1,{ADDR_BITS{1'b1}},0);
        if(actual!==16'hbd67)$fatal(1,"directed write/read failed");
        // Distinct nonzero words make a deferred previous-column fetch
        // distinguishable from simply reading the current address or zero.
        for(integer i=0;i<64;i++)begin
            seeded[i]=16'ha5c3^(16'(i)*16'h0171);
            step(1,1,1,seeded_address(i),0);
            step(0,0,0,seeded_address(i),seeded[i]);
            step(1,1,1,seeded_address(i),0);
        end
        for(integer i=0;i<64;i++)begin
            step(1,1,1,seeded_address(i),0);
            step(1,1,1,seeded_address((i+1)%64),0);
            if(i%8==0)pause_reset();
            step(0,0,1,seeded_address((i+2)%64),0);
            if(actual!==seeded[i])$fatal(1,"deferred previous column differs %d",i);
            step(0,0,1,seeded_address((i+3)%64),0);
            if(actual!==seeded[(i+1)%64])$fatal(1,"successive column differs %d",i);
        end
        for(integer i=0;i<32;i++)begin
            // Same-address old value must survive a physical write that
            // completed before this logical commit/output edge.
            step(1,1,1,seeded_address(i),0);
            step(0,0,0,seeded_address(i),~seeded[i]);
            step(0,0,1,seeded_address(i),0);
            if(actual!==seeded[i])$fatal(1,"write exposed NEW word on old pipeline edge");
            step(0,0,1,seeded_address((i+1)%64),0);
            if(actual!==seeded[i])$fatal(1,"write commit pipeline old word lost");
            seeded[i]=~seeded[i];
            step(0,0,1,seeded_address((i+1)%64),0);
            if(actual!==seeded[i])$fatal(1,"newly written word not visible");
            // Commit to B while output consumes the deferred A value.
            step(1,1,1,seeded_address(i),0);
            step(0,0,0,seeded_address((i+1)%64),~seeded[(i+1)%64]);
            step(0,0,1,seeded_address((i+2)%64),0);
            if(actual!==seeded[i])$fatal(1,"other-column write destroyed deferred pipeline");
            step(0,0,1,seeded_address((i+3)%64),0);
            if(actual!==seeded[(i+1)%64])$fatal(1,"other-column old commit word lost");
            seeded[(i+1)%64]=~seeded[(i+1)%64];
            pause_reset();
        end
        for(integer i=0;i<4000;i++)begin
            random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
            step(random_state[0],random_state[1],random_state[2],random_state[ADDR_BITS+2:3],random_state[31:16]);
        end
        // Retire a possible final pending write before comparing arrays.
        step(1,1,1,0,0);step(1,1,1,0,0);wait(ready);tick();
        finished=1;wait(verified==16);
        $display("STORAGE-GROUP PASS: defer=%0d, %d logical chip cycles, %d requests, %d writes, %0d retained-pipeline resets, all %0d backing bits identical, address_bits=%0d",DEFER_READS,steps,requests,writes,resets,16*WORDS,ADDR_BITS);
        $finish;
    end
    initial begin repeat(200000)tick();$fatal(1,"timeout");end
endmodule
