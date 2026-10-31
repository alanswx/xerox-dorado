// =============================================================================
// dorado_rtl_bootsrv.sv — In-FPGA Alto boot server (Breath of Life + EFTP)
// =============================================================================
// Plays the part of the boot server on the 3 Mb Ethernet for the Alto world
// (AEmu) boot, the way dorado/src/ethernet.c does in the C emulator:
//
//   * Breath of Life: while no EFTP transfer has started, every 50,000
//     microinstructions (500,000 after one is delivered) a BoL packet
//     carrying the Alto boot loader (eth_bol_loader) is put on the wire if
//     the receiver is on and has drained what it had.
//   * Mayday (Pup type 244B) from the booting Alto starts an EFTP transfer
//     of the boot file in ROM: Data packets (030B) of 256 words, sequence
//     number in the Pup ID, then End (032B). Each Ack (031B) carrying the
//     current sequence number releases the next packet; an Ack for any other
//     number resends the current one; the Ack of the End releases one more
//     End (the dally close) and ends the transfer (EFTPSPEC; ethernet.c
//     eth_eftp_ack). An unacknowledged packet is resent after 3,000,000
//     microinstructions.
//
// Packets are written into the controller's receive buffer in the C model's
// on-wire shape (12-word Ethernet/Pup header, payload, Pup checksum 177777B,
// a dummy hardware CRC word and a status word marked with attention), then
// rx_load starts the controller on them. The server holds the CPU (stall)
// while it reads the transmitted packet or fills the buffer, so the
// microcode never sees a half-written packet.
// =============================================================================

