// Packet buffers at the cable side of the real DskEth controller.
// 340 ns cells, one sync bit, MSB-first data, CRC-16/BUYPASS (HM 11).
// DskEth's CRC input includes the sync bit (its TxData wire-OR). The first
// captured FileSystem lookup's check word is 7c50 only with that leading 1.
// clk is the ungated fabric clock; step advances one emulated fabric sample.
// Host ports never execute guest I/O or supply fabricated receiver status.
`default_nettype none
module dorado_ethernet_wire #(
    parameter integer SYSPER = 4,
    parameter integer MAX_WORDS = 512,
    parameter integer GAP_TICKS = (200000*SYSPER)/30
) (
    input wire clk, reset, step,
    input wire tx_line,
    output reg rx_line = 0,
    input wire rx_wr, rx_commit,
    input wire [$clog2(MAX_WORDS)-1:0] rx_addr,
    input wire [15:0] rx_data,
    input wire [$clog2(MAX_WORDS):0] rx_words,
    output wire rx_ready,
    input wire [$clog2(MAX_WORDS)-1:0] tx_addr,
    output reg [15:0] tx_data,
    input wire tx_ack,
    output reg tx_valid = 0,
    output reg [$clog2(MAX_WORDS):0] tx_words = 0,
    output reg [31:0] tx_good = 0, tx_bad = 0, tx_dropped = 0,
    output reg [31:0] rx_bad = 0
);
  localparam integer ADDRESS_BITS=$clog2(MAX_WORDS);
  localparam integer HALF_UNITS = 170*SYSPER;
  localparam integer MID_MIN = (340*SYSPER*3)/(30*4);
  localparam integer IDLE_TICKS = (340*SYSPER*8)/(30*5);
  function automatic [15:0] crc_bit(input [15:0] r, input b);
    crc_bit = {r[14:0],1'b0} ^ ((r[15]^b) ? 16'h8005 : 16'h0000);
  endfunction

  // Host loads only an idle receive buffer. It remains immutable from
  // commit through the complete serial packet and interpacket gap.
  (* ramstyle = "no_rw_check" *) reg [15:0] rx_buffer [0:MAX_WORDS-1];
  localparam R_IDLE=0, R_WAIT=1, R_SYNC=2, R_DATA=3, R_CRC=4, R_GAP=5;
  reg [2:0] rstate = R_IDLE;
  reg [ADDRESS_BITS:0] rlength = 0, rword = 0;
  reg [15:0] rshift = 0, rcrc = 0, rq;
  reg [15:0] rphase = 0;
  reg [3:0] rbit = 15;
  reg rhalf = 0;
  reg [31:0] rgap = 0;
  wire [ADDRESS_BITS:0] read_address = rstate <= R_WAIT ? '0 : rword+1'b1;
  always @(posedge clk) begin
    if (rx_wr && rx_ready && rx_addr < MAX_WORDS) rx_buffer[rx_addr] <= rx_data;
    rq <= rx_buffer[read_address[ADDRESS_BITS-1:0]];
  end
  assign rx_ready = rstate == R_IDLE;
  wire rdata_bit = rstate == R_SYNC ? 1'b1 : rshift[15];
  wire [15:0] rcrc_next = crc_bit(rcrc,rshift[15]);
  always @(posedge clk) begin
    if (reset) begin
      rstate <= R_IDLE; rx_line <= 0; rphase <= 0; rhalf <= 0; rx_bad <= 0;
    end else begin
      if (rx_commit) begin
        if (!rx_ready || rx_words < 2 || rx_words > MAX_WORDS) rx_bad <= rx_bad+1'b1;
        else begin rlength <= rx_words; rstate <= R_WAIT; rgap <= 0; end
      end
      if (step) begin
        case (rstate)
        R_WAIT: begin
          // Let synchronous RAM settle and wait until the guest transmitter
          // has released the cable. The real receiver supplies carrier sense.
          if (rgap < 2) rgap <= rgap+1'b1;
          else if (!tactive) begin
            rshift <= rq; rword <= 0; rbit <= 15; rcrc <= 0;
            rphase <= 0; rhalf <= 0; rx_line <= 0; rstate <= R_SYNC;
          end
        end
        R_SYNC, R_DATA, R_CRC: begin
          if (rphase+30 >= HALF_UNITS) begin
            rphase <= rphase+30-HALF_UNITS;
            rhalf <= ~rhalf;
            if (!rhalf) rx_line <= rdata_bit; // mid-cell transition
            else if (rstate == R_SYNC) begin
              rstate <= R_DATA; rcrc <= 16'h8005; rx_line <= ~rshift[15];
            end else if (rstate == R_DATA) begin
              rcrc <= rcrc_next;
              if (rbit != 0) begin
                rbit <= rbit-1'b1; rshift <= {rshift[14:0],1'b0};
                rx_line <= ~rshift[14];
              end else if (rword+1'b1 < rlength) begin
                rword <= rword+1'b1; rshift <= rq; rbit <= 15;
                rx_line <= ~rq[15];
              end else begin
                rstate <= R_CRC; rshift <= rcrc_next; rbit <= 15;
                rx_line <= ~rcrc_next[15];
              end
            end else if (rbit != 0) begin
              rbit <= rbit-1'b1; rshift <= {rshift[14:0],1'b0};
              rx_line <= ~rshift[14];
            end else begin rstate <= R_GAP; rgap <= 0; rx_line <= 0; end
          end else rphase <= rphase+30;
        end
        R_GAP: if (rgap >= GAP_TICKS-1) rstate <= R_IDLE;
               else rgap <= rgap+1'b1;
        default: ;
        endcase
      end
    end
  end

  // A packet starts with a rising sync midpoint on the non-inverted cable.
  // Skip boundary transitions; midpoint direction is the data bit. Check
  // the actual transmitted CRC and only then publish its packet to Main.
  (* ramstyle = "no_rw_check" *) reg [15:0] tx_buffer [0:MAX_WORDS-1];
  reg tline_d = 0, tactive = 0, tdiscard = 0;
  reg [15:0] tage = 0, tshift = 0, tcrc = 0;
  reg [3:0] tbit = 0;
  reg [ADDRESS_BITS:0] tcount = 0;
  wire [15:0] tshift_next = {tshift[14:0],tx_line};
  always @(posedge clk) begin
    tx_data <= tx_buffer[tx_addr];
    if (reset) begin
      tline_d <= tx_line; tactive <= 0; tx_valid <= 0;
      tx_good <= 0; tx_bad <= 0; tx_dropped <= 0;
    end else begin
      if (tx_ack) tx_valid <= 0;
      if (step) begin
        tline_d <= tx_line;
        if (tactive) begin
          tage <= tage+1'b1;
          if (tx_line != tline_d && tage+1 >= MID_MIN) begin
            tage <= 0; tshift <= tshift_next;
            tcrc <= crc_bit(tcrc,tx_line); tbit <= tbit+1'b1;
            if (tbit == 15) begin
              if (!tdiscard && tcount < MAX_WORDS) tx_buffer[tcount[ADDRESS_BITS-1:0]] <= tshift_next;
              if (tcount < MAX_WORDS+2) tcount <= tcount+1'b1;
            end
          end else if (tage >= IDLE_TICKS) begin
            tactive <= 0;
            if (tbit != 0 || tcount < 3 || tcount > MAX_WORDS+1 || tcrc != 0) tx_bad <= tx_bad+1'b1;
            else if (tdiscard) tx_dropped <= tx_dropped+1'b1;
            else begin tx_words <= tcount-1'b1; tx_valid <= 1; tx_good <= tx_good+1'b1; end
          end
        end else if (tx_line && !tline_d) begin
          tactive <= 1; tage <= 0; tbit <= 0; tcount <= 0;
          tshift <= 0; tcrc <= 16'h8005; tdiscard <= tx_valid;
        end
      end
    end
  end
endmodule
`default_nettype wire
