// Raw storage backing only. The Dorado's DRAM front ends, cache/map, ECC,
// address selection and serial memory bus remain upstream in RTL.
// MiSTer's DDR port addresses 64-bit words; requests address 16-bit cells.
// 0x30000000 is outside Linux's 0..0x1fefffff RAM on MiSTer14 and the
// framework scaler's 0x20000000..0x207fffff buffer. No ARM code serves RAM.
`default_nettype none
module dorado_storage_ddram #(
    parameter integer ADDR_BITS = 24,
    parameter [31:0] BYTE_BASE = 32'h30000000
) (
    input wire clk, reset,
    input wire req_valid,
    output wire req_ready,
    input wire req_write,
    input wire [ADDR_BITS-1:0] req_addr,
    input wire [15:0] req_data,
    output wire rsp_valid,
    input wire rsp_ready,
    output reg [15:0] rsp_data = 0,
    input wire ddram_busy,
    output wire [7:0] ddram_burstcount,
    output wire [28:0] ddram_addr,
    output wire ddram_read, ddram_write,
    output wire [63:0] ddram_wdata,
    output wire [7:0] ddram_be,
    input wire [63:0] ddram_rdata,
    input wire ddram_rvalid
);
    localparam IDLE=0, ISSUE=1, READ_WAIT=2, RESPONSE=3;
    reg [1:0] state = IDLE;
    reg write_latched=0, aborted=0;
    reg [ADDR_BITS-1:0] address_latched=0;
    reg [15:0] data_latched=0;
    wire [5:0] shift = {address_latched[1:0],4'b0000};
    assign req_ready = state==IDLE && !reset;
    assign rsp_valid = state==RESPONSE && !aborted && !reset;
    assign ddram_burstcount = 8'd1;
    assign ddram_addr = BYTE_BASE[31:3] + (address_latched >> 2);
    assign ddram_read = state==ISSUE && !write_latched;
    assign ddram_write = state==ISSUE && write_latched;
    assign ddram_wdata = {4{data_latched}};
    assign ddram_be = write_latched ? (8'h03 << {address_latched[1:0],1'b0}) : 8'hff;

    always @(posedge clk) begin
        // Drain an already issued operation even across guest reset. In
        // particular, an old read response must never satisfy a new read.
        // A full FPGA reload is separately handled by the framework's
        // f2sdram_safe_terminator. Reset never fabricates a completion.
        if(reset && state!=IDLE) aborted<=1;
        case(state)
        IDLE: if(req_valid && !reset) begin
            address_latched<=req_addr;data_latched<=req_data;
            write_latched<=req_write;aborted<=0;state<=ISSUE;
        end
        ISSUE: if(!ddram_busy) begin
            if(write_latched) state<=RESPONSE;
            else if(ddram_rvalid) begin
                rsp_data<=ddram_rdata >> shift;state<=RESPONSE;
            end else state<=READ_WAIT;
        end
        READ_WAIT: if(ddram_rvalid) begin
            rsp_data<=ddram_rdata >> shift;state<=RESPONSE;
        end
        RESPONSE: if(aborted || reset || rsp_ready) state<=IDLE;
        endcase
    end

    // synthesis translate_off
    initial begin
        if(ADDR_BITS<2 || ADDR_BITS>24 || BYTE_BASE[2:0]!=0 ||
           BYTE_BASE<32'h20800000 ||
           {1'b0,BYTE_BASE}+(33'd2<<ADDR_BITS)>33'h040000000)
            $fatal(1,"raw storage window overlaps Linux/scaler or exceeds reserved DDR");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
