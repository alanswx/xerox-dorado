// =============================================================================
// dorado_rtl_storage_ddr.sv — Main storage in the MiSTer's DDR3
// =============================================================================
// Serves the storage munch port (see dorado_rtl_storage.sv) from the MiSTer
// framework's DDRAM interface, an Avalon-MM master of 64-bit beats: a munch
// of 16 words is 4 beats, word i in beat i/4, lane i%4. Every access is a
// single beat (BURSTCNT 1), as the Sorgelig cores' DDRAM clients do: a read
// is four pipelined one-beat reads, a write is one-beat writes of the beats
// whose words are being written. The first build used 4-beat bursts and on
// MiSTer14 a burst write came out shifted two beats; that left the HPS
// SDRAM port stuck (reads accepted and never answered, across core reloads,
// until the MiSTer was rebooted), so bursts have not been retested. Storage
// therefore lies in DDR as plain little-endian 16-bit words from DDR_BASE,
// word w at byte DDR_BASE*8 + 2*w, where the HPS can read and write it.
//
// After reset the region is cleared to zero (the block RAM it replaces
// powered up zero); `ready` rises when that is done. With CLEAR = 0 the
// region is left as it is (the HPS loads a checkpoint's storage into it) and
// `ready` rises at once.
//
// A second client, the aux port, makes single-beat accesses anywhere in the
// window (the HPS mailbox, dorado_rtl_mailbox); storage requests go first.
// =============================================================================

