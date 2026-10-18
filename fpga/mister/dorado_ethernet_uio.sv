// Dorado-only UIO commands, through hps_io EXT_BUS. Main remains SPI owner.
// 70: magic, flags, TX length. 71: TX length, words. 72: acknowledge TX.
// 73: RX-ready, length, words; deselection commits ONLY a complete frame.
`default_nettype none
module dorado_ethernet_uio #(parameter integer MAX_WORDS=512) (
    input wire clk, reset,
    inout wire [35:0] bus,
    input wire rx_ready,tx_valid,
    input wire [$clog2(MAX_WORDS):0] tx_words,
    input wire [15:0] tx_data,
    input wire [31:0] tx_good,tx_bad,tx_dropped,rx_bad,
    input wire [31:0] machine_debug,processor_debug,
    input wire [15:0] machine_status,
    output reg [$clog2(MAX_WORDS)-1:0] tx_addr=0,rx_addr=0,
    output reg tx_ack=0,rx_wr=0,rx_commit=0,
    output reg [15:0] rx_data=0,
    output reg [$clog2(MAX_WORDS):0] rx_words=0,
    output reg [31:0] aborted=0
);
  wire enable=bus[34],strobe=bus[33];
  wire [15:0] din=bus[31:16];
  reg [15:0] command=0,dout=0;
  reg [11:0] count=0;
  reg [$clog2(MAX_WORDS):0] received=0;
  reg receiving=0,rx_allowed=0,read_allowed=0;
  // One coherent observation across the multiword status transaction.
  // In particular, low/high counter halves must not straddle a rollover.
  reg [31:0] machine_snapshot=0,processor_snapshot=0;
  reg [15:0] status_snapshot=0;
  assign bus[32]=enable && command>=16'h70 && command<=16'h73;
  assign bus[15:0]=dout;
  always @(posedge clk)begin
    tx_ack<=0;rx_wr<=0;rx_commit<=0;
    if(reset)begin
      command<=0;count<=0;tx_addr<=0;receiving<=0;aborted<=0;
    end else if(!enable)begin
      if(receiving)begin
        if(rx_allowed && received==rx_words && rx_words>=2)rx_commit<=1;
        else aborted<=aborted+1'b1;
      end
      receiving<=0;command<=0;count<=0;tx_addr<=0;dout<=0;
    end else if(strobe)begin
      if(count!=12'hfff)count<=count+1'b1;
      if(count==0)begin
        command<=din;tx_addr<=0;
        case(din)
        16'h70:begin
          dout<=16'hd074;
          machine_snapshot<=machine_debug;
          processor_snapshot<=processor_debug;
          status_snapshot<=machine_status;
        end
        16'h71:begin dout<=tx_words;read_allowed<=tx_valid;end
        16'h72:begin tx_ack<=1;dout<=16'hd074;end
        16'h73:begin
          dout<={15'd0,rx_ready};receiving<=1;received<=0;
          rx_words<=0;rx_addr<=0;rx_allowed<=rx_ready;
        end
        default:dout<=0;
        endcase
      end else case(command)
        16'h70:case(count)
          1:dout<={13'd0,(MAX_WORDS>512),tx_valid,rx_ready};
          2:dout<=tx_words;
          3:dout<=tx_good[15:0];
          4:dout<=tx_bad[15:0];
          5:dout<=tx_dropped[15:0];
          6:dout<=rx_bad[15:0];
          7:dout<=aborted[15:0];
          8:dout<=machine_snapshot[15:0];
          9:dout<=machine_snapshot[31:16];
          10:dout<=status_snapshot;
          11:dout<=processor_snapshot[15:0];
          12:dout<=processor_snapshot[31:16];
          default:dout<=0;
        endcase
        16'h71:begin
          dout<=read_allowed && count<=tx_words ? tx_data : 16'd0;
          tx_addr<=tx_addr+1'b1;
        end
        16'h73:begin
          if(count==1)begin
            if(din<2 || din>MAX_WORDS)rx_allowed<=0;
            rx_words<=din[$clog2(MAX_WORDS):0];
          end else begin
            if(rx_allowed && received<rx_words)begin
              rx_wr<=1;rx_addr<=received[$clog2(MAX_WORDS)-1:0];rx_data<=din;
            end else rx_allowed<=0;
            if(received<MAX_WORDS+1)received<=received+1'b1;
          end
        end
        default: ;
      endcase
    end
  end
endmodule
`default_nettype wire
