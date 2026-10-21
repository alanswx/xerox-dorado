// Exercise the actual power-on sequencer through BL8 and independent chip pins.
`timescale 1ns/1ps
module tb_storage_selftest #(
    parameter integer ADDR_BITS=14,
    parameter integer CLOCK_HZ=50000000
);
    localparam integer CELLS=1<<ADDR_BITS;
    reg clk=0;always #(500000000.0/CLOCK_HZ)clk=~clk;
    reg reset=1;
    wire ready,failed,valid,accept,writing,response,response_ready;
    wire [ADDR_BITS-1:0] address;
    wire [15:0] value,returned;
    wire [127:0] observation;
    wire [2:0] phase=observation[5:3];
`ifdef DORADO_BURST_OBSERVER
    wire [127:0] burst_observation;
    task automatic check_first_burst;
        if(burst_observation[127:96]!==32'hb1780001)
            $fatal(1,"SELFTEST: first burst marker/valid");
        for(int lane=0;lane<8;lane++)begin
            if(burst_observation[12*lane+:12]!==12'(lane^'ha59^(fault==2 ? 128 : 0)))
                $fatal(1,"SELFTEST: first burst lane=%d data=%h",lane,burst_observation);
        end
    endtask
`endif
    int fault=0;
    initial if($value$plusargs("fault=%d",fault))begin end
    wire corrupt=(fault==1 && phase==1 && address==ADDR_BITS'('h1234)) ||
        (fault==4 && phase==5 && address==ADDR_BITS'('h1234));
    dorado_storage_selftest #(.ADDR_BITS(ADDR_BITS)) test(
        .clk(clk),.reset(reset),.ready(ready),.failed(failed),
        .req_valid(valid),.req_ready(accept),.req_write(writing),
        .req_addr(address),.req_data(value),.rsp_valid(response),
        .rsp_ready(response_ready),.rsp_data(returned^{15'b0,corrupt}),
        .observation(observation));
    wire backend_ready,backend_response,backend_rsp_ready;
    wire [12:0] a;
    wire [1:0] ba;
    wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe,initialized,cache_hit;
    wire [15:0] dq,dq_out;
    wire chip_clk=~clk;
    assign dq=dq_oe ? dq_out : 16'hzzzz;
    int ticks=0;
    always @(posedge clk)ticks<=ticks+1;
    wire gate_request=(ticks%11)!=3;
    wire gate_response=(ticks%7)!=2;
    assign accept=backend_ready && gate_request;
    assign response=backend_response && gate_response;
    assign backend_rsp_ready=response_ready && gate_response;
    wire [ADDR_BITS-1:0] physical_address=fault==2 ? address & ~ADDR_BITS'(128) :
        fault==3 ? address & ~ADDR_BITS'(8192) : address;
    dorado_storage_sdram_burst #(.ADDR_BITS(ADDR_BITS),.CLOCK_HZ(CLOCK_HZ)) backend(
        .clk(clk),.reset(reset),.req_valid(valid && gate_request),
        .req_ready(backend_ready),.req_write(writing),.req_addr(physical_address),
        .req_data(value),.rsp_valid(backend_response),.rsp_ready(backend_rsp_ready),
        .rsp_data(returned),.initialized(initialized),.cache_hit(cache_hit),
`ifdef DORADO_BURST_OBSERVER
        .burst_observation(burst_observation),
