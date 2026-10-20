// Coupled processor slice with functional RM/STK data/parity ports.
// Experiment only: physical address/select producers remain boundaries.
// Address/select must be stable across a root event; change them only while
// the root is high and the original write gates are closed. Native wiring,
// producer setup/hold and 60ns cadence remain unqualified.
`default_nettype none
module dorado_r_storage_step(
 input wire host_clk,advance,input wire [595:0] state_q,
 input wire clock_before,clock_after,input wire [151:0] pins_before,pins_after,
 input wire [31:0] address,input wire [3:0] select_n,
 output wire [595:0] state_next,output wire [32:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [1:0] r_write_n,output wire [15:0] file_read_data,
 output wire [1:0] file_read_parity);
 wire [151:0] source_before,source_after;
 // The old free R-data/R-parity inputs are replaced by actual RAM outputs.
 assign source_before={pins_before[151:145],file_read_parity,file_read_data,pins_before[126:0]};
 assign source_after={pins_after[151:145],file_read_parity,file_read_data,pins_after[126:0]};
 dorado_r_flags_step child(state_q,clock_before,clock_after,source_before,source_after,
   state_next,clock_events,repeated_events,unsettled,write_n,latch_n,t_latch_n,bypass,
   parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n);
 // Actual j05 OR gates: PreClock0'B | HoldDly | RbWriteEn'. Each physical
 // data/parity SRAM copy receives the same gate. Use the captured HoldDly.
 assign r_write_n={clock_after|!state_next[8]|state_next[10]|!state_next[23]|state_next[99]|state_next[53],
                   clock_after|!state_next[13]|state_next[12]|!state_next[23]|state_next[93]|state_next[37]};
 dorado_r_file_ports files(host_clk,advance,r_write_n,address,select_n,
   state_next[555:540],file_read_data,file_read_parity);
endmodule
`default_nettype wire
