# Additional POST-FIT audit only. No SDC changes or timing exceptions.
# Run only after the existing Native51 map/fit/asm/STA/device audit completes.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions: $corners"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 set board [get_registers {*|ContB_full_snapshot_experiment:b_ContB|*}]
 set others [remove_from_collection [get_registers *] $board]
 set board_count [get_collection_size $board]
 set other_count [get_collection_size $others]
 if {$board_count==0 || $other_count==0} {error "Missing ContB/full-core register coverage"}
 puts "CONTB_CROSSING_CORNER $name board=$board_count others=$other_count"
 foreach mode {setup hold} {
  report_timing -$mode -from $board -to $others -npaths 12 -detail full_path -file native51-contb-out-$mode-$name.rpt
  report_timing -$mode -from $others -to $board -npaths 12 -detail full_path -file native51-contb-in-$mode-$name.rpt
 }
 delete_timing_netlist
}
project_close
