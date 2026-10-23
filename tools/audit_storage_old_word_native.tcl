# Read-only physical checks of accepted-write qualifier and old-word epochs.
# Existing clocks/SDC are read unchanged; this script adds no exception.
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
    set words [get_registers {*|dorado_storage_group:u_storage_*|old_data*}]
    set pipes [get_registers {*|dorado_storage_group:u_storage_*|read_pipe*}]
    set qualifiers [get_registers {*|dorado_storage_group:u_storage_*|preserve_old_word*}]
    foreach {label nodes} [list word $words pipe $pipes qualifier $qualifiers] {
        if {[get_collection_size $nodes]<1} {error "Missing old-word register family: $label"}
        puts "OLD_WORD_FAMILY_COUNT $corner $label [get_collection_size $nodes]"
        foreach_in_collection node $nodes {
            puts "OLD_WORD_FAMILY_REGISTER $corner $label [get_register_info -name $node]"
        }
    }
    foreach mode {setup hold} {
        report_timing -$mode -to $words -npaths 256 -detail full_path -file [file join $outdir old-word-input-$mode-$corner.rpt]
        report_timing -$mode -from $words -to $pipes -npaths 256 -detail full_path -file [file join $outdir old-word-output-$mode-$corner.rpt]
        report_timing -$mode -to $qualifiers -npaths 128 -detail full_path -file [file join $outdir old-word-qualifier-$mode-$corner.rpt]
    }
    delete_timing_netlist
}
project_close
