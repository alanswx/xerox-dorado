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

# Native15 exposes the same F10145A write-address transfer already audited
# for the checkpoint cores (checkpoint_timing.sdc). In cell_F10145A:
#   a_lat changes only while !p13;
#   mem[a_lat] writes only while p13 && !we_d && ce_lat.
# Address capture and write commit therefore cannot occur on the same edge.
# Exclude ONLY the reported instances' own write-address HOLD paths.
# Their next-edge setup, asynchronous read addresses, data and enables stay
# checked. Slow-corner failures before review: j11 -0.106 ns, j09 -0.094 ns,
# k06 -0.015 ns. Native16 adds MemD h04, -0.055 ns slow / -0.001 ns fast,
# with the identical cell and mutually exclusive enables. The RBF is
# unchanged by these constraint reviews.
foreach {board instance position} {
    DskEth_m_Rev_m_Cf b_DskEth j11
    DskEth_m_Rev_m_Cf b_DskEth j09
    MemC_m_Rev_m_Be b_MemC k06
    MemD_m_Rev_m_Ca b_MemD h04
} {
    set ram_scope [format {*|dorado_native_machine:u_native|dorado_native_backplane:m|%s:%s|cell_F10145A:u_%s|} $board $instance $position]
    set address_source [get_registers -nowarn "${ram_scope}a_lat*"]
    set address_sink [get_registers -nowarn "${ram_scope}altdpram:mem_rtl_0|*|*~OBSERVABLEPORTAADDRESSREGOUT*"]
    if {[get_collection_size $address_source] && [get_collection_size $address_sink]} {
        set_false_path -hold -from $address_source -to $address_sink
    }
}

# Native19: two -0.050 ns slow-corner paths from MemX e20 a_lat[1]
# to that same F10145A's two observable RAM write-address bit1 endpoints.
# cell_F10145A captures a_lat only while !p13 and commits a write only
# while p13 && !we_d && ce_lat. Same-edge capture/commit is impossible.
# Exclude these two HOLD transfers only. Setup, reads, data, and enables
# remain timed. Preserve before-review reports; the RBF is unchanged.
set e20_source [get_registers -nowarn {*|dorado_native_machine:u_native|dorado_native_backplane:m|MemX_m_Rev_m_Ch:b_MemX|cell_F10145A:u_e20|a_lat[1]}]
set e20_sink [get_registers -nowarn {*|dorado_native_machine:u_native|dorado_native_backplane:m|MemX_m_Rev_m_Ch:b_MemX|cell_F10145A:u_e20|altdpram:mem_rtl_0|*|*~OBSERVABLEPORTAADDRESSREGOUT1}]
if {[get_collection_size $e20_source] != 1 || [get_collection_size $e20_sink] != 2} {
    error "Native19 e20 hold review must match exactly1 source and2 sinks"
}
set_false_path -hold -from $e20_source -to $e20_sink
