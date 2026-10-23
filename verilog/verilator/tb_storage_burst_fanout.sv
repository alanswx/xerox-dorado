// Synthetic coherent/scattered memory traffic through the exact native128
// arbiter and physical SDRAM model. No CPU, firmware or Cedar workload here.
`timescale 1ns/1ps
module tb_storage_burst_fanout;
    reg clk=0,reset=1;
    always #(500000000.0/57142857) clk=~clk;
    reg [127:0] valid=0,writing=0,ready_response=0;
    reg [2047:0] addresses=0,write_data=0;
    wire [127:0] ready,response;
    wire [2047:0] read_data;
    wire rv,rr,rw,sv,sr,physical_read,physical_write;
    wire [22:0] ra;
    wire [15:0] wd,sd;
    dorado_storage_arb #(.GROUPS(128),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(23),
        .PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)) arb(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(writing),.req_ready(ready),
        .req_addr(addresses),.req_data(write_data),.rsp_valid(response),
        .rsp_ready(ready_response),.rsp_data(read_data),.raw_req_valid(rv),
        .raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
    tb_xsds_storage #(.ADDR_BITS(23)) memory(
        .clk(clk),.reset(reset),.req_valid(rv),.req_write(rw),.rsp_ready(sr),
        .req_ready(rr),.rsp_valid(sv),.req_addr(ra),.req_data(wd),.rsp_data(sd),
        .physical_read(physical_read),.physical_write(physical_write));
    integer trace_file=0,ticks=0,round_id=-1,accepted=0,completed=0;
    integer physical_reads=0,physical_writes=0,forwarded=0,hits=0;
    integer class_count[0:7];
    integer offered_at[0:127],accepted_at[0:127];
    longint admission_wait=0,service_wait=0;
    reg active=0;
    reg [127:0] pending=0,finished=0,held=0;
    reg [2047:0] held_data=0;
    reg [15:0] expected[0:127];
    always @(posedge clk)begin
        ticks++;
        if(active)begin
            // Exclusive host-tick attribution, not a CPU speed estimate.
            if(sv && !sr)class_count[0]++;
            else if(rv && !rr && memory.dut.state==6)class_count[1]++;
            else if(rv && !rr && (memory.dut.refresh_due || memory.dut.state>=8))class_count[2]++;
            else if(rv && !rr)class_count[3]++;
            else if(rv && rr)class_count[4]++;
            else if(sv && sr)class_count[5]++;
            else if(arb.state==2)class_count[6]++;
            else class_count[7]++;
            if(physical_read)physical_reads++;
            if(physical_write)physical_writes++;
            if(memory.dut.cache_hit)hits++;
            if(rv && rr && memory.dut.state==6)forwarded++;
            for(integer g=0;g<128;g++)begin
                if(held[g] && (!response[g] || read_data[16*g+:16]!==held_data[16*g+:16]))
                    $fatal(1,"FANOUT response changed group=%0d",g);
                if(response[g] && ready_response[g])begin
                    if(!pending[g] || finished[g])$fatal(1,"FANOUT wrong owner group=%0d",g);
                    if(!writing[g] && read_data[16*g+:16]!==expected[g])
                        $fatal(1,"FANOUT data group=%0d got%h expected%h",g,read_data[16*g+:16],expected[g]);
                    pending[g]=0;finished[g]=1;completed++;
                    service_wait+=ticks-accepted_at[g];
                    $fdisplay(trace_file,"R,%0d,%0d,%0d,%h,%h,%0d",ticks,round_id,g,addresses[16*g+:16],read_data[16*g+:16],writing[g]);
                end
                if(valid[g] && ready[g])begin
                    if(pending[g] || finished[g])$fatal(1,"FANOUT duplicated acceptance");
                    if(!rv || !rr || ra!==({7'b0,addresses[16*g+:16]}<<7 | 23'(g)) ||
                       rw!==writing[g] || wd!==write_data[16*g+:16])
                        $fatal(1,"FANOUT raw ownership/payload mismatch");
                    pending[g]=1;accepted_at[g]=ticks;accepted++;
                    admission_wait+=ticks-offered_at[g];
                    $fdisplay(trace_file,"A,%0d,%0d,%0d,%h,%h,%0d",ticks,round_id,g,addresses[16*g+:16],write_data[16*g+:16],writing[g]);
                end
                held[g]=response[g] && !ready_response[g];
                held_data[16*g+:16]=read_data[16*g+:16];
            end
        end
    end
    function automatic [15:0] value(input integer family,g);
        value=16'h6321 ^ 16'(family<<8) ^ 16'(g);
    endfunction
    initial begin
        string trace_path;
        reg [127:0] mask;
        reg [127:0] take_request;
        reg [15:0] address;
        reg [63:0] physical;
        integer mode,family,begin_ticks,begin_reads,begin_forward,begin_hits,timeout;
        for(integer c=0;c<8;c++)class_count[c]=0;
        if(!$value$plusargs("trace=%s",trace_path))$fatal(1,"FANOUT trace path required");
        trace_file=$fopen(trace_path,"w");
        $fdisplay(trace_file,"kind,tick,round,group,address,data,write");
        repeat(5)@(negedge clk);reset=0;wait(memory.initialized);
        for(round_id=0;round_id<24;round_id++)begin
            @(negedge clk);mode=round_id%6;family=round_id/6;
            mask=0;finished=0;pending=0;held=0;
            for(integer g=0;g<128;g++)begin
                mask[g]=mode<3 || g%4==0;
                address=16'h20+16'(family*16)+(mode>=3 ? 16'h1000+16'(g*17) : 16'd0);
                addresses[16*g+:16]=address;write_data[16*g+:16]=value(family,g);
                writing[g]=mode==1 || mode==4;
                expected[g]=(mode==2 || mode==5) ? value(family,g) : 16'd0;
                offered_at[g]=ticks;
            end
            valid=mask;ready_response=0;active=1;timeout=0;
            begin_ticks=ticks;begin_reads=physical_reads;begin_forward=forwarded;begin_hits=hits;
            while(finished!==mask)begin
                // Occasional response holds retain stable ownership. Payloads
                // of every unaccepted producer stay fixed for the whole round.
                ready_response=pending & {128{mode!=5 || ticks%13<8}};
                @(posedge clk);take_request=valid & ready;
                @(negedge clk);valid=valid & ~take_request;
                timeout++;if(timeout>20000)$fatal(1,"FANOUT round deadline");
            end
            active=0;valid=0;ready_response=0;
            if(pending!==0)$fatal(1,"FANOUT unfinished owner");
            $display("FANOUT_ROUND round=%0d mode=%0d groups=%0d ticks=%0d physical_reads=%0d forwarded=%0d hits=%0d",round_id,mode,$countones(mask),ticks-begin_ticks,physical_reads-begin_reads,forwarded-begin_forward,hits-begin_hits);
        end
        // Verify every programmed location directly in the physical model.
        for(integer family0=0;family0<4;family0++)for(integer g=0;g<128;g++)begin
            for(integer form=0;form<2;form++)if(form==0 || g%4==0)begin
                address=16'h20+16'(family0*16)+(form ? 16'h1000+16'(g*17) : 16'd0);
                physical=memory.peek64(((int'(address)<<7)|g)/4);
                if(physical[16*(g%4)+:16]!==value(family0,g))$fatal(1,"FANOUT final physical memory mismatch");
            end
        end
        memory.check_model();
        if(accepted!=1920 || completed!=accepted || physical_writes!=640)
            $fatal(1,"FANOUT count mismatch accepted=%0d completed=%0d writes=%0d",accepted,completed,physical_writes);
        $display("FANOUT_TOTAL accepted=%0d completed=%0d reads=%0d writes=%0d forwarded=%0d hits=%0d admission_wait=%0d service_wait=%0d",accepted,completed,physical_reads,physical_writes,forwarded,hits,admission_wait,service_wait);
        for(integer c=0;c<8;c++)$display("FANOUT_CLASS id=%0d ticks=%0d",c,class_count[c]);
        $fclose(trace_file);$display("BURST_FANOUT PASS groups=128 synthetic_only=1");$finish;
    end
    initial begin #3000000;$fatal(1,"FANOUT overall deadline");end
endmodule
