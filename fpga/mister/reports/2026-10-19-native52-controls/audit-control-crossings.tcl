# Post-fit reports only. No timing exceptions or SDC mutations.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 foreach {pattern label} {
  {*|ContA_full_snapshot_experiment:b_ContA|*} conta
  {*|ContB_full_snapshot_experiment:b_ContB|*} contb
 } {
  set board [get_registers $pattern]
  set others [remove_from_collection [get_registers *] $board]
  set board_count [get_collection_size $board]
  set other_count [get_collection_size $others]
  if {$board_count==0 || $other_count==0} {error "Missing $label/core register coverage"}
  puts "CONTROL_CROSSING_CORNER $name $label board=$board_count others=$other_count"
  foreach mode {setup hold} {
   report_timing -$mode -from $board -to $others -npaths 12 -detail full_path -file native52-$label-out-$mode-$name.rpt
   report_timing -$mode -from $others -to $board -npaths 12 -detail full_path -file native52-$label-in-$mode-$name.rpt
  }
 }
 delete_timing_netlist
}
project_close
