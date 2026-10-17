// One outstanding raw-memory operation, round robin among chip groups.
// Requests and ownership are latched across DDR stalls and read latency.
`default_nettype none
module dorado_storage_arb #(
    parameter integer GROUPS=128,
    parameter integer GROUP_ADDR_BITS=14,
    parameter integer RAW_ADDR_BITS=24
) (
    input wire clk,reset,
    input wire [GROUPS-1:0] req_valid,req_write,
    output reg [GROUPS-1:0] req_ready,
    input wire [GROUPS*GROUP_ADDR_BITS-1:0] req_addr,
    input wire [GROUPS*16-1:0] req_data,
    output reg [GROUPS-1:0] rsp_valid,
    input wire [GROUPS-1:0] rsp_ready,
    output wire [GROUPS*16-1:0] rsp_data,
    output wire raw_req_valid,raw_req_write,
    input wire raw_req_ready,
    output wire [RAW_ADDR_BITS-1:0] raw_req_addr,
    output wire [15:0] raw_req_data,
    input wire raw_rsp_valid,
    output wire raw_rsp_ready,
    input wire [15:0] raw_rsp_data
);
    localparam ID_BITS=GROUPS<=1 ? 1 : $clog2(GROUPS);
    localparam IDLE=0,REQUEST=1,RESPONSE=2;
    reg [1:0] state=IDLE;
    reg [ID_BITS-1:0] owner=0,next_group=0;
    reg [GROUP_ADDR_BITS-1:0] address_latched=0;
    reg [15:0] data_latched=0;
    reg write_latched=0;
    reg found;
    integer chosen,candidate;
    always @* begin
        found=0;chosen=0;candidate=0;
        for(integer i=0;i<GROUPS;i++)begin
            candidate=next_group+i;
            if(candidate>=GROUPS)candidate=candidate-GROUPS;
            if(!found && req_valid[candidate])begin found=1;chosen=candidate;end
        end
        req_ready=0;rsp_valid=0;
        if(!reset && state==REQUEST)req_ready[owner]=raw_req_ready;
        if(!reset && state==RESPONSE)rsp_valid[owner]=raw_rsp_valid;
    end
    assign raw_req_valid=!reset && state==REQUEST;
    assign raw_req_write=write_latched;
    assign raw_req_addr=(RAW_ADDR_BITS'(owner)<<GROUP_ADDR_BITS) | address_latched;
    assign raw_req_data=data_latched;
    assign raw_rsp_ready=!reset && state==RESPONSE && rsp_ready[owner];
    genvar reply_group;
    generate for(reply_group=0;reply_group<GROUPS;reply_group=reply_group+1)begin:reply_data
        assign rsp_data[reply_group*16+:16]=raw_rsp_data;
    end endgenerate
    always @(posedge clk)begin
        if(reset)begin state<=IDLE;next_group<=0;end
        else case(state)
        IDLE: if(found)begin
            owner<=ID_BITS'(chosen);
            address_latched<=req_addr[chosen*GROUP_ADDR_BITS+:GROUP_ADDR_BITS];
            data_latched<=req_data[chosen*16+:16];write_latched<=req_write[chosen];
            state<=REQUEST;
        end
        REQUEST: if(raw_req_ready)state<=RESPONSE;
        RESPONSE: if(raw_rsp_valid && rsp_ready[owner])begin
            next_group<=owner==GROUPS-1 ? 0 : owner+1'b1;state<=IDLE;
        end
        endcase
    end
    // synthesis translate_off
    initial if(GROUPS<1 || GROUPS>1024 || GROUP_ADDR_BITS+ID_BITS>RAW_ADDR_BITS)
        $fatal(1,"storage groups exceed raw memory window");
    // synthesis translate_on
endmodule
`default_nettype wire
