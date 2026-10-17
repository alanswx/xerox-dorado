// The storage chips which share RAS/CAS/WE and row/column address are one
// vector here. Their original capture, write and output-register cycles
// are retained. Only their bit arrays move behind the raw storage port.
// host_clk keeps running while ready pauses the complete Dorado clock.
`default_nettype none
module dorado_storage_group #(
    parameter integer ADDR_BITS=14,
    parameter integer WIDTH=16
) (
    input wire host_clk, machine_clk, reset,
    input wire ras_n, cas_n, we_n,
    input wire [ADDR_BITS-1:0] addr,
    input wire [WIDTH-1:0] data_in,
    output reg [WIDTH-1:0] data_out=0,
    output wire ready,
    output wire req_valid,
    input wire req_ready,
    output wire req_write,
    output wire [ADDR_BITS-1:0] req_addr,
    output wire [15:0] req_data,
    input wire rsp_valid,
    output wire rsp_ready,
    input wire [15:0] rsp_data
);
    reg cas_d=0,we_d=0,write_pending=0;
    reg [WIDTH-1:0] write_data=0,read_pipe=0;
    reg cache_valid=0,write_done=0,waiting=0,saved_write=0;
    reg [ADDR_BITS-1:0] cache_addr=0,saved_addr=0;
    reg [15:0] cache_data=0,saved_data=0,old_data=0;
    wire hit=cache_valid && cache_addr==addr;
    assign ready=!reset && !waiting && hit && (!write_pending || write_done);
    assign req_valid=!reset && !waiting && (!hit || (write_pending && !write_done));
    // Read a missing word first, including before a write. The read value
    // on the chip's write-commit edge is the OLD word (read-before-write).
    assign req_write=hit && write_pending && !write_done;
    assign req_addr=addr;
    assign req_data={{(16-WIDTH){1'b0}},write_data};
    assign rsp_ready=waiting;
    wire [15:0] read_value=(write_pending && write_done) ? old_data : cache_data;

    // These are cell_MK4096P_6's cycles, on the paused machine clock.
    always @(posedge machine_clk) begin
        cas_d<=cas_n;we_d<=we_n;
        write_pending<=!ras_n && !cas_n && !we_n && (cas_d || we_d);
        if(!ras_n && !cas_n && !we_n && (cas_d || we_d))write_data<=data_in;
        read_pipe<=read_value[WIDTH-1:0];
        if(!ras_n && !cas_n && we_n)data_out<=read_pipe;
    end
    always @(posedge host_clk) begin
        if(reset) begin cache_valid<=0;write_done<=0;waiting<=0;end
        else begin
            if(!write_pending)write_done<=0;
            if(req_valid && req_ready)begin
                waiting<=1;saved_write<=req_write;saved_addr<=addr;
                saved_data<=req_data;
                if(req_write)old_data<=cache_data;
            end
            if(waiting && rsp_valid)begin
                waiting<=0;cache_valid<=1;cache_addr<=saved_addr;
                if(saved_write)begin cache_data<=saved_data;write_done<=1;end
                else cache_data<=rsp_data;
            end
        end
    end
    // synthesis translate_off
    initial if(WIDTH<1 || WIDTH>16)$fatal(1,"storage group does not fit one raw word");
    always @(posedge machine_clk) begin
        if(!ready)$fatal(1,"Dorado clock advanced while storage group was stalled");
        // native_timing.sdc's data-only hold exception relies on these:
        // a pending write leaves both delayed DRAM controls low, and a new
        // write cannot also capture a host request on this same edge.
        if(write_pending && (cas_d || we_d))
            $fatal(1,"Pending storage write has inconsistent DRAM controls");
        if(!ras_n && !cas_n && !we_n && (cas_d || we_d) && req_valid)
            $fatal(1,"Storage write_data changed while saved_data was enabled");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
