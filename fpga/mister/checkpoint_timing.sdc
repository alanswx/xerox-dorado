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
