# Read-only timing/inventory for the scaled original BaseBoard crystal.
# Do not add exceptions, derived clocks, or change the active project.
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
    set oscillator [get_registers {*|cell_MC12061:u_d08|*}]
    set baseboard [get_registers {*b_BaseBd*}]
    set divide5 [get_registers {*|cell_SN74LS163:u_e07|*}]
    set divide2 [get_registers {*|cell_SN74LS74:u_g08|*}]
    foreach {label nodes} [list oscillator $oscillator baseboard $baseboard divide5 $divide5 divide2 $divide2] {
        if {[get_collection_size $nodes]<1} {error "Missing BaseBoard crystal family: $label"}
        puts "CRYSTAL_FAMILY_COUNT $corner $label [get_collection_size $nodes]"
        foreach_in_collection node $nodes {
            puts "CRYSTAL_FAMILY_REGISTER $corner $label [get_register_info -name $node]"
        }
    }
    foreach mode {setup hold} {
        report_timing -$mode -to $oscillator -npaths 128 -detail full_path -file [file join $outdir crystal-oscillator-$mode-$corner.rpt]
        report_timing -$mode -from $oscillator -to $baseboard -npaths 128 -detail full_path -file [file join $outdir crystal-downstream-$mode-$corner.rpt]
        # Keep incoming paths unfiltered, including the previous critical
        # MemC-to-RIOT crossing. A faster local oscillator must not hide it.
        report_timing -$mode -to $baseboard -npaths 256 -detail full_path -file [file join $outdir crystal-baseboard-$mode-$corner.rpt]
    }
    delete_timing_netlist
}
project_close
