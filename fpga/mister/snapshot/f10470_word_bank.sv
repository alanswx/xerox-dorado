// Width-correct F10470 state experiment for the independently addressed MemD cache
// groups. Caller holds all consumers before guest_held; no global barrier.
// Ordinary old-data/read-enable semantics match the original clocked cells.
// Scan words0..4095 are RAM;4096 is the raw guest read pipeline. Writes ignore
// unused18-bit transport padding, reads return zero padding. Invalid indices
// acknowledge with error and cannot write state; their result is unspecified.
`default_nettype none
module dorado_snapshot_f10470_word_bank #(parameter integer WIDTH=1) (
    input wire host_clk,guest_advance,
    input wire [11:0] address,input wire [WIDTH-1:0] data,
    input wire ce_n,we_n,output wire [WIDTH-1:0] q,
    input wire freeze,guest_held,output reg frozen=1'b0,
    input wire request,write,input wire [12:0] word_index,
    input wire [17:0] state_data,
    output reg done=1'b0,error=1'b0,output wire [17:0] result
);
    generate if(WIDTH<1 || WIDTH>18)begin:invalid_width
        initial $fatal(1,"F10470 snapshot WIDTH must be1..18");
    end endgenerate
    localparam integer PADDING=(WIDTH>=1 && WIDTH<=18)?18-WIDTH:0;
    wire freeze_active=freeze && guest_held;
    wire scan_active=freeze_active && frozen;
    wire accepted=scan_active && request;
    wire valid_index=word_index<=13'd4096;
    wire state_write=accepted && valid_index && write;
    wire read_enable=scan_active || (!freeze_active && guest_advance);
    wire ram_write=scan_active?(state_write && !word_index[12]):
        (!freeze_active && guest_advance && !ce_n && !we_n);
    wire [11:0] ram_address=scan_active?word_index[11:0]:address;
    wire [WIDTH-1:0] ram_data=scan_active?state_data[WIDTH-1:0]:data;
    (* ramstyle="M10K" *) reg [WIDTH-1:0] mem[0:4095];
    reg [WIDTH-1:0] ram_q,saved_q;
    reg pending=1'b0,response_pipeline;
    wire [WIDTH-1:0] logical_q=pending?saved_q:ram_q;
    always @(posedge host_clk)begin
        if(read_enable)ram_q<=mem[ram_address];
        if(ram_write)mem[ram_address]<=ram_data;
        frozen<=freeze_active;
        done<=accepted;
        error<=accepted && !valid_index;
        response_pipeline<=word_index==13'd4096;
        if(freeze_active && !frozen)begin
            saved_q<=logical_q;
            pending<=1'b1;
        end else if(state_write && word_index==13'd4096)
            saved_q<=state_data[WIDTH-1:0];
        if(!freeze_active && guest_advance)pending<=1'b0;
    end
    assign q=ce_n?'0:((frozen || pending)?saved_q:ram_q);
    assign result={{PADDING{1'b0}},(response_pipeline?saved_q:ram_q)};
endmodule
`default_nettype wire
