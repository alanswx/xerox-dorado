// Raw-port scoreboard against the independent XSDS module/pin model.
// Physical clock is opposite phase to the host; model checks worst-case
// -7 timing and6ns read tAC, including board CS inversion and address DQM.
`timescale 1ns/1ps
module tb_storage_sdram #(
    parameter integer CLOCK_HZ=50000000,
    parameter realtime CLOCK_DELAY_NS=0
);
    reg clk=0;always #(500000000.0/CLOCK_HZ) clk=~clk;
    wire chip_clk;
    assign #(CLOCK_DELAY_NS) chip_clk=~clk;
    reg reset=1,req_valid=0,req_write=0,rsp_ready=0;
    reg [25:0] req_addr=0;
    reg [15:0] req_data=0;
    wire req_ready,rsp_valid,initialized,cache_hit;
    wire [15:0] rsp_data,dq_out;
    wire [12:0] a;
    wire [1:0] ba;
    wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe;
    wire [15:0] dq;
    assign dq=dq_oe ? dq_out : 16'hzzzz;
    dorado_storage_sdram #(.ADDR_BITS(26),.CLOCK_HZ(CLOCK_HZ)) dut(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(req_ready),
        .req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(rsp_valid),.rsp_ready(rsp_ready),.rsp_data(rsp_data),
        .initialized(initialized),.cache_hit(cache_hit),
        .sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),
        .sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
        .sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),
        .dq_out(dq_out),.dq_oe(dq_oe),.dq_in(dq));
    // Independent chip models wired as the XSDS schematic specifies.
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("XSDS_RANK0"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("XSDS_RANK1"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    int commands=0,reads=0,writes=0,checks=0;
    int refreshes[2]='{0,0};
    reg [15:0] expected[int unsigned];
    always @(posedge chip_clk)begin
        if({nras,ncas,nwe}!=3'b111)commands++;
        if({nras,ncas,nwe}==3'b001)refreshes[ncs]++;
        if(dqml!==a[11] || dqmh!==a[12] || !cke)$fatal(1,"module pin mapping");
    end
    task automatic send(input bit wr,input int unsigned address,input reg[15:0] value);
        int timeout;
        @(negedge clk);req_valid=1;req_write=wr;req_addr=26'(address);req_data=value;
        timeout=0;
        while(!req_ready)begin
            @(negedge clk);timeout++;
            if(timeout>15000)$fatal(1,"raw request timeout");
        end
        @(posedge clk);#1;
        @(negedge clk);req_valid=0;
    endtask
    task automatic transfer(input bit wr,input int unsigned address,input reg[15:0] value);
        int timeout;
        reg[15:0] saved;
        send(wr,address,value);timeout=0;
        while(!rsp_valid)begin
            @(negedge clk);timeout++;
            if(timeout>100)$fatal(1,"raw response timeout at %h",address);
        end
        saved=rsp_data;
        if(wr)begin expected[address]=value;writes++;end
        else begin
            if(!expected.exists(address) || rsp_data!==expected[address])
                $fatal(1,"read %h got %h expected %h",address,rsp_data,expected[address]);
            reads++;
        end
        repeat(3)begin
            @(negedge clk);
            if(!rsp_valid || rsp_data!==saved || req_ready)$fatal(1,"response not held");
        end
        rsp_ready=1;@(negedge clk);rsp_ready=0;checks++;
    endtask
    initial begin
        reg[31:0] rng=32'hc0d0da7a;
        int unsigned addresses[128];
        int r0,r1;
        repeat(4)@(negedge clk);reset=0;
        // First request is deliberately held throughout physical startup.
        transfer(1,0,16'h1234);transfer(0,0,0);
        if($realtime<200000 || !initialized)$fatal(1,"early initialization");
        // Every bank/rank, both row boundaries, and both byte lanes.
        for(int rank=0;rank<2;rank++)for(int bank=0;bank<4;bank++)begin
            int unsigned base=(32'(rank)<<25)|(32'(bank)<<23);
            transfer(1,base,16'h8001^16'(bank*17+rank));
            transfer(1,base+1023,16'habcd^16'(bank));
            transfer(1,base+1024,16'h5aa5^16'(rank));
            transfer(0,base,0);transfer(0,base+1024,0);transfer(0,base+1023,0);
        end
        for(int n=0;n<128;n++)begin
            rng=(rng<<1)^((rng[31])?32'h04c11db7:0);
            addresses[n]=rng & 32'h03ffffff;
            transfer(1,addresses[n],rng[15:0]^rng[31:16]);
        end
        for(int n=127;n>=0;n--)transfer(0,addresses[n],0);
        // A held consumer must not starve either rank of refresh.
        send(0,0,0);wait(rsp_valid);r0=refreshes[0];r1=refreshes[1];
        repeat(5000)begin
            @(negedge clk);
            if(!rsp_valid || rsp_data!==expected[0] || req_ready)$fatal(1,"backpressure corrupted data");
        end
        if(refreshes[0]<r0+10 || refreshes[1]<r1+10)$fatal(1,"refresh starved by response");
        rsp_ready=1;@(negedge clk);rsp_ready=0;
        // Accepted write drains despite reset. No completion may leak into
        // the next machine; the stored word is still the accepted payload.
        send(1,32'h2012345,16'hbeef);
        @(negedge clk);reset=1;
        repeat(30)begin @(negedge clk);if(rsp_valid)$fatal(1,"reset response");end
        reset=0;
        repeat(20)begin @(negedge clk);if(rsp_valid)$fatal(1,"orphan write response");end
        expected[32'h2012345]=16'hbeef;transfer(0,32'h2012345,0);
        send(0,32'h2012345,0);@(negedge clk);reset=1;
        repeat(30)@(negedge clk);reset=0;
        repeat(20)begin @(negedge clk);if(rsp_valid)$fatal(1,"orphan read response");end
        transfer(0,0,0);
        // Reset also withdraws an already completed, unconsumed response.
        send(0,0,0);wait(rsp_valid);@(negedge clk);reset=1;
        repeat(3)@(negedge clk);reset=0;
        if(rsp_valid)$fatal(1,"stale completed response after reset");
        transfer(0,32'h2012345,0);
        if(chip0.error_count!=0 || chip1.error_count!=0 ||
           chip0.warning_count!=0 || chip1.warning_count!=0)
            $fatal(1,"independent chip-model errors or warnings");
        $display("STORAGE-SDRAM PASS: %0d checks, %0d reads/%0d writes, pin timing, both ranks/banks, row boundaries, backpressure refresh, reset/drain",checks,reads,writes);
        $finish;
    end
    initial begin #2000000;$fatal(1,"test deadline");end
endmodule
