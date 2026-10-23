# Read-only, after the recorded full-core builder and all its stages exit.
# Preserve the existing SDC. Include RAM keeper endpoints in central crossings,
# and identify original visible-R owners separately from synthesis duplicates.
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
 set all [get_keepers *]
 set central [get_registers [list \
  {*|dorado_native_backplane:m|r_front_lower_q*} \
  {*|dorado_native_backplane:m|r_front_upper_q*}]]
 set history [get_registers [list \
  {*|dorado_native_backplane:m|r_front_before*} \
  {*|dorado_native_backplane:m|r_front_clock_before*}]]
 set rest [remove_from_collection $all $central]
 if {[get_collection_size $central]==0 || [get_collection_size $history]==0} {
  error "Missing central owner/history register coverage"
 }
 set primaries [dict create]
 set copies [dict create]
 foreach_in_collection node $central {
  set name [get_register_info -name $node]
  if {![regexp {\|r_front_(lower|upper)_q\[([0-9]+)\](~DUPLICATE[0-9]*)?$} $name match half bit duplicate]} {
   error "Unclassified central owner: $name"
  }
  set limit [expr {$half=="lower" ? 571 : 255}]
  if {$bit>$limit} {error "Owner index outside original encoding: $name"}
  set key "$half/$bit"
  if {$duplicate==""} {dict set primaries $key $name} else {dict set copies $name $key}
  puts "PHYSICAL_NATIVE_OWNER $corner $name"
 }
 dict for {name key} $copies {
  if {![dict exists $primaries $key]} {error "Central duplicate has no original: $name"}
 }
 puts "PHYSICAL_NATIVE_COVERAGE $corner keepers=[get_collection_size $all] central=[get_collection_size $central] primary=[dict size $primaries] duplicates=[dict size $copies] history=[get_collection_size $history]"
 set visible [get_registers [list \
  {*|ProcH_full_snapshot_experiment:b_ProcH|cell_MC10173_snapshot:u_g06|*} \
  {*|ProcH_full_snapshot_experiment:b_ProcH|cell_MC10173_snapshot:u_g07|*} \
  {*|ProcL_full_snapshot_experiment:b_ProcL|cell_MC10173_snapshot:u_g06|*} \
  {*|ProcL_full_snapshot_experiment:b_ProcL|cell_MC10173_snapshot:u_g07|*}]]
 set originals [dict create]
 set duplicates [dict create]
 foreach_in_collection node $visible {
  set name [get_register_info -name $node]
  if {![regexp {\|Proc([HL])_full_snapshot_experiment:b_Proc([HL])\|cell_MC10173_snapshot:u_(g0[67])\|q\[([0-3])\](~DUPLICATE[0-9]*)?$} $name match board alias package bit duplicate]} {
   error "Unclassified visible R register: $name"
  }
  if {$board!=$alias} {error "Ambiguous visible R owner: $name"}
  set key "$board/$package/$bit"
  if {$duplicate==""} {dict set originals $key $name} else {dict set duplicates $name $key}
  puts "PHYSICAL_NATIVE_VISIBLE_R $corner $name"
 }
 foreach board {H L} {foreach package {g06 g07} {foreach bit {0 1 2 3} {
  if {![dict exists $originals "$board/$package/$bit"]} {error "Missing canonical visible R bit $board/$package/$bit"}
 }}}
 dict for {name key} $duplicates {
  if {![dict exists $originals $key]} {error "Visible R copy has no original: $name"}
 }
 if {[dict size $originals]!=16} {error "Expected 16 original visible R owners"}
 foreach {bank label} {rm_h rm-h stk_h stk-h rm_l rm-l stk_l stk-l} {
  set ram [get_keepers "*|dorado_r_file_ports:files|altdpram:${bank}_rtl_0|*"]
  if {[get_collection_size $ram]==0} {error "Missing physical SRAM bank $bank"}
  puts "PHYSICAL_NATIVE_RAM $corner $bank keepers=[get_collection_size $ram]"
  foreach mode {setup hold} {
   report_timing -$mode -from $ram -to $central -npaths 20 -detail full_path -file physical-native-$label-read-$mode-$corner.rpt
   report_timing -$mode -from $central -to $ram -npaths 20 -detail full_path -file physical-native-$label-write-$mode-$corner.rpt
  }
 }
 foreach {label from to} [list internal $central $central input $rest $central \
   output $central $rest history $all $history visible-in $all $visible visible-out $visible $all] {
  foreach mode {setup hold} {
   report_timing -$mode -from $from -to $to -npaths 32 -detail full_path -file physical-native-$label-$mode-$corner.rpt
  }
 }
 report_ucp -file physical-native-ucp-$corner.rpt
 delete_timing_netlist
}
project_close
