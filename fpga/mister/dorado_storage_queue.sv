// Bounded raw-storage admission. Not selected by a native wrapper yet.
// A slot remains owned through queued, issued and held-response stages.
// Reset stops frontend traffic but drains ALL accepted descriptors, including
// unissued writes, before pulsing backend_reset and admitting a new epoch.
// Native integration must grant this drain ahead of the initialization mux;
// enable gates new admission only and must not disconnect accepted raw work.
`default_nettype none
module dorado_storage_queue #(
    parameter integer GROUPS=128,
    parameter integer GROUP_ADDR_BITS=16,
    parameter integer RAW_ADDR_BITS=23,
    parameter integer INTERLEAVE_GROUPS=1,
    parameter integer DEPTH=4
) (
    input wire clk,reset,enable,
    output wire backend_reset,
    output reg reset_draining=0,
    output wire idle,
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
    localparam GROUP_BITS=GROUPS<=1 ? 0 : $clog2(GROUPS);
    localparam PTR_BITS=DEPTH<=1 ? 1 : $clog2(DEPTH);
    localparam COUNT_BITS=$clog2(DEPTH+1);
    localparam LEAVES=1<<ID_BITS;
    localparam PAYLOAD=ID_BITS+GROUP_ADDR_BITS+17;
    reg [PAYLOAD-1:0] entries[0:DEPTH-1];
    reg [PTR_BITS-1:0] head=0,tail=0;
    reg [COUNT_BITS-1:0] queued_count=0,owned=0;
    reg [GROUPS-1:0] reserved=0;
    reg [ID_BITS-1:0] next_group=0,owner=0;
    reg active=0;
    // Observation only; this is not an architectural CPU state.
    wire [1:0] state=active ? 2'd2 : queued_count!=0 ? 2'd1 : 2'd0;
    wire [GROUPS-1:0] eligible=req_valid & ~reserved;
    wire [2*LEAVES-1:1] present,preferred;
    wire [PAYLOAD-1:0] payload[1:2*LEAVES-1];
    genvar leaf,node;
    generate for(leaf=0;leaf<LEAVES;leaf=leaf+1)begin:select_leaf
        if(leaf<GROUPS)begin:valid_group
            assign present[LEAVES+leaf]=eligible[leaf];
            assign preferred[LEAVES+leaf]=eligible[leaf] && ID_BITS'(leaf)>=next_group;
            assign payload[LEAVES+leaf]={ID_BITS'(leaf),
                req_addr[leaf*GROUP_ADDR_BITS+:GROUP_ADDR_BITS],req_data[leaf*16+:16],req_write[leaf]};
        end else begin:padding
            assign present[LEAVES+leaf]=0;
            assign preferred[LEAVES+leaf]=0;
            assign payload[LEAVES+leaf]='0;
        end
    end
    for(node=1;node<LEAVES;node=node+1)begin:select_node
        wire left=preferred[2*node] || (!preferred[2*node+1] && present[2*node]);
        assign present[node]=present[2*node] || present[2*node+1];
        assign preferred[node]=preferred[2*node] || preferred[2*node+1];
        assign payload[node]=left ? payload[2*node] : payload[2*node+1];
    end endgenerate
    wire [ID_BITS-1:0] chosen_owner=payload[1][PAYLOAD-1-:ID_BITS];
    wire [ID_BITS-1:0] head_owner;
    wire [GROUP_ADDR_BITS-1:0] head_address;
    wire [15:0] head_data;
    wire head_write;
    assign {head_owner,head_address,head_data,head_write}=entries[head];
    // Suppress reset until all previously admitted writes have reached their
    // normal physical completion. One full empty clock resets the backend.
    assign backend_reset=(reset || reset_draining) && owned==0;
    wire retiring=active && raw_rsp_valid && raw_rsp_ready;
    wire room=owned<DEPTH || retiring;
    wire admitting=!reset && !reset_draining && enable && room && present[1];
    assign raw_req_valid=!backend_reset && queued_count!=0 && (!active || retiring);
    assign raw_req_write=head_write;
    assign raw_req_addr=INTERLEAVE_GROUPS ?
        (RAW_ADDR_BITS'(head_address)<<GROUP_BITS) | RAW_ADDR_BITS'(head_owner) :
        (RAW_ADDR_BITS'(head_owner)<<GROUP_ADDR_BITS) | RAW_ADDR_BITS'(head_address);
    assign raw_req_data=head_data;
    assign raw_rsp_ready=active && (reset || reset_draining || rsp_ready[owner]);
    wire issuing=raw_req_valid && raw_req_ready;
    assign idle=owned==0 && !reset_draining && !reset;
    always @* begin
        req_ready=0;rsp_valid=0;
        if(admitting)req_ready[chosen_owner]=1;
        if(active && !reset && !reset_draining)rsp_valid[owner]=raw_rsp_valid;
    end
    genvar group;
    generate for(group=0;group<GROUPS;group=group+1)begin:reply_data
        assign rsp_data[group*16+:16]=raw_rsp_data;
    end endgenerate
    always @(posedge clk)begin
        if(reset)reset_draining<=1;
        else if(reset_draining && owned==0)reset_draining<=0;
        case({admitting,retiring})
        2'b10:owned<=owned+1'b1;
        2'b01:owned<=owned-1'b1;
        default:begin end
        endcase
        case({admitting,issuing})
        2'b10:queued_count<=queued_count+1'b1;
        2'b01:queued_count<=queued_count-1'b1;
        default:begin end
        endcase
        if(retiring)begin reserved[owner]<=0;active<=0;end
        if(issuing)begin
            owner<=head_owner;active<=1;
            head<=head==DEPTH-1 ? PTR_BITS'(0) : head+1'b1;
        end
        if(admitting)begin
            entries[tail]<=payload[1];reserved[chosen_owner]<=1;
            tail<=tail==DEPTH-1 ? PTR_BITS'(0) : tail+1'b1;
            next_group<=chosen_owner==GROUPS-1 ? ID_BITS'(0) : chosen_owner+1'b1;
        end
        if(reset_draining && owned==0)begin
            head<=0;tail<=0;next_group<=0;reserved<=0;
        end
    end
    // synthesis translate_off
    initial if(GROUPS<1 || GROUPS>1024 || DEPTH<1 || DEPTH>16 ||
        GROUP_ADDR_BITS+GROUP_BITS>RAW_ADDR_BITS ||
        (INTERLEAVE_GROUPS!=0 && INTERLEAVE_GROUPS!=1))
        $fatal(1,"STORAGE_QUEUE invalid geometry");
    always @(posedge clk)begin
        if(owned>DEPTH || queued_count>DEPTH || owned!=queued_count+integer'(active))
            $fatal(1,"STORAGE_QUEUE ownership accounting failed");
        if(admitting && reserved[chosen_owner])$fatal(1,"STORAGE_QUEUE duplicate group");
        if(issuing && !reserved[head_owner])$fatal(1,"STORAGE_QUEUE unowned issue");
        if(retiring && !reserved[owner])$fatal(1,"STORAGE_QUEUE unowned completion");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
