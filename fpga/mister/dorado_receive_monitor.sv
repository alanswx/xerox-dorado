// Passive bring-up observation. Never drives the controller or guest state.
`default_nettype none
module dorado_receive_monitor (
    input wire clk,
    input wire fifo_write_n, fifo_eop,
    input wire [15:0] fifo_word,
    input wire input_n, input_eop,
    input wire [7:0] tioa,
    input wire [15:0] input_word,
    output wire [31:0] observation
);
    reg write_d=1, input_d=1;
    reg [7:0] received=0, consumed=0, wire_status=0, cpu_status=0;
    always @(posedge clk) begin
        write_d<=fifo_write_n;
        input_d<=input_n;
        if(write_d && !fifo_write_n && fifo_eop) begin
            received<=received+1'b1;
            wire_status<=fifo_word[7:0];
        end
        if(input_d && !input_n && tioa==8'o15 && input_eop) begin
            consumed<=consumed+1'b1;
            cpu_status<=input_word[7:0];
        end
    end
    assign observation={received,consumed,wire_status,cpu_status};
endmodule
`default_nettype wire
