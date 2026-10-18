// Frame-only direct-daemon transport. Mailbox layout: server/dorado_ddr_ring.h.
// Original DskEth, wire pacing and CRC remain in dorado_ethernet_wire.
// Run on the UNGATED host clock. Main must stop the daemon, then deassert
// enable and wait for stopped before programming a core. reset requests a
// drained stop too; it never drops an accepted DDR read or fabricates data.
`default_nettype none
module dorado_ethernet_ddr #(
    parameter integer POLL_CYCLES=50000,
    parameter [31:0] BYTE_BASE=32'h1ff00000,
    parameter [31:0] INITIAL_COUNT=0
) (
    input wire clk,reset,enable,
    input wire [63:0] epoch,
    output wire stopped,
    output reg initialized=0,
    output reg [31:0] fault=0,
    input wire tx_valid,rx_ready,
    input wire [10:0] tx_words,
    input wire [15:0] tx_data,
    output reg [9:0] tx_addr=0,rx_addr=0,
    output reg tx_ack=0,rx_wr=0,rx_commit=0,
    output reg [15:0] rx_data=0,
    output reg [10:0] rx_words=0,
    input wire ddram_busy,ddram_rvalid,
    input wire [63:0] ddram_rdata,
    output reg [28:0] ddram_addr=0,
    output wire ddram_read,ddram_write,
    output wire [7:0] ddram_burstcount,
    output reg [7:0] ddram_be=0,
    output reg [63:0] ddram_wdata=0
);
    localparam [63:0] MAGIC=64'h31474e49524f4444,ABI=64'h0001080000100008;
    localparam STOPPED=0,INIT=1,IDLE=2,STOP_MAGIC=3,STOP_STATUS=4,STOP_DONE=5,
        TX_ROOM=10,TX_ROOM_RESULT=11,TX_WAIT=12,TX_CAPTURE=13,TX_NEXT=14,
        TX_DESC=15,TX_EPOCH=16,TX_PUBLISH=17,TX_DONE=18,
        RX_COUNT=20,RX_COUNT_RESULT=21,RX_DESC=22,RX_DESC_RESULT=23,
        RX_EPOCH=24,RX_EPOCH_RESULT=25,RX_READ=26,RX_READ_RESULT=27,
        RX_WORD=28,RX_COMMIT=29,RX_CONSUME=30,RX_DONE=31,
        DDR_WRITE=250,DDR_READ=251,DDR_RESPONSE=252;
    reg [7:0] state=STOPPED,return_state=0;
    reg [3:0] init_index=0;
    reg [63:0] active_epoch=0,read_data=0,tx_pack=0,rx_pack=0;
    reg [31:0] tx_prod=0,rx_cons=0,frame_bytes=0,poll_count=0;
    reg [10:0] word_index=0,frame_words=0;
    reg stop_latched=0,armed=0,prefer_rx=0;
    // STOPPED is the drained FSM state. Derive the indication from it rather
    // than maintaining a second initialized-high flag: on Native46 hardware
    // the disabled engine reported that flag low with no epoch/fault, and
    // Main correctly refused its lifecycle barrier. The state registers have
    // audited zero power-up values; no active transaction enters STOPPED
    // until STOP_MAGIC/STOP_STATUS writes have completed.
    assign stopped=state==STOPPED;
    wire [31:0] tx_slot=32'h1000+{18'd0,tx_prod[2:0],11'd0};
    wire [31:0] rx_slot=32'h5000+{17'd0,rx_cons[3:0],11'd0};
    wire [31:0] tx_desc=32'h100+{25'd0,tx_prod[2:0],4'd0};
    wire [31:0] rx_desc=32'h200+{24'd0,rx_cons[3:0],4'd0};
    assign ddram_read=state==DDR_READ;
    assign ddram_write=state==DDR_WRITE;
    assign ddram_burstcount=8'd1;

    task automatic write_word(input [31:0] offset,input [63:0] data,
                              input [7:0] mask,input [7:0] following);
        begin
            ddram_addr<=BYTE_BASE[31:3]+29'(offset>>3);
            ddram_wdata<=data;ddram_be<=mask;return_state<=following;state<=DDR_WRITE;
        end
    endtask
    task automatic read_word(input [31:0] offset,input [7:0] following);
        begin
            ddram_addr<=BYTE_BASE[31:3]+29'(offset>>3);
            ddram_be<=8'hff;return_state<=following;state<=DDR_READ;
        end
    endtask
    task automatic protocol_fault(input [31:0] reason);
        begin fault<=reason;stop_latched<=1;state<=IDLE;end
    endtask

    always @(posedge clk)begin
        tx_ack<=0;rx_wr<=0;rx_commit<=0;
        if(poll_count!=0)poll_count<=poll_count-1'b1;
        if(!enable)armed<=1;
        if(reset)armed<=0;
        if(state!=STOPPED && (!enable || reset || epoch!=active_epoch))stop_latched<=1;
        case(state)
        STOPPED:if(armed && enable && !reset && epoch!=0 && epoch!=active_epoch)begin
            active_epoch<=epoch;armed<=0;initialized<=0;
            stop_latched<=0;fault<=0;init_index<=0;state<=INIT;
            tx_prod<=INITIAL_COUNT;rx_cons<=INITIAL_COUNT;poll_count<=0;prefer_rx<=0;
        end
        INIT:case(init_index)
            0:begin write_word(0,0,8'hff,INIT);init_index<=1;end
            1:begin write_word(8,active_epoch,8'hff,INIT);init_index<=2;end
            2:begin write_word('h10,{32'd0,INITIAL_COUNT},8'hff,INIT);init_index<=3;end
            3:begin write_word('h18,{32'd0,INITIAL_COUNT},8'hff,INIT);init_index<=4;end
            4:begin write_word('h20,{32'd0,INITIAL_COUNT},8'hff,INIT);init_index<=5;end
            5:begin write_word('h28,{32'd0,INITIAL_COUNT},8'hff,INIT);init_index<=6;end
            6:begin write_word('h30,1,8'hff,INIT);init_index<=7;end
            7:begin write_word('h38,ABI,8'hff,INIT);init_index<=8;end
            8:begin write_word(0,MAGIC,8'hff,INIT);init_index<=9;end
            default:begin initialized<=1;state<=IDLE;end
        endcase
        IDLE:if(stop_latched || !enable || reset || epoch!=active_epoch)begin
            initialized<=0;state<=STOP_MAGIC;
        end else if(poll_count==0)begin
            poll_count<=32'(POLL_CYCLES-1);prefer_rx<=~prefer_rx;
            if(tx_valid && !prefer_rx)state<=TX_ROOM;
            else if(rx_ready)state<=RX_COUNT;
        end
        STOP_MAGIC:write_word(0,0,8'hff,STOP_STATUS);
        STOP_STATUS:write_word('h30,fault==0 ? 64'd0 : 64'd2,8'hff,STOP_DONE);
        STOP_DONE:state<=STOPPED;
        TX_ROOM:read_word('h18,TX_ROOM_RESULT);
        TX_ROOM_RESULT:if(tx_prod-read_data[31:0]>8)protocol_fault(1);
            else if(tx_prod-read_data[31:0]==8 || !tx_valid)state<=IDLE;
            else if(tx_words<2 || tx_words>1024)protocol_fault(2);
            else begin
                frame_words<=tx_words;frame_bytes<={20'd0,tx_words,1'b0};
                word_index<=0;tx_addr<=0;tx_pack<=0;state<=TX_WAIT;
            end
        // The original wire buffer has a synchronous16-bit read port.
        TX_WAIT:state<=TX_CAPTURE;
        TX_CAPTURE:begin:pack_word
            reg [63:0] updated;
            updated=word_index[1:0]==0 ? 64'd0 : tx_pack;
            updated[{word_index[1:0],4'b0000}+:16]={tx_data[7:0],tx_data[15:8]};
            tx_pack<=updated;
            if(word_index[1:0]==3 || word_index+1'b1==frame_words)
                write_word(tx_slot+{20'd0,word_index[10:2],3'b000},updated,8'hff,TX_NEXT);
            else begin
                word_index<=word_index+1'b1;tx_addr<=tx_addr+1'b1;state<=TX_WAIT;
            end
        end
        TX_NEXT:if(word_index+1'b1==frame_words)state<=TX_DESC;
            else begin word_index<=word_index+1'b1;tx_addr<=tx_addr+1'b1;state<=TX_WAIT;end
        TX_DESC:write_word(tx_desc,{tx_prod,frame_bytes},8'hff,TX_EPOCH);
        TX_EPOCH:write_word(tx_desc+8,active_epoch,8'hff,TX_PUBLISH);
        TX_PUBLISH:write_word('h10,{32'd0,tx_prod+1'b1},8'h0f,TX_DONE);
        TX_DONE:begin tx_ack<=1;tx_prod<=tx_prod+1'b1;state<=IDLE;end
        RX_COUNT:read_word('h20,RX_COUNT_RESULT);
        RX_COUNT_RESULT:if(read_data[31:0]-rx_cons>16)protocol_fault(3);
            else if(read_data[31:0]==rx_cons || !rx_ready)state<=IDLE;
            else state<=RX_DESC;
        RX_DESC:read_word(rx_desc,RX_DESC_RESULT);
        RX_DESC_RESULT:if(read_data[63:32]!=rx_cons || read_data[31:0]<4 ||
            read_data[31:0]>2048 || read_data[0])protocol_fault(4);
            else begin frame_bytes<=read_data[31:0];frame_words<=11'(read_data[31:0]>>1);state<=RX_EPOCH;end
        RX_EPOCH:read_word(rx_desc+8,RX_EPOCH_RESULT);
        RX_EPOCH_RESULT:if(read_data!=active_epoch)protocol_fault(5);
            else begin word_index<=0;rx_words<=frame_words;state<=RX_READ;end
        RX_READ:read_word(rx_slot+{20'd0,word_index[10:2],3'b000},RX_READ_RESULT);
        RX_READ_RESULT:begin rx_pack<=read_data;state<=RX_WORD;end
        RX_WORD:begin
            rx_wr<=1;rx_addr<=word_index[9:0];
            rx_data<={rx_pack[7:0],rx_pack[15:8]};rx_pack<=rx_pack>>16;
            word_index<=word_index+1'b1;
            if(word_index+1'b1==frame_words)state<=RX_COMMIT;
            else if(word_index[1:0]==3)state<=RX_READ;
        end
        RX_COMMIT:if(!rx_ready)protocol_fault(6);
            else begin rx_commit<=1;state<=RX_CONSUME;end
        RX_CONSUME:write_word('h28,{32'd0,rx_cons+1'b1},8'h0f,RX_DONE);
        RX_DONE:begin rx_cons<=rx_cons+1'b1;state<=IDLE;end
        DDR_WRITE:if(!ddram_busy)state<=return_state;
        DDR_READ:if(!ddram_busy)begin
            if(ddram_rvalid)begin read_data<=ddram_rdata;state<=return_state;end
            else state<=DDR_RESPONSE;
        end
        DDR_RESPONSE:if(ddram_rvalid)begin read_data<=ddram_rdata;state<=return_state;end
        default:protocol_fault(7);
        endcase
    end
    initial begin
        if(POLL_CYCLES<1 || BYTE_BASE!=32'h1ff00000)
            $error("Unsupported Dorado frame-mailbox layout");
    end
endmodule
`default_nettype wire
