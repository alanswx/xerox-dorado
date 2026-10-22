# Read-only counter inventory/crossings for the opt-in native backing buffer.
# No SDC/exception/project setting is created or changed here.
project_open Dorado
set outdir [file dirname [info script]]
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions"}
foreach corner $corners {
 create_timing_netlist
 set_operating_conditions $corner
 read_sdc
 update_timing_netlist
 set monitor [get_registers {*|dorado_speed_monitor:u_speed_monitor|*}]
 foreach_in_collection node $monitor {puts "CACHE_MONITOR_REGISTER $corner [get_register_info -name $node]"}
 # Quartus17 get_registers accepts Tcl string-match patterns, not -regexp.
 # Quartus also escapes backslashes before applying its bus-name filter.
 # Select with single-character wildcards, then require exact printed names.
 set hits [get_registers {*|dorado_speed_monitor:u_speed_monitor|causes.count?11??*}]
 foreach_in_collection node $hits {
  if {![regexp {\|causes\.count\[11\]\[[0-9]+\](~DUPLICATE(_[0-9]+)?)?$} [get_register_info -name $node]]} {
   error "Unexpected register in cache hit counter collection"
  }
 }
 set hit_signal [get_registers {*|dorado_storage_sdram_burst:u_sdram|cache_hit*}]
 set initialized [get_registers {*|dorado_storage_selftest:u_initialize|*}]
 if {[get_collection_size $hits]<32 || [get_collection_size $hit_signal]<1 || [get_collection_size $initialized]<1} {
  error "Missing real cache hit counter or source state"
 }
 puts "CACHE_HIT_COUNTER_COUNT $corner [get_collection_size $hits]"
 foreach_in_collection node $hits {puts "CACHE_HIT_COUNTER_REGISTER $corner [get_register_info -name $node]"}
 foreach mode {setup hold} {
  report_timing -$mode -to $hits -npaths 64 -detail full_path -file [file join $outdir cache-hit-counter-$mode-$corner.rpt]
  report_timing -$mode -from $hit_signal -to $hits -npaths 32 -detail full_path -file [file join $outdir cache-hit-event-$mode-$corner.rpt]
  report_timing -$mode -from $initialized -to $hits -npaths 32 -detail full_path -file [file join $outdir cache-hit-initialized-$mode-$corner.rpt]
 }
 delete_timing_netlist
}
project_close
