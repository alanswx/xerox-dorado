// =============================================================================
// dorado_rtl_alu_shifter.sv — Xerox Dorado 16-bit ALU & 32-bit Barrel Shifter
// =============================================================================
// Derived from dorado/src/cpu.c (alu_op, shifter_output, field_desc_to_shc).
// Purely combinational single-cycle datapath units.
// =============================================================================

/* verilator lint_off DECLFILENAME */
module dorado_rtl_alu (
    input  logic [5:0]  alufm_entry, // bit 5 = carry_in, bits 4:0 = op (HM Table 9)
    input  logic [15:0] a,
    input  logic [15:0] b,
    input  logic        carry20,     // Table 11a FA=0 FB=2 FC=6 (Carry20)
    output logic [15:0] result,
    output logic        carry_out,
    output logic        overflow,
    output logic        is_arith
);
    logic        carry_in;
    logic [4:0]  op;
    logic [16:0] r;
    logic        low_carry;
    logic        sa, sb, sr;
    logic [15:0] nb;

    always_comb begin
        carry_in  = alufm_entry[5];
        op        = alufm_entry[4:0];
        is_arith  = (op[0] == 1'b0);
        r         = 17'd0;
        low_carry = 1'b0;
        result    = 16'd0;
        carry_out = 1'b0;
        overflow  = 1'b0;
        sa        = a[15];
        sb        = b[15];
        sr        = 1'b0;
        nb        = 16'd0;

        case (op)
            // Logical operations (odd op codes, ignore carry_in)
            5'o01: result = ~a;
            5'o03: result = ~a | ~b;
            5'o05: result = ~a | b;
            5'o07: result = 16'hFFFF;
            5'o11: result = ~a & ~b;
            5'o13: result = ~b;
            5'o15: result = ~(a ^ b);
            5'o17: result = a | ~b;
            5'o21: result = ~a & b;
            5'o23: result = a ^ b;
            5'o25: result = b;
            5'o27: result = a | b;
            5'o31: result = 16'h0000;
            5'o33: result = a & ~b;
            5'o35: result = a & b;
            5'o37: result = a;

            // Arithmetic operations (even op codes)
            5'o00: begin // A + carry_in
                low_carry = (({1'b0, a[3:0]} + {4'd0, carry_in}) >> 4) != 0;
                r         = {1'b0, a} + {16'd0, carry_in} + ((carry20 && !low_carry) ? 17'h0010 : 17'd0);
                result    = r[15:0];
                carry_out = r[16];
                sr        = r[15];
                overflow  = (sa == 1'b0) && (sa != sr);
            end
            5'o06: begin // 2*A + carry_in
                low_carry = (({1'b0, a[3:0], 1'b0} + {5'd0, carry_in}) >> 4) != 0;
                r         = {1'b0, a} + {1'b0, a} + {16'd0, carry_in} + ((carry20 && !low_carry) ? 17'h0010 : 17'd0);
                result    = r[15:0];
                carry_out = r[16];
                sr        = r[15];
                overflow  = (sa != sr);
            end
            5'o14: begin // A + B + carry_in
                low_carry = (({1'b0, a[3:0]} + {1'b0, b[3:0]} + {4'd0, carry_in}) >> 4) != 0;
                r         = {1'b0, a} + {1'b0, b} + {16'd0, carry_in} + ((carry20 && !low_carry) ? 17'h0010 : 17'd0);
                result    = r[15:0];
                carry_out = r[16];
                sr        = r[15];
                overflow  = (sa == sb) && (sa != sr);
            end
            5'o22: begin // A + ~B + carry_in (A - B - 1 + carry_in)
                nb        = ~b;
                low_carry = (({1'b0, a[3:0]} + {1'b0, nb[3:0]} + {4'd0, carry_in}) >> 4) != 0;
                r         = {1'b0, a} + {1'b0, nb} + {16'd0, carry_in} + ((carry20 && !low_carry) ? 17'h0010 : 17'd0);
                result    = r[15:0];
                carry_out = r[16];
                sr        = r[15];
                overflow  = (sa == nb[15]) && (sa != sr);
            end
            5'o36: begin // A + 0xFFFF + carry_in (A - 1 + carry_in)
                low_carry = (({1'b0, a[3:0]} + 5'h0F + {4'd0, carry_in}) >> 4) != 0;
                r         = {1'b0, a} + 17'h0FFFF + {16'd0, carry_in} + ((carry20 && !low_carry) ? 17'h0010 : 17'd0);
                result    = r[15:0];
                carry_out = r[16];
                sr        = r[15];
                overflow  = (sa == 1'b1) && (sa != sr);
            end
            default: begin
                result    = 16'd0;
                carry_out = 1'b0;
                overflow  = 1'b0;
            end
        endcase
    end
endmodule

module dorado_rtl_shifter
    import dorado_rtl_pkg::*;
(
    input  dorado_uinstr_t u,
    input  logic [15:0]    t_in,
    input  logic [15:0]    r_in,
    input  logic [15:0]    id_peek,
    input  logic [15:0]    shc,
    input  logic [15:0]    task_md_in,
    output logic [15:0]    unmasked_lo16,
    output logic [15:0]    out_mask,
    output logic [15:0]    out_fill
);
    logic [15:0] t_val, r_val;
    logic [15:0] sha, shb;
    logic [3:0]  count, rmask_n, lmask_n;
    logic [31:0] in32, cyc;
    logic [15:0] lmask, rmask;
    logic [2:0]  op;
    logic        with_md;

    always_comb begin
        // HM §3.11: TisId/RisId substitutions apply to shifter T and RM/STK inputs
        t_val = ff_is_t_is_id(u) ? id_peek : t_in;
        r_val = ff_is_r_is_id(u) ? id_peek : r_in;

        if (u.bsel[2]) begin
            // FF-controlled shift (BSEL = 4..7)
            sha     = u.bsel[1] ? t_val : r_val;
            shb     = u.bsel[0] ? t_val : r_val;
            count   = u.ff[3:0];
            rmask_n = u.ff[3:0];
            lmask_n = u.ff[7:4];
        end else begin
            // ShC-controlled shift (BSEL = 0..3)
            sha     = shc[13] ? t_val : r_val;
            shb     = shc[12] ? t_val : r_val;
            count   = shc[11:8];
            rmask_n = shc[7:4];
            lmask_n = shc[3:0];
        end

        in32 = {sha, shb};
        if (count == 4'd0) begin
            cyc = in32;
        end else begin
            cyc = (in32 << count) | (in32 >> (6'd32 - {2'd0, count}));
        end
        unmasked_lo16 = cyc[15:0];

        lmask = (lmask_n != 4'd0) ? (16'hFFFF << (5'd16 - {1'b0, lmask_n})) : 16'h0000;
        rmask = (rmask_n != 4'd0) ? ((16'd1 << rmask_n) - 16'd1)            : 16'h0000;

        op = u.aluf[3:1];
        case (op)
            3'd0: begin out_mask = 16'h0000;      with_md = 1'b0; end // ShiftNoMask
            3'd1: begin out_mask = lmask;         with_md = 1'b0; end // ShiftLMask
            3'd2: begin out_mask = rmask;         with_md = 1'b0; end // ShiftRMask
            3'd3: begin out_mask = lmask | rmask; with_md = 1'b0; end // ShiftBothMasks
            3'd4: begin out_mask = 16'h0000;      with_md = 1'b1; end // ShMdNoMask
            3'd5: begin out_mask = lmask;         with_md = 1'b1; end // ShMdLMask
            3'd6: begin out_mask = rmask;         with_md = 1'b1; end // ShMdRMask
            3'd7: begin out_mask = lmask | rmask; with_md = 1'b1; end // ShMdBothMasks
        endcase

        out_fill = with_md ? task_md_in : 16'h0000;
    end
endmodule
