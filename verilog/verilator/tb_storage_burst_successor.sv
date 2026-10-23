// Raw-port protocol/ordering test. The small pin-driven memory below is a
// functional test double, not the independently maintained XSDS timing model.
// That model and original DRAM-array comparison are separate required gates.
`timescale 1ns/1ps
module tb_storage_burst_successor #(
    parameter integer ENABLED=1,
    parameter integer CLOCK_HZ=57142857
);
    reg clk=0;
    always #(500000000.0/CLOCK_HZ) clk=~clk;
    reg reset=1,req_valid=0,req_write=0,rsp_ready=0;
    reg [25:0] req_addr=0;
    reg [15:0] req_data=0,chip_data=0;
    wire req_ready,rsp_valid,initialized;
    wire [15:0] rsp_data,dq_out;
    wire [12:0] a;
    wire [1:0] ba;
    wire ncs,nras,ncas,nwe,dq_oe;
    wire [2:0] command={nras,ncas,nwe};
    dorado_storage_sdram_burst #(.ADDR_BITS(26),.CLOCK_HZ(CLOCK_HZ),
        .RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(1),
        .BURST_READ_SUCCESSOR(ENABLED)) dut(
        .clk(clk),.reset(reset),.req_valid(req_valid),.req_ready(req_ready),
        .req_write(req_write),.req_addr(req_addr),.req_data(req_data),
        .rsp_valid(rsp_valid),.rsp_ready(rsp_ready),.rsp_data(rsp_data),
        .initialized(initialized),.sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),
        .sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
        .dq_out(dq_out),.dq_oe(dq_oe),.dq_in(chip_data));

    function automatic [15:0] pattern(input reg [25:0] address);
        pattern=address[15:0] ^ {6'b0,address[25:16]} ^ 16'ha57b;
    endfunction
    reg [25:0] physical_address[0:255],logical_address[0:255];
    reg [15:0] physical_value[0:255],logical_value[0:255];
    integer physical_used=0,logical_used=0;
    function automatic [15:0] physical_read(input reg [25:0] address);
        reg [15:0] value;
        begin
            value=pattern(address);
            for(integer j=0;j<physical_used;j++)
                if(physical_address[j]==address)value=physical_value[j];
            physical_read=value;
        end
    endfunction
    function automatic [15:0] logical_read(input reg [25:0] address);
        reg [15:0] value;
        begin
            value=pattern(address);
            for(integer j=0;j<logical_used;j++)
                if(logical_address[j]==address)value=logical_value[j];
            logical_read=value;
        end
    endfunction
    // Decode actual SDRAM command/address pins. CAS2 launches on the first
    // elapsed chip edge for sampling at the second, with6ns tAC. The physical
    // XSDS/vendor comparison remains the independent timing qualification.
    reg [12:0] open_row[0:7];
    reg [25:0] read_base=0;
    integer read_delay=0,read_beat=8,physical_reads=0,physical_writes=0;
    integer refresh0=0,refresh1=0;
    always @(negedge clk)begin:chip
        reg [25:0] address;
        if(read_delay!=0)read_delay=read_delay-1;
        else if(read_beat<8)begin
            address={read_base[25:3],3'(read_base[2:0]+read_beat)};
            chip_data<=#6 physical_read(address);
            read_beat=read_beat+1;
        end
        case(command)
        3'b011:open_row[{ncs,ba}]=a;
        3'b101:begin
            read_base={ncs,ba,open_row[{ncs,ba}],a[9:0]};
            read_delay=0;read_beat=0;physical_reads++;
        end
        3'b100:begin
            if(!dq_oe || physical_used==256)$fatal(1,"SUCCESSOR physical write invalid");
            physical_address[physical_used]={ncs,ba,open_row[{ncs,ba}],a[9:0]};
            physical_value[physical_used]=dq_out;physical_used++;physical_writes++;
        end
        3'b001:if(ncs)refresh1++;else refresh0++;
        default:begin end
        endcase
    end

    integer ticks=0,accepted=0,completed=0,aborted_requests=0,forwarded=0,held_ticks=0,due_blocks=0;
    reg pending=0,pending_write=0,held=0;
    reg [15:0] expected=0,held_value=0;
    reg [7:0] first_beats=0,forward_beats=0,abort_beats=0;
    reg [4:0] excluded=0;
    always @(posedge clk)begin
        ticks++;
        if(reset)begin if(pending)aborted_requests++;pending=0;held=0;end
        else begin
            if(held && (!rsp_valid || rsp_data!==held_value))
                $fatal(1,"SUCCESSOR response changed under backpressure");
            if(rsp_valid && rsp_ready)begin
                if(!pending)$fatal(1,"SUCCESSOR orphan or duplicate response");
                if(!pending_write && rsp_data!==expected)
                    $fatal(1,"SUCCESSOR data/order got=%h expected=%h",rsp_data,expected);
                pending=0;completed++;
            end
            held=rsp_valid && !rsp_ready;held_value=rsp_data;
            if(held)held_ticks++;
            if(req_valid && dut.state==6 && dut.refresh_due && !req_ready)due_blocks++;
            if(req_valid && req_ready)begin
                if(dut.refresh_due)$fatal(1,"SUCCESSOR accepted while refresh due");
                if(pending)$fatal(1,"SUCCESSOR overwrote outstanding request");
                if(dut.state==6)begin
                    if(!ENABLED || req_write || !rsp_valid || !rsp_ready ||
                       dut.aborted || dut.refresh_due ||
                       req_addr[25:3]!=dut.address[25:3] || req_addr[2:0]!=dut.fill_beat)
                        $fatal(1,"SUCCESSOR accepted forbidden in-burst request");
                    forwarded++;forward_beats[req_addr[2:0]]=1;
                end
                pending=1;pending_write=req_write;expected=logical_read(req_addr);
                if(req_write)begin
                    if(logical_used==256)$fatal(1,"SUCCESSOR logical write table full");
                    logical_address[logical_used]=req_addr;
                    logical_value[logical_used]=req_data;logical_used++;
                end
                accepted++;
            end
        end
    end
    task automatic offer(input bit writing,input reg[25:0] address,input reg[15:0] value);
        reg sent;
        begin
            @(negedge clk);req_valid=1;req_write=writing;req_addr=address;req_data=value;
            sent=0;
            while(!sent)begin @(posedge clk);sent=req_ready;@(negedge clk);end
            req_valid=0;
        end
    endtask
    task automatic operation(input bit writing,input reg[25:0] address,input integer hold);
        begin
            rsp_ready=0;offer(writing,address,16'hb910^address[15:0]);
            while(!rsp_valid)@(negedge clk);
            repeat(hold)@(negedge clk);
            rsp_ready=1;@(negedge clk);rsp_ready=0;
        end
    endtask
    task automatic chain(input reg[25:0] base,input integer first,stall,abort_at);
        integer sent,received,local_ticks;
        begin:run_chain
            sent=0;received=0;local_ticks=0;
            @(negedge clk);
            while(received<8)begin
                if(abort_at>=0 && dut.state==6 && dut.delay_count==0 && dut.fill_beat==abort_at)begin
                    reset=1;req_valid=0;rsp_ready=0;abort_beats[abort_at]=1;
                    repeat(2)@(negedge clk);reset=0;
                    repeat(30)begin @(negedge clk);if(rsp_valid)$fatal(1,"SUCCESSOR stale reset reply");end
                    disable run_chain;
                end
                req_valid=sent<8;req_write=0;req_addr=base+26'((first+sent)&7);
                rsp_ready=stall==0 || local_ticks%11<6;
                @(posedge clk);
                if(rsp_valid && rsp_ready)received++;
                if(req_valid && req_ready)sent++;
                local_ticks++;
                if(local_ticks>2000)$fatal(1,"SUCCESSOR chain deadline");
                @(negedge clk);
            end
            req_valid=0;rsp_ready=0;
            if(abort_at>=0)$fatal(1,"SUCCESSOR abort not exercised");
            first_beats[first]=1;
        end
    endtask
    task automatic exclusion(input reg[25:0] base,input integer kind);
        begin
            rsp_ready=0;offer(0,base,0);while(!rsp_valid)@(negedge clk);
            req_valid=1;req_write=kind==0;
            case(kind)
            0:req_addr=base+1;
            1:req_addr=base+8+26'(dut.fill_beat);
            2:req_addr=base+1024+26'(dut.fill_beat);
            3:req_addr=(base^26'h2000000)+26'(dut.fill_beat);
            4:req_addr=base+26'((dut.fill_beat+1)&7);
            endcase
            rsp_ready=1;
            @(posedge clk);
            if(dut.state!=6 || req_ready)$fatal(1,"SUCCESSOR exclusion failed kind=%0d",kind);
            @(negedge clk);req_valid=0;req_write=0;rsp_ready=0;excluded[kind]=1;
        end
    endtask
    initial begin
        integer owner,first,kind,beat,r0,r1;
        repeat(5)@(negedge clk);reset=0;wait(initialized);
        for(owner=0;owner<8;owner++)for(first=0;first<8;first++)begin
            chain((26'(owner)<<23)+26'h20000+26'(first*16),first,first%2,-1);
        end
        for(kind=0;kind<5;kind++)exclusion(26'h40000+26'(kind*16),kind);
        for(first=0;first<32;first++)begin
            operation(1,26'h50000+26'(first%8),first%3);
            operation(0,26'h50000+26'((first+1)%8),first%4);
            operation(0,26'h50000+26'(first%8),0);
        end
        for(beat=0;beat<8;beat++)chain(26'h60000+26'(beat*16),0,0,beat);
        // Start just ahead of the natural refresh threshold and offer the
        // arriving beat continuously. Admission must stop when refresh is due.
        while(dut.refresh_count!=dut.REFRESH_CLOCKS-9)@(negedge clk);
        rsp_ready=0;offer(0,26'h70000,0);while(!rsp_valid)@(negedge clk);
        while(dut.state==6)begin
            req_valid=1;req_write=0;req_addr=26'h70000+26'(dut.fill_beat);rsp_ready=1;
            @(negedge clk);
        end
        req_valid=0;while(pending)@(negedge clk);rsp_ready=0;
        operation(0,26'h70000,0);
        rsp_ready=0;offer(0,26'h70001,0);while(!rsp_valid)@(negedge clk);
        r0=refresh0;r1=refresh1;
        repeat(5000)@(negedge clk);
        if(refresh0<r0+10 || refresh1<r1+10)$fatal(1,"SUCCESSOR refresh starved");
        rsp_ready=1;@(negedge clk);rsp_ready=0;
        if(first_beats!==8'hff || abort_beats!==8'hff || excluded!==5'h1f ||
           held_ticks<5000 || due_blocks==0 || physical_writes!=32 || pending ||
           accepted!=completed+aborted_requests ||
           (ENABLED && forward_beats!==8'hfe) || (!ENABLED && forwarded!=0))
            $fatal(1,"SUCCESSOR coverage missing first=%h abort=%h excluded=%h forward=%h due=%0d writes=%0d",first_beats,abort_beats,excluded,forward_beats,due_blocks,physical_writes);
        $display("BURST_SUCCESSOR PASS enabled=%0d hz=%0d ticks=%0d accepted=%0d completed=%0d forwarded=%0d held=%0d due_blocks=%0d physical_reads=%0d writes=%0d",ENABLED,CLOCK_HZ,ticks,accepted,completed,forwarded,held_ticks,due_blocks,physical_reads,physical_writes);
        $finish;
    end
    initial begin #2000000;$fatal(1,"SUCCESSOR overall deadline");end
endmodule