module dorado_rtl_bootsrv #(
    parameter int BOOT_WORDS = 28619,
    parameter     BOOT_HEX   = "roms/bootfile.hex",
    parameter     BOL_HEX    = "roms/bol.hex"
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        en,

    input  logic [63:0] cycles,          // microinstructions executed
    input  logic [7:0]  local_host,      // the booting Alto's host number

    // Controller interface
    input  logic        tx_packet_stb,   // level, raised by the tick that ended a packet
    output logic [9:0]  tx_rd_addr,
    input  logic [15:0] tx_rd_data,      // one clock after tx_rd_addr
    input  logic        rx_on,
    input  logic        rx_drained,      // receiver has read every queued word
    input  logic        io_busy,         // controller mid-tick / skip / render

    output logic        rx_wr_en,
    output logic [11:0] rx_wr_addr,
    output logic [15:0] rx_wr_data,
    output logic        rx_wr_att,
    output logic        rx_load,
    output logic [12:0] rx_load_count,

    output logic        stall,           // hold the CPU in C_FETCH

    // Observability
    output logic [15:0] eftp_max_seq,
    output logic [15:0] bol_sent
);
    localparam logic [7:0]  REMOTE_HOST      = 8'o001;
    localparam logic [15:0] PUP_ETHERNET     = 16'o001000;
    localparam logic [15:0] PUP_MAYDAY       = 16'o000244;
    localparam logic [15:0] PUP_EFTP_ACK     = 16'o000031;
    localparam logic [15:0] PUP_EFTP_DATA    = 16'o000030;
    localparam logic [15:0] PUP_EFTP_END     = 16'o000032;
    localparam int          DATA_WORDS       = 256;
    localparam int          BOL_BODY         = 253;
    localparam logic [31:0] RESEND_CYCLES    = 32'd3000000;
    localparam int          BA               = $clog2(BOOT_WORDS);

    // ---- ROMs ---------------------------------------------------------------
    logic [15:0] boot_rom [0:BOOT_WORDS-1];
    logic [15:0] bol_rom  [0:BOL_BODY-1];
    initial begin
        $readmemh(BOOT_HEX, boot_rom);
        $readmemh(BOL_HEX, bol_rom);
    end
    logic [15:0]   boot_addr;
    logic [7:0]    bol_addr;
    logic [15:0]   boot_q, bol_q;
    always_ff @(posedge clk) begin
        boot_q <= boot_rom[boot_addr[BA-1:0]];
        bol_q  <= bol_rom[bol_addr];
    end

    // ---- Server state -------------------------------------------------------
    typedef enum logic [3:0] {
        S_IDLE, S_TXADDR, S_TXWAIT, S_TXDATA, S_DECIDE,
        S_WAITIO, S_WADDR, S_WWAIT, S_WWRITE, S_LOAD, S_LOAD2
    } sstate_t;
    sstate_t st;

    logic        tx_stb_d;
    logic [2:0]  tx_k;                  // which header word is being read
    logic [15:0] tw1, tw3, tw5, tw10, tw11;

    logic [1:0]  eftp_state;            // 0 idle, 1 data, 2 end, 3 dally end
    logic        eftp_started;
    logic [15:0] seq;
    logic [15:0] pos;                   // word offset of the current packet
    logic [15:0] dsock_hi, dsock_lo;
    logic        pending, pend_bol;
    logic [63:0] next_bol;
    logic [31:0] resend_cnt;

    logic [8:0]  wk, wtotal;            // packet word index / length
    logic [8:0]  ndata;                 // payload words in the current packet

    function automatic logic [8:0] cur_ndata();
        logic [15:0] left;
        left = 16'(BOOT_WORDS) - pos;
        if (eftp_state != 2'd1)         return 9'd0;
        if (left > 16'(DATA_WORDS))     return 9'(DATA_WORDS);
        return left[8:0];
    endfunction

    // Word wk of the packet being delivered. ROM words arrive one clock after
    // their address is set (S_WADDR -> S_WWAIT -> S_WWRITE).
    function automatic logic [15:0] pkt_word(input logic [8:0] k);
        logic [15:0] w;
        if (pend_bol) begin
            if (k == 9'd0)                       w = {8'd0, REMOTE_HOST};
            else if (k <= 9'(BOL_BODY))          w = bol_q;
            else                                 w = 16'd0;          // CRC, status
        end else begin
            case (k)
                9'd0:  w = {local_host, REMOTE_HOST};
                9'd1:  w = PUP_ETHERNET;
                9'd2:  w = 16'o026 + {6'd0, ndata, 1'b0};
                9'd3:  w = (eftp_state == 2'd1) ? PUP_EFTP_DATA : PUP_EFTP_END;
                9'd4:  w = 16'd0;
                9'd5:  w = seq;
                9'd6:  w = {8'd0, local_host};
                9'd7:  w = dsock_hi;
                9'd8:  w = dsock_lo;
                9'd9:  w = {8'd0, REMOTE_HOST};
                9'd10: w = 16'd0;
                9'd11: w = 16'o000004;
                default: begin
                    if (k < 9'd12 + ndata)       w = boot_q;
                    else if (k == 9'd12 + ndata) w = 16'o177777;     // nil Pup checksum
                    else                         w = 16'd0;          // CRC, status
                end
            endcase
        end
        return w;
    endfunction

    assign stall = (st != S_IDLE);

    always_ff @(posedge clk) begin
        rx_wr_en <= 1'b0;
        rx_load  <= 1'b0;
        if (rst || !en) begin
            st           <= S_IDLE;
            tx_stb_d     <= 1'b0;
            eftp_state   <= 2'd0;
            eftp_started <= 1'b0;
            seq          <= 16'd0;
            pos          <= 16'd0;
            dsock_hi     <= 16'd0;
            dsock_lo     <= 16'o20;
            pending      <= 1'b0;
            pend_bol     <= 1'b0;
            next_bol     <= 64'd100000;
            resend_cnt   <= 32'd0;
            eftp_max_seq <= 16'd0;
            bol_sent     <= 16'd0;
            tx_rd_addr   <= 10'd0;
            boot_addr    <= 16'd0;
            bol_addr     <= 8'd0;
            rx_load_count <= 13'd0;
        end else begin
            tx_stb_d <= tx_packet_stb;
            case (st)
            S_IDLE: begin
                if (tx_packet_stb && !tx_stb_d) begin
                    tx_k <= 3'd0;
                    st   <= S_TXADDR;
                end else if (pending && rx_drained) begin
                    st <= S_WAITIO;
                end else if (!eftp_started && !pending && cycles >= next_bol) begin
                    // dorado_ethernet_breath_of_life, as Suite 6 paces it
                    if (rx_drained && rx_on) begin
                        pending  <= 1'b1;
                        pend_bol <= 1'b1;
                        next_bol <= cycles + 64'd500000;
                    end else begin
                        next_bol <= cycles + 64'd50000;
                    end
                end else if (eftp_state != 2'd0 && !pending && rx_drained) begin
                    if (resend_cnt >= RESEND_CYCLES) begin
                        pending    <= 1'b1;
                        pend_bol   <= 1'b0;
                        resend_cnt <= 32'd0;
                    end else begin
                        resend_cnt <= resend_cnt + 32'd1;
                    end
                end
            end

            // Read Pup header words 1, 3, 5, 10, 11 of the transmitted packet.
            S_TXADDR: begin
                case (tx_k)
                    3'd0: tx_rd_addr <= 10'd1;
                    3'd1: tx_rd_addr <= 10'd3;
                    3'd2: tx_rd_addr <= 10'd5;
                    3'd3: tx_rd_addr <= 10'd10;
                    default: tx_rd_addr <= 10'd11;
                endcase
                st <= S_TXWAIT;
            end
            S_TXWAIT: st <= S_TXDATA;
            S_TXDATA: begin
                case (tx_k)
                    3'd0: tw1  <= tx_rd_data;
                    3'd1: tw3  <= tx_rd_data;
                    3'd2: tw5  <= tx_rd_data;
                    3'd3: tw10 <= tx_rd_data;
                    default: tw11 <= tx_rd_data;
                endcase
                if (tx_k == 3'd4) st <= S_DECIDE;
                else begin
                    tx_k <= tx_k + 3'd1;
                    st   <= S_TXADDR;
                end
            end
            S_DECIDE: begin
                st <= S_IDLE;
                if (tw1 == PUP_ETHERNET && tw3 == PUP_MAYDAY) begin
                    dsock_hi     <= tw10;
                    dsock_lo     <= (tw10 == 16'd0 && tw11 == 16'd0) ? 16'o20 : tw11;
                    eftp_state   <= 2'd1;
                    eftp_started <= 1'b1;
                    seq          <= 16'd0;
                    pos          <= 16'd0;
                    pending      <= 1'b1;
                    pend_bol     <= 1'b0;
                end else if (tw1 == PUP_ETHERNET && tw3 == PUP_EFTP_ACK && eftp_state != 2'd0) begin
                    pending  <= 1'b1;
                    pend_bol <= 1'b0;
                    if (tw5 == seq) begin
                        case (eftp_state)
                            2'd1: begin
                                logic [15:0] np;
                                np  = pos + {7'd0, cur_ndata()};
                                pos <= np;
                                seq <= seq + 16'd1;
                                if (np >= 16'(BOOT_WORDS)) eftp_state <= 2'd2;
                            end
                            2'd2: begin
                                seq        <= seq + 16'd1;
                                eftp_state <= 2'd3;
                            end
                            default: begin
                                eftp_state <= 2'd0;
                                pending    <= 1'b0;
                            end
                        endcase
                    end
                end
            end

            // Deliver the pending packet: wait for the controller to be quiet
            // (the CPU is already held), then write it word by word.
            S_WAITIO: begin
                if (!io_busy) begin
                    ndata  <= cur_ndata();
                    wtotal <= pend_bol ? 9'(BOL_BODY + 3) : (9'd15 + cur_ndata());
                    wk     <= 9'd0;
                    st     <= S_WADDR;
                end
            end
            S_WADDR: begin
                boot_addr <= pos + {7'd0, wk} - 16'd12;
                bol_addr  <= 8'(wk - 9'd1);
                st        <= S_WWAIT;
            end
            S_WWAIT: st <= S_WWRITE;
            S_WWRITE: begin
                rx_wr_en   <= 1'b1;
                rx_wr_addr <= {3'd0, wk};
                rx_wr_data <= pkt_word(wk);
                rx_wr_att  <= (wk == wtotal - 9'd1);
                if (wk == wtotal - 9'd1) st <= S_LOAD;
                else begin
                    wk <= wk + 9'd1;
                    st <= S_WADDR;
                end
            end
            S_LOAD: begin
                rx_load       <= 1'b1;
                rx_load_count <= {4'd0, wtotal};
                pending       <= 1'b0;
                resend_cnt    <= 32'd0;
                if (pend_bol) begin
                    bol_sent <= bol_sent + 16'd1;
                end else begin
                    if (seq > eftp_max_seq) eftp_max_seq <= seq;
                    // the dally End is the last packet of the transfer
                    if (eftp_state == 2'd3) eftp_state <= 2'd0;
                end
                pend_bol <= 1'b0;
                st       <= S_LOAD2;
            end
            // The controller takes rx_load this clock; hold the CPU one more
            // so its registered read of word 0 has settled.
            S_LOAD2: st <= S_IDLE;
            default: st <= S_IDLE;
            endcase
        end
    end
endmodule
