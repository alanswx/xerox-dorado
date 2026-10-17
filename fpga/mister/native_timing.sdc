# Native4 reports three hold paths from a storage group's write_data to
# its own saved_data. These registers cannot transfer on an edge where
# write_data changes (dorado_storage_group.sv):
#
# * write_data changes only on a machine clock edge with a new DRAM write.
# * A new write implies write_pending=0: while write_pending=1, cas_d and
#   we_d are both zero, preventing another write-data capture.
# * The machine clock advances only while ready=1, which requires hit=1.
# * With hit=1 and write_pending=0, req_valid=0, so saved_data is disabled.
#
# Retain next-edge setup, data-to-other-group, and request/enable timing.
# Exclude only the affected instances' own data hold checks. The simulation
# assertions in dorado_storage_group guard these conditions.
foreach {board group} {msa2Odd 9 msa3 12 msa1Odd 2} {
    set scope [format {*|dorado_native_machine:u_native|dorado_native_backplane:m|msa_ddram:b_%s|dorado_storage_group:u_storage_%s|} $board $group]
    set data_source [get_registers -nowarn "${scope}write_data*"]
    set data_sink [get_registers -nowarn "${scope}saved_data*"]
    if {[get_collection_size $data_source] && [get_collection_size $data_sink]} {
        set_false_path -hold -from $data_source -to $data_sink
    }
}

# Native11: two own-address hold checks have the same disabled-transfer
# property. MK4096P_addr row/col change only on a machine-clock edge.
# Advancing that edge requires ready=1. The group equations make ready and
# req_valid mutually exclusive, so saved_addr cannot capture on that edge.
# Keep address setup, request/enable checks, and all other instances checked.
foreach {board group} {msa2 9 msa3Odd 0} {
    set scope [format {*|dorado_native_machine:u_native|dorado_native_backplane:m|msa_ddram:b_%s|} $board]
    set address_source [get_registers -nowarn [list \
        "${scope}cell_MK4096P_addr:u_dram_addr_${group}|row*" \
        "${scope}cell_MK4096P_addr:u_dram_addr_${group}|col*"]]
    set address_sink [get_registers -nowarn "${scope}dorado_storage_group:u_storage_${group}|saved_addr*"]
    if {[get_collection_size $address_source] && [get_collection_size $address_sink]} {
        set_false_path -hold -from $address_source -to $address_sink
    }
}
