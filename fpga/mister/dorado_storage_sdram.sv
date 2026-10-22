// Raw 16-bit storage backing for a MiSTer XSDS 128 MB SDRAM module.
// Original Dorado cache/map/ECC and chip timing remain upstream.
//
// Geometry and timing: XSDS v3.0 schematic and Alliance AS4C32M16SB
// Rev1.2 (March 2020), tables16/17 and power-up note11. Conservative -7
// timings also cover -6 parts. No code from the GPL Mac controller is used.
// The board inverts CS for rank1 and connects DQM to address pins11/12.
// Drive the SDRAM clock inverted from clk: commands leave on clk rising,
// the chip samples half a cycle later. Read DQ enters an unconditional
// falling-edge input register, then the rising-edge response register. This
// leaves a full clock for chip tAC plus physical clock-output/input routing.
// Supported host clocks are20..50MHz (half-cycle >=10ns, CL2 tAC<=6ns).
`default_nettype none
module dorado_storage_sdram #(
    parameter integer ADDR_BITS=23,
    parameter integer CLOCK_HZ=50000000
) (
    input wire clk,reset,
    output wire snapshot_idle,
    input wire req_valid,
    output wire req_ready,
    input wire req_write,
    input wire [ADDR_BITS-1:0] req_addr,
    input wire [15:0] req_data,
    output wire rsp_valid,
    input wire rsp_ready,
    output reg [15:0] rsp_data=0,
    output reg initialized=0,
    output wire cache_hit,
    output wire physical_read,physical_write,
    output reg read_return=0,
    output reg [12:0] sdram_a=13'h1800,
    output reg [1:0] sdram_ba=0,
    output reg sdram_ncs=0,
    output wire sdram_nras,sdram_ncas,sdram_nwe,
    output wire sdram_dqml,sdram_dqmh,sdram_cke,
    output reg [15:0] dq_out=0,
    output reg dq_oe=0,
    input wire [15:0] dq_in
);
    function automatic integer clocks(input integer nanoseconds);
        clocks=integer'((64'(CLOCK_HZ)*64'(nanoseconds)+999999999)/1000000000);
    endfunction
    localparam integer POWER_CLOCKS=clocks(200000);
    localparam integer REFRESH_CLOCKS=clocks(7000);
    localparam integer T_RP=clocks(21),T_RCD=clocks(21);
    localparam integer T_RFC=clocks(63),T_MRD=clocks(14),T_WR=clocks(14);
    localparam [2:0] NOP=3'b111,ACTIVE=3'b011,READ=3'b101,
        WRITE=3'b100,PRECHARGE=3'b010,REFRESH=3'b001,MODE=3'b000;
    // BL1, sequential, CAS2, single writes.
    localparam [12:0] MODE_WORD=13'h220;
    localparam POWER=0,INIT=1,IDLE=2,SELECT=3,PRE_WAIT=4,ACT_WAIT=5,
        READ_WAIT=6,WRITE_WAIT=7,REF_PRE1=8,REF_PRE_WAIT=9,
        REF_WAIT0=10,REF_WAIT1=11;
    reg [3:0] state=POWER;
    reg [2:0] command=NOP;
    reg [31:0] delay_count=0,power_count=0,refresh_count=0;
    reg [3:0] init_step=0;
    reg [7:0] open=0;
    reg [12:0] rows[0:7];
    // Halfword address: rank25, bank24:23, row22:10, column9:0.
    // The low16MiB raw backing occupies rank0/bank0, with no aliasing.
    reg [25:0] address=0;
    reg [15:0] data=0;
    reg [15:0] dq_capture;
    always @(negedge clk)dq_capture<=dq_in;
    reg writing=0,aborted=0,response_pending=0;
    wire [2:0] owner=address[25:23];
    wire refresh_due=refresh_count>=32'(REFRESH_CLOCKS);
    // Guest/raw work is idle during autonomous refresh. The physical SDRAM
    // clock and refresh controller must continue while frontends are frozen.
    // This does not claim that banks are closed or controller state is saved.
    assign snapshot_idle=initialized && !reset && !req_valid && !response_pending &&
        (state==IDLE || state==REF_PRE1 || state==REF_PRE_WAIT ||
         state==REF_WAIT0 || state==REF_WAIT1);
    assign req_ready=initialized && state==IDLE && !response_pending &&
        !refresh_due && !reset;
    assign rsp_valid=response_pending && !reset;
    assign {sdram_nras,sdram_ncas,sdram_nwe}=command;
    assign {sdram_dqmh,sdram_dqml}=sdram_a[12:11];
    assign sdram_cke=1'b1;
    assign cache_hit=1'b0;
    assign physical_read=command==READ;
    assign physical_write=command==WRITE;

    task automatic activate;
        begin
            sdram_ncs<=address[25];sdram_ba<=address[24:23];
            sdram_a<=address[22:10];command<=ACTIVE;
            rows[owner]<=address[22:10];open[owner]<=1'b1;
            delay_count<=32'(T_RCD-1);state<=ACT_WAIT;
        end
    endtask
    task automatic access_word;
        begin
            sdram_ncs<=address[25];sdram_ba<=address[24:23];
            sdram_a<={3'b000,address[9:0]};
            if(writing)begin
                command<=WRITE;dq_out<=data;dq_oe<=1'b1;
                delay_count<=32'(T_WR-1);state<=WRITE_WAIT;
            end else begin
                command<=READ;
                // Command reaches chip on the intervening falling edge.
                // CAS2 data reaches the falling-edge input register on the
                // third following falling edge, then the response register
                // on the third rising edge. Includes clock-output latency.
                delay_count<=2;state<=READ_WAIT;
            end
        end
    endtask

    always @(posedge clk)begin
        command<=NOP;dq_oe<=0;sdram_a<=0;read_return<=0;
        if(response_pending && (rsp_ready || reset))response_pending<=0;
        // A reset suppresses an old response, but accepted operations drain.
        // Physical initialization is power-on only. Refresh continues while
        // reset or response backpressure holds the guest stopped.
        if(reset)aborted<=1;
        if(initialized && !refresh_due)refresh_count<=refresh_count+1'b1;
        case(state)
        POWER:begin
            sdram_a<=13'h1800;
            if(reset)power_count<=0;
            else if(power_count==32'(POWER_CLOCKS-1))begin
                state<=INIT;init_step<=0;delay_count<=0;
            end else power_count<=power_count+1'b1;
        end
        INIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            init_step<=init_step+1'b1;sdram_ba<=0;
            case(init_step)
            0,1:begin
                sdram_ncs<=init_step[0];sdram_a<=13'h0400;
                command<=PRECHARGE;delay_count<=32'(T_RP-1);
            end
            2,3,4,5:begin
                sdram_ncs<=init_step[0];command<=REFRESH;
                delay_count<=32'(T_RFC-1);
            end
            6,7:begin
                sdram_ncs<=init_step[0];sdram_a<=MODE_WORD;command<=MODE;
                delay_count<=32'(T_MRD-1);
            end
            default:begin initialized<=1;refresh_count<=0;state<=IDLE;end
            endcase
        end
        IDLE:if(refresh_due)begin
            // Close all banks periodically, including ones idle for a long
            // time. Both ranks get a refresh within the7.8us average limit.
            command<=PRECHARGE;sdram_ncs<=0;sdram_a<=13'h0400;
            open<=0;state<=REF_PRE1;
        end else if(req_valid && req_ready)begin
            address<=26'(req_addr);data<=req_data;writing<=req_write;
            aborted<=0;state<=SELECT;
        end
        SELECT:if(open[owner] && rows[owner]==address[22:10])access_word();
        else if(open[owner])begin
            command<=PRECHARGE;sdram_ncs<=address[25];
            sdram_ba<=address[24:23];open[owner]<=0;
            delay_count<=32'(T_RP-1);state<=PRE_WAIT;
        end else activate();
        PRE_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
            else activate();
        ACT_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
            else access_word();
        READ_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            rsp_data<=dq_capture;read_return<=1;
            if(!aborted && !reset)response_pending<=1;
            state<=IDLE;
        end
        WRITE_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            if(!aborted && !reset)response_pending<=1;
            state<=IDLE;
        end
        REF_PRE1:begin
            command<=PRECHARGE;sdram_ncs<=1;sdram_a<=13'h0400;
            delay_count<=32'(T_RP-1);state<=REF_PRE_WAIT;
        end
        REF_PRE_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            command<=REFRESH;sdram_ncs<=0;
            delay_count<=32'(T_RFC-1);state<=REF_WAIT0;
        end
        REF_WAIT0:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            command<=REFRESH;sdram_ncs<=1;
            delay_count<=32'(T_RFC-1);state<=REF_WAIT1;
        end
        REF_WAIT1:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin refresh_count<=0;state<=IDLE;end
        default:state<=POWER;
        endcase
    end
    // synthesis translate_off
    initial begin
        if(ADDR_BITS<1 || ADDR_BITS>26 || CLOCK_HZ<20000000 || CLOCK_HZ>50000000)
            $fatal(1,"Unsupported SDRAM address width or clock");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
