# Reporting only. The qualified parent's SDC and exceptions are unchanged.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
set inventoried 0
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 set backend [get_registers {*|dorado_storage_sdram_burst:u_sdram|*}]
 set others [remove_from_collection [get_registers *] $backend]
 foreach {label pattern} {
  words {*|dorado_storage_sdram_burst:u_sdram|line_words*}
  tags {*|dorado_storage_sdram_burst:u_sdram|line_tag*}
  valid {*|dorado_storage_sdram_burst:u_sdram|line_valid*}
  capture {*|dorado_storage_sdram_burst:u_sdram|dq_capture*}
  response {*|dorado_storage_sdram_burst:u_sdram|rsp_data*}
 } {
  set burst_group($label) [get_registers $pattern]
  set count [get_collection_size $burst_group($label)]
  if {$count==0} {error "No physical BL8 $label registers"}
  puts "BURST_REGISTER_COUNT $name $label $count"
 }
 if {[get_collection_size $backend]==0 || [get_collection_size $others]==0} {
  error "Missing backend/core register coverage"
 }
 if {!$inventoried} {
  foreach_in_collection node $backend {
   puts "BURST_REGISTER [get_register_info -name $node]"
  }
  set inventoried 1
 }
 foreach {label from to} [list \
  internal $backend $backend \
  fill $backend $burst_group(words) \
  cached-read $burst_group(words) $burst_group(response) \
  tag-select $burst_group(tags) $burst_group(response) \
  input-capture $burst_group(capture) $burst_group(words) \
  request $others $backend \
  completion $backend $others] {
  foreach mode {setup hold} {
   report_timing -$mode -from $from -to $to -npaths 32 -detail full_path -file burst-$label-$mode-$name.rpt
  }
 }
 set fault_observer [get_registers -nowarn {*|dorado_boot_fault_monitor:u_boot_fault_monitor|*}]
 if {[get_collection_size $fault_observer]>0} {
  puts "BOOT_FAULT_REGISTERS $name [get_collection_size $fault_observer]"
  foreach mode {setup hold} {
   report_timing -$mode -to $fault_observer -npaths 32 -detail full_path -file burst-first-fault-$mode-$name.rpt
  }
 }
 set selftest [get_registers {*|dorado_storage_selftest:u_initialize|*}]
 set outside_selftest [remove_from_collection [get_registers *] $selftest]
 set count [get_collection_size $selftest]
 if {$count==0} {error "Missing actual selftest registers"}
 puts "STORAGE_SELFTEST_REGISTERS $name $count"
 if {$name==[lindex $corners 0]} {
  foreach_in_collection node $selftest {
   puts "STORAGE_SELFTEST_REGISTER [get_register_info -name $node]"
  }
 }
 foreach {label from to} [list selftest-internal $selftest $selftest \
  selftest-input $outside_selftest $selftest selftest-output $selftest $outside_selftest] {
  foreach mode {setup hold} {
   report_timing -$mode -from $from -to $to -npaths 32 -detail full_path -file burst-$label-$mode-$name.rpt
  }
 }
 set trace [get_registers {*|dorado_storage_sdram_burst:u_sdram|first_burst* *|dorado_storage_sdram_burst:u_sdram|burst_observation*}]
 set count [get_collection_size $trace]
 if {$count==0} {error "Missing first-burst trace registers"}
 puts "BURST_TRACE_COUNT $name $count"
 if {$name==[lindex $corners 0]} {
  foreach_in_collection node $trace {
   puts "BURST_TRACE_REGISTER [get_register_info -name $node]"
  }
 }
 foreach mode {setup hold} {
  report_timing -$mode -to $trace -npaths 32 -detail full_path -file burst-trace-capture-$mode-$name.rpt
 }
 delete_timing_netlist
}
project_close