module dorado_rtl_storage_ddr #(
    parameter int          STORAGE_WORDS = 4 * 1024 * 1024,
    parameter logic [28:0] DDR_BASE      = 29'h0600_0000,    // byte 0x3000_0000
    parameter bit          CLEAR         = 1'b1
) (
    input  logic         clk,
    input  logic         rst,
    output logic         ready,

    input  logic         req,
    input  logic         we,
    input  logic [31:0]  addr,
    input  logic [15:0]  mask,
    input  logic [255:0] wline,
    output logic         ack,
    output logic [255:0] rline,
    output logic [63:0]  dbg,      // {state, flags, read cmds, read beats, write beats}

    input  logic         aux_req,
    input  logic         aux_we,
    input  logic [28:0]  aux_addr,     // beat address
    input  logic [63:0]  aux_wdata,
    input  logic [7:0]   aux_be,
    output logic         aux_ack,
    output logic [63:0]  aux_rdata,

    input  logic         DDRAM_BUSY,
    output logic [7:0]   DDRAM_BURSTCNT,
    output logic [28:0]  DDRAM_ADDR,
    input  logic [63:0]  DDRAM_DOUT,
    input  logic         DDRAM_DOUT_READY,
    output logic         DDRAM_RD,
    output logic [63:0]  DDRAM_DIN,
    output logic [7:0]   DDRAM_BE,
    output logic         DDRAM_WE
);
    localparam int MUNCHES = STORAGE_WORDS / 16;
    localparam int MAW     = $clog2(MUNCHES);

    typedef enum logic [2:0] { S_CLEAR, S_IDLE, S_RD, S_RDW, S_WR, S_AUX } state_t;
    state_t        st;
    logic [MAW:0]  clr;            // munch being cleared
    logic [1:0]    beat;
    logic [255:0]  line;           // write data being sent
    logic [15:0]   wmask;
    logic [28:0]   wbase;          // the munch's first beat
    logic [1:0]    ri;             // read beats issued - 1
    logic [11:0]   n_rdcmd;
    logic [15:0]   n_rdbeat, n_wrbeat;
    logic [11:0]   n_busy;          // clocks BUSY was high, saturating
    assign dbg = {1'b0, st, ready, DDRAM_BUSY, DDRAM_RD, DDRAM_WE, DDRAM_DOUT_READY,
                  n_rdcmd, n_rdbeat, n_wrbeat, n_busy[11:1]};

    function automatic logic [7:0] beat_be(input logic [15:0] m, input logic [1:0] b);
        logic [3:0] w;
        w = m[4 * b +: 4];
        return {w[3], w[3], w[2], w[2], w[1], w[1], w[0], w[0]};
    endfunction

    logic          aux_cur, aux_we_q;          // S_AUX: the aux access in progress
    logic [63:0]   aux_d;
    logic [7:0]    aux_be_q;
    assign DDRAM_DIN = aux_cur ? aux_d : line[64 * beat +: 64];
    // A read carries all byte enables, as the MiSTer cores' clients do.
    assign DDRAM_BE  = DDRAM_RD ? 8'hFF : (aux_cur ? aux_be_q : beat_be(wmask, beat));

    always_ff @(posedge clk) begin
        ack <= 1'b0;
        aux_ack <= 1'b0;
        if (DDRAM_RD && !DDRAM_BUSY) n_rdcmd  <= n_rdcmd + 1'd1;
        if (DDRAM_DOUT_READY)        n_rdbeat <= n_rdbeat + 1'd1;
        if (DDRAM_WE && !DDRAM_BUSY) n_wrbeat <= n_wrbeat + 1'd1;
        if (DDRAM_BUSY && n_busy != 12'hFFF) n_busy <= n_busy + 1'd1;
        if (rst) begin
            n_rdcmd  <= '0;
            n_rdbeat <= '0;
            n_wrbeat <= '0;
            n_busy   <= '0;
            st             <= CLEAR ? S_CLEAR : S_IDLE;
            ready          <= !CLEAR;
            aux_cur        <= 1'b0;
            clr            <= '0;
            beat           <= 2'd0;
            line           <= '0;
            wmask          <= 16'hFFFF;
            DDRAM_RD       <= 1'b0;
            DDRAM_WE       <= 1'b0;
            DDRAM_BURSTCNT <= 8'd4;
            DDRAM_ADDR     <= DDR_BASE;
        end else begin
            case (st)
            S_CLEAR: begin
                // one zero beat at a time
                DDRAM_BURSTCNT <= 8'd1;
                if (!DDRAM_WE) begin
                    if (clr[MAW]) begin                 // MUNCHES is a power of two
                        ready <= 1'b1;
                        st    <= S_IDLE;
                    end else begin
                        DDRAM_ADDR <= DDR_BASE + {clr[MAW-1:0], beat};
                        DDRAM_WE   <= 1'b1;
                    end
                end else if (!DDRAM_BUSY) begin
                    DDRAM_WE <= 1'b0;
                    if (beat == 2'd3) clr <= clr + 1'b1;
                    beat <= beat + 2'd1;
                end
            end
            S_IDLE: begin
                if (!(req && !ack) && aux_req && !aux_ack) begin
                    aux_cur        <= 1'b1;
                    aux_we_q       <= aux_we;
                    aux_d          <= aux_wdata;
                    aux_be_q       <= aux_be;
                    DDRAM_ADDR     <= aux_addr;
                    DDRAM_BURSTCNT <= 8'd1;
                    DDRAM_RD       <= !aux_we;
                    DDRAM_WE       <= aux_we;
                    st             <= S_AUX;
                end else if (req && !ack) begin
                    wbase <= DDR_BASE + {addr[MAW+3:4], 2'b00};
                    beat  <= 2'd0;
                    if (we) begin
                        line  <= wline;
                        wmask <= mask;
                        st    <= S_WR;
                    end else begin
                        DDRAM_ADDR     <= DDR_BASE + {addr[MAW+3:4], 2'b00};
                        DDRAM_BURSTCNT <= 8'd1;
                        DDRAM_RD       <= 1'b1;
                        ri             <= 2'd0;
                        st             <= S_RD;
                    end
                end
            end
            S_RD: begin
                // four one-beat reads, each taken when BUSY is low; answers
                // (in order) may start arriving before the last is issued
                if (DDRAM_DOUT_READY) begin
                    rline[64 * beat +: 64] <= DDRAM_DOUT;
                    beat <= beat + 2'd1;
                end
                if (!DDRAM_BUSY) begin
                    if (ri == 2'd3) begin
                        DDRAM_RD <= 1'b0;
                        st       <= S_RDW;
                    end else begin
                        ri         <= ri + 2'd1;
                        DDRAM_ADDR <= wbase + {27'd0, ri + 2'd1};
                    end
                end
            end
            S_RDW: begin
                if (DDRAM_DOUT_READY) begin
                    rline[64 * beat +: 64] <= DDRAM_DOUT;
                    if (beat == 2'd3) begin
                        ack <= 1'b1;
                        st  <= S_IDLE;
                    end
                    beat <= beat + 2'd1;
                end
            end
            S_WR: begin
                // each beat with a word to write is one single-beat write
                if (DDRAM_WE) begin
                    if (!DDRAM_BUSY) begin
                        DDRAM_WE <= 1'b0;
                        if (beat == 2'd3) begin
                            ack <= 1'b1;
                            st  <= S_IDLE;
                        end
                        beat <= beat + 2'd1;
                    end
                end else if (wmask[4 * beat +: 4] != 4'd0) begin
                    DDRAM_ADDR     <= wbase + {27'd0, beat};
                    DDRAM_BURSTCNT <= 8'd1;
                    DDRAM_WE       <= 1'b1;
                end else begin
                    if (beat == 2'd3) begin
                        ack <= 1'b1;
                        st  <= S_IDLE;
                    end
                    beat <= beat + 2'd1;
                end
            end
            S_AUX: begin
                // one beat: the command is taken when BUSY is low; a read
                // then waits for its answer
                if (DDRAM_RD || DDRAM_WE) begin
                    if (!DDRAM_BUSY) begin
                        DDRAM_RD <= 1'b0;
                        DDRAM_WE <= 1'b0;
                        if (aux_we_q) begin
                            aux_ack <= 1'b1;
                            aux_cur <= 1'b0;
                            st      <= S_IDLE;
                        end
                    end
                end else if (DDRAM_DOUT_READY) begin
                    aux_rdata <= DDRAM_DOUT;
                    aux_ack   <= 1'b1;
                    aux_cur   <= 1'b0;
                    st        <= S_IDLE;
                end
            end
            default: st <= S_IDLE;
            endcase
        end
    end
endmodule
