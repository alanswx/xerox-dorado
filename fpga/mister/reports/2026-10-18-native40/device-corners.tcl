project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four supported industrial corners, found $corners"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 puts "AUDIT_DEVICE_CORNER $name"
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths 3 -detail full_path -file native40-$mode-$name.rpt
 }
 foreach mode {setup hold} {
  report_timing -$mode -from [get_ports {SDRAM_DQ[*]}] -npaths 64 -detail full_path -file native40-sdram-input-$mode-$name.rpt
  report_timing -$mode -to [get_ports {SDRAM_DQ[*] SDRAM_A[*] SDRAM_BA[*] SDRAM_nCS SDRAM_nRAS SDRAM_nCAS SDRAM_nWE SDRAM_DQML SDRAM_DQMH}] -npaths 128 -detail full_path -file native40-sdram-output-$mode-$name.rpt
 }
 report_min_pulse_width -file native40-mpw-$name.rpt
 report_clocks -file native40-clocks-$name.rpt
 report_ucp -file native40-ucp-$name.rpt
 delete_timing_netlist
}
project_close
