// Independent admission/issue/completion scoreboard and delayed raw memory.
// This is a queue protocol test, not an SDRAM timing or CPU boot fixture.
`timescale 1ns/1ps
module tb_storage_queue #(parameter integer GROUPS=8,DEPTH=4,GROUP_ADDR_BITS=4);
    localparam GA=GROUP_ADDR_BITS,GB=GROUPS<=1 ? 0 : $clog2(GROUPS),RA=GA+GB,WORDS=1<<RA;
    reg clk=0;always #5 clk=~clk;
    reg reset=1,enable=0;
    reg [GROUPS-1:0] valid=0,writing=0,response_ready=0;
    reg [GROUPS*GA-1:0] addresses=0;
    reg [GROUPS*16-1:0] data=0;
    wire [GROUPS-1:0] ready,response;
    wire [GROUPS*16-1:0] response_data;
    wire backend_reset,draining,idle,rv,rr,rw,sr;
    wire [RA-1:0] ra;
    wire [15:0] wd;
    reg sv=0;
    reg [15:0] sd=0;
    dorado_storage_queue #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(GA),.RAW_ADDR_BITS(RA),
        .DEPTH(DEPTH)) dut(.clk(clk),.reset(reset),.enable(enable),
        .backend_reset(backend_reset),.reset_draining(draining),.idle(idle),
        .req_valid(valid),.req_write(writing),.req_ready(ready),.req_addr(addresses),.req_data(data),
        .rsp_valid(response),.rsp_ready(response_ready),.rsp_data(response_data),
        .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
    reg [15:0] physical[0:WORDS-1],oracle[0:WORDS-1];
    reg raw_busy=0,raw_writing=0,raw_admit=1;
    reg [RA-1:0] raw_address=0;
    reg [15:0] raw_data=0;
    integer remaining=0,physical_writes=0;
    assign rr=!backend_reset && raw_admit && (!raw_busy || (sv && sr));
    always @(posedge clk)begin
        if(backend_reset)begin
            if(raw_busy)$fatal(1,"QUEUE_TEST backend reset before physical drain");
            sv<=0;raw_busy<=0;
        end else begin
            if(sv && sr)begin sv<=0;raw_busy<=0;end
            if(raw_busy && !sv)begin
                if(remaining==0)begin
                    sd<=physical[raw_address];sv<=1;
                    if(raw_writing)begin physical[raw_address]<=raw_data;physical_writes++;end
                end else remaining<=remaining-1;
            end
            if(rv && rr)begin
                raw_busy<=1;sv<=0;raw_address<=ra;raw_data<=wd;raw_writing<=rw;
                remaining<=1+integer'(ra[1:0]);
            end
        end
    end
    integer ticks=0,admitted=0,issued=0,retired=0,front_completed=0,discarded=0;
    integer raw_handoffs=0,maximum_owned=0,full_ticks=0,held_ticks=0,reset_drains=0;
    integer golden_group[0:4095],golden_address[0:4095];
    reg golden_write[0:4095];
    reg [15:0] golden_data[0:4095],golden_read[0:4095];
    reg [GROUPS-1:0] front_pending=0,accepted_mask=0,held=0,seen=0;
    reg [GROUPS*16-1:0] held_data=0;
    integer offer_age[0:GROUPS-1];
    reg old_backend_reset=0;
    reg [GA-1:0] address_ones=0,address_zeros=0;
    always @(posedge clk)begin
        ticks++;accepted_mask=0;
        if(dut.owned>maximum_owned)maximum_owned=dut.owned;
        if(dut.owned==DEPTH)full_ticks++;
        if(backend_reset && !old_backend_reset)reset_drains++;
        old_backend_reset=backend_reset;
        if(reset || draining)begin
            if((|ready) || (|response))$fatal(1,"QUEUE_TEST stale frontend epoch");
            front_pending=0;held=0;
        end
        for(integer g=0;g<GROUPS;g++)begin
            if(!reset && !draining)begin
                if(held[g] && (!response[g] || response_data[16*g+:16]!==held_data[16*g+:16]))
                    $fatal(1,"QUEUE_TEST held response changed");
                if(response[g] && (!front_pending[g] || g!=golden_group[retired]))
                    $fatal(1,"QUEUE_TEST wrong response owner");
                if(response[g] && response_ready[g])begin front_pending[g]=0;front_completed++;end
                held[g]=response[g] && !response_ready[g];
                held_data[16*g+:16]=response_data[16*g+:16];
                if(held[g])held_ticks++;
            end
            if(!valid[g] || ready[g] || reset || draining || !enable)offer_age[g]=0;
            else offer_age[g]++;
            if(offer_age[g]>(GROUPS*16+128>1024 ? GROUPS*16+128 : 1024))
                $fatal(1,"QUEUE_TEST admission deadline");
        end
        // Old raw completion precedes replacement on a simultaneous handoff.
        if(sv && sr)begin
            if(retired>=issued)$fatal(1,"QUEUE_TEST orphan raw completion");
            if(!golden_write[retired] && sd!==golden_read[retired])
                $fatal(1,"QUEUE_TEST read/order got%h expected%h",sd,golden_read[retired]);
            if(reset || draining)discarded++;
            retired++;
        end
        if(rv && rr)begin
            if(issued>=admitted || ra!==RA'(golden_address[issued]) ||
               rw!==golden_write[issued] || wd!==golden_data[issued])
                $fatal(1,"QUEUE_TEST raw payload/order changed");
            issued++;
            if(sv && sr)raw_handoffs++;
        end
        for(integer g=0;g<GROUPS;g++)if(valid[g] && ready[g])begin
            if(front_pending[g])$fatal(1,"QUEUE_TEST accepted duplicate group");
            if(admitted==4096)$fatal(1,"QUEUE_TEST oracle full");
            golden_group[admitted]=g;
            golden_address[admitted]=(integer'(addresses[GA*g+:GA])<<GB)|g;
            golden_write[admitted]=writing[g];golden_data[admitted]=data[16*g+:16];
            address_ones=address_ones | addresses[GA*g+:GA];
            address_zeros=address_zeros | ~addresses[GA*g+:GA];
            golden_read[admitted]=oracle[golden_address[admitted]];
            if(writing[g])oracle[golden_address[admitted]]=data[16*g+:16];
            admitted++;front_pending[g]=1;accepted_mask[g]=1;seen[g]=1;
        end
        if(admitted-retired!=dut.owned+integer'(|accepted_mask)-integer'(sv && sr))begin
            // owned updates after this edge; compare its pre-edge value with
            // the independently counted admissions and raw completions.
            $fatal(1,"QUEUE_TEST total ownership disagrees");
        end
    end
    task automatic drain;
        integer count;
        begin
            count=0;
            while(!idle)begin
                @(negedge clk);count++;
                if(count>2000)$fatal(1,"QUEUE_TEST reset drain timeout");
            end
            if(admitted!=issued || issued!=retired)$fatal(1,"QUEUE_TEST accepted work disappeared");
            for(integer a=0;a<WORDS;a++)
                if(physical[a]!==oracle[a])$fatal(1,"QUEUE_TEST accepted write lost");
        end
    endtask
    initial begin
        reg [31:0] random_state;
        integer target,produced=0;
        for(integer a=0;a<WORDS;a++)begin physical[a]=0;oracle[a]=0;end
        for(integer g=0;g<GROUPS;g++)offer_age[g]=0;
        repeat(5)@(negedge clk);reset=0;enable=1;
        wait(!draining);@(negedge clk);
        // Hold a second request from the same group while the accepted write
        // is outstanding. Saved payload must survive the live input change.
        valid=1;writing='1;data[15:0]=16'hab00;addresses[GA-1:0]=0;response_ready=0;
        while(admitted==0)@(negedge clk);
        data[15:0]=16'heeee;addresses[GA-1:0]=1;
        repeat(12)@(negedge clk);
        reset=1;valid=0;repeat(2)@(negedge clk);reset=0;drain();
        if(physical[0]!==16'hab00)$fatal(1,"QUEUE_TEST payload was not retained");
        // Fill the entire ownership window, then reset with queued writes
        // behind a held raw reply. Reset must force that reply to drain.
        @(negedge clk);valid='1;writing='1;response_ready=0;
        for(integer g=0;g<GROUPS;g++)begin addresses[GA*g+:GA]=2;data[16*g+:16]=16'hbc00^16'(g);end
        target=DEPTH<GROUPS ? DEPTH : GROUPS;
        while(dut.owned<target)begin @(negedge clk);valid=valid & ~accepted_mask;end
        repeat(12)@(negedge clk);
        reset=1;valid=0;repeat(30)@(negedge clk);reset=0;drain();
        // Pinned independent request arrivals, raw stalls and response holds.
        random_state=32'h619a3d75;
        for(integer t=0;t<900;t++)begin
            @(negedge clk);valid=valid & ~accepted_mask;
            raw_admit=t%7>=2;enable=t%61<51;
            response_ready={GROUPS{t%37<23}};
            random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
            for(integer g=0;g<GROUPS;g++)if(!valid[g] && !front_pending[g] && random_state[g%32])begin
                valid[g]=1;writing[g]=random_state[(g+9)%32];
                addresses[GA*g+:GA]=GA'(random_state>>(g%8));
                data[16*g+:16]=16'(random_state)^16'(g*251);produced++;
            end
            if(t==200 || t==600)begin
                reset=1;valid=0;response_ready=0;
                repeat(3)@(negedge clk);reset=0;drain();
            end
        end
        enable=1;raw_admit=1;response_ready='1;
        while((|valid) || !idle)begin @(negedge clk);valid=valid & ~accepted_mask;end
        drain();
        if(maximum_owned!=target || full_ticks<10 || held_ticks<10 || reset_drains<4 ||
           admitted<80 || physical_writes<20 || seen!=={GROUPS{1'b1}} ||
           address_ones!=={GA{1'b1}} || address_zeros!=={GA{1'b1}} ||
           (DEPTH>1 && raw_handoffs<8) || front_completed+discarded!=retired)
            $fatal(1,"QUEUE_TEST coverage missing admitted=%0d max=%0d held=%0d handoffs=%0d",admitted,maximum_owned,held_ticks,raw_handoffs);
        $display("STORAGE_QUEUE PASS groups=%0d depth=%0d address_bits=%0d admitted=%0d issued=%0d retired=%0d front=%0d discarded=%0d writes=%0d max=%0d full=%0d held=%0d handoffs=%0d resets=%0d",GROUPS,DEPTH,GA,admitted,issued,retired,front_completed,discarded,physical_writes,maximum_owned,full_ticks,held_ticks,raw_handoffs,reset_drains);
        $finish;
    end
    initial begin #1000000;$fatal(1,"QUEUE_TEST overall deadline");end
endmodule
