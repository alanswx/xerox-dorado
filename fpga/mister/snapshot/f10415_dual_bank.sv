// Snapshot experiment for one audited ContB 1024x18 half-bank.
// NOT connected to the production machine. See audit_contb_im.py for grouping.
//
// The guest RAM port retains the ORIGINAL gated guest clock. A free-running
// host clock scans a second port; it never adds guest read/write edges during
// ordinary backend pauses. The system caller must stop all guest clocks and
// let guest state settle BEFORE asserting guest_held. That machine-wide clock/
// disk/network barrier is outside this module. Keep guest_held asserted until
// freeze is low and frozen has deasserted, before allowing another guest edge.
//
// Word0..1023 is RAM; word1024 is the raw (pre-CE) registered read pipeline.
// Capture and restore preserve that pipeline separately from the current RAM
// word. Host writes cannot directly load the RAM's guest-clock output register:
// a saved-output override supplies it until the FIRST real resumed guest edge.
// Repeated pipeline writes while held must not cancel that override.
`default_nettype none
module dorado_snapshot_f10415_dual_bank (
    input wire guest_clk, host_clk,
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
    wire accepted = freeze && guest_held && frozen && request;
    wire valid_index = word_index <= 11'd1024;
    wire state_write = accepted && valid_index && write;
    wire ram_state_write = state_write && !word_index[10];
    reg [17:0] saved_q;
    wire [17:0] guest_q, scan_q;
    reg restore_generation = 1'b0;
    reg guest_generation = 1'b0;
    reg response_pipeline;
    wire [17:0] logical_q = (restore_generation != guest_generation) ? saved_q : guest_q;

    always @(posedge guest_clk) begin
        guest_generation <= restore_generation;
    end
    always @(posedge host_clk) begin
        frozen <= freeze && guest_held;
        done <= accepted;
        error <= accepted && !valid_index;
        response_pipeline <= word_index == 11'd1024;
        // Also mask any physical mixed-port effect of a host RAM write on
        // the stopped A-port output. Preserve the captured pipeline until a
        // real guest edge, even when only RAM (not word1024) was written.
        if (state_write)
            restore_generation <= !guest_generation;
        if (freeze && guest_held && !frozen) begin
            saved_q <= logical_q;
        end else if (state_write && word_index == 11'd1024) begin
            saved_q <= state_data;
        end
    end
    assign result = response_pipeline ? saved_q : scan_q;
    assign q = ce_n ? 18'b0 : (frozen ? saved_q : logical_q);

    genvar bit_index;
    generate
    for (bit_index = 0; bit_index < 18; bit_index = bit_index + 1) begin: bits
        (* ramstyle = "M10K" *) reg mem [0:1023];
        reg read_q, state_q;
        // Same-port old-before-write behavior as each unchanged F10415A.
        // Host/guest writes never overlap under the external hold contract.
        always @(posedge guest_clk) begin
            read_q <= mem[address];
            if (!ce_n && !we_n) mem[address] <= data[bit_index];
        end
        always @(posedge host_clk) begin
            // The administrative write response is unspecified, so use the
            // vendor's write-through inference template on this port. The
            // guest port above still requires old-before-write reads.
            if (ram_state_write) begin
                mem[word_index[9:0]] <= state_data[bit_index];
                state_q <= state_data[bit_index];
            end else begin
                state_q <= mem[word_index[9:0]];
            end
        end
        assign guest_q[bit_index] = read_q;
        assign scan_q[bit_index] = state_q;
    end
    endgenerate
endmodule
`default_nettype wire
