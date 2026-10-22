# Read-only path inventory for the optional opposite-edge address sample.
# Report both physical half-cycle transfers. Do not add timing exceptions.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
    lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners] != 4} {error "Expected four operating conditions"}
foreach name $corners {
    create_timing_netlist
    set_operating_conditions $name
    read_sdc
    update_timing_netlist
    set sampled [get_registers -nowarn {*|dorado_storage_group:u_storage_*|addr_before_host*}]
    set saved [get_registers -nowarn {*|dorado_storage_group:u_storage_*|saved_addr*}]
    if {[get_collection_size $sampled] < 1 || [get_collection_size $saved] < 1} {
        error "Missing sampled/saved storage address registers"
    }
    puts "STORAGE_SAMPLE_CORNER $name [get_collection_size $sampled] [get_collection_size $saved]"
    foreach_in_collection r $sampled {puts "STORAGE_SAMPLE_REGISTER [get_node_info -name $r]"}
    foreach mode {setup hold} {
        report_timing -$mode -to $sampled -npaths 256 -detail full_path -file storage-sample-input-$mode-$name.rpt
        report_timing -$mode -from $sampled -to $saved -npaths 256 -detail full_path -file storage-sample-output-$mode-$name.rpt
    }
    delete_timing_netlist
}
project_close
