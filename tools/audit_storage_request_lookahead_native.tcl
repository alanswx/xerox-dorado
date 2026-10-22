# Read-only checks for the new registered selection and ready comparison.
# Incoming/outgoing endpoints stay unfiltered where a live producer or
# address-preview latch may sit outside the nominal storage-group hierarchy.
project_open Dorado
set outdir [file dirname [info script]]
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
    set look [get_registers {*|dorado_storage_arb:u_arb|look_*}]
    set arb [get_registers {*|dorado_storage_arb:u_arb|*}]
    set backend [get_registers {*|dorado_storage_sdram_burst:u_sdram|*}]
    set groups [get_registers {*|dorado_storage_group:u_storage_*|*}]
    foreach {label nodes} [list lookahead $look arbiter $arb backend $backend groups $groups] {
        if {[get_collection_size $nodes]<1} {error "Missing request-lookahead family: $label"}
        puts "LOOK_FAMILY_COUNT $corner $label [get_collection_size $nodes]"
        foreach_in_collection node $nodes {
            puts "LOOK_FAMILY_REGISTER $corner $label [get_register_info -name $node]"
        }
    }
    foreach mode {setup hold} {
        report_timing -$mode -to $look -npaths 256 -detail full_path -file [file join $outdir look-selection-$mode-$corner.rpt]
        report_timing -$mode -from $look -to $backend -npaths 128 -detail full_path -file [file join $outdir look-cached-response-$mode-$corner.rpt]
        report_timing -$mode -to $arb -npaths 256 -detail full_path -file [file join $outdir look-admission-$mode-$corner.rpt]
        report_timing -$mode -to $groups -npaths 256 -detail full_path -file [file join $outdir look-group-ready-$mode-$corner.rpt]
        report_timing -$mode -from $look -npaths 256 -detail full_path -file [file join $outdir look-outgoing-$mode-$corner.rpt]
    }
    delete_timing_netlist
}
project_close
