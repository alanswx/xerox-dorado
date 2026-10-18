project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
foreach {family patterns} {
 processor {*|dorado_native_backplane:m|ProcH_m_Rev*|* *|dorado_native_backplane:m|ProcL_m_Rev*|* *|dorado_native_backplane:m|ContA_m_Rev*|* *|dorado_native_backplane:m|ContB_m_Rev*|* *|dorado_native_backplane:m|IFU_m_Rev*|*}
 memory {*|dorado_native_backplane:m|MemC_m_Rev*|* *|dorado_native_backplane:m|MemD_m_Rev*|* *|dorado_native_backplane:m|MemX_m_Rev*|*}
 io {*|dorado_native_backplane:m|DskEth_m_Rev*|* *|dorado_native_backplane:m|DispY_m_Rev*|*}
} {
 set regs [get_registers -nowarn $patterns]
 puts "BOARD_PATH_COLLECTION $family [get_collection_size $regs]"
 if {![get_collection_size $regs]} {error "Empty original-board collection: $family"}
 report_timing -setup -to $regs -npaths 10 -detail full_path -file native31-$family-setup.rpt
}
delete_timing_netlist
project_close
