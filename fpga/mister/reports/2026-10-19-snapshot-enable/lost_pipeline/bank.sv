// Snapshot experiment for one actual ContB18-bit half-bank. NOT production.
// guest_advance must be the SAME pre-edge enable that creates the guest's
// gated clock edge. All RAM logic uses the host clock, with explicit read/
// write enables, so ordinary backend pauses do not add guest RAM accesses.
// The caller establishes the global hold/barrier before guest_held and keeps
// it until freeze/frozen deassert. No whole-machine barrier is provided here.
//
// One shared RAM write port and one registered read port preserve old-data
// behavior at a simultaneous guest read/write. Frozen scanning temporarily
// borrows these ports. The captured raw read output remains visible until
// the FIRST actual guest_advance, including pauses after freeze is released.
`default_nettype none
module dorado_snapshot_f10415_enable_bank (
    input wire host_clk, guest_advance,
    input wire [9:0] address,
    input wire [17:0] data,
    input wire ce_n, we_n,
    output wire [17:0] q,
    input wire freeze, guest_held,
    output reg frozen = 1'b0,
    input wire request, write,
    input wire [10:0] word_index,
    input wire [17:0] state_data,
    output reg done = 1'b0,
    output reg error = 1'b0,
    output wire [17:0] result
);
    wire freeze_active = freeze && guest_held;
    wire scan_active = freeze_active && frozen;
    wire accepted = scan_active && request;
    wire valid_index = word_index <= 11'd1024;
    wire state_write = accepted && valid_index && write;
    wire read_enable = scan_active || (!freeze_active && guest_advance);
    wire ram_write = scan_active ? (state_write && !word_index[10]) :
                     (!freeze_active && guest_advance && !ce_n && !we_n);
    wire [9:0] ram_address = scan_active ? word_index[9:0] : address;
    wire [17:0] ram_data = scan_active ? state_data : data;
    wire [17:0] ram_q;
    reg [17:0] saved_q;
    reg pending = 1'b0;
    reg response_pipeline;
    wire [17:0] logical_q = pending ? saved_q : ram_q;
    always @(posedge host_clk) begin
        frozen <= freeze_active;
        done <= accepted;
        error <= accepted && !valid_index;
        response_pipeline <= word_index == 11'd1024;
        if (freeze_active && !frozen) begin
            saved_q <= logical_q;
            pending <= 1'b1;
        end else if (state_write && word_index == 11'd1024) begin
            saved_q <= 18'b0;
        end
        if (!freeze_active && guest_advance)
            pending <= 1'b0;
    end
    assign q = ce_n ? 18'b0 : ((frozen || pending) ? saved_q : ram_q);
    assign result = response_pipeline ? saved_q : ram_q;

    genvar bit_index;
    generate
    for (bit_index = 0; bit_index < 18; bit_index = bit_index + 1) begin: bits
        (* ramstyle = "M10K" *) reg mem [0:1023];
        reg read_q;
        always @(posedge host_clk) begin
            if (read_enable) read_q <= mem[ram_address];
            if (ram_write) mem[ram_address] <= ram_data[bit_index];
        end
        assign ram_q[bit_index] = read_q;
    end
    endgenerate
endmodule
`default_nettype wire
