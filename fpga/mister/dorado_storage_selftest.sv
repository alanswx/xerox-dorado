// Diagnostic power-on test of the native Dorado's 12-bit raw SDRAM cells.
// Runs before guest release. Two address slices distinguish every raw address;
// the final write/read pass leaves the same zeros as dorado_storage_init.
// An error retains the first failing transaction and prevents guest release.
`default_nettype none
module dorado_storage_selftest #(
    parameter integer ADDR_BITS=23
) (
    input wire clk,reset,
    output reg ready=0,
    output reg failed=0,
    output wire req_valid,
    input wire req_ready,
    output wire req_write,
    output wire [ADDR_BITS-1:0] req_addr,
    output wire [15:0] req_data,
    input wire rsp_valid,
    output wire rsp_ready,
    input wire [15:0] rsp_data,
    output wire [127:0] observation
);
    // 0 write low address,1 read forward,2 write high address,
    // 3 read backward,4 zero,5 verify zero,6 complete.
    reg [2:0] phase=0,first_phase=0;
    reg pending=0;
    reg [ADDR_BITS-1:0] address=0,first_address=0;
    reg [11:0] first_expected=0,first_actual=0;
    wire [11:0] expected=phase<2 ? (12'(address)^12'ha59) :
        phase<4 ? (12'(address>>12)^12'h5a6) : 12'd0;
    assign req_valid=!reset && !ready && !failed && !pending;
    assign req_write=phase==0 || phase==2 || phase==4;
    assign req_addr=address;
    assign req_data={4'b0,expected};
    assign rsp_ready=!reset && pending && !failed;
    assign observation={first_phase,first_expected,first_actual,5'b0,
        32'(first_address),32'(address),
        16'hb157,10'b0,phase,pending,failed,ready};
    always @(posedge clk)begin
        if(reset)begin
            ready<=0;failed<=0;pending<=0;address<=0;phase<=0;
            first_address<=0;first_phase<=0;first_expected<=0;first_actual<=0;
        end else if(!ready && !failed)begin
            if(req_valid && req_ready)pending<=1;
            if(rsp_valid && rsp_ready)begin
                pending<=0;
                if(!req_write && rsp_data[11:0]!=expected)begin
                    failed<=1;first_address<=address;first_phase<=phase;
                    first_expected<=expected;first_actual<=rsp_data[11:0];
                end else if((phase==3 && address==0) ||
                            (phase!=3 && (&address)))begin
                    if(phase==5)begin ready<=1;phase<=6;end
                    else begin
                        phase<=phase+1'b1;
                        address<=phase==2 ? {ADDR_BITS{1'b1}} : {ADDR_BITS{1'b0}};
                    end
                end else address<=phase==3 ? address-1'b1 : address+1'b1;
            end
        end
    end
endmodule
`default_nettype wire
