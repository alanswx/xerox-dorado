# Reporting only, after the native builder is terminal. Never run concurrently
# with another stage in the same Quartus database. Retain original SDC intact.
# Explicitly cover central storage/new evaluator paths that the historical
# per-board crossing reports cannot identify by their former package names.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
foreach corner $corners {
 create_timing_netlist
 set_operating_conditions $corner
 read_sdc
 update_timing_netlist
 set central [get_registers {*|dorado_native_backplane:m|r_front_q*}]
 set rest [remove_from_collection [get_registers *] $central]
 set count [get_collection_size $central]
 # Live synthesis may remove physical outputs with no consumers. Record the
 # actual count; the source inventory's 752 bits is not a fitted area claim.
 if {$count==0 || $count>752} {error "Missing/invalid central Q register coverage: $count"}
 puts "R_FRONT_READY_CORNER $corner central_Q=$count rest=[get_collection_size $rest]"
 foreach mode {setup hold} {
  report_timing -$mode -from $central -to $central -npaths 20 -detail full_path -file r-front-ready-internal-$mode-$corner.rpt
  report_timing -$mode -from $central -to $rest -npaths 20 -detail full_path -file r-front-ready-out-$mode-$corner.rpt
  report_timing -$mode -from $rest -to $central -npaths 20 -detail full_path -file r-front-ready-in-$mode-$corner.rpt
 }
 foreach {pattern label} {
  {*|ContA_full_snapshot_experiment:b_ContA|*} conta
  {*|ContB_full_snapshot_experiment:b_ContB|*} contb
  {*|ProcH_full_snapshot_experiment:b_ProcH|*} proch
  {*|ProcL_full_snapshot_experiment:b_ProcL|*} procl
  {*|IFU_full_snapshot_experiment:b_IFU|*} ifu
  {*|MemC_full_snapshot_experiment:b_MemC|*} memc
  {*|MemX_full_snapshot_experiment:b_MemX|*} memx
 } {
  set board [get_registers $pattern]
  set count [get_collection_size $board]
  if {$count==0} {error "Missing remaining native $label producer/consumer registers"}
  puts "R_FRONT_READY_BOUNDARY $corner $label registers=$count"
  foreach mode {setup hold} {
   report_timing -$mode -from $board -to $central -npaths 12 -detail full_path -file r-front-ready-$label-in-$mode-$corner.rpt
   report_timing -$mode -from $central -to $board -npaths 12 -detail full_path -file r-front-ready-$label-out-$mode-$corner.rpt
  }
 }
 delete_timing_netlist
}
project_close
