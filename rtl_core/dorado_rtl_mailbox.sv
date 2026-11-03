// =============================================================================
// dorado_rtl_mailbox.sv — The HPS's way in: a mailbox in DDR3
// =============================================================================
// On the MiSTer the C host services (Cedar's germ disk and Ethernet bridges,
// the Pilot timer, the keyboard and field notify, the Pup/STP server) run on
// the ARM, and reach the core through a region of DDR3 just past storage.
// This block polls it through the DDR engine's aux port (single beats) and
// turns what it finds into host-port operations (dorado_rtl_cpu ap_op, the
// AP_* codes) and Ethernet receive-buffer fills; it also writes a status
// block the ARM reads.
//
// Layout, in 64-bit beats from MB_BASE (byte offsets in brackets):
//   0        control [0x0000]: the ARM writes MB_MAGIC in bits 31:0 once
//            storage holds the checkpoint; the core is held until then (go).
//            The mailbox clears it first after reset: DDR keeps its contents
//            across a core reload, and a stale go would start the core on
//            the last run's storage.
//   8..11    status  [0x0040], written every STATUS_PERIOD clocks:
//              8  cycles (microinstructions)
//              9  the four keyboard words (word i = bits 16i+15:16i)
//             10  {mouse_x, mouse_y, 6'0, mouse_present, buttons[2:0],
//                  4'0, rx_drained, rx_on, status count[15:0]}  (MSB first)
//             11  {16'0, rx loads[15:0], tx packets[15:0], 8'0, cmd tail}
//             12  fabric clocks; 13 {31'0, pll_locked, CLK_50M edges}
//             14  the stall report (written only while an operation waits;
//                 see stall_tick): dorado_rtl_cpu dbg_stall
//             15  {dorado_rtl_storage_ddr dbg[63:16], stall reports[15:0]}
//   256..511 command ring [0x0800]: {seq[7:0], 7'0, cache, we, op[2:0],
//            va[27:0], wd[15:0]}. Entry i of lap L is valid when its seq is
//            (L mod 255) + 1; the ARM writes the low word first and the word
//            holding seq last, so a half-written entry is never taken.
//   512..767 result ring  [0x1000]: {seq, 40'0, rd[15:0]} for the entry.
//   1024..   receive words [0x2000]: up to 4096 words, four to a beat
//   2048..   receive attention bits [0x4000]: bit i of the bitmap = word i
//   2112..2119 transmit lengths [0x4200], one beat per slot
//   4096..   transmit slots [0x8000], 8 x 256 beats (1024 words each)
// Each packet the Ethernet transmitter finishes (tx_packet_stb) is copied,
// with the CPU held, into slot (n mod 8) and its length into that slot's
// length beat; then status beat 11's tx count becomes n + 1. The ARM hands
// the slots to its server in order.
// Operations: the AP_* codes go to the host port; op 7 (RXLOAD) copies wd
// words (and their attention bits) into the Ethernet receive buffer and
// starts the receiver on them (dorado_rtl_io rx_load). The CPU is held
// between microinstructions for each, and stays held while the ring has more.
// =============================================================================

