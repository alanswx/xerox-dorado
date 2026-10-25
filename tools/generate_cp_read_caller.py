#!/usr/bin/env python3
"""Compose the original828+16 publication seam with one tagged SRAM port.

This is the single-event staged-path experiment. The ordinary paired packet
path and full native source/clock/visible-R wiring remain integration gates.
Keep the actual caller's sole owner/context write block; prefix cache, read
stage, SRAM write and external visible-R enable share the same commit.
"""
from generate_owned_r_packet import native_publication


def generate(caller,functions):
    block,held=native_publication(caller)
    return '''`default_nettype none
module dorado_cp_read_caller(
 input wire storage_clk,reset,contb_advance,prefix_ready,
 input wire clock_after,input wire [400:0] pins_after,
 input wire [843:0] r_front_next,
 input wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL,
 input wire read_needed,read_accept,
 input wire [31:0] before_address,interior_address,
 input wire [3:0] before_select,interior_select,
 input wire [1:0] write_n,input wire [15:0] write_data,
 output wire [843:0] r_front_held,output wire [400:0] before_pins,
 output wire before_root,control_ready,owner_commit,
 output wire [17:0] before_data,capture_data,output wire capture_valid,
 output wire stage_active,stage_reading,stage_busy,stage_tag_match,
 output wire [31:0] physical_address,output wire [3:0] physical_select);
 FUNCTIONS
 // Exact original native ownership. These registers have no reset or init.
 reg [571:0] r_front_lower_q;reg [255:0] r_front_upper_q;
 reg [400:0] r_front_before;reg r_front_clock_before;
 assign r_front_held=HELD;
 assign before_pins=r_front_before;assign before_root=r_front_clock_before;
 wire [400:0] r_front_live=cp_source_pins(pins_after,cp_bus(pins_after));
 wire CLK_ca_p_=clock_after;
 wire read_ready;
 assign control_ready=prefix_ready && read_ready;
 assign owner_commit=contb_advance && control_ready;
 BLOCK
 wire [17:0] physical_read,saved_before;
 assign before_data=stage_active ? saved_before:physical_read;
 // Start preparation independently of contb_advance: that signal may itself
 // be held by this ready barrier. No circular request/ready dependency.
 dorado_r_interior_read_tagged_stage stage(storage_clk,reset,prefix_ready,read_needed,read_accept,owner_commit,
  before_address,interior_address,before_select,interior_select,physical_read,
  physical_address,physical_select,stage_active,stage_reading,capture_valid,read_ready,
  saved_before,capture_data,stage_busy,stage_tag_match);
 // Sole SRAM owner. A pending or stale read cannot authorize a write.
 dorado_r_file_ports files(storage_clk,owner_commit,write_n,before_address,before_select,
  write_data,physical_address,physical_select,physical_read[15:0],physical_read[17:16]);
endmodule
`default_nettype wire
'''.replace('FUNCTIONS',functions).replace('HELD',held).replace('BLOCK',block)
