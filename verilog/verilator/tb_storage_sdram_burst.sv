// Raw-port scoreboard against the independent XSDS module/pin model.
// Physical clock is opposite phase to the host; model checks worst-case
// -7 timing and6ns read tAC, including board CS inversion and address DQM.
`timescale 1ns/1ps
module tb_storage_sdram_burst #(
    parameter integer USE_BURST=1,
    parameter integer CACHE_LINES=4,
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
    generate if(USE_BURST)begin:buffered
    dorado_storage_sdram_burst #(.ADDR_BITS(26),.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES)) dut(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(req_ready),
        .req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(rsp_valid),.rsp_ready(rsp_ready),.rsp_data(rsp_data),
        .initialized(initialized),.cache_hit(cache_hit),
        .sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),
        .sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
        .sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),
        .dq_out(dq_out),.dq_oe(dq_oe),.dq_in(dq));
    end else begin:baseline
    dorado_storage_sdram #(.ADDR_BITS(26),.CLOCK_HZ(CLOCK_HZ)) dut(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(req_ready),
        .req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(rsp_valid),.rsp_ready(rsp_ready),.rsp_data(rsp_data),
        .initialized(initialized),.cache_hit(cache_hit),
        .sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),
        .sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
        .sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),
        .dq_out(dq_out),.dq_oe(dq_oe),.dq_in(dq));
    end endgenerate
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
    int commands=0,reads=0,writes=0,checks=0,physical_reads=0,physical_writes=0,hits=0;
    longint unsigned host_ticks=0;
    always @(posedge clk)begin host_ticks<=host_ticks+1;if(cache_hit)hits++;end
    int refreshes[2]='{0,0};
    reg [15:0] expected[int unsigned];
    // Pin-only tOH check. Sample just before the chip edge and 0.5ns after;
    // AS4C32M16SB Table16 requires the previous read data for at least2.5ns.
    // The controller is not driving during a read. Nonzero burst patterns
    // expose a spurious Hi-Z interval even in a two-state simulator.
    reg [15:0] held_dq;
    always @(negedge chip_clk)begin
        #(500000000.0/CLOCK_HZ-0.5);held_dq=dq;
        #1;
        if(!dq_oe && dq!==held_dq)$fatal(1,"read output hold tOH violation before=%h after=%h",held_dq,dq);
    end
    always @(posedge chip_clk)begin
        if({nras,ncas,nwe}!=3'b111)commands++;
        if({nras,ncas,nwe}==3'b101)physical_reads++;
        if({nras,ncas,nwe}==3'b100)physical_writes++;
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
    function automatic [15:0] pattern(input int unsigned address);
        pattern=16'(address) ^ 16'(address>>16) ^ 16'ha57b;
    endfunction
    task automatic seed_line(input int unsigned address);
        for(int lane=0;lane<8;lane++)transfer(1,(address & ~7)+32'(lane),pattern((address & ~7)+32'(lane)));
    endtask
    initial begin
        int before_reads,r0,r1;
        longint unsigned before_ticks;
        int unsigned a0=32'h004000,b0=32'h2004000;
        repeat(4)@(negedge clk);reset=0;
        // Every rank/bank and every selected first-return beat, with nonzero
        // neighboring cells. Physical pins/models are the only backing store.
        for(int rank=0;rank<2;rank++)for(int bank=0;bank<4;bank++)begin
            for(int lane=0;lane<8;lane++)begin
                int unsigned address=(32'(rank)<<25)|(32'(bank)<<23)|(32'(lane)<<10)|32'(lane);
                seed_line(address);before_reads=physical_reads;
                transfer(0,address,0);
                for(int other=0;other<8;other++)transfer(0,(address & ~7)+32'(other),0);
                if(USE_BURST && physical_reads!=before_reads+1)$fatal(1,"one fully filled line should need one physical read");
            end
        end
        // Pin-mode performance comparison: same requests and response holds.
        for(int n=0;n<32;n++)transfer(1,a0+32'(n),pattern(a0+32'(n)));
        before_reads=physical_reads;before_ticks=host_ticks;
        for(int n=0;n<32;n++)transfer(0,a0+32'(n),0);
        $display("BURST_SEQUENCE requests=32 physical_reads=%0d host_ticks=%0d",physical_reads-before_reads,host_ticks-before_ticks);
        if(USE_BURST && physical_reads!=before_reads+4)$fatal(1,"sequential burst misses");
        seed_line(a0);seed_line(b0);before_reads=physical_reads;before_ticks=host_ticks;
        for(int lane=0;lane<8;lane++)begin transfer(0,a0+32'(lane),0);transfer(0,b0+32'(lane),0);end
        $display("BURST_ALTERNATING requests=16 physical_reads=%0d host_ticks=%0d",physical_reads-before_reads,host_ticks-before_ticks);
        if(USE_BURST && CACHE_LINES>=2 && physical_reads!=before_reads+2)$fatal(1,"rank-tagged alternating lines");
        // A write must reach the physical chip and invalidate its complete
        // cached line. Both unchanged neighbor and written cell are checked.
        transfer(1,a0+3,16'h369c);before_reads=physical_reads;
        transfer(0,a0+2,0);transfer(0,a0+3,0);
        if(USE_BURST && physical_reads!=before_reads+1)$fatal(1,"write did not invalidate/refill one line");
        // Pulse reset at each physical burst-return position. The accepted
        // read drains while reset is released again; it may not refill a
        // new epoch's cache or leak a completion to the next request.
        for(int beat=0;beat<8;beat++)begin
            @(negedge clk);reset=1;repeat(2)@(negedge clk);reset=0;
            send(0,a0+7,0);wait({nras,ncas,nwe}==3'b101);
`ifdef DORADO_CORRECT_READ_CAPTURE
            repeat(3+beat)@(negedge clk);reset=1;@(negedge clk);reset=0;