`endif
        .sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),.sdram_nras(nras),
        .sdram_ncas(ncas),.sdram_nwe(nwe),.sdram_dqml(dqml),.sdram_dqmh(dqmh),
        .sdram_cke(cke),.dq_out(dq_out),.dq_oe(dq_oe),.dq_in(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("SELFTEST_RANK0"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),
        .We_n(nwe),.Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("SELFTEST_RANK1"),
        .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),
        .We_n(nwe),.Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    int requests=0,completions=0,outstanding=0,reads=0,writes=0,hits=0;
    int counts[6]='{default:0};
    int refreshes[2]='{0,0};
    // Independent transaction-order oracle, not a peek into sequencer state.
    always @(posedge clk)if(!reset)begin
        if(valid && accept)begin
            int p,offset,expected_address;
            reg [15:0] expected_data;
            p=requests/CELLS;offset=requests%CELLS;
            expected_address=p==3 ? CELLS-1-offset : offset;
            expected_data=p<2 ? 16'((expected_address & 4095)^'ha59) :
                p<4 ? 16'((expected_address>>12)^'h5a6) : 16'd0;
            if(p>5 || address!==ADDR_BITS'(expected_address) ||
               writing!==(p==0 || p==2 || p==4) || value!==expected_data)
                $fatal(1,"SELFTEST: request order/data %d %h %h",requests,address,value);
            if(outstanding!=0)$fatal(1,"SELFTEST: multiple outstanding");
            counts[p]++;requests++;outstanding++;
            if(writing)writes++;else reads++;
        end
        if(response && response_ready)begin
            if(outstanding!=1)$fatal(1,"SELFTEST: orphan completion");
`ifdef DORADO_BURST_OBSERVER
            if(phase==1 && address==0)check_first_burst();
`endif
            completions++;outstanding--;
        end
        if(cache_hit)hits++;
    end
    always @(posedge chip_clk)if({nras,ncas,nwe}==3'b001)refreshes[ncs]++;
    initial begin
        reg [127:0] saved;
        int r0,r1;
        repeat(5)@(negedge clk);reset=0;
        wait(ready || failed);@(negedge clk);
        if(observation[31:16]!==16'hb157 || outstanding!=0)
            $fatal(1,"SELFTEST: marker/drain");
        if(fault==0)begin
            if(!ready || failed || requests!=6*CELLS || completions!=requests ||
               writes!=3*CELLS || reads!=3*CELLS || hits<2*CELLS)
                $fatal(1,"SELFTEST: incomplete pass %d %d %d",requests,completions,hits);
            for(int p=0;p<6;p++)if(counts[p]!=CELLS)$fatal(1,"SELFTEST: phase omitted");
        end else begin
            int ep,ea,ee,ev;
            ep=fault==3 ? 3 : fault==4 ? 5 : 1;
            ea=fault==2 ? 0 : fault==3 ? 8191 : 'h1234;
            ee=ep==1 ? ((ea & 4095)^'ha59) : ep==3 ? ((ea>>12)^'h5a6) : 0;
            ev=ee^(fault==2 ? 128 : fault==3 ? 2 : 1);
            if(!failed || ready || observation[95:64]!==32'(ea) ||
               observation[127:125]!==3'(ep) || observation[124:113]!==12'(ee) ||
               observation[112:101]!==12'(ev))
                $fatal(1,"SELFTEST: first failure mismatch %h phase=%d addr=%h exp=%h got=%h",observation,ep,ea,ee,ev);
        end
`ifdef DORADO_BURST_OBSERVER
        check_first_burst();
`endif
        saved=observation;r0=refreshes[0];r1=refreshes[1];
        repeat(10000)begin
            @(negedge clk);
            if(valid || response_ready || observation!==saved)
                $fatal(1,"SELFTEST: terminal state changed");
        end
        if(refreshes[0]<=r0 || refreshes[1]<=r1)$fatal(1,"SELFTEST: stopped refresh");
        if(chip0.error_count || chip1.error_count || chip0.warning_count || chip1.warning_count)
            $fatal(1,"SELFTEST: physical model errors");
        $display("STORAGE-SELFTEST PASS fault=%0d bits=%0d hz=%0d requests=%0d completions=%0d hits=%0d",fault,ADDR_BITS,CLOCK_HZ,requests,completions,hits);
        $finish;
    end
    initial begin #(200000000.0*(CELLS/16384.0));$fatal(1,"SELFTEST: deadline");end
endmodule