module dorado_rtl_mailbox #(
    parameter logic [28:0] MB_BASE       = 29'h0610_0000,    // byte 0x3080_0000
    parameter logic [31:0] MB_MAGIC      = 32'h444F_5241,    // "DORA"
    parameter int          STATUS_PERIOD = 4096,
    parameter int          POLL_PERIOD   = 32
) (
    input  logic         clk,
    input  logic         rst,
    output logic         go,

    // DDR engine aux port
    output logic         aux_req,
    output logic         aux_we,
    output logic [28:0]  aux_addr,
    output logic [63:0]  aux_wdata,
    output logic [7:0]   aux_be,
    input  logic         aux_ack,
    input  logic [63:0]  aux_rdata,

    // host port (dorado_rtl_top hp_*)
    output logic         hp_hold,
    output logic         hp_req,
    output logic [2:0]   hp_op,
    output logic         hp_we,
    output logic         hp_cache,
    output logic [27:0]  hp_va,
    output logic [15:0]  hp_wd,
    input  logic         hp_done,
    input  logic [15:0]  hp_rd,
    input  logic         hp_between,

    // Ethernet receive buffer (dorado_rtl_top hx_rx_*)
    output logic         rx_wr_en,
    output logic [11:0]  rx_wr_addr,
    output logic [15:0]  rx_wr_data,
    output logic         rx_wr_att,
    output logic         rx_load,
    output logic [12:0]  rx_load_count,
    input  logic         rx_on,
    input  logic         rx_drained,
    input  logic         tx_packet_stb,
    input  logic [10:0]  tx_packet_words,
    output logic [9:0]   tx_rd_addr,
    input  logic [15:0]  tx_rd_data,

    // status
    input  logic [63:0]  cycles,
    input  logic [4:0]   cpu_st,        // the sequencer's state, for the histogram
    input  logic [63:0]  cpu_stall,     // the CPU's and memory section's state (dorado_rtl_cpu dbg_stall)
    input  logic [63:0]  ddr_dbg,       // the storage DDR client's state (dorado_rtl_storage_ddr dbg)
    input  logic         pll_locked,    // the core PLL's lock (status beat 13 bit 32)
    input  logic [31:0]  ref_cnt,       // a count of CLK_50M edges (beat 13 low word): the fabric clock against a known reference
    input  logic [63:0]  kbd_words,
    input  logic [2:0]   mouse_buttons,
    input  logic         mouse_present,
    input  logic [15:0]  mouse_x,
    input  logic [15:0]  mouse_y
);
    localparam logic [28:0] B_CTRL   = MB_BASE;
    localparam logic [28:0] B_STATUS = MB_BASE + 29'd8;
    localparam logic [28:0] B_CMD    = MB_BASE + 29'd256;
    localparam logic [28:0] B_RES    = MB_BASE + 29'd512;
    localparam logic [28:0] B_RXW    = MB_BASE + 29'd1024;
    localparam logic [28:0] B_RXA    = MB_BASE + 29'd2048;
    localparam logic [28:0] B_TXLEN  = MB_BASE + 29'd2112;
    localparam logic [28:0] B_TX     = MB_BASE + 29'd4096;
    localparam logic [2:0]  OP_RXLOAD = 3'd7;

    typedef enum logic [4:0] {
        M_CLR, M_GO, M_GOWAIT, M_IDLE, M_STATUS, M_CMDRD, M_EXEC, M_RES,
        M_RXHOLD, M_RXATT, M_RXBEAT, M_RXWR, M_RXLOAD,
        M_TXHOLD, M_TXADDR, M_TXWAIT, M_TXDATA, M_TXBEAT, M_TXLEN
    } mstate_t;
    mstate_t     st;

    logic [7:0]  tail;             // next command entry
    logic [7:0]  lap_seq;          // the seq valid for this lap: 1..255
    logic        burst;            // the last poll found a command
    logic [15:0] poll_t;
    logic [15:0] status_t;
    logic [23:0] clk_cnt;          // free-running: the host measures fabric clocks per microinstruction
    logic [63:0] clk64;            // ...and the same, whole (status beat 12)
    logic [39:0] st_cnt [0:15];    // clocks by sequencer state (status beats 16-31, every 16th round)
    logic [3:0]  hist_t;
    logic        hist_now;
    logic [15:0] status_n;
    logic [15:0] tx_count;
    logic [15:0] rxload_count;     // receive fills done (status: freshness of rx_drained)
    logic [4:0]  sbeat;
    logic [63:0] cmd;
    logic [15:0] res_rd;
    logic [12:0] rx_n, rx_i;       // words to load, next word
    logic [63:0] rx_beat, rx_att;
    logic [1:0]  rx_lane;
    logic        hold_cmd;         // held for a command or a receive fill
    logic        tx_pend;          // a transmitted packet waits to be copied (CPU held)
    logic [10:0] tx_len;
    logic [10:0] tx_i;
    logic [63:0] tx_beat;
    // The stall report: a host operation (or the CPU's arrival between
    // instructions for a receive fill or a transmit copy) that has waited
    // 2^26 clocks (1.5 s at 44 MHz) writes the CPU's and the DDR client's
    // state to status beats 14 and 15, and again every 2^26 clocks while it
    // waits; the status itself is not written meanwhile (M_IDLE writes it).
    logic [25:0] stall_t;
    logic [1:0]  stall_ph;
    logic [15:0] stall_n;
    task automatic stall_tick();
        stall_t <= stall_t + 26'd1;
        if (stall_ph == 2'd0 && stall_t == 26'h3FFFFFF) begin
            stall_ph <= 2'd1;
            stall_n  <= stall_n + 16'd1;
        end else if (stall_ph == 2'd1 && !aux_req) begin
            beat_wr(B_STATUS + 29'd6, cpu_stall);
            stall_ph <= 2'd2;
        end else if (stall_ph == 2'd2 && !aux_req) begin
            beat_wr(B_STATUS + 29'd7, {ddr_dbg[63:16], stall_n});
            stall_ph <= 2'd0;
        end
    endtask
    assign hp_hold = hold_cmd || tx_pend;

    wire [7:0]  c_seq   = cmd[63:56];
    wire        c_cache = cmd[48];
    wire        c_we    = cmd[47];
    wire [2:0]  c_op    = cmd[46:44];
    wire [27:0] c_va    = cmd[43:16];
    wire [15:0] c_wd    = cmd[15:0];

    task automatic beat_rd(input logic [28:0] a);
        aux_req  <= 1'b1;
        aux_we   <= 1'b0;
        aux_addr <= a;
        aux_be   <= 8'hFF;
    endtask
    task automatic beat_wr(input logic [28:0] a, input logic [63:0] d);
        aux_req   <= 1'b1;
        aux_we    <= 1'b1;
        aux_addr  <= a;
        aux_wdata <= d;
        aux_be    <= 8'hFF;
    endtask

    always_ff @(posedge clk) begin
        rx_wr_en <= 1'b0;
        rx_load  <= 1'b0;
        if (tx_packet_stb) begin
            tx_pend <= 1'b1;
            tx_len  <= tx_packet_words;
        end
        if (aux_ack) aux_req <= 1'b0;
        if (poll_t != 16'd0)   poll_t   <= poll_t - 16'd1;
        if (status_t != 16'd0) status_t <= status_t - 16'd1;
        clk_cnt <= clk_cnt + 24'd1;
        clk64   <= clk64 + 64'd1;
        st_cnt[cpu_st[3:0]] <= st_cnt[cpu_st[3:0]] + 40'd1;
        if (rst) begin
            st       <= M_CLR;
            clk64    <= 64'd0;
            hist_t   <= 4'd0;
            for (int i = 0; i < 16; i++) st_cnt[i] <= 40'd0;
            go       <= 1'b0;
            aux_req  <= 1'b0;
            aux_we   <= 1'b0;
            aux_addr <= '0;
            aux_wdata <= '0;
            aux_be   <= 8'hFF;
            hold_cmd <= 1'b0;
            hp_req   <= 1'b0;
            hp_op    <= 3'd0;
            hp_we    <= 1'b0;
            hp_cache <= 1'b0;
            hp_va    <= '0;
            hp_wd    <= '0;
            tail     <= 8'd0;
            lap_seq  <= 8'd1;
            burst    <= 1'b0;
            poll_t   <= 16'd0;
            status_t <= 16'd0;
            status_n <= 16'd0;
            tx_count <= 16'd0;
            rxload_count <= 16'd0;
            tx_pend  <= 1'b0;
            stall_t  <= 26'd0;
            stall_ph <= 2'd0;
            stall_n  <= 16'd0;
            tx_rd_addr <= '0;
            rx_load_count <= '0;
        end else begin
            case (st)
            // ---- wait for the ARM's go -------------------------------------
            M_CLR: if (!aux_req) begin
                beat_wr(B_CTRL, 64'd0);
                st <= M_GO;
            end
            M_GO: if (poll_t == 16'd0 && !aux_req) begin
                beat_rd(B_CTRL);
                st <= M_GOWAIT;
            end
            M_GOWAIT: if (aux_ack) begin
                poll_t <= 16'd1024;
                if (aux_rdata[31:0] == MB_MAGIC) begin
                    go <= 1'b1;
                    st <= M_IDLE;
                end else begin
                    st <= M_GO;
                end
            end
            // ---- the loop ---------------------------------------------------
            M_IDLE: if (!aux_req) begin
                stall_t  <= 26'd0;
                stall_ph <= 2'd0;
                if (tx_pend) begin
                    st <= M_TXHOLD;
                end else if (status_t == 16'd0) begin
                    status_t <= 16'(STATUS_PERIOD);
                    status_n <= status_n + 16'd1;
                    sbeat    <= 5'd0;
                    hist_now <= (hist_t == 4'd0);
                    hist_t   <= hist_t + 4'd1;
                    st       <= M_STATUS;
                end else if (burst || poll_t == 16'd0) begin
                    beat_rd(B_CMD + {21'd0, tail});
                    st <= M_CMDRD;
                end
            end
            M_STATUS: if (!aux_req || aux_ack) begin
                case (sbeat)
                5'd0: beat_wr(B_STATUS,          {clk_cnt, cycles[39:0]});
                5'd1: beat_wr(B_STATUS + 29'd1,  kbd_words);
                5'd2: beat_wr(B_STATUS + 29'd2,  {mouse_x, mouse_y, 6'd0, mouse_present,
                                                  mouse_buttons, 4'd0, rx_drained, rx_on, status_n});
                5'd3: beat_wr(B_STATUS + 29'd3,  {16'd0, rxload_count, tx_count, 8'd0, tail});
                5'd4: beat_wr(B_STATUS + 29'd4,  clk64);
                5'd5: beat_wr(B_STATUS + 29'd5,  {31'd0, pll_locked, ref_cnt});
                default: beat_wr(B_STATUS + 29'd8 + 29'(sbeat - 5'd6), {24'd0, st_cnt[4'(sbeat - 5'd6)]});
                endcase
                sbeat <= sbeat + 5'd1;
                if (sbeat == (hist_now ? 5'd21 : 5'd5)) st <= M_IDLE;
            end
            M_CMDRD: if (aux_ack) begin
                cmd <= aux_rdata;
                if (aux_rdata[63:56] == lap_seq) begin
                    burst   <= 1'b1;
                    hold_cmd <= 1'b1;
                    st      <= (aux_rdata[46:44] == OP_RXLOAD) ? M_RXHOLD : M_EXEC;
                end else begin
                    burst   <= 1'b0;
                    hold_cmd <= 1'b0;
                    poll_t  <= 16'(POLL_PERIOD);
                    st      <= M_IDLE;
                end
            end
            // ---- a host-port operation ---------------------------------------
            M_EXEC: begin
                if (!hp_req && !hp_done) begin
                    hp_req   <= 1'b1;
                    hp_op    <= c_op;
                    hp_we    <= c_we;
                    hp_cache <= c_cache;
                    hp_va    <= c_va;
                    hp_wd    <= c_wd;
                end else if (hp_req && hp_done) begin
                    hp_req <= 1'b0;
                    res_rd <= hp_rd;
                    st     <= M_RES;
                end else begin
                    stall_tick();
                end
            end
            M_RES: if (!aux_req) begin
                beat_wr(B_RES + {21'd0, tail}, {c_seq, 40'd0, res_rd});
                tail <= tail + 8'd1;
                if (tail == 8'd255) lap_seq <= (lap_seq == 8'd255) ? 8'd1 : lap_seq + 8'd1;
                st   <= M_IDLE;
            end
            // ---- an Ethernet receive-buffer fill ------------------------------
            M_RXHOLD: if (hp_between && stall_ph == 2'd0) begin
                rx_n <= (c_wd[12:0] > 13'd4096) ? 13'd4096 : c_wd[12:0];
                rx_i <= 13'd0;
                st   <= M_RXATT;
            end else begin
                stall_tick();
            end
            M_RXATT: begin                        // next 64 words' attention bits
                if (rx_i >= rx_n) begin
                    st <= M_RXLOAD;
                end else if (!aux_req) begin
                    beat_rd(B_RXA + {22'd0, rx_i[12:6]});
                    st <= M_RXBEAT;
                end
            end
            M_RXBEAT: begin
                if (aux_ack && !aux_we) begin
                    if (rx_i[5:0] == 6'd0 && aux_addr >= B_RXA) begin
                        rx_att <= aux_rdata;
                        beat_rd(B_RXW + {18'd0, rx_i[12:2]});
                    end else begin
                        rx_beat <= aux_rdata;
                        rx_lane <= 2'd0;
                        st      <= M_RXWR;
                    end
                end
            end
            M_RXWR: begin                         // four words of a beat
                if (rx_i < rx_n) begin
                    rx_wr_en   <= 1'b1;
                    rx_wr_addr <= rx_i[11:0];
                    rx_wr_data <= rx_beat[16 * rx_lane +: 16];
                    rx_wr_att  <= rx_att[rx_i[5:0]];
                    rx_i       <= rx_i + 13'd1;
                end
                rx_lane <= rx_lane + 2'd1;
                if (rx_lane == 2'd3 || rx_i + 13'd1 >= rx_n) begin
                    if (rx_i + 13'd1 >= rx_n)          st <= M_RXLOAD;
                    else if (rx_i[5:0] == 6'd63)       st <= M_RXATT;
                    else if (!aux_req) begin
                        beat_rd(B_RXW + {18'd0, rx_i[12:2] + 11'd1});
                        st <= M_RXBEAT;
                    end
                end
            end
            M_RXLOAD: begin
                rx_load       <= 1'b1;
                rx_load_count <= rx_n;
                rxload_count  <= rxload_count + 16'd1;
                status_t      <= 16'd0;     // refresh the status at once
                res_rd        <= {3'd0, rx_n};
                st            <= M_RES;
            end
            // ---- a transmitted packet, out to its slot ----------------------
            M_TXHOLD: if (hp_between && stall_ph == 2'd0) begin
                tx_i <= 11'd0;
                st   <= (tx_len == 11'd0) ? M_TXLEN : M_TXADDR;
            end else begin
                stall_tick();
            end
            M_TXADDR: begin                       // the buffer reads one clock late
                tx_rd_addr <= tx_i[9:0];
                st         <= M_TXWAIT;
            end
            M_TXWAIT: st <= M_TXDATA;
            M_TXDATA: begin
                tx_beat[16 * tx_i[1:0] +: 16] <= tx_rd_data;
                if (tx_i[1:0] == 2'd3 || tx_i + 11'd1 >= tx_len) st <= M_TXBEAT;
                else begin
                    tx_i <= tx_i + 11'd1;
                    st   <= M_TXADDR;
                end
            end
            M_TXBEAT: if (!aux_req) begin
                beat_wr(B_TX + {18'd0, tx_count[2:0], 8'd0} + {20'd0, tx_i[9:2]}, tx_beat);
                tx_beat <= 64'd0;
                if (tx_i + 11'd1 >= tx_len) st <= M_TXLEN;
                else begin
                    tx_i <= tx_i + 11'd1;
                    st   <= M_TXADDR;
                end
            end
            M_TXLEN: if (!aux_req) begin
                beat_wr(B_TXLEN + {26'd0, tx_count[2:0]}, {53'd0, tx_len});
                tx_count <= tx_count + 16'd1;
                tx_pend  <= 1'b0;
                status_t <= 16'd0;          // refresh the status at once
                st       <= M_IDLE;
            end
            default: st <= M_IDLE;
            endcase
        end
    end
endmodule