`else
            repeat(4+beat)@(negedge clk);reset=1;@(negedge clk);reset=0;
`endif
            repeat(24)begin @(negedge clk);if(rsp_valid)$fatal(1,"orphan burst response after reset");end
            before_reads=physical_reads;transfer(0,a0+7,0);
            if(physical_reads!=before_reads+1)$fatal(1,"orphan burst refilled cache after reset");
        end
        // A write accepted before reset still changes the chip, with no old
        // completion surviving reset. Reading neighbors refills from SDRAM.
        send(1,a0+4,16'hbeef);@(negedge clk);reset=1;
        repeat(30)begin @(negedge clk);if(rsp_valid)$fatal(1,"reset write response");end
        reset=0;expected[a0+4]=16'hbeef;
        repeat(10)begin @(negedge clk);if(rsp_valid)$fatal(1,"orphan write completion");end
        transfer(0,a0+3,0);transfer(0,a0+4,0);
        // Held cached or physical responses must allow both chips' refresh.
        send(0,a0,0);wait(rsp_valid);r0=refreshes[0];r1=refreshes[1];
        repeat(5000)begin
            @(negedge clk);
            if(!rsp_valid || rsp_data!==expected[a0] || req_ready)$fatal(1,"backpressure corrupted burst data");
        end
        if(refreshes[0]<r0+10 || refreshes[1]<r1+10)$fatal(1,"refresh starved by cached response");
        rsp_ready=1;@(negedge clk);rsp_ready=0;
        if(chip0.error_count!=0 || chip1.error_count!=0 || chip0.warning_count!=0 || chip1.warning_count!=0)
            $fatal(1,"independent physical chip-model error or warning");
        $display("STORAGE-SDRAM-BURST PASS use_burst=%0d lines=%0d checks=%0d reads=%0d writes=%0d physical_reads=%0d physical_writes=%0d cache_hits=%0d",USE_BURST,CACHE_LINES,checks,reads,writes,physical_reads,physical_writes,hits);
        $finish;
    end
    initial begin #2000000;$fatal(1,"test deadline");end
endmodule
