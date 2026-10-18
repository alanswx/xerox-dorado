// Experimental direct-enable translation of ProcH20 h22 and ProcL20 d17.
// HM 3.8/Table10: LC writes R/T; HM Table11a FF075 is TgetsMd.
// Original sheets: ProcH-Rev-Ce/ProcL-Rev-Ci, PDF page22, printed20.
// High half clocks on Clock0'Da; low half on SHCP'C. These are NOT
// interchangeable: the caller must derive and prove each capture enable.
// No reset is invented for the original MC10176 (which has none).
// Not instantiated in the generated machine or FPGA build.
`default_nettype none
module dorado_t_controls (
    input wire clk,
    input wire capture_h, capture_l,
    input wire [2:0] lc,        // {LC.0,LC.1,LC.2}, Xerox bit order
    input wire [7:0] ff,        // {FA[1:0],FB[2:0],FC[2:0]}
    input wire curr_ne_next_h, curr_ne_next_l,
    // Normalized by signal name, not pin order (R select pins are swapped
    // between boards): {Last=Curr',TbWriteEn',RbWriteEn',TbSelMd,RbSelMd',RbSelMd}.
    output reg [5:0] controls_h,
    output reg [5:0] controls_l
);
    wire t_md = (!lc[2] && lc[1]) || ff == 8'o075;
    wire r_write_n = !(lc[2] || (lc[1] && !lc[0]));
    wire t_write_n = !(lc[0] || (lc[1] && !lc[2]));
    wire [4:0] decoded = {t_write_n,r_write_n,t_md,lc[1],!lc[1]};
    always @(posedge clk) begin
        if(capture_h) controls_h <= {curr_ne_next_h,decoded};
        if(capture_l) controls_l <= {curr_ne_next_l,decoded};
    end
endmodule
`default_nettype wire
