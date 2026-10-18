// Dorado-only UIO commands, through hps_io EXT_BUS. Main remains SPI owner.
// 70: magic, flags, TX length. 71: TX length, words. 72: acknowledge TX.
// 73: RX-ready, length, words; deselection commits ONLY a complete frame.
// Optional74: coherent DDR capability/state.75: atomic mode+64-bit epoch
// control (six words including command); malformed/partial commands do nothing.
`default_nettype none
module dorado_ethernet_uio #(
    parameter integer MAX_WORDS=512,
    parameter integer DDR_CAPABLE=0
) (
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
    output reg [31:0] aborted=0,
    input wire ddr_stopped,ddr_initialized,
    input wire [31:0] ddr_fault,
    output reg ddr_enable=0,
    output reg [63:0] ddr_epoch=0,
    output reg [31:0] ddr_control_errors=0,
    output wire ddr_owned
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
  reg ddr_control=0,ddr_control_valid=0;
  reg [15:0] ddr_mode=0,ddr_flags_snapshot=0;
  reg [63:0] ddr_pending_epoch=0,ddr_epoch_snapshot=0;
  reg [31:0] ddr_fault_snapshot=0,ddr_errors_snapshot=0;
  // Ownership remains with DDR until its outstanding accesses have drained.
  assign ddr_owned=(DDR_CAPABLE!=0) && (ddr_enable || !ddr_stopped);
  assign bus[32]=enable && command>=16'h70 &&
      (command<=16'h73 || ((DDR_CAPABLE!=0) && command<=16'h75));
  assign bus[15:0]=dout;
  initial if(DDR_CAPABLE!=0 && MAX_WORDS!=1024)
    $error("DDR capability requires full2048-byte wire buffers");
  always @(posedge clk)begin
    tx_ack<=0;rx_wr<=0;rx_commit<=0;
    if(reset)begin
      command<=0;count<=0;tx_addr<=0;receiving<=0;aborted<=0;
      ddr_control<=0;ddr_enable<=0;ddr_epoch<=0;ddr_control_errors<=0;
    end else if(!enable)begin
      if(receiving)begin
        if(rx_allowed && received==rx_words && rx_words>=2)rx_commit<=1;
        else aborted<=aborted+1'b1;
      end
      if(ddr_control)begin
        if(ddr_control_valid && count==6 && (ddr_mode==0 ||
           (ddr_pending_epoch!=0 && ddr_pending_epoch!=ddr_epoch && ddr_stopped && !ddr_enable)))begin
          if(ddr_mode==0)ddr_enable<=0;
          else begin ddr_epoch<=ddr_pending_epoch;ddr_enable<=1;end
        end else ddr_control_errors<=ddr_control_errors+1'b1;
      end
      ddr_control<=0;
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
        16'h71:begin dout<=ddr_owned ? 16'd0 : 16'(tx_words);read_allowed<=tx_valid && !ddr_owned;end
        16'h72:begin tx_ack<=!ddr_owned;dout<=16'hd074;end
        16'h73:begin
          dout<={15'd0,rx_ready && !ddr_owned};receiving<=1;received<=0;
          rx_words<=0;rx_addr<=0;rx_allowed<=rx_ready && !ddr_owned;
        end
        16'h74:if(DDR_CAPABLE!=0)begin
          dout<=16'hd0d4;
          ddr_flags_snapshot<={11'd0,ddr_owned,(|ddr_fault),ddr_enable,ddr_initialized,ddr_stopped};
          ddr_fault_snapshot<=ddr_fault;ddr_epoch_snapshot<=ddr_epoch;
          ddr_errors_snapshot<=ddr_control_errors;
        end else dout<=0;
        16'h75:if(DDR_CAPABLE!=0)begin
          dout<=16'hd0d4;ddr_control<=1;ddr_control_valid<=1;
          ddr_mode<=0;ddr_pending_epoch<=0;
        end else begin
          dout<=0;
        end
        default:dout<=0;
        endcase
      end else case(command)
        16'h70:case(count)
          1:dout<={12'd0,(DDR_CAPABLE!=0),(MAX_WORDS>512),
                   tx_valid && !ddr_owned,rx_ready && !ddr_owned};
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
        16'h74:if(DDR_CAPABLE!=0)case(count)
          1:dout<=16'd1; // mailbox ABI version
          2:dout<=16'h0000; // aperture base low
          3:dout<=16'h1ff0; // aperture base high
          4:dout<=16'h0000; // aperture size low
          5:dout<=16'h0001; // aperture size high
          6:dout<=ddr_flags_snapshot;
          7:dout<=ddr_fault_snapshot[15:0];
          8:dout<=ddr_fault_snapshot[31:16];
          9:dout<=ddr_epoch_snapshot[15:0];
          10:dout<=ddr_epoch_snapshot[31:16];
          11:dout<=ddr_epoch_snapshot[47:32];
          12:dout<=ddr_epoch_snapshot[63:48];
          13:dout<=16'd2048;
          14:dout<=16'd8;
          15:dout<=16'd16;
          16:dout<=ddr_errors_snapshot[15:0];
          17:dout<=ddr_errors_snapshot[31:16];
          default:dout<=0;
        endcase
        16'h75:if(DDR_CAPABLE!=0)case(count)
          1:begin ddr_mode<=din;if(din>1)ddr_control_valid<=0;end
          2:ddr_pending_epoch[15:0]<=din;
          3:ddr_pending_epoch[31:16]<=din;
          4:ddr_pending_epoch[47:32]<=din;
          5:ddr_pending_epoch[63:48]<=din;
          default:ddr_control_valid<=0;
        endcase
        default: ;
      endcase
    end
  end
endmodule
`default_nettype wire
