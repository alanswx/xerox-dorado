`default_nettype none
// Isolated experiment: rotate existing physical FF state while ALL consumers
// are held. No shadow copy of the FF bank. Board/global integration is absent.
// A lost boundary quarantines the machine: hold_required stays asserted until
// SYSTEM reset. Resetting this transport alone cannot recover shifted state.
module dorado_ff_scan_controller #(
    parameter integer STATE_BITS = 56
) (
    input wire clk, reset, freeze, guest_held,
    input wire request, write,
    input wire [23:0] index,
    input wire [17:0] data,
    input wire serial_out,
    output wire serial_in, scan_step, ready, hold_required,
    output reg busy = 1'b0, done = 1'b0, error = 1'b0, fault = 1'b0,
    output reg [17:0] result = 18'b0
);
    localparam integer COUNT_BITS = (STATE_BITS < 2) ? 1 : $clog2(STATE_BITS+1);
    localparam integer WORDS = (STATE_BITS+17)/18;
    reg frozen = 1'b0;
    reg writing;
    reg [17:0] payload;
    reg [COUNT_BITS-1:0] position, first_bit;
    wire [COUNT_BITS:0] offset = {1'b0,position}-{1'b0,first_bit};
    wire in_window = position >= first_bit && offset < 18;
    assign ready = !reset && freeze && guest_held && frozen && !busy && !fault;
    assign scan_step = !reset && busy && freeze && guest_held && !fault;
    assign serial_in = writing && in_window ? payload[offset] : serial_out;
    assign hold_required = busy || fault;

    // synthesis translate_off
    initial if (STATE_BITS < 1) $fatal(1,"STATE_BITS must be positive");
    // synthesis translate_on

    always @(posedge clk) begin
        if (reset) begin
            frozen <= 1'b0;
            busy <= 1'b0;
            done <= 1'b0;
            error <= 1'b0;
            fault <= 1'b0;
            result <= 18'b0;
            writing <= 1'b0;
            payload <= 18'b0;
            position <= 0;
            first_bit <= 0;
        end else begin
            frozen <= freeze && guest_held;
            done <= 1'b0;
            error <= 1'b0;
            if (busy && (!freeze || !guest_held)) begin
                fault <= 1'b1;
                done <= !fault;
                error <= !fault;
            end else if (scan_step) begin
                if (in_window && !writing) result[offset] <= serial_out;
                if (position == STATE_BITS-1) begin
                    busy <= 1'b0;
                    done <= 1'b1;
                end else position <= position+1'b1;
            end else if (request && ready) begin
                result <= 18'b0;
                if (index >= WORDS) begin
                    done <= 1'b1;
                    error <= 1'b1;
                end else begin
                    busy <= 1'b1;
                    position <= 0;
                    first_bit <= index*18;
                    writing <= write;
                    payload <= data;
                end
            end
        end
    end
endmodule
