`timescale 1ns/1ps
module tb;
    // Generated fixture declarations and physical-field mask follow here.
    @DECLARATIONS@
    localparam integer STATE_BITS=1538,SCAN_WORDS=86;
    localparam [1537:0] FF_MASK=1538'h@FF_MASK@;
    localparam integer canonical_bit[1538]='{@CANONICAL_BITS@};
    initial clk=0;
    always #10 clk=~clk;
    assign ref_clk=clk;
    msa_board_fixture dut(.*);
    wire [255:0] address_bus,data_bus,response_bus;
    wire rv,rr,rw,sv,sr;
    wire [22:0] ra;
    wire [15:0] wd,sd;
    for(genvar g=0;g<16;g=g+1)begin
        assign address_bus[g*16+:16]=req_addr[g];
        assign data_bus[g*16+:16]=req_data[g];
        assign rsp_data[g]=response_bus[g*16+:16];
    end
    dorado_storage_arb #(.GROUPS(16),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(23),
        .PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1)) arbiter(
        .clk(clk),.reset(reset),.snapshot_idle(arbiter_idle),
        .req_valid(req_valid),.req_write(req_write),.req_ready(req_ready),
        .req_addr(address_bus),.req_data(data_bus),.rsp_valid(rsp_valid),
        .rsp_ready(rsp_ready),.rsp_data(response_bus),
        .raw_req_valid(rv),.raw_req_write(rw),.raw_req_ready(rr),
        .raw_req_addr(ra),.raw_req_data(wd),.raw_rsp_valid(sv),
        .raw_rsp_ready(sr),.raw_rsp_data(sd));
    wire initialized,cache_hit,physical_read,physical_write,read_return;
    wire [12:0] sdram_a;
    wire [1:0] sdram_ba;
    wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe;
    wire [15:0] dq_out,dq;
    assign dq=dq_oe?dq_out:16'hzzzz;
    dorado_storage_sdram #(.ADDR_BITS(23),.CLOCK_HZ(50000000)) memory(
        .clk(clk),.reset(reset),.snapshot_idle(raw_service_idle),
        .req_valid(rv),.req_ready(rr),.req_write(rw),.req_addr(ra),.req_data(wd),
        .rsp_valid(sv),.rsp_ready(sr),.rsp_data(sd),.initialized(initialized),
        .cache_hit(cache_hit),.physical_read(physical_read),.physical_write(physical_write),
        .read_return(read_return),.sdram_a(sdram_a),.sdram_ba(sdram_ba),
        .sdram_ncs(ncs),.sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
        .sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),
        .dq_out(dq_out),.dq_oe(dq_oe),.dq_in(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("PAUSE_RANK0"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
        .Clk(~clk),.Cke(cke),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(sdram_ba),.Addr(sdram_a),.Ldqm(dqml),.Udqm(dqmh),.Dq(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("PAUSE_RANK1"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
        .Clk(~clk),.Cke(cke),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(sdram_ba),.Addr(sdram_a),.Ldqm(dqml),.Udqm(dqmh),.Dq(dq));
    integer checks=0,host_edges=0,guest_edges=0,raw_reads=0,raw_writes=0;
    integer refreshes=0,pauses=0,pending_reads=0,pending_writes=0;
    bit raw_pending=0,pending_write=0;
    reg [31:0] rng=32'h5bd4af31;
    always @(posedge clk)begin
        host_edges++;
        if(advance)guest_edges++;
        if(reset)raw_pending=0;
        else begin
            if(rv&&rr)begin
                raw_pending=1;pending_write=rw;
                if(rw)raw_writes++;else raw_reads++;
            end
            if(sv&&sr)raw_pending=0;
        end
    end
    always @(negedge clk)if({nras,ncas,nwe}==3'b001)refreshes++;
    task check(input bit ok,input string message);
        checks++;if(!ok)$fatal(1,"%s host=%0d",message,host_edges);
    endtask
    task compare;
        check(dut_state===reference_state,"ordinary physical MSA state differs");
        check(dut_outputs===reference_outputs,"ordinary MSA pins differ");
        check(req_valid===ref_req_valid&&req_write===ref_req_write&&
              rsp_ready===ref_rsp_ready,"ordinary MSA raw controls differ");
        check(req_addr===ref_req_addr&&req_data===ref_req_data,"ordinary MSA raw payload differs");
    endtask
    task tick(input bit guest,input bit ordinary);
        @(negedge clk);advance=guest;
        check(!guest||(!pause_guest_hold&&storage_ready&&ref_storage_ready),"advanced held/stalled guest");
        @(posedge clk);#1;advance=0;
        if(ordinary)compare();
    endtask
    task settle;
        integer age;
        age=0;
        while(!storage_ready||!ref_storage_ready)begin
            tick(0,1);age++;check(age<20000,"normal raw service timeout");
        end
    endtask
    task step;
        @(negedge clk);
        for(integer b=0;b<$bits(stimulus);b++)begin
            rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);stimulus[b]=rng[0];
        end
        settle();tick(1,1);
    endtask
    function automatic [17:0] saved_word(input [1537:0] saved,input integer index);
        reg [17:0] value;
        value=0;
        for(integer b=0;b<18&&index*18+b<STATE_BITS;b++)value[b]=saved[canonical_bit[index*18+b]];
        return value;
    endfunction
    task pause_with_pending(input bit writing);
        integer search,age,held_guest,refresh_before;
        reg [1537:0] before_ff,saved;
        bit found;
        found=0;
        for(search=0;search<4000&&!found;search++)begin
            step();age=0;
            while(age<1000&&!found)begin
                tick(0,1);age++;
                found=raw_pending&&pending_write==writing;
                if(storage_ready&&ref_storage_ready)break;
            end
        end
        check(found,"pending read/write pause coverage missing");
        if(writing)pending_writes++;else pending_reads++;
        before_ff=dut_state;held_guest=guest_edges;
        @(negedge clk);global_hold=1;freeze=1;held=1;
        age=0;
        while(!pause_ready)begin
            tick(0,0);age++;
            check((dut_state&FF_MASK)===(before_ff&FF_MASK),"FF moved while real raw service drained");
            if(!pause_frontend_hold)compare();
            check(!pause_frontend_hold||(!raw_pending&&arbiter_idle&&raw_service_idle),"froze real raw work early");
            check(age<10000,"actual raw response deadlocked behind hold");
        end
        check(guest_edges==held_guest&&!raw_pending&&!pause_fault,"invalid real frozen boundary");
        saved=dut_state;refresh_before=refreshes;
        for(integer i=0;i<SCAN_WORDS;i++)begin
            check(ready,"scan not ready");
            @(negedge clk);state_index=24'(i);write=0;request=1;
            tick(0,0);request=0;age=0;
            while(!done)begin
                check(pause_frontend_hold&&pause_ready,"rotation lost real raw service hold");
                tick(0,0);age++;check(age<1600,"real raw-service scan timeout");
            end
            check(!error&&result===saved_word(saved,i),"real raw-service snapshot word differs");
            check(dut_state===saved,"real raw-service rotation changed state");
        end
        check(refreshes>refresh_before+20,"physical SDRAM refresh stopped while held");
        @(negedge clk);global_hold=0;freeze=0;held=0;#1;
        check(pause_guest_hold,"guest hold released before frontend thaw");
        tick(0,0);check(!pause_guest_hold&&!pause_frontend_hold&&!pause_fault,"real pause failed to thaw");
        compare();for(integer n=0;n<100;n++)step();settle();pauses++;
    endtask
    initial begin
        advance=0;freeze=0;held=0;global_hold=0;global_abort=0;system_reset=0;
        request=0;write=0;reset=1;drain_mode=1;state_index=0;state_data=0;stimulus=0;
        repeat(3)tick(0,0);reset=0;settle();
        for(integer n=0;n<2000;n++)step();settle();
        for(integer p=0;p<4;p++)pause_with_pending(p[0]);
        for(integer n=0;n<2000;n++)step();settle();
        check(chip0.error_count==0&&chip1.error_count==0&&
              chip0.warning_count==0&&chip1.warning_count==0,"SDRAM pin-model errors/warnings");
        check(raw_reads>0&&raw_writes>0&&pending_reads>0&&pending_writes>0,"missing actual raw traffic");
        $display("MSA SDRAM PAUSE PASS checks=%0d host=%0d guest=%0d reads=%0d writes=%0d pauses=%0d pending_reads=%0d pending_writes=%0d refreshes=%0d",checks,host_edges,guest_edges,raw_reads,raw_writes,pauses,pending_reads,pending_writes,refreshes);
        $finish;
    end
    initial begin #100000000;$fatal(1,"combined memory pause test deadline");end
endmodule
