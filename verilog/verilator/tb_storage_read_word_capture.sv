// Control/valid-data miter against the exact qualified backend, renamed only.
// A common arbitrary DQ stream covers every possible physical-device stream
// while equal commands/address/write pins establish equal external behavior.
// Invalid rsp_data is deliberately not an observable protocol value.
`timescale 1ns/1ps
module tb_storage_read_word_capture;
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
    dorado_storage_sdram_burst_reference #(.ADDR_BITS(26),.CLOCK_HZ(57142857),.CACHE_LINES(4),.RESPONSE_OVERLAP(1)) reference(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(flags_old[14]),.req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(flags_old[13]),.rsp_ready(rsp_ready),.rsp_data(data_old),.initialized(flags_old[12]),.cache_hit(flags_old[11]),
        .physical_read(flags_old[10]),.physical_write(flags_old[9]),.read_return(flags_old[8]),.snapshot_idle(flags_old[15]),.burst_observation(obs_old),
        .sdram_a(a_old),.sdram_ba(ba_old),.sdram_ncs(flags_old[7]),.sdram_nras(flags_old[6]),.sdram_ncas(flags_old[5]),.sdram_nwe(flags_old[4]),
        .sdram_dqml(flags_old[3]),.sdram_dqmh(flags_old[2]),.sdram_cke(flags_old[1]),.dq_out(out_old),.dq_oe(flags_old[0]),.dq_in(dq_in));
    dorado_storage_sdram_burst #(.ADDR_BITS(26),.CLOCK_HZ(57142857),.CACHE_LINES(4),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1)) candidate(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(flags_new[14]),.req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(flags_new[13]),.rsp_ready(rsp_ready),.rsp_data(data_new),.initialized(flags_new[12]),.cache_hit(flags_new[11]),
        .physical_read(flags_new[10]),.physical_write(flags_new[9]),.read_return(flags_new[8]),.snapshot_idle(flags_new[15]),.burst_observation(obs_new),
        .sdram_a(a_new),.sdram_ba(ba_new),.sdram_ncs(flags_new[7]),.sdram_nras(flags_new[6]),.sdram_ncas(flags_new[5]),.sdram_nwe(flags_new[4]),
        .sdram_dqml(flags_new[3]),.sdram_dqmh(flags_new[2]),.sdram_cke(flags_new[1]),.dq_out(out_new),.dq_oe(flags_new[0]),.dq_in(dq_in));
    integer checks=0,responses=0,held_checks=0,early_changes=0,aborts=0;
    reg held=0;
    reg [15:0] held_data=0;
    reg [7:0] read_beats=0,abort_beats=0;
    always @(posedge clk)begin
        #1;
        if({flags_old,a_old,ba_old,out_old,obs_old} !== {flags_new,a_new,ba_new,out_new,obs_new})
            $fatal(1,"READ_CAPTURE control/pin/timing mismatch flags=%h/%h",flags_old,flags_new);
        if(flags_old[13] && data_old!==data_new)
            $fatal(1,"READ_CAPTURE valid data mismatch got=%h expected=%h",data_new,data_old);
        if(held && !reset && !rsp_ready && (!flags_new[13] || data_new!==held_data))
            $fatal(1,"READ_CAPTURE response changed under hold");
        held=flags_old[13] && !rsp_ready;held_data=data_old;
        if(held)held_checks++;
        if(flags_old[13])responses++;
        if(!flags_old[13] && data_old!==data_new)early_changes++;
        if(reference.state==6 && reference.delay_count==0 && !reference.aborted)
            read_beats[reference.address[2:0]]=1;
        checks++;
    end
    task automatic send(input bit writing,input reg[25:0] address,input reg[15:0] value);
        integer ticks;
        reg accepted;
        begin
            @(negedge clk);req_valid=1;req_write=writing;req_addr=address;req_data=value;ticks=0;accepted=0;
            while(!accepted)begin
                @(posedge clk);accepted=flags_old[14];
                @(negedge clk);ticks++;
                if(ticks>1000)$fatal(1,"READ_CAPTURE request timeout");
            end
            req_valid=0;
        end
    endtask
    task automatic operation(input bit writing,input reg[25:0] address,input integer hold_ticks);
        integer ticks;
        begin
            send(writing,address,16'hc150^address[15:0]);ticks=0;
            while(!flags_old[13])begin
                @(negedge clk);ticks++;
                if(ticks>1000)$fatal(1,"READ_CAPTURE response timeout");
            end
            repeat(hold_ticks)@(negedge clk);
            rsp_ready=1;@(negedge clk);rsp_ready=0;
        end
    endtask
    initial begin
        integer i,beat,ticks;
        repeat(5)@(negedge clk);reset=0;
        wait(flags_old[12]);
        // Every requested beat on a distinct cold line, including last beat.
        for(i=0;i<8;i++)operation(0,26'(i*16+i),3+i);
        // Both ranks/all banks, row changes, warm hits, invalidation and writes.
        for(i=0;i<128;i++)begin
            operation(i%5==2,(26'(i%8)<<23)|(26'((i/8)%4)<<10)|26'(i%8),i%7);
            operation(0,(26'(i%8)<<23)|(26'((i/8)%4)<<10)|26'((i+3)%8),i%4);
        end
        // Abort a miss at every physical beat without restarting the device.
        for(beat=0;beat<8;beat++)begin
            send(0,26'h40000+26'(beat*16+beat),0);ticks=0;
            while(!(reference.state==6 && reference.delay_count==0 && reference.fill_beat==beat))begin
                @(negedge clk);ticks++;
                if(ticks>1000)$fatal(1,"READ_CAPTURE abort position timeout");
            end
            reset=1;abort_beats[beat]=1;aborts++;
            @(negedge clk);reset=0;
            repeat(30)begin
                @(negedge clk);
                if(flags_old[13] || flags_new[13])$fatal(1,"READ_CAPTURE aborted response escaped");
            end
            operation(0,26'h40000+26'(beat*16+beat),beat+2);
        end
        // Long held response must not change refresh/command timing.
        operation(0,26'h40007,1600);
        if(read_beats!==8'hff || abort_beats!==8'hff || held_checks<1600 || early_changes<8 || aborts!=8)
            $fatal(1,"READ_CAPTURE coverage missing beats=%h aborts=%h holds=%0d early=%0d",read_beats,abort_beats,held_checks,early_changes);
        $display("READ_CAPTURE_MITER PASS checks=%0d valid_samples=%0d held=%0d invalid_data_changes=%0d beats=%h aborts=%0d",checks,responses,held_checks,early_changes,read_beats,aborts);
        $finish;
    end
    initial begin #10000000;$fatal(1,"READ_CAPTURE overall timeout");end
endmodule
