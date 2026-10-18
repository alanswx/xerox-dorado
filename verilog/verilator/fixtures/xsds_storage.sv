// Pin-level storage fixture; unwritten cells read zero, matching the board
// equivalence fixture's deterministic power-up, not a hardware assumption.
`timescale 1ns/1ps
module tb_xsds_storage #(parameter integer ADDR_BITS=20)(
    input wire clk,reset,req_valid,req_write,rsp_ready,
    output wire req_ready,rsp_valid,
    input wire [ADDR_BITS-1:0] req_addr,
    input wire [15:0] req_data,
    output wire [15:0] rsp_data,
    output wire physical_read,physical_write
);
    wire initialized,cache_hit;
    wire chip_clk=~clk;
    wire [15:0] dq_out,dq;
    wire [12:0] a;
    wire [1:0] ba;
    wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe;
    assign dq=dq_oe ? dq_out : 16'hzzzz;
    assign physical_read={nras,ncas,nwe}==3'b101;
    assign physical_write={nras,ncas,nwe}==3'b100;
    dorado_storage_sdram #(.ADDR_BITS(ADDR_BITS)) dut(
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
        .INIT_UNWRITTEN_TO_X(0),.tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("XSDS_RANK1"),
        .INIT_UNWRITTEN_TO_X(0),.tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
    function automatic [63:0] peek64(input integer word_address);
        reg [25:0] a0;
        reg [63:0] value;
        for(integer lane=0;lane<4;lane++)begin
            a0=26'(word_address*4+lane);
            value[lane*16+:16]=a0[25] ?
                chip1.mem_read(int'(a0[24:23]),int'(a0[22:10]),int'(a0[9:0])) :
                chip0.mem_read(int'(a0[24:23]),int'(a0[22:10]),int'(a0[9:0]));
        end
        return value;
    endfunction
    task automatic check_model;
        if(chip0.error_count!=0 || chip1.error_count!=0 ||
           chip0.warning_count!=0 || chip1.warning_count!=0)$fatal(1,"SDRAM model warnings/errors");
    endtask
endmodule
