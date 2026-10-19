// Actual BaseBoard 2125 byte group:1024x8 plus eight raw read-pipeline bits.
// Reuse the separately qualified exact-advance/OLD_DATA RAM transport.
// Unlike F10415, 2125 outputs are also disabled while WE' is low.
`default_nettype none
module dorado_snapshot_i2125_byte_bank (
    input wire host_clk,guest_advance,
    input wire [9:0] address,input wire [7:0] data,
    input wire ce_n,we_n,output wire [7:0] q,
    input wire freeze,guest_held,output wire frozen,
    input wire request,write,input wire [10:0] word_index,
    input wire [17:0] state_data,
    output wire done,error,output wire [17:0] result
);
    wire [7:0] bank_q;
    wire [17:0] bank_result;
    dorado_snapshot_f10415_word_bank #(.WIDTH(8)) memory(
        .host_clk(host_clk),.guest_advance(guest_advance),
        .address(address),.data(data),.ce_n(ce_n),.we_n(we_n),.q(bank_q),
        .freeze(freeze),.guest_held(guest_held),.frozen(frozen),
        .request(request),.write(write),.word_index(word_index),.state_data(state_data),
        .done(done),.error(error),.result(bank_result)
    );
    assign q=we_n ? bank_q : 8'b0;
    assign result=error ? 18'b0 : bank_result;
endmodule
`default_nettype wire
