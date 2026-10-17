# ContA a04 is cell_F10145A (16x4 RAM). Its a_lat register updates only
# while !p13. The RAM writes only when p13 && !we_d && ce_lat, so the
# source cannot change on an enabled write edge. A same-edge hold check
# from a_lat to this RAM's write-address register is functionally false.
# The next-edge setup check remains necessary and is NOT excluded.
# Review this exception if cell_F10145A's capture/write enables change.
# Scope to this checkpoint instance and its internal WRITE address pins;
# do not exclude read-address paths or any other RAM/control signals.
set address_source [get_registers -nowarn {*|checkpoint_m_b_ContA:b_ContA|checkpoint_m_b_ContA_u_a04:u_a04|a_lat*}]
set address_sink [get_registers -nowarn {*|checkpoint_m_b_ContA:b_ContA|checkpoint_m_b_ContA_u_a04:u_a04|altdpram:mem_rtl_0|*|*~OBSERVABLEPORTAADDRESSREGOUT*}]
if {[get_collection_size $address_source] && [get_collection_size $address_sink]} {
    set_false_path -hold -from $address_source -to $address_sink
}

# The Invaders fit exposes the same cell_F10145A path in MemC d06 (base
# register low word) and MemX e20 (per-task tag). Both instances use the
# identical !p13 address-capture / p13 write-commit enables audited above.
# The slow 100C model reports six -0.041 ns paths here; these cannot change
# address on an enabled write edge. Keep each source paired with ONLY its
# own RAM's write-address pins, and retain setup and read-address checks.
# The mount/network fit additionally exposes ContA c04 (TLink RAM): four
# slow-corner paths from a_lat[1] to its own write-address register, -0.054
# ns each. Its generated F10145A uses the identical mutually exclusive
# capture/commit enables. The same hold-only exclusion applies; setup and
# the asynchronous read address remain timed.
# The selectable-station fit reports four -0.039 ns paths from DskEth
# j13 a_lat[1] to its own write-address registers. j13 is the same
# F10145A with mutually exclusive !p13 capture / p13 commit enables.
# Only that instance's write-address hold checks are excluded.
foreach {board position} {MemC d06 MemX e20 ContA c04 DskEth j13} {
    set ram_scope [format {*|checkpoint_m_b_%s:b_%s|checkpoint_m_b_%s_u_%s:u_%s|} $board $board $board $position $position]
    set address_source [get_registers -nowarn "${ram_scope}a_lat*"]
    set address_sink [get_registers -nowarn "${ram_scope}altdpram:mem_rtl_0|*|*~OBSERVABLEPORTAADDRESSREGOUT*"]
    if {[get_collection_size $address_source] && [get_collection_size $address_sink]} {
        set_false_path -hold -from $address_source -to $address_sink
    }
}
