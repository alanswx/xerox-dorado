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
    // Prefer the first request at/above next_group, wrapping to the lowest
    // requester if necessary. Carry its payload through the SAME balanced
    // tree. The old variable-index scan selected an owner, then ran a second
    // large address/data mux; Native29's measured path into address_latched
    // is22.7ns. This changes neither arbitration order nor handshake cycles.
    localparam LEAVES=1<<ID_BITS;
    localparam PAYLOAD=ID_BITS+GROUP_ADDR_BITS+17;
    wire [2*LEAVES-1:1] present,preferred;
    wire [PAYLOAD-1:0] payload [1:2*LEAVES-1];
    genvar leaf,node;
    generate for(leaf=0;leaf<LEAVES;leaf=leaf+1)begin:choice_leaf
        if(leaf<GROUPS)begin:populated
            assign present[LEAVES+leaf]=req_valid[leaf];
            if(leaf==LEAVES-1)begin:last_index
                assign preferred[LEAVES+leaf]=req_valid[leaf];
            end else begin:compare_index
                assign preferred[LEAVES+leaf]=req_valid[leaf] && ID_BITS'(leaf)>=next_group;
            end
            assign payload[LEAVES+leaf]={ID_BITS'(leaf),
                req_addr[leaf*GROUP_ADDR_BITS+:GROUP_ADDR_BITS],req_data[leaf*16+:16],req_write[leaf]};
        end else begin:padding
            assign present[LEAVES+leaf]=1'b0;
            assign preferred[LEAVES+leaf]=1'b0;
            assign payload[LEAVES+leaf]='0;
        end
    end
    for(node=1;node<LEAVES;node=node+1)begin:choice_node
        wire choose_left=preferred[2*node] || (!preferred[2*node+1] && present[2*node]);
        assign present[node]=present[2*node] || present[2*node+1];
        assign preferred[node]=preferred[2*node] || preferred[2*node+1];
        assign payload[node]=choose_left ? payload[2*node] : payload[2*node+1];
    end endgenerate
    wire [ID_BITS-1:0] chosen_owner;
    wire [GROUP_ADDR_BITS-1:0] chosen_address;
    wire [15:0] chosen_data;
    wire chosen_write;
    assign {chosen_owner,chosen_address,chosen_data,chosen_write}=payload[1];
    always @* begin
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
        IDLE: if(present[1])begin
            owner<=chosen_owner;
            address_latched<=chosen_address;
            data_latched<=chosen_data;write_latched<=chosen_write;
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
