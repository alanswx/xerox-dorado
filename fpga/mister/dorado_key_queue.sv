// Buffer MiSTer key events outside the stoppable native-machine clock.
// A USB press and release can both arrive during one HPS disk wait; their
// toggle then returns to its old value and a gated decoder sees neither.
//
// Keep each transition for two actual source fields. Cedar samples KeyBits
// on retrace, and HM Table 24's terminal serializes a word over 32 lines:
// one field transports the new state, the next lets the guest sample it.
// This is a host-input adapter, not a change to the original terminal wire.
`default_nettype none
module dorado_key_queue #(
    parameter integer ADDRESS_BITS = 8
) (
    input wire clk_host, reset,
    input wire [10:0] ps2_key,
    input wire vsync_n,
    output reg [10:0] machine_key = 0,
    output reg overflow = 0,
    output wire [ADDRESS_BITS:0] queued
);
    localparam integer DEPTH = 1 << ADDRESS_BITS;
    reg [9:0] events [0:DEPTH-1];
    reg [ADDRESS_BITS-1:0] head = 0, tail = 0;
    reg [ADDRESS_BITS:0] count = 0;
    reg previous_strobe = 0, previous_vsync = 1;
    reg last_valid = 0, field_wait = 0;
    reg [9:0] last_event = 0;
    wire event_edge = ps2_key[10] != previous_strobe;
    // USB autorepeat repeats a make event without changing the matrix.
    // Do not let a held key fill the queue and delay its eventual release.
    wire changed = !last_valid || ps2_key[9:0] != last_event;
    wire push = event_edge && changed && count < DEPTH;
    wire field_tick = previous_vsync && !vsync_n;
    wire pop = field_tick && !field_wait && count != 0;
    assign queued = count;

    always @(posedge clk_host) begin
        previous_strobe <= ps2_key[10];
        previous_vsync <= vsync_n;
        if (reset) begin
            head <= 0; tail <= 0; count <= 0;
            machine_key <= 0; overflow <= 0;
            last_valid <= 0; field_wait <= 0;
        end else begin
            if (event_edge && changed) begin
                last_event <= ps2_key[9:0];
                last_valid <= 1;
                if (count == DEPTH) overflow <= 1;
            end
            if (push) begin
                events[tail] <= ps2_key[9:0];
                tail <= tail + 1'b1;
            end
            if (field_tick) begin
                if (field_wait) field_wait <= 0;
                else if (pop) begin
                    machine_key <= {~machine_key[10], events[head]};
                    head <= head + 1'b1;
                    field_wait <= 1;
                end
            end
            case ({push,pop})
                2'b10: count <= count + 1'b1;
                2'b01: count <= count - 1'b1;
                default: ;
            endcase
        end
    end
endmodule
`default_nettype wire
