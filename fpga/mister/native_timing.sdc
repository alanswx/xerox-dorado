# A storage group cannot capture its own address/data on a fabric edge
# that changes those source registers. This is a property of every instance
# of dorado_storage_group, independent of its WIDTH or storage board:
#
# 1. MK4096P_addr row/col and write_data change only on machine_clk edges.
# 2. The complete machine advances only when every group's ready is true.
# 3. ready = !reset && !waiting && hit && (!write_pending || write_done).
#    req_valid = !reset && !waiting && (!hit ||
#                                    (write_pending && !write_done)).
#    Therefore ready implies !req_valid.
# 4. saved_addr/saved_data load only when req_valid && req_ready.
#
# Thus same-edge HOLD transfers from a group's own machine-clocked sources
# to its own request registers are impossible. Keep their next-edge SETUP,
# all enable/request paths, and every cross-group path checked. Assertions
# in dorado_storage_group verify ready and !req_valid at each machine edge;
# group and complete-board equivalence check all original storage cycles.
#
# Apply the proof to each exact source/destination pair. Earlier constraints
# listed only instances appearing as violations in Native4/11; Native12
# exposed the same impossible transfer in two more groups. Per-placement
# lists leave the fitter inserting unnecessary hold delays in other groups.
foreach board {msa msaOdd msa1 msa1Odd msa2 msa2Odd msa3 msa3Odd} {
    for {set group 0} {$group < 16} {incr group} {
        set scope [format {*|dorado_native_machine:u_native|dorado_native_backplane:m|msa_ddram:b_%s|} $board]
        set group_scope "${scope}dorado_storage_group:u_storage_${group}|"
        set data_source [get_registers -nowarn "${group_scope}write_data*"]
        set data_sink [get_registers -nowarn "${group_scope}saved_data*"]
        if {[get_collection_size $data_source] && [get_collection_size $data_sink]} {
            set_false_path -hold -from $data_source -to $data_sink
        }
        set address_source [get_registers -nowarn [list \
            "${scope}cell_MK4096P_addr:u_dram_addr_${group}|row*" \
            "${scope}cell_MK4096P_addr:u_dram_addr_${group}|col*"]]
        set address_sink [get_registers -nowarn "${group_scope}saved_addr*"]
        if {[get_collection_size $address_source] && [get_collection_size $address_sink]} {
            set_false_path -hold -from $address_source -to $address_sink
        }
    }
}
