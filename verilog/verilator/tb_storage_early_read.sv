// Compare physical behavior with the exact selected-word-capture parent.
// Common arbitrary DQ covers all data values. Synchronized requests isolate
// response latency; physical-pin/coherence and original-MSA suites are separate.
`timescale 1ns/1ps
module tb_storage_early_read #(parameter integer EARLY=1);
    reg clk=0,reset=1,req_valid=0,req_write=0,rsp_ready=0;
    reg [25:0] req_addr=0;
    reg [15:0] req_data=0,dq_in=16'h1234;
    reg [31:0] random_q=32'h734ac159;
    always #8.75 clk=~clk;
    always @(negedge clk)begin
        random_q<={random_q[30:0],random_q[31]^random_q[21]^random_q[1]^random_q[0]};
        dq_in<=random_q[23:8]^random_q[15:0];
    end
    wire [127:0] obs_old,obs_new;
    wire [12:0] a_old,a_new;
    wire [1:0] ba_old,ba_new;
    wire [15:0] data_old,data_new,out_old,out_new;
    // {idle,req_ready,rsp_valid,initialized,hit,read,write,return,
    //  ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe}
    wire [15:0] flags_old,flags_new;
    dorado_storage_sdram_burst_reference #(.ADDR_BITS(26),.CLOCK_HZ(57142857),.CACHE_LINES(4),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1)) reference(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(flags_old[14]),.req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(flags_old[13]),.rsp_ready(rsp_ready),.rsp_data(data_old),.initialized(flags_old[12]),.cache_hit(flags_old[11]),
        .physical_read(flags_old[10]),.physical_write(flags_old[9]),.read_return(flags_old[8]),.snapshot_idle(flags_old[15]),.burst_observation(obs_old),
        .sdram_a(a_old),.sdram_ba(ba_old),.sdram_ncs(flags_old[7]),.sdram_nras(flags_old[6]),.sdram_ncas(flags_old[5]),.sdram_nwe(flags_old[4]),
        .sdram_dqml(flags_old[3]),.sdram_dqmh(flags_old[2]),.sdram_cke(flags_old[1]),.dq_out(out_old),.dq_oe(flags_old[0]),.dq_in(dq_in));
    dorado_storage_sdram_burst #(.ADDR_BITS(26),.CLOCK_HZ(57142857),.CACHE_LINES(4),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(EARLY)) candidate(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(flags_new[14]),.req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(flags_new[13]),.rsp_ready(rsp_ready),.rsp_data(data_new),.initialized(flags_new[12]),.cache_hit(flags_new[11]),
        .physical_read(flags_new[10]),.physical_write(flags_new[9]),.read_return(flags_new[8]),.snapshot_idle(flags_new[15]),.burst_observation(obs_new),
        .sdram_a(a_new),.sdram_ba(ba_new),.sdram_ncs(flags_new[7]),.sdram_nras(flags_new[6]),.sdram_ncas(flags_new[5]),.sdram_nwe(flags_new[4]),
        .sdram_dqml(flags_new[3]),.sdram_dqmh(flags_new[2]),.sdram_cke(flags_new[1]),.dq_out(out_new),.dq_oe(flags_new[0]),.dq_in(dq_in));
    integer ticks=0,checks=0,held_checks=0,early_replies=0;
    integer old_first=-1,new_first=-1,accepted=0,old_acks=0,new_acks=0;
    reg old_inflight=0,new_inflight=0,held=0;
    reg [15:0] held_data=0;
    reg [7:0] read_beats=0,abort_beats=0;
    integer aborted_before=0,aborted_after=0;
    always @(posedge clk)begin
        ticks++;
        // Consume old completions before accepting a successor, exactly as
        // the single-outstanding external protocol requires.
        if(reset)begin old_inflight=0;new_inflight=0;end
        else begin
            if(flags_old[13] && rsp_ready)begin
                if(!old_inflight)$fatal(1,"EARLY_READ orphan parent response");
                old_inflight=0;old_acks++;
            end
            if(flags_new[13] && rsp_ready)begin
                if(!new_inflight)$fatal(1,"EARLY_READ orphan candidate response");
                new_inflight=0;new_acks++;
            end
            if(req_valid && (flags_new[14]!==flags_old[14]))
                $fatal(1,"EARLY_READ request entered before common drain");
            if(req_valid && flags_old[14])begin
                if(old_inflight || new_inflight)$fatal(1,"EARLY_READ outstanding request overwritten");
                old_inflight=1;new_inflight=1;old_first=-1;new_first=-1;accepted++;
            end
        end
        #1;
        if({flags_old[12:0],a_old,ba_old,out_old,obs_old} !== {flags_new[12:0],a_new,ba_new,out_new,obs_new})
            $fatal(1,"EARLY_READ physical commands/drain changed");
        if(data_old!==data_new)$fatal(1,"EARLY_READ selected data changed");
        if(!EARLY && flags_old!==flags_new)$fatal(1,"EARLY_READ default-off control changed");
        if(candidate.state==6 && flags_new[14])$fatal(1,"EARLY_READ request ready before burst drained");
        if(candidate.state==6 && flags_new[15])$fatal(1,"EARLY_READ idle before burst drained");
        if(flags_new[13] && !new_inflight)$fatal(1,"EARLY_READ duplicate completion");
        if(flags_old[13] && old_first<0)old_first=ticks;
        if(flags_new[13] && new_first<0)begin
            new_first=ticks;
            if(candidate.state==6)early_replies++;
        end
        if(held && !reset && !rsp_ready && (!flags_new[13] || data_new!==held_data))
            $fatal(1,"EARLY_READ response changed under hold");
        held=flags_new[13] && !rsp_ready;held_data=data_new;
        if(held)held_checks++;
        if(reference.state==6 && reference.delay_count==0 && !reference.aborted)
            read_beats[reference.address[2:0]]=1;
        checks++;
    end
    task automatic send(input bit writing,input reg[25:0] address,input reg[15:0] value);
        integer count;
        reg sent;
        begin
            @(negedge clk);req_write=writing;req_addr=address;req_data=value;count=0;
            while(!(flags_old[14] && flags_new[14]))begin
                @(negedge clk);count++;
                if(count>1000)$fatal(1,"EARLY_READ request-ready timeout");
            end
            req_valid=1;sent=0;
            while(!sent)begin
                @(posedge clk);sent=flags_old[14] && flags_new[14];
                @(negedge clk);
            end
            req_valid=0;
        end
    endtask
    task automatic operation(input bit writing,input reg[25:0] address,input integer hold_ticks);
        integer count;
        begin
            rsp_ready=hold_ticks==0;
            send(writing,address,16'hc150^address[15:0]);count=0;
            if(hold_ticks!=0)begin
                while(!flags_old[13])begin
                    @(negedge clk);count++;
                    if(count>1000)$fatal(1,"EARLY_READ held response timeout");
                end
                repeat(hold_ticks)@(negedge clk);
                rsp_ready=1;
            end
            while(old_inflight || new_inflight)begin
                @(negedge clk);count++;
                if(count>4000)$fatal(1,"EARLY_READ response timeout");
            end
        end
    endtask
    initial begin
        integer i,beat,count,total_saved=0,expect_saved;
        repeat(5)@(negedge clk);reset=0;wait(flags_old[12]);
        // Distinct cold lines: verify exact requested-beat latency savings.
        for(i=0;i<8;i++)begin
            operation(0,26'(i*16+i),0);
            expect_saved=EARLY ? 7-i : 0;
            if(old_first-new_first!=expect_saved)
                $fatal(1,"EARLY_READ latency beat=%0d old=%0d new=%0d expected=%0d",i,old_first,new_first,expect_saved);
            total_saved+=old_first-new_first;
        end
        for(i=0;i<128;i++)begin
            operation(i%5==2,(26'(i%8)<<23)|(26'((i/8)%4)<<10)|26'(i%8),i%7);
            operation(0,(26'(i%8)<<23)|(26'((i/8)%4)<<10)|26'((i+3)%8),i%4);
        end
        // Reset at every beat, including after the early reply was consumed.
        for(beat=0;beat<8;beat++)begin
            rsp_ready=1;send(0,26'h40000+26'(beat*16+(beat+3)%8),0);count=0;
            while(!(reference.state==6 && reference.delay_count==0 && reference.fill_beat==beat))begin
                @(negedge clk);count++;
                if(count>1000)$fatal(1,"EARLY_READ abort position timeout");
            end
            if(new_inflight)aborted_before++;else aborted_after++;
            reset=1;abort_beats[beat]=1;
            @(negedge clk);reset=0;
            repeat(30)begin
                @(negedge clk);
                if(flags_old[13] || flags_new[13])$fatal(1,"EARLY_READ aborted response escaped");
            end
            operation(0,26'h40000+26'(beat*16+(beat+3)%8),beat+2);
        end
        operation(0,26'h40007,1600);
        if(read_beats!==8'hff || abort_beats!==8'hff || held_checks<1600)
            $fatal(1,"EARLY_READ coverage missing");
        if(EARLY && (total_saved!=28 || early_replies<7 || aborted_after!=3 || aborted_before!=5))
            $fatal(1,"EARLY_READ early/drain/abort coverage missing saved=%0d early=%0d abort=%0d/%0d",total_saved,early_replies,aborted_before,aborted_after);
        $display("EARLY_READ_MITER PASS early=%0d checks=%0d accepted=%0d acks=%0d/%0d held=%0d first8_saved_ticks=%0d abort_before_after=%0d/%0d",EARLY,checks,accepted,old_acks,new_acks,held_checks,total_saved,aborted_before,aborted_after);
        $finish;
    end
    initial begin #10000000;$fatal(1,"EARLY_READ overall timeout");end
endmodule
