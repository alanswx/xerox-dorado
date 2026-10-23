// Experimental ownership for the original 32 input +16 visible-R latch bits.
// Read/visible gates are actual current levels plus the upcoming capture
// levels from their owner. No generated clock, reset or added guest cycle.
// Keep one current RAM/mux data port. Data and gates update after the host
// edge; closing captures the pre-edge physical view. Open views then settle
// combinationally, including a simultaneous read-close/visible-open handoff.
// Actual controller, l07 flags, functional RAM and native integration remain
// separate acceptance gates; this module does not replace those producers.
`default_nettype none
module dorado_r_operand_latches(
 input wire host_clk,advance,
 input wire [1:0] read_gate,read_gate_next,visible_gate,visible_gate_next,
 input wire [31:0] read_data,input wire [1:0] bypass,
 output wire [31:0] read_q,output wire [15:0] visible_q,
 output wire [47:0] held_state);
 reg [31:0] held_data;
 reg [15:0] held_r;
 wire [31:0] held_data_next;
 dorado_r_read_window_step data_latches(held_data,read_gate,read_gate_next,
   read_data,read_data,held_data_next,read_q,,);
 wire [15:0] selected={bypass[1] ? read_q[15:8] : read_q[31:24],
                        bypass[0] ? read_q[7:0] : read_q[23:16]};
 generate for(genvar half=0;half<2;half=half+1)begin: visible_latch
   assign visible_q[8*half+:8]=visible_gate[half] ? held_r[8*half+:8] : selected[8*half+:8];
   always @(posedge host_clk)if(advance && !visible_gate[half] && visible_gate_next[half])
     held_r[8*half+:8]<=visible_q[8*half+:8];
 end endgenerate
 always @(posedge host_clk)if(advance)held_data<=held_data_next;
 assign held_state={held_r,held_data};
endmodule
`default_nettype wire
