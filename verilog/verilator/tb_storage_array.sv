// Three groups exercise non-power-of-two arbitration and DDR byte lanes.
// Every logical output and final stored bit is compared to the old chips.
`timescale 1ns/1ps
module tb_storage_array #(parameter integer LINE_CACHE_BITS=0);
    localparam G=3,W=12,A=6;
    reg clk=0;always #1 clk=~clk;
    reg run=0,reset=0;
    wire machine_clk=clk && run;
    reg [G-1:0] ras_n='1,cas_n='1,we_n='1;
    reg [G*A-1:0] next_addr=0,addr=0;
    reg [G*W-1:0] data_in=0;
    wire [G*W-1:0] actual,expected;
    wire [G-1:0] ready,valid,wr,accept,response,response_ready;
    wire [G*A-1:0] addresses;
    wire [G*16-1:0] wdata,rdata;
    wire rv,rw,rr,sv,sr;
    wire [7:0] ra;
    wire [15:0] wd,sd;
    wire [28:0] da;
    wire dr,dw;
    wire [7:0] be,burst;
    wire [63:0] ddata;
    reg busy=1,dvalid=0,read_pending=0;
    reg [63:0] dout=0,saved_read=0;
    reg [63:0] backing[0:63];
    integer latency=0,reads=0,writes=0,steps=0;
    reg [31:0] random_state=32'h6ca4138f;
    reg [7:0] host_random=8'hb7;
    always @(posedge machine_clk)addr<=next_addr;
    for(genvar g=0;g<G;g++)begin:group
        dorado_storage_group #(.ADDR_BITS(A),.WIDTH(W)) d(
            .host_clk(clk),.machine_clk(machine_clk),.reset(reset),
            .ras_n(ras_n[g]),.cas_n(cas_n[g]),.we_n(we_n[g]),
            .addr(addr[g*A+:A]),.data_in(data_in[g*W+:W]),.data_out(actual[g*W+:W]),
            .ready(ready[g]),.req_valid(valid[g]),.req_ready(accept[g]),
            .req_write(wr[g]),.req_addr(addresses[g*A+:A]),.req_data(wdata[g*16+:16]),
            .rsp_valid(response[g]),.rsp_ready(response_ready[g]),.rsp_data(rdata[g*16+:16]));
        for(genvar b=0;b<W;b++)begin:chip
            cell_MK4096P_6 c(.sys_clk(machine_clk),.p2(data_in[g*W+b]),
                .p3(we_n[g]),.p4(ras_n[g]),.p15(cas_n[g]),
                .addr_i({8'd0,addr[g*A+:A]}),.p14(expected[g*W+b]));
            initial for(integer a=0;a<16384;a++)c.mem[a]=0;
        end
    end
    dorado_storage_arb #(.GROUPS(G),.GROUP_ADDR_BITS(A),.RAW_ADDR_BITS(8)) arb(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(wr),.req_ready(accept),
        .req_addr(addresses),.req_data(wdata),.rsp_valid(response),.rsp_ready(response_ready),.rsp_data(rdata),
        .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
    dorado_storage_ddram #(.ADDR_BITS(8),.LINE_CACHE_BITS(LINE_CACHE_BITS),.GROUP_ADDR_BITS(A)) memory_port(
        .clk(clk),.reset(reset),.req_valid(rv),.req_ready(rr),.req_write(rw),.req_addr(ra),.req_data(wd),
        .rsp_valid(sv),.rsp_ready(sr),.rsp_data(sd),.ddram_busy(busy),.ddram_burstcount(burst),
        .ddram_addr(da),.ddram_read(dr),.ddram_write(dw),.ddram_wdata(ddata),.ddram_be(be),
        .ddram_rdata(dout),.ddram_rvalid(dvalid));
    always @(posedge clk)begin
        host_random<={host_random[6:0],host_random[7]^host_random[5]^host_random[4]^host_random[3]};
        dvalid<=0;busy<=host_random[3] && host_random[6];
        if(!busy && (dr || dw))begin
            if(da<29'h6000000 || da>=29'h6000040 || burst!=1)$fatal(1,"bad DDR address/burst");
            if(dw)begin
                for(integer b=0;b<8;b++)if(be[b])backing[da-29'h6000000][b*8+:8]<=ddata[b*8+:8];
                writes<=writes+1;
            end else begin
                if(read_pending)$fatal(1,"multiple outstanding reads");
                read_pending<=1;latency<=1+((reads*3)%11);
                saved_read<=backing[da-29'h6000000];reads<=reads+1;
            end
        end
        if(read_pending)begin
            if(latency!=0)latency<=latency-1;
            else begin read_pending<=0;dout<=saved_read;dvalid<=1;end
        end
    end
    task tick;@(negedge clk);endtask
    task step;
        wait(&ready);tick();run=1;tick();run=0;steps++;
        if(actual!==expected)$fatal(1,"logical cycle differs at %d: %h expected %h",steps,actual,expected);
    endtask
    reg finished=0;
    integer verified=0;
    for(genvar g=0;g<G;g++)for(genvar b=0;b<W;b++)begin:check_bit
        initial begin
            wait(finished);
            for(integer a=0;a<64;a++)
                if(group[g].chip[b].c.mem[a] !== backing[(g*64+a)/4][((g*64+a)%4)*16+b])
                    $fatal(1,"backing differs group %d bit %d addr %d",g,b,a);
            verified++;
        end
    end
    initial begin
        for(integer a=0;a<64;a++)backing[a]=0;
        tick();
        for(integer i=0;i<1500;i++)begin
            wait(&ready);tick();
            random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
            ras_n=random_state[2:0];cas_n=random_state[5:3];we_n=random_state[8:6];
            next_addr=random_state[26:9];data_in={random_state,random_state[11:8]};
            step();
        end
        wait(&ready);ras_n='1;cas_n='1;we_n='1;next_addr=0;
        step();step();wait(&ready);tick();
        // Reset with a cache-miss read in flight. DDR data must drain
        // without getting assigned to a different bank after reset.
        next_addr='1;step();tick();reset=1;repeat(4)tick();reset=0;
        wait(&ready);step();step();wait(&ready);tick();
        finished=1;wait(verified==G*W);
        if(reads<(LINE_CACHE_BITS>0 ? 100 : 1000) || writes<100)
            $fatal(1,"test did not exercise enough memory operations: reads=%0d writes=%0d",reads,writes);
        $display("STORAGE-ARRAY PASS: %d chip cycles, %d DDR reads, %d writes, three isolated groups, reset drain",steps,reads,writes);
        $finish;
    end
    initial begin repeat(300000)tick();$fatal(1,"timeout state %d ready %b",arb.state,ready);end
endmodule
