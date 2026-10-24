// Forward one original RM/STK write into a later read of the SAME bank ports.
// Use only when the packet consumer has checked both physical address vectors
// and bank selections equal. The physical SRAM commits once at packet close.
// Data bytes H/L occupy bits7:0/15:8; odd parity occupies bits16/17.
// HM section2.3/Table4: retain the original pre-edge write-data epoch.
`default_nettype none
module dorado_r_pair_read_forward(
 input wire [17:0] original_read,input wire [15:0] write_data,
 input wire [1:0] write_n,input wire [3:0] select_n,
 output wire [17:0] later_read,output wire [1:0] forwarded);
 assign forwarded[0]=!write_n[0] && !(&select_n[1:0]);
 assign forwarded[1]=!write_n[1] && !(&select_n[3:2]);
 assign later_read[7:0]=forwarded[0] ? write_data[7:0] : original_read[7:0];
 assign later_read[15:8]=forwarded[1] ? write_data[15:8] : original_read[15:8];
 assign later_read[16]=forwarded[0] ? ~^write_data[7:0] : original_read[16];
 assign later_read[17]=forwarded[1] ? ~^write_data[15:8] : original_read[17];
endmodule
`default_nettype wire
