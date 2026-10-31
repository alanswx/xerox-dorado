// =============================================================================
// dorado_rtl_storage.sv — Main storage behind a munch port
// =============================================================================
// The memory system moves storage a munch at a time (HM section 5: a cache
// line, and a fast-I/O transfer, is one 16-word munch), so the CPU reaches
// storage through one port that transfers a whole aligned munch:
//
//   req      held until ack; we = write
//   addr     word address; the low four bits are ignored (munch aligned)
//   mask     write: the words of the munch to write (word i = bit i)
//   wline    write data, word i in [16*i +: 16]
//   ack      one clock: the write is done, or rline holds the munch
//   rline    read data, word i in [16*i +: 16]
//
// This module serves the port from block RAM, one word per clock. On the
// MiSTer the same port is served from DDR3 instead (dorado_rtl_storage_ddr),
// where 16 words are 4 beats of 64 bits.
// =============================================================================

module dorado_rtl_storage #(
    parameter int STORAGE_WORDS = 4 * 1024 * 1024
) (
    input  logic         clk,
    input  logic         rst,
    input  logic         req,
    input  logic         we,
    input  logic [31:0]  addr,
    input  logic [15:0]  mask,
    input  logic [255:0] wline,
    output logic         ack,
    output logic [255:0] rline
);
    localparam int STOR_AW = $clog2(STORAGE_WORDS);

    (* ramstyle = "M10K" *) logic [15:0] storage [0:STORAGE_WORDS-1];
    logic [15:0] storage_q;
    logic [STOR_AW-1:0] a;
    logic        a_we;
    logic [15:0] a_wd;

    // i counts the words of the munch; a read's data arrives a clock later.
    logic        busy;
    logic [4:0]  i;

    always_comb begin
        a    = {addr[STOR_AW-1:4], i[3:0]};
        a_we = busy && we && i < 5'd16 && mask[i[3:0]];
        a_wd = wline[16 * i[3:0] +: 16];
    end

    always_ff @(posedge clk) begin
        if (a_we) storage[a] <= a_wd;
        storage_q <= storage[a];
    end

    always_ff @(posedge clk) begin
        ack <= 1'b0;
        if (rst) begin
            busy <= 1'b0;
            i    <= 5'd0;
        end else if (!busy) begin
            if (req && !ack) begin
                busy <= 1'b1;
                i    <= 5'd0;
            end
        end else begin
            if (i != 5'd0) rline[16 * (i - 5'd1) +: 16] <= storage_q;
            if (i == 5'd16) begin
                busy <= 1'b0;
                ack  <= 1'b1;
            end
            i <= i + 5'd1;
        end
    end
endmodule
