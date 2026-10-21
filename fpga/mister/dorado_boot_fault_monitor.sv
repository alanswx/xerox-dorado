// Passive original-firmware diagnostic. No CPU, memory or clock control.
// HM 5.12/ContB09: preserve the six original error enables and source pins.
// Arm only after all enables are asserted with Error' high. Capture the
// first subsequently sampled Error' low; this may be an intentional firmware
// breakpoint (bad IM parity), not a defective instruction or memory cell.
`default_nettype none
module dorado_boot_fault_monitor (
    input wire clk,error_n,
    input wire [15:0] sources,
    input wire [31:0] processor,
    output wire [127:0] observation
);
    reg armed=0,valid=0;
    reg [15:0] previous_sources=0,first_sources=0,preceding_sources=0;
    reg [31:0] first_processor=0;
    always @(posedge clk)begin
        previous_sources<=sources;
        if(!valid)begin
            if(error_n && sources[15:10]==6'b111111)armed<=1;
            if(armed && !error_n)begin
                valid<=1;first_sources<=sources;preceding_sources<=previous_sources;
                first_processor<=processor;
            end
        end
    end
    // Each page is latched by the existing Main-owned command70 snapshot.
    // Pages1/2 are one retained event; live page0 is a separate observation.
    assign observation={16'hd0e2,14'd0,armed,valid,
                        preceding_sources,first_sources,
                        first_processor,16'hd0e1,sources};
endmodule
`default_nettype wire
