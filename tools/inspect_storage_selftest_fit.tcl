# Read-only fitted register inventory; no constraints or database changes.
project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
foreach {label pattern} {
 observer {*|dorado_boot_fault_monitor:u_boot_fault_monitor|*}
 selftest {*|dorado_storage_selftest:u_initialize|*}
 response {*|dorado_storage_sdram_burst:u_sdram|rsp_data*}
} {
 set group [get_registers $pattern]
 puts "SELFTEST_FIT_COUNT $label [get_collection_size $group]"
 foreach_in_collection node $group {
  puts "SELFTEST_FIT_REGISTER $label [get_register_info -name $node]"
 }
}
delete_timing_netlist
project_close
