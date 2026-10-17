// Compare every logical DRAM output cycle against the original bit chips
// while backing requests take varying numbers of ungated host clocks.
`timescale 1ns/1ps
module tb_storage_group;
    reg clk=0;always #1 clk=~clk;
    reg run=0;
    wire machine_clk=clk && run;
    reg ras_n=1,cas_n=1,we_n=1;
    reg [13:0] next_addr=0,addr=0;
    reg [15:0] data_in=0;
    wire [15:0] actual,expected;
    wire ready,valid,rwrite,response_ready;
    wire [13:0] address;
    wire [15:0] data_word;
    reg pending=0,response=0;
    reg [15:0] response_data=0;
    reg [15:0] backing[0:16383];
    reg saved_write=0;
    reg [13:0] saved_address=0;
    reg [15:0] saved_data=0;
    integer delay_count=0,requests=0,writes=0,steps=0;
    always @(posedge machine_clk)addr<=next_addr;
    dorado_storage_group d(.host_clk(clk),.machine_clk(machine_clk),.reset(1'b0),
        .ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(addr),.data_in(data_in),
        .data_out(actual),.ready(ready),.req_valid(valid),.req_ready(!pending),
        .req_write(rwrite),.req_addr(address),.req_data(data_word),
        .rsp_valid(response),.rsp_ready(response_ready),.rsp_data(response_data));
    for(genvar bitno=0;bitno<16;bitno++)begin:original
        cell_MK4096P_6 c(.sys_clk(machine_clk),.p2(data_in[bitno]),.p3(we_n),
            .p4(ras_n),.p15(cas_n),.addr_i(addr),.p14(expected[bitno]));
        initial for(integer a=0;a<16384;a++)c.mem[a]=0;
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
    task step(input bit r,input bit c,input bit w,input [13:0] a,input [15:0] value);
        wait(ready);tick();
        ras_n=r;cas_n=c;we_n=w;next_addr=a;data_in=value;run=1;
        tick();run=0;steps++;
        if(actual!==expected)$fatal(1,"DRAM cycle differs step %d: external %h chip %h",steps,actual,expected);
    endtask
    reg finished=0;
    integer verified=0;
    for(genvar bitno=0;bitno<16;bitno++)begin:check_plane
        initial begin
            wait(finished);
            for(integer a=0;a<16384;a++)
                if(original[bitno].c.mem[a]!==backing[a][bitno])
                    $fatal(1,"backing differs bit %d addr %h",bitno,a);
            verified++;
        end
    end
    reg [31:0] random_state=32'h159e742b;
    initial begin
        for(integer a=0;a<16384;a++)backing[a]=0;
        tick();
        // Directed same-address read/write turnaround, then random control
        // changes and addresses across every row/column bit.
        step(1,1,1,14'h3fff,0);
        step(0,0,0,14'h3fff,16'hbd67);
        step(0,0,1,14'h3fff,0);
        repeat(4)step(0,0,1,14'h3fff,0);
        if(actual!==16'hbd67)$fatal(1,"directed write/read failed");
        for(integer i=0;i<4000;i++)begin
            random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
            step(random_state[0],random_state[1],random_state[2],random_state[16:3],random_state[31:16]);
        end
        // Retire a possible final pending write before comparing arrays.
        step(1,1,1,0,0);step(1,1,1,0,0);wait(ready);tick();
        finished=1;wait(verified==16);
        $display("STORAGE-GROUP PASS: %d logical chip cycles, %d requests, %d writes, all 262144 backing bits identical",steps,requests,writes);
        $finish;
    end
    initial begin repeat(200000)tick();$fatal(1,"timeout");end
endmodule
