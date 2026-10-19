// Snapshot access experiment, NOT connected to the production Dorado.
// A 1024 x 34 IM sub-bank built from the cell_F10415A RAM/read contract.
// See verilog/cells/cell_F10415A.v for pin ordering and registered reads.
//
// The caller first holds guest clocks/phase, then asserts freeze. The first
// edge captures the *previous* RAM read outputs and suppresses guest writes.
// frozen acknowledges that boundary. Access is accepted only on later edges
// with freeze && frozen. Word 0..1023 is RAM; word 1024 is its read pipeline.
// Each request transfers one complete 34-bit word. Shared write enables keep
// the original RAM packing; independent per-bit writes cost 34 M10Ks instead
// of 4 in the isolated fit. done/error are one-clock responses;
// read data accompanies done. Write-response data is unspecified. Invalid
// word indices acknowledge with error and cannot alter state.
//
// The RAM read always clocks, even while frozen. Its address is multiplexed
// for scanning; held_q supplies the guest's saved read pipeline meanwhile.
// This avoids adding a read-clock enable that defeats RAM inference. On the
// first resumed edge, the ordinary RAM read/write occurs and takes over the
// guest output again. All other guest state must share that resume boundary.
`default_nettype none
module dorado_snapshot_f10415_bank (
    input wire clk,
    input wire [9:0] address,
    input wire [33:0] data,
    input wire ce_n, we_n,
    output wire [33:0] q,
    input wire freeze,
    output reg frozen = 1'b0,
    input wire request, write,
    input wire [10:0] word_index,
    input wire [33:0] state_data,
    output reg done = 1'b0,
    output reg error = 1'b0,
    output wire [33:0] result
);
    wire accepted = freeze && frozen && request;
    wire valid_index = word_index <= 11'd1024;
    wire [9:0] ram_address = (freeze && frozen) ? word_index[9:0] : address;
    wire [33:0] ram_q, held_q;
    reg response_pipeline;

    always @(posedge clk) begin
        frozen <= freeze;
        done <= accepted;
        error <= accepted && !valid_index;
        response_pipeline <= word_index == 11'd1024;
    end
    assign result = response_pipeline ? held_q : ram_q;

    genvar bit_index;
    generate
    for (bit_index = 0; bit_index < 34; bit_index = bit_index + 1) begin: bits
        // Same single-bit storage geometry and unconditional registered read
        // as the original cell, including old-before-write simulation order.
        // Retain old-data collision semantics; do not mark them don't-care.
        (* ramstyle = "M10K" *) reg mem [0:1023];
        reg read_q, saved_q;
        wire selected_write = accepted && valid_index && write;
        wire ram_write = freeze ? (selected_write && !word_index[10]) : (!ce_n && !we_n);
        wire ram_data = freeze ? state_data[bit_index] : data[bit_index];
        always @(posedge clk) begin
            read_q <= mem[ram_address];
            if (ram_write) mem[ram_address] <= ram_data;
            if (freeze && !frozen) begin
                saved_q <= read_q;
            end else if (selected_write && word_index == 11'd1024) begin
                saved_q <= state_data[bit_index];
            end
        end
        assign ram_q[bit_index] = read_q;
        assign held_q[bit_index] = saved_q;
        assign q[bit_index] = ce_n ? 1'b0 : (frozen ? saved_q : read_q);
    end
    endgenerate
endmodule
`default_nettype wire
