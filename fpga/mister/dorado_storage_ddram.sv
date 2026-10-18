// Raw storage backing only. The Dorado's DRAM front ends, cache/map, ECC,
// address selection and serial memory bus remain upstream in RTL.
// MiSTer's DDR port addresses 64-bit words; requests address 16-bit cells.
// 0x30000000 is outside Linux's 0..0x1fefffff RAM on MiSTer14 and the
// framework scaler's 0x20000000..0x207fffff buffer. No ARM code serves RAM.
`default_nettype none
module dorado_storage_ddram #(
    parameter integer ADDR_BITS = 24,
    parameter [31:0] BYTE_BASE = 32'h30000000,
    // Optional backing-store line buffer, independent of the original
    // Dorado cache/ECC. Zero retains the previously qualified adapter.
    parameter integer LINE_CACHE_BITS = 0,
    parameter integer GROUP_ADDR_BITS = 16
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
    output wire cache_hit,
    output wire [63:0] ddram_wdata,
    output wire [7:0] ddram_be,
    input wire [63:0] ddram_rdata,
    input wire ddram_rvalid
);
    localparam IDLE=0, ISSUE=1, READ_WAIT=2, RESPONSE=3, LOOKUP=4;
    reg [2:0] state = IDLE;
    reg write_latched=0, aborted=0;
    reg [ADDR_BITS-1:0] address_latched=0;
    reg [15:0] data_latched=0;
    wire [5:0] shift = {address_latched[1:0],4'b0000};
    wire line_hit;
    wire [63:0] line_data;
    generate if(LINE_CACHE_BITS>0)begin:line_buffer
        localparam LINES=1<<LINE_CACHE_BITS;
        // Mix owner and word-line bits: concurrent chip groups sharing a
        // DRAM address must not all evict the same slot. Full address tags
        // resolve every collision; the hash affects performance only.
        wire [LINE_CACHE_BITS-1:0] request_index=
            LINE_CACHE_BITS'((req_addr>>2) ^ (req_addr>>GROUP_ADDR_BITS));
        wire [LINE_CACHE_BITS-1:0] saved_index=
            LINE_CACHE_BITS'((address_latched>>2) ^ (address_latched>>GROUP_ADDR_BITS));
        reg [LINES-1:0] valid=0;
        reg [ADDR_BITS-3:0] tags [0:LINES-1];
        reg [63:0] data [0:LINES-1];
        reg valid_q=0;
        reg [ADDR_BITS-3:0] tag_q=0;
        reg [63:0] data_q=0;
        wire read_complete=ddram_rvalid &&
            ((ddram_read && !ddram_busy) || state==READ_WAIT);
        assign line_hit=valid_q && tag_q==address_latched[ADDR_BITS-1:2];
        assign line_data=data_q;
        always @(posedge clk)begin
            if(state==IDLE && req_valid && !reset)begin
                valid_q<=valid[request_index];
                tag_q<=tags[request_index];data_q<=data[request_index];
                // Writes always reach DDR and invalidate before any later
                // lookup. Never acknowledge a write from this buffer.
                if(req_write)valid[request_index]<=0;
            end
            if(read_complete && !aborted && !reset)begin
                data[saved_index]<=ddram_rdata;
                tags[saved_index]<=address_latched[ADDR_BITS-1:2];
                valid[saved_index]<=1;
            end
            // An orphan read may drain after reset, but cannot populate a
            // new machine's line buffer or satisfy a subsequent request.
            if(reset)begin valid<=0;valid_q<=0;end
        end
    end else begin:no_line_buffer
        assign line_hit=1'b0;
        assign line_data=64'd0;
    end endgenerate
    assign req_ready = state==IDLE && !reset;
    assign rsp_valid = state==RESPONSE && !aborted && !reset;
    assign ddram_burstcount = 8'd1;
    assign ddram_addr = BYTE_BASE[31:3] + 29'(address_latched >> 2);
    // Synchronous tags settle during LOOKUP. A miss can issue its read on
    // that very cycle, preserving the uncached adapter's cold-read latency.
    // Hits never assert a DDR command, even if the bus is unavailable.
    assign ddram_read = !write_latched &&
        (state==ISSUE || (state==LOOKUP && !line_hit));
    assign ddram_write = state==ISSUE && write_latched;
    // Observation only: one pulse per read satisfied without a DDR command.
    assign cache_hit = !reset && state==LOOKUP && !write_latched && line_hit;
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
            write_latched<=req_write;aborted<=0;
            state<=LINE_CACHE_BITS>0 && !req_write ? LOOKUP : ISSUE;
        end
        LOOKUP: if(!write_latched && line_hit)begin
            rsp_data<=16'(line_data >> shift);state<=RESPONSE;
        end else if(!ddram_busy)begin
            if(ddram_rvalid)begin
                rsp_data<=16'(ddram_rdata >> shift);state<=RESPONSE;
            end else state<=READ_WAIT;
        end else state<=ISSUE;
        ISSUE: if(!ddram_busy) begin
            if(write_latched) state<=RESPONSE;
            else if(ddram_rvalid) begin
                rsp_data<=16'(ddram_rdata >> shift);state<=RESPONSE;
            end else state<=READ_WAIT;
        end
        READ_WAIT: if(ddram_rvalid) begin
            rsp_data<=16'(ddram_rdata >> shift);state<=RESPONSE;
        end
        RESPONSE: if(aborted || reset || rsp_ready) state<=IDLE;
        endcase
    end

    // synthesis translate_off
    initial begin
        if(ADDR_BITS<2 || ADDR_BITS>24 || BYTE_BASE[2:0]!=0 ||
           LINE_CACHE_BITS<0 || LINE_CACHE_BITS>10 || GROUP_ADDR_BITS<2 ||
           BYTE_BASE<32'h20800000 ||
           {1'b0,BYTE_BASE}+(33'd2<<ADDR_BITS)>33'h040000000)
            $fatal(1,"raw storage window overlaps Linux/scaler or exceeds reserved DDR");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
