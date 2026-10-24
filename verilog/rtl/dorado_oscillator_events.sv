// Ordered oscillator transitions for a synchronous board-state evaluator.
// SAMPLES_PER_INSTRUCTION counts fabric intervals per original60ns instruction,
// NOT samples per30ns CPU clock. At four15ns intervals,50MHz produces alternating
// one/two half-period transitions; a sampled square wave loses those events.
// Consumers must apply both events in order, including derived clock/latch
// effects. This is NOT a replacement clock pin or a native board integration.
// advance pauses modeled time for transport barriers, not architectural CPU
// Hold. Samples count a NOMINAL60ns interval, independent of retirement.
`default_nettype none
module dorado_oscillator_events #(
    parameter integer SAMPLES_PER_INSTRUCTION=4,
    parameter integer FREQUENCY_KHZ=50000
) (
    input wire clk,reset,advance,
    output wire [1:0] edge_count,
    output wire rising,falling,rising_first,last_at_tick,
    output reg level_before=1'b0,
    output wire level_after,
    // Exact position within the interval: offset_units / interval_units.
    // Keep these when interleaving pixel edges with CPU subphase captures.
    output wire [31:0] first_offset_units,second_offset_units,interval_units
);
    // Exact rational units:60ns * two transitions/period * frequency(kHz).
    // One million is the ns/kHz conversion. No rounded virtual fabric rate.
    localparam integer MODULUS=SAMPLES_PER_INSTRUCTION*1000000;
    localparam integer STEP=120*FREQUENCY_KHZ;
    localparam integer WIDTH=$clog2(MODULUS);
    localparam integer SUM_WIDTH=$clog2(3*MODULUS);
    reg [WIDTH-1:0] remainder=0;
    wire [SUM_WIDTH-1:0] sum=SUM_WIDTH'(remainder)+SUM_WIDTH'(STEP);
    wire [1:0] due=(sum>=SUM_WIDTH'(2*MODULUS)) ? 2'd2 :
                         (sum>=SUM_WIDTH'(MODULUS)) ? 2'd1 : 2'd0;
    wire [SUM_WIDTH-1:0] reduced=sum-(due==2 ? SUM_WIDTH'(2*MODULUS) :
                                                   due==1 ? SUM_WIDTH'(MODULUS) : SUM_WIDTH'(0));
    wire active=advance && !reset;
    assign edge_count=active ? due : 2'd0;
    assign rising=(edge_count==2) || (edge_count==1 && !level_before);
    assign falling=(edge_count==2) || (edge_count==1 && level_before);
    assign rising_first=(edge_count!=0) && !level_before;
    assign last_at_tick=(edge_count!=0) && (reduced==0);
    assign level_after=level_before ^ edge_count[0];
    assign first_offset_units=(edge_count!=0) ? 32'(MODULUS)-32'(remainder) : 32'd0;
    assign second_offset_units=(edge_count==2) ? 32'(2*MODULUS)-32'(remainder) : 32'd0;
    assign interval_units=32'(STEP);
    always @(posedge clk)begin
        if(reset)begin remainder<=0;level_before<=0;end
        else if(advance)begin remainder<=WIDTH'(reduced);level_before<=level_after;end
    end
    initial begin
        if(SAMPLES_PER_INSTRUCTION<1 || FREQUENCY_KHZ<1 || STEP>2*MODULUS)
            $fatal(1,"OSC_EVENTS unsupported interval: more than two transitions required");
    end
endmodule
`default_nettype wire
