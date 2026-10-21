# Reporting only, after the native builder is terminal. Never run concurrently
# with another stage in the same Quartus database. Retain original SDC intact.
# Explicitly cover central storage/new evaluator paths that the historical
# per-board crossing reports cannot identify by their former package names.
project_open Dorado
set manifest_file [open ../r-front-ready-native.json r]
set manifest_text [read $manifest_file]
close $manifest_file
set canonical_r [regexp {"native_visible_R_bits": 16} $manifest_text]
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
 set central [get_registers [list {*|dorado_native_backplane:m|r_front_q*} \
   {*|dorado_native_backplane:m|r_front_lower_q*} \
   {*|dorado_native_backplane:m|r_front_upper_q*}]]
 set rest [remove_from_collection [get_registers *] $central]
 set count [get_collection_size $central]
 # Live synthesis may remove physical outputs with no consumers. Record the
 # actual count; the source inventory's 752 bits is not a fitted area claim.
 set maximum_central [expr {$canonical_r ? 736 : 752}]
 if {$count==0 || $count>$maximum_central} {error "Missing/invalid central Q register coverage: $count"}
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
 if {$canonical_r} {
  # These16 bits belong to the retained native latches, not central storage.
  # Cover both their feedback and the MapBuf receiver that exposed the bug.
  set visible_r [get_registers [list \
    {*|ProcH_full_snapshot_experiment:b_ProcH|cell_MC10173_snapshot:u_g06|*} \
    {*|ProcH_full_snapshot_experiment:b_ProcH|cell_MC10173_snapshot:u_g07|*} \
    {*|ProcL_full_snapshot_experiment:b_ProcL|cell_MC10173_snapshot:u_g06|*} \
    {*|ProcL_full_snapshot_experiment:b_ProcL|cell_MC10173_snapshot:u_g07|*}]]
  set r_count [get_collection_size $visible_r]
  set r_primary [dict create]
  set r_copies [dict create]
  set r_physical [dict create]
  foreach_in_collection r_register $visible_r {
   set r_name [get_register_info -name $r_register]
   if {![regexp {\|Proc([HL])_full_snapshot_experiment:b_Proc([HL])\|cell_MC10173_snapshot:u_(g0[67])\|q\[([0-3])\](~DUPLICATE[0-9]*)?$} $r_name whole board alias package bit duplicate]} {
    error "Unclassified native R register: $r_name"
   }
   if {$board!=$alias || [dict exists $r_physical $r_name]} {error "Ambiguous native R owner: $r_name"}
   dict set r_physical $r_name 1
   set r_key "$board/$package/$bit"
   if {$duplicate==""} {dict set r_primary $r_key $r_name} else {dict set r_copies $r_name $r_key}
   puts "R_FRONT_READY_R_REGISTER $corner $r_name"
  }
  foreach board {H L} {foreach package {g06 g07} {foreach bit {0 1 2 3} {
   if {![dict exists $r_primary "$board/$package/$bit"]} {error "Missing canonical native R bit $board/$package/$bit"}
  }}}
  dict for {r_name r_key} $r_copies {
   if {![dict exists $r_primary $r_key]} {error "Native R copy has no original owner: $r_name"}
  }
  if {[dict size $r_primary]!=16 || [dict size $r_physical]!=$r_count} {error "Native R coverage disagrees"}
  puts "R_FRONT_READY_NATIVE_R $corner registers=$r_count primary=16 replicas=[dict size $r_copies]"
  set memx [get_registers {*|MemX_full_snapshot_experiment:b_MemX|*}]
  foreach mode {setup hold} {
   report_timing -$mode -from $visible_r -to $central -npaths 16 -detail full_path -file r-front-ready-native-r-in-$mode-$corner.rpt
   report_timing -$mode -from $central -to $visible_r -npaths 16 -detail full_path -file r-front-ready-native-r-out-$mode-$corner.rpt
   report_timing -$mode -from $visible_r -to $memx -npaths 16 -detail full_path -file r-front-ready-native-r-mapbuf-$mode-$corner.rpt
  }
 }
 delete_timing_netlist
}
project_close
