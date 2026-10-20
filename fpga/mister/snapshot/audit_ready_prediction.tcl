# Reporting only. Run AFTER the Serial73 builder and all other STA processes
# have ended. No clock/exception/placement change or fitter rerun is made.
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
 set flags [get_registers {*|snapshot_msa_flags_ready[*]}]
 set step [get_registers {*|network_step*}]
 set count [get_collection_size $flags]
 set step_count [get_collection_size $step]
 if {$count!=8 || $step_count<1} {error "Missing actual readiness/advance registers: flags=$count step=$step_count"}
 puts "READY_PREDICTION_CORNER $name flags=$count step=$step_count"
 foreach_in_collection reg $flags {
  puts "READY_PREDICTION_REGISTER $name [get_register_info -name $reg]"
 }
 foreach mode {setup hold} {
  report_timing -$mode -to $flags -npaths 64 -detail full_path -file ready73-into-$mode-$name.rpt
  report_timing -$mode -from $flags -npaths 64 -detail full_path -file ready73-out-$mode-$name.rpt
  report_timing -$mode -from $flags -to $step -npaths 64 -detail full_path -file ready73-cache-to-step-$mode-$name.rpt
  report_timing -$mode -from $step -to $flags -npaths 64 -detail full_path -file ready73-step-to-cache-$mode-$name.rpt
 }
 delete_timing_netlist
}
project_close
