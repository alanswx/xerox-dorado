// =============================================================================
// dorado_rtl_assist.sv — Loader assists for the Alto-world (AEmu) boot
// =============================================================================
// The C emulator boots AEmu with three interventions that stand in for parts
// of the machine not otherwise modelled on this path (dorado/src/machine.c;
// the Verilator Suite 6 harness performs the same ones by poking arrays).
// This block performs them in RTL, through the CPU's assist port, so the FPGA
// boots the same way the cosimulation does:
//
//  1. Cold-Alto initialisation, once, when the emulator task (0) is about to
//     execute EBoot (02005/02006): AC0-AC3 (STK[1..4]) = 0, the Alto I/O page
//     VA 177000-177777 = 177777 (no device answers), and 177740-177757 =
//     177760.
//  2. Keyboard words: every 256 microinstructions while the IFU is idle (the
//     microcode boot, before the Alto program runs), the four Alto keyboard
//     words at VA 177034-177037 are written into storage and any resident
//     cache line, with BS held down (word 0 = 177776) until the EFTP transfer
//     starts, so AEmu's Start.mc chooses the Ethernet boot (EBoot). The
//     controller's own word 0 is set the same way.
//  3. Display wakeups are enabled (display_wake_gate) once the Alto display
//     list head DASTART (VA 420) is non-zero or a line is pending; sampled
//     with (2).
//
// Each check is made once per microinstruction, between microinstructions,
// with the CPU held (ap_hold) while the writes are done.
// =============================================================================

module dorado_rtl_assist (
    input  logic        clk,
    input  logic        rst,
    input  logic        en,

    // CPU state between microinstructions
    input  logic        cpu_between,
    input  logic [63:0] cycles,
    input  logic [11:0] real_pc,
    input  logic [3:0]  ctask,
    input  logic        ifu_active,
    input  logic        eftp_started,
    input  logic        next_wcb_any,

    // CPU assist port
    output logic        ap_hold,
    output logic        ap_req,
    output logic        ap_we,
    output logic        ap_stk,
    output logic        ap_cache,
    output logic [27:0] ap_va,
    output logic [15:0] ap_wd,
    input  logic        ap_done,
    input  logic [15:0] ap_rd,

    // Controller keyboard word 0, and the display gate
    output logic        kbd0_poke,
    output logic [15:0] kbd0_val,
    output logic        display_wake_gate
);
    typedef enum logic [2:0] { A_IDLE, A_COLD_STK, A_COLD_IO, A_KBD, A_DASTART, A_FIN } astate_t;
    astate_t     st;
    logic        cold_done;
    logic [63:0] last_cycles;
    logic        evaluated;           // a check has been made at last_cycles
    logic        do_kbd;              // the current visit also does (2)/(3)
    logic [8:0]  n;                   // word counter within a phase
    logic        busy_req;            // a request is outstanding

    wire cond_cold = !cold_done && ctask == 4'd0 &&
                     (real_pc == 12'o2005 || real_pc == 12'o2006);
    wire cond_kbd  = (cycles[7:0] == 8'd0) && !ifu_active;
    wire fresh     = !evaluated || cycles != last_cycles;
    wire need      = en && cpu_between && fresh && (cond_cold || cond_kbd);

    // Held combinationally, so the CPU cannot leave C_FETCH in the clock
    // the conditions become true.
    assign ap_hold = need || (st != A_IDLE);

    task automatic issue(input logic we, input logic stk, input logic cache,
                         input logic [27:0] va, input logic [15:0] wd);
        ap_req   <= 1'b1;
        ap_we    <= we;
        ap_stk   <= stk;
        ap_cache <= cache;
        ap_va    <= va;
        ap_wd    <= wd;
        busy_req <= 1'b1;
    endtask

    always_ff @(posedge clk) begin
        kbd0_poke <= 1'b0;
        if (rst || !en) begin
            st                <= A_IDLE;
            cold_done         <= 1'b0;
            last_cycles       <= 64'd0;
            evaluated         <= 1'b0;
            do_kbd            <= 1'b0;
            n                 <= 9'd0;
            busy_req          <= 1'b0;
            ap_req            <= 1'b0;
            ap_we             <= 1'b0;
            ap_stk            <= 1'b0;
            ap_cache          <= 1'b0;
            ap_va             <= 28'd0;
            ap_wd             <= 16'd0;
            kbd0_val          <= 16'hFFFE;
            display_wake_gate <= 1'b0;
        end else begin
            if (ap_done) begin
                ap_req   <= 1'b0;
                busy_req <= 1'b0;
            end
            case (st)
            A_IDLE: begin
                if (need) begin
                    evaluated   <= 1'b1;
                    last_cycles <= cycles;
                    do_kbd      <= cond_kbd;
                    n           <= 9'd0;
                    if (cond_cold) begin
                        cold_done <= 1'b1;
                        st        <= A_COLD_STK;
                    end else begin
                        st        <= A_KBD;
                    end
                end
            end
            A_COLD_STK: begin                    // STK[1..4] = 0
                if (!busy_req || ap_done) begin
                    if (n == 9'd4) begin
                        n  <= 9'd0;
                        st <= A_COLD_IO;
                    end else begin
                        issue(1'b1, 1'b1, 1'b0, {20'd0, 8'(n + 9'd1)}, 16'd0);
                        n <= n + 9'd1;
                    end
                end
            end
            A_COLD_IO: begin                     // I/O page, storage only
                if (!busy_req || ap_done) begin
                    if (n == 9'd272) begin
                        n  <= 9'd0;
                        st <= do_kbd ? A_KBD : A_FIN;
                    end else begin
                        if (n < 9'd256)
                            issue(1'b1, 1'b0, 1'b0, 28'o177000 + {20'd0, n[7:0]}, 16'o177777);
                        else
                            issue(1'b1, 1'b0, 1'b0, 28'o177740 + {24'd0, n[3:0]}, 16'o177760);
                        n <= n + 9'd1;
                    end
                end
            end
            A_KBD: begin                         // keyboard words, storage + cache
                if (n == 9'd0 && !busy_req) begin
                    kbd0_poke <= 1'b1;
                    kbd0_val  <= eftp_started ? 16'hFFFF : 16'hFFFE;
                end
                if (!busy_req || ap_done) begin
                    if (n == 9'd4) begin
                        n  <= 9'd0;
                        st <= A_DASTART;
                    end else begin
                        issue(1'b1, 1'b0, 1'b1, 28'o177034 + {26'd0, n[1:0]},
                              (n == 9'd0) ? (eftp_started ? 16'hFFFF : 16'hFFFE) : 16'hFFFF);
                        n <= n + 9'd1;
                    end
                end
            end
            A_DASTART: begin                     // read VA 420 through the cache
                if (n == 9'd0 && !busy_req) begin
                    issue(1'b0, 1'b0, 1'b1, 28'o000420, 16'd0);
                    n <= 9'd1;
                end else if (ap_done) begin
                    display_wake_gate <= (ap_rd != 16'd0) || next_wcb_any;
                    st                <= A_FIN;
                end
            end
            A_FIN: begin
                if (!busy_req || ap_done) st <= A_IDLE;
            end
            default: st <= A_IDLE;
            endcase
        end
    end
endmodule
