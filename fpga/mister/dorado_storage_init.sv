// Clear the raw DDR backing before releasing the power-on machine.
// This supplies the same deterministic zero power-up as the FPGA bit arrays;
// it never loads CPU state, microcode, mappings, or device completions.
`default_nettype none
module dorado_storage_init #(
    parameter integer ADDR_BITS=21
) (
    input wire clk,reset,
    output reg ready=0,
    output wire req_valid,
    input wire req_ready,
    output wire [ADDR_BITS-1:0] req_addr,
    input wire rsp_valid,
    output wire rsp_ready
);
    reg pending=0;
    reg [ADDR_BITS-1:0] address=0;
    assign req_valid=!reset && !ready && !pending;
    assign req_addr=address;
    assign rsp_ready=!reset && pending;
    always @(posedge clk)begin
        if(reset)begin ready<=0;pending<=0;address<=0;end
        else if(!ready)begin
            if(req_valid && req_ready)pending<=1;
            if(pending && rsp_valid)begin
                pending<=0;
                if(&address)ready<=1;
                else address<=address+1'b1;
            end
        end
    end
endmodule
`default_nettype wire
