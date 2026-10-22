# Read-only timing for the response/request overlap. No SDC or exceptions added.
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
    set backend [get_registers {*|dorado_storage_sdram_burst:u_sdram|*}]
    set groups [get_registers {*|dorado_storage_group:u_storage_*|*}]
    set arbiter [get_registers {*|dorado_storage_arb:u_arb|*}]
    foreach {label nodes} [list backend $backend groups $groups arbiter $arbiter] {
        if {[get_collection_size $nodes]<1} {error "Missing overlap path family: $label"}
        puts "RESPONSE_FAMILY_COUNT $corner $label [get_collection_size $nodes]"
        foreach_in_collection node $nodes {
            puts "RESPONSE_FAMILY_REGISTER $corner $label [get_register_info -name $node]"
        }
    }
    foreach mode {setup hold} {
        # Include all launch registers: the new live request payload bypasses
        # address_latched, and its actual worst source must not be filtered out.
        report_timing -$mode -to $backend -npaths 256 -detail full_path -file [file join $outdir response-backend-$mode-$corner.rpt]
        report_timing -$mode -from $backend -to $groups -npaths 256 -detail full_path -file [file join $outdir response-groups-$mode-$corner.rpt]
        report_timing -$mode -to $arbiter -npaths 256 -detail full_path -file [file join $outdir response-arbiter-$mode-$corner.rpt]
    }
    delete_timing_netlist
}
project_close
