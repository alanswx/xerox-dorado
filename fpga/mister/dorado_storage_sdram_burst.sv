// Experimental BL8 raw-storage buffer for a MiSTer XSDS 128 MB SDRAM module.
// Separate from the qualified BL1 backend; no native wrapper selects this yet.
// Four fully tagged lines hide physical neighbor reads only. Writes always
// reach SDRAM and invalidate every matching line. Guest cache/map/ECC and
// original DRAM front-end timing remain upstream, including read-before-write.
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
module dorado_storage_sdram_burst #(
    parameter integer ADDR_BITS=23,
    parameter integer CLOCK_HZ=50000000,
    parameter integer CACHE_LINES=4
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
    output reg cache_hit=0,
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
    // Reserve headroom for a complete BL8 drain, row change and both rank
    // refreshes even at20MHz. The BL1 backend's7us threshold is too late
    // for this longer non-interruptible operation at the lowest host rate.
    localparam integer REFRESH_CLOCKS=clocks(6000);
    localparam integer T_RP=clocks(21),T_RCD=clocks(21);
    localparam integer T_RFC=clocks(63),T_MRD=clocks(14),T_WR=clocks(14);
    localparam [2:0] NOP=3'b111,ACTIVE=3'b011,READ=3'b101,
        WRITE=3'b100,PRECHARGE=3'b010,REFRESH=3'b001,MODE=3'b000;
    // BL8 sequential reads, CAS2, single-location writes (mode bit9).
    localparam [12:0] MODE_WORD=13'h223;
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
    localparam integer INDEX_BITS=CACHE_LINES<=1 ? 1 : $clog2(CACHE_LINES);
    reg [CACHE_LINES-1:0] line_valid=0;
    reg [22:0] line_tag[0:CACHE_LINES-1];
    reg [127:0] line_words[0:CACHE_LINES-1];
    reg [INDEX_BITS-1:0] next_line=0,fill_line=0;
    reg [2:0] fill_beat=0;
    reg [15:0] selected_data=0;
    reg lookup_hit;
    reg [INDEX_BITS-1:0] lookup_line;
    always @* begin
        lookup_hit=0;lookup_line=0;
        for(integer i=0;i<CACHE_LINES;i=i+1)begin
            if(line_valid[i] && line_tag[i]==23'(26'(req_addr)>>3))begin
                lookup_hit=1;lookup_line=INDEX_BITS'(i);
            end
        end
    end
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
            sdram_a<=writing ? {3'b000,address[9:0]} : {3'b000,address[9:3],3'b000};
            if(writing)begin
                command<=WRITE;dq_out<=data;dq_oe<=1'b1;
                delay_count<=32'(T_WR-1);state<=WRITE_WAIT;
            end else begin
                command<=READ;
                // Command reaches chip on the intervening falling edge.
                // CAS2 data reaches the falling-edge input register on the
                // fourth following falling edge, then the response register
                // on the fourth rising edge. Includes clock-output latency.
                delay_count<=3;state<=READ_WAIT;
            end
        end
    endtask

    always @(posedge clk)begin
        command<=NOP;dq_oe<=0;sdram_a<=0;read_return<=0;cache_hit<=0;
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
            aborted<=0;
            if(req_write)begin
                for(integer i=0;i<CACHE_LINES;i=i+1)
                    if(line_valid[i] && line_tag[i]==23'(26'(req_addr)>>3))line_valid[i]<=0;
                state<=SELECT;
            end else if(lookup_hit)begin
                rsp_data<=line_words[lookup_line][16*req_addr[2:0]+:16];
                response_pending<=1;cache_hit<=1;
            end else begin
                fill_line<=next_line;line_valid[next_line]<=0;
                fill_beat<=0;selected_data<=0;state<=SELECT;
            end
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
            // Each following beat has the same full-cycle input capture.
            // Do not expose a completion until the entire physical burst drains.
            line_words[fill_line][16*fill_beat+:16]<=dq_capture;
            if(fill_beat==address[2:0])selected_data<=dq_capture;
            if(fill_beat==7)begin
                rsp_data<=address[2:0]==7 ? dq_capture : selected_data;
                read_return<=1;
                if(!aborted && !reset)begin
                    response_pending<=1;line_valid[fill_line]<=1;
                    line_tag[fill_line]<=address[25:3];
                    next_line<=fill_line==CACHE_LINES-1 ? 0 : fill_line+1'b1;
                end
                state<=IDLE;
            end else fill_beat<=fill_beat+1'b1;
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
        // Reset invalidates cached copies, including a same-edge refill.
        // Accepted physical reads/writes still drain under the original contract.
        if(reset)line_valid<=0;
    end
    // synthesis translate_off
    initial begin
        if(ADDR_BITS<3 || ADDR_BITS>26 || CLOCK_HZ<20000000 || CLOCK_HZ>50000000 ||
           CACHE_LINES<1 || CACHE_LINES>16 || (CACHE_LINES & (CACHE_LINES-1))!=0)
            $fatal(1,"Unsupported SDRAM address width or clock");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
